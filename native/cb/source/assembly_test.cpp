#include "cb/source/assembly.hpp"

#include <fstream>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace {
// (std::string::contains is C++23.)
bool has(std::string_view haystack, std::string_view needle) { return haystack.find(needle) != std::string_view::npos; }
}  // namespace


using namespace cb;
using namespace cb::source::assembly;
namespace stdfs = std::filesystem;

namespace {

Bytes bytes_of(std::initializer_list<int> values) {
    Bytes out;
    for (const int v : values) out.push_back(static_cast<std::uint8_t>(v));
    return out;
}

// An OMF record: type, length (payload and checksum), payload, checksum (the reader here does not check it).
void rec(Bytes& out, std::uint8_t type, const Bytes& payload) {
    const std::size_t len = payload.size() + 1;
    out.push_back(type);
    out.push_back(static_cast<std::uint8_t>(len & 0xFF));
    out.push_back(static_cast<std::uint8_t>(len >> 8));
    out.insert(out.end(), payload.begin(), payload.end());
    out.push_back(0);
}

// LEDATA: segment index, offset, data.
void ledata(Bytes& out, int segment, std::uint16_t offset, const Bytes& data) {
    Bytes payload = {static_cast<std::uint8_t>(segment), static_cast<std::uint8_t>(offset & 0xFF), static_cast<std::uint8_t>(offset >> 8)};
    payload.insert(payload.end(), data.begin(), data.end());
    rec(out, 0xA0, payload);
}

}  // namespace

CB_TEST("source.assembly.hex_numbers_in_the_assemblers_spelling", [] {
    CB_CHECK_EQ(operands("mov ax, 0x1f"), "mov ax, 01Fh");
    CB_CHECK_EQ(operands("0xdeadbeef,0x10"), "0DEADBEEFh,010h");
    CB_CHECK_EQ(operands("(0x10)"), "(010h)");
    CB_CHECK_EQ(operands("[0x0]"), "[00h]");
    // what is not a number standing alone stays as it is
    CB_CHECK_EQ(operands("a0x1f"), "a0x1f");           // inside a word
    CB_CHECK_EQ(operands("0x1F"), "0x1F");             // the decoders write lower case; an upper-case digit ends no number
    CB_CHECK_EQ(operands("0x1fg"), "0x1fg");           // a letter after it
    CB_CHECK_EQ(operands("0x"), "0x");
    CB_CHECK_EQ(operands(""), "");
    CB_CHECK_EQ(operands("call 0x10 ; 0x20"), "call 010h ; 020h");
});

CB_TEST("source.assembly.mnemonic_spellings", [] {
    CB_CHECK_EQ(mnemonic("pushal"), "pushad");
    CB_CHECK_EQ(mnemonic("popal"), "popad");
    CB_CHECK_EQ(mnemonic("int3"), "int 3");
    CB_CHECK_EQ(mnemonic("retn"), "ret");
    CB_CHECK_EQ(mnemonic("fwait"), "wait");
    CB_CHECK_EQ(mnemonic("pushaw"), "pusha");
    CB_CHECK_EQ(mnemonic("popaw"), "popa");
    CB_CHECK_EQ(mnemonic("retfw"), "retf");
    CB_CHECK_EQ(mnemonic("iretw"), "iret");
    CB_CHECK_EQ(mnemonic("mov"), "mov");
    CB_CHECK_EQ(std::size(kConditions), 16u);
    CB_CHECK_EQ(std::string(kConditions[7]), "a");
});

