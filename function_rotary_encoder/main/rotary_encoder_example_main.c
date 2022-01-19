/* PCNT example -- Rotary Encoder

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "rotary_encoder.h"

static const char *TAG = "example";

/* Definit les ports GPIO des 2 encodeurs */
#define ENC_RIGHT_CHANNEL_A 36
#define ENC_RIGHT_CHANNEL_B 37
#define ENC_LEFT_CHANNEL_A 38
#define ENC_LEFT_CHANNEL_B 39


void app_main(void)
{
    // Rotary encoder underlying device is represented by a PCNT unit in this example
    uint32_t pcnt_unit_right = 0;
    uint32_t pcnt_unit_left = 0;

    // Create rotary encoder instance
    rotary_encoder_config_t config_right = ROTARY_ENCODER_DEFAULT_CONFIG((rotary_encoder_dev_t)pcnt_unit_right, ENC_RIGHT_CHANNEL_A, ENC_RIGHT_CHANNEL_B);
    rotary_encoder_config_t config_left = ROTARY_ENCODER_DEFAULT_CONFIG((rotary_encoder_dev_t)pcnt_unit_left, ENC_LEFT_CHANNEL_A, ENC_LEFT_CHANNEL_B);

    rotary_encoder_t *encoder_right = NULL;
    rotary_encoder_t *encoder_left = NULL;

    ESP_ERROR_CHECK(rotary_encoder_new_ec11(&config_right, &encoder_right));
    ESP_ERROR_CHECK(rotary_encoder_new_ec11(&config_left, &encoder_left));


    // Filter out glitch (1us)
    ESP_ERROR_CHECK(encoder_right->set_glitch_filter(encoder_right, 1));
    ESP_ERROR_CHECK(encoder_left->set_glitch_filter(encoder_left, 1));


    // Start encoder
    ESP_ERROR_CHECK(encoder_right->start(encoder_right));
    ESP_ERROR_CHECK(encoder_left->start(encoder_left));


    // Report counter value
    while (1) {
        ESP_LOGI(TAG, "Encoder right value: %d", encoder_right->get_counter_value(encoder_right));
        ESP_LOGI(TAG, "Encoder left value: %d", encoder_left->get_counter_value(encoder_left));

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
