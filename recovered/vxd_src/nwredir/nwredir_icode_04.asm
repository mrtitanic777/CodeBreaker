; nwredir_icode_04.asm -- NWREDIR object 4 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

NWREDIR_SYS_CRITICAL_INIT:
        db  089h, 015h, 008h, 004h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 087h, 004h
        jb     NEAR PTR loc_4_04AE
        db  0CDh, 020h, 000h, 000h, 040h, 000h
        jb     NEAR PTR loc_4_04B0
        db  0CDh, 020h, 000h, 000h, 08Bh, 004h
        jb     NEAR PTR loc_4_04C2
        cmp    eax, 100h
        jne    NEAR PTR loc_4_04C2
        mov    eax, 400024h
        db  0BEh, 056h, 006h, 000h, 000h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        jb     NEAR PTR loc_4_04BA
        db  089h, 035h, 0DCh, 00Dh, 000h, 000h
        clc
        ret
NWREDIR_DEVICE_INIT:
        push   30000h
        push   22h
        db  068h, 060h, 03Ch, 000h, 000h
        db  0CDh, 020h, 002h, 000h, 040h, 000h
        add    esp, 0ch
        cmp    eax, -1
        je     NEAR PTR loc_4_04B2
        db  0A3h, 0A0h, 005h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 08Bh, 004h
        jb     NEAR PTR loc_4_04C2
        cmp    eax, 100h
        jne    NEAR PTR loc_4_04C2
        db  0BEh, 0A0h, 001h, 000h, 000h
        mov    ecx, 0
        db  0CDh, 020h, 001h, 000h, 08Bh, 004h
        jb     NEAR PTR loc_4_04C2
        db  0A2h, 016h, 00Eh, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 096h, 004h
        jae    SHORT loc_4_00B6
        db  0CDh, 020h, 000h, 000h, 097h, 004h
loc_4_00B6:
        sbb    eax, eax
        not    eax
        db  0A2h, 097h, 003h, 000h, 000h
        mov    eax, 1
        mov    ebx, 3
        db  0BEh, 0A0h, 0CBh, 000h, 000h
        db  0CDh, 020h, 006h, 000h, 087h, 004h
        db  0C6h, 005h, 0F4h, 00Dh, 000h, 000h, 001h
        db  0C7h, 005h, 09Ch, 003h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 052h, 004h
        jb     SHORT loc_4_00F7
        db  0C7h, 005h, 09Ch, 003h, 000h, 000h, 071h, 00Bh, 000h, 000h
loc_4_00F7:
        db  0E8h, 01Eh, 000h, 0FCh, 0FFh
        mov    eax, 14h
        db  0BFh, 084h, 00Fh, 000h, 000h
        db  0BEh, 07Ch, 00Fh, 000h, 000h
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        db  066h, 0A3h, 014h, 004h, 000h, 000h
        mov    edx, 0
        db  0BFh, 093h, 00Fh, 000h, 000h
        db  0BEh, 07Ch, 00Fh, 000h, 000h
        db  0CDh, 020h, 0B3h, 000h, 001h, 000h
        db  0C6h, 005h, 016h, 004h, 000h, 000h, 000h
        and    edx, edx
        je     SHORT loc_4_014C
        push   2fh
        push   edx
        db  068h, 016h, 004h, 000h, 000h
        db  0E8h, 046h, 006h, 0FCh, 0FFh
        db  0E8h, 07Dh, 006h, 0FCh, 0FFh
        add    esp, 0ch
loc_4_014C:
        db  0CDh, 020h, 06Ah, 000h, 001h, 000h
        push   0
        push   eax
        db  0CDh, 020h, 058h, 000h, 001h, 000h
        add    esp, 8
        db  089h, 015h, 068h, 01Ah, 000h, 000h
        mov    eax, 1
        db  0BEh, 07Ch, 00Fh, 000h, 000h
        db  0BFh, 0A3h, 00Fh, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        db  0A2h, 094h, 003h, 000h, 000h
        mov    eax, 14h
        db  0BFh, 0AEh, 00Fh, 000h, 000h
        db  0BEh, 07Ch, 00Fh, 000h, 000h
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        db  0A2h, 05Eh, 004h, 000h, 000h
        mov    eax, 2
        db  0BFh, 0B7h, 00Fh, 000h, 000h
        db  0BEh, 07Ch, 00Fh, 000h, 000h
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        db  0A2h, 05Fh, 004h, 000h, 000h
        mov    eax, 0ffffh
        db  0BFh, 0C0h, 00Fh, 000h, 000h
        db  0BEh, 07Ch, 00Fh, 000h, 000h
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        db  066h, 0A3h, 060h, 004h, 000h, 000h
        mov    eax, 1
        db  0BFh, 0C7h, 00Fh, 000h, 000h
        db  0BEh, 07Ch, 00Fh, 000h, 000h
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        db  0A2h, 064h, 004h, 000h, 000h
        mov    eax, 1
        db  0BFh, 0D2h, 00Fh, 000h, 000h
        db  0BEh, 07Ch, 00Fh, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        db  066h, 0A3h, 065h, 004h, 000h, 000h
        mov    eax, 1
        db  0BFh, 0DFh, 00Fh, 000h, 000h
        db  0BEh, 07Ch, 00Fh, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        db  0A2h, 067h, 004h, 000h, 000h
        mov    eax, 1
        db  0BFh, 0EBh, 00Fh, 000h, 000h
        db  0BEh, 07Ch, 00Fh, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        db  0A2h, 068h, 004h, 000h, 000h
        mov    eax, 1
        db  0BFh, 0FDh, 00Fh, 000h, 000h
        db  0BEh, 07Ch, 00Fh, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        db  0A2h, 069h, 004h, 000h, 000h
        mov    eax, 0
        db  0BFh, 00Ah, 010h, 000h, 000h
        db  0BEh, 07Ch, 00Fh, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        db  0A2h, 06Ah, 004h, 000h, 000h
        mov    eax, 1
        db  0BFh, 014h, 010h, 000h, 000h
        db  0BEh, 07Ch, 00Fh, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        db  0A2h, 06Bh, 004h, 000h, 000h
        mov    eax, 1
        db  0BFh, 020h, 010h, 000h, 000h
        db  0BEh, 07Ch, 00Fh, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        db  0A2h, 06Ch, 004h, 000h, 000h
        mov    eax, 0
        db  0BFh, 029h, 010h, 000h, 000h
        db  0BEh, 07Ch, 00Fh, 000h, 000h
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        db  0A3h, 0A4h, 005h, 000h, 000h
        mov    eax, 6
        db  0BFh, 031h, 010h, 000h, 000h
        db  0BEh, 07Ch, 00Fh, 000h, 000h
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        db  0A2h, 06Dh, 004h, 000h, 000h
        mov    eax, 2
        db  0BFh, 043h, 010h, 000h, 000h
        db  0BEh, 07Ch, 00Fh, 000h, 000h
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        db  0A2h, 06Eh, 004h, 000h, 000h
        db  080h, 03Dh, 094h, 003h, 000h, 000h, 000h
        je     NEAR PTR loc_4_03A0
        push   204h
        push   3d4h
        db  0CDh, 020h, 0A8h, 000h, 001h, 000h
        add    esp, 8
        and    eax, eax
        je     NEAR PTR loc_4_04B4
        db  0A3h, 0A4h, 003h, 000h, 000h
        add    eax, 100h
        db  0A3h, 0A8h, 003h, 000h, 000h
        add    eax, 180h
        db  0A3h, 0C8h, 003h, 000h, 000h
        add    eax, 20h
        db  0A3h, 0CCh, 003h, 000h, 000h
        add    eax, 21h
        db  0A3h, 0EFh, 003h, 000h, 000h
        shl    eax, 0ch
        shr    ax, 0ch
        db  0A3h, 0F3h, 003h, 000h, 000h
        push   304h
        push   41h
        db  0CDh, 020h, 0A8h, 000h, 001h, 000h
        add    esp, 8
        je     NEAR PTR loc_4_04B4
        db  0A3h, 0D0h, 003h, 000h, 000h
        add    eax, 20h
        db  0A3h, 0D4h, 003h, 000h, 000h
        add    eax, 20h
        db  0A3h, 0E8h, 003h, 000h, 000h
        push   0
        db  0FFh, 035h, 06Ch, 010h, 000h, 000h
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        add    esp, 8
        and    eax, eax
        je     NEAR PTR loc_4_04B6
        db  0A3h, 004h, 004h, 000h, 000h
        call   sub_4_04C4
        db  080h, 03Dh, 0C6h, 003h, 000h, 000h, 000h
        je     SHORT loc_4_03A0
