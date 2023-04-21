#ifndef __SHT30_H
#define __SHT30_H

#include "stm8s.h"
#include "delay.h"

// SHT30 command definitions
#define SHT30_MEAS_HIGHREP 0x2400

void SHT30_Init(uint8_t address1);
void SHT30_ReadRegister(uint8_t adress_of_slave, uint8_t *msb_temperature, uint8_t *lsb_temperature, uint8_t *msb_humidity, uint8_t *lsb_humidity);
// float SHT30_ReadTemp(uint8_t address);
// float SHT30_ReadHumidity(uint8_t address);

#endif