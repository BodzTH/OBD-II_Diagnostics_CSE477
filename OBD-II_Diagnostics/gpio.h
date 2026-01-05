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
#define PIN0  (1 << 0)
#define PIN1  (1 << 1)
#define PIN2  (1 << 2)
#define PIN3  (1 << 3)
#define PIN4  (1 << 4)
#define PIN5  (1 << 5)
#define PIN6  (1 << 6)
#define PIN7  (1 << 7)

/* Direction */
#define GPIO_INPUT  0
#define GPIO_OUTPUT 1

/* Level */
#define GPIO_LOW    0
#define GPIO_HIGH   1

/* Pull Configuration */
#define GPIO_PULL_NONE   0
#define GPIO_PULL_UP     1
#define GPIO_PULL_DOWN   2

/* Simple GPIO Functions */
void GPIO_InitPin(GPIO_Port port, uint8 pin, uint8 direction);
void GPIO_WritePin(GPIO_Port port, uint8 pin, uint8 level);
uint8 GPIO_ReadPin(GPIO_Port port, uint8 pin);
void GPIO_TogglePin(GPIO_Port port, uint8 pin);

/* Helper Macros */
#define GPIO_SET(port, pin)    GPIO_WritePin(port, pin, GPIO_HIGH)
#define GPIO_CLEAR(port, pin)  GPIO_WritePin(port, pin, GPIO_LOW)

#endif
