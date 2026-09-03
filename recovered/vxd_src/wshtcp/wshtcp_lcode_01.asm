; wshtcp_lcode_01.asm -- WSHTCP object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        db  053h, 056h, 057h, 08Bh, 054h, 024h, 010h, 08Bh, 042h, 04Ch, 083h, 0F8h, 00Ah, 075h, 007h, 0B8h
        db  042h, 027h, 000h, 000h, 0EBh, 04Ah, 083h, 0F8h, 006h, 074h, 00Ch, 083h, 0F8h, 007h, 074h, 007h
        db  0B8h, 049h, 027h, 000h, 000h, 0EBh, 039h, 08Bh, 05Ch, 024h, 018h, 08Bh, 042h, 024h, 039h, 003h
        db  073h, 007h, 0B8h, 01Eh, 027h, 000h, 000h, 0EBh, 027h, 08Bh, 07Ch, 024h, 014h, 00Bh, 0FFh, 075h
        db  007h, 0B8h, 01Eh, 027h, 000h, 000h, 0EBh, 018h, 08Bh, 072h, 018h, 08Bh, 0C8h, 0C1h, 0E9h, 002h
        db  0F3h, 0A5h, 08Bh, 0C8h, 083h, 0E1h, 003h, 0F3h, 0A4h, 08Bh, 042h, 024h, 089h, 003h, 02Bh, 0C0h
        db  05Fh, 05Eh, 05Bh, 0C3h
WSHTCP_Control:
        cmp    eax, 1
        db  00Fh, 084h, 0DEh, 0FFh, 0FEh, 0FFh
        cmp    eax, 1bh
        db  00Fh, 084h, 0C5h, 0FFh, 0FEh, 0FFh
        clc
        ret
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 008h, 000h, 088h, 004h
        add    esp, 4
        leave
        ret
        db  0CDh, 020h, 005h, 080h, 089h, 004h, 000h
sub_1_0090:
        db  0CDh, 020h, 00Dh, 080h, 040h, 000h
sub_1_0096:
        cmp    dword ptr [esp + 4], 0
        je     SHORT loc_1_00A3
        db  0CDh, 020h, 00Eh, 080h, 040h, 000h
loc_1_00A3:
        ret
        db  0CDh, 020h, 011h, 080h, 040h, 000h, 000h, 000h
loc_1_00AC:
        lock add dword ptr [eax], eax
        db  000h, 05Fh, 004h
        add    byte ptr [eax], al
        cdq
        add    al, byte ptr [eax]
        db  000h, 089h, 004h, 000h, 000h, 0E0h
        db  005h, 000h, 000h, 0D1h, 004h
        add    byte ptr [eax], al
        sbb    al, 7
        add    byte ptr [eax], al
        out    dx, eax
        or     al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 000h, 000h, 000h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 057h, 053h, 048h, 054h, 043h
        db  050h, 020h, 020h, 000h, 000h, 000h, 080h, 064h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h
        db  000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h
        db  000h, 000h, 000h, 000h
loc_1_0125:
        and    dword ptr [ecx], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebp + 53h], cl
        push   esp
        inc    ebx
        push   eax
        add    byte ptr [esi + 54h], dl
        inc    esp
        dec    ecx
        xor    edi, dword ptr cs:[eax]
        add    byte ptr ss:[eax], al
loc_1_013C:
        push   edi
        push   ebx
        dec    eax
        push   esp
        inc    ebx
        push   eax
        add    byte ptr [eax], al
loc_1_0144:
        dec    ebp
        push   ebx
        push   esp
        inc    ebx
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [esi], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [esi], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], dl
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], dl
        add    byte ptr [eax], al
        add    byte ptr [ebx + eax*2 + 50h], dl
        das
        dec    ecx
        push   eax
        add    byte ptr [eax], al
loc_1_01CC:
        push   ebp
        inc    esp
        push   eax
        das
        dec    ecx
        push   eax
        add    byte ptr [eax], al
loc_1_01D4:
        push   esp
        inc    ebx
        push   eax
        das
        dec    ecx
        push   eax
        add    byte ptr [eax], al
loc_1_01DC:
        push   ebp
        inc    esp
        push   eax
        das
        dec    ecx
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_01F0:
        push   edi
        mov    edi, dword ptr [esp + 8]
        cmp    word ptr [edi], 2
        je     SHORT loc_1_0205
        mov    eax, 273fh
        jmp    NEAR PTR loc_1_0297
loc_1_0205:
        cmp    dword ptr [esp + 0ch], 10h
        jae    SHORT loc_1_0216
        mov    eax, 271eh
        jmp    NEAR PTR loc_1_0297
loc_1_0216:
        mov    eax, dword ptr [edi + 4]
        or     eax, eax
        jne    SHORT loc_1_0229
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [ecx], 1
        jmp    SHORT loc_1_0253
loc_1_0229:
        cmp    eax, -1
        jne    SHORT loc_1_023A
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [ecx], 2
        jmp    SHORT loc_1_0253
loc_1_023A:
        cmp    eax, 7f000001h
        mov    ecx, dword ptr [esp + 10h]
        jne    SHORT loc_1_024D
        mov    dword ptr [ecx], 3
        jmp    SHORT loc_1_0253
