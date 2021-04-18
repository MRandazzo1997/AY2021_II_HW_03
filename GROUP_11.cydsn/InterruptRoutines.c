/*
*   \ InterruptRoutines.c file
*   \ Authors: Riccardo Mustoni, Alessia Pesenti and Mattia Randazzo
*   \ Date: 04/27/2021
*   \ Assignment 03 - Analog sampling and I2C communication
*   \ Group 11
*/

#include "InterruptRoutines.h"
extern volatile int flag;

CY_ISR(Custom_ISR_ADC)
{
    Timer_ReadStatusRegister();
    
    /* Everytime the timer ISR occurs (4 ms), the flag is put to 1
       while the ADC reading is computed in the main.c file
    */
    flag = 1;
}

/* [] END OF FILE */
