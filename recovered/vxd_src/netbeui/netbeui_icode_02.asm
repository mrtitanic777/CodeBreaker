; netbeui_icode_02.asm -- NETBEUI object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        db  0A0h, 0B0h, 06Dh, 000h, 000h
        test   al, al
        je     SHORT loc_2_000B
        clc
        ret
loc_2_000B:
        db  0FEh, 005h, 0B0h, 06Dh, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 028h, 000h
        jb     SHORT loc_2_0056
        db  0CDh, 020h, 000h, 000h, 014h, 000h
        jb     SHORT loc_2_0056
        cmp    eax, 30ah
        jb     SHORT loc_2_0056
        db  0CDh, 020h, 000h, 000h, 040h, 000h
        jb     SHORT loc_2_0037
        db  0C6h, 005h, 0C0h, 06Eh, 000h, 000h, 001h
loc_2_0037:
        clc
        pushf
        cld
        push   ecx
        push   edi
        push   eax
        xor    eax, eax
        db  0BFh, 0C0h, 077h, 000h, 000h
        mov    ecx, 100h
        rep stosd
        pop    eax
        pop    edi
        pop    ecx
        popf
        call   sub_2_043B
loc_2_0056:
        ret
        db  000h
sub_2_0058:
        xor    ecx, ecx
        db  00Fh, 0B6h, 005h, 022h, 06Fh, 000h, 000h
        imul   eax, eax, 84h
        add    ecx, eax
        db  00Fh, 0B6h, 005h, 023h, 06Fh, 000h, 000h
        imul   eax, eax, 3eh
        add    ecx, eax
        db  00Fh, 0B6h, 005h, 024h, 06Fh, 000h, 000h
        imul   eax, eax, 70h
        add    ecx, eax
        add    ecx, 3
        and    cl, 0fch
        push   1
        push   ecx
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        test   eax, eax
        je     SHORT loc_2_009D
        db  0A3h, 03Ah, 06Eh, 000h, 000h
        ret
loc_2_009D:
        stc
        ret
sub_2_009F:
        db  0C7h, 005h, 0ECh, 06Eh, 000h, 000h, 001h, 000h, 000h, 000h
        db  08Dh, 005h, 068h, 06Fh, 000h, 000h
        db  0A3h, 068h, 06Fh, 000h, 000h
        db  0A3h, 06Ch, 06Fh, 000h, 000h
        db  08Dh, 005h, 070h, 06Fh, 000h, 000h
        db  0A3h, 070h, 06Fh, 000h, 000h
        db  0A3h, 074h, 06Fh, 000h, 000h
        db  08Dh, 005h, 078h, 06Fh, 000h, 000h
        db  0A3h, 078h, 06Fh, 000h, 000h
        db  0A3h, 07Ch, 06Fh, 000h, 000h
        db  08Dh, 005h, 088h, 06Fh, 000h, 000h
        db  0A3h, 088h, 06Fh, 000h, 000h
        db  0A3h, 08Ch, 06Fh, 000h, 000h
        db  08Dh, 005h, 090h, 06Fh, 000h, 000h
        db  0A3h, 090h, 06Fh, 000h, 000h
        db  0A3h, 094h, 06Fh, 000h, 000h
        db  08Dh, 005h, 098h, 06Fh, 000h, 000h
        db  0A3h, 098h, 06Fh, 000h, 000h
        db  0A3h, 09Ch, 06Fh, 000h, 000h
        db  08Dh, 005h, 0A0h, 06Fh, 000h, 000h
        db  0A3h, 0A0h, 06Fh, 000h, 000h
        db  0A3h, 0A4h, 06Fh, 000h, 000h
        db  08Dh, 005h, 0A8h, 06Fh, 000h, 000h
        db  0A3h, 0A8h, 06Fh, 000h, 000h
        db  0A3h, 0ACh, 06Fh, 000h, 000h
        db  08Dh, 005h, 0B0h, 06Fh, 000h, 000h
        db  0A3h, 0B0h, 06Fh, 000h, 000h
        db  0A3h, 0B4h, 06Fh, 000h, 000h
        db  08Dh, 005h, 080h, 06Fh, 000h, 000h
        db  0A3h, 080h, 06Fh, 000h, 000h
        db  0A3h, 084h, 06Fh, 000h, 000h
        db  0BFh, 0C0h, 06Fh, 000h, 000h
        mov    ecx, 100h
