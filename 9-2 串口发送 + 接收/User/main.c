#include "stm32f10x.h"                  // Device header
#include "Hardware.h"
#include "System.h"

uint8_t MyArray[] = {0x6F, 0x6C, 0x6C, 0x65 ,0x20 ,0x77, 0x6F, 0x72, 0x6C, 0x64,0x21,0x0D,0x0A};
uint8_t MyReturn[] = {0x0D,0x0A};
char string[100];
uint8_t Rxdata;

int main(void)
{ 
	OLED_Init ();
	Serial_Init();

	// Serial_SendByte(72);
	
	// Serial_SendArray(MyArray, 13);

	// Serial_SendString("Holle world!\r\n");

	// Serial_SendNumber(114514,6);
	// Serial_SendArray(MyReturn, 2);

	// printf("Num = %d \r\n", 1919810);
	
	// sprintf(string, "Ciallo～(∠・ω< )⌒☆ = %08x \r\n", 0x0d000721);
	// Serial_SendString(string);

	// Serial_Printf("Ciallo～(∠・ω< )⌒☆ = %08x \r\n", 0x0d000721);
	
	while (1)
	{
		// if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)
		// {
		// 	Rxdata = USART_ReceiveData(USART1);
		// 	OLED_ShowBinNum(1,1,Rxdata,8);
		// }
		//查询法
		if (Serial_GetRxFlag() == 1)
		{
			Rxdata = Serial_GetRxData();
			Serial_SendByte(Rxdata);
			OLED_ShowBinNum(1,1,Rxdata,8);
		}
	}
}
