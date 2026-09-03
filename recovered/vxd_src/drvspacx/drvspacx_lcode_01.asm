; drvspacx_lcode_01.asm -- DRVSPACX object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

DRVSPACX_Control:
        cmp    eax, 1bh
        db  00Fh, 084h, 067h, 003h, 0FFh, 0FFh
        cmp    eax, 4
        je     NEAR PTR loc_1_098C
        cmp    eax, 17h
        je     NEAR PTR loc_1_098C
        cmp    eax, 25h
        je     NEAR PTR loc_1_0A7F
        cmp    eax, 14h
        je     NEAR PTR loc_1_0234
        clc
        ret
        db  000h
loc_1_0030:
        mov    ebx, dword ptr [esp + 4]
        mov    word ptr [ebx + 2], 0
        movzx  eax, word ptr [ebx]
        cmp    eax, 15h
        jae    SHORT loc_1_0090
        db  08Ah, 080h, 052h, 000h, 000h, 000h
        and    eax, 7fh
        db  0FFh, 024h, 085h, 068h, 000h, 000h, 000h
        db  000h, 000h, 001h, 002h, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h
        db  005h, 006h, 007h, 008h, 009h, 000h, 090h, 000h, 000h, 000h, 035h, 001h, 000h, 000h, 000h, 000h
        db  000h, 000h, 0ABh, 000h, 000h, 000h, 012h, 001h, 000h, 000h, 00Dh, 002h, 000h, 000h, 03Bh, 002h
        db  000h, 000h, 0B6h, 00Ah, 000h, 000h, 0B7h, 00Ah, 000h, 000h, 0F0h, 00Bh, 000h, 000h
loc_1_0090:
        ret
sub_1_0091:
        db  0FFh, 005h, 01Ch, 00Eh, 000h, 000h
        db  083h, 03Dh, 01Ch, 00Eh, 000h, 000h, 001h
        jne    SHORT loc_1_00C0
        db  08Bh, 00Dh, 018h, 00Eh, 000h, 000h
        jecxz  loc_1_00C0
        db  0A1h, 024h, 00Eh, 000h, 000h
        push   0
        push   ecx
        push   eax
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, 1
        jb     SHORT loc_1_00C0
        clc
loc_1_00C0:
        ret
loc_1_00C1:
        call   sub_1_0091
        jb     SHORT loc_1_00E2
        mov    eax, ebp
        db  087h, 005h, 044h, 00Eh, 000h, 000h
        mov    dword ptr [ebp], eax
        movzx  eax, byte ptr [ebp + 88h]
        db  089h, 02Ch, 085h, 0B0h, 00Dh, 000h, 000h
        clc
loc_1_00E2:
        ret
loc_1_00E3:
        movzx  ecx, byte ptr [ebp + 88h]
        xor    esi, esi
        db  0BAh, 044h, 00Eh, 000h, 000h
loc_1_00F1:
        mov    eax, dword ptr [edx]
        or     eax, eax
        je     SHORT loc_1_0134
        cmp    eax, ebp
        je     SHORT loc_1_0109
        mov    edx, eax
        cmp    byte ptr [eax + 88h], cl
        jne    SHORT loc_1_00F1
        mov    esi, eax
        jmp    SHORT loc_1_00F1
loc_1_0109:
        or     esi, esi
        jne    SHORT loc_1_011D
loc_1_010D:
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     SHORT loc_1_011D
        cmp    byte ptr [eax + 88h], cl
        jne    SHORT loc_1_010D
        mov    esi, eax
loc_1_011D:
        xor    eax, eax
        xchg   dword ptr [ebp], eax
        mov    dword ptr [edx], eax
        db  039h, 02Ch, 08Dh, 0B0h, 00Dh, 000h, 000h
        jne    SHORT loc_1_0134
        db  089h, 034h, 08Dh, 0B0h, 00Dh, 000h, 000h
loc_1_0134:
        ret
sub_1_0135:
        db  0FFh, 00Dh, 01Ch, 00Eh, 000h, 000h
        jne    SHORT loc_1_015A
        db  08Bh, 00Dh, 018h, 00Eh, 000h, 000h
        jecxz  loc_1_015A
        db  0A1h, 024h, 00Eh, 000h, 000h
        push   2000h
        push   ecx
        push   eax
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
loc_1_015A:
        ret
loc_1_015B:
        push   edx
        db  08Bh, 03Dh, 040h, 00Eh, 000h, 000h
loc_1_0162:
        cmp    byte ptr [edi], al
        je     SHORT loc_1_017D
        mov    edx, dword ptr [edi + 19h]
        cmp    dx, -1
        je     SHORT loc_1_017C
        movzx  edi, dx
        shr    edx, 0ch
        and    dl, 0f0h
        add    edi, edx
        jmp    SHORT loc_1_0162
loc_1_017C:
        stc
loc_1_017D:
        pop    edx
        ret
        db  000h
