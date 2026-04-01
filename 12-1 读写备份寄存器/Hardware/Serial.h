#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

extern uint8_t Serial_RxPacket_H[16];
extern char Serial_RxPacket_T[16];
extern uint8_t RxTyp;

void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *string);
int32_t Pow(uint8_t Value, uint8_t Time);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
void Serial_Printf(char *format, ...);
uint8_t Serial_GetRxFlag(void);
uint8_t Serial_GetRxData(void);
void Serial_SendPacket(uint8_t *Serial_TxPacket, uint16_t Length);

#endif
