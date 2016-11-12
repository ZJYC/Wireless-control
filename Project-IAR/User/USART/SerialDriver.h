/*
****************************************************
*  File name          : SerialDriver.h
*  Author             : 5A4A5943
*  Version            : V1.0.0
*  Date               : 2016--10--10--22--51--11
*  Description        : 
*  Function List      : 
*  History            : 
*****************************************************
*/

#ifndef __SERIALDRIVER_H__
#define __SERIALDRIVER_H__

#include "board.h"						//the header file for your own project

#define BufSize                         128//the Buffer size for both RX/TX,(BYTE)
#define SuspendListNum                  10//Suspend list amount

    
    #define IP_0	(0)
    #define IP_1	(1)
    #define IP_2	(2)
    #define IP_3	(3)
    #define IP_4	(4)
    #define IP_5	(5)
    #define IP_6	(6)

    #define PORT_0  (0)
    #define PORT_1  (1)
    #define PORT_2  (2)
    #define PORT_3  (3)
    #define PORT_4  (4)
    #define PORT_5  (5)
    #define PORT_6  (6)


#define DriverState_CLOSE			(1 << 0)
#define DriverState_OPEN			(1 << 1)
#define DriverState_BusyTx			(1 << 2)
#define DriverState_BusyRx			(1 << 3)
#define DriverState_TxBufAvailable	(1 << 4)
#define DriverState_RxBufAvailable	(1 << 5)

typedef enum SerialResultTypedef_
{
	
	SR_False = 0,				//no
    SR_True = 1,				//yes
	SR_Existed = 2,				//existed
	SR_Busy = 3,				//they were busy
	SR_Invalid = 4,				//the param was invalid(for example:pass 0x00 to a pointer)
	SR_SendCharFailed = 5		//send char func return a false
	
}SerialResultTypedef;

__packed typedef struct SuspendListTypedef_
{
	
	SerialResultTypedef Used;	//indicate whether the item was used
	uint8_t IP;					//IP address
	uint8_t Port;				//Port number
	uint8_t Reserve;			//not used currently
	uint8_t *Buf;               //point to the buffer which will storage the recevied data
	uint16_t *Len;              //pointer to a 16-bit variable to indicate the length of recevied data
	void * Signal;              //In OS mode,It can storage the blocked task's handle(who call <SerialRecv_OS>)
	
}SuspendListTypedef,*pSuspendListTypedef;

__packed typedef struct SerialDataFrameTypedef_
{
	
	/*
		final data to send:
		[frame header(2B:0xFFFF)]+[IP(1B)]+[Port(1B)]+[dataLen(1B)]+[Data(nB)]+[Crc[2B]]
		if the data size was less than MARO<BufSize>,the Crc will be copied to the Data's rare
	*/
	
	uint8_t		Head[2];
	uint8_t		IP;			
	uint8_t		Port;
	uint8_t		DataLen;
	uint8_t		Buf[BufSize];
	uint16_t	Crc;
	
	/* The following was control attributes */
	uint16_t    FrameLen;
	uint16_t    Counter;
	uint8_t		FlagCrash;
	uint8_t		RecvFF;
	
} SerialDataFrameTypedef, *pSerialDataFrameTypedef;

typedef struct SerialTCBTypedef_
{
	SerialDataFrameTypedef DataFrameTx;				//the Frame for tx,It has a perspective space
	SerialDataFrameTypedef DataFrameRx;             //the Frame for rx,It has a perspective space
	uint8_t State;                                  //the TCB's State
	uint8_t (*SerialInit)(void);                    //
	uint8_t (*SendSingleCharIT)(uint8_t  Char);     //
	uint8_t (*RecvSingleCharIT)(uint8_t *Char);     //
}SerialTCBTypedef,*pSerialTCBTypedef;

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$底层$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ */
//串行驱动初始化
//TCB：控制块
//SendSingleCharIT：函数
//RecvSingleCharIT：函数
//返回：成功返回0
uint8_t SerialInit_Basic(
					pSerialTCBTypedef TCB,
					uint8_t (*SerialInit)(void),
					uint8_t (*SendDataIT)(uint8_t  ),
					uint8_t (*RecvDataIT)(uint8_t *));
//串行发送
//TCB：控制块
//IP：IP地址
//Port：端口
//pData：数据地址
//Len：数据长度
//返回：成功返回0
SerialResultTypedef SerialSend_Basic(
					pSerialTCBTypedef TCB,
					uint8_t IP,
					uint8_t Port,
					uint8_t *pData,
					uint16_t Len);
//发送中断中要执行的函数
//TCB：控制块
//返回：意义不大
//
SerialResultTypedef SerialSendProcessIT(pSerialTCBTypedef TCB);
//串行接收
//TCB：控制块
//IP：IP地址
//Port：端口
//pData：数据接收地址
//Len：数据长度
//Callback:收到数据的回调函数
//返回：成功返回0
SerialResultTypedef SerialRecv_Basic(pSerialTCBTypedef TCB,
					uint8_t IP,
					uint8_t Port,
					uint8_t *pData,
					uint16_t *Len,
					void * Signal);
//接收中断中调用的函数
//TCB：控制块
//返回：意义不大
SerialResultTypedef SerialRecvProcessIT(pSerialTCBTypedef TCB);

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$操作系统$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ */

uint8_t SerialInit_OS(
					pSerialTCBTypedef TCB,
					uint8_t (*SerialInit)(void),
					uint8_t (*SendDataIT)(uint8_t  ),
					uint8_t (*RecvDataIT)(uint8_t *));
SerialResultTypedef SerialSend_OS(
					pSerialTCBTypedef TCB,
					uint8_t IP,
					uint8_t Port,
					uint8_t *pData,
					uint16_t Len);
SerialResultTypedef SerialRecv_OS(pSerialTCBTypedef TCB,
					uint8_t IP,
					uint8_t Port,
					uint8_t *pData,
					uint16_t *Len);
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$裸机$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ */
uint8_t SerialInit_Bare(
					pSerialTCBTypedef TCB,
					uint8_t (*SerialInit)(void),
					uint8_t (*SendDataIT)(uint8_t  ),
					uint8_t (*RecvDataIT)(uint8_t *));
SerialResultTypedef SerialSend_Bare(
					pSerialTCBTypedef TCB,
					uint8_t IP,
					uint8_t Port,
					uint8_t *pData,
					uint16_t Len);
SerialResultTypedef SerialRecv_Base(pSerialTCBTypedef TCB,
					uint8_t IP,
					uint8_t Port,
					uint8_t *pData,
					uint16_t *Len);
                                    


















#endif
