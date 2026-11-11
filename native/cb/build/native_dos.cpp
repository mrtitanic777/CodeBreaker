#include "cb/build/native_dos.hpp"

#include <algorithm>
#include <format>
#include <variant>

#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"

namespace cb::build::native_dos {

namespace {

namespace stdfs = std::filesystem;

constexpr std::string_view kMasmExe = "MASM.EXE";

// Whatever the executive throws, the run did not complete: the caller falls back to DOSBox.
template <class Run> auto guarded(Run&& run) {
    try {
        return std::forward<Run>(run)();
    } catch (const NativeDosError&) {
        throw;
    } catch (const Error& e) { // an isolation boundary: a service the executive lacks, a CPU fault, a limit
        throw NativeDosError(std::format("{}: {}", e.kind(), e.what()));
    }
}

// Text as a DOS text-mode write leaves it: Latin-1 bytes, CRLF line ends (one newline convention in, clean CRLF out).
Bytes dos_text(std::string_view s) {
    return text::to_latin1(text::replace_all(text::replace_all(s, "\r\n", "\n"), "\n", "\r\n"));
}

// <work>/<stem><ext>, with the sub-folders the stem names made. `stem` is relative and never leaves `work`.
stdfs::path stem_path(const stdfs::path& work, std::string_view stem, std::string_view ext) {
    const std::string slashed = text::replace_all(stem, "\\", "/"); // (the parts are views into it)
    const std::vector<std::string_view> parts = text::split(slashed, '/');
    for (const std::string_view part : parts) {
        if (part.empty() || part == "." || part == "..")
            throw UsageError(std::format("'{}' is not a DOS file stem", stem));
    }
    stdfs::path folder = work;
    for (std::size_t i = 0; i + 1 < parts.size(); ++i)
        folder /= std::string(parts[i]);
    fs::make_dirs(folder);
    return folder / (std::string(parts.back()) + std::string(ext));
}

// MASM.EXE copied into `work`.
stdfs::path install_masm(const stdfs::path& work, const stdfs::path& masm_root) {
    const stdfs::path exe = work / std::string(kMasmExe);
    const stdfs::path source = masm_root / "BIN" / std::string(kMasmExe);
    std::error_code ec;
    if (!stdfs::is_regular_file(source, ec)) {
        throw NativeDosError(std::format("MASM.EXE not found under {}", fs::to_text(masm_root / "BIN")));
    }
    stdfs::copy_file(source, exe, stdfs::copy_options::overwrite_existing, ec);
    if (ec)
        throw IoError(exe, ec.value(), "cannot copy MASM.EXE");
    return exe;
}

// MASM's command line: the source with its sub-path if the stem has one, the outputs the same, no cross-reference.
std::string masm_command(std::string_view stem) {
    const std::string name = text::replace_all(stem, "/", "\\");
    return std::format("{0}.ASM,{0}.OBJ,{0}.LST,NUL;", name);
}

std::optional<Bytes> read_if_file(const stdfs::path& p) {
    std::error_code ec;
    if (!stdfs::is_regular_file(p, ec))
        return std::nullopt;
    return fs::read_file(p);
}

// What one MASM run left in `work`; the log falls back to what MASM printed.
dos_sessions::Product collect(const stdfs::path& work, std::string_view stem, const Bytes& console) {
    dos_sessions::Product product;
    product.obj = read_if_file(stem_path(work, stem, ".OBJ"));
    if (const auto lst = read_if_file(stem_path(work, stem, ".LST")))
        product.lst = text::from_latin1(*lst);
    if (const auto log = read_if_file(stem_path(work, stem, ".LOG"))) {
        product.log = text::from_latin1(*log);
    } else {
        product.log = text::from_latin1(console);
    }
    return product;
}

dos_sessions::Product run_masm(const DosExecutive& dos, const stdfs::path& work, const stdfs::path& exe,
                               std::string_view stem, std::string_view source) {
    fs::write_file(stem_path(work, stem, ".ASM"), dos_text(source));
    const Exit r = guarded([&] { return dos.run(exe, work, masm_command(stem)); });
    return collect(work, stem, r.console);
}

} // namespace

Mode mode_from_environment() {
    const std::string value = text::lower(fs::get_env("CB_NATIVE_DOS").value_or("auto"));
    if (value == "0" || value == "off" || value == "no" || value == "false")
        return Mode::Off;
    if (value == "1" || value == "on" || value == "yes" || value == "true")
        return Mode::On;
    return Mode::Auto; // "auto", and anything not understood
}

bool is_proven(std::string_view tool) {
    return tool == "masm" || tool == "ml";
}

bool enabled(const DosExecutive& dos, std::string_view tool, Mode mode) {
    if (mode == Mode::Off || !dos.available())
        return false;
    return mode == Mode::On || is_proven(tool);
}

dos_sessions::Product assemble(const DosExecutive& dos, std::string_view source, std::string_view stem,
                               const stdfs::path& masm_root) {
    fs::ScopedTempDir work("masm_nat");
    const stdfs::path exe = install_masm(work.path(), masm_root);
    return run_masm(dos, work.path(), exe, stem, source);
}

dos_sessions::Finished assemble_many(const DosExecutive& dos, std::span<const dos_sessions::Source> items,
                                     const stdfs::path& masm_root) {
    fs::ScopedTempDir work("masm_nat");
    const stdfs::path exe = install_masm(work.path(), masm_root);
    dos_sessions::Finished done;
    for (const dos_sessions::Source& item : items) {
        done.results.emplace_back(item.stem, run_masm(dos, work.path(), exe, item.stem, item.text));
    }
    return done; // (`work` is gone: everything the runs made is in the results)
}

const Bytes& default_extended_env() {
    static const Bytes env = to_bytes(std::string_view("PATH=C:\\\0", 9));
    return env;
}

MemoryResult run_extended(const DosExecutive& dos, const std::map<std::string, stdfs::path>& tool_files,
                          std::string_view exe, std::string_view cmdline, const std::map<std::string, DosFile>& files,
                          std::span<const std::string> artifacts, const Bytes& env) {
    MemoryRun run;
    for (const auto& [name, path] : tool_files)
        run.files[text::upper(name)] = fs::read_file(path);
    for (const auto& [name, content] : files) {
        run.files[text::upper(name)] = std::holds_alternative<std::string>(content)
                                           ? dos_text(std::get<std::string>(content))
                                           : std::get<Bytes>(content);
    }
    run.exe = text::upper(exe);
    run.cmdline = std::string(cmdline);
    run.env = env;
    for (const std::string& name : artifacts)
        run.artifacts.push_back(name);
    MemoryResult result = guarded([&] { return dos.run_dpmi(run); });
    return result;
}

std::map<std::string, stdfs::path> ml_files(const MlToolchain& toolchain) {
    std::map<std::string, stdfs::path> out{{"ML.EXE", toolchain.ml_dir / "ML.EXE"},
                                           {"ML.ERR", toolchain.ml_dir / "ML.ERR"},
                                           {"DOSXNT.EXE", toolchain.msvc_root / "BIN" / "DOSXNT.EXE"}};
    std::vector<std::string> missing;
    for (const auto& [name, path] : out) {
        std::error_code ec;
        if (!stdfs::is_regular_file(path, ec))
            missing.push_back(fs::to_text(path));
    }
    if (!missing.empty())
        throw NativeDosError("TNT ML toolchain not found: " + text::join(missing, ", "));
    return out;
}

MlResult ml(const DosExecutive& dos, const MlToolchain& toolchain, std::string_view args,
            const std::map<std::string, DosFile>& files, std::span<const std::string> artifacts) {
    MemoryResult r = run_extended(dos, ml_files(toolchain), "ML.EXE", args, files, artifacts);
    return {std::move(r.made), text::from_latin1(r.console)};
}

} // namespace cb::build::native_dos
