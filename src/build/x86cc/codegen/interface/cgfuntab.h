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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


CGAPIDEF( BEMemInit,    void,           (BackRun& back, pointer) )
CGAPIDEF( BEInitCg,     cg_init_info,   (BackRun& back, cg_switches,cg_target_switches,uint,proc_revision) )
CGAPIDEF( BEInit,       cg_init_info,   (BackRun& back, cg_switches,cg_target_switches,uint,proc_revision) )
CGAPIDEF( BEStart,      void,           (BackRun& back) )
CGAPIDEF( BEStop,       void,           (BackRun& back) )
CGAPIDEF( BEAbort,      void,           (BackRun& back) )
CGAPIDEF( BEFini,       void,           (BackRun& back) )
CGAPIDEF( BEFiniCg,     void,           (BackRun& back) )
CGAPIDEF( BEMemAlloc,   pointer,        (BackRun& back, uint) )
CGAPIDEF( BEMemFree,    void,           (BackRun& back, pointer) )
CGAPIDEF( BEMemFini,    void,           (BackRun& back) )
CGAPIDEF( BEGetSeg,     segment_id,     (BackRun& back) )
CGAPIDEF( BESetSeg,     segment_id,     (BackRun& back, segment_id) )
CGAPIDEF( BEFlushSeg,   void,           (BackRun& back, segment_id) )
CGAPIDEF( BEDefSeg,     void,           (BackRun& back, segment_id,seg_attr,cchar_ptr,uint) )
CGAPIDEF( BENewBack,    back_handle,    (BackRun& back, cg_sym_handle) )
CGAPIDEF( BEFiniBack,   void,           (BackRun& back, back_handle) )
CGAPIDEF( BEFreeBack,   void,           (BackRun& back, back_handle) )
CGAPIDEF( BEDefType,    void,           (BackRun& back, cg_type,uint,unsigned_32) )
CGAPIDEF( BEAliasType,  void,           (BackRun& back, cg_type,cg_type) )
CGAPIDEF( BETypeLength, unsigned_32,    (BackRun& back, cg_type) )
CGAPIDEF( BETypeAlign,  uint,           (cg_type) )
CGAPIDEF( BEUnrollCount,unsigned_32,    (BackRun& back, unsigned_32) )
CGAPIDEF( BENewLabel,   label_handle,   (BackRun& back) )
CGAPIDEF( BEFiniLabel,  void,           (BackRun& back, label_handle) )
CGAPIDEF( BEPatch,      patch_handle,   (BackRun& back) )
CGAPIDEF( BEPatchInteger,void,          (BackRun& back, patch_handle,signed_32) )
CGAPIDEF( BEFiniPatch,  void,           (BackRun& back, patch_handle) )

