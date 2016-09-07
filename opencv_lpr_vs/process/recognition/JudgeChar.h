#ifndef JUDGECHAR_H
#define JUDGECHAR_H

#include "GetCos.h"
#include "DepthVectorHelper.h"
#include "std_depth_data.h"

#include <map>

using namespace cv;

class JudgeChar
{
public:
    JudgeChar();
    std::map<char, double> GetPossibleChars(const Mat&, char&, char&);

private:
    DepthVectorHelper fvec;
};

#endif // JUDGECHAR_H
