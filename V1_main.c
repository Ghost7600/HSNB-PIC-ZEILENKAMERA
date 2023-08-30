/*
 * File:   main.c
 * Author: hahn
 *
 * Created on May 17, 2022, 10:06 AM
 */
/*INCLUDES BEGIN*/
#include <xc.h>
#include <p24HJ128GP202.h>
#include <stdlib.h>
#include <p24Hxxxx.h>
#include "config.h"

//#include <adcDrv2.h>

/*INCLUDES END*/
/*DEFINES BEGIN*/
#define NPIXEL 2547 
#define _XTAL_FREQ 10000000L
#define T1 5                          //Zyklenzahl für 5000ns   12
#define T2 1                           //Zyklenzahl für 500ns   2   
#define T3 1                           //Zyklenzahl für 1000ns  2
/*DEFINE END*/

/*PRAGMAS BEGIN*/

/*PRAGMAS END*/


/*SETUP BEGIN*/

//PORT CONFIG BEGIN

//PORT CONFIG END
//UART SETUP BEGIN

//UART SETUP END
//PUBLIC VAR BEGIN
   volatile unsigned int buffer[NPIXEL];    //Array mit Anzahl an Pixel 
   volatile unsigned int i=0;               //Bufferindex 

 //  int BufferA[NPIXEL] __attribute__((space(dma)));
 //  int BufferB[NPIXEL] __attribute__((space(dma)));
//PUBLIC VAR END

//PRIVATE VAR BEGIN

//PRIVATE VAR END
/*SETUP END*/

/*MAIN BEGIN*/
int main(void) {
//    unsigned int BufferA[1274] __attribute__((space(dma)));
  //  unsigned int BufferB[1274] __attribute__((space(dma)));

    int start_flag = 1;             //Startflag wird gesetzt durch Kommunikation mit RASPI
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
    CLKDIVbits.PLLPRE = 0;          //OSC Vorteiler =2
    CLKDIVbits.PLLPOST = 0;         //OSC Nachteiler=2
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
    IEC0bits.AD1IE = 0; // Do Not Enable Analog-to-Digital interrupt
   // AD1CON1bits.ADON = 1;               //ADC Start
    //ADC SETUP END
    
    //DMA SETUP BEGIN
    DMA0CONbits.AMODE = 0; // Configure DMA for Register indirect with post increment
    DMA0CONbits.MODE = 2; // Configure DMA for Continuous Ping-Pong mode
    DMA0PAD = (int)&ADC1BUF0;
    DMA0CNT = 0;
    DMA0REQ = 13;
 //   DMA0STA = __builtin_dmaoffset(BufferA);
  //  DMA0STB = __builtin_dmaoffset(BufferB);
    IFS0bits.DMA0IF = 0; //Clear the DMA interrupt flag bit
    IEC0bits.DMA0IE = 0; //Set the DMA interrupt enable bit

    //DMA SETUP END
    
    
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
        
        
//CLOCK SWITCH
 if(OSCCONbits.COSC != 3)               //Überprüfe ob momentane Clock die richtige ist
 {
     OSCCONbits.NOSC = 3;               //Falls nicht setze neue Clock auf XT+PLL
     while(OSCCONbits.LOCK == 0){};     //Warte auf eingeschwungene PLL
     __builtin_write_OSCCONH(0x03);
    __builtin_write_OSCCONL(0x01);

     while(OSCCONbits.COSC != 3)       //Solange die aktuelle OSC-Quelle nicht die gewünschte ist
     {
         OSCCONbits.OSWEN = 1;          //OSC-Switch enable
     }
 }
//CLOCK SWITCH DONE
        
       
 /*HAUPTPROGRAM MAIN
  * nach dem Startsignal über I2C wird der Timer für Phi gestartet                  *I2C muss realisiert werden
  * über TimergesteuerteInterrupte wird der Port getoggelt                          *Timer ist soweit fertig
  * 
  * über Warteschleifen werden die Signale SH & ICG generiert                       *fertig
  * nachdem ICG HIGH wird, startet der AD-Wandler und die DMA           
  * 
  * Die DMA wird durch einen Interrupt des AD Wandler gesteuert und beschreibt 
  * das NPIXEL breite Array. Danach wird DMA und ADC abgeschalten
  * 
  * Daraufhin erfolgt die Übertragung über I2C an den Raspi
  */   
while(1)
    {
    //Abfrage I2C wann es losgehen soll
   while (start_flag == 0){};          //warte auf Startsignal ausgehend vom Raspi
    T2CONbits.TON = 1;                  // Start Timer
    
    PORTBbits.RB3 = 1;                  //ICG wird auf LOW gezogen
    //int ICG = 1;
    /*for (int a=0; a<T2; a++)            //T2 (Wartezeit zwischen SH & /ICG)
        {
        Nop();
        Nop();
       
        }*/
    
     Nop(); Nop(); Nop(); Nop(); Nop();// Nop(); Nop(); Nop(); Nop(); Nop();
   
    PORTBbits.RB4 = 0;                  //SH wird HIGH 
    //int SH = 0; 
    for (int b=0; b<T3; b++)            //T3 (Wartezeit wenn SH HIGH)
        {
        Nop();
        Nop();
        Nop();
        Nop();
       // Nop();
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
    
    AD1CON1bits.ADON = 1;
    IEC0bits.AD1IE = 1;
   // DMA0CONbits.CHEN = 1;
    for (unsigned int c=0; c < 20376; c++)            //T1 (Wartezeit wenn SH LOW & ICG HIGH)
        {
        Nop();
        }
       i = 0;
  //  IEC0bits.AD1IE = 0;
 
    }
    
}
/*MAIN END*/
/*ISR Definition*/
void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void)
{
/* Interrupt Service Routine code goes here */
        
 //  PORTBbits.RB2 = ~PORTBbits.RB2;     //Pin Toggle Phi(M)-Signal
    IFS0bits.T2IF = 0;                  // Clear Timer1 Interrupt Flag       
    return;
}

/*void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void)
{
    return;
}
 */

void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void)
{
    buffer[i]= ADCBUF0;
    i++;
    IFS0bits.AD1IF=0;
    return;
}

/*void __attribute__((interrupt, no_auto_psv)) _DMA0Interrupt(void)
{
    
    unsigned int *addr_DMA_A = (unsigned int *) &DMA0STA;
    unsigned int *addr_DMA_B = (unsigned int *) &DMA0STB;
    buffer[i] = *addr_DMA_A;
    buffer[i+1] = *addr_DMA_B;
    i=i+2;
    
    IFS0bits.DMA0IF = 0;
    return;
}
*/
/*
void __attribute__((__interrupt__,no_auto_psv)) _OC1Interrupt( void )
{

IFS0bits.OC1IF = 0; // Clear OC1 interrupt flag
return;
}
*/
/*void data_transfer(*buffer)
{
    
    return 1;
}
 */