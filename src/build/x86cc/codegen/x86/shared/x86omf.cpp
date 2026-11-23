/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2026 The Open Watcom Contributors. All Rights Reserved.
*    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
*
*  ========================================================================
*
*    This file contains Original Code and/or Modifications of Original
*    Code as defined in and that are subject to the Sybase Open Watcom
*    Public License version 1.0 (the 'License'). You may not use this file
*    except in compliance with the License. BY USING THIS FILE YOU AGREE TO
*    ALL TERMS AND CONDITIONS OF THE LICENSE. A copy of the License is
*    provided with the Original Code and Modifications, and is also
*    available at www.sybase.com/developer/opensource.
*
*    The Original Code and all software distributed under the License are
*    distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
*    EXPRESS OR IMPLIED, AND SYBASE AND ALL CONTRIBUTORS HEREBY DISCLAIM
*    ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
*    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR
*    NON-INFRINGEMENT. Please see the License for the specific language
*    governing rights and limitations under the License.
*
*  ========================================================================
*
* Description:  Emit OMF object records.
*
****************************************************************************/


#include "_cgstd.h"
#include <time.h>
#include "coderep.h"
#include "data.h"
#include "cgauxcc.h"
#include "cgauxinf.h"
#include "system.h"
#include "cgmem.h"
#include "zoiks.h"
#include "rtrtn.h"
#include "x86objd.h"
#include "utils.h"
#include "objout.h"
#include "cvdbg.h"
#include "dbsyms.h"
#include "cvsyms.h"
#include "dw.h"
#include "dfsyms.h"
#include "dfsupp.h"
#include "wvsyms.h"
#include "wvtypes.h"
#include "objio.h"
#include "onexit.h"
#include "optmain.h"
#include "opttell.h"
#include "intrface.h"
#include "x86obj.h"
#include "x86emit.h"
#include "i87data.h"
#include "cgsegids.h"
#include "feprotos.h"

#include "clibext.h"
#include "backrun.hpp"


static void         initSegDefs( BackRun& back );
static bool         freeObjCache( BackRun& back );
static segment_id   askOP( BackRun& back );
static segment_id   changeOP( BackRun& back, segment_id );
static void         setOP( BackRun& back, segment_id );
static void         defSegment( BackRun& back, segment_id, seg_attr, const char *, uint, bool );
static void         doEmptyQueue( BackRun& back );
static char         getMemModel( BackRun& back );
static segment_id   dbgSegDef( BackRun& back, const char *, const char *, int );
static void         objInit( BackRun& back );
static offset       askLocation( BackRun& back );
static void         chkDbgSegSize( BackRun& back, offset, bool );
static bool         useImportForm( BackRun& back, fe_attr );
static bool         askSegIsNear( BackRun& back, segment_id );
static bool         askSegIsBlank( BackRun& back, segment_id );
static bool         askSegIsPrivate( BackRun& back, segment_id );
static bool         askSegIsROM( BackRun& back, segment_id );
static segment_id   askBackSeg( BackRun& back );
static segment_id   askCodeSeg( BackRun& back );
static bool         haveCodeSeg( BackRun& back );
static segment_id   askAltCodeSeg( BackRun& back );
static segment_id   askCode16Seg( BackRun& back );
static void         outSelect( BackRun& back, bool );
static void         setUpObj( BackRun& back, bool );
static void         flushOP( BackRun& back, segment_id );
static void         objFini( BackRun& back );
static void         outDLLExport( BackRun& back, uint, cg_sym_handle );
static void         outLabel( BackRun& back, label_handle );
static void         absPatch( BackRun& back, abspatch_handle, offset );
static void         *initPatches( BackRun& back );
static void         setBigLocation( BackRun& back, long_offset );
static void         incLocation( BackRun& back, offset );
static void         setLocation( BackRun& back, offset );
static void         outFPPatch( BackRun& back, fp_patches );
static void         outPatch( BackRun& back, label_handle, patch_attr );
static abs_patch    *newAbsPatch( BackRun& back );
static void         outDataByte( BackRun& back, byte );
static void         outDataShort( BackRun& back, uint_16 );
static void         outDataLong( BackRun& back, uint_32 );
static void         outAbsPatch( BackRun& back, abs_patch *, patch_attr );
static void         outReloc( BackRun& back, segment_id, fix_class, bool );
static void         outSpecialCommon( BackRun& back, import_handle, fix_class, bool );
static void         outImport( BackRun& back, cg_sym_handle, fix_class, bool );
static void         outRTImportRel( BackRun& back, rt_class, fix_class, bool );
static void         outRTImport( BackRun& back, rt_class, fix_class );
static void         outBckExport( BackRun& back, const char *, bool );
static void         outBckImport( BackRun& back, const char *, back_handle, fix_class );
static void         outLineNum( BackRun& back, cg_linenum, bool );
static unsigned     savePendingLine( BackRun& back, unsigned );
static void         outDBytes( BackRun& back, unsigned, const byte * );
static void         outIBytes( BackRun& back, byte, offset );
static bool         needBaseSet( BackRun& back );
static offset       askMaxSize( BackRun& back );
static long_offset  askBigLocation( BackRun& back );
static long_offset  askBigMaxSize( BackRun& back );
static void         tellObjNewLabel( BackRun& back, cg_sym_handle );
static void         tellObjNewProc( BackRun& back, cg_sym_handle );
static void         tellObjVirtFuncRef( BackRun& back, void * );
static segment_id   askSegID( BackRun& back, pointer, cg_class );
static bool         askNameIsCode( BackRun& back, pointer, cg_class );
static bool         askNameIsROM( BackRun& back, pointer, cg_class );
static bool         symIsExported( BackRun& back, cg_sym_handle );


#define _NIDX_NULL      1   /* lname "" */
#define _NIDX_CODE      2   /* lname "CODE" */
#define _NIDX_DATA      3   /* lname "DATA" */
#define _NIDX_BSS       4   /* lname "BSS" */
#define _NIDX_TLS       5   /* lname "TLS" */

#define MODEST_HDR      50
#define INCREMENT_HDR   50

#define MODEST_INFO     5
#define INCREMENT_INFO  5

#define MODEST_IMP      BUFFSIZE
#define INCREMENT_IMP   50

#define MODEST_LINE     BUFFSIZE
#define INCREMENT_LINE  200

#define MODEST_OBJ      BUFFSIZE
#define INCREMENT_OBJ   256

#define NOMINAL_FIX     20
#define INCREMENT_FIX   50

#define MODEST_EXP      BUFFSIZE
#define INCREMENT_EXP   50

#define MODEST_PAT      10
#define INCREMENT_PAT   10


#define _CopyTrans( src, dst, len )        Copy( src, dst, len )

#define _ARRAYOF( what, type )  ((type *)(what)->array)

#define altCodeSegId    back.x86omf->codeSegId

#define OMFNAMELEN(s)   *(unsigned char *)(s)

enum {
    TIME_SEC_B  = 0,
    TIME_SEC_F  = 0x001f,
    TIME_MIN_B  = 5,
    TIME_MIN_F  = 0x07e0,
    TIME_HOUR_B = 11,
    TIME_HOUR_F = 0xf800
};

enum {
    DATE_DAY_B  = 0,
    DATE_DAY_F  = 0x001f,
    DATE_MON_B  = 5,
    DATE_MON_F  = 0x01e0,
    DATE_YEAR_B = 9,
    DATE_YEAR_F = 0xfe00
};

typedef struct lname_cache {
    struct lname_cache  *next;
    omf_idx             idx;
    char                name[1];        /* var sized, first byte is length */
} lname_cache;

typedef struct virt_func_ref_list {
    struct virt_func_ref_list   *next;
    void                        *cookie;
} virt_func_ref_list;

typedef struct dbg_seg_info {
    segment_id          *segid;
    const char          *seg_name;
    const char          *class_name;
} dbg_seg_info;


static const char * const FPPatchName[] = {
    #define pick_fp(enum,name,alt_name,win,alt_win,others,alt_others) name,
    #include "fppatche.h"
    #undef pick_fp
};

static const char * const FPPatchAltName[] = {
    #define pick_fp(enum,name,alt_name,win,alt_win,others,alt_others) alt_name,
    #include "fppatche.h"
    #undef pick_fp
};


namespace {

/* the statics of this file, one set for each run of the compiler */
}  // namespace
struct X86omfState {
    bool GenStaticImports;
    omf_idx ImportHdl;
    array_control Imports;
    array_control SegInfo;
    abs_patch *AbsPatches;
    segment_id codeSegId = BACKSEGS;
    segment_id dataSegId;
    segdef *SegDefs;
    long_offset CodeSize;
    long_offset DataSize;
    long_offset DbgTypeSize;
    index_rec *CurrSeg;
    omf_idx GroupIndex;
    omf_idx DGroupIndex;
    omf_idx SegmentIndex;
    omf_idx PrivateIndexRW;
    omf_idx PrivateIndexRO;
    omf_idx CodeGroupGIdx;
    omf_idx CodeGroupNIdx;
    char CodeGroup[80];
    char DataGroup[80];
    offset SelStart;
    omf_idx selIdx;
    segment_id backSegId = BACKSEGS;
    omf_idx FPPatchImp[FPP_NUMBER_OF_TYPES];
    int segmentCount;
    bool NoDGroup;
    short CurrFNo;
#if _TARGET & _TARG_80386
    omf_idx FlatGIndex;
#endif
#if _TARGET & _TARG_80386
    omf_idx FlatNIndex;
#endif
    omf_idx TLSGIndex;
    omf_idx NameIndex;
    lname_cache *NameCache;
    lname_cache *NameCacheDumped;
    struct dbg_seg_info DbgSegs[2];
    segment_id askCode16Seg_Code16Seg = BACKSEGS;
};

/* (the debug segments' ids are the run's: set when it makes this part) */
static void x86omf_start( BackRun& back, X86omfState& part )
{
    part.DbgSegs[0] = { &back.data.DbgLocals, "$$SYMBOLS", "DEBSYM" };
    part.DbgSegs[1] = { &back.data.DbgTypes,  "$$TYPES",   "DEBTYP" };
}
namespace {

}  // namespace


static void    initSegDefs( BackRun& back )
/*************************/
{
    back.x86omf->SegDefs = NULL;
    back.x86omf->NameCache = NULL;
    back.x86omf->NameCacheDumped = NULL;
    back.x86omf->NameIndex = 0;
    back.x86omf->GroupIndex = 0;
    back.x86omf->DGroupIndex = 0;
    back.x86omf->TLSGIndex = 0;
#if _TARGET & _TARG_80386
    back.x86omf->FlatGIndex = 0;
#endif
    back.x86omf->segmentCount = 0;
    back.x86omf->codeSegId = BACKSEGS; /* just so it doesn't match a FE seg_id */
    back.x86omf->backSegId = BACKSEGS;
}

static omf_idx GetNameIdx( BackRun& back, const char *name, const char *suff )
/*************************************************************/
{
    lname_cache         **owner;
    lname_cache         *curr;
    size_t              len;
    char                idxname[256];

    len = strlen( name ) + strlen( suff );
    assert( len < 256 );
    if( len < 256 ) {
        strcpy( idxname, name );
        strcat( idxname, suff );
        for( owner = &back.x86omf->NameCache; (curr = *owner) != NULL; owner = &curr->next ) {
            if( len == OMFNAMELEN( curr->name ) && strcmp( idxname, curr->name + 1 ) == 0 ) {
                return( curr->idx );
            }
        }
        curr = static_cast<lname_cache *>(CGAlloc( back, sizeof( *curr ) + len + 1 ));
        *owner = curr;
        curr->next = NULL;
        curr->idx = ++back.x86omf->NameIndex;
        curr->name[0] = (unsigned char)len;
        strcpy( curr->name + 1, idxname );
        return( back.x86omf->NameIndex );
    }
    return( 0 );
}

static  byte    DoSum( const byte *buff, size_t len )
/***************************************************/
{
    byte        sum;

    sum = 0;
    while( len > 0 ) {
        sum += *buff++;
        --len;
    }
    return( sum );
}

static void PutObjOMFRec( BackRun& back, byte class_, array_control *actl )
/*********************************************************/
{
    uint_16         blen;
    byte            cksum;

    if( actl->used > 0 ) {
        blen = _TargetShort( actl->used + 1 );
        cksum = class_;
        cksum += DoSum( static_cast<const byte *>((const void *)&blen), sizeof( blen ) );
        cksum += DoSum( static_cast<const byte *>(actl->array), actl->used );
        cksum = -cksum;
        PutObjBytes( back, &class_, 1 );
        PutObjBytes( back, (const byte *)&blen, sizeof( blen ) );
        PutObjBytes( back, actl->array, actl->used );
        PutObjBytes( back, &cksum, 1 );
        actl->used = 0;
    }
}

static void PatchObj( BackRun& back, objhandle rec, objoffset roffset, const byte *buff, size_t len )
/************************************************************************************/
{
    byte            cksum;
    uint_16         reclen;
    byte            inbuff[80];

    SeekGetObj( back, rec, 1, (byte *)&reclen, 2 );
    reclen = _HostShort( reclen );
    SeekGetObj( back, rec, roffset + 3, inbuff, len );
    SeekPutObj( back, rec, roffset + 3, buff, len );
    SeekGetObj( back, rec, 2 + reclen, &cksum, 1 );

    cksum += DoSum( inbuff, len );
    cksum -= DoSum( buff, len );

    SeekPutObj( back, rec, 2 + reclen, &cksum, 1 );
    NeedSeekObj(back);
}


static void FlushNames( BackRun& back )
/****************************/
{
    /*
     * don't want to allocate memory because we might be in a low memory
     * situation
     */
    unsigned_8          buff[512];
    unsigned            len;
    lname_cache         *dmp;
    array_control       tmp;

    tmp.array = buff;
    tmp.used = 0;
    dmp = ( back.x86omf->NameCacheDumped != NULL ) ? back.x86omf->NameCacheDumped->next : back.x86omf->NameCache;
    for( ; dmp != NULL; dmp = dmp->next ) {
        len = OMFNAMELEN( dmp->name );
        if( ( tmp.used + len ) > ( sizeof( buff ) - 1 ) ) {
            PutObjOMFRec( back, CMD_LNAMES, &tmp );
        }
        buff[tmp.used++] = len;
        _CopyTrans( dmp->name + 1, buff + tmp.used, len );
        tmp.used += len;
        back.x86omf->NameCacheDumped = dmp;
    }
    if( tmp.used > 0 ) {
        PutObjOMFRec( back, CMD_LNAMES, &tmp );
    }
}

static bool freeObjCache( BackRun& back )
/***********************/
{
    lname_cache         *tmp;

    if( back.x86omf->NameCache == NULL )
        return( false );
    FlushNames(back);
    while( back.x86omf->NameCache != NULL ) {
        tmp = back.x86omf->NameCache->next;
        CGFree( back, back.x86omf->NameCache );
        back.x86omf->NameCache = tmp;
    }
    back.x86omf->NameCacheDumped = NULL;
    return( true );
}


static  index_rec   *AskSegIndex( BackRun& back, segment_id segid )
/**************************************************/
{
    unsigned    i;

    for( i = 0; i < back.x86omf->SegInfo.used; ++i ) {
        if( _ARRAYOF( &back.x86omf->SegInfo, index_rec )[i].segid == segid ) {
            return( _ARRAYOF( &back.x86omf->SegInfo, index_rec ) + i );
        }
    }
    return( NULL );
}