loc_4_03A0:
        xor    eax, eax
        db  0BFh, 0D8h, 005h, 000h, 000h
        mov    ecx, 0ffh
        rep stosd
        db  0CDh, 020h, 0B9h, 000h, 001h, 000h
        db  066h, 0A3h, 0E8h, 00Dh, 000h, 000h
        db  0C7h, 005h, 0C4h, 005h, 000h, 000h, 0C4h, 005h, 000h, 000h
        db  0C7h, 005h, 0C8h, 005h, 000h, 000h, 0C4h, 005h, 000h, 000h
        db  0C7h, 005h, 0D0h, 00Dh, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0D8h, 00Dh, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 0F5h, 00Dh, 000h, 000h, 000h
        db  068h, 03Ch, 006h, 000h, 000h
        push   21005fh
        db  0CDh, 020h, 028h, 000h, 040h, 000h
        add    esp, 8
        and    eax, eax
        je     NEAR PTR loc_4_04BC
        db  0A3h, 0E0h, 00Dh, 000h, 000h
        db  0BEh, 01Ah, 00Eh, 000h, 000h
        mov    edi, 44h
        and    edi, edi
        jmp    SHORT loc_4_0437
loc_4_0417:
        mov    eax, 210000h
        mov    al, byte ptr [esi]
        db  068h, 047h, 006h, 000h, 000h
        push   eax
        db  0CDh, 020h, 028h, 000h, 040h, 000h
        add    esp, 8
        and    eax, eax
        je     NEAR PTR loc_4_04BC
        inc    esi
        dec    edi
loc_4_0437:
        jne    SHORT loc_4_0417
        db  080h, 03Dh, 094h, 003h, 000h, 000h, 000h
        je     SHORT loc_4_04A3
        db  068h, 0D3h, 004h, 000h, 000h
        db  0CDh, 020h, 029h, 000h, 040h, 000h
        add    esp, 4
        and    eax, eax
        je     SHORT loc_4_04BE
        db  0A3h, 0E4h, 00Dh, 000h, 000h
        mov    eax, 2fh
        db  0CDh, 020h, 044h, 000h, 001h, 000h
        db  066h, 089h, 00Dh, 0EAh, 00Dh, 000h, 000h
        db  089h, 015h, 0ECh, 00Dh, 000h, 000h
        db  0BEh, 02Ah, 007h, 000h, 000h
        db  0CDh, 020h, 00Ch, 000h, 001h, 000h
        jb     SHORT loc_4_04C0
        movzx  edx, ax
        mov    ecx, eax
        shr    ecx, 10h
        mov    eax, 2fh
        db  0CDh, 020h, 045h, 000h, 001h, 000h
        db  0BEh, 062h, 007h, 000h, 000h
        db  0CDh, 020h, 00Ch, 000h, 001h, 000h
        jb     SHORT loc_4_04C0
        db  0A3h, 0F0h, 00Dh, 000h, 000h
loc_4_04A3:
        call   sub_4_0900
        and    al, al
        jne    SHORT loc_4_04C2
        clc
        ret
loc_4_04AE:
        jmp    SHORT loc_4_04C2
loc_4_04B0:
        jmp    SHORT loc_4_04C2
loc_4_04B2:
        jmp    SHORT loc_4_04C2
loc_4_04B4:
        jmp    SHORT loc_4_04C2
loc_4_04B6:
        jmp    SHORT loc_4_04C2
        db  0EBh, 008h
loc_4_04BA:
        jmp    SHORT loc_4_04C2
loc_4_04BC:
        jmp    SHORT loc_4_04C2
loc_4_04BE:
        jmp    SHORT loc_4_04C2
loc_4_04C0:
        jmp    SHORT loc_4_04C2
loc_4_04C2:
        stc
        ret
sub_4_04C4:
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    byte ptr [ebp + 1dh], 58h
        mov    byte ptr [ebp + 1ch], 0d4h
        mov    word ptr [ebp + 18h], 0
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        sub    ebx, ebx
        movzx  eax, word ptr [ebp + 1ch]
        movzx  edx, word ptr [ebp + 14h]
        movzx  ecx, word ptr [ebp + 18h]
        cmp    cx, 3fcfh
        jne    SHORT loc_4_057D
        or     eax, eax
        je     SHORT loc_4_057D
        cmp    edx, 2
        jne    SHORT loc_4_057D
        movzx  eax, word ptr [ebp + 10h]
        test   eax, 1
        je     SHORT loc_4_057D
        movzx  eax, byte ptr [ebp + 1dh]
        cmp    eax, 1
        jne    SHORT loc_4_0572
        mov    byte ptr [ebp + 1dh], 58h
        mov    byte ptr [ebp + 1ch], 0d5h
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        movzx  ecx, word ptr [ebp + 38h]
        mov    edx, ecx
        shl    ecx, 4
        movzx  ebx, word ptr [ebp + 10h]
        add    ebx, ecx
        db  089h, 01Dh, 0BCh, 003h, 000h, 000h
        mov    byte ptr [ebp + 1dh], 58h
        mov    byte ptr [ebp + 1ch], 0d3h
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        movzx  ecx, word ptr [ebp + 38h]
        shl    ecx, 4
        movzx  ebx, word ptr [ebp + 10h]
        add    ebx, ecx
        db  089h, 01Dh, 0C0h, 003h, 000h, 000h
loc_4_0572:
        db  0B8h, 0D5h, 000h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 082h, 004h
loc_4_057D:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        or     ebx, ebx
        je     SHORT loc_4_05F0
        db  0C6h, 005h, 0C6h, 003h, 000h, 000h, 001h
        shl    edx, 4
        db  08Bh, 01Dh, 0BCh, 003h, 000h, 000h
        movzx  eax, word ptr [ebx + 8]
        add    eax, edx
        db  0A3h, 0A4h, 003h, 000h, 000h
        movzx  eax, word ptr [ebx + 0ah]
        add    eax, edx
        db  0A3h, 0A8h, 003h, 000h, 000h
        movzx  eax, word ptr [ebx + 4]
        add    eax, edx
        db  0A3h, 0C8h, 003h, 000h, 000h
        movzx  eax, word ptr [ebx + 6]
        add    eax, edx
        db  0A3h, 0CCh, 003h, 000h, 000h
        movzx  eax, word ptr [ebx + 2]
        add    eax, edx
        db  0A3h, 0D0h, 003h, 000h, 000h
        movzx  eax, word ptr [ebx + 0ch]
        db  066h, 0A3h, 0C4h, 003h, 000h, 000h
        movzx  eax, word ptr [ebx + 10h]
        db  0A2h, 0ECh, 003h, 000h, 000h
loc_4_05F0:
        ret
NWREDIR_INIT_COMPLETE:
        call   sub_4_1DF0
        and    al, al
        jne    NEAR PTR loc_4_08E6
        push   ebx
        push   ecx
        push   edx
        db  00Fh, 0B7h, 01Dh, 00Ch, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 00Eh, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 048h, 000h
        or     eax, eax
        je     SHORT loc_4_063A
        push   0
        push   ecx
        push   ebx
        push   0
        push   0
        push   esp
        db  0CDh, 020h, 001h, 000h, 048h, 000h
        add    esp, 18h
