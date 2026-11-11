// Tests of cb/symbols/xdk_libraries on code written out as PowerPC words (0x4E800020 is blr, 0x48000001 bl, 0x38600000
// li r3,0 and so on), so every case reads plainly. The signatures and the matcher were also run on the real XDK
// libraries (libcMT, XAPOBase, multidisc, oldnames) against the Xbox 360 programs of the corpus, and gave the Python
// module's signatures and matches: see the report of the refactor.
//
// DIVERGES from the Python module, by design: a function whose mask is shorter than its code uses the words the mask
// covers (the Python's struct.unpack raised); the signatures are not kept by this module (the caller keeps them, under
// the stamp), so there is no knowledge-base access in it; the output of `-o` is the JSON the Python wrote.
#include "cb/symbols/xdk_libraries.hpp"

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace xl = cb::symbols::xdk_libraries;

namespace {

constexpr std::uint32_t kBlr = 0x4E800020;
constexpr std::uint32_t kBl = 0x48000001;
constexpr std::uint32_t kLi = 0x38600000;

cb::Bytes be(const std::vector<std::uint32_t>& words) {
    cb::Bytes out;
    for (const std::uint32_t w : words) {
        out.push_back(static_cast<std::uint8_t>(w >> 24));
        out.push_back(static_cast<std::uint8_t>(w >> 16));
        out.push_back(static_cast<std::uint8_t>(w >> 8));
        out.push_back(static_cast<std::uint8_t>(w));
    }
    return out;
}

xl::LibraryFunction fn(const char* name, const std::vector<std::uint32_t>& words, std::vector<std::uint32_t> ignore = {},
                       std::uint32_t at = 0) {
    if (ignore.empty()) ignore.assign(words.size(), 0);
    return {name, be(words), std::move(ignore), at};
}

// Five words, the third of which a linker fills in.
const std::vector<std::uint32_t> kFoo = {0x7D8802A6, 0x9181FFF8, 0x48000000, 0x38600001, kBlr};
const std::vector<std::uint32_t> kFooMask = {0, 0, 0x03FFFFFC, 0, 0};

std::vector<std::uint32_t> foo_linked(std::uint32_t distance) {
    std::vector<std::uint32_t> w = kFoo;
    w[2] = 0x48000000 | (distance & 0x03FFFFFC);
    return w;
}

xl::ProgramText program(const std::vector<std::vector<std::uint32_t>>& pieces, std::set<std::uint32_t> starts = {}) {
    xl::ProgramText p;
    p.address = 0x82000000;
    for (const auto& piece : pieces) p.words.insert(p.words.end(), piece.begin(), piece.end());
    p.starts = std::move(starts);
    return p;
}

}  // namespace

CB_TEST("symbols.xdk_libraries.signatures_from_libraries", [] {
    xl::Library b{"b.lib", {fn("foo_alias", kFoo, kFooMask), fn("short3", {1, 2, 3}), fn("entered_by_falling", {5, 6, 7, 8, 9}, {}, 8)}};
    xl::Library a{"a.lib", {fn("foo", kFoo, kFooMask)}};
    const std::vector<xl::Library> libraries = {b, a};   // given out of order: a.lib comes first in the result
    const std::vector<xl::Signature> sigs = xl::make_signatures(libraries);
    CB_CHECK_EQ(sigs.size(), std::size_t{2});
    // the same code and mask in two libraries is one signature with both names and both libraries
    CB_CHECK(sigs[0].words == kFoo);
    CB_CHECK(sigs[0].ignore == kFooMask);
    CB_CHECK(sigs[0].names == (std::set<std::string>{"foo", "foo_alias"}));
    CB_CHECK(sigs[0].libs == (std::set<std::string>{"a.lib", "b.lib"}));
    CB_CHECK(!sigs[0].secondary);
    // under four words is no signature; a symbol not at the start of its section is a secondary entry
    CB_CHECK(sigs[1].names == (std::set<std::string>{"entered_by_falling"}));
    CB_CHECK(sigs[1].secondary);
});

