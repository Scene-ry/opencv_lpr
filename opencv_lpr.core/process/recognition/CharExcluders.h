#ifndef CHAR_EXCLUDERS_H
#define CHAR_EXCLUDERS_H

#include "../Common.h"
#include <map>

class CharExcluders
{
public:
    static void ByWhitePointsOnHorizontalMediumLine(const cv::Mat&, std::map<char, double>&, char);
    static void ByWhiteAreasOnVerticalMediumLine(const cv::Mat&, std::map<char, double>&, char);
    static void ByWhiteAreasOnHorizontal3_4Line(const cv::Mat&, std::map<char, double>&, char);
    static void ByWhiteAreasOnAllHorizontalLine(const cv::Mat&, std::map<char, double>&, char);

    static void ByWhitePixelUpDownRate(const cv::Mat&, std::map<char, double>&, char);
};

#endif
