/* 
 * File:   timer.c
 * Author: Kenner Marqueti Couto / Felix Hahn
 * Comments:
 * Revision history: 
 */

#include <xc.h>
#include <p24HJ128GP202.h>
#include <stdlib.h>
#include <p24Hxxxx.h>

void set_timer (void){



//TIMER SETUP BEGIN
       /*TIMER2*/
        T2CONbits.TON = 0;      // Disable Timer
        T2CONbits.TCS = 0;      // Select internal instruction cycle clock
        T2CONbits.TGATE = 0;    // Disable Gated Timer mode
        T2CONbits.TCKPS = 0b00; // Select 1:1 Prescaler
        TMR2 = 0x00;            // Clear timer register
        PR2 = 10;                // Load the period value 9+1 cycles => 4Mhz 
                                /*--> Interruptroutine toggelt Pin --> 2x in die ISR = 1Cycle => 2MHz Rechteckschwingung*/ 
        IPC1bits.T2IP = 0x01;   // Set Timer1 Interrupt Priority Level
        IFS0bits.T2IF = 0;      // Clear Timer1 Interrupt Flag
        IEC0bits.T2IE = 0;      // Enable Timer1 interrupt
         
//    int a = 2;    debug infinite loop
//    while (a <= 10 ){
//        a=2;
//    }    
        
        /*TIMER2*/
       /* T2CONbits.TON = 0;
        T2CONbits.TCS = 0;      // Select internal instruction cycle clock
        T2CONbits.TGATE = 0;    // Disable Gated Timer mode
        T2CONbits.TCKPS = 0b00;
        TMR2 = 0x00;
        PR2 = 
        IPC1bits.T2IP= 0x01;   // Set Timer1 Interrupt Priority Level
        IFS0bits.T2IF = 0;      // Clear Timer1 Interrupt Flag
        IEC0bits.T2IE = 1;      // Enable Timer1 interrupt
        */
        /*TIMER3*/
        
        /*TIMER5*/

//TIMER SETUP END
}