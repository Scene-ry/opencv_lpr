#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>

using namespace cv;

#define CROP_WIDTH        20
#define CROP_HEIGHT       40
#define WHITE_THRESHOLD   80
#define SIMILAR_THRESHOLD 0.95

static std::string IntToString(int num)
{
    std::string res;
    std::stringstream ss;
    ss << num;
    ss >> res;
    ss.clear();
    return res;
}


#endif // COMMON_H_INCLUDED
