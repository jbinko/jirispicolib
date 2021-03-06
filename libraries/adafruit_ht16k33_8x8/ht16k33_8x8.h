/*
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

#ifndef _HT16K33_8x8_H
#define _HT16K33_8x8_H

#include "hardware/i2c.h"

#ifdef __cplusplus
extern "C" {
  #endif

  enum operation_mode {
    Standby = 0,
      Normal = 1,
  };

  enum display_mode {
    Off = 0,
      On = 1,
  };

  enum blink {
    Blink_Off = 0,
      Blink_2Hz = 1,
      Blink_1Hz = 2,
      Blink_HalfHz = 3,
  };

  enum brightness {
    L0 = 0,
      L1 = 1,
      L2 = 2,
      L3 = 3,
      L4 = 4,
      L5 = 5,
      L6 = 6,
      L7 = 7,
      L8 = 8,
      L9 = 9,
      L10 = 10,
      L11 = 11,
      L12 = 12,
      L13 = 13,
      L14 = 14,
      L15 = 15,
  };

  struct ht16k33_inst;
  struct ht16k33_inst * ht16k33_init(struct i2c_inst * i2c, int addr);
  void ht16k33_operation_mode(struct ht16k33_inst * ht16k33, enum operation_mode m);
  void ht16k33_display_mode(struct ht16k33_inst * ht16k33, enum display_mode m);
  void ht16k33_clear_display(struct ht16k33_inst * ht16k33);
  void ht16k33_write_display(struct ht16k33_inst * ht16k33, uint8_t * d);
  void ht16k33_blink_rate(struct ht16k33_inst * ht16k33, enum blink b);
  void ht16k33_set_brightness(struct ht16k33_inst * ht16k33, enum brightness b);
  void ht16k33_deinit(struct ht16k33_inst * ht16k33);

  #ifdef __cplusplus
}
#endif

#endif