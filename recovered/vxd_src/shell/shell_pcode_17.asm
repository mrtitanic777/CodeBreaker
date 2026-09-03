; shell_pcode_17.asm -- SHELL object 17 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

SHELL_Event:
        pushad
        call   sub_17_0A20
        mov    dword ptr [esp + 1ch], eax
        add    eax, 1
        popad
        ret
sub_17_000F:
        db  0A1h, 000h, 00Ah, 000h, 000h
        lea    eax, [eax + ebx + 0ch]
        ret
        db  000h, 000h, 000h
SHELL_CheckHotkeyAllowed:
        push   ebx
        db  003h, 01Dh, 000h, 00Ah, 000h, 000h
        bt     dword ptr [ebx + 0ch], 1bh
        pop    ebx
        ret
sub_17_002A:
        db  0CDh, 020h, 01Ah, 000h, 017h, 000h
        jae    SHORT loc_17_0038
        db  0E8h, 0C9h, 0FFh, 0EEh, 0FFh
        stc
loc_17_0038:
        ret
        db  000h, 000h, 000h
loc_17_003C:
        call   sub_17_000F
        xchg   edi, eax
        inc    dword ptr [ebp + 24h]
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        mov    ecx, dword ptr [eax + 2]
        and    cx, 7f7fh
        bts    dword ptr [edi + 4], 10h
        jb     NEAR PTR loc_17_00E8
        cmp    cx, 3
        jbe    SHORT loc_17_0075
        cmp    cx, 7
        je     SHORT loc_17_0075
        btr    dword ptr [edi + 4], 1ch
        jmp    SHORT loc_17_007B
loc_17_0075:
        bts    dword ptr [edi + 4], 1ch
        cmc
loc_17_007B:
        jae    SHORT loc_17_00E4
        test   byte ptr [edi + 7], 10h
        jne    SHORT loc_17_009E
        xchg   ebx, eax
        db  0CDh, 020h, 008h, 000h, 00Dh, 000h
        xchg   ebx, eax
        xor    eax, ebx
        jne    SHORT loc_17_00E4
        and    byte ptr [edi + 3], 0dfh
        test   byte ptr [edi], 4
        je     SHORT loc_17_00E4
        or     byte ptr [edi + 3], 20h
        jmp    SHORT loc_17_00AC
loc_17_009E:
        test   byte ptr [edi], 4
        jne    SHORT loc_17_00E4
        test   byte ptr [edi + 3], 20h
        je     SHORT loc_17_00E4
        xor    eax, eax
        inc    eax
loc_17_00AC:
        or     eax, 200000h
        or     byte ptr [edi + 6], 2
        mov    edx, ebx
        db  0BEh, 0A0h, 002h, 000h, 000h
        mov    ecx, 42bh
        db  0CDh, 020h, 002h, 000h, 017h, 000h
        jb     SHORT loc_17_00E4
        jmp    SHORT loc_17_00D6
loc_17_00CB:
        push   5
        push   edi
        db  0CDh, 020h, 0FAh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_17_00D6:
        db  0CDh, 020h, 029h, 000h, 001h, 000h
        jb     SHORT loc_17_00E4
        test   byte ptr [edi + 6], 2
        jne    SHORT loc_17_00CB
loc_17_00E4:
        and    byte ptr [edi + 6], 0feh
loc_17_00E8:
        ret
loc_17_00E9:
        or     edx, edx
        jne    NEAR PTR loc_17_01A5
        push   edi
        push   esi
        call   sub_17_000F
        btr    dword ptr [eax], 2
        jae    SHORT loc_17_0118
        and    byte ptr [eax], 0f7h
        xor    eax, eax
        db  0CDh, 020h, 004h, 000h, 00Ah, 000h
        mov    ecx, 416h
        xor    eax, eax
        xor    esi, esi
        db  0CDh, 020h, 002h, 000h, 017h, 000h
loc_17_0118:
        pop    esi
        pop    edi
        mov    ecx, 415h
        mov    eax, 100000h
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_17_015A
        mov    cx, 420h
        xchg   edi, ebx
        cmp    esi, 2
        je     SHORT loc_17_015A
        inc    eax
        cmp    esi, 1
        jb     SHORT loc_17_0152
        mov    ecx, 421h
        or     ebx, ebx
        je     SHORT loc_17_0152
        cmp    ebx, edi
        je     SHORT loc_17_0152
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jae    SHORT loc_17_015A
loc_17_0152:
        xchg   edi, ebx
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
loc_17_015A:
        db  066h, 00Fh, 0BAh, 02Dh, 06Dh, 00Bh, 000h, 000h, 002h
        jae    SHORT loc_17_017E
        db  066h, 03Bh, 005h, 06Eh, 00Bh, 000h, 000h
        jne    SHORT loc_17_017E
        db  03Bh, 01Dh, 070h, 00Bh, 000h, 000h
        jne    SHORT loc_17_017E
        db  03Bh, 00Dh, 074h, 00Bh, 000h, 000h
        je     SHORT loc_17_01A5
loc_17_017E:
        db  089h, 01Dh, 070h, 00Bh, 000h, 000h
        db  089h, 00Dh, 074h, 00Bh, 000h, 000h
        db  066h, 0A3h, 06Eh, 00Bh, 000h, 000h
        db  0BEh, 0D5h, 002h, 000h, 000h
        xor    edx, edx
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_17_01A5
        db  0CDh, 020h, 002h, 000h, 017h, 000h
loc_17_01A5:
        clc
        ret
sub_17_01A7:
        push   ebx
        push   esi
        push   edi
        call   sub_17_0212
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  0CDh, 020h, 004h, 000h, 001h, 000h, 074h, 01Bh, 08Bh, 0DAh, 0B8h, 002h, 000h, 010h, 000h, 0B9h
        db  014h, 004h, 000h, 000h, 0BEh, 016h, 00Ah, 000h, 000h, 033h, 0D2h, 0CDh, 020h, 002h, 000h, 017h
        db  000h, 073h, 005h, 0E8h, 023h, 0FEh, 0EEh, 0FFh, 0C3h
