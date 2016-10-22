
/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
/* USER CODE BEGIN Includes */     
#include "ff.h"
#include "board.h"
#include "sdio.h"
/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId BackGroundTask_1_Handle;
osThreadId BackGroundTask_2_Handle;
osThreadId HighSpeedDeviceTask_1_Handle;
osThreadId HighSpeedDeviceTask_2_Handle;
osThreadId LowSpeedDeviceTask_2_Handle;
osThreadId LowSpeedDeviceTask_1_Handle;
osTimerId HighSpeedTimer;
osTimerId LowSpeedTimer;
osMutexId SI4463Mutex;
osMutexId AlarmMutex;
osMutexId Usart2TxMutex;
osMutexId Usart2RxMutex;
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void BackGroundTask_1(void const * argument);
void BackGroundTask_2(void const * argument);
void HighSpeedDeviceTask_1(void const * argument);
void HighSpeedDeviceTask_2(void const * argument);
void LowSpeedDeviceTask_2(void const * argument);
void HighSpeedTimerTask(void const * argument);
void LowSpeedTimerTask(void const * argument);
void LowSpeedDeviceTask_1(void const * argument);

extern void MX_FATFS_Init(void);
extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* Pre/Post sleep processing prototypes */
void PreSleepProcessing(uint32_t *ulExpectedIdleTime);
void PostSleepProcessing(uint32_t *ulExpectedIdleTime);

/* Hook prototypes */
void vApplicationIdleHook(void);
void vApplicationTickHook(void);
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);
void vApplicationMallocFailedHook(void);

/* USER CODE BEGIN 2 */
void vApplicationIdleHook( void )
{

}
/* USER CODE END 2 */

/* USER CODE BEGIN 3 */
void vApplicationTickHook( void )
{

}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
{

}
/* USER CODE END 4 */

/* USER CODE BEGIN 5 */
void vApplicationMallocFailedHook(void)
{

}
/* USER CODE END 5 */

/* USER CODE BEGIN PREPOSTSLEEP */
void PreSleepProcessing(uint32_t *ulExpectedIdleTime)
{
/* place for user code */ 
}

void PostSleepProcessing(uint32_t *ulExpectedIdleTime)
{
/* place for user code */
}
/* USER CODE END PREPOSTSLEEP */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) 
{

	SI4463Mutex = osMutexCreate(0x00);
	AlarmMutex = osMutexCreate(0x00);
    Usart2RxMutex = osMutexCreate(0x00);
    Usart2TxMutex = osMutexCreate(0x00);

	osTimerDef(HighSpeedTimer, HighSpeedTimerTask);
	HighSpeedTimer = osTimerCreate(osTimer(HighSpeedTimer), osTimerPeriodic, NULL);
	osTimerDef(LowSpeedTimer, LowSpeedTimerTask);
	LowSpeedTimer = osTimerCreate(osTimer(LowSpeedTimer), osTimerPeriodic, NULL);

	osTimerStart(HighSpeedTimer,20);
	osTimerStart(LowSpeedTimer,100);

	osThreadDef(BackGroundTask_1, BackGroundTask_1, osPriorityNormal, 0, 1024);
	BackGroundTask_1_Handle = osThreadCreate(osThread(BackGroundTask_1), NULL);

	osThreadDef(BackGroundTask_2, BackGroundTask_2, osPriorityNormal, 0, 1024);
	BackGroundTask_2_Handle = osThreadCreate(osThread(BackGroundTask_2), NULL);
	
	osThreadDef(HighSpeedDeviceTask_1,HighSpeedDeviceTask_1, osPriorityRealtime, 0, 1024);
	HighSpeedDeviceTask_1_Handle = osThreadCreate(osThread(HighSpeedDeviceTask_1), NULL);
	
	osThreadDef(HighSpeedDeviceTask_2,HighSpeedDeviceTask_2, osPriorityNormal, 0, 1024);
	HighSpeedDeviceTask_2_Handle = osThreadCreate(osThread(HighSpeedDeviceTask_2), NULL);
	
	osThreadDef(LowSpeedDeviceTask_2,LowSpeedDeviceTask_2, osPriorityNormal, 0, 1024);
	LowSpeedDeviceTask_2_Handle = osThreadCreate(osThread(LowSpeedDeviceTask_2), NULL);
	
	osThreadDef(LowSpeedDeviceTask_1,LowSpeedDeviceTask_1, osPriorityNormal, 0, 1024);
	LowSpeedDeviceTask_1_Handle = osThreadCreate(osThread(LowSpeedDeviceTask_1), NULL);
}
/*
****************************************************
*  Function       : BackGroundTask_1
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*  Author         : 
*****************************************************
*/
void BackGroundTask_1(void const * argument)
{
	for(;;)
	{
        while(1)osDelay(100);

	}
}
/*
****************************************************
*  Function       : BackGroundTask_2
*  Description    : 
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*  Author         : 
*****************************************************
*/
void BackGroundTask_2(void const * argument)
{
	for(;;)
	{
		osDelay(100);
	}
}

