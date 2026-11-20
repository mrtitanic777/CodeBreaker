// app/verbs.cpp -- see verbs.hpp.
#include "app/verbs.hpp"

#include <algorithm>
#include <array>
#include <filesystem>
#include <format>

#include "app/commands.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/log.hpp"
#include "core/text.hpp"
#include "formats/detect.hpp"

namespace cb::app::verbs {

namespace {

namespace stdfs = std::filesystem;

constexpr std::array<std::string_view, 9> kVerbs = {"convert", "inspect", "extract", "build", "check", "compare", "corpus", "lookup", "xbox"};
constexpr std::array<std::string_view, 10> kKept = {"settings", "cache", "regress", "serve", "mcp", "app", "disassemble", "assemble", "compile", "link"};   // (the last four: the front door of each role, app/role_commands)

// The modes: verb, flag (or word), the older command, the kind of input that picks it (when rows share a flag).
// clang-format off
constexpr Mode kModes[] = {
    // convert FILE: any program to source; the default is the window's pipeline, which routes itself
    {"convert", "",              "convert",           std::nullopt},
    {"convert", "--decompile",   "decompile",         std::nullopt},
    {"convert", "--module",      "module",            std::nullopt},
    {"convert", "--asm",         "pe-source",         Kind::Pe},
    {"convert", "--asm",         "ne-source",         Kind::Ne},
    {"convert", "--asm",         "le-source",         Kind::Le},
    {"convert", "--asm",         "dos-source",        Kind::Dos},
    {"convert", "--driver",      "driver-source",     std::nullopt},
    {"convert", "--vxd",         "vxd",               std::nullopt},
    {"convert", "--ne-asm",      "ne-asm",            std::nullopt},
    {"convert", "--flat-thunk",  "flat-thunk",        std::nullopt},
    {"convert", "--build16",     "build16",           std::nullopt},
    {"convert", "--invert",      "invert",            std::nullopt},
    {"convert", "--recompile",   "recompile",         std::nullopt},
    {"convert", "--project",     "recompile-project", std::nullopt},
    {"convert", "--xna",         "xna",               std::nullopt},
    {"convert", "--java",        "java",              std::nullopt},
    // inspect FILE: what it is and holds
    {"inspect", "",                "info",            std::nullopt},
    {"inspect", "--names",         "names",           std::nullopt},
    {"inspect", "--library",       "identify",        std::nullopt},
    {"inspect", "--origin",        "origin",          std::nullopt},
    {"inspect", "--analyze",       "analyze",         std::nullopt},
    {"inspect", "--classes",       "classes",         std::nullopt},
    {"inspect", "--functions",     "dos-functions",   Kind::Dos},
    {"inspect", "--imports",       "xex-imports",     std::nullopt},
    {"inspect", "--calls",         "ppc-calls",       std::nullopt},
    {"inspect", "--switch-tables", "switch-tables",   std::nullopt},
    {"inspect", "--facts",         "facts",           std::nullopt},
    // extract FILE [OUT]: files out of any container (--list lists instead)
    {"extract", "--media",       "media",             std::nullopt},
    {"extract", "--cab",         "cab",               std::nullopt},
    {"extract", "--xbox-disc",   "xbox-disc",         std::nullopt},
    {"extract", "--heavyiron",   "heavyiron",         std::nullopt},
    {"extract", "--gfx",         "gfx",               std::nullopt},
    {"extract", "--fonts",       "fonts",             std::nullopt},
    {"extract", "--system-ui",   "system-ui",         std::nullopt},
    {"extract", "--basic",       "xex-basic",         std::nullopt},
    // build FOLDER: make it again from its source folder
    {"build", "",                "build",             std::nullopt},
    {"build", "--module",        "build",             std::nullopt},
    {"build", "--dos",           "dos-build",         std::nullopt},
    {"build", "--driver",        "driver-build",      std::nullopt},
    {"build", "--resources",     "resources",         std::nullopt},
    {"build", "--floppy",        "boot-floppy",       std::nullopt},
    {"build", "--nasm",          "nasm",              std::nullopt},
    {"build", "--disc",          "disc",              std::nullopt},
    {"build", "--xenon",         "xenon-recompile",   std::nullopt},
    // check FILE: prove it behaves (or matches bytes)
    {"check", "",                "measure",           std::nullopt},
    {"check", "--bytes",         "match",             std::nullopt},
    {"check", "--exact",         "exact",             std::nullopt},
    {"check", "--dos-run",       "dos-run",           std::nullopt},
    {"check", "--repair",        "repair",            std::nullopt},
    {"check", "--teeth",         "teeth",             std::nullopt},
    {"check", "--drivers",       "driver-judge",      std::nullopt},
    {"check", "--java",          "judge-c-java",      std::nullopt},
    {"check", "--cs",            "judge-cs-cpp",      std::nullopt},
    {"check", "--py",            "judge-py-cpp",      std::nullopt},
    {"check", "--score16",       "score16",           std::nullopt},
    // compare A B: two builds, function by function
    {"compare", "",              "versions",          std::nullopt},
    // corpus WORD: the whole collection
    {"corpus", "run",            "sweep",             std::nullopt},
    {"corpus", "progress",       "progress",          std::nullopt},
    {"corpus", "scoreboard",     "scoreboard",        std::nullopt},
    {"corpus", "place",          "place",             std::nullopt},
    {"corpus", "coverage",       "coverage",          std::nullopt},
    {"corpus", "reach",          "reach",             std::nullopt},
    {"corpus", "list",           "corpus-executables", std::nullopt},
    // lookup NAME: a name, service or signature
    {"lookup", "--demangle",     "demangle",          std::nullopt},
    {"lookup", "--interrupt",    "dos-interrupts",    std::nullopt},
    {"lookup", "--services",     "driver-services",   std::nullopt},
    {"lookup", "--knowledge",    "knowledge",         std::nullopt},
    // xbox WORD: Xbox 360 specialist tools
    {"xbox", "xex",              "xex",               std::nullopt},
    {"xbox", "edit",             "xex-edit",          std::nullopt},
    {"xbox", "crypto",           "crypto",            std::nullopt},
    {"xbox", "config",           "xenon-config",      std::nullopt},
    {"xbox", "match",            "xdk-match",         std::nullopt},
    // cache: the clean-up and carry-over commands it takes in
    {"cache", "--tidy",          "tidy-state",        std::nullopt},
    {"cache", "--migrate",       "migrate-state",     std::nullopt},
};
// clang-format on

bool word_flag(std::string_view verb) { return verb == "corpus" || verb == "xbox"; }

std::string lower(std::string_view s) { return text::lower(s); }

bool ends_with_any(const std::string& name, std::initializer_list<std::string_view> suffixes) {
    const std::string l = lower(name);
    for (const std::string_view s : suffixes)
        if (l.size() >= s.size() && l.compare(l.size() - s.size(), s.size(), s) == 0) return true;
    return false;
}

// Options that take a value: the value is not the input.
bool takes_value(std::string_view a) {
    return a == "--to" || a == "--out" || a == "-o" || a == "--from" || a == "--stub" || a == "--against" || a == "--name" || a == "--original" ||
           a == "--what" || a == "--work" || a == "--main" || a == "--cxx";
}

// The inputs: the words that are not options or an option's value.
std::vector<std::string> inputs_of(std::span<const std::string> args) {
    std::vector<std::string> in;
    for (std::size_t i = 0; i < args.size(); ++i) {
        if (takes_value(args[i])) {
            ++i;
            continue;
        }
        if (!args[i].empty() && args[i][0] == '-') continue;
        in.push_back(args[i]);
    }
    return in;
}

Kind first_kind(std::span<const std::string> args, const Probe& probe) {
    const std::vector<std::string> in = inputs_of(args);
    return in.empty() ? Kind::Missing : probe(in.front());
}

std::vector<std::string> without(std::span<const std::string> args, std::string_view flag) {
    std::vector<std::string> out;
    bool dropped = false;
    for (const std::string& a : args) {
        if (!dropped && a == flag) {
            dropped = true;
            continue;
        }
        out.push_back(a);
    }
    return out;
}

bool has(std::span<const std::string> args, std::string_view flag) { return std::find(args.begin(), args.end(), flag) != args.end(); }

Route one(std::string command, std::vector<std::string> args) {
    return Route{Step{std::move(command), std::move(args)}};
}

// ---- extract: the sub-word each container's command wants, for a list and for taking out
struct Extractor {
    std::string_view command, list_word, extract_word;
};
constexpr Extractor kExtractors[] = {
    {"cab", "list", "extract"},      {"media", "list", "extract"},  {"xbox-disc", "files", "unpack"}, {"heavyiron", "list", "extract"},
    {"gfx", "scan", "images"},       {"xex", "list", "extract"},    {"fonts", "", ""},                {"system-ui", "", ""},
    {"xex-basic", "", ""},
};

Route extract_with(std::string_view command, std::vector<std::string> args) {
    const bool list = has(args, "--list");
    if (list) args = without(args, "--list");
    for (const Extractor& e : kExtractors) {
        if (e.command != command) continue;
        const std::string_view word = list ? e.list_word : e.extract_word;
        if (!word.empty()) args.insert(args.begin(), std::string(word));
        break;
    }
    return one(std::string(command), std::move(args));
}

std::optional<Route> route_extract(std::span<const std::string> args, const Probe& probe) {
    std::vector<std::string> rest(args.begin(), args.end());
    for (const Mode& m : kModes) {
        if (m.verb != "extract" || m.flag.empty() || !has(rest, m.flag)) continue;
        rest = without(rest, m.flag);
        return extract_with(m.old_name, std::move(rest));
    }
    if (inputs_of(args).empty()) return std::nullopt;
    switch (first_kind(args, probe)) {
        case Kind::Cab: return extract_with("cab", rest);
        case Kind::Disc: return extract_with("media", rest);
        case Kind::XboxDisc: return extract_with("xbox-disc", rest);
        case Kind::HeavyIron: return extract_with("heavyiron", rest);
        case Kind::Gfx: return extract_with("gfx", rest);
        case Kind::Font: return extract_with("fonts", rest);
        case Kind::Xex: return extract_with("xex", rest);
        default: break;
    }
    throw UsageError(std::format("extract: cannot tell what to take out of '{}' (a cabinet, disc image, package, archive or font); name the kind: --cab, --media, --xbox-disc, --heavyiron, --gfx, --fonts, --system-ui or --basic",
                                 inputs_of(args).front()));
}

// ---- the other verbs
std::optional<Route> route_convert(std::span<const std::string> args, const Probe& probe) {
    std::vector<std::string> rest(args.begin(), args.end());
    const bool check = has(rest, "--check");
    for (const Mode& m : kModes) {
        if (m.verb != "convert" || m.flag.empty() || !has(rest, m.flag)) continue;
        std::string_view command = m.old_name;
        rest = without(rest, m.flag);
        if (m.flag == "--asm") {
            const Kind k = first_kind(rest, probe);
            command = {};
            for (const Mode& c : kModes)
                if (c.verb == "convert" && c.flag == "--asm" && c.kind == k) command = c.old_name;
            if (command.empty()) {
                const std::vector<std::string> in = inputs_of(rest);
                throw UsageError(std::format("convert --asm: cannot tell what kind of program '{}' is (a Windows, DOS or driver file)", in.empty() ? std::string() : in.front()));
            }
            rest = without(rest, "--check"); // these build and compare as they go
        } else if (m.flag == "--java" || m.flag == "--xna" || m.flag == "--decompile") {
            // pass through whole
        }
        return one(std::string(command), std::move(rest));
    }
    if (check) {
        rest = without(rest, "--check");
        const std::vector<std::string> in = inputs_of(rest);
        if (in.empty()) return one("convert", std::move(rest));
        if (probe(in.front()) == Kind::Xex) return one("recompile", {in.front(), "--all", "--check"});
        Route r = one("convert", rest);
        if (!has(rest, "--to")) r.push_back(Step{"measure", {in.front()}}); // the pipeline proves a translation itself; a program is judged here
        return r;
    }
    return one("convert", std::move(rest));
}

std::optional<Route> route_inspect(std::span<const std::string> args, const Probe& probe) {
    std::vector<std::string> rest(args.begin(), args.end());
    for (const Mode& m : kModes) {
        if (m.verb != "inspect" || m.flag.empty() || !has(rest, m.flag)) continue;
        rest = without(rest, m.flag);
        if (m.flag == "--functions" && first_kind(rest, probe) != Kind::Dos) return one("names", std::move(rest));
        return one(std::string(m.old_name), std::move(rest));
    }
    if (rest.empty()) return std::nullopt;
    switch (first_kind(args, probe)) {
        case Kind::Folder: case Kind::DriverSource: case Kind::DosSource: case Kind::CSource: return one("reach", std::move(rest));
        default: return one("info", std::move(rest));
    }
}

std::optional<Route> route_build(std::span<const std::string> args, const Probe& probe) {
    std::vector<std::string> rest(args.begin(), args.end());
    for (const Mode& m : kModes) {
        if (m.verb != "build" || m.flag.empty() || !has(rest, m.flag)) continue;
        rest = without(rest, m.flag);
        return one(std::string(m.old_name), std::move(rest));
    }
    const std::vector<std::string> in = inputs_of(args);
    if (in.empty()) return std::nullopt;
    switch (probe(in.front())) {
        case Kind::Config: return one("xenon-recompile", std::move(rest));
        case Kind::DriverSource: return one("driver-build", std::move(rest));
        case Kind::DosSource: return one("dos-build", std::move(rest));
        case Kind::CSource: return one("build16", std::move(rest));
        case Kind::Folder:
            throw UsageError(std::format("build: '{}' holds no source it knows how to build (assembly or C); name the kind: --dos, --driver, --disc OUT.iso", in.front()));
        default: break;
    }
    if (ends_with_any(in.front(), {".asm"})) return one("driver-build", std::move(rest));
    return one("build", std::move(rest)); // a module (a name or a file): the whole-module build
}

std::optional<Route> route_check(std::span<const std::string> args, const Probe& probe) {
    std::vector<std::string> rest(args.begin(), args.end());
    for (const Mode& m : kModes) {
        if (m.verb != "check" || m.flag.empty() || !has(rest, m.flag)) continue;
        rest = without(rest, m.flag);
        if (m.old_name == "judge-c-java") rest.insert(rest.begin(), "roundtrip");
        return one(std::string(m.old_name), std::move(rest));
    }
    const std::vector<std::string> in = inputs_of(args);
    if (in.empty()) return std::nullopt;
    switch (probe(in.front())) {
        case Kind::JavaSource: rest.insert(rest.begin(), "roundtrip"); return one("judge-c-java", std::move(rest));
        case Kind::CsSource: return one("judge-cs-cpp", std::move(rest));
        case Kind::PySource: return one("judge-py-cpp", std::move(rest));
        case Kind::Le:
            if (in.size() == 2 && probe(in[1]) == Kind::Le) return one("driver-judge", std::move(rest));
            break;
        default: break;
    }
    return one("measure", std::move(rest));
}

std::optional<Route> route_lookup(std::span<const std::string> args, const Probe& probe) {
    std::vector<std::string> rest(args.begin(), args.end());
    for (const Mode& m : kModes) {
        if (m.verb != "lookup" || m.flag.empty() || !has(rest, m.flag)) continue;
        rest = without(rest, m.flag);
        return one(std::string(m.old_name), std::move(rest));
    }
    const std::vector<std::string> in = inputs_of(args);
    if (in.empty()) return one("knowledge", std::move(rest));
    const std::string& first = in.front();
    if (first.starts_with('?') || first.find("@@") != std::string::npos) return one("demangle", std::move(rest));
    const std::string l = lower(first);
    if (l.starts_with("0x") || (!l.empty() && l[0] >= '0' && l[0] <= '9')) return one("dos-interrupts", std::move(rest));
    if (probe(first) == Kind::Le) return one("driver-services", std::move(rest));
    return one("knowledge", std::move(rest)); // platform [kind [key]]
}

// corpus and xbox: the first word picks
std::optional<Route> route_word(std::string_view verb, std::span<const std::string> args, std::string_view when_empty) {
    if (args.empty()) {
        if (when_empty.empty()) return std::nullopt;
        return one(std::string(when_empty), {});
    }
    for (const Mode& m : kModes)
        if (m.verb == verb && !m.flag.empty() && m.flag == args[0]) return one(std::string(m.old_name), std::vector<std::string>(args.begin() + 1, args.end()));
    std::string words;
    for (const Mode& m : kModes)
        if (m.verb == verb && !m.flag.empty()) words += (words.empty() ? "" : ", ") + std::string(m.flag);
    throw UsageError(std::format("{}: '{}' is not one of: {}", verb, args[0], words));
}

// A folder's kind from what is in it (the top level only).
Kind classify_folder(const stdfs::path& dir) {
    std::error_code ec;
    bool asm_found = false, driver = false, c = false;
    int seen = 0;
    for (stdfs::directory_iterator it(dir, ec), end; !ec && it != end && seen < 400; it.increment(ec), ++seen) {
        std::error_code e2;
        if (!it->is_regular_file(e2)) continue;
        const std::string name = fs::to_text(it->path().filename());
        if (ends_with_any(name, {".c", ".def", ".mak"}) || lower(name) == "makefile") c = true;
        if (ends_with_any(name, {".asm"}) && !asm_found) {
            asm_found = true;
            try {
                Bytes data = fs::read_file(it->path());
                if (data.size() > 65536) data.resize(65536);
                const std::string body = lower(std::string_view(reinterpret_cast<const char*>(data.data()), data.size()));
                driver = body.find("declare_virtual_device") != std::string::npos || body.find("vxd") != std::string::npos;
            } catch (const std::exception&) {
            }
        }
    }
    if (asm_found) return driver ? Kind::DriverSource : Kind::DosSource;
    return c ? Kind::CSource : Kind::Folder;
}

Kind probe_file(const std::string& given) {
    const stdfs::path path = fs::from_text(given);
    std::error_code ec;
    if (stdfs::is_directory(path, ec)) return classify_folder(path);
    if (!stdfs::is_regular_file(path, ec)) return Kind::Missing;
    const std::string name = fs::to_text(path.filename());
    if (ends_with_any(name, {".toml"})) return Kind::Config;
    if (ends_with_any(name, {".java"})) return Kind::JavaSource;
    if (ends_with_any(name, {".cs"})) return Kind::CsSource;
    if (ends_with_any(name, {".py"})) return Kind::PySource;
    if (ends_with_any(name, {".ho", ".hip"})) return Kind::HeavyIron;
    if (ends_with_any(name, {".gfx"})) return Kind::Gfx;
    if (ends_with_any(name, {".fon", ".fnt"})) return Kind::Font;
    try {
        if (const auto lane = formats::detect::kind(path)) {
            switch (*lane) {
                case formats::detect::Lane::Pe: return Kind::Pe;
                case formats::detect::Lane::Ne: return Kind::Ne;
                case formats::detect::Lane::Le: return Kind::Le;
                case formats::detect::Lane::Dos: return Kind::Dos;
            }
        }
        for (const formats::detect::Detected& d : formats::detect::identify(path)) {
            if (d.format.find("Xbox 360 executable") != std::string::npos) return Kind::Xex;
            if (d.format.find("Microsoft cabinet") != std::string::npos) return Kind::Cab;
            if (d.format.find("XDVDFS") != std::string::npos) return Kind::XboxDisc;
            if (d.command == "media") return Kind::Disc;
        }
    } catch (const std::exception&) {
    }
    return Kind::Other;
}

}  // namespace

Probe file_probe() { return probe_file; }
std::span<const Mode> modes() { return kModes; }
std::span<const std::string_view> verb_names() { return kVerbs; }
std::span<const std::string_view> kept_names() { return kKept; }
bool is_verb(std::string_view name) { return std::find(kVerbs.begin(), kVerbs.end(), name) != kVerbs.end(); }

const Mode* alias_of(std::string_view old_name) {
    for (const Mode& m : kModes)
        if (m.old_name == old_name && !is_verb(old_name)) return &m;
    return nullptr;
}

std::string_view verb_of(std::string_view old_name) {
    const Mode* m = alias_of(old_name);
    return m ? m->verb : std::string_view{};
}

std::optional<Route> route(std::string_view verb, std::span<const std::string> args, const Probe& probe) {
    if (verb == "convert") return route_convert(args, probe);
    if (verb == "inspect") return route_inspect(args, probe);
    if (verb == "extract") return route_extract(args, probe);
    if (verb == "build") return route_build(args, probe);
    if (verb == "check") return route_check(args, probe);
    if (verb == "compare") return args.empty() ? std::nullopt : std::optional<Route>(one("versions", std::vector<std::string>(args.begin(), args.end())));
    if (verb == "corpus") return route_word("corpus", args, "coverage");
    if (verb == "lookup") return route_lookup(args, probe);
    if (verb == "xbox") return route_word("xbox", args, {});
    if (verb == "cache") {
        for (const Mode& m : kModes)
            if (m.verb == "cache" && !args.empty() && args[0] == m.flag) return one(std::string(m.old_name), std::vector<std::string>(args.begin() + 1, args.end()));
        return one("cache", std::vector<std::string>(args.begin(), args.end()));
    }
    return std::nullopt;
}

// ---- help

namespace {

struct Picks {
    std::string_view verb, text;
};
constexpr Picks kPicks[] = {
    {"convert",
     "FILE is a Windows, DOS or driver program, a disc image, a cabinet, an Xbox 360 game or a source file: convert looks at what it is and takes the\n"
     "steps itself, showing each stage as it goes. Name a step to take just that one:\n"
     "  --to LANG        also translate the source into another language (cpp17, csharp ...), proved by running both\n"
     "  --asm            the program as assembly source, built again and compared (Windows, DOS and driver files)\n"
     "  --check          build the result and judge it (an Xbox 360 game: also compile it)\n"},
    {"inspect",
     "With a file, says what it is and what it holds (headers, imports, exports, resources); with a folder, what each program in it is. One flag\n"
     "asks for one thing in particular, as its option below."},
    {"extract",
     "Looks at FILE and takes the files out: a cabinet, a floppy, hard-disk or CD image, an Xbox 360 package or disc, a game archive, a menu, a\n"
     "font, an Xbox 360 program. `extract FILE OUT` writes them to OUT; `extract FILE --list` only lists them."},
    {"build",
     "FOLDER is a source folder; build looks at what is in it: a driver's assembly, a DOS program's assembly, or a 16-bit DLL's C. A .toml file is a\n"
     "recompiler configuration. A module name or a file is the whole-module build, as before. Name the kind to be sure."},
    {"check",
     "Proves rebuilt source behaves like the original by running both. A .java, .cs or .py file is run against its C++ translation; two driver files\n"
     "are compared entry point by entry point; anything else is a module, reconstructed and judged. `--bytes` asks the stricter, optional question."},
    {"compare",
     "Two builds of a program, function by function: identical, changed, added, removed. `compare A B` takes two files; `compare MODULE --from X --to Y`\n"
     "takes one module of two products; `--all` and `--shared` take many."},
    {"corpus", "The first word says what to do with the whole collection. With no word it shows how much of every module is accounted for."},
    {"lookup",
     "Looks the name up itself: a decorated C++ name (?foo@@YAXH@Z) is read back as its declaration, a number is a DOS or BIOS interrupt, a driver\n"
     "file lists the ring-0 services it calls, anything else is searched for in the knowledge base (platform [kind [key]])."},
    {"xbox", "The first word says which tool: edit, patch or re-sign an Xbox 360 program, console crypto, a recompiler configuration, XDK library names."},
};

std::string indent_wrap(std::string_view text, std::size_t indent, std::size_t width) {
    std::string out, line(indent, ' ');
    bool first = true;
    for (std::string_view w : text::split_ws(text)) {
        if (!first && line.size() + 1 + w.size() > width) {
            out += line + "\n";
            line.assign(indent, ' ');
            first = true;
        }
        if (!first) line += ' ';
        line += w;
        first = false;
    }
    if (!first) out += line + "\n";
    return out;
}

}  // namespace

std::string help(std::string_view verb) {
    const CommandDoc* doc = find_command_doc(verb);
    if (!is_verb(verb) || doc == nullptr) return {};
    std::string out = std::format("usage: codebreaker {}\n\n", doc->synopsis);
    out += indent_wrap(doc->summary, 0, 100) + "\n";
    for (const Picks& p : kPicks)
        if (p.verb == verb) out += std::string(p.text) + "\n";
    const bool words = word_flag(verb);
    out += words ? "\nwhat to do:\n" : "\noptions:\n";
    std::vector<const Mode*> shown;
    for (const Mode& m : kModes) {
        if (m.verb != verb || m.flag.empty()) continue;
        if (std::any_of(shown.begin(), shown.end(), [&](const Mode* s) { return s->flag == m.flag; })) continue;
        shown.push_back(&m);
    }
    for (const Mode* m : shown) {
        const CommandDoc* old = find_command_doc(m->old_name);
        std::string flag = "  " + std::string(m->flag);
        flag.resize(std::max<std::size_t>(flag.size() + 2, 20), ' ');
        std::string_view what = old ? old->summary : std::string_view{};
        if (is_verb(m->old_name)) what = "the whole-module build: a corpus name or a file, built from its recovered source and checked";
        std::string line = indent_wrap(what, flag.size(), 118);
        if (line.empty()) line = std::string(flag.size(), ' ') + "\n";
        out += flag + line.substr(flag.size());
    }
    out += "\nOld names (they still work, and print what they always did):\n";
    for (const Mode& m : kModes) {
        if (m.verb != verb || is_verb(m.old_name)) continue;
        std::string line = std::format("  {:<18} = {} ", m.old_name, verb);
        line += words ? std::string(m.flag) : "FILE";
        if (!words && !m.flag.empty()) line += " " + std::string(m.flag);
        out += line + "\n";
    }
    if (verb == "extract") out += "  (and `--list` for the listing form of each)\n";
    return out;
}

void register_verbs(CommandTable& table, Probe probe) {
    std::vector<std::string> names(verb_names().begin(), verb_names().end());
    names.push_back("cache");
    for (const std::string& verb : names) {
        // `original`: the command of the same name that was there before (null when the verb is new)
        auto go = [&table, verb, probe](const RawRunner* original, std::span<const std::string> args) -> int {
            if (original && args.empty() && verb != "cache") return (*original)(args); // `convert` alone says what it always said
            const std::optional<Route> picked = route(verb, args, probe);
            if (!picked) {
                err_text(help(verb));
                return kExitUsage;
            }
            int code = 0;
            for (const Step& step : *picked) {
                code = (step.command == verb && original) ? (*original)(step.args) : table.invoke(step.command, step.args);
                if (code != 0) return code;
            }
            return code;
        };
        const bool wrapped = table.intercept(verb, [go](const RawRunner& original, std::span<const std::string> args) { return go(&original, args); });
        if (!wrapped) table.set(verb, [go](std::span<const std::string> args) { return go(nullptr, args); });
    }
}

}  // namespace cb::app::verbs
