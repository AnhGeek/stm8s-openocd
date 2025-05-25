#ifndef __NRF24_H__
#define __NRF24_H__
#include <stdint.h>
#include "spi.h"

#define NRF24L01_DETECTED       1
#define NRF24L01_NOT_DETECTED   0

#define NRF24_CONFIG_REG        0x00
#define NRF24_EN_AA_REG         0x01
#define NRF24_EN_EN_RXADDR      0x02
#define NRF24_SETUP_AW_REG      0x03
#define NRF24_SETUP_RETR        0x04
#define NRF24_RF_CH_REG         0x05
#define NRF24_RF_SETUP_REG      0x06
#define NRF24_STATUS_REG        0x07
#define NRF24_OBSERVE_TX        0x08
#define NRF24_RX_ADDR_P0_REG    0x0A
#define NRF24_TX_ADDR_REG       0x10
#define NRF24_RX_PW_P0          0x11
#define NRF24_FIFO_STATUS_REG   0x17
#define NRF24_DYNPD_REG         0x1C
#define NRF24_FEATURE_REG       0x1D 
          
// 00: CONFIG register bits from 0 t_die 6, bit 7 must be set to 0
// ============================================================
#define NRF24_CONFIG_PRIM_RX    (1 << 0)
#define NRF24_CONFIG_PWR_UP     (1 << 1)
#define NRF24_CONFIG_CRCO       (1 << 2)
#define NRF24_CONFIG_EN_CRC     (1 << 3)
#define NRF24_CONFIG_MAX_RT     (1 << 4)
#define NRF24_CONFIG_TX_DS      (1 << 5)
#define NRF24_CONFIG_RX_DR      (1 << 6)

// 07: STATUS -- Status Register (In parallel to the SPI command
//               word applied on the MOSI pin, the STATUS register
//               is shifted serially out on the MISO pin)   (bits from 0 to 6)
// ===============================================================
#define NRF24_STATUS_MAX_RT     NRF24_CONFIG_MAX_RT
#define NRF24_STATUS_TX_DS      NRF24_CONFIG_TX_DS
#define NRF24_STATUS_RX_DR      NRF24_CONFIG_RX_DR

// 06: RF_SETUP -- RF Setup Register (bits from 0 to 6)
// ====================================================
#define NRF24_RF_SETUP_RF_PWR_0         (1 << 0)
#define NRF24_RF_SETUP_RF_PWR_1         (1 << 1)
#define NRF24_RF_SETUP_RF_DR_HIGH       (1 << 3)
#define NRF24_RF_SETUP_RF_DR_LOW        (1 << 5)
          
#define NRF24_FIFO_STATUS_RX_EMPTY      (1 << 0)
#define NRF24_FIFO_STATUS_TX_FULL       (1 << 5)
#define NRF24_FIFO_STATUS_TX_EMPTY      (1 << 4)
        
#define NRF24_FEATURE_EN_DYN_ACK        (1 << 0)
#define NRF24_FEATURE_EN_DPL            (1 << 2)

          
#define NRF24_R_REGISTER        0x00
#define NRF24_W_REGISTER        0x20
#define NRF24_REGISTER_MASK     0x1F
#define NRF24_R_RX_PL_WID       0x60
#define NRF24_R_RX_PAYLOAD      0x61
#define NRF24_W_TX_PAYLOAD      0xA0

#define NRF24_FLUSH_RX          0xE2
#define NRF24_FLUSH_TX          0xE1

#define NRF24_NOP               0xFF       

extern uint8_t nrf_write_cmd(uint8_t cmd);
extern uint8_t nrf_detect(void);
uint8_t nrf_init(uint8_t channel);
void nrf_sendpayload(uint8_t *data, uint8_t len);
uint8_t nrf_read_register(uint8_t regNo);
void nrf_write_addr(uint8_t a, const uint8_t* data, uint8_t len);
uint8_t nrf_read_addr(uint8_t regNo, uint8_t len);
uint8_t nrf_openWritingPipe(const uint8_t* address);
#endif