CB_TEST("symbols.xdk_libraries.padding_is_not_part_of_the_signature", [] {
    std::vector<std::uint32_t> padded = kFoo;
    padded.push_back(0);
    padded.push_back(0);
    xl::Library lib{"a.lib", {fn("foo", padded, {0, 0, 0x03FFFFFC, 0, 0, 0, 0})}};
    const std::vector<xl::Library> libraries = {lib};
    const auto sigs = xl::make_signatures(libraries);
    CB_CHECK_EQ(sigs.size(), std::size_t{1});
    CB_CHECK(sigs[0].words == kFoo);                 // the two zero words that pad it are gone
    CB_CHECK(sigs[0].ignore == kFooMask);
    // a zero word inside is code
    xl::Library inner{"i.lib", {fn("inner", {1, 0, 2, 3, 4})}};
    const std::vector<xl::Library> inners = {inner};
    CB_CHECK_EQ(xl::make_signatures(inners)[0].words.size(), std::size_t{5});
    // a mask shorter than the code covers fewer words (and under four is too short)
    xl::Library short_mask{"m.lib", {fn("m", {1, 2, 3, 4, 5}, {0, 0, 0})}};
    const std::vector<xl::Library> shorts = {short_mask};
    CB_CHECK(xl::make_signatures(shorts).empty());
    xl::Library four{"m.lib", {fn("m", {1, 2, 3, 4, 5}, {0, 0, 0, 0})}};
    const std::vector<xl::Library> fours = {four};
    CB_CHECK_EQ(xl::make_signatures(fours)[0].words.size(), std::size_t{4});
});

CB_TEST("symbols.xdk_libraries.a_function_is_found_where_it_stands_once", [] {
    xl::Library lib{"a.lib", {fn("foo", kFoo, kFooMask)}};
    const std::vector<xl::Library> libraries = {lib};
    const auto sigs = xl::make_signatures(libraries);
    // the linker put another distance in the bl: still foo. It follows a blr, so it can start a function.
    const auto found = xl::match(program({{kLi, kBlr}, foo_linked(0x1230), {kLi, kBlr}}), sigs);
    CB_CHECK_EQ(found.size(), std::size_t{1});
    CB_CHECK(found.begin()->first == 0x82000000 + 2 * 4);
    CB_CHECK(found.begin()->second.names == (std::vector<std::string>{"foo"}));
    CB_CHECK(found.begin()->second.libs == (std::vector<std::string>{"a.lib"}));
    CB_CHECK_EQ(found.begin()->second.size, 20u);
    // a word outside the mask differing: not foo
    std::vector<std::uint32_t> other = foo_linked(0x40);
    other[3] = 0x38600002;
    CB_CHECK(xl::match(program({{kLi, kBlr}, other}), sigs).empty());
});

CB_TEST("symbols.xdk_libraries.two_places_name_nothing", [] {
    xl::Library lib{"a.lib", {fn("foo", kFoo, kFooMask)}};
    const std::vector<xl::Library> libraries = {lib};
    const auto sigs = xl::make_signatures(libraries);
    CB_CHECK(xl::match(program({foo_linked(0x10), {kLi, kBlr}, foo_linked(0x20)}), sigs).empty());   // not unique: not for certain
});

CB_TEST("symbols.xdk_libraries.a_function_cannot_start_in_the_middle_of_another", [] {
    xl::Library lib{"a.lib", {fn("tail", {0x38600001, 0x38600002, 0x38600003, 0x38600004, kBlr}),
                              fn("secondary_entry", {0x39600001, 0x39600002, 0x39600003, 0x39600004, kBlr}, {}, 4)}};
    const std::vector<xl::Library> libraries = {lib};
    const auto sigs = xl::make_signatures(libraries);
    const std::vector<std::uint32_t> tail = {0x38600001, 0x38600002, 0x38600003, 0x38600004, kBlr};
    const std::vector<std::uint32_t> entry = {0x39600001, 0x39600002, 0x39600003, 0x39600004, kBlr};
    // after a word that falls through, and no .pdata entry there: the tail of a bigger function
    CB_CHECK(xl::match(program({{kLi}, tail}), sigs).empty());
    CB_CHECK(xl::match(program({{kLi}, entry}), sigs).size() == 1);    // a secondary entry is entered by falling into it
    // ... unless something knows a function starts there
    CB_CHECK(xl::match(program({{kLi}, tail}, {1}), sigs).size() == 1);
    // after a return or a branch, or at the very start, it can
    CB_CHECK(xl::match(program({{kBlr}, tail}), sigs).size() == 1);
    CB_CHECK(xl::match(program({{kBl}, tail}), sigs).size() == 1);
    CB_CHECK(xl::match(program({{0x7C800008}, tail}), sigs).size() == 1);    // a trap
    CB_CHECK(xl::match(program({tail}), sigs).size() == 1);
    CB_CHECK(xl::match(program({{0}, tail}), sigs).size() == 1);              // padding
    CB_CHECK(xl::match(program({{0x4D820020}, tail}), sigs).empty());         // a conditional return falls through
});

