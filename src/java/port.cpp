// java/port.cpp -- see port.hpp.
#include "java/port.hpp"

#include <cctype>
#include <format>
#include <system_error>

#include "core/fs.hpp"
#include "core/text.hpp"
#include "java/console/project_scan.hpp"
#include "java/console/zip_archive.hpp"

namespace cb::java::port {

namespace {

namespace stdfs = std::filesystem;

bool pt_is_jar(const stdfs::path& p) {
    const std::string ext = text::lower(fs::to_text(p.extension()));
    return ext == ".jar" || ext == ".zip";
}

bool pt_digit(char c) { return std::isdigit(static_cast<unsigned char>(c)) != 0; }

// A version at `i` of `s`: [a|b|c|rd-|inf-|in-]digits(.digits)*(_suffix)? -- returns its length, 0 when there is none.
std::size_t pt_version_at(std::string_view s, std::size_t i) {
    std::size_t j = i;
    for (const std::string_view prefix : {"rd-", "inf-", "in-", "a", "b", "c"}) {
        if (s.substr(i, prefix.size()) == prefix && i + prefix.size() < s.size() && pt_digit(s[i + prefix.size()])) {
            j = i + prefix.size();
            break;
        }
    }
    if (j >= s.size() || !pt_digit(s[j])) return 0;
    const bool dated = j > i && s[j - 1] == '-';   // inf-20100618, rd-132211: digits alone
    std::size_t k = j;
    while (k < s.size() && pt_digit(s[k])) ++k;
    bool dotted = false;
    while (k + 1 < s.size() && s[k] == '.' && pt_digit(s[k + 1])) {
        dotted = true;
        k += 1;
        while (k < s.size() && pt_digit(s[k])) ++k;
    }
    if (!dotted && !dated) return 0;   // a lone number is not a version
    if (k + 1 < s.size() && s[k] == '_' && std::isalnum(static_cast<unsigned char>(s[k + 1]))) {   // c0.30_01c
        ++k;
        while (k < s.size() && std::isalnum(static_cast<unsigned char>(s[k]))) ++k;
    }
    return k - i;
}

}  // namespace

std::optional<Platform> parse_platform(std::string_view text) {
    std::string t;
    for (const char c : text)
        if (c != ' ' && c != '-' && c != '_') t += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    if (t == "pc" || t == "pcopengl" || t == "windows" || t == "opengl") return Platform::Pc;
    if (t == "xbox360" || t == "360" || t == "x360" || t == "xbox") return Platform::Xbox360;
    return std::nullopt;
}

std::string_view platform_id(Platform platform) { return platform == Platform::Xbox360 ? "xbox360" : "pc_opengl"; }
std::string_view platform_title(Platform platform) { return platform == Platform::Xbox360 ? "Xbox 360" : "PC"; }

std::string_view kind_title(SourceKind kind) {
    switch (kind) {
        case SourceKind::JavaTree: return "a Java source folder";
        case SourceKind::GameFolder: return "a decompiled game folder";
        case SourceKind::SourceJar: return "a jar of Java source";
        case SourceKind::ClassJar: return "a game jar";
        case SourceKind::Unknown: break;
    }
    return "something that is not a Java program";
}

std::string version_from_name(std::string_view stem) {
    const std::string s = text::lower(stem);
    for (std::size_t i = 0; i < s.size(); ++i) {
        if (i > 0 && std::isalnum(static_cast<unsigned char>(s[i - 1])) && !pt_digit(s[i])) continue;   // a version starts a word
        if (i > 0 && pt_digit(s[i - 1])) continue;
        if (const std::size_t n = pt_version_at(s, i); n > 0) return s.substr(i, n);
    }
    return {};
}

std::string project_name(std::string_view stem) {
    std::string out;
    for (const char c : stem) out += std::isalnum(static_cast<unsigned char>(c)) ? c : '_';
    if (out.empty() || pt_digit(out.front())) out.insert(out.begin(), 'P');
    return out;
}

JarCounts count_jar(const stdfs::path& jar) {
    JarCounts counts;
    const console::ZipArchive zip = console::ZipArchive::open(jar);
    for (const console::ZipArchive::Entry& e : zip.entries()) {
        if (e.is_directory()) continue;
        if (e.name.ends_with(".java")) ++counts.java;
        else if (e.name.ends_with(".class")) ++counts.classes;
    }
    return counts;
}

std::size_t extract_java_sources(const stdfs::path& jar, const stdfs::path& dest) {
    const console::ZipArchive zip = console::ZipArchive::open(jar);
    std::size_t written = 0;
    for (const console::ZipArchive::Entry& e : zip.entries()) {
        if (e.is_directory() || !e.name.ends_with(".java")) continue;
        if (e.name.find("..") != std::string::npos || e.name.starts_with('/')) continue;   // never outside dest
        const stdfs::path target = dest / fs::from_text(e.name);
        fs::make_dirs(target.parent_path());
        const Bytes data = zip.read(e);
        fs::write_file(target, ByteView(data.data(), data.size()));
        ++written;
    }
    return written;
}

Plan plan(const stdfs::path& input_in, Platform platform, const std::optional<stdfs::path>& out_root) {
    Plan p;
    p.platform = platform;
    std::error_code ec;
    p.input = stdfs::absolute(input_in, ec).lexically_normal();
    if (!p.input.has_filename()) p.input = p.input.parent_path();
    const std::string stem = fs::to_text(p.input.stem());
    p.name = project_name(stem);
    p.root = out_root ? stdfs::absolute(*out_root, ec).lexically_normal() : p.input.parent_path() / (fs::to_text(p.input.filename().stem()) + "-converted");
    p.project = p.root / p.name;

    if (stdfs::is_directory(p.input, ec)) {
        const console::project_scan::Resolved r = console::project_scan::resolve(p.input);
        p.java_src = r.java_src;
        p.jar = r.jar;
        p.resources = r.resources;
        p.lang = r.lang;
        if (r.jar || r.resources) p.kind = SourceKind::GameFolder;
        else if (r.java_src) p.kind = SourceKind::JavaTree;
        if (p.kind == SourceKind::GameFolder && !p.java_src && p.jar) p.mc_version = version_from_name(fs::to_text(p.jar->stem()));
        if (p.kind == SourceKind::Unknown) p.problem = "this folder holds no Java source, no game jar and no game resources";
        else if (!p.java_src && p.mc_version.empty())
            p.problem = "this game folder has no Java source, and its jar's name does not say which Minecraft version to decompile";
    } else if (stdfs::is_regular_file(p.input, ec) && pt_is_jar(p.input)) {
        JarCounts counts;
        try {
            counts = count_jar(p.input);
        } catch (const std::exception& e) {
            p.problem = std::format("this file cannot be read as a jar: {}", e.what());
            return p;
        }
        if (counts.java > 0) {
            p.kind = SourceKind::SourceJar;
        } else if (counts.classes > 0) {
            p.kind = SourceKind::ClassJar;
            p.jar = p.input;
            p.mc_version = version_from_name(stem);
            // a jar inside a decompiled game folder: that folder's source, sounds and language file come with it
            const console::project_scan::Resolved r = console::project_scan::resolve(p.input.parent_path());
            if (r.jar || r.resources) {
                p.resources = r.resources;
                p.lang = r.lang;
                if (r.java_src) p.java_src = r.java_src;
            }
            if (!p.java_src && p.mc_version.empty())
                p.problem = "this jar holds compiled classes only and its name does not say which Minecraft version it is; "
                            "drop the decompiled game folder, a source folder, or a jar named after its version (a1.2.6.jar)";
        } else {
            p.problem = "this jar holds neither Java source nor classes";
        }
    } else {
        p.problem = stdfs::exists(p.input, ec) ? "drop a .jar, a decompiled game folder or a Java source folder" : "there is no such file or folder";
    }
    if (!p.problem.empty()) return p;

    // the steps, in plain words
    if (p.kind == SourceKind::SourceJar) p.steps.push_back("Unpack the Java source");
    if (!p.java_src && !p.mc_version.empty() && p.kind != SourceKind::SourceJar) p.steps.push_back("Decompile Minecraft " + p.mc_version + " to Java");
    p.steps.push_back(std::format("Convert the Java to C++ for {}", platform_title(platform)));
    if (platform == Platform::Xbox360) p.steps.push_back("Write the platform config and the Visual Studio solution");
    if (p.jar) p.steps.push_back("Copy the game's textures and resources into Content");
    if (platform == Platform::Xbox360) {
        if (p.resources) p.steps.push_back("Make the XACT audio banks");
        p.steps.push_back("Map the achievements");
        p.steps.push_back("Build the solution (when MSBuild is set up)");
    } else {
        p.steps.push_back("Build it with CMake (when CMake is installed)");
    }
    return p;
}

}  // namespace cb::java::port
