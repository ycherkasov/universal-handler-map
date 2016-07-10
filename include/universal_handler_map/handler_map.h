#pragma once
#include <functional>


#define DeclareUniversalMap(MapName, Base) using BaseFunc##MapName = Base; \
template <int Val> \
struct UniversalMap##MapName {};


#define InsertUniversalMap(MapName, Value, Handler) template <> \
struct UniversalMap##MapName<Value> : BaseFunc##MapName \
{ \
    using Function = BaseFunc##MapName; \
    static Function handler##Value; \
    template <typename... Args> \
    static auto call(Args&&... args) -> std::result_of_t<Function(Args...)> \
    { \
        return handler##Value(std::forward<Args>(args)...); \
    } \
}; \
UniversalMap##MapName<Value>::Function UniversalMap##MapName<Value>::handler##Value = Handler;

#define UniversalMapCall(MapName, Value) UniversalMap##MapName<Value>

