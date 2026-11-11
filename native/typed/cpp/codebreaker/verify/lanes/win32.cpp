// codebreaker/verify/lanes/win32.cpp -- typed C++ port of codebreaker.verify.lanes.win32.
//
// What is particular to reconstructing a 32-bit Windows module: decompiled by decompile/decompiler, built by
// build/functions, each function run beside the original in the 32-bit emulator (emulate/emulator) and compared by
// verify/compare (calls and their arguments, result, memory changed, stack, saved registers, coverage).
//
// What runs a function, how it is called, its compiler's options and what builds the rebuild are the platform's
// (platforms/win32.py): its emulator (emulate::emulator::recorded), convention (4 arguments), compiler (wcc386 with
// c_text::CFLAGS) and packager (build::functions::build32).
//
// Notes on the port (where it is not the Python's, and why):
//   * `shadow.vars` of a run is (kind, where, bits, value) in concolic::Vars with `where` an int: for a register input
//     (kind "reg") the Python's where is the register's NAME ("ecx" / "edx"); here ECX is 1 and EDX is 2 (what
//     _derive reads back; concolic never looks at `where`).
//   * The Python's caches (_PROFILES, _IMAGE_SPAN, _INSN_COUNT, _THUNKS, _HARVEST, _FILL, _DATA) are process-wide dicts
//     kept under a mutex (the judge runs from several threads).

#include "codebreaker/verify/lanes/win32.hpp"
#include "codebreaker/jobs/workers.hpp"

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <map>
#include <mutex>
#include <set>
#include <string>
#include <system_error>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/functions.hpp"
#include "codebreaker/build/watcom.hpp"
#include "codebreaker/decompile/c_text.hpp"
#include "codebreaker/decompile/decompiler.hpp"
#include "codebreaker/decompile/describe.hpp"
#include "codebreaker/emulate/capstone_x86.hpp"
#include "codebreaker/emulate/pcode.hpp"
#include "codebreaker/emulate/shadow.hpp"
#include "codebreaker/formats/pe.hpp"
#include "codebreaker/symbols/api_arguments.hpp"
#include "codebreaker/symbols/runtime_library.hpp"
#include "codebreaker/symbols/x86.hpp"
#include "codebreaker/verify/verdicts.hpp"
#include "py_json.hpp"

