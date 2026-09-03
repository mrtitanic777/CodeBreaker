; shell_pcode_16.asm -- SHELL object 16 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_16_0000:
        cmp    dword ptr [esp + 4], 219h
        jne    SHORT loc_16_0024
        cmp    word ptr [esp + 8], 8042h
        db  00Fh, 084h, 039h, 002h, 0F0h, 0FFh
        cmp    word ptr [esp + 8], 8045h
        db  00Fh, 084h, 05Fh, 002h, 0F0h, 0FFh
loc_16_0024:
        xor    eax, eax
        inc    eax
        ret
sub_16_0028:
        dec    dword ptr [eax + 0ch]
        jne    SHORT loc_16_0039
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
loc_16_0039:
        ret
loc_16_003A:
        db  08Bh, 00Dh, 0B0h, 00Bh, 000h, 000h
        jecxz  loc_16_0057
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   eax
        push   505h
        push   ecx
        db  0CDh, 020h, 006h, 000h, 017h, 000h
        add    esp, 18h
loc_16_0057:
        ret
loc_16_0058:
        push   0
        push   dword ptr [eax + 4]
        mov    ecx, dword ptr [eax]
        mov    edx, dword ptr [eax + 8]
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        mov    esi, edx
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        call   ecx
        pop    eax
        pop    eax
loc_16_0074:
        db  08Bh, 035h, 0E0h, 00Bh, 000h, 000h
        cli
        db  0CDh, 020h, 0A5h, 000h, 001h, 000h
        sti
        jne    SHORT loc_16_0058
        db  080h, 025h, 0E8h, 00Bh, 000h, 000h, 0FEh
        ret
_SHELL_BroadcastSystemMessage:
        push   edi
        push   ebp
        mov    ebp, esp
        push   0
        cmp    dword ptr [ebp + 10h], 0
        jne    SHORT loc_16_009B
        mov    dword ptr [ebp + 10h], esp
loc_16_009B:
        mov    eax, dword ptr [ebp + 10h]
        db  083h, 03Dh, 0D8h, 00Bh, 000h, 000h, 000h
        je     SHORT loc_16_00F3
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        btr    dword ptr [ebp + 0ch], 1fh
        jb     SHORT loc_16_00C1
        cmp    dword ptr [eax], 1
        je     SHORT loc_16_00F3
        db  03Bh, 03Dh, 0D8h, 00Bh, 000h, 000h
        jne    SHORT loc_16_00E0
loc_16_00C1:
        lea    eax, [ebp + 0ch]
        or     byte ptr [eax], 20h
        push   eax
        mov    eax, esp
        push   eax
        push   4
        db  0FFh, 035h, 0F0h, 00Bh, 000h, 000h
        push   0
        db  0CDh, 020h, 018h, 000h, 017h, 000h
        add    esp, 10h
        jecxz  loc_16_00E5
loc_16_00E0:
        or     eax, 0ffffffffh
        jmp    SHORT loc_16_00F0
loc_16_00E5:
        mov    edi, dword ptr [edi + 10h]
        mov    edx, dword ptr [ebp + 10h]
        mov    edi, dword ptr [edi + 10h]
        mov    dword ptr [edx], edi
loc_16_00F0:
        leave
        pop    edi
        ret
loc_16_00F3:
        mov    dword ptr [eax], 1
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 1ch]
        call   sub_16_010C
        jmp    SHORT loc_16_00F0
sub_16_010C:
        push   esi
        push   edi
        db  08Bh, 035h, 0E4h, 00Bh, 000h, 000h
        test   esi, esi
        je     SHORT loc_16_0159
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
loc_16_011E:
        test   eax, eax
        je     SHORT loc_16_0159
        inc    dword ptr [eax + 0ch]
        push   esi
        push   eax
        push   dword ptr [eax + 4]
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 28h]
        call   dword ptr [eax]
        add    esp, 10h
        pop    ecx
        pop    esi
        xchg   ecx, eax
        mov    edi, eax
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        xchg   edi, eax
        call   sub_16_0028
        xchg   edi, eax
        test   byte ptr [esp + 18h], 1
        je     SHORT loc_16_011E
        cmp    ecx, 1
        jae    SHORT loc_16_011E