loc_1_0180:
        db  00Fh, 0BAh, 02Dh, 0E0h, 00Ah, 000h, 000h, 000h
        jb     SHORT loc_1_01CF
        mov    eax, 2fh
        db  0BEh, 0D0h, 001h, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        mov    eax, 2fh
        db  0CDh, 020h, 044h, 000h, 001h, 000h
        db  089h, 00Dh, 0DCh, 00Ah, 000h, 000h
        db  089h, 015h, 0D8h, 00Ah, 000h, 000h
        db  0BEh, 0ECh, 001h, 000h, 000h
        db  0CDh, 020h, 00Ch, 000h, 001h, 000h
        movzx  edx, ax
        mov    ecx, eax
        shr    ecx, 10h
        mov    eax, 2fh
        db  0CDh, 020h, 045h, 000h, 001h, 000h
loc_1_01CF:
        ret
sub_1_01D0:
        cmp    word ptr [ebp + 1ch], 4a11h
        je     SHORT loc_1_01DA
loc_1_01D8:
        stc
        ret
loc_1_01DA:
        movzx  ebx, word ptr [ebp + 10h]
        cmp    bx, 13h
        jae    SHORT loc_1_01D8
        db  0FFh, 024h, 09Dh, 00Ch, 00Fh, 000h, 000h
loc_1_01EC:
        cmp    word ptr [ebp + 1ch], 4a11h
        je     SHORT loc_1_0206
loc_1_01F4:
        db  08Bh, 00Dh, 0DCh, 00Ah, 000h, 000h
        db  08Bh, 015h, 0D8h, 00Ah, 000h, 000h
        db  0CDh, 020h, 049h, 080h, 001h, 000h
loc_1_0206:
        call   sub_1_01D0
        jb     SHORT loc_1_01F4
        db  0CDh, 020h, 047h, 000h, 001h, 000h
        ret
sub_1_0214:
        pushad
        lea    ebp, [esp + 1ch]
        push   es
        mov    edi, ds
        mov    es, edi
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [ebp + 0ch]
        mov    eax, 73h
        int    41h
        db  001h, 005h, 0E4h, 00Ah, 000h, 000h
        pop    es
        popad
        ret
loc_1_0234:
        cld
        db  0F7h, 005h, 03Bh, 002h, 000h, 000h, 000h, 000h, 040h, 000h
        je     NEAR PTR loc_1_0338
        db  0BFh, 07Eh, 017h, 000h, 000h
loc_1_024A:
        inc    edi
loc_1_024B:
        mov    eax, 77h
        mov    bl, 1
        int    41h
        or     ah, ah
        je     SHORT loc_1_0261
        cmp    byte ptr [edi], 3fh
        je     SHORT loc_1_024A
        scasb
        je     SHORT loc_1_024B
        dec    edi
loc_1_0261:
        cmp    byte ptr [edi], 0
        or     ah, ah
        jne    NEAR PTR loc_1_0324
        db  068h, 0E8h, 00Ah, 000h, 000h
        call   sub_1_0214
        add    esp, 4
        db  0B9h, 044h, 00Eh, 000h, 000h
loc_1_027E:
        mov    ecx, dword ptr [ecx]
        jecxz  loc_1_02BD
        movzx  edi, byte ptr [ecx + 88h]
        movzx  eax, byte ptr [ecx + 89h]
        movzx  edx, byte ptr [ecx + 8ah]
        add    edi, 41h
        add    eax, 41h
        add    edx, 41h
        mov    bl, 2ah
        cmp    dword ptr [ecx + 48h], 0
        jne    SHORT loc_1_02AA
        mov    bl, 20h
loc_1_02AA:
        push   edx
        push   eax
        push   ebx
        push   edi
        db  068h, 000h, 00Bh, 000h, 000h
        call   sub_1_0214
        add    esp, 14h
        jmp    SHORT loc_1_027E
loc_1_02BD:
        db  068h, 014h, 00Bh, 000h, 000h
        call   sub_1_0214
        add    esp, 4
loc_1_02CA:
        mov    ecx, 0dh
        db  0BBh, 088h, 017h, 000h, 000h
loc_1_02D4:
        mov    eax, dword ptr [ebx]
        mov    edx, 4eh
        sub    edx, ecx
        push   eax
        push   edx
        db  068h, 018h, 00Bh, 000h, 000h
        call   sub_1_0214
        add    esp, 0ch
        add    ebx, 8
        loop   loc_1_02D4
        db  068h, 0EEh, 018h, 000h, 000h
        db  068h, 024h, 00Bh, 000h, 000h
        call   sub_1_0214
        add    esp, 8
        db  0E8h, 0F8h, 0FCh, 0FEh, 0FFh
        cmp    al, 1bh
        jne    SHORT loc_1_030E
        mov    al, 58h
loc_1_030E:
        push   eax
        db  068h, 02Ch, 00Bh, 000h, 000h
        call   sub_1_0214
        add    esp, 8
        cmp    al, 61h
        jb     SHORT loc_1_0324
        sub    al, 61h
        jmp    SHORT loc_1_0326
loc_1_0324:
        sub    al, 41h
loc_1_0326:
        cmp    al, 17h
        je     SHORT loc_1_0338
        cmp    al, 0dh
        jae    SHORT loc_1_02CA
        movzx  eax, al
        db  0FFh, 014h, 0C5h, 08Ch, 017h, 000h, 000h
loc_1_0338:
        clc
        ret
        db  000h, 000h
