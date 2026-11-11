#include "cb/exact/emu/xenix.hpp"

#include <algorithm>
#include <chrono>
#include <exception>
#include <format>
#include <memory>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/text.hpp"
#include "cb/core/unicorn_api.hpp"

namespace cb::exact::emu::xenix {

namespace {

namespace stdfs = std::filesystem;
namespace uc = ::cb::emu::unicorn;

// The system's error numbers (Version 7).
constexpr std::uint16_t kEnoent = 2;
constexpr std::uint16_t kEbadf = 9;
constexpr std::uint16_t kEmfile = 24;
constexpr std::uint16_t kEinval = 22;
constexpr std::uint16_t kEnotty = 25;

constexpr std::uint16_t kSystemCallPort = 0x8800;
constexpr std::uint32_t kMemorySize = 0x100000;   // the 8086's megabyte
constexpr std::uint16_t kStackTop = 0xFF00;       // where the argument block ends, in the data segment
constexpr int kFirstFreeFd = 3;
constexpr int kFdLimit = 64;

// a.out magic numbers: 0107 and 0108 keep text and data in one segment, 0109 gives the data a segment of its own
constexpr std::uint16_t kMagicJoined = 0x0107;
constexpr std::uint16_t kMagicJoinedRo = 0x0108;
constexpr std::uint16_t kMagicSplit = 0x0109;
constexpr std::size_t kHeaderSize = 16;

// The first bytes of a program of XENIX 2.3: a jump over a row of `jmp .`
constexpr std::uint8_t kGatePrefix[4] = {0xEB, 0x7E, 0xEB, 0xFE};
constexpr std::uint32_t kGateSystemCall = 2;   // the far call to cs:0002 is a system call
constexpr std::uint32_t kGateLastByte = 5;     // (and the one to cs:0004 grows the stack)

// The system calls this provides, by number.
enum Call : std::uint16_t {
    kExit = 1, kRead = 3, kWrite = 4, kOpen = 5, kClose = 6, kCreat = 8, kUnlink = 10, kTime = 13, kChmod = 15, kBreak = 17, kStat = 18,
    kLseek = 19, kGetpid = 20, kGetuid = 24, kAccess = 33, kFstat = 28, kGetgid = 47, kSignal = 48, kIoctl = 54, kUmask = 60
};

int unicorn_register(Reg r) {
    switch (r) {
        case Reg::Ax: return UC_X86_REG_AX;
        case Reg::Bx: return UC_X86_REG_BX;
        case Reg::Cx: return UC_X86_REG_CX;
        case Reg::Dx: return UC_X86_REG_DX;
        case Reg::Si: return UC_X86_REG_SI;
        case Reg::Di: return UC_X86_REG_DI;
        case Reg::Bp: return UC_X86_REG_BP;
        case Reg::Sp: return UC_X86_REG_SP;
        case Reg::Ip: return UC_X86_REG_IP;
        case Reg::Cs: return UC_X86_REG_CS;
        case Reg::Ds: return UC_X86_REG_DS;
        case Reg::Es: return UC_X86_REG_ES;
        case Reg::Ss: return UC_X86_REG_SS;
        case Reg::Flags: break;
    }
    return UC_X86_REG_EFLAGS;
}

// A Unicorn engine for one run, as a Machine. Single-threaded, like every emulator instance.
class Engine : public Machine {
  public:
    Engine() { uc::check(uc::api().uc_open(UC_ARCH_X86, UC_MODE_16, &engine_), "uc_open"); }
    ~Engine() override {
        if (engine_ != nullptr) uc::api().uc_close(engine_);
    }
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    uc_engine* get() const { return engine_; }

    void map(std::uint32_t address, std::uint32_t size) { uc::check(uc::api().uc_mem_map(engine_, address, size, UC_PROT_ALL), "uc_mem_map"); }

