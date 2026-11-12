// cb/java/probe.cpp -- see probe.hpp.
#include "cb/java/probe.hpp"

#include <process.h>

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <format>
#include <functional>
#include <span>

#include "cb/core/fs.hpp"
#include "cb/core/parallel.hpp"
#include "cb/core/process.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/java/parse.hpp"
#include "cb/java/twophase.hpp"

namespace cb::java::probe {
namespace {

namespace stdfs = std::filesystem;

constexpr std::size_t kMaxCases = 48;
constexpr char kClasspathSeparator = ';';

// ---- the input table ---------------------------------------------------------------------------------------------------

// A Java primitive (or String): its C++ type, and the inputs it is tried with, written as Java literals.
struct Primitive {
    std::string_view name;
    std::string_view cxx_type;
    std::vector<std::string_view> inputs;
};

const std::vector<Primitive>& primitives() {
    static const std::vector<Primitive> table = {
        {"int", "int", {"0", "1", "-1", "2", "7", "-13", "100", "255", "256", "1000", "-1000", "65535", "123456", "2147483647", "-2147483648"}},
        {"long", "long long", {"0L", "1L", "-1L", "1000000007L", "-123456789012L", "9223372036854775807L"}},
        {"short", "short", {"0", "1", "-1", "255", "-32768", "32767"}},
        {"byte", "signed char", {"0", "1", "-1", "100", "-128", "127"}},
        {"char", "unsigned char", {"0", "32", "65", "97", "122", "167", "255"}},
        {"boolean", "bool", {"true", "false"}},
        {"float", "float", {"0.0F", "1.0F", "-1.0F", "0.5F", "-0.5F", "3.14159F", "100.25F", "-1000.75F", "0.001F", "1000000.0F", "-0.0F"}},
        {"double", "double", {"0.0", "1.0", "-1.0", "0.5", "-0.5", "3.14159", "100.25", "-1000.75", "0.001", "1000000.0", "-0.0", "12345.678"}},
        {"String", "JavaString", {"", "a", "Hello World", "123", "-45", "minecraft", " pad ", "A.B.C", "x=1,y=2", "\xC2\xA7" "cRed"}},
    };
    return table;
}

const Primitive* find_primitive(std::string_view name) {
    for (const Primitive& p : primitives()) {
        if (p.name == name) return &p;
    }
    return nullptr;
}

// A primitive or String type (not an array).
bool is_simple(const ir::JType& t) { return !t.is_array() && find_primitive(t.name) != nullptr; }

// ---- small text helpers ----------------------------------------------------------------------------------------------

bool is_word(char c) { return std::isalnum(static_cast<unsigned char>(c)) != 0 || c == '_'; }

std::size_t skip_space(std::string_view s, std::size_t at) {
    while (at < s.size() && text::is_space(s[at])) ++at;
    return at;
}

// `\bNAME\s*;` -- the header declares a member with that name.
bool declares_member(std::string_view header, std::string_view name) {
    for (std::size_t at = header.find(name); at != std::string_view::npos; at = header.find(name, at + 1)) {
        if (at > 0 && is_word(header[at - 1])) continue;
        const std::size_t after = skip_space(header, at + name.size());
        if (after < header.size() && header[after] == ';') return true;
    }
    return false;
}

// `\bNAME\s*\(` -- the text calls or declares a method with that name.
bool has_call_to(std::string_view body, std::string_view name) {
    for (std::size_t at = body.find(name); at != std::string_view::npos; at = body.find(name, at + 1)) {
        if (at > 0 && is_word(body[at - 1])) continue;
        const std::size_t after = skip_space(body, at + name.size());
        if (after < body.size() && body[after] == '(') return true;
    }
    return false;
}

// `\bstatic\s+[\w<>* ]+\bNAME\s*\(` -- the header declares a static method with that name: the keyword, then only a type (word
// characters, `<>*`, spaces and tabs) up to the name.
bool declares_static_method(std::string_view header, std::string_view name) {
    for (std::size_t at = header.find(name); at != std::string_view::npos; at = header.find(name, at + 1)) {
        if (at > 0 && is_word(header[at - 1])) continue;
        const std::size_t after = skip_space(header, at + name.size());
        if (after >= header.size() || header[after] != '(') continue;
        // walk back over the declaration's type: word characters, `<>*`, spaces and tabs
        std::size_t start = at;
        while (start > 0) {
            const char c = header[start - 1];
            if (!(is_word(c) || c == '<' || c == '>' || c == '*' || c == ' ' || c == '\t')) break;
            --start;
        }
        const std::string_view run = header.substr(start, at - start);
        for (std::size_t s = run.find("static"); s != std::string_view::npos; s = run.find("static", s + 1)) {
            const bool word_start = s == 0 ? (start == 0 || !is_word(header[start - 1])) : !is_word(run[s - 1]);
            const std::size_t end = s + 6;
            if (word_start && end < run.size() && (run[end] == ' ' || run[end] == '\t') && run.size() > end + 1) return true;
        }
    }
    return false;
}

// `(?<![\w.])(?:this\s*\.\s*)?NAME\s*\(` -- a call of one of the class's own methods.
bool calls_own_method(std::string_view body, std::string_view name) {
    for (std::size_t at = body.find(name); at != std::string_view::npos; at = body.find(name, at + 1)) {
        const std::size_t after = skip_space(body, at + name.size());
        if (after >= body.size() || body[after] != '(') continue;
        if (at == 0 || (!is_word(body[at - 1]) && body[at - 1] != '.')) return true;   // NAME(
        // this . NAME(   (the `this` itself not part of a longer word or a member access)
        std::size_t k = at;
        while (k > 0 && text::is_space(body[k - 1])) --k;
        if (k == 0 || body[k - 1] != '.') continue;
        --k;
        while (k > 0 && text::is_space(body[k - 1])) --k;
        if (k >= 4 && body.substr(k - 4, 4) == "this" && (k == 4 || (!is_word(body[k - 5]) && body[k - 5] != '.'))) return true;
    }
    return false;
}

std::string method_text(const ir::Method& m) {
    std::string out;
    for (std::size_t i = 0; i < m.body.size(); ++i) {
        if (i) out += '\n';
        out += m.body[i].text;
    }
    return out;
}

// The text of a header file, or nullopt if the class has none.
std::optional<std::string> read_header(const stdfs::path& cpp_dir, const std::string& cls) {
    const stdfs::path path = cpp_dir / (cls + ".h");
    std::error_code ec;
    if (!stdfs::is_regular_file(path, ec)) return std::nullopt;
    return text::sanitize_utf8(fs::read_text(path));
}

std::string harness_class_name(std::string_view cls) { return "J2XProbe_" + std::string(cls); }

// ---- running tools ----------------------------------------------------------------------------------------------------

struct ToolRun {
    bool timed_out = false;
    std::uint32_t exit_code = 0;
    std::string out;   // stdout (and stderr when merged)
    std::string err;
    bool ok() const { return !timed_out && exit_code == 0; }
};

enum class Streams { Stdout, Merged, StderrOnly };

ToolRun run_tool(std::vector<std::string> argv, std::chrono::seconds timeout, Streams streams) {
    process::ProcessSpec spec;
    spec.argv = std::move(argv);
    spec.timeout = std::chrono::duration_cast<std::chrono::milliseconds>(timeout);
    switch (streams) {
        case Streams::Stdout: spec.err = process::Capture::Discard; break;
        case Streams::Merged: spec.err = process::Capture::Merge; break;
        case Streams::StderrOnly: spec.out = process::Capture::Discard; break;
    }
    const process::ProcessResult r = process::run(spec);
    return {r.timed_out, r.exit_code, std::string(as_text(r.out)), std::string(as_text(r.err))};
}

std::string path_arg(const stdfs::path& p) { return fs::to_text(p); }

// The classpath of a run: these folders, the stand-ins (ahead of the library they stand in for), the libraries.
std::string classpath(const stdfs::path& work, const Toolchain& tools, const std::vector<stdfs::path>& dirs) {
    std::string out;
    auto add = [&](const stdfs::path& p) {
        if (!out.empty()) out += kClasspathSeparator;
        out += path_arg(p);
    };
    for (const stdfs::path& d : dirs) add(d);
    const stdfs::path shim = work / "probe" / "_shims";
    std::error_code ec;
    if (stdfs::is_regular_file(shim / ".compiled", ec)) add(shim);
    for (const stdfs::path& lib : tools.libraries) add(lib);
    return out;
}

// Compile the stand-ins into work/probe/_shims, once. A folder with no libraries to compile them against, or stand-ins that do
// not compile, leaves no marker and the classpath without them.
void ensure_shims(const stdfs::path& work, const Toolchain& tools) {
    static std::mutex lock;
    if (tools.libraries.empty() || tools.shim_sources.empty()) return;
    const stdfs::path out = work / "probe" / "_shims";
    std::lock_guard guard(lock);
    std::error_code ec;
    if (stdfs::is_regular_file(out / ".compiled", ec)) return;
    std::vector<std::string> files;
    if (stdfs::is_directory(tools.shim_sources, ec)) {
        for (const auto& entry : stdfs::recursive_directory_iterator(tools.shim_sources, ec)) {
            if (entry.is_regular_file() && entry.path().extension() == ".java") files.push_back(path_arg(entry.path()));
        }
    }
    if (files.empty()) return;
    std::sort(files.begin(), files.end());
    fs::make_dirs(out);
    std::string cp;
    for (const stdfs::path& lib : tools.libraries) cp += (cp.empty() ? "" : std::string(1, kClasspathSeparator)) + path_arg(lib);
    std::vector<std::string> argv = {path_arg(tools.javac), "-nowarn", "-encoding", "UTF-8", "-cp", cp, "-d", path_arg(out)};
    argv.insert(argv.end(), files.begin(), files.end());
    if (run_tool(std::move(argv), tools.run_timeout * 3, Streams::Stdout).ok()) fs::write_text(out / ".compiled", "");
}

// ---- compiling the translation ----------------------------------------------------------------------------------------

// A finished temporary file takes the name it is for (an existing file of that name is replaced).
void move_into_place(const stdfs::path& from, const stdfs::path& to) {
    std::error_code ec;
    stdfs::rename(from, to, ec);
    if (ec) throw IoError(to, ec.value(), "cannot move " + fs::to_text(from) + " into place");
}

struct UnitResult {
    bool ok = false;
    bool timed_out = false;
    std::string error;   // the compiler's last line
};

// unit -> obj (written under a temporary name and moved into place); an existing obj is kept.
UnitResult compile_unit(const Toolchain& tools, const stdfs::path& cpp_dir, const stdfs::path& unit, const stdfs::path& obj) {
    std::error_code ec;
    if (stdfs::is_regular_file(obj, ec)) return {true, false, ""};
    const stdfs::path tmp = obj.string() + "." + std::to_string(_getpid()) + ".tmp";
    std::vector<std::string> argv = {path_arg(tools.cxx), "-std=" + tools.cxx_std, "-w", "-O0", "-iquote", path_arg(cpp_dir)};
    if (tools.prefix) {
        argv.push_back("-include");
        argv.push_back(path_arg(*tools.prefix));
    }
    argv.push_back("-c");
    argv.push_back(path_arg(unit));
    argv.push_back("-o");
    argv.push_back(path_arg(tmp));
    const ToolRun r = run_tool(std::move(argv), tools.step_timeout, Streams::StderrOnly);
    if (!r.ok()) {
        stdfs::remove(tmp, ec);
        if (r.timed_out) return {false, true, ""};
        const std::string_view err = text::trim(r.err);
        const auto lines = text::lines(err);
        return {false, false, lines.empty() ? "compile failed" : std::string(lines.back())};
    }
    move_into_place(tmp, obj);
    return {true, false, ""};
}

}  // namespace

// ---- the sources -----------------------------------------------------------------------------------------------------------

std::vector<stdfs::path> list_libraries(const stdfs::path& folder) {
    std::vector<stdfs::path> out;
    std::error_code ec;
    if (!stdfs::is_directory(folder, ec)) return out;
    for (const auto& entry : stdfs::directory_iterator(folder, ec)) {
        if (entry.is_regular_file() && text::lower(fs::to_text(entry.path().extension())) == ".jar") out.push_back(entry.path());
    }
    std::sort(out.begin(), out.end(), [](const stdfs::path& a, const stdfs::path& b) { return fs::to_text(a) < fs::to_text(b); });
    return out;
}

namespace {

// Every .java file of a tree, one per file name (the first in path order), in name order.
std::vector<stdfs::path> java_files(const stdfs::path& dir) {
    std::vector<stdfs::path> all;
    std::error_code ec;
    for (const auto& entry : stdfs::recursive_directory_iterator(dir, ec)) {
        if (entry.is_regular_file() && entry.path().extension() == ".java") all.push_back(entry.path());
    }
    std::sort(all.begin(), all.end(), [](const stdfs::path& a, const stdfs::path& b) { return fs::to_text(a) < fs::to_text(b); });
    std::map<std::string, stdfs::path> by_name;
    for (const stdfs::path& p : all) by_name.emplace(fs::to_text(p.filename()), p);
    std::vector<stdfs::path> out;
    for (auto& [name, path] : by_name) out.push_back(path);
    return out;
}

}  // namespace

stdfs::path stage_sources(const stdfs::path& java_dir, const stdfs::path& dest) {
    static const Regex kPackage(R"(^\s*package\s+([\w.]+)\s*;)", RegexFlags::Multiline);
    for (const stdfs::path& path : java_files(java_dir)) {
        const std::string source = text::sanitize_utf8(fs::read_text(path));
        stdfs::path folder = dest;
        if (const auto m = kPackage.search(source)) {
            for (const std::string_view part : text::split(m->group(1), '.')) folder /= fs::from_text(part);
        }
        fs::write_text(folder / path.filename(), source);
    }
    return dest;
}

SourceTree::SourceTree(stdfs::path dir) : dir_(std::move(dir)) {
    std::vector<stdfs::path> all;
    std::error_code ec;
    for (const auto& entry : stdfs::recursive_directory_iterator(dir_, ec)) {
        if (entry.is_regular_file() && entry.path().extension() == ".java") all.push_back(entry.path());
    }
    std::sort(all.begin(), all.end(), [](const stdfs::path& a, const stdfs::path& b) { return fs::to_text(a) < fs::to_text(b); });
    for (const stdfs::path& p : all) index_.emplace(fs::to_text(p.stem()), p);   // (the first path of a name stays)
}

const ir::JClass* SourceTree::lookup(std::string_view name) const {
    const std::string key(name);
    std::lock_guard lock(mutex_);
    if (const auto it = parsed_.find(key); it != parsed_.end()) return it->second.get();
    std::unique_ptr<ir::JClass> parsed;
    if (const auto found = index_.find(name); found != index_.end()) {
        try {
            parsed = std::make_unique<ir::JClass>(parse::parse_file(found->second));
        } catch (const Error&) {   // a class the parser cannot read is a class with nothing to run
            parsed.reset();
        }
    }
    return parsed_.emplace(key, std::move(parsed)).first->second.get();
}

// ---- what can be run ------------------------------------------------------------------------------------------------------

std::optional<Maker> maker(const ir::JClass* cls) {
    if (cls == nullptr || cls->is_abstract || cls->is_interface || cls->is_enum) return std::nullopt;
    std::vector<const ir::Method*> ctors;
    for (const ir::Method& m : cls->methods) {
        if (m.is_ctor) ctors.push_back(&m);
    }
    auto all_simple = [](const ir::Method& m) {
        return std::all_of(m.params.begin(), m.params.end(), [](const ir::Param& p) { return is_simple(p.type); });
    };
    auto names_of = [](const ir::Method& m) {
        std::vector<std::string> out;
        for (const ir::Param& p : m.params) out.push_back(p.type.name);
        return out;
    };
    if (ctors.empty()) return Maker{std::nullopt, {}};   // Java's implicit default constructor
    for (const ir::Method* m : ctors) {
        if (m->access != ir::Access::Private && all_simple(*m)) return Maker{std::nullopt, names_of(*m)};
    }
    for (const ir::Method& m : cls->methods) {
        if (m.is_static && !m.is_ctor && m.access != ir::Access::Private && m.return_type && !m.return_type->is_array() &&
            m.return_type->name == cls->name && all_simple(m)) {
            return Maker{m.name, names_of(m)};
        }
    }
    return std::nullopt;
}

FieldList comparable_fields(const SourceTree& tree, const ir::JClass& cls, const stdfs::path& cpp_dir) {
    FieldList out;
    std::set<std::string> seen;
    for (const ir::JClass* c = &cls; c != nullptr && seen.insert(c->name).second;) {
        const std::string header = read_header(cpp_dir, c->name).value_or("");
        for (const ir::Field& f : c->fields) {
            if (f.is_static || f.access == ir::Access::Private || !is_simple(f.type) || !declares_member(header, f.name)) continue;
            if (std::none_of(out.begin(), out.end(), [&](const auto& have) { return have.first == f.name; })) out.emplace_back(f.name, f.type.name);
        }
        c = c->parent ? tree.lookup(*c->parent) : nullptr;
    }
    return out;
}

namespace {

// A parameter's input slot; nullopt when it cannot be passed.
std::optional<Slot> slot_for(const SourceTree& tree, const ir::JType& t) {
    if (is_simple(t)) return Slot(t.name);
    if (t.is_array()) return std::nullopt;
    if (auto made = maker(tree.lookup(t.name))) return Slot(ObjectSlot{t.name, std::move(*made)});
    return std::nullopt;
}

// How a result is compared; nullopt when it cannot be.
std::optional<Outcome> outcome_for(const SourceTree& tree, const stdfs::path& cpp_dir, const std::optional<ir::JType>& t) {
    if (!t || t->is_array()) return std::nullopt;
    if (find_primitive(t->name)) return Outcome{Outcome::Kind::Primitive, t->name, "", {}};
    const ir::JClass* cls = tree.lookup(t->name);
    FieldList fields = cls ? comparable_fields(tree, *cls, cpp_dir) : FieldList{};
    if (fields.empty()) return std::nullopt;
    return Outcome{Outcome::Kind::Object, "", t->name, std::move(fields)};
}

}  // namespace

ClockReads reads_clock(const ir::JClass& cls) {
    static const Regex kClock(R"(\b(?:System\s*\.\s*(?:currentTimeMillis|nanoTime)|new\s+Date|[\w.]*getSystemClipboard)\s*\()");
    std::map<std::string, std::string> bodies;   // (an overloaded name keeps its last body)
    for (const ir::Method& m : cls.methods) {
        if (!m.is_ctor) bodies[m.name] = method_text(m);
    }
    std::set<std::string> clocked;
    for (const auto& [name, body] : bodies) {
        if (kClock.test(body)) clocked.insert(name);
    }
    // a method that calls one that reads the clock reads it too
    for (bool grew = true; grew;) {
        grew = false;
        for (const auto& [name, body] : bodies) {
            if (clocked.contains(name)) continue;
            if (std::any_of(clocked.begin(), clocked.end(), [&](const std::string& c) { return calls_own_method(body, c); })) {
                clocked.insert(name);
                grew = true;
            }
        }
    }
    std::string ctor;
    bool first = true;
    for (const ir::Method& m : cls.methods) {
        if (!m.is_ctor) continue;
        if (!first) ctor += '\n';
        first = false;
        ctor += method_text(m);
    }
    std::string initialisers;
    for (const ir::Field* f : cls.instance_fields()) {
        if (!f->initializer || f->initializer->empty()) continue;
        if (!initialisers.empty()) initialisers += '\n';
        initialisers += *f->initializer;
    }
    ctor += initialisers;
    ClockReads out;
    out.methods.assign(clocked.begin(), clocked.end());
    out.constructors = kClock.test(ctor) || std::any_of(clocked.begin(), clocked.end(), [&](const std::string& c) { return calls_own_method(ctor, c); });
    return out;
}

std::vector<Probeable> probeable(const SourceTree& tree, const ir::JClass& cls, std::string_view header_text, const stdfs::path& cpp_dir) {
    std::vector<Probeable> out;
    std::set<std::pair<std::string, std::vector<std::string>>> seen;
    const std::optional<Maker> made = maker(&cls);
    const ClockReads clock = reads_clock(cls);
    FieldList own_fields = comparable_fields(tree, cls, cpp_dir);
    for (const ir::Method& m : cls.methods) {
        if (m.is_ctor || m.is_abstract || m.access == ir::Access::Private || (!m.is_static && !made)) continue;
        // a method that depends on the time cannot be compared
        if (std::find(clock.methods.begin(), clock.methods.end(), m.name) != clock.methods.end() || (clock.constructors && !m.is_static)) continue;
        std::optional<Outcome> outcome;
        if (m.return_type && m.return_type->name == "void" && !m.return_type->is_array()) {
            // a void instance method is compared by the object afterwards
            if (!m.is_static && !own_fields.empty()) outcome = Outcome{Outcome::Kind::Self, "", cls.name, own_fields};
        } else {
            outcome = outcome_for(tree, cpp_dir, m.return_type);
        }
        if (!outcome) continue;
        std::vector<Slot> slots;
        bool passable = true;
        for (const ir::Param& p : m.params) {
            if (auto s = slot_for(tree, p.type)) slots.push_back(std::move(*s));
            else passable = false;
        }
        if (!passable) continue;
        std::vector<std::string> param_types;
        for (const ir::Param& p : m.params) param_types.push_back(p.type.to_string());
        if (!seen.insert({m.name, param_types}).second) continue;
        if (!(m.is_static ? declares_static_method(header_text, m.name) : has_call_to(header_text, m.name))) continue;
        out.push_back({m.name, std::move(slots), std::move(*outcome), m.is_static ? std::nullopt : made});
    }
    return out;
}

std::size_t counted(const ir::JClass& cls) {
    return static_cast<std::size_t>(std::count_if(cls.methods.begin(), cls.methods.end(), [](const ir::Method& m) { return !m.is_abstract; }));
}

// ---- inputs -----------------------------------------------------------------------------------------------------------------

std::vector<std::vector<std::string>> cases(const std::vector<std::string>& types) {
    std::vector<const Primitive*> pools;
    for (const std::string& t : types) {
        const Primitive* p = find_primitive(t);
        if (p == nullptr) throw UsageError("probe: '" + t + "' is not a primitive or String type");
        pools.push_back(p);
    }
    if (pools.empty()) return std::vector<std::vector<std::string>>(1);
    std::size_t total = 1;
    bool few = true;   // whether every combination fits in kMaxCases (the product of the pools is never built beyond that)
    for (const Primitive* p : pools) {
        if (total > kMaxCases / p->inputs.size()) few = false;
        if (!few) break;
        total *= p->inputs.size();
    }
    std::vector<std::vector<std::string>> out;
    if (few && total <= kMaxCases) {
        // every combination, the last parameter varying fastest
        std::vector<std::size_t> at(pools.size(), 0);
        for (std::size_t n = 0; n < total; ++n) {
            std::vector<std::string> row;
            for (std::size_t k = 0; k < pools.size(); ++k) row.emplace_back(pools[k]->inputs[at[k]]);
            out.push_back(std::move(row));
            for (std::size_t k = pools.size(); k-- > 0;) {
                if (++at[k] < pools[k]->inputs.size()) break;
                at[k] = 0;
            }
        }
        return out;
    }
    // too many to try them all: a fixed spread -- the k-th parameter steps through its inputs at its own stride -- then every input
    // of every parameter at least once
    for (std::size_t i = 0; i < kMaxCases; ++i) {
        std::vector<std::string> row;
        for (std::size_t k = 0; k < pools.size(); ++k) row.emplace_back(pools[k]->inputs[(i * (2 * k + 1) + k) % pools[k]->inputs.size()]);
        if (std::find(out.begin(), out.end(), row) == out.end()) out.push_back(std::move(row));
    }
    for (std::size_t k = 0; k < pools.size(); ++k) {
        for (const std::string_view v : pools[k]->inputs) {
            if (std::any_of(out.begin(), out.end(), [&](const auto& c) { return c[k] == v; })) continue;
            std::vector<std::string> row;
            for (std::size_t j = 0; j < pools.size(); ++j) row.emplace_back(j == k ? v : pools[j]->inputs[0]);
            out.push_back(std::move(row));
        }
    }
    return out;
}

std::string java_literal(std::string_view type, std::string_view value) {
    if (type == "String") {
        // the Java string's UTF-16 units: printable ASCII as it is, anything else as a \u escape
        std::string out = "\"";
        const Bytes utf16 = text::to_utf16le(value);
        for (std::size_t i = 0; i + 1 < utf16.size(); i += 2) {
            const unsigned unit = utf16[i] | (utf16[i + 1] << 8);
            if (unit >= 32 && unit < 127 && unit != '"' && unit != '\\') out += static_cast<char>(unit);
            else out += std::format("\\u{:04x}", unit);
        }
        return out + "\"";
    }
    if (type == "byte" || type == "short" || type == "char") return std::format("({}){}", type, value);
    return std::string(value);
}

std::string cpp_literal(std::string_view type, std::string_view value) {
    if (type == "String") {
        // the port's String is a value of bytes: one byte per Java char below 256, each non-printable one as its own \x escape
        std::string out = "JavaString(\"";
        for (const std::uint8_t b : text::to_latin1(value)) {
            if (b >= 32 && b < 127 && b != '"' && b != '\\') out += static_cast<char>(b);
            else out += std::format("\\x{:02x}\" \"", b);
        }
        return out + "\")";
    }
    if (type == "long") return value.starts_with("-9223") ? "(-9223372036854775807LL - 1)" : std::string(value.substr(0, value.size() - 1)) + "LL";
    if ((type == "float" || type == "double") && value.starts_with('-')) return std::format("({})", value);
    if (type == "int" && value == "-2147483648") return "(-2147483647 - 1)";
    if (type == "byte" || type == "short" || type == "char") return std::format("({}){}", find_primitive(type)->cxx_type, value);
    return std::string(value);
}

// ---- comparing --------------------------------------------------------------------------------------------------------------

std::map<std::string, std::string> parse_results(std::string_view printed) {
    std::map<std::string, std::string> out;
    for (const std::string_view line : text::lines(printed)) {
        const std::string_view trimmed = text::trim(line);
        const std::size_t bar = trimmed.find('|');
        const std::string_view key = trimmed.substr(0, bar);
        if (!key.empty()) out[std::string(key)] = bar == std::string_view::npos ? "" : std::string(trimmed.substr(bar + 1));
    }
    return out;
}

bool same(std::string_view type, std::string_view a, std::string_view b) {
    if (a == b) return true;
    if (type != "float" && type != "double") return false;
    auto is_nan = [&](std::string_view bits) {
        const auto v = text::parse_int<std::uint64_t>(bits, 16);
        if (!v) return false;
        if (type == "float") return *v <= 0xffffffffull && (*v & 0x7f800000u) == 0x7f800000u && (*v & 0x007fffffu) != 0;
        return (*v & 0x7ff0000000000000ull) == 0x7ff0000000000000ull && (*v & 0x000fffffffffffffull) != 0;
    };
    return is_nan(a) && is_nan(b);   // (equal bits were settled above)
}

// ---- harnesses --------------------------------------------------------------------------------------------------------------

namespace {

// Printing one value: the helper for each type (the same names on both sides).
std::string_view formatter(std::string_view type) {
    if (type == "char") return "fc";
    if (type == "boolean") return "fb";
    if (type == "float") return "ff";
    if (type == "double") return "fd";
    if (type == "String") return "fs";
    return "fi";   // int, long, short, byte
}

constexpr std::string_view kJavaHelpers = R"(  static void out(String k, String v) { System.out.println(k + "|" + v); }
  static String fi(long v) { return Long.toString(v); }
  static String fc(char v) { return Integer.toString((int)v); }
  static String fb(boolean v) { return v ? "1" : "0"; }
  static String ff(float v) { return Integer.toHexString(Float.floatToRawIntBits(v)); }
  static String fd(double v) { return Long.toHexString(Double.doubleToRawLongBits(v)); }
  static String fs(String v) {
    if (v == null) return "";
    StringBuilder b = new StringBuilder();
    for (int i = 0; i < v.length(); ++i) { int c = v.charAt(i); b.append(c < 256 ? String.format("%02x", c) : "u" + Integer.toHexString(c)); }
    return b.toString();
  })";

constexpr std::string_view kCppHelpers = R"(#include <cstdio>
#include <cstring>
#include <string>
static void out(const char* k, const std::string& v) { std::printf("%s|%s\n", k, v.c_str()); }
static std::string fi(long long v) { char b[32]; std::sprintf(b, "%lld", v); return b; }
static std::string fc(unsigned char v) { char b[8]; std::sprintf(b, "%d", (int)v); return b; }
static std::string fb(bool v) { return v ? "1" : "0"; }
static std::string ff(float v) { unsigned int u; std::memcpy(&u, &v, 4); char b[16]; std::sprintf(b, "%x", u); return b; }
static std::string fd(double v) { unsigned long long u; std::memcpy(&u, &v, 8); char b[24]; std::sprintf(b, "%llx", u); return b; }
static std::string fs(const JavaString& v) {
	std::string r; char b[4];
	for (std::string::size_type i = 0; i < v.size(); ++i) { std::sprintf(b, "%02x", (unsigned char)v[i]); r += b; }
	return r;
})";

