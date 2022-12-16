#ifndef ROG_DETAILS_CONSOLE_OUTPUT_HPP
#define ROG_DETAILS_CONSOLE_OUTPUT_HPP

#include <librog/details/console.hpp>
#include <librog/visitors.hpp>
#include <string>

namespace rog
{
    /**
     *  \brief Specifies level of details in the console output.
     */
    enum class ConsoleOutputType
    {
        Full,
        NoLeaf
    };

    class LeafTest;
    class CompositeTest;

    /**
     *  \brief Prints results of all tests in the hierarchy.
     */
    class TestOutputterVisitor : public IVisitor
    {
    public:
        TestOutputterVisitor(ConsoleOutputType);
        auto visit (LeafTest&) -> void override;
        auto visit (CompositeTest&) -> void override;

    private:
        Console console_;
        std::string prefix_;
        ConsoleOutputType otype_;
    };
}

#endif