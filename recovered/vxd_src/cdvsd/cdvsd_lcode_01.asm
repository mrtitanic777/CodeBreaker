; cdvsd_lcode_01.asm -- CDVSD object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

CDVSD_Control:
        cmp    eax, 1bh
        db  00Fh, 084h, 0F7h, 0FFh, 0FEh, 0FFh
        cmp    eax, 32h
        je     NEAR PTR loc_1_0266
        clc
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0020:
        mov    ebx, dword ptr [esp + 4]
        mov    word ptr [ebx + 2], 0
        mov    si, word ptr [ebx]
        cmp    si, 3
        db  00Fh, 084h, 029h, 000h, 0FFh, 0FFh
        cmp    si, 4
        db  00Fh, 084h, 030h, 002h, 0FFh, 0FFh
        cmp    si, 2
        db  00Fh, 084h, 076h, 002h, 0FFh, 0FFh
        cmp    si, 9
        je     SHORT loc_1_0079
        cmp    si, 15h
        je     SHORT loc_1_005E
        mov    word ptr [ebx + 2], 0ffffh
        ret
loc_1_005E:
        mov    esi, dword ptr [ebx + 0ch]
        db  08Bh, 00Dh, 014h, 000h, 000h, 000h
loc_1_0067:
        or     ecx, ecx
        je     SHORT loc_1_0078
        cmp    dword ptr [ecx + 4], esi
        je     SHORT loc_1_0074
        mov    ecx, dword ptr [ecx]
        jmp    SHORT loc_1_0067
loc_1_0074:
        or     byte ptr [ecx + 8], 40h
loc_1_0078:
        ret
loc_1_0079:
        cli
        db  08Bh, 015h, 014h, 000h, 000h, 000h
        or     edx, edx
        je     SHORT loc_1_00B9
        db  0F6h, 005h, 018h, 000h, 000h, 000h, 002h
        je     SHORT loc_1_00B9
        db  0F6h, 005h, 018h, 000h, 000h, 000h, 001h
        jne    SHORT loc_1_00B9
        bts    dword ptr [edx + 8], 2
        jb     SHORT loc_1_00B9
        bts    dword ptr [edx + 8], 5
        push   esi
        db  0BEh, 0C6h, 001h, 000h, 000h
        and    byte ptr [edx + 8], 0f7h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        db  089h, 035h, 01Ch, 000h, 000h, 000h
        pop    esi
loc_1_00B9:
        sti
        ret
loc_1_00BB:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [edi + 8]
        db  0F6h, 005h, 018h, 000h, 000h, 000h, 001h
        jne    NEAR PTR loc_1_01AF
        mov    edx, dword ptr [edi + 90h]
        cmp    word ptr [edi + 6ah], 17h
        je     SHORT loc_1_0123
        cmp    word ptr [edi + 6ah], 10h
        jae    NEAR PTR loc_1_016E
        bts    dword ptr [edx + 8], 1
        jb     SHORT loc_1_016E
        cmp    dword ptr [ebx + 0a6h], 63656b6dh
        jne    SHORT loc_1_015B
        cmp    dword ptr [ebx + 0aah], 78783572h
        jne    SHORT loc_1_015B
        push   esi
        push   edx
        db  0BEh, 0B4h, 001h, 000h, 000h
        mov    edx, ebx
        mov    eax, 1388h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        pop    edx
        pop    esi
        jmp    SHORT loc_1_016E
loc_1_0123:
        push   edx
        cmp    dword ptr [ebx + 70h], 0
        je     SHORT loc_1_014E
        lea    edx, [ebp - 4]
        push   0
        push   dword ptr [ebx + 70h]
        push   edx
        db  0CDh, 020h, 003h, 000h, 033h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_014E
        push   0
        push   dword ptr [ebp - 4]
        db  0CDh, 020h, 033h, 000h, 033h, 000h
        add    esp, 8
loc_1_014E:
        pop    edx
        or     byte ptr [edx + 8], 40h
        mov    word ptr [edi + 6ah], 0
        jmp    SHORT loc_1_016E
loc_1_015B:
        push   edx
        push   0
        push   ebx
        db  068h, 023h, 000h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 017h, 000h
        add    esp, 0ch
        pop    edx
loc_1_016E:
        push   word ptr [edi + 6ah]
        push   edx
        call   sub_1_0CF0
        pop    edx
        pop    ax
        cmp    ax, 10h
        jb     SHORT loc_1_0188
        bts    dword ptr [edx + 8], 3
        jae    SHORT loc_1_019D
loc_1_0188:
        cli
        btr    dword ptr [edx + 8], 5
        mov    edx, dword ptr [edx]
        or     edx, edx
        je     SHORT loc_1_019D
        bts    dword ptr [edx + 8], 5
        and    byte ptr [edx + 8], 0f7h
loc_1_019D:
        sti
        db  0BEh, 0C6h, 001h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        db  089h, 035h, 01Ch, 000h, 000h, 000h
loc_1_01AF:
        pop    ebx
        pop    esi
        pop    edi
        leave
        ret
loc_1_01B4:
        push   0
        push   edx
        db  068h, 023h, 000h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 017h, 000h
        add    esp, 0ch
        ret
loc_1_01C6:
        cli
        xor    esi, esi
        db  087h, 035h, 01Ch, 000h, 000h, 000h
        sti
        or     esi, esi
        je     SHORT loc_1_0241
loc_1_01D4:
        or     edx, edx
        je     SHORT loc_1_0242
        db  0F6h, 005h, 018h, 000h, 000h, 000h, 001h
        jne    SHORT loc_1_0242
        bts    dword ptr [edx + 8], 5
        test   byte ptr [edx + 8], 50h
        jne    SHORT loc_1_0252
        test   byte ptr [edx + 8], 1
        je     SHORT loc_1_024E
        btr    dword ptr [edx + 8], 7
        jb     SHORT loc_1_024E
        mov    esi, dword ptr [edx + 4]
        cmp    byte ptr [esi + 76h], 0
        jne    SHORT loc_1_024E
        push   edx
        call   sub_1_0D0B
        pop    edx
        or     edi, edi
        je     SHORT loc_1_024E
        mov    word ptr [edi + 68h], 6
        mov    dword ptr [edi + 7ch], 0
        and    word ptr [edi + 6ch], 77f7h
        or     byte ptr [edi + 6dh], 40h
        db  0C7h, 047h, 070h, 0BBh, 000h, 000h, 000h
        mov    dword ptr [edi + 0a4h], edi
        mov    dword ptr [edi + 90h], edx
        mov    ebx, esi
        xor    edx, edx
        db  0FFh, 015h, 060h, 00Fh, 000h, 000h
