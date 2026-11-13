// cb/source/vxd/boot.cpp -- does Windows 95 start with these drivers? (see boot.hpp)
// cb-unity: alone
#include "cb/source/vxd/boot.hpp"

#include <algorithm>
#include <cmath>
#include <format>
#include <thread>

#include "cb/base/runs.hpp"
#include "cb/core/cli.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/process.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/le.hpp"
#include "cb/formats/media.hpp"
#include "cb/source/vxd/le_writer.hpp"
#include "cb/verify/gate16/setup16.hpp"

namespace cb::source::vxd::boot {

namespace stdfs = std::filesystem;
namespace le = formats::le;
using verify::gate16::Picture;
using verify::gate16::Rgb;

namespace {

constexpr std::array<Rgb, 2> kGreys = {Rgb{192, 192, 192}, Rgb{192, 199, 200}};   // the taskbar: the palette's grey, and on QEMU's VGA

bool is_grey(const Rgb& p) { return std::find(kGreys.begin(), kGreys.end(), p) != kGreys.end(); }

// Windows 95's desktop colour: (0, 128, 128) in the palette, (87, 168, 168) on QEMU's VGA.
bool is_teal(const Rgb& p) {
    const int r = p.r, g = p.g, b = p.b;
    return std::abs(g - b) < 12 && 110 < g && g < 190 && r < g - 50;
}

// The picture's rows [top, top + rows) shrunk to width x height by nearest neighbour, as Pillow's resize(size, NEAREST) does.
std::vector<Rgb> nearest(const Picture& picture, std::uint32_t top, std::uint32_t rows, std::uint32_t width, std::uint32_t height) {
    std::vector<Rgb> pixels;
    pixels.reserve(static_cast<std::size_t>(width) * height);
    const double xscale = static_cast<double>(picture.width()) / width, yscale = static_cast<double>(rows) / height;
    for (std::uint32_t y = 0; y < height; ++y) {
        const auto sy = std::min<std::uint32_t>(rows - 1, static_cast<std::uint32_t>((y + 0.5) * yscale));
        for (std::uint32_t x = 0; x < width; ++x) {
            const auto sx = std::min<std::uint32_t>(picture.width() - 1, static_cast<std::uint32_t>((x + 0.5) * xscale));
            pixels.push_back(*picture.at(sx, top + sy));
        }
    }
    return pixels;
}

template <class Test>
double share_of(const std::vector<Rgb>& pixels, Test test) {
    return static_cast<double>(std::count_if(pixels.begin(), pixels.end(), test)) / static_cast<double>(pixels.size());
}

std::string boot_upper_path(std::string_view p) { return text::upper(p); }

// QEMU's option parser ends a value at a comma: a comma in a path is doubled.
std::string boot_option_path(const stdfs::path& p) { return text::replace_all(fs::to_text(p), ",", ",,"); }

}  // namespace

std::string_view screen_word(Screen screen) {
    switch (screen) {
    case Screen::Desktop: return "desktop";
    case Screen::Blue: return "blue";
    case Screen::Text: return "text";
    case Screen::Black: return "black";
    case Screen::Other: return "other";
    }
    return "other";
}

Screen classify(const Picture& picture) {
    const std::uint32_t w = picture.width(), h = picture.height();
    if (w == 0 || h == 0) return Screen::Other;
    const std::vector<Rgb> shrunk = nearest(picture, 0, h, 160, 120);
    if (share_of(shrunk, [](const Rgb& p) { return p.b > 140 && p.r < 40 && p.g < 40; }) > 0.6) return Screen::Blue;
    if (w == 720 && h == 400) return Screen::Text;
    if (share_of(shrunk, [](const Rgb& p) { return p.sum() < 40; }) > 0.97) return Screen::Black;
    // (the desktop: mostly teal, with whatever box is on it -- the logon, "Welcome", a clock message -- and the taskbar once Explorer has it,
    // in a 640x480 or larger mode)
    if (w >= 640 && h >= 480 && share_of(shrunk, is_teal) > 0.4) return Screen::Desktop;
    if (w >= 640 && h >= 480) {   // (a wallpaper: the taskbar, then)
        const std::vector<Rgb> bar = nearest(picture, h - 24, 24, 80, 6);
        const auto greys = static_cast<double>(std::count_if(bar.begin(), bar.end(), is_grey));
        if (greys > static_cast<double>(bar.size()) * 0.7 && is_grey(*picture.at(0, h - 14))) return Screen::Desktop;
    }
    return Screen::Other;
}

stdfs::path corpus_file(const Environment& env, std::string_view name) {
    std::vector<std::string> names;
    if (name.find('.') != std::string_view::npos) names = {std::string(name)};
    else
        for (const char* extension : {".vxd", ".386", ".pdr"}) names.push_back(std::string(name) + extension);
    for (const std::string& n : names)
        if (const auto path = env.corpus.retail(n)) return *path;
    throw UsageError(std::format("no driver '{}' in the corpus", name));
}

std::string guest_path(const Environment& env, const stdfs::path& path) {
    const std::string relative = text::replace_all(fs::to_text(stdfs::relative(path, env.corpus.root())), "\\", "/");
    const std::size_t slash = relative.rfind('/');
    std::string folder = slash == std::string::npos ? "" : relative.substr(0, slash);
    const std::string name = slash == std::string::npos ? relative : relative.substr(slash + 1);
    if (text::upper(folder) == "WINDOWS/SYSTEM" && text::iends_with(name, ".VXD") && text::upper(name) != "VMM32.VXD") folder = "WINDOWS/SYSTEM/VMM32";
    return boot_upper_path(folder + "/" + name);
}

Bytes build(const Environment& env, const stdfs::path& path) {
    const Bytes data = fs::read_file(path);
    const auto modules = le::open_modules(path);
    if (modules.empty()) throw FormatError("LE", 0, "the file holds no driver");
    const le::Module& driver = modules.front().second;
    const masm::Emission code(Disassembly::run(driver, env.names), env.names);
    if (const auto container = le::container(data); container && container->kind == le::Container::Kind::W3) {
        if (container->members.size() != 1)
            throw UnsupportedError("LE", 0, std::format("{}: a bundle of {} drivers; the bundle writer rebuilds one", fs::to_text(path), container->members.size()));
        return le_writer::build_w3(code, driver);
    }
    return le_writer::build_le(code, driver, code.ddb() && !code.ddb()->name.empty() ? code.ddb()->name : "DEVICE");
}

std::vector<std::string> bundle_members(ByteView data) {
    auto container = le::container(data);
    if (container && container->kind == le::Container::Kind::W4) container = le::container(le::expand(data));
    std::vector<std::string> names;
    if (container)
        for (const auto& member : container->members) names.push_back(member.name);
    return names;
}

std::string boot_options(std::string_view source) {
    // BootLog: C:\BOOTLOG.TXT says which drivers loaded and where it stopped; AutoScan: no ScanDisk first (every run stops the machine unclean)
    const std::vector<std::pair<std::string, std::string>> options = {{"BootLog", "1"}, {"AutoScan", "0"}};
    std::vector<std::string> lines;
    for (const std::string_view line : text::split(source, "\r\n")) {
        const std::string key = text::lower(text::trim(line.substr(0, line.find('='))));
        const bool ours = std::any_of(options.begin(), options.end(), [&](const auto& o) { return text::lower(o.first) == key; });
        if (!ours) lines.emplace_back(line);
    }
    std::size_t at = 0;
    bool found = false;
    for (std::size_t i = 0; i < lines.size(); ++i)
        if (text::lower(text::trim(lines[i])) == "[options]") {
            at = i + 1;
            found = true;
            break;
        }
    if (!found) {
        lines.insert(lines.begin(), "[Options]");
        at = 1;
    }
    std::vector<std::string> settings;
    for (const auto& [key, value] : options) settings.push_back(key + "=" + value);
    lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(at), settings.begin(), settings.end());
    return text::join(lines, "\r\n");
}

