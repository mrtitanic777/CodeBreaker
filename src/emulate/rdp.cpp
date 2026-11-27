// emulate/rdp.cpp -- an RDP display list read as its commands (rdp.hpp).

#include "emulate/rdp.hpp"

#include <array>
#include <format>

namespace cb::emulate::rdp {

namespace {

constexpr std::array<std::string_view, 64> kNames = {
    "", "", "", "", "", "", "", "",                                                                   // 0x00
    "fill_triangle", "fill_zbuffer_triangle", "texture_triangle", "texture_zbuffer_triangle",          // 0x08
    "shade_triangle", "shade_zbuffer_triangle", "shade_texture_triangle", "shade_texture_zbuffer_triangle",
    "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",                                  // 0x10
    "", "", "", "", "texture_rectangle", "texture_rectangle_flip", "sync_load", "sync_pipe",          // 0x20
    "sync_tile", "sync_full", "set_key_gb", "set_key_r", "set_convert", "set_scissor", "set_prim_depth", "set_other_modes",
    "load_tlut", "", "set_tile_size", "load_block", "load_tile", "set_tile", "fill_rectangle", "set_fill_color",   // 0x30
    "set_fog_color", "set_blend_color", "set_prim_color", "set_env_color", "set_combine", "set_texture_image", "set_z_image", "set_color_image",
};

}  // namespace

std::string_view name_of(std::uint64_t word) { return kNames[(word >> 56) & 0x3F]; }

unsigned length_of(std::uint64_t word) {
    const unsigned op = (word >> 56) & 0x3F;
    if (op >= 0x08 && op <= 0x0F) {   // the edges, then the shade, texture and depth coefficients
        unsigned n = 4;
        if (op & 4) n += 8;
        if (op & 2) n += 8;
        if (op & 1) n += 2;
        return n;
    }
    if (op == 0x24 || op == 0x25) return 2;
    return 1;
}

std::string Command::text() const {
    std::string out = name;
    for (const std::uint64_t w : words) out += std::format(" {:016X}", w);
    return out;
}

std::vector<Command> commands(ByteView memory, std::uint32_t start, std::uint32_t end) {
    std::vector<Command> out;
    const auto word_at = [&](std::uint64_t at) {
        std::uint64_t w = 0;
        for (unsigned i = 0; i < 8; ++i) w = (w << 8) | memory[at + i];
        return w;
    };
    std::uint64_t at = start;
    while (at + 8 <= end && at + 8 <= memory.size()) {
        const std::uint64_t first = word_at(at);
        Command c;
        const std::string_view name = name_of(first);
        c.name = name.empty() ? std::format("unknown_{:02X}", (first >> 56) & 0x3F) : std::string(name);
        const unsigned n = length_of(first);
        for (unsigned i = 0; i < n && at + 8 <= memory.size() && at + 8 <= end; ++i, at += 8) c.words.push_back(word_at(at));
        out.push_back(std::move(c));
    }
    return out;
}

}  // namespace cb::emulate::rdp
