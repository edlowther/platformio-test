#include <Adafruit_NeoPixel.h>
#include <BasicLinearAlgebra.h>


// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        9 // On Trinket or Gemma, suggest changing this to 1

#define NUMPIXELS 20 // Popular NeoPixel ring size
#define NROWS      5
#define NCOLS      4

int t = 0;

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 50 // Time (in milliseconds) to pause between pixels

class Pixel {
  public: 
    int idx;
    int x;
    int y;
    bool isOn;
    Pixel() {
      idx = 10;
    }
    int getX() {
      if (this->y % 2 == 0) {
        return idx % NCOLS;
      }
      else {
        return NCOLS - (idx % NCOLS) - 1;
      }
    }
    Pixel(int idx) {
      this->idx = idx;
      this->y = idx / NCOLS;
      this->x = getX();
    }
    int getNextIdx() {
      int dx = random(-1, 2);
      int dy = random(-1, 2);

      Serial.print("Starting at idx ");
      Serial.print(this->idx);
      Serial.print(", with coords ");
      Serial.print(this->x);
      Serial.print(", ");
      Serial.println(this->y);

      Serial.print("Change is: ");
      Serial.print(dx);
      Serial.print(", ");
      Serial.println(dy);

      int x = this->x + dx;
      int y = this->y + dy;

      Serial.println("new coords are: ");
      Serial.print(x);
      Serial.print(", ");
      Serial.println(y);
      if (x < 0) {
        x = 0;
      }
      if (x >= NCOLS) {
        x = NCOLS - 1;
      }

      if (y < 0) {
        y = 0;
      }
      if (y >= NROWS) {
        y = NROWS - 1;
      }

      Serial.println("adjusted coords are: ");
      Serial.print(x);
      Serial.print(", ");
      Serial.println(y);

      int nextIdx;
      if (y % 2 == 0) {
        nextIdx = y * NCOLS + x;
      } 
      else {
        nextIdx = y * NCOLS + (NCOLS - x - 1);
      }

      Serial.println("Next idx is: ");
      Serial.println(nextIdx);

      return nextIdx;
    }
    String getState() {
      if (this->isOn) {
        return "yes, hello this is on";
      }
      return "nope";
    }
};

Pixel NowPixelManager[NUMPIXELS];
Pixel NextPixelManager[NUMPIXELS];

using namespace BLA;

void setup() {
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  Serial.begin(9600);
  int startIdx = random(20);
  for(int i=0; i< NUMPIXELS; i++) {
    Pixel pixel = Pixel(i);
    pixel.isOn = i == startIdx;
    NowPixelManager[i] = pixel;
    NextPixelManager[i] = Pixel(i);
  }

}

void loop() {
  // Serial.print(PixelManager);
  pixels.clear(); // Set all pixel colors to 'off'

  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.

  // int r = random(20);
  // int g = random(20);
  // int b = random(20);

  // int r = 200;
  // int g = 0;
  // int b = 200;

  int rs[] = {150, 150, 255, 0, 0, 75, 148};
  int gs[] = {0, 150, 255, 255, 0, 0, 0};
  int bs[] = {0, 0, 0, 0, 255, 130, 211};

  for(int i=0; i<NUMPIXELS; i++) {
    if (NowPixelManager[i].isOn) {
      pixels.setPixelColor(i, pixels.Color(rs[t % 7], gs[t % 7], bs[t % 7]));
      int nextPixelIdx = NowPixelManager[i].getNextIdx();
      Serial.println(nextPixelIdx);
      Serial.println("====");
      if (nextPixelIdx != i) {
        NextPixelManager[nextPixelIdx].isOn = true;
        NextPixelManager[i].isOn = false;
      }
    } else {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }
  }

  pixels.show();
  for(int i=0; i<NUMPIXELS; i++) {
    NowPixelManager[i] = NextPixelManager[i];
  }

  delay(DELAYVAL); 
  t++;

      
}