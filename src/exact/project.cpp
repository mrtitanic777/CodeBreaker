#include "exact/project.hpp"

#include <algorithm>
#include <cctype>
#include <format>

#include <nlohmann/json.hpp>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/regex.hpp"
#include "core/text.hpp"

namespace cb::exact::project {

namespace stdfs = std::filesystem;

namespace {

// ---- project.json -> Config ---------------------------------------------------------------------------------------------

[[noreturn]] void prj_bad(const std::string& what) { throw ParseError("project.json", 0, 0, what); }

// A number written as a number or as text ("0x20").
std::int64_t prj_number(const Json& v, std::string_view key) {
    if (v.is_number_integer()) return v.get<std::int64_t>();
    if (v.is_string()) {
        if (const auto n = text::parse_int<std::int64_t>(v.get_ref<const std::string&>(), 0)) return *n;
    }
    prj_bad(std::format("\"{}\" is not a number", key));
}

std::uint32_t prj_u32(const Json& v, std::string_view key) {
    const std::int64_t n = prj_number(v, key);
    if (n < 0 || n > 0xFFFFFFFFLL) prj_bad(std::format("\"{}\" is out of range", key));
    return static_cast<std::uint32_t>(n);
}

std::string prj_string(const Json& v, std::string_view key) {
    if (!v.is_string()) prj_bad(std::format("\"{}\" is not a string", key));
    return v.get<std::string>();
}

const Json* prj_find(const Json& o, std::string_view key) {
    const auto it = o.find(std::string(key));
    return it == o.end() || it->is_null() ? nullptr : &*it;
}

std::optional<std::string> prj_opt_text(const Json& o, std::string_view key) {
    const Json* v = prj_find(o, key);
    return v ? std::optional<std::string>(prj_string(*v, key)) : std::nullopt;
}

// An entry that names a file: the empty text is none.
std::optional<std::string> prj_opt_file(const Json& o, std::string_view key) {
    auto t = prj_opt_text(o, key);
    if (t && t->empty()) return std::nullopt;
    return t;
}

std::vector<std::string> prj_texts(const Json& o, std::string_view key) {
    std::vector<std::string> out;
    if (const Json* v = prj_find(o, key)) {
        if (!v->is_array()) prj_bad(std::format("\"{}\" is not a list", key));
        for (const Json& x : *v) out.push_back(prj_string(x, key));
    }
    return out;
}

std::vector<std::uint32_t> prj_numbers(const Json& o, std::string_view key) {
    std::vector<std::uint32_t> out;
    if (const Json* v = prj_find(o, key)) {
        if (!v->is_array()) prj_bad(std::format("\"{}\" is not a list", key));
        for (const Json& x : *v) out.push_back(prj_u32(x, key));
    }
    return out;
}

std::vector<LibraryRef> prj_libraries(const Json& v, std::string_view key) {
    std::vector<LibraryRef> out;
    if (v.is_null()) return out;
    if (!v.is_array()) prj_bad(std::format("\"{}\" is not a list", key));
    for (const Json& x : v) {
        if (!x.is_object()) prj_bad(std::format("an entry of \"{}\" is not an object", key));
        const Json* path = prj_find(x, "path");
        if (!path) prj_bad(std::format("an entry of \"{}\" has no \"path\"", key));
        out.push_back({prj_opt_text(x, "tag").value_or(""), prj_string(*path, "path")});
    }
    return out;
}

pascal_join::Config prj_pascal(const Json& d) {
    pascal_join::Config c;
    if (const auto root = prj_opt_text(d, "root")) c.root = *root;
    c.over = prj_texts(d, "over");
    c.map = prj_opt_file(d, "map");
    c.kinds = prj_opt_file(d, "kinds");
    c.procedures = prj_opt_text(d, "procedures").value_or("");
    c.equivalent = prj_opt_text(d, "equivalent");
    if (const Json* units = prj_find(d, "units")) {
        if (!units->is_object()) prj_bad("\"units\" is not an object");
        for (const auto& [name, v] : units->items()) c.units[name] = prj_number(v, "units");
    }
    if (const Json* cs = prj_find(d, "compilands")) {
        if (!cs->is_object()) prj_bad("\"compilands\" is not an object");
        for (const auto& [name, u] : cs->items()) {
            if (!u.is_object()) prj_bad(std::format("compiland {} is not an object", name));
            pascal_join::CompilandConfig x;
            x.dir = prj_opt_text(u, "dir").value_or("");
            const auto kind = prj_opt_text(u, "kind");
            if (!kind) prj_bad(std::format("compiland {} has no \"kind\"", name));
            x.kind = *kind == "PROGRAM" ? pascal_join::CompilandKind::Program : pascal_join::CompilandKind::Module;
            if (const Json* m = prj_find(u, "main")) x.main_at = prj_u32(*m, "main");
            if (const Json* p = prj_find(u, "parts")) x.parts = static_cast<int>(prj_number(*p, "parts"));
            if (prj_find(u, "over")) x.over = prj_texts(u, "over");
            if (prj_find(u, "dirs")) x.dirs = prj_texts(u, "dirs");
            if (const Json* un = prj_find(u, "unit")) x.unit = prj_number(*un, "unit");
            if (const Json* sc = prj_find(u, "stackck")) x.stackck = sc->is_boolean() ? sc->get<bool>() : prj_number(*sc, "stackck") != 0;
            x.equivalent = prj_opt_text(u, "equivalent");
            c.compilands.insert_or_assign(name, std::move(x));
        }
    }
    return c;
}

bool prj_flag(const Json& o, std::string_view key) {
    const Json* v = prj_find(o, key);
    if (!v) return false;
    if (v->is_boolean()) return v->get<bool>();
    if (v->is_number_integer()) return v->get<std::int64_t>() != 0;
    if (v->is_string()) return !v->get_ref<const std::string&>().empty();
    return true;
}

Config prj_from_json(const Json& d) {
    if (!d.is_object()) prj_bad("a project is a JSON object");
    Config c;
    const Json* image = prj_find(d, "image");
    if (!image) throw FormatError("project.json", 0, "no \"image\": the original program");
    c.image = prj_string(*image, "image");
    if (const auto s = prj_opt_text(d, "src")) c.src = *s;
    c.include = prj_texts(d, "include");
    if (const Json* v = prj_find(d, "dgroup")) c.dgroup = prj_number(*v, "dgroup");
    if (const Json* v = prj_find(d, "code_segments")) {
        if (v->is_string()) {
            if (v->get_ref<const std::string&>() != "relocs") prj_bad("\"code_segments\" is a list of numbers or \"relocs\"");
            c.code_segments_from_relocs = true;
        } else {
            c.code_segments = prj_numbers(d, "code_segments");
        }
    }
    if (const Json* v = prj_find(d, "loadseg")) c.loadseg = prj_u32(*v, "loadseg");
    if (const Json* v = prj_find(d, "code_end")) c.code_end = prj_u32(*v, "code_end");
    if (const Json* v = prj_find(d, "code_start")) c.code_start = prj_u32(*v, "code_start");
    if (const Json* v = prj_find(d, "data_end")) c.data_end = prj_u32(*v, "data_end");
    if (const auto s = prj_opt_text(d, "code")) c.code = *s;
    c.switches = prj_opt_text(d, "switches");
    c.starts = prj_opt_file(d, "starts");
    c.starts_add = prj_numbers(d, "starts_add");
    c.starts_drop = prj_numbers(d, "starts_drop");
    c.packed = prj_opt_file(d, "packed");
    if (const auto s = prj_opt_text(d, "sources")) c.sources = *s;
    c.place_by = prj_opt_file(d, "place_by");
    c.unplaced = prj_opt_file(d, "unplaced");
    c.names = prj_opt_text(d, "names");
    c.named_by = prj_opt_text(d, "named_by");
    c.first_segment = prj_opt_text(d, "first_segment");
    c.toolchain = prj_opt_text(d, "toolchain");
    c.variant = prj_opt_text(d, "variant");
    if (const Json* ts = prj_find(d, "toolchains")) {
        if (!ts->is_array()) prj_bad("\"toolchains\" is not a list");
        for (const Json& x : *ts) {
            const Json* name = prj_find(x, "toolchain");
            if (!name) prj_bad("an entry of \"toolchains\" has no \"toolchain\"");
            ToolchainSpec t;
            t.toolchain = prj_string(*name, "toolchain");
            t.label = prj_opt_text(x, "label").value_or("");
            if (t.label.empty()) t.label = t.toolchain;
            t.variant = prj_opt_text(x, "variant");
            c.toolchains.push_back(std::move(t));
        }
    }
    c.whole = prj_flag(d, "whole");
    c.recut = prj_flag(d, "recut");
    c.link_order = prj_opt_text(d, "link_order");
    if (const auto s = prj_opt_text(d, "model")) c.model = *s;
    if (const Json* v = prj_find(d, "library_gap")) c.library_gap = prj_u32(*v, "library_gap");
    if (const Json* v = prj_find(d, "library_start")) c.library_start = prj_u32(*v, "library_start");
    if (const Json* v = prj_find(d, "libraries")) c.libraries = prj_libraries(*v, "libraries");
    c.first_objects = prj_texts(d, "first_objects");
    c.after_library = prj_texts(d, "after_library");
    if (const Json* v = prj_find(d, "data_sizes")) {
        if (!v->is_object()) prj_bad("\"data_sizes\" is not an object");
        for (const auto& [name, n] : v->items()) c.data_sizes[name] = prj_u32(n, "data_sizes");
    }
    if (const Json* v = prj_find(d, "data_pads")) {
        if (!v->is_object()) prj_bad("\"data_pads\" is not an object");
        for (const auto& [at, n] : v->items()) {
            const auto address = text::parse_int<std::uint32_t>(at, 16);
            if (!address) prj_bad(std::format("\"data_pads\": {} is not a hexadecimal offset", at));
            c.data_pads.push_back({*address, prj_u32(n, "data_pads")});
        }
        std::ranges::sort(c.data_pads, {}, &library::Project::DataPad::address);
    }
    if (const Json* v = prj_find(d, "also")) {
        if (!v->is_array()) prj_bad("\"also\" is not a list");
        for (const Json& x : *v) c.also.push_back(x.dump());
    }
    if (const Json* v = prj_find(d, "link")) {
        if (v->is_object()) c.link = linkwhole::parse_link_config(v->dump());
    }
    c.pascal = prj_pascal(d);
    for (const auto& [key, value] : d.items())
        if (value.is_string()) c.strings[key] = value.get<std::string>();
    return c;
}

// ---- the toolchain profile's parts ---------------------------------------------------------------------------------------

std::vector<std::string> prj_string_list(const Json& v, std::string_view key) {
    std::vector<std::string> out;
    if (!v.is_array()) throw FormatError("toolchain profile", 0, std::format("\"{}\" is not a list", key));
    for (const Json& x : v) {
        if (!x.is_string()) throw FormatError("toolchain profile", 0, std::format("\"{}\" holds a non-text entry", key));
        out.push_back(x.get<std::string>());
    }
    return out;
}

std::optional<std::string> prj_profile_text(const Json& o, std::string_view key) {
    const auto it = o.find(std::string(key));
    if (it == o.end() || it->is_null()) return std::nullopt;
    if (!it->is_string()) throw FormatError("toolchain profile", 0, std::format("\"{}\" is not a string", key));
    return it->get<std::string>();
}

std::vector<LibraryRef> prj_profile_libraries(const Json& v) {
    std::vector<LibraryRef> out;
    if (!v.is_array()) throw FormatError("toolchain profile", 0, "a library list is not a list");
    for (const Json& x : v) {
        const auto path = prj_profile_text(x, "path");
        if (!path) throw FormatError("toolchain profile", 0, "a library has no \"path\"");
        out.push_back({prj_profile_text(x, "tag").value_or(""), *path});
    }
    return out;
}

bool prj_profile_flag(const Json& o, std::string_view key, bool otherwise) {
    const auto it = o.find(std::string(key));
    if (it == o.end()) return otherwise;
    if (it->is_boolean()) return it->get<bool>();
    if (it->is_number_integer()) return it->get<std::int64_t>() != 0;
    return !it->is_null();
}

// The lines of a text file: \n and \r\n end them.
std::vector<std::string_view> prj_lines(const std::string& text) { return text::lines(text); }
std::vector<std::string_view> prj_lines(std::string&&) = delete;   // (the lines are views: a temporary would be gone)

// The names of the files in a folder, sorted. NotFoundError when there is no such folder.
std::vector<std::string> prj_names_in(const stdfs::path& dir) {
    std::error_code ec;
    stdfs::directory_iterator it(dir, ec);
    if (ec) throw NotFoundError(dir, ec.value(), "cannot list the folder");
    std::vector<std::string> out;
    for (const stdfs::directory_iterator end; it != end; it.increment(ec)) {
        if (ec) throw IoError(dir, ec.value(), "cannot list the folder");
        out.push_back(fs::to_text(it->path().filename()));
    }
    std::ranges::sort(out);
    return out;
}

// The text with its /* ... */ comments left out (one that is not closed stays).
std::string prj_without_comments(std::string_view text) {
    std::string out;
    out.reserve(text.size());
    for (std::size_t i = 0; i < text.size();) {
        if (text.compare(i, 2, "/*") == 0) {
            const std::size_t close = text.find("*/", i + 2);
            if (close != std::string_view::npos) {
                i = close + 2;
                continue;
            }
        }
        out += text[i++];
    }
    return out;
}

std::string prj_hex4(std::int64_t v) { return v < 0 ? std::format("-{:04x}", static_cast<std::uint64_t>(-v)) : std::format("{:04x}", static_cast<std::uint64_t>(v)); }

bool prj_all_hex(std::string_view s) {
    return !s.empty() && std::ranges::all_of(s, [](unsigned char c) { return std::isxdigit(c) != 0; });
}

std::uint16_t prj_word(ByteView b, std::size_t at) { return static_cast<std::uint16_t>(b[at] | (b[at + 1] << 8)); }

}  // namespace

Config parse_config(std::string_view json_text) { return prj_from_json(json::parse(json_text)); }

ProfileParts read_profile_parts(const toolchain::Locations& locations, std::string_view name) {
    const stdfs::path file = locations.profiles / (std::string(name) + ".json");
    const Json d = json::load(file);
    if (!d.is_object()) throw FormatError("toolchain profile", 0, "a profile is a JSON object");
    ProfileParts p;
    p.name = std::string(name);
    p.kit = prj_profile_text(d, "kit");
    if (const auto it = d.find("carve"); it != d.end() && it->is_object() && !it->empty()) {
        if (it->contains("prologues")) p.carve.prologues = prj_string_list((*it)["prologues"], "prologues");
        p.carve.prologue_call = prj_profile_text(*it, "prologue_call");
        p.carve.frames = prj_profile_flag(*it, "frames", true);
        p.carve.calls = prj_profile_flag(*it, "calls", true);
        p.carve.emulator_ints = prj_profile_flag(*it, "emulator_ints", true);
    }
    if (const auto it = d.find("link"); it != d.end() && it->is_object() && !it->empty()) {
        LinkSpec l;
        l.kit = prj_profile_text(*it, "kit");
        l.run = prj_profile_text(*it, "run");
        l.dir = prj_profile_text(*it, "dir");
        if (const auto libs = it->find("libs"); libs != it->end() && libs->is_object())
            for (const auto& [model, files] : libs->items()) l.libs[model] = prj_string_list(files, "libs");
        if (it->contains("copy")) l.copy = prj_string_list((*it)["copy"], "copy");
        l.exemod = prj_profile_text(*it, "exemod");
        if (const auto psp = it->find("psp"); psp != it->end()) l.psp = prj_number(*psp, "psp");
        if (it->contains("names")) l.names = prj_string_list((*it)["names"], "names");
        p.link = std::move(l);
    }
    if (const auto it = d.find("communal"); it != d.end() && it->is_object() && !it->empty()) {
        const auto segment = prj_profile_text(*it, "segment");
        if (!segment) throw FormatError("toolchain profile", 0, "\"communal\" has no \"segment\"");
        p.communal = Communal{*segment, prj_profile_text(*it, "align") == "para"};
    }
    if (d.contains("headers")) p.headers = prj_string_list(d["headers"], "headers");
    if (const auto it = d.find("libraries"); it != d.end() && it->is_object())
        for (const auto& [model, list] : it->items()) p.libraries[model] = prj_profile_libraries(list);
    if (const auto it = d.find("objects"); it != d.end() && it->is_object())
        for (const auto& [model, list] : it->items()) p.objects[model] = prj_string_list(list, "objects");
    p.inverse = prj_profile_text(d, "inverse");
    if (const auto it = d.find("data"); it != d.end() && it->is_object())
        if (const auto order = prj_profile_text(*it, "order")) p.data_order = *order;
    return p;
}

// ---- the project --------------------------------------------------------------------------------------------------------

std::shared_ptr<Project> Project::open(const stdfs::path& folder, std::shared_ptr<const Services> services,
                                       const std::optional<std::string>& override_json) {
    if (!services) services = std::make_shared<Services>();
    return std::shared_ptr<Project>(new Project(folder, std::move(services), override_json));
}

Project::Project(stdfs::path folder, std::shared_ptr<const Services> services, std::optional<std::string> override_json)
    : dir_(stdfs::absolute(folder).lexically_normal()), services_(std::move(services)), override_(std::move(override_json)) {
    read_config();
    exe_ = (dir_ / fs::from_text(config_.image)).lexically_normal();
    src_ = dir_ / fs::from_text(config_.src);
    for (const std::string& x : config_.include) include_.push_back((dir_ / fs::from_text(x)).lexically_normal());
    dgroup_ = config_.dgroup.value_or(0);
    loadseg_ = config_.loadseg;
    code_end_ = config_.code_end;
    switches_ = config_.switches;
    read_program();
    if (config_.packed) packed_ = exepack::read(fs::read_file((dir_ / fs::from_text(*config_.packed)).lexically_normal()));
    derive_code_segments();
}

void Project::read_config() {
    Json d = json::load(dir_ / "project.json");
    if (override_) {   // (a set of the project's sources: its own folder, pattern, toolchain)
        if (!d.is_object()) prj_bad("a project is a JSON object");
        d.erase("also");
        const Json o = json::parse(*override_);
        if (!o.is_object()) prj_bad("an entry of \"also\" is not an object");
        for (const auto& [key, value] : o.items()) d[key] = value;
    }
    config_ = prj_from_json(d);
}

void Project::read_program() {
    file_ = fs::read_file(exe_);
    const ByteView whole_file(file_);
    const bool mz = file_.size() >= 2 && ((file_[0] == 'M' && file_[1] == 'Z') || (file_[0] == 'Z' && file_[1] == 'M'));
    if (!mz) {
        image_ = whole_file;
        return;
    }
    if (file_.size() < 28) throw TruncatedError("MZ header", 0, 28, file_.size());
    MzHeader h{};
    for (std::size_t i = 0; i < h.size(); ++i) h[i] = prj_word(whole_file, 2 * i);
    header_ = h;
    // The image: from the header's paragraphs to the end of the file's pages, less the last page's unused bytes.
    const std::int64_t begin = std::min<std::int64_t>(h[4] * 16, static_cast<std::int64_t>(file_.size()));
    const std::int64_t end = std::clamp<std::int64_t>(h[2] * 512 - (h[1] ? 512 - h[1] : 0), begin, static_cast<std::int64_t>(file_.size()));
    image_ = whole_file.subspan(static_cast<std::size_t>(begin), static_cast<std::size_t>(end - begin));
    // The relocation table: where the loader adds the load segment.
    const std::size_t table = h[12], count = h[3];
    if (count != 0 && table + 4 * count > file_.size()) throw TruncatedError("MZ relocation table", table, 4 * count, file_.size() - std::min(table, file_.size()));
    std::set<std::uint16_t> words;   // (the words the relocations stand at: a small program's all hold one paragraph, DGROUP's)
    for (std::size_t i = 0; i < count; ++i) {
        const std::uint32_t at = prj_word(whole_file, table + 4 * i) + static_cast<std::uint32_t>(prj_word(whole_file, table + 4 * i + 2)) * 16;
        relocs_.insert(at);
        if (at + 2 <= image_.size()) words.insert(prj_word(image_, at));
    }
    if (!config_.dgroup && count != 0 && words.size() == 1) dgroup_ = static_cast<std::int64_t>(*words.begin()) * 16;
}

void Project::derive_code_segments() {
    if (config_.code_segments_from_relocs || (config_.code == "far" && !config_.code_segments)) {
        // (far code: the segments the loader sets, from the words the relocation table points at - those outside the data segment)
        const std::int64_t data_end = config_.data_end ? config_.data_end : dgroup_ + 1;
        std::set<std::uint32_t> keep{0};
        for (const std::uint32_t at : relocs_) {
            if (at + 2 > image_.size()) continue;
            const std::uint32_t segment = static_cast<std::uint32_t>(prj_word(image_, at)) * 16;
            if (segment < image_.size() && !(dgroup_ <= segment && segment < data_end)) keep.insert(segment);
        }
        code_segments_.assign(keep.begin(), keep.end());
        return;
    }
    code_segments_ = config_.code_segments.value_or(std::vector<std::uint32_t>{0});
    std::ranges::sort(code_segments_);
}

std::uint32_t Project::code_base(std::uint32_t a) const {
    std::uint32_t best = 0;
    for (const std::uint32_t b : code_segments_)
        if (b <= a) best = std::max(best, b);
    return best;
}

std::vector<std::shared_ptr<Project>> Project::sets() {
    std::vector<std::shared_ptr<Project>> out{shared_from_this()};
    for (const std::string& x : config_.also) out.push_back(open(dir_, services_, x));
    return out;
}

std::vector<CodeFunction> Project::functions() {
    if (!services_->carve) throw UsageError("exact project: no function finder (Services::carve) was given");
    return services_->carve(*this);
}

std::uint32_t Project::own_end() {
    if (!own_end_) {
        std::uint32_t end = 0;
        for (const CodeFunction& f : functions()) end = std::max(end, f.address + f.length);
        own_end_ = end;
    }
    return *own_end_;
}

std::optional<std::vector<std::uint32_t>> Project::starts() const {
    if (!config_.starts) return std::nullopt;
    const stdfs::path file = dir_ / fs::from_text(*config_.starts);
    const std::string content = fs::read_text(file);
    std::vector<std::uint32_t> out;
    std::uint32_t number = 0;
    for (const std::string_view line : prj_lines(content)) {
        const std::string_view t = text::trim(line);
        if (t.empty() || line.starts_with('#')) continue;
        const auto at = text::parse_int<std::uint32_t>(text::split_ws(t)[0], 16);
        if (!at) throw ParseError(fs::to_text(file), number + 1, 0, "a function start is a hexadecimal address");
        out.push_back(*at);
        ++number;
    }
    std::ranges::sort(out);
    return out;
}

std::vector<NamedToolchain> Project::toolchains() const {
    if (!services_->toolchains) throw UsageError("exact project: no compilers (Services::toolchains) were given");
    std::vector<NamedToolchain> out;
    if (!config_.toolchains.empty()) {
        for (const ToolchainSpec& t : config_.toolchains) out.push_back({t.label, services_->toolchains->load(t.toolchain, t.variant)});
        return out;
    }
    if (!config_.toolchain) prj_bad("the project names no \"toolchain\"");
    out.push_back({*config_.toolchain, services_->toolchains->load(*config_.toolchain, config_.variant)});
    return out;
}

std::shared_ptr<toolchain::Toolchain> Project::toolchain() const {
    if (!services_->toolchains) throw UsageError("exact project: no compilers (Services::toolchains) were given");
    if (!config_.toolchains.empty()) return services_->toolchains->load(config_.toolchains.front().toolchain, config_.toolchains.front().variant);
    if (!config_.toolchain) prj_bad("the project names no \"toolchain\"");
    return services_->toolchains->load(*config_.toolchain, config_.variant);
}

std::shared_ptr<const ProfileParts> Project::profile_parts() const {
    if (!config_.toolchains.empty()) return profile_parts(config_.toolchains.front().toolchain);
    if (!config_.toolchain) prj_bad("the project names no \"toolchain\"");
    return profile_parts(*config_.toolchain);
}

std::shared_ptr<const ProfileParts> Project::profile_parts(std::string_view name) const {
    if (!services_->toolchains) throw UsageError("exact project: no compilers (Services::toolchains) were given");
    const std::scoped_lock lock(parts_mutex_);
    auto& slot = parts_[std::string(name)];
    if (!slot) slot = std::make_shared<const ProfileParts>(read_profile_parts(services_->toolchains->locations(), name));
    return slot;
}

SymbolTable Project::symbols() const {
    SymbolTable out;
    const stdfs::path file = path("SYMBOLS");
    std::error_code ec;
    if (!stdfs::exists(file, ec)) return out;
    const std::string content = fs::read_text(file);
    std::uint32_t number = 0;
    for (const std::string_view line : prj_lines(content)) {
        ++number;
        const auto c = text::split_ws(line);
        if (line.starts_with('#') || c.size() < 2) continue;
        const auto address = [&](std::string_view t) {
            const auto a = text::parse_int<std::int64_t>(t, 16);
            if (!a) throw ParseError(fs::to_text(file), number, 0, std::format("{} is not a hexadecimal address", t));
            return *a;
        };
        if (c.size() >= 3 && (c[1] == "C" || c[1] == "D" || c[1] == "S" || c[1] == "?")) {   // ("?": a place of no kind, as save_symbols writes it)
            const SymKind kind = c[1] == "C" ? SymKind::Code : c[1] == "D" ? SymKind::Data : c[1] == "S" ? SymKind::Segment : SymKind::Unknown;
            out.insert_or_assign(std::string(c[0]), SymPlace{kind, address(c[2])});
        } else if (prj_all_hex(c[1])) {   // (name address: of the code or the data, unsaid)
            out.insert_or_assign(std::string(c[0]), SymPlace{SymKind::Unknown, address(c[1])});
        }
    }
    return out;
}

void Project::save_symbols(const SymbolTable& symbols) const {
    std::vector<const std::pair<std::string, SymPlace>*> items;
    for (const auto& kv : symbols) items.push_back(&kv);
    // by kind letter ('?' < C < D < S), address, name
    std::ranges::sort(items, [](const auto* a, const auto* b) {
        return std::tuple(sym_kind_letter(a->second.kind), a->second.address, a->first) < std::tuple(sym_kind_letter(b->second.kind), b->second.address, b->first);
    });
    std::string out = "# name, C (code: image offset) or D (data: offset in the data segment), address\n";
    for (const auto* kv : items) out += std::format("{} {} {}\n", kv->first, sym_kind_letter(kv->second.kind), prj_hex4(kv->second.address));
    fs::write_text(path("SYMBOLS"), out);
}

std::vector<Source> Project::sources() const {
    std::vector<Source> out;
    const std::vector<std::string> names = prj_names_in(src_);
    const Regex pattern(config_.sources, RegexFlags::IgnoreCase);   // (project "sources": the pattern of the file names, group 1 the address)
    for (const std::string& f : names) {
        const auto m = pattern.full_match(f);
        if (!m || !m->has(1)) continue;
        const auto at = text::parse_int<std::int64_t>(m->group(1), 16);
        if (!at) continue;
        out.push_back({*at, src_ / fs::from_text(f)});
    }
    if (config_.place_by) {   // (whole files placed by a list of names and places: each at its first function's)
        std::map<std::string, std::int64_t> places;
        static const Regex place_re(R"([fF]?([0-9a-fA-F]{4,5}))");
        const std::string place_text = fs::read_text(path(*config_.place_by));   // (the lines are views of it)
        for (const std::string_view line : prj_lines(place_text)) {
            const auto c = text::split_ws(line.substr(0, line.find('#')));
            if (c.size() < 2) continue;
            const auto m = place_re.full_match(c[1]);
            if (m) places[text::upper(c[0]).substr(0, 8)] = *text::parse_int<std::int64_t>(m->group(1), 16);
        }
        static const Regex function_re(R"(^(?:[A-Za-z_][\w \*]*?\s)?\**\s*([A-Za-z_]\w*)\s*\([^;{]*$)", RegexFlags::Multiline);
        for (const std::string& f : names) {
            if (!text::lower(f).ends_with(".c")) continue;
            const std::string body = prj_without_comments(fs::read_text(path(f)));
            std::optional<std::int64_t> place;
            function_re.for_each_match(body, [&](const RegexMatch& m) {
                const std::string_view name = m.group(1);
                if (name == "if" || name == "while" || name == "for" || name == "switch" || name == "return") return true;
                const auto it = places.find(text::upper(name).substr(0, 8));
                if (it == places.end()) return true;
                place = it->second;
                return false;
            });
            if (place) out.push_back({*place, src_ / fs::from_text(f)});
        }
    }
    if (config_.unplaced) {   // (files with no place in their name: each is placed by the first of its names SYMBOLS knows - address -1)
        const Regex re(*config_.unplaced, RegexFlags::IgnoreCase);
        for (const std::string& f : names)
            if (re.full_match(f)) out.push_back({-1, src_ / fs::from_text(f)});
    }
    const stdfs::path order = path("ORDER");
    std::error_code ec;
    if (stdfs::exists(order, ec)) {
        const std::string order_text = fs::read_text(order);
        for (const std::string_view line : prj_lines(order_text)) {
            const auto c = text::split_ws(line.substr(0, line.find('#')));
            if (c.size() < 2 || !prj_all_hex(c[1]) || !stdfs::exists(path(c[0]), ec)) continue;
            if (const auto at = text::parse_int<std::int64_t>(c[1], 16)) out.push_back({*at, path(c[0])});
        }
    }
    std::ranges::sort(out, [](const Source& a, const Source& b) { return std::tuple(a.at, fs::to_text(a.path)) < std::tuple(b.at, fs::to_text(b.path)); });
    return out;
}

bool Project::whole(const stdfs::path& file) const {
    if (config_.whole) return true;
    const std::string name = fs::to_text(file.filename());   // not one of the one-function files: fADDR.c
    const bool one_function = name.size() == 8 && name[0] == 'f' && name.ends_with(".c") &&
                              std::ranges::all_of(name.substr(1, 5), [](unsigned char c) { return std::isdigit(c) || (c >= 'a' && c <= 'f'); });
    return !one_function;
}

std::map<std::string, Bytes> Project::headers_with(const std::vector<std::string>& extensions) const {
    std::map<std::string, Bytes> out;
    for (const std::string& f : prj_names_in(src_)) {
        const std::string lower = text::lower(f);
        if (std::ranges::any_of(extensions, [&](const std::string& e) { return lower.ends_with(e); })) out[f] = fs::read_file(src_ / fs::from_text(f));
    }
    return out;
}

std::map<std::string, Bytes> Project::headers() const { return headers_with(profile_parts()->headers); }

modules::CompileOptions Project::compile_options() const {
    modules::CompileOptions o;
    o.headers = headers();
    for (const stdfs::path& i : include_) o.include.push_back(fs::to_text(i));
    o.switches = switches_;
    return o;
}

batch::CompileOptions Project::batch_options() const {
    batch::CompileOptions o;
    o.headers = headers();
    for (const stdfs::path& i : include_) o.include.push_back(fs::to_text(i));
    o.switches = switches_;
    return o;
}

void Project::set_entry(std::string_view key, std::string_view sub, std::int64_t value) {
    const stdfs::path file = dir_ / "project.json";
    Json d = json::load(file);
    if (!d.is_object()) prj_bad("a project is a JSON object");
    Json& entry = d[std::string(key)];
    if (!entry.is_object()) entry = Json::object();
    entry[std::string(sub)] = value;
    json::save(file, d, json::Style::PythonCompatiblePretty);
    read_config();
}

compare::Program Project::program() const {
    compare::Program p;
    p.dgroup = dgroup_;
    p.loadseg = loadseg_;
    p.code_base = [this](std::uint32_t a) { return code_base(a); };
    p.relocs = &relocs_;
    return p;
}

StandinInput Project::standin_input() const {
    StandinInput in;
    in.symbols = symbols();
    in.image = image_;
    in.compare = program();
    if (packed_) in.relocations = packed_->relocs;   // (the packed original's table, in its order: the linker's)
    else {
        in.relocations.assign(relocs_.begin(), relocs_.end());
        std::ranges::sort(in.relocations);
    }
    return in;
}

RecutInput Project::recut_input() const {
    RecutInput in;
    in.symbols = symbols();
    if (packed_) in.packed_relocs = packed_->relocs;
    if (header_) in.table = recut::RelocTable{(*header_)[3], (*header_)[12]};
    in.file = file_;
    return in;
}

}  // namespace cb::exact::project
