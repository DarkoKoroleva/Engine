//
// Created by MacBook on 23.11.2024.
//

#include "Wrapper.h"

std::any Wrapper::execute(DefaultArgs&& args) {
    if (method) {
        return method(std::move(args));
    }
    throw std::runtime_error("Method not initialized.");
}


