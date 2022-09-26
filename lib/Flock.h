#ifndef FLOCK_H
#define FLOCK_H

#include <BasicLinearAlgebra.h>
#include "Positions.h"
#include "Velocities.h"

class Flock {
    public: 
    Positions positions;
    Velocities velocities;
    Flock() {
        float POS_LO = STARTING_POSITION_LIMITS[0];
        float POS_HI = STARTING_POSITION_LIMITS[1];
        float VEL_LO = STARTING_VELOCITY_LIMITS[0];
        float VEL_HI = STARTING_VELOCITY_LIMITS[1];

        for (int i = 0; i < NBOIDS; i++) {
            // positions.xs(i) = random(STARTING_POSITION_LIMITS[0]);
            // positions.ys(i) = random(STARTING_POSITION_LIMITS[0]);
            positions.xs(i) = POS_LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(POS_HI-POS_LO)));
            positions.ys(i) = POS_LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(POS_HI-POS_LO)));
            positions.zs(i) = POS_LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(POS_HI-POS_LO)));
            velocities.dxs(i) = VEL_LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(VEL_HI-VEL_LO)));
            velocities.dys(i) = VEL_LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(VEL_HI-VEL_LO)));
            velocities.dzs(i) = VEL_LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(VEL_HI-VEL_LO)));
        }
    }

    void update() {
        BLA::Matrix<NBOIDS> directionToMiddleXs = positions.xs - getMean(positions.xs);
        BLA::Matrix<NBOIDS> directionToMiddleYs = positions.ys - getMean(positions.ys);
        BLA::Matrix<NBOIDS> directionToMiddleZs = positions.zs - getMean(positions.zs);

        BLA::Matrix<NBOIDS> tempDxChanges;
        BLA::Matrix<NBOIDS> tempDyChanges;
        BLA::Matrix<NBOIDS> tempDzChanges;
        tempDxChanges.Fill(0);
        tempDyChanges.Fill(0);
        tempDzChanges.Fill(0);

        tempDxChanges -= directionToMiddleXs * COHESION_STRENGTH;
        tempDyChanges -= directionToMiddleYs * COHESION_STRENGTH;
        tempDzChanges -= directionToMiddleZs * COHESION_STRENGTH;

        BLA::Matrix<NBOIDS, NBOIDS> velocityDiffsX; 
        BLA::Matrix<NBOIDS, NBOIDS> velocityDiffsY; 
        BLA::Matrix<NBOIDS, NBOIDS> velocityDiffsZ;
        velocityDiffsX.Fill(0);
        velocityDiffsY.Fill(0);
        velocityDiffsZ.Fill(0);

        for (int i = 0; i < NBOIDS; i++) {
            for (int j = 0; j < NBOIDS; j++) {
                if (i != j) {
                    float diffX = positions.xs(i) - positions.xs(j);
                    float diffY = positions.ys(i) - positions.ys(j);
                    float diffZ = positions.zs(i) - positions.zs(j);
                    float squaredDisplacement = pow(diffX, 2) + pow(diffY, 2) + pow(diffZ, 2);
                    if (squaredDisplacement <= SEPARATION_DISTANCE) {
                        tempDxChanges(i) += diffX * SEPARATION_STRENGTH;
                        tempDyChanges(i) += diffY * SEPARATION_STRENGTH;
                        tempDzChanges(i) += diffZ * SEPARATION_STRENGTH;
                    }
                    if (squaredDisplacement <= ALIGNMENT_DISTANCE) {
                        float diffdX = velocities.dxs(j) - velocities.dxs(i);
                        float diffdY = velocities.dys(j) - velocities.dys(i);
                        float diffdZ = velocities.dzs(j) - velocities.dzs(i);
                        velocityDiffsX(i, j) = diffdX;
                        velocityDiffsY(i, j) = diffdY;
                        velocityDiffsZ(i, j) = diffdZ;
                    }
                }
            }
        }
        for (int i = 0; i < NBOIDS; i++) {
            float totalVelocityDiffX = 0;
            float totalVelocityDiffY = 0;
            float totalVelocityDiffZ = 0;
            for (int j = 0; j < NBOIDS; j++) {
                totalVelocityDiffX += velocityDiffsX(i, j);
                totalVelocityDiffY += velocityDiffsY(i, j);
                totalVelocityDiffZ += velocityDiffsZ(i, j);
            }
            velocities.dxs(i) += tempDxChanges(i) + (totalVelocityDiffX / NBOIDS) * ALIGNMENT_STRENGTH;
            velocities.dys(i) += tempDyChanges(i) + (totalVelocityDiffY / NBOIDS) * ALIGNMENT_STRENGTH;
            velocities.dzs(i) += tempDzChanges(i) + (totalVelocityDiffZ / NBOIDS) * ALIGNMENT_STRENGTH;
        }

        for (int i = 0; i < NBOIDS; i++) {
            if (positions.xs(i) < MARGIN) {
                velocities.dxs(i) += TURNFACTOR;
            }
            else if (positions.xs(i) > NCOLS - MARGIN - 1) {
                velocities.dxs(i) -= TURNFACTOR;
            }
            if (positions.ys(i) < MARGIN) {
                velocities.dys(i) += TURNFACTOR;
            }
            else if (positions.ys(i) > NROWS - MARGIN - 1) {
                velocities.dys(i) -= TURNFACTOR;
            }
            if (positions.zs(i) < MARGIN) {
                velocities.dzs(i) += TURNFACTOR;
            }
            else if (positions.zs(i) > NLAYERS - MARGIN - 1) {
                velocities.dzs(i) -= TURNFACTOR;
            }
            float speed = sqrt(pow(velocities.dxs(i), 2) + pow(velocities.dys(i), 2) + pow(velocities.dzs(i), 2));
            if (speed > MAXSPEED) {
                velocities.dxs(i) = (velocities.dxs(i) / speed) * MAXSPEED;
                velocities.dys(i) = (velocities.dys(i) / speed) * MAXSPEED;
                velocities.dzs(i) = (velocities.dzs(i) / speed) * MAXSPEED;
            }
            if (speed < MINSPEED) {
                velocities.dxs(i) = (velocities.dxs(i) / speed) * MINSPEED;
                velocities.dys(i) = (velocities.dys(i) / speed) * MINSPEED;
                velocities.dzs(i) = (velocities.dzs(i) / speed) * MINSPEED;
            }
        }

        positions.xs += velocities.dxs;
        positions.ys += velocities.dys;
        positions.zs += velocities.dzs;

    }

    float getMean(BLA::Matrix<NBOIDS> values) {
        float total = 0.0;
        for (int i = 0; i < values.Rows; i++) {
            total += values(i);
        }
        return total / values.Rows;
    }
};

#endif