// codebreaker/decompile/decompiler.cpp -- typed C++ of codebreaker.decompile.decompiler (see decompiler.hpp).
//
// decompiler.py -- CodeBreaker's decompiler: a module's functions as C. One decompiler for everything: the native program
// (native/decompiler) knows processors, not file formats; this module takes the file apart with the readers in
// codebreaker.formats and describes it, runs the program, and makes what comes back into C.
//
// Not kept from the Python: the `timeout=` of its subprocess runs (the runtime's subprocess has none); the _asm blocks pass
// (CB_ASM_BLOCKS=1: asm_blocks.place wants the decoder's operand access, which describe's decoder does not give -- it
// raises NotImplementedError there, as it is off by default).

#include "codebreaker/decompile/decompiler.hpp"

#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "codebreaker/base/cache.hpp"
#include "codebreaker/base/converted.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/base/parallel.hpp"
#include "codebreaker/build/functions.hpp"
#include "codebreaker/build/watcom.hpp"
#include "codebreaker/decompile/c_text.hpp"
#include "codebreaker/decompile/describe.hpp"
#include "codebreaker/decompile/frame16.hpp"
#include "codebreaker/decompile/shape16.hpp"
#include "codebreaker/formats/dos.hpp"
#include "codebreaker/formats/detect.hpp"
#include "codebreaker/formats/le.hpp"
#include "codebreaker/formats/nes.hpp"
#include "codebreaker/formats/pe.hpp"
#include "codebreaker/jobs/pool.hpp"
#include "codebreaker/symbols/dos_interrupts.hpp"
#include "codebreaker/translate/backends/c.hpp"
#include "json.hpp"
#include "py_settings.hpp"
#include "py_subprocess.hpp"

#include <iostream>
#include <stdlib.h>

// verify/lanes/win32.hpp, declared here for its one function: that header brings in the symbolic engine (z3), which a decompiler has no use for
namespace codebreaker::verify::lanes::win32 {
py::list<int64_t> functions(const py::str& lane, const py::str& path);   // sorted(pe_functions.carve(path).funcs)
}

