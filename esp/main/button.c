#include "headers/button.h"

void button_configure(void)
{
    gpio_config_t gpioConfigIn = {
        .pin_bit_mask = (1 << GPIO_NUM_2) | (1 << GPIO_NUM_9),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = true,
        .pull_down_en = false,
        .intr_type = GPIO_INTR_DISABLE};

    gpio_config(&gpioConfigIn);
}