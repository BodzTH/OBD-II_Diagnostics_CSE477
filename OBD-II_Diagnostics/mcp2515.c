/******************************************************************************
 *
 * Module: MCP2515 - CAN Controller Driver
 *
 * File Name: mcp2515.c
 *
 * Description: Source file for MCP2515 CAN Controller Driver
 *              Communicates via SPI interface
 *              Configured for 8 MHz crystal oscillator
 *
 * Author: BodzTH
 *
 *******************************************************************************/

#include "mcp2515.h"
#include "spi.h"
#include "delay.h"

/*******************************************************************************
 *                          Private Function Prototypes                         *
 *******************************************************************************/

static void MCP2515_ReadRegisters(uint8 address, uint8 *buffer, uint8 length);
static void MCP2515_WriteRegisters(uint8 address, const uint8 *buffer, uint8 length);
static uint8 MCP2515_FindFreeTxBuffer(void);

/*******************************************************************************
 *                          Function Definitions                                *
 *******************************************************************************/

/*
 * Description: Reset MCP2515 via SPI command
 */
void MCP2515_Reset(void)
{
    SPI_CS_Assert();
    SPI_Write(MCP2515_CMD_RESET);
    SPI_CS_Deassert();
    
    /* Wait for reset to complete (minimum 2ms after reset) */
    Delay_MS(10);
}

/*
 * Description:  Read MCP2515 register
 */
uint8 MCP2515_ReadRegister(uint8 address)
{
    uint8 value;
    
    SPI_CS_Assert();
    SPI_Write(MCP2515_CMD_READ);
    SPI_Write(address);
    value = SPI_Read();
    SPI_CS_Deassert();
    
    return value;
}

/*
 * Description: Read multiple MCP2515 registers
 */
static void MCP2515_ReadRegisters(uint8 address, uint8 *buffer, uint8 length)
{
    uint8 i;
    
    SPI_CS_Assert();
    SPI_Write(MCP2515_CMD_READ);
    SPI_Write(address);
    for (i = 0; i < length; i++)
    {
        buffer[i] = SPI_Read();
    }
    SPI_CS_Deassert();
}

/*
 * Description:  Write MCP2515 register
 */
void MCP2515_WriteRegister(uint8 address, uint8 value)
{
    SPI_CS_Assert();
    SPI_Write(MCP2515_CMD_WRITE);
    SPI_Write(address);
    SPI_Write(value);
    SPI_CS_Deassert();
}

/*
 * Description: Write multiple MCP2515 registers
 */
static void MCP2515_WriteRegisters(uint8 address, const uint8 *buffer, uint8 length)
{
    uint8 i;
    
    SPI_CS_Assert();
    SPI_Write(MCP2515_CMD_WRITE);
    SPI_Write(address);
    for (i = 0; i < length; i++)
    {
        SPI_Write(buffer[i]);
    }
    SPI_CS_Deassert();
}

/*
 * Description:  Modify bits in MCP2515 register
 */
void MCP2515_BitModify(uint8 address, uint8 mask, uint8 value)
{
    SPI_CS_Assert();
    SPI_Write(MCP2515_CMD_BIT_MODIFY);
    SPI_Write(address);
    SPI_Write(mask);
    SPI_Write(value);
    SPI_CS_Deassert();
}

/*
 * Description: Get MCP2515 status byte
 */
uint8 MCP2515_GetStatus(void)
{
    uint8 status;
    
    SPI_CS_Assert();
    SPI_Write(MCP2515_CMD_READ_STATUS);
    status = SPI_Read();
    SPI_CS_Deassert();
    
    return status;
}

/*
 * Description:  Get MCP2515 RX status byte
 */
uint8 MCP2515_GetRxStatus(void)
{
    uint8 status;
    
    SPI_CS_Assert();
    SPI_Write(MCP2515_CMD_RX_STATUS);
    status = SPI_Read();
    SPI_CS_Deassert();
    
    return status;
}