loc_1_0241:
        ret
loc_1_0242:
        db  08Bh, 015h, 014h, 000h, 000h, 000h
        btr    dword ptr [edx + 8], 2
        ret
loc_1_024E:
        mov    edx, dword ptr [edx]
        jmp    SHORT loc_1_01D4
loc_1_0252:
        test   byte ptr [edx + 8], 10h
        je     SHORT loc_1_0242
        push   0
        push   edx
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        jmp    SHORT loc_1_0242
loc_1_0266:
        cli
        db  080h, 00Dh, 018h, 000h, 000h, 000h, 001h
        db  08Bh, 015h, 014h, 000h, 000h, 000h
loc_1_0274:
        or     edx, edx
        je     SHORT loc_1_0283
        bt     dword ptr [edx + 8], 5
        jb     SHORT loc_1_0294
        mov    edx, dword ptr [edx]
        jmp    SHORT loc_1_0274
loc_1_0283:
        xor    esi, esi
        db  087h, 035h, 01Ch, 000h, 000h, 000h
        db  0CDh, 020h, 012h, 000h, 001h, 000h
        sti
        clc
        ret
loc_1_0294:
        sti
        mov    esi, dword ptr [edx + 4]
        push   edx
        call   sub_1_0D0B
        pop    edx
        or     edi, edi
        je     SHORT loc_1_0283
        mov    word ptr [edi + 68h], 10h
        mov    dword ptr [edi + 7ch], 0
        and    word ptr [edi + 6ch], 77f7h
        or     byte ptr [edi + 6dh], 41h
        mov    dword ptr [edi + 0a4h], edi
        mov    ebx, esi
        xor    edx, edx
        db  0FFh, 015h, 060h, 00Fh, 000h, 000h
        call   sub_1_0CF0
        jmp    SHORT loc_1_0283
        db  000h, 000h, 000h
sub_1_02D4:
        push   edi
        push   esi
        push   ebx
        mov    esi, dword ptr [esp + 10h]
        mov    eax, dword ptr [esi + 14h]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esi + 10h], eax
        test   byte ptr [esi + 6dh], 80h
        je     SHORT loc_1_0308
        mov    edi, dword ptr [esi + 20h]
        mov    dword ptr [edi + 44h], 0
        cmp    byte ptr [edi + 3], 1
        jne    SHORT loc_1_0311
        mov    word ptr [esi + 6ah], 0
        mov    edx, dword ptr [eax + 8]
        bts    dword ptr [edx + 8], 7
loc_1_0308:
        call   sub_1_038D
loc_1_030D:
        pop    ebx
        pop    esi
        pop    edi
        ret
loc_1_0311:
        call   sub_1_0418
        or     al, al
        je     SHORT loc_1_0308
        cmp    al, 2
        je     SHORT loc_1_030D
        mov    edi, dword ptr [esi + 20h]
        mov    ebx, dword ptr [esi + 8]
        mov    eax, dword ptr [esi + 7ch]
        test   dword ptr [esi + 6ch], 200h
        jne    SHORT loc_1_0336
        mul    dword ptr [ebx + 0dbh]
loc_1_0336:
        mov    dword ptr [edi + 10h], eax
        add    dword ptr [esi + 14h], 8
        mov    eax, dword ptr [esi + 10h]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [esi + 10h], eax
        push   esi
        call   dword ptr [eax]
        add    esp, 4
        jmp    SHORT loc_1_030D
loc_1_034E:
        push   edi
        push   esi
        push   ebx
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esi + 20h]
        test   byte ptr [edi + 3], 80h
        je     SHORT loc_1_0377
        mov    eax, dword ptr [edi + 1ch]
        mov    al, byte ptr [eax + 0ch]
        cmp    al, 28h
        jb     SHORT loc_1_0377
        cmp    al, 29h
        jbe    SHORT loc_1_0370
        cmp    al, 3ah
        jne    SHORT loc_1_0377
loc_1_0370:
        mov    edi, dword ptr [esi + 8]
        or     byte ptr [edi + 22h], 8
loc_1_0377:
        mov    edi, dword ptr [esi + 14h]
        sub    edi, 8
        mov    dword ptr [esi + 14h], edi
        mov    ebx, dword ptr [esi + 4]
        push   esi
        call   dword ptr [edi]
        add    esp, 4
        pop    ebx
        pop    esi
        pop    edi
        ret
sub_1_038D:
        cmp    word ptr [esi + 6ah], 19h
        je     SHORT loc_1_03E1
        cmp    word ptr [esi + 6ah], 18h
        je     SHORT loc_1_03E1
loc_1_039B:
        push   ebx
        cmp    word ptr [esi + 68h], 12h
        jne    SHORT loc_1_03A9
        mov    word ptr [esi + 6ah], 0
loc_1_03A9:
        mov    edi, dword ptr [esi + 14h]
        sub    edi, 8
        mov    dword ptr [esi + 14h], edi
        mov    ebx, dword ptr [esi + 4]
        push   esi
        call   dword ptr [edi]
        add    esp, 4
        xchg   ebx, esi
        cli
        cmp    dword ptr [esi + 49ch], 0
        je     SHORT loc_1_03D9
        cmp    dword ptr [esi + 49ch], ebx
        jne    SHORT loc_1_03DE
        mov    dword ptr [esi + 49ch], 0
loc_1_03D9:
        call   sub_1_0EFF
loc_1_03DE:
        sti
        pop    ebx
        ret
loc_1_03E1:
        mov    edx, dword ptr [esi + 10h]
        mov    edx, dword ptr [edx + 8]
        btr    dword ptr [edx + 8], 1
        jae    SHORT loc_1_039B
        mov    ebx, dword ptr [esi + 8]
        push   0
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 017h, 000h
        add    esp, 0ch
        jmp    SHORT loc_1_039B
