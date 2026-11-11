// codebreaker/formats/pe_functions.hpp -- compat shim over cb/formats/pe_functions.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module itself is native/cb/formats/pe_functions.{hpp,cpp}: the 32-bit function carve (starts from exports, call
// targets and prologues; extents; resolved calls; data references). This header keeps the old py:: surface for the ports
// that have not been refactored yet (analyze/image, emulate/emulator, symbols/names, verify/lanes/win32 ...) and is
// deleted with the last of them. Conversion and forwarding only, except the three places that need code that is not
// refactored yet and stay here tied to it:
//   COMPAT(py): module names     -- the PeCarve(path) / carve(module) / PeSeg(module) constructors resolve a corpus name
//                                   with base::corpus::resolve; remove when base.corpus is refactored
//   COMPAT(py): ensure_target    -- reads the old settings and changes directory; remove when base.settings is refactored
//   COMPAT(py): the carve cache  -- `carve(module)` keeps one PeCarve per module in a function-local map
#pragma once

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "cb/compat/py_bridge.hpp"
#include "cb/formats/pe_functions.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/formats/pe.hpp"
#include "py.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::formats::pe_functions {

namespace cf = cb::formats::pe_functions;

// capstone.dll is found beside the executable now (cb::disasm::capstone); nothing to load from a path.
inline void set_capstone_dll(const py::str&) {}

// capstone.x86 operand types
constexpr int64_t X86_OP_IMM = 2;
constexpr int64_t X86_OP_MEM = 3;

// one operand of Capstone's x86 detail: its type, an immediate, and a memory operand's base/index/disp
struct Operand {
    int64_t type = 0;        // X86_OP_INVALID 0, X86_OP_REG 1, X86_OP_IMM 2, X86_OP_MEM 3
    int64_t imm = 0;         // operand.imm (signed)
    int64_t base = 0;        // operand.mem.base (0: none)
    int64_t index = 0;       // operand.mem.index (0: none)
    int64_t disp = 0;        // operand.mem.disp (signed)
};

// Capstone's instruction (CsInsn) with its operands
struct Instruction {
    int64_t address = 0, size = 0;
    py::str mnemonic, op_str;
    std::vector<Operand> operands;
    int64_t disp = 0;
    int64_t disp_size = 0, disp_offset = 0;
    int64_t imm_size = 0, imm_offset = 0;
};

// (mnemonic, op_str, size, Capstone's instruction, call target)
struct Record {
    py::str mnemonic, op_str;
    int64_t size = 0;
    Instruction ins;
    std::optional<int64_t> tgt;       // the immediate of a `call imm`
};

// the function PeCarve carved at `start`
struct Func {
    int64_t start = 0, end = 0;
    std::optional<py::str> name;
};

// (base, bytes) of the section holding an address; base is None (and bytes null) when there is none
struct SecOf {
    std::optional<int64_t> base;
    const py::bytes* data = nullptr;
};

