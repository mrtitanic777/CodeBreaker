// build/x86link/directives/formats.hpp -- what each format's directives give the rest of the directive layer: its state
// set up and freed with the format (Set*Fmt / Free*Fmt), finished at the directives' end (Cmd*Fini), and the handlers
// common.cpp's and driver.cpp's tables run (`format <it>`, `option ...` for it, its own directives). One file per format
// (dos.cpp, os2.cpp, novell.cpp, phar.cpp, dos16m.cpp, qnx.cpp, elf.cpp, rdos.cpp, raw.cpp), each a port of wlink's
// cmd<format>.c; the names are wlink's, the code typed (reader.hpp, language.hpp).
#pragma once

#include "build/x86link/directives/language.hpp"
#include "linkstd.h"

namespace cb::x86link::directives {

#ifdef _EXE   // DOS (dos.cpp)
void SetDosFmt();
void FreeDosFmt();
overlay_ref GetOvlRef();
void CmdOvlFini();
void MakeNewSection();
bool ProcDosOptions();
bool ProcOverlay();
bool ProcBegin();
bool ProcFixedLib();
bool ProcNoVector();
bool ProcVector();
bool ProcForceVector();
bool ProcDosFormat();
#endif

#ifdef _OS2   // OS/2, Windows, Windows NT, VxD (os2.cpp)
void SetOS2Fmt();
void FreeOS2Fmt();
bool ProcOS2Options();
bool ProcOS2Runtime();
bool ProcCommit();
bool ProcAnonExport();
bool ProcResource();
bool ProcOS2Import();
bool ProcOS2Export();
bool ProcOS2Segment();
bool ProcOS2Alignment();
bool ProcOS2HeapSize();
bool ProcOS2NoRelocs();
bool ProcOS2Format();
bool ProcWindowsFormat();
#endif

#ifdef _NOVELL   // Netware (novell.cpp)
void SetNovFmt();
void FreeNovFmt();
void CmdNovFini();
bool ProcNovOptions();
bool ProcNovDBI();
bool ProcNovExportsDBI();
bool ProcNovModule();
bool ProcNovImport();
bool ProcNovExport();
bool ProcNovFormat();
#endif

#ifdef _PHARLAP   // Phar Lap (phar.cpp)
void SetPharFmt();
void FreePharFmt();
bool ProcPharOptions();
bool ProcPharRuntime();
bool ProcPharFormat();
#endif

#ifdef _DOS16M   // DOS/16M (dos16m.cpp)
void SetD16MFmt();
void FreeD16MFmt();
void CmdD16MFini();
bool Proc16MOptions();
bool Proc16MNoRelocs();
bool Proc16MRuntime();
bool Proc16MMemory();
bool Proc16MTransparent();
bool Proc16MFormat();
#endif

#ifdef _QNX   // QNX (qnx.cpp)
void SetQNXFmt();
void FreeQNXFmt();
void CmdQNXFini();
bool ProcQNXOptions();
bool ProcQNXNoRelocs();
bool ProcQNXSegment();
bool ProcQNXHeapSize();
bool ProcQNXFormat();
#endif

#ifdef _ELF   // ELF (elf.cpp)
void SetELFFmt();
void FreeELFFmt();
bool ProcELFOptions();
bool ProcELFRuntime();
bool ProcELFImport();
bool ProcELFExport();
bool ProcELFModule();
bool ProcELFAlignment();
bool ProcELFNoRelocs();
bool ProcELFFormat();
#endif

#ifdef _RDOS   // RDOS (rdos.cpp)
void SetRdosFmt();
void FreeRdosFmt();
bool ProcRdosOptions();
bool ProcRdosFormat();
#endif

#ifdef _RAW   // raw binary and hex (raw.cpp)
bool ProcRawFormat();
#endif

// help.cpp: `?` and the usage text
bool DoHelp();
void DisplayOptions();

}  // namespace cb::x86link::directives
