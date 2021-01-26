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

/*********************************************************************
* INCLUDES
*/
#include <stdint.h>
#include <rthw.h>
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

/* LED control structure */
typedef struct {
  uint8_t mode;       /* Operation mode */
  uint8_t todo;       /* Blink cycles left */
  uint8_t onPct;      /* On cycle percentage */
  uint32_t time;      /* On/off cycle time (msec) */
  uint32_t next;      /* Time for next change */
} HalLedStatus_t;

/*********************************************************************
* GLOBAL VARIABLES
*/

static uint8_t HalLedState;              // LED state at last set/clr/blink update
static uint8_t preBlinkState;            // Original State before going to blink mode
                                            // bit 0, 1, 2, 3 represent led 0, 1, 2, 3

HalLedStatus_t HalLedControlTable[HAL_LED_DEFAULT_MAX_LEDS];
/*********************************************************************
* LOCAL VARIABLES
*/


/*********************************************************************
* LOCAL FUNCTIONS
*/


/*********************************************************************
* GLOBAL FUNCTIONS
*/

void HalLedOnOff(uint8_t leds, uint8_t mode);

/***************************************************************************************************
 * @fn      HalLedInit
 *
 * @brief   Initialize LED Service
 *
 * @param   init - pointer to void that contains the initialized value
 *
 * @return  None
 ***************************************************************************************************/
void HalLedInit (void)
{
  /* Initialize all LEDs to OFF */
  HalLedSet (HAL_LED_ALL, HAL_LED_MODE_OFF);
}

/***************************************************************************************************
 * @fn      HalLedSet
 *
 * @brief   Tun ON/OFF/TOGGLE given LEDs
 *
 * @param   led - bit mask value of leds to be turned ON/OFF/TOGGLE
 *          mode - BLINK, FLASH, TOGGLE, ON, OFF
 * @return  None
 ***************************************************************************************************/
uint8_t HalLedSet (uint8_t leds, uint8_t mode)
{
  uint8_t led;
  HalLedStatus_t *sts;

  switch (mode)
  {
    case HAL_LED_MODE_BLINK:
      /* Default blink, 1 time, D% duty cycle */
      HalLedBlink (leds, 1, HAL_LED_DEFAULT_DUTY_CYCLE, HAL_LED_DEFAULT_FLASH_TIME);
      break;

    case HAL_LED_MODE_FLASH:
      /* Default flash, N times, D% duty cycle */
      HalLedBlink (leds, HAL_LED_DEFAULT_FLASH_COUNT, HAL_LED_DEFAULT_DUTY_CYCLE, HAL_LED_DEFAULT_FLASH_TIME);
      break;

    case HAL_LED_MODE_ON:
    case HAL_LED_MODE_OFF:
    case HAL_LED_MODE_TOGGLE:

      led = HAL_LED_1;
      leds &= HAL_LED_ALL;
      sts = HalLedControlTable;

      while (leds)
      {
        if (leds & led)
        {
          if (mode != HAL_LED_MODE_TOGGLE)
          {
            sts->mode = mode;  /* ON or OFF */
          }
          else
          {
            sts->mode ^= HAL_LED_MODE_ON;  /* Toggle */
          }
          HalLedOnOff (led, sts->mode);
          leds ^= led;
        }
        led <<= 1;
        sts++;
      }
      break;

    default:
      break;
  }
  return ( HalLedState );
}

/***************************************************************************************************
 * @fn      HalLedBlink
 *
 * @brief   Blink the leds
 *
 * @param   leds       - bit mask value of leds to be blinked
 *          numBlinks  - number of blinks
 *          percent    - the percentage in each period where the led
 *                       will be on
 *          period     - length of each cycle in milliseconds
 *
 * @return  None
 ***************************************************************************************************/
void HalLedBlink (uint8_t leds, uint8_t numBlinks, uint8_t percent, uint16_t period)
{
  uint8_t led;
  HalLedStatus_t *sts;

  if (leds && percent && period)
  {
    if (percent < 100)
    {
      led = HAL_LED_1;
      leds &= HAL_LED_ALL;
      sts = HalLedControlTable;

      while (leds)
      {
        if (leds & led)
        {
          /* Store the current state of the led before going to blinking if not already blinking */
          if(sts->mode < HAL_LED_MODE_BLINK )
          	preBlinkState |= (led & HalLedState);

          sts->mode  = HAL_LED_MODE_OFF;                    /* Stop previous blink */
          sts->time  = rt_tick_from_millisecond(period);    /* Time for one on/off cycle */
          sts->onPct = percent;                             /* % of cycle LED is on */
          sts->todo  = numBlinks;                           /* Number of blink cycles */
          if (!numBlinks) sts->mode |= HAL_LED_MODE_FLASH;  /* Continuous */
          sts->next = rt_tick_get();                        /* Start now */
          sts->mode |= HAL_LED_MODE_BLINK;                  /* Enable blinking */
          leds ^= led;
        }
        led <<= 1;
        sts++;
      }
    }
    else
    {
      HalLedSet (leds, HAL_LED_MODE_ON);                    /* >= 100%, turn on */
    }
  }
  else
  {
    HalLedSet (leds, HAL_LED_MODE_OFF);                     /* No on time, turn off */
  }
}

