// codebreaker/build/masm.cpp -- typed C++17 port of codebreaker.build.masm (see masm.hpp).

#include "codebreaker/build/masm.hpp"

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <map>
#include <mutex>
#include <thread>

#include "codebreaker/base/cache.hpp"
#include "codebreaker/formats/omf.hpp"
#include "codebreaker/jobs/pool.hpp"
#include "py_settings.hpp"

namespace codebreaker::build::masm {

using py::str;

// ---------------------------------------------------------------------------------------------
// settings and the environment, read when first asked
namespace {
std::string env_or(const char* name, const char* dflt) {
    auto v = py::os::environ_::get(str(name));
    return v ? v->utf8() : std::string(dflt);
}
}  // namespace

const str& DBX() {
    static const str v = py::settings::get_str(str("dosbox_x"));   // settings.json: dosbox_x
    return v;
}

const str& MASMROOT() {
    static const str v = py::settings::get_str(str("masm"));   // settings.json: masm (BIN, INC, LIB)
    return v;
}

str _dbx() {
    // settings.dosbox_x(require=True): CB_NO_DOSBOX forbids every DOSBox launch
    auto no = py::os::environ_::get(str("CB_NO_DOSBOX"));
    if (no && !no->empty())
        throw py::RuntimeError(
            "DOSBox is disabled (CB_NO_DOSBOX=1): the optional period-compiler byte-match cannot run. Unset CB_NO_DOSBOX to "
            "allow the spot-check (the native behaviour gate does not need it).");
    str dbx = py::settings::get_str(str("dosbox_x"));
    if (dbx.empty() || !py::os::path::exists(dbx))
        throw py::RuntimeError("dosbox-x not found: " + dbx.utf8() + " (set it: CodeBreaker config dosbox_x <path>)");
    return dbx;
}

const str& DOSBOX_CORE() {
    static const str v = str(env_or("WT_DOSBOX_CORE", "dynamic"));   // validated byte-identical to `normal`
    return v;
}

const str& CONF() {
    static const str v = str("[sdl]\nautolock=false\n[cpu]\ncputype=386\ncore=") + DOSBOX_CORE() +
                         str("\ncycles=max\n[dos]\nver=6.22\n[autoexec]\nmount c \"{masm}\"\nmount d \"{work}\"\nset PATH=C:\\BIN\n"
                             "set INCLUDE=C:\\INC\nset LIB=C:\\LIB\nset TMP=D:\\\nd:\n{cmd}\necho done > D:\\DONE.FLG\nexit\n");
    return v;
}

str format_conf(const str& masm, const str& work, const str& cmd) {
    // str.format(masm=, work=, cmd=): each {name} replaced once, what is put in not read again
    const std::u32string& t = CONF().u32();
    std::u32string out;
    for (size_t i = 0; i < t.size();) {
        if (t[i] == U'{') {
            size_t e = t.find(U'}', i);
            std::u32string name = t.substr(i + 1, e - i - 1);
            out += (name == U"masm" ? masm : name == U"work" ? work : cmd).u32();
            i = e + 1;
        } else {
            out.push_back(t[i++]);
        }
    }
    return str(out);
}

str _wait_dosbox(dos_sessions::Popen& proc, const str& work, double timeout, const std::function<bool()>& aborted) {
    // The autoexec writes a DONE.FLG sentinel AFTER the MASM commands (all .OBJs are closed by then), so we can
    // kill the box the instant assembly finishes instead of waiting out its shutdown.
    str flag = py::os::path::join(work, str("DONE.FLG"));
    double waited = 0.0;
    while (!proc.poll()) {
        if (aborted()) {
            proc.kill();
            proc.wait(5.0);
            return str("aborted");
        }
        if (py::os::path::exists(flag)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(150));   // small settle so the last OBJ is flushed to the host FS
            proc.kill();
            return str("done");
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        waited += 0.05;
        if (waited > timeout) {
            proc.kill();
            return str("timeout");
        }
    }
    return str("done");   // exited on its own (older DOSBox, or MASM error)
}

namespace {
// open(p, "w", newline="\r\n").write(text)
void write_crlf(const str& path, const str& text) {
    py::file f = py::open(path, str("w"), std::nullopt, std::nullopt, str("\r\n"));
    f.write(text);
    f.close();
}

// the parts of a DOSBox run's work folder, as the Python reads them: .OBJ bytes, .LST and .LOG latin1 text
Product read_parts(const str& work, const str& stem) {
    Product d;
    for (const char* ext : {".OBJ", ".LST", ".LOG"}) {
        str p = py::os::path::join(work, stem + str(ext));
        if (py::os::path::exists(p)) {
            if (str(ext) == str(".OBJ")) {
                py::file f = py::open_rb(p);
                d.obj = f.read();
                f.close();
            } else {
                py::file f = py::open(p, str("r"), str("latin1"));
                (str(ext) == str(".LST") ? d.lst : d.log) = f.read_text();
                f.close();
            }
        }
    }
    return d;
}

dos_sessions::Popen launch_dosbox(const str& dbx, const str& conf) {
    py::dict<str, str> extra;
    extra.set(str("SDL_VIDEODRIVER"), str("dummy"));
    return dos_sessions::Popen(py::list<str>{dbx, str("-conf"), conf, str("-nolog")}, extra);
}
}  // namespace

Assembly assemble(const str& asm_src, const str& stem, double timeout, const std::optional<str>& masmroot) {
    str masm = (masmroot && !masmroot->empty()) ? *masmroot : MASMROOT();
    if (native_dos::enabled(str("masm"))) {
        try {
            return native_dos::assemble(asm_src, stem, masm);
        } catch (const native_dos::NativeDosError&) {
            // the native runner could not; fall back to DOSBox
        }
    }
    str dbx = _dbx();
    if (!py::os::path::exists(py::os::path::join(py::os::path::join(masm, str("BIN")), str("MASM.EXE"))))
        throw py::RuntimeError("MASM.EXE not found under " + masm.utf8() + "\\BIN (set it: CodeBreaker config masm <path>)");
    str work = py::tempfile::mkdtemp(std::nullopt, str("masm_"));
    write_crlf(py::os::path::join(work, stem + str(".ASM")), asm_src);
    str cmd = str("MASM ") + stem + str(".ASM, ") + stem + str(".OBJ, ") + stem + str(".LST, NUL; > ") + stem + str(".LOG 2>&1");
    str conf = py::os::path::join(work, str("masm.conf"));
    write_crlf(conf, format_conf(masm, work, cmd));
    // (run as a Popen and polled so a cancel could kill the assembler at once)
    std::function<bool()> aborted = [] { return false; };
    dos_sessions::Popen proc = launch_dosbox(dbx, conf);
    Assembly out;
    out.work = work;
    if (_wait_dosbox(proc, work, timeout, aborted) == str("aborted")) {
        out.aborted = true;
        return out;
    }
    Product d = read_parts(work, stem);
    out.obj = d.obj;
    out.lst = d.lst;
    out.log = d.log;
    return out;
}

// ---------------------------------------------------------------------------------------------
// warm worker pool
int64_t POOL_MAX() {
    static const int64_t v = std::max<int64_t>(1, py::int_parse(str(env_or("WT_POOL_MAX", "3"))));
    return v;
}

bool WARM_POOL() {
    static const bool v = env_or("WT_NO_WARM_POOL", "") != "1";
    return v;
}

int64_t WORKER_MAX_USES() {
    static const int64_t v = py::int_parse(str(env_or("WT_WORKER_MAX_USES", "0")));
    return v;
}

int64_t ASM_BATCH() {
    static const int64_t v = std::max<int64_t>(1, py::int_parse(str(env_or("WT_ASM_BATCH", "40"))));
    return v;
}

namespace {
std::map<str, std::shared_ptr<dos_sessions::WorkerPool>>& pools() {
    static std::map<str, std::shared_ptr<dos_sessions::WorkerPool>> p;
    return p;
}
std::mutex& pools_lock() {
    static std::mutex m;
    return m;
}
std::map<const dos_sessions::DosboxWorker*, int64_t>& uses() {   // worker._uses
    static std::map<const dos_sessions::DosboxWorker*, int64_t> u;
    return u;
}
}  // namespace

std::shared_ptr<dos_sessions::WorkerPool> _get_pool(const str& masmroot) {
    std::lock_guard<std::mutex> g(pools_lock());
    auto it = pools().find(masmroot);
    if (it == pools().end()) {
        static bool registered = (std::atexit([] { close_pools(); }), true);   // atexit.register(close_pools)
        (void)registered;
        it = pools().emplace(masmroot, std::make_shared<dos_sessions::WorkerPool>(masmroot, _dbx(), POOL_MAX())).first;
    }
    return it->second;
}

void close_pools() {
    std::lock_guard<std::mutex> g(pools_lock());
    for (auto& kv : pools()) {
        try {
            kv.second->close();
        } catch (const std::exception&) {
        }
    }
    pools().clear();
}

bool _cancelled() { return false; }

// ---------------------------------------------------------------------------------------------
// zlib (a stream any zlib reads): inflate, and stored blocks
namespace {

uint32_t adler32(const std::string& d) {
    uint32_t a = 1, b = 0;
    for (unsigned char c : d) {
        a = (a + c) % 65521;
        b = (b + a) % 65521;
    }
    return (b << 16) | a;
}

struct Huff {
    int16_t count[16];
    int16_t symbol[288];
};

struct Inflate {
    const unsigned char* in;
    size_t n, pos = 0;
    uint32_t bitbuf = 0;
    int bitcnt = 0;
    std::string out;
    bool bad = false;

