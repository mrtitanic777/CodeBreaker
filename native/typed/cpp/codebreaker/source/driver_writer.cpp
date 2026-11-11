// codebreaker/source/driver_writer.cpp -- typed C++ of codebreaker.source.driver_writer (see driver_writer.hpp).

#include "codebreaker/source/driver_writer.hpp"

#include <algorithm>
#include <cstdio>
#include <string>
#include <unordered_set>
#include <vector>

#include <capstone/capstone.h>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/driver.hpp"
#include "codebreaker/build/vxdfile.hpp"
#include "codebreaker/symbols/driver_services.hpp"
#include "codebreaker/translate/backends/asm.hpp"
#include "py_json.hpp"
#include "py_settings.hpp"
#include "py_stdlib.hpp"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

namespace codebreaker::source::driver_writer {

namespace assembly = codebreaker::source::assembly;
namespace builder = codebreaker::build::driver;
namespace vxdfile = codebreaker::build::vxdfile;
namespace driver_services = codebreaker::symbols::driver_services;
namespace l2 = codebreaker::translate::l2;
namespace asm_ = codebreaker::translate::backends::asm_;

const int64_t _MARK = 0x7A000000;   // stands in for a patched address while decoding
const int64_t _MAX_ROUNDS = 40;
const py::set<py::str> _COMMON = {py::str("mov"), py::str("add"),  py::str("sub"),  py::str("cmp"),  py::str("lea"),   py::str("push"),
                                  py::str("pop"), py::str("call"), py::str("jmp"),  py::str("test"), py::str("and"),   py::str("or"),
                                  py::str("xor"), py::str("inc"),  py::str("dec"),  py::str("ret"),  py::str("nop"),   py::str("mov zx"),
                                  py::str("movzx"), py::str("movsx"), py::str("shl"), py::str("shr"), py::str("adc"), py::str("sbb")};

// ---- the capstone decoder with detail (platforms.decoder(32, detail=True)) ---------------------------------------
namespace {

struct CsLib {
#ifdef _WIN32
    HMODULE module = nullptr;
#endif
    decltype(&cs_open) open = nullptr;
    decltype(&cs_option) option = nullptr;
    decltype(&cs_malloc) malloc_ = nullptr;
    decltype(&cs_disasm_iter) disasm_iter = nullptr;
    csh handle = 0;
    cs_insn* insn = nullptr;
};

CsLib& cs_lib() {
    static CsLib lib;
    return lib;
}
std::wstring& dll_path() {
    static std::wstring p;
    return p;
}

template <class F> F sym_(HMODULE m, const char* name) {
    FARPROC p = GetProcAddress(m, name);
    if (!p) {
        throw py::ImportError(std::string("capstone.dll has no ") + name);
    }
    return reinterpret_cast<F>(p);
}

CsLib& cs_ready() {
    CsLib& lib = cs_lib();
    if (lib.handle) {
        return lib;
    }
    if (!lib.module) {
        if (dll_path().empty()) {
            throw py::ImportError("capstone: no DLL given (set_capstone_dll)");
        }
        lib.module = LoadLibraryW(dll_path().c_str());
        if (!lib.module) {
            throw py::ImportError("capstone: cannot load the DLL");
        }
        lib.open = sym_<decltype(&cs_open)>(lib.module, "cs_open");
        lib.option = sym_<decltype(&cs_option)>(lib.module, "cs_option");
        lib.malloc_ = sym_<decltype(&cs_malloc)>(lib.module, "cs_malloc");
        lib.disasm_iter = sym_<decltype(&cs_disasm_iter)>(lib.module, "cs_disasm_iter");
    }
    csh h = 0;
    if (lib.open(CS_ARCH_X86, CS_MODE_32, &h) != CS_ERR_OK) {
        throw py::RuntimeError("capstone: cs_open failed");
    }
    lib.option(h, CS_OPT_DETAIL, CS_OPT_ON);
    lib.handle = h;
    lib.insn = lib.malloc_(h);
    return lib;
}

// what next(md.disasm(code, address), None) gives, as far as _one reads it
struct Decoded {
    int64_t size = 0;
    py::str mnemonic, op_str;
    bool jump_or_call = false;      // X86_GRP_JUMP or X86_GRP_CALL in ins.groups
    bool first_imm = false;         // ins.operands and ins.operands[0].type == X86_OP_IMM
    int64_t imm = 0;                // ins.operands[0].imm
};

bool decode_first(const py::bytes& code, int64_t address, Decoded& out) {
    CsLib& lib = cs_ready();
    const uint8_t* p = reinterpret_cast<const uint8_t*>(code.raw().data());
    size_t sz = static_cast<size_t>(code.size());
    uint64_t addr = static_cast<uint64_t>(address);
    if (!lib.disasm_iter(lib.handle, &p, &sz, &addr, lib.insn)) {
        return false;
    }
    const cs_insn* in = lib.insn;
    out.size = in->size;
    out.mnemonic = py::str(std::string(in->mnemonic));
    out.op_str = py::str(std::string(in->op_str));
    out.jump_or_call = false;
    out.first_imm = false;
    if (in->detail) {
        for (int k = 0; k < in->detail->groups_count; ++k) {
            if (in->detail->groups[k] == X86_GRP_JUMP || in->detail->groups[k] == X86_GRP_CALL) {
                out.jump_or_call = true;
            }
        }
        if (in->detail->x86.op_count > 0 && in->detail->x86.operands[0].type == X86_OP_IMM) {
            out.first_imm = true;
            out.imm = in->detail->x86.operands[0].imm;
        }
    }
    return true;
}

// the L2 Module document the back end reads, as its typed Document (the keys it reads: node, units; of a unit: node, asm,
// within, name, text, syntax)
std::optional<py::str> unit_str(const l2::Dict& u, const char* key) {
    std::optional<l2::Val> v = u.get(py::str(key));
    if (!v.has_value()) {
        return std::nullopt;
    }
    if (const py::str* s = std::get_if<py::str>(&v->v)) {
        return *s;
    }
    return std::nullopt;
}

asm_::Document to_document(const l2::Dict& d) {
    asm_::Document doc;
    doc.node = unit_str(d, "node");
    py::list<asm_::Unit> units;
    std::optional<l2::Val> us = d.get(py::str("units"));
    if (us.has_value()) {
        if (const l2::Val::List* l = std::get_if<l2::Val::List>(&us->v)) {
            for (const l2::Val& x : *l) {
                const l2::Dict& u = std::get<l2::Dict>(x.v);
                asm_::Unit t;
                t.node = unit_str(u, "node");
                t.asm_ = unit_str(u, "asm");
                std::optional<l2::Val> w = u.get(py::str("within"));
                t.within = w.has_value() && !w->is_none() && !(std::holds_alternative<bool>(w->v) && !std::get<bool>(w->v));
                t.name = unit_str(u, "name");
                t.text = unit_str(u, "text");
                t.syntax = unit_str(u, "syntax");
                units.append(t);
            }
        }
    }
    doc.units = units;
    return doc;
}

// json.dump(value, fh) of an L2 value: ", " and ": " separators, ensure_ascii
void json_put(std::string& o, const l2::Val& v) {
    if (v.is_none()) {
        o += "null";
    } else if (const bool* b = std::get_if<bool>(&v.v)) {
        o += *b ? "true" : "false";
    } else if (const int64_t* n = std::get_if<int64_t>(&v.v)) {
        o += std::to_string(*n);
    } else if (const double* x = std::get_if<double>(&v.v)) {
        py::json::put(o, *x);
    } else if (const py::str* s = std::get_if<py::str>(&v.v)) {
        py::json::put(o, *s);
    } else if (const l2::Val::List* l = std::get_if<l2::Val::List>(&v.v)) {
        o += '[';
        bool first = true;
        for (const l2::Val& x : *l) {
            if (!first) {
                o += ", ";
            }
            json_put(o, x);
            first = false;
        }
        o += ']';
    } else if (const l2::Dict* d = std::get_if<l2::Dict>(&v.v)) {
        o += '{';
        bool first = true;
        for (const auto& kv : d->items()) {
            if (!first) {
                o += ", ";
            }
            py::json::put(o, std::get<0>(kv));
            o += ": ";
            json_put(o, std::get<1>(kv));
            first = false;
        }
        o += '}';
    } else {
        throw py::TypeError("Object of a type is not JSON serializable");
    }
}

l2::Dict origin_of(const py::str& module, int64_t segment, int64_t offset) {
    l2::Dict o;
    o.set(py::str("module"), l2::Val(module));
    o.set(py::str("segment"), l2::Val(segment));
    o.set(py::str("offset"), l2::Val(offset));
    return o;
}

// settings.state(*parts): a path inside the state folder, which is created if it is not there
py::str state_(const py::list<py::str>& parts) {
    py::str path = py::settings::get_str(py::str("state"));
    for (const py::str& p : parts) {
        path = py::os::path::join(path, p);
    }
    py::os::makedirs(parts.empty() ? path : py::os::path::dirname(path), 0777, true);
    return path;
}

void print_(const py::str& s) {
    const std::string u = s.utf8() + "\n";
    std::fwrite(u.data(), 1, u.size(), stdout);
}

}  // namespace

void set_capstone_dll(const py::str& path) {
    std::string u = path.utf8();
    int n = MultiByteToWideChar(CP_UTF8, 0, u.c_str(), -1, nullptr, 0);
    std::wstring w(static_cast<size_t>(n > 0 ? n - 1 : 0), L'\0');
    if (n > 0) {
        MultiByteToWideChar(CP_UTF8, 0, u.c_str(), -1, &w[0], n);
    }
    dll_path() = w;
}

py::str _attributes(int64_t flags) {
    py::list<py::str> words = {py::str((flags & 0x40) ? "preload" : "loadoncall")};
    const std::tuple<int64_t, const char*> table[] = {{0x10, "discardable"}, {0x20, "shared"}, {0x8000, "iopl"}, {0x4000, "conforming"}};
    for (const auto& [bit, w] : table) {
        if (flags & bit) {
            words.append(py::str(w));
        }
    }
    return py::join(py::str(" "), words);
}

DriverObject::DriverObject(const le::LEFile& lf, const le::Object& obj) {
    i = obj.idx;
    flags = obj.flags;
    size = obj.vsize;
    const py::bytes got = le::object_data(lf, obj);
    py::bytes pad;
    if (size - got.size() > 0) {
        pad = py::bytes(std::string(static_cast<size_t>(size - got.size()), '\0'));
    }
    data = got + pad;
    code = (flags & 4) != 0;
    wide = (flags & 0x2000) != 0;                      // 32-bit code; otherwise real-mode start-up code
    raw = data;
}

bool _reads_as_text(const py::dict<int64_t, Insn>& found) {
    std::string joined;
    for (int64_t a : py::sorted(found.keys())) {
        joined += found.at(a).data.raw();
    }
    const int64_t n = static_cast<int64_t>(joined.size());
    int64_t ok = 0;
    for (char c : joined) {
        const int64_t b = static_cast<unsigned char>(c);
        if ((0x20 <= b && b < 0x7F) || b == 0 || b == 10 || b == 13) {
            ok += 1;
        }
    }
    return n >= 6 && static_cast<double>(ok) >= 0.85 * static_cast<double>(n);
}

Writer::Writer(const le::LEFile& lf_) : lf(&lf_) {
    d = le::descriptor(lf_);
    for (const le::Object& o : lf_.objects) {
        objects.set(o.idx, std::make_shared<DriverObject>(lf_, o));
    }
    for (const auto& [o, a, kind, to, ta] : le::references(lf_)) {
        if (objects.contains(o) && objects.contains(to)) {
            objects.at(o)->fix.set(a, {kind, to, ta});
            objects.at(to)->labels.add(ta);
        }
    }
    if (d) {
        objects.at(d->obj)->labels.add(d->offset);
    }
    const py::str device = d ? d->name.strip() : py::str("DRV");
    static const py::re::Pattern ident = py::re::compile(py::str("[A-Za-z_]\\w*"));
    static const py::re::Pattern vmm = py::re::compile(py::str("(VMM|VxD)_[0-9A-F]{4}"));
    for (const auto& [label_, place] : py::sorted(le::entry_points(lf_).items())) {
        const auto [o, a] = place;
        py::str name = py::percent(py::str("%s_%s"), device, label_);
        if (label_.startswith(py::str("service_")) && d && d->device_id) {
            // a service this driver offers has a name of its own in the kit
            const py::str known = driver_services::label(d->device_id, py::int_parse(label_.slice_from(8)));
            if (ident.fullmatch(known).has_value() && !vmm.match(known).has_value()) {
                name = known;
            }
        }
        names.setdefault(place, name);
        objects.at(o)->labels.add(a);
    }
    if (d) {
        names.set(Place(d->obj, d->offset), py::percent(py::str("%s_DDB"), d->name.strip()));
    }
    _within = std::nullopt;
    l2 = std::nullopt;                                 // the file as one L2 tree, once written (source)
    _decode();
    // places inside an instruction, and every name that will be needed, before writing starts
    py::dict<Place, int64_t> within;
    for (const auto& io : objects.items()) {
        const int64_t i = std::get<0>(io);
        for (const auto& kv : std::get<1>(io)->ins.items()) {
            const int64_t off = std::get<0>(kv);
            for (int64_t x = off + 1; x < off + std::get<1>(kv).data.size(); ++x) {
                within.set(Place(i, x), off);
            }
        }
    }
    _within = within;
    for (const auto& io : objects.items()) {
        const std::shared_ptr<DriverObject>& ob = std::get<1>(io);
        py::set<int64_t> kept;
        for (int64_t a : ob->labels) {
            if (0 <= a && a <= ob->size) {
                kept.add(a);
            }
        }
        ob->labels = kept;
        for (const auto& fv : ob->fix.values()) {
            label(std::get<1>(fv), std::get<2>(fv));
        }
    }
}

py::str Writer::label(int64_t o, int64_t a) {
    const std::shared_ptr<DriverObject>& ob = objects.at(o);
    int64_t base = a;
    if (!(0 <= a && a <= ob->size)) {
        base = (a < 0 || a > 0x7FFFFFFF) ? 0 : ob->size;
        a = a > 0x7FFFFFFF ? a - (int64_t(1) << 32) : a;
    } else if (_within && _within->contains(Place(o, a))) {
        base = _within->at(Place(o, a));
    } else {
        for (int64_t back : {1, 2, 3}) {
            if (ob->fix.contains(a - back)) {
                base = a - back;
                break;
            }
        }
    }
    if (base != a) {
        ob->labels.add(base);
        return py::percent(py::str("%s %s %d"), label(o, base), py::str(a > base ? "+" : "-"), py::abs(a - base));
    }
    const auto n = names.get(Place(o, a));
    if (n && !n->empty()) {
        return *n;
    }
    return py::percent(py::str("L%d_%05X"), o, a);
}

// -- finding the code ------------------------------------------------------------------------
void Writer::_decode() {
    struct Item {
        int64_t o, a;
        bool maybe;
    };
    std::vector<Item> work;
    for (const auto& oa : le::entry_points(*lf).values()) {
        work.push_back({std::get<0>(oa), std::get<1>(oa), false});
    }
    for (const auto& io : objects.items()) {
        const std::shared_ptr<DriverObject>& ob = std::get<1>(io);
        for (const auto& kv : ob->fix.items()) {
            const auto& [kind, to, ta] = std::get<1>(kv);
            if (objects.at(to)->code) {
                work.push_back({to, ta, kind != 8});
            }
        }
    }
    py::set<Place> tried;
    while (!work.empty()) {
        const Item item = work.back();
        work.pop_back();
        const int64_t o = item.o, a = item.a;
        const bool maybe = item.maybe;
        const std::shared_ptr<DriverObject> ob = objects.get(o);
        if (!ob || !ob->code || !ob->wide || tried.contains(Place(o, a)) || ob->ins.contains(a)) {
            continue;
        }
        tried.add(Place(o, a));
        auto [found, more, ok] = _follow(*ob, a);
        if (maybe && (!ok || _reads_as_text(found))) {
            continue;                                  // a pointer into code that is not code
        }
        for (const auto& kv : found.items()) {
            ob->ins.set(std::get<0>(kv), std::get<1>(kv));
        }
        for (int64_t t : more) {
            ob->labels.add(t);
            work.push_back({o, t, false});
        }
    }
    // drop instructions another instruction's bytes run into (data read as code by mistake)
    for (const auto& io : objects.items()) {
        const std::shared_ptr<DriverObject>& ob = std::get<1>(io);
        std::unordered_set<int64_t> covered;
        for (int64_t off : py::sorted(ob->ins.keys())) {
            const int64_t n = ob->ins.at(off).data.size();
            bool hit = false;
            for (int64_t x = off; x < off + n && !hit; ++x) {
                hit = covered.count(x) != 0;
            }
            // ...or that something points into the middle of
            for (int64_t x = off + 1; x < off + n && !hit; ++x) {
                hit = ob->labels.contains(x);
            }
            if (hit) {
                ob->ins.pop(off);
                continue;
            }
            for (int64_t x = off; x < off + n; ++x) {
                covered.insert(x);
            }
        }
        // ...and jumps to places that are not the start of anything (so not code either)
        bool again = true;
        while (again) {
            again = false;
            std::unordered_set<int64_t> inside;
            for (const auto& kv : ob->ins.items()) {
                for (int64_t x = std::get<0>(kv) + 1; x < std::get<0>(kv) + std::get<1>(kv).data.size(); ++x) {
                    inside.insert(x);
                }
            }
            for (int64_t a : ob->fix) {
                for (int64_t k : {1, 2, 3}) {
                    inside.insert(a + k);
                }
            }
            for (int64_t off : ob->ins.keys()) {
                const auto& target = ob->ins.at(off).target;
                if (target && inside.count(*target)) {
                    ob->ins.pop(off);
                    again = true;
                }
            }
        }
        py::set<int64_t> drop;
        for (int64_t a : ob->fix) {
            for (int64_t k : {1, 2, 3}) {
                drop.add(a + k);
            }
        }
        ob->labels = ob->labels - drop;
    }
}

std::optional<One> Writer::_one(DriverObject& ob, int64_t a) {
    if (ob.memo.contains(a)) {
        return ob.memo.at(a);
    }
    const py::bytes& data = ob.raw;
    std::optional<One> out;
    bool any_fix = false;
    for (int64_t x = a; x < a + 6; ++x) {
        any_fix = any_fix || ob.fix.contains(x);
    }
    if (data.slice(a, a + 2) == py::bytes(std::string("\xCD\x20", 2)) && a + 6 <= ob.size && !any_fix) {
        const auto [svc, dev] = py::unpack_from<int64_t, int64_t>("<HH", data, a + 2);
        if (0 < dev && dev < 0x8000 && (svc & 0x7FFF) < 0x800) {
            ob.services.set(a, {dev, svc & 0x7FFF, (svc & 0x8000) != 0});
            One one;
            one.ins = Insn{data.slice(a, a + 6), py::str("service"), py::str(""), py::str("service"), std::nullopt};
            one.next = (svc & 0x8000) ? std::nullopt : std::optional<int64_t>(a + 6);
            out = one;
        }
    }
    if (!out) {
        // patched fields read as a marker, so the operand they belong to can be recognised
        py::bytearray window(data.slice(a, a + 16));
        for (int64_t x = a; x < std::min<int64_t>(a + 13, ob.size); ++x) {
            if (ob.fix.contains(x)) {
                py::pack_into("<I", window, x - a, _MARK + (x & 0xFFFFF));
            }
        }
        Decoded ins;
        if (decode_first(py::bytes(window.view()), a, ins) && a + ins.size <= ob.size) {
            py::list<int64_t> inside;
            for (int64_t x = a; x < a + ins.size; ++x) {
                if (ob.fix.contains(x)) {
                    inside.append(x);
                }
            }
            bool overrun = false, before = false;
            for (int64_t x : inside) {
                overrun = overrun || x + 4 > a + ins.size;
            }
            for (int64_t k : {1, 2, 3}) {
                before = before || ob.fix.contains(a - k);
            }
            if (!(overrun || before)) {
                std::optional<int64_t> target;
                if (ins.jump_or_call && ins.first_imm && inside.empty()) {
                    int64_t t = ins.imm & 0xFFFFFFFF;
                    if (t >= ob.size) {
                        target = std::nullopt;
                    } else {
                        target = t;
                    }
                }
                const py::str& m = ins.mnemonic;
                const bool ends = m == "ret" || m == "retn" || m == "retf" || m == "iretd" || m == "iret" || m == "jmp" || m == "hlt";
                One one;
                one.ins = Insn{data.slice(a, a + ins.size), ins.mnemonic, ins.op_str, py::str(""), target};
                one.next = ends ? std::nullopt : std::optional<int64_t>(a + ins.size);
                out = one;
            }
        }
    }
    ob.memo.set(a, out);
    return out;
}

std::tuple<py::dict<int64_t, Insn>, py::list<int64_t>, bool> Writer::_follow(DriverObject& ob, int64_t start) {
    py::dict<int64_t, Insn> found;
    py::list<int64_t> more;
    int64_t a = start;
    while (0 <= a && a < ob.size && !ob.ins.contains(a) && !found.contains(a)) {
        const std::optional<One> one = _one(ob, a);
        if (!one) {
            return {found, more, false};
        }
        found.set(a, one->ins);
        if (one->ins.target) {
            more.append(*one->ins.target);
        }
        if (!one->next) {
            return {found, more, true};
        }
        a = *one->next;
    }
    return {found, more, true};
}

// -- writing -------------------------------------------------------------------------------
std::optional<py::str> Writer::mnemonic(const DriverObject& ob, int64_t off) {
    static const py::re::Pattern string_ops = py::re::compile(assembly::STRING_OPS);
    const Insn& ins = ob.ins.at(off);
    const int64_t n = ins.data.size();
    if (ins.mnem == "service") {
        return std::nullopt;
    }
    py::list<int64_t> fixes;
    for (int64_t x = off; x < off + n; ++x) {
        if (ob.fix.contains(x)) {
            fixes.append(x);
        }
    }
    if (fixes.size() > 1) {
        return std::nullopt;
    }
    const auto m = string_ops.match(ins.mnem + py::str(" "));
    if (m && fixes.empty()) {
        const auto g1 = m->group_opt(1);
        return ((g1 ? *g1 : py::str("")) + m->group(2) + m->group(3)).strip();
    }
    if (ins.target) {
        // the distance is said, so the assembler keeps the original's size
        if (ins.mnem == "loop" || ins.mnem == "loope" || ins.mnem == "loopne" || ins.mnem == "jecxz" || ins.mnem == "jcxz") {
            return py::percent(py::str("%-6s %s"), ins.mnem, label(ob.i, *ins.target));
        }
        const char* how = n == 2 ? "short" : "near ptr";
        return py::percent(py::str("%-6s %s %s"), ins.mnem, py::str(how), label(ob.i, *ins.target));
    }
    py::str o = ins.ops;
    if (!fixes.empty()) {
        const auto [kind, to, ta] = ob.fix.at(fixes[0]);
        const py::str marker = py::percent(py::str("0x%x"), _MARK + (fixes[0] & 0xFFFFF));
        if (!o.contains(marker)) {
            return std::nullopt;
        }
        const py::str name = label(to, ta);
        if (kind == 8) {
            o = o.replace(marker, name);
        } else {
            const py::re::Pattern mem = py::re::compile(py::str("\\[[^\\]]*") + marker + py::str("[^\\]]*\\]"));
            const bool in_memory = mem.search(o).has_value();
            o = o.replace(marker, in_memory ? name : py::str("offset ") + name);
        }
    }
    o = assembly::operands(o);
    return py::percent(py::str("%-6s %s"), assembly::mnemonic(ins.mnem), o).rstrip();
}

py::list<py::str> Writer::_raw(const DriverObject& ob, int64_t off) {
    const Insn& ins = ob.ins.at(off);
    const py::bytes& data = ins.data;
    const int64_t n = data.size();
    if (ins.mnem == "service") {
        const auto [dev, svc, jump] = ob.services.at(off);
        return {py::str("        int  20h"),
                py::percent(py::str("        dd   0%08Xh    ; %s%s"), py::unpack_from1<int64_t>("<I", data, 2), py::str(jump ? "jump to " : ""),
                            driver_services::label(dev, svc))};
    }
    py::list<py::str> lines;
    std::vector<int64_t> run;
    int64_t x = off;

    auto flush = [&]() {
        if (!run.empty()) {
            py::list<py::str> parts;
            for (int64_t b : run) {
                parts.append(py::percent(py::str("0%02Xh"), b));
            }
            lines.append(py::str("        db   ") + py::join(py::str(", "), parts));
            run.clear();
        }
    };
    while (x < off + n) {
        if (ob.fix.contains(x)) {
            const auto [kind, to, ta] = ob.fix.at(x);
            const py::str name = label(to, ta);
            if (kind == 8 && !run.empty() && (run.back() == 0xE8 || run.back() == 0xE9)) {
                const int64_t op = run.back();
                run.pop_back();
                flush();
                lines.append(py::percent(py::str("        %s %s"), py::str(op == 0xE8 ? "call" : "jmp "), name));
            } else if (kind == 8 && run.size() > 1 && run[run.size() - 2] == 0x0F && 0x80 <= run.back() && run.back() <= 0x8F) {
                const py::str cc = assembly::CONDITIONS[run.back() - 0x80];
                run.pop_back();
                run.pop_back();
                flush();
                lines.append(py::percent(py::str("        j%-3s near ptr %s"), cc, name));
            } else {
                flush();
                lines.append(py::percent(py::str("        dd   offset %s%s"), name, py::str(kind == 8 ? " - ($ + 4)" : "")));
            }
            x += 4;
        } else {
            run.push_back(data[x - off]);
            x += 1;
        }
    }
    flush();
    std::optional<py::str> mt = mnemonic(ob, off);
    const py::str text = (mt && !mt->empty()) ? *mt : py::percent(py::str("%s %s"), ins.mnem, ins.ops).strip();
    if (!lines.empty()) {
        lines[0] = py::percent(py::str("%-44s ; %s"), lines[0], text);
    }
    return lines;
}

py::dict<Place, py::str> Writer::function_starts() {
    py::dict<Place, bool> out;
    for (const auto& oa : le::entry_points(*lf).values()) {
        out.set(oa, true);
    }
    for (const auto& io : objects.items()) {
        const int64_t i = std::get<0>(io);
        const std::shared_ptr<DriverObject>& ob = std::get<1>(io);
        for (const auto& kv : ob->ins.items()) {
            const Insn& ins = std::get<1>(kv);
            if (ins.mnem == "call" && ins.target) {
                out.set(Place(i, *ins.target), true);
            }
        }
        for (const auto& kv : ob->fix.items()) {
            const int64_t a = std::get<0>(kv);
            const auto& [kind, to, ta] = std::get<1>(kv);
            if (kind == 8 && a >= 1 && ob->raw[a - 1] == 0xE8 && ob->ins.contains(a - 1)) {
                out.set(Place(to, ta), true);
            }
        }
    }
    py::dict<Place, py::str> result;
    for (const Place& k : out.keys()) {
        const int64_t o = std::get<0>(k), a = std::get<1>(k);
        if (objects.contains(o) && objects.at(o)->ins.contains(a) && objects.at(o)->labels.contains(a)) {
            result.set(k, label(o, a));
        }
    }
    return result;
}

std::tuple<py::str, Counts, py::dict<int64_t, Place>> Writer::source(const std::optional<py::set<Place>>& good) {
    py::dict<int64_t, Place> line_of;
    const py::str name = d ? d->name.strip() : py::str("DRIVER");
    l2::Assembly out(py::list<py::str>{py::percent(py::str("; %s -- a virtual device driver, reconstructed from the image by CodeBreaker."), name),
                                       py::str(";"),
                                       py::str("; One segment per object of the original. Nothing here is an address: every place the"),
                                       py::str("; loader patches is written as the label it refers to. Labels are L<object>_<offset in the"),
                                       py::str("; original>; names the image itself carries (entry points, the descriptor block) are used"),
                                       py::str("; where there are any. Instructions written as bytes are ones the assembler would encode"),
                                       py::str("; differently from the original; their mnemonic is in the comment."),
                                       py::str(""), py::str(".386p"), py::str(".model flat"), py::str("")},
                     py::str("masm"), name);   // (the file as one L2 tree)
    Counts counts;
    const py::dict<Place, py::str> starts = function_starts();
    for (int64_t i : py::sorted(objects.keys())) {
        const std::shared_ptr<DriverObject>& ob = objects.at(i);
        out.part(py::percent(py::str("S%d"), i), py::str("object"), origin_of(name, i, 0), ob->size);
        py::list<int64_t> mine;
        for (const Place& k : starts) {
            if (std::get<0>(k) == i) {
                mine.append(std::get<1>(k));
            }
        }
        mine.sort();
        py::dict<int64_t, int64_t> ends;                                // (a function runs to the next one)
        for (int64_t k = 0; k < mine.size(); ++k) {
            ends.set(mine[k], k + 1 < mine.size() ? mine[k + 1] : ob->size);
        }
        const py::str cls = py::percent(py::str("O%d%s"), i, py::str(ob->code ? "CODE" : "DATA"));
        out.append(py::percent(py::str("; object %d: %d bytes, %s"), i, ob->size, _attributes(ob->flags)));
        out.append(py::percent(py::str("S%d segment dword public %s '%s'"), i, py::str(ob->wide ? "use32" : "use16"), cls));
        int64_t a = 0;
        std::vector<int64_t> run;

        auto flush = [&]() {
            out.extend(assembly::data_lines(py::bytes(std::string(run.begin(), run.end()))));
            run.clear();
        };
        while (a < ob->size) {
            if (ob->labels.contains(a) && ob->fix.contains(a) && !ob->ins.contains(a) && std::get<0>(ob->fix.at(a)) == 8 && run.size() > 1 &&
                run[run.size() - 2] == 0x0F && 0x80 <= run.back() && run.back() <= 0x8F) {
                const py::str cc = assembly::CONDITIONS[run.back() - 0x80];   // the same, for a conditional jump
                run.pop_back();
                run.pop_back();
                flush();
                const auto [kind, to, ta] = ob->fix.at(a);
                out.append(py::percent(py::str("        j%-3s near ptr %s"), cc, label(to, ta)));
                out.append(py::percent(py::str("%s equ $ - 4"), label(i, a)));
                counts.data -= 2;
                a += 4;
                continue;
            }
            if (ob->labels.contains(a) && ob->fix.contains(a) && !ob->ins.contains(a) && std::get<0>(ob->fix.at(a)) == 8 && !run.empty() &&
                (run.back() == 0xE8 || run.back() == 0xE9)) {
                // A call or jump among the data whose distance field is itself referred to (code that rewrites the jump):
                // written as the instruction, so the loader's patch is kept -- a `dd` of the distance to another object
                // loses it -- and the label is the four bytes before the instruction's end.
                const int64_t op = run.back();
                run.pop_back();
                flush();
                const auto [kind, to, ta] = ob->fix.at(a);
                out.append(py::percent(py::str("        %s %s"), py::str(op == 0xE8 ? "call" : "jmp "), label(to, ta)));
                out.append(py::percent(py::str("%s equ $ - 4"), label(i, a)));
                counts.data -= 1;
                a += 4;
                continue;
            }
            if (ob->labels.contains(a)) {
                flush();
                if (ends.contains(a)) {                                  // (each function a fragment of its own)
                    out.part(starts.at(Place(i, a)), py::str("function"), origin_of(name, i, a), ends.at(a) - a);
                }
                if (d && Place(i, a) == Place(d->obj, d->offset)) {
                    out.append(py::percent(py::str("public %s"), label(i, a)));
                }
                out.append(py::percent(py::str("%s:"), label(i, a)));
            }
            if (ob->ins.contains(a) && !ob->fix.contains(a)) {           // (bytes the loader patches from their first are an address, not an instruction)
                flush();
                std::optional<py::str> text;
                if (!good || good->contains(Place(i, a))) {
                    text = mnemonic(*ob, a);
                }
                counts.instructions += 1;
                if (!text || text->empty()) {
                    counts.raw += ob->ins.at(a).mnem != "service" ? 1 : 0;
                    out.extend(_raw(*ob, a));
                } else {
                    out.append(py::str("        ") + *text);
                    line_of.set(out.size(), Place(i, a));
                }
                a += ob->ins.at(a).data.size();
            } else if (ob->fix.contains(a)) {
                flush();
                const auto [kind, to, ta] = ob->fix.at(a);
                out.append(py::percent(py::str("        dd   offset %s%s"), label(to, ta), py::str(kind == 8 ? " - ($ + 4)" : "")));
                a += 4;
            } else {
                run.push_back(ob->data[a]);
                counts.data += 1;
                a += 1;
            }
        }
        flush();
        if (ob->labels.contains(ob->size)) {
            out.append(py::percent(py::str("%s:"), label(i, ob->size)));
        }
        out.append(py::percent(py::str("S%d ends"), i));
        out.append(py::str(""));
    }
    out.part(py::str("end"), py::str("end"));
    out.append(py::str("end"));
    l2 = out.document();
    return {asm_::module(to_document(*l2)) + py::str("\n"), counts, line_of};
}

py::list<std::tuple<py::str, py::str>> Writer::classes() const {
    py::list<std::tuple<py::str, py::str>> r;
    for (int64_t i : py::sorted(objects.keys())) {
        const std::shared_ptr<DriverObject>& ob = objects.at(i);
        r.append({py::percent(py::str("O%d%s"), i, py::str(ob->code ? "CODE" : "DATA")), _attributes(ob->flags)});
    }
    return r;
}

// -- which instructions the assembler gives back as they were --------------------------------
py::set<Place> Writer::probe(const py::str& outdir) {
    py::dict<Place, assembly::Candidate> cand;
    py::set<Place> good;
    for (const auto& io : objects.items()) {
        const int64_t i = std::get<0>(io);
        const std::shared_ptr<DriverObject>& ob = std::get<1>(io);
        for (const auto& kv : ob->ins.items()) {
            const int64_t off = std::get<0>(kv);
            const Insn& ins = std::get<1>(kv);
            const std::optional<py::str> text = mnemonic(*ob, off);
            if (!text) {
                continue;
            }
            if (ins.target) {
                good.add(Place(i, off));
                continue;
            }
            std::string mask(static_cast<size_t>(ins.data.size()), '\0');
            for (int64_t x = off; x < off + ins.data.size(); ++x) {
                if (ob->fix.contains(x)) {
                    // mask[x-off:x-off+4] = b"\1\1\1\1": the slice is replaced by four bytes (the mask grows if it is shorter)
                    const size_t from = static_cast<size_t>(x - off);
                    const size_t upto = std::min(from + 4, mask.size());
                    mask.replace(from, upto - from, std::string(4, '\1'));
                }
            }
            std::string kept;
            const size_t pairs = std::min(mask.size(), static_cast<size_t>(ins.data.size()));
            for (size_t k = 0; k < pairs; ++k) {
                if (!mask[k]) {
                    kept.push_back(ins.data.raw()[k]);
                }
            }
            cand.set(Place(i, off), assembly::Candidate(*text, py::bytes(kept), py::bytes(mask)));
        }
    }
    py::set<py::str> near_labels;   // (`near` is a macro of windows.h)
    for (const auto& io : objects.items()) {
        for (const auto& fv : std::get<1>(io)->fix.values()) {
            if (std::get<0>(fv) == 8) {
                near_labels.add(label(std::get<1>(fv), std::get<2>(fv)));
            }
        }
    }
    py::set<py::str> names_set;
    for (const auto& io : objects.items()) {
        const int64_t i = std::get<0>(io);
        const py::set<int64_t>& labels = std::get<1>(io)->labels;
        const std::vector<int64_t> snap(labels.begin(), labels.end());   // (label() may add to the set while this walks)
        for (int64_t a : snap) {
            names_set.add(label(i, a));
        }
    }
    py::set<py::str> plain;
    for (const py::str& n : names_set) {
        if (!n.contains(py::str("+"))) {
            plain.add(n);
        }
    }
    const py::set<assembly::Candidate> ok = assembly::probe(py::list<assembly::Candidate>::from(cand.values()), plain, outdir, py::str("flat32"), near_labels);
    for (const auto& kv : cand.items()) {
        if (ok.contains(std::get<1>(kv))) {
            good.add(std::get<0>(kv));
        }
    }
    return good;
}

std::tuple<py::set<Place>, py::list<int64_t>> _differences(Writer& w, const le::LEFile& built, const py::set<Place>& good) {
    py::set<Place> bad;
    py::list<int64_t> differing;
    py::dict<int64_t, le::Object> objs;
    for (const le::Object& o : built.objects) {
        objs.set(o.idx, o);
    }
    for (const auto& io : w.objects.items()) {
        const int64_t i = std::get<0>(io);
        const std::shared_ptr<DriverObject>& ob = std::get<1>(io);
        if (!objs.contains(i)) {
            differing.append(i);
            continue;
        }
        const py::bytes data = le::object_data(built, objs.at(i));
        py::set<int64_t> patched;
        for (int64_t a : ob->fix) {
            for (int64_t x = a; x < a + 4; ++x) {
                patched.add(x);
            }
        }
        const int64_t n = std::min<int64_t>(data.size(), ob->size);
        bool same = objs.at(i).vsize == ob->size;
        for (int64_t a = 0; same && a < n; ++a) {
            if (!patched.contains(a) && data[a] != ob->data[a]) {
                same = false;
            }
        }
        if (same) {
            continue;
        }
        differing.append(i);
        py::list<assembly::Piece<Place>> pieces;
        int64_t a = 0, start = 0;

        auto data_piece = [&](int64_t lo, int64_t hi) {
            if (hi > lo) {
                std::string m;
                for (int64_t x = lo; x < hi; ++x) {
                    m.push_back(patched.contains(x) ? '\1' : '\0');
                }
                pieces.append(assembly::Piece<Place>(lo, hi - lo, py::bytes(m), std::nullopt, false));
            }
        };
        while (a < ob->size) {
            if (ob->ins.contains(a) && good.contains(Place(i, a))) {
                data_piece(start, a);
                const int64_t length = ob->ins.at(a).data.size();
                std::string mask;
                for (int64_t x = a; x < a + length; ++x) {
                    mask.push_back(patched.contains(x) ? '\1' : '\0');
                }
                const bool jump = ob->ins.at(a).target.has_value();
                if (jump) {                                              // the distance: right once the rest is
                    const int64_t width = length == 2 ? 1 : 4;
                    // mask[length - width:] = b"\1" * width (a negative start counts from the end; the mask may grow)
                    int64_t from = length - width;
                    if (from < 0) {
                        from = std::max<int64_t>(0, from + static_cast<int64_t>(mask.size()));
                    }
                    mask.replace(static_cast<size_t>(from), mask.size() - static_cast<size_t>(from), std::string(static_cast<size_t>(width), '\1'));
                }
                pieces.append(assembly::Piece<Place>(a, length, py::bytes(mask), Place(i, a), jump));
                a += length;
                start = a;
            } else {
                a += ob->ins.contains(a) ? ob->ins.at(a).data.size() : 1;
            }
        }
        data_piece(start, ob->size);
        bad = bad | assembly::culprits<Place>(ob->data, data, pieces);
    }
    return {bad, differing};
}

void _match_module_flags(const py::str& image, const le::LEFile& lf) {
    const int64_t want = le::header_fields(lf).at(py::str("module_flags"));
    py::bytes blob;
    {
        py::file f = py::open_rb(image);
        blob = f.read();
        f.close();
    }
    py::bytearray data(blob);
    const int64_t lfa = py::unpack_from1<int64_t>("<I", blob, 0x3C);
    const py::bytes sig = blob.slice(lfa, lfa + 2);
    if (sig == py::bytes("LE") || sig == py::bytes("LX")) {
        if (py::unpack_from1<int64_t>("<I", blob, lfa + 0x10) != want) {
            py::pack_into("<I", data, lfa + 0x10, want);
            py::file f = py::open(image, py::str("wb"));
            f.write(py::bytes(data.view()));
            f.close();
        }
    }
}

bool _nonresident_names(const le::LEFile& lf) {
    const auto [at, n] = py::unpack_from<int64_t, int64_t>("<II", lf.d(), lf.lfa + 0x88);
    return at != 0 && n != 0 && at + n <= lf.d().size();
}

Reconstruction reconstruct(const py::str& module_path, const py::str& outdir, int64_t member, const std::function<void(const py::str&)>& log) {
    const py::list<std::tuple<py::str, le::LEFile>> mems = le::members(module_path);
    const py::str name = std::get<0>(mems[member]);
    const le::LEFile& lf = std::get<1>(mems[member]);
    Writer w(lf);
    py::str device = w.d ? w.d->name.strip() : name.strip();
    if (device.empty()) {
        device = py::str("DRIVER");
    }
    const py::str stem = py::re::sub(py::str("\\W"), py::str("_"), device.lower());
    py::os::makedirs(outdir, 0777, true);
    const py::str src = py::os::path::join(outdir, stem + py::str(".asm"));
    const py::str out = py::os::path::join(outdir, stem + py::str(".vxd"));
    py::set<Place> good = w.probe(outdir);
    Reconstruction result;
    result.source = src;
    result.image = std::nullopt;
    result.linked = false;
    result.rounds = 0;
    // the driver's .def -- its description and exports, which the linker puts in the non-resident name table -- when the
    // original has one; kept as written once it is there
    std::optional<py::str> definitions;
    const py::str def_path = py::os::path::join(outdir, stem + py::str(".def"));
    if (py::os::path::isfile(def_path)) {
        py::file f = py::open(def_path, py::str("r"), py::str("latin1"));
        definitions = f.read_text();
        f.close();
    } else if (_nonresident_names(lf)) {
        definitions = vxdfile::definitions(lf.d(), lf.lfa);
        py::file f = py::open(def_path, py::str("w"), py::str("latin1"), std::nullopt, py::str("\r\n"));
        f.write(py::percent(py::str("; %s -- the definitions it was linked with, from its non-resident name table\n"), device) +
                py::percent(py::str("VXD %s\n"), device) + *definitions);
        f.close();
    }
    if (definitions && !definitions->empty()) {
        result.definitions = def_path;
    }
    static const py::re::Pattern errors = py::re::compile(py::str("\\((\\d+)\\)\\s*:\\s*Error"));
    for (int64_t rounds = 1; rounds <= _MAX_ROUNDS; ++rounds) {
        auto [text, counts, line_of] = w.source(good);
        {
            py::file f = py::open(src, py::str("w"), py::str("latin1"), std::nullopt, py::str("\r\n"));
            f.write(text);
            f.close();
        }
        result.rounds = rounds;
        result.instructions = counts.instructions;
        result.raw = counts.raw;
        result.data = counts.data;
        auto [obj, messages] = assembly::assemble(src, outdir);
        if (!obj || obj->empty()) {
            py::set<Place> failed;
            for (const auto& m : errors.finditer(messages)) {
                const int64_t ln = py::int_parse(m.group(1));
                if (line_of.contains(ln)) {
                    failed.add(line_of.at(ln));
                }
            }
            // an uncommon opcode the assembler rejects (sidt, lgdt, ...) is rejected everywhere it appears, so drop every
            // instance of it at once rather than one per round
            py::set<Place> drop = failed;
            py::set<py::str> ops;
            for (const Place& k : failed) {
                ops.add(w.objects.at(std::get<0>(k))->ins.at(std::get<1>(k)).mnem);
            }
            ops = ops - _COMMON;
            if (!ops.empty()) {
                for (const auto& io : w.objects.items()) {
                    const int64_t i = std::get<0>(io);
                    for (const auto& kv : std::get<1>(io)->ins.items()) {
                        if (good.contains(Place(i, std::get<0>(kv))) && ops.contains(std::get<1>(kv).mnem)) {
                            drop.add(Place(i, std::get<0>(kv)));
                        }
                    }
                }
            }
            if (drop.empty()) {
                const py::list<py::str> ls = py::splitlines(messages.strip());
                const py::str last = ls.empty() ? py::str("failed") : ls[-1];
                result.notes = py::list<py::str>{py::str("assembler: ") + last.slice(0, 200)};
                break;
            }
            good = good - drop;
            continue;
        }
        const auto exp = le::exports(lf);
        const bool named = !exp.empty() && !std::get<0>(exp[0]).empty();
        auto [image, mlog] = builder::link(*obj, device, out, le::header_fields(lf).at(py::str("module_flags")) == le::MODULE_DYNAMIC, w.classes(), named,
                                           definitions);
        if (!image || image->empty()) {
            const py::list<py::str> ls = py::splitlines(mlog.strip());
            const py::str last = ls.empty() ? py::str("failed") : ls[-1];
            result.notes = py::list<py::str>{py::str("linker: ") + last.slice(0, 200)};
            break;
        }
        _match_module_flags(*image, lf);                      // a flag the linker will not emit (Win3.x LE)
        const py::list<std::tuple<py::str, le::LEFile>> bm = le::members(*image);
        const le::LEFile& built = std::get<1>(bm[0]);
        result.image = *image;
        result.linked = true;
        auto [bad, differing] = _differences(w, built, good);
        if (log && !differing.empty()) {
            log(py::percent(py::str("  round %d: objects %s differ; %d instructions go back to bytes"), rounds, differing, bad.size()));
        }
        if (differing.empty() || bad.empty()) {
            auto [checks, notes, same] = builder::compare(built, lf);
            result.checks = checks;
            result.notes = notes;
            result.same = same;
            result.differing_objects = differing;
            break;
        }
        good = good - bad;
    }
    // the driver as one L2 tree, as the file was last written: each object's head and each function a MachineFragment
    if (w.l2) {
        std::string text;
        json_put(text, l2::Val(*w.l2));
        const py::str p = py::os::path::join(outdir, py::str("module.l2.json"));
        py::file fh = py::open(p, py::str("w"), py::str("utf-8"), std::nullopt, py::str("\n"));
        fh.write(py::str(text));
        fh.close();
        result.l2 = p;
    }
    return result;
}

bool _report(const py::str& name, const Reconstruction& r) {
    const int64_t n = std::max<int64_t>(1, r.instructions);
    print_(py::percent(py::str("%s: %d instructions, %d (%d%%) written as bytes; %d bytes of data; %d round%s"), name, r.instructions, r.raw,
                       100 * r.raw / n, r.data, r.rounds, py::str(r.rounds == 1 ? "" : "s")));
    if (r.checks && !r.checks->empty()) {
        py::list<py::str> parts;
        for (const auto& kv : r.checks->items()) {
            parts.append(py::percent(py::str("%s %s"), std::get<0>(kv), py::str(std::get<1>(kv) ? "same" : "DIFFERENT")));
        }
        print_(py::str("  built: ") + py::join(py::str(", "), parts));
    }
    for (const py::str& note : r.notes) {
        print_(py::str("  ") + note);
    }
    print_(py::str("  source: ") + r.source);
    bool all = r.checks.has_value() && !r.checks->empty();
    if (all) {
        for (bool v : r.checks->values()) {
            all = all && v;
        }
    }
    return all;
}

// main.__doc__ as the interpreter keeps it (Python 3.13+ strips the docstring's indentation and its trailing blank line)
const py::str MAIN_DOC = py::str(
    "python -m codebreaker driver-source <driver> [--out DIR]\n"
    "\n"
    "Write a driver's assembly source from its image, build it and check the build against the\n"
    "original. A container of drivers (VMM32.VXD, WIN386.EXE) has every member written.");

int64_t main(const py::list<py::str>& argv) {
    py::list<py::str> args;
    for (const py::str& a : argv) {
        if (!a.startswith(py::str("--"))) {
            args.append(a);
        }
    }
    if (args.empty()) {
        print_(MAIN_DOC);
        return 2;
    }
    py::str path;
    if (py::os::path::isfile(args[0])) {
        path = args[0];
    } else {
        const auto found = codebreaker::base::corpus::retail(args[0]);
        if (!found || found->empty()) {
            throw py::SystemExit(py::percent(py::str("no driver %s"), py::repr(args[0])));
        }
        path = *found;
    }
    py::str base;
    if (argv.contains(py::str("--out"))) {
        base = argv[argv.index(py::str("--out")) + 1];
    } else {
        base = state_(py::list<py::str>{py::str("driver_source"), std::get<0>(py::os::path::splitext(py::os::path::basename(path))).lower()});
    }
    const py::list<std::tuple<py::str, le::LEFile>> members = le::members(path);
    bool ok = true;
    for (int64_t i = 0; i < members.size(); ++i) {
        const py::str member = std::get<0>(members[i]);
        const py::str outdir = members.size() == 1 ? base
                                                   : py::os::path::join(base, py::re::sub(py::str("\\W"), py::str("_"), member.lower()));
        const Reconstruction r = reconstruct(path, outdir, i, [](const py::str& s) { print_(s); });
        ok = _report(members.size() > 1 ? member : py::os::path::basename(path), r) && ok;
    }
    return ok ? 0 : 1;
}

}  // namespace codebreaker::source::driver_writer
