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
#include <tables/sin8192_int8.h> // sine table for oscillator
#include <tables/uphasor256_uint8.h> // sine table for oscillator
#include <tables/waveshape_chebyshev_6th_256_int8.h> // sine table for oscillator
#include <AutoMap.h>
#include <RollingAverage.h>
#include <CapacitiveSensor.h>

//#define AUDIO_MODE HIFI

// use: Oscil <table_size, update_rate> oscilName (wavetable), look in .h file of table #included above
//Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSin(SIN2048_DATA);
Oscil <SIN8192_NUM_CELLS, AUDIO_RATE> aSin(SIN8192_DATA);
Oscil <SIN8192_NUM_CELLS, AUDIO_RATE> aModulator(SIN8192_DATA);
//Oscil <UPHASOR256_NUM_CELLS, AUDIO_RATE> aSin(UPHASOR256_DATA);
//Oscil <CHEBYSHEV_6TH_256_NUM_CELLS, AUDIO_RATE> aSin(CHEBYSHEV_6TH_256_DATA);

CapacitiveSensor   cs_3_4 = CapacitiveSensor(3,4); 
CapacitiveSensor   cs_5_6 = CapacitiveSensor(5,6); 

// use #define for CONTROL_RATE, not a constant
#define CONTROL_RATE 64 // powers of 2 please
#define NUM_SAMPLES 3 //

// control variable, use the smallest data size you can for anything used in audio
byte gain = 150;

int mod_ratio = 2; // harmonics
long fm_intensity; // carries control info from updateControl() to updateAudio()

const int KNOB_PIN = 0; 

const int MIN_IN = 0;
const int MAX_IN = 1023;

const int MIN_C = 0;
const int MAX_C = 800;

//const int MIN_C = -2000;
//const int MAX_C = 1700;

const int MIN_F = 200;
const int MAX_F = 500;

//const int MIN_F = 1000;
//const int MAX_F = 200;

const int MIN_C2 = 0;
const int MAX_C2 = 400;

const int MIN_F2 = 10;
const int MAX_F2 = 200;

AutoMap kMapF(MIN_IN,MAX_IN,MIN_F,MAX_F);
AutoMap kMapC(MIN_C,MAX_C,MIN_F,MAX_F);
AutoMap kMapC2(MIN_C2,MAX_C2,MIN_F2,MAX_F2);

RollingAverage <int, 16> FAverage; 
RollingAverage <int, 64> Cap1Average; 
RollingAverage <int, 64> Cap2Average; 

void setup(){
  startMozzi(CONTROL_RATE); // set a control rate of 64 (powers of 2 please)
  //aSin.setFreq(440); // set the frequency
  Serial.begin(115200);
  cs_3_4.set_CS_AutocaL_Millis(0xFFFFFFFF);
  cs_5_6.set_CS_AutocaL_Millis(0xFFFFFFFF);
}


void updateControl(){
  
int fundamental = mozziAnalogRead(KNOB_PIN)+1;
//fundamental = FAverage.next(fundamental);
fundamental = kMapF(fundamental);

//gain = gain - 3 ; 

//Serial.print(fundamental);
//Serial.print("  ");



//Serial.print(capsense1_);
//Serial.print("  ");
//Serial.print(capsense1_);
//Serial.print("  ");

//long capsense1_ =  cs_3_4.capacitiveSensor(NUM_SAMPLES);
//int capsense1 = Cap1Average.next((int) capsense1_);
//capsense1 = kMapC(capsense1);
int capsense1 = Cap1Average.next((int) ((long) cs_3_4.capacitiveSensor(NUM_SAMPLES)));
//capsense1 = kMapC(capsense1);

int capsense2 = Cap2Average.next((int) ((long) cs_5_6.capacitiveSensor(NUM_SAMPLES)));
//capsense2 = kMapC(capsense2);

//gain = capsense2;

Serial.print(capsense1);
Serial.print("  ");

Serial.print(capsense2);
Serial.print("  ");

//Serial.print(gain);
//Serial.print("  ");

aSin.setFreq(capsense1); // set the frequency

int mod_freq = capsense1 * mod_ratio;

aModulator.setFreq(mod_freq);

fm_intensity = capsense2;

//fm_intensity = 10;
  
//    float R = aSin.next();
//    Serial.print(R);
//    Serial.print("  ");
    
    Serial.println("  ");
}


int updateAudio(){
//  return aSin.next(); // return an int signal centred around 0
  //return (aSin.next() * gain)>>8; //

  long modulation = fm_intensity * aModulator.next(); 
  return (aSin.phMod(modulation)* gain)>>8; // phMod does the FM
}


void loop(){
  audioHook(); // required here
}



