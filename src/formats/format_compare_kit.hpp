// formats/format_compare_kit.hpp -- what format_compare.cpp and format_compare_console.cpp share: the value checker and
// the checks every comparison ends with.
#pragma once

#include <format>
#include <string>
#include <string_view>

#include "formats/format_compare.hpp"

namespace cb::formats::engine {

inline constexpr std::size_t kKeptMismatches = 40;

class Checker {
public:
    explicit Checker(Comparison& c) : c_(c) {}

    void eq(std::string_view field, std::uint64_t engine, std::uint64_t reader) {
        ++c_.compared;
        if (engine != reader) add(field, std::format("{} (0x{:x})", engine, engine), std::format("{} (0x{:x})", reader, reader));
    }
    void eq_text(std::string_view field, const std::string& engine, const std::string& reader) {
        ++c_.compared;
        if (engine != reader) add(field, "\"" + engine + "\"", "\"" + reader + "\"");
    }
    void eq_bytes(std::string_view field, ByteView engine, ByteView reader) {
        ++c_.compared;
        if (!equal_bytes(engine, reader)) add(field, std::format("{} bytes", engine.size()), std::format("{} bytes (differ)", reader.size()));
    }
    void add(std::string_view field, std::string engine, std::string reader) {
        ++c_.mismatch_count;
        if (c_.mismatches.size() < kKeptMismatches) c_.mismatches.push_back({std::string(field), std::move(engine), std::move(reader)});
    }

private:
    Comparison& c_;
};

// The bytes the tree names, written onto zeros, equal the file's bytes at those places.
bool covered_equal(const Tree& tree);

// The tables behind the headers, from the tree read with every part of the description (format_compare_tables.cpp).
void compare_pe_tables(Checker& k, const Tree& tree, ByteView data);
void compare_ne_tables(Checker& k, const Tree& tree, ByteView data);
void compare_le_tables(Checker& k, const Tree& tree, ByteView data);

// The console formats (gb, gba, ...): the engine's tree of `data` against the reader of the same name. Returns false when
// `format` is not one of them.
bool compare_console(Comparison& c, const Registry& registry, std::string_view format, ByteView data);

}  // namespace cb::formats::engine
