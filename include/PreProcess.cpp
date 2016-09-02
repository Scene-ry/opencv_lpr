#include "PreProcess.h"

PreProcess::PreProcess()
{
}

//void AddBlackEdge(const Mat& src, Mat& dst)
//{
//    int width = src.cols;
//    int height = src.rows;
//
//    dst = Mat::zeros(height + 2, width + 2, CV_8UC1);
//
//    for (int h = 0; h < height; h++)
//    {
//        for (int w = 0; w < width; w++)
//        {
//            dst.at<uchar>(h + 1, w + 1) = src.at<uchar>(h, w);
//        }
//    }
//}

void binarize_by_average(const Mat& src, Mat& dst)
{
    // find the proper binarize threshold
    int threshold_max = 0;
    int threshold_min = 255;
    for (int h = 0; h < src.rows; h++)
    {
        for (int w = 0; w < src.cols; w++)
        {
            int pixel = (int)src.at<uchar>(h, w);
            if (pixel < threshold_min)
                threshold_min = pixel;
            if (pixel > threshold_max)
                threshold_max = pixel;
        }
    }
    int bin_threshold = (threshold_min + threshold_max) / 2;

    threshold(src, dst, bin_threshold, 255, CV_THRESH_BINARY);
}

void binarize_by_histogram(const Mat& src, Mat& dst)
{
    int hist[256] = {0};
    int hist_gram[256][256];

    for (int row = 0; row < 256; row++)
    {
        for (int col = 0; col < 256; col++)
        {
            hist_gram[row][col] = 0;
        }
    }

    for (int h = 0; h < src.rows; h++)
    {
        for (int w = 0; w < src.cols; w++)
        {
            int pixel = (int)src.at<uchar>(h, w);
            hist[pixel]++;
        }
    }

    for (int i = 0; i < 256; i++)
    {
        std::cout << hist[i] << " ";
    }
    std::cout << "\n\n";

    for (int i = 0; i < 256; i++)
    {
        for (int j = 0; j < hist[i]; j++)
            hist_gram[i][j] = 1;
    }

    std::cout << "-------------------------------------------------\n";

    for (int row = 0; row < 256; row++)
    {
        for (int col = 0; col < 256; col++)
        {
            if (hist_gram[row][col] == 1)
            {
                std::cout << "-";
                continue;
            }
            break;
        }
        std::cout << "\n";
    }

    std::cout << "-------------------------------------------------\n";
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
    imwrite("/home/user/Desktop/opencv/opencv_ocr_test/images/crops/gray.jpg", src_onechannel);

#ifdef __GET_STD_CHAR_IMAGE__
    int bin_threshold = 140;
#else
    // find the proper binarize threshold
    int threshold_max = 0;
    int threshold_min = 255;
    for (int h = 0; h < src_onechannel.rows; h++)
    {
        for (int w = 0; w < src_onechannel.cols; w++)
        {
            int pixel = (int)src_onechannel.at<uchar>(h, w);
            if (pixel < threshold_min)
                threshold_min = pixel;
            if (pixel > threshold_max)
                threshold_max = pixel;
        }
    }
    int bin_threshold = (threshold_min + threshold_max) / 2;
#endif

    // binarize
    threshold(src_onechannel, src_onechannel, bin_threshold, 255, CV_THRESH_BINARY);

    // reverse if needed
    if (toReverse)
        bitwise_not(src_onechannel, src_onechannel);

    imwrite("/home/user/Desktop/opencv/opencv_ocr_test/images/crops/binary.jpg", src_onechannel);

#ifdef __GET_STD_CHAR_IMAGE__
    std::string std_filename = std::string(img_dir) + "crops/" + filename + ".jpg";
    imwrite(std_filename.c_str(), src_onechannel);
    return ProcessResult::Success;
#endif
    // crop
    Mat src_crop;
    LicenseCropper(src_onechannel, src_crop);
    //imwrite("/home/user/Desktop/opencv/opencv_ocr_test/images/crops/test_crop.jpg", src_crop);

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
        //imwrite(s_filename.c_str(), tmp_resize);

        //std::cout << tmp_resize.channels() << std::endl;

        //rectangle(src_thin, r, Scalar(125), 1);
    }

    //imshow("original", src_dilate);
    //imshow("thinned", src_thin);

    //waitKey();

    return ProcessResult::Success;
}
