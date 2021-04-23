/*
*   \ InterruptRoutines.c file
*   \ Authors: Riccardo Mustoni, Alessia Pesenti and Mattia Randazzo
*   \ Date: 04/27/2021
*   \ Assignment 03 - Analog sampling and I2C communication
*   \ Group 11
*/


/*
*
* Source file for interrupt routines
*
*/

#include "InterruptRoutines.h"
#include "I2C_Communication.h"
#include "project.h"


extern volatile int flag, numSamp;
uint16_t count = 0;
float X;


CY_ISR(Custom_ISR_ADC)
{
    Timer_ReadStatusRegister();
    // per_isr*numSamp * X = 20 ms
    if(count == 0)
    {
        X = TIMER_CK/(TRANSMISSION_RATE*Timer_ReadPeriod()*numSamp);
        count++;
    }
    
    if(count >= X)
    {
        flag = 1;              
        count = 0; 
    }
}

/* [] END OF FILE */
