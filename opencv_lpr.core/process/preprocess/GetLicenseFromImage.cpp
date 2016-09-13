#include "GetLicenseFromImage.h"

void GetLicense(const cv::Mat& src, cv::Mat& dst)
{
    int h_threshold = 20, w_threshold = 20;

    // convert to gray
    cv::Mat src_hsv;
    cv::cvtColor(src, src_hsv, CV_BGR2HSV);

    // filter
    cv::Mat filter_hsv = cv::Mat::zeros(src_hsv.size(), CV_8UC1);
    for (cv::MatIterator_<cv::Vec3b> it = src_hsv.begin<cv::Vec3b>(); it != src_hsv.end<cv::Vec3b>(); ++it)
    {
        double pixel_h = (*it)[0] * 2.0;
        double pixel_s = (*it)[1] / 255.0;
        double pixel_v = (*it)[2] / 255.0;

        if (pixel_h >= 190 && pixel_h <= 245 && pixel_s >= 0.35 && pixel_v >= 0.3)
        {
            filter_hsv.at<uchar>(it.pos()) = 255;
        }
    }

    // blur
    //cv::blur(filter_hsv, filter_hsv, cv::Size(9, 9));

    // get edge
    cv::Mat filter_edge;
    cv::Canny(filter_hsv, filter_edge, 10, 90, 3);
    //cv::imwrite("../../opencv_lpr.core/samples/crops/canny.jpg", filter_edge);

    // construct histogram
    int *vertical_hist = new int[filter_edge.rows];
    int *horizontal_hist = new int[filter_edge.cols];
    memset(vertical_hist, 0, sizeof(int) * filter_edge.rows);
    memset(horizontal_hist, 0, sizeof(int) * filter_edge.cols);
    for (cv::MatIterator_<uchar> it = filter_edge.begin<uchar>(); it != filter_edge.end<uchar>(); ++it)
    {
        int pixel = *it;
        if (pixel >= WHITE_THRESHOLD)
        {
            vertical_hist[it.pos().y]++;
            horizontal_hist[it.pos().x]++;
        }
    }

    // locate the plate
    int h_start = -1, h_end = -1, w_start = -1, w_end = -1;
    for (int h = 0; h < filter_edge.rows; h++)
    {
        if (vertical_hist[h] >= h_threshold)
        {
            if (h_start == -1)
            {
                h_start = h;
                continue;
            }
            if (h_start != -1)
                h_end = h;
        }
    }
    for (int w = 0; w < filter_edge.cols; w++)
    {
        if (horizontal_hist[w] >= w_threshold)
        {
            if (w_start == -1)
            {
                w_start = w;
                continue;
            }
            if (w_start != -1)
                w_end = w;
        }
    }

    cv::Range rg_row, rg_col;
    rg_row.start = h_start;
    rg_row.end = h_end;
    rg_col.start = w_start;
    rg_col.end = w_end;

    dst = cv::Mat(src, rg_row, rg_col);

    delete[] vertical_hist;
    delete[] horizontal_hist;
}
