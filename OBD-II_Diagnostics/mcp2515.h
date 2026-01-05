/******************************************************************************
 *
 * Module: MCP2515 - CAN Controller Driver
 *
 * File Name: mcp2515.h
 *
 * Description: Header file for MCP2515 CAN Controller Driver
 *              Communicates via SPI interface
 *              Configured for 8 MHz crystal oscillator
 *
 * Author: BodzTH
 *
 *******************************************************************************/

#ifndef MCP2515_H_
#define MCP2515_H_

#include "std_types.h"

/*******************************************************************************
 *                          MCP2515 SPI Instructions                            *
 *******************************************************************************/

#define MCP2515_CMD_RESET           0xC0
#define MCP2515_CMD_READ            0x03
#define MCP2515_CMD_WRITE           0x02
#define MCP2515_CMD_READ_RX0        0x90    /* Read RX Buffer 0 starting at RXB0SIDH */
#define MCP2515_CMD_READ_RX1        0x94    /* Read RX Buffer 1 starting at RXB1SIDH */
#define MCP2515_CMD_LOAD_TX0        0x40    /* Load TX Buffer 0 starting at TXB0SIDH */
#define MCP2515_CMD_LOAD_TX1        0x42    /* Load TX Buffer 1 starting at TXB1SIDH */
#define MCP2515_CMD_LOAD_TX2        0x44    /* Load TX Buffer 2 starting at TXB2SIDH */
#define MCP2515_CMD_RTS_TX0         0x81    /* Request to send TX0 */
#define MCP2515_CMD_RTS_TX1         0x82    /* Request to send TX1 */
#define MCP2515_CMD_RTS_TX2         0x84    /* Request to send TX2 */
#define MCP2515_CMD_RTS_ALL         0x87    /* Request to send all */
#define MCP2515_CMD_READ_STATUS     0xA0
#define MCP2515_CMD_RX_STATUS       0xB0
#define MCP2515_CMD_BIT_MODIFY      0x05

/*******************************************************************************
 *                          MCP2515 Register Addresses                          *
 *******************************************************************************/

/* Configuration Registers */
#define MCP2515_REG_CANSTAT         0x0E
#define MCP2515_REG_CANCTRL         0x0F
#define MCP2515_REG_BFPCTRL         0x0C
#define MCP2515_REG_TEC             0x1C
#define MCP2515_REG_REC             0x1D

/* CAN Timing Registers */
#define MCP2515_REG_CNF1            0x2A
#define MCP2515_REG_CNF2            0x29
#define MCP2515_REG_CNF3            0x28

/* Interrupt Registers */
#define MCP2515_REG_CANINTE         0x2B
#define MCP2515_REG_CANINTF         0x2C
#define MCP2515_REG_EFLG            0x2D

/* TX Buffer 0 Registers */
#define MCP2515_REG_TXB0CTRL        0x30
#define MCP2515_REG_TXB0SIDH        0x31
#define MCP2515_REG_TXB0SIDL        0x32
#define MCP2515_REG_TXB0EID8        0x33
#define MCP2515_REG_TXB0EID0        0x34
#define MCP2515_REG_TXB0DLC         0x35
#define MCP2515_REG_TXB0D0          0x36

/* TX Buffer 1 Registers */
#define MCP2515_REG_TXB1CTRL        0x40
#define MCP2515_REG_TXB1SIDH        0x41

/* TX Buffer 2 Registers */
#define MCP2515_REG_TXB2CTRL        0x50
#define MCP2515_REG_TXB2SIDH        0x51

/* RX Buffer 0 Registers */
#define MCP2515_REG_RXB0CTRL        0x60
#define MCP2515_REG_RXB0SIDH        0x61
#define MCP2515_REG_RXB0SIDL        0x62
#define MCP2515_REG_RXB0EID8        0x63
#define MCP2515_REG_RXB0EID0        0x64
#define MCP2515_REG_RXB0DLC         0x65
#define MCP2515_REG_RXB0D0          0x66

/* RX Buffer 1 Registers */
#define MCP2515_REG_RXB1CTRL        0x70
#define MCP2515_REG_RXB1SIDH        0x71
#define MCP2515_REG_RXB1SIDL        0x72
#define MCP2515_REG_RXB1EID8        0x73
#define MCP2515_REG_RXB1EID0        0x74
#define MCP2515_REG_RXB1DLC         0x75
#define MCP2515_REG_RXB1D0          0x76