loc_1_024D:
        mov    dword ptr [ecx], 0
loc_1_0253:
        mov    ax, word ptr [edi + 2]
        or     ax, ax
        jne    SHORT loc_1_0265
        mov    dword ptr [ecx + 4], 1
        jmp    SHORT loc_1_0290
loc_1_0265:
        mov    dx, ax
        and    edx, 0ffh
        shr    ax, 8
        shl    edx, 8
        movzx  eax, ax
        add    edx, eax
        cmp    edx, 7d0h
        mov    dword ptr [ecx + 4], 2
        jl     SHORT loc_1_0290
        mov    dword ptr [ecx + 4], 0
loc_1_0290:
        add    edi, 8
        sub    eax, eax
        stosd
        stosd
loc_1_0297:
        pop    edi
        ret
loc_1_0299:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 24h]
        cmp    eax, 0fffeh
        mov    ecx, dword ptr [esp + 28h]
        jne    SHORT loc_1_02E4
        cmp    ecx, 1
        jne    SHORT loc_1_02E4
        mov    ebp, dword ptr [esp + 2ch]
        or     ebp, ebp
        mov    edx, dword ptr [esp + 30h]
        je     SHORT loc_1_02D9
        cmp    dword ptr [edx], 20h
        jae    SHORT loc_1_02CC
        mov    eax, 271eh
        jmp    NEAR PTR loc_1_045A
loc_1_02CC:
        mov    edi, ebp
        mov    esi, dword ptr [esp + 14h]
        mov    ecx, 8
        rep movsd
loc_1_02D9:
        mov    dword ptr [edx], 20h
        jmp    NEAR PTR loc_1_0458
loc_1_02E4:
        cmp    eax, 0ffffh
        je     SHORT loc_1_02FE
        cmp    eax, 6
        je     SHORT loc_1_02FE
        or     eax, eax
        je     SHORT loc_1_02FE
        mov    eax, 2726h
        jmp    NEAR PTR loc_1_045A
loc_1_02FE:
        mov    edx, dword ptr [esp + 30h]
        mov    ebx, dword ptr [edx]
        cmp    ebx, 4
        jae    SHORT loc_1_0313
        mov    eax, 271eh
        jmp    NEAR PTR loc_1_045A
loc_1_0313:
        cmp    eax, 6
        jne    SHORT loc_1_037F
        mov    esi, dword ptr [esp + 14h]
        cmp    dword ptr [esi + 4], 2
        jne    SHORT loc_1_032C
        mov    eax, 273ah
        jmp    NEAR PTR loc_1_045A
loc_1_032C:
        cmp    ecx, 1
        je     SHORT loc_1_0343
        cmp    ecx, 7000h
        je     SHORT loc_1_0361
        mov    eax, 2726h
        jmp    NEAR PTR loc_1_045A
loc_1_0343:
        mov    ebp, dword ptr [esp + 2ch]
        mov    edi, ebp
        sub    eax, eax
        mov    ecx, ebx
        shr    ecx, 2
        rep stosd
        mov    ecx, ebx
        and    ecx, 3
        rep stosb
        mov    al, byte ptr [esi + 1bh]
        jmp    NEAR PTR loc_1_044F
loc_1_0361:
        mov    ebp, dword ptr [esp + 2ch]
        mov    edi, ebp
        sub    eax, eax
        mov    ecx, ebx
        shr    ecx, 2
        rep stosd
        mov    ecx, ebx
        and    ecx, 3
        rep stosb
        mov    al, byte ptr [esi + 1ch]
        jmp    NEAR PTR loc_1_044F
loc_1_037F:
        or     eax, eax
        jne    SHORT loc_1_03F5
        mov    esi, dword ptr [esp + 14h]
        cmp    dword ptr [esi + 8], 6
        jne    SHORT loc_1_0397
        mov    eax, 273ah
        jmp    NEAR PTR loc_1_045A
loc_1_0397:
        cmp    ecx, 2
        je     SHORT loc_1_03B0
        cmp    ecx, 3
        je     SHORT loc_1_03BF
        cmp    ecx, 4
        je     SHORT loc_1_03DA
        mov    eax, 273ah
        jmp    NEAR PTR loc_1_045A
loc_1_03B0:
        mov    ebp, dword ptr [esp + 2ch]
        mov    eax, dword ptr [esi + 14h]
        mov    dword ptr [ebp], eax
        jmp    NEAR PTR loc_1_0452
loc_1_03BF:
        mov    ebp, dword ptr [esp + 2ch]
        mov    edi, ebp
        sub    eax, eax
        mov    ecx, ebx
        shr    ecx, 2
        rep stosd
        mov    ecx, ebx
        and    ecx, 3
        rep stosb
        mov    al, byte ptr [esi + 10h]
        jmp    SHORT loc_1_044F
loc_1_03DA:
        mov    ebp, dword ptr [esp + 2ch]
        mov    edi, ebp
        sub    eax, eax
        mov    ecx, ebx
        shr    ecx, 2
        rep stosd
        mov    ecx, ebx
        and    ecx, 3
        rep stosb
        mov    al, byte ptr [esi + 18h]
        jmp    SHORT loc_1_044F
