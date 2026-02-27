#include "stm32f10x.h"
#include "MySPI.h"
#include "W25Q64_Ins.h"

void W25Q64_Init(void)
{
    MySPI_Init();
}

void W25Q64_ReadID(uint8_t *MID, uint16_t *DID)
{
    MySPI_Start();
    MySPI_SwapByte(W25Q64_JEDEC_ID);
    *MID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
    *DID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
    *DID <<= 8;
    *DID |= MySPI_SwapByte(W25Q64_DUMMY_BYTE);
    MySPI_Stop();
}

void W25Q64_WriteCmd(FunctionalState State)
{
    if (State == DISABLE)
    {
        MySPI_Start();
        MySPI_SwapByte(W25Q64_WRITE_DISABLE);
        MySPI_Stop();
    }
    else
    {
        MySPI_Start();
        MySPI_SwapByte(W25Q64_WRITE_ENABLE);
        MySPI_Stop();
    }
}

void W25Q64_WaitBusy()
{
    uint32_t Timeout = 100000;
    MySPI_Start();
    MySPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);
    while (MySPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01 == 0x01)
    {
        if (Timeout-- == 0)
        {
            break;
        }
    }
    MySPI_Stop();
}

void W25Q64_PagePragram(uint32_t Address, uint8_t *DataArray, uint16_t count)
{
    W25Q64_WaitBusy();
    W25Q64_WriteCmd(ENABLE);
    MySPI_Start();
    MySPI_SwapByte(W25Q64_PAGE_PROGRAM);
    MySPI_SwapByte(Address >> 16);
    MySPI_SwapByte(Address >> 8);
    MySPI_SwapByte(Address);
    for (int16_t i = 0; i < count; i++)
    {
        MySPI_SwapByte(*(DataArray++));
    }
    MySPI_Stop();
}

void W25Q64_SecterErase(uint32_t Address)
{
    W25Q64_WaitBusy();
    W25Q64_WriteCmd(ENABLE);
    MySPI_Start();
    MySPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);
    MySPI_SwapByte(Address >> 16);
    MySPI_SwapByte(Address >> 8);
    MySPI_SwapByte(Address);
    MySPI_Stop();
}

void W25Q64_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t count)
{
    W25Q64_WaitBusy();
    MySPI_Start();
    MySPI_SwapByte(W25Q64_READ_DATA);
    MySPI_SwapByte(Address >> 16);
    MySPI_SwapByte(Address >> 8);
    MySPI_SwapByte(Address);
    for (int32_t i = 0; i < count; i++)
    {
        *(DataArray++) = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
    }
    MySPI_Stop();
}