namespace codebreaker::decompile::decompiler {

namespace cache = codebreaker::base::cache;
namespace cback = codebreaker::translate::backends::c;
namespace native_l2 = codebreaker::translate::native_l2;
namespace ir = codebreaker::translate::ir;
using native_l2::Wire;
using ir::Dict;
using ir::Val;

namespace {

py::str S(const char* s) { return py::str(s); }
py::str S(const std::string& s) { return py::str(s); }
py::str D(int64_t v) { return py::str(std::to_string(v)); }
py::str X(int64_t v) {
    char buf[32];
    std::snprintf(buf, sizeof buf, "%llx", static_cast<unsigned long long>(v));
    return py::str(std::string(buf));
}
py::str X8(int64_t v) {
    char buf[32];
    std::snprintf(buf, sizeof buf, "%08llx", static_cast<unsigned long long>(v));
    return py::str(std::string(buf));
}
py::str XU(int64_t v, int w) {
    char buf[32];
    std::snprintf(buf, sizeof buf, "%0*llX", w, static_cast<unsigned long long>(v));
    return py::str(std::string(buf));
}

py::str joins(const std::vector<py::str>& v, const char* sep) {
    std::string out;
    for (size_t i = 0; i < v.size(); ++i) {
        if (i) out += sep;
        out += v[i].utf8();
    }
    return py::str(out);
}

py::str read_text(const py::str& path) {
    py::file f = py::open(path, S("r"), S("utf-8"), S("replace"));
    py::str t = f.read_text();
    f.close();
    return t;
}

py::bytes read_bytes(const py::str& path) {
    py::file f = py::open(path, S("rb"));
    py::bytes b = f.read();
    f.close();
    return b;
}

// the extra parts a cache key carries when the L2 notes are asked for: a text made with them is another text
std::vector<cache::Part> l2_key() {
    std::vector<cache::Part> out;
    bool l2 = !py::os::getenv(S("CB_L2"), S("")).empty();
    if (l2) out.push_back(cache::Part(S("l2 2")));
    if (l2 && frame16_on()) out.push_back(cache::Part(S("frame16")));
    return out;
}

// ---- L2 documents ---------------------------------------------------------------------------------

const Val* vget(const Dict& d, const char* k) {
    py::str key = S(k);
    return d.contains(key) ? &d.at(key) : nullptr;
}
const Dict* as_dict(const Val* v) { return v ? std::get_if<Dict>(&v->v) : nullptr; }
const Val::List* as_list(const Val* v) { return v ? std::get_if<Val::List>(&v->v) : nullptr; }
const py::str* as_str(const Val* v) { return v ? std::get_if<py::str>(&v->v) : nullptr; }

// Every node of an L2 document, the types and symbols left out.
void l2_walk(const Val& x, std::vector<Dict>& out) {
    if (const Val::List* l = std::get_if<Val::List>(&x.v)) {
        for (const Val& y : l->vec()) l2_walk(y, out);
    } else if (const Dict* d = std::get_if<Dict>(&x.v)) {
        if (d->contains(S("node"))) out.push_back(*d);
        for (const auto& kv : d->items()) {
            const py::str& k = std::get<0>(kv);
            const Val& v = std::get<1>(kv);
            if ((std::holds_alternative<Dict>(v.v) || std::holds_alternative<Val::List>(v.v)) && k != "type" && k != "sym" && k != "ret")
                l2_walk(v, out);
        }
    }
}

std::vector<Dict> l2_nodes(const Dict& doc) {
    std::vector<Dict> out;
    if (const Val* body = vget(doc, "body")) l2_walk(*body, out);
    return out;
}

bool node_is(const Dict& e, const char* kind) {
    const py::str* s = as_str(vget(e, "node"));
    return s && *s == kind;
}

// the name of a Name node's symbol (None where it has none)
std::optional<py::str> sym_name(const Dict& e) {
    const Dict* sym = as_dict(vget(e, "sym"));
    if (!sym) return std::nullopt;
    const py::str* n = as_str(vget(*sym, "name"));
    return n ? std::optional<py::str>(*n) : std::nullopt;
}

// [(name, arguments)] of each call of a function by its name in an L2 document (`f(a,b)`).
std::vector<std::pair<py::str, int64_t>> l2_calls(const Dict& doc) {
    std::vector<std::pair<py::str, int64_t>> out;
    for (const Dict& e : l2_nodes(doc)) {
        if (!node_is(e, "Call")) continue;
        const Dict* t = as_dict(vget(e, "target"));
        if (t && !t->empty() && node_is(*t, "Name")) {
            std::optional<py::str> n = sym_name(*t);
            const Val::List* args = as_list(vget(e, "args"));
            out.emplace_back(n && !n->empty() ? *n : S(""), args ? args->size() : 0);
        }
    }
    return out;
}

// Every name an L2 document's body uses.
std::set<std::string> l2_names(const Dict& doc) {
    std::set<std::string> out;
    for (const Dict& e : l2_nodes(doc))
        if (node_is(e, "Name")) {
            std::optional<py::str> n = sym_name(e);
            if (n) out.insert(n->utf8());
        }
    return out;
}

std::optional<L2Doc> doc_of(const py::dict<py::str, FunctionNotes>& notes, const py::str& name) {
    if (!notes.contains(name)) return std::nullopt;
    return notes.at(name).l2;
}

const py::re::Pattern& CALLED() {
    static const py::re::Pattern p = py::re::compile("\\bfunc_0x([0-9a-f]{8})\\s*\\(");
    return p;
}

// Does the function name `name` anywhere (L2: among its body's names)?
bool mentions(const py::str& text, const std::optional<L2Doc>& doc, const py::str& name) {
    if (doc) return l2_names(*doc).count(name.utf8()) > 0;
    return text.contains(name);
}

}  // namespace

// ---- the native program ---------------------------------------------------------------------------

py::str _exe() {
    std::optional<py::str> bin = py::settings::get_opt_str(S("decompiler_bin"));
    if (!bin) throw py::TypeError("expected str, bytes or os.PathLike object, not NoneType");
    return py::os::path::join(*bin, S("decompiler.exe"));
}

py::str _languages() {
    // (CB_LANGUAGES_DIR: another folder of language files -- an older decompiler's, run beside this one to compare the two)
    py::str env = py::os::getenv(S("CB_LANGUAGES_DIR"), S(""));
    if (!env.empty()) return env;
    return py::os::path::join(py::settings::checkout_root(), S("native"), S("decompiler"), S("languages"));
}

// "1" when the native program is to recover structures for this description (its CB_STRUCTS), else "0". The setting
// `struct_recovery`: "off", "32" (32-bit x86 only), or "all" (the default: 16-bit code too).
py::str _structs(const py::str& description) {
    std::optional<py::str> s = py::settings::get_opt_str(S("struct_recovery"));
    py::str want = (s && !s->empty() ? *s : S("off")).strip().lower();
    if (want == "all") return S("1");
    if (want != "32") return S("0");
    try {
        py::file f = py::open(description, S("r"), S("utf-8"), S("replace"));
        for (const py::str& line : f.readlines_text().vec()) {
            if (line.startswith(S("arch\t"))) return py::split(line, S("\t"))[1].startswith(S("x86:LE:32")) ? S("1") : S("0");
        }
    } catch (const py::OSError&) {
    }
    return S("0");
}

bool available() { return py::os::path::isfile(_exe()) && py::os::path::isdir(_languages()); }

// CB_FRAME16=1: a 16-bit function's locals laid out as the original's frame and its statements shaped as C 8 writes them.
bool frame16_on() {
    py::str v = py::os::getenv(S("CB_FRAME16"), S("0"));
    return !(v.empty() || v == "0");
}

// Whether a 16-bit function's L2 tree is given passes that read its code (c_from_l2's `images`).
bool tree_images() { return !py::os::getenv(S("CB_ASM_BLOCKS"), S("")).empty() || frame16_on(); }

// {segment: bytes} of a 16-bit module's code, from its description's `bytes` lines (describe16).
py::dict<int64_t, py::bytes> _segment_images(const py::str& description) {
    py::dict<int64_t, py::bytes> images;
    py::file f = py::open(description, S("r"), S("utf-8"), S("replace"));
    for (const py::str& ln0 : f.readlines_text().vec()) {
        py::str ln = ln0.rstrip(S("\n"));
        py::list<py::str> p = py::split(ln, S("\t"));
        if (p[0] == "bytes" && p.size() >= 5) {
            py::file fh = py::open(p[2], S("rb"));
            fh.seek(py::int_parse(p[3], 16));
            images.set(std::get<0>(describe::where(py::int_parse(p[1], 16))), fh.read(py::int_parse(p[4], 16)));
            fh.close();
        }
    }
    return images;
}

// What the native program says of a function besides its C: notes on lines of their own before it (decompiler.cc
// decompileOne). They are read here, once, as the native program's output is taken in -- the C handed on holds none of
// them -- and become the function's facts (split_notes):
//   /*@G <va> <bytes> <bytes pointed at> */     a global it touches     -> globals
//   /*@P <name> <bytes> */                      a pointer and its size  -> pointers
//   /*@S <name> <bytes> <off>:<bytes>:<type>... */  a structure it recovered -> structs
//   /*@L2 {...} */                              its L2 document (CB_L2) -> l2
std::tuple<py::str, FunctionNotes> split_notes(const py::str& text) {
    static const py::re::Pattern note = py::re::compile("/\\*@(G|P|S|L2) (.*) \\*/");
    static const py::re::Pattern field = py::re::compile("([0-9a-f]+):(\\d+):([\\w*^]+)");
    FunctionNotes facts;
    py::list<py::str> lines = py::split(text, S("\n"));
    int64_t k = 0;
    while (k < lines.size()) {
        std::optional<py::re::Match> m = note.fullmatch(lines[k].strip());
        if (!m) break;
        py::str kind = m->group(1), rest = m->group(2);
        if (kind == "L2") {
            Val doc = native_l2::from_wire(::py::json_parse(rest.utf8()));
            const Dict* d = std::get_if<Dict>(&doc.v);
            if (!d) throw py::TypeError("an L2 document is a dict");
            facts.l2 = *d;
            native_l2::known_text(*d, rest);   // (the native writer reads the text itself)
        } else if (kind == "S") {
            py::list<py::str> parts = py::split(rest);
            core16::StructNote sn;
            sn.name = parts[0];
            sn.bytes = py::int_parse(parts[1], 10);
            for (int64_t i = 2; i < parts.size(); ++i) {
                std::optional<py::re::Match> fm = field.fullmatch(parts[i]);
                if (!fm) throw py::AttributeError("'NoneType' object has no attribute 'groups'");
                sn.fields.append(core16::StructField{py::int_parse(fm->group(1), 16), py::int_parse(fm->group(2), 10), fm->group(3)});
            }
            facts.structs.append(sn);
        } else {
            py::list<py::str> parts = py::split(rest);
            if (kind == "G") {
                facts.globals.append(core16::GlobalNote{py::int_parse(parts[0], 16), py::int_parse(parts[1], 10),
                                                        parts.size() > 2 ? py::int_parse(parts[2], 10) : 0});
            } else {
                if (!facts.pointers.contains(parts[0])) facts.pointers.set(parts[0], py::int_parse(parts[1], 10));
            }
        }
        ++k;
    }
    return {py::join(S("\n"), lines.slice(k, lines.size())), facts};
}

// A function's C written by the hub's C back end from its L2 document (facts["l2"]; translate/backends/c.py) in place of the C
// the decompiler printed (`text`). Without a document, or one the back end declines, the printed C stays. With `images`
// ({segment: code}; tree_images) a 16-bit function's tree is first given the passes that read its instructions: with
// CB_FRAME16=1 its locals laid out as the original's frame and its statements shaped as C 8 writes them (frame16, shape16) --
// once: facts.l2 is the document so made, facts.placed says it is. `module`: what the module says of the function, for the back
// end's passes that need it. facts.from_l2 says which C it is.
std::optional<py::str> c_from_l2(const std::optional<py::str>& text, FunctionNotes& facts,
                                 const std::optional<py::dict<int64_t, py::bytes>>& images, c_text::ModuleFacts* module) {
    if (!facts.l2) return text;
    Dict doc = *facts.l2;
    const Dict* origin = as_dict(vget(doc, "origin"));
    const Val* seg = origin ? vget(*origin, "segment") : nullptr;
    const int64_t* seg_n = seg ? std::get_if<int64_t>(&seg->v) : nullptr;
    if (images && !images->empty() && seg_n && images->contains(*seg_n) && !facts.placed) {
        py::bytes code = images->at(*seg_n);
        int64_t offset = std::get<int64_t>(vget(*origin, "offset")->v);
        if (!py::os::getenv(S("CB_ASM_BLOCKS"), S("")).empty())
            throw py::NotImplementedError("CB_ASM_BLOCKS: asm_blocks.place needs the decoder's operand access");
        if (frame16_on()) {   // (and its locals as the frame had them)
            auto [d1, said1] = frame16::lay_out(doc, code, offset, code.size());
            auto [d2, said2] = shape16::reshape(d1, code);
            doc = d2;
        }
        facts.l2 = doc;
        facts.placed = true;
    }
    py::str body;
    try {
        if (module) {
            Wire w = c_text::facts_wire(*module);
            try {
                body = native_l2::function(doc, cback::MOVED, std::nullopt, w);   // (its far pointers: L2's types say which)
            } catch (...) {
                module->wire = w;
                throw;
            }
            c_text::read_back(*module, w);
            module->wire = w;
        } else {
            body = native_l2::function(doc, cback::MOVED);
        }
    } catch (const cback::Decline&) {
        return text;
    } catch (const py::ValueError&) {
        return text;
    } catch (const py::KeyError&) {
        return text;
    }
    facts.from_l2 = true;
    return S("\n") + body;
}

// ---- running it -----------------------------------------------------------------------------------

namespace {

struct RunOut {
    py::dict<py::str, py::str> texts;
    py::list<py::str> order;
    py::dict<int64_t, py::str> by_va;
    py::str stderr_;
};

RunOut _run(const py::str& description, const py::list<int64_t>& only, int64_t timeout, std::optional<int64_t> max_instructions);

const py::re::Pattern& MARK() {
    static const py::re::Pattern p = py::re::compile("(?m)^/\\*@@ \\S+ ([0-9a-f]+) \\*/");
    return p;
}

}  // namespace

const py::re::Pattern _BLOCK = py::re::compile("(?ms)^/\\*@@ \\S+ ([0-9a-f]+) \\*/\\n.*?(?=^/\\*@@ |\\Z)");

namespace {

const int64_t CHUNK = 64;              // functions to a process
const int64_t PROCESS_MB = 1536;       // what the governor counts for each: the native program's own budget (CB_MEMORY_MB)


py::str tail_chars(const py::str& s, int64_t n) { return s.size() > n ? s.slice(s.size() - n, s.size()) : s; }

// os.environ[name] = value / del os.environ[name]: the C library's copy and the process's
void set_env(const py::str& name, const py::str& value) {
    std::wstring n = py::subprocess::to_wide(name), v = py::subprocess::to_wide(value);
    _wputenv_s(n.c_str(), v.c_str());
}
void unset_env(const py::str& name) {
    std::wstring n = py::subprocess::to_wide(name);
    _wputenv_s(n.c_str(), L"");
}

// the blocks of the native program's output: a line `/*@@ <name> <hex va> */` and what follows it up to the next line that
// begins `/*@@ ` (the Python's regexes, (?ms)^/\*@@ (\S+) ([0-9a-f]+) \*/\n(.*?)(?=^/\*@@ |\Z), found by walking the lines)
struct Block {
    py::str name;
    int64_t va = 0;
    int64_t start = 0;        // where the marker line starts
    int64_t body = 0;         // where its text starts (after the line's newline)
    int64_t end = 0;          // where the next marker starts, or the end
};

std::vector<Block> scan_blocks(const py::str& text) {
    static const py::re::Pattern header = py::re::compile("/\\*@@ (\\S+) ([0-9a-f]+) \\*/");
    const std::u32string& u = text.u32();
    std::vector<int64_t> markers;
    for (size_t i = 0; i + 5 <= u.size(); ++i)
        if ((i == 0 || u[i - 1] == U'\n') && u.compare(i, 5, U"/*@@ ") == 0) markers.push_back(static_cast<int64_t>(i));
    std::vector<Block> out;
    for (size_t k = 0; k < markers.size(); ++k) {
        int64_t i = markers[k];
        size_t nl = u.find(U'\n', static_cast<size_t>(i));
        if (nl == std::u32string::npos) continue;   // (the header must be followed by its newline)
        std::optional<py::re::Match> m = header.fullmatch(text.slice(i, static_cast<int64_t>(nl)));
        if (!m) continue;
        Block b;
        b.name = m->group(1);
        b.va = py::int_parse(m->group(2), 16);
        b.start = i;
        b.body = static_cast<int64_t>(nl) + 1;
        b.end = k + 1 < markers.size() ? markers[k + 1] : static_cast<int64_t>(u.size());
        out.push_back(b);
    }
    return out;
}

int64_t threads_of(const py::dict<py::str, py::str>& env) {
    py::str t = env.get(S("CB_THREADS"), S(""));
    if (t.empty()) return 1;
    int64_t n = py::int_parse(t, 10);
    return n ? n : 1;
}

// _run_resuming once the threads are settled: the native program; when it dies part way (a crash that depends on what it did
// before -- the function it died on decompiles on its own), start it again from the next function it had not written, and so
// on. A function a fresh start dies on at once is left out, with a note on stderr.
// -> (stdout, stderr, whether every function was attempted)
std::tuple<py::str, py::str, bool> _resuming(const py::str& description, const py::list<int64_t>& only, int64_t timeout,
                                             py::dict<py::str, py::str> env, const std::vector<int64_t>& wanted) {
    (void)timeout;
    bool first = true;
    std::vector<py::str> outs, errs;
    std::vector<int64_t> left = wanted;
    static const py::re::Pattern governed_rx =
        py::re::compile("/\\* decompile failed: over the (?:memory budget|time limit)[^*]*\\*/\\s*$");
    for (;;) {
        py::list<py::str> cmd;
        cmd.append(_exe());
        cmd.append(description);
        if (!only.empty() || !first) {
            cmd.append(S("--at"));
            for (int64_t va : left) cmd.append(X(va));
        }
        auto [rc, out, err] = build::functions::run_process(cmd, std::nullopt, env);
        outs.push_back(out);
        errs.push_back(tail_chars(err, 1000));
        if (rc == 0) return {joins(outs, ""), tail_chars(joins(errs, ""), 2000), true};
        py::list<py::re::Match> marks = MARK().finditer(out);
        std::set<int64_t> done;
        for (const py::re::Match& m : marks.vec()) done.insert(py::int_parse(m.group(1), 16));
        std::optional<int64_t> last;
        if (!marks.empty()) last = py::int_parse(marks[-1].group(1), 16);
        // (the last one written may be cut off by the crash: it is done again. Last as it wrote them -- in the description's
        // order, not the addresses': a function found in a later round is described at the end.)
        if (threads_of(env) > 1) {
            // Several threads at work: what ended the process was any of the functions in hand, not the last one written.
            // What is left is done on one thread, where the last written is the one in hand.
            env = env.copy();
            env.set(S("CB_THREADS"), S("1"));
            std::vector<int64_t> nl;
            for (int64_t va : left)
                if (!done.count(va) || (last && va == *last)) nl.push_back(va);
            left = nl;
            if (!done.empty()) outs.back() = out.slice(0, marks[-1].start());
            first = false;
            continue;
        }
        // the governor stopped it on one function (over its memory budget or its time): that one is done -- failed, as
        // written -- and is not tried again
        std::optional<py::re::Match> governed = governed_rx.search(out);
        if (governed) {
            std::optional<py::re::Match> over = py::re::search("over the [^*]*", governed->group(0));
            if (!last) throw py::TypeError("%x format: an integer is required, not NoneType");
            errs.push_back(S("\n[") + X(*last) + S(" stopped by the governor: ") + over->group(0).strip() + S("]\n"));
        }
        {
            std::vector<int64_t> nl;
            for (int64_t va : left)
                if (!done.count(va) || (last && va == *last && !governed)) nl.push_back(va);
            left = nl;
        }
        if (!done.empty() && last && !governed) outs.back() = out.slice(0, marks[-1].start());
        bool in_left = last && std::find(left.begin(), left.end(), *last) != left.end();
        if (done.empty() || (done.size() == 1 && last && *done.begin() == *last && in_left)) {
            int64_t dead;
            if (!done.empty()) {
                auto it = std::find(left.begin(), left.end(), *last);
                dead = *it;
                left.erase(it);
            } else {
                if (left.empty()) throw py::IndexError("pop from empty list");
                dead = left.front();
                left.erase(left.begin());
            }
            errs.push_back(S("\n[decompiler died on ") + X(dead) + S("; left out]\n"));
        }
        first = false;
        if (left.empty()) return {joins(outs, ""), tail_chars(joins(errs, ""), 2000), false};
    }
}

// Run the native program; chunks of functions as processes of their own (below). -> (stdout, stderr, whether every
// function was attempted)
std::tuple<py::str, py::str, bool> _run_resuming(const py::str& description, const py::list<int64_t>& only, int64_t timeout,
                                                 std::optional<int64_t> max_instructions) {
    // CB_THREADS (setting "threads"): functions analysed at once in the one process, by hand; unset, one thread, and the
    // functions are shared among processes instead (below)
    std::optional<int64_t> threads_setting = py::settings::get_opt_int(S("threads"));
    py::dict<py::str, py::str> env;
    env.set(S("CB_LANGUAGES"), _languages());
    env.set(S("CB_THREADS"), D(threads_setting && *threads_setting ? *threads_setting : 1));
    env.set(S("CB_STRUCTS"), _structs(description));   // (structures: see _structs)
    if (max_instructions && *max_instructions) env.set(S("CB_MAX_INSTRUCTIONS"), D(*max_instructions));
    std::vector<int64_t> wanted;
    if (!only.empty()) {
        py::list<int64_t> s = py::sorted(only);
        wanted = s.vec();
    } else {
        std::set<int64_t> found;
        py::file f = py::open(description, S("r"), S("utf-8"), S("replace"));
        for (const py::str& line : f.readlines_text().vec())
            if (line.startswith(S("F\t"))) found.insert(py::int_parse(py::split(line, S("\t"))[1], 16));
        wanted.assign(found.begin(), found.end());
    }
    if (wanted.empty()) return {S(""), S(""), true};   // (no code: a font or resource-only module)
    if (threads_setting && *threads_setting) return _resuming(description, only, timeout, env, wanted);   // (set by hand: as it says)
    if (static_cast<int64_t>(wanted.size()) <= CHUNK) return _resuming(description, only, timeout, env, wanted);
    // More than a chunk: the functions in fixed chunks of CHUNK, in address order, each chunk a process of its own on one
    // thread, as many at once as the governor gives slots and memory for (base/parallel.py). Not the native program's threads:
    // each keeps its own analysis, and the globals one function types stay typed for the next -- so with threads a function's
    // text depended on which thread it fell to. A chunk is always the same functions in the same order, so the texts are the
    // same however many run at once, on any machine.
    py::list<py::list<int64_t>> chunks;
    for (size_t i = 0; i < wanted.size(); i += static_cast<size_t>(CHUNK)) {
        std::vector<int64_t> c(wanted.begin() + static_cast<long>(i),
                               wanted.begin() + static_cast<long>(std::min(wanted.size(), i + static_cast<size_t>(CHUNK))));
        chunks.append(py::list<int64_t>(c));
    }
    base::parallel::slots slots(chunks.size(), PROCESS_MB, S("decompile"));
    int64_t n = slots.enter();
    py::list<std::tuple<py::str, py::str, bool>> parts;
    try {
        parts = jobs::pool::threads(
            [&](const py::list<int64_t>& c) { return _resuming(description, c, timeout, env, c.vec()); }, chunks, n);
    } catch (...) {
        slots.exit();
        throw;
    }
    slots.exit();
    std::vector<std::tuple<int64_t, py::str>> blocks;
    bool whole = true;
    std::vector<py::str> errs;
    for (const auto& p : parts.vec()) {
        const py::str& out = std::get<0>(p);
        for (const Block& b : scan_blocks(out)) blocks.emplace_back(b.va, out.slice(b.start, b.end));
        errs.push_back(std::get<1>(p));
        whole = whole && std::get<2>(p);
    }
    std::sort(blocks.begin(), blocks.end());
    std::vector<py::str> texts;
    for (const auto& b : blocks) texts.push_back(std::get<1>(b));
    return {joins(texts, ""), tail_chars(joins(errs, ""), 2000), whole};
}

// (run(), for the functions not kept: the native program on the description as a whole)
RunOut _run_whole(const py::str& description, const py::list<int64_t>& only, int64_t timeout, std::optional<int64_t> max_instructions) {
    // The description holds the module's bytes and every fact given with them, so what the program says of it is kept under its
    // hash (base/cache.py): a module is decompiled once.
    py::bytes text = read_bytes(description);
    // (by what the files it names hold, not where they are: the same module described in another build folder is the same
    // description)
    py::dict<py::str, py::str> files;
    py::str as_text = text.decode("latin1");
    py::str replaced = py::re::sub(
        "(?m)^(bytes\\t[0-9a-fA-F]+\\t)([^\\t\\r\\n]+)(\\t)",
        [&](const py::re::Match& mm) {
            py::str p = mm.group(2).encode("latin1").decode("utf-8", "replace");
            if (!files.contains(p)) files.set(p, cache::key({cache::Part(read_bytes(p))}));
            return mm.group(1) + files.at(p) + mm.group(3);
        },
        as_text);
    py::bytes keytext = replaced.encode("latin1");
    py::list<py::str> only_hex;
    for (int64_t va : only.vec()) only_hex.append(X(va));
    std::vector<cache::Part> parts{cache::Part(cache::tool(_exe())), cache::Part(_language_stamp()), cache::Part(keytext),
                                   cache::Part(only_hex),
                                   max_instructions && *max_instructions ? cache::Part(*max_instructions) : cache::Part(S("")),
                                   cache::Part(_structs(description))};
    for (const cache::Part& p : l2_key()) parts.push_back(p);
    py::str k = cache::key(parts);
    std::optional<py::bytes> had = cache::get(S("decompiled"), k);
    py::str stdout_, stderr_;
    if (had) {
        py::str all = had->decode("utf-8", "replace");
        int64_t cut = all.index(S(std::string(1, '\0')));
        stdout_ = all.slice(0, cut);
        stderr_ = all.slice(cut + 1, all.size());
    } else {
        auto [so, se, whole] = _run_resuming(description, only, timeout, max_instructions);
        stdout_ = so;
        stderr_ = se;
        if (whole && !stdout_.empty())
            cache::put(S("decompiled"), k, (stdout_ + S(std::string(1, '\0')) + stderr_).encode("utf-8", "replace"));
    }
    RunOut out;
    py::list<int64_t> large;
    for (const Block& b : scan_blocks(stdout_)) {
        py::str t = stdout_.slice(b.body, b.end).strip();
        if (t.startswith(S("/* decompile failed: Flow exceeded maximum allowable instructions"))) large.append(b.va);
        if (t.empty() || t.startswith(S("/* decompile failed"))) continue;
        if (!out.texts.contains(b.name)) {
            out.texts.set(b.name, t);
            out.order.append(b.name);
            out.by_va.set(b.va, b.name);
        }
    }
    if (!large.empty() && !(max_instructions && *max_instructions)) {   // longer than a wrong start would run: again, with room
        RunOut more = _run(description, large, timeout, LARGE);
        out.texts.update(more.texts);
        for (const py::str& n : more.order.vec())
            if (!out.order.contains(n)) out.order.append(n);
        out.by_va.update(more.by_va);
    }
    out.stderr_ = stderr_;
    return out;
}

// Run the native program on a description. Works for any processor it has a language for; `only` limits it to those addresses.
// Each function's text is kept on its own (base/cache.py) with the lines of the description it was made from: its own line, the
// calls through pointers inside it, and the line of every function and import its text calls (or that there was none, for a
// `func_0x...` it calls). A function whose lines are all as they were is not decompiled again.
RunOut _run(const py::str& description, const py::list<int64_t>& only, int64_t timeout, std::optional<int64_t> max_instructions) {
    py::list<py::str> lines = py::splitlines(read_text(description));
    std::map<int64_t, py::str> fn_line;
    py::dict<py::str, py::str> name_line;
    std::vector<std::pair<int64_t, py::str>> o_lines;
    py::list<py::str> base;
    for (const py::str& ln : lines.vec()) {
        py::list<py::str> p = py::split(ln, S("\t"));
        if (p[0] == "F" && p.size() > 2) {
            fn_line[py::int_parse(p[1], 16)] = ln;
            name_line.set(p[2], ln);
        } else if (p[0] == "X" && p.size() > 2) {
            name_line.set(p[2], ln);
            base.append(ln);   // (an import: every caller's as well)
        } else if (p[0] == "O" && p.size() > 2) {
            o_lines.emplace_back(py::int_parse(p[1], 16), ln);
        } else if (p[0] == "bytes" && p.size() > 2) {
            base.append(S("bytes\t") + p[1] + S("\t") + cache::key({cache::Part(read_bytes(p[2]))}));
        } else {
            base.append(ln);
        }
    }
    if (fn_line.empty()) return _run_whole(description, only, timeout, max_instructions);
    std::vector<cache::Part> cparts{cache::Part(cache::tool(_exe())), cache::Part(_language_stamp()), cache::Part(base),
                                    max_instructions && *max_instructions ? cache::Part(*max_instructions) : cache::Part(S("")),
                                    cache::Part(_structs(description))};
    for (const cache::Part& p : l2_key()) cparts.push_back(p);   // (with the L2 notes: another text)
    py::str common = cache::key(cparts);
    std::vector<int64_t> starts;
    for (const auto& kv : fn_line) starts.push_back(kv.first);   // (sorted: a map)
    auto own = [&](int64_t va) {
        auto it = std::upper_bound(starts.begin(), starts.end(), va);
        int64_t end = (it != starts.end() && (*it >> 16) == (va >> 16)) ? *it : ((va >> 16) + 1) << 16;
        py::list<py::str> out;
        for (const auto& [site, ln] : o_lines)
            if (va <= site && site < end) out.append(ln);
        return out;
    };
    std::vector<int64_t> wanted;
    if (!only.empty()) wanted = py::sorted(only).vec();
    else wanted = starts;
    struct Entry {
        py::str name, text;
    };
    std::map<int64_t, Entry> reused;
    std::vector<int64_t> misses;
    for (int64_t va : wanted) {
        auto f = fn_line.find(va);
        if (f == fn_line.end()) {
            misses.push_back(va);
            continue;
        }
        std::optional<py::bytes> had = cache::get(S("decompiled_fn"), cache::key({cache::Part(common), cache::Part(f->second), cache::Part(own(va))}));
        if (!had) {
            misses.push_back(va);
            continue;
        }
        Wire entry = ::py::json_parse(had->decode("utf-8").utf8());
        bool ok = true;
        for (const auto& kv : entry.get("deps").items()) {
            py::str n = py::str(kv.first.str());
            if (!(name_line.contains(n) && name_line.at(n) == py::str(kv.second.str()))) ok = false;
        }
        for (const auto& kv : entry.get("unknown").items()) {
            bool present = fn_line.count(py::int_parse(py::str(kv.first.str()), 16)) > 0;
            if (present != ::py::truthy(kv.second)) ok = false;
        }
        if (ok) reused[va] = Entry{py::str(entry.get("name").str()), py::str(entry.get("text").str())};
        else misses.push_back(va);
    }
    RunOut out;
    if (!misses.empty()) {
        bool whole_set = !only.empty() || misses.size() < starts.size();
        out = _run_whole(description, whole_set ? py::list<int64_t>(misses) : py::list<int64_t>(), timeout, max_instructions);
    }
    static const py::re::Pattern call_rx = py::re::compile("\\b([A-Za-z_]\\w*)\\s*\\(");
    static const py::re::Pattern unk_rx = py::re::compile("\\bfunc_0x([0-9a-f]{8})\\b");
    for (const auto& kv : out.by_va.items()) {   // (kept for the next time, with their lines)
        int64_t va = std::get<0>(kv);
        const py::str& name = std::get<1>(kv);
        if (!fn_line.count(va)) continue;
        py::str text = out.texts.get(name, S(""));
        Wire deps = Wire::dict();
        std::set<std::string> seen;
        for (const py::str& n : call_rx.findall(text).vec()) {
            if (name_line.contains(n) && n != name && !seen.count(n.utf8())) {
                seen.insert(n.utf8());
                deps.set(n.utf8().c_str(), Wire(name_line.at(n).utf8()));
            }
        }
        Wire unknown = Wire::dict();
        for (const py::str& v : unk_rx.findall(text).vec()) unknown.set(X8(py::int_parse(v, 16)).utf8().c_str(), Wire(false));
        Wire e = Wire::dict();
        e.set("name", Wire(name.utf8()));
        e.set("text", Wire(text.utf8()));
        e.set("deps", deps);
        e.set("unknown", unknown);
        cache::put(S("decompiled_fn"), cache::key({cache::Part(common), cache::Part(fn_line[va]), cache::Part(own(va))}),
                   py::str(::py::json_dump(e)).encode("utf-8"));
    }
    for (const auto& [va, entry] : reused) {
        out.texts.set(entry.name, entry.text);
        out.by_va.set(va, entry.name);
    }
    out.order = py::list<py::str>();
    for (int64_t va : py::sorted(py::list<int64_t>(out.by_va.keys())).vec()) out.order.append(out.by_va.at(va));
    return out;
}

}  // namespace

py::str _language_stamp() {
    static std::optional<py::str> stamp;
    if (!stamp) stamp = cache::folder(_languages());
    return *stamp;
}

// The version of what makes decompiled C, as the cache keeps its results apart (base/cache.py GENERATIONS): REV, the native
// program and its languages. (Not those of CB_LANGUAGES_DIR: a run comparing another folder of them shares the generation of now.)
std::tuple<int64_t, py::str, py::str> generation() {
    py::str languages = py::os::getenv(S("CB_LANGUAGES_DIR"), S("")).empty()
                            ? _language_stamp()
                            : cache::folder(py::os::path::join(py::settings::checkout_root(), S("native"), S("decompiler"), S("languages")));
    return {REV, cache::tool(_exe()), languages};
}

// As _run, the notes taken apart from the C (split_notes) -> Decompiled. With `l2` each function's C is the hub's C back end's,
// from its L2 (c_from_l2): the x86 lanes that make compilable C ask for it (decompile16, compilable32); for the others
// CB_C_FROM_L2=1 says. `write`=False: the L2 documents kept and the printed C handed on (decompile16: the module writes its C
// from them once it is whole, c_text._write16).
Decompiled run(const py::str& description, const py::list<int64_t>& only, int64_t timeout, const std::optional<int64_t>& max_instructions,
               const std::optional<bool>& l2_in, bool write) {
    bool l2 = l2_in ? *l2_in : !py::os::getenv(S("CB_C_FROM_L2"), S("")).empty();
    std::optional<py::str> was = py::os::getenv(S("CB_L2"));
    // (the environment of the process, for the time of the run: _run's cache keys read it)
    if (l2) set_env(S("CB_L2"), S("1"));
    RunOut r;
    try {
        r = _run(description, only, timeout, max_instructions);
    } catch (...) {
        if (l2 && !was) unset_env(S("CB_L2"));
        else if (l2) set_env(S("CB_L2"), *was);
        throw;
    }
    if (l2 && !was) unset_env(S("CB_L2"));
    else if (l2) set_env(S("CB_L2"), *was);
    std::optional<py::dict<int64_t, py::bytes>> images;
    if (l2 && tree_images()) images = _segment_images(description);
    Decompiled out;
    out.order = r.order;
    out.by_va = r.by_va;
    out.stderr_ = r.stderr_;
    for (const auto& kv : r.texts.items()) {
        const py::str& n = std::get<0>(kv);
        auto [t, notes] = split_notes(std::get<1>(kv));
        out.notes.set(n, notes);
        std::optional<py::str> text = t;
        if (l2 && write) text = c_from_l2(t, const_cast<FunctionNotes&>(out.notes.at(n)), images, nullptr);
        out.texts.set(n, *text);
    }
    return out;
}

// The native program's own output for a PE module, before it is made compilable (`l2`, `write`: run's).
Decompiled raw(const py::str& module_path, const py::list<int64_t>& funcs, const py::str& outdir, int64_t timeout,
               const std::optional<bool>& l2, bool write) {
    auto [description, names] = describe::describe32(module_path, funcs, outdir);
    return run(description, py::list<int64_t>(), timeout, std::nullopt, l2, write);
}

// Decompile a module (every function, or the addresses in `funcs`). -> Module
c_text::Module32 decompile(const py::str& module_path, const std::optional<py::list<int64_t>>& funcs0, const std::optional<py::str>& outdir0) {
    py::list<int64_t> funcs = funcs0 ? *funcs0 : verify::lanes::win32::functions(S("pe"), module_path);
    py::str stem = py::os::path::basename(module_path).lower();
    py::str outdir = outdir0 ? *outdir0 : build::watcom::_scratch(py::list<py::str>(std::vector<py::str>{S("decompiler"), stem}));
    c_text::Compilable32 c = c_text::compilable32(module_path, funcs, outdir);
    // (one L2 Module: each function a MethodDecl with its C; the imports it calls, and those of its functions that take their
    // first argument in ECX -- and their second in EDX -- beside it; each function's L2 document, and its declaration as the
    // back end wrote its heading from it)
    c_text::Module32 m(stem, c.preamble, c.texts, c.order, c.by_va);
    m.imports = c.imports;
    m.in_ecx = c.in_ecx;
    m.in_edx = c.in_edx;
    m.l2 = c.docs;
    m.declarations = c.declarations;
    return m;
}

// 16-bit modules ----------------------------------------------------------------------------------

namespace {

const py::re::Pattern& PLACEHOLDER() {
    static const py::re::Pattern p = py::re::compile("func_0x([0-9a-f]{8})");
    return p;
}

std::optional<L2Doc> doc_n(const py::dict<py::str, FunctionNotes>& notes, const py::str& name) { return doc_of(notes, name); }

// (the text of a sequence of words joined by a space)
py::str mods_text(const Dict& doc) {
    std::vector<py::str> w;
    if (const Val::List* l = as_list(vget(doc, "mods")))
        for (const Val& x : l->vec())
            if (const py::str* s = std::get_if<py::str>(&x.v)) w.push_back(*s);
    return joins(w, " ");
}

// (count, [bytes of each]) of arguments declared so (`int2 a1`, `char *a2`): the analysis's own declarations, and a far pointer as
// its facts say -- `char *param_1` alone reads as a near one.
std::tuple<int64_t, py::list<py::str>> _argument_sizes(const std::vector<py::str>& params, const std::optional<FunctionNotes>& facts) {
    static const py::re::Pattern wide_rx = py::re::compile("\\b(u?int4|xunknown4|float4|undefined4)\\b");
    py::list<py::str> sizes;
    for (const py::str& p : params) {
        py::list<py::str> words = py::re::findall("\\w+", p);
        py::str name_ = words[-1];
        bool is_far = facts && facts->pointers.contains(name_) && facts->pointers.at(name_) == 4;
        bool wide = wide_rx.search(p).has_value();
        // (a far pointer stated as one: `4p` -- stated as a 4-byte number, 34:0146's string and 34:016A's copy were dereferenced
        // as near pointers, into the data segment)
        sizes.append(is_far || (p.contains(S("__far")) && p.contains(S("*"))) ? S("4p") : wide ? S("4") : S("2"));
    }
    return {static_cast<int64_t>(params.size()), sizes};
}

}  // namespace

// The addresses of places a function calls that are not known as functions yet (func_0x...): from its L2 document's calls, or
// without one from its text.
py::set<int64_t> _placeholders(const py::str& text, const std::optional<L2Doc>& doc) {
    py::set<int64_t> out;
    if (!doc) {
        for (const py::str& x : CALLED().findall(text).vec()) out.add(py::int_parse(x, 16));
        return out;
    }
    for (const auto& [n, k] : l2_calls(*doc)) {
        std::optional<py::re::Match> m = PLACEHOLDER().fullmatch(n);
        if (m) out.add(py::int_parse(m->group(1), 16));
    }
    return out;
}

// Does the function call `name`?
bool _calls(const py::str& text, const std::optional<L2Doc>& doc, const py::str& name) {
    if (!doc) return py::re::search("\\b" + py::re::escape(name).utf8() + "\\s*\\(", text).has_value();
    for (const auto& [n, k] : l2_calls(*doc))
        if (n == name) return true;
    return false;
}

// The number of arguments each call to `name` passes: in its L2 document's calls (`doc`), or without one in `text`.
py::list<int64_t> _call_arguments(const py::str& text, const py::str& name, const std::optional<L2Doc>& doc) {
    py::list<int64_t> out;
    if (doc) {
        for (const auto& [n, k] : l2_calls(*doc))
            if (n == name) out.append(k);
        return out;
    }
    const std::u32string& u = text.u32();
    for (const py::re::Match& m : py::re::finditer("\\b" + py::re::escape(name).utf8() + "\\s*\\(", text).vec()) {
        int64_t depth = 1, n = 0;
        size_t k = static_cast<size_t>(m.end());
        bool seen = false;
        while (k < u.size() && depth) {
            char32_t ch = u[k];
            if (ch == U'(' || ch == U'[') depth += 1;
            else if (ch == U')' || ch == U']') depth -= 1;
            else if (ch == U',' && depth == 1) n += 1;
            if (depth && !py::str(ch).isspace()) seen = true;
            k += 1;
        }
        out.append(seen ? n + 1 : 0);
    }
    return out;
}

// (count, [bytes of each]) of the arguments a function declares -- in its L2 document (facts["l2"]), or without one in its own
// text -- or None (`facts`: its facts, split_notes -- which pointers are four bytes).
std::optional<std::tuple<int64_t, py::list<py::str>>> _own_arguments(const py::str& text, const py::str& name,
                                                                     const std::optional<FunctionNotes>& facts) {
    if (facts && facts->l2) {
        std::vector<py::str> params;
        if (const Val::List* l = as_list(vget(*facts->l2, "params"))) {
            for (const Val& pv : l->vec()) {
                const Dict* p = std::get_if<Dict>(&pv.v);
                if (!p) throw py::AttributeError("param is not a dict");
                ir::Val nm = cback::_param_name(*p);
                const Val* ty = vget(*p, "type");
                const py::str* ns = std::get_if<py::str>(&nm.v);
                params.push_back(cback::type_text(ty ? *ty : Val(), ns ? *ns : S("")));
            }
        }
        return _argument_sizes(params, facts);
    }
    py::str head = py::split(text, S("{"), 1)[0];
    std::optional<py::re::Match> m = py::re::search("\\b" + py::re::escape(name).utf8() + "\\(([^)]*)\\)", head);
    if (!m) return std::nullopt;
    std::vector<py::str> params;
    for (const py::str& p : py::split(m->group(1), S(",")).vec())
        if (!p.strip().empty() && p.strip() != "void") params.push_back(p.strip());
    return _argument_sizes(params, facts);
}

namespace {

using Own = std::tuple<int64_t, py::list<py::str>>;

// A cdecl function's own arguments' sizes, when they come to the bytes its instructions read through BP (describe.args_read);
// otherwise None -- its own text is wrong too (SETUPX's 25:0F96 once declared one word and read a far pointer). Its convention
// from its L2 document (the words before its name), or without one from its text.
std::optional<Own> _own_if_read(const py::dict<py::str, py::str>& out, int64_t va, const py::str& name, const describe::Description* d,
                                const py::dict<py::str, FunctionNotes>& notes) {
    py::str text = out.get(name, S(""));
    std::optional<L2Doc> doc = doc_n(notes, name);
    py::str head = doc ? mods_text(*doc) : py::split(text, S("{"), 1)[0];
    if (!head.contains(S("__cdecl16"))) return std::nullopt;
    std::optional<FunctionNotes> fn = notes.contains(name) ? std::optional<FunctionNotes>(notes.at(name)) : std::nullopt;
    auto own = _own_arguments(text, name, fn);
    if (!own || !std::get<0>(*own)) return std::nullopt;
    if (d) {
        auto [seg, off] = describe::where(va);
        py::bytes code = d->segment_bytes.get(seg, py::bytes());
        std::optional<int64_t> later;
        for (int64_t v : d->names.keys()) {
            auto [sg, o] = describe::where(v);
            if (sg == seg && o > off) later = later ? std::min(*later, o) : o;
        }
        std::optional<int64_t> read = describe::args_read(code, off, later ? *later : code.size(), head.contains(S("16far")) ? S("far") : S("near"));
        int64_t sum = 0;
        for (const py::str& s : std::get<1>(*own).vec()) sum += py::int_parse(s.rstrip(S("p")), 10);
        if (!read || *read != sum) return std::nullopt;
    }
    return own;
}

// {address: [bytes of each argument]} of the cdecl functions some call passes fewer arguments than they themselves take (and
// whose own arguments are what they read: stated from a wrong text, the function was made worse).
std::vector<std::pair<int64_t, py::list<py::str>>> _arguments_lost(const py::dict<py::str, py::str>& out, const py::dict<int64_t, py::str>& by_va,
                                                                     const describe::Description* d, const py::dict<py::str, FunctionNotes>& notes) {
    std::vector<std::pair<int64_t, py::list<py::str>>> lost;
    for (const auto& kv : by_va.items()) {
        int64_t va = std::get<0>(kv);
        const py::str& name = std::get<1>(kv);
        auto own = _own_if_read(out, va, name, d, notes);
        if (!own) continue;
        for (const auto& ot : out.items()) {
            const py::str& other = std::get<0>(ot);
            const py::str& t = std::get<1>(ot);
            std::optional<L2Doc> doc = doc_n(notes, other);
            if (other == name || !mentions(t, doc, name)) continue;
            bool fewer = false;
            for (int64_t n : _call_arguments(t, name, doc).vec()) fewer = fewer || n < std::get<0>(*own);
            if (fewer) {
                lost.emplace_back(va, std::get<1>(*own));
                break;
            }
        }
    }
    return lost;
}

// The description's lines for these functions given their arguments' sizes (its 7th field, `4,2,2`: a far pointer one argument
// of four bytes, not two words -- as words, 34:0146's pointer was put back together as a near one, and SUWIN faulted in it).
void _state_arguments(describe::Description& d, const std::vector<std::pair<int64_t, py::list<py::str>>>& lost) {
    py::list<py::str>& lines = d.lines;
    for (int64_t i = 0; i < lines.size(); ++i) {
        py::list<py::str> parts = py::split(lines[i], S("\t"));
        if (parts[0] == "F" && parts.size() >= 5) {
            int64_t va = py::int_parse(parts[1], 16);
            for (const auto& [lva, sizes] : lost) {
                if (lva != va) continue;
                std::vector<py::str> ps = parts.vec();
                while (ps.size() < 6) ps.push_back(S("0"));
                ps.resize(6);   // parts[6:] = [",".join(...)]
                ps.push_back(py::join(S(","), sizes));
                lines[i] = joins(ps, "\t");
                break;
            }
        }
    }
    d.write();
}

// A function found only because another called it was decompiled after its callers, whose text still calls it `func_0x<address>` --
// a name nothing has. The callers are given its name.
py::str by_their_names_text(const py::str& t, const py::dict<py::str, py::str>& known) {
    static const py::re::Pattern placeholder = py::re::compile("\\bfunc_0x([0-9a-f]{8})\\b");
    return placeholder.sub([&](const py::re::Match& m) { return known.get(m.group(1), m.group(0)); }, t);
}

py::dict<py::str, py::str> known_names(const py::dict<int64_t, py::str>& names) {
    py::dict<py::str, py::str> known;
    for (const auto& kv : names.items()) known.set(X8(std::get<0>(kv)), std::get<1>(kv));
    return known;
}

py::dict<py::str, py::str> _by_their_names(const py::dict<py::str, py::str>& out, const py::dict<int64_t, py::str>& names) {
    py::dict<py::str, py::str> known = known_names(names);
    if (known.empty()) return out;
    py::dict<py::str, py::str> r;
    for (const auto& kv : out.items()) r.set(std::get<0>(kv), by_their_names_text(std::get<1>(kv), known));
    return r;
}

bool same_own(const std::optional<Own>& a, const std::optional<Own>& b) {
    if (!a || !b) return !a && !b;
    if (std::get<0>(*a) != std::get<0>(*b) || std::get<1>(*a).size() != std::get<1>(*b).size()) return false;
    for (int64_t i = 0; i < std::get<1>(*a).size(); ++i)
        if (std::get<1>(*a)[i] != std::get<1>(*b)[i]) return false;
    return true;
}

}  // namespace

// Decompile a 16-bit Windows module. -> ({name: text}, [names], {address: name}, Description)
//
// Function starts come first from what the file states (entry table, relocations, agreed near-call targets). Decompiling then
// shows calls to places not yet known as functions (`func_0x...`); those are added and the module is done again, until no new
// ones appear.
Decompiled16 decompile16(const py::str& module_path, const std::optional<py::str>& outdir0, int64_t rounds) {
    py::str outdir = outdir0 ? *outdir0 : build::watcom::_scratch(py::list<py::str>(std::vector<py::str>{S("decompiler16"), py::os::path::basename(module_path).lower()}));
    std::shared_ptr<describe::Description> dp = describe::describe16(module_path, outdir);
    describe::Description& d = *dp;
    Decompiled got = run(d.path, py::list<int64_t>(), 3600, std::nullopt, true, false);
    py::dict<py::str, py::str> out = got.texts;
    py::list<py::str> order = got.order;
    py::dict<int64_t, py::str> by_va = got.by_va;
    py::dict<py::str, FunctionNotes> notes = got.notes.copy();   // {name: its facts} (split_notes)
    for (int64_t round = 0; round < rounds; ++round) {
        std::set<int64_t> news;
        for (const auto& kv : out.items())
            for (int64_t va : _placeholders(std::get<1>(kv), doc_n(notes, std::get<0>(kv)))) news.insert(va);
        for (auto it = news.begin(); it != news.end();) {
            if (d.names.contains(*it) || !describe::inside(d, *it)) it = news.erase(it);
            else ++it;
        }
        if (news.empty()) break;
        for (int64_t va : news) {
            core16::SegOff so = describe::where(va);
            d.names.set(va, py::str("sub_") + D(std::get<0>(so)) + S("_") + XU(std::get<1>(so), 4));
            d.where.set(va, so);
            d.lines.append(describe::model_line(d, va, d.names.at(va)));
        }
        d.write();
        // the new ones -- and again every function that calls one: it was made while the callee was unknown, a default near
        // function (a far call's pushed CS read as its first argument, the last one lost); now its prototype is known
        std::set<int64_t> callers;
        for (const auto& kv : by_va.items()) {
            for (int64_t c : news)
                if (mentions(out.get(std::get<1>(kv), S("")), doc_n(notes, std::get<1>(kv)), S("func_0x") + X8(c))) {
                    callers.insert(std::get<0>(kv));
                    break;
                }
        }
        std::set<int64_t> both = news;
        both.insert(callers.begin(), callers.end());
        Decompiled more = run(d.path, py::list<int64_t>(std::vector<int64_t>(both.begin(), both.end())), 3600, std::nullopt, true, false);
        out.update(more.texts);
        notes.update(more.notes);
        by_va.update(more.by_va);
    }
    // A function its callers clean up after (cdecl) has its arguments left to the analysis, which decides at each call on its own:
    // SETUPX's allocator 24:0F1E takes a size, and 11 of its 25 callers were made to pass none (sub_3_7E58's block got whatever
    // the stack held: Setup's SU99405). Where a call passes fewer than the function itself takes, the function's own argument
    // words are stated, and it and its callers are done again. (Stated for every such function, all 578 of SETUPX's texts
    // changed, their pointers made words, and new faults.)
    auto lost = _arguments_lost(out, by_va, &d, notes);
    if (!lost.empty()) {
        _state_arguments(d, lost);
        std::set<std::string> names;
        for (const auto& l : lost) names.insert(by_va.at(l.first).utf8());
        std::set<int64_t> callers;
        for (const auto& kv : by_va.items())
            for (const std::string& c : names)
                if (S(c) != std::get<1>(kv) && _calls(out.get(std::get<1>(kv), S("")), doc_n(notes, std::get<1>(kv)), S(c))) {
                    callers.insert(std::get<0>(kv));
                    break;
                }
        std::set<int64_t> both = callers;
        for (const auto& l : lost) both.insert(l.first);
        Decompiled more = run(d.path, py::list<int64_t>(std::vector<int64_t>(both.begin(), both.end())), 3600, std::nullopt, true, false);
        // (and of what is done again, only the callers that passed too few are taken: the rest -- the stated functions themselves,
        // callers that passed all -- kept as they were. Taken whole, the analysis's second look made other mistakes: 25:0F96 lost
        // one of its own two arguments, 30:00F6 wrote through a pointer it had read right before)
        std::set<std::string> shortn;
        for (int64_t va : callers) {
            py::str n = by_va.at(va);
            bool any = false;
            for (const auto& l : lost) {
                py::str cn = by_va.at(l.first);
                if (cn == n) continue;
                for (int64_t k : _call_arguments(out.get(n, S("")), cn, doc_n(notes, n)).vec())
                    if (k < l.second.size()) any = true;
            }
            if (any) shortn.insert(n.utf8());
        }
        // (and one whose own arguments the second look changed is not taken either: 25:0F96, two words become one, faulted -- its
        // calls passed in full, its own frame read wrong)
        std::vector<py::str> taken;
        for (const auto& kv : more.texts.items()) {
            const py::str& n = std::get<0>(kv);
            if (!shortn.count(n.utf8())) continue;
            std::optional<FunctionNotes> mf = more.notes.contains(n) ? std::optional<FunctionNotes>(more.notes.at(n)) : std::nullopt;
            std::optional<FunctionNotes> of = notes.contains(n) ? std::optional<FunctionNotes>(notes.at(n)) : std::nullopt;
            if (same_own(_own_arguments(std::get<1>(kv), n, mf), _own_arguments(out.get(n, S("")), n, of))) taken.push_back(n);
        }
        for (const py::str& n : taken) {
            out.set(n, more.texts.at(n));
            notes.set(n, more.notes.at(n));
        }
    }
    // a helper made part of its callers (describe.register_helper: `inline`) is still a function of the module, at its place, for
    // whatever else reaches it: its own text, from the description with it as an ordinary function
    if (!d.inlined.empty()) {
        describe::Description own(py::os::path::join(py::os::path::dirname(d.path), S("description_own.txt")), py::dict<int64_t, py::str>(),
                                  py::dict<int64_t, core16::SegOff>(), py::list<py::str>());
        for (const py::str& ln0 : d.lines.vec()) {
            py::str ln = ln0;
            py::list<py::str> p = py::split(ln, S("\t"));
            if (p[0] == "F" && p.size() > 4 && p[4].strip() == "inline" && d.inlined.contains(py::int_parse(p[1], 16)))
                ln = describe::model_line(d, py::int_parse(p[1], 16), p[2]);
            own.lines.append(ln);
        }
        py::list<int64_t> inl(std::vector<int64_t>(d.inlined.begin(), d.inlined.end()));
        Decompiled more = run(own.write(), py::sorted(inl), 3600, std::nullopt, true, false);
        out.update(more.texts);
        notes.update(more.notes);
        by_va.update(more.by_va);
    }
    out = _by_their_names(out, d.names);
    py::dict<py::str, py::str> known = known_names(d.names);
    for (const auto& kv : notes.items()) {   // (its L2 document's calls by those names too)
        FunctionNotes& facts = const_cast<FunctionNotes&>(std::get<1>(kv));
        if (!facts.l2 || known.empty()) continue;
        py::str dumped = py::str(::py::json_dump(native_l2::to_wire(Val(*facts.l2))));
        py::str changed = by_their_names_text(dumped, known);
        if (changed == dumped) continue;
        Val v = native_l2::from_wire(::py::json_parse(changed.utf8()));
        facts.l2 = std::get<Dict>(v.v);
    }
    py::dict<py::str, FunctionNotes> dn;
    for (const py::str& n : out.keys())
        if (notes.contains(n)) dn.set(n, notes.at(n));
    d.notes = dn;
    py::list<py::str> order2;
    for (int64_t va : py::sorted(py::list<int64_t>(by_va.keys())).vec()) order2.append(by_va.at(va));
    return Decompiled16{out, order2, by_va, dp};
}

namespace {

// Decompile a description; then, while the output calls places not yet known as functions (`func_0x...`), add those inside the
// module's code and decompile them too.
struct Grown {
    py::dict<py::str, py::str> texts;
    py::list<py::str> order;
    py::dict<int64_t, py::str> by_va;
};

Grown _grow(describe::Description& d, const std::function<std::tuple<py::str, py::str>(int64_t)>& line_for, int64_t rounds = 4) {
    Decompiled first = run(d.path);
    py::dict<py::str, py::str> out = first.texts;
    py::dict<int64_t, py::str> by_va = first.by_va;
    for (int64_t round = 0; round < rounds; ++round) {
        std::set<int64_t> news;
        for (const auto& kv : out.items())
            for (const py::str& x : CALLED().findall(std::get<1>(kv)).vec()) news.insert(py::int_parse(x, 16));
        for (auto it = news.begin(); it != news.end();) {
            bool inside = false;
            for (const auto& sp : d.spans.values())
                inside = inside || (std::get<0>(sp) <= *it && *it < std::get<0>(sp) + std::get<1>(sp));
            if (d.names.contains(*it) || !inside) it = news.erase(it);
            else ++it;
        }
        if (news.empty()) break;
        for (int64_t va : news) {
            auto [name, line] = line_for(va);
            d.names.set(va, name);
            d.lines.append(line);
        }
        d.write();
        std::set<int64_t> both = news;   // (made while these were unknown)
        for (const auto& kv : by_va.items())
            for (int64_t c : news)
                if (out.get(std::get<1>(kv), S("")).contains(S("func_0x") + X8(c))) {
                    both.insert(std::get<0>(kv));
                    break;
                }
        Decompiled more = run(d.path, py::list<int64_t>(std::vector<int64_t>(both.begin(), both.end())));
        out.update(more.texts);
        by_va.update(more.by_va);
    }
    out = _by_their_names(out, d.names);
    Grown g;
    g.texts = out;
    for (int64_t va : py::sorted(py::list<int64_t>(by_va.keys())).vec()) g.order.append(by_va.at(va));
    g.by_va = by_va;
    return g;
}

}  // namespace

// Decompile a virtual device driver file: one LE image, or every member of a container.
py::list<LeMember> decompile_le(const py::str& module_path, const std::optional<py::str>& outdir0) {
    py::str outdir = outdir0 ? *outdir0 : build::watcom::_scratch(py::list<py::str>(std::vector<py::str>{S("decompiler_le"), py::os::path::basename(module_path).lower()}));
    py::list<LeMember> results;
    for (const auto& [name, lf] : formats::le::open_vxds(module_path).vec()) {
        std::shared_ptr<describe::Description> d =
            describe::describe_le(lf, name, py::os::path::join(outdir, py::re::sub("\\W", S("_"), name)));
        auto line = [](int64_t va) -> std::tuple<py::str, py::str> {
            int64_t obj = py::floordiv(va - describe::LE_BASE, describe::LE_STRIDE) + 1;
            int64_t off = py::mod(va - describe::LE_BASE, describe::LE_STRIDE);
            py::str n = S("sub_") + D(obj) + S("_") + XU(off, 5);
            return {n, S("F\t") + X(va) + S("\t") + n + S("\t-1")};
        };
        Grown g = _grow(*d, line);
        results.append(LeMember{d->device ? *d->device : S(""), g.texts, g.order, d});
    }
    return results;
}

// Decompile a DOS program. -> ({name: text}, [names], Description)
DosDecompiled decompile_dos(const py::str& module_path, const std::optional<py::str>& outdir0) {
    py::str outdir = outdir0 ? *outdir0 : build::watcom::_scratch(py::list<py::str>(std::vector<py::str>{S("decompiler_dos"), py::os::path::basename(module_path).lower()}));
    std::shared_ptr<describe::Description> d = describe::describe_dos(module_path, outdir);
    auto line = [&](int64_t va) -> std::tuple<py::str, py::str> {
        auto [kind, pop] = describe::returns(d->image, va - d->base);
        py::str n = S("sub_") + XU(va - d->base, 5);
        return {n, S("F\t") + X(va) + S("\t") + n + S("\t-1\t__cdecl16") + (kind ? *kind : S("near"))};
    };
    Grown g = _grow(*d, line, 8);
    py::dict<py::str, py::str> out;
    for (const auto& kv : g.texts.items()) {
        py::list<symbols::dos_interrupts::Interrupt> calls;
        if (d->interrupts.contains(std::get<0>(kv)))
            for (const analyze::dos_functions::Interrupt& i : d->interrupts.at(std::get<0>(kv)).vec())
                calls.append(symbols::dos_interrupts::Interrupt{i.number, i.ah, i.al, i.address});
        out.set(std::get<0>(kv), symbols::dos_interrupts::annotate(std::get<1>(kv), calls));
    }
    return DosDecompiled{out, g.order, d};
}

bool _is_x86(const py::str& module_path) { return formats::pe::open_pe(module_path).machine == 0x014C; }

// Any executable as C: (text, functions decompiled, functions found). Raises ValueError for a file that is not an executable this
// can read.
std::tuple<py::str, int64_t, int64_t> as_c(const py::str& path) {
    namespace B = build::watcom;
    std::optional<py::str> kind = formats::nes::is_nes(path) ? std::optional<py::str>(S("nes")) : formats::detect::kind(path);
    py::dict<py::str, py::str> texts;
    py::list<py::str> order;
    py::str header;
    int64_t total = 0;
    py::str stem = py::os::path::basename(path).lower();
    if (kind && *kind == "ne") {
        Decompiled16 r = decompile16(path);
        texts = r.texts;
        order = r.order;
        header = S("");
        total = r.description->names.size();
    } else if (kind && *kind == "pe" && _is_x86(path)) {
        c_text::Module32 m = decompile(path);
        texts = m.texts();
        order = m.order();
        header = m.header();
        total = m.by_va().size();
    } else if (kind && *kind == "pe") {   // PowerPC: as the decompiler gives it
        py::list<int64_t> starts = describe::function_starts(path);
        Decompiled r = raw(path, starts, B::_scratch(py::list<py::str>(std::vector<py::str>{S("decompiler"), stem})));
        texts = r.texts;
        order = r.order;
        header = S("");
        total = starts.size();
    } else if (kind && *kind == "nes") {   // the 6502, as the decompiler gives it
        auto [description, by_va] = describe::describe_nes(path, B::_scratch(py::list<py::str>(std::vector<py::str>{S("decompiler_nes"), stem})));
        Decompiled r = run(description);
        texts = r.texts;
        order = r.order;
        header = S("");
        total = by_va.size();
    } else if (kind && *kind == "le") {
        header = S("");
        for (const LeMember& mb : decompile_le(path).vec()) {
            total += mb.description->names.size();
            for (const py::str& n : mb.order.vec()) {
                py::str key = mb.member + S(".") + n;
                texts.set(key, S("/* driver ") + mb.member + S(" */\n") + mb.texts.at(n));
                order.append(key);
            }
        }
    } else if (kind && *kind == "dos") {
        py::bytes data = read_bytes(path);
        auto parts = formats::dos::chain(data);
        header = S("");
        if (parts.empty()) {
            DosDecompiled r = decompile_dos(path);
            texts = r.texts;
            order = r.order;
            total = r.description->names.size();
        } else {   // programs one after another: each its own
            int64_t k = 1;
            for (const auto& [at, size] : parts.vec()) {
                py::str part = B::_scratch(py::list<py::str>(std::vector<py::str>{S("decompiler_dos"), stem + S(".part") + D(k), stem + S(".part") + D(k) + S(".exe")}));
                py::os::makedirs(py::os::path::dirname(part), 0777, true);
                py::file f = py::open(part, S("wb"));
                f.write(data.slice(at, at + size));
                f.close();
                DosDecompiled r = decompile_dos(part);
                total += r.description->names.size();
                for (const py::str& n : r.order.vec()) {
                    py::str key = S("part") + D(k) + S(".") + n;
                    texts.set(key, S("/* part ") + D(k) + S(" of ") + D(parts.size()) + S(": offset ") + XU(at, 1) + S("h, ") + D(size) +
                                       S(" bytes */\n") + r.texts.at(n));
                    order.append(key);
                }
                ++k;
            }
        }
    } else {
        throw py::ValueError(path.utf8() + ": not an executable this can read");
    }
    for (const py::str& n : texts.keys()) texts.set(n, texts.at(n).strip());
    std::vector<py::str> bodies;
    for (const py::str& n : order.vec()) bodies.push_back(texts.at(n));
    py::str text = S("/* ") + py::os::path::basename(path) + S(" -- decompiled by CodeBreaker; not built, not judged. */\n\n") +
                   (!header.empty() ? header + S("\n\n") : S("")) + joins(bodies, "\n\n") + S("\n");
    return {text, order.size(), total};
}

// python -m codebreaker decompile <module> [--out FILE]
//
// Decompile any module the corpus or a path names -- a 32-bit or 16-bit Windows module, a virtual device driver, a DOS program, a
// PowerPC image, a NES cartridge -- and write its functions as C. Nothing is built or judged; `module` does that.
int64_t main(const py::list<py::str>& argv) {
    if (argv.empty()) {
        std::cout << "python -m codebreaker decompile <module> [--out FILE]\n\n"
                     "    Decompile any module the corpus or a path names -- a 32-bit or 16-bit Windows module, a\n"
                     "    virtual device driver, a DOS program, a PowerPC image, a NES cartridge -- and write its functions as C. "
                     "Nothing is built or judged; `module` does that.\n";
        return 2;
    }
    std::optional<py::str> path = base::corpus::retail(argv[0]);
    if (!path) throw py::SystemExit(py::str("no module ") + py::repr(argv[0]));
    py::str text;
    int64_t decompiled = 0, total = 0;
    try {
        std::tie(text, decompiled, total) = as_c(*path);
    } catch (const py::ValueError& e) {
        throw py::SystemExit(py::str(std::string(e.what())));
    }
    std::optional<base::converted::Project> proj;
    py::str out;
    if (argv.contains(S("--out"))) {
        out = argv[argv.index(S("--out")) + 1];
    } else {   // the binary's project folder: Converted/<name>/src/
        proj = base::converted::project(*path);
        out = py::os::path::join(proj->src, std::get<0>(py::os::path::splitext(py::os::path::basename(*path))).lower() + S(".c"));
    }
    py::file f = py::open(out, S("w"), S("utf-8"), std::nullopt, S("\n"));
    f.write(text);
    f.close();
    if (proj)
        proj->record(S("decompile"), py::list<py::str>(std::vector<py::str>{out}),
                     D(decompiled) + S(" of ") + D(total) + S(" functions as C (readable; not built or judged)"));
    std::cout << (py::os::path::basename(*path) + S(": ") + D(decompiled) + S(" of ") + D(total) + S(" functions -> ") + out).utf8() << "\n";
    return 0;
}

}  // namespace codebreaker::decompile::decompiler
