#ifndef ROG_DETAILS_CONSOLE_OUTPUT_HPP
#define ROG_DETAILS_CONSOLE_OUTPUT_HPP

#include <librog/rog.hpp>
#include <librog/details/console.hpp>
#include <string>

namespace rog
{
    /**
     *  \brief Prints results of all tests in the hierarchy.
     */
    class TestOutputterVisitor : public IVisitor
    {
    public:
        auto visit (LeafTest&) -> void override;
        auto visit (CompositeTest&) -> void override;

    private:
        Console console_;
        std::string prefix_;
    };
}

#endif