    std::uint32_t reg(Reg r) const override {
        std::uint64_t value = 0;
        uc::check(uc::api().uc_reg_read(engine_, unicorn_register(r), &value), "uc_reg_read");
        return static_cast<std::uint32_t>(value);
    }
    void set_reg(Reg r, std::uint32_t value) override {
        std::uint64_t v = value;
        uc::check(uc::api().uc_reg_write(engine_, unicorn_register(r), &v), "uc_reg_write");
    }
    Bytes read(std::uint32_t address, std::size_t size) const override {
        Bytes out(size);
        uc::check(uc::api().uc_mem_read(engine_, address, out.data(), size), "uc_mem_read");
        return out;
    }
    void write(std::uint32_t address, ByteView data) override {
        if (data.empty()) return;
        uc::check(uc::api().uc_mem_write(engine_, address, data.data(), data.size()), "uc_mem_write");
    }
    void stop() override { uc::check(uc::api().uc_emu_stop(engine_), "uc_emu_stop"); }

  private:
    uc_engine* engine_ = nullptr;
};

// A file descriptor: the console and stdin, or a file of the host kept in memory.
struct Fd {
    enum class Kind { Input, Output, File };
    Kind kind = Kind::Output;
    std::size_t input_position = 0;   // Input: how much of stdin was read
    stdfs::path host;                 // File: where it is
    Bytes data;                       // File: its contents
    std::uint64_t position = 0;
    bool writable = false;
    bool dirty = false;
};

// Windows gives these names to devices wherever they appear; a program must not reach them through a file name.
bool is_device_name(std::string_view component) {
    const std::string stem = text::upper(component.substr(0, component.find('.')));
    if (stem == "CON" || stem == "PRN" || stem == "AUX" || stem == "NUL" || stem == "CONIN$" || stem == "CONOUT$") return true;
    return stem.size() == 4 && (stem.starts_with("COM") || stem.starts_with("LPT")) && stem[3] >= '1' && stem[3] <= '9';
}

// The big-endian-word pair of a V7 `long` ("high word first", each little-endian), as struct stat lays out its sizes
// and times.
void put_word(Bytes& out, std::uint32_t v) {
    out.push_back(static_cast<std::uint8_t>(v));
    out.push_back(static_cast<std::uint8_t>(v >> 8));
}
void put_long(Bytes& out, std::uint64_t v) {
    put_word(out, static_cast<std::uint32_t>((v >> 16) & 0xFFFF));
    put_word(out, static_cast<std::uint32_t>(v & 0xFFFF));
}

// The path of a file as the host has it: in `work` first, then in `root`. nullopt if the name reaches outside both.
std::optional<stdfs::path> host_path(const Spec& spec, const std::string& name, bool create) {
    if (name.find_first_of(":\\") != std::string::npos) return std::nullopt;
    std::vector<std::string> parts;
    for (const std::string_view part : text::split(name, '/')) {
        if (part.empty()) continue;
        std::string p = text::replace_all(part, "..", "_");
        if (is_device_name(p) || p.ends_with('.') || p.ends_with(' ')) return std::nullopt;
        parts.push_back(std::move(p));
    }
    const auto join = [&](const stdfs::path& base) {
        stdfs::path p = base;
        for (const std::string& part : parts) p /= fs::from_text(part);
        return p;
    };
    const auto exists = [](const stdfs::path& p) {
        std::error_code ec;
        return stdfs::exists(p, ec);
    };
    const stdfs::path in_work = join(spec.work);
    if (create || exists(in_work)) return in_work;
    const stdfs::path in_root = join(spec.root);
    return exists(in_root) ? in_root : in_work;
}

// The state of one run: the machine, the files, the result so far.
class Session {
  public:
    Session(const Spec& spec, Engine& machine, std::uint16_t data_segment) : spec_(spec), m_(machine), ds_(data_segment) {
        fds_[0].kind = Fd::Kind::Input;
        fds_[1].kind = Fd::Kind::Output;
        fds_[2].kind = Fd::Kind::Output;
    }

    Result& result() { return result_; }
    // an exception thrown by a hook or a system call: the engine is stopped, and it is thrown again when the run returns
    std::exception_ptr& pending_exception() { return pending_; }

