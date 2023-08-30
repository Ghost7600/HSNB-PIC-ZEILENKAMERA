/******************************************************************************/
/* projet testtttt                                                          */
/******************************************************************************/


/*INCLUDES BEGIN*/
#include <xc.h>
#include <p24HJ128GP202.h>
#include <stdlib.h>
#include <p24Hxxxx.h>
#include "config.h"
#include "timer.h"
#include "dma.h"
#include "oc.h"
#include "clkswitch.h"
#include "pconfig.h"
#include "i2c.h"
#include "process_sample.h"

//#include <adcDrv2.h>

/*INCLUDES END*/
/*DEFINES BEGIN*/
//#define NPIXEL 2000                   //DEFINED IN I2C.H
#define _XTAL_FREQ 10000000L
#define T1 5                          //Zyklenzahl für 5000ns   12
#define T2 1                           //Zyklenzahl für 500ns   2   
#define T3 4                           //Zyklenzahl für 1000ns  2

/*DEFINE END*/

/*PRAGMAS BEGIN*/

/*PRAGMAS END*/


/*SETUP BEGIN*/




//I2C SETUP BEGIN

//I2C SETUP END
//PUBLIC VAR BEGIN
    volatile unsigned int buffer[NPIXEL];    //Array mit Anzahl an Pixel 
   //volatile unsigned char I2CDataBuffer[NPIXEL][2]; //Der Buffer wird über I²C an Raspi gesendet
    volatile unsigned int i=0;               //Bufferindex 
    volatile unsigned int c=0;
    volatile unsigned int adcounter = 0;
    volatile unsigned int *counterptr = &adcounter;
    volatile byteinfo *sadd;
   //volatile unsigned int *bfrptr;
   
    struct byteinfo uno = {.byte =0 ,.testflag=0}; //allocates structure and initializaes a few values
    
    struct byteinfo *ptr = &uno; //creates a pointer ptr to the struct uno
    
    volatile unsigned int (*bfrptr) [NPIXEL] = &buffer; //creates a pointer bfrptr that points to the array of pixels
    
    int counter = 0;
   
 //  int BufferA[NPIXEL] __attribute__((space(dma)));
 //  int BufferB[NPIXEL] __attribute__((space(dma)));
//PUBLIC VAR END
//PUBLIC FUNCTIONS BEGIN

//   void processSamples(int * samplebuffer);
//   void transfer_info(int * i2c_buffer);
   
//PUBLIC FUNCTION END

//PRIVATE VAR BEGIN
int start_flag = 1;             //Start flag is set by communicating with RASPI
//PRIVATE VAR END
/*SETUP END*/

/*MAIN BEGIN*/
int main(void) {
    
    
    
    
    pconfig();
    
//    set_dma(); // DMA Setup
//    
    set_timer(); // Timer Setup
//   
    set_outcomp();
//        
//    set_clkswitch();
//    
    sadd = i2cinits();
    
    //af_raspi(); // waits and setups after raspis signal
//    i2cinitm();
//    
//    while (1)
//    {
//        i2cmsend (0b110010,0b101);
//    }
    
   
        
       
 /*MAIN PROGRAM MAIN
   * after the start signal via I2C the timer for Phi is started    *I2C muss realisiert werden
   * The port is toggled via timer-controlled interrupts    *Timer ist soweit fertig
   *
   * The signals SH & ICG are generated via waiting loops    *fertig
   * after ICG goes HIGH, AD converter and DMA starts
   *
   * The DMA is controlled and written by an interrupt from the AD converter
   * the NPIXEL wide array. Then DMA and ADC are switched off
   *
   * The transmission then takes place via I2C to the Raspi
   */
while(1)
    {
    //Query I2C when to start
//   while (getorder(ptr) != MEASURE);          //wait for start signal from Raspi
////   
////   //debug =1;
////
   
   
    while(adcounter < NPIXEL)            //T1 (Wartezeit wenn SH LOW & ICG HIGH)
        {
        Nop();
        }
       adcounter = 0;
  //  IEC0bits.AD1IE = 0;

    }
    
}
/*MAIN END*/
/*ISR Definition*/
void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void)
{
/* Interrupt Service Routine code goes here */
        
    //PORTBbits.RB2 = ~PORTBbits.RB2;     //Pin Toggle Phi(M)-Signal
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
    if(adcounter<=NPIXEL)   {
                    buffer[adcounter]= ADC1BUF0;
                    adcounter++;
                    }
    else{
        //c = 4723;
        }
    IFS0bits.AD1IF=0;
    return;
}

void __attribute__((interrupt, no_auto_psv)) _SI2C1Interrupt(void)
{
    I2CSTATbits.I2COV = 0;
    I2C1CONbits.SCLREL = 0; //holds clock
    int recieve = I2CRCV; // reads buffer to clear register and store data
    I2C1CONbits.SCLREL = 0; //holds clock
    while(_RBF){
        I2CSTATbits.I2COV = 0;
    int recieve = I2CRCV; // reads buffer to clear register and store data
    I2C1CONbits.SCLREL = 0; //holds clock
    }
    treati2c(ptr,bfrptr,recieve,counterptr);   
    I2C1CONbits.SCLREL = 1; // RELEASE CLOCK;
    IFS1bits.SI2C1IF = 0; //Clears interrupt flag
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