#ifndef __LM75A_I2C_H
#define __LM75A_I2C_H

#include "stm8s.h"
#include "delay.h"

// LM75A registers
#define LM75A_TEMP_REG 0x00 // Temperature register - (Read only) two 8-bit data
#define LM75A_CONF_REG 0x01 // Configuration register - (Read / Write) single 8-bit, default =0
#define LM75A_THYS_REG 0x02 // Hysteresis register - (Read / Write) two 8-bit, default = 75 °C
#define LM75A_TOS_REG 0x03  // Overtemperature shutdown threshold register (Read / Write) two 8-bit, default = 80 °C

// LM75A configuration bits
#define LM75A_CONF_SD 0x01
#define LM75A_CONF_OS 0x02
#define LM75A_CONF_COMP 0x04
#define LM75A_CONF_POL 0x08

///////////////////// PUBLIC FUNCTIONS //////////////////////////////

void LM75A_Init(uint8_t address1);
void LM75A_ReadTemperature(uint8_t adress_of_lm, uint8_t *data);
#endif