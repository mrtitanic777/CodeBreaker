// codebreaker/source/vxd/owbuild.cpp -- typed C++ of codebreaker.source.vxd.owbuild.

#include "codebreaker/source/vxd/owbuild.hpp"

#include <algorithm>
#include <ostream>
#include <string>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/formats/le.hpp"
#include "codebreaker/source/vxd/link.hpp"
#include "codebreaker/source/vxd/tree.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"
#include "py_stdlib.hpp"
#include "py_subprocess.hpp"

namespace codebreaker::source::vxd::owbuild {

namespace le = codebreaker::formats::le;

const py::str& _OW() {
    static const py::str v = [] {
        const std::optional<py::str> ow = py::settings::get_opt_str(py::str("openwatcom"));
        const py::str d = py::os::path::dirname(ow ? *ow : py::str(""));
        return !d.empty() ? d : py::os::path::join(py::settings::checkout_root(), py::str("experiments"), py::str("premise-proof"), py::str("open-watcom-v2"));
    }();
    return v;
}
const py::str& _WASM() {
    static const py::str v = py::os::path::join(_OW(), py::str("bld"), py::str("wasm"), py::str("nt386"), py::str("wasm.exe"));
    return v;
}
const py::str& _WLINK() {
    static const py::str v = py::os::path::join(_OW(), py::str("rel"), py::str("binnt64"), py::str("wlink.exe"));
    return v;
}
const py::str& _NASM() {
    static const py::str v = py::settings::get_str(py::str("nasm"));
    return v;
}

py::dict<py::str, py::str> _env() {
    py::dict<py::str, py::str> e = py::os::environ_::copy();
    e.set(py::str("WATCOM"), py::os::path::join(_OW(), py::str("rel")));
    const py::str old = e.get(py::str("PATH"), py::str(""));
    e.set(py::str("PATH"), py::os::path::join(_OW(), py::str("rel"), py::str("binnt64")) + py::str(";") + old);
    return e;
}

namespace {

// open(path, "w", newline="\n").write(text)
void write_lf(const py::str& path, const py::str& text) {
    py::file f = py::open(path, py::str("w"), py::str("cp1252"), std::nullopt, py::str("\n"));
    f.write(text);
    f.close();
}

py::bytes read_all(const py::str& path) {
    py::file f = py::open_rb(path);
    py::bytes b = f.read();
    f.close();
    return b;
}

void write_all(const py::str& path, const std::string& data) {
    py::file f = py::open(path, py::str("wb"));
    f.write(py::bytes(data));
    f.close();
}

}  // namespace

BuildResult build_vxd(const py::str& vxd_path, const py::str& name, const py::str& outroot, bool byte_exact) {
    BuildResult res;
    if (!(py::os::path::exists(_WASM()) && py::os::path::exists(_WLINK()))) {
        res.error = py::str("OpenWatcom wasm/wlink not built");
        return res;
    }
    const le::LEFile v = le::LEFile::from_file(vxd_path);
    const py::dict<py::str, py::str> env = _env();
    // emit UNSPLIT (max_ins huge) so cross-object branch targets stay internal. byte_exact: db every instruction the assembler
    // re-encodes differently from the oracle (the verified force-raw set) so the built binary is byte-identical to retail; otherwise
    // just converge to assemble (buildable, may drift).
    std::optional<link::ForceRaw> fr_map;
    if (byte_exact) fr_map = std::get<0>(link::byte_exact_force_raw(v));
    const py::list<py::str> files = tree::emit_tree(v, name.upper(), outroot, 1000000000, std::nullopt,
                                                    byte_exact ? std::nullopt : std::optional<py::str>(_WASM()), fr_map);
    const py::str d = py::os::path::join(outroot, name.lower());
    py::list<py::str> objs, asmfail;
    bool ddb = false;
    for (const py::str& fn : files) {
        if (!fn.endswith(py::str(".asm"))) continue;
        const py::str src = py::os::path::join(d, fn);
        const py::str text = read_all(src).decode("utf-8", "replace");
        if (text.contains(py::str("public VXD_DDB"))) ddb = true;
        const py::str ob = fn.slice(0, py::len(fn) - 4) + py::str(".obj");
        // byte-exact build uses the same nasm-masm (COFF) the force-raw set was verified against; the buildable build uses wasm.
        py::subprocess::RunOptions o;
        o.cwd = d;
        o.env = env;
        py::subprocess::CompletedProcess r;
        if (byte_exact) {
            r = py::subprocess::run(py::list<py::str>{_NASM(), py::str("--masm"), py::str("-f"), py::str("win32"), fn, py::str("-o"), ob}, o);
        } else {
            r = py::subprocess::run(py::list<py::str>{_WASM(), py::str("-zcm=masm"), py::str("-q"), py::str("-fo=") + ob, fn}, o);
        }
        if (r.returncode == 0 && py::os::path::exists(py::os::path::join(d, ob))) objs.append(ob);
        else asmfail.append(fn);
    }
    if (!asmfail.empty()) {
        res.error = py::str("assemble failed: ") + py::join(py::str(", "), asmfail);
        return res;
    }
    const py::str lnk = py::os::path::join(d, py::str("link.lnk"));
    py::str lnk_text = py::str("system win_vxd\nname ") + name.lower() + py::str(".vxd\noption quiet\n");
    lnk_text += py::str("file ") + py::join(py::str(","), objs) + py::str("\n");
    if (ddb) lnk_text += py::str("export VXD_DDB.1\n");   // a VxD's single required export is its DDB
    write_lf(lnk, lnk_text);
    const py::str out = py::os::path::join(d, name.lower() + py::str(".vxd"));
    if (py::os::path::exists(out)) py::os::unlink(out);   // (a .vxd an earlier run left would pass for this link's result)
    {
        py::subprocess::RunOptions o;
        o.cwd = d;
        o.env = env;
        py::subprocess::run(py::list<py::str>{_WLINK(), py::str("@link.lnk")}, o);
    }
    if (py::os::path::exists(out)) {
        res.ok = true;
        res.output = out;
        res.bytes = py::os::path::getsize(out);
        res.objects = objs.size();
        return res;
    }
    res.error = py::str("link produced no .vxd (missing DDB export?)");
    return res;
}

std::tuple<std::optional<int64_t>, py::list<std::tuple<py::str, int64_t, int64_t>>> _w3_components(const py::bytes& data) {
    const int64_t lfa = py::unpack_from1<int64_t>("<I", data, 0x3C);
    if (data.slice(lfa, lfa + 2) != py::bytes("W3")) return std::make_tuple(std::nullopt, py::list<std::tuple<py::str, int64_t, int64_t>>());
    const int64_t count = py::unpack_from1<int64_t>("<I", data, lfa + 4);
    py::list<std::tuple<py::str, int64_t, int64_t>> recs;
    for (int64_t i = 0; i < count; ++i) {
        const int64_t e = lfa + 16 + i * 16;
        const py::bytes raw = data.slice(e, e + 8);
        const std::string s = raw.raw();
        const py::bytes first(s.substr(0, s.find('\0')));
        const py::str name = first.decode("latin1").strip();
        const auto os_ = py::unpack_from<int64_t, int64_t>("<II", data, e + 8);
        recs.append(std::make_tuple(name, std::get<0>(os_), std::get<1>(os_)));
    }
    return std::make_tuple(std::optional<int64_t>(lfa), recs);
}

W3Result build_w3(const py::str& w3_path, const py::str& outroot) {
    W3Result res;
    const py::bytes data = read_all(w3_path);
    const auto comps = _w3_components(data);
    if (!std::get<0>(comps)) {
        res.error = py::str("not a W3 bundle");
        return res;
    }
    const int64_t lfa = *std::get<0>(comps);
    const auto& recs = std::get<1>(comps);
    std::string front = data.slice(0, lfa).raw();   // MZ + real-mode loader, preserved
    py::list<std::tuple<py::str, BuildResult>> built;
    for (const auto& rec : recs) {
        const py::str& name = std::get<0>(rec);
        const int64_t off = std::get<1>(rec);
        // the LE module runs from its offset to the next module (or EOF)
        int64_t end = data.size();
        for (const auto& r2 : recs)
            if (std::get<1>(r2) > off) end = std::min(end, std::get<1>(r2));
        const py::str wd = py::tempfile::mkdtemp();
        const py::str modpath = py::os::path::join(wd, py::str("module.vxd"));
        write_all(modpath, data.slice(off, end).raw());
        try {
            for (const auto& nv : le::open_vxds(modpath)) {
                (void)nv;
                built.append(std::make_tuple(name, build_vxd(modpath, name, outroot)));
                break;
            }
        } catch (...) {
            py::shutil::rmtree(wd, true);
            throw;
        }
        py::shutil::rmtree(wd, true);
    }
    bool all_ok = true;
    for (const auto& b : built) all_ok = all_ok && std::get<1>(b).ok;
    if (!all_ok) {
        res.error = py::str("component build failed");
        for (const auto& b : built) res.status.append(std::make_tuple(std::get<0>(b), std::optional<bool>(std::get<1>(b).ok)));
        return res;
    }
    // assemble the W3: front + header + records + page-aligned LE modules
    const int64_t hdr_size = 16 + recs.size() * 16;
    const int64_t body_start = (lfa + hdr_size + 0x3FF) & ~int64_t(0x3FF);   // page-align the first module
    std::string out = front;
    out += "W3";
    out += py::pack("<HI", int64_t(0x0400), recs.size()).raw();
    out += std::string(8, '\0');   // 16-byte header
    const int64_t rec_off = static_cast<int64_t>(out.size());
    out += std::string(static_cast<size_t>(recs.size() * 16), '\0');   // placeholder records
    while (static_cast<int64_t>(out.size()) < body_start) out += '\0';
    py::list<std::tuple<py::str, int64_t, int64_t>> records;
    for (const auto& b : built) {
        const py::bytes mod = read_all(*std::get<1>(b).output);
        const int64_t moff = static_cast<int64_t>(out.size());
        out += mod.raw();
        while (out.size() % 0x400) out += '\0';   // page-align next module
        records.append(std::make_tuple(std::get<0>(b), moff, mod.size()));
    }
    for (int64_t i = 0; i < records.size(); ++i) {
        const int64_t e = rec_off + i * 16;
        std::string nm = std::get<0>(records[i]).encode("latin1").raw().substr(0, 8);
        nm.resize(8, ' ');
        out.replace(static_cast<size_t>(e), 8, nm);
        const std::string pk = py::pack("<II", std::get<1>(records[i]), std::get<2>(records[i])).raw();
        out.replace(static_cast<size_t>(e + 8), 8, pk);
    }
    const py::str outp = py::os::path::join(outroot, py::os::path::basename(w3_path).lower());
    py::os::makedirs(outroot, 0777, true);
    write_all(outp, out);
    res.ok = true;
    res.output = outp;
    res.bytes = static_cast<int64_t>(out.size());
    for (const auto& b : built) res.components.append(std::get<0>(b));
    return res;
}

namespace {
const char* const USAGE = "usage: vxd_owbuild [-h] [--out OUT] vxd\n";
}

int64_t main(const py::list<py::str>& argv, std::ostream& out, std::ostream& err) {
    std::optional<py::str> vxd, outarg;
    auto fail = [&](const std::string& msg) {
        err << USAGE << "vxd_owbuild: error: " << msg << "\n";
        return int64_t(2);
    };
    for (int64_t i = 0; i < argv.size(); ++i) {
        const py::str a = argv[i];
        if (a == py::str("-h") || a == py::str("--help")) {
            out << USAGE;
            return 0;
        }
        if (a == py::str("--out")) {
            if (i + 1 >= argv.size() || (argv[i + 1].startswith(py::str("-")) && py::len(argv[i + 1]) > 1)) return fail("argument --out: expected one argument");
            outarg = argv[++i];
        } else if (a.startswith(py::str("--out="))) {
            outarg = a.slice(6, py::len(a));
        } else if (!a.empty() && a.at_(0) == U'-' && py::len(a) > 1) {
            return fail("unrecognized arguments: " + a.utf8());
        } else if (!vxd) {
            vxd = a;
        } else {
            return fail("unrecognized arguments: " + a.utf8());
        }
    }
    if (!vxd) return fail("the following arguments are required: vxd");
    py::str path = *vxd;
    if (!py::os::path::isfile(path)) {
        const std::optional<py::str> r = base::corpus::retail(*vxd);   // a name the corpus knows
        if (r && !r->empty()) path = *r;
    }
    py::str outdir;
    if (outarg && !outarg->empty()) {
        outdir = *outarg;
    } else {
        outdir = py::os::path::join(py::settings::get_str(py::str("state")), py::str("vxd_ow"));   // settings.state("vxd_ow")
        py::os::makedirs(py::os::path::dirname(outdir), 0777, true);                                   // (which makes its parent)
    }
    for (const auto& nv : le::open_vxds(path)) {
        const py::str base = py::split(std::get<0>(nv), py::str("."))[0];
        const BuildResult r = build_vxd(path, base, outdir);
        out << base.utf8() << ": "
            << (r.ok ? py::percent(py::str("built %d bytes (%d objs) -> %s"), r.bytes, r.objects, *r.output).utf8()
                     : "FAIL " + (r.error ? r.error->utf8() : std::string("")))
            << "\n";
        break;
    }
    return 0;
}

}  // namespace codebreaker::source::vxd::owbuild
