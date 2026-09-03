; pe3ndis_lcode_01_02.asm -- PE3NDIS object 1 segment _LTEXT  (part 2/2)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_47F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    al, 73h
        mov    edx, dword ptr [esi]
        out    dx, al
        add    edx, 2
        mov    cl, byte ptr [esi + 8]
        mov    al, cl
        or     al, 8
        out    dx, al
        mov    al, cl
        out    dx, al
        mov    ecx, 1
loc_1_4812:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    SHORT loc_1_4812
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, byte ptr [esi + 9]
        db  00Ah, 005h, 064h, 05Bh, 000h, 000h
        out    dx, al
        mov    ecx, 1
loc_1_482D:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    SHORT loc_1_482D
        mov    dx, word ptr [esi]
        in     al, dx
        mov    byte ptr [ebp - 3], al
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, byte ptr [esi + 8]
        out    dx, al
        mov    al, byte ptr [ebp - 3]
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_4850:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, 1
        mov    edx, dword ptr [ecx]
        add    edx, 2
        mov    al, byte ptr [ecx + 9]
        db  00Ah, 005h, 064h, 05Bh, 000h, 000h
        or     al, 0c0h
        out    dx, al
loc_1_4870:
        mov    edx, dword ptr [ecx]
        inc    edx
        in     al, dx
        dec    esi
        jne    SHORT loc_1_4870
        mov    dx, word ptr [ecx]
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    edx, dword ptr [ecx]
        add    edx, 2
        mov    al, byte ptr [ecx + 8]
        out    dx, al
        mov    al, byte ptr [ebp - 2]
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_48A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    al, 0ffh
        mov    dx, word ptr [esi]
        out    dx, al
        mov    ecx, 1
loc_1_48B5:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    SHORT loc_1_48B5
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, byte ptr [esi + 9]
        db  00Ah, 005h, 064h, 05Bh, 000h, 000h
        out    dx, al
        mov    ecx, 1
loc_1_48D0:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    SHORT loc_1_48D0
        mov    dx, word ptr [esi]
        in     al, dx
        mov    byte ptr [ebp - 3], al
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, byte ptr [esi + 8]
        out    dx, al
        mov    al, byte ptr [ebp - 3]
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 02Bh
        db  0C0h, 0C2h, 004h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4910:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    al, 50h
        mov    dx, word ptr [esi]
        out    dx, al
        mov    ecx, 1
loc_1_4925:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    SHORT loc_1_4925
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, byte ptr [esi + 8]
        db  00Ah, 005h, 064h, 05Bh, 000h, 000h
        out    dx, al
        mov    ecx, 1
loc_1_4940:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    SHORT loc_1_4940
        mov    edx, dword ptr [esi]
        add    dx, 2
        mov    cl, byte ptr [esi + 8]
        mov    al, cl
        out    dx, al
        or     al, 8
        out    dx, al
        mov    al, cl
        out    dx, al
        mov    ecx, 1
        mov    edi, edi
loc_1_4960:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    SHORT loc_1_4960
        cmp    byte ptr [ebp + 0ch], 1
        cmc
        sbb    al, al
        and    al, 3
        mov    dx, word ptr [esi]
        out    dx, al
        mov    ecx, 1
loc_1_4979:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    SHORT loc_1_4979
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, byte ptr [esi + 8]
        db  00Ah, 005h, 064h, 05Bh, 000h, 000h
        out    dx, al
        mov    ecx, 1
loc_1_4994:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    SHORT loc_1_4994
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, byte ptr [esi + 8]
        out    dx, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_1_49B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    al, 58h
        mov    esi, dword ptr [edi]
        mov    dx, si
        out    dx, al
        lea    ecx, [esi + 2]
        mov    bl, byte ptr [edi + 8]
        or     bl, 8
        mov    dx, cx
        mov    al, bl
        out    dx, al
        mov    al, byte ptr [edi + 8]
        out    dx, al
        mov    dx, si
        mov    al, byte ptr [ebp + 0ch]
        out    dx, al
        mov    dx, cx
        mov    al, bl
        out    dx, al
        mov    al, byte ptr [edi + 8]
        out    dx, al
        mov    dx, si
        mov    al, byte ptr [ebp + 10h]
        out    dx, al
        mov    dx, cx
        mov    al, bl
        out    dx, al
        mov    al, byte ptr [edi + 8]
        out    dx, al
        mov    dx, si
        mov    al, byte ptr [ebp + 14h]
        out    dx, al
        test   byte ptr [ebp + 10h], 4
        mov    dx, cx
        mov    al, bl
        out    dx, al
        mov    al, byte ptr [edi + 8]
        out    dx, al
        db  0C6h, 005h, 064h, 05Bh, 000h, 000h, 001h
        jne    SHORT loc_1_4A1B
        db  0C6h, 005h, 064h, 05Bh, 000h, 000h, 002h
loc_1_4A1B:
        mov    edx, dword ptr [edi]
        add    dx, 2
        mov    cl, byte ptr [edi + 8]
        db  0A0h, 064h, 05Bh, 000h, 000h
        or     al, cl
        out    dx, al
        mov    al, cl
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4A40:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [ebp + 0ch]
        or     al, 80h
        mov    edi, dword ptr [esi]
        mov    dx, di
        out    dx, al
        lea    ecx, [edi + 2]
        mov    bl, byte ptr [esi + 8]
        mov    al, bl
        or     al, 8
        mov    dx, cx
        out    dx, al
        mov    al, bl
        out    dx, al
        mov    dx, di
        mov    al, byte ptr [ebp + 10h]
        out    dx, al
        db  0A0h, 064h, 05Bh, 000h, 000h
        or     al, bl
        mov    dx, cx
        out    dx, al
        mov    ecx, 1
loc_1_4A80:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    SHORT loc_1_4A80
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, byte ptr [esi + 8]
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4AA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [ebp + 0ch]
        mov    edx, dword ptr [esi]
        or     al, 60h
        out    dx, al
        add    edx, 2
        mov    cl, byte ptr [esi + 8]
        mov    al, cl
        or     al, 8
        out    dx, al
        mov    al, cl
        out    dx, al
        db  0A0h, 064h, 05Bh, 000h, 000h
        or     al, cl
        out    dx, al
        mov    ecx, 1
loc_1_4ACD:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    SHORT loc_1_4ACD
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, byte ptr [esi + 8]
        out    dx, al
        mov    ecx, 1
loc_1_4AE9:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    SHORT loc_1_4AE9
        mov    al, byte ptr [ebp - 2]
        sar    al, 3
        and    al, 47h
        mov    byte ptr [ebp - 2], al
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 3], al
        and    byte ptr [ebp - 3], 0b8h
        mov    al, byte ptr [ebp - 3]
        or     al, byte ptr [ebp - 2]
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4B20:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   0
        push   4
        mov    di, 0fch
        push   0
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_49B0
loc_1_4B38:
        db  0A0h, 064h, 05Bh, 000h, 000h
        push   eax
        push   edi
        push   esi
        call   sub_1_4BC0
        or     al, al
        je     SHORT loc_1_4B4D
        dec    di
        jne    SHORT loc_1_4B38
loc_1_4B4D:
        or     di, di
        je     SHORT loc_1_4B85
        push   0
        push   0
        push   0
        push   esi
        call   sub_1_49B0
        mov    di, 0fch
loc_1_4B62:
        db  0A0h, 064h, 05Bh, 000h, 000h
        push   eax
        push   edi
        push   esi
        call   sub_1_4BC0
        or     al, al
        je     SHORT loc_1_4B77
        dec    di
        jne    SHORT loc_1_4B62
