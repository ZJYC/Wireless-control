/*===========================================================================
Function : Operation for SI446x
http://shop57165217.taobao.com
============================================================================*/
#include "si446x.h"
#include "radio_config_Si446x_1k_26M.h"//26m 晶振
//#include "radio_config_Si446x_1k_30M.h"  //30m 晶振

const INT8U config_table_1k[] = RADIO_CONFIGURATION_DATA_ARRAY;

/*read a array of command response*/
INT8U SI446X_READ_RESPONSE(INT8U *buffer, INT8U size);

/*wait the device ready to response a command*/
INT8U SI446X_WAIT_CTS(void);

/*write data to TX fifo*/
INT8U SI446X_W_TX_FIFO(INT8U *txbuffer, INT8U size);

/*Exchange a byte via the SPI bus*/
INT8U BSP_SPI_ExchangeByte(INT8U input);


void SI4463_Delay(uint32_t cnt)
{
    uint32_t i = 50;
    while(cnt -- )
    {
        while(i --);
        i = 50;
    }
}


/* ZJYC added */
INT8U BSP_SPI_ExchangeByte(INT8U input)
{
	//uint8_t temp = input;
	return spi_1.p_get_put(input);
}

/*===========================================================================
SI446X_WAIT_CTS();
Function : wait the device ready to response a command
============================================================================*/
INT8U SI446X_WAIT_CTS(void)
{
    INT8U cnt1 = 0,cnt2 = 0;
    INT8U cts;
    do
    {
        SI_CSN_LOW();
        //SI4463_Delay(40);
        BSP_SPI_ExchangeByte(READ_CMD_BUFF);
        cts = BSP_SPI_ExchangeByte(0xff);
        SI_CSN_HIGH();
        cnt1++;
        if(cnt1 >= 10)
		{
			cnt1 = 0;
			cnt2++;
			if(cnt2 >= 10)return false;
		}
    }while(cts != 0xFF);
	return true;
}

/*===========================================================================
SI446X_CMD();
Function : Send a command to the device
INTPUT   : cmd, the buffer stores the command array
           cmdsize, the size of the command array
============================================================================*/
INT8U SI446X_CMD(INT8U *cmd, INT8U cmdsize)
{
    if(SI446X_WAIT_CTS() != true)return false;
    SI_CSN_LOW();
	//SI4463_Delay(200);
	//HAL_Delay(10);
    while (cmdsize--)   { BSP_SPI_ExchangeByte(*cmd++);}
	//SI4463_Delay(400);
    SI_CSN_HIGH();
	//SI4463_Delay(400);
	return true;
}

/*===========================================================================
SI446X_POWER_UP();
Function : Power up the device
INTPUT   : f_xtal, the frequency of the external high-speed crystal
============================================================================*/
INT8U SI446X_POWER_UP(INT32U f_xtal)
{
    INT8U cmd[7];
    cmd[0] = POWER_UP;
    cmd[1] = 0x01;
    cmd[2] = 0x00;
    cmd[3] = f_xtal>>24;
    cmd[4] = f_xtal>>16;
    cmd[5] = f_xtal>>8;
    cmd[6] = f_xtal;
    return SI446X_CMD(cmd, 7);
}
/*===========================================================================
SI446X_READ_RESPONSE();
Function : read a array of command response
INTPUT   : buffer,  a buffer, stores the data responsed
           size,    How many bytes should be read
============================================================================*/
INT8U SI446X_READ_RESPONSE(INT8U *buffer, INT8U size)
{
    if(SI446X_WAIT_CTS() != true)return false;
    SI_CSN_LOW();
    BSP_SPI_ExchangeByte(READ_CMD_BUFF);
	while (size--)  { *buffer++ = BSP_SPI_ExchangeByte(0xFF);}
    SI_CSN_HIGH();
	return true;
}

/*===========================================================================
SI446X_NOP();
Function : NO Operation command
============================================================================*/
INT8U SI446X_NOP(void)
{
    INT8U cts;
    SI_CSN_LOW();
    cts = BSP_SPI_ExchangeByte(NOP);
    SI_CSN_HIGH();
	return cts;
}