sub_1_033C:
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        db  0BBh, 044h, 00Eh, 000h, 000h
        xor    ebp, ebp
loc_1_0349:
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        je     SHORT loc_1_0372
        test   byte ptr [ebx + 4], 80h
        je     SHORT loc_1_0349
        cmp    dword ptr [ebx + 48h], 0
        jne    SHORT loc_1_0349
        mov    edi, eax
        sub    edi, dword ptr [ebx + 28h]
        jb     SHORT loc_1_036E
        cmp    edi, eax
        je     SHORT loc_1_036E
        cmp    edi, 7d0h
        jb     SHORT loc_1_0349
loc_1_036E:
        mov    ebp, ebx
        jmp    SHORT loc_1_0349
loc_1_0372:
        or     ebp, ebp
        je     SHORT loc_1_03AD
        mov    dword ptr [ebp + 48h], 0ffffff44h
        mov    dword ptr [ebp + 23ch], esp
        lea    eax, [ebp + 0b6h]
        mov    dword ptr [ebp + 0e0h], eax
        mov    dword ptr [ebp + 0dch], 400h
        mov    esi, dword ptr [ebp + 24h]
        db  0E8h, 0EBh, 017h, 0FFh, 0FFh
        mov    ebx, 0ffffff44h
        db  0E8h, 099h, 03Fh, 0FFh, 0FFh
        jmp    SHORT sub_1_033C
loc_1_03AD:
        db  00Fh, 0BAh, 035h, 0FCh, 00Ch, 000h, 000h, 000h
        ret
loc_1_03B6:
        db  00Fh, 0BAh, 02Dh, 0FCh, 00Ch, 000h, 000h, 000h
        jb     SHORT loc_1_03C5
        call   sub_1_033C
loc_1_03C5:
        db  00Fh, 0BAh, 02Dh, 0FCh, 00Ch, 000h, 000h, 001h
        jb     SHORT loc_1_03D4
        call   sub_1_03ED
loc_1_03D4:
        mov    eax, 3e8h
        xor    edx, edx
        db  0BEh, 0B6h, 003h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 028h, 00Eh, 000h, 000h
        ret
sub_1_03ED:
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        db  08Bh, 035h, 094h, 00Dh, 000h, 000h
loc_1_03F9:
        db  081h, 0FEh, 07Ch, 00Dh, 000h, 000h
        je     SHORT loc_1_0438
        mov    edi, eax
        sub    edi, dword ptr [esi + 24h]
        jb     SHORT loc_1_0410
        cmp    edi, 7d0h
        jb     SHORT loc_1_0438
loc_1_0410:
        mov    edi, dword ptr [esi + 14h]
        mov    ebx, dword ptr [esi + 18h]
        mov    dword ptr [edi + 18h], ebx
        mov    dword ptr [ebx + 14h], edi
        mov    ebp, dword ptr [esi + 8]
        mov    ebx, esi
        xchg   dword ptr [ebp + 0e4h], ebx
        mov    dword ptr [esi + 14h], ebx
        mov    ebx, dword ptr [esi + 18h]
        mov    dword ptr [esi + 18h], 0ffffffcch
        mov    esi, ebx
        jmp    SHORT loc_1_03F9
loc_1_0438:
        db  0BDh, 044h, 00Eh, 000h, 000h
loc_1_043D:
        mov    ebp, dword ptr [ebp]
        or     ebp, ebp
        je     SHORT loc_1_0485
        cmp    dword ptr [ebp + 48h], 0
        jne    SHORT loc_1_043D
        cmp    dword ptr [ebp + 0e4h], 0
        je     SHORT loc_1_043D
        mov    dword ptr [ebp + 48h], 0ffffff44h
        mov    dword ptr [ebp + 23ch], esp
        lea    eax, [ebp + 0b6h]
        mov    dword ptr [ebp + 0e0h], eax
        mov    dword ptr [ebp + 0dch], 400h
        mov    ebx, 0ffffff44h
        db  0E8h, 0C4h, 03Eh, 0FFh, 0FFh
        jmp    NEAR PTR sub_1_03ED
loc_1_0485:
        db  00Fh, 0BAh, 035h, 0FCh, 00Ch, 000h, 000h, 001h
        ret
        db  000h, 000h
loc_1_0490:
        mov    edx, dword ptr [esp + 4]
        push   ebx
        push   edi
        push   esi
        push   ebp
        db  068h, 05Ch, 007h, 000h, 000h
loc_1_049D:
        movzx  eax, byte ptr [edx + 0b1h]
        cmp    al, 1ah
        jae    NEAR PTR loc_1_05B5
        db  08Bh, 02Ch, 085h, 0B0h, 00Dh, 000h, 000h
        mov    ebx, edx
        cmp    word ptr [edx + 68h], 0ch
        je     NEAR PTR loc_1_0606
        or     ebp, ebp
        je     NEAR PTR loc_1_064C
        mov    ecx, dword ptr [edx + 8]
        test   byte ptr [ecx + 20h], 4
        je     SHORT loc_1_04F8
        test   byte ptr [edx + 6fh], 2
        jne    SHORT loc_1_04F8
        test   byte ptr [edx + 6ch], 8
        jne    NEAR PTR loc_1_08D5
        mov    ecx, dword ptr [edx + 0a8h]
        or     ecx, ecx
        je     NEAR PTR loc_1_05A5
        cmp    dword ptr [ebp + 3ch], ecx
        jne    NEAR PTR loc_1_05DD
