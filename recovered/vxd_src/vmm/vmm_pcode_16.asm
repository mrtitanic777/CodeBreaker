; vmm_pcode_16.asm -- VMM object 16 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  060h, 08Bh, 073h, 0D4h, 085h, 0F6h, 074h, 018h, 0CDh, 020h, 0A5h, 000h, 001h, 000h, 074h, 007h
        db  0E8h, 016h, 00Ch, 0F0h, 0FFh, 0EBh, 0F1h, 089h, 043h, 0D4h, 0CDh, 020h, 09Ch, 000h, 001h, 000h
        db  0CDh, 020h, 088h, 000h, 001h, 000h, 00Fh, 0B7h, 06Bh, 09Ah, 0EBh, 00Ah, 0FFh, 077h, 004h, 0E8h
        db  08Ah, 001h, 0F0h, 0FFh, 05Fh, 04Dh, 085h, 0EDh, 074h, 00Ch, 085h, 0FFh, 074h, 008h, 066h, 081h
        db  07Fh, 00Ah, 0B0h, 0CBh, 074h, 0E6h, 061h, 0C3h
sub_16_0048:
        pushad
        call   sub_16_0078
        mov    esi, dword ptr [ebx - 40h]
loc_16_0051:
        db  0CDh, 020h, 0A5h, 000h, 001h, 000h
        je     SHORT loc_16_0070
        movzx  edi, word ptr [eax + 4]
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        push   0
        push   edi
        db  0CDh, 020h, 077h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        jmp    SHORT loc_16_0051
loc_16_0070:
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
        popad
        ret
sub_16_0078:
        pushad
        mov    esi, dword ptr [ebx - 40h]
        call   sub_16_0083
        popad
        ret
sub_16_0083:
        db  0CDh, 020h, 0A5h, 000h, 001h, 000h
        je     SHORT loc_16_00A8
        test   word ptr [eax + 6], 1
        jne    SHORT loc_16_009B
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        jmp    SHORT sub_16_0083
loc_16_009B:
        push   eax
        call   sub_16_0083
        pop    eax
        db  0CDh, 020h, 09Eh, 000h, 001h, 000h
loc_16_00A8:
        ret
        db  000h, 000h, 000h
loc_16_00AC:
        db  0CDh, 020h, 00Dh, 001h, 001h, 000h
        clc
        ret
sub_16_00B4:
        mov    eax, 0ah
        db  0CDh, 020h, 00Dh, 001h, 001h, 000h
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        ret
sub_16_00C6:
        mov    ebx, edx
        pushad
        push   ecx
        mov    ecx, 5
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        pop    ecx
        db  0CDh, 020h, 02Bh, 000h, 001h, 000h
        call   0
        mov    eax, 0bh
        mov    edx, ecx
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        push   edi
        mov    edi, dword ptr [ebx - 20h]
        or     byte ptr [edi], 10h
        pop    edi
        or     byte ptr [ebx], 10h
        push   4
        push   dword ptr [ebx - 20h]
        db  0E8h, 002h, 08Eh, 0F0h, 0FFh
        popad
        db  0CDh, 020h, 020h, 000h, 001h, 000h
        clc
        ret
sub_16_010C:
        pushad
        call   sub_16_0158
        call   sub_16_0048
loc_16_0117:
        call   sub_16_0138
loc_16_011C:
        call   sub_16_0148
loc_16_0121:
        call   sub_16_0301
loc_16_0126:
        push   ebx
        call   sub_16_0240
loc_16_012C:
        popad
        stc
        ret
loc_16_012F:
        call   sub_16_010C
        clc
        ret
        db  000h, 000h
sub_16_0138:
        lea    edx, [ebx + 20h]
        db  0E8h, 019h, 0FFh, 0EFh, 0FFh
        db  0E9h, 007h, 0FFh, 0EFh, 0FFh
        db  000h, 000h, 000h
sub_16_0148:
        push   edi
        db  0CDh, 020h, 00Dh, 001h, 001h, 000h
        db  0E8h, 039h, 0FFh, 0EFh, 0FFh
        pop    edi
        ret
        db  000h, 000h
sub_16_0158:
        pushad
        pushfd
        cli
        push   edi
        db  0CDh, 020h, 00Dh, 001h, 001h, 000h
        db  0E8h, 071h, 0FFh, 0EFh, 0FFh
        pop    edi
        lea    edx, [ebx + 38h]
        db  0E9h, 06Eh, 0FFh, 0EFh, 0FFh
_GetFirstV86Page:
        db  08Bh, 005h, 06Ch, 0F6h, 000h, 000h
        ret
        db  000h
sub_16_0178:
        cmp    word ptr [ebx + 1ah], 0
        je     SHORT loc_16_01AE
        xor    eax, eax
        db  03Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        jne    SHORT loc_16_018C
        lldt   ax
loc_16_018C:
        xchg   word ptr [ebx + 1ah], ax
        push   20000000h
        push   eax
        db  0CDh, 020h, 077h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        xor    eax, eax
        xchg   dword ptr [ebx + 28h], eax
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_16_01AE:
        ret
loc_16_01AF:
        push   ebx
        db  08Bh, 01Dh, 010h, 0F7h, 000h, 000h
loc_16_01B6:
        mov    eax, dword ptr [esp + 8]
        add    eax, ebx
        push   eax
        push   dword ptr [ebx - 70h]
        db  0E8h, 08Bh, 012h, 0F0h, 0FFh
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        jne    SHORT loc_16_01B6
        pop    ebx
        ret
        db  000h, 000h, 000h
sub_16_01D8:
        mov    ecx, dword ptr [esp + 4]
        jecxz  loc_16_01E4
        push   ecx
        call   sub_16_020C
