#include "JudgeChar.h"

JudgeChar::JudgeChar(const char* img_dir) : fvec(CROP_WIDTH, CROP_HEIGHT)
{
    for (int i = 0; i <= 9; i++)
    {
        std::string std_img_file = std::string(img_dir) + "standard/20x40/" + IntToString(i) + ".jpg";
        Mat std_img = imread(std_img_file.c_str());

        fvec.AddSampleDepthVectorLeft(std_img, StdCharVectorsLeft[i]);
        fvec.AddSampleDepthVectorRight(std_img, StdCharVectorsRight[i]);
        fvec.AddSampleDepthVectorUp(std_img, StdCharVectorsUp[i]);
        fvec.AddSampleDepthVectorDown(std_img, StdCharVectorsDown[i]);
    }

    for (int i = 10; i < 36; i++)
    {
        char ch = 'A' + i - 10;

        if (ch == 'O' || ch == 'I')
        {
            continue;
        }

        std::string std_img_file = std::string(img_dir) + "standard/20x40/" + ch + ".jpg";
        Mat std_img = imread(std_img_file.c_str());

        fvec.AddSampleDepthVectorLeft(std_img, StdCharVectorsLeft[i]);
        fvec.AddSampleDepthVectorRight(std_img, StdCharVectorsRight[i]);
        fvec.AddSampleDepthVectorUp(std_img, StdCharVectorsUp[i]);
        fvec.AddSampleDepthVectorDown(std_img, StdCharVectorsDown[i]);
    }
}

std::vector<char> JudgeChar::GetChar(const Mat& mat)
{
    std::vector<char> res;

    std::vector<double> mat_vec_left, mat_vec_right, mat_vec_up, mat_vec_down;
    fvec.AddSampleDepthVectorLeft(mat, mat_vec_left);
    fvec.AddSampleDepthVectorRight(mat, mat_vec_right);
    fvec.AddSampleDepthVectorUp(mat, mat_vec_up);
    fvec.AddSampleDepthVectorDown(mat, mat_vec_down);

    double min_value = 1;
//    char res = '\0';
    for (int i = 0; i < 36; i++)
    {
        double cos_value_left = GetCos::GetVectorCos(mat_vec_left, StdCharVectorsLeft[i]);
        double cos_value_right = GetCos::GetVectorCos(mat_vec_right, StdCharVectorsRight[i]);
        double cos_value_up = GetCos::GetVectorCos(mat_vec_up, StdCharVectorsUp[i]);
        double cos_value_down = GetCos::GetVectorCos(mat_vec_down, StdCharVectorsDown[i]);

        double cos_value = (0.8 * cos_value_left + 0.8 * cos_value_up + 0.7 * cos_value_right + 0.9 * cos_value_down) / 4.0;
        if (1 - cos_value < SIMILAR_THRESHOLD)
        {
            if (i >= 0 && i <= 9)
            {
                res.push_back(i + '0');
            }
            else
            {
                res.push_back('A' + i - 10);
            }
//            min_value = 1 - cos_value;
        }
    }

//    if (res == '0' || res == 'Q' || res == '8')
//    {
//        return Distinguish_0_8_Q(mat);
//    }
//    if (res == '2' || res == 'Z')
//    {
//
//    }

    return res;
}
