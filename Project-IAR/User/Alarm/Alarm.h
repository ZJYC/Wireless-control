#ifndef __ALARM_H__
#define __ALARM_H__

#define LED1    0
#define LED2    1
#define LED3    2
#define LED4    3
#define LED5	4

typedef struct Caution_
{
    //"1010001100"
    uint16_t Counter_1[5];      //报警次数
    uint8_t * Directive[5];   //报警信号
    uint16_t CommonCnt;         //公用计数器
    uint16_t PrivateCnt[5];     //独立计数器
}Caution,*p_Caution;


extern deviceModule Alarm;
extern Caution caution;











#endif



















