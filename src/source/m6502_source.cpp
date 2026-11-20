// source/m6502_source.cpp -- see m6502_source.hpp.

#include "source/m6502_source.hpp"

#include "source/processors.hpp"

namespace cb::source::m6502_source {
namespace {

const processors::Processor& processor(Cpu cpu) {
    static const processors::Processor* const table[] = {&processors::find("6502"), &processors::find("65c02"), &processors::find("huc6280"),
                                                         &processors::find("65816")};
    return *table[static_cast<std::size_t>(cpu)];
}

}  // namespace

std::optional<std::string> described(Cpu cpu, ByteView bytes, std::uint32_t pc, const Context& context) {
    return described_source::checked_text(processor(cpu), bytes, pc, bytes.size(), context);
}

std::string code(Cpu cpu, ByteView bytes, std::uint32_t pc, const decompile::AddressLabels& names, const Context& context) {
    if (const auto text = described(cpu, bytes, pc, context)) return described_source::name_numbers(*text, names);
    return described_source::byte_line(processor(cpu), bytes, pc, context, "not in the description");
}

Context widths(bool m16, bool x16) { return {{"m16", m16 ? 1 : 0}, {"x16", x16 ? 1 : 0}}; }

std::vector<std::string> data_lines(ByteView data) { return described_source::data_lines(data); }

std::string equ(const std::string& name, std::uint32_t value, unsigned digits) { return described_source::equ(name, value, digits); }

Bytes assemble(Cpu cpu, std::string_view source, const std::filesystem::path& folder) { return described_source::assemble(processor(cpu), source, folder); }

}  // namespace cb::source::m6502_source
