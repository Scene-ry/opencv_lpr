#include "include/recog/JudgeChar.h"
#include "include/PreProcess.h"
#include "include/recog/GetCos.hpp"

#define CHAR_MAX_WIDTH  10
#define CHAR_MAX_HEIGHT 100

int main()
{
    const char* img_dir = "./images/";
    const char* filename = "chepai4";

    PreProcess pre(CHAR_MAX_WIDTH, CHAR_MAX_HEIGHT);

    ProcessResult result = pre.pre_process(img_dir, filename, ".jpg", false);
    //ProcessResult result = ProcessResult::Success;

    if (result != ProcessResult::Success)
    {
        std::cout << "Process image failed." << std::endl;
        return 0;
    }

    std::cout << std::endl;

    JudgeChar jc(img_dir);
    for (int i = 0; true; i++)
    {
        std::string s_filename = std::string(img_dir) + "crops/" + filename + "_cut_" + IntToString(i) + ".jpg";
        Mat char_mat = imread(s_filename);

        if (!char_mat.data)
            break;

        std::cout << "Reading cropped image " << i << "..." << std::endl;

        // begin recognition
        std::vector<char> possible_chars = jc.GetChar(char_mat);
        for (int i = 0; i < possible_chars.size(); i++)
        {
            std::cout << possible_chars.at(i) << " ";
        }

        std::cout << std::endl;
    }

    return 0;
}