    // ---- the system
    // The out instruction at the system-call port (or the gate's far call): the call's number is `value`.
    void system_call(std::uint16_t number) {
        const std::uint16_t bx = reg16(Reg::Bx), cx = reg16(Reg::Cx), si = reg16(Reg::Si), di = reg16(Reg::Di);
        if (spec_.trace) result_.calls.push_back({number, bx, cx, si, di});
        switch (number) {
            case kExit:
                result_.status = bx;
                m_.stop();
                return;
            case kRead: return read_call(bx, cx, si);
            case kWrite: return write_call(bx, cx, si);
            case kOpen: return open_call(bx, cx);
            case kCreat: return creat_call(bx);
            case kClose: return close_call(bx);
            case kUnlink: return unlink_call(bx);
            case kLseek: return lseek_call(bx, cx, si, di);
            case kBreak: return succeed(0);   // (the break is not kept: nothing here allocates)
            case kStat:
            case kFstat: return stat_call(number, bx, cx);
            case kAccess: return access_call(bx);
            case kGetpid: return succeed(86);
            case kTime: {
                const auto t = static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count());
                return succeed(static_cast<std::uint16_t>(t >> 16), static_cast<std::uint16_t>(t & 0xFFFF));
            }
            case kSignal: return succeed(0);
            case kIoctl: return fail(kEnotty);   // nothing is a terminal
            case kGetuid:
            case kGetgid: return succeed(0);
            case kUmask: return succeed(022);
            case kChmod: return succeed(0);
            default: break;
        }
        result_.error = std::format("system call {} (bx={:04x} cx={:04x} si={:04x} di={:04x}) is not provided", number, bx, cx, si, di);
        m_.stop();
    }

    // The far call of XENIX 2.3's gate, caught at the first instruction of the jmp it lands on: 0002 is a system call,
    // 0004 asks for the stack to be grown down to BX. Returns as the far call expects.
    void gate_call(std::uint32_t offset) {
        if (offset == kGateSystemCall) {
            system_call(static_cast<std::uint16_t>(reg(Reg::Ax)));
        } else {
            m_.set_reg(Reg::Ax, reg(Reg::Bx));
            m_.set_reg(Reg::Flags, reg(Reg::Flags) & ~1u);
        }
        if (result_.status || result_.error) return;
        const std::uint32_t sp = reg(Reg::Sp);
        const Bytes frame = m_.read(ds_ * 16u + sp, 4);   // the return address: offset, then segment
        m_.set_reg(Reg::Sp, (sp + 4) & 0xFFFF);
        m_.set_reg(Reg::Cs, static_cast<std::uint32_t>(frame[2] | (frame[3] << 8)));
        m_.set_reg(Reg::Ip, static_cast<std::uint32_t>(frame[0] | (frame[1] << 8)));
    }

    // Write back every file that has changed.
    void flush_all() {
        for (auto& [number, fd] : fds_) flush(fd);
    }

  private:
    std::uint32_t reg(Reg r) const { return m_.reg(r); }
    std::uint16_t reg16(Reg r) const { return static_cast<std::uint16_t>(m_.reg(r)); }

    // The result of a call: AX (and BX) with the carry clear, or an error number in AX with the carry set.
    void succeed(std::uint16_t ax, std::optional<std::uint16_t> bx = std::nullopt) {
        const std::uint32_t flags = reg(Reg::Flags);
        m_.set_reg(Reg::Ax, ax);
        m_.set_reg(Reg::Flags, flags & ~1u);
        if (bx) m_.set_reg(Reg::Bx, *bx);
    }
    void fail(std::uint16_t error) {
        const std::uint32_t flags = reg(Reg::Flags);
        m_.set_reg(Reg::Ax, error);
        m_.set_reg(Reg::Flags, flags | 1u);
    }

    // a C string in the data segment
    std::string c_string(std::uint16_t offset) const {
        std::string s;
        while (true) {
            const Bytes chunk = m_.read(ds_ * 16u + offset + static_cast<std::uint32_t>(s.size()), 64);
            const auto nul = std::find(chunk.begin(), chunk.end(), std::uint8_t{0});
            s.append(chunk.begin(), nul);
            if (nul != chunk.end()) return s;
        }
    }