loc_1_03F5:
        cmp    ecx, 8
        je     SHORT loc_1_0406
        cmp    ecx, 10h
        je     SHORT loc_1_0432
        mov    eax, 273ah
        jmp    SHORT loc_1_045A
loc_1_0406:
        mov    esi, dword ptr [esp + 14h]
        cmp    dword ptr [esi + 4], 2
        jne    SHORT loc_1_0417
        mov    eax, 273ah
        jmp    SHORT loc_1_045A
loc_1_0417:
        mov    ebp, dword ptr [esp + 2ch]
        mov    edi, ebp
        sub    eax, eax
        mov    ecx, ebx
        shr    ecx, 2
        rep stosd
        mov    ecx, ebx
        and    ecx, 3
        rep stosb
        mov    al, byte ptr [esi + 19h]
        jmp    SHORT loc_1_044F
loc_1_0432:
        mov    ebp, dword ptr [esp + 2ch]
        mov    edi, ebp
        sub    eax, eax
        mov    ecx, ebx
        shr    ecx, 2
        rep stosd
        mov    ecx, ebx
        and    ecx, 3
        rep stosb
        mov    esi, dword ptr [esp + 14h]
        mov    al, byte ptr [esi + 1ah]
loc_1_044F:
        mov    byte ptr [ebp], al
loc_1_0452:
        mov    dword ptr [edx], 4
loc_1_0458:
        sub    eax, eax
loc_1_045A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_045F:
        push   edi
        mov    eax, dword ptr [esp + 10h]
        cmp    dword ptr [eax], 10h
        jae    SHORT loc_1_0470
        mov    eax, 271eh
        jmp    SHORT loc_1_0487
loc_1_0470:
        mov    dword ptr [eax], 10h
        mov    ecx, dword ptr [esp + 0ch]
        mov    edi, ecx
        sub    eax, eax
        stosd
        stosd
        stosd
        stosd
        mov    word ptr [ecx], 2
loc_1_0487:
        pop    edi
        ret
loc_1_0489:
        push   esi
        push   edi
        cmp    dword ptr [esp + 10h], 80h
        mov    eax, 80h
        jb     SHORT loc_1_04CE
        mov    eax, dword ptr [esp + 0ch]
        db  0BEh, 04Ch, 001h, 000h, 000h
        mov    dword ptr [eax], 0ah
        mov    dword ptr [eax + 4], 3
        lea    edi, [eax + 8]
        mov    ecx, 0fh
        rep movsd
        db  0BEh, 088h, 001h, 000h, 000h
        lea    edi, [eax + 44h]
        mov    ecx, 0fh
        rep movsd
        mov    eax, 80h
loc_1_04CE:
        pop    edi
        pop    esi
        ret
loc_1_04D1:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 1ch]
        mov    ebp, dword ptr [esp + 18h]
        push   dword ptr [ecx]
        mov    ebx, dword ptr [esp + 18h]
        push   dword ptr [ebp]
        push   dword ptr [ebx]
        push   5
        db  068h, 04Ch, 001h, 000h, 000h
        call   sub_1_0E19
        add    esp, 14h
        or     al, al
        je     SHORT loc_1_051C
        db  0A1h, 04Ch, 001h, 000h, 000h
        mov    dword ptr [ebx], eax
        db  0A1h, 050h, 001h, 000h, 000h
        mov    dword ptr [ebp], eax
        db  0BEh, 0C4h, 001h, 000h, 000h
        mov    ecx, dword ptr [esp + 1ch]
        db  0A1h, 054h, 001h, 000h, 000h
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_1_055D
loc_1_051C:
        mov    ecx, dword ptr [esp + 1ch]
        push   dword ptr [ecx]
        push   dword ptr [ebp]
        push   dword ptr [ebx]
        push   5
        db  068h, 088h, 001h, 000h, 000h
        call   sub_1_0E19
        add    esp, 14h
        or     al, al
        je     NEAR PTR loc_1_05D6
        db  0A1h, 088h, 001h, 000h, 000h
        mov    dword ptr [ebx], eax
        db  0A1h, 08Ch, 001h, 000h, 000h
        mov    dword ptr [ebp], eax
        db  0BEh, 0CCh, 001h, 000h, 000h
        mov    ecx, dword ptr [esp + 1ch]
        db  0A1h, 090h, 001h, 000h, 000h
        mov    dword ptr [ecx], eax
loc_1_055D:
        mov    edi, dword ptr [esp + 20h]
        movsd
        movsw
        movsb
        push   20h
        call   sub_1_0090
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_057A
        mov    eax, 2747h
        jmp    SHORT loc_1_05DB
