#include "cb/exact/emu/isis.hpp"

#include <memory>
#include <string>
#include <vector>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include <nlohmann/json.hpp>

#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

using namespace cb;
using namespace cb::exact::emu;

namespace {

namespace stdfs = std::filesystem;

// ---- building ISIS object files ------------------------------------------------------------------------

Bytes record(std::uint8_t type, const Bytes& body) {
    const std::size_t n = body.size() + 1;   // the length counts the checksum byte
    Bytes out = {type, static_cast<std::uint8_t>(n & 255), static_cast<std::uint8_t>(n >> 8)};
    out.insert(out.end(), body.begin(), body.end());
    out.push_back(0x55);   // (the loader does not check it)
    return out;
}

struct Segment {
    std::uint16_t address;
    Bytes data;
};

Bytes object_file(const std::vector<Segment>& segments, std::uint16_t entry) {
    Bytes out = record(0x02, to_bytes(std::string("\x04MOD\x00", 5)));   // module header: a record type the loader skips
    for (const Segment& s : segments) {
        Bytes body = {1, static_cast<std::uint8_t>(s.address & 255), static_cast<std::uint8_t>(s.address >> 8)};
        body.insert(body.end(), s.data.begin(), s.data.end());
        append(out, record(0x06, body));
    }
    append(out, record(0x04, {0, 0, static_cast<std::uint8_t>(entry & 255), static_cast<std::uint8_t>(entry >> 8)}));
    append(out, record(0x0E, {}));
    return out;
}

// A small assembler for the programs of these tests: code at 0x100, data at 0x4000.
class Asm {
  public:
    std::uint16_t alloc(ByteView b) {
        const auto address = static_cast<std::uint16_t>(0x4000 + data_.size());
        data_.insert(data_.end(), b.begin(), b.end());
        return address;
    }
    std::uint16_t name(std::string_view text) { return alloc(to_bytes(std::string(text) + std::string(" \0", 2))); }
    std::uint16_t word(std::uint16_t value) { return alloc(Bytes{static_cast<std::uint8_t>(value & 255), static_cast<std::uint8_t>(value >> 8)}); }
    std::uint16_t text(std::string_view t) { return alloc(as_bytes(t)); }
    std::uint16_t var(std::size_t n = 2) { return alloc(Bytes(n)); }
    // MVI C,fn / LXI D,block / CALL 40h
    void call(int function, std::initializer_list<std::uint16_t> words) {
        Bytes block;
        for (const std::uint16_t w : words) {
            block.push_back(static_cast<std::uint8_t>(w & 255));
            block.push_back(static_cast<std::uint8_t>(w >> 8));
        }
        const std::uint16_t pb = alloc(block);
        raw({0x0E, function, 0x11, pb & 255, pb >> 8, 0xCD, 0x40, 0x00});
    }
    void raw(std::initializer_list<int> bytes) {
        for (const int b : bytes) code_.push_back(static_cast<std::uint8_t>(b));
    }
    Bytes build() const {
        std::vector<Segment> segments = {{0x100, code_}};
        if (!data_.empty()) segments.push_back({0x4000, data_});
        return object_file(segments, 0x100);
    }
    // the word a variable holds after a run
    static std::uint16_t word(const isis::Machine& m, std::uint16_t address) { return static_cast<std::uint16_t>(m.memory()[address] | (m.memory()[address + 1] << 8)); }

  private:
    Bytes code_;
    Bytes data_;
};

enum Function { Open, Close, Delete, Read, Write, Seek, Load, Rename, Attrib, Exit, Consol, Rescan, ErrorReport, Whocon, Spath };

// A work folder with drive 0, and drive 1 in its "sub".
struct Work {
    fs::ScopedTempDir dir{"isis"};
    isis::Drives drives;
    Work() {
        fs::make_dirs(dir.path() / "sub");
        drives = {{0, dir.path()}, {1, dir.path() / "sub"}};
    }
    stdfs::path program(const Bytes& object, std::string_view name = "PROG.OBJ") {
        const stdfs::path p = dir.path() / name;
        fs::write_file(p, object);
        return p;
    }
    isis::Result run(const Asm& a, std::string_view tail = "", std::uint64_t limit = 100000) {
        isis::Options options;
        options.limit = limit;
        return isis::run(program(a.build()), tail, drives, options);
    }
};

std::string file_text(const stdfs::path& p) { return text::from_latin1(fs::read_file(p)); }

}  // namespace

