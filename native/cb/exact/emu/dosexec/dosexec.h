// dosexec.h -- the executive's pieces, shared by its files.
//
// The port is line for line: each function names the Python it is (dos.py / dpmi.py), and keeps
// that code's order of reads and writes, so a run here does to the guest what the Python did.
#pragma once
#include <atomic>
#include <cstdint>
#include <deque>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <unicorn/unicorn.h>

namespace cbd {

using Bytes = std::vector<uint8_t>;
using BufP = std::shared_ptr<Bytes>;

// ---- Unicorn, by the DLL Python loaded (uc.cc) ----------------------------------------------
struct UcLib {
    decltype(&uc_open) open = nullptr;
    decltype(&uc_close) close = nullptr;
    decltype(&uc_reg_read) reg_read = nullptr;
    decltype(&uc_reg_write) reg_write = nullptr;
    decltype(&uc_mem_read) mem_read = nullptr;
    decltype(&uc_mem_write) mem_write = nullptr;
    decltype(&uc_mem_map) mem_map = nullptr;
    decltype(&uc_mem_map_ptr) mem_map_ptr = nullptr;
    decltype(&uc_emu_start) emu_start = nullptr;
    decltype(&uc_emu_stop) emu_stop = nullptr;
    decltype(&uc_hook_add) hook_add = nullptr;
    decltype(&uc_strerror) strerror = nullptr;
};
extern UcLib UC;
bool load_unicorn(const wchar_t *dll, char *err, size_t errlen);

// ---- what a Python exception would have been ----------------------------------------------------
struct PyErr {
    int kind;                 // CBD_* of cbdos.h
    int err_no = 0;
    std::string message;
    std::wstring filename;
};
PyErr runtime_error(const std::string &m);
PyErr uc_error(int e);
struct DosExit {};
struct ArenaError { int code; };

// ---- Python's text (pyfmt.cc) -------------------------------------------------------------------
std::string fmt(const char *f, ...);
std::string py_hex(int64_t v, int width, bool upper = false);   // '%0<width>x' % v (a sign counts in the width)
std::string bytes_repr(const uint8_t *b, size_t n);             // repr(bytes)
std::string hex_str(const uint8_t *b, size_t n);                // bytes.hex()
std::wstring py_upper(const std::wstring &s);                   // str.upper() of latin-1 text
bool py_isspace(wchar_t c);
bool py_isalpha(wchar_t c);
std::wstring rstrip_ws(const std::wstring &s);
std::wstring latin1(const uint8_t *b, size_t n);
std::wstring latin1(const Bytes &b);
bool to_latin1(const std::wstring &s, Bytes &out);             // false: a character past U+00FF
std::string utf8(const std::wstring &s);
Bytes py_slice(const Bytes &b, int64_t start, int64_t stop);    // b[start:stop]

// ---- paths and files (fs.cc) --------------------------------------------------------------------
std::wstring abspath(const std::wstring &p);       // os.path.abspath
std::wstring normcase(const std::wstring &p);      // os.path.normcase
std::wstring pkey(const std::wstring &p);          // MemFS.key: normcase(abspath(p))
std::wstring path_join(const std::wstring &a, const std::vector<std::wstring> &parts);   // os.path.join
std::wstring basename(const std::wstring &p);      // os.path.basename
std::wstring relpath(const std::wstring &path, const std::wstring &start);   // os.path.relpath (raises ValueError)
bool disk_isfile(const std::wstring &p);           // os.path.isfile
int64_t disk_getsize(const std::wstring &p);       // os.path.getsize
void disk_remove(const std::wstring &p);           // os.remove
void disk_replace(const std::wstring &a, const std::wstring &b);   // os.replace
Bytes disk_read(const std::wstring &p);            // ropen(p, "rb").read()

// A file the program has open: the operations the DOS calls use (MemFile / a Python file object).
struct File {
    virtual ~File() {}
    virtual Bytes read(int64_t n) = 0;
    virtual void write(const uint8_t *d, size_t n) = 0;
    virtual int64_t seek(int64_t off, int whence) = 0;
    virtual int64_t tell() = 0;
    virtual void truncate() = 0;                    // at the position
    virtual void truncate_to(int64_t size) = 0;
    virtual void flush() {}
    virtual void close() {}
};
using FileP = std::shared_ptr<File>;
FileP disk_open(const std::wstring &p, const char *mode);   // ropen(p, mode)

struct MemFS {                                       // dos.py MemFS
    std::unordered_map<std::wstring, BufP> files;
    std::set<std::wstring> gone;
    std::vector<std::wstring> hide;
    std::set<std::wstring> touched;                  // keys whose entry this run set or removed
    // handed back after the run
    std::vector<std::wstring> out_keys;
    std::vector<BufP> out_data;
    std::vector<std::wstring> out_gone;

    bool hidden(const std::wstring &k) const;
    Bytes disk(const std::wstring &p);
    bool isfile(const std::wstring &p);
    int64_t getsize(const std::wstring &p);
    Bytes read(const std::wstring &p);
    FileP open(const std::wstring &p, const char *mode);
    void put(const std::wstring &p, const Bytes &b);
    void remove(const std::wstring &p);
    void replace(const std::wstring &a, const std::wstring &b);
};

// ---- the run --------------------------------------------------------------------------------
struct Settings {
    double fast_limit = 20, dpmi_limit = 300;
    int drive = 2;
    bool debug = false;
};

struct Desc { uint64_t base, limit, access; };

struct Handle { std::wstring dev; bool has_dev = false; FileP f; std::wstring name; };

class Host;

struct Run {
    // what dos.run was given
    std::wstring exe, workdir;
    Bytes cmdline, stdin_data, env;
    int64_t max_blocks = 60000000;
    bool trace_calls = false, fast = false, want_dpmi = false;
    int PSP = 0x100, TOP = 0xA000;
    MemFS *fs = nullptr;
    Settings S;

