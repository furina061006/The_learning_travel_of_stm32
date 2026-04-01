#include "stm32f10x.h"
#include "MySPI.h"

void MySPI_W_CS(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)BitValue);
}

void MySPI_W_MOSI(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_7, (BitAction)BitValue);
}

void MySPI_W_SCK(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction)BitValue);
}

uint8_t MySPI_R_MISO(void)
{
    return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6);
}
// 以上是对单个引脚电平变化的封装：

// 以下是对通信时序的封装：
//模式 0 初始化：
void MySPI_SInit(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_Initstruct;
    GPIO_Initstruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Initstruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_Initstruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_Initstruct);

    GPIO_Initstruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Initstruct.GPIO_Pin = GPIO_Pin_6;
    GPIO_Initstruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_Initstruct);

    MySPI_W_CS(1);
    MySPI_W_SCK(0);
}

void MySPI_Start(void)
{
    MySPI_W_CS(0);
}

void MySPI_Stop(void)
{
    MySPI_W_CS(1);
}

uint8_t MySPI_SSwapByte(uint8_t ByteSend)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        MySPI_W_MOSI((ByteSend) & 0x80);
        ByteSend <<= 1;
        MySPI_W_SCK(1);
        ByteSend |= MySPI_R_MISO();
        MySPI_W_SCK(0);
    }
    return ByteSend;
}

void MySPI_HInit(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    GPIO_InitTypeDef GPIO_Initstruct;
    GPIO_Initstruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Initstruct.GPIO_Pin = GPIO_Pin_4;
    GPIO_Initstruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_Initstruct);

    GPIO_Initstruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Initstruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_Initstruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_Initstruct);

    GPIO_Initstruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Initstruct.GPIO_Pin = GPIO_Pin_6;
    GPIO_Initstruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_Initstruct);

    SPI_InitTypeDef SPI_InitStruct;
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStruct.SPI_CRCPolynomial = 7;
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
    SPI_Init(SPI1, &SPI_InitStruct);

    SPI_Cmd(SPI1, ENABLE);

    MySPI_W_CS(1);
}

uint8_t MySPI_HSwapByte(uint8_t ByteSend)
{
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, ByteSend);

    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    return SPI_I2S_ReceiveData(SPI1);
}