loc_2_0153:
        lea    eax, [edi]
        mov    dword ptr [edi], eax
        mov    dword ptr [edi + 4], eax
        add    edi, 8
        loop   loc_2_0153
        db  08Bh, 03Dh, 03Ah, 06Eh, 000h, 000h
        db  089h, 03Dh, 064h, 06Fh, 000h, 000h
        db  00Fh, 0B6h, 00Dh, 022h, 06Fh, 000h, 000h
loc_2_0172:
        lea    eax, [edi + 34h]
        mov    dword ptr [edi + 24h], eax
        mov    word ptr [edi + 1eh], 1
        mov    word ptr [edi + 10h], 2
        cli
        push   esi
        db  08Dh, 035h, 070h, 06Fh, 000h, 000h
        db  0E8h, 0ECh, 026h, 0FEh, 0FFh
        pop    esi
        sti
        add    edi, 84h
        loop   loc_2_0172
        db  089h, 03Dh, 060h, 06Fh, 000h, 000h
        db  00Fh, 0B6h, 00Dh, 023h, 06Fh, 000h, 000h
loc_2_01A8:
        mov    word ptr [edi + 10h], 8001h
        cli
        push   esi
        db  08Dh, 035h, 068h, 06Fh, 000h, 000h
        db  0E8h, 0C2h, 026h, 0FEh, 0FFh
        pop    esi
        sti
        add    edi, 3eh
        loop   loc_2_01A8
        db  00Fh, 0B6h, 00Dh, 024h, 06Fh, 000h, 000h
        jecxz  loc_2_01DF
loc_2_01CB:
        cli
        push   esi
        db  08Dh, 035h, 078h, 06Fh, 000h, 000h
        db  0E8h, 0A5h, 026h, 0FEh, 0FFh
        pop    esi
        sti
        add    edi, 70h
        loop   loc_2_01CB
loc_2_01DF:
        db  0CDh, 020h, 002h, 000h, 005h, 000h
        db  0A3h, 0E8h, 06Dh, 000h, 000h
        mov    ecx, eax
        db  0A1h, 007h, 06Fh, 000h, 000h
        sub    edx, edx
        idiv   ecx
        cmp    edx, 1
        cmc
        adc    eax, 0
        db  0A3h, 007h, 06Fh, 000h, 000h
        db  0A1h, 00Bh, 06Fh, 000h, 000h
        sub    edx, edx
        idiv   ecx
        db  0A3h, 00Bh, 06Fh, 000h, 000h
        cmp    edx, 1
        cmc
        adc    eax, 0
        db  0A1h, 00Fh, 06Fh, 000h, 000h
        sub    edx, edx
        idiv   ecx
        cmp    edx, 1
        cmc
        adc    eax, 0
        db  0A3h, 00Fh, 06Fh, 000h, 000h
        db  0A1h, 00Bh, 06Fh, 000h, 000h
        db  00Fh, 0B6h, 015h, 026h, 06Fh, 000h, 000h
        db  088h, 015h, 02Dh, 06Fh, 000h, 000h
        test   edx, edx
        je     SHORT loc_2_0243
        mul    edx
loc_2_0243:
        mov    edx, eax
        shr    eax, 1
        or     al, 1
        add    eax, edx
        db  08Bh, 015h, 027h, 06Fh, 000h, 000h
        test   edx, edx
        je     SHORT loc_2_0259
        cmp    eax, edx
        ja     SHORT loc_2_025B
loc_2_0259:
        mov    eax, edx
