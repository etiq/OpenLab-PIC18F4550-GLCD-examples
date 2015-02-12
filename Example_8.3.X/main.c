
/*
 * File name            : main.c
 * Compiler             : MPLAB XC8/ MPLAB C18 compiler
 * IDE                  : Microchip  MPLAB X IDE v1.90/ MPLAB IDE v8
 * Author               : Etiq Technologies
 * Description          : Example_8.3 GLCD_3
 *                      : Created on February 25, 2014, 3:00 PM
 *                      : Example code for displaying a bitmap image on graphic LCD.
 */
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

/*  Displaying Bitmap image         */

void main (void)
{
CMCON |= 7;
ADCON1=0x0F;
GLCD_Initialize();
GLCD_ClearScreen();
while(1)
    {
    GLCD_Bitmap(logoglcd1, 0, 0, 128, 64);
    Delay_s(2);
    GLCD_ClearScreen();
    }
}