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













/* 单位us */
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

    /* 按顺序插入 */
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

    /* 查询链表第一个元素是否到期 */
    while(enumSoftTimerGetState((SoftTimerTypeDef *)(ptypeListNow->pElement)) == softTimerOver)
    {
        ptypeTimerNow = ptypeListNow->pElement;

        /* 回调函数 */
        if(ptypeTimerNow->vCallbackFunction != NULL)
        {
            ptypeTimerNow->vCallbackFunction(ptypeTimerNow);
        }

        /* 移出链表 */
        cError == cListRemove(ptypeListNow);

        /* 重复计时 */
        if(ptypeTimerNow->state & softTimerCircular)
        {
            cError |= cSoftTimerReload(ptypeTimerNow);

            cError |= cListInit(&(ptypeTimerNow->typeList), ptypeTimerNow->timeStop);

            /* 按顺序插入 */
            cError |= cListInsert(&g_typeListSoftTimerHead, &(ptypeTimerNow->typeList));
        }
        /* 单次计时 */
        else
            cError |= cSoftTimerClose(ptypeTimerNow);

        /* 把指针再次指令链表第一个元素（第一个元素总是最早到期） */
        ptypeListNow = g_typeListSoftTimerHead.pNext;
    }

    return cError;
}
