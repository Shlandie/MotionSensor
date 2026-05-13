#pragma once

#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/idf_additions.h"
#include "esp_err.h"

extern TaskHandle_t display_task_handle;

extern EventGroupHandle_t display_events;

typedef enum display_task_notifications
{
	DISPLAY_EVENT_START 			= (1 << 0),
	DISPLAY_EVENT_RESTART_COUNT		= (1 << 1)
}display_events_e;

extern bool g_display_shutdown_display;

esp_err_t display_init(void);

void display_task_create(void);


