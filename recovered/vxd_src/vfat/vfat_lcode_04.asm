; vfat_lcode_04.asm -- VFAT object 4 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        db  047h, 04Ah, 0EBh, 0E4h, 056h, 068h, 0F0h, 0BEh, 000h, 000h, 0E8h, 028h, 0F2h, 0FFh, 0FFh, 083h
        db  0C4h, 008h, 0F7h, 0C2h, 000h, 080h, 000h, 000h, 074h, 009h, 08Bh, 03Ch, 01Fh, 00Bh, 0FFh, 074h
        db  031h, 02Bh, 0FBh, 081h, 0E2h, 0FFh, 03Fh, 000h, 000h, 074h, 027h, 080h, 03Ch, 01Fh, 000h, 074h
        db  021h, 0FFh, 034h, 01Fh, 068h, 0F8h, 0BEh, 000h, 000h, 0E8h, 0F9h, 0F1h, 0FFh, 0FFh, 083h, 0C4h
        db  008h, 047h, 04Ah, 0EBh, 0DEh, 00Fh, 0B6h, 0C0h, 003h, 0F0h, 049h, 074h, 063h, 0E9h, 007h, 0FFh
        db  0FFh, 0FFh, 068h, 0FCh, 0BEh, 000h, 000h, 0E8h, 0DBh, 0F1h, 0FFh, 0FFh, 083h, 0C4h, 004h, 00Fh
        db  0B6h, 0C0h, 003h, 0F0h, 049h, 074h, 049h, 0A1h, 0BCh, 013h, 000h, 000h, 083h, 0F8h, 034h, 073h
        db  023h, 033h, 0D2h, 0BFh, 01Ah, 000h, 000h, 000h, 0F7h, 0F7h, 0F7h, 0DAh, 083h, 0C2h, 01Ah, 068h
        db  000h, 0BFh, 000h, 000h, 0E8h, 0AEh, 0F1h, 0FFh, 0FFh, 083h, 0C4h, 004h, 04Ah, 075h, 0F0h, 0E9h
        db  0C5h, 0FEh, 0FFh, 0FFh, 068h, 004h, 0BFh, 000h, 000h, 0E8h, 099h, 0F1h, 0FFh, 0FFh, 083h, 0C4h
        db  004h, 0C7h, 005h, 0BCh
loc_4_00A4:
        adc    eax, dword ptr [eax]
        add    byte ptr [eax], al
loc_4_00A8:
        add    byte ptr [eax], al
        add    cl, ch
        test   eax, 68fffffeh
        or     byte ptr [edi + 7de80000h], bh
        int1
        db  0FFh, 0FFh, 083h, 0C4h, 004h, 0C3h, 0B8h, 077h, 000h, 000h, 000h, 0B3h, 002h, 0CDh, 041h, 00Ah
        db  0E4h, 075h, 01Ch, 0BBh, 048h, 0B7h, 000h, 000h, 08Bh, 01Bh, 081h, 0FBh, 048h, 0B7h, 000h, 000h
        db  074h, 00Ch, 0BEh, 0F1h, 003h, 000h, 000h, 0E8h, 052h, 0FEh, 0FFh, 0FFh, 0EBh, 0EAh, 0C3h, 0B8h
        db  078h, 000h, 000h, 000h, 0CDh, 041h, 083h, 0FBh, 019h, 076h, 00Bh, 0BEh, 0F1h, 003h, 000h, 000h
        db  0E8h, 039h, 0FEh, 0FFh, 0FFh, 0C3h, 08Ah, 0D3h, 0BBh, 048h, 0B7h, 000h, 000h, 08Bh, 01Bh, 081h
        db  0FBh, 048h, 0B7h, 000h, 000h, 074h, 016h, 038h, 093h, 0ACh, 000h, 000h, 000h, 075h, 0EEh, 052h
        db  0BEh, 0F1h, 003h, 000h, 000h, 0E8h, 014h, 0FEh, 0FFh, 0FFh, 05Ah, 0EBh, 0E0h, 0C3h
sub_4_0126:
        jecxz  loc_4_0171
loc_4_0128:
        movzx  eax, word ptr [esi]
        sub    ecx, eax
        push   ecx
        lea    ecx, [eax - 2]
loc_4_0131:
        add    esi, 2
        jecxz  loc_4_016C
        db  068h, 00Ch, 0BFh, 000h, 000h
        db  0E8h, 0F7h, 0F0h, 0FFh
