/*
 * Copyright (c) Mr.Lee. 2021. All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2021-03-21
 */

#include "lcd_ssd1963.h"
#include <stdint.h>
#include "gpio.h"
#include "uart_device.h"
#include "dwt_delay.h"
#include "GUI.h"

	
//LCD驱动参数设置
#define SSD_HOR_PULSE_WIDTH		1		//水平脉宽
#define SSD_HOR_BACK_PORCH		46		//水平前廊
#define SSD_HOR_FRONT_PORCH		210		//水平后廊

#define SSD_VER_PULSE_WIDTH		1		//垂直脉宽
#define SSD_VER_BACK_PORCH		23		//垂直前廊
#define SSD_VER_FRONT_PORCH		22		//垂直前廊

#define SSD_HT	(SSD_HOR_RESOLUTION+SSD_HOR_BACK_PORCH+SSD_HOR_FRONT_PORCH)
#define SSD_HPS	(SSD_HOR_BACK_PORCH)
#define SSD_VT 	(SSD_VER_RESOLUTION+SSD_VER_BACK_PORCH+SSD_VER_FRONT_PORCH)
#define SSD_VPS (SSD_VER_BACK_PORCH)

uint16_t g_circleX = 0;
uint16_t g_circleY = 0;


void LcdWriteCmd8Bit(uint8_t cmd)
{
    *(volatile uint8_t *)LCD_CMD_ADDR = cmd;
}

void LcdWriteCmd16Bit(uint16_t cmd)
{
    *(volatile uint16_t *)LCD_CMD_ADDR = cmd;	
}

void LcdWriteData8Bit(uint8_t data)
{
    *(volatile uint8_t *)LCD_DATA_ADDR = data;	
}

void LcdWriteData16Bit(uint16_t data)
{
    *(volatile uint16_t *)LCD_DATA_ADDR = data;	
}

uint8_t LcdReadData8Bit(void)
{
    return (uint8_t)(*(volatile uint8_t *)LCD_DATA_ADDR);
}

uint16_t LcdReadData16Bit(void)
{
    return *(volatile uint16_t *)LCD_DATA_ADDR;
}

/******************************************************/

//void LccdReset(void)
//{
//    HAL_GPIO_WritePin(DCMI_PWDN_GPIO_Port, DCMI_PWDN_Pin, GPIO_PIN_RESET);
//    DelayM(100);
//    HAL_GPIO_WritePin(DCMI_PWDN_GPIO_Port, DCMI_PWDN_Pin, GPIO_PIN_SET);
//}

void LcdBacklightOn(void)
{
    HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET);
}

void LcdBacklightOff(void)
{
    HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_RESET);	
}

