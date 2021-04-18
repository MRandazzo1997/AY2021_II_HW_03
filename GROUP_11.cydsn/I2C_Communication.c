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


#include "project.h"
#include "I2C_Communication.h"



void start()
{
    Timer_Start(); //(?)
    ADC_Start();
    EZI2C_Start();
     
}

void stop()
{
    Timer_Stop(); //(?)
    ADC_Stop();
    Pin_LED_Write(LED_OFF);
    
}


void SetBuffer(){  //to be implemented how to write on 0 and 1
    
    SlaveBuffer[0] 
    SlaveBuffer[1] 
    SlaveBuffer[2] = WHO_AM_I;
    
}










/* [] END OF FILE */
