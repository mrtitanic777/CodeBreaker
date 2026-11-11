#include "cb/build/driver.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/process.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"

namespace cb::build::driver {

namespace {

namespace stdfs = std::filesystem;
namespace le = formats::le;

// The whole module-flags word of a driver that is loaded on demand (a `virtdevice`): the loader's flags and the
// driver's own bits together, as the linker writes them. (A start-up driver is 0x28000.)
constexpr std::uint32_t kDynamicModuleFlags = 0x38000;

// How much of a patched place of a driver the loader overwrites: a place is a 32-bit address.
constexpr std::int64_t kPatchedBytes = 4;

// How many bytes of code from a place identify a service or an entry point.
constexpr std::size_t kCodeProbe = 12;

// A path as the tools take it: absolute, with the system's separators.
std::string native_text(const stdfs::path& p) {
    std::string s = fs::to_text(stdfs::absolute(p).lexically_normal());
    std::replace(s.begin(), s.end(), '/', '\\');
    return s;
}

std::string single_quoted(std::string_view s) {
    return "'" + std::string(s) + "'";
}

// -- running a tool -----------------------------------------------------------------------------------------------

struct Ran {
    bool ok = false;
    std::string log; // stdout then stderr, trimmed
};

Ran run_tool(const Tools& tools, const std::vector<std::string>& argv, const stdfs::path& cwd) {
    process::ProcessSpec spec;
    spec.argv = argv;
    spec.cwd = cwd;
    spec.env = tools.env;
    spec.timeout = kToolTimeout;
    try {
        const process::ProcessResult r = process::run(spec);
        if (r.timed_out)
            return {false, std::format("timed out after {} s", kToolTimeout.count())};
        Bytes both = r.out;
        append(both, r.err);
        return {r.exit_code == 0, std::string(text::trim(text::from_latin1(both)))};
    } catch (const ToolError& e) { // the program cannot be started at all
        return {false, e.what()};
    }
}

bool is_file(const stdfs::path& p) {
    std::error_code ec;
    return stdfs::is_regular_file(p, ec);
}

// -- the structural check --------------------------------------------------------------------------------------

// {object: the offsets the loader writes into}: each place patched is a 32-bit address.
using Patched = std::map<std::uint32_t, std::set<std::int64_t>>;

Patched patched_places(const le::Module& image) {
    Patched out;
    for (const le::Reference& r : image.references()) {
        std::set<std::int64_t>& at = out[r.at.object];
        for (std::int64_t i = 0; i < kPatchedBytes; ++i)
            at.insert(r.at.offset + i);
    }
    return out;
}

// An image's objects as loaded, read once.
struct Loaded {
    const le::Module& image;
    Patched patched;
    std::map<std::uint32_t, Bytes> data; // by object number

    explicit Loaded(const le::Module& m) : image(m), patched(patched_places(m)) {}

