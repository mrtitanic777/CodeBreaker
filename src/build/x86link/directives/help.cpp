// build/x86link/directives/help.cpp -- `?`, `option ?` and `help <format>`: the linker's help text and usage, as the message
// resource holds it (a port of wlink's cmdhelp.c).
#include <cstdlib>

#include "build/x86link/directives/common.hpp"
#include "build/x86link/directives/formats.hpp"
#include "build/x86link/base/fileio.h"
#include "build/x86link/driver/ideentry.h"

namespace cb::x86link::directives {

namespace {

#define _GENERAL_HELP   MSG_GENERAL_HELP_0, MSG_GENERAL_HELP_51
#define _DOS_HELP       MSG_DOS_HELP_0,     MSG_DOS_HELP_15
#define _OS2_HELP       MSG_OS2_HELP_0,     MSG_OS2_HELP_31
#define _WINDOWS_HELP   MSG_WINDOWS_HELP_0, MSG_WINDOWS_HELP_31
#define _WINVXD_HELP    MSG_WIN_VXD_HELP_0, MSG_WIN_VXD_HELP_31
#define _NT_HELP        MSG_NT_HELP_0,      MSG_NT_HELP_31
#define _PHARLAP_HELP   MSG_PHAR_HELP_0,    MSG_PHAR_HELP_15
#define _NOVELL_HELP    MSG_NOVELL_HELP_0,  MSG_NOVELL_HELP_31
#define _DOS16M_HELP    MSG_DOS16_HELP_0,   MSG_DOS16_HELP_15
#define _QNX_HELP       MSG_QNX_HELP_0,     MSG_QNX_HELP_15
#define _ELF_HELP       MSG_ELF_HELP_0,     MSG_ELF_HELP_15
#define _RDOS_HELP      MSG_RDOS_HELP_0,    MSG_RDOS_HELP_15
#define _RAW_HELP       MSG_RAW_HELP_0,     MSG_RAW_HELP_15

#define HELP_ARGS(x)            _ ## x ## _HELP
#define WRITE_HELP_ONE(x)       WriteHelp( HELP_ARGS(x), false, previous_null )
#define WRITE_HELP_FULL(x)      WriteHelp( HELP_ARGS(x), true, previous_null )

// `previous_null`: the last message of the help being written was empty (a second empty one ends the range). wlink kept
// it in a static, so it went from one help to the next; here it is the help being written's own.
void doWriteHelp(int first_msg, int last_msg, bool& previous_null) {
    char msg_buffer[RESOURCE_MAX_SIZE];

    for (int msg = first_msg; msg <= last_msg; msg++) {
        bool ok = Msg_Get(msg, msg_buffer);
        if (!ok) {
            if (previous_null) {
                break;
            }
            previous_null = true;
            continue;
        }
        WriteStdOutInfo(msg_buffer, INF, nullptr);
        if (previous_null) {
            previous_null = false;
        }
    }
}

void WriteHelp(int first_msg, int last_msg, bool genhelp, bool& previous_null) {
    if (genhelp || (first_msg == MSG_GENERAL_HELP_0 && last_msg == MSG_GENERAL_HELP_51)) {
        WLPrtBanner();
        if (genhelp) {
            doWriteHelp(_GENERAL_HELP, previous_null);
        }
    }
    doWriteHelp(first_msg, last_msg, previous_null);
}

#ifdef _EXE
bool ProcDosHelp() {
    bool previous_null = false;
    WRITE_HELP_FULL(DOS);
    return true;
}
#endif
#ifdef _OS2
bool ProcOS2Help() {
    bool previous_null = false;
    WRITE_HELP_FULL(OS2);
    return true;
}

bool ProcWindowsHelp() {
    bool previous_null = false;
    WRITE_HELP_FULL(WINDOWS);
    return true;
}

bool ProcWinVxdHelp() {
    bool previous_null = false;
    WRITE_HELP_FULL(WINVXD);
    return true;
}

bool ProcNTHelp() {
    bool previous_null = false;
    WRITE_HELP_FULL(NT);
    return true;
}
#endif
#ifdef _PHARLAP
bool ProcPharHelp() {
    bool previous_null = false;
    WRITE_HELP_FULL(PHARLAP);
    return true;
}
#endif
#ifdef _NOVELL
bool ProcNovellHelp() {
    bool previous_null = false;
    WRITE_HELP_FULL(NOVELL);
    return true;
}
#endif
#ifdef _DOS16M
bool Proc16MHelp() {
    bool previous_null = false;
    WRITE_HELP_FULL(DOS16M);
    return true;
}
#endif
#ifdef _QNX
bool ProcQNXHelp() {
    bool previous_null = false;
    WRITE_HELP_FULL(QNX);
    return true;
}
#endif

#ifdef _ELF
bool ProcELFHelp() {
    bool previous_null = false;
    WRITE_HELP_FULL(ELF);
    return true;
}
#endif

#ifdef _RDOS
bool ProcRdosHelp() {
    bool previous_null = false;
    WRITE_HELP_FULL(RDOS);
    return true;
}
#endif

#ifdef _RAW
bool ProcRawHelp() {
    bool previous_null = false;
    WRITE_HELP_FULL(RAW);
    return true;
}
#endif

constexpr Entry FormatHelp[] = {
#ifdef _EXE
    {"Dos", ProcDosHelp, MK_ALL},
#endif
#ifdef _OS2
    {"OS2", ProcOS2Help, MK_ALL},
    {"WINdows", ProcWindowsHelp, MK_ALL},
    {"VXD", ProcWinVxdHelp, MK_ALL},
    {"NT", ProcNTHelp, MK_ALL},
#endif
#ifdef _PHARLAP
    {"PHARlap", ProcPharHelp, MK_ALL},
#endif
#ifdef _NOVELL
    {"NOVell", ProcNovellHelp, MK_ALL},
#endif
#ifdef _DOS16M
    {"DOS16M", Proc16MHelp, MK_ALL},
#endif
#ifdef _QNX
    {"QNX", ProcQNXHelp, MK_ALL},
#endif
#ifdef _ELF
    {"ELF", ProcELFHelp, MK_ALL},
#endif
#ifdef _RDOS
    {"RDos", ProcRdosHelp, MK_ALL},
#endif
#ifdef _RAW
    {"Raw", ProcRawHelp, MK_ALL},
#endif
};

}  // namespace

void DisplayOptions() {
    bool previous_null = false;
    WRITE_HELP_ONE(GENERAL);
#if defined(_QNX) && defined(__QNX__)
    WRITE_HELP_ONE(QNX);
#endif
#ifdef _EXE
    WRITE_HELP_ONE(DOS);
#endif
#ifdef _OS2
    WRITE_HELP_ONE(OS2);
    WRITE_HELP_ONE(WINDOWS);
    WRITE_HELP_ONE(WINVXD);
    WRITE_HELP_ONE(NT);
#endif
#ifdef _PHARLAP
    WRITE_HELP_ONE(PHARLAP);
#endif
#ifdef _NOVELL
    WRITE_HELP_ONE(NOVELL);
#endif
#ifdef _DOS16M
    WRITE_HELP_ONE(DOS16M);
#endif
#if defined(_QNX) && !defined(__QNX__)
    WRITE_HELP_ONE(QNX);
#endif
#ifdef _ELF
    WRITE_HELP_ONE(ELF);
#endif
#ifdef _RDOS
    WRITE_HELP_ONE(RDOS);
#endif
#ifdef _RAW
    WRITE_HELP_ONE(RAW);
#endif
}

// display help, optionally allowing the user to specify the format he/she wants the help for.
bool DoHelp() { return process_one(FormatHelp, Sep::none); }

}  // namespace cb::x86link::directives
