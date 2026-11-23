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
* Description:  Calling convention structures and setup for front ends.
*
****************************************************************************/


#include <string.h>
#include <stdio.h>
#include "callinfo.h"
#include "frontrun.hpp"

    #define AUX_STRALLOC        CMemStrdup
    #define DEFAULT_CALLINFO    WatcallInfo





namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct CallinfoFileState: callinfo_state.h) */

}  // namespace




void AuxInfoInit( FrontRun& front, int flag_stdatnum )
/***********************************/
{
    hw_reg_set          full_no_segs;
    call_class          cclass;
    call_class_target   cclass_target;

  #if _CPU == 8086
    flag_stdatnum = 0;
  #endif

    HW_CAsgn( full_no_segs, HW_FULL );
    HW_CTurnOff( full_no_segs, HW_SEGS );

    /*
     * get FECALL_X86_FAR_CALL attribute only
     */
    cclass          = FECALL_GEN_NONE;
    cclass_target   = WatcallInfo.cclass_target & FECALL_X86_FAR_CALL;

/*************************************************
 *  __fortran calling convention
 *************************************************/
    FortranInfo.objname = AUX_STRALLOC( front, "^" );

/*************************************************
 *  __cdecl calling convention
 *************************************************/
    CdeclInfo.cclass =    cclass |
                         //FECALL_GEN_REVERSE_PARMS |
                         FECALL_GEN_CALLER_POPS |
                         FECALL_GEN_NONE;
    CdeclInfo.cclass_target = cclass_target |
                         //FECALL_X86_PARMS_PREFER_REGS |
                         //FECALL_X86_GENERATE_STACK_FRAME |
  #if _CPU == 8086
                         FECALL_X86_LOAD_DS_ON_CALL |
                         FECALL_X86_NO_FLOAT_REG_RETURNS |
  #else /* _CPU == 386 */
                         //FECALL_X86_LOAD_DS_ON_CALL |
                         //FECALL_X86_NO_FLOAT_REG_RETURNS |
  #endif
                         FECALL_X86_NO_STRUCT_REG_RETURNS |
                         FECALL_X86_ROUTINE_RETURN |
                         //FECALL_X86_NO_8087_RETURNS |
                         //FECALL_X86_SPECIAL_RETURN |
                         FECALL_X86_SPECIAL_STRUCT_RETURN |
                         FECALL_X86_NONE;
    CdeclInfo.parms = front.callinfo_file.StackParms;
    CdeclInfo.objname = AUX_STRALLOC( front, "_*" );

    HW_CAsgn( CdeclInfo.returns, HW_EMPTY );
    HW_TurnOn( CdeclInfo.save, full_no_segs );
    HW_CTurnOff( CdeclInfo.save, HW_FLTS );
  #if _CPU == 8086
    HW_CAsgn( CdeclInfo.streturn, HW_AX );
    HW_CTurnOff( CdeclInfo.save, HW_ABCD );
    HW_CTurnOff( CdeclInfo.save, HW_ES );
  #else /* _CPU == 386 */
    HW_CAsgn( CdeclInfo.streturn, HW_EAX );
    HW_CTurnOff( CdeclInfo.save, HW_EAX );
//    HW_CTurnOff( CdeclInfo.save, HW_EBX );
    HW_CTurnOff( CdeclInfo.save, HW_ECX );
    HW_CTurnOff( CdeclInfo.save, HW_EDX );
  #endif

/*************************************************
 *  __pascal calling convention
 *************************************************/
    PascalInfo.cclass =   cclass |
                         FECALL_GEN_REVERSE_PARMS |
                         //FECALL_GEN_CALLER_POPS |
                         FECALL_GEN_NONE;
    PascalInfo.cclass_target = cclass_target |
                         //FECALL_X86_PARMS_PREFER_REGS |
                         //FECALL_X86_GENERATE_STACK_FRAME |
                         //FECALL_X86_LOAD_DS_ON_CALL |
                         FECALL_X86_NO_FLOAT_REG_RETURNS |
                         FECALL_X86_NO_STRUCT_REG_RETURNS |
                         //FECALL_X86_ROUTINE_RETURN |
                         //FECALL_X86_NO_8087_RETURNS |
                         //FECALL_X86_SPECIAL_RETURN |
                         FECALL_X86_SPECIAL_STRUCT_RETURN |
                         FECALL_X86_NONE;
    PascalInfo.parms = front.callinfo_file.StackParms;
    PascalInfo.objname = AUX_STRALLOC( front, "^" );

    HW_CAsgn( PascalInfo.returns, HW_EMPTY );
    HW_CAsgn( PascalInfo.streturn, HW_EMPTY );
    HW_TurnOn( PascalInfo.save, full_no_segs );
    HW_CTurnOff( PascalInfo.save, HW_FLTS );
  #if _CPU == 8086
    HW_CTurnOff( PascalInfo.save, HW_ABCD );
    HW_CTurnOff( PascalInfo.save, HW_ES );
  #else /* _CPU == 386 */
    HW_CTurnOff( PascalInfo.save, HW_EAX );
    HW_CTurnOff( PascalInfo.save, HW_EBX );
    HW_CTurnOff( PascalInfo.save, HW_ECX );
    HW_CTurnOff( PascalInfo.save, HW_EDX );
  #endif


/*************************************************
 *  __stdcall calling convention
 *************************************************/
    StdcallInfo.cclass =  cclass |
                         //FECALL_GEN_REVERSE_PARMS |
                         //FECALL_GEN_CALLER_POPS |
                         FECALL_GEN_NONE;
    StdcallInfo.cclass_target = cclass_target |
                         //FECALL_X86_PARMS_PREFER_REGS |
                         //FECALL_X86_GENERATE_STACK_FRAME |
                         //FECALL_X86_LOAD_DS_ON_CALL |
                         //FECALL_X86_NO_FLOAT_REG_RETURNS |
                         //FECALL_X86_NO_STRUCT_REG_RETURNS |
                         //FECALL_X86_ROUTINE_RETURN |
                         //FECALL_X86_NO_8087_RETURNS |
                         //FECALL_X86_SPECIAL_RETURN |
                         FECALL_X86_SPECIAL_STRUCT_RETURN |
                         FECALL_X86_NONE;
    StdcallInfo.parms = front.callinfo_file.StackParms;
    if( flag_stdatnum ) {
        StdcallInfo.objname = AUX_STRALLOC( front, "_*#" );
    } else {
        StdcallInfo.objname = AUX_STRALLOC( front, "_*" );
    }

    HW_CAsgn( StdcallInfo.returns, HW_EMPTY );
    HW_TurnOn( StdcallInfo.save, full_no_segs );
    HW_CTurnOff( StdcallInfo.save, HW_FLTS );
  #if _CPU == 8086
    HW_CAsgn( StdcallInfo.streturn, HW_AX );
    HW_CTurnOff( StdcallInfo.save, HW_ABCD );
    HW_CTurnOff( StdcallInfo.save, HW_ES );
  #else /* _CPU == 386 */
    HW_CAsgn( StdcallInfo.streturn, HW_EMPTY );
//    HW_CAsgn( StdcallInfo.streturn, HW_EAX );
    HW_CTurnOff( StdcallInfo.save, HW_EAX );
//    HW_CTurnOff( StdcallInfo.save, HW_EBX );
    HW_CTurnOff( StdcallInfo.save, HW_ECX );
    HW_CTurnOff( StdcallInfo.save, HW_EDX );
  #endif


/*************************************************
 *  __fastcall calling convention
 *************************************************/
    FastcallInfo.parms = front.callinfo_file.FastcallParms;
    HW_CAsgn( FastcallInfo.returns, HW_EMPTY );
    HW_CAsgn( FastcallInfo.streturn, HW_EMPTY );
    HW_TurnOn( FastcallInfo.save, full_no_segs );
    HW_CTurnOff( FastcallInfo.save, HW_FLTS );
    FastcallInfo.cclass = cclass |
                         //FECALL_GEN_REVERSE_PARMS |
                         //FECALL_GEN_CALLER_POPS |
                         FECALL_GEN_NONE;
    FastcallInfo.cclass_target = cclass_target |
  #if _CPU == 8086
                         FECALL_X86_PARMS_PREFER_REGS |
  #else /* _CPU == 386 */
                         //FECALL_X86_PARMS_PREFER_REGS |
  #endif
                         //FECALL_X86_GENERATE_STACK_FRAME |
                         //FECALL_X86_LOAD_DS_ON_CALL |
                         //FECALL_X86_NO_FLOAT_REG_RETURNS |
                         //FECALL_X86_NO_STRUCT_REG_RETURNS |
                         //FECALL_X86_ROUTINE_RETURN |
                         //FECALL_X86_NO_8087_RETURNS |
                         //FECALL_X86_SPECIAL_RETURN |
                         FECALL_X86_SPECIAL_STRUCT_RETURN |
                         FECALL_X86_NONE;
  #if _CPU == 8086
    FastcallInfo.objname = AUX_STRALLOC( front, "@*" );
    HW_CTurnOff( FastcallInfo.save, HW_ABCD );
    HW_CTurnOff( FastcallInfo.save, HW_ES );
  #else /* _CPU == 386 */
    FastcallInfo.objname = AUX_STRALLOC( front, "@*#" );
    HW_CTurnOff( FastcallInfo.save, HW_EAX );
//    HW_CTurnOff( FastcallInfo.save, HW_EBX );
    HW_CTurnOff( FastcallInfo.save, HW_ECX );
    HW_CTurnOff( FastcallInfo.save, HW_EDX );
  #endif

/*************************************************
 *  _Optlink calling convention
 *************************************************/
    OptlinkInfo.cclass =  cclass |
                         //FECALL_GEN_REVERSE_PARMS |
                         FECALL_GEN_CALLER_POPS  |
                         FECALL_GEN_NONE;
    OptlinkInfo.cclass_target = cclass_target |
                         FECALL_X86_PARMS_STACK_RESERVE |
                         //FECALL_X86_GENERATE_STACK_FRAME |
                         //FECALL_X86_LOAD_DS_ON_CALL |
                         //FECALL_X86_NO_FLOAT_REG_RETURNS |
                         FECALL_X86_NO_STRUCT_REG_RETURNS |
                         //FECALL_X86_ROUTINE_RETURN |
                         //FECALL_X86_NO_8087_RETURNS |
                         //FECALL_X86_SPECIAL_RETURN |
                         FECALL_X86_SPECIAL_STRUCT_RETURN |
                         FECALL_X86_NONE;
  #if _CPU == 8086
    OptlinkInfo.parms = front.callinfo_file.StackParms;
  #else /* _CPU == 386 */
    OptlinkInfo.parms = front.callinfo_file.OptlinkParms;
  #endif
    OptlinkInfo.objname = AUX_STRALLOC( front, "*" );

    HW_CAsgn( OptlinkInfo.returns, HW_EMPTY );
//    HW_CAsgn( OptlinkInfo.returns, HW_FLTS );
    HW_CAsgn( OptlinkInfo.streturn, HW_EMPTY );
    HW_TurnOn( OptlinkInfo.save, full_no_segs );
    HW_CTurnOff( OptlinkInfo.save, HW_FLTS );
  #if _CPU == 8086
    HW_CTurnOff( OptlinkInfo.save, HW_ABCD );
    HW_CTurnOff( OptlinkInfo.save, HW_ES );
  #else /* _CPU == 386 */
    HW_CTurnOff( OptlinkInfo.save, HW_EAX );
//    HW_CTurnOff( OptlinkInfo.save, HW_EBX );
    HW_CTurnOff( OptlinkInfo.save, HW_ECX );
    HW_CTurnOff( OptlinkInfo.save, HW_EDX );
  #endif

/*************************************************
 *  __syscall calling convention
 *************************************************/
    SyscallInfo.cclass =  cclass |
                         //FECALL_GEN_REVERSE_PARMS |
                         FECALL_GEN_CALLER_POPS |
                         FECALL_GEN_NONE;
    SyscallInfo.cclass_target = cclass_target |
                         //FECALL_X86_GENERATE_STACK_FRAME |
                         //FECALL_X86_LOAD_DS_ON_CALL |
                         //FECALL_X86_NO_FLOAT_REG_RETURNS |
                         FECALL_X86_NO_STRUCT_REG_RETURNS |
                         //FECALL_X86_ROUTINE_RETURN |
                         //FECALL_X86_NO_8087_RETURNS |
                         //FECALL_X86_SPECIAL_RETURN |
                         FECALL_X86_SPECIAL_STRUCT_RETURN |
                         FECALL_X86_NONE;
    SyscallInfo.parms = front.callinfo_file.StackParms;
    SyscallInfo.objname = AUX_STRALLOC( front, "*" );

    HW_CAsgn( SyscallInfo.returns, HW_EMPTY );
    HW_CAsgn( SyscallInfo.streturn, HW_EMPTY );
    HW_TurnOn( SyscallInfo.save, full_no_segs );
    HW_CTurnOff( SyscallInfo.save, HW_FLTS );
  #if _CPU == 8086
    HW_CTurnOff( SyscallInfo.save, HW_ABCD );
    HW_CTurnOff( SyscallInfo.save, HW_ES );
  #else /* _CPU == 386 */
    HW_CTurnOff( SyscallInfo.save, HW_EAX );
//    HW_CTurnOff( SyscallInfo.save, HW_EBX );
    HW_CTurnOff( SyscallInfo.save, HW_ECX );
    HW_CTurnOff( SyscallInfo.save, HW_EDX );
  #endif

  #if _CPU == 386
/****************************************************
 *  OS/2 32-bit->16-bit calling convention ( _Far16 )
 ****************************************************/
    /* these are internal, and will never be pointed to by
     * an aux_entry, so we don't have to worry about them
     */

    Far16CdeclInfo = CdeclInfo;
    Far16CdeclInfo.cclass_target |= FECALL_X86_FAR16_CALL;
    Far16CdeclInfo.parms = front.callinfo_file.StackParms;
    Far16CdeclInfo.objname = AUX_STRALLOC( front, CdeclInfo.objname );
    // __far16 __cdecl depends on EBX being trashed in __cdecl
    // but NT 386 __cdecl preserves EBX
    HW_CTurnOff( Far16CdeclInfo.save, HW_EBX );

    Far16PascalInfo = PascalInfo;
    Far16PascalInfo.cclass_target |= FECALL_X86_FAR16_CALL;
    Far16PascalInfo.parms = front.callinfo_file.StackParms;
    Far16PascalInfo.objname = AUX_STRALLOC( front, PascalInfo.objname );
  #endif
}

  #if _CPU == 386
