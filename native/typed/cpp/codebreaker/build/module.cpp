// codebreaker/build/module.cpp -- typed C++ port of codebreaker.build.module.
//
// Build a whole module from its reconstruction, and check what came out:
//
//     auto b = module::build("lzexpand.dll");        // -> Built; b.image is the linked file
//     py::print(b.report());
//
// The per-function builds (build/functions) make a throw-away DLL to judge functions in; this makes the module: the
// same decompiled functions (functions::compile_all), plus the module's data as C (build/data), its resources compiled
// from `.rc` (build/resources) and its exports and imports (build/definitions), linked by OpenWatcom into a 16-bit NE or
// 32-bit PE image. Then the image is checked against the original (format, exports, imports, resources, runs).
//
// Notes on the port (where it is not the Python's, and why):
//   * The engine's own build path (engine/win16.py, engine/lanes.py: `engine.enabled()`) is not here: module::build()
//     is the direct path the Python keeps for CB_ENGINE=0 (engine::lanes calls this module, not the other way round).
//   * The Python's cache stamps made from the SOURCE of modules (`inspect.getsource`, `open(asm16.__file__)`) are
//     constants here (kStamp and the ports' own module_file()s): a change to a stamped module's behaviour bumps its
//     stamp. The assembled-piece cache ("assembled16") holds this port's own encoding (why, the instructions kept as
//     text), not the Python's pickle: the stamp is part of its key, so the two never meet.
//   * subprocess.run(timeout=...) is functions::run_process, which has none.
//   * `Built::_publics` is {function: (symbol, object path)}; the 16-bit frame code reads `public[name][0]` only.
//   * build_many() builds the modules on threads (the Python spawned a process each); the tools' environment is shared
//     through functions::EnvScope's gate.
//   * An L2 document (translate::ir::Dict) is written as JSON by put_val() below (json.dump's separators and order).

#include "codebreaker/build/module.hpp"

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <map>
#include <mutex>
#include <set>
#include <string>
#include <system_error>
#include <vector>

#include "codebreaker/analyze/origin.hpp"
#include "codebreaker/base/cache.hpp"
#include "codebreaker/base/converted.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/base/parallel.hpp"
#include "codebreaker/build/asm16.hpp"
#include "codebreaker/build/functions.hpp"
#include "codebreaker/build/msc6.hpp"
#include "codebreaker/build/resources.hpp"
#include "codebreaker/build/watcom.hpp"
#include "codebreaker/decompile/describe.hpp"
#include "codebreaker/emulate/emulator.hpp"
#include "codebreaker/emulate/pcode.hpp"
#include "codebreaker/emulate/windows16.hpp"
#include "codebreaker/formats/detect.hpp"
#include "codebreaker/formats/ne.hpp"
#include "codebreaker/formats/omf.hpp"
#include "codebreaker/formats/pe.hpp"
#include "codebreaker/formats/resources.hpp"
#include "codebreaker/jobs/pool.hpp"
#include "codebreaker/platforms/win16.hpp"
#include "codebreaker/source/assembly.hpp"
#include "codebreaker/symbols/api_arguments.hpp"
#include "codebreaker/translate/native_l2.hpp"
#include "codebreaker/verify/gate16/contract16.hpp"
#include "codebreaker/verify/gate16/isolated16.hpp"
#include "codebreaker/verify/lane_common.hpp"
#include "codebreaker/verify/lanes/win16.hpp"
#include "codebreaker/verify/lanes/win32.hpp"
#include "codebreaker/verify/reconstruct.hpp"
#include "codebreaker/verify/verdicts.hpp"
#include "py_json.hpp"

