
//对应于一个子项（文本框、、、）
typedef struct ItemStruct_
{
	uint8_t ItemID;
	uint8_t ItemName[2];
	uint8_t ItemValue[8];
	uint8_t (*ItemHandle)(uint8_t);
}ItemStruct,*p_ItemStruct;
//对应于一个页面
typedef struct PageStruct_
{
	uint8_t	   PageID;
	uint8_t	   PageName[8];
	ItemStruct Temp;
	ItemStruct Humi;
	ItemStruct Ligh;
	ItemStruct App_1;
	ItemStruct App_2;
	ItemStruct App_3;
	uint8_t (*PageHandle)(uint8_t);
}PageStruct,* p_PageStruct;

typedef struct ProjectStruct_
{
	PageStruct  Page1;
	PageStruct  Page2;
}ProjectStruct,*p_ProjectStruct;

#define	RED		63488
#define BLUE	31
#define GRAY	33840 
#define BLACK	0 
#define WHITE	65535 
#define GREEN	2016
#define BROWN	48192 
#define YELLOW	65504 

#define HUMI_ERROR_UD			0x00
#define HUMI_ERROR_SUCCESS		0x01
#define HUMI_ERROR_UIID			0x02
#define HUMI_ERROR_UPID			0x03
#define HUMI_ERROR_UTID			0x04
#define HUMI_ERROR_UZID			0x05
#define HUMI_ERROR_UBID			0x11
#define HUMI_ERROR_IDorCH_UD	0x12
#define HUMI_ERROR_VUD			0x1A
#define HUMI_ERROR_BLYSWX		0x1B

#define HUMI_RE_
















