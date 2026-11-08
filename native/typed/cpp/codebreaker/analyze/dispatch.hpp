// codebreaker/analyze/dispatch.hpp -- interface of codebreaker.analyze.dispatch (typed C++; implementation pending).
//
// dispatch.py -- how code chooses what to do: jump tables, compare chains, message and command dispatch, keyword
// dispatch, and the system services a function asks for by number.
//
//     dispatch::switches(img)          [{function, on, cases: {value: target}, default, table}]
//     dispatch::messages(img)          window/dialog procedures: {message name: handler address}
//     dispatch::keywords(img)          functions comparing a string against a list of literals
//     dispatch::interrupts(img)        DOS, BIOS and DPMI services requested with `int`
//     dispatch::control_messages(img)  a driver's control procedure: which system messages it handles
//
// A switch reaches the binary in three shapes, all read into one {case value: where it goes}: a jump table, a compare
// chain (`cmp eax, 0x111 / je ...`) and a subtract chain (`sub eax, 5 / je ... dec eax / je ...`). `on` says what is
// switched on when that is in sight: an argument, a global, a field. A switch on the message argument of a window or
// dialog procedure is a message dispatch and its cases get their WM_ names; a switch on wParam inside one is the
// command dispatch.
//
// Typing notes:
//   * A switch's `key` is a tuple of mixed length in the Python: ("r", family) for a register, ("m", base, disp) or
//     ("m", base, index, disp) for memory (base/index registers are families; base None for an absolute address).
//     `SwitchKey` holds the three shapes (`index` is absent for the 3-tuple, which is_message_argument tests through
//     len(key) == 3); it has ==, < and a std::hash because the Python uses keys in sets and as dict keys.
//   * The records Switch, MessageDispatch, Keyword, Interrupt, ControlMessages are the Python's dicts / tuples. The
//     cases of a switch are {value: target} in insertion order (a py::dict keeps it).
//   * analyse() reports five kinds of fact: a std::variant of five records; counts is a dict of ints.
#pragma once

#include <cstdint>
#include <optional>
#include <variant>

#include "codebreaker/analyze/image.hpp"
#include "py.hpp"

namespace codebreaker::analyze::dispatch {
struct SwitchKey;
}
namespace std {
template <> struct hash<codebreaker::analyze::dispatch::SwitchKey>;   // (defined below; dict keys need it)
}

