#include "JudgeChar.h"

JudgeChar::JudgeChar(const char* img_dir)
{
    // Initialize static array
    for (int i = 0; i <= 9; i++)
    {
        std::string std_img_file = std::string(img_dir) + "standard/" + PreProcess::IntToString(i) + ".jpg";
        Mat std_img = imread(std_img_file.c_str());
        sc.AddSlopeVector(std_img, StdCharVectors[i]);
        dc.AddDepthVector(std_img, StdCharVectors[i]);
    }

    for (int i = 10; i < 36; i++)
    {
        char ch = 'A' + i - 10;

        if (ch == 'O' || ch == 'I')
        {
            continue;
        }

        std::string std_img_file = std::string(img_dir) + "standard/" + ch + ".jpg";
        Mat std_img = imread(std_img_file.c_str());
        sc.AddSlopeVector(std_img, StdCharVectors[i]);
        dc.AddDepthVector(std_img, StdCharVectors[i]);
    }
}

char JudgeChar::GetChar(const Mat& mat)
{
    std::vector<double> mat_vec;
    sc.AddSlopeVector(mat, mat_vec);
    dc.AddDepthVector(mat, mat_vec);

    double min_value = 1;
    char res = '\0';
    for (int i = 0; i < 36; i++)
    {
        double cos_value = GetCos::GetVectorCos(mat_vec, StdCharVectors[i]);
        if (1 - cos_value < min_value)
        {
            if (i >= 0 && i <= 9)
            {
                res = i + '0';
            }
            else
            {
                res = 'A' + i - 10;
            }
            min_value = 1 - cos_value;
        }
    }
    return res;
}
