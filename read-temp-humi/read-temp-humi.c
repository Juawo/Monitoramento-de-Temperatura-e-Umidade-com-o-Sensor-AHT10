#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "include/temp_humi_sensor.h"
#include "include/buzzer.h"

int main()
{
    stdio_init_all();

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);
    pwm_init_buzzer(BUZZER_PIN);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c

    if (!aht10_init())
    {
        printf("Falha ao inicializar o sensor AHT10!\n");
        return 1;
    }

    while (true)
    {
        float temp, hum;
        if (aht10_read(&temp, &hum))
        {
            printf("Temperatura : %.2f C | Umidade : %.2f %%\n", temp, hum);
            if (temp < 20 || hum > 70)
            {
                printf("beep\n");
                beep(BUZZER_PIN, 500);
            }
        }
        else
        {
            printf("Erro na leitura dos dados!\n");
        }

        sleep_ms(1000);
    }
}
