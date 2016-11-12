
#include "Board.h"

RfPrivateDataTypedef RFPD = 
{
    { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 },
    0,
};
     
//uint8_t AckBuffer[ACK_LENGTH] = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };

void SI4463_Delay(uint32_t cnt);

/*
****************************************************
*  Function       : d_open_si4463
*  Description    : init SI4463
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : set the module at RX state
*  Author         : 
*****************************************************
*/
static result d_open_si4463(void)
{
    //first open the SPI peripherals...
	if(spi_1.p_open() != true)return false;

	SI446X_RESET();  
    // config the module basic on the header file...the header was generated by WDS
    SI446X_CONFIG_INIT();  
    // set the power to max
    SI446X_SET_POWER(0x7F); 
     //     
    SI446X_START_RX(board.board_Local_channel, 0, PACKET_LENGTH, 8, 8, 8);
    //
    AddPrivateBuf(&si4463,(uint8_t *)&RFPD,sizeof(RfPrivateDataTypedef));
    //
    SET_STATE(si4463.state,STATE_OPEN);
    RESET_STATE(si4463.state,STATE_CLOSE);
    
	return true;
}
/*
****************************************************
*  Function       : d_close_si4463
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : Nothing to do here 
*  Author         : 
*****************************************************
*/
static result d_close_si4463(void)
{
    SET_STATE(si4463.state,STATE_CLOSE);
    RESET_STATE(si4463.state,STATE_OPEN);
	return true;
}
/*
****************************************************
*  Function       : d_detect_si4463
*  Description    : detect if the device is exist
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : Nothing to do here
*  Author         : 
*****************************************************
*/
static result d_detect_si4463(void)
{
    if(CHECK_STATE(si4463.state,STATE_CLOSE) && si4463.d_open() != true)return false;
    
	return true;
}
/*
****************************************************
*  Function       : d_command_si4463
*  Description    : we can send some command to some device
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : Nothing to do here
*  Author         : 
*****************************************************
*/
static result d_command_si4463 (uint8_t * Param1, uint32_t Param2)
{
    if(CHECK_STATE(si4463.state,STATE_CLOSE) && si4463.d_open() != true)return false;	
    
	Param1 = Param1;
	Param2 = Param2;

	return true;
}


/*
****************************************************
*  Function       : d_set_si4463
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : Nothing to do here
*  Author         : 
*****************************************************
*/
static result d_set_si4463(uint32_t Param1,uint32_t Param2)
{
    if(CHECK_STATE(si4463.state,STATE_CLOSE) && si4463.d_open() != true)return false;
	
	Param1 = Param1;
	Param2 = Param2;

	return true;
}
/*
****************************************************
*  Function       : d_puts_si4463
*  Description    : send a block of data,if receving ACK return true,else return false.
*  Calls          : 
*  Called By      : 
*  Input          : 
						RecvAddr:the addr where the data send to,
						start:the data's start addr
						length:data's length
*  Output         : 
*  Return         : 
					
*  Others         : 
*  Author         : 
*****************************************************
*/
static result d_puts_si4463(uint32_t RecvAddr,uint8_t * start,uint32_t length)
{
	uint8_t RepeatCounter = 0;
    p_RfPrivateDataTypedef RFPD = (p_RfPrivateDataTypedef)si4463.Buf;
	//retry the send for 4 timers
	ReSend:
    if(CHECK_STATE(si4463.state,STATE_CLOSE) && si4463.d_open() != true)return false;
    {
		uint8_t temp[10] = {0x00},cnt = 0,cnt1 = 0;
		//p_dataframe temp_dataframe = (p_dataframe)start;
		/***********************Notification***************************/
		/*
		I found that if I want to Tx at a different frequence,I should first call func <SI446X_START_RX>
		with the expect frequence,then call <SI446X_SEND_PACKET> to send at that expect frequence,That is why I
		write following code
		*/
		/******************************************************/
        if(SI446X_START_RX(RecvAddr, 0, PACKET_LENGTH,8, 8, 8) != true)return false;
        SI4463_Delay(4);// 2016--10--22--11--15--27:Change the delay length here from 20 to 4
		//inter critical mode,
        taskENTER_CRITICAL();
		if(SI446X_SEND_PACKET(start,length,RecvAddr,0) != true)return false;//��������
        //wait for send finish
        do{    
            if(SI446X_INT_STATUS(temp) != true || cnt++ > 200)return false;
			SI4463_Delay(1);
        }while (!(temp[3] & (1<<5))); 
		cnt = 0;
        //Inter RX mode to receive the ACK
        if(SI446X_START_RX(board.board_Local_channel, 0,ACK_LENGTH  ,8, 8, 8) != true)return false;
        taskEXIT_CRITICAL();
        //osDelay(20);
        //wait for 0.3 second to check the ACK
        for(cnt = 0;cnt < 30;cnt ++)
        {
            osDelay(10);
            if(SI446X_INT_STATUS(temp) != true)return false;
            if (temp[3] & (1<<4))
            {
                length = SI446X_READ_PACKET(start);
                
                for(cnt1 = 0;cnt1 < ACK_LENGTH;cnt1 ++)
                {
                    if(start[cnt1] != RFPD->AckBuffer[cnt1])return false;
                 }
                //indicate we get the right ACK
                if(cnt1 == ACK_LENGTH)
                {
                    if(SI446X_START_RX(board.board_Local_channel, 0, PACKET_LENGTH,8, 8, 8) != true)return false;
                    return true;
                }
                else 
                {
                    return false;
                }
            }
        }
		//runing to here indicate that we didn't receive the ACK,we will resend
		RepeatCounter ++;
		//variable length to delay
        osDelay(RepeatCounter * 5);
		if(RepeatCounter >= 4)
        {
            return false;
        }
		goto ReSend;
	}
}

