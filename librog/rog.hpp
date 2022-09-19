#ifndef ROG_ROG_HPP
#define ROG_ROG_HPP

#include <memory>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

namespace rog
{
    class LeafTest;
    class CompositeTest;

    /**
     *  \brief Visitor interface.
     */
    struct IVisitor
    {
        virtual ~IVisitor () = default;
        virtual auto visit (LeafTest&) -> void  = 0;
        virtual auto visit (CompositeTest&) -> void  = 0;
    };

    /**
     *  \brief Visitable interface.
     */
    struct IVisitable
    {
        virtual ~IVisitable () = default;
        virtual auto accept (IVisitor&) -> void = 0;
    };

    /**
     *  \brief Result of a Test.
     */
    enum class TestResult
    {
        Pass, Fail, Partial, NotEvaluated
    };

    /**
     *  \brief Type of a test message.
     */
    enum class TestMessageType
    {
        Pass, Fail, Info
    };

    /**
     *  \brief Result of an assertion.
     */
    struct TestMessage
    {
        TestMessageType type_;
        std::string text_;
    };

    /**
     *  \brief Common base class for test.
     */
    class Test : public IVisitable
    {
    public:
        virtual ~Test () = default;
        virtual auto run () -> void = 0;
        virtual auto result () const -> TestResult = 0;
        auto name () const -> std::string_view;

    protected:
        Test (std::string);

    private:
        std::string name_;
    };

    /**
     *  \brief Implements one specific test.
     */
    class LeafTest : public Test
    {
    public:
        LeafTest (std::string);
        auto run () -> void override;
        auto result () const -> TestResult override;
        auto output () const -> std::vector<TestMessage> const&;
        auto accept (IVisitor&) -> void override;

    protected:
        virtual auto test () -> void = 0;
        auto assert_true (bool, std::string) -> void;
        auto log_info (std::string) -> void;
        auto log_fail (std::string) -> void;
        auto log_pass (std::string) -> void;

    private:
        std::vector<TestMessage> results_;
    };

    /**
     *  \brief Container for tests. Creates hierarchy of tests.
     */
    class CompositeTest : public Test
    {
    public:
        CompositeTest(std::string);
        CompositeTest(CompositeTest const&) = delete;
        CompositeTest(CompositeTest&&) = default;
        auto accept (IVisitor&) -> void override;
        auto run () -> void override;
        auto result () const -> TestResult override;
        auto subtests () const -> std::vector<std::unique_ptr<Test>> const&;
        auto subtests () -> std::vector<std::unique_ptr<Test>>&;

    protected:
        auto add_test (std::unique_ptr<Test>) -> void;

    private:
        std::vector<std::unique_ptr<Test>> tests_;
    };

    /**
     *  \brief Prints results of the test into console.
     *  \param t test to be printed.
     */
    auto console_print_results (Test& t) -> void;
}

#endif