void LcdRegConfig(void)
{	
	LcdWriteCmd8Bit(0xE2);		//Set PLL with OSC = 10MHz (hardware),	Multiplier N = 35, 250MHz < VCO < 800MHz = OSC*(N+1), VCO = 300MHz
	LcdWriteData8Bit(0x1D);		//参数1 
	LcdWriteData8Bit(0x02);		//参数2 Divider M = 2, PLL = 300/(M+1) = 100MHz
	LcdWriteData8Bit(0x04);		//参数3 Validate M and N values   
	DelayU(100);
	LcdWriteCmd8Bit(0xE0);		// Start PLL command
	LcdWriteData8Bit(0x01);		// enable PLL
	DelayM(10);
	LcdWriteCmd8Bit(0xE0);		// Start PLL command again
	LcdWriteData8Bit(0x03);		// now, use PLL output as system clock	
	DelayM(12);  
	LcdWriteCmd8Bit(0x01);		//软复位
	DelayM(400);
	
	LcdWriteCmd8Bit(0xE6);		//设置像素频率,33Mhz
	LcdWriteData8Bit(0x2F);
	LcdWriteData8Bit(0xFF);
	LcdWriteData8Bit(0xFF);
	
	LcdWriteCmd8Bit(0xB0);		//设置LCD模式
	LcdWriteData8Bit(0x20);		//18位模式
	LcdWriteData8Bit(0x00);		//TFT 模式 

	LcdWriteData8Bit((uint8_t)((SSD_HOR_RESOLUTION-1)>>8));//设置LCD水平像素
	LcdWriteData8Bit((uint8_t)(SSD_HOR_RESOLUTION-1));		 
	LcdWriteData8Bit((uint8_t)((SSD_VER_RESOLUTION-1)>>8));//设置LCD垂直像素
	LcdWriteData8Bit((uint8_t)(SSD_VER_RESOLUTION-1));		 
	LcdWriteData8Bit(0x00);		//RGB序列 
	
	LcdWriteCmd8Bit(0xB4);		//Set horizontal period
	LcdWriteData8Bit((uint8_t)((SSD_HT-1)>>8));
	LcdWriteData8Bit((uint8_t)(SSD_HT-1));
	LcdWriteData8Bit(SSD_HPS>>8);
	LcdWriteData8Bit(SSD_HPS);
	LcdWriteData8Bit(SSD_HOR_PULSE_WIDTH-1);
	LcdWriteData8Bit(0x00);
	LcdWriteData8Bit(0x00);
	LcdWriteData8Bit(0x00);
	LcdWriteCmd8Bit(0xB6);		//Set vertical period
	LcdWriteData8Bit((SSD_VT-1)>>8);
	LcdWriteData8Bit((uint8_t)(SSD_VT-1));
	LcdWriteData8Bit(SSD_VPS>>8);
	LcdWriteData8Bit(SSD_VPS);
	LcdWriteData8Bit(SSD_VER_FRONT_PORCH-1);
	LcdWriteData8Bit(0x00);
	LcdWriteData8Bit(0x00);
	
	LcdWriteCmd8Bit(0xF0);	//设置SSD1963与CPU接口为16bit  
	LcdWriteData8Bit(0x03);	//16-bit(565 format) data for 16bpp 

	LcdWriteCmd8Bit(0x29);	//开启显示
	//设置PWM输出  背光通过占空比可调 
	LcdWriteCmd8Bit(0xD0);	//设置自动白平衡DBC
	LcdWriteData8Bit(0x00);	//disable

	LcdWriteCmd8Bit(0xBE);	//配置PWM输出
	LcdWriteData8Bit(0x05);	//1设置PWM频率
	LcdWriteData8Bit(0xFE);	//2设置PWM占空比
	LcdWriteData8Bit(0x01);	//3设置C
	LcdWriteData8Bit(0x00);	//4设置D
	LcdWriteData8Bit(0x00);	//5设置E 
	LcdWriteData8Bit(0x00);	//6设置F 
	
	LcdWriteCmd8Bit(0xB8);	//设置GPIO配置
	LcdWriteData8Bit(0x03);	//2个IO口设置成输出
	LcdWriteData8Bit(0x01);	//GPIO使用正常的IO功能 
	LcdWriteCmd8Bit(0xBA);
	LcdWriteData8Bit(0X01);	//GPIO[1:0]=01,控制LCD方向
	
	LCD_SSD_BackLightSet(100);
}


void LCD_SSD_BackLightSet(uint8_t pwm)
{	
	LcdWriteCmd8Bit(0xBE);	//配置PWM输出
	LcdWriteData8Bit(0x05);	//1设置PWM频率
	LcdWriteData8Bit(pwm*2.55);//2设置PWM占空比
	LcdWriteData8Bit(0x01);	//3设置C
	LcdWriteData8Bit(0xFF);	//4设置D
	LcdWriteData8Bit(0x00);	//5设置E
	LcdWriteData8Bit(0x00);	//6设置F
}


void LcdSetCursor(uint16_t x, uint16_t y)
{
    LcdWriteCmd8Bit(0x2a);
    LcdWriteData8Bit(x>>8);
    LcdWriteData8Bit(x&0xff);
    LcdWriteData8Bit(x>>8);
    LcdWriteData8Bit(x&0xff);	
	
    LcdWriteCmd8Bit(0x2b);
    LcdWriteData8Bit(y>>8);
    LcdWriteData8Bit(y&0xff);	
    LcdWriteData8Bit(y>>8);
    LcdWriteData8Bit(y&0xff);	
}


void LcdSetWindow(uint16_t x0, uint16_t y0, uint16_t xL, uint16_t yL)
{
    LcdWriteCmd8Bit(0x2a);
    LcdWriteData8Bit(x0>>8);
    LcdWriteData8Bit(x0&0xff);
    LcdWriteData8Bit((x0+xL-1)>>8);
    LcdWriteData8Bit((x0+xL-1)&0xff);	
	
    LcdWriteCmd8Bit(0x2b);
    LcdWriteData8Bit(y0>>8);
    LcdWriteData8Bit(y0&0xff);	
    LcdWriteData8Bit((y0+yL-1)>>8);
    LcdWriteData8Bit((y0+yL-1)&0xff);
	
    LcdWriteCmd8Bit(0x2c);		
}

void LcdFillRect(uint16_t x0, uint16_t y0, uint16_t xL, uint16_t yL)
{
    LcdSetWindow(x0, y0, xL, yL);
	LcdFillColor(xL*yL, FONT_BK_COLOR); 
}

