
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

void set_dma (void){

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

}