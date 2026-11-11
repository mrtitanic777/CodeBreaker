// codebreaker/platforms/package.cpp -- typed C++ port of codebreaker/platforms/__init__.py (see package.hpp).
#include "codebreaker/platforms/package.hpp"

#include <map>
#include <mutex>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/formats/pe.hpp"
#include "codebreaker/formats/xex.hpp"
#include "codebreaker/platforms/dos.hpp"
#include "codebreaker/platforms/gb.hpp"
#include "codebreaker/platforms/genesis.hpp"
#include "codebreaker/platforms/msx.hpp"
#include "codebreaker/platforms/nes.hpp"
#include "codebreaker/platforms/sms.hpp"
#include "codebreaker/platforms/vxd.hpp"
#include "codebreaker/platforms/win16.hpp"
#include "codebreaker/verify/lanes/java.hpp"
#include "py_os.hpp"

namespace codebreaker::platforms {

using py::str;

static py::list<str> strs(std::initializer_list<const char*> items) {
    py::list<str> out;
    for (const char* s : items) out.append(str(s));
    return out;
}

const py::list<str> ORDER = strs({"java", "xbox360", "xbox", "nes", "sms", "gb", "genesis", "zx", "msx", "win32",
                                  "win16", "vxd", "dos"});
const py::list<str> SLOTS = strs({"loader", "cpu", "convention", "emulator", "compiler", "packager"});

// ---- reprs ------------------------------------------------------------------------------------------
// a tuple of strings as Python writes it: ('a', 'b'), ('a',), ()
static str tuple_repr(const py::list<str>& xs) {
    std::string out = "(";
    bool first = true;
    for (const str& s : xs) {
        if (!first) out += ", ";
        out += py::repr(s).utf8();
        first = false;
    }
    if (xs.size() == 1) out += ",";
    out += ")";
    return str(out);
}

static str join_given(const char* cls, const py::list<std::tuple<str, str>>& given) {
    std::string out = std::string(cls) + "(";
    bool first = true;
    for (const auto& kv : given) {
        if (!first) out += ", ";
        out += std::get<0>(kv).utf8() + "=" + std::get<1>(kv).utf8();
        first = false;
    }
    return str(out + ")");
}

// ---- Ref ---------------------------------------------------------------------------------------------
std::tuple<str, str> Ref::pair() const { return {module, name}; }

str Ref::repr() const {
    std::vector<std::string> parts;
    for (const str& a : args) parts.push_back(py::repr(a).utf8());
    std::vector<std::pair<std::string, std::string>> items;
    for (const auto& kv : kw) items.emplace_back(std::get<0>(kv).utf8(), py::repr(std::get<1>(kv)).utf8());
    std::sort(items.begin(), items.end());
    for (const auto& kv : items) parts.push_back(kv.first + "=" + kv.second);
    std::string joined;
    for (size_t i = 0; i < parts.size(); i++) joined += (i ? ", " : "") + parts[i];
    return str(module.utf8() + "." + name.utf8() + (parts.empty() ? "" : "(" + joined + ")"));
}

bool operator==(const Ref& a, const Ref& b) {
    if (!(a.module == b.module && a.name == b.name)) return false;
    if (a.args.size() != b.args.size() || a.kw.size() != b.kw.size()) return false;
    for (int64_t i = 0; i < a.args.size(); i++)
        if (!(a.args[i] == b.args[i])) return false;
    // keyword arguments compare as dicts: by name, order does not matter
    for (const auto& x : a.kw) {
        bool found = false;
        for (const auto& y : b.kw)
            if (std::get<0>(x) == std::get<0>(y) && std::get<1>(x) == std::get<1>(y)) found = true;
        if (!found) return false;
    }
    return true;
}

// ---- slots -------------------------------------------------------------------------------------------
using Given = py::list<std::tuple<str, str>>;

static void add(Given& g, const char* field, const std::optional<Ref>& r) {
    if (r) g.append({str(field), r->repr()});
}
static void add(Given& g, const char* field, const std::optional<str>& s) {
    if (s) g.append({str(field), py::repr(*s)});
}
static void add(Given& g, const char* field, const std::optional<int64_t>& n) {
    if (n) g.append({str(field), py::repr(*n)});
}
static void add(Given& g, const char* field, const std::optional<py::list<str>>& l) {
    if (l) g.append({str(field), tuple_repr(*l)});
}

const py::list<str>& Loader::FIELDS() {
    static const py::list<str> f = strs({"container", "open", "readable", "parts", "entries", "exports", "imports"});
    return f;
}
Given Loader::given() const {
    Given g;
    add(g, "container", container);
    add(g, "open", open);
    add(g, "readable", readable);
    add(g, "parts", parts);
    add(g, "entries", entries);
    add(g, "exports", exports);
    add(g, "imports", imports);
    return g;
}
bool Loader::given_any() const { return given().size() > 0; }
str Loader::repr() const { return join_given("Loader", given()); }

const py::list<str>& Cpu::FIELDS() {
    static const py::list<str> f = strs({"bits", "endian", "decoder", "registers", "stack"});
    return f;
}
Given Cpu::given() const {
    Given g;
    if (!language.empty()) g.append({str("language"), py::repr(language)});
    add(g, "bits", bits);
    add(g, "endian", endian);
    if (const auto* c = std::get_if<Capstone>(&decoder)) {
        g.append({str("decoder"), str("('capstone', ") + py::repr(c->arch) + str(", ") + py::repr(c->bits) + str(")")});
    } else if (const auto* m = std::get_if<Module>(&decoder)) {
        g.append({str("decoder"), py::repr(m->name)});
    }
    add(g, "registers", registers);
    add(g, "stack", stack);
    return g;
}
str Cpu::repr() const { return py::repr(language); }

const py::list<str>& Convention::FIELDS() {
    static const py::list<str> f = strs({"names", "arguments", "word", "order", "results", "pops", "preserved"});
    return f;
}
Given Convention::given() const {
    Given g;
    add(g, "names", names);
    if (const auto* n = std::get_if<int64_t>(&arguments)) g.append({str("arguments"), py::repr(*n)});
    else if (const auto* s = std::get_if<str>(&arguments)) g.append({str("arguments"), py::repr(*s)});
    add(g, "word", word);
    add(g, "order", order);
    add(g, "results", results);
    add(g, "pops", pops);
    add(g, "preserved", preserved);
    return g;
}
bool Convention::given_any() const { return given().size() > 0; }
str Convention::repr() const { return join_given("Convention", given()); }

const py::list<str>& Emulator::FIELDS() {
    static const py::list<str> f = strs({"run", "call", "engine"});
    return f;
}
Given Emulator::given() const {
    Given g;
    add(g, "run", run);
    add(g, "call", call);
    add(g, "engine", engine);
    return g;
}
bool Emulator::given_any() const { return given().size() > 0; }
str Emulator::repr() const { return join_given("Emulator", given()); }

const py::list<str>& Compiler::FIELDS() {
    static const py::list<str> f = strs({"language", "toolchain", "tool", "tools", "flags", "env", "compile", "library"});
    return f;
}
Given Compiler::given() const {
    Given g;
    add(g, "language", language);
    add(g, "toolchain", toolchain);
    add(g, "tool", tool);
    add(g, "tools", tools);
    if (const auto* l = std::get_if<py::list<str>>(&flags)) g.append({str("flags"), tuple_repr(*l)});
    else if (const auto* r = std::get_if<Ref>(&flags)) g.append({str("flags"), r->repr()});
    add(g, "env", env);
    add(g, "compile", compile);
    add(g, "library", library);
    return g;
}
bool Compiler::given_any() const { return given().size() > 0; }
str Compiler::repr() const { return join_given("Compiler", given()); }
std::optional<py::list<str>> Compiler::options() const {
    if (const auto* l = std::get_if<py::list<str>>(&flags)) return *l;
    if (std::holds_alternative<std::monostate>(flags)) return py::list<str>();   // list(f or ())
    return std::nullopt;                                                          // a Ref: its target is read elsewhere
}

const py::list<str>& Packager::FIELDS() {
    static const py::list<str> f = strs({"link", "functions", "writer", "tools", "linker", "assembler", "resources"});
    return f;
}
Given Packager::given() const {
    Given g;
    add(g, "link", link);
    add(g, "functions", functions);
    add(g, "writer", writer);
    add(g, "tools", tools);
    add(g, "linker", linker);
    add(g, "assembler", assembler);
    add(g, "resources", resources);
    return g;
}
bool Packager::given_any() const { return given().size() > 0; }
str Packager::repr() const { return join_given("Packager", given()); }

// ---- Platform ------------------------------------------------------------------------------------------
void Platform::finish(const std::optional<std::tuple<str, str>>& runner_, const std::optional<Ref>& open_,
                      const str& container_) {
    // self.steps = steps or ("codebreaker.engine.lanes" if lane else None)
    if (!steps || steps->empty()) steps = (lane && !lane->empty()) ? std::optional<str>(str("codebreaker.engine.lanes")) : std::nullopt;
    if (!container_.empty() && !(loader.container && !loader.container->empty())) loader.container = container_;
    if (open_ && !loader.readable) loader.readable = *open_;
    if (runner_ && !emulator.run) emulator.run = Ref(std::get<0>(*runner_), std::get<1>(*runner_));
}

str Platform::container() const { return loader.container ? *loader.container : str(); }

std::optional<std::tuple<str, str>> Platform::runner() const {
    if (emulator.run) return emulator.run->pair();
    return std::nullopt;
}

py::list<std::tuple<str, str>> Platform::slots() const {
    return {{str("loader"), loader.repr()}, {str("cpu"), cpu.repr()}, {str("convention"), convention.repr()},
            {str("emulator"), emulator.repr()}, {str("compiler"), compiler.repr()}, {str("packager"), packager.repr()}};
}

bool Platform::recognises(const str& path) const {
    try {
        return recognise_fn ? recognise_fn(path) : false;
    } catch (const py::OSError&) {
        return false;
    } catch (const py::ValueError&) {
        return false;
    }
}

str Platform::readable(const str& path) const {
    if (loader.readable) {
        if (!readable_fn) throw py::RuntimeError("no readable function is installed for " + name.utf8());
        return readable_fn(path);
    }
    return path;
}

std::optional<str> Platform::lane_module() const { return lane && !lane->empty() ? lane : std::nullopt; }
std::optional<str> Platform::steps_module() const { return steps && !steps->empty() ? steps : std::nullopt; }
std::optional<Ref> Platform::run_function() const { return emulator.run; }
str Platform::repr() const { return str("<platform ") + name + str(": ") + title + str(">"); }

Platform machine(const str& name, const str& title, const str& family, const str& container, const Cpu& cpu,
                 const Recogniser& recognise, const std::tuple<str, str>& reader, const std::tuple<str, str>& runner,
                 const std::tuple<str, str>& assembler, const std::tuple<str, str>& writer,
                 const py::list<str>& results, const str& toolchain) {
    Platform p;
    p.name = name;
    p.title = title;
    p.family = family;
    p.source = str("asm");
    p.lane = str("codebreaker.verify.lanes.") + name;
    p.recognise_fn = recognise;
    p.cpu = cpu;
    p.loader.container = container;
    p.loader.open = Ref(std::get<0>(reader), std::get<1>(reader));
    p.convention.names = strs({"registers"});
    p.convention.arguments = str("registers");
    p.convention.order = str("registers");
    p.convention.results = results;
    p.convention.pops = str("caller");
    p.emulator.engine = str("CodeBreaker's own interpreter");
    p.emulator.run = Ref(std::get<0>(runner), std::get<1>(runner));
    p.compiler.language = str("asm");
    p.compiler.toolchain = toolchain;
    p.compiler.tool = Ref(std::get<0>(assembler), std::get<1>(assembler));
    p.packager.writer = Ref(std::get<0>(writer), std::get<1>(writer));
    p.finish();
    return p;
}

// ---- the descriptors ----------------------------------------------------------------------------------
static std::function<bool(const str&)>& zx_reader() {
    static std::function<bool(const str&)> r;
    return r;
}
void set_zx_reader(const std::function<bool(const str&)>& reader) { zx_reader() = reader; }

static std::function<str(const str&)>& xbe_to_pe() {
    static std::function<str(const str&)> r;
    return r;
}
void set_xbe_to_pe(const std::function<str(const str&)>& to_pe) { xbe_to_pe() = to_pe; }

std::optional<str> lane_of(const str& path) { return base::corpus::lane(path); }

int64_t pe_machine(const str& path) { return formats::pe::open_pe(path).machine; }

bool is_folder_or_java(const str& path) {
    return py::os::path::isdir(path) || path.lower().endswith(std::make_tuple(str(".java"), str(".jar")));
}

template <class R>
static auto args_of(const R& r, int) -> decltype(r.args, py::list<str>()) { return r.args; }
template <class R>
static py::list<str> args_of(const R&, long) { return {}; }

// a ported descriptor's Ref (module/name[/args]); an unset Ref (empty module and name) is None
template <class R>
static std::optional<Ref> cref(const R& r) {
    if (r.module.empty() && r.name.empty()) return std::nullopt;
    return Ref(r.module, r.name, args_of(r, 0));
}

template <class Cp>
static Cpu ccpu_capstone(const Cp& c) {
    Cpu o;
    o.language = c.language;
    o.bits = c.bits;
    o.endian = c.endian;
    o.decoder = Cpu::Capstone{std::get<1>(c.decoder), std::get<2>(c.decoder)};
    o.registers = c.registers;
    o.stack = c.stack;
    return o;
}

static Platform from_win16() {
    const win16::Platform& w = win16::PLATFORM();
    Platform p;
    p.name = w.name; p.title = w.title; p.family = w.family; p.source = w.source; p.lane = w.lane;
    p.steps = w.steps;
    p.recognise_fn = [](const str& path) { return win16::_recognise(path); };
    p.cpu = ccpu_capstone(w.cpu);
    p.loader.container = w.loader.container;
    p.loader.open = cref(w.loader.open);
    p.loader.parts = cref(w.loader.parts);
    p.loader.entries = cref(w.loader.entries);
    p.loader.exports = cref(w.loader.exports);
    p.loader.imports = cref(w.loader.imports);
    p.convention.names = w.convention.names;
    p.convention.arguments = w.convention.arguments;
    p.convention.word = w.convention.word;
    p.convention.order = w.convention.order;
    p.convention.results = w.convention.results;
    p.convention.pops = w.convention.pops;
    p.convention.preserved = w.convention.preserved;
    p.emulator.engine = w.emulator.engine;
    p.emulator.run = cref(w.emulator.run);
    p.emulator.call = cref(w.emulator.call);
    p.compiler.language = w.compiler.language;
    p.compiler.toolchain = w.compiler.toolchain;
    p.compiler.tool = cref(w.compiler.tool);
    p.compiler.tools = cref(w.compiler.tools);
    if (auto f = cref(w.compiler.flags)) p.compiler.flags = *f;
    p.compiler.env = cref(w.compiler.env);
    p.compiler.compile = cref(w.compiler.compile);
    p.compiler.library = cref(w.compiler.library);
    p.packager.link = cref(w.packager.link);
    p.packager.functions = cref(w.packager.functions);
    p.packager.tools = cref(w.packager.tools);
    p.packager.linker = cref(w.packager.linker);
    p.packager.assembler = cref(w.packager.assembler);
    p.packager.resources = cref(w.packager.resources);
    p.finish();
    return p;
}

static Platform from_dos() {
    const dos::Platform& w = dos::PLATFORM();
    Platform p;
    p.name = w.name; p.title = w.title; p.family = w.family; p.source = w.source; p.lane = w.lane;
    p.recognise_fn = [](const str& path) { return dos::_recognise(path); };
    p.cpu = ccpu_capstone(w.cpu);
    p.loader.container = w.loader.container;
    p.loader.open = cref(w.loader.open);
    p.loader.parts = cref(w.loader.parts);
    p.convention.names = w.convention.names;
    p.convention.word = w.convention.word;
    p.convention.order = w.convention.order;
    p.convention.results = w.convention.results;
    p.convention.pops = w.convention.pops;
    // (emulator=Emulator(): nothing is run)
    p.compiler.language = w.compiler.language;
    p.compiler.toolchain = w.compiler.toolchain;
    p.compiler.tool = cref(w.compiler.tool);
    p.compiler.tools = cref(w.compiler.tools);
    p.packager.writer = cref(w.packager.writer);
    p.packager.tools = cref(w.packager.tools);
    p.finish();
    return p;
}

static Platform from_vxd() {
    const vxd::Platform& w = vxd::PLATFORM();
    Platform p;
    p.name = w.name; p.title = w.title; p.family = w.family; p.source = w.source; p.lane = w.lane;
    p.recognise_fn = [](const str& path) { return vxd::_recognise(path); };
    p.cpu = ccpu_capstone(w.cpu);
    p.loader.container = w.loader.container;
    p.loader.open = cref(w.loader.open);
    p.loader.entries = cref(w.loader.entries);
    p.loader.parts = cref(w.loader.parts);
    p.convention.names = w.convention.names;
    p.convention.arguments = w.convention.arguments;
    p.convention.word = w.convention.word;
    p.convention.order = w.convention.order;
    p.convention.results = w.convention.results;
    p.convention.pops = w.convention.pops;
    p.convention.preserved = w.convention.preserved;
    p.emulator.engine = w.emulator.engine;
    p.emulator.run = cref(w.emulator.run);
    p.compiler.language = w.compiler.language;
    p.compiler.toolchain = w.compiler.toolchain;
    p.compiler.tool = cref(w.compiler.tool);
    p.compiler.tools = cref(w.compiler.tools);
    p.packager.writer = cref(w.packager.writer);
    p.packager.functions = cref(w.packager.functions);
    p.packager.tools = cref(w.packager.tools);
    p.finish();
    return p;
}

// nes, sms, gb, genesis, msx: platforms.machine(...) values held as msx::Platform records
static Platform from_machine(const msx::Platform& w, const Recogniser& rec) {
    Cpu c;
    c.language = w.cpu.language;
    c.bits = w.cpu.bits;
    c.endian = w.cpu.endian;
    c.decoder = Cpu::Module{w.cpu.decoder};
    c.registers = w.cpu.registers;
    c.stack = w.cpu.stack;
    return machine(w.name, w.title, w.family, w.container, c, rec, {w.reader.module, w.reader.name},
                   {w.runner.module, w.runner.name}, {w.assembler.module, w.assembler.name},
                   {w.writer.module, w.writer.name}, w.results, w.toolchain);
}

// java: a folder of source (or a .jar), translated to C++ and judged by running both under the JVM
static Platform make_java() {
    Platform p;
    p.name = "java"; p.title = "Java"; p.family = "bytecode"; p.source = str("java -> c++");
    p.lane = str("codebreaker.verify.lanes.java");
    p.recognise_fn = [](const str& path) {
        if (path.lower().endswith(str(".jar"))) return true;
        if (py::os::path::isdir(path) || path.lower().endswith(str(".java"))) return verify::lanes::java::is_module(path);
        return false;
    };
    p.cpu.language = "";
    p.cpu.decoder = Cpu::Module{str("codebreaker.decompile.jar")};
    p.loader.container = str("source folder, jar");
    p.convention.names = strs({"the JVM's"});
    p.convention.order = str("c");
    p.convention.pops = str("callee");
    p.emulator.engine = str("the JVM");
    p.compiler.language = str("c++");
    p.compiler.toolchain = str("the host C++ compiler (host_cxx, else g++)");
    p.compiler.tool = Ref(str("codebreaker.verify.lanes.java"), str("compiler"));
    p.packager.tools = Ref(str("codebreaker.verify.lanes.java"), str("jdk"));
    p.finish();
    return p;
}

// the original Xbox: x86 code in an XBE (it goes down the 32-bit Windows lane as the PE image formats/xbox/xbe makes of it)
static std::function<str(const str&)>& xbe_to_pe();
static Platform make_xbe() {
    Platform p;
    p.name = "xbe"; p.title = "original Xbox"; p.family = "x86"; p.source = str("c");
    p.recognise_fn = [](const str& path) {
        py::file f = py::open_rb(path);
        return f.read(4) == py::bytes("XBEH");
    };
    p.cpu.language = "x86:LE:32:default:windows";
    p.cpu.bits = 32;
    p.cpu.endian = str("little");
    p.cpu.decoder = Cpu::Capstone{str("x86"), 32};
    p.loader.container = str("XBE -> PE");
    p.loader.readable = Ref(str("codebreaker.platforms.xbox"), str("_open"));
    // _open: xbe.to_pe(path) if _recognise(path) else path (formats.xbox.xbe has no typed port: set_xbe_to_pe)
    p.readable_fn = [](const str& path) {
        py::file f = py::open_rb(path);
        if (!(f.read(4) == py::bytes("XBEH"))) return path;
        if (!xbe_to_pe()) throw py::RuntimeError("formats.xbox.xbe.to_pe has no typed port: call set_xbe_to_pe");
        return xbe_to_pe()(path);
    };
    p.finish();
    return p;
}

static py::list<str> r_regs(int from, int to) {
    py::list<str> out;
    for (int i = from; i < to; i++) out.append(str("r" + std::to_string(i)));
    return out;
}

// the Xbox 360: PowerPC code (Xenon) in a big-endian PE image inside a XEX2 container
static Platform make_xbox360() {
    const char* here = "codebreaker.platforms.xbox360";
    Platform p;
    p.name = "powerpc"; p.title = "Xbox 360 (PowerPC)"; p.family = "PowerPC"; p.source = str("c");
    p.machine = 0x01F2;
    p.lane = str("codebreaker.verify.lanes.powerpc");
    p.recognise_fn = [](const str& path) {
        return formats::xex::is_xex(path) || (lane_of(path) == std::optional<str>(str("pe")) && pe_machine(path) == 0x01F2);
    };
    p.readable_fn = [](const str& path) { return formats::xex::is_xex(path) ? formats::xex::basefile(path) : path; };
    p.loader.container = str("XEX2 -> PE");
    p.loader.readable = Ref(str(here), str("_open"));
    p.loader.open = Ref(str("codebreaker.formats.powerpc_image"), str("read"));
    p.cpu.language = "PowerPC:BE:64:Xenon:homed";
    p.cpu.bits = 64;
    p.cpu.endian = str("big");
    p.cpu.decoder = Cpu::Module{str("codebreaker.emulate.powerpc")};
    p.cpu.registers = r_regs(0, 32);
    p.cpu.stack = str("r1");
    p.convention.names = strs({"__cdecl"});
    p.convention.arguments = int64_t(8);
    p.convention.word = 8;
    p.convention.order = str("registers");
    p.convention.results = strs({"r3", "f1"});
    p.convention.pops = str("caller");
    p.convention.preserved = r_regs(14, 32);
    p.emulator.engine = str("CodeBreaker's own interpreter");
    p.emulator.run = Ref(str("codebreaker.emulate.powerpc"), str("recorded"));
    p.compiler.language = str("c");
    p.compiler.toolchain = str("the Xbox 360 SDK");
    p.compiler.tool = Ref(str(here), str("compiler"));
    p.compiler.tools = Ref(str(here), str("tools"));
    p.compiler.flags = Ref(str("codebreaker.build.functions"), str("CFLAGS_POWERPC"));
    p.packager.functions = Ref(str("codebreaker.build.functions"), str("build_powerpc"));
    p.packager.tools = Ref(str(here), str("tools"));
    p.finish();
    return p;
}

// 32-bit Windows: PE images for the 386 and later (machine type 0x14C)
static Platform make_win32() {
    const char* here = "codebreaker.platforms.win32";
    Platform p;
    p.name = "pe"; p.title = "32-bit Windows"; p.family = "x86"; p.source = str("c");
    p.machine = 0x014C;
    p.lane = str("codebreaker.verify.lanes.win32");
    p.recognise_fn = [](const str& path) {
        return lane_of(path) == std::optional<str>(str("pe")) && pe_machine(path) != 0x01F2;
    };
    p.loader.container = str("PE");
    p.loader.open = Ref(str("codebreaker.formats.pe"), str("open_pe"));
    p.loader.parts = Ref(str(here), str("sections"));
    p.loader.exports = Ref(str("codebreaker.build.definitions"), str("exports"));
    p.loader.imports = Ref(str("codebreaker.build.definitions"), str("imports"));
    p.cpu.language = "x86:LE:32:default:windows";
    p.cpu.bits = 32;
    p.cpu.endian = str("little");
    p.cpu.decoder = Cpu::Capstone{str("x86"), 32};
    p.cpu.registers = strs({"EAX", "EBX", "ECX", "EDX", "ESI", "EDI", "EBP", "ESP"});
    p.cpu.stack = str("ESP");
    p.convention.names = strs({"__stdcall", "__cdecl", "__fastcall", "__thiscall"});
    p.convention.arguments = int64_t(4);
    p.convention.word = 4;
    p.convention.order = str("c");
    p.convention.results = strs({"EAX", "EDX:EAX"});
    p.convention.pops = str("either");
    p.convention.preserved = strs({"EBX", "ESI", "EDI", "EBP"});
    p.emulator.engine = str("unicorn");
    p.emulator.run = Ref(str("codebreaker.emulate.emulator"), str("recorded"));
    p.emulator.call = Ref(str(here), str("call"));
    p.compiler.language = str("c");
    p.compiler.toolchain = str("openwatcom");
    p.compiler.tool = Ref(str("codebreaker.build.watcom"), str("ow_tool"), strs({"wcc386"}));
    p.compiler.tools = Ref(str(here), str("tools"));
    p.compiler.flags = Ref(str("codebreaker.decompile.c_text"), str("CFLAGS"));
    p.compiler.env = Ref(str("codebreaker.build.watcom"), str("ow_env"), strs({"pe"}));
    p.packager.link = Ref(str("codebreaker.build.module"), str("_build32"));
    p.packager.functions = Ref(str("codebreaker.build.functions"), str("build32"));
    p.packager.tools = Ref(str(here), str("tools"));
    p.packager.linker = Ref(str("codebreaker.build.watcom"), str("ow_tool"), strs({"wlink"}));
    p.packager.assembler = Ref(str("codebreaker.build.watcom"), str("ow_tool"), strs({"wasm"}));
    p.packager.resources = Ref(str("codebreaker.build.watcom"), str("ow_tool"), strs({"wrc"}));
    p.finish();
    return p;
}

static bool is_zx(const str& path) {
    if (!path.lower().endswith(std::make_tuple(str(".sna"), str(".z80"), str(".tap")))) return false;
    try {
        if (zx_reader()) return zx_reader()(path);
        py::file f = py::open_rb(path);          // (no formats.zx port: a readable file of some size)
        return py::len(f.read(16)) == 16;
    } catch (const py::OSError&) {
        return false;
    } catch (const py::ValueError&) {
        return false;
    } catch (const py::IndexError&) {
        return false;
    }
}

static Platform make_zx() {
    Cpu c;
    c.language = "z80:LE:16:default";
    c.bits = 8;
    c.endian = str("little");
    c.decoder = Cpu::Module{str("codebreaker.decompile.z80")};
    c.registers = strs({"A", "F", "BC", "DE", "HL", "IX", "IY", "SP"});
    c.stack = str("SP");
    return machine(str("zx"), str("ZX Spectrum"), str("z80"), str("snapshot or tape (.sna, .z80, .tap)"), c, is_zx,
                   {str("codebreaker.formats.zx"), str("read")}, {str("codebreaker.emulate.z80"), str("recorded")},
                   {str("codebreaker.build.z80_asm"), str("build_flat")},
                   {str("codebreaker.source.zx_writer"), str("reconstruct")},
                   strs({"A", "F", "BC", "DE", "HL", "IX", "IY"}),
                   str("CodeBreaker's own assembler, or sjasmplus / pasmo"));
}

// ---- the registry ----------------------------------------------------------------------------------
static py::list<Platform>* g_all = nullptr;

const py::list<Platform>& all() {
    if (!g_all) {
        auto* found = new py::list<Platform>();
        for (const str& m : ORDER) {
            Platform p;
            if (m == str("java")) p = make_java();
            else if (m == str("xbox360")) p = make_xbox360();
            else if (m == str("xbox")) p = make_xbe();
            else if (m == str("nes")) p = from_machine(nes::PLATFORM, [](const str& x) { return nes::_recognise(x); });
            else if (m == str("sms")) p = from_machine(sms::PLATFORM, [](const str& x) { return sms::_recognise(x); });
            else if (m == str("gb")) p = from_machine(gb::PLATFORM(), [](const str& x) { return gb::_recognise(x); });
            else if (m == str("genesis")) p = from_machine(genesis::PLATFORM(), [](const str& x) { return genesis::_recognise(x); });
            else if (m == str("zx")) p = make_zx();
            else if (m == str("msx")) p = from_machine(msx::PLATFORM, [](const str& x) { return msx::_recognise(x); });
            else if (m == str("win32")) p = make_win32();
            else if (m == str("win16")) p = from_win16();
            else if (m == str("vxd")) p = from_vxd();
            else p = from_dos();
            p.id = m;
            found->append(p);
        }
        g_all = found;
    }
    return *g_all;
}

const Platform* identify(const str& path) {
    for (const Platform& p : all())
        if (p.recognises(path)) return &p;
    return nullptr;
}

const Platform* named(const str& name) {
    for (const Platform& p : all())
        if (p.name == name) return &p;
    for (const Platform& p : all())
        if (p.id && *p.id == name) return &p;
    return nullptr;
}

const Platform& of(const str& kind) {
    const Platform* p = named(kind);
    if (!p) throw py::ValueError("no platform " + py::repr(kind).utf8());
    return *p;
}

const Platform* for_machine(int64_t machine_) {
    for (const Platform& p : all())
        if (p.machine && *p.machine == machine_) return &p;
    return nullptr;
}

py::dict<str, str> lanes() {
    py::dict<str, str> out;
    for (const Platform& p : all())
        if (p.lane && !p.lane->empty()) out.set(p.name, *p.lane);
    return out;
}

py::dict<str, std::tuple<str, str>> runners() {
    py::dict<str, std::tuple<str, str>> out;
    for (const Platform& p : all())
        if (auto r = p.runner()) out.set(p.name, *r);
    return out;
}

py::dict<str, str> step_modules() {
    py::dict<str, str> out;
    for (const Platform& p : all())
        if (p.steps && !p.steps->empty()) out.set(*p.id, *p.steps);
    return out;
}

}  // namespace codebreaker::platforms
