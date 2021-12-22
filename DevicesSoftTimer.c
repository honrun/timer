/*
 *  modifier: Honrun
 */
#include "stdio.h"
#include "time.h"
#include "stdint.h"
#include "string.h"
#include "DevicesList.h"
#include "DevicesSoftTimer.h"







static uint64_t ulSoftTimerGetNow(void);




int8_t cSoftTimerSet(SoftTimerTypeDef *ptypeTimer, uint64_t ulTime, SoftTimerStateEnum state)
{
    if(ptypeTimer == NULL)
        return -1;

    ptypeTimer->timeStop = ulSoftTimerGetNow() + ulTime;
    ptypeTimer->timeDuration = ulTime;
    ptypeTimer->state = state;

    return 0;
}

int8_t cSoftTimerReset(SoftTimerTypeDef *ptypeTimer)
{
    if(ptypeTimer == NULL)
        return -1;

    cSoftTimerSet(ptypeTimer, ptypeTimer->timeDuration, ptypeTimer->state | softTimerOpen);

    return 0;
}

int8_t cSoftTimerReload(SoftTimerTypeDef *ptypeTimer)
{
    if(ptypeTimer == NULL)
        return -1;

    ptypeTimer->timeStop += ptypeTimer->timeDuration;
    ptypeTimer->state |= softTimerOpen;

    return 0;
}

int8_t cSoftTimerOpen(SoftTimerTypeDef *ptypeTimer)
{
    if(ptypeTimer == NULL)
        return -1;

    ptypeTimer->state |= softTimerOpen;

    return 0;
}

int8_t cSoftTimerClose(SoftTimerTypeDef *ptypeTimer)
{
    if(ptypeTimer == NULL)
        return -1;

    ptypeTimer->state &= ~softTimerOpen;

    return 0;
}

SoftTimerStateEnum enumSoftTimerGetState(SoftTimerTypeDef *ptypeTimer)
{
    int64_t ulTimeNow = 0;

    if(ptypeTimer == NULL)
        return softTimerError;

    if((ptypeTimer->state & softTimerOpen) == 0)
        return softTimerClose;

    ulTimeNow = ulSoftTimerGetNow();

    if(ulTimeNow >= (ptypeTimer->timeStop))
        return softTimerOver;

    return softTimerOpen;
}













/* ��λus */
static uint64_t ulSoftTimerGetNow(void)
{
    static uint64_t ulSoftTimerTick = 0, st_ulSoftTimerTick = 0;

    if(st_ulSoftTimerTick == 0)
        time(&st_ulSoftTimerTick);

    time(&ulSoftTimerTick);

    return (ulSoftTimerTick - st_ulSoftTimerTick) * 1000000;
}










void vSoftTimerCallbackFunction(void *pvHandle)
{
    int64_t ulTimeNow = 0;

    (void)pvHandle;

    ulTimeNow = ulSoftTimerGetNow();

    printf("vSoftTimerCallbackFunction: %08X ulTimeNow: %d\r\n", pvHandle, ulTimeNow);
}



int8_t cSoftTimerMachineInit(SoftTimerTypeDef *ptypeTimer, uint64_t ulTime, SoftTimerStateEnum state, void (*vCallbackFunction)(void *pvHandle))
{
    int8_t cError = 0;

    cError |= cSoftTimerSetS(ptypeTimer, ulTime, state);

    cError |= cListInit(&(ptypeTimer->typeList), ptypeTimer->timeStop);

    /* ��˳����� */
    cError |= cListInsert(&g_typeListSoftTimerHead, &(ptypeTimer->typeList));

    ptypeTimer->typeList.pElement = ptypeTimer;

    ptypeTimer->vCallbackFunction = vCallbackFunction;

    return cError;
}

int8_t cSoftTimerMachine(void)
{
    SoftTimerTypeDef *ptypeTimerNow = NULL;
    struct ListStruct *ptypeListNow = g_typeListSoftTimerHead.pNext;
    int8_t cError = -1;

    /* ��ѯ�����һ��Ԫ���Ƿ��� */
    while(enumSoftTimerGetState((SoftTimerTypeDef *)(ptypeListNow->pElement)) == softTimerOver)
    {
        ptypeTimerNow = ptypeListNow->pElement;

        /* �ص����� */
        if(ptypeTimerNow->vCallbackFunction != NULL)
        {
            ptypeTimerNow->vCallbackFunction(ptypeTimerNow);
        }

        /* �Ƴ����� */
        cError == cListRemove(ptypeListNow);

        /* �ظ���ʱ */
        if(ptypeTimerNow->state & softTimerCircular)
        {
            cError |= cSoftTimerReload(ptypeTimerNow);

            cError |= cListInit(&(ptypeTimerNow->typeList), ptypeTimerNow->timeStop);

            /* ��˳����� */
            cError |= cListInsert(&g_typeListSoftTimerHead, &(ptypeTimerNow->typeList));
        }
        /* ���μ�ʱ */
        else
            cError |= cSoftTimerClose(ptypeTimerNow);

        /* ��ָ���ٴ�ָ�������һ��Ԫ�أ���һ��Ԫ���������絽�ڣ� */
        ptypeListNow = g_typeListSoftTimerHead.pNext;
    }

    return cError;
}
