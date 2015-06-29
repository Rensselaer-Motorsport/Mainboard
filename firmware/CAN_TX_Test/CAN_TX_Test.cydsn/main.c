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

uint8 CAN_Tx_data = 0;

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    CAN_Start();

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        /* Place your application code here. */
        
        CAN_TX_DATA_BYTE1(CAN_TX_MAILBOX_TX_message) = CAN_Tx_data;
        CAN_SendMsgTX_message();
        
        CyDelay(100);
        
        CAN_Tx_data++;
    }
}

/* [] END OF FILE */
