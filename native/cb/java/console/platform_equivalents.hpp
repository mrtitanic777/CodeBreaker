// cb/java/console/platform_equivalents.hpp -- the authoritative Java-platform-API -> Xbox 360 XDK map.
//
// The point of this module: NEVER hand-guess a platform port again. For every Java standard-library / LWJGL /
// platform type the converter has to stub, this table names the REAL Xbox 360 XDK equivalent -- the exact
// function(s) and the .lib they live in -- plus whether the port already implements it (via the hand-written
// reference layer) or still needs work.
//
// Two consumers read this table:
//   * the STUB EMITTER (runtime / stub headers): every emitted stub gets a comment
//         `// [360] equivalent: <api>  (<lib>)`   or   `// [360] no equivalent -- <why>`
//     so the answer is right there in the file, not in someone's head;
//   * the COVERAGE REPORT (platform_coverage): PLATFORM_COVERAGE.md lists the XDK equivalent and the status for
//     every platform symbol the game uses, up front.
//
// An entry is keyed by the emitted class/holder BASENAME (no .h). `methods` narrows the equivalent to specific
// calls when the class spans more than one XDK API. Grounded in docs/platform/reference/*, the XDK import table
// (xboxkrnl/xam/xnet/d3d9/xact3/xinput2), and the coverage audit.
#pragma once

#include <optional>
#include <span>
#include <string>
#include <string_view>

namespace cb::java::console::platform_equivalents {

enum class Status {
    Real,   // the port implements it for real (reference layer / runtime) -- ships working
    Stub,   // still a no-op/default, but the XDK equivalent named is the drop-in to implement it
    Dead,   // genuinely no console equivalent / dead code path (applet, minecraft.net): the stub is correct
};

// "real" | "stub" | "dead" (what the report prints), and back; nullopt for any other text.
std::string_view status_name(Status s);
std::optional<Status> parse_status(std::string_view name);

// How an emitted comment (and the report) tells the three cases apart.
enum class Kind {
    Xdk,           // there IS an XDK function to call: resolve the stub to it
    Handwritten,   // NO XDK equivalent exists, so we implement it ourselves (bundled puff.c, a pure C++ byte codec,
                   // a CRT call): the bespoke pieces that must never be lost
    Dead,          // no console equivalent and a dead code path: the stub is correct as it is
};

// An XDK detail for one method of the class.
struct MethodEquiv {
    std::string_view method;
    std::string_view detail;
};

struct Equiv {
    std::string_view basename;           // the emitted class: "GL11", "Socket"
    std::string_view xdk;                // the XDK function(s) that implement it (the thing to resolve to)
    std::string_view lib;                // the .lib / import module the xdk symbol lives in
    Status status;
    std::string_view note;               // one line of why / how (formats, gotchas); empty = none
    std::span<const MethodEquiv> methods;   // per-method equivalents; empty = the class as a whole
};

// A group of entries, in the report's order: "video", "input", "audio", "fileio", "compression", "network",
// "memory", "concurrency", "user", "image".
struct Subsystem {
    std::string_view name;
    std::span<const Equiv> entries;
};

std::span<const Subsystem> subsystems();

// The equivalent for an emitted class basename, or nullptr.
const Equiv* lookup(std::string_view basename);

// An entry's kind: dead if its status says so, else hand-written when it lives in no XDK library ("(none)" -- a pure
// codec --, "(bundled)" -- code we ship --, "(CRT)"), else an XDK function.
Kind kind(Status status, std::string_view lib);
Kind kind(const Equiv& e);
// "xdk" | "handwritten" | "dead" (what the coverage report prints).
std::string_view kind_name(Kind k);

// The one-line C++ comment dropped next to a stub -- three distinct, greppable markers so nothing is missed:
//     // [360] equivalent: <XDK api>  (<lib>)               -> resolve the stub to the named XDK call
//     // [360] NO XDK EQUIVALENT -- hand-implemented: ...    -> bespoke platform code (grep to find them ALL)
//     // [360] no equivalent -- dead on console: ...         -> genuinely dead path; the stub is correct
// "" when there is no mapping (the emitter leaves the stub untouched).
std::string annotation(std::string_view basename);

}  // namespace cb::java::console::platform_equivalents
