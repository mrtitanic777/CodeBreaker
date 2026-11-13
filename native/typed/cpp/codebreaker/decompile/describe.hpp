// codebreaker/decompile/describe.hpp -- COMPAT SHIM (docs/CPP_DESIGN.md 6.3). The module now lives in
// native/cb/decompile/describe*.{hpp,cpp}; this header keeps the old py:: surface for the ports and the dynamic exe that still
// use it (the Description record with its py containers, the module-level BY_NUMBER / IN_ECX registries, the decoder stand-in).
// Conversion and forwarding only; what describing asks of the symbol tables is answered by forwarding to the old ports below
// (COMPAT(py): remove with the last of them). Deleted with its last py:: consumer.
//
// Not carried over: `_arch(machine)`'s caller-visible behaviour is unchanged; the experiment switch CB_POINTER_CALLS is gone
// (see decompile/describe_test.cpp); `Description` keeps the new description it was made from, so the functions that take one
// ask it directly -- a Description whose containers a caller edited afterwards is not re-read.
#pragma once

#include <cstdint>
#include <memory>
#include <mutex>
#include <optional>
#include <tuple>
#include <variant>

#include "cb/compat/py_bridge.hpp"
#include "cb/decompile/describe.hpp"
#include "codebreaker/analyze/conventions.hpp"
#include "codebreaker/analyze/dos_functions.hpp"
#include "codebreaker/analyze/image.hpp"
#include "codebreaker/build/watcom.hpp"
#include "codebreaker/core16_types.hpp"
#include "codebreaker/decompile/inline_asm.hpp"
#include "codebreaker/formats/dos.hpp"
#include "codebreaker/formats/le.hpp"
#include "codebreaker/formats/ne.hpp"
#include "codebreaker/formats/nes.hpp"
#include "codebreaker/formats/pe.hpp"
#include "codebreaker/formats/powerpc_image.hpp"
#include "codebreaker/symbols/api_arguments.hpp"
#include "codebreaker/symbols/driver_services.hpp"
#include "codebreaker/symbols/names.hpp"
#include "codebreaker/symbols/ordinals.hpp"
#include "py.hpp"

// the one function of the 32-bit lane this header calls (LANES["pe"].functions(path)); verify/lanes/win32.hpp itself reaches the whole decompiler
#include "codebreaker/verify/lanes/win32_functions.hpp"

