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
    {0,"c0",14,1},
	{0,"c1",15,0},
	{0,"c2",16,1},
    {0,"n0",18,12},
	{0,"n1",19,13},
	{0,"n2",20,14},
    }
    },
    {
	"南屋",
	3,
    {
    {0,"c0",15,1},
	{0,"c1",16,0},
	{0,"c2",17,0},
    {0,"n0",18,23},
	{0,"n1",19,34},
	{0,"n2",20,56},
    }
	},
    {
	"大门",
	4,
    {
    {0,"c0",15,0},
	{0,"c1",16,1},
	{0,"c2",17,1},
    {0,"n0",18,1},
	{0,"n1",19,2},
	{0,"n2",20,3},
    }
	},
    {
	"庭院",
	5,
    {
    {0,"c0",15,1},
	{0,"c1",16,1},
	{0,"c2",17,0},
    {0,"n0",18,7},
	{0,"n1",19,8},
	{0,"n2",20,9},
    }
    }
};

/*变量声明*/





/*函数声明*/

result HMI_ExecInstruction(uint8_t * Data,uint8_t Length);
static result HMI_SetTime(p_DS1307TimeTypedef Time);
#if 0//发送指令

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

static result HMI_SendInstruction(uint8_t * Command)
{
    uint8_t Buf[30] = {0x00},Len = 0,cnt = 0;
    
    strcpy(Buf,Command);
    
    Len = strlen(Buf);
    
    Buf[Len + 0] = 0xff;
    Buf[Len + 1] = 0xff;
    Buf[Len + 2] = 0xff;
    
    usart_1.d_puts(0,Buf,Len + 3);
    
    while(1)
    {
        uint8_t i = 0;
        p_Uart2PrivateTypedef U2PD = (p_Uart2PrivateTypedef)usart_1.data;
        
        osDelay(5 + cnt * 5);
        
        if(U2PD->Recvd == 0xff)//表示收到数据
        {
            //要掠过前导0 
            while(U2PD->Uart2Buf[i] == 0)i++;
            if(i >= 100)return false;
            U2PD->Recvd = 0x00;
            if(HMI_ExecInstruction(&U2PD->Uart2Buf[i],100 - i) == HMI_ReturnTrue)break;
            
        }
        if(cnt ++ > 20)return false;
        usart_1.d_puts(0,Buf,Len + 3);
    }
    
    return true;
}

/*

向HMI设置数值，HMI会不会返回触摸热区或者是其他事件？NO

*/

static result SaveHMIValue(uint8_t PageID,uint8_t ItemID,uint8_t NewValue)
{
	uint8_t i = 0,j = 0;
	
	for(i = 0;i < sizeof(PageAll)/sizeof(PageAll[0]);i ++)
	{
		if(PageAll[i].ID == PageID)
		{
			for(j = 0;j < 6; j ++)
			{
				if(PageAll[i].Item[j].ID == ItemID)
				{
					//PageAll[i].Item[j].Changed = 0xff;
					if(PageAll[i].Item[j].Value == 0x00)PageAll[i].Item[j].Value = 1;
                    else PageAll[i].Item[j].Value = 0;
				}
			}
			
		}
	}
	
	return true;
	
}

static result SyncPage(uint8_t PageID)
{
	uint8_t i = 0,j = 0,k = 0,Buf[30] = {0x00},Buf_1[10] = {0x00};
    
    if(PageID == 0)
    {
        p_DS1307TimeTypedef DS1307Time = (p_DS1307TimeTypedef)DS1307.data;
        HMI_SetTime(DS1307Time);
    }
    
	for(i = 0;i < sizeof(PageAll)/sizeof(PageAll[0]);i ++)
	{
		if(PageAll[i].ID == PageID)
		{
			for(j = 0;j < 6;j ++)
			{
                //for(k = 0;k < 3;k ++)
                {
                    strcpy(Buf,PageAll[i].Item[j].Name);
                    strcat(Buf,".val=");
                    sprintf(Buf_1,"%d",PageAll[i].Item[j].Value);
                    strcat(Buf,Buf_1);
                    HMI_SendInstruction(Buf);
                    //osDelay(20);
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
		case HMI_RETURN_HotArea			:{
											HMI_ProcotolHotArea_Typedef *HotArea = (HMI_ProcotolHotArea_Typedef*)Data;
											SaveHMIValue(HotArea->CurPageID,HotArea->ButtonID,HotArea->TouchEvent);
											SyncPage(HotArea->CurPageID);
                                            break;
										}
		case HMI_RETURN_CurPageID		:{
											HMI_ReturnCurPageID_Typedef * PageID = (HMI_ReturnCurPageID_Typedef *)Data;
											SyncPage(PageID->CurPageID);
											CurPageID = PageID->CurPageID;
											break;
										}
		case HMI_RETURN_TouchPos			:{break;}
		case HMI_RETURN_SleepTouch		:{HMI_SendInstruction("sendme");break;}
		case HMI_RETURN_ReturnStr		:{break;}
		case HMI_RETURN_ReturnVal		:{break;}
		case HMI_RETURN_AutoInterSleep	:{break;}
		case HMI_RETURN_AutoAware		:{break;}
		case HMI_RETURN_StartUpSuccess	:{break;}
		case HMI_RETURN_SdUpdate			:{break;}
		case HMI_RETURN_DataTransparentCpt:{break;}
		case HMI_RETURN_DataTransparentRdy:{break;}
		default:break;
	}
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
        for(i = 0;i < 100;i ++)
        {
            U2PD->Uart2Buf[i] = 0;
        }
        U2PD->Recvd = 0x00;    
        Alarm.d_puts(LED2,"11110000",1);
	}
    if(cnt % 10 == 0)
    {
        HMI_SendInstruction("sendme");
    }
    if(cnt % 50 == 0)
    {
        p_DS1307TimeTypedef DS1307Time = (p_DS1307TimeTypedef)DS1307.data;
        HMI_SetTime(DS1307Time);
    }
}

static result HMI_SetTime(p_DS1307TimeTypedef Time)
{
	//t1.txt="2016-08-20"
	//t0.txt=23:35
	uint8_t Buf[30] = {0x00};
	
	if(CurPageID == 0)
	{
	
		sprintf(Buf,"t1.txt=\"20%02d-%02d-%02d\"",Time->year,Time->mon,Time->day);
		
		HMI_SendInstruction(Buf);
		
		sprintf(Buf,"t2.txt=\"%02d:%02d\"",Time->hour,Time->min);
		
		HMI_SendInstruction(Buf);
	
	}
}























