#ifndef JUDGECHAR_H
#define JUDGECHAR_H

#include "GetCos.hpp"
#include "SlopeCalc.h"
#include "DepthCalc.h"
#include "../PreProcess.h"
#include <vector>
#include <string>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

class JudgeChar
{
public:
    std::vector<double> StdCharVectors[36];
    JudgeChar(const char*);
    char GetChar(const Mat&);

private:
    SlopeCalc sc;
    DepthCalc dc;
};

#endif // JUDGECHAR_H