    static void flush(Fd& fd) {
        if (fd.kind == Fd::Kind::File && fd.dirty) {
            fs::write_file(fd.host, fd.data);
            fd.dirty = false;
        }
    }

    Fd* find(std::uint16_t number) {
        const auto it = fds_.find(number);
        return it == fds_.end() ? nullptr : &it->second;
    }

    // the lowest free descriptor from 3, or -1
    int free_fd() const {
        for (int n = kFirstFreeFd; n < kFdLimit; ++n) {
            if (!fds_.contains(static_cast<std::uint16_t>(n))) return n;
        }
        return -1;
    }

    static bool is_file(const stdfs::path& p) {
        std::error_code ec;
        return stdfs::is_regular_file(p, ec);
    }

    void read_call(std::uint16_t fd_number, std::uint16_t buffer, std::uint16_t count) {   // read(fd, buf, count)
        Fd* fd = find(fd_number);
        if (fd == nullptr) return fail(kEbadf);
        Bytes chunk;
        if (fd->kind == Fd::Kind::Input) {
            const std::size_t from = std::min(fd->input_position, spec_.stdin_data.size());
            const std::size_t to = std::min<std::size_t>(from + count, spec_.stdin_data.size());
            chunk.assign(spec_.stdin_data.begin() + static_cast<std::ptrdiff_t>(from), spec_.stdin_data.begin() + static_cast<std::ptrdiff_t>(to));
            fd->input_position += chunk.size();
        } else if (fd->kind == Fd::Kind::File) {
            const std::size_t from = std::min<std::size_t>(fd->position, fd->data.size());
            const std::size_t to = std::min<std::size_t>(fd->position + count, fd->data.size());
            chunk.assign(fd->data.begin() + static_cast<std::ptrdiff_t>(from), fd->data.begin() + static_cast<std::ptrdiff_t>(to));
            fd->position += chunk.size();
        } else {
            return fail(kEbadf);
        }
        m_.write(ds_ * 16u + buffer, chunk);
        succeed(static_cast<std::uint16_t>(chunk.size()));
    }

    void write_call(std::uint16_t fd_number, std::uint16_t buffer, std::uint16_t count) {   // write(fd, buf, count)
        Fd* fd = find(fd_number);
        const Bytes chunk = m_.read(ds_ * 16u + buffer, count);   // (read first: a buffer that is not there is a fault whatever the fd)
        if (fd == nullptr) return fail(kEbadf);
        if (fd->kind == Fd::Kind::Output) {
            append(result_.console, chunk);
        } else if (fd->kind == Fd::Kind::File) {
            if (!fd->writable) return fail(kEbadf);
            if (fd->position + chunk.size() > fd->data.size()) fd->data.resize(fd->position + chunk.size());   // a gap reads as zeros
            std::copy(chunk.begin(), chunk.end(), fd->data.begin() + static_cast<std::ptrdiff_t>(fd->position));
            fd->position += chunk.size();
            fd->dirty = true;
        }
        succeed(static_cast<std::uint16_t>(chunk.size()));
    }

    void open_call(std::uint16_t name, std::uint16_t mode) {   // open(name, mode): 0 reads, anything else reads and writes
        flush_all();
        const auto host = host_path(spec_, c_string(name), false);
        if (!host || !is_file(*host)) return fail(kEnoent);
        const int number = free_fd();
        if (number < 0) return fail(kEmfile);
        Fd fd;
        fd.kind = Fd::Kind::File;
        fd.host = *host;
        fd.data = fs::read_file(*host);
        fd.writable = mode != 0;
        fds_[static_cast<std::uint16_t>(number)] = std::move(fd);
        succeed(static_cast<std::uint16_t>(number));
    }

