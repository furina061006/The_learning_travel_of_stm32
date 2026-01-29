#include "stm32f10x.h"                  // Device header
#include "Hardware.h"

int16_t Num = 0;

 int main(void)
 { 
	OLED_Init ();
	Encoder_Init();
	
	
	while (1)
	{
		Num += Encoder_Get();
		OLED_ShowSignedNum(1, 5, Num, 5);
	}
 }
