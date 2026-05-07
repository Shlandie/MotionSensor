#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "freertos/FreeRTOS.h"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"

#include "esp_err.h"
#include "esp_log.h"

#include "driver/gpio.h"
#include "lamp.h"
#include "display.h"

static const char TAG[] = "app_main";

void app_main(void)
{
	// Delay to not miss logs
	vTaskDelay(pdMS_TO_TICKS(1000));
	
	// Wait 1 min for motion sensor to stabilize
	ESP_LOGI(TAG, "Motion sensor stabilizing ( 1 min )");
   	vTaskDelay(pdMS_TO_TICKS(60000));
	
	// Init hardware
	init_GPIO();
	init_LEDC();
	init_displa
	
	
	// Install global GPIO ISR service
	ESP_ERROR_CHECK(gpio_install_isr_service(0));
	
	display_task_create();
}
