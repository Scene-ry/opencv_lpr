#include "CharExcluders.h"

void CharExcluders::ByWhitePointsOnHorizontalMediumLine(const cv::Mat& mat, std::map<char, double>& cos_result, char to_remove, NumComparer comp, int pixel_count)
{
    int white_pixel_points = 0;
    int height = mat.rows, width = mat.cols;

    for (int w = 0; w < width; w++)
    {
        int pixel = mat.at<uchar>(height / 2, w);
        if (pixel >= WHITE_THRESHOLD)
            white_pixel_points++;
    }

    if (cos_result.find(to_remove) != cos_result.end())
    {
        switch (comp)
        {
        case NumComparer::Larger:
            if (white_pixel_points > pixel_count)
                cos_result.erase(to_remove);
            break;
        case NumComparer::Equal:
            if (white_pixel_points == pixel_count)
                cos_result.erase(to_remove);
            break;
        case NumComparer::Smaller:
            if (white_pixel_points < pixel_count)
                cos_result.erase(to_remove);
            break;
        }
    }
}

void CharExcluders::ByWhiteAreasOnVerticalMediumLine(const cv::Mat& mat, std::map<char, double>& cos_result, char to_remove, NumComparer comp, int area_count)
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

    if (cos_result.find(to_remove) != cos_result.end())
    {
        switch (comp)
        {
        case NumComparer::Larger:
            if (white_area_count > area_count)
                cos_result.erase(to_remove);
            break;
        case NumComparer::Equal:
            if (white_area_count == area_count)
                cos_result.erase(to_remove);
            break;
        case NumComparer::Smaller:
            if (white_area_count < area_count)
                cos_result.erase(to_remove);
            break;
        }
    }
}

void CharExcluders::ByWhiteAreasOnHorizontal3_4Line(const cv::Mat& mat, std::map<char, double>& cos_result, char to_remove, NumComparer comp, int area_count)
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

    if (cos_result.find(to_remove) != cos_result.end())
    {
        switch (comp)
        {
        case NumComparer::Larger:
            if (white_area_count > area_count)
                cos_result.erase(to_remove);
            break;
        case NumComparer::Equal:
            if (white_area_count == area_count)
                cos_result.erase(to_remove);
            break;
        case NumComparer::Smaller:
            if (white_area_count < area_count)
                cos_result.erase(to_remove);
            break;
        }
    }
}

void CharExcluders::ByOneWhiteAreaStartHorizontalLine(const cv::Mat& mat, std::map<char, double>& cos_result, char to_remove, NumComparer comp, int line_height)
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

    if (cos_result.find(to_remove) != cos_result.end())
    {
        switch (comp)
        {
        case NumComparer::Larger:
            if (h_with_one_white_area_start > line_height)
                cos_result.erase(to_remove);
            break;
        case NumComparer::Equal:
            if (h_with_one_white_area_start == line_height)
                cos_result.erase(to_remove);
            break;
        case NumComparer::Smaller:
            if (h_with_one_white_area_start < line_height)
                cos_result.erase(to_remove);
            break;
        }
    }
}

void CharExcluders::ByWhitePixelUpDownDiff(const cv::Mat& mat, std::map<char, double>& cos_result, char to_remove, NumComparer comp, int threshold)
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

    if (cos_result.find(to_remove) != cos_result.end())
    {
        switch (comp)
        {
        case NumComparer::Larger:
            if (up_count - down_count > threshold)
                cos_result.erase(to_remove);
            break;
        case NumComparer::Equal:
            if (up_count - down_count == threshold)
                cos_result.erase(to_remove);
            break;
        case NumComparer::Smaller:
            if (up_count - down_count < threshold)
                cos_result.erase(to_remove);
            break;
        }
    }
}

void CharExcluders::ByWhitePixelUp_LeftRightDiff(const cv::Mat& mat, std::map<char, double>& cos_result, char to_remove, NumComparer comp, int threshold)
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
        switch (comp)
        {
        case NumComparer::Larger:
            if (left_count - right_count > threshold)
                cos_result.erase(to_remove);
            break;
        case NumComparer::Equal:
            if (left_count - right_count == threshold)
                cos_result.erase(to_remove);
            break;
        case NumComparer::Smaller:
            if (left_count - right_count < threshold)
                cos_result.erase(to_remove);
            break;
        }
    }
}
