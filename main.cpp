#include "include/recog/JudgeChar.h"
#include "include/PreProcess.h"
#include "include/recog/GetCos.hpp"

//#define CHAR_MAX_WIDTH  1
//#define CHAR_MAX_HEIGHT 20

int main()
{
#ifdef __GET_STD_CHAR_IMAGE__
    PreProcess pre;   // binary threshold set to 140
    const char* img_dir = "./images/standard/chars/";
    for (int i = 0; i < 34; i++)
    {
        ProcessResult result = pre.pre_process(img_dir, IntToString(i).c_str(), ".bmp", true);
        if (result != ProcessResult::Success)
            break;
    }

#else
    PreProcess pre;
    const char* img_dir = "./images/";
    const char* filename = "chepai7";

    ProcessResult result = pre.pre_process(img_dir, filename, ".jpg");
    //ProcessResult result = ProcessResult::Success;

    if (result != ProcessResult::Success)
    {
        std::cout << "Process image failed." << std::endl;
        return 0;
    }

    std::cout << std::endl;

#ifdef __GET_EDGE_DATA__
    std::cout << "Edge data included." << std::endl;
#else
    std::cout << "Edge data not included." << std::endl;
#endif // __GET_EDGE_DATA__

    std::cout << std::endl;
    JudgeChar jc(img_dir);
    for (int i = 0; true; i++)
    {
        std::string s_filename = std::string(img_dir) + "crops/" + filename + "_cut_" + IntToString(i) + ".jpg";
        Mat char_mat = imread(s_filename, 0);

        if (!char_mat.data)
            break;

        std::cout << "Reading cropped image " << i << "..." << std::endl;

        // begin recognition
        char recommend = '\0';
        char recog = '\0';
        std::map<char, double> possible_chars = jc.GetPossibleChars(char_mat, recommend, recog);

        // sort by cos values (descending)
        std::vector<std::pair<char, double> > possible_chars_vec(possible_chars.begin(), possible_chars.end());
        std::sort(possible_chars_vec.begin(), possible_chars_vec.end(), [](const std::pair<char, double>& p1, const std::pair<char, double>& p2){ return p1.second > p2.second; });

        std::cout << "Possible chars: " << std::endl;
        for (std::vector<std::pair<char, double> >::iterator it = possible_chars_vec.begin(); it != possible_chars_vec.end();)
        {
            std::cout << it->first << ": " << it->second;
            if (++it == possible_chars_vec.end())
                break;
            std::cout << std::endl;
        }

        if (recog != '\0')
        {
            std::cout << std::endl << "Recognized as: " << recog;
        }
        else if (recommend != '\0')
        {
            std::cout << std::endl << "Recommend: " << recommend;
        }

        std::cout << std::endl << std::endl;
    }

#endif // __GET_STD_CHAR_IMAGE__

    return 0;
}
