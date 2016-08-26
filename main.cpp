#include "thinning/thinning_1.cpp"
#include "thinning/thinning_2.cpp"

//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <vector>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

using namespace cv;

std::string IntToString(int num)
{
    std::string res;
    std::stringstream ss;
    ss << num;
    ss >> res;
    ss.clear();
    return res;
}

int main()
{
    Mat src = imread("./chepai.jpg");

    if (!src.data)
    {
        return -1;
    }

    // window
    namedWindow("window", CV_WINDOW_AUTOSIZE);

    // convert to gray
    cvtColor(src, src, CV_BGR2GRAY);

    // binarize
    threshold(src, src, 125, 255, CV_THRESH_BINARY);

    // reverse
    bitwise_not(src, src);

    std::vector<std::vector<Point> > contours;
    findContours(src.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    std::cout << contours.size() << std::endl;

    std::vector<Rect> rects;
    for (std::vector<std::vector<Point> >::iterator it = contours.begin(); it != contours.end(); it++)
    {
        Rect r = boundingRect(*it);
        rects.push_back(r);
    }

    std::sort(rects.begin(), rects.end(), [](Rect a, Rect b){ return a.x < b.x; });

    int i = 0;
    Mat src_clone;
    //chao_thinimage(src_clone);
    thinning(src, src_clone);
    for (std::vector<Rect>::iterator it = rects.begin(); it != rects.end(); it++)
    {
        Rect r = *it;
        if (r.width > 15 && r.height > 30)
        {
            Range rg_row, rg_col;
            rg_row.start = r.y;
            rg_row.end = r.y + r.height;
            rg_col.start = r.x;
            rg_col.end = r.x + r.width;
            Mat tmp(src_clone, rg_row, rg_col);

            //resize(tmp, tmp, Size(30, 60));

            //Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
            //erode(tmp, tmp, kernel);
            //dilate(tmp, tmp, 0);

            std::string s = std::string("./crops/cut_") + IntToString(i++) + ".jpg";
            imwrite(s.c_str(), tmp);
         }
         rectangle(src, r, Scalar(125), 1);
    }

    imshow("window", src);

    waitKey();

    return 0;
}
