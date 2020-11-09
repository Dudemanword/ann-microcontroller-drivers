#ifndef UART1_H_
#define UART1_H_

#include "FreeRTOS.h"
#include "queue.h"

void UART1Init(QueueHandle_t* queueHandle);
void Uart1Task();

#endif