    int bits(int need) {
        long val = bitbuf;
        while (bitcnt < need) {
            if (pos >= n) {
                bad = true;
                return 0;
            }
            val |= static_cast<long>(in[pos++]) << bitcnt;
            bitcnt += 8;
        }
        bitbuf = static_cast<uint32_t>(val >> need);
        bitcnt -= need;
        return static_cast<int>(val & ((1L << need) - 1));
    }

    static int construct(Huff& h, const int16_t* length, int num) {
        for (int len = 0; len <= 15; len++) h.count[len] = 0;
        for (int s = 0; s < num; s++) h.count[length[s]]++;
        if (h.count[0] == num) return 0;
        int left = 1;
        for (int len = 1; len <= 15; len++) {
            left <<= 1;
            left -= h.count[len];
            if (left < 0) return left;
        }
        int16_t offs[16];
        offs[1] = 0;
        for (int len = 1; len < 15; len++) offs[len + 1] = offs[len] + h.count[len];
        for (int s = 0; s < num; s++)
            if (length[s] != 0) h.symbol[offs[length[s]]++] = static_cast<int16_t>(s);
        return left;
    }

    int decode(const Huff& h) {
        int code = 0, first = 0, index = 0;
        for (int len = 1; len <= 15; len++) {
            code |= bits(1);
            if (bad) return -1;
            int count = h.count[len];
            if (code - count < first) return h.symbol[index + (code - first)];
            index += count;
            first += count;
            first <<= 1;
            code <<= 1;
        }
        return -1;
    }

