
#ifndef _DATATRANSFER_H_
#define _DATATRANSFER_H_

#include "stm32f1xx_hal.h"

typedef enum _frame_type
{
    none = 0,		//��
	operation,		//����֡
	data_tran,		//���ݴ���֡
	alarm,			//����֡
	sync,			//ͬ��֡
	ack				//Ӧ��
}frame_type;


typedef struct _dataframe
{
    uint8_t     Key;			//��������
    uint16_t    crc;			//CRCУ��
	frame_type  type;			//����(OPERATION)+����(DATATRAN)+����(ALARM)+ͬ��(SYNC)+Ӧ��(ACK)
	//uint8_t     level;			//����
	uint8_t     sender_channel;
	uint8_t     recevier_channel;
	uint8_t     data_length;//���ݳ���
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




























