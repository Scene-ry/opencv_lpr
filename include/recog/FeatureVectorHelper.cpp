#include "FeatureVectorHelper.h"

FeatureVectorHelper::FeatureVectorHelper(int w, int h) : width(w), height(h)
{
}

void FeatureVectorHelper::AddSampleDepthVectorLeft(const Mat& mat, std::vector<double>& result)
{
    int h_incr = height / SAMPLE_COUNT;
    bool isPushed;

#ifdef __GET_EDGE_DATA__
    for (int h = 0; h <= height; h += h_incr)
    {
        if (h >= height)
            h = height - 1;
#else
    for (int h = h_incr; h < height; h += h_incr)
    {
#endif // __GET_EDGE_DATA__

        isPushed = false;
        for (int w = 0; w < width; w++)
        {
            if ((int)mat.at<uchar>(h, w) >= WHITE_THRESHOLD)
            {
                result.push_back(w + 1);
                isPushed = true;
                break;
            }
        }
        if (!isPushed)
        {
            result.push_back(0);
        }
    }
}

void FeatureVectorHelper::AddSampleDepthVectorRight(const Mat& mat, std::vector<double>& result)
{
    int h_incr = height / SAMPLE_COUNT;
    bool isPushed;

#ifdef __GET_EDGE_DATA__
    for (int h = 0; h <= height; h += h_incr)
    {
        if (h >= height)
            h = height - 1;
#else
    for (int h = h_incr; h < height; h += h_incr)
    {
#endif // __GET_EDGE_DATA__

        isPushed = false;
        for (int w = width - 1; w >= 0; w--)
        {
            if ((int)mat.at<uchar>(h, w) >= WHITE_THRESHOLD)
            {
                result.push_back(width - w);
                isPushed = true;
                break;
            }
        }
        if (!isPushed)
        {
            result.push_back(0);
        }
    }
}

void FeatureVectorHelper::AddSampleDepthVectorUp(const Mat& mat, std::vector<double>& result)
{
    int w_incr = width / SAMPLE_COUNT;
    bool isPushed;

#ifdef __GET_EDGE_DATA__
    for (int w = 0; w <= width; w += w_incr)
    {
        if (w >= width)
            w = width - 1;
#else
    for (int w = w_incr; w < width; w += w_incr)
    {
#endif // __GET_EDGE_DATA__

        isPushed = false;
        for (int h = 0; h < height; h++)
        {
            if ((int)mat.at<uchar>(h, w) >= WHITE_THRESHOLD)
            {
                result.push_back(h + 1);
                isPushed = true;
                break;
            }
        }
        if (!isPushed)
        {
            result.push_back(0);
        }
    }
}

void FeatureVectorHelper::AddSampleDepthVectorDown(const Mat& mat, std::vector<double>& result)
{
    int w_incr = width / SAMPLE_COUNT;
    bool isPushed;

#ifdef __GET_EDGE_DATA__
    for (int w = 0; w <= width; w += w_incr)
    {
        if (w >= width)
            w = width - 1;
#else
    for (int w = w_incr; w < width; w += w_incr)
    {
#endif // __GET_EDGE_DATA__

        isPushed = false;
        for (int h = height - 1; h >= 0; h--)
        {
            if ((int)mat.at<uchar>(h, w) >= WHITE_THRESHOLD)
            {
                result.push_back(height - h);
                isPushed = true;
                break;
            }
        }
        if (!isPushed)
        {
            result.push_back(0);
        }
    }
}
