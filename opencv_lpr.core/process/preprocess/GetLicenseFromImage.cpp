#include "GetLicenseFromImage.h"

void GetLicense(const cv::Mat& src, cv::Mat& dst)
{
    // convert to gray
    cv::Mat src_hsv;
    cv::cvtColor(src, src_hsv, CV_BGR2HSV);

    // filter
    cv::Mat filter_hsv = cv::Mat::zeros(src_hsv.size(), CV_8UC1);
    for (int h = 0; h < src_hsv.rows; h++)
    {
        for (int w = 0; w < src_hsv.cols; w++)
        {
            double pixel_h = src_hsv.at<cv::Vec3b>(h, w)[0] * 2.0;
            double pixel_s = src_hsv.at<cv::Vec3b>(h, w)[1] / 255.0;
            double pixel_v = src_hsv.at<cv::Vec3b>(h, w)[2] / 255.0;

            if (pixel_h >= 190 && pixel_h <= 245 && pixel_s >= 0.35 && pixel_v >= 0.3)
            {
                filter_hsv.at<uchar>(h, w) = 255;
            }
        }
    }

    // blur
    //cv::blur(filter_hsv, filter_hsv, cv::Size(9, 9));

    // get edge
    cv::Canny(filter_hsv, dst, 10, 90, 3);

    for (cv::MatIterator_<uchar> it = filter_hsv.begin<uchar>(); it != filter_hsv.end<uchar>(); ++it)
    {
        
    }
}
