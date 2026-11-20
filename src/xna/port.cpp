// cb-unity: alone
#include "xna/port.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <format>
#include <fstream>

#include <nlohmann/json.hpp>

#include "core/crypto.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/log.hpp"
#include "core/process.hpp"
#include "core/regex.hpp"
#include "core/text.hpp"
#include "xna/mgfx.hpp"
#include "xna/run.hpp"

namespace cb::xna::port {

namespace fs = std::filesystem;

namespace {

constexpr std::array<std::string_view, 4> kStubbed = {"GamerServices", "Net", "Storage", "Avatar"};
// what the stubs do not generate: the runtime writes these by hand
constexpr std::array<std::string_view, 7> kHandWritten = {"Guide", "Gamer", "SignedInGamer", "SignedInGamerCollection",
                                                         "GamerServicesDispatcher", "StorageDevice", "StorageContainer"};
constexpr std::array<std::string_view, 5> kXactTypes = {"AudioEngine", "WaveBank", "SoundBank", "Cue", "AudioCategory"};

std::map<std::string, std::string> dotnet_env() {
    return {{"DOTNET_ROLL_FORWARD", "LatestMajor"}, {"DOTNET_CLI_TELEMETRY_OPTOUT", "1"}, {"DOTNET_NOLOGO", "1"}, {"MSBUILDDISABLENODEREUSE", "1"}};
}

bool has(std::string_view hay, std::string_view needle) { return hay.find(needle) != std::string_view::npos; }

bool is_word_char(char c) { return std::isalnum(static_cast<unsigned char>(c)) || c == '_'; }

// `word` standing as a word of `text`.
bool has_word(std::string_view text, std::string_view word) {
    for (std::size_t at = text.find(word); at != std::string_view::npos; at = text.find(word, at + 1)) {
        const bool before = at == 0 || !is_word_char(text[at - 1]);
        const bool after = at + word.size() >= text.size() || !is_word_char(text[at + word.size()]);
        if (before && after) return true;
    }
    return false;
}

// A program found on PATH (with each of PATHEXT's extensions).
std::optional<fs::path> which(std::string_view name) {
    const auto path = cb::fs::get_env("PATH");
    if (!path) return std::nullopt;
    std::vector<std::string> exts = {""};
    if (const auto pathext = cb::fs::get_env("PATHEXT"))
        for (const std::string_view e : text::split(*pathext, ';'))
            if (!e.empty()) exts.emplace_back(e);
    for (const std::string_view dir : text::split(*path, ';')) {
        if (dir.empty()) continue;
        for (const std::string& ext : exts) {
            const fs::path candidate = cb::fs::from_text(dir) / (std::string(name) + ext);
            std::error_code ec;
            if (fs::is_regular_file(candidate, ec)) return candidate;
        }
    }
    return std::nullopt;
}

// Source text as the compilers read it: a UTF-8 byte-order mark is not part of it. Written back without one.
std::string read_source(const fs::path& p) {
    std::string t = cb::fs::read_text(p);
    if (t.starts_with("\xEF\xBB\xBF")) t.erase(0, 3);
    return t;
}

std::string lower_extension(const fs::path& p) { return text::lower(p.extension().string()); }

// Every file under `folder` with the extension (lower case, with the dot), in path order.
std::vector<fs::path> files_with(const fs::path& folder, std::string_view extension) {
    std::vector<fs::path> out;
    std::error_code ec;
    if (!fs::is_directory(folder, ec)) return out;
    for (fs::recursive_directory_iterator it(folder, fs::directory_options::skip_permission_denied, ec), end; it != end; it.increment(ec)) {
        if (ec) break;
        if (it->is_regular_file(ec) && lower_extension(it->path()) == extension) out.push_back(it->path());
    }
    std::sort(out.begin(), out.end());
    return out;
}

std::string relative_text(const fs::path& p, const fs::path& base) {
    const fs::path rel = p.lexically_relative(base);
    return (rel.empty() || *rel.begin() == "..") ? cb::fs::to_text(p) : rel.generic_string();
}

std::string hex_text(std::span<const std::uint8_t> bytes) {
    std::string out;
    for (const std::uint8_t b : bytes) out += std::format("{:02x}", static_cast<unsigned>(b));
    return out;
}

// src over dst; a file already there with the same size and time is left as it is. obj, bin and .git folders are not copied, nor an
// overlay's manifest.
void copy_over(const fs::path& src, const fs::path& dst) {
    std::error_code ec;
    if (!fs::is_directory(src, ec)) return;
    fs::create_directories(dst);
    for (const fs::directory_entry& e : fs::directory_iterator(src)) {
        const std::string name = e.path().filename().string();
        if (e.is_directory()) {
            if (name == "obj" || name == "bin" || name == ".git") continue;
            copy_over(e.path(), dst / name);
        } else if (e.is_regular_file() && name != "overlay.json") {
            const fs::path to = dst / name;
            if (fs::is_regular_file(to, ec) && fs::file_size(to) == fs::file_size(e.path())) {
                const auto secs = [](const fs::path& p) { return std::chrono::duration_cast<std::chrono::seconds>(cb::fs::last_write_time(p).time_since_epoch()).count(); };
                if (secs(to) == secs(e.path())) continue;
            }
            fs::copy_file(e.path(), to, fs::copy_options::overwrite_existing);
            cb::fs::set_last_write_time(to, cb::fs::last_write_time(e.path()));
        }
    }
}

std::string tail_of(std::string_view s, std::size_t n) { return std::string(s.size() > n ? s.substr(s.size() - n) : s); }

// ---- the patterns of the rewrites ----------------------------------------------------------------------------------------------

const Regex& game_class_pattern() {
    static const Regex re(R"(\bclass\s+(\w+)\s*:\s*(?:Microsoft\.Xna\.Framework\.)?Game\b)");
    return re;
}
const Regex& graphics_manager_pattern() {
    static const Regex re(R"(\bnew\s+(?:Microsoft\.Xna\.Framework\.)?GraphicsDeviceManager\s*\(\s*this\s*\))");
    return re;
}
const Regex& audio_using_pattern() {
    static const Regex re(R"(^using Microsoft\.Xna\.Framework\.Audio;\s*\n)", RegexFlags::Multiline);
    return re;
}
const Regex& audio_type_pattern() {
    static const Regex re(R"(\bMicrosoft\.Xna\.Framework\.Audio\.(AudioEngine|WaveBank|SoundBank|Cue|AudioCategory)\b)");
    return re;
}
const Regex& content_class_pattern() {
    static const Regex re(R"((\bclass\s+\w+\s*(?:<[^>]*>)?\s*:\s*)(?:Microsoft\.Xna\.Framework\.Content\.)?ContentManager\b)");
    return re;
}
const Regex& new_content_pattern() {
    static const Regex re(R"(\bnew\s+(?:Microsoft\.Xna\.Framework\.Content\.)?ContentManager\s*\()");
    return re;
}
const Regex& base_cast_pattern() {
    static const Regex re(
        R"(\(\((?:[\w.]+\.)?(?:GameComponent|DrawableGameComponent|Game|IDisposable)\)\s*this\)\.(Update|Draw|Initialize|LoadContent|UnloadContent|Dispose|BeginDraw|EndDraw)\()");
    return re;
}
const Regex& new_effect_pattern() {
    static const Regex re(R"(\bnew\s+(?:Microsoft\.Xna\.Framework\.Graphics\.)?Effect\s*\()");
    return re;
}
const Regex& derives_effect_pattern() {
    static const Regex re(R"(\bclass\s+\w+\s*:\s*(?:Microsoft\.Xna\.Framework\.Graphics\.)?Effect\b)");
    return re;
}
const Regex& base_call_pattern() {
    static const Regex re(R"(\)\s*:\s*base\s*\()");
    return re;
}
const Regex& shader_index_pattern() {
    static const Regex re(R"(\b(\w+)\s*=\s*(?:base\.|this\.)?Parameters\s*\[\s*"ShaderIndex"\s*\])");
    return re;
}
const Regex& get_state_pattern() {
    static const Regex re(R"(\b(?:Microsoft\.Xna\.Framework\.Input\.)?GamePad\.GetState\s*\()");
    return re;
}
const Regex& error_pattern() {
    static const Regex re(R"(^(?<file>[A-Za-z]:[^(]+\.cs)\((?<line>\d+),(?<col>\d+)\): error (?<code>CS\d+): (?<text>.*?)(?: \[.*\])?$)");
    return re;
}

// Replace each match by fn(match), counting them.
std::string replace_counting(const Regex& re, std::string_view subject, std::size_t& count, const std::function<std::string(const RegexMatch&)>& fn) {
    return re.replace_all(subject, [&](const RegexMatch& m) {
        ++count;
        return fn(m);
    });
}

// `new Effect(device, bytes)`, and `: base(device, bytes)` in a class deriving from Effect, with the bytes passed through
// XnaPort.Effects.ForPC (generic: any other argument goes through as it is).
std::string wrap_effect_bytes(std::string text, std::size_t& wrapped) {
    std::vector<std::size_t> spots;
    new_effect_pattern().for_each_match(text, [&](const RegexMatch& m) {
        spots.push_back(m.end() - 1);
        return true;
    });
    if (derives_effect_pattern().test(text))
        base_call_pattern().for_each_match(text, [&](const RegexMatch& m) {
            spots.push_back(m.end() - 1);
            return true;
        });
    std::sort(spots.rbegin(), spots.rend());
    for (const std::size_t paren : spots) {
        const auto args = call_arguments(text, paren);
        if (!args || args->spans.size() != 2) continue;
        const auto [a, b] = args->spans[1];
        const std::string_view whole = std::string_view(text).substr(a, b - a);
        const std::string_view arg = text::trim(whole);
        if (arg.starts_with("XnaPort.Effects.ForPC(")) continue;
        const std::size_t lead = whole.size() - text::ltrim(whole).size();
        text = text.substr(0, a + lead) + "XnaPort.Effects.ForPC(" + std::string(arg) + ")" + text.substr(b);
        ++wrapped;
    }
    return text;
}

// ---- editing the lines the compiler names ---------------------------------------------------------------------------------------

std::vector<std::string> lines_of(const std::string& text) {
    std::vector<std::string> out;
    for (const std::string_view l : text::split(text, '\n')) out.emplace_back(l);
    return out;
}

// Each error line of `code` whose text has `wanted`: edit(line) -> the new line. How many lines changed.
std::size_t edit_lines(const std::vector<BuildError>& errors, std::string_view code, std::string_view wanted,
                       const std::function<std::string(const std::string&)>& edit) {
    std::map<std::string, std::set<int>> by_file;
    for (const BuildError& e : errors)
        if (e.code == code && has(e.text, wanted)) by_file[e.file].insert(e.line);
    std::size_t n = 0;
    for (const auto& [path, at] : by_file) {
        std::vector<std::string> lines = lines_of(read_source(fs::path(path)));
        bool changed = false;
        for (const int ln : at) {
            if (ln < 1 || static_cast<std::size_t>(ln) > lines.size()) continue;
            std::string made = edit(lines[static_cast<std::size_t>(ln) - 1]);
            if (made != lines[static_cast<std::size_t>(ln) - 1]) {
                lines[static_cast<std::size_t>(ln) - 1] = std::move(made);
                ++n;
                changed = true;
            }
        }
        if (changed) cb::fs::write_text(fs::path(path), text::join(lines, "\n"));
    }
    return n;
}

// ---- the stubs --------------------------------------------------------------------------------------------------------------------

std::optional<fs::path> xna_assembly(std::string_view name, const Tools& tools) {
    fs::path root;
    if (tools.gac) {
        root = *tools.gac;
    } else {
        const auto windir = cb::fs::get_env("WINDIR");
        root = cb::fs::from_text(windir ? *windir : "C:\\Windows") / "Microsoft.NET" / "assembly";
    }
    const std::string wanted = text::lower(std::format("microsoft.xna.framework.{}.dll", name));
    std::error_code ec;
    if (!fs::is_directory(root, ec)) return std::nullopt;
    for (fs::recursive_directory_iterator it(root, fs::directory_options::skip_permission_denied, ec), end; it != end; it.increment(ec)) {
        if (ec) break;
        if (it->is_regular_file(ec) && text::lower(it->path().filename().string()) == wanted) return it->path();
    }
    return std::nullopt;
}

// The stub generator, built once under <state>/xna/apistubs (rebuilt when its sources are newer).
fs::path stub_tool(const Tools& tools) {
    const fs::path out = tools.state / "xna" / "apistubs";
    fs::create_directories(out);
    const fs::path exe = out / "apistubs.exe";
    fs::file_time_type newest{};
    for (const char* f : {"Program.cs", "apistubs.csproj"}) newest = std::max(newest, cb::fs::last_write_time(tools.apistubs_dir / f));
    std::error_code ec;
    if (!fs::is_regular_file(exe, ec) || cb::fs::last_write_time(exe) < newest) {
        const process::ProcessResult r = process::run({.argv = {"dotnet", "build", tools.apistubs_dir.string(), "-c", "Release", "-o", out.string(), "--nologo", "-v", "q"},
                                                       .env = dotnet_env(),
                                                       .err = process::Capture::Merge});
        if (r.exit_code != 0) throw ToolError("dotnet build apistubs", r.exit_code, tail_of(as_text(r.out), 800), "did not build");
    }
    return exe;
}

// The XNA Xbox-side API stubs, generated once (<state>/xna/stubs) and copied to `out_dir`.
void generate_stubs(const fs::path& out_dir, const Tools& tools) {
    const fs::path cache = tools.state / "xna" / "stubs";
    fs::create_directories(cache);
    fs::create_directories(out_dir);
    std::optional<fs::path> tool;
    for (const std::string_view name : kStubbed) {
        const fs::path cached = cache / (std::string(name) + ".stubs.cs");
        std::error_code ec;
        if (!fs::is_regular_file(cached, ec)) {
            const auto asm_path = xna_assembly(name, tools);
            if (!asm_path)
                throw NotFoundError(fs::path(std::format("Microsoft.Xna.Framework.{}.dll", name)), 0,
                                    "XNA Game Studio 4.0's assembly is not installed (the stubs are generated from its metadata)");
            if (!tool) tool = stub_tool(tools);
            std::vector<std::string> argv = {tool->string(), asm_path->string(), cached.string(), "Microsoft.Xna.Framework"};
            for (const std::string_view h : kHandWritten) argv.emplace_back(h);
            const process::ProcessResult r = process::run({.argv = argv, .env = dotnet_env()});
            if (r.exit_code != 0) throw ToolError("apistubs", r.exit_code, as_text(r.err).empty() ? as_text(r.out) : as_text(r.err), std::format("failed on {}", name));
        }
        fs::copy_file(cached, out_dir / (std::string(name) + ".stubs.cs"), fs::copy_options::overwrite_existing);
    }
}

// The port's project file: one program, the game's own C# and CodeBreaker's runtime and stubs in it, MonoGame DesktopGL under it.
std::string csproj_text(const std::string& name) {
    return std::format(R"xml(<Project Sdk="Microsoft.NET.Sdk">
  <!-- Written by CodeBreaker's xna port step (cb/xna/port). -->
  <PropertyGroup>
    <OutputType>WinExe</OutputType>
    <TargetFramework>net8.0</TargetFramework>
    <RollForward>Major</RollForward>
    <Nullable>disable</Nullable>
    <AllowUnsafeBlocks>true</AllowUnsafeBlocks>
    <GenerateAssemblyInfo>false</GenerateAssemblyInfo>
    <EnableDefaultCompileItems>false</EnableDefaultCompileItems>
    <AssemblyName>{0}</AssemblyName>
    <RootNamespace>{0}</RootNamespace>
    <DefineConstants>WINDOWS;XNAPORT</DefineConstants>
    <NoWarn>$(NoWarn);CS0108;CS0114;CS0162;CS0168;CS0169;CS0219;CS0414;CS0436;CS0649;CS0067;CS0618;CS1998;CA1416</NoWarn>
  </PropertyGroup>
  <ItemGroup>
    <PackageReference Include="MonoGame.Framework.DesktopGL" Version="{1}" />
  </ItemGroup>
  <ItemGroup>
    <Compile Include="src/**/*.cs" Exclude="src/**/obj/**;src/**/bin/**;src/**/AssemblyInfo.cs" />
    <Compile Include="XnaPort/**/*.cs" />
    <Compile Include="Stubs/**/*.cs" />
    <Compile Include="Port/**/*.cs" Exclude="Port/**/obj/**;Port/**/bin/**" />
  </ItemGroup>
  <ItemGroup>
    <None Include="game/**/*" Exclude="game/**/*.dll;game/**/*.exe;game/**/*.pdb;game/*.xml" CopyToOutputDirectory="PreserveNewest" Link="%(RecursiveDir)%(Filename)%(Extension)" />
    <None Include="ContentConverted/**/*" CopyToOutputDirectory="PreserveNewest" Link="ContentConverted/%(RecursiveDir)%(Filename)%(Extension)" />
    <None Include="Content/**/*" CopyToOutputDirectory="PreserveNewest" Link="Content/%(RecursiveDir)%(Filename)%(Extension)" />
  </ItemGroup>
</Project>
)xml",
                       name, kMonoGameVersion);
}

Json prepare_report_json(const PrepareReport& r) {
    Json rewrites = Json::object();
    for (const auto& [file, changes] : r.rewrites) rewrites[file] = changes;
    Json failed = Json::object();
    for (const auto& [file, why] : r.content.effects.failed) failed[file] = why;
    Json content = {{"wave banks", r.content.wave_banks},
                    {"sound banks", r.content.sound_banks},
                    {"songs", r.content.songs},
                    {"effects", Json{{"compiled", r.content.effects.compiled}, {"failed", failed}}}};
    Json out = {{"rewrites", rewrites}, {"content", content}, {"xnb reader names", r.xnb_reader_names}};
    if (r.overlay) out["overlay"] = *r.overlay;
    return out;
}

void put_json(const fs::path& path, const Json& j) { cb::fs::write_text(path, j.dump(1)); }

}  // namespace

