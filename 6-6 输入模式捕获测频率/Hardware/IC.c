#include "stm32f10x.h"

void IC_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	TIM_InternalClockConfig(TIM3);//开启后默认是内部时钟，但写一下更稳妥。
	
	//设置定时器基本功能： 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//输入滤波的时钟分频，TIM_CKD_DIV1代表分频系数为1
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//CNT计数器的计数模式
	TIM_TimeBaseInitStruct.TIM_Period = 65535;//ARR的自动重装值,比实际目标值小1.
	TIM_TimeBaseInitStruct.TIM_Prescaler = 71;//分频器的值，比分频系数小1.
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;//重复计数器的值（高级计时器才有）
	TIM_TimeBaseInit(TIM3 , &TIM_TimeBaseInitStruct);//为了让PSC和ARR的值立刻生效会马上触发一次更新事件,顺带着会触发更新中断

    TIM_ICInitTypeDef TIM_ICInitStruct;
    TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStruct.TIM_ICFilter = 0xF;
    TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInit(TIM3, &TIM_ICInitStruct);

    TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);
    TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);

    TIM_Cmd(TIM3, ENABLE);
}

uint32_t IC_GetFreq(void)
{
    return 1000000 / (TIM_GetCapture1(TIM3) + 1);//CNT的最后一次计数没加上，遂手动加1
}
