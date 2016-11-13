
#ifndef _DATATRANSFER_H_
#define _DATATRANSFER_H_

#include "stm32f1xx_hal.h"

typedef enum _frame_type
{
    none = 0,		//无
	operation,		//操作帧
	data_tran,		//数据传输帧
	alarm,			//警告帧
	sync,			//同步帧
	ack				//应答
}frame_type;


typedef struct _dataframe
{
    uint8_t     Key;			//解密种子
    uint16_t    crc;			//CRC校验
	frame_type  type;			//操作(OPERATION)+传输(DATATRAN)+报警(ALARM)+同步(SYNC)+应答(ACK)
	//uint8_t     level;			//级别
	uint8_t     sender_channel;
	uint8_t     recevier_channel;
	uint8_t     data_length;//数据长度
	uint8_t     databuf;	
}dataframe,*p_dataframe;

typedef struct _tranops
{
	uint8_t (*send_alarm)(uint8_t *,uint8_t);
	uint8_t (*send_sync) (uint8_t *,uint8_t);
	uint8_t (*send_datatran)(uint8_t *,uint8_t,uint8_t);
	uint8_t (*send_operation)(uint8_t *,uint8_t);
	uint8_t (*data_recv_process)(void);
    uint8_t (*CheckReceive)(void);
    uint8_t (*send_sync_time)(uint8_t);
    uint8_t (*sync_send_TH)(uint8_t);
}tranops,*p_tranops;

extern tranops ops;

extern uint8_t rx_buffer[64],tx_buffer[64];

uint8_t send_data(frame_type type,uint8_t recevier_channel,uint8_t * inf,uint8_t length);
uint8_t data_recv_process(void);
uint8_t send_alarm(uint8_t * inf,uint8_t recevier_channel);
uint8_t send_sync(uint8_t * inf,uint8_t recevier_channel);
uint8_t send_sync_time(uint8_t recevier_channel);
uint8_t sync_send_TH(uint8_t recevier_channel);
uint8_t send_datatran(uint8_t * data,uint8_t length,uint8_t recevier_channel);
uint8_t send_operation(uint8_t * inf,uint8_t recevier_channel);












#endif




























