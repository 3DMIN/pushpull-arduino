#include "encoder.h"
#include "SimpleMessageSystem.h"

uint8_t sub_clock;

typedef struct{
  int8_t increment;
  uint8_t clickedA;
  uint8_t clickedB;
}EncoderEvent;

RotaryEncoder<5,6,9,10> encoder1; 
EncoderEvent encoder1_event;

RotaryEncoder<7,8,11,12> encoder2; 
EncoderEvent encoder2_event;

ISR(TIMER0_COMPA_vect){
 
  if(sub_clock & 0x1f){
    // should be ~1kHz
    encoder1_event.increment += encoder1.Read();
    if(encoder1.clicked1()){
      encoder1_event.clickedA = encoder1.clicked1();
    };
    if(encoder1.clicked2()){
      encoder1_event.clickedB = encoder1.clicked2();
    };
    
    
    encoder2_event.increment += encoder2.Read();
    if(encoder2.clicked1()){
      encoder2_event.clickedA = encoder2.clicked1();
    };
    if(encoder2.clicked2()){
      encoder2_event.clickedB = encoder2.clicked2();
    };
    
  }
  sub_clock++;
}

void setupSampleTimer(){
  // Arduino Uno, Nano
  // Timer0 for samplerate
  // prescaler of timer is set to 64 by wiring.c
  TCCR0B = (TCCR0B & 0xf8) | 0x04;
  // set mode of Timer 2 to CTC Capture
  TCCR0A = (TCCR0A & 0xfc) | 0x02;  
  // set Compare Match A value, can be altered for Samplerate reduction. Hooray Aliasing!
  // desired samplerate 16MHz/512 => 31250Hz
  // range of the sample rate is between (16MHz/(kTimerPrescaler*kOCRRegister)) => 250000Hz - 980Hz
  OCR0A = 7;      
  // enable Timer/Counter2 Output Compare Match A Interrupt Enable
  // triggers ISR(TIMER0_COMPA_vect) routine when TCNT0 = OCR0A, (TCNT0 = timer value) 
  TIMSK0 |= (1<<OCIE0A);
}

void setup(){
  Serial.begin(115200);

  setupSampleTimer();
  
  encoder1.Init();
  encoder2.Init();
}

void EncoderTask(){
  //send encoder 1
  if(encoder1_event.increment != 0){
    

    messageSendChar('a');
    messageSendInt(encoder1_event.increment);
    messageEnd();
     
    encoder1_event.increment = 0;
  }
  //send encoder 2
  if(encoder2_event.increment != 0){
    

    messageSendChar('b');
    messageSendInt(encoder2_event.increment);
    messageEnd();
     
    encoder2_event.increment = 0;
  }
  //////////////////////////////////////////////////////////////////////////////////
  //send switch1   (c1)
  if(encoder1_event.clickedA != 0) {
    
    messageSendChar('c');
    messageSendInt(encoder1_event.clickedA); // 1 is note on 2 is note off
    messageEnd(); 
 
    encoder1_event.clickedA = 0;
  }
  //send switch2   (d1)
  if(encoder1_event.clickedB != 0) {
  
    messageSendChar('d');
    messageSendInt(encoder1_event.clickedB);
    messageEnd(); 
 
    encoder1_event.clickedB = 0;
  }
  //send switch3   (e1)
  if(encoder2_event.clickedA != 0) {
  
    messageSendChar('e');
    messageSendInt(encoder2_event.clickedA);
    messageEnd(); 
 
    encoder2_event.clickedA = 0;
  }
  //send switch4   (f1)
  if(encoder2_event.clickedB != 0) {
  
    messageSendChar('f');
    messageSendInt(encoder2_event.clickedB);
    messageEnd(); 
 
    encoder2_event.clickedB = 0;
  }
}

void loop(){
   EncoderTask();
}
