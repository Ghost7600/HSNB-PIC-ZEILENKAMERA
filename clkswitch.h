/* 
 * File:   clkswitch.h
 * Author: Kenner Marqueti Couto / Felix Hahn
 * Comments: Output compare configuration function
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_CLKSWITCH_H
#define	XC_CLKSWITCH_H

// include processor files - each processor file is guarded.

#include <xc.h>   

// TODO Insert appropriate #include <>

// Comment a function and leverage automatic documentation with slash star star

void set_clkswitch (void);

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

#endif	//XC_CLKSWITCH_H

