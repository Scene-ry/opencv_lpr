#ifndef PREPROCESS_H
#define PREPROCESS_H

#include "thin/Thin.h"

//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <vector>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#define CHAR_MAX_WIDTH  15
#define CHAR_MAX_HEIGHT 30
#define RESIZE_WIDTH    20
#define RESIZE_HEIGHT   40

enum ProcessResult
{
    Success = 0,
    Failed  = -1
};

class PreProcess
{
public:
    std::string IntToString(int);
    ProcessResult pre_process(const char*, const char*, bool);

private:
    Thin thinner;
};

#endif // PREPROCESS_H
