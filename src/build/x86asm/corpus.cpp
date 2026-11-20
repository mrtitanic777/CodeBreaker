#include "build/x86asm/corpus.hpp"

#include <algorithm>
#include <format>

#include <nlohmann/json.hpp>

#include "core/error.hpp"
#include "core/fs.hpp"

namespace cb::build::x86asm::corpus {

namespace stdfs = std::filesystem;

Case load(const stdfs::path& folder) {
    const stdfs::path json_path = folder / "case.json";
    if (!stdfs::is_regular_file(json_path)) throw FormatError(fs::to_text(folder), 0, "not a captured NASM case (no case.json)");
    const nlohmann::json j = nlohmann::json::parse(fs::read_text(json_path));
    Case c;
    c.id = folder.filename().string();
    c.cwd = stdfs::path(j.at("cwd").get<std::string>());
    c.request.cwd = c.cwd.generic_string();
    for (const auto& a : j.at("args")) c.request.args.push_back(a.get<std::string>());
    if (j.contains("files"))
        for (const auto& f : j.at("files")) c.request.files[f.at("path").get<std::string>()] = fs::read_file(folder / "in" / f.at("file").get<std::string>());
    c.exit = j.at("exit").get<int>();
    c.messages = j.value("stderr", std::string()) + j.value("stdout", std::string());
    for (const auto& o : j.at("outputs")) c.outputs.emplace_back(o.at("as").get<std::string>(), fs::read_file(folder / "out" / o.at("file").get<std::string>()));
    return c;
}

std::vector<std::string> compare(const Case& c, const Result& made) {
    std::vector<std::string> out;
    if (made.exit != c.exit) out.push_back(std::format("exit {} (the reference: {})", made.exit, c.exit));
    for (const auto& [name, want] : c.outputs) {
        const auto got = made.files.find(name);
        if (got == made.files.end()) {
            out.push_back(name + ": not written");
            continue;
        }
        if (got->second == want) continue;
        // A COFF object's header holds the time it was made (bytes 4-7): the one field a new run cannot give back. It is
        // the only thing set aside, and only in a COFF object (machine i386 or AMD64 in its first two bytes).
        const auto coff = [](const Bytes& b) {
            return b.size() >= 20 && ((b[0] == 0x4C && b[1] == 0x01) || (b[0] == 0x64 && b[1] == 0x86));
        };
        if (coff(want) && coff(got->second) && want.size() == got->second.size()) {
            Bytes masked = got->second;
            std::copy(want.begin() + 4, want.begin() + 8, masked.begin() + 4);
            if (masked == want) continue;
        }
        const auto [a, b] = std::mismatch(got->second.begin(), got->second.end(), want.begin(), want.end());
        out.push_back(std::format("{}: {} bytes (the reference: {}), first difference at {:#x}", name, got->second.size(), want.size(),
                                  static_cast<std::size_t>(a - got->second.begin())));
    }
    if (made.messages != c.messages) out.push_back("messages differ: \"" + made.messages.substr(0, 160) + "\" (the reference: \"" + c.messages.substr(0, 160) + "\")");
    return out;
}

std::vector<stdfs::path> cases(const stdfs::path& root) {
    std::vector<stdfs::path> out;
    std::error_code ec;
    for (const auto& e : stdfs::directory_iterator(root, ec))
        if (e.is_directory(ec) && stdfs::is_regular_file(e.path() / "case.json", ec)) out.push_back(e.path());
    std::sort(out.begin(), out.end());
    return out;
}

}  // namespace cb::build::x86asm::corpus