loc_2_025B:
        sub    edx, edx
        idiv   ecx
        cmp    edx, 1
        cmc
        adc    eax, 0
        db  0A3h, 013h, 06Fh, 000h, 000h
        db  0A0h, 02Eh, 06Fh, 000h, 000h
        shr    al, 1
        or     al, 1
        db  0A2h, 02Fh, 06Fh, 000h, 000h
        db  0A1h, 01Bh, 06Fh, 000h, 000h
        sub    edx, edx
        idiv   ecx
        cmp    edx, 1
        cmc
        adc    eax, 0
        db  0A3h, 01Bh, 06Fh, 000h, 000h
        mov    eax, 7530h
        sub    edx, edx
        idiv   ecx
        cmp    edx, 1
        cmc
        adc    eax, 0
        db  0A3h, 017h, 06Fh, 000h, 000h
        mov    eax, 1f4h
        sub    edx, edx
        idiv   ecx
        cmp    edx, 1
        cmc
        adc    eax, 0
        db  0A3h, 0C8h, 06Eh, 000h, 000h
        db  0A3h, 0C4h, 06Eh, 000h, 000h
        mov    eax, 3e8h
        sub    edx, edx
        idiv   ecx
        cmp    edx, 1
        cmc
        adc    eax, 0
        db  0A3h, 0D0h, 06Eh, 000h, 000h
        db  0A3h, 0CCh, 06Eh, 000h, 000h
        db  0A0h, 031h, 06Fh, 000h, 000h
        db  03Ah, 005h, 030h, 06Fh, 000h, 000h
        jbe    SHORT loc_2_02F0
        db  0A0h, 030h, 06Fh, 000h, 000h
        shr    al, 1
        db  0A2h, 031h, 06Fh, 000h, 000h
loc_2_02F0:
        clc
        ret
sub_2_02F2:
        db  0A1h, 0E8h, 06Dh, 000h, 000h
        db  0BEh, 054h, 036h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 038h, 06Fh, 000h, 000h
        cmp    esi, 1
        ret
sub_2_030C:
        enter  18h, 0
        push   edi
        push   esi
        db  08Dh, 03Dh, 048h, 06Eh, 000h, 000h
        db  08Dh, 035h, 0E0h, 06Dh, 000h, 000h
        mov    word ptr [ebp - 8], 8
        mov    word ptr [ebp - 6], 8
        mov    dword ptr [ebp - 4], esi
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        push   edi
        db  0E8h, 0FFh, 068h, 0FEh, 0FFh
        cmp    dword ptr [edi], 0
        je     SHORT loc_2_0346
loc_2_0340:
        stc
        jmp    NEAR PTR loc_2_0413
loc_2_0346:
        cld
        lea    ebx, [ebp - 0ch]
        db  0BAh, 03Eh, 004h, 000h, 000h
loc_2_034F:
        add    edx, 3eh
        db  081h, 0FAh, 08Ah, 00Ah, 000h, 000h
        jae    NEAR PTR loc_2_040A
        mov    ax, word ptr [edx + 20h]
        mov    word ptr [ebp - 8], ax
        mov    word ptr [ebp - 6], ax
        lea    eax, [edx]
        mov    dword ptr [ebp - 4], eax
        push   edx
        push   dword ptr [edx + 22h]
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp - 10h]
        push   ebx
        db  068h, 048h, 06Eh, 000h, 000h
        db  0E8h, 075h, 069h, 0FEh, 0FFh
        pop    edx
        db  083h, 03Dh, 048h, 06Eh, 000h, 000h, 000h
        jne    SHORT loc_2_03B9
        cmp    dword ptr [edx + 22h], 3
        je     SHORT loc_2_03BE
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 4]
        lea    esi, [edx + 2eh]
        cmp    eax, dword ptr [esi]
        jae    SHORT loc_2_03AF
        lea    esi, [edx + 2ah]
        cmp    eax, dword ptr [esi]
        jb     SHORT loc_2_03AF
        mov    esi, dword ptr [ebp - 0ch]
        lea    esi, [esi + 4]
loc_2_03AF:
        mov    edi, dword ptr [edx + 3ah]
        mov    ecx, dword ptr [edx + 36h]
        rep movsb
        jmp    SHORT loc_2_034F
loc_2_03B9:
        lea    esi, [edx + 32h]
        jmp    SHORT loc_2_03AF