// ---- the source rewrites ------------------------------------------------------------------------------------------------------

std::set<std::string> declared_types(const fs::path& folder) {
    static const Regex declaration(R"(\b(?:class|struct|enum|interface)\s+(\w+))");
    std::set<std::string> names;
    for (const fs::path& f : files_with(folder, ".cs")) {
        const std::string t = text::sanitize_utf8(read_source(f));
        declaration.for_each_match(t, [&](const RegexMatch& m) {
            names.emplace(m.group(1));
            return true;
        });
    }
    return names;
}

std::optional<Arguments> call_arguments(std::string_view text, std::size_t open_paren) {
    Arguments out;
    int depth = 0;
    std::size_t start = open_paren + 1;
    char quote = 0;
    for (std::size_t i = open_paren; i < text.size(); ++i) {
        const char c = text[i];
        if (quote) {
            if (c == quote && text[i - 1] != '\\') quote = 0;
            continue;
        }
        if (c == '"' || c == '\'') {
            quote = c;
        } else if (c == '(' || c == '[' || c == '{') {
            ++depth;
        } else if (c == ')' || c == ']' || c == '}') {
            --depth;
            if (depth == 0) {
                if (!text::trim(text.substr(start, i - start)).empty()) out.spans.emplace_back(start, i);
                out.after = i + 1;
                return out;
            }
        } else if (c == ',' && depth == 1) {
            out.spans.emplace_back(start, i);
            start = i + 1;
        }
    }
    return std::nullopt;
}