enum class Lang { Java, Cpp };

// A dump_<Class> function per class whose objects are compared by fields.
std::vector<std::string> dumpers(const std::vector<Probeable>& methods, Lang lang) {
    std::set<std::string> done;
    std::vector<std::string> out;
    for (const Probeable& m : methods) {
        if (m.outcome.kind == Outcome::Kind::Primitive || !done.insert(m.outcome.cls).second) continue;
        const std::string& name = m.outcome.cls;
        std::string joined;
        for (std::size_t i = 0; i < m.outcome.fields.size(); ++i) {
            const auto& [field, type] = m.outcome.fields[i];
            joined += std::format("{} + \"{}:\" + {}(o{}{})", i ? " + \",\"" : "", field, formatter(type), lang == Lang::Java ? "." : "->", field);
        }
        if (lang == Lang::Java) {
            out.push_back(std::format("  static String dump_{}({} o) {{ if (o == null) return \"null\"; return \"\"{}; }}", name, name, joined));
        } else {
            out.push_back(std::format("static std::string dump_{}({}* o) {{ if (o == NULL) return \"null\"; return std::string(){}; }}", name, name, joined));
        }
    }
    return out;
}

std::vector<std::string> flat_types(const std::vector<Slot>& slots, const std::optional<Maker>& made) {
    std::vector<std::string> out;
    if (made) out = made->params;
    for (const Slot& s : slots) {
        if (const auto* obj = std::get_if<ObjectSlot>(&s)) out.insert(out.end(), obj->made.params.begin(), obj->made.params.end());
        else out.push_back(std::get<std::string>(s));
    }
    return out;
}

