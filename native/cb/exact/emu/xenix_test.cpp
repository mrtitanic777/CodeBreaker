#include "cb/exact/emu/xenix.hpp"

#include <algorithm>
#include <chrono>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

using namespace cb;
using namespace cb::exact::emu;

namespace {

namespace stdfs = std::filesystem;

// ---- assembling XENIX programs: 8086 code that makes system calls ---------------------------------------

enum : int { AX, CX, DX, BX, SP, BP, SI, DI };   // the order the mov r16,imm16 opcodes number them

Bytes word(std::uint32_t v) { return {static_cast<std::uint8_t>(v), static_cast<std::uint8_t>(v >> 8)}; }

Bytes mov(int reg, std::uint32_t value) {
    Bytes out = {static_cast<std::uint8_t>(0xB8 + reg)};
    append(out, word(value));
    return out;
}

Bytes cat(std::initializer_list<Bytes> parts) {
    Bytes out;
    for (const Bytes& p : parts) append(out, p);
    return out;
}

constexpr std::int32_t kUnset = -1;

// the registers set, then `mov dx,8800h / mov ax,n / out dx,ax`
Bytes sysc(int number, std::int32_t bx = kUnset, std::int32_t cx = kUnset, std::int32_t si = kUnset, std::int32_t di = kUnset) {
    Bytes out;
    const std::pair<int, std::int32_t> regs[] = {{BX, bx}, {CX, cx}, {SI, si}, {DI, di}};
    for (const auto& [reg, value] : regs) {
        if (value != kUnset) append(out, mov(reg, static_cast<std::uint32_t>(value)));
    }
    append(out, mov(DX, 0x8800));
    append(out, mov(AX, static_cast<std::uint32_t>(number)));
    out.push_back(0xEF);   // out dx,ax
    return out;
}

const Bytes kMovBxAx = {0x89, 0xC3};
const Bytes kMovSiAx = {0x89, 0xC6};
const Bytes kMovCxSp = {0x89, 0xE1};

// a far call to cs:target of XENIX 2.3's gate, with the registers set first
Bytes fcall(int number, std::int32_t bx = kUnset, std::uint32_t target = 2) {
    Bytes out;
    if (bx != kUnset) append(out, mov(BX, static_cast<std::uint32_t>(bx)));
    append(out, mov(AX, static_cast<std::uint32_t>(number)));
    out.push_back(0x9A);
    append(out, word(target));
    append(out, word(0x1000));
    return out;
}

Bytes aout(std::uint16_t magic, const Bytes& text, const Bytes& data, std::uint16_t entry = 0) {
    Bytes out;
    for (const std::uint32_t v : {std::uint32_t{magic}, static_cast<std::uint32_t>(text.size()), static_cast<std::uint32_t>(data.size()), 0u, 0u, std::uint32_t{entry}, 0u, 0u}) append(out, word(v));
    append(out, text);
    append(out, data);
    return out;
}

// A text with the data it names (data starts at offset 0 of the data segment, for the split magic 0109)
struct Program {
    Bytes text;
    Bytes data;
    std::uint16_t put(std::string_view s, bool nul = true) {
        const auto at = static_cast<std::uint16_t>(data.size());
        append(data, as_bytes(s));
        if (nul) data.push_back(0);
        return at;
    }
    std::uint16_t alloc(std::size_t n) {
        const auto at = static_cast<std::uint16_t>(data.size());
        data.resize(data.size() + n);
        return at;
    }
    Bytes build(std::uint16_t magic = 0x0109) const { return aout(magic, text, data); }
};

// A work folder and a root folder with a few files in them.
struct World {
    fs::ScopedTempDir dir{"xenix"};
    stdfs::path root = dir.path() / "root";
    stdfs::path work = dir.path() / "work";
    World() {
        fs::make_dirs(root / "bin");
        fs::make_dirs(root / "etc");
        fs::make_dirs(work / "tmp");
        fs::write_file(root / "etc" / "data.txt", as_bytes("0123456789ABCDEFGHIJ\nline two\n"));
    }
    void install(const std::string& name, const Bytes& image, bool in_work = false) { fs::write_file((in_work ? work : root) / name, image); }
    xenix::Spec spec(const std::string& program) const {
        xenix::Spec s;
        s.root = root;
        s.work = work;
        s.program = program;
        return s;
    }
};

std::string console_text(const xenix::Result& r) { return text::from_latin1(r.console); }

xenix::Result run_program(World& w, const Program& p, std::uint16_t magic = 0x0109) {
    w.install("bin/p", p.build(magic));
    return xenix::run(w.spec("bin/p"));
}

}  // namespace

