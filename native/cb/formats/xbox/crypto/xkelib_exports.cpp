#include "cb/formats/xbox/crypto/xkelib_exports.hpp"

#include <algorithm>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/process.hpp"
#include "cb/core/regex.hpp"

namespace cb::formats::xbox::crypto::xkelib_exports {

namespace {

std::string dumpbin_output(const std::filesystem::path& dumpbin, std::string_view option, const std::filesystem::path& library) {
    const process::ProcessResult result =
        process::run_checked({.argv = {fs::to_text(dumpbin), std::string("/") + std::string(option), fs::to_text(std::filesystem::absolute(library))}});
    return std::string(as_text(result.out));
}

}  // namespace

std::string library_name(std::string_view headers) {
    static const Regex pattern(R"(DLL\sname\s*:\s*([\w\W]*?)\r\n)");
    if (const auto match = pattern.search(headers)) return std::string(match->group(1));
    return {};
}

std::vector<Export> parse_exports(std::string_view exports) {
    static const Regex pattern(R"(\s+(\d+)\s+([\w\W]*?)\r\n)");
    std::vector<Export> out;
    pattern.for_each_match(exports, [&out](const RegexMatch& match) {
        const std::string_view name = match.group(2);
        if (!name.starts_with(".")) {
            if (const auto ordinal = text::parse_int<int>(match.group(1))) out.push_back({*ordinal, std::string(name)});
        }
        return true;
    });
    std::stable_sort(out.begin(), out.end(), [](const Export& a, const Export& b) { return a.ordinal < b.ordinal; });
    return out;
}

Library read(const std::filesystem::path& dumpbin, const std::filesystem::path& library) {
    if (!std::filesystem::is_regular_file(library)) throw NotFoundError(library, 0, "the input file doesn't exist");
    Library out;
    out.name = library_name(dumpbin_output(dumpbin, "HEADERS", library));
    if (out.name.empty()) throw FormatError("xkelib_exports", 0, "error locating the library name");
    out.exports = parse_exports(dumpbin_output(dumpbin, "EXPORTS", library));
    return out;
}

std::string def_text(const Library& library) {
    std::string out = std::format("LIBRARY {}\r\nEXPORTS\r\n", library.name);
    for (const Export& e : library.exports) out += std::format("\t{} @{}\r\n", e.name, e.ordinal);
    return out;
}

cli::Command command(const std::filesystem::path& sdk_bin_dir) {
    return cli::Command{
        "xkelib_exports",
        "make a .def file from an Xbox 360 library's exports, with the SDK's dumpbin",
        {cli::Positional{"library", "the library to dump the exports of"}, cli::Positional{"output", "the .def file to write"},
         cli::Option{"--verbose", "-v", "print the definitions as they are written", cli::Arg::Flag, "", ""}},
        [sdk_bin_dir](const cli::Args& args) {
            const std::filesystem::path dumpbin = sdk_bin_dir / "dumpbin.exe";
            if (!std::filesystem::is_regular_file(dumpbin)) throw NotFoundError(dumpbin, 0, "the Xbox 360 SDK doesn't appear to be installed (setting xbox360_sdk)");
            const Library library = read(dumpbin, fs::from_text(args.required("library")));
            fs::write_text(fs::from_text(args.required("output")), def_text(library));
            if (args.flag("verbose")) {
                cb::out("LIBRARY {}\nEXPORTS\n", library.name);
                for (const Export& e : library.exports) cb::out("\t{} @{}\n", e.name, e.ordinal);
            }
            return 0;
        },
        {},
        "",
        ""};
}

}  // namespace cb::formats::xbox::crypto::xkelib_exports