namespace detail {

inline Instruction instruction_of(const cf::Instruction& c) {
    Instruction out;
    out.address = c.address;
    out.size = c.size;
    out.mnemonic = cb::compat::to_py(c.mnemonic);
    out.op_str = cb::compat::to_py(c.op_str);
    for (const cf::Operand& o : c.operands) {
        Operand op;
        op.type = static_cast<int64_t>(o.kind);
        op.imm = o.imm;
        op.base = o.base;
        op.index = o.index;
        op.disp = o.disp;
        out.operands.push_back(op);
    }
    out.disp = c.disp;
    out.disp_size = c.disp_size;
    out.disp_offset = c.disp_offset;
    out.imm_size = c.imm_size;
    out.imm_offset = c.imm_offset;
    return out;
}

inline cf::Instruction instruction_to(const Instruction& o) {
    cf::Instruction out;
    out.address = static_cast<cf::Va>(o.address);
    out.size = static_cast<std::uint8_t>(o.size);
    out.mnemonic = cb::compat::from_py(o.mnemonic);
    out.op_str = cb::compat::from_py(o.op_str);
    for (const Operand& p : o.operands) {
        cf::Operand op;
        op.kind = static_cast<cf::OperandKind>(p.type);
        op.imm = p.imm;
        op.base = static_cast<std::uint32_t>(p.base);
        op.index = static_cast<std::uint32_t>(p.index);
        op.disp = p.disp;
        out.operands.push_back(op);
    }
    return out;
}

inline Record record_of(const cf::Instruction& c) {
    Record r;
    r.ins = instruction_of(c);
    r.mnemonic = r.ins.mnemonic;
    r.op_str = r.ins.op_str;
    r.size = r.ins.size;
    if (auto t = c.call_target()) r.tgt = static_cast<int64_t>(*t);
    return r;
}

inline Func func_of(const cf::Function& f) {
    Func out;
    out.start = f.start;
    out.end = f.end;
    if (f.name) out.name = cb::compat::to_py(*f.name);
    return out;
}

}  // namespace detail

class PeCarve;

// {address: (mnemonic, op_str, size, Capstone's instruction, call target)} of every address the sweep decoded: the
// addresses come from the sweep, an instruction is decoded when it is asked for.
class Insns {
  public:
    explicit Insns(std::shared_ptr<const cf::PeCarve> carve) : carve_(std::move(carve)) {}
    bool contains(int64_t va) const { return 0 <= va && va <= 0xFFFFFFFFll && carve_->has_instruction(static_cast<cf::Va>(va)); }
    int64_t size() const { return static_cast<int64_t>(carve_->instruction_addresses().size()); }
    // the addresses, ascending (__iter__ / keys)
    std::vector<int64_t> keys() const { return std::vector<int64_t>(carve_->instruction_addresses().begin(), carve_->instruction_addresses().end()); }
    // self[va]: KeyError when the address was not decoded or lies in no section; StopIteration when Capstone
    // decodes nothing there
    Record at(int64_t va) const {
        if (!contains(va) || carve_->section_of(static_cast<cf::Va>(va)) == nullptr) throw py::KeyError(std::to_string(va));
        auto ins = carve_->instruction_at(static_cast<cf::Va>(va));
        if (!ins) throw py::StopIteration();
        return detail::record_of(*ins);
    }
    // .get(va): None when the address was not decoded
    std::optional<Record> get(int64_t va) const {
        if (!contains(va)) return std::nullopt;
        return at(va);
    }
    std::vector<std::pair<int64_t, Record>> items() const {
        std::vector<std::pair<int64_t, Record>> out;
        for (int64_t va : keys()) out.emplace_back(va, at(va));
        return out;
    }
    std::vector<Record> values() const {
        std::vector<Record> out;
        for (int64_t va : keys()) out.push_back(at(va));
        return out;
    }

  private:
    std::shared_ptr<const cf::PeCarve> carve_;
};

class PeCarve {
  public:
    // progress_cb(bytes_swept) is called when the sweep is done
    explicit PeCarve(const py::str& path, std::function<void(int64_t)> progress_cb = nullptr)
        : PeCarve(base::corpus::resolve(path), progress_cb, 0) {}
    PeCarve(const PeCarve&) = delete;
    PeCarve& operator=(const PeCarve&) = delete;

    std::function<void(int64_t)> _progress_cb;
    std::shared_ptr<pe::PEFile> pe;
    py::list<std::tuple<uint64_t, py::bytes, py::str>> sections;     // [(va, bytes, name)]
    py::dict<uint64_t, std::tuple<py::str, py::str>> imports;        // {iat_va: (dll, name)}
    py::dict<int64_t, py::str> exports;                              // export_va -> name
    py::set<int64_t> call_targets;
    std::shared_ptr<Insns> insn;
    py::dict<int64_t, Func> funcs;                                   // start -> function (ascending start)
    py::set<int64_t> starts;

