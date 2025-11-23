#ifndef LCD_H
#define LCD_H

#include <stdint.h>

/**
 * @brief Initialize the LCD in 4-bit mode.
 *
 */
void LCDInit(void);

/**
 * @brief Wipe the display.
 * 
 */
void LCDClear(void);

/**
 * @brief  Sets the cursor positon.
 * 
 * @param col Column (0-15)
 * @param row  Row (0-1)
 */
void LCDSetCursor(uint8_t col, uint8_t row);


/**
 * @brief  Prints a string to the lcd at the current cursor.
 * 
 * @param string The string to be printed.
 */
void LCDPrintString(const char* string);
#endif 