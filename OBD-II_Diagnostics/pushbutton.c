#include "pushbutton.h"
#include "gpio.h"
#include "tm4c123gh6pm_registers.h"

/* Button Configuration */
#define BUTTON_PORT  GPIO_PORTF
#define BUTTON_PIN   PIN4  /* PF4 */

static uint8 buttonLastState = 1; /* Button is active low, so start as not pressed */

void Button_Init(void) {
    /* Initialize button as input with pull-up */
    GPIO_InitPin(BUTTON_PORT, BUTTON_PIN, GPIO_INPUT);
    
    /* Configure pull-up in hardware registers directly */
    volatile uint32* portFBase = (volatile uint32*)0x40025000;
    portFBase[3] |= BUTTON_PIN;  /* PUR register offset */
    
    /* Read initial state */
    buttonLastState = GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN);
}

uint8 Button_IsPressed(void) {
    /* Button is active low (0 when pressed) */
    if(GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN) == 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

uint8 Button_WasPressed(void) {
    uint8 currentState = Button_IsPressed();
    uint8 wasPressed = FALSE;
    
    /* Check for rising edge (button was just released) */
    if(buttonLastState == TRUE && currentState == FALSE) {
        wasPressed = TRUE;
    }
    
    buttonLastState = currentState;
    return wasPressed;
}