CB_TEST("exact.emu.isis.console_through_the_monitor", [] {
    // MVI C,'H'  CALL CO  MVI C,'i'  CALL CO  HLT
    Work w;
    const Bytes program = object_file({{0x100, {0x0E, 'H', 0xCD, 0x09, 0xF8, 0x0E, 'i', 0xCD, 0x09, 0xF8, 0x76}}}, 0x100);
    const isis::Result r = isis::run(w.program(program), "", w.drives);
    CB_CHECK_EQ(r.console, "Hi");
    CB_CHECK(r.finished);
    CB_CHECK_EQ(r.steps, std::uint64_t{6});   // (HLT, the instruction that ends the program, is not counted)
});

CB_TEST("exact.emu.isis.the_instruction_limit_stops_a_program_that_does_not_end", [] {
    Work w;
    const Bytes loop = object_file({{0x100, {0xC3, 0x00, 0x01}}}, 0x100);   // JMP 100h
    isis::Options options;
    options.limit = 1000;
    const isis::Result r = isis::run(w.program(loop), "", w.drives, options);
    CB_CHECK(!r.finished);
    CB_CHECK_EQ(r.steps, std::uint64_t{1000});
});

CB_TEST("exact.emu.isis.arithmetic_sets_the_flags_the_8080_does", [] {
    const auto run = [](std::initializer_list<int> code) {
        Work w;
        Bytes bytes;
        for (const int b : code) bytes.push_back(static_cast<std::uint8_t>(b));
        bytes.push_back(0x76);
        auto m = std::make_unique<isis::Machine>(w.drives, "");
        const stdfs::path p = w.program(object_file({{0x100, bytes}}, 0x100));
        m->start(p);
        m->run(1000);
        return m;
    };
    {   // 7F + 1: sign, aux carry (from the low nibble), odd parity
        const auto m = run({0x3E, 0x7F, 0xC6, 0x01});
        const isis::Cpu& c = m->cpu();
        CB_CHECK_EQ(c.a, 0x80);
        CB_CHECK(c.sign && !c.zero && !c.parity && !c.carry && c.aux_carry);
    }
    {   // FF + 1: zero, carry, aux carry, even parity
        const isis::Cpu c = run({0x3E, 0xFF, 0xC6, 0x01})->cpu();
        CB_CHECK_EQ(c.a, 0);
        CB_CHECK(c.zero && c.carry && c.aux_carry && c.parity && !c.sign);
    }
    {   // 5 - 6: a borrow sets carry; the auxiliary carry is "no borrow out of bit 3"
        const isis::Cpu c = run({0x3E, 0x05, 0xD6, 0x06})->cpu();
        CB_CHECK_EQ(c.a, 0xFF);
        CB_CHECK(c.carry && c.sign && c.parity && !c.zero && !c.aux_carry);
    }
    {   // CPI leaves the accumulator
        const isis::Cpu c = run({0x3E, 0x05, 0xFE, 0x05})->cpu();
        CB_CHECK_EQ(c.a, 5);
        CB_CHECK(c.zero && !c.carry);
    }
    {   // ANI: aux carry is bit 3 of the OR of the operands, carry is cleared
        const isis::Cpu c = run({0x37, 0x3E, 0xF0, 0xE6, 0x0F})->cpu();   // STC, MVI A,F0, ANI 0F
        CB_CHECK_EQ(c.a, 0);
        CB_CHECK(c.zero && !c.carry && c.aux_carry);
    }
    {   // 15 + 27 = 3C, DAA gives 42
        const isis::Cpu c = run({0x3E, 0x15, 0xC6, 0x27, 0x27})->cpu();
        CB_CHECK_EQ(c.a, 0x42);
        CB_CHECK(!c.carry);
    }
    {   // 99 + 1 = 9A, DAA gives 00 with carry
        const isis::Cpu c = run({0x3E, 0x99, 0xC6, 0x01, 0x27})->cpu();
        CB_CHECK_EQ(c.a, 0);
        CB_CHECK(c.carry && c.zero);
    }
    {   // INR does not touch carry; DAD sets it from bit 16
        const isis::Cpu c = run({0x37, 0x06, 0xFF, 0x04, 0x21, 0xFF, 0xFF, 0x01, 0x02, 0x00, 0x09})->cpu();   // STC MVI B,FF INR B LXI H,FFFF LXI B,2 DAD B
        CB_CHECK_EQ(c.b, 0);
        CB_CHECK_EQ((c.h << 8) | c.l, 1);
        CB_CHECK(c.carry);
    }
    {   // rotate through carry
        const isis::Cpu c = run({0x37, 0x3E, 0x81, 0x17})->cpu();   // STC MVI A,81 RAL
        CB_CHECK_EQ(c.a, 0x03);
        CB_CHECK(c.carry);
    }
});