struct CallArgs {
    std::optional<std::string> object;   // how the object is made (instance methods)
    std::vector<std::string> args;
};

CallArgs call_args(const std::vector<Slot>& slots, const std::optional<Maker>& made, const std::vector<std::string>& values, Lang lang,
                   std::string_view cls_name) {
    std::size_t next = 0;
    auto take = [&](const std::vector<std::string>& types) {
        std::string out;
        for (const std::string& t : types) {
            if (!out.empty()) out += ", ";
            out += lang == Lang::Java ? java_literal(t, values.at(next)) : cpp_literal(t, values.at(next));
            ++next;
        }
        return out;
    };
    auto make = [&](std::string_view name, const Maker& how) {
        const std::string a = take(how.params);
        if (!how.factory) return std::format("new {}({})", name, a);
        return std::format("{}{}{}({})", name, lang == Lang::Java ? "." : "::", *how.factory, a);
    };
    CallArgs out;
    if (made) out.object = make(cls_name, *made);
    for (const Slot& s : slots) {
        if (const auto* obj = std::get_if<ObjectSlot>(&s)) out.args.push_back(make(obj->cls, obj->made));
        else out.args.push_back(take({std::get<std::string>(s)}));
    }
    return out;
}

// The statement printing one call's outcome under the key.
std::string statement(const Probeable& m, const CallArgs& call, Lang lang, std::string_view cls_name, const std::string& key) {
    const std::string a = text::join(call.args, ", ");
    const char* sep = lang == Lang::Java ? "." : "->";
    const std::string target = !m.made ? std::format("{}{}{}({})", cls_name, lang == Lang::Java ? "." : "::", m.name, a)
                                       : std::format("({}){}{}({})", *call.object, sep, m.name, a);
    switch (m.outcome.kind) {
        case Outcome::Kind::Self: {
            const std::string var = lang == Lang::Java ? std::format("{} o = {};", cls_name, *call.object) : std::format("{}* o = {};", cls_name, *call.object);
            return std::format("{} o{}{}({}); out(\"{}\", dump_{}(o));", var, sep, m.name, a, key, cls_name);
        }
        case Outcome::Kind::Object: return std::format("out(\"{}\", dump_{}({}));", key, m.outcome.cls, target);
        case Outcome::Kind::Primitive: return std::format("out(\"{}\", {}({}));", key, formatter(m.outcome.type), target);
    }
    return {};
}

