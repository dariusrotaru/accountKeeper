#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>

/**
 * @brief Initializes the ports for buttons.
 * 
 */
void BtnInit(void);
/**
 * @brief Gets the state from the left button with a light debounce.
 * 
 * @return uint8_t Returns 1 if pressed, 0 otherwise.
 */
uint8_t BtnLGetState(void);
/**
 * @brief Gets the state from the right button with a light debounce.
 * 
 * @return uint8_t Returns 1 if pressed, 0 otherwise.
 */
uint8_t BtnRGetState(void);

#endif