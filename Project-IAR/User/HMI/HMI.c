/*
****************************************************
*  文件名             : HMI.c
*  作者               : -5A4A5943-
*  版本               : V1.0.0
*  编写日期           : 2016--10--29--13
*  简介               : HMI屛驱动函数
*  函数列表           : 
*  历史版本           : 
*****************************************************
*/


/*头文件  */

#include "board.h"



/*宏定义  */





/*变量定义*/

UART_HandleTypeDef huart3;
uint8_t BuffHMI[30] = {0x00};
PageInfTypedef PageAll = 
{
    {0,"c0",0,0},
    {}
}

/*变量声明*/





/*函数声明*/
static result HMI_open(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    huart3.Instance = USART3;
    huart3.Init.BaudRate = 115200;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart3) != HAL_OK)
    {
        Error_Handler();
        return false;
    }
    
    __HAL_RCC_USART3_CLK_ENABLE();
    
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    __HAL_AFIO_REMAP_USART3_ENABLE();

    /* Peripheral interrupt init */
    HAL_NVIC_SetPriority(USART3_IRQn, 10, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
    
    SET_STATE(HMI.state,STATE_OPEN);
    RESET_STATE(HMI.state,STATE_CLOSE);
    
    return true;

}
static result HMI_close(void)
{
    HAL_UART_MspDeInit(&huart3);
    
    RESET_STATE(HMI.state,STATE_OPEN);
    SET_STATE(HMI.state,STATE_CLOSE);

}
static result HMI_detect(void)
{
    return true;
}
static result HMI_command(uint8_t * command, uint32_t param)
{
    (void *)command;
    (void *)param;
    
    return true;
}
//configuration such as Light/Baud etc
static result HMI_set(uint32_t Item, uint32_t State)
{
    
}
static result HMI_puts(uint32_t LEDx, uint8_t * Directive, uint32_t Cnt)
{
    
}
static result HMI_gets(uint32_t SendAddr, uint8_t * start, uint32_t length)
{
    
}
static result HMI_timing_process(uint32_t Param1, uint32_t Param2, uint32_t Param3)
{
    
}
static result HMI_process_it(uint32_t ADC_Handle, uint32_t Param2, uint32_t Param3)
{
    
}

deviceModule HMI = 
{
    "HMI",
    STATE_CLOSE,
    0x00,
	Private,
    HMI_open,
    HMI_close,
    HMI_detect,
    HMI_command,
    HMI_set,
    HMI_puts,
    HMI_gets,
    HMI_timing_process,
    HMI_process_it
};

#if 1//发送指令

static uint8_t * SendInstruct_page(uint8_t * ID)
{
    strcpy(BuffHMI,Instruct_page);
    strcat(BuffHMI," ");
    strcat(BuffHMI,ID);
    return BuffHMI;
}

static uint8_t * SendInstruct_ref(uint8_t * ID)
{
    strcpy(BuffHMI,Instruct_ref);
    strcat(BuffHMI," ");
    strcat(BuffHMI,ID);
    return BuffHMI;
}

static uint8_t * SendInstruct_click(uint8_t * ID,uint8_t * Event)
{
    strcpy(BuffHMI,Instruct_click);
    strcat(BuffHMI," ");
    strcat(BuffHMI,ID);
    strcat(BuffHMI,",");
    strcat(BuffHMI,Event);
    return BuffHMI;
}

static uint8_t * SendInstruct_get(uint8_t * ValueName)
{
    strcpy(BuffHMI,Instruct_get);
    strcat(BuffHMI," ");
    strcat(BuffHMI,ValueName);
    return BuffHMI;
}

static uint8_t * SendInstruct_print(uint8_t * ValueName)
{
    strcpy(BuffHMI,Instruct_print);
    strcat(BuffHMI," ");
    strcat(BuffHMI,ValueName);
    return BuffHMI;
}

static uint8_t * SendInstruct_printh(uint8_t * ValueName)
{
    strcpy(BuffHMI,Instruct_printh);
    strcat(BuffHMI," ");
    strcat(BuffHMI,ValueName);
    return BuffHMI;
}

