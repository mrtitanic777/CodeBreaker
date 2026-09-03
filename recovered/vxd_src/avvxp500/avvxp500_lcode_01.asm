; avvxp500_lcode_01.asm -- AVVXP500 object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 021h, 043h, 004h, 000h, 000h, 000h, 061h, 076h, 076h, 078h
        db  070h, 035h, 030h, 030h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
avvxp500_Control:
        cmp    eax, 0bh
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_0085:
        cmp    eax, 1bh
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_008E:
        cmp    eax, 1ch
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_0097:
        cmp    eax, 22h
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_00A0:
        clc
        ret
        db  08Bh, 0C0h
loc_1_00A4:
        push   eax
        push   edi
        db  0A1h, 01Ch, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_00B1
        mov    ebx, eax
loc_1_00B1:
        pop    edi
        pop    eax
        db  0CDh, 020h, 002h, 000h, 003h, 000h
        clc
        ret
        db  090h
loc_1_00BC:
        push   ecx
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        pop    ecx
        clc
        ret
loc_1_00CC:
        push   edi
        db  039h, 01Dh, 01Ch, 000h, 000h, 000h
        jne    SHORT loc_1_00E7
        jecxz  loc_1_00DF
loc_1_00D7:
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        jmp    SHORT loc_1_00F4
loc_1_00DF:
        db  0CDh, 020h, 009h, 000h, 003h, 000h
        jmp    SHORT loc_1_00F4
loc_1_00E7:
        db  066h, 0F7h, 005h, 00Ch, 000h, 000h, 000h, 000h, 080h
        jne    SHORT loc_1_00DF
        je     SHORT loc_1_00D7
loc_1_00F4:
        pop    edi
        clc
        ret
        db  0CCh
_LTEXT ends
        end
