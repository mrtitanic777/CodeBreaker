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
static inline aux_info& DefaultCallInfo( FrontRun& front ) { return WatcallInfo( front ); }





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
    cclass_target   = WatcallInfo( front ).cclass_target & FECALL_X86_FAR_CALL;

/*************************************************
 *  __fortran calling convention
 *************************************************/
    FortranInfo( front ).objname = AUX_STRALLOC( front, "^" );

/*************************************************
 *  __cdecl calling convention
 *************************************************/
    CdeclInfo( front ).cclass =    cclass |
                         //FECALL_GEN_REVERSE_PARMS |
                         FECALL_GEN_CALLER_POPS |
                         FECALL_GEN_NONE;
    CdeclInfo( front ).cclass_target = cclass_target |
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
    CdeclInfo( front ).parms = front.callinfo_file.StackParms;
    CdeclInfo( front ).objname = AUX_STRALLOC( front, "_*" );

    HW_CAsgn( CdeclInfo( front ).returns, HW_EMPTY );
    HwTurnOn( CdeclInfo( front ).save, full_no_segs );
    HW_CTurnOff( CdeclInfo( front ).save, HW_FLTS );
  #if _CPU == 8086
    HW_CAsgn( CdeclInfo( front ).streturn, HW_AX );
    HW_CTurnOff( CdeclInfo( front ).save, HW_ABCD );
    HW_CTurnOff( CdeclInfo( front ).save, HW_ES );
  #else /* _CPU == 386 */
    HW_CAsgn( CdeclInfo( front ).streturn, HW_EAX );
    HW_CTurnOff( CdeclInfo( front ).save, HW_EAX );
//    HW_CTurnOff( CdeclInfo.save, HW_EBX );
    HW_CTurnOff( CdeclInfo( front ).save, HW_ECX );
    HW_CTurnOff( CdeclInfo( front ).save, HW_EDX );
  #endif

/*************************************************
 *  __pascal calling convention
 *************************************************/
    PascalInfo( front ).cclass =   cclass |
                         FECALL_GEN_REVERSE_PARMS |
                         //FECALL_GEN_CALLER_POPS |
                         FECALL_GEN_NONE;
    PascalInfo( front ).cclass_target = cclass_target |
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
    PascalInfo( front ).parms = front.callinfo_file.StackParms;
    PascalInfo( front ).objname = AUX_STRALLOC( front, "^" );

    HW_CAsgn( PascalInfo( front ).returns, HW_EMPTY );
    HW_CAsgn( PascalInfo( front ).streturn, HW_EMPTY );
    HwTurnOn( PascalInfo( front ).save, full_no_segs );
    HW_CTurnOff( PascalInfo( front ).save, HW_FLTS );
  #if _CPU == 8086
    HW_CTurnOff( PascalInfo( front ).save, HW_ABCD );
    HW_CTurnOff( PascalInfo( front ).save, HW_ES );
  #else /* _CPU == 386 */
    HW_CTurnOff( PascalInfo( front ).save, HW_EAX );
    HW_CTurnOff( PascalInfo( front ).save, HW_EBX );
    HW_CTurnOff( PascalInfo( front ).save, HW_ECX );
    HW_CTurnOff( PascalInfo( front ).save, HW_EDX );
  #endif


/*************************************************
 *  __stdcall calling convention
 *************************************************/
    StdcallInfo( front ).cclass =  cclass |
                         //FECALL_GEN_REVERSE_PARMS |
                         //FECALL_GEN_CALLER_POPS |
                         FECALL_GEN_NONE;
    StdcallInfo( front ).cclass_target = cclass_target |
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
    StdcallInfo( front ).parms = front.callinfo_file.StackParms;
    if( flag_stdatnum ) {
        StdcallInfo( front ).objname = AUX_STRALLOC( front, "_*#" );
    } else {
        StdcallInfo( front ).objname = AUX_STRALLOC( front, "_*" );
    }

    HW_CAsgn( StdcallInfo( front ).returns, HW_EMPTY );
    HwTurnOn( StdcallInfo( front ).save, full_no_segs );
    HW_CTurnOff( StdcallInfo( front ).save, HW_FLTS );
  #if _CPU == 8086
    HW_CAsgn( StdcallInfo( front ).streturn, HW_AX );
    HW_CTurnOff( StdcallInfo( front ).save, HW_ABCD );
    HW_CTurnOff( StdcallInfo( front ).save, HW_ES );
  #else /* _CPU == 386 */
    HW_CAsgn( StdcallInfo( front ).streturn, HW_EMPTY );
//    HW_CAsgn( StdcallInfo.streturn, HW_EAX );
    HW_CTurnOff( StdcallInfo( front ).save, HW_EAX );
//    HW_CTurnOff( StdcallInfo.save, HW_EBX );
    HW_CTurnOff( StdcallInfo( front ).save, HW_ECX );
    HW_CTurnOff( StdcallInfo( front ).save, HW_EDX );
  #endif


