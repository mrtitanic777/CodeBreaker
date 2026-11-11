#include "cb/exact/pascal_join.hpp"

#include <algorithm>
#include <cctype>
#include <format>

#include <nlohmann/json.hpp>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/log.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/exact/omf_object.hpp"
#include "cb/formats/omf.hpp"

namespace cb::exact::pascal_join {

namespace stdfs = std::filesystem;
namespace omf = formats::omf;

namespace {

constexpr std::string_view kNl = "\n";

bool is_lower_hex(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'); }

bool file_exists(const stdfs::path& p) {
    std::error_code ec;
    return stdfs::exists(p, ec);
}

std::string read_source(const stdfs::path& p) {
    const Bytes b = fs::read_file(p);
    return std::string(as_text(b));
}

std::string rstrip_newlines(std::string s) {
    while (!s.empty() && s.back() == '\n') s.pop_back();
    return s;
}

// "pXXXXX.pas" with five lowercase hex digits
bool is_piece_file(std::string_view f) {
    return f.size() == 10 && f[0] == 'p' && f.ends_with(".pas") && std::all_of(f.begin() + 1, f.begin() + 6, is_lower_hex);
}

// a user's EDITS pattern is Python-style (\1 for a group); the engine's template says $1
std::string template_of(std::string_view replacement) {
    std::string out;
    for (std::size_t i = 0; i < replacement.size(); ++i) {
        if (replacement[i] == '$') out += "$$";
        else if (replacement[i] == '\\' && i + 1 < replacement.size() && replacement[i + 1] >= '1' && replacement[i + 1] <= '9') { out += '$'; out += replacement[++i]; }
        else out += replacement[i];
    }
    return out;
}

std::uint32_t hex_of(std::string_view s) {
    const auto v = text::parse_int<std::uint32_t>(text::trim(s), 16);
    if (!v) throw ParseError("project", 0, 0, std::format("'{}' is not a hex address", s));
    return *v;
}

// (an identifier of upper-case letters, digits and underscores, starting with a letter or underscore)
std::set<std::string> upper_words(std::string_view s) {
    std::set<std::string> out;
    const auto start = [](char c) { return (c >= 'A' && c <= 'Z') || c == '_'; };
    for (std::size_t i = 0; i < s.size();) {
        if (!start(s[i])) { ++i; continue; }
        std::size_t e = i + 1;
        while (e < s.size() && (start(s[e]) || (s[e] >= '0' && s[e] <= '9'))) ++e;
        out.emplace(s.substr(i, e - i));
        i = e;
    }
    return out;
}

// The lines of the text that begin with a procedure or function heading (or, for the variables, a G_xxxx name) whose
// name is used somewhere; every other line is kept.
std::string keep_used(std::string_view text, const std::set<std::string>& used, bool variables) {
    static const Regex variable(R"(^  (G_[0-9A-F]{4}):)");
    static const Regex heading(R"(^(?:PROCEDURE|FUNCTION) +([A-Z_0-9]+))");
    std::string out;
    for (const auto l : text::split(text, '\n')) {
        if (l.empty()) continue;
        const auto m = (variables ? variable : heading).match(l);
        if (!m || used.contains(std::string(m->group(1)))) { out += l; out += kNl; }
    }
    return out;
}

}  // namespace

std::string_view verdict_text(Verdict v) {
    switch (v) {
        case Verdict::DoesNotCompile: return "does not compile";
        case Verdict::NoPlace: return "no place";
        case Verdict::Identical: return "identical";
        case Verdict::Equivalent: return "equivalent";
        case Verdict::Differs: break;
    }
    return "differs";
}

Compiland::Compiland(const Project& project, std::string name, const CompilandConfig& config)
    : project_(project), name_(std::move(name)), config_(config) {
    const stdfs::path root = project.dir / project.config.root;
    const std::vector<std::string>& over = config.over ? *config.over : project.config.over;
    for (const std::string& o : over) dirs_.push_back(root / o / config.dir);
    for (const std::string& d : config.dirs ? *config.dirs : std::vector<std::string>{config.dir}) dirs_.push_back(root / d);
    src_ = dirs_.back();
    headline_ = config.kind == CompilandKind::Program ? std::format("PROGRAM {} (INPUT, OUTPUT);", name_) : std::format("MODULE {};", name_);
    empty_ = config.kind == CompilandKind::Program ? "BEGIN\nEND.\n" : "END.\n";
    over_ = !over.empty();
    if (project.config.map && file_exists(root / *project.config.map)) {   // (pieces of another program's: where each is in this one)
        const Json j = json::load(root / *project.config.map);
        if (const auto it = j.find(name_); it != j.end())
            for (const auto& [k, v] : it->items()) map_[k] = v.get<std::string>();
    }
    if (over_) {   // EDITS (FILE|old|new, FILE|+|a line; * every piece, *~ a regular expression) and REMOVED, lowest overlay first
        for (auto d = dirs_.rbegin(); d != dirs_.rend(); ++d) {
            if (!file_exists(*d / "EDITS")) continue;
            const std::string content = read_source(*d / "EDITS");
            for (const auto l : text::lines(content)) {
                if (std::count(l.begin(), l.end(), '|') != 2) continue;
                const auto f = text::split(l, '|');
                edits_.push_back({std::string(f[0]), std::string(f[1]), std::string(f[2])});
            }
        }
        for (const stdfs::path& d : dirs_) {
            if (!file_exists(d / "REMOVED")) continue;
            const std::string content = read_source(d / "REMOVED");
            for (const auto w : text::split_ws(content)) removed_.emplace(w);
        }
    }
    if (project.config.kinds && file_exists(root / *project.config.kinds)) {   // (each procedure with the stack check or without)
        const Json j = json::load(root / *project.config.kinds);
        for (const auto& [k, v] : j.items()) {
            const auto a = text::parse_int<std::uint32_t>(k, 16);
            if (!a) throw ParseError("kinds", 0, 0, std::format("'{}' is not a hex address", k));
            if (v.contains("kind") && v["kind"].is_string()) kinds_[*a] = v["kind"].get<std::string>();
        }
    }
}

bool Compiland::maps(std::string_view hex) const {
    std::string upper = text::upper(hex);
    return map_.contains(upper);
}

std::uint32_t Compiland::address_of(std::string_view hex) const {
    if (!map_.empty()) {
        const auto it = map_.find(text::upper(hex));
        if (it != map_.end()) return hex_of(it->second);
    }
    return hex_of(hex);
}

stdfs::path Compiland::where(std::string_view file) const {
    for (const stdfs::path& d : dirs_)
        if (file_exists(d / std::string(file))) return d / std::string(file);
    return src_ / std::string(file);
}

std::vector<std::string> Compiland::pieces() const {
    std::set<std::string> found;
    for (const stdfs::path& d : dirs_) {
        std::error_code ec;
        if (!stdfs::is_directory(d, ec)) continue;
        for (const auto& e : stdfs::directory_iterator(d, ec)) {
            const std::string f = e.path().filename().string();
            if (is_piece_file(f) && !removed_.contains(f)) found.insert(f);
        }
    }
    std::vector<std::string> out(found.begin(), found.end());
    if (!map_.empty()) {   // (in the order of the places they have here)
        const auto key = [&](const std::string& f) {
            const auto it = map_.find(text::upper(std::string_view(f).substr(1, 5)));
            return it != map_.end() ? it->second : f.substr(1, 5);
        };
        std::stable_sort(out.begin(), out.end(), [&](const std::string& a, const std::string& b) { return key(a) < key(b); });
    }
    return out;
}

// The metacommand that gives the procedure of piece `file` the stack check it has in the program.
std::string Compiland::check(std::string_view file) const {
    if (kinds_.empty()) return "";
    const std::string_view hex = file.substr(1, 5);
    if (!map_.empty() && !maps(hex)) return "";
    const auto it = kinds_.find(address_of(hex));
    if (it == kinds_.end()) return "";
    return (it->second == "check" ? "{$STACKCK+}" : "{$STACKCK-}") + std::string(kNl);
}

std::string Compiland::expand(std::string_view text_in, const std::string& dir, const std::optional<std::string>& own) const {
    static const Regex include(R"(\s*\{\+(p[0-9a-f]{5})\}\s*)");
    std::vector<std::string> out;
    bool again = false;
    for (const auto line : text::split(text_in, '\n')) {
        if (const auto m = include.full_match(line)) {
            const std::string nested(m->group(1));
            out.push_back(check(nested) + expand(rstrip_newlines(read_source(where(dir + "/" + nested + ".pas"))), dir, nested));
            again = true;
        } else {
            if (again && own && line.starts_with("BEGIN") && (line.size() == 5 || !(std::isalnum(static_cast<unsigned char>(line[5])) || line[5] == '_'))) {
                out.push_back(rstrip_newlines(check(*own)));   // (back to the outer procedure's own setting)
                again = false;
            }
            out.emplace_back(line);
        }
    }
    return text::join(out, "\n");
}

std::string Compiland::piece(std::string_view file) const {
    const std::string name(file);
    std::string t = check(file) + expand(rstrip_newlines(read_source(where(file))), name.substr(0, name.size() - 4) + ".d", name) + "\n";
    for (const Edit& e : edits_) {
        if (e.file == "*") t = text::replace_all(t, e.old_text, e.new_text);
        if (e.file == "*~") t = Regex(e.old_text).replace_all(t, template_of(e.new_text));   // (the project's own pattern, once per edit)
    }
    return t;
}

std::string Compiland::read(std::string_view file) const {
    static const Regex declaration_line(R"(  G_[0-9A-F]{4})");
    static const Regex joined_declarations(R"(;\s+(?=G_[0-9A-F]{4}))");
    const stdfs::path p = where(file);
    std::string t = file_exists(p) ? rstrip_newlines(read_source(p)) + "\n" : "";
    for (const Edit& e : edits_) {
        if (e.file != file) continue;
        if (e.old_text == "+") t += e.new_text + "\n";
        else if (t.find(e.old_text) != std::string::npos) t = text::replace_all(t, e.old_text, e.new_text);
    }
    if (file == "GLOBALS.pas" && !edits_.empty()) {   // (one declaration a line again)
        std::vector<std::string> lines;
        for (const auto l : text::split(t, '\n')) lines.push_back(declaration_line.match(l) ? joined_declarations.replace_all(std::string(l), ";\n  ") : std::string(l));
        t = text::join(lines, "\n");
    }
    if (file == "MAIN.pas") {
        for (const Edit& e : edits_) {
            if (e.file == "*") t = text::replace_all(t, e.old_text, e.new_text);
            if (e.file == "*~") t = Regex(e.old_text).replace_all(t, template_of(e.new_text));
        }
    }
    return config_.stackck ? text::replace_all(t, " {$STACKCK-}", "") : t;
}

std::map<std::string, std::string> Compiland::result_types() const {
    static const Regex function(R"(^FUNCTION +(P[0-9A-F]{5})(\([^)]*\))?: *([A-Z_0-9]+))", RegexFlags::Multiline);
    std::map<std::string, std::string> out;
    std::vector<std::string> texts = {read("FORWARDS.pas")};
    for (const std::string& f : pieces()) texts.push_back(read(f));
    for (const std::string& t : texts)
        function.for_each_match(t, [&](const RegexMatch& m) {
            out[std::string(m.group(1))] = std::string(m.group(3));
            return true;
        });
    return out;
}

std::string Compiland::stub(std::string_view file, const std::map<std::string, std::string>& types) const {
    static const Regex heading(R"((PROCEDURE|FUNCTION) +(P[0-9A-F]{5}))");
    const std::string content = read(file);
    const std::string head(text::split(content, '\n').front());
    std::string body;
    if (const auto m = heading.match(head); m && m->group(1) == "FUNCTION") {
        const std::string fn(m->group(2));
        const auto it = types.find(fn);
        const std::string t = it == types.end() ? "WORD" : it->second;
        body = std::format("  {} := {}\n", fn, t == "BOOLEAN" ? "FALSE" : (t == "WORD" || t == "BYTE" || t == "INTEGER") ? "0" : "NIL");
    }
    return text::replace_all(head, "{TODO}", "") + "\nBEGIN\n" + body + "END;\n";
}

std::vector<std::vector<std::string>> Compiland::parts() const {
    const std::vector<std::string> ps = pieces();
    std::vector<std::size_t> size;
    std::size_t total = 0;
    for (const std::string& f : ps) {
        size.push_back(piece(f).size());
        total += size.back();
    }
    const std::size_t n = static_cast<std::size_t>(std::max(config_.parts, 1));
    std::vector<std::vector<std::string>> runs(n);
    std::size_t acc = 0;
    for (std::size_t i = 0; i < ps.size(); ++i) {
        runs[std::min(n - 1, acc * n / std::max<std::size_t>(total, 1))].push_back(ps[i]);
        acc += size[i];
    }
    runs.erase(std::remove_if(runs.begin(), runs.end(), [](const auto& r) { return r.empty(); }), runs.end());
    if (file_exists(where("MAIN.pas")) || config_.kind == CompilandKind::Module) {
        if (runs.empty()) runs.emplace_back();
        runs.back().push_back("MAIN.pas");
    }
    return runs;
}

std::string Compiland::source(const std::vector<std::string>& part) const {
    const std::string globals = read("GLOBALS.pas"), externs = read("EXTERNS.pas"), forwards = read("FORWARDS.pas");
    const auto types = result_types();
    std::map<std::string, std::string> body;
    std::string all_bodies;
    for (const std::string& f : part) {
        body[f] = f == "MAIN.pas" ? (read(f).empty() ? empty_ : read(f)) : piece(f);
        all_bodies += body[f];
    }
    const std::set<std::string> used = upper_words(all_bodies);
    const std::size_t var_at = globals.find("VAR\n");
    const std::string head = var_at == std::string::npos ? globals : globals.substr(0, var_at);
    const std::string kept = var_at == std::string::npos ? "" : keep_used(std::string_view(globals).substr(var_at + 4), used, true);
    std::string t = headline_ + "\n" + head;
    if (!text::trim(kept).empty()) t += "VAR\n" + kept;
    t += keep_used(externs, used, false) + keep_used(forwards, used, false);
    for (const std::string& f : pieces()) {
        if (const auto it = body.find(f); it != body.end()) t += it->second;
        else if (used.contains("P" + text::upper(std::string_view(f).substr(1, 5)))) t += stub(f, types);
    }
    return t + (body.contains("MAIN.pas") ? body["MAIN.pas"] : empty_);
}

std::vector<std::string> Compiland::names_of(const std::vector<std::string>& part) const {
    static const Regex heading(R"(\s*(?:PROCEDURE|FUNCTION) +P([0-9A-F]{5}))");
    std::set<std::string> out;
    for (const std::string& f : part) {
        if (f == "MAIN.pas") { out.insert("MAIN"); continue; }
        const std::string t = piece(f);
        for (const auto line : text::split(t, '\n')) {
            const auto m = heading.match(line);
            if (m && line.find("{STUB}") == std::string_view::npos && line.find("{TODO}") == std::string_view::npos && line.find("FORWARD") == std::string_view::npos)
                out.emplace(m->group(1));
        }
    }
    return {out.begin(), out.end()};
}

std::map<std::uint32_t, Procedure> procedures(const Project& project) {
    const Json j = json::load(project.dir / project.config.root / project.config.procedures);
    std::map<std::uint32_t, Procedure> out;
    for (const auto& [k, v] : j.items()) {
        const auto a = text::parse_int<std::uint32_t>(k, 16);
        if (!a) throw ParseError(project.config.procedures, 0, 0, std::format("'{}' is not an address", k));
        Procedure p;
        if (v.contains("size") && v["size"].is_number_integer()) p.size = v["size"].get<std::uint32_t>();
        if (v.contains("unit") && v["unit"].is_number_integer()) p.unit = v["unit"].get<std::int64_t>();
        out[*a] = p;
    }
    return out;
}

namespace {

// image[a, a+n) as the program has it, clamped to the image (a procedure may end at its end)
ByteView image_slice(ByteView image, std::uint64_t a, std::uint64_t n) {
    if (a >= image.size()) return {};
    return image.subspan(static_cast<std::size_t>(a), static_cast<std::size_t>(std::min<std::uint64_t>(n, image.size() - a)));
}

std::string note_text(const compare::Note& n) { return n.severity == compare::Severity::Warning ? "? " + n.text : n.text; }

}  // namespace

std::vector<Row> run(const Project& project, bool verbose) {
    static const Regex procedure_name(R"(P([0-9A-F]{5}))");
    const Toolchain& tc = *project.toolchain;
    const x86::Decoder16& decoder = project.decoder ? *project.decoder : x86::capstone_decoder();
    const std::map<std::uint32_t, Procedure> procs = procedures(project);
    std::vector<Row> rows;
    for (const auto& [name, u] : project.config.compilands) {
        const Compiland c(project, name, u);
        const auto unit_it = project.config.units.find(name);
        const std::optional<std::int64_t> unit = unit_it != project.config.units.end() ? std::optional<std::int64_t>(unit_it->second) : u.unit;
        std::optional<std::uint32_t> helper_at;   // (the entry helper: after them all)
        for (const auto& [a, p] : procs)
            if (p.unit == unit && p.size) helper_at = std::max(helper_at.value_or(0), a + *p.size);
        SymbolTable symbols = project.symbols;   // (each procedure is held to the project's table: what a comparison learns is taken out again)
        for (const std::vector<std::string>& part : c.parts()) {
            const std::string text_in = c.source(part);
            const CompileResult compiled = tc.compile(as_bytes(text_in), "T.PAS");
            const std::string label = name + " " + part.front();
            if (!compiled.object) {
                const auto last = text::lines(text::trim(compiled.console));
                rows.push_back({label, std::nullopt, std::nullopt, 0, Verdict::DoesNotCompile, last.empty() ? std::vector<std::string>{} : std::vector<std::string>{std::string(last.back())}});
                continue;
            }
            const omf::Object parsed = omf::read(*compiled.object);
            const ObjectFile o = object_of(parsed, *compiled.object);
            std::string seg;
            for (const omf::SegDef& s : parsed.segdefs) {
                static const std::set<std::string, std::less<>> kNotCode = {"HEAP", "STACK", "DATA", "CONST", "MEMORY"};
                const std::uint64_t* filled = parsed.filled.find(s.name);
                if (!kNotCode.contains(s.name) && filled && *filled) { seg = s.name; break; }
            }
            if (seg.empty()) throw FormatError("pascal_join", 0, name + ": the compiled module has no code");
            const std::vector<omf::FunctionSpan> fs = omf::functions(parsed, seg);
            std::map<std::int64_t, std::int64_t> length_at;   // (two names at one offset - a main program's block and ENTGQQ, the entry: each has the code that starts there)
            for (const auto& f : fs) length_at[f.offset] = std::max(length_at.contains(f.offset) ? length_at[f.offset] : 0, f.length);
            std::map<std::string, std::pair<std::int64_t, std::int64_t>> off_of;   // upper-case name -> (offset, length); lookup
            for (const auto& f : fs) off_of[text::upper(f.name)] = {f.offset, length_at[f.offset]};
            // where each compiled procedure stands in the program: P<ADDR> at ADDR, the block at main, the helper after
            struct Place { std::int64_t offset, length; std::optional<std::uint32_t> at; };
            std::vector<Place> places;
            for (const auto& f : fs) {
                const std::string upper = text::upper(f.name);
                std::optional<std::uint32_t> a;
                if (const auto m = procedure_name.full_match(upper)) {
                    if (!c.has_map() || c.maps(m->group(1))) a = c.address_of(m->group(1));
                } else if (upper == text::upper(name)) a = u.main_at;
                else if (f.name == "(helper)") a = helper_at;
                places.push_back({f.offset, length_at[f.offset], a});
            }
            const compare::Where where = [&places](std::uint32_t x) -> std::int64_t {
                for (const Place& p : places)
                    if (p.at && p.offset <= x && x < p.offset + std::max<std::int64_t>(p.length, 1)) return static_cast<std::int64_t>(*p.at) + x - p.offset;
                return -1;
            };
            for (const std::string& h : c.names_of(part)) {
                const std::string pn = h == "MAIN" ? text::upper(name) : "P" + h;
                if (h != "MAIN" && c.has_map() && !c.maps(h)) {
                    rows.push_back({label, pn, std::nullopt, 0, Verdict::NoPlace, {"not in the program (no place in the map)"}});
                    continue;
                }
                const std::uint32_t a = h == "MAIN" ? u.main_at : c.address_of(h);
                const auto found = off_of.find(pn);
                if (found == off_of.end()) {
                    rows.push_back({label, pn, a, 0, Verdict::NoPlace, {"not in the compiled code"}});
                    continue;
                }
                const auto [of, ln] = found->second;
                const auto proc = procs.find(a);
                const std::optional<std::uint32_t> size = proc == procs.end() ? std::nullopt : proc->second.size;
                const compare::Subject subject{o, pn, static_cast<std::uint32_t>(of), static_cast<std::uint32_t>(ln), a, seg};
                SymbolTable learnt;
                compare::Report report = compare::function(subject, project.image, project.program, symbols, learnt, where, decoder);
                for (const auto& entry : learnt) symbols.erase(entry.first);
                if (size && *size != ln) report.notes.push_back({compare::Severity::Error, std::format("{} bytes, the program's {}", ln, *size)});
                Verdict verdict = report.identical() ? Verdict::Identical : Verdict::Differs;
                std::vector<std::string> notes;
                for (const auto& n : report.notes) notes.push_back(note_text(n));
                const std::optional<std::string>& eq = u.equivalent ? u.equivalent : project.config.equivalent;
                if (verdict == Verdict::Differs && eq && !eq->empty() && size && *size != 0) {   // (the same code but for another compiler state's habits?)
                    std::set<std::uint32_t> holes;
                    for (const std::int64_t k : omf::holes(parsed, seg))
                        if (of <= k && k < of + ln) holes.insert(static_cast<std::uint32_t>(k - of));
                    const Bytes& code = *parsed.segments.find(seg);
                    const ByteView got = ByteView(code).subspan(static_cast<std::size_t>(of), std::min<std::size_t>(static_cast<std::size_t>(ln), code.size() - static_cast<std::size_t>(of)));
                    const ByteView want = image_slice(project.image, a, *size);
                    Equivalence result;
                    if (const auto fn = project.equivalents.find(*eq); fn != project.equivalents.end()) result = fn->second(want, got, holes, tc.helper_data());
                    else result.why = {"no equivalence comparer named " + *eq};
                    if (result.same) {
                        verdict = Verdict::Equivalent;
                        notes = {"the same code but for the habits of another compiler state: " + text::join(result.why, "; ").substr(0, 200)};
                    }
                }
                if (verbose)
                    out("{:<22} {:<10} {:05x} {} {}\n", label, pn, a, verdict_text(verdict), text::join(std::vector<std::string>(notes.begin(), notes.begin() + std::min<std::size_t>(2, notes.size())), "; "));
                rows.push_back({label, pn, a, static_cast<std::uint32_t>(ln), verdict, std::move(notes)});
            }
        }
    }
    return rows;
}

}  // namespace cb::exact::pascal_join
