// codebreaker/java/parse.hpp -- typed C++ port of codebreaker.java.parse.
//
// parse.py -- Module 2: Java source text -> IR (codebreaker.java.ir).
//
// This is the front-end. It extracts STRUCTURE only -- it does not translate
// anything. Decompiled-Minecraft Java is a constrained, well-formatted subset
// (tab-indented, obfuscated `var1` locals, one declarator per line), so a
// brace/paren-aware scan is enough; no full Java grammar required.
//
// Design notes:
//   * Comments are blanked (replaced with spaces, newlines kept) rather than
//     deleted, so every character offset still maps to its ORIGINAL line number --
//     method bodies carry real line numbers for ledger attribution downstream.
//   * The body walk separates fields (end in `;` at depth 0) from methods (have a
//     `{ }` body at depth 0). Abstract/interface methods (`...);` with params) are
//     methods with an empty body.
//   * Deferred on purpose (added when `emit` needs them): inner types and
//     static-initializer blocks are recognized and skipped, not mis-parsed.
//
// Notes on the port:
//   * `_iter_members` was a generator; it returns the list of members it would have yielded (a MemberItem
//     each; `kind` is 'decl' / 'method' / 'static_init' / 'inst_init');
//   * `_split_mods` returns the modifier set and the remaining tokens;
//   * the module-level modifier set and compiled regex are made on first use (accessor functions).
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "py.hpp"
#include "py_os.hpp"
#include "codebreaker/java/ir.hpp"

namespace codebreaker::java::parse {

const py::set<py::str>& _MODIFIERS();
const py::re::Pattern& _TYPE_DECL();

// ---------------------------------------------------------------------------
// Lexical helpers
// ---------------------------------------------------------------------------
// Return src with // and /* */ comments replaced by spaces (newlines kept),
// so character offsets and line numbers are preserved. String/char literals
// are respected.
py::str _blank_comments(const py::str& src);

// Index just past the `}` matching the `{` at open_idx (literal-aware).
int64_t _match_brace(const py::str& s, int64_t open_idx);

// Split on commas not nested in (), <>, [] or literals.
py::list<py::str> _split_top_commas(const py::str& s);

// ---------------------------------------------------------------------------
// Type / parameter parsing
// ---------------------------------------------------------------------------
// 'int[][]' / 'List<ItemStack>' / 'net.minecraft.Block' -> ir.JType.
py::ref<ir::JType> parse_type(const py::str& s);

py::list<py::ref<ir::Param>> _parse_params(const py::str& s);

std::tuple<py::set<py::str>, py::list<py::str>> _split_mods(const py::list<py::str>& tokens);

py::str _access_of(const py::set<py::str>& mods);

// ---------------------------------------------------------------------------
// Member parsing
// ---------------------------------------------------------------------------
int64_t _line_of(const py::str& src, int64_t offset);

// `[mods] Type name [= init]` -> [Field]. One declarator per line (the MCP shape).
py::list<py::ref<ir::Field>> _parse_field_decl(const py::str& decl, const py::str& class_name, int64_t line);

int64_t _toplevel_index(const py::str& s, const py::str& ch);

// `public final Vec3D subtract(Vec3D var1)` -> Method (no body yet).
// The first '(' opens the param list; return type / name never contain one.
py::ref<ir::Method> _parse_signature(const py::str& header, const py::str& class_name);

// A member decl is a method (vs field) if its first '(' is not preceded by '='.
// (A field initializer's '(' always follows the '='.)
bool _looks_like_method(const py::str& decl);

// One member the walk yields: ('decl', decl, line), ('method', header, inner, hline, bline),
// ('static_init', inner, line) or ('inst_init', inner, line).
struct MemberItem {
    py::str kind;
    py::str text;       // decl / header / inner
    py::str inner;      // method body text ('method' only)
    int64_t line = 0;   // decl line / header line / body line
    int64_t bline = 0;  // method body line ('method' only)
};
py::list<MemberItem> _iter_members(const py::str& body, int64_t body_offset, const py::str& src);

py::list<std::tuple<int64_t, py::str>> _body_lines(const py::str& src, int64_t start_line, int64_t end_line);

// ---------------------------------------------------------------------------
// Top-level entry
// ---------------------------------------------------------------------------
py::ref<ir::JClass> parse_source(const py::str& src, const std::optional<py::str>& source_path = std::nullopt);

void _parse_enum_body(const py::ref<ir::JClass>& cls, const py::str& body, int64_t body_offset, const py::str& blanked,
                      const py::str& src);

py::ref<ir::JClass> parse_file(const py::str& path);

}  // namespace codebreaker::java::parse
