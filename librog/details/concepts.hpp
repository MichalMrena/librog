#ifndef ROG_DETAILS_CONCEPTS_HPP
#define ROG_DETAILS_CONCEPTS_HPP

#include <version>
#include <concepts>
#ifdef __cpp_lib_format
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

    #ifdef __cpp_lib_format
    template<class T>
    concept FormatAble = requires(T t)
    {
        { std::format("{}", t) } -> std::convertible_to<std::string>;
    };
    #endif
}

#endif