// codebreaker/exact/emu/xenix.cpp -- C++ port of codebreaker.exact.emu.xenix (see xenix.hpp).

#include "codebreaker/exact/emu/xenix.hpp"

#include <sys/stat.h>
#include <sys/types.h>

#include <cstdio>
#include <exception>
#include <map>
#include <memory>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <unicorn/unicorn.h>
#include <unicorn/x86.h>

namespace codebreaker::exact::emu::xenix {

const int64_t CS = 0x1000;
const int64_t DS = 0x3000;
const int64_t ENOENT_ = 2;
const int64_t EBADF_ = 9;
const int64_t ENOTTY_ = 25;
const int64_t EINVAL_ = 22;

// ---- Unicorn, by the DLL Python's package carries ---------------------------------------------------

namespace {
struct UcLib {
    decltype(&uc_open) open = nullptr;
    decltype(&uc_close) close = nullptr;
    decltype(&uc_reg_read) reg_read = nullptr;
    decltype(&uc_reg_write) reg_write = nullptr;
    decltype(&uc_mem_read) mem_read = nullptr;
    decltype(&uc_mem_write) mem_write = nullptr;
    decltype(&uc_mem_map) mem_map = nullptr;
    decltype(&uc_emu_start) emu_start = nullptr;
    decltype(&uc_emu_stop) emu_stop = nullptr;
    decltype(&uc_hook_add) hook_add = nullptr;
    decltype(&uc_strerror) strerror = nullptr;
};
UcLib UC;
bool uc_loaded = false;

template <typename T> bool load_symbol(HMODULE m, const char* name, T& slot) {
    slot = reinterpret_cast<T>(GetProcAddress(m, name));
    return slot != nullptr;
}

const UcLib& unicorn() {
    if (!uc_loaded) throw py::RuntimeError("No module named 'unicorn'");
    return UC;
}

// The engine of one run, as hooks see it.
class UcMachine : public Machine {
  public:
    explicit UcMachine(uc_engine* uc) : uc_(uc) {}
    int64_t reg_read(int64_t reg) override {
        uint64_t v = 0;
        check(unicorn().reg_read(uc_, static_cast<int>(reg), &v));
        return static_cast<int64_t>(v);
    }
    void reg_write(int64_t reg, int64_t value) override {
        uint64_t v = static_cast<uint64_t>(value);
        check(unicorn().reg_write(uc_, static_cast<int>(reg), &v));
    }
    py::bytes mem_read(int64_t address, int64_t size) override {
        std::string buf(static_cast<size_t>(size), '\0');
        check(unicorn().mem_read(uc_, static_cast<uint64_t>(address), &buf[0], static_cast<size_t>(size)));
        return py::bytes(buf);
    }
    void mem_write(int64_t address, const py::bytes& data) override {
        check(unicorn().mem_write(uc_, static_cast<uint64_t>(address), data.raw().data(), data.raw().size()));
    }
    void emu_stop() override { check(unicorn().emu_stop(uc_)); }
    static void check(uc_err e) {
        if (e != UC_ERR_OK) throw UcError(unicorn().strerror(e));
    }

