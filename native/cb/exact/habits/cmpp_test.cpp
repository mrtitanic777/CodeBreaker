#include "cb/exact/habits/cmpp.hpp"

#include <algorithm>
#include <nlohmann/json.hpp>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

using namespace cb;
using namespace cb::exact::habits::cmpp;

// DIVERGES from the Python module (cmpp.py), on purpose:
//   * the entry helper's data (the byte and word, or the word, behind its call) that are cut short by the end of the code
//     are decoded as the ordinary bytes they then are; the Python raised struct.error out of walk and loose_bytes
//   * an instruction with no operands is written "ret", not "ret " (the Python kept the blank)
//   * loose_bytes does not read the entry data's first byte as an opcode: with that byte 0xE8 the Python left the two
//     bytes after it out of the comparison as a call's displacement, hiding a difference in the frame size
//   * the module-level HDRLEN and decoder are options of each call; main() is parse_request, compare and render
//   * a ledger entry that is not an object with an unsigned "size", a ledger that lists no such address, or an image too
//     short for the procedure is an error of its own type, not the TypeError/KeyError of the Python; the harness's
//     cases of those kinds (error text compared by type only) were dropped as incidental
//   * more than the first twelve same-size differences are not listed; as in the Python, the report then shows where the
//     listings stop lining up (kept)

namespace {

Bytes unhex(std::string_view hex) {
    Bytes out;
    for (std::size_t i = 0; i + 1 < hex.size(); i += 2) {
        out.push_back(static_cast<std::uint8_t>(*text::parse_int<unsigned>(hex.substr(i, 2), 16)));
    }
    return out;
}

Options options_for(int entry) {
    Options o;
    o.entry_data = entry == 2 ? EntryData::Word : EntryData::ByteAndWord;
    return o;
}

std::string right_trimmed(const std::string& s) { return std::string(text::rtrim(s)); }

const Json& golden() {
    static const Json doc = json::load(testing::data_path("exact/habits/testdata/cmpp.json"));
    return doc;
}

}  // namespace

CB_TEST("exact.habits.cmpp.walk_matches_the_pythons_listings", [] {
    // 408 listings of random procedures, damaged ones and short ones, as the Python walked them
    std::size_t checked = 0;
    for (const Json& c : golden().at("walk")) {
        const Bytes code = unhex(c.at("hex").get<std::string>());
        const std::vector<Row> rows = walk(code, c.at("base").get<std::uint32_t>(), c.at("skip").get<int>() != 0, options_for(c.at("entry").get<int>()));
        const Json& want = c.at("rows");
        CB_CHECK_EQ(rows.size(), want.size());
        for (std::size_t k = 0; k < std::min(rows.size(), want.size()); ++k) {
            const Json& w = want[k];
            CB_CHECK_EQ(rows[k].offset, w[0].get<std::uint32_t>());
            CB_CHECK_EQ(rows[k].size, w[1].get<std::uint32_t>());
            CB_CHECK_EQ(right_trimmed(rows[k].text), right_trimmed(w[2].get<std::string>()));
            CB_CHECK_EQ(rows[k].mnemonic, w[3].get<std::string>());
        }
        ++checked;
    }
    CB_CHECK(checked > 300);
});

CB_TEST("exact.habits.cmpp.loose_bytes_match_the_pythons", [] {
    std::size_t checked = 0;
    for (const Json& c : golden().at("loose")) {
        const Bytes code = unhex(c.at("hex").get<std::string>());
        std::set<std::uint32_t> fixups;
        for (const Json& f : c.at("fixups")) fixups.insert(f.get<std::uint32_t>());
        const std::set<std::uint32_t> got = loose_bytes(code, fixups, options_for(c.at("entry").get<int>()));
        std::set<std::uint32_t> want;
        for (const Json& w : c.at("loose")) want.insert(w.get<std::uint32_t>());
        // (the one place the Python was wrong: see the DIVERGES block)
        const bool entry_byte_is_call = code.size() > 6 && code[0] == 0xE8 && code[3] == 0xE8;
        if (!entry_byte_is_call) CB_CHECK(got == want);
        ++checked;
    }
    CB_CHECK(checked > 200);
});