std::string case_key(std::size_t method, std::size_t index) { return std::format("{}.{}", method, index); }

std::vector<std::string> lines_of(std::string_view block) {
    const auto parts = text::split(block, '\n');
    return std::vector<std::string>(parts.begin(), parts.end());
}

}  // namespace

std::string java_harness(std::string_view cls_name, const std::vector<Probeable>& methods, const std::optional<std::string>& package,
                         std::string_view harness_name, const std::set<std::string>* only) {
    std::vector<std::string> lines;
    if (package) lines.push_back("package " + *package + ";");
    lines.push_back("public class " + std::string(harness_name) + " {");
    for (const std::string& l : lines_of(kJavaHelpers)) lines.push_back(l);
    for (std::string& l : dumpers(methods, Lang::Java)) lines.push_back(std::move(l));
    lines.push_back("  public static void main(String[] a) {");
    for (std::size_t mi = 0; mi < methods.size(); ++mi) {
        const auto all_cases = cases(flat_types(methods[mi].slots, methods[mi].made));
        for (std::size_t ci = 0; ci < all_cases.size(); ++ci) {
            const std::string key = case_key(mi, ci);
            if (only != nullptr && !only->contains(key)) continue;
            const CallArgs call = call_args(methods[mi].slots, methods[mi].made, all_cases[ci], Lang::Java, cls_name);
            lines.push_back(std::format("    try {{ {} }} catch (Throwable t) {{ out(\"{}\", \"throw\"); }}", statement(methods[mi], call, Lang::Java, cls_name, key), key));
        }
    }
    lines.push_back("  }");
    lines.push_back("}");
    lines.push_back("");
    return text::join(lines, "\n");
}

