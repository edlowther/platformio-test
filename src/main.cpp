#include <Adafruit_NeoPixel.h>
#include <BasicLinearAlgebra.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        9 // On Trinket or Gemma, suggest changing this to 1

int NROWS = 7;
int NCOLS = 7;
const int NPIXELS = 49;
const int NBOIDS = 21;

int BRIGHTNESS_INCREMENT = 50;
int DELAYVAL = 2;

int STARTING_POSITION_LIMITS[2] = {1, 2};
float STARTING_VELOCITY_LIMITS[2] = {-0.8, 1.3};

Adafruit_NeoPixel pixels(NPIXELS, PIN, NEO_GRB + NEO_KHZ800);
using namespace BLA;

class Positions {
    public: 
    BLA::Matrix<NBOIDS> xs;
    BLA::Matrix<NBOIDS> ys;
};

class Velocities {
    public: 
    BLA::Matrix<NBOIDS> dxs;
    BLA::Matrix<NBOIDS> dys;
};

class Flock {
    public: 
    Positions positions;
    Velocities velocities;
    Flock() {
        float LO = STARTING_VELOCITY_LIMITS[0];
        float HI = STARTING_VELOCITY_LIMITS[1];

        for (int i = 0; i < NBOIDS; i++) {
            positions.xs(i) = random(STARTING_POSITION_LIMITS[0]);
            positions.ys(i) = random(STARTING_POSITION_LIMITS[0]);
            velocities.dxs(i) = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(HI-LO)));
            velocities.dys(i) = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(HI-LO)));
        }
    }

    void update() {
        positions.xs += velocities.dxs;
        positions.ys += velocities.dys;

        BLA::Matrix<NBOIDS> directionToMiddleXs = positions.xs - getMean(positions.xs);
        BLA::Matrix<NBOIDS> directionToMiddleYs = positions.ys - getMean(positions.ys);

        velocities.dxs -= directionToMiddleXs * 0.1;
        velocities.dys -= directionToMiddleYs * 0.1;
    }

    float getMean(BLA::Matrix<NBOIDS> values) {
        float total = 0.0;
        for (int i = 0; i < values.Rows; i++) {
            total += values(i);
        }
        return total / values.Rows;
    }
};

class View {
    public: 
    BLA::Matrix<NPIXELS> output; 
    View() {
        output.Fill(0);
    }
    void map(Positions & positions) {
        output.Fill(0);
        for (int i = 0; i < NBOIDS; i++) {
            int y = round(positions.ys(i));
            int x = round(positions.xs(i));
            while (x < 0) {
                x += NCOLS;
            }
            while (x > NCOLS - 1) {
                x -= NCOLS;
            }
            while (y < 0) {
                y += NROWS;
            }
            while (y > NROWS - 1) {
                y -= NROWS;
            }
            int idx;
            if (y % 2 == 0) {
                idx = y * NCOLS + x;
            } 
            else {
                idx = y * NCOLS + (NCOLS - x - 1);
            }
            output(idx) += BRIGHTNESS_INCREMENT;
        }
    }
};

Flock flock;
View view;

void setup() {
    pixels.begin();
    Serial.begin(9600);
}

void loop() {
  flock.update();
  view.map(flock.positions);
  pixels.clear(); // Set all pixel colors to 'off'
  for(int i=0; i<NPIXELS; i++) {
    int brightness = view.output(i);
    Serial.println(brightness);
    pixels.setPixelColor(i, pixels.Color(brightness, brightness, brightness));
  }

  pixels.show();

  delay(DELAYVAL); 
      
}