/* 
 * File:   clkswitch.c
 * Author: Kenner Marqueti Couto / Felix Hahn
 * Comments: Clock Switch for PIC 24HJ128GP
 * Revision history: 
 */

#include <xc.h>
#include <p24HJ128GP202.h>
#include <stdlib.h>
#include <p24Hxxxx.h>

void set_clkswitch (void){

//CLOCK SWITCH
 if(OSCCONbits.COSC != 3)               //Check if current clock is correct
 {
     OSCCONbits.NOSC = 3;                //If not set new clock to XT+PLL
     while(OSCCONbits.LOCK == 0){};      //Wait for settled PLL
     __builtin_write_OSCCONH(0x03);
    __builtin_write_OSCCONL(0x01);

     while(OSCCONbits.COSC != 3)       //As long as the current OSC source is not the desired one
     {
         OSCCONbits.OSWEN = 1;          //OSC-Switch enable
     }
 }
//CLOCK SWITCH DONE
}

