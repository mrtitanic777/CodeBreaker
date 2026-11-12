// codebreaker/source/vxd/masm.hpp -- SHIM over cb/source/vxd/masm.hpp (docs/CPP_DESIGN.md 6.3): assemble-and-compare for a recovered VxD object.
// The old types in (VxDDisasm, its rows, py::set offsets), the new emitters and the new byte-exact test called, the old types out. Deleted
// with its last py:: consumer (link.hpp).
//
//     LEFile v = LEFile::from_file("vkd.vxd");
//     CompareObject r = compare_object(v, 1);      // r.match, r.oracle_len, r.symbolic_insns ...
//
// What the shim carries that is not conversion, and why: the DOS lane (`_stage_toolchain`, `_tnt_ml`, `assemble`, and the DOSBox batches of
// `assemble_coff_batch`) is the old build/dos_tools and build/native_dos, which are not refactored yet and cannot be called from the new code;
// it is handed to the new toolchain as its `period` assembler. It goes with those two modules.
//
// Differences from the old port that the shim cannot hide:
//   * The assemblers' source files get the line ends the text has (the Python's text-mode write turned each CRLF into CR CR LF).
//   * `_converge_range`, `_NASM_PROBE`, `_assemble_coff_nasm`, `_assemble_coff_native_ml`, `_ERRLINE` and the string and pattern constants are
//     not offered: nothing used them. `find_nasm` probes the same candidates (settings `nasm`, NASM_MASM, nasm-masm and nasm on PATH).
//   * `native_ml()` returns the NT 3.51 DDK's ML only when it exists (else the Win95 DDK's, as before: a program only DOS can run).
//   * `compare_object`'s `D` is the old recovery, made again; `force_raw_set` is in offset order.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/source/vxd/masm.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/dos_tools.hpp"
#include "codebreaker/build/native_dos.hpp"
#include "codebreaker/source/vxd/disassembly.hpp"
#include "codebreaker/source/vxd/symbols.hpp"
#include "py.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"
#include "py_stdlib.hpp"

