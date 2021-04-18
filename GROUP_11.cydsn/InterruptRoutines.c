/*
*   \ InterruptRoutines.c file
*   \ Authors: Riccardo Mustoni, Alessia Pesenti and Mattia Randazzo
*   \ Date: 04/27/2021
*   \ Assignment 03 - Analog sampling and I2C communication
*   \ Group 11
*/

#include "InterruptRoutines.h"
#include "I2C_Communication.h"
#include "I2C_Communication.c"
#include "project.h"


extern volatile int flag;

CY_ISR(Custom_ISR_ADC)
{
    Timer_ReadStatusRegister();
    
    /* Everytime the timer ISR occurs (4 ms), the flag is put to 1
       while the ADC reading is computed in the main.c file
    */
    flag = 1;
}

//switch betwen status and turn on-off LED
void EZI2C_ISR_ExitCallback(void){
    
    define_status = (SlaveBuffer[0] & 0b00000011);         
    
    switch(define_status) {
        
    case DEVICE_STOPPED: {stop();        
                         break;}
    
    case CHANN_TEMP:    {Pin_LED_Write(LED_OFF);
                         start();        
                         break;}
    
    case CHANN_LDR:     {Pin_LED_Write(LED_OFF);
                         start();        
                         break;}
    
    case CHANN_BOTH:     {Pin_LED_Write(LED_ON);
                         start();
                         break;}
    
    default:            {Pin_LED_Write(LED_OFF);
                         break;}
     
    }
}

/* [] END OF FILE */
