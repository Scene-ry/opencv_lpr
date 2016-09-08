#include "PreProcess.h"

int GetMeanThreshold(int* hist_gram)
{
    int sum = 0, amount = 0, max_count = 0;
    for (int i = 0; i <256; i++)
    {
        amount += hist_gram[i];
        sum += i * hist_gram[i];
    }
    return sum / amount;
}

int binarize_by_histogram(const cv::Mat& src)
{
    int hist[256] = {0};

    for (int h = 0; h < src.rows; h++)
    {
        for (int w = 0; w < src.cols; w++)
        {
            int pixel = (int)src.at<uchar>(h, w);
            hist[pixel]++;
        }
    }

    return GetMeanThreshold(hist);
}

void reverse_if_needed(cv::Mat& src)
{
    int white_count = 0;
    for (int h = 0; h < src.rows; h++)
    {
        for (int w = 0; w < src.cols; w++)
        {
            int pixel = (int)src.at<uchar>(h, w);
            if (pixel >= WHITE_THRESHOLD)
                white_count++;
        }
    }

    if (white_count / (double)(src.rows * src.cols) > 0.5)
        cv::bitwise_not(src, src);
}

ProcessResult pre_process(const char* img_path, std::vector<cv::Mat>& split_chars, bool is_output_img, bool is_cout, const char* output_img_path)
{
    //std::string s_filename = std::string(img_dir) + filename + extname;
    cv::Mat src = cv::imread(img_path);

    if (!src.data)
    {
        return ProcessResult::Failed;
    }

    // convert to 1-channel
    cv::Mat src_onechannel = cv::Mat::zeros(src.size(), CV_8UC1);

    // convert to gray
    cv::cvtColor(src, src_onechannel, CV_BGR2GRAY);
    //imwrite("./samples/crops/gray.jpg", src_onechannel);

    // Enhancement
    cv::Mat src_enhance;
    src_enhance = src_onechannel;
    //cv::equalizeHist(src_onechannel, src_enhance);
    //cv::bilateralFilter(src_onechannel, src_enhance, 9, 18, 4);
    //cv::medianBlur(src_onechannel, src_enhance, 3);
    //src_onechannel.convertTo(src_enhance, -1, 2.0, 50);

    //imwrite("./samples/crops/enhance.jpg", src_enhance);

    // Gaussian Blur
    cv::GaussianBlur(src_enhance, src_enhance, cv::Size(3, 3), 0, 0);

    // Get the proper threshold value
    int bin_threshold = binarize_by_histogram(src_enhance);

    if (is_cout)
        std::cout << "Threshold: " << bin_threshold << std::endl;

    // binarize
    cv::threshold(src_enhance, src_enhance, bin_threshold, 255, CV_THRESH_BINARY);

    // reverse if needed
    reverse_if_needed(src_enhance);

    //imwrite("./samples/crops/binary.jpg", src_enhance);

    // crop
    cv::Mat src_crop;
    LicenseCropper(src_enhance, src_crop);
    //imwrite("./samples/crops/test_crop.jpg", src_crop);

    // get the contours
    int char_max_width = 1;
    int char_max_height = src_crop.rows / 2;
    std::vector<cv::Mat> contours;
    findContours(src_crop.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    if (is_cout)
        std::cout << "Count of contours: " << contours.size() << std::endl;

    std::vector<cv::Rect> rects;
    for (std::vector<cv::Mat>::iterator it = contours.begin(); it != contours.end(); it++)
    {
        cv::Rect r = boundingRect(*it);
        // only store large contours
        if (r.width >= char_max_width && r.height >= char_max_height)
        {
            rects.push_back(r);
        }
        // remove small areas
        else
        {
            for (int h = r.y; h < r.y + r.height; h++)
            {
                for (int w = r.x; w < r.x + r.width; w++)
                {
                    src_crop.at<uchar>(h, w) = 0;
                }
            }
        }
    }

    if (is_cout)
        std::cout << "Count of rects available: " << rects.size() << std::endl;

    // sort contours from left to right
    std::sort(rects.begin(), rects.end(), [](cv::Rect a, cv::Rect b){ return a.x < b.x; });

    int i = 0;
    for (std::vector<cv::Rect>::iterator it = rects.begin(); it != rects.end(); it++)
    {
        cv::Rect r = *it;

        cv::Range rg_row, rg_col;

        //std::cout << r.y << " " << r.height << " " << r.x << " " << r.width << std::endl;

        rg_row.start = r.y;
        rg_row.end = r.y + r.height;
        rg_col.start = r.x;
        rg_col.end = r.x + r.width;

        cv::Mat tmp(src_crop, rg_row, rg_col);

        cv::Mat tmp_resize;
        //std::cout << (tmp.rows / (double)tmp.cols) << std::endl;
        if (tmp.rows / (double)tmp.cols > 2.5)
        {
            // deal with '1'
            cv::resize(tmp, tmp_resize, cv::Size(1, 1));
        }
        else
        {
            // normalization
            cv::resize(tmp, tmp_resize, cv::Size(CROP_WIDTH, CROP_HEIGHT));
        }

        cv::threshold(tmp_resize, tmp_resize, WHITE_THRESHOLD, 255, CV_THRESH_BINARY);

        if (is_output_img)
        {
            //std::string s_filename = std::string(img_dir) + "crops/" + filename + "_cut_" + IntToString(i++) + ".jpg";
            std::string s_filename = std::string(output_img_path) + "_cut_" + IntToString(i++) + ".jpg";
            cv::imwrite(s_filename.c_str(), tmp_resize);
        }
        split_chars.push_back(tmp_resize);

        //std::cout << tmp_resize.channels() << std::endl;
    }

    return ProcessResult::Success;
}
