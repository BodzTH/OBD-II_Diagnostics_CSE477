
#include "tm4c123gh6pm_registers.h"

#define NUMBER_OF_ITERATIONS_PER_ONE_MILI_SECOND 364

void Delay_MS(unsigned long long n)
{
    volatile unsigned long long count = 0;
    while(count++ < (NUMBER_OF_ITERATIONS_PER_ONE_MILI_SECOND * n) );
}

/* Enable PF1, PF2 and PF3 (RED, Blue and Green LEDs) */
void Leds_Init(void)
{
    GPIO_PORTF_AMSEL_REG &= 0xF1;         /* Disable Analog on PF1, PF2 and PF3 */
    GPIO_PORTF_PCTL_REG  &= 0xFFFF000F;   /* Clear PMCx bits for PF1, PF2 and PF3 to use it as GPIO pin */
    GPIO_PORTF_DIR_REG   |= 0x0E;         /* Configure PF1, PF2 and PF3 as output pin */
    GPIO_PORTF_AFSEL_REG &= 0xF1;         /* Disable alternative function on PF1, PF2 and PF3 */
    GPIO_PORTF_DEN_REG   |= 0x0E;         /* Enable Digital I/O on PF1, PF2 and PF3 */
    GPIO_PORTF_DATA_REG  &= 0xF1;         /* Clear bit 0, 1 and 2 in Data register to turn off the leds */
}

/* Enable PF4 (SW1) */
void SW1_Init(void)
{
    GPIO_PORTF_AMSEL_REG &= ~(1<<4);      /* Disable Analog on PF4 */
    GPIO_PORTF_PCTL_REG  &= 0xFFF0FFFF;   /* Clear PMCx bits for PF4 to use it as GPIO pin */
    GPIO_PORTF_DIR_REG   &= ~(1<<4);      /* Configure PF4 as input pin */
    GPIO_PORTF_AFSEL_REG &= ~(1<<4);      /* Disable alternative function on PF4 */
    GPIO_PORTF_PUR_REG   |= (1<<4);       /* Enable pull-up on PF4 */
    GPIO_PORTF_DEN_REG   |= (1<<4);       /* Enable Digital I/O on PF4 */
}

/* Enable PF0 (SW2) */
void SW2_Init(void)
{
    GPIO_PORTF_LOCK_REG   = 0x4C4F434B;   /* Unlock the GPIO_PORTF_CR_REG */
    GPIO_PORTF_CR_REG    |= (1<<0);       /* Enable changes on PF0 */
    GPIO_PORTF_AMSEL_REG &= ~(1<<0);      /* Disable Analog on PF0 */
    GPIO_PORTF_PCTL_REG  &= 0xFFFFFFF0;   /* Clear PMCx bits for PF0 to use it as GPIO pin */
    GPIO_PORTF_DIR_REG   &= ~(1<<0);      /* Configure PF0 as input pin */
    GPIO_PORTF_AFSEL_REG &= ~(1<<0);      /* Disable alternative function on PF0 */
    GPIO_PORTF_PUR_REG   |= (1<<0);       /* Enable pull-up on PF0 */
    GPIO_PORTF_DEN_REG   |= (1<<0);       /* Enable Digital I/O on PF0 */
}

int main(void)
{
    /* Enable clock for PORTF and wait for clock to start */  
    SYSCTL_RCGCGPIO_REG |= 0x20;
    while(!(SYSCTL_PRGPIO_REG & 0x20));
    
    /* Initialize the LEDs as GPIO Pins */
    Leds_Init();
    
    /* Initialize the SW1 and SW2 as GPIO Pins */
    SW1_Init();
    SW2_Init();
    
    while(1)
    {
        if(!(GPIO_PORTF_DATA_REG & (1<<4)))
        {
            Delay_MS(10);
            if(!(GPIO_PORTF_DATA_REG & (1<<4)))
            {
                GPIO_PORTF_DATA_REG = (GPIO_PORTF_DATA_REG & 0xF1) | 0x02; /* Turn on the Red LED and disable the others */
                Delay_MS(500);
                GPIO_PORTF_DATA_REG = (GPIO_PORTF_DATA_REG & 0xF1) | 0x04; /* Turn on the Blue LED and disable the others */
                Delay_MS(500);
            }
        }
        else if(!(GPIO_PORTF_DATA_REG & (1<<0)))
        {
            Delay_MS(10);
            if(!(GPIO_PORTF_DATA_REG & (1<<0)))
            {
                GPIO_PORTF_DATA_REG = (GPIO_PORTF_DATA_REG & 0xF1) | 0x04; /* Turn on the Blue LED and disable the others */
                Delay_MS(500);
                GPIO_PORTF_DATA_REG = (GPIO_PORTF_DATA_REG & 0xF1) | 0x08; /* Turn on the Green LED and disable the others */
                Delay_MS(500);
            }
        }
        else
        {
            GPIO_PORTF_DATA_REG = GPIO_PORTF_DATA_REG & 0xF1; /* Turn off all leds in case no Switches are pressed */
        }
          
    }
}
