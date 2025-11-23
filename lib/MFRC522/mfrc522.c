#include "mfrc522.h"
#include "spi.h"
#include <avr/io.h>
#include "timer.h"

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
 * @brief Initialises SPI and transfers the value byte into a specified address.
 * 
 * @param reg The address at where to write.
 * @param value The value to be written.
 */
static void MFRC522WriteReg(uint8_t reg, uint8_t value)
{
    SPISelect();
    SPITransfer((reg << 1) & 0x7E);
    SPITransfer(value);
    SPIDeselect();
}
/**
 * @brief Reads the register.
 * 
 * @param reg the local register address.
 * @return uint8_t the register that's read.
 */
static uint8_t MFRC522ReadReg(uint8_t reg)
{
    uint8_t value;
    SPISelect();
    SPITransfer(((reg << 1) & 0x7E) | 0x80);
    value = SPITransfer(0x00);
    SPIDeselect();
    return value;
}

/**
 * @brief Sets the masked bits.
 * 
 * @param reg The register for which to set the bits.
 * @param mask The mask that is used.
 */
static void MFRC522SetBitMask(uint8_t reg, uint8_t mask)
{
    uint8_t tmp = MFRC522ReadReg(reg);
    MFRC522WriteReg(reg, tmp | mask);
}

/**
 * @brief Clears the masked bits.
 * 
 * @param reg The register for which to clear the bits.
 * @param mask The mask that is used.
 */
static void MFRC522ClearBitMask(uint8_t reg, uint8_t mask)
{
    uint8_t tmp = MFRC522ReadReg(reg);
    MFRC522WriteReg(reg, tmp & (~mask));
}

/**
 * @brief Preforms both a hard and a soft reset.
 * 
 */
static void MFRC522ResetChip(void)
{
    // hardware RST pin pulse
    MFRC522_RST_PORT |= MFRC522_RST_PIN;
    DelayMs(1);
    MFRC522_RST_PORT &= ~MFRC522_RST_PIN;
    DelayMs(1);
    MFRC522_RST_PORT |= MFRC522_RST_PIN;
    DelayMs(50);

    // and soft reset command
    MFRC522WriteReg(MFRC522_REG_COMMAND, MFRC522_CMD_SOFTRESET);
    DelayMs(50);
}

void MFRC522Init(void)
{
    MFRC522_RST_DDR |= MFRC522_RST_PIN;
    MFRC522_RST_PORT |= MFRC522_RST_PIN;

    SPIInit();
    MFRC522ResetChip();

    MFRC522WriteReg(MFRC522_REG_T_MODE, 0x8D);
    MFRC522WriteReg(MFRC522_REG_T_PRESCALER, 0x3E);
    MFRC522WriteReg(MFRC522_REG_T_RELOAD_L, 30);
    MFRC522WriteReg(MFRC522_REG_T_RELOAD_H, 0);

    MFRC522WriteReg(MFRC522_REG_TX_ASK, 0x40);
    MFRC522WriteReg(MFRC522_REG_MODE, 0x3D);

    uint8_t tx = MFRC522ReadReg(MFRC522_REG_TX_CONTROL);
    if (!(tx & 0x03)) {
        MFRC522WriteReg(MFRC522_REG_TX_CONTROL, tx | 0x03);
    }
}

uint8_t MFRC522ReadVersion(void)
{
    return MFRC522ReadReg(MFRC522_REG_VERSION);
}

/**
 * @brief Sends data to the RFID card and recieves its response.
 *  Checks for errors (generic), waits for the transfer to be complete and reads the response
 *  back into the buffer.
 * 
 * @param sendData Points to the data to be transmitted.
 * @param sendLen Number of bytes in the pointer sendData.
 * @param backData Points to the buffer where the recieved response will be stored.
 * @param backLen Number of bytes in the pointer backData.
 * @param validBits The number of valid bits in the last byte of sendData.
 * @return uint8_t The status code that is returned. Check definitions.
 */
