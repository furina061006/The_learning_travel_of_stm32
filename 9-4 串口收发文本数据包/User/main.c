#include "stm32f10x.h"                  // Device header
#include "Hardware.h"
#include "System.h"
#include <string.h>

uint8_t KeyNum = 0;
uint8_t MyArray[] = {0x11, 0x45, 0x14, 0x00};
uint8_t MyReturn[] = {0x0D,0x0A};
char string[100];
uint8_t Rxdata;

int main(void)
{ 
	OLED_Init ();
	Key_Init();
	Serial_Init();
	LED_Init();

	OLED_ShowString(1,1,"TxPacket:");
	OLED_ShowString(3,1,"RxPacket:");

	while (1)
	{
		KeyNum = Key_GetNum();
		if(KeyNum == 1)
		{
			Serial_SendPacket(MyArray, 4);
			OLED_ShowHexNum(2,1,MyArray[0]++,2);
			OLED_ShowHexNum(2,4,MyArray[1]++,2);
			OLED_ShowHexNum(2,7,MyArray[2]++,2);
			OLED_ShowHexNum(2,10,MyArray[3]++,2);
		}

		if(Serial_GetRxFlag() == 1)
		{
			switch (RxTyp)
			{
			case 0: 
				OLED_ShowHexNum(4,1,Serial_RxPacket_H[0],2);
				OLED_ShowHexNum(4,4,Serial_RxPacket_H[1],2);
				OLED_ShowHexNum(4,7,Serial_RxPacket_H[2],2);
				OLED_ShowHexNum(4,10,Serial_RxPacket_H[3],2);
				break;

			case 1:
				OLED_ShowString(4,1,"                ");
				OLED_ShowString(4,1,Serial_RxPacket_T);
				if (strcmp(Serial_RxPacket_T, "LED_ON") == 0)
				{
					LED1_ON();
					Serial_SendString("LED_ON_OK\r\n");
					OLED_ShowString(2,1,"                ");
					OLED_ShowString(2,1,"LED_ON_OK");
				}
				else if (strcmp(Serial_RxPacket_T, "LED_OFF") == 0)
				{
					LED1_OFF();
					Serial_SendString("LED_OFF_OK\r\n");
					OLED_ShowString(2,1,"                ");
					OLED_ShowString(2,1,"LED_OFF_OK");
				}
				else
				{
					Serial_SendString("Error_Command\r\n");
					OLED_ShowString(2,1,"                ");
					OLED_ShowString(2,1,"Error_Command");
				}
				break;
			default:
				break;
			}
		}
	}
}
