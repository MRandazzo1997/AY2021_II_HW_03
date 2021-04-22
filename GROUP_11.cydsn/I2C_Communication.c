/** 
*   \ I2C_Communication.c file
*   \ Authors: Riccardo Mustoni, Alessia Pesenti and Mattia Randazzo
*   \ Date: 04/27/2021
*   \ Assignment 03 - Analog sampling and I2C communication
*   \ Group 11
*/


/*
*
* Source file for I2C Communication
*
*/

    #include "project.h"
    #include "I2C_Communication.h"
    
    
    //function which will start ADC
    void start()
    {
        ADC_Start();
        ADC_StartConvert();
    }

    
    //function which will stop ADC and LED
    void stop()
    {
        ADC_Stop();
        Pin_LED_Write(LED_OFF); 
    }

    
    //function which will sample the temperature sensor, summing up the aquired data in avg_temp
    void get_temp()
    {
        temp = ADC_Read32();
        if(temp > 65535)    temp = 65535;
        if(temp < 0)        temp = 0;
        avg_temp = avg_temp + temp;
    }
    
    
    //function which will sample the photoresistor, summing up the aquired data in avg_ldr
    void get_ldr()
    {
        ldr = ADC_Read32();
        if(ldr > 65535)    ldr = 65535;
        if(ldr < 0)       ldr = 0;
        avg_ldr = avg_ldr + ldr;
    }
    
    
    //function which will set the adresses of the Slave Buffer from 3 to 6, containing the sampled data or 0 if i'm not sampling that channel
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
