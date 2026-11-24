/* cmemmgr_state.h -- the variables of cmemmgr.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "cvars.h"
#include "cgdefs.h"
#include "feprotos.h"
#include "roundmac.h"

enum cmem_kind {
    CMEM_PERM,
    CMEM_MEM,
    CMEM_NONE,
};
typedef struct  mem_block {
    size_t              size;   /* length of stg */
    struct mem_block    *prev;  /* pointer to previous free memory block */
    struct mem_block    *next;  /* pointer to next     free memory block */
} MCB;
typedef struct mem_blk {
    struct mem_blk      *next;
    char                *ptr;   // old permanent area block pointer
    size_t              size;   // old permanent area block size
} mem_blk;

struct CmemmgrState {
    char *PermPtr;   /* first free byte in permanent area block */
    size_t PermSize;   /* total size of permanent area block */
    size_t PermAvail;   /* # of bytes available in permanent area block */
    MCB CFreeList;
    mem_blk *Blks;
};