static  void    ReallocArray( BackRun& back, array_control *arr, unsigned need )
/***************************************************************/
{
    byte        *p;
    unsigned    new_len;

    new_len = arr->alloc;
    for( ;; ) {
        new_len += arr->inc;
        if( new_len >= need ) {
            break;
        }
    }
    p = static_cast<byte *>(CGAlloc( back, arr->entry * new_len ));
    Copy( arr->array, p, arr->entry * arr->used );
    CGFree( back, arr->array );
    arr->array = p;
    arr->alloc = new_len;
}

static  byte    SegmentAttr( BackRun& back, byte align, seg_attr tipe, bool use_16 )
/*******************************************************************/
{
    byte        attr;

    /* unused parameters */ (void)use_16;

    if( align <= 1 ) {
        attr = SEG_ALGN_BYTE;
    } else if( align <= 2 ) {
        attr = SEG_ALGN_WORD;
#if _TARGET & _TARG_80386
    } else if( align <= 4 ) {
        attr = SEG_ALGN_DWORD;
#endif
    } else if( align <= 16 ) {
        attr = SEG_ALGN_PARA;
    } else {
        attr = SEG_ALGN_PAGE;
    }
    if( tipe & COMMON ) {
        attr |= SEG_COMB_COMMON;
    } else if( (tipe & (PRIVATE | GLOBAL)) == PRIVATE ) {
        attr |= SEG_COMB_PRIVATE;
    } else { /* normal or a kludge for wsl front end ( PRIVATE | GLOBAL )*/
        attr |= SEG_COMB_NORMAL;
    }
#if _TARGET & _TARG_80386
    if( _IsntTargetModel( CGSW_X86_EZ_OMF ) ) {
        if( _IsTargetModel( CGSW_X86_USE_32 ) ) {
            if( !use_16 ) {
                attr |= SEG_USE_32;
            }
        }
    }
#endif
    return( attr );
}

static  void    SegmentClass( BackRun& back, index_rec *rec )
/********************************************/
{
    char        *class_name;

    class_name = static_cast<char *>(FEAuxInfo( *back.front, (pointer)(pointer_uint)rec->segid, FEINF_CLASS_NAME ));
    if( class_name != NULL ) {
        rec->cidx = GetNameIdx( back, class_name, "" );
    }
}

/* Array Control Routines*/

static void FillArray( BackRun& back, array_control *res, unsigned size, unsigned starting, unsigned increment )
/***********************************************************************************************/
{
    res->array = CGAlloc( back, starting * size );
    res->alloc = starting;
    res->used = 0;
    res->entry = size;
    res->inc = increment;
}

static array_control *InitArray( BackRun& back, unsigned size, unsigned starting, unsigned increment )
/**************************************************************************************
 * DO NOT call InitArray with size or starting value zero
 */
{
    array_control       *res;

    res = static_cast<array_control *>(CGAlloc( back, sizeof( array_control ) ));
    FillArray( back, res, size, starting, increment );
    return( res );
}


static  void    SetMaxWritten( BackRun& back )
/***********************************/
{
    if( back.x86omf->CurrSeg->max_written < back.x86omf->CurrSeg->location ) {
        back.x86omf->CurrSeg->max_written = back.x86omf->CurrSeg->location;
    }
}


static void *allocOut( BackRun& back, array_control *dest, unsigned size )
/*********************************************************/
{
    unsigned    need;

    need = dest->used + size;
    if( need > dest->alloc ) {
        ReallocArray( back, dest, need );
    }
    size = dest->used;
    dest->used = need;
    return( (char *)dest->array + dest->entry * size );
}


static void *allocOutData( BackRun& back, unsigned size )
/****************************************/
{
    unsigned        need;
    unsigned        i;
    array_control   *obj_data;

    i = back.x86omf->CurrSeg->location - back.x86omf->CurrSeg->obj->start + back.x86omf->CurrSeg->data_prefix_size;
    incLocation( back, size );
    need = i + size;
    obj_data = &back.x86omf->CurrSeg->obj->data;
    if( obj_data->used < need ) {
        if( need > obj_data->alloc ) {
            ReallocArray( back, obj_data, need );
        }
        obj_data->used = need;
    }
    SetMaxWritten(back);
    return( (char *)obj_data->array + obj_data->entry * i );
}


static  void    OutByte( BackRun& back, byte value, array_control *dest )
/********************************************************/
{
    *(byte *)allocOut( back, dest, sizeof( byte ) ) = value;
}

static  void    OutShort( BackRun& back, uint_16 value, array_control *dest )
/************************************************************/
{
    value = _TargetShort( value );
    memcpy( allocOut( back, dest, sizeof( uint_16 ) ), &value, sizeof( uint_16 ) );
}

static  void    OutLongInt( BackRun& back, uint_32 value, array_control *dest )
/**************************************************************/
{
    value = _TargetLongInt( value );
    memcpy( allocOut( back, dest, sizeof( uint_32 ) ), &value, sizeof( uint_32 ) );
}

static  void    OutOffset( BackRun& back, offset value, array_control *dest )
/************************************************************/
{
    value = _TargetOffset( value );
    memcpy( allocOut( back, dest, sizeof( offset ) ), &value, sizeof( offset ) );
}

#if _TARGET & _TARG_8086
/*
 * 32bit debug seg support dwarf, codeview
 */
static  void    OutLongOffset( BackRun& back, long_offset value, array_control *dest )
/*********************************************************************/
{
    value = _TargetLongInt( value );
    memcpy( allocOut( back, dest, sizeof( long_offset ) ), &value, sizeof( long_offset ) );
}
#endif

static  void    OutIdx( BackRun& back, omf_idx value, array_control *dest )
/**********************************************************/
{
    if( value >= 128 ) {
        OutByte( back, (value >> 8) | 0x80, dest );
    }
    OutByte( back, value, dest );
}

static  void    OutBuffer( BackRun& back, const void *name, unsigned len, array_control *dest )
/******************************************************************************/
{
    memcpy( allocOut( back, dest, len ), name, len );
}

static object *InitTarg( BackRun& back, index_rec *rec )
/***************************************/
{
    object          *obj;

    obj = static_cast<object *>(CGAlloc( back, sizeof( object ) ));
    /*
     * try to bag all the memory we'll need now
     */
    FillArray( back, &obj->data, sizeof( byte ), MODEST_OBJ, INCREMENT_OBJ );
    FillArray( back, &obj->fixes, sizeof( byte ), NOMINAL_FIX, INCREMENT_FIX );
    FillArray( back, &obj->exports, sizeof( byte ), MODEST_EXP, INCREMENT_EXP );
    obj->lines = NULL;
    obj->tpatches = NULL;
    obj->gen_static_exports = false;
    obj->pending_line_number = 0;
    obj->last_line = 0;
    obj->last_offset = 0;
    obj->index = rec->sidx;
    obj->start = rec->location;
    return( obj );
}

static  void    DoASegDef( BackRun& back, index_rec *rec, bool use_16 )
/******************************************************/
{
    object          *obj;
    omf_cmd         cmd;
    array_control   *obj_data;

    /* unused parameters */ (void)use_16;

    rec->obj = obj = InitTarg( back, rec );
    obj_data = &obj->data;
    OutByte( back, rec->attr, obj_data );
#if _TARGET & _TARG_80386
    OutOffset( back, 0, obj_data );           /* segment size (for now) */
#else /* SEG32DBG dwarf, codeview */
    if( rec->attr & SEG_USE_32 ) {
        OutLongOffset( back, 0, obj_data );   /* segment size (for now) */
    } else {
        OutOffset( back, 0, obj_data );       /* segment size (for now) */
    }
#endif
    OutIdx( back, rec->nidx, obj_data );      /* segment name index */
    OutIdx( back, rec->cidx, obj_data );      /* class name index */
    OutIdx( back, _NIDX_NULL, obj_data );     /* overlay name index */
#if _TARGET & _TARG_80386
    if( _IsTargetModel( CGSW_X86_EZ_OMF ) ) {
        if( _IsntTargetModel( CGSW_X86_USE_32 ) || use_16 ) {
            OutByte( back, 2, obj_data );     /* to indicate USE16 EXECUTE/READ */
        }
    }
#endif
    FlushNames(back);
    obj->segfix = AskObjHandle(back);
    if( ++back.x86omf->segmentCount > 32000 ) {
        FatalError( back, "too many segments" );
    }
#if _TARGET & _TARG_80386
    if( _IsntTargetModel( CGSW_X86_EZ_OMF ) ) {
        cmd = CMD_SEGDEF32;
    } else {
        cmd = CMD_SEGDEF;
    }
#else /* SEG32DBG dwarf, codeview */
    if( (rec->attr & SEG_USE_32)
      && _IsntTargetModel( CGSW_X86_EZ_OMF ) ) {
        cmd = CMD_SEGDEF32;
    } else {
        cmd = CMD_SEGDEF;
    }
#endif
    PutObjOMFRec( back, cmd, obj_data );
    if( rec->exec ) {
        OutShort( back, LINKER_COMMENT, obj_data );
        OutByte( back, LDIR_OPT_FAR_CALLS, obj_data );
        OutIdx( back, rec->sidx, obj_data );
        PutObjOMFRec( back, CMD_COMENT, obj_data );
    }
    rec->location = 0;
}


static  void    OutCGroup( BackRun& back, omf_idx sidx, array_control *dest )
/************************************************************/
{
    FlushNames(back);
    OutIdx( back, back.x86omf->CodeGroupNIdx, dest );
    OutByte( back, GRP_SEGIDX, dest );
    OutIdx( back, sidx, dest );
    PutObjOMFRec( back, CMD_GRPDEF, dest );
}

static void OutGroup( BackRun& back, omf_idx sidx, array_control *group_def, omf_idx *index_p )
/******************************************************************************/
{
    if( *index_p == 0 ) {
        *index_p = ++back.x86omf->GroupIndex;
    }
    OutByte( back, GRP_SEGIDX, group_def );
    OutIdx( back, sidx, group_def );
}

static segment_id  askOP( BackRun& back )
/***********************/
{
    segment_id  segid;

    if( back.x86omf->CurrSeg == NULL ) {
        segid = UNDEFSEG;
    } else {
        segid = back.x86omf->CurrSeg->segid;
    }
    return( segid );
}

static segment_id  changeOP( BackRun& back, segment_id segid )
/**************************************/
{
    segment_id  old_segid;

    if( back.x86omf->CurrSeg == NULL ) {
        old_segid = UNDEFSEG;
    } else {
        old_segid = back.x86omf->CurrSeg->segid;
    }
    if( segid == UNDEFSEG ) {
        back.x86omf->CurrSeg = NULL;
    } else {
        back.x86omf->CurrSeg = AskSegIndex( back, segid );
    }
    return( old_segid );
}

static void    setOP( BackRun& back, segment_id segid )
/*******************************/
{
    if( segid == UNDEFSEG ) {
        back.x86omf->CurrSeg = NULL;
    } else {
        back.x86omf->CurrSeg = AskSegIndex( back, segid );
    }
}

static  index_rec   *AllocNewSegRec( BackRun& back )
/*****************************************/
{
    index_rec   *rec;
    segment_id  segid;

    /*
     * CurrSeg pointer might have moved on us by allocOut
     * therefore we need to call askOP before and setOP after
     * to refresh CurrSeg pointer
     */
    segid = askOP(back);
    rec = static_cast<index_rec *>(allocOut( back, &back.x86omf->SegInfo, 1 ));
    setOP( back, segid );
    return( rec );
}

static void DoSegment( BackRun& back, segdef *seg, array_control *dgroup_def, array_control *tgroup_def, bool use_16 )
/*****************************************************************************************************/
{
    index_rec   *rec;

    rec = AskSegIndex( back, seg->segid );
    if( rec == NULL ) {
        rec = AllocNewSegRec(back);
    }
    if( seg->attr & PRIVATE ) {
        rec->private_ = true;
    } else {
        rec->private_ = false;
    }
    rec->location = 0;
    rec->big = 0;
    rec->rom = 0;
    rec->need_base_set = true;
    rec->data_in_code = false;
    rec->start_data_in_code = false;
    rec->data_ptr_in_code = false;
    rec->prefix_comdat_state = PCS_OFF;
    rec->max_written = 0;
    rec->max_size = 0;
    rec->comdat_label = NULL;
    rec->comdat_symbol = NULL;
    rec->total_comdat_size = 0;
    rec->comdat_size = 0;
    rec->virt_func_refs = NULL;
    rec->data_prefix_size = 0;
    rec->comdat_prefix_import = NOT_IMPORTED;
    rec->sidx = ++back.x86omf->SegmentIndex;
    rec->segid = seg->segid;
    rec->attr = SegmentAttr( back, seg->align, seg->attr, use_16 );
    if( seg->attr & EXEC ) {
        rec->exec = true;
        rec->rom = true;   /* code is alway ROM */
        rec->cidx = _NIDX_CODE;
        if( seg->attr & GIVEN_NAME ) {
            rec->nidx = GetNameIdx( back, seg->str, "" );
        } else if( back.x86omf->CodeGroupGIdx != 0 ) {
            rec->nidx = GetNameIdx( back, back.x86omf->CodeGroup, seg->str );
        } else if( _IsTargetModel( CGSW_X86_BIG_CODE ) ) {
            rec->nidx = GetNameIdx( back, FEModuleName(*back.front), seg->str );
        } else {
            rec->nidx = GetNameIdx( back, seg->str, "" );
        }
        if( back.x86omf->CodeGroupGIdx != 0 ) {
            rec->base = back.x86omf->CodeGroupGIdx;
            rec->btype = BASE_GRP;
        } else {
            rec->base = back.x86omf->SegmentIndex;
            rec->btype = BASE_SEG;
        }
    } else {
        rec->exec = false;
        if( seg->attr & ROM )
            rec->rom = true;
        if( seg->attr & PRIVATE ) {
            rec->nidx = GetNameIdx( back, seg->str, "" );
            if( seg->attr & ROM ) {
                if( back.x86omf->PrivateIndexRO == 0 ) {
                    back.x86omf->PrivateIndexRO = GetNameIdx( back, "FAR_CONST", "" );
                }
                rec->cidx = back.x86omf->PrivateIndexRO;
            } else {
                if( back.x86omf->PrivateIndexRW == 0 ) {
                    back.x86omf->PrivateIndexRW = GetNameIdx( back, "FAR_DATA", "" );
                }
                rec->cidx = back.x86omf->PrivateIndexRW;
            }
            rec->base = back.x86omf->SegmentIndex;
            rec->btype = BASE_SEG;
        } else {
            if( seg->attr & THREAD_LOCAL ) {
                if( back.x86omf->TLSGIndex == 0 ) {
                    OutIdx( back, _NIDX_TLS, tgroup_def );
                }
                OutGroup( back, rec->sidx, tgroup_def, &back.x86omf->TLSGIndex );
                rec->base = back.x86omf->TLSGIndex;
                rec->btype = BASE_GRP;
            } else if( back.x86omf->NoDGroup ) {
                rec->btype = BASE_SEG;
                rec->base = back.x86omf->SegmentIndex;
            } else {
                OutGroup( back, rec->sidx, dgroup_def, &back.x86omf->DGroupIndex );
                rec->base = back.x86omf->DGroupIndex;
                rec->btype = BASE_GRP;
            }
            if( seg->attr & THREAD_LOCAL ) {
                rec->cidx = _NIDX_TLS;
            } else if( seg->attr & COMMON ) {
                rec->cidx = _NIDX_DATA;
            } else if( ( seg->attr & ( BACK | INIT ) ) == 0 ) {
                rec->cidx = _NIDX_BSS;
            } else {
                rec->cidx = _NIDX_DATA;
            }
            rec->nidx = GetNameIdx( back, back.x86omf->DataGroup, seg->str );
        }
    }
    SegmentClass( back, rec );
    DoASegDef( back, rec, use_16 );
    if( (seg->attr & EXEC) && back.x86omf->CodeGroupGIdx != 0 ) {
        OutCGroup( back, rec->sidx, &rec->obj->data );
    }
}

