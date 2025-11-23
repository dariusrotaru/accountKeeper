#ifndef MFRC522_H
#define MFRC522_H

#include <stdint.h>

#define MFRC522_REG_COMMAND       0x01
#define MFRC522_REG_COM_IEN       0x02
#define MFRC522_REG_COM_IRQ       0x04
#define MFRC522_REG_ERROR         0x06
#define MFRC522_REG_FIFO_DATA     0x09
#define MFRC522_REG_FIFO_LEVEL    0x0A
#define MFRC522_REG_CONTROL       0x0C
#define MFRC522_REG_BIT_FRAMING   0x0D
#define MFRC522_REG_MODE          0x11
#define MFRC522_REG_TX_MODE       0x12
#define MFRC522_REG_RX_MODE       0x13
#define MFRC522_REG_TX_CONTROL    0x14
#define MFRC522_REG_TX_ASK        0x15
#define MFRC522_REG_T_MODE        0x2A
#define MFRC522_REG_T_PRESCALER   0x2B
#define MFRC522_REG_T_RELOAD_H    0x2C
#define MFRC522_REG_T_RELOAD_L    0x2D
#define MFRC522_REG_VERSION       0x37

#define MFRC522_CMD_IDLE          0x00
#define MFRC522_CMD_TRANSCEIVE    0x0C
#define MFRC522_CMD_SOFTRESET     0x0F

#define PICC_CMD_REQA             0x26
#define PICC_CMD_ANTICOLL_CL1     0x93

#define MFRC522_STATUS_OK         0
#define MFRC522_STATUS_TIMEOUT    1
#define MFRC522_STATUS_ERROR      2
#define MFRC522_STATUS_NO_CARD    3

#define MFRC522_RST_DDR  DDRB
#define MFRC522_RST_PORT PORTB
#define MFRC522_RST_PIN  (1 << PB1)


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