CB_TEST("source.assembly.string_instructions", [] {
    const auto op = string_op("rep movsb");
    CB_CHECK(op.has_value());
    if (op) {
        CB_CHECK_EQ(op->prefix, "rep ");
        CB_CHECK_EQ(op->op, "movs");
        CB_CHECK_EQ(op->width, 'b');
    }
    const auto repne = string_op("repne scasb ax");
    CB_CHECK(repne.has_value() && repne->prefix == "repne " && repne->op == "scas");
    const auto repe = string_op("repe cmpsw");
    CB_CHECK(repe.has_value() && repe->prefix == "repe " && repe->width == 'w');
    const auto bare = string_op("stosd");
    CB_CHECK(bare.has_value() && bare->prefix.empty() && bare->op == "stos" && bare->width == 'd');
    CB_CHECK(string_op("insb").has_value());
    CB_CHECK(string_op("outsd").has_value());
    CB_CHECK(!string_op("movsx eax, bl").has_value());   // x is no width
    CB_CHECK(!string_op("movsbx").has_value());          // the width ends the word
    CB_CHECK(!string_op("mov ax, bx").has_value());
    CB_CHECK(!string_op("rep").has_value());
    CB_CHECK(!string_op("").has_value());
});

CB_TEST("source.assembly.data_lines_write_text_as_text", [] {
    // runs of five or more printable characters are text; shorter runs stay numbers; the quote is never text
    Bytes data = bytes_of({0, 1, 2});
    for (const char c : std::string("Hello")) data.push_back(static_cast<std::uint8_t>(c));
    data.push_back(0xFF);
    for (const char c : std::string("abcd")) data.push_back(static_cast<std::uint8_t>(c));
    data.push_back(7);
    for (const char c : std::string("it's a long enough run of printable text ... 0123456789 0123456789 0123456789 0123456789")) data.push_back(static_cast<std::uint8_t>(c));
    const std::vector<std::string> expected = {
        "        db   000h, 001h, 002h",
        "        db   'Hello'",
        "        db   0FFh, 061h, 062h, 063h",
        "        db   064h, 007h, 069h, 074h",
        "        db   027h",
        "        db   's a long enough run of printable text ... 0123456789 0123456'",
        "        db   '789 0123456789 0123456789'",
    };
    CB_CHECK((data_lines(data, 4)) == (expected));
    CB_CHECK((data_lines(as_bytes("abc"))) == ((std::vector<std::string>{"        db   061h, 062h, 063h"})));
    CB_CHECK((data_lines(as_bytes("hello world"))) == ((std::vector<std::string>{"        db   'hello world'"})));
    CB_CHECK(data_lines({}).empty());
    // sixteen values to a line by default
    const Bytes zeros(20, 0);
    const auto lines = data_lines(zeros);
    CB_CHECK_EQ(lines.size(), 2u);
    CB_CHECK_EQ(lines[1], "        db   000h, 000h, 000h, 000h");
});

CB_TEST("source.assembly.the_last_segments_data_records_are_joined", [] {
    Bytes obj;
    rec(obj, 0x80, bytes_of({3, 'a', '.', 'c'}));           // THEADR: not a segment
    rec(obj, 0x98, bytes_of({0x60, 0x10, 0x00, 1, 2, 3}));   // segment 1
    rec(obj, 0x98, bytes_of({0x60, 0x20, 0x00, 1, 2, 3}));   // segment 2
    ledata(obj, 1, 0, bytes_of({0xEE}));                     // segment 1's data is not the last segment's
    ledata(obj, 2, 0, bytes_of({0x90, 0x91}));
    ledata(obj, 2, 2, bytes_of({0x92}));                     // continues the record before: one run
    ledata(obj, 2, 32, bytes_of({0xB8, 1, 0}));
    ledata(obj, 2, 100, bytes_of({0x01}));                   // a gap before it: a run of its own
    ledata(obj, 2, 100, bytes_of({0x02}));                   // the same place again: the later record wins
    const auto got = last_segment_data(obj);
    CB_CHECK_EQ(got.size(), 3u);
    CB_CHECK((got.at(0)) == (bytes_of({0x90, 0x91, 0x92})));
    CB_CHECK((got.at(32)) == (bytes_of({0xB8, 1, 0})));
    CB_CHECK((got.at(100)) == (bytes_of({0x02})));
    CB_CHECK(last_segment_data({}).empty());
    // a 32-bit data record (A1) has a four-byte offset
    Bytes wide;
    rec(wide, 0x99, bytes_of({0x60, 0x20, 0x00, 1, 2, 3}));
    rec(wide, 0xA1, bytes_of({1, 0x10, 0, 0, 0, 0xCC}));
    CB_CHECK((last_segment_data(wide).at(0x10)) == (bytes_of({0xCC})));
    // a record that runs past the end of the file ends the walk; one too short to say where it goes is damage
    Bytes cut = obj;
    cut.resize(cut.size() - 2);
    CB_CHECK((last_segment_data(cut).at(0)) == (bytes_of({0x90, 0x91, 0x92})));
    Bytes damaged;
    rec(damaged, 0x98, bytes_of({1}));
    rec(damaged, 0xA0, bytes_of({1}));
    CB_CHECK_THROWS_AS(FormatError, (void)last_segment_data(damaged));
});

