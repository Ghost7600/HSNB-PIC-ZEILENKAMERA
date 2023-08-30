/* 
 * File:   pconfig.c
 * Author: Kenner Marqueti Couto / Felix Hahn
 * Comments: Port configuration and other functions
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
//#ifndef XC_PCONFIG_H
//#define	XC_PCONFIG_H

#include <xc.h>
#include <p24HJ128GP202.h>
#include <stdlib.h>
#include <p24Hxxxx.h>

#define NPIXEL 2000
#define _XTAL_FREQ 10000000L
#define T1 5                          //Zyklenzahl für 5000ns   12
#define T2 1                           //Zyklenzahl für 500ns   2   
#define T3 4

 int debug =187;

void pconfig (void){
   
    //PORT CONFIG BEGIN
    
  //    unsigned int BufferA[1274] __attribute__((space(dma)));
  //  unsigned int BufferB[1274] __attribute__((space(dma)));
   
    /*PORT SETUP BEGIN*/
    TRISBbits.TRISB2 = 0;           //RB2 ist Output (TIM1) PHI(M)
    TRISBbits.TRISB3 = 0;           //RB3 ist Output (TIM2) ICG
    PORTBbits.RB3 = 0;              //ICG nach INverter auf HIGH
    TRISBbits.TRISB4 = 0;           //RB4 ist Output (TIM)  SH
    PORTBbits.RB4 = 1;              //SH nach Inverter auf LOW
    TRISAbits.TRISA0 = 1;           //Port AN0 wird zu analogen Eingang
    RPOR1bits.RP2R = 18;            //Pin RB2 wird von Outputcompare getoggelt
    /*PORT SETUP END*/
    RCONbits.SWDTEN=0;              //Software Watchdogtimer auscchalten
    //OSC SETUP BEGIN
   CLKDIVbits.PLLPOST = 0;         //OSC Postscaler =2
   CLKDIVbits.PLLPRE = 0;          //OSC Prescaler  =2
   // 
    PLLFBDbits.PLLDIV = 0x1E;       //Teiler auf 32 =>40MIPS / fosc=80MHz / Fcy=40MHz
    //OSC SETUP END
    //ADC SETUP BEGIN
   	AD1PCFGL = 0xFFFF;              //AD1PCFGH/AD1PCFGL: Port Configuration Register
   
    AD1PCFGLbits.PCFG0 = 0;         // AN0 as Analog Input  
    
    AD1CON1 = 0;                    //clear all bits
    AD1CON2 = 0;
    AD1CON3 = 0;
    AD1CON1bits.SSRC = 7;           // internal counterends sampling and starts conversion (auto-convert)
    AD1CON1bits.ASAM = 1;           //Autostart sampling direkt nach conversion
    AD1CON1bits.AD12B = 0;          //10-Bit AD Wandlung
    AD1CON1bits.SIMSAM = 0;         //sequentielle Wandlung aller Channel
    AD1CON1bits.ADDMABM = 1;        // DMA buffers are built in conversion order mode
    AD1CON2bits.VCFG = 0;           // Avdd und Avss als Referenzspannung für AD Wandlung
    AD1CON2bits.CHPS = 1;           // converts CH0 +CH1
    AD1CON2bits.SMPI = 0;           // SMPI must be 0
    AD1CON3bits.ADRC = 0;           // Systemclock als Wandlungstaktquelle
    AD1CON3bits.ADCS = 3;           //Tdata=Tad+Tconv mit Tconv=12*Tad mit Tad=Tcy(ADCS+1)
    AD1CON3bits.SAMC = 10;          // Auto Sample Time = 0 * TAD 
   //refspannung für adc auswählen
    AD1CHS0 = 0x0000;
    AD1CHS0bits.CH0SA = 0;          // MUXA +ve input selection (AIN0) for CH0
    AD1CHS0bits.CH0NA = 0;          // MUXA -ve input selection (VREF-) for CH0
    AD1CHS123 = 0x0000;
    AD1CHS123bits.CH123SA =0;       // MUXA +ve input selection (AIN0) for CH1
    AD1CHS123bits.CH123NA =0;       // MUXA -ve input selection (VREF-) for CH1
    IFS0bits.AD1IF = 0; // Clear the Analog-to-Digital interrupt flag bit
    IPC3bits.AD1IP = 6;
    AD1CON1bits.ADON = 1;               //ADC Start
    IEC0bits.AD1IE = 0; // Do Not Enable Analog-to-Digital interrupt = 0
    
    //PORT CONFIG END
    
}

void af_raspi (void){

T2CONbits.TON = 1;                  // Start Timer
    
    PORTBbits.RB3 = 1;                  //ICG is pulled to LOW
    //int ICG = 1;
    /*for (int a=0; a<T2; a++)            //T2 (waiting time between SH & /ICG)
        {
        Nop();
        Nop();
       
        }*/
    
     Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); //Nop(); Nop(); Nop(); Nop();
     
    //debug = 2;
   
    PORTBbits.RB4 = 0;                  //SH goes HIGH 
    //int SH = 0; 
    for (int b=0; b<T3; b++)           //T3 (wait time if SH HIGH)
        {
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
       // Nop();
        //Nop();
        
        }
    PORTBbits.RB4 = 1;                  //SH wird LOW 
    //SH = 1;
    for (int c=0; c<T1; c++)            //T1 (Wartezeit wenn SH LOW & ICG HIGH)
        {
        //Nop();
        Nop();
        }
    PORTBbits.RB3 = 0;                  //ICG wird LOW
    //ICG = 0;
    /*AUSLESEN BEGINNT*/
    
   
    IEC0bits.AD1IE = 1;
   // DMA0CONbits.CHEN = 1;
}

//#endif	XC_PCONFIG_H