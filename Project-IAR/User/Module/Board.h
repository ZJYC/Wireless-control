
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
//需要自定义
#define CUR_BOARD           MAIN_BOARD
#define MAX_DEVICE_NUM      40          //最多允许添加外部40个器件
#define INFSETNUM           30
//设备列表(主要用于添加外板的器件)
//存在理由：实现消息中转机制
typedef struct DeviceListAll_
{
    uint8_t Used;                       //是否被使用
    uint8_t Addr;                       //外板地址
    uint8_t TTL;                        //生存时间
    uint8_t DeviceName[8];              //器件名称
}DeviceListAll,*p_DeviceListAll;

//描述一个板子
typedef struct _boardstruct
{
    uint8_t board_name[16];                     //板子的名称
    uint8_t board_level;                        //板子的等级
    uint8_t board_Local_channel;                //板子本地地址
    p_deviceModule device_list;                 //本板的所有器件列表
    p_DeviceListAll DeviceList;                 //外板的器件列表
    p_dataframe frame_rx;                       //本板的接收缓冲区
    p_dataframe frame_tx;                       //本板的发送缓冲区
    p_tranops ops;                              //本板的操作函数
}boardstruct;

typedef struct InfSet_
{
    result Used;
    uint8_t Name[32];//信息名称
    uint8_t Param[32];//信息内容
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

