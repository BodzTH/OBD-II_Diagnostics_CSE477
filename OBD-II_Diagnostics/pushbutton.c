#include "pushbutton.h"

static uint8_t lastState = 1;

void Button_Init(void) {
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;
    while((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5) == 0);
    
    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R = 0x1F;
    
    GPIO_PORTF_DIR_R &= ~SW1_PIN;
    GPIO_PORTF_PUR_R |= SW1_PIN;
    GPIO_PORTF_DEN_R |= SW1_PIN;
    
    lastState = Button_Read();
}

uint8_t Button_Read(void) {
    return (GPIO_PORTF_DATA_R & SW1_PIN) ? 0 : 1;
}

uint8_t Button_IsPressed(void) {
    return Button_Read();
}

uint8_t Button_WasPressed(void) {
    uint8_t current = Button_Read();
    uint8_t pressed = (lastState == 0 && current == 1);
    lastState = current;
    return pressed;
}

uint8_t Button_WasReleased(void) {
    uint8_t current = Button_Read();
    uint8_t released = (lastState == 1 && current == 0);
    lastState = current;
    return released;
}