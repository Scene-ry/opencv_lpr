#include "GetLicenseFromImage.h"

void GetLicense(const cv::Mat& src, cv::Mat& dst, bool is_output_img, const char* output_img_path)
{
    //// gamma correction
    //cv::Mat src_gamma = src.clone();
    //uchar lut[256];
    //for (int i = 0; i < 256; i++)
    //{
    //    lut[i] = pow((float)(i / 255.0), 1 / 2.2) * 255.0;
    //}
    //for (cv::MatIterator_<cv::Vec3b> it = src_gamma.begin<cv::Vec3b>(); it != src_gamma.end<cv::Vec3b>(); ++it)
    //{
    //    (*it)[0] = lut[(*it)[0]];
    //    (*it)[1] = lut[(*it)[1]];
    //    (*it)[2] = lut[(*it)[2]];
    //}

    //if (is_output_img)
    //{
    //    std::string s_filename = std::string(output_img_path) + "_gamma.jpg";
    //    cv::imwrite(s_filename.c_str(), src_gamma);
    //}

    // convert to hsv
    cv::Mat src_hsv;
    cv::cvtColor(src, src_hsv, CV_BGR2HSV);

    // filter
    cv::Mat filter_min(src_hsv.size(), CV_8UC3, cv::Scalar(95, 80, 60));
    cv::Mat filter_max(src_hsv.size(), CV_8UC3, cv::Scalar(123, 255, 255));
    cv::Mat filter_between = (src_hsv >= filter_min) & (src_hsv <= filter_max);
    cv::Mat filter_hsv = cv::Mat::zeros(filter_between.size(), CV_8UC1);
    cv::cvtColor(filter_between, filter_hsv, CV_BGR2GRAY);
    cv::threshold(filter_hsv, filter_hsv, 254, 255, CV_THRESH_BINARY);

    if (is_output_img)
    {
        std::string s_filename = std::string(output_img_path) + "_hsvfilter.jpg";
        cv::imwrite(s_filename.c_str(), filter_hsv);
    }

    // blur + th
    cv::blur(filter_hsv, filter_hsv, cv::Size(10, 10));
    cv::threshold(filter_hsv, filter_hsv, WHITE_THRESHOLD, 255, CV_THRESH_BINARY);

    cv::dilate(filter_hsv, filter_hsv, cv::Mat(cv::Size(5, 5), CV_8U));
    //cv::erode(filter_hsv, filter_hsv, cv::Mat(cv::Size(5, 5), CV_8U));

    if (is_output_img)
    {
        std::string s_filename = std::string(output_img_path) + "_hsvfilter_dilate.jpg";
        cv::imwrite(s_filename.c_str(), filter_hsv);
    }

    std::vector<cv::Mat> contours;
    std::vector<std::pair<cv::Rect, int> > rects_with_area;
    cv::findContours(filter_hsv, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    for (std::vector<cv::Mat>::iterator it = contours.begin(); it != contours.end(); it++)
    {
        cv::Rect r = boundingRect(*it);

        double width_height_rate = r.width / (double)r.height;
        if (width_height_rate > 2.5 && width_height_rate < 4.5 && r.height > 20)
        {
            rects_with_area.push_back(std::make_pair(r, r.area()));
        }
    }

    if (rects_with_area.empty())
    {
        dst = src.clone();
        return;
    }

    std::sort(rects_with_area.begin(), rects_with_area.end(), [](const std::pair<cv::Rect, int>& p1, const std::pair<cv::Rect, int>& p2) { return p1.second > p2.second; });
    cv::Rect max_area_rect = rects_with_area.front().first;
    cv::Range rg_row, rg_col;
    rg_row.start = max_area_rect.y;
    rg_row.end = max_area_rect.y + max_area_rect.height;
    rg_col.start = max_area_rect.x;
    rg_col.end = max_area_rect.x + max_area_rect.width;
    dst = src(rg_row, rg_col);

    //// get edge
    //cv::Mat filter_edge;
    //cv::Canny(filter_hsv, filter_edge, 10, 90, 3);
    //
    //if (is_output_img)
    //{
    //    std::string s_filename = std::string(output_img_path) + "_canny.jpg";
    //    cv::imwrite(s_filename.c_str(), filter_edge);
    //}
    //
    //// construct histogram
    //int *vertical_hist = new int[filter_edge.rows];
    //int *horizontal_hist = new int[filter_edge.cols];
    //memset(vertical_hist, 0, sizeof(int) * filter_edge.rows);
    //memset(horizontal_hist, 0, sizeof(int) * filter_edge.cols);
    //for (cv::MatIterator_<uchar> it = filter_edge.begin<uchar>(); it != filter_edge.end<uchar>(); ++it)
    //{
    //    int pixel = *it;
    //    if (pixel >= WHITE_THRESHOLD)
    //    {
    //        vertical_hist[it.pos().y]++;
    //        horizontal_hist[it.pos().x]++;
    //    }
    //}
    //
    //// get average
    //int h_max = 0, h_min = INT_MAX, w_max = 0, w_min = INT_MAX;
    //for (int i = 0; i < filter_edge.rows; i++)
    //{
    //    if (vertical_hist[i] > h_max)
    //        h_max = vertical_hist[i];
    //    if (vertical_hist[i] < h_min)
    //        h_min = vertical_hist[i];
    //}
    //for (int i = 0; i < filter_edge.cols; i++)
    //{
    //    if (horizontal_hist[i] > w_max)
    //        w_max = horizontal_hist[i];
    //    if (horizontal_hist[i] < w_min)
    //        w_min = horizontal_hist[i];
    //}
    //
    //int h_threshold = (h_min + h_max) / 2, w_threshold = (w_min + w_max) / 2;
    //
    //// locate the plate
    //int h_start = -1, h_end = -1, w_start = -1, w_end = -1;
    //for (int h = 0; h < filter_edge.rows; h++)
    //{
    //    if (vertical_hist[h] >= h_threshold)
    //    {
    //        if (h_start == -1)
    //        {
    //            h_start = h;
    //            continue;
    //        }
    //        if (h_start != -1)
    //            h_end = h;
    //    }
    //}
    //for (int w = 0; w < filter_edge.cols; w++)
    //{
    //    if (horizontal_hist[w] >= w_threshold)
    //    {
    //        if (w_start == -1)
    //        {
    //            w_start = w;
    //            continue;
    //        }
    //        if (w_start != -1)
    //            w_end = w;
    //    }
    //}
    //
    //cv::Range rg_row, rg_col;
    //rg_row.start = h_start;
    //rg_row.end = h_end;
    //rg_col.start = w_start;
    //rg_col.end = w_end;
    //
    //dst = cv::Mat(src, rg_row, rg_col);
    //
    //delete[] vertical_hist;
    //delete[] horizontal_hist;
}
