// codebreaker/source/pe_writer.cpp -- typed C++ of codebreaker.source.pe_writer.

#include "codebreaker/source/pe_writer.hpp"

#include <capstone/capstone.h>

#include <algorithm>
#include <cctype>

#include "codebreaker/base/capstone_dll.hpp"
#include "codebreaker/emulate/capstone_x86.hpp"
#include "codebreaker/formats/pe_functions.hpp"
#include "codebreaker/source/assembly.hpp"
#include "codebreaker/source/dos_writer.hpp"
#include "codebreaker/translate/backends/asm.hpp"

namespace codebreaker::source::pe_writer {

namespace w = codebreaker::source::image_writer;
namespace l2 = codebreaker::translate::l2;
namespace cs = codebreaker::emulate::capstone_x86;
namespace pe_functions = codebreaker::formats::pe_functions;

const py::str DOC(R"DOC(pe_writer.py -- a 32-bit Windows program as assembly source that builds back into it, byte for byte.

    python -m codebreaker pe-source <program> [--out DIR] [--stub SOURCE.ASM]

    from codebreaker.source import pe_writer
    r = pe_writer.reconstruct(path, outdir)
    r["source"], r["image"], r["same"], r["instructions"], r["raw"], r["rounds"]

The whole file is one NASM source assembled flat (-f bin): one section for the headers and one
for every section of the image, each placed at its file offset (start=) and at its address
(vstart=), so a label's value is the address the program sees. Nothing the loader patches is a
number: every place the base-relocation table names is written as the label it points at, the
import table's addresses are written as `label - IMAGE`, and the entry point as the label of the
entry. Calls and jumps go to labels.

Code is what is reached from the entry point, from the starts of functions, and from every
relocated pointer into code (function pointers, the tables of a switch), each path stopping at
an instruction that would cut through a relocated place; what is not reached is data.

An instruction is written as its mnemonic only where the assembler turns that text back into the
original's bytes: every round assembles the whole file, reads where each instruction went (the
assembler's map of labels) and compares its bytes; one that differs, or that the assembler
rejects, is written as its bytes (with its mnemonic beside it) in the next round, until nothing
differs. The DOS program in front of the headers (a stub) is either kept as the original's bytes
or, given --stub, assembled from its own source (dos-source) and included.
)DOC");

const py::set<py::str> _STOP{py::str("ret"), py::str("retf"), py::str("jmp"), py::str("hlt"), py::str("iretd"), py::str("iret"), py::str("ud2")};
const py::set<int64_t> _SEGS{0x26, 0x2E, 0x36, 0x3E, 0x64, 0x65};
const py::set<py::str> _ARITH{py::str("add"), py::str("or"),  py::str("adc"), py::str("sbb"), py::str("and"),  py::str("sub"),
                              py::str("xor"), py::str("cmp"), py::str("push"), py::str("imul"), py::str("mov"), py::str("test")};

namespace {

// _REGS: the 32, 16 and 8-bit register names, in encoding order
const std::vector<std::vector<const char*>>& regs_() {
    static const std::vector<std::vector<const char*>> r = {{"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"},
                                                           {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"},
                                                           {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"}};
    return r;
}

bool is_reg_(const py::str& x) {
    for (const auto& g : regs_()) {
        for (const char* n : g) {
            if (x == py::str(n)) {
                return true;
            }
        }
    }
    return false;
}

py::dict<py::str, int64_t> make_rr() {
    py::dict<py::str, int64_t> d;
    d.set(py::str("add"), 0x00);
    d.set(py::str("or"), 0x08);
    d.set(py::str("adc"), 0x10);
    d.set(py::str("sbb"), 0x18);
    d.set(py::str("and"), 0x20);
    d.set(py::str("sub"), 0x28);
    d.set(py::str("xor"), 0x30);
    d.set(py::str("cmp"), 0x38);
    d.set(py::str("mov"), 0x88);
    return d;
}

py::list<py::str> make_prelude() {
    py::list<py::str> p;
    p.append(py::str(""));
    p.append(py::str("; op_ r, r: the register-to-register form with the direction bit set (opcode r, r/m), the one"));
    p.append(py::str("; Microsoft's compiler writes; NASM itself writes the other"));
    const int64_t widths[3] = {1, 2, 0};
    for (int w = 0; w < 3; ++w) {
        const auto& names = regs_()[static_cast<size_t>(w)];
        for (size_t k = 0; k < names.size(); ++k) {
            p.append(py::percent("%%assign _r_%s %d", py::str(names[k]), static_cast<int64_t>(k)));
        }
        for (size_t k = 0; k < names.size(); ++k) {
            p.append(py::percent("%%assign _w_%s %d", py::str(names[k]), widths[w]));
        }
    }
    p.append(py::str("%macro _rr 3"));
    p.append(py::str("  %if _w_%2 == 2"));
    p.append(py::str("    db 0x66"));
    p.append(py::str("  %endif"));
    p.append(py::str("  db (%1) | 2 | (_w_%2 > 0), 0xC0 | (_r_%2 << 3) | _r_%3"));
    p.append(py::str("%endmacro"));
    // sorted(_RR.items())
    py::dict<py::str, int64_t> rr = make_rr();
    py::list<py::str> keys;
    for (const py::str& k : rr.keys()) {
        keys.append(k);
    }
    keys.sort();
    for (const py::str& m : keys) {
        p.append(py::percent("%%define %s_ _rr 0x%02x,", m, rr.at(m)));
    }
    return p;
}

bool hex_digit_(char32_t c) {
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f');
}

py::str spaces_(int64_t n) {
    return py::str(std::u32string(static_cast<size_t>(std::max<int64_t>(n, 0)), U' '));
}

py::str db_bytes_(const py::bytes& b, int64_t lo, int64_t hi) {
    std::vector<py::str> parts;
    for (int64_t k = lo; k < hi; ++k) {
        parts.push_back(py::percent("0x%02x", b[k]));
    }
    return py::join(py::str(","), parts);
}

}  // namespace

const py::dict<py::str, int64_t>& RR() {
    static const py::dict<py::str, int64_t> r = make_rr();
    return r;
}
const py::dict<py::str, int64_t> _RR = RR();
const py::list<py::str> _PRELUDE = make_prelude();

// ---- the decoder ----------------------------------------------------------------------------------------

std::optional<Insn> decode(int64_t bits, const py::bytes& code, int64_t address) {
    const cs::Api& api = cs::api();
    csh handle = cs::handle(bits, true);
    cs_insn* insn = nullptr;
    const std::string& raw = code.raw();
    size_t n = api.disasm(handle, reinterpret_cast<const uint8_t*>(raw.data()), raw.size(), static_cast<uint64_t>(address), 1, &insn);
    if (n == 0) {
        return std::nullopt;
    }
    Insn out;
    out.address = static_cast<int64_t>(insn->address);
    out.size = insn->size;
    out.mnemonic = py::str(std::string(insn->mnemonic));
    out.op_str = py::str(std::string(insn->op_str));
    out.bytes = py::bytes(std::string(reinterpret_cast<const char*>(insn->bytes), insn->size));
    if (insn->detail) {
        const cs_x86& x = insn->detail->x86;
        out.imm_size = x.encoding.imm_size;
        out.imm_offset = x.encoding.imm_offset;
        out.disp_size = x.encoding.disp_size;
        out.disp_offset = x.encoding.disp_offset;
        for (int k = 0; k < 4; ++k) {
            out.prefix.append(x.prefix[k]);
        }
        for (int k = 0; k < insn->detail->groups_count; ++k) {
            out.groups.append(insn->detail->groups[k]);
        }
        out.op_count = x.op_count;
        if (x.op_count > 0) {
            out.op0_type = x.operands[0].type;
            out.op0_imm = x.operands[0].imm;
        }
    }
    api.free_(insn, n);
    return out;
}

bool is_branch(const py::str& m) {
    // ^(j\w+|call|loop\w*|jecxz|jcxz)$  (a mnemonic is all word characters)
    if (m == py::str("call") || m.startswith(py::str("loop"))) {
        return true;
    }
    return py::len(m) >= 2 && m.startswith(py::str("j"));
}

std::optional<int64_t> branch_target(const Insn& i) {
    if (is_branch(i.mnemonic) && i.imm_size != 0) {
        // ^0x[0-9a-f]+$
        const py::str& s = i.op_str;
        if (py::len(s) > 2 && s.startswith(py::str("0x"))) {
            bool ok = true;
            for (int64_t k = 2; k < py::len(s); ++k) {
                if (!hex_digit_(s.at_(k))) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                return py::int_parse(s, 16);
            }
        }
    }
    return std::nullopt;
}

// ---- Writer ---------------------------------------------------------------------------------------------

Writer::Writer(const py::str& p) : path(p), pe(pe::open_pe(p)) {
    d = pe.data;
    B = static_cast<int64_t>(pe.image_base);
    headers = py::unpack_from1<uint32_t>("<I", d, pe.e_lfanew + 24 + 60);
    int64_t n_sections = py::len(pe.sections);
    for (int64_t i = 0; i < n_sections; ++i) {
        const pe::Section& s = pe.sections[i];
        Sec sec;
        sec.name = s.name;
        sec.vsize = s.vsize;
        sec.vaddr = s.vaddr;
        sec.rawsize = s.rawsize;
        sec.rawptr = s.rawptr;
        sec.chars = s.chars;
        int64_t va = B + s.vaddr;
        int64_t nxt = i + 1 < n_sections ? B + pe.sections[i + 1].vaddr : va + std::max(s.vsize, s.rawsize);
        py::str nasm = _secname(s.name, i);
        if (!s.rawptr && nasm != py::str(".bss")) {   // (and NASM's .text, .data are always stored)
            nasm = nasm.strip(py::str(".")) + py::str("_nobits");
        }
        sec.va = va;
        sec.file = s.rawptr != 0;
        sec.extent = nxt - va;
        sec.code = (s.chars & 0x20000020) != 0;
        sec.nasm = nasm;
        secs.push_back(sec);
    }
    for (int64_t r : pe.base_relocs()) {   // VA of a relocated dword -> the address it holds
        std::optional<int64_t> o = pe.file_offset(r);
        if (o.has_value()) {
            sites.set(B + r, py::unpack_from1<uint32_t>("<I", d, *o));
        }
    }
    for (const pe::ImportModule& mod : pe.import_table()) {   // IAT slot VA -> label
        for (const pe::ImportSymbol& s : mod.symbols) {
            py::str nm = py::truthy(s.name) ? s.name : py::percent("%s_%d", py::re::sub(py::str("\\W"), py::str("_"), mod.module), s.ordinal);
            imports.set(B + s.slot, py::str("__imp_") + py::re::sub(py::str("[^\\w@?$]"), py::str("_"), nm));
        }
    }
    rva_fields = _import_rvas();   // file offset of an RVA field -> the RVA
    int64_t top = 0;
    bool any = false;
    for (const Sec& s : secs) {
        top = any ? std::max(top, s.rawptr + s.rawsize) : s.rawptr + s.rawsize;
        any = true;
    }
    if (!any) {
        throw py::ValueError("max() arg is an empty sequence");
    }
    tail = py::len(d) - top;
    _decode();
}

py::str Writer::_secname(const py::str& name, int64_t i) {
    py::str n = py::re::sub(py::str("[^\\w.]"), py::str("_"), name);
    if (!py::truthy(n)) {
        n = py::percent("sec%d", i);
    }
    // (NASM's own .bss is never stored in the file; a stored section of that name is named apart)
    if (n != py::str("hdr") && n != py::str("tail") && n != py::str(".bss")) {
        return n;
    }
    return n.strip(py::str(".")) + py::str("_");
}

const Sec* Writer::section_of(int64_t va) const {
    for (const Sec& s : secs) {
        if (s.va <= va && va < s.va + s.extent) {
            return &s;
        }
    }
    return nullptr;
}

std::optional<int64_t> Writer::offset(int64_t va) const {
    if (B <= va && va < B + headers) {
        return va - B;
    }
    const Sec* s = section_of(va);
    if (s && s->file && va - s->va < s->rawsize) {
        return s->rawptr + va - s->va;
    }
    return std::nullopt;
}

py::dict<int64_t, int64_t> Writer::_import_rvas() const {
    py::dict<int64_t, int64_t> out;
    auto [va, _size] = pe.directory(1);
    std::optional<int64_t> off = va ? pe.file_offset(va) : std::nullopt;
    int64_t k = 0;
    while (off.has_value() && *off + 20 * (k + 1) <= py::len(d)) {
        int64_t rec = *off + 20 * k;
        auto [lookup, ts, fw, name, iat] = py::unpack_from<int64_t, int64_t, int64_t, int64_t, int64_t>("<IIIII", d, rec);
        (void)ts;
        (void)fw;
        if (!(lookup || name || iat)) {
            break;
        }
        const std::tuple<int64_t, int64_t> fields[3] = {{rec, lookup}, {rec + 12, name}, {rec + 16, iat}};
        for (const auto& [at, v] : fields) {
            if (v) {
                out.set(at, v);
            }
        }
        for (int64_t table : {lookup, iat}) {
            std::optional<int64_t> t = table ? pe.file_offset(table) : std::nullopt;
            int64_t j = 0;
            while (t.has_value()) {
                int64_t v = py::unpack_from1<uint32_t>("<I", d, *t + 4 * j);
                if (!v) {
                    break;
                }
                if (!(v & 0x80000000)) {
                    out.set(*t + 4 * j, v);
                }
                j += 1;
            }
        }
        k += 1;
    }
    return out;
}

void Writer::_decode() {
    std::vector<const Sec*> code;
    for (const Sec& s : secs) {
        if (s.code && s.file) {
            code.push_back(&s);
        }
    }
    auto in_code = [&](int64_t a) {
        for (const Sec* s : code) {
            if (s->va <= a && a < s->va + s->rawsize) {
                return true;
            }
        }
        return false;
    };
    py::list<int64_t> seeds;
    if (pe.entry) {
        seeds.append(B + pe.entry);
    }
    {
        // (the Python's `import capstone` in pe_functions: here the DLL is named once, where the shared lookup finds it)
        static const bool named = (pe_functions::set_capstone_dll(base::capstone_dll::find()), true);
        (void)named;
        py::list<int64_t> spans;
        for (const auto& [at, _v] : pe_functions::function_spans(path).items()) {
            spans.append(at);
        }
        spans.sort();
        seeds.extend(spans);
    }
    py::list<int64_t> entries = py::list<int64_t>::from(seeds);   // (the entry and the functions found: each a function)
    {
        py::list<int64_t> vs;
        for (const auto& [_k, v] : sites.items()) {
            if (in_code(v)) {
                vs.append(v);
            }
        }
        vs.sort();
        seeds.extend(vs);
    }
    py::list<int64_t> work;
    for (int64_t a : seeds) {
        if (in_code(a)) {
            work.append(a);
        }
    }
    while (py::truthy(work)) {
        int64_t a = work.pop();
        while (in_code(a) && !insns.contains(a)) {
            if (covered.contains(a)) {   // into the middle of one already decoded
                break;
            }
            std::optional<int64_t> o = offset(a);
            if (!o.has_value()) {
                throw py::TypeError("unsupported operand type(s) for +: 'NoneType' and 'int'");
            }
            std::optional<Insn> one = decode(32, d.slice(*o, *o + 15), a);
            bool bad = !one.has_value() || !_fits(*one);
            if (!bad) {
                for (int64_t k = 0; k < one->size; ++k) {
                    if (covered.get(a + k).has_value()) {
                        bad = true;
                        break;
                    }
                }
            }
            if (bad || !in_code(a + one->size - 1)) {
                break;
            }
            insns.set(a, *one);
            for (int64_t k = 0; k < one->size; ++k) {
                covered.set(a + k, a);
            }
            const py::str& m = one->mnemonic;
            std::optional<int64_t> tgt = _target(*one);
            if (tgt.has_value() && in_code(*tgt)) {
                work.append(*tgt);
            }
            if (_STOP.contains(m) || m.startswith(py::str("int3")) || m == py::str("jmp")) {
                break;
            }
            a += one->size;
        }
    }
    // where each function begins, for the fragment each is in the file's L2 tree: those and every
    // place a near call lands, where an instruction was found
    for (int64_t a : entries) {
        if (insns.contains(a)) {
            starts.add(a);
        }
    }
    for (const Insn& i : insns.values()) {
        if (i.mnemonic == py::str("call")) {
            std::optional<int64_t> t = _target(i);
            if (t.has_value() && insns.contains(*t)) {
                starts.add(*t);
            }
        }
    }
}

bool Writer::_fits(const Insn& i) const {
    for (int64_t k = 0; k < i.size; ++k) {
        if (sites.contains(i.address + k)) {
            int64_t at = k;
            if (!((at == i.disp_offset && i.disp_size == 4) || (at == i.imm_offset && i.imm_size == 4))) {
                return false;
            }
            if (is_branch(i.mnemonic) && at == i.imm_offset) {
                return false;   // a relative operand is never relocated
            }
        }
    }
    return true;
}

std::optional<int64_t> Writer::_target(const Insn& i) {
    return branch_target(i);
}

py::set<int64_t> Writer::wanted() const {
    py::set<int64_t> want;
    for (int64_t v : sites.values()) {
        want.add(v);
    }
    for (int64_t v : rva_fields.values()) {
        want.add(B + v);
    }
    for (const Insn& i : insns.values()) {
        std::optional<int64_t> t = _target(i);
        if (t.has_value()) {
            want.add(*t);
        }
    }
    if (pe.entry) {
        want.add(B + pe.entry);
    }
    return want;
}

py::str Writer::name(int64_t va) const {
    std::optional<py::str> imp = imports.get(va);
    if (imp.has_value()) {
        return *imp;
    }
    return py::percent(insns.contains(va) ? "L%x" : "D%x", va);
}

py::str Writer::ref(int64_t va) const {
    std::optional<py::str> p = placed.get(va);
    if (p.has_value()) {
        return *p;
    }
    std::optional<int64_t> st = covered.get(va);
    if (st.has_value()) {   // inside an instruction
        return py::percent("%s+%d", placed.at(*st), va - *st);
    }
    if ((B <= va && va < B + headers) || section_of(va) != nullptr) {
        return py::percent("IMAGE+0x%x", va - B);
    }
    return py::percent("0x%x", va & 0xFFFFFFFF);   // (outside the image: a fixed address)
}

py::list<py::str> Writer::text(const Insn& i, bool raw) const {
    int64_t a = i.address;
    const py::bytes& b = i.bytes;
    std::optional<int64_t> tgt = _target(i);
    py::list<int64_t> site_ks;
    for (int64_t k = 0; k < i.size; ++k) {
        if (sites.contains(a + k)) {
            site_ks.append(k);
        }
    }
    py::str asm_ = py::percent("%s %s", i.mnemonic, i.op_str);
    if (!raw) {
        std::optional<py::str> t = _nasm(i, tgt, site_ks);
        if (t.has_value() && py::truthy(*t)) {
            return py::list<py::str>{*t};
        }
    }
    py::list<py::str> out;
    int64_t k = 0;
    int64_t n = py::len(b);
    while (k < n) {
        if (site_ks.contains(k)) {
            out.append(py::percent("dd %s", ref(sites.at(a + k))));
            k += 4;
        } else if (tgt.has_value() && k == i.imm_offset) {
            const char* wd = i.imm_size == 4 ? "dd" : i.imm_size == 2 ? "dw" : "db";
            out.append(py::percent("%s %s-($+%d)", py::str(wd), ref(*tgt), i.imm_size));
            k += i.imm_size;
        } else {
            int64_t stop = n;
            for (int64_t x : site_ks) {
                if (x > k) {
                    stop = std::min(stop, x);
                }
            }
            if (tgt.has_value() && i.imm_offset > k) {
                stop = std::min(stop, i.imm_offset);
            }
            out.append(py::str("db ") + db_bytes_(b, k, stop));
            k = stop;
        }
    }
    out[0] = out[0] + py::percent("%s; %s", spaces_(std::max<int64_t>(1, 40 - py::len(out[0]))), asm_.strip());
    return out;
}

std::optional<py::str> Writer::_nasm(const Insn& i, const std::optional<int64_t>& tgt, const py::list<int64_t>& site_ks) const {
    static const py::re::Pattern string_ops = py::re::compile(assembly::STRING_OPS);
    static const py::re::Pattern string_ops2 = py::re::compile(py::str("^(rep |repe |repne )?(movs|cmps|stos|lods|scas|ins|outs)[bwd]$"));
    static const py::re::Pattern rr_ops = py::re::compile(py::str("^(\\w+), (\\w+)$"));
    static const py::re::Pattern nosplit = py::re::compile(py::str("\\[(\\w\\w:)?(e\\w\\w\\*[248])"));
    static const py::re::Pattern st_reg = py::re::compile(py::str("\\bst\\((\\d)\\)"));
    static const py::re::Pattern seg_br = py::re::compile(py::str("\\b([cdefgs]s):\\["));
    static const py::re::Pattern bracket = py::re::compile(py::str("\\[[^\\]]*\\]"));
    static const py::re::Pattern bracket_in = py::re::compile(py::str("\\[([^\\]]*)\\]"));
    static const py::re::Pattern imm_tail = py::re::compile(py::str("(-?0x[0-9a-f]+|-?\\d+)$"));
    static const py::re::Pattern base_idx = py::re::compile(py::str("[a-z]{2,3}\\s*[+-]"));
    const py::str& m = i.mnemonic;
    py::str ops = i.op_str;
    bool seg_prefix = false;
    for (int64_t p : i.prefix) {
        if (_SEGS.contains(p)) {
            seg_prefix = true;
        }
    }
    if (seg_prefix && string_ops.match(m).has_value()) {
        return std::nullopt;
    }
    if (m == py::str("ljmp") || m == py::str("lcall")) {
        return std::nullopt;
    }
    bool sm = string_ops.match(m).has_value();
    if (sm || string_ops2.match(m).has_value()) {
        if (seg_prefix) {
            return std::nullopt;
        }
        return m;
    }
    if (tgt.has_value()) {
        if (m == py::str("call")) {
            if (i.imm_size == 4) {
                return py::percent("call %s", ref(*tgt));
            }
            return std::nullopt;
        }
        if (m == py::str("loop") || m == py::str("loope") || m == py::str("loopne") || m == py::str("jecxz") || m == py::str("jcxz")) {
            return py::percent("%s %s", m, ref(*tgt));
        }
        return py::percent("%s %s %s", m, py::str(i.imm_size == 1 ? "short" : "near"), ref(*tgt));
    }
    std::optional<py::re::Match> rr = rr_ops.match(ops);
    if (_RR.contains(m) && rr.has_value() && is_reg_(rr->group(1)) && is_reg_(rr->group(2))) {
        int64_t op = i.bytes[i.bytes[0] == 0x66 ? 1 : 0];
        if (op & 2) {
            return py::percent("%s_ %s", m, ops);
        }
        return py::percent("%s %s", m, ops);
    }
    ops = ops.replace(py::str(" ptr "), py::str(" ")).replace(py::str("xmmword "), py::str("oword ")).replace(py::str("xword "), py::str("tword "));
    ops = nosplit.sub(py::str("[\\1nosplit \\2"), ops);
    ops = st_reg.sub(py::str("st\\1"), ops);
    ops = seg_br.sub(py::str("[\\1:"), ops);
    for (int64_t k : site_ks) {
        int64_t v = sites.at(i.address + k);
        py::str lab = ref(v);
        if (k == i.disp_offset) {
            std::optional<py::re::Match> br = bracket.search(ops);
            if (!br.has_value()) {
                return std::nullopt;
            }
            py::str inner = py::re::sub(py::percent("(?<![\\w+])0x%x\\b", v), lab, br->group(0), 1);
            if (inner == br->group(0)) {
                return std::nullopt;
            }
            ops = ops.slice(0, br->start()) + inner + ops.slice_from(br->end());
        } else {
            py::str repl = py::str(m == py::str("push") ? "dword " : "") + lab;
            py::str nw = py::re::sub(py::percent("(?<![\\w\\[+])0x%x$", v), repl, ops);
            if (nw == ops) {
                return std::nullopt;
            }
            ops = nw;
        }
    }
    if ((i.imm_size == 2 || i.imm_size == 4) && !site_ks.contains(i.imm_offset) && _ARITH.contains(m)) {
        std::optional<py::re::Match> mm = imm_tail.search(ops);
        if (mm.has_value()) {
            int64_t v = py::int_parse(mm->group(1), 0);
            if ((-128 <= v && v <= 127) || v >= (i.imm_size == 2 ? 0xFF80 : 0xFFFFFF80)) {
                ops = ops.slice(0, mm->start()) + py::str(i.imm_size == 2 ? "strict word " : "strict dword ") + mm->group(1);
            }
        }
    }
    if (i.disp_size == 4 && !site_ks.contains(i.disp_offset)) {
        std::optional<py::re::Match> br = bracket_in.search(ops);
        if (br.has_value() && base_idx.search(br->group(1)).has_value()) {
            ops = ops.slice(0, br->start()) + py::str("[dword ") + br->group(1) + py::str("]") + ops.slice_from(br->end());
        }
    }
    return py::percent("%s %s", m, ops).strip();
}

std::tuple<py::str, py::dict<int64_t, int64_t>> Writer::source(const py::set<int64_t>& raw, bool mark_all, const std::optional<py::str>& stub) {
    py::set<int64_t> want = wanted();
    if (mark_all) {
        for (int64_t a : insns.keys()) {
            want.add(a);
        }
    }
    placed = py::dict<int64_t, py::str>();
    // a label can stand at an instruction's start or anywhere in data
    for (int64_t va : want) {
        if (insns.contains(va) || !covered.contains(va)) {
            if (section_of(va) != nullptr || (B <= va && va < B + headers)) {
                placed.set(va, name(va));
            }
        }
    }
    {
        py::set<int64_t> sts;
        for (int64_t v : want) {
            std::optional<int64_t> st = covered.get(v);
            if (st.has_value() && !insns.contains(v)) {
                sts.add(*st);
            }
        }
        for (int64_t st : sts) {
            placed.set(st, name(st));
        }
    }
    py::str nm = py::os::path::basename(path);
    w::Assembly L(py::list<py::str>(), py::str("nasm"), nm);   // (the file as one L2 tree, as it is written)
    py::dict<int64_t, int64_t> lines_of;
    L.append(py::percent("; %s -- assembled with NASM: nasm -f bin -o %s %s", nm, nm, py::str("this file")));
    L.append(py::str("bits 32"));
    L.append(py::percent("IMAGE equ 0x%x", B));
    L.extend(_PRELUDE);
    {
        l2::Dict origin;
        origin.set(py::str("module"), l2::Val(nm));
        origin.set(py::str("va"), l2::Val(B));
        L.part(py::str("hdr"), py::str("headers"), origin, headers);
    }
    L.append(py::str(""));
    L.append(py::percent("section hdr start=0 vstart=0x%x align=1", B));
    if (stub.has_value() && py::truthy(*stub)) {
        L.append(py::percent("incbin \"%s\"", *stub));
        int64_t size = py::os::path::getsize(*stub);
        _data(L, B + size, B + headers, true);
    } else {
        _data(L, B, B + headers, true);
    }
    // what the file holds between the headers and the sections, or between two sections: as it is
    int64_t ends = headers;
    py::dict<py::str, std::tuple<int64_t, int64_t>> gaps;
    {
        std::vector<const Sec*> by_ptr;
        for (const Sec& s : secs) {
            if (s.file) {
                by_ptr.push_back(&s);
            }
        }
        std::stable_sort(by_ptr.begin(), by_ptr.end(), [](const Sec* x, const Sec* y) { return x->rawptr < y->rawptr; });
        for (const Sec* s : by_ptr) {
            if (s->rawptr > ends) {
                gaps.set(s->nasm, std::make_tuple(ends, s->rawptr));
            }
            ends = std::max(ends, s->rawptr + s->rawsize);
        }
    }
    for (const Sec& s : secs) {
        {
            l2::Dict origin;
            origin.set(py::str("module"), l2::Val(nm));
            origin.set(py::str("va"), l2::Val(s.va));
            L.part(s.nasm, py::str("section"), origin, s.file ? std::optional<int64_t>(s.rawsize) : std::nullopt);
        }
        L.append(py::str(""));
        std::optional<std::tuple<int64_t, int64_t>> gap = gaps.get(s.nasm);
        if (gap.has_value()) {
            auto [lo, hi] = *gap;
            L.append(py::percent("section gap_%s start=0x%x vstart=0 align=1", s.nasm.strip(py::str(".")), lo));
            for (const py::str& x : assembly::data_lines(d.slice(lo, hi))) {
                L.append(py::str("    ") + x);
            }
            L.append(py::str(""));
        }
        if (!s.file) {
            L.append(py::percent("section %s nobits vstart=0x%x", s.nasm, s.va));
            _bss(L, s);
            continue;
        }
        L.append(py::percent("section %s progbits start=0x%x vstart=0x%x align=1", s.nasm, s.rawptr, s.va));
        int64_t a = s.va, end = s.va + s.rawsize;
        py::list<int64_t> mine;
        for (int64_t x : starts) {
            if (a <= x && x < end) {
                mine.append(x);
            }
        }
        mine.sort();
        py::dict<int64_t, int64_t> fn_ends;   // (a function runs to the next one)
        for (int64_t k = 0; k < py::len(mine); ++k) {
            fn_ends.set(mine[k], k + 1 < py::len(mine) ? mine[k + 1] : end);
        }
        while (a < end) {
            if (insns.contains(a)) {
                const Insn& i = insns.at(a);
                std::optional<int64_t> fe = fn_ends.get(a);
                if (fe.has_value()) {   // (each function a fragment of its own)
                    std::optional<py::str> pl = placed.get(a);
                    l2::Dict origin;
                    origin.set(py::str("module"), l2::Val(nm));
                    origin.set(py::str("va"), l2::Val(a));
                    L.part(pl.has_value() && py::truthy(*pl) ? *pl : name(a), py::str("function"), origin, *fe - a);
                }
                if (placed.contains(a)) {
                    L.append(py::percent("%s:", placed.at(a)));
                }
                for (const py::str& t : text(i, raw.contains(a))) {
                    lines_of.set(py::len(L) + 1, a);
                    L.append(py::str("    ") + t);
                }
                a += i.size;
            } else {
                int64_t stop = a + 1;
                while (stop < end && !insns.contains(stop)) {
                    stop += 1;
                }
                _data(L, a, stop);
                a = stop;
            }
        }
        // labels in the part of the section the file does not store (zeros the loader adds): from its end
        py::list<int64_t> tailv;
        for (int64_t x : placed.keys()) {
            if (end <= x && x < s.va + s.extent) {
                tailv.append(x);
            }
        }
        tailv.sort();
        for (int64_t v : tailv) {
            L.append(py::percent("%s equ $+0x%x", placed.at(v), v - end));
        }
    }
    if (tail) {
        L.part(py::str("tail"), py::str("after the sections"), std::nullopt, tail);
        L.append(py::str(""));
        L.append(py::percent("section tail start=0x%x vstart=0 align=1", py::len(d) - tail));
        for (const py::str& x : assembly::data_lines(d.slice(py::len(d) - tail, py::len(d)))) {
            L.append(py::str("    ") + x);
        }
    }
    L.append(py::str(""));
    l2 = L.document();
    return std::make_tuple(w::text(L), lines_of);
}

void Writer::_bss(w::Assembly& L, const Sec& s) const {
    int64_t a = s.va, end = s.va + s.extent;
    py::list<int64_t> ps;
    for (int64_t v : placed.keys()) {
        if (a <= v && v < end) {
            ps.append(v);
        }
    }
    ps.sort();
    for (int64_t p : ps) {
        if (p > a) {
            L.append(py::percent("    resb 0x%x", p - a));
        }
        L.append(py::percent("%s:", placed.at(p)));
        a = p;
    }
    if (end > a) {
        L.append(py::percent("    resb 0x%x", end - a));
    }
}

void Writer::_data(w::Assembly& L, int64_t a, int64_t end, bool header) const {
    py::set<int64_t> cut_set;
    for (int64_t v : placed.keys()) {
        if (a <= v && v < end) {
            cut_set.add(v);
        }
    }
    for (int64_t v : sites.keys()) {
        if (a <= v && v < end) {
            cut_set.add(v);
        }
    }
    if (header && pe.entry) {
        int64_t ep = pe.e_lfanew + 24 + 16;
        cut_set.add(B + ep);
    }
    py::dict<int64_t, int64_t> rv;
    for (const auto& [o, v] : rva_fields.items()) {
        std::optional<int64_t> va = _va_of_offset(o);
        if (va.has_value() && a <= *va && *va < end) {
            rv.set(*va, v);
        }
    }
    for (int64_t v : rv.keys()) {
        cut_set.add(v);
    }
    py::list<int64_t> cuts = py::list<int64_t>::from(cut_set);
    cuts.sort();
    cuts.append(end);
    int64_t pos = a;
    for (int64_t c : cuts) {
        if (c < pos) {
            if (placed.contains(c)) {   // inside the dword just written
                L.append(py::percent("%s equ $-%d", placed.at(c), pos - c));
            }
            continue;
        }
        if (c > pos) {
            std::optional<int64_t> o = offset(pos);
            if (!o.has_value()) {
                throw py::TypeError("unsupported operand type(s) for +: 'NoneType' and 'int'");
            }
            for (const py::str& x : assembly::data_lines(d.slice(*o, *o + (c - pos)))) {
                L.append(py::str("    ") + x);
            }
            pos = c;
        }
        if (c == end) {
            break;
        }
        if (placed.contains(c)) {
            L.append(py::percent("%s:", placed.at(c)));
        }
        if (sites.contains(c) && c + 4 <= end) {
            L.append(py::percent("    dd %s", ref(sites.at(c))));
            pos = c + 4;
        } else if (rv.contains(c) && c + 4 <= end) {
            L.append(py::percent("    dd %s-IMAGE", ref(B + rv.at(c))));
            pos = c + 4;
        } else if (header && pe.entry && c == B + pe.e_lfanew + 40) {
            L.append(py::percent("    dd %s-IMAGE                          ; the entry point", ref(B + pe.entry)));
            pos = c + 4;
        }
    }
}

std::optional<int64_t> Writer::_va_of_offset(int64_t o) const {
    for (const Sec& s : secs) {
        if (s.file && s.rawptr <= o && o < s.rawptr + s.rawsize) {
            return s.va + o - s.rawptr;
        }
    }
    return std::nullopt;
}

py::set<int64_t> _differing(const Writer& wr, const py::bytes& data, const py::dict<py::str, std::tuple<int64_t, int64_t>>& labels,
                            const py::set<int64_t>& raw) {
    py::dict<py::str, int64_t> syms;
    for (const auto& [k, v] : labels.items()) {
        syms.set(k, std::get<0>(v));
    }
    py::set<int64_t> bad;
    py::list<int64_t> order = py::list<int64_t>::from(wr.insns.keys());
    order.sort();
    int64_t count = py::len(order);
    for (int64_t n = 0; n < count; ++n) {
        int64_t a = order[n];
        if (raw.contains(a)) {
            continue;
        }
        const Insn& i = wr.insns.at(a);
        std::optional<py::str> pl = wr.placed.get(a);
        std::optional<int64_t> at = syms.get(pl.has_value() ? *pl : py::str(""));
        const Sec* s = wr.section_of(a);
        if (!at.has_value() || s == nullptr) {
            bad.add(a);
            continue;
        }
        std::optional<int64_t> nxt = n + 1 < count ? std::optional<int64_t>(order[n + 1]) : std::nullopt;
        if (nxt.has_value() && *nxt == a + i.size) {
            std::optional<py::str> npl = wr.placed.get(*nxt);
            std::optional<int64_t> nat = syms.get(npl.has_value() ? *npl : py::str(""));
            if (nat.has_value() && wr.section_of(*nxt) == s && *nat - *at != i.size) {
                bad.add(a);
                continue;
            }
        }
        int64_t o = s->rawptr + *at - s->va;
        std::optional<int64_t> ob = wr.offset(a);
        if (!ob.has_value()) {
            throw py::TypeError("unsupported operand type(s) for +: 'NoneType' and 'int'");
        }
        py::bytes mine = data.slice(o, o + i.size), orig = wr.d.slice(*ob, *ob + i.size);
        py::set<int64_t> skip;
        for (int64_t k = 0; k < i.size; ++k) {
            if (wr.sites.contains(a + k)) {
                for (int64_t j = 0; j < 4; ++j) {
                    skip.add(k + j);
                }
            }
        }
        if (Writer::_target(i).has_value()) {
            for (int64_t k = i.imm_offset; k < i.imm_offset + i.imm_size; ++k) {
                skip.add(k);
            }
        }
        bool differs = py::len(mine) != py::len(orig);
        for (int64_t k = 0; !differs && k < py::len(orig); ++k) {
            if (!skip.contains(k) && mine[k] != orig[k]) {
                differs = true;
            }
        }
        if (differs) {
            bad.add(a);
        }
    }
    return bad;
}

std::optional<py::str> _stub(const py::bytes& d, int64_t e_lfanew, const py::str& outdir, const std::function<void(const py::str&)>& say) {
    auto [last, pages] = py::unpack_from<int64_t, int64_t>("<HH", d, 2);
    int64_t size = pages ? (pages - 1) * 512 + (last ? last : 512) : 0;
    if (!(0x80 < size && size <= e_lfanew)) {
        return std::nullopt;
    }
    py::str sd = py::os::path::join(outdir, py::str("stub"));
    py::os::makedirs(sd, 0777, true);
    py::str carved = py::os::path::join(sd, py::str("original"), py::str("STUB.EXE"));
    py::os::makedirs(py::os::path::dirname(carved), 0777, true);
    {
        py::file f = py::open(carved, py::str("wb"));
        f.write(d.slice(0, size));
        f.close();
    }
    dos_writer::Result r;
    try {
        r = dos_writer::reconstruct(carved, sd);
    } catch (const py::Exception& e) {   // (a stub the DOS lane cannot take: its bytes)
        say(py::str("the stub: ") + py::str(e.what()));
        return std::nullopt;
    }
    if (r.file.has_value() && py::truthy(*r.file) && r.file_same.value_or(false)) {
        say(py::percent("the stub: %d bytes, from its own source (%s)", size, r.source_text()));
        return py::os::path::abspath(*r.file);
    }
    say(py::str("the stub does not come back from its source; its bytes are kept"));
    return std::nullopt;
}

w::FlatReport Result::report() const {
    w::FlatReport r;
    r.instructions = instructions;
    r.raw = raw;
    r.rounds = rounds;
    r.same = same;
    r.image = image;
    r.source = source;
    return r;
}

Result reconstruct(const py::str& path, const py::str& outdir_, const StubArg& stub_arg, const std::function<void(const py::str&)>& log) {
    std::function<void(const py::str&)> say = log ? log : std::function<void(const py::str&)>([](const py::str&) {});
    if (!py::truthy(w::nasm())) {
        throw py::SystemExit(py::str("no assembler (settings.json: nasm)"));
    }
    py::str outdir = py::os::path::abspath(outdir_);
    py::os::makedirs(outdir, 0777, true);
    Writer wr(path);
    py::str name = py::os::path::basename(path).lower();
    std::optional<py::str> stub;
    if (std::holds_alternative<std::monostate>(stub_arg)) {
        stub = _stub(wr.d, wr.pe.e_lfanew, outdir, say);
    } else if (std::holds_alternative<py::str>(stub_arg)) {
        stub = std::get<py::str>(stub_arg);
    }
    if (stub.has_value() && !py::truthy(*stub)) {
        stub = std::nullopt;
    }
    w::FlatResult<int64_t> fr = w::flat<int64_t>(
        wr.d,
        [&](const py::set<int64_t>& raw, bool final) { return wr.source(raw, !final, stub); },
        [&](const py::bytes& data, const py::dict<py::str, std::tuple<int64_t, int64_t>>& labels, const py::set<int64_t>& raw) {
            return _differing(wr, data, labels, raw);
        },
        outdir, name, say);
    Result r;
    r.source = fr.source;
    r.image = fr.image;
    r.same = fr.same;
    r.rounds = fr.rounds;
    r.log = fr.log;
    r.instructions = py::len(wr.insns);
    r.raw = py::len(fr.raw);
    return r;
}

int64_t main(const py::list<py::str>& argv) {
    std::vector<py::str> args;
    for (const py::str& a : argv) {
        if (!a.startswith(py::str("--"))) {
            args.push_back(a);
        }
    }
    auto value_of = [&](const char* opt) -> std::optional<py::str> {
        int64_t at = argv.index(py::str(opt));   // (the word after the option)
        return argv[at + 1];
    };
    for (const char* opt : {"--out", "--stub"}) {
        if (argv.contains(py::str(opt))) {
            py::str v = *value_of(opt);
            auto it = std::find(args.begin(), args.end(), v);
            if (it == args.end()) {
                throw py::ValueError("list.remove(x): x not in list");
            }
            args.erase(it);
        }
    }
    if (args.empty()) {
        py::print(DOC);
        return 2;
    }
    py::str path = args[0];
    py::str outdir;
    if (argv.contains(py::str("--out"))) {
        outdir = *value_of("--out");
    } else {
        py::str base = std::get<0>(py::os::path::splitext(py::os::path::basename(path))).lower();
        py::str st = py::settings::get_str(py::str("state"));
        outdir = py::os::path::join(st, py::str("pe_source"), base);
        py::os::makedirs(py::os::path::dirname(outdir), 0777, true);
    }
    StubArg stub = std::monostate();
    if (argv.contains(py::str("--stub"))) {
        stub = py::os::path::abspath(*value_of("--stub"));
    }
    if (argv.contains(py::str("--keep-stub"))) {
        stub = false;
    }
    return w::flat_report(path, reconstruct(path, outdir, stub, [](const py::str& s) { py::print(s); }).report());
}

}  // namespace codebreaker::source::pe_writer