/*===========================================================================
SI446X_PART_INFO();
Function : Read the PART_INFO of the device, 8 bytes needed
INTPUT   : buffer, the buffer stores the part information
============================================================================*/
INT8U SI446X_PART_INFO(INT8U *buffer)
{
    INT8U cmd = PART_INFO;
    if(SI446X_CMD(&cmd, 1) != true)return false;
    return SI446X_READ_RESPONSE(buffer, 8);
}

/*===========================================================================
SI446X_FUNC_INFO();
Function : Read the FUNC_INFO of the device, 7 bytes needed
INTPUT   : buffer, the buffer stores the FUNC information
============================================================================*/
INT8U SI446X_FUNC_INFO(INT8U *buffer)
{
    INT8U cmd = FUNC_INFO;
    if(SI446X_CMD(&cmd, 1) != true)return false;
    return SI446X_READ_RESPONSE(buffer, 7);
}

/*===========================================================================
SI446X_INT_STATUS();
Function : Read the INT status of the device, 9 bytes needed
INTPUT   : buffer, the buffer stores the int status
============================================================================*/
INT8U SI446X_INT_STATUS(INT8U *buffer)
{
    INT8U cmd[4];
    cmd[0] = GET_INT_STATUS;
    cmd[1] = 0;
    cmd[2] = 0;
    cmd[3] = 0;
    if(SI446X_CMD(cmd, 4) != true)return false;
    return SI446X_READ_RESPONSE(buffer, 9);
}

/*===========================================================================
SI446X_GET_PROPERTY();
Function : Read the PROPERTY of the device
INTPUT   : buffer, the buffer stores the PROPERTY value
           GROUP_NUM, the group and number of the parameter
           NUM_GROUP, number of the group
============================================================================*/
INT8U SI446X_GET_PROPERTY_X(SI446X_PROPERTY GROUP_NUM, INT8U NUM_PROPS, 
                           INT8U *buffer)
{
    INT8U cmd[4];

    cmd[0] = GET_PROPERTY;
    cmd[1] = GROUP_NUM>>8;
    cmd[2] = NUM_PROPS;
    cmd[3] = GROUP_NUM;

    if(SI446X_CMD(cmd, 4) != true)return false;
    return SI446X_READ_RESPONSE(buffer, NUM_PROPS + 1);
}
    
/*===========================================================================
SI446X_SET_PROPERTY_X();
Function : Set the PROPERTY of the device
INTPUT   : GROUP_NUM, the group and the number of the parameter
           NUM_GROUP, number of the group
           PAR_BUFF, buffer stores parameters
============================================================================*/
INT8U SI446X_SET_PROPERTY_X(SI446X_PROPERTY GROUP_NUM, INT8U NUM_PROPS, INT8U *PAR_BUFF)
{
    INT8U cmd[20], i = 0;
    if (NUM_PROPS >= 16)    { return false; }
    cmd[i++] = SET_PROPERTY;
    cmd[i++] = GROUP_NUM>>8;
    cmd[i++] = NUM_PROPS;
    cmd[i++] = GROUP_NUM;
    while (NUM_PROPS--)     { cmd[i++] = *PAR_BUFF++; }
    return SI446X_CMD(cmd, i);
}
    
/*===========================================================================
SI446X_SET_PROPERTY_1();
Function : Set the PROPERTY of the device, only 1 byte
INTPUT   : GROUP_NUM, the group and number index
           prioriry,  the value to be set
============================================================================*/
INT8U SI446X_SET_PROPERTY_1(SI446X_PROPERTY GROUP_NUM, INT8U proirity)
{
    INT8U cmd[5];

    cmd[0] = SET_PROPERTY;
    cmd[1] = GROUP_NUM>>8;
    cmd[2] = 1;
    cmd[3] = GROUP_NUM;
    cmd[4] = proirity;
    return SI446X_CMD(cmd, 5);
}
    
/*===========================================================================
SI446X_GET_PROPERTY_1();
Function : Get the PROPERTY of the device, only 1 byte
INTPUT   : GROUP_NUM, the group and number index
OUTPUT   : the PROPERTY value read from device
============================================================================*/
INT8U SI446X_GET_PROPERTY_1(SI446X_PROPERTY GROUP_NUM)
{
    INT8U cmd[4];

    cmd[0] = GET_PROPERTY;
    cmd[1] = GROUP_NUM>>8;
    cmd[2] = 1;
    cmd[3] = GROUP_NUM;
    SI446X_CMD(cmd, 4);
    SI446X_READ_RESPONSE(cmd, 2);
    return cmd[1];
}
    
