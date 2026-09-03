; dosmgr_lcode_01.asm -- DOSMGR object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

DOSMGR_Control:
        cmp    eax, 1ah
        jae    SHORT loc_1_000C
        db  0FFh, 024h, 085h, 00Eh, 000h, 000h, 000h
loc_1_000C:
        clc
        ret
        db  000h, 000h, 000h, 000h, 05Ch, 001h, 000h, 000h, 057h, 004h, 000h, 000h, 000h, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 065h, 001h, 000h, 000h, 017h, 001h, 000h, 000h, 0CCh, 00Bh, 000h, 000h
        db  000h, 000h, 000h, 000h, 038h, 00Ch, 000h, 000h, 00Ch, 000h, 000h, 000h, 01Ah, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h
        db  00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h
        db  045h, 001h, 000h, 000h, 042h, 00Eh, 000h, 000h, 030h, 002h, 000h, 000h, 0A5h, 001h, 000h, 000h
        db  00Ch, 000h, 000h, 000h, 063h, 000h, 000h, 000h
DOSMGR_Get_Version:
        mov    eax, 400h
        clc
        ret
DOSMGR_LocalGlobalReg:
        pushad
        db  083h, 03Dh, 014h, 003h, 000h, 000h, 000h
        je     SHORT loc_1_00E8
        db  080h, 03Dh, 020h, 003h, 000h, 000h, 000h
        je     SHORT loc_1_00E8
        db  083h, 03Dh, 018h, 003h, 000h, 000h, 000h
        je     SHORT loc_1_00E8
        cmp    edx, edi
        ja     SHORT loc_1_00E8
        cmp    edx, 100h
        jae    SHORT loc_1_00E8
        db  0BEh, 014h, 002h, 000h, 000h
        db  08Bh, 00Dh, 014h, 003h, 000h, 000h
        jecxz  loc_1_00E8
loc_1_00B2:
        test   byte ptr [esi + 5], 1
        jne    SHORT loc_1_00E3
        movzx  eax, word ptr [esi]
        dec    eax
        shr    eax, 8
        movzx  ebx, word ptr [esi + 2]
        inc    ebx
        test   byte ptr [esi + 4], 2
        je     SHORT loc_1_00CB
        inc    ebx
loc_1_00CB:
        shr    ebx, 8
        add    ebx, eax
        dec    ebx
        cmp    edi, eax
        jb     SHORT loc_1_00E3
        cmp    edx, ebx
        ja     SHORT loc_1_00E3
        cmp    edi, ebx
        ja     SHORT loc_1_00FC
        cmp    edx, eax
        jb     SHORT loc_1_00FC
        jmp    SHORT loc_1_00F2
loc_1_00E3:
        add    esi, 10h
        loop   loc_1_00B2
loc_1_00E8:
        mov    dword ptr [esp + 1ch], 3
loc_1_00F0:
        popad
        ret
loc_1_00F2:
        mov    dword ptr [esp + 1ch], 0
        jmp    SHORT loc_1_00F0
loc_1_00FC:
        mov    dword ptr [esp + 1ch], 2
        jmp    SHORT loc_1_00F0
DOSMGR_Get_DOS_Crit_Status:
        push   eax
        db  0A1h, 0F8h, 001h, 000h, 000h
        movzx  eax, byte ptr [eax]
        db  00Bh, 005h, 0FCh, 001h, 000h, 000h
        pop    eax
        ret
DOSMGR_SYS_CRITICAL_EXIT:
        db  0E8h, 09Ch, 009h, 0FFh, 0FFh
        db  0A1h, 008h, 000h, 000h, 000h
        mov    dword ptr [8ch], eax
        db  0A1h, 084h, 001h, 000h, 000h
        test   eax, eax
        je     SHORT loc_1_0135
        xor    edx, edx
        dec    dx
        mov    dword ptr [eax], edx
loc_1_0135:
        db  08Bh, 03Dh, 058h, 000h, 000h, 000h
        xor    eax, eax
        mov    dword ptr [edi + 0ch], eax
        mov    dword ptr [edi + 10h], eax
        clc
        ret
DOSMGR_DEBUG_QUERY:
        db  0F7h, 005h, 04Bh, 001h, 000h, 000h, 000h, 000h, 040h, 000h
        je     SHORT loc_1_0156
        db  0E9h, 0AAh, 0FEh, 0FEh, 0FFh
loc_1_0156:
        ret
        db  000h, 076h, 000h, 000h, 000h, 078h, 000h, 000h, 000h, 0EFh, 009h, 000h, 000h, 0A3h, 003h, 000h
        db  000h, 02Ch, 004h, 000h, 000h, 0CDh, 00Ah, 000h, 000h, 01Dh, 00Bh, 000h, 000h, 09Dh, 017h, 000h
        db  000h, 006h, 001h, 000h, 000h, 058h, 00Ch, 000h, 000h, 0ACh, 00Bh, 000h, 000h, 07Dh, 000h, 000h
        db  000h, 084h, 00Ch, 000h, 000h, 084h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 049h, 00Ch, 000h
        db  000h, 0E5h, 00Bh, 000h, 000h, 003h, 012h, 000h, 000h, 0BDh, 001h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 015h, 000h, 004h, 000h, 000h, 000h, 044h, 04Fh, 053h, 04Dh
        db  047h, 052h, 020h, 020h, 000h, 000h, 000h, 058h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  058h, 001h, 000h, 000h, 013h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h
loc_1_01F8:
        or     al, 2
        add    byte ptr [eax], al
loc_1_01FC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0200:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0204:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0208:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_020C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0210:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0214:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0314:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0318:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_031C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0320:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end
