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

#define PWM_FREQ 100
#define NO_OF_MSEC 1000

#define RED_LINE 12000

extern uint8 compare_occured;

int paddle = 0x3;
int shifts = 0;
uint8 gear = 0;

void Shift(int paddle);

int main()
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    uint32 tach_freq = 0;
    static uint16 PWM_windowPeriod = 0;
    static uint32 counter_countVal;
    
    ISR_Compare_Start();
    PWM_Window_Start();
    Counter_Start();
    Clock_PWM_Start();
    
    PWM_windowPeriod = (PWM_Window_ReadPeriod() - PWM_Window_ReadCompare());
    PWM_windowPeriod = PWM_windowPeriod/ PWM_FREQ;
    
    CyGlobalIntEnable; //*/ /* Uncomment this line to enable global interrupts. */
    for(;;)
    {
        /* Place your application code here. */
        while (!Accel_Read()) {
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
        }
        while (Accel_Read()) {
            
            if (compare_occured == 1 && !Clutch_Paddle_Read()) {
                counter_countVal = Counter_ReadCapture();
                tach_freq = ((uint32)(NO_OF_MSEC * (uint32)counter_countVal) / (uint32)PWM_windowPeriod);
                
                if (tach_freq >= RED_LINE)
                    Shift(UP_SHIFT_PADDLE);
                compare_occured = 0;
            }
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
