
/*
****************************************************
*  File name          : Key.c
*  Author             : 5A4A5943
*  Version            : V1.0
*  Date               : 2016--08--27--11--36--44
*  Description        : ADC按键驱动
*  Function List      : 
*  History            : 
*****************************************************
*/

#include "Key.h"

extern ADC_HandleTypeDef hadc1;

//p_ADC_KeyStruct ADC_Key = (p_ADC_KeyStruct)d_ADC_Key.data;

static ADC_KeyStruct ADC_Key = 
{
	{0x00},
	0,
	0,
	{300,850,1500,3000,4096},/* 按键阈值-实测 */
	(KeyValueEnum)0,
    (KeyValueEnum)0
};

/*
****************************************************
*  Function       : ADC_KeyGetKey
*  Description    : 获取ADC按键值,涉及到算法，返回（NABCD）
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*  Author         : 
*****************************************************
*/
KeyValueEnum ADC_KeyGetKey(void)
{
	uint16_t Max = 0,Min = 4096,i = 0;
	uint32_t Sum = 0;
	p_ADC_KeyStruct ADC_Key = (p_ADC_KeyStruct)d_ADC_Key.data;
	
	//求和最大值和最小值
	for(i = 0;i < ADCBufDeep;i ++)
	{
		Sum += ADC_Key->KeyADC_Value[i];
		
		if(Max < ADC_Key->KeyADC_Value[i])Max = ADC_Key->KeyADC_Value[i];
		if(Min > ADC_Key->KeyADC_Value[i])Min = ADC_Key->KeyADC_Value[i];
	}
	//平均值
	ADC_Key->KeyADC_ValueAverage = (Sum - Max - Min)/(ADCBufDeep - 2);
	//比较
	for(i = 0;i < NumOfKey;i ++)
	{
		if(ADC_Key->KeyADC_Boundary[i] < ADC_Key->KeyADC_ValueAverage && 
		ADC_Key->KeyADC_ValueAverage < ADC_Key->KeyADC_Boundary[i + 1])
		{
			ADC_Key->NewKeyValue = (KeyValueEnum)(i + 1);
			break;
		}
	}
	
	Sum = 0;
	
	//计算平方和，判断是否稳定
	
	for(i = 0;i < ADCBufDeep;i ++)
	{
		Sum += (ADC_Key->KeyADC_Value[i] - ADC_Key->KeyADC_ValueAverage)*
		(ADC_Key->KeyADC_Value[i] - ADC_Key->KeyADC_ValueAverage);
	}
	
	if(Sum > ADC_DX_MAX)ADC_Key->NewKeyValue = N;
	
	//倘若按下一直不放，则返回N
	if(ADC_Key->NewKeyValue == ADC_Key->OldKeyValue)
	{
		return N;
	}
	else
	{
		ADC_Key->OldKeyValue = ADC_Key->NewKeyValue;
		return ADC_Key->NewKeyValue;
	}
}
/*
****************************************************
*  Function       : HAL_ADC_ConvCpltCallback
*  Description    : ADC转换完成的回调函数(重写)
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*  Author         : 
*****************************************************
*/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	d_ADC_Key.d_process_it((uint32_t)hadc,0,0);
}
/*
****************************************************
*  Function       : d_ADC_Key_Open
*  Description    : 初始化ADC外设(A2端口)
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
static result d_ADC_Key_Open (void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_ChannelConfTypeDef sConfig;

	__HAL_RCC_ADC1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    HAL_NVIC_SetPriority(ADC1_2_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(ADC1_2_IRQn);

	hadc1.Instance = ADC1;
	hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;////
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	
	if(HAL_ADC_Init(&hadc1) != HAL_OK)
	{
		Error_Handler();
	}
	
	sConfig.Channel = ADC_CHANNEL_2;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES_5;
	
	if(HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
	
	ADC_Enable(&hadc1);

	AddPrivateBuf(&d_ADC_Key,(uint8_t *)&ADC_Key,sizeof(ADC_KeyStruct));
	
	SET_STATE(d_ADC_Key.state,STATE_OPEN);
    RESET_STATE(d_ADC_Key.state,STATE_CLOSE);
	return true;
}
/*
****************************************************
*  Function       : d_ADC_Key_Close
*  Description    : 关闭ADC外设(!!!!!这会关闭所有的ADC设备!!!!!)
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
static result d_ADC_Key_Close (void)
{
	HAL_ADC_DeInit(&hadc1);
	
	SET_STATE(d_ADC_Key.state,STATE_CLOSE);
	return true;
}
/*
****************************************************
*  Function       : d_ADC_Key_Detect
*  Description    : 检测ADC是否存在
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 均返回true
*  Others         : 
*****************************************************
*/
static result d_ADC_Key_Detect (void)
{
	if(CHECK_STATE(d_ADC_Key.state,STATE_CLOSE) && d_ADC_Key.d_open() != true)return false;

	return true;
}
/*
****************************************************
*  Function       : d_ADC_Key_Command
*  Description    : 执行命令
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
static result d_ADC_Key_Command (uint8_t * command, uint32_t param)
{
	if(CHECK_STATE(d_ADC_Key.state,STATE_CLOSE) && d_ADC_Key.d_open() != true)return false;

	command = command;
	param = param;
	return true;
}
/*
****************************************************
*  Function       : d_ADC_Key_Set
*  Description    : 设置
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
static result d_ADC_Key_Set (uint32_t type, uint32_t param)
{
	if(CHECK_STATE(d_ADC_Key.state,STATE_CLOSE) && d_ADC_Key.d_open() != true)return false;

	type = type;
	param = param;
	
	return true;
}
/*
****************************************************
*  Function       : d_ADC_Key_Puts
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
static result d_ADC_Key_Puts (uint32_t RecvAddr, uint8_t * start, uint32_t length)
{
	if(CHECK_STATE(d_ADC_Key.state,STATE_CLOSE) && d_ADC_Key.d_open() != true)return false;
	
	RecvAddr = RecvAddr;
	start = start;
	length = length;
	return true;
}
/*
****************************************************
*  Function       : d_ADC_Key_Gets
*  Description    : 获取按键值(按键具有去抖动和防一直按下的功能)
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
static result d_ADC_Key_Gets (uint32_t SendAddr, uint8_t * p_KeyValue, uint32_t length)
{
	KeyValueEnum KeyValue = N;
	
	if(CHECK_STATE(d_ADC_Key.state,STATE_CLOSE) && d_ADC_Key.d_open() != true)return false;

    SendAddr = SendAddr;
	length = length;

	KeyValue = ADC_KeyGetKey();
	
	*(KeyValueEnum*)p_KeyValue = KeyValue;
	
	return true;
}
/*
****************************************************
*  Function       : d_ADC_Key_TimingProcess
*  Description    : 定时处理流程(定时开启ADC转换)
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
static result d_ADC_Key_TimingProcess(uint32_t Param1, uint32_t Param2, uint32_t Param3)
{
	if(CHECK_STATE(d_ADC_Key.state,STATE_CLOSE) && d_ADC_Key.d_open() != true)return false;
	
	Param1 = Param1;
	Param2 = Param2;
	Param3 = Param3;

	HAL_ADC_Start_IT(&hadc1);
	
	return true;
}
/*
****************************************************
*  Function       : d_ADC_Key_process_it
*  Description    : 中断处理流程(ADC转换完成中断)
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
static result d_ADC_Key_process_it(uint32_t ADC_Handle, uint32_t Param2, uint32_t Param3)
{
	p_ADC_KeyStruct ADC_Key = (p_ADC_KeyStruct)d_ADC_Key.data;
	
	if(CHECK_STATE(d_ADC_Key.state,STATE_CLOSE) && d_ADC_Key.d_open() != true)return false;
	
	Param2 = Param2;
	Param3 = Param3;
	
	if(ADC_Key->KeyADC_ValuePoint >= ADCBufDeep)ADC_Key->KeyADC_ValuePoint = 0;
	ADC_Key->KeyADC_Value[ADC_Key->KeyADC_ValuePoint++] = HAL_ADC_GetValue((ADC_HandleTypeDef*)ADC_Handle);
	HAL_ADC_Stop_IT((ADC_HandleTypeDef*)ADC_Handle);
	return true;
}

deviceModule d_ADC_Key = 
{
	.name               = "ADC_KEY",
	.state              = STATE_CLOSE,
	.next               = 0x00,
	.DA                 = Private,
	.d_open             = d_ADC_Key_Open,
	.d_close            = d_ADC_Key_Close,
	.d_detect           = d_ADC_Key_Detect,
	.d_command          = d_ADC_Key_Command,
	.d_set              = d_ADC_Key_Set,
	.d_puts             = d_ADC_Key_Puts,
	.d_gets             = d_ADC_Key_Gets,
	.d_timing_proceee   = d_ADC_Key_TimingProcess,
};








