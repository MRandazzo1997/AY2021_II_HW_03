/*
*
*
*/

#include "project.h"
#include "InterruptRoutines.h"

int main(void)
{
    isr_Timer_StartEx(Custom_Timer_ISR);
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