/***************************************************************************************************
 * @fn      HalLedUpdate
 *
 * @brief   Update leds to work with blink
 *
 * @param   none
 *
 * @return  none
 ***************************************************************************************************/
void HalLedUpdate (void)
{
  uint8_t led;
  uint8_t pct;
  uint8_t leds;
  HalLedStatus_t *sts;
  uint32_t time;
  uint32_t next;
  uint32_t wait;

  next = 0;
  led  = HAL_LED_1;
  leds = HAL_LED_ALL;
  sts = HalLedControlTable;

  while (leds)
  {
    if (leds & led)
    {
      if (sts->mode & HAL_LED_MODE_BLINK)
      {
        time = rt_tick_get();
        if (time >= sts->next)
        {
          if (sts->mode & HAL_LED_MODE_ON)
          {
            pct = 100 - sts->onPct;               /* Percentage of cycle for off */
            sts->mode &= ~HAL_LED_MODE_ON;        /* Say it's not on */
            HalLedOnOff (led, HAL_LED_MODE_OFF);  /* Turn it off */

            if (!(sts->mode & HAL_LED_MODE_FLASH))
            {
              sts->todo--;                        /* Not continuous, reduce count */
            }
          }            
          else if ( (!sts->todo) && !(sts->mode & HAL_LED_MODE_FLASH) )
          {
            sts->mode ^= HAL_LED_MODE_BLINK;      /* No more blinks */  
          }            
          else
          {
            pct = sts->onPct;                     /* Percentage of cycle for on */
            sts->mode |= HAL_LED_MODE_ON;         /* Say it's on */
            HalLedOnOff (led, HAL_LED_MODE_ON);   /* Turn it on */
          }
          
          
          if (sts->mode & HAL_LED_MODE_BLINK)
          {
            wait = (((uint32_t)pct * (uint32_t)sts->time) / 100);
            sts->next = time + wait;
            //rt_kprintf("sts->time:%d,pct:%d,next:%d,time:%d,wait:%d\n",sts->time,pct,sts->next,time,wait);
          }
          else
          {
            /* no more blink, no more wait */
            wait = 0;
            /* After blinking, set the LED back to the state before it blinks */
            HalLedSet (led, ((preBlinkState & led)!=0)?HAL_LED_MODE_ON:HAL_LED_MODE_OFF);
            /* Clear the saved bit */
            preBlinkState &= (led ^ 0xFF);
          }
        }
        else
        {
          wait = sts->next - time;  /* Time left */
        }

        if (!next || ( wait && (wait < next) ))
        {
          next = wait;
        }
      }
      leds ^= led;
    }
    led <<= 1;
    sts++;
  }
  
  if(next)
    rt_thread_delay(next);
  else
    rt_thread_mdelay(100);
}

/***************************************************************************************************
 * @fn      HalLedOnOff
 *
 * @brief   Turns specified LED ON or OFF
 *
 * @param   leds - LED bit mask
 *          mode - LED_ON,LED_OFF,
 *
 * @return  none
 ***************************************************************************************************/
void HalLedOnOff (uint8_t leds, uint8_t mode)
{
	if (leds & HAL_LED_1)
	{
		if (mode == HAL_LED_MODE_ON)
		{
			HAL_TURN_ON_LED1();
		}
		else
		{
			HAL_TURN_OFF_LED1();
		}
	}

	if (leds & HAL_LED_2)
	{
		if (mode == HAL_LED_MODE_ON)
		{
		  HAL_TURN_ON_LED2();
		}
		else
		{
		  HAL_TURN_OFF_LED2();
		}
	}

	if (leds & HAL_LED_3)
	{
		if (mode == HAL_LED_MODE_ON)
		{
		  HAL_TURN_ON_LED3();
		}
		else
		{
		  HAL_TURN_OFF_LED3();
		}
	}

	if (leds & HAL_LED_4)
	{
		if (mode == HAL_LED_MODE_ON)
		{
		  HAL_TURN_ON_LED4();
		}
		else
		{
		  HAL_TURN_OFF_LED4();
		}
	}

	/* Remember current state */
	if (mode)
	{
		HalLedState |= leds;
	}
	else
	{
		HalLedState &= (leds ^ 0xFF);
	}
}

/***************************************************************************************************
 * @fn      HalGetLedState
 *
 * @brief   Dim LED2 - Dim (set level) of LED2
 *
 * @param   none
 *
 * @return  led state
 ***************************************************************************************************/
uint8_t HalLedGetState(void)
{
  return HalLedState;
}
