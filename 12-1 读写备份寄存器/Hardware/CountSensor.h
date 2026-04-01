#ifndef __COUNTSENSOR_H
#define __COUNTSENSOR_H

void CountSensor_Init(void);
uint16_t CountSensor_Get(void);
//中断函数无需在头文件内声明，因为不在主程序内使用
#endif
