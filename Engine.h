#ifndef ENGINE_H
#define ENGINE_H
#include <unordered_map>
#include <any>
#include "Wrapper.h"

class Engine {
private:
    std::unordered_map<std::string, Wrapper*> commands;
public:
    void registerCommand(Wrapper* wrapper, const std::string& commandName);

    std::any execute(const std::string& commandName, DefaultArgs&& args);
};

#endif //ENGINE_H