loc_1_057A:
        mov    ecx, dword ptr [ebx]
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [ebp]
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [esp + 1ch]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [eax + 8], ecx
        mov    dword ptr [eax + 0ch], 2000h
        mov    dword ptr [eax + 10h], 1
        mov    dword ptr [eax + 14h], 0
        mov    byte ptr [eax + 18h], 1
        mov    byte ptr [eax + 19h], 0
        mov    byte ptr [eax + 1ah], 0
        mov    byte ptr [eax + 1bh], 0
        mov    byte ptr [eax + 1ch], 1
        mov    byte ptr [eax + 1dh], 0
        mov    byte ptr [eax + 1eh], 0
        mov    ecx, dword ptr [esp + 28h]
        mov    byte ptr [eax + 1fh], 0
        mov    dword ptr [ecx], 184h
        mov    ecx, dword ptr [esp + 24h]
        mov    dword ptr [ecx], eax
        sub    eax, eax
        jmp    SHORT loc_1_05DB
loc_1_05D6:
        mov    eax, 2726h
loc_1_05DB:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_05E0:
        sub    esp, 1ch
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 38h]
        cmp    eax, 4
        jne    NEAR PTR loc_1_06CE
        mov    dword ptr [esp + 8], 1
        mov    esi, dword ptr [esp + 28h]
        mov    dword ptr [esp + 10h], 0
        cmp    byte ptr [esi + 19h], 0
        je     SHORT loc_1_063C
        push   0
        lea    eax, [esp + 0ch]
        push   4
        push   eax
        push   2
        push   300h
        push   200h
        mov    edi, dword ptr [esp + 4ch]
        push   400h
        push   edi
        call   sub_1_0E56
        add    esp, 20h
        or     eax, eax
        je     SHORT loc_1_0640
        jmp    NEAR PTR loc_1_0716
loc_1_063C:
        mov    edi, dword ptr [esp + 34h]
loc_1_0640:
        cmp    byte ptr [esi + 1bh], 0
        je     SHORT loc_1_0671
        push   0
        lea    eax, [esp + 0ch]
        push   4
        push   eax
        push   1
        push   300h
        push   200h
        push   400h
        push   edi
        call   sub_1_0E56
        add    esp, 20h
        or     eax, eax
        jne    NEAR PTR loc_1_0716
loc_1_0671:
        lea    eax, [esi + 0ch]
        cmp    dword ptr [eax], 2000h
        je     SHORT loc_1_069F
        push   1
        push   4
        push   eax
        push   6
        push   300h
        push   200h
        push   400h
        push   edi
        call   sub_1_0E56
        add    esp, 20h
        or     eax, eax
        jne    SHORT loc_1_0716
loc_1_069F:
        cmp    byte ptr [esi + 1ch], 0
        jne    SHORT loc_1_070D
        push   0
        lea    eax, [esp + 14h]
        push   4
        push   eax
        push   4
        push   300h
        push   200h
        push   400h
        push   edi
        call   sub_1_0E56
        add    esp, 20h
        or     eax, eax
        je     SHORT loc_1_070D
        jmp    SHORT loc_1_0716
loc_1_06CE:
        cmp    eax, 80h
        jne    SHORT loc_1_06E3
        push   dword ptr [esp + 28h]
        call   sub_1_0096
        add    esp, 4
        jmp    SHORT loc_1_070D
loc_1_06E3:
        cmp    eax, 100h
        jne    SHORT loc_1_0711
        mov    dword ptr [esp + 0ch], 10h
        lea    eax, [esp + 0ch]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        push   dword ptr [esp + 34h]
        call   0
        add    esp, 0ch
        cmp    eax, -1
        je     SHORT loc_1_0716
loc_1_070D:
        sub    eax, eax
        jmp    SHORT loc_1_0716
loc_1_0711:
        mov    eax, 2726h
loc_1_0716:
        pop    edi
        pop    esi
        add    esp, 1ch
        ret
sub_1_071C:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 30h]
        cmp    eax, 0fffeh
        mov    edi, dword ptr [esp + 34h]
        jne    NEAR PTR loc_1_088C
        cmp    edi, 1
        jne    NEAR PTR loc_1_088C
        cmp    dword ptr [esp + 3ch], 20h
        jae    SHORT loc_1_0750
        mov    eax, 2726h
        jmp    NEAR PTR loc_1_0CE7
loc_1_0750:
        mov    esi, dword ptr [esp + 20h]
        or     esi, esi
        jne    SHORT loc_1_0786
        push   20h
        call   sub_1_0090
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_0770
        mov    eax, 2747h
        jmp    NEAR PTR loc_1_0CE7
loc_1_0770:
        mov    edi, eax
        mov    ebx, dword ptr [esp + 38h]
        mov    esi, ebx
        mov    ecx, 8
        rep movsd
        mov    dword ptr [ebx], eax
        jmp    NEAR PTR loc_1_0CE5
loc_1_0786:
        mov    dword ptr [esp + 14h], 1
        mov    ebx, dword ptr [esp + 38h]
        mov    dword ptr [esp + 18h], 0
        cmp    byte ptr [ebx + 19h], 0
        je     SHORT loc_1_07CE
        lea    eax, [esp + 14h]
        push   4
        push   eax
        push   8
        mov    edi, dword ptr [esp + 38h]
        push   0ffffh
        mov    ebp, dword ptr [esp + 38h]
        push   edi
        push   ebp
        push   dword ptr [esp + 3ch]
        push   esi
        call   sub_1_071C
        add    esp, 20h
        or     eax, eax
        je     SHORT loc_1_07D6
        jmp    NEAR PTR loc_1_0CE7
