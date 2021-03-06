#pragma once
#include <functional>

/// @param MapName - name of (int-function) compile-time map 
/// @param Base - std::function<> template with appropriate signature
/// @example DeclareUniversalMap(OneParam, std::function<void(int)>);
#define DeclareUniversalMap(MapName, Base) using BaseFunc##MapName = Base; \
template <int Val> \
struct UniversalMap##MapName {};


/// @param MapName - name of (int-function) compile-time map 
/// @param Value - integer of converted to integer value 
/// @param Handler - std::function or convertable with appropriate signature
/// @example: InsertUniversalMap(OneParam, 0, &MyHandlers1::class_handler_zero);
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

/// @param MapName - name of (int-function) compile-time map 
/// @param Value - integer which is call mapped handler
#define UniversalMapCall(MapName, Value) UniversalMap##MapName<Value>

