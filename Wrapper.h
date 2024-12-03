#ifndef WRAPPER_H
#define WRAPPER_H
#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <any>
#include <vector>

using DefaultArgs = std::unordered_map<std::string, std::any>;

class WrapperBase {
public:
    virtual ~WrapperBase() = default;
    virtual void execute (const std::unordered_map<std::string, std::any>& args) = 0;
};


template <typename T, typename Ret, typename ...Args>
class Wrapper : public WrapperBase {
private:
    T* obj;
    std::function<Ret(const DefaultArgs&)> method;
    DefaultArgs defaultArgs;
public:
    Wrapper(T* obj, Ret (T::*method)(Args...), DefaultArgs defaultArgs)
            : obj(obj),
              method([this, obj, method](const DefaultArgs& args) -> Ret {
                  return call(obj, method, args);
              }),
              defaultArgs(std::move(defaultArgs)) {}

    void execute(const std::unordered_map<std::string, std::any>& args) override {
        if (method) {
            method(args);
        } else {
            throw std::runtime_error("Method not initialized.");
        }
    }

    template <std::size_t... Is>
    static Ret invoke(T* obj, Ret (T::*method)(Args...), const std::tuple<Args...>& arguments, std::index_sequence<Is...>) {
        return (obj->*method)(std::get<Is>(arguments)...);
    }

    static Ret call(void* obj, Ret (T::*method)(Args...), const std::unordered_map<std::string, std::any>& args) {
        auto* typedObj = static_cast<T*>(obj);
        std::tuple<Args...> arguments = collectArguments(args);
        return invoke(typedObj, method, arguments, std::index_sequence_for<Args...>());
    }


    static std::tuple<Args...> collectArguments(const std::unordered_map<std::string, std::any>& args) {
        std::vector<std::string> keys;
        std::index_sequence<sizeof...(Args)> index;
        keys.reserve(args.size());
        for (const auto& [key, value] : args) {
            keys.emplace_back(key);
        }
        return collectArgumentsHelper(args, keys, std::make_index_sequence<sizeof...(Args)>());
    }

    template <std::size_t... Is>
    static std::tuple<Args...> collectArgumentsHelper(const std::unordered_map<std::string, std::any>& args,
                                                      const std::vector<std::string>& keys,
                                                      std::index_sequence<Is...>) {
        return std::make_tuple(getArgument<Args>(args, keys[Is])...);
    }

    template<typename Temp>
    static Temp getArgument(const std::unordered_map<std::string, std::any>& args, const std::string& key) {
        if (args.contains(key)) {
            return std::any_cast<Temp>(args.at(key));
        }
        throw std::invalid_argument("Invalid argument: " + key);
    }
};

#endif //WRAPPER_H
