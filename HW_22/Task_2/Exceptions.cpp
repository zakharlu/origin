#include "Exceptions.h"
#include <stdexcept>

Parameters_Error::Parameters_Error(const std::string& message) : std::domain_error(message.c_str()) {}