#ifndef CROPPER_HPP_INCLUDED
#define CROPPER_HPP_INCLUDED

#include "../Common.h"

class Cropper
{
public:
    static void crop(const Mat& src, Mat& dst)
    {
        int width = src.cols, height = src.rows;
        int h_start = -1, h_end = -1, w_start = -1, w_end = -1;

        for (int h = 0; h < height; h++)
        {
            int w;
            for (w = 0; w < width; w++)
            {
                if ((int)src.at<uchar>(h, w) >= WHITE_THRESHOLD)
                {
                    if (h_start == -1)
                        h_start = h;
                    break;
                }
            }

            if (w >= width && h_start != -1)
            {
                h_end = h;
                break;
            }
        }

        for (int w = 0; w < width; w++)
        {
            int h;
            for (h = 0; h < height; h++)
            {
                if ((int)src.at<uchar>(h, w) >= WHITE_THRESHOLD)
                {
                    if (w_start == -1)
                        w_start = w;
                    break;
                }
            }

            if (h >= height && w_start != -1)
            {
                w_end = w;
                break;
            }
        }

        Range rg_row, rg_col;

        rg_row.start = h_start;
        rg_row.end = h_end;
        rg_col.start = w_start;
        rg_col.end = w_end;

        dst = Mat(src, rg_row, rg_col);
    }
};

#endif // CROPPER_HPP_INCLUDED
