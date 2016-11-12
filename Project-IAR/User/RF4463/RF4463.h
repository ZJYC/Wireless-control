
#ifndef __RF4463_H__
#define __RF4463_H__

#define     ACK_LENGTH      10 

typedef struct RfPrivateDataTypedef_
{
    
    uint8_t AckBuffer[ACK_LENGTH];
    uint32_t Counter;
    
}RfPrivateDataTypedef,*p_RfPrivateDataTypedef;

extern deviceModule si4463;













#endif







































