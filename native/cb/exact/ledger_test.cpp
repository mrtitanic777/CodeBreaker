#include "cb/exact/ledger.hpp"

#include <functional>
#include <map>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/omf.hpp"

using namespace cb;
using namespace cb::exact;
namespace stdfs = std::filesystem;
namespace omf = cb::formats::omf;

// What the module is held to: the Python module this one replaced (its pad rule, its place rules, its merge of
// toolchains, its counts line) on objects the tests make themselves, each case read through for what the program
// would hold.
//
// DIVERGES: a compiland's row whose project names no "toolchain" has no `by` here (the Python put None there, which
// its own write() could not join); a source whose first-procedure file has no address (a place by names) is left as
// the names place it (the Python raised TypeError subtracting from None).

namespace {

constexpr std::uint8_t kLedgerTestMovAx1[4] = {0xB8, 0x01, 0x00, 0xC3};   // mov ax, 1; ret
constexpr std::uint8_t kLedgerTestXorAx[3] = {0x33, 0xC0, 0xC3};          // xor ax, ax; ret

Bytes ledger_test_bytes(std::initializer_list<std::uint8_t> v) { return Bytes(v); }

bool ledger_test_has(std::string_view hay, std::string_view needle) { return hay.find(needle) != std::string_view::npos; }

// An object of one code segment cut at the given publics.
Bytes ledger_test_object(const Bytes& code, std::initializer_list<std::pair<const char*, std::int64_t>> publics) {
    OrderedMap<std::string, std::int64_t> pubs;
    for (const auto& [name, offset] : publics) pubs.insert_or_assign(name, offset);
    return omf::raw_object("t", code, pubs);
}

// Compiles a file to what `make` says of its text.
struct LedgerTestToolchain final : modules::Toolchain {
    std::function<modules::CompileResult(std::string_view)> make;
    mutable int compiled = 0;
    std::optional<std::string> profile(std::string_view) const override { return std::nullopt; }
    modules::CompileResult compile(ByteView source, const modules::CompileOptions&) const override {
        ++compiled;
        return make(as_text(source));
    }
};

std::shared_ptr<LedgerTestToolchain> ledger_test_compiler(Bytes object) {
    auto tc = std::make_shared<LedgerTestToolchain>();
    tc->make = [object = std::move(object)](std::string_view) { return modules::CompileResult{object, "ok"}; };
    return tc;
}

std::shared_ptr<LedgerTestToolchain> ledger_test_failing(std::string console) {
    auto tc = std::make_shared<LedgerTestToolchain>();
    tc->make = [console = std::move(console)](std::string_view) { return modules::CompileResult{std::nullopt, console}; };
    return tc;
}

struct LedgerTestProject final : ledger::Project {
    fs::ScopedTempDir folder{"ledgertest"};
    Bytes img = Bytes(0x100, 0xEE);
    std::vector<std::uint32_t> code_starts;
    std::uint32_t end_of_code = 0;
    std::vector<ledger::LabelledToolchain> tcs;
    std::vector<ledger::SourceFile> files;
    SymbolTable syms;
    mutable std::optional<SymbolTable> saved;
    bool whole_files = false;
    std::map<std::string, std::string> config;

    stdfs::path dir() const override { return folder.path(); }
    stdfs::path src() const override { return folder.path(); }
    std::string exe() const override { return "x/PROG.EXE"; }
    stdfs::path path(std::string_view name) const override { return folder.path() / std::string(name); }
    std::optional<std::string> config_text(std::string_view key) const override {
        const auto it = config.find(std::string(key));
        return it == config.end() ? std::nullopt : std::optional<std::string>(it->second);
    }
    const pascal_join::Project* compilands() const override { return nullptr; }
    std::map<std::string, Bytes> headers() const override { return {}; }
    std::vector<std::string> include() const override { return {}; }
    std::optional<std::string> switches() const override { return std::nullopt; }
    std::vector<std::uint32_t> starts() const override { return code_starts; }
    bool whole(const stdfs::path&) const override { return whole_files; }
    ByteView image() const override { return img; }
    std::uint32_t code_end() const override { return end_of_code; }
    std::vector<ledger::LabelledToolchain> toolchains() const override { return tcs; }
    SymbolTable symbols() const override { return syms; }
    void save_symbols(const SymbolTable& s) const override { saved = s; }
    std::vector<ledger::SourceFile> sources() const override { return files; }
    std::vector<std::shared_ptr<const ledger::Project>> sets() const override {
        return {std::shared_ptr<const ledger::Project>(std::shared_ptr<void>(), this)};
    }
    compare::Program program() const override {
        compare::Program p;
        p.dgroup = 0x80;
        p.loadseg = 0;
        p.code_base = [](std::uint32_t) { return 0u; };
        return p;
    }