static segdef *AllocSegDefs( BackRun& back, const char *str )
{
    segdef  *seg;
    segdef  **owner;

    seg = static_cast<segdef *>(CGAlloc( back, sizeof( segdef ) ));
    seg->next = NULL;
    seg->str = static_cast<char *>(CGAlloc( back, Length( str ) + 1 ));
    CopyStr( str, seg->str );
    for( owner = &back.x86omf->SegDefs; *owner != NULL; ) {
        owner = &(*owner)->next;
    }
    *owner = seg;
    return( seg );
}

static void DoSegDefsAndFree( BackRun& back, array_control *dgroup_def, array_control *tgroup_def, bool use_16 )
{
    segdef  *seg;

    while( (seg = back.x86omf->SegDefs) != NULL ) {
        back.x86omf->SegDefs = seg->next;
        DoSegment( back, seg, dgroup_def, tgroup_def, use_16 );
        CGFree( back, seg->str );
        CGFree( back, seg );
    }
}

static void    defSegment( BackRun& back, segment_id segid, seg_attr attr, const char *str, uint align, bool use_16 )
/*********************************************************************************************/
{
    segdef              *new_seg;
    segment_id          first_code_segid;

    new_seg = AllocSegDefs( back, str );
    new_seg->segid = segid;
    new_seg->attr = attr;
    new_seg->align = align;
    first_code_segid = BACKSEGS;
    if( attr & EXEC ) {
        if( back.x86omf->codeSegId == BACKSEGS ) {
            back.x86omf->codeSegId = segid;
            first_code_segid = segid;
        }
        if( back.data.OptForSize == 0 ) {
            if( new_seg->align < 16 ) {
                new_seg->align = 16;
            }
        }
    }
    if( attr & BACK ) {
        back.x86omf->dataSegId = segid;
    }
    if( back.x86omf->NameIndex != 0 ) {
        /*
         * we are after BEStart processing
         *
         * don't use DGROUP for segments defined after BEStart
         */
        DoSegDefsAndFree( back, NULL, NULL, use_16 );
    }
    if( first_code_segid != BACKSEGS
      && _IsModel( CGSW_GEN_DBG_DF ) ) {
        DFBegCCU( back, first_code_segid, NULL );
    }
}

static void doEmptyQueue( BackRun& back )
/***********************/
{
    EmptyQueue(back);
    TellUnreachLabels(back);
}

static  void    FPInitPatches( BackRun& back )
/***********************************/
{
    int i;

    for( i = FPP_NUMBER_OF_TYPES; i-- > 0; ) {
        back.x86omf->FPPatchImp[i] = NOT_IMPORTED;
    }
}

static  void    OutName( BackRun& back, const char *name, array_control *dest )
/**************************************************************/
{
    uint            len;

    len = Length( name );
    if( len >= 256 ) {
        len = 255;
        FEMessage( *back.front, FEMSG_INFO_PROC, const_cast<pointer>(static_cast<const void *>("Code generator truncated name, its length exceeds allowed maximum.")) );
        FEMessage( *back.front, FEMSG_INFO_PROC, (pointer)name );
    }
    OutByte( back, len, dest );
    OutBuffer( back, name, len, dest );
}

static  void    OutObjectName( BackRun& back, cg_sym_handle sym, array_control *dest )
/*********************************************************************/
{
    OUTPUT_OBJECT_NAME( sym, OutName, dest, NORMAL );
}

static  void    OutString( BackRun& back, const char *name, array_control *dest )
/****************************************************************/
{
    uint    len;

    len = Length( name );
    OutBuffer( back, name, len, dest );
}

static char getMemModel( BackRun& back )
/**********************/
{
    char model;

    if( _IsTargetModel( CGSW_X86_BIG_CODE ) ) {
        if( _IsTargetModel( CGSW_X86_BIG_DATA ) ) {
            if( _IsntTargetModel( CGSW_X86_CHEAP_POINTER ) ) {
                model = 'h';
            } else {
                model = 'l';
            }
        } else {
            model = 'm';
        }
    } else if( _IsTargetModel( CGSW_X86_BIG_DATA ) ) {
        model = 'c';
    } else if( _IsTargetModel( CGSW_X86_FLAT_MODEL ) ) {
        model = 'f';
    } else {
        model = 's';
    }
    return( model );
}

static void OutModel( BackRun& back, array_control *dest )
/*****************************************/
{
    char                model[6];

    if( _CPULevel( CPU_386 ) ) {
        model[0] = '3';
    } else if( _CPULevel( CPU_286 ) ) {
        model[0] = '2';
    } else {
        model[0] = '0';
    }
    model[1] = getMemModel(back);
    model[2] = 'O';     /* Why? Ask microsoft! */
    if( _FPULevel( FPU_87 ) ) {
        if( _IsEmulation() ) {
            model[3] = 'e';
        } else {
            model[3] = 'p';
        }
    } else {
        model[3] = 'c';
    }
    model[4] = 'd';
    if( _IsModel( CGSW_GEN_POSITION_INDEPENDANT ) ) {
        model[4] = 'i';
    }
    model[5] = '\0';
    OutString( back, model, dest );
}

static segment_id dbgSegDef( BackRun& back, const char *seg_name, const char *seg_class, int seg_modifier )
/***********************************************************************************/
{
    index_rec   *rec;

    rec = AllocNewSegRec(back);
    rec->sidx = ++back.x86omf->SegmentIndex;
    rec->cidx = GetNameIdx( back, seg_class, "" );
    rec->nidx = GetNameIdx( back, seg_name, "" );
    rec->location = 0;
    rec->big = 0;
    rec->need_base_set = true;
    rec->data_ptr_in_code = false;
    rec->data_in_code = false;
    rec->start_data_in_code = false;
    rec->private_ = true;
    rec->exec = false;
    rec->rom = false;
    rec->prefix_comdat_state = PCS_OFF;
    rec->max_written = 0;
    rec->max_size = 0;
    rec->comdat_label = NULL;
    rec->comdat_symbol = NULL;
    rec->total_comdat_size = 0;
    rec->comdat_nidx = 0;
    rec->comdat_size = 0;
    rec->virt_func_refs = NULL;
    rec->segid = --back.x86omf->backSegId;
    rec->attr = SEG_ALGN_BYTE | seg_modifier;
    rec->data_prefix_size = 0;
    rec->comdat_prefix_import = NOT_IMPORTED;
    rec->base = rec->sidx;
    rec->btype = BASE_SEG;
    DoASegDef( back, rec, true );
    return( rec->segid );
}

static  void    DoSegGrpNames( BackRun& back, array_control *dgroup_def, array_control *tgroup_def )
/***********************************************************************************/
{
    char        *dgroup;
    omf_idx     dgroup_idx;

    GetNameIdx( back, "", "" );       /* _NIDX_NULL */
    GetNameIdx( back, "CODE", "" );   /* _NIDX_CODE */
    GetNameIdx( back, "DATA", "" );   /* _NIDX_DATA */
    GetNameIdx( back, "BSS", "" );    /* _NIDX_BSS */
    GetNameIdx( back, "TLS", "" );    /* _NIDX_TLS */

#if _TARGET & _TARG_80386
    if( _IsTargetModel( CGSW_X86_FLAT_MODEL )
      && _IsntTargetModel( CGSW_X86_EZ_OMF ) ) {
        back.x86omf->FlatNIndex = GetNameIdx( back, "FLAT", "" );
    }
#endif
    back.x86omf->SegmentIndex = 0;
    back.x86omf->PrivateIndexRO = 0;
    back.x86omf->PrivateIndexRW = 0;
    CopyStr( static_cast<const char *>(FEAuxInfo( *back.front, NULL, FEINF_CODE_GROUP )), back.x86omf->CodeGroup );
    if( back.x86omf->CodeGroup[0] == NULLCHAR ) {
        back.x86omf->CodeGroupGIdx = 0;
    } else {
        back.x86omf->CodeGroupNIdx = GetNameIdx( back, back.x86omf->CodeGroup, "" );
        back.x86omf->CodeGroupGIdx = ++back.x86omf->GroupIndex;
    }
    dgroup = static_cast<char *>(FEAuxInfo( *back.front, NULL, FEINF_DATA_GROUP ));
    if( dgroup == NULL ) {
        back.x86omf->NoDGroup = true;
    } else {
        CopyStr( dgroup, back.x86omf->DataGroup );
    }
    if( back.x86omf->DataGroup[0] != NULLCHAR ) {
        back.model.TargetModel |= CGSW_X86_FLOATING_SS;
        dgroup_idx = GetNameIdx( back, back.x86omf->DataGroup, "_GROUP" );
    } else {
        dgroup_idx = GetNameIdx( back, "DGROUP", "" );
    }
    OutIdx( back, dgroup_idx, dgroup_def );
    FillArray( back, &back.x86omf->SegInfo, sizeof( index_rec ), MODEST_INFO, INCREMENT_INFO );
    /*
     * we are in BEStart processing
     *
     * define all default segments defined by front-end
     * use DGROUP for these segments
     */
    DoSegDefsAndFree( back, dgroup_def, tgroup_def, false );
    if( _IsModel( CGSW_GEN_DBG_DF ) ) {
        if( _IsModel( CGSW_GEN_DBG_LOCALS )
          || _IsModel( CGSW_GEN_DBG_TYPES ) ) {
            DFDefSegs(back);
        }
    } else if( _IsModel( CGSW_GEN_DBG_CV ) ) {
        CVDefSegs(back);
    } else {
        back.x86omf->DbgTypeSize = 0;
        if( _IsModel( CGSW_GEN_DBG_LOCALS ) ) {
            back.data.DbgLocals = dbgSegDef( back, back.x86omf->DbgSegs[0].seg_name, back.x86omf->DbgSegs[0].class_name, SEG_COMB_PRIVATE );
        }
        if( _IsModel( CGSW_GEN_DBG_TYPES ) ) {
            back.data.DbgTypes = dbgSegDef( back, back.x86omf->DbgSegs[1].seg_name, back.x86omf->DbgSegs[1].class_name, SEG_COMB_PRIVATE );
        }
    }
}


static  void    KillStatic( BackRun& back, array_control *arr )
/**********************************************/
{
    CGFree( back, arr->array );
    arr->array = NULL;
    arr->alloc = 0;
    arr->used = 0;
}

static  void    KillArray( BackRun& back, array_control *arr )
/*********************************************/
{
    CGFree( back, arr->array );
    CGFree( back, arr );
}

static void initImports( BackRun& back )
{
    back.x86omf->ImportHdl = IMPORT_BASE;
    FillArray( back, &back.x86omf->Imports, sizeof( byte ), MODEST_IMP, INCREMENT_IMP );
    back.x86omf->GenStaticImports = false;
}

static void finiImports( BackRun& back )
{
    KillStatic( back, &back.x86omf->Imports );
}

static omf_idx getImportHdl( BackRun& back )
{
    if( back.x86omf->ImportHdl >= 0x7FFF ) {
        FatalError( back, "OMF file too many external symbols" );
    }
    return( back.x86omf->ImportHdl++ );
}

static time_t timet2dosu( time_t stamp )
/**************************************/
{
    struct tm       *t;
    unsigned short  dos_time;
    unsigned short  dos_date;

    t = gmtime( &stamp );

    dos_date = ( ( t->tm_year - 80 ) << DATE_YEAR_B )
             | ( ( t->tm_mon + 1 ) << DATE_MON_B )
             | ( t->tm_mday << DATE_DAY_B );
    dos_time = ( ( t->tm_hour ) << TIME_HOUR_B )
             | ( t->tm_min << TIME_MIN_B )
             | ( ( t->tm_sec / 2 ) << TIME_SEC_B );
    return( dos_date * 0x10000UL + dos_time );
}

static void    objInit( BackRun& back )
/*********************/
{
    array_control       *names;         /* for LNAMES*/
    array_control       *dgroup_def;
    array_control       *tgroup_def;
    void                *depend;

    FPInitPatches(back);
    back.x86omf->CodeSize = 0;
    back.x86omf->DataSize = 0;
    back.x86omf->CurrFNo = 0;
    OpenObj(back);
    names = InitArray( back, sizeof( byte ), MODEST_HDR, INCREMENT_HDR );
    OutName( back, static_cast<const char *>(FEAuxInfo( *back.front, NULL, FEINF_SOURCE_NAME )), names );
    PutObjOMFRec( back, CMD_THEADR, names );
#if _TARGET & _TARG_80386
    if( _IsTargetModel( CGSW_X86_EZ_OMF )
      || _IsTargetModel( CGSW_X86_FLAT_MODEL ) ) {
        OutShort( back, PHARLAP_OMF_COMMENT, names );
        if( _IsntTargetModel( CGSW_X86_EZ_OMF ) ) {
            OutString( back, "OS220", names );
        } else {
            OutString( back, "80386", names );
        }
        PutObjOMFRec( back, CMD_COMENT, names );
    }
#else
    OutShort( back, MS_OMF_COMMENT, names );
    PutObjOMFRec( back, CMD_COMENT, names );
#endif
    OutShort( back, MODEL_COMMENT, names );
    OutModel( back, names );
    PutObjOMFRec( back, CMD_COMENT, names );
    if( _IsTargetModel( CGSW_X86_FLAT_MODEL )
      && _IsModel( CGSW_GEN_DBG_DF ) ) {
        OutShort( back, LINKER_COMMENT, names );
        OutByte( back, LDIR_FLAT_ADDRS, names );
        PutObjOMFRec( back, CMD_COMENT, names );
    }
    if( _IsntModel( CGSW_GEN_DBG_DF )
      && _IsntModel( CGSW_GEN_DBG_CV ) ) {
        OutShort( back, LINKER_COMMENT, names );
        OutByte( back, LDIR_SOURCE_LANGUAGE, names );
        OutByte( back, DEBUG_MAJOR_VERSION, names );
        if( _IsModel( CGSW_GEN_DBG_TYPES )
          || _IsModel( CGSW_GEN_DBG_LOCALS ) ) {
            OutByte( back, DEBUG_MINOR_VERSION, names );
        } else {
            OutByte( back, 0, names );
        }
        OutString( back, (const char *)FEAuxInfo( *back.front, NULL, FEINF_SOURCE_LANGUAGE ), names );
        PutObjOMFRec( back, CMD_COMENT, names );
    }
    for( depend = NULL; (depend = FEAuxInfo( *back.front, depend, FEINF_NEXT_DEPENDENCY )) != NULL; ) {
        OutShort( back, DEPENDENCY_COMMENT, names );
        /*
         * OMF use dos time/date format
         * for better portability, we use UTC instead of local time,
         * so as not to be dependent on the time zone
         */
        OutLongInt( back, timet2dosu( *(time_t *)FEAuxInfo( *back.front, depend, FEINF_DEPENDENCY_TIMESTAMP ) ), names );
        OutName( back, static_cast<const char *>(FEAuxInfo( *back.front, depend, FEINF_DEPENDENCY_NAME )), names );
        PutObjOMFRec( back, CMD_COMENT, names );
    }
    /*
     * mark end of dependancy list
     */
    OutShort( back, DEPENDENCY_COMMENT, names );
    PutObjOMFRec( back, CMD_COMENT, names );
    KillArray( back, names );

    dgroup_def = InitArray( back, sizeof( byte ), MODEST_INFO, INCREMENT_INFO );
    tgroup_def = InitArray( back, sizeof( byte ), MODEST_INFO, INCREMENT_INFO );
    setOP( back, UNDEFSEG );
    DoSegGrpNames( back, dgroup_def, tgroup_def );
    if( dgroup_def->used > 0 ) {
        FlushNames(back);
        PutObjOMFRec( back, CMD_GRPDEF, dgroup_def );
    }
    if( tgroup_def->used > 0 ) {
        FlushNames(back);
        PutObjOMFRec( back, CMD_GRPDEF, tgroup_def );
    }
    KillArray( back, tgroup_def );
#if _TARGET & _TARG_80386
    if( _IsTargetModel( CGSW_X86_FLAT_MODEL )
      && _IsntTargetModel( CGSW_X86_EZ_OMF ) ) {
        back.x86omf->FlatGIndex = ++back.x86omf->GroupIndex;
        OutIdx( back, back.x86omf->FlatNIndex, dgroup_def );
        FlushNames(back);
        PutObjOMFRec( back, CMD_GRPDEF, dgroup_def );
    }
#endif
    KillArray( back, dgroup_def );
    setOP( back, back.x86omf->codeSegId );
    initImports(back);
    back.x86omf->AbsPatches = NULL;
    if( _IsModel( CGSW_GEN_DBG_DF ) ) {
        if( _IsModel( CGSW_GEN_DBG_LOCALS )
          || _IsModel( CGSW_GEN_DBG_TYPES ) ) {
            DFObjInitDbgInfo(back);
        }
    } else if( _IsModel( CGSW_GEN_DBG_CV ) ) {
        CVObjInitDbgInfo(back);
    } else {
        WVObjInitDbgInfo(back);
    }
}


