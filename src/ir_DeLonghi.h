#include "IRremoteESP8266.h"
#include "IRsend.h"

/*
	DeLonghi AC map

  (header mark and space)
  byte 0 = Basic Modes
    b2-0 = Modes
      000 (0) = Auto (temp = 25C)
      001 (1) = Cool
      010 (2) = Dry (temp = 25C, but not shown)
      011 (3) = Fan
      100 (4) = Heat
    b3 = Power Status (1 = On, 0 = Off)[duplicated at byte 2 b6]
    b5-4 = Fan (Basic modes)
      00 (0) = Auto
      01 (1) = Fan 1
      10 (2) = Fan 2
      11 (3) = Fan 3 or higher (See byte 14)
    b6 = Vent swing (1 = On, 0 = Off) (See byte 4)
    b7 = Sleep Modes 1 & 3 (1 = On, 0 = Off)
  byte 1 = Temperature
    b3-0 = Degrees C.
      0000 (0) = 16C
      0001 (1) = 17C
      0010 (2) = 18C
      ...
      1101 (13) = 29C
      1110 (14) = 30C
  byte 2 = Extras
    b3-0 = UNKNOWN, typically 0
    b4 = Turbo Fan (1 = On, 0 = Off)
    b5 = Light (Display) (1 = On, 0 = Off)
    b6 = Power Status (1 = On, 0 = Off)[duplicated at byte 0 b3]
    b7 = UNKNOWN, typically 0
  byte 3 = UNKNOWN, typically 0b01010000
  byte 4
    b2-0 = UNKNOWN, typically 0b010
    (intermission mark and space)
    b6-3 = Swing Vent Vertical
      0000 (0) = No swing
      0001 (1) = Full Swing
      0010 (2) = No swing - Top
      0011 (3) = No swing - Top Middle
      0100 (4) = No swing - Middle
      0101 (5) = No swing - Middle Bottom
      0110 (6) = No swing - Bottom
      0111 (7) = Swing - Bottom
      1001 (9) = Swing - Middle
      1011 (11) = Swing - Top
    b7 = UNKNOWN, typically 0
  byte 5
    b2-0 = UNKNOWN, typically 0
    b4-3 = Temp display
      00 (0) = Off
      01 (1) = Setting Temp
      10 (2) = Indoor Ambient Temp
      11 (3) = Outdoor Ambient Temp
    b5 = I Feel(requires follow-up payload)
  byte 6 = UNKNOWN, typically 0
  byte 7
    b6-0 = UNKNOWN, typically 0
    b7 = checksum (first bit of 4)
  byte 8
    b2-0 = checksum (last 3 bits of 4)
  (footer mark and space)

  Checksum is the sum of the first 4 bits of byte 0 and byte 1 minus 6.
  Overflow can be ignored.
*/

class IRDeLonghiAC {
public:
  explicit IRDeLonghiAC(uint16_t pin);

  void stateReset();
#if SEND_DELONGHI
  void send();
#endif  // SEND_DELONGHI
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
