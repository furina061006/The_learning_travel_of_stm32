#include "stm32f10x.h"                  // Device header
#include "Hardware.h"
#include "System.h"

uint16_t ADValue = 0;
float Volatge = 0;

int main(void)
{ 
	OLED_Init ();
	AD_Init();

	
	OLED_ShowString(1,1,"Volatge1:");
	OLED_ShowString(1,10,"0.00");
	OLED_ShowString(2,1,"Volatge2:");
	OLED_ShowString(2,10,"0.00");
	OLED_ShowString(3,1,"Volatge3:");
	OLED_ShowString(3,10,"0.00");
	OLED_ShowString(4,1,"Volatge4:");
	OLED_ShowString(4,10,"0.00");


	while (1)
	{
		ADValue = AD_GetValue(ADC_Channel_0);
		Volatge = (float) ADValue / 4095 * 3.3;
		OLED_ShowNum(1,10,(uint16_t) Volatge,1);
		OLED_ShowNum(1,12,(uint16_t) (Volatge * 100) % 100 ,2);
		ADValue = AD_GetValue(ADC_Channel_1);
		Volatge = (float) ADValue / 4095 * 3.3;
		OLED_ShowNum(2,10,(uint16_t) Volatge,1);
		OLED_ShowNum(2,12,(uint16_t) (Volatge * 100) % 100 ,2);
		ADValue = AD_GetValue(ADC_Channel_2);
		Volatge = (float) ADValue / 4095 * 3.3;
		OLED_ShowNum(3,10,(uint16_t) Volatge,1);
		OLED_ShowNum(3,12,(uint16_t) (Volatge * 100) % 100 ,2);
		ADValue = AD_GetValue(ADC_Channel_3);
		Volatge = (float) ADValue / 4095 * 3.3;
		OLED_ShowNum(4,10,(uint16_t) Volatge,1);
		OLED_ShowNum(4,12,(uint16_t) (Volatge * 100) % 100 ,2);
		Delay_ms(100);
	}
}