    // the machine
    uc_engine *uc = nullptr;
    std::vector<uint8_t> ram;
    int load = 0;
    int first_mcb = 0;
    std::unique_ptr<Host> host;

    // st
    Bytes console;
    std::deque<uint8_t> in;
    uint64_t dta = 0;
    std::map<int, std::pair<uint64_t, uint64_t>> vec;
    std::optional<int64_t> exit;
    int64_t child = 0;
    uint64_t cur_psp = 0;
    int64_t strategy = 0;
    bool pm = false;
    std::unordered_map<uint64_t, Desc> desc;
    std::optional<uint64_t> ov_ds, ov_es, out_ds, out_es;
    bool again = false;
    std::atomic<bool> timedout{false};
    std::map<int64_t, Handle> files;
    int64_t nexth = 5;
    std::map<uint64_t, FileP> fcbs;
    std::unordered_set<uint64_t> blocks;
    int64_t n = 0;
    std::map<uint64_t, int64_t> calls;
    std::vector<PyErr> err;

    ~Run();
    void go();                                   // dos.run's body (raises PyErr)

    // registers and memory, as dos.py's R / W / mu.mem_read / mu.mem_write
    uint64_t R(int r);
    void W(int r, uint64_t v);                   // & 0xFFFF
    void set_reg(int r, uint64_t v);             // mu.reg_write
    Bytes rd(uint64_t a, size_t n);
    uint8_t rd1(uint64_t a);
    uint16_t rd16(uint64_t a);
    void wr(uint64_t a, const uint8_t *d, size_t n);
    void wr(uint64_t a, const Bytes &d) { wr(a, d.data(), d.size()); }
    void stop();
    void carry(bool on, std::optional<uint64_t> ax = std::nullopt);

    uint64_t lin(uint64_t seg, uint64_t off);
    std::wstring cstr(uint64_t seg, uint64_t off);
    std::wstring hostpath(const std::wstring &name);
    uint64_t pc();
    int64_t newh();

    // the file system calls, the MemFS or the disk
    FileP fsopen(const std::wstring &p, const char *mode);
    bool fsisfile(const std::wstring &p);
    int64_t fssize(const std::wstring &p);
    void fsremove(const std::wstring &p);
    void fsreplace(const std::wstring &a, const std::wstring &b);

    void int21();
    void int21_fcb(int ah, int al, uint64_t cx, uint64_t ds, uint64_t dx);
    void dos_int(int intno);
    void intr(int intno);
};

// the memory arena (mcb.cc)
void mcb_write(Run &r, int64_t seg, uint8_t sig, int64_t owner, int64_t size);
struct Mcb { int64_t seg; uint8_t sig; int64_t owner, size; };
std::vector<Mcb> mcb_chain(Run &r, int64_t first);
int64_t mcb_largest(Run &r, int64_t first);
std::optional<int64_t> mcb_alloc(Run &r, int64_t first, int64_t paras, int64_t owner, int64_t strategy = 0);
void mcb_free(Run &r, int64_t first, int64_t block);
std::optional<int64_t> mcb_resize(Run &r, int64_t first, int64_t block, int64_t paras);
constexpr int ENVSEG = 0x00C0, DOSSEG = 0x0080, LOL_OFF = 0x0026, LOL_SIZE = 0x40;
constexpr int MCB_FREE = 0, MCB_DOS = 8;

// the DPMI host (dpmi.cc)
class Host {
public:
    explicit Host(Run &ctx) : ctx(ctx) {}
    void attach();
    bool handle(int intno);
private:
    Run &ctx;
    bool pm = false;
    std::map<int64_t, std::tuple<uint64_t, uint64_t, uint64_t>> ldt_used;
    int64_t next_ldt = 16;
    std::map<int64_t, std::pair<uint64_t, uint64_t>> mem;
    uint64_t pool = 0x00200000;
    int64_t next_handle = 1;
    std::optional<uint64_t> scratch_ds, scratch_es;
    std::map<int, std::pair<uint64_t, uint64_t>> pm_vec, exc_vec;
    bool client32 = false;
    int64_t vif = 1;
    std::map<uint64_t, uint64_t> segsel;

    void int2f_1687();
    void switch_();
    void seg(uint64_t sel, uint64_t &base, uint64_t &limit, uint64_t &access, uint64_t &flags);
    void push(const std::vector<uint64_t> &values);
    void dispatch(int intno);
    void default_int();
    void write_vector_stubs();
    void write_raw_code();
    void raw_switch();
    uint64_t make_ldt(uint64_t base, uint64_t limit, uint64_t access, std::optional<int64_t> index = std::nullopt);
    int64_t ldt_index(uint64_t sel) { return int64_t((sel & 0xFFF8) >> 3); }
    void int31();
    std::tuple<uint64_t, uint64_t, uint64_t> get(uint64_t sel);
    void set(uint64_t sel, std::optional<uint64_t> base, std::optional<uint64_t> limit, std::optional<uint64_t> access);
    uint64_t lin_esedi();
    std::optional<uint64_t> pool_alloc(uint64_t size);
    void mem_info();
    void vectors(int fn, uint64_t bx, uint64_t cx, uint64_t dx);
    void simulate_rm_int();
};

}  // namespace cbd
