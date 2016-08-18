#pragma once

#include <iostream>

#define LOG(message) \
    std::cout << message << std::endl;

#define LOG_ERROR(message) \
    std::cerr << __FILE__ << "," <<  __LINE__ << ": " << message << std::endl;