Rewritten rewrite(std::string_view source, const std::set<std::string>& own) {
    std::string text(source);
    std::vector<std::string> changed;
    if (const auto m = game_class_pattern().search(text)) {
        const std::string name(m->group(1));
        // the game's constructor first installs the runtime's content manager
        const Regex ctor("\\b(?:public|protected|internal|private)?\\s*" + name + R"(\s*\([^)]*\)\s*(?::\s*base\s*\([^)]*\)\s*)?\{)");
        const auto cm = ctor.search(text, m->end());
        if (cm && !has(text, "XnaPort.Runtime.Install")) {
            text.insert(cm->end(), "\n\t\tXnaPort.Runtime.Install(this);");
            changed.push_back(std::format("Install in {}()", name));
        }
    }
    std::size_t n = 0;
    text = replace_counting(graphics_manager_pattern(), text, n, [](const RegexMatch&) { return std::string("XnaPort.Runtime.Graphics(this)"); });
    if (n) changed.push_back(std::format("GraphicsDeviceManager x{}", n));

    // XACT: the runtime's cues, played from WAV
    const bool xact_present = std::any_of(kXactTypes.begin(), kXactTypes.end(), [&](std::string_view t) { return !own.contains(std::string(t)) && has_word(text, t); });
    if (xact_present && has(text, "Microsoft.Xna.Framework.Audio")) {
        std::string aliases;
        for (const std::string_view t : kXactTypes)
            if (!own.contains(std::string(t))) aliases += std::format("using {0} = XnaPort.Xact.{0};\n", t);
        if (const auto u = audio_using_pattern().search(text)) text.insert(u->end(), aliases);
        std::size_t unused = 0;
        text = replace_counting(audio_type_pattern(), text, unused, [&](const RegexMatch& m) {
            return own.contains(std::string(m.group(1))) ? std::string(m.whole()) : "XnaPort.Xact." + std::string(m.group(1));
        });
        changed.push_back("XACT aliases");
    }
    // the game's own content managers read through the runtime's, which knows the converted content
    n = 0;
    text = replace_counting(content_class_pattern(), text, n, [](const RegexMatch& m) { return std::string(m.group(1)) + "XnaPort.ContentManager"; });
    text = replace_counting(new_content_pattern(), text, n, [](const RegexMatch&) { return std::string("new XnaPort.ContentManager("); });
    if (n) changed.push_back(std::format("content manager x{}", n));
    // ILSpy writes a call of the base class's method as a cast of `this`, which calls the override again
    n = 0;
    text = replace_counting(base_cast_pattern(), text, n, [](const RegexMatch& m) { return "base." + std::string(m.group(1)) + "("; });
    if (n) changed.push_back(std::format("base calls x{}", n));
    // an effect made from the game's own bytes (Xbox 360 microcode) gets the PC's compiled one instead
    n = 0;
    text = wrap_effect_bytes(std::move(text), n);
    if (n) changed.push_back(std::format("effect bytes x{}", n));
    // a stock effect's ShaderIndex picks its technique on the PC (the effect has one per value)
    if (derives_effect_pattern().test(text)) {
        if (const auto m = shader_index_pattern().search(text)) {
            const std::string var(m->group(1));
            const Regex set_value(R"(\b)" + var + R"(\.SetValue\s*\()");
            n = 0;
            text = replace_counting(set_value, text, n, [&](const RegexMatch&) { return "XnaPort.Effects.SetShaderIndex(this, " + var + ", "; });
            if (n) changed.push_back(std::format("ShaderIndex x{}", n));
        }
    }
    // the controller is read through the runtime's, which adds the keyboard and scripted presses
    if (!own.contains("GamePad")) {
        n = 0;
        text = replace_counting(get_state_pattern(), text, n, [](const RegexMatch&) { return std::string("XnaPort.Input.GetState("); });
        if (n) changed.push_back(std::format("GamePad.GetState x{}", n));
    }
    return {std::move(text), std::move(changed)};
}

