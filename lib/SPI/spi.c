#include "spi.h"
#include <avr/io.h>

#define SPI_PORT PORTB
#define SPI_DDR  DDRB
#define SPI_SCK  (1 << PB5)
#define SPI_MISO (1 << PB4)
#define SPI_MOSI (1 << PB3)
#define SPI_SS   (1 << PB2)

void SPIInit(void){
    SPI_DDR |= (SPI_SCK | SPI_MOSI | SPI_SS);
    SPI_DDR &= ~SPI_MISO;

    SPI_PORT |= SPI_SS;

    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);


}

uint8_t SPITransfer(uint8_t data){
    SPDR = data;
    while(!(SPSR & (1 << SPIF)));

    return SPDR;
}
void SPISelect(void){
    SPI_PORT &= ~SPI_SS;
}

void SPIDeselect(void){
    SPI_PORT |= SPI_SS;
}
