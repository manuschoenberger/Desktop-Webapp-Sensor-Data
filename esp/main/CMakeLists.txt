idf_component_register(SRCS "button.c" "potentiometer.c" "shtc3.c" "main.c"
                       INCLUDE_DIRS "." REQUIRES driver esp_adc lwip)
