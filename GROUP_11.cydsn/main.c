/*
*   \ main.c file
*   \ Authors: Riccardo Mustoni, Alessia Pesenti and Mattia Randazzo
*   \ Date: 04/27/2021
*   \ Assignment 03 - Analog sampling and I2C communication
*   \ Group 11
*/


/*
 * Source file for main.
 * 
 * Analog sampling and I2C communication project - CY8CKIT-059 KIT - See TopDesign for more info on the hardware connections/settings.
 * This project can be used to sample two analog sensors using a single 16-bit Delta-Sigma ADC.
 * The communication has been implemented with an I2C Slave. The data can be visualized plottig the two signals through Bridge Control Panel.
 *
*/


#include "project.h"
#include "InterruptRoutines.h"
#include "I2C_Communication.h"

#define LDR_MUX 0x00
#define TEMP_MUX 0x01
#define START 0
//#define DEBUGGING   // For debug purposes only

volatile int flag = 0;
uint8_t define_status;
int i = 0;
volatile int numSamp;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
   
    /* Initialization/startup code */
    
    // Start ISR with custom function
    isr_Timer_StartEx(Custom_ISR_ADC);
    //Start ADC 
    start();
    //Start timer 
    Timer_Start();
    //Start EZI2C  
    EZI2C_Start();
    //Start MUX
    Analog_MUX_Start();
    
    // Initialize slave buffer
    SlaveBuffer[0] = 0x00;
    SlaveBuffer[1] =0x00;
    SlaveBuffer[2] = WHO_AM_I;
    SetBuffer(0,0);
    
    //Set the buffer size, how many bytes are writable in the beginning of the buffer, and the buffer pointer
    EZI2C_SetBuffer1(SLAVE_BUFFER_SIZE, SLAVE_BUFFER_RW, SlaveBuffer);
    
    /* UART start for debugging */
    #ifdef DEBUGGING
        char message [25] = {"\0"};
        UART_Start();
    #endif
    
    
    for(;;)
    {
        if(i == START || Timer_ReadPeriod() == 0)
        {
            // Initialising variables for average each time the counter is reset
            avg_ldr = 0;
            avg_temp = 0;
            
            //Extract from register1 number of samples to be averaged
            numSamp = (SlaveBuffer[0] & 0b00111100) >> 2;
            //If number of samples is equal to 0, stop the device
            if(numSamp == 0)    define_status = DEVICE_STOPPED;
            
            //Reset timer and set new period
            Timer_WritePeriod(SlaveBuffer[1]);
            Control_Reg_Reset_Write(1);
            CyDelay(1);
            Control_Reg_Reset_Write(0);
            Timer_Start();
        }
        
 
        define_status = (SlaveBuffer[0] & 0b00000011);           //Check the value of define_status (control register 0, bits 0 and 1)      
        switch (define_status){   
            case DEVICE_STOPPED:                                 //Switch to stop device     
                stop();                                          //Stop ADC and LED
                break;
             
                
            case CHANN_TEMP:                                     //Switch to acquiring temperature sensor data
                Pin_LED_Write(LED_OFF);                          //Stop LED
                start();                                         //Start ADC and sampling
                if(i < numSamp && Timer_ReadPeriod()!=0)         
                {   
                    Analog_MUX_FastSelect(TEMP_MUX);             //Select the MUX channel
                    while(!flag);
                    get_temp();                                  //Sample data
                    flag = 0;                                    //Reset the ISR flag
                    i++;                                         //Increment number of samples
                }
                else if(i == numSamp)                            
                {   
                    avg_temp = avg_temp / numSamp;               //Compute the average of the samples acquired
                    avg_temp = ADC_CountsTo_mVolts(avg_temp);    //Convert the average in mVolts
                    SetBuffer(avg_temp, avg_ldr);                //Set the slave buffer according to the average calculated 
                    i=0;                                         //Reset number of samples
                }    
                break;
            
                    
            case CHANN_LDR:                                       //Switch to acquiring photoresistor data
                Pin_LED_Write(LED_OFF);                           //Stop LED 
                start();                                          //Start ADC and sampling
                if(i < numSamp && Timer_ReadPeriod()!=0)          
                {   
                    Analog_MUX_FastSelect(LDR_MUX);               //Select the MUX channel
                    while(!flag);
                    get_ldr();                                    //Sample data
                    flag = 0;                                     //Reset the ISR flag
                    i++;                                          //Increment number of samples
                }  
                else if(i == numSamp)
                {   
                    avg_ldr = avg_ldr / numSamp;                  //Compute the average of the samples acquired
                    avg_ldr = ADC_CountsTo_mVolts(avg_ldr);       //Convert the average in mVolts
                    SetBuffer(avg_temp, avg_ldr);                 //Set the slave buffer according to the average calculated 
                    i=0;                                          //Reset number of samples
                } 
                break;
                
                    
            case CHANN_BOTH:                                      //Switch to acquiring both channels
                Pin_LED_Write(LED_ON);                            //Start LED 
                start();                                          //Start ADC and sampling
                if(i < numSamp && Timer_ReadPeriod()!=0)
                {
                    Analog_MUX_FastSelect(LDR_MUX);               //Select the MUX channels and get the data
                    while(!flag);
                    get_ldr();
                    Analog_MUX_FastSelect(TEMP_MUX);
                    get_temp();
                    flag = 0;                                     //Reset the ISR flag
                    i++;                                          //Increment number of samples
                }    
                else if(i == numSamp)                  
                {
                    avg_ldr = avg_ldr / numSamp;                  //Compute the averages of the samples acquired
                    avg_temp = avg_temp / numSamp;
                 
                    avg_ldr = ADC_CountsTo_mVolts(avg_ldr);       //Convert the averages in mVolts
                    avg_temp = ADC_CountsTo_mVolts(avg_temp);
                    
                    SetBuffer(avg_temp, avg_ldr);
                    
                    i=0;                                          //Reset number of samples
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
           }
        #endif
    }
}

/* [] END OF FILE */
