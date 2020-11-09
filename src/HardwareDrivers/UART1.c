#include "UART1.h"
#include <avr/io.h>
#include <avr/iom128a.h>
#include <avr/interrupt.h>
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "stdint.h"
#include "task.h"
#include "queue.h"

#define USART_BAUDRATE 9600
#define STACK_SIZE configMINIMAL_STACK_SIZE

static StackType_t xStack[STACK_SIZE];
static StaticTask_t xTaskBuffer;	
static QueueHandle_t* xQueue;

#define IMAGE_ROW_BYTES 255
static uint8_t pixel;

void UART1Init(QueueHandle_t* queueHandle)
{
    xQueue = queueHandle;
    DDRD |= (1 << DDRD1);
    DDRD &= ~(1 << DDRD2);
    UBRR1L = (uint8_t)(0x2F);
    UBRR1H |= (UBRR1H & 0xF0);
    UCSR1B = (1<<TXEN1)|(1<<TXCIE1);
    
    UCSR1C |= (1<<UCSZ11)|(1<<UCSZ10);
    UCSR1C &= ~(1 << USBS1);
    DDRA |= (1 << DDRA3);
    sei();
    PORTA &= ~(1 << PORTA3);
    xTaskCreateStatic(
        Uart1Task, 
        "Uart1 Task", 
        STACK_SIZE, 
        NULL, 
        tskIDLE_PRIORITY + 1,
        xStack, 
        &xTaskBuffer);

}

void Uart1Task()
{
    while (1)
    {
        if (xQueueReceive(xQueue, &pixel, portMAX_DELAY) == pdTRUE)
        {
            UDR1 = pixel;
        }
    }
    
}

ISR(USART1_TX_vect)
{
    PORTA ^= (1 << PORTA3);
    taskYIELD();
}