/*
****************************************************
*  Function       : d_gets_si4463
*  Description    : receive data through certain addr,if received success,send ACK immediately
*  Calls          : 
*  Called By      : 
*  Input          : 
					SendAddr:Addr to receive
					start:the addr to storage the data
					length��data length received
*  Output         : 
*  Return         : 
*  Others         : 
*  Author         : 
*****************************************************
*/
static result d_gets_si4463(uint32_t SendAddr,uint8_t * start,uint32_t length)
{
    static uint8_t LastSendAddr = 0xff,cnt2 = 0;
    static uint32_t cnt = 0;
    
    length = length;
    
    cnt ++;
    if(cnt > 1000000)cnt = 0;
    //In reality I found the SI4463 will become uncontrol so I want init it at regular time.
    if(CHECK_STATE(si4463.state,STATE_CLOSE) && si4463.d_open() != true)return false;
    if(LastSendAddr != SendAddr || cnt % 100 == 0)
    {
        LastSendAddr = SendAddr;
        if(SI446X_START_RX(SendAddr, 0, PACKET_LENGTH,8, 8, 8) != true)return false;
    }

    {
		uint8_t temp[10] = {0x00};
		/* check if we receive data */
        taskENTER_CRITICAL();
		if(SI446X_INT_STATUS(temp) != true)return false;
		if (temp[3] & (1<<4))
		{
            p_dataframe temp_dataframe = (p_dataframe)start;
            //read the data
			length = SI446X_READ_PACKET(start);
            taskEXIT_CRITICAL();
            //decryption the data we have received and compare it
            if(CRC_JIEMI((uint8_t *)start) == false)return false;
			//if the data was right then we'd send ACK to sender's addr
			//the following code due to the reason as above
            if(SI446X_START_RX(temp_dataframe->sender_channel, 0, PACKET_LENGTH,8, 8, 8) != true)return false;
            // send the ACK
            if(SI446X_SEND_PACKET(RFPD->AckBuffer, ACK_LENGTH,temp_dataframe->sender_channel,0) != true)return false;
            do
            {  
                //osDelay(5);
                if(SI446X_INT_STATUS(temp) != true || cnt2++ > 200)return false;
				SI4463_Delay(1);
            }while (!(temp[3] & (1<<5)));    //wait for send success
            //inter RX mode
            if(SI446X_START_RX(board.board_Local_channel, 0, PACKET_LENGTH,8, 8, 8) != true)return false;
            data_recv_process();
            return true;
		}
        taskEXIT_CRITICAL();
	}

	return false;
}
/*
****************************************************
*  Function       : d_timing_proceee_si4463
*  Description    : the device has some function that implemented by timing call
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
static result d_timing_proceee_si4463(uint32_t Interval, uint32_t Param2, uint32_t Param3)
{
	static uint32_t During = 0x00;
	
    if(CHECK_STATE(si4463.state,STATE_CLOSE) && si4463.d_open() != true)return false;
	
	During += Interval;
	//every 3 minutss the SI4463 was closed..and then every call to its func will init it 
	if(During >= 3 * 60 * 1000)
	{
		During = 0;
		si4463.d_close();
	}
	
	Param2 = Param2;
	Param3 = Param3;

	return true;
}
/*
****************************************************
*  Function       : d_process_it_si4463
*  Description    : The device have some functions need to be implemented by interrupt
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : Nothing to do here
*****************************************************
*/
static result d_process_it_si4463(uint32_t Param1, uint32_t Param2, uint32_t Param3)
{
    if(CHECK_STATE(si4463.state,STATE_CLOSE) && si4463.d_open() != true)return false;
	
	Param1 = Param1;
	Param2 = Param2;
	Param3 = Param3;

	return true;
}

deviceModule si4463 = 
{
	.name               = "si4463",
	.state              = STATE_CLOSE,
    .next               = 0x00,
	.DA                 = Private,
	.d_open             = d_open_si4463,
	.d_close            = d_close_si4463,
	.d_detect           = d_detect_si4463,
    .d_command          = d_command_si4463,
	.d_set              = d_set_si4463,
	.d_puts             = d_puts_si4463,
	.d_gets             = d_gets_si4463,
	.d_timing_proceee   = d_timing_proceee_si4463,
	.d_process_it       = d_process_it_si4463
};


























