#ifndef _ESP_LOG_H_
#define _ESP_LOG_H_
#include "Arduino.h"
#ifdef __cplusplus
extern "C" {
#endif
#define ESP_LOGI(tag, format, ...)                                                                          \
    do {                                                                                                    \
        char _log_buf[512];                                                                                 \
        snprintf(_log_buf, sizeof(_log_buf), "[I] (%lu) %s: " format "\r\n", millis(), tag, ##__VA_ARGS__); \
        Serial.print(_log_buf);                                                                             \
    } while (0)
#ifdef __cplusplus
}
#endif
#endif
