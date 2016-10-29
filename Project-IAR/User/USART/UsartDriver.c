
#include "board.h"

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

static SerialTCBTypedef SerialTCB_Usart2 = {0x00};
static uint8_t Uart2Buf[100] = {0x00};
static uint8_t Uart2BufIndex = 0x00;
static uint8_t Recvd = 0x00;
static uint8_t Timeout = 0x00;;

#if	1//操作系统驱动接口层

#if	1//USART1
/*
****************************************************
*  Function       : d_open_usart_1
*  Description    : A9 + A10
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
result d_open_usart_1 (void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	__HAL_RCC_USART1_CLK_ENABLE();
	
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Peripheral interrupt init */
    HAL_NVIC_SetPriority(USART1_IRQn, 9, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);

	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart1) != HAL_OK)
	{
		Error_Handler();
	}
	
    //Usart_1_Tcb.Usart = &huart1;
    
    SET_STATE(usart_1.state,STATE_OPEN);
    RESET_STATE(usart_1.state,STATE_CLOSE);
	
    Uart2BufIndex = 0;
	usart_1.d_gets(0,&Uart2Buf[Uart2BufIndex],1);
    usart_1.d_puts(0,"c0.val=1",strlen("c0.val=1"));
	
	return true;
}
/*
****************************************************
*  Function       : d_close_usart_1
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
result d_close_usart_1 (void)
{
    RESET_STATE(usart_1.state,STATE_OPEN);
    SET_STATE(usart_1.state,STATE_CLOSE);

	return true;
}
/*
****************************************************
*  Function       : d_detect_usart_1
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
result d_detect_usart_1 (void)
{
	if(CHECK_STATE(usart_1.state,STATE_CLOSE))usart_1.d_open();
	
	return true;
}
/*
****************************************************
*  Function       : d_command_usart_1
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
result d_command_usart_1 (uint8_t * Command, uint32_t Param)
{
	if(CHECK_STATE(usart_1.state,STATE_CLOSE))usart_1.d_open();
	
	return true;
}
/*
****************************************************
*  Function       : d_set_usart_1
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
result d_set_usart_1 (uint32_t Param_1, uint32_t Param_2)
{
	Param_1 = Param_1;
	Param_2 = Param_2;
	
	if(CHECK_STATE(usart_1.state,STATE_CLOSE))usart_1.d_open();
	
	return true;
}
/*
****************************************************
*  Function       : d_puts_usart_1
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
result d_puts_usart_1 (uint32_t Param, uint8_t * Str, uint32_t Length) 
{
	Param = Param;
	
	if(CHECK_STATE(usart_1.state,STATE_CLOSE))usart_1.d_open();
	
	//if(UsartTcbSends(&Usart_1_Tcb,Counter,Str) == true)return true;
	HAL_UART_Transmit_IT(&huart1,Str,Length);
	
    return false;
}
/*
****************************************************
*  Function       : d_gets_usart_1
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
result d_gets_usart_1 (uint32_t param, uint8_t * Str, uint32_t Length)
{
	if(CHECK_STATE(usart_1.state,STATE_CLOSE))usart_1.d_open();
	
	//if(UsartTcbRecvs(&Usart_1_Tcb,Counter,Str) == true)return true;
	HAL_UART_Receive_IT(&huart1,Str,Length);
	
    return false;	
}
/*
****************************************************
*  Function       : d_timing_proceee_usart_1
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
result d_timing_proceee_usart_1(uint32_t Param_1, uint32_t Param_2, uint32_t Param_3)
{
    uint8_t i = 0;
    
	Param_1 = Param_1;
	Param_2 = Param_2;
	Param_3 = Param_3;
	
	if(CHECK_STATE(usart_1.state,STATE_CLOSE))usart_1.d_open();
	//10ms调用一次
	//UsartTcbRxTxTimingProcess(&Usart_1_Tcb,Param_1);
    if(Timeout-- == 0)
    {   
        Timeout = 0;
        //Uart2BufIndex = 0;
    }
	if(Recvd == 0xff)
	{
		Recvd = 0x00;
        while(Uart2Buf[i] == 0)i++;
        if(i >= 100)return false;
		HMI_ExecInstruction(&Uart2Buf[i],100 - i);
	}
	return true;
}
/*
****************************************************
*  Function       : d_process_it_usart_1
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/

//static uint8_t Uart2Buf[100] = {0x00};
//static uint8_t Uart2BufIndex = 0x00;

result d_process_it_usart_1(uint32_t ItType, uint32_t Param_2, uint32_t Param_3)
{
	static uint8_t FF_Counter = 0;
	//Param_1 = Param_1;
	Param_2 = Param_2;
	Param_3 = Param_3;
	
	if(CHECK_STATE(usart_1.state,STATE_CLOSE))usart_1.d_open();
	
	if(ItType == RxIt)
	{
        Timeout = 5;
		//UsartTcbRxItProcess(&Usart_1_Tcb);
		if(Uart2Buf[Uart2BufIndex] == 0xFF)FF_Counter ++;else FF_Counter = 0;
		if(FF_Counter >= 3 && Uart2BufIndex > 3){FF_Counter = 0;Uart2BufIndex = 0;Recvd = 0xff;}
		if(Uart2BufIndex >= 100)Uart2BufIndex = 0;
		Uart2BufIndex++;
		usart_1.d_gets(0,&Uart2Buf[Uart2BufIndex],1);
	}
	if(ItType == TxIt)
	{
		//UsartTcbTxItProcess(&Usart_1_Tcb);
	}
	
	return true;
	
}

deviceModule usart_1 = 
{
	"Usart_1",
	STATE_CLOSE,
	0x00,
	Private,
	d_open_usart_1,
	d_close_usart_1,
	d_detect_usart_1,
	d_command_usart_1,
	d_set_usart_1,
	d_puts_usart_1,
	d_gets_usart_1,
	d_timing_proceee_usart_1,
	d_process_it_usart_1
};

#endif

#if	1//USART2

uint8_t Port_SendDataIT(uint8_t Char)
{
	uint8_t Temp = Char;
	HAL_UART_Transmit_IT(&huart2,&Temp,1);
    return 0;
}

uint8_t Port_RecvDataIT(uint8_t * Char)
{
	HAL_UART_Receive_IT(&huart2,Char,1);
    return 0;
}

uint8_t Port_SerialInit(void)
{
	return 0;
}

/*
****************************************************
*  Function       : d_open_usart_2
*  Description    : D5 + D6
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
result d_open_usart_2 (void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

    __HAL_RCC_USART2_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    __HAL_AFIO_REMAP_USART2_ENABLE();

    HAL_NVIC_SetPriority(USART2_IRQn, 9, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);

	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart2) != HAL_OK)
	{
		Error_Handler();
	}
    
	SerialInit_OS(&SerialTCB_Usart2,Port_SerialInit,Port_SendDataIT,Port_RecvDataIT);
	
    SET_STATE(usart_2.state,STATE_OPEN);
    RESET_STATE(usart_2.state,STATE_CLOSE);
	
	return true;
}
/*
****************************************************
*  Function       : d_close_usart_2
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
result d_close_usart_2 (void)
{
    RESET_STATE(usart_2.state,STATE_OPEN);
    SET_STATE(usart_2.state,STATE_CLOSE);

	return true;
}
/*
****************************************************
*  Function       : d_detect_usart_2
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
result d_detect_usart_2 (void)
{
	if(CHECK_STATE(usart_2.state,STATE_CLOSE))usart_2.d_open();
	
	return true;
}
/*
****************************************************
*  Function       : d_command_usart_2
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
result d_command_usart_2 (uint8_t * Command, uint32_t Param)
{
	if(CHECK_STATE(usart_2.state,STATE_CLOSE))usart_2.d_open();
	
	return true;
}
/*
****************************************************
*  Function       : d_set_usart_2
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
result d_set_usart_2 (uint32_t Param_1, uint32_t Param_2)
{
	Param_1 = Param_1;
	Param_2 = Param_2;
	
	if(CHECK_STATE(usart_2.state,STATE_CLOSE))usart_2.d_open();
	
	return true;
}
/*
****************************************************
*  Function       : d_puts_usart_2
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
result d_puts_usart_2 (uint32_t Port, uint8_t * Str, uint32_t Counter) 
{
	//uint8_t cnt = 0;
	
	//SET_STATE(usart_2.state,STATE_TX);
	
	if(CHECK_STATE(usart_2.state,STATE_CLOSE))usart_2.d_open();
	
	if(SerialSend_OS(&SerialTCB_Usart2,0,Port,Str,Counter) == SR_True)return true;
    
    return false;
	
	//RESET_STATE(usart_2.state,STATE_TX);
}
/*
****************************************************
*  Function       : d_gets_usart_2
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
result d_gets_usart_2 (uint32_t Port, uint8_t * Str, uint32_t Counter)
{
	//uint8_t cnt = 0;
	//uint16_t * Temp = (uint16_t *)Counter;
	
	//SET_STATE(usart_2.state,STATE_RX);
	
	if(CHECK_STATE(usart_2.state,STATE_CLOSE))usart_2.d_open();

	if(SerialRecv_OS(&SerialTCB_Usart2,0,Port,Str,(uint16_t *)Counter) == SR_True)return true;
    
    return false;
	
	//RESET_STATE(usart_2.state,STATE_RX);

}
/*
****************************************************
*  Function       : d_timing_proceee_usart_2
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
result d_timing_proceee_usart_2(uint32_t Param_1, uint32_t Param_2, uint32_t Param_3)
{
	Param_2 = Param_2;
	Param_3 = Param_3;
	
	if(CHECK_STATE(usart_2.state,STATE_CLOSE))usart_2.d_open();
	//10ms调用一次
	//UsartTcbRxTxTimingProcess(&Usart_2_Tcb,Param_1);
	
	return true;
}
/*
****************************************************
*  Function       : d_process_it_usart_2
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
result d_process_it_usart_2(uint32_t ItType, uint32_t Param_2, uint32_t Param_3)
{
	//Param_1 = Param_1;
	Param_2 = Param_2;
	Param_3 = Param_3;
	
	if(CHECK_STATE(usart_2.state,STATE_CLOSE))usart_2.d_open();
	
	if(ItType == RxIt)
	{
		SerialRecvProcessIT(&SerialTCB_Usart2);
	}
	if(ItType == TxIt)
	{
		SerialSendProcessIT(&SerialTCB_Usart2);
	}
	
	return true;
	
}

deviceModule usart_2 = 
{
	"Usart_2",
	STATE_CLOSE,
	0x00,
	Private,
	d_open_usart_2,
	d_close_usart_2,
	d_detect_usart_2,
	d_command_usart_2,
	d_set_usart_2,
	d_puts_usart_2,
	d_gets_usart_2,
	d_timing_proceee_usart_2,
	d_process_it_usart_2
};


#endif

#endif

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)usart_1.d_process_it(TxIt,0,0);
	if(huart->Instance == USART2)usart_2.d_process_it(TxIt,0,0);
	//if(huart->Instance == USART3)HMI.d_process_it(TxIt,0,0);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)usart_1.d_process_it(RxIt,0,0);
	if(huart->Instance == USART2)usart_2.d_process_it(RxIt,0,0);
	//if(huart->Instance == USART3)HMI.d_process_it(RxIt,0,0);
}

































































