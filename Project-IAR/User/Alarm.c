#include "board.h"

Caution caution = 
{
    {0,0,0,0,0},
    {0,0,0,0,0},
    0,
    {0,0,0,0,0}
};

/*
****************************************************
*  Function       : 打开Alarm
*  Description    : 初始化4个LED和1个蜂鸣器（C0,C1,C2,C3,E1）
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 均返回true
*  Others         : 
*****************************************************
*/
result Alarm_open(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
    //初始化4个LED
	__HAL_RCC_GPIOC_CLK_ENABLE();
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    //初始化蜂鸣器
    __HAL_RCC_GPIOE_CLK_ENABLE();
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1,GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	//设置状态
    SET_STATE(Alarm.state,STATE_OPEN);
    RESET_STATE(Alarm.state,STATE_CLOSE);
	
    return true;
}
/*
****************************************************
*  Function       : 关闭Alarm
*  Description    : 反初始化各个引脚
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 均返回true
*  Others         : 
*****************************************************
*/
result Alarm_close(void)
{
	HAL_GPIO_DeInit(GPIOC,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    HAL_GPIO_DeInit(GPIOE,GPIO_PIN_1);
	//设置状态
    RESET_STATE(Alarm.state,STATE_OPEN);
    SET_STATE(Alarm.state,STATE_CLOSE);

    return true;
}
/*
****************************************************
*  Function       : Alarm_detect
*  Description    : 检测继电器是否存在
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 均返回true
*  Others         : 
*****************************************************
*/
result Alarm_detect(void)
{
	if(CHECK_STATE(Alarm.state,STATE_CLOSE))
	{
		Alarm.d_open();
	}

    return true;
}
/*
****************************************************
*  Function       : Alarm_command
*  Description    : 执行命令，目前仅仅适用于蜂鸣器
*  Calls          : 
*  Called By      : 
*  Input          : command:"OPEN",param:none
*  Output         : 
*  Return         : 均返回true
*  Others         : 
*****************************************************
*/
result Alarm_command(uint8_t * command, uint32_t param)
{
	if(CHECK_STATE(Alarm.state,STATE_CLOSE))
	{
		Alarm.d_open();
	}

	if(strcmp((const char *)command,"OPEN") == 0)
	{
		Alarm.d_puts(LED5,"11100011100011111111000",10);
	}
    return true;
}
/*
****************************************************
*  Function       : Alarm_set
*  Description    : 操作某一个引脚
*  Calls          : 
*  Called By      : 
*  Input          : LEDx:引脚编号State:引脚状态
*  Output         : 
*  Return         : 均返回true
*  Others         : 
*****************************************************
*/
result Alarm_set(uint32_t LEDx, uint32_t State)
{
	if(CHECK_STATE(Alarm.state,STATE_CLOSE))
	{
		Alarm.d_open();
	}
	
	switch(LEDx)
	{
		
		case 0:{
					if(State == 0)HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);
					if(State == 1)HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);
					break;
				}
		case 1:{
					if(State == 0)HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_SET);
					if(State == 1)HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET);
					break;
				}
		case 2:{
					if(State == 0)HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_SET);
					if(State == 1)HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_RESET);
					break;
				}
		case 3:{
					if(State == 0)HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_SET);
					if(State == 1)HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_RESET);
					break;
				}
		case 4:{
					if(State == 0)HAL_GPIO_WritePin(GPIOE,GPIO_PIN_1,GPIO_PIN_SET);
					if(State == 1)HAL_GPIO_WritePin(GPIOE,GPIO_PIN_1,GPIO_PIN_RESET);
					break;
				}
		default:break;
	}
	
    return true;
}
/*
****************************************************
*  Function       : Alarm_puts
*  Description    : 设置提醒组
*  Calls          : 
*  Called By      : 
*  Input          : LEDx:LED编号Directive:指令Cnt:重复次数
*  Output         : 
*  Return         : 均返回true
*  Others         : 
*****************************************************
*/
result Alarm_puts(uint32_t LEDx, uint8_t * Directive, uint32_t Cnt)
{
	if(CHECK_STATE(Alarm.state,STATE_CLOSE))
	{
		Alarm.d_open();
	}

    osMutexWait(AlarmMutex,osWaitForever);
    caution.Counter_1[LEDx] = Cnt,
    caution.Directive[LEDx] = Directive;
    osMutexRelease(AlarmMutex);
    return true;
}
/*
****************************************************
*  Function       : Alarm_gets
*  Description    : 获取。。。
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
result Alarm_gets(uint32_t SendAddr, uint8_t * start, uint32_t length)
{
	if(CHECK_STATE(Alarm.state,STATE_CLOSE))
	{
		Alarm.d_open();
	}

    return true;
}
/*
****************************************************
*  Function       : Alarm_timing_process
*  Description    : 定时处理函数
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 均返回true
*  Others         : 
*****************************************************
*/
result Alarm_timing_process(uint32_t Param1, uint32_t Param2, uint32_t Param3)
{
    uint16_t Cnt1;
    uint8_t * Temp = 0;
	
	if(CHECK_STATE(Alarm.state,STATE_CLOSE))
	{
		Alarm.d_open();
	}

	Param1 = Param1;
	Param2 = Param2;
	Param3 = Param3;
    //由于这是在定时器回调里面，所以不能使之阻塞
    //osMutexWait(AlarmMutex,osWaitForever);
    //循环计数
    taskENTER_CRITICAL();
    caution.CommonCnt++;if(caution.CommonCnt > 100)caution.CommonCnt = 0;
    for(Cnt1 = 0;Cnt1 < 5;Cnt1 ++)
    {
        if(caution.Counter_1[Cnt1] != 0x00)
        {
            Temp = caution.Directive[Cnt1];
            if(Temp[caution.PrivateCnt[Cnt1]] == 0)
            {
                caution.PrivateCnt[Cnt1] = 0;
                caution.Counter_1[Cnt1]--;
                Alarm.d_set(Cnt1,0);
                continue;
            }
            if(Temp[caution.PrivateCnt[Cnt1]] == '1')
            {
                Alarm.d_set(Cnt1,1);
            }
            if(Temp[caution.PrivateCnt[Cnt1]] == '0')
            {
                Alarm.d_set(Cnt1,0);
            }
            caution.PrivateCnt[Cnt1] ++;
        }
    }
    //osMutexRelease(AlarmMutex);
	taskEXIT_CRITICAL();
	return true;	
}

deviceModule Alarm = 
{
	#if(CUR_BOARD == MAIN_BOARD)
	"Alarm_0",	
	#elif(CUR_BOARD == BOARD_1)
	"Alarm_1",	
	#elif(CUR_BOARD == BOARD_2)
	"Alarm_2",
	#elif(CUR_BOARD == BOARD_3)
	"Alarm_3",	
	#endif
	STATE_CLOSE,
	0x00,
	Public,
	Alarm_open,
	Alarm_close,
	Alarm_detect,
	Alarm_command,
	Alarm_set,
	Alarm_puts,
	Alarm_gets,
    Alarm_timing_process
};
