CB_TEST("symbols.xdk_libraries.too_generic_to_place", [] {
    // three words whole are enough; two (with the rest filled in by the linker) are not
    xl::Library lib{"a.lib", {fn("three", {1, 2, 3, 0x48000000}, {0, 0, 0, 0x03FFFFFC}), fn("two", {7, 8, 0x48000000, 0x48000000}, {0, 0, 0x03FFFFFC, 0x03FFFFFC})}};
    const std::vector<xl::Library> libraries = {lib};
    const auto sigs = xl::make_signatures(libraries);
    const auto found = xl::match(program({{kBlr}, {1, 2, 3, 0x48000100}, {kBlr}, {7, 8, 0x48000200, 0x48000300}}), sigs);
    CB_CHECK_EQ(found.size(), std::size_t{1});
    CB_CHECK(found.begin()->second.names == (std::vector<std::string>{"three"}));
});

CB_TEST("symbols.xdk_libraries.names_and_libraries_of_one_place_are_sorted_together", [] {
    // two signatures at one place (a function and a longer one that begins with it): both names, the longer size
    xl::Library lib{"z.lib", {fn("zeta", {1, 2, 3, 4, 5, 6}), fn("alpha_inner", {1, 2, 3, 4}), fn("beta_inner", {1, 2, 3, 4})}};
    xl::Library other{"m.lib", {fn("alpha_inner", {1, 2, 3, 4})}};
    const std::vector<xl::Library> libraries = {lib, other};
    const auto found = xl::match(program({{kBlr}, {1, 2, 3, 4, 5, 6}}), xl::make_signatures(libraries));
    CB_CHECK_EQ(found.size(), std::size_t{1});
    const auto& f = found.begin()->second;
    CB_CHECK(f.names == (std::vector<std::string>{"alpha_inner", "beta_inner", "zeta"}));
    CB_CHECK(f.libs == (std::vector<std::string>{"m.lib", "z.lib"}));
    CB_CHECK_EQ(f.size, 24u);
});

CB_TEST("symbols.xdk_libraries.falls_through", [] {
    CB_CHECK(xl::falls_through(kLi));
    CB_CHECK(xl::falls_through(0x40820010));   // bne
    CB_CHECK(!xl::falls_through(0));
    CB_CHECK(!xl::falls_through(0x48000010));  // b
    CB_CHECK(!xl::falls_through(kBl));
    CB_CHECK(!xl::falls_through(0x48000003));  // bla
    CB_CHECK(!xl::falls_through(kBlr));
    CB_CHECK(!xl::falls_through(0x4E800420));  // bctr
    CB_CHECK(xl::falls_through(0x4D820020));   // beqlr
    CB_CHECK(!xl::falls_through(0x0C000000));  // twi
    CB_CHECK(!xl::falls_through(0x7C800008));  // tw
    CB_CHECK(xl::falls_through(0x7C632378));   // or r3,r3,r4
});

CB_TEST("symbols.xdk_libraries.library_files_and_stamp", [] {
    cb::fs::ScopedTempDir tmp("cb-xdk");
    cb::fs::write_text(tmp.path() / "b.LIB", "12345");
    cb::fs::write_text(tmp.path() / "a.lib", "1");
    cb::fs::write_text(tmp.path() / "notes.txt", "x");
    cb::fs::write_text(tmp.path() / "c.obj", "x");
    const auto files = xl::library_files(tmp.path());
    CB_CHECK_EQ(files.size(), std::size_t{2});
    CB_CHECK_EQ(files[0].name, "a.lib");
    CB_CHECK_EQ(files[1].name, "b.LIB");
    CB_CHECK_EQ(files[1].size, std::uint64_t{5});
    CB_CHECK(xl::library_files(tmp.path() / "missing").empty());

    const std::string stamp = xl::signatures_stamp(files);
    CB_CHECK(stamp.starts_with(std::to_string(xl::kReaderRevision) + "-"));
    CB_CHECK_EQ(stamp.size(), std::to_string(xl::kReaderRevision).size() + 1 + 16);
    CB_CHECK_EQ(xl::signatures_stamp(files), stamp);   // the same files, the same stamp
    auto changed = files;
    changed[0].size = 2;
    CB_CHECK(xl::signatures_stamp(changed) != stamp);
    changed = files;
    changed[1].modified += 1;
    CB_CHECK(xl::signatures_stamp(changed) != stamp);
    CB_CHECK(xl::signatures_stamp({}) != stamp);
});

