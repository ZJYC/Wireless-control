
#include "Board.h"

result p_open_spi_1(void);
result p_close_spi_1(void);
result p_ioctl_spi_1(uint32_t type,uint32_t param);
result p_read_spi_1(uint8_t * start,uint32_t length);
result p_write_spi_1(uint8_t * start,uint32_t length);
uint8_t p_rw_spi_1(uint8_t value);

result p_open_II2C_1(void);
result p_close_II2C_1(void);
result p_ioctl_II2C_1(uint32_t type,uint32_t param);
result p_read_II2C_1(uint8_t * start,uint32_t length);
result p_write_II2C_1(uint8_t * start,uint32_t length);

#if 1//链表操作

void device_add(p_deviceModule new_device)
{
    p_deviceModule temp;
	//空针返回
	if(new_device == 0x00)return;
	//创建
	if(board.device_list == 0x00)
	{
		board.device_list = new_device;
		return;
	}
	//连接
    temp = board.device_list;
    while(temp->next != 0x00)temp = temp->next;
    
    temp->next = new_device;
}

void device_delete(p_deviceModule del_device)
{
	p_deviceModule temp = board.device_list;
	
	//空针返回
	if(del_device == 0x00 || board.device_list == 0x00)return;
	
	if(board.device_list->next == 0){board.device_list = 0;return;}
	
	while(temp->next != del_device || temp->next == 0)temp = temp->next;
	
	if(temp->next == 0)return;
	
	temp->next = del_device->next;
	
}

p_deviceModule device_find_local(uint8_t * name)
{
	p_deviceModule temp = board.device_list;

	if(temp == 0)return (p_deviceModule)0x00;

	while(temp)
	{
		//核对名称
		if(strcmp((char const *)name,(char const *)temp->name) == 0)
		{
			//核对主从号
			//if(major == temp->major && minor == temp->minor)
            return temp;
		}
		temp = temp->next;
	}
	return (p_deviceModule)0x00;
}

#endif

result AddPrivateBuf(p_deviceModule Device,uint8_t * Buf,uint32_t BufLen)
{
	if(Buf == 0 || BufLen == 0)return false;
	Device->data = Buf;
	Device->DataLen = BufLen;
	return true;
}

#if	1//SPI1

peripherals spi_1 = 
{
	"spi_1",
	0,
	0,
	STATE_CLOSE,
	/* 操作 */
	p_open_spi_1,
	p_close_spi_1,
	p_ioctl_spi_1,
	p_read_spi_1,
	p_write_spi_1,
    p_rw_spi_1
};

#if	1	//SPI_1的驱动程序

result p_open_spi_1(void)
{
    //init the peripherals
	MX_SPI1_Init();
    //modify the state
    spi_1.state = STATE_OPEN;
    {
        GPIO_InitTypeDef GPIO_InitStruct;
        GPIO_InitStruct.Pin = GPIO_PIN_12;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    }
	return true;
}
result p_close_spi_1(void)
{
    //Deinit the peripherals
	HAL_SPI_DeInit(&hspi1);
	HAL_SPI_MspDeInit(&hspi1);
    //modify the state
    spi_1.state = STATE_CLOSE;
	return true;
}
result p_ioctl_spi_1(uint32_t type,uint32_t param)
{
    //if closed,open it now
    if(spi_1.state != STATE_OPEN)
    {
        //can not open it,return false
        if(spi_1.p_open() != true)return false;
    }
    //user code begin...
    
    //user code end...
	return true;
}
result p_read_spi_1(uint8_t * start,uint32_t length)
{
    //if closed,open it now
    if(spi_1.state != STATE_OPEN)
    {
        //can not open it,return false
        if(spi_1.p_open() != true)return false;
    }
    
    //user code begin...
	if(HAL_OK == HAL_SPI_TransmitReceive(&hspi2,(uint8_t *)start,(uint8_t *)start,length,10000))return true;
    
	return false;    
    //user code end...
}
result p_write_spi_1(uint8_t * start,uint32_t length)
{
    
    //if closed,open it now
    if(spi_1.state != STATE_OPEN)
    {
        //can not open it,return false
        if(spi_1.p_open() != true)return false;
    }
    //user code begin...

	if(HAL_OK == HAL_SPI_Transmit(&hspi2,(uint8_t *)start,length,10000))return true;
    
	return false;
    
    //user code end...
}

uint8_t p_rw_spi_1(uint8_t value)
{
    uint8_t temp = value;
    //if closed,open it now
    if(spi_1.state != STATE_OPEN)
    {
        //can not open it,return false
        if(spi_1.p_open() != true)return false;
    }
    //user code begin...
     HAL_SPI_TransmitReceive(&hspi2,&temp,&temp,1,10000);
    return temp;
    //user code end...
}

#endif

#endif


#if 1//I2C1

uint8_t DeviceAddress = 0x00;

peripherals II2C_1 = 
{
	"II2C_1",
	0,
	0,
	STATE_CLOSE,
	/* 操作 */
	p_open_II2C_1,
	p_close_II2C_1,
	p_ioctl_II2C_1,
	p_read_II2C_1,
	p_write_II2C_1
};

result p_open_II2C_1(void)
{
	//MX_I2C1_Init();
	return true;
}
result p_close_II2C_1(void)
{
	//HAL_I2C_MspDeInit(&hi2c1);
	return true;
}
result p_ioctl_II2C_1(uint32_t type,uint32_t param)
{
	switch(type)
	{
		case 1:{DeviceAddress = param;break;}
		case 2:{break;}
		default:break;
	}
	return true;
}
result p_read_II2C_1(uint8_t * start,uint32_t length)
{
	//if(HAL_OK != HAL_I2C_Master_Receive(&hi2c1,DeviceAddress,start,length,0))return false;
    return true;
}
result p_write_II2C_1(uint8_t * start,uint32_t length)
{
	//if(HAL_OK != HAL_I2C_Master_Transmit(&hi2c1,DeviceAddress,start,length,0))return false;
    return true;
}
#endif















