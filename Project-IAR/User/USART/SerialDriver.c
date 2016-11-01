/*$T indentinput.cpp GC 1.140 10/05/16 19:02:05 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

 Serial driver in embeded system,You cannot use it with a virtual OS atmosphere,because it use so many pointer to delieve data...
 
 How to use this driver?
 
 the first to say,this driver was easy and simple,you could follow several steps to use it:
 
 one:implement three functions
 
 <Port_SendSingleCharIT/Port_RecvSingleCharIT/Port_SerialInit>
 
 Port_SendSingleCharIT:send one character while the send interrupt opend
 Port_RecvSingleCharIT:Recv one character while the recv interrupt opend
 Port_SerialInit:init the GPIO/clock/NVIC,,,etc

 two:Declare a <SerialTCBTypedef> structure
 
 three:Use function"SerialInit"to init the structure
 
 four:implement interrupt calls
 
 Add <SerialRecvProcessIT>to the Recevive interrupt
 Add <SerialSendProcessIT>to the transmit interrupt
 
 five:Use the API
 Call function<SerialSend_Basic>to send data,
 Call function<SerialRecv_Basic>to recv data,
 note:function<SerialRecv_Basic>was in noblocked mode,so you need to check the return value,you'd better to call this function in a loop:
 
 while(SerialRecv_Basic(...) == SR_False);
 
 or
 
 while(SerialRecv_Basic(...) == SR_False)osDelay(20);
 
 
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */

/*

	why did I use IP and Port as a param?
	
	In real condition,When two MCUs communicate by SerialPort(such as USART),It is diffcult to identify that:who send the data and the data were sended to who,

*/ 

#include "SerialDriver.h"
//CRC check table
uint16_t	CRC16Table_SerialDriver[16]=
{
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7, 
	0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef
};
static uint8_t TempRx = 0x00;
//等待列表
SuspendListTypedef SuspendList[SuspendListNum] = {0x00};

//填加到等待列表
static SerialResultTypedef SuspendListAdd(
                        #if (EnableIP_Port)
						uint8_t IP,
						uint8_t Port,
                        #endif
						uint8_t * Buf,
						uint16_t * Len,
						void * Signal);
//从等待列表删除
//static SerialResultTypedef SuspendListDel(uint8_t IP,uint8_t Port);
//执行
static SerialResultTypedef SuspendListCheck(
                        #if (EnableIP_Port)
						uint8_t IP,
						uint8_t Port,
                        #endif
						uint8_t ** Buf,
						uint16_t ** Len,
						void ** Signal);
SerialResultTypedef SerialNotifyHook(void * Signal);

static SerialResultTypedef SerialNotify(pSerialTCBTypedef TCB);
//返回16位CRC校验结果
//Spr：数据首
//Len：数据长度
//返回：校验结果
static uint16_t CRC16_SerialDriver(uint8_t *Spr,int Len)
{
	uint16_t 	Crc=0;
	uint8_t		Temp;
	
	if(Spr == 0)return 0xFFFF;
	
	while(Len--!=0)
	{
		Temp=((uint8_t)(Crc>>8))>>4;
		Crc<<=4;
		Crc ^= CRC16Table_SerialDriver[Temp^(*Spr/16)];
		Temp=((unsigned char)(Crc/256)/16);
		Crc<<=4;
		Crc ^= CRC16Table_SerialDriver[Temp^(*Spr&0x0f)];
		Spr++;
	}
	return Crc;
}
//串行驱动初始化
//TCB：控制块
//SendSingleCharIT：函数
//RecvSingleCharIT：函数
//返回：成功返回0
uint8_t SerialInit_Basic(
					pSerialTCBTypedef TCB,
					uint8_t (*SerialInit)(void),
					uint8_t (*SendDataIT)(uint8_t  ),
					uint8_t (*RecvDataIT)(uint8_t *))
{
	
	if(SerialInit == 0)return 1;
	if(SendDataIT == 0)return 1;
	if(RecvDataIT == 0)return 1;
	
	TCB->State |= DriverState_OPEN;
	TCB->SendSingleCharIT = SendDataIT;
	TCB->RecvSingleCharIT = RecvDataIT;
	TCB->SerialInit = SerialInit;
	return TCB->SerialInit();
}