    // the carve this one forwards to
    std::shared_ptr<const cf::PeCarve> impl;

    // -- section helpers
    SecOf _sec_of(int64_t va) const {
        for (const auto& s : sections) {
            int64_t base = static_cast<int64_t>(std::get<0>(s));
            if (base <= va && va < base + std::get<1>(s).size()) return {base, &std::get<1>(s)};
        }
        return {};
    }

    // -- queries
    // A capstone call/jmp insn -> a name, or None if not a call/branch.
    std::optional<py::str> resolve_call(const Instruction& ins) const {
        auto name = impl->resolve_call(detail::instruction_to(ins));
        if (!name) return std::nullopt;
        return cb::compat::to_py(*name);
    }
    // Resolved call/jmp targets inside the function carved at `start`.
    py::list<py::str> calls_in(int64_t start) const {
        if (start < 0 || start > 0xFFFFFFFFll) return py::list<py::str>();
        return cb::compat::to_py(impl->calls_in(static_cast<cf::Va>(start)));
    }
    py::list<std::tuple<int64_t, py::str, py::str>> disasm_func(int64_t start) const {
        py::list<std::tuple<int64_t, py::str, py::str>> rows;
        if (start < 0 || start > 0xFFFFFFFFll) return rows;
        for (const cf::DisasmLine& l : impl->disassemble(static_cast<cf::Va>(start)))
            rows.append(std::make_tuple(static_cast<int64_t>(l.address), cb::compat::to_py(l.mnemonic), cb::compat::to_py(l.op_str)));
        return rows;
    }

    // -- data cross-references
    py::list<std::tuple<int64_t, int64_t>> _image_ranges() const {
        py::list<std::tuple<int64_t, int64_t>> r;
        int64_t image_base = static_cast<int64_t>(impl->image().header().image_base);
        for (const auto& s : impl->image().sections())
            r.append(std::make_tuple(image_base + s.virtual_address, image_base + s.virtual_address + std::max(s.virtual_size, s.raw_size)));
        return r;
    }
    bool _in_image(int64_t va) const { return va >= 0 && impl->in_image(static_cast<std::uint64_t>(va)); }
    // {func_start: sorted[referenced data VA]}.
    const py::dict<int64_t, py::list<int64_t>>& data_refs() const {
        if (!drefs_) drefs_ = cb::compat::to_py(impl->data_refs());
        return *drefs_;
    }
    // Sorted function-starts that reference data address `va`.
    py::list<int64_t> xrefs_to(int64_t va) const {
        if (va < 0 || va > 0xFFFFFFFFll) return py::list<int64_t>();
        return cb::compat::to_py(impl->xrefs_to(static_cast<cf::Va>(va)));
    }
    // {func_start: [(va, text), ...]} -- the strings each function references; `va2text` is a {VA: string} map.
    py::dict<int64_t, py::list<std::tuple<int64_t, py::str>>> string_refs(const py::dict<int64_t, py::str>& va2text) const {
        std::unordered_map<cf::Va, std::string> texts;
        for (const auto& kv : va2text.items())
            if (std::get<0>(kv) >= 0 && std::get<0>(kv) <= 0xFFFFFFFFll) texts.emplace(static_cast<cf::Va>(std::get<0>(kv)), cb::compat::from_py(std::get<1>(kv)));
        py::dict<int64_t, py::list<std::tuple<int64_t, py::str>>> out;
        for (const auto& [fs, hits] : impl->string_refs(texts)) {
            py::list<std::tuple<int64_t, py::str>> list;
            for (const auto& [va, text] : hits) list.append(std::make_tuple(static_cast<int64_t>(va), cb::compat::to_py(text)));
            out.set(fs, list);
        }
        return out;
    }

    // A carve that already exists (PeSeg over a carve it was handed).
    PeCarve(std::shared_ptr<const cf::PeCarve> carved, const py::str& path, std::function<void(int64_t)> progress_cb) : _progress_cb(progress_cb), impl(std::move(carved)) {
        fill_(path);
    }

