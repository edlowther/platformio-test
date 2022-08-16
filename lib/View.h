#ifndef VIEW_H
#define VIEW_H

#include <BasicLinearAlgebra.h>
#include "Positions.h"

class View {
    public: 
    BLA::Matrix<NPIXELS> output; 
    View() {
        output.Fill(0);
    }
    void map(Positions & positions) {
        output.Fill(0);
        for (int i = 0; i < NBOIDS; i++) {
            int x = round(positions.xs(i));
            int y = round(positions.ys(i));
            int idx;
            if (x % 2 == 0) {
                idx = x * NROWS + y;
            } 
            else {
                idx = x * NROWS + (NROWS - y - 1);
            }
            output(idx) += BRIGHTNESS_INCREMENT;
        }
    }
};

#endif
