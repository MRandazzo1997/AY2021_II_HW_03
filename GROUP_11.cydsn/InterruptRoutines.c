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
#include "project.h"


extern volatile int flag;

CY_ISR(Custom_ISR_ADC)
{
    Timer_ReadStatusRegister();
    
    //Everytime the timer ISR occurs, flag is put to 1
    flag = 1;
}

/* [] END OF FILE */
