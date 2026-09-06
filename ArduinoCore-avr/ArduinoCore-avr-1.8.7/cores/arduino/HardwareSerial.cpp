/*
  HardwareSerial.cpp - Hardware serial library for Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified 23 November 2006 by David A. Mellis
  Modified 28 September 2010 by Mark Sproul
  Modified 14 August 2012 by Alarus
  Modified 3 December 2013 by Matthijs Kooijman
*/

#include "HardwareSerial.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Arduino.h"
#include "HardwareSerial_private.h"
#include "arduino_interface.h"

// Actual interrupt handlers //////////////////////////////////////////////////////////////

void HardwareSerial::_rx_complete_irq(void) {
    // room
    unsigned char c = rx_byte;
    rx_buffer_index_t i = (unsigned int)(_rx_buffer_head + 1) % SERIAL_RX_BUFFER_SIZE;

    // if we should be storing the received character into the location
    // just before the tail (meaning that the head would advance to the
    // current location of the tail), we're about to overflow the buffer
    // and so we don't write the character or advance the head.
    if (i != _rx_buffer_tail) {
        _rx_buffer[_rx_buffer_head] = c;
        _rx_buffer_head = i;
    }
    HAL_UART_Receive_IT(huart, &rx_byte, 1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == g_huart) {
        Serial._rx_complete_irq();
    }
}

void HardwareSerial::_tx_udr_empty_irq(void) {
    if (_tx_buffer_head != _tx_buffer_tail) {
        // If interrupts are enabled, there must be more data in the output
        // buffer. Send the next byte
        unsigned char c = _tx_buffer[_tx_buffer_tail];
        _tx_buffer_tail = (_tx_buffer_tail + 1) % SERIAL_TX_BUFFER_SIZE;
        HAL_UART_Transmit_IT(huart, &c, 1);
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == g_huart) {
        Serial._tx_udr_empty_irq();
    }
}

// Public Methods //////////////////////////////////////////////////////////////

void HardwareSerial::begin(unsigned long baud, byte config) {
    _written = false;
    HAL_UART_Receive_IT(huart, &rx_byte, 1);
}

void HardwareSerial::end() {}

int HardwareSerial::available(void) { return ((unsigned int)(SERIAL_RX_BUFFER_SIZE + _rx_buffer_head - _rx_buffer_tail)) % SERIAL_RX_BUFFER_SIZE; }

int HardwareSerial::peek(void) {
    if (_rx_buffer_head == _rx_buffer_tail) {
        return -1;
    } else {
        return _rx_buffer[_rx_buffer_tail];
    }
}

int HardwareSerial::read(void) {
    // if the head isn't ahead of the tail, we don't have any characters
    if (_rx_buffer_head == _rx_buffer_tail) {
        return -1;
    } else {
        unsigned char c = _rx_buffer[_rx_buffer_tail];
        _rx_buffer_tail = (rx_buffer_index_t)(_rx_buffer_tail + 1) % SERIAL_RX_BUFFER_SIZE;
        return c;
    }
}

int HardwareSerial::availableForWrite(void) {
    tx_buffer_index_t head;
    tx_buffer_index_t tail;

    head = _tx_buffer_head;
    tail = _tx_buffer_tail;
    if (head >= tail) return SERIAL_TX_BUFFER_SIZE - 1 - head + tail;
    return tail - head - 1;
}

void HardwareSerial::flush() {}

size_t HardwareSerial::write(uint8_t c) {
    _written = true;
    // If the buffer and the data register is empty, just write the byte
    // to the data register and be done. This shortcut helps
    // significantly improve the effective datarate at high (>
    // 500kbit/s) bitrates, where interrupt overhead becomes a slowdown.
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_TC) == RESET) {
        tx_buffer_index_t i = (_tx_buffer_head + 1) % SERIAL_TX_BUFFER_SIZE;
        // If the output buffer is full, there's nothing for it other than to
        // wait for the interrupt handler to empty it a bit
        while (i == _tx_buffer_tail) {
            delayMicroseconds(1);
        }
        _tx_buffer[_tx_buffer_head] = c;
        _tx_buffer_head = i;
    } else {
        HAL_UART_Transmit_IT(huart, &c, 1);
    }
    return 1;
}

HardwareSerial Serial(g_huart);
