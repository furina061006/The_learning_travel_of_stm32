#include "stm32f10x.h"                  // Device header
#include "Delay.h"
/**
  * @note 使用这个函数,必须要求按钮一端接的是正极而非负极
  */
void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//下拉输出模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

}
/**
  * @note 这里有个延迟消抖的致命问题,如果两个按钮同时按且放手,系统只会检测到第一个按钮的变化
  */
uint8_t Key_GetNum(void)
{
	uint8_t KeyNum = 0;
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1))
	{
		Delay_ms(20);//下沿延迟消抖
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1));//按着的时候一直循环
		Delay_ms(20);//上沿延迟消抖
		KeyNum = 1;
	}
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11))
	{
		Delay_ms(20);//下沿延迟消抖
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11));//按着的时候一直循环
		Delay_ms(20);//上沿延迟消抖
		KeyNum = 2;
	}
	return KeyNum;
}