/*===========================================================================
SI446X_RESET();
Function : reset the SI446x device
============================================================================*/
INT8U SI446X_RESET(void)
{
    SI_SDN_HIGH();
    SI4463_Delay(10);
    SI_SDN_LOW();
    SI_CSN_HIGH();
    return true;
}
    
/*===========================================================================
SI446X_CONFIG_INIT();
Function : configuration the device
============================================================================*/
INT8U SI446X_CONFIG_INIT(void)
{
    INT8U i;
    INT16U j = 0;
    const INT8U   *ptr = config_table_1k;


    while((i = *(ptr+j)) != 0)
    {
        j += 1;
        SI446X_CMD((INT8U*)ptr + j, i);
        //IWDG_ReloadCounter();
        j += i;
    }
#if PACKET_LENGTH > 0           //fixed packet length
    SI446X_SET_PROPERTY_1(PKT_FIELD_1_LENGTH_7_0, PACKET_LENGTH);
#else                           //variable packet length
    SI446X_SET_PROPERTY_1(PKT_CONFIG1, 0x00);
    SI446X_SET_PROPERTY_1(PKT_CRC_CONFIG, 0x00);
    SI446X_SET_PROPERTY_1(PKT_LEN_FIELD_SOURCE, 0x01);
    SI446X_SET_PROPERTY_1(PKT_LEN, 0x2A);
    SI446X_SET_PROPERTY_1(PKT_LEN_ADJUST, 0x00);
    SI446X_SET_PROPERTY_1(PKT_FIELD_1_LENGTH_12_8, 0x00);
    SI446X_SET_PROPERTY_1(PKT_FIELD_1_LENGTH_7_0, 0x01);
    SI446X_SET_PROPERTY_1(PKT_FIELD_1_CONFIG, 0x00);
    SI446X_SET_PROPERTY_1(PKT_FIELD_1_CRC_CONFIG, 0x00);
    SI446X_SET_PROPERTY_1(PKT_FIELD_2_LENGTH_12_8, 0x00);
    SI446X_SET_PROPERTY_1(PKT_FIELD_2_LENGTH_7_0, 0x10);
    SI446X_SET_PROPERTY_1(PKT_FIELD_2_CONFIG, 0x00);
    SI446X_SET_PROPERTY_1(PKT_FIELD_2_CRC_CONFIG, 0x00);
#endif //PACKET_LENGTH

    //重要： 4463的GDO2，GDO3控制射频开关，
    //发射时必须： GDO2=1  GDO3=0
    //接收时必须： GDO2=0，GDO3=1
    //所以，以下语句对于4463是必须的，4460或4461不需要射频开关，则可以去掉此语句
    SI446X_GPIO_CONFIG(35, 24, 33|0x40, 32|0x40, 0, 0, 0);    //重要
    //SI446X_GPIO_CONFIG(35, 24, 0x72, 0x73, 0, 0, 0);
    return true;
}
    
/*===========================================================================
SI446X_W_TX_FIFO();
Function : write data to TX fifo
INTPUT   : txbuffer, a buffer stores TX array
           size,  how many bytes should be written
============================================================================*/
INT8U SI446X_W_TX_FIFO(INT8U *txbuffer, INT8U size)
{
    SI_CSN_LOW();
    BSP_SPI_ExchangeByte(WRITE_TX_FIFO);
    while (size--)      { BSP_SPI_ExchangeByte(*txbuffer++);}
    SI_CSN_HIGH();
    return true;
}
    
/*===========================================================================
SI446X_SEND_PACKET();
Function : send a packet
INTPUT   : txbuffer, a buffer stores TX array
           size,  how many bytes should be written
           channel, tx channel
           condition, tx condition
============================================================================*/
INT8U SI446X_SEND_PACKET(INT8U *txbuffer, INT8U size, INT8U channel, INT8U condition)
{
    INT8U tx_len = size,cnt = 0;

    if(SI446X_TX_FIFO_RESET() != true)return false;
	if(SI446X_RX_FIFO_RESET() != true)return false;

    if(SI446X_CHANGE_STATE(2) != true)return false;
    while(SI446X_GET_DEVICE_STATE() != 2){cnt++;if(cnt >= 10)return false;}

	if(SI446X_WAIT_CTS() != true)return false;
    SI_CSN_LOW();
    BSP_SPI_ExchangeByte(WRITE_TX_FIFO);
#if PACKET_LENGTH == 0
    tx_len ++;
    BSP_SPI_ExchangeByte(size);
#endif
    while(size --)    { BSP_SPI_ExchangeByte(*txbuffer++);}
    SI_CSN_HIGH();

	return SI446X_START_TX(channel, condition, tx_len);
}
    
