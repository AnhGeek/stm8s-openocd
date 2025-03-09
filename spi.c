#include "spi.h"
/*
 * SPI pinout:
 * SCK  -> PC5
 * MOSI -> PC6
 * MISO -> PC7
 * CS   -> PC3
 * CE   -> PC4
 */

void SPI_init() {
    /* Initialize CS pin */
    PC_DDR |= (1 << CS_PIN);
    PC_CR1 |= (1 << CS_PIN);
    PC_ODR |= (1 << CS_PIN);

    /* Initialize SPI master at 500kHz  */
    SPI_CR2 = (1 << SSM) | (1 << SSI);
    SPI_CR1 = (1 << MSTR) | (1 << SPE) | (1 << BR0);
}

void SPI_write(uint8_t data) {
    SPI_DR = data;
    while (!(SPI_SR & (1 << TXE)));
}

uint8_t SPI_read() {
    SPI_write(0xFF);
    while (!(SPI_SR & (1 << RXNE)));
    return SPI_DR;
}

void chip_select() {
    PC_ODR &= ~(1 << CS_PIN);
}

void chip_deselect() {
    PC_ODR |= (1 << CS_PIN);
}