// cb/symbols/dos_interrupts.hpp -- names for the services a real-mode program asks of DOS and the BIOS.
//
//     const ServiceTable table = ServiceTable::standard();
//     Service s = table.name(0x21, 0x3D);          s.name "DOS_OpenFile"   s.arguments "AL = access mode, ..."
//     Service s = table.name(0x2F, 0x16, 0x00);    s.name "Multiplex_WindowsEnhancedModeCheck"
//     s.exact        false when only the interrupt (or only its group) could be named
//     table.annotate(text, interrupts)             C text with each call's service noted
//
// A program calls the system with `int n`; which service it wants is a number in AH (for some a second one in AL,
// for the mouse the whole of AX). The table (dos_interrupts.json, written for this project from the documented
// interfaces: interrupt -> AH -> name and register arguments, with an "al" table for services that take a
// subfunction) covers DOS (21h, 20h-2Fh), the BIOS (10h-1Ah), the multiplex interrupt (2Fh), the mouse (33h) and
// expanded memory (67h).
//
// The decompiler shows an interrupt as `swi(0x21)` without the registers, so the service number comes from the
// machine code (the constant loaded before each `int`). annotate() writes what is known in front of each function,
// and inline where every call of that interrupt in the function asks for the same service.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <string_view>

namespace cb::symbols::dos_interrupts {

// A service: its name, its register arguments, and whether the service itself was named.
struct Service {
    std::string name;
    std::string arguments;
    bool exact = false;
};

// One `int` instruction of a function: which interrupt, what is known of AH and AL there, and where.
struct Interrupt {
    std::uint32_t number = 0;
    std::optional<std::uint32_t> ah;
    std::optional<std::uint32_t> al;
    std::uint32_t address = 0;
};

class ServiceTable {
public:
    // The table of dos_interrupts.json. ParseError when the text is not JSON or not the table's shape.
    static ServiceTable parse(std::string_view json_text, std::string_view source = "dos_interrupts.json");
    // The file; IoError / ParseError.
    static ServiceTable load(const std::filesystem::path& file);
    // The project's own file (<checkout>/codebreaker/symbols/dos_interrupts.json, the checkout from CB_CHECKOUT_ROOT
    // or the working folder), read once per process.
    static const ServiceTable& standard();

    // The service an `int number` asks for, given what is known of AH and AL. Always answers: an interrupt outside
    // the table is `int_NNh`, a service outside it `<group>_AH_NNh`; `exact` says whether the service itself was named.
    Service name(std::uint32_t number, std::optional<std::uint32_t> ah = std::nullopt,
                 std::optional<std::uint32_t> al = std::nullopt) const;

    // One line for an Interrupt: `int 21h AH=3Dh  DOS_OpenFile(AL = ...)`.
    std::string describe(const Interrupt& interrupt) const;

    // A function's C text with its interrupt calls named. `interrupts` are those of that function, in address order.
    // A comment in front lists each; `swi(0xNN)` gets its service inline only where all the function's calls of that
    // interrupt ask for the same one.
    std::string annotate(std::string_view text, std::span<const Interrupt> interrupts) const;

    std::size_t interrupt_count() const { return entries_.size(); }
    // Services named: the table's rows, an interrupt with no rows of its own counting as one.
    std::size_t service_count() const;

private:
    struct Hit {
        std::string name;
        std::string arguments;
    };
    struct Entry {
        std::string name;
        std::string arguments;                                          // of an interrupt that is one service
        bool has_services = false;
        std::map<std::string, Hit> services;                            // by AH, two hex digits
        bool select_ax = false;                                         // the mouse: the number is all of AX
        std::map<std::string, std::map<std::string, Hit>> al;           // by AH, then AL
    };
    std::map<std::uint32_t, Entry> entries_;   // by interrupt number: looked up, and counted
};

// `codebreaker dos-interrupts [<interrupt> [<AH> [<AL>]]]`: names a service (numbers in hex), or with no arguments
// says how many are in the table. Returns the exit code.
int run(const ServiceTable& table, std::span<const std::string> args);

}  // namespace cb::symbols::dos_interrupts