loc_4_013F:
        inc    dword ptr [ebx + 7e8004c4h]
        add    dword ptr [eax], eax
        je     SHORT loc_4_0156
        db  068h, 010h, 0BFh, 000h, 000h
        db  0E8h, 0E4h, 0F0h, 0FFh, 0FFh
        add    esp, 4
loc_4_0156:
        push   esi
        db  068h, 014h, 0BFh, 000h, 000h
        db  0E8h, 0D6h, 0F0h, 0FFh, 0FFh
        add    esp, 8
        add    esi, 2
        sub    ecx, 2
        db  077h, 0D7h
loc_4_016C:
        pop    ecx
        or     ecx, ecx
        jne    SHORT loc_4_0128
loc_4_0171:
        ret
        db  08Bh, 01Dh, 078h, 0B2h, 000h, 000h
loc_4_0178:
        db  08Bh, 00Dh, 080h, 0B8h, 000h, 000h
loc_4_017E:
        cmp    word ptr [ebx + 14h], 0
        je     SHORT loc_4_01AA
        push   ecx
        db  0BEh, 02Bh, 009h, 000h, 000h
        db  0E8h, 0A6h, 0FDh, 0FFh, 0FFh
        movzx  ecx, word ptr [ebx + 14h]
        mov    esi, dword ptr [ebx + 8]
        call   sub_4_0126
        db  068h, 01Ch, 0BFh, 000h, 000h
        db  0E8h, 091h, 0F0h, 0FFh, 0FFh
loc_4_01A6:
        add    esp, 4
        pop    ecx
loc_4_01AA:
        add    ebx, 18h
        loop   loc_4_017E
        ret
        db  0B8h, 078h, 000h, 000h, 000h, 0CDh, 041h, 0BEh, 0E8h
loc_4_01B9:
        pop    es
        add    byte ptr [eax], al
        db  0E8h, 075h, 0FDh, 0FFh, 0FFh
        ret
        db  08Ah, 025h, 0F8h
loc_4_01C5:
        mov    dh, 0
        db  000h, 0BAh, 0D4h, 011h, 000h, 000h
        db  0CDh, 020h, 009h, 000h, 08Bh, 004h
        ret
loc_4_01D4:
        pushad
        mov    ebx, esi
        db  0BEh, 0CCh, 008h, 000h, 000h
        db  0E8h, 055h, 0FDh, 0FFh, 0FFh
        popad
        ret
loc_4_01E3:
        mov    eax, 77h
        mov    bl, 2
        int    41h
        or     ah, ah
        jne    SHORT loc_4_020A
        sub    esp, 80h
        mov    esi, esp
        db  0B8h
loc_4_01F9:
        jne    SHORT loc_4_01FB
loc_4_01FB:
        add    byte ptr [eax], al
        int    41h
        mov    ebx, dword ptr [esi + 14h]
        add    esp, 80h
        jmp    SHORT loc_4_0211
loc_4_020A:
        mov    eax, 78h
        int    41h
loc_4_0211:
        db  0BEh, 07Fh, 009h, 000h, 000h
        db  0E8h, 01Bh, 0FDh, 0FFh, 0FFh
        add    ebx, 34h
        db  0BEh, 04Dh, 00Ah, 000h, 000h
        db  0E8h, 00Eh, 0FDh, 0FFh, 0FFh
        cmp    byte ptr [ebx + 2], 41h
        jne    SHORT loc_4_0265
        test   word ptr [ebx + 4], 2000h
        je     SHORT loc_4_0265
        mov    esi, dword ptr [ebx - 28h]
        or     esi, esi
        je     SHORT loc_4_0265
        db  068h, 020h, 0BFh, 000h, 000h
        db  0E8h, 0F0h, 0EFh, 0FFh, 0FFh
        add    esp, 4
        movzx  ecx, word ptr [esi]
        sub    ecx, 4
        add    esi, 4
        db  0E8h, 0CEh, 0FEh, 0FFh
loc_4_0257:
        db  0FFh, 068h, 02Ch
        db  0BFh, 000h, 000h, 0E8h, 0D5h, 0EFh, 0FFh, 0FFh, 083h, 0C4h, 004h
loc_4_0265:
        cmp    byte ptr [ebx + 2], 41h
        jne    SHORT loc_4_0277
        db  0BEh, 09Eh, 00Bh, 000h, 000h
        db  0E8h, 0C1h, 0FCh, 0FFh, 0FFh
        jmp    SHORT loc_4_0287