std::map<std::string, Bytes> plan(const Environment& env, const stdfs::path& disk, std::span<const std::string> drivers, bool vmm32, bool retail,
                                  const Log& log) {
    auto volumes = formats::media::volumes(disk);
    if (volumes.empty()) throw FormatError("media", 0, "no volume on the disk");
    formats::media::Volume& volume = *volumes.front();
    std::map<std::string, const formats::media::Entry*> files;
    for (const formats::media::Entry& e : volume.files()) files[boot_upper_path(e.path)] = &e;
    std::map<std::string, Bytes> result;
    std::vector<std::string> names(drivers.begin(), drivers.end());
    if (vmm32) names.push_back("vmm32.vxd");
    for (const std::string& name : names) {
        const stdfs::path path = corpus_file(env, name);
        const std::string at = guest_path(env, path);
        result[at] = retail ? fs::read_file(path) : build(env, path);
        log(std::format("  {:<34} {}", at, retail ? "original" : "rebuilt"));
    }
    if (vmm32) {
        const auto own = files.find("WINDOWS/SYSTEM/VMM32.VXD");
        std::vector<std::string> fallback;
        if (own != files.end()) {
            for (const std::string& member : bundle_members(volume.read(*own->second))) {
                const std::string at = "WINDOWS/SYSTEM/VMM32/" + text::upper(member) + ".VXD";
                if (text::upper(member) == "VMM" || result.contains(at) || files.contains(at)) continue;
                result[at] = fs::read_file(corpus_file(env, text::lower(member) + ".vxd"));
                fallback.push_back(member);
            }
        }
        if (!fallback.empty())
            log(std::format("  + {} drivers of the installed bundle as their own files (originals): {}", fallback.size(), text::join(fallback, " ")));
    }
    if (const auto sys = files.find("MSDOS.SYS"); sys != files.end()) {
        const Bytes original = volume.read(*sys->second);
        const std::string rewritten = boot_options(text::from_latin1(original));
        result["MSDOS.SYS"] = Bytes(rewritten.begin(), rewritten.end());   // (latin-1 both ways: the bytes are kept)
    }
    return result;
}

