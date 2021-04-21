/*
*   \ main.c file
*   \ Authors: Riccardo Mustoni, Alessia Pesenti and Mattia Randazzo
*   \ Date: 04/27/2021
*   \ Assignment 03 - Analog sampling and I2C communication
*   \ Group 11
*/

#include "project.h"
#include "InterruptRoutines.h"
#include "I2C_Communication.h"
#include "stdio.h"

#define LDR_MUX 0x00
#define TEMP_MUX 0x01
#define START 0
//#define DEBUGGING   // For debug purposes only, comment this line in the final commit

volatile int flag = 0;
uint8_t define_status;

int i = 0, numSamp;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
   
    
    /* Initialization/startup code */
    isr_Timer_StartEx(Custom_ISR_ADC);
    start();
    Timer_Start();
    EZI2C_Start();
    
    Analog_MUX_Init();
    Analog_MUX_Start();
    
    SlaveBuffer[0] = 0x00;
    SlaveBuffer[1] =0x00;
    SlaveBuffer[2] = WHO_AM_I;
    SetBuffer(0,0);  //changed because i cant't define them 0 in communication.h or it gives me 1 exit status 
    
    EZI2C_SetBuffer1(SLAVE_BUFFER_SIZE, SLAVE_BUFFER_RW, SlaveBuffer);
    
    #ifdef DEBUGGING
        char message [25] = {"\0"};
        UART_Start();
    #endif
    
    for(;;)
    {
        if(i == START || Timer_ReadPeriod() == 0)
        {
            /* Initialising variables for average computing each time the counter is reset*/
            avg_ldr = 0;
            avg_temp = 0;
            
            /*Extract from register1 number of sample to be averaged*/
            numSamp = (SlaveBuffer[0] & 0b00111100) >> 2;
            if(numSamp == 0)    define_status = DEVICE_STOPPED;
            
            /*Reset timer and set new period*/
            Timer_WritePeriod(SlaveBuffer[1]);
            Control_Reg_Reset_Write(1);
            CyDelay(1);
            Control_Reg_Reset_Write(0);
            Timer_Start();
        }
        
        /* Reading numSamp samples from the ADC:
            0. Check the value of define_status (control register 0, bits 0 and 1)
            case 00 -> stop
            case 01 or 11: (case 11 -> LED_ON)
                1. Set the MUX channel (temp sensor)
                2. Wait the timer before reading the value (4 ms)
                3. Read the temp and check it to be between 0 and 65535
                4. Sum temp with its previous values and save the avg in I2C bytes
                   5 (MSB) and 6 (LSB)
            case 10 or 11:
                5. Same procedure with the ldr sensor (without point 2.)
            6. Put the timer flag to 0 -> wait 4 ms before a new ADC reading and save
               the avg in I2C bytes 3 (MSB) and 4 (LSB)
            7. Reinitialise the counter i to 0
        */ 
        define_status = (SlaveBuffer[0] & 0b00000011);
        switch (define_status){   //to be implemented how to write register 1 and 2
            case DEVICE_STOPPED:
                stop();
                break;
                
                
            case CHANN_TEMP:
                Pin_LED_Write(LED_OFF);
                start();
                if(i < numSamp && Timer_ReadPeriod()!=0)
                {   
                    Analog_MUX_FastSelect(TEMP_MUX);
                    while(!flag);
                    get_temp();
                    flag = 0;
                    i++;   
                }  
                
                else if(i == numSamp)
                {   
                    avg_temp = avg_temp / numSamp;
                    avg_temp = ADC_CountsTo_mVolts(avg_temp);
                    SetBuffer(avg_temp, avg_ldr);  
                    i=0;
                }    
                break;
            
                    
            case CHANN_LDR:
                Pin_LED_Write(LED_OFF);
                start();
                if(i < numSamp && Timer_ReadPeriod()!=0)
                {   
                    Analog_MUX_FastSelect(LDR_MUX);
                    while(!flag);
                    get_ldr();
                    flag = 0;
                    i++;   
                }  
                
                else if(i == numSamp)
                {   
                    avg_ldr = avg_ldr / numSamp;
                    avg_ldr = ADC_CountsTo_mVolts(avg_ldr);
                    SetBuffer(avg_temp, avg_ldr);  
                    i=0;
                } 
                    
                break;
                
                    
            case CHANN_BOTH:        
                Pin_LED_Write(LED_ON);
                start();
                if(i < numSamp && Timer_ReadPeriod()!=0)
                {
                    Analog_MUX_FastSelect(LDR_MUX);
                    while(!flag);
                    get_ldr();
                    Analog_MUX_FastSelect(TEMP_MUX);
                    get_temp();
                    flag = 0;
                    i++;
                }
                else if(i == numSamp)
                {
                    
                    /* Average calculation using numSamp samples */
                    avg_ldr = avg_ldr / numSamp;
                    avg_temp = avg_temp / numSamp;
                    
                    /* Convertion to mV */
                    avg_ldr = ADC_CountsTo_mVolts(avg_ldr);
                    avg_temp = ADC_CountsTo_mVolts(avg_temp);
                    
                    /*  Use these following lines if you wanna monitor the sensors using Bridge Control Panel
                            rx8 [h=A0] @1ldr @0ldr @1temp @0temp [t=C0], using 2 int variables ldr and temp
                            Bit rate 57600
                    */
                    SetBuffer(avg_temp, avg_ldr);
                    
                    i=0;         
                }
                break;
                
                
            default:
                break;
        }
        
        /* UART communication for debugging */
        #ifdef DEBUGGING
            if(i == 0)
            {
                /*  Use these 2 lines if you wanna monitor the sensors using CoolTerm
                    Bit rate 57600
                */
                sprintf(message,"Temp: %ld mV; LDR: %ld mV\r\n",avg_temp,avg_ldr);
                UART_PutString(message);
                
                // Use this line if you wanna monitor the sensors using BCP
                // UART_PutArray(DataBuffer,TRANSMIT_BUFFER_SIZE);
           }
        #endif
    }
}

/* [] END OF FILE */
