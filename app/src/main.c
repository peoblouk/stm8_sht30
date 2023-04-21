#include "stm8s.h"
#include "delay.h"
#include "LCD_I2C.h"
#include "milis.h"
#include "lm75a.h"

// Indikační LED
#define LED_PORT GPIOD
#define LED_PIN GPIO_PIN_4

#define TEPLOMER 0x49

void setup(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);           // Předdělička DIV1
    delay_init();                                            // Incializace časovače TIM4
    init_milis();                                            // Iniciaizace millis TIM2
    GPIO_Init(LED_PORT, LED_PIN, GPIO_MODE_OUT_PP_LOW_SLOW); // Pin LED RED
    LCD_I2C_Init(0x27, 16, 2);                               // Inicializace LCD
    LM75A_Init(TEPLOMER);                                    // Inicializace teploměrů
    GPIO_Init(LED_PORT, LED_PIN, GPIO_MODE_OUT_PP_LOW_SLOW); // Inicializace LED
    LCD_I2C_Print("Ultra Patrik");
    delay_ms(1000);
    LCD_I2C_Clear();
}

void blink(void)
{
    GPIO_WriteReverse(LED_PORT, LED_PIN);
    delay_ms(20);
    GPIO_WriteReverse(LED_PORT, LED_PIN);
}

int main(void)
{
    uint8_t data_teplomer[2]; // Proměnná pro uložení teploty
    uint16_t cas_Ted = 0;     // Proměnná pro millis
    char buffer[48];          // Proměnná pro zápis na displej
    uint8_t posun = 0;

    setup();                    // Inicializace všech periferií
    LCD_I2C_SetCursor(0, 0);    // Nastavení kurzoru
    LCD_I2C_Print("Teplota :"); // Úvodní obrazovka na displej
    while (1)
    {
        if ((get_milis() - cas_Ted) > 2000)
        {
            cas_Ted = get_milis();                          // Milis now
            LM75A_ReadTemperature(TEPLOMER, data_teplomer); // Čtení teploty

            if (posun <= 4)
            {
                posun = 0;
            }
            else
            {
                posun++;
            }

            LCD_I2C_Clear();
            LCD_I2C_SetCursor(posun, 1);                                        // Nastavení kurzoru
            sprintf(buffer, "T = %d.%d C", data_teplomer[0], data_teplomer[1]); // Zformátování stringu
            LCD_I2C_Print(buffer);                                              // Vytiskni na displej
        }
    }
}