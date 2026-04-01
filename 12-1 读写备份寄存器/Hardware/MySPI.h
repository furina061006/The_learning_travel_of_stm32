#ifndef __MYSPI_H
#define __MYSPI_H

void MySPI_SInit(void);
void MySPI_Start(void);
void MySPI_Stop(void);
uint8_t MySPI_SSwapByte(uint8_t ByteSend);
void MySPI_HInit(void);
uint8_t MySPI_HSwapByte(uint8_t ByteSend);

#endif
