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


//#define AUDIO_MODE HIFI

// use: Oscil <table_size, update_rate> oscilName (wavetable), look in .h file of table #included above
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSin(SIN2048_DATA);

CapacitiveSensor   cs_3_4 = CapacitiveSensor(4,2); 
//CapacitiveSensor   cs_5_6 = CapacitiveSensor(4,6);


// use #define for CONTROL_RATE, not a constant
#define CONTROL_RATE 64 // powers of 2 please
#define NUM_SAMPLES 1

int sensVal;           // for raw sensor values 
float filterVal = 0.0001;       // this determines smoothness  - .0001 is max  1 is off (no smoothing)
float smoothedVal1;     // this holds the last loop value just use a unique variable for every different sensor that needs smoothing
float smoothedVal2; 

int i, j;              // loop counters or demo    

const int KNOB_PIN = 0; 

const int MIN_IN = 0;
const int MAX_IN = 1023;

const int MIN_C = -100;
const int MAX_C = 100;
 
//const int MIN_C = -2000;
//const int MAX_C = 1700;

const int MIN_F = 300;
const int MAX_F = 600;

int capsense1;
int pastcapsense1;

AutoMap kMapF(MIN_IN,MAX_IN,MIN_F,MAX_F);
AutoMap kMapC(MIN_C,MAX_C,MIN_F,MAX_F);

RollingAverage <int, 512> FAverage; 
RollingAverage <int,512> CapAverage;
RollingAverage <int, 512> Cap2Average; 

void setup(){
  startMozzi(CONTROL_RATE); // set a control rate of 64 (powers of 2 please)
  //aSin.setFreq(440); // set the frequency
  Serial.begin(115200);
cs_3_4.set_CS_AutocaL_Millis(0xFFFFFFFF);
//cs_5_6.set_CS_AutocaL_Millis(0xFFFFFFFF);
}


void updateControl(){
  
int fundamental = mozziAnalogRead(KNOB_PIN)+1;
//fundamental = FAverage.next(fundamental);
fundamental = kMapF(fundamental);
//Serial.print(fundamental);
//Serial.print("  ");

//pastcapsense1 = capsense1;

Serial.print(pastcapsense1);
Serial.print("  ");

long capsense1_ =  cs_3_4.capacitiveSensor(NUM_SAMPLES);
int capsense1 = (int) capsense1_;
capsense1 = CapAverage.next(capsense1);

Serial.print(capsense1);
Serial.print("  ");

//capsense1 = kMapC(smoothedVal1);
//capsense1 = kMapC(capsense1);
int diff1 = (capsense1 - pastcapsense1);
Serial.print(diff1*10);
Serial.print("  ");

pastcapsense1 = capsense1;

//Serial.print(pastcapsense1);
//Serial.print("  ");

//long capsense2_ =  cs_5_6.capacitiveSensor(NUM_SAMPLES);
//int capsense2 = (int) capsense2_;
//smoothedVal2 = Cap2Average.next(capsense2);
////capsense2 = kMapC(smoothedVal2);
//int diff2 = (smoothedVal2 - oldsmoothedVal2);
//int smoothedVal2 = oldsmoothedVal2;

//Serial.print(capsense1_);
//Serial.print("  ");
//Serial.print(capsense1_);
//Serial.print("  ");




//Serial.print(capsense2);
//Serial.print("  ");
  //
  //aSin.setFreq(fundamental); // set the frequency
  aSin.setFreq(capsense1); // set the frequency
//    float R = aSin.next();
//    Serial.print(R);
//    Serial.print("  ");
    
    Serial.println("  ");
}


int updateAudio(){
  return aSin.next(); // return an int signal centred around 0
}


int smooth(int data, float filterVal, float smoothedVal){


  if (filterVal > 1){      // check to make sure param's are within range
    filterVal = .99;
  }
  else if (filterVal <= 0){
    filterVal = 0;
  }

  smoothedVal = (data * (1 - filterVal)) + (smoothedVal  *  filterVal);

  return (int)smoothedVal;
}



void loop(){
  audioHook(); // required here
}