loc_1_4B77:
        or     di, di
        je     SHORT loc_1_4B85
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_4B85:
        db  080h, 03Dh, 064h, 05Bh, 000h, 000h, 001h
        jne    SHORT loc_1_4BA0
        or     dword ptr [esi + 0bh], 4000h
        sub    ax, ax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_4BA0:
        db  080h, 03Dh, 064h, 05Bh, 000h, 000h, 002h
        jne    SHORT loc_1_4BB0
        or     dword ptr [esi + 0bh], 8000h
loc_1_4BB0:
        sub    ax, ax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4BC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   90h
        push   14h
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_4A40
        push   90h
        push   16h
        push   esi
        call   sub_1_4A40
        push   90h
        push   18h
        push   esi
        call   sub_1_4A40
        mov    ecx, dword ptr [esi]
        add    cx, 2
        mov    bl, byte ptr [esi + 8]
        mov    dx, cx
        mov    al, bl
        out    dx, al
        mov    ax, word ptr [ebp + 0ch]
        shr    ax, 1
        mov    edi, eax
        and    edi, 0ffffh
        or     edi, edi
        jle    SHORT loc_1_4C2F
        mov    al, byte ptr [ebp + 10h]
        xor    al, bl
        mov    byte ptr [ebp - 4], al
loc_1_4C20:
        mov    dx, cx
        mov    al, byte ptr [ebp - 4]
        out    dx, al
        mov    al, bl
        out    dx, al
        dec    edi
        or     edi, edi
        jg     SHORT loc_1_4C20
loc_1_4C2F:
        push   16h
        push   esi
        call   sub_1_4AA0
        and    eax, 0ffh
        mov    cx, word ptr [ebp + 0ch]
        and    ecx, 0feh
        sub    eax, ecx
        sub    eax, 2
        cmp    eax, 1
        sbb    al, al
        neg    al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah
        db  00Bh, 00Ch, 00Dh, 00Eh, 00Fh, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 018h, 019h, 01Ah
        db  01Bh, 01Ch, 01Dh, 01Eh, 01Fh, 020h, 021h, 022h, 023h, 024h, 025h, 026h, 027h, 028h, 029h, 02Ah
        db  02Bh, 02Ch, 02Dh, 02Eh, 02Fh, 030h, 031h, 032h, 033h, 034h, 035h, 036h, 037h, 038h, 039h, 03Ah
        db  03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 040h, 041h, 042h, 043h, 044h, 045h, 046h, 047h, 048h, 049h, 04Ah
        db  04Bh, 04Ch, 04Dh, 04Eh, 04Fh, 050h, 051h, 052h, 053h, 054h, 055h, 056h, 057h, 058h, 059h, 05Ah
        db  05Bh, 05Ch, 05Dh, 05Eh, 05Fh, 060h, 061h, 062h, 063h, 064h, 065h, 066h, 067h, 068h, 069h, 06Ah
        db  06Bh, 06Ch, 06Dh, 06Eh, 06Fh, 070h, 071h, 072h, 073h, 074h, 075h, 076h, 077h, 078h, 079h, 07Ah
        db  07Bh, 07Ch, 07Dh, 07Eh, 07Fh, 080h, 081h, 082h, 083h, 084h, 085h, 086h, 087h, 088h, 089h, 08Ah
        db  08Bh, 08Ch, 08Dh, 08Eh, 08Fh, 090h, 091h, 092h, 093h, 094h, 095h, 096h, 097h, 098h, 099h, 09Ah
        db  09Bh, 09Ch, 09Dh, 09Eh, 09Fh, 0A0h, 0A1h, 0A2h, 0A3h, 0A4h, 0A5h, 0A6h, 0A7h, 0A8h, 0A9h, 0AAh
        db  0ABh, 0ACh, 0ADh, 0AEh, 0AFh, 0B0h, 0B1h, 0B2h, 0B3h, 0B4h, 0B5h, 0B6h, 0B7h, 0B8h, 0B9h, 0BAh
        db  0BBh, 0BCh, 0BDh, 0BEh, 0BFh, 0C0h, 0C1h, 0C2h, 0C3h, 0C4h, 0C5h, 0C6h, 0C7h, 0C8h, 0C9h, 0CAh
        db  0CBh, 0CCh, 0CDh, 0CEh, 0CFh, 0D0h, 0D1h, 0D2h, 0D3h, 0D4h, 0D5h, 0D6h, 0D7h, 0D8h, 0D9h, 0DAh
        db  0DBh, 0DCh, 0DDh, 0DEh, 0DFh, 0E0h, 0E1h, 0E2h, 0E3h, 0E4h, 0E5h, 0E6h, 0E7h, 0E8h, 0E9h, 0EAh
        db  0EBh, 0ECh, 0EDh, 0EEh, 0EFh, 0F0h, 0F1h, 0F2h, 0F3h, 0F4h, 0F5h, 0F6h, 0F7h, 0F8h, 0F9h, 0FAh
        db  0FBh, 0FCh, 0FDh, 0FEh, 0FFh
loc_1_4DC0:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_4E4C:
        add    dword ptr [ecx], eax
        add    dword ptr [eax], eax
        add    al, byte ptr [ecx]
        add    dword ptr [eax], eax
        add    eax, dword ptr [ecx]
        add    dword ptr [eax], eax
        add    al, 1
        add    dword ptr [eax], eax
        add    eax, 6000101h
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], al
        add    dword ptr [ecx], eax
        add    byte ptr [eax], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], cl
        add    dword ptr [eax], eax
        or     eax, 10000101h
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [edx], eax
        add    byte ptr [edx], al
        add    dword ptr [edx], eax
        add    byte ptr [ebx], al
        add    dword ptr [edx], eax
        add    byte ptr [ecx + eax], al
        add    al, byte ptr [eax]
        add    eax, 1000201h
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ebx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx + eax], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx]
        add    al, byte ptr [ecx]
        add    al, byte ptr [ecx]
        add    eax, dword ptr [ecx]
        add    al, byte ptr [ecx]
loc_1_4EC8:
        add    dword ptr [ecx], eax
        add    dword ptr [eax], eax
        add    al, byte ptr [ecx]
        add    dword ptr [eax], eax
        add    eax, dword ptr [ecx]
        add    dword ptr [eax], eax
        add    al, 1
        add    dword ptr [eax], eax
        add    eax, 6000101h
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], al
        add    dword ptr [ecx], eax
        add    byte ptr [eax], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], cl
        add    dword ptr [eax], eax
        or     eax, 10000101h
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ebx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx + eax], eax
        add    dword ptr [ecx], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        js     SHORT loc_1_4F2C
        add    byte ptr [eax], al
loc_1_4F2C:
        db  078h, 003h
        add    byte ptr [eax], al
        mov    esp, 50000003h
        inc    ebp
        xor    ecx, dword ptr [esi + 44h]
        dec    ecx
        push   ebx
        add    byte ptr [eax + 45h], dl
        xor    ecx, dword ptr [esi + 44h]
        dec    ecx
        push   ebx
        add    byte ptr [ecx + 4fh], cl
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [ecx + 4fh], cl
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [ecx + 4eh], cl
        push   esp
        add    byte ptr [ecx + 4eh], cl
        push   esp
        add    byte ptr [ebp + 61h], cl
        db  078h, 069h
        insd
        jne    SHORT loc_1_4FD8
        dec    ebp
        db  075h, 06Ch
        je     SHORT loc_1_4FD9
        arpl   word ptr [ecx + 73h], sp
        db  074h, 04Ch
        imul   esi, dword ptr [ebx + 74h], 0
