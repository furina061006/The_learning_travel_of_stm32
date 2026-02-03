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
	PWMI_Init();

	PWM_SetPrescaler(719);
	PWM_SetCompare3(80);
	OLED_ShowString(1,1,"Frequency:");
	OLED_ShowString(2,1,"00000Hz");
	OLED_ShowString(3,1,"Duty:");
	OLED_ShowString(4,1,"000%");
	while (1)
	{
		OLED_ShowNum(2,1,IC_GetFreq(), 5);
		OLED_ShowNum(4,1,IC_GetDuty(), 3);
	}
 }
