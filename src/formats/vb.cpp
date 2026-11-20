// cb-unity: alone
#include "formats/vb.hpp"

#include <bit>
#include <format>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "formats/format_builtin.hpp"
#include "formats/probe.hpp"

namespace cb::formats::vb {

namespace {

constexpr std::string_view kReader = "vb";

bool size_ok(std::uint64_t size) { return size >= kMinSize && size <= kMaxSize && std::has_single_bit(size); }

// A slot holds code when its first halfword is not padding ($0000 or $FFFF) and its opcode is one the V810 defines (the formats know no instruction set; the three
// undefined opcodes are $1B, $32 and $36).
bool holds_code(ByteView slot) {
    if (slot.size() < 2) return false;
    const unsigned hw = slot[0] | slot[1] << 8;
    const unsigned opcode = hw >> 10;
    return hw != 0x0000 && hw != 0xFFFF && opcode != 0x1B && opcode != 0x32 && opcode != 0x36;
}

std::string text_of(std::string s) {
    while (!s.empty() && (s.back() == ' ' || s.back() == '\0')) s.pop_back();
    return s;
}

}  // namespace

const std::vector<VectorSlot>& vector_slots() {
    static const std::vector<VectorSlot> slots = {{0x200, "gamepad"}, {0x1F0, "timer"},       {0x1E0, "game_pak"},       {0x1D0, "comm"},          {0x1C0, "vip"},
                                                  {0xA0, "float"},    {0x90, "divide_by_zero"}, {0x80, "invalid_opcode"}, {0x60, "trap_0"},         {0x50, "trap_16"},
                                                  {0x40, "address_trap"}, {0x30, "duplexed_exception"}, {0x10, "reset"}};
    return slots;
}

std::vector<Rom::Entry> Rom::entries() const {
    std::vector<Entry> out;
    for (const VectorSlot& v : vector_slots()) {
        const std::size_t offset = data.size() - v.from_end;
        const ByteView slot = ByteView(data).subspan(offset, 16);
        if (holds_code(slot)) out.push_back({offset, v.name});
    }
    return out;
}

Rom parse(ByteView d) {
    const engine::Tree tree = engine::read_builtin(kReader, d);   // config/formats/vb.fmt
    const engine::Node& h = tree.root.get("header");
    Rom r;
    r.data.assign(d.begin(), d.end());
    r.header.title = text_of(h.get("title").text);
    r.header.maker = text_of(h.get("maker").text);
    r.header.game_code = text_of(h.get("game_code").text);
    r.header.version = static_cast<std::uint8_t>(h.u("version"));
    return r;
}

Rom read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view());
}

bool probe(const std::filesystem::path& path) {
    if (!has_extension(path, {".vb", ".vboy"})) return false;
    const std::optional<std::uint64_t> size = size_of(path);
    if (!size || !size_ok(*size)) return false;
    const std::optional<Bytes> slot = read_prefix(path, *size - 0x10, 16);
    return slot && slot->size() == 16 && holds_code(*slot);
}

}  // namespace cb::formats::vb