loc_17_01DE:
        cmp    word ptr [ebp + 14h], 26h
        je     SHORT loc_17_01FE
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_17_01FA
        test   byte ptr [ebx], 20h
        je     SHORT loc_17_01FA
        cmp    word ptr [ebp + 14h], 100h
        jb     SHORT loc_17_01FE
loc_17_01FA:
        or     eax, 0ffffffffh
        ret
loc_17_01FE:
        db  0E8h, 0FDh, 0FDh, 0EEh, 0FFh
        xor    eax, eax
        ret
        db  000h, 000h
loc_17_0208:
        mov    ebx, dword ptr [ebp + 10h]
        and    byte ptr [ebp + 2ch], 0feh
        xor    eax, eax
        inc    eax
sub_17_0212:
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jae    SHORT loc_17_0224
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        or     byte ptr [ebp + 2ch], 1
loc_17_0224:
        push   ebx
        push   eax
        call   sub_17_000F
        mov    esi, eax
        push   5
        pop    ecx
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        xor    eax, eax
        mov    al, 0fh
        lea    edx, [eax - 2]
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_17_025E
        cmp    dword ptr [esp], 0
        je     SHORT loc_17_0251
        db  0CDh, 020h, 031h, 000h, 001h, 000h
loc_17_0251:
        test   byte ptr [esi], 4
        je     SHORT loc_17_0264
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        jmp    SHORT loc_17_0264
loc_17_025E:
        db  0CDh, 020h, 093h, 000h, 001h, 000h
loc_17_0264:
        dec    edx
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        dec    edx
        dec    edx
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        cmp    ebx, dword ptr [esp + 4]
        jne    SHORT loc_17_02B3
        call   sub_17_035F
loc_17_027E:
        mov    ebx, dword ptr [esp + 4]
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_17_0298
        cmp    dword ptr [esp], 0
        je     SHORT loc_17_02A5
        db  0CDh, 020h, 031h, 000h, 001h, 000h
        jmp    SHORT loc_17_02A5
loc_17_0298:
        xor    eax, eax
        mov    al, 0fh
        lea    edx, [eax - 2]
        db  0CDh, 020h, 093h, 000h, 001h, 000h
loc_17_02A5:
        call   sub_17_0B64
        db  0CDh, 020h, 020h, 000h, 001h, 000h
        pop    eax
        pop    eax
        ret
loc_17_02B3:
        db  0CDh, 020h, 032h, 000h, 001h, 000h
        db  080h, 03Dh, 0E3h, 000h, 000h, 000h, 000h
        je     SHORT loc_17_02CF
        db  0C6h, 005h, 0E3h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 0ACh, 00Bh, 000h, 000h
loc_17_02CF:
        or     eax, 202h
        db  080h, 03Dh, 0E2h, 000h, 000h, 000h, 000h
        jne    SHORT loc_17_02E6
        movzx  edx, dx
        db  089h, 015h, 0A8h, 00Bh, 000h, 000h
loc_17_02E6:
        push   eax
        call   sub_17_000F
        mov    esi, eax
        pop    eax
        db  08Bh, 03Dh, 0A8h, 00Bh, 000h, 000h
loc_17_02F5:
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_17_0326
        cmp    ebx, dword ptr [esp + 4]
        je     SHORT loc_17_031A
        test   byte ptr [ebx], 2
        je     SHORT loc_17_02F5
        cmp    di, word ptr [esi + 20h]
        jae    SHORT loc_17_02F5
        mov    di, word ptr [esi + 20h]
        jmp    SHORT loc_17_02F5
loc_17_031A:
        cmp    di, word ptr [esi + 1eh]
        jae    SHORT loc_17_02F5
        mov    di, word ptr [esi + 1eh]
        jmp    SHORT loc_17_02F5
loc_17_0326:
        shr    di, 1
        movzx  edi, di
        db  03Bh, 03Dh, 0A8h, 00Bh, 000h, 000h
        jbe    SHORT loc_17_0347
        mov    edx, edi
        db  080h, 03Dh, 0E2h, 000h, 000h, 000h, 000h
        jne    SHORT loc_17_0354
        db  0FEh, 005h, 0E2h, 000h, 000h, 000h
        jmp    SHORT loc_17_0354
loc_17_0347:
        db  08Bh, 015h, 0A8h, 00Bh, 000h, 000h
        db  0C6h, 005h, 0E2h, 000h, 000h, 000h, 000h
loc_17_0354:
        db  0CDh, 020h, 033h, 000h, 001h, 000h
        jmp    NEAR PTR loc_17_027E
sub_17_035F:
        push   ebx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 032h, 000h, 001h, 000h
        and    eax, 0fffffdffh
        or     eax, 2
        db  080h, 03Dh, 0E2h, 000h, 000h, 000h, 000h
        je     SHORT loc_17_038A
        db  0C6h, 005h, 0E2h, 000h, 000h, 000h, 000h
        db  08Bh, 015h, 0A8h, 00Bh, 000h, 000h
loc_17_038A:
        cmp    ebx, dword ptr [esp]
        jne    SHORT loc_17_03FE
        mov    esi, eax
        call   sub_17_000F
        xchg   esi, eax
        db  080h, 03Dh, 0E3h, 000h, 000h, 000h, 000h
        jne    SHORT loc_17_03A9
        movzx  ecx, cx
        db  089h, 00Dh, 0ACh, 00Bh, 000h, 000h
loc_17_03A9:
        db  08Bh, 03Dh, 0ACh, 00Bh, 000h, 000h
