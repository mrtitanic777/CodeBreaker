// codebreaker/source/vxd/disassembly.hpp -- SHIM over cb/source/vxd/disassembly.hpp (docs/CPP_DESIGN.md 6.3): the recursive-descent 32-bit
// disassembler and listing of Win95 VxDs. The old types in (the old LEFile, py::dict rows, labels and tables), the new recovery called;
// `VxDDisasm::run()` fills the old members from what the new `Disassembly` found. Deleted with its last py:: consumer.
//
// Differences from the old port that the shim cannot hide:
//   * The members `mark`, `data` and `entries` (the recovery's own bookkeeping of which bytes are code, known data, entry points) are not
//     offered: nothing read them. `D.insns`, `D.labels` and `D.jtables` are in order of place, where the old dicts were in the order the
//     descent found things.
//   * An internal fix-up with no target (a 16-bit selector) is no entry point, branch target, label or table entry; the old port stopped with
//     the TypeError the Python raised on it. `NONE_TARGET` is still the place key the consumers use for it.
//   * `emit_asm(D, v)` over a disassembly the caller changed is not offered (nothing used it); `emit_asm(v)` is.
//   * `set_capstone_dll` is accepted and ignored: Capstone is loaded by cb::disasm::capstone, beside the executable.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>
#include <unordered_set>

#include "cb/compat/py_bridge.hpp"
#include "cb/source/vxd/disassembly.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/formats/le.hpp"
#include "codebreaker/source/vxd/symbols.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::source::vxd::disassembly {

namespace le = codebreaker::formats::le;
namespace cbv = cb::source::vxd;
using Place = le::Place;   // (object, offset)

// the place that stands for (obj, None): see the notes above
constexpr int64_t NONE_TARGET = INT64_MIN;

struct PlaceHash {
    size_t operator()(const Place& p) const {
        return static_cast<size_t>(std::get<0>(p)) * 0x9E3779B97F4A7C15ull ^ (static_cast<size_t>(std::get<1>(p)) + 0x7F4A7C15u);
    }
};
using PlaceSet = std::unordered_set<Place, PlaceHash>;

inline void set_capstone_dll(const py::str&) {}

namespace shim_ {

// What the DDK names, as the old layer's settings and corpus find the DDK: the services, the control messages, the debug symbols.
inline const cbv::DdkNames& names() {
    static const cbv::DdkNames n = cbv::DdkNames::standard(symbols::detail::ddk(), base::corpus::detail::instance().design_dir());
    return n;
}

inline py::dict<int64_t, py::str> devices() {
    py::dict<int64_t, py::str> out;
    for (uint32_t id = 0; id <= 0x4FF; ++id) {
        if (const auto name = cbv::device_name(id)) out.set(id, cb::compat::to_py(*name));
    }
    return out;
}

inline le::Fixup old_fixup(const cb::formats::le::Fixup& f) { return le::conv::fixup(f); }

inline cb::formats::le::Fixup new_fixup(const le::Fixup& f) {
    if (const auto* i = std::get_if<le::InternalFixup>(&f)) {
        cb::formats::le::InternalFixup o;
        o.object = static_cast<uint32_t>(i->obj);
        if (i->target) o.target = static_cast<uint32_t>(*i->target);
        o.source_type = static_cast<uint8_t>(i->srctype);
        return o;
    }
    const auto& m = std::get<le::ImportFixup>(f);
    return cb::formats::le::ImportFixup{static_cast<uint32_t>(m.module), static_cast<uint32_t>(m.ordinal)};
}

}  // namespace shim_

// Canonical Win95 VxD device ids -> names (DDK VxD service tables).
inline const py::dict<int64_t, py::str> VXD_DEVICES = shim_::devices();

// DDB field offsets, relative to the DDB start (the DDB_Next dword).
inline const int64_t DDB_CONTROL_PROC = cbv::kDdbControlProc;
inline const int64_t DDB_V86_API_PROC = cbv::kDdbV86ApiProc;
inline const int64_t DDB_PM_API_PROC = cbv::kDdbPmApiProc;
inline const int64_t DDB_SERVICE_TABLE = cbv::kDdbServiceTable;
inline const int64_t DDB_SERVICE_SIZE = cbv::kDdbServiceSize;

