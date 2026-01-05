#ifndef GPIO_H
#define GPIO_H

#include "std_types.h"

/* Port Definitions */
typedef enum {
    GPIO_PORTA,
    GPIO_PORTB,
    GPIO_PORTC,
    GPIO_PORTD,
    GPIO_PORTE,
    GPIO_PORTF
} GPIO_Port;

/* Pin Definitions */
#define PIN0  (1u << 0)
#define PIN1  (1u << 1)
#define PIN2  (1u << 2)
#define PIN3  (1u << 3)
#define PIN4  (1u << 4)
#define PIN5  (1u << 5)
#define PIN6  (1u << 6)
#define PIN7  (1u << 7)

/* Direction */
#define GPIO_INPUT  0u
#define GPIO_OUTPUT 1u

/* Level */
#define GPIO_LOW    0u
#define GPIO_HIGH   1u

/* Simple GPIO Functions */
void GPIO_InitPin(GPIO_Port port, uint8 pin, uint8 direction);
void GPIO_WritePin(GPIO_Port port, uint8 pin, uint8 level);
uint8 GPIO_ReadPin(GPIO_Port port, uint8 pin);
void GPIO_TogglePin(GPIO_Port port, uint8 pin);

/* Helper Macros */
#define GPIO_SET(port, pin)    GPIO_WritePin(port, pin, GPIO_HIGH)
#define GPIO_CLEAR(port, pin)  GPIO_WritePin(port, pin, GPIO_LOW)

#endif /* GPIO_H */