CB_TEST("exact.habits.cmpp.walk_by_hand", [] {
    // call 0x0003 forward past the entry data; db + dw; a push; a ret
    const Bytes proc{0xE8, 0x10, 0x00, 0x07, 0x34, 0x12, 0x55, 0xC3};
    const std::vector<Row> rows = walk(proc, 0x100);
    CB_CHECK_EQ(rows.size(), std::size_t{4});
    CB_CHECK_EQ(rows[0].text, "call 0x113");   // an address, so the base counts
    CB_CHECK_EQ(rows[1].mnemonic, "hdr");
    CB_CHECK_EQ(rows[1].text, "db 0x07 ; dw 0x1234");
    CB_CHECK_EQ(rows[1].size, 3u);
    CB_CHECK_EQ(rows[2].text, "push bp");
    CB_CHECK_EQ(rows[3].text, "ret");
    // MASM-era Pascal: only the word
    const std::vector<Row> masm = walk(proc, 0, true, options_for(2));
    CB_CHECK_EQ(masm[1].text, "dw 0x3407");
    CB_CHECK_EQ(masm[1].size, 2u);
    // not a procedure that begins with the helper's call (or told not to skip): ordinary code
    CB_CHECK_EQ(walk(proc, 0, false)[1].mnemonic, "pop");   // (07 is a pop es)
    CB_CHECK_EQ(walk(Bytes{0x55, 0x8B, 0xEC, 0x90, 0x90, 0x90}, 0)[3].mnemonic, "nop");
    // data cut short by the end of the code: the bytes are decoded
    CB_CHECK_EQ(walk(Bytes{0xE8, 0x00, 0x00, 0x01, 0x02}, 0)[1].mnemonic, "add");
    // a byte that decodes to nothing
    const std::vector<Row> odd = walk(Bytes{0xE8}, 0);
    CB_CHECK_EQ(odd[0].mnemonic, "db");
    CB_CHECK_EQ(odd[0].text, "db 0xe8");
    CB_CHECK(walk(Bytes{}, 0).empty());
});

CB_TEST("exact.habits.cmpp.what_is_not_compared", [] {
    // a near call outside the procedure (loose), one inside it (compared), a far call (four bytes), a far jump
    const Bytes code{0xE8, 0x00, 0x00, 0x00, 0x00, 0x00,                // the entry call and data
                     0xE8, 0x40, 0x00,                                   // call outside (offset 6)
                     0xE8, 0xFA, 0xFF,                                   // call back into the procedure (offset 9: target 6)
                     0x9A, 0x01, 0x02, 0x03, 0x04,                       // call far (offset 12)
                     0xEA, 0x01, 0x02, 0x03, 0x04, 0xC3};                // jmp far (offset 17)
    const std::set<std::uint32_t> loose = loose_bytes(code, {20});
    CB_CHECK((loose == std::set<std::uint32_t>{7, 8, 13, 14, 15, 16, 18, 19, 20, 21}));
    // the entry data's first byte is not an opcode, whatever it is
    const Bytes tricky{0xE8, 0x00, 0x00, 0xE8, 0x34, 0x12, 0xC3};
    CB_CHECK(loose_bytes(tricky).empty());
});

namespace {

// A compiler that answers from a table, and an image.
class FakeCompiler final : public PascalCompiler {
public:
    Bytes image(const std::string&) override { return image_; }
    Compiled compile(const std::filesystem::path&) override {
        ++compiles;
        return compiled_;
    }
    Bytes image_;
    Compiled compiled_;
    int compiles = 0;
};

}  // namespace