loc_17_03AF:
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_17_03CE
        test   byte ptr [ebx], 2
        je     SHORT loc_17_03AF
        cmp    di, word ptr [esi + 20h]
        jae    SHORT loc_17_03AF
        mov    di, word ptr [esi + 20h]
        jmp    SHORT loc_17_03AF
loc_17_03CE:
        shr    di, 1
        movzx  edi, di
        db  03Bh, 03Dh, 0ACh, 00Bh, 000h, 000h
        jbe    SHORT loc_17_03EF
        mov    ecx, edi
        db  080h, 03Dh, 0E3h, 000h, 000h, 000h, 000h
        jne    SHORT loc_17_0414
        db  0FEh, 005h, 0E3h, 000h, 000h, 000h
        jmp    SHORT loc_17_0414
loc_17_03EF:
        db  08Bh, 00Dh, 0ACh, 00Bh, 000h, 000h
        db  0C6h, 005h, 0E3h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_17_0414
loc_17_03FE:
        db  080h, 03Dh, 0E3h, 000h, 000h, 000h, 000h
        je     SHORT loc_17_0414
        db  0C6h, 005h, 0E3h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 0ACh, 00Bh, 000h, 000h
loc_17_0414:
        db  0CDh, 020h, 033h, 000h, 001h, 000h
        pop    ebx
        ret
loc_17_041C:
        ret
loc_17_041D:
        or     byte ptr [ebp + 2ch], 1
        mov    ax, 3800h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        inc    eax
        je     SHORT loc_17_041C
        dec    eax
        xchg   edi, eax
        mov    ebx, dword ptr [ebp + 10h]
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     SHORT loc_17_041C
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_17_041C
        push   5
        pop    ecx
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        call   sub_17_000F
        push   eax
        db  0CDh, 020h, 032h, 000h, 001h, 000h
        pop    esi
        mov    edx, eax
        and    eax, 3
        and    byte ptr [esi], 0fch
        or     dword ptr [esi], eax
        mov    ecx, dword ptr [esi]
        xor    ecx, dword ptr [edi]
        and    ecx, 1fh
        test   cl, 8
        je     SHORT loc_17_047A
        mov    eax, dword ptr [edi]
        and    eax, 8
        db  0CDh, 020h, 004h, 000h, 00Ah, 000h
loc_17_047A:
        test   cl, 10h
        je     SHORT loc_17_0494
        test   byte ptr [edi], 10h
        jne    SHORT loc_17_048C
        db  0CDh, 020h, 02Ch, 000h, 001h, 000h
        jmp    SHORT loc_17_0492
loc_17_048C:
        db  0CDh, 020h, 02Bh, 000h, 001h, 000h
loc_17_0492:
        jb     SHORT loc_17_04AC
loc_17_0494:
        xor    dword ptr [esi], ecx
        mov    eax, dword ptr [esi]
        and    eax, 3
        xor    ecx, ecx
        inc    ecx
        mov    edx, ecx
        db  0CDh, 020h, 033h, 000h, 001h, 000h
        jb     SHORT loc_17_04AC
        and    byte ptr [ebp + 2ch], 0feh
loc_17_04AC:
        db  0CDh, 020h, 020h, 080h, 001h, 000h
loc_17_04B2:
        or     byte ptr [ebp + 2ch], 1
        mov    ax, 3800h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        inc    edi
        je     SHORT loc_17_050B
        dec    edi
        mov    ebx, dword ptr [ebp + 10h]
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     SHORT loc_17_050B
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_17_050B
        call   sub_17_000F
        push   eax
        db  0CDh, 020h, 032h, 000h, 001h, 000h
        pop    ebx
        and    eax, 3
        mov    ecx, dword ptr [ebx]
        and    ecx, 0ch
        or     eax, ecx
        and    byte ptr [edi], 0f0h
        and    byte ptr [ebx], 0f0h
        or     dword ptr [edi], eax
        or     dword ptr [ebx], eax
        lea    esi, [ebx + 2eh]
        add    edi, 2eh
        mov    ecx, 8
        cld
        rep movsd
        and    byte ptr [ebp + 2ch], 0feh
loc_17_050B:
        ret
loc_17_050C:
        mov    eax, dword ptr [ebp]
        or     eax, eax
        je     SHORT loc_17_054A
        test   byte ptr [eax + 0bh], 80h
        jne    SHORT loc_17_054A
        sbb    edi, edi
        db  08Bh, 035h, 074h, 00Ah, 000h, 000h
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        jb     SHORT loc_17_054A
        push   dword ptr [eax + 8]
        mov    ecx, dword ptr [eax]
        mov    edx, dword ptr [eax + 4]
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        add    edi, 1
        call   ecx
        pop    edx
        test   edx, 100000h
        je     SHORT loc_17_054A
        call   sub_17_0B64
loc_17_054A:
        ret
loc_17_054B:
        mov    ebx, dword ptr [ebp + 10h]
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     SHORT loc_17_057D
        mov    edx, ebx
        db  003h, 015h, 06Ch, 00Ah, 000h, 000h
        bts    dword ptr [edx + 10h], 5
        jb     SHORT loc_17_057D
        test   byte ptr [edx + 10h], 40h
        jne    SHORT loc_17_057D
        xor    eax, eax
        db  0BEh, 07Eh, 005h, 000h, 000h
        lea    ecx, [eax + 63h]
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        mov    dword ptr [edx], esi
loc_17_057D:
        ret
loc_17_057E:
        xor    ecx, ecx
        mov    dword ptr [edx], ecx
        or     byte ptr [edx + 10h], 40h
        mov    cl, 5
        jmp    SHORT loc_17_0594
loc_17_058A:
        push   ecx
        push   edx
        db  0CDh, 020h, 0FAh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_17_0594:
        test   byte ptr [edx + 10h], 20h
        jne    SHORT loc_17_058A
        and    byte ptr [edx + 10h], 0bfh
        ret
