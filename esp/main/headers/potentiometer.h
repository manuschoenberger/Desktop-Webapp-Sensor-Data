#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include "esp_adc/adc_oneshot.h"

void potentiometer_configure_adc(adc_oneshot_unit_handle_t *adcHandle, adc_cali_handle_t *calHandle);
int32_t potentiometer_get_resistance(adc_oneshot_unit_handle_t *adcHandle, adc_cali_handle_t *calHandle);

#endif