CB_TEST("symbols.xdk_libraries.matches_as_json", [] {
    std::map<std::uint32_t, xl::Found> found;
    found[0x82001000] = {{"memcpy", "memmove"}, {"libcMT.lib"}, 64};
    found[0x820000A0] = {{"_purecall"}, {"libcMT.lib", "libcpMT.lib"}, 8};
    CB_CHECK_EQ(xl::format_matches(found),
                "{\n \"820000A0\": {\n  \"names\": [\n   \"_purecall\"\n  ],\n  \"libs\": [\n   \"libcMT.lib\",\n   \"libcpMT.lib\"\n  ],\n  \"size\": 8\n },\n"
                " \"82001000\": {\n  \"names\": [\n   \"memcpy\",\n   \"memmove\"\n  ],\n  \"libs\": [\n   \"libcMT.lib\"\n  ],\n  \"size\": 64\n }\n}");
    CB_CHECK_EQ(xl::format_matches({}), "{}");
});

CB_TEST("symbols.xdk_libraries.command", [] {
    cb::fs::ScopedTempDir tmp("cb-xdk");
    cb::fs::make_dirs(tmp.path() / "libs");
    xl::Services services;
    services.default_folder = tmp.path() / "libs";
    int program_loads = 0;
    std::filesystem::path loaded_from;
    services.load_program = [&](const std::filesystem::path& p) {
        ++program_loads;
        loaded_from = p;
        // two functions of lib a, one of lib b
        return program({{kBlr}, {1, 2, 3, 4}, {kBlr}, {5, 6, 7, 8}, {kBlr}, {9, 10, 11, 12}});
    };
    std::filesystem::path signatures_from;
    services.load_signatures = [&](const std::filesystem::path& folder) {
        signatures_from = folder;
        const std::vector<xl::Library> libraries = {{"a.lib", {fn("one", {1, 2, 3, 4}), fn("two", {5, 6, 7, 8})}}, {"b.lib", {fn("three", {9, 10, 11, 12})}}};
        return xl::make_signatures(libraries);
    };

    {
        cb::testing::CapturedStd cap;
        const std::vector<std::string> args = {"game.xex", "-o", (tmp.path() / "names.json").string()};
        CB_CHECK_EQ(xl::run(args, services), 0);
        CB_CHECK_EQ(cap.out(), "game.xex: 3 library functions named (48 bytes)\n       2  a.lib\n       1  b.lib\n  -> " + (tmp.path() / "names.json").string() + "\n");
    }
    CB_CHECK(signatures_from == tmp.path() / "libs");   // the default folder
    CB_CHECK_EQ(program_loads, 1);
    CB_CHECK(cb::fs::read_text(tmp.path() / "names.json").starts_with("{\n \"82000004\": {"));
    {
        cb::testing::CapturedStd cap;   // a folder given, and -o in front
        const std::vector<std::string> args = {"-o", (tmp.path() / "n2.json").string(), "x.bin", (tmp.path() / "libs").string()};
        CB_CHECK_EQ(xl::run(args, services), cb::kExitUsage);   // options first: not a program
    }
    {
        cb::testing::CapturedStd cap;
        const std::vector<std::string> args = {"x.bin", (tmp.path() / "libs").string()};
        CB_CHECK_EQ(xl::run(args, services), 0);
        CB_CHECK(loaded_from == std::filesystem::path("x.bin"));
        CB_CHECK(cap.out().starts_with("x.bin: 3 library functions named (48 bytes)\n"));
    }
    {
        cb::testing::CapturedStd cap;
        const std::vector<std::string> args = {"x.bin", (tmp.path() / "no-such-folder").string()};
        CB_CHECK_EQ(xl::run(args, services), cb::kExitFailed);
        CB_CHECK(cap.out().starts_with("no XDK library folder: "));
    }
    {
        cb::testing::CapturedStd cap;
        CB_CHECK_EQ(xl::run({}, services), cb::kExitUsage);
        CB_CHECK(cap.out().starts_with("codebreaker xdk-match <program.xex or image>"));
    }
    const std::vector<std::string> dangling = {"x.bin", "-o"};
    CB_CHECK_THROWS_AS(cb::UsageError, (void)xl::run(dangling, services));
});