loc_1_07CE:
        mov    ebp, dword ptr [esp + 28h]
        mov    edi, dword ptr [esp + 2ch]
loc_1_07D6:
        cmp    byte ptr [ebx + 1ah], 0
        je     SHORT loc_1_0801
        lea    eax, [esp + 14h]
        push   4
        push   eax
        push   10h
        push   0ffffh
        push   edi
        push   ebp
        push   dword ptr [esp + 3ch]
        push   esi
        call   sub_1_071C
        add    esp, 20h
        or     eax, eax
        jne    NEAR PTR loc_1_0CE7
loc_1_0801:
        cmp    byte ptr [ebx + 1bh], 0
        je     SHORT loc_1_0829
        lea    eax, [esp + 14h]
        push   4
        push   eax
        push   1
        push   6
        push   edi
        push   ebp
        push   dword ptr [esp + 3ch]
        push   esi
        call   sub_1_071C
        add    esp, 20h
        or     eax, eax
        jne    NEAR PTR loc_1_0CE7
loc_1_0829:
        lea    eax, [ebx + 0ch]
        cmp    dword ptr [eax], 2000h
        je     SHORT loc_1_0858
        push   4
        push   eax
        push   1002h
        push   0ffffh
        push   edi
        push   ebp
        push   dword ptr [esp + 3ch]
        push   esi
        call   sub_1_071C
        add    esp, 20h
        or     eax, eax
        jne    NEAR PTR loc_1_0CE7
loc_1_0858:
        cmp    byte ptr [ebx + 1ch], 0
        jne    NEAR PTR loc_1_0CE5
        lea    eax, [esp + 18h]
        push   4
        push   eax
        push   7000h
        push   6
        push   edi
        push   ebp
        push   dword ptr [esp + 3ch]
        push   esi
        call   sub_1_071C
        add    esp, 20h
        or     eax, eax
        jne    NEAR PTR loc_1_0CE7
        jmp    NEAR PTR loc_1_0CE5
loc_1_088C:
        cmp    eax, 0ffffh
        je     SHORT loc_1_08A6
        cmp    eax, 6
        je     SHORT loc_1_08A6
        or     eax, eax
        je     SHORT loc_1_08A6
        mov    eax, 2726h
        jmp    NEAR PTR loc_1_0CE7
loc_1_08A6:
        mov    edx, dword ptr [esp + 3ch]
        cmp    edx, 4
        jae    SHORT loc_1_08B9
        mov    eax, 271eh
        jmp    NEAR PTR loc_1_0CE7
loc_1_08B9:
        mov    ebx, dword ptr [esp + 38h]
        cmp    eax, 6
        mov    ecx, dword ptr [ebx]
        mov    dword ptr [esp + 10h], ecx
        jne    NEAR PTR loc_1_0A54
        cmp    edi, 1
        jne    NEAR PTR loc_1_098A
        mov    esi, dword ptr [esp + 20h]
        cmp    dword ptr [esi + 4], 2
        jne    SHORT loc_1_08E9
        mov    eax, 273ah
        jmp    NEAR PTR loc_1_0CE7
loc_1_08E9:
        mov    al, byte ptr [esi + 1bh]
        or     al, al
        jne    SHORT loc_1_0943
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_093B
        mov    dword ptr [esp + 10h], 1
        mov    edi, dword ptr [esp + 2ch]
        or     edi, edi
        je     SHORT loc_1_0932
        push   1
        lea    eax, [esp + 14h]
        push   4
        push   eax
        push   1
        push   300h
        push   200h
        push   400h
        push   edi
        call   sub_1_0E56
        add    esp, 20h
        or     eax, eax
        jne    NEAR PTR loc_1_0CE7
loc_1_0932:
        mov    byte ptr [esi + 1bh], 1
        jmp    NEAR PTR loc_1_0CE5
loc_1_093B:
        or     al, al
        je     NEAR PTR loc_1_0CE5
loc_1_0943:
        cmp    dword ptr [esp + 10h], 0
        jne    NEAR PTR loc_1_0CE5
        mov    edi, dword ptr [esp + 2ch]
        or     edi, edi
        je     SHORT loc_1_0981
        push   1
        lea    eax, [esp + 14h]
        push   4
        push   eax
        push   1
        push   300h
        push   200h
        push   400h
        push   edi
        call   sub_1_0E56
        add    esp, 20h
        or     eax, eax
        jne    NEAR PTR loc_1_0CE7
loc_1_0981:
        mov    byte ptr [esi + 1bh], 0
        jmp    NEAR PTR loc_1_0CE5
loc_1_098A:
        cmp    eax, 6
        jne    NEAR PTR loc_1_0A54
        cmp    edi, 7000h
        jne    NEAR PTR loc_1_0A54
        mov    esi, dword ptr [esp + 20h]
        cmp    dword ptr [esi + 4], 2
        jne    SHORT loc_1_09B3
        mov    eax, 273ah
        jmp    NEAR PTR loc_1_0CE7
