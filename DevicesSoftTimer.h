#ifndef DEVICESSOFTTIMER_H_INCLUDED
#define DEVICESSOFTTIMER_H_INCLUDED


#include "stdint.h"





typedef enum {
    softTimerClose = 0x00,    /* 定时器已关闭 */
    softTimerOpen = 0x01,     /* 正常计时当中 */
    softTimerOver = 0x02,     /* 定时器已溢出 */
    softTimerError = 0x08,    /* 定时器错误 */
    softTimerCircular = 0x10, /* 循环计时 */
}SoftTimerStateEnum;



typedef struct
{
    uint64_t timeStop;                  /* 定时结束时刻（us） */
    uint64_t timeDuration;              /* 定时时长（us） */
    SoftTimerStateEnum state;            /* 状态 */

    ListType typeList;                   /* 链表 */
    void (*vCallbackFunction)(void *);    /* 回调函数 */
}SoftTimerTypeDef;










#define cSoftTimerSetUs(ptypeTimer, ulTime, state) cSoftTimerSet((ptypeTimer), (ulTime), (state))
#define cSoftTimerSetMs(ptypeTimer, ulTime, state) cSoftTimerSet((ptypeTimer), (ulTime) * 1000.0f, (state))
#define cSoftTimerSetS(ptypeTimer, ulTime, state) cSoftTimerSet((ptypeTimer), (ulTime) * 1000000.0f, (state))
int8_t cSoftTimerSet(SoftTimerTypeDef *ptypeTimer, uint64_t ulTime, SoftTimerStateEnum state);
int8_t cSoftTimerReset(SoftTimerTypeDef *ptypeTimer);
int8_t cSoftTimerReload(SoftTimerTypeDef *ptypeTimer);
int8_t cSoftTimerOpen(SoftTimerTypeDef *ptypeTimer);
int8_t cSoftTimerClose(SoftTimerTypeDef *ptypeTimer);
SoftTimerStateEnum enumSoftTimerGetState(SoftTimerTypeDef *ptypeTimer);



void vSoftTimerCallbackFunction(void *pvHandle);


int8_t cSoftTimerMachineInit(SoftTimerTypeDef *ptypeTimer, uint64_t ulTime, SoftTimerStateEnum state, void (*vCallbackFunction)(void *pvHandle));
int8_t cSoftTimerMachine(void);


#endif // DEVICESSOFTTIMER_H_INCLUDED
