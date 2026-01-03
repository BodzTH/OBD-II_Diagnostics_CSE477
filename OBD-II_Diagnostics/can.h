/******************************************************************************
 *
 * Module: CAN - Controller Area Network Driver
 *
 * File Name: can.h
 *
 * Description: Header file for TM4C123GH6PM CAN Driver
 *              Uses CAN0 module with PE4 (CAN0Rx) and PE5 (CAN0Tx)
 *              For use with TJA1050 CAN Transceiver
 *
 * Author: BodzTH
 *
 *******************************************************************************/

#ifndef CAN_H_
#define CAN_H_

#include "std_types.h"

/*******************************************************************************
 *                          CAN0 Register Definitions                           *
 *******************************************************************************/

/* CAN0 Base Address:  0x40040000 */
#define CAN0_CTL_REG              (*((volatile uint32 *)0x40040000))
#define CAN0_STS_REG              (*((volatile uint32 *)0x40040004))
#define CAN0_ERR_REG              (*((volatile uint32 *)0x40040008))
#define CAN0_BIT_REG              (*((volatile uint32 *)0x4004000C))
#define CAN0_INT_REG              (*((volatile uint32 *)0x40040010))
#define CAN0_TST_REG              (*((volatile uint32 *)0x40040014))
#define CAN0_BRPE_REG             (*((volatile uint32 *)0x40040018))

/* CAN0 Interface 1 Registers (for transmit) */
#define CAN0_IF1CRQ_REG           (*((volatile uint32 *)0x40040020))
#define CAN0_IF1CMSK_REG          (*((volatile uint32 *)0x40040024))
#define CAN0_IF1MSK1_REG          (*((volatile uint32 *)0x40040028))
#define CAN0_IF1MSK2_REG          (*((volatile uint32 *)0x4004002C))
#define CAN0_IF1ARB1_REG          (*((volatile uint32 *)0x40040030))
#define CAN0_IF1ARB2_REG          (*((volatile uint32 *)0x40040034))
#define CAN0_IF1MCTL_REG          (*((volatile uint32 *)0x40040038))
#define CAN0_IF1DA1_REG           (*((volatile uint32 *)0x4004003C))
#define CAN0_IF1DA2_REG           (*((volatile uint32 *)0x40040040))
#define CAN0_IF1DB1_REG           (*((volatile uint32 *)0x40040044))
#define CAN0_IF1DB2_REG           (*((volatile uint32 *)0x40040048))

/* CAN0 Interface 2 Registers (for receive) */
#define CAN0_IF2CRQ_REG           (*((volatile uint32 *)0x40040080))
#define CAN0_IF2CMSK_REG          (*((volatile uint32 *)0x40040084))
#define CAN0_IF2MSK1_REG          (*((volatile uint32 *)0x40040088))
#define CAN0_IF2MSK2_REG          (*((volatile uint32 *)0x4004008C))
#define CAN0_IF2ARB1_REG          (*((volatile uint32 *)0x40040090))
#define CAN0_IF2ARB2_REG          (*((volatile uint32 *)0x40040094))
#define CAN0_IF2MCTL_REG          (*((volatile uint32 *)0x40040098))
#define CAN0_IF2DA1_REG           (*((volatile uint32 *)0x4004009C))
#define CAN0_IF2DA2_REG           (*((volatile uint32 *)0x400400A0))
#define CAN0_IF2DB1_REG           (*((volatile uint32 *)0x400400A4))
#define CAN0_IF2DB2_REG           (*((volatile uint32 *)0x400400A8))