namespace {

using Pieces = std::vector<Piece<std::string>>;

Piece<std::string> piece(std::size_t off, std::size_t n, std::optional<std::string> key = std::nullopt, bool jump = false,
                         std::optional<Bytes> mask = std::nullopt) {
    return {off, n, std::move(mask), std::move(key), jump};
}

Bytes slice(const Bytes& v, std::size_t from, std::size_t to) { return Bytes(v.begin() + static_cast<std::ptrdiff_t>(from), v.begin() + static_cast<std::ptrdiff_t>(to)); }
Bytes cat(Bytes a, const Bytes& b) {
    a.insert(a.end(), b.begin(), b.end());
    return a;
}

}  // namespace

// The expectations below are what the reference (codebreaker/source/assembly.py culprits) answers for the same inputs.
CB_TEST("source.assembly.culprits_blames_the_instruction_that_changed", [] {
    Bytes orig;
    for (int i = 0; i < 40; ++i) orig.push_back(static_cast<std::uint8_t>(i));
    const Pieces pieces = {piece(0, 2, "a"), piece(2, 3, "b"), piece(5, 1, "c"), piece(6, 4, "d"), piece(10, 2, "e", true), piece(12, 3, "f")};
    using Keys = std::set<std::string>;

    CB_CHECK((culprits<std::string>(orig, slice(orig, 0, 15), pieces)) == (Keys{}));   // everything came back
    CB_CHECK((culprits<std::string>(orig, cat(slice(orig, 0, 12), Bytes(3, 0)), pieces)) == (Keys{"f"}));

    Bytes b_differs = slice(orig, 0, 15);
    b_differs[2] = 0xEE;
    CB_CHECK((culprits<std::string>(orig, b_differs, pieces)) == (Keys{"b"}));

    // b came back two bytes longer: it is found itself, not the places behind it where the bytes no longer line up
    const Bytes b_longer = cat(cat(slice(orig, 0, 2), Bytes(5, 0xAA)), slice(orig, 5, 15));
    CB_CHECK((culprits<std::string>(orig, b_longer, pieces)) == (Keys{"b"}));

    // a jump that moved is blamed only when nothing else is
    const Bytes jump = cat(cat(slice(orig, 0, 10), Bytes(2, 0x55)), slice(orig, 12, 15));
    CB_CHECK((culprits<std::string>(orig, jump, pieces)) == (Keys{"e"}));
    const Bytes jump_and_f = cat(cat(slice(orig, 0, 10), Bytes(2, 0x55)), Bytes(3, 9));
    CB_CHECK((culprits<std::string>(orig, jump_and_f, pieces)) == (Keys{"e"}));   // f differs too, but e (a jump) was blamed first and the walk resumed
    // lost: nothing lines up again, what was found is all there is
    CB_CHECK((culprits<std::string>(orig, Bytes(3, 0xEE), pieces)) == (Keys{"a"}));
    CB_CHECK((culprits<std::string>(orig, slice(orig, 0, 2), pieces)) == (Keys{"b"}));   // the built file is too short
    CB_CHECK((culprits<std::string>(orig, {}, Pieces{})) == (Keys{}));
});