/* Filter and Mask Registers */
#define MCP2515_REG_RXF0SIDH        0x00
#define MCP2515_REG_RXF0SIDL        0x01
#define MCP2515_REG_RXF1SIDH        0x04
#define MCP2515_REG_RXF1SIDL        0x05
#define MCP2515_REG_RXF2SIDH        0x08
#define MCP2515_REG_RXF2SIDL        0x09
#define MCP2515_REG_RXF3SIDH        0x10
#define MCP2515_REG_RXF3SIDL        0x11
#define MCP2515_REG_RXF4SIDH        0x14
#define MCP2515_REG_RXF4SIDL        0x15
#define MCP2515_REG_RXF5SIDH        0x18
#define MCP2515_REG_RXF5SIDL        0x19
#define MCP2515_REG_RXM0SIDH        0x20
#define MCP2515_REG_RXM0SIDL        0x21
#define MCP2515_REG_RXM1SIDH        0x24
#define MCP2515_REG_RXM1SIDL        0x25

/*******************************************************************************
 *                          MCP2515 Register Bits                               *
 *******************************************************************************/

/* CANCTRL Register Bits */
#define MCP2515_CANCTRL_REQOP_MASK  0xE0
#define MCP2515_CANCTRL_REQOP_NORMAL    0x00
#define MCP2515_CANCTRL_REQOP_SLEEP     0x20
#define MCP2515_CANCTRL_REQOP_LOOPBACK  0x40
#define MCP2515_CANCTRL_REQOP_LISTEN    0x60
#define MCP2515_CANCTRL_REQOP_CONFIG    0x80
#define MCP2515_CANCTRL_CLKEN       0x04
#define MCP2515_CANCTRL_CLKPRE_MASK 0x03

/* CANSTAT Register Bits */
#define MCP2515_CANSTAT_OPMOD_MASK  0xE0

/* CANINTF Register Bits */
#define MCP2515_CANINTF_RX0IF       0x01
#define MCP2515_CANINTF_RX1IF       0x02
#define MCP2515_CANINTF_TX0IF       0x04
#define MCP2515_CANINTF_TX1IF       0x08
#define MCP2515_CANINTF_TX2IF       0x10
#define MCP2515_CANINTF_ERRIF       0x20
#define MCP2515_CANINTF_WAKIF       0x40
#define MCP2515_CANINTF_MERRF       0x80

/* TXBnCTRL Register Bits */
#define MCP2515_TXB_TXREQ           0x08
#define MCP2515_TXB_TXP_MASK        0x03

/* RXBnCTRL Register Bits */
#define MCP2515_RXB_RXM_MASK        0x60
#define MCP2515_RXB_RXM_ANY         0x60    /* Receive any message */
#define MCP2515_RXB_RXM_EXT         0x40    /* Extended ID only */
#define MCP2515_RXB_RXM_STD         0x20    /* Standard ID only */
#define MCP2515_RXB_RXM_FILTER      0x00    /* Use filters */
#define MCP2515_RXB_BUKT            0x04    /* Rollover enable */

/* Status Byte Bits */
#define MCP2515_STATUS_RX0IF        0x01
#define MCP2515_STATUS_RX1IF        0x02

/*******************************************************************************
 *                          Type Definitions                                     *
 *******************************************************************************/

/* MCP2515 Status Codes */
#define MCP2515_STATUS_OK           0
#define MCP2515_STATUS_ERROR        1
#define MCP2515_STATUS_TIMEOUT      2
#define MCP2515_STATUS_TX_BUSY      3
#define MCP2515_STATUS_NO_MSG       4

/* CAN Baud Rates for 8 MHz Crystal */
typedef enum
{
    MCP2515_BAUD_500KBPS,
    MCP2515_BAUD_250KBPS,
    MCP2515_BAUD_125KBPS,
    MCP2515_BAUD_100KBPS
} MCP2515_BaudRate;

/* CAN Frame Type */
typedef enum
{
    MCP2515_FRAME_STD,      /* Standard 11-bit ID */
    MCP2515_FRAME_EXT       /* Extended 29-bit ID */
} MCP2515_FrameType;

