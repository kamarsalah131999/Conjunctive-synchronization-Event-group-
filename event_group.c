/*
 * Event group.c
 *
 * Created: 7/22/2023 2:21:34 AM
 * Author : kamar
 */ 

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

EventGroupHandle_t  my_event_group ;                                                       // Handle of event group

void SetBit0 (void *pv);
void SetBit1 (void *pv);
void task (void *pv);


int main(void)
{
    /*create tasks*/
	xTaskCreate(SetBit0, "setbit0", 100, NULL, 3, NULL);
	xTaskCreate(SetBit1, "setbit1", 100, NULL, 2, NULL);
	xTaskCreate(task, "task", 100, NULL, 1, NULL);
	
	/*create event group*/
	my_event_group= xEventGroupCreate();
	
	/* start OS */
	vTaskStartScheduler();	
}


void SetBit0 (void *pv)
{
	while(1)
	{
		vTaskDelay(5000);
		xEventGroupSetBits(my_event_group, 0x01);                                            // set bit0 
	}
}

void SetBit1 (void *pv)
{
	while(1)
	{
		vTaskDelay(2000);
		xEventGroupSetBits(my_event_group, 0x02);                                            // set bit1
	}
}


void task (void *pv)
{
	/* intialization */
	DDRD|=(1<<7);                                                                          // set pin7 in PORTD is output
    DDRD|=(1<<6);                                                                          // set pin6 in PORTD is output
	DDRD|=(1<<5);                                                                          // set pin5 in PORTD is output
	
	EventBits_t  return_value=0x00;                                                        // value that returns from the event group
	
	while (1)
	{
		return_value= xEventGroupWaitBits(my_event_group,0x03,pdTRUE,pdFALSE,1000);        // wait for 2 events>> bit0 OR bit1
		if (return_value==0x01)
		{
			PORTD^=(1<<7);                                                                // taggle the led
		}
		if (return_value==0x02)
		{
			PORTD^=(1<<6);                                                                // taggle the led
		}
		if (return_value==0x03)
		{
			PORTD^=(1<<5);                                                                // taggle the led
		}
	}
}
