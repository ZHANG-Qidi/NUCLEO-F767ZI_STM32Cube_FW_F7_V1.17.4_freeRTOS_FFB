#include "Arduino_HAL.h"

#include "Arduino.h"
#include "Arduino_interface.h"
#include "FreeRTOS.h"
#include "main.h"
#include "task.h"

// delayMicroseconds using DWT cycle counter
void delayMicroseconds(unsigned int us) {
    uint32_t start = DWT->CYCCNT;
    uint32_t cycles = (SystemCoreClock / 1000000) * us;
    while ((DWT->CYCCNT - start) < cycles) {
    }
}

// delay with FreeRTOS awareness
void delay(unsigned long ms) {
    if (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED) {
        HAL_Delay(ms);
    } else {
        vTaskDelay(pdMS_TO_TICKS(ms));
    }
}

// millis() using DWT
unsigned long millis(void) { return (unsigned long)(DWT->CYCCNT / (SystemCoreClock / 1000000) / 1000); }

// micros() using DWT
unsigned long micros(void) { return (unsigned long)(DWT->CYCCNT / (SystemCoreClock / 1000000)); }

// digitalWrite using pin number encoding (port = pin/16, bit = pin%16)
void digitalWrite(uint8_t pin, uint8_t val) {
    GPIO_PinState pin_val = (val == HIGH) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    GPIO_TypeDef *pin_port;
    uint16_t pin_pin = 1 << (pin % 16);

    switch (pin / 16) {
        case 0:
            pin_port = GPIOA;
            break;
        case 1:
            pin_port = GPIOB;
            break;
        case 2:
            pin_port = GPIOC;
            break;
        case 3:
            pin_port = GPIOD;
            break;
        case 4:
            pin_port = GPIOE;
            break;
        case 5:
            pin_port = GPIOF;
            break;
        default:
            pin_port = GPIOA;
            break;
    }

    HAL_GPIO_WritePin(pin_port, pin_pin, pin_val);
}

// pinMode dummy (not implemented)
void pinMode(uint8_t pin, uint8_t mode) {
    (void)pin;
    (void)mode;
}

// analogWrite using TIM PWM
void analogWrite(uint8_t pin, int val) {
    if (val < 0) val = 0;
    if (val > 255) val = 255;

    uint32_t arr = __HAL_TIM_GET_AUTORELOAD(g_htim);
    uint32_t duty = (uint32_t)val * (arr + 1) / 255.0f;

    switch (pin) {
        case 0:
            __HAL_TIM_SET_COMPARE(g_htim, g_TIM_CHANNEL_U, duty);
            break;
        case 1:
            __HAL_TIM_SET_COMPARE(g_htim, g_TIM_CHANNEL_V, duty);
            break;
        case 2:
            __HAL_TIM_SET_COMPARE(g_htim, g_TIM_CHANNEL_W, duty);
            break;
        default:
            break;
    }
}
