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

    // distinguish '0' '8' 'D' 'Q'
    if (recommend == '0' || recommend == '8' || recommend == 'D' || recommend == 'Q')
    {
        Distinguish_0_8_D_Q(mat, recog);
    }

    return res;
}