loc_1_04F8:
        movzx  eax, word ptr [edx + 68h]
        cmp    eax, 2ah
        jae    NEAR PTR loc_1_05B5
        db  080h, 0B8h, 010h, 007h, 000h, 000h, 000h
        je     NEAR PTR loc_1_05B5
        jl     SHORT loc_1_0583
        cmp    eax, 0dh
        je     NEAR PTR loc_1_0761
loc_1_051D:
        or     dword ptr [edx + 8ch], 0ffffffffh
        lea    eax, [ebp + 40h]
        cmp    dword ptr [eax + 8], 0
        jne    NEAR PTR loc_1_05C8
        mov    dword ptr [eax + 8], edx
loc_1_0534:
        mov    ebx, edx
        mov    dword ptr [ebp + 23ch], esp
        lea    eax, [ebx + 6ah]
        mov    dword ptr [ebp + 0e0h], eax
        xor    edx, edx
        xchg   word ptr [ebp + 0b6h], dx
        mov    word ptr [ebx + 6ah], dx
        cmp    edx, 10h
        jae    NEAR PTR loc_1_08A6
        test   byte ptr [ebp + 4], 10h
        jne    SHORT loc_1_059A
        mov    eax, dword ptr [ebx + 6ch]
        mov    dword ptr [ebp + 0dch], eax
        movzx  eax, word ptr [ebx + 68h]
        db  068h, 0A6h, 008h, 000h, 000h
        db  08Ah, 080h, 010h, 007h, 000h, 000h
        and    eax, 7fh
        db  0FFh, 024h, 085h, 03Ch, 007h, 000h, 000h
loc_1_0583:
        mov    ebx, edx
        db  068h, 0D5h, 008h, 000h, 000h
        db  08Ah, 080h, 010h, 007h, 000h, 000h
        and    eax, 7fh
        db  0FFh, 024h, 085h, 03Ch, 007h, 000h, 000h
loc_1_059A:
        mov    word ptr [ebx + 6ah], 14h
        jmp    NEAR PTR loc_1_08A6
loc_1_05A5:
        cmp    word ptr [edx + 68h], 0dh
        je     NEAR PTR loc_1_04F8
        jmp    NEAR PTR loc_1_08D5
loc_1_05B5:
        mov    ebx, edx
        db  068h, 0D5h, 008h, 000h, 000h
        cmp    word ptr [edx + 68h], 0dh
        je     NEAR PTR sub_1_0870
        ret
loc_1_05C8:
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ecx], edx
        lea    ecx, [edx + 8ch]
        mov    dword ptr [ecx], 0
        mov    dword ptr [eax + 4], ecx
        ret
loc_1_05DD:
        db  0BDh, 044h, 00Eh, 000h, 000h
loc_1_05E2:
        mov    ebp, dword ptr [ebp]
        or     ebp, ebp
        je     NEAR PTR loc_1_08D5
        cmp    byte ptr [ebp + 88h], al
        jne    SHORT loc_1_05E2
        cmp    dword ptr [ebp + 3ch], ecx
        jne    SHORT loc_1_05E2
        db  089h, 02Ch, 085h, 0B0h, 00Dh, 000h, 000h
        jmp    NEAR PTR loc_1_04F8
loc_1_0606:
        or     ebp, ebp
        je     SHORT loc_1_0610
        mov    al, byte ptr [ebp + 89h]
loc_1_0610:
        db  0BDh, 044h, 00Eh, 000h, 000h
loc_1_0615:
        mov    ebp, dword ptr [ebp]
        or     ebp, ebp
        je     NEAR PTR loc_1_08D5
        cmp    byte ptr [ebp + 89h], al
        jne    SHORT loc_1_0615
        test   byte ptr [ebp + 6], 2
        jne    SHORT loc_1_0615
        cmp    dword ptr [ebp + 48h], 0
        jne    SHORT loc_1_0615
        test   byte ptr [ebp + 4], 80h
        jne    SHORT loc_1_0643
        push   eax
        push   ebx
        db  0E8h, 046h, 040h, 0FFh, 0FFh
        pop    ebx
        pop    eax
loc_1_0643:
        and    dword ptr [ebp + 4], 0ffffdfffh
        jmp    SHORT loc_1_0615
loc_1_064C:
        cmp    word ptr [ebx + 68h], 0dh
        jne    NEAR PTR loc_1_08D5
        cmp    word ptr [ebx + 92h], 440dh
        jne    NEAR PTR loc_1_08D5
        cmp    dword ptr [ebx + 94h], 872h
        je     SHORT loc_1_0682
        cmp    dword ptr [ebx + 94h], 4872h
        jne    NEAR PTR loc_1_08D5
