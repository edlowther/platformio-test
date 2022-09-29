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

void loop() {
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