namespace codebreaker::decompile::describe {

namespace nd = ::cb::decompile::describe;

using core16::FunctionNotes;
using core16::SegOff;

// --- the 16-bit address space -------------------------------------------------------------------------------------
inline constexpr int64_t FIRST_SELECTOR = nd::kFirstSelector;
inline constexpr int64_t EXTERN_SELECTOR = nd::kExternSelector;
inline constexpr int64_t IMPORT_SELECTOR = nd::kImportSelector;
inline constexpr int64_t LE_BASE = nd::kLeBase;
inline constexpr int64_t LE_STRIDE = nd::kLeStride;
inline constexpr int64_t LE_SLOTS = nd::kLeSlots;

inline const py::str ARCH = py::str(std::string(nd::kArchNe));
inline const py::str LE_ARCH = py::str(std::string(nd::kArchLe));
inline const py::str DOS_ARCH = py::str(std::string(nd::kArchDos));

inline std::optional<py::str> _arch(int64_t machine) {
    const auto lang = nd::language_of_machine(static_cast<std::uint16_t>(machine));
    if (!lang || machine < 0 || machine > 0xFFFF) return std::nullopt;
    return py::str(std::string(*lang));
}

// module file -> {the C name an import by number is called by: (MODULE, number, bytes of arguments it removes or None)}
inline py::dict<py::str, py::dict<py::str, std::tuple<py::str, int64_t, std::optional<int64_t>>>> BY_NUMBER;
// module file -> names of its functions that take an argument in ECX
inline py::dict<py::str, py::set<py::str>> IN_ECX;

inline int64_t selector(int64_t seg) { return nd::selector(seg); }
inline int64_t address(int64_t seg, int64_t off) { return nd::address(seg, off); }
inline SegOff where(int64_t va) {
    const nd::SegOff w = nd::where(va);
    return SegOff{static_cast<int64_t>(w.seg), static_cast<int64_t>(w.off)};
}
inline int64_t le_address(int64_t obj, int64_t off) { return nd::le_address(obj, off); }

// --- decoded instructions ---------------------------------------------------------------------------------------------
struct CsError : py::Exception {
    explicit CsError(const std::string& m) : py::Exception(m) {}
    const char* type() const override { return "CsError"; }
};
// (the decoder loads capstone.dll itself now: there is nothing to hand over)
inline void set_capstone_dll(const py::str&) {}

struct Insn : inline_asm::Instruction {
    py::list<int64_t> op_size;      // operands[k].size
    py::list<int64_t> op_access;    // (not kept by the new decoder: 0)
    py::list<int64_t> regs_read;    // regs_access()[0]
    py::list<int64_t> regs_write;   // regs_access()[1]
    py::list<int64_t> groups;       // the generic groups the instruction is in
    bool regs_ok = false;
    std::string mn, os;
    std::tuple<py::list<int64_t>, py::list<int64_t>> regs_access() const {
        if (!regs_ok) throw CsError("Information irrelevant for this instruction (CS_ERR_DETAIL)");
        return {regs_read, regs_write};
    }
    bool group(int64_t g) const {
        for (int64_t x : groups)
            if (x == g) return true;
        return false;
    }
};
inline constexpr int64_t CS_GRP_JUMP = 1;
inline constexpr int64_t CS_GRP_CALL = 2;
inline constexpr int64_t CS_GRP_RET = 3;
inline constexpr int64_t CS_GRP_INT = 4;
inline constexpr int64_t CS_GRP_IRET = 5;

namespace shim_detail {

inline Insn to_old(const nd::Insn& n) {
    Insn i;
    i.address = static_cast<uint64_t>(n.address);
    i.size = n.size;
    i.mn = n.mn;
    i.os = n.op_str;
    i.mnemonic = py::str(n.mn);
    i.op_str = py::str(n.op_str);
    i.bytes = ::cb::compat::to_py(::cb::ByteView(n.bytes));
    for (int k = 0; k < 4; ++k) i.prefix.append(0);
    for (const nd::Operand& o : n.operands) {
        inline_asm::Operand op;
        op.type = o.kind == nd::OperandKind::Register ? inline_asm::X86_OP_REG
                  : o.kind == nd::OperandKind::Immediate ? inline_asm::X86_OP_IMM
                  : o.kind == nd::OperandKind::Memory ? inline_asm::X86_OP_MEM
                                                      : inline_asm::X86_OP_INVALID;
        op.reg = o.reg;
        op.imm = o.imm;
        op.mem = {o.segment, o.base, o.index, 0, o.disp};
        i.operands.append(op);
        i.op_size.append(o.size);
        i.op_access.append(0);
    }
    for (std::uint16_t r : n.regs_read) i.regs_read.append(r);
    for (std::uint16_t r : n.regs_write) i.regs_write.append(r);
    for (unsigned g = 1; g < 32; ++g)
        if (n.in_group(g)) i.groups.append(g);
    i.regs_ok = n.regs_ok;
    return i;
}
inline py::list<Insn> to_old(const std::vector<nd::Insn>& v) {
    py::list<Insn> out;
    for (const nd::Insn& n : v) out.append(to_old(n));
    return out;
}
inline nd::Insn to_new(const Insn& i) {
    nd::Insn n;
    n.address = static_cast<int64_t>(i.address);
    n.size = i.size;
    n.mn = i.mn;
    n.op_str = i.os;
    n.bytes = ::cb::compat::from_py(i.bytes);
    for (std::size_t k = 0; k < i.operands.size(); ++k) {
        const inline_asm::Operand& o = i.operands.vec()[k];
        nd::Operand op;
        op.kind = o.type == inline_asm::X86_OP_REG ? nd::OperandKind::Register
                  : o.type == inline_asm::X86_OP_IMM ? nd::OperandKind::Immediate
                  : o.type == inline_asm::X86_OP_MEM ? nd::OperandKind::Memory
                                                     : nd::OperandKind::Invalid;
        op.reg = static_cast<std::uint32_t>(o.reg);
        op.imm = o.imm;
        op.segment = static_cast<std::uint32_t>(o.mem.segment);
        op.base = static_cast<std::uint32_t>(o.mem.base);
        op.index = static_cast<std::uint32_t>(o.mem.index);
        op.disp = o.mem.disp;
        op.size = k < i.op_size.size() ? static_cast<int32_t>(i.op_size.vec()[k]) : 0;
        n.operands.push_back(op);
    }
    for (int64_t r : i.regs_read) n.regs_read.push_back(static_cast<std::uint16_t>(r));
    for (int64_t r : i.regs_write) n.regs_write.push_back(static_cast<std::uint16_t>(r));
    for (int64_t g : i.groups)
        if (g > 0 && g < 32) n.groups |= 1u << g;
    n.regs_ok = i.regs_ok;
    return n;
}
inline std::vector<nd::Insn> to_new(const py::list<Insn>& ins) {
    std::vector<nd::Insn> out;
    for (const Insn& i : ins) out.push_back(to_new(i));
    return out;
}
inline nd::Distance distance_of(const py::str& s) { return s == py::str("far") ? nd::Distance::Far : nd::Distance::Near; }
inline py::str distance_text(nd::Distance d) { return py::str(std::string(nd::distance_name(d))); }
inline nd::SegOff new_place(const SegOff& s) { return nd::SegOff{static_cast<std::uint32_t>(std::get<0>(s)), static_cast<std::uint32_t>(std::get<1>(s))}; }
inline SegOff old_place(const nd::SegOff& s) { return SegOff{static_cast<int64_t>(s.seg), static_cast<int64_t>(s.off)}; }

// What describing asks of the symbol tables, answered by the old ports (COMPAT(py): remove with them).
class OldKnowledge : public nd::Knowledge {
public:
    std::optional<std::int64_t> win16_argument_bytes(std::string_view api) override { return symbols::api_arguments::of(py::str(std::string(api))); }
    std::optional<std::int64_t> win32_argument_bytes(std::string_view module, std::string_view api) override {
        return symbols::api_arguments::stack_bytes(py::str(std::string(module)), py::str(std::string(api)));
    }
    std::optional<std::int64_t> declared_argument_bytes(std::string_view name) override { return symbols::api_arguments::header_bytes(py::str(std::string(name))); }
    std::optional<std::string> import_name(std::string_view module, std::uint32_t ordinal) override {
        const std::optional<py::str> n = symbols::ordinals::identifier(py::str(std::string(module)), ordinal);
        return n ? std::optional<std::string>(n->utf8()) : std::nullopt;
    }
    std::string driver_service_label(std::uint32_t device, std::uint32_t service) override { return symbols::driver_services::label(device, service).utf8(); }
    std::set<std::string> imported_names(const std::filesystem::path& module) override {
        std::set<std::string> out;
        for (const py::str& k : build::watcom::import_map(py::str(module.string())).keys()) out.insert(k.utf8());
        return out;
    }
    Conventions conventions(const std::filesystem::path& module) override {
        Conventions c;
        auto img = analyze::image::load(py::str(module.string()));
        auto found = analyze::conventions::functions(*img);
        for (const auto& kv : found->items()) {
            const auto& fact = std::get<1>(kv);
            if (fact.convention && *fact.convention == "thiscall") c.thiscall.insert(std::get<0>(kv));
            if (fact.convention && *fact.convention == "fastcall") c.fastcall.insert(std::get<0>(kv));
        }
        return c;
    }
    std::optional<std::int64_t> pe_function_pop(const std::filesystem::path& module, std::uint32_t rva) override {
        if (!mod_) mod_ = std::make_shared<symbols::api_arguments::_Mod>(py::str(module.string()));
        try {
            return symbols::api_arguments::_scan(*mod_, rva, 4);
        } catch (const py::ImportError&) {
            throw;   // (no capstone is not "no pop count")
        } catch (const std::exception&) {
            return std::nullopt;
        }
    }

private:
    std::shared_ptr<symbols::api_arguments::_Mod> mod_;
};

}  // namespace shim_detail

// --- decoders ---------------------------------------------------------------------------------------------------------
class Decoder {
public:
    virtual py::list<Insn> disasm(const py::bytes& code, int64_t address) const = 0;
    virtual py::str reg_name(int64_t reg) const = 0;
    virtual ~Decoder() = default;
};
namespace shim_detail {
class PlainDecoder : public Decoder {
public:
    explicit PlainDecoder(bool detail) : detail_(detail) {}
    py::list<Insn> disasm(const py::bytes& code, int64_t address) const override {
        return ::cb::compat::guard([&] { return to_old(nd::decode16(::cb::compat::from_py(code), address, detail_)); });
    }
    py::str reg_name(int64_t reg) const override { return py::str(nd::register_name16(static_cast<std::uint32_t>(reg))); }

private:
    bool detail_;
};
}  // namespace shim_detail
inline std::shared_ptr<const Decoder> decoder16(bool detail = false) {
    static std::shared_ptr<const Decoder> plain = std::make_shared<shim_detail::PlainDecoder>(false);
    static std::shared_ptr<const Decoder> rich = std::make_shared<shim_detail::PlainDecoder>(true);
    return detail ? rich : plain;
}
// The instruction at `at`, or None. The decoded instructions of a segment are kept by the segment's bytes (the same segment
// gives the same object: tests/test_verification.py asks).
inline std::shared_ptr<const Insn> _one(const py::bytes& code, int64_t at) {
    struct Held {
        std::string bytes;
        std::unique_ptr<nd::Code16> decoded;
        std::map<int64_t, std::shared_ptr<const Insn>> seen;
    };
    static std::mutex m;
    static std::map<std::pair<const void*, size_t>, std::unique_ptr<Held>> held;
    std::lock_guard<std::mutex> hold(m);
    auto& slot = held[{code.raw().data(), code.raw().size()}];
    if (!slot || slot->bytes != code.raw()) {
        slot = std::make_unique<Held>();
        slot->bytes = code.raw();
        slot->decoded = std::make_unique<nd::Code16>(::cb::compat::from_py(code));
    }
    const auto it = slot->seen.find(at);
    if (it != slot->seen.end()) return it->second;
    const nd::Insn* i = ::cb::compat::guard([&] { return slot->decoded->one(at); });
    return slot->seen[at] = i ? std::make_shared<const Insn>(shim_detail::to_old(*i)) : nullptr;
}

// --- what describes a module ------------------------------------------------------------------------------------
class Description {
  public:
    py::str path;
    py::dict<int64_t, py::str> names;
    py::dict<int64_t, SegOff> where;
    py::list<py::str> imports;
    py::list<py::str> lines;
    py::dict<py::str, FunctionNotes> notes;

