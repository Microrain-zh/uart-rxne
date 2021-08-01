/*
 * Copyright (c) Mr.Lee. 2021. All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2021-05-26
 */

#include "rs485.h"
#include "stdint.h"
#include "mbcrc.h"
#include "stm32f4xx_hal.h"
#include "usart.h"
#include "uart_device.h"
#include "gpio.h"
#include <stdio.h>

#pragma pack(push)//把对齐方式压栈
#pragma pack(1) //按1个字节对齐方式

typedef struct
{
	uint8_t deviceAddress;
	uint8_t functionCode;
	uint8_t regAddrH;
	uint8_t regAddrL;
	uint8_t quantRegH;
	uint8_t quantRegL;
	uint8_t crc16L;
	uint8_t crc16H;	
}RequestType;

typedef struct
{
	uint8_t deviceAddress;
	uint8_t functionCode;
	uint8_t byteCount;
	uint8_t reg16Data0H;
	uint8_t reg16Data0L;
	uint8_t reg16Data1H;
	uint8_t reg16Data1L;
	uint8_t crc16L;
	uint8_t crc16H;
}ResponseType;

typedef union
{
	uint8_t reg16Data[4];
	float temperture;
}ResponseDataType;




#pragma pack(pop)//恢复对齐方式


ProtocolParameterType g_protocolParameter;

static void AnalyseMessage(ResponseType *pData);


void GetTemperature(void)
{
	ResponseType response = {0};
	uint8_t *p = &response.deviceAddress;
	uint16_t tmp = 0;
	
//	tmp = Uart1Read(p, 1);
	tmp = Uart1Read(p, 9);	
	DEBUG("%u\r\n",tmp);
	DEBUG("%02x %02x %02x %02x %02x %02x %02x %02x %02x\r\n",
	*p,*(p+1),*(p+2),*(p+3),*(p+4),*(p+5),*(p+6),*(p+7),*(p+8));
//	tmp = 0;
//	while (tmp != 0)
//	{
//		p++;
//		DEBUG("%02X\r\n",(uint8_t)(*(p-1)));
//		if (p >=  &response.deviceAddress + sizeof(ResponseType))
//		{
//			p = &response.deviceAddress;			
//			AnalyseMessage((ResponseType *)(p));
//		}		
//		tmp = Uart1Read(p, 1);
//	};
}

static void AnalyseMessage(ResponseType *pData)
{
	ResponseDataType ResponseData;
	
	if ((pData->deviceAddress == 0x01) && (pData->functionCode == 0x03))
	{
		if (pData->byteCount == 0x04)
		{
//			ResponseData.reg16Data0H = pData->reg16Data0H;
//			ResponseData.reg16Data0L = pData->reg16Data0L;
//			ResponseData.reg16Data1H = pData->reg16Data1H;
//			ResponseData.reg16Data1L = pData->reg16Data1L;
			ResponseData.reg16Data[0] =  pData->reg16Data0L;
			ResponseData.reg16Data[1] =  pData->reg16Data0H;
			ResponseData.reg16Data[2] =  pData->reg16Data1L;
			ResponseData.reg16Data[3] =  pData->reg16Data1H;
//			
//			sprintf((char*)&g_protocolParameter.temperture, "%.1f", ResponseData.temperture);
			g_protocolParameter.temperture = ResponseData.temperture;
			DEBUG("temperture：%u\r\n",g_protocolParameter.temperture);
			
		}
	}
}	


void SendGetTempRequest(void)
{
	RequestType request;
	uint16_t crcTemp; 
	uint16_t dataLen;
	
//	HAL_GPIO_WritePin(GPIOB, PK_485_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, GPIO_PIN_SET);
	request.deviceAddress = 0x01;
	request.functionCode = 0x03;
	request.regAddrH = 0x04;
	request.regAddrL = 0x00;
	request.quantRegH = 0x00;
	request.quantRegL = 0x02;
	
	dataLen = sizeof(RequestType) - sizeof(request.crc16H) - sizeof(request.crc16L);
	crcTemp = usMBCRC16((uint8_t *)&request.deviceAddress, dataLen);
	
	request.crc16L = crcTemp & 0xff;
	request.crc16H = (crcTemp>>8) & 0xff;
	
	HAL_UART_Transmit(&huart3, (uint8_t *)&request, sizeof(RequestType),0xff);
//	HAL_GPIO_WritePin(GPIOB, PK_485_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, GPIO_PIN_RESET);
}















/************************ (C) COPYRIGHT Mr.Lee *****END OF FILE****/

