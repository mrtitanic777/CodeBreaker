#include "cb/build/msvc152.hpp"

#include <algorithm>
#include <cctype>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"

namespace cb::build::msvc152 {

namespace {

bool is_dos_stem(std::string_view s) {
    return !s.empty() && s.size() <= 8 &&
           std::all_of(s.begin(), s.end(), [](char c) { return std::isalnum(static_cast<unsigned char>(c)) || c == '_'; });
}

// Characters that would turn a switch into a second command or a redirection in the batch file.
bool is_plain_switch_text(std::string_view s) {
    return s.find_first_of("&|<>^%\"\r\n") == std::string_view::npos;
}

}  // namespace

Compiled compile_c(const DosRunner& runner, const Tools& tools, std::string_view source, std::string_view flags,
                   std::string_view stem_in, std::chrono::seconds timeout) {
    if (!is_dos_stem(stem_in)) throw UsageError(std::format("'{}' is not a DOS file stem (1-8 letters, digits, underscores)", stem_in));
    if (!is_plain_switch_text(flags)) throw UsageError(std::format("compiler flags '{}' hold a character DOS reads as a command", flags));
    std::error_code ec;
    if (!std::filesystem::exists(tools.dosbox_x, ec)) throw NotFoundError(tools.dosbox_x, 2, "dosbox-x not found");
    const std::filesystem::path cl = tools.msvc_root / "BIN" / "CL.EXE";
    if (!std::filesystem::exists(cl, ec)) throw NotFoundError(cl, 2, "MSVC 1.52c CL.EXE not found");

    const std::string stem(stem_in);
    DosRunSpec spec;
    spec.mounts = {{"c", tools.msvc_root}};
    // /Fc emits a mixed source+machine-code listing; /Fo the obj.
    spec.commands = {std::format("CL {} /Fc{}.COD /Fo{}.OBJ {}.C > {}.LOG 2>&1", flags, stem, stem, stem, stem)};
    spec.artifacts = {stem + ".OBJ", stem + ".COD", stem + ".LOG"};
    spec.files[stem + ".C"] = std::string(source);
    spec.timeout = timeout;
    spec.cpu.insert_or_assign("cputype", "386");
    spec.cpu.insert_or_assign("core", "normal");
    spec.cpu.insert_or_assign("cycles", "max");
    spec.dos.insert_or_assign("ver", "6.22");
    spec.variables.insert_or_assign("PATH", "C:\\BIN");
    spec.variables.insert_or_assign("INCLUDE", "C:\\INCLUDE");
    spec.variables.insert_or_assign("LIB", "C:\\LIB");
    spec.variables.insert_or_assign("TMP", "D:\\");

    const DosRunResult ran = runner.run(spec);
    Compiled out;
    out.work = ran.work;
    if (const auto it = ran.made.find(stem + ".OBJ"); it != ran.made.end()) out.obj = it->second;
    if (const auto it = ran.made.find(stem + ".COD"); it != ran.made.end()) out.cod = text::from_latin1(it->second);
    if (const auto it = ran.made.find(stem + ".LOG"); it != ran.made.end()) out.log = text::from_latin1(it->second);
    return out;
}

std::string report(const Compiled& c) {
    std::string out;
    if (c.cod) {
        out += "=== /Fc listing (codegen) ===\n";
        out += c.cod->substr(0, 3000) + "\n";
    }
    if (c.obj) {
        out += std::format("=== OBJ: {} bytes ===\n", c.obj->size());
    } else {
        out += "=== NO OBJ produced -- compiler log: ===\n";
        out += (c.log ? c.log->substr(0, 2000) : std::string("(no log)")) + "\n";
    }
    out += std::format("# work dir: {}\n", c.work ? fs::to_text(*c.work) : std::string("none"));
    return out;
}

}  // namespace cb::build::msvc152
