#include <stdint.h>
#include "spi.h"
#include "lora.h"



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
__STATIC_INLINE uint8_t lora_write_cmd(uint8_t cmd) 
{
    CSN_LOW();
    uint8_t s = SPI_write(cmd);
    CSN_HIGH();
    return s;
}

// Read lora's register
__STATIC_INLINE uint8_t lora_read_register(uint8_t regNo) 
{
    CSN_LOW();
    SPI_write(LORA_R_REGISTER & regNo);
    uint8_t r = SPI_write(LORA_NOP);
    CSN_HIGH();
    return r;
}

// Write a value to lora's register
__STATIC_INLINE uint8_t lora_write_register(uint8_t regNo, uint8_t regVal) {
    CSN_LOW();
    uint8_t s = SPI_write(LORA_W_REGISTER | regNo);
    SPI_write(regVal);
    CSN_HIGH();
    return s;
}
/*
__STATIC_INLINE void lora_write_addr(uint8_t a, const uint8_t* data, uint8_t len)
{
    CSN_LOW();
    SPI_write(lora24_W_REGISTER | (lora24_REGISTER_MASK & a));
    while (len--) {
        SPI_write(*data++);
    }
    CSN_HIGH();
}


// Read lora's register
__STATIC_INLINE uint8_t lora_read_addr(uint8_t regNo, uint8_t len)
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
    uint8_t RegOpMode;
    uint8_t returnCode = LORA_NOT_DETECTED;

    lora_reset();
    RegOpMode = lora_read_register(REG_OP_MODE);
    if (RegOpMode == 0x09) {
        returnCode = LORA_DETECTED;
    }
        
    return returnCode;
}  


uint8_t lora_init(uint8_t channel) {
    
    return 1;
}

__STATIC_INLINE uint8_t lora_openWritingPipe(const uint8_t* address)
{

}

uint8_t lora_sendpayload(uint8_t *data, uint8_t len) {
    uint8_t ret_len = 0;

    return ret_len;
}

