// codebreaker/exact/emu/dos.cpp -- C++ port of codebreaker.exact.emu.dos (see dos.hpp).

#include "codebreaker/exact/emu/dos.hpp"

#include <algorithm>
#include <cstdlib>
#include <filesystem>

#include "cb/compat/py_bridge.hpp"
#include "cb/exact/emu/dos.hpp"
#include "codebreaker/exact/emu/native_dosexec.hpp"

namespace codebreaker::exact::emu::dos {

const double FAST_LIMIT = std::strtod(py::os::getenv(py::str("DOSRUN_FAST_LIMIT"), py::str("20")).utf8().c_str(), nullptr);
const int64_t DEFAULT_MAX_BLOCKS = py::int_parse(py::os::getenv(py::str("DOSRUN_MAX_BLOCKS"), py::str("60000000")), 10);

namespace {

// a bytes of `n` zero bytes
py::bytes zeros(int64_t n) { return py::bytes::zeros(n); }

}  // namespace

// ---- the DOS memory arena ----------------------------------------------------------------------
// The arena is cb::exact::emu::dos's: these convert and forward (the old Engine's memory as the new Engine).
namespace {

namespace nd = cb::exact::emu::dos;

class MemoryView final : public cb::exact::emu::dpmi::Engine {
  public:
    explicit MemoryView(dpmi::Engine& old) : old_(old) {}
    std::uint32_t reg(cb::exact::emu::dpmi::Reg) override { throw cb::Error("the arena reads no registers"); }
    void set_reg(cb::exact::emu::dpmi::Reg, std::uint32_t) override { throw cb::Error("the arena writes no registers"); }
    void set_table(cb::exact::emu::dpmi::TableReg, std::uint16_t, std::uint32_t, std::uint32_t, std::uint32_t) override {}
    cb::Bytes read(std::uint32_t address, std::size_t size) override { return cb::compat::from_py(old_.mem_read(address, static_cast<int64_t>(size))); }
    void write(std::uint32_t address, cb::ByteView data) override { old_.mem_write(address, cb::compat::to_py(data)); }
    void map_ram(std::uint32_t, std::uint64_t) override {}
    void stop() override {}

