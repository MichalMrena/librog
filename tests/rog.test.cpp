#include <memory>
#include <optional>
#include <ostream>
#include <string>
#if __has_include(<format>)
#include <format>
#endif
#include <librog/rog.hpp>

namespace adl
{
    struct ToStringAbleDummy
    {
    };

    auto to_string (ToStringAbleDummy const&) -> std::string
    {
        return "#ToStringAbleDummy";
    }

    auto operator== (ToStringAbleDummy, ToStringAbleDummy) -> bool
    {
        return true;
    }

    auto operator!= (ToStringAbleDummy, ToStringAbleDummy) -> bool
    {
        return false;
    }
}



struct OstreamAbleDummy
{
};

auto operator<< (std::ostream& ost, OstreamAbleDummy const&) -> std::ostream&
{
    ost << "#OstreamAbleDummy";
    return ost;
}

auto operator== (OstreamAbleDummy, OstreamAbleDummy) -> bool
{
    return true;
}

auto operator!= (OstreamAbleDummy, OstreamAbleDummy) -> bool
{
    return false;
}

struct FormatAbleDummy
{
};

#if __has_include(<format>)
template <>
struct std::formatter<FormatAbleDummy> : std::formatter<std::string> {
    auto format(FormatAbleDummy p, format_context& ctx)
    {
        return formatter<string>::format(std::format("#FormatAbleDummy"), ctx);
    }
};
#endif

auto operator== (FormatAbleDummy, FormatAbleDummy) -> bool
{
    return true;
}

auto operator!= (FormatAbleDummy, FormatAbleDummy) -> bool
{
    return false;
}

struct NoStringAbleDummy
{
};

auto operator== (NoStringAbleDummy, NoStringAbleDummy) -> bool
{
    return true;
}

auto operator!= (NoStringAbleDummy, NoStringAbleDummy) -> bool
{
    return false;
}

class DummyTest : public rog::LeafTest
{
public:
    DummyTest () :
        rog::LeafTest("Dummy", rog::AssertPolicy::StopAtFirstFail)
    {
    }

protected:
    auto test () -> void override
    {
        this->info("Following should pass:");
        this->assert_true(true, "Assert true");
        this->assert_false(false, "Assert false");
        this->assert_equals(2, 2);
        this->assert_not_equals(1, 2);
        this->assert_equals(.100000000001, .1, .0001);
        this->assert_not_equals(.1, .2, .00000001);
        this->assert_throws([](){ throw 1; });
        this->assert_null(nullptr);
        this->assert_equals(adl::ToStringAbleDummy(), adl::ToStringAbleDummy());
        this->assert_equals(OstreamAbleDummy(), OstreamAbleDummy());
        this->assert_equals(FormatAbleDummy(), FormatAbleDummy());
        this->assert_nullopt(std::nullopt);
        this->assert_has_value(std::optional<int>(10));

        this->info("Following shall not pass:");
        this->assert_true(false, "Assert true");
        this->assert_false(true, "Assert false");
        this->assert_equals(1, 2);
        this->assert_not_equals(2, 2);
        this->assert_not_equals(1.2001, 1.2002, 0.1);
        // this->assert_equals(.2, .1);
        // this->assert_not_equals(.2, .2);
        this->assert_throws([](){  });
        this->assert_not_null(nullptr);
        this->assert_nullopt(std::optional<int>(10));
        this->assert_has_value(std::nullopt);
    }
};

auto main () -> int
{
    auto t = DummyTest();
    t.run();
    rog::console_print_results(t, rog::ConsoleOutputType::NoLeaf);
}