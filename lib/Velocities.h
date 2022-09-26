#ifndef VELOCITIES_H
#define VELOCITIES_H

#include <BasicLinearAlgebra.h>

class Velocities {
    public: 
    BLA::Matrix<NBOIDS> dxs;
    BLA::Matrix<NBOIDS> dys;
    BLA::Matrix<NBOIDS> dzs;
};

#endif
