// build/x86link/directives/formats.hpp -- what each format's directives give the rest of the directive layer: its state
// set up and freed with the format (Set*Fmt / Free*Fmt), finished at the directives' end (Cmd*Fini), and the handlers
// common.cpp's and driver.cpp's tables run (`format <it>`, `option ...` for it, its own directives). One file per format
// (dos.cpp, os2.cpp, novell.cpp, phar.cpp, dos16m.cpp, qnx.cpp, elf.cpp, rdos.cpp, raw.cpp), each a port of wlink's
// cmd<format>.c; the names are wlink's, the code typed (reader.hpp, language.hpp).
#pragma once

#include "build/x86link/directives/language.hpp"
#include "build/x86link/link/linkstd.h"

namespace cb::x86link::directives {

#ifdef _EXE   // DOS (dos.cpp)
void SetDosFmt();
void FreeDosFmt();
overlay_ref GetOvlRef(Link& link);
void CmdOvlFini(Link& link);
void MakeNewSection(Link& link);
bool ProcDosOptions(Link& link);
bool ProcOverlay(Link& link);
bool ProcBegin(Link& link);
bool ProcFixedLib(Link& link);
bool ProcNoVector(Link& link);
bool ProcVector(Link& link);
bool ProcForceVector(Link& link);
bool ProcDosFormat(Link& link);
#endif

#ifdef _OS2   // OS/2, Windows, Windows NT, VxD (os2.cpp)
void SetOS2Fmt();
void FreeOS2Fmt();
bool ProcOS2Options(Link& link);
bool ProcOS2Runtime(Link& link);
bool ProcCommit(Link& link);
bool ProcAnonExport(Link& link);
bool ProcResource(Link& link);
bool ProcOS2Import(Link& link);
bool ProcOS2Export(Link& link);
bool ProcOS2Segment(Link& link);
bool ProcOS2Alignment(Link& link);
bool ProcOS2HeapSize(Link& link);
bool ProcOS2NoRelocs(Link& link);
bool ProcOS2Format(Link& link);
bool ProcWindowsFormat(Link& link);
#endif

#ifdef _NOVELL   // Netware (novell.cpp)
void SetNovFmt();
void FreeNovFmt();
void CmdNovFini();
bool ProcNovOptions(Link& link);
bool ProcNovDBI(Link& link);
bool ProcNovExportsDBI(Link& link);
bool ProcNovModule(Link& link);
bool ProcNovImport(Link& link);
bool ProcNovExport(Link& link);
bool ProcNovFormat(Link& link);
#endif

#ifdef _PHARLAP   // Phar Lap (phar.cpp)
void SetPharFmt();
void FreePharFmt();
bool ProcPharOptions(Link& link);
bool ProcPharRuntime(Link& link);
bool ProcPharFormat(Link& link);
#endif

#ifdef _DOS16M   // DOS/16M (dos16m.cpp)
void SetD16MFmt();
void FreeD16MFmt();
void CmdD16MFini(Link& link);
bool Proc16MOptions(Link& link);
bool Proc16MNoRelocs(Link& link);
bool Proc16MRuntime(Link& link);
bool Proc16MMemory(Link& link);
bool Proc16MTransparent(Link& link);
bool Proc16MFormat(Link& link);
#endif

#ifdef _QNX   // QNX (qnx.cpp)
void SetQNXFmt();
void FreeQNXFmt();
void CmdQNXFini(Link& link);
bool ProcQNXOptions(Link& link);
bool ProcQNXNoRelocs(Link& link);
bool ProcQNXSegment(Link& link);
bool ProcQNXHeapSize(Link& link);
bool ProcQNXFormat(Link& link);
#endif

#ifdef _ELF   // ELF (elf.cpp)
void SetELFFmt();
void FreeELFFmt();
bool ProcELFOptions(Link& link);
bool ProcELFRuntime(Link& link);
bool ProcELFImport(Link& link);
bool ProcELFExport(Link& link);
bool ProcELFModule(Link& link);
bool ProcELFAlignment(Link& link);
bool ProcELFNoRelocs(Link& link);
bool ProcELFFormat(Link& link);
#endif

#ifdef _RDOS   // RDOS (rdos.cpp)
void SetRdosFmt();
void FreeRdosFmt();
bool ProcRdosOptions(Link& link);
bool ProcRdosFormat(Link& link);
#endif

#ifdef _RAW   // raw binary and hex (raw.cpp)
bool ProcRawFormat(Link& link);
#endif

// help.cpp: `?` and the usage text
bool DoHelp(Link& link);
void DisplayOptions(Link& link);

}  // namespace cb::x86link::directives