loc_4_0277:
        cmp    byte ptr [ebx + 2], 59h
        jne    SHORT loc_4_0287
        db  0BEh, 097h, 00Ah, 000h, 000h
        db  0E8h, 0AFh, 0FCh, 0FFh, 0FFh
loc_4_0287:
        sub    esp, 10h
        mov    edi, esp
        mov    esi, dword ptr [ebx - 14h]
        call   sub_4_0361
        jb     SHORT loc_4_02A6
        mov    ecx, esp
        push   ecx
        db  068h, 030h, 0BFh, 000h, 000h
        db  0E8h, 094h, 0EFh, 0FFh, 0FFh
        add    esp, 8
loc_4_02A6:
        add    esp, 10h
        ret
        db  0B8h, 077h, 000h, 000h, 000h, 0B3h, 002h, 0CDh, 041h, 00Ah, 0E4h, 00Fh, 084h, 045h, 0EDh, 0FFh
        db  0FFh, 0B8h, 078h, 000h, 000h, 000h, 0CDh, 041h, 0BEh, 09Ah, 00Ch, 000h, 000h, 0E8h, 06Ah, 0FCh
        db  0FFh, 0FFh, 08Bh, 043h, 028h, 00Fh, 0B6h, 080h, 0ACh, 000h, 000h, 000h, 004h, 041h, 050h, 068h
        db  044h, 0BFh, 000h, 000h, 0E8h, 054h, 0EFh, 0FFh, 0FFh, 083h, 0C4h, 008h, 0C3h, 0B8h, 077h, 000h
        db  000h, 000h, 0B3h, 002h, 0CDh, 041h, 00Ah, 0E4h, 075h, 01Ah, 081h, 0ECh, 080h, 000h, 000h, 000h
        db  08Bh, 0F4h, 0B8h, 075h, 000h, 000h, 000h, 0CDh, 041h, 08Bh, 05Eh, 018h, 081h, 0C4h
loc_4_0308:
        add    byte ptr [eax], 0
        add    bl, ch
        pop    es
        mov    eax, 78h
        int    41h
        db  0BEh, 04Ch, 00Eh, 000h, 000h
        db  0E8h, 017h, 0FCh, 0FFh, 0FFh
        sub    esp, 10h
        mov    edi, esp
        mov    esi, ebx
        call   sub_4_0361
        jb     SHORT loc_4_033D
        mov    eax, esp
        push   eax
        db  068h, 058h, 0BFh, 000h, 000h
        db  0E8h
loc_4_0336:
        std
loc_4_0337:
        out    dx, al
        db  0FFh, 0FFh, 083h, 0C4h, 008h
loc_4_033D:
        add    esp, 10h
        ret
        db  0BBh, 060h, 0B7h, 000h, 000h, 0BEh, 0F7h, 00Eh, 000h, 000h, 0E8h, 0E6h, 0FBh, 0FFh, 0FFh, 0C3h
        db  0BFh, 060h, 0B7h, 000h, 000h, 0B9h, 004h, 000h, 000h, 000h, 033h, 0C0h, 0FCh, 0F3h, 0ABh, 0C3h
sub_4_0361:
        mov    ecx, 2ch
        mov    eax, 79h
        int    41h
        or     ax, ax
        jne    SHORT loc_4_03B9
        mov    esi, dword ptr [esi + 8]
        mov    ecx, 68h
        mov    eax, 79h
        int    41h
        or     ax, ax
        jne    SHORT loc_4_03B9
        cld
        push   esi
        mov    esi, dword ptr [esi + 28h]
        mov    ecx, 0cch
        mov    eax, 79h
        int    41h
        pop    ecx
        or     ax, ax
        jne    SHORT loc_4_03B9
        movzx  eax, byte ptr [esi + 0ach]
        add    al, 41h
        stosb
        mov    al, 3ah
        stosb
        lea    esi, [ecx + 45h]
        mov    ecx, 0bh
        rep movsb
        mov    byte ptr [edi], 0
        clc
loc_4_03B8:
        ret
loc_4_03B9:
        stc
        jmp    SHORT loc_4_03B8
        db  000h, 000h, 000h, 000h, 000h, 04Eh, 04Fh, 054h, 020h, 000h, 000h, 0D4h, 0B1h, 000h, 000h, 046h
        db  06Ch, 077h, 05Fh, 044h, 065h, 073h, 063h, 000h, 0BCh, 0B8h, 000h, 000h, 046h, 075h, 06Ch, 06Ch
        db  048h, 06Fh, 06Ch, 064h, 053h, 065h, 06Dh, 000h, 0B8h, 0B8h, 000h, 000h, 046h, 075h, 06Ch, 06Ch
        db  04Ch, 06Fh, 063h, 06Bh, 053h
