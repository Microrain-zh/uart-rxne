/*
 * Copyright (c) Mr.Lee. 2021. All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2021-03-21
 */
 
 #include "dis_china_char.h"
 #include "GUI.h"
 
void DispTNCompany(void)
{
	GUI_SetColor(GUI_BLUE);
	GUI_SetBkColor(GUI_WHITE);	
	GUI_Clear();
	GUI_UC_SetEncodeUTF8();
	GUI_SetFont(&GUI_Fonttn16);
	GUI_DispStringAt("西安托尼智能科技有限公司", 180, 390);
	GUI_DispStringAt("当前温度", 52, 50);
	GUI_DispStringAt("当前湿度", 52, 150);
	GUI_DispStringAt("当前时间", 52, 250);	
	GUI_SetFont(&GUI_Fonttndz);
	GUI_DispStringAt("地址：西安二环南路100号金叶家园D座1003", 180, 430);
	GUI_DispStringAt("电话：13891809912", 180, 450);
	GUI_SetFont(&GUI_Font24_ASCII);
}



void RefreshTemperature(const char *str)
{
	GUI_DispStringAt("                ", 52, 85);	
	GUI_DispStringAt(str, 52, 85);	
}

void RefreshHumidity(const char *str)
{
	GUI_DispStringAt(str, 52,185);	
}

void RefreshPressure(const char *str)
{
	GUI_DispStringAt(str, 52, 285);	
}

void RefreshRTC(const char *str)
{
	GUI_DispStringAt(str, 15, 285);	
}



/************************ (C) COPYRIGHT Mr.Lee *****END OF FILE****/
