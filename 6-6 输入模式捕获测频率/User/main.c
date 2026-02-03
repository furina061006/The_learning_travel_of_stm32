#include "stm32f10x.h"                  // Device header
#include "Hardware.h"
#include "System.h"

float Prescaler = 0;
uint8_t KeyNum = 0 ;

 int main(void)
 { 
	OLED_Init ();
	PWM_Init();
	Key_Init();
	IC_Init();

	PWM_SetPrescaler(719);
	PWM_SetCompare3(50);
	OLED_ShowString(1,1,"Frequency:");
	 OLED_ShowString(2,1,"00000Hz");
	while (1)
	{
		OLED_ShowNum(2,1,IC_GetFreq(), 5);
	}
 }
