/*  Example playing a sinewave at a set frequency,
    using Mozzi sonification library.
  
    Demonstrates the use of Oscil to play a wavetable.
  
    Circuit: Audio output on digital pin 9 on a Uno or similar, or
    DAC/A14 on Teensy 3.1, or 
    check the README or http://sensorium.github.com/Mozzi/
  
    Mozzi help/discussion/announcements:
    https://groups.google.com/forum/#!forum/mozzi-users
  
    Tim Barrass 2012, CC by-nc-sa.
*/

#include <MozziGuts.h>
#include <Oscil.h> // oscillator template
#include <tables/sin2048_int8.h> // sine table for oscillator
#include <AutoMap.h>

// use: Oscil <table_size, update_rate> oscilName (wavetable), look in .h file of table #included above
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSin(SIN2048_DATA);

// use #define for CONTROL_RATE, not a constant
#define CONTROL_RATE 64 // powers of 2 please

const int KNOB_PIN = 0; 

const int MIN_IN = 0;
const int MAX_IN = 1023;

const int MIN_F = 400;
const int MAX_F = 1200;

AutoMap kMapF(MIN_IN,MAX_IN,MIN_F,MAX_F);

void setup(){
  startMozzi(CONTROL_RATE); // set a control rate of 64 (powers of 2 please)
  //aSin.setFreq(440); // set the frequency
  Serial.begin(115200);
}


void updateControl(){
  // put changing controls in here
  int fundamental = mozziAnalogRead(KNOB_PIN)+1;
  //fundamental = FAverage.next(fundamental);
//  Serial.print(fundamental);
//    Serial.print("  ");
  fundamental = kMapF(fundamental);
//  Serial.print(fundamental);
//    Serial.print("  ");
  aSin.setFreq(fundamental); // set the frequency
  //aSin.setFreq(440); // set the frequency
    float R = aSin.next();
    Serial.print(R);
    Serial.println("  ");
}


int updateAudio(){
  return aSin.next(); // return an int signal centred around 0
}


void loop(){
  audioHook(); // required here
}