// ---- the compiler's errors -----------------------------------------------------------------------------------------------------

std::optional<BuildError> parse_error(std::string_view line) {
    const auto m = error_pattern().search(text::trim(line));
    if (!m) return std::nullopt;
    BuildError e;
    e.file = std::string(m->named("file").value());
    e.line = text::parse_int<int>(m->named("line").value()).value_or(0);
    e.column = text::parse_int<int>(m->named("col").value()).value_or(0);
    e.code = std::string(m->named("code").value());
    e.text = std::string(m->named("text").value());
    return e;
}

std::vector<BuildError> parse_errors(std::string_view output) {
    std::vector<BuildError> out;
    std::set<std::string> seen;   // lookup only
    for (const std::string_view raw : text::lines(output)) {
        const std::string_view line = text::trim(raw);
        const auto e = parse_error(line);
        if (e && seen.insert(std::string(line)).second) out.push_back(*e);
    }
    return out;
}

std::size_t fix_access(const std::vector<BuildError>& errors) {
    std::size_t n = 0;
    for (const BuildError& e : errors) {
        if (e.code != "CS0507" || !has(e.text, "'protected internal' inherited member")) continue;
        std::vector<std::string> lines = lines_of(read_source(fs::path(e.file)));
        if (e.line < 1 || static_cast<std::size_t>(e.line) > lines.size()) continue;
        static const Regex protected_override(R"(\bprotected\s+override\b)");
        std::string& line = lines[static_cast<std::size_t>(e.line) - 1];
        const std::string fixed = protected_override.replace_first(line, "protected internal override");
        if (fixed != line) {
            line = fixed;
            ++n;
            cb::fs::write_text(fs::path(e.file), text::join(lines, "\n"));
        }
    }
    return n;
}

std::size_t fix_color(const std::vector<BuildError>& errors) {
    static const Regex new_color(R"(\bnew\s+(?:Microsoft\.Xna\.Framework\.)?Color\s*\()");
    // MonoGame has both Color(int, ...) and Color(byte, ...); XNA had the int one
    return edit_lines(errors, "CS0121", "Color.Color(byte", [](const std::string& line) {
        std::string out = line;
        std::size_t from = 0;
        while (true) {
            const auto m = new_color.search(out, from);
            if (!m) return out;
            const std::size_t at = m->end() - 1;
            const auto args = call_arguments(out, at);
            if (!args || (args->spans.size() != 3 && args->spans.size() != 4) || has(std::string_view(out).substr(at, args->after - at), "(int)(")) {
                from = at + 1;
                continue;
            }
            std::string body;
            for (std::size_t i = 0; i < args->spans.size(); ++i) {
                const auto [a, b] = args->spans[i];
                body += (i ? ", " : "") + std::string("(int)(") + std::string(text::trim(std::string_view(out).substr(a, b - a))) + ")";
            }
            out = out.substr(0, at + 1) + body + ")" + out.substr(args->after);
            from = at + body.size() + 2;
        }
    });
}

std::size_t fix_vertex_binding(const std::vector<BuildError>& errors) {
    static const Regex array_initialiser(R"(new\s+VertexBufferBinding\s*\[[^\]]*\]\s*\{([^}]*)\})");
    return edit_lines(errors, "CS0029", "to 'Microsoft.Xna.Framework.Graphics.VertexBufferBinding'", [](const std::string& line) {
        const auto m = array_initialiser.search(line);
        if (!m) return line;
        std::string items;
        for (const std::string_view item : text::split(m->group(1), ',')) {
            const std::string_view x = text::trim(item);
            if (x.empty()) continue;
            items += (items.empty() ? "" : ", ") + (x.starts_with("new ") ? std::string(x) : "new VertexBufferBinding(" + std::string(x) + ")");
        }
        return line.substr(0, m->start(1)) + " " + items + " " + line.substr(m->end(1));
    });
}