loc_16_0159:
        pop    edi
        pop    esi
        sbb    eax, eax
        inc    eax
        ret    10h
loc_16_0161:
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
        cmp    word ptr [esi], 219h
        jne    SHORT loc_16_0194
        test   byte ptr [esi + 3], 80h
        je     SHORT loc_16_0194
        xor    eax, eax
        xchg   word ptr [ebp + 1eh], ax
        push   0
        push   eax
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, -1
        je     SHORT loc_16_01AA
        add    dword ptr [ebp + 1ch], eax
loc_16_0194:
        push   dword ptr [esi + 8]
        xor    eax, eax
        mov    ax, word ptr [esi]
        push   eax
        mov    ax, word ptr [esi + 2]
        push   eax
        push   dword ptr [ebp + 1ch]
        call   sub_16_010C
loc_16_01AA:
        mov    dword ptr [ebp + 1ch], eax
        ret
        db  000h, 000h
sub_16_01B0:
        db  0CDh, 020h, 00Dh, 000h, 017h, 000h
        and    eax, 2
        ret
_SHELL_LocalAllocEx:
        push   esi
        push   edi
        and    dword ptr [esp + 0ch], 30040h
        call   sub_16_01B0
        je     SHORT loc_16_0213
        test   byte ptr [esp + 0eh], 1
        je     SHORT loc_16_01E6
        xor    eax, eax
        mov    edi, dword ptr [esp + 14h]
        or     ecx, 0ffffffffh
        cld
        repne scasb
        sub    edi, dword ptr [esp + 14h]
        mov    dword ptr [esp + 10h], edi
loc_16_01E6:
        cmp    dword ptr [esp + 10h], 10000h
        jae    SHORT loc_16_0213
        mov    eax, dword ptr [esp + 0ah]
        mov    ax, word ptr [esp + 10h]
        push   eax
        mov    eax, esp
        push   eax
        push   4
        db  0FFh, 035h, 0F4h, 00Bh, 000h, 000h
        push   0
        db  0CDh, 020h, 018h, 000h, 017h, 000h
        add    esp, 10h
        pop    edx
        jecxz  loc_16_0218
loc_16_0213:
        xor    eax, eax
        cdq
        jmp    SHORT loc_16_0267
loc_16_0218:
        or     ax, ax
        je     SHORT loc_16_0213
        xchg   edx, eax
        mov    ax, 3c1ch
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edx, eax
        mov    esi, dword ptr [esp + 14h]
        or     esi, esi
        je     SHORT loc_16_023A
        mov    edi, edx
        cld
        mov    ecx, dword ptr [esp + 10h]
        rep movsb
loc_16_023A:
        rol    eax, 10h
        db  066h, 0A1h, 0DCh, 00Bh, 000h, 000h
        rol    eax, 10h
        test   byte ptr [esp + 0eh], 2
        je     SHORT loc_16_0267
        pushad
        push   eax
        push   eax
        mov    eax, esp
        push   eax
        push   8
        db  0FFh, 035h, 01Ch, 00Ch, 000h, 000h
        push   0
        db  0CDh, 020h, 018h, 000h, 017h, 000h
        add    esp, 18h
        popad
loc_16_0267:
        pop    edi
        pop    esi
        ret
_SHELL_LocalFree:
        call   sub_16_01B0
        je     SHORT loc_16_0296
        db  08Bh, 015h, 0DCh, 00Bh, 000h, 000h
        cmp    word ptr [esp + 6], dx
        jne    SHORT loc_16_0296
        lea    eax, [esp + 4]
        push   eax
        push   2
        db  0FFh, 035h, 0F8h, 00Bh, 000h, 000h
        push   0
        db  0CDh, 020h, 018h, 000h, 017h, 000h
        add    esp, 10h
