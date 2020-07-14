// Code by Victor Delafontaine
// Inspired by Foster Phillips (Instuctables, username: FosterP)

#include <Adafruit_SSD1306.h>
#include "HID-Project.h"    // for multimedia keys
#include "icons.h"          // for icons

// for keys
#define LUp 15
#define Up 8
#define RUp 14
#define Left 6
#define Down 7
#define Right 9

// mouse mouvement speed
int move_speed = 1;
int scroll_speed = 1;

// for LED
#define Led 5
int brightness = 0;
int fade_increment = 50;

// different modes
#define MODE_KEYBOARD 1
#define MODE_MOUSE 2
#define MODE_MUSIC 3
#define MODE_VLC 4
#define MODE_OFF 5
#define MODE_SNAKE 6
int mode = MODE_MUSIC;

// for OLED screen
#define OLED_RESET 4
#define NumLed 6
boolean OledClear = true;
Adafruit_SSD1306 display(OLED_RESET);


/*************************** clear screen ***********************/
void clear_screen() {
  if (OledClear == true) {
    display.clearDisplay();
    switch(mode){
      case(MODE_KEYBOARD): display.drawBitmap(0, 0, keyboard, 128, 32, WHITE); break;
      case(MODE_MOUSE): display.drawBitmap(0, 0, mouse, 128, 32, WHITE); break;
      case(MODE_MUSIC): display.drawBitmap(0, 0, music, 128, 32, WHITE); break;
      case(MODE_VLC): display.drawBitmap(0, 0, vlc, 128, 32, WHITE); break;
      case(MODE_OFF): display.drawBitmap(0, 0, music, 128, 32, WHITE); break;
      case(MODE_SNAKE): display.drawBitmap(0, 0, music, 128, 32, WHITE); break;
    }
    display.display();
    OledClear = false;
  }
}


/*************************** change mode ***********************/
void change_mode() {

  // for left button
  if (digitalRead(LUp) == LOW){                                   // left pressed
    delay(100);
    if (digitalRead(RUp) == LOW){                                   // also right 
      delay(200);
      if(digitalRead(LUp) == LOW && digitalRead(RUp) == LOW){         // both long pressed
        Serial.print("Both button long pressed\n");
        mode = MODE_SNAKE;
        OledClear = true;
        delay(500);  // to leave time to release
      }
      else{                                                           // both short pressed
        Serial.print("Both button short pressed\n");
        mode = MODE_OFF;
        OledClear = true;
        delay(500);  // to leave time to release
      }
    }
    else{                                                           // only left
      delay(200);
      if (digitalRead(LUp) == HIGH){                                  // left short pressed
        Serial.print("Left button short pressed\n");
        mode = MODE_MOUSE;
        OledClear = true;
        delay(500);  // to leave time to release
      }
      if (digitalRead(LUp) == LOW){                                   // left long pressed
        Serial.print("Left button long pressed\n");
        mode = MODE_KEYBOARD;
        OledClear = true;
        delay(500);  // to leave time to release
      }
    }
  }
  
  // for right button
  if (digitalRead(RUp) == LOW){                                   // right pressed
    delay(100);
    if (digitalRead(LUp) == LOW){                                   // also left 
      delay(200);
      if(digitalRead(LUp) == LOW && digitalRead(RUp) == LOW){         // both long pressed
        Serial.print("Both button long pressed\n");
        mode = MODE_SNAKE;
        OledClear = true;
        delay(500);  // to leave time to release
      }
      else{                                                           // both short pressed
        Serial.print("Both button short pressed\n");
        mode = MODE_OFF;
        OledClear = true;
        delay(500);  // to leave time to release
      }
    }
    else{                                                           // only right
      delay(200);
      if (digitalRead(RUp) == HIGH){                                  // right short pressed
        Serial.print("Right button short pressed\n");
        mode = MODE_MUSIC;
        OledClear = true;
        delay(500);  // to leave time to release
      }
      if (digitalRead(RUp) == LOW){                                   // right long pressed
        Serial.print("Right button long pressed\n");
        mode = MODE_VLC;
        OledClear = true;
        delay(500);  // to leave time to release
      }
    }
  }
}