CB_TEST("exact.emu.xenix.exit_status_and_the_trace_of_calls", [] {
    World w;
    Program p;
    p.text = sysc(1, 7);
    w.install("bin/p", p.build());
    xenix::Spec spec = w.spec("bin/p");
    spec.trace = true;
    const xenix::Result r = xenix::run(spec);
    CB_CHECK(r.status.has_value());
    CB_CHECK_EQ(r.status.value_or(0), 7);
    CB_CHECK(!r.error.has_value());
    CB_CHECK_EQ(r.calls.size(), std::size_t{1});
    CB_CHECK(r.calls[0] == (xenix::SysCall{1, 7, 0, 0, 0}));
    // without the trace there are no calls
    CB_CHECK(xenix::run(w.spec("bin/p")).calls.empty());
});

CB_TEST("exact.emu.xenix.write_goes_to_the_console", [] {
    World w;
    Program p;
    const std::uint16_t s = p.put("hello, xenix\n", false);
    p.text = cat({sysc(4, 1, s, 13), sysc(4, 2, s, 5), sysc(1, 0)});   // fd 1 and fd 2 are both the console
    for (const std::uint16_t magic : {std::uint16_t{0x0109}, std::uint16_t{0x0107}, std::uint16_t{0x0108}}) {
        Program q = p;
        // a program with one segment has its data after the text: the address moves by the text's size
        if (magic != 0x0109) {
            const auto base = static_cast<std::uint32_t>(p.text.size());
            q.text = cat({sysc(4, 1, s + base, 13), sysc(4, 2, s + base, 5), sysc(1, 0)});
        }
        const xenix::Result r = run_program(w, q, magic);
        CB_CHECK_EQ(console_text(r), "hello, xenix\nhello");
        CB_CHECK_EQ(r.status.value_or(99), 0);
    }
});

CB_TEST("exact.emu.xenix.the_arguments_are_on_the_stack", [] {
    World w;
    Program p;
    p.text = cat({kMovCxSp, sysc(4, 1, kUnset, 40), sysc(1, 0)});   // write 40 bytes from the stack pointer
    w.install("bin/p", p.build());
    xenix::Spec spec = w.spec("bin/p");
    spec.args = {"a", "bc"};
    const xenix::Result r = xenix::run(spec);
    CB_CHECK_EQ(r.console.size(), std::size_t{40});
    const auto at = [&](std::size_t i) { return static_cast<int>(r.console[i] | (r.console[i + 1] << 8)); };
    CB_CHECK_EQ(at(0), 3);                 // argc: the program and its two arguments
    const int argv0 = at(2), argv1 = at(4), argv2 = at(6);
    CB_CHECK_EQ(at(8), 0);                 // argv's end
    CB_CHECK_EQ(at(10), 0);                // envp's end
    CB_CHECK_EQ(argv1 - argv0, 6);         // "bin/p\0"
    CB_CHECK_EQ(argv2 - argv1, 2);         // "a\0"
    // the strings sit at the top of the data segment, just below 0xFF00, and the stack pointer is below the vector
    CB_CHECK(argv2 < 0xFF00 && argv0 > 0xFE00);
});

CB_TEST("exact.emu.xenix.stdin_is_read_in_pieces", [] {
    World w;
    Program p;
    const std::uint16_t buf = p.alloc(64);
    p.text = cat({sysc(3, 0, buf, 20), kMovSiAx, sysc(4, 1, buf), sysc(3, 0, buf, 20), kMovSiAx, sysc(4, 1, buf), sysc(1, 0)});
    w.install("bin/p", p.build());
    xenix::Spec spec = w.spec("bin/p");
    spec.stdin_data = to_bytes("hello stdin data that is longer than twenty bytes\n");
    const xenix::Result r = xenix::run(spec);
    CB_CHECK_EQ(console_text(r), "hello stdin data that is longer than twe");   // the first 20 bytes, then the next 20
});

