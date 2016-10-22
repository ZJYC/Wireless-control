/*
****************************************************
*  File name          : SerialDriver_Bare.c
*  Author             : 5A4A5943
*  Version            : V1.0.0
*  Date               : 2016--10--10--22--47--11
*  Description        : Serial driver's API for no-operation system
*  Function List      : 
						SerialInit_Bare
						SerialSend_Bare
						SerialRecv_Bare
*  History            : 
*****************************************************
*/
#include "SerialDriver.h"

uint8_t SerialInit_Bare(
					pSerialTCBTypedef TCB,
					uint8_t (*SerialInit)(void),
					uint8_t (*SendDataIT)(uint8_t  ),
					uint8_t (*RecvDataIT)(uint8_t *))
{
	return SerialInit_Basic(TCB,SerialInit,SendDataIT,RecvDataIT);
}

SerialResultTypedef SerialSend_Bare(
					pSerialTCBTypedef TCB,
					uint8_t IP,
					uint8_t Port,
					uint8_t *pData,
					uint16_t Len)
{
	return SerialSend_Basic(TCB,IP,Port,pData,Len);
}

SerialResultTypedef SerialRecv_Bare(pSerialTCBTypedef TCB,
					uint8_t IP,
					uint8_t Port,
					uint8_t *pData,
					uint16_t *Len)
{
	static SerialResultTypedef ExecuteSerialRecv_Once = SR_False;
	static uint16_t LenTemp = 0;
	
	if(ExecuteSerialRecv_Once == SR_False)
	{
		LenTemp = 0x00;
		SerialRecv_Basic(TCB,IP,Port,pData,&LenTemp,0x00);
		ExecuteSerialRecv_Once = SR_True;
	}
	
	if(LenTemp != 0x00)
	{
		*Len = LenTemp;
		ExecuteSerialRecv_Once = SR_False;
		return SR_True;
	}
	else
	{
		return SR_False;
	}
}

















