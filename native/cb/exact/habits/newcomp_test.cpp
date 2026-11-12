#include "cb/exact/habits/newcomp.hpp"

#include <format>
#include <sstream>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

using namespace cb;
using namespace cb::exact::habits;
namespace nc = cb::exact::habits::newcomp;

// testdata/newcomp.cases: 688 calls of the original (codebreaker.exact.habits.newcomp) on procedures a script wrote in
// the shapes the two compilers' habits give - for each habit, the 1981 code and the later one, with and without a renaming
// in force, and the ways each fails - with what the original answered: whether the two are the same, the notes (or where
// it stopped), the alignment of the instructions and where the compiled code's instructions begin. A line is
//
//     eq|eq3 <bytes after the entry helper's call: 3 or 2> <debug> <original, hex> <compiled, hex> <linker words, or ->
//     mask <text> | rel <hex> | rename <instruction> <family=register,..>      then " => " and the answer as JSON.
//
// The cases were read through for what each habit is: the notes are the reading of the code (REVIEWED: for the 159 that
// are the same, the habit named is the one the case was written to show; the rest stop where the code differs).
//
// DIVERGES: a procedure cut short in the middle of an instruction or its entry (two of the cases) raised IndexError in the
// original; here it is answered ("no pattern", "the entry is not call helper / dw frame").
// The text of an instruction with no operand has no trailing blank (the original's "ret "), which shows in the
// notes that name an instruction; nothing else in what is answered. The original's module tables DEBUG, AMAP and GWALK are not carried over: the
// alignment and the compiled code's instruction starts are part of the answer (`Finding`); DEBUG's lines are not printed.

namespace {

std::string newcomp_test_json(std::string_view s) {
    std::string out = "\"";
    for (const unsigned char c : s) {
        if (c == '"' || c == '\\') {
            out += '\\';
            out += static_cast<char>(c);
        } else if (c == '\n') {
            out += "\\n";
        } else if (c < 0x20 || c >= 0x7f) {
            out += std::format("\\u{:04x}", c);
        } else {
            out += static_cast<char>(c);
        }
    }
    return out + "\"";
}

Bytes newcomp_test_unhex(std::string_view h) {
    Bytes out;
    if (h == "-") return out;
    for (std::size_t i = 0; i + 1 < h.size(); i += 2) out.push_back(static_cast<std::uint8_t>(std::stoi(std::string(h.substr(i, 2)), nullptr, 16)));
    return out;
}

std::string newcomp_test_finding(const nc::Finding& f) {
    std::string out = std::string("[") + (f.same ? "true" : "false") + ",[";
    for (std::size_t i = 0; i < f.notes.size(); ++i) out += (i ? "," : "") + newcomp_test_json(f.notes[i]);
    out += "],[";
    for (std::size_t i = 0; i < f.alignment.size(); ++i) out += std::format("{}[{},{}]", i ? "," : "", f.alignment[i].first, f.alignment[i].second);
    out += "],[";
    for (std::size_t i = 0; i < f.compiled_starts.size(); ++i) out += std::format("{}{}", i ? "," : "", f.compiled_starts[i]);
    return out + "]]";
}

}  // namespace

CB_TEST("exact.habits.newcomp.the_originals_answers", [] {
    const std::string file = fs::read_text(testing::data_path("exact/habits/testdata/newcomp.cases"));
    std::size_t n = 0, bad = 0, errors = 0;
    for (const std::string_view line : text::lines(file)) {
        if (line.empty()) continue;
        const std::size_t arrow = line.find(" => ");
        const auto c = text::split(line.substr(0, arrow), '\t');
        // (the original's text of an instruction with no operand ends in a blank - capstone's "ret " - cmpp::walk gives "ret")
        const std::string want = text::replace_all(text::replace_all(std::string(line.substr(arrow + 4)), " , compiled", ", compiled"), " \"]", "\"]");
        std::string got;
        if (c[0] == "eq" || c[0] == "eq3") {
            cmpp::Options options;
            options.entry_data = c[1] == "2" ? cmpp::EntryData::Word : cmpp::EntryData::ByteAndWord;
            std::set<std::uint32_t> fx;
            if (c[5] != "-")
                for (const std::string_view x : text::split(c[5], ',')) fx.insert(*text::parse_int<std::uint32_t>(x));
            const Bytes original = newcomp_test_unhex(c[3]), compiled = newcomp_test_unhex(c[4]);
            const nc::Finding f = c[0] == "eq" ? nc::equivalent(original, compiled, fx, options) : nc::equivalent3(original, compiled, fx, options);
            got = "{\"ok\":" + newcomp_test_finding(f) + "}";
        } else if (c[0] == "mask") {
            got = "{\"ok\":" + newcomp_test_json(nc::mask(c[1] == "-" ? "" : c[1])) + "}";
        } else if (c[0] == "rel") {
            const auto r = nc::relative(newcomp_test_unhex(c[1]));
            got = "{\"ok\":" + (r ? std::format("[{},{}]", r->opcode_length, r->displacement) : std::string("null")) + "}";
        } else {
            std::map<std::string, std::string> renaming;
            if (c[2] != "-")
                for (const std::string_view kv : text::split(c[2], ',')) {
                    const auto p = text::split(kv, '=');
                    renaming[std::string(p[0])] = std::string(p[1]);
                }
            const auto r = nc::rename(c[1], renaming);
            got = "{\"ok\":" + (r ? newcomp_test_json(*r) : std::string("null")) + "}";
        }
        ++n;
        if (want.starts_with("{\"err\"")) {   // (a procedure cut short: the original read past its end - IndexError - this answers)
            ++errors;
            continue;
        }
        if (got != want) {
            ++bad;
            if (bad <= 8) CB_CHECK_EQ(got.substr(0, 500), want.substr(0, 500));
        }
    }
    CB_CHECK_EQ(n, 688u);
    CB_CHECK_EQ(bad, 0u);
    CB_CHECK_EQ(errors, 2u);
});

CB_TEST("exact.habits.newcomp.the_readers", [] {
    CB_CHECK_EQ(nc::mask("mov ax, word ptr [bp - 0x4]"), "mov ax, word ptr [bp + N]");
    CB_CHECK_EQ(nc::mask("add ax, 12"), "add ax, N");
    CB_CHECK(!nc::relative(Bytes{0x90}).has_value());
    CB_CHECK_EQ(nc::relative(Bytes{0xEB, 0xFE})->displacement, -2);
    CB_CHECK_EQ(nc::relative(Bytes{0xE8, 0x10, 0x00})->displacement, 16);
    // SI and DI have no halves: a byte register renamed to one cannot be written
    CB_CHECK(!nc::rename("mov al, 1", {{"ax", "si"}}).has_value());
    CB_CHECK_EQ(*nc::rename("mov al, 1", {{"ax", "dx"}}), "mov dl, 1");
    CB_CHECK_EQ(*nc::rename("push ax", {}), "push ax");
});
