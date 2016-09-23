#ifndef CHAR_EXCLUDERS_H
#define CHAR_EXCLUDERS_H

#include "../Common.h"
#include <cmath>
#include <map>

enum NumComparer
{
    Larger, Equal, Smaller
};

class CharExcluders
{
public:
    static void ByWhitePointsOnHorizontalMediumLine(const cv::Mat&, std::map<char, double>&, char, NumComparer, int);
    static void ByWhiteAreasOnVerticalMediumLine(const cv::Mat&, std::map<char, double>&, char, NumComparer, int);
    static void ByWhiteAreasOnHorizontal3_4Line(const cv::Mat&, std::map<char, double>&, char, NumComparer, int);
    static void ByOneWhiteAreaStartHorizontalLine(const cv::Mat&, std::map<char, double>&, char, NumComparer, int);

    static void ByWhitePixelUpDownDiff(const cv::Mat&, std::map<char, double>&, char, NumComparer, int);
    static void ByWhitePixelUp_LeftRightDiff(const cv::Mat&, std::map<char, double>&, char, NumComparer, int);
};

#endif
