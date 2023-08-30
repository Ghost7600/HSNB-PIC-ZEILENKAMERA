/* 
 * File: pconfig.h  
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PCONFIG_H
#define	PCONFIG_H

#include <xc.h> // include processor files - each processor file is guarded.  

//#define NPIXEL 2000
#define _XTAL_FREQ 10000000L
#define T1 5                          //Zyklenzahl für 5000ns   12
#define T2 1                           //Zyklenzahl für 500ns   2   
#define T3 4

extern int debug;

// TODO Insert appropriate #include <>

// TODO Insert C++ class definitions if appropriate

// TODO Insert declarations

// Comment a function and leverage automatic documentation with slash star star


    void pconfig (void);
/**
  
    <p><b>Summary: do initial setup of ports and peripherals</b></p>

    <p><b>Parameters: null </b></p>

    <p><b>Returns: null </b></p>

    <p><b>Example:</b></p>
    <code>
    void pconfig (void);
    </code>

    <p><b>Remarks:</b></p>
 */
    
    
    /**
  
    <p><b>Summary: Routine after raspis signal</b></p>

    <p><b>Parameters: null </b></p>

    <p><b>Returns: null </b></p>

    <p><b>Example:af_raspi()</b></p>
    <code>
    void af_raspi (void);
    </code>

    <p><b>Remarks:</b></p>
 */
    
    void af_raspi (void);
    
// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* PCONFIG_H */

