#include "Common.h"

std::string IntToString(int num)
{
    std::string res;
    std::stringstream ss;
    ss << num;
    ss >> res;
    ss.clear();
    return res;
}