CB_TEST("exact.emu.xenix.files_are_made_read_written_and_removed", [] {
    World w;
    fs::write_file(w.work / "etc" / "gone.txt", as_bytes("remove me"));   // (the work folder has no etc yet: write_file makes it)
    Program p;
    const std::uint16_t made = p.put("a/b/c.txt"), content = p.put("xyz", false);
    const std::uint16_t gone = p.put("etc/gone.txt"), kept = p.put("etc/data.txt");
    // creat a/b/c.txt, write xyz, close; unlink the work file; unlink a file of the root (said to work, and kept)
    p.text = cat({sysc(8, made, 0), sysc(4, 3, content, 3), sysc(6, 3), sysc(10, gone), sysc(10, kept), sysc(1, 0)});
    const xenix::Result r = run_program(w, p);
    CB_CHECK(!r.error.has_value());
    CB_CHECK_EQ(text::from_latin1(fs::read_file(w.work / "a" / "b" / "c.txt")), "xyz");
    CB_CHECK(!stdfs::exists(w.work / "etc" / "gone.txt"));
    CB_CHECK(stdfs::exists(w.root / "etc" / "data.txt"));
});

CB_TEST("exact.emu.xenix.a_file_of_the_work_folder_hides_the_one_of_the_root", [] {
    World w;
    fs::write_file(w.work / "etc" / "data.txt", as_bytes("WORK data file\n"));
    Program p;
    const std::uint16_t name = p.put("etc/data.txt"), buf = p.alloc(64);
    p.text = cat({sysc(5, name, 0), sysc(3, 3, buf, 64), kMovSiAx, sysc(4, 1, buf), sysc(1, 0)});
    CB_CHECK_EQ(console_text(run_program(w, p)), "WORK data file\n");
    stdfs::remove(w.work / "etc" / "data.txt");
    CB_CHECK_EQ(console_text(run_program(w, p)), "0123456789ABCDEFGHIJ\nline two\n");   // now the root's
});

CB_TEST("exact.emu.xenix.read_write_and_seek_on_one_file", [] {
    World w;
    fs::write_file(w.work / "etc" / "rw.txt", as_bytes("abcdefghij"));
    Program p;
    const std::uint16_t name = p.put("etc/rw.txt"), zz = p.put("ZZ", false), buf = p.alloc(32);
    // open for reading and writing, overwrite the first two bytes, seek to 1 and write one byte, back to 0, read it all
    p.text = cat({sysc(5, name, 2), sysc(4, 3, zz, 2), sysc(19, 3, 0, 1, 0), sysc(4, 3, zz, 1), sysc(19, 3, 0, 0, 0), sysc(3, 3, buf, 32), kMovSiAx, sysc(4, 1, buf), sysc(1, 0)});
    const xenix::Result r = run_program(w, p);
    CB_CHECK_EQ(console_text(r), "ZZcdefghij");
    CB_CHECK_EQ(text::from_latin1(fs::read_file(w.work / "etc" / "rw.txt")), "ZZcdefghij");   // written back at the end of the run, though never closed
});

CB_TEST("exact.emu.xenix.lseek_whence_and_errors", [] {
    World w;
    fs::write_file(w.work / "etc" / "f.txt", as_bytes("0123456789"));
    Program p;
    const std::uint16_t name = p.put("etc/f.txt");
    const auto status_after = [&](const Bytes& calls) {
        Program q = p;
        q.text = cat({sysc(5, name, 0), calls, kMovBxAx, sysc(1)});   // exit with the result of the last call
        return run_program(w, q).status.value_or(0xFFFF);
    };
    CB_CHECK_EQ(status_after(sysc(19, 3, 0, 5, 0)), 5);                       // set
    CB_CHECK_EQ(status_after(cat({sysc(19, 3, 0, 5, 0), sysc(19, 3, 0xFFFF, 0xFFFD, 1)})), 2);   // cur, -3
    CB_CHECK_EQ(status_after(sysc(19, 3, 0xFFFF, 0xFFFF, 2)), 9);             // end, -1
    CB_CHECK_EQ(status_after(sysc(19, 3, 0xFFFF, 0xFFFF, 0)), 22);            // before the start: EINVAL
    CB_CHECK_EQ(status_after(sysc(19, 3, 0, 0, 7)), 22);                      // a whence there is not
    CB_CHECK_EQ(status_after(sysc(19, 9, 0, 0, 0)), 9);                       // not an open file: EBADF
    CB_CHECK_EQ(status_after(sysc(19, 1, 0, 0, 0)), 9);                       // the console cannot seek
});