    void put(std::size_t at, ByteView b) { std::copy(b.begin(), b.end(), img.begin() + static_cast<std::ptrdiff_t>(at)); }
    stdfs::path source(const std::string& name, const std::string& text = "int f(void) { return 1; }\n") {
        fs::write_text(folder.path() / name, text);
        return folder.path() / name;
    }
    void add_toolchain(const std::string& label, std::shared_ptr<LedgerTestToolchain> tc) { tcs.push_back({label, std::move(tc)}); }
};

// two functions: _a (4 bytes) and _b (3), as one object
Bytes ledger_test_two_functions() {
    Bytes code(std::begin(kLedgerTestMovAx1), std::end(kLedgerTestMovAx1));
    code.insert(code.end(), std::begin(kLedgerTestXorAx), std::end(kLedgerTestXorAx));
    return ledger_test_object(code, {{"_a", 0}, {"_b", 4}});
}

void ledger_test_put_both(LedgerTestProject& p, std::size_t at) {
    p.put(at, kLedgerTestMovAx1);
    p.put(at + 4, kLedgerTestXorAx);
}

}  // namespace

CB_TEST("exact.ledger.a_file_whose_functions_are_the_programs_is_identical_and_learnt", [] {
    LedgerTestProject p;
    ledger_test_put_both(p, 0x10);
    p.syms.insert_or_assign("_b", SymPlace{SymKind::Code, 0x14});
    const stdfs::path file = p.source("f0010.c");
    const auto tc = ledger_test_compiler(ledger_test_two_functions());
    SymbolTable learnt;
    const std::vector<ledger::Row> rows = ledger::check_file(p, *tc, file, 0x10, p.syms, learnt);
    CB_CHECK_EQ(rows.size(), 2u);
    CB_CHECK_EQ(rows[0].file, "f0010.c");
    CB_CHECK_EQ(*rows[0].function, "_a");
    CB_CHECK_EQ(*rows[0].address, 0x10u);
    CB_CHECK_EQ(rows[0].length, 4u);
    CB_CHECK(rows[0].verdict == ledger::Verdict::Identical);
    CB_CHECK_EQ(*rows[1].address, 0x14u);
    CB_CHECK(rows[1].verdict == ledger::Verdict::Identical);
    CB_CHECK(rows[1].notes.empty());
    CB_CHECK_EQ(learnt.size(), 2u);
    CB_CHECK((*learnt.find("_a") == SymPlace{SymKind::Code, 0x10}));
    CB_CHECK((*learnt.find("_b") == SymPlace{SymKind::Code, 0x14}));
});

CB_TEST("exact.ledger.a_byte_that_differs_is_a_difference_and_nothing_is_learnt", [] {
    LedgerTestProject p;
    ledger_test_put_both(p, 0x10);
    p.img[0x11] = 0x02;   // the program loads 2
    p.syms.insert_or_assign("_b", SymPlace{SymKind::Code, 0x14});
    const auto tc = ledger_test_compiler(ledger_test_two_functions());
    SymbolTable learnt;
    const std::vector<ledger::Row> rows = ledger::check_file(p, *tc, p.source("f0010.c"), 0x10, p.syms, learnt);
    CB_CHECK_EQ(rows.size(), 2u);
    CB_CHECK(rows[0].verdict == ledger::Verdict::Differs);
    CB_CHECK(rows[1].verdict == ledger::Verdict::Identical);
    CB_CHECK(learnt.empty());   // (a file is learnt from only when all its placed functions are identical)
});

CB_TEST("exact.ledger.a_function_nothing_places_has_no_place_and_does_not_stop_the_learning", [] {
    LedgerTestProject p;
    ledger_test_put_both(p, 0x10);   // (_b is in no SYMBOLS)
    const auto tc = ledger_test_compiler(ledger_test_two_functions());
    SymbolTable learnt;
    const std::vector<ledger::Row> rows = ledger::check_file(p, *tc, p.source("f0010.c"), 0x10, p.syms, learnt);
    CB_CHECK_EQ(rows.size(), 2u);
    CB_CHECK(rows[1].verdict == ledger::Verdict::NoPlace);
    CB_CHECK(!rows[1].address.has_value());
    CB_CHECK_EQ(rows[1].length, 3u);
    CB_CHECK_EQ(learnt.size(), 1u);   // only _a: the placed functions are all identical
    CB_CHECK(learnt.contains("_a"));
});

