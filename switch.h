#include "Arduino.h"

template<uint8_t Input, bool enable_pull_up = true>
class DebouncedSwitch {
 public:
  DebouncedSwitch() { }
    
  static inline void Init() {
//    Input::set_mode(DIGITAL_INPUT);
    pinMode(Input, INPUT);
    if (enable_pull_up) {
//      Input::High();
      digitalWrite(Input, HIGH);
    }
    state_ = 0xff;
  }

  // To be called at a rate < 1000 Hz.
  static inline uint8_t Read() {
    state_ = (state_ << 1) | digitalRead(Input);
    return state_;
  }

  static inline uint8_t lowered() { return state_ == 0x80; }
  static inline uint8_t raised() { return state_ == 0x7f; }
  static inline uint8_t high() { return state_ == 0xff; }
  static inline uint8_t low() { return state_ == 0x00; }
  static inline uint8_t state() { return state_; }
  static inline uint8_t immediate_value() {  digitalRead(Input); }

 private:
  static uint8_t state_;

//  DISALLOW_COPY_AND_ASSIGN(DebouncedSwitch);
};

/* static */
template<uint8_t Input, bool enable_pull_up>
uint8_t DebouncedSwitch<Input, enable_pull_up>::state_;


