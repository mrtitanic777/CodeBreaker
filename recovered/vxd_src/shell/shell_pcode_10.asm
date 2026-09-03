; shell_pcode_10.asm -- SHELL object 10 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  0EBh, 006h, 0FFh, 025h, 00Ch, 00Ah, 000h, 000h
loc_10_0008:
        push   ebx
        db  003h, 01Dh, 000h, 00Ah, 000h, 000h
        cmp    ch, 3
        je     SHORT loc_10_0020
        btr    word ptr [ebx], 3
        jae    SHORT loc_10_0023
        or     byte ptr [ebx], 10h
        jmp    SHORT loc_10_0026
loc_10_0020:
        or     byte ptr [ebx], 28h
loc_10_0023:
        and    byte ptr [ebx], 0efh
loc_10_0026:
        pop    ebx
        db  0FFh, 025h, 00Ch, 00Ah, 000h, 000h
        db  000h, 000h, 000h, 0EBh, 006h, 0FFh, 025h, 0B4h, 00Bh, 000h, 000h
loc_10_0038:
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_10_007C
        pushad
        db  08Bh, 02Dh, 06Ch, 00Ah, 000h, 000h
        and    dword ptr [ebp + ebx + 14h], 0
        lea    edi, [esi - 8]
        db  0E8h, 0B4h, 005h, 0F6h, 0FFh
        jb     SHORT loc_10_007B
        mov    word ptr [eax], dx
        mov    edx, eax
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        cmp    word ptr [eax], 0
        je     SHORT loc_10_006F
        cld
        mov    esi, edi
        lea    edi, [edx + 4]
        movsd
        movsd
loc_10_006F:
        db  0E8h, 0CCh, 006h, 0F6h, 0FFh
        xor    edx, edx
        db  0E8h, 0E6h, 005h, 0F6h, 0FFh
loc_10_007B:
        popad
loc_10_007C:
        db  0FFh, 025h, 0B4h, 00Bh, 000h, 000h
        db  0EBh, 006h, 0FFh, 025h, 0B8h, 00Bh, 000h, 000h
loc_10_008A:
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     NEAR PTR loc_10_0125
        pushad
        db  08Bh, 00Dh, 06Ch, 00Ah, 000h, 000h
        and    byte ptr [ecx + ebx + 10h], 0feh
        xor    edx, edx
        xchg   dword ptr [ecx + ebx + 14h], edx
        test   edx, edx
        je     SHORT loc_10_00C7
        db  0CDh, 020h, 040h, 000h, 001h, 000h
        sub    eax, edx
        cmp    eax, 64h
        jae    SHORT loc_10_00C7
        pushad
        push   0
        push   ebx
        db  0CDh, 020h, 019h, 000h, 017h, 000h
        add    esp, 8
        popad
loc_10_00C7:
        mov    edx, dword ptr [esi + 16h]
        db  0E8h, 029h, 005h, 0F6h, 0FFh
loc_10_00CF:
        cmp    word ptr [eax], dx
        je     SHORT loc_10_00DE
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jne    SHORT loc_10_00CF
        jmp    SHORT loc_10_0118
loc_10_00DE:
        xchg   edx, eax
        jmp    SHORT loc_10_010E
loc_10_00E1:
        test   byte ptr [eax + 3], 1
        je     SHORT loc_10_00F8
        pushad
        mov    ecx, 428h
        xor    eax, eax
        xor    esi, esi
        db  0CDh, 020h, 002h, 000h, 017h, 000h
        popad
loc_10_00F8:
        test   byte ptr [eax + 3], 2
        jne    SHORT loc_10_0102
        dec    byte ptr [ecx + ebx + 18h]
loc_10_0102:
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
loc_10_010E:
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        cmp    edx, eax
        jne    SHORT loc_10_00E1
loc_10_0118:
        db  0E8h, 023h, 006h, 0F6h, 0FFh
        xor    edx, edx
        db  0E8h, 03Dh, 005h, 0F6h, 0FFh
        popad
loc_10_0125:
        db  0FFh, 025h, 0B8h, 00Bh, 000h, 000h
        db  000h
loc_10_012C:
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_10_014F
        mov    eax, dword ptr [ebp + 1ch]
        cmp    ax, 4b00h
        db  00Fh, 084h, 033h, 00Eh, 0F6h, 0FFh
        cmp    ah, 2bh
        je     SHORT loc_10_0149
        cmp    ah, 2dh
loc_10_0149:
        db  00Fh, 084h, 0EFh, 00Eh, 0F6h, 0FFh
loc_10_014F:
        stc
        ret
_PTEXT ends
        end
