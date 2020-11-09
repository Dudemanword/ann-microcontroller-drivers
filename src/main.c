#include "UART0.h"
#include "UART1.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "queue.h"

#define IMAGE_ROW_BYTES 1
#define ITEM_SIZE sizeof(uint8_t)

static StaticQueue_t xStaticQueue;
uint8_t ucQueueStorageArea[IMAGE_ROW_BYTES];
#if configSUPPORT_STATIC_ALLOCATION
/* static memory allocation for the IDLE task */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize) 
{
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
    *ppxIdleTaskStackBuffer = &xIdleStack[0];
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
#endif

int main()
{
    QueueHandle_t xQueue;
    xQueue = xQueueCreateStatic(IMAGE_ROW_BYTES,
                                ITEM_SIZE,
                                ucQueueStorageArea,
                                &xStaticQueue);
    UART0Init(xQueue);
    UART1Init(xQueue);
    vTaskStartScheduler();
}

void vApplicationStackOverflowHook( TaskHandle_t xTask, signed char *pcTaskName )
{
    DDRA |= (1 << DDRA3);
    PORTA |= (1 << PORTA3);
}