loc_1_4F7C:
        dec    ebp
        popad
        db  078h, 069h
        insd
        jne    SHORT loc_1_4FF0
        dec    ebp
        jne    SHORT loc_1_4FF2
        db  074h, 069h
        arpl   word ptr [ecx + 73h], sp
        je     SHORT loc_1_4FD9
        imul   esi, dword ptr [ebx + 74h], 0
        dec    esi
        db  065h, 074h, 077h
        outsd
        db  072h, 06Bh
        inc    ecx
        db  064h, 064h, 072h, 065h
        db  073h, 073h
        add    byte ptr [eax], al
        dec    esi
        db  065h, 074h, 077h
        outsd
        jb     SHORT loc_1_5016
        inc    ecx
        db  064h, 064h, 072h, 065h
        db  073h, 073h
        add    byte ptr [eax], al
        inc    edx
        jne    SHORT loc_1_502A
        push   esp
        jns    SHORT loc_1_502A
        add    byte ptr gs:[edx + 75h], al
        jae    SHORT loc_1_5014
        jns    SHORT loc_1_5032
        add    byte ptr gs:[esi + 65h], dl
        jb     SHORT loc_1_502A
        outsd
        db  073h, 065h
        push   ebx
        je     SHORT loc_1_502F
        je     SHORT loc_1_5045
        jae    SHORT loc_1_4FD2
loc_1_4FD2:
        add    byte ptr [eax], al
loc_1_4FD4:
        push   esi
        db  065h, 072h, 062h
loc_1_4FD8:
        outsd
loc_1_4FD9:
        jae    SHORT loc_1_5040
        push   ebx
        db  074h, 061h
        je     SHORT loc_1_5055
        jae    SHORT loc_1_4FE2
loc_1_4FE2:
        add    byte ptr [eax], al
loc_1_4FE4:
        pop    eax
        imul   esi, dword ptr [edx + 63h], 50206d6fh
        outsd
        arpl   word ptr [ebx + 65h], bp
loc_1_4FF0:
        je     SHORT loc_1_5012
loc_1_4FF2:
        inc    ebp
        je     SHORT loc_1_505D
        jb     NEAR PTR loc_1_5066
        db  065h, 074h, 020h
        dec    ecx
        dec    ecx
        dec    ecx
        and    byte ptr [ecx + 64h], al
        popad
        db  070h, 074h
        db  065h, 072h, 02Eh
        db  000h
sub_1_5008:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5012:
        add    byte ptr [eax], al
loc_1_5014:
        add    byte ptr [eax], al
loc_1_5016:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        pushad
        aas
        add    byte ptr [eax], al
        xor    byte ptr [esi], cl
        add    byte ptr [eax], al
loc_1_5028:
        adc    eax, dword ptr [ecx]
loc_1_502A:
        add    byte ptr [eax], al
        adc    byte ptr [edi], al
loc_1_502F:
        db  000h, 050h, 047h
loc_1_5032:
        add    byte ptr [eax], al
        adc    byte ptr [ecx], cl
        add    byte ptr [edx], dl
        add    dword ptr [eax], eax
        db  000h, 0F0h
        inc    esi
        add    byte ptr [eax], al
loc_1_5040:
        rol    byte ptr [edx], 0
        db  0F0h
loc_1_5045:
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_5055:
        add    byte ptr [eax], al
        db  000h
loc_1_5058:
        aas
        enter  5b01h, 33h
loc_1_505D:
        iretd
        db  0F7h, 0FFh, 0F7h, 0F7h, 0F7h, 07Bh, 0FBh, 0FBh
