/******************************************************************************
**************************Hardware interface layer*****************************
* | file      	:	DEV_Config.h
* |	version		  :	V1.0
* | date		    :	2020-06-16
* | function	  :	Provide the hardware underlying interface	
******************************************************************************/
#ifndef _DEV_CONFIG_H_
#define _DEV_CONFIG_H_

#include <SPI.h>
#include <Wire.h>
#include <avr/pgmspace.h>
/**
 * data
**/
#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t

#define USE_SPI_4W  1
#define USE_IIC     0

#define IIC_CMD        0X00
#define IIC_RAM        0X40

/****************************************************************************************
    //Use the  library function definition
*****************************************************************************************/
//GPIO config
#define OLED_CS0 6
#define OLED_CS0_0     digitalWrite(OLED_CS0, LOW)
#define OLED_CS0_1     digitalWrite(OLED_CS0, HIGH)

#define OLED_CS1 9
#define OLED_CS1_0     digitalWrite(OLED_CS1, LOW)
#define OLED_CS1_1     digitalWrite(OLED_CS1, HIGH)

#define OLED_CS2 10
#define OLED_CS2_0     digitalWrite(OLED_CS2, LOW)
#define OLED_CS2_1     digitalWrite(OLED_CS2, HIGH)

#define OLED_RST 8
#define OLED_RST_0    digitalWrite(OLED_RST, LOW)
#define OLED_RST_1    digitalWrite(OLED_RST, HIGH)

#define OLED_DC 7
#define OLED_DC_0     digitalWrite(OLED_DC, LOW)
#define OLED_DC_1     digitalWrite(OLED_DC, HIGH)

/*------------------------------------------------------------------------------------------------------*/
uint8_t System_Init(void);

void SPI4W_Write_Byte(uint8_t DATA);
void I2C_Write_Byte(uint8_t value, uint8_t Cmd);

void Driver_Delay_ms(unsigned long xms);
void Driver_Delay_us(int xus);

#endif