/* CAN0 Message Status Registers */
#define CAN0_TXRQ1_REG            (*((volatile uint32 *)0x40040100))
#define CAN0_TXRQ2_REG            (*((volatile uint32 *)0x40040104))
#define CAN0_NWDA1_REG            (*((volatile uint32 *)0x40040120))
#define CAN0_NWDA2_REG            (*((volatile uint32 *)0x40040124))
#define CAN0_MSG1INT_REG          (*((volatile uint32 *)0x40040140))
#define CAN0_MSG2INT_REG          (*((volatile uint32 *)0x40040144))
#define CAN0_MSG1VAL_REG          (*((volatile uint32 *)0x40040160))
#define CAN0_MSG2VAL_REG          (*((volatile uint32 *)0x40040164))

/*******************************************************************************
 *                          CAN Register Bit Definitions                        *
 *******************************************************************************/

/* CAN Control Register bits */
#define CAN_CTL_INIT              0x00000001  /* Initialization mode */
#define CAN_CTL_IE                0x00000002  /* Interrupt enable */
#define CAN_CTL_SIE               0x00000004  /* Status interrupt enable */
#define CAN_CTL_EIE               0x00000008  /* Error interrupt enable */
#define CAN_CTL_DAR               0x00000020  /* Disable automatic retransmission */
#define CAN_CTL_CCE               0x00000040  /* Configuration change enable */
#define CAN_CTL_TEST              0x00000080  /* Test mode enable */

/* CAN Status Register bits */
#define CAN_STS_LEC_MASK          0x00000007  /* Last error code */
#define CAN_STS_TXOK              0x00000008  /* Transmit OK */
#define CAN_STS_RXOK              0x00000010  /* Receive OK */
#define CAN_STS_EPASS             0x00000020  /* Error passive */
#define CAN_STS_EWARN             0x00000040  /* Error warning */
#define CAN_STS_BOFF              0x00000080  /* Bus off */

/* CAN Test Register bits */
#define CAN_TST_LBACK             0x00000010  /* Loopback mode */
#define CAN_TST_SILENT            0x00000008  /* Silent mode */

/* CAN IF Command Mask Register bits */
#define CAN_IFCMSK_DATAB          0x00000001  /* Data B access */
#define CAN_IFCMSK_DATAA          0x00000002  /* Data A access */
#define CAN_IFCMSK_TXRQST         0x00000004  /* Transmit request */
#define CAN_IFCMSK_CLRINTPND      0x00000008  /* Clear interrupt pending */
#define CAN_IFCMSK_CONTROL        0x00000010  /* Control access */
#define CAN_IFCMSK_ARB            0x00000020  /* Arbitration access */
#define CAN_IFCMSK_MASK           0x00000040  /* Mask access */
#define CAN_IFCMSK_WRNRD          0x00000080  /* Write/Read (1=Write) */

/* CAN IF Arbitration 2 Register bits */
#define CAN_IFARB2_MSGVAL         0x00008000  /* Message valid */
#define CAN_IFARB2_XTD            0x00004000  /* Extended identifier */
#define CAN_IFARB2_DIR            0x00002000  /* Direction (1=Transmit) */

/* CAN IF Message Control Register bits */
#define CAN_IFMCTL_DLC_MASK       0x0000000F  /* Data length code */
#define CAN_IFMCTL_EOB            0x00000080  /* End of buffer */
#define CAN_IFMCTL_TXRQST         0x00000100  /* Transmit request */
#define CAN_IFMCTL_RMTEN          0x00000200  /* Remote enable */
#define CAN_IFMCTL_RXIE           0x00000400  /* Receive interrupt enable */
#define CAN_IFMCTL_TXIE           0x00000800  /* Transmit interrupt enable */
#define CAN_IFMCTL_UMASK          0x00001000  /* Use acceptance mask */
#define CAN_IFMCTL_INTPND         0x00002000  /* Interrupt pending */
#define CAN_IFMCTL_MSGLST         0x00004000  /* Message lost */
#define CAN_IFMCTL_NEWDAT         0x00008000  /* New data */

/* CAN IF Mask 2 Register bits */
#define CAN_IFMSK2_MXTD           0x00008000  /* Mask extended identifier */
#define CAN_IFMSK2_MDIR           0x00004000  /* Mask direction */

