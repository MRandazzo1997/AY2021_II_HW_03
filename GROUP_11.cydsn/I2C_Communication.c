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
    ADC_Start();
    ADC_StartConvert();
   
}

void stop()
{
    ADC_Stop();
    Pin_LED_Write(LED_OFF);
    
}


void SetBuffer(int16 temp, int16 light){  //to be implemented how to write on 0 and 1
    
    SlaveBuffer[0] = ctrlReg0;
    SlaveBuffer[1] = ctrlReg1;
    SlaveBuffer[2] = WHO_AM_I;
    /*Convert int16 to char*/
    if(light != 0)
    {
        SlaveBuffer[4] = light & 0xFF;
        SlaveBuffer[3] = (light >> 8) & 0xFF;
    }
    else
    {
        SlaveBuffer[3] = 0;
        SlaveBuffer[4] = 0;
    }
    if(temp != 0)
    {
        SlaveBuffer[6] = temp & 0xFF;
        SlaveBuffer[5] = (temp >> 8) & 0xFF;
    }
    else
    {
        SlaveBuffer[5] = 0;
        SlaveBuffer[6] = 0;
    }
    
}










/* [] END OF FILE */