/*************************************************
 *  __fastcall calling convention
 *************************************************/
    FastcallInfo( front ).parms = front.callinfo_file.FastcallParms;
    HW_CAsgn( FastcallInfo( front ).returns, HW_EMPTY );
    HW_CAsgn( FastcallInfo( front ).streturn, HW_EMPTY );
    HwTurnOn( FastcallInfo( front ).save, full_no_segs );
    HW_CTurnOff( FastcallInfo( front ).save, HW_FLTS );
    FastcallInfo( front ).cclass = cclass |
                         //FECALL_GEN_REVERSE_PARMS |
                         //FECALL_GEN_CALLER_POPS |
                         FECALL_GEN_NONE;
    FastcallInfo( front ).cclass_target = cclass_target |
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
    FastcallInfo( front ).objname = AUX_STRALLOC( front, "@*" );
    HW_CTurnOff( FastcallInfo( front ).save, HW_ABCD );
    HW_CTurnOff( FastcallInfo( front ).save, HW_ES );
  #else /* _CPU == 386 */
    FastcallInfo( front ).objname = AUX_STRALLOC( front, "@*#" );
    HW_CTurnOff( FastcallInfo( front ).save, HW_EAX );
//    HW_CTurnOff( FastcallInfo.save, HW_EBX );
    HW_CTurnOff( FastcallInfo( front ).save, HW_ECX );
    HW_CTurnOff( FastcallInfo( front ).save, HW_EDX );
  #endif

/*************************************************
 *  _Optlink calling convention
 *************************************************/
    OptlinkInfo( front ).cclass =  cclass |
                         //FECALL_GEN_REVERSE_PARMS |
                         FECALL_GEN_CALLER_POPS  |
                         FECALL_GEN_NONE;
    OptlinkInfo( front ).cclass_target = cclass_target |
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
    OptlinkInfo( front ).parms = front.callinfo_file.StackParms;
  #else /* _CPU == 386 */
    OptlinkInfo( front ).parms = front.callinfo_file.OptlinkParms;
  #endif
    OptlinkInfo( front ).objname = AUX_STRALLOC( front, "*" );

    HW_CAsgn( OptlinkInfo( front ).returns, HW_EMPTY );
//    HW_CAsgn( OptlinkInfo.returns, HW_FLTS );
    HW_CAsgn( OptlinkInfo( front ).streturn, HW_EMPTY );
    HwTurnOn( OptlinkInfo( front ).save, full_no_segs );
    HW_CTurnOff( OptlinkInfo( front ).save, HW_FLTS );
  #if _CPU == 8086
    HW_CTurnOff( OptlinkInfo( front ).save, HW_ABCD );
    HW_CTurnOff( OptlinkInfo( front ).save, HW_ES );
  #else /* _CPU == 386 */
    HW_CTurnOff( OptlinkInfo( front ).save, HW_EAX );
//    HW_CTurnOff( OptlinkInfo.save, HW_EBX );
    HW_CTurnOff( OptlinkInfo( front ).save, HW_ECX );
    HW_CTurnOff( OptlinkInfo( front ).save, HW_EDX );
  #endif

/*************************************************
 *  __syscall calling convention
 *************************************************/
    SyscallInfo( front ).cclass =  cclass |
                         //FECALL_GEN_REVERSE_PARMS |
                         FECALL_GEN_CALLER_POPS |
                         FECALL_GEN_NONE;
    SyscallInfo( front ).cclass_target = cclass_target |
                         //FECALL_X86_GENERATE_STACK_FRAME |
                         //FECALL_X86_LOAD_DS_ON_CALL |
                         //FECALL_X86_NO_FLOAT_REG_RETURNS |
                         FECALL_X86_NO_STRUCT_REG_RETURNS |
                         //FECALL_X86_ROUTINE_RETURN |
                         //FECALL_X86_NO_8087_RETURNS |
                         //FECALL_X86_SPECIAL_RETURN |
                         FECALL_X86_SPECIAL_STRUCT_RETURN |
                         FECALL_X86_NONE;
    SyscallInfo( front ).parms = front.callinfo_file.StackParms;
    SyscallInfo( front ).objname = AUX_STRALLOC( front, "*" );

    HW_CAsgn( SyscallInfo( front ).returns, HW_EMPTY );
    HW_CAsgn( SyscallInfo( front ).streturn, HW_EMPTY );
    HwTurnOn( SyscallInfo( front ).save, full_no_segs );
    HW_CTurnOff( SyscallInfo( front ).save, HW_FLTS );
  #if _CPU == 8086
    HW_CTurnOff( SyscallInfo( front ).save, HW_ABCD );
    HW_CTurnOff( SyscallInfo( front ).save, HW_ES );
  #else /* _CPU == 386 */
    HW_CTurnOff( SyscallInfo( front ).save, HW_EAX );
//    HW_CTurnOff( SyscallInfo.save, HW_EBX );
    HW_CTurnOff( SyscallInfo( front ).save, HW_ECX );
    HW_CTurnOff( SyscallInfo( front ).save, HW_EDX );
  #endif

  #if _CPU == 386