  private:
    PeCarve(const py::str& resolved, std::function<void(int64_t)> progress_cb, int) : _progress_cb(progress_cb) {
        cb::compat::guard([&] { impl = std::make_shared<const cf::PeCarve>(cf::PeCarve::open(std::filesystem::path(cb::compat::from_py(resolved)))); });
        fill_(resolved);
        if (progress_cb) {
            int64_t total = 0;
            for (const auto& s : sections) total += std::get<1>(s).size();
            progress_cb(total);
        }
    }

    void fill_(const py::str& path) {
        pe = std::make_shared<pe::PEFile>(pe::open_pe(path));
        for (const cf::CodeSection& s : impl->sections())
            sections.append(std::make_tuple(static_cast<uint64_t>(s.va), cb::compat::to_py(s.data), cb::compat::to_py(s.name)));
        for (const auto& [slot, name] : impl->imports())
            imports.set(slot, std::make_tuple(cb::compat::to_py(name.dll), cb::compat::to_py(name.name)));
        for (const auto& [va, name] : impl->exports()) exports.set(va, cb::compat::to_py(name));
        for (cf::Va t : impl->call_targets()) call_targets.add(t);
        insn = std::make_shared<Insns>(impl);
        for (const auto& [start, f] : impl->functions()) {
            funcs.set(start, detail::func_of(f));
            starts.add(start);
        }
    }

    mutable std::optional<py::dict<int64_t, py::list<int64_t>>> drefs_;
};

// Point cores at win95 + cd into a data root, idempotently (mirrors msvc2cov._setup so a tool works with no `wt` wrapper).
// COMPAT(py): remove when base.settings is refactored
inline void ensure_target() {
    static const char* const eras[] = {"win95", "win98", "win3.1", "win1.0"};
    auto data_root = [&]() -> py::str {   // settings.data_root(): WT_DATA_ROOT when it holds era dirs (or is a directory), else the checkout's parent
        auto env = py::os::environ_::get(py::str("WT_DATA_ROOT"));
        if (env && !env->empty()) {
            for (const char* e : eras)
                if (py::os::path::isdir(py::os::path::join(*env, py::str(e)))) return *env;
            if (py::os::path::isdir(*env)) return *env;
        }
        return py::os::path::dirname(py::settings::checkout_root());
    };
    std::vector<py::str> roots;
    auto env = py::os::environ_::get(py::str("WT_DATA_ROOT"));
    if (env && !env->empty()) roots.push_back(py::os::path::join(*env, py::str("win95")));
    roots.push_back(data_root());
    roots.push_back(py::os::getcwd());
    for (const py::str& r : roots) {
        if (py::os::path::isdir(py::os::path::join(r, py::str("files"))) || py::os::path::isdir(py::os::path::join(r, py::str("src")))) {
            py::os::chdir(r);
            return;
        }
    }
}

// one PeCarve per module, kept
inline std::shared_ptr<PeCarve> carve(const py::str& module) {
    static py::dict<py::str, std::shared_ptr<PeCarve>> kept;       // _CARVE
    ensure_target();
    if (!kept.contains(module)) kept.set(module, std::make_shared<PeCarve>(base::corpus::resolve(module)));
    return kept.at(module);
}

// Linearly decode the blob as 32-bit code with capstone; return [(offset, asm_text), ...] with CONTIGUOUS coverage
// (undecodable bytes become a 1-byte '(bad)' row -> db). progress_cb(bytes_done) is called when the listing is done.
inline py::list<std::tuple<int64_t, py::str>> disasm_whole32(const py::bytes& code, int64_t base = 0, std::function<void(int64_t)> progress_cb = nullptr) {
    py::list<std::tuple<int64_t, py::str>> rows = cb::compat::guard([&] {
        py::list<std::tuple<int64_t, py::str>> out;
        const cb::Bytes bytes = cb::compat::from_py(code);
        for (const cf::ListingRow& r : cf::disassemble_listing(bytes, static_cast<cf::Va>(base)))
            out.append(std::make_tuple(static_cast<int64_t>(r.at), cb::compat::to_py(r.text)));
        return out;
    });
    if (progress_cb) progress_cb(static_cast<int64_t>(code.raw().size()));
    return rows;
}

