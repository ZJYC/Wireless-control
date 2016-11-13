
#include "board.h"

static void DS1307_GetTime(DS1307_Time * Ds1307Time);
static void DS1307_Delay(u32 n);
static void DS1307_Init(void);
static void DS1307_SCL_OUTPUT(void);
static void DS1307_SCL_INPUT(void);
static void DS1307_SDA_OUTPUT(void);
static void DS1307_SDA_INPUT(void);
static void DS1307_I2cStartCondition(void);
static void DS1307_I2cStopCondition(void);
static void DS1307_I2cAcknowledge(void);
static void DS1307_I2cNoAcknowledge(void);
static uint8_t DS1307_I2cReadByte(void);
static uint8_t DS1307_I2cWriteByte(uint8_t byte);
static uint8_t DS1307_Wait_Ack(void);
static void RTC_init(void);
static void ds1307_Write(uint8_t WriteAddr,uint8_t Data);
static uint8_t ds1307_Read(uint8_t ReadAddr);

//p_DS1307_Time DS1307_Time = (p_DS1307_Time)DS1307.data;
DS1307_Time Ds1307Time = {0x00};
/*
****************************************************
*  Function       : DS1307_open
*  Description    : 初始化
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
static result DS1307_open(void)
{
    RTC_init();
	
	AddPrivateBuf(&DS1307,(uint8_t *)&Ds1307Time,sizeof(DS1307_Time));
	
    SET_STATE(DS1307.state,STATE_OPEN);
    RESET_STATE(DS1307.state,STATE_CLOSE);
    return true;
}
/*
****************************************************
*  Function       : DS1307_close
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
static result DS1307_close(void)
{
    RESET_STATE(DS1307.state,STATE_OPEN);
    SET_STATE(DS1307.state,STATE_CLOSE);
	
    return true;
}
/*
****************************************************
*  Function       : DS1307_detect
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
static result DS1307_detect(void)
{
	if(CHECK_STATE(DS1307.state,STATE_CLOSE) && DS1307.d_open() != true)return false;

    return true;
}
/*
****************************************************
*  Function       : DS1307_command
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
static result DS1307_command(uint8_t * command, uint32_t param)
{
	if(CHECK_STATE(DS1307.state,STATE_CLOSE) && DS1307.d_open() != true)return false;

    return true;
}
/*
****************************************************
*  Function       : DS1307_set
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
static result DS1307_set(uint32_t Area, uint32_t Value)
{
	if(CHECK_STATE(DS1307.state,STATE_CLOSE) && DS1307.d_open() != true)return false;
	
	switch(Area)
	{
		case ADDR_SEC:
		{
			ds1307_Write(ADDR_SEC,Value);
			break;
		}
		case ADDR_MIN:
		{
			ds1307_Write(ADDR_MIN,Value);
			break;
		}
		case ADDR_HOUR:
		{
			ds1307_Write(ADDR_HOUR,Value);
			break;
		}
		case ADDR_DATE:
		{
			ds1307_Write(ADDR_DATE,Value);
			break;
		}
		case ADDR_MON:
		{
			ds1307_Write(ADDR_MON,Value);
			break;
		}
		case ADDR_YEAR:
		{
			ds1307_Write(ADDR_YEAR,Value);
			break;
		}
		case ADDR_WDAY:
		{
			ds1307_Write(ADDR_WDAY,Value);
			break;
		}
		default:break;
	}
	
    return true;
}
/*
****************************************************
*  Function       : DS1307_puts
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
static result DS1307_puts(uint32_t RecvAddr, uint8_t * start, uint32_t length)
{
	if(CHECK_STATE(DS1307.state,STATE_CLOSE) && DS1307.d_open() != true)return false;

    return true;
}
/*
****************************************************
*  Function       : DS1307_gets
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : Area:区域Time:时间
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
static result DS1307_gets(uint32_t Param1, uint8_t * Param2, uint32_t Param3)
{
	p_DS1307_Time DS1307_Time = (p_DS1307_Time)DS1307.data;
	
	if(CHECK_STATE(DS1307.state,STATE_CLOSE) && DS1307.d_open() != true)return false;

    Param1 = Param1;
	Param2 = Param2;
	Param3 = Param3;
	
    DS1307_GetTime(DS1307_Time);

    return true;
}

#if 1//DS1307低级驱动

static void DS1307_GetTime(DS1307_Time * Ds1307Time)
{
	uint8_t temp = 0;
	temp=ds1307_Read(ADDR_SEC);
	temp = (temp&0x0f) + (((temp>>4)&0x07)*10);
	Ds1307Time->sec = temp;
	
	temp=ds1307_Read(ADDR_MIN);
	temp = (temp&0x0f) + (((temp>>4)&0x07)*10);
	Ds1307Time->min = temp;

	temp=ds1307_Read(ADDR_HOUR);
	temp = (temp&0x0f) + (((temp>>4)&0x03)*10);
	Ds1307Time->hour = temp;

	temp=ds1307_Read(ADDR_WDAY);
	Ds1307Time->wday = temp;

	temp=ds1307_Read(ADDR_DATE);
	temp = (temp&0x0f) + (((temp>>4)&0x03)*10);
	Ds1307Time->day = temp;

	temp=ds1307_Read(ADDR_MON);
	temp = (temp&0x0f) + (((temp>>4)&0x01)*10);
	Ds1307Time->mon = temp;

	temp=ds1307_Read(ADDR_YEAR);
	temp = (temp&0x0f) + (((temp>>4))*10);
	Ds1307Time->year = temp;

}

static void DS1307_Delay(u32 n)
{
    uint16_t i;
    
    while(n--)
    {
        for(i = 0; i < 10; i++)
        {
            asm("NOP");
        }
    }
}

static void DS1307_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    DS1307_SCL_HIGH();
    DS1307_SDA_HIGH();
}

static void DS1307_SCL_OUTPUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin =  DS1307_SCL_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(DS1307_I2C_PORT, &GPIO_InitStructure);
}

static void DS1307_SCL_INPUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin =  DS1307_SCL_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_INPUT;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(DS1307_I2C_PORT, &GPIO_InitStructure);
}

static void DS1307_SDA_OUTPUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin =  DS1307_SDA_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(DS1307_I2C_PORT, &GPIO_InitStructure);
}

static void DS1307_SDA_INPUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin =  DS1307_SDA_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_INPUT;
    HAL_GPIO_Init(DS1307_I2C_PORT, &GPIO_InitStructure);
}

static void DS1307_I2cStartCondition(void)
{  
    DS1307_SCL_OUTPUT();
    DS1307_SDA_OUTPUT();
    DS1307_SDA_HIGH();
    DS1307_SCL_HIGH();
    DS1307_SDA_LOW();
    DS1307_Delay(1);
    DS1307_SCL_LOW();
    DS1307_Delay(1);
}

static void DS1307_I2cStopCondition(void)
{   
    DS1307_SCL_OUTPUT();
    DS1307_SDA_OUTPUT();
    DS1307_SDA_LOW();
    DS1307_SCL_LOW();
    DS1307_SCL_HIGH();
    DS1307_Delay(1);
    DS1307_SDA_HIGH();
    DS1307_Delay(1);

}

static void DS1307_I2cAcknowledge(void)
{    
    DS1307_SCL_OUTPUT();
    DS1307_SDA_OUTPUT();
    DS1307_SDA_LOW();
    DS1307_SCL_HIGH();
    DS1307_Delay(1);
    DS1307_SCL_LOW();   
    DS1307_Delay(1);
}

static void DS1307_I2cNoAcknowledge(void)
{
    
    DS1307_SCL_OUTPUT();
    DS1307_SDA_OUTPUT();
    DS1307_SDA_HIGH();
    DS1307_SCL_HIGH();
    DS1307_Delay(1);
    DS1307_SCL_LOW();   
    DS1307_Delay(1);

}

static uint8_t DS1307_I2cReadByte(void)
{
    uint8_t i, val = 0;
    
    //DS1307_SCL_OUTPUT();
    DS1307_SDA_OUTPUT();
    DS1307_SDA_HIGH();
    DS1307_SDA_INPUT();
    //此循环不应被打断
    taskENTER_CRITICAL();
    for(i = 0; i < 8; i++)
    {
        val <<= 1; 
        DS1307_SCL_HIGH();   
        DS1307_Delay(1);
        if(GPIO_PIN_SET == DS1307_SDA_STATE()) 
        {
            val |= 0x01;
        }
        DS1307_SCL_LOW();  
        DS1307_Delay(1);
    }
    taskEXIT_CRITICAL();
    DS1307_SDA_OUTPUT();
    DS1307_SDA_HIGH();
    
    return (val);
}

static uint8_t DS1307_I2cWriteByte(uint8_t byte)
{
    uint8_t i = 0,ack = 0;

    DS1307_SCL_OUTPUT();
    DS1307_SDA_OUTPUT();
	//此循环不应被打断
    taskENTER_CRITICAL();
    for(i = 0; i < 8; i++)
    {
        if(byte & 0x80) 
        {
            DS1307_SDA_HIGH();
        }
        else 
        {
            DS1307_SDA_LOW();
        }
        DS1307_SCL_HIGH();
        DS1307_Delay(1);
        DS1307_SCL_LOW();   
        DS1307_Delay(1);
        byte <<= 1;
    }
    taskEXIT_CRITICAL();
    DS1307_SDA_INPUT();
    DS1307_SCL_HIGH();
    
    if(GPIO_PIN_SET == DS1307_SDA_STATE()) 
    {
        ack = 0x00;
    }
    else
    {
        ack = 0x01;
    }
    
    DS1307_SCL_LOW();  
    DS1307_SDA_OUTPUT();
    DS1307_SDA_HIGH();

	return ack;
}

static uint8_t DS1307_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
    
	DS1307_SDA_INPUT(); 
	DS1307_Delay(1);	   
	DS1307_SCL_HIGH();
	DS1307_Delay(1);	 
	while(DS1307_SDA_STATE())
	{
		ucErrTime++;
        
        DS1307_Delay(1);
        
		if(ucErrTime>250)
		{
			DS1307_I2cStopCondition();
			return 1;
		}
	}
	DS1307_SCL_LOW();//时钟输出为0   
    
	return 0;  
} 

static void RTC_init(void)
{
	DS1307_Init();
	ds1307_Write(0x00,0x00);
	//使能时钟，即把0x00地址的最高位置0，让时钟开始工作
	//注意：这里会出一点问题，秒会被清零
}

static void ds1307_Write(uint8_t WriteAddr,uint8_t Data)
{	
    uint8_t temp;
    temp=(Data/10*16)+(Data%10);			//16进制转BCD	
    DS1307_I2cStartCondition();  
    DS1307_I2cWriteByte(0xd0);	            //器件写地址
    DS1307_I2cWriteByte(WriteAddr);			//寄存器地址   										  		   
    DS1307_I2cWriteByte(temp);				//发送字节  	    	   
    DS1307_I2cStopCondition();				//产生停止条件	 
}

static uint8_t ds1307_Read(uint8_t ReadAddr)
{				  
	uint8_t temp;		  
	DS1307_I2cStartCondition();  
	DS1307_I2cWriteByte(0xd0);				//器件写地址
	DS1307_I2cWriteByte(ReadAddr);			//寄存器地址
	DS1307_I2cStopCondition();				//注意！！！这里必须重启iic总线！！
	
	DS1307_I2cStartCondition(); 
	DS1307_I2cWriteByte(0xd1);				//进入接收模式   
    temp = DS1307_I2cReadByte();			//开始读数据
    DS1307_I2cNoAcknowledge();				//非应答
    DS1307_I2cStopCondition();                
    //temp自动转成uint8_t类型
	return temp;
}

#endif

deviceModule DS1307 = 
{
    .name               = "DS1307",
    .state              = STATE_CLOSE,
    .next               = 0x00,
	.DA                 = Private,
    .d_open             = DS1307_open,
    .d_close            = DS1307_close,
    .d_detect           = DS1307_detect,
    .d_command          = DS1307_command,
    .d_set              = DS1307_set,
    .d_puts             = DS1307_puts,
    .d_gets             = DS1307_gets
};


