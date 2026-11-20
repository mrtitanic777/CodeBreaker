// cb-unity: alone (file-local helpers named for this module)
// Bodies: the digest that tells one function's bytes from another's wherever the linker put it.
#include <algorithm>
#include <format>
#include <set>

#include "core/crypto.hpp"
#include "core/io.hpp"
#include "core/text.hpp"
#include "decompile/describe.hpp"
#include "symbols/names.hpp"
#include "symbols/x86.hpp"

namespace cb::symbols::names {

namespace pef = formats::pe_functions;

namespace {

// The instructions a function ends at: the last of these is where its body stops (what follows is padding).
bool ends_a_function(std::string_view mnemonic) {
    return mnemonic == "ret" || mnemonic == "retf" || mnemonic == "retn" || mnemonic == "jmp" || mnemonic == "ljmp" || mnemonic == "iret" ||
           mnemonic == "iretd";
}

bool is_branch(std::string_view mnemonic) { return mnemonic == "call" || (!mnemonic.empty() && mnemonic.front() == 'j'); }

// `0x<lowercase hex>` and nothing else: a near target as the decoder prints it.
std::optional<std::uint32_t> near_target(std::string_view operands) {
    if (!operands.starts_with("0x") || operands.size() == 2) return std::nullopt;
    const std::string_view digits = operands.substr(2);
    if (!std::all_of(digits.begin(), digits.end(), [](char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'); })) return std::nullopt;
    return text::parse_int<std::uint32_t>(digits, 16);
}

void mask_four(std::set<std::uint32_t>& masked, std::uint64_t from) {
    for (std::uint64_t k = from; k < from + 4; ++k) masked.insert(static_cast<std::uint32_t>(k));
}

}  // namespace

std::optional<std::string> body_digest(ByteView code, std::span<const std::uint32_t> masked, std::size_t last) {
    if (last < kSmallestBody) return std::nullopt;
    Bytes body(code.begin(), code.begin() + static_cast<std::ptrdiff_t>(std::min(last, code.size())));
    for (const std::uint32_t offset : masked)
        if (offset < last && offset < body.size()) body[offset] = 0;
    // the length goes into the digest as text, then the bytes: two bodies of different lengths never digest alike by padding
    Bytes input = to_bytes(std::format("{}:", last));
    append(input, body);
    return crypto::to_hex(crypto::sha1(input)).substr(0, 16);
}

std::map<Address, std::string> bodies32(const pef::PeCarve& carve) {
    const std::uint64_t low = carve.image_base();
    std::uint64_t extent = 0;   // how far the image reaches: an address inside [low, low + extent) is the module's own
    for (const formats::pe::Section& s : carve.image().sections())
        extent = std::max<std::uint64_t>(extent, std::uint64_t{s.virtual_address} + std::max(s.virtual_size, s.raw_size));
    const std::uint64_t high = low + extent;

    std::map<Address, std::string> out;
    for (const auto& [start, function] : carve.functions()) {
        const pef::CodeSection* section = carve.section_of(start);
        if (!section) continue;
        const std::uint32_t end = function.end;
        std::set<std::uint32_t> masked;
        std::size_t last = 0, own = 0;
        std::uint32_t va = start;
        while (va < end && carve.has_instruction(va)) {
            const std::optional<pef::Instruction> found = carve.instruction_at(va);
            if (!found) break;
            const pef::Instruction& ins = *found;
            const std::uint32_t at = va - start;
            struct Field {
                std::uint32_t offset, size;
                std::optional<std::uint32_t> value;
            };
            const Field fields[2] = {{ins.disp_offset, ins.disp_size, static_cast<std::uint32_t>(ins.disp)}, {ins.imm_offset, ins.imm_size, std::nullopt}};
            for (const Field& field : fields) {
                if (field.offset == 0 || field.size != 4) continue;
                std::uint32_t value;
                if (field.value) {
                    value = *field.value;
                } else {
                    // the immediate as it stands in the code (a place the file holds: one the section does not hold is not looked at)
                    const std::uint64_t place = std::uint64_t{va} - section->va + field.offset;
                    if (place + 4 > section->data.size()) continue;
                    value = io::ByteReader(section->data.subspan(place, 4), "section").u32();
                    if (is_branch(ins.mnemonic)) {   // relative: where it goes
                        value = va + ins.size + value;
                        if (start <= value && value < end) continue;   // inside the function: the same anywhere
                        mask_four(masked, std::uint64_t{at} + field.offset);
                        continue;
                    }
                }
                if (low <= value && value < high) mask_four(masked, std::uint64_t{at} + field.offset);   // an address in the module
            }
            own += !runtime_library::is_frame_instruction(ins.mnemonic, ins.op_str);
            va += ins.size;
            if (ends_a_function(ins.mnemonic)) last = va - start;
        }
        if (own < 6) continue;   // (a function of a few instructions of its own is everybody's)
        const std::uint64_t first = std::uint64_t{start} - section->va;
        const std::uint64_t stop = std::min<std::uint64_t>(std::uint64_t{end} - section->va, section->data.size());
        if (first > stop) continue;
        const std::vector<std::uint32_t> mask(masked.begin(), masked.end());
        if (auto digest = body_digest(section->data.subspan(first, stop - first), mask, last)) out[start] = std::move(*digest);
    }
    return out;
}

std::map<Address, std::string> bodies16(const formats::ne::Module& module, std::span<const core16::SegOff> starts, platforms::DecoderSet& decoders) {
    const platforms::Decoder& decoder = decoders.get(platforms::Arch::X86, 16, /*detail=*/false);
    std::map<std::uint32_t, std::vector<std::uint32_t>> by_segment;   // sorted by segment: the order the digests are made in
    for (const core16::SegOff& s : starts) by_segment[s.seg].push_back(s.off);
    std::map<std::uint32_t, formats::ne::Segment> segments;
    for (const formats::ne::Segment& s : module.segments()) segments[s.index] = s;

    std::map<Address, std::string> out;
    for (auto& [number, offsets] : by_segment) {
        const auto found = segments.find(number);
        if (found == segments.end()) continue;
        const formats::ne::Segment& segment = found->second;
        const ByteView file = module.data();
        const std::uint64_t first = std::min<std::uint64_t>(segment.file_offset, file.size());
        const ByteView code = file.subspan(first, std::min<std::uint64_t>(segment.length, file.size() - first));

        // every place the loader writes: each relocation's chain of places, as many bytes as the kind of reference patches
        std::set<std::uint32_t> patched;
        for (const formats::ne::Relocation& r : module.relocations(segment)) {
            const std::uint32_t width = r.source == 3 ? 4 : r.source == 2 ? 2 : r.source == 5 ? 2 : r.source == 0 ? 1 : 2;
            std::uint32_t place = r.offset;
            for (std::uint32_t hops = 0; place != 0xFFFF && place + 1 < code.size() && hops < 0x10000; ++hops) {
                for (std::uint32_t k = 0; k < width; ++k) patched.insert(place + k);
                if (r.additive()) break;   // (an additive reference is one place, not a chain)
                place = io::ByteReader(code.subspan(place, 2), "segment").u16();
            }
        }

        std::sort(offsets.begin(), offsets.end());
        for (std::size_t i = 0; i < offsets.size(); ++i) {
            const std::uint32_t start = offsets[i];
            const std::uint32_t end = i + 1 < offsets.size() ? offsets[i + 1] : static_cast<std::uint32_t>(code.size());
            if (start > end || end > code.size()) continue;
            const ByteView function = code.subspan(start, end - start);
            std::set<std::uint32_t> masked;
            for (auto it = patched.lower_bound(start); it != patched.end() && *it < end; ++it) masked.insert(*it - start);
            std::size_t last = 0, own = 0;
            for (const x86::Insn& ins : x86::decode(decoder, function, start)) {
                const std::uint32_t at = static_cast<std::uint32_t>(ins.address);
                if (is_branch(ins.mnemonic) && ins.size == 3) {
                    // a near call or jump out of the function: its three bytes are the opcode and the displacement the linker fills in
                    if (const auto target = near_target(ins.op_str); target && !(start <= *target && *target < end))
                        for (std::uint32_t k = at - start + 1; k < at - start + 3; ++k) masked.insert(k);
                }
                own += !runtime_library::is_frame_instruction(ins.mnemonic, ins.op_str);
                if (ends_a_function(ins.mnemonic)) last = at + ins.size - start;
            }
            if (own < 6) continue;
            const std::vector<std::uint32_t> mask(masked.begin(), masked.end());
            if (auto digest = body_digest(function, mask, last)) out[static_cast<Address>(decompile::describe::address(number, start))] = std::move(*digest);
        }
    }
    return out;
}

}  // namespace cb::symbols::names
