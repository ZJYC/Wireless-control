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

//UART_HandleTypeDef huart1;

uint8_t BuffHMI[30] = {0x00};


uint8_t CurPageID = 0xFF,CurItem = 0;
PageInfTypedef PageAll[] = 
{
    {
    "大北屋",
    2,
    {
    {0,"c0",14,1,"Temperature_0",0},
    {0,"c1",15,0,"Humidity_0",0},
    {0,"c2",16,1,0,0},
    {0,"n0",18,12,"Relay_0-0-",0},
    {0,"n1",19,13,"Relay_0-1-",0},
    {0,"n2",20,14,"Relay_0-2-",0},
    }
    },
    {
    "南屋",
    3,
    {
    {0,"c0",15,1,"Temperature_1",1},
    {0,"c1",16,0,"Humidity_1",1},
    {0,"c2",17,0,0,1},
    {0,"n0",18,23,"Relay_1-0-",1},
    {0,"n1",19,34,"Relay_1-1-",1},
    {0,"n2",20,56,"Relay_1-2-",1},
    }
    },
    {
    "大门",
    4,
    {
    {0,"c0",15,0,"Temperature_2",2},
    {0,"c1",16,1,"Humidity_2",2},
    {0,"c2",17,1,0,2},
    {0,"n0",18,1,"Relay_2-0-",2},
    {0,"n1",19,2,"Relay_2-1-",2},
    {0,"n2",20,3,"Relay_2-2-",2},
    }
    },
    {
    "庭院",
    5,
    {
    {0,"c0",15,1,"Temperature_3",3},
    {0,"c1",16,1,"Humidity_3",3},
    {0,"c2",17,0,0,3},
    {0,"n0",18,7,"Relay_3-0-",3},
    {0,"n1",19,8,"Relay_3-1-",3},
    {0,"n2",20,9,"Relay_3-2-",3},
    }
    }
};

/*变量声明*/





/*函数声明*/

result HMI_ExecInstruction(uint8_t * Data,uint8_t Length);
static result HMI_SetTime(p_DS1307TimeTypedef Time);
static result HMI_SendInstruction(uint8_t * Command,uint8_t Type);
static result HMI_SyncTH(void);

static result HMI_SendInstruction(uint8_t * Command,uint8_t Type)
{
    uint8_t Buf[30] = {0x00},Len = 0,cnt = 0;
    p_Uart2PrivateTypedef U2PD = (p_Uart2PrivateTypedef)usart_1.data;
    
    strcpy(Buf,Command);
    
    Len = strlen(Buf);
    
    Buf[Len + 0] = 0xff;
    Buf[Len + 1] = 0xff;
    Buf[Len + 2] = 0xff;
    //开始接收数据
    U2PD->Recvd = 0x00;
    
    usart_1.d_puts(0,Buf,Len + 3);
    
    while(Type == 0xff)
    {
        uint8_t i = 0;
        
        osDelay(100);
        
        if(U2PD->Recvd == 0xff)//如果收到数据，判断是否为成功
        {
            //要掠过前导0 
            while(U2PD->Uart2Buf[i] == 0)i++;
            if(i >= 100)return false;
            if(HMI_ExecInstruction(&U2PD->Uart2Buf[i],100 - i) == HMI_ReturnTrue)
            {
                U2PD->Recvd = 0x00;
                return true;
            }
            
        }
        //0.1S内没有收到，重试10次，最后返回false
        if(cnt ++ > 10)return false;
        usart_1.d_puts(0,Buf,Len + 3);
    }
    
    return true;
}

/*

向HMI设置数值，HMI会不会返回触摸热区或者是其他事件？NO

*/

static result SaveHMIValue(uint8_t PageID,uint8_t ItemID,uint8_t NewValue)
{
    uint8_t i = 0,j = 0,Buf[30] = {0x00};
    //保存数据
    for(i = 0;i < sizeof(PageAll)/sizeof(PageAll[0]);i ++)
    {
        if(PageAll[i].ID == PageID)
        {
            for(j = 0;j < 6; j ++)
            {
                if(PageAll[i].Item[j].ID == ItemID)
                {
                    PageAll[i].Item[j].Changed = 0xff;
                    if(PageAll[i].Item[j].Value == 0x00)PageAll[i].Item[j].Value = 1;
                    else PageAll[i].Item[j].Value = 0;
                }
            }
            
        }
    }
    //执行数据
    for(i = 0;i < sizeof(PageAll)/sizeof(PageAll[0]);i ++)
    {
        for(j = 0;j < 6; j ++)
        {
            if(PageAll[i].Item[j].Changed == 0xff)
            {
                PageAll[i].Item[j].Changed = 0x00;
                strcpy(Buf,PageAll[i].Item[j].DeviceName);
                if(PageAll[i].Item[j].Value == 0x00)strcat(Buf,"CLOSE-");
                else strcat(Buf,"OPEN-");
                send_operation(Buf,PageAll[i].Item[j].Board);
                Buf[0] = 0x00;
            }
        }
    }
    return true;
    
}

static result SyncPage(uint8_t PageID)
{
    uint8_t i = 0,j = 0,k = 0,Buf[30] = {0x00},Buf_1[10] = {0x00};
    
    for(i = 0;i < sizeof(PageAll)/sizeof(PageAll[0]);i ++)
    {
        if(PageAll[i].ID == PageID)
        {
            for(j = 0;j < 6;j ++)
            {
                if(PageAll[i].Item[j].Name[0] == 'n')
                {
                    strcpy(Buf,PageAll[i].Item[j].Name);
                    strcat(Buf,".val=");
                    sprintf(Buf_1,"%d",PageAll[i].Item[j].Value);
                    strcat(Buf,Buf_1);
                    HMI_SendInstruction(Buf,0xff);
                }
                if(PageAll[i].Item[j].Name[0] == 'c')
                {
                    strcpy(Buf,PageAll[i].Item[j].Name);
                    strcat(Buf,".val=");
                    //sprintf(Buf_1,"%d",PageAll[i].Item[j].Value);
                    strcat(Buf,PageAll[i].Item[j].Param);
                    HMI_SendInstruction(Buf,0xff);
                }
            }
        }
    }
}

