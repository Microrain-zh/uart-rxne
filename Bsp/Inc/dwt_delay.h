
/*
 * Copyright (c) Mr.Lee 2021 All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2021-02-21
 */

#ifndef __DWT_DELAY_H
#define __DWT_DELAY_H

#include "stm32F4xx.h"


#ifdef __cplusplus
extern "C" {
#endif


void DelayInit(void);
void DelayU(uint32_t us);
void DelayM(uint32_t ms);
void DelayS(uint32_t s);
	






#ifdef __cplusplus
}
#endif

#endif /* __DWT_DELAY_H */

/************************ (C) COPYRIGHT Mr. Lee *****END OF FILE****/
