#include <stdint.h>
#include "spi.h"
#include "lora.h"

# define __STATIC_INLINE

void lora_reset(void)
{
    uint8_t ms = 1;
    // Reset Lora module
    RST_LOW();
    while (ms--) {
        for (uint8_t i = 0; i < 100; i++) __asm__("nop");
    }
    RST_LOW();
}


// Send command to lora module
uint8_t lora_write_cmd(uint8_t cmd) 
{
    CSN_LOW();
    uint8_t s = SPI_write(cmd);
    CSN_HIGH();
    return s;
}

// Read lora's register
uint8_t lora_read_register(uint8_t regNo) 
{
    CSN_LOW();
    SPI_write(LORA_R_REGISTER & regNo);
    uint8_t r = SPI_write(LORA_NOP);
    CSN_HIGH();
    return r;
}

// Write a value to lora's register
uint8_t __STATIC_INLINE lora_write_register(uint8_t regNo, uint8_t regVal) {
    CSN_LOW();
    uint8_t s = SPI_write(LORA_W_REGISTER | regNo);
    SPI_write(regVal);
    CSN_HIGH();
    return s;
}
/*
void __STATIC_INLINE lora_write_addr(uint8_t a, const uint8_t* data, uint8_t len)
{
    CSN_LOW();
    SPI_write(lora24_W_REGISTER | (lora24_REGISTER_MASK & a));
    while (len--) {
        SPI_write(*data++);
    }
    CSN_HIGH();
}


// Read lora's register
uint8_t __STATIC_INLINE lora_read_addr(uint8_t regNo, uint8_t len)
{
    uint8_t r;
    CSN_LOW();
    SPI_write(lora24_R_REGISTER | (lora24_REGISTER_MASK & regNo));
    while (len--) {
        r = SPI_write(LORA_NOP);
    }
    CSN_HIGH();
    return r;
}
*/

uint8_t lora_detect(void) 
{  
    lora_write_cmd(lora24_FLUSH_TX);
    uint8_t fifo_status;
    uint8_t returnCode = lora_DETECTED;
    fifo_status = lora_read_register(lora24_FIFO_STATUS_REG);
    if ((fifo_status & lora24_FIFO_STATUS_TX_EMPTY) != lora24_FIFO_STATUS_TX_EMPTY) {
        returnCode = lora_NOT_DETECTED;
    }
    if ((fifo_status & lora24_FIFO_STATUS_TX_FULL) == lora24_FIFO_STATUS_TX_FULL) {
        returnCode = lora_NOT_DETECTED;
    }
  
    // send dummy 1-byte payload to the 1st FIFO buffer
    CSN_LOW();
    SPI_write(lora24_W_TX_PAYLOAD);
    SPI_write(0);
    CSN_HIGH();
    
    fifo_status = lora_read_register(lora24_FIFO_STATUS_REG);
    if ((fifo_status & lora24_FIFO_STATUS_TX_EMPTY) == lora24_FIFO_STATUS_TX_EMPTY) {
        returnCode = lora_NOT_DETECTED;
    }
  
    if ((fifo_status & lora24_FIFO_STATUS_TX_FULL) == lora24_FIFO_STATUS_TX_FULL) {
        returnCode = lora_NOT_DETECTED;
    }
    
    // send dummy 1-byte payload to the 2nd FIFO buffer
    CSN_LOW();
    SPI_write(lora24_W_TX_PAYLOAD);
    SPI_write(0);
    CSN_HIGH();
    
    fifo_status = lora_read_register(lora24_FIFO_STATUS_REG);
    if ((fifo_status & lora24_FIFO_STATUS_TX_EMPTY) == lora24_FIFO_STATUS_TX_EMPTY) {
        returnCode = lora_NOT_DETECTED;
    }
  
    if ((fifo_status & lora24_FIFO_STATUS_TX_FULL) == lora24_FIFO_STATUS_TX_FULL) {
        returnCode = lora_NOT_DETECTED;
    }
  
    // send dummy 1-byte payload to the 3rd FIFO buffer
    CSN_LOW();
    SPI_write(lora24_W_TX_PAYLOAD);
    SPI_write(0);
    CSN_HIGH();
  
    fifo_status = lora_read_register(lora24_FIFO_STATUS_REG);
    if ((fifo_status & lora24_FIFO_STATUS_TX_EMPTY) == lora24_FIFO_STATUS_TX_EMPTY) {
        returnCode = lora_NOT_DETECTED;
    }
  
    if ((fifo_status & lora24_FIFO_STATUS_TX_FULL) != lora24_FIFO_STATUS_TX_FULL) {
        returnCode = lora_NOT_DETECTED;
    }
  
    lora_write_cmd(lora24_FLUSH_TX);
    
    fifo_status = lora_read_register(lora24_FIFO_STATUS_REG);
    if ((fifo_status & lora24_FIFO_STATUS_TX_EMPTY) != lora24_FIFO_STATUS_TX_EMPTY) {
        returnCode = lora_NOT_DETECTED;
    }
  
    if ((fifo_status & lora24_FIFO_STATUS_TX_FULL) == lora24_FIFO_STATUS_TX_FULL) {
        returnCode = lora_NOT_DETECTED;
    }
    
    return returnCode;
}  


uint8_t __STATIC_INLINE lora_init(uint8_t channel) {
    lora_write_register(lora24_SETUP_RETR, 0x5F);  // CONFIG: ARD=5, ARC=15
    lora_write_register(lora24_RF_SETUP_REG, 0x27); //'00' – 250kbps, '11' – 0dBm, '0'
    lora_write_register(lora24_DYNPD_REG, 0x00); // disable dynamic payloads by default (for all pipes)
    lora_write_register(lora24_EN_AA_REG, 0x3F);  // enable auto-ack on all pipes
    lora_write_register(lora24_EN_EN_RXADDR, 3); // only open RX pipes 0 & 1
    lora_write_register(lora24_RX_PW_P0, 32);     // set static payload size to 32 (max) bytes by default
    lora_write_register(lora24_SETUP_AW_REG, 3);  // set default address length to (max) 5 bytes
    
    // Set radio channel number
    lora_write_register(lora24_RF_CH_REG, channel);

    // Reset current status
    // Notice reset and flush is the last thing we do
    lora_write_register(lora24_STATUS_REG, lora24_CONFIG_RX_DR | lora24_CONFIG_TX_DS | lora24_CONFIG_MAX_RT);

    lora_write_register(lora24_CONFIG_REG, 0x0E);  // CONFIG: Enable CRC, TX mode
    
    return 1;
}

uint8_t __STATIC_INLINE lora_openWritingPipe(const uint8_t* address)
{
    // Note that AVR 8-bit uC's store this LSB first, and the lora(+)
    // expects it LSB first too, so we're good.
    lora_write_addr(lora24_RX_ADDR_P0_REG, address, 5);
    lora_write_addr(lora24_TX_ADDR_REG, address, 5);
}

void lora_sendpayload(uint8_t *data, uint8_t len) {
    uint16_t ms = 1;
    uint16_t lora_status;
    //flush_tx
    lora_write_cmd(lora24_FLUSH_TX); 
    lora_write_register(lora24_STATUS_REG, lora24_CONFIG_RX_DR | lora24_CONFIG_TX_DS | lora24_CONFIG_MAX_RT);
    CSN_LOW();
    SPI_write(lora24_W_TX_PAYLOAD);
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
    
    lora_write_register(lora24_STATUS_REG, lora24_CONFIG_RX_DR | lora24_CONFIG_TX_DS | lora24_CONFIG_MAX_RT);
    lora_read_register(lora24_STATUS_REG);
}

