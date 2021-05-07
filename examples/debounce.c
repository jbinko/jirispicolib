#include <stdio.h>
#include "pico/stdlib.h"

const uint INPUT_GPIO = 21;

#define DEBOUNCE_MS 50
bool is_debounceing = false;

int64_t debounce_alarm_callback(alarm_id_t id, void * user_data) {
  printf("GPIO IRQ %d\n", gpio_get(INPUT_GPIO));
  is_debounceing = false;
  return 0;
}

void gpio_callback(uint gpio, uint32_t events) {

  if (!is_debounceing) {
    add_alarm_in_ms(DEBOUNCE_MS, & debounce_alarm_callback, NULL, false);
    is_debounceing = true;
  }
}

int main() {

  stdio_init_all();
  gpio_set_irq_enabled_with_callback(INPUT_GPIO, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, & gpio_callback);

  for (;;)
    sleep_ms(60000);

  return 0;
}





#if YYYYY


#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/gpio.h"

const uint INPUT_GPIO = 21;
const uint LED_PIN = PICO_DEFAULT_LED_PIN;




#define DEBOUNCE_MS 80
bool is_debounceing = false;

int64_t debounce_alarm_callback(alarm_id_t id, void *user_data) {
    is_debounceing = false;
    printf("GPIO IRQ %d\n", gpio_get(INPUT_GPIO));
    gpio_put(LED_PIN, gpio_get(INPUT_GPIO));
    return 0;
}




void gpio_callback(uint gpio, uint32_t events) {


if (!is_debounceing) {
        add_alarm_in_ms(DEBOUNCE_MS, &debounce_alarm_callback, NULL, false);
        is_debounceing = true;
    }


}

int main() {

stdio_init_all();
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
  gpio_set_irq_enabled_with_callback(INPUT_GPIO, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, & gpio_callback);

  // Wait forever
  for (;;)
    sleep_us(60000);

  return 0;
}


#endif






#if xxx

bool state;
const uint INPUT_GPIO = 21;
const uint LED_PIN = PICO_DEFAULT_LED_PIN;

// Debounce control
unsigned long time = to_ms_since_boot(get_absolute_time());
const int delayTime = 50; // Delay for every push button may vary


void inter_test(uint gpio, uint32_t events) {
    if ((to_ms_since_boot(get_absolute_time())-time)>delayTime) {
        // Recommend to not to change the position of this line
        time = to_ms_since_boot(get_absolute_time());
        
        // Interrupt function lines
        state = !state;
        gpio_put(LED_PIN, state);
    }
}




int main() {
    stdio_init_all();

    // just to know when the board is on ... not related to debounce
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    state = true;
    gpio_put(LED_PIN, state);


    // interrupt
    gpio_init(INPUT_GPIO);
    //gpio_pull_up(INPUT_GPIO);

    gpio_set_irq_enabled_with_callback(INPUT_GPIO, GPIO_IRQ_EDGE_FALL , true, &inter_test);

    while(1) {}
}

#endif