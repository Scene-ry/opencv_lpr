#ifndef JUDGECHAR_H
#define JUDGECHAR_H

#include "GetCos.hpp"
#include "DepthVectorHelper.h"
#include "../Common.h"

#ifdef __GET_EDGE_DATA__
#include "../resource/std_depth_data_with_edge.h"
#else
#include "../resource/std_depth_data.h"
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
    std::map<char, double> GetPossibleChars(const Mat&, char&, char&);

private:
    DepthVectorHelper fvec;
};

#endif // JUDGECHAR_H
