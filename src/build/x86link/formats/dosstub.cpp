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
* Description:  DOS stub stuff
*
****************************************************************************/


#include <array>
#include <cstddef>
#include <cstring>
#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "exedos.h"
#include "build/x86link/formats/output.h"
#include "build/x86link/formats/dosstub.h"


namespace {

constexpr std::array<unsigned_8, 78> DOSDefStub = {
    0x4D, 0x5A, 0x80, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x04, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00,
    0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00,
    0x0E, 0x1F, 0xBA, 0x0E, 0x00, 0xB4, 0x09, 0xCD,
    0x21, 0xB8, 0x01, 0x4C, 0xCD, 0x21
};

/* make up the "program is %f" string, and put it in tokbuff.*/
unsigned_32 doExeTypeString(Link& link)
{
    std::array<char, RESOURCE_MAX_SIZE> rc_buff;
    unsigned_32 msgsize;

    Msg_Get( MSG_IS_A_EXE, rc_buff.data() );
    msgsize = static_cast<unsigned_32>( FmtStr(link,  link.TokBuff, link.TokSize, rc_buff.data() ) );
    link.TokBuff[msgsize++] = '\r';
    link.TokBuff[msgsize++] = '\n';
    link.TokBuff[msgsize] = '$';     /* end of string for int 21 fn. 9 */
    return msgsize + 1;
}

}  // namespace

unsigned_32 GetDOSDefStubSize(Link& link)
/***********************************/
{
    return static_cast<unsigned_32>( DOSDefStub.size() ) + doExeTypeString(link);
}

unsigned_32 WriteDOSDefStub(Link& link,  unsigned_32 stub_align )
/****************************************************
 * write the default stub to the executable file
 */
{
    const unsigned_32   stubsize = static_cast<unsigned_32>( DOSDefStub.size() );
    const unsigned_32   msgsize = doExeTypeString(link);
    const unsigned_32   fullsize = __ROUND_UP_SIZE( msgsize + stubsize, stub_align );
    std::array<unsigned_8, DOSDefStub.size()>   stub = DOSDefStub;

    std::memcpy( stub.data() + NE_HEADER_OFFSET, &fullsize, sizeof( fullsize ) );
    WriteLoad(link,  stub.data(), stub.size() );
    WriteLoad(link,  link.TokBuff, msgsize );
    PadLoad(link,  fullsize - msgsize - stubsize );
    return fullsize;
}
