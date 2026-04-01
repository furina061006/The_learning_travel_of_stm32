#include "stm32f10x.h"
#include <time.h>
#include "Hardware.h"

void MyRTC_Init(void) {
    // 配置BKP和PWR的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);

    PWR_BackupAccessCmd(ENABLE);

    // LSE 归 RCC 模块管理。
    RCC_LSEConfig(RCC_LSE_ON);
    while ((RCC_GetFlagStatus(RCC_FLAG_LSERDY)) == RESET);//等待外部低速晶振（LSE）启动并稳定下来

    // 没想到吧， RTC 和 CLK 的连接 还是归 RCC 管
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
    RCC_RTCCLKCmd(ENABLE);// 确定RTC的时钟选择， 在ENABLE期间无法再更改时钟源配置
    
    // 与RTC相关操作可以使用条件复位， 但谨记，该条件语句上方的行为是每次初始化必须的，但是实验现象是可以放函数开头，不知道为什么...
    if (BKP_ReadBackupRegister(BKP_DR1) != 0xFFFF) {
        BKP_WriteBackupRegister(BKP_DR1, 0xFFFF); 
        
        // 等待 RTC数据上传到 APB1 接口的一个影子寄存器 
        RTC_WaitForSynchro();

        RTC_WaitForLastTask();// 等待上个写操作完毕
        RTC_SetPrescaler(32768 - 1);

        RTC_WaitForLastTask();
        RTC_SetCounter(0);
    } else {
        RTC_WaitForSynchro();
    }
}

void MyRTC_WriteTime(struct tm *time_data) {
    if (BKP_ReadBackupRegister(BKP_DR2) != 0xFFFF) {
        BKP_WriteBackupRegister(BKP_DR2, 0xFFFF); 
        
        time_data->tm_year -= 1900;
        time_data->tm_mon -= 1;

        time_t time_cnt = mktime(time_data);
        RTC_WaitForLastTask();
        RTC_SetCounter(time_cnt);
    }
}

struct tm *MyRTC_ReadTime(void){
    struct tm *time_data;

    RTC_WaitForLastTask();
    time_t time_cnt = RTC_GetCounter();

    time_data = localtime(&time_cnt);
    time_data->tm_year += 1900;
    time_data->tm_mon += 1;
    return time_data; 
}

void MyRTC_ShowTimeFormat(void) {
    OLED_ShowString(1,1,"date:xxxx-xx-xx");
	OLED_ShowString(2,1,"time:xx-xx-xx");
	OLED_ShowString(3,1,"CNT :xxxxxxxxxx");
    OLED_ShowString(4,1,"DIV :xxxxxxxxxx");
}


void MyRTC_ShowTimeData(struct tm *time_data) {
    time_data = MyRTC_ReadTime();
	OLED_ShowNum(1, 6,time_data->tm_year , 4);
	OLED_ShowNum(1, 11,time_data->tm_mon , 2);
	OLED_ShowNum(1, 14,time_data->tm_mday , 2);
	OLED_ShowNum(2, 6,time_data->tm_hour , 2);
	OLED_ShowNum(2, 9,time_data->tm_min , 2);
	OLED_ShowNum(2, 12,time_data->tm_sec , 2);
	OLED_ShowNum(3,6,RTC_GetCounter(),10);
    OLED_ShowNum(4,6,RTC_GetDivider(),10);
}