CB_TEST("source.assembly.culprits_ignores_patched_fields_and_unkeyed_pieces", [] {
    Bytes orig;
    for (int i = 0; i < 40; ++i) orig.push_back(static_cast<std::uint8_t>(i));
    using Keys = std::set<std::string>;
    const Pieces masked = {piece(0, 4, "m", false, bytes_of({0, 1, 1, 0})), piece(4, 2, "n")};
    CB_CHECK((culprits<std::string>(orig, bytes_of({0, 9, 9, 3, 4, 5}), masked)) == (Keys{}));      // bytes 1 and 2 are patched by the loader
    CB_CHECK((culprits<std::string>(orig, bytes_of({7, 9, 9, 3, 4, 5}), masked)) == (Keys{"m"}));   // byte 0 is not
    // a piece with no key (it was written as bytes) is never blamed
    const Pieces unkeyed = {piece(0, 2), piece(2, 3)};
    CB_CHECK((culprits<std::string>(orig, slice(orig, 0, 5), unkeyed)) == (Keys{}));
    const Pieces half = {piece(0, 2, "x"), piece(2, 3)};
    CB_CHECK((culprits<std::string>(orig, Bytes(5, 9), half)) == (Keys{"x"}));
    // a piece that lies past the end of the original is short there, so it cannot match
    const Bytes short_original = slice(orig, 0, 5);
    const Pieces past = {piece(0, 2, "a"), piece(4, 3, "z")};
    CB_CHECK((culprits<std::string>(short_original, cat(short_original, Bytes(5, 0)), past)) == (Keys{"z"}));
    // keys can be any ordered type: addresses, say
    const std::vector<Piece<std::uint32_t>> by_address = {{0, 2, std::nullopt, 0x1000u, false}, {2, 2, std::nullopt, 0x1002u, false}};
    CB_CHECK((culprits<std::uint32_t>(orig, bytes_of({0, 1, 9, 9}), by_address)) == (std::set<std::uint32_t>{0x1002u}));
});

namespace {

// OpenWatcom's tools stood in for by batch files: the "assembler" copies a canned object file to where it was asked to
// write one, after refusing the lines that hold BADINSN, as the real one refuses what it cannot encode; the "linker"
// copies its script to the file its `name` line names.
struct FakeTools {
    fs::ScopedTempDir dir{"cbassembly"};
    Tools tools;

    FakeTools() {
        fs::write_text(dir.path() / "wasm.bat",
                       "@echo off\r\n"
                       "set o=%~3\r\n"   // (cmd takes the = of -fo=<object> for a separator: the object is argument 3, the source 4)
                       "findstr /C:\"BADINSN\" \"%~4\" >nul\r\n"
                       "if %errorlevel%==0 goto bad\r\n"
                       "copy /Y \"%~dp0canned.obj\" \"%o%\" >nul\r\n"
                       "exit /b 0\r\n"
                       ":bad\r\n"
                       "for /f \"tokens=1 delims=:\" %%a in ('findstr /N /C:\"BADINSN\" \"%~4\"') do echo %~nx4(%%a): Error! E032: cannot encode\r\n"
                       "exit /b 1\r\n");
        fs::write_text(dir.path() / "wlink.bat",
                       "@echo off\r\n"
                       "set s=%~1\r\n"
                       "set s=%s:~1%\r\n"
                       "for /f \"tokens=1,*\" %%a in ('findstr /B /C:\"name \" \"%s%\"') do copy /Y \"%s%\" \"%%b\" >nul\r\n"
                       "echo linked\r\n"
                       "exit /b 0\r\n");
        tools.wasm = dir.path() / "wasm.bat";
        tools.wlink = dir.path() / "wlink.bat";
        tools.timeout = std::chrono::seconds(60);
    }
    // The object the "assembler" writes: two segments (the probe file's PD and PC), the last one holding what each
    // candidate at 32*n assembled to.
    void canned(const std::vector<Bytes>& at_slot) {
        Bytes obj;
        rec(obj, 0x98, bytes_of({0x60, 0x00, 0x00, 1, 2, 3}));
        rec(obj, 0x98, bytes_of({0x60, 0x20, 0x00, 1, 2, 3}));
        for (std::size_t n = 0; n < at_slot.size(); ++n) ledata(obj, 2, static_cast<std::uint16_t>(32 * n), at_slot[n]);
        fs::write_file(dir.path() / "canned.obj", obj);
    }
};

}  // namespace

