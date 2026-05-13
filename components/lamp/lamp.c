#include <string.h>

#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/idf_additions.h"

#include "esp_timer.h"

#include "driver/gpio.h"
#include "driver/ledc.h"

#include "lamp.h"
#include "common.h"
#include "display.h"


TaskHandle_t lamp_task_handle;
SemaphoreHandle_t lamp_semaphore_led_loop;
static volatile uint16_t g_current_duty = 2500;

/////////////////////////////////////////////////////////////
//					INTERNAL FUNCTIONS					   //
/////////////////////////////////////////////////////////////
void lamp_stop_internal()
{
	ledc_stop(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, OFF);
}


/////////////////////////////////////////////////////////////
//						INITILIZATION					   //
/////////////////////////////////////////////////////////////
void lamp_gpio_init(void)
{
	gpio_config_t gpio_setup;
	
	// Setting up motion sensor
	memset(&gpio_setup, 0x00, sizeof(gpio_config_t));
	gpio_setup.pin_bit_mask	= (1ULL << LAMP_GPIO_MOTION_SENSOR);
	gpio_setup.mode 		= GPIO_MODE_INPUT;
	gpio_setup.intr_type	= GPIO_INTR_ANYEDGE;
	gpio_setup.pull_up_en	= GPIO_PULLUP_DISABLE;
	gpio_setup.pull_down_en	= GPIO_PULLDOWN_DISABLE;
	ESP_ERROR_CHECK(gpio_config(&gpio_setup));
	
	// Setting up button
	gpio_setup.pin_bit_mask	= (1ULL << LAMP_GPIO_BUTTON);
	gpio_setup.mode 		= GPIO_MODE_INPUT;
	gpio_setup.intr_type	= GPIO_INTR_NEGEDGE;
	ESP_ERROR_CHECK(gpio_config(&gpio_setup));
}


void lamp_ledc_init(void)
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
	ESP_ERROR_CHECK(ledc_timer_config(&ledc_setup));
	
	memset(&ledc_channel_setup, 0x00, sizeof(ledc_channel_config_t));
	ledc_channel_setup.sleep_mode 	= LEDC_LOW_SPEED_MODE;
	ledc_channel_setup.gpio_num		= LAMP_GPIO_LED;
	ledc_channel_setup.channel		= LEDC_CHANNEL_0;
	ledc_channel_setup.duty			= 0;
	ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_setup));
	ledc_fade_func_install(0);
}


/*
* ISR handle for the motion sensor interrupt (ANYEDGE)
*/
static void IRAM_ATTR motion_sensor_isr(void *args)
{
	
	// Check if it's falling or rising edge
	if(gpio_get_level(LAMP_GPIO_MOTION_SENSOR) == 1)
	{
		g_display_shutdown_display = true;
		xSemaphoreGiveFromISR(lamp_semaphore_led_loop, NULL);
	}
	else
	{
		xEventGroupSetBitsFromISR(display_events, DISPLAY_EVENT_START, NULL);
		g_display_shutdown_display = false;
		
	}
	
}

/*
* ISR handle for the button ()
*/
static void IRAM_ATTR button_isr(void *args)
{
	
	static int64_t last_press = 0;
    int64_t now = esp_timer_get_time();
    if (now - last_press < 50000) return; // 50ms debounce
    last_press = now;
	
	if(g_current_duty == 15000)
	{
		g_current_duty = 0;
	}
	g_current_duty += 2500;
	
	xSemaphoreGiveFromISR(lamp_semaphore_led_loop, NULL);
	
}


void lamp_gpio_isr_init()
{	
	// Add motion sensor GPIO ISR handler
	ESP_ERROR_CHECK(gpio_isr_handler_add(LAMP_GPIO_MOTION_SENSOR, motion_sensor_isr, NULL));
	
	// Add button GPIO ISR handler
	ESP_ERROR_CHECK(gpio_isr_handler_add(LAMP_GPIO_BUTTON, button_isr, NULL));
}


static void lamp_task(void *pvParameters)
{
	lamp_semaphore_led_loop = xSemaphoreCreateBinary();
	for(;;)
	{
		xSemaphoreTake(lamp_semaphore_led_loop, portMAX_DELAY);
		ledc_set_duty_and_update(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, g_current_duty, 0);
	}
	
}


void lamp_task_create()
{
	xTaskCreatePinnedToCore(lamp_task, "lamp_task", LAMP_TASK_STACK_SIZE, NULL, LAMP_TASK_PRIORITY, &lamp_task_handle, LAMP_TASK_CORE_ID);
}

























