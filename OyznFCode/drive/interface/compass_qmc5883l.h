#ifndef COMPASS_QMC5883L_H
#define COMPASS_QMC5883L_H

#include <stdbool.h>
#include <math.h>
#include "main.h"
#include "sys.h"
#include "sensors_types.h"

#define QMC5883L_DECLINATION_ANGLE	93.67/1000	//turkey
#define QMC5883L_MAG_I2C_ADDRESS 0x0D<<1

// Registers
#define QMC5883L_REG_CONF1 0x09
#define QMC5883L_REG_CONF2 0x0A

// data output rates for 5883L
#define QMC5883L_ODR_10HZ  (0x00 << 2)
#define QMC5883L_ODR_50HZ  (0x01 << 2)
#define QMC5883L_ODR_100HZ (0x02 << 2)
#define QMC5883L_ODR_200HZ (0x03 << 2)

// Sensor operation modes
#define QMC5883L_MODE_STANDBY    0x00
#define QMC5883L_MODE_CONTINUOUS 0x01

#define QMC5883L_RNG_2G (0x00 << 4)
#define QMC5883L_RNG_8G (0x01 << 4)

#define QMC5883L_OSR_512 (0x00 << 6)
#define QMC5883L_OSR_256 (0x01 << 6)
#define QMC5883L_OSR_128 (0x10 << 6)
#define QMC5883L_OSR_64  (0x11 << 6)

#define QMC5883L_RST 0x80

#define QMC5883L_REG_DATA_OUTPUT_X 0x00
#define QMC5883L_REG_STATUS 0x06

#define QMC5883L_REG_ID 0x0D
#define QMC5883_ID_VAL 0xFF

#ifndef M_PI 
#define M_PI 3.14159265358979323846264338327950288f 
#endif

//TIMEOUT
#define TM_GLOB 100


bool qmc5883lInit();
bool qmc5883lRead(Axis3i16* magRaw);
bool qmc5883lReadHeading(I2C_HandleTypeDef *hi2c, float *heading);
bool qmc5883lDetect(I2C_HandleTypeDef *hi2c);
uint16_t searchDevice(I2C_HandleTypeDef *hi2c);
#endif