//H-speed timer task(20ms)
void HighSpeedTimerTask(void const * argument)
{
	static uint16_t cnt = 0x00;
	//every 2 seconds we flick the LED1
	cnt ++;if(cnt > 100)cnt = 0;
    if(cnt == 0)Alarm.d_puts(LED1,"100000000",1);
	//devices's timing process is here
	d_ADC_Key.d_timing_proceee(0,0,0);
	Alarm.d_timing_proceee(0,0,0);
	si4463.d_timing_proceee(20,0,0);
    GizwitsTimingProcess();
	//the TTL(time to live)
	/*
	The SI4463 commucation module is One master more slave,So the master need to know wheter the slaver is exist,
	So once the slaver power on,the first thing to do is send what I have&&Addr to the master,once master receive 
	such information,It record it and keep it for about 120 seconds,once expired,the master wiil delete it,
	the slaver need to send such information every 100 seconds to keep that.
	*/
	if(cnt % 50 == 0)board_device_list_opera();
}
//L-speed timer task(100ms)
void LowSpeedTimerTask(void const * argument)
{
	;
}


//H-Device-1
void HighSpeedDeviceTask_1(void const * argument)
{
    uint32_t cnt = 0x00;
    
    osDelay(200);
	//初始化板子数据结构
    board_init();
	//初始化完成闪烁LED2
    Alarm.d_puts(LED1,"10001000",1);
    Alarm.d_puts(LED2,"10001000",1);
    Alarm.d_puts(LED3,"10001000",1);
    Alarm.d_puts(LED4,"10001000",1);

    while(1)
    {
        osDelay(20);
        cnt ++;if(cnt >= 100000)cnt = 0;
		if(cnt % 2 == 0)
		{
			//抢占SI4463
			osMutexWait(SI4463Mutex,osWaitForever);
			//平均100秒就要发送同步信号
			if(cnt % 5000 == 0 && cnt != 0)board_sync_device();
			if(board.ops->CheckReceive() == true)Alarm.d_puts(LED2,"10001000",1);
			//释放SI4463
			osMutexRelease(SI4463Mutex);
		}
    }
}

//H-Device-2
void HighSpeedDeviceTask_2(void const * argument)
{
	static KeyValueEnum KeyValue = 0;
	uint32_t cnt  = 0; 
    while(1)
	{
        osDelay(20);
		cnt ++;if(cnt >= 100000)cnt = 0;
		//按键任务
		if(cnt % 1 == 0)
		{
			d_ADC_Key.d_gets(0,(uint8_t *)&KeyValue,0);
			if(KeyValue != N)
			{
				osMutexWait(SI4463Mutex,osWaitForever);
				switch(KeyValue)
				{
					case A:{board.ops->send_operation("Relay_1-1-OPEN-",CHANNEL_1);break;}
					case B:{board.ops->send_operation("Relay_2-1-OPEN-",CHANNEL_2);break;}
					case C:{board.ops->send_operation("Relay_3-1-OPEN-",CHANNEL_3);break;}
					case D:{board.ops->send_operation("Relay_0-1-OPEN-",CHANNEL_0);break;}
					default:break;
				}
				osMutexRelease(SI4463Mutex);
			}
		}
	}
}
//L-Device-2
void LowSpeedDeviceTask_1(void const * argument)
{

	uint32_t cnt = 0;

    if(CUR_BOARD == MAIN_BOARD)GizwitsInit();
    
    while(1)
    {
		cnt++;if(cnt >= 100000)cnt = 0;
		osDelay(100);
        if(CUR_BOARD == MAIN_BOARD)GizwitsExecDirective();
    }
}
//L-Device-1
void LowSpeedDeviceTask_2(void const * argument)
{
    uint8_t SHT2xSerialNumber[8] = {0x00},MinuteLast = 0x00;
	uint32_t cnt  = 0;
    float TemperatureLast = 0,HumidityLast = 0;
	//初始化SHT20
    SHT20.d_open();
	//获取SHT20序列号
    SHT20.d_set(0,(uint32_t)SHT2xSerialNumber);
	//初始化时钟
    DS1307.d_open();
    
    relay.d_open();
	
	while(1)
	{
		osDelay(100);
		cnt ++;if(cnt >= 100000)cnt = 0;
		//温湿度传感器任务
		if(cnt % 20 == 0)
		{
			SHT20.d_set(1,(uint32_t)&Temperature);
			SHT20.d_set(2,(uint32_t)&Humidity);
			if(fabs(TemperatureLast - Temperature) > 0.5 || fabs(HumidityLast - Humidity) > 2)
			{
				TemperatureLast = Temperature;
				HumidityLast = Humidity;
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
		//DS1307任务
		if(cnt % 5 == 0)
		{
			DS1307.d_gets(0,0,0);
			if(Ds1307Time.min != MinuteLast)
			{
				MinuteLast = Ds1307Time.min;
				osMutexWait(SI4463Mutex,osWaitForever);
				//同步时间成功闪烁LED2
				if(send_sync_time(0xFF) == true)
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
        
		if(cnt % 600 && CUR_BOARD == MAIN_BOARD && 0)
		{
			static uint8_t CntBoardNum = 0,CntBitNum = 0;
			uint8_t Buf[30] = {0};
			if(++ CntBoardNum > 3)CntBoardNum = 0;
			for(CntBitNum = 0;CntBitNum < 6;CntBitNum ++)
			{
				sprintf((char*)Buf,"GET-Relay_%d-%d-",CntBoardNum,CntBitNum);
				send_sync(Buf,GetBoardAddr(CntBoardNum));
				osDelay(100);
			}
		}
		
        if(cnt % 60 == 0 && CUR_BOARD == MAIN_BOARD)
        {
            GizwitsSync();
        }
		
	}
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