CB_TEST("source.assembly.the_assembler_is_run_and_its_object_found", [] {
    FakeTools fake;
    fake.canned({bytes_of({0x90})});
    fs::write_text(fake.dir.path() / "a.asm", "nop\n");
    const Made made = assemble(fake.tools, fake.dir.path() / "a.asm", fake.dir.path());
    CB_CHECK(made.file.has_value());
    if (made.file) {
        CB_CHECK(made.file->filename() == "a.obj");
        CB_CHECK(stdfs::exists(*made.file));
    }
    // a refusal is a failure with the tool's words, not an exception
    fs::write_text(fake.dir.path() / "bad.asm", "BADINSN\n");
    const Made bad = assemble(fake.tools, fake.dir.path() / "bad.asm", fake.dir.path());
    CB_CHECK(!bad.file.has_value());
    CB_CHECK(has(bad.log, "bad.asm(1): Error! E032"));
    // an object left from an earlier run is not mistaken for this one's
    fs::write_text(fake.dir.path() / "bad.obj", "stale");
    CB_CHECK(!assemble(fake.tools, fake.dir.path() / "bad.asm", fake.dir.path()).file.has_value());
    // no assembler configured, and one that cannot be started
    const Made none = assemble(Tools{}, fake.dir.path() / "a.asm", fake.dir.path());
    CB_CHECK(!none.file.has_value());
    CB_CHECK(has(none.log, "no assembler"));
    Tools missing = fake.tools;
    missing.wasm = fake.dir.path() / "not-there.exe";
    CB_CHECK(!assemble(missing, fake.dir.path() / "a.asm", fake.dir.path()).file.has_value());
});

CB_TEST("source.assembly.the_linker_gets_its_directives_in_a_script", [] {
    FakeTools fake;
    const std::vector<std::string> lines = {"system nt", "file a.obj"};
    const Made made = link(fake.tools, lines, fake.dir.path() / "out.dll", fake.dir.path());
    CB_CHECK(made.file.has_value());
    CB_CHECK_EQ(made.log, "linked");
    const std::string script = fs::read_text(fake.dir.path() / "out.lnk");
    CB_CHECK_EQ(script, "system nt\r\nfile a.obj\r\nname " + fs::to_text(stdfs::absolute(fake.dir.path() / "out.dll")) + "\r\n");
    CB_CHECK(!link(Tools{}, lines, fake.dir.path() / "x.dll", fake.dir.path()).file.has_value());
});