loc_16_0296:
        ret
_SHELL_LoadLibrary:
        call   sub_16_01B0
        je     SHORT loc_16_02DC
        push   dword ptr [esp + 4]
        push   0
        push   10000h
        db  0CDh, 020h, 013h, 000h, 017h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_16_02DC
        push   eax
        mov    ecx, esp
        push   eax
        push   ecx
        push   4
        db  0FFh, 035h, 0FCh, 00Bh, 000h, 000h
        push   0
        db  0CDh, 020h, 018h, 000h, 017h, 000h
        add    esp, 10h
        sub    eax, ecx
        mov    dword ptr [esp + 4], eax
        db  0CDh, 020h, 014h, 000h, 017h, 000h
        pop    edx
        pop    eax
loc_16_02DC:
        ret
_SHELL_FreeLibrary:
        call   sub_16_01B0
        je     SHORT loc_16_02FC
        lea    eax, [esp + 4]
        push   eax
        push   2
        db  0FFh, 035h, 000h, 00Ch, 000h, 000h
        push   0
        db  0CDh, 020h, 018h, 000h, 017h, 000h
        add    esp, 10h
loc_16_02FC:
        ret
_SHELL_GetProcAddress:
        push   edi
        mov    eax, dword ptr [esp + 0ch]
        xchg   dword ptr [esp + 8], eax
        mov    dword ptr [esp + 0ch], eax
        call   sub_16_01B0
        je     SHORT loc_16_0361
        xor    edi, edi
        cmp    word ptr [esp + 0ah], di
        je     SHORT loc_16_0337
        push   dword ptr [esp + 8]
        push   0
        push   10000h
        db  0CDh, 020h, 013h, 000h, 017h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_16_0361
        mov    dword ptr [esp + 8], eax
        inc    edi
loc_16_0337:
        lea    eax, [esp + 8]
        push   eax
        push   6
        db  0FFh, 035h, 004h, 00Ch, 000h, 000h
        push   0
        db  0CDh, 020h, 018h, 000h, 017h, 000h
        add    esp, 10h
        sub    eax, ecx
        dec    edi
        js     SHORT loc_16_0361
        push   eax
        push   dword ptr [esp + 0ch]
        db  0CDh, 020h, 014h, 000h, 017h, 000h
        pop    eax
        pop    eax
loc_16_0361:
        pop    edi
        ret
_SHELL_CallDll:
        sub    esp, 6ch
        push   ebx
        push   esi
        push   edi
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        xor    edi, edi
        mov    ecx, dword ptr [esp + 80h]
        db  03Bh, 00Dh, 0F0h, 00Bh, 000h, 000h
        jne    SHORT loc_16_038A
        lea    edi, [esp + 0ch]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
loc_16_038A:
        mov    ebx, dword ptr [ebx + 8]
        mov    eax, dword ptr [esp + 7ch]
        mov    ecx, dword ptr [esp + 80h]
        test   eax, eax
        je     SHORT loc_16_03C2
        push   eax
        db  0CDh, 020h, 015h, 000h, 017h, 000h
        add    esp, 4
        cmp    eax, 20h
        jb     SHORT loc_16_040E
        mov    dword ptr [esp + 7ch], eax
        push   dword ptr [esp + 80h]
        push   eax
        db  0CDh, 020h, 017h, 000h, 017h, 000h
        add    esp, 8
        mov    ecx, eax
loc_16_03C2:
        jecxz  loc_16_03F9
        mov    dword ptr [ebx + 1ch], ecx
        xor    eax, eax
        mov    dword ptr [ebx + 38h], eax
        mov    dword ptr [ebx + 40h], eax
        mov    dword ptr [ebx + 44h], eax
        db  00Fh, 0B7h, 00Dh, 0EEh, 00Bh, 000h, 000h
        db  00Fh, 0B7h, 015h, 0ECh, 00Bh, 000h, 000h
        push   dword ptr [esp + 88h]
        push   dword ptr [esp + 88h]
        push   edx
        push   ecx
        db  0CDh, 020h, 053h, 001h, 001h, 000h
        add    esp, 10h