sub_1_0404:
        mov    edx, dword ptr [esp + 4]
        push   esi
        db  0BEh, 0E8h, 002h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        pop    esi
        ret
        db  000h, 000h
sub_1_0418:
        mov    ebx, dword ptr [esi + 14h]
        mov    ebx, dword ptr [ebx + 4]
        mov    dword ptr [esi + 10h], ebx
        mov    ebx, dword ptr [esi + 4]
        test   byte ptr [edi + 3], 80h
        jne    NEAR PTR loc_1_04BD
        cmp    byte ptr [edi + 3], 0ah
        je     NEAR PTR loc_1_04F8
        cmp    byte ptr [edi + 3], 4
        jne    SHORT loc_1_0450
        cmp    byte ptr [edi + 4], 2
        je     SHORT loc_1_045F
        cmp    byte ptr [edi + 4], 8
        je     SHORT loc_1_0450
        cmp    byte ptr [edi + 4], 18h
        je     SHORT loc_1_04AB
loc_1_0450:
        cmp    byte ptr [edi + 4ch], 5
        jb     SHORT loc_1_04AB
loc_1_0456:
        mov    word ptr [esi + 6ah], 1ah
loc_1_045C:
        xor    al, al
        ret
loc_1_045F:
        cmp    byte ptr [edi + 30h], 3
        je     SHORT loc_1_0456
        mov    edx, edi
        db  0BEh, 075h, 004h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        mov    al, 2
        ret
loc_1_0475:
        mov    edi, edx
        mov    esi, dword ptr [edi + 40h]
        mov    ebx, dword ptr [esi + 4]
        mov    ax, 0bh
        mov    ecx, dword ptr [edi + 1ch]
        mov    edx, 0eh
        call   sub_1_0DDA
        or     eax, eax
        jne    SHORT loc_1_04EF
        or     byte ptr [edi + 3], 80h
        push   esi
        call   sub_1_02D4
        add    esp, 4
        ret
loc_1_04A0:
        cmp    byte ptr [edi + 4ch], 5
        jb     SHORT loc_1_04AB
        mov    al, byte ptr [ecx + 2]
        jmp    SHORT loc_1_0501
loc_1_04AB:
        inc    byte ptr [edi + 4ch]
        mov    byte ptr [edi + 3], 0
        mov    ax, word ptr [esi + 0eh]
        mov    word ptr [esi + 0ch], ax
        mov    al, 1
        ret
loc_1_04BD:
        mov    ecx, dword ptr [edi + 1ch]
        mov    al, byte ptr [ecx + 2]
        and    eax, 0fh
        cmp    al, 0
        je     SHORT loc_1_04A0
        cmp    al, 5
        je     SHORT loc_1_0501
        cmp    al, 1
        jne    SHORT loc_1_04DB
        mov    word ptr [esi + 6ah], 2
        xor    al, al
        ret
loc_1_04DB:
        cmp    byte ptr [ecx + 0ch], 45h
        je     SHORT loc_1_04F8
        cmp    al, 3
        je     SHORT loc_1_04A0
        cmp    al, 4
        je     SHORT loc_1_04A0
        cmp    al, 0bh
        je     SHORT loc_1_04A0
        jmp    SHORT loc_1_0501
loc_1_04EF:
        mov    word ptr [esi + 6ah], 20h
        xor    al, al
        ret
loc_1_04F8:
        mov    word ptr [esi + 6ah], 17h
        xor    al, al
        ret
loc_1_0501:
        mov    cl, byte ptr [ecx + 0ch]
        cmp    cl, 64h
        je     SHORT loc_1_0529
        cmp    cl, 3ah
        je     SHORT loc_1_052F
        cmp    cl, 24h
        jne    SHORT loc_1_0519
        cmp    byte ptr [edi + 30h], 28h
        je     SHORT loc_1_0529
loc_1_0519:
        db  00Fh, 0B6h, 080h, 035h, 005h, 000h, 000h
loc_1_0520:
        mov    word ptr [esi + 6ah], ax
        jmp    NEAR PTR loc_1_045C
loc_1_0529:
        mov    ax, 28h
        jmp    SHORT loc_1_0520
loc_1_052F:
        mov    ax, 18h
        jmp    SHORT loc_1_0520
        db  000h, 000h, 020h, 014h, 01Bh, 016h, 019h, 01Fh, 01Ah, 01Ah, 01Ah, 01Ah, 01Ah, 01Ah, 01Ah, 01Ah
        db  000h, 000h, 000h
loc_1_0548:
        push   ebp
        push   edi
        push   esi
        mov    ebx, dword ptr [esp + 10h]
        mov    esi, dword ptr [ebx + 4]
        mov    edx, dword ptr [ebx + 10h]
        mov    eax, dword ptr [ebx + 14h]
        db  0C7h, 000h, 0D4h, 002h, 000h, 000h
        mov    dword ptr [eax + 4], edx
        add    dword ptr [ebx + 14h], 8
        cmp    word ptr [ebx + 68h], 0fh
        je     NEAR PTR loc_1_060E
        test   byte ptr [ebx + 6dh], 8
        jne    SHORT loc_1_0596
        cli
        lea    eax, [esi + 43ch]
        xor    ecx, ecx
loc_1_057F:
        cmp    dword ptr [eax + 64h], ecx
        je     SHORT loc_1_0589
        mov    eax, dword ptr [eax + 64h]
        jmp    SHORT loc_1_057F
loc_1_0589:
        mov    dword ptr [eax + 64h], ebx
        mov    dword ptr [ebx + 64h], ecx
        call   sub_1_0EFF
        jmp    SHORT loc_1_060A
loc_1_0596:
        cmp    word ptr [ebx + 68h], 12h
        je     SHORT loc_1_0616
        call   sub_1_0E67
        movzx  eax, word ptr [ebx + 68h]
        cmp    eax, 0eh
        jae    NEAR PTR loc_1_0673
        db  0FFh, 014h, 085h, 03Bh, 006h, 000h, 000h
loc_1_05B8:
        or     al, al
        jne    NEAR PTR loc_1_0678
        mov    edi, dword ptr [ebx + 20h]
        mov    eax, dword ptr [ebx + 80h]
        mov    dword ptr [edi + 18h], eax
        mov    eax, dword ptr [ebx + 7ch]
        test   dword ptr [ebx + 6ch], 200h
        jne    SHORT loc_1_05DE
        mul    dword ptr [esi + 0dbh]
