#include "headers/potentiometer.h"

void potentiometer_configure_adc(adc_oneshot_unit_handle_t *adcHandle, adc_cali_handle_t *calHandle)
{
    adc_oneshot_unit_init_cfg_t adcConfig = {
        .unit_id = ADC_UNIT_1,
        .ulp_mode = ADC_ULP_MODE_DISABLE};
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&adcConfig, adcHandle));
    adc_oneshot_chan_cfg_t channelConfig = {
        .bitwidth = ADC_BITWIDTH_12,
        .atten = ADC_ATTEN_DB_11};
    ESP_ERROR_CHECK(adc_oneshot_config_channel(*adcHandle, ADC_CHANNEL_2, &channelConfig));

    adc_cali_curve_fitting_config_t calConfig = {
        .unit_id = ADC_UNIT_1,
        .atten = ADC_ATTEN_DB_11,
        .bitwidth = ADC_BITWIDTH_12,
    };

    adc_cali_create_scheme_curve_fitting(&calConfig, calHandle);
}

int32_t potentiometer_get_resistance(adc_oneshot_unit_handle_t *adcHandle, adc_cali_handle_t *calHandle)
{
    int rawValue;
    int voltage_mV;

    adc_oneshot_read(*adcHandle, ADC_CHANNEL_2, &rawValue);
    adc_cali_raw_to_voltage(*calHandle, rawValue, &voltage_mV);
    int32_t resist_ohm = (voltage_mV * 10000) / 2500;
    return resist_ohm;
}