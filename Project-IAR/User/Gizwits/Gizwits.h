
#ifndef __GIZWITS_H__
#define __GIZWITS_H__



#define DirectiveMaxLen	128

result GizwitsInit(void);
result GizwitsExecDirective(void);
result GizwitsSync(void);
result GizwitsRecv(uint8_t Port,uint8_t * Buf);
result GizwitsSend(uint8_t Port,uint8_t * Buf);
void GizwitsTimingProcess(void);






















#endif
























