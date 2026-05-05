#include <stdio.h>

#include "freertos/FreeRTOS.h"

#include "esp_log.h"
#include "esp_event_base.h"

#include "driver/gpio.h"

#include "common.h"
#include "freertos/idf_additions.h"
#include "soc/gpio_num.h"
#include "display.h"

TaskHandle_t display_task_handle;

/*
* Initialize all GPIOs for the program.
* Motion sensor (INPUT), 
* Button (INPUT)  
*/
esp_err_t init_GPIO()
{
	gpio_config_t gpio_setup;
	
	// Setting up motion sensor
	gpio_setup.pin_bit_mask	= (1ULL << DISPLAY_GPIO_MOTION_SENSOR);
	gpio_setup.mode 		= GPIO_MODE_INPUT;
	gpio_setup.intr_type	= GPIO_INTR_ANYEDGE;
	gpio_setup.pull_up_en	= GPIO_PULLUP_DISABLE;
	gpio_setup.pull_down_en	= GPIO_PULLDOWN_DISABLE;
	gpio_config(&gpio_setup);
	
	// Setting up button
	gpio_setup.pin_bit_mask	= (1ULL << DISPLAY_GPIO_MOTION_SENSOR);
	gpio_setup.mode 		= GPIO_MODE_INPUT;
	gpio_setup.intr_type	= GPIO_INTR_LOW_LEVEL;
	gpio_config(&gpio_setup);
	
	return ESP_OK;
}

/*
* Initialize single LEDC to control variable brightness via button interrupt later on
@params gpio - the GPIO the LED is gonna be connected to
@return ESP_OK
*/
esp_err_t init_LEDC(gpio_num_t gpio)
{
	
}

/*
* Initialize ISRS for motion sensor (FALLING AND RISING EDGE) and button
@return ESP_OK
*/
esp_err_t init_ISRS()
{
	
}

/*
* Initialize the display via heronet_tm1637 dependancy
@return ESP_OK
*/
esp_err_t display_init()
{
	
}


void display_task(void *pvParameters)
{
	
	for(;;)
	{
		
	}
}


void create_display_task()
{
	xTaskCreatePinnedToCore(display_task, "display_task", DISPLAY_TASK_STACK_SIZE, NULL, DISPLAY_TASK_PRIORITY, &display_task_handle, DISPLAY_TASK_CORE_ID);
}






