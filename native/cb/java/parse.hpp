// cb/java/parse.hpp -- Java source text -> IR (cb/java/ir.hpp).
//
// This is the front-end. It extracts STRUCTURE only -- it does not translate anything. Decompiled-Minecraft Java is a
// constrained, well-formatted subset (tab-indented, obfuscated `var1` locals, one declarator per line), so a
// brace/paren-aware scan is enough; no full Java grammar is required.
//
// Design notes:
//   * Comments are blanked (replaced with spaces, newlines kept) rather than deleted, so every byte offset still maps
//     to its ORIGINAL line number -- method bodies carry real line numbers for ledger attribution downstream.
//   * The body walk separates fields (end in `;` at depth 0) from methods (have a `{ }` body at depth 0).
//     Abstract/interface methods (`...);` with params) are methods with an empty body.
//   * Inner types are recognised and skipped, not mis-parsed; `static { }` and instance `{ }` blocks are kept
//     as raw bodies.
#pragma once

#include <filesystem>
#include <optional>
#include <string_view>

#include "cb/java/ir.hpp"

namespace cb::java::parse {

// `int[][]` / `List<ItemStack>` / `net.minecraft.Block` -> a JType (a package qualifier is dropped: emit uses simple
// names; varargs are not handled here, see the parameter list of a method).
ir::JType parse_type(std::string_view text);

// One Java top-level type from its source text. `source_path` is recorded on the class. ParseError when the text
// holds no type declaration or a member that cannot be read.
ir::JClass parse_source(std::string_view source, const std::optional<std::filesystem::path>& source_path = std::nullopt);

// The same for a file (read as UTF-8, bad bytes replaced, line ends normalised). IoError when it cannot be read.
ir::JClass parse_file(const std::filesystem::path& path);

}  // namespace cb::java::parse
