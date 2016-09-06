#include "LPRProcess.h"

int ProcessLicensePlate(const char* img_path, std::string& result, bool is_output_img, bool is_cout, const char* output_img_path = "")
{
    std::vector<Mat> split_chars;

    //const char* img_dir = "./images/";
    //const char* filename = "chepai1";

    ProcessResult pre_result = pre_process(img_path, split_chars, is_output_img, is_cout, output_img_path);
    //ProcessResult pre_result = ProcessResult::Success;

    if (pre_result != ProcessResult::Success)
    {
        if (is_cout)
            std::cout << "Process image failed." << std::endl;
        return -1;
    }

    if (is_cout)
        std::cout << std::endl;

    JudgeChar jc;

    int crop_count = 0;
    for (std::vector<Mat>::iterator it = split_chars.begin(); it != split_chars.end(); it++)
    {
        Mat char_mat = *it;

        if (!char_mat.data)
            break;

        if (is_cout)
            std::cout << "Reading cropped image " << crop_count++ << "..." << std::endl;

        // begin recognition
        char recommend = '\0';
        char recog = '\0';
        std::map<char, double> possible_chars = jc.GetPossibleChars(char_mat, recommend, recog);

        // sort by cos values (descending)
        std::vector<std::pair<char, double> > possible_chars_vec(possible_chars.begin(), possible_chars.end());
        std::sort(possible_chars_vec.begin(), possible_chars_vec.end(), [](const std::pair<char, double>& p1, const std::pair<char, double>& p2){ return p1.second > p2.second; });

        if (is_cout)
        {
            std::cout << "Possible chars: " << std::endl;
            for (std::vector<std::pair<char, double> >::iterator it = possible_chars_vec.begin(); it != possible_chars_vec.end();)
            {
                std::cout << it->first << ": " << it->second;
                if (++it == possible_chars_vec.end())
                    break;
                std::cout << std::endl;
            }
        }

        if (recog != '\0')
        {
            if (is_cout)
                std::cout << std::endl << "Recognized as: " << recog;
            result.append(1, recog);
        }
        else if (recommend != '\0')
        {
            if (is_cout)
                std::cout << std::endl << "Recommend: " << recommend;
            result.append(1, recommend);
        }

        if (is_cout)
            std::cout << std::endl << std::endl;
    }

    return 0;
}
