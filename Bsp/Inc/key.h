/*
 * Copyright (c) Mr.Lee 2021 All rights reserved.
 * Description: 按键检测状态机头文件
 * Author: Mr.Lee
 * Create: 2021-02-19
 */

#ifndef __KEY_H
#define __KEY_H

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef enum
{
    MENU_KEY = 0,
    ENTER_KEY,
    EXIT_KEY,
    UP_KEY,
    DOWN_KEY,
    POWER_KEY,
    KEY_NUM
}KeyList;

typedef enum
{
    KEY_NULL_EVENT = 0,
    KEY_PRESS_EVENT,
    KEY_LONG_EVENT,
    KEY_RELEASE_EVENT
}KeyEvent;

typedef struct
{
	uint8_t keyShield; /* 按键屏蔽，0表示按键不使用，1表示使用 */
	uint8_t keyVirtuaPressLevel; /* 虚拟按键按下的电平，按下为1，未按下为0 */
	uint8_t keyRealPressLevel; /* 实际按下电平，根据实际电路配置 */
	uint8_t keyState; /* 就是我们说的按键状态了，它负责记录某一时刻按键状态 */
    uint8_t keyEvent; /* 表示按键事件，我这里分了3个事件，有按下、抬起和长按 */
    uint8_t (*ReadPinLevel)(void); /* 读取按键下电平状态，需要根据实际情况配置 */
    uint16_t keyDownCount; /* 按键计数器，记录实际按下时间时长*/
}KeyComponent;

extern KeyComponent g_keyComponent[KEY_NUM];


void KeyStateMachine(void);












#ifdef __cplusplus
}
#endif

#endif /* __KEY_H */

/************************ (C) COPYRIGHT Mr. Lee *****END OF FILE****/