CGAPIDEF( CGBitMask,    cg_name,        (BackRun& back, cg_name,byte,byte,cg_type) )
CGAPIDEF( CGChoose,     cg_name,        (BackRun& back, cg_name,cg_name,cg_name,cg_type) )
CGAPIDEF( CGIndex,      cg_name,        (BackRun& back, cg_name,cg_name,cg_type,cg_type) )
CGAPIDEF( CGWarp,       cg_name,        (BackRun& back, cg_name,label_handle,cg_name) )
CGAPIDEF( CG3WayControl,void,           (BackRun& back, cg_name,label_handle,label_handle,label_handle) )
CGAPIDEF( CGFlow,       cg_name,        (BackRun& back, cg_op,cg_name,cg_name) )
CGAPIDEF( CGPostGets,   cg_name,        (BackRun& back, cg_op,cg_name,cg_name,cg_type) )
CGAPIDEF( CGPreGets,    cg_name,        (BackRun& back, cg_op,cg_name,cg_name,cg_type) )
CGAPIDEF( CGLVPreGets,  cg_name,        (BackRun& back, cg_op,cg_name,cg_name,cg_type) )
CGAPIDEF( CGSelectRestricted,void,      (BackRun& back, sel_handle,cg_name,cg_switch_type) )
CGAPIDEF( CGUnary,      cg_name,        (BackRun& back, cg_op,cg_name,cg_type) )
CGAPIDEF( CGBinary,     cg_name,        (BackRun& back, cg_op,cg_name,cg_name,cg_type) )
CGAPIDEF( CGAssign,     cg_name,        (BackRun& back, cg_name,cg_name,cg_type) )
CGAPIDEF( CGLVAssign,   cg_name,        (BackRun& back, cg_name,cg_name,cg_type) )
CGAPIDEF( CGProcDecl,   void,           (BackRun& back, cg_sym_handle,cg_type) )
CGAPIDEF( CGParmDecl,   void,           (BackRun& back, cg_sym_handle,cg_type) )
CGAPIDEF( CGLastParm,   label_handle,   (BackRun& back) )
CGAPIDEF( CGInitCall,   call_handle,    (BackRun& back, cg_name,cg_type,cg_sym_handle) )
CGAPIDEF( CGAddParm,    void,           (BackRun& back, call_handle,cg_name,cg_type) )
CGAPIDEF( CGCall,       cg_name,        (call_handle) )
CGAPIDEF( CGReturn,     void,           (BackRun& back, cg_name,cg_type) )
CGAPIDEF( CGCompare,    cg_name,        (BackRun& back, cg_op,cg_name,cg_name,cg_type) )
CGAPIDEF( CGControl,    void,           (BackRun& back, cg_op,cg_name,label_handle) )
CGAPIDEF( CGDone,       void,           (BackRun& back, cg_name) )
CGAPIDEF( CGTrash,      void,           (BackRun& back, cg_name) )
CGAPIDEF( CGAttr,       cg_name,        (cg_name,cg_sym_attr) )
CGAPIDEF( CGVolatile,   cg_name,        (cg_name) )
CGAPIDEF( CGBigLabel,   void,           (BackRun& back, back_handle) )
CGAPIDEF( CGBigGoto,    void,           (BackRun& back, label_handle,int) )
CGAPIDEF( CGDuplicate,  cg_name *,      (BackRun& back, cg_name) )
CGAPIDEF( CGType,       cg_type,        (cg_name) )
CGAPIDEF( CGEval,       cg_name,        (BackRun& back, cg_name) )
CGAPIDEF( CGVarargsBasePtr,cg_name,     (BackRun& back, cg_type) )

CGAPIDEF( CGCallback,   cg_name,        (BackRun& back, cg_callback,callback_handle) )
CGAPIDEF( CGPatchNode,  cg_name,        (BackRun& back, patch_handle,cg_type) )

CGAPIDEF( CGFloat,      cg_name,        (BackRun& back, cchar_ptr,cg_type) )
CGAPIDEF( CGInteger,    cg_name,        (BackRun& back, signed_32,cg_type) )
CGAPIDEF( CGInt64,      cg_name,        (BackRun& back, signed_64,cg_type) )

CGAPIDEF( CGBackName,   cg_name,        (BackRun& back, back_handle,cg_type) )
CGAPIDEF( CGFEName,     cg_name,        (BackRun& back, cg_sym_handle,cg_type) )
CGAPIDEF( CGTempName,   cg_name,        (BackRun& back, temp_handle,cg_type) )
CGAPIDEF( CGTemp,       temp_handle,    (BackRun& back, cg_type) )
CGAPIDEF( CGAutoDecl,   void,           (BackRun& back, cg_sym_handle,cg_type) )

CGAPIDEF( CGSelInit,    sel_handle,     (BackRun& back) )
CGAPIDEF( CGSelCase,    void,           (BackRun& back, sel_handle,label_handle,signed_64) )
CGAPIDEF( CGSelRange,   void,           (BackRun& back, sel_handle,signed_64,signed_64,label_handle) )
CGAPIDEF( CGSelOther,   void,           (sel_handle,label_handle) )
CGAPIDEF( CGSelect,     void,           (BackRun& back, sel_handle,cg_name) )

