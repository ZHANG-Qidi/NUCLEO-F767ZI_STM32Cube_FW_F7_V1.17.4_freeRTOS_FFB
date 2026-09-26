#ifndef _ARDUINO_INTERFACE_H_
#define _ARDUINO_INTERFACE_H_

#include "Arduino_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

extern SPI_HandleTypeDef *g_hspi;
extern TIM_HandleTypeDef *g_htim;
extern UART_HandleTypeDef *g_huart;
extern I2C_HandleTypeDef *g_hi2c;
extern ADC_HandleTypeDef *g_hadc;

extern uint64_t g_TIM_CHANNEL_U;
extern uint64_t g_TIM_CHANNEL_V;
extern uint64_t g_TIM_CHANNEL_W;

void arduino_serial_init(UART_HandleTypeDef *huart);
void arduino_spi_init(SPI_HandleTypeDef *hspi);
void arduino_tim_init(TIM_HandleTypeDef *htim, uint64_t TIM_CHANNEL_U, uint64_t TIM_CHANNEL_V, uint64_t TIM_CHANNEL_W);
void arduino_hi2c_init(I2C_HandleTypeDef *hi2c);
void arduino_hadc_init(ADC_HandleTypeDef *hadc);

#define SPI_MASTER_CS_IO (CONFIG_ARDUINO_SPI_CS0)

#ifdef __cplusplus
}
#endif

#endif
