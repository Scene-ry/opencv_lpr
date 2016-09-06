#include "PreProcess.h"

int GetMeanThreshold(int* hist_gram, int& max_pixel)
{
    int sum = 0, amount = 0, max_count = 0;
    for (int i = 0; i <256; i++)
    {
        amount += hist_gram[i];
        sum += i * hist_gram[i];

        if (hist_gram[i] > max_count)
        {
            max_pixel = i;
            max_count = hist_gram[i];
        }
    }
    return sum / amount;
}

int binarize_by_histogram(const Mat& src, int& max_pixel)
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

    return GetMeanThreshold(hist, max_pixel);
}


ProcessResult pre_process(const char* img_path, std::vector<Mat>& split_chars, bool is_output_img, bool is_cout, const char* output_img_path)
{
    //std::string s_filename = std::string(img_dir) + filename + extname;
    Mat src = imread(img_path);

    if (!src.data)
    {
        return ProcessResult::Failed;
    }

    // convert to 1-channel
    Mat src_onechannel = Mat::zeros(src.size(), CV_8UC1);

    // convert to gray
    cvtColor(src, src_onechannel, CV_BGR2GRAY);
    //imwrite("/home/user/Desktop/opencv/opencv_ocr_test/images/crops/gray.jpg", src_onechannel);

    int max_count_pixel;
    GaussianBlur(src_onechannel, src_onechannel, Size(3, 3), 0, 0);
    int bin_threshold = binarize_by_histogram(src_onechannel, max_count_pixel);

    if (is_cout)
        std::cout << "Threshold: " << bin_threshold << std::endl;

    // binarize
    threshold(src_onechannel, src_onechannel, bin_threshold, 255, CV_THRESH_BINARY);

    // reverse if needed
    if (bin_threshold < max_count_pixel)
        bitwise_not(src_onechannel, src_onechannel);

    //imwrite("/home/user/Desktop/opencv/opencv_ocr_test/images/crops/binary.jpg", src_onechannel);

    // crop
    Mat src_crop;
    LicenseCropper(src_onechannel, src_crop);
    //imwrite("/home/user/Desktop/opencv/opencv_ocr_test/images/crops/test_crop.jpg", src_crop);

    // get the contours
    int char_max_width = 1;
    int char_max_height = src_crop.rows / 2;
    std::vector<std::vector<Point> > contours;
    findContours(src_crop.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    if (is_cout)
        std::cout << "Count of contours: " << contours.size() << std::endl;

    std::vector<Rect> rects;
    for (std::vector<std::vector<Point> >::iterator it = contours.begin(); it != contours.end(); it++)
    {
        Rect r = boundingRect(*it);
        // only store large contours
        if (r.width >= char_max_width && r.height >= char_max_height)
        {
            rects.push_back(r);
        }
    }

    if (is_cout)
        std::cout << "Count of rects available: " << rects.size() << std::endl;

    // sort contours from left to right
    std::sort(rects.begin(), rects.end(), [](Rect a, Rect b){ return a.x < b.x; });

    int i = 0;
    for (std::vector<Rect>::iterator it = rects.begin(); it != rects.end(); it++)
    {
        Rect r = *it;

        Range rg_row, rg_col;

        //std::cout << r.y << " " << r.height << " " << r.x << " " << r.width << std::endl;

        rg_row.start = r.y;
        rg_row.end = r.y + r.height;
        rg_col.start = r.x;
        rg_col.end = r.x + r.width;

        Mat tmp(src_crop, rg_row, rg_col);

        Mat tmp_resize;
        //std::cout << (tmp.rows / (double)tmp.cols) << std::endl;
        if (tmp.rows / (double)tmp.cols > 2.5)
        {
            // deal with '1'
            resize(tmp, tmp_resize, Size(1, 1));
        }
        else
        {
            // normalization
            resize(tmp, tmp_resize, Size(CROP_WIDTH, CROP_HEIGHT));
        }

        threshold(tmp_resize, tmp_resize, WHITE_THRESHOLD, 255, CV_THRESH_BINARY);

        if (is_output_img)
        {
            //std::string s_filename = std::string(img_dir) + "crops/" + filename + "_cut_" + IntToString(i++) + ".jpg";
            std::string s_filename = std::string(output_img_path) + "/cut_" + IntToString(i++) + ".jpg";
            imwrite(s_filename.c_str(), tmp_resize);
        }
        split_chars.push_back(tmp_resize);

        //std::cout << tmp_resize.channels() << std::endl;
    }

    return ProcessResult::Success;
}
