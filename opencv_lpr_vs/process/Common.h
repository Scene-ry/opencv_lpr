#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <sstream>

#define CROP_WIDTH        20
#define CROP_HEIGHT       40

#define SAMPLE_COUNT      10

#define WHITE_THRESHOLD   80
#define SIMILAR_THRESHOLD 0.88   // 0.88 recommended

std::string IntToString(int);

#endif // COMMON_H_INCLUDED
