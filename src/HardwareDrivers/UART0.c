#include "UART0.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "stdint.h"
#include "task.h"

//sudo avarice -4 :4242
#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((( F_CPU / 16) + ( USART_BAUDRATE / 2)) / ( USART_BAUDRATE )) - 1)
#define STACK_SIZE configMINIMAL_STACK_SIZE

const TickType_t xDelay = 1000 / portTICK_PERIOD_MS;
static StackType_t xStack[STACK_SIZE];
static StaticTask_t xTaskBuffer;	
static uint8_t receivedByte;
static TaskHandle_t xTaskToNotify;

void UART0Init()
{
    DDRE |= (1 << DDRE1);
    DDRE &= ~(1 << DDRE0);
    UBRR0L = (uint8_t)(0x2F);
    UBRR0H |= (UBRR0H & 0xF0);
    /* Enable receiver and transmitter */
    UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0)|(1<<TXCIE0);
    
    UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
    UCSR0C &= ~(1 << USBS0);
    DDRA |= (1 << DDRA1) | (1 << DDRA2) | (1 << DDRA3);
    sei();

    xTaskToNotify = xTaskCreateStatic(
        UartTask, 
        "Uart Task", 
        STACK_SIZE, 
        NULL, 
        tskIDLE_PRIORITY + 1,
        xStack, 
        &xTaskBuffer);

}

void UartTask()
{
    while (1)
    {
        uint32_t ulNotificationValue = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        if(ulNotificationValue == 1)
        {
            UDR0 = receivedByte;
        }

    }
    
}

ISR(USART0_RX_vect)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    receivedByte = UDR0;
    vTaskNotifyGiveFromISR(xTaskToNotify, &xHigherPriorityTaskWoken);
}

ISR(USART0_TX_vect)
{
    PORTA ^= (1 << PORTA1);
}