// cb/exact/emu/native_dosexec.hpp -- finding and starting the DOS executive: where its two DLLs are, what it is run under, and
// why it cannot be used when it cannot.
//
//     auto opened = native_dosexec::open(native_dosexec::locate(config), native_dosexec::limits_from_environment());
//     if (opened.usable()) opened.executive->run(spec);                  // emu/dos.hpp: the executive itself
//     else for (const std::string& reason : opened.why) log::warn(reason);
//
// The executive (PSP, MCB arena, program load, INT 20h/21h over the MemFS or the disk, EXEC, the console and clock) and its DPMI
// host are C++ in native/dosexec (cbdos.dll, running on unicorn.dll); `dos::Executive` (emu/dos.hpp) is the program's view of them.
// What is here is what surrounds it: the switch that turns it off, the two places to look for the DLLs, the knobs it has always
// been run under, and a start that reports a DLL that cannot be used as a reason, not as a crash.
#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "cb/base/config.hpp"
#include "cb/exact/emu/dos.hpp"

namespace cb::exact::emu::native_dosexec {

// CB_NATIVE_DOSEXEC is 0, off, no or false (any case): the executive is not to be used (a byte-match spot-check on another
// executive, a test of the fallback).
bool switched_off();

// The limits the executive is run under: DOSRUN_DPMI_LIMIT (seconds a run with a DPMI host may take, default 300), DOSRUN_DRIVE
// (INT 21h 19h's answer, default 2: C) and DOSRUN_DEBUG (any value: the executive traces). A value that is not a number is ignored.
dos::Limits limits_from_environment(double fast_limit = 20);

struct Places {
    std::filesystem::path cbdos, unicorn;
};
// Where the DLLs are: cbdos.dll is CB_CBDOS_DLL, else `dosexec_bin` of the configuration (default native/dosexec/build/bin of the
// checkout) with cbdos.dll; unicorn.dll is CB_UNICORN_DLL, else native/cb/third_party/bin/unicorn.dll of the checkout.
Places locate(const config::Config& config);

struct Opened {
    std::unique_ptr<dos::Executive> executive;   // none when it cannot be used here
    std::vector<std::string> why;                // the reasons, when it cannot
    bool usable() const { return executive != nullptr; }
};
// Start the executive. A DLL that is missing or is not the executive is a reason (the first, in `why`); nothing is thrown for
// it. When it is switched off the reason says so and no DLL is loaded.
Opened open(const Places& places, const dos::Limits& limits = {});

}  // namespace cb::exact::emu::native_dosexec