    // --- NE (describe16)
    py::set<SegOff> far_called;
    py::dict<py::str, py::str> import_names;
    py::dict<int64_t, std::tuple<py::str, py::str>> selector_imports;
    py::dict<int64_t, int64_t> code;
    py::dict<int64_t, py::bytes> images;
    int64_t data_segment = 0;
    py::dict<int64_t, py::bytes> segment_bytes;
    py::dict<int64_t, int64_t> results;
    py::set<int64_t> inlined;

    // --- LE (describe_le) and DOS (describe_dos)
    py::dict<int64_t, std::tuple<int64_t, int64_t>> spans;
    std::optional<py::str> device;

    // --- DOS (describe_dos)
    py::bytes image;
    int64_t base = 0;
    std::optional<formats::dos::Program> program;
    std::shared_ptr<analyze::dos_functions::Analysis> analysis;
    py::dict<py::str, py::list<analyze::dos_functions::Interrupt>> interrupts;

    // the new description the 16-bit facts above were made from (what the functions below ask)
    std::shared_ptr<nd::NeDescription> impl;

    Description(const py::str& path_, const py::dict<int64_t, py::str>& names_, const py::dict<int64_t, SegOff>& where_,
                const py::list<py::str>& imports_)
        : path(path_), names(names_), where(where_), imports(imports_) {}

