#include "stm32f10x.h"                  // Device header
#include "Hardware.h"
#include "System.h"
#include <string.h>

uint8_t ReArray[6] = {0};
uint8_t WrArray[6] = {0};
int16_t DataArray[6] = {0};

int main(void)
{ 
	OLED_Init ();
	MPU6050_Init();

	WrArray[0] = 0x00;
	MPU6050_WriteRegs(0x6B, WrArray, 1);

	// WrArray[0] = 0xAA;
	// MPU6050_WriteRegs(0x19, WrArray, 1);

	// MPU6050_ReadRegs(0x19, ReArray, 1);

	// OLED_ShowHexNum(2,1,ReArray[0],2);

	// OLED_ShowHexNum(2,1,MPU6050_GetID(),2);

	while (1)
	{
		MPU6050_GetData(DataArray);
		OLED_ShowSignedNum(2,1,DataArray[0],5);
		OLED_ShowSignedNum(3,1,DataArray[1],5);
		OLED_ShowSignedNum(4,1,DataArray[2],5);
		OLED_ShowSignedNum(2,8,DataArray[3],5);
		OLED_ShowSignedNum(3,8,DataArray[4],5);
		OLED_ShowSignedNum(4,8,DataArray[5],5);
	}
}
