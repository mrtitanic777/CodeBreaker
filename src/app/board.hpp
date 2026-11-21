// app/board.hpp -- the operating-system board: every file of a disc image as a card, laid out in piles by what kind of file it is.
//
//     board::Board b = board::scan(image, root, report, cancel);    the disc and its cabinets extracted under root/files, a card per file
//     board::save(b);                                               kept in root/board.tsv and root/state.tsv
//     auto again = board::load(root);                               the next time: no extraction
//     auto stacks = board::build_stacks(b, "kernel");               what each column shows (to do, and done)
//
// A card is Done only when a conversion really ended Done and its output folder still has something in it: state.tsv is what the conversion said, and
// load() takes a Done back when the folder has gone. No operating-system API here: tested on every system (board_test.cpp).
#pragma once

#include <array>
#include <atomic>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "core/bytes.hpp"

namespace cb::app::board {

enum class Pile : std::uint8_t { Programs, Libraries, Drivers, Setup, Text, Help, Fonts, Media, Cabinets, Data };
inline constexpr std::size_t kPileCount = 10;
std::string_view pile_name(Pile pile);

// What a conversion of the card has come to. Queued and Running last only while the window runs; the others are kept.
enum class Conv : std::uint8_t { None, Queued, Running, Done, CannotConvert, Failed, Cancelled };
std::string_view conv_words(Conv conv);

struct Kind {
    Pile pile = Pile::Data;
    std::string format;                // "PE", "NE", "LE", "MZ", "SZDD", "CAB", "text"; empty when the bytes say nothing
};
// What a file is, from its name and its first bytes. A known extension chooses the pile; the bytes name the format, and choose the pile when the extension says nothing.
Kind classify(std::string_view name, ByteView head);

struct Card {
    std::string rel;                   // under root/files, '/' between folders
    std::string name;                  // the file's name
    std::string from;                  // the cabinet it came out of (its rel); empty: straight off the disc
    std::uint64_t size = 0;
    Pile pile = Pile::Data;
    std::string format;
    Conv conv = Conv::None;
    std::string output;                // Done: the folder the conversion wrote
    std::string note;                  // why it could not be converted, or failed
};

// Something of the image that could not be taken out.
struct Failure {
    std::string path;
    std::string why;
};

struct Board {
    std::filesystem::path root;        // the board's folder
    std::string source;                // the image
    std::uint64_t source_size = 0;
    std::int64_t source_time = 0;
    std::uint64_t cabinets = 0;        // cabinets opened
    bool complete = false;             // the whole image was gone through (false: cancelled)
    std::vector<Card> cards;           // sorted by rel
    std::vector<Failure> failures;

    std::filesystem::path file_of(const Card& card) const;
};

// Where the board of `image` lives under `boards`: a folder named for the image, its path, size and time. Another image, or the same one changed, has another.
std::filesystem::path board_folder(const std::filesystem::path& boards, const std::filesystem::path& image);

struct ScanProgress {
    std::string stage;                 // "Extracting the disc", "Opening cabinets"
    std::string detail;                // "1,204 of 2,310"
    int percent = 0;
    std::uint64_t cards = 0;
};
using ScanReport = std::function<void(const ScanProgress&)>;

// Extracts `image` (every volume) under root/files, then every cabinet found, also cabinets inside cabinets, into a folder "<cabinet>.d" beside it, and
// makes a card of every file. A file or cabinet that cannot be read is a Failure and the scan goes on. Throws only when the image cannot be opened at all.
Board scan(const std::filesystem::path& image, const std::filesystem::path& root, const ScanReport& report, const std::atomic<bool>& cancel);

// root/board.tsv and root/state.tsv, each replaced whole.
void save(const Board& board);
// Only the conversion states (cheap: what the window writes after each conversion).
void save_states(const Board& board);
// The board kept in `root`, nullopt when there is none or it was not finished or is of another format. A Done whose output folder is gone or empty is None again.
std::optional<Board> load(const std::filesystem::path& root);

// Applies what a conversion ended with. A Done with no output folder holding a file is refused and becomes Failed: the foundation shows only real results.
void record(Card& card, Conv conv, const std::filesystem::path& output, std::string note);

// What one column shows: the cards still to do (not Done) and the cards on its foundation, each by name. Indexes into Board::cards.
struct Stack {
    std::vector<std::uint32_t> todo, done;
    std::size_t total() const { return todo.size() + done.size(); }
};
using Stacks = std::array<Stack, kPileCount>;
// A cabinet entry whose stored name climbs out of its folder, read by a person and found harmless: exactly this name and these bytes.
struct ReviewedEntry {
    std::string_view name;             // as the cabinet stores it
    std::string_view sha256;
};
// The entries reviewed so far. Microsoft's XML Parser for Java stores its manifest as "..\<name>.osd"; every other package's manifest sits at its root.
std::span<const ReviewedEntry> reviewed_entries();
// `file`, kept contained under `folder`, goes to the root of `folder` when it is a reviewed entry and nothing different is there. The path it ends at.
std::filesystem::path place_reviewed(const std::filesystem::path& folder, std::string_view name, const std::filesystem::path& file, std::span<const ReviewedEntry> reviewed);

// `group`: the first card with the same bytes, per card: identical files become one master card (a converted copy stands for them, else the first).
// `allowed`: only these cards. Both may be null.
struct StackOptions {
    const std::vector<std::uint32_t>* group = nullptr;
    const std::vector<char>* allowed = nullptr;
};
// `query`: only cards whose path holds these words (case ignored); empty: all.
Stacks build_stacks(const Board& board, std::string_view query = {}, const StackOptions& options = {});

// "1.4 MB"
std::string size_words(std::uint64_t bytes);
// "12,345"
std::string count_words(std::uint64_t count);

}  // namespace cb::app::board
