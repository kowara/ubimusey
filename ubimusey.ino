/*************************************************
*  Ubimusey: A portable music-making glove for on-the-go musicians
*  v1.0
*  
*  Designing Novel Interactions (INFO90003) Project
*  University of Melboure S1/2020
*  created by The Triad
*  
 *************************************************/

/*************************************************
 * Note Public Constants
 *************************************************/

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

/*************************************************
 * Libraries
 *************************************************/

// The Synth library: https://github.com/dzlonline/the_synth
#include <synth.h>
#include <tables.h>

// Wire library
#include <Wire.h>

// Adafruit MLX90614 IR Temperature Sensor library
#include <Adafruit_MLX90614.h>

// Sparkfun MMA8452Q Accelerometer library: http://librarymanager/All#SparkFun_MMA8452Q
#include "SparkFun_MMA8452Q.h"

/*************************************************
 * Instance Declarations
 *************************************************/

#define I2C_ADDR 0x27                                 //I2C adress, you should use the code to scan the adress first (0x27) here

Adafruit_MLX90614 mlx = Adafruit_MLX90614();          // create new MLX90614 IR temperature sensor instance
MMA8452Q accel;                                       // create new MMA8452 accelerometer instance
synth glove;                                          // create new The Synth instance

/*************************************************
 * Variable Declarations
 *************************************************/

int notes[] = {0,1,2,3,4,5,6};                        // array to store selected chord's notes
int note;                                             // int to store selected note's frequency
int i;                                                // int to store number from RNG
int finger0 = 0;                                      // int to store pressure data from CH0
int finger1 = 0;                                      // int to store pressure data from CH1
int finger2 = 0;                                      // int to store pressure data from CH2
float celsius;                                        // float to store IR temperature sensor input

/*************************************************
 * Initial Setup
 *************************************************/
 
void setup() {

  // begin Serial Monitor and Wire processes
  Serial.begin(9600);
  Wire.begin();
  
  mlx.begin();                                        //set-up IR temperature scanner

  // check for accelerometer data
  if (accel.begin() == false) {
    Serial.println("Not Connected. Please check connections and read the hookup guide.");
    while (1);
  }
  
  glove.begin(CHB);                                   //polyphonic synth output on D3
  glove.setupVoice(0,TRIANGLE,60,ENVELOPE1,127,64);   //set-up synth voice 0
  glove.setupVoice(1,TRIANGLE,60,ENVELOPE1,127,64);   //set-up synth voice 1
  glove.setupVoice(2,TRIANGLE,60,ENVELOPE1,127,64);   //set-up synth voice 2
  
  pinMode(A0, INPUT);                                  //thumb input on A0
  pinMode(A1, INPUT);                                  //thumb input on A1
  pinMode(A2, INPUT);                                  //thumb input on A2

  delay(1000);
}

/*************************************************
 * Temperature-to-Chord Picker
 *************************************************/

// select chord based on detected temperature
void ChordPicker() {
  
  // detect current temperature
  Serial.println(mlx.readObjectTempC());
  celsius = mlx.readObjectTempC();

  // select chord to play based on detected temperature range
  if (celsius <= 4) {
    note = CsMaj();
    Serial.println("picked CsMaj");
  }
  
  else if (celsius > 4 && celsius <= 8) {
    note = GsMaj();
    Serial.println("picked GsMaj");
  }
  
  else if (celsius > 8 && celsius <= 12) {
    note = DsMaj();
    Serial.println("picked DsMaj");
  }
  
  else if (celsius > 12 && celsius <= 17) {
    note = AsMaj();
    Serial.println("picked AsMaj");
  }
  
  else if (celsius > 17 && celsius <= 21) {
    note = FMaj();
    Serial.println("picked FMaj");
  }
  
  else if (celsius > 21 && celsius <= 25) {
    note = CMaj();
    Serial.println("picked CMaj");
  }
  
  else if (celsius > 25 && celsius <= 30) {
    note = GMaj();
    Serial.println("picked GMaj");
  }
  
  else if (celsius > 30 && celsius <= 35) {
    note = DMaj();
    Serial.println("picked DMaj");
  }
  
  else if (celsius > 35 && celsius <= 40) {
    note = AMaj();
    Serial.println("picked AMaj");
  }
  
  else if (celsius > 40 && celsius <= 45) {
    note = EMaj();
    Serial.println("picked EMaj");
  }
  
  else if (celsius > 45 && celsius <= 50) {
    note = BMaj();
    Serial.println("picked BMaj");
  }
  
  else if (celsius > 50) {
    note = FsMaj();
    Serial.println("picked FsMaj");
  }  
}

