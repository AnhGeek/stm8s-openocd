#include <stdint.h>
#include "spi.h"
#include "nrf24.h"

# define __STATIC_INLINE
// Send command to NRF24L01 module
uint8_t nrf_write_cmd(uint8_t cmd) 
{
    CSN_LOW();
    uint8_t s = SPI_write(cmd);
    CSN_HIGH();
    return s;
}

// Read NRF24L01's register
uint8_t nrf_read_register(uint8_t regNo) 
{
    CSN_LOW();
    SPI_write(NRF24_R_REGISTER | (NRF24_REGISTER_MASK & regNo));
    uint8_t r = SPI_write(NRF24_NOP);
    CSN_HIGH();
    return r;
}

// Write a value to NRF24L01's register
uint8_t __STATIC_INLINE nrf_write_register(uint8_t regNo, uint8_t regVal) {
    CSN_LOW();
    uint8_t s = SPI_write(NRF24_W_REGISTER | (NRF24_REGISTER_MASK & regNo));
    SPI_write(regVal);
    CSN_HIGH();
    return s;
}

void __STATIC_INLINE nrf_write_addr(uint8_t a, const uint8_t* data, uint8_t len)
{
    CSN_LOW();
    SPI_write(NRF24_W_REGISTER | (NRF24_REGISTER_MASK & a));
    while (len--) {
        SPI_write(*data++);
    }
    CSN_HIGH();
}

// Read NRF24L01's register
uint8_t __STATIC_INLINE nrf_read_addr(uint8_t regNo, uint8_t len)
{
    uint8_t r;
    CSN_LOW();
    SPI_write(NRF24_R_REGISTER | (NRF24_REGISTER_MASK & regNo));
    while (len--) {
        r = SPI_write(NRF24_NOP);
    }
    CSN_HIGH();
    return r;
}

uint8_t nrf_detect(void) 
{  
    nrf_write_cmd(NRF24_FLUSH_TX);
    uint8_t fifo_status;
    uint8_t returnCode = NRF24L01_DETECTED;
    fifo_status = nrf_read_register(NRF24_FIFO_STATUS_REG);
    if ((fifo_status & NRF24_FIFO_STATUS_TX_EMPTY) != NRF24_FIFO_STATUS_TX_EMPTY) {
        returnCode = NRF24L01_NOT_DETECTED;
    }
    if ((fifo_status & NRF24_FIFO_STATUS_TX_FULL) == NRF24_FIFO_STATUS_TX_FULL) {
        returnCode = NRF24L01_NOT_DETECTED;
    }
  
    // send dummy 1-byte payload to the 1st FIFO buffer
    CSN_LOW();
    SPI_write(NRF24_W_TX_PAYLOAD);
    SPI_write(0);
    CSN_HIGH();
    
    fifo_status = nrf_read_register(NRF24_FIFO_STATUS_REG);
    if ((fifo_status & NRF24_FIFO_STATUS_TX_EMPTY) == NRF24_FIFO_STATUS_TX_EMPTY) {
        returnCode = NRF24L01_NOT_DETECTED;
    }
  
    if ((fifo_status & NRF24_FIFO_STATUS_TX_FULL) == NRF24_FIFO_STATUS_TX_FULL) {
        returnCode = NRF24L01_NOT_DETECTED;
    }
    
    // send dummy 1-byte payload to the 2nd FIFO buffer
    CSN_LOW();
    SPI_write(NRF24_W_TX_PAYLOAD);
    SPI_write(0);
    CSN_HIGH();
    
    fifo_status = nrf_read_register(NRF24_FIFO_STATUS_REG);
    if ((fifo_status & NRF24_FIFO_STATUS_TX_EMPTY) == NRF24_FIFO_STATUS_TX_EMPTY) {
        returnCode = NRF24L01_NOT_DETECTED;
    }
  
    if ((fifo_status & NRF24_FIFO_STATUS_TX_FULL) == NRF24_FIFO_STATUS_TX_FULL) {
        returnCode = NRF24L01_NOT_DETECTED;
    }
  
    // send dummy 1-byte payload to the 3rd FIFO buffer
    CSN_LOW();
    SPI_write(NRF24_W_TX_PAYLOAD);
    SPI_write(0);
    CSN_HIGH();
  
    fifo_status = nrf_read_register(NRF24_FIFO_STATUS_REG);
    if ((fifo_status & NRF24_FIFO_STATUS_TX_EMPTY) == NRF24_FIFO_STATUS_TX_EMPTY) {
        returnCode = NRF24L01_NOT_DETECTED;
    }
  
    if ((fifo_status & NRF24_FIFO_STATUS_TX_FULL) != NRF24_FIFO_STATUS_TX_FULL) {
        returnCode = NRF24L01_NOT_DETECTED;
    }
  
    nrf_write_cmd(NRF24_FLUSH_TX);
    
    fifo_status = nrf_read_register(NRF24_FIFO_STATUS_REG);
    if ((fifo_status & NRF24_FIFO_STATUS_TX_EMPTY) != NRF24_FIFO_STATUS_TX_EMPTY) {
        returnCode = NRF24L01_NOT_DETECTED;
    }
  
    if ((fifo_status & NRF24_FIFO_STATUS_TX_FULL) == NRF24_FIFO_STATUS_TX_FULL) {
        returnCode = NRF24L01_NOT_DETECTED;
    }
    
    return returnCode;
}  


