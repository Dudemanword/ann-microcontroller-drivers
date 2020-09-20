#include "UART0.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"

#include "task.h"

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
    UART0Init();
    vTaskStartScheduler();
}

void vApplicationStackOverflowHook( TaskHandle_t xTask, signed char *pcTaskName )
{
    DDRA |= (1 << DDRA3);
    PORTA |= (1 << PORTA3);
}