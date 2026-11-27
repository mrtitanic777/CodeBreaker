// The `ppc-calls` command and the reading of a file into the Module the call analysis runs on (ppc_calls.hpp).
#include <algorithm>
#include <filesystem>

#include "analyze/ppc_calls.hpp"
#include "core/fs.hpp"
#include "core/log.hpp"
#include "decompile/vmx128.hpp"
#include "formats/powerpc_image.hpp"
#include "recompile/disassemble.hpp"
#include "recompile/discover.hpp"

namespace cb::analyze::ppc_calls {

namespace {

namespace pi = formats::powerpc_image;
namespace rc = recompile;

// What discovery reads of an image.
rc::discover::Image discovery_image(const pi::Image& image) {
    rc::discover::Image out;
    out.code_begin = image.code_begin();
    out.code_end = image.code_end();
    out.entry = image.entry();
    const ByteView code = image.bytes_at(out.code_begin, out.code_end - out.code_begin);
    out.code.assign(code.begin(), code.end());
    for (const pi::Section& section : image.sections()) {
        const ByteView bytes = image.bytes_at(section.va, section.size);
        out.sections.push_back({section.name, section.va, section.characteristics, Bytes(bytes.begin(), bytes.end())});
    }
    for (const auto& [start, extent] : image.extents()) out.extents[start] = extent.size;
    for (const auto& [address, name] : image.imports()) out.imports.insert(address);
    return out;
}

// The Xenon's vector words first: Capstone reads them as other, later instructions.
std::optional<std::pair<std::string, std::string>> ppc_calls_vector(std::uint32_t word) {
    if (const auto decoded = decompile::vmx128::decode(word)) return std::make_pair(decoded->mnemonic, decoded->operands);
    return std::nullopt;
}

}  // namespace

Module module_of(const std::filesystem::path& file, const Environment& environment) {
    const bool is_xex = formats::xex::probe(file);
    const pi::Image image = is_xex ? pi::Image::read_xex(environment.tools, file) : pi::Image::read(file);
    rc::disassemble::Disassembler disassembler(ppc_calls_vector);
    const rc::discover::Discovery found = rc::discover::discover(discovery_image(image), disassembler);

    Module module;
    const std::uint32_t lo = image.code_begin(), hi = image.code_end();
    std::vector<std::uint32_t> starts;
    for (const auto& [address, size] : found.functions) {
        if (address >= lo && address < hi && !image.imports().contains(address) && !image.run_names().contains(address)) starts.push_back(address);
    }
    const ByteView code = image.bytes_at(lo, hi - lo);
    for (std::size_t i = 0; i < starts.size(); ++i) {
        const std::uint32_t start = starts[i];
        const std::uint32_t size = found.functions.at(start);
        std::uint32_t end = size != 0 ? start + size : i + 1 < starts.size() ? starts[i + 1] : hi;
        if (i + 1 < starts.size()) end = std::min(end, starts[i + 1]);
        end = std::min(end, hi);
        Body body;
        for (const rc::disassemble::Row& row : disassembler.listing(code, lo, start - lo, (end - start) / 4))
            body.push_back({row.address, row.mnemonic, split_operands(row.operands)});
        module.functions[start] = std::move(body);
    }

    if (is_xex) {
        const Bytes data = fs::read_file(file);
        for (const formats::xex_imports::Import& import : formats::xex_imports::imports(data, image, environment.ordinal_name))
            if (import.thunk && *import.thunk != 0) module.import_names[*import.thunk] = import.name;
    }
    for (const auto& [address, name] : image.imports()) module.import_names.emplace(address, name);
    for (const auto& [address, name] : image.run_names()) module.transparent.insert(address);
    return module;
}

cli::Command command(const Environment& environment) {
    cli::Command c;
    c.name = "ppc-calls";
    c.summary = "what a PowerPC program's calls show: argument counts, registers kept, variadic functions";
    c.area = "Xbox 360";
    c.params = {cli::Positional{"module", "an .xex or a PowerPC image"},
                cli::Option{"--output", "-o", "write the findings as tab-separated lines to this file", cli::Arg::Value, "FILE"}};
    c.handler = [environment](const cli::Args& a) {
        const Result result = analyse(module_of(fs::from_text(a.required("module")), environment));
        out("{}\n", summary(result));
        if (const std::optional<std::string> target = a.value("output")) {
            fs::write_text(fs::from_text(*target), to_tsv(result));
            out("written: {}\n", *target);
        }
        return kExitOk;
    };
    return c;
}

}  // namespace cb::analyze::ppc_calls
