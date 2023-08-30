/* 
 * File:   oc.c
 * Author: Kenner Marqueti Couto / Felix Hahn
 * Comments: Output Compare for PIC 24HJ128GP
 * Revision history: 
 */

#include <xc.h>
#include <p24HJ128GP202.h>
#include <stdlib.h>
#include <p24Hxxxx.h>

void set_outcomp (void){

//Outputcompare SETUP BEGIN
        OC1CONbits.OCM = 0b000; // Disable Output Compare Module
        OC1CONbits.OCM = 0b010; // Define Initial State for OC1 Pin (High if OCM=0b010)
        OC1CONbits.OCM = 0b000; // Disable Output Compare Module
        OC1CONbits.OCTSEL = 0; // Select Timer2 as output compare time base
        OC1R= 5; // Load the Compare Register Value
        IPC0bits.OC1IP = 0x02; // Set Output Compare 1 Interrupt Priority Level
        OC1CONbits.OCM = 0b011; // Select the Output Compare mode 
        IFS0bits.OC1IF = 0; // Clear Output Compare 1 Interrupt Flag
        IEC0bits.OC1IE = 0; // Enable Output Compare 1 interrupt
        
        //Outputcompare setup END
}
