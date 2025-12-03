#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"
#include "driver/i2c.h"
#include "driver/gpio.h"
#include "headers/button.h"
#include "headers/shtc3.h"
#include "headers/potentiometer.h"

static uint8_t s_state = 1;

void app_main(void)
{
    button_configure();

    shtc3_init();

    adc_oneshot_unit_handle_t adcHandle;
    adc_cali_handle_t calHandle;
    potentiometer_configure_adc(&adcHandle, &calHandle);

    float temp;
    float hum;

    uint8_t btnRightActive = 0;

    while (1)
    {
        if (gpio_get_level(9) == 0) // Right Button
        {
            if (++btnRightActive == 2)
            {
                if (s_state < 2)
                {
                    s_state += 1;
                }
                else
                {
                    s_state = 0;
                }
            }
        }
        else
        {
            btnRightActive = 0;
        }

        shtc3_getValues(&temp, &hum);
        int32_t resistance = potentiometer_get_resistance(&adcHandle, &calHandle);

        if (s_state == 0)
        {
            printf("{\"payload\":[{\"displayName\":\"Temperature\",\"displayUnit\":\"°C\",\"data\":%.2f}]}\n", temp);
        }
        else if (s_state == 1)
        {
            printf("{\"payload\":[{\"displayName\":\"Temperature\",\"displayUnit\":\"°C\",\"data\":%.2f},{\"displayName\":\"Humidity\",\"displayUnit\":\"%%\",\"data\":%.2f}]}\n", temp, hum);
        }
        else
        {
            printf("{\"payload\":[{\"displayName\":\"Temperature\",\"displayUnit\":\"°C\",\"data\":%.2f},{\"displayName\":\"Humidity\",\"displayUnit\":\"%%\",\"data\":%.2f},{\"displayName\":\"Resistance\",\"displayUnit\": \"Ohm\",\"data\":%ld}]}\n", temp, hum, resistance);
        }

        vTaskDelay(pdMS_TO_TICKS(55));
    }
}