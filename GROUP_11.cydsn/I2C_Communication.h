/*
*   \ I2C_Communication.h file
*   \ Authors: Riccardo Mustoni, Alessia Pesenti and Mattia Randazzo
*   \ Date: 04/27/2021
*   \ Assignment 03 - Analog sampling and I2C communication
*   \ Group 11
*/

#ifndef __I2C_COMMUNICATION_H__
    #define __I2C_COMMUNICATION_H__
    
    #include "project.h"
    
    #define SLAVE_BUFFER_SIZE 7
    #define WHO_AM_I  0xBC
    #define SLAVE_BUFFER_RW  2
    uint8_t SlaveBuffer[SLAVE_BUFFER_SIZE];
    
    
    #define DEVICE_STOPPED         0x00
    #define CHANN_TEMP             0x01
    #define CHANN_LDR              0x02
    #define CHANN_BOTH             0x03
    
    
    #define LED_ON        1
    #define LED_OFF       0
    
    int32 ldr,temp, avg_ldr, avg_temp;
    
    
    void stop();
    void start();
    void get_temp();
    void get_ldr();
    void SetBuffer(int32 temp, int32 light);
    
    
#endif

/* [] END OF FILE */
