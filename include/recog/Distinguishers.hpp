#ifndef ZEROQDISTINGUISHER_HPP
#define ZEROQDISTINGUISHER_HPP

#include "../Common.h"

static char Distinguish_0_8_Q(const Mat& mat)
{
    int white_area_count;
    int last_pixel;

    bool maybeQ = false;

    for (int w = 0; w < mat.cols; w++)
    {
        white_area_count = 0;
        last_pixel = 0;
        for (int h = 0; h < mat.rows; h++)
        {
            int pixel = (int)mat.at<uchar>(h, w * 3);
            if (pixel >= 125 && last_pixel < 125)
            {
                white_area_count++;
            }
            last_pixel = pixel;
        }

        if (white_area_count == 4)
        {
            return '8';
        }
        if (white_area_count == 3)
        {
            maybeQ = true;
        }
    }
    if (maybeQ)
    {
        return 'Q';
    }
    return '0';
}

#endif