loc_1_09B3:
        mov    al, byte ptr [esi + 1ch]
        or     al, al
        jne    SHORT loc_1_0A0D
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_0A05
        mov    dword ptr [esp + 10h], 1
        mov    edi, dword ptr [esp + 2ch]
        or     edi, edi
        je     SHORT loc_1_09FC
        push   1
        lea    eax, [esp + 14h]
        push   4
        push   eax
        push   4
        push   300h
        push   200h
        push   400h
        push   edi
        call   sub_1_0E56
        add    esp, 20h
        or     eax, eax
        jne    NEAR PTR loc_1_0CE7
loc_1_09FC:
        mov    byte ptr [esi + 1ch], 1
        jmp    NEAR PTR loc_1_0CE5
loc_1_0A05:
        or     al, al
        je     NEAR PTR loc_1_0CE5
loc_1_0A0D:
        cmp    dword ptr [esp + 10h], 0
        jne    NEAR PTR loc_1_0CE5
        mov    edi, dword ptr [esp + 2ch]
        or     edi, edi
        je     SHORT loc_1_0A4B
        push   1
        lea    eax, [esp + 14h]
        push   4
        push   eax
        push   4
        push   300h
        push   200h
        push   400h
        push   edi
        call   sub_1_0E56
        add    esp, 20h
        or     eax, eax
        jne    NEAR PTR loc_1_0CE7
loc_1_0A4B:
        mov    byte ptr [esi + 1ch], 0
        jmp    NEAR PTR loc_1_0CE5
loc_1_0A54:
        or     eax, eax
        jne    NEAR PTR loc_1_0BAF
        mov    esi, dword ptr [esp + 20h]
        cmp    dword ptr [esi + 8], 6
        jne    SHORT loc_1_0A70
        mov    eax, 273ah
        jmp    NEAR PTR loc_1_0CE7
loc_1_0A70:
        cmp    edi, 2
        je     SHORT loc_1_0A9B
        cmp    edi, 3
        je     SHORT loc_1_0AE4
        cmp    edi, 4
        je     NEAR PTR loc_1_0B48
        cmp    edi, 5
        jl     SHORT loc_1_0A91
        cmp    edi, 6
        jle    NEAR PTR loc_1_0B52
loc_1_0A91:
        mov    eax, 273ah
        jmp    NEAR PTR loc_1_0CE7
loc_1_0A9B:
        mov    ebp, dword ptr [esp + 28h]
        or     ebp, ebp
        je     SHORT loc_1_0ADA
        push   1
        lea    eax, [esp + 14h]
        push   4
        push   eax
        push   3
        push   200h
        push   200h
        push   401h
        push   ebp
        call   sub_1_0E56
        add    esp, 20h
        or     eax, eax
        jne    NEAR PTR loc_1_0CE7
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esi + 14h], eax
        jmp    NEAR PTR loc_1_0CE5
loc_1_0ADA:
        mov    eax, 2726h
        jmp    NEAR PTR loc_1_0CE7
loc_1_0AE4:
        cmp    dword ptr [esp + 10h], 0ffh
        jg     SHORT loc_1_0B3E
        cmp    dword ptr [esp + 10h], 0
        jl     SHORT loc_1_0B3E
        mov    ebp, dword ptr [esp + 28h]
        or     ebp, ebp
        je     SHORT loc_1_0B34
        push   1
        lea    eax, [esp + 14h]
        push   4
        push   eax
        push   2
        push   200h
        push   200h
        push   401h
        push   ebp
        call   sub_1_0E56
        add    esp, 20h
        or     eax, eax
        jne    NEAR PTR loc_1_0CE7
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esi + 10h], eax
        jmp    NEAR PTR loc_1_0CE5
loc_1_0B34:
        mov    eax, 2726h
        jmp    NEAR PTR loc_1_0CE7
loc_1_0B3E:
        mov    eax, 2726h
        jmp    NEAR PTR loc_1_0CE7
loc_1_0B48:
        mov    eax, 273ah
        jmp    NEAR PTR loc_1_0CE7
loc_1_0B52:
        cmp    edx, 8
        jae    SHORT loc_1_0B61
        mov    eax, 2726h
        jmp    NEAR PTR loc_1_0CE7
loc_1_0B61:
        mov    ebp, dword ptr [esp + 28h]
        or     ebp, ebp
        je     SHORT loc_1_0BA5
        push   1
        push   edx
        push   ebx
        sub    edi, 5
        cmp    edi, 1
        sbb    eax, eax
        add    eax, 7
        push   eax
        push   200h
        push   200h
        push   401h
        push   ebp
        call   sub_1_0E56
        add    esp, 20h
        or     eax, eax
        jne    NEAR PTR loc_1_0CE7
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esi + 14h], eax
        jmp    NEAR PTR loc_1_0CE5
loc_1_0BA5:
        mov    eax, 2726h
        jmp    NEAR PTR loc_1_0CE7
loc_1_0BAF:
        cmp    edi, 8
        je     SHORT loc_1_0BD3
        cmp    edi, 10h
        je     NEAR PTR loc_1_0C81
        cmp    edi, 1002h
        je     NEAR PTR loc_1_0C9C
        mov    eax, 273ah
        jmp    NEAR PTR loc_1_0CE7
