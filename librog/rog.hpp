#ifndef ROG_ROG_HPP
#define ROG_ROG_HPP

#include <cmath>
#include <concepts>
#include <cstddef>
#include <functional>
#include <iomanip>
#include <limits>
#include <memory>
#include <optional>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#include <librog/details/console_output.hpp>
#include <librog/details/concepts.hpp>
#include <librog/visitors.hpp>

#if __has_include(<format>)
#include <format>
#endif

namespace rog
{
    /**
     *  \brief Result of a Test.
     */
    enum class TestResult
    {
        Pass,
        Fail,
        Partial,
        NotEvaluated
    };

    /**
     *  \brief Type of a test message.
     */
    enum class TestMessageType
    {
        Pass,
        Fail,
        Info
    };

    /**
     *  \brief Message logged by a test.
     */
    struct TestMessage
    {
        TestMessageType type_;
        std::string text_;
    };

    /**
     *  \brief Specifies behavior of failed assertion.
     */
    enum class AssertPolicy
    {
        StopAtFirstFail,
        RunAll
    };

    /**
     *  \brief Common base class for tests.
     */
    class Test : public IVisitable
    {
    public:
        virtual ~Test () = default;

        /**
         *  \brief Runs the test. Implemented by child classes.
         */
        virtual auto run () -> void = 0;

        /**
         *  \brief Returns result of the test. Implemented by child classes.
         *  \return Result of the test.
         */
        virtual auto result () const -> TestResult = 0;

        /**
         *  \brief Returns name of the test.
         *  \return Name of the test.
         */
        auto name () const -> std::string_view;

    protected:
        /**
         *  \brief Initializes the test with \p name .
         *  \param name name of the test.
         */
        Test (std::string name);

    private:
        std::string name_;
    };

    /**
     *  \brief Base class for implementation of a single test.
     */
    class LeafTest : public Test
    {
    public:
        /**
         *  \brief Initializes the test with \p name .
         *  \param name name of the test.
         *  \param policy specifies behavior after first failed assertion.
         */
        LeafTest (
            std::string name,
            AssertPolicy policy = AssertPolicy::StopAtFirstFail
        );

        /**
         *  \brief Runs the test.
         */
        auto run () -> void override final;

        /**
         *  \brief Returns result of the test.
         *  \return Result of the test.
         */
        auto result () const -> TestResult override;

        /**
         *  \brief Returns output of the test.
         *  \return Vector of messages produced by the test.
         */
        auto output () const -> std::vector<TestMessage> const&;

        /**
         *  \brief Implements the visitor design patter.
         *  \param visitor visitor.
         */
        auto accept (IVisitor& visitor) -> void override;

    protected:
        /**
         *  \brief Child classes implements the test in this method.
         */
        virtual auto test () -> void = 0;

        /**
         *  \brief Asserts that \p b is true.
         *  \param b condition to be checked.
         *  \param message message that describes the assertion.
         */
        auto assert_true (bool b, std::string message) -> void;

        /**
         *  \brief Asserts that \p b is false.
         *  \param b condition to be checked.
         *  \param message message that describes the assertion.
         */
        auto assert_false (bool b, std::string message) -> void;

        /**
         *  \brief Asserts that \p expected and \p actual are equal.
         *
         *  Tries to print values of compared objects using to_string function,
         *  operator<<, and std::format.
         *
         *  \tparam T comparable non floating-point data type.
         *  \param expected expected value.
         *  \param actual actual value.
         */
        template<class T>
        requires (std::equality_comparable<T> && not std::floating_point<T>)
        auto assert_equals (T const& expected, T const& actual) -> void;

        /**
         *  \brief Asserts that \p expected and \p actual are equal.
         *
         *  Tries to print values of compared objects using to_string function,
         *  operator<<, and std::format.
         *
         *  \tparam T comparable non floating-point data type.
         *  \param expected expected value.
         *  \param actual actual value.
         *  \param message message that describes the assertion.
         */
        template<class T>
        requires (std::equality_comparable<T> && not std::floating_point<T>)
        auto assert_equals (
            T const& expected,
            T const& actual,
            std::string message
        ) -> void;

        /**
         *  \brief Asserts that \p expected and \p actual are equal
         *  using precision defined by \p epsilon .
         *
         *  Tries to print values of compared objects using to_string function,
         *  operator<<, and std::format.
         *
         *  \tparam T floating-point data type.
         *  \param expected expected value.
         *  \param actual actual value.
         *  \param epsilon precision used in the comparison.
         */
        template<class T>
        requires std::equality_comparable<T> && std::floating_point<T>
        auto assert_equals (T expected, T actual, T epsilon) -> void;


