/*
 * Copyright (c) Mr.Lee 2021 All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2021-02-21
 */
 
 
#include "stm32f4xx_hal.h"


#ifndef __SIMULATE_I2C_H
#define __SIMULATE_I2C_H

#ifdef __cplusplus
extern "C" {
#endif


#define SCL_PORT			GPIOD
#define SCL_PIN 			GPIO_PIN_6
#define SDA_PORT 			GPIOD
#define SDA_PIN 			GPIO_PIN_7
#define SLAVE_ADDRESS		0x00				
#define	DELAY_US			20


#define SIMULATE_I2C_SCL_SET		(HAL_GPIO_WritePin(pSimulateI2cPort->sclPort, pSimulateI2cPort->sclPin, GPIO_PIN_SET))
#define SIMULATE_I2C_SCL_RESET		(HAL_GPIO_WritePin(pSimulateI2cPort->sclPort, pSimulateI2cPort->sclPin, GPIO_PIN_RESET))
#define SIMULATE_I2C_SDA_SET		(HAL_GPIO_WritePin(pSimulateI2cPort->sdaPort, pSimulateI2cPort->sdaPin, GPIO_PIN_SET))
#define SIMULATE_I2C_SDA_RESET		(HAL_GPIO_WritePin(pSimulateI2cPort->sdaPort, pSimulateI2cPort->sdaPin, GPIO_PIN_RESET))
#define SIMULATE_I2C_READ			(HAL_GPIO_ReadPin(pSimulateI2cPort->sdaPort, pSimulateI2cPort->sdaPin))

#define I2C_ACK                   	0                        //应,答
#define I2C_NOACK                 	1                        //非应答


typedef struct
{
    GPIO_TypeDef *sclPort;
    uint32_t sclPin;
    GPIO_TypeDef *sdaPort;
    uint32_t sdaPin;
    uint8_t address;
}SimulateI2cPortType;


extern SimulateI2cPortType dcmiI2cPort;


void SimulateI2cInit(void);
void SimulateI2cStart(SimulateI2cPortType *pSimulateI2cPort);
void SimulateI2cStop(SimulateI2cPortType *pSimulateI2cPort);
void SimulateI2cOutAck(SimulateI2cPortType *pSimulateI2cPort, uint8_t ack);
uint8_t SimulateI2cGetAck(SimulateI2cPortType *pSimulateI2cPort);
uint8_t I2cWriteByte(SimulateI2cPortType *pSimulateI2cPort, uint8_t data);
uint8_t I2cReadByte(SimulateI2cPortType *pSimulateI2cPort, uint8_t ack);

void ConfigSclIn(SimulateI2cPortType *pSimulateI2cPort);
void ConfigSclOut(SimulateI2cPortType *pSimulateI2cPort);
void ConfigSdaIn(SimulateI2cPortType *pSimulateI2cPort);
void ConfigSdaOut(SimulateI2cPortType *pSimulateI2cPort);
#ifdef __cplusplus
}
#endif

#endif /* __SIMULATE_I2C_H */

/************************ (C) COPYRIGHT Mr. Lee *****END OF FILE****/
