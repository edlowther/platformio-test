#ifndef CONFIG_H
#define CONFIG_H

int NROWS = 6;
int NCOLS = 8;
const int NPIXELS = 48;
const int NBOIDS = 7;

const float MARGIN = 1.0;
const float TURNFACTOR = 0.3;

const float MAXSPEED = 0.5;
const float MINSPEED = 0.05;

int BRIGHTNESS_INCREMENT = 20;
int DELAYVAL = 200;

float COHESION_STRENGTH = 0.3;

float SEPARATION_DISTANCE = 4;
float SEPARATION_STRENGTH = 0.1; 

float ALIGNMENT_DISTANCE = 9;
float ALIGNMENT_STRENGTH = 0.5; 

int STARTING_POSITION_LIMITS[2] = {2, 3};
float STARTING_VELOCITY_LIMITS[2] = {-0.1, 0.1};

#endif