        /**
         *  \brief Asserts that \p expected and \p actual are equal
         *  using precision defined by \p epsilon .
         *
         *  Tries to print values of compared objects using to_string function,
         *  operator<<, and std::format.
         *
         *  \tparam T floating-point data type.
         *  \param expected expected value.
         *  \param actual actual value.
         *  \param epsilon precision used in the comparison.
         *  \param message message that describes the assertion.
         */
        template<class T>
        requires std::equality_comparable<T> && std::floating_point<T>
        auto assert_equals (
            T expected,
            T actual,
            T epsilon,
            std::string message
        ) -> void;

        /**
         *  \brief Asserts that \p expected and \p actual are different.
         *
         *  Tries to print values of compared objects using to_string function,
         *  operator<<, and std::format.
         *
         *  \tparam T comparable non floating-point data type.
         *  \param expected expected value.
         *  \param actual actual value.
         */
        template<class T>
        requires (std::equality_comparable<T> && not std::floating_point<T>)
        auto assert_not_equals (T const& expected, T const& actual) -> void;

        /**
         *  \brief Asserts that \p expected and \p actual are different.
         *
         *  Tries to print values of compared objects using to_string function,
         *  operator<<, and std::format.
         *
         *  \tparam T comparable non floating-point data type.
         *  \param expected expected value.
         *  \param actual actual value.
         *  \param message message that describes the assertion.
         */
        template<class T>
        requires (std::equality_comparable<T> && not std::floating_point<T>)
        auto assert_not_equals (
            T const& expected,
            T const& actual,
            std::string message
        ) -> void;

        /**
         *  \brief Asserts that \p expected and \p actual are different
         *  using precision defined by \p epsilon .
         *
         *  Tries to print values of compared objects using to_string function,
         *  operator<<, and std::format.
         *
         *  \tparam T floating-point data type.
         *  \param expected expected value.
         *  \param actual actual value.
         *  \param epsilon precision used in the comparison.
         */
        template<class T>
        requires std::equality_comparable<T> && std::floating_point<T>
        auto assert_not_equals (T expected, T actual, T epsilon) -> void;

        /**
         *  \brief Asserts that \p expected and \p actual are different
         *  using precision defined by \p epsilon .
         *
         *  Tries to print values of compared objects using to_string function,
         *  operator<<, and std::format.
         *
         *  \tparam T floating-point data type.
         *  \param expected expected value.
         *  \param actual actual value.
         *  \param epsilon precision used in the comparison.
         *  \param message message that describes the assertion.
         */
        template<class T>
        requires std::equality_comparable<T> && std::floating_point<T>
        auto assert_not_equals (
            T expected,
            T actual,
            T epsilon,
            std::string message
        ) -> void;

        /**
         *  \brief Asserts that callable object \p f throws something.
         *  \tparam F callable object type.
         *  \param f callable object.
         */
        template<std::invocable F>
        auto assert_throws (F f) -> void;

        /**
         *  \brief Asserts that callable object \p f throws something.
         *  \tparam F callable object type.
         *  \param f callable object.
         *  \param message message that describes the assertion.
         */
        template<std::invocable F>
        auto assert_throws (F f, std::string message) -> void;

        /**
         *  \brief Asserts that null literal is nullptr which is indeed true.
         */
        auto assert_null (std::nullptr_t) -> void;

        /**
         *  \brief Asserts that \p is nullptr.
         *  \param p pointer
         */
        template<class T>
        auto assert_null (T* p) -> void;

        /**
         *  \brief Asserts that \p is nullptr.
         *  \param p pointer
         */
        template<class T, class D>
        auto assert_null (std::unique_ptr<T, D> const& p) -> void;

        /**
         *  \brief Asserts that \p is nullptr.
         *  \param p pointer
         */
        template<class T>
        auto assert_null (std::shared_ptr<T> const& p) -> void;

        /**
         *  \brief Asserts that null literal is not nullptr which is never true.
         */
        auto assert_not_null (std::nullptr_t) -> void;

        /**
         *  \brief Asserts that \p p is not nullptr.
         *  \param p pointer
         */
        template<class T>
        auto assert_not_null (T* p) -> void;

        /**
         *  \brief Asserts that \p p is not nullptr.
         *  \param p pointer
         */
        template<class T, class D>
        auto assert_not_null (std::unique_ptr<T, D> const& p) -> void;