loc_1_05DE:
        mov    dword ptr [edi + 10h], eax
loc_1_05E1:
        mov    edi, dword ptr [ebx + 20h]
        or     dword ptr [ebx + 6ch], 8000h
        mov    dword ptr [edi + 44h], 0
loc_1_05F2:
        mov    eax, dword ptr [ebx + 10h]
        mov    edx, dword ptr [eax + 8]
        test   byte ptr [edx + 9], 2
        jne    SHORT loc_1_062D
loc_1_05FE:
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebx + 10h], eax
        push   ebx
        call   dword ptr [eax]
        add    esp, 4
loc_1_060A:
        pop    esi
        pop    edi
        pop    ebp
        ret
loc_1_060E:
        db  0C7h, 000h, 04Eh, 003h, 000h, 000h
        jmp    SHORT loc_1_05E1
loc_1_0616:
        mov    edi, dword ptr [ebx + 20h]
        cmp    byte ptr [edi + 30h], 43h
        jne    SHORT loc_1_05E1
        mov    word ptr [ebx + 0eh], 5ah
        mov    word ptr [ebx + 0ch], 5ah
        jmp    SHORT loc_1_05E1
loc_1_062D:
        mov    word ptr [ebx + 0eh], 3ch
        mov    word ptr [ebx + 0ch], 3ch
        jmp    SHORT loc_1_05FE
sub_1_063B:
        rol    dword ptr [esi], 0
        db  000h, 05Fh, 008h
        add    byte ptr [eax], al
        pop    edi
        or     byte ptr [eax], al
        db  000h, 05Fh, 008h
        add    byte ptr [eax], al
        pop    edi
        or     byte ptr [eax], al
        db  000h, 006h
        or     byte ptr [eax], al
        db  000h, 006h
        or     byte ptr [eax], al
        db  000h, 072h, 00Dh
        add    byte ptr [eax], al
        db  076h, 00Dh
        add    byte ptr [eax], al
        xor    al, 8
        add    byte ptr [eax], al
        cmp    byte ptr [eax], cl
        add    byte ptr [eax], al
        mov    al, 0dh
        add    byte ptr [eax], al
        db  068h, 008h, 000h, 000h, 088h
        sldt   word ptr [eax]
loc_1_0673:
        jmp    NEAR PTR loc_1_05F2
loc_1_0678:
        dec    al
        jne    SHORT loc_1_0691
        mov    eax, dword ptr [ebx + 14h]
        sub    eax, 8
        mov    dword ptr [ebx + 14h], eax
        mov    esi, ebx
        call   sub_1_038D
        jmp    NEAR PTR loc_1_060A
loc_1_0691:
        dec    al
        je     NEAR PTR loc_1_060A
        cmp    al, 2
        je     NEAR PTR loc_1_05F2
        mov    word ptr [ebx + 6ah], 16h
        cmp    word ptr [ebx + 68h], 0dh
        jne    NEAR PTR loc_1_060A
        mov    dword ptr [ebx + 0a0h], 1fh
        jmp    NEAR PTR loc_1_060A
loc_1_06C1:
        cmp    dword ptr [ebx + 74h], 10h
        jb     NEAR PTR loc_1_0757
        mov    dword ptr [edi + 0ch], 248h
        mov    byte ptr [edi + 0ah], 0ah
        mov    byte ptr [edi + 30h], 28h
        mov    al, byte ptr [esi + 7dh]
        shl    al, 5
        mov    byte ptr [edi + 31h], al
        mov    eax, dword ptr [ebx + 74h]
        xchg   al, ah
        ror    eax, 10h
        xchg   al, ah
        mov    dword ptr [edi + 32h], eax
        mov    eax, dword ptr [ebx + 7ch]
        test   byte ptr [ebx + 6eh], 4
        db  075h, 034h
        mov    byte ptr [edi + 36h], 0
        mov    byte ptr [edi + 39h], 0
        test   byte ptr [ebx + 6dh], 2
        jne    SHORT loc_1_071D
        xchg   al, ah
        mov    word ptr [edi + 37h], ax
        mov    eax, 800h
loc_1_0712:
        cmp    eax, dword ptr [esi + 490h]
        jne    SHORT loc_1_0769
        xor    al, al
        ret
loc_1_071D:
        mov    word ptr [edi + 37h], 100h
        jmp    SHORT loc_1_0712
loc_1_0725:
        pushad
        add    byte ptr [eax + 30094009h], dl
        or     dword ptr [edi - 73h], edx
        db  03Dh, 025h, 007h, 000h, 000h
        mov    ecx, 4
        repne scasw
        pop    edi
        jne    SHORT loc_1_0757
        mov    byte ptr [edi + 3ah], cl
        mov    dword ptr [edi + 36h], 1000000h
        mov    byte ptr [edi + 0ah], 0ch
        mov    byte ptr [edi + 30h], 0d8h
        xor    al, al
        mov    byte ptr [edi + 3bh], al
        ret
loc_1_0757:
        mov    word ptr [ebx + 6ah], 10h
        mov    al, 1
        ret
loc_1_0760:
        mov    word ptr [ebx + 6ah], 21h
        mov    al, 1
        ret
loc_1_0769:
        call   sub_1_07CD
        jb     SHORT loc_1_0760
        mov    edx, ebx
        push   esi
        db  0BEh, 0FCh, 002h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        pop    esi
        mov    al, 2
        ret
loc_1_0782:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [edi + 4]
        mov    esi, dword ptr [edi + 20h]
        cmp    byte ptr [esi + 3], 1
        pushfd
        push   dword ptr [edi + 90h]
        call   sub_1_0CF0
        pop    ebx
        mov    edi, dword ptr [ebx + 20h]
        mov    esi, dword ptr [ebx + 8]
        popfd
        jne    SHORT loc_1_07C0
        movzx  ecx, word ptr [esi + 466h]
        xchg   ch, cl
        mov    dword ptr [esi + 490h], ecx
        xor    al, al
        jmp    NEAR PTR loc_1_05B8
