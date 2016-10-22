
#include "Board.h"

/*
****************************************************
*  Function       : operation_handle
*  Description    : 操作指令“relay1-1-OPEN-”
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*  Author         : ZJYC
*****************************************************
*/
void operation_handle(void)
{
    uint8_t Buf[10][8] = {0x00},i = 0,cnt_1 = 0,* Index = 0,* temp;
	
	uint8_t Addr,BitNun;
    
	temp =  &board.frame_rx->databuf;
    
    p_deviceModule device = 0x00;
    
    Index = temp;
	
    //将字符串拆解到子字符串中
    
	for(i = 0;i < board.frame_rx->data_length;i ++)
	{

		if(temp[i] == '-')
        {
            temp[i] = 0x00;
            strcpy((char *)Buf[cnt_1++],(char const*)Index);
            Index = &temp[i] + 1;
        }
	}

	//获取位号
	BitNun = atoi((char const *)Buf[1]);
	//从本地寻找相应设备
	device = device_find_local(Buf[0]);
	if(device != 0x00)
	{
		//打开设备并执行命令
		device->d_command(Buf[2],BitNun);
	}
    else//不在本地
    {
        if(board_device_list_exit(Buf[0],&Addr) == true)
        {
            
            //RF4463Repeat(0,Addr,board.frame_rx->databuf);
			
			board.ops->send_operation(&board.frame_rx->databuf,Addr);
        }
    }
        
}
void data_tran_handle(void)
{
	
}
void alarm_handle(void)
{
	
}
/*
****************************************************
*  Function       : sync_handle
*  Description    : 同步指令"TIME-...","TH-...","DEVICE-..."
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*  Author         : 
*****************************************************
*/
void sync_handle(void)
{
	//"NEW-4-"
    //最大容下10个子字符串,子字符串长度最大8
    uint8_t Buf[10][8] = {0x00},i = 0,cnt_1 = 0,* Index = 0,* temp,BufBackup[64] = {0x00};
	temp =  &board.frame_rx->databuf;
    
	strcpy((char *)BufBackup,(char const *)temp);
	
    Index = temp;
	
    //将字符串拆解到子字符串中
    
	for(i = 0;i < board.frame_rx->data_length;i ++)
	{

		if(temp[i] == '-')
        {
            temp[i] = 0x00;
            strcpy((char *)Buf[cnt_1++],(char const *)Index);
            Index = &temp[i] + 1;
        }
	}
    
    //以下开始判断同步内容
    
    if(strcmp((char const *)Buf[0],"TIME") == 0)
    {
        //时间同步
        Ds1307Time.year = atoi((char const *)Buf[1]) - 2000;
        
        Ds1307Time.mon = atoi((char const *)Buf[2]);
        
        Ds1307Time.day = atoi((char const *)Buf[3]);
        
        Ds1307Time.hour = atoi((char const *)Buf[4]);
        
        Ds1307Time.min = atoi((char const *)Buf[5]);
        
        Ds1307Time.sec = atoi((char const *)Buf[6]);
        
        Ds1307Time.wday = atoi((char const *)Buf[7]);
        
    }
    if(strcmp((char const *)Buf[0],"TH") == 0)
    {
		uint8_t Buf_1[32] = {0x00};
		sprintf((char *)Buf_1,"Temperature_%d-",board.frame_rx->sender_channel);
		InfSetAdd(Buf_1,Buf[1]);
		sprintf((char *)Buf_1,"Humidity_%d-",board.frame_rx->sender_channel);
		InfSetAdd(Buf_1,Buf[2]);
    }
    if(strcmp((char const *)Buf[0],"DEVICE") == 0)
    {
        //器件同步
        board_device_list_add(board.frame_rx->sender_channel,Buf[1],120);
    }
	if(strcmp((char const *)Buf[0],"GET") == 0)
	{
		uint8_t Buf[30] = {0x00};
		strcpy((char *)Buf,"SYNC-");
		//获取0板继电器"GET-Relay_x-x-"
		if(strcmp((const char * )Buf[1],(const char * )relay.name) == 0)
		{
			strcat((char *)Buf,(const char * )GetRelayState(atoi((const char * )Buf[2])));
			send_sync(Buf,board.frame_rx->sender_channel);
		}
		//获取温湿度"GET-TH-"
		if(strcmp((const char * )Buf[1],"TH") == 0)
		{
			sync_send_TH(board.frame_rx->sender_channel);
		}			
	}
	//"SYNC-Relay_x-x-OPEN- || SYNC-Relay_x-x-CLOSE-"
	if(strcmp((const char * )Buf[0],"SYNC") == 0)
	{
		uint8_t * Temp = &BufBackup[5];
		GizwitsSend(1,Temp);
	}
}
void ack_handle(void)
{
	
}




