uint8_t __STATIC_INLINE nrf_init(uint8_t channel) {
    nrf_write_register(NRF24_SETUP_RETR, 0x5F);  // CONFIG: ARD=5, ARC=15
    nrf_write_register(NRF24_RF_SETUP_REG, 0x07); //'00' – 1Mbps, '11' – 0dBm, '0'
    nrf_write_register(NRF24_DYNPD_REG, 0x00); // disable dynamic payloads by default (for all pipes)
    nrf_write_register(NRF24_EN_AA_REG, 0x3F);  // enable auto-ack on all pipes
    nrf_write_register(NRF24_EN_EN_RXADDR, 3); // only open RX pipes 0 & 1
    nrf_write_register(NRF24_RX_PW_P0, 32);     // set static payload size to 32 (max) bytes by default
    nrf_write_register(NRF24_SETUP_AW_REG, 3);  // set default address length to (max) 5 bytes
    
    // Set radio channel number
    nrf_write_register(NRF24_RF_CH_REG, channel);

    // Reset current status
    // Notice reset and flush is the last thing we do
    nrf_write_register(NRF24_STATUS_REG, NRF24_CONFIG_RX_DR | NRF24_CONFIG_TX_DS | NRF24_CONFIG_MAX_RT);

    nrf_write_register(NRF24_CONFIG_REG, 0x0E);  // CONFIG: Enable CRC, TX mode
    
    return 1;
}

uint8_t __STATIC_INLINE nrf_openWritingPipe(const uint8_t* address)
{
    // Note that AVR 8-bit uC's store this LSB first, and the NRF24L01(+)
    // expects it LSB first too, so we're good.
    nrf_write_addr(NRF24_RX_ADDR_P0_REG, address, 5);
    nrf_write_addr(NRF24_TX_ADDR_REG, address, 5);
}

void nrf_sendpayload(uint8_t *data, uint8_t len) {
    uint16_t ms = 1;
    uint16_t nrf_status;
    //flush_tx
    nrf_write_cmd(NRF24_FLUSH_TX); 
    nrf_write_register(NRF24_STATUS_REG, NRF24_CONFIG_RX_DR | NRF24_CONFIG_TX_DS | NRF24_CONFIG_MAX_RT);
    CSN_LOW();
    SPI_write(NRF24_W_TX_PAYLOAD);
    for (uint8_t i = 0; i < len; i++) {
        SPI_write(data[i]);
    }
    CSN_HIGH();

    // Start transmission
    CE_HIGH();
    while (ms--) {
        for (uint16_t i = 0; i < 50; i++) __asm__("nop");
    }
    CE_LOW();
    
    nrf_write_register(NRF24_STATUS_REG, NRF24_CONFIG_RX_DR | NRF24_CONFIG_TX_DS | NRF24_CONFIG_MAX_RT);
    nrf_read_register(NRF24_STATUS_REG);
}