std::size_t fix_named_arguments(const std::vector<BuildError>& errors) {
    // Named arguments whose parameter MonoGame spells otherwise than XNA did.
    struct Renamed {
        std::string_view from, to;
        Regex name_colon;
    };
    static const Renamed renamed[] = {{"mipMap", "mipmap", Regex(R"(\bmipMap\s*:)")},
                                      {"vector1", "value1", Regex(R"(\bvector1\s*:)")},
                                      {"vector2", "value2", Regex(R"(\bvector2\s*:)")}};
    std::size_t n = 0;
    for (const Renamed& r : renamed)
        n += edit_lines(errors, "CS1739", std::format("named '{}'", r.from), [&](const std::string& line) { return r.name_colon.replace_all(line, std::string(r.to) + ":"); });
    return n;
}

std::size_t fix_moved_members(const std::vector<BuildError>& errors) {
    static const Regex stencil_set(R"(([\w.\[\]()]+)\.ReferenceStencil\s*=\s*([^;]+);)");
    static const Regex stencil_get(R"(([\w.\[\]()]+)\.ReferenceStencil\b)");
    static const Regex cube_face(R"(([\w.\[\]()]+)\.CubeMapFace\b)");
    const std::size_t a = edit_lines(errors, "CS1061", "definition for 'ReferenceStencil'", [](const std::string& line) {
        return stencil_get.replace_all(stencil_set.replace_all(line, "XnaPort.Compat.SetReferenceStencil($1, $2);"), "XnaPort.Compat.GetReferenceStencil($1)");
    });
    const std::size_t b = edit_lines(errors, "CS1061", "definition for 'CubeMapFace'", [](const std::string& line) {
        return cube_face.replace_all(line, "((Microsoft.Xna.Framework.Graphics.CubeMapFace)$1.ArraySlice)");
    });
    return a + b;
}

std::size_t fix_ref_out(const std::vector<BuildError>& errors) {
    static const Regex wanted(R"(must be passed with the '(ref|out)' keyword)");
    struct Site {
        int line, column;
        std::string keyword;
    };
    std::map<std::string, std::vector<Site>> by_file;   // each file's sites in the order the compiler gave them
    for (const BuildError& e : errors) {
        if (e.code != "CS1620") continue;
        if (const auto m = wanted.search(e.text)) {
            std::vector<Site>& sites = by_file[e.file];
            const auto same = std::find_if(sites.begin(), sites.end(), [&](const Site& s) { return s.line == e.line && s.column == e.column; });
            if (same != sites.end()) same->keyword = std::string(m->group(1));
            else sites.push_back({e.line, e.column, std::string(m->group(1))});
        }
    }
    std::size_t n = 0;
    for (const auto& [path, sites] : by_file) {
        std::vector<std::string> lines = lines_of(read_source(fs::path(path)));
        for (const Site& s : sites) {
            if (s.line < 1 || static_cast<std::size_t>(s.line) > lines.size()) continue;
            std::string& line = lines[static_cast<std::size_t>(s.line) - 1];
            const std::size_t at = static_cast<std::size_t>(std::max(0, s.column - 1));
            if (at > line.size()) continue;
            bool done = false;
            for (const std::string_view other : {"ref ", "out ", "in "}) {
                if (line.compare(at, other.size(), other) == 0) {
                    line = line.substr(0, at) + s.keyword + " " + line.substr(at + other.size());
                    ++n;
                    done = true;
                    break;
                }
            }
            if (!done && (std::string_view(line).substr(at).starts_with("ref") || std::string_view(line).substr(at).starts_with("out"))) {
                line = line.substr(0, at) + s.keyword + line.substr(at + 3);
                ++n;
            }
        }
        cb::fs::write_text(fs::path(path), text::join(lines, "\n"));
    }
    return n;
}

std::map<std::string, std::vector<Extension>> extension_methods(const fs::path& folder) {
    static const Regex namespace_pattern(R"(^\s*namespace\s+([\w.]+))", RegexFlags::Multiline);
    static const Regex class_pattern(R"(\bstatic\s+(?:partial\s+)?class\s+(\w+))");
    static const Regex extension_pattern(R"(\bstatic\s+[\w<>\[\],. ]+?\s+(\w+)\s*(?:<[^>]*>)?\s*\(\s*this\s+(?:ref\s+)?([\w.<>\[\]]+))");
    std::map<std::string, std::vector<Extension>> out;
    for (const fs::path& f : files_with(folder, ".cs")) {
        const std::string t = text::sanitize_utf8(read_source(f));
        const auto ns = namespace_pattern.search(t);
        std::size_t from = 0;
        while (const auto cm = class_pattern.search(t, from)) {
            from = cm->end();
            const std::string cls = (ns ? std::string(ns->group(1)) + "." : std::string()) + std::string(cm->group(1));
            const std::size_t next_class = t.find("\nclass ", cm->end());
            const std::string_view scope = std::string_view(t).substr(0, next_class != std::string::npos && next_class > 0 ? next_class : t.size());
            std::size_t at = cm->end();
            while (const auto em = extension_pattern.search(scope, at)) {
                out[std::string(em->group(1))].push_back({"global::" + cls, std::string(em->group(2))});
                at = em->end() > em->start() ? em->end() : em->end() + 1;
            }
        }
    }
    return out;
}

std::size_t fix_shadowed_extensions(const std::vector<BuildError>& errors, const std::map<std::string, std::vector<Extension>>& extensions) {
    if (extensions.empty()) return 0;
    std::string names;
    for (const auto& kv : extensions) names += (names.empty() ? "" : "|") + kv.first;
    // receiver.Method(  where Method is one of the game's extension methods
    const Regex call(R"(((?:[A-Za-z_]\w*)(?:\.[A-Za-z_]\w*|\[[^\]]*\])*)\.()" + names + R"()\s*\()");
    const auto edit = [&](const std::string& line) {
        std::string out = line;
        std::size_t from = 0;
        while (true) {
            const auto m = call.search(out, from);
            if (!m) return out;
            const std::string receiver(m->group(1)), name(m->group(2));
            const std::string& cls = extensions.at(name).front().cls;
            const std::string first = receiver.substr(0, receiver.find('.'));
            const std::string class_leaf = cls.substr(cls.rfind('.') == std::string::npos ? 0 : cls.rfind('.') + 1);
            if (first == "this" || first == "base" || receiver.ends_with(class_leaf)) {
                from = m->end();
                continue;
            }
            const std::size_t paren = m->end() - 1;
            const auto args = call_arguments(out, paren);
            if (!args) return out;
            std::string joined = receiver;
            for (const auto& [a, b] : args->spans) joined += ", " + std::string(text::trim(std::string_view(out).substr(a, b - a)));
            const std::string made = cls + "." + name + "(" + joined + ")";
            out = out.substr(0, m->start()) + made + out.substr(args->after);
            from = m->start() + made.size();
        }
    };
    std::size_t n = 0;
    for (const auto& [code, wanted] : {std::pair<std::string_view, std::string_view>{"CS0029", "type 'void'"}, {"CS1503", "from 'void'"}, {"CS0023", "type 'void'"}})
        n += edit_lines(errors, code, wanted, edit);
    return n;
}

