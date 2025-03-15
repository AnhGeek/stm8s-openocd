#include <stdint.h>
#include "register.h"
#include "main.h"

#define F_CPU 2000000UL

#define _SFR_(mem_addr)     (*(volatile uint8_t *)(0x5000 + (mem_addr)))



#define LED_PIN     5

static inline void delay_ms(uint16_t ms) {
    uint32_t i;
    for (i = 0; i < ((F_CPU / 18000UL) * ms); i++)
        __asm__("nop"); 
}

void main() {
    SPI_init();

    
    PB_DDR |= (1 << LED_PIN); // configure PD4 as output
    PB_CR1 |= (1 << LED_PIN); // push-pull mode

    while (1) {
        /* toggle pin every 250ms */
        //PB_ODR ^= (1 << LED_PIN);
        PIN_TOGGLE(B, LED_PIN);
        delay_ms(1000);
    }
}