loc_4_063A:
        or     eax, eax
        je     NEAR PTR loc_4_08CF
        db  0A3h, 010h, 01Eh, 000h, 000h
        db  00Fh, 0B7h, 01Dh, 010h, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 012h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 015h, 014h, 000h, 000h, 000h
        db  003h, 015h, 004h, 000h, 000h, 000h
        db  068h, 050h, 010h, 000h, 000h
        push   edx
        push   0
        push   ecx
        push   ebx
        push   2
        push   0
        push   esp
        db  0FFh, 035h, 010h, 01Eh, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 048h, 000h
        add    esp, 24h
        db  00Fh, 0B7h, 01Dh, 016h, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 018h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 015h, 01Ah, 000h, 000h, 000h
        db  003h, 015h, 004h, 000h, 000h, 000h
        db  068h, 054h, 010h, 000h, 000h
        push   edx
        push   0
        push   ecx
        push   ebx
        push   2
        push   0
        push   esp
        db  0FFh, 035h, 010h, 01Eh, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 048h, 000h
        add    esp, 24h
        db  00Fh, 0B7h, 01Dh, 01Ch, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 01Eh, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 015h, 020h, 000h, 000h, 000h
        db  003h, 015h, 004h, 000h, 000h, 000h
        db  068h, 097h, 049h, 000h, 000h
        push   edx
        push   0
        push   ecx
        push   ebx
        push   1
        push   0
        push   esp
        db  0FFh, 035h, 010h, 01Eh, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 048h, 000h
        add    esp, 24h
        db  00Fh, 0B7h, 01Dh, 022h, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 024h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 015h, 026h, 000h, 000h, 000h
        db  003h, 015h, 004h, 000h, 000h, 000h
        db  068h, 09Eh, 049h, 000h, 000h
        push   edx
        push   0
        push   ecx
        push   ebx
        push   1
        push   0
        push   esp
        db  0FFh, 035h, 010h, 01Eh, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 048h, 000h
        add    esp, 24h
        db  00Fh, 0B7h, 01Dh, 028h, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 02Ah, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 015h, 02Ch, 000h, 000h, 000h
        db  003h, 015h, 004h, 000h, 000h, 000h
        db  068h, 058h, 010h, 000h, 000h
        push   edx
        push   0
        push   ecx
        push   ebx
        push   0
        push   0
        push   esp
        db  0FFh, 035h, 010h, 01Eh, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 048h, 000h
        add    esp, 24h
        db  00Fh, 0B7h, 01Dh, 02Eh, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 030h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 015h, 032h, 000h, 000h, 000h
        db  003h, 015h, 004h, 000h, 000h, 000h
        db  068h, 05Ch, 010h, 000h, 000h
        push   edx
        push   0
        push   ecx
        push   ebx
        push   0
        push   0
        push   esp
        db  0FFh, 035h, 010h, 01Eh, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 048h, 000h
        add    esp, 24h
        db  00Fh, 0B7h, 01Dh, 034h, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 036h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 015h, 038h, 000h, 000h, 000h
        db  003h, 015h, 004h, 000h, 000h, 000h
        db  068h, 060h, 010h, 000h, 000h
        push   edx
        push   0
        push   ecx
        push   ebx
        push   0
        push   0
        push   esp
        db  0FFh, 035h, 010h, 01Eh, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 048h, 000h
        add    esp, 24h
        db  00Fh, 0B7h, 01Dh, 03Ah, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 03Ch, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 015h, 03Eh, 000h, 000h, 000h
        db  003h, 015h, 004h, 000h, 000h, 000h
        db  068h, 064h, 010h, 000h, 000h
        push   edx
        push   0
        push   ecx
        push   ebx
        push   0
        push   0
        push   esp
        db  0FFh, 035h, 010h, 01Eh, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 048h, 000h
        add    esp, 24h
        db  00Fh, 0B7h, 01Dh, 040h, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 042h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 015h, 044h, 000h, 000h, 000h
        db  003h, 015h, 004h, 000h, 000h, 000h
        db  068h, 068h, 010h, 000h, 000h
        push   edx
        push   0
        push   ecx
        push   ebx
        push   0
        push   0
        push   esp
        db  0FFh, 035h, 010h, 01Eh, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 048h, 000h
        add    esp, 24h
loc_4_08CF:
        push   esi
        mov    eax, 2fh
        db  0BEh, 0A2h, 000h, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        clc
        ret
loc_4_08E6:
        stc
        ret
sub_4_08E8:
        sub    esp, 40h
        mov    eax, esp
        push   0
        push   eax
        db  0CDh, 020h, 06Fh, 000h, 001h, 000h
        mov    eax, dword ptr [esp + 0ch]
        add    esp, 48h
        ret
        db  000h, 000h
sub_4_0900:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ech
        push   ebx
        push   esi
        push   edi
        db  0C7h, 005h, 0CEh, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        call   sub_4_08E8
        lea    eax, [eax*2 - 400h]
        db  0A3h, 05Ch, 01Ah, 000h, 000h
        cmp    eax, 400h
        jae    SHORT loc_4_0940
        db  0C7h, 005h, 05Ch, 01Ah, 000h, 000h, 000h, 004h, 000h, 000h
        jmp    SHORT loc_4_0956
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_4_0940:
        db  0A3h, 05Ch, 01Ah, 000h, 000h
        cmp    eax, 0c04h
        jbe    SHORT loc_4_0956
        db  0C7h, 005h, 05Ch, 01Ah, 000h, 000h, 004h, 00Ch, 000h, 000h
