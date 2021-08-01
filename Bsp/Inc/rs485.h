/*
 * Copyright (c) Mr.Lee 2021 All rights reserved.
 * Description: �������״̬��ͷ�ļ�
 * Author: Mr.Lee
 * Create: 2021-05-26
 */

#ifndef __RS485_H
#define __RS485_H



#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push)//�Ѷ��뷽ʽѹջ
#pragma pack(1) //��1���ֽڶ��뷽ʽ



typedef struct
{
	int temperture;
}ProtocolParameterType;




#pragma pack(pop)//�ָ����뷽ʽ



extern ProtocolParameterType g_protocolParameter;

void GetTemperature(void);
void SendGetTempRequest(void);

#ifdef __cplusplus
}
#endif

#endif /* __RS485_H */

/************************ (C) COPYRIGHT Mr. Lee *****END OF FILE****/
