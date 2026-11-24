/* csym_state.h -- the variables of csym.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "cvars.h"
#include "cgswitch.h"
#include "caux.h"
#include "cmacadd.h"
#include "roundmac.h"

#define SYM_BUF_SIZE_DEF    _1K
#define SYM_SEG_SIZE_DEF    _32K
#define SYM_MAX_INDEX_DEF   0xFFFFF
#define SYM_PER_BUF         (SYM_BUF_SIZE_DEF / sizeof( SYM_ENTRY ))
#define SYM_BUF_SIZE        (SYM_PER_BUF * sizeof( SYM_ENTRY ))
#define SYM_BUF_PER_SEG     (SYM_SEG_SIZE_DEF / SYM_BUF_SIZE_DEF)
#define SYM_SEG_SIZE        (SYM_BUF_SIZE * SYM_BUF_PER_SEG)
#define SYM_PER_SEG         (SYM_SEG_SIZE / sizeof( SYM_ENTRY ))
#define SYM_SEG_MAX         (SYM_MAX_INDEX_DEF / SYM_PER_SEG + 1)

typedef struct sym_stats {
    unsigned    get;
    unsigned    getptr;
    unsigned    replace;
    unsigned    read;
    unsigned    write;
} sym_stats;
typedef char        *SEGADDR_T;
typedef struct seg_info {
    SEGADDR_T           index;          /* segment #, EMS page #, disk seek # */
    bool             allocated : 1;  /* 1 => has been allocated */
} seg_info;

struct CsymState {
    unsigned Cached_sym_num;
    void *Cached_sym_addr;
    sym_stats SymStats;
    unsigned FirstSymInBuf;
    char *SymBufPtr;
    unsigned NextSymHandle;
    SEGADDR_T SymBufSegment;   /* segment # for symbol table buffers */
    seg_info SymBufSegs[SYM_SEG_MAX];   /* segments for symbols */
};