  private:
    dpmi::Engine& old_;
};

// A new-layer arena error as the old layer's.
template <class F>
auto arena(F&& f) {
    try {
        return f();
    } catch (const nd::ArenaError& e) {
        throw ArenaError(e.code());
    }
}

std::uint16_t seg16(int64_t v) { return static_cast<std::uint16_t>(v & 0xFFFF); }

}  // namespace

void _mcb_write(dpmi::Engine& mu, int64_t seg, const py::bytes& sig, int64_t owner, int64_t size) {
    MemoryView m(mu);
    nd::write_mcb(m, seg16(seg), sig.raw().empty() ? 'M' : sig.raw()[0], seg16(owner), seg16(size));
}

void _mcb_set(dpmi::Engine& mu, int64_t seg, const std::optional<py::bytes>& sig, const std::optional<int64_t>& owner,
              const std::optional<int64_t>& size) {
    if (sig) mu.mem_write(seg * 16, *sig);
    if (owner) mu.mem_write(seg * 16 + 1, py::pack("<H", *owner & 0xFFFF));
    if (size) mu.mem_write(seg * 16 + 3, py::pack("<H", *size & 0xFFFF));
}

std::tuple<int64_t, int64_t, int64_t> _mcb_read(dpmi::Engine& mu, int64_t seg) {
    MemoryView m(mu);
    return arena([&] {
        const nd::Mcb h = nd::read_mcb(m, seg16(seg));
        return std::make_tuple(int64_t(h.signature), int64_t(h.owner), int64_t(h.size));
    });
}

py::list<Mcb> mcb_chain(dpmi::Engine& mu, int64_t first) {
    MemoryView m(mu);
    return arena([&] {
        py::list<Mcb> out;
        for (const nd::Mcb& h : nd::mcb_chain(m, seg16(first))) out.append(Mcb(h.segment, h.signature, h.owner, h.size));
        return out;
    });
}

void _mcb_coalesce(dpmi::Engine& mu, int64_t first) {
    MemoryView m(mu);
    arena([&] {
        nd::coalesce(m, seg16(first));
        return 0;
    });
}

int64_t _mcb_largest(dpmi::Engine& mu, int64_t first) {
    MemoryView m(mu);
    return nd::largest(m, seg16(first));
}

void _split(dpmi::Engine& mu, int64_t seg, int64_t sig, int64_t size, int64_t keep) {
    if (size > keep) {
        _mcb_write(mu, seg + 1 + keep, py::bytes(std::string(1, static_cast<char>(sig))), MCB_FREE, size - keep - 1);
        _mcb_set(mu, seg, py::bytes("M"), std::nullopt, keep);
    }
}

std::optional<int64_t> _mcb_alloc(dpmi::Engine& mu, int64_t first, int64_t paras, int64_t owner, int64_t strategy) {
    MemoryView m(mu);
    const auto r = nd::allocate(m, seg16(first), seg16(paras), seg16(owner), static_cast<nd::Strategy>(strategy & 3));
    return r ? std::optional<int64_t>(*r) : std::nullopt;
}

void _mcb_free(dpmi::Engine& mu, int64_t first, int64_t block) {
    MemoryView m(mu);
    arena([&] {
        nd::free_block(m, seg16(first), seg16(block));
        return 0;
    });
}

std::optional<int64_t> _mcb_resize(dpmi::Engine& mu, int64_t first, int64_t block, int64_t paras) {
    MemoryView m(mu);
    return arena([&]() -> std::optional<int64_t> {
        const auto r = nd::resize(m, seg16(first), seg16(block), seg16(paras));
        return r ? std::optional<int64_t>(*r) : std::nullopt;
    });
}

// ---- MemFile -----------------------------------------------------------------------------------
py::bytes MemFile::read(std::optional<int64_t> n) {
    int64_t len = buf.size();
    int64_t end = (!n || *n < 0) ? len : std::min(len, pos + *n);
    py::bytes d = pos < end ? py::bytes(buf.raw().substr(static_cast<size_t>(pos), static_cast<size_t>(end - pos))) : py::bytes();
    pos = std::max(pos, end);
    return d;
}

int64_t MemFile::write(const py::bytes& d) {
    if (pos > buf.size()) buf.extend(zeros(pos - buf.size()));
    buf.set_slice(pos, pos + d.size(), d);
    pos += d.size();
    return d.size();
}

int64_t MemFile::seek(int64_t off, int64_t whence) {
    pos = std::max<int64_t>(0, whence == 0 ? off : whence == 1 ? pos + off : buf.size() + off);
    return pos;
}

int64_t MemFile::tell() { return pos; }

void MemFile::truncate(std::optional<int64_t> size) {
    // del self.buf[pos if size is None else size:]
    buf.set_slice(size ? *size : pos, py::none, py::bytes());
}

void MemFile::flush() {}
void MemFile::close() {}

// ---- MemFS -------------------------------------------------------------------------------------
py::dict<py::str, std::tuple<double, int64_t, py::bytes>>& MemFS::DISK() {
    static py::dict<py::str, std::tuple<double, int64_t, py::bytes>> d;
    return d;
}

MemFS::MemFS(const py::list<py::str>& hide_) {
    for (const py::str& h : hide_.vec())   // os.path.normcase(os.path.abspath(h)) + os.sep
        hide.append(py::os::path::normcase(py::os::path::abspath(h)) + py::str("\\"));
}

py::str MemFS::key(const py::str& p) const { return py::os::path::normcase(py::os::path::abspath(p)); }

bool MemFS::_hidden(const py::str& k) const {
    for (const py::str& h : hide.vec())
        if (k.startswith(h)) return true;
    return false;
}

py::bytes MemFS::disk(const py::str& p) const {
    py::str k = key(p);
    // os.stat(p): FileNotFoundError when it is not there
    double mtime = py::os::path::getmtime(p);
    std::error_code ec;
    int64_t size = static_cast<int64_t>(std::filesystem::file_size(py::fspath_(p), ec));
    if (ec) throw py::FileNotFoundError("[WinError 2] The system cannot find the file specified: " + py::repr(p).utf8());
    if (auto got = DISK().get(k)) {
        if (std::get<0>(*got) == mtime && std::get<1>(*got) == size) return std::get<2>(*got);
    }
    py::file f = ropen(p, py::str("rb"));
    py::bytes b = f.read();
    f.close();
    DISK()[k] = std::make_tuple(mtime, size, b);
    return b;
}

bool MemFS::isfile(const py::str& p) const {
    py::str k = key(p);
    if (files.contains(k)) return true;
    return !gone.contains(k) && !_hidden(k) && py::os::path::isfile(p);
}

int64_t MemFS::getsize(const py::str& p) const {
    py::str k = key(p);
    if (files.contains(k)) return files.at(k).size();
    std::error_code ec;
    int64_t n = static_cast<int64_t>(std::filesystem::file_size(py::fspath_(p), ec));
    if (ec) throw py::FileNotFoundError("[WinError 2] The system cannot find the file specified: " + py::repr(p).utf8());
    return n;
}

py::bytes MemFS::read(const py::str& p) const {
    py::str k = key(p);
    return files.contains(k) ? py::bytes(files.at(k).view()) : disk(p);
}

MemFile MemFS::open(const py::str& p, const py::str& mode) const {
    py::str k = key(p);
    if (mode.contains(py::str("w"))) {
        files[k] = py::bytearray();
        gone.discard(k);
    } else if (!files.contains(k)) {
        if (!isfile(p)) throw py::FileNotFoundError(p.utf8());
        files[k] = py::bytearray(disk(p));
    }
    return MemFile(files.at(k));
}

void MemFS::put(const py::str& p, const py::bytes& b) const {
    files[key(p)] = py::bytearray(b);
    gone.discard(key(p));
}

void MemFS::remove(const py::str& p) const {
    py::str k = key(p);
    if (files.contains(k)) {
        files.pop(k);
    } else if (!isfile(p)) {
        throw py::FileNotFoundError(p.utf8());
    }
    gone.add(k);
}

void MemFS::replace(const py::str& a, const py::str& b) const {
    put(b, read(a));
    remove(a);
}

py::file ropen(const py::str& path, const py::str& mode) {
    for (int64_t k = 0; k < 40; ++k) {
        try {
            return py::file::open(path, mode);
        } catch (py::FileNotFoundError&) {
            throw;
        } catch (py::OSError&) {
            if (k == 39) throw;
            py::time::sleep(0.05 * static_cast<double>(k + 1));
        }
    }
    throw py::RuntimeError("unreachable");   // (the loop returns or raises)
}

// ---- run ---------------------------------------------------------------------------------------
Result run(const py::str& exe, const py::str& workdir, const py::str& cmdline, const py::bytes& stdin_, int64_t max_blocks,
           bool trace_calls, const py::bytes& env, std::optional<int64_t> psp, std::optional<int64_t> top, bool fast,
           const MemFS* fs, bool dpmi) {
    // In Python: with no hooks, no DPMI host object and a MemFS (or the disk), the C++ executive answers
    // (native_dosexec.enabled()); otherwise the Python executive runs.  Here every run is of the first kind.
    if (!native_dosexec::enabled())
        throw py::NotImplementedError(
            "dos.run: the C++ executive is not available (CB_NATIVE_DOSEXEC off, or unicorn.dll not handed over "
            "with native_dosexec::load_unicorn / CB_UNICORN_DLL), and the Python executive is not part of the C++ port");
    return native_dosexec::run(exe, workdir, cmdline, stdin_, max_blocks, trace_calls, env, psp, top, fast, fs, dpmi,
                               FAST_LIMIT);
}

py::str main_(const py::list<py::str>& argv) {
    Result r = run(argv[1], argv[2], argv.size() > 3 ? argv[3] : py::str(""));
    std::string out = r.console.decode("latin1").utf8();
    out += "\n[exit " + (r.exit ? std::to_string(*r.exit) : std::string("None")) + ", " + std::to_string(r.steps) +
           " basic blocks executed, " + std::to_string(r.blocks.size()) + " distinct]\n";
    return py::str(out);
}

}  // namespace codebreaker::exact::emu::dos
