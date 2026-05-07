#include <stdio.h>

#include "freertos/FreeRTOS.h"

#include "esp_log.h"

#include "tm1637.h"

#include "common.h"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"
#include "soc/gpio_num.h"
#include "display.h"

static const char TAG[] = "display_task";

static TaskHandle_t display_task_handle;
tm1637_handle_t display_handle;

TickType_t g_display_time;									// Made global so ISRS can adjust and let the display start countdown at exact 1s interval after their events
bool shutdown_display = false;									

/*
* Initialize the display via heronet_tm1637 dependancy
@return ESP_OK
*/

esp_err_t display_init()
{
	tm1637_config_t display_config;
	display_config.bit_delay_us = 100;
	display_config.clk_pin 		= GPIO_NUM_15;
	display_config.dio_pin		= GPIO_NUM_16;
	if (tm1637_init(&display_config, &display_handle) != ESP_OK)
	{                                                                                                                 
		ESP_LOGI(TAG, "Display configuration error!");
	}
	tm1637_set_brightness(display_handle, 7, true);
	
	return ESP_OK;
}



static void display_task(void *pvParameters)
{
	display_init();
	
	for(;;)
	{
		/*
		xSemaphoreTake(semaphore_start_display, portMAX_DELAY);
		
		uint8_t count = 6;
		for(int i = 0; i <= count; i++)
		{
			g_display_time = xTaskGetTickCount();
			*/
			/* Check if Motion Sensor ISR reset the internal sensors timer and shut down the display if so
			 Else do countdown if Motion Sensor output is low (sensors base value 3 seconds + 3 seconds from manual addition) */
			/*if(shutdown_display == true)
			{
				break;
			}
			
			tm1637_show_number(display_handle, (count - i),
						                                 0, 4, 0);
			
			vTaskDelayUntil(&g_display_time, pdMS_TO_TICKS(1000));	
		}
		
		if(shutdown_display == false)
		{
			ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
			ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
			tm1637_clear(display_handle);
		*/
		vTaskDelay(pdMS_TO_TICKS(300));
	}
	
}


void display_task_create()
{
	xTaskCreatePinnedToCore(display_task, "display_task", DISPLAY_TASK_STACK_SIZE, NULL, DISPLAY_TASK_PRIORITY, &display_task_handle, DISPLAY_TASK_CORE_ID);
}






