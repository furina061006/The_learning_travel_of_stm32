#include "stm32f10x.h"                  // Device header
#include <string.h>
#include "Hardware.h"
#include "System.h"


uint8_t WArray[2] = {0xA1, 0xB2};
uint8_t RArray[2] = {0};
uint8_t Keynum = 0;



int main(void)
{
	OLED_Init();
	
	MyRTC_Init();
	
	struct tm time_data = {
		.tm_year = 2026,
		.tm_mon = 4,
		.tm_mday = 1,
		.tm_hour = 18,
		.tm_min = 38,
		.tm_sec = 34,
	};
	
	MyRTC_ShowTimeFormat();
	MyRTC_WriteTime(&time_data);
	while (1)
	{
		MyRTC_ShowTimeData(&time_data);
	}
}
