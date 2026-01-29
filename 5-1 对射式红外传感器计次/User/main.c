#include "stm32f10x.h"                  // Device header
#include "Hardware.h"

int16_t Num = 0;

 int main(void)
 { 
	OLED_Init ();
	CountSensor_Init();
	
	OLED_ShowString(1,1,"Count:");
	
	while (1)
	{
		OLED_ShowNum(2,1,CountSensor_Get(),5);
	}
 }
