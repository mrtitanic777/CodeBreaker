// Tests of the floating-point constants (floats.hpp). Every expected byte string is what nasm.exe makes of the same constant
// (`dd 1.5e10`, `dq -0.0`, `dt 3.14159`, the 80-bit and 128-bit and half formats, the special values, the hexadecimal,
// octal and binary forms, packed BCD, the directed roundings) and the messages are nasm's.
#include "build/x86asm/floats.hpp"

#include <string>

#include "build/x86asm/assembly.hpp"
#include "core/testing.hpp"

namespace {
using namespace cb::build::x86asm;

std::string ft_hexof(const FloatBytes& b, int n) {
    std::string out;
    for (int i = 0; i < n; ++i) {
        constexpr char d[] = "0123456789abcdef";
        out += d[b[static_cast<std::size_t>(i)] >> 4];
        out += d[b[static_cast<std::size_t>(i)] & 15];
    }
    return out;
}

// The bytes of a constant in a format, as hex (empty if the conversion failed).
std::string ft_conv(Floats& f, std::string_view text, int sign, Floatize fmt) {
    FloatBytes b{};
    if (!f.float_const(text, sign, b, fmt)) return "";
    return ft_hexof(b, kFpFormats[static_cast<std::size_t>(fmt)].bytes);
}

struct FtRow {
    Floatize fmt;
    int sign;
    const char* text;
    const char* hex;
};

// Every constant of the table: the format, the sign, the text, and the bytes nasm.exe made of `d? <text>` (see floats_rows.inc).
const FtRow kFtRows[] = {
#include "build/x86asm/floats_rows.inc"
};
}  // namespace

CB_TEST("x86asm.floats.constants", [] {
    Assembly as;
    Floats f(as);
    for (const FtRow& r : kFtRows) {
        const std::string got = ft_conv(f, r.text, r.sign, r.fmt);
        if (got != r.hex) CB_CHECK_EQ(std::string(r.text) + " -> " + got, std::string(r.text) + " -> " + r.hex);
    }
});

CB_TEST("x86asm.floats.the_examples", [] {
    Assembly as;
    Floats f(as);
    CB_CHECK_EQ(ft_conv(f, "1.5e10", 1, FLOAT_32), "75845f50");                 // dd 1.5e10
    CB_CHECK_EQ(ft_conv(f, "0.0", -1, FLOAT_64), "0000000000000080");           // dq -0.0
    CB_CHECK_EQ(ft_conv(f, "3.14159", 1, FLOAT_80M), "1d7233dc80cf0fc90040");   // dt 3.14159
}  );

CB_TEST("x86asm.floats.default_formats", [] {
    CB_CHECK_EQ(static_cast<int>(Floats::float_deffmt(2)), static_cast<int>(FLOAT_16));
    CB_CHECK_EQ(static_cast<int>(Floats::float_deffmt(4)), static_cast<int>(FLOAT_32));
    CB_CHECK_EQ(static_cast<int>(Floats::float_deffmt(8)), static_cast<int>(FLOAT_64));
    CB_CHECK_EQ(static_cast<int>(Floats::float_deffmt(10)), static_cast<int>(FLOAT_80M));
    CB_CHECK_EQ(static_cast<int>(Floats::float_deffmt(16)), static_cast<int>(FLOAT_128L));
    CB_CHECK_EQ(static_cast<int>(Floats::float_deffmt(1)), static_cast<int>(FLOAT_8));
    CB_CHECK_EQ(static_cast<int>(Floats::float_deffmt(3)), static_cast<int>(FLOAT_ERR));
    CB_CHECK_EQ(static_cast<int>(Floats::float_deffmt(32)), static_cast<int>(FLOAT_ERR));
});

CB_TEST("x86asm.floats.rounding_options", [] {
    // nasm: [float <mode>]  dd 0.1, -0.1, 16777217.0, -16777217.0, 1e-46, 3.4028236e38 / dq 0.1, -0.1 / dt 0.1
    struct Mode {
        const char* name;
        const char* dd[6];
        const char* dq[2];
        const char* dt;
    };
    static const Mode modes[] = {
        {"near", {"cdcccc3d", "cdccccbd", "0000804b", "000080cb", "00000000", "0000807f"}, {"9a9999999999b93f", "9a9999999999b9bf"}, "cdccccccccccccccfb3f"},
        {"up", {"cdcccc3d", "ccccccbd", "0000804b", "000080cb", "00000000", "0000807f"}, {"9a9999999999b93f", "999999999999b9bf"}, "cdccccccccccccccfb3f"},
        {"down", {"cccccc3d", "cdccccbd", "0000804b", "000080cb", "00000000", "ffff7f7f"}, {"999999999999b93f", "9a9999999999b9bf"}, "ccccccccccccccccfb3f"},
        {"zero", {"cccccc3d", "ccccccbd", "0000804b", "000080cb", "00000000", "ffff7f7f"}, {"999999999999b93f", "999999999999b9bf"}, "ccccccccccccccccfb3f"},
    };
    for (const Mode& m : modes) {
        Assembly as;
        Floats f(as);
        CB_CHECK_EQ(f.float_option(m.name), 0);
        CB_CHECK_EQ(ft_conv(f, "0.1", 1, FLOAT_32), m.dd[0]);
        CB_CHECK_EQ(ft_conv(f, "0.1", -1, FLOAT_32), m.dd[1]);
        CB_CHECK_EQ(ft_conv(f, "16777217.0", 1, FLOAT_32), m.dd[2]);
        CB_CHECK_EQ(ft_conv(f, "16777217.0", -1, FLOAT_32), m.dd[3]);
        CB_CHECK_EQ(ft_conv(f, "1e-46", 1, FLOAT_32), m.dd[4]);
        CB_CHECK_EQ(ft_conv(f, "3.4028236e38", 1, FLOAT_32), m.dd[5]);
        CB_CHECK_EQ(ft_conv(f, "0.1", 1, FLOAT_64), m.dq[0]);
        CB_CHECK_EQ(ft_conv(f, "0.1", -1, FLOAT_64), m.dq[1]);
        CB_CHECK_EQ(ft_conv(f, "0.1", 1, FLOAT_80M), m.dt);
    }
});

