#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include "tm4c123gh6pm.h"

typedef enum {
    GPIO_PORTA = 0,
    GPIO_PORTB,
    GPIO_PORTC,
    GPIO_PORTD,
    GPIO_PORTE,
    GPIO_PORTF
} GPIO_Port;

typedef enum {
    GPIO_DIR_INPUT = 0,
    GPIO_DIR_OUTPUT = 1
} GPIO_Direction;

typedef enum {
    GPIO_LOW = 0,
    GPIO_HIGH = 1
} GPIO_Level;

typedef enum {
    GPIO_PULL_NONE = 0,
    GPIO_PULL_UP,
    GPIO_PULL_DOWN
} GPIO_Pull;

typedef enum {
    GPIO_DRIVE_2MA = 0,
    GPIO_DRIVE_4MA,
    GPIO_DRIVE_8MA,
    GPIO_DRIVE_8MA_SLEW
} GPIO_DriveStrength;

void GPIO_InitPort(GPIO_Port port, uint8_t pins, GPIO_Direction dir);
void GPIO_WritePin(GPIO_Port port, uint8_t pin, GPIO_Level level);
void GPIO_TogglePin(GPIO_Port port, uint8_t pin);
GPIO_Level GPIO_ReadPin(GPIO_Port port, uint8_t pin);
void GPIO_WritePort(GPIO_Port port, uint8_t value);
uint8_t GPIO_ReadPort(GPIO_Port port);
void GPIO_SetPull(GPIO_Port port, uint8_t pin, GPIO_Pull pull);
void GPIO_SetDriveStrength(GPIO_Port port, uint8_t pin, GPIO_DriveStrength strength);
void GPIO_EnableDigital(GPIO_Port port, uint8_t pins);
void GPIO_EnableAltFunction(GPIO_Port port, uint8_t pins, uint32_t altFunction);

#define GPIO_SET(port, pin)     GPIO_WritePin(port, pin, GPIO_HIGH)
#define GPIO_CLEAR(port, pin)   GPIO_WritePin(port, pin, GPIO_LOW)
#define GPIO_TOGGLE(port, pin)  GPIO_TogglePin(port, pin)
#define GPIO_GET(port, pin)     GPIO_ReadPin(port, pin)

#endif