CB_TEST("exact.emu.xenix.error_numbers", [] {
    World w;
    Program p;
    const std::uint16_t none = p.put("nonexistent"), data = p.put("etc/data.txt"), nul = p.put("tmp/NUL"), drive = p.put("C:/x"), back = p.put("a\\b"), up = p.put("../bin/p");
    const auto status_of = [&](const Bytes& call) {
        Program q = p;
        q.text = cat({call, kMovBxAx, sysc(1)});
        return run_program(w, q).status.value_or(0xFFFF);
    };
    CB_CHECK_EQ(status_of(sysc(5, none, 0)), 2);      // ENOENT
    CB_CHECK_EQ(status_of(sysc(33, none)), 2);
    CB_CHECK_EQ(status_of(sysc(33, data)), 0);
    CB_CHECK_EQ(status_of(sysc(18, none, 0)), 2);
    CB_CHECK_EQ(status_of(sysc(10, none)), 2);
    CB_CHECK_EQ(status_of(sysc(6, 9)), 9);            // EBADF
    CB_CHECK_EQ(status_of(sysc(3, 9, 0, 3)), 9);
    CB_CHECK_EQ(status_of(sysc(3, 1, 0, 3)), 9);      // fd 1 does not read
    CB_CHECK_EQ(status_of(sysc(4, 9, 0, 3)), 9);
    CB_CHECK_EQ(status_of(sysc(28, 9, 0)), 9);        // fstat
    CB_CHECK_EQ(status_of(sysc(54, 1)), 25);          // ENOTTY: nothing is a terminal
    CB_CHECK_EQ(status_of(sysc(20)), 86);             // getpid
    CB_CHECK_EQ(status_of(sysc(60)), 18);             // umask 022
    // names that reach outside the folders are not found, however they are spelled
    CB_CHECK_EQ(status_of(sysc(8, nul, 0)), 2);       // a Windows device
    CB_CHECK_EQ(status_of(sysc(5, drive, 0)), 2);
    CB_CHECK_EQ(status_of(sysc(5, back, 0)), 2);
    CB_CHECK_EQ(status_of(sysc(5, up, 0)), 2);        // ".." does not climb: it is a name of its own ("_")
});

CB_TEST("exact.emu.xenix.no_more_than_61_files_open", [] {
    World w;
    Program p;
    const std::uint16_t name = p.put("etc/data.txt");
    Bytes calls;
    for (int i = 0; i < 61; ++i) append(calls, sysc(5, name, 0));   // fds 3..63
    p.text = cat({calls, sysc(5, name, 0), kMovBxAx, sysc(1)});      // the 62nd
    CB_CHECK_EQ(run_program(w, p).status.value_or(0), 24);          // EMFILE
});

CB_TEST("exact.emu.xenix.stat_of_a_file_and_of_the_console", [] {
    World w;
    const stdfs::path file = w.work / "etc" / "s.txt";
    fs::write_file(file, as_bytes("0123456789ABCDE"));
    // a modification time of 1000000000: 2001-09-09
    std::error_code ec;
    stdfs::last_write_time(file, std::chrono::clock_cast<std::chrono::file_clock>(std::chrono::sys_seconds{std::chrono::seconds{1000000000}}), ec);
    Program p;
    const std::uint16_t name = p.put("etc/s.txt"), st = p.alloc(32);
    p.text = cat({sysc(18, name, st), sysc(4, 1, st, 30), sysc(28, 1, st), sysc(4, 1, st, 30), sysc(1, 0)});
    const xenix::Result r = run_program(w, p);
    CB_CHECK_EQ(r.console.size(), std::size_t{60});
    const auto at = [&](std::size_t i) { return static_cast<int>(r.console[i] | (r.console[i + 1] << 8)); };
    CB_CHECK_EQ(at(4), 0100644);          // a regular file, rw-r--r--
    CB_CHECK_EQ(at(14), 0);               // the size, a long, the high word first
    CB_CHECK_EQ(at(16), 15);
    CB_CHECK_EQ(at(18), 0x3B9A);          // atime = mtime = ctime = 1000000000 = 0x3B9ACA00
    CB_CHECK_EQ(at(20), 0xCA00);
    CB_CHECK_EQ(at(26), 0x3B9A);
    // fstat of the console: a character device
    CB_CHECK_EQ(at(30 + 4), 020622);
    CB_CHECK_EQ(at(30 + 14), 0);
});

