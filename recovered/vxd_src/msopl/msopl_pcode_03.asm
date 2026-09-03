; msopl_pcode_03.asm -- MSOPL object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        mov    eax, 15000eh
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 01Ch, 001h, 001h, 000h
        xor    esi, esi
        db  087h, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_3_004A
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        push   ebx
loc_3_002F:
        xor    esi, esi
        xchg   dword ptr [edi + ebx], esi
        or     esi, esi
        je     SHORT loc_3_003E
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
loc_3_003E:
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, dword ptr [esp]
        jne    SHORT loc_3_002F
        pop    ebx
loc_3_004A:
        clc
        ret
_PTEXT ends
        end
