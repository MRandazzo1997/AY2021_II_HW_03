/*
*   \ InterruptRoutines.h file
*   \ Authors: Riccardo Mustoni, Alessia Pesenti and Mattia Randazzo
*   \ Date: 04/27/2021
*   \ Assignment 03 - Analog sampling and I2C communication
*   \ Group 11
*/

#ifndef __INTERRUPT_ROUTINES_H__
    
    #define __INTERRUPT_ROUTINES_H__
    #include "project.h"
    #include "I2C_Communication.h"
    #include "I2C_Communication.c"
    
    //UART
    /*  #define BYTES_TO_SEND 4
    #define TRANSMIT_BUFFER_SIZE 1+BYTES_TO_SEND+1 
    uint8 DataBuffer[TRANSMIT_BUFFER_SIZE];
    */  
    
    volatile uint8_t define_status;
    int numSamp;
    
    CY_ISR_PROTO(Custom_ISR_ADC);
    

    
#endif

/* [] END OF FILE */





