#ifndef WRAPPER_H
#define WRAPPER_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <any>
#include <vector>

using DefaultArgs = std::unordered_map<std::string, std::any>;

class Wrapper {
private:
    void* obj;
    std::function<std::any(DefaultArgs&&)> method;
    DefaultArgs defaultArgs;

public:
    template <typename T, typename Method, typename... Args>
    Wrapper(T* obj, Method&& method, DefaultArgs&& defaultArgs)
            : obj(static_cast<void*>(obj)),
              method([obj, method = std::forward<Method>(method)](DefaultArgs&& args) mutable -> std::any {
                  return Wrapper::call<T, Method, Args...>(obj, method, std::move(args));
              }),
              defaultArgs(std::move(defaultArgs)) {}

    std::any execute(DefaultArgs&& args);

    template <typename T, typename Method, typename... Args>
    static auto call(void* obj, Method&& method, DefaultArgs&& args) {
        auto* typedObj = static_cast<T*>(obj);

        std::tuple<Args...> arguments = collectArguments<Args...>(std::move(args));

        return invoke(typedObj, std::forward<Method>(method), arguments, std::index_sequence_for<Args...>());
    }

    template <typename T, typename Method, typename... Args, std::size_t... Is>
    static auto invoke(T* obj, Method&& method, const std::tuple<Args...>& arguments, std::index_sequence<Is...>) {
        return (obj->*std::forward<Method>(method))(std::get<Is>(arguments)...);
    }

    template <typename... Args>
    static std::tuple<Args...> collectArguments(DefaultArgs&& args) {
        return std::make_tuple(getArgument<Args>(args, Args::key)...);
    }


    template <typename T>
    static T getArgument(DefaultArgs& args, const std::string& key) {
        if (args.contains(key)) {
            return std::any_cast<T>(std::move(args.at(key)));
        }
        throw std::invalid_argument("Invalid argument: " + key);
    }
};
#endif //WRAPPER_H
