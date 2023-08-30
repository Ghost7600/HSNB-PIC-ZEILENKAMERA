/* 
 * File:   clkswitch.h
 * Author: Kenner Marqueti Couto / Felix Hahn
 * Comments: Output compare configuration function
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef I2C_H
#define	I2C_H


// include processor files - each processor file is guarded.

#include <xc.h>   

// TODO Insert appropriate #include <>

// Comment a function and leverage automatic documentation with slash star star

#define NPIXEL 2547

typedef struct byteinfo {
    int hlst; /*!< Indicates if it's going to send 1 high or 0 low part. */
    int counter; //counter for instruction, and byte high/low
    int readcounter;
    int byte; /*!< hold the full value of the byte, should not be needed but keeping here just in case*/
    int testflag; /*!<Function return, used for keeping track of things. */
    int index; /*!< Pixel index */
    int indexh; /*!< Pixel index low byte*/
    int indexl; /*!< Pixel index high byte*/
    int order; //used to keep track of what instruction the pic is recieving
} byteinfo;

typedef enum {
    
    NADA,
    HIGHINDEX,
    LOWINDEX,
    READ2,
    MEASURE,
    TRANSFERDATA,
    STOP,
    INDEXING,
    
};


void i2cinitm(void);
struct byteinfo* i2cinits(void);
void i2cmsend(char sadd, char data);
void i2cwrite(void);
void i2csendread10bit(volatile unsigned int (*inputbuffer)[NPIXEL], byteinfo *datas);
void i2csend(uint8_t data);
uint8_t bytesplit (volatile unsigned int ((*inputbuffer)), int index, int flag);
int getindex(byteinfo *datas);
int getcounter(byteinfo* datas);
int gettestflag(byteinfo* datas);
int getindexlow(byteinfo *datas);
int getindexhigh(byteinfo *datas);
int getorder(byteinfo *datas);
int mergeindex(byteinfo *datas);
int gethl(byteinfo* datas);
void storeindex(byteinfo* data, int order);
void treati2c(byteinfo *data, volatile unsigned int (*bfrptr) [NPIXEL], int order, volatile unsigned int *counterptr);





/**
    Setup and configure Direct Memory Access to declutter main
 */


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	//I2C_H

