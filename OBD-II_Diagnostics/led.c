#include "led.h"

void LED_Init(void) {
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;
    while((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5) == 0);
    
    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R = 0x1F;
    
    GPIO_PORTF_DIR_R |= (RED_LED | BLUE_LED | GREEN_LED);
    GPIO_PORTF_DEN_R |= (RED_LED | BLUE_LED | GREEN_LED);
    LED_AllOff();
}

void LED_On(uint8_t color) {
    GPIO_PORTF_DATA_R |= color;
}

void LED_Off(uint8_t color) {
    GPIO_PORTF_DATA_R &= ~color;
}

void LED_Toggle(uint8_t color) {
    GPIO_PORTF_DATA_R ^= color;
}

void LED_Set(uint8_t color, uint8_t state) {
    if(state) LED_On(color);
    else LED_Off(color);
}

void LED_AllOff(void) {
    GPIO_PORTF_DATA_R &= ~(RED_LED | BLUE_LED | GREEN_LED);
}