loc_2_03BE:
        mov    esi, dword ptr [ebp - 0ch]
        movzx  ecx, word ptr [esi + 4]
        push   edx
        mov    eax, ecx
        db  0E8h, 066h, 01Eh, 0FEh, 0FFh
        pop    edx
        movzx  ecx, word ptr [esi + 4]
        mov    esi, dword ptr [esi + 8]
        mov    edi, eax
        test   eax, eax
        je     NEAR PTR loc_2_0340
        push   ecx
        rep movsb
        pop    ecx
        mov    edi, eax
        mov    esi, dword ptr [edx + 3ah]
        mov    eax, dword ptr [edx + 36h]
loc_2_03EB:
        cmp    byte ptr [edi], 0
        je     NEAR PTR loc_2_034F
        mov    dword ptr [esi], edi
        add    esi, 4
        sub    eax, 4
        je     NEAR PTR loc_2_034F
        push   eax
        xor    al, al
        repne scasb
        pop    eax
        jmp    SHORT loc_2_03EB
loc_2_040A:
        push   dword ptr [ebp - 10h]
        db  0E8h, 0EEh, 068h, 0FEh, 0FFh
        clc
loc_2_0413:
        pop    esi
        pop    edi
        leave
        ret
sub_2_0417:
        enter  4, 0
        push   42h
        db  068h, 0ECh, 06Dh, 000h, 000h
        db  068h, 0B4h, 06Dh, 000h, 000h
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 0D4h, 067h, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 4]
        cmp    eax, 0
        je     SHORT loc_2_0439
        stc
loc_2_0439:
        leave
        ret
sub_2_043B:
        call   sub_2_0417
        jb     SHORT loc_2_0471
        call   sub_2_030C
        jb     SHORT loc_2_0473
        call   sub_2_0058
        jb     SHORT loc_2_0475
        call   sub_2_009F
        jb     SHORT loc_2_0477
        call   sub_2_02F2
        jb     SHORT loc_2_0479
        ret
loc_2_045F:
        db  0FFh, 035h, 0B4h, 06Dh, 000h, 000h
        db  068h, 048h, 06Eh, 000h, 000h
        db  0E8h, 09Bh, 067h, 0FEh, 0FFh
        stc
        ret
loc_2_0471:
        stc
        ret
loc_2_0473:
        jmp    SHORT loc_2_045F
loc_2_0475:
        jmp    SHORT loc_2_045F
loc_2_0477:
        jmp    SHORT loc_2_045F
