#ifndef _FOC_SETUP_H_
#define _FOC_SETUP_H_
#include "SimpleFOC.h"
#ifdef __cplusplus
extern "C" {
#endif
#define BLDC_MOTOR_PP (7)
#define VOLTAGE_POWER (9.0f)
#define VOLTAGE_LIMIT (6.0f)
#define VOLTAGE_SENSOR_ALIGN (1.0f)
#define MOTOR_U (0)
#define MOTOR_V (1)
#define MOTOR_W (2)
#define MOTOR_EN (PIN("F", 14))
#define SPI_MASTER_CS_IO (PIN("D", 14))
#define COMMANDER_BAUD_RATE (115200)
extern BLDCMotor motor;
extern MagneticSensorSPI sensor;
extern void foc_setup(void);
#ifdef __cplusplus
}
#endif
#endif