    void creat_call(std::uint16_t name) {   // creat(name, mode): the file is made empty, and its folders
        flush_all();
        const auto host = host_path(spec_, c_string(name), true);
        if (!host) return fail(kEnoent);
        const int number = free_fd();
        if (number < 0) return fail(kEmfile);
        fs::write_file(*host, {});
        Fd fd;
        fd.kind = Fd::Kind::File;
        fd.host = *host;
        fd.writable = true;
        fds_[static_cast<std::uint16_t>(number)] = std::move(fd);
        succeed(static_cast<std::uint16_t>(number));
    }

    void close_call(std::uint16_t fd_number) {   // close(fd)
        const auto it = fds_.find(fd_number);
        if (it == fds_.end()) return fail(kEbadf);
        flush(it->second);
        fds_.erase(it);
        succeed(0);
    }

    // unlink(name): only a file of the work folder is removed; one that the system has is "removed" without being touched
    void unlink_call(std::uint16_t name) {
        flush_all();
        const auto host = host_path(spec_, c_string(name), false);
        std::error_code ec;
        if (host && is_file(*host) && same_folder(*host, spec_.work)) {
            stdfs::remove(*host, ec);
            return succeed(0);
        }
        if (host && stdfs::exists(*host, ec)) return succeed(0);
        fail(kEnoent);
    }

    static bool same_folder(const stdfs::path& file, const stdfs::path& folder) {
        std::error_code ec;
        const stdfs::path f = stdfs::weakly_canonical(file, ec);
        const stdfs::path d = stdfs::weakly_canonical(folder, ec);
        const std::string a = text::lower(fs::to_text(f));
        const std::string b = text::lower(fs::to_text(d));
        return a == b || a.starts_with(b + "/");
    }

    // lseek(fd, long offset (high word in CX, low in SI), whence)
    void lseek_call(std::uint16_t fd_number, std::uint16_t high, std::uint16_t low, std::uint16_t whence) {
        Fd* fd = find(fd_number);
        if (fd == nullptr || fd->kind != Fd::Kind::File) return fail(kEbadf);
        const std::int64_t offset = static_cast<std::int32_t>((static_cast<std::uint32_t>(high) << 16) | low);
        std::int64_t base = 0;
        if (whence == 1) base = static_cast<std::int64_t>(fd->position);
        else if (whence == 2) base = static_cast<std::int64_t>(fd->data.size());
        else if (whence != 0) return fail(kEinval);
        if (base + offset < 0) return fail(kEinval);
        fd->position = static_cast<std::uint64_t>(base + offset);
        succeed(static_cast<std::uint16_t>(fd->position & 0xFFFF), static_cast<std::uint16_t>(fd->position >> 16));
    }

    // stat(name, buf) / fstat(fd, buf): the V7 struct stat: dev, ino, mode, nlink, uid, gid, rdev (7 words), size,
    // atime, mtime, ctime (longs; the three times are the modification time)
    void stat_call(std::uint16_t number, std::uint16_t first, std::uint16_t buffer) {
        stdfs::path host;
        if (number == kStat) {
            flush_all();
            const auto p = host_path(spec_, c_string(first), false);
            std::error_code ec;
            if (!p || !stdfs::exists(*p, ec)) return fail(kEnoent);
            host = *p;
        } else {
            Fd* fd = find(first);
            if (fd == nullptr) return fail(kEbadf);
            if (fd->kind != Fd::Kind::File) {   // a terminal
                Bytes out;
                for (const std::uint32_t w : {1u, 1u, 020622u, 1u, 0u, 0u, 0u}) put_word(out, w);
                out.resize(out.size() + 16);
                m_.write(ds_ * 16u + buffer, out);
                return succeed(0);
            }
            flush(*fd);
            host = fd->host;
        }
        std::error_code ec;
        const bool directory = stdfs::is_directory(host, ec);
        const std::uint64_t size = directory ? 0 : stdfs::file_size(host, ec);
        const auto written = std::chrono::clock_cast<std::chrono::system_clock>(stdfs::last_write_time(host, ec));
        const auto seconds = static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::seconds>(written.time_since_epoch()).count());
        Bytes out;
        for (const std::uint32_t w : {1u, 2u, (directory ? 040000u : 0100000u) | 0644u, 1u, 0u, 0u, 0u}) put_word(out, w);
        put_long(out, size);
        for (int i = 0; i < 3; ++i) put_long(out, seconds);
        m_.write(ds_ * 16u + buffer, out);
        succeed(0);
    }

