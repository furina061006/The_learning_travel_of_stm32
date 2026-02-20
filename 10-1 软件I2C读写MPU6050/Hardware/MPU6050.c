#include "stm32f10x.h"
#include "Hardware.h"


void MPU6050_Init(void)
{
    MyI2C_Init();

    MPU6050_WriteReg(MPU6050_PWR_MGMT_1,0x01);
    MPU6050_WriteReg(MPU6050_PWR_MGMT_2,0x00);
    MPU6050_WriteReg(MPU6050_SMPLRT_DIV,0x09);
    MPU6050_WriteReg(MPU6050_CONFIG,0x06);
    MPU6050_WriteReg(MPU6050_GYRO_CONFIG,0x18);
    MPU6050_WriteReg(MPU6050_ACCEL_CONFIG,0x18);

}

void Assert_ReceiveAck(void)
{
        if (MyI2C_ReceiveAck() == 0)
        {
            OLED_ShowString(1,1,"target connect!");
        }
}

void MPU6050_WriteReg(uint8_t Addr, uint8_t Data)
{
    MyI2C_Start();
    MyI2C_SendByte(MPU6050_ADDRESS_LOW | WRITE);
    Assert_ReceiveAck();
    
    MyI2C_SendByte(Addr);
    Assert_ReceiveAck();
    
    MyI2C_SendByte(Data);    
    Assert_ReceiveAck();

    MyI2C_Stop();
}

void MPU6050_WriteRegs(uint8_t Addr, uint8_t *Array, uint8_t Size)
{
    MyI2C_Start();
    MyI2C_SendByte(MPU6050_ADDRESS_LOW | WRITE);
    Assert_ReceiveAck();
    
    MyI2C_SendByte(Addr);
    Assert_ReceiveAck();
    
    for (uint8_t i = 0; i < Size; i++)
    {
        MyI2C_SendByte(*(Array++));    
        Assert_ReceiveAck();
    }
    
    MyI2C_Stop();
}

uint8_t MPU6050_ReadReg(uint8_t Addr)
{
    MyI2C_Start();
    
    MyI2C_SendByte(MPU6050_ADDRESS_LOW | WRITE);
    Assert_ReceiveAck();
    
    MyI2C_SendByte(Addr);
    Assert_ReceiveAck();
    
    MyI2C_Start();
    MyI2C_SendByte(MPU6050_ADDRESS_LOW | READ);
    Assert_ReceiveAck();

    uint8_t Data = MyI2C_ReceiveByte();
    MyI2C_SendAck(1);
    
    MyI2C_Stop();

    return Data;
}

void MPU6050_ReadRegs(uint8_t Addr, uint8_t *ParamArray, uint8_t Size)
{
    MyI2C_Start();
    
    MyI2C_SendByte(MPU6050_ADDRESS_LOW | WRITE);
    Assert_ReceiveAck();
    
    MyI2C_SendByte(Addr);
    Assert_ReceiveAck();
    
    MyI2C_Start();
    MyI2C_SendByte(MPU6050_ADDRESS_LOW | READ);
    Assert_ReceiveAck();

    for(uint8_t i = 0; i < (Size - 1); i++)
    {
        ParamArray[i] = MyI2C_ReceiveByte();
        MyI2C_SendAck(0);
    }
    ParamArray[Size - 1] = MyI2C_ReceiveByte();
    MyI2C_SendAck(1);
    
    MyI2C_Stop();
}

void MPU6050_GetData(int16_t ParamArray[6])
{
    MPU6050_ReadRegs(MPU6050_ACCEL_XOUT_H, (uint8_t *)ParamArray ,6);
    
    MPU6050_ReadRegs(MPU6050_GYRO_XOUT_H, (uint8_t *)&ParamArray[3] ,6);
    
    // 进行这步的原因是，MPU6050是大端序， STM32存储信息是小端序；
    for(uint8_t i = 0; i<6; i++)
    {
        ParamArray[i] = (int16_t)(((uint16_t)ParamArray[i]) << 8 | ((uint16_t)ParamArray[i]) >> 8);
        //要先转换成无符号的， 最后再回有符号的。
        //原因是这步之前， ParamArray内是无符号数据类型的写法， 是原码形式存在，最高位表示数值；
        // 但ParamArray本身是有符号的数据类型， 是以补码形式存在的， 最高位代表正负；
        // 由于大小序的关系，一开始的数据是原码类型的高低位颠倒关系，
        //所以需要先还原成 原码 的小端序表示形式。然后再通过强制类型转换变成 补码 的小端序表示形式。
    }
    //也可以换种思路：

    // uint8_t temp[12] = {0};
    // MPU6050_ReadRegs(MPU6050_ACCEL_XOUT_H, temp ,6);
    // MPU6050_ReadRegs(MPU6050_GYRO_XOUT_H, &temp[6] ,6);

    // for(uint8_t i =0; i < 6; i++)
    // {
    //     ParamArray[i] = temp[2*i]<<8 | temp[2*i + 1]; 
    // }

}

uint8_t MPU6050_GetID(void)
{
    return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}
