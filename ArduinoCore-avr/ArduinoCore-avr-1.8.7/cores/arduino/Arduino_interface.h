#ifndef _ARDUINO_INTERFACE_H_
#define _ARDUINO_INTERFACE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

extern SPI_HandleTypeDef *g_hspi;
extern TIM_HandleTypeDef *g_htim;
extern UART_HandleTypeDef *g_huart;
extern I2C_HandleTypeDef *g_hi2c;

void arduino_serial_init(UART_HandleTypeDef *huart);
void arduino_spi_init(SPI_HandleTypeDef *hspi);
void arduino_tim_init(TIM_HandleTypeDef *htim);
void arduino_hi2c_init(I2C_HandleTypeDef *hi2c);

#ifdef __cplusplus
}
#endif

#endif