loc_1_0682:
        test   byte ptr [ebx + 6ch], 10h
        je     NEAR PTR loc_1_08D5
        movzx  eax, byte ptr [ebx + 0b1h]
        mov    edx, dword ptr [ebx + 9ch]
        mov    edx, dword ptr [edx + 14h]
        cmp    dl, 1
        ja     NEAR PTR loc_1_08D5
        cmp    dh, 1
        ja     NEAR PTR loc_1_08D5
        mov    ecx, 1ah
loc_1_06B3:
        db  08Bh, 02Ch, 08Dh, 0ACh, 00Dh, 000h, 000h
        or     ebp, ebp
        je     SHORT loc_1_06D1
        cmp    byte ptr [ebp + 89h], al
        jne    SHORT loc_1_06D1
        test   dl, 1
        jne    SHORT loc_1_06F9
        test   byte ptr [ebp + 6], 4
        jne    SHORT loc_1_06E1
loc_1_06D1:
        loop   loc_1_06B3
        test   dl, 2
        jne    NEAR PTR loc_1_08AE
        jmp    NEAR PTR loc_1_08D5
loc_1_06E1:
        test   byte ptr [ebp + 4], 1
        je     SHORT loc_1_06D1
        and    dword ptr [ebx + 0a0h], 0
        mov    word ptr [ebx + 6ah], 0
        jmp    NEAR PTR loc_1_08AE
loc_1_06F9:
        or     dl, 2
        and    dword ptr [ebp + 4], 0fffbffffh
        or     dh, dh
        je     SHORT loc_1_06D1
        or     dword ptr [ebp + 4], 40000h
        jmp    SHORT loc_1_06D1
loc_1_0710:
        add    dword ptr [edx], eax
        add    dword ptr [eax], -7dh
        add    byte ptr [eax + eax], al
        add    byte ptr [0], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi], al
        push   es
        pop    es
        add    byte ptr [eax], al
        db  089h, 008h, 000h, 000h, 0FFh, 03Ch, 000h, 000h, 0E4h, 03Ch, 000h, 000h, 04Bh, 009h, 000h, 000h
        db  089h, 008h, 000h, 000h, 086h, 03Eh, 000h, 000h, 08Ah, 008h, 000h, 000h, 069h, 03Fh, 000h, 000h
loc_1_075C:
        pop    ebp
        pop    esi
        pop    edi
        pop    ebx
        ret
loc_1_0761:
        mov    ebx, edx
        movzx  eax, word ptr [ebx + 92h]
        cmp    ax, 440dh
        jne    SHORT loc_1_07C5
        cmp    dword ptr [ebx + 94h], 4860h
        je     SHORT loc_1_078C
        cmp    dword ptr [ebx + 94h], 860h
        jne    NEAR PTR loc_1_08D5
loc_1_078C:
        mov    eax, dword ptr [ebp + 38h]
        test   byte ptr [eax + 20h], 4
        je     SHORT loc_1_07A4
        mov    eax, dword ptr [ebx + 98h]
        test   byte ptr [eax], 1
        je     NEAR PTR loc_1_08D5
loc_1_07A4:
        mov    eax, dword ptr [ebx + 14h]
        db  0C7h, 000h, 0B8h, 042h, 000h, 000h
        mov    dword ptr [eax + 4], ebp
        add    dword ptr [ebx + 14h], 8
        mov    al, byte ptr [ebp + 89h]
        mov    byte ptr [ebx + 0b1h], al
        jmp    NEAR PTR loc_1_08D5
loc_1_07C5:
        cmp    ax, 4404h
        jne    NEAR PTR loc_1_08D5
        cmp    dword ptr [ebx + 94h], 5
        jb     NEAR PTR loc_1_08D5
        mov    esi, dword ptr [ebx + 98h]
        cmp    word ptr [esi], 444dh
        jne    NEAR PTR loc_1_08D5
        mov    al, byte ptr [esi + 2]
        mov    ecx, 80000000h
        cmp    al, 72h
        je     SHORT loc_1_0803
        cmp    al, 77h
        jne    NEAR PTR loc_1_051D
        xor    ecx, ecx
loc_1_0803:
        test   byte ptr [esi + 5], 1
        jne    NEAR PTR loc_1_051D
        mov    word ptr [esi + 3], 2
        mov    edx, dword ptr [ebx + 94h]
        db  0E8h, 0DEh, 00Ah, 0FFh, 0FFh
        mov    edx, ebx
        mov    dword ptr [ebx + 98h], esi
        jae    NEAR PTR loc_1_049D
        mov    word ptr [ebx + 6ah], 1dh
        jmp    SHORT loc_1_08AE
sub_1_0834:
        cmp    word ptr [ebx + 68h], 0dh
        jne    SHORT loc_1_086E
        cmp    word ptr [ebx + 92h], 4404h
        jne    SHORT loc_1_086E
        cmp    dword ptr [ebx + 94h], 5
        jb     SHORT loc_1_086E
        mov    esi, dword ptr [ebx + 98h]
        cmp    word ptr [esi], 444dh
        jne    SHORT loc_1_086E
        mov    al, byte ptr [esi + 2]
        cmp    al, 72h
        je     SHORT loc_1_0867
        cmp    al, 77h
        jne    SHORT loc_1_086E
loc_1_0867:
        test   byte ptr [esi + 5], 1
        stc
        jne    SHORT loc_1_086F
