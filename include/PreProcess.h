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
    PreProcess();
    ProcessResult pre_process(const char*, const char*, const char*);

private:
    Thin thinner;
    int bin_threshold;
};

#endif // PREPROCESS_H
