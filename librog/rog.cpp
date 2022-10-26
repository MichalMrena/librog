#include <librog/rog.hpp>
#include <librog/details/console_output.hpp>

#include <algorithm>
#include <iostream>
#include <ranges>

namespace rog
{
// Test:

    Test::Test
        (std::string name) :
        name_ (std::move(name))
    {
    }

    auto Test::name
        () const -> std::string_view
    {
        return name_;
    }

// LeafTest:

    LeafTest::LeafTest
        (std::string name) :
        rog::Test::Test (std::move(name))
    {
    }

    auto LeafTest::run
        () -> void
    {
        results_.clear();
        this->test();
    }

    auto LeafTest::result
        () const -> TestResult
    {
        auto const is_pass = [](auto const r)
        {
            return r == TestMessageType::Pass;
        };
        auto asserts = results_
                     | std::ranges::views::transform(&TestMessage::type_)
                     | std::ranges::views::filter([](auto const t)
                       {
                           return t != TestMessageType::Info;
                       });
        return
            results_.empty()
                ? TestResult::NotEvaluated :
            std::ranges::all_of(asserts, is_pass)
                ? TestResult::Pass :
            std::ranges::none_of(asserts, is_pass)
                ? TestResult::Fail :
            TestResult::Partial;
    }

    auto LeafTest::output
        () const -> std::vector<TestMessage> const&
    {
        return results_;
    }

    auto LeafTest::accept
        (IVisitor& v) -> void
    {
        v.visit(*this);
    }

    auto LeafTest::assert_true
        (bool const b, std::string m) -> void
    {
        auto const type = b ? TestMessageType::Pass : TestMessageType::Fail;
        results_.emplace_back(TestMessage {type, std::move(m)});
    }

    auto LeafTest::assert_false
        (bool const b, std::string m) -> void
    {
        this->assert_true(not b, std::move(m));
    }

    auto LeafTest::assert_null
        (std::nullptr_t) -> void
    {
        this->pass("Null literal is indeed null");
    }

    auto LeafTest::assert_not_null
        (std::nullptr_t) -> void
    {
        this->fail("Null literal cannot be not null");
    }

    auto LeafTest::assert_nullopt
        (std::nullopt_t) -> void
    {
        this->pass("Nullopt literal is indeed nullopt");
    }

    auto LeafTest::assert_has_value
        (std::nullopt_t) -> void
    {
        this->fail("Nullopt literal never has value");
    }

    auto LeafTest::info
        (std::string m) -> void
    {
        results_.emplace_back(
            TestMessage {TestMessageType::Info, std::move(m)}
        );
    }

    auto LeafTest::fail
        (std::string m) -> void
    {
        results_.emplace_back(
            TestMessage {TestMessageType::Fail, std::move(m)}
        );
    }

    auto LeafTest::pass
        (std::string m) -> void
    {
        results_.emplace_back(
            TestMessage {TestMessageType::Pass, std::move(m)}
        );
    }

// CompositeTest:

    CompositeTest::CompositeTest
        (std::string name) :
        Test (std::move(name))
    {
    }

    auto CompositeTest::accept
        (IVisitor& v) -> void
    {
        v.visit(*this);
    }

    auto CompositeTest::run
        () -> void
    {
        for (auto& t : tests_)
        {
            t->run();
        }
    }

    auto CompositeTest::result
        () const -> TestResult
    {
        auto const didfail = [](auto const r)
        {
            return r == TestResult::Fail;
        };
        auto const didpass = [](auto const r)
        {
            return r == TestResult::Pass;
        };
        auto const didntev = [](auto const r)
        {
            return r == TestResult::NotEvaluated;
        };

        return
            std::ranges::all_of(tests_, didntev, &Test::result)
                ? TestResult::NotEvaluated :
            std::ranges::all_of(tests_, didfail, &Test::result)
                ? TestResult::Fail :
            std::ranges::all_of(tests_, didpass, &Test::result)
                ? TestResult::Pass :
            TestResult::Partial;
    }

    auto CompositeTest::subtests
        () const -> std::vector<std::unique_ptr<Test>> const&
    {
        return tests_;
    }

    auto CompositeTest::subtests
        () -> std::vector<std::unique_ptr<Test>>&
    {
        return tests_;
    }

    auto CompositeTest::add_test
        (std::unique_ptr<Test> t) -> void
    {
        tests_.emplace_back(std::move(t));
    }

// Free functions:

    auto console_print_results (Test& t) -> void
    {
        auto out = TestOutputterVisitor();
        t.accept(out);
    }
}