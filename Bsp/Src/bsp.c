/*
 * Copyright (c) Mr.Lee. 2021. All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2021-03-12
 */

#include "bsp.h"
#include "uart_device.h"
#include "stm32f4xx_hal.h"
#include "GUI.h"
#include "rtc.h"
#include "lcd_ssd1963.h"
#include "ov5640.h"
#include "dwt_delay.h"
#include "dis_china_char.h"
#include "w25qxx.h"
#include <rthw.h>
#include <rtthread.h>
#include "rs485.h"

HAL_StatusTypeDef SetDate(uint8_t years,uint8_t mons,uint8_t dates,uint8_t weeds);
HAL_StatusTypeDef SetTime(uint8_t hours,uint8_t mins,uint8_t secs);
void SetTimeDate(RTC_TimeTypeDef sTime, RTC_DateTypeDef sDate);

volatile uint32_t g_RefreshCnt = 0;

RTC_DateTypeDef sdatestructure = {2, 6, 22, 21};
RTC_TimeTypeDef stimestructure = {23, 5, 0};

void BspInit(void)
{
	__HAL_RCC_CRC_CLK_ENABLE();
	DelayInit();
    Uart1DeviceInit();
//	W25qxx_Init();
	GUI_Init();
//	LcdInit();
//	LcdSsd1963GramScan(SCA_DIRE);
//	LcdFillRect(0,0,300, FONT_HEIGHT);//


	DispTNCompany();
	Ov5640IoInit();
	OV5640_Init();

	OV5640_CONFIG();
	
	SetTimeDate(stimestructure, sdatestructure);
	
	DEBUG("System startup\r\n");	
	
//	while(1)
//	{
//////		LcdBacklightOn();
//////		DelayS(1);
//////		DEBUG();
//////		LcdBacklightOff();
//////		DelayS(1);		
//		if(g_RefreshCnt >= 1000)
//		{
//			RefreshInfo(25.6, 55.4, 102.5);
//		}
//	}
}

//HAL_StatusTypeDef HAL_RTC_SetTime(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, uint32_t Format);
//HAL_StatusTypeDef HAL_RTC_GetTime(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, uint32_t Format);
//HAL_StatusTypeDef HAL_RTC_SetDate(RTC_HandleTypeDef *hrtc, RTC_DateTypeDef *sDate, uint32_t Format);
//HAL_StatusTypeDef HAL_RTC_GetDate(RTC_HandleTypeDef *hrtc, RTC_DateTypeDef *sDate, uint32_t Format);

//设置当前时间
HAL_StatusTypeDef SetTime(uint8_t hours,uint8_t mins,uint8_t secs)
{
    RTC_TimeTypeDef sTime;

    sTime.Hours = hours;
    sTime.Minutes = mins;
    sTime.Seconds = secs;
    return HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
}

//设置日期
HAL_StatusTypeDef SetDate(uint8_t years,uint8_t mons,uint8_t dates,uint8_t weeds)
{
    RTC_DateTypeDef sDate;
   
    sDate.Year=years;
    sDate.Month=mons;
    sDate.Date=dates;
    sDate.WeekDay=weeds;
   
    return HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
}


void RefreshInfo(int temp, float humi, float press)
{
    uint32_t level;
	uint8_t t[16] = {0};
	uint8_t h[16] = {0};	
	uint8_t p[16] = {0};
	uint8_t time[30] = {0};
	
	sprintf((char *)t, "%d", temp);
	sprintf((char *)h, "%.1f", humi);
	sprintf((char *)p, "%.1f", press);	

	HAL_RTC_GetTime(&hrtc, &stimestructure, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sdatestructure, RTC_FORMAT_BIN);
	
//	sprintf((char *)time, "%04d/%02d/%02d  %02d:%02d:%02d",
		sprintf((char *)time, "%04d/%02d/%02d  %02d:%02d",
		sdatestructure.Year+2000, sdatestructure.Month, sdatestructure.Date,
//		stimestructure.Hours, stimestructure.Minutes, stimestructure.Seconds);
		stimestructure.Hours, stimestructure.Minutes);
	GetTemperature();			
//	SendGetTempRequest();	
	GUI_SetFont(&GUI_Font24_ASCII);		
	DcmiStop();
	level = rt_hw_interrupt_disable();	
	RefreshTemperature((const char *)t);
	RefreshHumidity((const char *)h);
//	RefreshPressure((const char *)p);
	RefreshRTC((const char *)time);
	rt_hw_interrupt_enable(level);
	Ov5640DcmiDmaConfig();
//	GUI_SetFont(&GUI_Font24_ASCII);	
}

void SetTimeDate(RTC_TimeTypeDef sTime, RTC_DateTypeDef sDate)
{
	
	RTC_DateTypeDef date;
	RTC_TimeTypeDef time;
	
	HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);	
	
	SetTime(sTime.Hours, sTime.Minutes, sTime.Seconds);
	SetDate(sDate.Year, sDate.Month, sDate.Date, sDate.WeekDay);
}




/************************ (C) COPYRIGHT Mr.Lee *****END OF FILE****/