static uint8_t MFRC522Transceive(uint8_t *sendData, uint8_t sendLen,
                                  uint8_t *backData, uint8_t *backLen,
                                  uint8_t validBits)
{
    uint8_t waitIrq = 0x30;
    uint16_t i;

    MFRC522WriteReg(MFRC522_REG_COMMAND, MFRC522_CMD_IDLE);
    MFRC522WriteReg(MFRC522_REG_COM_IRQ, 0x7F);
    MFRC522WriteReg(MFRC522_REG_FIFO_LEVEL, 0x80);

    for (i = 0; i < sendLen; i++) {
        MFRC522WriteReg(MFRC522_REG_FIFO_DATA, sendData[i]);
    }

    MFRC522WriteReg(MFRC522_REG_BIT_FRAMING, validBits & 0x07);

    MFRC522WriteReg(MFRC522_REG_COMMAND, MFRC522_CMD_TRANSCEIVE);
    MFRC522SetBitMask(MFRC522_REG_BIT_FRAMING, 0x80);

    i = 2000;
    uint8_t irq;
    do {
        irq = MFRC522ReadReg(MFRC522_REG_COM_IRQ);
        if (irq & waitIrq) break;
    } while (--i);

    MFRC522ClearBitMask(MFRC522_REG_BIT_FRAMING, 0x80);

    if (i == 0) {
        return MFRC522_STATUS_TIMEOUT;
    }

    uint8_t error = MFRC522ReadReg(MFRC522_REG_ERROR);
    if (error & 0x13) {
        return MFRC522_STATUS_ERROR;
    }

    uint8_t fifoLevel = MFRC522ReadReg(MFRC522_REG_FIFO_LEVEL);
    if (fifoLevel == 0) {
        return MFRC522_STATUS_NO_CARD;
    }

    if (fifoLevel > *backLen) {
        fifoLevel = *backLen;
    }

    for (i = 0; i < fifoLevel; i++) {
        backData[i] = MFRC522ReadReg(MFRC522_REG_FIFO_DATA);
    }
    *backLen = fifoLevel;

    return MFRC522_STATUS_OK;
}

/**
 * @brief Sends the request a command to detect a RFID card nearby.
 * 
 * @param atqa Pointer toa 2-byte buffer where the recieved answer to request a is stored.
 * @return uint8_t The status code. Check definitions.
 */
static uint8_t MFRC522RequestA(uint8_t *atqa)
{
    uint8_t cmd = PICC_CMD_REQA;
    uint8_t backLen = 2;

    uint8_t status = MFRC522Transceive(&cmd, 1, atqa, &backLen, 0x07);

    if (status != MFRC522_STATUS_OK) {
        return MFRC522_STATUS_NO_CARD;
    }
    if (backLen != 2) {
        return MFRC522_STATUS_ERROR;
    }
    return MFRC522_STATUS_OK;
}

/**
 * @brief Runs anticollision to retrieve the RFID card's UID.
 * It also verified the UID bytes intey checking the BCC with XOR'ing the bytes.
 * @param uid Points to a 4-byte buffer where the UID will be stored.
 * @return uint8_t Status code.
 */
static uint8_t MFRC522Anticoll(uint8_t *uid)
{
    uint8_t buffer[10];
    uint8_t bufferLen = sizeof(buffer);

    buffer[0] = PICC_CMD_ANTICOLL_CL1;
    buffer[1] = 0x20;

    uint8_t status = MFRC522Transceive(buffer, 2, buffer, &bufferLen, 0x00);
    if (status != MFRC522_STATUS_OK) {
        return status;
    }

    if (bufferLen != 5) {
        return MFRC522_STATUS_ERROR;
    }

    uid[0] = buffer[0];
    uid[1] = buffer[1];
    uid[2] = buffer[2];
    uid[3] = buffer[3];

    uint8_t bcc = buffer[4] ^ buffer[0] ^ buffer[1] ^ buffer[2] ^ buffer[3];
    if (bcc != 0) {
        return MFRC522_STATUS_ERROR;
    }

    return MFRC522_STATUS_OK;
}

uint8_t MFRC522GetUID(uint8_t *uid, uint8_t *uidSize)
{
    uint8_t atqa[2];
    uint8_t status;

    status = MFRC522RequestA(atqa);
    if (status != MFRC522_STATUS_OK) {
        return MFRC522_STATUS_NO_CARD;
    }

    status = MFRC522Anticoll(uid);
    if (status != MFRC522_STATUS_OK) {
        return status;
    }

    *uidSize = 4;
    return MFRC522_STATUS_OK;
}
