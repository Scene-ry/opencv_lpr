#include "Distinguishers.h"

void Distinguish_0_8_Q(const cv::Mat& mat, char& recog)
{
    int white_area_count;
    int last_pixel;
    int height = mat.rows, width = mat.cols;

    // horizontal
    for (int h = height - 1; h >= height / 2; h--)
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
    for (int w = width / 2 - 2; w < width / 2 + 2; w++)
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

    recog = '0';
}

void Distinguish_5_S(const cv::Mat& mat, char& recog)
{
    int close_to_edge_count = 0;

    for (int w = 0; w < mat.cols; w++)
    {
        for (int h = 0; h < mat.rows; h++)
        {
            int pixel = (int)mat.at<uchar>(h, w);
            if (pixel >= WHITE_THRESHOLD)
            {
                if (h == 0)
                    close_to_edge_count++;
                break;
            }
        }
    }

    if (CROP_WIDTH - close_to_edge_count < 5)
    {
        recog = '5';
    }
    else
    {
        recog = 'S';
    }
}