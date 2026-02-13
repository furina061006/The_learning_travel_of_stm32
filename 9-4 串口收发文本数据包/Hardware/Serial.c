#include "stm32f10x.h"
#include <stdio.h>
#include <stdarg.h>


uint8_t Serial_RxPacket_H[16] = {0};
char Serial_RxPacket_T[16] = {0};
uint8_t Serial_RxData;
uint8_t Serial_RxFlag;
uint8_t RxTyp = 0;

void Serial_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStruct);

    USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.USART_BaudRate = 9600;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART1, &USART_InitStruct);

    USART_ITConfig(USART1, USART_IT_RXNE,ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStruct);

    USART_Cmd(USART1, ENABLE);
}

void Serial_SendByte(uint8_t Byte)
{
    USART_SendData(USART1, Byte);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    // 在 启用 USART_SendData(USART1, Byte) 时， 会该标志位会自动置 RESET。
}

void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
    uint16_t i;
    for (i = 0; i < Length; i++)
    {
        Serial_SendByte(Array[i]);
    }
}

void Serial_SendString(char *string)
{
    uint8_t i;
    for (i = 0; string[i] != '\0'; i++)
    {
        Serial_SendByte(string[i]);
    }
}

int32_t Pow(uint8_t Value, uint8_t Time)
{
    int32_t Pow = 1;
    for (int8_t i = 0 ; i < Time; i++)
    {
        Pow *= Value;
    }
    return Pow;
}


void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
    for (int8_t i = 0; i < Length; i++)
    {
        Serial_SendByte ((Number / Pow(10 ,Length - i - 1)) % 10 + '0');
    }
}

int fputc(int ch, FILE *f)
{
    Serial_SendByte(ch);
    return ch;
}

void Serial_Printf(char *format, ...)
{
    char String[100];
    va_list arg;
    va_start (arg, format);
    vsprintf(String, format, arg);
    va_end (arg);
    Serial_SendString(String);
}

uint8_t Serial_GetRxFlag(void)
{
    if (Serial_RxFlag == 1)
    {
        Serial_RxFlag = 0;
        return 1;
    }
    return 0;
}

uint8_t Serial_GetRxData(void)
{
    return Serial_RxData;
}

void Serial_SendPacket(uint8_t *Serial_TxPacket, uint16_t Length)
{
    Serial_SendByte(0xFF);
    Serial_SendArray(Serial_TxPacket, Length);
    Serial_SendByte(0xFE);
}

void ClearArray(char *array, uint8_t Num)
{
    for(int8_t i = 0; i < Num; i++)
    {
        array[i] = ' ';
    }
}

void USART1_IRQHandler(void)
{
    
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
    {
        static uint8_t Rxstate = 0;
        static uint8_t RxDataNum = 0;
        static uint8_t RxType = 0;
        Serial_RxData = USART_ReceiveData(USART1);
        switch (Rxstate)
        {
        case 0:
            if (Serial_RxData == 0xFF)
            {
                Rxstate = 1;
                RxType = 0;
            }
            else if (Serial_RxData == '@' || Serial_RxFlag == 0)
            {
                Rxstate = 1;
                RxType = 1;
                ClearArray(Serial_RxPacket_T,16);
            }
            
            break;
        
        case 1:
            switch (RxType)
            {
            case 0:
                
                if (RxDataNum > 3)
                {
                    RxDataNum = 0;
                    Rxstate = 2;
                    break;
                }
                Serial_RxPacket_H [RxDataNum ++] = Serial_RxData;
                break;
            
            case 1:
                if (Serial_RxData == '\r')
                {
                    Rxstate = 2;
                    break;
                }
                if (RxDataNum > 15 )
                {
                    RxDataNum = 0;
                    Rxstate = 0;
                    Serial_SendString("too long OR error !\r\n");
                    break;
                }
                Serial_RxPacket_T [RxDataNum ++] = Serial_RxData;
                break;

            default:
                break;
            }
            break;

        case 2:
            switch (RxType)
            {
            case 0:
                if (Serial_RxData == 0xFE)
                {
                    Serial_RxFlag = 1;
                    Rxstate = 0;
                }
                break;
            
            case 1:
                if (Serial_RxData == '\n')
                {
                    Serial_RxPacket_T [RxDataNum ++] = '\0';
                    Serial_RxFlag = 1;
                    RxDataNum = 0;
                    Rxstate = 0;
                }
                else
                {
                    Rxstate = 1;
                }
                break;

            default:
                break;
            }
            break;

        default:
            break;
        }
        RxTyp = RxType;
        USART_ClearITPendingBit(USART1 ,USART_IT_RXNE);
    }
}

