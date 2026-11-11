#include "cb/build/flat_thunk.hpp"

#include <algorithm>
#include <chrono>
#include <format>
#include <iterator>

#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/process.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/detect.hpp"
#include "cb/formats/omf.hpp"

namespace cb::build::flat_thunk {
namespace stdfs = std::filesystem;

namespace {

bool is_alnum(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'); }
bool is_word(char c) { return is_alnum(c) || c == '_'; }
bool is_letter(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }

// Windows text, as the period tools read it and the earlier recoveries have it: every line ends CR LF.
void write_windows_text(const stdfs::path& file, std::string_view text) {
    std::string out;
    out.reserve(text.size() + text.size() / 16);
    for (const char c : text) {
        if (c == '\n') out += '\r';
        out += c;
    }
    fs::write_text(file, out);
}

std::string lowered(const stdfs::path& p) { return text::lower(fs::to_text(p.filename())); }

// The files of a folder with one of the extensions (lower case, with the dot), by name as NTFS lists them.
std::vector<stdfs::path> files_with(const stdfs::path& folder, std::initializer_list<std::string_view> extensions) {
    std::vector<stdfs::path> found;
    std::error_code ec;
    for (const auto& entry : stdfs::directory_iterator(folder, ec)) {
        if (!entry.is_regular_file(ec)) continue;
        const std::string ext = text::lower(fs::to_text(entry.path().extension()));
        if (std::find(extensions.begin(), extensions.end(), ext) != extensions.end()) found.push_back(entry.path());
    }
    std::sort(found.begin(), found.end(), [](const stdfs::path& a, const stdfs::path& b) { return lowered(a) < lowered(b); });
    return found;
}

std::string joined_list(const std::vector<std::string>& items) { return "[" + text::join(items, ", ") + "]"; }

}  // namespace

// ---- the binary model ---------------------------------------------------------------------------------------------

PeModule PeModule::open(const stdfs::path& file) { return PeModule(formats::pe::Image::open(file)); }

std::vector<PeModule::NamedExport> PeModule::exports() const {
    std::vector<NamedExport> out;
    for (const formats::pe::ExportSymbol& s : image_.export_table().symbols) {
        if (s.name && !s.name->empty()) out.push_back({s.ordinal, *s.name});
    }
    std::sort(out.begin(), out.end());
    return out;
}

OrderedMap<std::string, std::vector<std::string>> PeModule::imports() const {
    OrderedMap<std::string, std::vector<std::string>> out;
    for (const formats::pe::ImportModule& m : image_.import_table(false)) {
        std::vector<std::string> names;
        for (const formats::pe::ImportSymbol& s : m.symbols) {
            names.push_back(s.name && !s.name->empty() ? *s.name : "@" + std::to_string(s.ordinal.value_or(0)));
        }
        out.insert_or_assign(m.module, std::move(names));
    }
    return out;
}

std::map<std::string, std::uint32_t> PeModule::export_addresses() const {
    struct Row {
        std::uint32_t hint;
        std::string name;
        std::uint32_t rva;
    };
    std::vector<Row> rows;
    for (const formats::pe::ExportSymbol& s : image_.export_table().symbols) {
        if (s.name && !s.name->empty()) rows.push_back({s.hint.value_or(0), *s.name, s.rva});
    }
    std::sort(rows.begin(), rows.end(), [](const Row& a, const Row& b) {
        return std::tie(a.hint, a.name, a.rva) < std::tie(b.hint, b.name, b.rva);
    });
    std::map<std::string, std::uint32_t> out;
    for (const Row& r : rows) out[r.name] = r.rva;
    return out;
}

ByteView PeModule::read_at(std::uint32_t rva, std::size_t n) const {
    const auto offset = image_.file_offset(rva);
    const ByteView all = image_.data();
    if (!offset || *offset >= all.size()) return {};
    return all.subspan(static_cast<std::size_t>(*offset), std::min<std::size_t>(n, all.size() - static_cast<std::size_t>(*offset)));
}

NeModule NeModule::open(const stdfs::path& file) { return NeModule(formats::ne::Module::open_file(file)); }

std::vector<SegmentKind> NeModule::segment_kinds() const {
    std::vector<SegmentKind> out;
    for (const formats::ne::Segment& s : module_.segments()) out.push_back(s.is_data() ? SegmentKind::Data : SegmentKind::Code);
    return out;
}

std::vector<std::pair<std::uint16_t, std::string>> NeModule::resident_names() const {
    std::vector<std::pair<std::uint16_t, std::string>> out;
    for (const formats::ne::Name& n : module_.resident_names()) out.emplace_back(n.ordinal, n.name);
    return out;
}

std::optional<Binary> load(const stdfs::path& file) {
    const std::optional<std::string> format = formats::detect::format_of(file);
    if (format == "PE") return Binary(PeModule::open(file));
    if (format == "NE") return Binary(NeModule::open(file));
    return std::nullopt;
}

// ---- classify -----------------------------------------------------------------------------------------------------

std::string_view format_name(Format f) {
    switch (f) {
        case Format::Ne: return "NE";
        case Format::Pe: return "PE";
        case Format::Unknown: break;
    }
    return "?";
}

std::string_view subtype_name(Subtype s) {
    switch (s) {
        case Subtype::Win16C: return "win16-c";
        case Subtype::FlatThunk: return "flat-thunk";
        case Subtype::Win32C: return "win32-c";
        case Subtype::Unknown: break;
    }
    return "unknown";
}

namespace {

// The KERNEL32 symbols of an import table, whatever the case of the library's name.
const std::vector<std::string>& kernel32_symbols(const OrderedMap<std::string, std::vector<std::string>>& imports) {
    static const std::vector<std::string> kNone;
    for (const auto& [dll, symbols] : imports) {
        if (text::iequals(dll, "KERNEL32.DLL") && !symbols.empty()) return symbols;
    }
    return kNone;
}

}  // namespace

Classification classify(const stdfs::path& file) {
    const std::optional<Binary> binary = load(file);
    if (!binary) return {Format::Unknown, Subtype::Unknown, std::nullopt, std::nullopt, std::nullopt, "not MZ/PE/NE"};
    if (const NeModule* ne = std::get_if<NeModule>(&*binary)) {
        return {Format::Ne, Subtype::Win16C, "msc6-dosbox", std::nullopt, std::nullopt,
                std::format("16-bit NE; {} segments", ne->segment_count())};
    }
    // a PE: flat-thunk veneer, or 32-bit C logic
    const PeModule& pe = std::get<PeModule>(*binary);
    const auto imports = pe.imports();
    const std::vector<std::string>& k32 = kernel32_symbols(imports);
    std::set<std::string> markers;
    for (const std::string& symbol : k32) {
        for (const std::string_view marker : kThunkMarkers) {
            if (symbol.find(marker) != std::string::npos) markers.emplace(marker);
        }
    }
    const std::uint64_t exports = pe.exports().size();
    if (!markers.empty()) {
        std::uint64_t imported = 0;
        for (const auto& [dll, symbols] : imports) imported += symbols.size();
        return {Format::Pe, Subtype::FlatThunk, "thunk-ml-cl-link", exports, imported,
                "imports flat-thunk helpers (" + text::join(markers, ",") + ")"};
    }
    std::uint32_t text_size = 0;
    for (const formats::pe::Section& s : pe.image().sections()) {
        if (s.name == ".text") {
            text_size = s.virtual_size;
            break;
        }
    }
    return {Format::Pe, Subtype::Win32C, "msvc-native", exports, std::nullopt,
            std::format("32-bit PE, no flat-thunk markers; .text={} B", text_size)};
}

// ---- diff ---------------------------------------------------------------------------------------------------------

std::string_view verdict_name(Verdict v) {
    switch (v) {
        case Verdict::ByteSurfaceIdentical: return "byte-surface-identical";
        case Verdict::StructuralParity: return "structural-parity";
        case Verdict::Partial: break;
    }
    return "partial";
}

namespace {

std::vector<std::string> section_names(const PeModule& pe) {
    std::vector<std::string> out;
    for (const formats::pe::Section& s : pe.image().sections()) out.push_back(s.name);
    return out;
}

std::set<std::string> distinct_symbols(const OrderedMap<std::string, std::vector<std::string>>& imports) {
    std::set<std::string> out;
    for (const auto& [dll, symbols] : imports) {
        for (const std::string& s : symbols) out.insert(dll + "!" + s);
    }
    return out;
}

std::vector<std::string> sorted_keys(const OrderedMap<std::string, std::vector<std::string>>& imports) {
    std::vector<std::string> out;
    for (const auto& [dll, symbols] : imports) out.push_back(dll);
    std::sort(out.begin(), out.end());
    return out;
}

std::size_t distinct_ordinals(const std::vector<PeModule::NamedExport>& exports) {
    std::set<std::uint64_t> ordinals;
    for (const auto& e : exports) ordinals.insert(e.ordinal);
    return ordinals.size();
}

std::vector<std::string> only_in(const std::set<std::string>& a, const std::set<std::string>& b) {
    std::vector<std::string> out;
    std::set_difference(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(out));
    return out;
}

PeDiff diff_pe(const PeModule& built, const PeModule& shipped) {
    PeDiff d;
    d.entry = {built.entry_rva(), shipped.entry_rva()};
    d.entry_match = d.entry.first == d.entry.second;
    d.sections_built = section_names(built);
    d.sections_shipped = section_names(shipped);
    d.sections_match = d.sections_built == d.sections_shipped;
    const auto exports_built = built.exports();
    const auto exports_shipped = shipped.exports();
    d.exports_match = exports_built == exports_shipped;
    d.exports = {distinct_ordinals(exports_built), distinct_ordinals(exports_shipped)};
    const auto imports_built = built.imports();
    const auto imports_shipped = shipped.imports();
    d.import_dlls_built = sorted_keys(imports_built);
    d.import_dlls_shipped = sorted_keys(imports_shipped);
    const auto symbols_built = distinct_symbols(imports_built);
    const auto symbols_shipped = distinct_symbols(imports_shipped);
    d.imports_only_in_shipped = only_in(symbols_shipped, symbols_built);
    d.imports_only_in_built = only_in(symbols_built, symbols_shipped);
    d.import_count = {symbols_built.size(), symbols_shipped.size()};
    return d;
}

NeDiff diff_ne(const NeModule& built, const NeModule& shipped) {
    NeDiff d;
    d.segments = {built.segment_count(), shipped.segment_count()};
    d.kinds_built = built.segment_kinds();
    d.kinds_shipped = shipped.segment_kinds();
    d.kinds_match = d.kinds_built == d.kinds_shipped;
    auto names_built = built.resident_names();
    auto names_shipped = shipped.resident_names();
    std::sort(names_built.begin(), names_built.end());
    std::sort(names_shipped.begin(), names_shipped.end());
    d.exports_match = names_built == names_shipped;
    return d;
}

std::uint64_t size_of(const Binary& b) {
    return std::visit([](const auto& m) -> std::uint64_t {
        if constexpr (std::is_same_v<std::decay_t<decltype(m)>, PeModule>) return m.image().data().size();
        else return m.module().data().size();
    }, b);
}

}  // namespace

Result<BinDiff> bindiff(const stdfs::path& built, const stdfs::path& shipped) {
    const std::optional<Binary> b = load(built);
    const std::optional<Binary> s = load(shipped);
    if (!b || !s) return UsageError("could not parse one of the inputs");
    BinDiff r;
    r.built = fs::to_text(built.filename());
    r.shipped = fs::to_text(shipped.filename());
    r.size_built = size_of(*b);
    r.size_shipped = size_of(*s);
    if (const PeModule* pb = std::get_if<PeModule>(&*b); pb && std::holds_alternative<PeModule>(*s)) {
        PeDiff d = diff_pe(*pb, std::get<PeModule>(*s));
        const bool structural = d.entry_match && d.sections_match && d.exports_match;
        const bool exact = structural && d.imports_only_in_shipped.empty() && d.imports_only_in_built.empty();
        r.verdict = exact ? Verdict::ByteSurfaceIdentical : structural ? Verdict::StructuralParity : Verdict::Partial;
        r.detail = std::move(d);
    } else if (const NeModule* nb = std::get_if<NeModule>(&*b); nb && std::holds_alternative<NeModule>(*s)) {
        NeDiff d = diff_ne(*nb, std::get<NeModule>(*s));
        r.verdict = d.kinds_match && d.exports_match ? Verdict::StructuralParity : Verdict::Partial;
        r.detail = std::move(d);
    } else {
        return UsageError(std::format("format mismatch: built={} shipped={}", std::holds_alternative<PeModule>(*b) ? "PE" : "NE",
                                      std::holds_alternative<PeModule>(*s) ? "PE" : "NE"));
    }
    return r;
}

std::string format_diff(const BinDiff& r) {
    const auto yes = [](bool b) { return b ? "MATCH" : "differ"; };
    const auto kinds = [](const std::vector<SegmentKind>& k) {
        std::vector<std::string> names;
        for (const SegmentKind x : k) names.push_back(x == SegmentKind::Data ? "DATA" : "CODE");
        return joined_list(names);
    };
    std::string out;
    const bool is_pe = std::holds_alternative<PeDiff>(r.detail);
    out += std::format("  kind={}  size built={} shipped={}  verdict={}\n", is_pe ? "PE" : "NE", r.size_built, r.size_shipped,
                       verdict_name(r.verdict));
    if (const PeDiff* d = std::get_if<PeDiff>(&r.detail)) {
        out += std::format("  entry 0x{:x} / 0x{:x} {}\n", d->entry.first, d->entry.second, yes(d->entry_match));
        out += std::format("  sections {}", yes(d->sections_match));
        if (!d->sections_match) out += std::format(" ({} vs {})", joined_list(d->sections_built), joined_list(d->sections_shipped));
        out += "\n";
        out += std::format("  exports {} / {} {}\n", d->exports.first, d->exports.second, yes(d->exports_match));
        out += std::format("  imports {} / {}  only-in-shipped={}  only-in-built={}\n", d->import_count.first, d->import_count.second,
                           joined_list(d->imports_only_in_shipped), joined_list(d->imports_only_in_built));
    } else if (const NeDiff* n = std::get_if<NeDiff>(&r.detail)) {
        out += std::format("  segments {} / {}  kinds {}  exports {}\n", n->segments.first, n->segments.second, yes(n->kinds_match),
                           yes(n->exports_match));
        if (!n->kinds_match) out += std::format("  segment kinds {} vs {}\n", kinds(n->kinds_built), kinds(n->kinds_shipped));
    }
    return out;
}

// ---- toolchains ---------------------------------------------------------------------------------------------------

Toolchain toolchain(const Locations& where) {
    std::vector<stdfs::path> roots;
    for (const stdfs::path& r : where.reference_roots) {
        std::error_code ec;
        if (!r.empty() && stdfs::is_directory(r, ec) && std::find(roots.begin(), roots.end(), r) == roots.end()) roots.push_back(r);
    }
    const auto ref_find = [&](std::initializer_list<std::string_view> relative) -> stdfs::path {
        for (const stdfs::path& root : roots) {
            stdfs::path p = root;
            for (const std::string_view part : relative) p /= fs::from_text(part);
            std::error_code ec;
            if (stdfs::exists(p, ec)) return p;
        }
        return {};
    };

    // the setting names the BIN folder of Visual C++ 2.0 (or its root)
    stdfs::path msvc2 = where.period_compiler;
    if (!msvc2.empty() && !msvc2.has_filename()) msvc2 = msvc2.parent_path();   // a trailing separator
    if (text::iequals(fs::to_text(msvc2.filename()), "BIN")) msvc2 = msvc2.parent_path();
    Toolchain tc;
    const stdfs::path win32lib = ref_find({"WIN32SDK", "MSTOOLS", "LIB", "I386", "THUNK32.LIB"});
    tc.thunk = ref_find({"thunk-1.01", "THUNK.EXE"});
    if (tc.thunk.empty()) tc.thunk = ref_find({"WIN32SDK", "MSTOOLS", "BIN", "I386", "THUNK.EXE"});
    tc.ml = ref_find({"Microsoft Windows 95 DDK", "MASM611C", "ML.EXE"});
    tc.cl = msvc2 / "BIN" / "CL.EXE";
    tc.link = msvc2 / "BIN" / "LINK.EXE";
    tc.lib = msvc2 / "BIN" / "LIB.EXE";
    tc.rc = msvc2 / "BIN" / "RC.EXE";
    tc.include = msvc2 / "INCLUDE";
    tc.lib_dirs = {msvc2 / "LIB"};
    if (!win32lib.empty()) tc.lib_dirs.push_back(win32lib.parent_path());
    return tc;
}

// ---- build --------------------------------------------------------------------------------------------------------

Result<Discovery> discover(const stdfs::path& srcdir) {
    const std::vector<stdfs::path> thks = files_with(srcdir, {".thk"});
    const std::vector<stdfs::path> defs = files_with(srcdir, {".def"});
    const std::vector<stdfs::path> cfiles = files_with(srcdir, {".c"});
    const std::vector<stdfs::path> rcs = files_with(srcdir, {".rc"});
    std::vector<stdfs::path> k32, moddefs;
    for (const stdfs::path& d : defs) (text::iends_with(lowered(d), "k32.def") ? k32 : moddefs).push_back(d);
    const std::string where = fs::to_text(srcdir);
    if (thks.empty()) return UsageError("no .thk in " + where);
    if (moddefs.empty()) return UsageError("no module .def in " + where);
    if (k32.empty()) return UsageError("no *k32.def (KERNEL32 ordinal import lib) in " + where);
    if (cfiles.empty()) return UsageError("no veneer .c in " + where);

    // the module is the LIBRARY the module .def names (else the .def's own name)
    std::string module = text::lower(fs::to_text(moddefs[0].stem()));
    const std::string def_text = text::from_latin1(fs::read_file(moddefs[0]));
    for (const std::string_view line : text::lines(def_text)) {
        const std::string_view t = text::ltrim(line);
        if (t.size() > 7 && text::iequals(t.substr(0, 7), "LIBRARY") && (t[7] == ' ' || t[7] == '\t')) {
            const std::string_view rest = text::ltrim(t.substr(7));
            std::size_t n = 0;
            while (n < rest.size() && is_word(rest[n])) ++n;
            if (n > 0) {
                module = text::lower(rest.substr(0, n));
                break;
            }
        }
    }
    Discovery found;
    found.thk = thks[0];
    found.cfiles = cfiles;
    found.moddef = moddefs[0];
    found.k32def = k32[0];
    if (!rcs.empty()) found.rc = rcs[0];
    found.module = module;
    return found;
}

namespace {

constexpr auto kToolTimeout = std::chrono::seconds(300);

// Run a tool with its output (stdout and stderr, in order) appended to `log`; its exit code.
std::uint32_t run_logged(const std::vector<std::string>& argv, const stdfs::path& cwd, const std::map<std::string, std::string>& env,
                         std::string& log, std::chrono::seconds timeout = kToolTimeout) {
    process::ProcessSpec spec;
    spec.argv = argv;
    spec.cwd = cwd;
    spec.env = env;
    spec.err = process::Capture::Merge;
    spec.timeout = timeout;
    const process::ProcessResult r = process::run(spec);
    log += text::from_latin1(r.out);
    if (r.timed_out) log += "(timed out)\n";
    return r.exit_code;
}

std::string first_line_with(std::string_view output, std::string_view needle) {
    for (const std::string_view line : text::lines(output)) {
        if (line.find(needle) != std::string_view::npos) return std::string(line);
    }
    return {};
}

std::string stripped_prefix(std::string_view s, std::size_t n) {
    const std::string_view t = text::trim(s);
    return std::string(t.substr(0, n));
}

}  // namespace

BuildResult build_period(const Locations& where, const stdfs::path& srcdir_in, const std::optional<stdfs::path>& outdir_in) {
    BuildResult result;
    const stdfs::path srcdir = stdfs::absolute(srcdir_in);
    const stdfs::path outdir = stdfs::absolute(outdir_in ? *outdir_in : srcdir / "_build");
    fs::make_dirs(outdir);
    const Toolchain tc = toolchain(where);
    const std::pair<const char*, const stdfs::path*> needed[] = {{"thunk", &tc.thunk}, {"ml", &tc.ml}, {"cl", &tc.cl}, {"link", &tc.link}, {"lib", &tc.lib}};
    for (const auto& [name, path] : needed) {
        std::error_code ec;
        if (path->empty() || !stdfs::exists(*path, ec)) {
            result.error = std::format("missing tool: {} ({})", name, fs::to_text(*path));
            result.log = result.error;
            return result;
        }
    }
    const Result<Discovery> found = discover(srcdir);
    if (!found.ok()) {
        result.error = found.error().what();
        result.log = result.error;
        return result;
    }
    const Discovery& disc = found.value();
    result.module = disc.module;

    // stage the sources into outdir (the tracked tree stays clean)
    for (const stdfs::path& f : files_with(srcdir, {".thk", ".c", ".h", ".def", ".rc", ".ico", ".res"})) {
        const stdfs::path to = outdir / f.filename();
        if (!fs::same_path(f, to)) stdfs::copy_file(f, to, stdfs::copy_options::overwrite_existing);
    }
    const std::string thk = fs::to_text(disc.thk.filename());
    const std::string thk_stem = fs::to_text(disc.thk.stem());
    const std::string k32 = fs::to_text(disc.k32def.filename());
    const std::string k32_stem = fs::to_text(disc.k32def.stem());
    const std::string moddef = fs::to_text(disc.moddef.filename());
    const std::string res = disc.rc ? disc.module + ".res" : std::string();

    // INCLUDE: the Win32 SDK headers (msacm.h, vfw.h, commctrl.h ...) as well as the compiler's own, so a veneer's real
    // SDK-typed signatures resolve -- the authentic build configuration
    std::vector<std::string> include;
    for (const stdfs::path& root : where.reference_roots) {
        const stdfs::path sdk = root / "WIN32SDK" / "MSTOOLS" / "INCLUDE";
        std::error_code ec;
        if (stdfs::is_directory(sdk, ec)) {
            include.push_back(fs::to_text(sdk));
            break;
        }
    }
    include.push_back(fs::to_text(tc.include));
    std::vector<std::string> libs;
    for (const stdfs::path& l : tc.lib_dirs) libs.push_back(fs::to_text(l));
    const std::map<std::string, std::string> env = {
        {"INCLUDE", text::join(include, ";")},
        {"LIB", text::join(libs, ";")},
        {"PATH", fs::to_text(tc.cl.parent_path()) + ";" + fs::get_env("PATH").value_or("")}};

    // start from nothing: the previous outputs go
    std::error_code ec;
    stdfs::remove(outdir / (disc.module + ".dll"), ec);
    for (const stdfs::path& f : files_with(outdir, {".obj"})) stdfs::remove(f, ec);
    stdfs::remove(outdir / (thk_stem + ".asm"), ec);
    if (!res.empty()) stdfs::remove(outdir / res, ec);

    std::string log;
    try {
        run_logged({fs::to_text(tc.thunk), "-o", thk_stem + ".asm", thk}, outdir, env, log);
        run_logged({fs::to_text(tc.ml), "/c", "/W0", "/DIS_32", "/Fo" + thk_stem + "_thk.obj", thk_stem + ".asm"}, outdir, env, log);
        std::vector<std::string> objects;
        for (const stdfs::path& c : disc.cfiles) {
            // /DWIN32 /D_WIN32: the SDK headers gate their 32-bit types on _WIN32 (msacm.h)
            run_logged({fs::to_text(tc.cl), "/c", "/W3", "/DWIN32", "/D_WIN32", fs::to_text(c.filename())}, outdir, env, log);
            objects.push_back(fs::to_text(c.stem()) + ".obj");
        }
        if (disc.rc) {
            run_logged({fs::to_text(tc.rc), "/r", "/fo", res, fs::to_text(disc.rc->filename())}, outdir, env, log);
        }
        run_logged({fs::to_text(tc.lib), "/NOLOGO", "/DEF:" + k32, "/OUT:" + k32_stem + ".lib", "/MACHINE:IX86"}, outdir, env, log);
        std::vector<std::string> link = {fs::to_text(tc.link), "/DLL", "/NOLOGO", "/NODEFAULTLIB", "/ENTRY:DllMain@12", "/DEF:" + moddef,
                                         "/OUT:" + disc.module + ".dll"};
        link.insert(link.end(), objects.begin(), objects.end());
        link.push_back(thk_stem + "_thk.obj");
        if (!res.empty()) link.push_back(res);
        link.insert(link.end(), {k32_stem + ".lib", "kernel32.lib", "thunk32.lib"});
        const std::uint32_t rc = run_logged(link, outdir, env, log);
        log += std::format("RC={}\n", rc);
    } catch (const ToolError& e) {   // a tool that cannot start ends the build with what it did say
        result.error = std::string("build run failed: ") + e.what();
        result.log = log + result.error;
        return result;
    }
    fs::write_text(outdir / "_t.log", log);
    result.log = log;
    const stdfs::path dll = outdir / (disc.module + ".dll");
    result.ok = stdfs::exists(dll, ec);
    if (result.ok) {
        result.dll = dll;
        result.bytes = fs::file_size(dll);
    } else {
        result.error = "link produced no DLL";
    }
    return result;
}

// ---- OpenWatcom ---------------------------------------------------------------------------------------------------

namespace {

struct OpenWatcomTools {
    stdfs::path wcc386, wlink;
    std::map<std::string, std::string> env;
};

// nullopt when OpenWatcom is not built.
std::optional<OpenWatcomTools> openwatcom_tools(const watcom::OpenWatcom& ow) {
    OpenWatcomTools t;
    t.wcc386 = ow.cc(watcom::Target::Win32);
    t.wlink = ow.tool("wlink");
    if (t.wcc386.empty()) return std::nullopt;
    const stdfs::path root = ow.root();
    t.env["WATCOM"] = fs::to_text(root);
    t.env["INCLUDE"] = fs::to_text(root / "h" / "nt") + ";" + fs::to_text(root / "h");
    t.env["PATH"] = fs::to_text(t.wcc386.parent_path()) + ";" + fs::get_env("PATH").value_or("");
    return t;
}

constexpr std::string_view kOpenWatcomMissing = "OpenWatcom not built";

// The text of a veneer as the build copies it: LF line ends, damaged bytes replaced.
std::string veneer_text(const stdfs::path& file) {
    const Bytes bytes = fs::read_file(file);
    const std::string raw = text::sanitize_utf8(as_text(bytes));
    std::string out;
    out.reserve(raw.size());
    for (std::size_t i = 0; i < raw.size(); ++i) {
        if (raw[i] == '\r') {
            out += '\n';
            if (i + 1 < raw.size() && raw[i + 1] == '\n') ++i;
        } else {
            out += raw[i];
        }
    }
    return out;
}

}  // namespace

CompileResult compile_openwatcom(const watcom::OpenWatcom& ow, const stdfs::path& srcdir) {
    CompileResult result;
    const auto tools = openwatcom_tools(ow);
    if (!tools) {
        result.error = std::string(kOpenWatcomMissing);
        return result;
    }
    result.ok = true;
    for (const stdfs::path& c : files_with(srcdir, {".c"})) {
        std::string log;
        const std::uint32_t rc =
            run_logged({fs::to_text(tools->wcc386), "-bt=nt", "-zq", "-w0", fs::to_text(c.filename())}, srcdir, tools->env, log, std::chrono::seconds(180));
        result.files.push_back({fs::to_text(c.filename()), rc, stripped_prefix(first_line_with(log, "Error"), 100)});
        if (rc != 0) result.ok = false;
    }
    return result;
}

namespace {

// The clean name of an export -> the public symbol the object carries for it (stdcall `_name@N`, cdecl `_name`).
std::map<std::string, std::string> public_symbols(ByteView object) {
    std::vector<std::string> publics = formats::omf::publics(object);
    std::sort(publics.begin(), publics.end());   // a clean name with two decorated symbols gets the same one every time
    std::map<std::string, std::string> by_clean;
    for (const std::string& sym : publics) {
        std::string base = !sym.empty() && sym[0] == '_' ? sym.substr(1) : sym;
        base = base.substr(0, base.find('@'));
        by_clean.emplace(base, sym);
    }
    return by_clean;
}

}  // namespace

std::vector<VeneerFunction> veneer_functions(std::string_view c_source) {
    static const Regex definition(R"(^([A-Za-z_][\w \*]*?)\s+(\w+)\s+(\w+)\s*\(([^;{]*)\)\s*\{)", RegexFlags::Multiline);
    std::vector<VeneerFunction> out;
    definition.for_each_match(c_source, [&](const RegexMatch& m) {
        out.push_back({std::string(m.group(2)), std::string(m.group(3)), std::string(m.group(4))});
        return true;
    });
    return out;
}

BuildResult build_openwatcom(const watcom::OpenWatcom& ow, const stdfs::path& srcdir_in, const std::optional<stdfs::path>& outdir_in) {
    BuildResult result;
    const auto tools = openwatcom_tools(ow);
    if (!tools) {
        result.error = std::string(kOpenWatcomMissing);
        return result;
    }
    const stdfs::path srcdir = stdfs::absolute(srcdir_in);
    const stdfs::path outdir = stdfs::absolute(outdir_in ? *outdir_in : srcdir / "_ow");
    fs::make_dirs(outdir);
    const std::vector<stdfs::path> cfiles = files_with(srcdir, {".c"});
    if (cfiles.empty()) {
        result.error = "no veneer .c in " + fs::to_text(srcdir);
        return result;
    }
    const std::string module = fs::to_text(cfiles[0].stem());
    result.module = module;
    const stdfs::path libdir = ow.lib_dir(watcom::Target::Win32);

    // the veneer is copied verbatim (authentic source; the exports come from directives, not __declspec)
    const std::string veneer = veneer_text(cfiles[0]);
    fs::write_text(outdir / (module + ".c"), veneer);
    // the thunk connection routine is emitted by the thunk compiler; stub it for the scaffold link
    fs::write_text(outdir / "thkstub.c",
                   "#define WIN32_LEAN_AND_MEAN\n#include <windows.h>\n"
                   "/* scaffold stub for the thunk compiler's connection routine (real one at body curation) */\n"
                   "BOOL __stdcall " + module + "_ThunkConnect32(LPSTR a,LPSTR b,HINSTANCE h,DWORD r){return TRUE;}\n");
    std::string log;
    try {
        for (const std::string& c : {module + ".c", std::string("thkstub.c")}) {
            std::string out;
            const std::uint32_t rc = run_logged({fs::to_text(tools->wcc386), "-bt=nt", "-zq", "-w0", c}, outdir, tools->env, out, std::chrono::seconds(180));
            log += std::format("$ wcc386 {} -> {}\n{}\n", c, rc, std::string(text::trim(out)));
            if (rc != 0) {
                result.error = "compile " + c + " failed";
                result.log = log;
                return result;
            }
        }

        // export directives: OpenWatcom decorates a public symbol by convention -- stdcall `_name@N` (N = the parameter BYTES),
        // cdecl `_name`. The authoritative N is in the object's PUBDEF records, so by-value structs (POINT = 8, RECT = 16) get
        // the right N and not a guessed nargs*4; each clean export name is aliased to its decorated public symbol.
        const std::map<std::string, std::string> by_clean = public_symbols(fs::read_file(outdir / (module + ".obj")));
        std::vector<std::string> directives;
        for (const VeneerFunction& f : veneer_functions(veneer)) {
            if (f.name == "DllMain") continue;
            std::string internal;
            if (const auto it = by_clean.find(f.name); it != by_clean.end()) {
                internal = it->second;   // the exact symbol from PUBDEF (best)
            } else {   // fallback: compute the stdcall decoration
                const std::string_view parameters = text::trim(f.parameters);
                std::uint32_t bytes = 0;
                if (!parameters.empty() && parameters != "void") {
                    for (const std::string& p : split_parameters(parameters)) bytes += parameter_bytes(p);
                }
                const bool cdecl_call = f.convention.ends_with("APIV") || f.convention == "__cdecl" || f.convention == "cdecl";
                internal = cdecl_call ? "_" + f.name : std::format("_{}@{}", f.name, bytes);
            }
            directives.push_back("export " + f.name + "='" + internal + "'");
        }

        std::string lnk = "system nt_dll\nname " + module + ".dll\noption quiet\n";
        lnk += "libpath " + fs::to_text(libdir) + "\nlibrary kernel32,user32,gdi32\n";
        lnk += "file " + module + ".obj\nfile thkstub.obj\n";
        for (const std::string& d : directives) lnk += d + "\n";
        fs::write_text(outdir / "link.lnk", lnk);
        std::string out;
        const std::uint32_t rc = run_logged({fs::to_text(tools->wlink), "@link.lnk"}, outdir, tools->env, out);
        log += std::format("$ wlink -> {}\n{}\n", rc, std::string(text::trim(out)));
    } catch (const ToolError& e) {
        result.error = std::string("build run failed: ") + e.what();
        result.log = log;
        return result;
    }
    result.log = log;
    const stdfs::path dll = outdir / (module + ".dll");
    std::error_code ec;
    if (!stdfs::exists(dll, ec)) {
        result.error = "link produced no DLL";
        return result;
    }
    result.ok = true;
    result.dll = dll;
    result.bytes = fs::file_size(dll);
    try {
        result.exports = PeModule::open(dll).exports().size();
    } catch (const FormatError&) {   // a DLL that does not read counts no exports
        result.exports = 0;
    }
    return result;
}

// ---- the stub reader ----------------------------------------------------------------------------------------------

namespace {

// Bytes in the ModRM (+SIB +displacement) cluster starting at code[at]; 0 past the end.
std::size_t modrm_length(ByteView d, std::size_t i) {
    if (i >= d.size()) return 0;
    const unsigned modrm = d[i];
    const unsigned mod = modrm >> 6, rm = modrm & 7;
    std::size_t n = 1;
    if (mod == 3) return n;
    if (rm == 4) {   // a SIB byte
        n += 1;
        const unsigned base = i + 1 < d.size() ? d[i + 1] & 7u : 0u;
        if (mod == 0 && base == 5) n += 4;
    }
    if (mod == 1) n += 1;
    else if (mod == 2) n += 4;
    else if (mod == 0 && rm == 5) n += 4;
    return n;
}

constexpr bool in(unsigned op, unsigned lo, unsigned hi) { return op >= lo && op <= hi; }

// add..cmp r/m,r and r,r/m
constexpr bool is_arith_rm(unsigned op) {
    return op < 0x40 && (op & 4u) == 0;   // 00-03 08-0B 10-13 18-1B 20-23 28-2B 30-33 38-3B
}
// op eax,imm
constexpr bool is_arith_ax(unsigned op) { return op < 0x40 && (op & 7u) == 5; }
constexpr bool is_modrm_only(unsigned op) {
    return is_arith_rm(op) || op == 0x62 || op == 0x63 || in(op, 0x84, 0x8F) || in(op, 0xD0, 0xD3) || op == 0xFE ||
           in(op, 0xD8, 0xDF);   // (D8-DF: the x87 FPU in its ModRM form)
}

}  // namespace

std::size_t instruction_length(ByteView d, std::size_t at) {
    std::size_t i = at;
    bool opsize = false;
    while (i < d.size() && (d[i] == 0xF0 || d[i] == 0xF2 || d[i] == 0xF3 || d[i] == 0x2E || d[i] == 0x36 || d[i] == 0x3E || d[i] == 0x26 ||
                            d[i] == 0x64 || d[i] == 0x65 || d[i] == 0x66 || d[i] == 0x67)) {
        if (d[i] == 0x66) opsize = true;
        ++i;
    }
    if (i >= d.size()) return 0;
    const std::size_t immw = opsize ? 2 : 4;
    const unsigned op = d[i++];
    const auto used = [&] { return i - at; };
    if (op == 0x0F) {   // two bytes
        if (i >= d.size()) return 0;
        const unsigned op2 = d[i++];
        if (in(op2, 0x80, 0x8F)) return used() + immw;   // jcc rel32
        if (op2 == 0x1F || in(op2, 0x90, 0x9F) || op2 == 0xA3 || op2 == 0xA5 || op2 == 0xAB || op2 == 0xAD || op2 == 0xAF || op2 == 0xB6 ||
            op2 == 0xB7 || op2 == 0xBE || op2 == 0xBF || op2 == 0xB0 || op2 == 0xB1 || in(op2, 0x40, 0x4F)) {
            return used() + modrm_length(d, i);   // setcc, movzx/movsx, imul, cmov, bt, nop r/m
        }
        if (op2 == 0xA2 || op2 == 0x31 || op2 == 0x77 || op2 == 0x05 || op2 == 0x34 || op2 == 0x0B) return used();   // no operand
        return 0;
    }
    if (is_modrm_only(op)) return used() + modrm_length(d, i);
    if (op == 0x6B || op == 0x80 || op == 0x82 || op == 0x83 || op == 0xC0 || op == 0xC1 || op == 0xC6) return used() + modrm_length(d, i) + 1;
    if (op == 0x69 || op == 0x81 || op == 0xC7) return used() + modrm_length(d, i) + immw;
    if (op == 0xF6 || op == 0xF7) {   // group 3: test (/0, /1) has an immediate, the rest do not
        const std::size_t ml = modrm_length(d, i);
        const unsigned reg = i < d.size() ? (d[i] >> 3) & 7u : 7u;
        return used() + ml + (reg <= 1 ? (op == 0xF6 ? std::size_t{1} : immw) : 0);
    }
    if (op == 0xFF) return used() + modrm_length(d, i);   // group 5: inc, dec, call, jmp, push r/m
    if (in(op, 0x50, 0x5F) || in(op, 0x90, 0x9D) || in(op, 0x40, 0x4F) || op == 0xC3 || op == 0xC9 || op == 0xCB || op == 0xCC ||
        op == 0xF4 || op == 0xF5 || in(op, 0xF8, 0xFD) || op == 0x98 || op == 0x99) {
        return used();   // push, pop, inc, dec, nop, ret, leave, flags: one byte
    }
    if (in(op, 0xB8, 0xBF)) return used() + immw;   // mov reg, imm32
    if (in(op, 0xB0, 0xB7)) return used() + 1;      // mov reg8, imm8
    if (op == 0x68) return used() + immw;           // push imm32
    if (op == 0x6A || op == 0xEB || in(op, 0x70, 0x7F) || op == 0xA8 || (op < 0x40 && (op & 7u) == 4)) {
        return used() + 1;   // push imm8, jmp rel8, jcc rel8, test al, op al,imm8
    }
    if (op == 0xE8 || op == 0xE9) return used() + immw;   // call/jmp rel32
    if (op == 0xA9 || is_arith_ax(op)) return used() + immw;   // test eax,imm; op eax,imm
    if (in(op, 0xA0, 0xA3)) return used() + 4;   // mov moffs (address-sized)
    if (op == 0xC2) return used() + 2;           // ret imm16
    return 0;
}

std::set<std::uint32_t> smapls_targets(const PeModule& pe) {
    const std::uint64_t base = pe.image().header().image_base;
    const auto absolute = [&](std::uint64_t rva) { return static_cast<std::uint32_t>((base + rva) & 0xFFFFFFFFu); };
    std::set<std::uint32_t> slots;
    for (const formats::pe::ImportModule& m : pe.image().import_table(false)) {
        if (text::upper(m.module).find("KERNEL32") == std::string::npos) continue;
        for (const formats::pe::ImportSymbol& s : m.symbols) {
            if (s.name && (s.name->starts_with("SMapLS") || s.name->starts_with("SUnMapLS"))) slots.insert(absolute(s.slot_rva));
        }
    }
    std::set<std::uint32_t> targets = slots;
    // the `jmp dword ptr [slot]` (FF 25 abs32) thunks of an incremental link forward to those IAT slots
    const ByteView d = pe.image().data();
    for (std::size_t o = 0; o + 6 <= d.size(); ++o) {
        if (d[o] != 0xFF || d[o + 1] != 0x25) continue;
        const std::uint32_t operand = static_cast<std::uint32_t>(d[o + 2]) | static_cast<std::uint32_t>(d[o + 3]) << 8 |
                                      static_cast<std::uint32_t>(d[o + 4]) << 16 | static_cast<std::uint32_t>(d[o + 5]) << 24;
        if (!slots.contains(operand)) continue;
        // the offset of the thunk, as an RVA (nullopt when it lies in no section's raw data)
        for (const formats::pe::Section& s : pe.image().sections()) {
            if (s.raw_size != 0 && s.raw_ptr <= o && o < static_cast<std::uint64_t>(s.raw_ptr) + s.raw_size) {
                targets.insert(absolute(s.virtual_address + (o - s.raw_ptr)));
                break;
            }
        }
    }
    return targets;
}

std::optional<std::uint32_t> argument_bytes_by_epilogue(ByteView d) {
    // the candidates in the order first met, each with how often it occurs; and how often right after a terminator
    std::vector<std::pair<std::uint32_t, std::uint32_t>> counts;
    std::vector<std::pair<std::uint32_t, std::uint32_t>> after_terminator;
    const auto bump = [](std::vector<std::pair<std::uint32_t, std::uint32_t>>& v, std::uint32_t n) {
        for (auto& [key, count] : v) {
            if (key == n) {
                ++count;
                return;
            }
        }
        v.emplace_back(n, 1);
    };
    for (std::size_t i = 0; i + 2 < d.size(); ++i) {
        if (d[i] != 0xC2 || d[i + 2] != 0x00) continue;
        const std::uint32_t n = d[i + 1];
        if (n % 4 != 0 || n == 0 || n > 256) continue;
        bump(counts, n);
        if (i > 0 && (in(d[i - 1], 0x58, 0x5F) || d[i - 1] == 0xC9)) bump(after_terminator, n);   // pop reg / leave
    }
    if (counts.empty()) return std::nullopt;
    auto best = counts.front();
    for (const auto& c : counts) {
        if (c.second > best.second) best = c;
    }
    if (best.second >= 2) return best.first;
    if (after_terminator.size() == 1) return after_terminator.front().first;   // exactly one terminator-preceded candidate
    return std::nullopt;
}

StubSignature stub_signature(ByteView d, std::uint32_t address, const std::set<std::uint32_t>& marshalling) {
    StubSignature sig;
    if (d.empty()) return sig;
    const auto le32 = [&](std::size_t at) {
        return static_cast<std::uint32_t>(d[at]) | static_cast<std::uint32_t>(d[at + 1]) << 8 | static_cast<std::uint32_t>(d[at + 2]) << 16 |
               static_cast<std::uint32_t>(d[at + 3]) << 24;
    };
    std::size_t i = 0;
    while (i < d.size()) {
        const unsigned b = d[i];
        if (b == 0xC2) {   // ret imm16: a stdcall epilogue
            if (i + 3 > d.size()) return sig;   // cut off inside the operand: unresolved
            const std::uint32_t n = d[i + 1] | static_cast<std::uint32_t>(d[i + 2]) << 8;
            if (n % 4 == 0 && n <= 256) sig.argument_bytes = n;
            return sig;
        }
        if (b == 0xC3) {   // ret: no stack cleanup (no arguments, or cdecl)
            sig.argument_bytes = 0;
            return sig;
        }
        if (b == 0xCC) return sig;   // int3 padding before a ret: unknown
        if (b == 0xE8 && i + 5 <= d.size()) {   // call rel32 -> an SMapLS jump thunk?
            const std::uint32_t target = address + static_cast<std::uint32_t>(i + 5) + le32(i + 1);   // (mod 2^32)
            if (marshalling.contains(target)) ++sig.pointer_args;
        } else if (b == 0xFF && i + 6 <= d.size() && d[i + 1] == 0x15) {   // call dword ptr [abs] -> the IAT's SMapLS?
            if (marshalling.contains(le32(i + 2))) ++sig.pointer_args;
        }
        const std::size_t length = instruction_length(d, i);
        if (length == 0) break;   // an opcode that is not modelled (an inline jump table): fall back to the epilogue scan
        i += length;
    }
    // A function with an inline switch table defeats the linear walk. Its epilogue still appears at every return site with
    // the same N, so scan for it.
    sig.argument_bytes = argument_bytes_by_epilogue(d);
    return sig;
}

ThunkTarget thunk_target(const PeModule& pe, std::string_view module) {
    const ByteView d = pe.image().data();
    const std::string up = text::upper(module);
    ThunkTarget out;
    std::set<std::string> seen;
    // every "<name>.DLL" string of the image: 2 to 11 characters of [A-Za-z0-9_] starting with a letter or '_', then .dll in
    // either case (leftmost, not overlapping)
    std::size_t resume = 0;
    for (std::size_t p = 2; p + 4 <= d.size(); ++p) {
        if (d[p] != '.' || (d[p + 1] | 0x20) != 'd' || (d[p + 2] | 0x20) != 'l' || (d[p + 3] | 0x20) != 'l') continue;
        const std::size_t lowest = std::max(resume, p >= 11 ? p - 11 : std::size_t{0});
        std::size_t run = p;
        while (run > lowest && is_word(static_cast<char>(d[run - 1]))) --run;
        std::size_t start = run;
        while (start + 2 <= p && !is_letter(static_cast<char>(d[start]))) ++start;
        if (start + 2 > p) continue;
        std::string name = text::upper(text::from_latin1(d.subspan(start, p + 4 - start)));
        resume = p + 4;
        if (seen.insert(name).second) out.candidates.push_back(std::move(name));
    }
    // A flat thunk's 16-bit partner is almost always the 32-bit name with a trailing "32" dropped (AVICAP32 -> AVICAP,
    // MSACM32 -> MSACM); a few well-known ones rename. Take the partner's name only if it is actually in the image --
    // authoritative -- else the lone non-system .DLL string; else leave it ambiguous.
    static const std::map<std::string, std::string> kAlias = {
        {"COMCTL32", "COMMCTRL.DLL"}, {"MSVFW32", "MSVIDEO.DLL"}, {"WINMM", "MMSYSTEM.DLL"}, {"SHELL32", "SHELL.DLL"},
        {"COMDLG32", "COMMDLG.DLL"},  {"MPR", "PMSPL.DLL"},       {"VERSION", "VER.DLL"},    {"WSOCK32", "WINSOCK.DLL"},
        {"OLE32", "OLE2.DLL"}};
    std::vector<std::string> partners;
    if (const auto it = kAlias.find(up); it != kAlias.end()) partners.push_back(it->second);
    partners.push_back((up.ends_with("32") ? up.substr(0, up.size() - 2) : up) + ".DLL");
    for (const std::string& partner : partners) {
        if (seen.contains(partner)) {
            out.dll16 = partner;
            return out;
        }
    }
    static const std::set<std::string> kSystem = {"KERNEL32.DLL", "USER32.DLL",  "GDI32.DLL",   "ADVAPI32.DLL", "WINMM.DLL", "SHELL32.DLL",
                                                  "COMDLG32.DLL", "COMCTL32.DLL", "OLE32.DLL",   "OLEAUT32.DLL", "RPCRT4.DLL", "VERSION.DLL",
                                                  "LZ32.DLL",     "MPR.DLL",      "WSOCK32.DLL", "IMM32.DLL",    "WINSPOOL.DRV", "DDEML.DLL",
                                                  "MSVCRT.DLL",   "MSVCRT20.DLL", "CRTDLL.DLL",  "WOW32.DLL",    "DCIMAN32.DLL", "MMSYSTEM.DLL"};
    std::vector<std::string> others;
    for (const std::string& c : out.candidates) {
        if (!kSystem.contains(c) && c != up + ".DLL") others.push_back(c);
    }
    if (others.size() == 1) out.dll16 = others.front();
    return out;
}

const Kernel32Ordinal* kernel32_ordinal(std::uint32_t ordinal) {
    static const std::map<std::uint32_t, Kernel32Ordinal> kTable = {
        {17, {"CommonUnimpStub", 0}},         {20, {"DosFileHandleToWin32Handle", 4}}, {21, {"Win32HandleToDosFileHandle", 4}},
        {22, {"DisposeLZ32Handle", 4}},       {41, {"ThunkInitLSF", std::nullopt}},     {89, {"FT_PrologPrime", std::nullopt}},
        {90, {"QT_ThunkPrime", std::nullopt}}};
    const auto it = kTable.find(ordinal);
    return it == kTable.end() ? nullptr : &it->second;
}

// ---- recover ------------------------------------------------------------------------------------------------------

std::map<std::string, SdkPrototype> sdk_prototypes(const symbols::prototypes::PrototypeTable& table, symbols::prototypes::Bits bits) {
    std::map<std::string, SdkPrototype> out;
    const auto& protos = bits == symbols::prototypes::Bits::W16 ? table.headers().w16 : table.headers().w32;
    for (const auto& [name, p] : protos) {
        std::vector<std::string> parts;
        for (const auto& parameter : p.parameters) parts.push_back(std::string(text::trim(parameter.type + " " + parameter.name)));
        out[name] = {p.returns, parts.empty() ? "void" : text::join(parts, ", "), p.marker.empty() ? "WINAPI" : p.marker, text::upper(p.header)};
    }
    return out;
}

bool is_thunk_data(std::string_view nm) {
    if (nm.ends_with("ThunkData32") || nm.ends_with("ThunkData") || nm.find("_ThunkData") != std::string_view::npos || nm.ends_with("ThkSL")) {
        return true;
    }
    // <alnum>+ Thk [SL] [_] ThunkData at the start
    for (std::size_t k = 1; k + 3 <= nm.size() && is_alnum(nm[k - 1]); ++k) {
        if (nm.substr(k, 3) != "Thk") continue;
        std::string_view rest = nm.substr(k + 3);
        if (rest.starts_with("SL")) rest.remove_prefix(2);
        if (rest.starts_with("_")) rest.remove_prefix(1);
        if (rest.starts_with("ThunkData")) return true;
        // (the optional parts may also have been skipped over by a shorter reading)
        std::string_view bare = nm.substr(k + 3);
        if (bare.starts_with("ThunkData") || (bare.starts_with("_") && bare.substr(1).starts_with("ThunkData")) ||
            (bare.starts_with("SL") && bare.substr(2).starts_with("ThunkData"))) {
            return true;
        }
    }
    return false;
}

std::uint32_t parameter_bytes(std::string_view part) {
    part = text::trim(part);
    if (part.empty() || part == "void" || part == "...") return 0;
    if (part.find('*') != std::string_view::npos) return 4;
    static const std::set<std::string_view> kQualifiers = {"const", "far", "near", "FAR", "NEAR", "unsigned", "signed"};
    static const std::map<std::string_view, std::uint32_t> kByValue = {
        {"POINT", 8}, {"POINTL", 8}, {"SIZE", 8}, {"SIZEL", 8}, {"RECT", 16}, {"RECTL", 16}, {"double", 8},
        {"__int64", 8}, {"LARGE_INTEGER", 8}, {"ULARGE_INTEGER", 8}, {"DWORDLONG", 8}, {"LONGLONG", 8}};
    for (const std::string_view token : text::split_ws(part)) {
        if (kQualifiers.contains(token)) continue;
        const auto it = kByValue.find(token);
        return it == kByValue.end() ? 4 : it->second;
    }
    return 4;
}

std::vector<std::string> split_parameters(std::string_view s) {
    std::vector<std::string> out;
    int depth = 0;
    std::string current;
    for (const char ch : s) {
        if (ch == '(') ++depth;
        else if (ch == ')') --depth;
        if (ch == ',' && depth == 0) {
            out.push_back(std::move(current));
            current.clear();
        } else {
            current += ch;
        }
    }
    if (!text::trim(current).empty()) out.push_back(std::move(current));
    return out;
}

std::string name_parameters(std::string_view list) {
    const std::string_view p = text::trim(list);
    if (p.empty() || text::iequals(p, "void")) return "void";
    static const std::set<std::string_view> kKeywords = {"void",  "char", "int",   "long",   "short",    "unsigned", "signed", "double",
                                                         "float", "const", "struct", "union", "enum",     "far",      "near",   "huge",
                                                         "volatile"};
    std::vector<std::string> out;
    int index = 0;
    for (const std::string& raw : split_parameters(p)) {
        ++index;
        const std::string part(text::trim(raw));
        if (part == "..." || part.empty()) {
            out.push_back(part.empty() ? "void" : part);
            continue;
        }
        // the parameter is named iff its trailing identifier (a glued `*` aside, as in `int*pn` or `UINT FAR *puMxId`) is a
        // mixed- or lower-case name -- SDK types are ALL-CAPS or keywords
        bool is_type;
        if (text::split_ws(part).size() == 1) {
            is_type = true;   // a single token is always a bare type (HWND, LPCSTR, int, va_list): no name present
        } else {
            std::size_t end = part.size();
            while (end > 0 && text::is_space(part[end - 1])) --end;
            std::size_t begin = end;
            while (begin > 0 && is_word(part[begin - 1])) --begin;
            while (begin < end && !is_letter(part[begin])) ++begin;   // an identifier starts with a letter or '_'
            const std::string last = part.substr(begin, end - begin);
            const bool has_letter = std::any_of(last.begin(), last.end(), [](char c) { return is_letter(c) && c != '_'; });
            const bool all_upper = has_letter && std::none_of(last.begin(), last.end(), [](char c) { return c >= 'a' && c <= 'z'; });
            // a single upper-case letter (X, Y, W, H) is a legitimate parameter name
            is_type = last.empty() || kKeywords.contains(text::lower(last)) || (all_upper && last.size() >= 2);
        }
        out.push_back(is_type ? part + " a" + std::to_string(index) : part);
    }
    return text::join(out, ", ");
}

std::vector<std::string> needed_includes(const std::vector<const SdkPrototype*>& used) {
    // SDK headers whose contents <windows.h> already pulls in
    static const std::set<std::string> kInWindowsH = {"WINUSER.H", "WINGDI.H", "WINBASE.H", "WINREG.H", "WINNLS.H", "WINNT.H",
                                                      "WINDOWS.H", "WINDEF.H", "WINCON.H",  "WINSVC.H", "WINERROR.H"};
    // some SDK headers only compile if a prerequisite is included first (the original source did so): msacm.h and vfw.h use
    // the multimedia types of mmsystem.h; mciavi.h builds on both
    static const std::map<std::string, std::vector<std::string>> kPrerequisites = {
        {"msacm.h", {"mmsystem.h", "mmreg.h"}}, {"vfw.h", {"mmsystem.h", "commdlg.h"}}, {"digitalv.h", {"mmsystem.h"}},
        {"mmreg.h", {"mmsystem.h"}},            {"mciavi.h", {"mmsystem.h", "vfw.h"}},  {"prsht.h", {"commctrl.h"}},
        {"commctrl.h", {"objbase.h"}}};   // (IStream in ImageList_Read/Write, the property sheets)
    std::vector<std::string> headers;
    std::set<std::string> seen;
    const std::function<void(const std::string&)> add = [&](const std::string& h) {
        const std::string lower = text::lower(h);
        if (seen.contains(lower) || kInWindowsH.contains(text::upper(h))) return;
        if (const auto it = kPrerequisites.find(lower); it != kPrerequisites.end()) {
            for (const std::string& pre : it->second) add(pre);
        }
        seen.insert(lower);
        headers.push_back(lower);
    };
    for (const SdkPrototype* p : used) {
        if (p != nullptr && !p->header.empty()) add(p->header);
    }
    return headers;
}

namespace {

// The parameter list text for an export. The real SDK prototype wins; else the stdcall arity recovered from `ret N` (each
// argument a machine word, the first `pointer_args` flagged as marshalled pointers); nullopt if the stub did not resolve
// (which keeps the honest `(void)` scaffold with its TODO).
std::optional<std::string> signature_parameters(const StubSignature& sig, const SdkPrototype* proto) {
    if (proto != nullptr) return proto->params;
    if (!sig.argument_bytes) return std::nullopt;
    const std::uint32_t count = *sig.argument_bytes / 4;
    if (count == 0) return "void";
    std::vector<std::string> parts;
    for (std::uint32_t i = 0; i < count; ++i) {
        parts.push_back(i < sig.pointer_args ? "LPVOID p" + std::to_string(i + 1) : "DWORD a" + std::to_string(i + 1));
    }
    return text::join(parts, ", ");
}

// The function's calling-convention keyword: the SDK prototype's when known (WINAPI, or the varargs VFWAPIV of ICCompress),
// so the definition's convention matches the header and does not clash (C2373); else WINAPI (flat thunks are stdcall).
std::string_view signature_convention(const SdkPrototype* proto) {
    return proto != nullptr && !proto->convention.empty() ? std::string_view(proto->convention) : std::string_view("WINAPI");
}

std::string signature_return(const SdkPrototype* proto) { return proto != nullptr && !proto->returns.empty() ? proto->returns : "DWORD"; }

const SdkPrototype* find_prototype(const std::map<std::string, SdkPrototype>& sdk, const std::string& name) {
    const auto it = sdk.find(name);
    return it == sdk.end() ? nullptr : &it->second;
}

}  // namespace

Recovery recover(const stdfs::path& pe_file, const stdfs::path& outdir, const std::map<std::string, SdkPrototype>& sdk) {
    const std::optional<Binary> binary = load(pe_file);
    if (!binary || !std::holds_alternative<PeModule>(*binary)) throw UsageError("not a PE: " + fs::to_text(pe_file));
    const PeModule& pe = std::get<PeModule>(*binary);
    const std::string module = text::lower(fs::to_text(pe_file.stem()));
    fs::make_dirs(outdir);
    const auto exports = pe.exports();
    const auto imports = pe.imports();
    const std::vector<std::string>& k32 = kernel32_symbols(imports);

    // SMapLS in the IAT, so a `call SMapLS` in a stub is recognisable (it marks a pointer argument)
    const auto addresses = pe.export_addresses();
    const std::set<std::uint32_t> marshalling = smapls_targets(pe);
    std::vector<std::uint32_t> starts;
    for (const auto& [name, rva] : addresses) starts.push_back(rva);
    std::sort(starts.begin(), starts.end());
    const auto extent = [&](std::uint32_t rva) {   // bound the walk to this function
        const auto next = std::upper_bound(starts.begin(), starts.end(), rva);
        return std::min<std::size_t>(next == starts.end() ? 8192 : *next - rva, 8192);
    };
    std::map<std::string, StubSignature> signatures;
    const std::uint32_t image_base = static_cast<std::uint32_t>(pe.image().header().image_base);
    for (const auto& [name, rva] : addresses) signatures[name] = stub_signature(pe.read_at(rva, extent(rva)), image_base + rva, marshalling);
    const ThunkTarget target = thunk_target(pe, module);

    // 1) the module .def: the exports, verbatim
    {
        std::string def = "LIBRARY   " + text::upper(module) + "\nEXPORTS\n";
        for (const auto& e : exports) def += std::format("    {:<18} @{}\n", e.name, e.ordinal);
        write_windows_text(outdir / (module + ".def"), def);
    }

    // 2) <module>k32.def: the KERNEL32 by-ordinal helpers this binary imports, named
    std::vector<std::uint32_t> ordinals, unknown;
    for (const std::string& n : k32) {
        if (n.starts_with("@")) {
            if (const auto v = text::parse_int<std::uint32_t>(n.substr(1))) ordinals.push_back(*v);
        }
    }
    std::sort(ordinals.begin(), ordinals.end());
    for (const std::uint32_t o : ordinals) {
        if (kernel32_ordinal(o) == nullptr) unknown.push_back(o);
    }
    {
        std::string def = "; KERNEL32 by-ordinal flat-thunk / handle helpers imported by " + text::upper(module) + ".\n";
        def += "; Names from the Windows Me symbol file (Geoff Chappell). NONAME = by-ordinal import.\n";
        def += "; NOTE: ThunkInitLSF@41 is the Microsoft-INTERNAL connect (never in a public SDK); we\n";
        def += ";       connect via the public ThunkConnect32 instead -- see the engine docs.\n";
        def += "LIBRARY KERNEL32\nEXPORTS\n";
        for (const std::uint32_t o : ordinals) {
            if (o == 41) continue;   // the internal ThunkInitLSF: deliberately not imported (use ThunkConnect32)
            const Kernel32Ordinal* known = kernel32_ordinal(o);
            const std::string name = known != nullptr ? std::string(known->name) : "Kernel32_Ord" + std::to_string(o);
            const bool sized = known != nullptr && known->argument_bytes;
            const std::string decorated = sized ? name + "@" + std::to_string(*known->argument_bytes) : name;
            def += std::format("    {:<28} @{} NONAME\n", decorated, o);
        }
        write_windows_text(outdir / (module + "k32.def"), def);
    }

    const std::string dll16 = target.dll16.value_or("?16.DLL");   // the real 16-bit target when unambiguous, else a placeholder
    Recovery out;
    out.module = module;
    out.exports = exports.size();
    out.kernel32_ordinals = ordinals;
    out.unknown_ordinals = unknown;
    out.outdir = outdir;
    out.target16 = target.dll16;
    out.dll_candidates = target.candidates;

    // 3) the .thk: one thunked function per export; arity recovered from the stub's stdcall epilogue, pointer arguments (from
    //    SMapLS) flagged for the thunk compiler
    {
        std::string thk =
            "/* " + module + ".thk -- flat-thunk script. Function list from the export table; per-function\n"
            " * ARITY recovered from the stub's `ret N` (stdcall) and pointer args from the SMapLS\n"
            " * marshalling. Pointer input/output DIRECTION is defaulted to input -- curate where a\n"
            " * function writes back through a pointer (mark output/inout). 16-bit target: " + dll16 + ". */\n"
            "enablemapdirect3216 = true;\n\n"
            "typedef unsigned short WORD;  typedef unsigned long DWORD;  typedef int INT;\n"
            "typedef long LONG;  typedef char *LPSTR;  typedef void *LPVOID;\n"
            "typedef void VOID;  typedef int HFILE;\n\n";
        // 16-bit Windows has no wide APIs: the A and W exports share ONE 16-bit target (the ANSI form), so each base is
        // emitted once, preferring the A variant's (LPSTR) prototype. Thunk-data exports are not functions: noted, skipped.
        struct Pick {
            std::uint64_t ordinal;
            std::string name;
        };
        OrderedMap<std::string, Pick> by_base;
        std::vector<std::string> thunk_data;
        for (const auto& e : exports) {
            if (is_thunk_data(e.name)) {
                thunk_data.push_back(e.name);
                continue;
            }
            const bool wide_or_ansi = e.name.ends_with('A') || e.name.ends_with('W');
            const std::string base = wide_or_ansi ? e.name.substr(0, e.name.size() - 1) : e.name;
            const Pick* have = by_base.find(base);
            // prefer an A export (or a suffix-less one) as the canonical 16-bit target
            if (have == nullptr || (e.name.ends_with('A') && !have->name.ends_with('A'))) by_base.insert_or_assign(base, Pick{e.ordinal, e.name});
        }
        if (!thunk_data.empty()) {
            thk += "/* thunk-data exports (emitted by the thunk compiler, not APIs): " + text::join(thunk_data, ", ") + " */\n\n";
        }
        std::vector<std::pair<std::string, Pick>> bases(by_base.begin(), by_base.end());
        std::stable_sort(bases.begin(), bases.end(), [](const auto& a, const auto& b) { return a.second.ordinal < b.second.ordinal; });
        for (const auto& [base, pick] : bases) {
            const SdkPrototype* proto = find_prototype(sdk, pick.name);
            if (proto == nullptr) proto = find_prototype(sdk, base + "A");
            if (proto == nullptr) proto = find_prototype(sdk, base);
            const StubSignature sig = signatures.contains(pick.name) ? signatures.at(pick.name) : StubSignature{};
            const std::optional<std::string> params = signature_parameters(sig, proto);
            const std::string ret = signature_return(proto);
            if (!params) {
                thk += std::format("/* @{} {} -- stub did not resolve; TODO: signature + pointer directions */\n{} {}16(void)\n{{\n}}\n\n",
                                   pick.ordinal, pick.name, ret, base);
                continue;
            }
            thk += std::format("/* @{} {} -- arity recovered from stub{} */\n{} {}16({})\n{{\n}}\n\n", pick.ordinal, pick.name,
                               proto != nullptr ? " (SDK prototype)" : "", ret, base, *params);
        }
        write_windows_text(outdir / (module + ".thk"), thk);
    }

    // 4) the veneer .c: DllMain (the real 16-bit target) and a thunk-through per export with its recovered signature; the
    //    bodies stay scaffolds (the marshalling, the thunk and the return are filled in during curation)
    {
        std::vector<const SdkPrototype*> used;
        for (const auto& e : exports) {
            if (const SdkPrototype* p = find_prototype(sdk, e.name)) used.push_back(p);
        }
        std::string includes;
        for (const std::string& h : needed_includes(used)) includes += "#include <" + h + ">\n";
        const std::string upper = text::upper(module);
        std::string c =
            "/* " + module + ".c -- 32-bit flat-thunk veneer. DllMain + per-export thunk-through.\n"
            " * SIGNATURES are recovered from each stub (stdcall arity via `ret N`; pointer args\n"
            " * via SMapLS). Per-function tags: documented/undocumented = published API; behavioral\n"
            " * = body reconstructed from observed behaviour (not byte-exact); scaffold = body not\n"
            " * yet implemented. Curation fills the marshalling and confirms doc/undoc + accuracy. */\n"
            "#define WIN32_LEAN_AND_MEAN\n#include <windows.h>\n" + includes + "\n"
            "extern BOOL __stdcall " + module + "_ThunkConnect32(LPSTR,LPSTR,HINSTANCE,DWORD);\n\n"
            "/* DLL entry -- connects the 32:16 thunk to the 16-bit target " + dll16 + " */\n"
            "BOOL WINAPI DllMain(HINSTANCE h, DWORD r, LPVOID res)\n{\n"
            "    if (r == DLL_PROCESS_ATTACH) { DisableThreadLibraryCalls(h);\n"
            "        if (!" + module + "_ThunkConnect32(\"" + dll16 + "\", \"" + upper + ".DLL\", h, r)) return FALSE; }\n"
            "    else if (r == DLL_PROCESS_DETACH) " + module + "_ThunkConnect32(\"" + dll16 + "\", \"" + upper + ".DLL\", h, r);\n"
            "    return TRUE;\n}\n\n";
        // (the connection call names the 16-bit DLL first and this module second: the strings the thunk compiler's routine matches)
        for (const auto& e : exports) {
            const std::string at = "@" + std::to_string(e.ordinal) + " " + e.name;
            if (is_thunk_data(e.name)) {   // a data export, emitted by the thunk compiler
                c += "/* " + at + " -- thunk-data table (generated by the thunk compiler from " + module + ".thk;\n"
                     " * exported by name in the .def, no C definition here) */\n\n";
                continue;
            }
            if (!text::is_identifier(e.name)) {   // decorated or non-identifier export (_f@4)
                c += "/* " + at + " -- decorated/non-identifier export name; kept in the .def, no C\n"
                     " * definition here (the name is not a legal C identifier) */\n\n";
                continue;
            }
            // #undef the name before defining it: many SDK names are macros in the headers -- A/W aliases (`#define PlaySound
            // PlaySoundA`) and function-like macros (`ICCompress` -> ICSendMessage in vfw.h) -- which would corrupt our real
            // definition. A no-op when the name is not a macro.
            const std::string undef = "#undef " + e.name + "\n";
            const SdkPrototype* proto = find_prototype(sdk, e.name);
            const StubSignature sig = signatures.contains(e.name) ? signatures.at(e.name) : StubSignature{};
            const std::optional<std::string> params = signature_parameters(sig, proto);
            const std::string ret = signature_return(proto);
            if (proto != nullptr) ++out.sdk_prototypes;
            if (!params) {   // the stub did not resolve: an honest scaffold
                c += "/* " + at + " -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */\n" +
                     undef + "DWORD WINAPI " + e.name + "(void) { return 0; }\n\n";
                continue;
            }
            ++out.signatures;
            out.pointer_args += sig.pointer_args;
            std::string tag = "documented";
            if (proto == nullptr) {
                tag = std::to_string(sig.argument_bytes.value_or(0) / 4) + "-arg";
                if (sig.pointer_args != 0) tag += ", " + std::to_string(sig.pointer_args) + " ptr";
            }
            const std::string_view lowered_return = text::trim(ret);
            const std::string body = text::iequals(lowered_return, "void") ? "{ }" : "{ return 0; }";   // void: no return value
            c += "/* " + at + " -- " + tag + "; scaffold. TODO: marshal args, thunk to " + dll16 + ", return */\n" + undef + ret + " " +
                 std::string(signature_convention(proto)) + " " + e.name + "(" + name_parameters(*params) + ") " + body + "\n\n";
        }
        write_windows_text(outdir / (module + ".c"), c);
    }

    out.recovered = {module + ".def", module + "k32.def", std::format("{}/{} signatures (stub arity)", out.signatures, exports.size())};
    out.scaffolded = {module + ".thk", module + ".c"};
    return out;
}

// ---- the command --------------------------------------------------------------------------------------------------

namespace {

std::string numbers(const std::vector<std::uint32_t>& values) {
    std::vector<std::string> text;
    for (const std::uint32_t v : values) text.push_back(std::to_string(v));
    return joined_list(text);
}

std::string describe(const Classification& c) {
    std::string out;
    const auto row = [&](std::string_view key, std::string_view value) { out += std::format("  {:<12} {}\n", key, value); };
    row("format", format_name(c.format));
    row("subtype", subtype_name(c.subtype));
    if (c.recipe) row("recipe", *c.recipe);
    if (c.exports) row("n_exports", std::to_string(*c.exports));
    if (c.imports) row("n_imports", std::to_string(*c.imports));
    row("why", c.why);
    return out;
}

void print_build_failure(const BuildResult& r) {
    out_text("  BUILD FAILED: " + r.error + "\n");
    std::vector<std::string_view> lines = text::lines(r.log);
    const std::size_t from = lines.size() > 12 ? lines.size() - 12 : 0;
    for (std::size_t i = from; i < lines.size(); ++i) out_text("    " + std::string(lines[i]) + "\n");
}

int run_command(const Services& services, const cli::Args& args) {
    const std::string name = args.required("module");
    stdfs::path path = fs::from_text(name);
    std::error_code ec;
    if (!stdfs::exists(path, ec)) {
        const std::optional<stdfs::path> found = services.retail ? services.retail(name) : std::nullopt;
        if (!found) throw NotFoundError(path, 0, "not a file, and no retail module of that name");
        path = *found;
    }
    int status = kExitOk;
    out_text("=== classify " + fs::to_text(path.filename()) + " ===\n");
    out_text(describe(classify(path)));

    const std::optional<std::string> recover_dir = args.value("recover");
    if (recover_dir) {
        out_text("=== recover (flat-thunk) -> " + *recover_dir + " ===\n");
        const Recovery r = recover(path, fs::from_text(*recover_dir), services.sdk ? services.sdk() : std::map<std::string, SdkPrototype>{});
        out_text(std::format("  module={}  exports={}  KERNEL32 ordinals={}{}\n", r.module, r.exports, numbers(r.kernel32_ordinals),
                             r.unknown_ordinals.empty() ? std::string() : "  UNKNOWN=" + numbers(r.unknown_ordinals)));
        out_text("  16-bit target: " + (r.target16 ? *r.target16 : "?16.DLL (ambiguous)") +
                 (r.target16 ? std::string() : "   candidates=" + joined_list(r.dll_candidates)) + "\n");
        out_text(std::format("  signatures recovered from stubs: {}/{}  ({} pointer args, {} SDK prototypes)\n", r.signatures, r.exports,
                             r.pointer_args, r.sdk_prototypes));
        out_text("  recovered (from binary): " + text::join(r.recovered, ", ") + "\n");
        out_text("  scaffolded (need curation): " + text::join(r.scaffolded, ", ") + "\n");
        if (args.flag("ow-compile")) {
            const CompileResult c = compile_openwatcom(services.locations.openwatcom, fs::from_text(*recover_dir));
            if (!c.error.empty()) {
                out_text("  ow-compile: " + c.error + "\n");
                status = kExitFailed;
            }
            for (const CompiledFile& f : c.files) {
                out_text(std::format("  ow-compile {:<16} {}{}\n", f.file, f.exit_code == 0 ? "OK" : "FAIL", f.first_error.empty() ? "" : "  " + f.first_error));
            }
        }
    }

    std::optional<stdfs::path> built;
    if (const auto diff = args.value("diff")) built = fs::from_text(*diff);
    if (const auto dir = args.value("ow-build")) {
        out_text("=== build (flat-thunk, from-source OpenWatcom) from " + *dir + " ===\n");
        const BuildResult r = build_openwatcom(services.locations.openwatcom, fs::from_text(*dir));
        if (!r.ok) {
            print_build_failure(r);
            return kExitFailed;
        }
        built = r.dll;
        out_text(std::format("  built {} ({} bytes, {} exports) -- no Microsoft tools\n", fs::to_text(built->filename()), r.bytes, r.exports));
    }
    if (const auto dir = args.value("build")) {
        out_text("=== build (flat-thunk) from " + *dir + " ===\n");
        const BuildResult r = build_period(services.locations, fs::from_text(*dir));
        if (!r.ok) {
            print_build_failure(r);
            return kExitFailed;
        }
        built = r.dll;
        out_text(std::format("  built {} ({} bytes)\n", fs::to_text(built->filename()), r.bytes));
    }
    if (built) {
        out_text("=== diff " + fs::to_text(built->filename()) + " vs " + fs::to_text(path.filename()) + " ===\n");
        const Result<BinDiff> d = bindiff(*built, path);
        if (!d.ok()) {
            out_text(std::string("  ERROR: ") + d.error().what() + "\n");
            return kExitFailed;
        }
        out_text(format_diff(d.value()));
    }
    return status;
}

}  // namespace

cli::Command command(Services services) {
    return cli::Command{
        "flat-thunk",
        "a 32-to-16-bit thunk DLL (lz32 and its kind): its source recovered, built with the thunk compiler, ML, CL and LINK, and compared",
        {cli::Positional{"module", "a corpus name, or the path of the module"},
         cli::Option{"--recover", "", "write the module's recovered source (.def, k32.def, .thk, .c) into DIR", cli::Arg::Value, "DIR"},
         cli::Option{"--ow-compile", "", "with --recover: check the recovered .c compiles with OpenWatcom", cli::Arg::Flag},
         cli::Option{"--build", "", "build a DLL from a source folder with the period tools (thunk, ML, CL, LINK)", cli::Arg::Value, "DIR"},
         cli::Option{"--ow-build", "", "build a DLL from a source folder with from-source OpenWatcom", cli::Arg::Value, "DIR"},
         cli::Option{"--diff", "", "compare this built DLL with the module", cli::Arg::Value, "BUILT"}},
        [services = std::move(services)](const cli::Args& args) { return run_command(services, args); },
        {},
        "",
        "Classifies the module by how it was built. For a flat thunk (a 32-bit veneer over a 16-bit DLL) the source is "
        "recovered from the binary's tables and stub code, built, and the result diffed against the module: entry point, "
        "sections, exports and imports."};
}

}  // namespace cb::build::flat_thunk
