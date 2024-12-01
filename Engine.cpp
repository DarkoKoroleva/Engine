//
// Created by MacBook on 23.11.2024.
//

#include "Engine.h"

void Engine::registerCommand(Wrapper* wrapper, const std::string& commandName) {
    commands[commandName] = wrapper;
}

std::any Engine::execute(const std::string& commandName, const std::unordered_map<std::string, std::any>& args) {
    if (!commands.contains(commandName)) {
        throw std::invalid_argument("Command not found: " + commandName);
    }
    return commands[commandName]->execute(args);
}