inline const py::set<py::str> _TERMS = {py::str("ret"), py::str("retn"), py::str("retf"), py::str("iret"), py::str("iretd"), py::str("iretw")};
inline const py::set<py::str> _BRANCHES = {
    py::str("call"), py::str("jmp"), py::str("je"), py::str("jne"), py::str("jz"), py::str("jnz"), py::str("ja"),
    py::str("jae"), py::str("jb"), py::str("jbe"), py::str("jg"), py::str("jge"), py::str("jl"), py::str("jle"),
    py::str("jo"), py::str("jno"), py::str("js"), py::str("jns"), py::str("jp"), py::str("jnp"), py::str("jc"),
    py::str("jnc"), py::str("jcxz"), py::str("jecxz"), py::str("loop"), py::str("loope"), py::str("loopne")};

// [start, end) byte ranges in `code` that are ASCII string DATA the disassembler would otherwise decode as instructions.
inline py::list<std::tuple<int64_t, int64_t>> _string_ranges(const py::bytes& code) {
    py::list<std::tuple<int64_t, int64_t>> out;
    for (const auto& [from, to] : cbv::string_ranges(cb::compat::from_py(code))) out.append(std::make_tuple(static_cast<int64_t>(from), static_cast<int64_t>(to)));
    return out;
}

// one recovered instruction (what `insns` holds, as dicts)
struct Row {
    int64_t off = 0, size = 0;
    py::str mnem, ops;
    py::bytes raw;
    std::optional<le::Fixup> fx;                                        // the fix-up inside the instruction
    std::optional<std::tuple<int64_t, int64_t, py::str>> vxdcall;       // (device, service, via)
};

class VxDDisasm {
  public:
    explicit VxDDisasm(const le::LEFile& v_) : v(&v_), fx(v_.fixups()), ddb(v_.ddb()) {
        for (const le::Object& o : v_.objects) {
            code.set(o.idx, v_.object_bytes(o));
            objs.set(o.idx, o);
        }
    }

    const le::LEFile* v;
    py::dict<Place, le::Fixup> fx;
    std::optional<le::Ddb> ddb;
    py::dict<int64_t, py::bytes> code;
    py::dict<int64_t, le::Object> objs;
    py::dict<Place, Row> insns;                                         // (oi, off) -> row
    py::dict<Place, py::str> labels;                                    // (oi, off) -> label name
    py::dict<Place, py::list<Place>> jtables;                           // (oi, base) -> [(oi, target), ...]
    std::optional<std::tuple<int64_t, int64_t, int64_t>> svctab;        // (oi, base, count) if a service table was found

    std::optional<le::Fixup> fsite(int64_t oi, int64_t off) const { return fx.get(Place(oi, off)); }

    // Recover the driver (recursive descent from the DDB's entry points, the gap recovery, the labels, the DDK's names).
    VxDDisasm& run() { return run(shim_::names()); }
    // The same with the DDK's names given (nothing is renamed with `cbv::DdkNames()`).
    VxDDisasm& run(const cbv::DdkNames& names) {
        cb::compat::guard([&] {
            const cbv::Disassembly found = cbv::Disassembly::run(v->module(), names);
            for (const auto& [at, insn] : found.instructions()) {
                Row row;
                row.off = at.offset;
                row.size = insn.size;
                row.mnem = cb::compat::to_py(insn.mnemonic);
                row.ops = cb::compat::to_py(insn.operands);
                row.raw = cb::compat::to_py(cb::ByteView(insn.raw));
                if (insn.fixup) row.fx = shim_::old_fixup(*insn.fixup);
                if (insn.service) row.vxdcall = std::make_tuple(int64_t{insn.service->device}, int64_t{insn.service->service}, py::str(insn.service->from_import ? "import" : "inline"));
                insns.set(Place(at.object, at.offset), row);
            }
            for (const auto& [at, name] : found.labels()) labels.set(Place(at.object, at.offset), cb::compat::to_py(name));
            for (const auto& [base, targets] : found.jump_tables()) {
                py::list<Place> list;
                for (const cbv::Place& t : targets) list.append(Place(t.object, t.offset));
                jtables.set(Place(base.object, base.offset), list);
            }
            if (found.service_table()) svctab = std::make_tuple(int64_t{found.service_table()->object}, found.service_table()->base, static_cast<int64_t>(found.service_table()->count));
        });
        return *this;
    }
};