    bool codes(const Huff& lencode, const Huff& distcode) {
        static const int16_t lens[29] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31, 35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258};
        static const int16_t lext[29] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0};
        static const int16_t dists[30] = {1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193, 257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577};
        static const int16_t dext[30] = {0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13};
        for (;;) {
            int sym = decode(lencode);
            if (sym < 0) return false;
            if (sym < 256) {
                out.push_back(static_cast<char>(sym));
            } else if (sym == 256) {
                return true;
            } else {
                sym -= 257;
                if (sym >= 29) return false;
                int len = lens[sym] + bits(lext[sym]);
                int ds = decode(distcode);
                if (ds < 0 || ds >= 30) return false;
                size_t dist = static_cast<size_t>(dists[ds] + bits(dext[ds]));
                if (bad || dist > out.size()) return false;
                for (int i = 0; i < len; i++) out.push_back(out[out.size() - dist]);
            }
        }
    }

    bool stored() {
        bitbuf = 0;
        bitcnt = 0;
        if (pos + 4 > n) return false;
        unsigned len = in[pos] | (in[pos + 1] << 8);
        unsigned nlen = in[pos + 2] | (in[pos + 3] << 8);
        pos += 4;
        if (len != (~nlen & 0xFFFF) || pos + len > n) return false;
        out.append(reinterpret_cast<const char*>(in + pos), len);
        pos += len;
        return true;
    }

    bool fixed() {
        Huff lencode, distcode;
        int16_t lengths[288];
        int s = 0;
        for (; s < 144; s++) lengths[s] = 8;
        for (; s < 256; s++) lengths[s] = 9;
        for (; s < 280; s++) lengths[s] = 7;
        for (; s < 288; s++) lengths[s] = 8;
        construct(lencode, lengths, 288);
        for (s = 0; s < 30; s++) lengths[s] = 5;
        construct(distcode, lengths, 30);
        return codes(lencode, distcode);
    }

