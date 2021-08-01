/*
 * Copyright (c) Mr.Lee. 2021. All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2021-03-21
 */

#include "simulate_i2c.h"
#include "dwt_delay.h"



SimulateI2cPortType dcmiI2cPort;

void SCCB_Delay(void)
{
	DelayU(5);
}

void SimulateI2cInit(void)
{
    dcmiI2cPort.sclPort = SCL_PORT;
    dcmiI2cPort.sclPin = SCL_PIN;
    dcmiI2cPort.sdaPort = SCL_PORT;
    dcmiI2cPort.sdaPin = SDA_PIN;
    dcmiI2cPort.address = SLAVE_ADDRESS;
	
	ConfigSclOut(&dcmiI2cPort);
	ConfigSdaOut(&dcmiI2cPort);
}



// 配置SCL输入模式，多主使用
void ConfigSclIn(SimulateI2cPortType *pSimulateI2cPort)
{
    GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.Pin = pSimulateI2cPort->sclPin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(pSimulateI2cPort->sclPort, &GPIO_InitStruct);	
}

// 配置SCL输出模式
void ConfigSclOut(SimulateI2cPortType *pSimulateI2cPort)
{
    GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.Pin = pSimulateI2cPort->sclPin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(pSimulateI2cPort->sclPort, &GPIO_InitStruct);	
}

// 配置SDA输入模式
void ConfigSdaIn(SimulateI2cPortType *pSimulateI2cPort)
{
    GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.Pin = pSimulateI2cPort->sdaPin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(pSimulateI2cPort->sdaPort, &GPIO_InitStruct);	
}

// 配置SDA输出模式
void ConfigSdaOut(SimulateI2cPortType *pSimulateI2cPort)
{
    GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.Pin = pSimulateI2cPort->sdaPin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(pSimulateI2cPort->sdaPort, &GPIO_InitStruct);	
}


void SimulateI2cStart(SimulateI2cPortType *pSimulateI2cPort)
{
	SIMULATE_I2C_SCL_SET;
	SCCB_Delay();
	SIMULATE_I2C_SDA_SET;
	SCCB_Delay();
	
	SIMULATE_I2C_SDA_RESET;
	SCCB_Delay();
	SIMULATE_I2C_SCL_RESET; // 待写
	SCCB_Delay();
}

void SimulateI2cStop(SimulateI2cPortType *pSimulateI2cPort)
{
	SIMULATE_I2C_SDA_RESET;
	SCCB_Delay();

	SIMULATE_I2C_SCL_SET;
	SCCB_Delay();
	SIMULATE_I2C_SDA_SET;
	SCCB_Delay();
}

/*
功    能 ： I2C主机产生应答(或非应答)位
参    数 ： I2C_ACK ----- 应答
            I2C_NOACK --- 非应答
返 回 值 ： 无
*/
void SimulateI2cOutAck(SimulateI2cPortType *pSimulateI2cPort, uint8_t ack)
{
	SIMULATE_I2C_SCL_RESET;
	SCCB_Delay();

	if (ack == I2C_ACK)
	{
		SIMULATE_I2C_SDA_RESET;
	}
	else 
	{
		SIMULATE_I2C_SDA_SET;
	}
	SCCB_Delay();	
	SIMULATE_I2C_SCL_SET;
	SCCB_Delay();
	SIMULATE_I2C_SCL_RESET; // 待写
	SCCB_Delay();
}

/*
功    能 ： I2C主机读取应答(或非应答)位
参    数 ： 无
返 回 值 ： I2C_ACK ----- 应答
            I2C_NOACK --- 非应答
*/
uint8_t SimulateI2cGetAck(SimulateI2cPortType *pSimulateI2cPort)
{
	uint8_t ack;
	
	SIMULATE_I2C_SCL_RESET; 
	SCCB_Delay();	
	
	ConfigSdaIn(pSimulateI2cPort);

	SIMULATE_I2C_SCL_SET;
	SCCB_Delay();

	if (SIMULATE_I2C_READ)
	{
		ack = I2C_NOACK;
	}
	else
	{
		ack = I2C_ACK;
	}
	
	SIMULATE_I2C_SCL_RESET;
	SCCB_Delay();

	ConfigSdaOut(pSimulateI2cPort);
		
	return ack;
}

/*
功    能 ： I2C写一字节
参    数 ： Data -------- 数据
返 回 值 ： I2C_ACK ----- 应答
            I2C_NOACK --- 非应答
*/
uint8_t I2cWriteByte(SimulateI2cPortType *pSimulateI2cPort, uint8_t data)
{
    uint8_t i;
	
	for (i=0; i<8; i++)
	{
		SIMULATE_I2C_SCL_RESET;
		SCCB_Delay();

		if (data & 0x80)
		{
			SIMULATE_I2C_SDA_SET;
		}
		else
		{
			SIMULATE_I2C_SDA_RESET;
		}
		data <<= 1;
		SCCB_Delay();

		SIMULATE_I2C_SCL_SET;
		SCCB_Delay();		
	}
	SIMULATE_I2C_SCL_RESET;
	SCCB_Delay();
	
	return SimulateI2cGetAck(pSimulateI2cPort);
}

/*
功    能 ： I2C读一字节
参    数 ： ack --------- 产生应答(或者非应答)位
返 回 值 ： data -------- 读取的一字节数据
*/
uint8_t I2cReadByte(SimulateI2cPortType *pSimulateI2cPort, uint8_t ack)
{
	uint8_t i;
	uint8_t ret;

	SIMULATE_I2C_SCL_RESET;
	SCCB_Delay();
	
	ConfigSdaIn(pSimulateI2cPort);

	for (i=0; i<8; i++)
	{
		SIMULATE_I2C_SCL_SET;
		SCCB_Delay();
		ret <<= 1;
		
		if (SIMULATE_I2C_READ)
		{
			ret |= 0x01;
		}
		SIMULATE_I2C_SCL_RESET;
		SCCB_Delay();		
	}
	
	ConfigSdaOut(pSimulateI2cPort);
	SimulateI2cOutAck(pSimulateI2cPort, ack);
	
	return ret;
}




/************************ (C) COPYRIGHT Mr.Lee *****END OF FILE****/
