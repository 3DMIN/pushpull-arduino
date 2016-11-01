#include "switch.h"

template<uint8_t A, uint8_t B, uint8_t Click1, uint8_t Click2>
class RotaryEncoder {
 public:
  typedef DebouncedSwitch<A> SwitchA;
  typedef DebouncedSwitch<B> SwitchB;
  typedef DebouncedSwitch<Click1> SwitchClick1;
  typedef DebouncedSwitch<Click2> SwitchClick2;
 
  RotaryEncoder() { }

  static void Init() {
    SwitchA::Init();
    SwitchB::Init();
    SwitchClick1::Init();
    SwitchClick2::Init();
  }

  static inline int8_t Read() {
    ReadSwitch1();
    ReadSwitch2();
    return ReadEncoder();
  }
  
  static inline int8_t ReadEncoder() {
    int8_t increment = 0;
    uint8_t a = SwitchA::Read();
    uint8_t b = SwitchB::Read();
    if (a == 0x80 && ((b & 0xf0) == 0x00)) {
        increment = 1;
    } else {
      if (b == 0x80 && ((a & 0xf0) == 0x00)) {
        increment = -1;
      }
    }
    return increment;
  }
  
  static uint8_t ReadSwitch1() {
    return SwitchClick1::Read();
  }
  static uint8_t ReadSwitch2() {
    return SwitchClick2::Read();
  }

  static uint8_t clicked1() { 
    if(SwitchClick1::raised())
      return 1;
    else if(SwitchClick1::lowered())
      return 2;
      
    return 0; 
  }
  
  static uint8_t immediate_value1() { return SwitchClick1::immediate_value(); }
  
  static uint8_t clicked2() { 
    if(SwitchClick2::raised())
      return 1;
    else if(SwitchClick2::lowered())
      return 2;
      
    return 0; 
  }
  
  static uint8_t immediate_value2() { return SwitchClick1::immediate_value2(); }

 private:
//  DISALLOW_COPY_AND_ASSIGN(RotaryEncoder);
};