//串行发送
//TCB：控制块
//IP：IP地址
//Port：端口
//pData：数据地址
//Len：数据长度
//返回：成功返回0
SerialResultTypedef SerialSend_Basic(
					pSerialTCBTypedef TCB,
                    #if (EnableIP_Port)
					uint8_t IP,
					uint8_t Port,
                    #endif
					uint8_t *pData,
					uint16_t Len)
{
    uint8_t * pTemp8_t = 0;
    uint16_t * pTemp16_t = 0;
	
	if(TCB == 0)return SR_Invalid;
	if(pData == 0)return SR_Invalid;
	
	/* 当前忙，不能使用 */
	if(DriverState_BusyTx & TCB->State)return SR_Busy;
	TCB->State |= DriverState_BusyTx;
	
	TCB->DataFrameTx.Head[0] = 0xFF;
	TCB->DataFrameTx.Head[1] = 0xFF;
    #if (EnableIP_Port)
	TCB->DataFrameTx.IP = IP;
	TCB->DataFrameTx.Port = Port;
    #endif
	/* 如果长度为零，默认为字符串 */
	if(Len != 0)TCB->DataFrameTx.DataLen = Len;
	else TCB->DataFrameTx.DataLen = strlen((char const *)pData) + 1;
	/* 数据超限返回非零 */
	if(TCB->DataFrameTx.DataLen > BufSize || TCB->DataFrameTx.DataLen == 0)return SR_Invalid;
	memcpy(TCB->DataFrameTx.Buf,pData,TCB->DataFrameTx.DataLen);
	/* 计算整帧的CRC<并不是全部的Buf> */
	TCB->DataFrameTx.Crc = CRC16_SerialDriver((uint8_t *)&TCB->DataFrameTx,TCB->DataFrameTx.DataLen + 5);
	/* 帧长度=数据+头部(5)+CRC(2) */
	TCB->DataFrameTx.FrameLen = TCB->DataFrameTx.DataLen + 7;
	TCB->DataFrameTx.Counter = 0;
	TCB->DataFrameTx.FlagCrash = 0;
	/* CRC提前到数据尾部 */
    //pTemp16_t = (uint16_t *)&TCB->DataFrameTx[TCB->DataFrameTx.DataLen + 6];
    pTemp8_t = (uint8_t *)&TCB->DataFrameTx;
    pTemp16_t = (uint16_t *)&pTemp8_t[TCB->DataFrameTx.DataLen + 5];
	*pTemp16_t = TCB->DataFrameTx.Crc;
	
	/* 到此数据填充完成,开始发送第一个字节 */
	if(TCB->SendSingleCharIT(pTemp8_t[TCB->DataFrameTx.Counter ++]) == 0)
	{
		//TCB->DataFrameTx.Counter += 1;
		return SR_True;
	}
	return SR_SendCharFailed;
}
//发送中断中要执行的函数
//TCB：控制块
//返回：意义不大
//
SerialResultTypedef SerialSendProcessIT(pSerialTCBTypedef TCB)
{
	uint8_t * Buf = 0x00;
	
	if(TCB == 0)return SR_Invalid;
	
	Buf = (uint8_t *)&TCB->DataFrameTx;
	
	if(DriverState_BusyTx & TCB->State)
	{
		if(TCB->DataFrameTx.Counter == TCB->DataFrameTx.FrameLen)
		{
			//发送完成
			TCB->State &= ~DriverState_BusyTx;
			return SR_True;
		}
		
		if(TCB->DataFrameTx.Counter >= 2)//非帧头部分
		{
			if(Buf[TCB->DataFrameTx.Counter] == 0xFF && TCB->DataFrameTx.FlagCrash == 0x00)
			{
				if(TCB->SendSingleCharIT(Buf[TCB->DataFrameTx.Counter]) == 0)
				{
					TCB->DataFrameTx.Counter += 1;
					TCB->DataFrameTx.FlagCrash = 0xFF;
					return SR_True;
				}
			}
			if(TCB->DataFrameTx.FlagCrash == 0xff)
			{
				if(TCB->SendSingleCharIT(0x55) == 0)
				{
					TCB->DataFrameTx.FlagCrash = 0x00;
					return SR_True;
				}
			}
			if(TCB->SendSingleCharIT(Buf[TCB->DataFrameTx.Counter]) == 0)
			{
				TCB->DataFrameTx.Counter += 1;
				return SR_True;
			}
			return SR_True;
		}
		else//帧头部分
		{
			if(TCB->SendSingleCharIT(Buf[TCB->DataFrameTx.Counter]) == 0)
			{
				TCB->DataFrameTx.Counter += 1;
				return SR_True;
			}
		}
	}
    return SR_True;
}