CB_TEST("x86asm.floats.options", [] {
    Assembly as;
    Floats f(as);
    CB_CHECK_EQ(f.float_option("DAZ"), 0);
    CB_CHECK(f.daz());
    CB_CHECK_EQ(ft_conv(f, "1e-45", 1, FLOAT_32), "00000000");    // nasm: [float daz] dd 1e-45 -> 0
    CB_CHECK_EQ(ft_conv(f, "1e-40", 1, FLOAT_32), "00000000");
    CB_CHECK_EQ(f.float_option("nodaz"), 0);
    CB_CHECK_EQ(ft_conv(f, "1e-45", 1, FLOAT_32), "01000000");
    CB_CHECK_EQ(f.float_option("zero"), 0);
    CB_CHECK(f.rounding() == FloatRound::Zero);
    CB_CHECK_EQ(f.float_option("default"), 0);
    CB_CHECK(f.rounding() == FloatRound::Near && !f.daz());
    CB_CHECK_EQ(f.float_option("bogus"), -1);   // nasm: "unknown 'float' directive: bogus" is the directive's message
});

CB_TEST("x86asm.floats.errors_and_warnings", [] {
    Assembly as;
    Floats f(as);
    as.src.set(2, "t.asm");
    for (int i = 0; i < 4; ++i) {   // the warnings of a constant are for the final pass
        as.error_pass_end();
        as.begin_pass();
    }

    // nasm: dd 1e50 -> "overflow in floating-point constant", the result is infinity.
    CB_CHECK_EQ(ft_conv(f, "1e50", 1, FLOAT_32), "0000807f");
    CB_CHECK_EQ(as.diagnostics().size(), std::size_t{1});
    CB_CHECK_EQ(as.diagnostics()[0].text, "t.asm:2: warning: overflow in floating-point constant [-w+float-overflow]");
    as.clear_diagnostics();

    // nasm: dd 1.2.3
    FloatBytes b{};
    CB_CHECK(f.float_const("1.2.3", 1, b, FLOAT_32));    // an error is reported, and the result is a quiet NaN
    CB_CHECK_EQ(as.diagnostics()[0].text, "t.asm:2: error: too many periods in floating-point constant");
    CB_CHECK_EQ(ft_hexof(b, 4), "0000c07f");
    as.clear_diagnostics();

    // nasm: dd 0x1.8q
    f.float_const("0x1.8q", 1, b, FLOAT_32);
    CB_CHECK_EQ(as.diagnostics()[0].text, "t.asm:2: error: floating-point constant: `q' is invalid character");
    as.clear_diagnostics();

    // nasm: dd 0p12345 (a packed BCD constant needs the 80-bit format)
    CB_CHECK(!f.float_const("0p12345", 1, b, FLOAT_32));
    CB_CHECK_EQ(as.diagnostics()[0].text, "t.asm:2: error: packed BCD requires an 80-bit format");
    as.clear_diagnostics();

    // nasm: dt 12345678901234567890p
    CB_CHECK_EQ(ft_conv(f, "12345678901234567890p", 1, FLOAT_80M), "90785634129078563400");
    CB_CHECK_EQ(as.diagnostics()[0].text, "t.asm:2: warning: packed BCD truncated to 18 digits [-w+other]");
    as.clear_diagnostics();

    // The significand has more than 52 digits.
    ft_conv(f, "12345678901234567890123456789012345678901234567890123456789012345678.0", 1, FLOAT_32);
    CB_CHECK_EQ(as.diagnostics().size(), std::size_t{2});
    CB_CHECK_EQ(as.diagnostics()[0].text, "t.asm:2: warning: floating-point constant significand contains more than 52 digits [-w+float-toolong]");
    CB_CHECK_EQ(as.diagnostics()[1].text, "t.asm:2: warning: overflow in floating-point constant [-w+float-overflow]");
});