    const Bytes& object(std::uint32_t number) {
        auto it = data.find(number);
        if (it == data.end()) {
            const auto objects = image.objects();
            Bytes bytes;
            if (number >= 1 && number <= objects.size())
                bytes = image.object_data(objects[number - 1]);
            it = data.emplace(number, std::move(bytes)).first;
        }
        return it->second;
    }
    // The first bytes of code at a place, with the bytes the loader patches left out.
    Bytes code_at(const le::Place& where) {
        const Bytes& bytes = object(where.object);
        const auto patched_here = patched.find(where.object);
        Bytes out;
        if (where.offset < 0)
            return out;
        for (std::size_t i = static_cast<std::size_t>(where.offset);
             i < bytes.size() && i < static_cast<std::size_t>(where.offset) + kCodeProbe; ++i) {
            if (patched_here == patched.end() || !patched_here->second.contains(static_cast<std::int64_t>(i)))
                out.push_back(bytes[i]);
        }
        return out;
    }
};

bool is_set(const std::optional<le::Place>& p) {
    return p.has_value();
}

std::string export_text(const std::vector<Structure::Export>& exports) {
    std::string out = "[";
    for (std::size_t i = 0; i < exports.size(); ++i) {
        out += std::format("{}({}, {})", i == 0 ? "" : ", ", exports[i].name ? single_quoted(*exports[i].name) : "None",
                           exports[i].ordinal);
    }
    return out + "]";
}

void print_line(const std::string& line) {
    cb::out("{}\n", line);
}

std::string trimmed_line(std::string_view line) {
    return std::string(text::utf8_prefix(line, 120));
}

} // namespace

// ---- the tools -------------------------------------------------------------------------------------------------

stdfs::path tool(const watcom::OpenWatcom& open_watcom, std::string_view name) {
    if (!open_watcom.root().empty()) {
        const stdfs::path exe = open_watcom.root() / "binnt" / (std::string(name) + ".exe");
        if (is_file(exe))
            return exe;
    }
    return open_watcom.tool(name);
}

stdfs::path kit_folder(const stdfs::path& kit_source, const stdfs::path& copy_to) {
    if (kit_source.empty())
        return {};
    if (fs::to_text(kit_source).find(' ') == std::string::npos)
        return kit_source;
    if (!is_file(copy_to / "VMM.INC")) {
        fs::make_dirs(copy_to);
        for (const auto& entry : stdfs::directory_iterator(kit_source)) {
            if (entry.is_regular_file() && text::iends_with(fs::to_text(entry.path().filename()), ".INC")) {
                stdfs::copy_file(entry.path(), copy_to / entry.path().filename(),
                                 stdfs::copy_options::overwrite_existing);
            }
        }
    }
    return fs::to_text(copy_to).find(' ') != std::string::npos ? stdfs::path() : copy_to;
}

Tools find_tools(const watcom::OpenWatcom& open_watcom, const stdfs::path& kit_source, const stdfs::path& state) {
    Tools tools;
    tools.wasm = tool(open_watcom, "wasm");
    tools.wlink = tool(open_watcom, "wlink");
    tools.kit = kit_folder(kit_source, state / "driver_kit");
    tools.env = open_watcom.environment(watcom::Target::Win32);
    return tools;
}

stdfs::path source_of(const stdfs::path& path) {
    std::error_code ec;
    if (!stdfs::is_directory(path, ec))
        return path;
    stdfs::path folder = path.lexically_normal();
    if (!folder.has_filename())
        folder = folder.parent_path();
    const stdfs::path candidate = path / (folder.filename().string() + ".asm");
    if (!is_file(candidate))
        throw UsageError(std::format("{}: no {}.asm in it", fs::to_text(path), folder.filename().string()));
    return candidate;
}

std::optional<stdfs::path> original_of(const stdfs::path& source, std::span<const stdfs::path> candidates) {
    const std::string stem = text::lower(source.stem().string());
    for (const stdfs::path& candidate : candidates) {
        if (text::lower(candidate.stem().string()) == stem)
            return candidate;
    }
    return std::nullopt;
}

// ---- assembling and linking ------------------------------------------------------------------------------------

ToolResult assemble(const Tools& tools, const stdfs::path& source, const stdfs::path& work) {
    if (tools.wasm.empty())
        return {std::nullopt, "no assembler"};
    if (tools.kit.empty())
        return {std::nullopt, "the driver kit's include files were not found"};
    const stdfs::path obj = work / (source.stem().string() + ".obj");
    std::error_code ec;
    stdfs::remove(obj, ec);
    const Ran r = run_tool(tools,
                           {fs::to_text(tools.wasm), "-zq", "-dIS_32=1", "-i=" + fs::to_text(tools.kit),
                            "-fo=" + native_text(obj), native_text(source)},
                           work);
    return {r.ok && is_file(obj) ? std::optional<stdfs::path>(obj) : std::nullopt, r.log};
}

ToolResult link(const Tools& tools, const stdfs::path& obj, std::string_view device, const stdfs::path& out_in,
                const LinkOptions& options) {
    if (tools.wlink.empty())
        return {std::nullopt, "no linker"};
    const stdfs::path out =
        stdfs::absolute(out_in).lexically_normal(); // the linker runs with its folder as its directory
    stdfs::path script = out;
    script.replace_extension(".lnk");
    stdfs::path map = out;
    map.replace_extension(".map");

    std::vector<std::string> lines{std::string("format windows vxd") + (options.dynamic ? " virtdevice" : ""),
                                   "option quiet", "option map=" + native_text(map), "name " + native_text(out)};
    static const Regex description(R"(DESCRIPTION\s+'((?:[^']|'')*)')");
    if (const auto described = description.search(options.definitions)) {
        lines.insert(lines.begin() + 2, "option description '" + std::string(described->group(1)) + "'");
    }
    static const Regex export_line(R"(^\s+(\w+)\s+@(\d+))", RegexFlags::Multiline);
    std::vector<std::string> exports;
    export_line.for_each_match(options.definitions, [&](const RegexMatch& m) {
        exports.push_back(std::format("export {}.{}", m.group(1), m.group(2)));
        return true;
    });
    if (exports.empty())
        exports.push_back(std::format("export {}_DDB{}", device, options.named ? "" : ".1"));
    lines.insert(lines.end(), exports.begin(), exports.end());
    lines.push_back("file " + native_text(obj));
    if (options.classes.empty()) {
        for (const SegmentClass& c : kSegmentClasses)
            lines.push_back(std::format("segment class '{}' {}", c.name, c.attributes));
    } else {
        for (const auto& [name, attributes] : options.classes)
            lines.push_back(std::format("segment class '{}' {}", name, attributes));
    }
    fs::write_text(script, text::join(lines, "\r\n") + "\r\n");
    std::error_code ec;
    stdfs::remove(out, ec);
    const Ran r = run_tool(tools, {fs::to_text(tools.wlink), "@" + native_text(script)}, out.parent_path());
    return {r.ok && is_file(out) ? std::optional<stdfs::path>(out) : std::nullopt, r.log};
}

// ---- the structural check ----------------------------------------------------------------------------------------

Structure structure(const le::Module& image) {
    Structure s;
    for (const le::Object& o : image.objects())
        s.objects.push_back({o.vsize, o.flags});
    s.descriptor = image.descriptor();
    if (s.descriptor)
        s.services = image.service_table(*s.descriptor);
    for (const le::Export& e : image.exports())
        s.exports.push_back({e.name, e.ordinal});
    if (s.descriptor) {
        for (const le::Export& e : image.exports()) {
            if (e.object == s.descriptor->where.object &&
                static_cast<std::int64_t>(e.offset) == s.descriptor->where.offset) {
                s.export_is_descriptor = true;
            }
        }
    }
    s.module_flags = image.header().module_flags;
    s.references = image.references().size();
    return s;
}

Comparison compare(const le::Module& built, const le::Module& original) {
    const Structure a = structure(built), b = structure(original);
    Loaded loaded_a(built), loaded_b(original);
    Comparison c;
    auto fail = [&](bool& check, std::string_view name, const std::string& why) {
        check = false;
        c.notes.push_back(std::format("{}: {}", name, why));
    };

    if (a.objects != b.objects) {
        if (a.objects.size() != b.objects.size()) {
            fail(c.checks.objects, "objects",
                 std::format("{} objects, the original has {}", a.objects.size(), b.objects.size()));
        } else {
            std::size_t bad = 0, first = 0;
            for (std::size_t i = 0; i < a.objects.size(); ++i) {
                if (a.objects[i] == b.objects[i])
                    continue;
                if (bad++ == 0)
                    first = i;
            }
            fail(c.checks.objects, "objects",
                 std::format("{} of {} differ; object {} is {:#x} bytes flags {:#x}, original {:#x} flags {:#x}", bad,
                             b.objects.size(), first + 1, a.objects[first].size, a.objects[first].flags,
                             b.objects[first].size, b.objects[first].flags));
        }
    }

    if (!a.descriptor || !b.descriptor) {
        fail(c.checks.descriptor, "descriptor",
             std::format("none in the {}", !a.descriptor ? "rebuilt file" : "original"));
    } else {
        const le::Descriptor& da = *a.descriptor;
        const le::Descriptor& db = *b.descriptor;
        auto differs = [&](std::string_view field, const auto& x, const auto& y) {
            if (x == y)
                return false;
            if constexpr (std::is_convertible_v<decltype(x), std::string_view>) {
                fail(c.checks.descriptor, "descriptor",
                     std::format("{} is {}, original {}", field, single_quoted(x), single_quoted(y)));
            } else {
                fail(c.checks.descriptor, "descriptor", std::format("{} is {}, original {}", field, x, y));
            }
            return true;
        };
        const bool same_fields =
            !(differs("name", da.name, db.name) || differs("kit_version", da.kit_version, db.kit_version) ||
              differs("device_id", da.device_id, db.device_id) || differs("major", da.major + 0, db.major + 0) ||
              differs("minor", da.minor + 0, db.minor + 0) || differs("flags", da.flags, db.flags) ||
              differs("init_order", da.init_order, db.init_order) ||
              differs("service_count", da.service_count, db.service_count) || differs("size", da.size, db.size) ||
              differs("v86_api_address", da.v86_api_address, db.v86_api_address) ||
              differs("pm_api_address", da.pm_api_address, db.pm_api_address));
        if (same_fields) {
            // what the descriptor points at: present in both, and (for the entry points) starting with the same code
            struct Pointer {
                std::string_view name;
                const std::optional<le::Place>& built;
                const std::optional<le::Place>& original;
                bool compare_code;
            };
            const Pointer pointers[] = {{"control", da.at_control, db.at_control, true},
                                        {"v86_api", da.at_v86_api, db.at_v86_api, true},
                                        {"pm_api", da.at_pm_api, db.at_pm_api, true},
                                        {"service_table", da.at_service_table, db.at_service_table, false},
                                        {"reference_data", da.at_reference_data, db.at_reference_data, false}};
            for (const Pointer& p : pointers) {
                const bool both = is_set(p.built) && is_set(p.original);
                if (is_set(p.built) != is_set(p.original) ||
                    (both && p.compare_code && loaded_a.code_at(*p.built) != loaded_b.code_at(*p.original))) {
                    fail(c.checks.descriptor, "descriptor",
                         std::format("{} {}", p.name,
                                     both ? "starts with different code"
                                          : std::format("is {}, original {}", is_set(p.built) ? "set" : "empty",
                                                        is_set(p.original) ? "set" : "empty")));
                    break;
                }
            }
        }
    }

    if (a.services.size() != b.services.size()) {
        fail(c.checks.services, "services",
             std::format("{} services, the original has {}", a.services.size(), b.services.size()));
    } else {
        std::size_t bad = 0, first = 0;
        for (std::size_t i = 0; i < a.services.size(); ++i) {
            const auto& x = a.services[i];
            const auto& y = b.services[i];
            const bool differs = is_set(x) != is_set(y) || (x && y && loaded_a.code_at(*x) != loaded_b.code_at(*y));
            if (differs && bad++ == 0)
                first = i;
        }
        if (bad != 0)
            fail(c.checks.services, "services",
                 std::format("{} of {} differ, first service {}", bad, b.services.size(), first));
    }

    if (a.exports != b.exports || a.export_is_descriptor != b.export_is_descriptor) {
        fail(c.checks.exports, "exports",
             std::format("{}, original {}", export_text(a.exports), export_text(b.exports)));
    }
    if (a.module_flags != b.module_flags) {
        fail(c.checks.module, "module",
             std::format("module flags {:#x}, original {:#x}", a.module_flags, b.module_flags));
    }

    // the bytes of the objects that pair up, outside the places the loader patches
    std::uint64_t equal = 0, total = 0;
    const auto objects_a = built.objects();
    const auto objects_b = original.objects();
    for (std::size_t i = 0; i < std::min(objects_a.size(), objects_b.size()); ++i) {
        if (objects_a[i].vsize != objects_b[i].vsize)
            continue;
        const Bytes& ba = loaded_a.object(objects_a[i].index);
        const Bytes& bb = loaded_b.object(objects_b[i].index);
        // (only the places that lie inside the object leave the count: DIVERGES, see driver_test.cpp)
        std::vector<bool> skip(ba.size(), false);
        auto skip_patched = [&](const Patched& patched, std::uint32_t object) {
            const auto it = patched.find(object);
            if (it == patched.end())
                return;
            for (const std::int64_t at : it->second) {
                if (at >= 0 && static_cast<std::size_t>(at) < skip.size())
                    skip[static_cast<std::size_t>(at)] = true;
            }
        };
        skip_patched(loaded_a.patched, objects_a[i].index);
        skip_patched(loaded_b.patched, objects_b[i].index);
        for (std::size_t at = 0; at < ba.size(); ++at) {
            if (skip[at])
                continue;
            ++total;
            if (at < bb.size() && ba[at] == bb[at])
                ++equal;
        }
    }
    if (a.references != b.references)
        c.notes.push_back(std::format("places the loader patches: {}, original {}", a.references, b.references));
    if (total != 0)
        c.same = static_cast<double>(equal) / static_cast<double>(total);
    return c;
}

// ---- the whole build -------------------------------------------------------------------------------------------

BuildResult build(const Tools& tools, const BuildRequest& request) {
    BuildResult r;
    r.source = source_of(request.source);
    r.name = text::lower(r.source.stem().string());
    r.original = request.original;
    fs::make_dirs(request.work);

    static const Regex declares(R"(^\s*Declare_Virtual_Device\s+(\w+))",
                                RegexFlags::IgnoreCase | RegexFlags::Multiline);
    const std::string source_text = fs::read_text(r.source);
    const auto device = declares.search(source_text);
    if (!device) {
        r.log = "the source declares no device (no Declare_Virtual_Device)";
        return r;
    }
    const ToolResult assembled = assemble(tools, r.source, request.work);
    r.log = assembled.log;
    if (!assembled.file)
        return r;
    r.assembled = true;

    // the original: its member of the same name (or the first), for the kind of module and the file's extension
    std::optional<le::Module> original_module;
    std::string extension = ".vxd";
    if (request.original) {
        try {
            std::vector<std::pair<std::string, le::Module>> members = le::open_modules(*request.original);
            if (!members.empty()) {
                auto chosen = std::find_if(members.begin(), members.end(),
                                           [&](const auto& m) { return text::lower(m.first) == r.name; });
                original_module = std::move((chosen != members.end() ? *chosen : members.front()).second);
                if (members.size() == 1)
                    extension = request.original->extension().string();
            }
        } catch (const FormatError& e) { // not a driver: nothing to check against
            r.notes.push_back(std::format("the original could not be read: {}", e.what()));
        }
    }
    LinkOptions options;
    options.dynamic = original_module && original_module->header().module_flags == kDynamicModuleFlags;
    const ToolResult linked =
        link(tools, *assembled.file, device->group(1), request.work / (r.name + extension), options);
    r.log = std::string(text::trim(r.log + "\n" + linked.log));
    if (!linked.file)
        return r;
    r.linked = true;
    r.path = linked.file;

    if (original_module) {
        try {
            const Comparison c = compare(le::Module::open(*linked.file), *original_module);
            r.checks = c.checks;
            r.notes = c.notes;
            r.same = c.same;
        } catch (const FormatError& e) { // a file the reader cannot take apart
            r.notes = {std::format("the rebuilt file could not be read: {}", e.what())};
        }
    }
    return r;
}

std::string report(const BuildResult& r) {
    const std::vector<std::string_view> log_lines = text::lines(r.log);
    if (!r.assembled) {
        return std::format("{:<12} does not assemble: {}", r.name,
                           log_lines.empty() ? "?" : trimmed_line(log_lines.front()));
    }
    if (!r.linked) {
        return std::format("{:<12} assembles, does not link: {}", r.name,
                           log_lines.empty() ? "?" : trimmed_line(log_lines.back()));
    }
    if (!r.checks) {
        return std::format("{:<12} built ({})", r.name,
                           r.notes.empty() ? "no original to check against" : r.notes.front());
    }
    auto mark = [](std::string_view name, bool same) { return std::format("{} {}", name, same ? "same" : "DIFFERS"); };
    const std::string marks = std::format(
        "{} {} {} {} {}", mark("objects", r.checks->objects), mark("descriptor", r.checks->descriptor),
        mark("services", r.checks->services), mark("exports", r.checks->exports), mark("module", r.checks->module));
    const std::string same = r.same ? std::format("  {:.1f}% of paired bytes equal", 100.0 * *r.same) : "";
    return std::format("{:<12} built  {}{}", r.name, marks, same);
}

// ---- the command -------------------------------------------------------------------------------------------------

cli::Command command(const Environment& environment) {
    return cli::Command{
        "driver-build",
        "build a virtual device driver from assembly source and check it against the original",
        {cli::Positional{"source", "a source folder or .asm", true, true},
         cli::Option{"--original", "",
                     "the driver to check against (a file or a corpus name); found by name when not given",
                     cli::Arg::Value, "FILE"},
         cli::Option{"--out", "", "where the objects and the image go (default: the state folder)", cli::Arg::Value,
                     "DIR"}},
        [environment](const cli::Args& args) {
            int bad = 0;
            const std::optional<std::string> original_name = args.value("original");
            const std::optional<std::string> out = args.value("out");
            for (const std::string& src : args.values("source")) {
                try {
                    BuildRequest request;
                    request.source = fs::from_text(src);
                    const stdfs::path source = source_of(request.source);
                    if (original_name) {
                        request.original = environment.retail ? environment.retail(*original_name) : std::nullopt;
                    } else {
                        request.original = original_of(source, environment.corpus_drivers);
                    }
                    request.work = out ? fs::from_text(*out)
                                       : environment.state / "driver_build" / text::lower(source.stem().string());
                    const BuildResult r = build(environment.tools, request);
                    print_line(report(r));
                    for (const std::string& note : r.notes)
                        print_line("    " + note);
                    if (r.path)
                        print_line("    -> " + fs::to_text(*r.path));
                    if (!(r.linked && (!r.checks || r.checks->all())))
                        ++bad;
                } catch (const UsageError& e) {
                    print_line(e.what());
                    ++bad;
                }
            }
            return bad != 0 ? 1 : 0;
        },
        {},
        "Build",
        ""};
}

} // namespace cb::build::driver