std::string cpp_harness(std::string_view cls_name, const std::vector<Probeable>& methods, const std::set<std::string>& keep,
                        const std::vector<std::string>& init_order, const std::set<std::string>& extra_headers,
                        const std::vector<std::pair<std::string, std::string>>& ancestry) {
    // the headers: the class, the classes with static initialisers, those the harness names, the bases -- each once, in that order
    std::vector<std::string> heads = {std::string(cls_name)};
    for (const std::string& c : init_order) heads.push_back(c);
    for (const std::string& c : extra_headers) heads.push_back(c);
    for (const auto& [derived, base] : ancestry) {
        heads.push_back(derived);
        heads.push_back(base);
    }
    std::set<std::string> included;
    std::vector<std::string> lines = {"#include \"stdafx.h\"", "#include \"JavaRT.h\""};
    for (const std::string& h : heads) {
        if (included.insert(h).second) lines.push_back("#include \"" + h + ".h\"");
    }
    for (const std::string& l : lines_of(kCppHelpers)) lines.push_back(l);
    for (std::string& l : dumpers(methods, Lang::Cpp)) lines.push_back(std::move(l));
    lines.push_back("int main() {");
    // The game registers each class's bases at boot (j2x_rtti.cpp) so that instanceof sees subclasses; without that a harness
    // that links a class whose constructor tests `x instanceof Base` takes the wrong branch.
    for (const auto& [derived, base] : ancestry) lines.push_back(std::format("\tj2xRegisterBase(typeid({}).name(), typeid({}).name());", derived, base));
    for (const std::string& c : init_order) lines.push_back(std::format("\t{}::initStatics();", c));
    for (std::size_t mi = 0; mi < methods.size(); ++mi) {
        const auto all_cases = cases(flat_types(methods[mi].slots, methods[mi].made));
        for (std::size_t ci = 0; ci < all_cases.size(); ++ci) {
            const std::string key = case_key(mi, ci);
            if (!keep.contains(key)) continue;
            const CallArgs call = call_args(methods[mi].slots, methods[mi].made, all_cases[ci], Lang::Cpp, cls_name);
            lines.push_back("\t{ " + statement(methods[mi], call, Lang::Cpp, cls_name, key) + " }");
        }
    }
    lines.push_back("\treturn 0;");
    lines.push_back("}");
    lines.push_back("");
    return text::join(lines, "\n");
}

