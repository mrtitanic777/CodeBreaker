; spap_pcode_02.asm -- SPAP object 2 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        push   ebp
        mov    ebp, esp
        db  083h, 03Dh, 030h, 01Bh, 000h, 000h, 000h
        je     SHORT loc_2_0015
        mov    eax, 1
        pop    ebp
        ret    4
loc_2_0015:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 062h, 00Fh, 0FEh, 0FFh
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        db  0C7h, 005h, 030h, 01Bh, 000h, 000h, 001h, 000h, 000h, 000h
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0040:
        mov    eax, 1
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_0050:
        mov    eax, 1
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_0060:
        push   1
        call   0
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_0070:
        push   0
        call   0
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_0080:
        db  0C7h, 005h, 030h, 01Bh, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 1
        ret
sub_2_0090:
        ret
        db  030h, 045h, 06Eh
_PTEXT ends
        end
