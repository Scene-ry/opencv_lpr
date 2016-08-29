#ifndef SLOPECALC_H
#define SLOPECALC_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

using namespace cv;

class SlopeCalc
{
public:
    void AddSlopeVector(const Mat&, std::vector<double>&);
};

#endif // SLOPECALC_H
