#include "ir_DeLonghi.h"

#define DELONGHI_HDR_MARK                    8960U
#define DELONGHI_HDR_SPACE                   4488U
#define DELONGHI_BIT_MARK_TICKS              8U
#define DELONGHI_BIT_MARK                    634U
#define DELONGHI_ONE_SPACE                   1660U
#define DELONGHI_ZERO_SPACE                  566U
#define DELONGHI_INTER_SPACE                   19962U
#define DELONGHI_GAP                         100000U

#define DELONGHI_POWER_OFFSET_1                3U
#define DELONGHI_POWER_OFFSET_2               22U
#define DELONGHI_MODE_OFFSET                   0U

#if SEND_DELONGHI
// Send a DeLonghi A/C message.
//
// Args:
//   data: An array of bytes containing the IR command.
//   nbytes: Nr. of bytes of data in the array. (>=DELONGHI_STATE_LENGTH)
void IRsend::sendDeLonghi(uint64_t data_A, uint64_t data_B,
                          uint16_t repeat) {
  for (uint16_t r = 0; r <= repeat; r++) {
    sendGeneric(DELONGHI_HDR_MARK, DELONGHI_HDR_SPACE,
                DELONGHI_BIT_MARK, DELONGHI_ONE_SPACE,
                DELONGHI_BIT_MARK, DELONGHI_ZERO_SPACE,
                0, 0,  // No Footer yet.
                data_A, 35, 38, false, 0, 50);

    sendGeneric(DELONGHI_BIT_MARK, DELONGHI_INTER_SPACE,
                DELONGHI_BIT_MARK, DELONGHI_ONE_SPACE,
                DELONGHI_BIT_MARK, DELONGHI_ZERO_SPACE,
                DELONGHI_BIT_MARK, DELONGHI_GAP,
                data_B, 32, 38, false, 0, 50);
  }
}
#endif  // SEND_DELONGHI

IRDeLonghiAC::IRDeLonghiAC(uint16_t pin) : _irsend(pin) {
  stateReset();
}

void IRDeLonghiAC::begin() {
  _irsend.begin();
}

void IRDeLonghiAC::stateReset() {
  remote_state_A = 0;
  remote_state_B = 0;
}

void IRDeLonghiAC::on() {
  remote_state_A |= 1ULL << DELONGHI_POWER_OFFSET_1;
  remote_state_A |= 1ULL << DELONGHI_POWER_OFFSET_2;
}

void IRDeLonghiAC::off() {
  remote_state_A &= ~(1ULL << DELONGHI_POWER_OFFSET_1);
  remote_state_A &= ~(1ULL << DELONGHI_POWER_OFFSET_2);
}

void IRDeLonghiAC::setPower(bool state) {
  if (state)
    on();
  else
    off();
}

bool IRDeLonghiAC::getPower() {
  return ((remote_state_A & (1ULL << DELONGHI_POWER_OFFSET_1)) != 0);
}

#if SEND_DELONGHI
void IRDeLonghiAC::send() {
  _irsend.sendDeLonghi(remote_state_A, remote_state_B);
}
#endif  // SEND_DELONGHI
