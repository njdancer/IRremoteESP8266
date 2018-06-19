#include "ir_DeLonghi.h"

#define DELONGHI_HDR_MARK                   8960U
#define DELONGHI_HDR_SPACE                  4488U
#define DELONGHI_BIT_MARK_TICKS                8U
#define DELONGHI_BIT_MARK                    634U
#define DELONGHI_ONE_SPACE                  1660U
#define DELONGHI_ZERO_SPACE                  566U
#define DELONGHI_INTER_SPACE               19962U
#define DELONGHI_GAP                      100000U

#define DELONGHI_POWER_OFFSET_1                3U
#define DELONGHI_POWER_OFFSET_2               22U
#define DELONGHI_MODE_MASK                   7ULL
#define DELONGHI_TEMP_OFFSET                 8ULL
#define DELONGHI_TEMP_MASK                 0xFULL << DELONGHI_TEMP_OFFSET

#define DELONGHI_CHECKSUM_OFFSET              28U
#define DELONGHI_CHECKSUM_MASK             0xFULL << DELONGHI_CHECKSUM_OFFSET


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

void IRDeLonghiAC::stateReset() {
  remote_state_A = 0;
  // set display to be on
  remote_state_A |= 1ULL << 21;
  // unsure what these bits do but the remote doesn't work without them
  remote_state_A |= 1ULL << 28;
  remote_state_A |= 1ULL << 30;
  remote_state_A |= 1ULL << 33;

  remote_state_B = 0;
}

void IRDeLonghiAC::begin() {
  _irsend.begin();
}

#if SEND_DELONGHI
void IRDeLonghiAC::send() {
  checksum();
  _irsend.sendDeLonghi(remote_state_A, remote_state_B);
}
#endif  // SEND_DELONGHI

void IRDeLonghiAC::setMode(uint8_t mode) {
  remote_state_A &= ~DELONGHI_MODE_MASK;
  // If we get an unexpected mode, default to AUTO.
  if (mode > DELONGHI_MODE_HEAT) mode = DELONGHI_MODE_AUTO;
  remote_state_A |= mode;
}

uint8_t IRDeLonghiAC::getMode() {
  return (remote_state_A & DELONGHI_MODE_MASK);
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

void IRDeLonghiAC::on() {
  remote_state_A |= 1ULL << DELONGHI_POWER_OFFSET_1;
  remote_state_A |= 1ULL << DELONGHI_POWER_OFFSET_2;
}

void IRDeLonghiAC::off() {
  remote_state_A &= ~(1ULL << DELONGHI_POWER_OFFSET_1);
  remote_state_A &= ~(1ULL << DELONGHI_POWER_OFFSET_2);
}

void IRDeLonghiAC::setTemp(uint8_t temp) {
  temp = std::max((uint8_t) DELONGHI_TEMP_MIN, temp);
  temp = std::min((uint8_t) DELONGHI_TEMP_MAX, temp);
  remote_state_A |= (temp - DELONGHI_TEMP_MIN) << DELONGHI_TEMP_OFFSET;
}

uint8_t IRDeLonghiAC::getTemp() {
  return ((remote_state_A & DELONGHI_TEMP_MASK) >> DELONGHI_TEMP_OFFSET) + DELONGHI_TEMP_MIN;
}

void IRDeLonghiAC::checksum() {
  uint64_t mask = 0xF;
  uint8_t data1 = remote_state_A & mask;
  uint8_t data2 = (remote_state_A >> DELONGHI_TEMP_OFFSET) & mask;

  uint8_t sum = data1 + data2 - 6;
  // reset checksum
  remote_state_B &= ~DELONGHI_CHECKSUM_MASK;
  remote_state_B |= (sum & mask) << DELONGHI_CHECKSUM_OFFSET;
}