CGAPIDEF( DGLabel,      void,           (BackRun& back, back_handle) )
CGAPIDEF( DGTell,       unsigned_32,    (BackRun& back) )
CGAPIDEF( DGBackTell,   unsigned_32,    (BackRun& back, back_handle) )
CGAPIDEF( DGBackPtr,    void,           (BackRun& back, back_handle,segment_id,signed_32,cg_type) )
CGAPIDEF( DGFEPtr,      void,           (BackRun& back, cg_sym_handle,cg_type,signed_32) )
CGAPIDEF( DGInteger,    void,           (BackRun& back, unsigned_32,cg_type) )
CGAPIDEF( DGInteger64,  void,           (BackRun& back, unsigned_64,cg_type) )
CGAPIDEF( DGFloat,      void,           (BackRun& back, cchar_ptr,cg_type) )
CGAPIDEF( DGChar,       void,           (BackRun& back, char) )
CGAPIDEF( DGString,     void,           (BackRun& back, cchar_ptr,uint) )
CGAPIDEF( DGBytes,      void,           (BackRun& back, unsigned_32,const void *) )
CGAPIDEF( DGIBytes,     void,           (BackRun& back, unsigned_32,byte) )
CGAPIDEF( DGUBytes,     void,           (BackRun& back, unsigned_32) )
CGAPIDEF( DGAlign,      void,           (BackRun& back, uint) )
CGAPIDEF( DGSeek,       unsigned_32,    (BackRun& back, unsigned_32) )
CGAPIDEF( DGCFloat,     void,           (BackRun& back, pointer,cg_type) )

