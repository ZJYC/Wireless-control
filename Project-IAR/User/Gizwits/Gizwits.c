
/*

	主要为串口指令解析
	

*/

#include "board.h"

extern InfSet InfSet_1[30];

/*
****************************************************
*  Function       : GizwitsInit
*  Description    : 机智云初始化（主要是串口初始化）
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*  Author         : 
*****************************************************
*/
result GizwitsInit(void)
{
	usart_2.d_open();
    //usart_2.d_puts(0,"USART Init Finished...\r\n",0);
    return true;
}
/*
****************************************************
*  Function       : GizwitsExecDirective
*  Description    : 执行指令（从串口接受指令并执行）
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*  Author         : 
*****************************************************
*/
result GizwitsExecDirective(void)
{
	uint8_t BufTemp[4][16] = {0x00},DirectiveCnt = 0,BufTempCnt = 0,*TempDirective = 0;
    
	//because this function was called timing,so the buf need to be static...FUCK!!!
    
	static uint8_t Buf[64] = {0x00};//缓冲随即被破坏
	static uint8_t Buf_BackUp[64] = {0x00};//缓冲随即被破坏
	
	if(GizwitsRecv(PORT_0,Buf) == false)return false;
    
	strcpy((char *)Buf_BackUp,(char *)Buf);
	
	TempDirective = Buf;
	for(DirectiveCnt = 0;DirectiveCnt < DirectiveMaxLen;DirectiveCnt ++)
	{
		if(Buf[DirectiveCnt] == 0x00)break;
		if(Buf[DirectiveCnt] == '-')
		{
			Buf[DirectiveCnt] = 0x00;
			strcpy((char *)BufTemp[BufTempCnt++],(char *)TempDirective);
			TempDirective = &Buf[DirectiveCnt] + 1;
			if(BufTempCnt >= 4)break;
		}
	}
	//Relay_0_0-OPEN-
	if(strcmp((char const *)BufTemp[0],"Relay_0") == 0)board.ops->send_operation(Buf_BackUp,CHANNEL_0);
	if(strcmp((char const *)BufTemp[0],"Relay_1") == 0)board.ops->send_operation(Buf_BackUp,CHANNEL_1);
	if(strcmp((char const *)BufTemp[0],"Relay_2") == 0)board.ops->send_operation(Buf_BackUp,CHANNEL_2);
	if(strcmp((char const *)BufTemp[0],"Relay_3") == 0)board.ops->send_operation(Buf_BackUp,CHANNEL_3);
    
	return true;
}
/*
****************************************************
*  Function       : GizwitsSync
*  Description    : 同步控制板信息到机智云
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*  Author         : 
*****************************************************
*/
result GizwitsSync(void)
{
	uint8_t Temp[32] = {0x00},cnt = 0;
	for(cnt = 0;cnt < 30;cnt ++)
	{
		if(InfSet_1[cnt].Used == true)
		{
			strcpy((char *)Temp,(char const*)InfSet_1[cnt].Name);
			strcat((char *)Temp,(char const *)InfSet_1[cnt].Param);
            strcat((char *)Temp,"-");
			if(GizwitsSend(PORT_1,Temp) == false)
            {
                cnt --;
                osDelay(100);
            }
		}
	}
	return true;
}

result GizwitsSend(uint8_t Port,uint8_t * Buf)
{
    uint8_t ACK[10] = {0x00},RetryCnt = 0;
    uint16_t Temp = 0;

    SendRetry:
    
    //osMutexWait(Usart2TxMutex,osWaitForever);
        
    usart_2.d_puts(Port,Buf,0);
	
    usart_2.d_gets(Port,ACK,(uint32_t)&Temp);
    
    //osMutexRelease(Usart2TxMutex);
    
    if(strcmp((char const *)ACK,"OK") == 0)
    {
        return true;
    }
    else
    {
        osDelay(100);
        RetryCnt++;
        if(RetryCnt >= 4)
        {
            RetryCnt = 0;
            return false;
        }
        goto SendRetry;
    }
}

result GizwitsRecv(uint8_t Port,uint8_t * Buf)
{
    result res = false;
    
    uint16_t Temp = 0;
    
    //osMutexWait(Usart2Mutex,osWaitForever);
    
	res = usart_2.d_gets(Port,Buf,(uint32_t)&Temp);
	
    if(res == true)
    {
        usart_2.d_puts(Port,"OK",0);
        //osMutexRelease(Usart2Mutex);
        return true;
    }
    else
    {
        //osMutexRelease(Usart2Mutex);
        return false;
    }

    
    
}

void GizwitsTimingProcess(void)
{
    usart_2.d_timing_proceee(20,0,0);
}







































