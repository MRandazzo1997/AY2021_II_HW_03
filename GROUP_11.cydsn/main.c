/*
*   \ main.c file
*   \ Authors: Riccardo Mustoni, Alessia Pesenti and Mattia Randazzo
*   \ Date: 04/27/2021
*   \ Assignment 03 - Analog sampling and I2C communication
*   \ Group 11
*/

#include "project.h"
#include "InterruptRoutines.h"
#include "stdio.h"

#define LDR_MUX 0x00
#define TEMP_MUX 0x01
//#define HEADER 0xA0 //I don't think we need header and tail in the buffer with i2c protocol
//#define TAIL 0xC0
#define START 0
#define DEBUGGING   // For debug purposes only, comment this line in the final commit

volatile int flag = 0;
int32 ldr = 0, temp = 0, avg_ldr = 0, avg_temp = 0;
char message[25] = {"\0"};
int numSamp = 5, i = 0; 

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
   
    
    /* Initialization/startup code */
    isr_Timer_StartEx(Custom_ISR_ADC);
    void start(); 
    ADC_StartConvert();
    
    Analog_MUX_Init();
    Analog_MUX_Start();
    
    ADC_StartConvert();
    
    EZI2C_SetBuffer1(SLAVE_BUFFER_SIZE, SLAVE_BUFFER_RW ,SlaveBuffer);
    
    SetBuffer();
    
    
    
    //UART_Start();
   
    //UART
    /*DataBuffer[0] = HEADER;
    DataBuffer[TRANSMIT_BUFFER_SIZE-1] = TAIL;
    UART_PutString("\f");*/
    
    
    
    for(;;)
    {
        /*###############*/
        /*  IF SOLUTION  */
        /*###############*/
        
        if(i == START)
        {
            
            /* Initialising variables for average computing each time the counter is reset*/
            avg_ldr = 0;
            avg_temp = 0;
        }
        
        /* for loop, reading numSamp samples from the ADC:
            1. Setting the MUX channel (temp sensor)
            2. Waiting the timer before reading the value (4 ms)
            3. Reading the temp and checking it to be between 0 and 65535
            4. Summing temp with its previous values
            5. Repeating the procedure with the ldr sensor (without point 2.)
            6. Put the timer flag to 0 -> wait 4 ms before a new ADC reading
            7. Increment the sample counter
        */
        switch (define_status){   //to be implemented how to write register 1 and 2
       
            case CHANN_TEMP:
                
                if(i < numSamp)
                {   Analog_MUX_FastSelect(TEMP_MUX);
                    temp = ADC_Read32();
                    if(temp > 65535)    temp = 65535;
                    if(temp < 0)        temp = 0;
                    avg_temp = avg_temp + temp;
                    flag = 0;
                    i++;   
                }  
                
                else if(i == numSamp)
                {    avg_temp = avg_temp / numSamp;
                     avg_temp = ADC_CountsTo_mVolts(avg_temp);
                    
                  SlaveBuffer[3] = avg_temp >> 8;
                  SlaveBuffer[4] = avg_temp & 0xFF;  
                // i have to write 0 on the other ones? 
                 i=0;
                }    
                 break;
            
                    
            case CHANN_LDR:
                    
                    //same as before
                    
                    
                    break;
                
                    
            case CHANN_BOTH:        
         
                if(i < numSamp)
                {
                    Analog_MUX_FastSelect(LDR_MUX);
                    while(!flag);
                    ldr = ADC_Read32();
                    if(ldr > 65535)     ldr = 65535;
                    if(ldr < 0)         ldr = 0;
                    avg_ldr = avg_ldr + ldr;
                    Analog_MUX_FastSelect(TEMP_MUX);
                    temp = ADC_Read32();
                    if(temp > 65535)    temp = 65535;
                    if(temp < 0)        temp = 0;
                    avg_temp = avg_temp + temp;
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
                        SlaveBuffer[1] = avg_ldr >> 8;
                        SlaveBuffer[2] = avg_ldr & 0xFF;
                        SlaveBuffer[3] = avg_temp >> 8;
                        SlaveBuffer[4] = avg_temp & 0xFF;  
                     
                     i=0;
                     
                        
                /* UART communication for debugging */
                #ifdef DEBUGGING
                    
                    /*  Use these 2 lines if you wanna monitor the sensors using CoolTerm
                        Bit rate 57600
                    */
                    sprintf(message,"Temp: %ld mV; LDR: %ld mV\r\n",avg_temp,avg_ldr);
                    UART_PutString(message);
                    
                   // UART_PutArray(SlaveBuffer,TRANSMIT_BUFFER_SIZE);
                    
                #endif
         
                }
                break;
    }
}

//        /* Initialising variables for average computing */
//        avg_ldr = 0;
//        avg_temp = 0;
//        
        /* for loop, reading numSamp samples from the ADC:
            1. Setting the MUX channel (temp sensor)
            2. Waiting the timer before reading the value (4 ms)
            3. Reading the temp and checking it to be between 0 and 65535
            4. Summing temp with its previous values
            5. Repeating the procedure with the ldr sensor (without point 2.)
            6. Put the timer flag to 0 -> wait 4 ms before a new ADC reading
        */
//        for(int i = 0; i < numSamp; i++){ //we are already in a for loop, I think it's better to exploit it
//            Analog_MUX_FastSelect(LDR_MUX);
//            while(!flag);
//            ldr = ADC_Read32();
//            if(ldr > 65535)     ldr = 65535;
//            if(ldr < 0)         ldr = 0;
//            avg_ldr = avg_ldr + ldr;
//            Analog_MUX_FastSelect(TEMP_MUX);
//            temp = ADC_Read32();
//            if(temp > 65535)    temp = 65535;
//            if(temp < 0)        temp = 0;
//            avg_temp = avg_temp + temp;
//            flag = 0;
//        }
//        /* Average calculation using numSamp samples */
//        avg_ldr = avg_ldr / numSamp;
//        avg_temp = avg_temp / numSamp;
//        
//        /* Convertion to mV */
//        avg_ldr = ADC_CountsTo_mVolts(avg_ldr);
//        avg_temp = ADC_CountsTo_mVolts(avg_temp);
        
//        /* UART communication for debugging */
//        #ifdef DEBUGGING
//            
//            /*  Use these 2 lines if you wanna monitor the sensors using CoolTerm
//                Bit rate 57600
//            */
//            sprintf(message,"Temp: %ld mV; LDR: %ld mV\r\n",avg_temp,avg_ldr);
//            UART_PutString(message);
//            
//            /*  Use these following lines if you wanna monitor the sensors using Bridge Control Panel
//                rx8 [h=A0] @1ldr @0ldr @1temp @0temp [t=C0], using 2 int variables ldr and temp
//                Bit rate 57600
//            */
//            DataBuffer[1] = avg_ldr >> 8;
//            DataBuffer[2] = avg_ldr & 0xFF;
//
//            DataBuffer[3] = avg_temp >> 8;
//            DataBuffer[4] = avg_temp & 0xFF;
//            
//            UART_PutArray(DataBuffer,TRANSMIT_BUFFER_SIZE);
//            
//        #endif




/* [] END OF FILE */
