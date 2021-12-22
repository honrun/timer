#ifndef DEVICESSOFTTIMER_H_INCLUDED
#define DEVICESSOFTTIMER_H_INCLUDED


#include "stdint.h"





typedef enum {
    softTimerClose = 0x00,    /* ��ʱ���ѹر� */
    softTimerOpen = 0x01,     /* ������ʱ���� */
    softTimerOver = 0x02,     /* ��ʱ������� */
    softTimerError = 0x08,    /* ��ʱ������ */
    softTimerCircular = 0x10, /* ѭ����ʱ */
}SoftTimerStateEnum;



typedef struct
{
    uint64_t timeStop;                  /* ��ʱ����ʱ�̣�us�� */
    uint64_t timeDuration;              /* ��ʱʱ����us�� */
    SoftTimerStateEnum state;            /* ״̬ */

    ListType typeList;                   /* ���� */
    void (*vCallbackFunction)(void *);    /* �ص����� */
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
