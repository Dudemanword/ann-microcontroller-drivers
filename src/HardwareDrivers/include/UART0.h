#ifndef UART0_H_
#define UART0_H_

#include "FreeRTOS.h"
#include "queue.h"

void UART0Init(QueueHandle_t* queueHandle);
void Uart0Task();

#endif