//串行接收
//TCB：控制块
//IP：IP地址
//Port：端口
//pData：数据接收地址
//Len：数据长度
//Callback:收到数据的回调函数
//返回：成功返回0
SerialResultTypedef SerialRecv_Basic(pSerialTCBTypedef TCB,
                    #if (EnableIP_Port)
					uint8_t IP,
					uint8_t Port,
                    #endif
					uint8_t *pData,
					uint16_t *Len,
					void * Signal)
{
	
	//SerialResultTypedef SerialResult = SR_False;
	
	if(TCB == 0)return SR_Invalid;
	if(pData == 0)return SR_Invalid;
	if(Len == 0)return SR_Invalid;
	//if(Signal == 0)return SR_Invalid;//the signal can be 0x00
  
	if(!(TCB->State & DriverState_BusyRx))
	{
		TCB->State |= DriverState_BusyRx;
		//this step was optional,Because the HAL driver was too...
		//TCB->RecvSingleCharIT(&TempRx);
	}
    TCB->RecvSingleCharIT(&TempRx);
	//add such information to the pending list
	//SerialResult = 
    SuspendListAdd(
                    #if (EnableIP_Port)
                    IP,Port,
                    #endif
                    pData,Len,Signal);
	//we judge whether we recevie data by the Len,So the first step is to clear the Len,
	//this step only execute once
	//if(SerialResult == SR_True){*Len = 0;}
	
	return SR_False;
}
//接收中断中调用的函数
//TCB：控制块
//返回：意义不大
SerialResultTypedef SerialRecvProcessIT(pSerialTCBTypedef TCB)
{
	uint8_t * Frame = 0x00;
	uint16_t * pTemp16 = 0x00;
	
	if(TCB == 0)return SR_Invalid;
	
	Frame = (uint8_t *)&TCB->DataFrameRx;
	
	if(TCB->DataFrameRx.Counter >= BufSize + 7)TCB->DataFrameRx.Counter = 0;
	
	//if(DriverState_BusyRx & TCB->State)
	{
		if(TCB->RecvSingleCharIT(&TempRx) == 0)
		{
			if(TCB->DataFrameRx.RecvFF == 1)
			{
				if(TempRx == 0x55)
				{
					TCB->DataFrameRx.RecvFF = 0;
					return SR_True;
				}
				if(TempRx == 0xFF)
				{
					TCB->DataFrameRx.Counter = 2;
					TCB->DataFrameRx.RecvFF = 0;
					Frame[0] = 0xFF;
					Frame[1] = 0xFF;
					return SR_True;
				}
                TCB->DataFrameRx.RecvFF = 0;
				return SR_True;
			}
			
			if(TempRx == 0xFF)TCB->DataFrameRx.RecvFF += 1;
			else TCB->DataFrameRx.RecvFF = 0;

			Frame[TCB->DataFrameRx.Counter] = TempRx;
			TCB->DataFrameRx.Counter += 1;
			
			if(Frame[0] == 0xFF && 
			Frame[1] == 0xFF && 
			TCB->DataFrameRx.Counter == TCB->DataFrameRx.DataLen + 7)
			{
                pTemp16 = (uint16_t *)&Frame[TCB->DataFrameRx.DataLen + 5];
                TCB->DataFrameRx.Crc = *pTemp16;
				if(TCB->DataFrameRx.Crc == CRC16_SerialDriver((uint8_t *)&TCB->DataFrameRx,TCB->DataFrameRx.DataLen + 5))
				{
					//here we recevie a frame successful
					TCB->State &= ~DriverState_BusyRx;
					TCB->State |= DriverState_RxBufAvailable;
					SerialNotify(TCB);
				}
			}
		}
		return SR_True;
	}
    //else
    {
        //I have no choice......The HAL was too FUCK...
        //TCB->RecvSingleCharIT(&TempRx);
        //return SR_False;
    }
    return SR_True;
}

