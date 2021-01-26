/**************************************************************************
 * Filename:        lenblink.h
 * Author:          Edward
 * E-Mail:          oxp@yystart.com
 * Description:     LEDÇý¶¯
 *
 * Copyright (C) 2016 YunYin Company - http://www.yystart.con/
 *
 * Version:         V1.0  (2020/3/24)    :   first version.
 *************************************************************************/
#ifndef __LED_BLINK_H__
#define __LED_BLINK_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include <ledblink_conf.h>
/*********************************************************************
 * CONSTANTS
 */
 
/* LEDS - The LED number is the same as the bit position */
#define HAL_LED_1     0x01
#define HAL_LED_2     0x02
#define HAL_LED_3     0x04
#define HAL_LED_4     0x08
  
//#define HAL_LED_5     0x10
//#define HAL_LED_6     0x20
//#define HAL_LED_7     0x40
//#define HAL_LED_8     0x80

#define HAL_LED_ALL   (HAL_LED_1 | HAL_LED_2 | HAL_LED_3 | HAL_LED_4)

/* Modes */
#define HAL_LED_MODE_OFF     0x00
#define HAL_LED_MODE_ON      0x01
#define HAL_LED_MODE_BLINK   0x02
#define HAL_LED_MODE_FLASH   0x04
#define HAL_LED_MODE_TOGGLE  0x08

/* Defaults */
#define HAL_LED_DEFAULT_MAX_LEDS      4
#define HAL_LED_DEFAULT_DUTY_CYCLE    50
#define HAL_LED_DEFAULT_FLASH_COUNT   50
#define HAL_LED_DEFAULT_FLASH_TIME    1000

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * VARIABLES
 */
 
    
/*********************************************************************
 * FUNCTIONS
 */
 
/*
 * Initialize LED Service.
 */
extern void HalLedInit( void );

/*
 * Set the LED ON/OFF/TOGGLE.
 */
extern uint8_t HalLedSet( uint8_t led,uint8_t mode );

/*
 * Blink the LED.
 */
extern void HalLedBlink( uint8_t leds, uint8_t cnt, uint8_t duty, uint16_t time );


extern void HalLedUpdate (void);
/*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* __ENCRYPT_H__ */