CB_TEST("exact.ledger.a_whole_file_lies_in_one_piece_from_its_first_function", [] {
    LedgerTestProject p;
    ledger_test_put_both(p, 0x20);
    p.whole_files = true;
    const auto tc = ledger_test_compiler(ledger_test_two_functions());
    SymbolTable learnt;
    const std::vector<ledger::Row> rows = ledger::check_file(p, *tc, p.source("m.plm"), 0x20, p.syms, learnt);
    CB_CHECK_EQ(rows.size(), 2u);
    CB_CHECK_EQ(*rows[1].address, 0x24u);
    CB_CHECK(rows[0].verdict == ledger::Verdict::Identical && rows[1].verdict == ledger::Verdict::Identical);
});

CB_TEST("exact.ledger.a_module_with_code_before_its_named_procedure_is_placed_by_where_it_agrees", [] {
    // the file is named after the procedure at 0x20; the module's two bytes of code come before its frame
    const Bytes code = ledger_test_bytes({0x90, 0x90, 0x55, 0x8B, 0xEC, 0xB8, 0x01, 0x00, 0x5D, 0xC3});
    LedgerTestProject p;
    p.put(0x1E, code);
    p.whole_files = true;
    const auto tc = ledger_test_compiler(ledger_test_object(code, {{"_main", 2}}));
    SymbolTable learnt;
    const stdfs::path file = p.source("p0020.plm");
    const std::vector<ledger::Row> plain = ledger::check_file(p, *tc, file, 0x20, p.syms, learnt);
    CB_CHECK_EQ(plain.size(), 2u);
    CB_CHECK(plain[0].verdict == ledger::Verdict::Differs);   // (placed at 0x20 as the file's name says: the bytes are not there)
    p.config["named_by"] = "first procedure";
    const std::vector<ledger::Row> rows = ledger::check_file(p, *tc, file, 0x20, p.syms, learnt);
    CB_CHECK_EQ(rows.size(), 2u);
    CB_CHECK_EQ(*rows[0].function, "(module start)");
    CB_CHECK_EQ(*rows[0].address, 0x1Eu);
    CB_CHECK_EQ(*rows[1].address, 0x20u);
    CB_CHECK(rows[0].verdict == ledger::Verdict::Identical && rows[1].verdict == ledger::Verdict::Identical);
});

CB_TEST("exact.ledger.a_file_that_does_not_compile_is_one_row_with_the_last_line_of_the_console", [] {
    LedgerTestProject p;
    const auto tc = ledger_test_failing("\r\n  Microsoft C\r\nf0010.c(3): error C2065: 'x': undeclared\r\n\r\n");
    SymbolTable learnt;
    const std::vector<ledger::Row> rows = ledger::check_file(p, *tc, p.source("f0010.c"), 0x10, p.syms, learnt);
    CB_CHECK_EQ(rows.size(), 1u);
    CB_CHECK(rows[0].verdict == ledger::Verdict::DoesNotCompile);
    CB_CHECK(!rows[0].function.has_value());
    CB_CHECK_EQ(*rows[0].address, 0x10u);
    CB_CHECK_EQ(rows[0].length, 0u);
    CB_CHECK_EQ(rows[0].notes.size(), 1u);
    CB_CHECK_EQ(rows[0].notes[0], "f0010.c(3): error C2065: 'x': undeclared");
    // a console with nothing in it has no line to give
    const auto silent = ledger_test_failing(" \r\n");
    CB_CHECK(ledger::check_file(p, *silent, p.source("f0020.c"), std::nullopt, p.syms, learnt)[0].notes.empty());
});

CB_TEST("exact.ledger.a_source_that_is_not_there_is_an_io_error", [] {
    LedgerTestProject p;
    const auto tc = ledger_test_compiler(ledger_test_two_functions());
    SymbolTable learnt;
    CB_CHECK_THROWS_AS(IoError, ledger::check_file(p, *tc, p.folder.path() / "nothing.c", 0x10, p.syms, learnt));
});

CB_TEST("exact.ledger.an_object_that_is_no_omf_module_is_a_format_error", [] {
    LedgerTestProject p;
    const auto tc = ledger_test_compiler(Bytes{1, 2, 3});
    SymbolTable learnt;
    CB_CHECK_THROWS_AS(FormatError, ledger::check_file(p, *tc, p.source("f0010.c"), 0x10, p.syms, learnt));
});

