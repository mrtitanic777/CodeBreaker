// codebreaker/symbols/dos_interrupts.hpp -- compat shim for cb/symbols/dos_interrupts.hpp (docs/CPP_DESIGN.md 6.3). No
// logic: conversion and forwarding. Deleted with its last old-layer consumer.
//
//     Service s = dos_interrupts::name(0x21, 0x3D);        s.name "DOS_OpenFile"  s.arguments "AL = access mode, ..."
//     Service s = dos_interrupts::name(0x2F, 0x16, 0x00);  s.name "Multiplex_WindowsEnhancedModeCheck"
//     s.exact        false when only the interrupt (or only its group) could be named
//     text = dos_interrupts::annotate(text, interrupts)    C text with each call's service noted
#pragma once

#include <optional>

#include "cb/compat/legacy_settings.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/knowledge/sources.hpp"
#include "cb/symbols/dos_interrupts.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::symbols::dos_interrupts {

// A service: its name, its register arguments, and whether the service itself was named.
class Service {
public:
    py::str name;
    py::str arguments;
    bool exact = false;

    Service(const py::str& name, const py::str& arguments, bool exact) : name(name), arguments(arguments), exact(exact) {}
    py::str __repr__() const { return py::str("Service(") + name + py::str(exact ? "" : ", not exact") + py::str(")"); }
};

// One `int` instruction of codebreaker.analyze.dos_functions: the fields these functions read of it.
struct Interrupt {
    int64_t number = 0;
    std::optional<int64_t> ah;
    std::optional<int64_t> al;
    int64_t address = 0;
};

namespace detail {

// dos_interrupts.json of the checkout the old layer's settings name.
inline const cb::symbols::dos_interrupts::ServiceTable& table() {
    static const cb::symbols::dos_interrupts::ServiceTable t = cb::symbols::dos_interrupts::ServiceTable::load(
        cb::knowledge::DataFolders::under(cb::compat::legacy::path_of(py::settings::checkout_root())).symbols / "dos_interrupts.json");
    return t;
}

inline cb::symbols::dos_interrupts::Interrupt to_new(const Interrupt& i) {
    using cb::compat::int_from_py;
    cb::symbols::dos_interrupts::Interrupt out;
    out.number = int_from_py<std::uint32_t>(i.number);
    if (i.ah) out.ah = int_from_py<std::uint32_t>(*i.ah);
    if (i.al) out.al = int_from_py<std::uint32_t>(*i.al);
    out.address = int_from_py<std::uint32_t>(i.address);
    return out;
}

}  // namespace detail

// The service an `int number` asks for, given what is known of AH and AL.
inline Service name(int64_t number, std::optional<int64_t> ah = std::nullopt, std::optional<int64_t> al = std::nullopt) {
    using cb::compat::int_from_py;
    const auto s = detail::table().name(int_from_py<std::uint32_t>(number),
                                        ah ? std::optional(int_from_py<std::uint32_t>(*ah)) : std::nullopt,
                                        al ? std::optional(int_from_py<std::uint32_t>(*al)) : std::nullopt);
    return Service(cb::compat::to_py(s.name), cb::compat::to_py(s.arguments), s.exact);
}

// One line for an Interrupt of dos_functions: `int 21h AH=3Dh  DOS_OpenFile(AL = ...)`.
inline py::str describe(const Interrupt& interrupt) {
    return cb::compat::to_py(detail::table().describe(detail::to_new(interrupt)));
}

// A function's C text with its interrupt calls named.
inline py::str annotate(const py::str& text, const py::list<Interrupt>& interrupts) {
    std::vector<cb::symbols::dos_interrupts::Interrupt> calls;
    for (const Interrupt& i : interrupts) calls.push_back(detail::to_new(i));
    return cb::compat::to_py(detail::table().annotate(cb::compat::from_py(text), calls));
}

// python -m codebreaker dos-interrupts [<interrupt> [<AH> [<AL>]]]
inline int64_t main(const py::list<py::str>& argv) {
    const std::vector<std::string> args = cb::compat::from_py(argv);
    return cb::symbols::dos_interrupts::run(detail::table(), args);
}

}  // namespace codebreaker::symbols::dos_interrupts
