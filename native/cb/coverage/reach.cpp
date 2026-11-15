// cb/coverage/reach.cpp -- see reach.hpp.
// cb-unity: alone
#include "cb/coverage/reach.hpp"

#include <algorithm>
#include <chrono>
#include <format>
#include <cmath>
#include <ctime>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include <nlohmann/json.hpp>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/log.hpp"
#include "cb/core/process.hpp"
#include "cb/core/sys.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/detect.hpp"
#include "cb/formats/pe.hpp"
#include "cb/platforms/package.hpp"

namespace cb::coverage::reach {

namespace {

namespace stdfs = std::filesystem;

// What is worth looking inside a container for: executables and cartridge images.
constexpr std::string_view kProgramExts[] = {
    ".xex", ".exe", ".dll", ".xbe", ".com", ".sys", ".drv", ".vxd", ".386", ".nes", ".gb", ".gbc", ".sms", ".gg", ".sg",
    ".md", ".gen", ".smd", ".bin", ".rom", ".sna", ".z80", ".tap", ".jar", ".gba", ".nds", ".z64", ".n64", ".v64", ".sfc",
    ".smc", ".elf", ".self", ".apk", ".ipa", ".xap", ".dol", ".rpx", ".rpl"};
// Not programs: never measured, only counted.
constexpr std::string_view kSkipFamilies[] = {"media", "xna"};
constexpr std::string_view kContainerFormats[] = {"STFS package", "CD image", "Xbox disc image", "disk image",
                                                  "compressed disc image"};

template <std::size_t N>
bool contains(const std::string_view (&list)[N], std::string_view s) {
    return std::find(std::begin(list), std::end(list), s) != std::end(list);
}

bool ends_with_program_ext(std::string_view path) {
    const std::string lower = text::lower(path);
    return std::any_of(std::begin(kProgramExts), std::end(kProgramExts),
                       [&](std::string_view e) { return lower.ends_with(e); });
}

void say(const Services& services, std::string_view line) {
    if (services.log) {
        services.log(line);
    } else {
        out("{}\n", line);
        flush_out();
    }
}

stdfs::path reach_dir(const Services& services) { return services.state_dir / "reach"; }

double now(const Services& services) {
    if (services.clock) return services.clock();
    return std::chrono::duration<double>(std::chrono::system_clock::now().time_since_epoch()).count();
}

std::string sha256_hex(const stdfs::path& file) {
    const fs::MappedFile mapped(file);
    return crypto::to_hex(crypto::sha256(mapped.view()));
}

// An open volume list: the Services' opener, else the media module's.
std::vector<std::unique_ptr<formats::media::Volume>> open_volumes(const stdfs::path& path, const Services& services) {
    if (services.open_volumes) return services.open_volumes(path);
    std::error_code ec;
    if (!stdfs::is_regular_file(path, ec)) return {};
    try {
        return formats::media::volumes(path);
    } catch (const FormatError&) {
        return {};   // a file the media module does not know is not a container
    }
}

std::string tools_of(const Services& services) {
    if (services.tools_revision) return services.tools_revision();
    try {
        process::ProcessSpec spec;
        spec.argv = {"git", "rev-parse", "--short", "HEAD"};
        if (!services.checkout_root.empty()) spec.cwd = services.checkout_root;
        spec.timeout = std::chrono::seconds(20);
        const process::ProcessResult r = process::run(spec);
        const std::string rev(text::trim(std::string_view(reinterpret_cast<const char*>(r.out.data()), r.out.size())));
        return rev.empty() ? "?" : rev;
    } catch (const Error&) {
        return "?";
    }
}

std::vector<std::string> measure_argv_of(const stdfs::path& program, const Services& services) {
    if (services.measure_command) {
        std::vector<std::string> argv = services.measure_command(program);
        if (!argv.empty()) return argv;   // (empty: the default below)
    }
    return {fs::to_text(fs::executable_path()), "measure", fs::to_text(program)};
}

// ---- results file
Json to_json(const Row& r) {
    Json j = Json::object();
    j["found"] = r.found;
    j["platform"] = r.platform;
    j["lane"] = r.lane ? Json(*r.lane) : Json(nullptr);
    j["family"] = r.family;
    j["path"] = r.path;
    j["origin"] = r.origin;
    j["size"] = r.size;
    j["sha"] = r.sha;
    if (r.picked) j["picked"] = true;
    if (r.judged) {
        Json t = Json::object();
        for (const auto& [k, v] : r.judged->tally) t[k] = v;
        Json judged = Json::object();
        judged["tally"] = t;
        judged["seconds"] = r.judged->seconds;
        judged["stopped"] = r.judged->stopped;
        j["judged"] = judged;
    }
    if (r.tools) j["tools"] = *r.tools;
    return j;
}

std::string string_of(const Json& j, const char* key) {
    const auto it = j.find(key);
    return it != j.end() && it->is_string() ? it->get<std::string>() : std::string();
}

Row row_from(const Json& j) {
    Row r;
    r.found = string_of(j, "found");
    r.platform = string_of(j, "platform");
    if (j.contains("lane") && j["lane"].is_string()) r.lane = j["lane"].get<std::string>();
    r.family = string_of(j, "family");
    r.path = string_of(j, "path");
    r.origin = string_of(j, "origin");
    if (j.contains("size") && j["size"].is_number_unsigned()) r.size = j["size"].get<std::uint64_t>();
    r.sha = string_of(j, "sha");
    r.picked = j.contains("picked") && j["picked"].is_boolean() && j["picked"].get<bool>();
    if (j.contains("judged") && j["judged"].is_object()) {
        Judged judged;
        const Json& jj = j["judged"];
        if (jj.contains("tally") && jj["tally"].is_object()) {
            for (const auto& [k, v] : jj["tally"].items()) {
                if (v.is_number_integer()) judged.tally[k] = v.get<std::int64_t>();
            }
        }
        if (jj.contains("seconds") && jj["seconds"].is_number()) judged.seconds = jj["seconds"].get<double>();
        judged.stopped = string_of(jj, "stopped");
        r.judged = std::move(judged);
    }
    if (j.contains("tools") && j["tools"].is_string()) r.tools = j["tools"].get<std::string>();
    return r;
}

std::string clock_text(double seconds) {
    const std::time_t t = static_cast<std::time_t>(seconds);
    const std::tm tm = sys::local_tm(t);
    char buf[64];
    std::strftime(buf, sizeof buf, "%Y-%m-%d %I:%M %p", &tm);
    return buf;
}

double round_to_tenth(double x) { return std::round(x * 10.0) / 10.0; }

}  // namespace

std::function<std::optional<PlatformInfo>(const stdfs::path&)> identify_with(const platforms::Registry& registry) {
    return [&registry](const stdfs::path& path) -> std::optional<PlatformInfo> {
        const platforms::Platform* p = registry.identify(path);
        if (p == nullptr) return std::nullopt;
        return PlatformInfo{p->title, p->name, p->family, p->loader.container, p->lane.has_value()};
    };
}

// ---- what a file is, where detect has nothing to say
std::optional<Gap> gap_of(const stdfs::path& path, ByteView head) {
    const std::string ext = text::lower(fs::to_text(path.extension()));
    const auto starts = [&](std::string_view magic) {
        return head.size() >= magic.size() && std::equal(magic.begin(), magic.end(), head.begin(), [](char a, std::uint8_t b) {
                   return static_cast<std::uint8_t>(a) == b;
               });
    };
    if (starts("PS-X EXE")) return Gap{"PlayStation (PS1)", "MIPS R3000"};
    if (starts("\x80\x37\x12\x40") || starts("\x37\x80\x40\x12") || starts("\x40\x12\x37\x80")) {
        return Gap{"Nintendo 64", "MIPS R4300"};
    }
    if (head.size() > 0xB3 && head[0xB2] == 0x96 && head[3] == 0xEA) return Gap{"Game Boy Advance", "ARM7TDMI"};
    if (ext == ".nds") return Gap{"Nintendo DS", "ARM9 / ARM7"};
    if (ext == ".sfc" || ext == ".smc") return Gap{"Super Nintendo", "65816"};
    if (head.size() >= 20 && starts("\x7f" "ELF")) {
        const unsigned machine = head[5] == 1 ? head[18] | (head[19] << 8) : (head[18] << 8) | head[19];
        std::string cpu;
        switch (machine) {
            case 0x08: cpu = "MIPS"; break;
            case 0x14: cpu = "PowerPC"; break;
            case 0x15: cpu = "PowerPC 64"; break;
            case 0x28: cpu = "ARM"; break;
            case 0x3E: cpu = "x86-64"; break;
            case 0x03: cpu = "x86"; break;
            case 0xB7: cpu = "AArch64"; break;
            case 0x2A: cpu = "SuperH"; break;
            case 0x17: cpu = "Cell SPU"; break;
            default: cpu = std::format("machine 0x{:X}", machine);
        }
        return Gap{"ELF program", cpu};
    }
    if (starts("PK") && ext == ".apk") return Gap{"Android app", "Dalvik"};
    return std::nullopt;
}

Described describe(const stdfs::path& path, const Services& services) {
    CB_ASSERT(services.identify_platform != nullptr, "reach::describe needs Services::identify_platform");
    const std::vector<formats::detect::Detected> detected = formats::detect::identify(path);
    const formats::detect::Detected found = detected.empty() ? formats::detect::Detected{} : detected.front();
    const std::string& fmt = found.format;

    Bytes head;
    try {
        const fs::MappedFile mapped(path);
        const ByteView v = mapped.view();
        head.assign(v.begin(), v.begin() + std::min<std::size_t>(v.size(), 0x200));
    } catch (const IoError&) {
        head.clear();
    }

    if (fmt.starts_with("PE") && found.family == "windows" && formats::pe::is_dotnet_file(path)) {
        return {".NET assembly", ".NET (CIL)", "xna", "xna"};   // (the `xna` lane's)
    }
    std::error_code ec;
    const bool big = stdfs::file_size(path, ec) > (std::uintmax_t{256} << 20) && fmt.empty();   // (data: not read further)
    std::optional<PlatformInfo> p;
    if (!big) p = services.identify_platform(path);
    if (p) {
        return {fmt.empty() ? p->container : fmt, p->title, p->has_lane ? std::optional<std::string>(p->name) : std::nullopt,
                p->family};
    }
    if (const std::optional<Gap> g = gap_of(path, head)) {
        return {fmt.empty() ? g->title : fmt, std::format("{} ({})", g->title, g->cpu), std::nullopt, "gap"};
    }
    return {fmt.empty() ? "unknown" : fmt, "", std::nullopt, found.family};
}

// ---- finding the programs
namespace {

std::vector<stdfs::path> unpack_volumes(std::vector<std::unique_ptr<formats::media::Volume>>& volumes,
                                        const stdfs::path& container, std::size_t limit, const Services& services) {
    struct Candidate {
        formats::media::Volume* volume;
        const formats::media::Entry* entry;
    };
    std::vector<Candidate> entries;
    try {
        for (auto& v : volumes) {
            for (const formats::media::Entry& e : v->files()) {
                if (!e.is_dir && ends_with_program_ext(e.path) && e.size > 0 && e.size < (std::uint64_t{512} << 20)) {
                    entries.push_back({v.get(), &e});
                }
            }
        }
    } catch (const Error& e) {   // any reader's error, listing too: this container is skipped
        say(services, std::format("  {}: not opened ({})", fs::to_text(container.filename()), std::string(e.what()).substr(0, 80)));
        return {};
    }
    std::stable_sort(entries.begin(), entries.end(), [](const Candidate& a, const Candidate& b) { return a.entry->size < b.entry->size; });
    const std::string key = crypto::to_hex(crypto::sha1(as_bytes(fs::to_text(stdfs::absolute(container)))));
    const stdfs::path root = reach_dir(services) / "unpacked" / key.substr(0, 12);
    std::vector<stdfs::path> result;
    for (std::size_t i = 0; i < entries.size() && i < limit; ++i) {
        stdfs::path dest = root;
        std::string normal = text::replace_all(entries[i].entry->path, "\\", "/");
        for (const std::string_view part : text::split(normal, '/')) {
            if (!part.empty() && part != "." && part != "..") dest /= fs::from_text(part);
        }
        if (!stdfs::is_regular_file(dest)) {
            try {
                const Bytes data = entries[i].volume->read(*entries[i].entry);
                fs::make_dirs(dest.parent_path());
                fs::write_file(dest, data);
            } catch (const Error&) {
                continue;   // one file that will not come out is left out
            }
        }
        result.push_back(std::move(dest));
    }
    return result;
}

}  // namespace

std::vector<stdfs::path> unpack(const stdfs::path& container, std::size_t limit, const Services& services) {
    std::vector<std::unique_ptr<formats::media::Volume>> volumes;
    try {
        volumes = open_volumes(container, services);
    } catch (const Error& e) {
        say(services, std::format("  {}: not opened ({})", fs::to_text(container.filename()), std::string(e.what()).substr(0, 80)));
        return {};
    }
    return unpack_volumes(volumes, container, limit, services);
}

std::vector<Found> programs(const std::vector<stdfs::path>& inputs, std::size_t containers, const Services& services) {
    std::vector<Found> out;
    const auto one = [&](const stdfs::path& path, const std::string& origin) {
        const std::vector<formats::detect::Detected> detected = formats::detect::identify(path);
        const std::string_view format = detected.empty() ? std::string_view() : std::string_view(detected.front().format);
        std::vector<stdfs::path> inner;
        if (std::any_of(std::begin(kContainerFormats), std::end(kContainerFormats),
                        [&](std::string_view c) { return format.starts_with(c); })) {
            inner = unpack(path, containers, services);
        }
        if (inner.empty()) {   // (a container with no program: counted)
            out.push_back({path, origin});
        } else {
            for (stdfs::path& p : inner) out.push_back({std::move(p), fs::to_text(path)});
        }
    };

    // A folder holding one Games-on-Demand title is a container itself, and is not walked into.
    const auto god = [&](const stdfs::path& folder) {
        try {
            return open_volumes(folder, services);
        } catch (const Error&) {
            return std::vector<std::unique_ptr<formats::media::Volume>>();
        }
    };
    const auto walk = [&](const auto& self, const stdfs::path& dir, const std::string& root) -> void {
        if (auto volumes = god(dir); !volumes.empty()) {
            for (stdfs::path& inner : unpack_volumes(volumes, dir, containers, services)) out.push_back({std::move(inner), fs::to_text(dir)});
            return;
        }
        std::vector<stdfs::path> subdirs, files;
        std::error_code ec;
        for (stdfs::directory_iterator it(dir, ec), end; !ec && it != end; it.increment(ec)) {
            std::error_code e2;
            if (it->is_directory(e2)) {
                const std::string name = fs::to_text(it->path().filename());
                if (!name.starts_with(".") && name != "__pycache__" && name != "node_modules") subdirs.push_back(it->path());
            } else {
                files.push_back(it->path());
            }
        }
        std::sort(files.begin(), files.end());
        std::sort(subdirs.begin(), subdirs.end());
        for (const stdfs::path& f : files) one(f, root);
        for (const stdfs::path& d : subdirs) self(self, d, root);
    };
    for (const stdfs::path& root : inputs) {
        std::error_code ec;
        if (stdfs::is_regular_file(root, ec)) {
            one(root, "");
        } else if (stdfs::is_directory(root, ec)) {
            walk(walk, root, fs::to_text(root));
        }
    }
    return out;
}

// ---- measuring
Judged judge(const stdfs::path& program, int timeout_seconds, const Services& services) {
    const double t0 = now(services);
    std::string stopped;
    // The child and everything it starts run in one job (process::run), so a measure stopped at its time limit leaves
    // no worker behind to hold the run open.
    try {
        process::ProcessSpec spec;
        spec.argv = measure_argv_of(program, services);
        spec.err = process::Capture::Merge;
        spec.timeout = std::chrono::seconds(timeout_seconds);
        const process::ProcessResult r = process::run(spec);
        if (r.timed_out) {
            stopped = std::format("timed out after {} s", timeout_seconds);
        } else if (r.exit_code != 0) {
            const std::string output = text::sanitize_utf8(std::string_view(reinterpret_cast<const char*>(r.out.data()), r.out.size()));
            std::string_view last;
            for (const std::string_view line : text::lines(text::trim(output))) last = line;
            stopped = last.empty() ? std::format("exit {}", r.exit_code) : std::string(last.substr(0, 160));
        }
    } catch (const ToolError& e) {
        stopped = std::string(e.what()).substr(0, 160);   // the measure command could not be started
    }
    Judged judged;
    std::string why;
    try {
        if (!services.tally) throw UsageError("no function store is wired");
        judged.tally = services.tally(program);
    } catch (const std::exception& e) {
        why = "no tally: " + std::string(e.what()).substr(0, 120);
    }
    judged.seconds = round_to_tenth(now(services) - t0);
    judged.stopped = stopped.empty() ? why : stopped;
    return judged;
}

std::vector<Row*> spread(std::vector<Row*> rows, std::size_t n, std::uint64_t max_size) {
    rows.erase(std::remove_if(rows.begin(), rows.end(), [&](const Row* r) { return r->size > max_size; }), rows.end());
    std::stable_sort(rows.begin(), rows.end(), [](const Row* a, const Row* b) { return a->size < b->size; });
    if (rows.size() <= n) return rows;
    if (n <= 1) return {rows.begin(), rows.begin() + static_cast<std::ptrdiff_t>(std::min<std::size_t>(rows.size(), 1))};
    std::vector<Row*> picked;
    for (std::size_t i = 0; i < n; ++i) {
        // round half to even, as the sample has always been taken
        picked.push_back(rows[static_cast<std::size_t>(std::nearbyint(static_cast<double>(i) * static_cast<double>(rows.size() - 1) / static_cast<double>(n - 1)))]);
    }
    return picked;
}

std::vector<Row> read_results(const stdfs::path& file) {
    std::vector<Row> rows;
    std::error_code ec;
    if (!stdfs::is_regular_file(file, ec)) return rows;
    try {
        const Json doc = json::load(file);
        if (doc.is_object() && doc.contains("programs") && doc["programs"].is_array()) {
            for (const Json& j : doc["programs"]) {
                if (j.is_object()) rows.push_back(row_from(j));
            }
        }
    } catch (const Error&) {
        rows.clear();   // results are a cache of measurements: one that cannot be read is measured again
    }
    return rows;
}

Result run(const std::vector<stdfs::path>& inputs, const Options& options, const Services& services) {
    const stdfs::path store = reach_dir(services) / (options.name + ".json");
    std::vector<Row> old;   // earlier results by hash, in the file's order (a hash met again replaces its row)
    std::unordered_map<std::string, std::size_t> old_by_sha;
    for (Row& r : read_results(store)) {
        const auto [it, fresh] = old_by_sha.emplace(r.sha, old.size());
        if (fresh) old.push_back(std::move(r)); else old[it->second] = std::move(r);
    }

    const std::string tools = tools_of(services);
    const std::vector<Found> found = programs(inputs, options.containers, services);
    say(services, std::format("{} files found", found.size()));

    std::vector<Row> rows;
    std::unordered_set<std::string> seen;
    for (const Found& f : found) {
        try {
            const Described d = describe(f.path, services);
            Row row;
            row.found = d.found;
            row.platform = d.platform;
            row.lane = d.lane;
            row.family = d.family;
            row.path = fs::to_text(f.path);
            row.origin = f.origin;
            row.size = fs::file_size(f.path);
            // only programs are hashed (a game library is mostly data: never read whole)
            row.sha = (!row.platform.empty() || row.family == "gap") ? sha256_hex(f.path) : "path:" + row.path;
            if (!seen.insert(row.sha).second) continue;
            rows.push_back(std::move(row));
        } catch (const IoError&) {
            continue;   // a file that went away since it was listed
        }
    }

    const auto save = [&] {   // after every program: a stopped run keeps its results (and what earlier runs measured)
        Json programs_json = Json::array();
        for (const Row& r : rows) programs_json.push_back(to_json(r));
        for (const Row& r : old) {
            if (!seen.contains(r.sha)) programs_json.push_back(to_json(r));
        }
        Json inputs_json = Json::array();
        for (const stdfs::path& i : inputs) inputs_json.push_back(fs::to_text(stdfs::absolute(i)));
        Json doc = Json::object();
        doc["when"] = clock_text(now(services));
        doc["inputs"] = inputs_json;
        doc["tools"] = tools;
        doc["programs"] = programs_json;
        fs::make_dirs(store.parent_path());
        json::save(store, doc);
    };

    // which get judged: programs of a platform with a lane, smallest first, per_platform of each
    std::map<std::string, std::vector<Row*>> by_platform;
    for (Row& r : rows) {
        if (r.lane && !contains(kSkipFamilies, r.family)) by_platform[r.platform].push_back(&r);
    }
    for (auto& [platform, platform_rows] : by_platform) {
        for (Row* r : spread(platform_rows, options.per_platform, options.max_size)) {
            r->picked = true;
            const auto prev = old_by_sha.find(r->sha);
            if (prev != old_by_sha.end() && old[prev->second].tools == tools && old[prev->second].judged && !options.again) {
                r->judged = old[prev->second].judged;
                r->tools = tools;
                continue;
            }
            if (options.judge) {
                say(services, std::format("judging {} ({}, {} KB)", fs::to_text(stdfs::path(r->path).filename()), platform, r->size / 1024));
                r->judged = judge(r->path, options.timeout_seconds, services);
                r->tools = tools;
                std::string tally_text;
                for (const auto& [name, count] : r->judged->tally) {
                    if (count != 0) tally_text += (tally_text.empty() ? "" : ", ") + std::format("{} {}", name, count);
                }
                say(services, std::format("  {}{}", tally_text.empty() ? "nothing recorded" : tally_text,
                                          r->judged->stopped.empty() ? "" : "; " + r->judged->stopped));
                save();
            }
        }
    }
    save();

    std::vector<Row> all = rows;
    for (const Row& r : old) {
        if (!seen.contains(r.sha)) all.push_back(r);
    }
    Result result;
    result.report = report(all);
    fs::write_text(reach_dir(services) / (options.name + ".md"), result.report);
    result.rows = std::move(rows);
    return result;
}

// ---- the report
std::string report(const std::vector<Row>& rows) {
    const auto is_program = [](const Row& r) { return !r.platform.empty() || r.family == "gap"; };
    std::vector<const Row*> programs_sorted;
    for (const Row& r : rows) {
        if (is_program(r)) programs_sorted.push_back(&r);
    }
    std::stable_sort(programs_sorted.begin(), programs_sorted.end(), [](const Row* a, const Row* b) {
        return std::make_tuple(!a->lane.has_value(), std::string_view(a->platform)) <
               std::make_tuple(!b->lane.has_value(), std::string_view(b->platform));
    });
    std::vector<std::pair<std::string, std::vector<const Row*>>> platforms;   // first-seen order of the sort above
    for (const Row* r : programs_sorted) {
        auto it = std::find_if(platforms.begin(), platforms.end(), [&](const auto& p) { return p.first == r->platform; });
        if (it == platforms.end()) it = platforms.insert(platforms.end(), {r->platform, {}});
        it->second.push_back(r);
    }

    std::string out;
    out += "# Reach\n\n";
    out += "What CodeBreaker made of every program it was given (`codebreaker reach`).\n";
    out += "Functions and percentages are those of the judged programs that finished; a program stopped at\n";
    out += "its time limit is counted under Stopped only.\n\n";
    out += "| Platform | Programs | Lane | Judged | Functions | Verified | Weak | Unverified | Stopped |\n";
    out += "|---|---|---|---|---|---|---|---|---|\n";
    for (const auto& [platform, rs] : platforms) {
        std::size_t judged_count = 0, stopped = 0;
        Tally t;
        for (const Row* r : rs) {
            if (!r->judged) continue;
            ++judged_count;
            // (a program stopped at its time limit: the functions it never reached are not failures; counted as stopped)
            if (!r->judged->stopped.starts_with("timed out")) {
                for (const auto& [k, v] : r->judged->tally) t[k] += v;
            }
            if (!r->judged->stopped.empty()) ++stopped;
        }
        std::int64_t total = 0;
        for (const auto& [k, v] : t) total += v;
        const auto get = [&](const char* k) {
            const auto it = t.find(k);
            return it == t.end() ? std::int64_t{0} : it->second;
        };
        const auto pct = [&](std::int64_t n) { return total ? std::format("{} ({}%)", n, 100 * n / total) : std::string("-"); };
        out += std::format("| {} | {} | {} | {} | {} | {} | {} | {} | {} |\n", platform, rs.size(),
                           rs.front()->lane ? *rs.front()->lane : "**none yet**", judged_count,
                           total ? std::to_string(total) : "-", pct(get("verified")), pct(get("weak")),
                           pct(get("unverified") + get("no-compile")), stopped ? std::to_string(stopped) : "");
    }

    std::vector<std::pair<std::string, std::size_t>> other;   // what is not a program, most common first
    std::size_t other_total = 0;
    for (const Row& r : rows) {
        if (is_program(r)) continue;
        ++other_total;
        auto it = std::find_if(other.begin(), other.end(), [&](const auto& p) { return p.first == r.found; });
        if (it == other.end()) other.push_back({r.found, 1}); else ++it->second;
    }
    std::stable_sort(other.begin(), other.end(), [](const auto& a, const auto& b) { return a.second > b.second; });
    std::string other_text;
    for (std::size_t i = 0; i < other.size() && i < 8; ++i) {
        other_text += (i ? ", " : "") + std::format("{} {}", other[i].first, other[i].second);
    }
    out += std::format("\nNot programs, or not recognised: {} files ({}).\n", other_total, other_text);

    std::vector<const Row*> stops;
    for (const Row& r : rows) {
        if (r.judged && !r.judged->stopped.empty()) stops.push_back(&r);
    }
    if (!stops.empty()) {
        out += "\n## Where judging stopped\n\n";
        for (const Row* r : stops) {
            out += std::format("- {} ({}): {}\n", fs::to_text(stdfs::path(r->path).filename()), r->platform, r->judged->stopped);
        }
    }
    return out;
}

cli::Command command(const Services& services) {
    cli::Command c;
    c.name = "reach";
    c.summary = "throw anything at CodeBreaker and see how far each program gets";
    c.params = {
        cli::Positional{"inputs", "files and folders (with --report: the run's name)", false, true},
        cli::Option{"--per-platform", "", "programs judged of each platform", cli::Arg::Value, "N", "5"},
        cli::Option{"--judge", "", "judge the programs picked (the platform's lane, in a process of its own)", cli::Arg::Flag},
        cli::Option{"--timeout", "", "seconds before a judged program is stopped", cli::Arg::Value, "S", "300"},
        cli::Option{"--containers", "", "programs taken out of each container", cli::Arg::Value, "N", "20"},
        cli::Option{"--name", "", "the run: state/reach/<name>.json", cli::Arg::Value, "RUN", "reach"},
        cli::Option{"--again", "", "measure programs measured before with these tools too", cli::Arg::Flag},
        cli::Option{"--max-size", "", "KB: larger programs are not judged", cli::Arg::Value, "KB", "4096"},
        cli::Option{"--report", "", "print the last results again (the run named by the first argument or --name)", cli::Arg::Flag},
    };
    c.handler = [services](const cli::Args& a) {
        const std::vector<std::string> given = a.values("inputs");
        if (a.flag("report")) {
            const std::string run_name = given.empty() ? a.value_or("name", "reach") : given.front();
            out("{}", fs::read_text(reach_dir(services) / (run_name + ".md")));
            return 0;
        }
        if (given.empty()) throw UsageError("reach needs a file or folder to look at");
        Options o;
        o.per_platform = a.integer<std::size_t>("per-platform").value_or(5);
        o.judge = a.flag("judge");
        o.timeout_seconds = a.integer<int>("timeout").value_or(300);
        o.containers = a.integer<std::size_t>("containers").value_or(20);
        o.name = a.value_or("name", "reach");
        o.again = a.flag("again");
        o.max_size = a.integer<std::uint64_t>("max-size").value_or(4096) * 1024;
        std::vector<stdfs::path> inputs;
        for (const std::string& g : given) inputs.push_back(fs::from_text(g));
        out("{}", run(inputs, o, services).report);
        return 0;
    };
    return c;
}

}  // namespace cb::coverage::reach