CB_TEST("exact.ledger.a_file_placed_by_its_first_known_name_has_it_at_that_place", [] {
    // an assembly module: no place in its name (-1), SYMBOLS knows its first function
    LedgerTestProject p;
    ledger_test_put_both(p, 0x30);
    p.syms.insert_or_assign("_a", SymPlace{SymKind::Code, 0x30});
    p.syms.insert_or_assign("_b", SymPlace{SymKind::Unknown, 0x34});   // (a place whose kind is not said counts)
    const auto tc = ledger_test_compiler(ledger_test_two_functions());
    SymbolTable learnt;
    const std::vector<ledger::Row> rows = ledger::check_file(p, *tc, p.source("m.asm"), -1, p.syms, learnt);
    CB_CHECK_EQ(rows.size(), 2u);
    CB_CHECK_EQ(*rows[0].address, 0x30u);
    CB_CHECK(rows[0].verdict == ledger::Verdict::Identical && rows[1].verdict == ledger::Verdict::Identical);
    // with no name known there is nothing to place the file by: no rows
    LedgerTestProject q;
    SymbolTable none;
    CB_CHECK(ledger::check_file(q, *tc, q.source("m.asm"), -1, q.syms, none).empty());
});

CB_TEST("exact.ledger.stand_ins_after_the_samefile_line_are_compiled_and_not_compared", [] {
    LedgerTestProject p;
    ledger_test_put_both(p, 0x10);
    p.syms.insert_or_assign("_b", SymPlace{SymKind::Code, 0x14});
    const auto tc = ledger_test_compiler(ledger_test_two_functions());
    SymbolTable learnt;
    const std::string text = "int _a(void) { return 1; }\n/* SAMEFILE */\nint _b(void) { }\n";
    const std::vector<ledger::Row> rows = ledger::check_file(p, *tc, p.source("f0010.c", text), 0x10, p.syms, learnt);
    CB_CHECK_EQ(rows.size(), 1u);
    CB_CHECK_EQ(*rows[0].function, "_a");
});

CB_TEST("exact.ledger.the_assemblers_pad_to_an_even_length_is_not_a_difference", [] {
    // a function ending in a near jump and a pad byte of zero, which in the program is the next module's first byte
    const Bytes code = ledger_test_bytes({0x90, 0x90, 0xE9, 0x05, 0x00, 0x00});
    LedgerTestProject p;
    p.put(0x10, code);
    p.img[0x15] = 0x55;
    const auto tc = ledger_test_compiler(ledger_test_object(code, {{"_a", 0}}));
    SymbolTable learnt;
    const std::vector<ledger::Row> rows = ledger::check_file(p, *tc, p.source("f0010.c"), 0x10, p.syms, learnt);
    CB_CHECK_EQ(rows.size(), 1u);
    CB_CHECK(rows[0].verdict == ledger::Verdict::Identical);
    CB_CHECK_EQ(rows[0].notes.size(), 1u);
    CB_CHECK_EQ(rows[0].notes[0], "? the last byte is the pad to an even length; the next module begins there");
    // any other byte that differs is a difference
    p.img[0x12] = 0xEB;
    CB_CHECK(ledger::check_file(p, *tc, p.source("f0010.c"), 0x10, p.syms, learnt)[0].verdict == ledger::Verdict::Differs);
});

CB_TEST("exact.ledger.a_function_must_reach_the_next_start", [] {
    LedgerTestProject p;
    ledger_test_put_both(p, 0x10);
    p.syms.insert_or_assign("_b", SymPlace{SymKind::Code, 0x14});
    p.code_starts = {0x10, 0x18};   // _b ends at 0x17, and the next function begins at 0x18
    const auto tc = ledger_test_compiler(ledger_test_two_functions());
    SymbolTable learnt;
    const std::vector<ledger::Row> rows = ledger::check_file(p, *tc, p.source("f0010.c"), 0x10, p.syms, learnt);
    CB_CHECK_EQ(rows.size(), 2u);
    CB_CHECK(rows[0].verdict == ledger::Verdict::Differs);   // (_a ends at 0x14: the next start is 0x18)
    CB_CHECK_EQ(rows[0].notes.size(), 1u);
    CB_CHECK_EQ(rows[0].notes[0], "ends at 00014, before the next function's start");
    CB_CHECK(rows[1].verdict == ledger::Verdict::Identical);   // (an odd end rounds to the even start)
});

