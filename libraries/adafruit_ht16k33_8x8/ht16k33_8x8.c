#include <stdlib.h>
#include <string.h>

#include "ht16k33_8x8.h"

static const uint8_t OSC_CMD = 0x20;
static const uint8_t DISPLAY_CMD = 0x80;
static const uint8_t BRIGHTNESS_CMD = 0xE0;

struct ht16k33_inst {

  struct i2c_inst * i2c;
  int addr;
};

static uint8_t rotr8(uint8_t x, int n) {
  return ((x >> n) | (x << (8 - n))) & UINT8_MAX;
}

static void i2c_write_byte(struct ht16k33_inst * ht16k33, uint8_t val) {

  i2c_write_blocking(ht16k33 -> i2c, ht16k33 -> addr, & val, 1, false);
}

struct ht16k33_inst * ht16k33_init(struct i2c_inst * i2c, int addr) {

  struct ht16k33_inst * ht16k33 = malloc(sizeof(struct ht16k33_inst));
  memset(ht16k33, 0, sizeof(struct ht16k33_inst));
  if (ht16k33 == NULL) return NULL;
  ht16k33 -> i2c = i2c;
  ht16k33 -> addr = addr;

  ht16k33_operation_mode(ht16k33, Normal);
  ht16k33_clear_display(ht16k33);
  ht16k33_set_brightness(ht16k33, L0);
  ht16k33_display_mode(ht16k33, On);

  return ht16k33;
}

void ht16k33_operation_mode(struct ht16k33_inst * ht16k33, enum operation_mode m) {

  i2c_write_byte(ht16k33, OSC_CMD | m);
}

void ht16k33_display_mode(struct ht16k33_inst * ht16k33, enum display_mode m) {

  i2c_write_byte(ht16k33, DISPLAY_CMD | m);
}

void ht16k33_clear_display(struct ht16k33_inst * ht16k33) {

  uint8_t d[1 + 2 * 8];
  memset(d, 0x00, sizeof(d));
  i2c_write_blocking(ht16k33 -> i2c, ht16k33 -> addr, d, sizeof(d), false);
}

void ht16k33_write_display(struct ht16k33_inst * ht16k33, uint8_t * d) {

  uint8_t temp[1 + 2 * 8];
  memset(temp, 0x00, sizeof(temp));

  for (int i = 0, j = 1; i < 8; i++, j = j + 2)
    temp[j] = rotr8(d[i], 1);

  i2c_write_blocking(ht16k33 -> i2c, ht16k33 -> addr, temp, sizeof(temp), false);
}

void ht16k33_blink_rate(struct ht16k33_inst * ht16k33, enum blink b) {

  const enum display_mode m = On;
  i2c_write_byte(ht16k33, DISPLAY_CMD | m | (b << 1));
}

void ht16k33_set_brightness(struct ht16k33_inst * ht16k33, enum brightness b) {

  i2c_write_byte(ht16k33, BRIGHTNESS_CMD | b);
}

void ht16k33_deinit(struct ht16k33_inst * ht16k33) {

  if (ht16k33 != NULL) {
    memset(ht16k33, 0, sizeof(struct ht16k33_inst));
    free(ht16k33);
  }
}