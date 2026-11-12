// cb/analyze/dispatch.hpp -- how code chooses what to do: jump tables, compare chains, message and command dispatch,
// keyword dispatch, and the system services a function asks for by number.
//
//     Image image = image::load_pe(...);
//     auto sw   = dispatch::switches(image);                   every switch: {function, on, cases, default, table}
//     auto msgs = dispatch::messages(image, sw, conventions);  window/dialog procedures: {message: handler}
//     auto kw   = dispatch::keywords(image);                   functions comparing a string against a list of literals
//     auto ints = dispatch::interrupts(image);                 DOS, BIOS and DPMI services requested with `int`
//     auto ctl  = dispatch::control_messages(image, sw, device);   a driver's control procedure: its system messages
//     Report r  = dispatch::analyse(image, conventions, device);
//
// A switch reaches the binary in three shapes, all read into one {case value: where it goes}:
//
//     a jump table        `cmp eax, 7 / ja default / jmp [table + eax*4]`, with or without a byte table in front and a
//                         `sub` that moves the first case to zero
//     a compare chain     `cmp eax, 0x111 / je ...  cmp eax, 0x110 / je ...`
//     a subtract chain    `sub eax, 5 / je ...  dec eax / je ...  sub eax, 0x10 / je ...`  (cases 5, 6, 0x16)
//
// `on` says what is switched on when that is in sight: an argument, a global, a field. A switch on the message
// argument of a window or dialog procedure (the second argument; a 16-bit procedure takes hwnd, message, wParam,
// lParam at bp+0Eh, 0Ch, 0Ah, 6) is a message dispatch and its cases get their WM_ names; so is a switch whose cases
// are nearly all message numbers, several of them above the small integers and one of them among the handful every
// window handles. A switch on wParam inside one is the command dispatch: its cases are menu and control identifiers.
//
// Which procedure a function is (a window procedure, a dialog procedure ...) and whether it is far is the calling
// convention pass's finding (analyze/conventions); this module is handed it as `ConventionFacts` and does not call it.
//
// An Image decodes its functions lazily, so these take it by non-const reference and are not thread-safe on one image.
#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "cb/analyze/convention_facts.hpp"
#include "cb/analyze/native.hpp"
#include "cb/core/ordered_map.hpp"

namespace cb::analyze::dispatch {

// The WM_ name of a message number, and the name of a driver's control message (Sys_Critical_Init ...).
std::optional<std::string_view> message_name(Va message);
std::optional<std::string_view> control_message_name(Va message);

// What a compare, a table index or a subtracted register is a piece of.
struct SwitchKey {
    enum class Kind { Register, Memory };
    Kind kind = Kind::Register;
    std::string base;                    // Register: the register's whole name; Memory: the base register's ("" none)
    std::optional<std::string> index;    // Memory: the index register's; none for a plain base+displacement
    Va disp = 0;                         // Memory: the displacement (the address when there is no base)

