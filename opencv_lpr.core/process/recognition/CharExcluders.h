#ifndef CHAR_EXCLUDERS_H
#define CHAR_EXCLUDERS_H

#include "../Common.h"
#include <map>

class CharExcluders
{
public:
    static void JudgeByHorizontalMediumLine(const cv::Mat&, std::map<char, double>&, char);
};

#endif
