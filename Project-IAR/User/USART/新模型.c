
typedef enum DriverAttriTypedef_
{
    Private = 0,
    Public = 1
}DriverAttriTypedef;

typedef enum DriverCommandTypedef_
{
    
    Command_SetPriData = 0,
    Command_SetSendHook = 1,
    Command_SetSendsHook = 2,
    Command_SetRecvHook = 3,
    Command_SetRecvsHook = 4
    
    
}DriverCommandTypedef;

typedef struct DriverModuleTypedef_
{
    uint8_t  * PrivateData;
    uint32_t   PrivateDataLen;
    uint8_t    Name[16];
    uint32_t   SN;
    
    DriverAttriTypedef DriverAttri;
}



