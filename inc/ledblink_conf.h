/**************************************************************************
 * Filename:        lenblink_conf.h
 * Author:          Edward
 * E-Mail:          oxp@yystart.com
 * Description:     LED外设相关配置
 *
 * Copyright (C) 2016 YunYin Company - http://www.yystart.con/
 *
 * Version:         V1.0  (2020/3/24)    :   first version.
 *************************************************************************/
#ifndef __LED_BLINK_CONF_H__
#define __LED_BLINK_CONF_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include <stdint.h>
#include <rtthread.h>
#include <stm32f10x.h>

/*********************************************************************
 * CONSTANTS
 */
//使用的是STM32标准库
#define LED1_RCC	RCC_APB2Periph_GPIOA 
#define LED1_POART  GPIOA
#define LED1_PIN    GPIO_Pin_0

#define LED2_RCC	RCC_APB2Periph_GPIOA 
#define LED2_POART  GPIOA
#define LED2_PIN    GPIO_Pin_1

/*********************************************************************
 * MACROS
 */

#define st(x) do{x}while(0)

#define HAL_TURN_ON_LED1()  st(GPIO_ResetBits(LED1_POART, LED1_PIN);) 
#define HAL_TURN_ON_LED2()  st(GPIO_ResetBits(LED2_POART, LED2_PIN);) 
#define HAL_TURN_ON_LED3()   
#define HAL_TURN_ON_LED4()   

#define HAL_TURN_OFF_LED1()  st(GPIO_SetBits(LED1_POART, LED1_PIN);) 
#define HAL_TURN_OFF_LED2()  st(GPIO_SetBits(LED2_POART, LED2_PIN);) 
#define HAL_TURN_OFF_LED3()   
#define HAL_TURN_OFF_LED4()   
/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * VARIABLES
 */
 
    
/*********************************************************************
 * FUNCTIONS
 */
 
/*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* __ENCRYPT_H__ */