loc_1_0BD3:
        mov    esi, dword ptr [esp + 20h]
        cmp    dword ptr [esi + 4], 2
        jne    SHORT loc_1_0BE7
        mov    eax, 273ah
        jmp    NEAR PTR loc_1_0CE7
loc_1_0BE7:
        mov    al, byte ptr [esi + 19h]
        or     al, al
        jne    SHORT loc_1_0C41
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_0C39
        mov    dword ptr [esp + 10h], 1
        mov    edi, dword ptr [esp + 2ch]
        or     edi, edi
        je     SHORT loc_1_0C30
        push   1
        lea    eax, [esp + 14h]
        push   4
        push   eax
        push   2
        push   300h
        push   200h
        push   400h
        push   edi
        call   sub_1_0E56
        add    esp, 20h
        or     eax, eax
        jne    NEAR PTR loc_1_0CE7
loc_1_0C30:
        mov    byte ptr [esi + 19h], 1
        jmp    NEAR PTR loc_1_0CE5
loc_1_0C39:
        or     al, al
        je     NEAR PTR loc_1_0CE5
loc_1_0C41:
        cmp    dword ptr [esp + 10h], 0
        jne    NEAR PTR loc_1_0CE5
        mov    edi, dword ptr [esp + 2ch]
        or     edi, edi
        je     SHORT loc_1_0C7B
        push   1
        lea    eax, [esp + 14h]
        push   4
        push   eax
        push   2
        push   300h
        push   200h
        push   400h
        push   edi
        call   sub_1_0E56
        add    esp, 20h
        or     eax, eax
        jne    SHORT loc_1_0CE7
loc_1_0C7B:
        mov    byte ptr [esi + 19h], 0
        jmp    SHORT loc_1_0CE5
loc_1_0C81:
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_0C92
        mov    esi, dword ptr [esp + 20h]
        mov    byte ptr [esi + 1ah], 1
        jmp    SHORT loc_1_0CE5
loc_1_0C92:
        mov    esi, dword ptr [esp + 20h]
        mov    byte ptr [esi + 1ah], 0
        jmp    SHORT loc_1_0CE5
loc_1_0C9C:
        mov    esi, dword ptr [esp + 20h]
        mov    eax, dword ptr [esp + 10h]
        cmp    dword ptr [esi + 0ch], eax
        je     SHORT loc_1_0CE5
        cmp    dword ptr [esi + 4], 2
        je     SHORT loc_1_0CE5
        mov    edi, dword ptr [esp + 2ch]
        or     edi, edi
        je     SHORT loc_1_0CDE
        push   1
        lea    eax, [esp + 14h]
        push   4
        push   eax
        push   6
        push   300h
        push   200h
        push   400h
        push   edi
        call   sub_1_0E56
        add    esp, 20h
        or     eax, eax
        jne    SHORT loc_1_0CE7
loc_1_0CDE:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esi + 0ch], eax
loc_1_0CE5:
        sub    eax, eax
loc_1_0CE7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_1_0CEF:
        push   ebx
        push   esi
        push   edi
        sub    edx, edx
        sub    ebx, ebx
        mov    ecx, dword ptr [esp + 10h]
        cmp    ecx, edx
        je     SHORT loc_1_0D22
        cmp    dword ptr [ecx], edx
        je     SHORT loc_1_0D29
loc_1_0D02:
        mov    eax, dword ptr [ecx]
        cmp    eax, 6
        jne    SHORT loc_1_0D0E
        mov    edx, 1
loc_1_0D0E:
        cmp    eax, 11h
        jne    SHORT loc_1_0D18
        mov    ebx, 1
loc_1_0D18:
        add    ecx, 4
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_1_0D02
        jmp    SHORT loc_1_0D29
loc_1_0D22:
        mov    edx, 1
        mov    ebx, edx
loc_1_0D29:
        or     edx, edx
        jne    SHORT loc_1_0D42
        or     ebx, ebx
        jne    SHORT loc_1_0D42
        mov    ecx, dword ptr [esp + 1ch]
        sub    eax, eax
        mov    dword ptr [ecx], 0
        jmp    NEAR PTR loc_1_0E15
loc_1_0D42:
        mov    ecx, dword ptr [esp + 1ch]
        mov    esi, dword ptr [ecx]
        cmp    esi, 4eh
        jae    SHORT loc_1_0D5D
        mov    dword ptr [ecx], 4eh
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_0E15
loc_1_0D5D:
        or     edx, edx
        mov    eax, dword ptr [esp + 18h]
        je     SHORT loc_1_0DB3
        mov    dword ptr [eax], 1066h
        mov    dword ptr [eax + 4], 2
        mov    dword ptr [eax + 8], 10h
        mov    dword ptr [eax + 0ch], 10h
        lea    edi, [esi + eax - 7]
        mov    dword ptr [eax + 10h], 1
        mov    dword ptr [eax + 14h], 6
        mov    dword ptr [eax + 18h], 0
        mov    dword ptr [eax + 1ch], edi
        db  0BEh, 0D4h, 001h, 000h, 000h
        movsd
        movsw
        movsb
        lea    edi, [eax + 20h]
        mov    eax, dword ptr [eax + 1ch]
        sub    eax, 7
        mov    dword ptr [edi + 1ch], eax
        jmp    SHORT loc_1_0DBD
