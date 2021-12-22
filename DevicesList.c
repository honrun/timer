/*
 *  modifier: Honrun
 */
#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "stdlib.h"
#include "DevicesList.h"



ListType g_typeListSoftTimerHead = {&g_typeListSoftTimerHead, &g_typeListSoftTimerHead, LIST_VALUE_MAX};



int8_t cListInit(ListType *ptypeList, int64_t lValue)
{
    if(ptypeList == NULL)
        return -1;

    ptypeList->value = lValue;

    return 0;
}

int8_t cListInsert(ListType *ptypeListHead, ListType *ptypeListNew)
{
    struct ListStruct *ptypeListNow = NULL;
    int8_t cError = 0;

    if((ptypeListHead == NULL) || (ptypeListNew == NULL))
        return -1;

    /* 插入到最后 */
    if(ptypeListNew->value >= LIST_VALUE_MAX)
    {
        cError |= cListInsertEnd(ptypeListHead, ptypeListNew);

        return cError;
    }

    /* 找到排序插入的位置 */
    for(ptypeListNow = ptypeListHead; ptypeListNow->pNext->value <= ptypeListNew->value; ptypeListNow = ptypeListNow->pNext);

    ptypeListNew->pNext            = ptypeListNow->pNext;
    ptypeListNow->pNext->pPrevious = ptypeListNew;
    ptypeListNow->pNext            = ptypeListNew;
    ptypeListNew->pPrevious        = ptypeListNow;

    return 0;
}

int8_t cListInsertHead(ListType *ptypeListHead, ListType *ptypeListNew)
{
    if((ptypeListHead == NULL) || (ptypeListNew == NULL))
        return -1;

    ptypeListNew->pNext             = ptypeListHead->pNext;
    ptypeListHead->pNext->pPrevious = ptypeListNew;
    ptypeListHead->pNext            = ptypeListNew;
    ptypeListNew->pPrevious         = ptypeListHead;

    return 0;
}

int8_t cListInsertEnd(ListType *ptypeListHead, ListType *ptypeListNew)
{
    if((ptypeListHead == NULL) || (ptypeListNew == NULL))
        return -1;

    ptypeListNew->pPrevious         = ptypeListHead->pPrevious;
    ptypeListHead->pPrevious->pNext = ptypeListNew;
    ptypeListHead->pPrevious        = ptypeListNew;
    ptypeListNew->pNext             = ptypeListHead;

    return 0;
}

int8_t cListRemove(ListType *ptypeListNow)
{
    if(ptypeListNow == NULL)
        return -1;

    ptypeListNow->pPrevious->pNext = ptypeListNow->pNext;
    ptypeListNow->pNext->pPrevious = ptypeListNow->pPrevious;

    return 0;
}
