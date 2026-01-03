/******************************************************************************
 *
 * Module: CAN - Controller Area Network Driver
 *
 * File Name: can.c
 *
 * Description:  Source file for TM4C123GH6PM CAN Driver
 *              Uses CAN0 module with PE4 (CAN0Rx) and PE5 (CAN0Tx)
 *              For use with TJA1050 CAN Transceiver
 *
 * Author: BodzTH
 *
 *******************************************************************************/

#include "can.h"
#include "tm4c123gh6pm_registers.h"
#include "delay.h"

/*******************************************************************************
 *                          Private Function Prototypes                         *
 *******************************************************************************/

static void CAN_SetBitTiming(uint32 baudRate);
static void CAN_WaitForIF1Ready(void);
static void CAN_WaitForIF2Ready(void);

/*******************************************************************************
 *                          Function Definitions                                *
 *******************************************************************************/

/*
 * Description: Initialize CAN0 module
 */
uint8 CAN_Init(const CAN_ConfigType *config)
{
    if (config == NULL_PTR)
    {
        return CAN_STATUS_ERROR;
    }
    
    /* 1. Enable CAN0 module clock */
    SYSCTL_RCGCCAN_REG |= 0x01;  /* Enable CAN0 clock */
    
    /* 2. Enable GPIO Port E clock */
    SYSCTL_RCGCGPIO_REG |= 0x10;  /* Enable PORTE clock */
    
    /* Small delay for clock stabilization */
    Delay_MS(1);
    
    /* 3. Configure PE4 and PE5 for CAN0 */
    /* Unlock PORTE (usually not needed for PE4/PE5) */
    GPIO_PORTE_LOCK_REG = 0x4C4F434B;
    GPIO_PORTE_CR_REG |= 0x30;  /* Allow changes to PE4, PE5 */
    
    /* Disable analog function */
    GPIO_PORTE_AMSEL_REG &= ~0x30;
    
    /* Enable alternate function for PE4 and PE5 */
    GPIO_PORTE_AFSEL_REG |= 0x30;
    
    /* Configure PE4 as CAN0Rx and PE5 as CAN0Tx (PCTL = 8) */
    GPIO_PORTE_PCTL_REG &= ~0x00FF0000;
    GPIO_PORTE_PCTL_REG |= 0x00880000;
    
    /* Enable digital I/O */
    GPIO_PORTE_DEN_REG |= 0x30;
    
    /* 4. Initialize CAN0 controller */
    /* Enter initialization mode */
    CAN0_CTL_REG = CAN_CTL_INIT;
    
    /* Enable configuration change */
    CAN0_CTL_REG |= CAN_CTL_CCE;
    
    /* 5. Set bit timing for desired baud rate */
    CAN_SetBitTiming(config->baudRate);
    
    /* 6. Configure test mode if loopback is enabled */
    if (config->loopbackMode)
    {
        CAN0_CTL_REG |= CAN_CTL_TEST;
        CAN0_TST_REG = CAN_TST_LBACK;
    }
    
    /* 7. Exit initialization mode */
    CAN0_CTL_REG &= ~(CAN_CTL_INIT | CAN_CTL_CCE);
    
    /* Wait for CAN module to exit init mode */
    Delay_MS(1);
    
    return CAN_STATUS_OK;
}

/*
 * Description: Set CAN bit timing for desired baud rate
 */
static void CAN_SetBitTiming(uint32 baudRate)
{
    uint32 bitTime;
    uint32 brp;
    uint32 tseg1, tseg2, sjw;
    
    /*
     * Bit timing calculation for 500 kbps with 16 MHz clock:
     * - Nominal bit time = 1 / 500000 = 2 us
     * - Time quanta (Tq) per bit = 8 (typical:  8-25)
     * - Tq = 2us / 8 = 250 ns
     * - BRP = (Tq * SYSCLK) - 1 = (250ns * 16MHz) - 1 = 4 - 1 = 3
     * - Tseg1 = 5 Tq (Prop + Phase1)
     * - Tseg2 = 2 Tq (Phase2)
     * - SJW = 1 Tq
     */
    
    if (baudRate == CAN_BAUD_RATE_500KBPS)
    {
        brp = 3;      /* Baud rate prescaler - 1 */
        tseg1 = 4;    /* Time segment 1 - 1 (5 Tq) */
        tseg2 = 1;    /* Time segment 2 - 1 (2 Tq) */
        sjw = 0;      /* Sync jump width - 1 (1 Tq) */
    }
    else if (baudRate == CAN_BAUD_RATE_250KBPS)
    {
        brp = 7;      /* Baud rate prescaler - 1 */
        tseg1 = 4;    /* Time segment 1 - 1 */
        tseg2 = 1;    /* Time segment 2 - 1 */
        sjw = 0;      /* Sync jump width - 1 */
    }
    else
    {
        /* Default to 500 kbps */
        brp = 3;
        tseg1 = 4;
        tseg2 = 1;
        sjw = 0;
    }
    
    /* Configure CAN bit timing register */
    bitTime = (tseg2 << 12) | (tseg1 << 8) | (sjw << 6) | brp;
    CAN0_BIT_REG = bitTime;
    
    /* Extended prescaler (usually 0 for standard rates) */
    CAN0_BRPE_REG = 0;
}

