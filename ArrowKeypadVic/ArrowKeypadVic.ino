// Code by Victor Delafontaine
// Inspired by Foster Phillips (Instuctables, username: FosterP)

#include <Adafruit_SSD1306.h>
#include "HID-Project.h"    // for multimedia keys

// for keys
#define LUp 15
#define Up 8
#define RUp 14
#define Left 6
#define Down 7
#define Right 9

// for LED
#define LedData 5
bool led_on = false;

// different modes
#define MODE_KEYBOARD 1
#define MODE_MOUSE 2
#define MODE_MEDIA 3
#define MODE_VLC 4
#define MODE_OFF 5
#define MODE_SNAKE 6
int mode = MODE_MEDIA;

// for OLED screen
#define OLED_RESET 4
#define NumLed 6
boolean OledClear = true;
Adafruit_SSD1306 display(OLED_RESET);


/*************************** clear screen ***********************/
void clear_screen() {
  if (OledClear == true) {
    display.clearDisplay();
    display.setCursor(0, 0);
    switch(mode){
      case(MODE_KEYBOARD): display.println("Keyboard mode"); break;
      case(MODE_MOUSE): display.println("Mouse mode"); break;
      case(MODE_MEDIA): display.println("Media mode"); break;
      case(MODE_VLC): display.println("VLC mode"); break;
      case(MODE_OFF): display.println("Off mode"); break;
      case(MODE_SNAKE): display.println("SNAKE mode"); break;
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
        mode = MODE_MEDIA;
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
  if (digitalRead(Down) == LOW) Mouse.move(0, 2); 
  if (digitalRead(Up) == LOW) Mouse.move(0, -2);
  if (digitalRead(Right) == LOW) Mouse.move(2, 0);
  if (digitalRead(Left) == LOW) Mouse.move(-2, 0);

  // for mouse buttons: for now hold LR and click up/down: TODO change it
  if (digitalRead(Left) == LOW && digitalRead(Right) == LOW){   // left and right pressed
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
void media_function() {

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

  // does nothing, that's the point
}


/*************************** snake mode ***********************/
void snake_function(){

  // TODO
}


/*************************** LED control ***********************/
void led_control(){

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


/*************************** loop ***********************/
void loop() {
  
    // check buttons for mode change
    change_mode();
    
    // change the line of text accordingly
    clear_screen();

    // change LED status
    led_control();
    
    /*************************** mode switch ***********************/
    switch(mode){
      case(MODE_KEYBOARD): keyboard_function(); break;
      case(MODE_MOUSE): mouse_function(); break;
      case(MODE_MEDIA): media_function(); break;
      case(MODE_VLC): vlc_function(); break;
      case(MODE_OFF): off_function(); break;
      case(MODE_SNAKE): snake_function(); break;
    }
}