loc_1_07C0:
        mov    word ptr [ebx + 6ah], 28h
        mov    al, 1
        jmp    NEAR PTR loc_1_05B8
sub_1_07CD:
        push   ebx
        push   dword ptr [ebx + 10h]
        call   sub_1_0D0B
        pop    eax
        or     edi, edi
        stc
        je     SHORT loc_1_0805
        mov    dword ptr [edi + 10h], eax
        mov    dword ptr [edi + 0a4h], edi
        mov    word ptr [edi + 68h], 12h
        mov    dword ptr [edi + 8], esi
        mov    ebx, edi
        call   sub_1_0E67
        pop    dword ptr [ebx + 90h]
        mov    al, byte ptr [esi + 7dh]
        shl    al, 5
        mov    byte ptr [edi + 31h], al
        clc
loc_1_0805:
        ret
sub_1_0806:
        mov    dword ptr [edi + 0ch], 0
        mov    byte ptr [edi + 0ah], 6
        mov    byte ptr [edi + 30h], 0
        mov    al, byte ptr [esi + 7dh]
        shl    al, 5
        mov    byte ptr [edi + 31h], al
        mov    dword ptr [edi + 32h], 0
        mov    word ptr [ebx + 0eh], 3ch
        mov    word ptr [ebx + 0ch], 3ch
        xor    al, al
        ret
loc_1_0834:
        mov    al, 1
        jmp    SHORT loc_1_083A
loc_1_0838:
        xor    al, al
loc_1_083A:
        mov    dword ptr [edi + 0ch], 0
        mov    byte ptr [edi + 0ah], 6
        mov    byte ptr [edi + 30h], 1eh
        mov    dword ptr [edi + 32h], 0
        mov    byte ptr [edi + 34h], al
        mov    al, byte ptr [esi + 7dh]
        shl    al, 5
        mov    byte ptr [edi + 31h], al
        xor    al, al
        ret
loc_1_085F:
        mov    word ptr [ebx + 6ah], 16h
        mov    al, 1
        ret
loc_1_0868:
        and    byte ptr [esi + 47eh], 0fbh
        mov    edx, ebx
        db  0BEh, 07Fh, 008h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        mov    al, 2
        ret
loc_1_087F:
        mov    eax, dword ptr [edx + 14h]
        mov    ecx, dword ptr [edx + 10h]
        mov    dword ptr [eax + 4], ecx
        mov    ebx, edx
        mov    esi, dword ptr [edx + 8]
        call   sub_1_07CD
        jb     SHORT loc_1_0912
        call   sub_1_0806
        mov    dword ptr [edi + 10h], 0
        db  0C7h, 043h, 070h, 0ACh, 008h, 000h, 000h
        jmp    NEAR PTR sub_1_0ED9
loc_1_08AC:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        pushfd
        sti
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [edi + 8]
        mov    esi, dword ptr [edi + 20h]
        cmp    byte ptr [esi + 3], 1
        jne    NEAR PTR loc_1_0B8A
        xor    edx, edx
        mov    dword ptr [esi + 32h], 96000000h
        mov    word ptr [esi + 37h], dx
        mov    byte ptr [esi + 39h], dl
        mov    byte ptr [esi + 30h], 45h
        mov    byte ptr [esi + 3], 0
        mov    byte ptr [esi + 0ah], 0ah
        mov    dword ptr [esi + 0ch], 0
        mov    dword ptr [esi + 10h], edx
        mov    dword ptr [esi + 36h], edx
        or     dword ptr [edi + 6ch], 0e08h
        and    dword ptr [edi + 6ch], 0fffffefdh
        xchg   edi, ebx
        xchg   esi, edi
        db  0C7h, 043h, 070h, 022h, 009h, 000h, 000h
        call   sub_1_0ED9
        popfd
        pop    esi
        pop    edi
        pop    ebp
        ret
loc_1_0912:
        mov    ebx, edx
        mov    word ptr [ebx + 6ah], 1dh
        push   ebp
        push   edi
        push   esi
        jmp    NEAR PTR loc_1_05B8
loc_1_0922:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        pushfd
        sti
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [edi + 8]
        mov    esi, dword ptr [edi + 20h]
        mov    eax, dword ptr [edi + 80h]
        cmp    byte ptr [esi + 30h], 45h
        je     NEAR PTR loc_1_0BCC
        cmp    byte ptr [esi + 30h], 1ah
        je     NEAR PTR loc_1_0C2C
        cmp    byte ptr [esi + 3], 1
        je     SHORT loc_1_0971
        cmp    byte ptr [esi + 3], 12h
        je     SHORT loc_1_0971
        test   byte ptr [esi + 39h], 40h
        jne    NEAR PTR loc_1_0B6A
        test   byte ptr [esi + 39h], 80h
        jne    NEAR PTR loc_1_0B07
        jmp    NEAR PTR loc_1_0B8A
loc_1_0971:
        test   byte ptr [esi + 39h], 40h
        jne    NEAR PTR loc_1_0B34
        test   byte ptr [esi + 39h], 80h
        jne    NEAR PTR loc_1_0B73
        test   dword ptr [ebx + 482h], 2
        je     SHORT loc_1_09C4
        mov    edx, dword ptr [ebx + 48ch]
        lea    edx, [edx + 10h]
        movzx  ecx, byte ptr [ebx + 137h]
        sub    cl, byte ptr [ebx + 136h]
        inc    cl
        inc    cl
loc_1_09AB:
        push   ecx
        mov    ecx, dword ptr [ebx + ecx*8 + 134h]
        call   sub_1_0F9C
        cmp    ecx, edx
        pop    ecx
        jbe    SHORT loc_1_09CF
        loop   loc_1_09AB
        jmp    NEAR PTR loc_1_0B8A
loc_1_09C4:
        xor    edx, edx
        mov    word ptr [ebx + 474h], dx
        jmp    SHORT loc_1_09DD
loc_1_09CF:
        movzx  ecx, byte ptr [ebx + ecx*8 + 132h]
        mov    byte ptr [ebx + 133h], cl
