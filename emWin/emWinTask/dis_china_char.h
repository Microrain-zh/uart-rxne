/*
 * Copyright (c) Mr.Lee 2021 All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2021-02-19
 */

#ifndef __DIS_CHINA_CHAR_H
#define __DIS_CHINA_CHAR_H

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif



void DispTNCompany(void);

void RefreshTemperature(const char *str);
void RefreshHumidity(const char *str);
void RefreshPressure(const char *str);
void RefreshRTC(const char *str);

#ifdef __cplusplus
}
#endif

#endif /* __DIS_CHINA_CHAR_H */

/************************ (C) COPYRIGHT Mr. Lee *****END OF FILE****/