CB_TEST("exact.emu.xenix.a_system_call_it_does_not_provide_ends_the_run", [] {
    World w;
    Program p;
    p.text = sysc(99, 5);
    const xenix::Result r = run_program(w, p);
    CB_CHECK(!r.status.has_value());
    CB_CHECK_EQ(r.error.value_or(""), "system call 99 (bx=0005 cx=0000 si=0000 di=0000) is not provided");
});

CB_TEST("exact.emu.xenix.faults_of_the_program_end_the_run", [] {
    World w;
    Program ud2;
    ud2.text = cat({mov(AX, 1), Bytes{0x0F, 0x0B}, sysc(1, 0)});   // an instruction that does not exist
    const xenix::Result r = run_program(w, ud2);
    CB_CHECK(!r.status.has_value());
    CB_CHECK(r.error.value_or("").find("UC_ERR_INSN_INVALID") != std::string::npos);
    CB_CHECK(r.error.value_or("").find(" at 1000:") != std::string::npos);
    // a program that never ends is stopped by the limit
    Program loop;
    loop.text = cat({mov(AX, 5), Bytes{0xEB, 0xFE}});
    w.install("bin/loop", loop.build());
    xenix::Spec spec = w.spec("bin/loop");
    spec.limit = 1000;
    const xenix::Result limited = xenix::run(spec);
    CB_CHECK(!limited.status.has_value());
    CB_CHECK(limited.error.value_or("").starts_with("did not finish (instruction limit, or it stopped at "));
    // a write that runs to the end of the segment and past it reads the next segment: the machine is flat
    Program far;
    far.text = cat({sysc(4, 1, 0xFFFF, 0xFFFF), sysc(1, 0)});
    const xenix::Result big = run_program(w, far);   // (65535 bytes from 0xFFFF in the data segment are still inside the megabyte)
    CB_CHECK(!big.error.has_value());
    CB_CHECK_EQ(big.console.size(), std::size_t{65535});
});

CB_TEST("exact.emu.xenix.hooks_see_and_change_the_machine", [] {
    World w;
    Program p;
    p.text = sysc(1, 7);   // mov bx,7 / mov dx,8800 / mov ax,1 / out dx,ax : the out is the 10th byte
    w.install("bin/p", p.build());
    xenix::Spec spec = w.spec("bin/p");
    std::vector<std::string> seen;
    spec.hooks[0] = [&](xenix::Machine& m, std::uint16_t cs, std::uint16_t ds) {
        seen.push_back(std::format("{:04x} {:04x} ip={:04x} sp={:04x}", cs, ds, m.reg(xenix::Reg::Ip), m.reg(xenix::Reg::Sp)));
    };
    spec.hooks[9] = [&](xenix::Machine& m, std::uint16_t, std::uint16_t) {
        m.set_reg(xenix::Reg::Bx, 42);   // just before the out: the exit status becomes 42
        seen.push_back(std::format("ax={} dx={:04x}", m.reg(xenix::Reg::Ax), m.reg(xenix::Reg::Dx)));
    };
    spec.hooks[99999] = [&](xenix::Machine&, std::uint16_t, std::uint16_t) { seen.push_back("never"); };
    const xenix::Result r = xenix::run(spec);
    CB_CHECK_EQ(r.status.value_or(0), 42);
    CB_CHECK_EQ(seen.size(), std::size_t{2});
    if (seen.size() == 2) {
        CB_CHECK(seen[0].starts_with("1000 3000 ip=0000"));
        CB_CHECK_EQ(seen[1], "ax=1 dx=8800");
    }
    // memory through the machine: the data segment is at 0x30000
    Program q;
    const std::uint16_t s = q.put("hello", false);
    q.text = cat({sysc(4, 1, s, 5), sysc(1, 0)});
    w.install("bin/q", q.build());
    xenix::Spec spec2 = w.spec("bin/q");
    spec2.hooks[0] = [&](xenix::Machine& m, std::uint16_t, std::uint16_t ds) {
        const Bytes before = m.read(ds * 16u, 5);
        CB_CHECK_EQ(std::string(before.begin(), before.end()), "hello");
        m.write(ds * 16u, as_bytes("HELLO"));
    };
    CB_CHECK_EQ(console_text(xenix::run(spec2)), "HELLO");
});