std::vector<std::pair<std::string, std::string>> read_ancestry(const stdfs::path& cpp_dir) {
    static const Regex kBase(R"(j2xRegisterBase\(typeid\((\w+)\)\.name\(\), typeid\((\w+)\)\.name\(\)\);)");
    std::error_code ec;
    if (!stdfs::is_regular_file(cpp_dir / "j2x_rtti.cpp", ec)) return {};
    const std::string source = text::sanitize_utf8(fs::read_text(cpp_dir / "j2x_rtti.cpp"));
    std::vector<std::pair<std::string, std::string>> out;
    kBase.for_each_match(source, [&](const RegexMatch& m) {
        out.emplace_back(std::string(m.group(1)), std::string(m.group(2)));
        return true;
    });
    return out;
}

std::vector<std::string> read_init_order(const stdfs::path& cpp_dir) {
    static const Regex kInit(R"(^\s*(\w+)::initStatics\(\);)", RegexFlags::Multiline);
    std::error_code ec;
    if (!stdfs::is_regular_file(cpp_dir / "j2x_init.cpp", ec)) return {};
    const std::string source = text::sanitize_utf8(fs::read_text(cpp_dir / "j2x_init.cpp"));
    std::vector<std::string> out;
    kInit.for_each_match(source, [&](const RegexMatch& m) {
        out.emplace_back(m.group(1));
        return true;
    });
    return out;
}

// ---- building and running --------------------------------------------------------------------------------------------------

std::optional<stdfs::path> library(const Toolchain& tools, const stdfs::path& cpp_dir, const stdfs::path& work, std::size_t jobs) {
    const stdfs::path folder = work / "probe" / "_objects";
    const stdfs::path lib = work / "probe" / "libj2x.a";
    std::error_code ec;
    if (stdfs::is_regular_file(lib, ec)) return lib;
    fs::make_dirs(folder);
    std::vector<std::string> units;
    for (const auto& entry : stdfs::directory_iterator(cpp_dir, ec)) {
        const std::string name = fs::to_text(entry.path().filename());
        if (entry.is_regular_file() && name.ends_with(".cpp")) units.push_back(name);
    }
    std::sort(units.begin(), units.end());
    parallel::ThreadPool pool(jobs ? jobs : static_cast<std::size_t>(parallel::workers()));
    const std::vector<std::optional<stdfs::path>> compiled = parallel::parallel_map(pool, units, [&](const std::string& unit) -> std::optional<stdfs::path> {
        const stdfs::path obj = folder / (unit.substr(0, unit.size() - 4) + ".o");
        const UnitResult r = compile_unit(tools, cpp_dir, cpp_dir / unit, obj);
        return r.ok ? std::optional(obj) : std::nullopt;
    });
    const stdfs::path tmp = lib.string() + "." + std::to_string(_getpid()) + ".tmp";
    // the member list in a file: the command line would be too long for Windows
    const stdfs::path listing = tmp.string() + ".rsp";
    std::string members;
    for (const auto& obj : compiled) {
        if (obj) members += fs::to_text(obj->filename()) + "\n";
    }
    fs::write_text(listing, members);
    stdfs::path ar = tools.cxx.parent_path() / "ar.exe";
    if (!stdfs::is_regular_file(ar, ec)) ar = "ar";
    process::ProcessSpec spec;
    spec.argv = {path_arg(ar), "rcs", path_arg(tmp), "@" + path_arg(listing)};
    spec.cwd = folder;
    spec.out = process::Capture::Discard;
    spec.err = process::Capture::Discard;
    const process::ProcessResult r = process::run(spec);
    stdfs::remove(listing, ec);
    if (!r.ok()) {
        stdfs::remove(tmp, ec);
        return std::nullopt;
    }
    move_into_place(tmp, lib);
    return lib;
}

NativeBuild build_native(const Toolchain& tools, const stdfs::path& cpp_dir, const stdfs::path& harness, const stdfs::path& exe, const stdfs::path& lib) {
    NativeBuild out;
    stdfs::path main = harness;
    main.replace_extension(".o");
    std::error_code ec;
    stdfs::remove(main, ec);
    const UnitResult compiled = compile_unit(tools, cpp_dir, harness, main);
    if (compiled.timed_out) {
        out.why = std::format("a compile or link took over {} s", tools.step_timeout.count());
        return out;
    }
    if (!compiled.ok) {
        out.why = "the harness did not compile: " + compiled.error.substr(0, 160);
        return out;
    }
    // the link map names every archive member the linker took
    const stdfs::path map = exe.string() + ".map";
    const ToolRun linked = run_tool({path_arg(tools.cxx), path_arg(main), "-Wl,--start-group", path_arg(lib), "-Wl,--end-group", "-Wl,-Map," + path_arg(map), "-o",
                                     path_arg(exe)},
                                    tools.step_timeout, Streams::Merged);
    if (linked.timed_out) {
        out.why = std::format("a compile or link took over {} s", tools.step_timeout.count());
        return out;
    }
    if (linked.exit_code != 0) {
        // the missing symbols, not the linker's long path in front of them
        std::set<std::string> missing;
        const std::string_view marker = "undefined reference to `";
        for (std::size_t at = linked.out.find(marker); at != std::string::npos; at = linked.out.find(marker, at + 1)) {
            const std::size_t start = at + marker.size(), end = linked.out.find('\'', start);
            if (end != std::string::npos) missing.insert(linked.out.substr(start, end - start));
        }
        if (!missing.empty()) {
            std::string names;
            std::size_t shown = 0;
            for (const std::string& m : missing) {
                if (shown++ == 4) break;
                names += (names.empty() ? "" : ", ") + m;
            }
            out.why = "undefined: " + names.substr(0, 200);
            return out;
        }
        std::string line = linked.out.substr(0, 200);
        for (const std::string_view l : text::lines(linked.out)) {
            if (l.find("undefined") != std::string_view::npos || l.find("error") != std::string_view::npos) {
                line = std::string(l);
                break;
            }
        }
        const std::string_view trimmed = text::trim(line);
        out.why = std::string(trimmed.size() > 200 ? trimmed.substr(trimmed.size() - 200) : trimmed);
        return out;
    }
    out.ok = true;
    if (stdfs::is_regular_file(map, ec)) {
        static const Regex kMember(R"(libj2x\.a\(([A-Za-z_]\w*)\.o\))");
        std::set<std::string> linked_classes;
        kMember.for_each_match(text::sanitize_utf8(fs::read_text(map)), [&](const RegexMatch& m) {
            linked_classes.emplace(m.group(1));
            return true;
        });
        out.linked.assign(linked_classes.begin(), linked_classes.end());
    }
    return out;
}

