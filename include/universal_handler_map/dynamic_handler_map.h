#pragma once
#include <map>
#include <functional>

/// @brief is_callable<> concept implementation
/// To be removed in C++17
/// @example: static_assert(IsCallable<Func>, "Func() should be callable")
template<typename T>
struct IsCallable {
private:
    typedef char(&yes)[1];
    typedef char(&no)[2];

    struct Fallback { void operator()(); };
    struct Derived : T, Fallback { };

    template<typename U, U> struct Check;

    template<typename>
    static yes test(...);

    template<typename C>
    static no test(Check<void (Fallback::*)(), &C::operator()>*);

public:
    static const bool value = sizeof(test<Derived>(0)) == sizeof(yes);
};

/// @brief Policy class that passed to HandlerMap
/// Implement behavior when no handler associated with the key
/// Return default value or do nothing if the function is void()
class DefaultValuePolicy
{
public:

    /// @brief Return default value or do nothing if the function is void()
    template <typename RetType>
    static RetType null_handler()
    {
        return default_value<RetType>();
    }

private:

    /// @brief Base implementation for every return type except void
    template <typename RetType>
    static RetType default_value()
    {
        return RetType();
    }

    /// @brief Specialization for void return type
    template <>
    static void default_value()
    {
        return;
    }
};

/// @brief Policy class that passed to HandlerMap
/// Implement default behavior when the key is not found
/// It may lead to serious problems, so we throw here
struct ThrowPolicy
{
    template <typename RetType>
    [[noreturn]]
    static RetType no_handler()
    {
        throw std::runtime_error("Inappropriate handler value");
    }
};


/// @brief Perform universal mapping Key type to executable Handler
/// @typename Key: any param that satisfies std::map key prerequisites
/// @typename Handler: any callable object
/// @typename NoKeyPolicy: action when Key is not found
/// @typename NullHandlePolicy: action when Handle is NULL
template <typename Key, typename Handler, 
    typename NoKeyPolicy = ThrowPolicy, 
    typename NullHandlePolicy = DefaultValuePolicy>
class HandlerMap
{
public:

    /// @brief Empty handlers map
    HandlerMap()
    {
        static_assert(IsCallable<Handler>::value, "Second template param should be callable");
    }

    /// @brief Initialize handlers map with {}-notation
    HandlerMap(std::map<Key, Handler> other) : handler_map_(std::move(other)) {}

    /// @brief Initialize handlers map std::move() operation
    HandlerMap(std::map<Key, Handler>&& other) : handler_map_(std::move(other)) {}

    /// @brief Add new handler
    void insert(const Key key, Handler handler)
    {
        handler_map_[key] = handler;
    }

    /// @brief Add new handler in a functional way
    /// It could be chained like m(0, handler0)(1, handler1)...
    HandlerMap& operator()(const Key key, Handler handler)
    {
        handler_map_[key] = handler;
        return *this;
    }

    /// @brief Handler invoke point. Lookup handler by the key, pass params pack,
    /// return handler value. Perform Policy actions on non-existent Key and nullptr Handle
    template <typename... Args>
    auto call(const Key& key, Args&&... args) -> std::result_of_t<Handler(Args...)>
    {
        /// Check whether we have a return type in our handler
        using ResultType = std::result_of_t <Handler(Args...)>;

        // If handler does not exist just leave
        auto it = handler_map_.find(key);
        if (it == handler_map_.end()) {
            return NoKeyPolicy::no_handler<ResultType>();
        }

        // call handler if not 0
        auto callback = handler_map_.at(key);
        if (nullptr == callback) {
            return NullHandlePolicy::null_handler<ResultType>();
        }
        return callback(std::forward<Args>(args)...);
    }

    /// @brief Sometimes we have to deal with fixed nimber of handlers
    /// This method provide us simple runtime-check
    [[noreturn]] 
    void throw_if_unexpected(size_t s)
    {
        if (handler_map_.size() != s) {
            throw std::runtime_error("Size of dynamic handler map is not same as expected");
        }
    }

private:

    /// Map of callable objects
    std::map<Key, Handler> handler_map_;
};

