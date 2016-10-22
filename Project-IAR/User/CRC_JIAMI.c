
#include "CRC_JIAMI.h"

#define TableSize	10

static __IO uint32_t uwTick;

uint8_t JIAMI_Table[TableSize] = {7,13,21,9,18,64,33,71,27,81};
uint8_t JIEMI_Table[TableSize] = {81,27,71,33,64,18,9,21,13,7};

uint16_t	CRC16CCITT_Table[16]=
{
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7, 
	0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef
};

void JIAMI(uint8_t * Buf,uint32_t Len,uint8_t Key)
{
	uint32_t cnt_1 = 0,cnt_2 = 0;
	uint8_t * temp = Buf;
	for(cnt_1 = 0;cnt_1 < Len;cnt_1 ++)
	{
		*temp ^= Key;
		*temp ^= cnt_1;
		*temp ^= 0xFF;
		for(cnt_2 = 0;cnt_2 < TableSize;cnt_2 ++)
		{
			*temp ^= JIAMI_Table[cnt_2];
		}
		temp++;
	}
}

void JIEMI(uint8_t * Buf,uint32_t Len,uint8_t Key)
{
	uint32_t cnt_1 = 0,cnt_2 = 0;
	uint8_t * temp = Buf;
	for(cnt_1 = 0;cnt_1 < Len;cnt_1 ++)
	{
		for(cnt_2 = 0;cnt_2 < TableSize;cnt_2 ++)
		{
			*temp ^= JIEMI_Table[cnt_2];
		}
		*temp ^= 0xFF;
		*temp ^= cnt_1;
		*temp ^= Key;
		temp++;
	}
}

uint16_t CRC16Ccitt(uint8_t *Spr,int Len)
{
	uint16_t 	Crc=0;
	uint8_t		Temp;
	
	while(Len--!=0)
	{
		Temp=((uint8_t)(Crc>>8))>>4;
		Crc<<=4;
		Crc ^= CRC16CCITT_Table[Temp^(*Spr/16)];
		Temp=((unsigned char)(Crc/256)/16);
		Crc<<=4;
		Crc ^= CRC16CCITT_Table[Temp^(*Spr&0x0f)];
		Spr++;
	}
	return Crc;
}

result CRC_JIAMI(uint8_t * Data)
{
	uint8_t * Buf = Data + 3;
    
    uint16_t crc = CRC16Ccitt(Buf,61);
    
    Data[1] = crc >> 8;
    Data[2] = crc &  0x00FF;
	
	Buf = Data + 1;
	
    //获取伪随机数
    
	Data[0] = HAL_GetTick() % 256;//0x12;//random();uwTick
	
	JIAMI(Buf,63,Data[0]);
    
    return true;
}

result CRC_JIEMI(uint8_t * Data)
{
	uint8_t * Buf = Data + 1;
	
    uint16_t crc = 0x00;
    
	JIEMI(Buf,63,Data[0]);
	
    crc = Data[1] * 256 + Data[2];
    
	Buf = Data + 3;
	
	if(crc == CRC16Ccitt(Buf,61))return true;
	
	return false;
}
