/*************************** keyboard mode ***********************/
void keyboard_function() {
  
  // up arrow
  if (digitalRead(Up) == LOW) Keyboard.press(KEY_UP_ARROW);
  if (digitalRead(Up) == HIGH) Keyboard.release(KEY_UP_ARROW);
  
  // down arrow
  if (digitalRead(Down) == LOW) Keyboard.press(KEY_DOWN_ARROW);
  if (digitalRead(Down) == HIGH) Keyboard.release(KEY_DOWN_ARROW);
  
  // right arrow
  if (digitalRead(Right) == LOW) Keyboard.press(KEY_RIGHT_ARROW);
  if (digitalRead(Right) == HIGH) Keyboard.release(KEY_RIGHT_ARROW);
  
  // left arrow
  if (digitalRead(Left) == LOW) Keyboard.press(KEY_LEFT_ARROW);
  if (digitalRead(Left) == HIGH) Keyboard.release(KEY_LEFT_ARROW);
}

  
/*************************** mouse mode ***********************/
void mouse_function() {

  // mouse mouvements
  if (digitalRead(Down) == LOW) Mouse.move(0, move_speed); 
  if (digitalRead(Up) == LOW) Mouse.move(0, -move_speed);
  if (digitalRead(Right) == LOW) Mouse.move(move_speed, 0);
  if (digitalRead(Left) == LOW) Mouse.move(-move_speed, 0);

  // for mouse buttons: hold up/down and click left/right
  if (digitalRead(Up) == LOW && digitalRead(Down) == LOW){          // up and down
    delay(200);
    if (digitalRead(Up) == LOW && digitalRead(Down) == LOW){        // still pressing both
      while(digitalRead(Up) == LOW && digitalRead(Down) == LOW){
        if (digitalRead(Left) == LOW){
          Mouse.click(MOUSE_LEFT);
          delay(300);
        }
        if (digitalRead(Right) == LOW){
          Mouse.click(MOUSE_RIGHT);
          delay(300);
        }
      }
    }
  }

  // for scroll: hold left/right and click up/down
  if (digitalRead(Left) == LOW && digitalRead(Right) == LOW){          // left and right
    delay(200);
    if (digitalRead(Left) == LOW && digitalRead(Right) == LOW){        // still pressing both
      while(digitalRead(Left) == LOW && digitalRead(Right) == LOW){
        if (digitalRead(Up) == LOW){
          Mouse.move(0, 0, scroll_speed);
          delay(300);
        }
        if (digitalRead(Down) == LOW){
          Mouse.move(0, 0, -scroll_speed);
          delay(300);
        }
      }
    }
  }
}


/*************************** music mode ***********************/
void music_function() {

  // down for mute: hold for volume control with LR
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

  // up for play/pause
  if (digitalRead(Up) == LOW){
    Consumer.write(MEDIA_PLAY_PAUSE);
    delay(150);
  }

  // right/left for next/previous
  if (digitalRead(Right) == LOW){
    Consumer.write(MEDIA_NEXT);
    delay(150);
  }
  if (digitalRead(Left) == LOW){
    Consumer.write(MEDIA_PREVIOUS);
    delay(150);
  }
}