void SetAuxStackConventions( FrontRun& front )
/*********************************/
{
    WatcallInfo.cclass &= FECALL_GEN_NONE;
    WatcallInfo.cclass |= FECALL_GEN_CALLER_POPS |
                          FECALL_GEN_NONE;
    WatcallInfo.cclass_target &= FECALL_X86_GENERATE_STACK_FRAME |
                                 FECALL_X86_FAR_CALL |
                                 FECALL_X86_NONE;
    WatcallInfo.cclass_target |= FECALL_X86_NO_8087_RETURNS |
                                 FECALL_X86_NONE;
    WatcallInfo.parms = front.callinfo_file.MetaWareParms;
    HW_CTurnOff( WatcallInfo.save, HW_EAX );
//    HW_CTurnOff( WatcallInfo.save, HW_EBX );
    HW_CTurnOff( WatcallInfo.save, HW_ECX );
    HW_CTurnOff( WatcallInfo.save, HW_EDX );
    HW_CTurnOff( WatcallInfo.save, HW_FLTS );
    WatcallInfo.objname = AUX_STRALLOC( front, "*" );
}
  #endif


int IsAuxParmsBuiltIn( FrontRun& front, hw_reg_set *parms )
/***************************************/
{
    if( parms == front.callinfo_file.WatcallParms ) {
        return( true );
    } else if( parms == front.callinfo_file.StackParms ) {
        return( true );
    } else if( parms == front.callinfo_file.FastcallParms ) {
        return( true );
  #if _CPU == 386
    } else if( parms == front.callinfo_file.OptlinkParms ) {
        return( true );
    } else if( parms == front.callinfo_file.MetaWareParms ) {
        return( true );
  #endif
    } else if( parms == NULL ) {
        return( true );
    } else {
        return( false );
    }
}