CB_TEST("exact.habits.cmpp.the_pythons_reports", [] {
    // 73 runs of the Python main with a fake compiler: its stdout and exit status are the golden
    fs::ScopedTempDir dir;
    std::size_t identical = 0, differs = 0, no_code = 0, asm_only = 0;
    for (const Json& c : golden().at("main")) {
        std::vector<std::string> args;
        for (const Json& a : c.at("argv")) args.push_back(a.get<std::string>());
        FakeCompiler compiler;
        compiler.image_ = unhex(c.at("image").get<std::string>());
        if (!c.at("code").is_null()) compiler.compiled_.code = unhex(c.at("code").get<std::string>());
        for (const auto& [name, span] : c.at("procs").items()) {
            compiler.compiled_.procedures[name] = {span[0].get<std::uint32_t>(), span[1].get<std::uint32_t>()};
        }
        for (const Json& f : c.at("fix")) compiler.compiled_.fixups.push_back(f.get<std::uint32_t>());
        compiler.compiled_.listing = c.at("lst").get<std::string>();
        compiler.compiled_.console = c.at("console").get<std::string>();
        compiler.compiled_.cod = c.at("cod").get<std::string>();
        const std::string exe = args[0];
        fs::write_file(dir.path() / (text::upper(exe) + ".json"), to_bytes(json::dump(c.at("ledger"))));

        const Request request = parse_request(args);
        const Comparison result = compare(request, compiler, dir.path(), options_for(c.at("entry").get<int>()));
        const std::string text = render(result);
        // (the compiler's blank lines and the instruction texts' trailing blanks aside)
        std::string want = c.at("stdout").get<std::string>();
        // (The Python counted the two bytes after the entry data's first byte as not compared when that byte was 0xE8 -- see
        // the DIVERGES block -- so the count in the IDENTICAL line is set aside for such a procedure.)
        const auto without_count = [](const std::string& t) {
            const std::size_t open = t.find(" (");
            const std::size_t close = t.find(" not compared");
            return open != std::string::npos && close != std::string::npos && open < close ? t.substr(0, open + 2) + t.substr(close) : t;
        };
        bool entry_byte_is_call = false;
        if (!c.at("code").is_null()) {
            const Bytes code = unhex(c.at("code").get<std::string>());
            for (const auto& item : c.at("procs").items()) {
                const std::size_t at = item.value()[0].get<std::size_t>();
                entry_byte_is_call = entry_byte_is_call || (code.size() > at + 3 && code[at] == 0xE8 && code[at + 3] == 0xE8);
            }
        }
        const std::string got_text = right_trimmed(text::replace_all(text, " \n", "\n"));
        const std::string want_text = right_trimmed(text::replace_all(want, " \n", "\n"));
        CB_CHECK_EQ(entry_byte_is_call ? without_count(got_text) : got_text, entry_byte_is_call ? without_count(want_text) : want_text);
        CB_CHECK_EQ(exit_status(result), c.at("status").get<int>());
        switch (result.verdict) {
            case Comparison::Verdict::Identical: ++identical; break;
            case Comparison::Verdict::Differs: ++differs; break;
            case Comparison::Verdict::NoCode: ++no_code; break;
            case Comparison::Verdict::OriginalOnly: ++asm_only; break;
        }
        if (request.original_only) CB_CHECK_EQ(compiler.compiles, 0);   // --asm compiles nothing
    }
    // the set exercises every outcome
    CB_CHECK(identical > 2);
    CB_CHECK(differs > 10);
    CB_CHECK(no_code > 1);
    CB_CHECK(asm_only > 3);
});

CB_TEST("exact.habits.cmpp.the_request_and_its_errors", [] {
    const std::vector<std::string> ok{"M.EXE", "0x00001", "m100/p00001.pas", "--proc", "Foo", "--cod"};
    const Request r = parse_request(ok);
    CB_CHECK_EQ(r.exe, "M.EXE");
    CB_CHECK_EQ(r.address, 1u);
    CB_CHECK_EQ(r.source.generic_string(), "m100/p00001.pas");
    CB_CHECK_EQ(*r.procedure, "Foo");
    CB_CHECK(r.show_listing && !r.original_only);
    CB_CHECK(parse_request(std::vector<std::string>{"--asm", "M.EXE", "35e8", "x.pas"}).original_only);
    CB_CHECK_EQ(parse_request(std::vector<std::string>{"M.EXE", "35e8", "x.pas"}).address, 0x35e8u);
    CB_CHECK_THROWS_AS(UsageError, parse_request(std::vector<std::string>{}));
    CB_CHECK_THROWS_AS(UsageError, parse_request(std::vector<std::string>{"M.EXE", "zz", "x.pas"}));
    CB_CHECK_THROWS_AS(UsageError, parse_request(std::vector<std::string>{"M.EXE", "0x1", "x.pas", "--proc"}));
    CB_CHECK_THROWS_AS(UsageError, parse_request(std::vector<std::string>{"M.EXE", "0x1", "x.pas", "--what"}));
    // an address with a minus sign is not one
    CB_CHECK_THROWS_AS(UsageError, parse_request(std::vector<std::string>{"M.EXE", "-0x1", "x.pas"}));
});

