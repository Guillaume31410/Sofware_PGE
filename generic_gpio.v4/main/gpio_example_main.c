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


// -----------------------------------------------------------
void gpio_encoder_R() ;
void other_task() ;

int globalEncR = 0 ;
// -----------------------------------------------------------
void app_main(void)
{
    gpio_reset_pin(GPIO_INPUT_IO_0) ;
    gpio_reset_pin(GPIO_INPUT_IO_1) ;
    
    gpio_config_t io_conf_A = {
        .pin_bit_mask = GPIO_INPUT_PIN_SEL,
        .intr_type = GPIO_INTR_POSEDGE ,
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

    int val;

    xTaskCreate(gpio_encoder_R, "encodeur_right_fnt", 2048, NULL, 1, NULL) ;
    //xTaskCreate(other_task, "other task", 2048, NULL, 1, NULL) ;

    while(1) {
        //gpio_encoder_R() ;
        printf("Valeur de l'encodeur : %d\n", globalEncR) ;
        vTaskDelay(pdMS_TO_TICKS(100));   
    }
}

// -----------------------------------------------------------
void gpio_encoder_R()
{
    int old = 0 ;
    int current ;

   while(1){
       current = gpio_get_level(GPIO_INPUT_IO_0) ;
       if((current != old) && (old==0)){
           if(gpio_get_level(GPIO_INPUT_IO_1)){globalEncR++ ;}
           else{globalEncR-- ;}
            //printf("val = %d\n", val) ;
       }
        old = current ;
   }
}

void other_task()
{
    while(1)
    {
        printf("[OTHER] Autre tache\n") ;
        vTaskDelay(10) ;
    }
}