static SerialResultTypedef SerialNotify(pSerialTCBTypedef TCB)
{
	uint16_t Cnt = 0;
	
	uint8_t * Buf = 0x00;
	uint16_t * Len = 0x00;
	void * Signal = 0x00;
	
	if(TCB == 0)return SR_Invalid;
	
	//check whether there is a correspond suspendlist,
	if(SuspendListCheck(
                            #if (EnableIP_Port)
                            TCB->DataFrameRx.IP,TCB->DataFrameRx.Port,
                            #endif
                            &Buf,&Len,&Signal) == SR_True)
	{
		if(Buf != 0 && Len != 0)
		{
			//get the lengh
			//in bare mode,this indicate that we have recevied successful,because 0-size data was invalid
			*Len = TCB->DataFrameRx.DataLen;
			//copy the data to user buffer
			for(Cnt = 0;Cnt < *Len;Cnt ++)
			{
				Buf[Cnt] = TCB->DataFrameRx.Buf[Cnt];
			}
			//in OS mode,these step will aware the blocked task,the task handle was passed by 'Signal'
			//before that,you should implement <SerialNotifyHook>,which was declared with Keyword '__weak'
			if(Signal != 0x00)
			{
				SerialNotifyHook(Signal);
			}
		}
	}
	else
	{
		//if the data was not wanted,you can add your own code here to address these data
	}
    return SR_True;
}

__weak SerialResultTypedef SerialNotifyHook(void * Signal)
{
	
	if(Signal == 0)return SR_Invalid;
	
	Signal = Signal;
	return SR_True;
}

/*
	why did I use usupend list??

	

*/
//填加到等待列表
static SerialResultTypedef SuspendListAdd(
                        #if (EnableIP_Port)
						uint8_t IP,
						uint8_t Port,
                        #endif
						uint8_t * Buf,
						uint16_t * Len,
						void * Signal)
{
	uint8_t Index = 0;
	
	if(Buf == 0)return SR_Invalid;
	if(Len == 0)return SR_Invalid;
	//the signal can be 0x00
	//if(Signal == 0)return SR_Invalid;
	
	for(Index = 0;Index < SuspendListNum;Index ++)
	{
		if(SuspendList[Index].Used == SR_True && 
		SuspendList[Index].IP == IP && 
		SuspendList[Index].Port == Port)
		{
			return SR_Existed;
		}
	}
	for(Index = 0;Index < SuspendListNum;Index ++)
	{
		if(SuspendList[Index].Used == SR_False)
		{
			SuspendList[Index].Used = SR_True;
			SuspendList[Index].IP = IP;
			SuspendList[Index].Port = Port;
			SuspendList[Index].Buf = Buf;
			SuspendList[Index].Len = Len;
			SuspendList[Index].Signal = Signal;
			return SR_True;
		}
	}
	
	return SR_False;
}
//从等待列表删除
//static SerialResultTypedef SuspendListDel(uint8_t IP,uint8_t Port)
//{
//	uint8_t Index = 0;
//	
//	for(Index = 0;Index < SuspendListNum;Index ++)
//	{
//		if(SuspendList[Index].Used == SR_True && 
//		SuspendList[Index].IP == IP && 
//		SuspendList[Index].Port == Port)
//		{
//			SuspendList[Index].Used = SR_False;
//			return SR_True;
//		}
//	}
//
//	return SR_False;
//}
//执行
static SerialResultTypedef SuspendListCheck(
						uint8_t IP,
						uint8_t Port,
						uint8_t ** Buf,
						uint16_t ** Len,
						void ** Signal)
{
	
	uint8_t Index = 0;
	
	if(Buf == 0)return SR_Invalid;
	if(Len == 0)return SR_Invalid;
	if(Signal == 0)return SR_Invalid;
	
	for(Index = 0;Index < SuspendListNum;Index ++)
	{
		if(SuspendList[Index].Used == SR_True && 
		SuspendList[Index].IP == IP && 
		SuspendList[Index].Port == Port)
		{
			//返回信息
			*Buf = SuspendList[Index].Buf;
			*Len = SuspendList[Index].Len;
            *Signal = SuspendList[Index].Signal;
			//本条等待列表失效
			SuspendList[Index].Used = SR_False;
			return SR_True;
		}
	}
	
	return SR_False;
	
}









