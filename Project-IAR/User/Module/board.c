
#include "Board.h"

//外部器件列表
DeviceListAll DeviceList[MAX_DEVICE_NUM] = {0x00};
//所有信息集合
InfSet InfSet_1[30] = {0x00};

#if (CUR_BOARD == MAIN_BOARD)
boardstruct board = 
{
	"board_0",
	255,
	CHANNEL_0,
	0x00,
    DeviceList,
	(p_dataframe)rx_buffer,
	(p_dataframe)tx_buffer,
	&ops
};
#endif

#if (CUR_BOARD == BOARD_1)
boardstruct board = 
{
	"board_1",
	200,
	CHANNEL_1,
	0x00,
    DeviceList,
	(p_dataframe)rx_buffer,
	(p_dataframe)tx_buffer,
    &ops
};
#endif

#if (CUR_BOARD == BOARD_2)
boardstruct board = 
{
	"board_2",
	200,
	CHANNEL_2,
	0x00,
    DeviceList,
	(p_dataframe)rx_buffer,
	(p_dataframe)tx_buffer,
    &ops
};
#endif

#if (CUR_BOARD == BOARD_3)
boardstruct board = 
{
	"board_3",
	200,
	CHANNEL_3,
	0x00,
    DeviceList,
	(p_dataframe)rx_buffer,
	(p_dataframe)tx_buffer,
    &ops
};
#endif

/*
****************************************************
*  Function       : board_sync_device
*  Description    : 同步本板器件"DEVICE-spi_1-"
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*  Author         : ZJYC
*****************************************************
*/
void board_sync_device(void)
{
	uint8_t buf[30] = {0x00},cnt = 0;
    p_deviceModule temp = board.device_list;
    //自己既是主板
    if(MAIN_BOARD == board.board_Local_channel)return;
    while(temp)
    {
        
        strcpy((char *)buf,"DEVICE-");
        strcat((char *)buf,(char *)temp->name);
        strcat((char *)buf,"-");
        retry_sync_device:
		if(temp->DA == Public)
		{
			if(board.ops->send_sync(buf,MAIN_BOARD) != true)
			{
				osDelay(cnt * 500 + board.board_Local_channel * 500);
				cnt ++;if(cnt > 20)cnt = 0;
				goto retry_sync_device;
			}
			cnt = 0;
		}
        temp = temp->next;
    }
}
/*
****************************************************
*  Function       : board_device_list_add
*  Description    : 添加外板器件
*  Calls          : 
*  Called By      : 
*  Input          : 
                    new_channel：外板地址
                    DeviceName：器件名称
                    TTL：生存时间
*  Output         : 
*  Return         : 
                    添加成功则返回真
*  Others         : 当前容量为40
*  Author         : ZJYC
*****************************************************
*/
uint8_t board_device_list_add(uint8_t new_channel,uint8_t * DeviceName,uint8_t TTL)
{
	uint8_t cnt = 0;
	//防止访问冲突
    taskENTER_CRITICAL();
	//此条信息已存在，直接更新
	for(cnt = 0;cnt < MAX_DEVICE_NUM;cnt ++)
	{
		if(board.DeviceList[cnt].Used == true)
		{
			if((strcmp((const char *)DeviceName,(const char *)board.DeviceList[cnt].DeviceName) == 0) && (new_channel == board.DeviceList[cnt].Addr))
			{
				board.DeviceList[cnt].TTL = TTL;
				taskEXIT_CRITICAL();
				return true;
			}
		}
	}
	//此条信息不存在，需要创建
	for(cnt = 0;cnt < MAX_DEVICE_NUM;cnt ++)
	{
		if(board.DeviceList[cnt].Used == false)
		{
			board.DeviceList[cnt].Used = true;
            board.DeviceList[cnt].Addr = new_channel,
            strcpy((char *)board.DeviceList[cnt].DeviceName,(char *)DeviceName);
            board.DeviceList[cnt].TTL = TTL;
            taskEXIT_CRITICAL();
			return true;
		}
	}
    taskEXIT_CRITICAL();
    return false;
}
/*
****************************************************
*  Function       : board_device_list_opera
*  Description    : 外板器件列表操作，主要是递减生存时间
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 建议1秒钟运行一次
*  Author         : ZJYC
*****************************************************
*/
uint8_t board_device_list_opera(void)
{
	uint8_t cnt = 0;
    taskENTER_CRITICAL();
	for(cnt = 0;cnt < MAX_DEVICE_NUM;cnt ++)
	{
		if(board.DeviceList[cnt].Used == true)
        {
            if(board.DeviceList[cnt].TTL == 0)
            {
                board.DeviceList[cnt].Used = false;
                continue;
            }
            board.DeviceList[cnt].TTL --;
        }      
	}
    taskEXIT_CRITICAL();
    return false;	
}

