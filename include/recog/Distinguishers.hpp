#ifndef ZEROQDISTINGUISHER_HPP
#define ZEROQDISTINGUISHER_HPP

#include "../Common.h"

static char DistinguishZeroQ(const Mat& mat)
{
    int col_pixel_count;
    for (int w = 0; w < mat.cols; w++)
    {
        col_pixel_count = 0;
        for (int h = 0; h < mat.rows; h++)
        {
            if ((int)mat.data[h * mat.cols + w] >= 125)
            {
                col_pixel_count++;
            }
        }

        if (col_pixel_count == 3)
        {
            return 'Q';
        }
    }
    return '0';
}

#endif
