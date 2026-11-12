#include "cb/exact/linkwhole.hpp"

#include <algorithm>
#include <format>
#include <set>

#include <nlohmann/json.hpp>

#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/log.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/exact/bytesearch.hpp"
#include "cb/exact/omf_object.hpp"
#include "cb/formats/omf.hpp"

namespace cb::exact::linkwhole {

namespace stdfs = std::filesystem;
namespace omf = cb::formats::omf;

namespace {

// ---- reading the "link" entry ------------------------------------------------------------------------------------

[[noreturn]] void bad_config(const std::string& what) { throw ParseError("project.json: link", 0, 0, what); }

std::string text_of(const Json& j, std::string_view key) {
    if (!j.is_string()) bad_config(std::format("\"{}\" is not text", key));
    return j.get<std::string>();
}

std::vector<std::string> texts_of(const Json& j, std::string_view key) {
    if (!j.is_array()) bad_config(std::format("\"{}\" is not a list", key));
    std::vector<std::string> out;
    for (const Json& x : j) out.push_back(text_of(x, key));
    return out;
}

// A number the project gives as a number or as text ("0x20").
std::uint32_t number_of(const Json& j, std::string_view key) {
    std::optional<std::int64_t> v;
    if (j.is_number_integer()) v = j.get<std::int64_t>();
    else if (j.is_string()) v = text::parse_int<std::int64_t>(j.get<std::string>(), 0);
    if (!v || *v < 0 || *v > 0xFFFFFFFFll) bad_config(std::format("\"{}\" is not a number", key));
    return static_cast<std::uint32_t>(*v);
}

NameScheme name_scheme_of(const std::string& text) {
    const std::size_t colon = text.find(':');
    const std::string_view kind = colon == std::string::npos ? std::string_view(text) : std::string_view(text).substr(0, colon);
    const std::string_view prefix = colon == std::string::npos ? std::string_view() : std::string_view(text).substr(colon + 1);
    NameScheme s;
    if (kind == "files") {
        s.kind = NameScheme::Kind::Files;
    } else if (kind == "pairs") {
        s.kind = NameScheme::Kind::Pairs;
        s.prefix = 'D';
    }
    if (!prefix.empty()) {
        if (s.kind != NameScheme::Kind::Files && prefix.size() != 1) bad_config("\"names\": the prefix is one letter");
        s.prefix = prefix[0];
    }
    return s;
}

Accept accept_of(const Json& j) {
    if (!j.is_object()) bad_config("\"accept\" holds objects");
    Accept a;
    if (j.contains("tail_zeros") && !j["tail_zeros"].is_null()) {
        const Json& t = j["tail_zeros"];
        Accept::TailZeros z;
        if (t.is_string() && t.get<std::string>() == "record") z.fill_record = true;
        else if (t.is_string() && t.get<std::string>().empty()) z.count = 0;   // (written as nothing: nothing accepted)
        else z.count = number_of(t, "tail_zeros");
        a.tail_zeros = z;
    }
    if (j.contains("header")) {
        const Json& h = j["header"];
        if (!h.is_array() || h.size() != 2) bad_config("\"header\" is [FROM, TO]");
        a.header = Accept::HeaderRange{number_of(h[0], "header"), number_of(h[1], "header")};
    }
    if (j.contains("why")) a.why = text_of(j["why"], "why");
    return a;
}

// ---- OMF records ---------------------------------------------------------------------------------------------------

constexpr std::uint8_t kLnames = 0x96, kSegdef = 0x98, kModend = 0x8A, kModend32 = 0x8B;

// An OMF index at q: the index (one byte, or two when the first has its high bit set) and where it ends.
struct Index {
    std::uint32_t value = 0;
    std::size_t end = 0;
};
Index omf_index(ByteView b, std::size_t q) {
    if (q >= b.size()) throw TruncatedError("link: reclass", q, 1, 0);
    if (b[q] & 0x80) {
        if (q + 1 >= b.size()) throw TruncatedError("link: reclass", q, 2, 1);
        return {static_cast<std::uint32_t>(((b[q] & 0x7F) << 8) | b[q + 1]), q + 2};
    }
    return {b[q], q + 1};
}

std::uint32_t le16(ByteView b, std::size_t at) { return static_cast<std::uint32_t>(b[at] | (b[at + 1] << 8)); }

}  // namespace

LinkConfig parse_link_config(std::string_view json_text) {
    LinkConfig c;
    if (text::trim(json_text).empty()) return c;
    const Json j = json::parse(json_text);
    if (j.is_null()) return c;
    if (!j.is_object()) bad_config("the entry is not an object");
    if (j.contains("linker")) {
        if (!j["linker"].is_null()) c.linker = text_of(j["linker"], "linker");
    }
    if (j.contains("emulate") && !j["emulate"].is_null()) {
        const std::string e = text_of(j["emulate"], "emulate");
        if (!e.empty()) c.emulate = e;
    }
    if (j.contains("order") && !j["order"].is_null()) c.order = text_of(j["order"], "order");
    if (j.contains("switches")) c.switches = text_of(j["switches"], "switches");
    if (j.contains("output")) c.output = text_of(j["output"], "output");
    if (j.contains("map")) {
        const Json& m = j["map"];
        c.map = m.is_boolean() ? m.get<bool>() : !(m.is_null() || (m.is_number() && m.get<double>() == 0) || (m.is_string() && m.get<std::string>().empty()));
    }
    if (j.contains("names") && !j["names"].is_null()) {
        const std::string n = text_of(j["names"], "names");
        if (!n.empty()) c.names = name_scheme_of(n);
    }
    if (j.contains("placed") && !j["placed"].is_null()) c.placed = text_of(j["placed"], "placed");
    if (j.contains("last")) c.last = texts_of(j["last"], "last");
    if (j.contains("find")) {
        for (const Json& x : j["find"]) {
            if (!x.is_object() || !x.contains("file") || !x.contains("toolchain")) bad_config("\"find\" holds {file, toolchain, variant}");
            Find f{text_of(x["file"], "file"), text_of(x["toolchain"], "toolchain"), std::nullopt};
            if (x.contains("variant") && !x["variant"].is_null()) f.variant = text_of(x["variant"], "variant");
            c.find.push_back(std::move(f));
        }
    }
    if (j.contains("first")) {
        for (const Json& x : j["first"]) {
            if (!x.is_array() || x.size() != 2) bad_config("\"first\" holds [NAME, file]");
            c.first.push_back({text_of(x[0], "first"), text_of(x[1], "first")});
        }
    }
    if (j.contains("libraries")) c.libraries = texts_of(j["libraries"], "libraries");
    if (j.contains("class")) {
        if (!j["class"].is_object()) bad_config("\"class\" is not an object");
        for (const auto& [cls, spec] : j["class"].items()) {
            ClassSpec s;
            if (spec.contains("segments")) s.segments = texts_of(spec["segments"], "segments");
            if (spec.contains("files")) s.files = texts_of(spec["files"], "files");
            c.classes.insert_or_assign(cls, std::move(s));
        }
    }
    if (j.contains("accept")) {
        if (!j["accept"].is_array()) bad_config("\"accept\" is not a list");
        for (const Json& x : j["accept"]) c.accept.push_back(accept_of(x));
    }
    if (j.contains("psp") && !j["psp"].is_null()) c.psp = number_of(j["psp"], "psp");
    return c;
}

bool wanted(const LinkConfig& config) { return config.linker.has_value() || config.emulate.has_value(); }

namespace {

// What a compiler said, cut to the end of it that holds the error.
std::string console_tail(const std::string& console) {
    return console.size() > 1500 ? console.substr(console.size() - 1500) : console;
}

[[noreturn]] void did_not_compile(std::string_view file, const std::string& console) {
    throw LinkError(std::format("{} does not compile:\n{}", file, console_tail(console)));
}

Bytes compile_file(const modules::Toolchain& tc, const stdfs::path& file, const Project& part, std::string_view name) {
    const modules::CompileOptions options{part.headers(), part.include(), part.switches()};
    modules::CompileResult r = tc.compile(fs::read_file(file), options);
    if (!r.object) did_not_compile(name, r.console);
    return std::move(*r.object);
}

}  // namespace

std::vector<ListedModule> listed(const Project& project, const LinkConfig& config) {
    std::vector<ListedModule> out;
    if (!config.order) return out;
    const auto sets = project.sets();
    const std::string order_text = fs::read_text(project.dir() / *config.order);
    for (std::string_view line : text::lines(order_text)) {
        if (text::split_ws(line).empty() || line.starts_with('#')) continue;
        const auto fields = text::split_ws(line.substr(0, line.find('#')));
        if (fields.empty()) continue;
        const std::string name(fields[0]);
        const Project* part = nullptr;
        for (const auto& p : sets) {
            if (stdfs::exists(p->src() / name)) {
                part = p.get();
                break;
            }
        }
        if (part == nullptr) throw LinkError(name + ": no source set has it");
        ListedModule m;
        m.index = out.size();
        m.name = name;
        m.object = compile_file(*part->toolchain(), part->src() / name, *part, name);
        m.odd = std::find(fields.begin() + 1, fields.end(), "odd") != fields.end();
        out.push_back(std::move(m));
    }
    return out;
}

std::optional<std::uint32_t> place_of(const Project& project, ByteView object) {
    const omf::Object o = omf::read(object);
    std::vector<std::string> cs = omf::code_segments(o);
    if (cs.empty()) {   // (no segment says it is code: the ones that hold bytes)
        for (const omf::SegDef& s : o.segdefs) {
            const std::uint64_t* filled = o.filled.find(s.name);
            if (filled != nullptr && *filled != 0) cs.push_back(s.name);
        }
    }
    if (cs.empty()) return std::nullopt;
    const Bytes* seg = o.segments.find(cs[0]);
    const std::uint64_t* filled = o.filled.find(cs[0]);
    if (seg == nullptr) return std::nullopt;
    const std::size_t len = std::min<std::size_t>(seg->size(), filled ? *filled : 0);
    const ByteView code(seg->data(), len);
    const std::set<std::int64_t> hole = omf::holes(o, cs[0]);
    const auto is_hole = [&](std::size_t x) { return hole.contains(static_cast<std::int64_t>(x)); };
    // The longest run with no hole, to look for (of equal runs the later).
    std::size_t best_len = 0, best_at = 0;
    for (std::size_t r = 0; r < len;) {
        std::size_t e = r;
        while (e < len && !is_hole(e)) ++e;
        if (e - r >= best_len) {
            best_len = e - r;
            best_at = r;
        }
        r = e + 1;
    }
    if (best_len < 8) return std::nullopt;
    const ByteView img = project.image();
    const ByteView key = code.subspan(best_at, best_len);
    std::vector<std::size_t> hits;
    for (std::size_t k = find_bytes(img, key); k != img.size(); k = find_bytes(img, key, k + 1)) {
        if (k < best_at) continue;
        const std::size_t a = k - best_at;
        if (a + len > img.size()) continue;
        bool same = true;
        for (std::size_t x = 0; x < len && same; ++x) same = is_hole(x) || img[a + x] == code[x];
        if (same) hits.push_back(a);
    }
    if (hits.size() != 1) return std::nullopt;
    return static_cast<std::uint32_t>(hits[0]);
}

std::vector<PlacedObject> placed_objects(const Project& project) {
    const LinkConfig& lk = project.link();
    // A file with the toolchain that makes it, or its object already (placed by its bytes).
    struct Entry {
        std::uint32_t place = 0;
        stdfs::path file;
        const Project* part = nullptr;               // null: compiled already
        std::shared_ptr<const modules::Toolchain> tc;
        Bytes object;
    };
    std::vector<Entry> entries;
    const auto sets = project.sets();
    for (const auto& part : sets) {
        const auto tc = part->toolchain();
        for (const auto& [at, path] : part->sources()) entries.push_back({at, path, part.get(), tc, {}});
    }
    const Project& main = *sets.at(0);
    if (lk.placed) {
        const Regex pattern(*lk.placed, RegexFlags::IgnoreCase);   // (the project's own pattern: built once)
        std::vector<std::string> names;
        for (const auto& e : stdfs::directory_iterator(main.src())) names.push_back(e.path().filename().string());
        std::sort(names.begin(), names.end());
        for (const std::string& f : names) {
            const auto m = pattern.full_match(f);
            if (!m || !m->has(1)) continue;
            const auto at = text::parse_int<std::uint32_t>(m->group(1), 16);
            if (!at) throw LinkError(f + ": the place in its name is not a hexadecimal number");
            entries.push_back({*at, main.src() / f, &main, main.toolchain(), {}});
        }
    }
    for (const Find& x : lk.find) {
        const auto tc = project.load_toolchain(x.toolchain, x.variant);
        const stdfs::path path = main.src() / x.file;
        const modules::CompileResult r = tc->compile(fs::read_file(path), {main.headers(), main.include(), std::nullopt});
        if (!r.object) throw LinkError(x.file + ": does not compile");
        const auto at = place_of(project, *r.object);
        if (!at) throw LinkError(x.file + ": its code is not found in the program");
        entries.push_back({*at, path, nullptr, nullptr, *r.object});
    }
    std::stable_sort(entries.begin(), entries.end(), [](const Entry& a, const Entry& b) { return a.place < b.place; });
    std::uint32_t k = 0;
    for (const std::string& f : lk.last) entries.push_back({kLastPlace + k++, main.src() / f, &main, main.toolchain(), {}});
    std::vector<PlacedObject> objs;
    for (Entry& e : entries) {
        const std::string name = e.file.filename().string();
        if (e.part == nullptr) {
            objs.push_back({e.place, name, std::move(e.object)});
        } else {
            objs.push_back({e.place, name, compile_file(*e.tc, e.file, *e.part, name)});
        }
    }
    return objs;
}

ByteView own_records(ByteView object) {
    std::size_t p = 0;
    while (p + 3 <= object.size()) {
        const std::uint8_t type = object[p];
        p += 3 + le16(object, p + 1);
        if (type == kModend || type == kModend32) return object.first(std::min(p, object.size()));
    }
    return object;
}

std::string object_name(const std::optional<NameScheme>& scheme, std::size_t k, std::string_view file) {
    const NameScheme s = scheme.value_or(NameScheme{});
    std::string name;
    switch (s.kind) {
        case NameScheme::Kind::Files: {
            const std::string_view stem = file.substr(0, file.rfind('.') == std::string_view::npos ? file.size() : file.rfind('.'));
            for (char c : stem) {
                const char u = (c >= 'a' && c <= 'z') ? static_cast<char>(c - 'a' + 'A') : c;
                if ((u >= 'A' && u <= 'Z') || (u >= '0' && u <= '9')) name.push_back(u);
            }
            if (name.size() > 8) name.resize(8);
            return name;
        }
        case NameScheme::Kind::Pairs:
            name.push_back(static_cast<char>(s.prefix + k / 26));
            name.push_back(static_cast<char>('A' + k % 26));
            return name;
        case NameScheme::Kind::Sequence: break;
    }
    name.push_back(s.prefix);
    if (k < 26) name.push_back(static_cast<char>('A' + k));
    else if (k < 36) name.push_back(static_cast<char>('0' + (k - 26)));
    else name += k < 10 ? "0" + std::to_string(k) : std::to_string(k);
    return name;
}

Bytes reclass(ByteView obj, std::string_view cls, const std::vector<std::string>& segments) {
    Bytes out;
    std::vector<std::string> names;   // the LNAMES, in order: an index is a place in this list
    std::optional<std::size_t> added;   // the index the class's name was given (in the first LNAMES)
    std::set<std::string> want;
    for (const std::string& s : segments) want.insert(text::upper(s));
    std::size_t i = 0;
    while (i + 3 <= obj.size()) {
        const std::uint8_t type = obj[i];
        const std::size_t n = le16(obj, i + 1);
        if (n == 0) throw FormatError("link: reclass", i, "a record of no length");
        if (i + 3 + n > obj.size()) throw TruncatedError("link: reclass", i, 3 + n, obj.size() - i);
        Bytes body(obj.begin() + static_cast<std::ptrdiff_t>(i + 3), obj.begin() + static_cast<std::ptrdiff_t>(i + 2 + n));   // (the checksum left off)
        i += 3 + n;
        if (type == kLnames) {   // the class's name added to the first
            for (std::size_t p = 0; p < body.size();) {
                const std::size_t len = body[p];
                if (p + 1 + len > body.size()) throw FormatError("link: reclass", i, "a name runs past the end of its record");
                names.push_back(text::from_latin1(ByteView(body).subspan(p + 1, len)));
                p += 1 + len;
            }
            if (!added) {
                body.push_back(static_cast<std::uint8_t>(cls.size()));
                const Bytes name = text::to_latin1(cls);
                append(body, name);
                names.emplace_back(cls);
                added = names.size();
            }
        } else if (type == kSegdef && added && !body.empty()) {   // SEGDEF: ACBP [frame offset] length name class overlay
            const std::size_t p = 1 + ((body[0] >> 5) == 0 ? 3 : 0) + 2;
            const Index nm = omf_index(body, p);
            const std::string name = (nm.value > 0 && nm.value <= names.size()) ? text::upper(names[nm.value - 1]) : std::string();
            const bool wanted_segment =
                want.contains(name) || std::any_of(want.begin(), want.end(), [&](const std::string& s) { return name.ends_with("_" + s); });
            if (wanted_segment) {
                if (nm.end >= body.size()) throw FormatError("link: reclass", i, "a SEGDEF without a class");
                if ((body[nm.end] & 0x80) || *added > 0x7F) throw UnsupportedError("link: reclass", 0, "a two-byte name index is not handled");
                body[nm.end] = static_cast<std::uint8_t>(*added);
            }
        }
        const Bytes rec = [&] {
            Bytes r{type, static_cast<std::uint8_t>((body.size() + 1) & 0xFF), static_cast<std::uint8_t>((body.size() + 1) >> 8)};
            append(r, body);
            return r;
        }();
        append(out, rec);
        std::uint8_t sum = 0;
        for (std::uint8_t b : rec) sum = static_cast<std::uint8_t>(sum + b);
        out.push_back(static_cast<std::uint8_t>(-sum));
    }
    return out;
}

LinkOutput link(const Project& project, const std::vector<PlacedObject>& objects, const DosHost& dos, const std::optional<stdfs::path>& keep) {
    const LinkConfig& lk = project.link();
    if (!lk.linker) throw LinkError("the project names no linker to run");
    std::optional<fs::ScopedTempDir> temp;
    if (keep) fs::make_dirs(*keep);
    else temp.emplace("exlinkw");
    const stdfs::path w = keep ? *keep : temp->path();
    const stdfs::path linker = project.resolve(*lk.linker);
    stdfs::copy_file(linker, w / "LINK.EXE", stdfs::copy_options::overwrite_existing);
    std::vector<std::string> names;
    for (const FirstObject& f : lk.first) {
        stdfs::copy_file(project.resolve(f.file), w / (f.name + ".OBJ"), stdfs::copy_options::overwrite_existing);
        names.push_back(f.name);
    }
    // file -> (class, segments): a later class wins for a file two name
    std::map<std::string, const ClassSpec*> class_of;
    std::map<std::string, std::string> class_name_of;
    for (const auto& [cls, spec] : lk.classes) {
        for (const std::string& f : spec.files) {
            class_of[f] = &spec;
            class_name_of[f] = cls;
        }
    }
    for (std::size_t k = 0; k < objects.size(); ++k) {
        const PlacedObject& o = objects[k];
        Bytes raw(own_records(o.object).begin(), own_records(o.object).end());
        if (const auto it = class_of.find(o.name); it != class_of.end()) raw = reclass(raw, class_name_of[o.name], it->second->segments);
        const std::string n = object_name(lk.names, k, o.name);
        names.push_back(n);
        fs::write_file(w / (n + ".OBJ"), raw);
    }
    std::vector<std::string> libs;
    for (const std::string& f : lk.libraries) {
        const stdfs::path from = project.resolve(f);
        stdfs::copy_file(from, w / from.filename(), stdfs::copy_options::overwrite_existing);
        const std::size_t slash = f.find_last_of("/\\");
        std::string base = slash == std::string::npos ? f : f.substr(slash + 1);
        base = base.substr(0, base.rfind('.') == std::string::npos ? base.size() : base.rfind('.'));
        libs.push_back(text::upper(base));
    }
    const std::string& out = lk.output;
    std::string cmd = text::join(names, "+") + "," + out + "," + (lk.map ? out : std::string("NUL"));
    if (!libs.empty()) cmd += "," + text::join(libs, "+");
    cmd += lk.switches + ";";
    std::string console;
    if (cmd.size() > 120) {   // (a DOS command line holds 127 characters: the answers go in a response file, one a line)
        std::string body = cmd;
        while (!body.empty() && body.back() == ';') body.pop_back();
        fs::write_text(w / "L.LNK", text::join(text::split(body, ','), "\r\n") + ";\r\n");
        console = dos.run(w / "LINK.EXE", w, "@L.LNK", lk.psp);
    } else {
        console = dos.run(w / "LINK.EXE", w, cmd, lk.psp);
    }
    LinkOutput result;
    result.console = text::replace_all(console, "\r", "");
    if (const stdfs::path exe = w / (out + ".EXE"); stdfs::exists(exe)) result.exe = fs::read_file(exe);
    if (const stdfs::path map = w / (out + ".MAP"); stdfs::exists(map)) result.map = fs::read_text(map);
    return result;
}

Comparison compare(const LinkConfig& config, ByteView ref, ByteView exe) {
    if (equal_bytes(exe, ref)) return {true, {}};
    std::set<std::size_t> skip;
    Comparison result;
    std::size_t n = exe.size();
    for (const Accept& a : config.accept) {
        if (a.tail_zeros) {
            std::size_t tz = a.tail_zeros->count;
            if (a.tail_zeros->fill_record) tz = (128 - exe.size() % 128) % 128;
            if (tz != 0 && ref.size() == exe.size() + tz &&
                std::all_of(ref.begin() + static_cast<std::ptrdiff_t>(exe.size()), ref.end(), [](std::uint8_t b) { return b == 0; })) {
                n = ref.size();
                result.notes.push_back(std::format("the original has {} zero bytes more at its end ({})", tz, a.why));
            }
        }
        if (a.header) {
            const std::size_t lo = a.header->from, hi = a.header->to, common = std::min(ref.size(), exe.size());
            bool differs = false;
            for (std::size_t k = lo; k < hi && k < common; ++k) differs = differs || ref[k] != exe[k];
            if (differs) {
                for (std::size_t k = lo; k < hi; ++k) skip.insert(k);
                skip.insert(0x12);
                skip.insert(0x13);
                result.notes.push_back(std::format("header bytes {:02x}..{:02x} and the checksum ({})", lo, hi - 1, a.why));
            }
        }
    }
    if (n != ref.size()) return {false, {std::format("the file is {} bytes, the original {}", exe.size(), ref.size())}};
    std::vector<std::size_t> bad;
    for (std::size_t k = 0; k < std::min(exe.size(), ref.size()); ++k) {
        if (!skip.contains(k) && exe[k] != ref[k]) bad.push_back(k);
    }
    if (!bad.empty()) {
        const std::size_t header = ref.size() >= 10 ? le16(ref, 8) * 16u : 0;   // (the program's header, in bytes)
        const std::string where = bad[0] < header ? "the header" : std::format("image {:05x}", bad[0] - header);
        return {false, {std::format("{} bytes differ, the first at file offset {:05x} ({})", bad.size(), bad[0], where)}};
    }
    return {true, std::move(result.notes)};
}

int run(const Project& project, const std::vector<std::string>& args, const DosHost& dos) {
    const LinkConfig& lk = project.link();
    std::optional<stdfs::path> keep;
    bool verbose = false;
    for (std::size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "--keep" && i + 1 < args.size()) keep = fs::from_text(args[++i]);
        else if (args[i] == "--keep") throw UsageError("--keep needs a folder");
        else if (args[i] == "-v") verbose = true;
    }
    const std::string exe_name = stdfs::path(project.exe()).filename().string();
    const auto report = [&](const Comparison& c, std::size_t modules, std::string_view how, const Bytes& exe) {
        for (const std::string& l : c.notes) out("   {}\n", l);
        const bool same = equal_bytes(exe, project.file());
        out("{} linked from {} modules{}: {} bytes; {}\n", exe_name, modules, how, exe.size(),
            c.identical ? (same ? "the FILE IDENTICAL" : "identical but for what the project accepts") : "not identical");
        return c.identical ? 0 : 1;
    };
    if (lk.emulate) {   // (a linker done here: exact/linkemu)
        if (*lk.emulate != "link24") throw UnsupportedError("link", 0, "no linker called " + *lk.emulate + " is done here");
        std::vector<linkemu::LinkInput> inputs;
        if (lk.order) {
            for (ListedModule& m : listed(project, lk)) inputs.push_back({m.name, read_object(m.object), m.odd});
        } else {
            for (const PlacedObject& o : placed_objects(project)) inputs.push_back({o.name, read_object(o.object), false});
        }
        const std::size_t count = inputs.size();
        const linkemu::Log log = verbose ? linkemu::Log([](std::string_view s) { out("{}\n", s); }) : linkemu::Log();
        const linkemu::LinkResult linked = linkemu::link24(inputs, project.symbols(), log);
        return report(compare(lk, project.file(), linked.exe), count, std::format(" ({}, done here)", *lk.emulate), linked.exe);
    }
    const std::vector<PlacedObject> objs = placed_objects(project);
    if (verbose) {
        for (const PlacedObject& o : objs) out("   {:05x} {}\n", o.place < kLastPlace ? o.place : 0, o.name);
    }
    const LinkOutput linked = link(project, objs, dos, keep);
    int shown = 0;
    for (std::string_view l : text::split(linked.console, '\n')) {
        if (text::trim(l).empty() || l.find("Microsoft") != l.npos || l.find("Copyright") != l.npos || l.find("Version") != l.npos) continue;
        if (shown++ < 12) out("   LINK: {}\n", l);
    }
    if (!linked.exe) {
        out("no program was written\n");
        return 1;
    }
    return report(compare(lk, project.file(), *linked.exe), objs.size(), "", *linked.exe);
}

}  // namespace cb::exact::linkwhole
