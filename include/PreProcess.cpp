#include "PreProcess.h"

PreProcess::PreProcess(int w, int h) : char_max_width(w), char_max_height(h)
{
}

ProcessResult PreProcess::pre_process(const char* img_dir, const char* filename, const char* extname, bool toReverse)
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

    // binarize
    threshold(src_onechannel, src_onechannel, 125, 255, CV_THRESH_BINARY);

    // reverse if needed
    if (toReverse)
        bitwise_not(src_onechannel, src_onechannel);

    // dilate
    Mat src_dilate;// = src_onechannel.clone();
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    dilate(src_onechannel, src_dilate, kernel);
    //erode(tmp, tmp, 0);

    // thinning
    Mat src_thin;
    thinner.Thinning_2(src_dilate, src_thin);

    // get the contours
    std::vector<std::vector<Point> > contours;
    findContours(src_thin.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    std::cout << "Count of contours: " << contours.size() << std::endl;

    std::vector<Rect> rects;
    for (std::vector<std::vector<Point> >::iterator it = contours.begin(); it != contours.end(); it++)
    {
        Rect r = boundingRect(*it);
        // only store large contours
        if (r.width > char_max_width && r.height > char_max_height)
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

        Mat tmp(src_thin, rg_row, rg_col);

        Mat tmp_resize;
        if (tmp.cols < 5)
        {
            // deal with '1'
            tmp_resize = Mat::zeros(CROP_HEIGHT, CROP_WIDTH, CV_8UC1);
            for (int h = 0; h < CROP_HEIGHT; h++)
            {
                for (int w = 0; w < tmp.cols; w++)
                {
                    tmp_resize.at<uchar>(h, ((CROP_WIDTH - tmp.cols) / 2 + w)) = 255;
                }
            }
        }
        else
        {
            // normalization
            resize(tmp, tmp_resize, Size(CROP_WIDTH, CROP_HEIGHT));
        }

        threshold(tmp_resize, tmp_resize, WHITE_THRESHOLD, 255, CV_THRESH_BINARY);

        std::string s_filename = std::string(img_dir) + "crops/" + filename + "_cut_" + IntToString(i++) + ".jpg";
        imwrite(s_filename.c_str(), tmp_resize);

        std::cout << tmp_resize.channels() << std::endl;

        //rectangle(src_thin, r, Scalar(125), 1);
    }

    //imshow("original", src_dilate);
    //imshow("thinned", src_thin);

    //waitKey();

    return ProcessResult::Success;
}