void LcdFillColor(uint32_t cnt, uint16_t color)
{
	uint32_t i = cnt;
	
//    LcdWriteCmd8Bit(0x2c);

	while(i--)
	{
		LcdWriteData16Bit(color);
	}
}



void LcdClear(void)
{
	uint32_t i;
	
    LcdSetWindow(0, 0, SSD_HOR_RESOLUTION, SSD_VER_RESOLUTION);
	i = SSD_HOR_RESOLUTION * SSD_VER_RESOLUTION;
	while(i--)
	{
		LcdWriteData16Bit(0xFFFF);
	}
}

void LcdInit(void)
{

	LcdRegConfig();
//	LcdSsd1963GramScan(0);	
//	LcdClear();
	LcdBacklightOn();
	LcdReadId();	
}

uint16_t g_xL;
uint16_t g_yL;

void LcdSsd1963GramScan (uint8_t option)
{	
//	//参数检查，只可输入0-7
//	if(ucOption >7 )
//		return;
//	
//	//根据模式更新XY方向的像素宽度
//	if(ucOption%2 == 0)	
//	{
//		//0 2 4 6模式下X方向像素宽度为480，Y方向为854
////		g_xL = SSD_VER_RESOLUTION;
////		g_yL = SSD_HOR_RESOLUTION;
//		g_xL = 300;
//		g_yL = 300;		
//	}
//	else				
//	{
//		//1 3 5 7模式下X方向像素宽度为854，Y方向为480
////		g_xL = SSD_HOR_RESOLUTION;
////		g_yL = SSD_VER_RESOLUTION;
//		g_xL = 300;
//		g_yL = 300;				
//	}
		g_xL = 800;
		g_yL = 480;	
	//0x36命令参数的高3位可用于设置GRAM扫描方向	
	LcdWriteCmd8Bit (0x36);
	LcdWriteData8Bit(option);
	LcdWriteData8Bit(option);
	DEBUG("%02x\r\n",option);
//	LcdWriteCmd8Bit ( 0x2a ); 
//	LcdWriteData16Bit(0x0000);
//	LcdWriteData16Bit(g_xL);
//	LcdWriteCmd8Bit ( 0x2b );	
//	LcdWriteData16Bit(0x0000);
//	LcdWriteData16Bit(g_yL);

	LcdWriteCmd8Bit ( 0x2a ); 	
	LcdWriteData8Bit ( 0x00 );		/* x 起始坐标高8位 */
	LcdWriteData8Bit ( 0x00 );		/* x 起始坐标低8位 */
	LcdWriteData8Bit ( ((g_xL-1)>>8)&0xFF ); /* x 结束坐标高8位 */	
	LcdWriteData8Bit ( (g_xL-1)&0xFF );				/* x 结束坐标低8位 */

	LcdWriteCmd8Bit ( 0x2b ); 
	LcdWriteData8Bit ( 0x00 );		/* y 起始坐标高8位 */
	LcdWriteData8Bit ( 0x00 );		/* y 起始坐标低8位 */
	LcdWriteData8Bit ( ((g_yL-1)>>8)&0xFF );	/* y 结束坐标高8位 */	 
	LcdWriteData8Bit ( (g_yL-1)&0xFF );				/* y 结束坐标低8位 */

	/* write gram start */
	LcdWriteCmd8Bit ( 0x2c );	
	
	LcdFillColor(800*480, 0x0000);
	
//	LcdFillColor(200*400, 0xffff);
}

void CameraSetWindow(void)
{
//	LcdSetWindow(0, FONT_HEIGHT, SSD_HOR_RESOLUTION, SSD_VER_RESOLUTION-FONT_HEIGHT);
	DisCircle();	
	LcdSetWindow(200, 0, SSD_HOR_RESOLUTION-200, SSD_VER_RESOLUTION-100);

}


void LcdReadId(void)
{
	uint16_t id = 0;
	
    LcdWriteCmd8Bit(0xa1);
	id = LcdReadData8Bit();
	id = LcdReadData8Bit();	
	id = LcdReadData8Bit();	
	id <<= 8;
	id |= LcdReadData8Bit();	
	
	if (id == 0x5761)
	{
		DEBUG("LCD Init OK, ID %04X\r\n", 0x1963);
	}
	else
	{
		DEBUG("LCD error %04X\r\n", id);		
	}
}

void DisCircle(void)
{
	GUI_SetColor(GUI_RED);
	GUI_DrawCircle(g_circleX + 220, g_circleY + 20, 20);
	GUI_SetColor(GUI_BLUE);
}






/************************ (C) COPYRIGHT Mr.Lee *****END OF FILE****/