CB_TEST("exact.emu.isis.stack_and_calls", [] {
    Work w;
    // LXI SP,3000  LXI B,1234  PUSH B  POP D  CALL sub  HLT   sub: MVI A,7 RET
    const Bytes code = {0x31, 0x00, 0x30, 0x01, 0x34, 0x12, 0xC5, 0xD1, 0xCD, 0x10, 0x01, 0x76, 0, 0, 0, 0, 0x3E, 0x07, 0xC9};
    isis::Machine m(w.drives, "");
    m.start(w.program(object_file({{0x100, code}}, 0x100)));
    m.run(100);
    CB_CHECK(m.finished());
    CB_CHECK_EQ(m.cpu().d, 0x12);
    CB_CHECK_EQ(m.cpu().e, 0x34);
    CB_CHECK_EQ(m.cpu().a, 7);
    CB_CHECK_EQ(m.cpu().sp, 0x3000);
    CB_CHECK_EQ(m.cpu().pc, 0x10C);   // after the HLT
});

CB_TEST("exact.emu.isis.write_to_the_console_by_an_isis_call", [] {
    Work w;
    Asm a;
    const std::uint16_t t = a.text("Hello ISIS\r\n");
    a.call(Write, {0, t, 12, a.var()});
    a.call(Exit, {0});
    const isis::Result r = w.run(a);
    CB_CHECK_EQ(r.console, "Hello ISIS\r\n");
    CB_CHECK(r.finished);
});

CB_TEST("exact.emu.isis.files_are_written_back_at_close_and_read_whole", [] {
    Work w;
    fs::write_file(w.dir.path() / "OLD.TXT", as_bytes("old contents that are long"));
    Asm a;
    const std::uint16_t aftn = a.var(), st = a.var(), actual = a.var();
    const std::uint16_t out = a.name(":F0:OUT.TXT"), old = a.name("old.txt"), missing = a.name("MISSING.TXT");
    const std::uint16_t data = a.text("abcdefghij"), buffer = a.var(200);
    a.call(Open, {aftn, out, 2, 0, st});          // new file, for writing
    a.call(Write, {2, data, 10, st});
    a.call(Close, {2, st});
    a.call(Open, {aftn, out, 1, 0, st});          // and back for reading
    a.call(Read, {2, buffer, 100, actual, st});
    a.call(Write, {0, buffer, 10, st});           // what was read, to the console
    a.call(Read, {2, buffer, 100, actual, st});   // at the end: nothing
    const std::uint16_t second_actual = actual;
    a.call(Close, {2, st});
    a.call(Open, {aftn, old, 3, 0, st});          // update: keeps what is there, a lower-case name is found
    a.call(Write, {2, data, 3, st});
    a.call(Close, {2, st});
    a.call(Open, {aftn, missing, 1, 0, st});      // not there
    a.call(Exit, {0});
    Bytes object = a.build();
    isis::Machine m(w.drives, "");
    m.start(w.program(object));
    m.run(100000);
    CB_CHECK(m.finished());
    CB_CHECK_EQ(text::from_latin1(m.console()), "abcdefghij");
    CB_CHECK_EQ(file_text(w.dir.path() / "OUT.TXT"), "abcdefghij");
    CB_CHECK_EQ(Asm::word(m, second_actual), 0);
    CB_CHECK_EQ(Asm::word(m, st), 13);   // the last call: the file is not there
    // an update opened a file that is there, and wrote over the start of it
    CB_CHECK_EQ(file_text(w.dir.path() / "OLD.TXT"), "abc contents that are long");
});