loc_2_0479:
        jmp    SHORT loc_2_045F
        db  000h, 050h, 049h, 047h, 047h, 059h, 042h, 041h, 043h, 04Bh, 041h, 043h, 04Bh, 053h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 00Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h
        db  000h, 000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 032h, 06Fh, 000h, 000h, 042h
        db  049h, 04Eh, 044h, 049h, 04Eh, 047h, 053h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 009h
        db  000h, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 020h, 000h, 000h, 000h, 0B8h, 06Dh, 000h, 000h, 053h, 045h, 053h
        db  053h, 049h, 04Fh, 04Eh, 053h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 009h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 0FEh, 000h, 000h, 000h, 006h
        db  000h, 000h, 000h, 001h, 000h, 000h, 000h, 0F0h, 06Eh, 000h, 000h, 04Eh, 041h, 04Dh, 045h, 043h
        db  041h, 043h, 048h, 045h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 001h, 000h, 000h, 000h, 025h, 06Fh, 000h, 000h, 04Eh, 041h, 04Dh, 045h, 053h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 006h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 001h, 000h, 000h, 000h, 0FEh, 000h, 000h, 000h, 011h, 000h, 000h, 000h, 001h
        db  000h, 000h, 000h, 0F1h, 06Eh, 000h, 000h, 050h, 041h, 043h, 04Bh, 045h, 054h, 053h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 008h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 001h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 032h, 000h, 000h, 000h, 001h, 000h, 000h
        db  000h, 023h, 06Fh, 000h, 000h, 04Ch, 04Fh, 04Fh, 050h, 050h, 041h, 043h, 04Bh, 045h, 054h, 053h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 003h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 024h
        db  06Fh, 000h, 000h, 044h, 041h, 054h, 041h, 047h, 052h, 041h, 04Dh, 050h, 041h, 043h, 04Bh, 045h
        db  054h, 053h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 010h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 000h
        db  000h, 0FFh, 0FFh, 0FFh, 0FFh, 014h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 022h, 06Fh, 000h
        db  000h, 054h, 049h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh
        db  0FFh, 0FFh, 0FFh, 030h, 075h, 000h, 000h, 004h, 000h, 000h, 000h, 007h, 06Fh, 000h, 000h, 054h
        db  031h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 003h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh
        db  0FFh, 0F4h, 001h, 000h, 000h, 004h, 000h, 000h, 000h, 00Bh, 06Fh, 000h, 000h, 054h, 032h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 003h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 0C8h
        db  000h, 000h, 000h, 004h, 000h, 000h, 000h, 00Fh, 06Fh, 000h, 000h, 041h, 044h, 041h, 050h, 054h
        db  052h, 041h, 054h, 045h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 0E8h, 003h, 000h
        db  000h, 004h, 000h, 000h, 000h, 027h, 06Fh, 000h, 000h, 057h, 049h, 04Eh, 044h, 04Fh, 057h, 045h
        db  052h, 052h, 04Fh, 052h, 053h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Dh, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 001h, 000h, 000h, 000h, 001h
        db  000h, 000h, 000h, 026h, 06Fh, 000h, 000h, 04Eh, 045h, 054h, 042h, 049h, 04Fh, 053h, 054h, 049h
        db  04Dh, 045h, 04Fh, 055h, 054h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 0F4h, 001h, 000h, 000h, 004h, 000h, 000h
        db  000h, 01Bh, 06Fh, 000h, 000h, 04Eh, 045h, 054h, 042h, 049h, 04Fh, 053h, 052h, 045h, 054h, 052h
        db  049h, 045h, 053h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h
        db  000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 002h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 01Fh
        db  06Fh, 000h, 000h, 057h, 041h, 04Eh, 052h, 045h, 054h, 052h, 049h, 045h, 053h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 00Bh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h
        db  000h, 0FFh, 0FFh, 0FFh, 0FFh, 002h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 021h, 06Fh, 000h
        db  000h, 044h, 04Ch, 043h, 052h, 045h, 054h, 052h, 049h, 045h, 053h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 00Bh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 0FFh
        db  0FFh, 0FFh, 0FFh, 005h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 020h, 06Fh, 000h, 000h, 04Dh
        db  041h, 058h, 049h, 04Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 006h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 07Fh, 000h, 000h
        db  000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 02Bh, 06Fh, 000h, 000h, 04Dh, 041h, 058h
        db  04Fh, 055h, 054h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 007h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 07Fh, 000h, 000h, 000h, 001h
        db  000h, 000h, 000h, 001h, 000h, 000h, 000h, 02Ch, 06Fh, 000h, 000h, 050h, 049h, 050h, 045h, 04Ch
        db  049h, 04Eh, 045h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 009h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 014h, 000h, 000h
        db  000h, 001h, 000h, 000h, 000h, 02Eh, 06Fh, 000h, 000h, 04Dh, 041h, 058h, 054h, 052h, 041h, 04Eh
        db  053h, 04Dh, 049h, 054h, 053h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Dh, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 001h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 005h, 000h, 000h, 000h, 001h
        db  000h, 000h, 000h, 030h, 06Fh, 000h, 000h, 04Dh, 049h, 04Eh, 054h, 052h, 041h, 04Eh, 053h, 04Dh
        db  049h, 054h, 053h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Dh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 002h, 000h, 000h, 000h, 001h, 000h, 000h
        db  000h, 031h, 06Fh, 000h, 000h, 04Ch, 041h, 04Eh, 041h, 042h, 041h, 053h, 045h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 009h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 01Fh, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 033h
        db  06Fh, 000h, 000h, 055h, 053h, 045h, 043h, 053h, 041h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 007h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 035h, 06Fh, 000h
        db  000h, 051h, 075h, 065h, 072h, 079h, 041h, 06Ch, 077h, 061h, 079h, 073h, 055h, 073h, 065h, 073h
        db  053h, 06Fh, 075h, 072h, 063h, 065h, 052h, 06Fh, 075h, 074h, 069h, 06Eh, 067h, 000h, 000h, 000h
        db  000h, 01Dh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 036h, 06Fh, 000h, 000h, 000h
        db  000h
_ITEXT ends
        end