loc_16_03F9:
        push   eax
        mov    ecx, dword ptr [esp + 80h]
        jecxz  loc_16_040D
        push   ecx
        db  0CDh, 020h, 016h, 000h, 017h, 000h
        add    esp, 4
loc_16_040D:
        pop    eax
loc_16_040E:
        or     edi, edi
        je     SHORT loc_16_041A
        mov    esi, edi
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
loc_16_041A:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 6ch
        ret
        db  000h, 000h, 000h
_SHELL_ShellExecute:
        push   esi
        xor    esi, esi
        db  0CDh, 020h, 0B5h, 000h, 001h, 000h
        pop    esi
        push   dword ptr [esp + 4]
        push   edx
        mov    eax, esp
        push   eax
        push   8
        db  0FFh, 035h, 010h, 00Ch, 000h, 000h
        push   0
        db  0CDh, 020h, 018h, 000h, 017h, 000h
        add    esp, 18h
        ret
sub_16_044A:
        push   0
        mov    eax, esp
        push   dword ptr [esp + 8]
        mov    edx, esp
        push   edx
        push   edi
        push   eax
        push   0
        push   dword ptr [esp + 20h]
        push   ebp
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 20h
        ret    8
loc_16_046A:
        and    byte ptr [edi], ch
        das
        and    byte ptr [eax], al
loc_16_046F:
        inc    ebx
        dec    edi
        dec    ebp
        dec    ebp
        inc    ecx
        dec    esi
        inc    esp
        pop    esp
        push   ebx
        push   esp
        inc    ecx
        push   edx
        push   esp
        inc    ebp
        pop    eax
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
sub_16_0585:
        pushad
        db  0BFh, 081h, 004h, 000h, 000h
        cmp    byte ptr [edi], 0
        ja     NEAR PTR loc_16_0665
        db  0CDh, 020h, 0B7h, 000h, 001h, 000h
        push   104h
        push   edx
        push   edi
        db  0CDh, 020h, 07Bh, 001h, 001h, 000h
        add    esp, 0ch
        push   ecx
        db  068h, 06Fh, 004h, 000h, 000h
        push   edx
        db  0CDh, 020h, 07Bh, 001h, 001h, 000h
        add    esp, 0ch
        mov    edx, edi
        mov    ax, 4300h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jae    NEAR PTR loc_16_065F
        push   0
        mov    eax, esp
        push   eax
        db  068h, 0D0h, 00Fh, 000h, 000h
        push   80000002h
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        pop    ebp
        test   eax, eax
        jne    SHORT loc_16_0649
        db  068h, 010h, 010h, 000h, 000h
        push   4
        call   sub_16_044A
        test   eax, eax
        jne    SHORT loc_16_060E
        cmp    dword ptr [edi], eax
        je     SHORT loc_16_060E
        db  068h, 017h, 010h, 000h, 000h
        push   0f2h
        call   sub_16_044A
loc_16_060E:
        push   eax
        push   ebp
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
        pop    eax
        test   eax, eax
        jne    SHORT loc_16_0649
        push   edi
        db  0CDh, 020h, 07Ch, 001h, 001h, 000h
        add    esp, 4
        push   -1
        db  068h, 06Fh, 004h, 000h, 000h
        push   edx
        db  0CDh, 020h, 07Bh, 001h, 001h, 000h
        add    esp, 0ch
        mov    edx, edi
        mov    ax, 4300h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jae    SHORT loc_16_065F