namespace codebreaker::source::vxd::masm {

using codebreaker::source::vxd::disassembly::Row;
using codebreaker::source::vxd::disassembly::VxDDisasm;
namespace le = codebreaker::formats::le;
namespace cbm = cb::source::vxd::masm;
namespace dis = codebreaker::source::vxd::disassembly;

using Asm = std::tuple<std::optional<py::bytes>, py::str>;   // (object file or None, the assembler's log)

// ML.EXE (DDK 6.11c) is a Phar Lap TNT executable -- it needs DOSXNT.EXE, and the mount path must be space-free (DOSBox `mount` breaks on spaces).
// All three are staged into one cached space-free dir.
inline py::str _TC() { return py::os::path::join(py::os::getenv(py::str("TEMP"), py::str("/tmp")), py::str("vxdtc")); }
inline const py::list<py::str> _ML_REF = {py::str("Microsoft Windows 95 DDK"), py::str("MASM611C")};
inline const py::str _DOSXNT_SRC = py::str("toolchains/period/compilers/msvc-1.52c/BIN/DOSXNT.EXE");

// Locate the 'Microsoft Windows 95 DDK' dir (holds INC32 / MSVC20 / MASM611C), robust to a design_root() that points at the checkouts' parent.
inline py::str _ddk_root() {
    py::list<py::str> cands;
    cands.append(base::corpus::reference(py::list<py::str>{py::str("Microsoft Windows 95 DDK")}));
    cands.append(py::os::path::join(py::os::path::dirname(py::settings::checkout_root()), py::str("Windows95"), py::str("design"), py::str("reference"),
                                    py::str("Microsoft Windows 95 DDK")));
    for (const py::str& c : cands) {
        if (!c.empty() && py::os::path::isdir(c)) return c;
    }
    return cands[0];
}

// Path to a NATIVE (Win32) ML.EXE -- MASM 6.11d from the NT 3.51 DDK -- or the Win95 DDK's ML.
inline py::str native_ml() {
    const py::str cand = py::os::path::join(py::settings::checkout_root(), py::str("toolchains"), py::str("period"), py::str("sdks"), py::str("nt351-ddk"),
                                            py::str("DDK"), py::str("BIN"), py::str("I386"), py::str("FREE"), py::str("ML.EXE"));
    return py::os::path::exists(cand) ? cand : py::os::path::join(_ddk_root(), py::str("MASM611C"), py::str("ML.EXE"));
}

inline py::str _stage_toolchain(const py::str& root) {
    const py::str tc = _TC();
    py::os::makedirs(tc, 0777, true);
    const py::str ml = base::corpus::reference(_ML_REF);
    for (const py::str& src : {py::os::path::join(ml, py::str("ML.EXE")), py::os::path::join(ml, py::str("ML.ERR")), py::os::path::join(root, _DOSXNT_SRC)}) {
        const py::str dst = py::os::path::join(tc, py::os::path::basename(src));
        if (!py::os::path::exists(dst) && py::os::path::exists(src)) py::shutil::copy(src, dst);
    }
    return tc;
}

// The Phar Lap TNT ML.EXE on CodeBreaker's own DOS executive, or None to use DOSBox.
inline std::optional<Asm> _tnt_ml(const py::str& args, const py::str& masm_text, const py::str& root) {
    namespace nd = codebreaker::build::native_dos;
    if (!nd::enabled(py::str("ml"))) return std::nullopt;
    py::dict<py::str, nd::FileData> files;
    files.set(py::str("m.asm"), nd::FileData(masm_text));
    try {
        auto [made, log] = nd::ml(args, files, py::list<py::str>{py::str("m.obj")}, root);
        return Asm{made.get(py::str("m.obj")), log};
    } catch (const nd::NativeDosError&) {
        return std::nullopt;
    }
}

namespace shim_ {

inline py::str read_replace(const py::str& path) {
    py::file f = py::open(path, py::str("r"), py::str("cp1252"), py::str("replace"));
    py::str t = f.read_text();
    f.close();
    return t;
}

// MASM text through the period ML under DOS: one source per call (`/c /Fo`), or a batch of many in as few DOSBox sessions as it can
// (`budget` characters and `maxfiles` files a session; a larger text runs alone).
inline std::vector<Asm> period_batch(const std::vector<py::str>& texts, const py::str& root, int64_t budget = 300000, int64_t maxfiles = 32) {
    namespace dt = codebreaker::build::dos_tools;
    std::vector<Asm> results(texts.size(), Asm{std::nullopt, py::str("")});
    std::vector<size_t> rest;
    for (size_t i = 0; i < texts.size(); ++i) {   // TNT ML on our own executive: no cold start
        if (auto got = _tnt_ml(py::str("-coff -c -Fom.obj m.asm"), texts[i], root)) results[i] = *got;
        else rest.push_back(i);
    }
    if (rest.empty()) return results;
    const py::str tc = _stage_toolchain(root);
    std::sort(rest.begin(), rest.end(), [&](size_t a, size_t b) { return texts[a].size() < texts[b].size(); });
    std::vector<std::vector<size_t>> batches;
    std::vector<size_t> cur;
    int64_t cl = 0;
    for (const size_t i : rest) {
        const int64_t sz = texts[i].size();
        if (sz > budget) {
            batches.push_back({i});
            continue;
        }
        if (!cur.empty() && (cl + sz > budget || static_cast<int64_t>(cur.size()) >= maxfiles)) {
            batches.push_back(cur);
            cur.clear();
            cl = 0;
        }
        cur.push_back(i);
        cl += sz;
    }
    if (!cur.empty()) batches.push_back(cur);
    for (const auto& batch : batches) {
        py::dict<py::str, dt::FileData> files;
        py::list<py::str> bat, arts;
        int64_t total = 0;
        for (size_t j = 0; j < batch.size(); ++j) {
            const py::str fn = py::percent(py::str("m%d"), static_cast<int64_t>(j));
            files.set(fn + py::str(".asm"), dt::FileData(texts[batch[j]]));
            bat.append(py::percent(py::str("c:\\ml.exe -coff -c -Fo%s.obj %s.asm > %s.txt"), fn, fn, fn));
            arts.append(fn + py::str(".obj"));
            arts.append(fn + py::str(".txt"));
            total += static_cast<int64_t>(texts[batch[j]].size());
        }
        dt::Mounts mounts;
        mounts.append({py::str("c"), tc});
        auto [ok, out, work] = dt::run(mounts, bat, arts, static_cast<double>(std::min<int64_t>(600, 90 + total / 400)), files);
        for (size_t j = 0; j < batch.size(); ++j) {
            const py::bytes log = out.get(py::str(py::percent(py::str("m%d.txt"), static_cast<int64_t>(j))), py::bytes());
            results[batch[j]] = Asm{out.get(py::str(py::percent(py::str("m%d.obj"), static_cast<int64_t>(j)))), !log.empty() ? log.decode("latin1", "replace") : py::str("")};
        }
        if (work) py::shutil::rmtree(*work, true);
    }
    return results;
}

// The toolchain as the old layer's settings and corpus find it: the configured nasm, the DDK, and the DOS lane as the period assembler.
inline cbm::Toolchain toolchain(const py::str& root) {
    cbm::PeriodAssembler period = [root](std::span<const std::string> texts) {
        std::vector<py::str> in;
        for (const std::string& t : texts) in.push_back(cb::compat::to_py(t));
        std::vector<cbm::Assembled> out;
        for (const Asm& a : period_batch(in, root)) {
            cbm::Assembled made;
            if (std::get<0>(a)) made.object = cb::compat::from_py(*std::get<0>(a));
            made.log = cb::compat::from_py(std::get<1>(a));
            out.push_back(std::move(made));
        }
        return out;
    };
    const auto nasm = py::settings::get_opt_str(py::str("nasm"));
    return cbm::Toolchain::standard(symbols::detail::ddk(), cb::fs::from_text(py::settings::checkout_root().utf8()),
                                    nasm ? std::optional<std::string>(nasm->utf8()) : std::nullopt, period);
}

inline Asm asm_of(const cbm::Assembled& a) {
    return Asm{a.object ? std::optional<py::bytes>(cb::compat::to_py(cb::ByteView(*a.object))) : std::nullopt, cb::compat::to_py(a.log)};
}

// The old recovery as the new emitters work on it (the labels and instructions the caller may have changed included).
inline cbm::Emission emission_of(const VxDDisasm& D) {
    cbm::EmissionParts parts;
    if (D.ddb) {
        const le::Ddb& d = *D.ddb;
        cb::formats::le::Ddb n;
        n.object = static_cast<uint32_t>(d.obj);
        n.rva = d.rva;
        n.version = static_cast<uint16_t>(d.version);
        n.device_id = static_cast<uint16_t>(d.device_id);
        n.major = static_cast<uint8_t>(d.major);
        n.minor = static_cast<uint8_t>(d.minor);
        n.flags = static_cast<uint16_t>(d.flags);
        n.name = d.name.utf8();
        n.init_order = static_cast<uint32_t>(d.init_order);
        n.control_proc = static_cast<uint32_t>(d.control_proc);
        parts.ddb = n;
    }
    for (const auto& kv : D.fx.items()) {
        const dis::Place& p = std::get<0>(kv);
        parts.fixups.insert_or_assign(cb::source::vxd::Place{static_cast<uint32_t>(std::get<0>(p)), std::get<1>(p)}, dis::shim_::new_fixup(std::get<1>(kv)));
    }
    for (const auto& kv : D.code.items()) {
        const size_t index = static_cast<size_t>(std::get<0>(kv));
        if (parts.code.size() < index) parts.code.resize(index);
        parts.code[index - 1] = cb::compat::from_py(std::get<1>(kv));
    }
    for (const auto& kv : D.insns.items()) {
        const dis::Place& p = std::get<0>(kv);
        const Row& r = std::get<1>(kv);
        cb::source::vxd::Instruction insn;
        insn.size = static_cast<uint32_t>(r.size);
        insn.mnemonic = r.mnem.utf8();
        insn.operands = r.ops.utf8();
        insn.raw = cb::compat::from_py(r.raw);
        if (r.fx) insn.fixup = dis::shim_::new_fixup(*r.fx);
        if (r.vxdcall) insn.service = cb::source::vxd::ServiceCall{static_cast<uint32_t>(std::get<0>(*r.vxdcall)), static_cast<uint32_t>(std::get<1>(*r.vxdcall)), std::get<2>(*r.vxdcall) == py::str("import")};
        parts.instructions[cb::source::vxd::Place{static_cast<uint32_t>(std::get<0>(p)), std::get<1>(p)}] = std::move(insn);
    }
    for (const auto& kv : D.labels.items()) {
        const dis::Place& p = std::get<0>(kv);
        parts.labels[cb::source::vxd::Place{static_cast<uint32_t>(std::get<0>(p)), std::get<1>(p)}] = std::get<1>(kv).utf8();
    }
    for (const auto& kv : D.jtables.items()) {
        const dis::Place& p = std::get<0>(kv);
        std::vector<cb::source::vxd::Place> targets;
        for (const dis::Place& t : std::get<1>(kv)) targets.push_back({static_cast<uint32_t>(std::get<0>(t)), std::get<1>(t)});
        parts.jump_tables[cb::source::vxd::Place{static_cast<uint32_t>(std::get<0>(p)), std::get<1>(p)}] = std::move(targets);
    }
    return cbm::Emission(D.v->module(), dis::shim_::names(), std::move(parts));
}

inline cb::source::vxd::Instruction instruction_of(const Row& r) {
    cb::source::vxd::Instruction insn;
    insn.size = static_cast<uint32_t>(r.size);
    insn.mnemonic = r.mnem.utf8();
    insn.operands = r.ops.utf8();
    insn.raw = cb::compat::from_py(r.raw);
    if (r.fx) insn.fixup = dis::shim_::new_fixup(*r.fx);
    if (r.vxdcall) insn.service = cb::source::vxd::ServiceCall{static_cast<uint32_t>(std::get<0>(*r.vxdcall)), static_cast<uint32_t>(std::get<1>(*r.vxdcall)), std::get<2>(*r.vxdcall) == py::str("import")};
    return insn;
}

}  // namespace shim_

// ---- MASM text for a single instruction
inline py::str _masm_ops(const py::str& ops) { return cb::compat::to_py(cbm::masm_operands(cb::compat::from_py(ops))); }

// MASM text for one instruction, or None if it must be emitted as raw bytes instead.
inline std::optional<py::str> _masm_line(const VxDDisasm& D, int64_t oi, const Row& row) {
    return cb::compat::guard([&]() -> std::optional<py::str> {
        const cbm::Emission code = shim_::emission_of(D);
        const auto line = cbm::masm_line(code, static_cast<uint32_t>(oi), shim_::instruction_of(row));
        return line ? std::optional<py::str>(cb::compat::to_py(*line)) : std::nullopt;
    });
}

inline py::str _db(const py::bytes& bs) { return cb::compat::to_py(cbm::db_line(cb::compat::from_py(bs))); }
inline bool has_abs_mem(const Row& row) { return cbm::has_absolute_memory(shim_::instruction_of(row)); }
inline bool has_seg_prefix(const Row& row) { return cbm::has_segment_prefix(shim_::instruction_of(row)); }

using LineMap = py::dict<int64_t, int64_t>;

// The body lines of one object, and the map from a source line (base_line + index, 1-based) to the instruction offset.
inline std::tuple<py::list<py::str>, LineMap> object_body(const VxDDisasm& D, int64_t oi, const std::optional<py::set<int64_t>>& force_raw = std::nullopt,
                                                          int64_t base_line = 0, const std::optional<py::dict<int64_t, py::str>>& extra_labels = std::nullopt,
                                                          bool probe = false, const std::optional<std::tuple<int64_t, int64_t>>& crange = std::nullopt) {
    return cb::compat::guard([&] {
        cbm::BodyOptions options;
        if (force_raw) {
            for (const int64_t f : *force_raw) options.force_raw.insert(static_cast<uint32_t>(f));
        }
        options.base_line = static_cast<std::size_t>(base_line);
        if (extra_labels) {
            for (const auto& kv : extra_labels->items()) options.extra_labels[static_cast<uint32_t>(std::get<0>(kv))] = std::get<1>(kv).utf8();
        }
        options.probe = probe;
        if (crange) options.range = std::make_pair(static_cast<uint32_t>(std::get<0>(*crange)), static_cast<uint32_t>(std::get<1>(*crange)));
        const cbm::Emission code = shim_::emission_of(D);
        const cbm::Body body = cbm::object_body(code, static_cast<uint32_t>(oi), options);
        LineMap map;
        for (const auto& [line, offset] : body.line_map) map.set(static_cast<int64_t>(line), int64_t{offset});
        return std::make_tuple(cb::compat::to_py(body.lines), map);
    });
}

// Assemble to a COFF object (so section bytes and a symbol table are available). (obj_bytes, log).
inline Asm assemble_coff(const py::str& masm_text, const py::str& root) {
    return cb::compat::guard([&] { return shim_::asm_of(cbm::assemble_coff(shim_::toolchain(root), cb::compat::from_py(masm_text))); });
}

inline py::list<Asm> assemble_coff_batch(const py::list<py::str>& masm_list, const py::str& root, int64_t = 300000, int64_t = 32) {
    return cb::compat::guard([&] {
        std::vector<std::string> texts;
        for (const py::str& m : masm_list) texts.push_back(cb::compat::from_py(m));
        py::list<Asm> out;
        for (const cbm::Assembled& a : cbm::assemble_coff_batch(shim_::toolchain(root), texts)) out.append(shim_::asm_of(a));
        return out;
    });
}

// From a COFF object with one code section, (section_bytes, {symbol_name: offset}).
inline std::tuple<py::bytes, py::dict<py::str, int64_t>> coff_text_syms(const py::bytes& obj) {
    return cb::compat::guard([&] {
        const cbm::CoffText found = cbm::coff_text(cb::compat::from_py(obj));
        py::dict<py::str, int64_t> syms;
        for (const auto& [name, value] : found.symbols) syms.set(cb::compat::to_py(name), int64_t{value});
        return std::make_tuple(cb::compat::to_py(cb::ByteView(found.text)), syms);
    });
}

// compare_object's dict
struct CompareObject {
    bool ok = true, match = false;
    int64_t oracle_len = 0, asm_len = 0, n_diffs = 0, symbolic_insns = 0, forced_raw = 0;
    py::set<int64_t> force_raw_set;
    bool raw_fallback = false;
    int64_t n_files = 0;
    py::bytes asm_bytes, oracle_bytes;
};

// Assemble-and-compare object oi against the oracle, maximising GENUINE re-encodes.
inline CompareObject compare_object(const le::LEFile& v, int64_t oi, const py::str& root = py::str("."), int64_t max_repair = 8, int64_t max_ins = 6000) {
    return cb::compat::guard([&] {
        const cbm::Toolchain tools = shim_::toolchain(py::os::path::abspath(root));
        const cb::source::vxd::Disassembly found = cb::source::vxd::Disassembly::run(v.module(), dis::shim_::names());
        const cbm::Emission code(found, dis::shim_::names());
        const cbm::ObjectReport r = cbm::compare_object(code, static_cast<uint32_t>(oi), tools, static_cast<size_t>(max_repair), static_cast<size_t>(max_ins));
        CompareObject out;
        out.match = r.match;
        out.oracle_len = static_cast<int64_t>(r.original_length);
        out.asm_len = static_cast<int64_t>(r.assembled_length);
        out.n_diffs = r.match ? 0 : 1;
        out.symbolic_insns = static_cast<int64_t>(r.symbolic);
        out.forced_raw = static_cast<int64_t>(r.forced_raw);
        for (const uint32_t f : r.force_raw) out.force_raw_set.add(int64_t{f});
        out.raw_fallback = r.all_raw;
        out.n_files = static_cast<int64_t>(r.files);
        out.asm_bytes = cb::compat::to_py(cb::ByteView(r.assembled));
        out.oracle_bytes = cb::compat::to_py(cb::ByteView(r.original));
        return out;
    });
}

struct ObjectReport {
    bool ok = true;
    int64_t obj = 0;
    bool match = false;
    int64_t oracle_len = 0, asm_len = 0, n_diffs = 0, symbolic_insns = 0, forced_raw = 0;
    py::set<int64_t> force_raw_set;
    int64_t n_files = 0;
    bool raw_fallback = false;
    py::bytes asm_bytes, oracle_bytes;
};
struct ModuleReport {
    bool byte_exact = false;
    py::list<ObjectReport> objects;
    int64_t symbolic = 0, forced_raw = 0, raw_fallback_objs = 0;
};

// Assemble-and-compare every executable object of a VxD.
inline ModuleReport compare_module(const le::LEFile& v, const py::str& root = py::str("."), int64_t max_repair = 8, int64_t max_ins = 6000) {
    return cb::compat::guard([&] {
        const cbm::Toolchain tools = shim_::toolchain(py::os::path::abspath(root));
        const cb::source::vxd::Disassembly found = cb::source::vxd::Disassembly::run(v.module(), dis::shim_::names());
        const cbm::Emission code(found, dis::shim_::names());
        const cbm::ModuleReport r = cbm::compare_module(code, tools, static_cast<size_t>(max_repair), static_cast<size_t>(max_ins));
        ModuleReport out;
        out.byte_exact = r.byte_exact;
        out.symbolic = static_cast<int64_t>(r.symbolic);
        out.forced_raw = static_cast<int64_t>(r.forced_raw);
        out.raw_fallback_objs = static_cast<int64_t>(r.all_raw_objects);
        for (const cbm::ObjectReport& o : r.objects) {
            ObjectReport n;
            n.obj = o.object;
            n.match = o.match;
            n.oracle_len = static_cast<int64_t>(o.original_length);
            n.asm_len = static_cast<int64_t>(o.assembled_length);
            n.n_diffs = o.match ? 0 : 1;
            n.symbolic_insns = static_cast<int64_t>(o.symbolic);
            n.forced_raw = static_cast<int64_t>(o.forced_raw);
            for (const uint32_t f : o.force_raw) n.force_raw_set.add(int64_t{f});
            n.n_files = static_cast<int64_t>(o.files);
            n.raw_fallback = o.all_raw;
            n.asm_bytes = cb::compat::to_py(cb::ByteView(o.assembled));
            n.oracle_bytes = cb::compat::to_py(cb::ByteView(o.original));
            out.objects.append(n);
        }
        return out;
    });
}

}  // namespace codebreaker::source::vxd::masm
