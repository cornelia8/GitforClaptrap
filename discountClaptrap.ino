#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

int vibPin = 6;
int pirPin = 7; 
int pirStat = 0;
int motor1pin1 = 4;
int motor1pin2 = 5;
int enA = 3;

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
int randomTrack;

void setup() {   
  pinMode(13, OUTPUT);

  //turn on led
  digitalWrite(13, HIGH);
  delay(200);
  digitalWrite(13, LOW);
  delay(200);
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  delay(100);
  digitalWrite(13, HIGH);
  
  pinMode(pirPin, INPUT);
  pinMode(vibPin, INPUT);

  mySoftwareSerial.begin(9600);
  Serial.begin(9600);

  Serial.println();
  // Serial.println("Initializing DFPlayer ... (May take 3~5 seconds)");

  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
//    Serial.println("Unable to begin:");
//    Serial.println("1.Please recheck the connection!");
//    Serial.println("2.Please insert the SD card!");
    while(true);
  }
  //myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(enA, INPUT);
  
  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor1pin2, LOW);
  analogWrite(enA, 0);

  myDFPlayer.volume(25);
  playTrack(2);
  delay(2000);
}

void loop(){

  // vibration sensor will take priority in input take, since the PIR sensor is so sensitive
  long measurement = TP_init();
  if (measurement > 1000)
  {
    playTrack(8);
    digitalWrite(13, LOW);

    // claptrap go brr for 3 seconds
    analogWrite(enA, 100);   
    delay(3000);
    analogWrite(enA, 0);
    
    digitalWrite(13, HIGH);
    delay(200);
    digitalWrite(13, LOW);
    delay(200);
    digitalWrite(13, HIGH);
    delay(500);
    digitalWrite(13, LOW);
    delay(100);
    digitalWrite(13, HIGH);
    delay(2000); // give it 2 seconds to calm down
  }
  delay(200);
  
  pirStat = digitalRead(pirPin); 
 
  Serial.println(pirStat);
  if (pirStat == HIGH) // if motion detected
  { 
    // Serial.println("Motion detected");
    // generate a random number from 1 to 10 that is not 2 or 8
    randomTrack = random(1, 11);
    while(randomTrack == 2 || randomTrack == 8)
    {
      randomTrack = random(1, 11);
    }
    
    playTrack(randomTrack);
    delay(2000); // give it two seconds to calm down
  }
 
  delay(200);
} 

void playTrack(uint8_t track) {
  // make sure the tracks are stopped
   myDFPlayer.stop();
   delay(200);
   myDFPlayer.play(track);
   int file = myDFPlayer.readCurrentFileNumber();

  // sometimes is gets stuck on the wrong track, this is to prevent it
   while (file != track) 
   {
     myDFPlayer.play(track);
     file = myDFPlayer.readCurrentFileNumber();
   }

   // add a delay for the length of each track
   // 1 - oh no i'm going to ie
   // 2 - ha ha i am alive
   // 3 - hey
   // 4 - good as new
   // 5 - dubstep
   // 6 - aah sorry
   // 7 - actually quite depressed (this one needs volume up)
   // 8 - ow
   // 9 - so lonely
   // 10 - far uglier than i remembered
   
   switch(track)
   {
      case 1:
        delay(2000);
        break;
      case 2:
        delay(3000);
        break;
      case 3:
        delay(1000);
        break;
      case 4:
        delay(3000);
        break;
      case 5:
        delay(11000);
        break;
      case 6:
        delay(11000);
        break;
      case 7:
        myDFPlayer.volume(30);
        delay(6000);
        myDFPlayer.volume(25);
        break;
      case 8:
        delay(1000);
        break;
      case 9:
        delay(1000);
        break;
      case 10:
        delay(4000);
        break;
   }
}

long TP_init(){
  // get value from the vibration sensor
  //wait for the pin to get HIGH and returns measurement
  long measurement = pulseIn(vibPin, HIGH);
  return measurement;
}
