#include "pushbutton.h"
#include "tm4c123gh6pm_registers.h"

/* SW1 Configuration - PF4 */
#define SW1_PIN      (1u << 4)  /* PF4 */

static boolean sw1LastState = FALSE;

void Button_Init(void) {
    /* Enable clock for PORTF */
    SYSCTL_RCGCGPIO_REG |= (1u << 5u);
    
    /* Wait for clock to be ready */
    while((SYSCTL_PRGPIO_REG & (1u << 5u)) == 0u);
    
    /* Unlock PORTF to allow configuration of PF4 */
    GPIO_PORTF_LOCK_REG = 0x4C4F434Bu; /* Unlock key */
    GPIO_PORTF_CR_REG = 0x1Fu;         /* Allow changes to PF4-0 */
    
    /* Configure PF4 (SW1) as input */
    GPIO_PORTF_DIR_REG &= ~SW1_PIN;
    
    /* Enable pull-up resistor for SW1 (active low) */
    GPIO_PORTF_PUR_REG |= SW1_PIN;
    
    /* Enable digital function for SW1 */
    GPIO_PORTF_DEN_REG |= SW1_PIN;
    
    /* Read initial state */
    sw1LastState = Button_IsPressed();
}

boolean Button_IsPressed(void) {
    /* SW1 is active low (0 when pressed) */
    /* Check if SW1 pin is LOW (pressed) */
    if((GPIO_PORTF_DATA_REG & SW1_PIN) == 0u) {
        return TRUE;  /* Button is pressed */
    } else {
        return FALSE; /* Button is not pressed */
    }
}

boolean Button_WasPressed(void) {
    boolean currentState = Button_IsPressed();
    boolean wasPressed = FALSE;
    
    /* Detect rising edge (button was just released) */
    /* This detects when button goes from pressed to not pressed */
    if(sw1LastState == TRUE && currentState == FALSE) {
        wasPressed = TRUE;
    }
    
    /* Update last state */
    sw1LastState = currentState;
    
    return wasPressed;
}
