#ifndef THIN_H
#define THIN_H


#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

using namespace cv;

class Thin
{
public:
    void Thinning_1(Mat &);
    void Thinning_2(const Mat&, Mat&);
};

#endif // THIN_H
