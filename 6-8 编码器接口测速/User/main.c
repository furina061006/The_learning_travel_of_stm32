#include "stm32f10x.h"                  // Device header
#include "Hardware.h"
#include "System.h"

float Speed = 0;
uint8_t KeyNum = 0 ;
int16_t LastAngle = 0;
int16_t NewAngle = 0;
uint16_t Prescaler = 0;
uint16_t Period = 0;

int main(void)
{ 
	OLED_Init ();
	TIM_Encoder_Init();
	Timer_InInit();

	Prescaler = 7199;
	Period = 999;
	// TIM_PrescalerConfig(TIM2,Prescaler,TIM_PSCReloadMode_Update);
	// TIM_SetAutoreload(TIM2,Period);

	OLED_ShowString(1,1,"Angle:");
	OLED_ShowString(2,1,"000");
	OLED_ShowString(3,1,"Speed:");
	OLED_ShowString(4,1,"0000");

	while (1)
	{
		LastAngle = NewAngle;
		NewAngle = TIM_Encoder_GetAngle();
		OLED_ShowSignedNum(2,1,NewAngle,3);
		if (TIM2_Count() == 1)
		{
			Speed = TIM_Encoder_GetSpeed(NewAngle,LastAngle,Prescaler,Period);
			OLED_ShowSignedNum(4,1,Speed,4);
			Delay_ms(100);
		}
	}
}
