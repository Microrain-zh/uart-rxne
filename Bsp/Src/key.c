/*
 * Copyright (c) Mr.Lee. 2021. All rights reserved.
 * Description: 按键检测状态机
 * Author: Mr.Lee
 * Create: 2021-02-19
 */

#include "key.h"
#include "stdint.h"
#include "gpio.h"

typedef enum
{
	KEY_UP_STATE = 0, /* 未按下状态 */
	KEY_DOWN_JITTER_STATE, /* 按下消抖状态 */
	KEY_DOWN_STATE, /* 按下状态 */
	KEY_UP_JITTER_STATE, /* 释放消抖状态 */
	KEY_LONG_DOWN_STATE /* 长按状态 */
}KeyStatus;



static uint8_t KeyMenuReadPin(void);
static uint8_t KeyEnterReadPin(void);
static uint8_t KeyExitReadPin(void);
static uint8_t KeyUpReadPin(void);
static uint8_t KeyDownReadPin(void);
static uint8_t KeyPowerReadPin(void);
static void GetKeyLevel(void);


#define SCAN_PERIOD         10 //状态机10ms调度
#define LONG_PRESS_TIME     2000 //长按2s

/* 按键组件初始化，需要根据实际项目补充 */
KeyComponent g_keyComponent[KEY_NUM] = {
    {1,0,0,KEY_UP_STATE,KEY_NULL_EVENT,KeyMenuReadPin,0},
    {1,0,0,KEY_UP_STATE,KEY_NULL_EVENT,KeyEnterReadPin,0},
    {1,0,0,KEY_UP_STATE,KEY_NULL_EVENT,KeyExitReadPin,0},
    {0,0,0,KEY_UP_STATE,KEY_NULL_EVENT,KeyUpReadPin,0},
    {0,0,0,KEY_UP_STATE,KEY_NULL_EVENT,KeyDownReadPin,0},
    {0,0,0,KEY_UP_STATE,KEY_NULL_EVENT,KeyPowerReadPin,0},
};


// 按键IO初始化，需要补充完整
void KeyInit(void)
{
    // IO Init
    return;
}

// 获取MENU按键电平状态，需要补充完整
static uint8_t KeyMenuReadPin(void)
{
    // read io level
    return HAL_GPIO_ReadPin(GPIOE, KEY_0_Pin);
}

// 获取ENTER按键电平状态，需要补充完整
static uint8_t KeyEnterReadPin(void)
{
    // read io level
	return HAL_GPIO_ReadPin(GPIOE, KEY_1_Pin);
}

// 获取EXIT按键状态，需要补充完整
static uint8_t KeyExitReadPin(void)
{
    // read io level
    return HAL_GPIO_ReadPin(GPIOE, KEY_2_Pin);
}

// 获取UP按键电平状态，需要补充完整
static uint8_t KeyUpReadPin(void)
{
    // read io level
    return 0;
}

// 获取DOWN按键电平状态，需要补充完整
static uint8_t KeyDownReadPin(void)
{
    // read io level
    return 0;
}

// 获取POWER按键电平状态，需要补充完整
static uint8_t KeyPowerReadPin(void)
{
    // read io level
    return 0;
}




// 获取所有按键电平状态
static void GetKeyLevel(void)
{
    uint8_t i;

    for(i=0; i<KEY_NUM-3; i++)
    {
        if(g_keyComponent[i].keyShield == 0)
        {
            continue;
        }
        if(g_keyComponent[i].ReadPinLevel() == g_keyComponent[i].keyRealPressLevel)
        {
            g_keyComponent[i].keyVirtuaPressLevel = 1;
        }
        else
        {
            g_keyComponent[i].keyVirtuaPressLevel = 0;
        }
    }
}

/*
 * 按键状态机，空闲，按下抖动，按下，弹起抖动，长按5个状态
 * 需要10ms进行一次调度
 * 注意，此处只产生释放和长按事件，需要在按键处理中及时处理事件并清除事件
 */
void KeyStateMachine(void)
{
    uint8_t i;

    GetKeyLevel();

    for(i=0; i<KEY_NUM-3; i++)
    {
        switch (g_keyComponent[i].keyState)
        {
            case KEY_UP_STATE:
            {
                if(g_keyComponent[i].keyVirtuaPressLevel == 1)
                {
                    g_keyComponent[i].keyState = KEY_DOWN_JITTER_STATE;
                }
                else
                {
                    g_keyComponent[i].keyState = KEY_UP_STATE;
                }
                break;
            }
            case KEY_DOWN_JITTER_STATE:
            {
                if(g_keyComponent[i].keyVirtuaPressLevel == 1)
                {
                    g_keyComponent[i].keyState = KEY_DOWN_STATE;
                    g_keyComponent[i].keyDownCount = 0;
                }
                else
                {
                    g_keyComponent[i].keyState = KEY_UP_STATE; //抖动
                }
                break;
            }
            case KEY_DOWN_STATE:
            {
                if(g_keyComponent[i].keyVirtuaPressLevel == 1)
                {
                    g_keyComponent[i].keyDownCount += SCAN_PERIOD;
                    if(g_keyComponent[i].keyDownCount>LONG_PRESS_TIME)
                    {
                        g_keyComponent[i].keyEvent = KEY_LONG_EVENT;
                        g_keyComponent[i].keyState = KEY_LONG_DOWN_STATE;
                    }
                    else
                    {
                        g_keyComponent[i].keyState = KEY_DOWN_STATE;
                    }
                }
                else
                {
                    g_keyComponent[i].keyState = KEY_UP_JITTER_STATE;
                }
                break;
            }
            case KEY_UP_JITTER_STATE:
            {
                if(g_keyComponent[i].keyVirtuaPressLevel == 1)
                {
                    g_keyComponent[i].keyDownCount += SCAN_PERIOD;
                    g_keyComponent[i].keyState = KEY_DOWN_STATE;
                }
                else
                {
                    g_keyComponent[i].keyEvent = KEY_RELEASE_EVENT;
                    g_keyComponent[i].keyState = KEY_UP_STATE;
                }
                break;
            }
            case KEY_LONG_DOWN_STATE:
            {
                if(g_keyComponent[i].keyVirtuaPressLevel == 1)
                {
                    g_keyComponent[i].keyDownCount += SCAN_PERIOD;
                    g_keyComponent[i].keyState = KEY_LONG_DOWN_STATE;
                }
                else
                {
                    g_keyComponent[i].keyState = KEY_UP_JITTER_STATE;
                }
                break;
            }
            
        }
    }
}







/************************ (C) COPYRIGHT Mr.Lee *****END OF FILE****/