    py::str write(const std::optional<py::str>& to = std::nullopt) {
        return ::cb::compat::guard([&] {
            nd::Description d;
            d.path = std::filesystem::path(path.utf8());
            for (const py::str& l : lines) d.lines.push_back(l.utf8());
            d.write(to && !to->empty() ? std::optional<std::filesystem::path>(to->utf8()) : std::nullopt);
            path = py::str(d.path.string());
            return path;
        });
    }
    py::list<py::list<py::str>> records(const py::list<py::str>& kinds) const {
        nd::Description d;
        for (const py::str& l : lines) d.lines.push_back(l.utf8());
        std::vector<std::string> wanted;
        for (const py::str& k : kinds) wanted.push_back(k.utf8());
        std::vector<std::string_view> views(wanted.begin(), wanted.end());
        py::list<py::list<py::str>> out;
        for (const auto& rec : d.records(views)) {
            py::list<py::str> parts;
            for (const std::string& f : rec) parts.append(py::str(f));
            out.append(parts);
        }
        return out;
    }
};

namespace shim_detail {

inline Description from_new(const nd::NeDescription& n) {
    Description d(py::str(n.path.string()), ::cb::compat::to_py(n.names), py::dict<int64_t, SegOff>(), ::cb::compat::to_py(n.imports));
    for (const auto& [va, w] : n.where_map) d.where.set(va, old_place(w));
    for (const std::string& l : n.lines) d.lines.append(py::str(l));
    for (const nd::SegOff& p : n.far_called) d.far_called.add(old_place(p));
    d.import_names = ::cb::compat::to_py(n.import_names);
    for (const auto& [v, si] : n.selector_imports)
        d.selector_imports.set(v, std::make_tuple(py::str(si.symbol), py::str(si.use == nd::SelectorUse::Segment ? "segment" : "offset")));
    for (const auto& [seg, len] : n.code) d.code.set(seg, len);
    for (const auto& [seg, b] : n.images) d.images.set(seg, ::cb::compat::to_py(::cb::ByteView(b)));
    d.data_segment = n.data_segment;
    for (const auto& [seg, b] : n.segment_bytes) d.segment_bytes.set(seg, ::cb::compat::to_py(::cb::ByteView(b)));
    for (const auto& [va, r] : n.results) d.results.set(va, r);
    for (nd::Va va : n.inlined) d.inlined.add(va);
    return d;
}

// The new description a (possibly old-made) Description stands for.
inline const nd::NeDescription& new_of(const Description& d) {
    if (d.impl) return *d.impl;
    throw py::ValueError("this Description was not made by describe16");
}

inline nd::PeDescription describe32_new(const py::str& module_path, const py::list<int64_t>& funcs, const py::str& outdir) {
    const std::filesystem::path module(module_path.utf8());
    const cb::formats::pe::Image image = cb::formats::pe::Image::open(module);
    std::vector<int64_t> starts(funcs.vec().begin(), funcs.vec().end());
    OldKnowledge knowledge;
    if (image.header().machine != 0x01F2) return nd::describe32(module, image, starts, std::filesystem::path(outdir.utf8()), knowledge);
    // PowerPC: the image says where its bytes sit, which stubs are imports and which runs are part of their callers
    nd::PowerPcFacts ppc;
    auto img = formats::powerpc_image::read(module_path);
    for (const auto& span : img->spans) ppc.spans.push_back({std::get<0>(span), static_cast<uint64_t>(std::get<1>(span)), static_cast<uint64_t>(std::get<2>(span))});
    for (const auto& kv : img->imports.items()) ppc.imports[std::get<0>(kv)] = std::get<1>(kv).utf8();
    for (const auto& kv : img->inline_.items()) ppc.inlined[std::get<0>(kv)] = std::get<1>(kv).utf8();
    const symbols::names::Named own = symbols::names::powerpc_names(module_path);
    for (int64_t va : funcs)
        if (own.contains(va)) ppc.names[va] = std::get<0>(own.at(va)).utf8();
    return nd::describe32(module, image, starts, std::filesystem::path(outdir.utf8()), knowledge, &ppc);
}

}  // namespace shim_detail

// Is `va` in one of the described module's code segments?
inline bool inside(const Description& d, int64_t va) { return ::cb::compat::guard([&] { return shim_detail::new_of(d).inside(va); }); }

// --- PE (and PowerPC) ----------------------------------------------------------------------------------------------
inline py::dict<int64_t, py::str> function_names(const py::str& module_path, const py::list<int64_t>& funcs) {
    return ::cb::compat::guard([&] {
        const cb::formats::pe::Image image = cb::formats::pe::Image::open(std::filesystem::path(module_path.utf8()));
        std::vector<int64_t> starts(funcs.vec().begin(), funcs.vec().end());
        shim_detail::OldKnowledge knowledge;
        return ::cb::compat::to_py(nd::function_names(image, starts, knowledge.imported_names(std::filesystem::path(module_path.utf8()))));
    });
}
inline py::dict<int64_t, py::str> decorated_names(const py::str& module_path) {
    return ::cb::compat::guard([&] {
        const std::filesystem::path file(module_path.utf8());
        const auto lane = ::cb::formats::detect::lane(file);
        if (lane && *lane == ::cb::formats::detect::Lane::Ne) return ::cb::compat::to_py(nd::decorated_names(cb::formats::ne::Module::open_file(file)));
        return ::cb::compat::to_py(nd::decorated_names(cb::formats::pe::Image::open(file)));
    });
}
inline py::dict<py::str, py::str> import_names16(const formats::ne::NEFile& nef) {
    return ::cb::compat::guard([&] {
        shim_detail::OldKnowledge knowledge;
        return ::cb::compat::to_py(nd::import_names16(nef.module(), knowledge));
    });
}
// Addresses of a PE module's functions (PowerPC: the .pdata table; x86: found by following the code), sorted.
inline py::list<int64_t> function_starts(const py::str& module_path) {
    if (formats::pe::open_pe(module_path).machine == formats::powerpc_image::MACHINE) return formats::powerpc_image::read(module_path)->functions;
    return verify::lanes::win32::functions(py::str("pe"), module_path);
}
// Write the description of a PE module for the native program. -> (path of the description, {va: name})
inline std::tuple<py::str, py::dict<int64_t, py::str>> describe32(const py::str& module_path, const py::list<int64_t>& funcs, const py::str& outdir) {
    return ::cb::compat::guard([&] {
        const nd::PeDescription d = shim_detail::describe32_new(module_path, funcs, outdir);
        py::set<py::str> ecx;
        for (const std::string& n : d.in_ecx) ecx.add(py::str(n));
        IN_ECX.set(module_path, ecx);
        py::dict<py::str, std::tuple<py::str, int64_t, std::optional<int64_t>>> numbered;
        for (const auto& [name, imp] : d.by_number) numbered.set(py::str(name), std::make_tuple(py::str(imp.module), imp.number, imp.pop));
        BY_NUMBER.set(module_path, numbered);
        return std::make_tuple(py::str(d.path.string()), ::cb::compat::to_py(d.names));
    });
}

// --- NE: finding functions and how they return -------------------------------------------------------------------
inline py::dict<SegOff, py::str> functions(const formats::ne::NEFile& nef) {
    return ::cb::compat::guard([&] {
        py::dict<SegOff, py::str> out;
        for (const auto& [place, how] : nd::find_functions(nef.module())) out.set(shim_detail::old_place(place), shim_detail::distance_text(how));
        return out;
    });
}
inline py::list<SegOff> hidden_functions(const formats::ne::NEFile& nef, const py::set<SegOff>& known) {
    return ::cb::compat::guard([&] {
        std::set<nd::SegOff> k;
        for (const SegOff& s : known) k.insert(shim_detail::new_place(s));
        py::list<SegOff> out;
        for (const nd::SegOff& s : nd::hidden_functions(nef.module(), k)) out.append(shim_detail::old_place(s));
        return out;
    });
}
inline py::list<int64_t> prologues_after_ends(const py::bytes& code) {
    return ::cb::compat::guard([&] {
        py::list<int64_t> out;
        for (int64_t p : nd::prologues_after_ends(nd::Code16(::cb::compat::from_py(code)))) out.append(p);
        return out;
    });
}
inline bool jump_by_return(const py::list<Insn>& ins, int64_t k, const py::set<int64_t>& targets = py::set<int64_t>()) {
    std::set<int64_t> t;
    for (int64_t x : targets) t.insert(x);
    return nd::jump_by_return(shim_detail::to_new(ins), static_cast<std::size_t>(k), t);
}
inline py::list<int64_t> return_jumps(const Description& d) {
    return ::cb::compat::guard([&] {
        py::list<int64_t> out;
        for (int64_t va : nd::return_jumps(shim_detail::new_of(d))) out.append(va);
        return out;
    });
}
inline std::tuple<std::optional<py::str>, int64_t> returns(const py::bytes& code, int64_t off, int64_t limit = 600, const std::optional<py::str>& want = std::nullopt) {
    return ::cb::compat::guard([&] {
        const nd::Return r = nd::returns(nd::Code16(::cb::compat::from_py(code)), off, limit,
                                         want ? std::optional<nd::Distance>(shim_detail::distance_of(*want)) : std::nullopt);
        return std::tuple<std::optional<py::str>, int64_t>(r.kind ? std::optional<py::str>(shim_detail::distance_text(*r.kind)) : std::nullopt, r.pops);
    });
}
inline py::dict<SegOff, py::str> callers_say(const Description& d) {
    return ::cb::compat::guard([&] {
        py::dict<SegOff, py::str> out;
        for (const auto& [place, who] : nd::callers_say(shim_detail::new_of(d)))
            out.set(shim_detail::old_place(place), py::str(who == nd::Remover::Caller ? "caller" : "callee"));
        return out;
    });
}
inline std::optional<int64_t> forwards(const Description& d, int64_t seg, int64_t off, const py::str& kind) {
    return ::cb::compat::guard([&] { return nd::forwards(shim_detail::new_of(d), seg, off, shim_detail::distance_of(kind)); });
}
inline std::optional<int64_t> popped_by_hand(const py::bytes& code, int64_t off, const py::str& kind) {
    return ::cb::compat::guard([&] { return nd::popped_by_hand(nd::Code16(::cb::compat::from_py(code)), off, shim_detail::distance_of(kind)); });
}
inline std::optional<int64_t> args_read(const py::bytes& code, int64_t start, int64_t end, const py::str& kind) {
    return ::cb::compat::guard([&] { return nd::args_read(nd::Code16(::cb::compat::from_py(code)), start, end, shim_detail::distance_of(kind)); });
}
inline bool windows_far_prologue(const py::bytes& code, int64_t off) { return nd::windows_far_prologue(::cb::compat::from_py(code), off); }

using PassOn = std::variant<py::none_t, int64_t, std::tuple<py::str, int64_t>>;
inline int64_t result_bytes(const py::bytes& code, int64_t start, int64_t end, py::list<PassOn>* passes = nullptr, py::list<int64_t>* voided = nullptr) {
    return ::cb::compat::guard([&] {
        std::vector<nd::PassOn> p;
        std::vector<int64_t> v;
        const int64_t r = nd::result_bytes(nd::Code16(::cb::compat::from_py(code)), start, end, passes ? &p : nullptr, voided ? &v : nullptr);
        if (passes)
            for (const nd::PassOn& t : p)
                passes->append(t.kind == nd::PassOn::Kind::Unknown ? PassOn(py::none_t{})
                               : t.kind == nd::PassOn::Kind::Near ? PassOn(t.offset)
                                                                  : PassOn(std::tuple<py::str, int64_t>(py::str("into"), t.offset)));
        if (voided)
            for (int64_t x : v) voided->append(x);
        return r;
    });
}
inline py::list<py::str> jump_tables(const Description& d) {
    return ::cb::compat::guard([&] { return ::cb::compat::to_py(nd::jump_tables(shim_detail::new_of(d))); });
}
inline py::list<py::str> call_overrides(const Description& d) {
    return ::cb::compat::guard([&] { return ::cb::compat::to_py(nd::call_overrides(shim_detail::new_of(d))); });
}
inline py::dict<int64_t, int64_t> results(const Description& d) {
    return ::cb::compat::guard([&] { return ::cb::compat::to_py(nd::results(shim_detail::new_of(d))); });
}
inline int64_t _caller_cleanup(const py::list<Insn>& ins, int64_t j) { return nd::caller_cleanup(shim_detail::to_new(ins), static_cast<std::size_t>(j)); }

class _DxReturns {
  public:
    explicit _DxReturns(const Description& d) : keep_(d.impl), impl_(std::make_unique<nd::DxReturns>(shim_detail::new_of(d))) {}
    bool every_way(const SegOff& place, const py::set<SegOff>& four) {
        std::set<nd::SegOff> f;
        for (const SegOff& s : four) f.insert(shim_detail::new_place(s));
        return impl_->every_way(shim_detail::new_place(place), f);
    }