// One PE executable section, shaped like trace.Seg for asmrecon32.
class PeSeg {
  public:
    // carve: a pre-built PeCarve to REUSE (a multi-section PE otherwise re-runs the whole capstone sweep once per section).
    explicit PeSeg(const py::str& module_, std::optional<py::str> section = std::nullopt, std::shared_ptr<PeCarve> carve_ = nullptr,
                   std::function<void(int64_t)> progress_cb = nullptr) {
        module = module_;
        path = base::corpus::resolve(module);
        cf::PeSeg seg = cb::compat::guard([&] {
            return cf::PeSeg::open(std::filesystem::path(cb::compat::from_py(path)), cb::compat::from_py(module),
                                   section ? std::optional<std::string>(cb::compat::from_py(*section)) : std::nullopt,
                                   carve_ ? carve_->impl : nullptr);
        });
        pe = std::make_shared<pe::PEFile>(pe::open_pe(path));
        base = pe->image_base;
        va = seg.va;
        code = cb::compat::to_py(cb::ByteView(seg.code));
        name = cb::compat::to_py(seg.name);
        secva = va;
        for (const cf::ListingRow& r : seg.rows) rows.append(std::make_tuple(static_cast<int64_t>(r.at), cb::compat::to_py(r.text)));
        if (progress_cb) progress_cb(static_cast<int64_t>(seg.code.size()));
        for (std::size_t i = 0; i < seg.rows.size(); ++i) off2idx.set(seg.rows[i].at, static_cast<int64_t>(i));
        for (const auto& [start, f] : seg.funcs) funcs.set(start, detail::func_of(f));
        for (const auto& [off, n] : seg.export_names) export_names.set(off, cb::compat::to_py(n));
        _carve = carve_ ? carve_ : std::make_shared<PeCarve>(seg.carve, path, nullptr);
        for (std::size_t i = 0; i < seg.reloc_mask.size(); ++i)
            if (seg.reloc_mask[i]) mask.add(static_cast<int64_t>(i));
    }

    py::str module, path;
    std::shared_ptr<pe::PEFile> pe;
    uint64_t base = 0;
    int64_t va = 0;
    py::bytes code;
    py::str name;
    int64_t secva = 0;                                   // image VA of section start
    py::list<std::tuple<int64_t, py::str>> rows;
    py::dict<int64_t, int64_t> off2idx;
    py::dict<int64_t, Func> funcs;
    py::dict<int64_t, py::str> export_names;
    std::shared_ptr<PeCarve> _carve;                     // kept for call resolution/annotate
    py::set<int64_t> mask;                               // base-relocation mask, section-relative
    py::dict<int64_t, py::str> fixups;                   // resolved call sites for annotate (site_off -> "DLL!name")
};

// {start: (end, exported name or None)} -- the functions PeCarve finds, by the same rule and with the same answer,
// without keeping an object for every instruction of the module.
inline py::dict<int64_t, std::tuple<int64_t, std::optional<py::str>>> function_spans(const py::str& path) {
    return cb::compat::guard([&] {
        py::dict<int64_t, std::tuple<int64_t, std::optional<py::str>>> out;
        const cb::formats::pe::Image image = cb::formats::pe::Image::open(std::filesystem::path(cb::compat::from_py(base::corpus::resolve(path))));
        for (const cf::Function& f : cf::function_spans(image))
            out.set(f.start, std::make_tuple(static_cast<int64_t>(f.end), f.name ? std::optional<py::str>(cb::compat::to_py(*f.name)) : std::nullopt));
        return out;
    });
}

}  // namespace codebreaker::formats::pe_functions