loc_1_086E:
        clc
loc_1_086F:
        ret
sub_1_0870:
        call   sub_1_0834
        jae    SHORT loc_1_0888
        mov    esi, dword ptr [ebx + 98h]
        mov    edx, dword ptr [ebx + 94h]
        db  0E8h, 02Ah, 00Bh, 0FFh, 0FFh
loc_1_0888:
        ret
loc_1_0889:
        ret
loc_1_088A:
        call   sub_1_093E
        jb     SHORT loc_1_08A5
        mov    ebx, dword ptr [ebp + 48h]
        cmp    word ptr [ebx + 68h], 28h
        jne    SHORT loc_1_08CC
        db  0E8h, 0E7h, 03Dh, 0FFh, 0FFh
        mov    ebx, dword ptr [ebp + 48h]
        jmp    SHORT loc_1_08CC
loc_1_08A5:
        ret
loc_1_08A6:
        mov    ebx, dword ptr [ebp + 48h]
        db  0E8h, 096h, 03Ah, 0FFh, 0FFh
loc_1_08AE:
        cmp    word ptr [ebx + 68h], 0dh
        je     SHORT loc_1_08C5
loc_1_08B5:
        mov    eax, dword ptr [ebx + 14h]
        sub    eax, 8
        mov    dword ptr [ebx + 14h], eax
        push   ebx
        call   dword ptr [eax]
        add    esp, 4
        ret
loc_1_08C5:
        call   sub_1_0870
        jmp    SHORT loc_1_08B5
loc_1_08CC:
        pop    eax
        mov    ebx, dword ptr [ebp + 48h]
        db  0E8h, 06Fh, 03Ah, 0FFh, 0FFh
loc_1_08D5:
        mov    eax, dword ptr [ebx + 8]
        test   byte ptr [eax + 1dh], 20h
        jne    SHORT loc_1_08EE
loc_1_08DE:
        mov    eax, dword ptr [ebx + 10h]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebx + 10h], eax
        push   ebx
        call   dword ptr [eax]
        add    esp, 4
        ret
loc_1_08EE:
        test   byte ptr [ebx + 6eh], 1
        jne    SHORT loc_1_08DE
        cmp    byte ptr [ebx + 0b1h], 1
        jbe    SHORT loc_1_08DE
        movzx  ecx, byte ptr [ebx + 0b1h]
        db  08Bh, 004h, 08Dh, 0B0h, 00Dh, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_0918
        movzx  ecx, byte ptr [eax + 8ah]
        jmp    SHORT loc_1_0936
loc_1_0918:
        db  0B8h, 044h, 00Eh, 000h, 000h
loc_1_091D:
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     SHORT loc_1_08DE
        cmp    byte ptr [eax + 89h], cl
        jne    SHORT loc_1_091D
        mov    cl, byte ptr [eax + 8ah]
        cmp    cl, 1
        ja     SHORT loc_1_08DE
loc_1_0936:
        mov    byte ptr [ebx + 0b1h], cl
        jmp    SHORT loc_1_08DE
sub_1_093E:
        db  0E8h, 049h, 012h, 0FFh, 0FFh
        jb     SHORT loc_1_094A
        db  0E8h, 0A0h, 013h, 0FFh, 0FFh
loc_1_094A:
        ret
loc_1_094B:
        mov    word ptr [ebx + 6ah], 0
        pop    eax
        jmp    NEAR PTR loc_1_08AE
        db  000h
sub_1_0958:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_096C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0974:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_098C:
        db  08Bh, 035h, 028h, 00Eh, 000h, 000h
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        xor    edx, edx
        db  088h, 015h, 063h, 00Eh, 000h, 000h
        db  0BEh, 0CAh, 009h, 000h, 000h
        db  0CDh, 020h, 010h, 000h, 001h, 000h
        db  0CDh, 020h, 083h, 000h, 001h, 000h
loc_1_09B1:
        db  080h, 03Dh, 063h, 00Eh, 000h, 000h, 000h
        jne    SHORT loc_1_09C2
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        jmp    SHORT loc_1_09B1
loc_1_09C2:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        clc
        ret
loc_1_09CA:
        push   ebp
loc_1_09CB:
        cmp    edx, 1ah
        jae    SHORT loc_1_0A3C
        db  08Bh, 02Ch, 095h, 0B0h, 00Dh, 000h, 000h
        inc    edx
        or     ebp, ebp
        je     SHORT loc_1_09CB
        push   edx
        mov    edx, 1
        db  0E8h, 0F0h, 040h, 0FFh, 0FFh
        pop    ecx
        jb     SHORT loc_1_09CB
        mov    dword ptr [esi + 80h], ecx
        dec    ecx
        xor    eax, eax
        mov    word ptr [esi + 68h], 28h
        mov    dword ptr [esi + 0ach], eax
        mov    byte ptr [esi + 0b0h], al
        mov    dword ptr [esi + 64h], eax
        mov    dword ptr [esi + 0a8h], eax
        mov    word ptr [esi + 6ah], ax
        mov    dword ptr [esi + 7ch], eax
        mov    dword ptr [esi + 6ch], 400h
        mov    byte ptr [esi + 0b1h], cl
        db  0C7h, 046h, 070h, 045h, 00Ah, 000h, 000h
        add    esi, 64h
        neg    edx
        lea    edx, [edx + esi]
        mov    dword ptr [esi + 20h], edx
        db  0CDh, 020h, 004h, 000h, 010h, 000h
