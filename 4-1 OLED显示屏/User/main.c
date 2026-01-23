#include "stm32f10x.h"                  // Device header
#include "Hardware.h"

uint8_t KeyNum = 0;

 int main(void)
 { 
	OLED_Init ();
	 
	
	 
	while (1)
	{
	//OLED_ShowChar (1,1,'A');
	OLED_ShowString(1,1,"Furina,");
	Delay_ms(1013);
	OLED_Clear();
	OLED_ShowString(2,1,"After");	
	OLED_ShowNum(2,7,182376,6);
	OLED_ShowString(2,14,"day");
	//OLED_ShowSignedNum(2,7,-66,2);
	Delay_ms(1013);
	OLED_Clear();
	//OLED_ShowHexNum(3,1,0xAA55,4);
	OLED_ShowString(3,1,"You,");
	Delay_ms(1013);
	OLED_Clear();	
	//OLED_ShowBinNum(4,1, 0xAA55,16);
	OLED_ShowString(4,1,"will free.");
	Delay_ms(1013);
	OLED_Clear();
	}
 }
 