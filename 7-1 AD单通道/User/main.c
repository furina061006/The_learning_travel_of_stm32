#include "stm32f10x.h"                  // Device header
#include "Hardware.h"
#include "System.h"

uint16_t ADValue = 0;
float Volatge = 0;

int main(void)
{ 
	OLED_Init ();
	AD_Init();


	OLED_ShowString(1,1,"ADValue:");
	OLED_ShowString(2,1,"0000");
	OLED_ShowString(3,1,"Volatge:");
	OLED_ShowString(4,1,"0.00");

	while (1)
	{
		ADValue = AD_GetValue();
		Volatge = (float) ADValue / 4095 * 3.3;
		OLED_ShowNum(2,1,ADValue,4);
		OLED_ShowNum(4,1,(uint16_t) Volatge,1);
		OLED_ShowNum(4,3,(uint16_t) (Volatge * 100) % 100 ,2);

		Delay_ms(100);
	}
}
