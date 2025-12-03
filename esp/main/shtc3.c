#include "headers/shtc3.h"

void shtc3_init()
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = GPIO_NUM_5,
        .scl_io_num = GPIO_NUM_6,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 40000,
    };
    i2c_param_config(I2C_MASTER_NUM, &conf);
    ESP_ERROR_CHECK(i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0));
}

void shtc3_writeRegister(uint16_t cmd)
{
    uint8_t buf[2] = {cmd >> 8, cmd & 0xFF};
    ESP_ERROR_CHECK(i2c_master_write_to_device(I2C_MASTER_NUM, SHTC3_ADDR, buf, 2, pdMS_TO_TICKS(50)));
}

void shtc3_readRegister(float *pTemp, float *pHum)
{
    uint8_t data[6] = {0};
    ESP_ERROR_CHECK(i2c_master_read_from_device(I2C_MASTER_NUM, SHTC3_ADDR, data, 6, pdMS_TO_TICKS(100)));

    uint16_t rawT = (data[0] << 8) | data[1];
    uint16_t rawRH = (data[3] << 8) | data[4];

    *pTemp = -45 + 175 * ((float)rawT / 65535.0f);
    *pHum = 100 * ((float)rawRH / 65535.0f);
}

void shtc3_getValues(float *pTemp, float *pHum)
{
    shtc3_writeRegister(0x3517);
    vTaskDelay(pdMS_TO_TICKS(15));

    shtc3_writeRegister(0x7CA2);
    vTaskDelay(pdMS_TO_TICKS(20));

    shtc3_readRegister(pTemp, pHum);

    vTaskDelay(pdMS_TO_TICKS(10));
    shtc3_writeRegister(0xB098);
}