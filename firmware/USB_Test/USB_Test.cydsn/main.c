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

uint8 buffer[64];
uint8 length = 13;

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    /* Start USBFS Operation with 5V operation */
    USBFS_Start(0u, USBFS_5V_OPERATION);

    /* Wait for Device to enumerate */
    while(!USBFS_bGetConfiguration());

    for(;;)
    {
        memcpy(&buffer, "Hello World!", length);
        
        /* Check for IN buffer is empty */
        while(USBFS_bGetEPState(1) != USBFS_IN_BUFFER_EMPTY);
        
        /* Load the IN buffer */
        USBFS_LoadInEP(1, &buffer[0], length);
        
        CyDelay(100);
    }
}

/* [] END OF FILE */
