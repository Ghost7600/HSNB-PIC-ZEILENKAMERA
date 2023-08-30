/* 
 * File:   i2c.c
 * Author: Kenner Marqueti Couto / Felix Hahn
 * Comments: I2C for PIC 24HJ128GP
 * Revision history: 
 */

#include <xc.h>
#include <p24HJ128GP202.h>
#include <stdlib.h>
#include <stdint.h>
#include <p24Hxxxx.h>
#include "i2c.h"
#include "pconfig.h"

#define FOSC 8000000 //cystal oscilator frequency
#define FCYY FOSC /2
#define I2C_BRG 0x9

#define I2C_READ 1
#define I2C_FALSE 0

#define TRUE 1
#define FALSE 0

void i2cinitm(void) {
    I2C1BRG = I2C_BRG;
    I2CCON = 0b1000000000000000;
    I2CCONbits.I2CEN = 1;
    //I2CCONbits.STREN = 1; // Enable clock stretching
    //    TRISBbits.TRISB8 = 1;
    //    TRISBbits.TRISB9 = 1;
    //I2C1CONbits.DISSLW = 1;

}

void i2cmsend(char sadd, char data) {

    I2CCONbits.SEN = 1; // send start condition;
    while (I2CCONbits.SEN == 1);
    I2CTRN = sadd; //sending address
    while (I2C1STATbits.TBF);
    //    if (I2CSTATbits.ACKSTAT)
    //    {
    //        I2CCONbits.PEN = 1; //send stop condition
    //        while (I2CCONbits.PEN){};
    //        return;
    //    }
    I2CCONbits.PEN = 1; //send stop condition


}

byteinfo* i2cinits() {

    // CONTROL REGISTER
    I2C1BRG = I2C_BRG;
    I2CCON = 0b1000000000000000; //enable the i2c and start the register mostly blank
    I2CCONbits.GCEN = 1; //Enable interrupt calls for I2C
    I2CCONbits.STREN = 1; //Enable software clock strechting
    I2CCONbits.SCLREL = 1; //Release i2c clock SHOULD SET AT THE END OF EVERY TRANSMIT
    I2CCONbits.DISSLW = 1; //DISABLE SLEW RATE CONTROL, CONTROL ONLY REQUIRED FOR 400KHZ
    I2CTRN = 0; //Clear Transmission Register
    I2CADD = 10;
    IEC1bits.SI2CIE = 1; // enable SI2CIF interrupt SlaveI2CFlag
    IFS1bits.SI2CIF = 0;

    byteinfo*saddress;

    saddress = malloc(sizeof (struct byteinfo));

    
    
    return saddress;
    // STATUS REGISTER   
}

int getindex(byteinfo* datas) {
    return datas ->index;
}

uint8_t bytesplit (volatile unsigned int ((*inputbuffer)), int index, int flag) //flag 0 for highbyte 1 for lowbte
{
    volatile unsigned int *end;
    int t;
    switch (flag)
    {
        case 0: end = (inputbuffer+(index));
        t = *end;
        t = t & 0xFF00;
        t = t >> 8;
        t = t & 0x00FF;
        break;
        
        case 1: end = (inputbuffer+(index));
        t = *end;
        t = t & 0x00FF;
        break;
    }
    return t;
}

int getcounter(byteinfo* datas) {
    return datas ->counter;
}

int getreadcounter(byteinfo* datas) {
    return datas ->readcounter;
}

int getindexlow(byteinfo* datas) {
    return datas ->indexl;

}

int getindexhigh(byteinfo* datas) {
    return datas ->indexh;
}

int gettestflag(byteinfo* datas) {
    return datas ->testflag;
}

int getorder(byteinfo* datas) {
    return datas ->order;
}

int gethl(byteinfo* datas) {
    return datas ->hlst;
}

int mergeindex(byteinfo *datas) {
    int low = getindexlow(datas);
    int high = getindexhigh(datas);
    // datas->index = (high << 8) | low;  // there might be a problem in this line, not sure if the arrow operator wil work
    int shigh = high << 8;
    datas -> index = shigh | low;
    return datas->index;
}

