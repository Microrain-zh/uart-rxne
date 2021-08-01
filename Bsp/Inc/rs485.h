/*
 * Copyright (c) Mr.Lee 2021 All rights reserved.
 * Description: 按键检测状态机头文件
 * Author: Mr.Lee
 * Create: 2021-05-26
 */

#ifndef __RS485_H
#define __RS485_H



#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push)//把对齐方式压栈
#pragma pack(1) //按1个字节对齐方式



typedef struct
{
	int temperture;
}ProtocolParameterType;




#pragma pack(pop)//恢复对齐方式



extern ProtocolParameterType g_protocolParameter;

void GetTemperature(void);
void SendGetTempRequest(void);

#ifdef __cplusplus
}
#endif

#endif /* __RS485_H */

/************************ (C) COPYRIGHT Mr. Lee *****END OF FILE****/
