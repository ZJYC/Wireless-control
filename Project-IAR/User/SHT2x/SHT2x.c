
#include "board.h"

void LL_SHT2x_Init(void);
float LL_SHT2x_MeasureTempHM(void);
float LL_SHT2x_MeasureHumiHM(void);
float LL_SHT2x_MeasureTempPoll(void);
float LL_SHT2x_MeasureHumiPoll(void);
u8 LL_SHT2x_ReadUserReg(void);
u8 LL_SHT2x_WriteUserReg(u8 reg);
void LL_SHT2x_SoftReset(void);
void LL_SHT2x_GetSerialNumber(u8 *buf);
void LL_SHT2x_Test(void);
static SHT2x_PARAM AHT2xData = {0x00};
/*
****************************************************
*  Function       : d_open_SHT20
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
result d_open_SHT20(void)
{
    LL_SHT2x_Init();
    
    AddPrivateBuf(&SHT20,(uint8_t *)&AHT2xData,sizeof(SHT2x_PARAM));
    
    SET_STATE(SHT20.state,STATE_OPEN);
    RESET_STATE(SHT20.state,STATE_CLOSE);
    return true;
}
/*
****************************************************
*  Function       : d_close_SHT20
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
result d_close_SHT20 (void)
{
    RESET_STATE(SHT20.state,STATE_OPEN);
    SET_STATE(SHT20.state,STATE_CLOSE);
	return true;
}
/*
****************************************************
*  Function       : d_detect_SHT20
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
result d_detect_SHT20 (void)
{
    if(CHECK_STATE(SHT20.state,STATE_CLOSE) && SHT20.d_open() != true)return false;
    
	return true;
}
/*
****************************************************
*  Function       : d_command_SHT20
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
result d_command_SHT20 (uint8_t * Param1, uint32_t Param2)
{
	Param1 = Param1;
	Param2 = Param2;

    if(CHECK_STATE(SHT20.state,STATE_CLOSE) && SHT20.d_open() != true)return false;
	return true;
}
/*
****************************************************
*  Function       : d_set_SHT20
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
result d_set_SHT20 (uint32_t type, uint32_t param)
{
    SHT2x_PARAM * SHT2xData = (SHT2x_PARAM *)SHT20.data;
    
    if(CHECK_STATE(SHT20.state,STATE_CLOSE) && SHT20.d_open() != true)return false;
    
    switch(type)
    {
        case 0:{LL_SHT2x_GetSerialNumber(SHT2xData->SerialNumber);break;}
        case 1:{SHT2xData->Temperature = LL_SHT2x_MeasureTempPoll();break;}
        case 2:{SHT2xData->Humidity = LL_SHT2x_MeasureHumiPoll();break;}
        default:break;
    }

	return true;
}
/*
****************************************************
*  Function       : d_puts_SHT20
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
result d_puts_SHT20 (uint32_t Param1, uint8_t * Param2, uint32_t Param3)
{
	
	Param1 = Param1;
	Param2 = Param2;
	Param3 = Param3;

    if(CHECK_STATE(SHT20.state,STATE_CLOSE) && SHT20.d_open() != true)return false;
    
	return true;
}
/*
****************************************************
*  Function       : d_gets_SHT20
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
result d_gets_SHT20 (uint32_t Param1, uint8_t * Param2, uint32_t Param3)
{
	Param1 = Param1;
	Param2 = Param2;
	Param3 = Param3;
	
    if(CHECK_STATE(SHT20.state,STATE_CLOSE) && SHT20.d_open() != true)return false;
    
	return true;
}
/*
****************************************************
*  Function       : d_timing_proceee_SHT20
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
result d_timing_proceee_SHT20(uint32_t Param1, uint32_t Param2, uint32_t Param3)
{
	Param1 = Param1;
	Param2 = Param2;
	Param3 = Param3;
	
    if(CHECK_STATE(SHT20.state,STATE_CLOSE) && SHT20.d_open() != true)return false;
    
	return true;
}
/*
****************************************************
*  Function       : d_process_it_SHT20
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
result d_process_it_SHT20(uint32_t Param1, uint32_t Param2, uint32_t Param3)
{
	
	Param1 = Param1;
	Param2 = Param2;
	Param3 = Param3;
	
    if(CHECK_STATE(SHT20.state,STATE_CLOSE) && SHT20.d_open() != true)return false;

	return true;
}

result SHT2x_Task(uint32_t Type,uint32_t Param)
{
    
    SHT2x_PARAM * SHT2xData = (SHT2x_PARAM *)SHT20.data;
    
	SHT2xData->Count =  Param;
	 
	if(Task_CalledPeriod == Type && SHT2xData->Count % 2000 == 0)
	{
        //get ...
		SHT20.d_set(1,0);
		SHT20.d_set(2,0);
        
		if(fabs(SHT2xData->TemperatureLast - SHT2xData->Temperature) > 0.5 || fabs(SHT2xData->HumidityLast - SHT2xData->Humidity) > 2)
		{
			SHT2xData->TemperatureLast = SHT2xData->Temperature;
			SHT2xData->HumidityLast = SHT2xData->Humidity;
			osMutexWait(SI4463Mutex,osWaitForever);
			if(board.ops->sync_send_TH(CHANNEL_0) == true)
			{
				Alarm.d_puts(LED2,"10001000",1);
			}
			else
			{
				Alarm.d_puts(LED2,"11110000",1);
			}
			osMutexRelease(SI4463Mutex);
		}
	}

}

#if 1//SHT20µÍ¼¶Çý¶¯

void LL_SHT2x_Delay(u32 n)
{
    uint16_t i;

    
    while(n--)
    {
        for(i = 0; i < 50; i++)
        {
            asm("NOP");
        }
    }
}

void LL_SHT2x_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    SHT2x_SCL_HIGH();
    SHT2x_SDA_HIGH();

    LL_SHT2x_SoftReset();
}

void LL_SHT2x_SCL_OUTPUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin =  SHT2x_SCL_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(SHT2x_I2C_PORT, &GPIO_InitStructure);
}

void LL_SHT2x_SCL_INPUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin =  SHT2x_SCL_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_INPUT;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(SHT2x_I2C_PORT, &GPIO_InitStructure);
}

void LL_SHT2x_SDA_OUTPUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin =  SHT2x_SDA_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(SHT2x_I2C_PORT, &GPIO_InitStructure);
}

void LL_SHT2x_SDA_INPUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin =  SHT2x_SDA_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_INPUT;
    HAL_GPIO_Init(SHT2x_I2C_PORT, &GPIO_InitStructure);
}

void LL_SHT2x_I2cStartCondition(void)
{
    LL_SHT2x_SCL_OUTPUT();
    LL_SHT2x_SDA_OUTPUT();
    
    SHT2x_SDA_HIGH();
    SHT2x_SCL_HIGH();
    SHT2x_SDA_LOW();
    LL_SHT2x_Delay(30);
    SHT2x_SCL_LOW();
    LL_SHT2x_Delay(30);
}

void LL_SHT2x_I2cStopCondition(void)
{
    LL_SHT2x_SCL_OUTPUT();
    LL_SHT2x_SDA_OUTPUT();
    
    SHT2x_SDA_LOW();
    SHT2x_SCL_LOW();
    SHT2x_SCL_HIGH();
    LL_SHT2x_Delay(30);
    SHT2x_SDA_HIGH();
    LL_SHT2x_Delay(30);
}

void LL_SHT2x_I2cAcknowledge(void)
{
    LL_SHT2x_SCL_OUTPUT();
    LL_SHT2x_SDA_OUTPUT();

    SHT2x_SDA_LOW();
    
    SHT2x_SCL_HIGH();
    LL_SHT2x_Delay(30);
    SHT2x_SCL_LOW();   
    LL_SHT2x_Delay(30);
}

void LL_SHT2x_I2cNoAcknowledge(void)
{
    LL_SHT2x_SCL_OUTPUT();
    LL_SHT2x_SDA_OUTPUT();

    SHT2x_SDA_HIGH();
    
    SHT2x_SCL_HIGH();
    LL_SHT2x_Delay(30);
    SHT2x_SCL_LOW();   
    LL_SHT2x_Delay(30);
}

u8 LL_SHT2x_I2cReadByte(void)
{
    u8 i, val = 0;


    LL_SHT2x_SCL_OUTPUT();
    LL_SHT2x_SDA_OUTPUT();

    SHT2x_SDA_HIGH();
    
    LL_SHT2x_SDA_INPUT();
        
    for(i = 0; i < 8; i++)
    {
        val <<= 1; 
        
        SHT2x_SCL_HIGH();   
        
        if(GPIO_PIN_SET == SHT2x_SDA_STATE()) 
        {
            val |= 0x01;
        }
        
        SHT2x_SCL_LOW();  
        LL_SHT2x_Delay(10);
    }

    LL_SHT2x_SDA_OUTPUT();

    SHT2x_SDA_HIGH();
    
    return (val);
}

u8 LL_SHT2x_I2cWriteByte(u8 byte)
{
    u8 i, ack;


    LL_SHT2x_SCL_OUTPUT();
    LL_SHT2x_SDA_OUTPUT();
    
    for(i = 0; i < 8; i++)
    {
        if(byte & 0x80) 
        {
            SHT2x_SDA_HIGH();
        }
        else 
        {
            SHT2x_SDA_LOW();
        }
        
        SHT2x_SCL_HIGH();
        LL_SHT2x_Delay(30);
        SHT2x_SCL_LOW();   
        LL_SHT2x_Delay(30);
        
        byte <<= 1;
    }

    LL_SHT2x_SDA_INPUT();
    
    SHT2x_SCL_HIGH();
    
    if(GPIO_PIN_SET == SHT2x_SDA_STATE()) 
    {
        ack = ACK_ERROR;
    }
    else
    {
        ack = ACK_OK;
    }
    
    SHT2x_SCL_LOW();  

    LL_SHT2x_SDA_OUTPUT();

    SHT2x_SDA_HIGH();
    
    return (ack);
}

float LL_SHT2x_MeasureTempHM(void)
{
    float TEMP;
    u8 tmp1, tmp2;
    u16 ST;
    

    LL_SHT2x_SCL_OUTPUT();
    
    LL_SHT2x_I2cStartCondition(); 
    LL_SHT2x_Delay(1);
    LL_SHT2x_I2cWriteByte(I2C_ADR_W);
    LL_SHT2x_Delay(1);
    LL_SHT2x_I2cWriteByte(TRIG_TEMP_MEASUREMENT_HM);
    LL_SHT2x_Delay(1);
    LL_SHT2x_I2cStartCondition();
    LL_SHT2x_Delay(1);
    LL_SHT2x_I2cWriteByte(I2C_ADR_R);
    LL_SHT2x_Delay(1);
    SHT2x_SCL_HIGH();
    LL_SHT2x_Delay(1);
    LL_SHT2x_SCL_INPUT();

    while(GPIO_PIN_RESET == SHT2x_SCL_STATE())
    {
        LL_SHT2x_Delay(20);
    }
    LL_SHT2x_Delay(1);
    tmp1 = LL_SHT2x_I2cReadByte();
    LL_SHT2x_Delay(1);
    LL_SHT2x_I2cAcknowledge();
    LL_SHT2x_Delay(1);
    tmp2 = LL_SHT2x_I2cReadByte();
    LL_SHT2x_Delay(1);
    LL_SHT2x_I2cNoAcknowledge();
    LL_SHT2x_Delay(1);
    LL_SHT2x_I2cStopCondition();
    LL_SHT2x_Delay(1);
    
    ST = (tmp1 << 8) | (tmp2 << 0);
    ST &= ~0x0003;
    TEMP = ((float)ST * 0.00268127) - 46.85;

    LL_SHT2x_SCL_OUTPUT();

    return (TEMP);	  
}

float LL_SHT2x_MeasureHumiHM(void)
{
    float HUMI;
    u8 tmp1, tmp2;    
    u16 SRH;


    LL_SHT2x_SCL_OUTPUT();
    
    LL_SHT2x_I2cStartCondition();                               
    LL_SHT2x_I2cWriteByte(I2C_ADR_W);
    LL_SHT2x_I2cWriteByte(TRIG_HUMI_MEASUREMENT_HM);

    LL_SHT2x_I2cStartCondition();
    LL_SHT2x_I2cWriteByte(I2C_ADR_R);

    SHT2x_SCL_HIGH();

    LL_SHT2x_SCL_INPUT();

    while(GPIO_PIN_RESET == SHT2x_SCL_STATE())
    {
        LL_SHT2x_Delay(20);
    }
    
    tmp1 = LL_SHT2x_I2cReadByte();
    LL_SHT2x_I2cAcknowledge();
    tmp2 = LL_SHT2x_I2cReadByte();
    LL_SHT2x_I2cNoAcknowledge();
    LL_SHT2x_I2cStopCondition();
    
    SRH = (tmp1 << 8) | (tmp2 << 0);
    SRH &= ~0x0003;
    HUMI = ((float)SRH * 0.00190735) - 6;

    LL_SHT2x_SCL_OUTPUT();

    return (HUMI);
}

float LL_SHT2x_MeasureTempPoll(void)
{
    float TEMP;
    u8 ack, tmp1, tmp2;
    u16 ST;
    
    
    LL_SHT2x_I2cStartCondition();                            
    LL_SHT2x_I2cWriteByte(I2C_ADR_W);
    LL_SHT2x_I2cWriteByte(TRIG_TEMP_MEASUREMENT_POLL);

    do {
        LL_SHT2x_Delay(20);  
        //osDelay(10);
        LL_SHT2x_I2cStartCondition();
        ack = LL_SHT2x_I2cWriteByte(I2C_ADR_R);
    } while(ACK_ERROR == ack);
    
    tmp1 = LL_SHT2x_I2cReadByte();
    LL_SHT2x_I2cAcknowledge();
    tmp2 = LL_SHT2x_I2cReadByte();
    LL_SHT2x_I2cNoAcknowledge();
    LL_SHT2x_I2cStopCondition();
    
    ST = (tmp1 << 8) | (tmp2 << 0);
    ST &= ~0x0003;
    TEMP = ((float)ST * 0.00268127) - 46.85;

    return (TEMP);	  
}

float LL_SHT2x_MeasureHumiPoll(void)
{
    float HUMI;
    u8 ack, tmp1, tmp2;    
    u16 SRH;

    LL_SHT2x_I2cStartCondition();                               
    LL_SHT2x_I2cWriteByte(I2C_ADR_W);
    LL_SHT2x_I2cWriteByte(TRIG_HUMI_MEASUREMENT_POLL);
    
    do 
	{
        osDelay(10);
        LL_SHT2x_I2cStartCondition();
        ack = LL_SHT2x_I2cWriteByte(I2C_ADR_R);
    } while(ACK_ERROR == ack);
    
    tmp1 = LL_SHT2x_I2cReadByte();
    LL_SHT2x_I2cAcknowledge();
    tmp2 = LL_SHT2x_I2cReadByte();
    LL_SHT2x_I2cNoAcknowledge();
    LL_SHT2x_I2cStopCondition();
    
    SRH = (tmp1 << 8) | (tmp2 << 0);
    SRH &= ~0x0003;
    HUMI = ((float)SRH * 0.00190735) - 6;

    return (HUMI);
}

u8 LL_SHT2x_ReadUserReg(void)    
{
    u8 reg;

    LL_SHT2x_I2cStartCondition();                 
    LL_SHT2x_I2cWriteByte(I2C_ADR_W);
    LL_SHT2x_I2cWriteByte(USER_REG_R);
    LL_SHT2x_I2cStartCondition(); 
    LL_SHT2x_I2cWriteByte(I2C_ADR_R);
    reg = LL_SHT2x_I2cReadByte();
    LL_SHT2x_I2cNoAcknowledge();
    LL_SHT2x_I2cStopCondition();
    
    return (reg); 
}

u8 LL_SHT2x_WriteUserReg(u8 reg)
{
    u8 ack;
    LL_SHT2x_I2cStartCondition();
    LL_SHT2x_I2cWriteByte(I2C_ADR_W);
    LL_SHT2x_I2cWriteByte(USER_REG_W);
    ack = LL_SHT2x_I2cWriteByte(reg);
    LL_SHT2x_I2cStopCondition();  
    
    return (ack);
}

void LL_SHT2x_SoftReset(void)
{
    LL_SHT2x_I2cStartCondition();
    LL_SHT2x_I2cWriteByte(I2C_ADR_W);
    LL_SHT2x_I2cWriteByte(SOFT_RESET);
    LL_SHT2x_I2cStopCondition();
}

void LL_SHT2x_GetSerialNumber(u8 *buf)
{
    /* Read from memory location 1 */
    LL_SHT2x_I2cStartCondition();
    LL_SHT2x_I2cWriteByte(I2C_ADR_W); //I2C address
    LL_SHT2x_I2cWriteByte(0xFA); //Command for readout on-chip memory
    LL_SHT2x_I2cWriteByte(0x0F); //on-chip memory address
    LL_SHT2x_I2cStartCondition();
    LL_SHT2x_I2cWriteByte(I2C_ADR_R); //I2C address
    buf[5] = LL_SHT2x_I2cReadByte(); //Read SNB_3
    LL_SHT2x_I2cAcknowledge();
    LL_SHT2x_I2cReadByte(); //Read CRC SNB_3 (CRC is not analyzed)
    LL_SHT2x_I2cAcknowledge();
    buf[4] = LL_SHT2x_I2cReadByte(); //Read SNB_2
    LL_SHT2x_I2cAcknowledge();
    LL_SHT2x_I2cReadByte(); //Read CRC SNB_2 (CRC is not analyzed)
    LL_SHT2x_I2cAcknowledge();
    buf[3] = LL_SHT2x_I2cReadByte(); //Read SNB_1
    LL_SHT2x_I2cAcknowledge();
    LL_SHT2x_I2cReadByte(); //Read CRC SNB_1 (CRC is not analyzed)
    LL_SHT2x_I2cAcknowledge();
    buf[2] = LL_SHT2x_I2cReadByte(); //Read SNB_0
    LL_SHT2x_I2cAcknowledge();
    LL_SHT2x_I2cReadByte(); //Read CRC SNB_0 (CRC is not analyzed)
    LL_SHT2x_I2cNoAcknowledge();
    LL_SHT2x_I2cStopCondition();

    /* Read from memory location 2 */
    LL_SHT2x_I2cStartCondition();
    LL_SHT2x_I2cWriteByte(I2C_ADR_W); //I2C address
    LL_SHT2x_I2cWriteByte(0xFC); //Command for readout on-chip memory
    LL_SHT2x_I2cWriteByte(0xC9); //on-chip memory address
    LL_SHT2x_I2cStartCondition();
    LL_SHT2x_I2cWriteByte(I2C_ADR_R); //I2C address
    buf[1] = LL_SHT2x_I2cReadByte(); //Read SNC_1
    LL_SHT2x_I2cAcknowledge();
    buf[0] = LL_SHT2x_I2cReadByte(); //Read SNC_0
    LL_SHT2x_I2cAcknowledge();
    LL_SHT2x_I2cReadByte(); //Read CRC SNC0/1 (CRC is not analyzed)
    LL_SHT2x_I2cAcknowledge();
    buf[7] = LL_SHT2x_I2cReadByte(); //Read SNA_1
    LL_SHT2x_I2cAcknowledge();
    buf[6] = LL_SHT2x_I2cReadByte(); //Read SNA_0
    LL_SHT2x_I2cAcknowledge();
    LL_SHT2x_I2cReadByte(); //Read CRC SNA0/1 (CRC is not analyzed)
    LL_SHT2x_I2cNoAcknowledge();
    LL_SHT2x_I2cStopCondition();
}

#endif

deviceModule SHT20 = 
{
    .name               = "SHT20",
    .state              = STATE_CLOSE,
    .next               = 0x00,
	.DA                 = Private,
    .d_open             = d_open_SHT20,
    .d_close            = d_close_SHT20,
    .d_detect           = d_detect_SHT20,
    .d_command          = d_command_SHT20,
    .d_set              = d_set_SHT20,
    .d_puts             = d_puts_SHT20,
    .d_gets             = d_gets_SHT20,
    .d_timing_proceee   = d_timing_proceee_SHT20,
	.d_process_it       = d_process_it_SHT20,
    .Task               = SHT2x_Task
};
