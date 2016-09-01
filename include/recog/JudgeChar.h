#ifndef JUDGECHAR_H
#define JUDGECHAR_H

#include "GetCos.hpp"
#include "FeatureVectorHelper.h"
#include "../Common.h"

#ifdef __GET_EDGE_DATA__
#include "../resource/stddata_dim11.h"
#else
#include "../resource/stddata_dim9.h"
#endif

#include <map>
#include <vector>

using namespace cv;

class JudgeChar
{
public:
//    std::vector<double> StdCharVectorsLeft[36];
//    std::vector<double> StdCharVectorsRight[36];
//    std::vector<double> StdCharVectorsUp[36];
//    std::vector<double> StdCharVectorsDown[36];
    JudgeChar(const char*);
    std::string GetPossibleChars(const Mat&, char&);

private:
    FeatureVectorHelper fvec;
};

#endif // JUDGECHAR_H
