#include <stdint.h>
#include "register.h"
#include "main.h"
#include "nrf24.h"

#define F_CPU 2000000UL

#define _SFR_(mem_addr)     (*(volatile uint8_t *)(0x5000 + (mem_addr)))
#define RF_CHANNEL              99


#define LED_PIN     5

static inline void delay_ms(uint16_t ms) {
    uint32_t i;
    for (i = 0; i < ((F_CPU / 18000UL) * ms); i++)
        __asm__("nop"); 
}

void main() {
    uint8_t nrf_status = NRF24L01_NOT_DETECTED;
    uint8_t tx_data[] = "HELLOHELLOHELLOHELLOHELLOHELLOO";  // Example payload
    const uint8_t pipe[] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
    uint8_t counter = 0;
    SPI_init();

    
    PB_DDR |= (1 << LED_PIN); // configure PD4 as output
    PB_CR1 |= (1 << LED_PIN); // push-pull mode

    while (nrf_status == NRF24L01_NOT_DETECTED) 
    {   
        counter += 1;
        nrf_status = nrf_detect();
        delay_ms(1000);
    }

    // Init, open channel 10
    nrf_init(10);

    // Open pipe, with maximum length address 5
    nrf_openWritingPipe(pipe);

    while (1) {
        /* toggle pin every 250ms */
        //PB_ODR ^= (1 << LED_PIN);
        PIN_TOGGLE(B, LED_PIN);
        nrf_sendpayload(tx_data, 32);
        delay_ms(1000);
    }
}