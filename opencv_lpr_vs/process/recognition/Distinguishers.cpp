#include "Distinguishers.h"

void Distinguish_0_8_D_Q(const cv::Mat& mat, char& recog)
{
    int white_area_count;
    int last_pixel;

    // horizontal
    for (int h = 0; h < mat.rows; h++)
    {
        white_area_count = 0;
        last_pixel = 0;
        for (int w = 0; w < mat.cols; w++)
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
            recog = 'Q';
            return;
        }
    }

    // vertical
    for (int w = 0; w < mat.cols; w++)
    {
        white_area_count = 0;
        last_pixel = 0;
        for (int h = 0; h < mat.rows; h++)
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
            recog = '8';
            return;
        }
    }
}