uint8_t board_device_list_exit(uint8_t * name,uint8_t * Channel)
{
    uint8_t cnt = 0;
    for(cnt = 0;cnt < MAX_DEVICE_NUM;cnt ++)
    {
		if(board.DeviceList[cnt].Used == true)
        {
            if(strcmp((const char *)board.DeviceList[cnt].DeviceName,(char *)name) == 0)
            {
                *Channel = board.DeviceList[cnt].Addr;
                return true;
            }
        }
    }
    return false;
}

result InfSetAdd(uint8_t * Name,uint8_t * Param)
{
	uint8_t cnt = 0;
	taskENTER_CRITICAL();
	//首先检查是否有这一条信息
	for(cnt = 0;cnt < INFSETNUM;cnt ++)
	{
		if(InfSet_1[cnt].Used == true)
		{
			if(strcmp((const char *)Name,(const char *)InfSet_1[cnt].Name) == 0)
			{
				strcpy((char *)InfSet_1[cnt].Param,(char *)Param);
				taskEXIT_CRITICAL();
				return true;
			}
		}
	}
	//这条信息不存在，则新建一个
	for(cnt = 0;cnt < INFSETNUM;cnt ++)
	{
		if(InfSet_1[cnt].Used == false)
		{
			InfSet_1[cnt].Used = true;
            strcpy((char *)InfSet_1[cnt].Name,(char *)Name);
			strcpy((char *)InfSet_1[cnt].Param,(char *)Param);
			taskEXIT_CRITICAL();
			return true;
		}
	}
	taskEXIT_CRITICAL();
	//失败...
	return false;
}

uint8_t * InfSetSearch(uint8_t * Name)
{
	uint8_t cnt = 0;
	
	for(cnt = 0;cnt < INFSETNUM;cnt ++)
	{
		if(InfSet_1[cnt].Used == true)
		{
			if(strcmp((const char *)Name,(const char *)InfSet_1[cnt].Name) == 0)
			{
				return InfSet_1[cnt].Param;
			}
		}
	}
	
	return 0x00;
}

uint8_t * InfSetGetOne(void)
{
	static uint8_t InfSetIndex = 0;
	//跳过空信息，循环完成返回0x00
	while(InfSet_1[InfSetIndex].Used != true)
	{
		InfSetIndex++;
		if(InfSetIndex >= INFSETNUM)
		{
			InfSetIndex = 0;
			return 0x00;
		}
	}

	return InfSet_1[InfSetIndex++].Param;
}

uint8_t GetBoardAddr(uint8_t Num)
{
	switch(Num)
	{
		case 0:{return MAIN_BOARD;}
		case 1:{return BOARD_1;}
		case 2:{return BOARD_2;}
		case 3:{return BOARD_3;}
		default:break;
	}
	return MAIN_BOARD;
}

void board_init(void)
{
    //依次添加本地器件
	device_add(&si4463);
	device_add(&relay);
    device_add(&SHT20);
    device_add(&DS1307);
    device_add(&Alarm);
    device_add(&d_ADC_Key);
    device_add(&usart_1);
    device_add(&usart_2);
    //上电即同步本地器件
	board_sync_device();
}