/* CAN Message Structure */
typedef struct
{
    uint32 id;              /* Message ID (11 or 29 bits) */
    MCP2515_FrameType idType;  /* Standard or Extended */
    uint8 dlc;              /* Data Length Code (0-8) */
    uint8 data[8];          /* Data bytes */
} MCP2515_Message;

/* MCP2515 Configuration */
typedef struct
{
    MCP2515_BaudRate baudRate;
    uint8 loopbackMode;     /* TRUE for loopback testing */
} MCP2515_Config;

/*******************************************************************************
 *                          Function Prototypes                                  *
 *******************************************************************************/

/*
 * Description: Initialize MCP2515 CAN Controller
 * Parameters:
 *   - config:  Pointer to configuration structure
 * Returns:  MCP2515_STATUS_OK on success, error code otherwise
 */
uint8 MCP2515_Init(const MCP2515_Config *config);

/*
 * Description: Reset MCP2515 via SPI command
 */
void MCP2515_Reset(void);

/*
 * Description: Set MCP2515 operating mode
 * Parameters:
 *   - mode: Operating mode (NORMAL, SLEEP, LOOPBACK, LISTEN, CONFIG)
 * Returns: MCP2515_STATUS_OK on success
 */
uint8 MCP2515_SetMode(uint8 mode);

/*
 * Description: Configure receive filter
 * Parameters: 
 *   - filterNum: Filter number (0-5)
 *   - id: CAN ID to filter
 *   - idType: Standard or Extended ID
 * Returns: MCP2515_STATUS_OK on success
 */
uint8 MCP2515_ConfigureFilter(uint8 filterNum, uint32 id, MCP2515_FrameType idType);

/*
 * Description: Configure receive mask
 * Parameters: 
 *   - maskNum:  Mask number (0-1)
 *   - mask:  Mask value
 *   - idType:  Standard or Extended ID
 * Returns: MCP2515_STATUS_OK on success
 */
uint8 MCP2515_ConfigureMask(uint8 maskNum, uint32 mask, MCP2515_FrameType idType);

/*
 * Description:  Transmit CAN message
 * Parameters:
 *   - msg:  Pointer to message structure
 * Returns: MCP2515_STATUS_OK on success
 */
uint8 MCP2515_Transmit(const MCP2515_Message *msg);

/*
 * Description:  Receive CAN message (non-blocking)
 * Parameters:
 *   - msg:  Pointer to message structure to fill
 * Returns:  MCP2515_STATUS_OK if message received, MCP2515_STATUS_NO_MSG otherwise
 */
uint8 MCP2515_Receive(MCP2515_Message *msg);

/*
 * Description:  Receive CAN message with timeout
 * Parameters:
 *   - msg: Pointer to message structure to fill
 *   - timeout_ms: Timeout in milliseconds
 * Returns: MCP2515_STATUS_OK if message received, MCP2515_STATUS_TIMEOUT otherwise
 */
uint8 MCP2515_ReceiveWithTimeout(MCP2515_Message *msg, uint32 timeout_ms);

/*
 * Description: Check if message is available
 * Returns: TRUE if message available, FALSE otherwise
 */
uint8 MCP2515_MessageAvailable(void);

/*
 * Description: Read MCP2515 register
 * Parameters: 
 *   - address: Register address
 * Returns: Register value
 */
uint8 MCP2515_ReadRegister(uint8 address);

/*
 * Description: Write MCP2515 register
 * Parameters:
 *   - address: Register address
 *   - value: Value to write
 */
void MCP2515_WriteRegister(uint8 address, uint8 value);

/*
 * Description:  Modify bits in MCP2515 register
 * Parameters:
 *   - address: Register address
 *   - mask: Bit mask (1s indicate bits to modify)
 *   - value:  New values for masked bits
 */
void MCP2515_BitModify(uint8 address, uint8 mask, uint8 value);

/*
 * Description:  Get MCP2515 status
 * Returns: Status byte
 */
uint8 MCP2515_GetStatus(void);

/*
 * Description: Get MCP2515 RX status
 * Returns:  RX status byte
 */
uint8 MCP2515_GetRxStatus(void);

#endif /* MCP2515_H_ */
