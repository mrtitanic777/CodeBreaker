// cb/port/arith16.hpp -- the arithmetic of a 16-bit int kept, where the C of today would do it otherwise.
//
// On the 8086 an int was 16 bits, so an unsigned (16 bits) met by an int made the int unsigned: `u > -1` was false,
// `u - 1` with u 0 was 65535. The port keeps every int 16 bits wide (int16_t, uint16_t), which keeps what is stored -
// but the C of today makes both of them a 32-bit int before it compares, divides or widens them: `u > -1` is true,
// `(long)(u - 1)` is -1. Wherever that changes the answer this pass says, by a cast, what the 8086 did:
//
//   a comparison, / % or >> whose operands the 8086 made unsigned   each operand that is not so already: (uint16_t)
//   ... of 16-bit operands of which one is arithmetic (it may have      that operand: (int16_t)(...) or (uint16_t)(...)
//       passed 16 bits)
//   arithmetic of 16 bits made 32 (a long assigned, given, cast)    the arithmetic, cut to its 16 bits first
//
// Each expression's type is worked out by the 8086's rules from the tree clang makes of the port (clang -Xclang
// -ast-dump=json: every expression with its exact place, and the types as the source names them); the casts go in
// at those places, the rest of the text as it was.
//
//     std::optional<Tree> tree = dump(clang, "a.c", folder);        // clang's tree of one file, or nothing
//     std::vector<Edit> edits = edits_of(*tree, text, tree->find_file("a.c"));
//     std::string cast_text = apply_edits(text, edits);
//
// run() does that for a list of files and keeps each file's result in a cache by everything it depends on.
//
// The tree is read by a streaming parser into typed nodes (`Tree`), never as a JSON document: clang's dump of a
// program with its headers is hundreds of megabytes. A location carries the file it is in as a `FileId`: clang names
// the file only where it changes, so the parser follows the names in document order. (A location's `includedFrom`
// names the file that included it, not the file it is in, and does not move that cursor.)
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/base/cache.hpp"

