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
#include <RollingAverage.h>
#include <CapacitiveSensor.h>

// use: Oscil <table_size, update_rate> oscilName (wavetable), look in .h file of table #included above
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSin(SIN2048_DATA);

CapacitiveSensor   cs_3_4 = CapacitiveSensor(3,4); 

// use #define for CONTROL_RATE, not a constant
#define CONTROL_RATE 64 // powers of 2 please

const int KNOB_PIN = 0; 

const int MIN_IN = 0;
const int MAX_IN = 1023;

//const int MIN_C = 600;
//const int MAX_C = 1200;

const int MIN_C = -2000;
const int MAX_C = 1700;

const int MIN_F = 400;
const int MAX_F = 1200;

AutoMap kMapF(MIN_IN,MAX_IN,MIN_F,MAX_F);
AutoMap kMapC(MIN_C,MAX_C,MIN_F,MAX_F);

RollingAverage <int, 16> FAverage; 
RollingAverage <int, 64> CapAverage; 

void setup(){
  startMozzi(CONTROL_RATE); // set a control rate of 64 (powers of 2 please)
  //aSin.setFreq(440); // set the frequency
  Serial.begin(115200);
}


void updateControl(){
  
int fundamental = mozziAnalogRead(KNOB_PIN)+1;
//fundamental = FAverage.next(fundamental);
fundamental = kMapF(fundamental);
Serial.print(fundamental);
Serial.print("  ");

long capsense1_ =  cs_3_4.capacitiveSensor(1);
int capsense1 = (int) capsense1_;
capsense1 = CapAverage.next(capsense1);
capsense1 = kMapC(capsense1);
//Serial.print(capsense1_);
//Serial.print("  ");
Serial.print(capsense1);
Serial.println("  ");
  
  //aSin.setFreq(fundamental); // set the frequency
  aSin.setFreq(capsense1); // set the frequency
    float R = aSin.next();
//    Serial.print(R);
//    Serial.println("  ");
}


int updateAudio(){
  return aSin.next(); // return an int signal centred around 0
}


void loop(){
  audioHook(); // required here
}



