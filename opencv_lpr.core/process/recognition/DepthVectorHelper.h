#ifndef DEPTHVECTORHELPER_H
#define DEPTHVECTORHELPER_H

#include "../Common.h"
#include <vector>

class DepthVectorHelper
{
public:
    DepthVectorHelper(int, int);

    void AddSampleDepthVector(const cv::Mat&, std::vector<double>&, std::vector<double>&, std::vector<double>&, std::vector<double>&);
    void AddSampleDepthVectorLeft(const cv::Mat&, std::vector<double>&);
    void AddSampleDepthVectorRight(const cv::Mat&, std::vector<double>&);
    void AddSampleDepthVectorUp(const cv::Mat&, std::vector<double>&);
    void AddSampleDepthVectorDown(const cv::Mat&, std::vector<double>&);

private:
    int width, height;
};

#endif // DEPTHVECTORHELPER_H
