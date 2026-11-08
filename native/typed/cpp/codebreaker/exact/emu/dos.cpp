// codebreaker/exact/emu/dos.cpp -- C++ port of codebreaker.exact.emu.dos (see dos.hpp).

#include "codebreaker/exact/emu/dos.hpp"

#include <algorithm>
#include <cstdlib>
#include <filesystem>

#include "codebreaker/exact/emu/native_dosexec.hpp"

namespace codebreaker::exact::emu::dos {

const double FAST_LIMIT = std::strtod(py::os::getenv(py::str("DOSRUN_FAST_LIMIT"), py::str("20")).utf8().c_str(), nullptr);
const int64_t DEFAULT_MAX_BLOCKS = py::int_parse(py::os::getenv(py::str("DOSRUN_MAX_BLOCKS"), py::str("60000000")), 10);

namespace {

// bytes([v])
py::bytes byte1(int64_t v) { return py::bytes(std::string(1, static_cast<char>(v))); }

// a bytes of `n` zero bytes
py::bytes zeros(int64_t n) { return py::bytes::zeros(n); }

}  // namespace

// ---- the DOS memory arena ----------------------------------------------------------------------
void _mcb_write(dpmi::Engine& mu, int64_t seg, const py::bytes& sig, int64_t owner, int64_t size) {
    mu.mem_write(seg * 16, py::bytes(sig.raw() + py::pack("<HH", owner & 0xFFFF, size & 0xFFFF).raw() + zeros(11).raw()));
}

void _mcb_set(dpmi::Engine& mu, int64_t seg, const std::optional<py::bytes>& sig, const std::optional<int64_t>& owner,
              const std::optional<int64_t>& size) {
    if (sig) mu.mem_write(seg * 16, *sig);
    if (owner) mu.mem_write(seg * 16 + 1, py::pack("<H", *owner & 0xFFFF));
    if (size) mu.mem_write(seg * 16 + 3, py::pack("<H", *size & 0xFFFF));
}

std::tuple<int64_t, int64_t, int64_t> _mcb_read(dpmi::Engine& mu, int64_t seg) {
    py::bytes h = mu.mem_read(seg * 16, 5);
    int64_t h0 = h[0], h1 = h[1], h2 = h[2], h3 = h[3], h4 = h[4];
    if (h0 != 0x4D && h0 != 0x5A) throw ArenaError(7);
    return {h0, h1 | h2 << 8, h3 | h4 << 8};
}

py::list<Mcb> mcb_chain(dpmi::Engine& mu, int64_t first) {
    py::list<Mcb> out;
    int64_t seg = first;
    for (int64_t k = 0; k < 0x10000; ++k) {
        auto [sig, owner, size] = _mcb_read(mu, seg);
        out.append(Mcb(seg, sig, owner, size));
        if (sig == 0x5A) return out;
        seg += 1 + size;
        if (seg > 0xFFFF) break;
    }
    throw ArenaError(7);
}

void _mcb_coalesce(dpmi::Engine& mu, int64_t first) {
    py::list<Mcb> chain = mcb_chain(mu, first);
    int64_t i = 0;
    while (i < chain.size()) {
        auto [seg, sig, owner, size] = chain[i];
        int64_t j = i;
        while (owner == MCB_FREE && j + 1 < chain.size() && std::get<2>(chain[j + 1]) == MCB_FREE) {
            j += 1;
            size += 1 + std::get<3>(chain[j]);
            sig = std::get<1>(chain[j]);
        }
        if (j > i) _mcb_set(mu, seg, byte1(sig), std::nullopt, size);
        i = j + 1;
    }
}

int64_t _mcb_largest(dpmi::Engine& mu, int64_t first) {
    try {
        _mcb_coalesce(mu, first);
        int64_t best = 0;   // max([sizes of the free blocks] + [0])
        py::list<Mcb> chain = mcb_chain(mu, first);
        for (const Mcb& c : chain.vec())
            if (std::get<2>(c) == MCB_FREE) best = std::max(best, std::get<3>(c));
        return best;
    } catch (ArenaError&) {
        return 0;
    }
}

void _split(dpmi::Engine& mu, int64_t seg, int64_t sig, int64_t size, int64_t keep) {
    if (size > keep) {
        _mcb_write(mu, seg + 1 + keep, byte1(sig), MCB_FREE, size - keep - 1);
        _mcb_set(mu, seg, py::bytes("M"), std::nullopt, keep);
    }
}

std::optional<int64_t> _mcb_alloc(dpmi::Engine& mu, int64_t first, int64_t paras, int64_t owner, int64_t strategy) {
    py::list<Mcb> chain;
    try {
        _mcb_coalesce(mu, first);
        chain = mcb_chain(mu, first);
    } catch (ArenaError&) {
        return std::nullopt;
    }
    std::vector<Mcb> fits;
    for (const Mcb& c : chain.vec())
        if (std::get<2>(c) == MCB_FREE && std::get<3>(c) >= paras) fits.push_back(c);
    if (fits.empty()) return std::nullopt;
    strategy &= 3;
    Mcb pick = fits[0];
    if (strategy == 2) {
        pick = fits.back();
    } else if (strategy != 0) {   // min(fits, key=size): the first of the smallest
        for (const Mcb& c : fits)
            if (std::get<3>(c) < std::get<3>(pick)) pick = c;
    }
    auto [seg, sig, own, size] = pick;
    (void)own;
    if (strategy == 2 && size > paras) {   // the block at the free block's top end
        _mcb_set(mu, seg, py::bytes("M"), std::nullopt, size - paras - 1);
        seg += size - paras;
        _mcb_write(mu, seg, byte1(sig), owner, paras);
        return seg + 1;
    }
    _split(mu, seg, sig, size, paras);
    _mcb_set(mu, seg, std::nullopt, owner);
    return seg + 1;
}

void _mcb_free(dpmi::Engine& mu, int64_t first, int64_t block) {
    try {
        if (block < 1) throw ArenaError(9);
        _mcb_read(mu, block - 1);
    } catch (ArenaError&) {
        throw ArenaError(9);   // no header there: not a block
    }
    bool found = false;
    py::list<Mcb> chain = mcb_chain(mu, first);
    for (const Mcb& c : chain.vec())
        if (std::get<0>(c) == block - 1) found = true;
    if (!found) throw ArenaError(9);
    _mcb_set(mu, block - 1, std::nullopt, MCB_FREE);
    _mcb_coalesce(mu, first);
}

std::optional<int64_t> _mcb_resize(dpmi::Engine& mu, int64_t first, int64_t block, int64_t paras) {
    _mcb_coalesce(mu, first);
    py::list<Mcb> chain = mcb_chain(mu, first);
    std::optional<int64_t> k;
    for (int64_t i = 0; i < chain.size(); ++i)
        if (std::get<0>(chain[i]) == block - 1) {
            k = i;
            break;
        }
    if (!k) throw ArenaError(9);
    auto [seg, sig, owner, size] = chain[*k];
    (void)owner;
    const Mcb* nxt = *k + 1 < chain.size() ? &chain.vec()[static_cast<size_t>(*k + 1)] : nullptr;
    int64_t avail = size + (nxt != nullptr && std::get<2>(*nxt) == MCB_FREE ? 1 + std::get<3>(*nxt) : 0);
    if (paras > avail) return avail;
    if (paras > size) {   // grow into the free block behind
        sig = std::get<1>(*nxt);
        size = avail;
        _mcb_set(mu, seg, byte1(sig), std::nullopt, size);
    }
    _split(mu, seg, sig, size, paras);
    _mcb_coalesce(mu, first);
    return std::nullopt;
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