loc_1_09DD:
        mov    eax, dword ptr [edi + 80h]
        mov    cl, byte ptr [eax + 2]
        or     cl, cl
        je     NEAR PTR loc_1_0B8A
        cmp    cl, 64h
        jae    NEAR PTR loc_1_0B8A
        movzx  ecx, byte ptr [eax + 3]
        or     cl, cl
        je     NEAR PTR loc_1_0B8A
        cmp    cl, 64h
        jae    SHORT loc_1_0A53
        jmp    SHORT loc_1_0A3F
loc_1_0A0A:
        sub    cl, byte ptr [eax + 2]
        jb     NEAR PTR loc_1_0B8A
        inc    cl
        mov    byte ptr [eax + ecx*8 + 6], 0aah
        movzx  ecx, word ptr [eax]
        xchg   cl, ch
        push   dword ptr [edi + 90h]
        call   sub_1_0CF0
        pop    esi
        mov    word ptr [esi + 6ah], 0
loc_1_0A31:
        sub    dword ptr [esi + 14h], 8
        call   sub_1_038D
        popfd
        pop    esi
        pop    edi
        pop    ebp
        ret
loc_1_0A3F:
        mov    edx, ecx
        sub    dl, byte ptr [eax + 2]
        inc    edx
        cmp    byte ptr [eax + edx*8 + 6], 0aah
        je     SHORT loc_1_0A8D
        cmp    byte ptr [eax + edx*8 + 6], 0a2h
        je     SHORT loc_1_0A8D
loc_1_0A53:
        push   eax
        mov    eax, ecx
        shl    ax, 4
        shr    al, 4
        aad    0ah
        mov    ecx, eax
        pop    eax
        cmp    ecx, 63h
        jae    NEAR PTR loc_1_0B8A
        mov    edx, ecx
        sub    dl, byte ptr [eax + 2]
        inc    edx
        cmp    byte ptr [eax + edx*8 + 6], 0aah
        je     SHORT loc_1_0A8D
        cmp    byte ptr [eax + edx*8 + 6], 0a2h
        je     SHORT loc_1_0A8D
        movzx  ecx, word ptr [eax]
        xchg   cl, ch
        sub    ecx, 0ch
        shr    ecx, 3
        add    cl, byte ptr [eax + 2]
loc_1_0A8D:
        push   ecx
        push   esi
        mov    byte ptr [eax + 3], cl
        mov    ecx, edx
        mov    dl, byte ptr [eax + 2]
        lea    ebp, [eax + 4]
        xor    esi, esi
loc_1_0A9C:
        mov    byte ptr [ebp + 2], dl
        movzx  si, byte ptr [ebp + 1]
        lea    ebp, [ebp + 8]
        inc    edx
        test   esi, 0ffff0000h
        jne    SHORT loc_1_0AFC
        test   esi, 4
        jne    SHORT loc_1_0AFC
        test   byte ptr [ebp + 1], 4
        je     SHORT loc_1_0AFC
        cmp    byte ptr [ebp + 2], 0aah
        je     SHORT loc_1_0AFC
        cmp    byte ptr [ebp + 2], 0a2h
        je     SHORT loc_1_0AFC
        or     byte ptr [ebx + 482h], 10h
        mov    byte ptr [ebx + 487h], dl
        push   ecx
        push   eax
        mov    ecx, dword ptr [ebp + 4]
        call   sub_1_0F9C
        cmp    ecx, 2bf2h
        jb     SHORT loc_1_0AF2
        sub    ecx, 2bf2h
        shl    esi, 10h
loc_1_0AF2:
        call   sub_1_0FC9
        mov    dword ptr [ebp + 4], ecx
        pop    eax
        pop    ecx
loc_1_0AFC:
        loop   loc_1_0B05
        pop    esi
        pop    ecx
        jmp    NEAR PTR loc_1_0A0A
loc_1_0B05:
        jmp    SHORT loc_1_0A9C
loc_1_0B07:
        mov    cl, byte ptr [ebx + 474h]
        inc    cl
        cmp    cl, byte ptr [ebx + 475h]
        jne    SHORT loc_1_0B8A
        mov    eax, dword ptr [ebx + 48ch]
        sub    eax, 96h
        mov    dword ptr [ebx + 488h], eax
        mov    byte ptr [ebx + 486h], 0ffh
        jmp    NEAR PTR loc_1_0BC6
loc_1_0B34:
        and    byte ptr [ebx + 482h], 0fdh
        cmp    word ptr [eax], 0a00h
        jne    SHORT loc_1_0B6A
        movzx  edx, word ptr [eax + 2]
        mov    word ptr [ebx + 474h], dx
        mov    ecx, dword ptr [eax + 8]
        call   sub_1_0F9C
        mov    dword ptr [ebx + 48ch], ecx
        cmp    dh, dl
        je     SHORT loc_1_0B6A
        or     byte ptr [ebx + 482h], 2
        or     byte ptr [esi + 39h], 80h
loc_1_0B6A:
        xchg   edi, esi
        xchg   esi, ebx
        jmp    NEAR PTR loc_1_0C74
loc_1_0B73:
        lea    edx, [eax + 4]
        movzx  ecx, word ptr [eax]
        xchg   cl, ch
        add    eax, ecx
loc_1_0B7D:
        cmp    byte ptr [edx + 3], 0a1h
        je     SHORT loc_1_0BA1
        add    edx, 0bh
        cmp    edx, eax
        jl     SHORT loc_1_0B7D
loc_1_0B8A:
        push   dword ptr [edi + 90h]
        call   sub_1_0CF0
        pop    esi
        mov    word ptr [esi + 6ah], 18h
        jmp    NEAR PTR loc_1_0A31
loc_1_0BA1:
        movzx  eax, byte ptr [edx + 8]
        mov    byte ptr [ebx + 486h], al
        add    edx, 0bh
        cmp    byte ptr [edx + 3], 0a2h
        jne    SHORT loc_1_0B8A
        mov    eax, dword ptr [edx + 7]
        movzx  ax, ah
        rol    eax, 10h
        xchg   ah, al
        mov    dword ptr [ebx + 488h], eax
loc_1_0BC6:
        mov    byte ptr [esi + 39h], 40h
        jmp    SHORT loc_1_0B6A