Outcome run_machine(const Environment& env, const stdfs::path& disk, const std::map<std::string, Bytes>& files, std::string_view tag,
                    std::chrono::seconds wait, const Log& log) {
    namespace gate = verify::gate16;
    const stdfs::path shots = base::runs::results(env.state, "vxdboot");
    fs::make_dirs(shots);
    Outcome outcome;
    outcome.screenshot = shots / (std::string(tag) + ".png");
    outcome.verdict = "FAIL(timeout)";
    const double need = static_cast<double>(fs::file_size(disk)) + env.min_free_gb * 1073741824.0;
    if (static_cast<double>(stdfs::space(shots).available) < need)
        throw IoError(disk, 0, "not enough free disk space for a copy");
    const stdfs::path work = base::runs::start(env.state, "vxdboot", tag);   // (the disk copy: this run's alone)
    try {
        const stdfs::path hd = work / "hd.img";
        stdfs::copy_file(disk, hd, stdfs::copy_options::overwrite_existing);
        std::vector<formats::media::NewFile> put;
        for (const auto& [path, data] : files) put.push_back({path, data});
        formats::media::put(hd, put);
        const gate::setup16::Monitor monitor(gate::setup16::free_port());
        process::ProcessSpec spec;
        spec.argv = {fs::to_text(env.qemu), "-M", "pc", "-m", "64", "-drive", std::format("file={},format=raw,if=ide,index=0", boot_option_path(hd)), "-boot", "c",
                     "-display", "none", "-monitor", std::format("tcp:127.0.0.1:{},server,nowait", monitor.port())};
        spec.out = process::Capture::Discard;
        spec.err = process::Capture::Discard;
        spec.priority = process::Priority::BelowNormal;
        process::Process machine = process::start(spec);
        log(std::format("  QEMU started (below-normal priority), up to {}s", wait.count()));
        std::vector<Screen> seen;
        int same = 0;
        std::optional<Screen> last;
        const auto until = std::chrono::steady_clock::now() + wait;
        while (std::chrono::steady_clock::now() < until) {
            std::this_thread::sleep_for(std::chrono::seconds(4));
            if (!machine.running()) {
                outcome.verdict = "FAIL(qemu exited)";
                break;
            }
            const std::optional<Picture> picture = monitor.shot(outcome.screenshot);
            if (!picture) continue;
            const Screen kind = classify(*picture);
            seen.push_back(kind);
            if (kind != last)
                log(std::format("    {:>3}s  {} screen {}x{}", static_cast<int>(wait.count() - std::chrono::duration<double>(until - std::chrono::steady_clock::now()).count()),
                                screen_word(kind), picture->width(), picture->height()));
            same = kind == last ? same + 1 : 0;
            last = kind;
            if (kind == Screen::Desktop && same >= 2) {   // (three screens running: settled)
                outcome.verdict = "PASS";
                break;
            }
            if (kind == Screen::Blue && same >= 8) {   // (ScanDisk is blue too, for a while)
                outcome.verdict = "FAIL(blue screen)";
                break;
            }
            // a text screen or a black one after Windows' own graphic screens is a stop (the protection error; a fault with the screen left dark), but only
            // once it has stayed
            if ((kind == Screen::Text || kind == Screen::Black) && std::find(seen.begin(), seen.end(), Screen::Other) != seen.end() && same >= 15) {
                outcome.verdict = std::format("FAIL({} screen)", screen_word(kind));
                break;
            }
        }
        machine.kill();
        machine.wait(std::chrono::seconds(20));
        try {
            auto volumes = formats::media::volumes(hd);
            for (const formats::media::Entry& e : volumes.front()->files())
                if (text::upper(e.path) == "BOOTLOG.TXT") outcome.boot_log = text::from_latin1(volumes.front()->read(e));
        } catch (const Error&) {   // no log: the machine never got that far
        }
    } catch (...) {
        base::runs::remove(work);
        throw;
    }
    base::runs::remove(work);   // (QEMU lets go of its files a moment later)
    fs::write_file(shots / (std::string(tag) + ".bootlog.txt"), as_bytes(outcome.boot_log.empty() ? std::string_view() : std::string_view(outcome.boot_log)));
    return outcome;
}

