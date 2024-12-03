#include "Engine.h"

void Engine::execute(const std::string& commandName, const std::unordered_map<std::string, std::any>& args) {
    if (!commands.contains(commandName)) {
        throw std::invalid_argument("Command not found: " + commandName);
    }
    commands[commandName]->execute(args);
}