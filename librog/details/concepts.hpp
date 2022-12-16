#ifndef ROG_DETAILS_CONCEPTS_HPP
#define ROG_DETAILS_CONCEPTS_HPP

#include <concepts>
#if __has_include(<format>)
#include <format>
#endif
#include <string>

namespace rog
{
    template<class T>
    concept ToStringAble = requires(T t)
    {
        // ADL
        { to_string(t) } -> std::convertible_to<std::string>;
    }
    ||
    requires(T t)
    {
        { std::to_string(t) } -> std::convertible_to<std::string>;
    };

    template<class T>
    concept OstreamAble = requires(T t, std::ostream ost)
    {
        ost << t;
    };

    #if __has_include(<format>)
    template<class T>
    concept FormatAble = requires(T t)
    {
        { std::format("{}", t) } -> std::convertible_to<std::string>;
    };
    #endif
}

#endif