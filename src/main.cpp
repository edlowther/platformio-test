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

void loop() {
  loudness = analogRead(0);
  Serial.println(loudness);
  loudnessFactor = map(loudness, 0, 200, 100, 0);
  if (loudnessFactor < 0) {
    loudnessFactor = 0;
  } else if (loudnessFactor > 100) {
    loudnessFactor = 100;
  }
  if (loudnessFactor > 20) {
    SEPARATION_STRENGTH = 0.9;
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
  for(int i=0; i<NPIXELS; i++) {
    int brightness = view.output(i);
    if (brightness >= 255) {
      brightness = 255;
    }
    pixels.setPixelColor(i, pixels.Color(brightness, brightness*0.1, brightness*0.1));
  }
  pixels.show();
  // Pause before next iteration:
  delay(DELAYVAL); 
}