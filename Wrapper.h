#ifndef WRAPPER_H
#define WRAPPER_H
#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <any>
#include <vector>
#include <utility>
#include "TextEditor.h"

using DefaultArgs = std::unordered_map<std::string, std::any>;

class WrapperBase {
public:
    virtual ~WrapperBase() = default;
    virtual void execute (const DefaultArgs& args) = 0;
};


template <typename T, typename Ret, typename ...Args>
class Wrapper : public WrapperBase {
private:
    T* obj;
    //Ret (T::*func)(Args...); //for debug
    std::function<Ret(const DefaultArgs&)> method;
    DefaultArgs defaultArgs;
public:
    Wrapper(T* obj, Ret (T::*func)(Args...), const DefaultArgs& defaultArgs)
            : obj(obj),
            //func(func), //for debug
            method([this, obj, func](const DefaultArgs& args) -> Ret {
                return call(obj, func, args);
            }),
            defaultArgs(std::move(defaultArgs)) {}

    void execute(const DefaultArgs& args) override {
        if (method) {
            //call(obj, func, args); //for debug
            method(args);
        } else {
            throw std::runtime_error("Method not initialized.");
        }
    }

private:
    template <std::size_t... Is>
    static Ret invoke(T* obj, Ret (T::*method)(Args...), const std::tuple<Args...>& arguments, std::index_sequence<Is...>) {
        //std::cout << "Extracted value: " << std::get<0>(arguments) << std::endl;
        return (obj->*method)(std::get<Is>(arguments)...);
    }

    Ret call(T* obj, Ret (T::*method)(Args...), const DefaultArgs& args) {
        if constexpr (sizeof...(Args) == 0) {
            return (obj->*method)();
        } else {
            std::tuple<Args...> arguments = collectArguments(args);
            return invoke(obj, method, arguments, std::index_sequence_for<Args...>());
        }
    }


    std::tuple<Args...> collectArguments(const DefaultArgs& args) {
        std::vector<std::string> keys;
        std::index_sequence<sizeof...(Args)> index;
        keys.reserve(args.size());
        for (const auto& [key, value] : defaultArgs) {
            keys.emplace_back(key);
        }
        return collectArgumentsHelper(args, keys, std::make_index_sequence<sizeof...(Args)>());
    }

    template <std::size_t... Is>
    static std::tuple<Args...> collectArgumentsHelper(const DefaultArgs& args,
                                                      const std::vector<std::string>& keys,
                                                      std::index_sequence<Is...>) {
        std::tuple tuple = std::make_tuple(getArgument<Args>(args, keys[Is])...); // FIXME if Args... = const&, then we get a hanging link & undefined behavior next, oops
        return tuple;
    }

    template<typename Temp>
    static Temp getArgument(const DefaultArgs& args, const std::string& key) {
        if (args.contains(key)) {
            return std::any_cast<Temp>(args.at(key));
        }
        throw std::invalid_argument("Invalid argument: " + key);
    }
};

#endif //WRAPPER_H
