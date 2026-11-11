// cb/exact/draft/forms.hpp -- the forms of values and places, read back: registers folded into expressions, steps and
// stores joined, pointers and their widths, the library's macros, the frame's locals, and the variants a beam tries.
//
// A draft is C text written by an inverse model of the compiler (text.hpp describes the layout the passes read). Each
// pass here is a rewrite of that text, of one of three kinds:
//
//   * `std::string pass(text)`                  the text with the rewrite applied everywhere it applies;
//   * `std::optional<std::string> pass(text)`   a *variant*: the rewritten text, or nullopt when the pass changes
//                                               nothing - the beam search tries a variant only when it differs;
//   * `std::vector<std::string> pass(text)`     several variants (each int parameter a char ...).
//
// What a pass learns of the function it reads (an array found, a stream a macro reads, the bytes the code widens) is
// written to the `text::FunctionFacts` it is handed; what it asks of the project's toolchain (the kit's headers, the
// library's data places) is a `KitContext`, and what it keeps between the functions of a project is a
// `text::ProjectCaches`. Nothing is held at namespace scope.
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/exact/draft/text.hpp"

namespace cb::exact::draft::forms {

using text::FunctionFacts;
using text::ProjectCaches;

// shapes' flat_returns (the returns as a source with early exits writes them); guard_flat and guard_shared end with it.
// It is handed in so that this module sits below shapes.
using FlatReturns = std::function<std::optional<std::string>(std::string_view)>;

// ---- the frame ------------------------------------------------------------------------------------------------

// The toolchain profile's "frame_order": {"mod": M}. A rule without a modulus (or with 0) renames nothing.
struct FrameRule {
    std::optional<std::uint32_t> mod;
};

struct FrameOptions {
    bool registers_last = false;           // the register locals take the slots after every other local's
    bool register_local_has_slot = false;  // the toolchain gives a register local a frame slot (a filler is made for the gap)
};

// The locals renamed so that the compiler gives each the frame slot the program shows. The compiler gives the locals
// of a function (register ones too) their slots in the order of their names' buckets in its table of names - the sum
// of the name's characters, mod M - ascending, the first next to BP. An auto the model names vN is at BP-N; a filler
// _pN after vN; a register local takes a free slot. A name is kept where its bucket falls in order, else a letter is
// put after it. Nullopt: nothing to rename (or no letter keeps the order).
std::optional<std::string> frame_names(std::string_view text, const FrameRule& rule, FrameOptions options = {});

// The frame one word bigger: the trailing filler two bytes more, or one added after the last local.
std::optional<std::string> frame_more(std::string_view text);

// The frame's first local vN (bp-N) with room between it and bp: a filler declared before it.
std::optional<std::string> lead_filler(std::string_view text);

// ---- pointers and their widths --------------------------------------------------------------------------------

// The pointers a function's names are, from those it declares: a name compared with a pointer, given one's value, or
// giving its value to one is one too.
std::string pointer_flow(std::string_view text);

// The text with one of its int parameters a char (each, at most `most`), then those parameters all chars.
std::vector<std::string> char_param_variants(std::string_view text, std::size_t most = 2);

// The text with its register pointers (at most `most`) made int * - each, then all.
std::vector<std::string> pointer_widths(std::string_view text, std::size_t most = 2);

// The text with one of its int globals unsigned (each, at most `most`).
std::vector<std::string> unsigned_globals(std::string_view text, std::size_t most = 2);

// A number a pointer is compared with, or given (`p < 14708`): the place of data there, as g_XXXX (one variant each,
// at most `most`; numbers from `lo` up to `data_size`).
struct AddressVariant {
    std::string name, text;
};
std::vector<AddressVariant> address_variants(std::string_view text, std::uint32_t data_size, std::size_t most = 2,
                                             std::uint32_t lo = 0x100);

// (E << 1)->fN, N a place in the data: an array there indexed by E, and the other readings of a scaled index through a
// place in the data. Records the arrays and arrays of structs it finds in `facts`.
std::string word_arrays(std::string_view text, FunctionFacts& facts, std::uint32_t lo = 0x40);

// (i << 1) + K, K a place in the data: the address of a word there, &g_K[i].
std::optional<std::string> scaled_places(std::string_view text, FunctionFacts& facts);

// bx = p + (i << 1); .. *bx: a word of p's at i, p[i]; p declared a pointer to int.
std::string scaled_index(std::string_view text);

// (*x)(..): x declared a pointer to a function.
std::string fn_pointers(std::string_view text);

// ---- the library ----------------------------------------------------------------------------------------------

// The macros of a kit header that read a table of the library (profile "table_macros").
struct TableMacroSpec {
    std::string header;
};

// A library's data a macro names - stdio's streams (profile "data_macros"): `base` is the data's name, `size` the
// bytes of one element, `names` the elements' names, `fields` a format by the offset within an element.
struct DataMacroSpec {
    std::string base;
    std::uint32_t size = 0;
    std::vector<std::string> names;
    std::map<std::uint32_t, std::string> fields;   // offset in the element -> "fileno(%s)"
    std::optional<std::string> include;
};

// A library macro the compiler expanded in line (profile "macros"). `as` is the macro's text with a `%s` for each
// argument of the routine (`%%` is a percent sign).
struct LibraryMacroSpec {
    std::string call;                    // the routine the expansion calls
    std::string as;                      // "getc(%s)"
    std::string type = "FILE";           // what the stream argument points to
    std::optional<std::string> include;  // the header the macro comes from
    std::vector<std::size_t> byte_args;  // arguments the macro stores as a byte
};

// What the passes ask of the project's toolchain.
struct KitContext {
    std::string project;                         // the project's directory: the key of what is cached
    std::string toolchain;                       // the toolchain's name
    std::optional<std::filesystem::path> kit;    // the kit's folder (headers anywhere below it); nullopt: none
    // {name: address} of the library's data (the library stage's); empty when it could not be located.
    std::function<std::map<std::string, std::uint32_t>()> library_data;
};

// A header of the kit, as text (empty when the kit has none). The file is found by name, case-insensitively, anywhere
// below the kit's folder.
std::string header_text(const KitContext& kit, std::string_view header);

// The functions and macros a header of the kit declares (the draft does not declare them again).
std::set<std::string> header_names(const KitContext& kit, ProjectCaches& caches, std::string_view header);

// The macros of a header that read a table of the library (ctype.h's isdigit(c) ...), read back where the model has
// the table's place: (c->fN & M), N the table's place + its offset, M the macro's mask.
std::string table_macros(const KitContext& kit, ProjectCaches& caches, FunctionFacts& facts, std::string text,
                         std::span<const TableMacroSpec> specs);

// The library's data a macro names, read back where the model has their places (g_XXXX, or a number passed: a
// stream's place). Throws UsageError for a spec whose element size is 0 or whose format takes no argument.
std::string data_macros(const KitContext& kit, ProjectCaches& caches, FunctionFacts& facts, std::string text,
                        std::span<const DataMacroSpec> specs);

// A library macro the compiler expanded in line, read back as the macro: from the line that steps the stream's count
// to the routine's call, the region is the macro's value. Throws UsageError when a spec's `as` takes a different
// number of arguments than the call has.
std::string library_macros(std::string text, std::span<const LibraryMacroSpec> macros, FunctionFacts& facts);

// ---- steps, stores, tests -------------------------------------------------------------------------------------

std::optional<std::string> pre_steps(std::string_view text);                            // X--; then a test of X
std::optional<std::string> pre_increments(std::string_view text, FunctionFacts& facts); // X = X + n; *X: *++X
std::optional<std::string> and_returns(std::string_view text);                          // if (A) return E; return 0;
std::string word_constants(std::string_view text);                                      // 32768..65535 as the negative int
std::optional<std::string> assign_in_tests(std::string_view text);                      // X = E; if (X ..)
std::string char_params(std::string_view text);                                         // a parameter only read as a byte
std::optional<std::string> compare_results(std::string_view text);                      // ((A op B) ? 1 : 0)
std::optional<std::string> register_reads(std::string_view text);                       // r = E; S(E->f0): through r
std::optional<std::string> split_ternaries(std::string_view text);                      // return (C ? A : B);
std::string store_reloads(std::string_view text);                                       // M = A; .. r = M;
std::optional<std::string> register_copies(std::string_view text, const FunctionFacts& facts);   // r = x = E;
std::optional<std::string> unsigned_registers(std::string_view text);                   // register int locals unsigned
std::optional<std::string> unmasked(std::string_view text);                             // (A == (B & 0xff))
std::optional<std::string> high_words(std::string_view text);                           // a long's high word tested alone
std::optional<std::string> assign_in_args(std::string_view text);                       // X = E; f(..X..)
std::optional<std::string> return_sources(std::string_view text);                       // X = Y; return X;
std::optional<std::string> compound_assigns(std::string_view text);                     // X = X op E;
std::optional<std::string> chain_assigns(std::string_view text);                        // A = E; B = A;
std::optional<std::string> register_temps(std::string_view text);                       // a call's value read once
std::string char_stores(std::string_view text);                                         // vN = al: a char local
std::optional<std::string> store_increments(std::string_view text);                     // *p = X; p = p + 1;
std::string register_values(std::string_view text);                                     // r = E; r read more than once

// ---- control flow ---------------------------------------------------------------------------------------------

std::string loop_gotos(std::string_view text);                                          // gotos back to a loop's top
std::optional<std::string> default_last(std::string_view text);                         // a switch's default arm last
std::optional<std::string> empty_cases_first(std::string_view text);                    // arms that only break first
std::optional<std::string> or_loops(std::string_view text);                             // for (;;) { if (A) ..: while (A || B)
std::optional<std::string> switch_breaks(std::string_view text);                        // continue -> break
std::optional<std::string> default_after(std::string_view text);                        // default falling into a return
std::optional<std::string> push_tails(std::string_view text);                           // the statement after an if, in each arm
std::string shared_arms(std::string_view text);                                         // if (A) { if (B) { S } } else { S }
std::optional<std::string> guard_returns(std::string_view text);                        // if (C) { S } return X;
std::optional<std::string> guard_shared(std::string_view text, const FlatReturns& flat_returns);
std::optional<std::string> guard_flat(std::string_view text, const FlatReturns& flat_returns);
std::string bool_trees(std::string_view text);                                          // an if-tree giving r 1 or 0
std::optional<std::string> shared_tails(std::string_view text);                         // one copy of T both ways reach
std::optional<std::string> no_default(std::string_view text);                           // default: return X; out of the switch
std::string guard_gotos(std::string_view text);                                         // a goto past statements under a test
std::string merge_conditions(std::string_view text);                                    // nested ifs joined with && and ||
std::string case_falls(std::string_view text);                                          // a case going on into the next
std::optional<std::string> inner_registers(std::string_view text);                      // a register variable in a block
std::string shapes(std::string_view text);                                              // while loops, dead code, *p++
std::string unused_locals(std::string_view text);                                       // locals nothing names, left out

// ---- declarations ---------------------------------------------------------------------------------------------

// t without x in its `int a, x, b;` declaration (the line gone if x was all of it).
std::string drop_decl(std::string_view t, std::string_view x);

// t with x's declaration (`int a, x, b;`: a parameter's, a local's) as `new_declaration;`, or nullopt.
std::optional<std::string> redeclare(std::string_view t, std::string_view x, std::string_view new_declaration);

}  // namespace cb::exact::draft::forms
