#include "gpio.h"
#include "tm4c123gh6pm_registers.h"

/* Get the correct GPIO data register for a specific pin */
static volatile uint32* getDataReg(GPIO_Port port) {
    switch(port) {
        case GPIO_PORTA: return &GPIO_PORTA_DATA_REG;
        case GPIO_PORTB: return &GPIO_PORTB_DATA_REG;
        case GPIO_PORTC: return &GPIO_PORTC_DATA_REG;
        case GPIO_PORTD: return &GPIO_PORTD_DATA_REG;
        case GPIO_PORTE: return &GPIO_PORTE_DATA_REG;
        case GPIO_PORTF: return &GPIO_PORTF_DATA_REG;
        default: return &GPIO_PORTA_DATA_REG;
    }
}

/* Get the correct GPIO direction register for a port */
static volatile uint32* getDirReg(GPIO_Port port) {
    switch(port) {
        case GPIO_PORTA: return &GPIO_PORTA_DIR_REG;
        case GPIO_PORTB: return &GPIO_PORTB_DIR_REG;
        case GPIO_PORTC: return &GPIO_PORTC_DIR_REG;
        case GPIO_PORTD: return &GPIO_PORTD_DIR_REG;
        case GPIO_PORTE: return &GPIO_PORTE_DIR_REG;
        case GPIO_PORTF: return &GPIO_PORTF_DIR_REG;
        default: return &GPIO_PORTA_DIR_REG;
    }
}

/* Get the correct GPIO DEN register for a port */
static volatile uint32* getDenReg(GPIO_Port port) {
    switch(port) {
        case GPIO_PORTA: return &GPIO_PORTA_DEN_REG;
        case GPIO_PORTB: return &GPIO_PORTB_DEN_REG;
        case GPIO_PORTC: return &GPIO_PORTC_DEN_REG;
        case GPIO_PORTD: return &GPIO_PORTD_DEN_REG;
        case GPIO_PORTE: return &GPIO_PORTE_DEN_REG;
        case GPIO_PORTF: return &GPIO_PORTF_DEN_REG;
        default: return &GPIO_PORTA_DEN_REG;
    }
}

/* Get the correct GPIO PUR register for a port */
static volatile uint32* getPurReg(GPIO_Port port) {
    switch(port) {
        case GPIO_PORTA: return &GPIO_PORTA_PUR_REG;
        case GPIO_PORTB: return &GPIO_PORTB_PUR_REG;
        case GPIO_PORTC: return &GPIO_PORTC_PUR_REG;
        case GPIO_PORTD: return &GPIO_PORTD_PUR_REG;
        case GPIO_PORTE: return &GPIO_PORTE_PUR_REG;
        case GPIO_PORTF: return &GPIO_PORTF_PUR_REG;
        default: return &GPIO_PORTA_PUR_REG;
    }
}

void GPIO_InitPin(GPIO_Port port, uint8 pin, uint8 direction) {
    /* Enable clock for the GPIO port */
    SYSCTL_RCGCGPIO_REG |= (1u << port);
    
    /* Wait for clock to be ready */
    while((SYSCTL_PRGPIO_REG & (1u << port)) == 0u);
    
    volatile uint32* dirReg = getDirReg(port);
    volatile uint32* denReg = getDenReg(port);
    
    /* Configure direction */
    if(direction == GPIO_OUTPUT) {
        *dirReg |= pin;
    } else {
        *dirReg &= ~pin;
    }
    
    /* Enable digital function */
    *denReg |= pin;
}

void GPIO_WritePin(GPIO_Port port, uint8 pin, uint8 level) {
    volatile uint32* dataReg = getDataReg(port);
    
    if(level == GPIO_HIGH) {
        *dataReg |= pin;
    } else {
        *dataReg &= ~pin;
    }
}

uint8 GPIO_ReadPin(GPIO_Port port, uint8 pin) {
    volatile uint32* dataReg = getDataReg(port);
    
    if((*dataReg & pin) != 0u) {
        return GPIO_HIGH;
    } else {
        return GPIO_LOW;
    }
}

void GPIO_TogglePin(GPIO_Port port, uint8 pin) {
    volatile uint32* dataReg = getDataReg(port);
    *dataReg ^= pin;
}