        /**
         *  \brief Asserts that \p p is not nullptr.
         *  \param p pointer
         */
        template<class T>
        auto assert_not_null (std::shared_ptr<T> const& p) -> void;

        /**
         *  \brief Asserts that nullopt literal is nullopt which is indeed true.
         */
        auto assert_nullopt (std::nullopt_t) -> void;

        /**
         *  \brief Asserts that \p o is nullopt.
         *  \param o optional
         */
        template<class T>
        auto assert_nullopt (std::optional<T> const& o) -> void;

        /**
         *  \brief Asserts that nullopt literal has value which is always false.
         */
        auto assert_has_value (std::nullopt_t) -> void;

        /**
         *  \brief Asserts that \p o contains value.
         *  \param o optional
         */
        template<class T>
        auto assert_has_value (std::optional<T> const& o) -> void;

        /**
         *  \brief Logs informational message.
         *  \param message message to be logged.
         */
        auto info (std::string message) -> void;

        /**
         *  \brief Fails the test as if \c asert_true(false,message) .
         *  \param message message to be logged.
         */
        auto fail (std::string message) -> void;

        /**
         *  \brief Logs passed assertion as if \c asert_true(true,message) .
         *  \param message message to be logged.
         */
        auto pass (std::string message) -> void;

    private:
        /**
         *  \brief Logs failed assertion.
         *  \param message message to be logged.
         */
        auto log_fail (std::string message) -> void;

    private:
        std::vector<TestMessage> results_;
        AssertPolicy assertPolicy_;
    };

    /**
     *  \brief Container for tests. Creates hierarchy of tests.
     */
    class CompositeTest : public Test
    {
    public:
        /**
         *  \brief Initializes the test with \p name .
         *  \param name name of the test.
         */
        CompositeTest (std::string name);

        /**
         *  \brief Deleted copy constructor.
         *  It is up to the user to create a deep copy if necessary.
         */
        CompositeTest (CompositeTest const&) = delete;

        /**
         *  \brief Compiler generated move constructor.
         */
        CompositeTest (CompositeTest&&) = default;

        /**
         *  \brief Implements the visitor design patter.
         *  Does NOT proceed with visiting subtests.
         *  \param visitor visitor.
         */
        auto accept (IVisitor& visitor) -> void override;

        /**
         *  Runs all substests.
         */
        auto run () -> void override final;

        /**
         *  \brief Returns result of the test.
         *  Pass if all subtests passed.
         *  Fail if all subtests failed.
         *  Partial if some subtests passed and some failed.
         *  NotEvaluated if there is no output.
         *  \return Resuslt of the test.
         */
        auto result () const -> TestResult override;

        /**
         *  \brief Returns subtests.
         *  \return Vector of subtests.
         */
        auto subtests () const -> std::vector<std::unique_ptr<Test>> const&;

        /**
         *  \brief Returns subtests.
         *  \return Vector of subtests.
         */
        auto subtests () -> std::vector<std::unique_ptr<Test>>&;

    protected:
        /**
         *  \brief Adds new subtest.
         *  \param t subtest.
         */
        auto add_test (std::unique_ptr<Test> t) -> void;

    private:
        std::vector<std::unique_ptr<Test>> tests_;
    };

    /**
     *  \brief Prints results of the test into console.
     *  It is best to use this with the root test.
     *  \param t test to be printed.
     *  \param o specifies level of details in the output.
     */
    auto console_print_results (
        Test& t,
        ConsoleOutputType o = ConsoleOutputType::Full
    ) -> void;

// LeafTest:

    namespace details
    {
        template<class T>
        auto try_print (T const& e) -> std::optional<std::string>
        {
            return std::invoke([&]()
            {
                if constexpr (ToStringAble<T>)
                {
                    using std::to_string;
                    return to_string(e);
                }
                else if constexpr (OstreamAble<T>)
                {
                    auto ost = std::ostringstream();
                    ost << e;
                    return ost.str();
                }
                #if __has_include(<format>)
                else if constexpr (FormatAble<T>)
                {
                    return std::format("{}", e);
                }
                #endif
                else
                {
                    return std::nullopt;
                }
            });
        }
    }

    template<class T>
    requires (std::equality_comparable<T> && not std::floating_point<T>)
    auto LeafTest::assert_equals (T const& expected, T const& actual) -> void
    {
        auto const expectedStr = details::try_print(expected);
        auto const actualStr = details::try_print(actual);
        this->assert_equals(
            expected,
            actual,
            expectedStr && actualStr
                ? "Expected " + *expectedStr + " got " + *actualStr
                : "Expected value equals to the actual value"
        );
    }

