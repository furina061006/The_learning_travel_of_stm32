#include "stm32f10x.h"                  // Device header
#include "Hardware.h"
#include "System.h"
#include <string.h>

uint8_t WArray[2] = {0xA1, 0xB2};
uint8_t RArray[2] = {0};
uint8_t Keynum = 0;

int main(void)
{
	OLED_Init();
	Key_Init();
	MyBKP_Init();

	OLED_ShowString(1,1, "W:");
	OLED_ShowString(3,1, "R:");

	OLED_ShowHexNum(2,1,WArray[0],4);
	OLED_ShowHexNum(2,8,WArray[1],4);
	OLED_ShowHexNum(4,1,RArray[0],4);
	OLED_ShowHexNum(4,8,RArray[1],4);

	while (1)
	{
		Keynum = Key_GetNum();
		
		if (Keynum == 1) {
			WArray[0]++, WArray[1]++;
			BKP_WriteBackupRegister(BKP_DR1, WArray[0]);
			BKP_WriteBackupRegister(BKP_DR2, WArray[1]);
			OLED_ShowHexNum(2,1,WArray[0],4);
			OLED_ShowHexNum(2,8,WArray[1],4);
		}
		RArray[0] = BKP_ReadBackupRegister(BKP_DR1);
		RArray[1] =	BKP_ReadBackupRegister(BKP_DR2);
		OLED_ShowHexNum(4,1,RArray[0],4);
		OLED_ShowHexNum(4,8,RArray[1],4);
	}
}
