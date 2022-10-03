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
            int z = round(positions.zs(i));
            int idx;
            if (z % 2 == 0) {
                if (x % 2 == 0) {
                    idx = z * NROWS * NCOLS + x * NROWS + y;
                } 
                else {
                    idx = z * NROWS * NCOLS + x * NROWS + (NROWS - y - 1);
                }
            }
            else {
                if (x % 2 == 0) {
                    idx = z * NROWS * NCOLS + NROWS * NCOLS - (x * NROWS + y + 1);
                } 
                else {
                    idx = z * NROWS * NCOLS + NROWS * NCOLS - (x * NROWS + (NROWS - y));
                }
            }
            output(idx) += BRIGHTNESS_INCREMENT;
            if (output(idx) > 255) {
                output(idx) = 255;
            }
        }
    }
};

#endif
