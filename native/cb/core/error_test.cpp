#include "cb/core/error.hpp"

#include "cb/core/testing.hpp"

using namespace cb;

CB_TEST("core.error.hierarchy", [] {
    CB_CHECK_THROWS_AS(Error, throw FormatError("pe", 0x40, "bad"));
    CB_CHECK_THROWS_AS(FormatError, throw TruncatedError("pe", 4, 8, 2));
    CB_CHECK_THROWS_AS(FormatError, throw UnsupportedError("ne", 0, "compressed"));
    CB_CHECK_THROWS_AS(IoError, throw NotFoundError("x.bin", 2));
    CB_CHECK_THROWS_AS(IoError, throw AccessError("x.bin", 5));
    CB_CHECK_THROWS_AS(ToolError, throw TimeoutError("wasm", "partial"));
    CB_CHECK_THROWS_AS(std::runtime_error, throw UsageError("bad flag"));
});

CB_TEST("core.error.messages_carry_fields", [] {
    TruncatedError t("ne.header", 0x10, 8, 3);
    CB_CHECK_EQ(t.offset(), 0x10u);
    CB_CHECK_EQ(t.wanted(), 8u);
    CB_CHECK_EQ(t.available(), 3u);
    CB_CHECK_EQ(t.reader(), "ne.header");
    CB_CHECK(std::string(t.what()).find("0x10") != std::string::npos);
    ParseError p("a.asm", 12, 5, "unexpected token");
    CB_CHECK_EQ(std::string(p.what()), "a.asm:12:5: unexpected token");
    ParseError nopos("a.asm", 0, 0, "empty");
    CB_CHECK_EQ(std::string(nopos.what()), "a.asm: empty");
    ToolError tool("wlink", 3, "boom");
    CB_CHECK_EQ(tool.tool_exit_code(), 3u);
    CB_CHECK_EQ(tool.output(), "boom");
    IoError io("dir/file.txt", 5, "denied");
    CB_CHECK(std::string(io.what()).find("dir/file.txt") != std::string::npos);
});

CB_TEST("core.error.exit_codes", [] {
    CB_CHECK_EQ(exit_code_of(FormatError("x", 0, "y")), kExitFailed);
    CB_CHECK_EQ(exit_code_of(UsageError("u")), kExitUsage);
    CB_CHECK_EQ(exit_code_of(NotFoundError("p", 2)), kExitNotFound);
    CB_CHECK_EQ(exit_code_of(InternalError("bug")), kExitInternal);
    CB_CHECK_EQ(exit_code_of(std::runtime_error("foreign")), kExitInternal);
    CB_CHECK_EQ(kExitUsage, 2);
    CB_CHECK_EQ(kExitNotFound, 3);
    CB_CHECK_EQ(kExitInternal, 70);
});

CB_TEST("core.error.assert", [] {
    CB_CHECK_NOTHROW(CB_ASSERT(1 + 1 == 2, "arithmetic"));
    CB_CHECK_THROWS_AS(InternalError, CB_ASSERT(false, "always"));
    try {
        CB_ASSERT(1 > 2, "ordering");
    } catch (const InternalError& e) {
        CB_CHECK(std::string(e.what()).find("error_test.cpp") != std::string::npos);
        CB_CHECK(std::string(e.what()).find("ordering") != std::string::npos);
    }
});

CB_TEST("core.error.kind_names", [] {
    CB_CHECK_EQ(FormatError("a", 0, "b").kind(), "FormatError");
    CB_CHECK_EQ(TruncatedError("a", 0, 1, 0).kind(), "TruncatedError");
    CB_CHECK_EQ(TimeoutError("t", "").kind(), "TimeoutError");
});

namespace {
Result<int> parse_digit(char c) {
    if (c < '0' || c > '9') return FormatError("digit", 0, "not a digit");
    return c - '0';
}
Result<int> sum_two(char a, char b) {
    CB_TRY_ASSIGN(int x, parse_digit(a));
    CB_TRY_ASSIGN(int y, parse_digit(b));
    return x + y;
}
Result<void> check_digit(char c) {
    CB_TRY(parse_digit(c));
    return {};
}
}  // namespace

CB_TEST("core.result.value_and_error", [] {
    auto ok = parse_digit('7');
    CB_CHECK(ok.ok());
    CB_CHECK_EQ(ok.value(), 7);
    CB_CHECK_EQ(ok.value_or(-1), 7);
    auto bad = parse_digit('x');
    CB_CHECK(!bad.ok());
    CB_CHECK_EQ(bad.error().kind(), "FormatError");
    CB_CHECK_EQ(bad.value_or(-1), -1);
    // value() on a failure throws the stored error with its own dynamic type
    CB_CHECK_THROWS_AS(FormatError, bad.value());
});

CB_TEST("core.result.try_macros_propagate", [] {
    CB_CHECK_EQ(sum_two('3', '4').value(), 7);
    auto bad = sum_two('3', 'z');
    CB_CHECK(!bad.ok());
    CB_CHECK_EQ(bad.error().kind(), "FormatError");
    CB_CHECK(check_digit('5').ok());
    CB_CHECK(!check_digit('q').ok());
});

CB_TEST("core.result.keeps_subtype", [] {
    Result<int> r = TruncatedError("r", 1, 4, 2);
    CB_CHECK(!r.ok());
    CB_CHECK_EQ(r.error().kind(), "TruncatedError");
    CB_CHECK_THROWS_AS(TruncatedError, r.value());
    Result<void> v = NotFoundError("f", 2);
    CB_CHECK_THROWS_AS(NotFoundError, v.value());
    Result<void> fine;
    CB_CHECK_NOTHROW(fine.value());
    CB_CHECK_THROWS_AS(InternalError, (void)fine.error());   // asking a success for its error is a bug
});
