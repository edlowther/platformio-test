#ifndef POSITIONS_H
#define POSITIONS_H

#include <BasicLinearAlgebra.h>

class Positions {
    public: 
    BLA::Matrix<NBOIDS> xs;
    BLA::Matrix<NBOIDS> ys;
    BLA::Matrix<NBOIDS> zs;
};

#endif