loc_4_03F1:
        insd
        db  000h, 0F4h
        mov    bh, 0
        add    byte ptr [edi + 6fh], al
        db  066h, 074h, 057h
        popad
        db  069h, 074h, 000h, 0D0h, 0B1h, 000h, 000h, 04Eh
        inc    ecx
        inc    edx
        inc    esp
        jae    NEAR PTR loc_4_046F
        db  000h, 0C8h
        mov    cl, 0
        add    byte ptr [edx + 64h], dl
        bound  eax, qword ptr [esi + 44h]
        db  065h, 073h, 063h
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_046F:
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_092B:
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_097F:
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0A4D:
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
        add    byte ptr [eax], al
loc_4_0A97:
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0EF7:
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
        db  000h, 08Ah, 025h, 0F8h, 0B6h, 000h
        add    ch, cl
        and    byte ptr [edi], cl
        add    byte ptr [ebx - 323c06fch], cl
        and    byte ptr [eax], al
        add    byte ptr [ebx + 3df77204h], cl
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        db  072h, 0F0h
        db  0CDh, 020h, 000h, 000h, 040h, 000h
        db  072h, 0E8h
        cmp    eax, 22h
        db  072h, 0E3h
        db  0BEh, 000h, 000h, 000h, 000h
        mov    ecx, 4
        db  0CDh, 020h, 001h, 000h, 08Bh, 004h
        db  072h, 0D1h
        db  0A2h, 0F8h, 0B6h, 000h, 000h
        push   0
        push   0
        sub    ecx, ecx
        db  0CDh, 020h, 05Ch, 000h, 001h, 000h
        db  089h, 00Dh, 08Ah, 0B7h, 000h, 000h
        pop    eax
        pop    eax
        push   22h
        push   1
        push   0
        push   40h
        db  068h, 058h, 062h, 000h, 000h
        push   0
        db  0CDh, 020h, 076h, 000h, 040h, 000h
        add    esp, 18h
        cld
        db  0B8h, 0FCh, 0B6h, 000h, 000h
        mov    dword ptr [eax + 1ch], eax
        mov    dword ptr [eax + 20h], eax
        db  0C7h, 005h, 028h, 0B7h, 000h, 000h, 020h, 0B7h, 000h, 000h
        db  0C7h, 005h, 02Ch, 0B7h, 000h, 000h, 020h, 0B7h, 000h, 000h
        call   sub_4_12C0
        mov    eax, 7d0h
        db  0BEh, 027h, 08Ch, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  0A1h, 0F0h, 0B6h, 000h, 000h
        mov    dword ptr [eax], 0ffffffffh
        cld
        db  089h, 035h, 01Eh, 0B8h, 000h, 000h
        cmp    esi, 1
        ret
        db  0E8h, 0A0h, 000h, 000h, 000h, 083h
loc_4_10BE:
        in     al, dx
        db  010h, 08Dh, 035h, 03Ch, 007h, 000h
        add    byte ptr [ebp + 5657243ch], cl
        push   80000002h
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_4_1158
        db  08Dh, 01Dh, 01Ah, 009h, 000h, 000h
loc_4_10E3:
        mov    ecx, dword ptr [ebx + 4]
        or     ecx, ecx
        je     SHORT loc_4_1146
        mov    eax, dword ptr [ebx + 8]
        lea    edx, [esp + 4]
        lea    esi, [esp + 0ch]
        lea    edi, [esp + 8]
        mov    dword ptr [edi], ecx
        mov    ecx, dword ptr [esp]
        push   edi
        push   esi
        push   edx
        push   0
        push   eax
        push   ecx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_4_1141
        mov    eax, dword ptr [esp + 4]
        cmp    eax, dword ptr [ebx]
        je     SHORT loc_4_112A
        cmp    eax, 4
        jne    SHORT loc_4_1128
        cmp    dword ptr [ebx], 3
        jne    SHORT loc_4_1128
        cmp    dword ptr [ebx + 4], 4
loc_4_1128:
        jne    SHORT loc_4_1141
loc_4_112A:
        mov    edi, dword ptr [ebx + 0ch]
        mov    ecx, dword ptr [ebx + 4]
        sub    al, al
        rep stosb
        lea    esi, [esp + 0ch]
        mov    edi, dword ptr [ebx + 0ch]
        mov    ecx, dword ptr [esp + 8]
        rep movsb
