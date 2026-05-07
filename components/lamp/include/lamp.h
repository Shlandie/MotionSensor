#pragma once

#include "esp_err.h"

#define DISPLAY_GPIO_MOTION_SENSOR 					11
#define DISPLAY_GPIO_BUTTON							12
#define DISPLAY_GPIO_LED							10

/*
* Initialize all GPIOs for the program.
* Motion sensor (INPUT), 
* Button (INPUT)  
* @return ESP_OK
*/
static void lamp_gpio_init(void);

/*
* Initialize single LEDC to control variable brightness via button interrupt later on
@return ESP_OK
*/
static void lamp_ledc_init(void);

/*
* Initialize ISRS for motion sensor (FALLING AND RISING EDGE) and button
* @return ESP_OK
*/
void lampini(void);


