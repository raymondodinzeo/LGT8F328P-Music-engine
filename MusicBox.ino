[code]
//============================================
//MusicBox By Ray Houghton
//A rough and ready tune playing engine based loosely
//on the example programme dac0_ramp.
//Licence GPLv3
//Program sets up samples and notes are produced by varying the increase in amplitude per sample. 
//A higher increase per sample results in a higher frequency.
//Written for LGT8F328P, may work on an ESP32. Will not work on a standard arduino. 
  
// Larduino w/ 328D
// DAC0 output
// DACO output ==> D4 on board
//NOTES (Sample[50])
//1 C2; 2 C#2; 3 D2; 4 D#2; 5 E2; 6 F2; 7 F#2; 8 G2; 9 G#2; 10 A2; 11 A#2
//12 B2; 13 C3; 14 C#3; 15 D3; 16 D#3; 17 E3; 18; F3; 19 F#3; 20 G3; 21 G#3;
//22 A3; 23 A#3; 24 B3; 25 C4(Mid); 26 C#4; 27 D4; 28 D#4; 29 E4; 30 F4; 
//31 F#4; 32 G4; 33 G#4; 34 A4; 35 A#4 36 B4; 37 C5; 38 C#5; 39 D5; 40 D#5;
//41 E5; 42 F5; 43 F#5; 44; G5; 45 G#5; 46 A5; 47 A#5; 48 B5; 49 C6    
//============================================
 #include <avr/power.h> //Allows 32MHz
unsigned long previousMicros = 0;  //initial value of micros
unsigned long currentMicros; //current value of micros
const unsigned long UperiodConst = 23; //number of microseconds per sample
//Sample rate 43478.26 samples per second 
unsigned long previousMillis = 0;  //initial value of millis
unsigned long currentMillis; //current value of millis
float sampleVal; //Increase in amplitude per sample
float value = 0; //Actual output amplitude (0 to 255)
byte index = 0; //Note count
const float sample[50] PROGMEM = {0.38511, 0.40801, 0.43227, 0.45798, 0.48521,
0.51406, 0.54463, 0.57702, 0.61133, 0.64768, 0.68619, 0.72700, 0.77023, 0.81603,
0.86455, 0.91596, 1.97042, 1.02813, 1.08926, 1.15403, 1.22657, 1.29536,
1.37239, 1.45399, 1.54045, 1.63205, 1.72910, 1.83192, 1.90485, 2.05626, 2.17857,
2.30807, 2.44531, 2.59072, 2.74477, 2.90798, 3.08090, 3.26410, 3.45820, 3.66383,
3.88169, 4.11251, 4.35705, 4.61614, 4.89063, 5.18144, 5.48954, 5.81597, 6.16181}; 
//Array containing increase in amplitude per sample for each note.
//const float sample[32] PROGMEM = {0.82021, 0.86700, 0.91407, 0.97506, 0.97000, 1.07532, 1.13838, 1.20813, 1.27997,1.35608,
//1.43258, 1.51787, 1.613088, 1.71115, 1.80851, 1.91728, 2.03182, 2.15999, 2.27679,
//2.41626, 2.55994, 2.71276, 2.87343, 3.04320, 3.22786, 3.41711, 3.6428603, 3.86299,
//4.06363, 4.3220317}; 
byte wewish[106] = {100, 25, 4, /*BAR*/ 30, 4, 30, 2, 32, 2, 30, 2, 29, 2, /*BAR*/ 27, 4, 27, 4,
27, 4,  /*BAR*/ 32, 4, 32, 2, 34, 2, 32, 2, 30, 2,  /*BAR*/29, 4, 25, 4, 25, 4,  /*BAR*/
34, 4, 34, 2, 35, 2, 34, 2, 32, 2,  /*BAR*/30, 4, 27, 4, 25, 2, 25, 2,  /*BAR*/27, 4, 32, 4, 
29, 4, /*BAR*/ 30, 8, 25, 4,  /*BAR*/30, 4, 30, 4, 30, 4,  /*BAR*/29, 8, 29, 4,  /*BAR*/
30, 4, 29, 4, 27, 4,  /*BAR*/ 25, 8, 25, 4,  /*BAR*/34, 4, 32, 4, 30, 4,  /*BAR*/37, 4, 25, 4, 
25, 2, 25, 2,  /*BAR*/27, 4, 32, 4, 29, 4,  /*BAR*/ 30, 8};
//Array containing tune. Tempo (first element) along with note number and duration.
//Semi quaver = 1  quaver = 2 crotchet = 4 etc. 

void setup() 
{
  clock_prescale_set(clock_div_1); //Allows 32MHz
  analogReference(INTERNAL2V56); //Example code ??
  pinMode(4, ANALOG); //DAC output
  analogWrite(4, 255); //Example code ??
}

void loop() 
{
  currentMillis = millis(); //Initialise time
  currentMicros = micros();
   
  if (currentMicros - previousMicros >= UperiodConst) //Sets sample rate
  {
    previousMicros = currentMicros; //Resets micros for next sample
    if(index != 0)
    {
    sampleVal = pgm_read_float(&sample[wewish[index - 1]]); 
    }
    //reads note from wewish to index sample array from PROGMEM to produce correct note.
    analogWrite(4, int(value)); //Writes samples to DAC as ints
    value += sampleVal; //Builds sawtooth waveform
    
    if(value >= 255) //Resets sawtooth to zero every repeat
    {
      value = 0; 
    }
    
  }
 if (currentMillis - previousMillis >= wewish[0] * wewish[index]) //Sets note duration
 {
  analogWrite(4, 0); //Turns off output for between notes
  delay(100); //Note off duration (between notes
  index = index + 2; //reads next note from wewish array.
  previousMillis = currentMillis; //reset timer for next note
  if(index >= sizeof(wewish)) //Stops at end of array and returns to beginning
  {
    index = 2;
  }
 }
 
}
//[/code]
