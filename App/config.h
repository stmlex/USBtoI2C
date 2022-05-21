#ifndef CONFIG_H
#define CONFIG_H

#include "i2c.h"

#define LOG_LIBS_PRINT_CUSTOM
#define LOG_LIBS_PRINT_UART
#define LOG_LIBS_USE_PRINTF 2

#define SYSTEM_LED_BLINK_PEROD 500 // ms
#define READ_COMMAND_BUF_LEN 32

#define I2C_INSTANSE (hi2c1)
#define I2C_MAX_DATA_SIZE 200

#endif // !CONFIG_H