// codebreaker/build/asm16.cpp -- typed C++17 port of codebreaker.build.asm16 (see asm16.hpp).
//
// asm16.py -- a 16-bit function the C lane cannot give as assembly that links into the rebuild.
//
// The 16-bit system modules were largely written in assembly, and some of their functions say things C cannot: they move the
// stack, they take a software interrupt with the registers set up before it, they read a segment the caller left in ES. When
// the decompiler's C for one does not compile, this writes it instead as what it is -- its own instructions -- in a form that
// links into the rebuilt module wherever the linker puts it:
//
//   * every place the loader fixes (the segment's relocations: a selector, an offset, a far pointer; to a function of the
//     module, its data segment, or an import) is written as the symbol it refers to, so the linker fixes it for where things
//     are now;
//   * a near call or jump that leaves the function (no relocation exists for those: the target was at a known distance in the
//     original segment) is written as `call`/`jmp` to the target's symbol;
//   * everything else is the original's bytes.
//
// A function that refers to its own segment by number (a jump table read through CS, a short jump out of itself) depends on
// where it sits, and is refused: its C stand-in remains -- unless what it reads is the segment's lead-in (the bytes before its
// first function, often a variable such as the data segment's selector), which the build writes the same way and links first
// into the segment, so it stays where it was.
//
// Capstone (the Python binding's C library) is loaded from its DLL: see set_capstone_dll.

#include "codebreaker/build/asm16.hpp"

#include <algorithm>
#include <cstdio>
#include <map>
#include <memory>
#include <mutex>
#include <unordered_map>

#include <capstone/capstone.h>

#include "codebreaker/build/data.hpp"
#include "codebreaker/build/msc6.hpp"
#include "codebreaker/source/assembly.hpp"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#undef near
#undef far
#endif

namespace codebreaker::build::asm16 {

using py::str;
namespace re = py::re;
namespace data16 = codebreaker::build::data;
namespace assembly = codebreaker::source::assembly;

str module_file() {
    // (build/module.py hashes the source text of this module into its cache stamp: the Python module's path)
    auto root = py::os::getenv(str("CB_CHECKOUT_ROOT"));
    return py::os::path::join(root ? *root : str("."), str("codebreaker"), str("build"), str("asm16.py"));
}

// ---------------------------------------------------------------------------------------------------------------------
// capstone: platforms.decoder(bits, detail=...) and its one-instruction reads

namespace {

struct CsLib {
#ifdef _WIN32
    HMODULE module = nullptr;
#endif
    decltype(&cs_open) open = nullptr;
    decltype(&cs_option) option = nullptr;
    decltype(&cs_disasm) disasm = nullptr;
    decltype(&cs_free) free_ = nullptr;
    bool loaded = false;
};

std::wstring& dll_path_() {
    static std::wstring p;
    return p;
}

template <class F> F sym_(HMODULE m, const char* name) {
    FARPROC p = GetProcAddress(m, name);
    if (!p) throw py::ImportError(std::string("capstone.dll has no ") + name);
    return reinterpret_cast<F>(p);
}

CsLib& cs_lib() {
    static CsLib lib;
    static std::mutex lock;
    std::lock_guard<std::mutex> g(lock);
    if (lib.loaded) return lib;
    std::wstring path = dll_path_();
    if (path.empty()) {
        if (const wchar_t* env = _wgetenv(L"CB_CAPSTONE_DLL")) path = env;
    }
    if (path.empty()) path = L"capstone.dll";
    lib.module = LoadLibraryW(path.c_str());
    if (!lib.module) throw py::ImportError("capstone: cannot load the DLL (set_capstone_dll, CB_CAPSTONE_DLL)");
    lib.open = sym_<decltype(&cs_open)>(lib.module, "cs_open");
    lib.option = sym_<decltype(&cs_option)>(lib.module, "cs_option");
    lib.disasm = sym_<decltype(&cs_disasm)>(lib.module, "cs_disasm");
    lib.free_ = sym_<decltype(&cs_free)>(lib.module, "cs_free");
    lib.loaded = true;
    return lib;
}

struct COperand {
    int64_t type = 0;
    int64_t imm = 0;
    int64_t segment = 0, base = 0, index = 0, disp = 0;   // op.mem
};

// capstone's CsInsn, as asm16 reads it
struct Ins {
    int64_t address = 0;
    int64_t size = 0;
    std::string bytes;
    std::string mnemonic, op_str;
    std::vector<int64_t> groups;
    std::vector<COperand> operands;
    int64_t b0() const { return static_cast<unsigned char>(bytes[0]); }
};

using InsPtr = std::shared_ptr<const Ins>;

Ins convert_(const cs_insn& c, bool detail) {
    Ins i;
    i.address = static_cast<int64_t>(c.address);
    i.size = c.size;
    i.bytes.assign(reinterpret_cast<const char*>(c.bytes), c.size);
    i.mnemonic = c.mnemonic;
    i.op_str = c.op_str;
    if (detail && c.detail) {
        for (int g = 0; g < c.detail->groups_count; ++g) i.groups.push_back(c.detail->groups[g]);
        const cs_x86& x = c.detail->x86;
        for (int k = 0; k < x.op_count; ++k) {
            const cs_x86_op& o = x.operands[k];
            COperand op;
            op.type = o.type;
            op.imm = o.imm;
            op.segment = static_cast<int64_t>(static_cast<uint32_t>(o.mem.segment));
            op.base = static_cast<int64_t>(static_cast<uint32_t>(o.mem.base));
            op.index = static_cast<int64_t>(static_cast<uint32_t>(o.mem.index));
            op.disp = o.mem.disp;
            i.operands.push_back(op);
        }
    }
    return i;
}

// platforms.decoder(bits, detail): one handle per thread, bits and detail
class Md {
  public:
    Md(int64_t bits, bool detail) : detail_(detail) {
        CsLib& lib = cs_lib();
        if (lib.open(CS_ARCH_X86, bits == 32 ? CS_MODE_32 : CS_MODE_16, &h_) != CS_ERR_OK) throw py::RuntimeError("capstone: cs_open failed");
        lib.option(h_, CS_OPT_DETAIL, detail ? CS_OPT_ON : CS_OPT_OFF);
    }
    // next(md.disasm(data, address), None)
    InsPtr one(const std::string& data, int64_t address) const {
        CsLib& lib = cs_lib();
        cs_insn* insn = nullptr;
        size_t n = lib.disasm(h_, reinterpret_cast<const uint8_t*>(data.data()), data.size(), static_cast<uint64_t>(address), 1, &insn);
        if (n == 0) return nullptr;
        InsPtr out = std::make_shared<const Ins>(convert_(insn[0], detail_));
        lib.free_(insn, n);
        return out;
    }
    // list(md.disasm(data, address)): decoding stops at the first byte it cannot read
    std::vector<Ins> all(const std::string& data, int64_t address) const {
        CsLib& lib = cs_lib();
        cs_insn* insn = nullptr;
        size_t n = lib.disasm(h_, reinterpret_cast<const uint8_t*>(data.data()), data.size(), static_cast<uint64_t>(address), 0, &insn);
        std::vector<Ins> out;
        for (size_t k = 0; k < n; ++k) out.push_back(convert_(insn[k], detail_));
        if (n) lib.free_(insn, n);
        return out;
    }

