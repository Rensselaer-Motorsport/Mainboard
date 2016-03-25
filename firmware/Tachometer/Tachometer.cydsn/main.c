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

uint8 compare_occured;

#define PWM_FREQ 100
#define NO_OF_MSEC 1000

int main()
{
    uint32 input_freq = 0;
    static uint16 PWM_windowPeriod = 0;
    static uint32 counter_countVal;
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    ISR_Compare_Start();
    PWM_Window_Start();
    Counter_Start();
    Clock_PWM_Start();

    PWM_windowPeriod = (PWM_Window_ReadPeriod() - PWM_Window_ReadCompare());
    PWM_windowPeriod = PWM_windowPeriod/ PWM_FREQ;

    CyGlobalIntEnable; /* Enable global interrupts. */
    
    for(;;)
    {
        /* Place your application code here. */
        if (compare_occured == 1)
        {
            counter_countVal = Counter_ReadCapture();
            input_freq = ((uint32)(NO_OF_MSEC * (uint32)counter_countVal) / (uint32)PWM_windowPeriod);
            compare_occured = 0;
        }
    }
}

/* [] END OF FILE */