std::optional<stdfs::path> compile_shims(const stdfs::path& work, const Toolchain& tools) {
    ensure_shims(work, tools);
    const stdfs::path out = work / "probe" / "_shims";
    std::error_code ec;
    if (stdfs::is_regular_file(out / ".compiled", ec)) return out;
    return std::nullopt;
}

std::vector<ClassPlan> plan(const SourceTree& tree, const stdfs::path& cpp_dir, const std::vector<std::string>& names) {
    std::vector<ClassPlan> out;
    for (const std::string& name : names) {
        const ir::JClass* cls = tree.lookup(name);
        if (cls == nullptr) continue;
        const std::optional<std::string> header = read_header(cpp_dir, name);
        if (!header) continue;
        std::vector<Probeable> methods = probeable(tree, *cls, *header, cpp_dir);
        if (!methods.empty()) out.push_back({cls, std::move(methods)});
    }
    return out;
}

Compiled compile_java(const SourceTree& tree, const std::vector<ClassPlan>& todo, const stdfs::path& work, const Toolchain& tools, std::size_t batch,
                      std::size_t jobs, std::string_view tag) {
    Compiled out;
    std::mutex out_lock;
    const stdfs::path root = work / "probe" / "_java";
    ensure_shims(work, tools);

    // One javac for a group of classes. -> ok, and why not
    auto attempt = [&](std::span<const ClassPlan> group, const std::string& label) -> std::pair<bool, std::string> {
        const stdfs::path jdir = root / label;
        std::string listing;
        for (const ClassPlan& c : group) {
            stdfs::path folder = jdir;
            if (c.cls->package) {
                for (const std::string_view part : text::split(*c.cls->package, '.')) folder /= fs::from_text(part);
            }
            const stdfs::path path = folder / (harness_class_name(c.cls->name) + ".java");
            fs::write_text(path, java_harness(c.cls->name, c.methods, c.cls->package, harness_class_name(c.cls->name)));
            listing += "\"" + fs::to_text(path) + "\"\n";
        }
        if (!listing.empty()) listing.pop_back();
        fs::write_text(jdir / "sources.txt", listing);   // an @file: the command line stays short
        std::vector<std::string> argv = {path_arg(tools.javac), "-nowarn", "-encoding", "UTF-8"};
        if (!tools.libraries.empty()) {
            argv.push_back("-cp");
            argv.push_back(classpath(work, tools, {}));
        }
        argv.push_back("-sourcepath");
        argv.push_back(path_arg(tree.dir()));
        argv.push_back("-d");
        argv.push_back(path_arg(jdir));
        argv.push_back("@" + path_arg(jdir / "sources.txt"));
        const ToolRun r = run_tool(std::move(argv), tools.run_timeout * 3 + std::chrono::seconds(10 * static_cast<long long>(group.size())), Streams::Merged);
        if (r.timed_out) return {false, "javac did not finish"};
        if (r.exit_code != 0) {
            std::string first = "failed";
            for (const std::string_view l : text::lines(r.out)) {
                if (l.find("error") != std::string_view::npos) {
                    first = std::string(l);
                    break;
                }
            }
            return {false, "javac: " + first.substr(0, 180)};
        }
        std::lock_guard lock(out_lock);
        for (const ClassPlan& c : group) {
            out[c.cls->name] = {jdir, (c.cls->package ? *c.cls->package + "." : "") + harness_class_name(c.cls->name), ""};
        }
        return {true, ""};
    };
    // A group javac rejects is split in half and tried again, so a class whose source cannot compile fails alone.
    std::function<void(std::span<const ClassPlan>, const std::string&)> settle = [&](std::span<const ClassPlan> group, const std::string& label) {
        const auto [ok, why] = attempt(group, label);
        if (ok) return;
        if (group.size() == 1) {
            std::lock_guard lock(out_lock);
            out[group[0].cls->name] = {{}, "", why};
            return;
        }
        const std::size_t half = group.size() / 2;
        settle(group.first(half), label + "a");
        settle(group.subspan(half), label + "b");
    };

    const std::size_t batches = (todo.size() + batch - 1) / batch;
    parallel::ThreadPool pool(std::min<std::size_t>(jobs ? jobs : static_cast<std::size_t>(parallel::workers()), std::max<std::size_t>(batches, 1)));
    parallel::parallel_for(pool, batches, [&](std::size_t i) {
        const std::size_t from = i * batch;
        settle(std::span(todo).subspan(from, std::min(batch, todo.size() - from)), std::string(tag) + std::to_string(i));
    });
    return out;
}

namespace {

// Run the original once more on just the cases in `only`, in their order; nullopt when it could not be compiled or run.
std::optional<std::map<std::string, std::string>> java_again(const SourceTree& tree, const ir::JClass& cls, const std::vector<Probeable>& methods,
                                                             const stdfs::path& work, const Toolchain& tools, const stdfs::path& jdir,
                                                             const std::set<std::string>& only) {
    const stdfs::path kdir = work / "probe" / cls.name / "_java_only";
    stdfs::path folder = kdir;
    if (cls.package) {
        for (const std::string_view part : text::split(*cls.package, '.')) folder /= fs::from_text(part);
    }
    const std::string name = harness_class_name(cls.name) + "Only";
    const stdfs::path path = folder / (name + ".java");
    fs::write_text(path, java_harness(cls.name, methods, cls.package, name, &only));
    const ToolRun compiled = run_tool({path_arg(tools.javac), "-nowarn", "-encoding", "UTF-8", "-cp", classpath(work, tools, {jdir}), "-sourcepath",
                                       path_arg(tree.dir()), "-d", path_arg(kdir), path_arg(path)},
                                      tools.run_timeout * 3, Streams::Stdout);
    if (!compiled.ok()) return std::nullopt;
    const ToolRun ran = run_tool({path_arg(tools.java), "-cp", classpath(work, tools, {kdir, jdir}), (cls.package ? *cls.package + "." : "") + name},
                                 tools.run_timeout, Streams::Stdout);
    if (ran.timed_out) return std::nullopt;
    return parse_results(ran.out);
}

bool built_in_two_phases(const stdfs::path& cpp_dir, const std::string& name) {
    const std::optional<std::string> header = read_header(cpp_dir, name);
    return header && header->find(" j2xInit(") != std::string::npos;
}

// The classes the harness names besides the class itself (made as arguments, compared as results).
std::set<std::string> classes_named(const std::vector<Probeable>& methods, const std::string& cls_name) {
    std::set<std::string> out;
    for (const Probeable& m : methods) {
        for (const Slot& s : m.slots) {
            if (const auto* obj = std::get_if<ObjectSlot>(&s)) out.insert(obj->cls);
        }
        if (m.outcome.kind == Outcome::Kind::Object) out.insert(m.outcome.cls);
    }
    out.erase(cls_name);
    return out;
}

}  // namespace

