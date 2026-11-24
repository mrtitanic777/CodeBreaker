// build/x86cc/replay.cpp -- see replay.hpp.
#include "build/x86cc/replay.hpp"

#include <algorithm>
#include <format>
#include <map>

#include <nlohmann/json.hpp>

#include "build/x86cc/x86cc.hpp"
#include "build/x86link/replay_paths.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"

namespace cb::build::x86cc::replay {

namespace stdfs = std::filesystem;
using replay_paths::lower;
using replay_paths::PathMap;
using replay_paths::slashes;

namespace {

std::string messages(const std::string& text) { return text::replace_all(text, "\r\n", "\n"); }

// An OMF object as compared: the names of the files it was made from mapped back to the recorded run's folders (the
// module's name, THEADR, and each dependency record, COMENT class 0xE9, written for the source and every header it read:
// the linker's make-style check), and the times those records hold cleared (a replay's copies of the files do not keep
// them). Each record so changed is made again with its length and a zero checksum; every other byte is compared as it is.
Bytes as_compared(const Bytes& obj, const PathMap& map) {
    Bytes out;
    std::size_t at = 0;
    auto put = [&](std::uint8_t type, const Bytes& data) {
        out.push_back(type);
        const std::size_t len = data.size() + 1;
        out.push_back(static_cast<std::uint8_t>(len));
        out.push_back(static_cast<std::uint8_t>(len >> 8));
        out.insert(out.end(), data.begin(), data.end());
        out.push_back(0);
    };
    // data[from] is a name's length, its text after: the name mapped back
    auto renamed = [&](Bytes data, std::size_t from) {
        if (from >= data.size() || from + 1 + data[from] > data.size()) return data;
        const std::string name(data.begin() + from + 1, data.begin() + from + 1 + data[from]);
        const std::string back = map.backward(name);
        Bytes made(data.begin(), data.begin() + from);
        made.push_back(static_cast<std::uint8_t>(std::min<std::size_t>(back.size(), 255)));
        made.insert(made.end(), back.begin(), back.begin() + std::min<std::size_t>(back.size(), 255));
        made.insert(made.end(), data.begin() + from + 1 + data[from], data.end());
        return made;
    };
    while (at + 3 <= obj.size()) {
        const std::uint8_t type = obj[at];
        const std::size_t len = obj[at + 1] | obj[at + 2] << 8;
        if (len == 0 || at + 3 + len > obj.size()) break;   // (not OMF, or its end: the rest compared as it is)
        Bytes data(obj.begin() + at + 3, obj.begin() + at + 3 + len - 1);   // (less the checksum)
        if (type == 0x80 || type == 0x82) {
            put(type, renamed(data, 0));
        } else if (type == 0x88 && data.size() >= 2 + 4 && data[1] == 0xE9) {
            for (std::size_t i = 2; i < 6; ++i) data[i] = 0;
            put(type, data.size() > 6 ? renamed(data, 6) : data);
        } else {
            out.insert(out.end(), obj.begin() + at, obj.begin() + at + 3 + len);
        }
        at += 3 + len;
    }
    out.insert(out.end(), obj.begin() + at, obj.end());
    return out;
}

}  // namespace

Outcome one(const stdfs::path& case_dir, const stdfs::path& blobs, const stdfs::path& scratch, const stdfs::path& watcom) {
    Outcome o;
    o.id = case_dir.filename().string();
    const nlohmann::json c = nlohmann::json::parse(fs::read_text(case_dir / "case.json"));
    const std::string program = c.value("program", std::string());
    if (program != "wcc" && program != "wcc386") throw Error("x86cc replay: " + o.id + " is not a compile (" + program + ")");
    const stdfs::path root = stdfs::absolute(scratch).lexically_normal() / o.id;   // (absolute: the compile runs inside the case's folder)
    std::error_code ec;
    stdfs::remove_all(root, ec);
    fs::make_dirs(root / "cwd");

    PathMap map;
    map.add(c.at("cwd").get<std::string>(), root / "cwd");
    std::map<std::string, std::string> env;
    if (c.contains("env"))
        for (const auto& [k, v] : c["env"].items()) env[k] = v.get<std::string>();
    // the run's OpenWatcom is this one: its headers are read from it, not from copies (the captures hold the run's own)
    if (env.contains("WATCOM")) map.add(env["WATCOM"], watcom);
    int other = 0;
    for (const auto& in : c.at("inputs")) {
        const std::string path = in.at("path").get<std::string>();
        if (map.place(path)) continue;
        map.add(slashes(fs::to_text(fs::from_text(path).parent_path())), root / "other" / std::to_string(other++));
    }
    for (const auto& in : c.at("inputs")) {
        const stdfs::path to = *map.place(in.at("path").get<std::string>());
        if (fs::to_text(to).starts_with(fs::to_text(watcom))) continue;   // (OpenWatcom's own: there already)
        fs::make_dirs(to.parent_path());
        fs::write_file(to, fs::read_file(blobs / in.at("sha1").get<std::string>()));
    }
    std::vector<std::string> args;
    for (const auto& a : c.at("args")) args.push_back(map.forward(a.get<std::string>()));
    for (auto& [k, v] : env) v = map.forward(v);

    const Run run = compile(program == "wcc" ? Target::i86 : Target::i386, args, root / "cwd", env);

    std::vector<std::string> why;
    if (run.exit_code != c.at("exit").get<int>())
        why.push_back(std::format("exit {} ({}.exe: {})", run.exit_code, program, c["exit"].get<int>()));
    for (const auto& out : c.at("outputs")) {
        const std::string as = map.forward(out.at("as").get<std::string>());
        const stdfs::path mine = fs::from_text(as).is_absolute() ? fs::from_text(as) : root / "cwd" / fs::from_text(as);
        const stdfs::path theirs = case_dir / "out" / out.at("file").get<std::string>();
        if (!stdfs::is_regular_file(mine, ec)) {
            why.push_back(out.at("as").get<std::string>() + " not written");
        } else if (lower(as).ends_with(".err")) {   // an error file names the folders: compared with them mapped back
            if (messages(map.backward(fs::read_text(mine))) != messages(fs::read_text(theirs))) why.push_back(as + " differs");
        } else if (as_compared(fs::read_file(mine), map) != as_compared(fs::read_file(theirs), PathMap{})) {
            why.push_back(out.at("as").get<std::string>() + " differs");
        }
    }
    if (messages(map.backward(run.out)) != messages(c.value("stdout", std::string()))) why.push_back("stdout differs");
    if (messages(map.backward(run.err)) != messages(c.value("stderr", std::string()))) why.push_back("stderr differs");
    o.same = why.empty();
    o.why = text::join(why, "; ");
    if (!o.same) {   // (beside the case: what this run said, to compare with case.json's)
        fs::write_text(root / "x86cc.stdout.txt", run.out);
        fs::write_text(root / "x86cc.stderr.txt", run.err);
    } else {
        stdfs::remove_all(root, ec);   // (a case that differs is left to look at)
    }
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

}  // namespace cb::build::x86cc::replay