/*
 * Description: Set MCP2515 operating mode
 */
uint8 MCP2515_SetMode(uint8 mode)
{
    uint32 timeout = 100;
    
    /* Request mode change */
    MCP2515_BitModify(MCP2515_REG_CANCTRL, MCP2515_CANCTRL_REQOP_MASK, mode);
    
    /* Wait for mode change to take effect */
    while (timeout--)
    {
        uint8 newMode = MCP2515_ReadRegister(MCP2515_REG_CANSTAT) & MCP2515_CANSTAT_OPMOD_MASK;
        if (newMode == mode)
        {
            return MCP2515_STATUS_OK;
        }
        Delay_MS(1);
    }
    
    return MCP2515_STATUS_TIMEOUT;
}

/*
 * Description: Initialize MCP2515 CAN Controller
 */
uint8 MCP2515_Init(const MCP2515_Config *config)
{
    uint8 cnf1, cnf2, cnf3;
    
    /* Initialize SPI first */
    SPI_Init();
    
    /* Reset MCP2515 */
    MCP2515_Reset();
    
    /* Set configuration mode */
    if (MCP2515_SetMode(MCP2515_CANCTRL_REQOP_CONFIG) != MCP2515_STATUS_OK)
    {
        return MCP2515_STATUS_ERROR;
    }
    
    /* Configure bit timing for 8 MHz crystal
     * 
     * For 500 kbps with 8 MHz crystal:
     * TQ = 2 * (BRP + 1) / 8MHz
     * Nominal Bit Time = TQ * (1 + PropSeg + PS1 + PS2)
     * 
     * BRP = 0, TQ = 250ns
     * PropSeg = 1, PS1 = 3, PS2 = 3
     * NBT = 250ns * 8 = 2us = 500 kbps
     * Sample point at 75%
     */
    switch (config->baudRate)
    {
        case MCP2515_BAUD_500KBPS:
            /* 8 TQ, BRP=0, PropSeg=1, PS1=3, PS2=3 */
            cnf1 = 0x00;    /* SJW=1, BRP=0 */
            cnf2 = 0x90;    /* BTLMODE=1, SAM=0, PHSEG1=2, PRSEG=0 */
            cnf3 = 0x02;    /* PHSEG2=2 */
            break;
            
        case MCP2515_BAUD_250KBPS: 
            /* 16 TQ, BRP=0 */
            cnf1 = 0x00;    /* SJW=1, BRP=0 */
            cnf2 = 0xB8;    /* BTLMODE=1, SAM=0, PHSEG1=7, PRSEG=0 */
            cnf3 = 0x05;    /* PHSEG2=5 */
            break;
            
        case MCP2515_BAUD_125KBPS:
            /* 16 TQ, BRP=1 */
            cnf1 = 0x01;    /* SJW=1, BRP=1 */
            cnf2 = 0xB8;
            cnf3 = 0x05;
            break;
            
        case MCP2515_BAUD_100KBPS:
            /* 16 TQ, BRP=4 */
            cnf1 = 0x04;    /* SJW=1, BRP=4 */
            cnf2 = 0xB8;
            cnf3 = 0x05;
            break;
            
        default: 
            return MCP2515_STATUS_ERROR;
    }
    
    MCP2515_WriteRegister(MCP2515_REG_CNF1, cnf1);
    MCP2515_WriteRegister(MCP2515_REG_CNF2, cnf2);
    MCP2515_WriteRegister(MCP2515_REG_CNF3, cnf3);
    
    /* Clear all filters and masks - accept all messages */
    MCP2515_WriteRegister(MCP2515_REG_RXM0SIDH, 0x00);
    MCP2515_WriteRegister(MCP2515_REG_RXM0SIDL, 0x00);
    MCP2515_WriteRegister(MCP2515_REG_RXM1SIDH, 0x00);
    MCP2515_WriteRegister(MCP2515_REG_RXM1SIDL, 0x00);
    
    /* Configure RX buffer 0: receive any message, rollover to RXB1 */
    MCP2515_WriteRegister(MCP2515_REG_RXB0CTRL, MCP2515_RXB_RXM_ANY | MCP2515_RXB_BUKT);
    
    /* Configure RX buffer 1: receive any message */
    MCP2515_WriteRegister(MCP2515_REG_RXB1CTRL, MCP2515_RXB_RXM_ANY);
    
    /* Clear interrupt flags */
    MCP2515_WriteRegister(MCP2515_REG_CANINTF, 0x00);
    
    /* Enable RX interrupts */
    MCP2515_WriteRegister(MCP2515_REG_CANINTE, 
                          MCP2515_CANINTF_RX0IF | MCP2515_CANINTF_RX1IF);
    
    /* Set operating mode */
    if (config->loopbackMode)
    {
        return MCP2515_SetMode(MCP2515_CANCTRL_REQOP_LOOPBACK);
    }
    else
    {
        return MCP2515_SetMode(MCP2515_CANCTRL_REQOP_NORMAL);
    }
}

