#include "SlopeCalc.h"

void SlopeCalc::AddSlopeVector(const Mat& mat, std::vector<double>& result)
{
    int width = mat.cols;
    int height = mat.rows;

    // left
    double l_pos = 0, l_neg = 0, l_zerocount = 0;
    int h_prev = -1, w_prev = -1;
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            if ((int)mat.data[h * width + w] != 0)
            {
                if (h_prev != -1)
                {
                    if (w == w_prev)
                    {
                        // slope is not exist
                        break;
                    }
                    if (h == h_prev)
                    {
                        // slope is zero
                        l_zerocount++;
                    }
                    else
                    {
                        double k = (h - h_prev) / (double)(w - w_prev);
                        if (k > 0) l_pos += k;
                        else l_neg += k;
                    }

                }
                h_prev = h;
                w_prev = w;
                break;
            }
        }
    }
    result.push_back(l_pos);
    result.push_back(l_neg);
    result.push_back(l_zerocount);

    // up
    double u_pos = 0, u_neg = 0, u_zerocount = 0;
    h_prev = -1; w_prev = -1;
    for (int w = 0; w < width; w++)
    {
        for (int h = 0; h < height; h++)
        {
            if ((int)mat.data[h * width + w] != 0)
            {
                if (h_prev != -1)
                {
                    if (w == w_prev)
                    {
                        // slope is not exist
                        break;
                    }
                    if (h == h_prev)
                    {
                        // slope is zero
                        u_zerocount++;
                    }
                    else
                    {
                        double k = (h - h_prev) / (double)(w - w_prev);
                        if (k > 0) u_pos += k;
                        else u_neg += k;
                    }

                }
                h_prev = h;
                w_prev = w;
                break;
            }
        }
    }
    result.push_back(u_pos);
    result.push_back(u_neg);
    result.push_back(u_zerocount);

    // right
    double r_pos = 0, r_neg = 0, r_zerocount = 0;
    h_prev = -1; w_prev = -1;
    for (int h = 0; h < height; h++)
    {
        for (int w = width - 1; w >= 0; w--)
        {
            if ((int)mat.data[h * width + w] != 0)
            {
                if (h_prev != -1)
                {
                    if (w == w_prev)
                    {
                        // slope is not exist
                        break;
                    }
                    if (h == h_prev)
                    {
                        // slope is zero
                        r_zerocount++;
                    }
                    else
                    {
                        double k = (h - h_prev) / (double)(w - w_prev);
                        if (k > 0) r_pos += k;
                        else r_neg += k;
                    }

                }
                h_prev = h;
                w_prev = w;
                break;
            }
        }
    }
    result.push_back(r_pos);
    result.push_back(r_neg);
    result.push_back(r_zerocount);

    // down
    double d_pos = 0, d_neg = 0, d_zerocount = 0;
    h_prev = -1; w_prev = -1;
    for (int w = 0; w < width; w++)
    {
        for (int h = height - 1; h >= 0; h--)
        {
            if ((int)mat.data[h * width + w] != 0)
            {
                if (h_prev != -1)
                {
                    if (w == w_prev)
                    {
                        // slope is not exist
                        break;
                    }
                    if (h == h_prev)
                    {
                        // slope is zero
                        d_zerocount++;
                    }
                    else
                    {
                        double k = (h - h_prev) / (double)(w - w_prev);
                        if (k > 0) d_pos += k;
                        else d_neg += k;
                    }

                }
                h_prev = h;
                w_prev = w;
                break;
            }
        }
    }
    result.push_back(d_pos);
    result.push_back(d_neg);
    result.push_back(d_zerocount);
}
