#pragma once
#include <string>
#include <sstream>
#include <deque>
#include <vector>

namespace hp
{
    bool isNumber(const std::string &str)
    {
        for (char const &c : str)
        {
            if (std::isdigit(c) == 0)
                return false;
        }
        return true;
    }

    bool isFloat(std::string myString)
    {
        std::istringstream iss(myString);
        float f;
        iss >> std::noskipws >> f; // noskipws considers leading whitespace invalid
        // Check the entire string was consumed and if either failbit or badbit is set
        return iss.eof() && !iss.fail();
    }

    bool isDouble(std::string myString)
    {
        std::istringstream iss(myString);
        double d;
        iss >> std::noskipws >> d; // noskipws considers leading whitespace invalid
        // Check the entire string was consumed and if either failbit or badbit is set
        return iss.eof() && !iss.fail();
    }

}