// Program for the Minecraft Ore lamp. See https://www.thingiverse.com/thing:3625720
//
// Made on an Arduino Nano V3. The LED pins needs to be PWM capable pins. 
// 
// Pins:
// Red: 3
// Green: 5
// Blue: 6
// Switch: 12
//
// The button has seven functions. 
// Button press #1-#5 switches between the different Minecraft ore colors (except coal). 
// To change the fade effect on the ore colors, change the MIN/MAX variable. The fade effect will cycle the intensisty
// of the LED between these values.
// Button press #6 runs the LED through all colors. To speed up/slow down this effect, change the SPEED variable.
// Button press #7 switches off the LED. Another click on the button starts from the beginning again.
//
// Jörgen Pehrson, 2019
// 
// Code edited to Chrono-Library cause Metro is not longer supported
// Thomas Denkmann, 2025 


//#include <Metro.h>   //Not longer supported
#include <Chrono.h>

byte buttonPresses = 0; 
byte lastPressCount = 1;

#define switchPin 12
#define RED_PIN 3
#define GREEN_PIN 5
#define BLUE_PIN 6
#define ARR_LEN 6
#define RGB_MAX 255
#define MAX_STEPS ARR_LEN * RGB_MAX
#define SPEED 40

int rgbRainbowMap[ARR_LEN][3] = {
  { 1, 0, 0 },
  { 1, 1, 0 },
  { 0, 1, 0 },
  { 0, 1, 1 },
  { 0, 0, 1 },
  { 1, 0, 1 },
};

byte maxi = 255; // 'max' and 'min' edited due ...
byte mini = 120; // ...to conflicts during compilation
float arg1 = 0;
float arg2 = 0;

int gleamStep = MAX_STEPS;
// Metro gleamMetro = Metro(SPEED); 
Chrono myChrono;

void setup() {
  pinMode(switchPin, INPUT_PULLUP);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

}

void loop(){
  float out;

  if (digitalRead(switchPin) == LOW)
   {
    Serial.print("Pressed!");
    buttonPresses++;                
    delay(300);                      
  }
  if (buttonPresses == 8) {
    buttonPresses = 0;          
  }
  if (lastPressCount != buttonPresses)
  {
    lastPressCount = buttonPresses;
  }
      switch (buttonPresses) {
      case 0:
        // Iron
        analogWrite(RED_PIN, 200);
        analogWrite(GREEN_PIN, 200);
        analogWrite(BLUE_PIN, 100);
        break;
        case 1:
        // Redstone
        out = fadeLed();
        analogWrite(RED_PIN, out);
        analogWrite(GREEN_PIN, LOW);
        analogWrite(BLUE_PIN, LOW);
        break;
        case 2:
        // Lapiz Lazuli
        out = fadeLed();
        analogWrite(RED_PIN, LOW);
        analogWrite(GREEN_PIN, LOW);
        analogWrite(BLUE_PIN, out);
        break;
        case 3:
        // Gold
        out = fadeLed();
        analogWrite(RED_PIN, out);
        analogWrite(GREEN_PIN, out);
        analogWrite(BLUE_PIN, LOW);
        break;
        case 4:
        // Diamond
        out = fadeLed();
        analogWrite(RED_PIN, LOW);
        analogWrite(GREEN_PIN, out);
        analogWrite(BLUE_PIN, out);
        break;
        case 5:
        // Emerald
        out = fadeLed();
        analogWrite(RED_PIN, LOW);
        analogWrite(GREEN_PIN, out);
        analogWrite(BLUE_PIN, LOW);
        break;
        case 6:
         moodLight();
         break;
        case 7:
        //Off
        analogWrite(RED_PIN, LOW);   
        analogWrite(GREEN_PIN, LOW);
        analogWrite(BLUE_PIN, LOW);
        break;
        
    }
}

float fadeLed (void) {
  static float in = 4.712;

  float result;
  arg1 = maxi/2 - mini/2;
  arg2 = maxi/2 + mini/2;
  in = in + 0.0004;
  if (in > 10.995)
   in = 4.712;
  result = sin(in) * arg1 + arg2;
  return result;
  }

void moodLight() {
  //if (gleamMetro.check()) {}
  if (myChrono.hasPassed(SPEED)) {
    myChrono.restart(); //New
    if (gleamStep >= MAX_STEPS) {
      gleamStep = 0;
    };
    moodRgb(gleamStep++);
  };
};

void moodRgb(int step) {
  int rgb[3] = {0, 0, 0};
  const int index = step / RGB_MAX;
  const int _mod = step % RGB_MAX;
  const int next = (index + 1 < ARR_LEN) ? index + 1 : 0;

  for (int i = 0; i < 3; i++) {
    const int section = rgbRainbowMap[index][i];
    const int nextSection = rgbRainbowMap[next][i];
    if (section == nextSection)
      rgb[i] = section * RGB_MAX;
    else if (section > nextSection)
      rgb[i] = RGB_MAX - _mod;
    else
      rgb[i] = _mod;
  };
  setRgb(rgb[0], rgb[1], rgb[2]);
}

void setRgb(int red, int green, int blue) {
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}