loc_1_5066:
        sti
        jnp    SHORT loc_1_5066
        jmp    SHORT loc_1_5066
        db  0DBh, 01Bh, 01Bh, 09Dh, 0CDh, 0EDh, 0C5h, 08Dh, 0EFh, 0D8h, 08Fh, 08Fh, 08Fh, 08Fh, 08Bh, 0C5h
        db  0C7h, 0C4h, 0C7h, 046h, 0FCh, 0EFh, 0CFh, 0CFh, 0CFh, 0CFh, 0DFh, 0FFh, 0E7h, 0FFh, 0CFh, 0EFh
        db  0CFh, 07Ah, 07Ah, 07Ah, 07Ah, 07Bh, 0BDh, 0BDh, 0BDh, 0BFh, 03Eh, 0FDh, 07Fh, 0BFh, 0BFh, 0BFh
        db  0BDh, 0BEh, 0DFh, 0D7h, 0CFh, 0FFh, 0BFh, 0DEh, 04Fh, 05Fh, 05Fh, 05Fh, 05Dh, 0AFh, 0AFh, 0AFh
        db  0ADh, 0EEh, 0FCh, 03Dh, 0F5h, 0F5h, 0FDh, 0FDh, 0FAh, 0FEh, 0FEh, 0FEh, 0FEh, 0EFh, 0DFh, 0BEh
        db  0AEh, 0AEh, 0ACh, 09Fh, 0D7h, 0D6h, 056h, 0F7h, 055h, 0FCh, 07Fh, 0EFh, 0FFh, 0FFh, 0FFh, 0DFh
        db  0FFh, 0EFh, 0EFh, 0BBh, 0AFh, 0DEh, 03Fh, 03Fh, 03Fh, 03Fh, 03Ch, 09Eh, 01Dh, 09Fh, 09Fh, 09Fh
        db  0FDh, 0FDh, 06Fh, 06Fh, 07Fh, 07Fh, 07Fh, 0BFh, 0BFh, 0BFh, 0BFh, 0BDh, 0DFh, 0BFh, 0BFh, 0BFh
        db  0BFh, 0BFh, 0DFh, 0DFh, 0DFh, 0DEh, 07Eh, 0FDh, 0FFh, 0FFh, 07Fh, 09Fh, 09Fh, 0AFh, 0FFh, 0EDh
        db  0D7h, 0E5h, 07Fh, 0C1h, 0CAh, 0CBh, 0CBh, 0FAh, 0FBh, 079h, 0FEh, 07Fh, 0FFh, 0FAh, 01Ch, 0FFh
        db  0FDh, 0FDh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0EFh, 0DFh, 0EFh, 0EFh, 0EFh, 0EFh, 0CFh
        db  0F7h, 0F7h, 0F5h, 0F7h, 0E7h, 0FDh, 0FFh, 0FDh, 0FDh, 0FDh, 0FFh, 0FEh, 0FFh, 0FFh, 0FFh, 0EFh
        db  0FBh, 0DBh, 0DFh, 0DBh, 0DBh, 0DFh, 0DBh, 0EDh, 0EFh, 0EDh, 0EFh, 0EFh, 03Ch, 0FEh, 0BEh, 0FEh
        db  0FEh, 0BEh, 0FFh, 07Eh, 05Fh, 07Eh, 06Fh, 07Fh, 0CFh, 0CFh, 0DBh, 0C9h, 0DFh, 0DBh, 0FCh, 0FFh
        db  0FEh, 0FFh, 0E7h, 0DDh, 02Eh, 05Dh, 05Fh, 0FDh, 05Dh, 06Eh, 037h, 0BEh, 0AFh, 0BFh, 0B1h, 0D5h
        db  0FFh, 0FFh, 0FCh, 0FFh, 0FDh, 0FEh, 07Fh, 0FFh, 0FFh, 0F6h, 0BDh, 03Fh, 0FFh, 0FFh, 0DFh, 0FFh
        db  0DFh, 0EFh, 0FFh, 0FFh, 0AFh, 0F3h, 0DEh, 06Fh, 06Fh, 06Eh, 06Fh, 06Fh, 0B7h, 037h, 0B3h, 036h
        db  0B3h, 0FDh, 07Fh, 0BFh, 07Fh, 0FFh, 07Fh, 05Fh, 0FFh, 0BFh, 0FFh, 0B7h, 0FBh, 0D5h, 0F7h, 0F7h
        db  0F4h, 0F7h, 0F7h, 0FCh, 0FFh, 0FCh, 07Bh, 0FAh, 0BDh, 07Fh, 0FFh, 0FFh, 0DFh, 0FEh, 07Fh, 0EFh
        db  0FFh, 0EFh, 05Fh, 0EBh, 0DDh, 0F7h, 0CEh, 0F7h, 0C3h, 0F5h, 0FBh, 0E7h, 07Bh, 0E3h, 07Ah, 0DCh
        db  0FCh, 0FFh, 0FFh, 0AFh, 0BFh, 0BEh, 0D7h, 0FEh, 0D7h, 0DEh, 0BDh, 0CDh, 0E7h, 0E6h, 0E7h, 0E5h
        db  0EFh, 0F7h, 076h, 0F7h, 073h, 0F3h, 0FDh, 0BCh, 0EFh, 0FFh, 0EFh, 0FFh, 0DFh, 0F7h, 0FFh, 0F7h
        db  0FEh, 0F3h, 0DFh, 0FFh, 0F6h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FEh, 0FFh, 0EEh, 0BDh, 07Ch, 0DCh
        db  0FEh, 0FEh, 0FEh, 05Fh, 07Eh, 07Fh, 07Fh, 07Eh, 07Bh, 0D5h, 0DEh, 0D5h, 0DFh, 0DCh, 0DFh, 0FFh
        db  0FFh, 07Fh, 0FFh, 0EEh, 0DDh, 0AFh, 0EDh, 09Fh, 0FDh, 0FDh, 0D7h, 0FFh, 0FFh, 0DEh, 0EFh, 0F5h
        db  0C1h, 0CFh, 0CAh, 0CBh, 0DBh, 0FCh, 0E7h, 0E7h, 0FFh, 0E7h, 067h, 0BDh, 0DFh, 0FDh, 0FDh, 0FEh
        db  0FFh, 0EFh, 0EEh, 0FFh, 0B7h, 0E7h, 0F7h, 0DFh, 0EDh, 0EFh, 0EFh, 0EFh, 0CEh, 0E5h, 0E7h, 0E7h
        db  0E5h, 0E7h, 0FDh, 07Dh, 0FFh, 0EDh, 0FDh, 0FFh, 0FFh, 0FFh, 0DFh, 0FFh, 0FFh, 0FBh, 0DFh, 0FFh
        db  0DBh, 0DBh, 0DBh, 0DFh, 0EFh, 06Fh, 0EFh, 0EFh, 06Fh, 0BCh, 0FEh, 0DEh, 0BEh, 0FEh, 0FEh, 0FFh
        db  07Fh, 077h, 07Fh, 07Eh, 07Fh, 0CFh, 0DFh, 0CBh, 0DBh, 0DBh, 0DFh, 0ECh, 0EFh, 0EFh, 0EFh, 06Ch
        db  0DDh, 03Fh, 07Eh, 05Dh, 05Ch, 07Dh, 07Eh, 02Eh, 0BEh, 0BEh, 0AFh, 0B9h, 0D5h, 0EFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FEh, 0FEh, 0FFh, 0FFh, 07Fh, 09Dh, 07Fh, 0EFh, 09Fh, 0FFh, 0BFh, 08Fh, 0DFh, 0DFh
        db  0F7h, 0FFh, 0FBh, 0DFh, 06Dh, 06Fh, 06Fh, 06Fh, 06Fh, 0B7h, 0B7h, 033h, 0B7h, 0B7h, 0FDh, 07Fh
        db  07Fh, 0BFh, 01Fh, 0EFh, 07Fh, 0CFh, 0BFh, 0FFh, 0BFh, 0BBh, 0D7h, 0F7h, 0F7h, 0F7h, 0F5h, 0F7h
        db  0FBh, 0FBh, 0FFh, 0FBh, 0FBh, 0BDh, 01Fh, 0FFh, 0FFh, 07Eh, 07Fh, 07Fh, 0BFh, 0BFh, 0FFh, 0BFh
        db  0BBh, 0DDh, 0CEh, 0FFh, 0CFh, 0F7h, 0C7h, 0FBh, 0E9h, 0FBh, 0E3h, 062h, 0DCh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0BEh, 0FFh, 07Fh, 06Fh, 07Fh, 07Fh, 07Dh, 0CFh, 0E6h, 0E7h, 0EFh, 0EFh, 0EFh, 0F6h, 0E7h
        db  0F7h, 0F7h, 0F7h, 0FDh, 0BFh, 0FCh, 0FDh, 0FFh, 0FDh, 0FEh, 0FEh, 0EEh, 0FEh, 0FEh, 0FBh, 0DFh
        db  0FEh, 0FFh, 0FFh, 0FFh, 0DFh, 0EFh, 0EEh, 0EFh, 0EFh, 06Fh, 07Dh, 0FCh, 0FCh, 0FEh, 04Eh, 0FCh
        db  0FEh, 076h, 06Eh, 07Eh, 07Eh, 07Bh, 0DDh, 0FDh, 0DFh, 0CEh, 0DEh, 0DDh, 0EEh, 06Eh, 0EFh, 06Ah
        db  0EEh, 0DDh, 0BDh, 09Fh, 0FDh, 0CDh, 0EDh, 0FEh, 0DEh, 0DEh, 0F6h, 0CEh, 0EDh, 0DFh, 0C9h, 0FFh
        db  0CFh, 0FFh, 0CFh, 0FEh, 066h, 0FEh, 067h, 0FEh, 01Dh, 09Fh, 0FDh, 07Dh, 08Dh, 0DDh, 0FFh, 0B6h
        db  0DEh, 0EFh, 0D6h, 0ABh, 0DFh, 0EFh, 0EDh, 0EFh, 0CBh, 0CFh, 0F7h, 0E7h, 0F7h, 0F7h, 0E7h, 07Dh
        db  0FFh, 0FFh, 0FFh, 0AFh, 0BFh, 0FFh, 0FFh, 0F7h, 0DEh, 0E7h, 0FFh, 0DFh, 0DFh, 0DFh, 0D9h, 0DBh
        db  0DFh, 0EFh, 07Eh, 0EFh, 06Eh, 0EDh, 0FCh, 0DCh, 0FCh, 0CCh, 0FEh, 0CEh, 0DFh, 05Fh, 06Eh, 047h
        db  07Fh, 05Fh, 0CFh, 0FDh, 0FFh, 0FFh, 0CEh, 0CFh, 0ECh, 0EFh, 0FFh, 067h, 0EFh, 0DDh, 07Fh, 05Eh
        db  07Fh, 07Eh, 07Fh, 07Eh, 0BFh, 0AFh, 0BFh, 0F6h, 0A9h, 0D7h, 0FDh, 0FDh, 0FFh, 0FEh, 0F7h, 0FEh
        db  0F7h, 0FFh, 07Fh, 0FFh, 0BDh, 03Fh, 0FFh, 0FFh, 0FFh, 0FFh, 08Fh, 0FFh, 0FFh, 0DFh, 0FFh, 0F3h
        db  0DFh, 066h, 06Ch, 06Fh, 06Eh, 06Fh, 0B3h, 0B7h, 0B7h, 0B7h, 0B3h, 0FDh, 07Fh, 09Fh, 0CFh, 07Fh
        db  0EFh, 06Fh, 0CFh, 0BFh, 0BFh, 0FFh, 0FBh, 0D6h, 0F7h, 0FDh, 0F7h, 0FFh, 0F7h, 0FEh, 0FBh, 0FFh
        db  0FFh, 0FDh, 0BDh, 05Fh, 07Fh, 0DFh, 07Eh, 0FFh, 07Fh, 0FFh, 0FFh, 0FFh, 0FFh, 0EBh, 0DFh, 0F7h
        db  0F7h, 0C6h, 0D7h, 0FCh, 0E3h, 063h, 063h, 07Bh, 0FBh, 0DCh, 0FDh, 05Ch, 0FEh, 0FFh, 0BDh, 0CFh
        db  0BFh, 07Fh, 07Fh, 07Fh, 0BDh, 0CFh, 0EFh, 0EFh, 0EEh, 0CFh, 0EFh, 0F7h, 0F3h, 0F7h, 077h, 0F7h
        db  07Dh, 0FDh, 0DDh, 0FFh, 0FDh, 0ECh, 0FEh, 0FFh, 0FFh, 0FEh, 07Eh, 07Bh, 0DBh, 0FFh, 0FCh, 0FEh
        db  0F7h, 0FFh, 0EFh, 07Fh, 07Fh, 07Fh, 0FFh, 07Dh, 0FEh, 0DEh, 0FCh, 0FEh, 0DEh, 0DEh, 07Eh, 07Eh
        db  07Eh, 07Eh, 07Bh, 0DFh, 0EFh, 0EFh, 0FDh, 0CFh, 0FFh, 0EEh, 0EAh, 0EEh, 0EFh, 06Fh, 07Ch, 0DFh
        db  0DFh, 0AFh, 09Fh, 0DDh, 0FEh, 0F6h, 0CEh, 0CFh, 0FEh, 0FBh, 0C7h, 0C6h, 0FFh, 0CFh, 0DFh, 0CFh
        db  0FFh, 0E3h, 0FEh, 06Fh, 0E7h, 09Dh, 0DFh, 0EDh, 0CFh, 0FEh, 0CFh, 0EFh, 0EEh, 0F6h, 0F7h, 07Fh
        db  0F3h, 0DBh, 0EAh, 0CBh, 0EAh, 0CBh, 0EBh, 0E5h, 0E7h, 0E7h, 0E7h, 0F7h, 0FDh, 07Dh, 0FFh, 0FDh
        db  0FFh, 0FDh, 0FFh, 0FFh, 0FFh, 0FFh, 0FEh, 0FFh, 0DFh, 0FFh, 0DAh, 0DFh, 0DAh, 0DFh, 0EDh, 0EFh
        db  0FFh, 07Fh, 0EFh, 0FCh, 0FEh, 0FEh, 0FEh, 0FEh, 0FEh, 0FFh, 037h, 07Fh, 05Fh, 067h, 04Fh, 0CFh
        db  0DFh, 0CEh, 0EFh, 0DEh, 0EDh, 0E7h, 0FFh, 0ECh, 0EFh, 067h, 0DDh, 07Fh, 0FFh, 05Fh, 0FCh, 07Fh
        db  0DFh, 02Eh, 0BFh, 0BFh, 0BFh, 0A9h, 0D5h, 0FFh, 0FEh, 0FFh, 0FEh, 0FDh, 0FEh, 0FFh, 0FEh, 0FFh
        db  07Fh, 0BDh, 07Fh, 0DFh, 0FFh, 0CFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0F3h, 0DEh, 06Fh, 06Fh
        db  06Fh, 06Fh, 06Fh, 0B7h, 033h, 0B3h, 0B7h, 0B7h, 0FDh, 07Fh, 0FFh, 05Fh, 0EFh, 0FFh, 09Fh, 0CFh
        db  0FFh, 0CFh, 0BFh, 0BBh, 0D7h, 0F7h, 0F7h, 0F5h, 0F7h, 0F7h, 0FBh, 0FBh, 0FAh, 0FBh, 0F9h, 0BDh
        db  04Fh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0DFh, 0FFh, 0FFh, 0FFh, 0FBh, 0DFh, 0DFh, 0C7h, 0F4h, 0D7h
        db  0D7h, 0FBh, 0FBh, 0E3h, 0E3h, 063h, 0DCh, 0EFh, 0FEh, 0FEh, 0FEh, 0FFh, 05Fh, 07Eh, 0BFh, 0AFh
        db  07Fh, 07Dh, 0CFh, 0C7h, 0E7h, 0E5h, 0C7h, 0C7h, 0F2h, 0F3h, 0F3h, 0F3h, 073h, 0FDh, 0FDh, 0FFh
        db  0DDh, 0FDh, 0DDh, 05Eh, 0FEh, 0FEh, 0FFh, 0FFh, 0FBh, 0DFh, 0FFh, 0FFh, 0FFh, 0DFh, 0FFh, 0EFh
        db  0FFh, 0EEh, 07Fh, 07Fh, 07Dh, 0FCh, 0ECh, 05Eh, 0FCh, 0ECh, 0DEh, 077h, 07Eh, 07Eh, 07Eh, 07Fh
        db  0DFh, 0ECh, 0DFh, 0EEh, 0DFh, 0E7h, 0EFh, 076h, 0EFh, 0EEh, 0EFh, 0FCh, 0FFh, 0BFh, 0DFh, 0EDh
        db  0FFh, 0FFh, 0E7h, 0FEh, 0D6h, 0CFh, 0EFh, 0C3h, 0FFh, 0FFh, 0FDh, 0FFh, 0CDh, 087h, 0E5h, 06Eh
        db  0EFh, 0FFh, 09Ch, 0BFh, 07Dh, 0EDh, 0EFh, 0FDh, 069h, 0FEh, 0FFh, 07Fh, 046h, 0E3h, 0D3h, 0CDh
        db  0CFh, 0CFh, 0CFh, 0CFh, 087h, 0E7h, 0E5h, 0E7h, 0E7h, 0FDh, 0FFh, 0FFh, 0DFh, 0FFh, 0DFh, 0F9h
        db  0FFh, 0F7h, 0FFh, 0C7h, 0FFh, 0DFh, 0DFh, 0DFh, 0DFh, 0FFh, 0DEh, 09Fh, 0EDh, 0EFh, 0ECh, 0EFh
        db  0FCh, 0FEh, 0FEh, 0FEh, 0BEh, 0FEh, 0B9h, 07Fh, 047h, 07Fh, 07Fh, 07Fh, 0CFh, 0CBh, 0DDh, 0D9h
        db  0DDh, 0D9h, 08Fh, 0E5h, 0FFh, 0EFh, 0FFh, 0DDh, 05Fh, 0FDh, 07Dh, 07Eh, 05Dh, 079h, 02Fh, 0AEh
        db  0BEh, 0B6h, 0B1h, 0D7h, 0F7h, 0FDh, 0FDh, 0FDh, 0FDh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0BDh, 07Fh
        db  0BFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0CFh, 0FFh, 0FFh, 0BBh, 0DEh, 06Fh, 06Fh, 067h, 06Eh, 067h
        db  0F7h, 0B7h, 0B7h, 0B7h, 0B7h, 0FDh, 07Fh, 0FFh, 01Fh, 0DFh, 08Fh, 09Fh, 0DFh, 0AFh, 0BFh, 0BFh
        db  0BBh, 0D7h, 0F7h, 0FFh, 0FFh, 0FFh, 0F7h, 0FBh, 0F9h, 0FBh, 0FBh, 0FFh, 0BDh, 07Fh, 07Fh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0F7h, 0FFh, 0F7h, 0EBh, 0DDh, 0F3h, 0C2h, 0F3h, 0D6h, 0F7h, 08Bh, 0E3h
        db  0E3h, 0E3h, 0FAh, 0DCh, 0ECh, 0FEh, 0FEh, 0FEh, 0CFh, 079h, 07Fh, 07Fh, 07Fh, 06Fh, 07Dh, 0C7h
        db  0EFh, 0EFh, 0EFh, 0CDh, 0E5h, 082h, 0F3h, 0F3h, 0F3h, 0F7h, 0BDh, 0BDh, 0FDh, 0FDh, 0DDh, 0FCh
        db  0F8h, 0FFh, 0EEh, 0FEh, 07Eh, 0F3h, 0DFh, 0DFh, 0DFh, 0DFh, 0DFh, 0FFh, 09Bh, 0FFh, 0FFh, 0FEh
        db  07Fh, 0FDh, 07Ch, 0DCh, 0FCh, 0FCh, 0FEh, 0F8h, 076h, 077h, 07Eh, 07Fh, 07Fh, 0CDh, 0DFh, 0DDh
        db  0FFh, 0DEh, 0FFh, 086h, 06Eh, 076h, 0EFh, 0F6h, 0FCh, 0EFh, 0BDh, 0DDh, 05Dh, 0FDh, 0F8h, 0DFh
        db  0FFh, 0FEh, 0F7h, 0F5h, 0C7h, 0CEh, 0DEh, 0FFh, 0FEh, 0CBh, 0E2h, 0FFh, 0EEh, 0FFh, 07Fh, 0BDh
        db  0DFh, 0FEh, 05Fh, 07Dh, 0FFh, 0DFh, 0DEh, 0FFh, 01Eh, 0E7h, 0EBh, 0DBh, 0E7h, 0CBh, 0CFh, 0C3h
        db  0EFh, 0F7h, 0F6h, 0F5h, 0F5h, 0E7h, 0BDh, 07Dh, 0FFh, 07Fh, 0FFh, 0FCh, 07Eh, 0E7h, 0FEh, 0FFh
        db  0FFh, 0F7h, 0DFh, 0DDh, 0FDh, 070h, 0DFh, 0DCh, 0ECh, 0EBh, 0FFh, 0FDh, 06Fh, 0FCh, 0BEh, 0FEh
        db  0F6h, 0FCh, 07Eh, 0EFh, 07Fh, 067h, 07Fh, 07Fh, 04Fh, 0CEh, 0ECh, 0FFh, 05Fh, 0FEh, 0CFh, 0EFh
        db  0EFh, 0FFh, 0EFh, 077h, 0DDh, 07Fh, 0FEh, 077h, 05Fh, 07Fh, 0FEh, 0F6h, 0BFh, 0B7h, 0BFh, 0E9h
        db  0D5h, 0FFh, 0FFh, 0CFh, 0FFh, 0FEh, 0FFh, 0FFh, 0FBh, 0FFh, 07Fh, 0BDh, 07Fh, 0FFh, 0F7h, 0EEh
        db  0FFh, 0FFh, 0DFh, 0FFh, 0C7h, 0FFh, 0F3h, 0DFh, 06Dh, 06Fh, 0E5h, 06Fh, 06Fh, 0B7h, 0B2h, 033h
        db  0B7h, 0B7h, 0FDh, 07Eh, 09Eh, 0FFh, 0FFh, 03Fh, 0FFh, 0FFh, 0F7h, 0FFh, 0BFh, 0FBh, 0D6h, 0F6h
        db  0F7h, 072h, 0FFh, 0F7h, 0FBh, 0FEh, 0FBh, 0FFh, 0F9h, 03Dh, 04Fh, 07Eh, 07Dh, 07Fh, 0BFh, 05Fh
        db  0BFh, 0E7h, 0BFh, 07Fh, 0BBh, 0DFh, 0C6h, 0C7h, 077h, 0D7h, 0F3h, 0FEh, 0EBh, 0FFh, 0E3h, 0FBh
        db  0DCh, 0FEh, 0DFh, 0B4h, 0FEh, 0DEh, 0DEh, 0FFh, 0AFh, 0FFh, 0DFh, 07Dh, 0CFh, 0EDh, 0EDh, 06Dh
        db  0CFh, 0EFh, 0F7h, 0E7h, 0F6h, 0F7h, 0F7h, 0FDh, 0FFh, 0FFh, 0F5h, 0FDh, 05Dh, 0EEh, 07Eh, 0FEh
        db  077h, 0FEh, 0FBh, 0DBh, 0FDh, 0FFh, 0DFh, 0FDh, 0FFh, 0FFh, 0FEh, 07Fh, 0FFh, 0EFh, 03Dh, 05Ch
        db  0FEh, 0FCh, 0CCh, 05Eh, 0FEh, 076h, 07Fh, 067h, 07Eh, 07Bh, 0D7h, 0FFh, 0FFh, 0FFh, 0CDh, 0CFh
        db  0EEh, 067h, 0FFh, 06Eh, 0FFh, 05Ch, 09Fh, 0FDh, 0FFh, 07Dh, 0BFh, 0F6h, 0FEh, 0FEh, 0E6h, 0FFh
        db  0EDh, 0C3h, 007h, 04Eh, 0BAh, 07Dh, 0CDh, 0FFh, 0C2h, 067h, 0FFh, 0FEh, 09Ch, 0F3h, 08Bh, 0F9h
        db  07Bh, 07Dh, 0DEh, 0FCh, 0FFh, 0FEh, 0DEh, 0FBh, 0DBh, 00Fh, 05Bh, 057h, 05Fh, 01Fh, 0B4h, 0F7h
        db  077h, 0E7h, 0E7h, 0FDh, 0F2h, 0FAh, 077h, 0FBh, 0E3h, 07Ah, 0FCh, 07Eh, 0FFh, 0C7h, 0FFh, 0DDh
        db  00Ah, 08Bh, 08Ch, 08Eh, 00Bh, 0AFh, 08Dh, 06Dh, 0EAh, 0EFh, 0FCh, 0F2h, 0F7h, 0F9h, 0F7h, 0F3h
        db  0FBh, 07Bh, 07Fh, 07Fh, 077h, 07Fh, 0CFh, 03Eh, 09Fh, 09Fh, 09Fh, 0DDh, 0EFh, 09Fh, 06Eh, 0E7h
        db  0EFh, 0DDh, 071h, 074h, 07Ah, 074h, 07Ch, 07Eh, 0F8h, 0BEh, 0EFh, 0B6h, 0B9h, 0D6h, 0FFh, 05Eh
        db  07Dh, 04Fh, 05Fh, 0AFh, 0EFh, 0FFh, 0FFh, 0FFh, 0BDh, 03Eh, 0BFh, 0FFh, 07Fh, 0FFh, 0FBh, 0BBh
        db  0FFh, 0F7h, 0FFh, 0F3h, 0DFh, 0E7h, 06Fh, 06Fh, 06Fh, 06Dh, 0F7h, 0F7h, 0B7h, 0B7h, 037h, 0FDh
        db  07Fh, 0DFh, 0AFh, 0FFh, 0BFh, 0FFh, 0DFh, 0BFh, 0DFh, 0A7h, 0BBh, 0D6h, 0F4h, 0B7h, 0B7h, 075h
        db  0B5h, 0BAh, 0BFh, 0FFh, 0FDh, 0FDh, 0BDh, 05Fh, 077h, 0FDh, 0E3h, 0F6h, 0FAh, 03Eh, 0FFh, 0FFh
        db  0F7h, 0EBh, 0DFh, 037h, 057h, 0F6h, 0C7h, 0D7h, 0EBh, 08Bh, 06Fh, 0E3h, 0E3h, 0DCh, 0F1h, 07Ah
        db  0FDh, 0BFh, 0BFh, 05Fh, 0D9h, 0BFh, 0FFh, 07Fh, 07Dh, 0CFh, 03Fh, 057h, 037h, 037h, 017h, 0A7h
        db  0AFh, 0E6h, 0F7h, 0F7h, 0FDh, 0F1h, 0F9h, 0E1h, 0F1h, 0F1h, 0FAh, 0F8h, 0FEh, 0FEh, 06Eh, 0FBh
        db  0DBh, 03Fh, 0FFh, 0EFh, 0FFh, 0FFh, 0A7h, 0C7h, 0FBh, 0FFh, 0FFh, 0FDh, 0F3h, 0DFh, 0CFh, 0DFh
        db  0EFh, 0EBh, 06Fh, 0FEh, 076h, 077h, 07Fh, 0DFh, 02Dh, 007h, 00Fh, 00Fh, 007h, 0EFh, 0DEh, 0E6h
        db  0EEh, 067h, 0FCh, 0D3h, 0F1h, 0F1h, 0F1h, 0D1h, 0D6h, 09Ch, 0FEh, 0DFh, 0FEh, 0FDh, 0CDh, 0F5h
        db  04Fh, 04Fh, 07Eh, 05Eh, 05Eh, 09Dh, 0EAh, 0E7h, 0EEh, 01Dh, 0FFh, 0F7h, 07Fh, 0E7h, 0B4h, 0BCh
        db  0DDh, 0FFh, 07Eh, 0FFh, 06Bh, 0D7h, 0FFh, 0EFh, 0C7h, 05Bh, 05Fh, 0FFh, 087h, 067h, 0C7h, 0E7h
        db  07Dh, 0FFh, 0FFh, 0F7h, 0E7h, 0F7h, 0FCh, 0E9h, 05Fh, 0FDh, 0FFh, 0FFh, 0DFh, 01Fh, 0DBh, 01Fh
        db  05Fh, 05Bh, 09Fh, 09Ch, 0CDh, 04Bh, 0EFh, 0FCh, 0F1h, 0FCh, 0F4h, 0F7h, 0F7h, 07Bh, 0E9h, 077h
        db  07Dh, 07Fh, 07Fh, 0CFh, 01Dh, 05Fh, 059h, 05Bh, 05Ch, 08Eh, 0DFh, 0FFh, 06Fh, 0EEh, 0DDh, 053h
        db  0E6h, 073h, 0F4h, 074h, 079h, 0F9h, 0B4h, 0FEh, 0AEh, 0B1h, 0D7h, 04Ch, 0BFh, 05Eh, 05Dh, 05Fh
        db  0AFh, 05Fh, 0AFh, 0AFh, 0F6h, 0BDh, 037h, 0C7h, 0FAh, 0F6h, 0F5h, 0FAh, 0FBh, 0FBh, 0CBh, 0FFh
        db  0FBh, 0DFh, 06Eh, 06Fh, 06Dh, 06Fh, 0CFh, 0E7h, 0B3h, 0F7h, 0F7h, 0B7h, 0FDh, 07Fh, 0FFh, 0EFh
        db  0FFh, 0EFh, 0EFh, 0FFh, 0FFh, 0EFh, 0BFh, 0BBh, 0D6h, 0B5h, 07Eh, 077h, 076h, 074h, 0BBh, 03Fh
        db  0BFh, 0BBh, 0FBh, 0BDh, 073h, 0E5h, 0DDh, 0FDh, 0F5h, 0FAh, 0FAh, 07Ah, 0FAh, 0B7h, 0ABh, 0DDh
        db  0F7h, 007h, 005h, 0B4h, 057h, 0BAh, 0EBh, 0C3h, 083h, 0F8h, 0DCh, 0ECh, 0D3h, 072h, 0F4h, 0E4h
        db  0F8h, 0BFh, 0CBh, 069h, 06Eh, 07Dh, 0CDh, 0F7h, 06Fh, 0E7h, 077h, 057h, 0B3h, 067h, 0B7h, 0DFh
        db  0F7h, 0FDh, 0FDh, 0E8h, 0FDh, 0F9h, 0F5h, 0FAh, 0BEh, 0F4h, 0FCh, 07Eh, 0F3h, 0DFh, 03Fh, 0FFh
        db  0FDh, 09Dh, 04Fh, 0EFh, 0CFh, 06Fh, 0DEh, 06Bh, 0FDh, 0F3h, 0FCh, 0DCh, 075h, 0F5h, 0DFh, 07Fh
        db  072h, 075h, 0FEh, 07Bh, 0DFh, 0EFh, 0BDh, 0CFh, 05Eh, 05Dh, 0DAh, 0BFh, 0BFh, 00Fh, 0EEh, 0DCh
        db  0BFh, 0C7h, 0FDh, 0D9h, 0F5h, 0F7h, 0F1h, 0FEh, 0F9h, 0F6h, 0F5h, 0D7h, 0FEh, 0DEh, 075h, 07Ch
        db  0FBh, 04Eh, 027h, 026h, 0DBh, 07Fh, 09Ch, 0DFh, 0FEh, 055h, 0F7h, 0FDh, 0FDh, 03Dh, 0DCh, 0FDh
        db  0CFh, 0E3h, 0DFh, 0EFh, 0EFh, 08Fh, 0CFh, 0C5h, 0C7h, 0CFh, 0CFh, 07Fh, 067h, 0FDh, 0FFh, 0FFh
        db  076h, 0EEh, 0DFh, 0DFh, 0BBh, 0FBh, 0FFh, 0D7h, 0FFh, 0DBh, 0DBh, 0FFh, 03Bh, 01Bh, 0F7h, 0AFh
        db  0BDh, 0AAh, 0CCh, 0EFh, 0FCh, 0FEh, 07Eh, 072h, 070h, 0FEh, 0D9h, 07Dh, 0FDh, 0FDh, 0FFh, 07Fh
        db  0CAh, 0CEh, 0FFh, 05Fh, 07Dh, 01Fh, 095h, 0CEh, 0CFh, 08Fh, 0EDh, 0DDh, 07Fh, 07Eh, 077h, 076h
        db  073h, 0F9h, 0BBh, 0FAh, 0B8h, 0BEh, 0B9h, 0D7h, 0FFh, 0FFh, 05Fh, 04Fh, 0DFh, 0AFh, 0AFh, 027h
        db  0BEh, 0FFh, 0BDh, 0FFh, 0FFh, 0F7h, 0E7h, 0E6h, 0BBh, 0FBh, 0FBh, 0F5h, 0DFh, 0DBh, 0CFh, 0EEh
        db  06Dh, 0E6h, 06Fh, 06Fh, 0F3h, 0B7h, 0B3h, 0B7h, 0B7h, 0FDh, 07Fh, 07Eh, 0FFh, 0FFh, 0DFh, 0AFh
        db  0FFh, 0DFh, 0E7h, 0BFh, 0BBh, 0D7h, 0F1h, 0F5h, 077h, 0B7h, 075h, 0BFh, 05Bh, 0DBh, 0BBh, 0FBh
        db  0BDh, 07Fh, 07Fh, 065h, 07Dh, 073h, 07Ah, 0FBh, 0FBh, 0FAh, 077h, 0ABh, 0DFh, 0DFh, 0FFh, 046h
        db  083h, 045h, 09Bh, 01Bh, 09Bh, 0C3h, 0FBh, 0DCh, 0FFh, 0FFh, 0D7h, 075h, 092h, 0FAh, 079h, 079h
        db  07Dh, 0DEh, 07Dh, 0CDh, 0DFh, 0EFh, 07Fh, 06Fh, 06Fh, 0B7h, 0DAh, 0DBh, 05Bh, 0F7h, 0FDh, 07Dh
        db  0FCh, 0E9h, 0F9h, 07Fh, 0EAh, 0EAh, 0B2h, 0FDh, 0FEh, 0FBh, 0DFh, 0F4h, 0FEh, 0FFh, 0DFh, 0FFh
        db  0FFh, 0E7h, 06Fh, 0DFh, 06Fh, 0FDh, 0FDh, 0FEh, 0FFh, 0EEh, 0F6h, 0FFh, 07Eh, 0FEh, 0EDh, 0F6h
        db  07Bh, 0DDh, 0CFh, 0FFh, 06Fh, 0DDh, 01Fh, 0F6h, 0CFh, 0CFh, 0CFh, 0EFh, 05Dh, 0BFh, 0FDh, 0F5h
        db  0EDh, 075h, 0BCh, 0F2h, 0BAh, 0FCh, 0FEh, 0EDh, 0D7h, 003h, 00Dh, 07Dh, 0CFh, 0CAh, 08Fh, 09Fh
        db  0ADh, 0FEh, 0FFh, 09Dh, 051h, 0D1h, 0F5h, 0FDh, 06Dh, 0F9h, 058h, 0FDh, 07Fh, 0CEh, 0BBh, 0DFh
        db  0FFh, 0CFh, 05Fh, 0DFh, 0DFh, 0EFh, 085h, 0E5h, 0E7h, 0E7h, 07Dh, 0FDh, 0FDh, 0F4h, 0FDh, 0FCh
        db  0FFh, 0F5h, 0F7h, 07Fh, 0FFh, 0BFh, 0DEh, 0DFh, 07Bh, 05Bh, 01Fh, 01Bh, 0FFh, 08Dh, 04Fh, 0EDh
        db  0EFh, 0FCh, 0FDh, 0B6h, 0F7h, 0A1h, 0B3h, 0FFh, 0F8h, 05Bh, 06Fh, 057h, 07Fh, 0CFh, 03Eh, 07Fh
        db  05Fh, 03Fh, 03Fh, 09Fh, 08Fh, 017h, 0EFh, 0FFh, 0DDh, 072h, 066h, 065h, 072h, 070h, 069h, 0B1h
        db  0F9h, 0BEh, 0BEh, 0B9h, 0D5h, 0BFh, 0D6h, 057h, 05Fh, 057h, 0ABh, 0E7h, 0ABh, 0FFh, 0FFh, 0BDh
        db  0F7h, 0F7h, 0EBh, 0FBh, 0FBh, 0FBh, 0FBh, 0FBh, 0F7h, 0FFh, 0DBh, 0CFh, 06Eh, 06Fh, 06Eh, 06Fh
        db  06Fh, 0B3h, 0B7h, 0F3h, 0B7h, 0B3h, 0FDh, 07Fh, 0FFh, 0FFh, 07Fh, 0FFh, 0FFh, 0F7h, 0F7h, 0FFh
        db  037h, 0FBh, 0D7h, 0F7h, 0B3h, 077h, 072h, 073h, 0DBh, 0BFh, 0BBh, 0FBh, 0FFh, 0BCh, 0F5h, 0FBh
        db  0FFh, 0F6h, 0F3h, 0EAh, 0FAh, 0FAh, 037h, 0B7h, 0FBh, 0DFh, 003h, 07Fh, 043h, 01Fh, 017h, 0DBh
        db  02Bh, 08Bh, 0E3h, 078h, 0DCh, 0F3h, 077h, 0F7h, 073h, 0F1h, 0BBh, 0BDh, 079h, 0DFh, 076h, 0ADh
        db  0CDh, 066h, 077h, 0A7h, 0C7h, 0D6h, 0BBh, 087h, 0AFh, 0F7h, 0F7h, 0FDh, 0D5h, 0F5h, 0F8h, 0FDh
        db  0FDh, 0FDh, 0F8h, 07Dh, 0F6h, 07Eh, 0EBh, 0DFh, 07Fh, 05Fh, 07Fh, 0AFh, 0DFh, 0DFh, 09Fh, 0FFh
        db  0FFh, 0FEh, 0FDh, 074h, 0F7h, 0F4h, 0F6h, 0FFh, 0FAh, 0F8h, 07Eh, 0FEh, 077h, 07Bh, 0CFh, 0CEh
        db  07Eh, 05Fh, 04Eh, 01Fh, 0AFh, 09Fh, 0DFh, 06Fh, 0F7h, 0FCh, 07Dh, 0F7h, 0B7h, 0F9h, 0F1h, 0B4h
        db  0F9h, 0FBh, 0FEh, 0FFh, 0FBh, 0DDh, 07Eh, 0FAh, 07Eh, 07Eh, 0FAh, 0BFh, 03Fh, 02Fh, 03Bh, 07Fh
        db  0FDh, 066h, 0F7h, 0E7h, 0F7h, 0E7h, 0FBh, 033h, 07Bh, 07Bh, 07Bh, 077h, 0CFh, 06Eh, 0A7h, 0AFh
        db  0CFh, 0AFh, 0A7h, 097h, 037h, 037h, 036h, 0FDh, 072h, 0EAh, 0E2h, 0EAh, 0EAh, 0EDh, 0B1h, 0D5h
        db  0F1h, 075h, 077h, 0DEh, 0BFh, 09Fh, 09Eh, 09Fh, 09Eh, 0EFh, 0EFh, 0FFh, 0DFh, 0DEh, 07Dh, 068h
        db  0F0h, 0F0h, 0F0h, 0F0h, 0F8h, 078h, 07Ch, 07Ch, 07Ch, 07Fh, 0DFh, 0FEh, 0FEh, 0FEh, 0FEh, 0FEh
        db  0FFh, 07Fh, 0FFh, 0FFh, 07Fh, 0FFh, 0FFh, 0FFh, 0FFh
loc_1_5B34:
        add    dword ptr [edx], eax
        add    al, 0ch
        adc    byte ptr [eax], ah
        inc    eax
        or     byte ptr [0e0d0e07h], 0eh
        add    dword ptr [eax], eax
loc_1_5B44:
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 60c0c06h], cl
        add    byte ptr [eax], al
loc_1_5B54:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, 0
loc_1_5B64:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
_LTEXT ends
        end