/*************************** VLC mode ***********************/
void vlc_function(){

  // down for printscreen, hold for +-3s
  if (digitalRead(Down) == LOW){
    delay(200);
    if (digitalRead(Down) == LOW){        // still pressing down: accessing 3s control
      while(digitalRead(Down) == LOW){
        if (digitalRead(Right) == LOW){
          Keyboard.press(KEY_RIGHT_SHIFT);      // shift + -> for +3s
          Keyboard.press(KEY_RIGHT_ARROW);
          Keyboard.release(KEY_RIGHT_ARROW);
          Keyboard.release(KEY_RIGHT_SHIFT);
          delay(150);
        }
        if (digitalRead(Left) == LOW){
          Keyboard.press(KEY_RIGHT_SHIFT);      // shift + <- for -3s
          Keyboard.press(KEY_LEFT_ARROW);
          Keyboard.release(KEY_LEFT_ARROW);
          Keyboard.release(KEY_RIGHT_SHIFT);
          delay(150);
        }
      }
    }
    else{
      Keyboard.press(KEY_RIGHT_SHIFT);          // shift + s for printscreen
      Keyboard.press('s');
      Keyboard.release('s');
      Keyboard.release(KEY_RIGHT_SHIFT);
      delay(150);
    }
  }
  
  // up for play/pause, hold for +-1mn
  if (digitalRead(Up) == LOW){
    delay(200);
    if (digitalRead(Up) == LOW){        // still pressing up: accessing 1mn control
      while(digitalRead(Up) == LOW){
        if (digitalRead(Right) == LOW){
          Keyboard.press(KEY_RIGHT_CTRL);       // control + -> for +1mn
          Keyboard.press(KEY_RIGHT_ARROW);
          Keyboard.release(KEY_RIGHT_ARROW);
          Keyboard.release(KEY_RIGHT_CTRL);
          delay(150);
        }
        if (digitalRead(Left) == LOW){
          Keyboard.press(KEY_RIGHT_CTRL);       // control + <- for -1mn
          Keyboard.press(KEY_LEFT_ARROW);
          Keyboard.release(KEY_LEFT_ARROW);
          Keyboard.release(KEY_RIGHT_CTRL);
          delay(150);
        }
      }
    }
    else{
       Keyboard.press(' ');
       Keyboard.release(' ');         // play/pause (space to not interact with possible music player)
      delay(150);
    }
  }
  
  // right for +10s
  if (digitalRead(Right) == LOW) Keyboard.press(KEY_RIGHT_ARROW);
  if (digitalRead(Right) == HIGH) Keyboard.release(KEY_RIGHT_ARROW);

  // left for -10s
  if (digitalRead(Left) == LOW) Keyboard.press(KEY_LEFT_ARROW);
  if (digitalRead(Left) == HIGH) Keyboard.release(KEY_LEFT_ARROW);
}


/*************************** off mode ***********************/
void off_function(){

  // in off mode, can choose lLED brightness by pressing up
  if (digitalRead(Up) == LOW){
    delay(150);
    brightness = brightness + fade_increment;     // add 50 to the brightness
    if (brightness >= 255) brightness = 0;        // if above 255, go back to 
    analogWrite(Led, brightness);
    delay(150);
  }
}


/*************************** snake mode ***********************/
void snake_function(){

  // TODO
}


/*************************** setup ***********************/
void setup() {
  // Telling the Arduino that the switches are inputs, and the LED is an output //
  pinMode (LUp, INPUT_PULLUP);
  pinMode (Up, INPUT_PULLUP);
  pinMode (RUp, INPUT_PULLUP);
  pinMode (Left, INPUT_PULLUP);
  pinMode (Down, INPUT_PULLUP);
  pinMode (Right, INPUT_PULLUP);
  pinMode (Led, OUTPUT);
  
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


/*************************** loop ***********************/
void loop() {
  
    // check buttons for mode change
    change_mode();
    
    // change the line of text accordingly
    clear_screen();

    // write initial LED brightness
    analogWrite(Led, brightness);
    
    /*************************** mode switch ***********************/
    switch(mode){
      case(MODE_KEYBOARD): keyboard_function(); break;
      case(MODE_MOUSE): mouse_function(); break;
      case(MODE_MUSIC): music_function(); break;
      case(MODE_VLC): vlc_function(); break;
      case(MODE_OFF): off_function(); break;
      case(MODE_SNAKE): snake_function(); break;
    }
}