  private:
    csh h_ = 0;
    bool detail_;
};

const Md& decoder_(int64_t bits, bool detail) {
    thread_local std::map<std::pair<int64_t, bool>, std::unique_ptr<Md>> held;
    auto& slot = held[{bits, detail}];
    if (!slot) slot = std::make_unique<Md>(bits, detail);
    return *slot;
}

// _DECODED: (inputs) -> the instruction: a segment's code is decoded once, not again for every run and pass that goes over it
InsPtr decoded_(const std::string& data, int64_t at, int64_t bits) {
    thread_local std::unordered_map<std::string, InsPtr> memo;
    std::string key;
    key.reserve(data.size() + 12);
    key.push_back(static_cast<char>(bits));
    key.append(reinterpret_cast<const char*>(&at), sizeof at);
    key += data;
    auto it = memo.find(key);
    if (it != memo.end()) return it->second;
    if (memo.size() > 400000) memo.clear();
    InsPtr r = decoder_(bits, true).one(data, at);
    memo.emplace(key, r);
    return r;
}

// ---- small helpers ---------------------------------------------------------------------------------------------------

str num_(int64_t v) { return str(std::to_string(v)); }

// "%0*x" / "%0*X"
str hx_(int64_t v, int width, bool upper = false) {
    char buf[40];
    std::snprintf(buf, sizeof buf, upper ? "%0*llX" : "%0*llx", width, static_cast<unsigned long long>(v));
    return str(std::string(buf));
}

str which_text_(const Which& w) { return std::holds_alternative<int64_t>(w) ? num_(std::get<int64_t>(w)) : std::get<str>(w); }

bool truthy_(const std::optional<str>& s) { return s && !s->empty(); }

// blob[at:at+16]-style slice as the decoder is fed it
std::string slice_(const py::bytes& b, int64_t lo, int64_t hi) { return b.slice(lo, hi).raw(); }

int64_t u16_(const py::bytes& b, int64_t at) { return py::unpack_from1<int64_t>("<H", b, at); }

const re::Pattern& string_ops_() {
    static const re::Pattern p(assembly::STRING_OPS, 0);
    return p;
}

}  // namespace

void set_capstone_dll(const str& path) {
    std::wstring w;
    for (char32_t c : path.u32()) w.push_back(static_cast<wchar_t>(c));
    dll_path_() = w;
}

// ---------------------------------------------------------------------------------------------------------------------

Function::Function(const formats::ne::NEFile& nef, int64_t seg, int64_t start, int64_t end, const Publics& public_in,
                   const SymbolFor& symbol_for, int64_t cs_ok, const py::list<int64_t>& extra_entries, bool whole,
                   const py::list<std::tuple<int64_t, int64_t, str>>& holes_in) {
    // `seg` the segment number (1-based), [start, end) the function in it -- or the run of functions, written together as
    // their source file had them -- `public` the symbol to define, or [(offset, symbol)] for each function of a run,
    // `symbol_for(kind, *what)` -> symbol or None, kind one of ("function", seg, off), ("segment", seg) [any symbol in that
    // code segment], ("data", segment) [a data segment's first byte], ("import", module, ordinal or name). `cs_ok`: offsets
    // of the segment below it may be read through CS (the segment's lead-in, which the build keeps where it was).
    // `extra_entries`: other places of the run called into from outside (each must also be among `public`).
    // `whole`: the run is a whole segment, which the build keeps where it was -- any place of it may be read through CS by its
    // number. `holes`: [(start, end, symbol)] of the functions inside it rebuilt from C, which the build places after it:
    // their bytes here are filler, and a call to one is a call by its name.
    holes = py::sorted(holes_in);
    auto sf_function = [&](int64_t s, int64_t o) {
        Reference r;
        r.kind = str("function");
        r.seg = s;
        r.off = o;
        return symbol_for(r);
    };
    auto sf_segment = [&](int64_t s) {
        Reference r;
        r.kind = str("segment");
        r.seg = s;
        return symbol_for(r);
    };
    auto sf_data = [&](int64_t s) {
        Reference r;
        r.kind = str("data");
        r.seg = s;
        return symbol_for(r);
    };
    auto sf_import = [&](const str& mod, const Which& what) {
        Reference r;
        r.kind = str("import");
        r.module = mod;
        r.which = what;
        return symbol_for(r);
    };

    std::vector<int64_t> hole_starts;
    for (const auto& h : holes) hole_starts.push_back(std::get<0>(h));
    // (the holes do not overlap: the last one starting at or before x)
    auto hole_at = [&](int64_t x) -> const std::tuple<int64_t, int64_t, str>* {
        int64_t k = static_cast<int64_t>(std::upper_bound(hole_starts.begin(), hole_starts.end(), x) - hole_starts.begin()) - 1;
        if (k >= 0 && x < std::get<1>(holes[k])) return &holes.vec()[static_cast<size_t>(k)];
        return nullptr;
    };

    if (std::holds_alternative<str>(public_in)) publics = py::list<std::tuple<int64_t, str>>{{start, std::get<str>(public_in)}};
    else publics = py::sorted(std::get<py::list<std::tuple<int64_t, str>>>(public_in));
    const str public_name = std::get<1>(publics[0]);   // (publics[0]: IndexError when empty)
    // a 32-bit code segment (NE flag 2000h: GDI's 36 and 38, its flat thunks) is read as 32-bit code -- read as 16-bit, `66 B8
    // iw` took the next two bytes for its immediate and the `mov gs, ax` after it became a near call far past the segment's
    // end. It is written into the 16-bit code class all the same, each instruction as its bytes with its 32-bit spelling
    // beside them: the linker keeps a class's 16- and 32-bit segments apart, and a use32 segment went after all of the
    // original's, renumbering both (and it sets no 2000h flag in a Windows module either way -- docs/NATIVE.md, the linker's
    // backlog)
    const formats::ne::Segment s = nef.segments()[seg - 1];
    bits = (s.flags & 0x2000) ? 32 : 16;
    py::bytes blob_;
    if (s.file_off) blob_ = nef.data.slice(s.file_off, s.file_off + s.length);
    // what the loader allocates past the stored bytes is zeros it then fixes up: a far jump at the very end of a segment has
    // its selector there (GDI's segment 15)
    if (blob_.size() && s.minalloc > blob_.size() && s.minalloc - blob_.size() <= 16) {
        if (end == blob_.size()) end = s.minalloc;
        blob_ = blob_ + py::bytes::zeros(s.minalloc - blob_.size());
    }
    if (!(0 <= start && start < end && end <= blob_.size())) {
        why = str("its bytes are not in the file");
        return;
    }
    this->start = start;
    this->end = end;
    blob = blob_;
    const py::list<formats::ne::Segment> segs = nef.segments();
    for (const auto& x : segs)
        if (x.kind == "CODE") code_segments.set(x.idx, x.length);
    py::dict<int64_t, formats::ne::Entry> entries;
    for (const auto& e : nef.entries())
        if (e.kind == "FIXED" || e.kind == "MOVEABLE") entries.set(e.ordinal, e);
    const py::list<str> refs = nef.module_refs();
    py::dict<int64_t, int64_t> internal_selector;   // offset -> segment: a selector of the module's own
    for (const auto& place : data16::_places(nef, s, blob)) {
        const int64_t off = std::get<0>(place), src = std::get<1>(place), add = std::get<2>(place);
        const formats::ne::Relocation& r = std::get<3>(place);
        if (!(start <= off && off < end) || hole_at(off)) continue;
        int64_t width;
        switch (src) {
            case 0: width = 1; break;
            case 2: width = 2; break;
            case 3: width = 4; break;
            case 5: width = 2; break;
            default: width = -1;
        }
        if (width < 0 || off + width > end) {
            why = str("a fix-up of kind ") + num_(src) + str(" at ") + hx_(off, 4);
            return;
        }
        str sym;
        int64_t selector_seg = 0;   // (the segment a selector fix-up names: tseg below)
        if (r.kind == "IMPORTORD" || r.kind == "IMPORTNAME") {
            const int64_t modi = r.module ? *r.module : 0;
            std::optional<str> mod;
            if (0 < modi && modi <= refs.size()) mod = refs[modi - 1];
            const Which what = r.kind == "IMPORTORD" ? Which(*r.ordinal) : Which(nef.imported_name(*r.name_off));
            // KERNEL's huge-pointer constants, __AHSHIFT (113) and __AHINCR (114), taken as a byte or a word: the number the
            // loader writes there in protected mode, 3 and 8. (`db low offset _KERNEL_113` does not assemble: SETUPX's segment
            // 28, its decompressor, fell back to C over it -- and faulted 15% into copying files)
            std::optional<int64_t> konst;
            if (mod && *mod == "KERNEL") {
                if (std::holds_alternative<int64_t>(what)) {
                    if (std::get<int64_t>(what) == 113) konst = 3;
                    else if (std::get<int64_t>(what) == 114) konst = 8;
                } else if (std::get<str>(what) == "__AHSHIFT") konst = 3;
                else if (std::get<str>(what) == "__AHINCR") konst = 8;
            }
            if (konst && (src == 0 || src == 5)) {
                const int64_t value = (*konst + (r.additive ? add : 0)) & (src == 0 ? 0xFF : 0xFFFF);
                sites.set(off, {width, src == 0 ? str("db   0") + hx_(value, 2, true) + str("h") : str("dw   0") + hx_(value, 4, true) + str("h")});
                continue;
            }
            std::optional<str> got;
            if (mod) got = sf_import(*mod, what);
            if (!truthy_(got)) {
                why = str("an import (") + (mod ? *mod : str("None")) + str(".") + which_text_(what) + str(") with no symbol");
                return;
            }
            sym = *got;
            imports.set(sym, {*mod, what});
            externs.set(sym, str("far"));
        } else if (r.kind == "INTERNAL") {
            int64_t tseg, toff;
            if (r.seg && *r.seg == 0xFF) {
                if (!entries.contains(*r.entry)) {
                    why = str("a fix-up to entry ") + num_(*r.entry) + str(", which the module does not have");
                    return;
                }
                const formats::ne::Entry& e = entries.at(*r.entry);
                tseg = *e.seg;
                toff = *e.offset;
            } else {
                tseg = *r.seg;
                toff = *r.entry;
            }
            toff = (toff + add) & 0xFFFF;
            const str kind = (0 < tseg && tseg <= segs.size()) ? segs[tseg - 1].kind : str("");
            std::optional<str> got;
            if (kind == "DATA") {   // a data segment: the auto one or one carried
                got = sf_data(tseg);
                if (truthy_(got)) {
                    externs.set(*got, str("byte"));
                    if (src != 2 && toff) got = *got + str(" + ") + num_(toff);
                }
            } else if (src == 2) {
                got = sf_segment(tseg);
                if (truthy_(got)) externs.set(*got, str("far"));
            } else {
                got = sf_function(tseg, toff);
                if (truthy_(got)) externs.set(*got, str("far"));
            }
            if (!truthy_(got)) {
                why = str("a fix-up to ") + num_(tseg) + str(":") + hx_(toff, 4) + str(", where nothing has a symbol");
                return;
            }
            sym = *got;
            selector_seg = tseg;
        } else {
            why = str("an operating-system fix-up (floating point) at ") + hx_(off, 4);
            return;
        }
        str text;
        switch (src) {
            case 2: text = str("dw   seg ") + sym; break;
            case 3: text = str("dd   ") + sym; break;
            case 5: text = str("dw   offset ") + sym; break;
            default: text = str("db   low offset ") + sym;
        }
        if (add && r.kind != "INTERNAL" && src != 2) text += str(" + ") + num_(add);
        sites.set(off, {width, text});
        if (r.kind == "INTERNAL" && src == 2) internal_selector.set(off, selector_seg);
    }
    // The instructions: near calls and jumps leaving the function, and references to its own segment
    py::dict<int64_t, std::tuple<int64_t, str>> calls_;   // offset -> (size, text)
    py::dict<int64_t, str> sym_of;                         // site offset -> the symbol text of its directive
    for (const auto& kv : sites.items()) {
        const str& text = std::get<1>(std::get<1>(kv));
        // text.split(None, 1)[1] if " " in text else text
        if (text.contains(str(" "))) {
            const std::u32string& u = text.u32();
            size_t a = 0;
            while (a < u.size() && str::space_(u[a])) a++;
            size_t b = a;
            while (b < u.size() && !str::space_(u[b])) b++;
            while (b < u.size() && str::space_(u[b])) b++;
            if (b >= u.size()) throw py::IndexError("list index out of range");
            sym_of.set(std::get<0>(kv), str(u.substr(b)));
        } else {
            sym_of.set(std::get<0>(kv), text);
        }
    }
    // The instructions are found by following the code from its entry points (each function of the run, and any other place
    // called into from outside): what nothing reaches is data and stays bytes. A jump through a table in the run is followed
    // into the table.
    std::vector<int64_t> work;
    {
        py::set<int64_t> entry_points;
        for (const auto& p : publics) entry_points.add(std::get<0>(p));
        for (int64_t e : extra_entries) entry_points.add(e);
        std::vector<int64_t> v(entry_points.begin(), entry_points.end());
        std::sort(v.begin(), v.end());
        work = v;
    }
    py::set<int64_t> seen;
    missing = py::set<int64_t>();
    tables = py::set<int64_t>();
    while (!work.empty()) {
        int64_t at = work.back();
        work.pop_back();
        while (start <= at && at < end && !seen.contains(at) && !hole_at(at)) {
            InsPtr insp = decoded_(slice_(blob, at, std::min(end, at + 16)), at, bits);
            if (!insp || at + insp->size > end || hole_at(at + insp->size - 1)) break;   // (nothing runs on into a function rebuilt from C)
            const Ins& ins = *insp;
            seen.add(at);
            bool covered = false;   // (its few bytes, looked up)
            for (int64_t o = at; o < at + ins.size; ++o)
                if (sites.contains(o)) {
                    covered = true;
                    break;
                }
            const int64_t b0 = ins.b0();
            const str mn(ins.mnemonic);
            if (covered) {
                // the forms with a symbolic spelling: a far call or jump to a symbol, a symbol's selector or offset loaded or
                // pushed
                const int64_t site = at + 1;
                auto site_width = [&](int64_t o) -> int64_t { return sites.contains(o) ? std::get<0>(sites.at(o)) : 0; };
                if ((b0 == 0x9A || b0 == 0xEA) && ins.size == 5 && site_width(site) == 4) {
                    // sym_of[site].split()[-1]
                    py::list<str> parts = py::split(sym_of.at(site));
                    const str name = parts[-1];
                    symbolic.set(at, {5, str(b0 == 0x9A ? "call" : "jmp") + str(" far ptr ") + name});
                } else if ((b0 == 0x9A || b0 == 0xEA) && ins.size == 5 && internal_selector.contains(at + 3)) {
                    // a far call into the module: the file relocates only its selector; its offset is plain bytes -- the
                    // target's, which moves with it: the whole is the name
                    const int64_t tseg = internal_selector.at(at + 3);
                    const int64_t toff = u16_(blob, at + 1);
                    std::optional<str> name = sf_function(tseg, toff);
                    if (!truthy_(name)) {
                        far_missing.add({tseg, toff});
                        why = str("a far ") + mn + str(" to ") + num_(tseg) + str(":") + hx_(toff, 4) + str(", where no function has a symbol");
                        if (tseg == seg) missing.add(toff);
                        return;
                    }
                    externs.set(*name, str("far"));
                    symbolic.set(at, {5, str(b0 == 0x9A ? "call" : "jmp") + str(" far ptr ") + *name});
                    sites.pop(at + 3);
                } else if (((0xB8 <= b0 && b0 <= 0xBF) || b0 == 0x68) && ins.size == 3 && site_width(site) == 2) {
                    static const char* const REGS[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
                    const str what = sym_of.at(site);   // `seg X` or `offset X`
                    symbolic.set(at, {3, b0 != 0x68 ? str("mov  ") + str(REGS[b0 - 0xB8]) + str(", ") + what : str("push ") + what});
                }
            } else {
                for (const COperand& op : ins.operands) {
                    if (op.type != X86_OP_MEM || op.segment != X86_REG_CS) continue;
                    const int64_t disp = op.disp & 0xFFFF;
                    if (op.base == 0 && op.index == 0 && 0 <= disp && disp < cs_ok) continue;   // the segment's lead-in: where it was
                    if (whole && op.base == 0 && op.index == 0) continue;                      // the whole segment is where it was
                    if (hole_at(disp)) {
                        why = str("it reads a function rebuilt from C through CS (") + mn + str(" ") + str(ins.op_str) + str(" at ") + hx_(at, 4) + str(")");
                        reaches = disp;   // (that one is the assembly's own: a table)
                        return;
                    }
                    if (start <= disp && disp < end && ins.size >= 3) {
                        cs_labels.set(at, disp);   // a table or a variable of the run: a label
                        if ((mn == "jmp" || mn == "call") && op.index == 0 && op.base != 0 && bits == 16) {
                            // a jump or call table: its words are places in the run to go on from (it ends, at the latest,
                            // where the first place it sends to after it begins: past that is the code it goes on to)
                            int64_t t = disp, first = end;
                            while (start <= t && t < std::min(end - 1, first) && !seen.contains(t)) {
                                const int64_t target = u16_(blob, t);
                                if (!(start <= target && target < end)) break;
                                if (target > disp) first = std::min(first, target);
                                tables.add(t);
                                work.push_back(target);
                                t += 2;
                            }
                        }
                        continue;
                    }
                    if (whole) continue;
                    why = str("it reads its own segment through CS (") + mn + str(" ") + str(ins.op_str) + str(" at ") + hx_(at, 4) + str(")");
                    return;
                }
            }
            bool has_jump_or_call = false;
            for (int64_t g : ins.groups)
                if (g == X86_GRP_JUMP || g == X86_GRP_CALL) has_jump_or_call = true;
            std::optional<int64_t> target;
            if (!covered && has_jump_or_call && !ins.operands.empty() && ins.operands[0].type == X86_OP_IMM && b0 != 0x9A && b0 != 0xEA) {
                target = ins.operands[0].imm & (bits == 16 ? 0xFFFF : 0xFFFFFFFFLL);
                const auto* h = hole_at(*target);
                if (h && *target == std::get<0>(*h) && (b0 == 0xE8 || b0 == 0xE9)) {
                    externs.set(std::get<2>(*h), str("near"));   // a function rebuilt from C: by its name
                    calls_.set(at, {ins.size, str(b0 == 0xE8 ? "call" : "jmp ") + str(" near ptr ") + std::get<2>(*h)});
                } else if (h) {
                    why = str("a jump into the middle of ") + std::get<2>(*h) + str(", rebuilt from C (") + mn + str(" at ") + hx_(at, 4) + str(")");
                    reaches = *target;
                    return;
                } else if (start <= *target && *target < end) {
                    this->ins.set(at, {py::bytes(ins.bytes), mn, str(ins.op_str), *target});
                    work.push_back(*target);
                } else if (*target >= blob.size()) {
                    // (past the end of the segment: no code jumps or calls there -- bytes read as code. NETDI's segment 54
                    // ends in strings far pointers name ("NETDI: ", "netlog.txt"), described as functions: `ja 316C` in a
                    // segment 310A long refused all 666 of its functions. Followed no further: bytes. A near call there was
                    // taken for one to a function at that place: an entry no piece defines, a stand-in in another segment,
                    // and a link that failed -- "relocation not in the same segment")
                    break;
                } else if (b0 != 0xE8 && b0 != 0xE9) {
                    why = str("a short jump out of itself (") + mn + str(" at ") + hx_(at, 4) + str(")");
                    reaches = *target;
                    return;
                } else {
                    std::optional<str> sym = sf_function(seg, *target);
                    if (!truthy_(sym)) {
                        missing.add(*target);
                    } else {
                        externs.set(*sym, str("near"));
                        calls_.set(at, {ins.size, str(b0 == 0xE8 ? "call" : "jmp ") + str(" near ptr ") + *sym});
                    }
                }
            } else if (!covered) {
                this->ins.set(at, {py::bytes(ins.bytes), mn, str(ins.op_str), std::nullopt});
            }
            if (mn == "ret" || mn == "retf" || mn == "iret" || mn == "jmp" || mn == "ljmp" || mn == "hlt") break;   // the code does not go on past it
            at += ins.size;
        }
    }
    if (!missing.empty()) {
        py::list<int64_t> m = py::sorted(py::list<int64_t>::from(missing));
        std::vector<str> parts;
        for (int64_t t : m) parts.push_back(hx_(t, 4));
        why = str("a near call or jump to ") + py::join(str(", "), py::list<str>(parts)) + str(", where no function has a symbol");
        reaches = m[0];
        return;
    }
    if (!calls_.empty() && bits == 32) {
        // (a near call or jump out of the run, by its name, would be written as 16-bit code)
        int64_t lowest = INT64_MAX;
        for (const auto& kv : calls_.items()) lowest = std::min(lowest, std::get<0>(kv));
        why = str("a near call or jump out of a 32-bit segment's run (") + hx_(lowest, 4) + str(")");
        return;
    }
    calls = calls_;
    if (bits == 16) _far_pointer_offsets(symbol_for, internal_selector, seg);   // (its numbers are imm16s: see there)
    _raw_far_pointers(symbol_for, internal_selector);
    module = nef.module_name();
    this->seg = seg;
    public_ = public_name;
    // a jump into the middle of an instruction: that instruction is not written as one (each instruction's own few bytes
    // looked up in the set: not every target for each)
    py::set<int64_t> targets;
    for (const auto& kv : this->ins.items()) {
        const auto& t3 = std::get<3>(std::get<1>(kv));
        if (t3) targets.add(*t3);
    }
    for (int64_t a : this->ins.keys()) {
        const int64_t n = std::get<0>(this->ins.at(a)).size();
        for (int64_t x = a + 1; x < a + n; ++x)
            if (targets.contains(x)) {
                this->ins.pop(a);
                break;
            }
    }
    // a place read through CS is a label too: no instruction may run over it
    py::set<int64_t> read;
    for (const auto& kv : cs_labels.items()) read.add(std::get<1>(kv));
    for (int64_t a : this->ins.keys()) {
        const int64_t n = std::get<0>(this->ins.at(a)).size();
        for (int64_t x = a + 1; x < a + n; ++x)
            if (read.contains(x)) {
                this->ins.pop(a);
                break;
            }
    }
    for (int64_t a : cs_labels.keys())
        if (!this->ins.contains(a)) cs_labels.pop(a);
    labels = py::set<int64_t>();
    for (const auto& kv : this->ins.items()) {
        const auto& t3 = std::get<3>(std::get<1>(kv));
        if (t3 && this->ins.contains(*t3)) labels.add(*t3);
    }
    for (const auto& kv : this->ins.items()) {
        const auto& t3 = std::get<3>(std::get<1>(kv));
        if (t3 && !labels.contains(*t3)) this->ins.pop(std::get<0>(kv));   // (its target is not an instruction start)
    }
    for (const auto& kv : cs_labels.items()) labels.add(std::get<1>(kv));
    py::set<int64_t> held;
    for (int64_t t : tables) {   // what a jump table holds: labels, as words
        labels.add(u16_(blob, t));
        held.add(t);
        held.add(t + 1);
    }
    for (int64_t a : this->ins.keys()) {
        const int64_t n = std::get<0>(this->ins.at(a)).size();
        for (int64_t x = a; x < a + n; ++x)
            if (held.contains(x)) {
                this->ins.pop(a);
                break;
            }
    }
    text = write(py::set<int64_t>());
}

void Function::_raw_far_pointers(const SymbolFor& symbol_for, const py::dict<int64_t, int64_t>& internal_selector) {
    // The same far pointers where the frame is written as bytes (code not followed, a table): a selector fix-up to one of the
    // module's code segments, and in the two bytes before it the offset -- of `call far` (9A) or of a pointer stored -- is a
    // number. If it is where a function begins, it is that function's offset, by its name (USER's 25:23DF `call far 12:085E`,
    // LoadIcon's, in code reached only through a table: with 085E rebuilt from C and placed after its frame, the call ran its
    // filler into the next function).
    py::set<int64_t> covered;
    for (const auto& kv : ins.items()) {
        const int64_t n = std::get<0>(std::get<1>(kv)).size();
        for (int64_t x = std::get<0>(kv); x < std::get<0>(kv) + n; ++x) covered.add(x);
    }
    for (const auto& kv : symbolic.items()) {
        const int64_t size = std::get<0>(std::get<1>(kv));
        for (int64_t x = std::get<0>(kv); x < std::get<0>(kv) + size; ++x) covered.add(x);
    }
    std::vector<std::pair<int64_t, int64_t>> selectors;
    for (const auto& kv : internal_selector.items()) selectors.push_back({std::get<0>(kv), std::get<1>(kv)});
    std::sort(selectors.begin(), selectors.end());
    for (const auto& so : selectors) {
        const int64_t off = so.first, tseg = so.second;
        if (!sites.contains(off)) continue;
        // the offset before the selector (a stored pointer, `call far`), or pushed right after it (`push seg X` / `push
        // 0FF1h`: 68 ss ss 68 oo oo)
        std::vector<int64_t> places = {off - 2};
        if (blob.slice(off - 1, off) == py::bytes("\x68", 1) && blob.slice(off + 2, off + 3) == py::bytes("\x68", 1)) places = {off + 3};
        for (int64_t p : places) {
            if (p < start || p + 2 > end || sites.contains(p) || sites.contains(p + 1) || covered.contains(p) || covered.contains(p + 1)) continue;
            const int64_t value = u16_(blob, p);
            std::optional<str> name;
            {
                Reference r;
                r.kind = str("function");
                r.seg = tseg;
                r.off = value;
                name = symbol_for(r);
            }
            if (truthy_(name)) {
                if (!externs.contains(*name)) externs.set(*name, str("far"));
                sites.set(p, {2, str("dw   offset ") + *name});
                break;
            }
            if (p == off + 3 || blob[p - 1] == 0x9A || blob[p - 1] == 0xEA) {
                // a call or a pushed pointer into the middle of a function: that place is an entry of its own (USER 35:035A
                // `call far 43:0234`, inside sub_43_01D4 -- rebuilt from C, the call ran its filler)
                raw_far_entries.add({tseg, value});
            }
        }
    }
}

std::optional<int64_t> Function::_pointer_selector(int64_t at, const py::bytes& data, const py::dict<int64_t, int64_t>& selectors) const {
    // The code segment whose selector makes a far pointer with the number the instruction at `at` gives, in one of the two
    // shapes a far pointer is built in -- `push seg X` right before `push n`, or the two words stored side by side (`mov [m], n`
    // / `mov [m+2], seg X`, either order) -- or None.
    if (data[0] == 0x68 && data.size() == 3) {
        if (blob.slice(at - 3, at - 2) == py::bytes("\x68", 1) && selectors.contains(at - 2)) return selectors.at(at - 2);
        return std::nullopt;
    }
    InsPtr me = decoded_(slice_(blob, at, at + 16), at, bits);
    const COperand* mem = nullptr;
    if (me)
        for (const COperand& o : me->operands)
            if (o.type == X86_OP_MEM) {
                mem = &o;
                break;
            }
    if (!mem) return std::nullopt;
    std::map<int64_t, int64_t> sizes_map;
    std::vector<int64_t> order;
    for (const auto& kv : ins.items()) {
        if (!sizes_map.count(std::get<0>(kv))) order.push_back(std::get<0>(kv));
        sizes_map[std::get<0>(kv)] = std::get<0>(std::get<1>(kv)).size();
    }
    for (const auto& kv : symbolic.items()) {   // (the selector's own: a fix-up)
        if (!sizes_map.count(std::get<0>(kv))) order.push_back(std::get<0>(kv));
        sizes_map[std::get<0>(kv)] = std::get<0>(std::get<1>(kv));
    }
    std::vector<int64_t> before;
    for (int64_t a : order)
        if (a + sizes_map[a] == at) before.push_back(a);
    std::vector<int64_t> candidates = {at + data.size()};
    candidates.insert(candidates.end(), before.begin(), before.end());
    for (int64_t other : candidates) {
        if (!sizes_map.count(other)) continue;
        const int64_t site = other + sizes_map[other] - 2;
        if (!selectors.contains(site)) continue;
        InsPtr ip = decoded_(slice_(blob, other, other + 16), other, bits);
        const COperand* om = nullptr;
        if (ip)
            for (const COperand& o : ip->operands)
                if (o.type == X86_OP_MEM) {
                    om = &o;
                    break;
                }
        if (om && om->base == mem->base && om->index == mem->index && om->segment == mem->segment && om->disp == mem->disp + 2) return selectors.at(site);
    }
    return std::nullopt;
}

void Function::_far_pointer_offsets(const SymbolFor& symbol_for, const py::dict<int64_t, int64_t>& selectors, int64_t seg) {
    // selectors: {offset of the fix-up: the segment it is the selector of}
    // A far pointer to one of the module's functions put together in the code: its selector is a fix-up (or CS), its offset a
    // plain number -- the function's, in the original. Built from the number, the pointer would still go where the function WAS
    // (USER's 3:1505 `mov word ptr [bp-18h], 1587h` with segment 10's selector: the window procedure BEAR306, rebuilt from C and
    // placed after its frame, entered at its old place -- the frame's filler). So that number is written as the function's
    // offset, by its name.
    // (the selector is the one beside it -- a few instructions away: a frame may hold selectors of several segments, each with a
    // function at the same offset: USER's `push seg 12; push 0FF1h`, LoadDIBCursorHandler, where 35:0FF1 is a function too)
    const int64_t near_ = 24;
    std::vector<int64_t> own;
    for (const auto& kv : ins.items()) {
        const str& mn = std::get<1>(std::get<1>(kv));
        const str& ops = std::get<2>(std::get<1>(kv));
        if ((mn == "push" && ops == "cs") || (mn == "mov" && ops.endswith(str(", cs")))) own.push_back(std::get<0>(kv));
    }
    if (selectors.empty() && own.empty()) return;
    for (const auto& kv : ins.items()) {
        const int64_t at = std::get<0>(kv);
        const py::bytes& data = std::get<0>(std::get<1>(kv));
        const str& mn = std::get<1>(std::get<1>(kv));
        const auto& target = std::get<3>(std::get<1>(kv));
        if (target || !(mn == "mov" || mn == "push") || data.size() < 3) continue;
        const int64_t d0 = data[0];
        if (!(d0 == 0x68 || (0xB8 <= d0 && d0 <= 0xBF) || d0 == 0xC7 ||
              ((d0 == 0x26 || d0 == 0x2E || d0 == 0x36 || d0 == 0x3E) && data.size() > 3 && data[1] == 0xC7)))
            continue;   // (an imm16 form: push, mov reg / mem)
        const int64_t value = u16_(data, data.size() - 2);
        if (!value) continue;
        py::set<int64_t> segs;
        for (const auto& so : selectors.items())
            if (std::llabs(std::get<0>(so) - at) <= near_) segs.add(std::get<1>(so));
        for (int64_t a : own)
            if (std::llabs(a - at) <= near_) {
                segs.add(seg);
                break;
            }
        py::set<str> names;
        for (int64_t t : segs) {
            Reference r;
            r.kind = str("function");
            r.seg = t;
            r.off = value;
            std::optional<str> n = symbol_for(r);
            if (n) names.add(*n);
        }
        if (names.empty()) {
            // a far pointer to where no function is known to begin: a function nothing else calls (USER's callback at 1:3452,
            // `push seg 1 / push 3452h`, passed to an enumeration -- taken for the tail of sub_1_3412, and gone with it when
            // that was rebuilt from C): that place is an entry of its own. Only in the exact shapes of a far pointer -- a
            // number merely near a selector is not one (taken loosely, 126 of USER's functions went back to assembly for
            // constants)
            std::optional<int64_t> t = _pointer_selector(at, data, selectors);
            if (t && value < code_segments.get(*t, 0)) raw_far_entries.add({*t, value});
            continue;
        }
        if (names.size() != 1) continue;
        const str name = *names.begin();
        const str text = mnemonic(at);
        const str new_text = re::Pattern(str("(?i)\\b0*") + hx_(value, 0, true) + str("h$"), 0)
                                 .sub([&](const re::Match&) { return str("offset ") + name; }, text);
        if (new_text == text) continue;
        if (!externs.contains(name)) externs.set(name, str("far"));
        symbolic.set(at, {data.size(), new_text});   // (written by its name: see write)
    }
}

str Function::mnemonic(int64_t at) const {
    // The instruction at `at` as the assembler reads it, or None.
    const auto& rec = ins.at(at);
    const py::bytes& data = std::get<0>(rec);
    const str& mn = std::get<1>(rec);
    str ops = std::get<2>(rec);
    const auto& target = std::get<3>(rec);
    std::optional<re::Match> m = string_ops_().match(mn + str(" "));
    if (m && !ops.replace(str("es:[di]"), str("")).contains(str(":"))) {
        std::optional<str> g1 = m->group_opt(1);
        return ((g1 ? *g1 : str("")) + m->group(2) + m->group(3)).strip();
    }
    if (target) {
        const str label = str("L_") + hx_(*target, 4, true);
        if (mn == "loop" || mn == "loope" || mn == "loopne" || mn == "jcxz" || mn == "jecxz") return mn.ljust(6) + str(" ") + label;
        return mn.ljust(6) + str(" ") + (data.size() == 2 ? str("short") : str("near ptr")) + str(" ") + label;
    }
    if (cs_labels.contains(at)) {   // what it reads through CS, by its label
        const int64_t disp = cs_labels.at(at);
        if (disp) {
            ops = re::Pattern(str("\\b0x") + hx_(disp, 0) + str("\\b"), 0).sub([&](const re::Match&) { return str("L_") + hx_(disp, 4, true); }, ops);
        } else {
            // ops.replace("cs:[", "cs:[L_0000 + ", 1)
            int64_t i = ops.find(str("cs:["));
            if (i >= 0) ops = ops.slice(0, i) + str("cs:[L_0000 + ") + ops.slice_from(i + 4);
        }
    }
    ops = re::Pattern(str("(?<![a-z]:)\\[(0x[0-9a-f]+)\\]"), 0).sub([&](const re::Match& mm) { return str("ds:[") + mm.group(1) + str("]"); }, ops);
    return (assembly::mnemonic(mn).ljust(6) + str(" ") + assembly::operands(ops)).rstrip();
}

bool Function::_stays_inside(int64_t at, int64_t lo, int64_t hi) const {
    // Whether what the instruction at `at` points at (a jump's target, a place it reads through CS) is inside [lo, hi): if not,
    // it is written as bytes in a piece.
    const auto& target = std::get<3>(ins.at(at));
    if (target && !(lo <= *target && *target < hi)) return false;
    if (!cs_labels.contains(at)) return true;
    const int64_t disp = cs_labels.at(at);
    return lo <= disp && disp < hi;
}

bool Function::_used(int64_t label, int64_t lo, int64_t hi) {
    // Whether a label is referred to from inside [lo, hi) (so a piece writes it).
    if (!(lo <= label && label < hi)) return false;
    if (!_refs) {   // (who refers to each label, once)
        py::dict<int64_t, py::list<int64_t>> refs;
        for (const auto& kv : ins.items()) {
            const auto& t3 = std::get<3>(std::get<1>(kv));
            if (t3) {
                if (!refs.contains(*t3)) refs.set(*t3, py::list<int64_t>());
                refs.at(*t3).append(std::get<0>(kv));
            }
        }
        for (const auto& kv : cs_labels.items()) {
            if (!refs.contains(std::get<1>(kv))) refs.set(std::get<1>(kv), py::list<int64_t>());
            refs.at(std::get<1>(kv)).append(std::get<0>(kv));
        }
        for (int64_t t : tables) {
            const int64_t w = u16_(blob, t);
            if (!refs.contains(w)) refs.set(w, py::list<int64_t>());
            refs.at(w).append(t);
        }
        _refs = refs;
    }
    if (!_refs->contains(label)) return false;
    for (int64_t a : _refs->at(label))
        if (lo <= a && a < hi) return true;
    return false;
}

py::dict<int64_t, std::tuple<str, py::bytes, py::bytes>> Function::candidates() const {
    // {offset: (text, bytes, mask)} of the instructions the assembler's spelling is probed for.
    py::dict<int64_t, std::tuple<str, py::bytes, py::bytes>> out;
    if (bits != 16) return out;   // (see write)
    for (const auto& kv : ins.items()) {
        const int64_t at = std::get<0>(kv);
        const auto& rec = std::get<1>(kv);
        if (!std::get<3>(rec) && !cs_labels.contains(at))   // (a CS label is checked in the whole)
            out.set(at, {mnemonic(at), std::get<0>(rec), py::bytes::zeros(std::get<0>(rec).size())});
    }
    return out;
}

py::dict<str, int64_t> Function::raw_calls_into_holes() {
    // {name: offset} of each function rebuilt from C (a hole) that a near call or jump in the part of the frame NOT followed as
    // code -- written out as its bytes -- goes to: the bytes hold the displacement to where the function was, and it is placed
    // elsewhere (USER's 25:2460 `E8 59 FB`, call 1FBC, in code reached only through a table).
    const Md& md = decoder_(bits, false);
    const int64_t size = bits == 16 ? 3 : 5;
    py::set<int64_t> decoded;
    for (const auto& kv : ins.items()) {
        const int64_t n = std::get<0>(std::get<1>(kv)).size();
        for (int64_t x = std::get<0>(kv); x < std::get<0>(kv) + n; ++x) decoded.add(x);
    }
    py::dict<int64_t, str> hole_names;
    std::vector<std::pair<int64_t, int64_t>> hole_spans;
    for (const auto& h : holes) {
        hole_names.set(std::get<0>(h), std::get<2>(h));
        hole_spans.push_back({std::get<0>(h), std::get<1>(h)});
    }
    auto in_hole = [&](int64_t x) {
        for (const auto& hs : hole_spans)
            if (hs.first <= x && x < hs.second) return true;
        return false;
    };
    py::dict<str, int64_t> out;
    int64_t at = start;
    while (at < end) {
        if (decoded.contains(at) || in_hole(at)) {
            at += 1;
            continue;
        }
        int64_t stop = at;
        while (stop < end && !decoded.contains(stop) && !in_hole(stop)) stop++;
        for (const Ins& i : md.all(slice_(blob, at, stop), at)) {
            if ((i.b0() == 0xE8 || i.b0() == 0xE9) && i.size == size && bits == 16) {
                const int64_t rel = static_cast<int16_t>(static_cast<uint16_t>(static_cast<unsigned char>(i.bytes[1]) | (static_cast<unsigned char>(i.bytes[2]) << 8)));
                const int64_t target = (i.address + size + rel) & 0xFFFF;
                if (hole_names.contains(target)) {
                    if (!out.contains(hole_names.at(target))) out.set(hole_names.at(target), i.address);
                    raw_hole_calls.set(i.address, {hole_names.at(target), i.b0()});
                }
            }
        }
        at = stop;
    }
    return out;
}

str Function::write(const std::optional<py::set<int64_t>>& good_in, const std::optional<int64_t>& lo_in, const std::optional<int64_t>& hi_in) {
    // The source, with the instructions in `good` (offsets) as mnemonics and the rest as bytes. With `lo`, `hi`: only that piece
    // of it -- one between two functions rebuilt from C, in a frame whose places stay where they were: what points out of the
    // piece (a jump, a place read through CS, a table's entry) is written as the original's bytes, which stay right.
    const int64_t lo = lo_in ? *lo_in : start;
    const int64_t hi = hi_in ? *hi_in : end;
    std::optional<py::set<int64_t>> good = good_in;
    if (bits == 32) good = py::set<int64_t>();   // (its instructions as their bytes: see __init__)
    if (!holes.empty() && !_raw_scanned) {
        // (a call in bytes not followed as code to a function rebuilt from C is written by its name: the function is placed
        // elsewhere, and the displacement in the bytes would not)
        _raw_scanned = true;
        raw_calls_into_holes();
        for (const auto& kv : raw_hole_calls.items()) externs.set(std::get<0>(std::get<1>(kv)), str("near"));
    }
    auto inside = [&](int64_t x) { return lo <= x && x < hi; };
    auto is_good = [&](int64_t a) { return !good || good->contains(a); };
    // `at in good` as the original writes it: with `good` None (the documented "every instruction") that is a TypeError, as
    // Python raises it -- the original's own slip, kept as it behaves (an instruction is reached first as `at in self.ins`)
    auto good_in_ = [&](int64_t a) -> bool {
        if (!good) throw py::TypeError("argument of type 'NoneType' is not a container or iterable");
        return good->contains(a);
    };
    py::set<int64_t> written;   // the starts
    if (!good) for (int64_t a : ins.keys()) written.add(a);
    else for (int64_t a : *good) if (ins.contains(a)) written.add(a);
    py::set<int64_t> covered;   // (inside an instruction)
    for (int64_t a : written) {
        const int64_t n = std::get<0>(ins.at(a)).size();
        for (int64_t x = a + 1; x < a + n; ++x) covered.add(x);
    }
    std::vector<str> out;
    out.push_back(str("; ") + module + str(", ") + public_ + str(": ") + num_(seg) + str(":") + hx_(lo, 4) + str("-") + hx_(hi, 4) +
                  str(" -- written in assembly, as its author wrote it: what the"));
    out.push_back(str("; loader fixes and the calls out of the function are written as what they refer to;"));
    out.push_back(str("; an instruction the assembler would encode differently is given as bytes."));
    out.push_back(str(".386p"));
    out.push_back(str("CODE") + num_(seg) + str(" segment byte public use16 'MCODE'"));
    out.push_back(str("        assume cs:CODE") + num_(seg) + str(", ds:nothing, es:nothing, ss:nothing"));
    py::set<str> mine;
    for (const auto& p : publics)
        if (inside(std::get<0>(p))) mine.add(std::get<1>(p));
    py::dict<str, str> others;   // (other pieces of the frame)
    for (const auto& p : publics)
        if (!inside(std::get<0>(p))) others.set(std::get<1>(p), str("near"));
    {
        py::dict<str, str> merged = others;
        for (const auto& kv : externs.items()) merged.set(std::get<0>(kv), std::get<1>(kv));
        std::vector<std::tuple<str, str>> rows = merged.items();
        std::sort(rows.begin(), rows.end());
        for (const auto& row : rows)
            if (!mine.contains(std::get<0>(row))) out.push_back(str("        extrn ") + std::get<0>(row) + str(":") + std::get<1>(row));
    }
    for (const auto& p : publics)
        if (inside(std::get<0>(p))) out.push_back(str("        public ") + std::get<1>(p));
    py::dict<int64_t, str> entry;
    for (const auto& p : publics)
        if (inside(std::get<0>(p))) entry.set(std::get<0>(p), std::get<1>(p));
    // an instruction something enters in its middle (hand-written overlapping entries: `cmp ax, 0B1xxh` chains entered one byte
    // in, where they read `mov cl, xx`) is written as its bytes -- the same bytes, and the place inside it gets its label
    py::set<int64_t> targets;
    for (int64_t o : entry.keys()) targets.add(o);
    for (int64_t x : labels)
        if (inside(x)) targets.add(x);
    py::set<int64_t> split;
    for (int64_t a : ins.keys()) {
        if (!inside(a)) continue;
        const int64_t n = std::get<0>(ins.at(a)).size();
        for (int64_t t = a + 1; t < a + n; ++t)
            if (targets.contains(t)) {
                split.add(a);
                break;
            }
    }
    std::vector<int64_t> run;
    auto flush = [&]() {
        for (size_t i = 0; i < run.size(); i += 16) {
            std::vector<str> parts;
            for (size_t k = i; k < run.size() && k < i + 16; ++k) parts.push_back(str("0") + hx_(run[k], 2, true) + str("h"));
            out.push_back(str("        db   ") + py::join(str(", "), py::list<str>(parts)));
        }
        run.clear();
    };
    int64_t at = lo;
    while (at < hi) {
        const std::tuple<int64_t, int64_t, str>* h = nullptr;
        for (const auto& x : holes)
            if (std::get<0>(x) == at) {
                h = &x;
                break;
            }
        if (h) {
            flush();
            out.push_back(str("        db   ") + num_(std::get<1>(*h) - std::get<0>(*h)) + str(" dup (0CCh)                         ; ") + std::get<2>(*h) +
                          str(", rebuilt from C, is placed after this"));
            at = std::get<1>(*h);
            continue;
        }
        if (entry.contains(at)) {
            flush();
            out.push_back(entry.at(at) + str(":"));
        }
        if (labels.contains(at) && _used(at, lo, hi)) {
            flush();
            out.push_back(str("L_") + hx_(at, 4, true) + str(":"));
        }
        if (symbolic.contains(at) && good) {
            flush();
            const auto& sz = symbolic.at(at);
            out.push_back(str("        ") + std::get<1>(sz));
            at += std::get<0>(sz);
        } else if (sites.contains(at)) {
            flush();
            const auto& wt = sites.at(at);
            out.push_back(str("        ") + std::get<1>(wt));
            at += std::get<0>(wt);
        } else if (tables.contains(at) && inside(u16_(blob, at))) {
            flush();
            const int64_t t = u16_(blob, at);
            // an entry is a label where an instruction of this piece is written; anywhere else (a function rebuilt from C, the
            // middle of an instruction: a table guessed too long reads on into data) it is the number it is -- the same bytes
            bool in_hole = false;
            for (const auto& x : holes) in_hole = in_hole || (std::get<0>(x) <= t && t < std::get<1>(x));
            const bool held = ins.contains(t) && is_good(t) && !covered.contains(t) && !in_hole;
            out.push_back(held ? str("        dw   L_") + hx_(t, 4, true) : str("        dw   0") + hx_(t, 4, true) + str("h"));
            at += 2;
        } else if (calls.contains(at)) {
            flush();
            const auto& c = calls.at(at);
            out.push_back(str("        ") + std::get<1>(c));
            at += std::get<0>(c);
        } else if (ins.contains(at) && (good_in_(at)) && !split.contains(at) && _stays_inside(at, lo, hi)) {
            flush();
            out.push_back(str("        ") + mnemonic(at));
            at += std::get<0>(ins.at(at)).size();
        } else if (ins.contains(at) && !split.contains(at)) {
            flush();
            const auto& rec = ins.at(at);
            const py::bytes& data = std::get<0>(rec);
            std::vector<str> parts;
            for (int64_t b : data) parts.push_back(str("0") + hx_(b, 2, true) + str("h"));
            out.push_back(str("        db   ") + py::join(str(", "), py::list<str>(parts)).ljust(40) + str(" ; ") + std::get<1>(rec) + str(" ") + std::get<2>(rec));
            at += data.size();
        } else if (raw_hole_calls.contains(at)) {
            flush();
            const auto& rc = raw_hole_calls.at(at);
            out.push_back(str("        ") + str(std::get<1>(rc) == 0xE8 ? "call" : "jmp ") + str(" near ptr ") + std::get<0>(rc));
            at += bits == 16 ? 3 : 5;
        } else {
            run.push_back(blob[at]);
            at += 1;
        }
    }
    flush();
    out.push_back(str("CODE") + num_(seg) + str(" ends"));
    out.push_back(str("end"));
    out.push_back(str(""));
    const str text_ = py::join(str("\n"), py::list<str>(out));
    // (a table read on into what is code or data: an entry whose place got no label is the number it is -- the same two bytes)
    py::set<str> defined;
    for (const re::Match& m : re::Pattern(str("(?m)^(L_[0-9A-F]{4}):"), 0).finditer(text_)) defined.add(m.group(1));
    return re::Pattern(str("(?m)^(        dw   )(L_([0-9A-F]{4}))$"), 0).sub(
        [&](const re::Match& mm) { return defined.contains(mm.group(2)) ? mm.group(0) : mm.group(1) + str("0") + mm.group(3) + str("h"); }, text_);
}

// ---------------------------------------------------------------------------------------------------------------------

py::list<int64_t> differences(const py::bytes& code, const py::bytes& mask, const Function& f) {
    // The offsets (in the segment) where the assembled bytes are not the original's, the places the loader or the linker sets
    // aside.
    const py::bytes want = f.blob.slice(f.start, f.end);
    py::set<int64_t> fixed;
    for (const auto& kv : f.sites.items())
        for (int64_t x = std::get<0>(kv) - f.start; x < std::get<0>(kv) - f.start + std::get<0>(std::get<1>(kv)); ++x) fixed.add(x);
    for (const auto& kv : f.calls.items())
        for (int64_t x = std::get<0>(kv) - f.start + 1; x < std::get<0>(kv) - f.start + std::get<0>(std::get<1>(kv)); ++x) fixed.add(x);
    for (const auto& kv : f.symbolic.items())
        for (int64_t x = std::get<0>(kv) - f.start + 1; x < std::get<0>(kv) - f.start + std::get<0>(std::get<1>(kv)); ++x) fixed.add(x);
    for (int64_t t : f.tables) {
        fixed.add(t - f.start);
        fixed.add(t - f.start + 1);
    }
    for (const auto& h : f.holes)
        for (int64_t x = std::get<0>(h) - f.start; x < std::get<1>(h) - f.start; ++x) fixed.add(x);
    py::list<int64_t> out;
    const int64_t n = std::min(code.size(), want.size());
    for (int64_t i = 0; i < n; ++i)
        if (code.byte_(i) != want.byte_(i) && !fixed.contains(i) && !mask.byte_(i)) out.append(f.start + i);
    return out;
}

str check_piece(const py::bytes& obj, Function& f, int64_t lo, int64_t hi) {
    // A piece of a frame (see Function.write) against the original. -> "" or what differs.
    auto [code, mask] = msc6::object_code(obj);
    if (code.size() != hi - lo) return num_(code.size()) + str(" bytes, the original ") + num_(hi - lo);
    const int64_t start = f.start, end = f.end;
    f.start = lo;
    f.end = hi;
    py::list<int64_t> bad;
    try {
        bad = differences(code, mask, f);
    } catch (...) {
        f.start = start;
        f.end = end;
        throw;
    }
    f.start = start;
    f.end = end;
    return !bad.empty() ? str("differs at ") + hx_(bad[0], 4) : str("");
}

str check(const py::bytes& obj, const Function& f) {
    // The assembled object against the original: every byte the same but those the loader or the linker sets. -> "" or where
    // they differ.
    auto [code, mask] = msc6::object_code(obj);
    const py::bytes want = f.blob.slice(f.start, f.end);
    if (code.size() != want.size()) return num_(code.size()) + str(" bytes, the original ") + num_(want.size());
    py::list<int64_t> bad = differences(code, mask, f);
    return !bad.empty() ? str("differs at ") + hx_(bad[0], 4) : str("");
}

}  // namespace codebreaker::build::asm16