loc_17_059F:
        mov    ebx, dword ptr [ebp + 10h]
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     SHORT loc_17_05D0
        mov    edx, ebx
        db  003h, 015h, 06Ch, 00Ah, 000h, 000h
        xor    esi, esi
        xchg   dword ptr [edx], esi
        db  0CDh, 020h, 015h, 000h, 001h, 000h
        and    byte ptr [edx + 10h], 0dfh
        test   byte ptr [edx + 10h], 40h
        je     SHORT loc_17_05D0
        push   edx
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        add    esp, 4
loc_17_05D0:
        ret
loc_17_05D1:
        and    byte ptr [ebp + 2ch], 0feh
        mov    ebx, dword ptr [ebp + 10h]
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     SHORT loc_17_05F7
        db  0CDh, 020h, 01Ah, 000h, 017h, 000h
        jae    SHORT loc_17_05F7
        cmp    byte ptr [ebp + 1ch], 0
        je     SHORT loc_17_05F3
        db  0E8h, 00Dh, 0FAh, 0EEh, 0FFh
loc_17_05F3:
        or     byte ptr [ebp + 2ch], 1
loc_17_05F7:
        ret
sub_17_05F8:
        db  0A1h, 06Ch, 00Ah, 000h, 000h
        mov    esi, dword ptr [eax + ebx + 4]
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        ret
sub_17_0608:
        push   edx
        push   edi
        db  08Bh, 00Dh, 06Ch, 00Ah, 000h, 000h
        mov    esi, dword ptr [ecx + ebx + 4]
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     SHORT loc_17_065E
        xor    edx, edx
        cmp    dword ptr [edi], 54534e49h
        jne    SHORT loc_17_062F
        cmp    dword ptr [edi + 4], 4c4c41h
        je     SHORT loc_17_063C
loc_17_062F:
        cmp    dword ptr [edi], 55544553h
        jne    SHORT loc_17_063C
        cmp    word ptr [edi + 4], 50h
loc_17_063C:
        jne    SHORT loc_17_0645
        or     dl, 2
        dec    byte ptr [ecx + ebx + 18h]
loc_17_0645:
        inc    byte ptr [ecx + ebx + 18h]
        push   eax
        xchg   edi, eax
        xor    eax, eax
        cld
        lea    ecx, [eax + 15h]
        rep stosd
        pop    eax
        mov    byte ptr [eax + 3], dl
        db  0CDh, 020h, 09Eh, 000h, 001h, 000h
        clc
loc_17_065E:
        pop    edi
        pop    edx
loc_17_0660:
        ret
sub_17_0661:
        db  0A1h, 06Ch, 00Ah, 000h, 000h
        bts    dword ptr [eax + ebx + 10h], 0ah
        jb     SHORT loc_17_0660
        mov    ecx, 423h
        xchg   edx, eax
        xor    esi, esi
        db  0CDh, 020h, 002h, 080h, 017h, 000h
loc_17_067C:
        sub    esp, 14h
        mov    ax, 3800h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        movzx  edx, word ptr [ebp + 14h]
        movzx  ecx, word ptr [ebp + 18h]
        mov    dword ptr [esp + 10h], eax
        mov    dword ptr [esp + 0ch], ecx
        and    dword ptr [esp], 0
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_17_071E
        cmp    edx, 4
        ja     SHORT loc_17_071E
        cmp    eax, dword ptr [ebx + 4]
        je     SHORT loc_17_06B4
        cmp    eax, -1
        jne    SHORT loc_17_06BC
loc_17_06B4:
        db  0C7h, 044h, 024h, 010h, 0B2h, 00Bh, 000h, 000h
loc_17_06BC:
        test   dl, 1
        je     SHORT loc_17_06CE
        call   sub_17_000F
        add    eax, 2eh
        push   1eh
        pop    ecx
        jmp    SHORT loc_17_06D9
loc_17_06CE:
        call   sub_17_05F8
        add    eax, 4
        push   50h
        pop    ecx
loc_17_06D9:
        mov    esi, dword ptr [esp + 10h]
        xchg   edi, eax
        cmp    edx, 2
        jb     SHORT loc_17_0704
        db  0CDh, 020h, 002h, 000h, 001h, 000h
        je     SHORT loc_17_06F6
        db  08Bh, 00Dh, 06Ch, 00Ah, 000h, 000h
        and    byte ptr [ecx + ebx + 11h], 0fbh
loc_17_06F6:
        xchg   edi, esi
        mov    ecx, dword ptr [esp + 0ch]
        db  081h, 0FFh, 0B2h, 00Bh, 000h, 000h
        je     SHORT loc_17_071E
loc_17_0704:
        jecxz  loc_17_0711
loc_17_0706:
        lodsb
        stosb
        or     al, al
        loopne loc_17_0706
        je     SHORT loc_17_0711
        mov    byte ptr [edi - 1], cl
loc_17_0711:
        cmp    edx, 2
        jae    SHORT loc_17_071B
        call   sub_17_0661
loc_17_071B:
        inc    dword ptr [esp]
loc_17_071E:
        mov    eax, dword ptr [esp]
        add    esp, 14h
        mov    dword ptr [ebp + 1ch], eax
        clc
        ret
loc_17_0729:
        mov    ebx, dword ptr [ebp + 10h]
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     SHORT loc_17_073F
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [ebp + 14h], eax
        jmp    NEAR PTR loc_17_067C
loc_17_073F:
        ret
sub_17_0740:
        db  08Bh, 03Dh, 06Ch, 00Ah, 000h, 000h
        mov    ecx, dword ptr [edi + ebx + 10h]
        and    byte ptr [edi + ebx + 10h], 7fh
        call   sub_17_05F8
loc_17_0754:
        test   byte ptr [eax + 2], 1
        je     SHORT loc_17_0767
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jne    SHORT loc_17_0754
        or     byte ptr [edi + ebx + 10h], 80h