// The operand of a direct branch ("0x401000") as a number, or None.
inline std::optional<int64_t> _direct_target(const py::str& op_str) { return cbv::direct_operand(cb::compat::from_py(op_str)); }

inline std::optional<int64_t> _direct_target_row(const Row& row) {
    cbv::Instruction insn;
    insn.mnemonic = cb::compat::from_py(row.mnem);
    insn.operands = cb::compat::from_py(row.ops);
    return insn.direct_target();
}

inline py::str _svc_name(int64_t dev, int64_t svc, const py::str& via) {
    return cb::compat::to_py(cbv::service_text(cbv::ServiceCall{static_cast<uint32_t>(dev), static_cast<uint32_t>(svc), via == py::str("import")}, shim_::names()));
}

// Return the operand text of a row and whether it resolved to a label.
inline std::tuple<py::str, bool> _render_ops(const VxDDisasm& D, int64_t oi, const Row& row) {
    cbv::Instruction insn;
    insn.mnemonic = cb::compat::from_py(row.mnem);
    insn.operands = cb::compat::from_py(row.ops);
    if (row.fx) insn.fixup = shim_::new_fixup(*row.fx);
    const auto [text, resolved] = cbv::operand_text(insn, static_cast<uint32_t>(oi), [&](const cbv::Place& p) -> std::optional<std::string> {
        const auto label = D.labels.get(Place(p.object, p.offset));
        return label && !label->empty() ? std::optional<std::string>(label->utf8()) : std::nullopt;
    });
    return std::make_tuple(cb::compat::to_py(text), resolved);
}

// Emit annotated 32-bit assembly for the whole VxD (accepts an LEFile).
inline py::str emit_asm(const le::LEFile& s_v) {
    return cb::compat::guard([&] { return cb::compat::to_py(cbv::emit_asm(s_v.module(), shim_::names())); });
}

// verify()'s per-object answer
struct VerifyRow {
    int64_t obj = 0, n = 0, code = 0, data = 0, overlap = 0, overrun = 0;
};
inline py::list<VerifyRow> verify(const le::LEFile& v) {
    return cb::compat::guard([&] {
        py::list<VerifyRow> out;
        for (const cbv::ObjectCheck& c : cbv::verify(cbv::Disassembly::run(v.module(), shim_::names()))) {
            out.append(VerifyRow{c.object, static_cast<int64_t>(c.size), static_cast<int64_t>(c.code), static_cast<int64_t>(c.data), static_cast<int64_t>(c.overlap),
                                 static_cast<int64_t>(c.overrun)});
        }
        return out;
    });
}

// disassemble_file()'s per-module stat dict
struct FileStat {
    py::str name;
    int64_t insns = 0;
    double cov = 0;
    int64_t labels = 0, jtables = 0;
    std::optional<py::str> device;
    std::optional<int64_t> device_id;
};
inline py::list<FileStat> disassemble_file(const py::str& path, const std::optional<py::str>& outdir = std::nullopt) {
    return cb::compat::guard([&] {
        std::optional<std::filesystem::path> out_folder;
        if (outdir) out_folder = cb::fs::from_text(cb::compat::from_py(*outdir));
        py::list<FileStat> out;
        for (const cbv::ModuleStats& s : cbv::disassemble_file(cb::fs::from_text(cb::compat::from_py(path)), shim_::names(), out_folder)) {
            FileStat f;
            f.name = cb::compat::to_py(s.name);
            f.insns = static_cast<int64_t>(s.instructions);
            f.cov = s.coverage_percent;
            f.labels = static_cast<int64_t>(s.labels);
            f.jtables = static_cast<int64_t>(s.jump_tables);
            if (s.device) f.device = cb::compat::to_py(*s.device);
            if (s.device_id) f.device_id = static_cast<int64_t>(*s.device_id);
            out.append(f);
        }
        return out;
    });
}

}  // namespace codebreaker::source::vxd::disassembly
