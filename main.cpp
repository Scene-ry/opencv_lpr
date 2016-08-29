#include "include/PreProcess.h"

int main()
{
    PreProcess pre;

    const char* img_dir = "./images/";
    const char* filename = "chepai3";

    ProcessResult result = pre.pre_process(img_dir, filename, ".png", false);

    if (result != ProcessResult::Success)
    {
        std::cout << "Process image failed." << std::endl;
        return 0;
    }

    std::cout << std::endl;

    for (int i = 0; true; i++)
    {
        std::string s_filename = std::string(img_dir) + "crops/" + filename + "_cut_" + pre.IntToString(i) + ".jpg";
        Mat char_mat = imread(s_filename);

        if (!char_mat.data)
            break;

        std::cout << "Reading cropped image " << i << "..." << std::endl;

        // begin recognition

    }

    return 0;
}