static offset  askLocation( BackRun& back )
/*************************/
{
    return( (offset)back.x86omf->CurrSeg->location );
}

static void chkDbgSegSize( BackRun& back, offset max, bool typing )
/*******************************************/
{
    const dbg_seg_info  *info;
    long_offset         curr;

    info = back.x86omf->DbgSegs;
    if( typing )
        info++;
    PUSH_OP( *info->segid );
        curr = (offset)back.x86omf->CurrSeg->location;
        if( curr >= max ) {
            if( typing ) {
                back.x86omf->DbgTypeSize += curr;
            }
            *info->segid = dbgSegDef( back, info->seg_name, info->class_name, SEG_COMB_PRIVATE );
        }
    POP_OP();
}


static bool    useImportForm( BackRun& back, fe_attr attr )
/***********************************/
{
    if( attr & (FE_GLOBAL|FE_IMPORT) )
        return( true );
    if( attr & FE_INTERNAL )
        return( false );
    return( true );
}


static bool    askSegIsNear( BackRun& back, segment_id segid )
/**************************************/
{
    index_rec   *rec;

    if( segid < 0 )
        return( false );
    rec = AskSegIndex( back, segid );
    if( rec->btype != BASE_GRP )
        return( false );
    if( rec->base > back.x86omf->CodeGroupGIdx )
        return( true );
    return( false );
}


static bool    askSegIsBlank( BackRun& back, segment_id segid )
/***************************************/
{
    index_rec *rec;

    if( segid < 0 )
        return( true );
    rec = AskSegIndex( back, segid );
    return( rec->cidx == _NIDX_BSS );
}


static bool    askSegIsPrivate( BackRun& back, segment_id segid )
/*****************************************/
{
    index_rec   *rec;

    if( segid < 0 )
        return( true );
    rec = AskSegIndex( back, segid );
    return( rec->private_ || rec->exec );
}


static bool    askSegIsROM( BackRun& back, segment_id segid )
/*************************************/
{
    index_rec   *rec;

    if( segid < 0 )
        return( false );
    rec = AskSegIndex( back, segid );
    return( rec->rom != 0 );
}


static segment_id  askBackSeg( BackRun& back )
/****************************/
{
    return( back.x86omf->dataSegId );
}


static segment_id  askCodeSeg( BackRun& back )
/****************************/
{
    return( back.x86omf->codeSegId );
}


static bool    haveCodeSeg( BackRun& back )
/*************************/
{
    return( back.x86omf->codeSegId != BACKSEGS );
}


static segment_id  askAltCodeSeg( BackRun& back )
/*******************************/
{
    return( altCodeSegId );
}

static segment_id  askCode16Seg( BackRun& back )
/******************************/
{

    if( back.x86omf->askCode16Seg_Code16Seg == BACKSEGS ) {
        back.x86omf->askCode16Seg_Code16Seg = --back.x86omf->backSegId;
        defSegment( back, back.x86omf->askCode16Seg_Code16Seg, EXEC | GIVEN_NAME, "_TEXT16", 16, true );
    }
    return( back.x86omf->askCode16Seg_Code16Seg );
}


static  void    EjectImports( BackRun& back )
/**********************************/
{
    omf_cmd     cmd;

    if( back.x86omf->Imports.used > 0 ) {
        if( back.x86omf->GenStaticImports ) {
            cmd = CMD_LEXTDEF;
        } else {
            cmd = CMD_EXTDEF;
        }
        PutObjOMFRec( back, cmd, &back.x86omf->Imports );
    }
}


static  void    SetPatches( BackRun& back )
/********************************/
{
    temp_patch          *tpatch;
    array_control       *lbl_patches;
    obj_patch           *patch;

    while( (tpatch = back.x86omf->CurrSeg->obj->tpatches) != NULL ) {
        back.x86omf->CurrSeg->obj->tpatches = tpatch->link;
        lbl_patches = static_cast<array_control *>(AskLblPatches( back, tpatch->lbl ));
        patch = static_cast<obj_patch *>(allocOut( back, lbl_patches, 1 ));
        patch->ref = AskObjHandle(back);
        patch->where = tpatch->patch.where;
        patch->attr = tpatch->patch.attr;
        CGFree( back, tpatch );
    }
}


static  void    SetAbsPatches( BackRun& back )
/***********************************/
{
    abs_patch   *apatch;
    object      *obj;

    obj = back.x86omf->CurrSeg->obj;
    for( apatch = back.x86omf->AbsPatches; apatch != NULL; apatch = apatch->link ) {
        if( apatch->patch.ref == INVALID_OBJHANDLE && apatch->obj == obj ) {
            apatch->patch.ref = AskObjHandle(back);
            apatch->flags |= AP_HAVE_OFFSET;
        }
    }
}


static void     EjectLEData( BackRun& back )
/*********************************/
{
    array_control   *obj_data;
    omf_cmd         cmd;

    EjectImports(back);
    if( back.x86omf->CurrSeg->obj->data.used > back.x86omf->CurrSeg->data_prefix_size ) {
        SetPatches(back);
        SetAbsPatches(back);
        obj_data = &back.x86omf->CurrSeg->obj->data;
        if( back.x86omf->CurrSeg->comdat_label != NULL ) {
#if _TARGET & _TARG_80386
            if( _IsntTargetModel( CGSW_X86_EZ_OMF ) ) {
                cmd = CMD_COMDAT32;
            } else {
                cmd = CMD_COMDAT;
            }
#else
            cmd = CMD_COMDAT;
#endif
        } else {
#if _TARGET & _TARG_80386
            if( _IsntTargetModel( CGSW_X86_EZ_OMF ) ) {
                cmd = CMD_LEDATA32;
            } else {
                cmd = CMD_LEDATA;
            }
#else /* SEG32DBG dwarf, codeview */
            if( (back.x86omf->CurrSeg->attr & SEG_USE_32)
              && _IsntTargetModel( CGSW_X86_EZ_OMF ) ) {
                cmd = CMD_LEDATA32;
            } else {
                cmd = CMD_LEDATA;
            }
#endif
        }
        PutObjOMFRec( back, cmd, obj_data );
        if( back.x86omf->CurrSeg->obj->fixes.used > 0 ) {
            if( back.x86omf->CurrSeg->data_ptr_in_code ) {
                OutShort( back, LINKER_COMMENT, obj_data );
                OutByte( back, LDIR_OPT_UNSAFE, obj_data );
                PutObjOMFRec( back, CMD_COMENT, obj_data );
            }
#if _TARGET & _TARG_80386
            if( _IsntTargetModel( CGSW_X86_EZ_OMF ) ) {
                cmd = CMD_FIXUPP32;
            } else {
                cmd = CMD_FIXUPP;
            }
#else
            cmd = CMD_FIXUPP;
#endif
            PutObjOMFRec( back, cmd, &back.x86omf->CurrSeg->obj->fixes );
        }
        back.x86omf->CurrSeg->data_ptr_in_code = false;
    }
}


static void GetSymLName( BackRun& back, const char *name, omf_idx *nidx )
/********************************************************/
{
    *nidx = GetNameIdx( back, name, "" );
}

static omf_idx NeedComdatNidx( BackRun& back, import_kind kind )
/***********************************************/
{
    if( back.x86omf->CurrSeg->comdat_nidx == 0 ) {
        OUTPUT_OBJECT_NAME( back.x86omf->CurrSeg->comdat_symbol, GetSymLName, &back.x86omf->CurrSeg->comdat_nidx, kind );
        FlushNames(back);
    }
    return( back.x86omf->CurrSeg->comdat_nidx );
}


static void    outSelect( BackRun& back, bool starts )
/******************************/
{
    array_control   *obj_data;

    if( starts ) {
        back.x86omf->selIdx = back.x86omf->CurrSeg->sidx;
        back.x86omf->SelStart = (offset)back.x86omf->CurrSeg->location;
    } else if( back.x86omf->selIdx != 0 ) {
        if( !back.x86omf->CurrSeg->start_data_in_code ) {
            EjectLEData(back);
            obj_data = &back.x86omf->CurrSeg->obj->data;
            OutShort( back, DISASM_COMMENT, obj_data );
#if _TARGET & _TARG_80386
            OutByte( back, DDIR_SCAN_TABLE_32, obj_data );
#else
            OutByte( back, DDIR_SCAN_TABLE, obj_data );
#endif
            if( back.x86omf->CurrSeg->comdat_label != NULL ) {
                OutIdx( back, 0, obj_data );
                OutIdx( back, NeedComdatNidx( back, NORMAL ), obj_data );
            } else {
                OutIdx( back, back.x86omf->selIdx, obj_data );
            }
            OutOffset( back, back.x86omf->SelStart, obj_data );
            OutOffset( back, (offset)back.x86omf->CurrSeg->location, obj_data );
            PutObjOMFRec( back, CMD_COMENT, obj_data );
        }
        back.x86omf->selIdx = 0;
    }
    back.x86omf->CurrSeg->start_data_in_code = false;
}

static  void    OutLEDataStart( BackRun& back, bool iterated )
/*********************************************/
{
    byte            flag;
    array_control   *obj_data;

    obj_data = &back.x86omf->CurrSeg->obj->data;
    if( obj_data->used == 0 ) {
        if( back.x86omf->CurrSeg->comdat_label != NULL ) {
            flag = 0;
            if( back.x86omf->CurrSeg->location != 0 )
                flag |= 1; /* continued */
            if( iterated )
                flag |= 2;   /* LIDATA form */
            if( (FEAttr( *back.front, back.x86omf->CurrSeg->comdat_symbol ) & FE_GLOBAL) == 0 ) {
                flag |= 0x4;    /* local comdat */
            }
            OutByte( back, flag, obj_data );
            OutByte( back, 0x10, obj_data );
            OutByte( back, 0, obj_data );
            OutOffset( back, (offset)back.x86omf->CurrSeg->location, obj_data );
            OutIdx( back, 0, obj_data );
            if( back.x86omf->CurrSeg->btype == BASE_GRP ) {
                OutIdx( back, back.x86omf->CurrSeg->base, obj_data );  /* group index*/
            } else {
                OutIdx( back, 0, obj_data );
            }
            OutIdx( back, back.x86omf->CurrSeg->sidx, obj_data );      /* segment index*/
            OutIdx( back, NeedComdatNidx( back, NORMAL ), obj_data );
        } else { /* LEDATA */
            OutIdx( back, back.x86omf->CurrSeg->sidx, obj_data );
#if _TARGET & _TARG_80386
            OutOffset( back, (offset)back.x86omf->CurrSeg->location, obj_data );
#else /* SEG32DBG dwarf, codeview */
            if( back.x86omf->CurrSeg->attr & SEG_USE_32 ) {
                OutLongOffset( back, back.x86omf->CurrSeg->location, obj_data );
            } else {
                OutOffset( back, (offset)back.x86omf->CurrSeg->location, obj_data );
            }
#endif
        }
        back.x86omf->CurrSeg->obj->start = back.x86omf->CurrSeg->location;
        back.x86omf->CurrSeg->data_prefix_size = obj_data->used;
    }
    if( back.x86omf->CurrSeg->start_data_in_code ) {
        outSelect( back, true );
    }
}


static  void    CheckLEDataSize( BackRun& back, unsigned max_size, bool need_init )
/******************************************************************/
{
    long_offset     start;
    unsigned        used;
    object          *obj;
    long_offset     end_valid;

    obj = back.x86omf->CurrSeg->obj;
    start = obj->start;
    used = obj->data.used;
    if( back.x86omf->CurrSeg->location < start ) {
        EjectLEData(back);
    } else if( back.x86omf->CurrSeg->location - start + max_size > BUFFSIZE - TOLERANCE - back.x86omf->CurrSeg->data_prefix_size ) {
        EjectLEData(back);
    } else {
        end_valid = start + used - back.x86omf->CurrSeg->data_prefix_size;
        if( back.x86omf->CurrSeg->max_written > end_valid && back.x86omf->CurrSeg->location > end_valid ) {
            EjectLEData(back);
        }
    }
    if( need_init ) {
        OutLEDataStart( back, false );
    }
}

static void    setUpObj( BackRun& back, bool is_data )
/******************************/
{
    object      *obj;
    bool        old_data;

    obj = back.x86omf->CurrSeg->obj;
    if( obj == NULL )
        return;
    if( obj->fixes.used >= BUFFSIZE - TOLERANCE ) {
        EjectLEData(back);
        return;
    }
    if( back.x86omf->Imports.used >= BUFFSIZE - TOLERANCE ) {
         EjectLEData(back);
         return;
    }
    /* so that a call will always fit */
    CheckLEDataSize( back, 4 * sizeof( offset ), false );
    if( back.x86omf->CurrSeg->exec ) {
        old_data = ( back.x86omf->CurrSeg->data_in_code != 0 );
        back.x86omf->CurrSeg->data_in_code = is_data;
        if( is_data != old_data ) {
            if( is_data ) {
                back.x86omf->CurrSeg->start_data_in_code = true;
            } else {
                outSelect( back, false );
                setUpObj( back, false );
            }
        }
    }
}


