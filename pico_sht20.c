#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

const int SDA_PIN = 20;
const int SCL_PIN = 21;

const uint8_t txdata_temp = 0b11100011;
const uint8_t txdata_hum = 0b11100101;
const uint8_t addr = 0b01000000;

int ret;

uint8_t rxdata[3];

int main() {

    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Enable UART so we can print status output
    stdio_init_all();

    // This example will use I2C0 on the default SDA and SCL pins (GP20, GP21 on a Pico)
    i2c_init(i2c0, 100 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);
    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(SDA_PIN, SCL_PIN, GPIO_FUNC_I2C));

    while(true) {
        sleep_ms(5000);
        gpio_put(LED_PIN, 1);

        //printf("Iniciando Leitura do sensor\n");

        ret = i2c_write_blocking(i2c0, addr, &txdata_temp, 1, false);
        if (ret < 0) {
            printf("Erro na leitura do sensor 1\n");
        } else {
            ret = i2c_read_blocking(i2c0, addr, rxdata, 3, false);
            if (ret < 0) {
                printf("Erro na leitura do sensor 2\n");
            } else {
                uint16_t st;
                st = (rxdata[0] << 8) | (rxdata[1] & 0b11111100);
                float temp = -46.85 + 175.72 * ((float) st / (float) 65536);
                printf("\rTemperatura %.3f °C   ", temp);
            }
        }

        ret = i2c_write_blocking(i2c0, addr, &txdata_hum, 1, false);
        if (ret < 0) {
            printf("Erro na leitura do sensor 1\n");
        } else {
            ret = i2c_read_blocking(i2c0, addr, rxdata, 3, false);
            if (ret < 0) {
                printf("Erro na leitura do sensor 2\n");
            } else {
                uint16_t srh;
                srh = (rxdata[0] << 8) | (rxdata[1] & 0b11111100);
                float humi = -6.0 + 125.0 * ((float) srh / (float) 65536);
                printf("Umidade Relativa %.2f %%", humi);
            }
        }
        gpio_put(LED_PIN, 0);
    }
    return 0;
}
