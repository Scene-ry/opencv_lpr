#include "process/LPRProcess.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <dirent.h>
#endif

#define PLATE_RECOG_SIZE 6

int main()
{
    std::vector<std::string> plate_images;
    std::vector<std::string> error_chars;

#ifdef _WIN32
    WIN32_FIND_DATA search_data;
    memset(&search_data, 0, sizeof(WIN32_FIND_DATA));
    HANDLE handle = FindFirstFile("./samples/images/*", &search_data);

    while (handle != INVALID_HANDLE_VALUE)
    {
        plate_images.push_back(std::string(search_data.cFileName));
        if (FindNextFile(handle, &search_data) == FALSE)
            break;
    }

    //Close the handle after use or memory/resource leak
    FindClose(handle);

#else
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir("./samples/images/")) != 0)
    {
        while ((ent = readdir(dir)) != 0)
        {
            plate_images.push_back(std::string(ent->d_name));
        }
        closedir(dir);
    }
    else
    {
        std::cout << "File open error." << std::endl;
    }
#endif

    std::string img_path, crop_output_path, result_str;
    int correct_count = 0;
    int total_count = 0;
    for (std::vector<std::string>::iterator it = plate_images.begin(); it != plate_images.end(); it++)
    {
        if ((*it).find(".jpg") == std::string::npos)
        {
            continue;
        }

        std::string plate_name = (*it).substr(0, (*it).find_last_of('.'));

        img_path = std::string("./samples/images/") + (*it);
        crop_output_path = std::string("./samples/crops/") + plate_name;
        result_str = "";

        /**
        * the function to recognize chars on a license plate
        * param1: path of the image
        * param2: recognition result (by reference)
        * param3: whether output cropped images or not
        * param4: whether output messages to console or not
        * param5: folder to output cropped images (default: empty)
        */
        int result_value = ProcessLicensePlate(
            img_path.c_str(),
            result_str,
            false,
            false,
            crop_output_path.c_str()
            );

        if (result_value == 0 && result_str.size() >= PLATE_RECOG_SIZE)
        {
            error_chars.clear();

            result_str = result_str.substr(result_str.size() - PLATE_RECOG_SIZE, PLATE_RECOG_SIZE);
            unsigned long long plate_name_size = plate_name.size(), res_size = result_str.size();

            for (unsigned long long i = 0; i < PLATE_RECOG_SIZE; i++)
            {
                char original = plate_name.at(plate_name_size - i - 1);
                char recognize = result_str.at(res_size - i - 1);
                if (original != recognize)
                {
                    error_chars.push_back(std::string("") + original + "->" + recognize);
                }
            }
            std::cout << plate_name << " -> " << result_str;

            for (std::vector<std::string>::iterator err = error_chars.begin(); err != error_chars.end(); err++)
            {
                std::cout << " " << (*err);
            }

            if (error_chars.empty())
                correct_count++;

            total_count++;
            std::cout << std::endl;
        }
        else
        {
            std::cout << "Process of plate " << plate_name << " failed." << std::endl;
        }
    }

    if (total_count != 0)
        std::cout << "Correct rate: " << (correct_count / (double)total_count * 100) << "%" << std::endl;

    return 0;
}
