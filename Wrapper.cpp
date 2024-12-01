//
// Created by MacBook on 23.11.2024.
//

#include "Wrapper.h"

std::any Wrapper::execute(const std::unordered_map<std::string, std::any> &args) {
    if (method) {
        return method(args);
    }
    throw std::runtime_error("Method not initialized.");
}


