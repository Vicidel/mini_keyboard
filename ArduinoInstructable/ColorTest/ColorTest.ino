//Code made by Foster Phillips
//March-April 2018
//Created for Instuctables, username: FosterP
//Instagram: Lego_Rocket_08
//File Name: ColorTest

#include <Adafruit_NeoPixel.h>
#define PIN 10
#define Num 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(Num, PIN, NEO_GRB + NEO_KHZ800);
void setup() {
  // put your setup code here, to run once:
strip.begin();
strip.show();
}

void loop() {
  // put your main code here, to run repeatedly:

strip.setPixelColor(0,255,0,0);
strip.setPixelColor(1,255,0,0);
strip.setPixelColor(2,255,0,0);
strip.setPixelColor(4,255,0,0);

strip.setPixelColor(3,0,0,255);
strip.setPixelColor(5,0,0,255);
strip.show();
}
