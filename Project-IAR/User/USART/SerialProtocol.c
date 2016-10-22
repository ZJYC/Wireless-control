
#include "SerialDriver.h"

#define SerialSend_X(a,b,c,d,e)	SerialSend_OS((a),(b),(c),(d),(e))
#define SerialRecv_X(a,b,c,d,e)	SerialRecv_OS((a),(b),(c),(d),(e))

SerialResultTypedef SerialProtocolSend(uint8_t IP,uint8_t Port,uint8_t * Buf,uint8_t Len)
{
	
	if(Buf == 0)return SR_False;
	
	if(SerialSend_X()
	
}





















