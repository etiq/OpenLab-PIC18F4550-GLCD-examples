/* 
 * File:   KS0108-PIC18.h
 * Author: tbi
 *
 * Created on December 6, 2013, 12:13 PM
 */

#ifndef KS0108_PIC18_H
#define	KS0108_PIC18_H

void GLCD_Delay(void);
void GLCD_InitializePorts(void);
void GLCD_EnableController(unsigned char controller);
void GLCD_DisableController(unsigned char controller);
unsigned char GLCD_ReadStatus(unsigned char controller);
void GLCD_WriteCommand(unsigned char, unsigned char);
unsigned char GLCD_ReadData(void);
void GLCD_WriteData(unsigned char);
unsigned char GLCD_ReadByteFromROMMemory(char *);



#endif	/* KS0108_PIC18_H */

