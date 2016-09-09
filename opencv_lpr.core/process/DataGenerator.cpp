#include "DataGenerator.h"

void DataGenerator(char direction)
{
    DepthVectorHelper fvec(CROP_WIDTH, CROP_HEIGHT);
    cv::Mat mat;
    std::vector<double> to_output;

    std::cout << "{";
    for (int i = 0; i < 34; i++)
    {
        std::string filename = std::string("D:\\wuja6_Projects\\Projects\\Eclipse\\VECRobot\\trunk\\JAMESWU\\standard_chars\\") + IntToString(i) + ".jpg";
        mat = cv::imread(filename.c_str(), 0);

        std::vector<double> mat_vec_left, mat_vec_right, mat_vec_up, mat_vec_down;
        fvec.AddSampleDepthVector(mat, mat_vec_left, mat_vec_right, mat_vec_up, mat_vec_down);

        switch (direction)
        {
        case 'l':
            to_output = mat_vec_left;
            break;
        case 'r':
            to_output = mat_vec_right;
            break;
        case 'u':
            to_output = mat_vec_up;
            break;
        case 'd':
            to_output = mat_vec_down;
            break;
        }

        std::cout << "{";
        for (int j = 0; j < to_output.size(); j++)
        {
            std::cout << to_output.at(j) << ".0";
            if (j != to_output.size() - 1)
                std::cout << ",";
            std::cout << " ";
        }
        
        std::cout << "}";
        if (i != 33)
            std::cout << ",\n";
    }
    std::cout << "}";
}