CB_TEST("source.assembly.probe_says_which_texts_assemble_back", [] {
    FakeTools fake;
    const stdfs::path known_file = fake.dir.path() / "state" / "assembly_probes.json";
    // candidates sort by text: BADINSN, "mov ax, 1", nop -- slots 0, 1, 2
    fake.canned({bytes_of({0x90}), bytes_of({0x66, 0xB8, 1, 0}), bytes_of({0x90})});
    const std::vector<Candidate> candidates = {
        {"nop", bytes_of({0x90}), bytes_of({0})},
        {"mov ax, 1", bytes_of({0x66, 0xB8, 1, 0}), bytes_of({0, 0, 0, 0})},
        {"BADINSN", bytes_of({0x90}), bytes_of({0})},   // the assembler refuses it: it is retried as a nop and not trusted
        {"mov ax, 1", bytes_of({0x66, 0xB8, 1, 0}), bytes_of({0, 0, 0, 0})},   // the same one twice is one
    };
    Prober prober(fake.tools, known_file);
    const std::set<Candidate> good = prober.probe(candidates, {"label_a", "label_b"}, {"label_b"}, fake.dir.path(), Mode::Flat32);
    CB_CHECK_EQ(good.size(), 2u);
    CB_CHECK(good.contains(candidates[0]));
    CB_CHECK(good.contains(candidates[1]));
    CB_CHECK(!good.contains(candidates[2]));

    // the probe file the assembler was given
    const std::string probe_file = fs::read_text(fake.dir.path() / "probe.asm");
    CB_CHECK(probe_file.starts_with(".386p\r\n.model flat\r\nPD segment para public use32 'PXCODE'\r\nlabel_a label byte\r\nlabel_b label near\r\nPD ends\r\n"
                                    "PC segment para public use32 'PCODE'\r\n        org  0\r\n        nop\r\n        org  32\r\n        mov ax, 1\r\n        org  64\r\n        nop\r\n"));
    CB_CHECK(probe_file.ends_with("PC ends\r\nend\r\n"));

    // what it established is remembered, in a file that anything can read
    const auto known = prober.known();
    CB_CHECK_EQ(known.size(), 3u);
    CB_CHECK(known.at("flat32|nop|90|00"));
    CB_CHECK(known.at("flat32|mov ax, 1|66b80100|00000000"));
    CB_CHECK(!known.at("flat32|BADINSN|90|00"));
    CB_CHECK(has(fs::read_text(known_file), "\"flat32|nop|90|00\": true"));

    // asked again, nothing is assembled: the answers come from the file, even with no assembler at all
    Prober offline(Tools{}, known_file);
    CB_CHECK(offline.probe(candidates, {}, {}, fake.dir.path(), Mode::Flat32) == good);
});

CB_TEST("source.assembly.probe_compares_only_the_bytes_outside_patched_fields", [] {
    FakeTools fake;
    // mov eax, label: B8 + four patched bytes. The assembler wrote zeros there; the original has the loader's value.
    fake.canned({bytes_of({0xB8, 0, 0, 0, 0}), bytes_of({0xB8, 0, 0, 0, 0})});
    const Candidate right{"mov eax, label", bytes_of({0xB8}), bytes_of({0, 1, 1, 1, 1})};
    const Candidate wrong_opcode{"mov ebx, label", bytes_of({0xBB}), bytes_of({0, 1, 1, 1, 1})};
    Prober prober(fake.tools, fake.dir.path() / "known.json");
    const std::set<Candidate> good = prober.probe(std::vector<Candidate>{right, wrong_opcode}, {"label"}, {}, fake.dir.path(), Mode::Real16);
    CB_CHECK(good == std::set<Candidate>{right});
    CB_CHECK(has(fs::read_text(fake.dir.path() / "probe.asm"), "assume cs:PC, ds:nothing, es:nothing, ss:nothing"));
    CB_CHECK(has(fs::read_text(fake.dir.path() / "probe.asm"), "use16"));
    CB_CHECK(prober.known().at("real16|mov eax, label|b8|0001010101"));
    CB_CHECK(!prober.known().at("real16|mov ebx, label|bb|0001010101"));
});

CB_TEST("source.assembly.probe_without_an_assembler_trusts_nothing", [] {
    fs::ScopedTempDir dir{"cbassembly"};
    Prober prober(Tools{}, dir.path() / "known.json");
    const Candidate c{"nop", bytes_of({0x90}), bytes_of({0})};
    CB_CHECK(prober.probe(std::vector<Candidate>{c}, {}, {}, dir.path()).empty());
    CB_CHECK(!prober.known().at("flat32|nop|90|00"));   // remembered as not assembling, so it is not tried again
    // a damaged file of known answers is the same as none
    fs::write_text(dir.path() / "damaged.json", "{ not json");
    CB_CHECK(Prober(Tools{}, dir.path() / "damaged.json").known().empty());
    fs::write_text(dir.path() / "list.json", "[1]");
    CB_CHECK(Prober(Tools{}, dir.path() / "list.json").known().empty());
    CB_CHECK(Prober(Tools{}, dir.path() / "absent.json").known().empty());
});