loc_1_0A3A:
        pop    ebp
        ret
loc_1_0A3C:
        db  0C6h, 005h, 063h, 00Eh, 000h, 000h, 001h
        jmp    SHORT loc_1_0A3A
loc_1_0A45:
        mov    edx, dword ptr [esi + 1ch]
        mov    esi, dword ptr [esi + 20h]
        sub    esp, 8
        mov    dword ptr [esp + 4], esi
        mov    word ptr [esp], 4
        mov    esi, esp
        push   esi
        db  0FFh, 015h, 058h, 009h, 000h, 000h
        add    esp, 0ch
        db  0BEh, 0CAh, 009h, 000h, 000h
        push   eax
        push   ebx
        push   ecx
        xor    eax, eax
        mov    ebx, eax
        mov    ecx, 408h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        pop    ecx
        pop    ebx
        pop    eax
        ret
loc_1_0A7F:
        db  0E8h, 0FCh, 010h, 0FFh, 0FFh
        ret
        db  000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 003h, 00Ah, 000h, 000h, 044h, 052h, 056h, 053h
        db  050h, 041h, 043h, 058h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 09Ch, 007h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0AE8:
        push   ebx
        db  075h, 070h
        jo     SHORT loc_1_0B5C
        jb     SHORT loc_1_0B63
        db  073h, 020h
        xor    ebp, dword ptr [esi]
        xor    byte ptr [eax], ah
        inc    ebx
        push   esi
        inc    esi
        db  073h, 02Eh
        pop    esp
        outsb
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0B00:
        and    byte ptr [eax], ah
        and    eax, 3d632563h
        and    byte ptr ds:[25282063h], ah
        arpl   word ptr [ecx], bp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0B14:
        pop    esp
        outsb
        add    byte ptr [eax], al
loc_1_0B18:
        pop    ebx
        and    eax, 2d205d63h
        and    byte ptr [6e5c73h], ah
loc_1_0B24:
        and    eax, 3f6e5c73h
        and    byte ptr [eax], al
        db  000h
loc_1_0B2C:
        and    eax, 6e5c63h
        add    byte ptr [eax], al
        db  000h
loc_1_0B34:
        pop    edx
        db  065h, 072h, 06Fh
        and    byte ptr [esi + 69h], ah
        insb
        db  064h, 073h, 020h
        db  077h, 069h
        insb
        insb
        and    byte ptr [20656273h], ah
        imul   esi, dword ptr fs:[ebx + 70h], 6579616ch
        pop    esp
        outsb
        add    byte ptr [eax], al
loc_1_0B58:
        push   ebx
        outsd
        jb     SHORT loc_1_0BCE
loc_1_0B5C:
        jns    SHORT loc_1_0B8A
        and    byte ptr [ecx + 6fh], bh
        db  075h, 020h
loc_1_0B63:
        outsb
        and    byte ptr fs:[eax + ebp*2 + 65h], dh
        and    byte ptr [ebp + 62h], ah
        db  075h, 067h
        and    byte ptr [edx + esi*2 + 69h], ah
        jbe    SHORT loc_1_0BDC
        db  072h, 020h
        outsw
        jb     SHORT loc_1_0B9D
        db  074h, 068h
        imul   esi, dword ptr [ebx + 2eh], 6e5ch
        add    byte ptr [eax], al
loc_1_0B88:
        pop    esp
        outsb
loc_1_0B8A:
        and    eax, 64412073h
        db  064h, 072h, 065h
        db  073h, 073h
        and    byte ptr [38302520h], bh
        insb
        js     SHORT loc_1_0BF9
loc_1_0B9D:
        outsb
        add    byte ptr [eax], al
loc_1_0BA0:
        and    eax, 203d2073h
        and    eax, 786c3430h
        add    byte ptr [eax], al
loc_1_0BAC:
        and    eax, 203d2073h
        and    eax, 786c3830h
        add    byte ptr [eax], al
loc_1_0BB8:
        and    eax, 203d2073h
        add    byte ptr [eax], al
        add    byte ptr [786c3230h], ah
        add    byte ptr [eax], al
        add    byte ptr [203d2073h], ah
        daa
loc_1_0BCE:
        add    byte ptr [eax], al
loc_1_0BD0:
        and    eax, 25000063h
        db  073h, 020h
        cmp    eax, 0
loc_1_0BDC:
        and    byte ptr [270073h], ah
        add    byte ptr [eax], al
loc_1_0BE4:
        and    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BE8:
        pop    esp
        outsb
        add    byte ptr [eax], al
loc_1_0BEC:
        pop    esp
        outsb
        add    byte ptr [eax], al
loc_1_0BF0:
        sub    cl, byte ptr [eax + 45h]
        dec    esp
        inc    esp
        sub    bl, byte ptr [esi + ebp*2]
loc_1_0BF9:
        add    byte ptr [eax], al
        db  000h