loc_4_0956:
        db  0E8h, 0D6h, 03Eh, 0FCh, 0FFh
        push   eax
        db  0E8h, 091h, 0F8h, 0FBh, 0FFh
        add    esp, 4
        db  0BEh, 0BCh, 010h, 000h, 000h
        db  0BFh, 070h, 004h, 000h, 000h
        mov    ecx, 5
        rep movsd
        movsw
        movsb
        lea    eax, [ebp - 18h]
        mov    dword ptr [ebp - 1ech], eax
        db  0C7h, 085h, 018h, 0FEh, 0FFh, 0FFh, 0D4h, 010h, 000h, 000h
        mov    dword ptr [ebp - 1e4h], 80000002h
        push   dword ptr [ebp - 1ech]
        push   dword ptr [ebp - 1e8h]
        push   dword ptr [ebp - 1e4h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_4_1125
        mov    dword ptr [ebp - 10h], 30h
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 1e0h], eax
        db  0C7h, 085h, 024h, 0FEh, 0FFh, 0FFh, 070h, 004h, 000h, 000h
        lea    eax, [ebp - 1ch]
        mov    dword ptr [ebp - 1d8h], eax
        mov    dword ptr [ebp - 1d4h], 0
        db  0C7h, 085h, 030h, 0FEh, 0FFh, 0FFh, 004h, 011h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 1cch], eax
        push   dword ptr [ebp - 1e0h]
        push   dword ptr [ebp - 1dch]
        push   dword ptr [ebp - 1d8h]
        push   dword ptr [ebp - 1d4h]
        push   dword ptr [ebp - 1d0h]
        push   dword ptr [ebp - 1cch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        mov    dword ptr [ebp - 10h], 2
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 1c8h], eax
        db  0C7h, 085h, 03Ch, 0FEh, 0FFh, 0FFh, 014h, 004h, 000h, 000h
        lea    eax, [ebp - 1ch]
        mov    dword ptr [ebp - 1c0h], eax
        mov    dword ptr [ebp - 1bch], 0
        db  0C7h, 085h, 048h, 0FEh, 0FFh, 0FFh, 014h, 011h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 1b4h], eax
        push   dword ptr [ebp - 1c8h]
        push   dword ptr [ebp - 1c4h]
        push   dword ptr [ebp - 1c0h]
        push   dword ptr [ebp - 1bch]
        push   dword ptr [ebp - 1b8h]
        push   dword ptr [ebp - 1b4h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        mov    dword ptr [ebp - 10h], 1
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 1b0h], eax
        db  0C7h, 085h, 054h, 0FEh, 0FFh, 0FFh, 05Eh, 004h, 000h, 000h
        lea    eax, [ebp - 1ch]
        mov    dword ptr [ebp - 1a8h], eax
        mov    dword ptr [ebp - 1a4h], 0
        db  0C7h, 085h, 060h, 0FEh, 0FFh, 0FFh, 024h, 011h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 19ch], eax
        push   dword ptr [ebp - 1b0h]
        push   dword ptr [ebp - 1ach]
        push   dword ptr [ebp - 1a8h]
        push   dword ptr [ebp - 1a4h]
        push   dword ptr [ebp - 1a0h]
        push   dword ptr [ebp - 19ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        mov    dword ptr [ebp - 10h], 1
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 198h], eax
        db  0C7h, 085h, 06Ch, 0FEh, 0FFh, 0FFh, 05Fh, 004h, 000h, 000h
        lea    eax, [ebp - 1ch]
        mov    dword ptr [ebp - 190h], eax
        mov    dword ptr [ebp - 18ch], 0
        db  0C7h, 085h, 078h, 0FEh, 0FFh, 0FFh, 030h, 011h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 184h], eax
        push   dword ptr [ebp - 198h]
        push   dword ptr [ebp - 194h]
        push   dword ptr [ebp - 190h]
        push   dword ptr [ebp - 18ch]
        push   dword ptr [ebp - 188h]
        push   dword ptr [ebp - 184h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        mov    dword ptr [ebp - 10h], 2
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 180h], eax
        db  0C7h, 085h, 084h, 0FEh, 0FFh, 0FFh, 060h, 004h, 000h, 000h
        lea    eax, [ebp - 1ch]
        mov    dword ptr [ebp - 178h], eax
        mov    dword ptr [ebp - 174h], 0
        db  0C7h, 085h, 090h, 0FEh, 0FFh, 0FFh, 03Ch, 011h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 16ch], eax
        push   dword ptr [ebp - 180h]
        push   dword ptr [ebp - 17ch]
        push   dword ptr [ebp - 178h]
        push   dword ptr [ebp - 174h]
        push   dword ptr [ebp - 170h]
        push   dword ptr [ebp - 16ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        mov    dword ptr [ebp - 10h], 4
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 168h], eax
        db  0C7h, 085h, 09Ch, 0FEh, 0FFh, 0FFh, 0A4h, 005h, 000h, 000h
        lea    eax, [ebp - 1ch]
        mov    dword ptr [ebp - 160h], eax
        mov    dword ptr [ebp - 15ch], 0
        db  0C7h, 085h, 0A8h, 0FEh, 0FFh, 0FFh, 044h, 011h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 154h], eax
        push   dword ptr [ebp - 168h]
        push   dword ptr [ebp - 164h]
        push   dword ptr [ebp - 160h]
        push   dword ptr [ebp - 15ch]
        push   dword ptr [ebp - 158h]
        push   dword ptr [ebp - 154h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        mov    dword ptr [ebp - 10h], 1
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 150h], eax
        db  0C7h, 085h, 0B4h, 0FEh, 0FFh, 0FFh, 06Dh, 004h, 000h, 000h
        lea    eax, [ebp - 1ch]
        mov    dword ptr [ebp - 148h], eax
        mov    dword ptr [ebp - 144h], 0
        db  0C7h, 085h, 0C0h, 0FEh, 0FFh, 0FFh, 04Ch, 011h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 13ch], eax
        push   dword ptr [ebp - 150h]
        push   dword ptr [ebp - 14ch]
        push   dword ptr [ebp - 148h]
        push   dword ptr [ebp - 144h]
        push   dword ptr [ebp - 140h]
        push   dword ptr [ebp - 13ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        mov    dword ptr [ebp - 10h], 1
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 138h], eax
        db  0C7h, 085h, 0CCh, 0FEh, 0FFh, 0FFh, 06Eh, 004h, 000h, 000h
        lea    eax, [ebp - 1ch]
        mov    dword ptr [ebp - 130h], eax
        mov    dword ptr [ebp - 12ch], 0
        db  0C7h, 085h, 0D8h, 0FEh, 0FFh, 0FFh, 060h, 011h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 124h], eax
        push   dword ptr [ebp - 138h]
        push   dword ptr [ebp - 134h]
        push   dword ptr [ebp - 130h]
        push   dword ptr [ebp - 12ch]
        push   dword ptr [ebp - 128h]
        push   dword ptr [ebp - 124h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        mov    dword ptr [ebp - 10h], 1
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 120h], eax
        db  0C7h, 085h, 0E4h, 0FEh, 0FFh, 0FFh, 069h, 004h, 000h, 000h
        lea    eax, [ebp - 1ch]
        mov    dword ptr [ebp - 118h], eax
        mov    dword ptr [ebp - 114h], 0
        db  0C7h, 085h, 0F0h, 0FEh, 0FFh, 0FFh, 06Ch, 011h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 10ch], eax
        push   dword ptr [ebp - 120h]
        push   dword ptr [ebp - 11ch]
        push   dword ptr [ebp - 118h]
        push   dword ptr [ebp - 114h]
        push   dword ptr [ebp - 110h]
        push   dword ptr [ebp - 10ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        mov    dword ptr [ebp - 10h], 1
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 108h], eax
        db  0C7h, 085h, 0FCh, 0FEh, 0FFh, 0FFh, 064h, 004h, 000h, 000h
        lea    eax, [ebp - 1ch]
        mov    dword ptr [ebp - 100h], eax
        mov    dword ptr [ebp - 0fch], 0
        db  0C7h, 085h, 008h, 0FFh, 0FFh, 0FFh, 07Ch, 011h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 0f4h], eax
        push   dword ptr [ebp - 108h]
        push   dword ptr [ebp - 104h]
        push   dword ptr [ebp - 100h]
        push   dword ptr [ebp - 0fch]
        push   dword ptr [ebp - 0f8h]
        push   dword ptr [ebp - 0f4h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        mov    dword ptr [ebp - 10h], 2
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 0f0h], eax
        db  0C7h, 085h, 014h, 0FFh, 0FFh, 0FFh, 065h, 004h, 000h, 000h
        lea    eax, [ebp - 1ch]
        mov    dword ptr [ebp - 0e8h], eax
        mov    dword ptr [ebp - 0e4h], 0
        db  0C7h, 085h, 020h, 0FFh, 0FFh, 0FFh, 088h, 011h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 0dch], eax
        push   dword ptr [ebp - 0f0h]
        push   dword ptr [ebp - 0ech]
        push   dword ptr [ebp - 0e8h]
        push   dword ptr [ebp - 0e4h]
        push   dword ptr [ebp - 0e0h]
        push   dword ptr [ebp - 0dch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        mov    dword ptr [ebp - 10h], 1
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 0d8h], eax
        db  0C7h, 085h, 02Ch, 0FFh, 0FFh, 0FFh, 067h, 004h, 000h, 000h
        lea    eax, [ebp - 1ch]
        mov    dword ptr [ebp - 0d0h], eax
        mov    dword ptr [ebp - 0cch], 0
        db  0C7h, 085h, 038h, 0FFh, 0FFh, 0FFh, 098h, 011h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 0c4h], eax
        push   dword ptr [ebp - 0d8h]
        push   dword ptr [ebp - 0d4h]
        push   dword ptr [ebp - 0d0h]
        push   dword ptr [ebp - 0cch]
        push   dword ptr [ebp - 0c8h]
        push   dword ptr [ebp - 0c4h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        mov    dword ptr [ebp - 10h], 1
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 0c0h], eax
        db  0C7h, 085h, 044h, 0FFh, 0FFh, 0FFh, 068h, 004h, 000h, 000h
        lea    eax, [ebp - 1ch]
        mov    dword ptr [ebp - 0b8h], eax
        mov    dword ptr [ebp - 0b4h], 0
        db  0C7h, 085h, 050h, 0FFh, 0FFh, 0FFh, 0A4h, 011h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 0ach], eax
        push   dword ptr [ebp - 0c0h]
        push   dword ptr [ebp - 0bch]
        push   dword ptr [ebp - 0b8h]
        push   dword ptr [ebp - 0b4h]
        push   dword ptr [ebp - 0b0h]
        push   dword ptr [ebp - 0ach]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        mov    dword ptr [ebp - 10h], 1
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 0a8h], eax
        db  0C7h, 085h, 05Ch, 0FFh, 0FFh, 0FFh, 06Ch, 004h, 000h, 000h
        lea    eax, [ebp - 1ch]
        mov    dword ptr [ebp - 0a0h], eax
        mov    dword ptr [ebp - 9ch], 0
        db  0C7h, 085h, 068h, 0FFh, 0FFh, 0FFh, 0B8h, 011h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 94h], eax
        push   dword ptr [ebp - 0a8h]
        push   dword ptr [ebp - 0a4h]
        push   dword ptr [ebp - 0a0h]
        push   dword ptr [ebp - 9ch]
        push   dword ptr [ebp - 98h]
        push   dword ptr [ebp - 94h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        mov    dword ptr [ebp - 10h], 1
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 90h], eax
        db  0C7h, 085h, 074h, 0FFh, 0FFh, 0FFh, 06Ah, 004h, 000h, 000h
        lea    eax, [ebp - 1ch]
        mov    dword ptr [ebp - 88h], eax
        mov    dword ptr [ebp - 84h], 0
        db  0C7h, 045h, 080h, 0C4h, 011h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 7ch], eax
        push   dword ptr [ebp - 90h]
        push   dword ptr [ebp - 8ch]
        push   dword ptr [ebp - 88h]
        push   dword ptr [ebp - 84h]
        push   dword ptr [ebp - 80h]
        push   dword ptr [ebp - 7ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        mov    dword ptr [ebp - 10h], 1
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 78h], eax
        db  0C7h, 045h, 08Ch, 06Bh, 004h, 000h, 000h
        lea    eax, [ebp - 1ch]
        mov    dword ptr [ebp - 70h], eax
        mov    dword ptr [ebp - 6ch], 0
        db  0C7h, 045h, 098h, 0D0h, 011h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 64h], eax
        push   dword ptr [ebp - 78h]
        push   dword ptr [ebp - 74h]
        push   dword ptr [ebp - 70h]
        push   dword ptr [ebp - 6ch]
        push   dword ptr [ebp - 68h]
        push   dword ptr [ebp - 64h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        mov    dword ptr [ebp - 10h], 1
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 60h], eax
        db  0C7h, 045h, 0A4h, 06Fh, 004h, 000h, 000h
        lea    eax, [ebp - 1ch]
        mov    dword ptr [ebp - 58h], eax
        mov    dword ptr [ebp - 54h], 0
        db  0C7h, 045h, 0B0h, 0DCh, 011h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 4ch], eax
        push   dword ptr [ebp - 60h]
        push   dword ptr [ebp - 5ch]
        push   dword ptr [ebp - 58h]
        push   dword ptr [ebp - 54h]
        push   dword ptr [ebp - 50h]
        push   dword ptr [ebp - 4ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 48h], eax
        push   dword ptr [ebp - 48h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
loc_4_1125:
        lea    eax, [ebp - 18h]
        mov    dword ptr [ebp - 44h], eax
        db  0C7h, 045h, 0C0h, 0F8h, 011h, 000h, 000h
        mov    dword ptr [ebp - 3ch], 80000002h
        push   dword ptr [ebp - 44h]
        push   dword ptr [ebp - 40h]
        push   dword ptr [ebp - 3ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_4_11B1
        mov    dword ptr [ebp - 10h], 30h
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 38h], eax
        db  0C7h, 045h, 0CCh, 016h, 004h, 000h, 000h
        lea    eax, [ebp - 1ch]
        mov    dword ptr [ebp - 30h], eax
        mov    dword ptr [ebp - 2ch], 0
        db  0C7h, 045h, 0D8h, 034h, 012h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 24h], eax
        push   dword ptr [ebp - 38h]
        push   dword ptr [ebp - 34h]
        push   dword ptr [ebp - 30h]
        push   dword ptr [ebp - 2ch]
        push   dword ptr [ebp - 28h]
        push   dword ptr [ebp - 24h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        db  068h, 016h, 004h, 000h, 000h
        db  0E8h, 018h, 0F6h, 0FBh, 0FFh
        add    esp, 4
loc_4_11B1:
        db  080h, 03Dh, 094h, 003h, 000h, 000h, 000h
        jne    SHORT loc_4_1206
        push   1
        push   100h
        db  0E8h, 0F8h, 0F7h, 0FBh, 0FFh
        add    esp, 8
        db  0A3h, 0A4h, 003h, 000h, 000h
        push   1
        push   180h
        db  0E8h, 0E4h, 0F7h, 0FBh, 0FFh
        add    esp, 8
        db  0A3h, 0A8h, 003h, 000h, 000h
        db  083h, 03Dh, 0A4h, 003h, 000h, 000h, 000h
        jne    SHORT loc_4_11F4
        mov    al, 0ffh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_4_11F4:
        db  0A3h, 0A8h, 003h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_4_1206
        mov    al, 0ffh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_4_1206:
        db  080h, 03Dh, 0C6h, 003h, 000h, 000h, 000h
        je     SHORT loc_4_1228
        db  066h, 08Bh, 00Dh, 0C4h, 003h, 000h, 000h
        call   sub_4_1B90
        cmp    al, 0ffh
        jne    SHORT loc_4_1228
        mov    al, 0ffh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_4_1228:
        xor    eax, eax
        db  066h, 0A1h, 014h, 004h, 000h, 000h
        push   1
        lea    eax, [eax + eax*4]
        shl    eax, 5
        push   eax
        db  0E8h, 080h, 0F7h, 0FBh, 0FFh
        add    esp, 8
        db  0A3h, 0A0h, 003h, 000h, 000h
        push   1
        push   20h
        db  0E8h, 06Fh, 0F7h, 0FBh, 0FFh
        add    esp, 8
        db  0A3h, 0ACh, 003h, 000h, 000h
        push   1
        push   4d0h
        db  0E8h, 05Bh, 0F7h, 0FBh, 0FFh
        add    esp, 8
        db  0A3h, 0B0h, 003h, 000h, 000h
        db  08Bh, 00Dh, 0A0h, 003h, 000h, 000h
        or     ecx, ecx
        jne    SHORT loc_4_1280
        mov    al, 0ffh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_4_1280:
        db  083h, 03Dh, 0ACh, 003h, 000h, 000h, 000h
        jne    SHORT loc_4_1292
        mov    al, 0ffh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_4_1292:
        db  083h, 03Dh, 0B0h, 003h, 000h, 000h, 000h
        jne    SHORT loc_4_12A4
        mov    al, 0ffh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_4_12A4:
        db  0C7h, 005h, 0A8h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 012h, 004h, 000h, 000h, 000h
        db  0C6h, 005h, 013h, 004h, 000h, 000h, 000h
        sub    edi, edi
        db  066h, 039h, 03Dh, 014h, 004h, 000h, 000h
        jbe    SHORT loc_4_133C
        mov    eax, ecx
        sub    edx, edx
        mov    ebx, 0ffh
loc_4_12D0:
        mov    dword ptr [eax], eax
        mov    dword ptr [eax + 4], eax
        lea    esi, [eax + 8]
        mov    dword ptr [eax + 8], esi
        mov    dword ptr [eax + 0ch], esi
        mov    dword ptr [eax + 10h], edx
        mov    byte ptr [eax + 58h], dl
        mov    byte ptr [eax + 5ah], dl
        mov    byte ptr [eax + 34h], dl
        mov    byte ptr [eax + 35h], dl
        mov    byte ptr [eax + 36h], dl
        mov    byte ptr [eax + 37h], dl
        mov    byte ptr [eax + 56h], bl
        mov    byte ptr [eax + 57h], 1
        mov    byte ptr [eax + 77h], dl
        mov    byte ptr [eax + 5ch], dl
        mov    dword ptr [eax + 18h], edx
        mov    dword ptr [eax + 1ch], edx
        mov    byte ptr [eax + 5bh], dl
        mov    dword ptr [eax + 38h], edx
        mov    word ptr [eax + 50h], dx
        mov    dword ptr [eax + 3ch], edx
        inc    edi
        mov    dword ptr [eax + 40h], edx
        mov    dword ptr [eax + 44h], edx
        mov    dword ptr [eax + 64h], edx
        mov    dword ptr [eax + 30h], edx
        xor    esi, esi
        mov    dword ptr [eax + 84h], 4e4e4f43h
        add    eax, 0a0h
        db  066h, 08Bh, 035h, 014h, 004h, 000h, 000h
        cmp    esi, edi
        ja     SHORT loc_4_12D0
loc_4_133C:
        mov    dword ptr [ebp - 20h], 0
        sub    ebx, ebx
        sub    edi, edi
        sub    esi, esi
        lea    esp, [esp]
loc_4_1350:
        db  080h, 03Dh, 0C6h, 003h, 000h, 000h, 000h
        je     SHORT loc_4_13A0
        db  0A1h, 0A4h, 003h, 000h, 000h
        cmp    byte ptr [esi + eax], 0
        je     SHORT loc_4_13A0
        add    ecx, edi
        mov    edx, dword ptr [ebp - 20h]
        call   sub_4_1840
        cmp    eax, 0ffh
        db  08Bh, 00Dh, 0A0h, 003h, 000h, 000h
        je     NEAR PTR loc_4_1468
        lea    edx, [edi + ecx]
        db  0A1h, 0ACh, 003h, 000h, 000h
        mov    dword ptr [ebx + eax], edx
        mov    al, byte ptr [ebp - 20h]
        mov    byte ptr [edi + ecx + 56h], al
        db  0A1h, 0A4h, 003h, 000h, 000h
        add    eax, esi
        mov    dword ptr [edi + ecx + 38h], eax
        jmp    SHORT loc_4_13B5
        db  08Bh, 0FFh
loc_4_13A0:
        db  0A1h, 0A4h, 003h, 000h, 000h
        mov    byte ptr [esi + eax], 0
        db  0A1h, 0ACh, 003h, 000h, 000h
        mov    dword ptr [ebx + eax], 0
loc_4_13B5:
        add    ebx, 4
        add    edi, 0a0h
        add    esi, 20h
        inc    dword ptr [ebp - 20h]
        cmp    esi, 100h
        jb     SHORT loc_4_1350
        db  08Bh, 00Dh, 0B0h, 003h, 000h, 000h
        add    ecx, 8
        mov    eax, 8
        lea    ebx, [ebx]
loc_4_13E0:
        mov    byte ptr [ecx], 0
        add    ecx, 9ah
        dec    eax
        jne    SHORT loc_4_13E0
        db  080h, 03Dh, 094h, 003h, 000h, 000h, 000h
        je     NEAR PTR loc_4_165E
        mov    byte ptr [ebp - 2], 0
        db  08Bh, 01Dh, 0EFh, 003h, 000h, 000h
        db  0A1h, 0F3h, 003h, 000h, 000h
        mov    dword ptr [ebp - 14h], eax
        push   1
        push   80h
        db  0E8h, 0A7h, 0F5h, 0FBh, 0FFh
        add    esp, 8
        db  0A3h, 0DCh, 003h, 000h, 000h
        push   1
        push   20h
        db  0E8h, 096h, 0F5h, 0FBh, 0FFh
        add    esp, 8
        db  0A3h, 0D8h, 003h, 000h, 000h
        push   1
        push   0c0h
        db  0E8h, 082h, 0F5h, 0FBh, 0FFh
        add    esp, 8
        db  0A3h, 0E0h, 003h, 000h, 000h
        push   1
        push   18h
        db  0E8h, 071h, 0F5h, 0FBh, 0FFh
        add    esp, 8
        db  0A3h, 0E4h, 003h, 000h, 000h
        db  08Bh, 015h, 0DCh, 003h, 000h, 000h
        or     edx, edx
        jne    SHORT loc_4_1471
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_4_1468:
        mov    al, 0ffh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_4_1471:
        db  08Bh, 00Dh, 0D8h, 003h, 000h, 000h
        or     ecx, ecx
        jne    SHORT loc_4_1484
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_4_1484:
        db  083h, 03Dh, 0E0h, 003h, 000h, 000h, 000h
        jne    SHORT loc_4_1496
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_4_1496:
        db  083h, 03Dh, 0E4h, 003h, 000h, 000h, 000h
        jne    SHORT loc_4_14A8
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_4_14A8:
        db  080h, 03Dh, 0C6h, 003h, 000h, 000h, 000h
        je     SHORT loc_4_14E1
        sub    esi, esi
        db  08Bh, 03Dh, 0A4h, 003h, 000h, 000h
        lea    esp, [esp]
loc_4_14C0:
        db  080h, 03Dh, 0ECh, 003h, 000h, 000h, 000h
        jne    SHORT loc_4_14E8
        cmp    byte ptr [edi], 0
        je     SHORT loc_4_14D6
        lea    eax, [esi + 1]
        db  0A2h, 0ECh, 003h, 000h, 000h
loc_4_14D6:
        add    edi, 20h
        inc    esi
        cmp    esi, 8
        jb     SHORT loc_4_14C0
        jmp    SHORT loc_4_14E8
loc_4_14E1:
        db  0C6h, 005h, 0ECh, 003h, 000h, 000h, 000h
loc_4_14E8:
        db  0C6h, 005h, 0EDh, 003h, 000h, 000h, 000h
        db  0B8h, 0F7h, 003h, 000h, 000h
        db  0C6h, 005h, 0EEh, 003h, 000h, 000h, 000h
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        xor    edi, edi
        db  08Bh, 035h, 008h, 004h, 000h, 000h
        mov    byte ptr [eax + 0ch], 0
        mov    al, byte ptr [esi + 21h]
        db  0A2h, 011h, 004h, 000h, 000h
        xor    eax, eax
        db  0C6h, 005h, 010h, 004h, 000h, 000h, 058h
        mov    di, word ptr [esi + 18h]
        shl    edi, 4
        mov    ax, word ptr [esi + 16h]
        add    eax, edi
        db  0A3h, 00Ch, 004h, 000h, 000h
        sub    al, al
loc_4_1540:
        mov    esi, eax
        and    esi, 0ffh
        mov    byte ptr [ecx + esi], 0
        mov    dword ptr [edx + esi*4], 0
        db  080h, 0BEh, 0F6h, 00Dh, 000h, 000h, 000h
        je     SHORT loc_4_155F
        mov    byte ptr [ebp - 2], al
loc_4_155F:
        inc    al
        cmp    al, 20h
        jb     SHORT loc_4_1540
        db  0BEh, 0F6h, 00Dh, 000h, 000h
        db  08Bh, 03Dh, 0C8h, 003h, 000h, 000h
        mov    ecx, 8
        rep movsd
        xor    ecx, ecx
        db  08Ah, 00Dh, 06Dh, 004h, 000h, 000h
        xor    eax, eax
        mov    al, byte ptr [ebp - 2]
        dec    ecx
        cmp    ecx, eax
        jg     SHORT loc_4_158B
        mov    ecx, eax
loc_4_158B:
        or     cl, cl
        je     SHORT loc_4_15AC
        xor    edx, edx
        mov    dl, cl
        db  08Bh, 03Dh, 0C8h, 003h, 000h, 000h
        mov    eax, 80808080h
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
loc_4_15AC:
        mov    byte ptr [ebp - 1], 0
loc_4_15B0:
        xor    edx, edx
        mov    dl, byte ptr [ebp - 1]
        mov    ecx, edx
        shl    ecx, 6
        db  08Bh, 035h, 0E0h, 003h, 000h, 000h
        add    esi, ecx
        mov    edi, esi
        sub    eax, eax
        mov    ecx, 10h
        rep stosd
        db  0B8h, 048h, 012h, 000h, 000h
        lea    ecx, [esi + 7]
        mov    edi, dword ptr [eax]
        mov    dword ptr [ecx], edi
        mov    al, byte ptr [eax + 4]
        mov    byte ptr [ecx + 4], al
        mov    byte ptr [esi + 1], 80h
        mov    byte ptr [esi + 2], 8
        mov    byte ptr [esi + 4], 1
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 15h], al
        mov    word ptr [ebp - 0ah], 42h
        mov    ax, word ptr [ebp - 0ah]
        xchg   al, ah
        mov    word ptr [ebp - 8], ax
        mov    ax, word ptr [ebp - 8]
        mov    word ptr [esi + 19h], ax
        mov    word ptr [ebp - 6], 84h
        mov    ax, word ptr [ebp - 6]
        xchg   al, ah
        mov    word ptr [ebp - 4], ax
        mov    ax, word ptr [ebp - 4]
        mov    word ptr [esi + 1bh], ax
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [esi + 2dh], eax
        mov    word ptr [ebx], 40h
        add    ebx, 44h
        add    dword ptr [ebp - 14h], 44h
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [esi + 31h], eax
        mov    word ptr [ebx], 14h
        add    ebx, 18h
        add    dword ptr [ebp - 14h], 18h
        db  0A1h, 0E4h, 003h, 000h, 000h
        mov    dword ptr [eax + edx*8], 0
        inc    byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 1], 3
        jb     NEAR PTR loc_4_15B0