    friend bool operator==(const SwitchKey&, const SwitchKey&) = default;
    friend auto operator<=>(const SwitchKey&, const SwitchKey&) = default;
};
struct SwitchKeyHash {
    std::size_t operator()(const SwitchKey& key) const;
};

// One switch.
struct Switch {
    Va function = 0;
    std::string on;                      // what is switched on, as text: "eax", "[ebp+0xc]", "[0x403000]"
    std::optional<SwitchKey> key;        // none when the selector was not found in sight
    std::map<Va, Va> cases;              // value -> target
    std::optional<Va> default_target;    // where an out-of-range value goes (none for a compare chain)
    std::optional<Va> table;             // the jump table's address (none for a chain)
    std::optional<Va> site;              // the indirect jump
    bool bounded = true;                 // is the number of entries known?
};

// A chain found in one function: what is compared and the case values it tests, first test of a value wins.
struct Chain {
    std::string on;
    std::map<Va, Va> cases;
};
using Chains = OrderedMap<SwitchKey, Chain, SwitchKeyHash>;   // in the order the chains are first met

// The compare and subtract chains of one decoded function, by what they test.
Chains chains(const Function& function);

// Every switch of the module, by function and in the order the function's tables, then its chains, come.
std::vector<Switch> switches(Image& image);

// One window/dialog procedure's dispatch.
struct MessageDispatch {
    Va function = 0;
    std::optional<std::string> procedure;
    std::string why;                     // how it was told to be a message dispatch
    std::map<Va, Va> messages;           // message number -> handler
    std::map<Va, Va> commands;           // command id -> handler
};
// The procedures that dispatch on messages, by function. Not for a driver (an LE image).
std::vector<MessageDispatch> messages(Image& image, const std::vector<Switch>& switches, const ConventionFacts& conventions);

// A function that compares a string against literals.
struct KeywordFunction {
    Va function = 0;
    std::string compares_with;           // the first comparing API ("lstrcmpi")
    std::vector<std::string> keywords;   // in the order they are compared, repeats included
};
// A table in the data pairing strings with functions: string, handler, string, handler ...
struct KeywordTable {
    Va table = 0;
    std::vector<std::string> keywords;
    std::vector<Va> handlers;
};
struct Keywords {
    std::vector<KeywordFunction> functions;
    std::vector<KeywordTable> tables;
};
Keywords keywords(Image& image);

// One `int` instruction.
struct Interrupt {
    Va function = 0;
    Va site = 0;
    std::uint8_t number = 0;
    std::optional<std::uint32_t> code;   // the service number, where a constant was loaded before the `int`
    std::string name;                    // "DOS: open file", "DPMI: get version", "video function 0Eh", "interrupt 5Fh"
    bool named = false;                  // is the service itself known? (a BIOS interrupt outside the table, or a DOS
                                         // or DPMI call whose number is not in sight, is not)
};
// DOS, BIOS and DPMI services requested with `int`, in function and instruction order.
std::vector<Interrupt> interrupts(Image& image);

// A driver's device descriptor block, as far as dispatch needs it.
struct DeviceBlock {
    Va descriptor = 0;                   // its address in the image
    std::string name;                    // the device's name
};

struct HandledMessage {
    Va value = 0;
    std::string name;                    // Sys_Critical_Init, or "message 0x..." for one that has no name
    Va handler = 0;
};
struct ControlMessages {
    Va procedure = 0;
    std::vector<HandledMessage> handles; // by message number
};
// The control procedure of a driver and the system messages it handles; none for another image, a driver with no
// device block, or a control procedure that is not a known function.
std::optional<ControlMessages> control_messages(Image& image, const std::vector<Switch>& switches,
                                                const std::optional<DeviceBlock>& device);

// ---- the report ---------------------------------------------------------------------------------------------------

struct Counts {
    std::int64_t jump_tables = 0;
    std::int64_t jump_table_cases = 0;
    std::int64_t jump_tables_with_bound = 0;
    std::int64_t compare_chains = 0;
    std::int64_t compare_chain_cases = 0;
    std::int64_t switches_on_an_argument = 0;
    std::int64_t message_dispatchers = 0;
    std::int64_t messages_handled = 0;
    std::int64_t named_messages = 0;
    std::int64_t command_dispatchers = 0;
    std::int64_t command_ids = 0;
    std::int64_t keyword_dispatchers = 0;
    std::int64_t keywords = 0;
    std::int64_t interrupt_sites = 0;
    std::int64_t named_services = 0;
    std::int64_t driver_control_messages = 0;
};

struct SwitchFact {                      // kind "switch"
    Va function = 0;
    std::string on;
    std::optional<Va> site, table, default_target;
    std::vector<std::pair<Va, Va>> cases;                // value -> target, by value
};
struct MessageFact {                     // kind "message_dispatch"
    Va function = 0;
    std::optional<std::string> procedure;
    std::string why;
    std::vector<HandledMessage> messages;                // by number; the name is the WM_ name, or "0x0123" without one
    std::vector<std::pair<Va, Va>> commands;             // command id -> handler, by id
};
struct KeywordFunctionFact {             // kind "keyword_dispatch", from a function
    KeywordFunction keyword;
};
struct KeywordTableFact {                // kind "keyword_dispatch", from a table in the data
    KeywordTable keyword;
};
struct ServicesFact {                    // kind "system_services"
    Va function = 0;
    std::vector<std::string> services;                   // sorted, once each
};
struct DriverControlFact {               // kind "driver_control"
    Va procedure = 0;
    std::optional<std::string> device;
    std::vector<HandledMessage> handles;
};
using Fact = std::variant<SwitchFact, MessageFact, KeywordFunctionFact, KeywordTableFact, ServicesFact, DriverControlFact>;

struct Report {
    Counts counts;
    std::vector<Fact> facts;
    std::vector<std::string> text;       // a few lines saying what was found
};

// Everything above, as the analysis pass reports it.
Report analyse(Image& image, const ConventionFacts& conventions, const std::optional<DeviceBlock>& device = std::nullopt);

}  // namespace cb::analyze::dispatch
