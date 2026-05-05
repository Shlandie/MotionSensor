#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "freertos/FreeRTOS.h"

#include "esp_log.h"

#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"

static const char TAG[] = "app_main";

void app_main(void)
{
	// Wait 1 min for motion sensor to stabilize
	ESP_LOGI(TAG, "Motion sensor stabilizing ( 1 min )");
   	vTaskDelay(pdMS_TO_TICKS(60000));
}