/*
 * Description: Configure receive filter
 */
uint8 MCP2515_ConfigureFilter(uint8 filterNum, uint32 id, MCP2515_FrameType idType)
{
    uint8 sidh, sidl;
    uint8 regAddr;
    
    /* Must be in configuration mode */
    if (MCP2515_SetMode(MCP2515_CANCTRL_REQOP_CONFIG) != MCP2515_STATUS_OK)
    {
        return MCP2515_STATUS_ERROR;
    }
    
    /* Calculate register address based on filter number */
    switch (filterNum)
    {
        case 0: regAddr = MCP2515_REG_RXF0SIDH; break;
        case 1: regAddr = MCP2515_REG_RXF1SIDH; break;
        case 2: regAddr = MCP2515_REG_RXF2SIDH; break;
        case 3: regAddr = MCP2515_REG_RXF3SIDH; break;
        case 4: regAddr = MCP2515_REG_RXF4SIDH; break;
        case 5: regAddr = MCP2515_REG_RXF5SIDH; break;
        default: return MCP2515_STATUS_ERROR;
    }
    
    if (idType == MCP2515_FRAME_STD)
    {
        /* Standard ID:  11 bits */
        sidh = (uint8)(id >> 3);
        sidl = (uint8)((id & 0x07) << 5);
    }
    else
    {
        /* Extended ID: 29 bits */
        sidh = (uint8)(id >> 21);
        sidl = (uint8)((id >> 13) & 0xE0) | 0x08 | (uint8)((id >> 16) & 0x03);
    }
    
    MCP2515_WriteRegister(regAddr, sidh);
    MCP2515_WriteRegister(regAddr + 1, sidl);
    
    if (idType == MCP2515_FRAME_EXT)
    {
        MCP2515_WriteRegister(regAddr + 2, (uint8)(id >> 8));
        MCP2515_WriteRegister(regAddr + 3, (uint8)id);
    }
    
    /* Return to normal mode */
    return MCP2515_SetMode(MCP2515_CANCTRL_REQOP_NORMAL);
}

/*
 * Description: Configure receive mask
 */
