#include "JudgeChar.h"

JudgeChar::JudgeChar(const char* img_dir) : fvec(CROP_WIDTH, CROP_HEIGHT)
{
//    for (int i = 0; i <= 9; i++)
//    {
//        std::string std_img_file = std::string(img_dir) + "standard/20x40/" + IntToString(i) + ".jpg";
//        Mat std_img = imread(std_img_file.c_str());
//
//        fvec.AddSampleDepthVectorLeft(std_img, StdCharVectorsLeft[i]);
//        fvec.AddSampleDepthVectorRight(std_img, StdCharVectorsRight[i]);
//        fvec.AddSampleDepthVectorUp(std_img, StdCharVectorsUp[i]);
//        fvec.AddSampleDepthVectorDown(std_img, StdCharVectorsDown[i]);
//    }
//
//    for (int i = 10; i < 36; i++)
//    {
//        char ch = 'A' + i - 10;
//
//        if (ch == 'O' || ch == 'I')
//        {
//            continue;
//        }
//
//        std::string std_img_file = std::string(img_dir) + "standard/20x40/" + ch + ".jpg";
//        Mat std_img = imread(std_img_file.c_str());
//
//        fvec.AddSampleDepthVectorLeft(std_img, StdCharVectorsLeft[i]);
//        fvec.AddSampleDepthVectorRight(std_img, StdCharVectorsRight[i]);
//        fvec.AddSampleDepthVectorUp(std_img, StdCharVectorsUp[i]);
//        fvec.AddSampleDepthVectorDown(std_img, StdCharVectorsDown[i]);
//    }
}

std::string JudgeChar::GetPossibleChars(const Mat& mat, char& recommend)
{
    std::string res;
    std::map<char, double> char_cos_values;

    std::vector<double> mat_vec_left, mat_vec_right, mat_vec_up, mat_vec_down;
    fvec.AddSampleDepthVector(mat, mat_vec_left, mat_vec_right, mat_vec_up, mat_vec_down);

    double max_cos_value = 0;
    recommend = '\0';

    for (int i = 0; i < 36; i++)
    {
        if (i == 18 || i == 24)
            continue;

        double cos_value_left = GetCos::GetVectorCos(mat_vec_left, StdCharVectorsLeft[i]);
        double cos_value_right = GetCos::GetVectorCos(mat_vec_right, StdCharVectorsRight[i]);
        double cos_value_up = GetCos::GetVectorCos(mat_vec_up, StdCharVectorsUp[i]);
        double cos_value_down = GetCos::GetVectorCos(mat_vec_down, StdCharVectorsDown[i]);

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
                res.append(1, '0' + i);
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
                res.append(1, 'A' + i - 10);
        }
    }

    // increase threshold until a char matches
    max_cos_value = 0;
    for (double lower_th = SIMILAR_THRESHOLD - 0.01; res.length() <= 0; lower_th -= 0.01)
    {
        for (std::map<char, double>::iterator it = char_cos_values.begin(); it != char_cos_values.end(); it++)
        {
            if (it->second > max_cos_value)
            {
                recommend = it->first;
                max_cos_value = it->second;
            }
            if (it->second >= lower_th)
                res.append(1, it->first);
        }
    }

    return res;
}