loc_4_165E:
        db  0E8h, 06Dh, 0B6h, 0FCh, 0FFh
        or     eax, eax
        jne    SHORT loc_4_166C
        db  0E8h, 0F4h, 0B6h, 0FCh, 0FFh
loc_4_166C:
        db  0BFh, 0A0h, 020h, 000h, 000h
        sub    eax, eax
        mov    ecx, 9
        rep stosd
        stosw
        db  038h, 005h, 0C6h, 003h, 000h, 000h
        je     SHORT loc_4_1689
        call   sub_4_16A0
loc_4_1689:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_16A0:
        sub    esp, 144h
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  08Bh, 01Dh, 0C0h, 003h, 000h, 000h
        lea    edx, [ebx + 0a8h]
        mov    dword ptr [esp + 14h], edx
        cmp    byte ptr [ebx + 2], 2
        je     SHORT loc_4_16D0
        mov    eax, 0ffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 144h
        ret
loc_4_16D0:
        cmp    word ptr [ebx], 4954h
        je     SHORT loc_4_16E7
        mov    eax, 0ffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 144h
        ret
loc_4_16E7:
        db  0C6h, 005h, 0C7h, 003h, 000h, 000h, 001h
        add    ebx, 0a7h
        sub    ebp, ebp
        cmp    byte ptr [ebx], 0
        jle    NEAR PTR loc_4_1821
        db  08Bh, 00Dh, 0CCh, 003h, 000h, 000h
