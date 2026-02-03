#ifndef __ENCODER_H
#define __ENCODER_H

void IT_Encoder_Init(void);
int16_t IT_Encoder_Get(void);
void TIM_Encoder_Init(void);
int16_t TIM_Encoder_GetAngle(void);
int16_t TIM_Encoder_GetSpeed(int16_t NewAngle,int16_t LastAngle,uint16_t Prescaler,uint16_t Period);

#endif
