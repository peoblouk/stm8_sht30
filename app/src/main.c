#include <stdio.h>
#include "stm8s.h"
#include "delay.h"
#include "LCD_I2C.h"
#include "sht30.h"

//! Makra
// Indikační LED
#define SHT30_ADDRESS_1 0x44 // BME280 I2C Address
// #define SHT30_ADDRESS_2 0x45 // BME280 I2C Address

//! Proměnné
// uint16_t mtime_key = 0;                    // Proměnná pro millis
// float humidity, temperature; // Proměnné pro teplotu a vlhkost

//! Uživatelské funkce
void setup(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
    delay_init(); // Incializace časovače TIM4
    // init_milis(); // Iniciaizace millis TIM2
    // Serial_Begin(9600);
    SHT30_Init(SHT30_ADDRESS_1);        // Inicilaizace sht30
    LCD_I2C_Init(0x27, 16, 2);          // Inicializace LCD
    LCD_I2C_Print("Ultra teploměr..."); // Úvodní obrazovka na displej
    GPIO_Init(GPIOD, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_SLOW);
    delay_ms(1000);
    LCD_I2C_Clear();
}

//! Main program loop
int main(void)
{
    setup();                 // Inicializace všech periferií
    LCD_I2C_SetCursor(0, 0); // Nastavení kurzor
    uint8_t temperature_1[2];
    uint8_t humidity_1[2];

    while (1)
    {
        char buffer1[48];
        char buffer2[48];

        SHT30_ReadRegister(SHT30_ADDRESS_1, temperature_1[0], temperature_1[1], humidity_1[0], humidity_1[1]);
        // SHT30_ReadRegister(SHT30_ADDRESS_2, temperature_1[0], temperature_1[1], humidity_1[0], humidity_1[1]);

        GPIO_WriteReverse(GPIOD, GPIO_PIN_4);
        LCD_I2C_SetCursor(0, 0); // Nastavení kurzoru
        sprintf(buffer1, "temp= %d.%d C", temperature_1[0], temperature_1[1]);
        LCD_I2C_Print(buffer1);
        // Serial_Print_String(buffer1);

        LCD_I2C_SetCursor(0, 1); // Nastavení kurzoru
        sprintf(buffer2, "Hum= %d.%d %%", humidity_1[0], humidity_1[1]);
        LCD_I2C_Print(buffer2);

        delay_ms(1000);
    }
}
