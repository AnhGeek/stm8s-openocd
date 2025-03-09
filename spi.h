#include <stdint.h>
#include "register.h"

/*
 * SPI pinout:
 * SCK  -> PC5
 * MOSI -> PC6
 * MISO -> PC7
 * CS   -> PC3
 * CE   -> PC4
 */

/* Chip select */
#define CS_PIN      3 // PC3
#define CE_PIN      4 // PC4


void SPI_init();
void SPI_write(uint8_t data)
uint8_t SPI_read();
void chip_select();
void chip_deselect();