#include "verify/lanes/java.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "base/cache.hpp"
#include "core/crypto.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/process.hpp"
#include "core/text.hpp"
#include "java/corpus.hpp"
#include "java/parse.hpp"
#include "java/textutil.hpp"
#include "verify/lane_common.hpp"

namespace cb::verify::lanes {
namespace {

namespace stdfs = std::filesystem;
using Item = JavaLane::Item;

// The checks and the harnesses are compiled with these; the compile's switches are the flags minus `-fsyntax-only`.
constexpr std::string_view kSyntaxOnly = "-fsyntax-only";

std::string read_replaced(const stdfs::path& file) { return java::textutil::read_text(file, java::textutil::Decode::Replace); }

// Whether a line says `error` as a word.
bool says_error(std::string_view line) {
    const auto word = [](char c) { return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_'; };
    for (std::size_t at = line.find("error"); at != std::string_view::npos; at = line.find("error", at + 1)) {
        const bool before = at == 0 || !word(line[at - 1]);
        const bool after = at + 5 >= line.size() || !word(line[at + 5]);
        if (before && after) return true;
    }
    return false;
}

std::string joined(std::span<const std::string> lines, std::string_view separator) { return text::join(lines, separator); }

}  // namespace

std::vector<std::string> JavaLane::flags(std::string_view standard) {
    return {"-std=" + std::string(standard), std::string(kSyntaxOnly), "-w"};
}

const std::string& JavaLane::name() const {
    static const std::string kName = "java";
    return kName;
}

reconstruct::KindTraits JavaLane::traits() {
    reconstruct::KindTraits t;
    t.per_function = true;
    t.identifies_library = false;
    t.source_rev = std::format("java-{}", kRev);
    t.sha = [](const stdfs::path& path) -> std::optional<std::string> { return module_sha(path); };
    return t;
}

// ---- the module ----------------------------------------------------------------------------------------------------------

bool JavaLane::is_module(const stdfs::path& path) {
    std::error_code ec;
    if (stdfs::is_regular_file(path, ec)) return text::lower(fs::to_text(path)).ends_with(".java");
    if (!stdfs::is_directory(path, ec)) return false;
    for (stdfs::recursive_directory_iterator it(path, stdfs::directory_options::skip_permission_denied, ec), end; !ec && it != end; it.increment(ec)) {
        std::error_code status_ec;
        if (it->is_regular_file(status_ec) && it->path().extension() == ".java") return true;
    }
    return false;
}

stdfs::path JavaLane::source_dir(const stdfs::path& path) {
    std::error_code ec;
    return stdfs::is_regular_file(path, ec) ? stdfs::absolute(path).parent_path() : path;
}

std::string JavaLane::module_sha(const stdfs::path& path) {
    const stdfs::path root = source_dir(path);
    Bytes all;
    const auto add = [&all](std::string_view s) { all.insert(all.end(), s.begin(), s.end()); };
    for (const stdfs::path& file : java::corpus::discover(root)) {
        add(text::replace_all(fs::to_text(file.lexically_relative(root)), "\\", "/"));
        all.push_back(0);
        const Bytes content = fs::read_file(file);
        all.insert(all.end(), content.begin(), content.end());
        all.push_back(0);
    }
    return crypto::to_hex(crypto::sha256(all));
}

// ---- the host compiler -----------------------------------------------------------------------------------------------------

JavaLane::Check JavaLane::check(const stdfs::path& cxx, const stdfs::path& include_dir, const stdfs::path& unit,
                                const std::optional<stdfs::path>& prefix, std::string_view standard) {
    // -iquote, not -I: the emitted tree has headers named like the C library's (Math.h, Locale.h) and on a case-insensitive disk -I
    // would put them in front of <math.h> and <locale.h>
    process::ProcessSpec spec;
    spec.argv = {fs::to_text(cxx)};
    const std::vector<std::string> switches = flags(standard);
    spec.argv.insert(spec.argv.end(), switches.begin(), switches.end());
    spec.argv.insert(spec.argv.end(), {"-iquote", fs::to_text(include_dir)});
    if (prefix && !prefix->empty()) spec.argv.insert(spec.argv.end(), {"-include", fs::to_text(*prefix)});
    spec.argv.push_back(fs::to_text(unit));
    spec.out = process::Capture::Discard;
    const process::ProcessResult r = process::run(spec);
    const std::string said(as_text(r.err));
    Check out;
    for (const std::string_view line : text::lines(said)) {
        if (says_error(line)) out.errors.emplace_back(line);
    }
    out.ok = r.exit_code == 0 && out.errors.empty();
    return out;
}

std::optional<stdfs::path> JavaLane::precompile(const stdfs::path& cxx, const stdfs::path& include_dir, const stdfs::path& work,
                                                std::string_view standard) {
    // GCC's (g++, g++-13, x86_64-w64-mingw32-g++): a precompiled .gch is its format; clang++ has "g++" in its name and is not it
    const std::string compiler = text::lower(fs::to_text(cxx.stem()));
    if (compiler != "g++" && !compiler.ends_with("-g++") && !compiler.starts_with("g++-")) return std::nullopt;
    const stdfs::path folder = work / "pch";
    fs::make_dirs(folder);
    const stdfs::path head = folder / "j2x_pch.h";
    java::textutil::write_text(head, "#include \"stdafx.h\"\n#include \"JavaRT.h\"\n");
    process::ProcessSpec spec;
    spec.argv = {fs::to_text(cxx)};
    for (const std::string& f : flags(standard)) {
        if (f != kSyntaxOnly) spec.argv.push_back(f);
    }
    spec.argv.insert(spec.argv.end(), {"-iquote", fs::to_text(include_dir), "-x", "c++-header", fs::to_text(head), "-o", fs::to_text(head) + ".gch"});
    spec.out = process::Capture::Discard;
    spec.err = process::Capture::Discard;
    const process::ProcessResult r = process::run(spec);
    if (r.exit_code == 0 && stdfs::is_regular_file(fs::to_text(head) + ".gch")) return head;
    return std::nullopt;
}

// ---- the kind --------------------------------------------------------------------------------------------------------------

Profile JavaLane::profile(const stdfs::path&) const {
    const Tools tools = tools_();
    if (tools.cxx.empty()) throw NotFoundError("g++", 0, "no C++ compiler to judge Java with (settings: host_cxx, or g++ on PATH)");
    std::string libraries;
    for (const stdfs::path& jar : tools.libraries) libraries += (libraries.empty() ? "" : ",") + fs::to_text(jar.filename());   // what the game is compiled against
    const auto stamp = [](const stdfs::path& tool) { return tool.empty() ? std::string("none") : base::cache::tool_id(tool); };
    const std::string settings = std::format("java rev {} cxx {} flags {} probe {} javac {} java {}{}", kRev, stamp(tools.cxx), joined(flags(options_.standard), " "),
                                             java::probe::kRevision, stamp(tools.javac), stamp(tools.java),
                                             libraries.empty() ? std::string() : " libraries " + libraries);
    return {crypto::to_hex(crypto::sha1(as_bytes(settings))).substr(0, 12), settings};
}

std::unique_ptr<Prepared> JavaLane::prepare(const stdfs::path& module_path, const stdfs::path& work) const {
    if (text::lower(fs::to_text(module_path)).ends_with(".jar")) {
        throw UsageError(std::format("{}: a jar -- decompile it to Java source first (codebreaker java decompile)", module_path.filename().string()));
    }
    auto m = std::make_unique<PreparedJava>();
    m->src = source_dir(module_path);
    m->out = work / "cpp";
    for (const char* stale_name : {"probe", "javasrc"}) {   // the last run's objects and archive are of other C++
        std::error_code ec;
        stdfs::remove_all(work / stale_name, ec);
    }
    std::error_code ec;
    stdfs::remove_all(m->out, ec);
    m->summary = convert_(m->src, m->out);
    for (const stdfs::path& file : java::corpus::discover(m->src)) m->sources[fs::to_text(file.stem())] = file;
    const std::set<std::string> markers(m->summary.markers.begin(), m->summary.markers.end());   // (the dropped empty markers)
    std::vector<std::string> classes;
    for (const auto& entry : m->sources) {
        if (!markers.contains(entry.first)) classes.push_back(entry.first);   // (a map: sorted)
    }
    std::set<std::string> emitted;   // the files the transpiler wrote, by name
    for (const auto& entry : stdfs::directory_iterator(m->out)) emitted.insert(fs::to_text(entry.path().filename()));
    for (const std::string& c : classes) {
        std::vector<std::string> parts;
        for (const char* ext : {".h", ".cpp"}) {
            if (emitted.contains(c + ext)) parts.push_back(read_replaced(m->out / (c + ext)));
        }
        if (!parts.empty()) m->texts[c] = text::join(parts, "\n");
    }
    for (const std::string& f : emitted) {   // the support headers: what is not a class's own
        if (f.ends_with(".h") && !m->texts.contains(fs::to_text(fs::from_text(f).stem()))) {
            m->header += std::format("// ---- {}\n{}\n", f, read_replaced(m->out / fs::from_text(f)));
        }
    }
    for (const auto& entry : m->texts) m->order.push_back(entry.first);
    std::ranges::sort(m->order);
    for (const std::string& c : m->order) m->key[c] = c;
    m->all_keys = classes;
    std::string revision = std::format("{}", kRev);
    revision.push_back('\0');
    m->sig = crypto::to_hex(crypto::sha1(as_bytes(revision + m->header)));
    return m;
}

Built JavaLane::build(const Prepared& prepared, const stdfs::path&, const stdfs::path& work) const {
    const auto& m = static_cast<const PreparedJava&>(prepared);
    const Tools tools = tools_();
    const stdfs::path units = work / "units";
    fs::make_dirs(units);
    std::map<std::string, Item> items;
    for (const std::string& c : m.order) {
        stdfs::path unit = m.out / (c + ".cpp");
        if (!stdfs::is_regular_file(unit)) {   // (a header-only class: a unit that includes it)
            unit = units / (c + ".cpp");
            java::textutil::write_text(unit, std::format("#include \"stdafx.h\"\n#include \"{}.h\"\n", c));
        }
        const auto source = m.sources.find(c);
        items[c] = Item{c, unit, source == m.sources.end() ? stdfs::path() : source->second};
    }
    auto context = std::make_shared<Context>();
    context->tools = tools;
    context->standard = options_.standard;
    context->include_dir = m.out;
    context->work = work;
    java::probe::Toolchain& tc = context->toolchain;
    tc.cxx = tools.cxx;
    tc.javac = tools.javac;
    tc.java = tools.java;
    tc.cxx_std = options_.standard;
    tc.libraries = tools.libraries;
    tc.shim_sources = tools.shim_sources;
    tc.prefix = precompile(tools.cxx, m.out, work, options_.standard);
    if (!tools.javac.empty()) {
        const stdfs::path staged = work / "javasrc";
        java::probe::stage_sources(m.src, staged);
        context->tree = std::make_shared<java::probe::SourceTree>(staged);
        java::probe::library(tc, m.out, work);   // the whole program, once, before the judges link to it
        // every class's Java harness, compiled in batches here rather than one javac per class there
        context->compiled = java::probe::compile_java(*context->tree, java::probe::plan(*context->tree, m.out, m.order), work, tc);
    }
    Built built;
    built.items = m.order;
    built.judge = [context, items = std::move(items)](const std::string& name) { return judge_class(*context, items.at(name)); };
    return built;
}

store::Judgement JavaLane::judge_class(const Context& context, const Item& item) {
    const store::Judgement judged = lane_common::judged([&]() -> store::Judgement {
        const Check checked = check(context.tools.cxx, context.include_dir, item.unit, context.toolchain.prefix, context.standard);
        if (!checked.ok) {
            const std::string first = checked.errors.empty() ? "the compiler rejected it" : checked.errors.front();
            const std::size_t shown = std::min<std::size_t>(checked.errors.size(), 20);
            return {store::Verdict::NoCompile, first.substr(0, 200), {}, joined(std::span(checked.errors).first(shown), "\n")};
        }
        const auto compiles = [&](const std::string& why) {
            return std::format("compiles ({} {}); not run: {}", fs::to_text(context.tools.cxx.filename()), joined(flags(context.standard), " "), why);
        };
        if (context.tools.javac.empty() || item.source.empty() || !context.tree) {
            return {store::Verdict::Inconclusive, "not-run", {}, compiles("no JDK (settings: java)")};
        }
        java::ir::JClass cls;
        try {
            cls = java::parse::parse_file(item.source);
        } catch (const Error& e) {   // the transpiler parsed it; say so if this did not
            return {store::Verdict::Inconclusive, "not-run", {}, compiles(std::format("could not read {}: {}", fs::to_text(item.source), e.what()))};
        }
        const java::probe::RunResult r = java::probe::run(*context.tree, context.include_dir, cls, context.work, context.toolchain, &context.compiled);
        if (r.judged == 0) return {store::Verdict::Inconclusive, "not-run", {}, compiles(r.why)};
        const std::string said = std::format("{} of {} methods run beside the JVM on {} inputs", r.judged, r.total, r.cases);
        if (!r.differs.empty()) {
            std::vector<std::string> lines;
            for (std::size_t i = 0; i < r.differs.size() && i < 12; ++i) {
                const java::probe::Difference& d = r.differs[i];
                lines.push_back(std::format("{}({}): Java {}, C++ {}", d.method, text::join(d.inputs, ", "), d.java, d.native));
            }
            return {store::Verdict::Divergent, "result-differs", {}, said + std::format("; {} differ:\n", r.differs.size()) + joined(lines, "\n")};
        }
        std::vector<std::string> limits;
        if (r.judged < r.total) limits.push_back("methods-partial");
        return {store::Verdict::Equivalent, "", limits, said + ": every result alike"};
    });
    return judged;
}

}  // namespace cb::verify::lanes
