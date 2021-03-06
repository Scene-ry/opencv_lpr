#include "JudgeChar.h"

JudgeChar::JudgeChar() : fvec(CROP_WIDTH, CROP_HEIGHT)
{
}

std::map<char, double> JudgeChar::GetPossibleChars(const cv::Mat& mat)
{
    std::map<char, double> res;
    if (mat.cols == 1 && mat.rows == 1)
    {
        res.insert(std::make_pair('1', 1.0));
        return res;
    }

    std::map<char, double> char_cos_values;

    std::vector<double> mat_vec_left, mat_vec_right, mat_vec_up, mat_vec_down;
    fvec.AddSampleDepthVector(mat, mat_vec_left, mat_vec_right, mat_vec_up, mat_vec_down);

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

            if (cos_value >= SIMILAR_THRESHOLD)
            {
                res.insert(std::make_pair('0' + i, cos_value));
                std::vector<double> fdir;
                fdir.push_back(cos_value_left);
                fdir.push_back(cos_value_right);
                fdir.push_back(cos_value_up);
                fdir.push_back(cos_value_down);
                four_directions.insert(std::make_pair('0' + i, fdir));
            }
        }
        else
        {
            char_cos_values.insert(std::make_pair('A' + i - 10, cos_value));

            if (cos_value >= SIMILAR_THRESHOLD)
            {
                res.insert(std::make_pair('A' + i - 10, cos_value));
                std::vector<double> fdir;
                fdir.push_back(cos_value_left);
                fdir.push_back(cos_value_right);
                fdir.push_back(cos_value_up);
                fdir.push_back(cos_value_down);
                four_directions.insert(std::make_pair('A' + i - 10, fdir));
            }
        }
    }

    // increase threshold until a char matches
    for (double lower_th = SIMILAR_THRESHOLD - 0.01; res.size() <= 0; lower_th -= 0.01)
    {
        for (std::map<char, double>::iterator it = char_cos_values.begin(); it != char_cos_values.end(); it++)
        {
            if (it->second >= lower_th)
                res.insert(std::make_pair(it->first, it->second));
        }
    }

    // Exclude some chars
    if (res.find('8') != res.end())
    {
        CharExcluders::ByWhiteAreasOnVerticalMediumLine(mat, res, '8', NumComparer::Smaller, 3);
    }
    if (res.find('H') != res.end())
    {
        //
    }
    if (res.find('K') != res.end())
    {
        CharExcluders::ByWhitePointsOnHorizontalMediumLine(mat, res, 'K', NumComparer::Larger, 18);
    }
    if (res.find('M') != res.end())
    {
        CharExcluders::ByWhitePixelUpDownDiff(mat, res, 'M', NumComparer::Smaller, 10);
    }
    if (res.find('N') != res.end())
    {
        CharExcluders::ByWhitePixelUp_LeftRightDiff(mat, res, 'N', NumComparer::Smaller, 12);
    }
    if (res.find('P') != res.end())
    {
        CharExcluders::ByWhiteAreasOnVerticalMediumLine(mat, res, 'P', NumComparer::Larger, 2);
    }
    if (res.find('Q') != res.end())
    {
        CharExcluders::ByWhiteAreasOnVerticalMediumLine(mat, res, 'Q', NumComparer::Smaller, 3);
    }
    if (res.find('S') != res.end())
    {
        //
    }
    if (res.find('V') != res.end())
    {
        CharExcluders::ByOneWhiteAreaStartHorizontalLine(mat, res, 'V', NumComparer::Larger, 30);
    }
    if (res.find('X') != res.end())
    {
        CharExcluders::ByWhitePointsOnHorizontalMediumLine(mat, res, 'X', NumComparer::Larger, 18);
        CharExcluders::ByWhitePixelUp_LeftRightDiff(mat, res, 'X', NumComparer::Larger, 12);
    }
    if (res.find('Y') != res.end())
    {
        CharExcluders::ByWhiteAreasOnHorizontal3_4Line(mat, res, 'Y', NumComparer::Larger, 1);
    }

    return res;
}