    void access_call(std::uint16_t name) {   // access(name, mode): does it exist?
        flush_all();
        const auto host = host_path(spec_, c_string(name), false);
        std::error_code ec;
        if (host && stdfs::exists(*host, ec)) return succeed(0);
        fail(kEnoent);
    }

    const Spec& spec_;
    Engine& m_;
    std::uint16_t ds_;
    std::map<std::uint16_t, Fd> fds_;   // by number: looked up, and flushed in any order
    Result result_;
    std::exception_ptr pending_;
};

// ---- what Unicorn calls ---------------------------------------------------------------------------------

// A hook for the machine to call: what to do, and where an exception it throws goes.
struct Callback {
    Session* session = nullptr;
    Engine* machine = nullptr;
    std::function<void(std::uint64_t)> action;   // called with the address of the instruction about to be done
};

// The out instruction: only the system-call port is ours
struct OutCallback {
    Session* session = nullptr;
    Engine* machine = nullptr;
};

void stash(Session& session, Engine& machine) {
    if (!session.pending_exception()) session.pending_exception() = std::current_exception();
    uc::api().uc_emu_stop(machine.get());
}

void out_trampoline(uc_engine*, std::uint32_t port, int, std::uint32_t value, void* user) {
    auto* c = static_cast<OutCallback*>(user);
    if (port != kSystemCallPort) return;
    try {
        c->session->system_call(static_cast<std::uint16_t>(value & 0xFFFF));
    } catch (...) {
        stash(*c->session, *c->machine);
    }
}

void code_trampoline(uc_engine*, std::uint64_t address, std::uint32_t, void* user) {
    auto* c = static_cast<Callback*>(user);
    try {
        c->action(address);
    } catch (...) {
        stash(*c->session, *c->machine);
    }
}

}  // namespace