static  void    GenComdef( BackRun& back )
/*******************************/
{
    array_control       *comdef;
    unsigned            count;
    unsigned_8          ind;
    unsigned            size;
    cg_sym_handle       sym;
    omf_cmd             cmd;

    if( back.x86omf->CurrSeg->comdat_label != NULL &&
        back.x86omf->CurrSeg->max_written < back.x86omf->CurrSeg->comdat_size ) {
        if( back.x86omf->CurrSeg->max_written != 0 ) {
            Zoiks( back, ZOIKS_080 );
        }
        /*
         * have to eject any pending imports here or the ordering
         * gets messed up
         */
        EjectImports(back);
        size = back.x86omf->CurrSeg->comdat_size - back.x86omf->CurrSeg->max_written;
        comdef = InitArray( back, sizeof( byte ), MODEST_EXP, INCREMENT_EXP );
        sym = back.x86omf->CurrSeg->comdat_symbol;
        OutObjectName( back, sym, comdef );
        OutByte( back, 0, comdef );                  /* type index */
        if( back.x86omf->CurrSeg->btype == BASE_GRP && back.x86omf->CurrSeg->base == back.x86omf->DGroupIndex ) {
            OutByte( back, COMDEF_NEAR, comdef );    /* common type */
        } else {
            OutByte( back, COMDEF_FAR, comdef );     /* common type */
            OutByte( back, 1, comdef );              /* number of elements */
        }
        /*
         * Strictly speaking, this should be <= 0x80. However a number
         * of tools (including our own!) have problems with doing a
         * 128 byte COMDEF size in 1 byte, so we'll waste some space
         * and use 2 bytes, but sleep much sounder at night.
         */
        if( size < 0x80 ) {
            count = 1;
            ind = 0;
        } else if( size < 0x1000 ) {
            count = 2;
            ind = COMDEF_LEAF_2;
        } else if( size < 0x100000 ) {
            count = 3;
            ind = COMDEF_LEAF_3;
        } else {
            count = 4;
            ind = COMDEF_LEAF_4;
        }
        if( ind != 0 ) {
            /*
             * multi-byte indicator
             */
            OutByte( back, ind, comdef );
        }
        do {
            /*
             * element size
             */
            OutByte( back, (byte)size, comdef );
            size >>= 8;
            --count;
        } while( count != 0 );
        if( FEAttr( *back.front, sym ) & FE_GLOBAL ) {
            cmd = CMD_COMDEF;
        } else {
            cmd = CMD_LCOMDEF;
        }
        PutObjOMFRec( back, cmd, comdef );
        KillArray( back, comdef );
        TellImportHandle( back, sym, getImportHdl(back) );
    }
}

static  void    EjectExports( BackRun& back )
/**********************************/
{
    omf_cmd         cmd;

    if( back.x86omf->CurrSeg->obj->exports.used > 0 ) {
        if( back.x86omf->CurrSeg->obj->gen_static_exports ) {
#if _TARGET & _TARG_80386
            if( _IsntTargetModel( CGSW_X86_EZ_OMF ) ) {
                cmd = CMD_LPUBDEF32;
            } else {
                cmd = CMD_LPUBDEF;
            }
#else
            cmd = CMD_LPUBDEF;
#endif
        } else {
#if _TARGET & _TARG_80386
            if( _IsntTargetModel( CGSW_X86_EZ_OMF ) ) {
                cmd = CMD_PUBDEF32;
            } else {
                cmd = CMD_PUBDEF;
            }
#else
            cmd = CMD_PUBDEF;
#endif
        }
        PutObjOMFRec( back, cmd, &back.x86omf->CurrSeg->obj->exports );
    }
}


static  void    FlushLineNum( BackRun& back, object *obj )
/*****************************************/
{
    omf_cmd     cmd;

    if( obj->last_line ) {
        OutShort( back, obj->last_line, obj->lines );
        OutOffset( back, obj->last_offset, obj->lines );
        if( back.x86omf->CurrSeg->comdat_label != NULL ) {
#if _TARGET & _TARG_80386
            if( _IsntTargetModel( CGSW_X86_EZ_OMF ) ) {
                cmd = CMD_LINSYM32;
            } else {
                cmd = CMD_LINSYM;
            }
#else
            cmd = CMD_LINSYM;
#endif
        } else {
#if _TARGET & _TARG_80386
            if( _IsntTargetModel( CGSW_X86_EZ_OMF ) ) {
                cmd = CMD_LINNUM32;
            } else {
                cmd = CMD_LINNUM;
            }
#else
            cmd = CMD_LINNUM;
#endif
        }
        PutObjOMFRec( back, cmd, obj->lines );
        obj->lines_generated = true;
        obj->last_line = 0;
        obj->last_offset = 0;
    }
}


static  void    FlushObject( BackRun& back )
/*********************************/
{
    object      *obj;

    setUpObj( back, false );
    GenComdef(back);
    back.x86omf->CurrSeg->total_comdat_size += back.x86omf->CurrSeg->comdat_size;
    EjectLEData(back);
    EjectExports(back);
    obj = back.x86omf->CurrSeg->obj;
    if( obj->lines != NULL ) {
        FlushLineNum( back, obj );
        KillArray( back, obj->lines );
        obj->lines = NULL;
    }
    freeObjCache(back);
}


static  index_rec       *AskIndexRec( BackRun& back, uint_16 sidx )
/**************************************************/
{
    unsigned    i;

    for( i = 0; i < back.x86omf->SegInfo.used; ++i ) {
        if( _ARRAYOF( &back.x86omf->SegInfo, index_rec )[i].sidx == sidx ) {
            return( _ARRAYOF( &back.x86omf->SegInfo, index_rec ) + i );
        }
    }
    return( NULL );
}

#if _TARGET & _TARG_8086
#define _TargetInt _TargetShort
#else
#define _TargetInt _TargetBigInt
#endif

static  void    FiniTarg( BackRun& back )
/******************************/
{
    byte        attr;
    index_rec   *rec;
    object      *obj;
    offset      size_s;
    long_offset size_l;

    FlushObject(back);
    obj = back.x86omf->CurrSeg->obj;
    rec = AskIndexRec( back, obj->index );
    size_l = rec->max_size;
    if( rec->exec ) {
        back.x86omf->CodeSize += size_l + rec->total_comdat_size;
    } else if( rec->cidx == _NIDX_DATA ) {
        back.x86omf->DataSize += size_l + rec->total_comdat_size;
    }
    if( rec->big ) {
        attr = rec->attr | SEG_BIG;
        PatchObj( back, obj->segfix, SEGDEF_ATTR, &attr, sizeof( attr ) );
    } else {
#if _TARGET & _TARG_80386
        size_s = _TargetInt( size_l );
        PatchObj( back, obj->segfix, SEGDEF_SIZE, (byte *)&size_s, sizeof( size_s ) );
#else /* SEG32DBG dwarf, codeview */
        if( rec->attr & SEG_USE_32 ) {
            size_l = _TargetLongInt( size_l );
            PatchObj( back, obj->segfix, SEGDEF_SIZE, (byte *)&size_l, sizeof( size_l ) );
        } else {
            size_s = _TargetInt( size_l );
            PatchObj( back, obj->segfix, SEGDEF_SIZE, (byte *)&size_s, sizeof( size_s ) );
        }
#endif
    }
    KillStatic( back, &obj->exports );
    KillStatic( back, &obj->data );
    KillStatic( back, &obj->fixes );
    CGFree( back, obj );
}

static void    flushOP( BackRun& back, segment_id segid )
/*********************************/
{
    index_rec   *rec;

    PUSH_OP( segid );
        if( segid == back.x86omf->codeSegId ) {
            doEmptyQueue(back);
        }
        if( _IsModel( CGSW_GEN_DBG_DF ) ) {
            rec = back.x86omf->CurrSeg;
            if( rec->exec || rec->cidx == _NIDX_DATA || rec->cidx == _NIDX_BSS ) {
                if( rec->max_size != 0 ) {
                    DFSegRange(back);
                }
            }
        }
        FiniTarg(back);
        back.x86omf->CurrSeg->obj = NULL;
    POP_OP();
}

static void FiniWVTypes( BackRun& back )
/*****************************/
{
    long_offset         curr;
    const dbg_seg_info  *info;

    WVTypesEof(back);
    info = &back.x86omf->DbgSegs[1];
    PUSH_OP( *info->segid );
        curr = (offset)back.x86omf->CurrSeg->location;
        curr += back.x86omf->DbgTypeSize;
        *info->segid = dbgSegDef( back, info->seg_name, info->class_name, SEG_COMB_PRIVATE );
        setOP( back, *info->segid );
        WVDmpCueInfo( back, curr );
    POP_OP();
}

static void FlushSelect( BackRun& back )
/*****************************/
{
    if( back.x86omf->selIdx != 0 ) {
        outSelect( back, false );
    }
}


static  void    FlushData( BackRun& back )
/*******************************/
{
    GenComdef(back);
    back.x86omf->CurrSeg->total_comdat_size += back.x86omf->CurrSeg->comdat_size;
    EjectLEData(back);
    FlushLineNum( back, back.x86omf->CurrSeg->obj );
    FlushSelect(back);
}

static  void    NormalData( BackRun& back )
/********************************/
{
    FlushData(back);
    back.x86omf->CurrSeg->location = back.x86omf->CurrSeg->max_written = back.x86omf->CurrSeg->max_size;
    back.x86omf->CurrSeg->comdat_size = 0;
    back.x86omf->CurrSeg->comdat_label = NULL;
    back.x86omf->CurrSeg->comdat_symbol = NULL;
    back.x86omf->CurrSeg->need_base_set = true;
    back.x86omf->CurrSeg->prefix_comdat_state = PCS_OFF;
    KillLblRedirects(back);
}


static void DoSegARange( BackRun& back, offset *codesize, index_rec *rec )
/*********************************************************/
{
    if( rec->exec || rec->cidx == _NIDX_DATA ||rec->cidx == _NIDX_BSS ) {
        if( rec->max_size != 0 ) {
            PUSH_OP( rec->segid );
                if( back.x86omf->CurrSeg->comdat_symbol != NULL ) {
                    DFSymRange( back, rec->comdat_symbol, (offset)rec->comdat_size );
                }
                NormalData(back);
                DFSegRange(back);
            POP_OP();
        }
        if( rec->exec ) {
            *codesize += rec->max_size + rec->total_comdat_size;
        }
    }
}

static  void    DoPatch( BackRun& back, obj_patch *patch, offset lc )
/****************************************************/
{
    uint_32     lword_val;
    uint_16     word_val;
    byte        byte_val;

    if( patch->attr & LONG_PATCH ) {
        if( patch->attr & ADD_PATCH ) {
            GetFromObj( back, patch->ref, patch->where, (byte *)&lword_val, sizeof( lword_val ) );
            _TargetAddL( lword_val, lc );
        } else {
            lword_val = _TargetLongInt( lc );
        }
        PatchObj( back, patch->ref, patch->where, (byte *)&lword_val, sizeof( lword_val ) );
    } else if( patch->attr & WORD_PATCH ) {
        if( patch->attr & ADD_PATCH ) {
            GetFromObj( back, patch->ref, patch->where, (byte *)&word_val, sizeof( word_val ) );
            _TargetAddW( word_val, lc );
        } else {
            word_val = _TargetShort( lc );
        }
        PatchObj( back, patch->ref, patch->where, (byte *)&word_val, sizeof( word_val ) );
    } else {
        if( patch->attr & ADD_PATCH ) {
            GetFromObj( back, patch->ref, patch->where, (byte *)&byte_val, sizeof( byte_val ) );
            byte_val += lc;
        } else {
            byte_val = lc;
        }
        PatchObj( back, patch->ref, patch->where, (byte *)&byte_val, sizeof( byte_val ) );
    }
}


static  void    FiniAbsPatches( BackRun& back )
/************************************/
{
    abs_patch   *apatch;

    while( (apatch = back.x86omf->AbsPatches) != NULL ) {
        back.x86omf->AbsPatches = apatch->link;
        DoPatch( back, &apatch->patch, (offset)apatch->value );
        CGFree( back, apatch );
    }
}


static  void    EndModule( BackRun& back )
/*******************************/
{
    byte          b;
    omf_cmd       cmd;
    array_control tmp;

    /*
     * There is a bug in MS's LINK386 program that causes it not to recognize a
     * MODEND386 record in some situations. We can get around it by only outputing
     * 16-bit MODEND records. This causes us no pain, since we never need any
     * features provided by the 32-bit form anyway.
     */
    cmd = CMD_MODEND;
    b = 0;                     /* non-main module, no start address*/
    tmp.used = sizeof( byte );
    tmp.array = &b;
    PutObjOMFRec( back, cmd, &tmp );
}


static void    objFini( BackRun& back )
/*********************/
{
    unsigned    i;
    pointer     auto_import;
    char        *lib;
    char        *alias;

    if( _IsModel( CGSW_GEN_DBG_DF ) ) {
        if( _IsModel( CGSW_GEN_DBG_LOCALS )
          || _IsModel( CGSW_GEN_DBG_TYPES ) ) {
            offset  codesize;

            codesize = 0;
            for( i = 0; i < back.x86omf->SegInfo.used; ++i ) {
                if( _ARRAYOF( &back.x86omf->SegInfo, index_rec )[i].obj != NULL ) {
                    DoSegARange( back, &codesize, _ARRAYOF( &back.x86omf->SegInfo, index_rec ) + i );
                }
            }
            DFObjFiniDbgInfo( back, codesize );
        }
    } else if( _IsModel( CGSW_GEN_DBG_CV ) ) {
        CVObjFiniDbgInfo();
    } else {
        if( _IsModel( CGSW_GEN_DBG_TYPES ) ) {
            FiniWVTypes(back);
        }
        WVObjFiniDbgInfo();
    }
    /*
     * Fini all segments.
     */
    for( i = 0; i < back.x86omf->SegInfo.used; ++i ) {
        if( _ARRAYOF( &back.x86omf->SegInfo, index_rec )[i].obj != NULL ) {
            back.x86omf->CurrSeg = _ARRAYOF( &back.x86omf->SegInfo, index_rec ) + i;
            FiniTarg(back);
        }
    }
    /*
     * if 8087 is used then enable emit default 8087 import records.
     */
    if( back.i87data.Used87 ) {
        (void)FEAuxInfo( *back.front, NULL, FEINF_USED_8087 );
    }
    /*
     * Emit default import records.
     */
    for( auto_import = NULL; (auto_import = FEAuxInfo( *back.front, auto_import, FEINF_NEXT_IMPORT )) != NULL; ) {
        OutName( back, static_cast<const char *>(FEAuxInfo( *back.front, auto_import, FEINF_IMPORT_NAME )), &back.x86omf->Imports );
        OutIdx( back, 0, &back.x86omf->Imports );           /* type index*/
        if( back.x86omf->Imports.used >= BUFFSIZE - TOLERANCE ) {
            PutObjOMFRec( back, CMD_EXTDEF, &back.x86omf->Imports );
        }
    }
    for( auto_import = NULL; (auto_import = FEAuxInfo( *back.front, auto_import, FEINF_NEXT_IMPORT_S )) != NULL; ) {
        OutObjectName( back, static_cast<cg_sym_handle>(FEAuxInfo( *back.front, auto_import, FEINF_IMPORT_NAME_S )), &back.x86omf->Imports );
        OutIdx( back, 0, &back.x86omf->Imports );           /* type index*/
        if( back.x86omf->Imports.used >= BUFFSIZE - TOLERANCE ) {
            PutObjOMFRec( back, CMD_EXTDEF, &back.x86omf->Imports );
        }
    }
    if( back.x86omf->Imports.used > 0 ) {
        PutObjOMFRec( back, CMD_EXTDEF, &back.x86omf->Imports );
    }
    /*
     * Emit default library search records.
     */
    for( lib = NULL; (lib = static_cast<char *>(FEAuxInfo( *back.front, lib, FEINF_NEXT_LIBRARY ))) != NULL; ) {
        OutShort( back, LIBNAME_COMMENT, &back.x86omf->Imports );
        OutString( back, (char *)FEAuxInfo( *back.front, lib, FEINF_LIBRARY_NAME ), &back.x86omf->Imports );
        PutObjOMFRec( back, CMD_COMENT, &back.x86omf->Imports );
    }
    /*
     * Emit alias definition records.
     */
    for( alias = NULL; (alias = static_cast<char *>(FEAuxInfo( *back.front, alias, FEINF_NEXT_ALIAS ))) != NULL; ) {
        char    *alias_name;
        char    *subst_name;

        alias_name = static_cast<char *>(FEAuxInfo( *back.front, alias, FEINF_ALIAS_NAME ));
        if( alias_name == NULL ) {
            OutObjectName( back, static_cast<cg_sym_handle>(FEAuxInfo( *back.front, alias, FEINF_ALIAS_SYMBOL )), &back.x86omf->Imports );
        } else {
            OutName( back, alias_name, &back.x86omf->Imports );
        }
        subst_name = static_cast<char *>(FEAuxInfo( *back.front, alias, FEINF_ALIAS_SUBST_NAME ));
        if( subst_name == NULL ) {
            OutObjectName( back, static_cast<cg_sym_handle>(FEAuxInfo( *back.front, alias, FEINF_ALIAS_SUBST_SYMBOL )), &back.x86omf->Imports );
        } else {
            OutName( back, subst_name, &back.x86omf->Imports );
        }
        PutObjOMFRec( back, CMD_ALIAS, &back.x86omf->Imports );
    }
    finiImports(back);
    KillStatic( back, &back.x86omf->SegInfo );
    FiniAbsPatches(back);
    EndModule(back);
    CloseObj(back);
    FEMessage( *back.front, FEMSG_CODE_SIZE, (pointer)(pointer_uint)back.x86omf->CodeSize );
    FEMessage( *back.front, FEMSG_DATA_SIZE, (pointer)(pointer_uint)back.x86omf->DataSize );
}