loc_1_0BCC:
        and    byte ptr [ebx + 482h], 0edh
        or     byte ptr [ebx + 482h], 1
        cmp    byte ptr [esi + 3], 1
        je     SHORT loc_1_0BFB
        mov    ecx, dword ptr [esi + 1ch]
        cmp    byte ptr [ecx + 0ch], 64h
        je     SHORT loc_1_0BFB
        mov    cl, byte ptr [ecx + 2]
        and    ecx, 0fh
        cmp    cl, 5
        jne    SHORT loc_1_0BFB
        and    byte ptr [ebx + 482h], 0feh
loc_1_0BFB:
        xchg   edi, esi
        xchg   esi, ebx
        mov    byte ptr [edi + 0ah], 6
        mov    byte ptr [edi + 30h], 1ah
        mov    al, byte ptr [esi + 7dh]
        shl    al, 5
        mov    byte ptr [edi + 31h], al
        mov    byte ptr [edi + 32h], 41h
        xor    ecx, ecx
        mov    byte ptr [edi + 35h], cl
        mov    byte ptr [edi + 33h], cl
loc_1_0C1C:
        mov    cl, 14h
        mov    byte ptr [edi + 34h], cl
        lea    eax, [esi + 45ch]
        jmp    NEAR PTR loc_1_0CC5
loc_1_0C2C:
        xchg   edi, esi
        xchg   esi, ebx
        xor    ecx, ecx
        mov    dword ptr [esi + 490h], 800h
        and    byte ptr [esi + 482h], 0fbh
        cmp    byte ptr [edi + 3], 1
        jne    SHORT loc_1_0C74
        test   word ptr [esi + 466h], 0ffffh
        je     SHORT loc_1_0C74
        or     byte ptr [esi + 482h], 4
        cmp    byte ptr [edi + 32h], 41h
        mov    byte ptr [edi + 32h], 1
        je     SHORT loc_1_0C1C
        movzx  ecx, word ptr [esi + 466h]
        xchg   cl, ch
        mov    dword ptr [esi + 490h], ecx
loc_1_0C74:
        mov    byte ptr [edi + 30h], 43h
        push   edi
        lea    edi, [esi + 134h]
        mov    ecx, 0c9h
        xor    eax, eax
        rep stosd
        pop    edi
        mov    al, byte ptr [esi + 7dh]
        shl    al, 5
        or     al, 2
        mov    byte ptr [edi + 31h], al
        xor    eax, eax
        mov    dword ptr [edi + 32h], eax
        mov    al, byte ptr [edi + 39h]
        xor    al, 40h
        ror    eax, 8
        mov    dword ptr [edi + 36h], eax
        mov    word ptr [edi + 37h], 2403h
        mov    byte ptr [edi + 0ah], 0ah
        lea    eax, [esi + 134h]
        mov    ecx, 324h
        mov    word ptr [ebx + 0ch], 3ch
        mov    word ptr [ebx + 0eh], 3ch
loc_1_0CC5:
        mov    dword ptr [ebx + 80h], eax
        mov    dword ptr [edi + 0ch], 248h
        mov    dword ptr [ebx + 7ch], ecx
        mov    dword ptr [edi + 10h], ecx
        or     dword ptr [ebx + 6ch], 0e08h
        and    dword ptr [ebx + 6ch], 0fffffefdh
        call   sub_1_0EC7
        popfd
        pop    esi
        pop    edi
        pop    ebp
        ret
sub_1_0CF0:
        sub    edi, dword ptr [ebx + 4]
        sub    esp, 8
        mov    esi, esp
        mov    dword ptr [esi + 4], edi
        mov    word ptr [esi], 4
        push   esi
        db  0FFh, 015h, 054h, 00Fh, 000h, 000h
        add    esp, 0ch
        ret
sub_1_0D0B:
        push   esi
        mov    esi, dword ptr [esi]
        mov    ecx, dword ptr [esi + 4]
        add    ecx, 64h
        sub    esp, 10h
        mov    edi, esp
        mov    dword ptr [edi + 6], ecx
        add    cx, 68h
        mov    word ptr [edi + 4], cx
        mov    byte ptr [edi + 0eh], 0
        mov    word ptr [edi], 2
        push   edi
        db  0FFh, 015h, 054h, 00Fh, 000h, 000h
        mov    edi, dword ptr [edi + 0ah]
        add    esp, 14h
        mov    eax, edi
        or     edi, edi
        je     SHORT loc_1_0D70
        add    edi, dword ptr [esi + 4]
        xor    eax, eax
        mov    dword ptr [edi + 64h], eax
        mov    dword ptr [edi + 0a8h], eax
        mov    dword ptr [edi + 7ch], eax
        mov    dword ptr [edi + 6ch], 8c09h
        pop    esi
        mov    al, byte ptr [esi + 14h]
        mov    byte ptr [edi + 0b1h], al
        mov    eax, edi
        add    eax, 0bch
        mov    dword ptr [edi + 0ach], eax
        ret
loc_1_0D70:
        pop    esi
        ret
loc_1_0D72:
        mov    al, 3
        jmp    SHORT loc_1_0D78
loc_1_0D76:
        mov    al, 2
loc_1_0D78:
        mov    dword ptr [edi + 10h], 0
        mov    dword ptr [edi + 0ch], 0
        mov    byte ptr [edi + 0ah], 6
        mov    byte ptr [edi + 30h], 1bh
        mov    dword ptr [edi + 32h], 0
        mov    byte ptr [edi + 34h], al
        mov    al, byte ptr [esi + 7dh]
        shl    al, 5
        mov    byte ptr [edi + 31h], al
        mov    word ptr [ebx + 0ch], 3ch
        mov    word ptr [ebx + 0eh], 3ch
        xor    al, al
        ret
loc_1_0DB0:
        mov    dword ptr [edi + 0ch], 40h
        mov    byte ptr [edi + 0ah], 6
        mov    byte ptr [edi + 30h], 3
        mov    al, byte ptr [esi + 7dh]
        shl    al, 5
        mov    byte ptr [edi + 31h], al
        mov    eax, dword ptr [ebx + 7ch]
        mov    byte ptr [edi + 34h], al
        xor    eax, eax
        mov    word ptr [edi + 32h], ax
        mov    byte ptr [edi + 35h], al
        xor    al, al
        ret