namespace codebreaker::verify::lanes::win32 {

namespace V = codebreaker::verify::verdicts;
namespace em = codebreaker::emulate::emulator;

const py::str KIND("pe");

int64_t ARITY() { return 4; }   // P.convention.arguments: the arguments every function is given (its count is not known)

const py::list<py::str>& GOOD() { return V::GOOD(); }

// lane_common.PLAIN, read when this is loaded (the judge reads lane_common::PLAIN itself where it matters)
static bool plain_() {
    const std::optional<py::str> e = py::os::getenv(py::str("CB_PLAIN"));
    return e && !e->empty();
}
const bool _PLAIN = plain_();

const std::array<int64_t, 4> _ALT_SENTINELS = {em::SAVED_SENTINELS[0] ^ 0xFFFFFFFFLL, em::SAVED_SENTINELS[1] ^ 0xFFFFFFFFLL,
                                              em::SAVED_SENTINELS[2] ^ 0xFFFFFFFFLL, em::SAVED_SENTINELS[3] ^ 0xFFFFFFFFLL};

namespace {

// a + b as Python lists
template <class T>
py::list<T> cat(const py::list<T>& a, const py::list<T>& b) {
    py::list<T> out = a.copy();
    for (const T& x : b) {
        out.append(x);
    }
    return out;
}

py::bytes u32le(int64_t v) { return py::pack("<I", v & 0xFFFFFFFFLL); }

py::bytes bytes_of(const std::string& s) { return py::bytes(s); }

// mutex-kept process-wide cache of one kind
template <class K, class T>
struct Cache {
    std::mutex mu;
    std::map<K, T> items;
    bool get(const K& k, T& out) {
        std::lock_guard<std::mutex> g(mu);
        auto it = items.find(k);
        if (it == items.end()) {
            return false;
        }
        out = it->second;
        return true;
    }
    void put(const K& k, const T& v) {
        std::lock_guard<std::mutex> g(mu);
        items[k] = v;
    }
};

// --- capstone's x86-32 detail decoder, as platforms.decoder(32, detail=True) hands instructions out --------------------
struct Op {
    int type = 0;                 // X86_OP_IMM 2, X86_OP_MEM 3 ...
    int64_t imm = 0;
    int base = 0, index = 0;      // X86_REG_INVALID is 0
    int64_t disp = 0;
};
struct DIns {
    int64_t address = 0;
    int64_t size = 0;
    std::string mn, op_str;
    std::vector<Op> ops;
};

// md.disasm(code, address[, count]): the instructions from the start of `code` until one does not decode
std::vector<DIns> decode32(const std::string& code, int64_t address, size_t count = 0, bool detail = true) {
    const emulate::capstone_x86::Api& api = emulate::capstone_x86::api();
    csh h = emulate::capstone_x86::handle(32, detail);
    cs_insn* insn = nullptr;
    size_t n = api.disasm(h, reinterpret_cast<const uint8_t*>(code.data()), code.size(), static_cast<uint64_t>(address), count, &insn);
    std::vector<DIns> out;
    out.reserve(n);
    for (size_t i = 0; i < n; ++i) {
        DIns d;
        d.address = static_cast<int64_t>(insn[i].address);
        d.size = insn[i].size;
        d.mn = insn[i].mnemonic;
        d.op_str = insn[i].op_str;
        if (detail && insn[i].detail != nullptr) {
            const cs_x86& x = insn[i].detail->x86;
            for (int k = 0; k < x.op_count; ++k) {
                Op o;
                o.type = static_cast<int>(x.operands[k].type);
                if (o.type == X86_OP_IMM) {
                    o.imm = x.operands[k].imm;
                } else if (o.type == X86_OP_MEM) {
                    o.base = static_cast<int>(x.operands[k].mem.base);
                    o.index = static_cast<int>(x.operands[k].mem.index);
                    o.disp = x.operands[k].mem.disp;
                }
                d.ops.push_back(o);
            }
        }
        out.push_back(std::move(d));
    }
    if (insn != nullptr) {
        api.free_(insn, n);
    }
    return out;
}

constexpr int64_t M32 = 0xFFFFFFFFLL;

// the Python's `dict(v, ...)`: a copy
Vector copy_of(const Vector& v) { return v; }

}  // namespace

// ---- inputs ------------------------------------------------------------------------------------------------------
guided::Input to_guided(const Vector& v) {
    guided::Input g;
    g.args = v.args.copy();
    if (v.mem_init) {
        g.mem_init = v.mem_init->copy();
    }
    g.stub_ret = v.stub_ret;
    if (v.ecx) {
        g.extra.set(py::str("ecx"), guided::Extra(*v.ecx));
    }
    if (v.edx) {
        g.extra.set(py::str("edx"), guided::Extra(*v.edx));
    }
    if (v.given) {
        g.extra.set(py::str("given"), guided::Extra(v.given->copy()));
    }
    if (v.room) {
        g.extra.set(py::str("room"), guided::Extra(*v.room));
    }
    if (v.sentinels) {
        g.extra.set(py::str("sentinels"), guided::Extra(v.sentinels->copy()));
    }
    return g;
}

Vector from_guided(const guided::Input& g) {
    Vector v;
    v.args = g.args.copy();
    if (g.mem_init) {
        v.mem_init = g.mem_init->copy();
    }
    v.stub_ret = g.stub_ret;
    if (g.extra.contains(py::str("ecx"))) {
        v.ecx = std::get<int64_t>(g.extra.at(py::str("ecx")));
    }
    if (g.extra.contains(py::str("edx"))) {
        v.edx = std::get<int64_t>(g.extra.at(py::str("edx")));
    }
    if (g.extra.contains(py::str("given"))) {
        v.given = std::get<py::list<int64_t>>(g.extra.at(py::str("given"))).copy();
    }
    if (g.extra.contains(py::str("room"))) {
        v.room = std::get<int64_t>(g.extra.at(py::str("room")));
    }
    if (g.extra.contains(py::str("sentinels"))) {
        v.sentinels = std::get<py::list<int64_t>>(g.extra.at(py::str("sentinels"))).copy();
    }
    return v;
}

// The same input for a function that takes its first argument in ECX (a member function's object): the first value
// there, the rest on the stack.
Vector in_ecx(const Vector& v) {
    if (v.ecx) {
        return v;
    }
    Vector w = copy_of(v);
    w.ecx = v.args[0];
    py::list<int64_t> rest = v.args.slice_from(1);
    rest.append(0);
    w.args = rest;
    w.given = v.args.copy();
    return w;
}

// ... and for one that takes its first two in ECX and EDX.
Vector in_ecx_edx(const Vector& v) {
    if (v.ecx) {
        return v;
    }
    Vector w = copy_of(v);
    w.ecx = v.args[0];
    w.edx = v.args[1];
    py::list<int64_t> rest = v.args.slice_from(2);
    rest.append(0);
    rest.append(0);
    w.args = rest;
    w.given = v.args.copy();
    return w;
}

// A pretend object whose every field is a pointer to a landing pad of its own: a call through a field returns, and
// WHICH field was called through is seen (emulator `pads`).
py::list<std::tuple<int64_t, py::bytes>> _prov_meminit() {
    std::string obj;
    for (int64_t i = 0; i < 0x400; ++i) {
        obj += u32le(_SAFE + 4 * i).raw();
    }
    py::list<std::tuple<int64_t, py::bytes>> out;
    out.append({_OBJ, bytes_of(obj)});
    out.append({_SAFE, bytes_of(std::string(0x1000, '\xc3'))});
    return out;
}

// Input vectors to exercise a function past the immediate null-fault (no per-function signatures needed). Beyond
// zeros/handles, the PROVISIONED vector points every arg at a valid object region whose contents resolve
// dereference/call chains to a safe RET -- so a pointer-taking function runs its real path to completion instead of
// faulting (env-provisioned).
py::list<Vector> _vectors(int64_t arity, const py::str& lane) {
    py::list<Vector> vs;
    if (arity <= 0) {
        Vector v;
        vs.append(v);
        return vs;
    }
    const int64_t n = arity;
    // Every argument gets a DIFFERENT value wherever the environment allows it: with one value in every slot, a body
    // that uses its parameters in the wrong order behaves identically.
    {
        Vector v;
        for (int64_t i = 0; i < n; ++i) {
            v.args.append(0);
        }
        vs.append(v);
    }
    if (lane == py::str("pe") || lane == py::str("dos")) {
        {
            Vector v;
            v.args = lane_common::counting(n, 0x200000, 0x100);   // bare scratch pointers
            vs.append(v);
        }
        {
            Vector v;                                               // provisioned objects
            for (int64_t i = 0; i < n; ++i) {
                v.args.append(_OBJ + 0x40 * (i % 32));
            }
            v.mem_init = _prov_meminit();
            vs.append(v);
        }
    } else {                                                        // NE controlled env: valid handles
        {
            Vector v;                                               // LZ handle base
            for (int64_t i = 0; i < n; ++i) {
                v.args.append(0x0400);
            }
            vs.append(v);
        }
        {
            Vector v;                                               // open-file handle
            for (int64_t i = 0; i < n; ++i) {
                v.args.append(0x0080);
            }
            vs.append(v);
        }
    }
    {
        Vector v;
        v.args = lane_common::counting(n);
        vs.append(v);
    }
    return vs;
}

// More inputs, run only for a body that already matched on every basic vector. Four inputs are few; these add boundary
// integers, a reversed ordering and mixes of pointers and integers, so a body is not called verified on the strength of
// four lucky inputs.
py::list<Vector> _confirm_vectors(int64_t arity, const py::str& lane) {
    if (arity <= 0 || !(lane == py::str("pe") || lane == py::str("dos"))) {
        return py::list<Vector>();
    }
    const int64_t n = arity;
    const py::list<std::tuple<int64_t, py::bytes>> mi = _prov_meminit();
    py::list<int64_t> obj;
    for (int64_t i = 0; i < n; ++i) {
        obj.append(_OBJ + 0x40 * (i % 32));
    }
    const int64_t edge[8] = {0x7FFFFFFF, 0x80000000LL, 0x10000, 0xFFFF, 0xFF, 0x100, 0x8000, 0x7FFF};
    const py::list<int64_t> ints = lane_common::counting(n);
    // Objects whose fields are not pointers: all-ones (every flag bit set, counts large) and small counts. A field
    // tested with `& 0x40` or compared with an index is false on the pointer-filled object above, so everything behind
    // such a test never ran.
    py::list<std::tuple<int64_t, py::bytes>> ones, small;
    ones.append({_OBJ, bytes_of(std::string(0x1000, '\xff'))});
    ones.append({_SAFE, bytes_of(std::string(16, '\xc3'))});
    {
        std::string three;
        for (int i = 0; i < 0x400; ++i) {
            three += u32le(3).raw();
        }
        small.append({_OBJ, bytes_of(three)});
        small.append({_SAFE, bytes_of(std::string(16, '\xc3'))});
    }
    // Objects whose words are zero and non-zero by turns, both ways round. On an object that is the same all through,
    // a field read one byte off (`p + 0x3d` for `p + 0x3c`) reads what the right one holds, and `if (p->x == 0)` goes
    // the same way whichever field it tests.
    py::list<py::list<std::tuple<int64_t, py::bytes>>> striped;
    for (int64_t phase = 0; phase < 2; ++phase) {
        std::string body;
        for (int64_t i = 0; i < 0x400; ++i) {
            body += u32le(i % 2 == phase ? 0 : _SAFE + 4 * i).raw();
        }
        py::list<std::tuple<int64_t, py::bytes>> one;
        one.append({_OBJ, bytes_of(body)});
        one.append({_SAFE, bytes_of(std::string(0x1000, '\xc3'))});
        striped.append(one);
    }
    py::list<Vector> out;
    auto add = [&](const py::list<int64_t>& args, const std::optional<py::list<std::tuple<int64_t, py::bytes>>>& mem,
                   std::optional<int64_t> stub_ret) {
        Vector v;
        v.args = args;
        v.mem_init = mem;
        v.stub_ret = stub_ret;
        out.append(v);
    };
    // Imported calls return 0 by default, so the code after `if (!CreateThing()) return` never ran. Run again with
    // imports succeeding: returning 1, and returning a valid object.
    add(obj, mi, 1);
    add(obj, mi, _OBJ + 0x800);
    add(ints, std::nullopt, 1);
    add(obj, ones, 1);
    add(obj, small, 1);
    add(obj, small, std::nullopt);
    add(obj, striped[0], 1);
    add(obj, striped[1], 1);
    add(lane_common::descending(n, 0xFFFFFFFFLL), std::nullopt, std::nullopt);   // small negatives
    {
        py::list<int64_t> a;                                                       // boundary integers
        for (int64_t i = 0; i < n; ++i) {
            a.append(edge[i % 8]);
        }
        add(a, std::nullopt, std::nullopt);
    }
    add(lane_common::descending(n, n), std::nullopt, std::nullopt);                // distinct, reversed
    {
        py::list<int64_t> a;                                                       // pointer, then integers
        a.append(obj[0]);
        for (int64_t i = 1; i < n; ++i) {
            a.append(i + 2);
        }
        add(a, mi, std::nullopt);
    }
    {
        py::list<int64_t> a;                                                       // integer, then pointers
        a.append(5);
        for (int64_t i = 1; i < n; ++i) {
            a.append(obj[i]);
        }
        add(a, mi, std::nullopt);
    }
    add(lane_common::alternating(n, lane_common::Pattern(std::function<int64_t(int64_t)>([](int64_t i) { return 0x100 + i; })),
                                 lane_common::Pattern(std::function<int64_t(int64_t)>([&](int64_t i) { return obj[i]; }))),
        mi, std::nullopt);
    return out;
}

// ---- lane ----------------------------------------------------------------------------------------------------------
namespace {

// repr() of a vector's items, sorted by key, as `repr([sorted(v.items()) for v in ...])` writes one
py::str items_repr(const Vector& v) {
    py::list<py::str> items;   // (key, value) in the order of sorted keys: args ecx edx given mem_init room sentinels stub_ret
    auto ints = [](const py::list<int64_t>& xs) { return py::repr(xs); };
    items.append(py::str("('args', ") + ints(v.args) + py::str(")"));
    if (v.ecx) {
        items.append(py::str("('ecx', ") + py::to_str(*v.ecx) + py::str(")"));
    }
    if (v.edx) {
        items.append(py::str("('edx', ") + py::to_str(*v.edx) + py::str(")"));
    }
    if (v.given) {
        items.append(py::str("('given', ") + ints(*v.given) + py::str(")"));
    }
    if (v.mem_init) {
        items.append(py::str("('mem_init', ") + py::repr(*v.mem_init) + py::str(")"));
    }
    if (v.room) {
        items.append(py::str("('room', ") + py::to_str(*v.room) + py::str(")"));
    }
    if (v.sentinels) {
        items.append(py::str("('sentinels', ") + ints(*v.sentinels) + py::str(")"));
    }
    if (v.stub_ret) {
        items.append(py::str("('stub_ret', ") + py::to_str(*v.stub_ret) + py::str(")"));
    }
    return py::str("[") + py::join(py::str(", "), items) + py::str("]");
}

Cache<std::string, std::tuple<py::str, py::str>>& profiles() {
    static Cache<std::string, std::tuple<py::str, py::str>> c;
    return c;
}

py::str inputs_json(const py::list<Vector>& vs) {
    std::string o = "[";
    bool first = true;
    for (const Vector& v : vs) {
        if (!first) {
            o += ", ";
        }
        first = false;
        py::json::put(o, v.args);
    }
    return py::str(o + "]");
}

}  // namespace

PathsProfile _paths_profile() {
    PathsProfile p;
    p.ready = concolic_ready();
    p.path_budget = lane_common::PATH_BUDGET;
    p.max_paths = lane_common::MAX_PATHS;
    p.guided_rest = lane_common::GUIDED_REST;
    if (p.ready) {
        const std::optional<py::str> mode = _mode();
        p.stamp = emulate::pcode::stamp(mode ? *mode : py::str("x86-32-flat"));
    }
    return p;
}

// (digest, settings) -- everything OTHER than a body's own source that a verdict depends on: the pipeline and oracle
// revisions, the inputs each body is run on, the compiler and its flags. The digest is part of every body's signature,
// so changing any of these re-verifies the corpus without anyone having to remember to bump a number; the settings are
// stored beside it.
std::tuple<py::str, py::str> settings_profile(const py::str& lane) {
    std::tuple<py::str, py::str> had;
    if (profiles().get(lane.utf8(), had)) {
        return had;
    }
    const py::str cc = build::watcom::ow_tool(py::str("wcc386"));   // platforms.of("pe" | "ne").compiler.tool()
    py::str compiler("");
    {
        const py::str tool = lane == py::str("pe") ? cc : build::watcom::ow_tool(py::str("wcc"));
        std::error_code ec;
        const auto size = std::filesystem::file_size(std::filesystem::u8path(tool.utf8()), ec);
        if (!ec) {
            double mtime = 0;
            bool ok = true;
            try {
                mtime = py::os::path::getmtime(tool);
            } catch (const py::OSError&) {
                ok = false;
            }
            if (ok) {
                compiler = py::percent("%s:%d:%d", py::os::path::basename(tool), static_cast<int64_t>(size), static_cast<int64_t>(mtime));
            }
        }
    }
    const py::list<Vector> basic = _vectors(ARITY(), lane), confirm = _confirm_vectors(ARITY(), lane);
    // (and what they put in memory: two inputs with the same arguments can differ only there)
    py::list<py::str> reprs;
    for (const Vector& v : basic) {
        reprs.append(items_repr(v));
    }
    for (const Vector& v : confirm) {
        reprs.append(items_repr(v));
    }
    const py::str memory = py::hashlib::sha1(py::str("[" + py::join(py::str(", "), reprs).utf8() + "]").encode()).hexdigest().slice(0, 16);
    const bool pe = lane == py::str("pe");
    std::string o = "{";
    o += "\"cflags\": ";
    py::json::put(o, decompile::c_text::CFLAGS);
    o += ", \"compiler\": ";
    py::json::put(o, compiler);
    o += ", \"confirm_inputs\": " + inputs_json(confirm).utf8();
    o += ", \"directed\": ";
    o += pe ? "[" + std::to_string(MAX_DIRECTED) + ", " + std::to_string(_HARVEST_DEPTH) + "]" : "[]";
    o += ", \"guided\": ";
    o += pe ? "[" + std::to_string(lane_common::GUIDED_BUDGET) + ", " + std::to_string(lane_common::MAX_GUIDED) + "]" : "[]";
    o += ", \"input_memory\": ";
    py::json::put(o, memory);
    o += ", \"inputs\": " + inputs_json(basic).utf8();
    o += ", \"lane\": ";
    py::json::put(o, lane);
    o += ", \"max_calls\": " + std::to_string(_MAX_CALLS);
    o += ", \"max_support\": " + std::to_string(MAX_SUPPORT);
    o += ", \"oracle\": " + std::to_string(compare::REV);
    // (and whether the paths are explored, and with what: the lifter and its specification)
    o += ", \"paths\": ";
    if (pe) {
        const PathsProfile p = _paths_profile();
        o += std::string("[") + (p.ready ? "true" : "false") + ", " + std::to_string(p.path_budget) + ", " + std::to_string(p.max_paths) +
             ", " + std::to_string(p.guided_rest) + ", ";
        if (p.ready && p.stamp) {
            o += "[";
            py::json::put(o, std::get<0>(*p.stamp));
            o += ", ";
            py::json::put(o, std::get<1>(*p.stamp));
            o += "]";
        } else {
            o += "\"\"";
        }
        o += "]";
    } else {
        o += "[]";
    }
    o += ", \"pipeline\": " + std::to_string(PIPE_REV);
    o += ", \"runtime\": ";
    py::json::put(o, build::watcom::_RTL);
    o += "}";
    const py::str text(o);
    std::tuple<py::str, py::str> out{py::hashlib::sha1(text.encode()).hexdigest().slice(0, 12), text};
    profiles().put(lane.utf8(), out);
    return out;
}

std::tuple<py::str, py::str> profile(const py::str& module_path) {
    (void)module_path;
    return settings_profile(py::str("pe"));
}

// ---- PELane ---------------------------------------------------------------------------------------------------------
py::list<int64_t> PELane::functions(const py::str& path) const {
    const std::shared_ptr<symbols::x86::PeCarve> carved = symbols::x86::carve(path);
    std::vector<int64_t> keys = carved->funcs.keys();
    std::sort(keys.begin(), keys.end());
    py::list<int64_t> out;
    for (int64_t k : keys) {
        out.append(k);
    }
    return out;
}

py::str PELane::key(int64_t va) const { return py::percent("%x", va); }

py::dict<py::str, PELane> LANES = [] {
    py::dict<py::str, PELane> d;
    d.set(py::str("pe"), PELane());
    return d;
}();

py::list<int64_t> functions(const py::str& lane, const py::str& path) {
    if (!LANES.contains(lane)) {
        throw py::KeyError(py::repr(lane).utf8());
    }
    return LANES.at(lane).functions(path);
}

// ---- the pipeline ---------------------------------------------------------------------------------------------------
// The decompiled module: decompile(module_path, addresses, work), with key/va/all_keys/sig added.
reconstruct::Prepared prepare(const py::str& module_path, const py::str& work) {
    const PELane& lane = LANES.at(py::str("pe"));
    const py::list<int64_t> addrs = lane.functions(module_path);
    auto m = std::make_shared<decompile::c_text::Module32>(decompile::decompiler::decompile(module_path, addrs, work));
    reconstruct::Prepared out;
    out.header = m->header();
    out.texts = m->texts();
    out.order = m->order();
    for (const auto& kv : m->by_va().items()) {
        out.key.set(std::get<1>(kv), lane.key(std::get<0>(kv)));
    }
    for (int64_t va : addrs) {
        out.all_keys.append(lane.key(va));
    }
    // The module is one translation unit: any change to it may change any function's build.
    py::str body("");
    for (const py::str& n : out.order) {
        body = body + out.texts.at(n);
    }
    const py::str nul(std::u32string(1, U'\0'));
    out.sig = py::hashlib::sha1((py::percent("%s", py::to_str(decompile::decompiler::REV)) + nul + out.header + nul + body).encode("utf-8", "replace"))
                  .hexdigest();
    out.lane = m;
    return out;
}

// -> (context for judge, {name: item}, {name: why it is not in the build})
Built32 build(const reconstruct::Prepared& prepared, const py::str& module_path, const py::str& work) {
    const std::shared_ptr<decompile::c_text::Module32> m = std::static_pointer_cast<decompile::c_text::Module32>(prepared.lane);
    const py::dict<int64_t, py::str> by_va = m->by_va();
    py::dict<py::str, int64_t> va;
    for (const auto& kv : by_va.items()) {
        va.set(std::get<1>(kv), std::get<0>(kv));
    }
    // Only functions with a known address are built. The rest are library routines recognised by signature (__allshl,
    // _memcpy): they belong to the C runtime, which is linked.
    py::list<py::str> placed;
    for (const py::str& n : prepared.order) {
        if (va.contains(n)) {
            placed.append(n);
        }
    }
    // A flat thunk's stubs are the thunk compiler's, not C: not rebuilt, so a call to one goes to the original's
    // (build32: a jump to it), which the judge enters as an import (_thunk_stubs).
    const auto stubs = std::get<0>(_thunk_stubs(module_path));
    {
        py::list<py::str> kept;
        for (const py::str& n : placed) {
            if (!stubs.contains(va.at(n))) {
                kept.append(n);
            }
        }
        placed = kept;
    }
    const py::dict<int64_t, py::str> every = decompile::describe::function_names(module_path, LANES.at(py::str("pe")).functions(module_path));
    py::dict<py::str, int64_t> originals;
    for (const auto& kv : every.items()) {
        originals.set(std::get<1>(kv), std::get<0>(kv));
    }
    const build::functions::Built32 made = build::functions::build32(module_path, prepared.header, prepared.texts, placed, m->imports,
                                                                    work, decompile::c_text::CFLAGS, std::nullopt, originals);
    Built32 out;
    out.module_path = module_path;
    out.dropped = made.dropped.copy();
    for (const py::str& fn : placed) {
        if (out.dropped.contains(fn)) {
            continue;
        }
        if (!made.where.contains(fn)) {
            out.dropped.set(fn, py::str("not in the build"));
        } else {
            Item it;
            it.key = prepared.key.at(fn);
            it.va = va.at(fn);
            it.dll = made.dll ? *made.dll : py::str("");
            it.entry = made.where.at(fn);
            it.regs = m->in_edx.contains(fn) ? 2 : m->in_ecx.contains(fn) ? 1 : 0;
            out.items.set(fn, it);
        }
    }
    return out;
}

// ---- the fingerprint -----------------------------------------------------------------------------------------------
// compiler/CRT-runtime imports the toolchain emits (not the body's own logic) -- ignored so MSC-vs-OpenWatcom startup
// noise is not scored as a behavioral divergence.
static const std::set<std::string>& toolchain_() {
    static const std::set<std::string> s = {"KERNEL.GETVERSION",   "KERNEL.GETWINFLAGS",     "KERNEL.LOCALINIT",
                                            "KERNEL.GETVERSION16", "KERNEL32.GETVERSION",    "KERNEL32.GETVERSIONEXA",
                                            "KERNEL32.GETCURRENTTHREADID"};
    return s;
}

// Canonicalise an imported-call name so the two builds' import-table SPELLINGS of the same import compare equal:
// uppercase, drop a .DLL/.EXE/.SYS (NDIS.SYS is NDIS) on the module half, unify the separator. ('KERNEL32.dll!#21' and
// 'KERNEL32!#21' are the same ordinal import -> same behavior).
py::str _norm_dep(const py::str& d) {
    const py::str s = d.upper().replace(py::str("!"), py::str(".")).replace(py::str(":"), py::str("."));
    py::list<py::str> kept;
    for (const py::str& p : py::split(s, py::str("."))) {
        const std::string t = p.utf8();
        if (t == "DLL" || t == "EXE" || t == "DRV" || t == "SYS" || t == "VXD" || t == "OCX" || t == "CPL") {
            continue;
        }
        kept.append(p);
    }
    return py::join(py::str("."), kept);
}

// Toolchain/thunk imports that differ by CODEGEN (OpenWatcom vs MS), not by the body's logic -- excluded from the
// behavioral fingerprint so they don't read as a divergence.
bool _is_noise(const std::optional<py::str>& api) {
    const py::str a = (api ? *api : py::str("None")).upper();
    if (toolchain_().count(a.utf8())) {
        return true;
    }
    return a.contains(py::str("SMAPLS")) || a.contains(py::str("SUNMAPLS")) || a.contains(py::str("FT_")) || a.contains(py::str("QT_")) ||
           a.contains(py::str("THUNK")) || a.endswith(py::str(".@0")) || a.contains(py::str("GETVERSION")) ||
           a.contains(py::str("GETWINFLAGS"));
}

// Argument count of an imported function, for the oracle's import stub: read from the retail module that exports it
// (apiarity), else the small hand table. None = unknown (the stub then removes nothing and the call's arguments are not
// compared).
std::optional<int64_t> _argc(const py::str& dll, const py::str& name) {
    const std::optional<int64_t> n = symbols::api_arguments::argc(dll, name);
    return n ? n : em::_builtin_argc(name);
}

// The imported-call part of a fingerprint: every call in order, its name as both builds spell it, toolchain noise left
// out, and each argument as lane_common.argument writes it -- which input it is, a small constant, or a stack address
// or an address inside the rebuilt image ("S": those differ by build; a rebuilt body's locals live in its stand-in
// frame, inside its own image); otherwise the value itself. Reconstructed code refers to the original's data by its
// literal address, so a pointer into the original image is the same number on both sides and is compared as one:
// passing the wrong string or the wrong table is a difference.
py::list<py::str> _call_seq(const py::list<lane_common::Called>& calls, const py::list<int64_t>& ins,
                            const std::optional<std::tuple<int64_t, int64_t>>& own) {
    py::list<lane_common::Span> frame;
    frame.append(std::make_optional(std::make_tuple(_STACK_LO, _STACK_HI)));
    frame.append(own);
    return lane_common::calls(
        calls, [&](int64_t a) { return lane_common::argument(a, ins, std::nullopt, true, frame); },
        [](const std::optional<py::str>& n) { return _norm_dep(n ? *n : py::str("None")); },
        [](const std::optional<py::str>& n) { return _is_noise(n); }, _MAX_CALLS);
}

// --- behavioral-fingerprint cache -------------------------------------------------------------
// A PE fingerprint is a pure function of (module bytes, func addr, input vector): the trace is deterministic given
// those. The RETAIL side (the corpus binary) is reconstruction-independent, so its fingerprint is identical across every
// body and every sweep re-run -- yet it was re-traced each time, making it ~half of the oracle cost (4 of 8 traces/body).
// Cache fingerprints keyed by the module's content hash so a re-run (the common case after any synth/lifter change) skips
// the retail traces entirely. Keying on content hash (not path) makes it correct for both sides and for shared bodies: a
// rebuilt DLL gets a fresh hash each build, so it never stales. (lane_common.remembered; not with CB_NO_FP_CACHE.)
py::str _fp_key(const py::str& sha, int64_t func, const Vector& spec) {
    // repr((func, args, mi, stub_ret) + ((sentinels,) if sentinels) + ("ecx", n) + ("edx", n) + ("room", n))
    py::list<py::str> parts;
    parts.append(py::to_str(func));
    py::list<py::str> args;
    for (int64_t a : spec.args) {
        args.append(py::to_str(a));
    }
    parts.append(py::str("(") + py::join(py::str(", "), args) + (args.size() == 1 ? py::str(",") : py::str("")) + py::str(")"));
    py::list<py::str> mi;
    if (spec.mem_init) {
        for (const auto& ab : *spec.mem_init) {
            mi.append(py::str("(") + py::to_str(std::get<0>(ab)) + py::str(", ") + py::repr(std::get<1>(ab)) + py::str(")"));
        }
    }
    parts.append(py::str("(") + py::join(py::str(", "), mi) + (mi.size() == 1 ? py::str(",") : py::str("")) + py::str(")"));
    parts.append(py::to_str(spec.stub_ret ? *spec.stub_ret : 0));
    if (spec.sentinels && !spec.sentinels->empty()) {
        py::list<py::str> s;
        for (int64_t a : *spec.sentinels) {
            s.append(py::to_str(a));
        }
        parts.append(py::str("(") + py::join(py::str(", "), s) + (s.size() == 1 ? py::str(",") : py::str("")) + py::str(")"));
    }
    if (spec.ecx) {
        parts.append(py::str("'ecx'"));
        parts.append(py::to_str(*spec.ecx));
    }
    if (spec.edx) {
        parts.append(py::str("'edx'"));
        parts.append(py::to_str(*spec.edx));
    }
    if (spec.room && *spec.room) {
        parts.append(py::str("'room'"));
        parts.append(py::to_str(*spec.room));
    }
    // a sentinels tuple is nested as ONE element of the outer tuple: it is one part above, as the Python writes it
    const py::str text = py::str("(") + py::join(py::str(", "), parts) + (parts.size() == 1 ? py::str(",") : py::str("")) + py::str(")");
    return py::percent("r%d:%s:%s", compare::REV, sha, py::hashlib::sha1(text.encode()).hexdigest());
}

// The oracle's stack (and the RET landing pad just under it) sits at the top of the scratch region. Writes there are the
// function's own frame, laid out differently by every compiler.
namespace {
Cache<std::string, std::tuple<int64_t, int64_t>>& image_spans() {
    static Cache<std::string, std::tuple<int64_t, int64_t>> c;
    return c;
}
Cache<std::string, int64_t>& insn_counts() {
    static Cache<std::string, int64_t> c;
    return c;
}
}  // namespace

// (lo, hi) addresses of a PE image as loaded.
std::tuple<int64_t, int64_t> _image_span(const py::str& path) {
    std::tuple<int64_t, int64_t> had;
    if (image_spans().get(path.utf8(), had)) {
        return had;
    }
    std::tuple<int64_t, int64_t> out{0, 0};
    try {
        formats::pe::PEFile pe(path);
        int64_t top = 0;
        for (const formats::pe::Section& s : pe.sections) {
            top = std::max<int64_t>(top, s.vaddr + std::max(s.vsize, s.rawsize));
        }
        out = {static_cast<int64_t>(pe.image_base), static_cast<int64_t>(pe.image_base) + top};
    } catch (const std::exception&) {
        out = {0, 0};
    }
    image_spans().put(path.utf8(), out);
    return out;
}

// What the function left in memory outside its own frame: a digest of the final value of every byte it wrote
// (lane_common.left). A function that stores to a global or through a pointer argument is judged on what it stored and
// where -- the return value alone says nothing about either. A byte left holding what it held on entry is not an
// effect: one compiler clears a flag with a 16-bit `and`, another with an 8-bit one, and memory ends up the same.
//
// `own` is the address range of a REBUILT image: writes into it are that build's own statics (compiler temporaries,
// register stand-ins), which the original has no counterpart for. The original's globals are reached by their literal
// addresses on both sides, so they compare. A stored stack address is recorded as such (its value differs by build). A
// call through a pointer that resolves to nothing lands in zero bytes; the oracle returns from it after one
// `add [eax], al`: that write is the harness's (the record says so), not the function's. None = too many writes were
// made to record them all.
std::optional<py::str> _effects(const emulate::run::Run& r, const std::optional<std::tuple<int64_t, int64_t>>& own) {
    auto inside = [&](int64_t a) { return own && std::get<0>(*own) <= a && a < std::get<1>(*own); };
    auto place = [&](int64_t a) -> std::optional<lane_common::Where> {
        if ((_STACK_LO <= a && a < _STACK_HI) || inside(a)) {
            return std::nullopt;
        }
        return lane_common::Where(a);
    };
    auto word = [&](int64_t v) -> std::optional<py::str> {
        return ((_STACK_LO <= v && v < _STACK_HI) || inside(v)) ? std::optional<py::str>(py::str("S")) : std::nullopt;
    };
    return lane_common::left(r, place, word);
}

static std::shared_ptr<em::Trace> trace_of(const emulate::run::Run& r) { return std::static_pointer_cast<em::Trace>(r.raw); }

// (own, reach): offsets of the function's own instructions that ran, and the addresses of every instruction that ran
// anywhere in the module (the function and the callees it entered).
std::tuple<py::list<int64_t>, py::list<int64_t>> _coverage(const emulate::run::Run& r) {
    const std::shared_ptr<em::Trace> t = trace_of(r);
    const int64_t lo = t ? t->func_va : 0, hi = t ? t->func_end_va : 0;
    std::vector<int64_t> own, reach;
    for (int64_t a : r.path) {
        reach.push_back(a);
        if (lo <= a && a < hi) {
            own.push_back(a - lo);
        }
    }
    std::sort(own.begin(), own.end());
    std::sort(reach.begin(), reach.end());
    if (static_cast<int64_t>(reach.size()) > _MAX_REACH) {
        reach.resize(_MAX_REACH);
    }
    py::list<int64_t> o, rc;
    for (int64_t a : own) {
        o.append(a);
    }
    for (int64_t a : reach) {
        rc.append(a);
    }
    return {o, rc};
}

// How many instructions the function at `func` has, or 0: those reachable from its entry through its own jumps (a decode
// to the next known function also counted what lies after the function's last jump or return -- the rest of an
// import-thunk table, padding, the next function's data -- and so called a one-instruction thunk 2% covered). Where the
// function jumps through a register or a table, its reach is not known: then every instruction to the end of its extent,
// as before.
int64_t insn_count(const py::str& path, int64_t func) {
    const std::string key = path.utf8() + "\x01" + std::to_string(func);
    int64_t had = 0;
    if (insn_counts().get(key, had)) {
        return had;
    }
    int64_t n = 0;
    try {
        const em::ResolvedFunc f = em::_resolve_func(path, em::FuncSelector(func));
        n = _reachable_insns(f.code.slice(f.off, f.end), f.va0 + f.off);
    } catch (const std::exception&) {
        n = 0;
    }
    insn_counts().put(key, n);
    return n;
}

int64_t _reachable_insns(const py::bytes& code, int64_t start) {
    const int64_t lo = start, hi = start + code.size();
    std::set<int64_t> seen;
    std::vector<int64_t> todo{start};
    while (!todo.empty()) {
        int64_t a = todo.back();
        todo.pop_back();
        while (lo <= a && a < hi && !seen.count(a)) {
            const std::string chunk = code.raw().substr(static_cast<size_t>(a - lo), 16);
            const std::vector<DIns> one = decode32(chunk, a, 1);
            if (one.empty()) {
                break;
            }
            const DIns& ins = one[0];
            seen.insert(a);
            const std::string& mn = ins.mn;
            const Op* op = ins.ops.empty() ? nullptr : &ins.ops[0];
            if (mn == "ret" || mn == "retf" || mn == "iret" || mn == "iretd" || mn == "hlt" || mn == "int3" || mn == "ud2") {
                break;
            }
            if (mn == "jmp" || (!mn.empty() && mn[0] == 'j' && op != nullptr)) {
                if (op != nullptr && op->type == X86_OP_IMM) {
                    todo.push_back(op->imm & M32);
                } else if (mn == "jmp" && !(op != nullptr && op->type == X86_OP_MEM && !op->base && !op->index)) {
                    // through a register or a table: where it goes is not known here
                    return static_cast<int64_t>(decode32(code.raw(), start, 0).size());
                }
                if (mn == "jmp") {
                    break;
                }
            }
            a += ins.size;
        }
    }
    return static_cast<int64_t>(seen.size());
}

namespace {
using ThunkStubs = std::tuple<py::dict<int64_t, std::tuple<py::str, py::str>>, py::dict<py::str, std::optional<int64_t>>>;
Cache<std::string, ThunkStubs>& thunk_cache() {
    static Cache<std::string, ThunkStubs> c;
    return c;
}
}  // namespace

// {address: ("THUNK", name)} and {name: argument count} for the stubs a flat thunk's compiler generated
// (symbols/runtime_library: identified, not rebuilt). A call into one goes on into 16-bit code through a pointer only
// Windows fills in; the judge enters it as it enters an import -- removing the arguments its `ret N` removes -- so the
// functions that call the 16-bit side can be judged at all (mnmcpi32.dll: 76 of them could not).
std::tuple<py::dict<int64_t, std::tuple<py::str, py::str>>, py::dict<py::str, std::optional<int64_t>>> _thunk_stubs(
    const py::str& original) {
    ThunkStubs had;
    if (thunk_cache().get(original.utf8(), had)) {
        return had;
    }
    py::dict<int64_t, std::tuple<py::str, py::str>> at;
    py::dict<py::str, std::optional<int64_t>> argc;
    try {
        formats::pe::PEFile pe(original);
        const auto sections = pe.code_sections();
        for (int64_t va : symbols::runtime_library::_flat_thunk_stubs(original, pe, sections)) {
            const py::str name = py::percent("stub_%x", va);
            at.set(va, {py::str("THUNK"), name});
            for (const auto& sec : sections) {
                const int64_t base = static_cast<int64_t>(std::get<0>(sec));
                const py::bytes& raw = std::get<1>(sec);
                if (base <= va && va < base + raw.size()) {
                    for (const DIns& ins : decode32(raw.slice(va - base, va - base + 512).raw(), va, 0, false)) {
                        if (ins.mn == "ret" || ins.mn == "retn") {
                            const int64_t n = ins.op_str.empty() ? 0 : py::int_parse(py::str(ins.op_str), 0);
                            argc.set(name, (n % 4 == 0 && n <= 256) ? std::optional<int64_t>(n / 4) : std::nullopt);
                            break;
                        }
                    }
                    break;
                }
            }
        }
    } catch (const std::exception&) {
        at = py::dict<int64_t, std::tuple<py::str, py::str>>();
        argc = py::dict<py::str, std::optional<int64_t>>();
    }
    ThunkStubs out{at, argc};
    thunk_cache().put(original.utf8(), out);
    return out;
}

// How a run enters the stubs: as imports (_thunk_stubs), each with its argument count. The same for a fingerprint run and
// a branch-logged one, so that the inputs aimed at branches are chosen on the program the judge runs.
StubOptions _stub_options(const py::str& original) {
    const auto stubs = _thunk_stubs(original);
    StubOptions o;
    if (!std::get<0>(stubs).empty()) {
        o.stub_at = std::get<0>(stubs);
    }
    const py::dict<py::str, std::optional<int64_t>> stub_argc = std::get<1>(stubs);
    o.sig_resolver = [stub_argc](const py::str& d, const py::str& n) -> std::optional<int64_t> {
        if (d == py::str("THUNK")) {
            const auto it = stub_argc.get(n);
            return it ? *it : std::nullopt;
        }
        return _argc(d, n);
    };
    return o;
}

namespace {

// the options of one run, as _trace_pe / _branch_run / explore_vectors give them
em::TraceOptions options_for(const Vector& spec, const std::optional<py::str>& overlay, bool branches, bool fast, const py::str& stubs_of) {
    em::TraceOptions o;
    o.args = spec.args;
    o.mem_init = spec.mem_init;
    o.insn_cap = spec.room && *spec.room ? *spec.room : em::_MAX_INSNS;
    o.stub_externals = true;
    o.stub_ret = spec.stub_ret ? *spec.stub_ret : 0;
    o.overlay = overlay;
    if (spec.sentinels && spec.sentinels->size() == 4) {
        o.sentinels = std::array<int64_t, 4>{(*spec.sentinels)[0], (*spec.sentinels)[1], (*spec.sentinels)[2], (*spec.sentinels)[3]};
    }
    o.reads = false;
    o.stop_wild = true;
    o.ecx = spec.ecx;
    o.edx = spec.edx;
    o.quiet_stack = true;
    const bool plain = lane_common::PLAIN;
    o.reuse = !plain;
    o.fast = fast && !plain;
    o.lean = !plain;
    o.pads = std::make_tuple(_PADS[0], _PADS[1]);
    o.branches = branches;
    const StubOptions s = _stub_options(stubs_of);
    o.stub_at = s.stub_at;
    o.sig_resolver = s.sig_resolver;
    return o;
}

bool wild_of(const em::Trace& t) { return (t.bogus_stubs && *t.bogus_stubs) || (t.ran_into_zeros && *t.ran_into_zeros); }

}  // namespace

// Fingerprint one run. For a REBUILT image pass `rebuilt` = the path of the original: its own statics are then left out
// of the comparison and the original's code is mapped beside it.
//
// Kept across runs under the module's content hash (lane_common.remembered, _fp_key). A run cut off at the step limit is
// kept too: a run stops on counts alone (the tracer's old wall-clock cap is gone), the same on every machine, so a
// truncated run is as settled as any other -- and they are the slowest to make again.
compare::Fingerprint _fp_pe(const py::str& path, int64_t func, const Vector& spec, const std::optional<py::str>& rebuilt) {
    const std::optional<py::str> sha = base::corpus::sha(path);
    return lane_common::remembered(sha ? std::optional<py::str>(_fp_key(*sha, func, spec)) : std::nullopt,
                                   [&]() { return _trace_pe(path, func, spec, rebuilt); });
}

// One uncached run as a fingerprint (an error is a fingerprint with `err` set; an ImportError goes on).
compare::Fingerprint _trace_pe(const py::str& path, int64_t func, const Vector& spec, const std::optional<py::str>& rebuilt) {
    emulate::run::Run r(std::nullopt);
    try {
        r = em::recorded(path, em::FuncSelector(func), options_for(spec, rebuilt, false, true, rebuilt ? *rebuilt : path));
    } catch (const py::ImportError&) {
        throw;                                     // a broken setup is not an observation
    } catch (const py::Exception& e) {
        compare::Fingerprint fp;                   // transient trace errors are never cached
        fp.err = py::str(e.type());
        return fp;
    }
    // A call that landed in zero bytes went through a pointer nothing in this environment fills in (a 32->16 thunk entry
    // the loader would set, an uninitialised callback). What the ORIGINAL does after that is the harness's doing, not
    // the function's: the run is not an observation. A REBUILT body that does it where the original did not has simply
    // gone somewhere wrong.
    const std::shared_ptr<em::Trace> t = trace_of(r);
    const bool wild = t && wild_of(*t);
    if (wild && !rebuilt) {
        compare::Fingerprint fp;
        fp.err = py::str("call-into-nothing");
        return fp;
    }
    std::optional<std::tuple<int64_t, int64_t>> own;
    if (rebuilt) {
        own = _image_span(path);
    }
    const py::list<int64_t> given = spec.given ? *spec.given : spec.args;
    const py::list<py::str> deps = _call_seq(lane_common::called(r.calls), given, own);
    std::optional<int64_t> eax;
    if (r.results.contains(py::str("EAX"))) {
        const emulate::run::Register& reg = r.results.at(py::str("EAX"));
        if (std::holds_alternative<int64_t>(reg)) {
            eax = std::get<int64_t>(reg);
        }
    }
    py::list<int64_t> cov, reach;
    if (!rebuilt) {
        std::tie(cov, reach) = _coverage(r);
    }
    // how the function left the stack pointer (4 = the caller removes the arguments, 4 + N = the function removed N bytes
    // of them) and whether it preserved EBX/ESI/EDI/EBP
    compare::Fingerprint fp = lane_common::fingerprint(r, deps, eax, _effects(r, own), r.kept(), cov, reach, r.steps);
    if (wild) {
        fp.exit = py::str("wild-call");
    }
    return fp;
}

// ---- inputs aimed at branches --------------------------------------------------------------------------------------
namespace {
Cache<std::string, std::tuple<py::list<int64_t>, py::list<int64_t>>>& harvests() {
    static Cache<std::string, std::tuple<py::list<int64_t>, py::list<int64_t>>> c;
    return c;
}
Cache<std::string, py::list<std::tuple<int64_t, py::bytes>>>& fills() {
    static Cache<std::string, py::list<std::tuple<int64_t, py::bytes>>> c;
    return c;
}
Cache<std::string, py::list<std::tuple<int64_t, int64_t>>>& datas() {
    static Cache<std::string, py::list<std::tuple<int64_t, int64_t>>> c;
    return c;
}
}  // namespace

// (constants, globals) read off the function's own instructions: the immediates it compares or masks with, and the
// absolute data addresses it reads. These are what its branches test, so they are what an input has to contain to get
// past them.
std::tuple<py::list<int64_t>, py::list<int64_t>> _harvest(const py::str& path, int64_t func) {
    const std::string key = path.utf8() + "\x01" + std::to_string(func);
    std::tuple<py::list<int64_t>, py::list<int64_t>> had;
    if (harvests().get(key, had)) {
        return had;
    }
    py::list<int64_t> consts, globs;
    try {
        const auto span = _image_span(path);
        const int64_t lo = std::get<0>(span), hi = std::get<1>(span);
        formats::pe::PEFile pe(path);
        std::set<int64_t> iat;
        for (const auto& kv : pe.imports().items()) {
            iat.insert(static_cast<int64_t>(std::get<0>(kv)));
        }
        // The function first, then the functions it calls: a caller often cannot get past `if (!helper(x)) return` until
        // the input satisfies what the HELPER tests.
        std::vector<std::pair<int64_t, int64_t>> todo{{func, 0}};
        std::set<int64_t> seen;
        while (!todo.empty() && static_cast<int64_t>(seen.size()) < _HARVEST_FUNCS) {
            const auto [f, depth] = todo.front();
            todo.erase(todo.begin());
            if (seen.count(f)) {
                continue;
            }
            seen.insert(f);
            em::ResolvedFunc rf;
            try {
                rf = em::_resolve_func(path, em::FuncSelector(f));
            } catch (const std::exception&) {
                continue;
            }
            for (const DIns& ins : decode32(rf.code.slice(rf.off, rf.end).raw(), rf.va0 + rf.off, 0)) {
                if (ins.mn == "call" && depth < _HARVEST_DEPTH && !ins.ops.empty() && ins.ops[0].type == X86_OP_IMM) {
                    todo.push_back({ins.ops[0].imm & M32, depth + 1});
                    continue;
                }
                for (const Op& op : ins.ops) {
                    if (op.type == X86_OP_IMM && (ins.mn == "cmp" || ins.mn == "test" || ins.mn == "sub" || ins.mn == "and")) {
                        const int64_t v = op.imm & M32;
                        if (v && !consts.contains(v)) {
                            consts.append(v);
                        }
                    } else if (op.type == X86_OP_MEM && !op.base && !op.index) {
                        const int64_t a = op.disp & M32;
                        if (lo <= a && a < hi && !iat.count(a) && !globs.contains(a) && ins.mn != "lea") {
                            globs.append(a);
                        }
                    }
                }
            }
        }
    } catch (const std::exception&) {
    }
    std::tuple<py::list<int64_t>, py::list<int64_t>> out{consts.slice(0, 10), globs.slice(0, 24)};
    harvests().put(key, out);
    return out;
}

// mem_init that gives every word of the original's writable sections a value of its own: the address of a landing pad,
// so one used as a function pointer is called and returns, and which was called is seen. The emulator does not load a
// module's data, so its globals all read as zero: two of them passed in each other's places, or a loop that clears a
// table it never reaches, look the same as the right code while both are zero. (The file's own data is not used: its
// tables of initialisers would be run, which is the harness's doing, not the body's.)
py::list<std::tuple<int64_t, py::bytes>> _data_filled(const py::str& path) {
    py::list<std::tuple<int64_t, py::bytes>> had;
    if (fills().get(path.utf8(), had)) {
        return had;
    }
    py::list<std::tuple<int64_t, py::bytes>> out;
    try {
        formats::pe::PEFile pe(path);
        for (const formats::pe::Section& s : pe.sections) {
            if (!(s.chars & 0x80000000LL) || (s.chars & 0x20000000LL) || !s.vsize) {
                continue;                                    // writable data only, not code
            }
            const int64_t at = static_cast<int64_t>(pe.image_base) + s.vaddr;
            std::string body;
            for (int64_t i = 0; i < std::min<int64_t>(s.vsize, _FILL_MAX) / 4; ++i) {
                body += u32le(_SAFE + 4 * (((at >> 2) + i) % 0x400)).raw();
            }
            out.append({at, bytes_of(body)});
        }
    } catch (const std::exception&) {
        out = py::list<std::tuple<int64_t, py::bytes>>();
    }
    fills().put(path.utf8(), out);
    return out;
}

// Inputs chosen for THIS function. Generic inputs stop at the first guard (`if (msg != 0x4E) return`, `if
// (!g_initialised) return`), so most of a body never ran and was 'verified' on its first few instructions. Candidates put
// each constant the function compares against into each argument, and set the globals it reads to non-zero values; a
// candidate is kept only if the ORIGINAL then runs instructions no earlier input reached. Returns at most MAX_DIRECTED.
py::list<Vector> directed_vectors(const py::str& path, int64_t func, const py::set<int64_t>& covered, int64_t arity,
                                  const py::set<int64_t>& reached, const Shape& shape) {
    (void)covered;
    const auto hv = _harvest(path, func);
    const py::list<int64_t>& consts = std::get<0>(hv);
    const py::list<int64_t>& globs = std::get<1>(hv);
    if (consts.empty() && globs.empty() && _data_filled(path).empty()) {
        return py::list<Vector>();
    }
    const int64_t n = arity;
    const py::list<std::tuple<int64_t, py::bytes>> mi = _prov_meminit();
    py::list<int64_t> obj;
    for (int64_t i = 0; i < n; ++i) {
        obj.append(_OBJ + 0x40 * (i % 32));
    }
    const py::list<int64_t> ints = lane_common::counting(n);
    py::list<Vector> cands;
    py::list<py::list<std::tuple<int64_t, py::bytes>>> gsets;
    if (!globs.empty()) {
        for (int64_t val : {int64_t(1), int64_t(_SAFE), int64_t(_OBJ + 0x800)}) {
            py::list<std::tuple<int64_t, py::bytes>> g;
            for (int64_t a : globs) {
                g.append({a, u32le(val)});
            }
            gsets.append(g);
        }
        // each global a value of its own (see _data_filled)
        py::list<std::tuple<int64_t, py::bytes>> g;
        int64_t i = 0;
        for (int64_t a : globs) {
            g.append({a, u32le(_SAFE + 4 * i)});
            ++i;
        }
        gsets.append(g);
    }
    auto make = [](const py::list<int64_t>& args, const std::optional<py::list<std::tuple<int64_t, py::bytes>>>& mem,
                   std::optional<int64_t> stub_ret) {
        Vector v;
        v.args = args;
        v.mem_init = mem;
        v.stub_ret = stub_ret;
        return v;
    };
    const py::list<std::tuple<int64_t, py::bytes>> filled = _data_filled(path);
    if (!filled.empty()) {
        cands.append(make(obj, cat(mi, filled), 1));
        cands.append(make(ints, cat(mi, filled), std::nullopt));
    }
    for (const auto& g : gsets) {
        cands.append(make(obj, cat(mi, g), 1));
        cands.append(make(ints, cat(mi, g), std::nullopt));
    }
    // (Every position is tried, also past the arguments the function removes on return: a function does sometimes read
    // further up the stack, and one difference was only found so.)
    for (int64_t k : consts) {
        for (int64_t i = 0; i < n; ++i) {
            py::list<int64_t> a = obj.copy();
            a[i] = k;
            cands.append(make(a, mi, 1));
            if (!gsets.empty()) {
                cands.append(make(a, cat(mi, gsets[1]), 1));
            }
        }
        {
            py::list<int64_t> a;
            for (int64_t i = 0; i < n; ++i) {
                a.append(k);
            }
            cands.append(make(a, std::nullopt, 1));
        }
        // ...and into every field of the objects the arguments point at (`if (p->flags & 0x40)`).
        std::string four;
        for (int i = 0; i < 0x400; ++i) {
            four += u32le(k).raw();
        }
        py::list<std::tuple<int64_t, py::bytes>> fill;
        fill.append({_OBJ, bytes_of(four)});
        fill.append({_SAFE, bytes_of(std::string(16, '\xc3'))});
        cands.append(make(obj, fill, 1));
        cands.append(make(obj, fill, std::nullopt));
    }
    // "New code" is counted over everything that ran, callees included: an input that drives a helper down a different
    // path exercises the caller's use of it even at 100% own coverage.
    if (shape) {                                    // (a member function: the first value goes in ECX)
        py::list<Vector> shaped;
        for (const Vector& v : cands) {
            shaped.append(shape(v));
        }
        cands = shaped;
    }
    py::list<Vector> keep;
    std::set<int64_t> seen;
    for (int64_t x : reached) {
        seen.insert(x);
    }
    for (const Vector& v : cands) {
        const compare::Fingerprint f = _fp_pe(path, func, v);
        if (f.err) {
            continue;
        }
        bool fresh = false;
        for (int64_t x : f.reach) {
            if (!seen.count(x)) {
                fresh = true;
                break;
            }
        }
        if (fresh) {
            keep.append(v);
            for (int64_t x : f.reach) {
                seen.insert(x);
            }
            if (static_cast<int64_t>(keep.size()) >= MAX_DIRECTED) {
                return keep;
            }
        }
    }
    // Then inputs that make the original DO something no earlier input made it do, though no new code runs: the values
    // either side of each constant it compares with. `x < 100` and `x < 101` run the same instructions on every input but
    // 100; only that input tells them apart, and no count of instructions reached would ever ask for it.
    auto did = [](const compare::Fingerprint& f) {
        py::list<py::str> parts;
        parts.append(f.exit);
        parts.append(f.ret ? py::to_str(*f.ret) : py::str("None"));
        parts.append(py::join(py::str("\x1f"), f.deps));
        parts.append(f.writes ? *f.writes : py::str("\x1e"));
        return py::join(py::str("\x1d"), parts).utf8();
    };
    std::set<std::string> done;
    {
        py::list<Vector> all = cat(_vectors(arity, py::str("pe")), _confirm_vectors(arity, py::str("pe")));
        py::list<Vector> shaped;
        for (const Vector& v : all) {
            shaped.append(shape ? shape(v) : v);
        }
        for (const Vector& v : cat(shaped, keep)) {
            const compare::Fingerprint f = _fp_pe(path, func, v);
            if (!f.err) {
                done.insert(did(f));
            }
        }
    }
    py::list<Vector> edges;
    for (int64_t k : consts) {
        for (int64_t near : {(k - 1) & M32, (k + 1) & M32, k}) {
            for (int64_t i = 0; i < n; ++i) {
                py::list<int64_t> a = ints.copy();
                a[i] = near;
                edges.append(make(a, std::nullopt, std::nullopt));
                a = obj.copy();
                a[i] = near;
                edges.append(make(a, mi, 1));
            }
        }
    }
    py::list<Vector> shaped_edges;
    for (const Vector& v : edges) {
        shaped_edges.append(shape ? shape(v) : v);
    }
    for (const Vector& v : cat(cands, shaped_edges)) {
        const compare::Fingerprint f = _fp_pe(path, func, v);
        if (f.err) {
            continue;
        }
        const std::string d = did(f);
        if (done.count(d)) {
            continue;
        }
        done.insert(d);
        keep.append(v);
        if (static_cast<int64_t>(keep.size()) >= MAX_DIRECTED) {
            break;
        }
    }
    return keep;
}

// The original run on `spec` with its branches logged: {"branches", "reach"}, or None when the run is not an observation
// (as _fp_pe: an error, or a call into nothing). A flat thunk's stubs are entered as _fp_pe enters them (_stub_options).
std::optional<guided::Observation> _branch_run(const py::str& path, int64_t func, const Vector& spec) {
    emulate::run::Run r(std::nullopt);
    try {
        r = em::recorded(path, em::FuncSelector(func), options_for(spec, std::nullopt, true, true, path));
    } catch (const py::ImportError&) {
        throw;
    } catch (const std::exception&) {
        return std::nullopt;
    }
    const std::shared_ptr<em::Trace> t = trace_of(r);
    if (t && wild_of(*t)) {
        return std::nullopt;
    }
    guided::Observation o;
    if (t && t->branches) {
        for (const emulate::native_trace::BranchEntry& b : *t->branches) {
            std::optional<guided::Compare> cmp;
            if (std::get<2>(b)) {
                const emulate::native_trace::Compare& c = *std::get<2>(b);
                guided::Compare g;
                g.mn = std::get<0>(c);
                g.size = static_cast<int64_t>(std::get<1>(c));
                g.a = static_cast<int64_t>(std::get<2>(c));
                g.b = static_cast<int64_t>(std::get<3>(c));
                if (std::get<4>(c)) {
                    g.at_a = static_cast<int64_t>(*std::get<4>(c));
                }
                if (std::get<5>(c)) {
                    g.at_b = static_cast<int64_t>(*std::get<5>(c));
                }
                g.has_at = true;
                cmp = g;
            }
            o.branches.append({static_cast<int64_t>(std::get<0>(b)), std::get<1>(b), cmp});
        }
    }
    const auto seen_cov = _coverage(r);
    for (int64_t a : std::get<1>(seen_cov)) {
        o.reach.add(a);
    }
    o.args_at = (t ? t->entry_esp : 0) + 4;
    o.stack = std::make_tuple(_STACK_LO, _STACK_HI);
    return o;
}

// Inputs that take branches of the original no seed took (verify/guided.py).
py::list<Vector> guided_vectors(const py::str& path, int64_t func, const py::list<Vector>& seeds, const Shape& shape) {
    py::list<guided::Input> gseeds;
    for (const Vector& v : seeds) {
        gseeds.append(to_guided(v));
    }
    py::list<guided::Input> got = lane_common::guided(
        [&](const guided::Input& g) -> std::optional<guided::Observation> {
            const Vector v = from_guided(g);
            return _branch_run(path, func, shape ? shape(v) : v);
        },
        gseeds, func & 0xFFFF);
    py::list<Vector> out;
    for (const guided::Input& g : got) {
        out.append(from_guided(g));
    }
    return out;
}

// The directed inputs, then those found path by path (or guessed) seeded with every input run so far. `found`: where the
// exploration is left (its concolic::Exploration).
py::list<Vector> directed_and_guided(const py::str& path, int64_t func, const py::set<int64_t>& covered, const py::set<int64_t>& reached,
                                     const Shape& shape, Found* found) {
    const py::list<Vector> d = directed_vectors(path, func, covered, 4, reached, shape);
    const py::list<Vector> seeds = cat(cat(_vectors(ARITY(), py::str("pe")), _confirm_vectors(ARITY(), py::str("pe"))), d);
    if (concolic_ready()) {
        auto [kept, got] = explore_vectors(path, func, seeds, shape);
        if (found != nullptr) {
            found->got = got;
        }
        return cat(d, kept);
    }
    return cat(d, guided_vectors(path, func, seeds, shape));
}

// The lift mode of the platform's processor (emulate/shadow.py mode: its cpu slot).
std::optional<py::str> _mode() { return emulate::shadow::mode(py::str("x86:LE:32:default:windows")); }

// Path by path (verify/concolic.py), as the 16-bit judge finds them (lanes/win16.py): the original run with its symbolic
// shadow (emulate/shadow.py Shadow32, on the decompiler's x86-32-flat p-code) -- its argument dwords (on the stack, or in
// ECX / EDX for a member function), the bytes of the scratch memory pointer arguments point into and of the module's
// writable data, and what an import the tracer answers returns (stub_ret) are the inputs -- and inputs solved for to take
// each path its conditions allow. Where the lifter is not there or CB_NO_CONCOLIC is set, the inputs are guessed as
// before (guided_vectors).
bool concolic_ready() {
    if (!(lane_common::guided_on() && lane_common::concolic_on())) {
        return false;
    }
    const std::optional<py::str> mode = _mode();
    return mode && emulate::pcode::lifter(*mode) != nullptr;
}

// [(low, high)] of the module's writable data sections, as loaded.
py::list<std::tuple<int64_t, int64_t>> _writable(const py::str& path) {
    py::list<std::tuple<int64_t, int64_t>> had;
    if (datas().get(path.utf8(), had)) {
        return had;
    }
    py::list<std::tuple<int64_t, int64_t>> out;
    try {
        formats::pe::PEFile pe(path);
        for (const formats::pe::Section& s : pe.sections) {
            if ((s.chars & 0x80000000LL) && !(s.chars & 0x20000000LL)) {
                const int64_t base = static_cast<int64_t>(pe.image_base) + s.vaddr;
                out.append({base, base + std::max(s.vsize, s.rawsize)});
            }
        }
    } catch (const std::exception&) {
        out = py::list<std::tuple<int64_t, int64_t>>();
    }
    datas().put(path.utf8(), out);
    return out;
}

// Input `v` (as the run is given it: `shape`d) with the named inputs (shadow.vars) set to `values`: an argument dword,
// ECX or EDX, what the answered imports return, bytes of memory (put after the input's own memory, so they are what the
// run finds there).
std::optional<Vector> _derive(const Vector& v, const concolic::Values& values, const concolic::Vars& names, const Shape& shape) {
    Vector w = shape ? shape(v) : copy_of(v);
    py::list<int64_t> args = w.args.copy();
    std::map<int64_t, int64_t> mem;
    bool changed = false;
    for (const auto& kv : values.items()) {
        const py::str& name = std::get<0>(kv);
        const int64_t value = std::get<1>(kv);
        py::str kind("");
        int64_t where = 0, was = 0;
        if (names.contains(name)) {
            const auto& t = names.at(name);
            kind = std::get<0>(t);
            where = std::get<1>(t);
            was = std::get<3>(t);
        }
        if (kind == py::str("arg") && where < static_cast<int64_t>(args.size())) {
            changed |= args[where] != (value & M32);
            args[where] = value & M32;
        } else if (kind == py::str("reg") && where == 1 && w.ecx) {
            changed |= *w.ecx != (value & M32);
            w.ecx = value & M32;
        } else if (kind == py::str("reg") && where == 2 && w.edx) {
            changed |= *w.edx != (value & M32);
            w.edx = value & M32;
        } else if (kind == py::str("ret")) {
            changed |= (w.stub_ret ? *w.stub_ret : 0) != (value & M32);
            w.stub_ret = value & M32;
        } else if (kind == py::str("mem")) {
            changed |= was != (value & 0xFF);
            mem[where] = value & 0xFF;
        }
    }
    if (!changed) {
        return std::nullopt;
    }
    w.args = args;
    if (w.given) {                                           // (what the calls' arguments are named by)
        py::list<int64_t> g;
        g.append(*w.ecx);
        if (w.edx) {
            g.append(*w.edx);
        }
        for (int64_t a : args) {
            g.append(a);
        }
        w.given = g.slice(0, static_cast<int64_t>(w.given->size()));
    }
    if (!mem.empty()) {
        std::vector<std::pair<int64_t, std::string>> runs;
        for (const auto& [at, byte] : mem) {
            if (!runs.empty() && runs.back().first + static_cast<int64_t>(runs.back().second.size()) == at) {
                runs.back().second.push_back(static_cast<char>(byte));
            } else {
                runs.push_back({at, std::string(1, static_cast<char>(byte))});
            }
        }
        py::list<std::tuple<int64_t, py::bytes>> m = w.mem_init ? w.mem_init->copy() : py::list<std::tuple<int64_t, py::bytes>>();
        for (const auto& r : runs) {
            m.append({r.first, bytes_of(r.second)});
        }
        w.mem_init = m;
    }
    return w;
}

// (inputs, exploration): inputs taking paths of the original no seed took, found path by path (verify/concolic.py;
// lane_common.explored), and how far the exploration got. The seeds are run first as the guessing explorer runs them
// (their branches logged): of those that took the same path only one is run again with its shadow. Where the paths found
// are not all there are and a jump of the function's own was still seen going one way only, what is left of the guessing
// explorer's budget (lane_common.GUIDED_REST) goes to it, seeded with every run made.
std::tuple<py::list<Vector>, std::shared_ptr<concolic::Exploration<Vector>>> explore_vectors(const py::str& path, int64_t func,
                                                                                            const py::list<Vector>& seeds,
                                                                                            const Shape& shape) {
    em::ResolvedFunc rf;
    try {
        rf = em::_resolve_func(path, em::FuncSelector(func));
    } catch (const std::exception&) {
        return {guided_vectors(path, func, seeds, shape), nullptr};
    }
    const int64_t lo = rf.va0 + rf.off, hi = rf.va0 + rf.end;
    const std::optional<py::str> mode = _mode();
    const std::shared_ptr<emulate::pcode::Lifter> lifter = mode ? emulate::pcode::lifter(*mode) : nullptr;
    const py::list<std::tuple<int64_t, int64_t>> data = _writable(path);
    emulate::shadow::Inputs inputs;
    inputs.append({em::_SCRATCH_BASE, em::_SCRATCH_BASE + em::_SCRATCH_SIZE});
    for (const auto& d : data) {
        inputs.append(d);
    }

    std::vector<std::pair<Vector, guided::Observation>> seen;
    py::dict<int64_t, concolic::Trail> known;
    int64_t i = 0;
    for (const Vector& v : seeds) {
        const std::optional<guided::Observation> o = _branch_run(path, func, shape ? shape(v) : v);
        if (o) {
            seen.push_back({v, *o});
            concolic::Trail trail;
            for (const auto& b : o->branches) {
                trail.append(concolic::Step{std::get<0>(b), std::get<1>(b)});
            }
            known.set(i, trail);
        }
        ++i;
    }

    auto run = [&](Vector& v) -> std::optional<concolic::Observation> {
        const Vector w = shape ? shape(v) : v;
        std::shared_ptr<emulate::native_shadow::Shadow> sh = emulate::shadow::Shadow32(lifter, inputs, std::make_tuple(lo, hi));
        const int64_t esp = em::_STACK_TOP - 4 * static_cast<int64_t>(w.args.size()) - 4;
        py::list<emulate::native_shadow::Place> places;
        int64_t k = 0;
        for (int64_t a : w.args) {
            emulate::native_shadow::Place p;
            p.where = esp + 4 + 4 * k;
            p.value = a & M32;
            p.k = std::variant<int64_t, py::str>(k);
            places.append(p);
            ++k;
        }
        for (const char* reg : {"ecx", "edx"}) {
            const std::optional<int64_t>& val = std::string(reg) == "ecx" ? w.ecx : w.edx;
            if (val) {
                emulate::native_shadow::Place p;
                p.where = py::str(reg).upper();
                p.value = *val & M32;
                p.k = std::variant<int64_t, py::str>(py::str(reg));
                places.append(p);
            }
        }
        sh->enter(places, 32);
        emulate::run::Run r(std::nullopt);
        try {
            em::TraceOptions o = options_for(w, std::nullopt, true, false, path);
            o.shadow = sh.get();
            r = em::recorded(path, em::FuncSelector(func), o);
        } catch (const py::ImportError&) {
            throw;
        } catch (const std::exception&) {
            return std::nullopt;
        }
        const std::shared_ptr<em::Trace> t = trace_of(r);
        if (t && wild_of(*t)) {
            return std::nullopt;
        }
        {
            guided::Observation o;
            if (t && t->branches) {
                for (const emulate::native_trace::BranchEntry& b : *t->branches) {
                    std::optional<guided::Compare> cmp;
                    if (std::get<2>(b)) {
                        const emulate::native_trace::Compare& c = *std::get<2>(b);
                        guided::Compare g;
                        g.mn = std::get<0>(c);
                        g.size = static_cast<int64_t>(std::get<1>(c));
                        g.a = static_cast<int64_t>(std::get<2>(c));
                        g.b = static_cast<int64_t>(std::get<3>(c));
                        if (std::get<4>(c)) {
                            g.at_a = static_cast<int64_t>(*std::get<4>(c));
                        }
                        if (std::get<5>(c)) {
                            g.at_b = static_cast<int64_t>(*std::get<5>(c));
                        }
                        g.has_at = true;
                        cmp = g;
                    }
                    o.branches.append({static_cast<int64_t>(std::get<0>(b)), std::get<1>(b), cmp});
                }
            }
            const auto seen_cov = _coverage(r);
            for (int64_t a : std::get<1>(seen_cov)) {
                o.reach.add(a);
            }
            o.args_at = esp + 4;
            o.stack = std::make_tuple(_STACK_LO, _STACK_HI);
            seen.push_back({v, o});
        }
        concolic::Observation out;
        out.trail = sh->trail();
        for (const emulate::native_shadow::PathEntry& e : sh->path()) {
            concolic::PathEntry pe;
            pe.site = e.site;
            pe.taken = e.taken;
            pe.cond = e.cond;
            pe.kind = e.kind;
            out.path.append(pe);
        }
        for (const auto& kv : sh->vars().items()) {
            const emulate::native_shadow::Var& var = std::get<1>(kv);
            int64_t where = 0;
            if (std::holds_alternative<int64_t>(var.where)) {
                where = std::get<int64_t>(var.where);
            } else if (std::holds_alternative<py::str>(var.where)) {
                const py::str& nm = std::get<py::str>(var.where);
                where = nm == py::str("ecx") ? 1 : nm == py::str("edx") ? 2 : 0;
            }
            out.vars.set(std::get<0>(kv), {var.kind, where, var.bits, var.value});
        }
        out.lost = sh->lost();
        if (r.truncated) {
            out.lost.add(py::str("truncated"));
        }
        out.cut = r.truncated;
        return out;
    };
    py::list<Vector> seed_list = seeds;
    concolic::Exploration<Vector> got = lane_common::explored<Vector>(
        run, seed_list, [&](const Vector& v, const concolic::Values& values, const concolic::Vars& names) { return _derive(v, values, names, shape); },
        [&](int64_t site) { return lo <= site && site < hi; }, known);
    py::list<Vector> kept = got.kept.copy();
    std::map<int64_t, int64_t> one_way;
    for (const concolic::Step& e : got.edges) {
        one_way[std::get<0>(e)] |= (int64_t(1) << (std::get<1>(e) ? 1 : 0));
    }
    bool partial = false;
    for (const auto& kv : one_way) {
        if (kv.second != 3) {
            partial = true;
        }
    }
    if (!got.complete && partial && lane_common::GUIDED_REST && !seen.empty()) {
        py::list<std::tuple<guided::Input, guided::Observation>> observed;
        for (const auto& s : seen) {
            observed.append({to_guided(s.first), s.second});
        }
        const py::list<guided::Input> extra = guided::guided_inputs(
            [&](const guided::Input& g) -> std::optional<guided::Observation> {
                const Vector v = from_guided(g);
                return _branch_run(path, func, shape ? shape(v) : v);
            },
            py::list<guided::Input>(), lane_common::GUIDED_REST, lane_common::MAX_GUIDED, func & 0xFFFF, py::str("little"), 4, observed);
        for (const guided::Input& g : extra) {
            kept.append(from_guided(g));
        }
        got.guessed = static_cast<int64_t>(extra.size());
    }
    return {kept, std::make_shared<concolic::Exploration<Vector>>(got)};
}

// Bits of the original's return value that are not a result at all.
//
// A function returning a 16- or 8-bit value sets AX or AL and leaves whatever was in the rest of EAX -- often a copy of a
// register the caller owns. Run the original again with those registers holding the opposite bits: every bit of the
// return value that flips is such a leftover. Only whole upper parts count (the low byte must stay put), so a real
// result is never excused.
int64_t undefined_ret_bits(const std::function<compare::Fingerprint(const Vector&)>& fp_retail, const Vector& v,
                           const compare::Fingerprint& a) {
    if (a.exit != py::str("ret") || !a.ret) {
        return 0;
    }
    Vector alt = copy_of(v);
    py::list<int64_t> s;
    for (int64_t x : _ALT_SENTINELS) {
        s.append(x);
    }
    alt.sentinels = s;
    const compare::Fingerprint c = fp_retail(alt);
    if (c.err || c.exit != py::str("ret") || !c.ret || !(c.deps == a.deps)) {
        return 0;
    }
    const int64_t flips = (*a.ret ^ *c.ret) & M32;
    for (int64_t mask : {int64_t(0xFFFF0000LL), int64_t(0xFFFFFF00LL)}) {
        if (flips && !(flips & ~mask)) {
            return mask;
        }
    }
    // the whole upper half follows the caller's registers and the low half moves too: the function returns nothing, and
    // EAX is simply what its last instructions left there
    if ((flips & 0xFFFF0000LL) == 0xFFFF0000LL && (flips & 0xFFFF)) {
        return 0xFFFFFFFFLL;
    }
    return 0;
}

// Worker: judge one function. item = (key, address, dll, entry, regs).
reconstruct::Verdict judge(const py::str& module_path, const Item& item) {
    const int64_t va = item.va, entry = item.entry;
    const py::str dll = item.dll;
    const bool member = item.regs != 0;
    const Shape shape = member ? (item.regs == 2 ? Shape(in_ecx_edx) : Shape(in_ecx)) : Shape([](const Vector& v) { return v; });

    auto rebuilt = [&](const Vector& v) -> compare::Fingerprint {
        // A rebuild may spend many more instructions than the original on the same work -- a C loop where the original
        // filled memory with one `rep stosd` (dnr.exe's memset over 64 KB: 16,418 steps for the original, 229,414 for the
        // loop, the same 16,387 writes) -- and stop at the step limit where the original returned. Such a run is tried
        // again with room before it counts as different -- also where the original itself stopped at the limit: the
        // rebuild must reach every call the original made by then.
        compare::Fingerprint b = _fp_pe(dll, entry, shape(v), module_path);
        if (b.truncated && b.exit != py::str("wild-call")) {
            const compare::Fingerprint o = _fp_pe(module_path, va, shape(v));
            if (o.exit == py::str("ret") || o.exit == py::str("truncated")) {
                Vector w = shape(v);
                w.room = ROOM;
                b = _fp_pe(dll, entry, w, module_path);
            }
        }
        return b;
    };

    Found found;
    auto judged = [&](compare::Info& info) -> std::tuple<py::str, py::str, py::list<py::str>> {
        auto [verdict, reason, limits] = compare::compare<Vector>(
            [&](const Vector& v) { return _fp_pe(module_path, va, shape(v)); }, rebuilt, _vectors(ARITY(), py::str("pe")),
            _confirm_vectors(ARITY(), py::str("pe")), insn_count(module_path, va),
            [&](const py::set<int64_t>& cov, const py::set<int64_t>& rch) {
                return directed_and_guided(module_path, va, cov, rch, member ? shape : Shape(), &found);
            },
            &info, undefined_ret_bits);
        if (verdict != py::str(V::DIVERGENT)) {
            limits = cat(limits, lane_common::path_limits(found.got.get()));
        }
        return {verdict, reason, limits};
    };
    return lane_common::judged(judged);
}

// ---- the kind -------------------------------------------------------------------------------------------------------
py::str Win32Kind::KIND() const { return win32::KIND; }

std::tuple<py::str, py::str> Win32Kind::profile(const py::str& module_path) const { return win32::profile(module_path); }

reconstruct::Prepared Win32Kind::prepare(const py::str& module_path, const py::str& work) const { return win32::prepare(module_path, work); }

reconstruct::Built Win32Kind::build(const reconstruct::Prepared& m, const py::str& module_path, const py::str& work) const {
    const Built32 b = win32::build(m, module_path, work);
    reconstruct::Built out;
    out.context = std::make_shared<py::str>(b.module_path);
    for (const auto& kv : b.items.items()) {
        out.items.set(std::get<0>(kv), std::make_shared<Item>(std::get<1>(kv)));
    }
    out.dropped = b.dropped;
    return out;
}

reconstruct::Verdict Win32Kind::judge(const reconstruct::Context& context, const reconstruct::Item& item) const {
    return win32::judge(*std::static_pointer_cast<py::str>(context), *std::static_pointer_cast<Item>(item));
}

std::optional<std::string> Win32Kind::encode_context(const reconstruct::Context& context) const {
    return jobs::workers::Enc().s(std::static_pointer_cast<py::str>(context)->utf8()).text();
}
reconstruct::Context Win32Kind::decode_context(const std::string& text) const {
    jobs::workers::Dec d(text);
    return std::make_shared<py::str>(py::str(d.s()));
}
std::string Win32Kind::encode_item(const reconstruct::Item& item) const {
    const Item& it = *std::static_pointer_cast<Item>(item);
    return jobs::workers::Enc().s(it.key.utf8()).i(it.va).s(it.dll.utf8()).i(it.entry).i(it.regs).text();
}
reconstruct::Item Win32Kind::decode_item(const std::string& text) const {
    jobs::workers::Dec d(text);
    auto it = std::make_shared<Item>();
    it->key = py::str(d.s());
    it->va = d.i();
    it->dll = py::str(d.s());
    it->entry = d.i();
    it->regs = d.i();
    return it;
}

const std::shared_ptr<reconstruct::Kind>& kind() {
    static std::shared_ptr<reconstruct::Kind> k = std::make_shared<Win32Kind>();
    return k;
}

void register_lane() { reconstruct::register_kind(KIND, kind()); }

}  // namespace codebreaker::verify::lanes::win32
