#ifndef ENGINE_H
#define ENGINE_H
#include <unordered_map>
#include <any>
#include "Wrapper.h"

class Engine {
private:
    std::unordered_map<std::string, WrapperBase*> commands;
public:
    template<class T, typename Ret, typename... Args>
    void registerCommand(Wrapper<T, Ret, Args...>* wrapper, const std::string& commandName) {
        commands[commandName] = wrapper;
        // commands[commandName] = dynamic_cast<WrapperBase*>(wrapper);
    }

    void execute(const std::string& commandName, const std::unordered_map<std::string, std::any>& args);
};

#endif //ENGINE_H
