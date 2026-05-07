#include <stdio.h>

#include "freertos/FreeRTOS.h"

#include "esp_log.h"

#include "driver/gpio.h"
#include "driver/ledc.h"

#include "lamp.h"
#include "common.h"



SemaphoreHandle_t semaphore_start_display;
static uint16_t g_current_duty = 2000;

/*
* Initialize all GPIOs for the program.
* Motion sensor (INPUT), 
* Button (INPUT)  
*/
static esp_err_t init_GPIO(void)
{
	gpio_config_t gpio_setup;
	semaphore_start_display = xSemaphoreCreateBinary();
	
	// Setting up motion sensor
	memset(&gpio_setup, 0x00, sizeof(gpio_config_t));
	gpio_setup.pin_bit_mask	= (1ULL << DISPLAY_GPIO_MOTION_SENSOR);
	gpio_setup.mode 		= GPIO_MODE_INPUT;
	gpio_setup.intr_type	= GPIO_INTR_ANYEDGE;
	gpio_setup.pull_up_en	= GPIO_PULLUP_DISABLE;
	gpio_setup.pull_down_en	= GPIO_PULLDOWN_DISABLE;
	gpio_config(&gpio_setup);
	
	// Setting up button
	gpio_setup.pin_bit_mask	= (1ULL << DISPLAY_GPIO_BUTTON);
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
static esp_err_t init_LEDC(void)
{
	ledc_timer_config_t ledc_setup;
	ledc_channel_config_t ledc_channel_setup;
	
	// Setting up main LED
	memset(&ledc_setup, 0x00, sizeof(ledc_timer_config_t));
	ledc_setup.speed_mode 			= LEDC_LOW_SPEED_MODE;
	ledc_setup.freq_hz				= 1500;
	ledc_setup.duty_resolution		= LEDC_TIMER_14_BIT;
	ledc_setup.timer_num			= LEDC_TIMER_0;
	ledc_setup.clk_cfg				= LEDC_AUTO_CLK;
	ledc_timer_config(&ledc_setup);
	
	memset(&ledc_channel_setup, 0x00, sizeof(ledc_channel_config_t));
	ledc_channel_setup.sleep_mode 	= LEDC_LOW_SPEED_MODE;
	ledc_channel_setup.gpio_num		= DISPLAY_GPIO_LED;
	ledc_channel_setup.channel		= LEDC_CHANNEL_0;
	ledc_channel_setup.duty			= 0;
	ledc_channel_config(&ledc_channel_setup);
	
	return ESP_OK;
}


/*
* ISR handle for the motion sensor interrupt (ANYEDGE)
*/
static void IRAM_ATTR motion_sensor_isr(void *args)
{
	
	// Check if it's falling or rising edge
	if(gpio_get_level(DISPLAY_GPIO_MOTION_SENSOR) == 1)
	{

	}
	else
	{
		//shutdown_display = false;
		//xSemaphoreGiveFromISR(semaphore_start_display, NULL);
	}
	
}

/*
* ISR handle for the button ()
*/
static void IRAM_ATTR button_isr(void *args)
{
	
}

/*
* Initialize ISRS for motion sensor (FALLING AND RISING EDGE) and button
@return ESP_OK
*/
static esp_err_t init_ISRS()
{
	gpio_install_isr_service(0);
	
	// Add motion sensor GPIO ISR handler
	gpio_isr_handler_add(DISPLAY_GPIO_MOTION_SENSOR, motion_sensor_isr, NULL);
	
	// Add button GPIO ISR handler
	gpio_isr_handler_add(DISPLAY_GPIO_BUTTON, button_isr, NULL);
	
	return ESP_OK;
}

