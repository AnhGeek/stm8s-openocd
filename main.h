#include "spi.h"

// macro to configure a pin
#define PIN_MODE(PORT, PIN, MODE)                     \
P##PORT##_DDR_DDR##PIN = (MODE) >> 2;       \
P##PORT##_CR1_C1##PIN = ((MODE) >> 1) & 1;  \
P##PORT##_CR2_C2##PIN = (MODE) & 1

// macro to set a pin
#define PIN_WRITE(PORT, PIN, VALUE) P##PORT##_ODR##PIN = VALUE

// macro to toggle a pin
#define PIN_TOGGLE(PORT, PIN) P##PORT##_ODR ^= (1 << PIN)
                              
// macro to get state of a pin
#define PIN_READ(PORT, PIN)     P##PORT##_IDR_IDR##PIN