#pragma once
#include <stdexcept>

class Parameters_Error : public std::domain_error
{
public:
    Parameters_Error(const std::string& message);
};

