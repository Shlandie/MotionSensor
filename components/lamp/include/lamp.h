#pragma once

#define OFF											0

#define LAMP_GPIO_MOTION_SENSOR 					9
#define LAMP_GPIO_BUTTON							38
#define LAMP_GPIO_LED								10

/*
* Initialize all GPIOs for the program.
* Motion sensor (INPUT), 
* Button (INPUT)  
* @return ESP_OK
*/
void lamp_gpio_init(void);

/*
* Initialize single LEDC to control variable brightness via button interrupt later on
@return ESP_OK
*/
void lamp_ledc_init(void);

/*
* Initialize ISRS for motion sensor (FALLING AND RISING EDGE) and button
* @return ESP_OK
*/
void lamp_gpio_isr_init(void);


void lamp_task_create(void);