loc_16_0649:
        lea    edx, [edi - 0ah]
        db  0CDh, 020h, 0B8h, 000h, 001h, 000h
        jb     SHORT loc_16_0662
        xchg   ebx, eax
        mov    ah, 3eh
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
loc_16_065F:
        clc
        jmp    SHORT loc_16_0665
loc_16_0662:
        mov    byte ptr [edi], 0
loc_16_0665:
        popad
        ret
loc_16_0667:
        cmp    word ptr [edx + 40h], 1
        jae    SHORT loc_16_0676
        cmp    word ptr [edx + 1ah], 1202h
        jne    SHORT loc_16_06A2
loc_16_0676:
        mov    cx, word ptr [edx + 4]
        jcxz   loc_16_0688
        sub    cx, 14ch
        cmp    cx, 2
        ja     SHORT loc_16_06A2
loc_16_0688:
        cmp    word ptr [edx + 5ch], 2
        je     NEAR PTR loc_16_0731
        cmp    word ptr [edx + 5ch], 3
        jne    SHORT loc_16_06A2
        inc    dword ptr [esp]
        jmp    NEAR PTR loc_16_0731
loc_16_06A2:
        db  0E9h, 097h, 009h, 0F0h, 0FFh
loc_16_06A7:
        cmp    byte ptr [edx + 36h], 2
        jne    SHORT loc_16_06A2
        cmp    dword ptr [edx + 14h], 0
        je     SHORT loc_16_06A2
        cmp    byte ptr [edx + 2], 5
        jb     SHORT loc_16_06A2
        sub    word ptr [edx + 3eh], 401h
        cmp    word ptr [edx + 3eh], 0feffh
loc_16_06C5:
        jb     SHORT loc_16_06A2
        test   byte ptr [edx + 37h], 2
        jne    SHORT loc_16_06A2
        movzx  esi, word ptr [edx + 4]
        sub    si, word ptr [edx + 2ah]
loc_16_06D5:
        jbe    SHORT loc_16_06A2
        cmp    si, 60h
        jbe    SHORT loc_16_06E2
        mov    esi, 60h
loc_16_06E2:
        mov    eax, dword ptr [edx + 2ah]
        sub    eax, 5eh
        cwd
        mov    ecx, edx
        xchg   edx, eax
        mov    ebx, dword ptr [esp + 4]
        mov    ax, 4201h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
loc_16_06FD:
        jb     SHORT loc_16_06C5
        mov    ecx, esi
        lea    edx, [esp + 14h]
        mov    ah, 3fh
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_16_06FD
loc_16_0711:
        movzx  eax, byte ptr [edx]
        inc    edx
        cmp    al, 8
        jne    SHORT loc_16_072A
        cmp    dword ptr [edx], 43534f44h
        jne    SHORT loc_16_072A
        cmp    dword ptr [edx + 4], 534c4c41h
        je     SHORT loc_16_06D5
loc_16_072A:
        add    edx, eax
        stc
        sbb    ecx, eax
        ja     SHORT loc_16_0711
loc_16_0731:
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        mov    dword ptr [esp + 0ch], eax
        movzx  edi, word ptr [eax]
        or     edi, edi
        jne    SHORT loc_16_075E
        xor    ecx, ecx
        db  0CDh, 020h, 042h, 001h, 001h, 000h
        movzx  eax, ax
        shl    eax, 4
        movzx  edi, word ptr [eax + 2ch]
loc_16_075E:
        shl    edi, 4
        mov    eax, dword ptr [esp + 0ch]
        movzx  esi, word ptr [eax + 4]
        shl    esi, 4
        movzx  edx, word ptr [eax + 2]
        add    esi, edx
        call   sub_16_0878
        movzx  eax, byte ptr [esi - 1]
        jb     SHORT loc_16_078B
        or     byte ptr [esp], 2
        push   esi
        db  0CDh, 020h, 07Ch, 001h, 001h, 000h
        add    esp, 4
