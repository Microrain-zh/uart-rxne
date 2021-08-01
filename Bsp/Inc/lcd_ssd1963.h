/*
 * Copyright (c) Mr.Lee 2021 All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2021-02-21
 */

#ifndef __LCD_SSD1963_H
#define __LCD_SSD1963_H

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
2^26 =0X0400 0000 = 64MB,每个 BANK 有4*64MB = 256MB
64MB:FSMC_Bank1_NORSRAM1:0X6000 0000 ~ 0X63FF FFFF
64MB:FSMC_Bank1_NORSRAM2:0X6400 0000 ~ 0X67FF FFFF
64MB:FSMC_Bank1_NORSRAM3:0X6800 0000 ~ 0X6BFF FFFF
64MB:FSMC_Bank1_NORSRAM4:0X6C00 0000 ~ 0X6FFF FFFF
*/
/*
 * 64MB:FSMC_Bank1_NORSRAM4:0X6C00 0000 ~ 0X6FFF FFFF
 *
 */
 
//LCD分辨率设置
#define SSD_HOR_RESOLUTION		800		
#define SSD_VER_RESOLUTION		480	 

#define LCD_CMD_ADDR		(0x6c000000)
#define LCD_RS_AX_BIT		6
#define LCD_DATA_ADDR		(LCD_CMD_ADDR | (1 << (LCD_RS_AX_BIT+1)))

//#define LCD_CMD_ADDR		(0x60000000)
//#define LCD_RS_AX_BIT		18
//#define LCD_DATA_ADDR		(LCD_CMD_ADDR | (1 << (LCD_RS_AX_BIT+1)))


#define FONT_HEIGHT			28
#define FONT_BK_COLOR		0x001f

//0x001f, 0xf800,

/*
 * 边框信息在下方向
 *
 */
 
/*********************横屏**********************/ 
#define U2D_R2L				0x00	
#define U2D_L2R				0x80
#define D2U_R2L				0x40
#define D2U_L2R				0xc0
/*********************竖屏**********************/ 
#define R2L_U2D				0x20	
#define R2L_D2U				0xa0
#define L2R_U2D				0x60
#define L2R_D2U				0xe0

#define SCA_DIRE			D2U_L2R


void LcdWriteData16Bit(uint16_t data);
void LcdWriteCmd16Bit(uint16_t cmd);
uint16_t LcdReadData16Bit(void);
uint8_t LcdReadData8Bit(void);
void LcdSsd1963GramScan(uint8_t option);
void LcdFillRect(uint16_t x0, uint16_t y0, uint16_t xL, uint16_t yL);
void LcdFillColor(uint32_t cnt, uint16_t color);
void LCD_SSD_BackLightSet(uint8_t pwm);
void LcdClear(void);
void LcdInit(void);
void LcdReadId(void);
void LcdBacklightOn(void);
void LcdBacklightOff(void);

void CameraSetWindow(void);
void LcdSetWindow(uint16_t x0, uint16_t y0, uint16_t xL, uint16_t yL);
void DisCircle(void);

extern uint16_t g_circleX;
extern uint16_t g_circleY;

#ifdef __cplusplus
}
#endif

#endif /* __LCD_SSD1963_H */

/************************ (C) COPYRIGHT Mr. Lee *****END OF FILE****/