std::string loaded(std::string_view boot_log, std::string_view path) {
    const std::string name = text::upper(path.substr(path.rfind('/') == std::string_view::npos ? 0 : path.rfind('/') + 1));
    const std::string stem = name.substr(0, name.find('.'));
    std::vector<std::string> steps;
    for (const std::string_view line : text::lines(boot_log)) {
        const std::size_t bracket = line.find(']');
        const std::string_view rest = bracket == std::string_view::npos ? line : line.substr(bracket + 1);
        const std::size_t equals = rest.find('=');
        const std::string what = std::string(text::trim(rest.substr(0, equals)));
        const std::string value = equals == std::string_view::npos ? "" : text::upper(text::trim(rest.substr(equals + 1)));
        if (text::iends_with(value, "\\" + name) || value == name || value == stem) {
            if (what != "Loading Vxd" && std::find(steps.begin(), steps.end(), what) == steps.end()) steps.push_back(what);
        }
    }
    std::vector<std::string> shown;
    for (const std::string& s : steps)   // (a phase that ended: once)
        if (std::find(steps.begin(), steps.end(), s + "SUCCESS") == steps.end()) shown.push_back(s);
    return text::join(shown, " ");
}

std::string run(const Environment& env, const stdfs::path& disk, std::span<const std::string> drivers, bool vmm32, bool retail, std::string_view tag,
                std::chrono::seconds wait, const Log& log) {
    std::vector<std::string> parts(drivers.begin(), drivers.end());
    if (vmm32) parts.push_back("VMM32");
    const std::string label = parts.empty() ? "(nothing put in)" : text::join(parts, ", ");
    log(std::format("{}: {} {}", tag, retail ? "originals of" : "rebuilt", label));
    const std::map<std::string, Bytes> files = plan(env, disk, drivers, vmm32, retail, log);
    const Outcome outcome = run_machine(env, disk, files, tag, wait, log);
    for (const std::string& d : drivers) {
        const std::string p = guest_path(env, corpus_file(env, d));
        const std::string what = loaded(outcome.boot_log, p);
        log(std::format("    {:<34} {}", p, what.empty() ? "(not in BOOTLOG.TXT: not loaded)" : what));
    }
    log(std::format("  {}   screen: {}", outcome.verdict, fs::to_text(outcome.screenshot)));
    if (outcome.verdict != "PASS" && !outcome.boot_log.empty()) {
        std::vector<std::string> end;
        for (const std::string_view line : text::lines(text::trim(outcome.boot_log))) end.emplace_back(line);
        const std::size_t from = end.size() > 6 ? end.size() - 6 : 0;
        log("  BOOTLOG.TXT ends:\n    " + text::join(std::vector<std::string>(end.begin() + static_cast<std::ptrdiff_t>(from), end.end()), "\n    "));
    }
    return outcome.verdict;
}

std::vector<std::string> bisect(std::span<const std::string> units,
                                const std::function<std::string(std::span<const std::string>, const std::string&)>& trial,
                                const std::string& tag, const Log& log) {
    const std::function<std::vector<std::string>(std::vector<std::string>, const std::string&)> go =
        [&](std::vector<std::string> group, const std::string& label) -> std::vector<std::string> {
        if (group.size() == 1) return group;
        std::vector<std::string> found;
        const std::size_t half = group.size() / 2;
        const std::vector<std::string> halves[2] = {{group.begin(), group.begin() + static_cast<std::ptrdiff_t>(half)},
                                                    {group.begin() + static_cast<std::ptrdiff_t>(half), group.end()}};
        for (int i = 0; i < 2; ++i) {
            if (trial(halves[i], std::format("{}_b{}{}", tag, label, i)) != "PASS") {
                const std::vector<std::string> part = go(halves[i], label + std::to_string(i));
                found.insert(found.end(), part.begin(), part.end());
            }
        }
        if (found.empty()) {
            log(std::format("  {}: no half fails alone -- they fail together", text::join(group, ", ")));
            return group;
        }
        return found;
    };
    return go(std::vector<std::string>(units.begin(), units.end()), "");
}

