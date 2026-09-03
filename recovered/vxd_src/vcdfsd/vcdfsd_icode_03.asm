; vcdfsd_icode_03.asm -- VCDFSD object 3 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 0C8h, 016h, 000h, 01Bh, 000h, 06Dh, 073h, 063h, 064h, 024h, 024h
        db  024h, 024h, 000h, 000h, 000h, 000h, 0EAh, 000h, 000h, 000h, 000h, 0EAh, 000h, 000h, 000h, 000h
loc_3_0020:
        db  066h, 089h, 015h, 0C4h, 000h, 000h, 000h
        clc
        ret
loc_3_0029:
        push   ebx
        push   edi
        push   esi
        db  0CDh, 020h, 000h, 000h, 040h, 000h
        jb     NEAR PTR loc_3_00C4
        cmp    eax, 22h
        jne    NEAR PTR loc_3_00D2
        db  0CDh, 020h, 000h, 000h, 010h, 000h
        jb     SHORT loc_3_00C4
        call   sub_3_00F5
        jb     SHORT loc_3_00C4
        push   204h
        push   20h
        db  0CDh, 020h, 0A8h, 000h, 001h, 000h
        lea    esp, [esp + 8]
        or     eax, eax
        je     SHORT loc_3_00C3
        shr    eax, 4
        shl    eax, 10h
        db  0A3h, 0C8h, 000h, 000h, 000h
        shr    eax, 0ch
        call   sub_3_0225
        jb     SHORT loc_3_00C4
        mov    eax, 2fh
        db  0BEh, 03Ah, 000h, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        jb     SHORT loc_3_00C4
        push   0
        push   22h
        db  068h, 017h, 000h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 040h, 000h
        lea    esp, [esp + 0ch]
        cmp    eax, -1
        je     SHORT loc_3_00A6
        jmp    SHORT loc_3_00B9
loc_3_00A6:
        mov    eax, 2fh
        db  0BEh, 03Ah, 000h, 000h, 000h
        db  0CDh, 020h, 018h, 001h, 001h, 000h
        stc
        jmp    SHORT loc_3_00C4
loc_3_00B9:
        db  066h, 0C7h, 005h, 0C0h, 000h, 000h, 000h, 001h, 000h
        stc
loc_3_00C3:
        cmc
loc_3_00C4:
        db  0C7h, 005h, 08Ch, 001h, 000h, 000h, 032h, 000h, 000h, 000h
        pop    esi
        pop    edi
        pop    ebx
        ret
loc_3_00D2:
        clc
        jmp    SHORT loc_3_00C3
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_3_00F5:
        push   ebx
        push   edi
        push   esi
        db  066h, 081h, 03Dh, 0C4h, 000h, 000h, 000h, 010h, 001h
        jle    NEAR PTR loc_3_0214
        push   0
        push   82h
        db  0CDh, 020h, 0A9h, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_3_021E
        db  0A3h, 0D5h, 000h, 000h, 000h
        push   ebx
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 1500h
        mov    word ptr [ebp + 10h], 0
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    ax, word ptr [ebp + 10h]
        db  066h, 0A3h, 0D9h, 000h, 000h, 000h
        cmp    ax, 1ah
        ja     NEAR PTR loc_3_01F8
        cmp    ax, 0
        je     NEAR PTR loc_3_01F8
        mov    word ptr [ebp + 1ch], 150dh
        db  0A1h, 0D5h, 000h, 000h, 000h
        shr    eax, 4
        mov    word ptr [ebp + 10h], 0
        mov    word ptr [ebp + 38h], ax
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  00Fh, 0B7h, 00Dh, 0D9h, 000h, 000h, 000h
        db  08Bh, 035h, 0D5h, 000h, 000h, 000h
        db  0BFh, 0DBh, 000h, 000h, 000h
loc_3_01A4:
        lodsb
        cmp    al, 1ah
        jae    SHORT loc_3_01F8
        stosb
        loop   loc_3_01A4
        mov    word ptr [ebp + 1ch], 1501h
        db  0A1h, 0D5h, 000h, 000h, 000h
        shr    eax, 4
        mov    word ptr [ebp + 10h], 0
        mov    word ptr [ebp + 38h], ax
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        xor    ecx, ecx
        db  08Bh, 035h, 0D5h, 000h, 000h, 000h
loc_3_01D7:
        db  00Fh, 0B6h, 099h, 0DBh, 000h, 000h, 000h
        db  08Dh, 03Ch, 09Dh, 0D0h, 000h, 000h, 000h
        lea    edi, [edi + ebx*2]
        lodsb
        mov    ah, 28h
        stosw
        movsd
        inc    ecx
        db  066h, 03Bh, 00Dh, 0D9h, 000h, 000h, 000h
        jb     SHORT loc_3_01D7
loc_3_01F8:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    ebx
        db  0CDh, 020h, 0AAh, 000h, 001h, 000h
loc_3_0214:
        xor    eax, eax
loc_3_0216:
        cmp    eax, 1
        cmc
        pop    esi
        pop    edi
        pop    ebx
        ret
loc_3_021E:
        mov    eax, 8
        jmp    SHORT loc_3_0216
sub_3_0225:
        push   eax
        xor    edx, edx
        db  0BEh, 014h, 000h, 000h, 000h
        db  0CDh, 020h, 00Bh, 000h, 001h, 000h
        db  0A3h, 017h, 000h, 000h, 000h
        mov    edx, 1
        db  0BEh, 01Ch, 000h, 000h, 000h
        db  0CDh, 020h, 00Bh, 000h, 001h, 000h
        db  0A3h, 01Ch, 000h, 000h, 000h
        db  08Dh, 035h, 000h, 000h, 000h, 000h
        pop    edi
        push   edi
        mov    ecx, 8
        rep movsd
        pop    eax
        push   eax
        xor    ebx, ebx
        db  0CDh, 020h, 005h, 000h, 015h, 000h
        pop    eax
        ret
loc_3_0268:
        cmp    eax, 0
        je     NEAR PTR loc_3_0020
        cmp    eax, 1
        je     NEAR PTR loc_3_0029
        clc
        ret
_ITEXT ends
        end