static  void    FreeAbsPatch( BackRun& back, abs_patch *apatch )
/***********************************************/
{
    abs_patch   **owner;

    for( owner = &back.x86omf->AbsPatches; *owner != apatch; ) {
        owner = &(*owner)->link;
    }
    *owner = (*owner)->link;
    CGFree( back, apatch );
}


static void CheckExportSwitch( BackRun& back, bool next_is_static )
/**************************************************/
{
    /*
     * are we switching from global to statics or vis-versa
     */
    if( back.x86omf->CurrSeg->obj->gen_static_exports != next_is_static ) {
        EjectExports(back);
        back.x86omf->CurrSeg->obj->gen_static_exports = next_is_static;
    }
}


static  void    CheckImportSwitch( BackRun& back, bool next_is_static )
/******************************************************/
{
    /*
     * are we switching from static imports to global or vis-versa
     */
    if( back.x86omf->GenStaticImports != next_is_static ) {
        EjectImports(back);
        back.x86omf->GenStaticImports = next_is_static;
    }
}


static  void    OutExport( BackRun& back, cg_sym_handle sym )
/*****************************************/
{
    array_control       *obj_exports;
    object              *obj;

    obj = back.x86omf->CurrSeg->obj;
    obj_exports = &obj->exports;
    if( obj_exports->used >= BUFFSIZE - TOLERANCE ) {
        EjectExports(back);
    }
    CheckExportSwitch( back, (FEAttr( *back.front, sym ) & FE_GLOBAL) == 0 );
    if( obj_exports->used == 0 ) {
        if( back.x86omf->CurrSeg->btype == BASE_GRP ) {
            OutIdx( back, back.x86omf->CurrSeg->base, obj_exports );   /* group index*/
        } else {
#if _TARGET & _TARG_80386
            OutIdx( back, back.x86omf->FlatGIndex, obj_exports );      /* will be 0 if we have none */
#else
            OutIdx( back, 0, obj_exports );
#endif
        }
        OutIdx( back, obj->index, obj_exports );          /* segment index*/
    }
    OutObjectName( back, sym, obj_exports );
    OutOffset( back, (offset)back.x86omf->CurrSeg->location, obj_exports );
    OutIdx( back, 0, obj_exports );                       /* type index*/
}


static void _TellImportHandle( BackRun& back, cg_sym_handle sym, import_handle imphdl, bool alt_dllimp )
/*************************************************************************************/
{
    if( alt_dllimp ) {
        FEBack( *back.front, sym )->imp_alt = imphdl;
    } else {
        TellImportHandle( back, sym, imphdl );
    }
}

static import_handle _AskImportHandle( BackRun& back, cg_sym_handle sym, bool alt_dllimp )
/**********************************************************************/
{
    if( alt_dllimp ) {
        return( FEBack( *back.front, sym )->imp_alt );
    } else {
        return( AskImportHandle( back, sym ) );
    }
}

static void GenImportResolve( BackRun& back, cg_sym_handle sym, omf_idx idx, omf_idx def_idx )
/*****************************************************************************/
{
    array_control       *cmt;
    omf_idx             nidx;
    pointer             cond;
    import_type         type;

    cmt = InitArray( back, sizeof( byte ), MODEST_HDR, INCREMENT_HDR );
    type = (import_type)(pointer_uint)FEAuxInfo( *back.front, sym, FEINF_IMPORT_TYPE );
    switch( type ) {
    case IMPORT_IS_LAZY:
        OutShort( back, LAZY_EXTRN_COMMENT, cmt );
        OutIdx( back, idx, cmt );
        OutIdx( back, def_idx, cmt );
        break;
    case IMPORT_IS_WEAK:
        OutShort( back, WEAK_EXTRN_COMMENT, cmt );
        OutIdx( back, idx, cmt );
        OutIdx( back, def_idx, cmt );
        break;
    case IMPORT_IS_CONDITIONAL_PURE:
        OutShort( back, WEAK_EXTRN_COMMENT, cmt );
        OutIdx( back, idx, cmt );
        OutIdx( back, def_idx, cmt );
        PutObjOMFRec( back, CMD_COMENT, cmt );
        /* fall through */
    case IMPORT_IS_CONDITIONAL:
        OutShort( back, LINKER_COMMENT, cmt );
        if( type == IMPORT_IS_CONDITIONAL ) {
            OutByte( back, LDIR_VF_TABLE_DEF, cmt );
        } else {
            OutByte( back, LDIR_VF_PURE_DEF, cmt );
        }
        OutIdx( back, idx, cmt );
        OutIdx( back, def_idx, cmt );
        for( cond = FEAuxInfo( *back.front, sym, FEINF_CONDITIONAL_IMPORT ); cond != NULL; cond = FEAuxInfo( *back.front, cond, FEINF_NEXT_CONDITIONAL ) ) {
            sym = static_cast<cg_sym_handle>(FEAuxInfo( *back.front, cond, FEINF_CONDITIONAL_SYMBOL ));
            OUTPUT_OBJECT_NAME( sym, GetSymLName, &nidx, NORMAL );
            OutIdx( back, nidx, cmt );
        }
        FlushNames(back);
        break;
    }
    PutObjOMFRec( back, CMD_COMENT, cmt );
    KillArray( back, cmt );
}


static omf_idx  GenImport( BackRun& back, cg_sym_handle sym, bool alt_dllimp )
/*************************************************************/
{
    omf_idx         imp_idx;
    fe_attr         attr;
    import_kind     kind;
    cg_sym_handle   def_resolve;
    omf_idx         def_idx;

    imp_idx = _AskImportHandle( back, sym, alt_dllimp );
    if( imp_idx == NOT_IMPORTED ) {
        attr = FEAttr( *back.front, sym );
        imp_idx = getImportHdl(back);
        CheckImportSwitch( back, (attr & FE_GLOBAL) == 0 );
        _TellImportHandle( back, sym, imp_idx, alt_dllimp );
        kind = NORMAL;
        if( attr & FE_DLLIMPORT ) {
            if( !alt_dllimp ) {
                kind = DLLIMPORT;
            }
        } else if( _IsModel( CGSW_GEN_POSITION_INDEPENDANT ) ) {
            if( attr & FE_THREAD_DATA ) {
                kind = PIC_RW;
            }
        }
        OUTPUT_OBJECT_NAME( sym, OutName, &back.x86omf->Imports, kind );
        OutIdx( back, 0, &back.x86omf->Imports );           /* type index*/
        def_resolve = static_cast<cg_sym_handle>(FEAuxInfo( *back.front, sym, FEINF_DEFAULT_IMPORT_RESOLVE ));
        if( def_resolve != NULL && def_resolve != sym ) {
            def_idx = GenImport( back, def_resolve, false );
            EjectImports(back);
            GenImportResolve( back, sym, imp_idx, def_idx );
        }
    }
    return( imp_idx );
}

static  omf_idx     GenImportComdat( BackRun& back )
/*****************************************/
{
    CheckImportSwitch( back, (FEAttr( *back.front, back.x86omf->CurrSeg->comdat_symbol ) & FE_GLOBAL) == 0 );
    OUTPUT_OBJECT_NAME( back.x86omf->CurrSeg->comdat_symbol, OutName, &back.x86omf->Imports, SPECIAL );
    OutIdx( back, 0, &back.x86omf->Imports );           /* type index*/
    return( getImportHdl(back) );
}

static  void    ComdatData( BackRun& back, label_handle lbl, cg_sym_handle sym )
/************************************************************/
{
    FlushData(back);
    back.x86omf->CurrSeg->obj->lines_generated = false;
    back.x86omf->CurrSeg->location = back.x86omf->CurrSeg->max_written = 0;
    back.x86omf->CurrSeg->comdat_size = 0;
    back.x86omf->CurrSeg->comdat_nidx = 0;
    back.x86omf->CurrSeg->comdat_label = lbl;
    if( sym != NULL ) {
        back.x86omf->CurrSeg->comdat_symbol = sym;
    } else {
        NeedComdatNidx( back, SPECIAL );
        back.x86omf->CurrSeg->comdat_prefix_import = GenImportComdat(back);
        TellCommonLabel( back, lbl, back.x86omf->CurrSeg->comdat_prefix_import );
    }
    back.x86omf->CurrSeg->need_base_set = true;
    back.x86omf->CurrSeg->prefix_comdat_state = PCS_OFF;
    KillLblRedirects(back);
}

static void     OutVirtFuncRef( BackRun& back, cg_sym_handle virt )
/***********************************************/
{
    array_control   *obj_data;
    omf_idx         extdef;

    if( virt == NULL ) {
        extdef = NOT_IMPORTED;
    } else {
        extdef = GenImport( back, virt, false );
    }
    EjectLEData(back);
    obj_data = &back.x86omf->CurrSeg->obj->data;
    OutShort( back, LINKER_COMMENT, obj_data );
    OutByte( back, LDIR_VF_REFERENCE, obj_data );
    OutIdx( back, extdef, obj_data );
    if( back.x86omf->CurrSeg->comdat_symbol != NULL ) {
        OutIdx( back, 0, obj_data );
        OutIdx( back, NeedComdatNidx( back, NORMAL ), obj_data );
    } else {
        OutIdx( back, back.x86omf->CurrSeg->sidx, obj_data );
    }
    PutObjOMFRec( back, CMD_COMENT, obj_data );
}


static void    outDLLExport( BackRun& back, uint words, cg_sym_handle sym )
/***************************************************/
{
    array_control   *obj_data;

    /* unused parameters */ (void)words;

    setUpObj( back, false );
    EjectLEData(back);
    obj_data = &back.x86omf->CurrSeg->obj->data;
    OutShort( back, EXPORT_COMMENT, obj_data );
    OutByte( back, 2, obj_data );
#if _TARGET & _TARG_8086
    OutByte( back, words, obj_data );
#else
    /*
     * this should be 0 for everything except callgates to
     * 16-bit segments (from MS Knowledge Base)
     */
    OutByte( back, 0, obj_data );
#endif
    OutObjectName( back, sym, obj_data );
    OutByte( back, 0, obj_data );
    PutObjOMFRec( back, CMD_COMENT, obj_data );
}


static void    outLabel( BackRun& back, label_handle lbl )
/**********************************/
{
    temp_patch          **owner;
    temp_patch          *tpatch;
    array_control       *lbl_patches;
    unsigned            i;
    pointer             patchptr;
    object              *obj;
    offset              lc;
    cg_sym_handle       sym;
    fe_attr             attr;
    void                *cookie;
    virt_func_ref_list  *curr;
    virt_func_ref_list  *next;

    sym = AskForLblSym( back, lbl );
    if( sym != NULL ) {
        attr = FEAttr( *back.front, sym );
        if( ( attr & FE_PROC ) == 0 ) {
            if( attr & FE_DLLEXPORT ) {
                outDLLExport( back, 0, sym );
            }
        }
        if( attr & FE_COMMON ) {
            ComdatData( back, lbl, sym );
        } else {
            if( back.x86omf->CurrSeg->comdat_label != NULL )
                NormalData(back);
            if( useImportForm( back, attr ) ) {
                OutExport( back, sym );
            }
        }
        for( curr = static_cast<virt_func_ref_list *>(back.x86omf->CurrSeg->virt_func_refs); curr != NULL; curr = next ) {
            next = curr->next;
            for( cookie = curr->cookie; cookie != NULL; cookie = FEAuxInfo( *back.front, cookie, FEINF_VIRT_FUNC_NEXT_REFERENCE ) ) {
                OutVirtFuncRef( back, static_cast<cg_sym_handle>(FEAuxInfo( *back.front, cookie, FEINF_VIRT_FUNC_SYM )) );
            }
            CGFree( back, curr );
        }
        back.x86omf->CurrSeg->virt_func_refs = NULL;
    } else if( back.x86omf->CurrSeg->prefix_comdat_state != PCS_OFF ) {
        /*
            We have data coming out before the COMDAT symbol (select table
            before a common procedure). Gen a 'magical' comdat name to
            deal with it.
        */
        if( back.x86omf->CurrSeg->prefix_comdat_state == PCS_NEED ) {
            ComdatData( back, lbl, NULL );
            back.x86omf->CurrSeg->prefix_comdat_state = PCS_ACTIVE;
        }
        TellCommonLabel( back, lbl, back.x86omf->CurrSeg->comdat_prefix_import );
    }
    lc = (offset)back.x86omf->CurrSeg->location;
    TellAddress( back, lbl, lc );
    for( i = back.x86omf->SegInfo.used; i-- > 0; ) {
        obj = _ARRAYOF( &back.x86omf->SegInfo, index_rec )[i].obj;
        if( obj != NULL ) { /* twas flushed and not redefined*/
            owner = &obj->tpatches;
            for( ;; ) {
                tpatch = *owner;
                if( tpatch == NULL )
                    break;
                if( tpatch->lbl == lbl ) {
                    patchptr = &_ARRAYOF( &obj->data, byte )[tpatch->patch.where];
                    if( tpatch->patch.attr & ADD_PATCH ) {
                        if( tpatch->patch.attr & LONG_PATCH ) {
                            uint_32 val;

                            val = MGET_U32_UN( patchptr ) + lc;
                            MPUT_32_UN( patchptr, val );
                        } else if( tpatch->patch.attr & WORD_PATCH ) {
                            uint_16 val;

                            val = MGET_U16_UN( patchptr ) + lc;
                            MPUT_16_UN( patchptr, val );
                        } else {
                            *(byte *)patchptr += lc;
                        }
                    } else {
                        if( tpatch->patch.attr & LONG_PATCH ) {
                            MPUT_32_UN( patchptr, lc );
                        } else if( tpatch->patch.attr & WORD_PATCH ) {
                            MPUT_16_UN( patchptr, lc );
                        } else {
                            *(byte *)patchptr = lc;
                        }
                    }
                    *owner = tpatch->link;
                    CGFree( back, tpatch );
                } else {
                    owner = &tpatch->link;
                }
            }
        }
    }
    lbl_patches = static_cast<array_control *>(AskLblPatches( back, lbl ));
    for( i = 0; i < lbl_patches->used; i++ ) {
        DoPatch( back, _ARRAYOF( lbl_patches, obj_patch ) + i, lc );
    }
    KillArray( back, lbl_patches );
    TellDonePatches( back, lbl );
}


