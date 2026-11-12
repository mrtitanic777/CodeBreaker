// codebreaker/analyze/dispatch.hpp -- COMPAT SHIM over cb/analyze/dispatch.hpp (docs/CPP_DESIGN.md 6.3), for the callers that
// still hold an old analyze::image::Image (analyze/classes reads MESSAGES; the analysis pass reads analyse()). Deleted with its
// last py:: consumer. Conversion and forwarding only: the analysis is native/cb/analyze/dispatch.hpp, over the cb Image the
// bridge (analyze/cb_image.hpp) makes of the old one.
//
// dispatch.py -- how code chooses what to do: jump tables, compare chains, message and command dispatch, keyword dispatch,
// and the system services a function asks for by number.
//
//     dispatch::switches(img)          [{function, on, cases: {value: target}, default, table}]
//     dispatch::messages(img)          window/dialog procedures: {message name: handler address}
//     dispatch::keywords(img)          functions comparing a string against a list of literals
//     dispatch::interrupts(img)        DOS, BIOS and DPMI services requested with `int`
//     dispatch::control_messages(img)  a driver's control procedure: which system messages it handles
//
// What differs from the port it replaces: the inner steps (`_origin`, `_chains`, `_is_message_argument`, `_is_command_argument`)
// are not here (nothing called them); a switch's cases are by value, not in the order they were met; a memory selector with
// an index but no base is described "[eax*4+0x403000]", not "[None+eax*4+0x403000]"; `named_services` counts every service the
// tables know (the port left out those whose name contains the word "interrupt ").
#pragma once

#include <cstdint>
#include <format>
#include <memory>
#include <optional>
#include <variant>

#include "cb/analyze/dispatch.hpp"
#include "cb/analyze/image.hpp"
#include "codebreaker/analyze/cb_image.hpp"
#include "codebreaker/analyze/image.hpp"
#include "py.hpp"