/****************************************************
 *  OS/2 32-bit->16-bit calling convention ( _Far16 )
 ****************************************************/
    /* these are internal, and will never be pointed to by
     * an aux_entry, so we don't have to worry about them
     */

    Far16CdeclInfo( front ) = CdeclInfo( front );
    Far16CdeclInfo( front ).cclass_target |= FECALL_X86_FAR16_CALL;
    Far16CdeclInfo( front ).parms = front.callinfo_file.StackParms;
    Far16CdeclInfo( front ).objname = AUX_STRALLOC( front, CdeclInfo( front ).objname );
    // __far16 __cdecl depends on EBX being trashed in __cdecl
    // but NT 386 __cdecl preserves EBX
    HW_CTurnOff( Far16CdeclInfo( front ).save, HW_EBX );

    Far16PascalInfo( front ) = PascalInfo( front );
    Far16PascalInfo( front ).cclass_target |= FECALL_X86_FAR16_CALL;
    Far16PascalInfo( front ).parms = front.callinfo_file.StackParms;
    Far16PascalInfo( front ).objname = AUX_STRALLOC( front, PascalInfo( front ).objname );
  #endif
}

  #if _CPU == 386
void SetAuxStackConventions( FrontRun& front )
/*********************************/
{
    WatcallInfo( front ).cclass &= FECALL_GEN_NONE;
    WatcallInfo( front ).cclass |= FECALL_GEN_CALLER_POPS |
                          FECALL_GEN_NONE;
    WatcallInfo( front ).cclass_target &= FECALL_X86_GENERATE_STACK_FRAME |
                                 FECALL_X86_FAR_CALL |
                                 FECALL_X86_NONE;
    WatcallInfo( front ).cclass_target |= FECALL_X86_NO_8087_RETURNS |
                                 FECALL_X86_NONE;
    WatcallInfo( front ).parms = front.callinfo_file.MetaWareParms;
    HW_CTurnOff( WatcallInfo( front ).save, HW_EAX );
//    HW_CTurnOff( WatcallInfo.save, HW_EBX );
    HW_CTurnOff( WatcallInfo( front ).save, HW_ECX );
    HW_CTurnOff( WatcallInfo( front ).save, HW_EDX );
    HW_CTurnOff( WatcallInfo( front ).save, HW_FLTS );
    WatcallInfo( front ).objname = AUX_STRALLOC( front, "*" );
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
    front.callinfo.DftCallConv         = &DefaultCallInfo( front );

    WatcallInfo( front ).cclass        = FECALL_GEN_NONE;
    WatcallInfo( front ).cclass_target = FECALL_X86_NONE;
    WatcallInfo( front ).code    = NULL;
    WatcallInfo( front ).parms   = front.callinfo_file.WatcallParms;
    HW_CAsgn( WatcallInfo( front ).returns, HW_EMPTY );
    HW_CAsgn( WatcallInfo( front ).streturn, HW_EMPTY );
    HW_CAsgn( WatcallInfo( front ).save, HW_FULL );
    WatcallInfo( front ).use     = 0;
    WatcallInfo( front ).objname = NULL;
}

void SetDefaultAuxInfo( FrontRun& front )
/****************************/
{
    DefaultInfo( front ) = *front.callinfo.DftCallConv;
    if( DefaultInfo( front ).objname != NULL ) {
        DefaultInfo( front ).objname = AUX_STRALLOC( front, DefaultInfo( front ).objname );
    }
}

int IsAuxInfoBuiltIn( FrontRun& front, aux_info *inf )
/***********************************/
{
    if( inf == &DefaultInfo( front ) )
        return( true );
    if( inf == &WatcallInfo( front ) )
        return( true );
    if( inf == &CdeclInfo( front ) )
        return( true );
    if( inf == &PascalInfo( front ) )
        return( true );
    if( inf == &FortranInfo( front ) )
        return( true );
    if( inf == &SyscallInfo( front ) )
        return( true );
    if( inf == &StdcallInfo( front ) )
        return( true );
    if( inf == &FastcallInfo( front ) )
        return( true );
    if( inf == &OptlinkInfo( front ) )
        return( true );
#if _CPU == 386
    if( inf == &Far16PascalInfo( front ) )
        return( true );
    if( inf == &Far16CdeclInfo( front ) )
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
    if( inf == &DefaultInfo( front ) )
        inf = front.callinfo.DftCallConv;
    if( inf == &WatcallInfo( front ) )
        return( WatcallInfo( front ).objname );
    if( inf == &CdeclInfo( front ) || inf == &StdcallInfo( front ) || inf == &FastcallInfo( front ) )
        return const_cast<char *>(( "_*" ));
    if( inf == &PascalInfo( front )  || inf == &FortranInfo( front ) )
        return const_cast<char *>(( "^" ));
    if( inf == &SyscallInfo( front ) || inf == &OptlinkInfo( front ) )
        return const_cast<char *>(( "*" ));
    return( inf->objname );
}
