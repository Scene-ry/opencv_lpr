#ifndef DEPTHVECTORHELPER_H
#define DEPTHVECTORHELPER_H

#include "../Common.h"
#include <vector>

class DepthVectorHelper
{
public:
    DepthVectorHelper(int, int);

    void AddSampleDepthVector(const Mat&, std::vector<double>&, std::vector<double>&, std::vector<double>&, std::vector<double>&);
    void AddSampleDepthVectorLeft(const Mat&, std::vector<double>&);
    void AddSampleDepthVectorRight(const Mat&, std::vector<double>&);
    void AddSampleDepthVectorUp(const Mat&, std::vector<double>&);
    void AddSampleDepthVectorDown(const Mat&, std::vector<double>&);

private:
    int height, width;
};

#endif // DEPTHVECTORHELPER_H
