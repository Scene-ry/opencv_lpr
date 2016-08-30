#ifndef PREPROCESS_H
#define PREPROCESS_H

#include "thin/Thin.h"
#include "Common.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#define CHAR_MAX_WIDTH  15
#define CHAR_MAX_HEIGHT 30

enum ProcessResult
{
    Success = 0,
    Failed  = -1
};

class PreProcess
{
public:
    ProcessResult pre_process(const char*, const char*, const char*, bool);

private:
    Thin thinner;
};

#endif // PREPROCESS_H
