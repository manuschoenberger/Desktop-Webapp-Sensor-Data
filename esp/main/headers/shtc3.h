#ifndef SHTC3_H
#define SHTC3_H

#include <stdint.h>
#include "driver/i2c.h"
#include "driver/gpio.h"

#define SHTC3_ADDR 0x70
#define I2C_MASTER_NUM I2C_NUM_0

void shtc3_init();
void shtc3_writeRegister(uint16_t cmd);
void shtc3_readRegister(float *pTemp, float *pHum);
void shtc3_getValues(float *pTemp, float *pHum);

#endif