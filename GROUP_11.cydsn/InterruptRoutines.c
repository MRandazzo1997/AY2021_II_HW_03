/*
*   \ InterruptRoutines.c file
*   \ Authors: Riccardo Mustoni, Alessia Pesenti and Mattia Randazzo
*   \ Date: 04/27/2021
*   \ Assignment 03 - Analog sampling and I2C communication
*   \ Group 11
*/

#include "InterruptRoutines.h"
//#include "I2C_Communication.h"
//#include "I2C_Communication.c"
#include "project.h"
#include "stdlib.h"


extern volatile int flag;
uint8_t nSamp;

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
    
    nSamp = SlaveBuffer[0] & 0b00111100;  // Updating numSamp via W on control register 0
    switch(nSamp){
        case 0b00000000:    numSamp = 1;    break;
        case 0b00000100:    numSamp = 2;    break;
        case 0b00001000:    numSamp = 3;    break;
        case 0b00001100:    numSamp = 4;    break;
        case 0b00010000:    numSamp = 5;    break;
        case 0b00010100:    numSamp = 6;    break;
        case 0b00011000:    numSamp = 7;    break;
        case 0b00011100:    numSamp = 8;    break;
        case 0b00100000:    numSamp = 9;    break;
        case 0b00100100:    numSamp = 10;    break;
        case 0b00101000:    numSamp = 11;   break;
        case 0b00101100:    numSamp = 12;   break;
        case 0b00110000:    numSamp = 13;   break;
        case 0b00110100:    numSamp = 14;   break;
        case 0b00111000:    numSamp = 15;   break;
        case 0b00111100:    numSamp = 16;   break;
        default:                            break;
    }
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
