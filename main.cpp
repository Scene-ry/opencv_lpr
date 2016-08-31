#include "include/recog/JudgeChar.h"
#include "include/PreProcess.h"
#include "include/recog/GetCos.hpp"
#include "include/recog/Distinguishers.hpp"

#define CHAR_MAX_WIDTH  5
#define CHAR_MAX_HEIGHT 20

int main()
{
    const char* img_dir = "./images/";
    const char* filename = "chepai2";

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
        std::string possible_chars = jc.GetPossibleChars(char_mat);
        char recog = '\0';

        // when met some hard-recognized chars
        if (possible_chars.find('0') != std::string::npos || possible_chars.find('8') != std::string::npos || possible_chars.find('Q') != std::string::npos)
        {
            recog = Distinguish_0_8_Q(char_mat);
        }

        std::cout << "Possible chars: ";
        for (int i = 0; i < possible_chars.size(); i++)
        {
            std::cout << possible_chars.at(i) << " ";
        }

        if (recog != '\0')
        {
            std::cout << std::endl << "Recognized as: " << recog;
        }

        std::cout << std::endl << std::endl;
    }

    return 0;
}
