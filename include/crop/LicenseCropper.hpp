#ifndef LICENSECROPPER_HPP_INCLUDED
#define LICENSECROPPER_HPP_INCLUDED

#include "../Common.h"

static void LicenseCropper(const Mat& src, Mat& dst)
{
    int width = src.cols, height = src.rows;
    int h_start = 0, h_end = height, w_start = 0, w_end = width;

    int white_area_count;
    int last_pixel;

    for (int h = 0; h < height; h++)
    {
        white_area_count = 0;
        last_pixel = 0;
        for (int w = 0; w < width; w++)
        {
            int pixel = (int)src.at<uchar>(h, w);
            if (pixel >= WHITE_THRESHOLD && last_pixel < WHITE_THRESHOLD)
            {
                white_area_count++;
            }
            last_pixel = pixel;
        }

        if (white_area_count > 2 && h_start == 0)
        {
            h_start = h;
        }
        if (white_area_count <= 2 && h_start != 0)
        {
            h_end = h;
            break;
        }
    }

    Range rg_row, rg_col;
    rg_row.start = h_start;
    rg_row.end = h_end;
    rg_col.start = w_start;
    rg_col.end = w_end;

    dst = Mat(src, rg_row, rg_col);
    //imwrite("/home/user/Desktop/opencv/opencv_ocr_test/images/crops/test.jpg", dst);
}

#endif // LICENSECROPPER_HPP_INCLUDED