loc_17_0767:
        cmp    ecx, dword ptr [edi + ebx + 10h]
        je     SHORT loc_17_077C
        mov    ecx, 42ah
        xor    eax, eax
        xor    esi, esi
        db  0CDh, 020h, 002h, 000h, 017h, 000h
loc_17_077C:
        ret
sub_17_077D:
        push   edx
        xor    ecx, ecx
        db  0CDh, 020h, 042h, 001h, 001h, 000h
        xchg   ecx, eax
        pop    edx
        call   sub_17_05F8
        cmp    word ptr [eax], cx
        ret
loc_17_0791:
        movsx  edx, word ptr [ebp + 14h]
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_17_07B6
        db  08Bh, 035h, 06Ch, 00Ah, 000h, 000h
        cmp    dh, 1
        jne    SHORT loc_17_07B8
        mov    eax, dword ptr [esi + ebx + 10h]
        test   al, 2
        je     SHORT loc_17_07B6
        and    eax, 1
loc_17_07B3:
        mov    dword ptr [ebp + 1ch], eax
loc_17_07B6:
        clc
        ret
loc_17_07B8:
        cmp    dh, 2
        ja     SHORT loc_17_07D7
        je     SHORT loc_17_083B
        call   sub_17_077D
        jne    SHORT loc_17_07B6
        and    edx, 1
        mov    byte ptr [eax + 2], dl
        jne    SHORT loc_17_07CE
loc_17_07CE:
        call   sub_17_0740
loc_17_07D3:
        xor    eax, eax
        jmp    SHORT loc_17_07B3
loc_17_07D7:
        cmp    dh, 4
        ja     SHORT loc_17_07F4
        je     SHORT loc_17_07E5
        db  0E8h, 091h, 000h, 0EFh, 0FFh
        jmp    SHORT loc_17_07D3
loc_17_07E5:
        call   sub_17_077D
        jne    SHORT loc_17_07B6
        mov    ecx, dword ptr [ebp + 10h]
        mov    word ptr [eax], cx
        jmp    SHORT loc_17_07D3
loc_17_07F4:
        cmp    edx, -2
        jb     SHORT loc_17_07B6
        cmp    byte ptr [esi + ebx + 18h], 2
        jne    SHORT loc_17_0836
        and    byte ptr [esi + ebx + 19h], 0feh
        inc    edx
        jne    SHORT loc_17_0836
        or     byte ptr [esi + ebx + 19h], 1
        mov    ax, 3c04h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        mov    ax, 3c00h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        call   sub_17_0DF6
        test   eax, eax
        je     SHORT loc_17_0836
        and    byte ptr [esi + ebx + 19h], 0feh
        mov    word ptr [ebp + 14h], 0
loc_17_0836:
        jmp    NEAR PTR loc_17_07B6
loc_17_083B:
        or     byte ptr [esi + ebx + 10h], 1
        mov    ecx, 429h
        xor    eax, eax
        mov    al, 30h
        xor    esi, esi
        db  0CDh, 020h, 002h, 000h, 017h, 000h
        jmp    SHORT loc_17_07D3
loc_17_0853:
        mov    ebx, dword ptr [ebp + 10h]
        xor    eax, eax
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     SHORT loc_17_086E
        db  08Bh, 03Dh, 06Ch, 00Ah, 000h, 000h
        test   byte ptr [edi + ebx + 10h], 80h
        je     SHORT loc_17_086E
        inc    eax
loc_17_086E:
        mov    dword ptr [ebp + 1ch], eax
        ret
        db  000h, 000h
sub_17_0874:
        db  08Bh, 035h, 0BCh, 00Bh, 000h, 000h
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        jmp    SHORT loc_17_0888
loc_17_0882:
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
loc_17_0888:
        je     SHORT loc_17_088F
        cmp    eax, edx
        jne    SHORT loc_17_0882
        ret
loc_17_088F:
        stc
        ret
loc_17_0891:
        call   sub_17_0874
        jb     SHORT loc_17_08BA
        db  0CDh, 020h, 008h, 000h, 00Dh, 000h
        call   sub_17_002A
        jb     SHORT loc_17_08BA
        db  0BEh, 0BAh, 008h, 000h, 000h
        mov    ecx, 422h
        mov    eax, 100000h
        db  0CDh, 020h, 002h, 000h, 017h, 000h
loc_17_08BA:
        ret
loc_17_08BB:
        or     byte ptr [ebp + 2ch], 1
        mov    edx, dword ptr [ebp]
        mov    edx, dword ptr [edx + 4]
        call   sub_17_0874
        jb     SHORT loc_17_08DC
        and    byte ptr [ebp + 2ch], 0feh
        mov    eax, dword ptr [edx + 2]
        mov    edx, dword ptr [edx + 4]
        mov    dword ptr [ebp], eax
        mov    dword ptr [ebp + 1ch], edx
loc_17_08DC:
        ret
        db  000h, 000h, 000h
loc_17_08E0:
        push   ebx
        push   ecx
        db  0CDh, 020h, 008h, 000h, 00Dh, 000h
        call   sub_17_002A
        jb     SHORT loc_17_0928
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_17_0928
        xor    ecx, ecx
        inc    ecx
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        xor    esi, esi
        mov    eax, 100000h
        mov    ecx, 0ffffh
        db  0CDh, 020h, 002h, 000h, 017h, 000h
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        pop    ecx
        pop    eax
        db  0CDh, 020h, 005h, 000h, 00Dh, 000h
        db  0CDh, 020h, 020h, 000h, 001h, 000h
        jmp    SHORT loc_17_092A
loc_17_0928:
        pop    ecx
        pop    ebx
loc_17_092A:
        ret
