#ifndef JUDGECHAR_H
#define JUDGECHAR_H

#include "GetCos.hpp"
#include "FeatureVectorHelper.h"
#include "../Common.h"

#include "Distinguishers.hpp"

#include <vector>

using namespace cv;

class JudgeChar
{
public:
    std::vector<double> StdCharVectorsLeft[36];
    std::vector<double> StdCharVectorsRight[36];
    std::vector<double> StdCharVectorsUp[36];
    std::vector<double> StdCharVectorsDown[36];
    JudgeChar(const char*);
    std::vector<char> GetChar(const Mat&);

private:
    FeatureVectorHelper fvec;
};

#endif // JUDGECHAR_H
