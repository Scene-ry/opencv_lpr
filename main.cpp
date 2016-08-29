#include "include/PreProcess.h"

int main()
{
    PreProcess pre;

    const char* filename = "chepai3";
    ProcessResult result = pre.pre_process(filename, ".png", false);

    if (result != ProcessResult::Success)
    {
        return 0;
    }

    for (int i = 0; true; i++)
    {
        std::string s_filename = std::string("./images/crops/") + filename + "_cut_" + pre.IntToString(i) + ".jpg";
        Mat char_mat = imread(s_filename);

        if (!char_mat.data)
            break;

        std::cout << "Reading cropped image " << i << "..." << std::endl;
    }

    return 0;
}