loc_1_0DB3:
        mov    edi, eax
        add    esi, eax
        sub    esi, 7
        mov    dword ptr [eax + 1ch], esi
loc_1_0DBD:
        or     ebx, ebx
        je     SHORT loc_1_0DFD
        mov    dword ptr [edi], 1609h
        mov    dword ptr [edi + 4], 2
        mov    dword ptr [edi + 8], 10h
        mov    dword ptr [edi + 0ch], 10h
        db  0BEh, 0DCh, 001h, 000h, 000h
        mov    dword ptr [edi + 10h], 2
        mov    dword ptr [edi + 14h], 11h
        mov    dword ptr [edi + 18h], 0ffbbh
        mov    edi, dword ptr [edi + 1ch]
        movsd
        movsw
        movsb
loc_1_0DFD:
        mov    dword ptr [ecx], 4eh
        or     edx, edx
        je     SHORT loc_1_0E10
        or     ebx, ebx
        mov    eax, 2
        jne    SHORT loc_1_0E15
loc_1_0E10:
        mov    eax, 1
loc_1_0E15:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_0E19:
        push   ebx
        push   esi
        push   edi
        sub    ebx, ebx
        mov    ecx, dword ptr [esp + 14h]
        cmp    ecx, ebx
        je     SHORT loc_1_0E4C
        mov    eax, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 18h]
        mov    esi, dword ptr [esp + 1ch]
        mov    edi, dword ptr [esp + 20h]
loc_1_0E36:
        cmp    dword ptr [eax], edx
        jne    SHORT loc_1_0E44
        cmp    dword ptr [eax + 4], esi
        jne    SHORT loc_1_0E44
        cmp    dword ptr [eax + 8], edi
        je     SHORT loc_1_0E52
loc_1_0E44:
        add    eax, 0ch
        inc    ebx
        cmp    ebx, ecx
        jb     SHORT loc_1_0E36
loc_1_0E4C:
        sub    al, al
loc_1_0E4E:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_0E52:
        mov    al, 1
        jmp    SHORT loc_1_0E4E
sub_1_0E56:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 20h]
        add    eax, 21h
        push   eax
        call   sub_1_0090
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_0E7E
        mov    eax, 2747h
        jmp    NEAR PTR loc_1_0F7D
loc_1_0E7E:
        cmp    byte ptr [ebp + 24h], 0
        je     SHORT loc_1_0EB9
        mov    dword ptr [ebp - 14h], 0
        mov    ecx, dword ptr [ebp - 14h]
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        cmc
        sbb    ecx, ecx
        and    eax, ecx
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebx + 4], eax
        or     eax, eax
        jne    SHORT loc_1_0EB9
        push   ebx
        call   sub_1_0096
        add    esp, 4
        mov    eax, 2747h
        jmp    NEAR PTR loc_1_0F7D
loc_1_0EB9:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebx + 8], eax
        mov    dword ptr [ebx + 0ch], 0
        mov    eax, dword ptr [ebp + 10h]
        lea    edx, [ebx + 20h]
        mov    dword ptr [ebx + 10h], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ebx + 14h], eax
        mov    eax, dword ptr [ebp + 18h]
        mov    edi, edx
        mov    dword ptr [ebx + 18h], eax
        mov    eax, dword ptr [ebp + 20h]
        mov    esi, dword ptr [ebp + 1ch]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [ebx + 1ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 24h], eax
        lea    eax, [ebx + 8]
        db  0C7h, 045h, 0E0h, 084h, 00Fh, 000h, 000h
        mov    dword ptr [ebp - 1ch], ebx
        mov    dword ptr [ebp - 18h], 0
        push   dword ptr [ebx + 1ch]
        push   edx
        push   eax
        lea    eax, [ebp - 24h]
        push   eax
        db  0A1h, 025h, 001h, 000h, 000h
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4ch]
        add    esp, 10h
        mov    esi, eax
        cmp    dword ptr [ebx + 4], 0
        je     SHORT loc_1_0F61
        cmp    esi, 0ffh
        jne    SHORT loc_1_0F52
        mov    dword ptr [ebp - 8], 1
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        mov    esi, dword ptr [ebx]
loc_1_0F52:
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 026h, 000h, 001h, 000h
loc_1_0F61:
        cmp    esi, 0ffh
        je     SHORT loc_1_0F72
        push   ebx
        call   sub_1_0096
        add    esp, 4
loc_1_0F72:
        or     esi, esi
        mov    eax, 2747h
        jne    SHORT loc_1_0F7D
        sub    eax, eax
loc_1_0F7D:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_0F84:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 4]
        or     eax, eax
        je     SHORT loc_1_0FA7
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], edx
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 028h, 000h, 001h, 000h
        jmp    SHORT loc_1_0FB0
loc_1_0FA7:
        push   ecx
        call   sub_1_0096
        add    esp, 4
loc_1_0FB0:
        mov    esp, ebp
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h
_LTEXT ends
        end
