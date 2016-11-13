
#ifndef __RELAY_H__
#define __RELAY_H__

#include "Board.h"

#define RelayOpen   0xff
#define RelayClose  0x00

typedef struct RelayStruct_
{
    GPIO_TypeDef *  GPIOx;
    uint16_t        GPIO_PIN;
}RelayStruct;

typedef struct REPD_Typedef_
{
	RelayStruct RelayArray[6];
	uint8_t RelayState[6];
}REPD_Typedef,*p_REPD_Typedef;

extern deviceModule relay;

result SetRelayState(uint8_t Num,uint8_t State);
uint8_t * GetRelayState(uint8_t Num);

























#endif

































