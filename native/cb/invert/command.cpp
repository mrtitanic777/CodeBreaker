#include "cb/invert/command.hpp"

#include <algorithm>
#include <array>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/omf.hpp"
#include "cb/invert/early_c.hpp"
#include "cb/invert/early_c_forward.hpp"
#include "cb/invert/known_differences.hpp"
#include "cb/invert/quad_align.hpp"
#include "cb/invert/quad_dump.hpp"

namespace cb::invert::command {

namespace {

namespace stdfs = std::filesystem;

constexpr std::array<Tool, 19> kTools{{
    {"early-c", "FILE OFFSET LENGTH -> C", true},
    {"early-c-check", "... and put the text to Microsoft C 2.03", false},
    {"early-c-frame", "the stack-frame model against its table", false},
    {"early-c-round-trip", "compile -> read -> compile, for locals", false},
    {"early-c-frame-probe", "how the compiler places locals", false},
    {"early-c-order-probe", "how it orders loads and stores", false},
    {"early-c-forward", "the forward model against a quad file and its object", true},
    {"quads", "list a quad file", true},
    {"quad-align", "a quad file beside its object, per function", true},
    {"omf", "list an object file or library", true},
    {"msc6", "the Microsoft C 6.0a model", false},
    {"msc6-templates", "the statement templates", false},
    {"msc6-function", "one function through the model", false},
    {"msc6-registers", "register-value tracking", false},
    {"msc6-walk", "the control-flow walk", false},
    {"msc6-idioms", "what each compiler switch changes", false},
    {"known-differences", "the divergence knowledge base", true},
    {"msvc2", "the Microsoft C/C++ 2.0 (32-bit) model", false},
    {"msvc2-forward", "the forward model against its compiler", false},
}};

// The models the Python's --check runs, in its order.
constexpr std::array<std::string_view, 8> kChecks{"early-c-frame", "early-c-round-trip", "msc6-function", "msc6-registers",
                                                  "msc6-walk",      "msc6",               "msvc2-forward",  "msvc2"};

const Tool* find_tool(std::string_view name) {
    for (const Tool& t : kTools)
        if (t.name == name) return &t;
    return nullptr;
}

int not_native(const Tool& tool) {
    err("invert --tool {}: not native yet ({}). Its model is the C++ of native/invert, not yet a typed module of native/cb.\n", tool.name, tool.summary);
    return 2;
}

// early-c FILE OFFSET LENGTH: the function at OFFSET (hex) of FILE, LENGTH bytes, as C.
int early_c_tool(std::span<const std::string> args) {
    if (args.size() < 3) throw UsageError("early-c FILE OFFSET LENGTH   (OFFSET in hex)");
    const Bytes image = fs::read_file(fs::from_text(args[0]));
    const auto offset = text::parse_int<std::size_t>(args[1], 16);
    const auto length = text::parse_int<std::size_t>(args[2], 10);
    if (!offset || !length) throw UsageError("early-c: the offset is hex and the length decimal");
    if (*offset > image.size() || *length > image.size() - *offset) throw UsageError(std::format("early-c: {} + {} is past the end of the file ({} bytes)", *offset, *length, image.size()));
    const early_c::Recovered r = early_c::recover(ByteView(image).subspan(*offset, *length), 0, {}, early_c::Options::from_environment());
    out("{}\n", early_c::to_c(std::format("f{:05x}", *offset), r.frame_size, r.params, r.statements, r.return_type, r.locals));
    return 0;
}

int quads_tool(std::span<const std::string> args) {
    if (args.empty()) {
        out("{}\n", quad_dump::usage());
        return 1;
    }
    out("{}", quad_dump::dump(quad_dump::parse(fs::read_file(fs::from_text(args[0]))), args[0]));
    return 0;
}

int quad_align_tool(std::span<const std::string> args) {
    if (args.size() < 2) throw UsageError("quad-align QUADFILE OBJECT");
    out("{}", quad_align::report(fs::read_file(fs::from_text(args[0])), fs::read_file(fs::from_text(args[1]))));
    return 0;
}

int omf_tool(std::span<const std::string> args) {
    if (args.empty()) throw UsageError("omf FILE.obj|FILE.lib [--raw]   each module's segments, publics and externals; --raw every record");
    out("{}", formats::omf::listing(fs::read_file(fs::from_text(args[0])), std::ranges::find(args, "--raw") != args.end()));
    return 0;
}

int known_differences_tool() {
    out("{}", KnownDifferences().listing());
    return 0;
}

int run_tool(const Tool& tool, std::span<const std::string> rest) {
    if (!tool.native) return not_native(tool);
    if (tool.name == "early-c") return early_c_tool(rest);
    if (tool.name == "early-c-forward") return early_c_forward::command(rest);
    if (tool.name == "quads") return quads_tool(rest);
    if (tool.name == "quad-align") return quad_align_tool(rest);
    if (tool.name == "omf") return omf_tool(rest);
    if (tool.name == "known-differences") return known_differences_tool();
    return not_native(tool);
}

}  // namespace

std::span<const Tool> tools() { return kTools; }

std::string usage() {
    std::string names;
    for (const Tool& t : tools()) names += (names.empty() ? "" : ", ") + std::string(t.name);
    return "codebreaker invert <16-bit module> [--first N] [--flags \"/AM /G2 /Gsw /Ox\"] [--show NAME]\n"
           "codebreaker invert --early FILE OFFSET LENGTH     one function of a program built with the C compiler of 1983-84 (offset in hex)\n"
           "codebreaker invert --check [MODEL]                each model against its own compiler\n"
           "codebreaker invert --tool NAME [arguments]        a model's own command line: " + names + "\n";
}

int run(std::span<const std::string> args, const Environment& environment) {
    const auto at = [&](std::string_view flag) { return std::ranges::find(args, flag); };
    if (const auto tool = at("--tool"); tool != args.end()) {
        const std::string name = tool + 1 != args.end() ? *(tool + 1) : std::string();
        const Tool* found = find_tool(name);
        if (found == nullptr) throw UsageError(std::format("no model '{}' (have: {})", name, usage().substr(usage().rfind(": ") + 2, std::string::npos)));
        return run_tool(*found, std::span<const std::string>(tool + (tool + 1 != args.end() ? 2 : 1), args.end()));
    }
    if (const auto early = at("--early"); early != args.end()) return run_tool(*find_tool("early-c"), std::span<const std::string>(early + 1, args.end()));
    if (at("--check") != args.end()) {
        std::vector<std::string_view> named;
        for (const std::string& a : args)
            if (!a.starts_with("--")) named.push_back(a);
        if (named.empty()) named.assign(kChecks.begin(), kChecks.end());
        int worst = 0;
        for (const std::string_view name : named) {
            out("== {}\n", name);
            const Tool* found = find_tool(name);
            if (found == nullptr) throw UsageError(std::format("no model '{}'", name));
            worst = std::max(worst, run_tool(*found, {}));
        }
        return worst;
    }
    std::vector<std::string> positional;
    for (std::size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "--flags" || args[i] == "--first" || args[i] == "--show") {
            ++i;
            continue;
        }
        if (!args[i].starts_with("--")) positional.push_back(args[i]);
    }
    if (positional.empty()) {
        out("{}", usage());
        return 2;
    }
    // the module pass: the 16-bit module's functions read by the models, each text put to the real compiler
    const std::optional<stdfs::path> file = stdfs::is_regular_file(fs::from_text(positional[0])) ? std::optional<stdfs::path>(fs::from_text(positional[0]))
                                                                                                 : (environment.retail ? environment.retail(positional[0]) : std::nullopt);
    if (!file) throw NotFoundError(fs::from_text(positional[0]), 0, "no module of that name or path");
    err("invert {}: the module pass is not native yet: it runs the msc6 / msvc2 models, which are not in native/cb.\n", fs::to_text(file->filename()));
    return 2;
}

std::function<int(std::span<const std::string>)> runner(const Environment& environment) {
    return [environment](std::span<const std::string> args) { return run(args, environment); };
}

}  // namespace cb::invert::command
