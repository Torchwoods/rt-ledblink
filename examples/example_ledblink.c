#include <rtthread.h>
#include <finsh.h>

#include <ledblink.h>

/**************************************************************************
 * Filename:        led_task.c
 * Author:          Edward
 * E-Mail:          oxp@yystart.com
 * Description:     LED任务
 *
 * Copyright (C) 2016 YunYin Company - http://www.yystart.con/
 * 
 * Version:         V1.0  (2020/3/24)    :   first version.
 *************************************************************************/


/*********************************************************************
* INCLUDES
*/
#include <stm32f10x.h>
#include <rtthread.h>
#include <ledblink.h>
/*********************************************************************
* MACROS
*/

/*********************************************************************
* CONSTANTS
*/

/*********************************************************************
* TYPEDEFS
*/

/*********************************************************************
* GLOBAL VARIABLES
*/

       
/*********************************************************************
* LOCAL VARIABLES
*/


/*********************************************************************
* LOCAL FUNCTIONS
*/


/*********************************************************************
* GLOBAL FUNCTIONS
*/

/*********************************************************************
* @fn          funtion_name
*
* @brief       Add device to descovery list.
*
* @param       pSimpleDescRsp - SimpleDescRsp containing epInfo of new EP.
*
* @return      index of device or 0xFF if no room in list
*/

static void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

static void led_thread_entry(void* parameter)
{
    rt_uint32_t delays = 0;
    GPIO_Configuration();
    HalLedInit();
    //HalLedSet( HAL_LED_1, HAL_LED_MODE_FLASH );
    HalLedSet( HAL_LED_2, HAL_LED_MODE_ON );
    HalLedBlink(HAL_LED_1,0,50,2000);
    for (;;)
    {
       HalLedUpdate();
    }
}

void led_task_init(void)
{
  	static rt_uint8_t s_led_stack[256];
  	static struct rt_thread led_thread;
    rt_err_t result;

    result = rt_thread_init(&led_thread,
                            "led",
                            led_thread_entry,
                            RT_NULL,
                            (rt_uint8_t*)&s_led_stack[0],
                            sizeof(s_led_stack),
                            7,
                            5);
    if (result == RT_EOK)
    {
        rt_thread_startup(&led_thread);
    }
}

/*********************************************************************/

