/*
****************************************************
*  File name          : SerialDriver_OS.c
*  Author             : 5A4A5943
*  Version            : V1.0.0
*  Date               : 2016--10--10--22--49--33
*  Description        : serial driver'API for OS
*  Function List      : 
						SerialInit_OS
						SerialSend_OS
						SerialRecv_OS
*  History            : 
*****************************************************
*/
#include "SerialDriver.h"

#define OS_Delay(x)	osDelay(x)


uint8_t SerialInit_OS(
					pSerialTCBTypedef TCB,
					uint8_t (*SerialInit)(void),
					uint8_t (*SendDataIT)(uint8_t  ),
					uint8_t (*RecvDataIT)(uint8_t *))
{
	return SerialInit_Basic(TCB,SerialInit,SendDataIT,RecvDataIT);
}

SerialResultTypedef SerialSend_OS(
					pSerialTCBTypedef TCB,
					uint8_t IP,
					uint8_t Port,
					uint8_t *pData,
					uint16_t Len)
{
	while(SerialSend_Basic(TCB,IP,Port,pData,Len) != SR_True)OS_Delay(20);
    return SR_True;
}

SerialResultTypedef SerialRecv_OS(pSerialTCBTypedef TCB,
					uint8_t IP,
					uint8_t Port,
					uint8_t *pData,
					uint16_t *Len)
{
	
	TaskHandle_t CurTaskHandle = xTaskGetCurrentTaskHandle();
	
	if(SerialRecv_Basic(TCB,IP,Port,pData,Len,(void *)&CurTaskHandle) != SR_True)
	{
		vTaskSuspend(NULL);
	}
    return SR_True;
}

SerialResultTypedef SerialNotifyHook(void * Signal)
{
	xTaskResumeFromISR((TaskHandle_t)(*(uint32_t*)Signal));
    return SR_True;
}












