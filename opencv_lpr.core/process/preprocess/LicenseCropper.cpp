#include "LicenseCropper.h"

void LicenseCropper(const cv::Mat& src, cv::Mat& dst)
{
    int width = src.cols, height = src.rows;
    int h_start = 0, h_end = height, w_start = 0, w_end = width;

    int white_area_count;
    int last_pixel;

    bool up_edge_found = false;
    for (int h = 0; h < height / 2; h++)
    {
        white_area_count = 0;
        last_pixel = 0;
        for (int w = 0; w < width; w++)
        {
            int pixel = src.at<uchar>(h, w);
            if (pixel >= WHITE_THRESHOLD && last_pixel < WHITE_THRESHOLD)
            {
                white_area_count++;
            }
            last_pixel = pixel;
        }

        if (white_area_count > 5 && !up_edge_found)
        {
            h_start = h;
            up_edge_found = true;
        }
        else if (white_area_count <= 5)
        {
            up_edge_found = false;
        }
    }

    bool down_edge_found = false;
    for (int h = height - 1; h >= height / 2; h--)
    {
        white_area_count = 0;
        last_pixel = 0;
        for (int w = 0; w < width; w++)
        {
            int pixel = src.at<uchar>(h, w);
            if (pixel >= WHITE_THRESHOLD && last_pixel < WHITE_THRESHOLD)
            {
                white_area_count++;
            }
            last_pixel = pixel;
        }

        if (white_area_count > 5 && !down_edge_found)
        {
            h_end = h + 1;
            if (h_end > height)
                h_end = height;
            down_edge_found = true;
        }
        else if (white_area_count <= 5)
        {
            down_edge_found = false;
        }
    }

    // crop up & down
    cv::Range rg_row, rg_col;
    rg_row.start = h_start;
    rg_row.end = h_end;
    cv::Mat up_down_crop = src(rg_row, cv::Range::all());
    int up_down_crop_width = up_down_crop.cols, up_down_crop_height = up_down_crop.rows;

    // prevent left & right edges (test)
    w_start += 5;
    w_end -= 5;

    // prevent right edges (test)
    for (int w = w_end; w >= up_down_crop_width / 10 * 9; w--)
    {
        bool is_column_black = true;
        for (int h = 0; h < up_down_crop_height; h++)
        {
            int pixel = up_down_crop.at<uchar>(h, w);
            if (pixel >= WHITE_THRESHOLD)
            {
                is_column_black = false;
                break;
            }
        }

        if (is_column_black && up_down_crop_width > (up_down_crop_width - w - 1) * 10)
        {
            w_end = w;
            break;
        }
    }
    
    rg_col.start = w_start;
    rg_col.end = w_end;

    dst = up_down_crop(cv::Range::all(), rg_col);
    //imwrite("/home/user/Desktop/opencv/opencv_ocr_test/images/crops/test.jpg", dst);
}