CB_TEST("exact.ledger.stubs_are_the_empty_functions_after_the_marker", [] {
    const std::string src =
        "int early(void) { }\n/* SAMEFILE */\nvoid _g(void) { }\nint far pascal h(int a, int b)\n{\n}\nint k(void);\nint m(void) { return 0; }\n"
        "static n( ) /* c */ { }\n";
    const std::set<std::string> got = ledger::stubs(src);
    CB_CHECK((got == std::set<std::string>{"G", "H", "N"}));
    CB_CHECK(ledger::stubs("int f(void) { }\n").empty());   // (no marker: none)
    CB_CHECK(ledger::stubs("/* SAMEFILE */").empty());
});

CB_TEST("exact.ledger.reaching_the_next_start", [] {
    Bytes image(0x40, 0xEE);
    const std::vector<std::uint32_t> starts{0x10, 0x20};
    CB_CHECK(ledger::reaches(starts, 0x30, image, 0x10, 0x10));    // ends at the next start
    CB_CHECK(ledger::reaches(starts, 0x30, image, 0x10, 0x0F));    // an odd end: the assembler's even start
    CB_CHECK(!ledger::reaches(starts, 0x30, image, 0x10, 0x08));   // short, and the bytes after are not a pad
    std::fill(image.begin() + 0x18, image.begin() + 0x20, std::uint8_t{0});
    CB_CHECK(ledger::reaches(starts, 0x30, image, 0x10, 0x08));    // a pad of zeros up to the next start
    std::fill(image.begin() + 0x20, image.begin() + 0x40, std::uint8_t{0});
    CB_CHECK(ledger::reaches(starts, 0x30, image, 0x20, 0x08));    // the last function: a pad of zeros up to the code's end
    CB_CHECK(ledger::reaches(starts, 0x30, image, 0x20, 0x10));    // reaches the end of the code
    CB_CHECK(!ledger::reaches(starts, 0x38, image, 0x20, 0x00));   // 24 zero bytes after it: more than a pad is
    CB_CHECK(ledger::reaches(starts, 0, image, 0x20, 0x02));       // no next start and no end of code: nothing to fall short of
    CB_CHECK(!ledger::reaches(starts, 0x30, ByteView(image).first(0x1C), 0x10, 0x04));   // (an image that ends before the next start: the bytes after are not all zero)
});

namespace {

ledger::Row ledger_test_row(std::string function, ledger::Verdict v, std::uint32_t address = 0x10) {
    ledger::Row r;
    r.file = "f0010.c";
    r.function = std::move(function);
    r.address = address;
    r.length = 4;
    r.verdict = v;
    return r;
}

}  // namespace

CB_TEST("exact.ledger.the_toolchains_rows_are_merged_by_which_gave_the_bytes", [] {
    using ledger::Verdict;
    const std::vector<ledger::Attempt> per{
        {"x", {ledger_test_row("_a", Verdict::Identical), ledger_test_row("_b", Verdict::Differs)}},
        {"y", {ledger_test_row("_a", Verdict::Identical), ledger_test_row("_b", Verdict::Identical)}},
        {"z", {ledger_test_row("_a", Verdict::Differs), ledger_test_row("_b", Verdict::Differs)}}};
    const std::vector<ledger::Row> rows = ledger::merge(per);
    CB_CHECK_EQ(rows.size(), 2u);
    CB_CHECK((rows[0].by == std::vector<std::string>{"x", "y"}));
    CB_CHECK(rows[0].verdict == Verdict::Identical);
    CB_CHECK((rows[1].by == std::vector<std::string>{"y"}));
    CB_CHECK(rows[1].verdict == Verdict::Identical);
    // none identical: the first that compiled says what is wrong; none gave the bytes
    std::vector<ledger::Row> broken = {ledger_test_row("_a", Verdict::DoesNotCompile)};
    const std::vector<ledger::Attempt> none{{"x", broken}, {"y", {ledger_test_row("_a", Verdict::Differs)}}};
    const std::vector<ledger::Row> bad = ledger::merge(none);
    CB_CHECK_EQ(bad.size(), 1u);
    CB_CHECK(bad[0].verdict == Verdict::Differs);
    CB_CHECK(bad[0].by.empty());
    // the toolchain that compiled has the longer list of rows: it is the base
    const std::vector<ledger::Attempt> longer{{"x", broken}, {"y", {ledger_test_row("_a", Verdict::Differs), ledger_test_row("_b", Verdict::NoPlace)}}};
    CB_CHECK_EQ(ledger::merge(longer).size(), 2u);
    CB_CHECK(ledger::merge({}).empty());
});

