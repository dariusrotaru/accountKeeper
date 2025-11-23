#ifndef MFRC522_H
#define MFRC522_H

#include <stdint.h>

/**
 * @brief Initialises the one shit timer, which runs in non gated mode and sets the prescaler.
 * 
 */
void MFRC522Init(void);
/**
 * @brief Read the register which stores the chip's version.
 * Used for debugging.
 * @return uint8_t Returns the contents at register 0x37.
 */
uint8_t MFRC522ReadVersion(void);
/**
 * @brief Detects if a card is present and retrieves the UID.
 * 
 * @param uid Points to a buffer where the UID will be stored.
 * @param uidSize Pointer to a variable that will be set to the UID length, set 4 for cascade level 1.
 * @return uint8_t Status code.
 */
uint8_t MFRC522GetUID(uint8_t *uid, uint8_t *uidSize);


#endif