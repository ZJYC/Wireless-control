
#include "Board.h"

uint8_t rx_buffer[64] = {0x00};
uint8_t tx_buffer[64] = {0x00};
p_dataframe frame_rx = (p_dataframe)rx_buffer;
p_dataframe frame_tx = (p_dataframe)tx_buffer;

/*
****************************************************
*  Function       : 发送协议数据包
*  Description    : 包含各种类型
*  Calls          : 
*  Called By      : 
*  Input          : type:帧类型
                    recevier_channel:帧接收这地址
                    inf:信息内容
                    length:信息长度
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
uint8_t send_data(frame_type type,uint8_t recevier_channel,uint8_t * inf,uint8_t length)
{
    uint8_t cnt = 0,* buf = 0;
    //数据填充
    board.frame_tx->type = type;
    board.frame_tx->recevier_channel = recevier_channel;
    board.frame_tx->data_length = length;
    board.frame_tx->sender_channel = board.board_Local_channel;
    //复制数据
    buf = &board.frame_tx->databuf;
    for(cnt = 0;cnt < length;cnt ++)buf[cnt] = inf[cnt];
    //如果是给自己发送的,则直接调用处理
    if(recevier_channel == CUR_BOARD)
    {
        for(cnt = 0;cnt < 64;cnt ++)rx_buffer[cnt] = tx_buffer[cnt];
        data_recv_process();
        return true;
    }
    //加密数据
    CRC_JIAMI((uint8_t *)tx_buffer);
    /* 发送数据 */
    if(si4463.d_puts(recevier_channel,tx_buffer,64)== true)return true;
    else return false;
}
/*
****************************************************
*  Function       : 检查是否收到数据
*  Description    : 如若有数据，数据会保存在frame_rx中
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*  Author         : 
*****************************************************
*/
uint8_t CheckReceive(void)
{
    if(si4463.d_gets(board.board_Local_channel,rx_buffer,64) == true)return true;
    return false;
}

/*
****************************************************
*  Function       : 发送警告
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : inf:警告内容
                    recevier_channel:接收者
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
uint8_t send_alarm(uint8_t * inf,uint8_t recevier_channel)
{
    /*
    数据类型+类型+数值+趋向
    "ALARM-WET-80-LOW-"
    
    
    
    */
    uint8_t length = strlen((char const *)inf);
    return send_data(alarm,recevier_channel,inf,length);
}
/*
****************************************************
*  Function       : 发送同步信息
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : inf:同步内容
                    recevier_channel:接收者
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
uint8_t send_sync(uint8_t * inf,uint8_t recevier_channel)
{
    
    uint8_t cnt = 0,ChannelTemp = 0xff;
    uint8_t length = strlen((char const *)inf);

    //如果频道为0xff，则轮训,否则单发
    
    if(recevier_channel == 0xff)
    {
        for(cnt = 0;cnt < MAX_DEVICE_NUM;cnt ++)
        {
            
            if(board.DeviceList[cnt].Used == true && board.DeviceList[cnt].Addr != ChannelTemp)
            {
                ChannelTemp = board.DeviceList[cnt].Addr;
                
                send_data(sync,board.DeviceList[cnt].Addr,inf,length);
            }
        }
        return true;
    }
    else
    {
        return send_data(sync,recevier_channel,inf,length);
    }
}

uint8_t send_sync_time(uint8_t recevier_channel)
{
    p_DS1307TimeTypedef DS1307Time = (p_DS1307TimeTypedef)DS1307.data;
    uint8_t string[64] = {0x00},temp[10] = {0x00};
    
    strcpy((char *)string,"TIME-20"); 
    sprintf((char *)temp,"%d-",DS1307Time->year);
    strcat((char *)string,(char *)temp);
    sprintf((char *)temp,"%d-",DS1307Time->mon);
    strcat((char *)string,(char *)temp);
    sprintf((char *)temp,"%d-",DS1307Time->day);
    strcat((char *)string,(char *)temp);
    sprintf((char *)temp,"%d-",DS1307Time->hour);
    strcat((char *)string,(char *)temp);
    sprintf((char *)temp,"%d-",DS1307Time->min);
    strcat((char *)string,(char *)temp);
    sprintf((char *)temp,"%d-",DS1307Time->sec);
    strcat((char *)string,(char *)temp);
    sprintf((char *)temp,"%d-",DS1307Time->wday);
    strcat((char *)string,(char *)temp);
    
    return send_sync(string,recevier_channel);
}

uint8_t sync_send_TH(uint8_t recevier_channel)
{
    uint8_t string[64] = {0x00},temp[10] = {0x00};
    SHT2x_PARAM * SHT2xData = (SHT2x_PARAM *)SHT20.data;
    
    strcpy((char *)string,"TH-");
    sprintf((char *)temp,"%4.2f-",SHT2xData->Temperature);
    strcat((char *)string,(char *)temp);
    sprintf((char *)temp,"%4.2f-",SHT2xData->Humidity);
    strcat((char *)string,(char *)temp);
    
    return send_sync(string,recevier_channel);
}

/*
****************************************************
*  Function       : 发送数据传输
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : inf:应答内容
                    length:数据长度
                    recevier_channel:接收者
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
uint8_t send_datatran(uint8_t * data,uint8_t length,uint8_t recevier_channel)
{
    return send_data(data_tran,recevier_channel,data,length);
}
/*
****************************************************
*  Function       : 发送操作指令
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : inf:应答内容
                    recevier_channel:接收者
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
uint8_t send_operation(uint8_t * inf,uint8_t recevier_channel)
{
    uint8_t length = strlen((char const *)inf);
    return send_data(operation,recevier_channel,inf,length);
}
/*
****************************************************
*  Function       : 接收数据处理流程
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 信息只能使用一次(会被清除)
*****************************************************
*/
uint8_t data_recv_process(void)
{
    switch(frame_rx->type)
    {
        case operation: {operation_handle();break;}
        case data_tran: {data_tran_handle();break;}
        case alarm:     {alarm_handle();break;}
        case sync:      {sync_handle();break;}
        case ack:       {ack_handle();break;}
        default:break;
    }
    //信息只能使用一次
    frame_rx->type = none;
    return true;
}


tranops ops = 
{
    send_alarm,
    send_sync,
    send_datatran,
    send_operation,
    data_recv_process,
    CheckReceive,
    send_sync_time,
    sync_send_TH
};



