// ---- the project ---------------------------------------------------------------------------------------------------------------------

std::string port_reader_name(std::string_view name, std::string_view program) {
    // XNA assemblies whose types MonoGame does not have; the port's stubs give them, in the program itself
    static const Regex stub_assembly(
        R"(, Microsoft\.Xna\.Framework\.(?:Avatar|GamerServices|Net|Storage)(?:, Version=[^\],]*)?(?:, Culture=[^\],]*)?(?:, PublicKeyToken=[^\],]*)?)");
    constexpr std::string_view reflective = "Microsoft.Xna.Framework.Content.ReflectiveReader`1[";
    std::string out = stub_assembly.replace_all(name, [&](const RegexMatch&) { return ", " + std::string(program); });
    if (out.starts_with(reflective)) out = "XnaPort.XnaReflectiveReader`1[" + out.substr(reflective.size()) + ", " + std::string(program);
    return out;
}

std::size_t port_reader_names(const fs::path& game_dir, std::string_view program, const Hooks& hooks) {
    const fs::path stamp = game_dir.parent_path() / "reader-names.stamp";
    std::error_code ec;
    if (fs::is_regular_file(stamp, ec)) return 0;
    std::size_t n = 0;
    for (const fs::path& f : files_with(game_dir, ".xnb")) {
        const Bytes raw = cb::fs::read_file(f);
        if (raw.size() < 3 || raw[0] != 'X' || raw[1] != 'N' || raw[2] != 'B') continue;
        if (!hooks.rename_readers) throw UnsupportedError("xna port", 0, "renaming the type readers of an .xnb needs formats/xnb (Hooks::rename_readers)");
        const auto renamed = hooks.rename_readers(raw, [&](const std::string& name) { return port_reader_name(name, program); });
        if (renamed) {
            cb::fs::write_file(f, *renamed);
            ++n;
        }
    }
    cb::fs::write_text(stamp, std::format("{}\n", n));
    return n;
}

std::pair<std::size_t, std::size_t> convert_xact(const fs::path& game_dir, const fs::path& converted, const Hooks& hooks) {
    std::size_t waves = 0, sounds = 0;
    for (const fs::path& src : files_with(game_dir, ".xwb")) {
        if (!hooks.decode_wave_bank || !hooks.wave_bank_name) throw UnsupportedError("xna port", 0, "XACT wave banks need formats/xbox xwb and xma (Hooks)");
        const fs::path out = converted / src.parent_path().lexically_relative(game_dir) / src.stem();
        if (!fs::is_regular_file(out / "bank.txt")) {
            fs::create_directories(out);
            hooks.decode_wave_bank(src, out);
            cb::fs::write_text(out / "bank.txt", hooks.wave_bank_name(src));
        }
        ++waves;
    }
    for (const fs::path& src : files_with(game_dir, ".xsb")) {
        if (!hooks.sound_bank_cues) throw UnsupportedError("xna port", 0, "XACT sound banks need formats/xbox xsb (Hooks)");
        const fs::path folder = converted / src.parent_path().lexically_relative(game_dir);
        fs::create_directories(folder);
        cb::fs::write_text(folder / (src.stem().string() + ".cues.json"), hooks.sound_bank_cues(src));
        ++sounds;
    }
    return {waves, sounds};
}

EffectsReport compile_effects(const fs::path& converted, const fs::path& shaders, const Tools& tools) {
    const fs::path cache = tools.state / "xna" / "mgfxo";
    fs::create_directories(cache);
    fs::create_directories(shaders);
    const std::optional<fs::path> tool = tools.mgfxc ? tools.mgfxc : which("mgfxc");
    EffectsReport report;
    for (const fs::path& src : files_with(converted, ".fx")) {
        const std::string t = cb::fs::read_text(src);
        const std::string_view first = text::split_n(t, '\n', 1)[0];
        if (!first.starts_with("// sha1 ")) continue;
        const std::string digest(text::trim(first.substr(8)));
        const std::string rel = relative_text(src, converted);
        const fs::path built = cache / (hex_text(crypto::sha1(as_bytes(t))) + ".mgfxo");
        std::error_code ec;
        if (!fs::is_regular_file(built, ec)) {
            if (!tool) {
                report.failed[rel] = "mgfxc is not installed (NOT_IN_REPO.md)";
                continue;
            }
            const process::ProcessResult r = process::run({.argv = {tool->string(), src.string(), built.string(), "/Profile:OpenGL"}, .env = dotnet_env()});
            if (r.exit_code != 0 || !fs::is_regular_file(built, ec)) {
                report.failed[rel] = tail_of(text::trim(std::string(as_text(r.out)) + std::string(as_text(r.err))), 600);
                continue;
            }
        }
        Bytes data = cb::fs::read_file(built);
        const fs::path params = fs::path(src.string() + ".json");
        if (fs::is_regular_file(params, ec)) data = mgfx::add_parameters(data, mgfx::parse_parameters(cb::fs::read_text(params)));
        cb::fs::write_file(shaders / (digest + ".mgfxo"), data);
        ++report.compiled;
    }
    return report;
}

std::size_t convert_songs(const fs::path& game_dir, const fs::path& converted, const Tools& tools) {
    std::size_t n = 0;
    for (const fs::path& src : files_with(game_dir, ".wma")) {
        const fs::path out = converted / src.parent_path().lexically_relative(game_dir) / (src.stem().string() + ".ogg");
        std::error_code ec;
        if (!fs::is_regular_file(out, ec)) {
            const std::optional<fs::path> ffmpeg = tools.ffmpeg ? tools.ffmpeg : which("ffmpeg");
            if (!ffmpeg) throw NotFoundError(fs::path("ffmpeg"), 0, "ffmpeg is needed to convert the songs and was not found");
            fs::create_directories(out.parent_path());
            const process::ProcessResult r = process::run(
                {.argv = {ffmpeg->string(), "-y", "-hide_banner", "-loglevel", "error", "-i", src.string(), "-c:a", "libvorbis", "-q:a", "5", out.string()}});
            if (r.exit_code != 0)
                throw ToolError("ffmpeg", r.exit_code, tail_of(text::trim(as_text(r.err)), 300), std::format("could not convert {}", src.filename().string()));
        }
        ++n;
    }
    return n;
}