void SetAuxWatcallInfo( FrontRun& front )
/****************************/
{
    front.callinfo.DftCallConv         = &DEFAULT_CALLINFO;

    WatcallInfo.cclass        = FECALL_GEN_NONE;
    WatcallInfo.cclass_target = FECALL_X86_NONE;
    WatcallInfo.code    = NULL;
    WatcallInfo.parms   = front.callinfo_file.WatcallParms;
    HW_CAsgn( WatcallInfo.returns, HW_EMPTY );
    HW_CAsgn( WatcallInfo.streturn, HW_EMPTY );
    HW_CAsgn( WatcallInfo.save, HW_FULL );
    WatcallInfo.use     = 0;
    WatcallInfo.objname = NULL;
}

void SetDefaultAuxInfo( FrontRun& front )
/****************************/
{
    DefaultInfo = *front.callinfo.DftCallConv;
    if( DefaultInfo.objname != NULL ) {
        DefaultInfo.objname = AUX_STRALLOC( front, DefaultInfo.objname );
    }
}

int IsAuxInfoBuiltIn( FrontRun& front, aux_info *inf )
/***********************************/
{
    if( inf == &DefaultInfo )
        return( true );
    if( inf == &WatcallInfo )
        return( true );
    if( inf == &CdeclInfo )
        return( true );
    if( inf == &PascalInfo )
        return( true );
    if( inf == &FortranInfo )
        return( true );
    if( inf == &SyscallInfo )
        return( true );
    if( inf == &StdcallInfo )
        return( true );
    if( inf == &FastcallInfo )
        return( true );
    if( inf == &OptlinkInfo )
        return( true );
#if _CPU == 386
    if( inf == &Far16PascalInfo )
        return( true );
    if( inf == &Far16CdeclInfo )
        return( true );
#endif
    return( false );
}

/* Variables need name mangling different from functions in many cases
 * (__pascal, __stdcall, etc.). Note that _System and _Optlink do not
 * decorate variable names. This is required for OS/2 SOM support to work
 * and useful for interoperability with IBM compilers.
 * NB: WatcallInfo.objname value depends on the -3r/3s switch.
 */
char *VarNamePattern( FrontRun& front, aux_info *inf )
/***********************************/
{
    if( inf == &DefaultInfo )
        inf = front.callinfo.DftCallConv;
    if( inf == &WatcallInfo )
        return( WatcallInfo.objname );
    if( inf == &CdeclInfo || inf == &StdcallInfo || inf == &FastcallInfo )
        return const_cast<char *>(( "_*" ));
    if( inf == &PascalInfo  || inf == &FortranInfo )
        return const_cast<char *>(( "^" ));
    if( inf == &SyscallInfo || inf == &OptlinkInfo )
        return const_cast<char *>(( "*" ));
    return( inf->objname );
}
