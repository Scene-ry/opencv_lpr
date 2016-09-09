#include "CharExcluders.h"

void CharExcluders::JudgeByHorizontalMediumLine(const cv::Mat& mat, std::map<char, double>& cos_result, char to_remove)
{
    int white_pixel_points = 0;
    int height = mat.rows, width = mat.cols;

    for (int w = 0; w < width; w++)
    {
        int pixel = mat.at<uchar>(height / 2, w);
        if (pixel >= WHITE_THRESHOLD)
            white_pixel_points++;
    }

    if (white_pixel_points > 18)
        cos_result.erase(to_remove);
}
