#pragma once
/* jumpsout.h -- the opcode bytes of jumps and calls, and the addresses that follow them, as the encoder puts them into the object
 * (x86esc.cpp, x86data.cpp). The bytes of the 8086 and 80386 targets differ in the size of an offset and the 66h prefix. */
#include "model.h"

inline void OutShortD( BackRun& back, offset addr ) { OutDataByte( back, addr - AskLocation( back ) - 1 ); }
inline void OutFarSeg( BackRun& back, uint_16 seg ) { OutDataShort( back, seg ); }
#if _TARGET & _TARG_8086
    inline void OutOpndSize( BackRun& ) {}
    inline void OutFarD( BackRun& back, uint_16 seg, uint_16 off ) { OutDataShort( back, seg ); OutDataShort( back, off ); }
    inline void OutFarOff( BackRun& back, uint_16 off ) { OutDataShort( back, off ); }
    inline void OutNearD( BackRun& back, offset addr ) { OutDataShort( back, addr - AskLocation( back ) - 2 ); }
#else
    inline void OutOpndSize( BackRun& back )
    {
        if( IsntTargetModel( back, CGSW_X86_USE_32 ) ) {
            OutDataByte( back, 0x66 );
        }
    }
    inline void OutFarOff( BackRun& back, uint_32 off ) { OutDataLong( back, off ); }
    inline void OutNearD( BackRun& back, offset addr ) { OutDataLong( back, addr - AskLocation( back ) - 4 ); }
    inline void OutFarD( BackRun& back, uint_16 seg, uint_32 off ) { OutDataShort( back, seg ); OutDataLong( back, off ); }
#endif
inline void OutCCyp( BackRun& back ) { OutDataByte( back, 0x0e ); OutOpndSize( back ); OutDataByte( back, 0xe8 ); }
inline void OutCNear( BackRun& back ) { OutOpndSize( back ); OutDataByte( back, 0xe8 ); }
inline void OutJNear( BackRun& back ) { OutOpndSize( back ); OutDataByte( back, 0xe9 ); }
inline void OutCFar( BackRun& back ) { OutOpndSize( back ); OutDataByte( back, 0x9a ); }
inline void OutJFar( BackRun& back ) { OutOpndSize( back ); OutDataByte( back, 0xea ); }
inline void OutJShort( BackRun& back ) { OutDataByte( back, 0xeb ); }
inline void OutJCond( BackRun& back, int cond ) { OutDataByte( back, 0x70 + cond ); }
inline void OutJCondNear( BackRun& back, int cond ) { OutOpndSize( back ); OutDataByte( back, 0x0f ); OutDataByte( back, 0x80 + cond ); }
