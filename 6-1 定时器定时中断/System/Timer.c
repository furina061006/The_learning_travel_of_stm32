#include "stm32f10x.h"                  // Device header

uint16_t num = 0;

void Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_InternalClockConfig(TIM2);//开启后默认是内部时钟，但写一下更稳妥。
	
	//设置定时器基本功能： 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//输入滤波的时钟分频，TIM_CKD_DIV1代表分频系数为1
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//CNT计数器的计数模式
	TIM_TimeBaseInitStruct.TIM_Period = 9999;//ARR的自动重装值,比实际目标值小1.
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7199;//分频器的值，比分频系数小1.
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;//重复计数器的值（高级计时器才有）
	TIM_TimeBaseInit(TIM2 , &TIM_TimeBaseInitStruct);//为了让PSC和ARR的值立刻生效会马上触发一次更新事件,顺带着会触发更新中断
	
	//清除更新中断标志位
	TIM_ClearFlag(TIM2 ,  TIM_FLAG_Update);
	
	//设置更新中断/事件
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//开启更新中断到NVIC 的通路
	
	//选择NVIC优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	//设置NVIC 的基本配置。
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;//     对TIM2_IRQn的通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//     				   开放
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	//使能CNT计数器
	TIM_Cmd(TIM2, ENABLE);
	
}

uint16_t TIM2_Count()
{
	return num;
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		num ++;
		TIM_ClearITPendingBit(TIM2 , TIM_IT_Update);
	}
}