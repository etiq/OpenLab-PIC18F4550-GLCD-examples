
/*
 * File name            : main.c
 * Compiler             : MPLAB XC8/ MPLAB C18 compiler
 * IDE                  : Microchip  MPLAB X IDE v1.90/ MPLAB IDE v8
 * Author               : Etiq Technologies
 * Description          : Example_8.2 GLCD_2
 *                      : Created on November 6, 2013, 2:00 PM
 *                      : Example code, demonstrates GLCD initialization, and drawing circle, line, rectangle.
 */

#include <xc.h>
#include "config.h"
#include "KS0108.h"
#include "graphic.h"
#include "delay.h"
#include "KS0108-PIC18.h"
#include "font5x8.h"
#include "bitmap.h"
#ifndef _XTAL_FREQ
#define _XTAL_FREQ 20000000
#endif

void main (void)
    {
    CMCON |= 7;
    ADCON1=0x0F;
    GLCD_Initialize();
    GLCD_ClearScreen();
while(1)
    {
    GLCD_WriteString("ETIQ TECHNOLOGIES");
    Delay_s(2);
    GLCD_Circle(64, 32, 28);
    Delay_s(2);
    GLCD_ClearScreen();
    GLCD_Rectangle(30, 12, 70,40);
    Delay_s(2);
    GLCD_ClearScreen();
    }
}