  private:
    std::shared_ptr<nd::NeDescription> keep_;
    std::unique_ptr<nd::DxReturns> impl_;
};

inline py::set<SegOff> callers_read_dx(const Description& d, py::list<std::tuple<SegOff, SegOff>>* edges = nullptr) {
    return ::cb::compat::guard([&] {
        std::vector<std::pair<nd::SegOff, nd::SegOff>> e;
        py::set<SegOff> out;
        for (const nd::SegOff& s : nd::callers_read_dx(shim_detail::new_of(d), edges ? &e : nullptr)) out.add(shim_detail::old_place(s));
        if (edges)
            for (const auto& [a, b] : e) edges->append(std::make_tuple(shim_detail::old_place(a), shim_detail::old_place(b)));
        return out;
    });
}
inline bool register_helper(const py::bytes& code, int64_t off, const std::optional<int64_t>& end = std::nullopt) {
    return ::cb::compat::guard([&] { return nd::register_helper(nd::Code16(::cb::compat::from_py(code)), off, end); });
}
inline py::set<SegOff> callers_read_ax(const Description& d, const py::set<SegOff>& places) {
    return ::cb::compat::guard([&] {
        std::set<nd::SegOff> p;
        for (const SegOff& s : places) p.insert(shim_detail::new_place(s));
        py::set<SegOff> out;
        for (const nd::SegOff& s : nd::callers_read_ax(shim_detail::new_of(d), p)) out.add(shim_detail::old_place(s));
        return out;
    });
}
inline bool register_arguments(const py::bytes& code, int64_t off) {
    return ::cb::compat::guard([&] { return nd::register_arguments(nd::Code16(::cb::compat::from_py(code)), off); });
}
inline py::str model_line(const Description& d, int64_t va, const py::str& name, const py::str& dflt = py::str("near")) {
    return ::cb::compat::guard([&] { return py::str(nd::model_line(shim_detail::new_of(d), va, name.utf8(), shim_detail::distance_of(dflt))); });
}

// --- the writers ----------------------------------------------------------------------------------------------------
inline std::shared_ptr<Description> describe16(const py::str& module_path, const py::str& outdir) {
    return ::cb::compat::guard([&] {
        shim_detail::OldKnowledge knowledge;
        auto n = std::make_shared<nd::NeDescription>(
            nd::describe16(cb::formats::ne::Module::open_file(std::filesystem::path(module_path.utf8())), std::filesystem::path(outdir.utf8()), knowledge,
                           nd::Policy::from_environment()));
        auto d = std::make_shared<Description>(shim_detail::from_new(*n));
        d->impl = n;
        return d;
    });
}
inline std::shared_ptr<Description> describe_le(const formats::le::LEFile& lf, const py::str& name, const py::str& outdir) {
    return ::cb::compat::guard([&] {
        shim_detail::OldKnowledge knowledge;
        const nd::LeDescription n = nd::describe_le(lf.module(), name.utf8(), std::filesystem::path(outdir.utf8()), knowledge);
        auto d = std::make_shared<Description>(py::str(n.path.string()), ::cb::compat::to_py(n.names), py::dict<int64_t, SegOff>(), py::list<py::str>());
        for (const auto& [obj, span] : n.spans) d->spans.set(obj, std::make_tuple(span.first, span.second));
        d->device = py::str(n.device);
        for (const std::string& l : n.lines) d->lines.append(py::str(l));
        return d;
    });
}
inline std::shared_ptr<Description> describe_dos(const py::str& module_path, const py::str& outdir) {
    return ::cb::compat::guard([&] {
        formats::dos::Program prog = formats::dos::read(module_path);
        const nd::DosDescription n = nd::describe_dos(analyze::dos_functions::shim_detail::program_to_new(prog), std::filesystem::path(outdir.utf8()));
        auto d = std::make_shared<Description>(py::str(n.path.string()), ::cb::compat::to_py(n.names), py::dict<int64_t, SegOff>(), py::list<py::str>());
        for (const auto& [part, span] : n.spans) d->spans.set(part, std::make_tuple(span.first, span.second));
        d->image = prog.image;
        d->base = n.base;
        d->program = prog;
        d->analysis = std::make_shared<analyze::dos_functions::Analysis>(analyze::dos_functions::discover(prog));
        for (const analyze::dos_functions::Interrupt& call : d->analysis->interrupts) {
            if (!call.function) continue;
            if (!d->interrupts.contains(call.function->name)) d->interrupts.set(call.function->name, py::list<analyze::dos_functions::Interrupt>());
            d->interrupts.at(call.function->name).append(call);
        }
        for (const std::string& l : n.lines) d->lines.append(py::str(l));
        return d;
    });
}
inline std::tuple<py::str, py::dict<int64_t, py::str>> describe_nes(const py::str& module_path, const py::str& outdir) {
    return ::cb::compat::guard([&] {
        const auto rom = formats::nes::read(module_path);
        const auto starts = rom->functions();
        std::vector<std::uint32_t> functions;
        for (int64_t va : starts) functions.push_back(static_cast<std::uint32_t>(va));
        const nd::NesDescription n = nd::describe_nes(::cb::formats::nes::read(std::filesystem::path(module_path.utf8())), functions, std::filesystem::path(outdir.utf8()));
        return std::make_tuple(py::str(n.path.string()), ::cb::compat::to_py(n.names));
    });
}

}  // namespace codebreaker::decompile::describe
