#ifndef GETCOS_HPP
#define GETCOS_HPP

#include <cmath>
#include <vector>

class GetCos
{
public:
    static double GetVectorCos(const std::vector<double>& v1, double* v2)
    {
        int v1_size = v1.size();
//        if (v1_size != v2.size())
//        {
//            return 0;
//        }

        double numerator = 0;
        double denominator_l = 0, denominator_r = 0;

        for (int i = 0; i < v1_size; i++)
        {
            numerator += v1.at(i) * v2[i];
            denominator_l += v1.at(i) * v1.at(i);
            denominator_r += v2[i] * v2[i];
        }

        if (denominator_l != 0 && denominator_r != 0)
        {
            return numerator / (std::sqrt(denominator_l) * std::sqrt(denominator_r));
        }

        return 0;
    }
};

#endif