    bool dynamic() {
        static const int16_t order[19] = {16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};
        int nlen = bits(5) + 257, ndist = bits(5) + 1, ncode = bits(4) + 4;
        if (bad || nlen > 286 || ndist > 30) return false;
        int16_t lengths[320];
        int index = 0;
        for (; index < ncode; index++) lengths[order[index]] = static_cast<int16_t>(bits(3));
        for (; index < 19; index++) lengths[order[index]] = 0;
        Huff lencode, distcode;
        if (construct(lencode, lengths, 19) != 0) return false;
        index = 0;
        while (index < nlen + ndist) {
            int sym = decode(lencode);
            if (sym < 0) return false;
            if (sym < 16) {
                lengths[index++] = static_cast<int16_t>(sym);
            } else {
                int len = 0, rep;
                if (sym == 16) {
                    if (index == 0) return false;
                    len = lengths[index - 1];
                    rep = 3 + bits(2);
                } else if (sym == 17) {
                    rep = 3 + bits(3);
                } else {
                    rep = 11 + bits(7);
                }
                if (index + rep > nlen + ndist) return false;
                while (rep--) lengths[index++] = static_cast<int16_t>(len);
            }
        }
        if (lengths[256] == 0) return false;
        int err = construct(lencode, lengths, nlen);
        if (err != 0 && (err < 0 || nlen != lencode.count[0] + lencode.count[1])) return false;
        err = construct(distcode, lengths + nlen, ndist);
        if (err != 0 && (err < 0 || ndist != distcode.count[0] + distcode.count[1])) return false;
        return codes(lencode, distcode);
    }
};

}  // namespace

std::optional<py::bytes> zlib_decompress(const py::bytes& data) {
    const std::string& z = data.raw();
    if (z.size() < 6) return std::nullopt;
    unsigned cmf = static_cast<unsigned char>(z[0]), flg = static_cast<unsigned char>(z[1]);
    if ((cmf & 15) != 8 || (cmf >> 4) > 7 || ((cmf << 8) | flg) % 31 != 0 || (flg & 0x20)) return std::nullopt;
    Inflate s{reinterpret_cast<const unsigned char*>(z.data()) + 2, z.size() - 2};
    int last;
    do {
        last = s.bits(1);
        int type = s.bits(2);
        if (s.bad) return std::nullopt;
        bool ok = type == 0 ? s.stored() : type == 1 ? s.fixed() : type == 2 ? s.dynamic() : false;
        if (!ok || s.bad) return std::nullopt;
    } while (!last);
    if (s.pos + 4 > s.n) return std::nullopt;
    uint32_t want = (static_cast<uint32_t>(s.in[s.pos]) << 24) | (static_cast<uint32_t>(s.in[s.pos + 1]) << 16) |
                    (static_cast<uint32_t>(s.in[s.pos + 2]) << 8) | s.in[s.pos + 3];
    if (want != adler32(s.out)) return std::nullopt;
    return py::bytes(s.out);
}

py::bytes zlib_compress(const py::bytes& data) {
    const std::string& d = data.raw();
    std::string z = "\x78\x01";
    size_t pos = 0;
    do {
        size_t n = std::min<size_t>(65535, d.size() - pos);
        bool last = pos + n >= d.size();
        z.push_back(last ? 1 : 0);
        z.push_back(static_cast<char>(n & 0xFF));
        z.push_back(static_cast<char>(n >> 8));
        z.push_back(static_cast<char>(~n & 0xFF));
        z.push_back(static_cast<char>((~n >> 8) & 0xFF));
        z.append(d, pos, n);
        pos += n;
    } while (pos < d.size());
    uint32_t a = adler32(d);
    for (int sh = 24; sh >= 0; sh -= 8) z.push_back(static_cast<char>((a >> sh) & 0xFF));
    return py::bytes(z);
}

// ---------------------------------------------------------------------------------------------
// the assembled-bytes cache
namespace {
std::map<str, str>& roots_seen() {   // _ROOTS
    static std::map<str, str> r;
    return r;
}
std::mutex& roots_lock() {
    static std::mutex m;
    return m;
}
}  // namespace

