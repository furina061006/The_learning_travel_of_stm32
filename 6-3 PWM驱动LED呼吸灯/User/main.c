#include "stm32f10x.h"                  // Device header
#include "Hardware.h"
#include "System.h"

uint16_t Num = 0;

 int main(void)
 { 
	OLED_Init ();
	PWM_Init();
	
	OLED_ShowString(1,1,"Compare:");
	
	while (1)
	{
		for (Num = 0; Num<100; Num++)
		{
			PWM_SetCompare1(Num);
			OLED_ShowNum(2,1,(uint32_t)Num,5);
			Delay_ms(10);
		}
		for (int i=100, Num = 100; i>=0;i--, Num--)
		{
			PWM_SetCompare1(Num);
			OLED_ShowNum(2,1,(uint32_t)Num,5);
			Delay_ms(10);
		}
	}
 }
