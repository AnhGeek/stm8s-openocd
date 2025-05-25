#include <stdint.h>
#include "register.h"
#include "main.h"
#include "nrf24.h"
#include "lora.h"

#define F_CPU 2000000UL

#define _SFR_(mem_addr)     (*(volatile uint8_t *)(0x5000 + (mem_addr)))
#define RF_CHANNEL              10

#define BUTTON_PIN  2
#define LED_PIN     5

static inline void delay_ms(uint16_t ms) {
    uint32_t i;
    for (i = 0; i < ((F_CPU / 18000UL) * ms); i++)
        __asm__("nop"); 
}

void main() {
    uint8_t nrf_status = NRF24L01_NOT_DETECTED;
    uint8_t state[1] = {0};
    uint8_t tx_data[] = "BT0XBT1XBT2XBT3XIN0XIN1XIN2XYYY";  // Payload template
    const uint8_t pipe[] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
    uint16_t counter[1] = {0};
    SPI_init();

    PA_DDR &= ~(1 << BUTTON_PIN); // configure PA2 as input
    PA_CR1 |= (1 << BUTTON_PIN); //  Input with pull-up mode

    PB_DDR |= (1 << LED_PIN); // configure PD4 as output
    PB_CR1 |= (1 << LED_PIN); // push-pull mode

    while (nrf_status == NRF24L01_NOT_DETECTED) 
    {   
        nrf_status = nrf_detect();
        delay_ms(1000);
    }

    // Init, open channel 10
    nrf_init(RF_CHANNEL);

    // Open pipe, with maximum length address 5
    nrf_openWritingPipe(pipe);
    //lora_write_cmd(0x00);

    while (1) {
        if (!(PA_IDR & (1 << BUTTON_PIN))) 
        {
            delay_ms(50);
            counter[0] += 1;
            if (!(PA_IDR & (1 << BUTTON_PIN))) 
            {
                tx_data[3] = '0';
                nrf_sendpayload(tx_data, 32);
                PB_ODR &= ~(1 << LED_PIN);
            }
            while (!(PA_IDR & (1 << BUTTON_PIN))) {delay_ms(50);}
        } else {
            tx_data[3] = '1';
        }
        counter[0] += 1;
        for(uint8_t i = 0;i < sizeof(counter); i++)
        {
            switch (state[i])
            {
            case 0:
                /* code */
                if (counter[i] >= 1000)
                {
                    state[i] = 1; // to state debouncing
                }
                break;
            case 1:
                PIN_TOGGLE(B, LED_PIN);
                nrf_sendpayload(tx_data, 32);
                counter[i] = 0;
                state[i] = 0;
                break;
            default:
                break;
            }
        }
        delay_ms(1);
    }
}