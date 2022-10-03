#include <Adafruit_NeoPixel.h>

#include "../lib/Config.h"
#include "../lib/Flock.h"
#include "../lib/View.h"

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        9

Adafruit_NeoPixel pixels(NPIXELS, PIN, NEO_GRB + NEO_KHZ800);

Flock flock;
View view;

void setup() {
  pixels.begin();
  Serial.begin(9600);
}

int loudness;
float loudnessFactor; 

BLA::Matrix<NPIXELS> t_minus_0;
BLA::Matrix<NPIXELS> t_minus_1;
BLA::Matrix<NPIXELS> t_minus_2;
BLA::Matrix<NPIXELS> t_minus_3;
BLA::Matrix<NPIXELS> t_minus_4;

void loop() {
  loudness = analogRead(0);
  Serial.println(loudness);
  loudnessFactor = map(loudness, 0, 250, 3, 0);
  if (loudnessFactor < 0) {
    loudnessFactor = 0;
  } else if (loudnessFactor > 3) {
    loudnessFactor = 3;
  }
  if (loudnessFactor > 1) {
    SEPARATION_STRENGTH = loudnessFactor;
  } else {
    SEPARATION_STRENGTH = 0.5; 
  }
  Serial.println(loudnessFactor);
  Serial.println("----");
  // Update the position of the flock: 
  flock.update();
  // Map the new positions to the pixels in the grid:
  view.map(flock.positions);
  // Tell the NeoPixel API about the new mapping:
  pixels.clear(); // Set all pixel colors to 'off'
  t_minus_0 = view.output;
  for(int i=0; i<NPIXELS; i++) {
    int brightness_minus_0 = t_minus_0(i);
    int brightness_minus_1 = t_minus_1(i);
    int brightness_minus_2 = t_minus_2(i);
    int brightness_minus_3 = t_minus_3(i);
    int brightness_minus_4 = t_minus_4(i);
    pixels.setPixelColor(i, pixels.Color(brightness_minus_0*0.5, brightness_minus_0, brightness_minus_0*0.5));
    if (brightness_minus_0 == 0) {
      if (brightness_minus_1 == 0) {
        if (brightness_minus_2 == 0) {
          if (brightness_minus_3 == 0) {
            pixels.setPixelColor(i, pixels.Color(0, brightness_minus_4*0.05, 0));
          } else {
            pixels.setPixelColor(i, pixels.Color(0, brightness_minus_3*0.2, 0));
          }
        } else {
          pixels.setPixelColor(i, pixels.Color(brightness_minus_2*0.1, brightness_minus_2*0.5, brightness_minus_2*0.1));
        }
      } else {
        pixels.setPixelColor(i, pixels.Color(brightness_minus_1*0.5, brightness_minus_1*0.8, brightness_minus_1*0.5));
      }
    }
  }
  pixels.show();
  t_minus_4 = t_minus_3;
  t_minus_3 = t_minus_2;
  t_minus_2 = t_minus_1;
  t_minus_1 = t_minus_0;
  // Pause before next iteration:
  delay(DELAYVAL); 
}