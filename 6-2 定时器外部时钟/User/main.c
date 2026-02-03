#include "stm32f10x.h"                  // Device header
#include "Hardware.h"
#include "System.h"

int16_t Num = 0;

 int main(void)
 { 
	OLED_Init ();
	Timer_ETRInit();
	
	OLED_ShowString(1,1,"Time:");
	
	while (1)
	{
		OLED_ShowNum(2,1,TIM2_Count(),5);
		OLED_ShowNum(3,1,TIM_GetCounter(TIM2),5);
	}
 }
