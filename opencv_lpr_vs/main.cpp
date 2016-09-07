#include "process/LPRProcess.h"

int main()
{
    std::string img_path, crop_output_path, result_str;
    for (int i = 1; i <= 50; i++)
    {
        img_path = std::string("./samples/images/") + IntToString(i) + ".jpg";
        crop_output_path = std::string("./samples/crops/") + IntToString(i);
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

        if (result_value == 0)
            std::cout << "Result of plate " << i << ":\t" << result_str << std::endl;
        else
            std::cout << "Process of plate " << i << " failed." << std::endl;
    }
    
    return 0;
}
