#ifndef THIN_H
#define THIN_H

#include "../Common.h"
#include <vector>

using namespace cv;

class Thin
{
public:
    void Thinning_1(Mat &);
    void Thinning_2(const Mat&, Mat&);
};

#endif // THIN_H
