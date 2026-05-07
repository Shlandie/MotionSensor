#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "freertos/FreeRTOS.h"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"

#include "esp_log.h"

#include "display.h"

static const char TAG[] = "app_main";

void app_main(void)
{
	vTaskDelay(pdMS_TO_TICKS(300));
	// Wait 1 min for motion sensor to stabilize
	ESP_LOGI(TAG, "Motion sensor stabilizing ( 1 min )");
   	vTaskDelay(pdMS_TO_TICKS(60000));
	display_task_create();
}
