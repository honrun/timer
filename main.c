/*
 *  modifier: Honrun
 */
#include <stdio.h>
#include <stdlib.h>
#include "stdint.h"
#include "DevicesList.h"
#include "DevicesSoftTimer.h"



SoftTimerTypeDef g_typeSoftTimerBuff[16] = {0};




int main(void)
{
    struct ListStruct *ptypeListNow = NULL;
    long timeZone = 0;
    int32_t i = 0;

    for(i = 0; i < 16; ++i)
    {
        cSoftTimerMachineInit(&g_typeSoftTimerBuff[i], (rand() & 0xF) + 4, softTimerOpen | softTimerCircular, vSoftTimerCallbackFunction);
        printf("%02d: %d\r\n", i, g_typeSoftTimerBuff[i].timeDuration);
    }

    printf("\n\n\rcListInsert.\r\n\n");

    i = 0;
    for(ptypeListNow = g_typeListSoftTimerHead.pNext; ptypeListNow != &g_typeListSoftTimerHead; ptypeListNow = ptypeListNow->pNext)
    {
        printf("%02d: %d\r\n", i++, ptypeListNow->value);
    }

    printf("\n\n\rcSoftTimerMachine.\r\n\n");

    while(1)
    {
        cSoftTimerMachine();
    }

    printf("\n\n\rexit.\r\n");

    return 0;
}