result HMI_ExecInstruction(uint8_t * Data,uint8_t Length)
{
    
    if(Data == 0)return false;
    
    switch(*Data)
    {
        case HMI_RETURN_00:{return HMI_ReturnErr;}
        case HMI_RETURN_01:{return HMI_ReturnTrue;}
        case HMI_RETURN_02:{return HMI_ReturnErr;}
        case HMI_RETURN_03:{return HMI_ReturnErr;}
        case HMI_RETURN_04:{return HMI_ReturnErr;}
        case HMI_RETURN_05:{return HMI_ReturnErr;}
        case HMI_RETURN_11:{return HMI_ReturnErr;}
        case HMI_RETURN_12:{return HMI_ReturnErr;}
        case HMI_RETURN_1A:{return HMI_ReturnErr;}
        case HMI_RETURN_1B:{return HMI_ReturnErr;}
        case HMI_RETURN_1C:{return HMI_ReturnErr;}
        case HMI_RETURN_1D:{return HMI_ReturnErr;}
        case HMI_RETURN_1E:{return HMI_ReturnErr;}
        case HMI_RETURN_1F:{return HMI_ReturnErr;}
        case HMI_RETURN_20:{return HMI_ReturnErr;}
        case HMI_RETURN_23:{return HMI_ReturnErr;}
        case HMI_RETURN_HotArea         :{
                                            HMI_ProcotolHotArea_Typedef *HotArea = (HMI_ProcotolHotArea_Typedef*)Data;
                                            SaveHMIValue(HotArea->CurPageID,HotArea->ButtonID,HotArea->TouchEvent);
                                            SyncPage(HotArea->CurPageID);
                                            break;
                                        }
        case HMI_RETURN_CurPageID       :{
                                            HMI_ReturnCurPageID_Typedef * PageID = (HMI_ReturnCurPageID_Typedef *)Data;
                                            SyncPage(PageID->CurPageID);
                                            CurPageID = PageID->CurPageID;
                                            break;
                                        }
        case HMI_RETURN_TouchPos            :{break;}
        case HMI_RETURN_SleepTouch      :{HMI_SendInstruction("sendme",0x00);break;}
        case HMI_RETURN_ReturnStr       :{break;}
        case HMI_RETURN_ReturnVal       :{break;}
        case HMI_RETURN_AutoInterSleep  :{break;}
        case HMI_RETURN_AutoAware       :{break;}
        case HMI_RETURN_StartUpSuccess  :{break;}
        case HMI_RETURN_SdUpdate            :{break;}
        case HMI_RETURN_DataTransparentCpt:{break;}
        case HMI_RETURN_DataTransparentRdy:{break;}
        default:break;
    }
	return true;
}


result HMI_Task(uint32_t Type,uint32_t cnt)
{
    uint8_t i = 0;
    p_Uart2PrivateTypedef U2PD = (p_Uart2PrivateTypedef)usart_1.data;
    if(CHECK_STATE(usart_1.state,STATE_CLOSE))usart_1.d_open();
    if(U2PD->Recvd == 0xff)//表示收到数据
    {
        //要掠过前导0 
        while(U2PD->Uart2Buf[i] == 0)i++;
        if(i >= 100)return false;
        HMI_ExecInstruction(&U2PD->Uart2Buf[i],100 - i);
        for(i = 0;i < 100;i ++)U2PD->Uart2Buf[i] = 0;
        Alarm.d_puts(LED2,"1100",1);
        //恢复数据的接收
        U2PD->Recvd = 0x00;
    }
    if(cnt % 30 == 0)
    {
        //HMI_SendInstruction("sendme",0x00);
    }
    if(cnt % 10 == 0)
    {
        p_DS1307TimeTypedef DS1307Time = (p_DS1307TimeTypedef)DS1307.data;
        HMI_SetTime(DS1307Time);
        HMI_SyncTH();
    }
}

static result HMI_SetTime(p_DS1307TimeTypedef Time)
{
    //t1.txt="2016-08-20"
    //t0.txt="23:35"
    uint8_t Buf[30] = {0x00};
    
    if(CurPageID == 0)
    {
    
        sprintf(Buf,"t1.txt=\"20%02d-%02d-%02d\"",Time->year,Time->mon,Time->day);
        
        HMI_SendInstruction(Buf,0xff);
        
        sprintf(Buf,"t2.txt=\"%02d:%02d\"",Time->hour,Time->min);
        
        HMI_SendInstruction(Buf,0xff);
    
    }
	else
	{
		HMI_SendInstruction("sendme",0x00);
	}
		
}

static result HMI_SyncTH(void)
{
    uint8_t i,j;
    
    for(i = 0;i < sizeof(PageAll)/sizeof(PageAll[0]);i ++)
    {
        for(j = 0;j < 6; j ++)
        {
            if(PageAll[i].Item[j].Name[0] == 'c')
            {
                strcpy(PageAll[i].Item[j].Param,(const char *)InfSetSearch(PageAll[i].Item[j].DeviceName));
            }
        }
    }
    
    return true;
}






