CGAPIDEF( DBLineNum,    void,           (BackRun& back, uint) )
CGAPIDEF( DBModSym,     void,           (BackRun& back, cg_sym_handle,cg_type) )
CGAPIDEF( DBLocalSym,   void,           (BackRun& back, cg_sym_handle,cg_type) )
CGAPIDEF( DBTypeDef,    void,           (BackRun& back, cchar_ptr,dbg_type) )
CGAPIDEF( DBLocalType,  void,           (BackRun& back, cg_sym_handle,bool) )
CGAPIDEF( DBScalar,     dbg_type,       (BackRun& back, cchar_ptr,cg_type) )
CGAPIDEF( DBFtnType,    dbg_type,       (BackRun& back, cchar_ptr,dbg_ftn_type) )
CGAPIDEF( DBDereference,dbg_type,       (BackRun& back, cg_type,dbg_type) )
CGAPIDEF( DBIndCharBlock,dbg_type,      (BackRun& back, back_handle,cg_type,int) )
CGAPIDEF( DBLocCharBlock,dbg_type,      (BackRun& back, dbg_loc,cg_type) )
CGAPIDEF( DBCharBlock,  dbg_type,       (BackRun& back, unsigned_32) )
CGAPIDEF( DBCharBlockNamed, dbg_type,   (BackRun& back, cchar_ptr,unsigned_32) )
CGAPIDEF( DBScope,      dbg_type,       (BackRun& back, cchar_ptr) )
CGAPIDEF( DBNested,     bool,           (BackRun& back, bool) )
CGAPIDEF( DBBegName,    dbg_name,       (BackRun& back, cchar_ptr,dbg_type) )
CGAPIDEF( DBForward,    dbg_type,       (BackRun& back, dbg_name) )
CGAPIDEF( DBEndName,    dbg_type,       (BackRun& back, dbg_name,dbg_type) )
CGAPIDEF( DBArray,      dbg_type,       (BackRun& back, dbg_type,dbg_type) )
CGAPIDEF( DBBegArray,   dbg_array,      (BackRun& back, dbg_type,cg_type, bool) )
CGAPIDEF( DBDimCon,     void,           (BackRun& back, dbg_array,dbg_type,signed_32,signed_32) )
CGAPIDEF( DBDimVar,     void,           (BackRun& back, dbg_array,back_handle,int,cg_type,cg_type) )
CGAPIDEF( DBEndArray,   dbg_type,       (BackRun& back, dbg_array) )
CGAPIDEF( DBFtnArray,   dbg_type,       (BackRun& back, back_handle,cg_type,cg_type,int,dbg_type) )
CGAPIDEF( DBIntArray,   dbg_type,       (BackRun& back, unsigned_32,dbg_type) )
CGAPIDEF( DBIntArrayCG, dbg_type,       (BackRun& back, cg_type,unsigned_32,dbg_type) )
CGAPIDEF( DBSubRange,   dbg_type,       (BackRun& back, signed_32,signed_32,dbg_type) )
CGAPIDEF( DBPtr,        dbg_type,       (BackRun& back, cg_type,dbg_type) )
CGAPIDEF( DBBegStruct,  dbg_struct,     (BackRun& back, cg_type,bool) )
CGAPIDEF( DBBegNameStruct,dbg_struct,   (BackRun& back, cchar_ptr,cg_type,bool) )
CGAPIDEF( DBStructForward,dbg_type,     (dbg_struct) )
CGAPIDEF( DBAddField,   void,           (BackRun& back, dbg_struct,unsigned_32,cchar_ptr,dbg_type) )
CGAPIDEF( DBAddBitField,void,           (BackRun& back, dbg_struct,unsigned_32,byte,byte,cchar_ptr,dbg_type) )
CGAPIDEF( DBAddStField, void,           (BackRun& back, dbg_struct,dbg_loc,cchar_ptr,unsigned_32,dbg_type) )
CGAPIDEF( DBAddMethod,  void,           (BackRun& back, dbg_struct,dbg_loc,uint,uint,cchar_ptr,dbg_type) )
CGAPIDEF( DBAddNestedType,void,         (BackRun& back, dbg_struct,cchar_ptr,dbg_type) )
CGAPIDEF( DBEndStruct,  dbg_type,       (BackRun& back, dbg_struct) )
CGAPIDEF( DBBegEnum,    dbg_enum,       (BackRun& back, cg_type) )
CGAPIDEF( DBAddConst,   void,           (BackRun& back, dbg_enum,cchar_ptr,signed_32) )
CGAPIDEF( DBAddConst64, void,           (BackRun& back, dbg_enum,cchar_ptr,signed_64) )
CGAPIDEF( DBEndEnum,    dbg_type,       (BackRun& back, dbg_enum) )
CGAPIDEF( DBBegProc,    dbg_proc,       (BackRun& back, cg_type,dbg_type) )
CGAPIDEF( DBAddMethParms,void,          (dbg_proc,dbg_type,dbg_type) )
CGAPIDEF( DBAddParm,    void,           (BackRun& back, dbg_proc,dbg_type) )
CGAPIDEF( DBEndProc,    dbg_type,       (BackRun& back, dbg_proc) )
CGAPIDEF( DBGenSym,     void,           (BackRun& back, cg_sym_handle,dbg_loc,int) )
CGAPIDEF( DBGenStMem,   void,           (BackRun& back, cg_sym_handle,dbg_loc) )
CGAPIDEF( DBBasedPtr,   dbg_type,       (BackRun& back, cg_type,dbg_type,dbg_loc) )
CGAPIDEF( DBAddLocField,void,           (BackRun& back, dbg_struct,dbg_loc,uint,byte,byte,cchar_ptr,dbg_type) )
CGAPIDEF( DBAddInheritance,void,        (BackRun& back, dbg_struct,dbg_type,uint,uint,dbg_loc) )
CGAPIDEF( DBAddBaseInfo,void,           (dbg_struct,unsigned_32,int,dbg_type,cg_type) )
CGAPIDEF( DBAddVFuncInfo,void,          (BackRun& back, dbg_struct,unsigned_32,int,cg_type) )
CGAPIDEF( DBLocInit,    dbg_loc,        (void) )
CGAPIDEF( DBLocSym,     dbg_loc,        (BackRun& back, dbg_loc,cg_sym_handle) )
CGAPIDEF( DBLocTemp,    dbg_loc,        (BackRun& back, dbg_loc,temp_handle) )
CGAPIDEF( DBLocConst,   dbg_loc,        (BackRun& back, dbg_loc,unsigned_32) )
CGAPIDEF( DBLocOp,      dbg_loc,        (BackRun& back, dbg_loc,dbg_loc_op,unsigned) )
CGAPIDEF( DBLocFini,    void,           (BackRun& back, dbg_loc) )
CGAPIDEF( DBObject,     void,           (BackRun& back, dbg_type,dbg_loc,cg_type) )
CGAPIDEF( DBSrcFile,    uint,           (BackRun& back, cchar_ptr) )
CGAPIDEF( DBSrcCue,     void,           (BackRun& back, uint,uint,uint) )
CGAPIDEF( DBBegBlock,   void,           (BackRun& back) )
CGAPIDEF( DBEndBlock,   void,           (BackRun& back) )
CGAPIDEF( DFClient,     pointer,        (BackRun& back) )
CGAPIDEF( DFDwarfLocal, void,           (BackRun& back, pointer,pointer,pointer) )
CGAPIDEF( DGFEPtrBase,  void,           (BackRun& back, cg_sym_handle) )
