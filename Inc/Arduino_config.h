#ifndef _ARDUINO_CONFIG_H_
#define _ARDUINO_CONFIG_H_

// SPI master used by the magnetic encoder.
#define CONFIG_ARDUINO_SPI_CS0 (PIN("D", 14))

// FOC driver enable output. The phase values below are timer channel indices,
// not GPIO numbers.
#define CONFIG_FOC_MOTOR_U 0
#define CONFIG_FOC_MOTOR_V 1
#define CONFIG_FOC_MOTOR_W 2
#define CONFIG_FOC_MOTOR_EN (PIN("F", 14))

#endif