Result run(const Spec& spec) {
    // the program: its header, then its text and data
    const auto program_path = host_path(spec, spec.program, false);
    if (!program_path) throw NotFoundError(fs::from_text(spec.program), 2, "not a name inside the XENIX system");
    const Bytes file = fs::read_file(*program_path);
    io::ByteReader header(file, "a.out");
    const std::uint16_t magic = header.u16();
    const std::size_t text_size = header.u16();
    const std::size_t data_size = header.u16();
    header.skip(2 * 2);   // bss size, symbol table size
    const std::uint16_t entry = header.u16();
    if (magic != kMagicJoined && magic != kMagicJoinedRo && magic != kMagicSplit) {
        throw FormatError("a.out", 0, std::format("{} is not an a.out file for the 8086 ({:04x})", spec.program, magic));
    }
    const auto text_range = io::try_range(file, kHeaderSize, text_size);
    const auto data_range = io::try_range(file, kHeaderSize + text_size, data_size);
    if (!text_range || !data_range) throw TruncatedError("a.out", kHeaderSize, text_size + data_size, file.size() - kHeaderSize);
    const ByteView text_bytes = *text_range;
    const ByteView data_bytes = *data_range;

    Engine machine;
    machine.map(0, kMemorySize);
    const bool split = magic == kMagicSplit;
    const std::uint16_t cs = kCodeSegment;
    const std::uint16_t ds = split ? kDataSegment : kCodeSegment;
    machine.write(cs * 16u, text_bytes);
    machine.write(ds * 16u + (split ? 0 : static_cast<std::uint32_t>(text_size)), data_bytes);

    // the stack: argc, argv[], 0, envp[], 0, the strings - at the top of the data segment
    std::vector<Bytes> argv;
    argv.push_back(to_bytes(spec.program));
    for (const std::string& a : spec.args) argv.push_back(to_bytes(a));
    Bytes strings;
    std::vector<std::size_t> pointers;
    for (const Bytes& a : argv) {
        pointers.push_back(strings.size());
        append(strings, a);
        strings.push_back(0);
    }
    const std::size_t vector_size = 2 * (argv.size() + 3);
    if (strings.size() + (strings.size() & 1) + vector_size > kStackTop) throw UsageError("the arguments do not fit in the program's stack");
    std::size_t sbase = kStackTop - strings.size() - (strings.size() & 1);
    machine.write(ds * 16u + static_cast<std::uint32_t>(sbase), strings);
    Bytes vec;
    put_word(vec, static_cast<std::uint32_t>(argv.size()));
    for (const std::size_t p : pointers) put_word(vec, static_cast<std::uint32_t>(sbase + p));
    put_word(vec, 0);
    put_word(vec, 0);
    std::size_t sp = sbase - vec.size();
    sp -= sp & 1;
    machine.write(ds * 16u + static_cast<std::uint32_t>(sp), vec);
    machine.set_reg(Reg::Cs, cs);
    machine.set_reg(Reg::Ds, ds);
    machine.set_reg(Reg::Es, ds);
    machine.set_reg(Reg::Ss, ds);
    machine.set_reg(Reg::Sp, static_cast<std::uint32_t>(sp));
    machine.set_reg(Reg::Bp, 0);

    Session session(spec, machine, ds);
    // the callbacks live as long as the run
    OutCallback out_callback{&session, &machine};
    std::vector<std::unique_ptr<Callback>> code_callbacks;
    uc_hook handle = 0;
    uc::check(uc::api().uc_hook_add(machine.get(), &handle, UC_HOOK_INSN, reinterpret_cast<void*>(out_trampoline), &out_callback, 1, 0, UC_X86_INS_OUT), "uc_hook_add");
    const auto add_code_hook = [&](std::uint64_t begin, std::uint64_t end, std::function<void(std::uint64_t)> action) {
        code_callbacks.push_back(std::make_unique<Callback>(Callback{&session, &machine, std::move(action)}));
        uc::check(uc::api().uc_hook_add(machine.get(), &handle, UC_HOOK_CODE, reinterpret_cast<void*>(code_trampoline), code_callbacks.back().get(), begin, end), "uc_hook_add");
    };
    if (text_bytes.size() >= 4 && std::equal(std::begin(kGatePrefix), std::end(kGatePrefix), text_bytes.begin())) {
        // XENIX 2.3's other way in: the program's first bytes are a jump over a row of `jmp .`, and a system call is a
        // far call to the one at 0002 (the number in AX as before); a far call to the one at 0004 asks for the stack to be
        // grown down to BX. The system catches both there; so does this.
        add_code_hook(cs * 16u + kGateSystemCall, cs * 16u + kGateLastByte,
                      [&](std::uint64_t address) { session.gate_call(static_cast<std::uint32_t>(address - cs * 16u)); });
    }
    for (const auto& entry_hook : spec.hooks) {
        const Hook& hook = entry_hook.second;
        add_code_hook(cs * 16u + entry_hook.first, cs * 16u + entry_hook.first, [&machine, &hook, cs, ds](std::uint64_t) { hook(machine, cs, ds); });
    }

    Result& result = session.result();
    const uc_err code = uc::api().uc_emu_start(machine.get(), cs * 16u + entry, 0xFFFFF, 0, spec.limit);
    const auto where = [&] { return std::format("{:04x}:{:04x}", machine.reg(Reg::Cs), machine.reg(Reg::Ip)); };
    try {
        if (session.pending_exception()) std::rethrow_exception(session.pending_exception());   // (what a hook or a call threw)
        if (code != UC_ERR_OK) result.error = std::format("{} at {}", uc::api().uc_strerror(code), where());
    } catch (const EmulationError& e) {
        result.error = std::format("{} at {}", e.what(), where());
    }
    session.flush_all();
    if (!result.status && !result.error) {
        result.error = std::format("did not finish (instruction limit, or it stopped at {:04x})", machine.reg(Reg::Ip));
    }
    return std::move(result);
}

}  // namespace cb::exact::emu::xenix