CB_TEST("exact.habits.cmpp.damaged_ledgers_and_images", [] {
    fs::ScopedTempDir dir;
    FakeCompiler compiler;
    compiler.image_ = Bytes(30, 0x90);
    const Request request = parse_request(std::vector<std::string>{"M.EXE", "0x00001", "x.pas", "--asm"});
    const auto ledger = [&](std::string_view text) { fs::write_file(dir.path() / "M.EXE.json", to_bytes(text)); };
    CB_CHECK_THROWS_AS(NotFoundError, compare(request, compiler, dir.path()));   // no ledger
    ledger("{not json");
    CB_CHECK_THROWS_AS(ParseError, compare(request, compiler, dir.path()));
    ledger("[1,2]");
    CB_CHECK_THROWS_AS(NotFoundError, compare(request, compiler, dir.path()));
    ledger(R"({"0x00002": {"size": 3}})");   // another address
    CB_CHECK_THROWS_AS(NotFoundError, compare(request, compiler, dir.path()));
    ledger(R"({"0x00001": [1, 2]})");
    CB_CHECK_THROWS_AS(FormatError, compare(request, compiler, dir.path()));
    ledger(R"({"0x00001": {"size": "12"}})");
    CB_CHECK_THROWS_AS(FormatError, compare(request, compiler, dir.path()));
    ledger(R"({"0x00001": {"size": -3}})");
    CB_CHECK_THROWS_AS(FormatError, compare(request, compiler, dir.path()));
    ledger(R"({"0x00001": {"size": 400}})");   // past the end of the image
    CB_CHECK_THROWS_AS(FormatError, compare(request, compiler, dir.path()));
    ledger(R"({"0x00001": {"size": 3}})");
    CB_CHECK_EQ(compare(request, compiler, dir.path()).original_size, 3u);
});

CB_TEST("exact.habits.cmpp.numbers_do_not_separate_instructions_a_linker_fills_in", [] {
    // The compiled call and move differ from the original's in a word the linker fills in: the comparison sets the numbers
    // aside there, and the procedure is the same.
    fs::ScopedTempDir dir;
    FakeCompiler compiler;
    const Bytes original{0xE8, 0x10, 0x00, 0x01, 0x02, 0x03, 0xA1, 0x10, 0x00, 0xC3};   // mov ax, [0x10]
    compiler.image_ = original;
    Bytes compiled = original;
    compiled[7] = 0x99;   // the variable's place: a fix-up
    compiler.compiled_.code = compiled;
    compiler.compiled_.procedures["P00000"] = {0, 10};
    compiler.compiled_.fixups = {7, 8};
    fs::write_file(dir.path() / "M.EXE.json", to_bytes(R"({"0x00000": {"size": 10}})"));
    const Request request = parse_request(std::vector<std::string>{"M.EXE", "0x0", "x.pas"});
    const Comparison c = compare(request, compiler, dir.path());
    CB_CHECK(c.verdict == Comparison::Verdict::Identical);
    CB_CHECK_EQ(c.not_compared, std::size_t{4});   // the two fix-up bytes and the call's displacement
    // the same difference without a fix-up to explain it
    compiler.compiled_.fixups.clear();
    const Comparison d = compare(request, compiler, dir.path());
    CB_CHECK(d.verdict == Comparison::Verdict::Differs);
    CB_CHECK_EQ(d.differences.size(), std::size_t{1});
    CB_CHECK_EQ(d.differences[0].original, "mov ax, word ptr [0x10]");
    CB_CHECK_EQ(d.differences[0].compiled, "mov ax, word ptr [0x99]");
    CB_CHECK_EQ(exit_status(d), 1);
});
