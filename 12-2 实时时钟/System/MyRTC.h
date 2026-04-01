#ifndef __MYRTC_H
#define __MYRTC_H

#include <time.h>

void MyRTC_Init(void);
void MyRTC_WriteTime(struct tm *time_data);
struct tm *MyRTC_ReadTime(void);
void MyRTC_ShowTimeFormat(void);
void MyRTC_ShowTimeData(struct tm *time_data);

#endif