str _cache_key(const str& src, const str& masmroot, const str& stem) {
    str root = py::os::path::normpath(masmroot);
    str seen;
    {
        std::lock_guard<std::mutex> g(roots_lock());
        auto it = roots_seen().find(root);
        if (it == roots_seen().end()) {
            str v = py::os::path::isdir(root) ? base::cache::files_bytes(py::list<str>{root}) : base::cache::tool_bytes(root);
            it = roots_seen().emplace(root, v).first;
        }
        seen = it->second;
    }
    return base::cache::key({base::cache::Part(str("masm")), base::cache::Part(CACHE_VER), base::cache::Part(seen),
                             base::cache::Part(stem.upper()), base::cache::Part(src)});
}

std::optional<Product> _cache_get(const str& key) {
    std::optional<py::bytes> blob_z = base::cache::get(str("masm"), key);
    if (!blob_z) return std::nullopt;
    std::optional<py::bytes> blob_o = zlib_decompress(*blob_z);
    if (!blob_o) return std::nullopt;
    const std::string& blob = blob_o->raw();
    uint32_t n = 0;
    for (size_t i = 0; i < 4 && i < blob.size(); ++i) n |= static_cast<uint32_t>(static_cast<unsigned char>(blob[i])) << (8 * i);
    Product p;
    size_t lo = std::min<size_t>(4, blob.size()), hi = std::min<size_t>(4 + static_cast<size_t>(n), blob.size());
    p.obj = py::bytes(blob.substr(lo, hi - lo));
    p.lst = py::bytes(blob.substr(std::min<size_t>(4 + static_cast<size_t>(n), blob.size()))).decode("latin1");
    p.log = str("");
    return p;
}

void _cache_put(const str& key, const Product& res) {
    if (!res.obj || res.obj->empty()) return;   // never cache an empty/failed assembly
    const std::string& obj = res.obj->raw();
    std::string blob;
    for (int i = 0; i < 4; ++i) blob.push_back(static_cast<char>((obj.size() >> (8 * i)) & 0xFF));
    blob += obj;
    blob += (res.lst ? *res.lst : str("")).encode("latin1", "replace").raw();
    base::cache::put(str("masm"), key, zlib_compress(py::bytes(blob)));
}

// ---------------------------------------------------------------------------------------------
// assemble_many

Assembled assemble_many(const Items& items, std::optional<double> timeout, const std::optional<str>& masmroot) {
    str masm = (masmroot && !masmroot->empty()) ? *masmroot : MASMROOT();
    py::dict<str, Product> hits;
    Items misses;
    py::dict<str, str> mkeys;
    for (const auto& it : items) {
        const str& stem = std::get<0>(it);
        const str& src = std::get<1>(it);
        str key = _cache_key(src, masm, stem);
        std::optional<Product> c = _cache_get(key);
        if (c) {
            hits.set(stem, *c);
        } else {
            misses.append(it);
            mkeys.set(stem, key);
        }
    }
    if (misses.empty()) {   // everything cached: no assembler run at all
        Assembled a;
        a.work = str("");
        a.results = hits;
        return a;
    }
    Assembled out = _assemble_uncached(misses, timeout, masm);
    if (out.aborted) return out;
    py::dict<str, Product> fresh = out.results ? *out.results : py::dict<str, Product>();
    for (const auto& it : misses) {   // persist newly-assembled results
        const str& stem = std::get<0>(it);
        if (fresh.contains(stem)) {
            const Product& r = fresh.at(stem);
            if (r.obj || r.lst || r.log) _cache_put(mkeys.at(stem), r);
        }
    }
    fresh.update(hits);   // merge cache hits back in
    Assembled a;
    a.work = out.work ? *out.work : str("");
    a.results = fresh;
    return a;
}

