#include "soc/gpio_num.h"

#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/idf_additions.h"

#include "esp_log.h"

#include "tm1637.h"

#include "display.h"
#include "common.h"
#include "lamp_private.h"

static const char TAG[] = "display_task";
TaskHandle_t display_task_handle;
tm1637_handle_t display_handle;

TickType_t g_display_time;									// Made global so ISRS can adjust and let the display start countdown at exact 1s interval after their events
bool g_display_shutdown_display;									

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
	for(;;)
	{
		display_events = xEventGroupCreate();
		xTaskNotifyWaitIndexed(0, 0, (1 << 0), NULL, portMAX_DELAY);
		
		uint8_t count = 6;
		for(int i = 0; i <= count; i++)
		{
			//g_display_time = xTaskGetTickCount();
		/*
			 Check if Motion Sensor ISR reset the internal sensors timer and shut down the display if so
			 Else do countdown if Motion Sensor output is low (sensors base value 3 seconds + 3 seconds from manual addition) */

			tm1637_show_number(display_handle, (count - i),
						                                 0, 4, 0);
			if (xTaskNotifyWaitIndexed(1, 0, (1 << DISPLAY_EVENT_RESTART_COUNT), NULL, pdMS_TO_TICKS(1000)) == pdTRUE)
			{
				count = 6;	
			}
		}
		lamp_stop_internal();
	}
}


void display_task_create()
{
	xTaskCreatePinnedToCore(display_task, "display_task", DISPLAY_TASK_STACK_SIZE, NULL, DISPLAY_TASK_PRIORITY, &display_task_handle, DISPLAY_TASK_CORE_ID);
}






