/* GPIO Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "math.h"

/**
 * Brief:
 * This test code shows how to configure gpio and how to use gpio interrupt.
 *
 * GPIO status:
 * GPIO18: output
 * GPIO19: output
 * GPIO4:  input, pulled up, interrupt from rising edge and falling edge
 * GPIO5:  input, pulled up, interrupt from rising edge.
 *
 * Test:
 * Connect GPIO18 with GPIO4
 * Connect GPIO19 with GPIO5
 * Generate pulses on GPIO18/19, that triggers interrupt on GPIO4/5
 *
 */

// -----------------------------------------------------------
#define GPIO_INPUT_IO_0     4 // channel A
#define GPIO_INPUT_IO_1     6 // channel B
#define GPIO_INPUT_PIN_SEL  (1ULL<<GPIO_INPUT_IO_0)
#define GPIO_INPUT_PINB_SEL (1ULL<<GPIO_INPUT_IO_1) 

#define ESP_INTR_FLAG_DEFAULT 0


// -----------------------------------------------------------
void gpio_encoder_R(int *param) ;

// -----------------------------------------------------------
void app_main(void)
{
    gpio_reset_pin(GPIO_INPUT_IO_0) ;
    gpio_reset_pin(GPIO_INPUT_IO_1) ;
    
    gpio_config_t io_conf_A = {
        .pin_bit_mask = GPIO_INPUT_PIN_SEL,
        .intr_type = GPIO_INTR_DISABLE ,
        .mode = GPIO_MODE_INPUT ,
        .pull_up_en = 0 ,
    };

    gpio_config_t io_conf_B = {
        .pin_bit_mask = GPIO_INPUT_PINB_SEL,
        .intr_type = GPIO_INTR_DISABLE ,
        .mode = GPIO_MODE_INPUT ,
        .pull_up_en = 0 ,
    };


    gpio_config(&io_conf_A) ;
    gpio_config(&io_conf_B) ;

    gpio_set_intr_type(GPIO_INPUT_IO_0, GPIO_INTR_POSEDGE) ;
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT) ;
    gpio_isr_handler_add(GPIO_INPUT_IO_0, gpio_isr_handler, (void*) GPIO_INPUT_IO_0) ;


    int LocalVal = 0;

    while(1) {
        if(ESP_INTR_FLAG_DEFAULT){gpio_encoder_R(&LocalVal) ;}
        printf("Valeur de l'encodeur : %d\n", LocalVal) ;
    }
}

// -----------------------------------------------------------
void gpio_encoder_R(int *param)
{
    if(gpio_get_level(GPIO_INPUT_IO_1)){(*param)++ ;}
    else{(*param)-- ;}
}
