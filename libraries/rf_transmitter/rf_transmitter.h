#ifndef _RF_TRANSMITTER_H
#define _RF_TRANSMITTER_H

#ifdef __cplusplus
extern "C" {
  #endif

  struct rft_inst;
  struct rft_inst * rft_init(uint outputPin, uint8_t nodeId, uint16_t pulseLength, uint16_t backoffDelay, uint8_t resendCount);

  void rft_send(struct rft_inst * rft, uint8_t * data, uint8_t len);
  void rft_resend(struct rft_inst * rft, uint8_t * data, uint8_t len);
  void rft_deinit(struct rft_inst * rft);

  #ifdef __cplusplus
}
#endif

#endif