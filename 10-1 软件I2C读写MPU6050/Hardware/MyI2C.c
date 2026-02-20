#include "stm32f10x.h"
#include "System.h"
#include "MyI2C.h"

void MyI2C_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStruct);

    GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);
}

void MyI2C_W_SCL(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)BitValue);
    Delay_us(10);
}

void MyI2C_W_SDA(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)BitValue);
    Delay_us(10);
}

uint8_t MyI2C_R_SDA(void)
{
    uint8_t BitValue;
    BitValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
    Delay_us(10);
    return BitValue;
}
// 上述四个函数已经把 I2C通信 的基本步骤全封装了。 该行注释以下的函数是利用以上四个基本函数实现的基本时序
// 进行封装的好处是， 如果我需要修改硬件连接的引脚， 那我只需要修改以上的四个函数即可。

void MyI2C_Start(void)
{
    MyI2C_W_SDA(1);
    MyI2C_W_SCL(1);
    MyI2C_W_SDA(0);
    MyI2C_W_SCL(0);//在除了停止外的其他所有时序中， 最后一步都是拉低时钟线。
}

void MyI2C_Stop(void)
{
    MyI2C_W_SDA(0);
    MyI2C_W_SCL(1);
    MyI2C_W_SDA(1);
    //停止后就不需要从机执行任何和主机相关的交互操作, 所以不用拉低时钟线。
}

void MyI2C_SendByte(uint8_t Byte)
{
    for(int8_t i= 0 ; i < 8 ; i++)
    {
        MyI2C_W_SDA(Byte & (0x80 >> i));// 能这样写的原因在于GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)BitValue)函数的封装
        MyI2C_W_SCL(1);// 让从机读
        MyI2C_W_SCL(0);// 为下一次应答提供条件
    }
}

uint8_t MyI2C_ReceiveByte(void)
{
    uint8_t Byte = 0x00;
    MyI2C_W_SDA(1);
    for(int8_t i = 0; i < 8; i++)
    {
        MyI2C_W_SCL(1);
        if(MyI2C_R_SDA() == (uint8_t)Bit_SET)
        {
            Byte |= (0x80 >> i);
        }
        MyI2C_W_SCL(0);
    }
    return Byte;
}

void MyI2C_SendAck(uint8_t AckBit)
{
    MyI2C_W_SDA(AckBit);
    MyI2C_W_SCL(1);
    MyI2C_W_SCL(0);
}

uint8_t MyI2C_ReceiveAck(void)
{
    MyI2C_W_SDA(1);// 主机释放SDA
    MyI2C_W_SCL(1);
    uint8_t AckBit = MyI2C_R_SDA();
    MyI2C_W_SCL(0);
    return AckBit;
}