CB_TEST("exact.emu.isis.seek_and_sparse_writes", [] {
    Work w;
    Asm a;
    const std::uint16_t aftn = a.var(), st = a.var();
    const std::uint16_t block = a.word(1), byte = a.word(4);   // the position to seek to: block 1, byte 4 = 132
    const std::uint16_t out = a.name("S.DAT"), data = a.text("XY");
    a.call(Open, {aftn, out, 2, 0, st});
    a.call(Write, {2, data, 2, st});
    a.call(Seek, {2, 2, block, byte, st});   // to the position the two words give
    a.call(Write, {2, data, 2, st});         // beyond the end: the gap reads as zeros
    a.call(Seek, {2, 0, block, byte, st});   // where am I? (the words get the answer)
    a.call(Close, {2, st});
    a.call(Exit, {0});
    isis::Machine m(w.drives, "");
    m.start(w.program(a.build()));
    m.run(100000);
    CB_CHECK(m.finished());
    std::string expected = "XY" + std::string(130, '\0') + "XY";
    CB_CHECK_EQ(file_text(w.dir.path() / "S.DAT"), expected);
    CB_CHECK_EQ(Asm::word(m, block), 1);   // 134 = block 1, byte 6
    CB_CHECK_EQ(Asm::word(m, byte), 6);
});

CB_TEST("exact.emu.isis.the_command_line_is_read_from_the_console", [] {
    Work w;
    Asm a;
    const std::uint16_t st = a.var(), actual = a.var(), buffer = a.var(100);
    a.call(Read, {1, buffer, 100, actual, st});    // what follows the program's name
    a.call(Write, {0, buffer, 6, st});
    a.call(Rescan, {st});
    a.call(Read, {1, buffer, 100, actual, st});    // from the start: the name too
    a.call(Write, {0, buffer, 10, st});
    a.call(Exit, {0});
    isis::Machine m(w.drives, " abc");
    m.start(w.program(a.build()));
    m.run(100000);
    CB_CHECK_EQ(text::from_latin1(m.console()), " abc\r\nPROG abc\r\n");
});

CB_TEST("exact.emu.isis.drives_and_devices", [] {
    Work w;
    Asm a;
    const std::uint16_t aftn = a.var(), st = a.var(), info = a.var(12);
    const std::uint16_t on_one = a.name(":F1:ONE.TXT"), on_seven = a.name(":F7:SEVEN.TXT"), spath = a.name(":F1:ABC.OBJ"), co = a.name(":CO:"), bb = a.name(":BB:");
    const std::uint16_t data = a.text("x");
    a.call(Open, {aftn, on_one, 2, 0, st});
    a.call(Write, {2, data, 1, st});
    a.call(Close, {2, st});
    a.call(Open, {aftn, on_seven, 2, 0, st});   // drive 7 is not there: it is drive 0
    a.call(Write, {2, data, 1, st});
    a.call(Close, {2, st});
    a.call(Spath, {spath, info, st});
    a.call(Open, {aftn, co, 2, 0, st});          // the console as a file: number 0
    a.call(Exit, {0});
    isis::Machine m(w.drives, "");
    m.start(w.program(a.build()));
    m.run(100000);
    (void)bb;
    CB_CHECK_EQ(file_text(w.dir.path() / "sub" / "ONE.TXT"), "x");
    CB_CHECK_EQ(file_text(w.dir.path() / "SEVEN.TXT"), "x");
    // SPATH: drive 1, "ABC" padded to 6, "OBJ", 3, 0
    const ByteView mem = m.memory();
    const Bytes expected = {1, 'A', 'B', 'C', 0, 0, 0, 'O', 'B', 'J', 3, 0};
    CB_CHECK(equal_bytes(mem.subspan(info, 12), expected));
    CB_CHECK_EQ(Asm::word(m, aftn), 0);   // (the last OPEN: the console)
});

