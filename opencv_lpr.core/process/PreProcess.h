#ifndef PREPROCESS_H
#define PREPROCESS_H

#include "preprocess/GetLicenseFromImage.h"
#include "preprocess/LicenseCropper.h"

#include <algorithm>
#include <iostream>
#include <vector>

enum ProcessResult
{
    Success = 0,
    Failed  = -1
};

ProcessResult pre_process(const char*, std::vector<cv::Mat>&, bool, bool, const char*);

#endif // PREPROCESS_H