int main(const Environment& env, std::span<const std::string> args) {
    const cli::Command command{
        "boot",
        "do the rebuilt drivers boot Windows 95 to the desktop?",
        {cli::Option{"--drivers", "", "the drivers to put in, comma-separated (vtd, vpicd, drvspacx ...: files of the corpus)", cli::Arg::Value, "NAME,..."},
         cli::Option{"--vmm32", "", "also VMM32.VXD", cli::Arg::Flag},
         cli::Option{"--retail", "", "the corpus originals instead of the rebuilt files: the baseline", cli::Arg::Flag},
         cli::Option{"--bisect", "", "on a failure, boot halves of the set until the drivers that fail alone are found", cli::Arg::Flag},
         cli::Option{"--disk", "", "a raw image of a hard disk Windows 95 is installed on (default: the setting win95_installed_hd)", cli::Arg::Value, "IMAGE"},
         cli::Option{"--wait", "", "how long to wait for the desktop", cli::Arg::Value, "SECONDS", "300"},
         cli::Option{"--tag", "", "the name the last screen and boot log are kept under", cli::Arg::Value, "NAME"}},
        [&](const cli::Args& a) -> int {
            std::vector<std::string> drivers;
            for (const std::string_view d : text::split(a.value_or("drivers", ""), ','))
                if (!text::trim(d).empty()) drivers.push_back(text::lower(text::trim(d)));
            const stdfs::path disk = a.value("disk") ? fs::from_text(*a.value("disk")) : env.default_disk;
            const bool vmm32 = a.flag("vmm32"), retail = a.flag("retail");
            const auto wait = std::chrono::seconds(a.integer<int>("wait").value_or(300));
            const std::string disk_text = fs::to_text(disk);
            std::error_code ec;
            if ((text::istarts_with(disk_text, "\\\\wsl$") || text::istarts_with(disk_text, "\\\\wsl.localhost")) && !stdfs::is_regular_file(disk, ec)) {
                try {
                    process::ProcessSpec wake;   // (WSL stops when idle)
                    wake.argv = {"wsl", "-e", "true"};
                    wake.timeout = std::chrono::seconds(120);
                    process::run(wake);
                } catch (const Error&) {   // no WSL here
                }
            }
            if (!stdfs::is_regular_file(disk, ec)) {
                out("no installed Windows 95 disk at {} (set win95_installed_hd in settings.json, or --disk)\n", disk_text);
                return kExitFailed;
            }
            const std::string tag = a.value("tag") ? *a.value("tag") : (retail ? "retail" : "rebuilt");   // (the last of a tag is kept)
            const Log say = [](std::string_view line) {
                out("{}\n", line);
                flush_out();
            };
            const std::string verdict = run(env, disk, drivers, vmm32, retail, tag, wait, say);
            if (verdict != "PASS" && a.flag("bisect")) {
                std::vector<std::string> units = drivers;
                if (vmm32) units.push_back("VMM32");
                if (units.size() > 1) {
                    const std::vector<std::string> culprits = bisect(
                        units,
                        [&](std::span<const std::string> part, const std::string& name) {
                            std::vector<std::string> only;
                            for (const std::string& u : part)
                                if (u != "VMM32") only.push_back(u);
                            return run(env, disk, only, std::find(part.begin(), part.end(), "VMM32") != part.end(), retail, name, wait, say);
                        },
                        tag, say);
                    out("fails: {}\n", text::join(culprits, ", "));
                }
            }
            return verdict == "PASS" ? kExitOk : kExitFailed;
        },
        {}, {}, {}};
    if (args.empty()) {
        out("{}\n", cli::Cli("vxd boot", {command}).help(command, "vxd boot"));
        return kExitUsage;
    }
    const cli::Cli program("boot", {command});
    std::vector<std::string> line = {"boot"};
    line.insert(line.end(), args.begin(), args.end());
    return program.run(line);
}

}  // namespace cb::source::vxd::boot