/*===========================================================================
SI446X_START_TX();
Function : start TX command
INTPUT   : channel, tx channel
           condition, tx condition
           tx_len, how many bytes to be sent
============================================================================*/
INT8U SI446X_START_TX(INT8U channel, INT8U condition, INT16U tx_len)
{
    INT8U cmd[6],cnt = 0;

    if(SI446X_CHANGE_STATE(2) != true)return false;
    while (SI446X_GET_DEVICE_STATE() != 2){cnt++;if(cnt >= 10)return false;}
    if(SI446X_CHANGE_STATE(5) != true)return false;
    while (SI446X_GET_DEVICE_STATE() != 5){cnt++;if(cnt >= 10)return false;}
    cmd[0] = START_TX;
    cmd[1] = channel;
    cmd[2] = condition;
    cmd[3] = tx_len>>8;
    cmd[4] = tx_len;
    cmd[5] = 128;
    return SI446X_CMD(cmd, 6);
}
    
/*===========================================================================
SI446X_READ_PACKET();
Function : read RX fifo
INTPUT   : buffer, a buffer to store data read
OUTPUT   : received bytes
============================================================================*/
INT8U SI446X_READ_PACKET(INT8U *buffer)
{
    INT8U length, i;
    SI446X_WAIT_CTS();
    SI_CSN_LOW();

    BSP_SPI_ExchangeByte(READ_RX_FIFO);
#if PACKET_LENGTH == 0
    length = BSP_SPI_ExchangeByte(0xFF);
#else
    length = PACKET_LENGTH;
#endif
    i = length;

    while (length--)    { *buffer++ = BSP_SPI_ExchangeByte(0xFF);}
    SI_CSN_HIGH();
    return i;
}
    
/*===========================================================================
SI446X_START_RX();
Function : start RX state
INTPUT   : channel, receive channel
           condition, receive condition
           rx_len, how many bytes should be read
           n_state1, next state 1
           n_state2, next state 2
           n_state3, next state 3
============================================================================*/
INT8U SI446X_START_RX(INT8U channel, INT8U condition, INT16U rx_len,
                      INT8U n_state1, INT8U n_state2, INT8U n_state3)
{
    INT8U cmd[8],cnt = 0;
    if(SI446X_RX_FIFO_RESET() != true)return false;
    if(SI446X_TX_FIFO_RESET() != true)return false;

    if(SI446X_CHANGE_STATE(2) != true)return false;
    while (SI446X_GET_DEVICE_STATE() != 2){cnt++;if(cnt >= 10)return false;};
    if(SI446X_CHANGE_STATE(6) != true)return false;
    while (SI446X_GET_DEVICE_STATE() != 6){cnt++;if(cnt >= 10)return false;};
    cmd[0] = START_RX;
    cmd[1] = channel;
    cmd[2] = condition;
    cmd[3] = rx_len>>8;
    cmd[4] = rx_len;
    cmd[5] = n_state1;
    cmd[6] = n_state2;
    cmd[7] = n_state3;
    return SI446X_CMD(cmd, 8);
}
    
/*===========================================================================
SI446X_RX_FIFO_RESET();
Function : reset the RX FIFO of the device
============================================================================*/
INT8U SI446X_RX_FIFO_RESET(void)
{
    INT8U cmd[2];

    cmd[0] = FIFO_INFO;
    cmd[1] = 0x02;
    return SI446X_CMD(cmd, 2);
}
    
/*===========================================================================
SI446X_TX_FIFO_RESET();
Function : reset the TX FIFO of the device
============================================================================*/
INT8U SI446X_TX_FIFO_RESET(void)
{
    INT8U cmd[2];

    cmd[0] = FIFO_INFO;
    cmd[1] = 0x01;
    return SI446X_CMD(cmd, 2);
}
    