static uint8_t * SendInstruct_vis(uint8_t * ID,uint8_t * State)
{
    strcpy(BuffHMI,Instruct_vis);
    strcat(BuffHMI," ");
    strcat(BuffHMI,ID);
    strcat(BuffHMI,",");
    strcat(BuffHMI,State);
    return BuffHMI;
}

static uint8_t * SendInstruct_tsw(uint8_t * ID,uint8_t * State)
{
    strcpy(BuffHMI,Instruct_tsw);
    strcat(BuffHMI," ");
    strcat(BuffHMI,ID);
    strcat(BuffHMI,",");
    strcat(BuffHMI,State);
    return BuffHMI;
}

static uint8_t * SendInstruct_randset(uint8_t * Min,uint8_t * Max)
{
    strcpy(BuffHMI,Instruct_randset);
    strcat(BuffHMI," ");
    strcat(BuffHMI,Min);
    strcat(BuffHMI,",");
    strcat(BuffHMI,Max);
    return BuffHMI;
}

static uint8_t * SendInstruct_add(uint8_t * ID,uint8_t * ch,uint8_t * val)
{
    strcpy(BuffHMI,Instruct_add);
    strcat(BuffHMI," ");
    strcat(BuffHMI,ID);
    strcat(BuffHMI,",");
    strcat(BuffHMI,ch);
    strcat(BuffHMI,",");
    strcat(BuffHMI,val);
    return BuffHMI;
}

static uint8_t * SendInstruct_cle(uint8_t * ID,uint8_t * ch)
{
    strcpy(BuffHMI,Instruct_cle);
    strcat(BuffHMI," ");
    strcat(BuffHMI,ID);
    strcat(BuffHMI,",");
    strcat(BuffHMI,ch);
    return BuffHMI;
}

static uint8_t * SendInstruct_addt(uint8_t * ID,uint8_t * ch,uint8_t * qyt)
{
    strcpy(BuffHMI,Instruct_addt);
    strcat(BuffHMI," ");
    strcat(BuffHMI,ID);
    strcat(BuffHMI,",");
    strcat(BuffHMI,ch);
    strcat(BuffHMI,",");
    strcat(BuffHMI,qyt);
    return BuffHMI;
}

static uint8_t * SendInstruct_dovents(void)
{
    return Instruct_dovents;
}

static uint8_t * SendInstruct_sendme(void)
{
    return Instruct_sendme;
}

static uint8_t * SendInstruct_cov(uint8_t * att1,uint8_t * att2,uint8_t * length)
{
    strcpy(BuffHMI,Instruct_cov);
    strcat(BuffHMI," ");
    strcat(BuffHMI,att1);
    strcat(BuffHMI,",");
    strcat(BuffHMI,att2);
    strcat(BuffHMI,",");
    strcat(BuffHMI,length);
    return BuffHMI;
}

static uint8_t * SendInstruct_strlen(uint8_t * att0,uint8_t * att1)
{
    strcpy(BuffHMI,Instruct_strlen);
    strcat(BuffHMI," ");
    strcat(BuffHMI,att0);
    strcat(BuffHMI,",");
    strcat(BuffHMI,att1);
    return BuffHMI;
}

static uint8_t * SendInstruct_touch_j(void)
{
    return Instruct_touch_j;
}

static uint8_t * SendInstruct_ref_stop(void)
{
    return Instruct_ref_stop;
}

static uint8_t * SendInstruct_ref_start(void)
{
    return Instruct_ref_start;
}

static uint8_t * SendInstruct_com_stop(void)
{
    return Instruct_com_stop;
}

static uint8_t * SendInstruct_com_start(void)
{
    return Instruct_com_start;
}

static uint8_t * SendInstruct_code_c(void)
{
    return Instruct_code_c;
}

static uint8_t * SendInstruct_rest(void)
{
    return Instruct_rest;
}

#endif

/*

向HMI设置数值，HMI会不会返回触摸热区或者是其他事件？

*/

result SaveHMIValue(uint8_t PageID,uint8_t )






