namespace cb::port::arith16 {

// ---- the tree clang makes -------------------------------------------------------------------------------------

// A file clang named: an index into Tree::files(), in the order the names are first met.
using FileId = std::int32_t;
inline constexpr FileId kNoFile = -1;

// The kinds of node this pass reads (the rest are Other; their children are still walked).
enum class Kind : std::uint8_t {
    Other,
    ParenExpr,
    ImplicitCastExpr,
    CStyleCastExpr,
    IntegerLiteral,
    CharacterLiteral,
    BinaryOperator,
    CompoundAssignOperator,
    UnaryOperator,
    ConditionalOperator,
    DeclRefExpr,
    MemberExpr,
    ArraySubscriptExpr,
    CallExpr,
};

enum class CastKind : std::uint8_t { Other, LValueToRValue, NoOp, IntegralCast };

// The operators of a BinaryOperator or UnaryOperator node (a unary minus and a binary one are both Sub).
enum class Op : std::uint8_t {
    Other, Add, Sub, Mul, Div, Rem, Shl, Shr, And, Or, Xor,    // arithmetic
    Lt, Gt, Le, Ge, Eq, Ne, LogAnd, LogOr,                      // comparison and logic
    Comma, Assign, Not, BitNot, Plus, Inc, Dec, Deref, AddrOf,   // , = ! ~ + ++ -- * &
};

// A place in a file: the byte offset and the length of the token there. A location in a macro has an expansion
// (where the macro was used) and, instead of a place of its own, says it has a spelling (where its text is).
struct Loc {
    std::optional<std::uint64_t> offset;
    std::uint64_t tok_len = 0;
    FileId file = kNoFile;
    bool has_spelling = false;
    std::unique_ptr<Loc> expansion;
};

struct Range {
    Loc begin;
    Loc end;
};

// The 8086's types of an expression: what an int of 16 bits and a long of 32 made of it.
enum class Prim : std::uint8_t { I16, U16, I32, U32 };

// A node of the tree. Nodes are moved, never copied.
struct Node {
    Kind kind = Kind::Other;
    CastKind cast_kind = CastKind::Other;
    Op op = Op::Other;
    std::string qual_type;                     // the type as the source names it; empty: the node has none
    std::optional<Prim> prim;                  // the 8086's type of qual_type, when it is one of those
    std::optional<std::int64_t> value;         // an IntegerLiteral's or CharacterLiteral's value
    Loc loc;
    Range range;
    std::vector<Node> inner;
};

// What clang made of one file: the top-level nodes and the names of the files they are in.
class Tree {
public:
    const std::vector<Node>& top() const { return root_.inner; }
    // The files named, in the order they were first met (including the files that only included another).
    const std::vector<std::string>& files() const { return files_; }
    // The file `name` stands for (as the source's own #include spelled it, or as the command line gave it): the
    // first of files() with that name, separators and a leading "./" aside. kNoFile when clang never named it.
    FileId find_file(std::string_view name) const;

private:
    friend std::optional<Tree> parse_tree(std::string_view json);
    Node root_;
    std::vector<std::string> files_;
};

// The tree of a JSON document of clang's -ast-dump=json; nothing when it is not that.
std::optional<Tree> parse_tree(std::string_view json);

// clang's tree of a file of the port: runs `clang -fsyntax-only -ast-dump=json` in `folder` (the compiler's complaints
// do not stop it; a file it can read none of gives nothing). `extra` are more compiler arguments. Throws ToolError
// when clang cannot be started.
std::optional<Tree> dump(const std::filesystem::path& clang, std::string_view file, const std::filesystem::path& folder,
                         std::span<const std::string> extra = {});

// ---- what the 8086 would have made of an expression ----------------------------------------------------------

// The file being read: its text (the offsets of the tree are bytes of it) and which file of the tree that is.
struct Source {
    std::string_view text;
    FileId main = kNoFile;
};

// The first and last byte of an expression in the file, and the file it is in.
struct Span {
    std::uint64_t start = 0;
    std::uint64_t end = 0;
    FileId file = kNoFile;
};

// The place of an expression in the file - nothing when it is in a macro that takes arguments (assert(...): not a
// constant) or has no place of its own.
std::optional<Span> place(const Node& n, std::string_view text);

// The text of an expression, when it is in the file being read.
std::optional<std::string_view> spelled(const Node& n, const Source& src);

// The type the 8086's compiler gave an expression - nothing for a pointer, a double, or what is not known.
std::optional<Prim> old_type(const Node& n, const Source& src);

// The 8086's usual arithmetic conversions (int 16 bits, long 32).
std::optional<Prim> common(std::optional<Prim> a, std::optional<Prim> b);

// An expression with its parentheses and conversions passed.
const Node& bare(const Node& n);

// A constant of no sign: the same number whichever way it is converted.
bool nonneg(const Node& n);

// An operand that is arithmetic of its own that may have gone past 16 bits (or, with & | ^, mixed signs), its
// parentheses passed; nullptr when it is not.
const Node* compound(const Node& n, const Source& src);

// Whether an expression is written as one plain word: a name, a member, an index, a number.
bool simple(const Node& n, const Source& src);

// ---- the casts ------------------------------------------------------------------------------------------------

// Text to put at a byte offset of the file.
struct Edit {
    std::uint64_t at = 0;
    std::string text;
    friend bool operator==(const Edit&, const Edit&) = default;
};

// The casts that keep the 8086's arithmetic, as insertions. A cast that wraps an expression is an opening
// "(type)(" at its start and a ")" at its end; two casts that end at the same place each get their ")".
std::vector<Edit> edits_of(const Tree& tree, std::string_view text, FileId main);

// The text with the insertions made: at one offset a ")" goes before what opens there, and openers keep the order
// they were given in. Offsets past the end of the text are an error.
std::string apply_edits(std::string_view text, std::span<const Edit> edits);

// ---- keeping the results --------------------------------------------------------------------------------------

// The kind of cache entry run() keeps its results in, and the version of the code that makes them: change the
// version whenever the casts a file gets would change.
inline constexpr std::string_view kCacheKind = "port-passes";
inline constexpr std::string_view kCodeVersion = "arith16-1";

// Declare kCacheKind to the cache (a State kind of this code's generation). run() does it; call it first only to have the
// kind declared before anything else uses the cache.
void declare_cache(base::cache::Cache& cache);

// The names a file `#include`s (in "" or <>), in the order they stand.
std::vector<std::string> included_names(std::string_view text);

// The file and every header of the port it reaches, by their contents (one changed: another result). Headers not
// in `folder` are not part of the port and are passed; the digest is lowercase hex.
std::string reach_key(const std::filesystem::path& folder, std::string_view file);

// What a pass's result for one file depends on: the file and the headers it reaches, the compiler (by name, size and
// date), the pass (`kind`, `code_version`) and its arguments (`extra`).
std::string pass_key(const std::filesystem::path& clang, const std::filesystem::path& folder, std::string_view file,
                     std::string_view kind, std::string_view code_version, std::span<const std::string> extra = {});

// For each file of the port that clang could read and that needs casts: its text with them. A file's result is kept
// in the cache by its pass_key, so the same file again is not read again. Files clang could read none of, and files
// that need no casts, are left out.
std::map<std::string, std::string> run(base::cache::Cache& cache, const std::filesystem::path& clang,
                                       const std::filesystem::path& folder, std::span<const std::string> files);

}  // namespace cb::port::arith16
