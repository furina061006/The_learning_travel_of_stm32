#include "stm32f10x.h"                  // Device header
#include "Delay.h"
 int main(void)
 { 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* 
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);//置低电平
	GPIO_SetBits(GPIOA, GPIO_Pin_0);//置高电平
	GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);//看第三个参数，如果是RESET就是置低电平；是SET就是置高电平 
	 */
	while (1)
	{
		GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);//Bit_RESET,也可以写成 (BitAction)0
		Delay_ms(500);
		GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);////Bit_SET,也可以写成 (BitAction)1
		Delay_ms(500);
	}
 }
 