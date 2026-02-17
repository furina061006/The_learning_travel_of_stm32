#include "stm32f10x.h"                  // Device header
#include "Hardware.h"
#include "System.h"
#include <string.h>
#include <stdlib.h>

// 定义一个简单的烟花粒子结构体
typedef struct {
    int x;
    int y;      // 高度
    int vy;     // 垂直速度
    int life;   // 生命周期
    int state;  // 0:发射中, 1:爆炸中, 2:消失
} Firework;

// 初始化烟花
void Firework_Init(Firework *fw, int x_pos) {
    fw->x = x_pos;
    fw->y = 63; // 从底部开始
    fw->vy = -(rand() % 3 + 4); // 向上速度 -4 到 -6
    fw->life = 30; // 爆炸后持续帧数
    fw->state = 0; // 发射状态
}

// 更新烟花状态并绘制
void Firework_Update(Firework *fw) {
    if (fw->state == 0) {
        // 发射阶段：向上画一条线或点
        OLED_DrawPoint(fw->x, fw->y, 1);
        fw->y += fw->vy;
        
        // 到达随机高度后爆炸，或者速度减为0
        if (fw->y < (rand() % 20 + 10)) { // 在10-30高度爆炸
            fw->state = 1;
            fw->vy = 0;
        }
    } 
    else if (fw->state == 1) {
        // 爆炸阶段：简单的圆形扩散
        // 这里做一个简单的十字或米字扩散
        int radius = 30 - fw->life; // 随时间变大
        uint8_t bright = (fw->life > 15) ? 1 : 0; // 闪烁效果
        
        // 画简单的火花 (上下左右斜)
        OLED_DrawPoint(fw->x + radius, fw->y, bright);
        OLED_DrawPoint(fw->x - radius, fw->y, bright);
        OLED_DrawPoint(fw->x, fw->y + radius, bright);
        OLED_DrawPoint(fw->x, fw->y - radius, bright);
        
        OLED_DrawPoint(fw->x + radius/2, fw->y + radius/2, bright);
        OLED_DrawPoint(fw->x - radius/2, fw->y - radius/2, bright);
        OLED_DrawPoint(fw->x + radius/2, fw->y - radius/2, bright);
        OLED_DrawPoint(fw->x - radius/2, fw->y + radius/2, bright);
        
        fw->life--;
        if (fw->life == 0) {
            fw->state = 2;
        }
    }
}

int main(void)
{
    // 硬件初始化
    OLED_Init();
    Timer_InInit();
    // 创建两个烟花对象，左边一个，右边一个
    Firework fw_left, fw_right;
    int timer = 0; // 控制发射间隔
    
    while (1)
    {
        // 1. 清空显存
        OLED_ClearBuffer();
        
        // 3. 烟花逻辑
        // 定时发射新烟花
        timer++;
        if (timer > 50) { // 控制发射频率
            timer = 0;
            // 随机在左边或右边发射
            if (rand() % 2)
			{
				Firework_Init(&fw_left, 20 + rand() % 20); // x: 20~40
				TIM_Cmd(TIM2, DISABLE);
				TIM_Cmd(TIM2, ENABLE);
				while (TIM2_Count() == 0);
				OLED_ShowString(1,1,"Python");
				Delay_ms(200);
				OLED_ShowString(2,1,"Java");
				Delay_ms(200);
				OLED_ShowString(3,1,"C++");
				Delay_ms(200);
				OLED_Clear();
				OLED_ShowString(1,1,"Line");
				Delay_ms(200);
				OLED_ShowString(2,1,"Line");
				Delay_ms(200);
				OLED_ShowString(3,1,"So");
				Delay_ms(200);
				OLED_ShowString(4,1,"Easy");
				Delay_ms(200);
				OLED_Clear();
				Delay_ms(800);
			}
			else
			{
				 Firework_Init(&fw_right, 90 + rand() % 20); // x: 90~110
				 TIM_Cmd(TIM2, DISABLE);
				TIM_Cmd(TIM2, ENABLE);
				while (TIM2_Count() == 0);
				OLED_ShowString(1,10,"Years");
				Delay_ms(200);
				OLED_ShowString(2,10,"Months");
				Delay_ms(200);
				OLED_ShowString(3,10,"Weeks");
				Delay_ms(200);
				OLED_Clear();
				OLED_ShowString(1,10,"Day");
				Delay_ms(200);
				OLED_ShowString(2,10,"Day");
				Delay_ms(200);
				OLED_ShowString(3,10,"No");
				Delay_ms(200);
				OLED_ShowString(4,10,"Bug");
				Delay_ms(200);
				OLED_Clear();
				Delay_ms(800);
			}
		}
        
        // 更新并绘制烟花
        if (fw_left.state != 2) Firework_Update(&fw_left);
        if (fw_right.state != 2) Firework_Update(&fw_right);
        
        // 4. 将显存数据发送到屏幕显示
        OLED_Refresh();
        
        // 5. 简单延时，控制动画速度
        for (int i = 0; i < 20000; i++);
    }
}