uint8 MCP2515_ConfigureMask(uint8 maskNum, uint32 mask, MCP2515_FrameType idType)
{
    uint8 sidh, sidl;
    uint8 regAddr;
    
    /* Must be in configuration mode */
    if (MCP2515_SetMode(MCP2515_CANCTRL_REQOP_CONFIG) != MCP2515_STATUS_OK)
    {
        return MCP2515_STATUS_ERROR;
    }
    
    if (maskNum == 0)
    {
        regAddr = MCP2515_REG_RXM0SIDH;
    }
    else if (maskNum == 1)
    {
        regAddr = MCP2515_REG_RXM1SIDH;
    }
    else
    {
        return MCP2515_STATUS_ERROR;
    }
    
    if (idType == MCP2515_FRAME_STD)
    {
        sidh = (uint8)(mask >> 3);
        sidl = (uint8)((mask & 0x07) << 5);
    }
    else
    {
        sidh = (uint8)(mask >> 21);
        sidl = (uint8)((mask >> 13) & 0xE0) | (uint8)((mask >> 16) & 0x03);
    }
    
    MCP2515_WriteRegister(regAddr, sidh);
    MCP2515_WriteRegister(regAddr + 1, sidl);
    
    if (idType == MCP2515_FRAME_EXT)
    {
        MCP2515_WriteRegister(regAddr + 2, (uint8)(mask >> 8));
        MCP2515_WriteRegister(regAddr + 3, (uint8)mask);
    }
    
    /* Return to normal mode */
    return MCP2515_SetMode(MCP2515_CANCTRL_REQOP_NORMAL);
}

/*
 * Description: Find free TX buffer
 */
static uint8 MCP2515_FindFreeTxBuffer(void)
{
    uint8 ctrl;
    
    /* Check TXB0 */
    ctrl = MCP2515_ReadRegister(MCP2515_REG_TXB0CTRL);
    if ((ctrl & MCP2515_TXB_TXREQ) == 0)
    {
        return 0;
    }
    
    /* Check TXB1 */
    ctrl = MCP2515_ReadRegister(MCP2515_REG_TXB1CTRL);
    if ((ctrl & MCP2515_TXB_TXREQ) == 0)
    {
        return 1;
    }
    
    /* Check TXB2 */
    ctrl = MCP2515_ReadRegister(MCP2515_REG_TXB2CTRL);
    if ((ctrl & MCP2515_TXB_TXREQ) == 0)
    {
        return 2;
    }
    
    return 0xFF;  /* No free buffer */
}

/*
 * Description:  Transmit CAN message
 */
uint8 MCP2515_Transmit(const MCP2515_Message *msg)
{
    uint8 txBuf;
    uint8 txData[13];
    uint8 loadCmd;
    uint8 rtsCmd;
    uint8 i;
    
    /* Find free TX buffer */
    txBuf = MCP2515_FindFreeTxBuffer();
    if (txBuf == 0xFF)
    {
        return MCP2515_STATUS_TX_BUSY;
    }
    
    /* Determine load command and RTS command */
    switch (txBuf)
    {
        case 0:
            loadCmd = MCP2515_CMD_LOAD_TX0;
            rtsCmd = MCP2515_CMD_RTS_TX0;
            break;
        case 1:
            loadCmd = MCP2515_CMD_LOAD_TX1;
            rtsCmd = MCP2515_CMD_RTS_TX1;
            break;
        case 2:
            loadCmd = MCP2515_CMD_LOAD_TX2;
            rtsCmd = MCP2515_CMD_RTS_TX2;
            break;
        default:
            return MCP2515_STATUS_ERROR;
    }
    
    /* Prepare ID bytes */
    if (msg->idType == MCP2515_FRAME_STD)
    {
        /* Standard ID: 11 bits */
        txData[0] = (uint8)(msg->id >> 3);                  /* SIDH */
        txData[1] = (uint8)((msg->id & 0x07) << 5);        /* SIDL */
        txData[2] = 0x00;                                    /* EID8 */
        txData[3] = 0x00;                                    /* EID0 */
    }
    else
    {
        /* Extended ID: 29 bits */
        txData[0] = (uint8)(msg->id >> 21);                                          /* SIDH */
        txData[1] = (uint8)((msg->id >> 13) & 0xE0) | 0x08 | (uint8)((msg->id >> 16) & 0x03);  /* SIDL with EXIDE */
        txData[2] = (uint8)(msg->id >> 8);                                           /* EID8 */
        txData[3] = (uint8)(msg->id);                                                /* EID0 */
    }
    
    /* DLC */
    txData[4] = msg->dlc & 0x0F;
    
    /* Data bytes */
    for (i = 0; i < msg->dlc && i < 8; i++)
    {
        txData[5 + i] = msg->data[i];
    }
    
    /* Load TX buffer using quick command */
    SPI_CS_Assert();
    SPI_Write(loadCmd);
    for (i = 0; i < (5 + msg->dlc); i++)
    {
        SPI_Write(txData[i]);
    }
    SPI_CS_Deassert();
    
    /* Request to send */
    SPI_CS_Assert();
    SPI_Write(rtsCmd);
    SPI_CS_Deassert();
    
    return MCP2515_STATUS_OK;
}

