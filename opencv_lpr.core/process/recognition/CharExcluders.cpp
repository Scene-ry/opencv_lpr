#include "CharExcluders.h"

void CharExcluders::ByWhitePointsOnHorizontalMediumLine(const cv::Mat& mat, std::map<char, double>& cos_result, char to_remove)
{
    int white_pixel_points = 0;
    int height = mat.rows, width = mat.cols;

    for (int w = 0; w < width; w++)
    {
        int pixel = mat.at<uchar>(height / 2, w);
        if (pixel >= WHITE_THRESHOLD)
            white_pixel_points++;
    }

    if (white_pixel_points >= 18 && cos_result.find(to_remove) != cos_result.end())
        cos_result.erase(to_remove);
}

void CharExcluders::ByWhiteAreasOnVerticalMediumLine(const cv::Mat& mat, std::map<char, double>& cos_result, char to_remove)
{
    int height = mat.rows, width = mat.cols;

    int white_area_count = 0;
    int last_pixel = 0;
    for (int h = 0; h < height; h++)
    {
        int pixel = mat.at<uchar>(h, width / 2);
        if (pixel >= WHITE_THRESHOLD && last_pixel < WHITE_THRESHOLD)
            white_area_count++;
        last_pixel = pixel;
    }

    if (white_area_count < 3 && cos_result.find(to_remove) != cos_result.end())
        cos_result.erase(to_remove);
}

void CharExcluders::ByWhiteAreasOnHorizontal3_4Line(const cv::Mat& mat, std::map<char, double>& cos_result, char to_remove)
{
    int height = mat.rows, width = mat.cols;

    int white_area_count = 0;
    int last_pixel = 0;
    for (int w = 0; w < width; w++)
    {
        int pixel = mat.at<uchar>(height * 3 / 4, w);
        if (pixel >= WHITE_THRESHOLD && last_pixel < WHITE_THRESHOLD)
            white_area_count++;
        last_pixel = pixel;
    }

    if (white_area_count > 1 && cos_result.find(to_remove) != cos_result.end())
        cos_result.erase(to_remove);
}

void CharExcluders::ByWhiteAreasOnAllHorizontalLine(const cv::Mat& mat, std::map<char, double>& cos_result, char to_remove)
{
    int height = mat.rows, width = mat.cols;

    int h_with_one_white_area_start = 0;

    int white_area_count;
    int last_pixel;
    for (int h = 0; h < height; h++)
    {
        white_area_count = 0;
        last_pixel = 0;
        for (int w = 0; w < width; w++)
        {
            int pixel = mat.at<uchar>(h, w);
            if (pixel >= WHITE_THRESHOLD && last_pixel < WHITE_THRESHOLD)
                white_area_count++;
            last_pixel = pixel;
        }

        if (white_area_count < 2)
        {
            h_with_one_white_area_start = h;
            break;
        }
    }

    if (h_with_one_white_area_start > 30 && cos_result.find(to_remove) != cos_result.end())
        cos_result.erase(to_remove);
}

void CharExcluders::ByWhitePixelUpDownRate(const cv::Mat& mat, std::map<char, double>& cos_result, char to_remove)
{
    int height = mat.rows, width = mat.cols;
    int up_count = 0, down_count = 0;

    for (cv::MatConstIterator_<uchar> it = mat.begin<uchar>(); it != mat.end<uchar>(); ++it)
    {
        int pixel = *it;
        if (pixel >= WHITE_THRESHOLD)
        {
            if (it.pos().y < height / 2)
                up_count++;
            else
                down_count++;
        }
    }

    if (up_count - down_count < 10 && cos_result.find(to_remove) != cos_result.end())
        cos_result.erase(to_remove);
}

void CharExcluders::ByWhitePixelUp_LeftRightRate(const cv::Mat& mat, std::map<char, double>& cos_result, char to_remove, bool isLarger)
{
    int height = mat.rows, width = mat.cols;
    int left_count = 0, right_count = 0;

    for (cv::MatConstIterator_<uchar> it = mat.begin<uchar>(); it != mat.end<uchar>(); ++it)
    {
        if (it.pos().y >= mat.rows / 2)
            break;

        int pixel = *it;
        if (pixel >= WHITE_THRESHOLD)
        {
            if (it.pos().x < width / 2)
                left_count++;
            else
                right_count++;
        }
    }

    if (cos_result.find(to_remove) != cos_result.end())
    {
        if (isLarger)
        {
            if (abs(left_count - right_count) > 10)
                cos_result.erase(to_remove);
        }
        else
        {
            if (abs(left_count - right_count) < 10)
                cos_result.erase(to_remove);
        }

    }
}
