/******************************************************************************
 *
 * Module: SPI - Serial Peripheral Interface Driver
 *
 * File Name: spi.h
 *
 * Description:  Header file for TM4C123GH6PM SPI Driver
 *              Configured for MCP2515 CAN Controller communication
 *
 * Author: BodzTH
 *
 *******************************************************************************/

#ifndef SPI_H_
#define SPI_H_

#include "std_types.h"

/*******************************************************************************
 *                          SSI0 Register Definitions                           *
 *******************************************************************************/

/* SSI0 Base Address:  0x40008000 - Using Port A pins */
#define SSI0_CR0_REG              (*((volatile uint32 *)0x40008000))
#define SSI0_CR1_REG              (*((volatile uint32 *)0x40008004))
#define SSI0_DR_REG               (*((volatile uint32 *)0x40008008))
#define SSI0_SR_REG               (*((volatile uint32 *)0x4000800C))
#define SSI0_CPSR_REG             (*((volatile uint32 *)0x40008010))
#define SSI0_IM_REG               (*((volatile uint32 *)0x40008014))
#define SSI0_RIS_REG              (*((volatile uint32 *)0x40008018))
#define SSI0_MIS_REG              (*((volatile uint32 *)0x4000801C))
#define SSI0_ICR_REG              (*((volatile uint32 *)0x40008020))
#define SSI0_DMACTL_REG           (*((volatile uint32 *)0x40008024))
#define SSI0_CC_REG               (*((volatile uint32 *)0x40008FC8))

/* System Control Registers */
#define SYSCTL_RCGCSSI_REG        (*((volatile uint32 *)0x400FE61C))
#define SYSCTL_RCGCGPIO_REG       (*((volatile uint32 *)0x400FE608))
#define SYSCTL_PRSSI_REG          (*((volatile uint32 *)0x400FEA1C))
#define SYSCTL_PRGPIO_REG         (*((volatile uint32 *)0x400FEA08))

/* GPIO Port A Registers (for SSI0) */
#define GPIO_PORTA_AFSEL_REG      (*((volatile uint32 *)0x40004420))
#define GPIO_PORTA_PCTL_REG       (*((volatile uint32 *)0x4000452C))
#define GPIO_PORTA_DEN_REG        (*((volatile uint32 *)0x4000451C))
#define GPIO_PORTA_DIR_REG        (*((volatile uint32 *)0x40004400))
#define GPIO_PORTA_DATA_REG       (*((volatile uint32 *)0x400043FC))
#define GPIO_PORTA_PUR_REG        (*((volatile uint32 *)0x40004510))

/*******************************************************************************
 *                          SSI Status Register Bits                            *
 *******************************************************************************/

#define SSI_SR_TFE                (1 << 0)    /* Transmit FIFO Empty */
#define SSI_SR_TNF                (1 << 1)    /* Transmit FIFO Not Full */
#define SSI_SR_RNE                (1 << 2)    /* Receive FIFO Not Empty */
#define SSI_SR_RFF                (1 << 3)    /* Receive FIFO Full */
#define SSI_SR_BSY                (1 << 4)    /* SSI Busy */

/*******************************************************************************
 *                          Pin Definitions                                      *
 *******************************************************************************/

/* SSI0 Pins on Port A: 
 * PA2 - SSI0Clk (SCK)
 * PA3 - SSI0Fss (CS) - We'll control this manually as GPIO
 * PA4 - SSI0Rx  (MISO)
 * PA5 - SSI0Tx  (MOSI)
 */
#define SPI_CS_PIN                (1 << 3)    /* PA3 for Chip Select */

/*******************************************************************************
 *                          Type Definitions                                     *
 *******************************************************************************/

/* SPI Status */
#define SPI_STATUS_OK             0
#define SPI_STATUS_ERROR          1
#define SPI_STATUS_BUSY           2
#define SPI_STATUS_TIMEOUT        3

/*******************************************************************************
 *                          Function Prototypes                                  *
 *******************************************************************************/

/*
 * Description:  Initialize SPI module for MCP2515 communication
 *              Configures SSI0 for 8-bit, Mode 0,0 (CPOL=0, CPHA=0)
 *              Clock speed configurable (typically 1-10 MHz for MCP2515)
 */
void SPI_Init(void);

/*
 * Description: Transfer single byte over SPI (send and receive)
 * Parameters:
 *   - data:  Byte to transmit
 * Returns:  Received byte
 */
uint8 SPI_Transfer(uint8 data);

/*
 * Description: Assert chip select (drive low)
 */
void SPI_CS_Assert(void);

/*
 * Description:  Deassert chip select (drive high)
 */
void SPI_CS_Deassert(void);

/*
 * Description: Transfer multiple bytes over SPI
 * Parameters: 
 *   - txBuffer: Pointer to transmit buffer
 *   - rxBuffer:  Pointer to receive buffer (can be NULL if not needed)
 *   - length: Number of bytes to transfer
 */
void SPI_TransferBuffer(const uint8 *txBuffer, uint8 *rxBuffer, uint16 length);

/*
 * Description: Write single byte to SPI (ignore received data)
 * Parameters:
 *   - data: Byte to transmit
 */
void SPI_Write(uint8 data);

/*
 * Description: Read single byte from SPI (send 0xFF as dummy)
 * Returns: Received byte
 */
uint8 SPI_Read(void);

#endif /* SPI_H_ */
