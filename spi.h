#ifndef __SPI_H__
#define __SPI_H__
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

#define CSN_LOW()   chip_select()
#define CSN_HIGH()   chip_deselect()

// Pin Mapping
#define CE_HIGH()   ce_high()
#define CE_LOW()    ce_low()


void SPI_init();
uint8_t SPI_write(uint8_t data);
uint8_t SPI_read(uint8_t data);
void chip_select();
void chip_deselect();
void ce_low();
void ce_high();

#endif