#include "PreProcess.h"

int GetMeanThreshold(int* hist_gram)
{
    int sum = 0, amount = 0;
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

    //cv::MatND histogram;
    //const int histSize = 256;
    //float range[] = { 0, 255 };
    //const float *ranges[] = { range };
    //const int channels = 0;
    //cv::calcHist(&src, 1, &channels, cv::Mat(), histogram, 1, &histSize, &ranges[0], true, false);

    for (cv::MatConstIterator_<uchar> it = src.begin<uchar>(); it != src.end<uchar>(); ++it)
    {
        int pixel = *it;
        // only available for white chars
        if (pixel > 50)
            hist[pixel]++;
    }

    return GetMeanThreshold(hist);
}

void reverse_if_needed(cv::Mat& src)
{
    cv::Mat cmp = src >= cv::Mat(src.size(), CV_8UC1, cv::Scalar(WHITE_THRESHOLD));
    int white_count = cv::countNonZero(cmp);

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

    // Get the License
    cv::Mat src_license;
    GetLicense(src, src_license, is_output_img, output_img_path);
    if (is_output_img)
    {
        std::string s_filename = std::string(output_img_path) + "_license.jpg";
        cv::imwrite(s_filename.c_str(), src_license);
    }
    //return ProcessResult::Failed; // end here for test

    // convert to 1-channel
    cv::Mat src_onechannel = cv::Mat::zeros(src_license.size(), CV_8UC1);

    // convert to gray
    cv::cvtColor(src_license, src_onechannel, CV_BGR2GRAY);
    //imwrite("./samples/crops/gray.jpg", src_onechannel);

    // Enhancement (not using any yet)
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

    if (is_output_img)
    {
        std::string s_filename = std::string(output_img_path) + "_binary.jpg";
        cv::imwrite(s_filename.c_str(), src_enhance);
    }

    if (src_enhance.rows >= 45)
    {
        cv::erode(src_enhance, src_enhance, cv::Mat(cv::Size(3, 3), CV_8U));

        if (is_output_img)
        {
            std::string s_filename = std::string(output_img_path) + "_erode.jpg";
            cv::imwrite(s_filename.c_str(), src_enhance);
        }

        // remove small areas
        std::vector<cv::Mat> contours;
        cv::findContours(src_enhance.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
        for (std::vector<cv::Mat>::iterator it = contours.begin(); it != contours.end(); it++)
        {
            cv::Rect r = boundingRect(*it);

            if (r.height < src_enhance.rows / 3)
            {
                for (int h = r.y; h < r.y + r.height; h++)
                {
                    for (int w = r.x; w < r.x + r.width; w++)
                    {
                        src_enhance.at<uchar>(h, w) = 0;
                    }
                }
            }
        }

        cv::dilate(src_enhance, src_enhance, cv::Mat(cv::Size(3, 3), CV_8U));
        //cv::dilate(src_enhance, src_enhance, cv::Mat(cv::Size(3, 3), CV_8U));
        //cv::erode(src_enhance, src_enhance, cv::Mat(cv::Size(3, 3), CV_8U));
    }

    if (is_output_img)
    {
        std::string s_filename = std::string(output_img_path) + "_openclose.jpg";
        cv::imwrite(s_filename.c_str(), src_enhance);
    }

    // crop
    cv::Mat src_crop;
    LicenseCropper(src_enhance, src_crop);
    //imwrite("./samples/crops/test_crop.jpg", src_crop);

    if (is_output_img)
    {
        std::string s_filename = std::string(output_img_path) + "_cropped.jpg";
        cv::imwrite(s_filename.c_str(), src_crop);
    }

    // get the contours
    std::vector<cv::Mat> contours;
    cv::findContours(src_crop.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    if (is_cout)
        std::cout << "Count of contours: " << contours.size() << std::endl;

    std::vector<cv::Rect> rects;
    for (std::vector<cv::Mat>::iterator it = contours.begin(); it != contours.end(); it++)
    {
        cv::Rect r = boundingRect(*it);
        // only store large contours
        double h_w_rate = r.height / (double)r.width;
        if (r.height >= src_crop.rows / 2 && h_w_rate >= 1.2 && r.x / (double)src_crop.cols < 0.95)
        {
            rects.push_back(r);
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
