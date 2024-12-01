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
    std::function<std::any(const DefaultArgs&)> method;
    DefaultArgs defaultArgs;

public:
    template <typename T, typename Ret, typename... Args>
    Wrapper(T* obj, Ret (T::*method)(Args...), DefaultArgs defaultArgs)
        : obj(static_cast<void*>(obj)),
          method([this, obj, method](const DefaultArgs& args) -> std::any {
              return call(obj, method, args);
          }),
          defaultArgs(std::move(defaultArgs)) {}


    std::any execute(const std::unordered_map<std::string, std::any> &args);

    template <typename T, typename Ret, typename... Args, std::size_t... Is>
    static Ret invoke(T* obj, Ret (T::*method)(Args...), const std::tuple<Args...>& arguments, std::index_sequence<Is...>) {
        return (obj->*method)(std::get<Is>(arguments)...);
    }

    template <typename T, typename Ret, typename... Args>
    //std::enable_if_t<!std::is_same<Ret, void>::value, std::any>  // overload for not void func
    static std::any call(void* obj, Ret (T::*method)(Args...), const std::unordered_map<std::string, std::any>& args) {
        auto* typedObj = static_cast<T*>(obj);

        std::tuple<Args...> arguments = collectArguments<Args...>(args);

        // Вызываем метод
        return invoke(typedObj, method, arguments, std::index_sequence_for<Args...>());
    }

     /*template <typename T, typename Ret, typename... Args> // overload FOR VOID FUNC
        std::enable_if_t<std::is_same<Ret, void>::value, std::any> call(void* obj, Ret (T::*method)(Args...),
                                                                        const std::unordered_map<std::string, std::any>& args) {
        auto* typedObj = static_cast<T*>(obj);

        std::tuple<Args...> arguments = collectArguments<Args...>(args);

        invoke(typedObj, method, arguments, std::index_sequence_for<Args...>());

        return std::any{};
    }*/

    template <typename... Args>
    static std::tuple<Args...> collectArguments(const std::unordered_map<std::string, std::any>& args) {
        std::vector<std::string> keys;
        std::index_sequence<sizeof...(Args)> index;

        keys.reserve(args.size());
        for (const auto& [key, value] : args) {
            keys.emplace_back(key);
        }

        return collectArgumentsHelper<Args...>(args, keys, std::make_index_sequence<sizeof...(Args)>());
    }

    template <typename... Args, std::size_t... Is>
    static std::tuple<Args...> collectArgumentsHelper(const std::unordered_map<std::string, std::any>& args,
                                                    const std::vector<std::string>& keys,
                                                    std::index_sequence<Is...>) {
        return std::make_tuple(getArgument<Args>(args, keys[Is])...);
    }

    template <typename T>
    static T getArgument(const std::unordered_map<std::string, std::any>& args, const std::string& key) {
        if (args.contains(key)) {
            return std::any_cast<T>(args.at(key));
        }
        throw std::invalid_argument("Invalid argument: " + key);
    }
};
#endif //WRAPPER_H
