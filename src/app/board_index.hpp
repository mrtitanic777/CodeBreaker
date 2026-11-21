// app/board_index.hpp -- how the files of a board refer to one another: a node per card (its SHA-256, the group of identical copies, what its headers say) and
// a directed edge per link found.
//
//     Imports      the file's PE, NE or LE import table names the module           the loader needs it
//     Installs     a setup script's CopyFiles sections name the file               the script copies it
//     References   the file's bytes hold the other file's exact name               a lead, not a proof: it may be a LoadLibrary, or only a word
//
// An edge's `confidence` (0..100) says how sure the *target* is: 100 when one file on the board has that name or every copy of it is identical, less when
// copies differ and the nearest one was chosen. A name that matches nothing is counted on the node as unresolved, never guessed.
// No operating-system API: tested on every system (board_index_test.cpp).
#pragma once

#include <atomic>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "app/board.hpp"
#include "core/bytes.hpp"

namespace cb::app::board {

enum class Link : std::uint8_t { Imports, Installs, References };
std::string_view link_name(Link link);

struct Edge {
    std::uint32_t from = 0, to = 0;    // Board::cards
    Link link = Link::References;
    std::uint8_t confidence = 0;
    // Imports only:
    std::uint32_t symbols = 0;         // functions the source imports from the target
    std::uint32_t matched = 0;         // of those, how many the target really exports: the export-to-import links
    std::uint32_t calls = 0;           // places in the source's code that call or load them (static call sites, not calls made at run time)
    friend bool operator==(const Edge&, const Edge&) = default;
};

struct Node {
    std::string sha256;                // empty: the file could not be read
    std::uint32_t group = 0;           // the first card with the same bytes (itself when it is the first or unread)
    std::uint32_t copies = 1;          // how many cards hold these bytes
    std::uint32_t exports = 0;
    std::uint32_t unresolved = 0;      // imported or installed names no file on the board has
    std::string subsystem;             // PE: "Windows GUI"; NE and LE: empty
};

// A function a file exports, and one file's use of it.
struct ExportedSymbol {
    std::uint32_t card = 0, ordinal = 0;
    std::string name;                  // empty: by ordinal only
};
struct ExportUse {
    std::uint32_t from = 0;            // the file that imports it
    std::uint32_t symbol = 0;          // Index::exports
    std::uint32_t calls = 0;           // static call sites in that file
};

struct Index {
    std::vector<Node> nodes;           // one per Board::cards
    std::vector<Edge> edges;           // sorted by (from, to, link)
    std::vector<ExportedSymbol> exports;   // sorted by (card, ordinal)
    std::vector<ExportUse> export_uses;    // sorted by (symbol, from)
    std::uint64_t unreadable = 0;      // files that could not be read or hashed
    std::uint64_t damaged = 0;         // executables whose tables could not be parsed
    std::map<std::string, std::uint32_t> missing;   // imported or installed names no file on the board has (lower case), and how many files ask for each

    // Rebuilt by finish(): the edges of a card, as positions in `edges`.
    std::vector<std::uint32_t> out_start, in_start, in_edges, export_start, use_start;
    void finish();
    std::span<const ExportedSymbol> exports_of(std::uint32_t card) const;
    std::uint32_t first_export(std::uint32_t card) const { return export_start[card]; }
    std::span<const ExportUse> uses_of(std::uint32_t symbol) const;
    std::span<const Edge> out_of(std::uint32_t card) const;
    std::span<const std::uint32_t> into(std::uint32_t card) const;   // positions in `edges`
};

// ---- the parts, each testable alone

// What a binary uses of one module: the functions ("name" in lower case, "#12" by ordinal) and the places in its code that reach them.
struct ModuleUse {
    std::map<std::string, std::uint32_t> symbols;   // each function and its call sites
    std::uint32_t sites = 0;
};
// What a binary's own tables say. `modules` as stored ("KERNEL32.dll", "USER"), `uses` beside them.
struct BinaryFacts {
    std::vector<std::string> modules;
    std::vector<ModuleUse> uses;
    std::unordered_map<std::string, std::uint32_t> export_names;   // lower-case names and "#ordinal", to the ordinal
    std::map<std::uint32_t, std::string> export_list;              // ordinal to name (empty: by ordinal only)
    std::uint32_t exports = 0;
    std::string subsystem;
    bool damaged = false;
};
BinaryFacts read_binary(ByteView data, std::string_view format);

// The call sites of each module in 32-bit code: call [slot], a load of the slot into a register (mov reg, [slot]), and a direct call of a jmp [slot] thunk.
struct CodeSpan {
    ByteView bytes;
    std::uint64_t va = 0;
};
std::vector<std::uint32_t> pe_call_sites(std::span<const CodeSpan> code, const std::unordered_map<std::uint64_t, std::uint32_t>& slot_module, std::size_t modules);
// How many places one NE relocation patches: the chain threaded through the segment from `first`, ending at 0xFFFF.
std::uint32_t ne_chain_sites(ByteView segment, std::uint16_t first);

// The files a setup script copies: every entry of every section some CopyFiles= names, the installed and the source name both ("@file" directly too).
std::vector<std::string> inf_copied_files(std::string_view script);

// Every file name in the bytes (ASCII or UTF-16LE) that is in `known` (lower case), lower case, each once.
std::vector<std::string> names_in_bytes(ByteView data, const std::unordered_set<std::string>& known);

// Which card a name means, seen from `from`.
struct Resolved {
    std::uint32_t card = 0;
    std::uint8_t confidence = 0;
};
class NameTable {
public:
    explicit NameTable(const Board& board);
    // `identical(a, b)`: two cards hold the same bytes. A module name with no extension is tried as .dll, .exe, .drv, .386, .vxd, .sys; the 16-bit KERNEL module is the file KRNL386.EXE.
    std::optional<Resolved> resolve(std::string_view name, std::uint32_t from, const std::function<bool(std::uint32_t, std::uint32_t)>& identical) const;
    const std::unordered_set<std::string>& names() const { return names_; }

private:
    const Board& board_;
    std::unordered_map<std::string, std::vector<std::uint32_t>> by_name_;
    std::unordered_set<std::string> names_;
};

// ---- the whole board

struct IndexProgress {
    std::string stage;
    std::uint64_t done = 0, total = 0;
};
using IndexReport = std::function<void(const IndexProgress&)>;

// Reads every card's file once. Throws nothing for a bad file (it is counted); nullopt when cancelled.
std::optional<Index> build_index(const Board& board, const IndexReport& report, const std::atomic<bool>& cancel);
// root/index.tsv, replaced whole.
void save_index(const Board& board, const Index& index);
// The index kept for this board; nullopt when there is none or it is for other cards.
std::optional<Index> load_index(const Board& board);

}  // namespace cb::app::board
