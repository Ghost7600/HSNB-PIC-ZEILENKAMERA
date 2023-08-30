/* 
 * File:   process_sample.h
 * Author: hahn
 *
 * Created on 16. September 2022, 09:49
 * Diese Funktion spaltet die Abtastwerte in 8-Bit (1 char) auf
 * der outputbuffer ist ein Array mit 2 spalten 
 * 1. Spalte: höherwertiges Byte
 * 2. Spalte: niederwertiges Byte
 * 
 * Übergabe: Adresse von Buffer der Abtastwerte, Adresse des Buffer für I²C Übertragung
 * Rückgabe: leer  
 * 
 */

#ifndef PROCESS_SAMPLE_H
#define	PROCESS_SAMPLE_H

#ifdef	__cplusplus
extern "C" {
#endif

  

    extern void processSamples(volatile unsigned int inputbuffer[2547], volatile unsigned char outputbuffer[2547][2], unsigned long pixelzahl)
    {
        volatile unsigned int inputbufferadress;
        inputbufferadress =  (int) &inputbuffer;  //Eingangsbufferadresse: 0x0856
        for (int highbyte = 0; highbyte<pixelzahl; highbyte++)
        {
           outputbuffer[highbyte][2]= (0x00FF && inputbuffer[highbyte]); //lowbyte extrahieren 
           outputbuffer[highbyte][1]= (0xFF00 && inputbuffer[highbyte])>>8;   
        }
        
        
        return;
    }

#ifdef	__cplusplus
}
#endif

#endif	/* PROCESS_SAMPLE_H */

