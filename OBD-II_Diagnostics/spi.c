/******************************************************************************
 *
 * Module: SPI - Serial Peripheral Interface Driver
 *
 * File Name:  spi.c
 *
 * Description: Source file for TM4C123GH6PM SPI Driver
 *              Configured for MCP2515 CAN Controller communication
 *
 * Author: BodzTH
 *
 *******************************************************************************/

#include "spi.h"

/*******************************************************************************
 *                          Function Definitions                                *
 *******************************************************************************/

/*
 * Description:  Initialize SPI module for MCP2515 communication
 */
void SPI_Init(void)
{
    /* Enable clock for SSI0 and GPIO Port A */
    SYSCTL_RCGCSSI_REG |= (1 << 0);     /* Enable SSI0 clock */
    SYSCTL_RCGCGPIO_REG |= (1 << 0);    /* Enable Port A clock */
    
    /* Wait for peripherals to be ready */
    while ((SYSCTL_PRSSI_REG & (1 << 0)) == 0);
    while ((SYSCTL_PRGPIO_REG & (1 << 0)) == 0);
    
    /* Disable SSI0 during configuration */
    SSI0_CR1_REG = 0x00000000;
    
    /* Configure PA2 (CLK), PA4 (MISO), PA5 (MOSI) for SSI0 */
    /* PA3 (CS) configured as GPIO output for manual control */
    GPIO_PORTA_AFSEL_REG |= (1 << 2) | (1 << 4) | (1 << 5);  /* Alternate function for SSI pins */
    GPIO_PORTA_AFSEL_REG &= ~(1 << 3);                        /* GPIO for CS */
    
    /* Configure PCTL for SSI0 function (value 2) on PA2, PA4, PA5 */
    GPIO_PORTA_PCTL_REG = (GPIO_PORTA_PCTL_REG & 0xFF0F00FF) | 0x00202200;
    
    /* Configure CS pin (PA3) as output, initially high */
    GPIO_PORTA_DIR_REG |= SPI_CS_PIN;       /* PA3 as output */
    GPIO_PORTA_DATA_REG |= SPI_CS_PIN;      /* CS high (deasserted) */
    
    /* Enable digital function on SSI pins and CS */
    GPIO_PORTA_DEN_REG |= (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);
    
    /* Configure SSI0 clock source - use system clock */
    SSI0_CC_REG = 0x00;
    
    /* Configure SSI0:
     * System clock = 16 MHz (assuming default)
     * Desired SPI clock = 1 MHz (safe for MCP2515 with 8 MHz crystal)
     * CPSDVSR = 2 (must be even, 2-254)
     * SCR = (16MHz / (1MHz * 2)) - 1 = 7
     * 
     * For 2 MHz SPI:  SCR = 3
     * For 4 MHz SPI:  SCR = 1
     */
    SSI0_CPSR_REG = 2;  /* Clock prescale divisor = 2 */
    
    /* CR0 Configuration: 
     * SCR = 7 (bits 15: 8) - Serial clock rate for 1 MHz
     * SPH = 0 (bit 7) - CPHA = 0 (data captured on first clock edge)
     * SPO = 0 (bit 6) - CPOL = 0 (clock idle low)
     * FRF = 0 (bits 5:4) - Freescale SPI frame format
     * DSS = 0x7 (bits 3:0) - 8-bit data
     */
    SSI0_CR0_REG = (7 << 8) | (0 << 7) | (0 << 6) | (0 << 4) | 0x07;
    
    /* Enable SSI0 (Master mode is default) */
    SSI0_CR1_REG = 0x00000002;  /* SSE = 1, enable SSI */
}

/*
 * Description: Assert chip select (drive low)
 */
void SPI_CS_Assert(void)
{
    GPIO_PORTA_DATA_REG &= ~SPI_CS_PIN;
}

/*
 * Description: Deassert chip select (drive high)
 */
void SPI_CS_Deassert(void)
{
    GPIO_PORTA_DATA_REG |= SPI_CS_PIN;
}

/*
 * Description: Transfer single byte over SPI
 */
uint8 SPI_Transfer(uint8 data)
{
    /* Wait until transmit FIFO is not full */
    while ((SSI0_SR_REG & SSI_SR_TNF) == 0);
    
    /* Write data to transmit FIFO */
    SSI0_DR_REG = data;
    
    /* Wait until receive FIFO is not empty */
    while ((SSI0_SR_REG & SSI_SR_RNE) == 0);
    
    /* Read and return received data */
    return (uint8)(SSI0_DR_REG & 0xFF);
}

/*
 * Description: Write single byte to SPI
 */
void SPI_Write(uint8 data)
{
    SPI_Transfer(data);
}

/*
 * Description: Read single byte from SPI
 */
uint8 SPI_Read(void)
{
    return SPI_Transfer(0xFF);
}

/*
 * Description: Transfer multiple bytes over SPI
 */
void SPI_TransferBuffer(const uint8 *txBuffer, uint8 *rxBuffer, uint16 length)
{
    uint16 i;
    
    for (i = 0; i < length; i++)
    {
        uint8 txByte = (txBuffer != NULL_PTR) ? txBuffer[i] :  0xFF;
        uint8 rxByte = SPI_Transfer(txByte);
        
        if (rxBuffer != NULL_PTR)
        {
            rxBuffer[i] = rxByte;
        }
    }
}