    template<class T>
    requires (std::equality_comparable<T> && not std::floating_point<T>)
    auto LeafTest::assert_equals (
        T const& expected,
        T const& actual,
        std::string m
    ) -> void
    {
        this->assert_true(expected == actual, std::move(m));
    }

    template<class T>
    requires std::equality_comparable<T> && std::floating_point<T>
    auto LeafTest::assert_equals (T expected, T actual, T epsilon) -> void
    {
        auto ost = std::ostringstream();

        ost.precision(std::numeric_limits<double>::max_digits10);
        ost << "Expected " << expected
            << " got " << actual
            << " using presision " << epsilon;

        this->assert_equals(
            expected,
            actual,
            epsilon,
            ost.str()
        );
    }

    template<class T>
    requires std::equality_comparable<T> && std::floating_point<T>
    auto LeafTest::assert_equals (
        T expected,
        T actual,
        T epsilon,
        std::string m
    ) -> void
    {
        this->assert_true(std::abs(expected - actual) < epsilon, std::move(m));
    }

    template<class T>
    requires (std::equality_comparable<T> && not std::floating_point<T>)
    auto LeafTest::assert_not_equals (
        T const& expected,
        T const& actual
    ) -> void
    {
        auto const expectedStr = details::try_print(expected);
        auto const actualStr = details::try_print(actual);
        this->assert_not_equals(
            expected,
            actual,
            expectedStr && actualStr
                ? "Expected " + *expectedStr + " and " + *actualStr +
                  " to be different"
                : "Values should be different"
        );
    }

    template<class T>
    requires (std::equality_comparable<T> && not std::floating_point<T>)
    auto LeafTest::assert_not_equals (
        T const& expected,
        T const& actual,
        std::string m
    ) -> void
    {
        this->assert_true(expected != actual, std::move(m));
    }

    template<class T>
    requires std::equality_comparable<T> && std::floating_point<T>
    auto LeafTest::assert_not_equals (
        T expected,
        T actual,
        T epsilon
    ) -> void
    {
        auto ost = std::ostringstream();

        ost.precision(std::numeric_limits<double>::max_digits10);
        ost << "Expected " << expected
            << " and " << actual
            << " to be different using presision " << epsilon;

        this->assert_not_equals(
            expected,
            actual,
            epsilon,
            ost.str()
        );
    }

    template<class T>
    requires std::equality_comparable<T> && std::floating_point<T>
    auto LeafTest::assert_not_equals (
        T expected,
        T actual,
        T epsilon,
        std::string m
    ) -> void
    {
        this->assert_true(std::abs(expected - actual) >= epsilon, std::move(m));
    }

    template<std::invocable F>
    auto LeafTest::assert_throws
        (F f) -> void
    {
        this->assert_throws(f, "Function throws");
    }

    template<std::invocable F>
    auto LeafTest::assert_throws
        (F f, std::string m) -> void
    {
        try
        {
            std::invoke(f);
            this->fail(std::move(m));
        }
        catch (...)
        {
            this->pass(std::move(m));
        }
    }

    template<class T>
    auto LeafTest::assert_null
        (T* p) -> void
    {
        this->assert_true(not p, "Pointer is nullptr");
    }

    template<class T, class D>
    auto LeafTest::assert_null
        (std::unique_ptr<T, D> const& p) -> void
    {
        this->assert_true(not p.get(), "Pointer is nullptr");
    }

    template<class T>
    auto LeafTest::assert_null
        (std::shared_ptr<T> const& p) -> void
    {
        this->assert_true(not p.get(), "Pointer is nullptr");
    }

    template<class T>
    auto LeafTest::assert_not_null
        (T* p) -> void
    {
        this->assert_true(p, "Pointer is not nullptr");
    }

    template<class T, class D>
    auto LeafTest::assert_not_null
        (std::unique_ptr<T, D> const& p) -> void
    {
        this->assert_true(p.get(), "Pointer is not nullptr");
    }

    template<class T>
    auto LeafTest::assert_not_null
        (std::shared_ptr<T> const& p) -> void
    {
        this->assert_true(p.get(), "Pointer is not nullptr");
    }

    template<class T>
    auto LeafTest::assert_nullopt
        (std::optional<T> const& o) -> void
    {
        this->assert_true(not o.has_value(), "Optional is nullopt");
    }

    template<class T>
    auto LeafTest::assert_has_value
        (std::optional<T> const& o) -> void
    {
        this->assert_true(o.has_value(), "Optional has value");
    }
}

#endif