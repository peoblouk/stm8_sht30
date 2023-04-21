#include "sht30.h"

// static int data_bme[2];

void SHT30_Init(uint8_t address1)
{
    I2C_DeInit();
    GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST);
    GPIO_Init(GPIOB, GPIO_PIN_4, GPIO_MODE_OUT_PP_HIGH_FAST);
    I2C_Init(100000, address1 << 1, I2C_DUTYCYCLE_2, I2C_ACK_CURR, I2C_ADDMODE_7BIT, CLK_GetClockFreq() / 1000000);
    // I2C_Init(100000, address2 << 1, I2C_DUTYCYCLE_2, I2C_ACK_CURR, I2C_ADDMODE_7BIT, CLK_GetClockFreq() / 1000000);

    I2C_Cmd(ENABLE);
    delay_ms(500);
}

// Read temperature data from LM75A
void SHT30_ReadRegister(uint8_t adress_of_slave, uint8_t *msb_temperature, uint8_t *lsb_temperature, uint8_t *msb_humidity, uint8_t *lsb_humidity)
{
    uint8_t _address_of_slave = adress_of_slave << 1; // Přepočet na 7bit
    uint8_t command[2] = {0x2C, 0x06};
    uint8_t sht30_buffer[6];

    //! 1.
    I2C_GenerateSTART(ENABLE); // Vygeneruj start
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT))
        ;                                                     // Počkej na vlajku
    I2C_Send7bitAddress(_address_of_slave, I2C_DIRECTION_TX); // Pošli adresu slave
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
        ;
    //! 2.
    I2C_SendData(command[0]); // Žádost o čtení
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        ;
    I2C_SendData(command[1]); // Žádost o čtení
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        ;
    I2C_GenerateSTOP(ENABLE); // Vygeneruj stop
    delay_ms(30);
    ///////////////////////////////////////////////////////////////////
    //! 3.
    I2C_GenerateSTART(ENABLE); // Vygeneruj start
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT))
        ;
    I2C_Send7bitAddress(_address_of_slave, I2C_DIRECTION_RX);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
        ;
    I2C_AcknowledgeConfig(DISABLE); // Zapni ACK
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED))
        ;
    sht30_buffer[0] = I2C_ReceiveData();
    I2C_AcknowledgeConfig(ENABLE);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED))
        ;
    sht30_buffer[1] = I2C_ReceiveData();
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED))
        ;
    sht30_buffer[2] = I2C_ReceiveData();
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED))
        ;
    sht30_buffer[3] = I2C_ReceiveData();
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED))
        ;
    sht30_buffer[4] = I2C_ReceiveData();
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED))
        ;
    sht30_buffer[5] = I2C_ReceiveData();
    I2C_GenerateSTOP(ENABLE);

    //! Calculation
    /////
    // uint16_t raw_temperature = ((uint16_t)sht30_buffer[0] << 8) | sht30_buffer[1];
    // uint16_t raw_humidity = ((uint16_t)sht30_buffer[3] << 8) | sht30_buffer[4];

    // raw_temperature = -45 + 175 * raw_temperature;
    // raw_humidity = 100 * raw_humidity;
    // float temperature_ = -45 + 175 * (float)raw_temperature / 65535.0;
    // float humidity_ = 100 * (float)raw_humidity / 65535.0;

    // *msb_temperature = ((raw_temperature) / 10);
    // *lsb_temperature = (raw_temperature % 10);

    // *lsb_humidity = (raw_humidity / 10);
    // *msb_humidity = (raw_humidity % 10);
    /////
    uint16_t raw_temperature_data = (sht30_buffer[0] << 8) | sht30_buffer[1];
    uint16_t temperature_uint = ((uint16_t)raw_temperature_data + 32768);

    uint16_t raw_humidity_data = (sht30_buffer[2] << 8) | sht30_buffer[3];
    uint16_t humidity_uint = (uint16_t)raw_humidity_data;

    *msb_temperature = (temperature_uint >> 8) & 0xFF;
    *lsb_temperature = temperature_uint & 0xFF;
    *msb_humidity = (humidity_uint >> 8) & 0xFF;
    *lsb_humidity = humidity_uint & 0xFF;
}