  private:
    uc_engine* uc_;
};

// ---- struct.pack helpers -----------------------------------------------------------------------------
void pack_H(std::string& out, int64_t v) {   // struct.pack("<H", v)
    if (v < 0 || v > 0xFFFF) throw py::StructError("ushort format requires 0 <= number <= 65535");
    out.push_back(static_cast<char>(v & 0xFF));
    out.push_back(static_cast<char>((v >> 8) & 0xFF));
}
void pack_long(std::string& out, int64_t v) {   // V7's long: the high word first, each word little-endian
    pack_H(out, py::shr(v, 16) & 0xFFFF);
    pack_H(out, v & 0xFFFF);
}

// ---- file descriptors --------------------------------------------------------------------------------
enum class Kind { In, Out, File };
struct Fd {
    Kind kind = Kind::Out;
    int64_t pos = 0;     // an "in" descriptor: how much of stdin was read
    py::file file;       // a "file" one
    py::str name;        // the file's name (the host path it was opened by)
};
}  // namespace

void load_unicorn(const py::str& dll_path) {
    std::wstring w;
    for (char32_t c : dll_path.u32()) {
        if (c >= 0x10000) {
            w.push_back(static_cast<wchar_t>(0xD800 + ((c - 0x10000) >> 10)));
            w.push_back(static_cast<wchar_t>(0xDC00 + ((c - 0x10000) & 0x3FF)));
        } else {
            w.push_back(static_cast<wchar_t>(c));
        }
    }
    HMODULE u = LoadLibraryW(w.c_str());
    if (!u) throw py::RuntimeError("cannot load unicorn.dll (error " + std::to_string(GetLastError()) + ")");
    uc_loaded = load_symbol(u, "uc_open", UC.open) && load_symbol(u, "uc_close", UC.close) &&
                load_symbol(u, "uc_reg_read", UC.reg_read) && load_symbol(u, "uc_reg_write", UC.reg_write) &&
                load_symbol(u, "uc_mem_read", UC.mem_read) && load_symbol(u, "uc_mem_write", UC.mem_write) &&
                load_symbol(u, "uc_mem_map", UC.mem_map) && load_symbol(u, "uc_emu_start", UC.emu_start) &&
                load_symbol(u, "uc_emu_stop", UC.emu_stop) && load_symbol(u, "uc_hook_add", UC.hook_add) &&
                load_symbol(u, "uc_strerror", UC.strerror);
    if (!uc_loaded) throw py::RuntimeError("unicorn.dll lacks an entry point");
}

namespace {
// What a Unicorn callback runs (the callbacks are C functions; `user` points at one of these).
struct OutCallback {
    std::function<void(uint32_t port, int size, uint32_t value)> fn;
};
struct CodeCallback {
    std::function<void(uint64_t address, uint32_t size)> fn;
};
std::exception_ptr hook_exception;   // (an exception raised in a callback is re-raised after emu_start)
uc_engine* current_uc = nullptr;

void stash_exception() {
    if (!hook_exception) hook_exception = std::current_exception();
    unicorn().emu_stop(current_uc);
}
void out_trampoline(uc_engine*, uint32_t port, int size, uint32_t value, void* user) {
    try {
        static_cast<OutCallback*>(user)->fn(port, size, value);
    } catch (...) {
        stash_exception();
    }
}
void code_trampoline(uc_engine*, uint64_t address, uint32_t size, void* user) {
    try {
        static_cast<CodeCallback*>(user)->fn(address, size);
    } catch (...) {
        stash_exception();
    }
}

// os.path.join(base, *rel.split("/"))
py::str join_parts(const py::str& base, const py::str& rel) {
    py::str out = base;
    for (const py::str& part : py::split(rel, py::str("/"))) {
        out = py::os::path::join(out, part);
    }
    return out;
}

// the V7 stat structure: dev, ino, mode, nlink, uid, gid, rdev (7 words), size (long), atime, mtime, ctime (longs)
py::bytes stat_buf(const py::str& p) {
    struct _stat64 st;
    std::wstring w;
    for (char32_t c : p.u32()) w.push_back(static_cast<wchar_t>(c));   // (BMP names; the XENIX files are ASCII)
    if (_wstat64(w.c_str(), &st) != 0) {
        throw py::FileNotFoundError("[WinError 2] The system cannot find the file specified: " + py::repr(p).utf8());
    }
    int64_t mode = (py::os::path::isdir(p) ? 040000 : 0100000) | 0644;
    std::string out;
    int64_t words[7] = {1, 2, mode, 1, 0, 0, 0};
    for (int64_t v : words) pack_H(out, v);
    pack_long(out, static_cast<int64_t>(st.st_size));
    for (int k = 0; k < 3; ++k) pack_long(out, static_cast<int64_t>(st.st_mtime));
    return py::bytes(out);
}
}  // namespace

Result run(const py::str& root, const py::str& work, const py::str& prog, const py::list<Arg>& args,
           const py::bytes& stdin_, bool trace, int64_t limit, const py::dict<int64_t, Hook>& hooks) {
    // the path of a file as the host has it: in WORKDIR first, then in ROOT
    auto host = [&](const py::str& p, bool create) -> py::str {
        py::str rel = p.lstrip(py::str("/")).replace(py::str(".."), py::str("_"));
        py::str w = py::truthy(rel) ? join_parts(work, rel) : work;
        if (create || py::os::path::exists(w)) return w;
        py::str r = py::truthy(rel) ? join_parts(root, rel) : root;
        return py::os::path::exists(r) ? r : w;
    };
    py::file image = py::open(host(prog, false), py::str("rb"));
    py::bytes b = image.read();
    image.close();
    if (py::len(b) < 16) throw py::StructError("unpack_from requires a buffer of at least 16 bytes for unpacking 16 bytes at offset 0");
    auto word = [&](int64_t k) { return b.byte_(2 * k) | (b.byte_(2 * k + 1) << 8); };
    int64_t magic = word(0), tsz = word(1), dsz = word(2), bsz = word(3), entry = word(5);
    if (magic != 0x0107 && magic != 0x0108 && magic != 0x0109) {
        throw SystemExit(py::percent("%s is not an a.out file (%04x)", prog, magic).utf8());
    }
    py::bytes text = b.slice(16, 16 + tsz);
    py::bytes data = b.slice(16 + tsz, 16 + tsz + dsz);

    const UcLib& lib = unicorn();
    uc_engine* uc = nullptr;
    UcMachine::check(lib.open(UC_ARCH_X86, UC_MODE_16, &uc));
    struct EngineGuard {   // (Python leaves the engine to its garbage collector)
        uc_engine* uc;
        ~EngineGuard() {
            unicorn().close(uc);
            current_uc = nullptr;
        }
    } guard{uc};
    current_uc = uc;
    hook_exception = nullptr;
    UcMachine mu(uc);
    UcMachine::check(lib.mem_map(uc, 0, 0x100000, UC_PROT_ALL));
    bool split = magic == 0x0109;
    int64_t cs = CS;
    int64_t ds = split ? DS : CS;
    mu.mem_write(cs * 16, text);
    mu.mem_write(ds * 16 + (split ? 0 : tsz), data);
    int64_t brk = split ? (dsz + bsz) : (tsz + dsz + bsz);
    // the stack: argc, argv[], 0, envp[], 0, the strings - at the top of the data segment
    py::list<py::bytes> argv;
    argv.append(prog.encode());
    for (const Arg& a : args) {
        argv.append(std::holds_alternative<py::str>(a) ? std::get<py::str>(a).encode() : std::get<py::bytes>(a));
    }
    int64_t sp = 0xFF00;
    std::string strs;
    py::list<int64_t> ptrs;
    for (const py::bytes& a : argv) {
        ptrs.append(static_cast<int64_t>(strs.size()));
        strs += a.raw();
        strs.push_back('\0');
    }
    int64_t sbase = sp - static_cast<int64_t>(strs.size()) - (static_cast<int64_t>(strs.size()) & 1);
    mu.mem_write(ds * 16 + sbase, py::bytes(strs));
    std::string vec;
    pack_H(vec, py::len(argv));
    for (int64_t p : ptrs) pack_H(vec, sbase + p);
    pack_H(vec, 0);
    pack_H(vec, 0);
    sp = sbase - static_cast<int64_t>(vec.size());
    sp -= sp & 1;
    mu.mem_write(ds * 16 + sp, py::bytes(vec));
    mu.reg_write(UC_X86_REG_CS, cs);
    mu.reg_write(UC_X86_REG_DS, ds);
    mu.reg_write(UC_X86_REG_ES, ds);
    mu.reg_write(UC_X86_REG_SS, ds);
    mu.reg_write(UC_X86_REG_SP, sp);
    mu.reg_write(UC_X86_REG_BP, 0);
    Result res;
    std::map<int64_t, std::shared_ptr<Fd>> fds;
    {
        auto in = std::make_shared<Fd>();
        in->kind = Kind::In;
        fds[0] = in;
        auto out1 = std::make_shared<Fd>();
        out1->kind = Kind::Out;
        fds[1] = out1;
        auto out2 = std::make_shared<Fd>();
        out2->kind = Kind::Out;
        fds[2] = out2;
    }
    std::string console;
    auto R = [&](int reg) { return mu.reg_read(reg); };
    auto cstr = [&](int64_t off) -> std::string {
        std::string s;
        while (true) {
            std::string c = mu.mem_read(ds * 16 + off + static_cast<int64_t>(s.size()), 64).raw();
            size_t k = c.find('\0');
            if (k != std::string::npos) return s + c.substr(0, k);
            s += c;
        }
    };
    auto ret = [&](int64_t ax, std::optional<int64_t> bx, std::optional<int64_t> err) {
        int64_t fl = R(UC_X86_REG_EFLAGS);
        if (err) {
            mu.reg_write(UC_X86_REG_AX, *err);
            mu.reg_write(UC_X86_REG_EFLAGS, fl | 1);
        } else {
            mu.reg_write(UC_X86_REG_AX, ax & 0xFFFF);
            mu.reg_write(UC_X86_REG_EFLAGS, fl & ~static_cast<int64_t>(1));
            if (bx) mu.reg_write(UC_X86_REG_BX, *bx & 0xFFFF);
        }
    };
    auto ok = [&](int64_t ax = 0, std::optional<int64_t> bx = std::nullopt) { ret(ax, bx, std::nullopt); };
    auto fail = [&](int64_t err) { ret(0, std::nullopt, err); };
    auto newfd = [&](std::shared_ptr<Fd> f) -> int64_t {
        for (int64_t k = 3; k < 64; ++k) {
            if (!fds.count(k)) {
                fds[k] = f;
                return k;
            }
        }
        throw StopIteration();
    };
    auto host_bytes = [&](const std::string& raw, bool create) { return host(py::bytes(raw).decode("latin1"), create); };
    auto open_file = [&](const py::str& p, const char* mode) {
        auto f = std::make_shared<Fd>();
        f->kind = Kind::File;
        f->file = py::open(p, py::str(mode));
        f->name = p;
        return f;
    };
    // (a buffered file object lets a read follow a write; the C stream needs a seek between)
    auto sync = [&](Fd& f) { f.file.seek(0, 1); };

    auto syscall = [&](uint32_t port, int, uint32_t value) {
        if (port != 0x8800) return;
        int64_t n = value & 0xFFFF;
        int64_t bx = R(UC_X86_REG_BX), cx = R(UC_X86_REG_CX), si = R(UC_X86_REG_SI), di = R(UC_X86_REG_DI);
        if (trace) res.calls.append(Call(n, bx, cx, si, di));
        if (n == 1) {
            res.status = bx;
            mu.emu_stop();
            return;
        }
        if (n == 3) {   // read(fd, buf, count)
            auto it = fds.find(bx);
            if (it == fds.end()) return fail(EBADF_);
            Fd& f = *it->second;
            py::bytes d;
            if (f.kind == Kind::In) {
                d = stdin_.slice(f.pos, f.pos + si);
                f.pos += py::len(d);
            } else if (f.kind == Kind::File) {
                sync(f);
                d = f.file.read(si);
            } else {
                return fail(EBADF_);
            }
            mu.mem_write(ds * 16 + cx, d);
            return ok(py::len(d));
        }
        if (n == 4) {   // write(fd, buf, count)
            auto it = fds.find(bx);
            py::bytes d = mu.mem_read(ds * 16 + cx, si);
            if (it == fds.end()) return fail(EBADF_);
            Fd& f = *it->second;
            if (f.kind == Kind::Out) {
                console += d.raw();
            } else if (f.kind == Kind::File) {
                sync(f);
                f.file.write(d);
            }
            return ok(py::len(d));
        }
        if (n == 5) {   // open(name, mode)
            py::str p = host_bytes(cstr(bx), false);
            if (!py::os::path::isfile(p)) return fail(ENOENT_);
            return ok(newfd(open_file(p, cx == 0 ? "rb" : "r+b")));
        }
        if (n == 8) {   // creat(name, mode)
            py::str p = host_bytes(cstr(bx), true);
            py::os::makedirs(py::os::path::dirname(p), 0777, true);
            return ok(newfd(open_file(p, "w+b")));
        }
        if (n == 6) {   // close(fd)
            auto it = fds.find(bx);
            if (it == fds.end()) return fail(EBADF_);
            std::shared_ptr<Fd> f = it->second;
            fds.erase(it);
            if (f->kind == Kind::File) f->file.close();
            return ok(0);
        }
        if (n == 10) {   // unlink(name)
            py::str p = host_bytes(cstr(bx), false);
            if (py::os::path::isfile(p) && py::os::path::abspath(p).startswith(py::os::path::abspath(work))) {
                py::os::remove(p);
                return ok(0);
            }
            if (py::os::path::exists(p)) return ok(0);
            return fail(ENOENT_);
        }
        if (n == 19) {   // lseek(fd, long offset (high, low), whence)
            auto it = fds.find(bx);
            if (it == fds.end() || it->second->kind != Kind::File) return fail(EBADF_);
            Fd& f = *it->second;
            int64_t off = (cx << 16) | si;
            off -= (off & 0x80000000) << 1;
            if (di < 0 || di > 2) throw py::ValueError(py::percent("whence value %d unsupported", di));
            if (di == 2) f.file.flush();
            int64_t base = di == 0 ? 0 : di == 1 ? f.file.tell() : py::os::path::getsize(f.name);
            if (base + off < 0) throw py::OSError("[Errno 22] Invalid argument");
            f.file.seek(off, di);
            int64_t pos = f.file.tell();
            return ok(pos & 0xFFFF, py::shr(pos, 16));
        }
        if (n == 17) {   // break(address)
            brk = bx;
            return ok(0);
        }
        if (n == 18 || n == 28) {   // stat(name, buf) / fstat(fd, buf)
            py::str p;
            if (n == 18) {
                p = host_bytes(cstr(bx), false);
                if (!py::os::path::exists(p)) return fail(ENOENT_);
            } else {
                auto it = fds.find(bx);
                if (it == fds.end()) return fail(EBADF_);
                Fd& f = *it->second;
                if (f.kind != Kind::File) {
                    std::string out;
                    int64_t words[7] = {1, 1, 020622, 1, 0, 0, 0};
                    for (int64_t v : words) pack_H(out, v);
                    out += std::string(16, '\0');
                    mu.mem_write(ds * 16 + cx, py::bytes(out));
                    return ok(0);
                }
                p = f.name;
            }
            mu.mem_write(ds * 16 + cx, stat_buf(p));
            return ok(0);
        }
        if (n == 33) {   // access
            if (py::os::path::exists(host_bytes(cstr(bx), false))) return ok(0);
            return fail(ENOENT_);
        }
        if (n == 20) return ok(86);   // getpid
        if (n == 13) {                // time
            int64_t t = py::int_from_float(py::time::time());
            return ok(py::shr(t, 16), t & 0xFFFF);
        }
        if (n == 48) return ok(0);              // signal
        if (n == 54) return fail(ENOTTY_);      // ioctl: nothing is a terminal
        if (n == 24 || n == 47) return ok(0);   // getuid, getgid
        if (n == 60) return ok(022);            // umask
        if (n == 15) return ok(0);              // chmod
        res.error = py::percent("system call %d (bx=%04x cx=%04x si=%04x di=%04x) is not provided", n, bx, cx, si, di);
        mu.emu_stop();
    };

    // the hooks (kept alive for the run)
    std::vector<std::unique_ptr<OutCallback>> out_callbacks;
    std::vector<std::unique_ptr<CodeCallback>> code_callbacks;
    {
        out_callbacks.push_back(std::make_unique<OutCallback>());
        out_callbacks.back()->fn = syscall;
        uc_hook hh;
        UcMachine::check(lib.hook_add(uc, &hh, UC_HOOK_INSN, reinterpret_cast<void*>(out_trampoline), out_callbacks.back().get(), 1, 0, UC_X86_INS_OUT));
    }
    auto add_code_hook = [&](std::function<void(uint64_t, uint32_t)> fn, int64_t begin, int64_t end) {
        code_callbacks.push_back(std::make_unique<CodeCallback>());
        code_callbacks.back()->fn = std::move(fn);
        uc_hook hh;
        UcMachine::check(lib.hook_add(uc, &hh, UC_HOOK_CODE, reinterpret_cast<void*>(code_trampoline), code_callbacks.back().get(),
                                      static_cast<uint64_t>(begin), static_cast<uint64_t>(end)));
    };
    if (text.slice(0, 4) == py::bytes("\xeb\x7e\xeb\xfe", 4)) {
        // XENIX 2.3's other way in: the program's first bytes are a jump over a row of `jmp .`,
        // and a system call is a far call to the one at 0002 (the number in AX as before); a far
        // call to the one at 0004 asks for the stack to be grown down to BX. The system catches
        // both there; so does this, and returns as the far call expects.
        add_code_hook(
            [&](uint64_t addr, uint32_t) {
                int64_t which = static_cast<int64_t>(addr) - cs * 16;
                if (which == 2) {
                    syscall(0x8800, 2, static_cast<uint32_t>(R(UC_X86_REG_AX)));
                } else {
                    mu.reg_write(UC_X86_REG_AX, R(UC_X86_REG_BX));
                    mu.reg_write(UC_X86_REG_EFLAGS, R(UC_X86_REG_EFLAGS) & ~static_cast<int64_t>(1));
                }
                if (res.status || (res.error && py::truthy(*res.error))) return;
                int64_t sp_ = R(UC_X86_REG_SP);
                py::bytes frame = mu.mem_read(ds * 16 + sp_, 4);
                int64_t ip = frame.byte_(0) | (frame.byte_(1) << 8);
                int64_t seg = frame.byte_(2) | (frame.byte_(3) << 8);
                mu.reg_write(UC_X86_REG_SP, (sp_ + 4) & 0xFFFF);
                mu.reg_write(UC_X86_REG_CS, seg);
                mu.reg_write(UC_X86_REG_IP, ip);
            },
            cs * 16 + 2, cs * 16 + 5);
    }
    if (py::truthy(hooks)) {
        // (to watch a program at work: {an address of its text: a function called, with the machine and the two
        // segments, each time the instruction there is about to be done})
        for (const auto& [at, fn] : hooks.items()) {
            Hook f = fn;
            add_code_hook([f, &mu, cs, ds](uint64_t, uint32_t) { f(mu, cs, ds); }, cs * 16 + at, cs * 16 + at);
        }
    }
    try {
        uc_err e = lib.emu_start(uc, static_cast<uint64_t>(cs * 16 + entry), 0xFFFFF, 0, static_cast<size_t>(limit));
        if (e != UC_ERR_OK) throw UcError(lib.strerror(e));
        if (hook_exception) {   // (the binding re-raises what a callback raised, once the run returns)
            std::exception_ptr p = hook_exception;
            hook_exception = nullptr;
            std::rethrow_exception(p);
        }
    } catch (const UcError& e) {
        res.error = py::percent("%s at %04x:%04x", py::str(std::string(e.what())), R(UC_X86_REG_CS), R(UC_X86_REG_IP));
    }
    for (auto& kv : fds) {
        if (kv.second->kind == Kind::File) kv.second->file.close();
    }
    if (!res.status && !res.error) {
        res.error = py::percent("did not finish (instruction limit, or it stopped at %04x)", R(UC_X86_REG_IP));
    }
    res.console = py::bytes(console);
    return res;
}

void main(const py::list<py::str>& argv, std::ostream& out) {
    py::list<py::str> a;
    py::list<py::str> rest = argv.slice(1, py::len(argv));
    for (const py::str& x : rest) {
        if (x != "--trace") a.append(x);
    }
    bool trace = argv.contains(py::str("--trace"));
    py::list<Arg> run_args;
    for (const py::str& x : a.slice(3, py::len(a))) run_args.append(Arg(x));
    Result r = run(a[0], a[1], a[2], run_args, py::bytes(), trace);
    out << r.console.decode("latin1").utf8();
    py::list<Call> last = r.calls.slice(-40, py::len(r.calls));
    for (const Call& c : last) {
        out << py::percent("  call %d bx=%04x cx=%04x si=%04x di=%04x", std::get<0>(c), std::get<1>(c), std::get<2>(c), std::get<3>(c),
                           std::get<4>(c))
                   .utf8()
            << "\n";
    }
    py::str status = r.status ? py::to_str(*r.status) : py::str("None");
    out << py::percent("[exit status %s%s]", status, r.error && py::truthy(*r.error) ? py::str("; ") + *r.error : py::str("")).utf8() << "\n";
}

}  // namespace codebreaker::exact::emu::xenix
