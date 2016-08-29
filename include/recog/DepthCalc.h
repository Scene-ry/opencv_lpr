#ifndef DEPTHCALC_H
#define DEPTHCALC_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

using namespace cv;

class DepthCalc
{
public:
    void AddDepthVector(const Mat&, std::vector<double>&);
};

#endif // DEPTHCALC_H
