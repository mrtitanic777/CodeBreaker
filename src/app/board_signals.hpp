// app/board_signals.hpp -- the channels files share without importing one another: COM classes, named kernel objects, window messages, hardware resources,
// registry keys and what starts by itself.
//
//     Where a signal comes from                                          confidence
//     a setup script's AddReg, UpdateInis or LogConfig, a .reg file      100   the file says so
//     a driver a script installs, given that script's ports and class     50   the script binds them to the device, not to this one file
//     a name pushed just before a direct call of CreateMutex and kin      70   read from x86 code by pattern, not by a full disassembly
//     mov dx, port followed by in or out in a driver's code               50   the same; a port computed at run time is not seen
//     a GUID or a registry path in a file's bytes                         60   the file holds it; what it does with it is not known
//
// Everything is static. A signal says a file is wired to a channel, never that the channel was used. DelReg and the "old" field of UpdateInis are removals and
// are not read as writes. No operating-system API: tested on every system (board_signals_test.cpp).
#pragma once

#include <array>
#include <atomic>
#include <cstdint>
#include <functional>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "app/board.hpp"
#include "app/board_index.hpp"
#include "core/bytes.hpp"

namespace cb::app::board {

enum class SignalType : std::uint8_t { Clsid, Mutex, Event, Semaphore, Mapping, WindowMessage, IoPort, Irq, Dma, RegKey, Autostart, HwClass };
inline constexpr std::size_t kSignalTypes = 12;
std::string_view signal_type_name(SignalType type);

enum class Direction : std::uint8_t { Reads, Writes, Registers, Listens, Mentions };
std::string_view direction_name(Direction direction);

struct Signal {
    SignalType type = SignalType::RegKey;
    std::string value;                 // canonical: lower-case key, "{guid}", "0x3f8", "irq 5"
};

struct NodeSignal {
    std::uint32_t node = 0;            // Board::cards
    std::uint32_t signal = 0;
    Direction direction = Direction::Mentions;
    std::uint8_t confidence = 0;
};

struct Signals {
    std::vector<Signal> signals;       // sorted by (type, value)
    std::vector<NodeSignal> links;     // sorted by (node, signal, direction)
    std::vector<std::uint32_t> node_start, signal_start, signal_links;   // rebuilt by finish()
    void finish(std::size_t cards);
    std::span<const NodeSignal> of_node(std::uint32_t card) const;
    std::span<const std::uint32_t> of_signal(std::uint32_t signal) const;   // positions in `links`
};

// ---- the parts, each testable alone

struct RegWrite {
    std::string key;                   // "hklm\software\..." in lower case
    std::string data;                  // the value's data as written; may be empty
};
struct IniWrite {
    std::string file, section, entry;  // "system.ini", "386enh", "device=x.386"
};
struct SetupFacts {
    std::vector<RegWrite> registry;
    std::vector<IniWrite> inis;
    std::vector<std::uint32_t> io_ports;   // the first port of each range
    std::vector<std::uint32_t> irqs, dmas;
    std::string device_class;          // [Version] Class=, lower case
};
SetupFacts parse_inf(std::string_view script);
std::vector<RegWrite> parse_reg(std::string_view text);
// One comma-separated INF line: quotes keep commas, and are removed.
std::vector<std::string> inf_fields(std::string_view line);

// Printable runs of at least `min_length`, ASCII and UTF-16LE.
void for_each_string(ByteView data, std::size_t min_length, const std::function<void(std::string_view)>& each);
// Every "{xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}" in a text, lower case.
std::vector<std::string> guids_in_text(std::string_view text);
// The 16 stored bytes of a GUID as a COM structure holds them.
std::optional<std::array<std::uint8_t, 16>> guid_bytes(std::string_view guid);
// A registry path at the start of a string, in lower case; empty when it is none.
std::string registry_path(std::string_view text);

struct NamedObject {
    SignalType type = SignalType::Mutex;
    Direction direction = Direction::Writes;
    std::string name;
};
// Names passed to CreateMutex, OpenEvent, CreateFileMapping, RegisterWindowMessage and kin in a 32-bit PE image, through a direct call or an import thunk.
std::vector<NamedObject> named_objects(ByteView image);

struct PortUse {
    std::uint16_t port = 0;
    bool writes = false;
};
// `mov dx, port` (or edx) followed within a few bytes by in or out.
std::vector<PortUse> ports_in_code(ByteView code);

// ---- the whole board

using SignalReport = std::function<void(const IndexProgress&)>;
std::optional<Signals> build_signals(const Board& board, const Index& index, const SignalReport& report, const std::atomic<bool>& cancel);
// root/signals.tsv, replaced whole.
void save_signals(const Board& board, const Signals& signals);
// The signals kept for exactly these cards; nullopt otherwise.
std::optional<Signals> load_signals(const Board& board);

}  // namespace cb::app::board