Assembled _assemble_uncached(const Items& items, std::optional<double> timeout, const std::optional<str>& masmroot) {
    str masm = (masmroot && !masmroot->empty()) ? *masmroot : MASMROOT();
    if (native_dos::enabled(str("masm"))) {
        try {
            return native_dos::assemble_many(items, masm);
        } catch (const native_dos::NativeDosError&) {
            // the native runner could not; fall back to DOSBox
        }
    }
    str dbx = _dbx();   // honours CB_NO_DOSBOX; also used by the cold fallback
    if (!py::os::path::exists(py::os::path::join(py::os::path::join(masm, str("BIN")), str("MASM.EXE"))))
        throw py::RuntimeError("MASM.EXE not found under " + masm.utf8() + "\\BIN (set it: CodeBreaker config masm <path>)");

    // Chunk large modules so no single DOSBox dispatch can overflow the [autoexec] buffer (E_Exit) on the
    // cold-boot path. Results merge; an abort in any chunk aborts the whole call.
    if (items.size() > ASM_BATCH()) {
        py::list<Items> chunks;
        for (int64_t b = 0; b < items.size(); b += ASM_BATCH()) chunks.append(items.slice(b, b + ASM_BATCH()));
        py::dict<str, Product> merged;
        // Fan chunks across the warm pool: each sub-call leases its OWN worker, so a single large call uses all
        // POOL_MAX DOSBoxes instead of one serially. Serial when the warm pool is off.
        int64_t nthreads = WARM_POOL() ? std::min<int64_t>(POOL_MAX(), chunks.size()) : 1;
        Assembled none;
        none.work = str("");
        if (nthreads <= 1) {
            for (const Items& ch : chunks) {
                Assembled r = _assemble_uncached(ch, timeout, masm);
                if (r.aborted) {
                    Assembled a;
                    a.work = str("");
                    a.aborted = true;
                    return a;
                }
                if (r.results) merged.update(*r.results);
            }
            Assembled a;
            a.work = str("");
            a.results = merged;
            return a;
        }
        bool aborted = false;
        py::list<Assembled> got = jobs::pool::threads([&](const Items& ch) { return _assemble_uncached(ch, timeout, masm); }, chunks, nthreads);
        for (const Assembled& r : got) {
            if (r.aborted) aborted = true;
            else if (r.results) merged.update(*r.results);
        }
        Assembled a;
        a.work = str("");
        if (aborted) a.aborted = true;
        else a.results = merged;
        return a;
    }

    if (WARM_POOL()) {
        std::shared_ptr<dos_sessions::WorkerPool> pool;
        std::shared_ptr<dos_sessions::DosboxWorker> worker;
        try {
            pool = _get_pool(masm);
            worker = pool->lease();
        } catch (const std::exception&) {
            worker = nullptr;
            pool = nullptr;
        }
        if (worker) {
            Assembled res = worker->assemble_many(items, timeout, _cancelled);
            if (res.aborted) {
                pool->release(worker, false);
                Assembled a;
                a.work = str("");
                a.aborted = true;
                return a;
            }
            if (!res.dead) {
                int64_t n;
                {
                    std::lock_guard<std::mutex> g(pools_lock());
                    n = uses()[worker.get()] += 1;
                }
                // Retire the worker before it degrades (see WORKER_MAX_USES); keep=false drops it so the pool boots a fresh
                // one next lease. work="" so the caller's rmtree is a harmless no-op.
                bool keep = WORKER_MAX_USES() <= 0 || n < WORKER_MAX_USES();
                pool->release(worker, keep);
                Assembled a;
                a.work = str("");
                a.results = res.results;
                return a;
            }
            pool->release(worker, false);   // dead: drop it, cold-boot below
        }
    }
    double t = timeout ? *timeout : static_cast<double>(120 + 3 * items.size());
    str work = py::tempfile::mkdtemp(std::nullopt, str("masm_"));
    str cmd;
    for (const auto& it : items) {
        const str& s = std::get<0>(it);
        write_crlf(py::os::path::join(work, s + str(".ASM")), std::get<1>(it));
        cmd = cmd + (cmd.empty() ? str("") : str("\n")) + str("MASM ") + s + str(".ASM, ") + s + str(".OBJ, ") + s + str(".LST, NUL; > ") + s +
              str(".LOG 2>&1");
    }
    str conf = py::os::path::join(work, str("masm.conf"));
    write_crlf(conf, format_conf(masm, work, cmd));
    std::function<bool()> aborted = [] { return false; };
    dos_sessions::Popen proc = launch_dosbox(dbx, conf);
    if (_wait_dosbox(proc, work, t, aborted) == str("aborted")) {
        Assembled a;
        a.work = work;
        a.aborted = true;
        return a;
    }
    py::dict<str, Product> results;
    for (const auto& it : items) results.set(std::get<0>(it), read_parts(work, std::get<0>(it)));
    Assembled a;
    a.work = work;
    a.results = results;
    return a;
}

py::dict<str, std::tuple<py::bytes, py::set<int64_t>>> functions(const str& asm_src, const std::optional<py::list<str>>&) {
    Assembly r = assemble(asm_src);
    if (!r.obj) throw py::RuntimeError("MASM produced no OBJ:\n" + (r.log ? r.log->slice(0, 600) : str("")).utf8());
    return formats::omf::function_bytes(*r.obj);
}

