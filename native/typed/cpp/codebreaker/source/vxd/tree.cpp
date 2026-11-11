// codebreaker/source/vxd/tree.cpp -- typed C++ of codebreaker.source.vxd.tree.

#include "codebreaker/source/vxd/tree.hpp"

#include <algorithm>
#include <ostream>
#include <vector>

#include "codebreaker/formats/le.hpp"
#include "codebreaker/source/vxd/masm.hpp"
#include "py_settings.hpp"
#include "py_stdlib.hpp"
#include "py_subprocess.hpp"

namespace codebreaker::source::vxd::tree {

namespace vxd_masm = codebreaker::source::vxd::masm;

const py::str _ERRLINE(":(\\d+):\\s*(?:error|fatal)");
const py::str _ERRLINE_WASM("\\((\\d+)\\):\\s*Error");

namespace {

const py::re::Pattern& errline() {
    static const py::re::Pattern p = py::re::compile(_ERRLINE, py::re::I);
    return p;
}
const py::re::Pattern& errline_wasm() {
    static const py::re::Pattern p = py::re::compile(_ERRLINE_WASM, py::re::I);
    return p;
}

// open(path, "w", newline="").write(text)
void write_raw(const py::str& path, const py::str& text) {
    py::file f = py::open(path, py::str("w"), py::str("cp1252"), std::nullopt, py::str(""));
    f.write(text);
    f.close();
}

}  // namespace

Probe _nasm_probe(const py::str& nasm) {
    return [nasm](const py::str& path) -> ProbeResult {
        const py::subprocess::CompletedProcess r =
            py::subprocess::run(py::list<py::str>{nasm, py::str("--masm"), py::str("-f"), py::str("bin"), path, py::str("-o"), py::str("nul")});
        return ProbeResult(r.returncode, r.stderr_ ? *r.stderr_ : py::str(""), errline());
    };
}

Probe _wasm_probe(const py::str& wasm) {
    return [wasm](const py::str& path) -> ProbeResult {
        const py::str wd = py::tempfile::mkdtemp();   // (the object goes into a folder of its own, removed after)
        const py::str obj = py::os::path::join(wd, py::str("probe.obj"));
        py::subprocess::CompletedProcess r;
        try {
            r = py::subprocess::run(py::list<py::str>{wasm, py::str("-zcm=masm"), py::str("-q"), py::str("-fo=") + obj, path});
        } catch (...) {
            py::shutil::rmtree(wd, true);
            throw;
        }
        py::shutil::rmtree(wd, true);
        return ProbeResult(r.returncode, (r.stdout_ ? *r.stdout_ : py::str("")) + (r.stderr_ ? *r.stderr_ : py::str("")), errline_wasm());
    };
}

py::list<py::str> _converge_buildable(const VxDDisasm& D, int64_t oi, int64_t lo, int64_t hi, const py::dict<int64_t, py::str>& extra_labels,
                                      const py::list<py::str>& head, const py::list<py::str>& tail, const Probe& probe, int64_t maxit) {
    py::set<int64_t> force;
    py::list<py::str> body;
    // write the source to a file of its own and run the probe on it (the file goes away after)
    auto probe_text = [&](const py::list<py::str>& lines) -> ProbeResult {
        const py::str wd = py::tempfile::mkdtemp();
        const py::str path = py::os::path::join(wd, py::str("m.asm"));
        write_raw(path, py::join(py::str("\r\n"), lines));
        try {
            ProbeResult r = probe(path);
            py::shutil::rmtree(wd, true);
            return r;
        } catch (...) {
            py::shutil::rmtree(wd, true);
            throw;
        }
    };
    auto all_lines = [&](const py::list<py::str>& b) { return head + b + tail; };   // (a new list: a py::list copy shares its lines)
    for (int64_t it = 0; it < maxit; ++it) {
        auto ob = vxd_masm::object_body(D, oi, std::optional<py::set<int64_t>>(force), head.size(), std::optional<py::dict<int64_t, py::str>>(extra_labels),
                                        false, std::make_tuple(lo, hi));
        body = std::get<0>(ob);
        const vxd_masm::LineMap& linemap = std::get<1>(ob);
        const ProbeResult r = probe_text(all_lines(body));
        if (std::get<0>(r) == 0) return body;
        py::set<int64_t> lines;
        for (const auto& m : std::get<2>(r).finditer(std::get<1>(r))) lines.add(py::int_parse(m.group(1)));
        py::set<int64_t> mapped;
        for (int64_t L : lines)
            if (linemap.contains(L)) mapped.add(linemap.at(L));
        const py::set<int64_t> fresh = mapped - force;
        if (fresh.empty()) break;   // error not pinnable per-instruction -> full-raw fallback
        force.update(fresh);
    }
    // Last resort: an error the linemap can't pin (e.g. a re-encode-drift SHORT jump reported at its TARGET label, not the jump).
    // Byte-preserve the WHOLE object -- all db, still byte-exact -- so it assembles. Only reached for the rare object that won't
    // converge per-instruction.
    py::set<int64_t> alloff;
    for (const auto& k : D.insns.keys())
        if (std::get<0>(k) == oi && lo <= std::get<1>(k) && std::get<1>(k) < hi) alloff.add(std::get<1>(k));
    const py::set<int64_t> forced = force | alloff;
    auto raw = vxd_masm::object_body(D, oi, std::optional<py::set<int64_t>>(forced), head.size(), std::optional<py::dict<int64_t, py::str>>(extra_labels),
                                     false, std::make_tuple(lo, hi));
    const py::list<py::str> body_raw = std::get<0>(raw);
    const ProbeResult r = probe_text(all_lines(body_raw));
    return std::get<0>(r) == 0 ? body_raw : body;
}

std::tuple<py::str, py::str> seg_class(int64_t flags) {
    if (!(flags & 0x04)) {   // not executable -> data
        return (flags & 0x10) ? std::make_tuple(py::str("IDATA"), py::str("_IDATA")) : std::make_tuple(py::str("LDATA"), py::str("_LDATA"));
    }
    if (flags & 0x8000) return std::make_tuple(py::str("MCODE"), py::str("_LMSGTABLE"));   // IOPL -> message-hook table
    if (flags & 0x10) return std::make_tuple(py::str("ICODE"), py::str("_ITEXT"));         // discardable -> init code
    if (flags & 0x40) return std::make_tuple(py::str("LCODE"), py::str("_LTEXT"));         // preload+locked
    return std::make_tuple(py::str("PCODE"), py::str("_PTEXT"));                           // pageable
}

std::optional<std::tuple<int64_t, int64_t>> _entrytab_ddb(const le::LEFile& v) {
    const py::bytes& raw = v.d();
    const int64_t et = v.entrytab;
    if (et <= 0 || et + 9 > raw.size()) return std::nullopt;
    const int64_t cnt = raw[et], typ = raw[et + 1];
    if (cnt < 1 || typ != 3) return std::nullopt;   // 32-bit entry bundle
    const int64_t obj = py::unpack_from1<int64_t>("<H", raw, et + 2);
    const int64_t rva = py::unpack_from1<int64_t>("<I", raw, et + 5);   // ordinal-1 entry: flags[1] + offset[4]
    return std::make_tuple(obj, rva);
}

py::list<py::str> emit_tree(const le::LEFile& v, const py::str& name, const py::str& outdir, int64_t max_ins, const std::optional<py::str>& probe_nasm,
                            const std::optional<py::str>& probe_wasm, const std::optional<link::ForceRaw>& force_raw_map) {
    Probe probe;
    if (probe_nasm && !probe_nasm->empty()) probe = _nasm_probe(*probe_nasm);
    else if (probe_wasm && !probe_wasm->empty()) probe = _wasm_probe(*probe_wasm);
    VxDDisasm D(v);
    D.run();
    const py::str d = py::os::path::join(outdir, name.lower());
    py::os::makedirs(d, 0777, true);
    const std::optional<le::Ddb>& db = D.ddb;
    // DDB location for the exported VXD_DDB label: prefer the heuristic scan, but fall back to the LE entry table's ordinal-1 export
    // when it misses (e.g. VMM itself).
    std::optional<int64_t> ddb_obj, ddb_rva;
    if (db) {
        ddb_obj = db->obj;
        ddb_rva = db->rva;
    }
    if (!ddb_obj) {
        if (const auto et = _entrytab_ddb(v)) {
            ddb_obj = std::get<0>(*et);
            ddb_rva = std::get<1>(*et);
        }
    }
    py::list<py::str> written;
    py::list<py::str> index = {py::percent(py::str("; %s -- recovered VxD source tree (multi-file, by segment + function)"), name), py::str(";")};
    if (db) {
        index.append(py::percent(py::str("; Declare_Virtual_Device %s, %d, %d, %s_Control, 0%04Xh"), name, db->major, db->minor, name, db->device_id));
        index.append(py::percent(py::str("; DDK version 0%03Xh   init_order 0%08Xh"), db->version, db->init_order));
        index.append(py::str(";"));
    }
    index.extend(py::list<py::str>{py::str("; NOTE: a stripped LE binary carries no original file or (mostly) function names;"),
                                   py::str("; files are named by segment class and role. Exported service names, control-dispatch"),
                                   py::str("; handler names, and -- for the 12 core VxDs with a debug .SYM -- internal function"),
                                   py::str("; names are the real DDK names. Other internals are sub_<offset>."), py::str(""), py::str("; files:")});
    for (const le::Object& o : v.objects) {
        const int64_t oi = o.idx;
        const auto cs = seg_class(o.flags);
        const py::str& cls = std::get<0>(cs);
        const py::str& seg = std::get<1>(cs);
        int64_t n_insns = 0;
        for (const auto& k : D.insns.keys())
            if (std::get<0>(k) == oi) n_insns += 1;
        py::list<std::tuple<int64_t, int64_t>> ranges;
        if (n_insns <= max_ins) ranges.append(std::make_tuple(int64_t(0), D.code.at(oi).size()));
        else ranges = vxd_masm::split_ranges(D, oi, max_ins);
        for (int64_t i = 0; i < ranges.size(); ++i) {
            const int64_t lo = std::get<0>(ranges[i]), hi = std::get<1>(ranges[i]);
            const py::str suffix = ranges.size() == 1 ? py::str("") : py::percent(py::str("_%02d"), i + 1);
            const py::str fn = py::percent(py::str("%s_%s_%02d%s.asm"), name.lower(), cls.lower(), oi, suffix);
            py::dict<int64_t, py::str> extra;
            if (ddb_obj && *ddb_obj == oi && ddb_rva && lo <= *ddb_rva && *ddb_rva < hi) extra.set(*ddb_rva, py::str("VXD_DDB"));
            py::list<py::str> head = {py::percent(py::str("; %s -- %s object %d segment %s%s"), fn, name, oi, seg,
                                                  ranges.size() > 1 ? py::percent(py::str("  (part %d/%d)"), i + 1, ranges.size()) : py::str("")),
                                      py::str(".386p"), py::percent(py::str("%s segment dword public use32 '%s'"), seg, cls), py::str("")};
            py::list<py::str> tail = {py::percent(py::str("%s ends"), seg), py::str("        end"), py::str("")};
            py::list<py::str> body;
            if (force_raw_map) {   // byte-exact build: db the drift set
                py::set<int64_t> fr;
                if (force_raw_map->contains(oi))
                    for (int64_t a : force_raw_map->at(oi))
                        if (lo <= a && a < hi) fr.add(a);
                body = std::get<0>(vxd_masm::object_body(D, oi, std::optional<py::set<int64_t>>(fr), 0, std::optional<py::dict<int64_t, py::str>>(extra), false,
                                                         std::make_tuple(lo, hi)));
            } else if (probe) {
                body = _converge_buildable(D, oi, lo, hi, extra, head, tail, probe);
            } else {
                body = std::get<0>(vxd_masm::object_body(D, oi, std::nullopt, 0, std::optional<py::dict<int64_t, py::str>>(extra), false, std::make_tuple(lo, hi)));
            }
            const py::list<py::str> all = head + body + tail;
            write_raw(py::os::path::join(d, fn), py::join(py::str("\r\n"), all));
            written.append(fn);
            index.append(py::percent(py::str(";   %-28s object %d %s [0x%x-0x%x)"), fn, oi, seg, lo, hi));
        }
    }
    write_raw(py::os::path::join(d, py::percent(py::str("%s.txt"), name.lower())), py::join(py::str("\r\n"), index) + py::str("\r\n"));
    return written;
}

int64_t main(const py::list<py::str>& argv, std::ostream& out, std::ostream& err) {
    (void)err;
    if (argv.size() < 1) throw py::IndexError("list index out of range");
    const py::str path = argv[0];
    py::str outdir;
    if (argv.size() > 1) {
        outdir = argv[1];
    } else {
        outdir = py::os::path::join(py::settings::get_str(py::str("state")), py::str("vxd"), py::str("tree"));
        py::os::makedirs(py::os::path::dirname(outdir), 0777, true);
    }
    for (const auto& nv : le::open_vxds(path)) {
        const py::str base = py::split(std::get<0>(nv), py::str("."))[0];
        const py::list<py::str> files = emit_tree(std::get<1>(nv), base.upper(), outdir);
        out << py::percent(py::str("%-14s -> %d files in %s/%s"), std::get<0>(nv), files.size(), outdir, base.lower()).utf8() << "\n";
    }
    return 0;
}

}  // namespace codebreaker::source::vxd::tree
