#include "board.h"

//p_Caution caution = (p_Caution)Alarm.data;
static Caution caution = 
{
    {0,0,0,0,0},
    {0,0,0,0,0},
    0,
    {0,0,0,0,0}
};

/*
****************************************************
*  Function       : ��Alarm
*  Description    : ��ʼ��4��LED��1����������C0,C1,C2,C3,E1��
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : ������true
*  Others         : 
*****************************************************
*/
static result Alarm_open(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    //��ʼ��4��LED
    __HAL_RCC_GPIOC_CLK_ENABLE();
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,GPIO_PIN_RESET);
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    //��ʼ��������
    __HAL_RCC_GPIOE_CLK_ENABLE();
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1,GPIO_PIN_RESET);
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
    
    AddPrivateBuf(&Alarm,(uint8_t *)&caution,sizeof(Caution));
    //����״̬
    SET_STATE(Alarm.state,STATE_OPEN);
    RESET_STATE(Alarm.state,STATE_CLOSE);
    
    return true;
}
/*
****************************************************
*  Function       : �ر�Alarm
*  Description    : ����ʼ����������
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : ������true
*  Others         : 
*****************************************************
*/
static result Alarm_close(void)
{
    HAL_GPIO_DeInit(GPIOC,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    HAL_GPIO_DeInit(GPIOE,GPIO_PIN_1);
    //����״̬
    RESET_STATE(Alarm.state,STATE_OPEN);
    SET_STATE(Alarm.state,STATE_CLOSE);

    return true;
}
/*
****************************************************
*  Function       : Alarm_detect
*  Description    : ���̵����Ƿ����
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : ������true
*  Others         : 
*****************************************************
*/
static result Alarm_detect(void)
{
    if(CHECK_STATE(Alarm.state,STATE_CLOSE) && Alarm.d_open() != true)return false;

    return true;
}
/*
****************************************************
*  Function       : Alarm_command
*  Description    : ִ�����Ŀǰ���������ڷ�����
*  Calls          : 
*  Called By      : 
*  Input          : command:"OPEN",param:none
*  Output         : 
*  Return         : ������true
*  Others         : 
*****************************************************
*/
static result Alarm_command(uint8_t * command, uint32_t param)
{
    if(CHECK_STATE(Alarm.state,STATE_CLOSE) && Alarm.d_open() != true)return false;
    return true;
}
/*
****************************************************
*  Function       : Alarm_set
*  Description    : ����ĳһ������
*  Calls          : 
*  Called By      : 
*  Input          : LEDx:���ű��State:����״̬
*  Output         : 
*  Return         : ������true
*  Others         : 
*****************************************************
*/
static result Alarm_set(uint32_t LEDx, uint32_t State)
{
    if(CHECK_STATE(Alarm.state,STATE_CLOSE) && Alarm.d_open() != true)return false;
    
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
*  Description    : ����������
*  Calls          : 
*  Called By      : 
*  Input          : LEDx:LED���Directive:ָ��Cnt:�ظ�����
*  Output         : 
*  Return         : ������true
*  Others         : 
*****************************************************
*/
static result Alarm_puts(uint32_t LEDx, uint8_t * Directive, uint32_t Cnt)
{
    p_Caution caution = (p_Caution)Alarm.data;
    
    if(CHECK_STATE(Alarm.state,STATE_CLOSE) && Alarm.d_open() != true)return false;
    
    osMutexWait(AlarmMutex,osWaitForever);
    caution->Counter_1[LEDx] = Cnt,
    caution->Directive[LEDx] = Directive;
    osMutexRelease(AlarmMutex);
    return true;
}
/*
****************************************************
*  Function       : Alarm_gets
*  Description    : ��ȡ������
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : 
*  Others         : 
*****************************************************
*/
static result Alarm_gets(uint32_t SendAddr, uint8_t * start, uint32_t length)
{
    if(CHECK_STATE(Alarm.state,STATE_CLOSE) && Alarm.d_open() != true)return false;

    return true;
}
/*
****************************************************
*  Function       : Alarm_timing_process
*  Description    : ��ʱ������
*  Calls          : 
*  Called By      : 
*  Input          : 
*  Output         : 
*  Return         : ������true
*  Others         : 
*****************************************************
*/
static result Alarm_timing_process(uint32_t Param1, uint32_t Param2, uint32_t Param3)
{
    uint16_t Cnt1;
    uint8_t * Temp = 0;
    p_Caution caution = (p_Caution)Alarm.data;
    
    if(CHECK_STATE(Alarm.state,STATE_CLOSE) && Alarm.d_open() != true)return false;

    Param1 = Param1;
    Param2 = Param2;
    Param3 = Param3;
    //���������ڶ�ʱ���ص����棬���Բ���ʹ֮����
    //osMutexWait(AlarmMutex,osWaitForever);
    //ѭ������
    taskENTER_CRITICAL();
    caution->CommonCnt++;if(caution->CommonCnt > 100)caution->CommonCnt = 0;
    for(Cnt1 = 0;Cnt1 < 5;Cnt1 ++)
    {
        if(caution->Counter_1[Cnt1] != 0x00)
        {
            Temp = caution->Directive[Cnt1];
            if(Temp[caution->PrivateCnt[Cnt1]] == 0)
            {
                caution->PrivateCnt[Cnt1] = 0;
                caution->Counter_1[Cnt1]--;
                Alarm.d_set(Cnt1,0);
                continue;
            }
            if(Temp[caution->PrivateCnt[Cnt1]] == '1')
            {
                Alarm.d_set(Cnt1,1);
            }
            if(Temp[caution->PrivateCnt[Cnt1]] == '0')
            {
                Alarm.d_set(Cnt1,0);
            }
            caution->PrivateCnt[Cnt1] ++;
        }
    }
    //osMutexRelease(AlarmMutex);
    taskEXIT_CRITICAL();
    return true;    
}

deviceModule Alarm = 
{
    #if(CUR_BOARD == MAIN_BOARD)
    .name               = "Alarm_0",    
    #elif(CUR_BOARD == BOARD_1)
    .name               = "Alarm_1",    
    #elif(CUR_BOARD == BOARD_2)
    .name               = "Alarm_2",
    #elif(CUR_BOARD == BOARD_3)
    .name               = "Alarm_3",    
    #endif
    .state              = STATE_CLOSE,
    .next               = 0x00,
    .DA                 = Public,
    .d_open             = Alarm_open,
    .d_close            = Alarm_close,
    .d_detect           = Alarm_detect,
    .d_command          = Alarm_command,
    .d_set              = Alarm_set,
    .d_puts             = Alarm_puts,
    .d_gets             = Alarm_gets,
    .d_timing_proceee   = Alarm_timing_process
};
