/*************************************************
 * Possible Chord Selections (CMaj~BMaj)
 *************************************************/

int CMaj() {
     notes[0] = NOTE_C4;
     notes[1] = NOTE_E4;
     notes[2] = NOTE_G4;
     notes[3] = NOTE_C5;
     notes[4] = NOTE_E5;
     notes[5] = NOTE_G5;
     notes[6] = NOTE_C6;
     i = random(0,6);
     int note = notes[i];
     return note;
}

int CsMaj() {
     notes[0] = NOTE_CS4;
     notes[1] = NOTE_F4;
     notes[2] = NOTE_GS4;
     notes[3] = NOTE_CS5;
     notes[4] = NOTE_F5;
     notes[5] = NOTE_GS5;
     notes[6] = NOTE_CS6;
     i = random(0,6);
     int note = notes[i];
     return note;
}

int DMaj() {
     notes[0] = NOTE_D4;
     notes[1] = NOTE_FS4;
     notes[2] = NOTE_A4;
     notes[3] = NOTE_D5;
     notes[4] = NOTE_FS5;
     notes[5] = NOTE_A5;
     notes[6] = NOTE_D6;
     i = random(0,6);
     int note = notes[i];
     return note;
}

int DsMaj() {
     notes[0] = NOTE_DS4;
     notes[1] = NOTE_G4;
     notes[2] = NOTE_AS4;
     notes[3] = NOTE_DS5;
     notes[4] = NOTE_G5;
     notes[5] = NOTE_AS5;
     notes[6] = NOTE_DS6;
     i = random(0,6);
     int note = notes[i];
     return note;
}

int EMaj() {
     notes[0] = NOTE_E4;
     notes[1] = NOTE_GS4;
     notes[2] = NOTE_B4;
     notes[3] = NOTE_E5;
     notes[4] = NOTE_GS5;
     notes[5] = NOTE_B5;
     notes[6] = NOTE_E6;
     i = random(0,6);
     int note = notes[i];
     return note;
}

int FMaj() {
     notes[0] = NOTE_F3;
     notes[1] = NOTE_A3;
     notes[2] = NOTE_C4;
     notes[3] = NOTE_F4;
     notes[4] = NOTE_A4;
     notes[5] = NOTE_C5;
     notes[6] = NOTE_F5;
     i = random(0,6);
     int note = notes[i];
     return note;
}

int FsMaj() {
     notes[0] = NOTE_FS3;
     notes[1] = NOTE_AS3;
     notes[2] = NOTE_CS4;
     notes[3] = NOTE_FS4;
     notes[4] = NOTE_AS4;
     notes[5] = NOTE_CS5;
     notes[6] = NOTE_FS5;
     i = random(0,6);
     int note = notes[i];
     return note;
}

int GMaj() {
     notes[0] = NOTE_G3;
     notes[1] = NOTE_B3;
     notes[2] = NOTE_D4;
     notes[3] = NOTE_G4;
     notes[4] = NOTE_B4;
     notes[5] = NOTE_D5;
     notes[6] = NOTE_G5;
     i = random(0,6);
     int note = notes[i];
     return note;
}

int GsMaj() {
     notes[0] = NOTE_GS3;
     notes[1] = NOTE_C4;
     notes[2] = NOTE_DS4;
     notes[3] = NOTE_GS4;
     notes[4] = NOTE_C5;
     notes[5] = NOTE_DS5;
     notes[6] = NOTE_GS5;
     i = random(0,6);
     int note = notes[i];
     return note;
}

int AMaj() {
     notes[0] = NOTE_A3;
     notes[1] = NOTE_CS4;
     notes[2] = NOTE_E4;
     notes[3] = NOTE_A4;
     notes[4] = NOTE_CS5;
     notes[5] = NOTE_E5;
     notes[6] = NOTE_A5;
     i = random(0,6);
     int note = notes[i];
     return note;
}

int AsMaj() {
     notes[0] = NOTE_AS3;
     notes[1] = NOTE_D4;
     notes[2] = NOTE_F4;
     notes[3] = NOTE_AS4;
     notes[4] = NOTE_D5;
     notes[5] = NOTE_F5;
     notes[6] = NOTE_AS5;
     i = random(0,6);
     int note = notes[i];
     return note;
}

