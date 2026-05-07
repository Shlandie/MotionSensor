#ifndef DISPLAY_H 
#define DISPLAY_H

#include "esp_event_base.h"

esp_err_t display_init(void);

void display_task_create(void);

#endif /*DISPLAY_H*/
