#include "process/LPRProcess.h"

int main()
{
    std::string result_str;

    /**
     * the function to recognize chars on a license plate
     * param1: path of the image
     * param2: recognition result (by reference)
     * param3: whether output cropped images or not
     * param4: whether output messages to console or not
     * param5: folder to output cropped images (default: empty)
     */
    int result_value = ProcessLicensePlate(
        "./images/chepai1.jpg",
        result_str,
        true,
        false,
        "./images/crops/"
    );

    std::cout << "Result: " << result_str << std::endl;
    return result_value;
}
