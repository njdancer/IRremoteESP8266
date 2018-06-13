#include "IRremoteESP8266.h"
#include "IRsend.h"

class IRDeLonghiAC {
public:
  IRDeLonghiAC (uint16_t pin);

  void stateReset();
#if SEND_KELVINATOR
  void send();
#endif  // SEND_KELVINATOR

private:
   // The state of the IR remote in IR code form.
   uint8_t remote_state[DELONGHI_STATE_LENGTH];

  IRsend _irsend;
};
