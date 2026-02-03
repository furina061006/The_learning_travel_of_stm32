#include "stm32f10x.h"                  // Device header
#include "Hardware.h"
#include "System.h"

float Speed = 0;
uint8_t KeyNum = 0 ;

 int main(void)
 { 
	OLED_Init ();
	Motor_Init();
	Key_Init();

	OLED_ShowString(1,1,"Speed:");
	
	while (1)
	{
		KeyNum = Key_GetNum();
		if(KeyNum == 1)
		{
			Motor_Set_Speed(-70);//这两行是为了小功率时防止熄火用的。
			Delay_ms(20);//不然一开始时,风扇摩檫力太大,电机容易转不起来
			Speed -=10;
			if(Speed< -50)
			{
				Speed = 0;
			}
			Motor_Set_Speed(Speed );
			OLED_ShowSignedNum(2,1,Speed,5);
			Delay_ms(10);
		}
		if(KeyNum == 2)
		{
			Speed +=10;
			Motor_Set_Speed(70);
			Delay_ms(20);
			if(Speed>50)
			{
				Speed = 0;
			}
			Motor_Set_Speed(Speed );
			OLED_ShowSignedNum(2,1,Speed,5);
			Delay_ms(10);
		}
	}
 }