CB_TEST("exact.emu.isis.rename_delete_and_error", [] {
    Work w;
    fs::write_file(w.dir.path() / "A.TXT", as_bytes("a"));
    Asm a;
    const std::uint16_t renamed = a.word(99), deleted = a.word(99), reported = a.word(99);
    const std::uint16_t from = a.name("A.TXT"), to = a.name("B.TXT"), none = a.name("NONE.TXT"), nothing = a.name("NOTHING.TXT");
    a.call(Rename, {from, to, renamed});
    a.call(Delete, {none, deleted});
    a.call(ErrorReport, {5, reported});
    a.call(Rename, {nothing, to, deleted});   // (the second status variable is reused: the last call's status stays)
    a.call(Exit, {0});
    isis::Machine m(w.drives, "");
    m.start(w.program(a.build()));
    m.run(100000);
    CB_CHECK(!stdfs::exists(w.dir.path() / "A.TXT"));
    CB_CHECK_EQ(file_text(w.dir.path() / "B.TXT"), "a");
    CB_CHECK_EQ(Asm::word(m, renamed), 0);
    CB_CHECK_EQ(Asm::word(m, deleted), 13);   // renaming a file that is not there
    CB_CHECK_EQ(Asm::word(m, reported), 0);
    CB_CHECK(text::from_latin1(m.console()).starts_with("ERROR 5 USER PC "));
    CB_CHECK(text::from_latin1(m.console()).ends_with("\r\n"));
});

CB_TEST("exact.emu.isis.load_returns_the_entry_point", [] {
    Work w;
    // the program SUB.OBJ: MVI A,5 / HLT at 0200h
    fs::write_file(w.dir.path() / "SUB.OBJ", object_file({{0x200, {0x3E, 0x05, 0x76}}}, 0x200));
    Asm a;
    const std::uint16_t st = a.var(), entry = a.var();
    const std::uint16_t sub = a.name("SUB.OBJ"), absent = a.name("ABSENT.OBJ");
    a.call(Load, {sub, 0x1000, 0, entry, st});   // bias 1000h: loaded at 1200h, the entry comes back
    const std::uint16_t first_status = st;
    a.call(Load, {absent, 0, 0, entry, st});
    a.call(Exit, {0});
    isis::Machine m(w.drives, "");
    m.start(w.program(a.build()));
    m.run(100000);
    CB_CHECK_EQ(m.memory()[0x1200], 0x3E);
    CB_CHECK_EQ(Asm::word(m, entry), 0x1200);
    CB_CHECK_EQ(Asm::word(m, first_status), 13);   // (the second call's status: not found)
});

CB_TEST("exact.emu.isis.a_call_this_does_not_provide_is_an_error", [] {
    for (const int function : {15, 16, 99, 255}) {
        Work w;
        Asm a;
        a.call(function, {0});
        CB_CHECK_THROWS_AS(EmulationError, w.run(a));
    }
});

CB_TEST("exact.emu.isis.more_than_97_open_files_is_an_error", [] {
    Work w;
    Asm a;
    const std::uint16_t aftn = a.var(), st = a.var();
    const std::uint16_t name = a.name("F.TMP");
    for (int i = 0; i < 98; ++i) a.call(Open, {aftn, name, 2, 0, st});
    CB_CHECK_THROWS_AS(EmulationError, w.run(a));
});

CB_TEST("exact.emu.isis.damaged_object_files", [] {
    Work w;
    const Bytes good = object_file({{0x100, {0x76}}}, 0x100);
    // cut in the middle of the content record
    Bytes cut(good.begin(), good.begin() + 14);
    isis::Machine a(w.drives, "");
    CB_CHECK_THROWS_AS(TruncatedError, a.load(w.program(cut)));
    // a content record too short to hold an address
    isis::Machine b(w.drives, "");
    CB_CHECK_THROWS_AS(FormatError, b.load(w.program(record(0x06, {1}))));
    // an entry record too short
    isis::Machine c(w.drives, "");
    CB_CHECK_THROWS_AS(FormatError, c.load(w.program(record(0x04, {0, 0}))));
    // no such file
    isis::Machine d(w.drives, "");
    CB_CHECK_THROWS_AS(NotFoundError, d.load(w.dir.path() / "NOPE.OBJ"));
    // records of types it does not know are skipped; a file with no entry record starts at 0
    isis::Machine e(w.drives, "");
    CB_CHECK_EQ(e.load(w.program(record(0x99, {1, 2, 3}))), std::uint32_t{0});
    // what is loaded past the top of memory is cut off, not wrapped
    isis::Machine f(w.drives, "");
    f.load(w.program(object_file({{0xFFFE, {1, 2, 3, 4}}}, 0)));
    CB_CHECK_EQ(f.memory()[0xFFFE], 1);
    CB_CHECK_EQ(f.memory()[0xFFFF], 2);
    CB_CHECK_EQ(f.memory()[0], 0);
    // a machine needs drive 0
    CB_CHECK_THROWS_AS(UsageError, isis::Machine({{1, w.dir.path()}}, ""));
});