/*******************************************************************************
 *                                Definitions                                   *
 *******************************************************************************/

/* CAN Baud Rate - Standard OBD-II uses 500 kbps */
#define CAN_BAUD_RATE_500KBPS       500000
#define CAN_BAUD_RATE_250KBPS       250000

/* CAN Message Object Numbers (1-32 available) */
#define CAN_MSG_OBJ_TX              1
#define CAN_MSG_OBJ_RX              2

/* CAN Frame Types */
#define CAN_FRAME_STD               0   /* Standard 11-bit ID */
#define CAN_FRAME_EXT               1   /* Extended 29-bit ID */

/* CAN Message Flags */
#define CAN_MSG_TX_PENDING          0x01
#define CAN_MSG_RX_PENDING          0x02
#define CAN_MSG_TX_COMPLETE         0x04
#define CAN_MSG_RX_COMPLETE         0x08
#define CAN_MSG_ERROR               0x10

/* CAN Status */
#define CAN_STATUS_OK               0
#define CAN_STATUS_ERROR            1
#define CAN_STATUS_BUSY             2
#define CAN_STATUS_TIMEOUT          3

/* Maximum CAN Data Length */
#define CAN_MAX_DATA_LENGTH         8

/* System Clock frequency (assuming 16 MHz) */
#define CAN_SYSTEM_CLOCK            16000000

/*******************************************************************************
 *                              Type Definitions                                *
 *******************************************************************************/

/* CAN Message Structure */
typedef struct {
    uint32 msgID;                       /* Message identifier (11-bit or 29-bit) */
    uint8  msgIDType;                   /* Standard (0) or Extended (1) ID */
    uint8  dataLength;                  /* Number of data bytes (0-8) */
    uint8  data[CAN_MAX_DATA_LENGTH];   /* Message data bytes */
} CAN_Message;

/* CAN Configuration Structure */
typedef struct {
    uint32 baudRate;                    /* CAN bus baud rate */
    uint8  loopbackMode;                /* Enable loopback for testing */
} CAN_ConfigType;

/*******************************************************************************
 *                              Function Prototypes                             *
 *******************************************************************************/

/*
 * Description: Initialize CAN0 module
 * Parameters:   Pointer to configuration structure
 * Returns:     CAN_STATUS_OK on success, CAN_STATUS_ERROR on failure
 */
uint8 CAN_Init(const CAN_ConfigType *config);

/*
 * Description:  Transmit a CAN message
 * Parameters:  Pointer to CAN message structure
 * Returns:      CAN_STATUS_OK on success, CAN_STATUS_ERROR on failure
 */
uint8 CAN_Transmit(const CAN_Message *msg);

/*
 * Description: Receive a CAN message (blocking with timeout)
 * Parameters:  Pointer to CAN message structure to store received data
 *              Timeout in milliseconds
 * Returns:     CAN_STATUS_OK on success, CAN_STATUS_TIMEOUT on timeout
 */
uint8 CAN_Receive(CAN_Message *msg, uint32 timeout_ms);

/*
 * Description: Configure a message object for reception with filter
 * Parameters:  Message object number (1-32)
 *              Message ID to filter
 *              Message ID mask (1 = must match, 0 = don't care)
 * Returns:     CAN_STATUS_OK on success
 */
uint8 CAN_ConfigureRxFilter(uint8 msgObjNum, uint32 msgID, uint32 msgMask);

/*
 * Description: Check if a message is available for reception
 * Parameters:  Message object number
 * Returns:     TRUE if message available, FALSE otherwise
 */
boolean CAN_IsMessageAvailable(uint8 msgObjNum);

/*
 * Description: Get CAN error status
 * Returns:     Error flags from CAN status register
 */
uint32 CAN_GetErrorStatus(void);

/*
 * Description:  Reset CAN module
 */
void CAN_Reset(void);

#endif /* CAN_H_ */
