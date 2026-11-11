// cb/core/testing.hpp -- the in-tree test framework (no download, ~200 lines).
//
// A test is registered by CB_TEST("area.module.case", [] { ... }); every `<name>_test.cpp` is linked into the one
// `cb_tests` executable, which runs them all (or those whose name contains `--filter`). Checks do not abort the
// test: a failed CB_CHECK records the location and the test continues, so one run shows every failure.
//
//   CB_CHECK(cond)                    CB_CHECK_EQ(actual, expected)     CB_CHECK_NE(a, b)
//   CB_CHECK_THROWS_AS(Type, expr)    CB_CHECK_THROWS(expr)             CB_CHECK_NOTHROW(expr)
//
// cb_tests options: --filter <substring>  --list  --xml <file>  (exit code 0 if every test passed).
// See docs/CPP_DESIGN.md 7.1.
#pragma once

#include <filesystem>
#include <format>
#include <functional>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

namespace cb::testing {

struct Failure {
    std::string file;
    int line = 0;
    std::string message;
};

using TestFn = std::function<void()>;

// Registers a test at static-initialisation time (what CB_TEST expands to).
struct Registrar {
    Registrar(const char* name, TestFn fn);
};

// Record a failed check against the running test.
void record_failure(const char* file, int line, std::string message);
// Count a check (for the summary).
void count_check();

// The directory holding test data files for the module under test: <source>/native/cb. Test inputs live in
// `testdata/` folders next to the `_test.cpp` that uses them; `data_path("formats/testdata/x.bin")`.
std::filesystem::path data_path(std::string_view relative);

// Captures what the code under test writes to stdout and stderr (through cb::out/cb::err/cb::log or the C
// runtime's handles) for the life of the object; the originals are restored afterwards.
class CapturedStd {
public:
    CapturedStd();
    CapturedStd(const CapturedStd&) = delete;
    CapturedStd& operator=(const CapturedStd&) = delete;
    ~CapturedStd();
    std::string out();   // everything written to stdout so far (flushes cb::out's buffer first)
    std::string err();

private:
    std::filesystem::path dir_;
    void* saved_out_ = nullptr;
    void* saved_err_ = nullptr;
    void* out_ = nullptr;
    void* err_ = nullptr;
};

// Run the registered tests; returns the process exit code. `main` of cb_tests calls this.
int run_all(int argc, char** argv);

// Render a value for a failure message: formattable types with std::format, enums and bytes as integers,
// anything else as "<value>".
template <class T>
std::string show(const T& v) {
    if constexpr (std::is_enum_v<T>) {
        return std::to_string(static_cast<long long>(v));
    } else if constexpr (requires { std::format("{}", v); }) {
        return std::format("{}", v);
    } else if constexpr (requires { std::string_view(v); }) {
        return std::string(std::string_view(v));
    } else {
        return "<value>";
    }
}

template <class A, class B>
bool equal(const A& a, const B& b) {
    return a == b;
}

}  // namespace cb::testing

#define CB_TEST_CAT2_(a, b) a##b
#define CB_TEST_CAT_(a, b) CB_TEST_CAT2_(a, b)

// (variadic: a lambda body may contain commas outside parentheses, e.g. in a braced initialiser)
#define CB_TEST(name, ...) \
    static ::cb::testing::Registrar CB_TEST_CAT_(cb_test_registrar_, __LINE__)(name, __VA_ARGS__)

#define CB_CHECK(...)                                                                               \
    do {                                                                                            \
        ::cb::testing::count_check();                                                               \
        if (!(__VA_ARGS__))                                                                         \
            ::cb::testing::record_failure(__FILE__, __LINE__, "CB_CHECK failed: " #__VA_ARGS__);    \
    } while (false)

#define CB_CHECK_EQ(actual, expected)                                                               \
    do {                                                                                            \
        ::cb::testing::count_check();                                                               \
        const auto cb_a_ = (actual);   /* by value: the actual may be a reference into a temporary */                                                               \
        const auto cb_e_ = (expected);                                                             \
        if (!::cb::testing::equal(cb_a_, cb_e_))                                                    \
            ::cb::testing::record_failure(__FILE__, __LINE__,                                       \
                std::format("CB_CHECK_EQ failed: {} == {}\n    actual:   {}\n    expected: {}",      \
                            #actual, #expected, ::cb::testing::show(cb_a_), ::cb::testing::show(cb_e_))); \
    } while (false)

#define CB_CHECK_NE(a, b)                                                                           \
    do {                                                                                            \
        ::cb::testing::count_check();                                                               \
        if (::cb::testing::equal((a), (b)))                                                         \
            ::cb::testing::record_failure(__FILE__, __LINE__, "CB_CHECK_NE failed: " #a " != " #b); \
    } while (false)

// The expression may be any statement-like code (a call, a `throw`, another macro); it is run once.
#define CB_CHECK_THROWS_AS(Type, ...)                                                               \
    do {                                                                                            \
        ::cb::testing::count_check();                                                               \
        try {                                                                                       \
            __VA_ARGS__;                                                                            \
            ::cb::testing::record_failure(__FILE__, __LINE__, "expected " #Type " from: " #__VA_ARGS__); \
        } catch (const Type&) {                                                                     \
        } catch (const std::exception& cb_ex_) {                                                    \
            ::cb::testing::record_failure(__FILE__, __LINE__,                                       \
                std::string("expected " #Type " from: " #__VA_ARGS__ ", got: ") + cb_ex_.what());   \
        }                                                                                           \
    } while (false)

#define CB_CHECK_THROWS(...) CB_CHECK_THROWS_AS(std::exception, __VA_ARGS__)

#define CB_CHECK_NOTHROW(...)                                                                       \
    do {                                                                                            \
        ::cb::testing::count_check();                                                               \
        try {                                                                                       \
            __VA_ARGS__;                                                                            \
        } catch (const std::exception& cb_ex_) {                                                    \
            ::cb::testing::record_failure(__FILE__, __LINE__,                                       \
                std::string("unexpected exception from: " #__VA_ARGS__ ": ") + cb_ex_.what());      \
        }                                                                                           \
    } while (false)