sub_1_0DDA:
        pushad
        mov    esi, dword ptr [ebx]
        mov    ecx, dword ptr [esi + 4]
        add    ecx, 64h
        sub    esp, 10h
        mov    edi, esp
        mov    dword ptr [edi + 6], ecx
        add    cx, 58h
        mov    word ptr [edi + 4], cx
        mov    byte ptr [edi + 0eh], 0
        mov    word ptr [edi], 2
        push   edi
        db  0FFh, 015h, 054h, 00Fh, 000h, 000h
        mov    edi, dword ptr [edi + 0ah]
        add    esp, 14h
        mov    ecx, eax
        mov    eax, 1dh
        or     edi, edi
        je     SHORT loc_1_0E61
        add    edi, dword ptr [esi + 4]
        xor    edx, edx
        mov    dword ptr [edi + 0a8h], edx
        mov    dword ptr [edi + 7ch], edx
        mov    word ptr [edi + 68h], cx
        mov    dword ptr [edi + 6ch], 8d09h
        db  0BAh, 048h, 005h, 000h, 000h
        xor    edx, edx
        mov    al, byte ptr [esi + 14h]
        mov    byte ptr [edi + 0b1h], al
        db  0FFh, 015h, 060h, 00Fh, 000h, 000h
        movzx  eax, word ptr [edi + 6ah]
        sub    edi, dword ptr [ebx + 4]
        sub    esp, 8
        mov    esi, esp
        mov    dword ptr [esi + 4], edi
        mov    word ptr [esi], 4
        push   esi
        db  0FFh, 015h, 054h, 00Fh, 000h, 000h
        add    esp, 0ch
loc_1_0E61:
        mov    dword ptr [esp + 1ch], eax
        popad
        ret
sub_1_0E67:
        push   edx
        mov    eax, dword ptr [ebx + 10h]
        movsx  edi, word ptr [eax + 10h]
        lea    edi, [edi + ebx + 4]
        mov    dword ptr [edi + 40h], ebx
        mov    dword ptr [edi], 40h
        mov    byte ptr [edi + 2], 0
        mov    byte ptr [edi + 3], 0
        mov    dword ptr [ebx + 20h], edi
        xor    eax, eax
        mov    dword ptr [edi + 24h], eax
        mov    dword ptr [edi + 44h], eax
        mov    dword ptr [edi + 20h], eax
        mov    byte ptr [edi + 4ch], al
        mov    byte ptr [edi + 8], al
        mov    byte ptr [edi + 9], al
        mov    al, byte ptr [esi + 75h]
        mov    byte ptr [edi + 5], al
        mov    al, byte ptr [esi + 7ch]
        mov    byte ptr [edi + 6], al
        mov    al, byte ptr [esi + 7dh]
        mov    byte ptr [edi + 7], al
        mov    edx, edi
        add    edx, 50h
        mov    dword ptr [edi + 28h], edx
        movzx  eax, word ptr [esi + 80h]
        add    edx, eax
        mov    dword ptr [edi + 1ch], edx
        mov    byte ptr [edi + 0bh], 0eh
        pop    edx
        ret
sub_1_0EC7:
        push   ebx
        db  0FFh, 015h, 068h, 00Fh, 000h, 000h
        add    esp, 4
        or     al, al
        je     SHORT sub_1_0ED9
        or     dword ptr [ebx + 6ch], 40h
sub_1_0ED9:
        mov    byte ptr [edi + 3], 0
        mov    dword ptr [edi + 44h], 0
        mov    cx, word ptr [ebx + 0eh]
        mov    word ptr [ebx + 0ch], cx
        xor    edx, edx
        xchg   esi, edi
        xchg   edi, ebx
        mov    dword ptr [edi + 0a4h], edi
        db  0FFh, 015h, 060h, 00Fh, 000h, 000h
        ret
sub_1_0EFF:
        cmp    dword ptr [esi + 49ch], 0
        jne    SHORT loc_1_0F38
        mov    ebx, dword ptr [esi + 4a0h]
        or     ebx, ebx
        je     SHORT loc_1_0F38
        mov    dword ptr [esi + 49ch], ebx
        mov    ecx, dword ptr [ebx + 64h]
        mov    dword ptr [esi + 4a0h], ecx
        sti
        mov    ax, word ptr [ebx + 0eh]
        mov    word ptr [ebx + 0ch], ax
        cmp    byte ptr [ebx + 68h], 0dh
        je     SHORT loc_1_0F3A
        push   ebp
        push   edi
        push   esi
        jmp    NEAR PTR loc_1_0596
loc_1_0F38:
        sti
        ret
loc_1_0F3A:
        mov    edx, ebx
        db  0BEh, 047h, 00Fh, 000h, 000h
        db  0CDh, 020h, 00Eh, 080h, 001h, 000h
loc_1_0F47:
        sub    esp, 0ch
        mov    ebx, edx
        mov    esi, dword ptr [ebx + 4]
        jmp    NEAR PTR loc_1_0596
sub_1_0F54:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_0F60:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_0F68:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0F70:
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
loc_1_0F88:
        db  0C7h, 047h, 0FCh, 0F5h, 003h, 000h, 000h
        push   ebx
        call   sub_1_0404
        lea    esp, [esp + 4]
        mov    al, 2
        ret
sub_1_0F9C:
        push   edx
        mov    eax, 1194h
        movzx  cx, ch
        mul    cx
        shl    edx, 10h
        add    eax, edx
        ror    ecx, 10h
        movzx  edx, ch
        add    edx, eax
        xchg   ecx, edx
        mov    eax, 4bh
        mul    dl
        add    ecx, eax
        sub    ecx, 96h
        pop    edx
        ret
sub_1_0FC9:
        push   edx
        push   ebx
        add    ecx, 96h
        mov    ebx, 1194h
        mov    eax, ecx
        xor    edx, edx
        div    ebx
        movzx  ecx, al
        mov    eax, edx
        mov    ebx, 4bh
        xor    edx, edx
        div    bx
        mov    ch, al
        shl    ecx, 8
        shl    edx, 18h
        or     ecx, edx
        pop    ebx
        pop    edx
        ret
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 003h, 00Ah, 000h, 000h, 043h, 044h, 056h, 053h
        db  044h, 020h, 020h, 020h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 048h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
_LTEXT ends
        end
