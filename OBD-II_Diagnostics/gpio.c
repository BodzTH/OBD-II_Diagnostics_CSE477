#include "gpio.h"

static volatile uint32_t* portAddresses[] = {
    (volatile uint32_t*)0x40004000,
    (volatile uint32_t*)0x40005000,
    (volatile uint32_t*)0x40006000,
    (volatile uint32_t*)0x40007000,
    (volatile uint32_t*)0x40024000,
    (volatile uint32_t*)0x40025000
};

void GPIO_InitPort(GPIO_Port port, uint8_t pins, GPIO_Direction dir) {
    SYSCTL_RCGCGPIO_R |= (1 << port);
    while((SYSCTL_PRGPIO_R & (1 << port)) == 0);
    
    volatile uint32_t* portBase = portAddresses[port];
    
    if(dir == GPIO_DIR_OUTPUT) portBase[GPIO_O_DIR] |= pins;
    else portBase[GPIO_O_DIR] &= ~pins;
    
    portBase[GPIO_O_DEN] |= pins;
}

void GPIO_WritePin(GPIO_Port port, uint8_t pin, GPIO_Level level) {
    volatile uint32_t* portBase = portAddresses[port];
    portBase[(pin << 2)] = (level) ? pin : 0;
}

GPIO_Level GPIO_ReadPin(GPIO_Port port, uint8_t pin) {
    volatile uint32_t* portBase = portAddresses[port];
    return (portBase[(pin << 2)] & pin) ? GPIO_HIGH : GPIO_LOW;
}

void GPIO_TogglePin(GPIO_Port port, uint8_t pin) {
    if(GPIO_ReadPin(port, pin) == GPIO_HIGH)
        GPIO_WritePin(port, pin, GPIO_LOW);
    else
        GPIO_WritePin(port, pin, GPIO_HIGH);
}

void GPIO_SetPull(GPIO_Port port, uint8_t pin, GPIO_Pull pull) {
    volatile uint32_t* portBase = portAddresses[port];
    
    if(pull == GPIO_PULL_UP)
        portBase[GPIO_O_PUR] |= pin;
    else if(pull == GPIO_PULL_DOWN)
        portBase[GPIO_O_PDR] |= pin;
    else {
        portBase[GPIO_O_PUR] &= ~pin;
        portBase[GPIO_O_PDR] &= ~pin;
    }
}