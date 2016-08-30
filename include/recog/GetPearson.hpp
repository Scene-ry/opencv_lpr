#ifndef GETPEARSON_HPP_INCLUDED
#define GETPEARSON_HPP_INCLUDED

#include <cmath>
#include <vector>

class GetPearson
{
public:
    static double GetVectorPearson(const std::vector<double>& v1, const std::vector<double>& v2)
    {
        int v1_size = v1.size();
        if (v1_size != v2.size())
        {
            return 0;
        }

        double numerator = 0;
        double denominator_l = 0, denominator_r = 0;

        double v1_average = 0, v2_average = 0;

        for (int i = 0; i < v1_size; i++)
        {
            v1_average += v1.at(i);
            v2_average += v2.at(i);
        }
        v1_average /= (double)v1_size;
        v2_average /= (double)v1_size;

        for (int i = 0; i < v1_size; i++)
        {
            double v1_diff = v1.at(i) - v1_average;
            double v2_diff = v2.at(i) - v2_average;
            numerator += v1_diff * v2_diff;
            denominator_l += v1_diff * v1_diff;
            denominator_r += v2_diff * v2_diff;
        }

        if (denominator_l == 0 || denominator_r == 0)
        {
            return 0;
        }

        return numerator / (std::sqrt(denominator_l) * std::sqrt(denominator_r));
    }
};

#endif // GETPEARSON_HPP_INCLUDED
