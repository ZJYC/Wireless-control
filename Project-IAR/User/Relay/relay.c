
#include "Board.h"

const RelayStruct RelayArray[6] = 
{
    {GPIOE,GPIO_PIN_10},
    {GPIOE,GPIO_PIN_11},
    {GPIOE,GPIO_PIN_12},
    {GPIOE,GPIO_PIN_13},
    {GPIOE,GPIO_PIN_14},
    {GPIOE,GPIO_PIN_15}
};

static uint8_t RelayState[6] = 
{
    RelayClose,
    RelayClose,
    RelayClose,
    RelayClose,
    RelayClose,
    RelayClose
};

result SetRelayState(uint8_t Num,uint8_t State)
{
    RelayState[Num] = State;
    return true;
}

uint8_t * GetRelayState(uint8_t Num)
{
    static uint8_t Buf[30] = {0x00},Buf_1[10] = {0x00};
	static uint8_t NOP[] = "NONE";
	if(Num > 5)return NOP;
	
    strcmp((char const *)Buf,(char const *)relay.name);
    strcat((char *)Buf,"-");
    sprintf((char *)Buf_1,"%d-",Num);
    strcat((char *)Buf,(char const *)Buf_1);
    if(RelayState[Num] == RelayOpen)
    {
        strcat((char *)Buf,"OPNE-");
    }
    else
    {
        strcat((char *)Buf,"CLOSE-");
    }
    return Buf;
}

/*
****************************************************
*  Function       : d_open_relay
*  Description    : E10-E15
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
static result d_open_relay(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//初始化IO
	__HAL_RCC_GPIOE_CLK_ENABLE();
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15,GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	//设置状态
    SET_STATE(relay.state,STATE_OPEN);
    RESET_STATE(relay.state,STATE_CLOSE);
	
	return true;
}
/*
****************************************************
*  Function       : d_close_relay
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
static result d_close_relay(void)
{
    RESET_STATE(relay.state,STATE_OPEN);
    SET_STATE(relay.state,STATE_CLOSE);

	return true;
}
/*
****************************************************
*  Function       : d_detect_relay
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
static result d_detect_relay(void)
{
	if(CHECK_STATE(relay.state,STATE_CLOSE))
	{
		relay.d_open();
	}

	return true;
}
/*
****************************************************
*  Function       : d_command_relay
*  Description    : OPEN:打开CLOSE:关闭
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
static result d_command_relay(uint8_t * command,uint32_t param)
{
	if(CHECK_STATE(relay.state,STATE_CLOSE))
	{
		relay.d_open();
	}

	if(strcmp((char const *)command,"OPEN") == 0)
    {
        HAL_GPIO_WritePin(RelayArray[param].GPIOx,RelayArray[param].GPIO_PIN,GPIO_PIN_SET);
    }
	if(strcmp((char const *)command,"CLOSE") == 0)
    {
        HAL_GPIO_WritePin(RelayArray[param].GPIOx,RelayArray[param].GPIO_PIN,GPIO_PIN_RESET);
    }
	
	return true;
}
/*
****************************************************
*  Function       : d_set_relay
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
static result d_set_relay (uint32_t Bit, uint32_t State)
{
	if(CHECK_STATE(relay.state,STATE_CLOSE))
	{
		relay.d_open();
	}
	
	if(State == 1)HAL_GPIO_WritePin(RelayArray[Bit].GPIOx,RelayArray[Bit].GPIO_PIN,GPIO_PIN_SET);
	if(State == 0)HAL_GPIO_WritePin(RelayArray[Bit].GPIOx,RelayArray[Bit].GPIO_PIN,GPIO_PIN_RESET);
	
	return true;
}
/*
****************************************************
*  Function       : d_puts_relay
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
static result d_puts_relay (uint32_t Param1, uint8_t * Param2, uint32_t Param3)
{
	if(CHECK_STATE(relay.state,STATE_CLOSE))
	{
		relay.d_open();
	}
	return true;	
}
/*
****************************************************
*  Function       : d_gets_relay
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
static result d_gets_relay (uint32_t Param1, uint8_t * Param2, uint32_t Param3)
{
	if(CHECK_STATE(relay.state,STATE_CLOSE))
	{
		relay.d_open();
	}
	return true;	
}
/*
****************************************************
*  Function       : d_timing_proceee_relay
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
static result d_timing_proceee_relay(uint32_t Param1, uint32_t Param2, uint32_t Param3)
{
	if(CHECK_STATE(relay.state,STATE_CLOSE))
	{
		relay.d_open();
	}
	return true;
}
/*
****************************************************
*  Function       : d_process_it_relay
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
static result d_process_it_relay(uint32_t Param1, uint32_t Param2, uint32_t Param3)
{
	if(CHECK_STATE(relay.state,STATE_CLOSE))
	{
		relay.d_open();
	}
	return true;	
}

deviceModule relay = 
{
	#if(CUR_BOARD == MAIN_BOARD)
	.name               ="Relay_0",	
	#elif(CUR_BOARD == BOARD_1)
	.name               ="Relay_1",	
	#elif(CUR_BOARD == BOARD_2)
	.name               ="Relay_2",
	#elif(CUR_BOARD == BOARD_3)
	.name               ="Relay_3",	
    #endif
	.state              = STATE_CLOSE,
	.next               = 0x00,
	.DA                 = Public,
	.d_open             = d_open_relay,
	.d_close            = d_close_relay,
	.d_detect           = d_detect_relay,
	.d_command          = d_command_relay,
	.d_set              = d_set_relay,
	.d_puts             = d_puts_relay,
	.d_gets             = d_gets_relay,
	.d_timing_proceee   = d_timing_proceee_relay,
	.d_process_it       = d_process_it_relay
};


