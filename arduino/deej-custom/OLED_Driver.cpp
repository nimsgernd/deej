/*****************************************************************************
* | File      	:   OLED_Driver.cpp
* | Author      :   Waveshare team
* | Function    :   1.5inch RGB OLED Module Drive function
* | Info        :
*----------------
* |	This version:   V2.0
* | Date        :   2020-08-20
* | Info        :
* -----------------------------------------------------------------------------
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include "OLED_Driver.h"
#include "stdio.h"

/*******************************************************************************
function:
            Hardware reset
*******************************************************************************/
static void OLED_Reset(void)
{
    OLED_RST_1;
    Driver_Delay_ms(100);
    OLED_RST_0;
    Driver_Delay_ms(100);
    OLED_RST_1;
    Driver_Delay_ms(100);
}

/*******************************************************************************
function:
            Write register address and data
*******************************************************************************/
static void OLED_WriteReg(uint8_t Reg, uint8_t cs)
{

#if USE_SPI_4W
    OLED_DC_0;
    switch(cs)
    {
      case 0:
      {
        OLED_CS0_0;
        break;
      }
      case 1:
      {
        OLED_CS1_0;
        break;
      }
      case 2:
      {
        OLED_CS2_0;
        break;
      }
    }
    SPI4W_Write_Byte(Reg);
    switch(cs)
    {
      case 0:
      {
        OLED_CS0_1;
        break;
      }
      case 1:
      {
        OLED_CS1_1;
        break;
      }
      case 2:
      {
        OLED_CS2_1;
        break;
      }
    }
#endif
}

static void OLED_WriteData(uint8_t Data, uint8_t cs)
{   
#if USE_SPI_4W
    OLED_DC_1;
    switch(cs)
    {
      case 0:
      {
        OLED_CS0_0;
        break;
      }
      case 1:
      {
        OLED_CS1_0;
        break;
      }
      case 2:
      {
        OLED_CS2_0;
        break;
      }
    }
    SPI4W_Write_Byte(Data);
    switch(cs)
    {
      case 0:
      {
        OLED_CS0_1;
        break;
      }
      case 1:
      {
        OLED_CS1_1;
        break;
      }
      case 2:
      {
        OLED_CS2_1;
        break;
      }
    }
#endif
}

/*******************************************************************************
function:
        Common register initialization
*******************************************************************************/
static void OLED_InitReg(uint8_t cs)
{
    OLED_WriteReg(0xfd,cs);  // command lock
    OLED_WriteData(0x12,cs);
    OLED_WriteReg(0xfd,cs);  // command lock
    OLED_WriteData(0xB1,cs);

    OLED_WriteReg(0xae,cs);  // display off
    OLED_WriteReg(0xa4,cs);  // Normal Display mode

    OLED_WriteReg(0x15,cs);  //set column address
    OLED_WriteData(0x00,cs);     //column address start 00
    OLED_WriteData(0x7f,cs);     //column address end 127
    OLED_WriteReg(0x75,cs);  //set row address
    OLED_WriteData(0x00,cs);     //row address start 00
    OLED_WriteData(0x7f,cs);     //row address end 127    

    OLED_WriteReg(0xB3,cs);
    OLED_WriteData(0xF1,cs);

    OLED_WriteReg(0xCA,cs);  
    OLED_WriteData(0x7F,cs);

    OLED_WriteReg(0xa0,cs);  //set re-map & data format
    OLED_WriteData(0x74,cs);     //Horizontal address increment

    OLED_WriteReg(0xa1,cs);  //set display start line
    OLED_WriteData(0x00,cs);     //start 00 line

    OLED_WriteReg(0xa2,cs);  //set display offset
    OLED_WriteData(0x00,cs);

    OLED_WriteReg(0xAB,cs);  
    OLED_WriteReg(0x01,cs);  

    OLED_WriteReg(0xB4,cs);  
    OLED_WriteData(0xA0,cs);   
    OLED_WriteData(0xB5,cs);  
    OLED_WriteData(0x55,cs);    

    OLED_WriteReg(0xC1,cs);  
    OLED_WriteData(0xC8,cs); 
    OLED_WriteData(0x80,cs);
    OLED_WriteData(0xC0,cs);

    OLED_WriteReg(0xC7,cs);  
    OLED_WriteData(0x0F,cs);

    OLED_WriteReg(0xB1,cs);  
    OLED_WriteData(0x32,cs);

    OLED_WriteReg(0xB2,cs);  
    OLED_WriteData(0xA4,cs);
    OLED_WriteData(0x00,cs);
    OLED_WriteData(0x00,cs);

    OLED_WriteReg(0xBB,cs);  
    OLED_WriteData(0x17,cs);

    OLED_WriteReg(0xB6,cs);
    OLED_WriteData(0x01,cs);

    OLED_WriteReg(0xBE,cs);
    OLED_WriteData(0x05,cs);

    OLED_WriteReg(0xA6,cs);
}

