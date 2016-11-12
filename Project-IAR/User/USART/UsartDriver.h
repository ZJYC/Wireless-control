
#ifndef __USARTDRIVER_H__
#define __USARTDRIVER_H__

#define TxBufMaxLen	128
#define RxBufMaxLen	128

#define TxIt		1
#define RxIt		0

#define StateTXing	(1 << 0)/* 正在发送 */
#define StateRXing	(2 << 1)/* 正在接收 */		

typedef struct UsartTcbStruct_
{
	
	uint8_t State;
	UART_HandleTypeDef * Usart;
	
	uint8_t TxBuf[TxBufMaxLen];
	uint8_t RxBuf[RxBufMaxLen];

	uint16_t TxBufCapacity;
	uint16_t RxBufCapacity;
	
	uint16_t TxBuf_W_Index;
	uint16_t TxBuf_R_Index;
	uint16_t RxBuf_W_Index;
	uint16_t RxBuf_R_Index;
	
	uint32_t PrivateCounter;
	uint32_t Valve;
	result   FlagReceved;
	result	 IntervalCpt;
	
}UsartTcbStruct,*p_UsartTcbStruct;
typedef struct Uart2PrivateTypedef_
{
    uint8_t Uart2Buf[100];
    uint8_t Uart2BufIndex;
    uint8_t Recvd;
}Uart2PrivateTypedef,*p_Uart2PrivateTypedef;

extern deviceModule usart_1;
extern deviceModule usart_2;





















#endif


























