#include "lcd.h"
#include "timer.h"
#include <avr/io.h>


#define LCD_PORT PORTD
#define LCD_DDR DDRD
#define LCD_RS (1 << PD7)
#define LCD_E  (1 << PD6)
#define LCD_D4 (1 << PD5)
#define LCD_D5 (1 << PD4)
#define LCD_D6 (1 << PD3)
#define LCD_D7 (1 << PD2)

#define LCD_DATA_MASK (LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7)

/**
 * @brief Pulses E High then Low.
 * 
 */
static void LCDPulseEnable(void){
    LCD_PORT |= LCD_E;
    DelayUs(2);
    LCD_PORT &= ~LCD_E;
    DelayUs(40);
}
/**
 * @brief Writes a nibble to the data pins.
 * 
 * @param nibble The nibble to write.
 */
static void LCDWriteNibble(uint8_t nibble){
    //sets all data pins to 0.
    LCD_PORT &= ~LCD_DATA_MASK;
    if(nibble & (1<<0)) LCD_PORT |= LCD_D4;
    if(nibble & (1<<1)) LCD_PORT |= LCD_D5;
    if(nibble & (1<<2)) LCD_PORT |= LCD_D6;
    if(nibble & (1<<3)) LCD_PORT |= LCD_D7;
}

/**
 * @brief Sends a command byte to the LCD.
 * 
 * @param cmd The 8-bit commnad to be sent.
 */
static void LCDSendCommand(uint8_t cmd){
    LCD_PORT &= ~LCD_RS;
    LCDWriteNibble(cmd >> 4);
    LCDPulseEnable();

    LCDWriteNibble(cmd);
    LCDPulseEnable();
}

/**
 * @brief Sends a data byte to the lcd.
 * 
 * @param data The byte character to be sent.
 */
static void LCDSendData(uint8_t data){
    LCD_PORT |= LCD_RS;

    LCDWriteNibble(data >> 4);
    LCDPulseEnable();

    LCDWriteNibble(data);
    LCDPulseEnable();
}
void LCDInit(void){
    LCD_DDR |= (LCD_RS | LCD_E | LCD_DATA_MASK);

    DelayMs(50);

    LCDWriteNibble(0x03);
    LCDPulseEnable();
    DelayMs(5);

    LCDWriteNibble(0x03);
    LCDPulseEnable();
    DelayMs(100);

    LCDWriteNibble(0x03);
    LCDPulseEnable();
    DelayMs(100);

    LCDWriteNibble(0x02);
    LCDPulseEnable();
    DelayMs(100);

    LCDSendCommand(0x28);
    LCDSendCommand(0x0C);
    LCDSendCommand(0x06);

    LCDClear();
}

void LCDClear(void){
    LCDSendCommand(0x01);
    DelayMs(5);
}

void LCDSetCursor(uint8_t col, uint8_t row){
    uint8_t address = col;
    if (row == 1){
        address += 0x40;
    }
    LCDSendCommand(0x80 | address);
}

void LCDPrintString(const char* string){
    while (*string != '\0'){
        LCDSendData(*string);
        string++;
    }
}