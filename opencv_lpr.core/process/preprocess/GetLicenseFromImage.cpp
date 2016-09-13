#include "GetLicenseFromImage.h"

void GetLicense(const cv::Mat& src, cv::Mat& dst)
{
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
    cv::Canny(filter_hsv, dst, 10, 90, 3);

}
