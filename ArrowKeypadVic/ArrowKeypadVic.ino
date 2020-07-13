// Code by Victor Delafontaine
// Inspired by Foster Phillips (Instuctables, username: FosterP)

#include <Adafruit_SSD1306.h>
#include "HID-Project.h"    // for multimedia keys

// Defining each pin, with a unique name
#define LUp 15
#define Up 8
#define RUp 14
#define Left 6
#define Down 7
#define Right 9
#define LedData 5

// modes
#define MODE_KEYBOARD 1
#define MODE_MOUSE 2
#define MODE_MEDIA 3
#define MODE_DEFAULT 4
int mode = MODE_MEDIA;

#define OLED_RESET 4
#define NumLed 6
boolean OledClear = true;

long time_start = 0;
long time_start_l = 0;
long time_start_r = 0;

//Setting up the OLED display and Neopixel Libraries
Adafruit_SSD1306 display(OLED_RESET);

void setup() {
  // Telling the Arduino that the switches are inputs, and the LED is an output //
  pinMode (LUp, INPUT_PULLUP);
  pinMode (Up, INPUT_PULLUP);
  pinMode (RUp, INPUT_PULLUP);
  pinMode (Left, INPUT_PULLUP);
  pinMode (Down, INPUT_PULLUP);
  pinMode (Right, INPUT_PULLUP);
  pinMode (LedData, OUTPUT);
  
  Keyboard.begin();
  Mouse.begin();
  Consumer.begin();

  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void loop() {
  
    /*************************** clear screen ***********************/
    if (OledClear == true) {
      display.clearDisplay();
      display.setCursor(0, 0);
      switch(mode){
        case(MODE_KEYBOARD): display.println("Keyboard mode"); break;
        case(MODE_MOUSE): display.println("Mouse mode"); break;
        case(MODE_MEDIA): display.println("Media mode"); break;
        case(MODE_DEFAULT): display.println("Default mode"); break;
      }
      display.display();
      OledClear = false;
    }

    /*************************** change mode ***********************/
    // for left button
    if (digitalRead(LUp) == LOW){              // left pressed
      delay(100);
      if (digitalRead(RUp) == LOW){            // also right 
        delay(500);
        if(digitalRead(LUp) == LOW && digitalRead(RUp) == LOW){       // both long pressed
          Serial.print("Both button long pressed\n");
        }
        else{                                                         // both short pressed
          Serial.print("Both button short pressed\n");
        }
      }
      else{                                    // only left
        delay(200);
        if (digitalRead(LUp) == HIGH){         // left short pressed
          Serial.print("Left button short pressed\n");
          mode = MODE_MOUSE;
          OledClear = true;
          delay(300);  // to leave time to release
        }
        if (digitalRead(LUp) == LOW){          // left long pressed
          Serial.print("Left button long pressed\n");
          mode = MODE_KEYBOARD;
          OledClear = true;
          delay(300);  // to leave time to release
        }
      }
    }
    // for right button
    if (digitalRead(RUp) == LOW){            // right pressed
      delay(100);
      if (digitalRead(LUp) == LOW){          // also left 
        delay(500);
        if(digitalRead(LUp) == LOW && digitalRead(RUp) == LOW){       // both long pressed
          Serial.print("Both button long pressed\n");
        }
        else{                                                         // both short pressed
          Serial.print("Both button short pressed\n");
        }
      }
      else{                                  // only right
        delay(200);
        if (digitalRead(RUp) == HIGH){       // right short pressed
          Serial.print("Right button short pressed\n");
          mode = MODE_MEDIA;
          OledClear = true;
          delay(300);  // to leave time to release
        }
        if (digitalRead(RUp) == LOW){       // right long pressed
          Serial.print("Right button long pressed\n");
          mode = MODE_DEFAULT;
          OledClear = true;
          delay(300);  // to leave time to release
        }
      }
    }

    /*************************** keyboard mode ***********************/
    if (mode == MODE_KEYBOARD){
        //Up Arrow//
        if (digitalRead(Up) == LOW) Keyboard.press(KEY_UP_ARROW);
        if (digitalRead(Up) == HIGH) Keyboard.release(KEY_UP_ARROW);
    
        //Down Arrow//
        if (digitalRead(Down) == LOW) Keyboard.press(KEY_DOWN_ARROW);
        if (digitalRead(Down) == HIGH) Keyboard.release(KEY_DOWN_ARROW);
    
        //Right Arrow//
        if (digitalRead(Right) == LOW) Keyboard.press(KEY_RIGHT_ARROW);
        if (digitalRead(Right) == HIGH) Keyboard.release(KEY_RIGHT_ARROW);
    
        //Left Arrow//
        if (digitalRead(Left) == LOW) Keyboard.press(KEY_LEFT_ARROW);
        if (digitalRead(Left) == HIGH) Keyboard.release(KEY_LEFT_ARROW);
    
        /*//Top Right Button//
        if (digitalRead(RUp) == LOW) Keyboard.press(KEY_LEFT_ALT);
        if (digitalRead(RUp) == HIGH) Keyboard.release(KEY_LEFT_ALT);
    
        //Top Right Button//
        if (digitalRead(LUp) == LOW) Keyboard.press(KEY_LEFT_CTRL);
        if (digitalRead(LUp) == HIGH) Keyboard.release(KEY_LEFT_CTRL);*/
    }

    /*************************** mouse mode ***********************/
    if (mode == MODE_MOUSE){
        if (digitalRead(Down) == LOW) Mouse.move(0, 2);               // normal mouse mouvements
        if (digitalRead(Up) == LOW) Mouse.move(0, -2);
        if (digitalRead(Right) == LOW) Mouse.move(2, 0);
        if (digitalRead(Left) == LOW) Mouse.move(-2, 0);

        if (digitalRead(Left) == LOW && digitalRead(Right) == LOW){   // left and right pressed: trigger mouse buttons
          delay(200);
          if (digitalRead(Left) == LOW && digitalRead(Right) == LOW){        // still pressing both
            while(digitalRead(Left) == LOW && digitalRead(Right) == LOW){
              Serial.print("Pressing RL: accessing mouse buttons\n");
              if (digitalRead(Up) == LOW){
                Serial.print("Left button\n");
                Mouse.click(MOUSE_LEFT);
                delay(300);
              }
              if (digitalRead(Down) == LOW){
                Serial.print("Right button\n");
                Mouse.click(MOUSE_RIGHT);
                delay(300);
              }
            }
          }
        }
    }

    /*************************** media mode ***********************/
    if (mode == MODE_MEDIA){
         //Mouse Functions, first value is horizontal, second value is vertical//
        if (digitalRead(Down) == LOW){
          delay(200);
          if (digitalRead(Down) == LOW){        // still pressing mute/down: accessing volume control
            while(digitalRead(Down) == LOW){
              if (digitalRead(Right) == LOW){
                Consumer.write(MEDIA_VOLUME_UP);
                delay(150);
              }
              if (digitalRead(Left) == LOW){
                Consumer.write(MEDIA_VOLUME_DOWN);
                delay(150);
              }
            }
          }
          else{
            Consumer.write(MEDIA_VOLUME_MUTE);
          }
          delay(150);
        }
        if (digitalRead(Up) == LOW){
          Consumer.write(MEDIA_PLAY_PAUSE);
          delay(150);
        }
        if (digitalRead(Right) == LOW){
          Consumer.write(MEDIA_NEXT);
          delay(150);
        }
        if (digitalRead(Left) == LOW){
          Consumer.write(MEDIA_PREVIOUS);
          delay(150);
        }
    }
}