loc_4_1705:
        xor    eax, eax
        mov    ax, word ptr [edx]
        add    eax, 2
        add    dword ptr [esp + 14h], eax
        movsx  eax, byte ptr [edx + 2]
        mov    word ptr [esp + 36h], 1
        mov    word ptr [esp + 38h], 0
        mov    word ptr [esp + 30h], 0
        mov    al, byte ptr [ecx + eax - 41h]
        dec    al
        mov    cl, byte ptr [edx + 2]
        mov    byte ptr [esp + 1ch], cl
        mov    byte ptr [esp + 1dh], 3ah
        mov    byte ptr [esp + 1eh], 0
        db  0B9h, 050h, 012h, 000h, 000h
        lea    esi, [esp + 50h]
        mov    di, word ptr [ecx]
        mov    word ptr [esi], di
        mov    cl, byte ptr [ecx + 2]
        and    eax, 0ffh
        shl    eax, 4
        mov    byte ptr [esi + 2], cl
        lea    eax, [eax + eax*2]
        db  08Bh, 03Dh, 0A8h, 003h, 000h, 000h
        add    edi, eax
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    dword ptr [esp + 10h], ecx
        lea    edi, [esp + 50h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, dword ptr [esp + 10h]
        shr    ecx, 2
        rep movsd
        mov    ecx, dword ptr [esp + 10h]
        and    ecx, 3
        rep movsb
        lea    edi, [esp + 50h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        db  066h, 0A1h, 054h, 012h, 000h, 000h
        mov    word ptr [edi - 1], ax
        lea    edi, [edx + 4]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        lea    edi, [esp + 50h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        lea    eax, [esp + 50h]
        mov    dword ptr [esp + 26h], eax
        db  0C7h, 044h, 024h, 02Ah, 058h, 012h, 000h, 000h
        push   0ffh
        push   1
        inc    ebp
        lea    eax, [esp + 24h]
        push   -1
        push   eax
        db  0E8h, 00Ah, 0F3h, 0FBh, 0FFh
        mov    edx, dword ptr [esp + 24h]
        add    esp, 10h
        movsx  ecx, byte ptr [ebx]
        cmp    ecx, ebp
        db  08Bh, 00Dh, 0CCh, 003h, 000h, 000h
        jg     NEAR PTR loc_4_1705
        jmp    SHORT loc_4_1825
loc_4_1821:
        mov    eax, dword ptr [esp + 18h]
loc_4_1825:
        db  0C6h, 005h, 0C7h, 003h, 000h, 000h, 000h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 144h
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1840:
        sub    esp, 5ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, edx
        mov    esi, ecx
        shl    edx, 5
        db  08Bh, 03Dh, 0A4h, 003h, 000h, 000h
        add    edi, edx
        mov    byte ptr [esp + 64h], 0
        mov    byte ptr [esp + 65h], 0ffh
        db  0E8h, 01Bh, 0F4h, 0FBh, 0FFh
        mov    eax, dword ptr [esi + 40h]
        mov    dword ptr [esp + 1ch], eax
        mov    eax, dword ptr [esi + 44h]
        mov    dword ptr [esp + 20h], eax
        lea    ebp, [edi + 2]
        mov    ecx, dword ptr [esp + 1ch]
        mov    eax, dword ptr [ebp]
        mov    dword ptr [ecx + 0aeh], eax
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [ecx + 0b2h], eax
        add    ecx, 0aeh
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [esp + 1ch]
        add    eax, 1ch
        mov    dword ptr [esp + 14h], eax
        mov    dword ptr [esp + 18h], 3
loc_4_18AB:
        lea    eax, [esp + 10h]
        push   eax
        mov    eax, dword ptr [esp + 18h]
        push   eax
        push   ebp
        db  0E8h, 0BCh, 0F2h, 0FBh, 0FFh
        add    esp, 0ch
        or     al, al
        je     SHORT loc_4_18DF
        dec    dword ptr [esp + 18h]
        jne    SHORT loc_4_18AB
        or     al, al
        je     SHORT loc_4_18DF
        lea    eax, [edi + 10h]
        mov    ecx, dword ptr [esp + 14h]
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ecx + 4], ax
loc_4_18DF:
        mov    eax, dword ptr [esp + 1ch]
        lea    ecx, [ebx + ebx*4]
        mov    byte ptr [eax + 0adh], 11h
        shl    ecx, 5
        db  003h, 00Dh, 0A0h, 003h, 000h, 000h
        mov    dword ptr [eax + 90h], ecx
        mov    eax, dword ptr [esp + 20h]
        dec    byte ptr [edi + 16h]
        mov    dword ptr [eax + 90h], ecx
        mov    byte ptr [esi + 56h], bl
        shl    ebx, 4
        db  066h, 0A1h, 0CCh, 005h, 000h, 000h
        mov    word ptr [esi + 60h], ax
        db  066h, 0A1h, 0D2h, 005h, 000h, 000h
        mov    word ptr [esi + 62h], ax
        lea    ebx, [ebx + ebx*2]
        mov    dword ptr [esi + 38h], edi
        db  0A1h, 0A8h, 003h, 000h, 000h
        add    ebx, eax
        mov    dword ptr [esi + 3ch], ebx
        mov    byte ptr [esi + 5bh], 0
        mov    dword ptr [esi + 64h], 0
        and    byte ptr [esi + 58h], 0efh
        mov    word ptr [esi + 4ch], 222h
        mov    word ptr [esi + 4eh], 20eh
        mov    byte ptr [esi + 55h], 0
        mov    byte ptr [esi + 57h], 1
        mov    byte ptr [esi + 77h], 0
        mov    byte ptr [esi + 5ch], 0
        mov    dword ptr [esi + 88h], 0
        mov    dword ptr [esi + 8ch], 0
        mov    word ptr [esi + 90h], 0
        mov    dword ptr [esi + 48h], 0
        db  0A0h, 05Eh, 004h, 000h, 000h
        mov    byte ptr [esi + 54h], al
        mov    byte ptr [esi + 59h], 1
        mov    ax, word ptr [esp + 10h]
        imul   ax, ax, 0a5h
        add    ax, 113h
        mov    word ptr [esi + 6ch], ax
        mov    ax, word ptr [esp + 10h]
        imul   ax, ax, 226h
        add    ax, 113h
        mov    word ptr [esi + 6eh], ax
        mov    ax, word ptr [esp + 10h]
        mov    word ptr [esi + 52h], ax
        mov    al, byte ptr [edi + 1bh]
        mov    byte ptr [esi + 73h], al
        lea    ecx, [esi + 78h]
        mov    al, byte ptr [edi + 1ch]
        mov    byte ptr [esi + 74h], al
        mov    al, byte ptr [edi + 1dh]
        mov    byte ptr [esi + 75h], al
        mov    al, byte ptr [edi + 1fh]
        mov    byte ptr [esi + 76h], al
        mov    eax, dword ptr [ebp]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 8], eax
        mov    al, byte ptr [edi + 16h]
        mov    byte ptr [esi + 72h], al
        db  080h, 03Dh, 069h, 004h, 000h, 000h, 000h
        je     NEAR PTR loc_4_1B79
        cmp    byte ptr [esi + 5bh], 0
        ja     NEAR PTR loc_4_1B79
        push   1
        push   12ah
        db  0E8h, 0AEh, 0EFh, 0FBh, 0FFh
        add    esp, 8
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_4_1A23
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 5ch
        ret
loc_4_1A23:
        mov    word ptr [edi + 54h], 0
        mov    word ptr [edi + 2ch], 10h
        mov    word ptr [edi + 2eh], 4
        mov    word ptr [edi + 30h], 2
        lea    eax, [edi + 54h]
        mov    word ptr [edi + 32h], 0
        push   esi
        db  068h, 09Bh, 004h, 000h, 000h
        push   eax
        db  0E8h, 0A3h, 0F1h, 0FBh, 0FFh
        add    esp, 0ch
        or     al, al
        je     SHORT loc_4_1A70
        push   0
        push   edi
        db  0E8h, 072h, 0EFh, 0FBh, 0FFh
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 5ch
        ret
        db  08Dh, 064h, 024h, 000h
loc_4_1A70:
        db  0E8h, 07Eh, 0EFh, 0FBh, 0FFh
        mov    dword ptr [edi], eax
        mov    bp, word ptr [esi + 4ch]
        sub    bp, 24h
        and    ebp, 0fffch
        mov    eax, 0ffffh
        cdq
        idiv   ebp
        mov    ecx, eax
        cmp    ecx, 0fh
        jb     SHORT loc_4_1A99
        mov    ecx, 0fh
loc_4_1A99:
        imul   ecx, ebp
        mov    dword ptr [edi + 18h], ecx
        lea    ebx, [edi + 14h]
        cmp    eax, 0ah
        jb     SHORT loc_4_1AAC
        mov    eax, 0ah
loc_4_1AAC:
        imul   eax, ebp
        mov    dword ptr [ebx], eax
        push   ebx
        lea    edx, [edi + 18h]
        push   edx
        add    bp, 42h
        lea    edx, [esp + 1ah]
        push   edx
        lea    edx, [edi + 4]
        push   edx
        push   ecx
        push   eax
        xor    eax, eax
        mov    ax, word ptr [edi + 54h]
        push   eax
        mov    eax, ebp
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [edi]
        push   eax
        push   65h
        lea    eax, [esp + 4ch]
        db  068h, 05Ch, 012h, 000h, 000h
        push   eax
        push   esi
        db  0E8h, 027h, 0F3h, 0FBh, 0FFh
        add    esp, 34h
        or     al, al
        je     SHORT loc_4_1B12
        mov    ax, word ptr [edi + 54h]
        push   eax
        db  0E8h, 01Fh, 0F1h, 0FBh, 0FFh
        add    esp, 4
        push   0
        push   edi
        db  0E8h, 0CCh, 0EEh, 0FBh, 0FFh
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 5ch
        ret
loc_4_1B12:
        mov    ax, word ptr [esp + 12h]
        cmp    ax, bp
        jb     SHORT loc_4_1B1F
        mov    ax, bp
loc_4_1B1F:
        sub    ax, 42h
        mov    word ptr [edi + 46h], ax
        sub    dword ptr [ebx], 18h
        mov    dword ptr [edi + 8], 0
        xor    ecx, ecx
        sub    dword ptr [edi + 18h], 8
        mov    word ptr [edi + 48h], 0
        mov    word ptr [edi + 4ah], 0
        mov    word ptr [edi + 5ah], 0
        sub    edx, edx
        mov    word ptr [edi + 5ch], 0
        mov    cx, word ptr [esi + 4ch]
        mov    eax, dword ptr [esi + 24h]
        imul   eax, ecx
        mov    ecx, 3e8h
        div    ecx
        mov    dword ptr [edi + 10h], eax
        mov    dword ptr [edi + 0ch], eax
        mov    dword ptr [esi + 48h], edi
        mov    byte ptr [esi + 59h], 11h
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 5ch
        ret
loc_4_1B79:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 5ch
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1B90:
        sub    esp, 14h
        push   esi
        db  066h, 089h, 00Dh, 0CCh, 005h, 000h, 000h
        push   0
        db  068h, 062h, 004h, 000h, 000h
        db  068h, 0CCh, 005h, 000h, 000h
        db  0E8h, 047h, 0F0h, 0FBh, 0FFh
        add    esp, 0ch
        or     al, al
        je     SHORT loc_4_1BC0
        mov    al, 0ffh
        pop    esi
        add    esp, 14h
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_4_1BC0:
        db  066h, 0A1h, 0CCh, 005h, 000h, 000h
        mov    word ptr [esp + 16h], ax
        mov    ax, word ptr [esp + 16h]
        xchg   al, ah
        mov    word ptr [esp + 14h], ax
        mov    ax, word ptr [esp + 14h]
        inc    ax
        mov    word ptr [esp + 12h], ax
        mov    ax, word ptr [esp + 12h]
        xchg   al, ah
        mov    word ptr [esp + 10h], ax
        mov    ax, word ptr [esp + 10h]
        db  066h, 0A3h, 0D2h, 005h, 000h, 000h
        db  068h, 0D2h, 005h, 000h, 000h
        db  0E8h, 0CEh, 0EFh, 0FBh, 0FFh
        add    esp, 4
        or     al, al
        je     SHORT loc_4_1C12
        mov    al, 0ffh
        pop    esi
        add    esp, 14h
        ret
loc_4_1C12:
        push   1
        push   4ch
        db  0E8h, 0A3h, 0EDh, 0FBh, 0FFh
        add    esp, 8
        db  0A3h, 0ACh, 005h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_4_1C30
        mov    al, 0ffh
        pop    esi
        add    esp, 14h
        ret
        db  08Bh, 0FFh
loc_4_1C30:
        db  0A3h, 0ACh, 005h, 000h, 000h
        mov    byte ptr [eax + 2ah], 0
        db  0C7h, 040h, 004h, 050h, 009h, 000h, 000h
        db  066h, 08Bh, 00Dh, 0D2h, 005h, 000h, 000h
        mov    word ptr [eax + 0ah], cx
        mov    word ptr [eax + 22h], 1
        lea    ecx, [eax + 2ch]
        mov    dword ptr [eax + 24h], ecx
        mov    word ptr [eax + 28h], 20h
        push   eax
        db  0E8h, 00Dh, 0F0h, 0FBh, 0FFh
        add    esp, 4
        db  066h, 0A1h, 0CCh, 005h, 000h, 000h
        mov    word ptr [esp + 0eh], ax
        mov    ax, word ptr [esp + 0eh]
        xchg   al, ah
        mov    word ptr [esp + 0ch], ax
        mov    ax, word ptr [esp + 0ch]
        add    ax, 2
        mov    word ptr [esp + 0ah], ax
        mov    ax, word ptr [esp + 0ah]
        xchg   al, ah
        mov    word ptr [esp + 8], ax
        mov    ax, word ptr [esp + 8]
        db  066h, 0A3h, 0D4h, 005h, 000h, 000h
        db  068h, 0D4h, 005h, 000h, 000h
        db  0E8h, 026h, 0EFh, 0FBh, 0FFh
        add    esp, 4
        or     al, al
        je     SHORT loc_4_1CBA
        mov    al, 0ffh
        pop    esi
        add    esp, 14h
        ret
loc_4_1CBA:
        push   1
        push   4ch
        db  0E8h, 0FBh, 0ECh, 0FBh, 0FFh
        add    esp, 8
        db  0A3h, 0B0h, 005h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_4_1CD6
        mov    al, 0ffh
        pop    esi
        add    esp, 14h
        ret
loc_4_1CD6:
        db  0A3h, 0B0h, 005h, 000h, 000h
        mov    byte ptr [eax + 2ah], 0
        db  0C7h, 040h, 004h, 060h, 00Ah, 000h, 000h
        db  066h, 08Bh, 00Dh, 0D4h, 005h, 000h, 000h
        mov    word ptr [eax + 0ah], cx
        lea    ecx, [eax + 2ch]
        mov    word ptr [eax + 22h], 1
        mov    dword ptr [eax + 24h], ecx
        mov    word ptr [eax + 28h], 20h
        push   eax
        db  0E8h, 067h, 0EFh, 0FBh, 0FFh
        add    esp, 4
        push   1
        push   6ch
        db  0E8h, 0A9h, 0ECh, 0FBh, 0FFh
        add    esp, 8
        db  0A3h, 0B4h, 005h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_4_1D28
        mov    al, 0ffh
        pop    esi
        add    esp, 14h
        ret
loc_4_1D28:
        db  0A3h, 0B4h, 005h, 000h, 000h
        mov    byte ptr [eax + 8], 0
        db  066h, 0C7h, 005h, 050h, 004h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 05Ch, 004h, 000h, 000h, 000h, 000h
        push   1
        push   4ch
        db  0E8h, 072h, 0ECh, 0FBh, 0FFh
        add    esp, 8
        mov    ecx, eax
        db  089h, 00Dh, 0B8h, 005h, 000h, 000h
        or     ecx, ecx
        jne    SHORT loc_4_1D62
        mov    al, 0ffh
        pop    esi
        add    esp, 14h
        ret
loc_4_1D62:
        mov    byte ptr [ecx + 8], 0
        db  0C7h, 041h, 004h, 060h, 019h, 000h, 000h
        mov    word ptr [ecx + 0ah], 0
        mov    word ptr [ecx + 22h], 1
        lea    eax, [ecx + 2ah]
        mov    dword ptr [ecx + 24h], eax
        mov    word ptr [ecx + 28h], 22h
        db  0B8h, 06Ch, 012h, 000h, 000h
        lea    esi, [ecx + 1ch]
        mov    edx, dword ptr [eax]
        mov    dword ptr [esi], edx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [esi + 4], ax
        db  0BAh, 074h, 012h, 000h, 000h
        lea    esi, [ecx + 30h]
        mov    eax, dword ptr [edx]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [esi + 8], eax
        mov    byte ptr [ecx + 2fh], 11h
        mov    word ptr [esp + 6], 4
        mov    ax, word ptr [esp + 6]
        xchg   al, ah
        mov    word ptr [esp + 4], ax
        db  089h, 00Dh, 0B8h, 005h, 000h, 000h
        mov    ax, word ptr [esp + 4]
        mov    word ptr [ecx + 4ah], ax
        db  0C7h, 005h, 0BCh, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        sub    eax, eax
        db  0A3h, 0C0h, 005h, 000h, 000h
        pop    esi
        add    esp, 14h
        ret
        db  08Dh, 049h, 000h
sub_4_1DF0:
        db  080h, 03Dh, 0C6h, 003h, 000h, 000h, 000h
        je     SHORT loc_4_1E00
        sub    al, al
        ret
        db  08Dh, 064h, 024h, 000h
loc_4_1E00:
        sub    ecx, ecx
        jmp    NEAR PTR sub_4_1B90
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
_ITEXT ends
        end