const str SELF_ASM(
    "_TEXT   segment byte public 'CODE'\n"
    "        assume  cs:_TEXT\n"
    "        public  _addasm\n"
    "_addasm proc near\n"
    "        push    bp\n"
    "        mov     bp, sp\n"
    "        mov     ax, [bp+4]\n"
    "        add     ax, [bp+6]\n"
    "        pop     bp\n"
    "        ret\n"
    "_addasm endp\n"
    "_TEXT   ends\n"
    "        end\n");

namespace {
void say(const std::string& s) { std::cout << s << "\n"; }
}  // namespace

bool _selftest() {
    Assembly r = assemble(SELF_ASM);
    if (!r.obj) {
        say("no OBJ; MASM log:\n" + (r.log ? r.log->slice(0, 800) : str("")).utf8());
        return false;
    }
    auto fns = formats::omf::function_bytes(*r.obj);
    py::dict<str, int64_t> sizes;
    for (const auto& kv : fns.items()) sizes.set(std::get<0>(kv), std::get<0>(std::get<1>(kv)).size());
    std::string shown_sizes;   // repr(dict)
    for (const auto& kv : sizes.items())
        shown_sizes += (shown_sizes.empty() ? "" : ", ") + py::repr(std::get<0>(kv)).utf8() + ": " + std::to_string(std::get<1>(kv));
    say("functions: {" + shown_sizes + "}");
    // MASM upper-cases public symbols by default (_ADDASM); match case-insensitively
    std::optional<str> key;
    for (const str& k : fns.keys())
        if (k.lower() == str("_addasm")) {
            key = k;
            break;
        }
    py::bytes b = key ? std::get<0>(fns.at(*key)) : py::bytes(std::string());
    say("_addasm bytes: " + b.hex().utf8());
    // 55 8b ec  8b 46 04  03 46 06  5d c3  = push bp;mov bp,sp;mov ax,[bp+4];add;pop;ret
    const std::string& raw = b.raw();
    bool ok = raw.compare(0, 3, "\x55\x8b\xec") == 0 && raw.find(std::string("\x03\x46\x06", 3)) != std::string::npos && !raw.empty() &&
              raw.back() == '\xc3';
    say("\nSELFTEST: " + std::string(ok ? "PASS - MASM assembled byte-exact proc" : "FAIL"));
    return ok;
}

int64_t main(const py::list<str>& argv) {
    std::optional<str> asm_path;
    bool selftest = false;
    for (const str& a : argv) {
        if (a == str("--selftest")) {
            selftest = true;
        } else if (a == str("-h") || a == str("--help")) {
            say("usage: masmlift.py [-h] [--selftest] [asm]");
            return 0;
        } else if (a.startswith(str("-"))) {
            std::cerr << "usage: masmlift.py [-h] [--selftest] [asm]\nmasmlift.py: error: unrecognized arguments: " << a.utf8() << "\n";
            return 2;
        } else if (!asm_path) {
            asm_path = a;
        } else {
            std::cerr << "usage: masmlift.py [-h] [--selftest] [asm]\nmasmlift.py: error: unrecognized arguments: " << a.utf8() << "\n";
            return 2;
        }
    }
    if (selftest) return _selftest() ? 0 : 1;
    if (!asm_path) {
        std::cerr << "usage: masmlift.py [-h] [--selftest] [asm]\nmasmlift.py: error: give a .ASM file or --selftest\n";
        return 2;
    }
    py::file f = py::open(*asm_path);
    str text = f.read_text();
    f.close();
    Assembly r = assemble(text);
    if (!r.obj) {
        std::cerr << "no OBJ produced; MASM log:\n" << (r.log ? r.log->slice(0, 1000) : str("")).utf8() << "\n";
        return 1;
    }
    formats::omf::Obj obj = formats::omf::read(*r.obj);
    for (const auto& s : obj.segs) {
        const str& name = std::get<0>(s);
        const str& cls = std::get<1>(s);
        py::bytes b = obj.seg.get(name, py::bytes(std::string()));
        char buf[512];
        std::snprintf(buf, sizeof buf, "  %-12s class=%-8s %6lld bytes", name.utf8().c_str(), cls.utf8().c_str(), static_cast<long long>(b.size()));
        say(buf);
    }
    say("# work dir: " + r.work.utf8());
    return 0;
}

}  // namespace codebreaker::build::masm