CB_TEST("exact.emu.xenix.a_hook_that_faults_ends_the_run_and_other_errors_pass_through", [] {
    World w;
    Program p;
    p.text = sysc(1, 0);
    w.install("bin/p", p.build());
    xenix::Spec spec = w.spec("bin/p");
    spec.hooks[0] = [](xenix::Machine& m, std::uint16_t, std::uint16_t) { (void)m.read(0xFFFFFFF0u, 64); };   // memory that is not there
    const xenix::Result r = xenix::run(spec);
    CB_CHECK(!r.status.has_value());
    CB_CHECK(r.error.has_value());
    xenix::Spec other = w.spec("bin/p");
    other.hooks[0] = [](xenix::Machine&, std::uint16_t, std::uint16_t) { throw UsageError("a hook's own failure"); };
    CB_CHECK_THROWS_AS(UsageError, xenix::run(other));
});

CB_TEST("exact.emu.xenix.the_gate_of_xenix_2_3", [] {
    World w;
    // a jump over a row of `jmp .`, then code at 0x80 that makes calls through the gate
    const Bytes head = cat({Bytes{0xEB, 0x7E, 0xEB, 0xFE, 0xEB, 0xFE}, Bytes(0x80 - 6, 0x90)});
    Program exit_only;
    exit_only.text = cat({head, fcall(1, 9)});
    w.install("bin/g1", exit_only.build());
    CB_CHECK_EQ(xenix::run(w.spec("bin/g1")).status.value_or(0), 9);   // a system call through the far call to 0002
    Program grow;
    const std::uint16_t s = grow.put("via gate\n", false);
    // write through the gate; ask for the stack to be grown to BX=0x1234 (the far call to 0004 gives AX=BX, carry clear)
    grow.text = cat({head, mov(CX, s), mov(SI, 9), fcall(4, 1), fcall(3, 0x1234, 4), kMovBxAx, sysc(1)});
    w.install("bin/g2", grow.build());
    const xenix::Result r = xenix::run(w.spec("bin/g2"));
    CB_CHECK_EQ(console_text(r), "via gate\n");
    CB_CHECK_EQ(r.status.value_or(0), 0x1234);
});

CB_TEST("exact.emu.xenix.programs_that_are_not_a_out_files", [] {
    World w;
    w.install("bin/notaout", cat({Bytes(64, 0), to_bytes("this is no a.out")}));
    CB_CHECK_THROWS_AS(FormatError, xenix::run(w.spec("bin/notaout")));
    w.install("bin/short", {0x09, 0x01, 0x00});
    CB_CHECK_THROWS_AS(TruncatedError, xenix::run(w.spec("bin/short")));
    w.install("bin/empty", {});
    CB_CHECK_THROWS_AS(TruncatedError, xenix::run(w.spec("bin/empty")));
    // the header says more than there is
    Bytes lying = aout(0x0109, Bytes(100, 0), Bytes(100, 0));
    lying.resize(60);
    w.install("bin/truncated", lying);
    CB_CHECK_THROWS_AS(TruncatedError, xenix::run(w.spec("bin/truncated")));
    CB_CHECK_THROWS_AS(NotFoundError, xenix::run(w.spec("bin/nothere")));
    CB_CHECK_THROWS_AS(NotFoundError, xenix::run(w.spec("../bin/p")));   // a name outside the system
    CB_CHECK_THROWS_AS(IoError, xenix::run(w.spec("bin")));              // a folder
    // arguments that do not fit in the stack
    Program p;
    p.text = sysc(1, 0);
    w.install("bin/p", p.build());
    xenix::Spec spec = w.spec("bin/p");
    spec.args = {std::string(70000, 'z')};
    CB_CHECK_THROWS_AS(UsageError, xenix::run(spec));
});

