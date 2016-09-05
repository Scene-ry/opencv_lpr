#include "PreProcess.h"

PreProcess::PreProcess()
{
}

void AddBlackEdge(const Mat& src, Mat& dst)
{
    int width = src.cols;
    int height = src.rows;

    dst = Mat::zeros(height + 2, width + 2, CV_8UC1);

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            dst.at<uchar>(h + 1, w + 1) = src.at<uchar>(h, w);
        }
    }
}

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


ProcessResult PreProcess::pre_process(const char* img_dir, const char* filename, const char* extname)
{
    std::string s_filename = std::string(img_dir) + filename + extname;
    Mat src = imread(s_filename.c_str());

    if (!src.data)
    {
        return ProcessResult::Failed;
    }

    // window
    //namedWindow("original", CV_WINDOW_AUTOSIZE);
    //namedWindow("thinned", CV_WINDOW_AUTOSIZE);

    // convert to 1-channel
    Mat src_onechannel = Mat::zeros(src.size(), CV_8UC1);

    // convert to gray
    cvtColor(src, src_onechannel, CV_BGR2GRAY);
    imwrite("/home/user/Desktop/opencv/opencv_ocr_test/images/crops/gray.jpg", src_onechannel);

#ifdef __GET_STD_CHAR_IMAGE__
    int bin_threshold = 140;
#else
    int max_count_pixel;
    GaussianBlur(src_onechannel, src_onechannel, Size(3, 3), 0, 0);
    int bin_threshold = binarize_by_histogram(src_onechannel, max_count_pixel);
#endif

    std::cout << "Threshold: " << bin_threshold << std::endl;

    // binarize
    threshold(src_onechannel, src_onechannel, bin_threshold, 255, CV_THRESH_BINARY);

    // reverse if needed
    if (bin_threshold < max_count_pixel)
        bitwise_not(src_onechannel, src_onechannel);

    imwrite("/home/user/Desktop/opencv/opencv_ocr_test/images/crops/binary.jpg", src_onechannel);

#ifdef __GET_STD_CHAR_IMAGE__
    Mat src_onechannel_edge;
    AddBlackEdge(src_onechannel, src_onechannel_edge);
    int h_start = -1, h_end = -1, w_start = -1, w_end = -1;

    for (int h = 0; h < src_onechannel_edge.rows; h++)
    {
        bool white_found = false;
        for (int w = 0; w < src_onechannel_edge.cols; w++)
        {
            if (src_onechannel_edge.at<uchar>(h, w) >= WHITE_THRESHOLD)
            {
                white_found = true;
                break;
            }
        }

        if (white_found && h_start == -1)
        {
            h_start = h;
        }
        if (!white_found && h_start != -1 && h_end == -1)
        {
            h_end = h;
        }
    }

    for (int w = 0; w < src_onechannel_edge.cols; w++)
    {
        bool white_found = false;
        for (int h = 0; h < src_onechannel_edge.rows; h++)
        {
            if (src_onechannel_edge.at<uchar>(h, w) >= WHITE_THRESHOLD)
            {
                white_found = true;
                break;
            }
        }

        if (white_found && w_start == -1)
        {
            w_start = w;
        }
        if (!white_found && w_start != -1 && w_end == -1)
        {
            w_end = w;
        }
    }
    Range rg_row, rg_col;
    rg_row.start = h_start;
    rg_row.end = h_end;
    rg_col.start = w_start;
    rg_col.end = w_end;

    Mat cr = Mat(src_onechannel_edge, rg_row, rg_col);
    Mat rs;
    resize(cr, rs, Size(CROP_WIDTH, CROP_HEIGHT));

    threshold(rs, rs, WHITE_THRESHOLD, 255, CV_THRESH_BINARY);

    std::string std_filename = std::string(img_dir) + "crops/" + filename + ".jpg";
    imwrite(std_filename.c_str(), rs);
    return ProcessResult::Success;
#endif
    // crop
    Mat src_crop;
    LicenseCropper(src_onechannel, src_crop);
    imwrite("/home/user/Desktop/opencv/opencv_ocr_test/images/crops/test_crop.jpg", src_crop);

    // dilate
    //Mat src_dilate;// = src_crop.clone();
    //Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    //dilate(src_crop, src_dilate, kernel);
    //imwrite("/home/user/Desktop/opencv/opencv_ocr_test/images/crops/test_dilate.jpg", src_dilate);
    //erode(tmp, tmp, 0);


    // add edge
    //Mat src_dilate_addedge;
    //AddBlackEdge(src_dilate, src_dilate_addedge);
    //imwrite("/home/user/Desktop/opencv/opencv_ocr_test/images/crops/test.jpg", src_dilate_addedge);

    // thinning-1
    //Mat src_thin = src_dilate_addedge.clone();
    //thinner.Thinning_1(src_thin);

    // thinning-2
    //Mat src_thin;
    //thinner.Thinning_2(src_dilate_addedge, src_thin);
    //imwrite("/home/user/Desktop/opencv/opencv_ocr_test/images/crops/test_thin.jpg", src_thin);

    // crop
    //Mat src_thin_crop;
    //LicenseCropper(src_thin, src_thin_crop);
    //imwrite("/home/user/Desktop/opencv/opencv_ocr_test/images/crops/test.jpg", src_thin_crop);

    // get the contours
    int char_max_width = 1;
    int char_max_height = src_crop.rows / 2;
    std::vector<std::vector<Point> > contours;
    findContours(src_crop.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

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

        std::string s_filename = std::string(img_dir) + "crops/" + filename + "_cut_" + IntToString(i++) + ".jpg";
        imwrite(s_filename.c_str(), tmp_resize);

        //std::cout << tmp_resize.channels() << std::endl;

        //rectangle(src_thin, r, Scalar(125), 1);
    }

    //imshow("original", src_dilate);
    //imshow("thinned", src_thin);

    //waitKey();

    return ProcessResult::Success;
}
