/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>

#define CAN_RX_MAILBOX_0_SHIFT  (1u)

CY_ISR_PROTO(ISR_CAN);

uint8 buffer[8];
uint8 length = 1;
volatile uint8 receiveMailboxNumber = 0xFFu;

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    CAN_Start();
    
    /* Start USBFS Operation with 5V operation */
    USBFS_Start(0u, USBFS_5V_OPERATION);
    
    /* Wait for Device to enumerate */
    while(!USBFS_bGetConfiguration());

    for(;;)
    {
        /* Check for IN buffer is empty */
        if (receiveMailboxNumber == CAN_RX_MAILBOX_RX_message) {
            while(USBFS_bGetEPState(1) != USBFS_IN_BUFFER_EMPTY);
            
            buffer[0] = CAN_RX_DATA_BYTE1(CAN_RX_MAILBOX_RX_message);
        
            /* Load the IN buffer */
            USBFS_LoadInEP(1, &buffer[0], length);
        }
    }
}

CY_ISR(ISR_CAN)
{
    /* Clear receive message flag */
    CAN_INT_SR_REG.byte[1u] = CAN_RX_MESSAGE_MASK;
    
    /* Switch Status message received */
    if ((CY_GET_REG16((reg16 *) &CAN_BUF_SR_REG.byte[0u]) & CAN_RX_MAILBOX_0_SHIFT) != 0u)
    {
        receiveMailboxNumber = CAN_RX_MAILBOX_RX_message;
        
        /* Acknowledge receipt of new message */
        CAN_RX_ACK_MESSAGE(CAN_RX_MAILBOX_RX_message);
    }
}
/* [] END OF FILE */
