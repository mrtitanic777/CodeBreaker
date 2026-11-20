// build/x86link/replay.cpp -- see replay.hpp.
#include "build/x86link/replay.hpp"

#include <algorithm>
#include <cctype>
#include <format>
#include <map>
#include <utility>

#include <nlohmann/json.hpp>

#include "build/x86link/x86link.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"
#include "build/x86link/replay_paths.hpp"

namespace cb::build::x86link::replay {

namespace stdfs = std::filesystem;
using replay_paths::lower;
using replay_paths::PathMap;
using replay_paths::slashes;

namespace {

// The messages as compared: CRLF as LF, the banner's first line (it names wlink's build time) left out.
std::string messages(std::string text) {
    text = text::replace_all(text, "\r\n", "\n");
    std::string out;
    for (std::string_view line : text::lines(text)) {
        if (line.starts_with("Open Watcom Linker Version") || line.starts_with("Created on:") || line.starts_with("Link time:")) continue;   // (the build and the time)
        out += std::string(line) + "\n";
    }
    return out;
}

// A PE image with the fields that say when it was linked cleared: the COFF header's TimeDateStamp and the optional header's
// CheckSum (which covers the stamp). wlink.exe writes the link's time there, so two links of the same input differ only in
// them; every other byte is compared.
Bytes without_link_time(Bytes image) {
    auto u32 = [&](std::size_t at) -> std::uint32_t {
        return at + 4 <= image.size() ? image[at] | image[at + 1] << 8 | image[at + 2] << 16 | static_cast<std::uint32_t>(image[at + 3]) << 24 : 0;
    };
    if (image.size() < 0x40) return image;
    // (behind an MZ stub, or at the start: `option nostub`)
    const bool stub = image[0] == 'M' && image[1] == 'Z';
    if (!stub && !(image[0] == 'P' && image[1] == 'E')) return image;
    const std::size_t pe = stub ? u32(0x3C) : 0;
    // (PE, or PL: Phar Lap TNT's PE, `format windows nt tnt`)
    if (pe + 24 + 68 > image.size() || image[pe] != 'P' || (image[pe + 1] != 'E' && image[pe + 1] != 'L') || image[pe + 2] != 0 ||
        image[pe + 3] != 0)
        return image;
    auto clear4 = [&](std::size_t at) {
        for (std::size_t i = 0; i < 4 && at + i < image.size(); ++i) image[at + i] = 0;
    };
    clear4(pe + 8);         // TimeDateStamp
    clear4(pe + 24 + 64);   // CheckSum
    // ... and each debug directory entry's TimeDateStamp (debug codeview: the same link time, again)
    auto u16 = [&](std::size_t at) -> std::uint32_t { return at + 2 <= image.size() ? image[at] | image[at + 1] << 8 : 0; };
    const std::size_t opt = pe + 24;
    const std::uint32_t magic = u16(opt);
    const std::size_t dirs = opt + (magic == 0x20b ? 112 : 96);
    const std::uint32_t debug_rva = u32(dirs + 6 * 8), debug_size = u32(dirs + 6 * 8 + 4);
    const std::size_t sections = opt + u16(pe + 20);
    for (std::uint32_t s = 0, n = u16(pe + 6); debug_rva != 0 && s < n; ++s) {
        const std::size_t h = sections + s * 40;
        const std::uint32_t va = u32(h + 12), vsize = std::max(u32(h + 8), u32(h + 16)), raw = u32(h + 20);
        if (debug_rva < va || debug_rva >= va + vsize) continue;
        for (std::uint32_t e = 0; e + 28 <= debug_size; e += 28) clear4(raw + (debug_rva - va) + e + 4);
        break;
    }
    return image;
}

bool is_text_input(const std::string& as) {
    const std::string l = lower(as);
    return l.ends_with(".lnk") || l.ends_with(".def") || l.ends_with(".txt") || l.ends_with(".cmd");
}

}  // namespace

Outcome one(const stdfs::path& case_dir, const stdfs::path& blobs, const stdfs::path& scratch, const stdfs::path& watcom) {
    Outcome o;
    o.id = case_dir.filename().string();
    const nlohmann::json c = nlohmann::json::parse(fs::read_text(case_dir / "case.json"));
    const stdfs::path root = stdfs::absolute(scratch) / o.id;   // (absolute: the link runs inside the case's folder)
    std::error_code ec;
    stdfs::remove_all(root, ec);
    fs::make_dirs(root / "cwd");

    PathMap map;
    const std::string cwd = c.at("cwd").get<std::string>();
    map.add(cwd, root / "cwd");
    std::map<std::string, std::string> env;
    if (c.contains("env"))
        for (const auto& [k, v] : c["env"].items()) env[k] = v.get<std::string>();
    // the run's OpenWatcom is this one (the same build, wherever it is now): its libraries, wlink.lnk and the default
    // libraries the objects ask for are read from it, not from copies (the captures hold only what the directives name)
    if (env.contains("WATCOM")) map.add(env["WATCOM"], watcom);
    int other = 0;
    for (const auto& in : c.at("inputs")) {   // any other folder an input was in
        const std::string path = in.at("path").get<std::string>();
        if (map.place(path)) continue;
        map.add(slashes(fs::to_text(fs::from_text(path).parent_path())), root / "other" / std::to_string(other++));
    }

    // the inputs where the run had them (link scripts with their folders mapped)
    for (const auto& in : c.at("inputs")) {
        const std::string path = in.at("path").get<std::string>();
        const stdfs::path to = *map.place(path);
        if (fs::to_text(to).starts_with(fs::to_text(watcom))) continue;   // (OpenWatcom's own: there already)
        fs::make_dirs(to.parent_path());
        const Bytes bytes = fs::read_file(blobs / in.at("sha1").get<std::string>());
        if (is_text_input(in.at("as").get<std::string>())) {
            fs::write_text(to, map.forward(std::string(bytes.begin(), bytes.end())));
        } else {
            fs::write_file(to, bytes);
        }
    }
    std::vector<std::string> args;
    for (const auto& a : c.at("args")) args.push_back(map.forward(a.get<std::string>()));
    for (auto& [k, v] : env) v = map.forward(v);

    // a resource the scripts link in that the capture did not keep (captures before core/wlink_capture read `option
    // resource`): the case cannot be linked again, which is said, not counted as a difference
    for (const auto& in : c.at("inputs")) {
        if (!is_text_input(in.at("as").get<std::string>())) continue;
        const Bytes b = fs::read_file(blobs / in.at("sha1").get<std::string>());
        const std::string script = lower(std::string(b.begin(), b.end()));
        for (std::size_t at = 0; (at = script.find("resource=", at)) != std::string::npos;) {
            at += 9;
            const std::size_t end = script.find_first_of(" \t\r\n,", at);
            const std::string name = script.substr(at, end == std::string::npos ? std::string::npos : end - at);
            bool kept = false;
            for (const auto& held : c.at("inputs"))
                if (lower(fs::from_text(held.at("as").get<std::string>()).filename().string()) == lower(fs::from_text(name).filename().string())) kept = true;
            if (!kept) {
                o.skipped = true;
                o.why = "the capture lacks " + name + " (option resource): not replayable";
                return o;
            }
        }
    }

    const Run run = link(args, root / "cwd", env);

    std::vector<std::string> why;
    if (run.exit_code != c.at("exit").get<int>()) why.push_back(std::format("exit {} (wlink.exe: {})", run.exit_code, c["exit"].get<int>()));
    for (const auto& out : c.at("outputs")) {
        const std::string as = out.at("as").get<std::string>();
        const stdfs::path mine = (fs::from_text(map.forward(as)).is_absolute() ? fs::from_text(map.forward(as)) : root / "cwd" / fs::from_text(as));
        const stdfs::path theirs = case_dir / "out" / out.at("file").get<std::string>();
        if (!stdfs::is_regular_file(mine, ec)) {
            why.push_back(as + " not written");
        } else if (lower(as).ends_with(".ilk")) {
            // an incremental link's state file: wlink.exe's own runs differ in it (it keeps heap addresses and times), so it
            // is enough that it is written
        } else if (lower(as).ends_with(".map")) {   // a map names the folders: compared with them mapped back
            if (messages(map.backward(fs::read_text(mine))) != messages(fs::read_text(theirs))) why.push_back(as + " differs");
        } else if (without_link_time(fs::read_file(mine)) != without_link_time(fs::read_file(theirs))) {
            why.push_back(as + " differs");
        }
    }
    const std::string out_mine = messages(map.backward(run.out)), err_mine = messages(map.backward(run.err));
    // the messages, stdout then stderr: the 32-bit wlink.exe (binnt) writes its warnings on stdout, the 64-bit one on stderr
    if (out_mine + err_mine != messages(c.value("stdout", std::string())) + messages(c.value("stderr", std::string())))
        why.push_back("the messages differ");
    o.same = why.empty();
    o.why = text::join(why, "; ");
    if (!o.same) {   // (beside the case: what this run said, to compare with case.json's)
        fs::write_text(root / "x86link.stdout.txt", run.out);
        fs::write_text(root / "x86link.stderr.txt", run.err);
    }
    if (o.same) stdfs::remove_all(root, ec);   // (a case that differs is left to look at)
    return o;
}

std::vector<Outcome> all(const stdfs::path& corpus, const stdfs::path& scratch, const stdfs::path& watcom) {
    std::vector<Outcome> out;
    std::vector<stdfs::path> cases;
    for (const auto& e : stdfs::directory_iterator(corpus))
        if (e.is_directory() && stdfs::is_regular_file(e.path() / "case.json")) cases.push_back(e.path());
    std::sort(cases.begin(), cases.end());
    for (const stdfs::path& c : cases) {
        try {
            out.push_back(one(c, corpus / "blobs", scratch, watcom));
        } catch (const std::exception& e) {
            out.push_back({.id = c.filename().string(), .same = false, .why = std::string("could not replay: ") + e.what()});
        }
    }
    return out;
}

}  // namespace cb::build::x86link::replay