namespace codebreaker::analyze::dispatch {

// message number -> WM_ name
extern const py::dict<int64_t, py::str> MESSAGES;
// a driver's control messages: number -> name (Sys_Critical_Init ...)
extern const py::dict<int64_t, py::str> CONTROL_MESSAGES;

// What a compare, a table index or a subtracted register is a piece of: see the typing notes.
struct SwitchKey {
    py::str kind;                        // "r" | "m"
    std::optional<py::str> base;         // "r": the register's family; "m": the base register's family or None
    std::optional<py::str> index;        // "m": the index register's family (the 4-tuple shape)
    int64_t disp = 0;                    // "m"
    bool operator==(const SwitchKey& o) const;
    bool operator!=(const SwitchKey& o) const { return !(*this == o); }
    bool operator<(const SwitchKey& o) const;    // (the Python sorts / reprs keys; repr order)
};

// _origin(): what an operand really is -> a hashable key and a text
std::tuple<SwitchKey, py::str> _origin(const image::Function& fn, int64_t i, const image::Op& op);

// {"on": text, "cases": {value: target}} of one chain
struct Chain {
    py::str on;
    py::dict<int64_t, int64_t> cases;
};
// {key: chain} from the compare and subtract chains of one function.
py::dict<SwitchKey, Chain> _chains(image::Image& img, const image::Function& fn);

// One switch: {function, on, key, cases, default, table, site, bounded}
struct Switch {
    int64_t function = 0;
    py::str on;
    std::optional<SwitchKey> key;                  // None when the selector was not found in sight
    py::dict<int64_t, int64_t> cases;              // value -> target
    std::optional<int64_t> default_;               // where an out-of-range value goes (None for a compare chain)
    std::optional<int64_t> table;                  // the jump table's address (None for a chain)
    std::optional<int64_t> site;                   // the indirect jump
    bool bounded = true;
};
std::shared_ptr<py::list<Switch>> _switches(image::Image& img);
std::shared_ptr<py::list<Switch>> switches(image::Image& img);       // memo "dispatch.switches"

bool _is_message_argument(const image::Image& img, const std::optional<SwitchKey>& key, bool far);
bool _is_command_argument(const image::Image& img, const std::optional<SwitchKey>& key, bool far);

// One window/dialog procedure's dispatch: {function, procedure, why, messages, commands}
struct MessageDispatch {
    int64_t function = 0;
    std::optional<py::str> procedure;              // "window procedure" ... (the convention fact's `procedure`)
    py::str why;
    py::dict<int64_t, int64_t> messages;           // message number -> handler
    py::dict<int64_t, int64_t> commands;           // command id -> handler
};
std::shared_ptr<py::list<MessageDispatch>> _messages(image::Image& img);
std::shared_ptr<py::list<MessageDispatch>> messages(image::Image& img);   // memo "dispatch.messages"

// A function that compares a string against literals (function set, compares_with, keywords), or a table in the data
// pairing strings with functions (table set, keywords, handlers).
struct Keyword {
    std::optional<int64_t> function;
    std::optional<py::str> compares_with;
    std::optional<int64_t> table;
    py::list<py::str> keywords;
    std::optional<py::list<int64_t>> handlers;
};
std::shared_ptr<py::list<Keyword>> _keywords(image::Image& img);
std::shared_ptr<py::list<Keyword>> keywords(image::Image& img);          // memo "dispatch.keywords"

// (function, site, interrupt, function number or None, name) for one `int` instruction
struct Interrupt {
    int64_t function = 0;
    int64_t site = 0;
    int64_t interrupt = 0;
    std::optional<int64_t> code;
    py::str name;
};
// DOS, BIOS and DPMI services requested with `int`.
py::list<Interrupt> interrupts(image::Image& img);

// A driver's control procedure: {procedure, handles: {message name: handler address}}
struct ControlMessages {
    int64_t procedure = 0;
    py::dict<py::str, int64_t> handles;
};
// None unless the image is a driver with a device descriptor block whose control procedure is a known function.
std::optional<ControlMessages> control_messages(image::Image& img);

// ---- analyse() ----
struct SwitchFact {                                // kind "switch"
    int64_t addr = 0;
    py::str kind = py::str("switch");
    py::str on;
    std::optional<int64_t> site, table, default_;
    py::dict<py::str, int64_t> cases;              // "0x111" -> target
};
struct MessageFact {                               // kind "message_dispatch"
    int64_t addr = 0;
    py::str kind = py::str("message_dispatch");
    std::optional<py::str> procedure;
    py::str why;
    py::dict<py::str, int64_t> messages;           // WM_ name (or "0x0123") -> handler
    py::dict<py::str, int64_t> commands;           // "123" -> handler
};
struct KeywordFact {                               // kind "keyword_dispatch": the Keyword plus addr (= its function)
    std::optional<int64_t> addr;
    py::str kind = py::str("keyword_dispatch");
    Keyword keyword;
};
struct ServicesFact {                              // kind "system_services"
    int64_t addr = 0;
    py::str kind = py::str("system_services");
    py::list<py::str> services;
};
struct DriverControlFact {                         // kind "driver_control"
    int64_t addr = 0;
    py::str kind = py::str("driver_control");
    std::optional<py::str> device;
    py::dict<py::str, int64_t> handles;
};
using AnyFact = std::variant<SwitchFact, MessageFact, KeywordFact, ServicesFact, DriverControlFact>;
struct Analysis {
    py::dict<py::str, int64_t> counts;
    py::list<AnyFact> facts;
    py::list<py::str> text;
};
Analysis analyse(image::Image& img);

}  // namespace codebreaker::analyze::dispatch

namespace std {
template <> struct hash<codebreaker::analyze::dispatch::SwitchKey> {
    size_t operator()(const codebreaker::analyze::dispatch::SwitchKey& k) const;
};
}  // namespace std