// What the Python this replaces did with 91 runs (a hand-made set that uses every call, and 55 programs of random
// instructions that use every instruction, run on the fixtures in testdata/isis): the console, the instruction count,
// the registers, the flags, a checksum of the whole 64 KiB, and the host files afterwards, all equal.
CB_TEST("exact.emu.isis.programs_of_the_python_it_replaces", [] {
    const stdfs::path data = testing::data_path("exact/emu/testdata/isis");
    const std::string lines = fs::read_text(data / "cases.jsonl");
    int cases = 0;
    for (const std::string_view line : text::lines(lines)) {
        if (line.empty()) continue;
        const Json expected = json::parse(line);
        const std::string program = expected["program"];
        fs::ScopedTempDir work("isis_case");
        fs::copy_tree(data / expected["fixture"].get<std::string>(), work.path());
        fs::make_dirs(work.path() / "sub");   // (an empty folder is not in the checkout)
        std::string echoed;
        isis::Options options;
        if (expected["echo"].get<bool>()) options.echo = [&](std::string_view s) { echoed += s; };
        isis::Machine m({{0, work.path()}, {1, work.path() / "sub"}}, expected["tail"].get<std::string>(), options);
        m.start(data / program);
        const std::uint64_t steps = m.run(expected["limit"].get<std::uint64_t>());
        ++cases;
        const std::string label = program + " " + expected["tail"].get<std::string>();
        const auto check = [&](bool ok, const std::string& what) {
            if (!ok) testing::record_failure(__FILE__, __LINE__, label + ": " + what);
        };
        check(text::from_latin1(m.console()) == expected["console"].get<std::string>(), "console");
        check(steps == expected["steps"].get<std::uint64_t>(), "steps " + std::to_string(steps) + " vs " + std::to_string(expected["steps"].get<std::uint64_t>()));
        const isis::Cpu& c = m.cpu();
        const Json& regs = expected["regs"];   // B C D E H L A
        check(std::vector<int>{c.b, c.c, c.d, c.e, c.h, c.l, c.a} == regs.get<std::vector<int>>(), "registers");
        check(c.sp == expected["sp"].get<int>() && c.pc == expected["pc"].get<int>(), "sp/pc");
        const Json& f = expected["flags"];
        check(c.zero == (f["zero"].get<int>() != 0) && c.sign == (f["sign"].get<int>() != 0) && c.parity == (f["parity"].get<int>() != 0) &&
                  c.carry == (f["carry"].get<int>() != 0) && c.aux_carry == (f["aux"].get<int>() != 0),
              "flags");
        check(crypto::crc32(m.memory()) == expected["memory_crc32"].get<std::uint32_t>(), "memory");
        check(echoed == expected["echoed"].get<std::string>(), "echo");
        // the host files afterwards
        std::size_t found = 0;
        for (const auto& entry : stdfs::recursive_directory_iterator(work.path())) {
            if (!entry.is_regular_file()) continue;
            ++found;
            const std::string rel = text::replace_all(fs::to_text(stdfs::relative(entry.path(), work.path())), "\\", "/");
            const auto it = expected["files"].find(rel);
            check(it != expected["files"].end() && it->get<std::string>() == file_text(entry.path()), "file " + rel);
        }
        check(found == expected["files"].size(), "number of files");
    }
    CB_CHECK_EQ(cases, 91);
});
