/* pchdr_state.h -- the variables of pchdr.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "cvars.h"
#include "wio.h"
#include "watcom.h"
#include "sopen.h"
#include "cmacadd.h"
#include "toggles.h"
#include "clibext.h"

enum pch_aux_info_index {
    PCH_NULL_INDEX,
    PCH_FIRST_INDEX
};

struct PchdrState {
    jmp_buf PH_jmpbuf;
    int PH_handle;
    char *PH_Buffer;
    char *PH_BufPtr;
    unsigned PH_BufSize;
    MEPTR *PCHMacroHash;
    MEPTR PCHUndefMacroList;
    TYPEPTR TypeArray;
    TAGPTR *TagArray;
    FNAMEPTR FNameList;
    textsegment **TextSegArray;
    unsigned PH_SymHashCount;
    unsigned PH_FileCount;
    unsigned PH_RDirCount;
    unsigned PH_IAliasCount;
    unsigned PH_IncFileCount;
    unsigned PH_LibraryCount;
    unsigned PH_AliasCount;
    unsigned PH_SegCount;
    unsigned PH_MacroCount;
    unsigned PH_UndefMacroCount;
    unsigned PH_TypeCount;
    unsigned PH_TagCount;
    unsigned PH_PragmaCount;
    unsigned PH_PragmaEntryCount;
    unsigned PH_size;
    unsigned PH_MacroSize;
    unsigned PH_cwd_len;
    char PH_computing_size;
    RDIRPTR PCHRDirNames;   /* list of read-only directories */
    IALIASPTR PCHIAliasNames;   /* list of include aliases */
    INCFILE *PCHOldIncFileList;   /* temporary copy of old include file list */
    bool PCHOldIncListValid;   /* flag to indicate if PCHOldIncFileList is valid */
};
