#include "stm32f10x.h"                  // Device header
#include "Hardware.h"
#include "System.h"

float Angle = 0;
uint8_t KeyNum = 0 ;

 int main(void)
 { 
	OLED_Init ();
	Servo_Init();
	Key_Init();

	OLED_ShowString(1,1,"Angle:");
	
	while (1)
	{
		KeyNum = Key_GetNum();
		if(KeyNum == 1)
		{
			for (float Angle_1 = 0; Angle_1<180; Angle_1++)
			{
				Servo_SetAngle(Angle_1);
				OLED_ShowNum(2,1,(uint32_t)Angle_1,5);
				Delay_ms(10);
			}
			for (float Angle_1 =180;Angle_1>=0; Angle_1--)
			{
				Servo_SetAngle(Angle_1);
				OLED_ShowNum(2,1,(uint32_t)Angle_1,5);
				Delay_ms(10);
			}
		}
		if(KeyNum == 2)
		{
			Angle +=30;
			if(Angle>180)
			{
				Angle = 0;
			}
			Servo_SetAngle(Angle);
			OLED_ShowNum(2,1,(uint32_t)Angle,5);
			Delay_ms(10);
		}
	}
 }