loc_1_0BFC:
        arpl   word ptr [ecx + 5fh], sp
        dec    eax
        outsd
        insb
        and    byte ptr fs:[32302520h], bh
        insb
        db  078h, 05Ch
        outsb
        add    byte ptr [eax], al
        add    byte ptr [edx], ch
        dec    eax
        inc    ebp
        dec    esp
        inc    esp
        sub    bl, byte ptr [esi + ebp*2]
        add    byte ptr [eax], al
        add    byte ptr [ebx + 63h], ah
        pop    edi
        dec    eax
        outsd
        insb
        and    byte ptr fs:[32302520h], bh
        insb
        db  078h, 05Ch
        outsb
        add    byte ptr [eax], al
        add    byte ptr [ebx + 6fh], dl
        db  072h, 072h
        jns    SHORT loc_1_0C62
        and    byte ptr [ecx + 6fh], bh
        db  075h, 020h
        outsb
        and    byte ptr fs:[eax + ebp*2 + 65h], dh
        and    byte ptr [ebp + 62h], ah
        db  075h, 067h
        and    byte ptr [edx + esi*2 + 69h], ah
        db  076h, 065h
        db  072h, 020h
        outsw
        db  072h, 020h
        db  074h, 068h
        imul   esi, dword ptr [ebx + 2eh], 6e5ch
        add    byte ptr [eax], al
loc_1_0C60:
        push   ebx
        outsd
loc_1_0C62:
        db  072h, 072h
        jns    SHORT loc_1_0C92
        and    byte ptr [ecx + 6fh], bh
        db  075h, 020h
        outsb
        and    byte ptr fs:[eax + ebp*2 + 65h], dh
        and    byte ptr [ebp + 62h], ah
        db  075h, 067h
        and    byte ptr [edx + esi*2 + 69h], ah
        db  076h, 065h
        db  072h, 020h
        outsw
        db  072h, 020h
        db  074h, 068h
        imul   esi, dword ptr [ebx + 2eh], 6e5ch
        add    byte ptr [eax], al
loc_1_0C90:
        push   ebx
        outsd
loc_1_0C92:
        db  072h, 072h
        jns    SHORT loc_1_0CC2
        and    byte ptr [ecx + 6fh], bh
        db  075h, 020h
        outsb
        and    byte ptr fs:[eax + ebp*2 + 65h], dh
        and    byte ptr [ebp + 62h], ah
        jne    SHORT loc_1_0D10
        and    byte ptr [edx + esi*2 + 69h], ah
        jbe    SHORT loc_1_0D14
        db  072h, 020h
        outsw
        db  072h, 020h
        je     SHORT loc_1_0D1F
        imul   esi, dword ptr [ebx + 2eh], 6e5ch
        add    byte ptr [eax], al
loc_1_0CC0:
        push   ebx
        outsd
loc_1_0CC2:
        db  072h, 072h
        jns    SHORT loc_1_0CF2
        and    byte ptr [ecx + 6fh], bh
        db  075h, 020h
        outsb
        and    byte ptr fs:[eax + ebp*2 + 65h], dh
        and    byte ptr [ebp + 62h], ah
        jne    SHORT loc_1_0D40
        and    byte ptr [edx + esi*2 + 69h], ah
        db  076h, 065h
        jb     SHORT loc_1_0D01
        outsw
        jb     SHORT loc_1_0D05
        je     SHORT loc_1_0D4F
        imul   esi, dword ptr [ebx + 2eh], 6e5ch
        add    byte ptr [eax], al
loc_1_0CF0:
        add    byte ptr [eax], al
loc_1_0CF2:
        add    byte ptr [eax], al
        db  0F0h, 00Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0D01:
        add    byte ptr [eax], al
        db  000h, 000h
loc_1_0D05:
        or     eax, 0
        add    byte ptr [eax], al
loc_1_0D0C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0D10:
        or     al, 0dh
        add    byte ptr [eax], al
loc_1_0D14:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
loc_1_0D1F:
        add    byte ptr [eax], ch
        add    byte ptr [eax], al
        push   0
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0D40:
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        db  000h
loc_1_0D4F:
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 058h, 00Dh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0D64:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0D68:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0D6C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0D70:
        add    byte ptr [eax], al
loc_1_0D72:
        add    byte ptr [eax], al
loc_1_0D74:
        add    dword ptr [ecx], eax
loc_1_0D76:
        add    byte ptr [eax], al
loc_1_0D78:
        add    byte ptr [eax], al
loc_1_0D7A:
        add    byte ptr [eax], al
loc_1_0D7C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0D90:
        db  07Ch, 00Dh
        add    byte ptr [eax], al
loc_1_0D94:
        db  07Ch, 00Dh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0DAC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0DB0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0E18:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0E1C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0E20:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0E24:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0E28:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0E2C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0E30:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0E34:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 00Fh, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 0F0h, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 048h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0E5F:
        add    dword ptr [ebx], eax
        add    al, byte ptr [esi]
loc_1_0E63:
        add    byte ptr [eax], al
loc_1_0E65:
        add    byte ptr [eax], al
loc_1_0E67:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
_LTEXT ends
        end