static void    absPatch( BackRun& back, abspatch_handle patch_handle, offset lc )
/*********************************************************/
{
    abs_patch   *apatch;

    apatch = (abs_patch *)patch_handle;
    if( apatch->flags & AP_HAVE_OFFSET ) {
        DoPatch( back, &apatch->patch, lc );
        FreeAbsPatch( back, apatch );
    } else {
        apatch->value = lc;
        apatch->flags |= AP_HAVE_VALUE;
    }
}


static void    *initPatches( BackRun& back )
/**************************/
{
    return( InitArray( back, sizeof( obj_patch ),  MODEST_PAT, INCREMENT_PAT ) );
}


static omf_fix_loc getOMFFixLoc( BackRun& back, fix_class class_ )
/************************************************/
{
#if _TARGET & _TARG_80386
    if( class_ & F_FAR16 ) {
        /* want a 16:16 fixup for a __far16 call */
        return( LOC_BASE_OFFSET );
    }
#endif
    switch( F_CLASS( class_ ) ) {
    case F_BASE:
        return( LOC_BASE );
#if _TARGET & _TARG_8086
    case F_OFFSET:
        return( LOC_OFFSET );
    case F_BIG_OFFSET:
        return( LOC_OFFSET_32 );
    case F_LDR_OFFSET:
        return( LOC_OFFSET_LOADER );
    case F_PTR:
        return( LOC_BASE_OFFSET );
#else
    case F_OFFSET:
    case F_BIG_OFFSET:
        if( _IsTargetModel( CGSW_X86_EZ_OMF ) ) {
            return( LOC_PHARLAP_OFFSET_32 );
        } else {
            return( LOC_OFFSET_32 );
        }
    case F_LDR_OFFSET:
        if( _IsTargetModel( CGSW_X86_EZ_OMF ) ) {
            return( LOC_PHARLAP_OFFSET_32 );
        } else {
            return( LOC_OFFSET_LOADER_32 );
        }
    case F_PTR:
        if( _IsTargetModel( CGSW_X86_EZ_OMF ) ) {
            return static_cast<omf_fix_loc>(( LOC_PHARLAP_BASE_OFFSET_32 ));
        } else {
            return( LOC_BASE_OFFSET_32 );
        }
#endif
    default:
        break;
    }
    /* error */
    return static_cast<omf_fix_loc>(( 0 ));
}


static void DoFix( BackRun& back, omf_idx idx, bool rel, base_type base, fix_class class_, omf_idx sidx )
/***************************************************************************************/
{
    fixup       *cursor;
    int         where;
    object      *obj;
    index_rec   *rec;
    byte        b;

    b = rel ? LOCAT_REL : LOCAT_ABS;
    if( F_CLASS( class_ ) == F_PTR && back.x86omf->CurrSeg->data_in_code ) {
        back.x86omf->CurrSeg->data_ptr_in_code = true;
    }
    obj = back.x86omf->CurrSeg->obj;
    cursor = static_cast<fixup *>(allocOut( back, &obj->fixes, sizeof( fixup ) ));
    where = back.x86omf->CurrSeg->location - obj->start;
    cursor->locatof = b + ( getOMFFixLoc( back, class_ ) << S_LOCAT_LOC ) + ( where >> 8 );
    cursor->fset = where;
    if( base != BASE_IMP ) {
        rec = AskIndexRec( back, sidx );
        /*
         * fixups to a code segment that is currently in COMDAT mode must be
         * done as imports relative to the comdat symbol.
         */
        if( rec->exec && rec->comdat_symbol != NULL ) {
            idx = GenImport( back, rec->comdat_symbol, false );
            base = BASE_IMP;
        }
    }
#if _TARGET & _TARG_80386
    if( _IsTargetModel( CGSW_X86_FLAT_MODEL )
      && _IsntTargetModel( CGSW_X86_EZ_OMF )
      && ( F_CLASS( class_ ) != F_PTR ) ) {
        omf_idx     grp_idx;

        grp_idx = back.x86omf->FlatGIndex;
        if( base == BASE_GRP ) {
            cursor->fixdat = FIXDAT_FRAME_GROUP + FIXDAT + BASE_SEG;
            OutIdx( back, grp_idx, &obj->fixes );
            OutIdx( back, sidx, &obj->fixes );
        } else {
            cursor->fixdat = FIXDAT_FRAME_GROUP + FIXDAT + base;
            OutIdx( back, grp_idx, &obj->fixes );
            OutIdx( back, idx, &obj->fixes );
        }
    } else
#endif
    if( base == BASE_GRP ) {
        cursor->fixdat = FIXDAT_FRAME_GROUP + FIXDAT + BASE_SEG;
        OutIdx( back, idx, &obj->fixes );
        OutIdx( back, sidx, &obj->fixes );
    } else {
        cursor->fixdat = FIXDAT_FRAME_IMPLIED + FIXDAT + base;
        OutIdx( back, idx, &obj->fixes );
    }
}


static void    setBigLocation( BackRun& back, long_offset loc )
/***************************************/
{
    back.x86omf->CurrSeg->location = loc;
    if( back.x86omf->CurrSeg->comdat_label != NULL ) {
        if( back.x86omf->CurrSeg->comdat_size < loc ) {
            back.x86omf->CurrSeg->comdat_size = loc;
        }
    } else {
        if( back.x86omf->CurrSeg->max_size < loc ) {
            back.x86omf->CurrSeg->max_size = loc;
        }
    }
}

static void    incLocation( BackRun& back, offset by )
/******************************/
{
    long_offset     sum;

    back.x86omf->CurrSeg->obj->pending_line_number = 0;
    sum = back.x86omf->CurrSeg->location + by;
    if( _IsntTargetModel( CGSW_X86_EZ_OMF )
      && (back.x86omf->CurrSeg->attr & SEG_USE_32) == 0 ) {
        sum &= 0xFFFF;
    }
    if( sum < back.x86omf->CurrSeg->location ) { /* if wrapped*/
        if( sum != 0 || back.x86omf->CurrSeg->big == 1 ) {
            FatalError( back, "segment too large" );
        } else if( back.x86omf->CurrSeg->comdat_label == NULL ) {
            back.x86omf->CurrSeg->big = 1;
            if( back.x86omf->CurrSeg->attr & SEG_USE_32 ) {
                back.x86omf->CurrSeg->max_size = (long_offset)(-1);
                back.x86omf->CurrSeg->location = (long_offset)(-1);
            } else {
                back.x86omf->CurrSeg->max_size = (short_offset)(-1);
                back.x86omf->CurrSeg->location =  1L << 8 * sizeof( short_offset );
            }
        }
    } else {
        setBigLocation( back, sum );
    }
}


static void    setLocation( BackRun& back, offset loc )
/*******************************/
{
    back.x86omf->CurrSeg->location = loc;
    if( back.x86omf->CurrSeg->comdat_label != NULL ) {
        if( back.x86omf->CurrSeg->comdat_size < loc ) {
            back.x86omf->CurrSeg->comdat_size = loc;
        }
    } else {
        if( back.x86omf->CurrSeg->max_size < loc ) {
            back.x86omf->CurrSeg->max_size = loc;
        }
    }
}

static  void    DecLocation( BackRun& back, offset by )
/**************************************/
{
    setLocation( back, (offset)back.x86omf->CurrSeg->location - by );
}

static void    outFPPatch( BackRun& back, fp_patches i )
/********************************/
{
    omf_idx     idx;

    idx = back.x86omf->FPPatchImp[i];
    if( idx == NOT_IMPORTED ) {
        idx = getImportHdl(back);
        back.x86omf->FPPatchImp[i] = idx;
        if( back.x86omf->GenStaticImports ) {
            EjectImports(back);
            back.x86omf->GenStaticImports = false;
        }
        OutName( back, FPPatchName[i], &back.x86omf->Imports );
        OutIdx( back, 0, &back.x86omf->Imports );                   /* type index*/
        if( FPPatchAltName[i] != NULL ) {
            getImportHdl(back);
            OutName( back, FPPatchAltName[i], &back.x86omf->Imports );
            OutIdx( back, 0, &back.x86omf->Imports );               /* type index*/
        }
    }
    CheckLEDataSize( back, 2 * sizeof( offset ), true );
    DoFix( back, idx, false, BASE_IMP, F_OFFSET, 0 );
    if( FPPatchAltName[i] != NULL ) {
        incLocation( back, sizeof( byte ) );
        DoFix( back, idx + 1, false, BASE_IMP, F_OFFSET, 0 );
        DecLocation( back, sizeof( byte ) );
    }
}


static void    outPatch( BackRun& back, label_handle lbl, patch_attr attr )
/***************************************************/
{
    temp_patch  *tpatch;

    /* careful, might be patching offset of seg:off */
    CheckLEDataSize( back, 3 * sizeof( offset ), true );
    tpatch = static_cast<temp_patch *>(CGAlloc( back, sizeof( *tpatch ) ));
    tpatch->link = back.x86omf->CurrSeg->obj->tpatches;
    back.x86omf->CurrSeg->obj->tpatches = tpatch;
    tpatch->lbl = lbl;
    tpatch->patch.ref = INVALID_OBJHANDLE;
    tpatch->patch.where = back.x86omf->CurrSeg->location - back.x86omf->CurrSeg->obj->start + back.x86omf->CurrSeg->data_prefix_size;
    tpatch->patch.attr = attr;
}

static abs_patch       *newAbsPatch( BackRun& back )
/**********************************/
{
    abs_patch   *apatch;

    apatch = static_cast<abs_patch *>(CGAlloc( back, sizeof( *apatch ) ));
    memset( apatch, 0, sizeof( *apatch ) );
    apatch->link = back.x86omf->AbsPatches;
    back.x86omf->AbsPatches = apatch;
    return( apatch );
}


static  void    InitLineInfo( BackRun& back, object *obj )
/*****************************************/
{
    obj->lines->used = 0;
    if( back.x86omf->CurrSeg->comdat_label != NULL ) {
        OutByte( back, obj->lines_generated, obj->lines );
        OutIdx( back, NeedComdatNidx( back, NORMAL ), obj->lines );
    } else {
        if( back.x86omf->CurrSeg->btype == BASE_GRP ) {
            OutIdx( back, back.x86omf->CurrSeg->base, obj->lines );
        } else {
            OutIdx( back, 0, obj->lines );
        }
        OutIdx( back, obj->index, obj->lines );
    }
    obj->last_line = 0;
    obj->last_offset = 0;
}


static  void    ChangeObjSrc( BackRun& back, short fno )
/***************************************/
{
    array_control       *names;
    const char          *fname;

    fname = SrcFNoFind( back, fno );
    back.x86omf->CurrFNo = fno;
    names = InitArray( back, sizeof( byte ), MODEST_HDR, INCREMENT_HDR );
    OutName( back, fname, names );
    PutObjOMFRec( back, CMD_THEADR, names );
    KillArray( back, names );
}

static  void    AddLineInfo( BackRun& back, object *obj, cg_linenum line, offset offs )
/**********************************************************************/
{
    cue_state           info;

    if( _IsModel( CGSW_GEN_DBG_DF )
      || _IsModel( CGSW_GEN_DBG_CV ) ) {
        CueFind( back, line, &info );
        if( _IsModel( CGSW_GEN_DBG_DF ) ) {
            if( _IsModel( CGSW_GEN_DBG_LOCALS )
              || _IsModel( CGSW_GEN_DBG_TYPES ) ) {
                 DFLineNum( back, &info, offs );
            }
        } else if( _IsModel( CGSW_GEN_DBG_CV ) ) {
            if( info.fno != back.x86omf->CurrFNo ) {
                FlushLineNum( back, obj );
                InitLineInfo( back, obj );
                ChangeObjSrc( back, info.fno );
            }
        }
        line = info.line;
    }
    if( obj->lines->used >= BUFFSIZE - TOLERANCE ) {
        FlushLineNum( back, obj );
        InitLineInfo( back, obj );
    }
    if( obj->last_line ) {
        OutShort( back, obj->last_line, obj->lines );
        OutOffset( back, obj->last_offset, obj->lines );
    }
    obj->last_line = line;
    obj->last_offset = offs;
}

static  void    SetPendingLine( BackRun& back )
/************************************/
{
    object              *obj;
    cg_linenum          line;
    offset              offs;

    obj = back.x86omf->CurrSeg->obj;
    line = obj->pending_line_number;
    if( line == 0 )
        return;
    offs = back.x86omf->CurrSeg->location;
    obj->pending_line_number = 0;
    obj->pending_label_line = false;
    if( obj->lines == NULL ) {
        obj->lines = InitArray( back, sizeof( byte ), MODEST_LINE, INCREMENT_LINE );
    }
    if( obj->lines->used == 0 ) {
        InitLineInfo( back, obj );
    } else if( obj->last_line ) {
        if( line == obj->last_line )
            return;
        if( offs <= obj->last_offset ) {
            obj->last_line = line;
            return;
        }
    }
    AddLineInfo( back, obj, line, offs );
}


static void    outDataByte( BackRun& back, byte value )
/*******************************/
{
    SetPendingLine(back);
    CheckLEDataSize( back, sizeof( byte ), true );
    *(byte *)allocOutData( back, sizeof( byte ) ) = value;
}

static void    outDataShort( BackRun& back, uint_16 value )
/***********************************/
{
    SetPendingLine(back);
    CheckLEDataSize( back, sizeof( uint_16 ), true );
    value = _TargetShort( value );
    memcpy( allocOutData( back, sizeof( uint_16 ) ), &value, sizeof( uint_16 ) );
}


static void    outDataLong( BackRun& back, uint_32 value )
/**********************************/
{
    SetPendingLine(back);
    CheckLEDataSize( back, sizeof( uint_32 ), true );
    value = _TargetLongInt( value );
    memcpy( allocOutData( back, sizeof( uint_32 ) ), &value, sizeof( uint_32 ) );
}


static void    outAbsPatch( BackRun& back, abs_patch *apatch, patch_attr attr )
/*******************************************************/
{
    long_offset value;

    CheckLEDataSize( back, 2 * sizeof( offset ), true );
    if( apatch->flags & AP_HAVE_VALUE ) {
        value = apatch->value;
        FreeAbsPatch( back, apatch );
    } else {
        apatch->obj = back.x86omf->CurrSeg->obj;
        apatch->patch.ref = INVALID_OBJHANDLE;
        apatch->patch.where = back.x86omf->CurrSeg->location - back.x86omf->CurrSeg->obj->start + back.x86omf->CurrSeg->data_prefix_size;
        apatch->patch.attr = attr;
        value = 0;
    }
    if( attr & LONG_PATCH ) {
        outDataLong( back, value );
    } else if( attr & WORD_PATCH ) {
        outDataShort( back, value );
    } else {
        outDataByte( back, (byte)value );
    }
}


static void    outReloc( BackRun& back, segment_id segid, fix_class class_, bool rel )
/*************************************************************/
{
    index_rec   *rec;

    rec = AskSegIndex( back, segid );
    if( F_CLASS( class_ ) == F_BIG_OFFSET ) {
        CheckLEDataSize( back, 3 * sizeof( long_offset ), true );
    } else {
        CheckLEDataSize( back, 3 * sizeof( offset ), true );
    }
    DoFix( back, rec->base, rel, rec->btype, class_, rec->sidx );
}


