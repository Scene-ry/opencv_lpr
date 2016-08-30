#include "PreProcess.h"

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

ProcessResult PreProcess::pre_process(const char* img_dir, const char* filename, const char* extname, bool toReverse)
{
    std::string s_filename = std::string(img_dir) + filename + extname;
    Mat src = imread(s_filename.c_str());

    if (!src.data)
    {
        return ProcessResult::Failed;
    }

    // window
    //namedWindow("window", CV_WINDOW_AUTOSIZE);

    // convert to gray
    cvtColor(src, src, CV_BGR2GRAY);

    // binarize
    threshold(src, src, 125, 255, CV_THRESH_BINARY);

    // reverse
    if (toReverse)
        bitwise_not(src, src);

    // get the contours
    std::vector<std::vector<Point> > contours;
    findContours(src.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    std::cout << "Count of contours: " << contours.size() << std::endl;

    std::vector<Rect> rects;
    for (std::vector<std::vector<Point> >::iterator it = contours.begin(); it != contours.end(); it++)
    {
        Rect r = boundingRect(*it);
        // only store large contours
        if (r.width > CHAR_MAX_WIDTH && r.height > CHAR_MAX_HEIGHT)
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

        rg_row.start = r.y;
        rg_row.end = r.y + r.height - 1;
        rg_col.start = r.x;
        rg_col.end = r.x + r.width - 1;

        Mat tmp(src, rg_row, rg_col);

        // normalization
        Mat tmp_resize;
        resize(tmp, tmp_resize, Size(CROP_WIDTH - 2, CROP_HEIGHT - 2));

        // add black edge
        Mat tmp_edge;
        AddBlackEdge(tmp_resize, tmp_edge);

        // thinning
        Mat tmp_thin;
        thinner.Thinning_2(tmp_edge, tmp_thin);

        //Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
        //erode(tmp, tmp, kernel);
        //dilate(tmp, tmp, 0);

        std::string s_filename = std::string(img_dir) + "crops/" + filename + "_cut_" + IntToString(i++) + ".jpg";
        imwrite(s_filename.c_str(), tmp_thin);

        //rectangle(src, r, Scalar(125), 1);
    }

    //imshow("window", src);

    //waitKey();

    return ProcessResult::Success;
}
