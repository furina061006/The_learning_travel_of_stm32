#include "stm32f10x.h"                  // Device header
#include "Hardware.h"
#include "System.h"
#include <string.h>

uint8_t MID = 0;
uint16_t DID = 0;
uint8_t WArray[10] = {0xA1, 0xB2, 0xC3, 0xD4};
uint8_t RArray[10] = {0};

int main(void)
{
	OLED_Init ();
	W25Q64_Init();

	OLED_ShowString(1,1, "MID:   DID:");
	OLED_ShowString(2,1, "W:");
	OLED_ShowString(3,1, "R:");

	W25Q64_ReadID(&MID, &DID);
	OLED_ShowHexNum(1,5,MID, 2);
	OLED_ShowHexNum(1,12,DID, 4);
	
	W25Q64_SecterErase(0x000000);
	// W25Q64_PagePragram(0x000000, WArray, 4);
	W25Q64_ReadData(0x000000, RArray, 4);

	for(uint8_t i = 0; i < 4; i++)
	{
		OLED_ShowHexNum(2, 3*(i+1), WArray[i], 2);
	}
	for(uint8_t i = 0; i < 4; i++)
	{
		OLED_ShowHexNum(3, 3*(i+1), RArray[i], 2);
	}
	


	while (1)
	{
		
	}
}