static void outSpecialCommon( BackRun& back, import_handle imphdl, fix_class class_, bool rel )
/**********************************************************************/
{
    CheckLEDataSize( back, 3 * sizeof( offset ), true );
    DoFix( back, imphdl, rel, BASE_IMP, class_, 0 );
}


static void    outImport( BackRun& back, cg_sym_handle sym, fix_class class_, bool rel )
/***************************************************************/
{
    fe_attr     attr;

    attr = FEAttr( *back.front, sym );
#if  _TARGET & _TARG_80386
    if( !rel && F_CLASS( class_ ) == F_OFFSET && (attr & FE_PROC) ) {
        if( (call_class_target)(pointer_uint)FindAuxInfoSym( sym, FEINF_CALL_CLASS_TARGET ) & FECALL_X86_FAR16_CALL ) {
            class_ |= F_FAR16;
        }
    }
#endif
    if( attr & FE_THREAD_DATA )
        class_ |= F_TLS;
    outSpecialCommon( back, GenImport( back, sym, (class_ & F_ALT_DLLIMP) != 0 ), class_, rel );
}


static void    outRTImportRel( BackRun& back, rt_class rtindex, fix_class class_, bool rel )
/*******************************************************************/
{
    import_handle   imphdl;

    imphdl = AskRTHandle( back, rtindex );
    if( imphdl == NOT_IMPORTED ) {
        imphdl = getImportHdl(back);
        CheckImportSwitch( back, false );
        TellRTHandle( back, rtindex, imphdl );
        OutName( back, AskRTName( back, rtindex ), &back.x86omf->Imports );
        OutIdx( back, 0, &back.x86omf->Imports );           /* type index*/
    }
    outSpecialCommon( back, imphdl, class_, rel );
}


static void    outRTImport( BackRun& back, rt_class rtindex, fix_class class_ )
/******************************************************/
{
    outRTImportRel( back, rtindex, class_, ( F_CLASS( class_ ) == F_OFFSET || F_CLASS( class_ ) == F_LDR_OFFSET ) );
}

static void    outBckExport( BackRun& back, const char *name, bool is_export )
/******************************************************/
{
    array_control       *obj_exports;
    object              *obj;

    obj = back.x86omf->CurrSeg->obj;
    obj_exports = &obj->exports;
    if( obj_exports->used >= BUFFSIZE - TOLERANCE ) {
        EjectExports(back);
    }
    CheckExportSwitch( back, is_export );
    if( obj_exports->used == 0 ) {
        if( back.x86omf->CurrSeg->btype == BASE_GRP ) {
            OutIdx( back, back.x86omf->CurrSeg->base, obj_exports );   /* group index*/
        } else {
#if _TARGET & _TARG_80386
            OutIdx( back, back.x86omf->FlatGIndex, obj_exports );      /* will be 0 if we have none */
#else
            OutIdx( back, 0, obj_exports );
#endif
        }
        OutIdx( back, obj->index, obj_exports );          /* segment index*/
    }
    OutName( back, name, obj_exports );
    OutOffset( back, (offset)back.x86omf->CurrSeg->location, obj_exports );
    OutIdx( back, 0, obj_exports );                       /* type index*/
}

static void    outBckImport( BackRun& back, const char *name, back_handle bck, fix_class class_ )
/************************************************************************/
{
    omf_idx     idx;

    idx = bck->imp;
    if( idx == NOT_IMPORTED ) {
        idx = getImportHdl(back);
        CheckImportSwitch( back, false );
        bck->imp = idx;
        OutName( back, name, &back.x86omf->Imports );
        OutIdx( back, 0, &back.x86omf->Imports );               /* type index*/
    }
    outSpecialCommon( back, idx, class_, false );
}


static void    outLineNum( BackRun& back, cg_linenum  line, bool label_line )
/*****************************************************/
{
    object      *obj;

    obj = back.x86omf->CurrSeg->obj;
    if( obj->pending_line_number == 0
        || obj->pending_label_line
        || obj->pending_line_number > line ) {
        obj->pending_line_number = line;
        obj->pending_label_line = label_line;
    }
}


static unsigned        savePendingLine( BackRun& back, unsigned new_line )
/***************************************************
 * We're about to dump some alignment bytes. Save and restore
 * the pending_line_number field so the that line number info
 * offset is after the alignment.
 */
{
    unsigned    old;

    old = back.x86omf->CurrSeg->obj->pending_line_number;
    back.x86omf->CurrSeg->obj->pending_line_number = new_line;
    return( old );
}


static void    outDBytes( BackRun& back, unsigned len, const byte *src )
/************************************************/
{
    unsigned    max;
    unsigned    n;

    SetPendingLine(back);
    CheckLEDataSize( back, sizeof( byte ), true );
    max = (BUFFSIZE - TOLERANCE) - ( back.x86omf->CurrSeg->location - back.x86omf->CurrSeg->obj->start + back.x86omf->CurrSeg->data_prefix_size );
    while( len != 0 ) {
        if( len > max ) {
            n = max;
        } else {
            n = len;
        }
        memcpy( allocOutData( back, n ), src, n );
        src += n;
        len -= n;
        if( len == 0 )
            break;
        EjectLEData(back);
        OutLEDataStart( back, false );
        max = (BUFFSIZE - TOLERANCE);
    }
}


static void    outIBytes( BackRun& back, byte pattern, offset len )
/*******************************************/
{
    omf_cmd         cmd;
    array_control   *obj_data;

    SetPendingLine(back);
    if( len <= TRADEOFF ) {
        for( ; len != 0; --len ) {
            outDataByte( back, pattern );
        }
    } else {
        EjectLEData(back);
        OutLEDataStart( back, true );
        obj_data = &back.x86omf->CurrSeg->obj->data;
#if _TARGET & _TARG_80386
        if( _IsntTargetModel( CGSW_X86_EZ_OMF ) ) {
            OutOffset( back, len, obj_data );         /* repeat count */
        } else {
            OutShort( back, len, obj_data );          /* repeat count */
        }
#else
        OutShort( back, len, obj_data );              /* repeat count */
#endif
        OutShort( back, 0, obj_data );                /* nesting count */
        OutByte( back, 1, obj_data );                 /* pattern length */
        OutByte( back, pattern, obj_data );
        if( back.x86omf->CurrSeg->comdat_label != NULL ) {
#if _TARGET & _TARG_80386
            if( _IsntTargetModel( CGSW_X86_EZ_OMF ) ) {
                cmd = CMD_COMDAT32;
            } else {
                cmd = CMD_COMDAT;
            }
#else
            cmd = CMD_COMDAT;
#endif
        } else {
#if _TARGET & _TARG_80386
            if( _IsntTargetModel( CGSW_X86_EZ_OMF ) ) {
                cmd = CMD_LIDATA32;
            } else {
                cmd = CMD_LIDATA;
            }
#else
            cmd = CMD_LIDATA;
#endif
        }
        PutObjOMFRec( back, cmd, obj_data );
        incLocation( back, len );
        SetMaxWritten(back);
    }
}


static bool    needBaseSet( BackRun& back )
/*************************/
{
    bool        need;

    need = ( back.x86omf->CurrSeg->need_base_set != 0 );
    back.x86omf->CurrSeg->need_base_set = false;
    return( need );
}


static offset  askMaxSize( BackRun& back )
/************************/
{
    return( (offset)back.x86omf->CurrSeg->max_size );
}

static long_offset  askBigLocation( BackRun& back )
/*********************************/
{
    return( back.x86omf->CurrSeg->location );
}

static long_offset  askBigMaxSize( BackRun& back )
/********************************/
{
    return( back.x86omf->CurrSeg->max_size );
}

static void    tellObjNewLabel( BackRun& back, cg_sym_handle lbl )
/******************************************/
{
    if( lbl == NULL )
        return;
    if( back.x86omf->CurrSeg == NULL )
        return;
    if( back.x86omf->codeSegId != back.x86omf->CurrSeg->segid )
        return;

    /*
       We've got a data label going into a code segment.
       Make sure everything's OK with regards to COMDATs.
    */
    if( FEAttr( *back.front, lbl ) & FE_COMMON ) {
        doEmptyQueue(back);
        if( _IsModel( CGSW_GEN_DBG_DF ) ) {
            if( back.x86omf->CurrSeg->comdat_symbol != NULL ) {
                DFSymRange( back, back.x86omf->CurrSeg->comdat_symbol, (offset)back.x86omf->CurrSeg->comdat_size );
            }
        }
    } else if( back.x86omf->CurrSeg->comdat_symbol != NULL ) {
        doEmptyQueue(back);
        setUpObj( back, false );
        if( _IsModel( CGSW_GEN_DBG_DF ) ) {
            if( back.x86omf->CurrSeg->comdat_symbol != NULL ) {
                DFSymRange( back, back.x86omf->CurrSeg->comdat_symbol, (offset)back.x86omf->CurrSeg->comdat_size );
            }
        }
    }
}

static void    tellObjNewProc( BackRun& back, cg_sym_handle proc )
/******************************************/
{
    segment_id  proc_segid;

    PUSH_OP( back.x86omf->codeSegId );
        proc_segid = FESegID( *back.front, proc );
        if( back.x86omf->codeSegId != proc_segid ) {
            if( _IsModel( CGSW_GEN_DBG_DF ) ) {
                if( back.x86omf->CurrSeg->comdat_symbol != NULL ) {
                    DFSymRange( back, back.x86omf->CurrSeg->comdat_symbol, (offset)back.x86omf->CurrSeg->comdat_size );
                }
            }
            if( back.x86omf->CurrSeg->obj != NULL ) {
                doEmptyQueue(back);
                FlushObject(back);
            }
            back.x86omf->codeSegId = proc_segid;
            setOP( back, back.x86omf->codeSegId );
            back.x86omf->CurrSeg->need_base_set = true;
            if( !back.x86omf->CurrSeg->exec ) {
                Zoiks( back, ZOIKS_088 );
            }
        }
        if( FEAttr( *back.front, proc ) & FE_COMMON ) {
            doEmptyQueue(back);
            if( _IsModel( CGSW_GEN_DBG_DF ) ) {
                if( back.x86omf->CurrSeg->comdat_symbol != NULL ) {
                    DFSymRange( back, back.x86omf->CurrSeg->comdat_symbol, (offset)back.x86omf->CurrSeg->comdat_size );
                }
            }
            back.x86omf->CurrSeg->comdat_symbol = proc;
            back.x86omf->CurrSeg->prefix_comdat_state = PCS_NEED;
        } else if( back.x86omf->CurrSeg->comdat_symbol != NULL ) {
            doEmptyQueue(back);
            setUpObj( back, false );
            if( _IsModel( CGSW_GEN_DBG_DF ) ) {
                if( back.x86omf->CurrSeg->comdat_symbol != NULL ) {
                    DFSymRange( back, back.x86omf->CurrSeg->comdat_symbol, (offset)back.x86omf->CurrSeg->comdat_size );
                }
            }
            NormalData(back);
        }
    POP_OP();
}

static void     tellObjVirtFuncRef( BackRun& back, void *cookie )
/*****************************************/
{
    virt_func_ref_list  *new_vf;

    PUSH_OP( back.x86omf->codeSegId );
        new_vf = static_cast<virt_func_ref_list *>(CGAlloc( back, sizeof( virt_func_ref_list ) ));
        new_vf->cookie = cookie;
        new_vf->next = static_cast<struct virt_func_ref_list *>(back.x86omf->CurrSeg->virt_func_refs);
        back.x86omf->CurrSeg->virt_func_refs = new_vf;
    POP_OP();
}

static bool     InlineFunction( BackRun& back, cg_sym_handle sym )
/*************************************************/
{
    if( FEAttr( *back.front, sym ) & FE_PROC ) {
        if( FindAuxInfoSym( sym, FEINF_CALL_BYTES ) != NULL
          || ((call_class)(pointer_uint)FindAuxInfoSym( sym, FEINF_CALL_CLASS ) & FECALL_GEN_MAKE_CALL_INLINE) ) {
            return( true );
        }
    }
    return( false );
}

static segment_id      askSegID( BackRun& back, pointer hdl, cg_class class_ )
/*****************************************************/
{
    switch( class_ ) {
    case CG_FE:
        if( InlineFunction( back, (cg_sym_handle)hdl ) ) {
            return( back.x86omf->codeSegId );    /* askCodeSeg() */
        }
        return( FESegID( *back.front, (cg_sym_handle)hdl ) );
    case CG_BACK:
        return( ((back_handle)hdl)->segid );
    case CG_TBL:
    case CG_VTB:
        return( back.x86omf->codeSegId );        /* askCodeSeg() */
    case CG_CLB:
        return( altCodeSegId );     /* askAltCodeSeg() */
    default:
        return( back.x86omf->dataSegId );        /* askBackSeg() */
    }
}

static bool    askNameIsCode( BackRun& back, pointer hdl, cg_class class_ )
/**************************************************/
{
    switch( class_ ) {
    case CG_FE:
        return( (FEAttr( *back.front, static_cast<cg_sym_handle>(hdl) ) & FE_PROC) != 0 );
    case CG_TBL:
    case CG_VTB:
    case CG_CLB:
        return( true );
    default:
        break;
    }
    return( false );
}

static bool    askNameIsROM( BackRun& back, pointer hdl, cg_class class_ )
/*************************************************/
{
    return( askSegIsROM( back, askSegID( back, hdl, class_ ) ) );
}

static bool symIsExported( BackRun& back, cg_sym_handle sym )
/*************************************/
{
    bool        exported;

    exported = false;
    if( sym != NULL ) {
        if( FEAttr( *back.front, sym ) & FE_DLLEXPORT ) {
            exported = true;
        } else if( (call_class)(pointer_uint)FindAuxInfoSym( sym, FEINF_CALL_CLASS ) & FECALL_GEN_DLL_EXPORT ) {
            exported = true;
        }
    }
    return( exported );
}


const x86_obj_emitter X86OmfEmitter = {
    initSegDefs,
    freeObjCache,
    askOP,
    changeOP,
    setOP,
    defSegment,
    doEmptyQueue,
    getMemModel,
    dbgSegDef,
    objInit,
    askLocation,
    chkDbgSegSize,
    useImportForm,
    askSegIsNear,
    askSegIsBlank,
    askSegIsPrivate,
    askSegIsROM,
    askBackSeg,
    askCodeSeg,
    haveCodeSeg,
    askAltCodeSeg,
    askCode16Seg,
    outSelect,
    setUpObj,
    flushOP,
    objFini,
    outDLLExport,
    outLabel,
    absPatch,
    initPatches,
    setBigLocation,
    incLocation,
    setLocation,
    outFPPatch,
    outPatch,
    newAbsPatch,
    outDataByte,
    outDataShort,
    outDataLong,
    outAbsPatch,
    outReloc,
    outSpecialCommon,
    outImport,
    outRTImportRel,
    outRTImport,
    outBckExport,
    outBckImport,
    outLineNum,
    savePendingLine,
    outDBytes,
    outIBytes,
    needBaseSet,
    askMaxSize,
    askBigLocation,
    askBigMaxSize,
    tellObjNewLabel,
    tellObjNewProc,
    tellObjVirtFuncRef,
    askSegID,
    askNameIsCode,
    askNameIsROM,
    symIsExported
};

/* the run makes and frees its copy of this file's variables (run/ files: see the run header) */
X86omfState* x86omf_make(BackRun& back) { X86omfState* part = new X86omfState(); x86omf_start(back, *part); return part; }
void x86omf_free(X86omfState* part) { delete part; }
