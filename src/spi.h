#ifndef SPI_H
#define SPI_H

#include <stdint.h>


/**
 * @brief Initializes the spi in master mode.
 * 
 */
void SPIInit(void);
/**
 * @brief Send one byte and recieve one byte simultaneously.
 * 
 * @param data The byte to send.
 * @return The byte recieved from the slave.
 */
uint8_t SPITransfer(uint8_t data);
/**
 * @brief Selects the SPI slave.
 * 
 */
void SPISelect(void);
/**
 * @brief Deselects the SPI slave.
 * 
 */
void SPIDeselect(void);
#endif