#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <MovingAverage.h>

int speakerPin = 9;
int numTones = 10;
/*int tones[] = {261, 277, 294, 311, 330, 349, 370, 392, 415, 440};
//            mid C  C#   D    D#   E    F    F#   G    G#   A*/
int tones[] = {200, 220, 240, 260, 280, 380, 400, 420, 440, 460};
int tsz = sizeof(tones) / sizeof(int);
float lN = (float) tsz / 2.0;
int offset = 0;
int zeroSpace = 5;
float sR = 90-zeroSpace;
MovingAverage filter(0.08);
Adafruit_MMA8451 mma = Adafruit_MMA8451();

void setup() {
  Serial.begin(9600);
  /*while (!Serial){};
  Serial.println("Adafruit MMA8451 test!");*/
  if (! mma.begin()){
    /*Serial.println("Couldnt start");*/
    while (1);
  }
  /*Serial.println("MMA8451 found!");*/
  mma.setRange(MMA8451_RANGE_2_G);
  /*Serial.print("Range = "); Serial.print(2 << mma.getRange());
  Serial.println("G");*/

  float avg;
  for (int i = 0; i < 1000; i++){
    mma.read();
    avg += mma.x;
    delay(1);
  }
  offset = (int) avg/1000;
  /*Serial.print("Offset = "); Serial.print(offset);Serial.println();*/
  filter.reset(0.0);
}

void loop() {
  mma.read();
  /*Serial.print(mma.x-offset);*/
  filter.update(mma.x-offset);
  /*Serial.print(",");
  Serial.println(filter.get());*/
  playNote(filter.get());
  delay(10);
}

void playNote(float val) {
  int idx;
  
  if (val < -4000){
    val = -4000;
  }
  else if (val > 4000){
    val = 4000;
  }
  else{};
  
  float a = val+4000;
  a = a * (0.0225);
  float b;

  if (a > 90+zeroSpace){
    b = (lN/sR)*(a-(90+zeroSpace));
    idx = (int) b;
    idx = idx + lN;
    tone(speakerPin, tones[idx]);
  }
  else if (a < 90-zeroSpace){
    b = (lN/sR)*a;
    idx = (int) b;  
    tone(speakerPin, tones[idx]);
  }
  else {
    noTone(speakerPin);
    }
}