Prepared prepare(const base::converted::Project& project, const Tools& tools, const Hooks& hooks, const std::optional<fs::path>& overlay) {
    const fs::path game_dir = project.assets() / "game";
    std::error_code probe;
    const std::vector<fs::path> games = fs::is_directory(game_dir, probe) ? run::assemblies(game_dir) : std::vector<fs::path>{};
    if (games.empty()) throw UsageError(cb::fs::to_text(project.root()) + ": no game assemblies (run `codebreaker xna` on the game first)");
    const auto exe_it = std::find_if(games.begin(), games.end(), [](const fs::path& g) { return lower_extension(g) == ".exe"; });
    const fs::path exe = exe_it != games.end() ? *exe_it : games.front();
    const fs::path exe_dir = exe.parent_path();
    const fs::path exe_rel = exe_dir.lexically_relative(game_dir);
    std::string name = exe.stem().string();
    name.erase(std::remove(name.begin(), name.end(), ' '), name.end());
    Prepared out;
    out.name = name;
    out.dir = project.build() / "port";
    const fs::path& port = out.dir;
    fs::create_directories(port);
    PrepareReport& report = out.report;

    // the game's C#, rewritten
    const std::set<std::string> own = declared_types(project.src());
    std::vector<fs::path> assemblies;
    for (const fs::directory_entry& e : fs::directory_iterator(project.src()))
        if (e.is_directory()) assemblies.push_back(e.path());
    std::sort(assemblies.begin(), assemblies.end());
    for (const fs::path& src : assemblies) {
        const fs::path dst = port / "src" / src.filename();
        std::error_code ec;
        fs::remove_all(dst, ec);
        copy_over(src, dst);
        for (const fs::path& f : files_with(dst, ".csproj")) fs::remove(f);
        for (const fs::path& p : files_with(dst, ".cs")) {
            const std::string t = read_source(p);
            Rewritten r = rewrite(t, own);
            if (!r.changes.empty()) {
                report.rewrites.insert_or_assign(relative_text(p, port), std::move(r.changes));
                cb::fs::write_text(p, r.text);
            }
        }
    }
    // the runtime and the stubs
    const fs::path runtime = port / "XnaPort";
    fs::create_directories(runtime);
    for (const fs::path& f : files_with(tools.runtime_dir, ".cs"))
        if (f.parent_path() == tools.runtime_dir) fs::copy_file(f, runtime / f.filename(), fs::copy_options::overwrite_existing);
    if (hooks.stubs) hooks.stubs(port / "Stubs");
    else generate_stubs(port / "Stubs", tools);
    // the game's own files beside the program, and the converted content
    const fs::path game_link = port / "game";
    std::error_code ec;
    if (!fs::is_directory(game_link, ec)) copy_over(exe_dir, game_link);
    report.xnb_reader_names = port_reader_names(game_link, name, hooks);
    const fs::path converted = port / "ContentConverted";
    const fs::path content_src = project.assets() / "content" / exe_rel;
    if (fs::is_directory(content_src, ec)) copy_over(content_src, converted);
    fs::create_directories(converted);
    std::tie(report.content.wave_banks, report.content.sound_banks) = convert_xact(exe_dir, converted, hooks);
    report.content.songs = convert_songs(exe_dir, converted, tools);
    report.content.effects = compile_effects(converted, port / "Content" / "shaders", tools);

    // the game's own hand work
    std::optional<fs::path> ov = overlay;
    if (!ov) {
        const fs::path d = (tools.overlays ? *tools.overlays : tools.state / "xna" / "overlays") / project.name();
        if (fs::is_directory(d, ec)) ov = d;
    }
    if (ov) {
        Json manifest = Json::object();
        if (fs::is_regular_file(*ov / "overlay.json", ec)) manifest = json::load(*ov / "overlay.json");
        const auto strings = [&](const char* key) {
            std::vector<std::string> v;
            if (manifest.contains(key) && manifest[key].is_array())
                for (const auto& x : manifest[key]) v.push_back(x.get<std::string>());
            return v;
        };
        for (const std::string& rel : strings("replace")) fs::remove_all(port / fs::path(rel), ec);
        if (manifest.contains("sources") && manifest["sources"].is_object())
            for (const auto& [rel, folder] : manifest["sources"].items()) copy_over(fs::path(folder.get<std::string>()), port / fs::path(rel));
        copy_over(*ov, port);
        for (const std::string& rel : strings("drop")) {
            const fs::path p = port / fs::path(rel);
            if (fs::is_directory(p, ec)) fs::remove_all(p, ec);
            else if (fs::is_regular_file(p, ec)) fs::remove(p, ec);
        }
        if (manifest.contains("assets") && manifest["assets"].is_string() && !manifest["assets"].get<std::string>().empty() && !fs::is_directory(port / "assets", ec))
            copy_over(fs::path(manifest["assets"].get<std::string>()), port / "assets");
        report.overlay = cb::fs::to_text(*ov);
    }
    cb::fs::write_text(port / (name + ".Port.csproj"), csproj_text(name));
    return out;
}

BuildResult build(const base::converted::Project& project, const Tools& tools, const Hooks& hooks, const std::optional<fs::path>& overlay, std::size_t rounds) {
    Prepared prepared = prepare(project, tools, hooks, overlay);
    const fs::path& port = prepared.dir;
    BuildResult result;
    result.report = std::move(prepared.report);
    const auto extensions = extension_methods(port / "src");
    std::vector<BuildError> errors;
    std::string output;
    bool ok = false;
    for (std::size_t round = 0; round < rounds; ++round) {
        const process::ProcessResult r = process::run({.argv = {"dotnet", "build", (port / (prepared.name + ".Port.csproj")).string(), "-c", "Release", "--nologo", "-v", "q",
                                                                "-clp:NoSummary"},
                                                       .cwd = port,
                                                       .env = dotnet_env()});
        output = std::string(as_text(r.out)) + "\n" + std::string(as_text(r.err));
        errors = parse_errors(output);
        ok = r.exit_code == 0 && errors.empty();
        std::size_t fixed = 0;
        if (!ok)
            fixed = fix_ref_out(errors) + fix_access(errors) + fix_color(errors) + fix_vertex_binding(errors) + fix_named_arguments(errors) +
                    fix_moved_members(errors) + fix_shadowed_extensions(errors, extensions);
        result.rounds.push_back({errors.size(), fixed});
        if (ok || !fixed) break;
    }
    for (const BuildError& e : errors) ++result.errors[e.code];
    const fs::path exe = port / "bin" / "Release" / "net8.0" / (prepared.name + ".exe");
    result.ok = ok;
    if (ok) result.exe = exe;
    for (std::size_t i = 0; i < errors.size() && i < 60; ++i)
        result.first.push_back(std::format("{}({}): {} {}", relative_text(fs::path(errors[i].file), port), errors[i].line, errors[i].code, errors[i].text));
    if (!ok && errors.empty()) result.build_output_tail = tail_of(output, 2000);

    Json doc = prepare_report_json(result.report);
    Json by_code = Json::object();
    for (const auto& [code, n] : result.errors) by_code[code] = n;
    Json rounds_json = Json::array();
    for (const BuildRound& r : result.rounds) rounds_json.push_back(Json{{"errors", r.errors}, {"fixed", r.fixed}});
    Json out = {{"ok", result.ok}, {"exe", result.exe ? Json(cb::fs::to_text(*result.exe)) : Json(nullptr)}, {"errors", by_code}, {"first", result.first},
                {"build_output_tail", result.build_output_tail}};
    for (const auto& [k, v] : doc.items()) out[k] = v;
    out["rounds"] = rounds_json;
    fs::create_directories(project.reports());
    put_json(project.reports() / "port.json", out);
    return result;
}