loc_16_01E4:
        db  0FFh, 00Dh, 078h, 003h, 000h, 000h
        jne    SHORT loc_16_0209
        push   0
        db  0FFh, 035h, 058h, 0F6h, 000h, 000h
        db  0CDh, 020h, 015h, 001h, 001h, 000h
        pop    ecx
        pop    ecx
        db  08Bh, 00Dh, 010h, 0F7h, 000h, 000h
        mov    dword ptr [ecx + 30h], 0
loc_16_0209:
        ret    4
sub_16_020C:
        pushad
        db  00Fh, 0B6h, 00Dh, 0F8h, 0F1h, 000h, 000h
        jecxz  loc_16_023A
        db  08Bh, 03Dh, 0E4h, 0F0h, 000h, 000h
        mov    ebx, dword ptr [esp + 24h]
loc_16_0220:
        cmp    dword ptr [edi + 10h], ebx
        jne    SHORT loc_16_0235
        pushad
        db  08Bh, 01Dh, 010h, 0F7h, 000h, 000h
        mov    ecx, dword ptr [edi + 14h]
        db  0E8h, 077h, 06Dh, 0F0h, 0FFh
        popad
loc_16_0235:
        add    edi, 18h
        loop   loc_16_0220
loc_16_023A:
        popad
        ret    4
        db  000h, 000h
sub_16_0240:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_16_01D8
        mov    ax, word ptr [esi + 1ah]
        or     ax, ax
        je     SHORT loc_16_0274
        push   20000000h
        movzx  eax, ax
        push   eax
        db  0FFh, 015h, 004h, 0D5h, 000h, 000h
        add    esp, 8
        push   0
        push   dword ptr [esi + 28h]
        db  0FFh, 015h, 07Ch, 0D4h, 000h, 000h
        add    esp, 8
loc_16_0274:
        push   dword ptr [esi + 0ch]
        db  0FFh, 035h, 064h, 0F6h, 000h, 000h
        db  0E8h, 069h, 035h, 0F0h, 0FFh
        push   10h
        push   dword ptr [esi + 4]
        db  0FFh, 015h, 07Ch, 0D4h, 000h, 000h
        add    esp, 8
        pop    esi
        ret    4
loc_16_0294:
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        db  00Fh, 084h, 0C2h, 008h, 0F0h, 0FFh
        call   sub_16_00B4
        db  08Bh, 015h, 098h, 0F6h, 000h, 000h
        db  0BEh, 0C4h, 0F6h, 000h, 000h
        cmp    dword ptr [esi + 0ch], 0
        je     SHORT loc_16_02C8
        cmp    edx, dword ptr [esi + 10h]
        jne    SHORT loc_16_02C8
        mov    dword ptr [esi + 0ch], 1
        db  0CDh, 020h, 020h, 000h, 001h, 000h
loc_16_02C8:
        mov    byte ptr [edx - 33h], 0
        mov    edx, ebx
        db  0BEh, 0FAh, 002h, 000h, 000h
        db  08Bh, 01Dh, 010h, 0F7h, 000h, 000h
        mov    eax, 10h
        mov    ecx, 3
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        mov    ebx, edx
        db  0CDh, 020h, 02Bh, 000h, 001h, 000h
        pushad
        xor    esi, esi
        db  0CDh, 020h, 0BEh, 000h, 001h, 000h
loc_16_02FA:
        xor    ecx, ecx
        jmp    NEAR PTR sub_16_00C6
sub_16_0301:
        pushad
        db  0CDh, 020h, 02Bh, 000h, 001h, 000h
        db  0CDh, 020h, 00Dh, 001h, 001h, 000h
        db  0E8h, 098h, 009h, 0F0h, 0FFh
        push   4
        push   edi
        db  0E8h, 0EAh, 08Bh, 0F0h, 0FFh
        push   edi
        db  0E8h, 0D4h, 07Dh, 0F0h, 0FFh
        mov    ecx, dword ptr [edi + 54h]
        db  0E8h, 08Bh, 001h, 0F0h, 0FFh
        db  08Bh, 00Dh, 014h, 0F7h, 000h, 000h
        cmp    ecx, ebx
        jne    SHORT loc_16_033E
        mov    ecx, dword ptr [ebx + 2ch]
        db  089h, 00Dh, 014h, 0F7h, 000h, 000h
        jmp    SHORT loc_16_034E
loc_16_033E:
        cmp    dword ptr [ecx + 2ch], ebx
        je     SHORT loc_16_0348
        mov    ecx, dword ptr [ecx + 2ch]
        jmp    SHORT loc_16_033E
loc_16_0348:
        mov    edx, dword ptr [ebx + 2ch]
        mov    dword ptr [ecx + 2ch], edx
loc_16_034E:
        popad
        ret
loc_16_0350:
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        jne    SHORT loc_16_038C
        pushad
        db  0BEh, 024h, 0F7h, 000h, 000h
        cmp    dword ptr [esi + 4], 0
        je     SHORT loc_16_038B
        mov    eax, 2710h
        mov    edx, esi
        db  0BEh, 08Eh, 003h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    esi, edx
loc_16_0378:
        cmp    dword ptr [esi + 4], 0
        jle    SHORT loc_16_038B
        push   1
        push   esi
        db  0CDh, 020h, 0FAh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        jmp    SHORT loc_16_0378
loc_16_038B:
        popad
loc_16_038C:
        clc
        ret
loc_16_038E:
        mov    esi, dword ptr [edx]
        db  0CDh, 020h, 05Bh, 001h, 001h, 000h
        xor    eax, eax
        mov    dword ptr [edx + 4], eax
        push   edx
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        pop    ecx
        ret
_PTEXT ends
        end
