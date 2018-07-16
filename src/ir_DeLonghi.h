#ifndef IR_DELONGHI_H_
#define IR_DELONGHI_H_

#ifndef UNIT_TEST
#include <Arduino.h>
#else
#include <string>
#endif
#include "IRremoteESP8266.h"
#include "IRsend.h"

#define DELONGHI_MODE_AUTO                     0U
#define DELONGHI_MODE_COOL                     1U
#define DELONGHI_MODE_DRY                      2U
#define DELONGHI_MODE_FAN                      3U
#define DELONGHI_MODE_HEAT                     4U
#define DELONGHI_TEMP_MIN                     16U  // 16C
#define DELONGHI_TEMP_MAX                     30U  // 30C
#define DELONGHI_TEMP_AUTO                    25U  // 25C

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
    b5 = I Feel(requires follow-up payload which is repeated every 10 minutes)
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
  void begin();

#if SEND_DELONGHI
  void send();
#endif  // SEND_DELONGHI

  void setMode(uint8_t mode);
  uint8_t getMode();
  void setPower(bool state);
  bool getPower();
  void on();
  void off();
  void setFan(uint8_t fan);
  uint8_t getFan();
  void setSwingVertical(uint8_t state);
  bool getSwingVertical();
  void setTemp(uint8_t temp);
  uint8_t getTemp();
  void setTurbo(bool state);
  bool getTurbo();
  void setLight(bool state);
  bool getLight();

  uint64_t* getRaw();
  String getRawBinary();
  void setRaw(uint64_t new_code[]);
#ifdef ARDUINO
  String toString();
#else
  std::string toString();
#endif

private:
  // The state of the IR remote in IR code form.
  uint64_t remote_state_A;
  uint64_t remote_state_B;

  void checksum();

  IRsend _irsend;
};

#endif  // IR_DELONGHI_H_
