#ifndef CONFIG_H
#define CONFIG_H

int NROWS = 8;
int NCOLS = 7;
int NLAYERS = 7;
const int NPIXELS = 392;
const int NBOIDS = 8;

const float MARGIN = 1.0;
const float TURNFACTOR = 0.5;

const float MAXSPEED = 5.5;
const float MINSPEED = 0.05;

int BRIGHTNESS_INCREMENT = 20;
int DELAYVAL = 120;

float COHESION_STRENGTH = 0.3;

float SEPARATION_DISTANCE = 2;
float SEPARATION_STRENGTH = 0.5; 

float ALIGNMENT_DISTANCE = 8;
float ALIGNMENT_STRENGTH = 1.0; 

int STARTING_POSITION_LIMITS[2] = {1, 5};
float STARTING_VELOCITY_LIMITS[2] = {-1.1, 1.1};

#endif
