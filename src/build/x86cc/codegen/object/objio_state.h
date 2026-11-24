/* objio_state.h -- the variables of objio.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include "cgmem.h"
#include "_cg.h"
#include "cgaux.h"
#include "bckdef.h"
#include "system.h"
#include "pcobj.h"
#include "zoiks.h"
#include "banner.h"
#include "utils.h"
#include "objio.h"
#include "onexit.h"
#include "feprotos.h"

typedef FILE            *handle;
typedef struct buf {
    struct buf  *nextbuf;
    char        *bufptr;        // current position within buffer
    char        *buf;           // start of buffer
    size_t      bytes_left;     // number of bytes remaining in buffer
    size_t      bytes_written;  // number of bytes written to buffer
} buf;

struct ObjioState {
    objoffset ObjOffset;
    handle ObjFile;
    bool NeedSeek;
    bool EraseObj;
    buf *BufList;   // start of list of buffers
    buf *CurBuf;   // current buffer
};
