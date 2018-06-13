#include "ir_DeLonghi.h"

#if SEND_DELONGHI
// Send a DeLonghi A/C message.
//
// Args:
//   data: An array of bytes containing the IR command.
//   nbytes: Nr. of bytes of data in the array. (>=DELONGHI_STATE_LENGTH)
void IRsend::sendDeLonghi(unsigned char data[], uint16_t nbytes,
                            uint16_t repeat) {

}
#endif  // SEND_DELONGHI

IRDeLonghiAC::IRDeLonghiAC(uint16_t pin) : _irsend(pin) {
  stateReset();
}

void IRDeLonghiAC::stateReset() {
  for (uint8_t i = 0; i < DELONGHI_STATE_LENGTH; i++)
    remote_state[i] = 0x0;
}

#if SEND_DELONGHI
void IRDeLonghiAC::send() {
  _irsend.sendDeLonghi(remote_state);
}
#endif  // SEND_DELONGHI
