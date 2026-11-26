// Tests of dotnet/decompiler's disassembler slice (the IL of a module, as `ilspycmd -il` writes it) and of the .NET text rules it rests on
// (char_info, number_format). The goldens: what the installed ilspycmd 11.0 wrote for the test assemblies, and what .NET itself writes for
// 16,000 floats and doubles (testdata/oracle/NetText); src\dotnet\testdata\build.cmd remakes them.
#include "dotnet/decompiler/disassembler/reflection_disassembler.hpp"

#include <bit>
#include <sstream>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "dotnet/decompiler/disassembler/disassembler_helpers.hpp"
#include "dotnet/decompiler/metadata/metadata_file.hpp"
#include "dotnet/decompiler/util/char_info.hpp"
#include "dotnet/decompiler/util/number_format.hpp"

namespace cb::dotnet::decompiler {
namespace {

std::filesystem::path testdata(std::string_view relative) { return testing::data_path(std::string("dotnet/testdata/") + std::string(relative)); }

// The first line where two texts differ, for the failure message.
std::string first_difference(const std::string& actual, const std::string& expected) {
    std::istringstream a(actual), e(expected);
    std::string al, el;
    for (int line = 1;; ++line) {
        const bool ha = static_cast<bool>(std::getline(a, al)), he = static_cast<bool>(std::getline(e, el));
        if (!ha && !he) return "(the same lines; a difference in line endings)";
        if (al != el || ha != he) return std::format("line {}:\n  ours:    {}\n  ilspycmd: {}", line, ha ? al : "<end>", he ? el : "<end>");
    }
}

void check_il(std::string_view name) {
    const auto module = metadata::PEFile::open(testdata(std::string("inputs/") + std::string(name) + ".dll"));
    const std::string actual = disassembler::show_il(*module);
    const std::string expected = testing::regold_golden(testdata(std::string("golden/") + std::string(name) + ".il.txt"), actual);
    CB_CHECK(actual == expected);
    if (actual != expected) testing::record_failure(__FILE__, __LINE__, first_difference(actual, expected));
}

CB_TEST("dotnet.decompiler.disassembler.il_pe32_matches_ilspycmd", [] { check_il("Sample"); });
CB_TEST("dotnet.decompiler.disassembler.il_pe32plus_matches_ilspycmd", [] { check_il("Sample64"); });

CB_TEST("dotnet.decompiler.util.number_format_matches_dotnet", [] {
    std::istringstream in(fs::read_text(testdata("golden/numbers.txt")));
    std::string kind, bits, text;
    int checked = 0, wrong = 0;
    while (in >> kind >> bits >> text) {
        std::string ours;
        if (kind == "d") ours = util::format_r(std::bit_cast<double>(static_cast<std::uint64_t>(std::stoull(bits, nullptr, 16))));
        else ours = util::format_r(std::bit_cast<float>(static_cast<std::uint32_t>(std::stoul(bits, nullptr, 16))));
        ++checked;
        if (ours != text && wrong++ < 5) testing::record_failure(__FILE__, __LINE__, std::format("{} {}: ours {}, .NET {}", kind, bits, ours, text));
    }
    CB_CHECK(checked > 16000);
    CB_CHECK_EQ(wrong, 0);
});

CB_TEST("dotnet.decompiler.util.char_info", [] {
    CB_CHECK(util::is_letter_or_digit(u'a'));
    CB_CHECK(util::is_letter_or_digit(u'é'));   // é
    CB_CHECK(util::is_letter_or_digit(u'٣'));   // Arabic-Indic three
    CB_CHECK(!util::is_letter_or_digit(u'<'));
    CB_CHECK(util::is_digit(u'7'));
    CB_CHECK(util::is_control(u'\x01'));
    CB_CHECK(util::is_white_space(u' '));
    CB_CHECK(util::is_white_space(u' '));
    CB_CHECK(!util::is_white_space(u'x'));
    CB_CHECK_EQ(util::utf8(util::utf16("Gr\xc3\xb6\xc3\x9f" "e \xf0\x9f\x98\x80")), std::string("Gr\xc3\xb6\xc3\x9f" "e \xf0\x9f\x98\x80"));
});

CB_TEST("dotnet.decompiler.disassembler.escape", [] {
    CB_CHECK_EQ(disassembler::escape("Main"), std::string("Main"));
    CB_CHECK_EQ(disassembler::escape(".ctor"), std::string(".ctor"));
    CB_CHECK_EQ(disassembler::escape("<Main>$"), std::string("'<Main>$'"));
    CB_CHECK_EQ(disassembler::escape("type"), std::string("'type'"));   // an ILAsm keyword
    CB_CHECK_EQ(disassembler::escape("it's"), std::string("'it\\'s'"));
    CB_CHECK_EQ(disassembler::escape_string(u"a\"b\\c\n\u0001"), std::string("a\\\"b\\\\c\\n\\u0001"));
    CB_CHECK_EQ(disassembler::offset_to_string(42), std::string("IL_002a"));
});

}  // namespace
}  // namespace cb::dotnet::decompiler
