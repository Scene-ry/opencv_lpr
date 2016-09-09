#include "process/LPRProcess.h"

int main()
{
    std::string result;

    /**
     * the function to recognize chars on a license plate
     * param1: path of the image
     * param2: recognition result (by reference)
     * param3: whether output cropped images or not
     * param4: whether output messages to console or not
     * param5: path to output cropped images (default: empty)
     */
    ProcessLicensePlate(
        "./images/chepai1.jpg",
        result,
        true,
        false,
        "./images/crops/"
    );

    std::cout << "Result: " << result << std::endl;
    return 0;
}
