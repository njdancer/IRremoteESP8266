#include "IRremoteESP8266.h"
#include "IRsend.h"

class IRDeLonghiAC {
public:
  explicit IRDeLonghiAC(uint16_t pin);

  void stateReset();
#if SEND_KELVINATOR
  void send();
#endif  // SEND_KELVINATOR
  void begin();
  void on();
  void off();
  void setPower(bool state);
  bool getPower();

private:
  // The state of the IR remote in IR code form.
  uint64_t remote_state_A;
  uint64_t remote_state_B;

  IRsend _irsend;
};
