// java/projects/minecraft/prep/prepare.cpp -- see prepare.hpp. Ported from RetroMCP (org.mcphackers.mcp.tasks.TaskDecompile: its stages and
// applyInjector; tools.fernflower.Decompiler; tools.FileUtil.compress / deletePackages / deleteEmptyFolders / copyDirectory) and RDI
// (nio.RDInjector: the order of its transforms and visitors; nio.IOUtil.readJar, ClassStorageWriter.write), MIT License, (c) MCPHackers.
// cb-unity: alone (file-local helpers of the port)
#include "java/projects/minecraft/prep/prepare.hpp"

#include <algorithm>
#include <format>
#include <memory>
#include <set>

#include <nlohmann/json.hpp>

#include "core/fs.hpp"
#include "core/json.hpp"
#include "java/bytecode/tree.hpp"
#include "formats/zip.hpp"
#include "java/decompiler/decompiler.hpp"
#include "java/projects/minecraft/prep/constants.hpp"
#include "java/projects/minecraft/prep/fixes.hpp"
#include "java/projects/minecraft/prep/patch.hpp"
#include "java/projects/minecraft/prep/remap.hpp"
#include "java/projects/minecraft/prep/storage.hpp"

namespace cb::java::minecraft::prep {

namespace stdfs = std::filesystem;

namespace {

// RDI's IOUtil.readJar: every .class entry, in the jar's order.
ClassStorage pp_read_jar(const stdfs::path& path) {
    const auto zip = formats::ZipArchive::open(path);
    std::vector<std::unique_ptr<bc::ClassNode>> nodes;
    for (const auto& e : zip.entries()) {
        if (e.name.ends_with(".class")) nodes.push_back(std::make_unique<bc::ClassNode>(bc::read(zip.read(e), e.name)));
    }
    return ClassStorage(std::move(nodes));
}

// RDI's ClassStorageWriter.write(out, resources): each class once, then each resource jar's entries that are not classes (META-INF's files but the
// manifest left out), each name once.
void pp_write_jar(const ClassStorage& storage, const std::vector<stdfs::path>& resources, const stdfs::path& out) {
    formats::ZipWriter z;
    std::set<std::string> written;
    for (const bc::ClassNode* node : storage) {
        if (!written.insert(node->name).second) continue;
        z.add(node->name + ".class", bc::write(*node));
    }
    std::set<std::string> added;
    for (const stdfs::path& path : resources) {
        const auto zip = formats::ZipArchive::open(path);
        for (const auto& e : zip.entries()) {
            if (e.name.starts_with("META-INF/") && e.name != "META-INF/MANIFEST.MF" && !e.is_directory()) continue;
            if (e.name.ends_with(".class")) continue;
            if (!added.insert(e.name).second) continue;
            z.add(e.name, e.is_directory() ? Bytes{} : zip.read(e));
        }
    }
    fs::write_file(out, z.finish());
}

// FileUtil.cleanDirectory: what the folder holds deleted (the folder kept)
void pp_clean_directory(const stdfs::path& dir) {
    std::error_code ec;
    if (!stdfs::is_directory(dir, ec)) return;
    for (const auto& e : stdfs::directory_iterator(dir)) stdfs::remove_all(e.path());
}

// FileUtil.compress: every file under `dir`, by its path relative to it
void pp_compress(const stdfs::path& dir, const stdfs::path& target) {
    formats::ZipWriter z;
    std::vector<stdfs::path> files;
    for (const auto& e : stdfs::recursive_directory_iterator(dir)) {
        if (e.is_regular_file()) files.push_back(e.path());
    }
    for (const stdfs::path& f : files) z.add(fs::to_text(stdfs::relative(f, dir).generic_string()), fs::read_file(f));
    fs::write_file(target, z.finish());
}

// Path.startsWith(Paths.get(prefix)): by whole names
bool pp_starts_with(const stdfs::path& relative, const std::string& prefix) {
    const stdfs::path p = fs::from_text(prefix);
    auto a = relative.begin();
    for (auto b = p.begin(); b != p.end(); ++b, ++a) {
        if (a == relative.end() || *a != *b) return false;
    }
    return true;
}

// FileUtil.deletePackages, then deleteEmptyFolders: the files (and the empty folders) under an excluded package deleted; then every folder that is
// empty, deepest first (the folder itself too when it ends empty)
void pp_delete_packages(const stdfs::path& dir, const std::vector<std::string>& excluded) {
    std::vector<stdfs::path> all;
    for (const auto& e : stdfs::recursive_directory_iterator(dir)) all.push_back(e.path());
    for (const stdfs::path& p : all) {
        std::error_code ec;
        if (stdfs::is_directory(p, ec) && !stdfs::is_empty(p, ec)) continue;
        const stdfs::path rel = stdfs::relative(p, dir);
        for (const std::string& pkg : excluded) {
            if (pp_starts_with(rel, pkg)) {
                stdfs::remove(p, ec);
                break;
            }
        }
    }
    std::vector<stdfs::path> dirs{dir};
    for (const auto& e : stdfs::recursive_directory_iterator(dir)) {
        if (e.is_directory()) dirs.push_back(e.path());
    }
    std::sort(dirs.begin(), dirs.end(), std::greater<>());
    for (const stdfs::path& d : dirs) {
        std::error_code ec;
        if (stdfs::is_empty(d, ec)) stdfs::remove(d, ec);
    }
}

// FileUtil.copyDirectory
void pp_copy_directory(const stdfs::path& from, const stdfs::path& to) {
    stdfs::create_directories(to);
    stdfs::copy(from, to, stdfs::copy_options::recursive | stdfs::copy_options::overwrite_existing);
}

Version pp_version(const stdfs::path& workspace) { return Version::from(json::parse(fs::read_text(mcp_paths::get(workspace, mcp_paths::VERSION)))); }

// The access file of a side, when there is one
std::optional<Access> pp_access(const stdfs::path& workspace, Side side) {
    const stdfs::path acc = mcp_paths::get(workspace, mcp_paths::ACCESS, side);
    std::error_code ec;
    if (!stdfs::exists(acc, ec)) return std::nullopt;
    return load_access(acc);
}

// TaskDecompile.applyInjector: the side's classes read, remapped and fixed, written to its deobfuscated jar. The order is RDInjector's: the global
// transforms in the order they were added, then the visitors, the first added innermost (it visits first).
void pp_apply_injector(const stdfs::path& workspace, Side side, const PrepareOptions& options) {
    const stdfs::path rdi_out = mcp_paths::get(workspace, mcp_paths::REMAPPED, side);
    const stdfs::path mappings_path = mcp_paths::get(workspace, mcp_paths::MAPPINGS);
    const bool has_lwjgl = side == Side::Client || side == Side::Merged;

    ClassStorage storage;
    std::vector<stdfs::path> resources;
    std::vector<std::function<void(ClassStorage&)>> globals;
    const auto strip = [&] {
        if (!options.strip_generics) return;
        globals.emplace_back([](ClassStorage& s) { Transform::strip_lvt(s); });
        globals.emplace_back([](ClassStorage& s) { Transform::strip_signatures(s); });
    };
    const auto map_with = [&](Side mapped_side) {
        if (std::optional<Mappings> mappings = get_mappings(mappings_path, storage, side_name(mapped_side))) {
            auto shared = std::make_shared<Mappings>(std::move(*mappings));
            globals.emplace_back([shared](ClassStorage& s) {
                const Mappings completed = completed_for_source(*shared, s);   // (what the source needs to compile: remap.hpp)
                Remapper().load(s).load(completed).process();
            });
        }
    };
    const auto run_globals = [&] {
        for (auto& g : globals) g(storage);
        globals.clear();
    };
    ClassStorage server_storage;
    if (side == Side::Merged) {
        const stdfs::path server_jar = mcp_paths::get(workspace, mcp_paths::JAR_ORIGINAL, Side::Server);
        storage = pp_read_jar(server_jar);
        resources.push_back(server_jar);
        strip();
        map_with(Side::Server);
        run_globals();   // (the injector's transform(): no visitors yet)
        server_storage = std::move(storage);
        const stdfs::path client_jar = mcp_paths::get(workspace, mcp_paths::JAR_ORIGINAL, Side::Client);
        storage = pp_read_jar(client_jar);
        resources.push_back(client_jar);
        strip();
        map_with(Side::Client);
        globals.emplace_back([&server_storage](ClassStorage& s) { Transform::merge(s, server_storage); });
    } else {
        const stdfs::path jar = mcp_paths::get(workspace, mcp_paths::JAR_ORIGINAL, side);
        storage = pp_read_jar(jar);
        resources.push_back(jar);
        strip();
        map_with(side);
    }
    globals.emplace_back([](ClassStorage& s) { Transform::decompose_vars(s); });
    globals.emplace_back([](ClassStorage& s) { Transform::replace_common_constants(s); });
    std::unique_ptr<ClassVisitor> visitors;
    if (has_lwjgl) {
        std::shared_ptr<const GLConstantsData> data;
        std::error_code ec;
        if (!options.gl_constants.empty() && stdfs::exists(options.gl_constants, ec)) data = std::make_shared<GLConstantsData>(GLConstantsData::load(options.gl_constants));
        visitors = ClassVisitor::merge(std::move(visitors), std::make_unique<GLConstants>(nullptr, std::move(data)));
    }
    globals.emplace_back([](ClassStorage& s) { Transform::restore_source_file(s); });
    globals.emplace_back([](ClassStorage& s) { Transform::fix_inner_classes(s); });
    visitors = std::make_unique<ClassInitAdder>(storage, std::move(visitors));
    std::shared_ptr<GuessGenerics> generics;
    if (options.guess_generics) {
        generics = std::make_shared<GuessGenerics>(storage);
        globals.emplace_back([generics](ClassStorage& s) { generics->transform(s); });
    }
    const stdfs::path exc = mcp_paths::get(workspace, mcp_paths::EXC);
    std::error_code ec;
    if (stdfs::exists(exc, ec)) {
        if (std::optional<Exceptions> exceptions = load_exceptions(exc)) visitors = std::make_unique<AddExceptions>(std::move(*exceptions), std::move(visitors));
    }
    for (const Side s : side == Side::Merged ? std::vector<Side>{Side::Client, Side::Server} : std::vector<Side>{side}) {
        if (std::optional<Access> access = pp_access(workspace, s)) visitors = std::make_unique<AccessFixer>(std::move(*access), std::move(visitors));
    }
    run_globals();
    if (visitors) visitors->visit(storage);
    pp_write_jar(storage, resources, rdi_out);
}

}  // namespace

stdfs::path decompile_side(const stdfs::path& workspace, Side side, const PrepareOptions& options, std::vector<std::string>& messages) {
    const auto say = [&](const std::string& line) {
        if (options.on_output) options.on_output(line);
    };
    const std::string name = side_name(side);
    const stdfs::path rdi_out = mcp_paths::get(workspace, mcp_paths::REMAPPED, side);
    const stdfs::path ff_out = mcp_paths::get(workspace, mcp_paths::SOURCE_UNPATCHED, side);
    const stdfs::path src_path = mcp_paths::get(workspace, mcp_paths::SOURCE, side);
    const stdfs::path patches_path = mcp_paths::get(workspace, mcp_paths::CONF_PATCHES, side);

    // prepare
    pp_clean_directory(mcp_paths::get(workspace, mcp_paths::PROJECT, side));
    stdfs::create_directories(mcp_paths::get(workspace, mcp_paths::JARS_DIR, side));
    stdfs::create_directories(mcp_paths::get(workspace, mcp_paths::MD5_DIR, side));
    stdfs::create_directories(mcp_paths::get(workspace, mcp_paths::GAMEDIR, side));

    say(std::format("[{}] remapping and fixing the classes", name));
    pp_apply_injector(workspace, side, options);

    say(std::format("[{}] decompiling", name));
    java::decompiler::DecompileRequest req;
    req.sources.push_back(rdi_out);
    for (const stdfs::path& lib : DownloadData::get_libraries(mcp_paths::get(workspace, mcp_paths::LIB), pp_version(workspace))) {
        std::error_code ec;
        if (stdfs::exists(lib, ec)) req.libraries.push_back(lib);
    }
    // RetroMCP's Fernflower options (TaskParameter.getDefaultFFOptions over the defaults), the bridge methods kept unless generics are guessed
    req.options = {{"nco", "1"}, {"rbr", options.guess_generics ? "1" : "0"}, {"asc", "1"}, {"ovr", "0"}, {"dgs", "1"}, {"ind", "\t"}};
    req.on_log = [&](const std::string& line) {
        if (line.starts_with("ERROR:") || line.starts_with("WARN:")) messages.push_back(line);
    };
    const java::decompiler::DecompileResult res = java::decompiler::decompile(req);
    stdfs::create_directories(ff_out);
    const int classes = java::decompiler::save_into_folder(res, ff_out);
    say(std::format("[{}] {} classes decompiled", name, classes));

    // patch
    std::error_code ec;
    if (options.patches && stdfs::exists(patches_path, ec)) {
        const PatchOutcome r = apply_patch(ff_out, ff_out, patches_path);
        if (!r.success) {
            messages.push_back(r.log);
            messages.push_back(std::format("[{}] Patching failed!", name));
        }
        say(std::format("[{}] patched", name));
    }
    // (Source.modify with RetroMCP's source adapters: there are none but a plugin's)

    // copysrc
    if (!options.decompile_resources) {
        std::vector<stdfs::path> other;
        for (const auto& e : stdfs::recursive_directory_iterator(ff_out)) {
            if (e.is_regular_file() && e.path().extension() != ".java") other.push_back(e.path());
        }
        for (const stdfs::path& p : other) stdfs::remove(p);
    }
    stdfs::create_directories(src_path);
    pp_compress(ff_out, mcp_paths::get(workspace, mcp_paths::SOURCE_JAR, side));
    if (options.output_src) {
        pp_delete_packages(ff_out, options.ignored_packages);
        if (stdfs::exists(ff_out, ec)) pp_copy_directory(ff_out, src_path);
    }
    if (includes_client(side)) {
        const stdfs::path assets = mcp_paths::get(workspace, mcp_paths::JARS) / "assets";
        if (stdfs::exists(assets, ec)) pp_copy_directory(assets, mcp_paths::get(workspace, mcp_paths::GAMEDIR, side) / "assets");
    }
    return src_path;
}

PrepareResult prepare(const std::string& version, const stdfs::path& workspace, const PrepareOptions& options) {
    PrepareResult result;
    result.workspace = workspace;
    if (options.setup) {
        if (options.on_output) options.on_output("[setup] " + version);
        SetupOptions so = options.setup_options;
        so.side = options.side;
        const SetupResult made = setup(version, workspace, so);
        result.messages.insert(result.messages.end(), made.messages.begin(), made.messages.end());
    }
    // TaskDecompile's requirement: the side's original jar is readable (MERGED: both)
    const auto has = [&](Side s) {
        std::error_code ec;
        return stdfs::is_regular_file(mcp_paths::get(workspace, mcp_paths::JAR_ORIGINAL, s), ec);
    };
    std::vector<Side> sides;
    if (options.side == Side::Any) {
        for (const Side s : {Side::Client, Side::Server}) {
            if (has(s)) sides.push_back(s);
        }
    } else if (options.side == Side::Merged ? (has(Side::Client) && has(Side::Server)) : has(options.side)) {
        sides.push_back(options.side);
    }
    for (const Side s : sides) result.sources.push_back(decompile_side(workspace, s, options, result.messages));
    return result;
}

}  // namespace cb::java::minecraft::prep