/*
 * Description: Wait for Interface 1 to be ready
 */
static void CAN_WaitForIF1Ready(void)
{
    while (CAN0_IF1CRQ_REG & 0x8000)
    {
        /* Wait for BUSY bit to clear */
    }
}

/*
 * Description: Wait for Interface 2 to be ready
 */
static void CAN_WaitForIF2Ready(void)
{
    while (CAN0_IF2CRQ_REG & 0x8000)
    {
        /* Wait for BUSY bit to clear */
    }
}

/*
 * Description: Transmit a CAN message
 */
uint8 CAN_Transmit(const CAN_Message *msg)
{
    uint32 arb1, arb2, mctl;
    
    if (msg == NULL_PTR || msg->dataLength > CAN_MAX_DATA_LENGTH)
    {
        return CAN_STATUS_ERROR;
    }
    
    /* Wait for interface to be ready */
    CAN_WaitForIF1Ready();
    
    /* Set command mask:  write arbitration, control, and data */
    CAN0_IF1CMSK_REG = CAN_IFCMSK_WRNRD | CAN_IFCMSK_ARB | CAN_IFCMSK_CONTROL |
                       CAN_IFCMSK_DATAA | CAN_IFCMSK_DATAB;
    
    /* Configure arbitration registers */
    if (msg->msgIDType == CAN_FRAME_STD)
    {
        /* Standard 11-bit ID */
        arb1 = 0;
        arb2 = ((msg->msgID & 0x7FF) << 2) | CAN_IFARB2_MSGVAL | CAN_IFARB2_DIR;
    }
    else
    {
        /* Extended 29-bit ID */
        arb1 = msg->msgID & 0xFFFF;
        arb2 = ((msg->msgID >> 16) & 0x1FFF) | CAN_IFARB2_MSGVAL | 
               CAN_IFARB2_XTD | CAN_IFARB2_DIR;
    }
    
    CAN0_IF1ARB1_REG = arb1;
    CAN0_IF1ARB2_REG = arb2;
    
    /* Configure message control:  set DLC, EOB, and transmit request */
    mctl = (msg->dataLength & CAN_IFMCTL_DLC_MASK) | CAN_IFMCTL_EOB | CAN_IFMCTL_TXRQST;
    CAN0_IF1MCTL_REG = mctl;
    
    /* Load data bytes */
    CAN0_IF1DA1_REG = (msg->data[1] << 8) | msg->data[0];
    CAN0_IF1DA2_REG = (msg->data[3] << 8) | msg->data[2];
    CAN0_IF1DB1_REG = (msg->data[5] << 8) | msg->data[4];
    CAN0_IF1DB2_REG = (msg->data[7] << 8) | msg->data[6];
    
    /* Start transfer to message object */
    CAN0_IF1CRQ_REG = CAN_MSG_OBJ_TX;
    
    /* Wait for transfer to complete */
    CAN_WaitForIF1Ready();
    
    return CAN_STATUS_OK;
}

/*
 * Description: Configure a message object for reception with filter
 */
uint8 CAN_ConfigureRxFilter(uint8 msgObjNum, uint32 msgID, uint32 msgMask)
{
    uint32 arb2, msk2;
    
    if (msgObjNum < 1 || msgObjNum > 32)
    {
        return CAN_STATUS_ERROR;
    }
    
    /* Wait for interface to be ready */
    CAN_WaitForIF2Ready();
    
    /* Set command mask: write arbitration, mask, and control */
    CAN0_IF2CMSK_REG = CAN_IFCMSK_WRNRD | CAN_IFCMSK_ARB | CAN_IFCMSK_CONTROL | CAN_IFCMSK_MASK;
    
    /* Configure mask for standard ID filtering */
    CAN0_IF2MSK1_REG = 0;
    msk2 = ((msgMask & 0x7FF) << 2) | CAN_IFMSK2_MDIR;
    CAN0_IF2MSK2_REG = msk2;
    
    /* Configure arbitration for receive (DIR = 0) */
    CAN0_IF2ARB1_REG = 0;
    arb2 = ((msgID & 0x7FF) << 2) | CAN_IFARB2_MSGVAL;  /* DIR = 0 for receive */
    CAN0_IF2ARB2_REG = arb2;
    
    /* Configure message control: enable receive interrupt, use mask */
    CAN0_IF2MCTL_REG = CAN_IFMCTL_EOB | CAN_IFMCTL_UMASK | (8 & CAN_IFMCTL_DLC_MASK);
    
    /* Start transfer to message object */
    CAN0_IF2CRQ_REG = msgObjNum;
    
    /* Wait for transfer to complete */
    CAN_WaitForIF2Ready();
    
    return CAN_STATUS_OK;
}

