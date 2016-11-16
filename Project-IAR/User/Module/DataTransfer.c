
#include "Board.h"

uint8_t rx_buffer[64] = {0x00};
uint8_t tx_buffer[64] = {0x00};
p_dataframe frame_rx = (p_dataframe)rx_buffer;
p_dataframe frame_tx = (p_dataframe)tx_buffer;

/*
****************************************************
*  Function       : ����Э�����ݰ�
*  Description    : ������������
*  Calls          : 
*  Called By      : 
*  Input          : type:֡����
                    recevier_channel:֡�������ַ
                    inf:��Ϣ����
                    length:��Ϣ����
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
uint8_t send_data(frame_type type,uint8_t recevier_channel,uint8_t * inf,uint8_t length)
{
    uint8_t cnt = 0,* buf = 0;
    //�������
    board.frame_tx->type = type;
    board.frame_tx->recevier_channel = recevier_channel;
    board.frame_tx->data_length = length;
    board.frame_tx->sender_channel = board.board_Local_channel;
    //��������
    buf = &board.frame_tx->databuf;
    for(cnt = 0;cnt < length;cnt ++)buf[cnt] = inf[cnt];
    //����Ǹ��Լ����͵�,��ֱ�ӵ��ô���
    if(recevier_channel == CUR_BOARD)
    {
        for(cnt = 0;cnt < 64;cnt ++)rx_buffer[cnt] = tx_buffer[cnt];
        data_recv_process();
        return true;
    }
    //��������
    CRC_JIAMI((uint8_t *)tx_buffer);
    /* �������� */
    if(si4463.d_puts(recevier_channel,tx_buffer,64)== true)return true;
    else return false;
}
/*
****************************************************
*  Function       : ����Ƿ��յ�����
*  Description    : ���������ݣ����ݻᱣ����frame_rx��
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
*  Function       : ���;���
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : inf:��������
                    recevier_channel:������
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
uint8_t send_alarm(uint8_t * inf,uint8_t recevier_channel)
{
    /*
    ��������+����+��ֵ+����
    "ALARM-WET-80-LOW-"
    
    
    
    */
    uint8_t length = strlen((char const *)inf);
    return send_data(alarm,recevier_channel,inf,length);
}
/*
****************************************************
*  Function       : ����ͬ����Ϣ
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : inf:ͬ������
                    recevier_channel:������
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
uint8_t send_sync(uint8_t * inf,uint8_t recevier_channel)
{
    
    uint8_t cnt = 0,ChannelTemp = 0xff;
    uint8_t length = strlen((char const *)inf);

    //���Ƶ��Ϊ0xff������ѵ,���򵥷�
    
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
*  Function       : �������ݴ���
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : inf:Ӧ������
                    length:���ݳ���
                    recevier_channel:������
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
*  Function       : ���Ͳ���ָ��
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : inf:Ӧ������
                    recevier_channel:������
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
*  Function       : �������ݴ�������
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : ��Ϣֻ��ʹ��һ��(�ᱻ���)
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
    //��Ϣֻ��ʹ��һ��
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



















