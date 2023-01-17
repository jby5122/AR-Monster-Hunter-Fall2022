

//keyboard lib
#include <Keyboard.h>
#include <Adafruit_CircuitPlayground.h>

bool hasKeyPressed = false;
// variables will change:
char key = 'a';

//----------------------------//

// Tilt Circuit Playground left/right and up/down to move your mouse, and
// press the left and right push buttons to click the mouse buttons!  Make sure
// the slide switch is in the on (+) position to enable the mouse, or slide into
// the off (-) position to disable it.  By default the sketch assumes you hold
// Circuit Playground with the USB cable coming out the top.
// Author: Tony DiCola
// License: MIT License (https://opensource.org/licenses/MIT)
#include <Adafruit_CircuitPlayground.h>
#include <Mouse.h>
#include <Wire.h>
#include <SPI.h>


// Configuration values to adjust the sensitivity and speed of the mouse.
// X axis (left/right) configuration:
#define XACCEL_MIN 3    // Minimum range of X axis acceleration, values below
                            // this won't move the mouse at all.
#define XACCEL_MAX 12      // Maximum range of X axis acceleration, values above
                            // this will move the mouse as fast as possible.
#define XMOUSE_RANGE 30   // Range of velocity for mouse movements.  The higher
                            // this value the faster the mouse will move.
#define XMOUSE_SCALE 1      // Scaling value to apply to mouse movement, this is
                            // useful to set to -1 to flip the X axis movement.

// Z axis (up/down) configuration:
#define ZACCEL_MIN 3
#define ZACCEL_MAX 12
#define ZMOUSE_RANGE 40
#define ZMOUSE_SCALE 1

// Y axis for detecting runing (the head up and down)
#define YACCEL_MIN 8
#define YACCEL_MAX 15
#define YMOUSE_RANGE 60
#define YMOUSE_SCALE 1

// Floating point linear interpolation function that takes a value inside one
// range and maps it to a new value inside another range.  This is used to transform
// each axis of acceleration to mouse velocity/speed. See this page for details
// on the equation: https://en.wikipedia.org/wiki/Linear_interpolation
float lerp(float x, float x0, float x1, float y0, float y1) {
  // Check if the input value (x) is outside its desired range and clamp to
  // those min/max y values.
  if (x <= x0) {
    return y0;
  }
  else if (x >= x1) {
    return y1;
  }
  // Otherwise compute the value y based on x's position within its range and
  // the desired y min & max.
  return y0 + (y1-y0)*((x-x0)/(x1-x0));
}

const int attack = A7;
int slider = A0;
int sliderReading;

void setup() {
  // Initialize Circuit Playground library.
  CircuitPlayground.begin();
  // Initialize Arduino mouse library.
  Mouse.begin();
  pinMode(attack, INPUT);
  Serial.begin(115200);
}

void loop() {
  // Check if the slide switch is enabled (on +) and if not then just exit out
  // and run the loop again.  This lets you turn on/off the mouse movement with
  // the slide switch.
  if (!CircuitPlayground.slideSwitch()) {
    return;
  }

//  // Grab initial left & right button states to later check if they are pressed
//  // or released.  Do this early in the loop so other processing can take some
//  // time and the button state change can be detected.
//  boolean left_first = CircuitPlayground.leftButton();
//  boolean right_first = CircuitPlayground.rightButton();

  // Grab x, z acceleration values (in m/s^2).
  float x = CircuitPlayground.motionX()*10;
  float z = CircuitPlayground.motionZ();
  
  // Use the magnitude of acceleration to interpolate the mouse velocity.
  float x_mag = abs(x);
//  Serial.println(x_mag);
  float x_mouse = lerp(x_mag, XACCEL_MIN, XACCEL_MAX, 0.0, XMOUSE_RANGE);
  float z_mag = abs(z);
  float z_mouse = lerp(z_mag, ZACCEL_MIN, ZACCEL_MAX, 0.0, ZMOUSE_RANGE);
  // Change the mouse direction based on the direction of the acceleration.
  if (x < 0) {
    x_mouse *= -1.0;
  }
  if (z < 0) {
    z_mouse *= -1.0;
  }
  // Apply any global scaling to the axis (to flip it for example) and truncate
  // to an integer value.

  x_mouse = floor(x_mouse*XMOUSE_SCALE);
//  Serial.println(x_mouse);
  z_mouse = floor(z_mouse*ZMOUSE_SCALE);
   
  Mouse.move((int)x_mouse, (int)z_mouse, 0);

//---------------------- keyboard -----------------------------//
  Keyboard.begin();
  // Only send KeyRelease if previously pressed to avoid sending
  // multiple keyRelease reports (that consume memory and bandwidth)
  if ( hasKeyPressed )
  {
    hasKeyPressed = false;
    Keyboard.release(key);
    // Delay a bit after a report
    delay(5);
  }


  if((CircuitPlayground.readCap(A1)) >= 1000){
  //alligator clip to A1 for capacitive touch 
    key = 'w';
    Keyboard.press(key);
    hasKeyPressed = true;

    // Delay a bit after a report
    delay(5);   
  }

  if((CircuitPlayground.readCap(A2)) >= 1000){
    //alligator clip to A2 for capacitive touch
    key = 'a';
    Keyboard.press(key);
    hasKeyPressed = true;

    // Delay a bit after a report
    delay(5);
  }

  if((CircuitPlayground.readCap(A3)) >= 1000){
//     alligator clip to A3 for capacitive touch                               
    key = 's';
    Keyboard.press(key);
    hasKeyPressed = true;

    // Delay a bit after a report
    delay(5);
  }

  if((CircuitPlayground.readCap(A4)) >= 1000){
     //alligator clip to A4 for capacitive touch
   key = 'd';
    Keyboard.press(key);
    hasKeyPressed = true;

    // Delay a bit after a report
    delay(5);
  }

  //------running for space press -------//
  float y_State1 = CircuitPlayground.motionY();
  float y_mag1 = abs(y_State1)*100;
  delay(5);
  float y_State2 = CircuitPlayground.motionY();
  float y_mag2 = abs(y_State2)*100;
//  Serial.print("y1: ");
//  Serial.println(y_mag1);
//  Serial.print("y2: ");
//  Serial.println(y_mag2);
    int y_movement = abs (y_mag2 - y_mag1);
//    Serial.println(y_movement); 
  if(y_movement > 70 && y_movement< 100){
    key = ' ';
    Keyboard.press(key);
    hasKeyPressed = true;

    // Delay a bit after a report
    delay(5);
//    Serial.println("test");
  }

    int attackState = digitalRead(attack);
//    Serial.println(attackState);
  if(attackState == HIGH){
     //attack monster > right arm > mouse left
    Mouse.click(MOUSE_LEFT);
    Serial.print("hit");
    // Delay a bit after a report
    delay(5);
  }         

//  if((CircuitPlayground.readCap(A0)) >= 1000){
//     //find items in the bag > mouse scroll
//    Mouse.move(0, 0, -1);
//    
//  }
//
//  sliderReading = analogRead (slider);
//  Serial.println(sliderReading);

  if((CircuitPlayground.readCap(A6)) >= 1000){
     //use item > keyboard E
    key = 'e';
    Keyboard.press(key);
    hasKeyPressed = true;

    // Delay a bit after a report
    delay(5);
  }

  if((CircuitPlayground.readCap(A5)) >= 1000){
     //gather/carve & confirm/talk > keyboard F
    key = 'f';
    Keyboard.press(key);
    hasKeyPressed = true;
   
    // Delay a bit after a report
    delay(5);
  }
}



      
