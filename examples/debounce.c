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
