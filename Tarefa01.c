#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/irq.h"

#define JOYSTICK_VRX 27
#define BUZZER_PIN 21
#define LED_R 13
#define LED_G 11
#define LED_B 12

volatile int estado = 0;
const int LIMIAR_CRITICO = 3000; // ajuste conforme necessário

// Configura LED RGB
void init_leds() {
    gpio_init(LED_R); gpio_set_dir(LED_R, GPIO_OUT);
    gpio_init(LED_G); gpio_set_dir(LED_G, GPIO_OUT);
    gpio_init(LED_B); gpio_set_dir(LED_B, GPIO_OUT);
}

// PWM no buzzer
void init_buzzer_pwm() {
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(BUZZER_PIN);
    pwm_set_wrap(slice, 12500); // 1000 Hz: 125 MHz / 12500 = 10 kHz base freq
    pwm_set_chan_level(slice, pwm_gpio_to_channel(BUZZER_PIN), 6250); // 50% duty cycle
    pwm_set_enabled(slice, false);
}

// Toca um som no buzzer
void buzzer_on() {
    uint slice = pwm_gpio_to_slice_num(BUZZER_PIN);
    pwm_set_enabled(slice, true);
}

// Para o som do buzzer
void buzzer_off() {
    uint slice = pwm_gpio_to_slice_num(BUZZER_PIN);
    pwm_set_enabled(slice, false);
}

// Define cor do LED RGB
void set_led_rgb(bool r, bool g, bool b) {
    gpio_put(LED_R, r);
    gpio_put(LED_G, g);
    gpio_put(LED_B, b);
}

// Leitura do joystick no Core 0
void alarme_callback() {
    adc_select_input(1); // GPIO27 = ADC1
    estado = adc_read();
    multicore_fifo_push_blocking(estado);
}

int64_t repetir_alarme(alarm_id_t id, void *user_data) {
    alarme_callback();
    return 2000; // repetir a cada 2 segundos
}

// Core 1: leitura da FIFO e ação nos atuadores
void core1_main() {
    init_leds();
    init_buzzer_pwm();

    while (true) {
        if (multicore_fifo_rvalid()) {
            int recebido = multicore_fifo_pop_blocking();
            if (recebido > LIMIAR_CRITICO) {
                set_led_rgb(1, 0, 0); // Vermelho
                buzzer_on();
            } else if (recebido > LIMIAR_CRITICO / 2) {
                set_led_rgb(0, 0, 1); // Azul
                buzzer_off();
            } else {
                set_led_rgb(0, 1, 0); // Verde
                buzzer_off();
            }
        }
    }
}

int main() {
    stdio_init_all();
    adc_init();
    adc_gpio_init(JOYSTICK_VRX);

    multicore_launch_core1(core1_main);
    add_alarm_in_ms(2000, repetir_alarme, NULL, true);

    while (true) {
        tight_loop_contents(); // evita travamento
    }
}