/*===========================================================================
SI446X_PKT_INFO();
Function : read packet information
INTPUT   : buffer, stores the read information
           FIELD, feild mask
           length, the packet length
           diff_len, diffrence packet length
============================================================================*/
INT8U SI446X_PKT_INFO(INT8U *buffer, INT8U FIELD, INT16U length, INT16U diff_len)
{
    INT8U cmd[6];
    cmd[0] = PACKET_INFO;
    cmd[1] = FIELD;
    cmd[2] = length >> 8;
    cmd[3] = length;
    cmd[4] = diff_len >> 8;
    cmd[5] = diff_len;

    if(SI446X_CMD(cmd, 6) != true)return false;
    return SI446X_READ_RESPONSE(buffer, 3);
}
    
/*===========================================================================
SI446X_FIFO_INFO();
Function : read fifo information
INTPUT   : buffer, stores the read information
============================================================================*/
INT8U SI446X_FIFO_INFO(INT8U *buffer)
{
    INT8U cmd[2];
    cmd[0] = FIFO_INFO;
    cmd[1] = 0x03;

    if(SI446X_CMD(cmd, 2) != true)return false;
    return SI446X_READ_RESPONSE(buffer, 3);
}
    
/*===========================================================================
SI446X_GPIO_CONFIG();
Function : config the GPIOs, IRQ, SDO
============================================================================*/
INT8U SI446X_GPIO_CONFIG(INT8U G0, INT8U G1, INT8U G2, INT8U G3,
                        INT8U IRQ, INT8U SDO, INT8U GEN_CONFIG)
{
    INT8U cmd[10];
    cmd[0] = GPIO_PIN_CFG;
    cmd[1] = G0;
    cmd[2] = G1;
    cmd[3] = G2;
    cmd[4] = G3;
    cmd[5] = IRQ;
    cmd[6] = SDO;
    cmd[7] = GEN_CONFIG;
    if(SI446X_CMD(cmd, 8) != true)return false;
    return SI446X_READ_RESPONSE(cmd, 8);
}
/*===========================================================================
SI446X_CHANGE_STATE();
Function : change a new state
INTPUT   : NewState, input the new state
============================================================================*/
INT8U SI446X_CHANGE_STATE(INT8U NewState)
{
    INT8U cmd[2];
    cmd[0] = 0x34;
    cmd[1] = NewState;
    return SI446X_CMD(cmd, 2);
}
/*===========================================================================
SI446X_CHIP_STATUS();
Function : Get the chip status
INTPUT   : buffer_8, a buffer to store the read date
============================================================================*/
INT8U SI446X_CHIP_STATUS(INT8U* buffer_8)
{
    INT8U cmd[2];
    cmd[0] = GET_CHIP_STATUS;
    cmd[1] = 0xFF;
    if(SI446X_CMD(cmd, 8) != true)return false;
    return SI446X_READ_RESPONSE(buffer_8, 4);
}
/*===========================================================================
SI446X_SET_SYNC();
Function : Set the SYNC bytes of the device
INTPUT   : sync, the SYNC word
============================================================================*/
INT8U SI446X_SET_SYNC(INT32U sync)
{
    SI446X_SET_PROPERTY_1(SYNC_BITS_31_24, sync>>24);
    SI446X_SET_PROPERTY_1(SYNC_BITS_23_16, sync>>16);
    SI446X_SET_PROPERTY_1(SYNC_BITS_15_8,  sync>>8);
    SI446X_SET_PROPERTY_1(SYNC_BITS_7_0,   sync   );
    return true;
}
    
/*===========================================================================
SI446X_SET_POWER();
Function : Set the PA Power of the device
INTPUT   : Power_Level,  0- 7F
============================================================================*/
INT8U SI446X_SET_POWER(INT8U Power_Level)
{
    SI446X_SET_PROPERTY_1(PA_PWR_LVL, Power_Level);
    return true;
}

INT8U SI446X_GET_DEVICE_STATE(void)
{
   INT8U cmd[3];

   cmd[0] = REQUEST_DEVICE_STATE;
   SI446X_CMD(cmd, 1);
   SI446X_READ_RESPONSE(cmd, 3);
   return cmd[1] & 0x0F;
}
    
/*===========================================================================
------------------------------------End of FILE----------------------------------
=================================================================================
*/
