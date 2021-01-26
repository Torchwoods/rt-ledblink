# ledblink

## 1、介绍

与硬件无关的LED控制软件包，可以容易的控制LED灯开、关、翻转和各种闪烁；可同时用于RT-Thread标准板和Nano版本

### 1.1 目录结构

| 名称 | 说明 |
| ---- | ---- |
| docs  | 文档目录 |
| examples | 例子目录，并有相应的一些说明 |
| inc  | 头文件目录 |
| src  | 源代码目录 |

### 1.2 依赖

- RT-Thread 3.0+
- RT-Thread 4.0+

## 2、如何打开 ledblink

可以将c文件和h文件添加到工程代码中

## 3、使用ledblink

### 3.1 使用步骤

- 1、GPIO硬件初始化


- 2、实现HAL_TURN_ON_LEDx及HAL_TURN_OFF_LEDx宏定义


### 3.2 API介绍

- LED灯初始化状态 
 
 `void HalLedInit(void) `

默认将所有LED设置为关闭状态


- LED状态设置

`uint8_t HalLedSet( uint8_t led,uint8_t mode )`

| 参数 | 说明 |
| ---- | ---- |
| led  | LED序号 |
| mode | 模式|

	//LED序号
	#define HAL_LED_1     0x01
	#define HAL_LED_2     0x02
	#define HAL_LED_3     0x04
	#define HAL_LED_4     0x08

	//HalLedSet mode模式
    #define HAL_LED_MODE_OFF     0x00 //LED关闭
	#define HAL_LED_MODE_ON      0x01 //LED开启
	#define HAL_LED_MODE_TOGGLE  0x08 //LED翻转

- LED闪烁设置

`void HalLedBlink( uint8_t leds, uint8_t cnt, uint8_t duty, uint16_t time )`

| 参数 | 说明 |
| ---- | ---- |
| leds  | LED序号 |
| cnt | 闪烁次数，填0为无线循环|
| duty | 占空比，LED亮的时间|
| time | 闪烁周期，单位ms|


- LED状态更新

`void HalLedUpdate (void)`

## 4、注意事项

- 运行HalLedUpdate需要将需要的LED端口就行初始化
- 在h文件中实现HAL_TURN_ON_LEDx和HAL_TURN_OFF_LEDx宏逻辑，
- 当前默认控制4个LED灯，最多可以控制8个LED灯，开发者可以自行添加

## 5、联系方式 & 感谢

* 维护：ou
* 主页：https://github.com/RT-Thread-packages/hello
