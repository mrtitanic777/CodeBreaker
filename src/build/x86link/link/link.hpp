// cb/link.hpp -- the state of one link: what wlink kept in global variables, one Link per run of the linker
// (cb_wlink::RunState::begin_run makes it), reached as wlink() (docs/X86LINK_DESIGN.md phase 4b, wave 7). Each
// member is a global it was, with the global's name, type and initial value.
#pragma once

#include "build/x86link/link/linkstd.h"
#include "build/x86link/base/carve.h"
#include "build/x86link/formats/output.h"
#include "build/x86link/objects/objnode.h"
#include "build/x86link/base/strtab.h"
#include "build/x86link/link/permdata.h"
#include "build/x86link/messages/msg.h"

namespace cb_wlink {

struct Link {
    group_entry    * AbsGroups;   /* pointer to defined absolute groups       */
    bool BannerPrinted;
    unsigned_32 CVDebugDirEntryPos = 0;
    unsigned_32 CVSize;
    infilelist       * CachedFiles;
    infilelist       * CachedLibFiles;
    carve_t CarveClass;
    carve_t CarveDLLInfo;
    carve_t CarveExportInfo;
    carve_t CarveGroup;
    carve_t CarveLeader;
    carve_t CarveModEntry;
    carve_t CarveSegData;
    carve_t CarveSymbol;
    addr_type CurrLoc;   /* current location pointer                 */
    mod_entry      * CurrMod;   /* pointer to current mod_entry struct      */
    lobject_data CurrRec;
    section        * CurrSect;   /* current section for file processing      */
    seg_leader     * CurrentSeg;   /* current segment being allocated          */
    dbi_flags DBIFlag;
    group_entry * DBIGroups;
    group_entry    * DataGroup;   /* pointer to DGROUP                        */
    byte           * EOObjRec;   /* end of object buffer                     */
    char           * ExePath;   /* PATH environment path                    */
    nodearray_handle ExtNodes;   // ptr to obj file import list
    mod_entry      * FakeModule;   /* Module containing all the extra sections */
    reloc_info FloatFixups;
    struct fmt_data FmtData;   /* format specific data                     */
    unsigned FmtRelocSize;
    group_entry    * Groups;   /* pointer to defined groups                */
    nodearray_handle GrpNodes;   // ptr to obj file group list
    symbol         * HeadSym;   /* head of symbol table list                */
    group_entry    * IDataGroup;   /* pointer to .idata                        */
    char             * IncFileName;
    incgroupdef      * IncGroupDefs;
    group_entry      ** IncGroups;
    symbol           * LastSym;
    mod_entry      * LibModules;   /* library modules (for processing)         */
    char           * LibPath;   /* LIB environment path                     */
    linkflag LinkFlags;   /* flags to indicate presence of parms      */
    stateflag LinkState;   /* flags to indicate linker state           */
    char           * MapFName;   /* name of map file                         */
    FILE           * MapFile;   /* handle of map file                       */
    mapflag MapFlags;   /* flags dealing with the map file          */
    unsigned_32 MaxErrors;
    byte MsgFlags[(MSG_MAX_ERR_MSG_NUM / 8) + 1];   // (msg.cpp: MSG_ARRAY_SIZE)
    size_t NameLen;
    nodearray_handle NameNodes;   // ptr to obj file lname list
    section        * NonSect;   /* non-overlay-class section                */
    unsigned_32 NumExports;
    int NumGroups;
    unsigned_32 NumImports;
    byte           * ObjBuff;   /* offset into object buffer                */
    file_format ObjFileFormat;
    obj_format ObjFormat;
    file_list      * ObjLibFiles;   /* list of library files (for searching)    */
    path_entry     * ObjPath;   /* path for object files                    */
    outfilelist    * OutFiles;   /* list of files which will be written to   */
    unsigned_16 OvlAreaSize;
    list_of_names    * OvlClasses;
    overlay_ref OvlSectNum;
    seg_leader       * OvlSeg;
    unsigned_32 OvlTabOffset;
    offset PackCodeLimit;   /* Maximum size of a packed code seg        */
    offset PackDataLimit;   /* Maximum size of a packed data seg        */
    stringtable PermStrings;
    stringtable PrefixStrings;   /* these are NetWare prefix strings of which there could possibly be several */
    int RecNum;   /* record number of the obj file            */
    section        * Root;   /* pointer to root information              */
    libnamelist      * SavedDefLibs;
    libnamelist      * SavedUserLibs;
    nodearray_handle SegNodes;   // ptr to obj file segment list
    addr_type StackAddr;   /* initial stack address                    */
    seg_leader       * StackSegPtr;
    offset StackSize;   /* size of stack                            */
    startinfo StartInfo;
    char         * SymFileName;
    offset TocShift;
    offset TocSize;
    char           * TokBuff;   /* Multi-purpose large buffer               */
    unsigned TokSize;   /* size of above buffer                     */
    path_entry     * UsrLibPath;   /* path for library files (LibPath)         */
};

extern Link* current_link;   // (set at each run's start)

}  // namespace cb_wlink

inline cb_wlink::Link& wlink() { return *cb_wlink::current_link; }
