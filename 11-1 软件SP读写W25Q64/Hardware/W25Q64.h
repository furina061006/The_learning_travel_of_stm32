#ifndef __W25Q64_H
#define __W25Q64_H

#include "W25Q64_Ins.h"

void W25Q64_Init(void);
void W25Q64_ReadID(uint8_t *MID, uint16_t *DID);
void W25Q64_PagePragram(uint32_t Address, uint8_t *DataArray, uint16_t count);
void W25Q64_SecterErase(uint32_t Address);
void W25Q64_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t count);

#endif 
