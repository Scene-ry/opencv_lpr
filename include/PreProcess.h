#ifndef PREPROCESS_H
#define PREPROCESS_H

#include "thin/Thin.h"
#include "crop/LicenseCropper.hpp"
#include "Common.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

enum ProcessResult
{
    Success = 0,
    Failed  = -1
};

class PreProcess
{
public:
    //PreProcess(int, int);
    ProcessResult pre_process(const char*, const char*, const char*, bool);

private:
    Thin thinner;
    int char_max_width, char_max_height;
};

#endif // PREPROCESS_H
