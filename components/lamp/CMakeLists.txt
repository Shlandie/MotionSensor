idf_component_register(SRCS "lamp.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "common" "display" "esp_driver_gpio" "esp_driver_ledc" "esp_event" "esp_timer")

