// cb/source/flat_rounds.cpp -- the NASM side of the whole-file x86 writers (flat_rounds.hpp).

#include "cb/source/flat_rounds.hpp"

#include <format>

#include "cb/build/x86asm/x86asm.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/process.hpp"
#include "cb/core/text.hpp"

namespace cb::source::flat_rounds {

namespace {

bool upper_hex(std::string_view s) {
    return !s.empty() && std::ranges::all_of(s, [](char c) { return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F'); });
}

}  // namespace

Bytes read_all(const std::filesystem::path& path) { return fs::read_file(path); }

std::string tail(std::string_view said, std::size_t n) { return std::string(said.size() > n ? said.substr(said.size() - n) : said); }

Assembled assemble(std::string_view source, const std::filesystem::path& work, std::string_view name) {
    Assembled out;
    const std::filesystem::path asm_file = work / (std::string(name) + ".asm");
    const std::filesystem::path image = work / std::string(name);
    out.map = work / (std::string(name) + ".map");
    fs::make_dirs(work);
    const auto text = text::to_latin1("[map symbols " + fs::to_text(out.map) + "]\n" + std::string(source));
    fs::write_file(asm_file, text);   // (the source is kept: the writers hand it over)
    std::filesystem::remove(image);
    std::filesystem::remove(out.map);
    // x86asm, CodeBreaker's own assembler (build/x86asm), in memory: the image and the map come back as data. A file the
    // source pulls in by name (`incbin` of a stub) is read from the disk.
    build::x86asm::Request request;
    request.cwd = work.generic_string();
    request.args = {"-f", "bin", "-w-all", "-o", fs::to_text(image), fs::to_text(asm_file)};
    request.files[asm_file.generic_string()] = Bytes(text.begin(), text.end());
    request.read = [](const std::string& path) -> std::optional<Bytes> {
        std::error_code ec;
        if (!std::filesystem::is_regular_file(fs::from_text(path), ec)) return std::nullopt;
        return fs::read_file(fs::from_text(path));
    };
    const build::x86asm::Result r = build::x86asm::assemble(request);
    for (const auto& [file, bytes] : r.files) fs::write_file(fs::from_text(file), bytes);
    out.said = r.messages;
    if (r.exit == 0 && std::filesystem::is_regular_file(image)) out.file = image;
    return out;
}

LabelMap read_map(const std::filesystem::path& path) {
    LabelMap labels;
    const std::string content = fs::read_text(path);   // (the lines are views into it: it must outlive the loop)
    for (const std::string_view line : text::lines(content)) {
        const std::vector<std::string_view> parts = text::split_ws(line);
        if (parts.size() != 3 || !upper_hex(parts[0]) || !upper_hex(parts[1])) continue;
        const auto place = text::parse_int<std::uint64_t>(parts[0], 16);
        const auto address = text::parse_int<std::uint64_t>(parts[1], 16);
        if (place && address) labels[std::string(parts[2])] = Label{*address, *place};
    }
    return labels;
}

std::vector<std::size_t> error_lines(std::string_view said) {
    std::vector<std::size_t> out;
    for (std::size_t i = 0; i < said.size(); ++i) {
        if (said[i] != ':') continue;
        std::size_t j = i + 1;
        while (j < said.size() && said[j] >= '0' && said[j] <= '9') ++j;
        if (j == i + 1 || !said.substr(j).starts_with(": error")) continue;
        if (const auto n = text::parse_int<std::size_t>(said.substr(i + 1, j - i - 1))) out.push_back(*n);
        i = j - 1;
    }
    return out;
}

ReportText report(const std::filesystem::path& program, const FlatReport& r) {
    const std::size_t n = std::max<std::size_t>(1, r.instructions);
    ReportText out;
    out.text = std::format("{}: {} instructions, {} ({}%) written as bytes; {} round{}; {}\n  source: {}\n", program.filename().string(), r.instructions,
                           r.raw, 100 * r.raw / n, r.rounds, r.rounds == 1 ? "" : "s",
                           r.same ? "builds back THE SAME" : r.built ? "built, differs" : "not built", fs::to_text(r.source));
    out.exit_code = r.same ? 0 : 1;
    return out;
}

}  // namespace cb::source::flat_rounds
