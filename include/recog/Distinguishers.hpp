#ifndef ZEROQDISTINGUISHER_HPP
#define ZEROQDISTINGUISHER_HPP

#include "../Common.h"

static char Distinguish_0_8_B_Q(const Mat& mat)
{
    int width = mat.cols, height = mat.rows;
    int white_area_count;
    int last_pixel;

    bool maybeQ = false;

    for (int w = 0; w < width; w++)
    {
        white_area_count = 0;
        last_pixel = 0;
        for (int h = 0; h < height; h++)
        {
            int pixel = (int)mat.at<uchar>(h, w);
            if (pixel >= WHITE_THRESHOLD && last_pixel < WHITE_THRESHOLD)
            {
                white_area_count++;
            }
            last_pixel = pixel;
        }

        if (white_area_count == 4)
        {
            // may be 'B' or '8'
            int last_h = -1, last_w = -1;
            double last_slope;
            double slope_diff_sum = 0;
            bool last_slope_exist = false;

            for (int h2 = 0; h2 < height; h2++)
            {
                for (int w2 = 0; w2 < width; w2++)
                {
                    int pixel = (int)mat.at<uchar>(h2, w2);
                    if (pixel >= WHITE_THRESHOLD)
                    {
                        if (last_h != -1)
                        {
                            double k = (w2 - last_w) / (double)(h2 - last_h);
                            if (last_slope_exist)
                            {
                                slope_diff_sum += std::fabs(k - last_slope);
                            }
                            last_slope_exist = true;
                            last_slope = k;
                        }
                        last_h = h2;
                        last_w = w2;
                        break;
                    }
                }
            }
            //std::cout << slope_diff_sum << std::endl;
            if (slope_diff_sum < 3)
                return 'B';
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

static char Distinguish_2_Z(const Mat& mat)
{
    int width = mat.cols, height = mat.rows;
    int last_h = -1, last_w = -1;
    double last_slope;
    double slope_diff_sum = 0;
    bool last_slope_exist = false;

    for (int w = 0; w < width; w++)
    {
        for (int h = 0; h < height; h++)
        {
            int pixel = (int)mat.at<uchar>(h, w);
            if (pixel >= WHITE_THRESHOLD)
            {
                if (last_h != -1)
                {
                    double k = (h - last_h) / (double)(w - last_w);
                    if (last_slope_exist)
                    {
                        slope_diff_sum += std::fabs(k - last_slope);
                    }
                    last_slope_exist = true;
                    last_slope = k;
                }
                last_h = h;
                last_w = w;
                break;
            }
        }
    }

    //std::cout << slope_diff_sum << std::endl;
    if (slope_diff_sum < 3)
        return 'Z';

    return '2';
}

static char Distinguish_E_F_H_L_T(const Mat& mat)
{
    int height = mat.rows, width = mat.cols;

    // find 'T'
    for (int w = 0; w < width; w++)
    {
        int pixel = (int)mat.at<uchar>(height / 2, w);
        if (pixel >= WHITE_THRESHOLD)
        {
            if (std::fabs(width / 2 - w) < 5)
            {
                return 'T';
            }
        }
    }

    // find 'H'
    for (int h = 0; h < height; h++)
    {
        int pixel = (int)mat.at<uchar>(h, (width / 2));
        if (pixel >= WHITE_THRESHOLD)
        {
            if (std::fabs(height / 2 - h) < 5)
            {
                return 'H';
            }
        }
    }

    // find 'E'
    int white_area_count;
    int last_pixel;
    for (int w = 0; w < width; w++)
    {
        white_area_count = 0;
        last_pixel = 0;
        for (int h = 0; h < height; h++)
        {
            int pixel = (int)mat.at<uchar>(h, w);
            if (pixel >= WHITE_THRESHOLD && last_pixel < WHITE_THRESHOLD)
            {
                white_area_count++;
            }
            last_pixel = pixel;
        }

        if (white_area_count == 3)
        {
            return 'E';
        }
    }

    // find 'F'
    for (int h = 0; h < height; h++)
    {
        int pixel = (int)mat.at<uchar>(h, (width / 2));
        if (pixel >= WHITE_THRESHOLD)
        {
            if (h < 5)
            {
                return 'F';
            }
        }
    }

    return 'L';
}

static char Distinguish_7_J(const Mat& mat)
{
    int height = mat.rows, width = mat.cols;

    // find '7'
    for (int w = 0; w < width; w++)
    {
        int pixel = (int)mat.at<uchar>(height / 2, w);
        if (pixel >= WHITE_THRESHOLD)
        {
            if (std::fabs(width / 2 - w) < 5)
            {
                return '7';
            }
        }
    }

    return 'J';
}

#endif
