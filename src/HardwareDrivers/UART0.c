#include "UART0.h"
#include <avr/io.h>
#include <avr/iom128a.h>
#include <avr/interrupt.h>
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "stdint.h"
#include "task.h"

#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((( F_CPU / 16) + ( USART_BAUDRATE / 2)) / ( USART_BAUDRATE )) - 1)
#define STACK_SIZE configMINIMAL_STACK_SIZE

static StackType_t xStack[STACK_SIZE];
static StaticTask_t xTaskBuffer;	
static TaskHandle_t xTaskToNotify;
static QueueHandle_t* xQueue;
static uint8_t pixel;
void UART0Init(QueueHandle_t* queueHandle)
{
    xQueue = queueHandle;
    DDRE |= (1 << DDRE1);
    DDRE &= ~(1 << DDRE0);
    UBRR0L = (uint8_t)(0x2F);
    UBRR0H |= (UBRR0H & 0xF0);
    UCSR0B = (1 << RXEN0)|(1 << RXCIE0)|(1 << TXEN0);
    
    UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
    UCSR0C &= ~(1 << USBS0);
    DDRA |= (1 << DDRA1) | (1 << DDRA2) | (1 << DDRA3);
    sei();

    xTaskToNotify = xTaskCreateStatic(
        Uart0Task, 
        "Uart0 Task", 
        STACK_SIZE, 
        NULL, 
        tskIDLE_PRIORITY + 1,
        xStack, 
        &xTaskBuffer);

}

void Uart0Task()
{
    while (1)
    {
    }
    
}

ISR(USART0_RX_vect)
{
    uint8_t receivedByte = UDR0;
    PORTA ^= (1 << PORTA1);
    xQueueSendFromISR(xQueue, receivedByte, portMAX_DELAY);
    taskYIELD();
}