/*
  This code is port and based on great work of Andreas Rohner
  See: https://github.com/zeitgeist87/RFTransmitter

  MIT License

  Copyright (c) 2021 Jiri Binko

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include <stdlib.h>
#include <string.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "rf_transmitter.h"

static const uint8_t MAX_PAYLOAD_SIZE = 80;

struct rft_inst {

  uint outputPin;
  uint8_t lineState;
  uint16_t pulseLength; // Pulse lenght in microseconds
  uint8_t nodeId;
  uint16_t backoffDelay; // Backoff period for repeated sends in milliseconds
  uint8_t resendCount; // How often a reliable package is resent
  uint8_t packageId;
};

static inline int32_t random_arduino(int32_t min, int32_t max) {
  return (rand() % (max - min + 1)) + min;
}

static void send0(struct rft_inst * rft) {
  rft -> lineState = !rft -> lineState;
  gpio_put(rft -> outputPin, rft -> lineState);
  sleep_us(rft -> pulseLength << 1);
}

static void send1(struct rft_inst * rft) {
  gpio_put(rft -> outputPin, !rft -> lineState);
  sleep_us(rft -> pulseLength);
  gpio_put(rft -> outputPin, rft -> lineState);
  sleep_us(rft -> pulseLength);
}

static inline void send00(struct rft_inst * rft) {
  send0(rft);
  sleep_us(rft -> pulseLength << 1);
}

static inline void send01(struct rft_inst * rft) {
  send1(rft);
  sleep_us(rft -> pulseLength << 1);
}

static inline void send10(struct rft_inst * rft) {
  send1(rft);
  send0(rft);
}

static inline void send11(struct rft_inst * rft) {
  send1(rft);
  send1(rft);
}

static void sendByte(struct rft_inst * rft, uint8_t data) {
  uint8_t i = 4;
  do {
    switch (data & 3) {
    case 0:
      send00(rft);
      break;
    case 1:
      send01(rft);
      break;
    case 2:
      send10(rft);
      break;
    case 3:
      send11(rft);
      break;
    }
    data >>= 2;
  } while (--i);
}

static inline void sendByteRed(struct rft_inst * rft, uint8_t data) {
  sendByte(rft, data);
  sendByte(rft, data);
  sendByte(rft, data);
}

static inline uint16_t crc_update(uint16_t crc, uint8_t data) {
  data ^= crc & 0xFF;
  data ^= data << 4;

  return ((((uint16_t) data << 8) | (crc >> 8)) ^ (uint8_t)(data >> 4) ^
    ((uint16_t) data << 3));
}

static void sendPackage(struct rft_inst * rft, uint8_t * data, uint8_t len) {
  // Synchronize receiver
  sendByte(rft, 0x00);
  sendByte(rft, 0x00);
  sendByte(rft, 0xE0);

  // Add from, id and crc to the message
  uint8_t packageLen = len + 4;
  sendByteRed(rft, packageLen);

  uint16_t crc = 0xffff;
  crc = crc_update(crc, packageLen);

  for (uint8_t i = 0; i < len; ++i) {
    sendByteRed(rft, data[i]);
    crc = crc_update(crc, data[i]);
  }

  sendByteRed(rft, rft -> nodeId);
  crc = crc_update(crc, rft -> nodeId);

  sendByteRed(rft, rft -> packageId);
  crc = crc_update(crc, rft -> packageId);

  sendByteRed(rft, crc & 0xFF);
  sendByteRed(rft, crc >> 8);

  gpio_put(rft -> outputPin, 0);
  rft -> lineState = 0;
}

struct rft_inst * rft_init(uint outputPin, uint8_t nodeId, uint16_t pulseLength, uint16_t backoffDelay, uint8_t resendCount) {

  struct rft_inst * rft = malloc(sizeof(struct rft_inst));
  memset(rft, 0, sizeof(struct rft_inst));
  if (rft == NULL) return NULL;

  rft -> outputPin = outputPin;
  rft -> nodeId = nodeId;
  rft -> pulseLength = pulseLength;
  rft -> backoffDelay = backoffDelay;
  rft -> resendCount = resendCount;
  rft -> packageId = 0;

  gpio_init(rft -> outputPin);
  gpio_set_dir(rft -> outputPin, GPIO_OUT);
  gpio_put(rft -> outputPin, 0);
  rft -> lineState = 0;

  return rft;
}

void rft_send(struct rft_inst * rft, uint8_t * data, uint8_t len) {

  rft -> packageId = rft -> packageId + 1;
  rft_resend(rft, data, len);
}

void rft_resend(struct rft_inst * rft, uint8_t * data, uint8_t len) {

  if (len > MAX_PAYLOAD_SIZE)
    len = MAX_PAYLOAD_SIZE;

  sendPackage(rft, data, len);

  for (uint8_t i = 0; i < rft -> resendCount; ++i) {
    sleep_ms(random_arduino(rft -> backoffDelay, rft -> backoffDelay << 1));
    sendPackage(rft, data, len);
  }
}

void rft_deinit(struct rft_inst * rft) {

  if (rft != NULL) {
    memset(rft, 0, sizeof(struct rft_inst));
    free(rft);
  }
}