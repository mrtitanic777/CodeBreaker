#include "cb/invert/command.hpp"

#include <algorithm>
#include <array>
#include <set>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/omf.hpp"
#include "cb/invert/early_c.hpp"
#include "cb/invert/early_c_forward.hpp"
#include "cb/invert/early_c_frame.hpp"
#include "cb/invert/msc6_small.hpp"
#include "cb/invert/known_differences.hpp"
#include "cb/invert/model_tools.hpp"
#include "cb/invert/quad_align.hpp"
#include "cb/invert/quad_dump.hpp"

namespace cb::invert::command {

namespace {

namespace stdfs = std::filesystem;

constexpr std::array<Tool, 19> kTools{{
    {"early-c", "FILE OFFSET LENGTH -> C"},
    {"early-c-check", "... and put the text to Microsoft C 2.03"},
    {"early-c-frame", "the stack-frame model against its table"},
    {"early-c-round-trip", "compile -> read -> compile, for locals"},
    {"early-c-frame-probe", "how the compiler places locals"},
    {"early-c-order-probe", "how it orders loads and stores"},
    {"early-c-forward", "the forward model against a quad file and its object"},
    {"quads", "list a quad file"},
    {"quad-align", "a quad file beside its object, per function"},
    {"omf", "list an object file or library"},
    {"msc6", "the Microsoft C 6.0a model"},
    {"msc6-templates", "the statement templates"},
    {"msc6-function", "one function through the model"},
    {"msc6-registers", "register-value tracking"},
    {"msc6-walk", "the control-flow walk"},
    {"msc6-idioms", "what each compiler switch changes"},
    {"known-differences", "the divergence knowledge base"},
    {"msvc2", "the Microsoft C/C++ 2.0 (32-bit) model"},
    {"msvc2-forward", "the forward model against its compiler"},
}};

// The models --check runs, in this order.
constexpr std::array<std::string_view, 8> kChecks{"early-c-frame", "early-c-round-trip", "msc6-function", "msc6-registers",
                                                  "msc6-walk",      "msc6",               "msvc2-forward",  "msvc2"};

const Tool* find_tool(std::string_view name) {
    for (const Tool& t : kTools)
        if (t.name == name) return &t;
    return nullptr;
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

// ---- early-c-frame ----------------------------------------------------------------------------------------------------------------------

// The stack frames of small functions as the C compiler of 1983-84 laid them out (the oracle harness's seed set): the temporaries, the locals and
// the parameters the source declared, the frame size the compiler gave, and the frame offsets its code reached.
struct FrameCase {
    std::string_view name;
    std::vector<std::string> temps;
    std::vector<early_c_frame::Local> locals;
    std::vector<early_c_frame::Param> params;
    std::uint32_t frame_size;
    std::vector<early_c_frame::Offset> reached;
};

const std::vector<FrameCase>& frame_cases() {
    using L = early_c_frame::Local;
    using P = early_c_frame::Param;
    static const std::vector<FrameCase> cases{
        {"f01", {}, {L{"a", "int", 1}}, {}, 2, {0}},
        {"f02", {}, {L{"a", "int", 1}, L{"b", "int", 1}}, {}, 4, {0, 2}},
        {"f03", {}, {L{"a", "int", 1}, L{"b", "int", 1}, L{"c", "int", 1}}, {}, 6, {0, 2, 4}},
        {"f04", {}, {L{"a", "int", 1}, L{"b", "int", 1}}, {}, 4, {0, 2}},
        {"f08", {}, {L{"a", "int", 1}, L{"b", "int", 1}, L{"c", "int", 1}}, {}, 6, {0, 4}},   // b unused
        {"f11", {}, {L{"a", "int", 1}, L{"b", "int", 1}, L{"c", "int", 1}, L{"d", "int", 1}}, {}, 8, {0, 2, 4, 6}},
        {"f12", {"int"}, {L{"a", "int", 1}, L{"b", "int", 1}, L{"c", "int", 1}}, {P{"p", "int"}}, 8, {2, 4, 6, 12}},
        {"f13", {"int"}, {L{"a", "int", 1}, L{"b", "int", 1}}, {}, 6, {2, 4}},
        {"f14", {"long"}, {L{"a", "int", 1}, L{"b", "long", 1}}, {P{"p", "int"}}, 10, {4, 6, 8, 14}},
        {"f15", {"int"}, {L{"a", "int", 1}, L{"b", "int", 1}}, {}, 6, {2, 4}},
        {"g01", {"int"}, {L{"a", "int", 1}, L{"b", "int", 1}}, {}, 6, {2, 4}},
        {"g02", {"int", "int"}, {L{"a", "int", 1}, L{"b", "int", 1}, L{"c", "int", 1}}, {}, 10, {4, 6, 8}},
        {"g03", {"long"}, {L{"a", "long", 1}, L{"b", "long", 1}}, {}, 12, {4, 6, 8, 10}},
        {"g04", {"long"}, {L{"a", "long", 1}, L{"b", "int", 1}}, {}, 10, {4, 6, 8}},
        {"g05", {}, {L{"a", "int", 1}, L{"b", "long", 1}, L{"c", "long", 1}}, {}, 10, {0}},
        {"g06", {}, {L{"a", "char", 1}, L{"b", "char", 1}}, {}, 2, {0, 1}},
        {"g07", {}, {L{"a", "int", 1}, L{"b", "char", 1}, L{"c", "long", 1}}, {}, 8, {0, 2, 3, 5}},
        {"g08", {}, {L{"a", "int", 3}}, {}, 6, {0}},
        {"g09", {}, {L{"a", "int", 1}, L{"b", "int", 1}, L{"c", "int", 1}}, {}, 6, {0, 2, 4}},
        {"g10", {"int"}, {L{"a", "int", 1}}, {P{"p", "int"}}, 4, {2, 8}},
        {"g11", {"int"}, {L{"a", "int", 1}, L{"b", "int", 1}, L{"c", "int", 1}}, {}, 8, {2, 4, 6}},
        {"g12", {"int"}, {L{"a", "int", 1}}, {}, 4, {2}},
        {"g13", {"int"}, {L{"a", "int", 1}, L{"b", "int", 1}}, {}, 6, {2, 4}},
        {"g14", {"long"}, {L{"a", "long", 1}, L{"b", "long", 1}, L{"c", "long", 1}}, {}, 16, {4, 6, 8, 10, 12, 14}},
        {"g15", {}, {L{"a", "int", 1}, L{"b", "int", 1}, L{"c", "int", 1}, L{"d", "int", 1}, L{"e", "int", 1}}, {}, 10, {0, 2, 4, 6, 8}},
    };
    return cases;
}

std::string offsets_text(const std::set<early_c_frame::Offset>& offsets) {
    std::vector<std::string> parts;
    for (const early_c_frame::Offset o : offsets) parts.push_back(std::to_string(o));
    return "[" + text::join(parts, ", ") + "]";
}

// early-c-frame: the frame model against the compiler's own frames. Each case must give the frame size and cover every offset the code reached.
int early_c_frame_tool() {
    std::size_t good_cases = 0;
    for (const FrameCase& c : frame_cases()) {
        const early_c_frame::Layout layout = early_c_frame::layout(c.temps, c.locals, c.params);
        std::set<early_c_frame::Offset> predicted;   // every local slot, the high word of a long, and the parameters
        for (const early_c_frame::Local& local : c.locals) {
            const early_c_frame::Offset base = layout.local_offsets.at(local.name);
            const std::uint32_t size = early_c_frame::type_size(local.type);
            for (std::uint32_t k = 0; k < local.count; ++k) {
                predicted.insert(base + k * size);
                if (local.type == "long" || local.type == "float") predicted.insert(base + k * size + 2);
            }
        }
        for (const early_c_frame::Param& param : c.params) predicted.insert(layout.param_offsets.at(param.name));
        bool good = layout.frame_size == c.frame_size;
        for (const early_c_frame::Offset d : c.reached)
            if (!predicted.contains(d)) good = false;
        if (good) {
            ++good_cases;
        } else {
            out("  {:<5} FAIL  fs={}/{}  pred={}  obs={}\n", c.name, layout.frame_size, c.frame_size, offsets_text(predicted), offsets_text({c.reached.begin(), c.reached.end()}));
        }
    }
    out("framemodel self-test: {}/{} byte-exact frame layouts\n", good_cases, frame_cases().size());
    return 0;
}

// ---- the small readers of Microsoft C 6.0a ----------------------------------------------------------------------------------------------

// The functions the self-tests read: LZEXPAND's Cleanup (the long-global free idiom, with its three globals), and a function that keeps a
// call's result in SI.
constexpr std::string_view kCleanupCode =
    "1e589045558bec1e8ed883ec02a146000b0644007411ff3646009ae70000002bc0a34600a34400a14a000b0648007411ff364a009a010100002bc0a34a00a34800a156000b0654007411ff3656"
    "009affff00002bc0a35600a354004d4d8be51f5d4dcb90";
constexpr std::string_view kRegisterCode = "1e589045558bec1e8ed883ec02568b7606569a00000000508bf09a000000008bc65e4d4d8be51f5d4dca0200";
constexpr std::string_view kLeafFlags[] = {"/AM /G2 /Gsw /Ox", "/AM /G2 /Gsw /Os"};

Bytes bytes_from_hex(std::string_view hex) {
    if (hex.size() % 2 != 0) throw UsageError("the bytes are hexadecimal digits, two to a byte");
    Bytes out;
    for (std::size_t i = 0; i < hex.size(); i += 2) {
        const auto b = text::parse_int<std::uint32_t>(hex.substr(i, 2), 16);
        if (!b || hex[i] == '-' || hex[i] == '+') throw UsageError("the bytes are hexadecimal digits, two to a byte");
        out.push_back(static_cast<std::uint8_t>(*b));
    }
    return out;
}

std::string hex_text(ByteView b) {
    std::string out;
    for (const std::uint8_t x : b) out += std::format("{:02x}", x);
    return out;
}

// The text through the compiler. Nothing when it is not set up (said, and the tool goes without); an empty answer when it refuses the text.
std::optional<std::optional<Bytes>> through_msc6(const Environment& environment, std::string_view text, std::string_view flags) {
    if (!environment.compile_msc6) {
        err("Microsoft C 6 is not set up (settings.json: dosbox_x, msc6): the text was not put to the compiler\n");
        return std::nullopt;
    }
    std::optional<Bytes> built = environment.compile_msc6(text, flags);
    if (!built) err("Microsoft C 6 gave no object (flags={})\n", flags);
    return built;
}

// msc6-templates HEX [--verify]: the leaf template the bytes are, as C; --verify puts it to the compiler under each switch set.
int msc6_templates_tool(std::span<const std::string> args, const Environment& environment) {
    const auto word = std::ranges::find_if(args, [](const std::string& a) { return !a.starts_with("--"); });
    if (word == args.end()) throw UsageError("msc6-templates HEXBYTES [--verify]   the leaf templates of Microsoft C 6.00a (medium model, optimized)");
    const Bytes code = bytes_from_hex(*word);
    const Result<msc6_small::Leaf> read = msc6_small::leaf(code);
    if (!read.ok()) {
        out("DECLINED: {}\n", read.error().what());
        return 1;
    }
    out("{}\n", read.value().text);
    if (std::ranges::find(args, "--verify") == args.end()) return 0;
    for (const std::string_view flags : kLeafFlags) {
        const auto built = through_msc6(environment, read.value().text, flags);
        if (!built) return 2;
        if (!*built) return 1;
        if (**built == code) {
            out("// VERIFIED byte-exact ({}): recompiles to the identical {} bytes\n", flags, code.size());
            return 0;
        }
    }
    out("// NOT byte-exact under the tried flags -- template incomplete for this function\n");
    return 0;
}

// What a recompiled function and the original are compared under: the same length, and the same bytes outside the places a linker fills in.
bool masked_equal(ByteView built, ByteView original) { return built.size() == original.size() && msc6_small::mask(built) == msc6_small::mask(original); }

// msc6-function: LZEXPAND's Cleanup through the free-idiom reader and the compiler.
int msc6_function_tool(const Environment& environment) {
    const Bytes code = bytes_from_hex(kCleanupCode);
    const Result<std::string> read = msc6_small::cleanup(code, {{0x44, "gpA"}, {0x48, "gpB"}, {0x54, "gpC"}});
    if (!read.ok()) {
        out("DECLINED: {}\n", read.error().what());
        return 1;
    }
    out("=== recovered C ===\n{}\n\n", read.value());
    const auto built = through_msc6(environment, read.value(), kLeafFlags[0]);
    if (!built || !*built) return built ? 1 : 2;
    const bool same = masked_equal(**built, code);
    out("recompiled {} bytes vs shipped {}; masked-equal: {}\n", (*built)->size(), code.size(), same ? "True" : "False");
    if (same) out("*** WHOLE-FUNCTION VERIFIED byte-exact (relocs masked) ***\n");
    else out("*** mismatch:\n  shipped {}\n  ours    {}\n", hex_text(msc6_small::mask(code)), hex_text(msc6_small::mask(**built)));
    return 0;
}

// msc6-walk: the same function through the statement walker.
int msc6_walk_tool(const Environment& environment) {
    const Bytes code = bytes_from_hex(kCleanupCode);
    const msc6_small::Globals globals{{0x44, {"gpA", msc6_small::Width::Long}}, {0x48, {"gpB", msc6_small::Width::Long}}, {0x54, {"gpC", msc6_small::Width::Long}}};
    const Result<msc6_small::Walked> read = msc6_small::walk(code, globals, "GlobalFree");
    if (!read.ok()) {
        out("DECLINED: {}\n", read.error().what());
        return 1;
    }
    out("{}\n", read.value().text);
    const auto built = through_msc6(environment, text::replace_all(read.value().text, "void far F", "void far Cleanup"), kLeafFlags[0]);
    if (!built || !*built) return built ? 1 : 2;
    out("\n// generalized walker: masked-equal = {} ({} stmts)\n", masked_equal(**built, code) ? "True" : "False", read.value().statements);
    return 0;
}

// msc6-registers: a function that keeps a call's result in a register variable, through the register tracker.
int msc6_registers_tool(const Environment& environment) {
    const Bytes code = bytes_from_hex(kRegisterCode);
    const Result<msc6_small::Registers> read = msc6_small::registers(code, {{0, "G"}, {1, "H"}});
    if (!read.ok()) {
        out("DECLINED: {}\n", read.error().what());
        return 0;
    }
    out("{}\n", read.value().text);
    const auto built = through_msc6(environment, read.value().text, kLeafFlags[0]);
    if (!built || !*built) return built ? 1 : 2;
    out("\n// register-var tracker: masked-equal = {} ({} vs {} B)\n", masked_equal(**built, code) ? "True" : "False", (*built)->size(), code.size());
    return 0;
}

int run_tool(const Tool& tool, std::span<const std::string> rest, const Environment& environment) {
    const std::string_view name = tool.name;
    if (name == "early-c-frame") return early_c_frame_tool();
    if (name == "msc6-templates") return msc6_templates_tool(rest, environment);
    if (name == "msc6-function") return msc6_function_tool(environment);
    if (name == "msc6-walk") return msc6_walk_tool(environment);
    if (name == "msc6-registers") return msc6_registers_tool(environment);
    if (name == "msc6") return tools::msc6(environment);
    if (name == "msc6-idioms") return tools::msc6_idioms(environment);
    if (name == "msvc2") return tools::msvc2(environment, rest);
    if (name == "msvc2-forward") return tools::msvc2_forward(environment, rest);
    if (name == "early-c-check") return tools::early_c_check(environment, rest);
    if (name == "early-c-round-trip") return tools::early_c_round_trip(environment);
    if (name == "early-c-frame-probe") return tools::early_c_frame_probe(environment);
    if (name == "early-c-order-probe") return tools::early_c_order_probe(environment);
    if (name == "early-c") return early_c_tool(rest);
    if (name == "early-c-forward") return early_c_forward::command(rest);
    if (name == "quads") return quads_tool(rest);
    if (name == "quad-align") return quad_align_tool(rest);
    if (name == "omf") return omf_tool(rest);
    if (name == "known-differences") return known_differences_tool();
    throw InternalError(std::format("invert: the tool '{}' is in the table and has no code", name));
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
        return run_tool(*found, std::span<const std::string>(tool + (tool + 1 != args.end() ? 2 : 1), args.end()), environment);
    }
    if (const auto early = at("--early"); early != args.end()) return run_tool(*find_tool("early-c"), std::span<const std::string>(early + 1, args.end()), environment);
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
            worst = std::max(worst, run_tool(*found, {}, environment));
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
    return tools::module(environment, args);
}

std::function<int(std::span<const std::string>)> runner(const Environment& environment) {
    return [environment](std::span<const std::string> args) { return run(args, environment); };
}

}  // namespace cb::invert::command
