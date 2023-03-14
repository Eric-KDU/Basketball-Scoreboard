#include <SPI.h>
#include <DMD.h>
#include <TimerOne.h>
#include "SystemFont5x7.h"
#include "Arial_black_16.h"
#include "Arial14.h"
 
char b[8];
String str;
#define DISPLAYS_ACROSS 2
#define DISPLAYS_DOWN 1
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

// Define pin numbers for buttons
const int startPausePin = 2;
const int resetPin = 3;

// Define variables for stopwatch
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
unsigned long elapsedMillis = 0;
bool isRunning = false;
bool buttonPressed = false;

void ScanDMD(){
  dmd.scanDisplayBySPI();
}

 
void setup() {
  // Set input mode for pins
  pinMode(startPausePin, INPUT_PULLUP);
  pinMode(resetPin, INPUT_PULLUP);

  // Start serial communication for output
  Serial.begin(9600);
  Timer1.initialize(5000);
  Timer1.attachInterrupt(ScanDMD);
  dmd.selectFont(Arial_Black_16);
  dmd.clearScreen(true);
}

void loop() {
  // Read input from start/pause button
  if (digitalRead(startPausePin) == LOW) {
    if (!buttonPressed) {
      buttonPressed = true;
      if (!isRunning) {
        isRunning = true;
        previousMillis = millis();
        dmd.clearScreen(true);
      } else {
        isRunning = false;
//        elapsedMillis = (currentMillis - previousMillis);
        dmd.clearScreen(true);
      }
    }
  } else {
    buttonPressed = false;
    elapsedMillis = (currentMillis - previousMillis);
  }

  // Read input from reset button
  if (digitalRead(resetPin) == LOW) {
    elapsedMillis = 0;
    currentMillis = 0;
    previousMillis = 0;
    isRunning = false;
    dmd.clearScreen(true);
    delay(100); // Debounce the button
  }

  // Update stopwatch display
  if (isRunning) {
    currentMillis = millis();
    elapsedMillis = currentMillis - previousMillis;
  }

  // Calculate elapsed time in hours, minutes, seconds, and milliseconds
  int hours = elapsedMillis / 3600000;
  int minutes = (elapsedMillis / 60000) % 60;
  int seconds = (elapsedMillis / 1000) % 60;
  int millis1 = (elapsedMillis / 100) % 10;
  int millis2 = (elapsedMillis / 10) % 10;
  int millis3 = elapsedMillis % 10;

  String strMin, strSec;
  if(minutes<10){
    strMin =  "0" + String(minutes, DEC);
  }else{
    strMin = String(minutes, DEC);
  }
  if(seconds<10){
    strSec =  "0" + String(seconds, DEC);
  }else{
    strSec = String(seconds, DEC);
  }
  
  // Print elapsed time to serial monitor
//  Serial.print(hours);
//  Serial.print(":");
//  Serial.print(minutes);
//  Serial.print(":");
//  Serial.print(seconds);
//  Serial.print(":");
//  Serial.print(millis1);
//  Serial.print(millis2);
//  Serial.print(millis3);
//  Serial.println();
  
//  dmd.clearScreen(true);
  int slen = 0;                               
  str=strMin + ":" + strSec + ":" + String(millis1, DEC) + String(millis2, DEC);
  slen = str.length()+1;                                     
  str.toCharArray(b,slen);           
  dmd.drawString(0,1,b,slen,GRAPHICS_NORMAL);
  
}