/*
 * Description: Check if message is available
 */
uint8 MCP2515_MessageAvailable(void)
{
    uint8 intf = MCP2515_ReadRegister(MCP2515_REG_CANINTF);
    return (intf & (MCP2515_CANINTF_RX0IF | MCP2515_CANINTF_RX1IF)) ? TRUE : FALSE;
}

/*
 * Description:  Receive CAN message (non-blocking)
 */
uint8 MCP2515_Receive(MCP2515_Message *msg)
{
    uint8 intf;
    uint8 rxBuf;
    uint8 rxData[13];
    uint8 readCmd;
    uint8 i;
    
    /* Check which RX buffer has message */
    intf = MCP2515_ReadRegister(MCP2515_REG_CANINTF);
    
    if (intf & MCP2515_CANINTF_RX0IF)
    {
        rxBuf = 0;
        readCmd = MCP2515_CMD_READ_RX0;
    }
    else if (intf & MCP2515_CANINTF_RX1IF)
    {
        rxBuf = 1;
        readCmd = MCP2515_CMD_READ_RX1;
    }
    else
    {
        return MCP2515_STATUS_NO_MSG;
    }
    
    /* Read RX buffer using quick command */
    SPI_CS_Assert();
    SPI_Write(readCmd);
    for (i = 0; i < 13; i++)
    {
        rxData[i] = SPI_Read();
    }
    SPI_CS_Deassert();
    
    /* Parse ID */
    if (rxData[1] & 0x08)
    {
        /* Extended ID */
        msg->idType = MCP2515_FRAME_EXT;
        msg->id = ((uint32)rxData[0] << 21) |
                  ((uint32)(rxData[1] & 0xE0) << 13) |
                  ((uint32)(rxData[1] & 0x03) << 16) |
                  ((uint32)rxData[2] << 8) |
                  (uint32)rxData[3];
    }
    else
    {
        /* Standard ID */
        msg->idType = MCP2515_FRAME_STD;
        msg->id = ((uint32)rxData[0] << 3) | ((uint32)(rxData[1] >> 5));
    }
    
    /* Parse DLC */
    msg->dlc = rxData[4] & 0x0F;
    if (msg->dlc > 8)
    {
        msg->dlc = 8;
    }
    
    /* Copy data */
    for (i = 0; i < msg->dlc; i++)
    {
        msg->data[i] = rxData[5 + i];
    }
    
    /* Clear interrupt flag */
    if (rxBuf == 0)
    {
        MCP2515_BitModify(MCP2515_REG_CANINTF, MCP2515_CANINTF_RX0IF, 0x00);
    }
    else
    {
        MCP2515_BitModify(MCP2515_REG_CANINTF, MCP2515_CANINTF_RX1IF, 0x00);
    }
    
    return MCP2515_STATUS_OK;
}

/*
 * Description:  Receive CAN message with timeout
 */
uint8 MCP2515_ReceiveWithTimeout(MCP2515_Message *msg, uint32 timeout_ms)
{
    while (timeout_ms > 0)
    {
        if (MCP2515_Receive(msg) == MCP2515_STATUS_OK)
        {
            return MCP2515_STATUS_OK;
        }
        Delay_MS(1);
        timeout_ms--;
    }
    
    return MCP2515_STATUS_TIMEOUT;
}