namespace codebreaker::build::module {

namespace fs = std::filesystem;
namespace desc = codebreaker::decompile::describe;
namespace ct = codebreaker::decompile::c_text;
using core16::SegOff;
using formats::ne::NEFile;
using formats::ne::Segment;
using Env = py::dict<py::str, py::str>;
using Place = std::tuple<int64_t, int64_t>;   // (segment, offset)
using Which = asm16::Which;                   // an import's ordinal or name
using ImportMap = py::dict<py::str, std::tuple<py::str, Which>>;

const py::re::Pattern _BY_ADDRESS = py::re::compile("\\bfunc_0x([0-9a-f]{8})\\b");

// where kept_as_asm.txt is (beside module.py)
const py::str KEPT_AS_ASM = py::os::path::join(py::settings::checkout_root(), py::str("codebreaker"), py::str("build"), py::str("kept_as_asm.txt"));

namespace {

// the stamp the module's own code puts in the keys of what it caches (see the notes above)
const char* const kStamp = "codebreaker.build.module typed port 1";

// ---- small helpers ------------------------------------------------------------------------------------------------
bool str_less(const py::str& a, const py::str& b) { return a.u32() < b.u32(); }

template <class T>
py::list<T> cat(const py::list<T>& a, const py::list<T>& b) {
    py::list<T> out = a.copy();
    for (const T& x : b) {
        out.append(x);
    }
    return out;
}

template <class D, class K>
auto opt_get(const D& d, const K& k) -> std::optional<std::decay_t<decltype(d.at(k))>> {
    if (!d.contains(k)) {
        return std::nullopt;
    }
    return d.at(k);
}

bool is_in(const py::str& s, std::initializer_list<const char*> words) {
    const std::string t = s.utf8();
    for (const char* w : words) {
        if (t == w) {
            return true;
        }
    }
    return false;
}

// s[-n:]
py::str tail(const py::str& s, int64_t n) { return s.slice(-n, py::none); }

// s.strip()[-n:]
py::str strip_tail(const py::str& s, int64_t n) { return tail(s.strip(), n); }

// the text of what follows the first "{" (`text.split("{", 1)[-1]`)
py::str after_brace(const py::str& text) {
    const py::list<py::str> parts = py::split(text, py::str("{"), 1);
    return parts[-1];
}

// bytes([int ...]) of a file; an OSError for a file that cannot be read
py::bytes read_bytes(const py::str& path) { return py::file::open(path, py::str("rb")).read(); }

// open(path, encoding="utf-8", errors="replace").read()
py::str read_text(const py::str& path) { return read_bytes(path).decode("utf-8", "replace"); }

// open(path, "w", newline="\n") written as UTF-8
void write_text(const py::str& path, const py::str& text) {
    py::file f = py::file::open(path, py::str("w"), py::str("utf-8"), std::nullopt, py::str("\n"));
    f.write(text);
}

void write_bytes(const py::str& path, const py::bytes& data) {
    py::file f = py::file::open(path, py::str("wb"));
    f.write(data);
}

int64_t file_size(const py::str& path) {
    std::error_code ec;
    const auto n = fs::file_size(fs::u8path(path.utf8()), ec);
    if (ec) {
        throw py::FileNotFoundError("[WinError 2] The system cannot find the file specified: " + py::repr(path).utf8());
    }
    return static_cast<int64_t>(n);
}

uint64_t u_at(const py::bytes& b, int64_t off, int n) {
    if (off < 0 || off + n > b.size()) {
        throw py::ValueError("unpack_from requires a buffer of at least " + std::to_string(off + n) + " bytes");
    }
    uint64_t v = 0;
    for (int i = n - 1; i >= 0; --i) {
        v = (v << 8) | static_cast<unsigned char>(b.raw()[static_cast<size_t>(off + i)]);
    }
    return v;
}

void emit(const Log& log, const py::str& text) {
    if (log) {
        log(text);
    } else {
        py::print(text);
    }
}

bool env_set(const char* name) {
    const std::optional<py::str> v = py::os::getenv(py::str(name));
    return v && !v->empty();
}

py::str env_get(const char* name, const char* dflt = "") {
    const std::optional<py::str> v = py::os::getenv(py::str(name));
    return v ? *v : py::str(dflt);
}

// settings.state(*parts): a path inside the state folder (its parent made)
py::str state_path(const py::str& a, const py::str& b) {
    const py::str path = py::os::path::join(py::settings::get_str(py::str("state")), a, b);
    py::os::makedirs(py::os::path::dirname(path), 0777, true);
    return path;
}

// the sorted() of strings as Python orders them
py::list<py::str> sorted_strs(const std::vector<py::str>& xs) {
    std::vector<py::str> v = xs;
    std::sort(v.begin(), v.end(), str_less);
    py::list<py::str> out;
    for (const py::str& s : v) {
        out.append(s);
    }
    return out;
}

py::list<py::str> sorted_set(const py::set<py::str>& s) {
    std::vector<py::str> v(s.begin(), s.end());
    return sorted_strs(v);
}

// the JSON of an L2 value as json.dump writes it (default separators, str keys in insertion order)
void put_val(std::string& o, const translate::ir::Val& v) {
    using translate::ir::Val;
    if (std::holds_alternative<std::monostate>(v.v)) {
        o += "null";
    } else if (std::holds_alternative<bool>(v.v)) {
        o += std::get<bool>(v.v) ? "true" : "false";
    } else if (std::holds_alternative<int64_t>(v.v)) {
        o += std::to_string(std::get<int64_t>(v.v));
    } else if (std::holds_alternative<double>(v.v)) {
        o += py::to_str(std::get<double>(v.v)).utf8();
    } else if (std::holds_alternative<py::str>(v.v)) {
        py::json::put(o, std::get<py::str>(v.v));
    } else if (std::holds_alternative<Val::List>(v.v)) {
        o += "[";
        bool first = true;
        for (const Val& x : std::get<Val::List>(v.v)) {
            if (!first) {
                o += ", ";
            }
            first = false;
            put_val(o, x);
        }
        o += "]";
    } else if (std::holds_alternative<Val::Dict>(v.v)) {
        o += "{";
        bool first = true;
        for (const auto& kv : std::get<Val::Dict>(v.v).items()) {
            if (!first) {
                o += ", ";
            }
            first = false;
            py::json::put(o, std::get<0>(kv));
            o += ": ";
            put_val(o, std::get<1>(kv));
        }
        o += "}";
    } else {
        throw py::TypeError("Object of type Type is not JSON serializable");
    }
}

void write_json(const py::str& path, const translate::ir::Dict& doc) {
    std::string o;
    put_val(o, translate::ir::Val(doc));
    write_text(path, py::str(o));
}

py::str quote_symbol(const py::str& symbol) { return py::str("'") + symbol + py::str("'"); }

using Cs = std::vector<base::cache::Part>;

}  // namespace

// ---- Built ----------------------------------------------------------------------------------------------------------
Built::Built(const py::str& original_) {
    original = original_;
    name = py::os::path::basename(original_).lower();
    kind = base::corpus::lane(original_);
}

bool Built::linked() const { return image && !image->empty(); }

// A build and what was found out about it.
py::str Built::report() const {
    if (!linked()) {
        return py::percent("%s: NOT BUILT: %s", name, error);
    }
    const int64_t size = file_size(*image), was = file_size(original);
    py::list<py::str> out;
    out.append(py::percent("%s: linked, %d bytes (the original is %d: %.0f%%)", name, size, was, 100.0 * static_cast<double>(size) / static_cast<double>(std::max<int64_t>(1, was))));
    out.append(py::percent("  functions   %d in the module, %d with reconstructed C, %d compiled, %d rejected by the "
                           "compiler (stand-ins)",
                           functions, with_text, with_text - py::len(rejected), py::len(rejected)));
    out.append(py::percent("              %d called but not reconstructed (stand-ins), %d left unresolved", py::len(absent), py::len(undefined)));
    if (!data_error.empty()) {
        out.append(py::percent("  data        NOT CARRIED: %s", data_error));
    } else if (data) {
        out.append(py::percent("  data        %d bytes as C, %d pointers written as pointers, %d left as numbers, "
                               "%d addresses used by the code not placed",
                               data->size, data->pointers, py::len(data->unresolved), lost_addresses));
    }
    if (!code_data.empty()) {
        py::list<py::str> parts;
        for (const auto& sn : code_data) {
            const int64_t sg = std::get<0>(sn);
            const py::list<py::str>& ns = std::get<1>(sn);
            parts.append(py::percent("%d (for %s)", sg, py::join(py::str(", "), ns.slice(0, 4)) + (py::len(ns) > 4 ? py::str(" ...") : py::str(""))));
        }
        out.append(py::str("  code data   the original's bytes the C reads through CS, in its segment: ") + py::join(py::str(", "), parts));
    }
    if (!disputes.empty()) {
        py::list<py::str> parts;
        for (const auto& nw : disputes.slice(0, 12)) {
            parts.append(py::percent("%s (callers: %s)", std::get<0>(nw), std::get<1>(nw)));
        }
        out.append(py::percent("  conventions %d functions whose callers contradict how they are described (who removes "
                               "the arguments): %s",
                               py::len(disputes), py::join(py::str(", "), parts)));
    }
    if (contract_error && !contract_error->empty()) {
        out.append(py::percent("  contracts   NOT CHECKED: %s", *contract_error));
    }
    if (gate_error && !gate_error->empty()) {
        out.append(py::percent("  gate        NOT CHECKED for %s", *gate_error));
    }
    if (!gated.empty()) {
        std::vector<std::pair<py::str, py::str>> items;
        for (const auto& kv : gated.items()) {
            items.push_back({std::get<0>(kv), std::get<1>(kv)});
        }
        std::sort(items.begin(), items.end(), [](const auto& a, const auto& b) { return str_less(a.first, b.first); });
        py::str lines("");
        for (size_t i = 0; i < items.size() && i < 12; ++i) {
            lines = lines + py::percent("\n              %s: %s", items[i].first, items[i].second.slice(0, 140));
        }
        out.append(py::percent("  gated       %d functions' C differs from the original (rebuilt from their instructions)%s", py::len(gated), lines));
    }
    if (!hidden.empty()) {
        py::list<py::str> parts;
        for (const auto& h : hidden.slice(0, 16)) {
            parts.append(py::percent("%d:%04X", std::get<0>(h), std::get<1>(h)));
        }
        out.append(py::percent("  HIDDEN      %d functions begin inside another (an entry sequence after its end) and "
                               "are not described: %s",
                               py::len(hidden), py::join(py::str(" "), parts)));
    }
    out.append(py::percent("  entry       %s", entry));
    out.append(py::percent("  format      %s", format_same ? py::str("same as the original") : py::str("DIFFERENT from the original")));
    {
        py::str missing("");
        for (const auto& em : exports_missing.slice(0, 12)) {
            const build::definitions::Export& e = std::get<0>(em);
            missing = missing + py::percent("\n              missing: %s @%d -- %s", e.name && !e.name->empty() ? *e.name : py::str("(no name)"), e.ordinal, std::get<1>(em));
        }
        out.append(py::percent("  exports     %d of %d with the original's ordinal and name (%d of them point at stand-ins)%s", exports_same,
                               exports_total, exports_on_stand_ins, missing));
    }
    {
        py::str all;
        if (imports_extra.empty()) {
            all = py::str("all are the original's");
        } else {
            py::list<py::str> parts;
            for (const auto& i : imports_extra.slice(0, 8)) {
                parts.append(py::percent("%s.%s", std::get<0>(i), std::get<1>(i)));
            }
            all = py::str("NOT in the original: ") + py::join(py::str(", "), parts);
        }
        out.append(py::percent("  imports     %d (the original has %d); %s", imports_built, imports_original, all));
    }
    out.append(py::percent("  resources   %s", !resources_line.empty() ? resources_line : py::str("the original has none")));
    if (!ran.empty()) {
        int64_t ok = 0, same = 0;
        for (const auto& r : ran) {
            ok += std::get<1>(r) == py::str("ret") ? 1 : 0;
            same += std::get<3>(r) ? 1 : 0;
        }
        out.append(py::percent("  runs        %d exported functions called with zero arguments: %d returned, %d did what "
                               "the original does (same end, same result)",
                               py::len(ran), ok, same));
    }
    return py::join(py::str("\n"), out);
}

namespace {

// ---- the linker ---------------------------------------------------------------------------------------------------
// Run the linker on `lines`. -> (path of m.dll or None, everything it said)
std::tuple<std::optional<py::str>, py::str> _link(const py::str& tool, const Env& env, const py::str& folder, const py::list<py::str>& lines) {
    write_text(py::os::path::join(folder, py::str("m.lnk")), py::join(py::str("\n"), lines) + py::str("\n"));
    for (const char* stale : {"m.dll", "m.map"}) {
        try {
            py::os::remove(py::os::path::join(folder, py::str(stale)));
        } catch (const py::OSError&) {
        }
    }
    py::list<py::str> argv;
    argv.append(tool);
    argv.append(py::str("@m.lnk"));
    const auto r = build::functions::run_process(argv, folder, env);
    const py::str out = py::os::path::join(folder, py::str("m.dll"));
    return {py::os::path::isfile(out) ? std::optional<py::str>(out) : std::nullopt, std::get<1>(r) + std::get<2>(r)};
}

// Where a function's instructions put a value in AX that it returns with: walking back from each return through its
// straight run of instructions, the first one writing AX (or a call: what the callee left there goes back to the caller as
// it came) -- one past the prologue's (`mov ax, ds` / `push ds; pop ax` / `mov ax, DGROUP` ahead of `mov ds, ax`), and not
// a `pop ax` (its own AX given back). -> offset, or None.
std::optional<int64_t> _result_in_ax(const py::bytes& code, int64_t start, int64_t end) {
    const auto md = desc::decoder16(true);
    const py::list<desc::Insn> ins = md->disasm(code.slice(start, end), start);
    int64_t prologue = start;
    for (int64_t i = 0; i < std::min<int64_t>(6, py::len(ins)); ++i) {
        if (ins[i].mn == "mov" && ins[i].os == "ds, ax") {
            prologue = static_cast<int64_t>(ins[i].address) + ins[i].size;
            break;
        }
    }
    std::set<int64_t> targets;
    for (const desc::Insn& i : ins) {
        if (i.group(desc::CS_GRP_JUMP) && !i.operands.empty() && i.operands[0].type == decompile::inline_asm::X86_OP_IMM) {
            targets.insert(i.operands[0].imm);
        }
    }
    const std::set<int64_t> ax = {1 /*AH*/, 2 /*AL*/, 3 /*AX*/, 19 /*EAX*/};
    for (int64_t k = 0; k < py::len(ins); ++k) {
        if (ins[k].mn != "ret" && ins[k].mn != "retf") {
            continue;
        }
        for (int64_t j = k - 1; j >= 0; --j) {
            const desc::Insn& p = ins[j];
            const int64_t addr = static_cast<int64_t>(p.address);
            if (p.group(desc::CS_GRP_CALL) && addr >= prologue) {
                return addr;                                     // the callee's result, passed on as it came
            }
            if (addr < prologue || p.mn == "jmp" || p.mn == "ret" || p.mn == "retf") {
                break;
            }
            const auto rw = p.regs_access();
            bool writes_ax = false;
            for (int64_t r : std::get<1>(rw)) {
                if (ax.count(r)) {
                    writes_ax = true;
                }
            }
            if (writes_ax) {
                if (p.mn != "pop") {
                    return addr;
                }
                break;
            }
            if (targets.count(addr)) {
                break;
            }
        }
    }
    return std::nullopt;
}

// Whether every fix-up the original makes to a place of its own is by the segment's number -- none through the entry
// table (a movable segment's), none a far pointer.
bool _segment_number_fixups(const NEFile& nef) {
    const py::bytes& blob = nef.data;
    bool seen = false;
    for (const Segment& s : nef.segments()) {
        if (!(s.flags & 0x100) || !s.file_off) {
            continue;
        }
        const int64_t at = s.file_off + (s.length ? s.length : 0x10000);
        const int64_t count = static_cast<int64_t>(u_at(blob, at, 2));
        for (int64_t k = 0; k < count; ++k) {
            const int64_t base = at + 2 + k * 8;
            const int64_t src = static_cast<int64_t>(u_at(blob, base, 1)), flags = static_cast<int64_t>(u_at(blob, base + 1, 1));
            const int64_t target = static_cast<int64_t>(u_at(blob, base + 6, 2));
            if ((flags & 3) == 0) {                              // a place of the module's own
                seen = true;
                if ((target & 0xFF) == 0xFF || src != 2) {
                    return false;
                }
            }
        }
    }
    return seen;
}

// The original's DOS part (the bytes before its NE header), as a stub for the linker: its MZ header made to say just
// that much. -> the file's name, or None.
std::optional<py::str> _dos_stub(const py::str& path, const py::str& folder) {
    py::file f = py::file::open(path, py::str("rb"));
    const std::optional<formats::detect::DosHeader> h = formats::detect::dos_header(f.read(0x40));
    if (!h) {
        return std::nullopt;
    }
    f.seek(0);
    py::bytes stub0 = f.read(h->new_header);
    if (stub0.size() < 0x40) {
        return std::nullopt;
    }
    std::string stub = stub0.raw();
    const int64_t n = static_cast<int64_t>(stub.size());
    const uint16_t last = static_cast<uint16_t>(n % 512), pages = static_cast<uint16_t>((n + 511) / 512);
    stub[2] = static_cast<char>(last & 0xFF);
    stub[3] = static_cast<char>(last >> 8);
    stub[4] = static_cast<char>(pages & 0xFF);
    stub[5] = static_cast<char>(pages >> 8);
    write_bytes(py::os::path::join(folder, py::str("stub.exe")), py::bytes(stub));
    return py::str("stub.exe");
}

// Link; give whatever is undefined a stand-in and link again; whatever is still undefined after that is left unresolved
// (and reported). `stand_in(symbol)` -> C text or None; `compile_extra(stem, text)` -> "" or why it does not compile.
std::optional<py::str> _link_resolving(Built& b, const py::str& tool, const Env& env, const py::str& folder, const py::list<py::str>& lines0,
                                       const std::function<std::optional<py::str>(const py::str&)>& stand_in,
                                       const std::function<py::str(const py::str&, const py::str&)>& compile_extra) {
    static const py::re::Pattern undefined = py::re::compile("(\\S+) is an undefined reference");
    py::list<py::str> lines = lines0;
    auto [image, log] = _link(tool, env, folder, lines);
    std::vector<py::str> found;
    {
        std::set<std::string> seen;
        for (const py::str& s : undefined.findall(log)) {
            if (seen.insert(s.utf8()).second) {
                found.push_back(s);
            }
        }
    }
    py::list<py::str> names = sorted_strs(found);
    if (!names.empty()) {
        // `__imp__Name@N` is asked for by the compiler's own runtime library, pulled in by a library routine the functions
        // use: it needs those system functions for itself. They come from the import libraries, and show up in the check as
        // imports the original does not have.
        bool imp = false;
        for (const py::str& s : names) {
            if (s.startswith(py::str("__imp_"))) {
                imp = true;
            }
        }
        if (imp) {
            lines = cat(lines, py::list<py::str>{py::str("library kernel32.lib, user32.lib, gdi32.lib, advapi32.lib")});
            py::list<py::str> kept;
            for (const py::str& s : names) {
                if (!s.startswith(py::str("__imp_"))) {
                    kept.append(s);
                }
            }
            names = kept;
        }
        std::vector<std::pair<py::str, py::str>> texts;
        for (const py::str& s : names) {
            const std::optional<py::str> t = stand_in(s);
            if (t && !t->empty()) {
                texts.push_back({s, *t});
            }
        }
        py::list<py::str> objects;
        py::list<py::str> joined_texts;
        for (const auto& st : texts) {
            joined_texts.append(st.second);
        }
        if (!texts.empty() && compile_extra(py::str("absent"), py::join(py::str("\n"), joined_texts)).empty()) {
            objects = py::list<py::str>{py::str("absent.obj")};
            b.absent = py::list<py::str>();
            for (const auto& st : texts) {
                b.absent.append(st.first);
            }
        } else {                                                 // one of them does not compile: each on its own
            for (size_t i = 0; i < texts.size(); ++i) {
                const py::str stem = py::percent("absent%03d", static_cast<int64_t>(i));
                if (compile_extra(stem, texts[i].second).empty()) {
                    objects.append(stem + py::str(".obj"));
                    b.absent.append(texts[i].first);
                }
            }
        }
        py::list<py::str> plain, libs;
        for (const py::str& ln : lines) {
            (ln.startswith(py::str("library ")) ? libs : plain).append(ln);
        }
        py::list<py::str> files;
        for (const py::str& o : objects) {
            files.append(py::str("file ") + o);
        }
        lines = cat(cat(plain, files), libs);
        std::tie(image, log) = _link(tool, env, folder, lines);
        std::vector<py::str> still;
        {
            std::set<std::string> seen;
            for (const py::str& s : undefined.findall(log)) {
                if (seen.insert(s.utf8()).second) {
                    still.push_back(s);
                }
            }
        }
        b.undefined = sorted_strs(still);
        if (!b.undefined.empty()) {
            std::tie(image, log) = _link(tool, env, folder, cat(lines, py::list<py::str>{py::str("option undefsok")}));
        }
    }
    if (!image) {
        const py::str said = strip_tail(log, 400);
        b.error = py::str("link: ") + (!said.empty() ? said : py::str("the linker produced nothing"));
    }
    return image;
}

// ---- assembly in a 16-bit module -------------------------------------------------------------------------------------
// A piece of assembly (asm16::Function) and the instructions kept as text for it (Python's `f.good`).
struct Fn {
    std::shared_ptr<asm16::Function> f;
    py::set<int64_t> good;
};

// Assemble `text` as `stem`.asm into `stem`.obj -- what the assembler made of this text with these options is kept
// (base/cache.py): a piece the same as in an earlier build, or earlier in this one, is not assembled again.
// -> (0 or the assembler's exit code, its messages)
std::tuple<int64_t, py::str> _wasm(const py::str& wasm, const Env& env, const py::str& folder, const py::str& stem, const py::str& text,
                                   const py::list<py::str>& flags = py::list<py::str>{py::str("-q"), py::str("-zcm")}) {
    write_text(py::os::path::join(folder, stem + py::str(".asm")), text);
    const py::str obj = py::os::path::join(folder, stem + py::str(".obj"));
    // (every input of the object, in the shared build cache: the assembler by its bytes, the INCLUDE files it may read and
    // the WASM variable of options (functions.tool_inputs), the options, the text, and the stem -- the object names its
    // source file, stem.asm, and the messages of a failure name it too)
    const py::str k = base::cache::key(Cs{py::str("wasm16"), build::functions::tool_inputs(wasm, env, py::str("WASM")), flags, stem, text});
    const std::optional<py::bytes> had = base::cache::get(py::str("wasm16"), k);
    if (had) {
        if (had->slice(0, 2) != py::bytes("OK")) {
            return {1, build::functions::message_here(had->slice_from(2), folder)};
        }
        const std::optional<py::bytes> here = build::functions::omf_here(had->slice_from(2), py::os::path::dirname(obj));   // (as wasm writes it in this folder, now)
        if (here) {
            write_bytes(obj, *here);
            return {0, py::str("")};
        }
    }
    try {
        py::os::remove(obj);
    } catch (const py::OSError&) {
    }
    py::list<py::str> argv;
    argv.append(wasm);
    for (const py::str& f : flags) {
        argv.append(f);
    }
    argv.append(py::str("-fo=") + stem + py::str(".obj"));
    argv.append(stem + py::str(".asm"));
    const auto r = build::functions::run_process(argv, folder, env);
    const py::str msg = std::get<1>(r) + std::get<2>(r);
    if (std::get<0>(r) == 0 && py::os::path::isfile(obj)) {
        base::cache::put(py::str("wasm16"), k, py::bytes(std::string("OK") + read_bytes(obj).raw()));
        return {0, msg};
    }
    // (a failure that says nothing is the assembler stopped from outside -- a build interrupted -- not the text: kept,
    // every later build of the module found that piece "not assembling" and gave up its whole frame, USER's segment 1 with
    // all its C)
    if (!msg.strip().empty()) {
        base::cache::put(py::str("wasm16"), k, py::bytes(std::string("ER") + build::functions::message_kept(msg, folder).raw()));
    }
    return {std::get<0>(r) ? std::get<0>(r) : 1, msg};
}

// An object of `n` bytes of INT 3 in code segment `seg`: a place kept where it was.
py::str _filler(const py::str& folder, const py::str& wasm, const Env& env, int64_t seg, const py::str& stem0, int64_t n) {
    const py::str stem = py::str("asm/") + stem0;
    _wasm(wasm, env, folder, stem,
          py::percent("CODE%d segment byte public 'MCODE'\n        db   %d dup (0CCh)\nCODE%d ends\nend\n", seg, n, seg),
          py::list<py::str>{py::str("-q")});
    return stem;
}

// Record a function as built from assembly (`stem`).
void _assembled_one(Built& b, const py::str& name, py::dict<py::str, py::str>& stems, const py::str& stem) {
    stems.set(name, stem);
    const auto o = opt_get(b.origins, name);
    if (o && *o && (*o)->language == py::str("assembly")) {
        py::list<py::str> ev;
        for (int64_t i = 0; i < std::min<int64_t>(2, py::len((*o)->evidence)); ++i) {
            ev.append((*o)->evidence[i]);
        }
        b.assembled.set(name, py::str("written in assembly: ") + py::join(py::str("; "), ev));
    } else {
        b.assembled.set(name, py::str("its C does not compile: ") + (b.rejected.contains(name) ? b.rejected.at(name) : py::str("")));
    }
    if (b.rejected.contains(name)) {
        b.rejected.pop(name);
    }
}

// the stem of an object from its C stem (module-level in the Python: used by _frame_pieces)

// The functions rebuilt from C into the frame's holes: each in its own hole first, when it fits; the rest packed into
// what room is left (near ones first, then the biggest). `holes`: [(start, end, name)]; `size`: {name: bytes of code};
// `aside`: far ones sent elsewhere first; `limit`: where the segment's resident part ends -- a function from below it goes
// only into a hole that ends below it too (KERNEL keeps only the first A940h bytes of its segment 1).
struct Bin {
    int64_t lo = 0, hi = 0;
    std::vector<std::pair<int64_t, py::str>> objs;   // (pad, name)
    int64_t used = 0;
};
struct Packed {
    std::vector<Bin> bins;
    py::list<py::str> no_room_far, no_room_near;
};
using HoleName = std::optional<py::str>;

Packed _pack_holes(const std::vector<std::tuple<int64_t, int64_t, HoleName>>& holes, const py::dict<py::str, int64_t>& size,
                   const py::set<py::str>& near, const py::set<py::str>& aside, const std::optional<int64_t>& limit) {
    Packed out;
    std::vector<py::str> rest;
    for (const auto& h : holes) {
        const int64_t lo = std::get<0>(h), hi = std::get<1>(h);
        Bin bin;
        bin.lo = lo;
        bin.hi = hi;
        bin.used = lo;
        const HoleName& name = std::get<2>(h);
        if (name && size.contains(*name) && !aside.contains(*name)) {
            const int64_t pad = bin.used & 1;                   // (OpenWatcom aligns its code to a word)
            if (pad + size.at(*name) <= hi - lo) {
                bin.objs.push_back({pad, *name});
                bin.used += pad + size.at(*name);
            } else {
                rest.push_back(*name);
            }
        }
        out.bins.push_back(bin);
    }
    std::map<std::string, int64_t> own;                          // {name: lo of its hole} (the last hole of a name wins)
    for (const auto& h : holes) {
        if (std::get<2>(h)) {
            own[std::get<2>(h)->utf8()] = std::get<0>(h);
        }
    }
    std::stable_sort(rest.begin(), rest.end(), [&](const py::str& a, const py::str& b) {
        const bool na = !near.contains(a), nb = !near.contains(b);               // (near ones first)
        if (na != nb) {
            return na < nb;
        }
        return -size.at(a) < -size.at(b);
    });
    for (const py::str& name : rest) {
        bool placed = false;
        for (Bin& bn : out.bins) {
            const auto it = own.find(name.utf8());
            if (limit && (it == own.end() ? 0 : it->second) < *limit && bn.hi > *limit) {
                continue;
            }
            const int64_t pad = bn.used & 1;
            if (bn.used + pad + size.at(name) <= bn.hi) {
                bn.objs.push_back({pad, name});
                bn.used += pad + size.at(name);
                placed = true;
                break;
            }
        }
        if (!placed) {
            (near.contains(name) ? out.no_room_near : out.no_room_far).append(name);
        }
    }
    return out;
}

// Where a self-loading KERNEL cuts its own code segment after start-up: GlobalReAlloc of CS to `mov cx, size` --
// `mov cx, N / push cs / push dx / push cx / push dx`. -> N or None
std::optional<int64_t> _resident_limit(const py::bytes& code) {
    const std::string& c = code.raw();
    const std::string tailing("\x0e\x52\x51\x52\x0e", 5);
    for (int64_t at = 0; at < static_cast<int64_t>(c.size()) - 8; ++at) {
        if (static_cast<unsigned char>(c[at]) == 0xB9 && c.compare(static_cast<size_t>(at + 3), 5, tailing) == 0) {
            return static_cast<int64_t>(static_cast<unsigned char>(c[at + 1])) | static_cast<int64_t>(static_cast<unsigned char>(c[at + 2])) << 8;
        }
    }
    return std::nullopt;
}

using StemOf = std::function<std::optional<py::str>(const py::str&)>;   // elsewhere(name) -> stem or None
using PieceAssembler = std::function<py::str(const py::str&, Fn&, int64_t, int64_t)>;

// The frame of a segment with assembly in it, in the order the linker is to lay it out: its pieces at the original's
// places, and in the places of the functions rebuilt from C (the frame's holes) those functions -- each in its own place
// when it fits there, and the ones that do not, packed into what room the holes have left (biggest first; a near call
// reaches anywhere in the segment), filler for the rest. A far one with no room goes to the segment beside (CODE<n>X), a
// near one after the frame. -> [object stem] or None.
std::optional<py::list<py::str>> _frame_pieces(Built& b, Fn& fn, int64_t seg, py::dict<py::str, py::str>& stems, const py::str& folder,
                                               const py::str& wasm, const Env& env, const PieceAssembler& assemble_piece, const StemOf& elsewhere) {
    asm16::Function& f = *fn.f;
    py::dict<py::str, py::str> name_of;                          // {symbol: name}
    for (const auto& kv : b._publics.items()) {
        name_of.set(std::get<0>(std::get<1>(kv)), std::get<0>(kv));
    }
    auto name_for = [&](const py::str& sym) -> HoleName { return opt_get(name_of, sym); };
    py::dict<py::str, int64_t> size;
    py::dict<py::str, py::str> cstem;
    for (const auto& h : f.holes) {
        const HoleName name = name_for(std::get<2>(h));
        std::optional<py::str> st;
        if (name) {
            st = opt_get(stems, *name);
        }
        if (st && !st->empty() && !st->startswith(py::str("asm/"))) {
            try {
                const msc6::Code code = msc6::object_code(read_bytes(py::os::path::join(folder, *st + py::str(".obj"))));
                size.set(*name, std::get<0>(code).size());
                cstem.set(*name, *st);
            } catch (const py::OSError&) {
            }
        }
    }
    const py::set<py::str> near = b.near_names;
    std::optional<int64_t> limit = opt_get(b.resident_limit, seg);

    std::vector<std::tuple<int64_t, int64_t, HoleName>> holes;
    for (const auto& h : f.holes) {
        holes.push_back({std::get<0>(h), std::get<1>(h), name_for(std::get<2>(h))});
    }
    auto place = [&](const py::set<py::str>& aside) { return _pack_holes(holes, size, near, aside, limit); };

    // A near function with no room goes after the frame and makes the segment bigger than the original's -- past 64K, in the
    // end. A far one can go to the segment beside instead: the far ones give up their places, largest first, while near ones
    // are still left over.
    py::set<py::str> aside;
    Packed packed = place(aside);
    // (no longer done: every function stays in its own segment -- a segment the original does not have renumbers the
    // selectors of all that loads after it. The link moves far ones out of a segment only past 64K.)
    while (!packed.no_room_near.empty() && elsewhere && env_set("CB_ASIDE")) {
        std::vector<py::str> placed_far;
        for (const auto& kv : size.items()) {
            const py::str& n = std::get<0>(kv);
            if (!near.contains(n) && !aside.contains(n) && !packed.no_room_far.contains(n)) {
                placed_far.push_back(n);
            }
        }
        std::stable_sort(placed_far.begin(), placed_far.end(), [&](const py::str& x, const py::str& y) { return -size.at(x) < -size.at(y); });
        if (placed_far.empty()) {
            break;
        }
        aside.add(placed_far[0]);
        packed = place(aside);
    }
    for (const auto& h : f.holes) {
        const HoleName name = name_for(std::get<2>(h));
        b.frame_fit.append({name ? *name : py::str("None"), std::get<1>(h) - std::get<0>(h), name && size.contains(*name) ? size.at(*name) : 0});
    }
    py::list<py::str> after;
    {
        py::list<py::str> order = sorted_set(aside);
        order = cat(cat(order, packed.no_room_far), packed.no_room_near);
        for (const py::str& name : order) {
            // (only those that gave up their places for near ones go to the segment beside; a far one that just did not fit its
            // own place goes after the frame, as a near one does: a segment the original does not have renumbers the selectors
            // of all that loads after it -- USER's sub_39_05AC, sent to a new segment 46, broke Setup with sub_7_03F8 rebuilt
            // from C as well, though it never ran. The link moves far ones out only past 64K.)
            std::optional<py::str> moved;
            if (elsewhere && aside.contains(name)) {
                moved = elsewhere(name);
            }
            if (moved && !moved->empty()) {                       // far: into the segment beside this one
                stems.set(name, *moved);
                b.moved_c.add(name);
            } else {
                after.append(cstem.at(name));                     // near: after the frame, in this segment
                std::optional<int64_t> own;
                for (const auto& h : f.holes) {
                    const HoleName hn = name_for(std::get<2>(h));
                    if (hn && *hn == name) {
                        own = std::get<0>(h);
                        break;
                    }
                }
                if (limit && own && *own < *limit) {
                    // (past where the segment is cut after start-up: the build is done again with it as its instructions -- see
                    // _resident_limit)
                    b.resident_overflow.set(name, py::percent("its C finds no room in segment %d below %04x (where it is "
                                                              "cut, or a fixed segment's end)",
                                                              seg, *limit));
                }
            }
        }
    }
    for (const auto& kv : size.items()) {
        if (!b.moved_c.contains(std::get<0>(kv))) {
            b.placed_c.add(std::get<0>(kv));
        }
    }
    py::list<py::str> out;
    int64_t at = f.start;
    std::vector<Bin> all = packed.bins;
    {
        Bin last;
        last.lo = f.end;
        last.hi = f.end;
        last.used = f.end;
        all.push_back(last);
    }
    for (size_t k = 0; k < all.size(); ++k) {
        const Bin& bin = all[k];
        if (at < bin.lo) {
            const py::str stem = py::percent("asm/frame%02d_%02d", seg, static_cast<int64_t>(k));
            const py::str why = assemble_piece(stem, fn, at, bin.lo);
            if (!why.empty()) {
                b.frame_why = py::percent("%s (%04x-%04x): %s", stem, at, bin.lo, why);
                return std::nullopt;
            }
            out.append(stem);
        }
        if (bin.lo == bin.hi) {
            break;
        }
        int64_t pos = bin.lo;
        for (const auto& po : bin.objs) {
            const int64_t pad = po.first;
            const py::str& name = po.second;
            if (pad) {
                out.append(_filler(folder, wasm, env, seg, py::percent("pad%02d_%04x", seg, pos), pad));
            }
            out.append(cstem.at(name));
            pos = pos + pad + size.at(name);
        }
        if (bin.hi > bin.used) {
            out.append(_filler(folder, wasm, env, seg, py::percent("fill%02d_%04x", seg, bin.used), bin.hi - bin.used));
        }
        at = bin.hi;
    }
    return cat(out, after);
}

// The functions whose addresses sit in the automatic data segment as near pointers -- a table of handlers the code calls
// through (`mov di, 776h` / `call word ptr [di]`): SETUPX's CONFIG.SYS and AUTOEXEC.BAT line handlers, DS:0776, ten offsets
// in segment 3. A near pointer has no relocation, so nothing tells the build it is code: a handler rebuilt from C that moves
// leaves the table pointing at whatever now sits there -- sub_3_36DE moved to 2A3C, TPWRITEPROFILESTRING went into its place,
// and the table's second entry ran it (a far return from a near call: a fault in SETUPX at 0003:3700 as Setup began to copy).
// A run of three or more words in a row that are all function starts of one code segment, or two that the segment's code
// loads as the table's address, and that segment calls or jumps through a word in memory.
py::set<py::str> _near_pointer_tables(const NEFile& nef, const py::dict<Place, py::str>& at) {
    const auto md = desc::decoder16(false);
    py::dict<int64_t, Segment> segs;
    for (const Segment& s : nef.segments()) {
        segs.set(s.idx, s);
    }
    const int64_t auto_ = nef.h.autodata_seg;
    if (!auto_ || !segs.contains(auto_) || !segs.at(auto_).file_off) {
        return py::set<py::str>();
    }
    const Segment& ds = segs.at(auto_);
    const py::bytes data = nef.data.slice(ds.file_off, ds.file_off + ds.length);
    std::map<int64_t, std::map<int64_t, py::str>> starts;       // segment -> {offset: name}
    for (const auto& kv : at.items()) {
        starts[std::get<0>(std::get<0>(kv))][std::get<1>(std::get<0>(kv))] = std::get<1>(kv);
    }
    py::set<py::str> out;
    for (const auto& sn : starts) {
        const int64_t sg = sn.first;
        const std::map<int64_t, py::str>& names = sn.second;
        const auto sit = segs.get(sg);
        if (!sit || !sit->file_off || names.size() < 2) {
            continue;
        }
        const py::bytes code = nef.data.slice(sit->file_off, sit->file_off + sit->length);
        bool indirect = false;
        std::set<int64_t> immediates;
        for (const desc::Insn& i : md->disasm(code, 0)) {
            if ((i.mn == "call" || i.mn == "jmp") && i.os.compare(0, 10, "word ptr [") == 0) {
                indirect = true;
            }
            if (i.mn == "mov") {
                // i.op_str.count(", 0x") == 1 and i.op_str[:2] in ("bx", "si", "di", "ax", "cx", "dx")
                size_t count = 0;
                for (size_t p = i.os.find(", 0x"); p != std::string::npos; p = i.os.find(", 0x", p + 1)) {
                    ++count;
                }
                const std::string head = i.os.substr(0, 2);
                if (count == 1 && (head == "bx" || head == "si" || head == "di" || head == "ax" || head == "cx" || head == "dx")) {
                    immediates.insert(py::int_parse(py::split(py::str(i.os), py::str(", "))[1], 16));
                }
            }
        }
        if (!indirect) {
            continue;
        }
        for (int64_t align : {0, 1}) {
            std::vector<std::pair<int64_t, py::str>> run;
            for (int64_t p = align; p < data.size() - 1; p += 2) {
                const int64_t w = static_cast<int64_t>(u_at(data, p, 2));
                if (names.count(w) && w) {
                    run.push_back({p, names.at(w)});
                    continue;
                }
                if (run.size() >= 3 || (run.size() >= 2 && immediates.count(run[0].first))) {
                    for (const auto& r : run) {
                        out.add(r.second);
                    }
                }
                run.clear();
            }
            if (run.size() >= 3 || (run.size() >= 2 && !run.empty() && immediates.count(run[0].first))) {
                for (const auto& r : run) {
                    out.add(r.second);
                }
            }
        }
    }
    return out;
}

// The functions whose original loads AX from its frame just before a far call through a pointer in its frame -- `mov ax,
// [bp-8]` / `call dword ptr [bp-6]`: a VxD's protected-mode API entry (INT 2Fh AX=1684h), the service number in AX and the
// arguments read from the caller's frame as it is (SETUPX's CONFIGMG wrappers 4:13E6, 4:1470, 4:149E ...). The decompiler
// drops the register and C has no way to set it, nor to lay the frame out as the entry reads it.
py::set<py::str> _register_argument_calls(const NEFile& nef, const py::dict<Place, py::str>& at) {
    const auto md = desc::decoder16(false);
    py::dict<int64_t, Segment> segs;
    for (const Segment& s : nef.segments()) {
        segs.set(s.idx, s);
    }
    std::map<int64_t, std::vector<std::pair<int64_t, py::str>>> by_seg;
    for (const auto& kv : at.items()) {
        by_seg[std::get<0>(std::get<0>(kv))].push_back({std::get<1>(std::get<0>(kv)), std::get<1>(kv)});
    }
    py::set<py::str> out;
    for (auto& sl : by_seg) {
        const auto sit = segs.get(sl.first);
        if (!sit || !sit->file_off) {
            continue;
        }
        const py::bytes code = nef.data.slice(sit->file_off, sit->file_off + sit->length);
        auto& lst = sl.second;
        std::sort(lst.begin(), lst.end(), [](const auto& a, const auto& b) { return a.first < b.first || (a.first == b.first && str_less(a.second, b.second)); });
        for (size_t k = 0; k < lst.size(); ++k) {
            const int64_t off = lst[k].first;
            const int64_t end = k + 1 < lst.size() ? lst[k + 1].first : code.size();
            const desc::Insn* prev = nullptr;
            const py::list<desc::Insn> ins = md->disasm(code.slice(off, end), off);
            for (const desc::Insn& i : ins) {
                if (i.mn == "lcall" && i.os.compare(0, 6, "[bp - ") == 0 && prev != nullptr && prev->mn == "mov" &&
                    prev->os.compare(0, 19, "ax, word ptr [bp - ") == 0) {
                    out.add(lst[k].second);
                    break;
                }
                prev = &i;
            }
        }
    }
    return out;
}

// the registers a C function's text reads as they come in
const std::map<std::string, std::string>& whole_register() {
    static const std::map<std::string, std::string> m = {{"al", "a"}, {"ah", "a"}, {"ax", "a"}, {"eax", "a"}, {"bl", "b"}, {"bh", "b"},
                                                         {"bx", "b"}, {"ebx", "b"}, {"cl", "c"}, {"ch", "c"}, {"cx", "c"}, {"ecx", "c"},
                                                         {"dl", "d"}, {"dh", "d"}, {"dx", "d"}, {"edx", "d"}, {"si", "si"}, {"esi", "si"},
                                                         {"di", "di"}, {"edi", "di"}};
    return m;
}

// Whether a function's instructions load DS and do not put it back: a `mov ds, x` from anything but a copy taken of DS
// (`mov dx, ds`), with no `pop ds` or restoring `mov ds` after it.
bool _leaves_ds(const py::list<desc::Insn>& ins) {
    static const py::re::Pattern copy_of_ds = py::re::compile("(\\w+), ds");
    std::set<std::string> saved;
    bool changed = false;
    for (const desc::Insn& x : ins) {
        if (x.mn == "mov" && copy_of_ds.fullmatch(py::str(x.os))) {
            saved.insert(x.os.substr(0, x.os.find(',')));
        } else if (x.mn == "mov" && x.os.compare(0, 3, "ds,") == 0) {
            const py::str src = py::str(x.os.substr(x.os.find(',') + 1)).strip();
            changed = !saved.count(src.utf8());
        } else if (x.mn == "pop" && x.os == "ds") {
            changed = false;
        } else if (x.mn == "lds") {
            changed = true;
        }
    }
    return changed;
}

// Functions whose C cannot say what the original passes or returns in a register. -> {name: why}
//  - It sets a register just before calling one that reads that register as it comes in (USER's CallMsgFilter, sub_1_80CD:
//    `mov bx, 6` / `mov bx, -1`, then the hook caller sub_1_800A, which takes the hook type in BX; the C passed no BX, the
//    caller looked up a hook record at 1 and jumped to a selector D400 -- a GP on Setup's License page).
//  - It returns a word, but the original sets DX just before it returns: a DX:AX result (sub_1_81C8 returns es:[38h]:1Eh,
//    the C 1Eh alone).
py::dict<py::str, py::str> _registers_c_cannot_carry(const NEFile& nef, const py::dict<int64_t, Segment>& segs, const py::dict<Place, py::str>& at,
                                                     const py::dict<py::str, py::str>& texts) {
    static const py::re::Pattern register_in = py::re::compile("\\b(?:in|unaff)_(AL|AH|AX|BL|BH|BX|CL|CH|CX|DL|DH|DX|SI|DI)\\b");
    static const py::re::Pattern wide_re = py::re::compile("\\b(?:int4|uint4|xunknown4|undefined4)\\b|__far\\s*\\*");
    static const py::re::Pattern es_re = py::re::compile("\\bes\\b");
    static const py::re::Pattern es_first = py::re::compile("\\s*es\\s*,");
    const auto md = desc::decoder16(false);
    std::map<int64_t, std::vector<int64_t>> starts;
    std::map<int64_t, std::vector<std::tuple<int64_t, py::str, std::set<std::string>>>> takes;   // offset -> [(segment, name, registers)]
    std::map<int64_t, std::vector<std::pair<int64_t, py::str>>> by_offset;
    for (const auto& kv : at.items()) {
        const int64_t sg = std::get<0>(std::get<0>(kv)), o = std::get<1>(std::get<0>(kv));
        const py::str& n = std::get<1>(kv);
        starts[sg].push_back(o);
        by_offset[o].push_back({sg, n});
        std::set<std::string> regs;
        const py::str body = after_brace(texts.get(n, py::str("")));
        for (const py::str& r : register_in.findall(body)) {
            regs.insert(whole_register().at(r.lower().utf8()));
        }
        if (!regs.empty()) {
            takes[o].push_back({sg, n, regs});
        }
    }
    for (auto& s : starts) {
        std::sort(s.second.begin(), s.second.end());
    }
    std::map<int64_t, py::bytes> images;
    py::dict<py::str, py::str> out;
    std::set<std::string> leaves_ds;
    std::map<std::string, std::set<std::string>> calls_of;   // (and who calls each: a caller of one that leaves DS changed counts on that too)
    auto setdefault = [&](const py::str& k, const py::str& v) {
        if (!out.contains(k)) {
            out.set(k, v);
        }
    };
    for (const auto& kv : at.items()) {
        const int64_t sg = std::get<0>(std::get<0>(kv)), o = std::get<1>(std::get<0>(kv));
        const py::str& n = std::get<1>(kv);
        const auto text = texts.get(n);
        if (!text || !segs.contains(sg)) {
            continue;
        }
        if (!images.count(sg)) {
            const Segment& s = segs.at(sg);
            images[sg] = nef.data.slice(s.file_off, s.file_off + s.length);
        }
        const py::bytes& code = images.at(sg);
        int64_t stop = code.size();
        for (int64_t x : starts[sg]) {
            if (x > o) {
                stop = x;
                break;
            }
        }
        const py::list<desc::Insn> ins = md->disasm(code.slice(o, stop), o);
        const bool wide = static_cast<bool>(wide_re.search(py::split(*text, py::str("("), 1)[0]));
        // (it loads DS and never puts it back: it leaves the caller another DS -- sub_1_222B's `mov ds, ax; ret`, KERNEL's
        // sub_2_0170 `mov ds, cs:[16Eh]; ret`. Not USER's copy helper sub_1_39E2: `mov dx, ds` ... `mov ds, dx` puts it back)
        if (_leaves_ds(ins)) {
            leaves_ds.insert(n.utf8());
            setdefault(n, py::str("it leaves DS changed for its caller"));
        }
        for (int64_t k = 0; k < py::len(ins); ++k) {
            const desc::Insn& i = ins[k];
            std::vector<const desc::Insn*> prev;                 // ins[max(0, k - 4):k]
            for (int64_t p = std::max<int64_t>(0, k - 4); p < k; ++p) {
                prev.push_back(&ins[p]);
            }
            if (i.mn == "call" || i.mn == "lcall") {
                int64_t target = 0;
                try {
                    const py::str last = py::split(py::str(i.os), py::str(","))[-1].strip();
                    target = py::int_parse(last, 0);
                } catch (const py::ValueError&) {
                    continue;
                }
                const auto bo = by_offset.find(target);
                if (bo != by_offset.end()) {
                    for (const auto& cs_ : bo->second) {
                        if (!(i.mn == "call" && cs_.first != sg) && cs_.second != n) {
                            calls_of[cs_.second.utf8()].insert(n.utf8());
                        }
                    }
                }
                const auto tk = takes.find(target);
                if (tk != takes.end()) {
                    for (const auto& t : tk->second) {
                        const int64_t csg = std::get<0>(t);
                        const py::str& callee = std::get<1>(t);
                        const std::set<std::string>& regs = std::get<2>(t);
                        if ((i.mn == "call" && csg != sg) || callee == n) {
                            continue;
                        }
                        // (by the whole register: SUWIN's 7:0056 `mov ax, es:[si]` / `call 0`, and 7:0000 compares AL with the
                        // character looked for -- the C passed no AL, found no dot, and Setup set MSDOS.SYS aside as
                        // WINBOOT.---: Windows never started)
                        std::vector<const desc::Insn*> set_;
                        for (size_t pi = prev.size() > 3 ? prev.size() - 3 : 0; pi < prev.size(); ++pi) {
                            const desc::Insn* p = prev[pi];
                            const std::string first = py::str(p->os.substr(0, p->os.find(','))).strip().utf8();
                            const auto w = whole_register().find(first);
                            if (w != whole_register().end() && regs.count(w->second) &&
                                (p->mn == "mov" || p->mn == "xor" || p->mn == "sub" || p->mn == "lea" || p->mn == "les" || p->mn == "lds" ||
                                 p->mn == "add" || p->mn == "or")) {
                                set_.push_back(p);
                            }
                        }
                        if (!set_.empty()) {
                            const desc::Insn* last = set_.back();
                            setdefault(n, py::percent("it passes %s to %s in a register (%04x)",
                                                      py::str(last->os.substr(0, last->os.find(','))).strip().upper(), callee,
                                                      static_cast<int64_t>(i.address)));
                        }
                    }
                }
                // ... and the caller reads ES the callee left: an ES result no C returns (USER's sub_1_281C leaves ES = the task's
                // queue, and its caller clears a flag at es:[30h]; the C left ES at another segment, the flag stayed set and Setup
                // sat idle on its Options page)
                const desc::Insn* nxt = k + 1 < py::len(ins) ? &ins[k + 1] : nullptr;
                // ... and the caller branches on the flags the callee leaves: a result no C returns, and a test no C can write
                // (USER's sub_1_222B: `or ax, ax; ret`, its callers `call 222B; je` -- the C returned nothing, its callers tested
                // AX left from elsewhere)
                if (nxt != nullptr && !nxt->mn.empty() && nxt->mn[0] == 'j' && nxt->mn != "jmp" && nxt->mn != "jcxz" && nxt->mn != "jecxz") {
                    if (bo != by_offset.end()) {
                        for (const auto& cs_ : bo->second) {
                            if (!(i.mn == "call" && cs_.first != sg) && cs_.second != n) {
                                setdefault(cs_.second, py::percent("it returns in the flags (%s branches on them, %04x)", n, static_cast<int64_t>(nxt->address)));
                                setdefault(n, py::percent("it branches on the flags %s leaves (%04x)", cs_.second, static_cast<int64_t>(nxt->address)));
                            }
                        }
                    }
                }
                if (nxt != nullptr && es_re.search(py::str(nxt->os)) && !es_first.match(py::str(nxt->os)) && nxt->mn != "les" && nxt->mn != "pop") {
                    if (bo != by_offset.end()) {
                        for (const auto& cs_ : bo->second) {
                            if (!(i.mn == "call" && cs_.first != sg) && cs_.second != n) {
                                setdefault(cs_.second, py::percent("its callers read the ES it returns (%s, %04x)", n, static_cast<int64_t>(nxt->address)));
                            }
                        }
                    }
                }
            } else if ((i.mn == "ret" || i.mn == "retf") && !wide) {
                bool dx = false;
                for (const desc::Insn* p : prev) {
                    if (((p->mn == "mov" || p->mn == "xor" || p->mn == "sub" || p->mn == "les" || p->mn == "lds") && p->os.compare(0, 3, "dx,") == 0) ||
                        p->mn == "cwd") {
                        dx = true;
                    }
                }
                if (dx) {
                    setdefault(n, py::percent("it returns DX:AX where its C returns a word (%04x)", static_cast<int64_t>(i.address)));
                }
            }
        }
    }
    // (a caller of one that leaves DS changed: the original puts its own DS back after it -- KERNEL's sub_2_2437 `push ds; call
    // 0170 ... pop ds`; its C kept the kernel's DS and the caller after it faulted, "Invalid DPMI return from 0367:2BC3")
    for (const std::string& callee : leaves_ds) {
        const auto it = calls_of.find(callee);
        if (it == calls_of.end()) {
            continue;
        }
        for (const std::string& caller : it->second) {
            setdefault(py::str(caller), py::percent("it calls %s, which leaves DS changed", py::str(callee)));
        }
    }
    return out;
}

// {function name: (object-file symbol, object path relative to folder)}
py::dict<py::str, std::tuple<py::str, py::str>> _publics(const py::str& folder, const py::dict<py::str, py::str>& stems) {
    py::dict<py::str, std::tuple<py::str, py::str>> out;
    for (const auto& kv : stems.items()) {
        const py::str& name = std::get<0>(kv);
        const py::str& stem = std::get<1>(kv);
        py::bytes body;
        try {
            body = read_bytes(py::os::path::join(folder, stem + py::str(".obj")));
        } catch (const py::OSError&) {
            continue;
        }
        const py::str k = base::cache::key(Cs{body, py::str("formats.omf publics typed port 1")});   // (an object's publics: read once, kept by its bytes and the reader)
        const std::optional<py::bytes> had = base::cache::get(py::str("omf_publics"), k);
        std::vector<py::str> symbols;
        if (!had) {
            const py::set<py::str> found = formats::omf::publics(body);
            symbols.assign(found.begin(), found.end());
            const py::list<py::str> sorted = sorted_strs(symbols);
            base::cache::put(py::str("omf_publics"), k, py::bytes(py::join(py::str("\n"), sorted).utf8()));
        } else {
            for (const py::str& s : py::split(had->decode("utf-8"), py::str("\n"))) {
                if (!s.empty()) {
                    symbols.push_back(s);
                }
            }
        }
        // as the compiler decorates a C name: _name (__cdecl, or __stdcall with an open argument list), _name@N (__stdcall),
        // name_ (register arguments)
        const py::re::Pattern decorated = py::re::compile(py::str("[_@]") + py::re::escape(name) + py::str("@\\d+"));
        std::optional<py::str> symbol;
        for (const py::str& s : sorted_strs(symbols)) {
            if (s == py::str("_") + name || s == name || s == name + py::str("_") || decorated.fullmatch(s)) {
                symbol = s;
                break;
            }
        }
        if (symbol) {
            out.set(name, {*symbol, stem + py::str(".obj")});
        }
    }
    return out;
}

// A stand-in for something the link could not find: the header's declaration of it with an empty body, or -- when it was
// never declared -- the function a call to an undeclared name assumes.
std::optional<py::str> _from_header(const py::str& header, const py::str& symbol) {
    static const py::re::Pattern c_name = py::re::compile("[A-Za-z_]\\w*");
    static const py::re::Pattern sized_re = py::re::compile("_\\w+@(\\d+)");
    const py::str name = build::functions::_plain(symbol);
    if (!c_name.fullmatch(name)) {
        return std::nullopt;
    }
    const py::re::Pattern decl = py::re::compile(py::str("^(?:extern\\s+)?([^;{}#\\n]*\\b") + py::re::escape(name) + py::str("\\s*\\([^;{}\\n]*\\))\\s*;"), py::re::M);
    const auto m = decl.search(header);
    if (m) {
        return py::percent("%s\n{\n  return 0;\n}\n", m->group(1));
    }
    const auto sized = sized_re.fullmatch(symbol);                // _name@N: __stdcall with N bytes of arguments
    if (sized) {
        py::list<py::str> params;
        for (int64_t i = 0; i < py::floordiv(py::int_parse(sized->group(1)), 4); ++i) {
            params.append(py::percent("int a%d", i));
        }
        return py::percent("int __stdcall %s(%s)\n{\n  return 0;\n}\n", name, params.empty() ? py::str("void") : py::join(py::str(", "), params));
    }
    return py::percent("int %s()\n{\n  return 0;\n}\n", name);
}

// The assembly of the module's own instructions where C does not do (build/asm16.py): each code segment's lead-in -- what
// is before its first function, often a variable the code reads through CS -- linked first into the segment so that it stays
// where it was; and each rejected function, instead of a stand-in, when it can be written so and assembles back to the
// original's bytes. -> {import symbol without its underscore: (module, what)} they need.
ImportMap _assembled16(Built& b, const NEFile& nef, const py::dict<Place, py::str>& at, const py::dict<int64_t, Segment>& segs,
                       py::dict<py::str, py::str>& stems, const py::dict<py::str, std::tuple<py::str, py::str>>& public_, const py::str& folder,
                       const Env& env, const py::dict<int64_t, py::bytes>& carried, const py::set<py::str>& forced0, const StemOf& elsewhere) {
    const py::str wasm = build::watcom::ow_tool(py::str("wasm"));   // platforms.of("ne").packager.assembler()
    // (what the rounds of a piece come to, below, is kept in the shared build cache by every input of them: the assembler
    // (functions.tool_inputs: its bytes, INCLUDE, WASM), the code that writes, probes and checks the text (asm16 and the data
    // writer it uses, source/assembly, msc6's object reader, this function -- the rounds are in it -- and _wasm), the decoder
    // (capstone's version), and the original module's bytes, which say each instruction's bytes, its segment's width (16- or
    // 32-bit) and its relocations; then, per piece, its text, place and stem)
    const py::str module_bytes = base::cache::key(Cs{nef.data});
    const py::str asm_stamp = base::cache::key(Cs{py::str("assembled16"), wasm.empty() ? py::str("") : build::functions::tool_inputs(wasm, env, py::str("WASM")),
                                                  asm16::module_file(), py::str("build.data source.assembly build.msc6 typed ports 1"), py::str(kStamp),
                                                  py::str("capstone 5"), module_bytes});
    b.lead_ins = py::list<py::str>();
    b.placed_c = py::set<py::str>();
    b.moved_c = py::set<py::str>();
    b._publics = public_.copy();
    ImportMap out;
    if (wasm.empty()) {
        return out;
    }
    py::dict<py::str, Place> where;
    std::map<int64_t, std::vector<int64_t>> starts;
    for (const auto& kv : at.items()) {
        where.set(std::get<1>(kv), std::get<0>(kv));
        starts[std::get<0>(std::get<0>(kv))].push_back(std::get<1>(std::get<0>(kv)));
    }
    for (auto& s : starts) {
        std::sort(s.second.begin(), s.second.end());
    }
    std::vector<std::pair<Place, py::str>> sorted_at;
    for (const auto& kv : at.items()) {
        sorted_at.push_back({std::get<0>(kv), std::get<1>(kv)});
    }
    std::sort(sorted_at.begin(), sorted_at.end(), [](const auto& x, const auto& y) { return x.first < y.first; });
    std::map<int64_t, py::str> in_segment;
    for (const auto& pn : sorted_at) {
        const py::str& name = pn.second;
        if (public_.contains(name) && !b.rejected.contains(name) && !in_segment.count(std::get<0>(pn.first))) {
            in_segment[std::get<0>(pn.first)] = std::get<0>(public_.at(name));
        }
    }
    std::map<Place, py::str> entries;                           // (segment, offset) -> symbol: a place called into mid-run

    static const py::re::Pattern non_word = py::re::compile("\\W");
    const int64_t autodata = nef.h.autodata_seg;
    const asm16::SymbolFor symbol_for = [&](const asm16::Reference& r) -> std::optional<py::str> {
        if (r.kind == py::str("function")) {
            const auto name = opt_get(at, Place{r.seg, r.off});
            if (name && public_.contains(*name)) {
                return std::get<0>(public_.at(*name));
            }
            const bool in_entries = entries.count(Place{r.seg, r.off}) != 0;
            if (!in_entries && name && stems.contains(*name)) {
                // (a function built on its own -- from C, or assembled by itself -- by the name it links under: NETDI's 9:0000
                // and 48:0000, far pointers to them in assembly beside -- with no symbol, 76 functions of four segments were
                // refused as assembly)
                return py::str("_") + *name;
            }
            const auto e = entries.find(Place{r.seg, r.off});
            return e == entries.end() ? std::nullopt : std::optional<py::str>(e->second);
        }
        if (r.kind == py::str("segment")) {
            // (any symbol in the segment gives its selector -- a function of it built on its own too: NETDI's segments 9 and 48
            // hold one each, written in assembly, and the selectors of them in the code beside had no symbol)
            const auto it = in_segment.find(r.seg);
            if (it != in_segment.end() && !it->second.empty()) {
                return it->second;
            }
            for (const auto& pn : sorted_at) {
                if (std::get<0>(pn.first) == r.seg && stems.contains(pn.second)) {
                    return py::str("_") + pn.second;
                }
            }
            return std::nullopt;
        }
        if (r.kind == py::str("data")) {                        // a data segment: the auto one, or one carried
            if (autodata && r.seg == autodata) {
                return py::str("_codebreaker_data");
            }
            return carried.contains(r.seg) ? std::optional<py::str>(py::percent("_codebreaker_segment_%d", r.seg)) : std::nullopt;
        }
        if (std::holds_alternative<int64_t>(r.which)) {
            return py::percent("_%s_%s", non_word.sub(py::str("_"), r.module), std::get<int64_t>(r.which));
        }
        return py::str("_") + non_word.sub(py::str("_"), std::get<py::str>(r.which));
    };
    // (from nothing: what an earlier build left there is read below -- which far function may move to the segment beside
    // depends on what the pieces refer to -- and would decide it)
    try {
        py::shutil::rmtree(py::os::path::join(folder, py::str("asm")), true);
    } catch (const py::OSError&) {
    }
    py::os::makedirs(py::os::path::join(folder, py::str("asm")), 0777, true);

    const PieceAssembler assemble_piece = [&](const py::str& stem, Fn& fn, int64_t lo, int64_t hi) -> py::str {
        const auto r = _wasm(wasm, env, folder, stem, fn.f->write(fn.good, lo, hi));
        const py::str obj = py::os::path::join(folder, stem + py::str(".obj"));
        if (std::get<0>(r) || !py::os::path::isfile(obj)) {
            return py::str("the assembler: ") + strip_tail(std::get<1>(r), 160);
        }
        return asm16::check_piece(read_bytes(obj), *fn.f, lo, hi);
    };

    // Written as readable as the assembler allows: each instruction as its mnemonic where the probe says the assembler gives
    // back the original's bytes, and round after round any that did not come back as they were go back to bytes.
    // -> "" or why it could not be.
    auto assemble_rounds = [&](const py::str& stem, Fn& fn) -> py::str {
        static const py::re::Pattern error_line = py::re::compile("\\((\\d+)\\): Error");
        asm16::Function& f = *fn.f;
        const py::dict<int64_t, std::tuple<py::str, py::bytes, py::bytes>> cand = f.candidates();
        py::set<source::assembly::Candidate> ok;
        if (!cand.empty()) {
            py::list<source::assembly::Candidate> values;
            for (const auto& kv : cand.items()) {
                values.append(std::get<1>(kv));
            }
            ok = source::assembly::probe(values, py::set<py::str>(), folder, py::str("real16"));
        }
        fn.good = py::set<int64_t>();
        py::set<int64_t>& good = fn.good;
        for (const auto& kv : cand.items()) {
            if (ok.contains(std::get<1>(kv))) {
                good.add(std::get<0>(kv));
            }
        }
        for (const auto& kv : f.ins.items()) {
            if (std::get<3>(std::get<1>(kv)).has_value()) {
                good.add(std::get<0>(kv));
            }
        }
        py::str why("no rounds");
        for (int round = 0; round < 40; ++round) {
            const auto r = _wasm(wasm, env, folder, stem, f.write(good));
            const py::str obj = py::os::path::join(folder, stem + py::str(".obj"));
            if (std::get<0>(r) || !py::os::path::isfile(obj)) {
                const py::list<py::str> lines = py::split(f.write(good), py::str("\n"));
                std::set<std::string> bad;
                for (const auto& m : error_line.finditer(std::get<1>(r))) {
                    const int64_t n = py::int_parse(m.group(1));
                    if (n >= 1 && n <= py::len(lines)) {
                        bad.insert(lines[n - 1].strip().utf8());
                    }
                }
                std::vector<int64_t> drop;
                for (int64_t at_ : good) {
                    if (f.ins.contains(at_) && bad.count(f.mnemonic(at_).utf8())) {
                        drop.push_back(at_);
                    }
                }
                if (drop.empty()) {
                    return py::str("the assembler: ") + strip_tail(std::get<1>(r), 160);
                }
                for (int64_t a : drop) {
                    good.discard(a);
                }
                continue;
            }
            const msc6::Code oc = msc6::object_code(read_bytes(obj));
            const py::bytes& code = std::get<0>(oc);
            std::vector<int64_t> hit;
            if (code.size() == f.end - f.start) {
                const py::list<int64_t> diff = asm16::differences(code, std::get<1>(oc), f);
                if (diff.empty()) {
                    return py::str("");
                }
                const int64_t at_ = diff[0];
                for (int64_t a : good) {
                    if (a <= at_ && at_ < a + std::get<0>(f.ins.at(a)).size()) {
                        hit.push_back(a);
                    }
                }
                if (hit.empty()) {
                    std::optional<int64_t> mx;
                    for (int64_t a : good) {
                        if (a <= at_ && (!mx || a > *mx)) {
                            mx = a;
                        }
                    }
                    if (mx) {
                        hit.push_back(*mx);
                    }
                }
                why = py::percent("differs at %04x", at_);
            } else {
                // longer or shorter: the first instruction whose bytes do not line up is blamed
                why = py::percent("%d bytes, the original %d", code.size(), f.end - f.start);
                std::vector<int64_t> order(good.begin(), good.end());
                std::sort(order.begin(), order.end());
                for (int64_t a : order) {
                    const int64_t n = std::get<0>(f.ins.at(a)).size();
                    if (code.slice(a - f.start, a - f.start + n) != f.blob.slice(a, a + n)) {
                        hit.push_back(a);
                        break;
                    }
                }
            }
            if (hit.empty()) {
                return why;
            }
            for (int64_t a : hit) {
                good.discard(a);
            }
        }
        return why;
    };
    auto good_text = [](const py::set<int64_t>& good) {
        std::vector<int64_t> v(good.begin(), good.end());
        std::sort(v.begin(), v.end());
        std::string s;
        for (size_t i = 0; i < v.size(); ++i) {
            s += (i ? "," : "") + std::to_string(v[i]);
        }
        return s;
    };
    auto assemble = [&](const py::str& stem, Fn& fn) -> py::str {
        asm16::Function& f = *fn.f;
        // (what the rounds came to is kept by what decides it -- the module, where the piece is, its names and holes, this code
        // and the assembler: the same piece in a later build is written once, as it ended, and not probed and assembled round
        // after round again)
        const py::str k = base::cache::key(Cs{f.text ? *f.text : py::str(""), f.seg, f.start, f.end, stem, asm_stamp});
        const std::optional<py::bytes> had = base::cache::get(py::str("assembled16"), k);
        if (had) {
            const std::string& raw = had->raw();
            const size_t nul = raw.find('\0');
            const py::str why = py::str::from_utf8(raw.data(), nul == std::string::npos ? raw.size() : nul);
            fn.good = py::set<int64_t>();
            if (nul != std::string::npos) {
                size_t p = nul + 1;
                while (p < raw.size()) {
                    const size_t q = raw.find(',', p);
                    const std::string num = raw.substr(p, q == std::string::npos ? std::string::npos : q - p);
                    if (!num.empty()) {
                        fn.good.add(std::stoll(num));
                    }
                    if (q == std::string::npos) {
                        break;
                    }
                    p = q + 1;
                }
            }
            if (!why.empty()) {
                return why;
            }
            const auto r = _wasm(wasm, env, folder, stem, f.write(fn.good));
            const py::str obj = py::os::path::join(folder, stem + py::str(".obj"));
            if (!std::get<0>(r) && py::os::path::isfile(obj)) {   // (checked again: the original's bytes)
                const msc6::Code oc = msc6::object_code(read_bytes(obj));
                if (std::get<0>(oc).size() == f.end - f.start && asm16::differences(std::get<0>(oc), std::get<1>(oc), f).empty()) {
                    return py::str("");
                }
            }
        }
        const py::str result = assemble_rounds(stem, fn);
        base::cache::put(py::str("assembled16"), k, py::bytes(result.utf8() + std::string(1, '\0') + good_text(fn.good)));
        return result;
    };

    std::map<int64_t, int64_t> lead;
    // A code segment with assembly in it is rebuilt as one frame from its first byte: the assembly, and the lead-in, stay
    // where they were -- hand-written code reaches into its own segment by number (tables, variables, LODS through CS) -- and
    // where each function rebuilt from C was is filler: those are placed after the frame and called by their names.
    std::set<int64_t> whole;
    for (const auto& s : starts) {
        if (segs.contains(s.first) && segs.at(s.first).kind == py::str("CODE")) {
            for (int64_t o : s.second) {
                if (forced0.contains(at.at(Place{s.first, o}))) {
                    whole.insert(s.first);
                    break;
                }
            }
        }
    }
    for (const auto& s : starts) {
        const int64_t seg = s.first, first = s.second[0];
        if (first && segs.contains(seg) && segs.at(seg).kind == py::str("CODE") && !whole.count(seg)) {
            Fn fn;
            fn.f = std::make_shared<asm16::Function>(nef, seg, 0, first, asm16::Publics(py::percent("codebreaker_lead_%d", seg)), symbol_for);
            const py::str stem = py::percent("asm/lead%02d", seg);
            if (fn.f->text && assemble(stem, fn).empty()) {
                lead[seg] = first;
                b.lead_ins.append(stem);
                for (const auto& kv : fn.f->imports.items()) {
                    out.set(std::get<0>(kv).slice_from(1), std::get<1>(kv));
                }
            }
        }
    }
    // the functions written in assembly, each run of them in a segment together -- as the file they were written in had them: a
    // jump from one into the next is a jump to a label -- and each rejected C function on its own
    py::set<py::str> forced;
    for (const py::str& x : forced0) {
        forced.add(x);
    }
    struct Done {
        bool is_why = true;
        py::str why;
        py::str stem;
        std::shared_ptr<Fn> f;
        std::vector<py::str> run;
    };
    py::dict<py::str, Done> done;                              // run (joined names) -> its stem, or why it was refused
    auto run_key = [](const std::vector<py::str>& run) {
        py::list<py::str> parts;
        for (const py::str& n : run) {
            parts.append(n);
        }
        return py::join(py::str("\x1f"), parts);
    };
    for (int pass = 0; pass < 40; ++pass) {                    // (each pass may find a place entered mid-run: its run is written again)
        std::vector<std::vector<py::str>> runs;
        for (int64_t seg : whole) {                            // one frame per such segment
            std::vector<py::str> mine;
            for (int64_t o : starts.at(seg)) {
                const py::str& n = at.at(Place{seg, o});
                if (public_.contains(n) && (forced.contains(n) || b.rejected.contains(n))) {
                    mine.push_back(n);
                }
            }
            if (!mine.empty()) {
                runs.push_back(mine);
            }
        }
        for (const auto& s : starts) {
            const int64_t seg = s.first;
            if (whole.count(seg)) {
                continue;
            }
            std::vector<py::str> run;
            for (int64_t off : s.second) {
                const py::str& name = at.at(Place{seg, off});
                if (forced.contains(name) && public_.contains(name)) {
                    run.push_back(name);
                    continue;
                }
                if (!run.empty()) {
                    runs.push_back(run);
                }
                run.clear();
                if (b.rejected.contains(name) && public_.contains(name)) {
                    runs.push_back({name});
                }
            }
            if (!run.empty()) {
                runs.push_back(run);
            }
        }
        py::set<py::str> joined;
        py::set<Place> wanted;

        // A run written and assembled. -> (run, Function) -- many at once: each its own files
        auto one = [&](const py::list<py::str>& run_l) -> std::pair<std::vector<py::str>, std::shared_ptr<Fn>> {
            std::vector<py::str> run;
            for (const py::str& n : run_l) {
                run.push_back(n);
            }
            const Place w0 = where.at(run[0]);
            const int64_t seg = std::get<0>(w0);
            int64_t off = std::get<1>(w0);
            const int64_t last = std::get<1>(where.at(run.back()));
            int64_t end = last;
            {
                std::optional<int64_t> later;
                for (int64_t o : starts.at(seg)) {
                    if (o > last) {
                        later = o;
                        break;
                    }
                }
                end = later ? *later : (segs.contains(seg) ? segs.at(seg).length : last);
            }
            py::list<std::tuple<int64_t, int64_t, py::str>> holes;
            if (whole.count(seg)) {
                off = 0;
                end = segs.at(seg).length;
                std::set<std::string> names;
                for (const py::str& n : run) {
                    names.insert(n.utf8());
                }
                const std::vector<int64_t>& so = starts.at(seg);
                for (size_t k = 0; k < so.size(); ++k) {
                    const int64_t o = so[k];
                    const py::str& n = at.at(Place{seg, o});
                    if (!names.count(n.utf8())) {
                        const int64_t hi = k + 1 < so.size() ? so[k + 1] : end;
                        holes.append({o, hi, public_.contains(n) ? std::get<0>(public_.at(n)) : py::str("_") + n});
                    }
                }
            }
            py::list<std::tuple<int64_t, py::str>> publics;
            for (const py::str& n : run) {
                publics.append({std::get<1>(where.at(n)), std::get<0>(public_.at(n))});
            }
            py::list<int64_t> extra_entries;
            for (const auto& e : entries) {
                if (std::get<0>(e.first) == seg && off <= std::get<1>(e.first) && std::get<1>(e.first) < end) {
                    publics.append({std::get<1>(e.first), e.second});
                    extra_entries.append(std::get<1>(e.first));
                }
            }
            auto fn = std::make_shared<Fn>();
            fn->f = std::make_shared<asm16::Function>(nef, seg, off, end, asm16::Publics(publics), symbol_for,
                                                      lead.count(seg) ? lead.at(seg) : 0, extra_entries, whole.count(seg) != 0, holes);
            fn->f->assembled = std::nullopt;
            if (fn->f->text) {
                const py::str stem = py::str("asm/") + opt_get(stems, run[0]).value().replace(py::str("/"), py::str("_"));
                fn->f->assembled = assemble(stem, *fn);
            }
            return {run, fn};
        };
        py::list<py::list<py::str>> todo;
        for (const auto& run : runs) {
            if (!done.contains(run_key(run))) {
                py::list<py::str> l;
                for (const py::str& n : run) {
                    l.append(n);
                }
                todo.append(l);
            }
        }
        const auto made = jobs::pool::threads(one, todo, base::parallel::workers());
        for (const auto& rf : made) {
            const std::vector<py::str>& run = rf.first;
            asm16::Function& f = *rf.second->f;
            const int64_t seg = std::get<0>(where.at(run[0]));
            for (const Place& place : f.raw_far_entries) {
                if (!entries.count(place)) {
                    wanted.add(place);
                }
            }
            if (!f.text && (!f.missing.empty() || !f.far_missing.empty())) {
                // it calls into the middle of a run of assembly (in its segment, or far in another): that place is an entry of
                // its own
                for (int64_t t : f.missing) {
                    if (!entries.count(Place{seg, t})) {
                        wanted.add(Place{seg, t});
                    }
                }
                for (const Place& place : f.far_missing) {
                    if (!entries.count(place)) {
                        wanted.add(place);
                    }
                }
                continue;
            }
            if (!f.text && f.reaches) {
                // it jumps into the middle of a function beside it: that one was written in assembly too (nothing in its own
                // code said so); the run takes it in
                std::optional<int64_t> inside;
                const auto so = starts.find(seg);
                if (so != starts.end()) {
                    for (int64_t o : so->second) {
                        if (o <= *f.reaches && (!inside || o > *inside)) {
                            inside = o;
                        }
                    }
                }
                if (inside) {
                    const py::str& nm = at.at(Place{seg, *inside});
                    if (!forced.contains(nm) && public_.contains(nm)) {
                        joined.add(nm);
                        continue;
                    }
                }
            }
            const py::str why = !f.text ? f.why : (f.assembled ? *f.assembled : py::str(""));
            Done d;
            d.run = run;
            if (!why.empty()) {
                d.is_why = true;
                d.why = why;
            } else {
                d.is_why = false;
                d.stem = py::str("asm/") + opt_get(stems, run[0]).value().replace(py::str("/"), py::str("_"));
                d.f = rf.second;
            }
            done.set(run_key(run), d);
        }
        for (const Place& pl : wanted) {
            const int64_t seg = std::get<0>(pl), t = std::get<1>(pl);
            // (a segment no function is known to start in has nothing to give an entry: KRNL386's segment 4 and GDI's 45, reached
            // far from a run's bytes -- the build stopped there)
            std::optional<int64_t> inside;
            const auto so = starts.find(seg);
            if (so != starts.end()) {
                for (int64_t o : so->second) {
                    if (o <= t && (!inside || o > *inside)) {
                        inside = o;
                    }
                }
            }
            std::optional<py::str> name;
            if (inside) {
                name = opt_get(at, Place{seg, *inside});
            }
            if (!name || !segs.contains(seg) || t >= std::max<int64_t>(segs.at(seg).length, segs.at(seg).minalloc)) {
                continue;                                      // (nor does a place past the segment's end: no piece would define it --
                //                                                a stand-in elsewhere, and the link refused the near call)
            }
            if (forced.contains(*name)) {
                entries[Place{seg, t}] = py::percent("_sub_%d_%04X", seg, t);
                std::vector<py::str> gone;
                for (const auto& kv : done.items()) {
                    for (const py::str& n : std::get<1>(kv).run) {
                        if (n == *name) {
                            gone.push_back(std::get<0>(kv));
                            break;
                        }
                    }
                }
                for (const py::str& k : gone) {
                    done.pop(k);                               // its run is written again, with the entry
                }
            } else if (public_.contains(*name)) {
                joined.add(*name);                             // called into mid-body: written in assembly
            }
        }
        if (joined.empty() && wanted.empty()) {
            break;
        }
        for (const py::str& name : joined) {
            if (b.origins.contains(name)) {
                const auto o = b.origins.at(name);
                o->language = py::str("assembly");
                o->evidence.append(py::str("entered in its middle from assembly beside it"));
            }
        }
        for (const py::str& n : joined) {
            forced.add(n);
        }
    }
    py::set<py::str> claimed;
    for (const auto& kv : done.items()) {
        const Done& result = std::get<1>(kv);
        const std::vector<py::str>& run = result.run;
        if (result.is_why) {
            continue;
        }
        bool overlap = false;
        for (const py::str& n : run) {
            if (claimed.contains(n)) {
                overlap = true;
            }
        }
        if (overlap) {
            continue;
        }
        bool unforced = false;
        for (const py::str& n : run) {
            if (!forced.contains(n) && !b.rejected.contains(n)) {
                unforced = true;
            }
        }
        if (unforced) {
            continue;
        }
        for (const py::str& n : run) {
            claimed.add(n);
        }
        const int64_t seg = std::get<0>(where.at(run[0]));
        std::optional<py::list<py::str>> placed;
        if (whole.count(seg)) {
            placed = _frame_pieces(b, *result.f, seg, stems, folder, wasm, env, assemble_piece, elsewhere);
            if (!placed) {
                for (const py::str& name : run) {
                    b.assembly_refused.set(name, py::str("its frame's pieces do not assemble back: ") + b.frame_why);
                }
                b.placed_c = py::set<py::str>();
                continue;
            }
            for (const py::str& p : *placed) {                 // first into its segment, in order
                b.lead_ins.append(p);
            }
        }
        for (const py::str& name : run) {
            _assembled_one(b, name, stems, whole.count(seg) ? (*placed)[0] : result.stem);
        }
        for (const auto& imp : result.f->f->imports.items()) {
            out.set(std::get<0>(imp).slice_from(1), std::get<1>(imp));
        }
    }
    for (const auto& kv : done.items()) {
        const Done& result = std::get<1>(kv);
        if (result.is_why) {
            for (const py::str& name : result.run) {
                if (!claimed.contains(name)) {
                    b.assembly_refused.set(name, result.why);
                }
            }
        }
    }
    return out;
}

// Write and compile the module's resources. -> the .res to link, or None.
std::optional<py::str> _resources(Built& b, const py::str& path, const py::str& folder) {
    if (formats::resources::read(path).empty()) {
        return std::nullopt;
    }
    b.resources = std::make_shared<build::resources::Result>(build::resources::rebuild(path, folder));
    return b.resources->link_res;
}

void _write_definitions(const py::str& path, const py::str& folder, const std::function<std::optional<py::str>(const build::definitions::Export&)>& declaration_at) {
    const py::str stem = std::get<0>(py::os::path::splitext(py::os::path::basename(path))).lower();
    write_text(py::os::path::join(folder, stem + py::str(".def")), build::definitions::definition_file(path));
    write_text(py::os::path::join(folder, stem + py::str("_public.h")), build::definitions::public_header(path, declaration_at));
}

}  // namespace

// ---- the small public ones --------------------------------------------------------------------------------------------
// the stem of an object from its C stem (`cstem` {name: stem}): the name that has it.
py::str name_of_stem(const py::str& stem, const py::dict<py::str, py::str>& cstem) {
    for (const auto& kv : cstem.items()) {
        if (std::get<1>(kv) == stem) {
            return std::get<0>(kv);
        }
    }
    throw py::StopIteration();
}

// {name: why} for the functions of `module` kept as their instructions by kept_as_asm.txt: C the gate passes that a run of
// the whole module showed wrong. The list is for those the decompiler cannot yet write right; each line goes when it can.
py::dict<py::str, py::str> kept_as_asm(const py::str& module) {
    py::dict<py::str, py::str> kept;
    try {
        const py::str text = read_text(KEPT_AS_ASM);
        for (const py::str& line : py::split(text, py::str("\n"))) {
            // line.split("#", 1)[0].split(None, 2)
            const py::str code = py::split(line, py::str("#"), 1)[0];
            std::vector<py::str> parts;
            const std::u32string& u = code.u32();
            size_t i = 0;
            auto is_space = [](char32_t c) { return c == U' ' || c == U'\t' || c == U'\r' || c == U'\n' || c == U'\f' || c == U'\v'; };
            while (i < u.size() && parts.size() < 2) {
                while (i < u.size() && is_space(u[i])) {
                    ++i;
                }
                if (i >= u.size()) {
                    break;
                }
                size_t j = i;
                while (j < u.size() && !is_space(u[j])) {
                    ++j;
                }
                parts.push_back(py::str(u.substr(i, j - i)));
                i = j;
            }
            if (parts.size() == 2) {
                while (i < u.size() && is_space(u[i])) {
                    ++i;
                }
                if (i < u.size()) {
                    parts.push_back(py::str(u.substr(i)));
                }
            }
            if (parts.size() >= 2 && parts[0].lower() == module.lower()) {
                kept.set(parts[1], parts.size() > 2 ? parts[2].strip() : py::str(""));
            }
        }
    } catch (const py::OSError&) {
    }
    return kept;
}

// An original whose MZ header has DOS load the whole file (KRNL386: its real-mode start reads the NE header and the segments
// out of memory) -- ours says the same of ours.
void _whole_file_image(const py::str& original, const py::str& image) {
    const int64_t size = file_size(original);
    int64_t pages = 0;
    {
        py::file f = py::file::open(original, py::str("rb"));
        const py::bytes head = f.read(6);
        pages = static_cast<int64_t>(u_at(head, 4, 2));
    }
    if (pages * 512 < size - 511) {                            // (a stub alone: nothing to do)
        return;
    }
    py::file f = py::file::open(image, py::str("r+b"));
    f.seek(0, 2);
    const int64_t n = f.tell();
    f.seek(2);
    f.write(py::pack("<HH", n % 512, py::floordiv(n + 511, 512)));
}

// [(address, bytes)] for everything of a 32-bit image that is not code: the emulator maps a module's code itself, and reads
// anything else as zeros unless it is given.
py::list<std::tuple<int64_t, py::bytes>> _sections(const py::str& path) {
    formats::pe::PEFile pe(path);
    py::list<std::tuple<int64_t, py::bytes>> out;
    for (const formats::pe::Section& s : pe.sections) {
        if (s.rawsize && !(s.chars & 0x20000020LL)) {
            out.append({static_cast<int64_t>(pe.image_base) + s.vaddr, pe.data.slice(s.rawptr, s.rawptr + s.rawsize)});
        }
    }
    return out;
}

// ---- 32-bit ---------------------------------------------------------------------------------------------------------
// A 32-bit module as one L2 tree (docs/IR_UNIFICATION.md step 4), as module_document has a 16-bit one: its functions in
// address order, each rebuilt from C its L2 document (its MethodDecl: m.l2, which compilable32 keeps), placed by its address
// (origin {module, va}); one with no L2 (the decompiler could not make it) its name and place alone. (No 32-bit function is
// kept as its instructions: one the compiler rejects is a stand-in.)
translate::ir::Dict module_document32(const py::str& module, const decompile::c_text::Module32& m) {
    using translate::ir::Val;
    const py::dict<py::str, translate::ir::Dict>& docs = m.l2;
    std::vector<std::pair<int64_t, py::str>> by;
    for (const auto& kv : m.by_va().items()) {
        by.push_back({std::get<0>(kv), std::get<1>(kv)});
    }
    std::sort(by.begin(), by.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
    Val::List units;
    for (const auto& vn : by) {
        translate::ir::Dict origin;
        origin.set(py::str("module"), Val(module));
        origin.set(py::str("va"), Val(vn.first));
        if (docs.contains(vn.second)) {
            translate::ir::Dict d = docs.at(vn.second).copy();
            d.set(py::str("origin"), Val(origin));
            units.append(Val(d));
        } else {
            translate::ir::Dict d;
            d.set(py::str("node"), Val("MethodDecl"));
            d.set(py::str("name"), Val(vn.second));
            d.set(py::str("kind"), Val("function"));
            d.set(py::str("origin"), Val(origin));
            units.append(Val(d));
        }
    }
    translate::ir::Dict out;
    out.set(py::str("node"), Val("Module"));
    out.set(py::str("units"), Val(units));
    return out;
}

std::optional<py::str> _build32(Built& b, const py::str& path, const decompile::c_text::Module32& m, const py::str& folder) {
    static const py::re::Pattern c_name = py::re::compile("[A-Za-z_]\\w*");
    static const py::re::Pattern ordinal_import = py::re::compile("_?([A-Za-z0-9]+)_ord_(\\d+)(?:@\\d+)?");
    static const py::re::Pattern non_word = py::re::compile("\\W");
    static const py::re::Pattern at_n = py::re::compile("@\\d+$");
    const build::watcom::OW ow = build::watcom::_ow();
    if (!py::os::path::exists(ow.wcc)) {
        throw py::RuntimeError("OpenWatcom's 32-bit compiler is not set up (settings.json: openwatcom)");
    }
    const Env env = build::watcom::_ow_env(ow);
    const py::list<py::str> cflags = cat(ct::CFLAGS, py::list<py::str>{py::str("-e=20")});
    formats::pe::PEFile pe(path);
    const py::dict<int64_t, py::str> by_va = m.by_va();
    py::dict<py::str, int64_t> va;                            // m.va: the inverse of by_va
    for (const auto& kv : by_va.items()) {
        va.set(std::get<1>(kv), std::get<0>(kv));
    }
    const py::dict<py::str, py::str> mtexts = m.texts();
    py::list<py::str> placed;
    for (const py::str& n : m.order()) {
        if (va.contains(n)) {
            placed.append(n);
        }
    }
    b.with_text = py::len(placed);
    py::dict<int64_t, py::str> at;
    for (const py::str& n : placed) {
        at.set(va.at(n), n);
    }
    auto extra = [&](const py::str& stem, const py::str& text) { return build::functions::_compile(ow.wcc, cflags, env, folder, stem, text); };
    auto at_get = [&](int64_t a) -> std::optional<py::str> { return opt_get(at, a); };

    // Exported data is cut out of its section as an object under its own name. That is tried first; if the compiler does not
    // keep the pieces of a section together, the sections are written whole and the data exports are reported missing.
    py::dict<int64_t, py::str> exported;
    for (const auto& e : build::definitions::exports(path)) {
        if (e.kind == py::str("data") && e.name && !e.name->empty() && c_name.fullmatch(*e.name) && !mtexts.contains(*e.name) &&
            std::holds_alternative<uint64_t>(e.address)) {
            exported.set(static_cast<int64_t>(std::get<uint64_t>(e.address)), *e.name);
        }
    }
    py::str header = m.header();
    py::dict<py::str, py::str> texts = mtexts;
    std::optional<py::str> data_object;
    std::vector<py::dict<int64_t, py::str>> attempts;
    if (!exported.empty()) {
        attempts.push_back(exported);
    }
    attempts.push_back(py::dict<int64_t, py::str>());
    std::shared_ptr<build::data::Data> d;
    for (const auto& named : attempts) {
        d = std::make_shared<build::data::Data>(build::data::data32(path, at_get, named));
        b.data = d;
        if (!d->movable) {
            b.data_error = py::str("the module has no relocation table, so nothing says which numbers are addresses");
            exported = py::dict<int64_t, py::str>();
            break;
        }
        const auto hl = build::data::relocate_header(m.header(), *d);
        header = std::get<0>(hl);
        b.lost_addresses = py::len(std::get<1>(hl));          // globals that are in no carried section
        write_text(py::os::path::join(folder, py::str("m.h")), header);
        const py::str why = extra(py::str("data"), d->source);
        if (!why.empty()) {
            b.data_error = py::str("the data does not compile: ") + why;
        } else if (build::data::layout_holds(read_bytes(py::os::path::join(folder, py::str("data.obj"))), *d)) {
            data_object = py::str("data.obj");
            exported = named;
            break;
        } else {
            b.data_error = py::str("the compiler did not keep the pieces of a section together");
        }
    }
    if (data_object) {
        b.data_error = py::str("");
        py::list<int64_t> starts;                              // int(k, 16) of m.all_keys: every function the lane carves
        for (int64_t a : verify::lanes::win32::LANES.at(py::str("pe")).functions(path)) {
            starts.append(a);
        }
        const auto constants = build::data::address_constants(path, starts);
        texts = py::dict<py::str, py::str>();
        for (const py::str& n : placed) {
            const auto ref = build::data::relocate_text(mtexts.at(n), constants.contains(va.at(n)) ? constants.at(va.at(n)) : py::set<int64_t>(), *d, at_get);
            texts.set(n, std::get<0>(ref));
            b.lost_addresses += py::len(std::get<1>(ref));
        }
    } else {
        header = m.header();
        exported = py::dict<int64_t, py::str>();
    }
    const auto compiled = build::functions::compile_all(ow.wcc, cflags, env, header, texts, placed, folder);
    const py::dict<py::str, py::str>& stems = compiled.stems;
    b.rejected = compiled.dropped;
    py::list<py::str> objects;
    for (const py::str& n : placed) {
        if (stems.contains(n)) {
            objects.append(stems.at(n) + py::str(".obj"));
        }
    }
    if (data_object) {
        objects.append(*data_object);
    }
    const auto public_ = _publics(folder, stems);
    std::optional<py::str> entry;
    if (pe.entry) {
        entry = at_get(static_cast<int64_t>(pe.image_base) + pe.entry);
    }
    std::optional<py::str> start;
    if (entry && public_.contains(*entry)) {
        start = std::get<0>(public_.at(*entry));
        b.entry = py::percent("%s, the function at the original's entry point", *entry);
    } else if (pe.is_dll()) {
        const py::str why = extra(py::str("entry"), py::str("int __stdcall codebreaker_entry(int4 module, int4 reason, int4 reserved)\n{\n  return 1;\n}\n"));
        if (!why.empty()) {                                    // the header itself does not compile
            b.error = py::str("the module's header does not compile: ") + why;
            return std::nullopt;
        }
        objects.append(py::str("entry.obj"));
        start = py::str("_codebreaker_entry@12");
        b.entry = pe.entry ? py::str("a stand-in that reports success (the original's entry point has no reconstruction)")
                           : py::str("a stand-in that reports success (the original has no entry point)");
    } else {
        b.entry = py::str("NONE: the program's entry point has no reconstruction");
    }
    const py::str subsystem = pe.build_meta().subsystem;
    py::list<py::str> lines{py::percent("format windows nt%s", pe.is_dll() ? py::str(" dll") : py::str("")),
                            py::percent("runtime %s=4.0", subsystem == py::str("console") ? py::str("console") : py::str("windows")),
                            py::str("name m.dll"),
                            py::str("option quiet, map, caseexact"),
                            py::percent("option modname='%s'", build::definitions::module_name(path)),
                            py::percent("option offset=0x%X", static_cast<int64_t>(pe.image_base)),
                            py::percent("libpath %s", build::watcom::ow_lib(py::str("pe")))};
    if (start && !start->empty()) {
        lines.append(py::percent("option start=%s", quote_symbol(*start)));
    }
    for (const py::str& o : objects) {
        lines.append(py::str("file ") + o);
    }
    const auto res = _resources(b, path, folder);
    if (res && !res->empty()) {
        lines.append(py::percent("option resource=%s", py::os::path::basename(*res)));
    }
    // the functions the original imports by name, bound to the same DLL; then those by ordinal (an ordinal is written as a bare
    // number: `'MOD'.'@17'` would be read as "take directives from the file 17"). An import whose own name is not a C name --
    // `_Name@12`, a C++ name -- is in the C text with every other character turned into `_`; it is bound back here.
    const build::watcom::Numbered numbered = [&]() {
        const auto& by_number = desc::BY_NUMBER;
        if (by_number.contains(path) && !by_number.at(path).empty()) {
            return by_number.at(path);
        }
        const py::str ap = py::os::path::abspath(path);
        if (by_number.contains(ap) && !by_number.at(ap).empty()) {
            return by_number.at(ap);
        }
        return build::watcom::Numbered();
    }();
    const py::dict<py::str, py::str> named = build::watcom::import_map(path);
    py::set<py::str> seen;
    const py::set<build::definitions::Import> originals = build::definitions::imports(path);
    py::dict<py::str, std::tuple<py::str, py::str>> odd;
    for (const auto& imp : originals) {
        if (std::holds_alternative<py::str>(std::get<1>(imp)) && !c_name.fullmatch(std::get<py::str>(std::get<1>(imp)))) {
            odd.set(non_word.sub(py::str("_"), std::get<py::str>(std::get<1>(imp))), {std::get<0>(imp), std::get<py::str>(std::get<1>(imp))});
        }
    }
    py::set<py::str> import_names;
    for (const py::str& n : m.imports) {
        import_names.add(n);
    }
    for (const py::str& o : objects) {
        const py::bytes obj = read_bytes(py::os::path::join(folder, o));
        py::list<py::str> binds = py::split(build::watcom::_import_lines(obj, import_names, named, numbered).strip(), py::str("\n"));
        for (const py::str& sym : formats::omf::externs(obj)) {
            const auto mm = ordinal_import.fullmatch(sym);
            if (mm && originals.contains(build::definitions::Import{mm->group(1).upper(), std::variant<py::str, int64_t>(py::int_parse(mm->group(2)))})) {
                binds.append(py::percent("import '%s' '%s'.%s", sym, mm->group(1).upper(), mm->group(2)));
            }
            const py::str plain = at_n.sub(py::str(""), sym.startswith(py::str("_")) ? sym.slice_from(1) : sym);
            if (odd.contains(plain)) {
                binds.append(py::percent("import '%s' '%s'.'%s'", sym, std::get<0>(odd.at(plain)), std::get<1>(odd.at(plain))));
            }
        }
        for (const py::str& line : binds) {
            if (!line.empty() && !seen.contains(line)) {
                seen.add(line);
                lines.append(line);
            }
        }
    }
    auto symbol_at = [&](const build::definitions::Export& e) -> std::optional<py::str> {
        const int64_t addr = std::holds_alternative<uint64_t>(e.address) ? static_cast<int64_t>(std::get<uint64_t>(e.address)) : 0;
        if (e.kind == py::str("data")) {
            return exported.contains(addr) ? std::optional<py::str>(py::str("_") + exported.at(addr)) : std::nullopt;
        }
        const auto fn = at_get(addr);
        return fn && public_.contains(*fn) ? std::optional<py::str>(std::get<0>(public_.at(*fn))) : std::nullopt;
    };
    const auto ex = build::definitions::export_directives(path, symbol_at);
    for (const py::str& l : std::get<0>(ex)) {
        lines.append(l);
    }
    b.exports_missing = std::get<1>(ex);
    for (const py::str& x : py::split(build::watcom::_RTL.strip(), py::str("\n"))) {
        if (!x.empty()) {
            lines.append(x);
        }
    }
    const auto image = _link_resolving(
        b, ow.wlink, env, folder, lines, [&](const py::str& s) { return _from_header(header, s); },
        [&](const py::str& stem, const py::str& text) { return extra(stem, text); });
    b.stand_in_addresses = py::set<Address>();
    for (const py::str& n : b.rejected.keys()) {
        b.stand_in_addresses.add(Address(static_cast<uint64_t>(va.at(n))));
    }
    // (each export declared as the back end wrote its function's heading from L2: m.declarations, as the 16-bit build does -- not
    // read back off the text; a function with no C has none)
    _write_definitions(path, folder, [&](const build::definitions::Export& e) -> std::optional<py::str> {
        const int64_t addr = std::holds_alternative<uint64_t>(e.address) ? static_cast<int64_t>(std::get<uint64_t>(e.address)) : 0;
        const auto fn = at_get(addr);
        if (!fn) {
            return std::nullopt;
        }
        return opt_get(m.declarations, *fn);
    });
    // the module as one L2 tree, whatever each function came back as (docs/IR_UNIFICATION.md step 4)
    b.l2 = module_document32(b.name, m);
    write_json(py::os::path::join(folder, py::str("module.l2.json")), b.l2);
    return image;
}

// ---- 16-bit ---------------------------------------------------------------------------------------------------------
// The whole module as one L2 tree (docs/IR_UNIFICATION.md step 4): a Module whose units are its functions in address order,
// each as it came back -- one rebuilt from C as its L2 document (its MethodDecl, where CB_C_FROM_L2 gave one: m.l2; else its name
// and place alone), one kept as its instructions as a MachineFragment: its place, length, why, and the assembly the build wrote
// for it (a run of them assembled as one file: the text on the first, the others `within` it). `assembled`: {name: why}
// (Build.assembled); `stems`: {name: object stem}; `lengths`: {segment: bytes}, for the last function of each.
translate::ir::Dict module_document(const py::str& module, const decompile::c_text::Module16& m, const py::dict<py::str, py::str>& assembled,
                                    const py::dict<py::str, py::str>& stems, const py::str& folder,
                                    const std::optional<py::dict<int64_t, int64_t>>& lengths) {
    using translate::ir::Val;
    std::vector<std::pair<Place, py::str>> places;
    for (const auto& kv : m.by_va().items()) {
        if (m.where.contains(std::get<0>(kv)) && m.where.at(std::get<0>(kv))) {
            places.push_back({*m.where.at(std::get<0>(kv)), std::get<1>(kv)});
        }
    }
    std::sort(places.begin(), places.end(), [](const auto& a, const auto& b) { return a.first < b.first || (a.first == b.first && str_less(a.second, b.second)); });
    const py::dict<py::str, translate::ir::Dict>& docs = m.l2;
    Val::List units;
    py::dict<py::str, py::str> holder;                        // asm stem -> the fragment that holds its text
    std::optional<py::str> holder_none;                       // (and the one held under the key None)
    for (size_t k = 0; k < places.size(); ++k) {
        const int64_t seg = std::get<0>(places[k].first), off = std::get<1>(places[k].first);
        const py::str& name = places[k].second;
        std::optional<int64_t> end;
        if (k + 1 < places.size() && std::get<0>(places[k + 1].first) == seg) {
            end = std::get<1>(places[k + 1].first);
        } else if (lengths && lengths->contains(seg)) {
            end = lengths->at(seg);
        }
        translate::ir::Dict origin;
        origin.set(py::str("module"), Val(module));
        origin.set(py::str("segment"), Val(seg));
        origin.set(py::str("offset"), Val(off));
        if (assembled.contains(name)) {
            translate::ir::Dict frag;
            frag.set(py::str("node"), Val("MachineFragment"));
            frag.set(py::str("origin"), Val(origin));
            frag.set(py::str("name"), Val(name));
            frag.set(py::str("reason"), Val(assembled.at(name)));
            frag.set(py::str("syntax"), Val("wasm"));
            if (end && *end > off) {
                frag.set(py::str("length"), Val(*end - off));
            }
            const auto stem = opt_get(stems, name);
            // (a function with no stem is held under the key None, as in the Python's dict)
            if (stem ? holder.contains(*stem) : holder_none.has_value()) {
                frag.set(py::str("asm"), Val());
                frag.set(py::str("within"), Val(stem ? holder.at(*stem) : *holder_none));
            } else {
                try {
                    if (!stem) {
                        throw py::TypeError("expected str, bytes or os.PathLike object, not NoneType");
                    }
                    frag.set(py::str("asm"), Val(read_text(py::os::path::join(folder, *stem + py::str(".asm")))));
                } catch (const py::OSError&) {
                    frag.set(py::str("asm"), Val());
                } catch (const py::TypeError&) {
                    frag.set(py::str("asm"), Val());
                }
                if (stem) {
                    holder.set(*stem, name);
                } else {
                    holder_none = name;
                }
            }
            units.append(Val(frag));
        } else if (docs.contains(name)) {
            translate::ir::Dict d = docs.at(name).copy();
            d.set(py::str("origin"), Val(origin));
            units.append(Val(d));
        } else {
            translate::ir::Dict d;
            d.set(py::str("node"), Val("MethodDecl"));
            d.set(py::str("name"), Val(name));
            d.set(py::str("kind"), Val("function"));
            d.set(py::str("origin"), Val(origin));
            units.append(Val(d));
        }
    }
    translate::ir::Dict out;
    out.set(py::str("node"), Val("Module"));
    out.set(py::str("units"), Val(units));
    return out;
}

std::optional<py::str> _build16(Built& b, const py::str& path, const decompile::c_text::Module16& m, const py::str& folder) {
    static const py::re::Pattern unaff_cs = py::re::compile("\\w+\\([^;{}]*\\bunaff_CS\\b[^;{}]*\\)");
    static const py::re::Pattern unaff_es = py::re::compile("(?<![\\w.])unaff_ES\\b(?!\\s*=[^=])");
    static const py::re::Pattern call_re = py::re::compile("\\b(\\w+)\\s*\\(");
    static const py::re::Pattern file_apart = py::re::compile("file (s\\d+/f\\d+)\\.obj.*?not in the same segment");
    static const py::re::Pattern size_over = py::re::compile("size of segment CODE(\\d+) exceeds 64k by (\\d+) bytes");
    static const py::re::Pattern c_name = py::re::compile("[A-Za-z_]\\w*");
    static const py::re::Pattern near_ptr = py::re::compile("near ptr ([\\w@$?]+)");
    const py::str wcc = build::watcom::ow_tool(py::str("wcc")), wlink = build::watcom::ow_tool(py::str("wlink"));   // (16-bit Windows: its compiler and packager)
    if (wcc.empty() || wlink.empty()) {
        throw py::RuntimeError("OpenWatcom's 16-bit compiler is not set up (settings.json: openwatcom)");
    }
    const py::str wasm_tool = build::watcom::ow_tool(py::str("wasm"));
    const Env env = build::watcom::ow_env(py::str("ne"));
    const NEFile nef(path);
    py::dict<int64_t, Segment> segs;
    for (const Segment& s : nef.segments()) {
        segs.set(s.idx, s);
    }
    const py::dict<int64_t, py::str> by_va = m.by_va();
    const py::dict<py::str, py::str> mtexts = m.texts();
    const py::str mheader = m.header();
    py::dict<Place, py::str> at;
    for (const auto& kv : by_va.items()) {
        if (m.where.contains(std::get<0>(kv)) && m.where.at(std::get<0>(kv))) {
            at.set(*m.where.at(std::get<0>(kv)), std::get<1>(kv));
        }
    }
    const py::list<py::str> order = m.order();
    b.with_text = py::len(order);
    // the guard: a function the description does not have would be taken for the tail of the one before it, and erased with it if
    // that one is rebuilt from C -- never silently
    {
        py::set<SegOff> known;
        for (const auto& kv : at.items()) {
            known.add(std::get<0>(kv));
        }
        b.hidden = desc::hidden_functions(nef, known);
    }
    // each function into a code segment named for the segment it came from
    py::dict<py::str, py::str> stems;
    std::map<int64_t, py::list<py::str>> by_segment;
    std::vector<std::pair<Place, py::str>> sorted_at;
    for (const auto& kv : at.items()) {
        sorted_at.push_back({std::get<0>(kv), std::get<1>(kv)});
    }
    std::sort(sorted_at.begin(), sorted_at.end(), [](const auto& x, const auto& y) { return x.first < y.first; });
    for (const auto& pn : sorted_at) {
        by_segment[std::get<0>(pn.first)].append(pn.second);
    }
    py::list<py::str> cflags;
    for (const py::str& f : build::functions::CFLAGS16) {
        if (f != py::str("-ms")) {
            cflags.append(f);
        }
    }
    for (const char* f : {"-mm", "-fpi87", "-os", "-nc=MCODE"}) {                  // (MCODE: see the link order; size: a function rebuilt from C goes back into its place when it fits)
        cflags.append(py::str(f));
    }
    py::dict<py::str, int64_t> segment_of;
    for (const auto& kv : at.items()) {
        segment_of.set(std::get<1>(kv), std::get<0>(std::get<0>(kv)));
    }
    // A call the decompiler left as an address (`func_0x...`) goes to the function reconstructed at that address -- when it can
    // be reached from this segment.
    auto named_text = [&](const py::str& text, int64_t seg) {
        return _BY_ADDRESS.sub(
            [&](const py::re::Match& mm) -> py::str {
                const auto target = opt_get(by_va, py::int_parse(mm.group(1), 16));
                if (target && (!m.near.contains(*target) || (segment_of.contains(*target) && segment_of.at(*target) == seg))) {
                    return *target;
                }
                return mm.group(0);
            },
            text);
    };
    py::dict<py::str, py::str> texts;
    for (const auto& kv : segment_of.items()) {
        texts.set(std::get<0>(kv), named_text(mtexts.at(std::get<0>(kv)), std::get<1>(kv)));
    }
    {
        py::list<build::functions::Group> groups;
        for (const auto& sn : by_segment) {
            build::functions::Group g;
            g.cflags = cat(cflags, py::list<py::str>{py::percent("-nt=CODE%d", sn.first)});
            g.order = sn.second;
            g.outdir = py::os::path::join(folder, py::percent("s%02d", sn.first));
            groups.append(g);
        }
        const py::list<build::functions::Compiled> compiled = build::functions::compile_groups(wcc, env, mheader, texts, groups);
        size_t i = 0;
        for (const auto& sn : by_segment) {
            const build::functions::Compiled& c = compiled[static_cast<int64_t>(i++)];
            for (const auto& kv : c.stems.items()) {
                stems.set(std::get<0>(kv), py::percent("s%02d/%s", sn.first, std::get<1>(kv)));
            }
            for (const auto& kv : c.dropped.items()) {
                b.rejected.set(std::get<0>(kv), std::get<1>(kv));
            }
        }
    }

    write_text(py::os::path::join(folder, py::str("m.h")), mheader);   // for what is compiled beside the functions

    // (code the build adds -- an entry stand-in, the interrupt routines' registers, stand-ins -- in a class of its own, XCODE
    // (code to the linker: the name ends in CODE), laid out after the original's segments)
    auto extra = [&](const py::str& stem, const py::str& text) {
        py::list<py::str> flags = cat(cflags, py::list<py::str>{py::str("-nc=XCODE")});
        if (stem == py::str("data")) {
            flags.append(py::str("-dCODEBREAKER_DATA"));              // (data.c defines the segments it declares)
        }
        return build::functions::_compile(wcc, flags, env, folder, stem, text);
    };

    // every data segment besides the automatic one is carried, whether or not the C reads it: it is part of the module (and code
    // written as its own instructions refers to it)
    const int64_t auto_ = nef.h.autodata_seg;
    for (const auto& kv : segs.items()) {
        const int64_t n = std::get<0>(kv);
        const Segment& sg = std::get<1>(kv);
        if (sg.kind == py::str("DATA") && n != auto_ && !m.segments.contains(n)) {
            m.segments.set(n, sg.file_off ? nef.data.slice(sg.file_off, sg.file_off + sg.length)
                                          : py::bytes(std::string(static_cast<size_t>(std::min<int64_t>(sg.minalloc ? sg.minalloc : 0x10000, 0x10000)), '\0')));
        }
    }
    py::dict<int64_t, py::str> in_segment;                       // a code segment -> a function in it (its selector)
    for (const auto& pn : sorted_at) {
        if (!in_segment.contains(std::get<0>(pn.first))) {
            in_segment.set(std::get<0>(pn.first), pn.second);
        }
    }
    const auto at16 = [&](const std::tuple<int64_t, int64_t>& place) -> std::optional<py::str> { return opt_get(at, place); };
    const auto seg_symbol = [&](int64_t s) -> std::optional<py::str> { return opt_get(in_segment, s); };
    py::set<int64_t> carried_keys;
    for (int64_t k : m.segments.keys()) {
        carried_keys.add(k);
    }
    const auto d = std::make_shared<build::data::Data>(build::data::data16(path, at16, carried_keys, seg_symbol));
    b.data = d;
    const py::str far = build::data::far_segments(m.segments, path, at16, seg_symbol);
    {
        const py::str why = extra(py::str("data"), d->source + far);
        if (!why.empty()) {                                       // the code still needs a data segment to exist
            b.data_error = py::str("the data does not compile: ") + why;
            extra(py::str("data"), py::str("char codebreaker_data = 1;\n") + far);
        }
    }
    py::dict<py::str, std::tuple<py::str, py::str>> public_ = _publics(folder, stems);
    // A function written in assembly is rebuilt as assembly (analyze/origin.py says which, from what its code does), not as a C
    // translation of it -- where its instructions can be written so that they assemble back to the original's bytes; the C stays
    // only where that cannot be done.
    b.origins = py::dict<py::str, std::shared_ptr<analyze::origin::Origin>>();
    for (const auto& kv : analyze::origin::module16(path, by_va).functions.items()) {
        b.origins.set(std::get<1>(kv).name, std::make_shared<analyze::origin::Origin>(std::get<1>(kv)));
    }
    auto evidence = [&](const py::str& n, const py::str& text) {
        if (b.origins.contains(n)) {
            b.origins.at(n)->evidence.append(text);
        }
    };
    py::set<py::str> forced;
    for (const py::str& n : order) {
        if (stems.contains(n) && b.origins.contains(n) && b.origins.at(n)->language == py::str("assembly")) {
            forced.add(n);
        }
    }
    // A function of a 32-bit code segment (NE flag 2000h: GDI's 36 and 38, its flat thunks, which the thunk compiler wrote as use32
    // assembly) is its instructions: the C compiler here makes 16-bit code, which would run as 32-bit code in that segment
    for (const py::str& n : order) {
        const auto sg = opt_get(segment_of, n);
        if (stems.contains(n) && !forced.contains(n) && sg && segs.contains(*sg) && segs.at(*sg).kind == py::str("CODE") && (segs.at(*sg).flags & 0x2000)) {
            forced.add(n);
            evidence(n, py::percent("in a 32-bit code segment (%d): not 16-bit C", *sg));
        }
    }
    // C the decompiler got wrong in a way that can be seen: a call it gives the caller's CS as an argument (a far call whose
    // arguments it read a word off -- see the decompiler's far-call argument recovery). Until that is put right, such a function is
    // rebuilt from its own instructions rather than from C that would pass the wrong arguments.
    // ... and C with no result for a function whose instructions leave one in AX: the decompiler saw nothing in the module read it
    // (an export: Windows reads it -- KEYBOARD's Inquire returns the size it copied), and compiled C leaves its own AX, not the
    // original's
    std::map<int64_t, std::vector<int64_t>> starts;
    for (const auto& kv : at.items()) {
        starts[std::get<0>(std::get<0>(kv))].push_back(std::get<1>(std::get<0>(kv)));
    }
    for (const py::str& n : m.novalue) {
        if (!stems.contains(n) || forced.contains(n)) {
            continue;
        }
        std::optional<int64_t> vaddr;
        for (const auto& kv : by_va.items()) {
            if (std::get<1>(kv) == n) {
                vaddr = std::get<0>(kv);
                break;
            }
        }
        if (!vaddr || !m.where.contains(*vaddr) || !m.where.at(*vaddr)) {
            continue;
        }
        const int64_t sg = std::get<0>(*m.where.at(*vaddr)), off = std::get<1>(*m.where.at(*vaddr));
        if (!segs.contains(sg) || !segs.at(sg).file_off) {
            continue;
        }
        const py::bytes code = nef.data.slice(segs.at(sg).file_off, segs.at(sg).file_off + segs.at(sg).length);
        int64_t end = code.size();
        if (starts.count(sg)) {
            for (int64_t o : starts.at(sg)) {
                if (o > off) {
                    end = std::min(end, o);
                }
            }
        }
        const auto where_ax = _result_in_ax(code, off, end);
        if (where_ax) {
            evidence(n, py::percent("leaves a result in AX (%04x) its C does not return (not trusted as C)", *where_ax));
            forced.add(n);
        }
    }
    py::set<py::str> untrusted;
    for (const py::str& n : order) {
        if (stems.contains(n) && mtexts.contains(n) && unaff_cs.search(mtexts.at(n))) {
            untrusted.add(n);
        }
    }
    for (const py::str& n : untrusted) {
        if (!forced.contains(n)) {
            evidence(n, py::str("its decompiled calls pass the caller's CS (not trusted as C)"));
        }
    }
    for (const py::str& n : untrusted) {
        forced.add(n);
    }
    // ... and C one of whose calls passes clearly fewer (or more) words than the callee takes off (its `ret N`): the decompiler kept
    // to the pushes it could see, and the stack would come back wrong
    // ... and C that reads ES as the function was entered: no compiled C takes ES in. The original read ES as a callee left it --
    // USER's sub_1_7CD4 returns `mov ax, es` after KERNEL's GetCodeInfo (its unwritten second result, the module); the C returned
    // the ES it came in with, SetWindowsHook stored that as the hook's module, and calling the hook faulted (1:8093, selector D400)
    // on Setup's License page.
    for (const py::str& n : order) {
        if (forced.contains(n) || !stems.contains(n) || !mtexts.contains(n)) {
            continue;
        }
        if (unaff_es.search(after_brace(mtexts.at(n)))) {
            forced.add(n);
            evidence(n, py::str("its C reads ES as the function was entered (not trusted as C)"));
        }
    }
    for (const auto& kv : _registers_c_cannot_carry(nef, segs, at, mtexts).items()) {
        if (!forced.contains(std::get<0>(kv)) && stems.contains(std::get<0>(kv))) {
            forced.add(std::get<0>(kv));
            evidence(std::get<0>(kv), std::get<1>(kv) + py::str(" (not trusted as C)"));
        }
    }
    py::dict<py::str, int64_t> arity;
    for (const py::str& line : py::split(read_text(m.description->path), py::str("\n"))) {
        const py::list<py::str> parts = py::split(line.rstrip(py::str("\n")), py::str("\t"));
        if (parts[0] == py::str("F") && py::len(parts) > 4 && parts[4].startswith(py::str("__stdcall16")) && py::int_parse(parts[3]) > 0) {
            arity.set(parts[2], py::floordiv(py::int_parse(parts[3]), 2));
        }
    }
    {
        py::list<py::str> escaped;
        for (const py::str& x : arity.keys()) {
            escaped.append(py::re::escape(x));
        }
        const py::re::Pattern arity_call = py::re::compile(!arity.empty() ? py::str("\\b(") + py::join(py::str("|"), escaped) + py::str(")\\s*\\(") : py::str("(?!x)x"));
        for (const py::str& n : order) {
            if (forced.contains(n) || !stems.contains(n) || !mtexts.contains(n)) {
                continue;
            }
            const py::str body = after_brace(mtexts.at(n));
            for (const auto& mm : arity_call.finditer(body)) {
                const auto args = translate::native_l2::arguments_at(body, mm.end() - 1);
                const int64_t words = arity.at(mm.group(1));
                if (py::len(std::get<0>(args)) * 2 < words || py::len(std::get<0>(args)) > words) {
                    evidence(n, py::percent("its call of %s passes %d arguments for %d words (not trusted as C)", mm.group(1), py::len(std::get<0>(args)), words));
                    forced.add(n);
                    break;
                }
            }
        }
    }
    // ... and C that reads a variable nothing in it sets (the compiler's flow check): a value the original took from a register on
    // entry or from a buffer it had filled through a pointer, which the C reads as whatever happens to be there -- right or wrong by
    // where the build happens to put things
    {
        py::list<py::str> left;
        for (const py::str& n : order) {
            if (!forced.contains(n) && stems.contains(n) && texts.contains(n)) {
                left.append(n);
            }
        }
        const auto unset = jobs::pool::threads(
            [&](const py::str& n) {
                return build::functions::never_set(wcc, cflags, env, py::os::path::join(folder, py::percent("s%02d", segment_of.at(n))), n, texts.at(n));
            },
            left, base::parallel::workers());
        for (int64_t i = 0; i < py::len(left); ++i) {
            const py::list<py::str>& names = unset[i];
            if (!names.empty()) {
                evidence(left[i], py::percent("its C reads %s, which it never sets (not trusted as C)", py::join(py::str(", "), names)));
                forced.add(left[i]);
            }
        }
    }
    // (for finding which rebuilt functions make a module misbehave: CB_ASM_ONLY=all rebuilds every function from its own instructions;
    // a comma list, those named; CB_ASM_SEGMENTS=1,3 those in the segments named)
    {
        const py::str only = env_get("CB_ASM_ONLY");
        std::set<int64_t> segs_only;
        for (const py::str& x : py::split(env_get("CB_ASM_SEGMENTS"), py::str(","))) {
            if (!x.strip().empty()) {
                segs_only.insert(py::int_parse(x));
            }
        }
        if (!only.empty() || !segs_only.empty()) {
            std::set<std::string> named_;
            if (only != py::str("all")) {
                for (const py::str& x : py::split(only, py::str(","))) {
                    named_.insert(x.utf8());
                }
            }
            for (const py::str& n : order) {
                if (stems.contains(n) && ((only == py::str("all") || named_.count(n.utf8())) || (segment_of.contains(n) && segs_only.count(segment_of.at(n))))) {
                    forced.add(n);
                }
            }
        }
    }
    for (const py::str& n : _near_pointer_tables(nef, at)) {
        // (its address is a number in the data segment: it must stay where it is)
        if (stems.contains(n) && !forced.contains(n)) {
            forced.add(n);
            evidence(n, py::str("its address is in a table of near code pointers in the data segment (called through it) -- kept as its instructions, at its place"));
        }
    }
    for (const py::str& n : _register_argument_calls(nef, at)) {
        // (a call through a pointer that takes an argument in a register: C cannot say it)
        if (stems.contains(n) && !forced.contains(n)) {
            forced.add(n);
            evidence(n, py::str("it loads AX for a call through a pointer (a VxD's API entry: the service in AX, the arguments read from its frame) -- kept as its instructions"));
        }
    }
    for (const auto& kv : kept_as_asm(b.name).items()) {                      // (C a whole run of the module showed wrong)
        if (stems.contains(std::get<0>(kv))) {
            forced.add(std::get<0>(kv));
            evidence(std::get<0>(kv), py::str("kept as its instructions (build/kept_as_asm.txt): ") + std::get<1>(kv));
        }
    }
    for (const auto& kv : b.gated.items()) {                                  // (C that, run, did not do what the original does)
        if (stems.contains(std::get<0>(kv))) {
            forced.add(std::get<0>(kv));
            evidence(std::get<0>(kv), py::str("its C, run beside the original, differs (not trusted as C): ") + std::get<1>(kv).slice(0, 160));
        }
    }
    for (const py::str& x : py::split(env_get("CB_FORCE_C"), py::str(","))) {    // (and these as C whatever origin says)
        forced.discard(x);
    }
    if (const std::optional<py::str> keep_c = py::os::getenv(py::str("CB_ASM_KEEP_C"))) {   // (and the other way: C for these alone)
        std::set<std::string> kept;
        for (const py::str& x : py::split(*keep_c, py::str(","))) {
            kept.insert(x.utf8());
        }
        for (const py::str& n : order) {
            if (stems.contains(n) && !kept.count(n.utf8())) {
                forced.add(n);
            }
        }
    }
    std::set<std::string> fresh;                                              // (the folders beside whose header is written)

    int64_t pages = 0;
    {
        const py::bytes head = py::file::open(path, py::str("rb")).read(6);
        pages = static_cast<int64_t>(u_at(head, 4, 2));
    }
    const bool loads_itself = pages * 512 >= file_size(path) - 511;
    b.resident_limit = py::dict<int64_t, int64_t>();
    if (loads_itself) {
        // A module that loads itself keeps only part of a segment after start-up: KERNEL cuts its segment 1 to A940h (GlobalReAlloc of
        // CS at 1:B567), the init code past it thrown away. A function from below that point that finds no room below it would go
        // after the frame, past the end -- KRNL386's C fault decoder (1:A296) at 1:C602 faulted on every fault it was to decode
        // ("Invalid DPMI return from 0367:C608"). The packing the frame will do is done here first, and only such a one stays as its
        // instructions; one from past the point (init code) may go after the frame.
        for (const auto& kv : segs.items()) {
            const Segment& s = std::get<1>(kv);
            if (s.kind == py::str("CODE")) {
                const auto lim = _resident_limit(nef.data.slice(s.file_off, s.file_off + s.length));
                if (lim && *lim && *lim < s.length) {
                    b.resident_limit.set(std::get<0>(kv), *lim);
                }
            }
        }
        // (the data such a segment's C reads through CS is linked after its frame: past the cut, thrown away with the init code -- a
        // function that reads it stays as its instructions, which read the original's bytes where they are)
        for (const py::str& n : order) {
            const auto sg = opt_get(segment_of, n);
            if (sg && b.resident_limit.contains(*sg) && stems.contains(n) && !forced.contains(n) &&
                py::re::search(py::percent("\\bcodebreaker_code_%d\\b", *sg), texts.get(n, py::str("")))) {
                forced.add(n);
                evidence(n, py::percent("it reads data beside its code through CS, in a segment cut after start-up (%04x)", b.resident_limit.at(*sg)));
            }
        }
    }
    for (const auto& kv : segs.items()) {
        const Segment& s = std::get<1>(kv);
        // A FIXED code segment is no bigger than the original's: Windows puts a fixed segment as low in memory as it can -- below 1 MB,
        // where every task's PSP and task database must go too. USER's segment 1 grown from BA2Eh to C6DEh with the functions rebuilt
        // from C that found no room in their places and went after the frame, Setup could no longer start the program after its
        // Welcome page: "There is not enough memory to run Setup". (At C63Ch it still could; C778h -- the same build with sub_1_3514's
        // C made 300 bytes longer, nothing else -- could not.) So a function with no room in the frame stays as its instructions.
        // (CB_FIXED_GROWS=1: let them grow, as before)
        if (s.kind == py::str("CODE") && !(s.flags & 0x0010) && env_get("CB_FIXED_GROWS") != py::str("1")) {
            const int64_t cur = b.resident_limit.contains(std::get<0>(kv)) ? b.resident_limit.at(std::get<0>(kv)) : s.length;
            b.resident_limit.set(std::get<0>(kv), std::min(cur, s.length));
        }
    }
    if (!b.resident_limit.empty()) {
        std::map<int64_t, std::vector<std::pair<int64_t, py::str>>> starts_of;
        for (const auto& kv : at.items()) {
            starts_of[std::get<0>(std::get<0>(kv))].push_back({std::get<1>(std::get<0>(kv)), std::get<1>(kv)});
        }
        for (const auto& kv : b.resident_limit.items()) {
            const int64_t sg = std::get<0>(kv), lim = std::get<1>(kv);
            std::vector<std::pair<int64_t, py::str>> ord = starts_of[sg];
            std::sort(ord.begin(), ord.end(), [](const auto& x, const auto& y) { return x.first < y.first || (x.first == y.first && str_less(x.second, y.second)); });
            py::dict<py::str, int64_t> size;
            for (const auto& on : ord) {
                const py::str& nm = on.second;
                if (forced.contains(nm) || !stems.contains(nm) || stems.at(nm).startswith(py::str("asm/"))) {
                    continue;
                }
                try {
                    size.set(nm, std::get<0>(msc6::object_code(read_bytes(py::os::path::join(folder, stems.at(nm) + py::str(".obj"))))).size());
                } catch (const py::OSError&) {
                }
            }
            while (true) {
                std::vector<std::tuple<int64_t, int64_t, HoleName>> holes;
                std::map<std::string, int64_t> own;
                for (size_t k = 0; k < ord.size(); ++k) {
                    if (size.contains(ord[k].second)) {
                        holes.push_back({ord[k].first, k + 1 < ord.size() ? ord[k + 1].first : segs.at(sg).length, HoleName(ord[k].second)});
                        own[ord[k].second.utf8()] = ord[k].first;
                    }
                }
                const Packed packed = _pack_holes(holes, size, m.near, py::set<py::str>(), lim);
                std::vector<py::str> stuck;
                for (const py::str& nm : cat(packed.no_room_far, packed.no_room_near)) {
                    if (own.at(nm.utf8()) < lim) {
                        stuck.push_back(nm);
                    }
                }
                if (stuck.empty()) {
                    break;
                }
                for (const py::str& nm : stuck) {
                    forced.add(nm);
                    size.pop(nm);
                    evidence(nm, lim < segs.at(sg).length
                                     ? py::percent("its C finds no room below where segment %d is cut (%04x) in a module that loads itself", sg, lim)
                                     : py::percent("its C finds no room in fixed segment %d, which must stay the original's size (%04x bytes)", sg, lim));
                }
            }
        }
    }

    std::map<std::string, std::tuple<int64_t, int64_t, py::str>> asm_read;      // (file -> (size, mtime, text): each read once)
    auto asm_texts = [&]() {
        const py::str asm_dir = py::os::path::join(folder, py::str("asm"));
        py::list<py::str> parts;
        if (py::os::path::isdir(asm_dir)) {
            for (const py::str& fn : py::os::listdir(asm_dir)) {
                if (fn.endswith(py::str(".asm"))) {
                    const py::str p = py::os::path::join(asm_dir, fn);
                    std::error_code ec;
                    const auto sz = static_cast<int64_t>(fs::file_size(fs::u8path(p.utf8()), ec));
                    const auto mt = static_cast<int64_t>(fs::last_write_time(fs::u8path(p.utf8()), ec).time_since_epoch().count());
                    auto it = asm_read.find(p.utf8());
                    if (it == asm_read.end() || std::get<0>(it->second) != sz || std::get<1>(it->second) != mt) {
                        asm_read[p.utf8()] = {sz, mt, read_text(p)};
                    }
                    parts.append(std::get<2>(asm_read.at(p.utf8())));
                }
            }
        }
        return py::join(py::str("\n"), parts);
    };
    std::map<size_t, py::set<py::str>> near_seen;
    // The symbols the hand-written code beside reaches near (`near ptr NAME`) -- found again only when the assembly has changed since.
    auto near_reached = [&]() -> py::set<py::str> {
        const py::str text = asm_texts();
        const size_t k = std::hash<std::string>()(text.utf8());
        if (!near_seen.count(k)) {
            near_seen.clear();
            py::set<py::str> found;
            for (const py::str& s : near_ptr.findall(text)) {
                found.add(s);
            }
            near_seen[k] = found;
        }
        return near_seen.at(k);
    };

    // A far function rebuilt from C, compiled into a code segment of its own beside its segment (CODE<n>X) -- for one that does not
    // fit back into its place. -> stem or None. Not in a module that loads itself (KRNL386: DOS loads the whole file and its own start
    // sets up its segments and entries -- only the ones the original has; an export moved to a segment of ours is a "Call to Undefined
    // Dynalink").
    const StemOf elsewhere = [&](const py::str& name) -> std::optional<py::str> {
        const auto seg = opt_get(segment_of, name);
        if (!seg || m.near.contains(name) || loads_itself) {
            return std::nullopt;
        }
        const py::str text = texts.at(name);
        bool calls_near = false;
        for (const py::str& c : call_re.findall(after_brace(text))) {
            if (m.near.contains(c)) {
                calls_near = true;
            }
        }
        if (calls_near || text.contains(py::str("code __near *")) || text.contains(py::str("codebreaker_cs()")) ||
            py::re::search("\\bcodebreaker_code_\\d+\\b", text)) {
            return std::nullopt;                               // it calls near ones, which only its own segment reaches (or reads its segment's own bytes through CS)
        }
        const py::str sym = public_.contains(name) ? std::get<0>(public_.at(name)) : py::str("_") + name;
        if (near_reached().contains(sym)) {
            return std::nullopt;                               // hand-written code beside it reaches it near
        }
        const py::str sub = py::os::path::join(folder, py::percent("s%02dx", *seg));
        py::os::makedirs(sub, 0777, true);
        if (!fresh.count(sub.utf8())) {                        // (the header beside the function, as in its own folder -- this build's, not one left from before)
            fresh.insert(sub.utf8());
            write_text(py::os::path::join(sub, py::str("m.h")), mheader);
            build::functions::set_header_key(sub, base::cache::key(Cs{mheader}));
        }
        const py::str stem = py::os::path::basename(stems.at(name));
        const py::str why = build::functions::_compile(wcc, cat(cflags, py::list<py::str>{py::percent("-nt=CODE%dX", *seg), py::str("-nc=XCODE")}), env, sub, stem, text);
        if (!why.empty()) {
            b.elsewhere_why.set(name, why);
        }
        return why.empty() ? std::optional<py::str>(py::percent("s%02dx/%s", *seg, stem)) : std::nullopt;
    };

    b.near_names = py::set<py::str>();
    for (const py::str& n : m.near) {
        b.near_names.add(n);                                   // (for placing them: a near one stays in its segment)
    }
    const ImportMap asm_imports = _assembled16(b, nef, at, segs, stems, public_, folder, env, m.segments, forced, elsewhere);
    for (const auto& kv : m.failed.items()) {                  // no C, and not rebuilt from its instructions
        if (!b.assembled.contains(std::get<0>(kv))) {
            if (!b.rejected.contains(std::get<0>(kv))) {
                b.rejected.set(std::get<0>(kv), std::get<1>(kv));
            }
        }
    }
    const bool library = build::definitions::is_library(path);
    py::list<py::str> lines{py::percent("format windows%s", library ? py::str(" dll") : py::str("")), py::str("name m.dll"),
                            py::str("option quiet, map, nodefaultlibs, caseexact, packcode=1, packdata=0"),
                            py::percent("option modname='%s'", build::definitions::module_name(path))};
    if (nef.h.heap) {
        lines.append(py::percent("option heapsize=%d", nef.h.heap));
    }
    if (nef.h.stack) {                                         // (the linker refuses a size of 0)
        lines.append(py::percent("option stack=%d", nef.h.stack));
    }
    if (!nef.h.autodata_seg) {
        // (no automatic data segment in the original -- WIN87EM, SYSTEM.DRV: their functions run on the caller's DS. The build's own
        // little DGROUP made one, and the loader set it up)
        lines.append(py::str("option noautodata"));
    }
    if (!build::definitions::description(path).empty()) {
        lines.append(py::str("option description '") + build::definitions::description(path).replace(py::str("'"), py::str(" ")) + py::str("'"));
    }
    // what the original's header says of it, as its linker was told: the Windows version it expects (3.10: Windows treats a 3.0 module
    // the old way), protected mode only, and no application type (KERNEL's)
    {
        const py::bytes raw = py::file::open(path, py::str("rb")).read(0x10000);
        const int64_t ne_at = static_cast<int64_t>(u_at(raw, 0x3C, 4));
        const int64_t expver = static_cast<int64_t>(u_at(raw, ne_at + 0x3E, 2)), info = static_cast<int64_t>(u_at(raw, ne_at + 0x0C, 2));
        if (expver) {
            lines.append(py::percent("option version=%d.%d", expver >> 8, expver & 0xFF));
        }
        if (info & 0x0008) {
            lines.append(py::str("option protmode"));
        }
        if (!(info & 0x0300)) {
            lines.append(py::str("option noapptype"));
        }
    }
    lines.append(py::percent("libpath %s", build::watcom::ow_lib(py::str("ne"))));   // lib286/win: the real library, not the stub above it
    const int64_t cs = nef.h.csip >> 16, ip = nef.h.csip & 0xFFFF;
    const auto entry = cs ? opt_get(at, Place{cs, ip}) : std::nullopt;
    if (entry && public_.contains(*entry)) {
        lines.append(py::percent("option start=%s", quote_symbol(std::get<0>(public_.at(*entry)))));
        b.entry = py::percent("%s, the function at the original's entry point", *entry);
    } else if (cs) {
        extra(py::str("entry"), py::str("int2 __far __stdcall codebreaker_entry()\n{\n  return 1;\n}\n"));
        lines.append(py::str("option start='_codebreaker_entry'"));
        b.entry = py::percent("a stand-in that reports success (the original's entry point %d:%04x has no reconstruction)", cs, ip);
    } else {
        b.entry = py::str("none, as in the original");
    }
    lines.append(py::str("file data.obj"));                    // first: the data segment starts with it
    py::set<py::str> used_c;
    for (const py::str& n : b.placed_c) {
        used_c.add(n);
    }
    for (const py::str& n : b.moved_c) {
        used_c.add(n);
    }
    bool uses_interrupts = false;
    for (const py::str& n : used_c) {
        if (mtexts.contains(n) && mtexts.at(n).contains(py::str("codebreaker_int"))) {
            uses_interrupts = true;
        }
    }
    if (!m.interrupts.empty() && uses_interrupts) {
        // the interrupts the module's C takes (none, with every function rebuilt from its instructions: then nothing is added to the
        // original's segments)
        py::set<int64_t> vectors;
        for (const auto& i : m.interrupts) {
            vectors.add(i.vector);
        }
        const auto ac = ct::interrupt_routines(vectors);
        write_text(py::os::path::join(folder, py::str("interrupts.asm")), std::get<0>(ac));
        const auto r = build::functions::run_process(py::list<py::str>{wasm_tool, py::str("-q"), py::str("interrupts.asm")}, folder, env);
        const py::str why = extra(py::str("interrupt_registers"), std::get<1>(ac).replace(py::str("#include \"m.h\"\n"), py::str("")));
        if (std::get<0>(r) == 0 && why.empty()) {
            lines.append(py::str("file interrupts.obj"));
            lines.append(py::str("file interrupt_registers.obj"));
        } else {
            const py::str said = !std::get<1>(r).empty() ? std::get<1>(r) : std::get<2>(r);
            b.error = py::percent("the interrupt routines: %s", !why.empty() ? why : strip_tail(said, 200));
        }
    }
    for (const py::str& stem : b.lead_ins) {                   // each code segment starts with its lead-in
        lines.append(py::str("file ") + stem + py::str(".obj"));
    }
    {
        std::set<std::string> lead_set;
        for (const py::str& s : b.lead_ins) {
            lead_set.insert(s.utf8());
        }
        std::set<std::string> seen;
        for (const py::str& n : order) {                       // (a run of functions written in assembly is one object)
            if (!stems.contains(n)) {
                continue;
            }
            const py::str s = stems.at(n);
            if (seen.insert(s.utf8()).second && !lead_set.count(s.utf8())) {
                lines.append(py::str("file ") + s + py::str(".obj"));
            }
        }
    }
    // the original's data that the C of a segment reads through CS (c_text: codebreaker_code_<n>), after the segment's code -- for a
    // segment with any such function rebuilt from C
    b.code_data = py::list<std::tuple<int64_t, py::list<py::str>>>();
    {
        std::vector<std::pair<int64_t, py::str>> cd;
        for (const auto& kv : m.code_data.items()) {
            cd.push_back({std::get<0>(kv), std::get<1>(kv)});
        }
        std::sort(cd.begin(), cd.end(), [](const auto& x, const auto& y) { return x.first < y.first; });
        for (const auto& st : cd) {
            const int64_t seg = st.first;
            py::list<py::str> readers;
            if (by_segment.count(seg)) {
                const py::re::Pattern reads = py::re::compile(py::percent("\\bcodebreaker_code_%d\\b", seg));
                for (const py::str& n : by_segment.at(seg)) {
                    if (stems.contains(n) && !stems.at(n).startswith(py::str("asm/")) && reads.search(texts.get(n, py::str("")))) {
                        readers.append(n);
                    }
                }
            }
            if (readers.empty()) {
                continue;
            }
            const py::str why = build::functions::_compile(wcc, cat(cflags, py::list<py::str>{py::percent("-nt=CODE%d", seg)}), env, folder, py::percent("code%02d", seg), st.second);
            if (!why.empty()) {
                if (b.error.empty()) {
                    b.error = py::percent("the data code segment %d's C reads through CS: %s", seg, why);
                }
            } else {
                lines.append(py::percent("file code%02d.obj", seg));
                b.code_data.append({seg, readers});
            }
        }
    }
    if (cs && !(entry && public_.contains(*entry))) {
        lines.append(py::str("file entry.obj"));
    }
    static const std::tuple<int64_t, const char*, const char*> segment_options[] = {
        {0x40, "preload", "loadoncall"}, {0x10, "moveable", "fixed"}, {0x1000, "discardable", ""}, {0x20, "shared", "nonshared"}};
    for (const auto& sn : by_segment) {                        // each code segment loaded as the original's was
        const int64_t seg = sn.first;
        const int64_t flags = segs.contains(seg) ? segs.at(seg).flags : 0x1D10;
        py::list<py::str> opts;
        for (const auto& o : segment_options) {
            const char* pick = (flags & std::get<0>(o)) ? std::get<1>(o) : std::get<2>(o);
            if (*pick) {
                opts.append(py::str(pick));
            }
        }
        lines.append(py::percent("segment 'CODE%d' %s", seg, py::join(py::str(" "), opts)));
    }
    if (segs.contains(auto_)) {                                // the data segment, loaded as the original's
        const int64_t flags = segs.at(auto_).flags;
        py::list<py::str> how;
        for (int i = 0; i < 2; ++i) {
            how.append(py::str((flags & std::get<0>(segment_options[i])) ? std::get<1>(segment_options[i]) : std::get<2>(segment_options[i])));
        }
        lines.append(py::percent("segment class 'DATA' %s", py::join(py::str(" "), how)));
        lines.append(py::percent("segment class 'BSS' %s", py::join(py::str(" "), how)));
    }
    // the original's DOS part (its stub -- KRNL386's is its real-mode start, which finds the NE header by its own length and runs from
    // the whole file DOS loads)
    const auto stub = _dos_stub(path, folder);
    if (stub) {
        lines.append(py::str("option stub=") + *stub);
        lines.append(py::str("option stubverbatim"));
    }
    // its file alignment, and -- where its loader fixes are all by segment number, chained, as the original linker wrote KRNL386's
    // (which relocates itself, before there is an entry table to go through) -- ours the same
    lines.append(py::percent("option alignment=%d", int64_t(1) << (nef.h.align_shift ? nef.h.align_shift : 9)));
    if (_segment_number_fixups(nef)) {
        lines.append(py::str("option selectorfixups"));
    }
    // ... and where the original's was: its code segments first, by their numbers (segment n is the original's segment n -- what reaches
    // one by its number finds it), what the build adds (CODE<n>X, the runtime's _TEXT) after them
    std::vector<int64_t> code_numbers, data_numbers;
    for (const Segment& s : nef.segments()) {
        (s.kind == py::str("CODE") ? code_numbers : data_numbers).push_back(s.idx);
    }
    // (data_numbers is the ones that are not code)
    bool all_rebuilt = !code_numbers.empty();
    for (int64_t n : code_numbers) {
        if (!by_segment.count(n)) {
            all_rebuilt = false;
        }
    }
    if (all_rebuilt) {
        py::list<py::str> segs_text;
        for (int64_t n : code_numbers) {
            segs_text.append(py::percent("segment CODE%d", n));
        }
        py::str order_line = py::str("order clname MCODE ") + py::join(py::str(" "), segs_text);
        // the data segments after them, each at its number as there: the automatic one (its classes) and those carried as they were
        // (CODEBREAKER_SEGMENT_<n>, class FAR_DATA). Anything the build adds goes after all of the original's -- a runtime helper's code
        // put before a data segment renumbers it and every selector after it (USER's segment 44, its second data segment, became 46
        // behind the long-division routine)
        const int64_t max_code = *std::max_element(code_numbers.begin(), code_numbers.end());
        if (!data_numbers.empty() && *std::min_element(data_numbers.begin(), data_numbers.end()) == max_code + 1) {
            std::vector<int64_t> far_;
            for (int64_t n : data_numbers) {
                if (n != auto_) {
                    far_.push_back(n);
                }
            }
            bool in_data = false;
            for (int64_t n : data_numbers) {
                if (n == auto_) {
                    in_data = true;
                }
            }
            bool all_below = true, all_above = true;
            for (int64_t n : far_) {
                all_below = all_below && n < auto_;
                all_above = all_above && n > auto_;
            }
            if (far_.empty() || (std::is_sorted(far_.begin(), far_.end()) && (!in_data || all_below || all_above))) {
                py::list<py::str> pieces;
                for (int64_t n : data_numbers) {
                    if (n == auto_) {
                        pieces.append(py::str("clname BEGDATA clname DATA clname BSS clname STACK"));
                    } else if (n == far_[0]) {
                        py::list<py::str> f;
                        for (int64_t k : far_) {
                            f.append(py::percent("segment CODEBREAKER_SEGMENT_%d", k));
                        }
                        pieces.append(py::str("clname FAR_DATA ") + py::join(py::str(" "), f));
                    }
                }
                order_line = order_line + py::str(" ") + py::join(py::str(" "), pieces);
            }
        }
        lines.append(order_line + py::str(" clname XCODE clname CODE"));    // (the runtime's CODE last)
    } else {
        py::list<py::str> missing;
        for (int64_t n : code_numbers) {
            if (!by_segment.count(n)) {
                missing.append(py::to_str(n));
            }
        }
        b.layout = py::percent("segments renumbered: the original's code segments [%s] are not all rebuilt", py::join(py::str(", "), missing));
    }
    const auto res = _resources(b, path, folder);
    if (res && !res->empty()) {
        lines.append(py::str("option resource=") + py::os::path::basename(*res));
    }
    ImportMap imports;
    for (const auto& kv : m.imports.items()) {
        imports.set(std::get<0>(kv), {std::get<0>(std::get<1>(kv)), Which(std::get<1>(std::get<1>(kv)))});
    }
    for (const auto& kv : d->imports.items()) {
        if (!imports.contains(std::get<0>(kv))) {
            imports.set(std::get<0>(kv), {std::get<0>(std::get<1>(kv)), Which(std::get<1>(std::get<1>(kv)))});
        }
    }
    for (const auto& kv : asm_imports.items()) {
        if (!imports.contains(std::get<0>(kv))) {
            imports.set(std::get<0>(kv), std::get<1>(kv));
        }
    }
    {
        // a C name compiled as __stdcall with an open argument list is the symbol `_name`
        std::vector<std::pair<py::str, std::tuple<py::str, Which>>> items;
        for (const auto& kv : imports.items()) {
            items.push_back({std::get<0>(kv), std::get<1>(kv)});
        }
        std::sort(items.begin(), items.end(), [](const auto& x, const auto& y) { return str_less(x.first, y.first); });
        for (const auto& it : items) {
            const Which& what = std::get<1>(it.second);
            lines.append(py::percent("import _%s %s.%s", it.first, std::get<0>(it.second),
                                     std::holds_alternative<int64_t>(what) ? py::to_str(std::get<int64_t>(what)) : quote_symbol(std::get<py::str>(what))));
        }
    }

    // The module's constants -- entries of segment 0xFE, KERNEL's __AHINCR, __0040H and the like: values, not places. Each is a public
    // EQU in constants.asm, which the linker exports as a constant entry.
    py::dict<int64_t, build::definitions::Export> constants;
    for (const auto& e : build::definitions::exports(path)) {
        if (std::holds_alternative<Place>(e.address) && std::get<0>(std::get<Place>(e.address)) == 0xFE) {
            constants.set(e.ordinal, e);
        }
    }
    auto const_symbol = [&](const build::definitions::Export& e) {
        return e.name && c_name.fullmatch(*e.name) ? *e.name : py::percent("constant_%d", e.ordinal);
    };
    if (!constants.empty()) {
        py::list<py::str> src{py::percent("; %s -- the values it exports as constants (entry segment 0FEh)", build::definitions::module_name(path)), py::str("")};
        std::vector<int64_t> ords = constants.keys();
        std::sort(ords.begin(), ords.end());
        for (int64_t o : ords) {
            const build::definitions::Export& e = constants.at(o);
            src.append(py::str("public ") + const_symbol(e));
            src.append(py::percent("%s equ 0%Xh", const_symbol(e), std::get<1>(std::get<Place>(e.address))));
        }
        src.append(py::str("end"));
        src.append(py::str(""));
        write_text(py::os::path::join(folder, py::str("constants.asm")), py::join(py::str("\n"), src));
        const auto r = build::functions::run_process(py::list<py::str>{wasm_tool, py::str("-q"), py::str("constants.asm")}, folder, env);
        if (std::get<0>(r) == 0) {
            lines.append(py::str("file constants.obj"));
        } else {
            constants = py::dict<int64_t, build::definitions::Export>();
        }
    }

    auto symbol_at = [&](const build::definitions::Export& e) -> std::optional<py::str> {
        if (constants.contains(e.ordinal)) {
            return const_symbol(e);
        }
        if (std::holds_alternative<Place>(e.address) && d->exported.contains(std::get<Place>(e.address))) {   // exported data: its part of the segment
            return py::str("_") + d->exported.at(std::get<Place>(e.address));
        }
        const auto fn = std::holds_alternative<Place>(e.address) ? opt_get(at, std::get<Place>(e.address)) : std::nullopt;
        return fn && public_.contains(*fn) ? std::optional<py::str>(std::get<0>(public_.at(*fn))) : std::nullopt;
    };
    {
        const auto ex = build::definitions::export_directives(path, symbol_at);
        for (const py::str& l : std::get<0>(ex)) {
            lines.append(l);
        }
        b.exports_missing = std::get<1>(ex);
    }
    lines.append(py::str("library clibm.lib"));
    const auto stand_in = [&](const py::str& s) { return _from_header(mheader, s); };
    const auto compile_extra = [&](const py::str& stem, const py::str& text) { return extra(stem, text); };
    std::optional<py::str> image = _link_resolving(b, wlink, env, folder, lines, stand_in, compile_extra);
    // As the function-by-function build does (build/functions.py): a function that reaches near for something in another segment (the
    // original called it far; the text has lost that) cannot be linked, and a segment whose rebuilt functions come to more than 64K cannot
    // be either. Those functions become stand-ins -- the largest first, for a segment too big -- and the module is linked again.
    py::dict<py::str, py::str> by_stem;
    for (const auto& kv : stems.items()) {
        by_stem.set(std::get<1>(kv), std::get<0>(kv));
    }
    for (int attempt = 0; attempt < 120; ++attempt) {
        if (image) {
            break;
        }
        const auto apart = file_apart.search(b.error);
        const auto over = size_over.search(b.error);
        std::vector<std::pair<py::str, py::str>> swap;
        if (apart && by_stem.contains(apart->group(1)) && !b.rejected.contains(by_stem.at(apart->group(1)))) {
            py::str line("");
            for (const py::str& ln : py::splitlines(b.error)) {
                if (ln.contains(apart->group(1))) {
                    line = ln;
                    break;
                }
            }
            swap.push_back({by_stem.at(apart->group(1)), py::str("reaches near for something in another segment: ") + strip_tail(line, 160)});
        } else if (over) {
            const int64_t seg = py::int_parse(over->group(1)), need = py::int_parse(over->group(2)) + 512;
            int64_t freed = 0;
            // First the far ones, largest first, go to the segment beside (CODE<n>X): they are called far, from anywhere. Only what that
            // cannot free becomes stand-ins.
            bool moved_now = false;
            std::vector<std::pair<int64_t, py::str>> cands;
            if (by_segment.count(seg)) {
                for (const py::str& n : by_segment.at(seg)) {
                    if (stems.contains(n) && !b.rejected.contains(n) && !m.near.contains(n) && !b.moved_c.contains(n) && !b.assembled.contains(n) &&
                        !stems.at(n).startswith(py::str("asm/"))) {
                        cands.push_back({file_size(py::os::path::join(folder, stems.at(n) + py::str(".obj"))), n});
                    }
                }
            }
            std::sort(cands.begin(), cands.end(), [](const auto& x, const auto& y) { return x.first > y.first || (x.first == y.first && str_less(y.second, x.second)); });
            for (const auto& sn : cands) {
                if (freed >= need) {
                    break;
                }
                const py::str old = py::str("file ") + stems.at(sn.second) + py::str(".obj");
                int64_t idx = -1;
                for (int64_t i = 0; i < py::len(lines); ++i) {
                    if (lines[i] == old) {
                        idx = i;
                        break;
                    }
                }
                if (idx < 0) {
                    continue;
                }
                const auto moved = elsewhere(sn.second);
                if (moved && !moved->empty()) {
                    lines[idx] = py::str("file ") + *moved + py::str(".obj");
                    stems.set(sn.second, *moved);
                    b.moved_c.add(sn.second);
                    freed += sn.first;
                    moved_now = true;
                }
            }
            if (moved_now) {
                b.error = py::str("");
                b.absent = py::list<py::str>();
                image = _link_resolving(b, wlink, env, folder, lines, stand_in, compile_extra);
                continue;
            }
            cands.clear();
            if (by_segment.count(seg)) {
                for (const py::str& n : by_segment.at(seg)) {
                    if (stems.contains(n) && !b.rejected.contains(n)) {
                        cands.push_back({file_size(py::os::path::join(folder, stems.at(n) + py::str(".obj"))), n});
                    }
                }
            }
            std::sort(cands.begin(), cands.end(), [](const auto& x, const auto& y) { return x.first > y.first || (x.first == y.first && str_less(y.second, x.second)); });
            for (const auto& sn : cands) {
                if (freed >= need) {
                    break;
                }
                swap.push_back({sn.second, py::str("too large: the functions of its segment do not fit in 64K when rebuilt")});
                freed += sn.first;
            }
        }
        if (swap.empty()) {
            break;
        }
        for (const auto& nw : swap) {
            const py::list<py::str> sub_stem = py::split(stems.at(nw.first), py::str("/"));
            if (!build::functions::_compile(wcc, cat(cflags, py::list<py::str>{py::percent("-nt=CODE%d", segment_of.at(nw.first))}), env,
                                            py::os::path::join(folder, sub_stem[0]), sub_stem[1], build::functions::stand_in(texts.at(nw.first))).empty()) {
                return image;                                  // not even a stand-in compiles: give up
            }
            b.rejected.set(nw.first, nw.second);
        }
        b.error = py::str("");
        b.absent = py::list<py::str>();
        image = _link_resolving(b, wlink, env, folder, lines, stand_in, compile_extra);
    }
    b.stand_in_addresses = py::set<Address>();
    for (const auto& kv : by_va.items()) {
        if (b.rejected.contains(std::get<1>(kv)) && m.where.contains(std::get<0>(kv)) && m.where.at(std::get<0>(kv))) {
            b.stand_in_addresses.add(Address(*m.where.at(std::get<0>(kv))));
        }
    }
    if (image && b.data_error.empty()) {                       // is the data where the code expects it?
        py::str text("");
        try {
            text = read_text(py::os::path::join(folder, py::str("m.map")));
        } catch (const py::OSError&) {
            text = py::str("");
        }
        const auto group = py::re::search("^DGROUP\\s+([0-9a-fA-F]{4}):([0-9a-fA-F]{4})", text, py::re::M);
        const auto mine = py::re::search("^([0-9a-fA-F]{4}):([0-9a-fA-F]{4})[+*]?\\s+_codebreaker_data\\s*$", text, py::re::M);
        if (!(group && mine && group->group(1) == mine->group(1) && group->group(2) == mine->group(2))) {
            b.data_error = py::str("the data did not land at the start of the automatic data segment");
        }
    }
    // (each export declared as the back end wrote its function's heading from L2: m.declarations)
    _write_definitions(path, folder, [&](const build::definitions::Export& e) -> std::optional<py::str> {
        if (!std::holds_alternative<Place>(e.address)) {
            return std::nullopt;
        }
        const auto fn = opt_get(at, std::get<Place>(e.address));
        if (!fn) {
            return std::nullopt;
        }
        return opt_get(m.declarations, *fn);
    });
    // the module as one L2 tree, whatever each function came back as (docs/IR_UNIFICATION.md step 4)
    {
        py::dict<int64_t, int64_t> lengths;
        for (const auto& kv : segs.items()) {
            lengths.set(std::get<0>(kv), std::get<1>(kv).length);
        }
        b.l2 = module_document(b.name, m, b.assembled, stems, folder, lengths);
    }
    write_json(py::os::path::join(folder, py::str("module.l2.json")), b.l2);
    return image;
}

// ---- what both kinds share ------------------------------------------------------------------------------------------
// Compare the linked image with the original: format, exports, imports, resources.
Built& check(Built& b) {
    const py::str orig = b.original;
    const py::str image = *b.image;
    b.format_same = formats::detect::format_of(image) == formats::detect::format_of(orig) &&
                    build::definitions::is_library(image) == build::definitions::is_library(orig);
    py::dict<int64_t, build::definitions::Export> mine;
    for (const auto& e : build::definitions::exports(image)) {
        mine.set(e.ordinal, e);
    }
    const auto theirs = build::definitions::exports(orig);
    b.exports_total = py::len(theirs);
    for (const auto& e : theirs) {
        const auto got = opt_get(mine, e.ordinal);
        // an export that has no name in the original cannot be made without one by this linker
        if (got && (got->name == e.name || (!e.name && got->name && *got->name == py::percent("ordinal_%d", e.ordinal)))) {
            b.exports_same += 1;
            if (b.stand_in_addresses.contains(e.address)) {
                b.exports_on_stand_ins += 1;
            }
        } else {
            bool listed = false;
            for (const auto& x : b.exports_missing) {
                if (std::get<0>(x).ordinal == e.ordinal) {
                    listed = true;
                }
            }
            if (!listed) {
                b.exports_missing.append({e, py::percent("in the build as %s", py::repr(got ? got->name : std::optional<py::str>()))});
            }
        }
    }
    const auto want = build::definitions::imports(orig), have = build::definitions::imports(image);
    b.imports_original = py::len(want);
    b.imports_built = py::len(have);
    {
        std::vector<std::pair<py::str, build::definitions::Import>> extra;
        for (const auto& i : have) {
            if (!want.contains(i)) {
                extra.push_back({py::to_str(i), i});
            }
        }
        std::sort(extra.begin(), extra.end(), [](const auto& x, const auto& y) { return str_less(x.first, y.first); });
        b.imports_extra = py::list<build::definitions::Import>();
        for (const auto& e : extra) {
            b.imports_extra.append(e.second);
        }
    }
    const auto original = formats::resources::read(orig);
    if (!original.empty()) {
        const auto r = build::resources::compare(original, formats::resources::read(image), formats::detect::wide_resources(b.kind) ? 1 : 0);
        b.resources_same = r.same();
        b.resources_line = py::percent("%d of %d the same in the image (%d identical, %d equivalent)", r.identical + r.equivalent, r.total, r.identical, r.equivalent);
        const auto& w = b.resources;
        if (w && w->error.empty()) {
            b.resources_line = b.resources_line + py::percent("; written as text %d, image files %d, opaque payload %d, fallback payload %d", w->as_text, w->as_files, w->opaque, w->fallback);
        } else if (w) {
            b.resources_line = b.resources_line + py::percent("; NOT COMPILED: %s", w->error);
        }
    }
    return b;
}

namespace {

// Load a module in the emulator and call one exported function with zero arguments: the platform's emulator does it
// (codebreaker/platforms: emulator.call). -> (how it ended, its result)
std::tuple<py::str, std::optional<int64_t>> _call(const py::str& path, const std::optional<py::str>& kind, const build::definitions::Export& e) {
    if (kind && *kind == py::str("pe")) {
        emulate::emulator::TraceOptions o;
        o.args = py::list<int64_t>{0, 0, 0, 0};
        o.mem_init = _sections(path);
        o.stub_externals = true;
        o.sig_resolver = [](const py::str& dll, const py::str& name) { return verify::lanes::win32::_argc(dll, name); };
        try {
            const emulate::run::Run r = emulate::emulator::recorded(path, emulate::emulator::FuncSelector(static_cast<int64_t>(std::get<uint64_t>(e.address))), o);
            std::optional<int64_t> eax;
            if (r.results.contains(py::str("EAX")) && std::holds_alternative<int64_t>(r.results.at(py::str("EAX")))) {
                eax = std::get<int64_t>(r.results.at(py::str("EAX")));
            }
            return {r.ended(), eax};
        } catch (const py::ImportError&) {
            throw;
        } catch (const py::Exception& ex) {                    // the emulator giving up is a result too
            return {py::str("error: ") + py::str(ex.type()), std::nullopt};
        }
    }
    if (kind && *kind == py::str("ne")) {
        return platforms::win16::call(path, e.ordinal, [](const py::str& p, int64_t ordinal, const py::list<int64_t>& args) {
            emulate::windows16::ProbeOptions o;
            o.ordinal = ordinal;
            o.args = args;
            const emulate::run::Run r = emulate::windows16::record(emulate::windows16::probe(p, o));
            platforms::win16::RunRecord rec;
            rec.ended = r.ended();
            for (const auto& kv : r.results.items()) {
                if (std::holds_alternative<int64_t>(std::get<1>(kv))) {
                    rec.results.set(std::get<0>(kv), std::get<int64_t>(std::get<1>(kv)));
                }
            }
            return rec;
        });
    }
    throw py::NotImplementedError("a module of kind " + (kind ? kind->utf8() : std::string("None")) + " is not called here");
}

// What links a module of this kind: its platform's packager (_build16, _build32).
void _linker_check(const Built& b) {
    if (!b.kind || (*b.kind != py::str("ne") && *b.kind != py::str("pe"))) {
        throw py::SystemExit(py::percent("%s: a %s module is not linked whole here", b.name, b.kind ? *b.kind : py::str("None")));
    }
}

py::str doc_text() {
    return py::str(
        "module.py -- build a whole module from its reconstruction, and check what came out.\n"
        "\n"
        "    python -m codebreaker build <module> [--out DIR] [--no-run] [--engine]\n"
        "\n"
        "    from codebreaker.build import module\n"
        "    b = module.build(\"lzexpand.dll\")          # -> Built; b.image is the linked file\n"
        "    print(b.report())\n"
        "\n"
        "The per-function builds (build/functions.py) make a throw-away DLL to judge functions in: it\n"
        "has no data, no resources, no entry table of its own. This makes the module: the same\n"
        "decompiled functions, compiled the same way (functions.compile_all -- there is one compile\n"
        "path), plus the module's data as C (build/data.py), its resources compiled from `.rc`\n"
        "(build/resources.py) and its exports and imports (build/definitions.py), linked by OpenWatcom\n"
        "into a 16-bit NE or 32-bit PE image. Then the image is checked against the original:\n"
        "\n"
        "    format      the same kind of file, library or program as the original\n"
        "    exports     every original export present, under the same ordinal and name\n"
        "    imports     nothing imported that the original does not import\n"
        "    resources   the same resources (build/resources.compare)\n"
        "    runs        loaded in the emulator, each exported function is called with zero arguments\n"
        "                and what it did is set beside what the original does\n"
        "\n"
        "What \"built\" means here, plainly: a function the compiler rejected is in the image as a\n"
        "stand-in that returns 0, and so is anything called that has no reconstruction; a module can\n"
        "link, export everything and still do little. The report counts both. The check is of the\n"
        "container, not of behaviour -- behaviour is what verify/ judges, one function at a time.\n"
        "\n"
        "Toolchain facts this relies on (each cost a failed build once):\n"
        "\n"
        "  * OpenWatcom's tools are started by full path: Windows searches the caller's PATH, not the\n"
        "    environment handed to the tool.\n"
        "  * 16-bit: the C library to link is lib286/win/clib?.lib (the same file as the source tree's\n"
        "    bld/clib/library/windows.086/m?/clib?.lib). lib286/clibl.lib one level up is an empty\n"
        "    stub, and it is found first if lib286 is on the library path before lib286/win.\n"
        "  * 16-bit: floating point needs -fpi87 (the emulator library lacks the fix-up symbols -fpi\n"
        "    asks for, and -fpc does not work in this build).\n"
        "  * 16-bit: a module is a library because the linker is told `format windows dll` -- that sets\n"
        "    the header's library flag without the stock start-up object, which wants a LIBMAIN. The\n"
        "    header flag, not the file name, says what the original was.\n"
        "  * 16-bit: one segment holds 64 KB, and a near call cannot leave its segment, so each function\n"
        "    is compiled into a code segment named for the original segment it came from (-nt=). The\n"
        "    compiler's own helpers (32-bit multiply and divide, ...) then cannot be near either: the\n"
        "    functions are compiled for the medium model (-mm: far code, near data -- the per-function\n"
        "    builds use -ms, which is the same for data) and linked with the medium-model library.\n"
        "  * 16-bit: the data object is linked first, so it starts at offset 0 of the automatic data\n"
        "    segment and the code's offsets into it stay right.\n"
        "  * Linker symbols holding `@`, `?` or `$` are quoted (unquoted, `@` means \"read a file\").\n"
        "  * Exports are tied to the function at the export's address, and the entry point to the\n"
        "    function at the original's entry address; a library with no entry point is not loadable\n"
        "    as one, so one whose entry was not reconstructed gets a stand-in that reports success.\n");
}

}  // namespace

// Call the built module's exported functions in the emulator, and the original's beside.
Built& run(Built& b, int64_t limit) {
    py::dict<int64_t, build::definitions::Export> mine;
    for (const auto& e : build::definitions::exports(*b.image)) {
        if (e.kind == py::str("code")) {
            mine.set(e.ordinal, e);
        }
    }
    std::vector<build::definitions::Export> todo;
    for (const auto& x : build::definitions::exports(b.original)) {
        if (x.kind == py::str("code") && mine.contains(x.ordinal)) {
            todo.push_back(x);
        }
    }
    for (size_t i = 0; i < todo.size() && static_cast<int64_t>(i) < limit; ++i) {
        const auto& e = todo[i];
        const auto got = _call(*b.image, b.kind, mine.at(e.ordinal)), want = _call(b.original, b.kind, e);
        b.ran.append({e.name && !e.name->empty() ? *e.name : py::percent("@%d", e.ordinal), std::get<0>(got), std::get<0>(want), got == want});
    }
    return b;
}

// [(name, who the callers say removes the arguments)] for each function the description says otherwise of -- the original's own
// calls are the evidence (describe.callers_say). (A report, not the build: never raises; a failure is one pair ("(not checked)", message).)
py::list<std::tuple<py::str, py::str>> _disputes(const decompile::describe::Description& d) {
    py::list<std::tuple<py::str, py::str>> out;
    try {
        const py::dict<SegOff, py::str> say = desc::callers_say(d);
        for (const py::str& line : py::split(read_text(d.path), py::str("\n"))) {
            const py::list<py::str> p = py::split(line.rstrip(py::str("\n")), py::str("\t"));
            if (p[0] != py::str("F") || py::len(p) < 4) {
                continue;
            }
            const auto who = opt_get(say, desc::where(py::int_parse(p[1], 16)));
            if (who && !who->empty() && *who != (py::int_parse(p[3]) > 0 ? py::str("callee") : py::str("caller"))) {
                out.append({p[2], *who});
            }
        }
        return out;
    } catch (const std::exception& e) {                       // (a report, not the build: never stops it)
        py::list<std::tuple<py::str, py::str>> failed;
        failed.append({py::str("(not checked)"), py::str(e.what()).slice(0, 80)});
        return failed;
    }
}

namespace {

// the keys of the gate's verdicts: what each is made of (the original module, the checker's tools, the header, the function's text, what the
// description says of it and what it calls)
py::dict<py::str, py::str> _gate16_keys(const py::str& path, const py::list<py::str>& names, const decompile::c_text::Module16& m) {
    static const py::re::Pattern call_re = py::re::compile("\\b([A-Za-z_]\\w*)\\s*\\(");
    const py::str module_key = base::cache::key(Cs{read_bytes(path)});
    const py::bytes sizes = symbols::api_arguments::sizes16();      // (what each Windows call removes: the checker answers calls by it)
    // (and the branch-guided inputs the checker runs: the explorers, their limits, whether they are on -- the path explorer's lifter too)
    const bool ready = verify::lanes::win16::concolic_ready();
    Cs parts{base::cache::tool(build::watcom::ow_tool(py::str("wcc"))), build::functions::CFLAGS16, sizes, verify::lane_common::guided_on(),
             verify::lane_common::GUIDED_BUDGET, verify::lane_common::MAX_GUIDED, ready, verify::lane_common::PATH_BUDGET,
             verify::lane_common::MAX_PATHS};
    if (ready) {
        const auto stamp = emulate::pcode::stamp();
        parts.push_back(std::get<0>(stamp));
        parts.push_back(std::get<1>(stamp));
    } else {
        parts.push_back(py::str(""));
    }
    parts.push_back(py::str("gate16 checker, emulators, explorers typed port 1"));   // (the sources of isolated16, windows16, win16, api_arguments, guided, lane_common, concolic, shadow, symbolic, pcode)
    const py::str tools = base::cache::key(parts);
    const py::str header = base::cache::key(Cs{m.header()});
    // (and the description's lines for the function and for each it calls: the checker answers a call as the description says the callee
    // does -- what it removes, what it returns -- so a callee described anew is a verdict to make again)
    py::dict<py::str, py::str> name_line;
    try {
        for (const py::str& line : py::split(read_text(m.description->path), py::str("\n"))) {
            const py::list<py::str> p = py::split(line, py::str("\t"));
            if ((p[0] == py::str("F") || p[0] == py::str("X")) && py::len(p) > 2) {
                name_line.set(p[2], line.rstrip(py::str("\n")));
            }
        }
    } catch (const py::OSError&) {
    }
    const py::dict<py::str, py::str> mtexts = m.texts();
    py::dict<py::str, py::str> out;
    for (const py::str& n : names) {
        py::set<py::str> called;
        for (const py::str& c : call_re.findall(mtexts.get(n, py::str("")))) {
            called.add(c);
        }
        called.add(n);
        std::vector<py::str> deps;
        for (const py::str& c : called) {
            if (name_line.contains(c)) {
                deps.push_back(name_line.at(c));
            }
        }
        out.set(n, base::cache::key(Cs{module_key, tools, header, n, mtexts.get(n, py::str("")), sorted_strs(deps)}));
    }
    return out;
}

thread_local py::list<std::tuple<py::str, py::str>> t_not_run;   // _gate16.not_run

}  // namespace

// {name: how it differs} when the gate's verdict is kept for every function the module has as C text (the last build's set, recorded with
// it) -- else None.
std::optional<py::dict<py::str, py::str>> _gate16_known(const py::str& path, const decompile::c_text::Module16& m) {
    const std::optional<py::bytes> names = base::cache::get(py::str("gate16_set"), base::cache::key(Cs{read_bytes(path), m.header()}));
    if (!names) {
        return std::nullopt;
    }
    py::list<py::str> list;
    for (const py::str& n : py::split(names->decode("utf-8"), py::str("\n"))) {
        if (!n.empty()) {
            list.append(n);
        }
    }
    py::dict<py::str, py::str> differ;
    for (const auto& kv : _gate16_keys(path, list, m).items()) {
        const std::optional<py::bytes> had = base::cache::get(py::str("gate16"), std::get<1>(kv));
        if (!had) {
            return std::nullopt;
        }
        if (had->slice(0, 1) == py::bytes("D")) {
            differ.set(std::get<0>(kv), had->slice_from(1).decode("utf-8", "replace"));
        }
    }
    return differ;
}

// {name: how it differs} for the named functions rebuilt from C, each run beside the original's (verify/gate16/isolated16.py). A function's
// verdict is kept (base/cache.py) by what makes it: the original module, the function's C and the header it compiles with, the compiler and
// its options, and the source of the checker and the emulator -- a function the same as in an earlier build is not run again.
py::dict<py::str, py::str> _gate16(const py::str& path, const py::str& folder, const py::list<py::str>& names, const decompile::c_text::Module16& m) {
    const py::dict<py::str, py::str> keys = _gate16_keys(path, names, m);
    py::dict<py::str, py::str> differ;
    py::list<py::str> run;
    for (const py::str& n : names) {
        const std::optional<py::bytes> had = base::cache::get(py::str("gate16"), keys.at(n));
        if (!had) {
            run.append(n);
        } else if (had->slice(0, 1) == py::bytes("D")) {
            differ.set(n, had->slice_from(1).decode("utf-8", "replace"));
        }
    }
    t_not_run = py::list<std::tuple<py::str, py::str>>();
    if (!run.empty()) {
        for (const auto& c : verify::gate16::isolated16::check(path, folder, run)) {
            if (c.verdict == py::str(verify::verdicts::INCONCLUSIVE)) {        // (no verdict: not kept, and said)
                t_not_run.append({c.name, c.why});
                continue;
            }
            base::cache::put(py::str("gate16"), keys.at(c.name),
                             c.verdict == py::str(verify::verdicts::DIVERGENT) ? py::bytes("D" + c.why.encode("utf-8", "replace").raw()) : py::bytes("S"));
            if (c.verdict == py::str(verify::verdicts::DIVERGENT)) {
                differ.set(c.name, c.why);
            }
        }
    }
    return differ;
}

// Build a module whole. -> Built
//
// A 16-bit module's functions rebuilt from C are then each run beside the original's (verify/gate16/isolated16.py: on their own, every call
// out answered alike on both sides); one that does not do what the original does is rebuilt from its own instructions instead, and the module
// built again (b.gated: name -> how it differed). CB_NO_GATE=1 leaves them.
Built build(const py::str& module, const std::optional<py::str>& outdir, bool call, const Log& log, const std::optional<py::dict<py::str, py::str>>& gated_in,
            bool _gate) {
    const std::optional<py::str> path0 = base::corpus::retail(module);
    if (!path0 || path0->empty()) {
        throw py::SystemExit(py::percent("no module %s in the corpus", py::repr(module)));
    }
    const py::str path = *path0;
    Built b(path);
    std::optional<py::dict<py::str, py::str>> gated = gated_in;
    b.gated = gated ? gated->copy() : py::dict<py::str, py::str>();
    const std::shared_ptr<verify::reconstruct::Kind> kind = verify::reconstruct::kind_of(path);
    b.folder = outdir ? *outdir : state_path(py::str("build"), b.name);
    const py::str folder = *b.folder;
    py::os::makedirs(folder, 0777, true);
    emit(log, py::percent("%s: decompiling", b.name));
    const verify::reconstruct::Prepared prepared = kind->prepare(path, py::os::path::join(folder, py::str("decompiled")));
    b.functions = py::len(prepared.all_keys);
    bool from_cache = false;
    std::shared_ptr<decompile::c_text::Module16> m16;
    std::shared_ptr<decompile::c_text::Module32> m32;
    if (b.kind && *b.kind == py::str("ne")) {
        m16 = std::static_pointer_cast<decompile::c_text::Module16>(prepared.lane);
    } else if (b.kind && *b.kind == py::str("pe")) {
        m32 = std::static_pointer_cast<decompile::c_text::Module32>(prepared.lane);
    }
    if (m16 && !gated && !env_set("CB_NO_GATE")) {
        // every C function's verdict already known (kept by _gate16): the ones that differ go to assembly now, and the module is built once
        // instead of twice
        const auto known = _gate16_known(path, *m16);
        if (known) {
            b.gated = known->copy();
            gated = known;
            from_cache = true;
        }
    }
    if (m16 && m16->description) {
        b.disputes = _disputes(*m16->description);
    }
    emit(log, py::percent("%s: compiling %d functions, data and resources", b.name, py::len(prepared.order)));
    _linker_check(b);
    const std::optional<py::str> image = m16 ? _build16(b, path, *m16, folder) : _build32(b, path, *m32, folder);
    if (!b.resident_overflow.empty()) {
        emit(log, py::percent("%s: %d placed past where a segment is cut: rebuilt from their instructions", b.name, py::len(b.resident_overflow)));
        // (and the gate still runs on what is left as C, if this build was to run it: the overflow is found before the gate, and a build
        // handed its gated set runs none)
        py::dict<py::str, py::str> merged = b.gated.copy();
        for (const auto& kv : b.resident_overflow.items()) {
            merged.set(std::get<0>(kv), std::get<1>(kv));
        }
        return build(module, outdir, call, log, merged, _gate || !gated || from_cache);
    }
    if (!image || image->empty()) {
        return b;
    }
    b.image = py::os::path::join(folder, py::os::path::basename(path).lower());
    py::shutil::copyfile(*image, *b.image);
    if (b.kind && *b.kind == py::str("ne")) {
        _whole_file_image(path, *b.image);
        if ((!gated || from_cache || _gate) && !env_set("CB_NO_GATE")) {
            // (built with the kept verdicts: only a C function not among them is run now)
            py::set<py::str> cset;
            for (const py::str& n : b.placed_c) {
                cset.add(n);
            }
            for (const py::str& n : b.moved_c) {
                cset.add(n);
            }
            const py::list<py::str> c = sorted_set(cset);
            emit(log, py::percent("%s: running %d functions rebuilt from C beside the original's", b.name, py::len(c)));
            py::dict<py::str, py::str> differ = _gate16(path, folder, c, *m16);
            std::vector<std::tuple<py::str, py::str>> failed;
            for (const auto& nw : t_not_run) {
                if (std::get<1>(nw).startswith(py::str("the checker failed"))) {
                    failed.push_back(nw);
                }
            }
            if (!failed.empty()) {
                b.gate_error = py::percent("%d functions (%s)", static_cast<int64_t>(failed.size()), std::get<1>(failed[0]).slice(0, 120));
                emit(log, py::percent("%s: the gate could not run %d functions: %s", b.name, static_cast<int64_t>(failed.size()), std::get<1>(failed[0]).slice(0, 120)));
            }
            // (and its calls beside the original's: what each call is followed by -- the check above answers a call as the description
            // believes; this reads both as they are)
            try {
                for (const auto& kv : verify::gate16::contract16::mismatches(path, folder, *b.image, c).items()) {
                    if (!differ.contains(std::get<0>(kv))) {
                        differ.set(std::get<0>(kv), py::str("its calls are not the original's: ") + std::get<1>(kv));
                    }
                }
            } catch (const py::Exception& e) {                  // (a check that fails says so: never "all agree")
                b.contract_error = py::percent("%s: %s", py::str(e.type()), py::str(e.what()));
                emit(log, py::percent("%s: the call-contract check FAILED: %s", b.name, *b.contract_error));
            } catch (const std::exception& e) {
                b.contract_error = py::percent("Exception: %s", py::str(e.what()));
                emit(log, py::percent("%s: the call-contract check FAILED: %s", b.name, *b.contract_error));
            }
            {
                py::set<py::str> all;
                for (const py::str& n : c) {
                    all.add(n);
                }
                if (gated) {
                    for (const py::str& n : gated->keys()) {
                        all.add(n);
                    }
                }
                base::cache::put(py::str("gate16_set"), base::cache::key(Cs{read_bytes(path), m16->header()}), py::bytes(py::join(py::str("\n"), sorted_set(all)).utf8()));
            }
            if (!differ.empty()) {
                emit(log, py::percent("%s: %d do not do what the original does: rebuilt from their instructions", b.name, py::len(differ)));
                py::dict<py::str, py::str> merged = gated ? gated->copy() : py::dict<py::str, py::str>();
                for (const auto& kv : differ.items()) {
                    merged.set(std::get<0>(kv), std::get<1>(kv));
                }
                return build(module, outdir, call, log, merged);
            }
        }
    }
    check(b);
    if (call) {
        run(b);
    }
    return b;
}

// Several modules built side by side -> {module: Built}, in the order given. Each is built as build() builds it (on threads here: the Python
// gave each module a process of its own); together they take the machine's budget (parallel.workers()): as many modules at once as that, the
// largest first so that the longest is not the one started last. One module, or a budget of one, is built here, in this thread.
// `outdir_of(module)` names each one's folder.
py::dict<py::str, Built> build_many(const py::list<py::str>& modules, const std::function<std::optional<py::str>(const py::str&)>& outdir_of, bool call,
                                    const Log& log, const std::optional<int64_t>& jobs_in) {
    const int64_t jobs = jobs_in && *jobs_in ? *jobs_in : base::parallel::workers();
    // (the Python gave each module a process of its own. The gate and the judge run Unicorn and the tracer, whose state is per process,
    // so builds on threads corrupt each other: one at a time unless CB_BUILD_THREADS asks for more)
    const int64_t procs = py::os::environ_::get(py::str("CB_BUILD_THREADS")) ? std::max<int64_t>(1, std::min<int64_t>(jobs, py::len(modules))) : 1;
    py::dict<py::str, Built> out;
    if (procs == 1) {
        for (const py::str& m : modules) {
            out.set(m, build(m, outdir_of ? outdir_of(m) : std::nullopt, call, log));
        }
        return out;
    }
    auto size = [](const py::str& m) -> int64_t {
        const auto p = base::corpus::retail(m);
        if (p && py::os::path::isfile(*p)) {
            return -file_size(*p);
        }
        return 0;
    };
    std::vector<py::str> by_size;
    for (const py::str& m : modules) {
        by_size.push_back(m);
    }
    std::stable_sort(by_size.begin(), by_size.end(), [&](const py::str& a, const py::str& b) { return size(a) < size(b); });
    auto one = [&](const py::str& m) -> std::pair<py::str, Built> {
        emit(log, py::percent("%s: building (one of %d at once)", py::os::path::basename(m), procs));
        return {m, build(m, outdir_of ? outdir_of(m) : std::nullopt, call, [](const py::str&) {})};
    };
    py::list<py::str> todo;
    for (const py::str& m : by_size) {
        todo.append(m);
    }
    const auto made = jobs::pool::threads(one, todo, procs);
    py::dict<py::str, Built> by_name;
    for (const auto& pm : made) {
        by_name.set(pm.first, pm.second);
    }
    for (const py::str& m : modules) {
        out.set(m, by_name.at(m));
    }
    return out;
}

// python -m codebreaker build <module> [--out DIR] [--no-run] [--engine] [--no-engine]
//
// The engine is the default (engine/win16.py, engine/lanes.py); --no-engine is the old path. Without --out the build goes into the module's
// project folder: Converted/<name>/build/.  (This port has the old path only; the flags are accepted.)
int64_t main(const py::list<py::str>& argv) {
    std::optional<py::str> out;
    for (int64_t i = 0; i < py::len(argv); ++i) {
        if (argv[i] == py::str("--out")) {
            out = argv[i + 1];                                  // (IndexError for a missing folder, as argv[index + 1])
        }
    }
    py::list<py::str> names;
    for (const py::str& a : argv) {
        if (!a.startswith(py::str("--")) && !(out && a == *out)) {
            names.append(a);
        }
    }
    if (names.empty()) {
        py::print(doc_text());
        return 2;
    }
    const bool no_run = argv.contains(py::str("--no-run"));
    int64_t bad = 0;
    py::dict<py::str, std::shared_ptr<base::converted::Project>> projects;
    py::dict<py::str, std::optional<py::str>> where_of;
    for (const py::str& name : names) {
        where_of.set(name, out);
        if (!out) {
            const auto path = base::corpus::retail(name);
            if (path && !path->empty()) {
                const auto proj = std::make_shared<base::converted::Project>(base::converted::project(*path));
                projects.set(name, proj);
                where_of.set(name, proj->build);
            }
        }
    }
    // (several: side by side, as many at once as the budget -- build_many; one: here)
    py::dict<py::str, Built> made;
    if (py::len(names) > 1 && !out) {
        made = build_many(names, [&](const py::str& m) { return where_of.at(m); }, !no_run);
    }
    for (const py::str& name : names) {
        const auto proj = opt_get(projects, name);
        const Built b = made.contains(name) ? made.at(name) : build(name, where_of.at(name), !no_run);
        py::print(b.report());
        if (proj) {
            (*proj)->record(py::str("build"), py::list<py::str>{(*proj)->build},
                            py::percent("the whole module built from its recovered source: %s", b.linked() ? py::str("linked: ") + *b.image : py::str("did not link")));
        }
        if (b.linked()) {
            py::print(py::str("  image      "), *b.image);
        }
        bad += b.linked() ? 0 : 1;
    }
    return bad ? 1 : 0;
}

namespace {

}  // namespace

}  // namespace codebreaker::build::module