loc_4_1141:
        add    ebx, 10h
        jmp    SHORT loc_4_10E3
loc_4_1146:
        mov    eax, dword ptr [esp]
        push   eax
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
        add    esp, 10h
        clc
        ret
loc_4_1158:
        add    esp, 10h
        stc
        ret
        db  083h, 0ECh, 038h, 08Dh, 005h, 068h, 007h, 000h, 000h, 08Dh, 01Ch, 024h, 053h, 050h, 068h, 002h
        db  000h, 000h, 080h, 0CDh, 020h
loc_4_1172:
        dec    eax
        add    dword ptr [ecx], eax
        add    byte ptr [ebx - 3ff4f33ch], al
        jne    NEAR PTR loc_4_129D
        mov    dword ptr [esp + 0ch], 0
loc_4_1189:
        mov    dword ptr [esp + 4], 0
        mov    dword ptr [esp + 8], 0
        mov    dword ptr [esp + 10h], 0
        mov    dword ptr [esp + 14h], 0
loc_4_11A9:
        mov    dword ptr [esp + 1ch], 12h
        mov    esi, dword ptr [esp]
        mov    edi, dword ptr [esp + 10h]
        lea    eax, [esp + 14h]
        lea    ebx, [esp + 18h]
        lea    ecx, [esp + 22h]
        lea    edx, [esp + 1ch]
        push   edx
        push   ecx
        push   ebx
        push   0
        push   eax
        push   0
        push   edi
        push   esi
        db  0CDh, 020h
loc_4_11D4:
        push   eax
        add    dword ptr [ecx], eax
        add    byte ptr [ebx - 77cdf3ch], al
        add    byte ptr [eax + edx + 3dh], dh
        ljmp   3d09h:74000000h
        add    eax, dword ptr [ecx]
        add    byte ptr [eax], al
        je     SHORT loc_4_1258
        jmp    SHORT loc_4_11A9
        db  0FFh, 044h, 024h, 010h, 083h, 07Ch, 024h, 004h, 020h, 073h, 0AEh, 083h, 07Ch, 024h, 018h, 003h
        db  075h, 0A7h, 08Bh, 04Ch, 024h, 01Ch, 083h, 0E9h, 002h, 076h, 09Eh, 083h, 0F9h, 010h, 077h, 099h
        db  00Fh, 0B7h, 044h, 024h, 022h, 08Dh, 004h, 001h, 03Dh, 000h, 002h, 000h, 000h, 077h, 08Ah, 066h
        db  089h, 04Ch, 024h, 020h, 001h, 04Ch, 024h, 008h, 0FFh, 044h, 024h, 004h, 083h, 03Dh, 094h, 0B8h
        db  000h, 000h, 000h, 00Fh, 084h, 070h, 0FFh, 0FFh, 0FFh, 08Bh, 03Dh, 094h, 0B8h, 000h, 000h, 0FFh
        db  007h, 08Bh, 07Ch, 024h, 00Ch, 083h, 0C1h, 004h, 08Dh, 074h, 024h, 020h, 0FCh, 0F3h, 0A4h, 089h
        db  07Ch, 024h, 00Ch, 0E9h, 051h, 0FFh, 0FFh, 0FFh
loc_4_1258:
        cmp    dword ptr [esp + 0ch], 0
        jne    SHORT loc_4_1290
        cmp    dword ptr [esp + 4], 0
        je     SHORT loc_4_1290
        imul   eax, dword ptr [esp + 4], 4
        add    eax, dword ptr [esp + 8]
        add    eax, 4
        db  0E8h, 057h, 09Fh, 0FDh, 0FFh
        jb     SHORT loc_4_1290
        db  0A3h, 094h, 0B8h, 000h, 000h
        mov    dword ptr [eax], 0
        lea    eax, [eax + 4]
        mov    dword ptr [esp + 0ch], eax
        jmp    NEAR PTR loc_4_1189
loc_4_1290:
        mov    eax, dword ptr [esp]
        push   eax
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
loc_4_129D:
        add    esp, 38h
        ret
        db  0BEh, 086h, 008h, 000h, 000h
loc_4_12A6:
        lodsd
        or     eax, eax
        db  074h
loc_4_12AA:
        adc    al, 8bh
        clc
        lodsd
        push   esi
        sub    esi, esi
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        pop    esi
        mov    edx, eax
        lodsd
        mov    dword ptr [eax], edx
        jmp    SHORT loc_4_12A6
        db  0C3h