// What the Python this replaces did with 67 runs (programs that make every call there is, in many orders and with the
// calls failing, run on the file tree in testdata/xenix): the console, the exit status, the error, the calls, the work
// folder afterwards, and what hooks saw.
CB_TEST("exact.emu.xenix.runs_of_the_python_it_replaces", [] {
    const stdfs::path data = testing::data_path("exact/emu/testdata/xenix");
    int cases = 0;
    const std::string all_cases = fs::read_text(data / "cases.jsonl");
    for (const std::string_view line : text::lines(all_cases)) {
        if (line.empty()) continue;
        const Json expected = json::parse(line);
        // the work folder afresh: every file and folder with the modification time the Python's runs had
        fs::ScopedTempDir temp("xenix_case");
        const stdfs::path work = temp.path() / "run";
        fs::copy_tree(data / "work0", work);
        fs::make_dirs(work / "tmp");   // (an empty folder is not in the checkout)
        const auto fixed = std::chrono::clock_cast<std::chrono::file_clock>(std::chrono::sys_seconds{std::chrono::seconds{1000000000}});
        for (const auto& e : stdfs::recursive_directory_iterator(work)) {
            std::error_code ec;
            stdfs::last_write_time(e.path(), fixed, ec);
        }
        // (the root is used where it is; its files are only read)
        const std::string kind = expected["kind"];
        xenix::Spec spec;
        spec.root = data / "root";
        spec.work = work;
        spec.program = expected["program"];
        std::string seen;
        if (kind == "run") {
            spec.trace = expected["trace"];
            spec.limit = expected["limit"];
            spec.stdin_data = crypto::from_hex(expected["stdin"].get<std::string>());
            for (const Json& a : expected["args"]) {
                const Bytes raw = crypto::from_hex(a["hex"].get<std::string>());
                spec.args.push_back(std::string(as_text(raw)));
            }
        } else {
            for (const std::uint32_t at : expected["at"].get<std::vector<std::uint32_t>>()) {
                spec.hooks[at] = [&seen, at](xenix::Machine& m, std::uint16_t cs, std::uint16_t ds) {
                    if (!seen.empty()) seen += "|";
                    seen += std::format("{}:{},{},{},{},{},{},{}", at, m.reg(xenix::Reg::Ax), m.reg(xenix::Reg::Bx), m.reg(xenix::Reg::Cx), m.reg(xenix::Reg::Si),
                                        m.reg(xenix::Reg::Sp), cs, ds);
                };
            }
        }
        const xenix::Result r = xenix::run(spec);
        ++cases;
        const std::string label = kind + " " + spec.program;
        const auto check = [&](bool ok, const std::string& what) {
            if (!ok) testing::record_failure(__FILE__, __LINE__, label + ": " + what);
        };
        check(crypto::to_hex(crypto::sha1(r.console)) == expected["console_sha1"].get<std::string>() && r.console.size() == expected["console_size"].get<std::size_t>(), "console");
        if (expected.contains("console_hex")) check(crypto::to_hex(r.console) == expected["console_hex"].get<std::string>(), "console text");
        check((r.status ? std::to_string(*r.status) : "N") == expected["status"].get<std::string>(), "status");
        check((r.error ? crypto::to_hex(as_bytes(*r.error)) : "N") == expected["error"].get<std::string>(),
              "error " + r.error.value_or("(none)") + " expected " + std::string(as_text(crypto::from_hex(expected["error"].get<std::string>() == "N" ? "" : expected["error"].get<std::string>()))));
        if (kind == "hook") {
            check(seen == expected["hooks"].get<std::string>(), "hooks " + seen);
            continue;
        }
        std::string calls;
        for (const xenix::SysCall& c : r.calls) {
            if (!calls.empty()) calls += ";";
            calls += std::format("{},{},{},{},{}", c.number, c.bx, c.cx, c.si, c.di);
        }
        check(calls == expected["calls"].get<std::string>(), "calls");
        // the work folder: "F:path:hex" for files, "D:path" for folders, sorted, joined by ';'
        std::vector<std::string> tree;
        for (const auto& e : stdfs::recursive_directory_iterator(work)) {
            const std::string rel = text::replace_all(fs::to_text(stdfs::relative(e.path(), work)), "\\", "/");
            if (e.is_directory()) tree.push_back("D:" + rel);
            else tree.push_back("F:" + rel + ":" + crypto::to_hex(fs::read_file(e.path())));
        }
        std::sort(tree.begin(), tree.end());
        check(crypto::to_hex(crypto::sha1(as_bytes(text::join(tree, ";")))) == expected["fs_sha1"].get<std::string>(), "work folder");
    }
    CB_CHECK_EQ(cases, 67);
});
