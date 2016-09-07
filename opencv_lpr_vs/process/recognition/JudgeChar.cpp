#include "JudgeChar.h"

JudgeChar::JudgeChar() : fvec(CROP_WIDTH, CROP_HEIGHT)
{
}

std::map<char, double> JudgeChar::GetPossibleChars(const cv::Mat& mat, char& recommend, char& recog)
{
    std::map<char, double> res;
    if (mat.cols == 1 && mat.rows == 1)
    {
        recommend = '1';
        res.insert(std::make_pair('1', 1.0));
        return res;
    }

    std::map<char, double> char_cos_values;

    std::vector<double> mat_vec_left, mat_vec_right, mat_vec_up, mat_vec_down;
    fvec.AddSampleDepthVector(mat, mat_vec_left, mat_vec_right, mat_vec_up, mat_vec_down);

    double max_cos_value = 0;

    for (int i = 0; i < 36; i++)
    {
        if (i == 1 || i == 18 || i == 24)
            continue;

        double cos_value_left = GetVectorCos(mat_vec_left, StdCharVectorsLeft[i]);
        double cos_value_right = GetVectorCos(mat_vec_right, StdCharVectorsRight[i]);
        double cos_value_up = GetVectorCos(mat_vec_up, StdCharVectorsUp[i]);
        double cos_value_down = GetVectorCos(mat_vec_down, StdCharVectorsDown[i]);

        double cos_value = (cos_value_left + cos_value_up + cos_value_right + cos_value_down) / 4.0;
        if (i >= 0 && i <= 9)
        {
            char_cos_values.insert(std::make_pair('0' + i, cos_value));
            if (cos_value > max_cos_value)
            {
                recommend = '0' + i;
                max_cos_value = cos_value;
            }
            if (cos_value >= SIMILAR_THRESHOLD)
                res.insert(std::make_pair('0' + i, cos_value));
        }
        else
        {
            char_cos_values.insert(std::make_pair('A' + i - 10, cos_value));
            if (cos_value > max_cos_value)
            {
                recommend = 'A' + i - 10;
                max_cos_value = cos_value;
            }
            if (cos_value >= SIMILAR_THRESHOLD)
                res.insert(std::make_pair('A' + i - 10, cos_value));
        }
    }

    // increase threshold until a char matches
    max_cos_value = 0;
    for (double lower_th = SIMILAR_THRESHOLD - 0.01; res.size() <= 0; lower_th -= 0.01)
    {
        for (std::map<char, double>::iterator it = char_cos_values.begin(); it != char_cos_values.end(); it++)
        {
            if (it->second > max_cos_value)
            {
                recommend = it->first;
                max_cos_value = it->second;
            }
            if (it->second >= lower_th)
                res.insert(std::make_pair(it->first, it->second));
        }
    }

    // distinguish '0' and '8'
    if (res.find('0') != res.end() && res.find('8') != res.end()
        && (recommend == '0' || recommend == '8'))
    {
        int white_area_count = 0;
        int last_pixel = 0;
        for (int h = 0; h < mat.rows; h++)
        {
            int pixel = (int)mat.at<uchar>(h, CROP_WIDTH / 2);
            if (pixel >= WHITE_THRESHOLD && last_pixel < WHITE_THRESHOLD)
            {
                white_area_count++;
            }
            last_pixel = pixel;
        }

        if (white_area_count == 3)
        {
            recog = '8';
        }
        else if (white_area_count == 2)
            recog = '0';
    }

    // distinguish '0' and 'Q'
    if (res.find('0') != res.end() && res.find('Q') != res.end()
        && (recommend == '0' || recommend == 'Q'))
    {
        double down_last_value = 0;
        int selected_col = mat.cols - 2;
        for (int h = mat.rows - 1; h >= 0; h--)
        {
            int pixel = mat.at<uchar>(h, selected_col);
            if (pixel >= WHITE_THRESHOLD)
            {
                down_last_value = mat.rows - h - 1;
                break;
            }
        }
        
        //double down_last_value = mat_vec_down.at(mat_vec_down.size() - 1);
        if (down_last_value > 1)
            recog = '0';
        else
            recog = 'Q';
    }

    return res;
}