sub_4_12C0:
        ret
        db  000h, 000h, 000h, 0F8h, 0C3h, 0E8h, 0EDh, 0FDh, 0FFh, 0FFh, 073h, 005h, 0E8h, 0CFh, 0FFh, 0FFh
        db  0FFh, 0CDh, 020h, 051h, 000h, 040h, 000h, 083h, 038h, 006h, 072h, 079h, 08Bh, 050h, 014h, 089h
        db  015h, 084h, 0B2h, 000h, 000h, 08Bh
loc_4_12E7:
        push   eax
        db  018h, 089h, 015h, 088h, 0B2h, 000h
        add    byte ptr [ebx], ch
        db  0F6h, 087h, 035h, 084h, 0B8h, 000h, 000h
        or     esi, esi
        je     SHORT loc_4_1356
        db  083h, 03Dh, 080h, 0B8h, 000h, 000h, 000h
        je     SHORT loc_4_1356
        or     esi, 1
        db  03Bh, 035h, 080h, 0B8h, 000h, 000h
        jb     SHORT loc_4_1356
        cmp    esi, 3ffdh
        ja     SHORT loc_4_1356
        mov    edi, 0ffffffffh
        mov    edx, edi
        mov    ecx, esi
loc_4_1320:
        inc    edi
        add    edx, 2
        sub    ecx, edx
        ja     SHORT loc_4_1320
        dec    edi
        sub    esi, 2
loc_4_132C:
        mov    ecx, 3
        inc    edi
        add    esi, 2
loc_4_1335:
        sub    edx, edx
        mov    eax, esi
        div    ecx
        or     edx, edx
        je     SHORT loc_4_132C
        inc    ecx
        cmp    ecx, edi
        jbe    SHORT loc_4_1335
        mov    eax, 10h
        mul    esi
        db  0A3h, 07Ch, 0B2h, 000h, 000h
        db  089h, 035h, 084h, 0B8h, 000h, 000h
loc_4_1356:
        db  0A1h, 080h, 0B8h, 000h, 000h
        cmp    eax, 10h
        jae    SHORT loc_4_1367
        mov    al, 10h
        db  0A3h, 080h, 0B8h, 000h, 000h
loc_4_1367:
        cmp    eax, 100h
        jbe    SHORT loc_4_1378
        mov    eax, 100h
        db  0A3h, 080h, 0B8h, 000h, 000h
loc_4_1378:
        mov    ecx, 18h
        mul    ecx
        push   1
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        cmp    eax, 1
        db  00Fh, 082h, 0D3h, 000h, 000h, 000h
        db  0A3h, 078h, 0B2h, 000h, 000h
        db  0A1h, 098h, 0B8h, 000h, 000h
        db  08Bh, 015h, 09Ch, 0B8h, 000h, 000h
        cmp    eax, 7ffb0000h
        jne    SHORT loc_4_13B3
        cmp    edx, 7ffb0000h
        je     SHORT loc_4_13E8
loc_4_13B3:
        shr    eax, 0fh
        shr    edx, 0fh
        cmp    eax, 0fffeh
        ja     SHORT loc_4_13E8
loc_4_13C0:
        cmp    eax, 8000h
        jb     SHORT loc_4_13E8
loc_4_13C7:
        cmp    edx, 0fffeh
        ja     SHORT loc_4_13DB
        cmp    edx, 8000h
        jb     SHORT loc_4_13DB
        cmp    edx, eax
        jbe    SHORT loc_4_13DD
loc_4_13DB:
        mov    edx, eax
loc_4_13DD:
        db  0A3h, 0A0h, 0B8h, 000h, 000h
        db  089h, 015h, 0A4h, 0B8h, 000h, 000h
loc_4_13E8:
        db  0E8h, 021h, 0FCh, 0FFh, 0FFh
        db  072h, 078h
        db  0CDh, 020h, 000h, 000h, 010h, 000h
        db  072h, 070h
        db  0E8h, 0D4h, 002h, 000h, 000h
        db  072h, 069h
        db  0BEh, 07Ah, 0BDh, 000h, 000h
        db  0CDh, 020h, 0EFh, 000h, 001h, 000h
        db  072h, 05Ch
        db  0CDh, 020h, 0FFh, 000h, 001h, 000h
        or     eax, eax
        stc
        db  074h, 051h
        db  0A3h, 08Ch, 0BDh, 000h, 000h
        mov    esi, eax
        int    20h
        db  001h
_LTEXT ends
        end