/*
 * Description:  Receive a CAN message (blocking with timeout)
 */
uint8 CAN_Receive(CAN_Message *msg, uint32 timeout_ms)
{
    uint32 timeout_count = 0;
    uint32 arb2;
    
    if (msg == NULL_PTR)
    {
        return CAN_STATUS_ERROR;
    }
    
    /* Wait for new data with timeout */
    while (! CAN_IsMessageAvailable(CAN_MSG_OBJ_RX))
    {
        Delay_MS(1);
        timeout_count++;
        if (timeout_count >= timeout_ms)
        {
            return CAN_STATUS_TIMEOUT;
        }
    }
    
    /* Wait for interface to be ready */
    CAN_WaitForIF2Ready();
    
    /* Set command mask: read arbitration, control, data, and clear interrupt */
    CAN0_IF2CMSK_REG = CAN_IFCMSK_ARB | CAN_IFCMSK_CONTROL | 
                       CAN_IFCMSK_DATAA | CAN_IFCMSK_DATAB | CAN_IFCMSK_CLRINTPND;
    
    /* Request read from message object */
    CAN0_IF2CRQ_REG = CAN_MSG_OBJ_RX;
    
    /* Wait for transfer to complete */
    CAN_WaitForIF2Ready();
    
    /* Extract arbitration info */
    arb2 = CAN0_IF2ARB2_REG;
    
    if (arb2 & CAN_IFARB2_XTD)
    {
        /* Extended ID */
        msg->msgIDType = CAN_FRAME_EXT;
        msg->msgID = ((arb2 & 0x1FFF) << 16) | CAN0_IF2ARB1_REG;
    }
    else
    {
        /* Standard ID */
        msg->msgIDType = CAN_FRAME_STD;
        msg->msgID = (arb2 >> 2) & 0x7FF;
    }
    
    /* Get data length */
    msg->dataLength = CAN0_IF2MCTL_REG & CAN_IFMCTL_DLC_MASK;
    
    /* Extract data bytes */
    msg->data[0] = CAN0_IF2DA1_REG & 0xFF;
    msg->data[1] = (CAN0_IF2DA1_REG >> 8) & 0xFF;
    msg->data[2] = CAN0_IF2DA2_REG & 0xFF;
    msg->data[3] = (CAN0_IF2DA2_REG >> 8) & 0xFF;
    msg->data[4] = CAN0_IF2DB1_REG & 0xFF;
    msg->data[5] = (CAN0_IF2DB1_REG >> 8) & 0xFF;
    msg->data[6] = CAN0_IF2DB2_REG & 0xFF;
    msg->data[7] = (CAN0_IF2DB2_REG >> 8) & 0xFF;
    
    return CAN_STATUS_OK;
}

/*
 * Description:  Check if a message is available for reception
 */
boolean CAN_IsMessageAvailable(uint8 msgObjNum)
{
    uint32 newDataReg;
    
    if (msgObjNum < 1 || msgObjNum > 32)
    {
        return FALSE;
    }
    
    if (msgObjNum <= 16)
    {
        newDataReg = CAN0_NWDA1_REG;
        return ((newDataReg & (1 << (msgObjNum - 1))) != 0) ? TRUE : FALSE;
    }
    else
    {
        newDataReg = CAN0_NWDA2_REG;
        return ((newDataReg & (1 << (msgObjNum - 17))) != 0) ? TRUE : FALSE;
    }
}

/*
 * Description: Get CAN error status
 */
uint32 CAN_GetErrorStatus(void)
{
    return CAN0_STS_REG;
}

/*
 * Description: Reset CAN module
 */
void CAN_Reset(void)
{
    /* Enter initialization mode */
    CAN0_CTL_REG |= CAN_CTL_INIT;
    
    /* Small delay */
    Delay_MS(1);
    
    /* Exit initialization mode */
    CAN0_CTL_REG &= ~CAN_CTL_INIT;
}
