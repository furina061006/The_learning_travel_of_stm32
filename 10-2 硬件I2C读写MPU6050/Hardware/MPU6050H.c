#include "stm32f10x.h"
#include "Hardware.h"


void MPU6050H_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

    I2C_InitTypeDef I2C_InitStructure;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 50000;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
    I2C_Init(I2C2,&I2C_InitStructure);

    I2C_Cmd(I2C2,ENABLE);

    MPU6050H_WriteReg(MPU6050_PWR_MGMT_1, 0x01);				//电源管理寄存器1，取消睡眠模式，选择时钟源为X轴陀螺仪
	MPU6050H_WriteReg(MPU6050_PWR_MGMT_2, 0x00);				//电源管理寄存器2，保持默认值0，所有轴均不待机
	MPU6050H_WriteReg(MPU6050_SMPLRT_DIV, 0x09);				//采样率分频寄存器，配置采样率
	MPU6050H_WriteReg(MPU6050_CONFIG, 0x06);					//配置寄存器，配置DLPF
	MPU6050H_WriteReg(MPU6050_GYRO_CONFIG, 0x18);			//陀螺仪配置寄存器，选择满量程为±2000°/s
	MPU6050H_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);			//加速度计配置寄存器，选择满量程为±16g
}

void MPU6050H_CheckEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
    uint32_t timeout = 10000;
    while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)
    {
        timeout --;
        if (timeout == 0)
        break;
    }
    
}

void MPU6050H_WriteReg(uint8_t Addr, uint8_t Data)
{
    I2C_GenerateSTART(I2C2, ENABLE);

    MPU6050H_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
    I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS_LOW, I2C_Direction_Transmitter);

    MPU6050H_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
    I2C_SendData(I2C2, Addr);

    MPU6050H_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);
    I2C_SendData(I2C2, Data);
    
    MPU6050H_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
    I2C_GenerateSTOP(I2C2, ENABLE);
}

void MPU6050H_WriteRegs(uint8_t Addr, uint8_t *Array, uint8_t Size)
{
    I2C_GenerateSTART(I2C2, ENABLE);

    MPU6050H_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
    I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS_LOW, I2C_Direction_Transmitter);

    MPU6050H_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
    I2C_SendData(I2C2, Addr);

    MPU6050H_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);
    for ( uint8_t i = 0; i < Size - 1; i++)
    {
        I2C_SendData(I2C2, *(Array++));
        MPU6050H_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);
    }
    
    I2C_SendData(I2C2, Array[Size - 1]);

    MPU6050H_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
    I2C_GenerateSTOP(I2C2, ENABLE);
}

uint8_t MPU6050H_ReadReg(uint8_t Addr)
{
    I2C_GenerateSTART(I2C2, ENABLE);

    MPU6050H_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
    I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS_LOW, I2C_Direction_Transmitter);

    MPU6050H_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
    I2C_SendData(I2C2, Addr);

    MPU6050H_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
    I2C_GenerateSTART(I2C2, ENABLE);

    MPU6050H_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
    I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS_LOW, I2C_Direction_Receiver);

    MPU6050H_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
    I2C_AcknowledgeConfig(I2C2, DISABLE);
    I2C_GenerateSTOP(I2C2, ENABLE);

    MPU6050H_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);
    uint8_t Data = I2C_ReceiveData(I2C2);

    I2C_AcknowledgeConfig(I2C2, ENABLE);

    return Data;
}

void MPU6050H_ReadRegs(uint8_t Addr, uint8_t *ParamArray, uint8_t Size)
{
    I2C_GenerateSTART(I2C2, ENABLE);
    MPU6050H_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);

    I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS_LOW, I2C_Direction_Transmitter);
    MPU6050H_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);

    I2C_SendData(I2C2, Addr);
    MPU6050H_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);

    I2C_GenerateSTART(I2C2, ENABLE);
    MPU6050H_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);

    I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS_LOW, I2C_Direction_Receiver);
    MPU6050H_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);

    for (uint8_t i = 0; i < Size - 1; i++)
    {
        MPU6050H_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);
        ParamArray[i] = I2C_ReceiveData(I2C2);

    }
    
    I2C_AcknowledgeConfig(I2C2, DISABLE);
    I2C_GenerateSTOP(I2C2, ENABLE);
    MPU6050H_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);
    ParamArray[Size - 1] = I2C_ReceiveData(I2C2);

    I2C_AcknowledgeConfig(I2C2, ENABLE);
}

void MPU6050H_GetData(int16_t ParamArray[6])
{
    MPU6050H_ReadRegs(MPU6050_ACCEL_XOUT_H, (uint8_t *)ParamArray ,6);
    
    MPU6050H_ReadRegs(MPU6050_GYRO_XOUT_H, (uint8_t *)&ParamArray[3] ,6);
    
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
    // MPU6050H_ReadRegs(MPU6050_ACCEL_XOUT_H, temp ,6);
    // MPU6050H_ReadRegs(MPU6050_GYRO_XOUT_H, &temp[6] ,6);

    // for(uint8_t i =0; i < 6; i++)
    // {
    //     ParamArray[i] = temp[2*i]<<8 | temp[2*i + 1]; 
    // }

}

uint8_t MPU6050H_GetID(void)
{
    return MPU6050H_ReadReg(MPU6050_WHO_AM_I);
}
