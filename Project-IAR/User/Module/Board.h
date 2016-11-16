
#ifndef _BOARD_H_
#define _BOARD_H_

#include "DataTransfer.h"
#include "DeviceModule.h"
#include "stm32f1xx_hal.h"
#include "string.h"
#include "spi.h"
#include "iwdg.h"
#include "FreeRTOS.h"
#include "Alarm.h"
#include "SHT2x.h"
#include "typedefs.h"
#include "DS1307.h"
#include "task.h"
#include "Key.h"
#include <stdlib.h>
#include <math.h>
#include "si446x.h"
#include "radio_config_Si446x_1k_26M.h"
#include "relay.h"
#include "RF4463.h"
#include "CRC_JIAMI.h"
#include "cmsis_os.h"
#include "UsartDriver.h"
#include "Gizwits.h"
#include "SerialDriver.h"
#include "Handle.h"
#include "Gizwits.h"
#include "HMI.h"
#define u8          uint8_t
#define u16         uint16_t
#define u32         uint32_t

#define CHANNEL_0   0
#define CHANNEL_1   1
#define CHANNEL_2   2
#define CHANNEL_3   3
#define CHANNEL_4   4
#define CHANNEL_5   5
#define CHANNEL_6   6
#define CHANNEL_7   7
#define CHANNEL_8   8
#define CHANNEL_9   9
#define MAIN_BOARD      CHANNEL_0
#define BOARD_1         CHANNEL_1
#define BOARD_2         CHANNEL_2
#define BOARD_3         CHANNEL_3
#define BOARD_4         CHANNEL_4
//��Ҫ�Զ���
#define CUR_BOARD           MAIN_BOARD
#define MAX_DEVICE_NUM      40          //�����������ⲿ40������
#define INFSETNUM           30
//�豸�б�(��Ҫ���������������)
//�������ɣ�ʵ����Ϣ��ת����
typedef struct DeviceListAll_
{
    uint8_t Used;                       //�Ƿ�ʹ��
    uint8_t Addr;                       //����ַ
    uint8_t TTL;                        //����ʱ��
    uint8_t DeviceName[8];              //��������
}DeviceListAll,*p_DeviceListAll;

//����һ������
typedef struct _boardstruct
{
    uint8_t board_name[16];                     //���ӵ�����
    uint8_t board_level;                        //���ӵĵȼ�
    uint8_t board_Local_channel;                //���ӱ��ص�ַ
    p_deviceModule device_list;                 //��������������б�
    p_DeviceListAll DeviceList;                 //���������б�
    p_dataframe frame_rx;                       //����Ľ��ջ�����
    p_dataframe frame_tx;                       //����ķ��ͻ�����
    p_tranops ops;                              //����Ĳ�������
}boardstruct;

typedef struct InfSet_
{
    result Used;
    uint8_t Name[32];//��Ϣ����
    uint8_t Param[32];//��Ϣ����
}InfSet,*p_InfSet;

extern boardstruct board;
extern osMutexId SI4463Mutex;
extern osMutexId AlarmMutex;
extern osMutexId Usart2TxMutex;
extern osMutexId Usart2RxMutex;

uint8_t GetBoardAddr(uint8_t Num);
void board_init(void);
void board_sync_device(void);
uint8_t board_device_list_add(uint8_t new_channel,uint8_t * DeviceName,uint8_t TTL);
uint8_t board_device_list_opera(void);
uint8_t board_device_list_exit(uint8_t * name,uint8_t * Channel);
result InfSetAdd(uint8_t * Name,uint8_t * Param);


#endif

