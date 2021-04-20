/** 
*   \ I2C_Communication.c file
*   \ Authors: Riccardo Mustoni, Alessia Pesenti and Mattia Randazzo
*   \ Date: 04/27/2021
*   \ Assignment 03 - Analog sampling and I2C communication
*   \ Group 11
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


    void SetBuffer(int32 temp, int32 light){ 
        
        /*Convert int32 to char*/
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