/********************************************************************************
function:
            initialization
********************************************************************************/
void OLED_1in5_rgb_Init(uint8_t cs)
{
    //Hardware reset
    OLED_Reset();

    //Set the initialization register
    OLED_InitReg(cs);
    Driver_Delay_ms(200);

    //Turn on the OLED display
    OLED_WriteReg(0xAF,cs);
}

/********************************************************************************
function:
            Clear screen
********************************************************************************/
void OLED_1in5_rgb_Clear(uint8_t cs)
{
    UWORD i;

    OLED_WriteReg(0x15,cs);
    OLED_WriteData(0,cs);
    OLED_WriteData(127,cs);
    OLED_WriteReg(0x75,cs);
    OLED_WriteData(0,cs);
    OLED_WriteData(127,cs);
    // fill!
    OLED_WriteReg(0x5C,cs);

    for(i=0; i<OLED_1in5_RGB_WIDTH*OLED_1in5_RGB_HEIGHT*2; i++){
        OLED_WriteData(0x00,cs);
    }
}

/********************************************************************************
function:   Draw a point
********************************************************************************/
void OLED_1in5_rgb_Set_Point(UBYTE Xpoint, UBYTE Ypoint, UWORD Color, uint8_t cs)
{
    OLED_WriteReg(0x15,cs);
    OLED_WriteData(Xpoint,cs);
    OLED_WriteData(Xpoint,cs);
    OLED_WriteReg(0x75,cs);
    OLED_WriteData(Ypoint,cs);
    OLED_WriteData(Ypoint,cs);
    // fill!
    OLED_WriteReg(0x5C,cs);   
    
    OLED_WriteData(Color>>8,cs);
    OLED_WriteData(Color,cs);
}

/********************************************************************************
function:   Update all memory to OLED
********************************************************************************/
void OLED_1in5_rgb_Display(const UBYTE *Image, uint8_t cs)
{
    UWORD i, j, temp;

    OLED_WriteReg(0x15,cs);
    OLED_WriteData(0,cs);
    OLED_WriteData(127,cs);
    OLED_WriteReg(0x75,cs);
    OLED_WriteData(0,cs);
    OLED_WriteData(127,cs);
    // fill!
    OLED_WriteReg(0x5C,cs);   
    
    for(i=0; i<OLED_1in5_RGB_HEIGHT; i++)
        for(j=0; j<OLED_1in5_RGB_WIDTH*2; j++)
        {
            temp = Image[j + i*256];
            OLED_WriteData(temp,cs);
			
        }
}

/********************************************************************************
function:   Update all memory to OLED
********************************************************************************/
void OLED_1in5_rgb_Display_Part(const UBYTE *Image, UBYTE Xstart, UBYTE Ystart, UBYTE Xend, UBYTE Yend, uint8_t cs)
{
    UWORD i, j, temp;

    OLED_WriteReg(0x15,cs);
    OLED_WriteData(Xstart,cs);
    OLED_WriteData(Xend-1,cs);
    OLED_WriteReg(0x75,cs);
    OLED_WriteData(Ystart,cs);
    OLED_WriteData(Yend-1,cs);
    // fill!
    OLED_WriteReg(0x5C,cs);   
    
    for(i=0; i<Yend-Ystart; i++)
        for(j=0; j<(Xend-Xstart)*2; j++)
        {
            temp = pgm_read_byte(&Image[j + i*(Xend-Xstart)*2]);
            OLED_WriteData(temp,cs);
        }
}
