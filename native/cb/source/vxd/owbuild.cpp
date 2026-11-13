// cb/source/vxd/owbuild.cpp -- a ring-0 VxD built with OpenWatcom's tools (see owbuild.hpp).
// cb-unity: alone
#include "cb/source/vxd/owbuild.hpp"

#include <algorithm>
#include <chrono>
#include <format>
#include <limits>
#include <tuple>

#include "cb/core/cli.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/log.hpp"
#include "cb/core/process.hpp"
#include "cb/core/text.hpp"
#include "cb/source/vxd/tree.hpp"

namespace cb::source::vxd::owbuild {

namespace stdfs = std::filesystem;
namespace le = formats::le;

namespace {

constexpr std::chrono::minutes kToolLimit{10};

bool is_file(const stdfs::path& p) {
    std::error_code ec;
    return stdfs::is_regular_file(p, ec);
}

std::map<std::string, std::string> tool_environment(const Tools& tools) {
    const std::string path = fs::get_env("PATH").value_or("");
    return {{"WATCOM", fs::to_text(tools.watcom)}, {"PATH", fs::to_text(tools.watcom / "binnt64") + ";" + path}};
}

}  // namespace

Tools tools_from_settings(const stdfs::path& checkout_root, std::string_view openwatcom_setting, const stdfs::path& nasm) {
    // <tree>/rel is what the setting names; the assembler lives in the tree's own build folder, beside rel
    const stdfs::path tree = openwatcom_setting.empty() ? checkout_root / "experiments" / "premise-proof" / "open-watcom-v2"
                                                        : fs::from_text(openwatcom_setting).parent_path();
    Tools tools;
    tools.wasm = tree / "bld" / "wasm" / "nt386" / "wasm.exe";
    tools.wlink = tree / "rel" / "binnt64" / "wlink.exe";
    tools.watcom = tree / "rel";
    tools.nasm = nasm;
    return tools;
}

Result build_vxd(const Environment& env, const le::Module& driver, std::string_view name, const stdfs::path& out, bool byte_exact) {
    const Tools& tools = env.tools;
    Result result;
    if (!is_file(tools.wasm) || !is_file(tools.wlink)) {
        result.error = "OpenWatcom wasm/wlink not built";
        return result;
    }
    // Written UNSPLIT, so cross-object branch targets stay inside their file. A byte-exact build writes raw the instructions the assembler
    // re-encodes differently from the original (the verified force-raw set), so that the built file is byte-identical to retail; otherwise
    // each file is converged to assemble, and may drift.
    tree::Options options;
    options.max_instructions = std::numeric_limits<std::size_t>::max();
    link::ForceRaw force_raw;
    if (byte_exact) {
        force_raw = link::byte_exact_force_raw(env.link, driver).force_raw;
        options.force_raw = &force_raw;
    } else {
        options.probe = tree::Probe{tree::Probe::Kind::Wasm, tools.wasm};
    }
    const std::string upper = text::upper(name);
    const std::vector<std::string> files = tree::emit_tree(driver, upper, out, env.link.names, options);
    const stdfs::path folder = out / text::lower(name);
    const std::map<std::string, std::string> environment = tool_environment(tools);

    std::vector<std::string> objects, failed;
    bool has_ddb = false;
    for (const std::string& file : files) {
        if (!file.ends_with(".asm")) continue;
        if (fs::read_text(folder / file).find("public VXD_DDB") != std::string::npos) has_ddb = true;
        const std::string object = file.substr(0, file.size() - 4) + ".obj";
        process::ProcessSpec spec;
        spec.cwd = folder;
        spec.env = environment;
        spec.timeout = kToolLimit;
        // the byte-exact build uses the same nasm-masm (COFF) the force-raw set was verified against; the buildable one uses wasm
        spec.argv = byte_exact ? std::vector<std::string>{fs::to_text(tools.nasm), "--masm", "-f", "win32", file, "-o", object}
                               : std::vector<std::string>{fs::to_text(tools.wasm), "-zcm=masm", "-q", "-fo=" + object, file};
        const process::ProcessResult r = process::run(spec);
        if (r.exit_code == 0 && is_file(folder / object)) objects.push_back(object);
        else failed.push_back(file);
    }
    if (!failed.empty()) {
        result.error = "assemble failed: " + text::join(failed, ", ");
        return result;
    }
    std::string link_file = std::format("system win_vxd\nname {}.vxd\noption quiet\nfile {}\n", text::lower(name), text::join(objects, ","));
    if (has_ddb) link_file += "export VXD_DDB.1\n";   // a VxD's single required export is its DDB
    fs::write_text(folder / "link.lnk", link_file);
    process::ProcessSpec link_spec;
    link_spec.argv = {fs::to_text(tools.wlink), "@link.lnk"};
    link_spec.cwd = folder;
    link_spec.env = environment;
    link_spec.timeout = kToolLimit;
    process::run(link_spec);
    const stdfs::path built = folder / (text::lower(name) + ".vxd");
    if (is_file(built)) {
        result.ok = true;
        result.output = built;
        result.bytes = fs::file_size(built);
        result.objects = objects.size();
        return result;
    }
    result.error = "link produced no .vxd (missing DDB export?)";
    return result;
}

std::vector<Component> w3_components(ByteView data) {
    if (data.size() < 0x40) return {};   // no DOS header to point at a bundle
    io::ByteReader reader(data);
    reader.seek(0x3C);
    const std::uint32_t lfa = reader.u32();
    if (lfa + 2 > data.size() || data[lfa] != 'W' || data[lfa + 1] != '3') return {};
    reader.seek(lfa + 4);
    const std::uint32_t count = reader.u32();
    std::vector<Component> components;
    for (std::uint32_t i = 0; i < count; ++i) {
        const std::uint64_t at = static_cast<std::uint64_t>(lfa) + 16 + static_cast<std::uint64_t>(i) * 16;
        reader.seek(at);
        const ByteView raw = reader.bytes(8);
        std::string name = text::from_latin1(raw.subspan(0, std::find(raw.begin(), raw.end(), std::uint8_t{0}) - raw.begin()));
        const std::uint32_t offset = reader.u32(), size = reader.u32();
        components.push_back({std::string(text::trim(name)), offset, size});
    }
    return components;
}

BundleResult build_w3(const Environment& env, const stdfs::path& bundle, const stdfs::path& out) {
    const Bytes data = fs::read_file(bundle);
    BundleResult result;
    const std::vector<Component> components = w3_components(data);
    io::ByteReader reader(data);
    reader.seek(0x3C);
    const std::uint32_t lfa = reader.u32();
    if (components.empty()) {
        result.error = "not a W3 bundle";
        return result;
    }
    Bytes front(data.begin(), data.begin() + lfa);   // the MZ and real-mode loader, kept
    std::vector<std::pair<std::string, Result>> built;
    for (const Component& c : components) {
        // the LE module runs from its offset to the next module (or the end of the file)
        std::uint32_t end = static_cast<std::uint32_t>(data.size());
        for (const Component& other : components)
            if (other.offset > c.offset) end = std::min(end, other.offset);
        fs::ScopedTempDir scratch("cbowbuildw3");
        const stdfs::path part = scratch.path() / "part.vxd";
        fs::write_file(part, ByteView(data).subspan(c.offset, end - c.offset));
        const auto modules = le::open_modules(part);
        if (modules.empty()) {
            built.emplace_back(c.name, Result{});
            continue;
        }
        built.emplace_back(c.name, build_vxd(env, modules.front().second, c.name, out));
    }
    for (const auto& [name, r] : built) result.components.emplace_back(name, r.ok);
    if (!std::all_of(built.begin(), built.end(), [](const auto& b) { return b.second.ok; })) {
        result.error = "component build failed";
        return result;
    }
    // front + header + records + the page-aligned LE modules
    const std::size_t header_size = 16 + components.size() * 16;
    const std::size_t body_start = (lfa + header_size + 0x3FF) & ~static_cast<std::size_t>(0x3FF);   // the first module, page-aligned
    Bytes image = front;
    io::ByteWriter header;
    header.bytes(as_bytes(std::string_view("W3")));
    header.u16(0x0400);
    header.u32(static_cast<std::uint32_t>(components.size()));
    header.zeros(8);
    image.insert(image.end(), header.data().begin(), header.data().end());
    const std::size_t record_at = image.size();
    image.resize(image.size() + components.size() * 16, 0);   // placeholder records
    image.resize(std::max(image.size(), body_start), 0);
    std::vector<std::tuple<std::string, std::uint32_t, std::uint32_t>> records;
    for (const auto& [name, r] : built) {
        const Bytes module = fs::read_file(r.output);
        const auto offset = static_cast<std::uint32_t>(image.size());
        image.insert(image.end(), module.begin(), module.end());
        while (image.size() % 0x400 != 0) image.push_back(0);   // the next module, page-aligned
        records.emplace_back(name, offset, static_cast<std::uint32_t>(module.size()));
    }
    for (std::size_t i = 0; i < records.size(); ++i) {
        const auto& [name, offset, size] = records[i];
        const std::size_t at = record_at + i * 16;
        std::string padded = name.substr(0, 8);
        padded.resize(8, ' ');
        std::copy(padded.begin(), padded.end(), image.begin() + static_cast<std::ptrdiff_t>(at));
        for (int k = 0; k < 4; ++k) {
            image[at + 8 + static_cast<std::size_t>(k)] = static_cast<std::uint8_t>(offset >> (8 * k));
            image[at + 12 + static_cast<std::size_t>(k)] = static_cast<std::uint8_t>(size >> (8 * k));
        }
    }
    fs::make_dirs(out);
    result.output = out / text::lower(fs::to_text(bundle.filename()));
    fs::write_file(result.output, image);
    result.ok = true;
    result.bytes = image.size();
    return result;
}

int main(const CommandEnvironment& env, std::span<const std::string> args) {
    const cli::Command command{
        "owbuild",
        "a driver built with OpenWatcom's assembler and linker only",
        {cli::Positional{"vxd", "a .vxd or .386 file, or a corpus name", true},
         cli::Option{"--out", "", "where the built driver goes (default: the state folder's vxd_ow)", cli::Arg::Value, "DIR"}},
        [&](const cli::Args& a) -> int {
            stdfs::path path = fs::from_text(a.required("vxd"));
            std::error_code ec;
            if (!stdfs::is_regular_file(path, ec))
                if (const auto found = env.corpus.retail(a.required("vxd"))) path = *found;   // a name the corpus knows
            const stdfs::path out_folder = a.value("out") ? fs::from_text(*a.value("out")) : env.output_folder;
            for (const auto& [member, module] : le::open_modules(path)) {
                const std::string stem = member.substr(0, member.find('.'));
                const Result r = build_vxd(env.build, module, stem, out_folder);
                if (r.ok) out("{}: built {} bytes ({} objs) -> {}\n", stem, r.bytes, r.objects, fs::to_text(r.output));
                else out("{}: FAIL {}\n", stem, r.error);
                break;
            }
            return kExitOk;
        },
        {}, {}, {}};
    const cli::Cli program("owbuild", {command});
    std::vector<std::string> line = {"owbuild"};
    line.insert(line.end(), args.begin(), args.end());
    return program.run(line);
}

}  // namespace cb::source::vxd::owbuild
