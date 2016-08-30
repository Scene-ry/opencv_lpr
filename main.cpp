#include "include/recog/JudgeChar.h"
#include "include/PreProcess.h"
#include "include/recog/GetCos.hpp"

int main()
{
    const char* img_dir = "./images/";
    const char* filename = "chepai2";

    PreProcess pre;
    JudgeChar jc(img_dir);

    ProcessResult result = pre.pre_process(img_dir, filename, ".jpg", false);
    //ProcessResult result = ProcessResult::Success;

    if (result != ProcessResult::Success)
    {
        std::cout << "Process image failed." << std::endl;
        return 0;
    }

    std::cout << std::endl;

    for (int i = 0; true; i++)
    {
        std::string s_filename = std::string(img_dir) + "crops/" + filename + "_cut_" + IntToString(i) + ".jpg";
        Mat char_mat = imread(s_filename);

        if (!char_mat.data)
            break;

        std::cout << "Reading cropped image " << i << "..." << std::endl;

        // begin recognition
        std::cout << jc.GetChar(char_mat) << std::endl;

        std::cout << std::endl;
    }

    return 0;
}
