/*
 * File:   main.c
 * Author: tbi
 * Created on December 11, 2013, 4:17 PM
 */
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "delay.h"
#include "config.h"

#define GLCD_Data   PORTD
#define GLCD_Dir    TRISD

#define GLCD_CS1 LATAbits.LATA3;
#define GLCD_CS2 LATAbits.LATA4;
#define GLCD_RS  LATA2;
#define GLCD_RW  LATA1;
#define GLCD_RST LATA5;
#define GLCD_EN  LATA0;

#define GLCD_CS1_Direction TRISA3;
#define GLCD_CS2_Direction TRISA4;
#define GLCD_RS_Direction  TRISA2;
#define GLCD_RW_Direction  TRISA1;
#define GLCD_RST_Direction TRISA5;
#define GLCD_EN_Direction  TRISA0;

#define ANSEL ADCON1

void Enable_Pulse()
{
   LATA0 = 1;
   delay_us(1);
   LATA0 = 0;                                                  //EN low
   delay_us(1);
}

void GLCD_ON()
{
    //Activate both chips
    LATAbits.LATA3 = 1;                                                 //CS1 ON
    LATAbits.LATA4 = 1;                                                 //CS2 ON
    LATA2 = 0;                                                 //RS low --> command
    LATA1 = 0;                                                 //RW low --> write
    PORTD = 0x3F;                                                       //ON command
    Enable_Pulse();
}

void Set_Start_Line(unsigned short line)
{
    LATA2 = 0;                                             //RS low --> command
    LATA1 = 0;                                             //RW low --> write

    //Activate both chips

    LATAbits.LATA4 = 1;                                             //CS2 ON
    LATAbits.LATA3 = 1;                                             //CS1 ON
    PORTD = 0xC0 | line;                                          //Set Start Line command
    Enable_Pulse();
}

void GOTO_COL(unsigned int x)
{
   unsigned short Col_Data;
   LATA2 = 0;                                                    //RS low --> command
   LATA1 = 0;                                                    //RW low --> write
   if(x<64)                                                               //left section
   {
      LATAbits.LATA4 = 0;                                                 //select chip 1
      LATAbits.LATA3 = 1;                                                 //deselect chip 2
      Col_Data = x;                                                       //put column address on data port
   }
   else                                                                   //right section
   {

      LATAbits.LATA4 = 1;
      LATAbits.LATA3 = 0;
      Col_Data = x-64;                                                    //put column address on data port
   }
   Col_Data = (Col_Data | 0x40 ) & 0x7F;                                  //Command format
   PORTD = Col_Data;
   Enable_Pulse();
}


void GOTO_ROW(unsigned int y)
{
   unsigned short Row_Data;
   LATA2 = 0;                                          //RS low --> command
   LATA1 = 0;                                          //RW low --> write
   Row_Data = (y | 0xB8 ) & 0xBF;                           //put row address on data port set command
   PORTD = Row_Data;
   Enable_Pulse();
}

void GOTO_XY(unsigned int x,unsigned int y)
{
    GOTO_COL(x);
    GOTO_ROW(y);
}

void GLCD_Write(unsigned short b)
{
   LATA2 = 1;                                  //RS high --> data
   LATA1 = 0;                                  //RW low --> write
   PORTD = b;                                           //put data on data port
   delay_us(10);
   Enable_Pulse();
}

unsigned short GLCD_Read(unsigned short column)
{
    unsigned short read_data = 0;                       //Read data here
    GLCD_Dir = 0xFF;                                    //PORTD as Input
    LATA2 = 1;                                 //Read
    LATA1 = 1;                                 //Data
    LATAbits.LATA4 = (column>63);                       //If the expression true then CS2 = 1
    LATAbits.LATA3 = !LATAbits.LATA4;                   //CS1 = ~CS2
    delay_us(1);                                        //tasu
    LATA0 = 1;                                 //Latch RAM data into ouput register
    delay_us(1);                                        //twl + tf

    //Dummy read

    LATA0 = 0;                                 //Low Enable
    delay_us(5);
    LATA0 = 1;                                 //latch data from output register to data bus
    delay_us(1);                                        //tr + td(twh)
    read_data = GLCD_Data;                              //Input data
    LATA0 = 0;                                 //Low Enable to remove data from the bus
    delay_us(1);                                        //tdhr
    GLCD_Dir = 0x00;                                    //Output again
    return read_data;
}

 void GLCD_Clrln(unsigned short ln)
 {
    int i;
    GOTO_XY(0,ln);      //At start of line of left side
    GOTO_XY(64,ln);     //At start of line of right side (Problem)
    LATAbits.LATA3 = 1;
    for(i=0;i<65;i++)
    {
        GLCD_Write(0xff);
        delay_us(10);
    }
 }

//-- -----------------------

void GLCD_CLR()
{
   unsigned short m;
   for(m=0;m<8;m++){
    GLCD_Clrln(m);
   }
}







 LCD_CLR()
 {
     unsigned char x,y;
     for(y=0;y<64;y++)
     {
     for(x=0;x<128;x++)
          {
            GOTO_XY(x,(y/8));
            GLCD_Write(0X00);
            delay_us(1);
          }
     }
 }
void Draw_Point(unsigned short x,unsigned short y, unsigned short color)
{
    unsigned short Col_Data = 0X0F;
    GOTO_XY(x,(y/8));
    switch (color)
    {
        case 0:         //Light spot
            Col_Data = ~(1<<(y%8)) & GLCD_Read(x);
        break;
        case 1:         //Dark spot
            Col_Data = (1<<(y%8)) | GLCD_Read(x);
        break;
    }
    GOTO_XY(x,(y/8));
    GLCD_Write(Col_Data);
}



void line_(void)
{
    unsigned char x,y=24;
    for(x=0;x<128;x++)
          {
            GOTO_XY(x,(y/8));
            GLCD_Write(0X01);
            delay_us(10);
          }
}
void main()
{
  unsigned short x =0, y = 8;
  unsigned char i, j, m, n, a, b;
  ADCON1  = 0x0F;                                                       // Configure AN pins as digital
  CMCON = 0X07;
  TRISD = 0x00;
  TRISC = 0x00;
  TRISA = 0X00;
  LATAbits.LATA4 = 1;                                             //CS1 ON
  LATAbits.LATA3 = 1;
  LATA5 = 1;
  GLCD_ON();
  LCD_CLR();
  m = 126, n=56;
  Set_Start_Line(0);
  while(1)
  {
          line_();
          delay_us(10);
          LCD_CLR();

/*

          for(y=0;y<n;y++)
          {
          GOTO_XY(x,(y/8));
          GLCD_Write(0X00);
          delay_us(10);
          }
          for(x=m;x>a;x--)
          {
            GOTO_XY(x,(y/8));
            GLCD_Write(0X0f);
            delay_us(10);
          }
          for(y=n;y>b;y--)
          {
          GOTO_XY(x,(y/8));
          GLCD_Write(0X00);
          delay_us(50);
          }
          m=m-4;
          n=n-2;
          a=a+4;
          b=b+2;
 * */
   }
}

