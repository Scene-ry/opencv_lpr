#include "DepthCalc.h"

void DepthCalc::AddDepthVector(const Mat& mat, std::vector<double>& result)
{
    int width = mat.cols;
    int height = mat.rows;

    // left
    int depth_sum = 0;
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            if ((int)mat.data[h * width + w] != 0)
            {
                depth_sum += w / (double)width;
                break;
            }
        }
    }
    result.push_back(depth_sum);

    // up
    depth_sum = 0;
    for (int w = 0; w < width; w++)
    {
        for (int h = 0; h < height; h++)
        {
            if ((int)mat.data[h * width + w] != 0)
            {
                depth_sum += h / (double)height;
                break;
            }
        }
    }
    result.push_back(depth_sum);

    // right
    depth_sum = 0;
    for (int h = 0; h < height; h++)
    {
        for (int w = width - 1; w >= 0; w--)
        {
            if ((int)mat.data[h * width + w] != 0)
            {
                depth_sum += (width - w) / (double)width;
                break;
            }
        }
    }
    result.push_back(depth_sum);

    // down
    depth_sum = 0;
    for (int w = 0; w < width; w++)
    {
        for (int h = height - 1; h >= 0; h--)
        {
            if ((int)mat.data[h * width + w] != 0)
            {
                depth_sum += (height - h) / (double)height;
                break;
            }
        }
    }
    result.push_back(depth_sum);
}
