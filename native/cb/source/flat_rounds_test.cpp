// Tests of the rounds that write an x86 file whole against NASM. The helpers are tested without an assembler; the rounds themselves
// run NASM (settings: nasm; the main checkout's build of it, or CB_NASM) and are skipped where there is none.

#include "cb/source/flat_rounds.hpp"

#include <cstdlib>

#include "cb/base/config.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace cb::source::flat_rounds {
namespace {

std::optional<std::filesystem::path> find_nasm() {
    if (const auto env = fs::get_env("CB_NASM"); env && !env->empty()) return fs::from_text(*env);
    try {
        const config::Config main = config::Config::load(config::current().main_checkout());
        if (const auto configured = main.path("nasm"); configured && std::filesystem::is_regular_file(*configured)) return configured;
        const std::filesystem::path built = main.checkout_root() / "native" / "nasm" / "build" / "bin" / "nasm.exe";
        if (std::filesystem::is_regular_file(built)) return built;
    } catch (const Error&) {   // (no configuration to read: no assembler to test against)
    }
    return std::nullopt;
}

CB_TEST("source.flat_rounds.error_lines_are_the_lines_nasm_blames", [] {
    const std::string said = "x.asm:12: error: parser: instruction expected\nx.asm:40: warning: not this\nx.asm:7: error: symbol `foo' undefined\n";
    CB_CHECK((error_lines(said) == std::vector<std::size_t>{12, 7}));
    CB_CHECK(error_lines("").empty());
    CB_CHECK(error_lines("a: error: no number\n:: error\n:5:error\n").empty());   // a number and ": error" are both needed
    CB_CHECK((error_lines("C:\\work\\x.asm:3: error: bad") == std::vector<std::size_t>{3}));   // a drive letter's colon is not a line
});

CB_TEST("source.flat_rounds.tail_is_the_end_of_a_message", [] {
    CB_CHECK_EQ(tail("abcdef", 3), "def");
    CB_CHECK_EQ(tail("abc", 10), "abc");
    CB_CHECK_EQ(tail("", 4), "");
});

CB_TEST("source.flat_rounds.read_map_reads_the_labels_nasm_wrote", [] {
    fs::ScopedTempDir dir("cb_flat");
    const std::filesystem::path map = dir.path() / "x.map";
    fs::write_file(map, as_bytes("\n- NASM Map file ----\n\nSource file:  x.asm\nSymbols\n-------\n\n"
                                 "Address   Offset   Name\n"
                                 "  0000001A  00000010  start\n"
                                 "  0000002B  0000000E  next\n"
                                 "  0000002C  000000zz  junk\n"
                                 "  00000030  00000020\n"));
    const LabelMap labels = read_map(map);
    CB_CHECK_EQ(labels.size(), 2u);
    CB_CHECK(labels.at("start").address == 0x10 && labels.at("start").place == 0x1A);   // columns: place in the file, then address
    CB_CHECK(labels.at("next").address == 0x0E && labels.at("next").place == 0x2B);
});

CB_TEST("source.flat_rounds.report", [] {
    FlatReport r;
    r.instructions = 200;
    r.raw = 5;
    r.rounds = 2;
    r.same = true;
    r.built = true;
    r.source = "out/prog.asm";
    const ReportText ok = report("dir/prog.dll", r);
    CB_CHECK_EQ(ok.text, "prog.dll: 200 instructions, 5 (2%) written as bytes; 2 rounds; builds back THE SAME\n  source: out/prog.asm\n");
    CB_CHECK_EQ(ok.exit_code, 0);
    r.same = false;
    r.rounds = 1;
    const ReportText differs = report("prog.dll", r);
    CB_CHECK(differs.text.find("1 round; built, differs") != std::string::npos);
    CB_CHECK_EQ(differs.exit_code, 1);
    r.built = false;
    CB_CHECK(report("prog.dll", r).text.find("not built") != std::string::npos);
});

// A toy file: the original bytes of each instruction, and the text NASM is given for it. `add eax, 0x10` is stored as `05 10 00 00 00`
// (the long form), which NASM does not choose; `frobnicate eax` is not an instruction NASM knows.
struct ToyInsn {
    std::string text;
    Bytes original;
};
const std::vector<ToyInsn> kToy = {{"mov eax, 1", {0xB8, 1, 0, 0, 0}}, {"add eax, 0x10", {0x05, 0x10, 0, 0, 0}}, {"ret", {0xC3}}, {"frobnicate eax", {0x90, 0x90}}};

class ToyFile final : public FlatImage<int> {
public:
    Source<int> write(const std::set<int>& raw, bool) override {
        Source<int> out;
        std::size_t line = 0;
        const auto add = [&](const std::string& text) {
            out.text += text + "\n";
            return ++line;
        };
        add("bits 32");
        add("org 0");
        for (int k = 0; k < static_cast<int>(kToy.size()); ++k) {
            add("i" + std::to_string(k) + ":");
            if (raw.contains(k)) {
                std::string bytes = "db ";
                for (std::size_t b = 0; b < kToy[k].original.size(); ++b) bytes += (b ? ", " : "") + std::to_string(kToy[k].original[b]);
                add(bytes);
            } else {
                out.instruction_at_line[add(kToy[k].text)] = k;
            }
        }
        return out;
    }
    // An instruction came back different when the bytes at its label are not its own, or the next label is not where they end.
    std::set<int> differing(ByteView built, const LabelMap& labels, const std::set<int>& raw) override {
        std::set<int> bad;
        for (int k = 0; k < static_cast<int>(kToy.size()); ++k) {
            if (raw.contains(k)) continue;
            const std::uint64_t at = labels.at("i" + std::to_string(k)).address;
            const std::uint64_t next = k + 1 < static_cast<int>(kToy.size()) ? labels.at("i" + std::to_string(k + 1)).address : built.size();
            const Bytes& want = kToy[k].original;
            if (next - at != want.size() || !std::equal(want.begin(), want.end(), built.begin() + at)) bad.insert(k);
        }
        return bad;
    }
};

Bytes toy_original() {
    Bytes all;
    for (const ToyInsn& i : kToy) all.insert(all.end(), i.original.begin(), i.original.end());
    return all;
}

CB_TEST("source.flat_rounds.rounds_write_as_bytes_what_nasm_rejects_or_gives_back_different", [] {
    const auto nasm = find_nasm();
    if (!nasm) return;   // (no assembler here)
    fs::ScopedTempDir dir("cb_flat");
    ToyFile file;
    std::vector<std::string> said;
    FlatOptions options;
    options.nasm = *nasm;
    options.say = [&](std::string_view line) { said.emplace_back(line); };
    const FlatResult<int> r = flat<int>(toy_original(), file, dir.path(), "toy", options);
    CB_CHECK(r.same);
    CB_CHECK((r.raw == std::set<int>{1, 3}));   // the add NASM shortens, the instruction NASM does not know
    CB_CHECK_EQ(r.rounds, 3u);                  // reject, give back different, nothing differs
    CB_CHECK(r.image && std::filesystem::is_regular_file(*r.image));
    CB_CHECK(r.source == dir.path() / "toy.asm");
    CB_CHECK_EQ(said.size(), 3u);
    CB_CHECK(said[0].find("rejects") != std::string::npos);
    CB_CHECK(said[1].find("different") != std::string::npos);
});

// A file with a line the writer cannot blame on any instruction.
class BrokenFile final : public FlatImage<int> {
public:
    Source<int> write(const std::set<int>&, bool) override {
        Source<int> out;
        out.text = "bits 32\nthis is not nasm\n";
        return out;
    }
    std::set<int> differing(ByteView, const LabelMap&, const std::set<int>&) override { return {}; }
};

CB_TEST("source.flat_rounds.an_error_no_instruction_owns_stops_the_rounds", [] {
    const auto nasm = find_nasm();
    if (!nasm) return;
    fs::ScopedTempDir dir("cb_flat");
    BrokenFile file;
    FlatOptions options;
    options.nasm = *nasm;
    CB_CHECK_THROWS_AS(ToolError, flat<int>(Bytes{0x90}, file, dir.path(), "broken", options));
});

CB_TEST("source.flat_rounds.a_missing_assembler_is_a_tool_error", [] {
    fs::ScopedTempDir dir("cb_flat");
    CB_CHECK_THROWS_AS(ToolError, assemble(dir.path() / "no-such-nasm.exe", "bits 32\nret\n", dir.path(), "x"));
});

CB_TEST("source.flat_rounds.assemble_gives_the_image_and_the_map", [] {
    const auto nasm = find_nasm();
    if (!nasm) return;
    fs::ScopedTempDir dir("cb_flat");
    const Assembled a = assemble(*nasm, "bits 32\norg 0\nstart: nop\nnext: ret\n", dir.path(), "tiny");
    CB_CHECK(a.file.has_value());
    CB_CHECK((read_all(*a.file) == Bytes{0x90, 0xC3}));
    const LabelMap labels = read_map(a.map);
    CB_CHECK(labels.at("start").address == 0 && labels.at("next").address == 1);
    const Assembled bad = assemble(*nasm, "bits 32\nmov eax, ebx, ecx\n", dir.path(), "bad");
    CB_CHECK(!bad.file.has_value());
    CB_CHECK(!error_lines(bad.said).empty());
});

}  // namespace
}  // namespace cb::source::flat_rounds