loc_17_092B:
        db  0CDh, 020h, 006h, 000h, 00Dh, 000h
        db  0CDh, 020h, 008h, 000h, 00Dh, 000h
        call   sub_17_002A
        jb     SHORT loc_17_096A
        db  00Fh, 0BAh, 02Dh, 0A3h, 00Ah, 000h, 000h, 000h
        jb     SHORT loc_17_096A
        db  089h, 00Dh, 0C0h, 00Bh, 000h, 000h
        db  0BEh, 0A5h, 009h, 000h, 000h
        mov    eax, 100002h
        mov    ecx, 415h
        db  0CDh, 020h, 002h, 000h, 017h, 000h
        db  080h, 01Dh, 0A3h, 00Ah, 000h, 000h, 000h
loc_17_096A:
        ret
        db  0B8h, 0AAh
loc_17_096D:
        mov    edi, dword ptr [ebp + 4]
        jmp    SHORT loc_17_0978
        db  090h, 0D0h, 0ECh, 013h, 0C0h, 097h
loc_17_0978:
        db  08Bh, 00Dh, 0C0h, 00Bh, 000h, 000h
        db  0A1h, 094h, 00Ah, 000h, 000h
        db  0CDh, 020h, 005h, 000h, 00Dh, 000h
        db  0BEh, 06Bh, 009h, 000h, 000h
loc_17_098E:
        shr    edi, 1
        jb     SHORT loc_17_099B
        xor    ecx, ecx
        inc    ecx
        db  0CDh, 020h, 007h, 000h, 00Dh, 000h
loc_17_099B:
        inc    esi
        db  081h, 0FEh, 06Dh, 009h, 000h, 000h
        jb     SHORT loc_17_098E
        ret
loc_17_09A5:
        db  080h, 025h, 0A3h, 00Ah, 000h, 000h, 0FEh
        ret
loc_17_09AD:
        db  0CDh, 020h, 006h, 000h, 00Dh, 000h
        push   ebx
        push   ecx
        db  0CDh, 020h, 008h, 000h, 00Dh, 000h
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_17_0A12
        call   sub_17_000F
        test   byte ptr [eax], 4
        je     SHORT loc_17_09F5
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        pop    ecx
        pop    eax
        db  0CDh, 020h, 005h, 000h, 00Dh, 000h
        mov    eax, 100000h
        mov    ecx, 0ffffh
        xor    esi, esi
        db  0CDh, 020h, 008h, 000h, 00Dh, 000h
        db  0CDh, 020h, 002h, 000h, 017h, 000h
        jmp    SHORT loc_17_0A15
loc_17_09F5:
        lea    eax, [edx + 100000h]
        mov    ecx, 414h
        db  0BEh, 016h, 00Ah, 000h, 000h
        db  0CDh, 020h, 002h, 000h, 017h, 000h
        jae    SHORT loc_17_0A12
        db  0E8h, 0EEh, 0F5h, 0EEh, 0FFh
loc_17_0A12:
        add    esp, 8
loc_17_0A15:
        ret
loc_17_0A16:
        jae    SHORT loc_17_0A1D
        db  0E8h, 0E3h, 0F5h, 0EEh, 0FFh
loc_17_0A1D:
        ret
        db  000h, 000h
sub_17_0A20:
        enter  14h, 0
        mov    dword ptr [ebp - 4], ebx
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 10h], esi
        mov    dword ptr [ebp - 14h], edx
        db  083h, 03Dh, 00Ch, 00Ch, 000h, 000h, 000h
        je     NEAR PTR loc_17_0ADD
        db  08Bh, 035h, 074h, 00Ah, 000h, 000h
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     NEAR PTR loc_17_0ADD
        db  0CDh, 020h, 09Eh, 000h, 001h, 000h
        mov    esi, eax
        mov    eax, dword ptr [ebp - 14h]
        mov    ebx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [esi + 0ch], ebx
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     SHORT loc_17_0AC9
        call   sub_17_000F
        movzx  edx, word ptr [eax + 2ah]
        or     edx, edx
        je     SHORT loc_17_0AC9
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 30ffffh
        mov    edi, dword ptr [ebp - 10h]
        mov    dword ptr [esi + 8], ecx
        cmp    word ptr [ebp - 8], 41ah
        je     SHORT loc_17_0A9A
        test   ecx, 300000h
        je     SHORT loc_17_0AA0
loc_17_0A9A:
        mov    word ptr [esi + 0ah], 10h
loc_17_0AA0:
        mov    dword ptr [esi], edi
        movzx  ecx, cx
        mov    eax, esi
        or     edi, edi
        jne    SHORT loc_17_0AAD
        xor    eax, eax
loc_17_0AAD:
        push   esi
        db  068h, 0E3h, 00Ah, 000h, 000h
        push   eax
        push   ecx
        push   dword ptr [ebp - 8]
        push   edx
        db  0CDh, 020h, 006h, 000h, 017h, 000h
        add    esp, 18h
        xor    edx, edx
        xchg   edx, eax
        or     edx, edx
        jne    SHORT loc_17_0AE0
loc_17_0AC9:
        mov    eax, esi
        db  08Bh, 035h, 074h, 00Ah, 000h, 000h
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
loc_17_0ADD:
        or     eax, 0ffffffffh
loc_17_0AE0:
        leave
        ret
        db  0C3h
loc_17_0AE3:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        push   ebp
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    edi, dword ptr [ebp + 0ch]
        cmp    dword ptr [ebp + 8], 0
        je     SHORT loc_17_0B37
        test   byte ptr [edi + 0ah], 10h
        je     SHORT loc_17_0B30
        db  00Fh, 0BAh, 02Dh, 0C8h, 00Bh, 000h, 000h, 000h
        jb     SHORT loc_17_0B30
        mov    eax, 7d0h
        db  0BEh, 05Dh, 00Bh, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  087h, 035h, 0C4h, 00Bh, 000h, 000h
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        mov    eax, 10h
        db  0CDh, 020h, 01Eh, 000h, 001h, 000h
