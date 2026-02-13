#include "stm32f10x.h"                  // Device header

void PWM_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	TIM_InternalClockConfig(TIM2);//开启后默认是内部时钟，但写一下更稳妥。
	
	//设置定时器基本功能： 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//输入滤波的时钟分频，TIM_CKD_DIV1代表分频系数为1
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//CNT计数器的计数模式
	TIM_TimeBaseInitStruct.TIM_Period = 99;//ARR的自动重装值,比实际目标值小1.
	TIM_TimeBaseInitStruct.TIM_Prescaler = 719;//分频器的值，比分频系数小1.
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;//重复计数器的值（高级计时器才有）
	TIM_TimeBaseInit(TIM2 , &TIM_TimeBaseInitStruct);//为了让PSC和ARR的值立刻生效会马上触发一次更新事件,顺带着会触发更新中断
	
    TIM_OCInitTypeDef TIM_OCInitStruct;
    TIM_OCStructInit(&TIM_OCInitStruct);
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_Pulse = 50;
	TIM_OC3Init(TIM2, &TIM_OCInitStruct);
	
	//使能CNT计数器
	TIM_Cmd(TIM2, ENABLE);
	
}

void PWM_SetCompare1(uint16_t Compare)
{
    TIM_SetCompare1(TIM2, Compare);
}

void PWM_SetCompare2(uint16_t Compare)
{
    TIM_SetCompare2(TIM2, Compare);
}

void PWM_SetCompare3(uint16_t Compare)
{
    TIM_SetCompare3(TIM2, Compare);
}

void PWM_SetPrescaler(uint16_t Prescaler)
{
	TIM_PrescalerConfig(TIM2, Prescaler, TIM_PSCReloadMode_Immediate);
}