CB_TEST("exact.ledger.run_learns_what_symbols_does_not_have_and_keeps_what_it_has", [] {
    LedgerTestProject p;
    ledger_test_put_both(p, 0x10);
    p.syms.insert_or_assign("_b", SymPlace{SymKind::Code, 0x14});
    p.syms.insert_or_assign("_old", SymPlace{SymKind::Data, 0x77});
    p.add_toolchain("x", ledger_test_compiler(ledger_test_two_functions()));
    p.files.push_back({0x10, p.source("f0010.c")});
    const std::vector<ledger::Row> rows = ledger::run(p, false, false);
    CB_CHECK_EQ(rows.size(), 2u);
    CB_CHECK(!p.saved.has_value());   // (without --learn nothing is saved)
    ledger::run(p, true, false);
    CB_CHECK(p.saved.has_value());
    CB_CHECK_EQ(p.saved->size(), 3u);
    CB_CHECK((*p.saved->find("_a") == SymPlace{SymKind::Code, 0x10}));
    CB_CHECK((*p.saved->find("_b") == SymPlace{SymKind::Code, 0x14}));
    CB_CHECK((*p.saved->find("_old") == SymPlace{SymKind::Data, 0x77}));
    // a name SYMBOLS already has keeps its place
    p.saved.reset();
    p.syms.insert_or_assign("_a", SymPlace{SymKind::Code, 0x99});
    ledger::run(p, true, false);
    CB_CHECK(p.saved.has_value());
    CB_CHECK((*p.saved->find("_a") == SymPlace{SymKind::Code, 0x99}));
});

CB_TEST("exact.ledger.the_command_counts_and_names_the_toolchain_that_gave_each", [] {
    LedgerTestProject p;
    ledger_test_put_both(p, 0x10);
    p.syms.insert_or_assign("_b", SymPlace{SymKind::Code, 0x14});
    p.add_toolchain("x", ledger_test_failing("x.c(1): error"));
    p.add_toolchain("y", ledger_test_compiler(ledger_test_two_functions()));
    p.files.push_back({0x10, p.source("f0010.c")});
    testing::CapturedStd captured;
    CB_CHECK_EQ(ledger::command(p, {"--write"}), 0);
    const std::string said = captured.out();
    CB_CHECK(ledger_test_has(said, "2 functions, 2 identical, 0 differ, 0 no place, 0 files do not compile\n"));
    CB_CHECK(ledger_test_has(said, "   x          gives 0 (0 of them it alone)\n"));
    CB_CHECK(ledger_test_has(said, "   y          gives 2 (2 of them it alone)\n"));
    const std::string ledger_file = fs::read_text(p.folder.path() / "LEDGER");
    CB_CHECK_EQ(ledger_file,
                "# file, function, bytes, compared with PROG.EXE, the toolchains that give it (codebreaker exact ledger)\n"
                "f0010.c _a 4 identical y\n"
                "f0010.c _b 3 identical y\n");
});

CB_TEST("exact.ledger.the_command_counts_every_kind_of_verdict", [] {
    LedgerTestProject p;
    ledger_test_put_both(p, 0x10);
    p.img[0x10] = 0x00;   // _a differs; _b has no place
    p.files.push_back({0x10, p.source("f0010.c")});
    p.files.push_back({0x40, p.source("f0040.c", "broken")});
    // the second file does not compile
    const auto tc = std::make_shared<LedgerTestToolchain>();
    tc->make = [](std::string_view src) {
        if (src == "broken") return modules::CompileResult{std::nullopt, "error"};
        return modules::CompileResult{ledger_test_two_functions(), ""};
    };
    p.add_toolchain("x", tc);
    testing::CapturedStd captured;
    ledger::command(p, {});
    const std::string said = captured.out();
    CB_CHECK(ledger_test_has(said, "3 functions, 0 identical, 1 differ, 1 no place, 1 files do not compile\n"));
    CB_CHECK(!ledger_test_has(said, "gives"));   // (one toolchain: no table)
    // -v says a line each
    testing::CapturedStd verbose;
    ledger::command(p, {"-v"});
    CB_CHECK(ledger_test_has(verbose.out(), std::format("{:<10} {:<24} differs", "f0010.c", "_a")));
});