int BMaj() {
     notes[0] = NOTE_B3;
     notes[1] = NOTE_DS4;
     notes[2] = NOTE_FS4;
     notes[3] = NOTE_B4;
     notes[4] = NOTE_DS5;
     notes[5] = NOTE_FS5;
     notes[6] = NOTE_B5;
     i = random(0,6);
     int note = notes[i];
     return note;
}

/*************************************************
 * Possible Waveform Selections (Triangle~Noise)
 *************************************************/

void ChangeWave() {

  // wait for new data from accelerometer
  if (accel.available()) {                            
    // orientation of board (Right, Left, Down, Up, Flat)
    if (accel.isRight() == true) {
      glove.setWave(0,SAW);
      glove.setWave(1,SAW);
      glove.setWave(2,SAW);
      Serial.println("Right");
    }
    else if (accel.isLeft() == true) {
      glove.setWave(0,SAW);
      glove.setWave(1,SAW);
      glove.setWave(2,SAW);
      Serial.println("Left");
    }
    else if (accel.isUp() == true) {
      glove.setWave(0,SQUARE);
      glove.setWave(1,SQUARE);
      glove.setWave(2,SQUARE);
      Serial.println("Up");
    }
    else if (accel.isDown() == true) {
      glove.setWave(0,SQUARE);
      glove.setWave(1,SQUARE);
      glove.setWave(2,SQUARE);
      Serial.println("Down");
    }
    else if (accel.isFlat() == true) {
      glove.setWave(0,TRIANGLE);
      glove.setWave(1,TRIANGLE);
      glove.setWave(2,TRIANGLE);
      Serial.println("Flat");
    }
  }
}

/*************************************************
 * Audio Channels (CH0~CH2)
 *************************************************/

// play selected note on audio CH0
void TonePlayerA(int note) {
  Serial.println(note);
  glove.setFrequency(0, note);
  glove.trigger(0);
  delay(1000);
}

// play selected note on audio CH1
void TonePlayerB(int note) {
  Serial.println(note);
  glove.setFrequency(1, note);
  glove.trigger(1);
  delay(1000);
}

// play selected note on audio CH2
void TonePlayerC(int note) {
  Serial.println(note);
  glove.setFrequency(2, note);
  glove.trigger(2);
  delay(1000);
}

/*************************************************/

void loop() {

  // begin tilt data transmission
  Wire.beginTransmission(0x1D); 
  Serial.println("Begin orientation data transmission!");

  ChangeWave();                                     // execute orientation-to-wave change for CH0

  // end tilt data transmission
  Wire.endTransmission(0x1D);
 
  // begin temperature data transmission
  Wire.beginTransmission(0x5A);


  // turn on sound for audio CH0
  finger0 = analogRead(A0);
  Serial.print("Finger 0 ");
  Serial.println(finger0);
  if(finger0 > 10) {

    // display CH0 info
    Serial.println("A"); 
    Serial.println("CH0");
    
    // execute chord and note selection for CH0
    ChordPicker();
    TonePlayerA(note);
    
  // turn off sound for audio CH0   
  } else if(finger0 <= 10) {
    glove.setFrequency(0,0);
  }

  // turn on sound for audio CH1
  finger1 = analogRead(A1);
  Serial.print("Finger 1 ");
  Serial.println(finger1);
  if(finger1 > 10) {

    // display CH1 info
    Serial.println("B"); 
    Serial.println("CH1");

    // execute chord and note selection for CH1
    ChordPicker();
    TonePlayerB(note);

  // turn off sound for audio CH1   
  } else if(finger1 <= 10) {
    glove.setFrequency(1,0);
  }

  // turn on sound for audio CH2
  finger2 = analogRead(A2);
  Serial.print("Finger 2 ");
  Serial.println(finger2);
  if(finger2 > 10) {

    // display CH2 info
    Serial.println("C"); 
    Serial.println("CH2");

    // execute chord and note selection for CH2
    ChordPicker();
    TonePlayerC(note);
 
  // turn off sound for audio CH2   
  } else if(finger2 <= 10) {
    glove.setFrequency(2,0);
  }

  // end temperature data transmission
  Wire.endTransmission(0x5A);
}
