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

#define DEBOUNCE_DELAY 13
#define CLUTCH_ENGAGE_DELAY_UP 70
#define CLUTCH_ENGAGE_DELAY_DOWN 150
#define CLUTCH_RELEASE_DELAY 20
#define SHIFT_ENGAGE_DELAY 165
#define NEUTRAL_FIND 50

#define UP_SHIFT_PADDLE 0x2   // 01
#define DOWN_SHIFT_PADDLE 0x1 // 10
#define NEUTRAL_BUTTON 0x7

int paddle = 0x3;
int shifts = 0;

void Shift(int paddle);

int main()
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    CyGlobalIntEnable; //*/ /* Uncomment this line to enable global interrupts. */
    for(;;)
    {
        /* Place your application code here. */
        paddle = Steering_Wheel_Read();
        
        if (paddle != 0x3) {
            CyDelay(DEBOUNCE_DELAY);
            if (paddle == Steering_Wheel_Read()) Shift(paddle);
            while (paddle == Steering_Wheel_Read());
        }
        
        if (Clutch_Paddle_Read() == 0x0) {
            CyDelay(DEBOUNCE_DELAY);
            Shift(NEUTRAL_BUTTON);
            while (Clutch_Paddle_Read() == 0x0);
        }
        
        CyDelayUs(200);
    }
}

void Shift(int paddle) {
    shifts++;
    if (paddle == NEUTRAL_BUTTON) {
        Up_Shift_Air_Write(1);
        CyDelay(NEUTRAL_FIND);
        Up_Shift_Air_Write(0);
        return;
    }
    Engage_Clutch_Write(1);
    if (paddle == UP_SHIFT_PADDLE) {
        CyDelay(CLUTCH_ENGAGE_DELAY_UP);
        Up_Shift_Air_Write(1);
        CyDelay(SHIFT_ENGAGE_DELAY);
        Up_Shift_Air_Write(0);
        CyDelay(CLUTCH_RELEASE_DELAY);
        Engage_Clutch_Write(0);
        return;
    }
    if (paddle == DOWN_SHIFT_PADDLE) {
        CyDelay(CLUTCH_ENGAGE_DELAY_DOWN);
        Down_Shift_Air_Write(1);
        CyDelay(SHIFT_ENGAGE_DELAY);
        Down_Shift_Air_Write(0);
        CyDelay(CLUTCH_RELEASE_DELAY);
        Engage_Clutch_Write(0);
        return;
    }
}

/* [] END OF FILE */
