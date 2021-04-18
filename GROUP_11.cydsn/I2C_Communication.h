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
    
    
    
#endif

/* [] END OF FILE */