RunResult run_for(const fs::path& exe, std::chrono::seconds seconds, const std::map<std::string, std::string>& env) {
    const fs::path folder = exe.parent_path();
    const fs::path log = folder / "xnaport.log";
    std::error_code ec;
    fs::remove(log, ec);
    std::map<std::string, std::string> all = dotnet_env();
    for (const auto& [k, v] : env) all[k] = v;
    const process::ProcessResult r = process::run({.argv = {exe.string()}, .cwd = folder, .env = all, .err = process::Capture::Merge,
                                                   .timeout = std::chrono::duration_cast<std::chrono::milliseconds>(seconds)});
    RunResult out;
    if (!r.timed_out) out.exit = static_cast<int>(r.exit_code);
    out.output = tail_of(as_text(r.out), 6000);
    if (fs::is_regular_file(log, ec)) out.log = tail_of(text::sanitize_utf8(cb::fs::read_text(log)), 6000);
    return out;
}

std::optional<std::map<std::string, std::string>> check_content(const fs::path& exe, std::chrono::seconds seconds) {
    const fs::path out = exe.parent_path() / "content_check.json";
    std::error_code ec;
    fs::remove(out, ec);
    run_for(exe, seconds, {{"XNAPORT_CHECK_CONTENT", "1"}});
    if (!fs::is_regular_file(out, ec)) return std::nullopt;
    const Json j = json::load(out);
    std::map<std::string, std::string> checked;
    if (j.is_object())
        for (const auto& [k, v] : j.items()) checked[k] = v.is_string() ? v.get<std::string>() : v.dump();
    return checked;
}

int main(std::span<const std::string> args, const base::converted::Store& store, const Tools& tools, const Hooks& hooks) {
    if (args.empty() || args[0] == "-h" || args[0] == "--help") {
        out("{}\n", "xna port <Converted project or name> [--overlay DIR] [--run SECONDS] [--check-content]");
        return args.empty() ? 2 : 0;
    }
    const std::string& target = args[0];
    std::error_code ec;
    const fs::path path = fs::is_directory(fs::path(target), ec) ? fs::path(target) : store.root() / target;
    if (!fs::is_regular_file(path / "project.json", ec)) {
        out("xna port: {} is not a Converted project\n", target);
        return 2;
    }
    const auto after = [&](std::string_view flag) -> std::optional<std::string> {
        for (std::size_t i = 0; i + 1 < args.size(); ++i)
            if (args[i] == flag) return args[i + 1];
        return std::nullopt;
    };
    const auto has_flag = [&](std::string_view flag) { return std::find(args.begin(), args.end(), flag) != args.end(); };
    std::optional<fs::path> overlay;
    if (const auto o = after("--overlay")) overlay = fs::path(*o);
    std::optional<std::size_t> run_seconds;
    if (has_flag("--run")) {
        const auto s = after("--run");
        run_seconds = s ? text::parse_int<std::size_t>(*s) : std::nullopt;
        if (!run_seconds) {
            out("xna port: --run needs a number of seconds\n");
            return 2;
        }
    }
    const base::converted::Project project(path);
    const auto t0 = std::chrono::steady_clock::now();
    BuildResult result;
    try {
        result = build(project, tools, hooks, overlay);
    } catch (const Error& e) {
        out("xna port: {}\n", e.what());
        return 1;
    }
    std::size_t left = 0;
    for (const auto& kv : result.errors) left += kv.second;
    if (result.ok) {
        out("built {} ({:.0f} s)\n", cb::fs::to_text(*result.exe), std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count());
    } else {
        std::vector<std::pair<std::string, std::size_t>> by_count(result.errors.begin(), result.errors.end());
        std::stable_sort(by_count.begin(), by_count.end(), [](const auto& a, const auto& b) { return a.second > b.second; });
        std::string counts;
        for (const auto& [code, n] : by_count) counts += (counts.empty() ? "" : ", ") + std::format("{} {}", code, n);
        out("not built: {} errors ({}); reports/port.json lists them\n", left, counts);
        for (std::size_t i = 0; i < result.first.size() && i < 15; ++i) out("  {}\n", result.first[i]);
    }
    if (result.ok && run_seconds) {
        const RunResult r = run_for(*result.exe, std::chrono::seconds(*run_seconds));
        out("ran {} s: {}\n", *run_seconds, r.exit ? std::format("exited {}", *r.exit) : std::string("still running"));
        out("{}\n", tail_of(r.log.empty() ? r.output : r.log, 2500));
        put_json(project.reports() / "port_run.json",
                 Json{{"exit", r.exit ? Json(*r.exit) : Json(nullptr)}, {"output", text::sanitize_utf8(r.output)}, {"log", text::sanitize_utf8(r.log)}});
    }
    if (result.ok && has_flag("--check-content")) {
        const auto checked = check_content(*result.exe);
        if (!checked) {
            out("content check: the program wrote no result\n");
        } else {
            std::map<std::string, std::string> failed;
            std::size_t other = 0;
            for (const auto& [asset, v] : *checked) {
                if (v.starts_with("error:")) failed[asset] = v;
                else if (v.starts_with("not an XNB") || v.starts_with("empty")) ++other;
            }
            out("content check: {} of {} .xnb loaded, {} failed; {} are not XNB content (empty, or a file the game reads itself)\n",
                checked->size() - failed.size() - other, checked->size() - other, failed.size(), other);
            std::size_t shown = 0;
            for (const auto& [asset, why] : failed) {
                if (shown++ >= 20) break;
                out("  {}: {}\n", asset, why.substr(7, 160));
            }
            Json doc = Json::object();
            for (const auto& [k, v] : *checked) doc[k] = v;
            put_json(project.reports() / "content_check.json", doc);
        }
    }
    project.record("xna port", result.ok ? std::vector<fs::path>{*result.exe} : std::vector<fs::path>{},
                   std::format("MonoGame build of the decompiled C# in build/port: {}", result.ok ? "built" : std::format("{} errors left (reports/port.json)", left)));
    return result.ok ? 0 : 1;
}

}  // namespace cb::xna::port
