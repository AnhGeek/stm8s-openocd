#ifndef __REGISTER_H__
#define __REGISTER_H__

#define F_CPU 2000000UL

#define _SFR_(mem_addr)     (*(volatile uint8_t *)(0x5000 + (mem_addr)))

/* PORT A */
#define PA_ODR      _SFR_(0x00)
#define PA_IDR      _SFR_(0x01)
#define PA_DDR      _SFR_(0x02)
#define PA_CR1      _SFR_(0x03)

/* PORT B */
#define PB_ODR      _SFR_(0x05)
#define PB_DDR      _SFR_(0x07)
#define PB_CR1      _SFR_(0x08)

/* PORT C */
#define PC_ODR      _SFR_(0x0A)
#define PC_DDR      _SFR_(0x0C)
#define PC_CR1      _SFR_(0x0D)

/* PORT D */
#define PD_ODR      _SFR_(0x0F)
#define PD_DDR      _SFR_(0x11)
#define PD_CR1      _SFR_(0x12)

/* SPI */
#define SPI_CR1     _SFR_(0x200)
#define SPE         6
#define BR0         3
#define MSTR        2
#define SPI_CR2     _SFR_(0x201)
#define SSM         1
#define SSI         0
#define SPI_SR      _SFR_(0x203)
#define BSY         7
#define TXE         1
#define RXNE        0
#define SPI_DR      _SFR_(0x204)

#endif