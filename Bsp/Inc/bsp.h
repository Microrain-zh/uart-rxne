/*
 * Copyright (c) Mr.Lee 2021 All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2021-02-19
 */

#ifndef __BSP_H
#define __BSP_H

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

extern volatile uint32_t g_RefreshCnt;

void BspInit(void);

void RefreshInfo(int temp, float humi, float press);


#ifdef __cplusplus
}
#endif

#endif /* __BSP_H */

/************************ (C) COPYRIGHT Mr. Lee *****END OF FILE****/