loc_17_0B30:
        cmp    dword ptr [edi], 0
        jne    SHORT loc_17_0B57
        jmp    SHORT loc_17_0B42
loc_17_0B37:
        cmp    dword ptr [edi], 0
        je     SHORT loc_17_0B42
        mov    edx, dword ptr [edi + 4]
        stc
        call   dword ptr [edi]
loc_17_0B42:
        db  08Bh, 035h, 074h, 00Ah, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
loc_17_0B57:
        pop    ebp
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
loc_17_0B5D:
        db  083h, 025h, 0C4h, 00Bh, 000h, 000h, 000h
sub_17_0B64:
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  00Fh, 0BAh, 035h, 0C8h, 00Bh, 000h, 000h, 000h
        jae    SHORT loc_17_0B8B
        xor    esi, esi
        db  087h, 035h, 0C4h, 00Bh, 000h, 000h
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        push   -10h
        pop    eax
        db  0CDh, 020h, 01Eh, 000h, 001h, 000h
loc_17_0B8B:
        ret
_SHELL_PostMessage:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        xor    ecx, ecx
        db  039h, 00Dh, 00Ch, 00Ch, 000h, 000h
        je     SHORT loc_17_0BE8
        db  08Bh, 035h, 080h, 00Ah, 000h, 000h
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     SHORT loc_17_0BE8
        mov    edx, eax
        push   esi
        lea    esi, [ebp + 8]
        mov    ecx, 6
        mov    edi, eax
        rep movsd
        pop    esi
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        mov    ecx, dword ptr [ebp + 0eh]
        and    ecx, 0ah
        xor    ecx, 7
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 039h, 000h, 001h, 000h
        db  0BEh, 040h, 007h, 000h, 000h
        mov    eax, 1000h
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        or     ecx, 0ffffffffh
loc_17_0BE8:
        mov    eax, ecx
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h
sub_17_0BF0:
        push   ebx
        db  0CDh, 020h, 029h, 000h, 001h, 000h
        or     eax, 0ffffffffh
        jecxz  loc_17_0C02
        db  0CDh, 020h, 002h, 000h, 001h, 000h
loc_17_0C02:
        pop    ebx
        ret
sub_17_0C04:
        call   sub_17_0BF0
        je     SHORT loc_17_0C21
        db  0A1h, 0D0h, 00Bh, 000h, 000h
        push   5
        pop    ecx
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        db  089h, 01Dh, 0CCh, 00Bh, 000h, 000h
        clc
        ret
loc_17_0C21:
        stc
        ret
sub_17_0C23:
        db  0A1h, 0D4h, 00Bh, 000h, 000h
        push   5
        pop    ecx
        db  0CDh, 020h, 027h, 080h, 001h, 000h
sub_17_0C31:
        db  083h, 025h, 0CCh, 00Bh, 000h, 000h, 000h
        db  0A1h, 0D0h, 00Bh, 000h, 000h
        jmp    SHORT loc_17_0C44
loc_17_0C3F:
        db  0A1h, 0D4h, 00Bh, 000h, 000h
loc_17_0C44:
        db  0CDh, 020h, 028h, 080h, 001h, 000h
        add    byte ptr [eax], al
_SHELL_GetDOSAppInfo:
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        db  08Bh, 035h, 06Ch, 00Ah, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_17_0CA3
        mov    edi, dword ptr [esi + ebx + 0ch]
        cmp    dword ptr [esp + 10h], 1
        ja     SHORT loc_17_0CA3
        jb     SHORT loc_17_0C86
        mov    ecx, dword ptr [esp + 18h]
        cmp    ecx, 20h
        jae    SHORT loc_17_0CA4
        bt     dword ptr [edi + 1c4h], ecx
        jb     SHORT loc_17_0CA5
        jmp    SHORT loc_17_0CA4
loc_17_0C86:
        lea    esi, [edi]
        call   sub_17_0F19
        push   dword ptr [esp + 14h]
        push   esi
        db  0CDh, 020h, 08Eh, 001h, 001h, 000h
        add    esp, 8
        test   eax, eax
        je     SHORT loc_17_0CA5
        xor    eax, eax
        jmp    SHORT loc_17_0CA4
loc_17_0CA3:
        inc    eax
loc_17_0CA4:
        inc    eax
loc_17_0CA5:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
sub_17_0DAD:
        mov    ah, 47h
sub_17_0DAF:
        push   eax
        mov    ah, 19h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        mov    dword ptr [esi], 5c3a41h
        add    byte ptr [esi], al
        pop    eax
        xor    dl, dl
        add    esi, 3
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        sub    esi, 3
        ret
sub_17_0DD4:
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        movzx  edi, word ptr [eax + 4]
        shl    edi, 4
        movzx  edx, word ptr [eax + 2]
        add    edi, edx
        inc    edi
        mov    ax, 3c14h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
sub_17_0DF6:
        sub    esp, 18h
        push   esi
        mov    dword ptr [esp + 18h], eax
        mov    esi, dword ptr [esi + ebx + 0ch]
        xor    ecx, ecx
        mov    dword ptr [esp + 4], esi
        mov    dword ptr [esi + 1c4h], ecx
        lea    ecx, [esi]
        mov    dword ptr [esp + 14h], ecx
        push   40h
        push   eax
        push   ecx
        db  0CDh, 020h, 07Bh, 001h, 001h, 000h
        add    esp, 0ch
        lea    eax, [esi + 40h]
        push   40h
        push   edi
        push   eax
        db  0CDh, 020h, 07Dh, 001h, 001h, 000h
        add    esp, 0ch
        lea    esi, [esi + 80h]
        call   sub_17_0DAD
        mov    esi, dword ptr [esp + 14h]
        call   sub_17_0F19
        mov    edi, esi
        call   sub_17_0F2A
        jae    NEAR PTR loc_17_0F12
        lea    eax, [esp + 10h]
        push   eax
        push   edi
        db  0FFh, 035h, 0A9h, 00Ch, 000h, 000h
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_17_0F12
        xor    esi, esi