namespace codebreaker::analyze::dispatch {

namespace ns = ::cb::analyze::dispatch;

namespace shim_detail {
// message number -> name, for the numbers the tables know
template <class Name>
py::dict<int64_t, py::str> names_up_to(int64_t last, Name name) {
    py::dict<int64_t, py::str> out;
    for (int64_t v = 0; v <= last; ++v) {
        if (const auto n = name(v)) out.set(v, ::cb::compat::to_py(*n));
    }
    return out;
}
}  // namespace shim_detail

// message number -> WM_ name
inline const py::dict<int64_t, py::str> MESSAGES = shim_detail::names_up_to(0x400, [](int64_t v) { return ns::message_name(v); });
// a driver's control messages: number -> name (Sys_Critical_Init ...)
inline const py::dict<int64_t, py::str> CONTROL_MESSAGES = shim_detail::names_up_to(0x40, [](int64_t v) { return ns::control_message_name(v); });

// What a compare, a table index or a subtracted register is a piece of: ("r", family) | ("m", base, disp) | ("m", base, index, disp).
struct SwitchKey {
    py::str kind;                        // "r" | "m"
    std::optional<py::str> base;         // "r": the register's family; "m": the base register's family or None
    std::optional<py::str> index;        // "m": the index register's family (the 4-tuple shape)
    int64_t disp = 0;                    // "m"
    bool operator==(const SwitchKey& o) const { return kind == o.kind && base == o.base && index == o.index && disp == o.disp; }
    bool operator!=(const SwitchKey& o) const { return !(*this == o); }
};

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

// One window/dialog procedure's dispatch: {function, procedure, why, messages, commands}
struct MessageDispatch {
    int64_t function = 0;
    std::optional<py::str> procedure;
    py::str why;
    py::dict<int64_t, int64_t> messages;           // message number -> handler
    py::dict<int64_t, int64_t> commands;           // command id -> handler
};

// A function that compares a string against literals (function set, compares_with, keywords), or a table in the data pairing
// strings with functions (table set, keywords, handlers).
struct Keyword {
    std::optional<int64_t> function;
    std::optional<py::str> compares_with;
    std::optional<int64_t> table;
    py::list<py::str> keywords;
    std::optional<py::list<int64_t>> handlers;
};

// (function, site, interrupt, function number or None, name) for one `int` instruction
struct Interrupt {
    int64_t function = 0;
    int64_t site = 0;
    int64_t interrupt = 0;
    std::optional<int64_t> code;
    py::str name;
};

// A driver's control procedure: {procedure, handles: {message name: handler address}}
struct ControlMessages {
    int64_t procedure = 0;
    py::dict<py::str, int64_t> handles;
};

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

namespace shim_detail {

inline py::str s(const std::string& v) { return ::cb::compat::to_py(v); }

inline SwitchKey key_to_old(const ns::SwitchKey& k) {
    SwitchKey out;
    out.disp = k.disp;
    if (k.kind == ns::SwitchKey::Kind::Register) {
        out.kind = py::str("r");
        out.base = s(k.base);
    } else {
        out.kind = py::str("m");
        if (!k.base.empty()) out.base = s(k.base);
        if (k.index) out.index = s(*k.index);
    }
    return out;
}

template <class Map>
py::dict<int64_t, int64_t> cases_to_old(const Map& m) {
    py::dict<int64_t, int64_t> out;
    for (const auto& [v, target] : m) out.set(v, target);
    return out;
}

// The cb image's switches, worked out once per old image.
inline const std::vector<ns::Switch>& cb_switches(image::Image& img) {
    return *img.memo<std::vector<ns::Switch>>(py::str("cb.dispatch.switches"),
                                              [](image::Image& i) { return std::make_shared<std::vector<ns::Switch>>(ns::switches(bridge::cb_image_of(i))); });
}

// A driver's device descriptor block, as dispatch takes it.
inline std::optional<ns::DeviceBlock> device_of(const image::Image& img) {
    if (!img.device) return std::nullopt;
    return ns::DeviceBlock{::cb::analyze::image::le_address(static_cast<int>(img.device->obj), static_cast<std::uint32_t>(img.device->rva)),
                           img.device->name.utf8()};
}

inline Keyword keyword_to_old(const ns::KeywordFunction& k) {
    Keyword out;
    out.function = k.function;
    out.compares_with = s(k.compares_with);
    for (const std::string& w : k.keywords) out.keywords.append(s(w));
    return out;
}
inline Keyword keyword_to_old(const ns::KeywordTable& k) {
    Keyword out;
    out.table = k.table;
    for (const std::string& w : k.keywords) out.keywords.append(s(w));
    py::list<int64_t> handlers;
    for (int64_t h : k.handlers) handlers.append(h);
    out.handlers = handlers;
    return out;
}

inline py::dict<py::str, int64_t> handles_to_old(const std::vector<ns::HandledMessage>& hs) {
    py::dict<py::str, int64_t> out;
    for (const ns::HandledMessage& h : hs) out.set(s(h.name), h.handler);
    return out;
}

}  // namespace shim_detail

// Every switch of the module (memo "dispatch.switches").
inline std::shared_ptr<py::list<Switch>> switches(image::Image& img) {
    return img.memo<py::list<Switch>>(py::str("dispatch.switches"), [](image::Image& i) {
        auto out = std::make_shared<py::list<Switch>>();
        for (const ns::Switch& sw : shim_detail::cb_switches(i)) {
            Switch o;
            o.function = sw.function;
            o.on = shim_detail::s(sw.on);
            if (sw.key) o.key = shim_detail::key_to_old(*sw.key);
            o.cases = shim_detail::cases_to_old(sw.cases);
            o.default_ = sw.default_target;
            o.table = sw.table;
            o.site = sw.site;
            o.bounded = sw.bounded;
            out->append(o);
        }
        return out;
    });
}
inline std::shared_ptr<py::list<Switch>> _switches(image::Image& img) { return switches(img); }

// Window/dialog procedures (memo "dispatch.messages").
inline std::shared_ptr<py::list<MessageDispatch>> messages(image::Image& img) {
    return img.memo<py::list<MessageDispatch>>(py::str("dispatch.messages"), [](image::Image& i) {
        auto out = std::make_shared<py::list<MessageDispatch>>();
        for (const ns::MessageDispatch& m : ns::messages(bridge::cb_image_of(i), shim_detail::cb_switches(i), bridge::convention_facts_of(i))) {
            MessageDispatch o;
            o.function = m.function;
            if (m.procedure) o.procedure = shim_detail::s(*m.procedure);
            o.why = shim_detail::s(m.why);
            o.messages = shim_detail::cases_to_old(m.messages);
            o.commands = shim_detail::cases_to_old(m.commands);
            out->append(o);
        }
        return out;
    });
}
inline std::shared_ptr<py::list<MessageDispatch>> _messages(image::Image& img) { return messages(img); }

// Functions comparing a string against a list of literals, and tables pairing strings with functions (memo "dispatch.keywords").
inline std::shared_ptr<py::list<Keyword>> keywords(image::Image& img) {
    return img.memo<py::list<Keyword>>(py::str("dispatch.keywords"), [](image::Image& i) {
        auto out = std::make_shared<py::list<Keyword>>();
        const ns::Keywords found = ns::keywords(bridge::cb_image_of(i));
        for (const ns::KeywordFunction& k : found.functions) out->append(shim_detail::keyword_to_old(k));
        for (const ns::KeywordTable& k : found.tables) out->append(shim_detail::keyword_to_old(k));
        return out;
    });
}
inline std::shared_ptr<py::list<Keyword>> _keywords(image::Image& img) { return keywords(img); }

// DOS, BIOS and DPMI services requested with `int`.
inline py::list<Interrupt> interrupts(image::Image& img) {
    py::list<Interrupt> out;
    for (const ns::Interrupt& i : ns::interrupts(bridge::cb_image_of(img))) {
        Interrupt o;
        o.function = i.function;
        o.site = i.site;
        o.interrupt = i.number;
        if (i.code) o.code = static_cast<int64_t>(*i.code);
        o.name = shim_detail::s(i.name);
        out.append(o);
    }
    return out;
}

// A driver's control procedure: None unless the image is a driver with a device descriptor block whose control procedure is a
// known function.
inline std::optional<ControlMessages> control_messages(image::Image& img) {
    const auto found = ns::control_messages(bridge::cb_image_of(img), shim_detail::cb_switches(img), shim_detail::device_of(img));
    if (!found) return std::nullopt;
    ControlMessages out;
    out.procedure = found->procedure;
    out.handles = shim_detail::handles_to_old(found->handles);
    return out;
}

inline Analysis analyse(image::Image& img) {
    const ns::Report r = ns::analyse(bridge::cb_image_of(img), bridge::convention_facts_of(img), shim_detail::device_of(img));
    Analysis out;
    const ns::Counts& c = r.counts;
    out.counts.set(py::str("jump_tables"), c.jump_tables);
    out.counts.set(py::str("jump_table_cases"), c.jump_table_cases);
    out.counts.set(py::str("jump_tables_with_bound"), c.jump_tables_with_bound);
    out.counts.set(py::str("compare_chains"), c.compare_chains);
    out.counts.set(py::str("compare_chain_cases"), c.compare_chain_cases);
    out.counts.set(py::str("switches_on_an_argument"), c.switches_on_an_argument);
    out.counts.set(py::str("message_dispatchers"), c.message_dispatchers);
    out.counts.set(py::str("messages_handled"), c.messages_handled);
    out.counts.set(py::str("named_messages"), c.named_messages);
    out.counts.set(py::str("command_dispatchers"), c.command_dispatchers);
    out.counts.set(py::str("command_ids"), c.command_ids);
    out.counts.set(py::str("keyword_dispatchers"), c.keyword_dispatchers);
    out.counts.set(py::str("keywords"), c.keywords);
    out.counts.set(py::str("interrupt_sites"), c.interrupt_sites);
    out.counts.set(py::str("named_services"), c.named_services);
    out.counts.set(py::str("driver_control_messages"), c.driver_control_messages);
    for (const ns::Fact& fact : r.facts) {
        std::visit(
            [&](const auto& f) {
                using T = std::decay_t<decltype(f)>;
                if constexpr (std::is_same_v<T, ns::SwitchFact>) {
                    SwitchFact o;
                    o.addr = f.function;
                    o.on = shim_detail::s(f.on);
                    o.site = f.site;
                    o.table = f.table;
                    o.default_ = f.default_target;
                    for (const auto& [v, target] : f.cases) o.cases.set(::cb::compat::to_py(std::format("{:#x}", v)), target);
                    out.facts.append(AnyFact(o));
                } else if constexpr (std::is_same_v<T, ns::MessageFact>) {
                    MessageFact o;
                    o.addr = f.function;
                    if (f.procedure) o.procedure = shim_detail::s(*f.procedure);
                    o.why = shim_detail::s(f.why);
                    for (const ns::HandledMessage& m : f.messages) o.messages.set(shim_detail::s(m.name), m.handler);
                    for (const auto& [id, handler] : f.commands) o.commands.set(::cb::compat::to_py(std::format("{}", id)), handler);
                    out.facts.append(AnyFact(o));
                } else if constexpr (std::is_same_v<T, ns::KeywordFunctionFact>) {
                    KeywordFact o;
                    o.addr = f.keyword.function;
                    o.keyword = shim_detail::keyword_to_old(f.keyword);
                    out.facts.append(AnyFact(o));
                } else if constexpr (std::is_same_v<T, ns::KeywordTableFact>) {
                    KeywordFact o;
                    o.keyword = shim_detail::keyword_to_old(f.keyword);
                    out.facts.append(AnyFact(o));
                } else if constexpr (std::is_same_v<T, ns::ServicesFact>) {
                    ServicesFact o;
                    o.addr = f.function;
                    for (const std::string& name : f.services) o.services.append(shim_detail::s(name));
                    out.facts.append(AnyFact(o));
                } else {
                    DriverControlFact o;
                    o.addr = f.procedure;
                    if (f.device) o.device = shim_detail::s(*f.device);
                    o.handles = shim_detail::handles_to_old(f.handles);
                    out.facts.append(AnyFact(o));
                }
            },
            fact);
    }
    for (const std::string& line : r.text) out.text.append(shim_detail::s(line));
    return out;
}

}  // namespace codebreaker::analyze::dispatch

namespace std {
template <> struct hash<codebreaker::analyze::dispatch::SwitchKey> {
    size_t operator()(const codebreaker::analyze::dispatch::SwitchKey& k) const {
        size_t h = std::hash<std::string>()(k.kind.utf8());
        h = h * 31 + (k.base ? std::hash<std::string>()(k.base->utf8()) : 0);
        h = h * 31 + (k.index ? std::hash<std::string>()(k.index->utf8()) + 1 : 0);
        return h * 31 + std::hash<int64_t>()(k.disp);
    }
};
}  // namespace std
