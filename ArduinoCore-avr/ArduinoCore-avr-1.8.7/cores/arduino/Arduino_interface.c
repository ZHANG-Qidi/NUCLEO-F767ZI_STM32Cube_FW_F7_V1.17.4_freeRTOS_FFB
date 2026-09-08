#include "Arduino_interface.h"

#include <sys/types.h>

SPI_HandleTypeDef *g_hspi;
TIM_HandleTypeDef *g_htim;
UART_HandleTypeDef *g_huart;
I2C_HandleTypeDef *g_hi2c;

uint64_t g_TIM_CHANNEL_U;
uint64_t g_TIM_CHANNEL_V;
uint64_t g_TIM_CHANNEL_W;

void arduino_serial_init(UART_HandleTypeDef *huart) { g_huart = huart; }

void arduino_spi_init(SPI_HandleTypeDef *hspi) { g_hspi = hspi; }

void arduino_tim_init(TIM_HandleTypeDef *htim, uint64_t TIM_CHANNEL_U, uint64_t TIM_CHANNEL_V, uint64_t TIM_CHANNEL_W) {
    g_htim = htim;
    g_TIM_CHANNEL_U = TIM_CHANNEL_U;
    g_TIM_CHANNEL_V = TIM_CHANNEL_V;
    g_TIM_CHANNEL_W = TIM_CHANNEL_W;
    SystemCoreClockUpdate();
#if defined(STM32F7) || defined(STM32H7)
    DWT->LAR = 0xC5ACCE55;
#endif
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    HAL_TIM_PWM_Start(g_htim, g_TIM_CHANNEL_U);
    HAL_TIM_PWM_Start(g_htim, g_TIM_CHANNEL_V);
    HAL_TIM_PWM_Start(g_htim, g_TIM_CHANNEL_W);
}

void arduino_hi2c_init(I2C_HandleTypeDef *hi2c) { g_hi2c = hi2c; }

int __io_putchar(int ch) {
    HAL_UART_Transmit(g_huart, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}