void i2csendread10bit(volatile unsigned int ((*inputbuffer)[NPIXEL]), byteinfo *datas) {
    I2CCONbits.SCLREL = 0; // HOLDS CLOCK LOW FOR SPLITTING BITS


    //int index = mergeindex(datas); commented to allow index incrementation
    int index = getindex(datas);
    uint8_t send;
    switch(getreadcounter(datas))
    {
        case 0:  
        send = (bytesplit(inputbuffer,index,0));
         i2csend(send);
        datas->readcounter = READ2;
        break;
        
        case READ2:
        send = (bytesplit(inputbuffer,index,1));
        i2csend(send);
        datas-> readcounter = 0;
        break;
    }
    
     //sending highbzte

    //datas->index = getindex(datas) + 1; //incremental mode
    //    char lowbyte,highbyte =0x0;
    //    lowbyte = (0x00FF && data); //lowbyte extrahieren 
    //    highbyte = (0xFF00 && data) >> 8;
    //datas->byte++;
}

void i2csend(uint8_t data) {

    while (I2CSTATbits.IWCOL == 0 && I2CSTATbits.TBF == 0) //No collision AND Transmit complete
    {

        //TRISBbits.TRISB11 = 0;              // !!!!! Set tristate to digital out DONT THINK WE SHOULD DO THAT
        //TRISBbits.TRISB10 = 1;              // !!!!! Set tristate to digital out DONT THINK WE SHOULD DO THAT

        I2CTRN = data; // Transfer register, write data here.       
    }
}

void storeindex(byteinfo* data, int order) {
    switch (gethl(data)) { //reads order stored in struct and decides if its writing low or high byte
        case LOWINDEX:
            data->indexl = order;
            break;

        case HIGHINDEX:
            data->indexh = order;
            break;
    }
}

void treati2c(byteinfo *data, volatile unsigned int (*bfrptr) [NPIXEL], int order, volatile unsigned int *counterptr) {

    int tmp;
    if (I2CSTATbits.D_A == 1) {
        data -> testflag = 0;
        if (I2CSTATbits.R_W == 0) //case master is trying to write
        {
            if (data->counter == 2) //Instruction was indexing and is now getting second byte, LOWINDEX
            {
                data->hlst = LOWINDEX; // tells the struct we are recieving High byte
                storeindex(data, order);
                order = mergeindex(data);
                data -> counter = HIGHINDEX;
            }
             
            if (data->counter == 1) //Instruction was indexing and is now getting first byte
            {
                data->hlst = HIGHINDEX; // tells the struct we are recieving High byte
                storeindex(data, order);
                data -> counter++;
            }           
            
            if (data->counter == 0) { //GOT INSTRUCTION 
                switch (order) //switch case for knowing if its writing 
                {
                    case INDEXING: data->counter = 1;
                        break;
                        
                    case MEASURE: 
                            af_raspi(); // waits and setups after raspis signal
                            while(*counterptr<= 2547)            //T1 (Wartezeit wenn SH LOW & ICG HIGH)
                            {
                             IEC0bits.AD1IE = 1;
                             Nop();
                            }
                            IEC0bits.AD1IE = 0;
                            *counterptr = 0;
                            //IEC0bits.AD1IE = 0;
                            break;
                }
            }



            
        }

        if (I2CSTATbits.R_W == 1) {
            //i2csendread10bit(bfrptr, data);
        }



    }

    if (I2CSTATbits.D_A == 0) {
        data->testflag = (gettestflag(data) + 1);
        I2CSTATbits.I2COV = 0;
        I2CCONbits.SCLREL = 0; // HOLDS CLOCK LOW FOR SPLITTING BITS
        tmp = I2CRCV;
        while (_RBF) {
            I2CSTATbits.I2COV = 0;
            I2CCONbits.SCLREL = 0;
            tmp = I2CRCV;
        }
        
        if (I2CSTATbits.R_W == 1) {
           i2csendread10bit(bfrptr, data);
        }
        else
        {
        data->counter = 0;        
        }
    }

    if (I2CSTATbits.I2COV == 1) {
        I2CSTATbits.I2COV = 0;
    }

    return;
}