RunResult run(const SourceTree& tree, const stdfs::path& cpp_dir, const ir::JClass& cls, const stdfs::path& work, const Toolchain& tools,
              const Compiled* compiled) {
    std::vector<Probeable> methods;
    if (const auto header = read_header(cpp_dir, cls.name)) methods = probeable(tree, cls, *header, cpp_dir);
    RunResult res;
    for (const Probeable& m : methods) res.methods.push_back(m.name);
    res.total = counted(cls);
    if (methods.empty()) {
        res.why = reads_clock(cls).any() ? "it reads the clock or the clipboard (they differ between the two runs)"
                                         : "no method it can call and compare (its arguments made, its result or object read back)";
        return res;
    }
    const stdfs::path folder = work / "probe" / cls.name;
    fs::make_dirs(folder);

    CompiledHarness harness;
    if (compiled != nullptr && compiled->contains(cls.name)) {
        harness = compiled->at(cls.name);
    } else {   // not compiled with the module's batch: alone
        const Compiled alone = compile_java(tree, {{&cls, methods}}, work, tools, 40, 0, cls.name);
        harness = alone.at(cls.name);
    }
    if (!harness.ok()) {
        res.why = harness.why;
        return res;
    }

    // The original runs twice: a method any of whose results changes between two runs of Java itself (a Random seeded from the clock,
    // the time, an identity hash) is left out whole.
    std::vector<std::map<std::string, std::string>> runs;
    for (int twice = 0; twice < 2; ++twice) {
        const ToolRun r = run_tool({path_arg(tools.java), "-cp", classpath(work, tools, {harness.classes_dir}), harness.main_class}, tools.run_timeout,
                                   Streams::Stdout);
        if (r.timed_out) {
            res.why = "the Java side did not finish";
            return res;
        }
        runs.push_back(parse_results(r.out));
    }
    std::map<std::string, std::string> java_results = runs[0];
    auto method_of = [](const std::string& key) { return key.substr(0, key.find('.')); };
    std::set<std::string> shaky;
    for (const auto& [k, v] : java_results) {
        const auto again = runs[1].find(k);
        if (again == runs[1].end() || again->second != v) shaky.insert(method_of(k));
    }
    std::set<std::string> unsteady, keep;
    for (const auto& [k, v] : java_results) {
        if (shaky.contains(method_of(k))) unsteady.insert(k);
    }
    res.unsteady = unsteady.size();
    for (const auto& [k, v] : java_results) {
        if (v != "throw" && !unsteady.contains(k)) keep.insert(k);
    }
    if (keep.empty()) {
        res.why = unsteady.empty() ? "the Java methods threw on every input" : "Java gives a different result on every run";
        return res;
    }
    // The cases run one after another in one program, and a case can leave state behind for the next ones (an Item's constructor
    // registers it in Item.itemsList, even when the rest of that call throws). The C++ side runs only the cases kept, so when some
    // were left out the original is run again on exactly those, in the same order, and that is what C++ is compared with; repeated
    // until no kept case throws.
    bool settled = keep.size() == java_results.size();
    for (int again = 0; again < 4 && !settled; ++again) {
        const auto sub = java_again(tree, cls, methods, work, tools, harness.classes_dir, keep);
        if (!sub) {
            res.why = "the original could not be run again on the cases kept";
            return res;
        }
        std::set<std::string> now;
        for (const std::string& k : keep) {
            const auto it = sub->find(k);
            if (it != sub->end() && it->second != "throw") now.insert(k);
        }
        for (const std::string& k : now) java_results[k] = sub->at(k);
        settled = now == keep;
        keep = std::move(now);
        if (keep.empty()) break;
    }
    if (keep.empty() || !settled) {
        res.why = "the cases depend on what the ones left out did (no steady set to compare)";
        return res;
    }
    const auto lib = library(tools, cpp_dir, work);
    if (!lib) {
        res.why = "the program could not be archived for linking";
        return res;
    }
    const std::vector<std::string> order = read_init_order(cpp_dir);
    const stdfs::path source = folder / "probe.cpp", exe = folder / "probe.exe";
    const std::set<std::string> named = classes_named(methods, cls.name);
    std::set<std::string> present = named;
    present.insert(cls.name);
    std::vector<std::string> need;
    for (const std::string& c : order) {
        if (present.contains(c)) need.push_back(c);
    }
    twophase::NameSet two_phase;
    for (const std::string& c : present) {
        if (built_in_two_phases(cpp_dir, c)) two_phase.insert(c);
    }
    const auto edges = read_ancestry(cpp_dir);
    NativeBuild built;
    for (int pass = 0; pass < 4; ++pass) {
        std::vector<std::pair<std::string, std::string>> pairs;
        for (const auto& [derived, base] : edges) {
            if (present.contains(derived) && present.contains(base)) pairs.emplace_back(derived, base);
        }
        fs::write_text(source, twophase::rewrite_news(cpp_harness(cls.name, methods, keep, need, named, pairs), two_phase).text);
        built = build_native(tools, cpp_dir, source, exe, *lib);
        // the classes linked in may have static initialisers of their own, and bases to register: all of theirs, in order
        std::set<std::string> now(built.linked.begin(), built.linked.end());
        now.insert(named.begin(), named.end());
        now.insert(cls.name);
        std::vector<std::string> wanted;
        for (const std::string& c : order) {
            if (now.contains(c)) wanted.push_back(c);
        }
        if (!built.ok || (wanted == need && now == present)) break;
        need = std::move(wanted);
        present = std::move(now);
    }
    if (!built.ok) {
        res.why = "did not link: " + built.why;
        return res;
    }
    const ToolRun ran = run_tool({path_arg(exe)}, tools.run_timeout, Streams::Stdout);
    if (ran.timed_out) {
        res.why = "the C++ side did not finish";
        return res;
    }
    const std::map<std::string, std::string> native_results = parse_results(ran.out);
    std::set<std::size_t> judged;
    for (std::size_t mi = 0; mi < methods.size(); ++mi) {
        const auto all_cases = cases(flat_types(methods[mi].slots, methods[mi].made));
        for (std::size_t ci = 0; ci < all_cases.size(); ++ci) {
            const std::string key = case_key(mi, ci);
            if (!keep.contains(key)) continue;
            ++res.cases;
            judged.insert(mi);
            const std::string kind = methods[mi].outcome.kind == Outcome::Kind::Primitive ? methods[mi].outcome.type : "obj";
            const auto got = native_results.find(key);
            if (got == native_results.end()) res.differs.push_back({methods[mi].name, all_cases[ci], java_results.at(key), "no result (crashed?)"});
            else if (!same(kind, java_results.at(key), got->second)) res.differs.push_back({methods[mi].name, all_cases[ci], java_results.at(key), got->second});
        }
    }
    res.judged = judged.size();
    return res;
}

}  // namespace cb::java::probe