loc_16_078B:
        xchg   edi, eax
        push   dword ptr [esp + 8]
        db  0CDh, 020h, 07Ch, 001h, 001h, 000h
        add    esp, 4
        add    edi, eax
        inc    edi
        inc    edi
        test   byte ptr [esp], 1
        jne    SHORT loc_16_07C0
        call   sub_16_0585
        jb     NEAR PTR loc_16_06A2
        db  068h, 081h, 004h, 000h, 000h
        db  0CDh, 020h, 07Ch, 001h, 001h, 000h
        add    esp, 4
        lea    edi, [eax + edi + 4]
loc_16_07C0:
        add    edi, 108h
        push   200h
        push   edi
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_16_06A2
        mov    dword ptr [esp + 10h], eax
        xchg   edx, eax
        mov    ecx, edi
        test   byte ptr [esp], 1
        jne    SHORT loc_16_080A
        push   ecx
        db  068h, 081h, 004h, 000h, 000h
        push   edx
        db  0CDh, 020h, 07Bh, 001h, 001h, 000h
        add    esp, 0ch
        push   ecx
        db  068h, 06Ah, 004h, 000h, 000h
        push   edx
        db  0CDh, 020h, 07Bh, 001h, 001h, 000h
        add    esp, 0ch
loc_16_080A:
        push   ecx
        push   dword ptr [esp + 0ch]
        push   edx
        db  0CDh, 020h, 07Bh, 001h, 001h, 000h
        add    esp, 0ch
        cmp    byte ptr [esi], 20h
        je     SHORT loc_16_0822
        mov    byte ptr [edx], 20h
        inc    edx
loc_16_0822:
        test   byte ptr [esp], 2
        jne    SHORT loc_16_0836
        movzx  ecx, byte ptr [esi - 1]
        mov    edi, edx
        rep movsb
        xchg   ecx, eax
        stosb
        mov    edx, edi
        jmp    SHORT loc_16_0843
loc_16_0836:
        push   ecx
        push   esi
        push   edx
        db  0CDh, 020h, 07Bh, 001h, 001h, 000h
        add    esp, 0ch
        inc    edx
loc_16_0843:
        mov    esi, edx
        mov    ax, 7147h
        db  0E8h, 061h, 005h, 0F0h, 0FFh
        mov    edi, dword ptr [esp + 10h]
        db  0CDh, 020h, 001h, 000h, 038h, 000h
        and    byte ptr [ebp + 2ch], 0feh
        push   0
        push   dword ptr [esp + 14h]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        db  0E9h, 0CEh, 007h, 0F0h, 0FFh
loc_16_0870:
        inc    ebx
        dec    ebp
        inc    esp
        dec    esp
        dec    ecx
        dec    esi
        inc    ebp
        db  03Dh
sub_16_0878:
        lodsb
        push   esi
        cmp    al, 7fh
        jne    SHORT loc_16_0897
        xor    eax, eax
        jmp    SHORT loc_16_0893
loc_16_0882:
        db  0BEh, 070h, 008h, 000h, 000h
        lea    ecx, [eax + 8]
        repe cmpsb
        je     SHORT loc_16_089A
        or     ecx, 0ffffffffh
        repne scasb
loc_16_0893:
        cmp    byte ptr [edi], al
        jne    SHORT loc_16_0882
loc_16_0897:
        pop    esi
        stc
        ret
loc_16_089A:
        mov    esi, dword ptr [esp]
        mov    cl, byte ptr [esi]
        mov    esi, edi
        mov    edx, edi
loc_16_08A3:
        lodsb
        test   al, al
        je     SHORT loc_16_0897
        cmp    al, 22h
        jne    SHORT loc_16_08B1
        xor    ah, 1
        jmp    SHORT loc_16_08A3
loc_16_08B1:
        cmp    al, cl
        jne    SHORT loc_16_08A3
        test   ah, ah
        jne    SHORT loc_16_08A3
        dec    esi
        pop    eax
        ret
_PTEXT ends
        end
