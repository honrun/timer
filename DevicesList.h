#ifndef DEVICESLIST_H_INCLUDED
#define DEVICESLIST_H_INCLUDED

#include "stdint.h"



#define LIST_VALUE_MAX 0x7FFFFFFF





typedef struct ListStruct{
    struct ListStruct *pPrevious;
    struct ListStruct *pNext;

    int64_t value;                  /* ≈≈–Ú÷µ */

    void *pElement;                 /* À˘ Ù‘™Àÿ */
}ListType;










extern ListType g_typeListSoftTimerHead;








int8_t cListInit(ListType *ptypeList, int64_t lValue);
int8_t cListInsert(ListType *ptypeListHead, ListType *ptypeListNew);
int8_t cListInsertHead(ListType *ptypeListHead, ListType *ptypeListNew);
int8_t cListInsertEnd(ListType *ptypeListHead, ListType *ptypeListNew);
int8_t cListRemove(ListType *ptypeListNow);



#endif // DEVICESLIST_H_INCLUDED