loc_17_0E71:
        lea    ecx, [edi + 40h]
        sub    ecx, dword ptr [esp + 14h]
        lea    eax, [esp + 0ch]
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [esp + 4]
        lea    ecx, [ecx + 1c4h]
        lea    edx, [esp + 8]
        mov    dword ptr [edx], 4
        push   edx
        push   ecx
        push   0
        push   0
        push   eax
        push   edi
        push   esi
        push   dword ptr [esp + 2ch]
        db  0CDh, 020h, 050h, 001h, 001h, 000h
        add    esp, 20h
        test   eax, eax
        jne    SHORT loc_17_0EC1
        mov    ah, 43h
        mov    edx, dword ptr [esp + 14h]
        inc    esi
        pushad
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        popad
        jb     SHORT loc_17_0E71
        jmp    SHORT loc_17_0ECD
loc_17_0EC1:
        mov    ecx, dword ptr [esp + 4]
        xor    edx, edx
        mov    dword ptr [ecx + 1c4h], edx
loc_17_0ECD:
        push   dword ptr [esp + 10h]
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
        push   40h
        push   dword ptr [esp + 1ch]
        push   dword ptr [esp + 1ch]
        db  0CDh, 020h, 07Bh, 001h, 001h, 000h
        add    esp, 0ch
        mov    ecx, dword ptr [esp + 4]
        test   byte ptr [ecx + 1c4h], 1
        je     SHORT loc_17_0F12
        db  08Bh, 035h, 06Ch, 00Ah, 000h, 000h
        test   byte ptr [esi + ebx + 11h], 3
        jne    SHORT loc_17_0F12
        db  0E8h, 0C9h, 0FDh, 0EEh, 0FFh
        jae    SHORT loc_17_0F12
        mov    al, 1
        jmp    SHORT loc_17_0F14
loc_17_0F12:
        xor    eax, eax
loc_17_0F14:
        pop    esi
        add    esp, 18h
        ret
sub_17_0F19:
        push   esi
loc_17_0F1A:
        lodsb
        test   al, al
        je     SHORT loc_17_0F28
        cmp    al, 5ch
        jne    SHORT loc_17_0F1A
        mov    dword ptr [esp], esi
        jmp    SHORT loc_17_0F1A
loc_17_0F28:
        pop    esi
        ret
sub_17_0F2A:
        xor    eax, eax
        mov    ecx, eax
        mov    edx, eax
loc_17_0F30:
        lodsb
        test   al, al
        je     SHORT loc_17_0F54
        cmp    al, 41h
        jae    SHORT loc_17_0F3C
        add    ah, 0dh
loc_17_0F3C:
        and    al, 1fh
        add    dx, dx
        rol    cx, 5
        xor    dl, al
        xor    cl, al
        add    dl, ah
        add    ah, ch
        and    ah, 0f8h
        xor    cl, ah
        jmp    SHORT loc_17_0F30
loc_17_0F54:
        and    ch, 7
        and    dh, 7
        db  00Fh, 0A3h, 00Dh, 0ADh, 00Ch, 000h, 000h
        jae    SHORT loc_17_0F6A
        db  00Fh, 0A3h, 015h, 0ADh, 00Ch, 000h, 000h
loc_17_0F6A:
        ret
        db  000h
loc_17_0F6C:
        stc
        ret
loc_17_0F6E:
        and    byte ptr [ebp + 2ch], 0feh
        clc
        ret
loc_17_0F74:
        db  08Bh, 035h, 06Ch, 00Ah, 000h, 000h
        cmp    word ptr [esi + ebx + 18h], 2
        jb     SHORT loc_17_0F6C
        ja     SHORT loc_17_0F8D
        call   sub_17_0DD4
        test   eax, eax
        jne    SHORT loc_17_0F6E
loc_17_0F8D:
        test   byte ptr [esi + ebx + 10h], 8
        je     SHORT loc_17_0F6C
        or     byte ptr [ebp + 2ch], 1
        add    esp, -74h
        xor    eax, eax
        mov    dword ptr [esp], eax
        mov    dword ptr [esp + 10h], eax
        mov    ax, 3c14h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edx, eax
        mov    dword ptr [esp + 8], edx
        mov    ax, 3d40h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     NEAR PTR loc_17_104C
        mov    dword ptr [esp + 4], eax
        mov    ebx, eax
        mov    ecx, 40h
        lea    edx, [esp + 14h]
        mov    ah, 3fh
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_17_103E
        cmp    eax, 40h
        jne    SHORT loc_17_103E
        cmp    word ptr [edx], 5a4dh
        jne    SHORT loc_17_103E
        mov    edx, dword ptr [esp + 50h]
        test   edx, edx
        je     SHORT loc_17_103E
        shld   ecx, edx, 10h
        mov    ebx, dword ptr [esp + 4]
        mov    ax, 4200h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_17_103E
        mov    ebx, dword ptr [esp + 4]
        mov    ecx, 5eh
        lea    edx, [esp + 14h]
        mov    ah, 3fh
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_17_103E
        sub    eax, ecx
        jne    SHORT loc_17_103E
        cmp    dword ptr [edx], 4550h
        db  00Fh, 084h, 034h, 0F6h, 0EEh, 0FFh
        cmp    word ptr [edx], 454eh
        db  00Fh, 084h, 069h, 0F6h, 0EEh, 0FFh
loc_17_103E:
        mov    ebx, dword ptr [esp + 4]
        mov    ah, 3eh
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
loc_17_104C:
        add    esp, 74h
        bt     word ptr [ebp + 2ch], 0
        ret
_PTEXT ends
        end
