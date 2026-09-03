; ifsmgr_icode_03.asm -- IFSMGR object 3 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

        db  057h
loc_3_0001:
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        movzx  eax, word ptr [edi + 18h]
        pop    edi
        ret
sub_3_000D:
        mov    eax, dword ptr [esp + 4]
        db  080h, 03Dh, 0C7h, 00Fh, 000h, 000h, 001h
        jb     SHORT loc_3_0055
        ja     SHORT loc_3_0056
        push   eax
        call   sub_3_0060
        je     SHORT loc_3_0036
        cmp    al, 13h
        jne    SHORT loc_3_0036
loc_3_0028:
        jmp    SHORT loc_3_002B
        db  090h
loc_3_002B:
        call   sub_3_0060
        je     SHORT loc_3_0028
        cmp    al, 13h
        je     SHORT loc_3_0028
loc_3_0036:
        db  066h, 08Bh, 015h, 0C5h, 00Fh, 000h, 000h
        add    dx, 5
loc_3_0041:
        jmp    SHORT loc_3_0044
        db  090h
loc_3_0044:
        in     al, dx
        test   al, 20h
        je     SHORT loc_3_0041
        db  066h, 08Bh, 015h, 0C5h, 00Fh, 000h, 000h
        jmp    SHORT loc_3_0053
        db  090h
loc_3_0053:
        pop    eax
        out    dx, al
loc_3_0055:
        ret
loc_3_0056:
        mov    dl, al
        mov    eax, 0
        int    41h
        ret
sub_3_0060:
        sub    ax, ax
        db  080h, 03Dh, 0C7h, 00Fh, 000h, 000h, 001h
        jb     SHORT loc_3_008C
        ja     SHORT loc_3_008F
        db  066h, 08Bh, 015h, 0C5h, 00Fh, 000h, 000h
        add    dx, 5
        in     al, dx
        jmp    SHORT loc_3_007D
        db  090h
loc_3_007D:
        and    al, 1
        je     SHORT loc_3_008E
        db  066h, 08Bh, 015h, 0C5h, 00Fh, 000h, 000h
        in     al, dx
        jmp    SHORT loc_3_008C
        db  090h
loc_3_008C:
        and    al, 7fh
loc_3_008E:
        ret
loc_3_008F:
        mov    dl, al
        mov    eax, 3
        int    41h
        ret
sub_3_0099:
        sub    ax, ax
        db  080h, 03Dh, 0C7h, 00Fh, 000h, 000h, 001h
        jb     SHORT loc_3_00C5
        ja     SHORT loc_3_00C8
        db  066h, 08Bh, 015h, 0C5h, 00Fh, 000h, 000h
        add    dx, 5
loc_3_00B2:
        in     al, dx
        jmp    SHORT loc_3_00B6
        db  090h
loc_3_00B6:
        and    al, 1
        je     SHORT loc_3_00B2
        db  066h, 08Bh, 015h, 0C5h, 00Fh, 000h, 000h
        in     al, dx
        jmp    SHORT loc_3_00C5
        db  090h
loc_3_00C5:
        and    al, 7fh
        ret
loc_3_00C8:
        mov    dl, al
        mov    eax, 1
        int    41h
        ret
        db  080h, 03Dh, 0C4h, 00Fh, 000h, 000h, 003h, 075h, 010h, 053h, 08Bh, 05Ch, 024h, 008h, 066h, 08Ch
        db  0C9h, 0B8h, 00Fh, 000h, 000h, 000h, 0CDh, 041h, 05Bh, 0C3h
sub_3_00EC:
        push   ebx
        mov    eax, 77h
        mov    bl, 2
        int    41h
        or     ah, ah
        jne    SHORT loc_3_00FE
        pop    ebx
        xor    eax, eax
        ret
loc_3_00FE:
        mov    eax, 78h
        int    41h
        mov    eax, ebx
        pop    ebx
        ret
sub_3_0109:
        push   ebx
        mov    eax, 77h
        mov    bl, 1
        int    41h
        pop    ebx
        or     ah, ah
        mov    ah, 0
        jne    SHORT loc_3_011C
        xor    al, al
loc_3_011C:
        ret
        db  000h, 000h, 000h, 080h, 03Dh, 0C7h, 00Fh, 000h, 000h, 001h, 072h, 061h, 074h, 05Fh, 056h, 057h
        db  08Bh, 074h, 024h, 00Ch, 0BFh, 08Bh, 001h, 000h, 000h, 08Bh, 044h, 024h, 010h, 0ABh, 08Bh, 044h
        db  024h, 014h, 0ABh, 08Bh, 044h, 024h, 018h, 0ABh, 08Bh, 044h, 024h, 01Ch, 0ABh, 08Bh, 044h, 024h
        db  020h, 0ABh, 08Bh, 044h, 024h, 024h, 0ABh, 08Bh, 044h, 024h, 028h, 0ABh, 08Bh, 044h, 024h, 02Ch
        db  0ABh, 08Bh, 044h, 024h, 030h, 0ABh, 08Bh, 044h, 024h, 034h, 0ABh, 08Bh, 044h, 024h, 038h, 0ABh
        db  08Bh, 044h, 024h, 03Ch, 0ABh, 08Bh, 044h, 024h, 040h, 0ABh, 08Bh, 044h, 024h, 044h, 0ABh, 0BFh
        db  08Bh, 001h, 000h, 000h, 0B8h, 073h, 000h, 000h, 000h, 0CDh, 041h, 05Fh, 05Eh, 0C3h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 057h, 08Bh, 07Ch, 024h, 008h, 0CDh, 020h, 013h, 001h, 001h
        db  000h, 08Bh, 0C7h, 05Fh, 0C3h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 056h, 08Bh, 035h, 00Ch, 071h, 000h, 000h, 068h, 038h, 071h, 000h, 000h, 0E8h
        db  04Fh, 032h, 0F8h, 0FFh, 083h, 0C4h, 004h, 00Bh, 0F6h, 074h, 02Eh, 00Fh, 0B7h, 046h, 01Ah, 050h
        db  00Fh, 0B7h, 046h, 018h, 050h, 08Dh, 046h, 004h, 0FFh, 076h, 014h, 0FFh, 076h, 010h, 0FFh, 076h
        db  00Ch, 050h, 0FFh, 036h, 056h, 068h, 090h, 071h, 000h, 000h, 0E8h, 024h, 032h, 0F8h, 0FFh, 083h
        db  0C4h, 024h, 08Bh, 076h, 008h, 00Bh, 0F6h, 075h, 0D2h, 068h, 0BCh, 071h, 000h, 000h, 0E8h, 010h
        db  032h, 0F8h, 0FFh, 083h, 0C4h, 004h, 02Bh, 0C0h, 05Eh, 0C3h, 053h, 056h, 057h, 08Bh, 035h, 00Ch
        db  071h, 000h, 000h, 068h, 0C0h, 071h, 000h, 000h, 0E8h, 0F6h, 031h, 0F8h, 0FFh, 083h, 0C4h, 004h
        db  00Bh, 0F6h, 074h, 040h
loc_3_0251:
        lea    edi, [esi + 1ch]
        db  068h, 0E4h, 071h, 000h, 000h
        db  0E8h, 0E2h, 031h, 0F8h, 0FFh
        add    esp, 4
loc_3_0261:
        db  0B8h, 0E8h, 071h, 000h, 000h
        mov    ebx, dword ptr [edi]
        or     ebx, ebx
        jge    SHORT loc_3_0273
        db  0B8h, 0F0h, 071h, 000h, 000h
        neg    ebx
loc_3_0273:
        push   eax
        push   ebx
        push   edi
        db  068h, 0F8h, 071h, 000h, 000h
        db  0E8h, 0C0h, 031h, 0F8h, 0FFh
        add    edi, ebx
        add    esp, 10h
loc_3_0285:
        cmp    dword ptr [edi], 0
        jne    SHORT loc_3_0261
        mov    esi, dword ptr [esi + 8]
        or     esi, esi
        jne    SHORT loc_3_0251
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
loc_3_02A9:
        db  068h, 0E0h, 075h, 000h, 000h
        db  0E8h, 08Dh, 031h, 0F8h, 0FFh
loc_3_02B3:
        add    esp, 4
        sub    edi, edi
        cmp    dword ptr [esi], edi
        je     SHORT loc_3_02DA
        mov    ebx, esi
loc_3_02BE:
        push   dword ptr [ebx]
        lea    eax, [edi + 61h]
        push   eax
        db  068h, 0E4h, 075h, 000h, 000h
        db  0E8h, 072h, 031h, 0F8h, 0FFh
        add    esp, 0ch
        add    ebx, 8
        inc    edi
        cmp    dword ptr [ebx], 0
        jne    SHORT loc_3_02BE
loc_3_02DA:
        db  068h, 0F4h, 075h, 000h, 000h
        db  0E8h, 05Ch, 031h, 0F8h, 0FFh
        add    esp, 4
        call   sub_3_0099
        mov    bl, al
loc_3_02EE:
        or     bl, bl
        je     SHORT loc_3_033D
        cmp    bl, 3
        je     SHORT loc_3_033D
        cmp    bl, 1bh
        je     SHORT loc_3_033D
        movsx  eax, bl
        push   eax
        call   sub_3_000D
        add    esp, 4
        cmp    bl, 41h
        jl     SHORT loc_3_0315
        cmp    bl, 5ah
loc_3_0310:
        jg     SHORT loc_3_0315
        or     bl, 20h
loc_3_0315:
        cmp    bl, 61h
        jl     SHORT loc_3_02A9
        movsx  eax, bl
        add    edi, 61h
        cmp    eax, edi
        jge    SHORT loc_3_02A9
        db  068h, 010h, 076h, 000h, 000h
        db  0E8h, 012h, 031h, 0F8h, 0FFh
        add    esp, 4
        movsx  eax, bl
        mov    eax, dword ptr [esi + eax*8 - 304h]
        jmp    SHORT loc_3_034C
loc_3_033D:
        db  068h, 00Ch, 076h, 000h, 000h
        db  0E8h, 0F9h, 030h, 0F8h, 0FFh
        add    esp, 4
        sub    eax, eax
loc_3_034C:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  053h, 056h, 02Bh, 0F6h, 080h, 03Dh, 01Ch, 076h, 000h, 000h, 000h, 074h, 020h, 02Bh, 0DBh
loc_3_0360:
        call   sub_3_0109
        cmp    al, bl
        je     NEAR PTR loc_3_03EE
        db  038h, 086h, 01Ch, 076h, 000h, 000h
        jne    SHORT loc_3_03EE
        inc    esi
        db  038h, 09Eh, 01Ch, 076h, 000h, 000h
loc_3_037C:
        jne    SHORT loc_3_0360
        db  0E8h, 086h
IFSMGR_SYS_CRITICAL_INIT:
        std
        db  0FFh, 0FFh, 00Ah, 0C0h, 074h, 04Bh, 03Ch, 041h, 07Ch, 006h, 03Ch, 05Ah, 07Fh, 002h, 00Ch, 020h
        db  02Bh, 0D2h, 039h, 015h, 060h, 075h, 000h, 000h, 074h, 00Eh, 0B9h, 060h, 075h, 000h, 000h, 083h
        db  0C1h, 008h, 042h, 083h, 039h, 000h, 075h, 0F7h, 03Ch, 061h, 07Ch, 025h, 00Fh, 0BEh, 0F0h, 083h
        db  0C2h, 061h, 03Bh, 0D6h, 07Eh, 01Bh, 068h, 024h, 076h, 000h, 000h, 0E8h, 07Fh, 030h, 0F8h, 0FFh
        db  083h, 0C4h, 004h, 06Ah, 000h, 0FFh, 014h, 0F5h, 05Ch, 072h, 000h, 000h, 083h, 0C4h, 004h, 0EBh
        db  01Ch, 068h, 060h, 075h, 000h, 000h, 0E8h, 0C4h, 0FEh, 0FFh, 0FFh, 083h, 0C4h, 004h, 00Bh, 0C0h
        db  074h, 00Bh, 06Ah, 000h, 0FFh, 0D0h, 083h, 0C4h, 004h, 00Bh, 0C0h, 074h, 0E4h
loc_3_03EE:
        pop    esi
        pop    ebx
        ret
        db  068h, 028h, 076h, 000h, 000h, 0E8h, 045h, 030h, 0F8h, 0FFh, 083h, 0C4h, 004h, 02Bh, 0C0h, 0C3h
        db  053h, 056h, 057h, 02Bh, 0FFh, 0BEh, 001h, 000h, 000h, 000h
loc_3_040B:
        db  08Bh, 004h, 0B5h, 0BCh, 065h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_045C
        mov    ebx, dword ptr [eax]
        or     ebx, ebx
        jne    SHORT loc_3_042F
        lea    eax, [esi + 40h]
        push   eax
        db  068h, 060h, 076h, 000h, 000h
        db  0E8h, 016h, 030h, 0F8h, 0FFh
        add    esp, 8
        jmp    SHORT loc_3_045C
loc_3_042F:
        test   byte ptr [ebx + 1bh], 8
        lea    eax, [esi + 40h]
        push   eax
        je     SHORT loc_3_0440
        db  068h, 07Ch, 076h, 000h, 000h
        jmp    SHORT loc_3_0445
loc_3_0440:
        db  068h, 08Ch, 076h, 000h, 000h
loc_3_0445:
        db  0E8h, 0F6h, 02Fh, 0F8h, 0FFh
        add    esp, 8
        db  068h, 09Ch, 076h, 000h, 000h
        push   ebx
IFSMGR_DEVICE_INIT:
        call   sub_3_050B
        add    esp, 8
        inc    edi
loc_3_045C:
        inc    esi
        cmp    esi, 20h
        jle    SHORT loc_3_040B
        or     edi, edi
        jne    SHORT loc_3_0473
        db  068h, 0A4h, 076h, 000h, 000h
        db  0E8h, 0D0h, 02Fh, 0F8h, 0FFh
        add    esp, 4
loc_3_0473:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  056h, 0E8h, 06Dh, 0FCh, 0FFh, 0FFh, 00Bh, 0C0h, 074h, 010h, 068h, 0BCh, 076h, 000h, 000h, 050h
        db  0E8h, 07Dh, 000h, 000h, 000h, 083h, 0C4h, 008h, 0EBh, 074h, 068h, 0C0h, 076h, 000h, 000h, 068h
        db  0DCh, 06Dh, 000h, 000h, 0E8h, 069h, 000h, 000h, 000h, 083h, 0C4h, 008h, 068h, 0C4h, 076h, 000h
        db  000h, 068h, 0F8h, 06Eh, 000h, 000h, 0E8h, 057h, 000h, 000h, 000h, 083h, 0C4h, 008h, 068h, 0C8h
        db  076h, 000h, 000h, 068h, 0A4h, 072h, 000h, 000h, 0E8h, 045h, 000h, 000h, 000h, 083h, 0C4h, 008h
        db  08Bh, 035h, 088h, 06Eh, 000h, 000h, 00Bh, 0F6h, 074h, 015h, 068h, 0CCh, 076h, 000h, 000h, 056h
        db  0E8h, 02Dh, 000h, 000h, 000h, 083h, 0C4h, 008h, 08Bh, 076h, 004h, 00Bh, 0F6h, 075h, 0EBh, 08Bh
        db  035h, 084h, 06Eh, 000h, 000h, 00Bh, 0F6h, 074h, 015h, 068h, 0D0h, 076h, 000h, 000h, 056h, 0E8h
        db  00Eh, 000h, 000h, 000h, 083h, 0C4h, 008h, 08Bh, 076h, 004h, 00Bh, 0F6h, 075h, 0EBh, 02Bh, 0C0h
        db  05Eh, 0C3h
sub_3_050B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        db  068h, 0D4h, 076h, 000h, 000h
        db  0E8h, 01Eh, 02Fh, 0F8h, 0FFh
        add    esp, 8
        mov    esi, dword ptr [ebp + 8]
        db  081h, 0FEh, 0DCh, 06Dh, 000h, 000h
        jne    SHORT loc_3_0537
        db  068h, 0E0h, 076h, 000h, 000h
        jmp    SHORT loc_3_05A8
loc_3_0537:
        db  081h, 0FEh, 0F8h, 06Eh, 000h, 000h
        jne    SHORT loc_3_0546
        db  068h, 0F0h, 076h, 000h, 000h
        jmp    SHORT loc_3_05A8
loc_3_0546:
        db  081h, 0FEh, 0A4h, 072h, 000h, 000h
        jne    SHORT loc_3_0555
        db  068h, 0FCh, 076h, 000h, 000h
        jmp    SHORT loc_3_05A8
loc_3_0555:
        test   byte ptr [esi + 1bh], 8
        je     SHORT loc_3_0568
        cmp    dword ptr [esi + 8], 0
        jne    SHORT loc_3_05B0
        db  068h, 010h, 077h, 000h, 000h
        jmp    SHORT loc_3_05A8
loc_3_0568:
        mov    ebx, 1
        db  0B9h, 0C0h, 065h, 000h, 000h
loc_3_0572:
        mov    eax, dword ptr [ecx]
        or     eax, eax
        je     SHORT loc_3_057C
        cmp    dword ptr [eax], esi
        je     SHORT loc_3_058A
loc_3_057C:
        add    ecx, 4
        inc    ebx
        db  081h, 0F9h, 03Ch, 066h, 000h, 000h
        jbe    SHORT loc_3_0572
        jmp    SHORT loc_3_059E
loc_3_058A:
        push   dword ptr [esi + 8]
        lea    eax, [ebx + 40h]
        push   eax
        db  068h, 028h, 077h, 000h, 000h
        db  0E8h, 0A5h, 02Eh, 0F8h, 0FFh
        add    esp, 0ch
loc_3_059E:
        cmp    ebx, 20h
        jle    SHORT loc_3_05B0
        db  068h, 03Ch, 077h, 000h, 000h
loc_3_05A8:
        db  0E8h, 093h, 02Eh, 0F8h, 0FFh
        add    esp, 4
loc_3_05B0:
        mov    al, byte ptr [esi]
        mov    byte ptr [ebp - 4], al
        mov    ax, word ptr [esi]
        mov    byte ptr [ebp - 3], ah
        mov    byte ptr [ebp - 2], 0
        push   edi
        push   esi
        db  068h, 04Ch, 077h, 000h, 000h
        db  0E8h, 075h, 02Eh, 0F8h, 0FFh
        add    esp, 0ch
        movzx  eax, byte ptr [esi + 3]
        push   eax
        movzx  eax, byte ptr [esi + 2]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        db  068h, 05Ch, 077h, 000h, 000h
        db  0E8h, 05Ah, 02Eh, 0F8h, 0FFh
        add    esp, 10h
        push   dword ptr [esi + 0ch]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        db  068h, 0A0h, 077h, 000h, 000h
        db  0E8h, 044h, 02Eh, 0F8h, 0FFh
        add    esp, 10h
        movzx  eax, word ptr [esi + 16h]
        push   eax
        movzx  eax, word ptr [esi + 14h]
        push   eax
        push   dword ptr [esi + 10h]
        db  068h, 0E0h, 077h, 000h, 000h
        db  0E8h, 02Ah, 02Eh, 0F8h, 0FFh
        add    esp, 10h
        movzx  eax, byte ptr [esi + 1ah]
        push   eax
        movzx  eax, byte ptr [esi + 19h]
        push   eax
        movzx  eax, byte ptr [esi + 18h]
        push   eax
        db  068h, 024h, 078h, 000h, 000h
        db  0E8h, 00Eh, 02Eh, 0F8h, 0FFh
        add    esp, 10h
        push   dword ptr [esi + 20h]
        push   dword ptr [esi + 1ch]
        movzx  eax, byte ptr [esi + 1bh]
        push   eax
        db  068h, 068h, 078h, 000h, 000h
        db  0E8h, 0F6h, 02Dh, 0F8h, 0FFh
        add    esp, 10h
        push   dword ptr [esi + 2ch]
        push   dword ptr [esi + 28h]
        push   dword ptr [esi + 24h]
        db  068h, 0ACh, 078h, 000h, 000h
        db  0E8h, 0E0h, 02Dh, 0F8h, 0FFh
        add    esp, 10h
        movzx  eax, byte ptr [esi + 31h]
        push   eax
        movzx  eax, word ptr [esi + 32h]
        push   eax
        movzx  eax, byte ptr [esi + 30h]
        push   eax
        db  068h, 0F4h, 078h, 000h, 000h
        db  0E8h, 0C4h, 02Dh, 0F8h, 0FFh
        add    esp, 10h
        movzx  eax, word ptr [esi + 3ah]
        push   eax
        push   dword ptr [esi + 34h]
        movzx  eax, word ptr [esi + 38h]
        push   eax
        db  068h, 03Ch, 079h, 000h, 000h
        db  0E8h, 0AAh, 02Dh, 0F8h, 0FFh
        add    esp, 10h
        push   dword ptr [esi + 40h]
        movzx  eax, byte ptr [esi + 3eh]
        push   eax
        movzx  eax, word ptr [esi + 3ch]
        push   eax
        db  068h, 084h, 079h, 000h, 000h
        db  0E8h, 090h, 02Dh, 0F8h, 0FFh
        add    esp, 10h
        db  068h, 0C4h, 079h, 000h, 000h
        db  0E8h, 083h, 02Dh, 0F8h, 0FFh
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  056h, 08Bh, 035h, 088h, 065h, 000h, 000h, 081h, 0FEh, 088h, 065h, 000h, 000h, 074h, 018h, 068h
        db  0C8h, 079h, 000h, 000h, 056h, 0E8h, 011h, 000h, 000h, 000h, 083h, 0C4h, 008h, 08Bh, 036h, 081h
        db  0FEh, 088h, 065h, 000h, 000h, 075h, 0E8h, 02Bh, 0C0h, 05Eh, 0C3h
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, esi
        db  02Bh, 01Dh, 0B4h, 055h, 000h, 000h
        push   ebx
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        db  068h, 0CCh, 079h, 000h, 000h
        db  0E8h, 02Eh, 02Dh, 0F8h, 0FFh
        add    esp, 0ch
        db  039h, 01Dh, 0B0h, 055h, 000h, 000h
        jne    SHORT loc_3_0724
        db  068h, 0D8h, 079h, 000h, 000h
        jmp    SHORT loc_3_0732
loc_3_0724:
        db  0E8h, 0EBh, 007h, 0F8h, 0FFh
        cmp    eax, ebx
        jne    SHORT loc_3_073A
        db  068h, 0E8h, 079h, 000h, 000h
loc_3_0732:
        db  0E8h, 009h, 02Dh, 0F8h, 0FFh
        add    esp, 4
loc_3_073A:
        push   dword ptr [ebx + 0ch]
        push   dword ptr [ebx + 8]
        push   dword ptr [ebx + 4]
        push   dword ptr [ebx]
        push   edi
        db  068h, 0F8h, 079h, 000h, 000h
        db  0E8h, 0F0h, 02Ch, 0F8h, 0FFh
        add    esp, 18h
        push   esi
        push   edi
        db  068h, 034h, 07Ah, 000h, 000h
        db  0E8h, 0E1h, 02Ch, 0F8h, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [esi + 10h]
        or     eax, eax
        je     SHORT loc_3_0779
        push   eax
        db  068h, 048h, 07Ah, 000h, 000h
        db  0E8h, 0CCh, 02Ch, 0F8h, 0FFh
        add    esp, 8
        jmp    SHORT loc_3_0786
loc_3_0779:
        db  068h, 054h, 07Ah, 000h, 000h
        db  0E8h, 0BDh, 02Ch, 0F8h, 0FFh
        add    esp, 4
loc_3_0786:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  08Bh, 035h, 088h, 065h, 000h, 000h
        mov    eax, esi
        db  02Bh, 005h, 0B4h, 055h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        db  081h, 0FEh, 088h, 065h, 000h, 000h
        je     SHORT loc_3_0817
loc_3_07AD:
        mov    eax, dword ptr [ebp - 4]
        test   byte ptr [eax], 8
        je     SHORT loc_3_07BD
        push   eax
        db  068h, 060h, 07Ah, 000h, 000h
        jmp    SHORT loc_3_07C5
loc_3_07BD:
        push   dword ptr [ebp - 4]
        db  068h, 090h, 07Ah, 000h, 000h
loc_3_07C5:
        db  0E8h, 076h, 02Ch, 0F8h, 0FFh
        add    esp, 8
        sub    ebx, ebx
        sub    edi, edi
        db  039h, 01Dh, 0D8h, 066h, 000h, 000h
        jbe    SHORT loc_3_07FC
loc_3_07D9:
        cmp    dword ptr [esi + edi*4 + 0d4h], 0
        je     SHORT loc_3_07F3
        inc    ebx
        db  068h, 0B4h, 07Ah, 000h, 000h
        push   edi
        push   esi
        call   sub_3_0A06
        add    esp, 0ch
loc_3_07F3:
        inc    edi
        db  039h, 03Dh, 0D8h, 066h, 000h, 000h
        ja     SHORT loc_3_07D9
loc_3_07FC:
        or     ebx, ebx
        jne    SHORT loc_3_080D
        db  068h, 0BCh, 07Ah, 000h, 000h
        db  0E8h, 036h, 02Ch, 0F8h, 0FFh
        add    esp, 4
loc_3_080D:
        mov    esi, dword ptr [esi]
        db  081h, 0FEh, 088h, 065h, 000h, 000h
        jne    SHORT loc_3_07AD
loc_3_0817:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_0820:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        db  08Bh, 00Ch, 0BDh, 0E0h, 066h, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        lea    eax, [ecx + eax*8]
        mov    dword ptr [ebp - 4], eax
        mov    esi, dword ptr [eax]
        push   dword ptr [ebp + 10h]
        mov    ebx, dword ptr [esi + 10h]
        test   byte ptr [ebx + 1bh], 8
        je     SHORT loc_3_0851
        db  068h, 0E8h, 07Ah, 000h, 000h
        jmp    SHORT loc_3_0856
loc_3_0851:
        db  068h, 0F0h, 07Ah, 000h, 000h
loc_3_0856:
        db  0E8h, 0E5h, 02Bh, 0F8h, 0FFh
        add    esp, 8
        cmp    dword ptr [esi + 0ch], esi
        jne    SHORT loc_3_0870
        db  068h, 0FCh, 07Ah, 000h, 000h
        db  0E8h, 0D3h, 02Bh, 0F8h, 0FFh
        add    esp, 4
loc_3_0870:
        push   ebx
        push   esi
        shl    edi, 8
        add    edi, dword ptr [ebp + 0ch]
        push   edi
        db  068h, 004h, 07Bh, 000h, 000h
        db  0E8h, 0BDh, 02Bh, 0F8h, 0FFh
        add    esp, 10h
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 4]
        movzx  eax, word ptr [esi + 20h]
        push   eax
        db  068h, 038h, 07Bh, 000h, 000h
        db  0E8h, 0A5h, 02Bh, 0F8h, 0FFh
        add    esp, 0ch
        movzx  eax, word ptr [esi + 22h]
        push   eax
        movzx  eax, byte ptr [esi + 1fh]
        push   eax
        movzx  eax, byte ptr [esi + 1eh]
        push   eax
        movsx  eax, byte ptr [esi + 58h]
        push   eax
        push   dword ptr [esi + 18h]
        push   dword ptr [ebp + 10h]
        db  068h, 054h, 07Bh, 000h, 000h
        db  0E8h, 07Eh, 02Bh, 0F8h, 0FFh
        add    esp, 1ch
        cmp    dword ptr [esi + 0ch], esi
        db  075h, 020h
        db  081h, 07Eh, 004h, 0C0h, 0F6h, 000h, 000h
        push   dword ptr [ebp + 10h]
        jne    SHORT loc_3_08DD
        db  068h, 094h, 07Bh, 000h, 000h
        db  0EBh, 005h
loc_3_08DD:
        db  068h, 0B0h, 07Bh, 000h, 000h
        db  0E8h, 059h, 02Bh, 0F8h
IFSMGR_INIT_COMPLETE:
        db  0FFh, 083h, 0C4h, 008h, 068h, 0CCh
        jnp    SHORT loc_3_08EE
loc_3_08EE:
        add    al, ch
        dec    esp
        sub    edi, eax
        inc    dword ptr [ebx + 5e5f04c4h]
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  068h, 0D0h, 07Bh, 000h, 000h
        db  0E8h, 02Fh, 02Bh, 0F8h, 0FFh
        add    esp, 4
        sub    edi, edi
        db  0B8h, 0E0h, 066h, 000h, 000h
loc_3_091B:
        cmp    dword ptr [eax], 0
        je     SHORT loc_3_092B
        inc    edi
        add    eax, 4
        db  03Dh, 0D8h, 06Ah, 000h, 000h
        jbe    SHORT loc_3_091B
loc_3_092B:
        shl    edi, 8
        db  0FFh, 035h, 0E4h, 06Ah, 000h, 000h
        db  0FFh, 035h, 0E8h, 06Ah, 000h, 000h
        db  0FFh, 035h, 0E0h, 06Ah, 000h, 000h
        push   edi
        db  068h, 0F0h, 07Bh, 000h, 000h
        db  0E8h, 0F5h, 02Ah, 0F8h, 0FFh
        add    esp, 14h
        call   sub_3_00EC
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_3_09A8
        cmp    esi, 200h
        jge    SHORT loc_3_096B
        mov    eax, 1
        jmp    NEAR PTR loc_3_09FF
loc_3_096B:
        sub    esi, 200h
        cmp    esi, edi
        jl     SHORT loc_3_097F
        mov    eax, 1
        jmp    NEAR PTR loc_3_09FF
loc_3_097F:
        mov    eax, esi
        sar    eax, 8
        db  08Bh, 00Ch, 085h, 0E0h, 066h, 000h, 000h
        and    esi, 0ffh
        cmp    dword ptr [ecx + esi*8], 0
        je     SHORT loc_3_09FD
        db  068h, 034h, 07Ch, 000h, 000h
        push   esi
        push   eax
        call   sub_3_0820
        add    esp, 0ch
        jmp    SHORT loc_3_09FD
loc_3_09A8:
        mov    dword ptr [ebp - 4], 0
loc_3_09AF:
        mov    eax, dword ptr [ebp - 4]
        db  08Bh, 01Ch, 085h, 0E0h, 066h, 000h, 000h
        or     ebx, ebx
        je     SHORT loc_3_09EC
        sub    edi, edi
loc_3_09BF:
        cmp    dword ptr [ebx + edi*8], 0
        je     SHORT loc_3_09D7
        db  068h, 03Ch, 07Ch, 000h, 000h
        push   edi
        push   dword ptr [ebp - 4]
        call   sub_3_0820
        add    esp, 0ch
        inc    esi
loc_3_09D7:
        inc    edi
        cmp    edi, 0ffh
        jle    SHORT loc_3_09BF
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 0feh
        jle    SHORT loc_3_09AF
loc_3_09EC:
        or     esi, esi
        jne    SHORT loc_3_09FD
        db  068h, 044h, 07Ch, 000h, 000h
        db  0E8h, 046h, 02Ah, 0F8h, 0FFh
        add    esp, 4
loc_3_09FD:
        sub    eax, eax
loc_3_09FF:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_0A06:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + esi*4 + 0d4h]
        mov    dword ptr [ebp - 4], eax
        mov    ebx, dword ptr [eax + 10h]
        mov    edi, dword ptr [ebp + 8]
        db  02Bh, 03Dh, 0B4h, 055h, 000h, 000h
        test   byte ptr [ebx + 1bh], 8
        push   dword ptr [ebp + 10h]
        je     SHORT loc_3_0A3B
        db  068h, 064h, 07Ch, 000h, 000h
        jmp    SHORT loc_3_0A40
loc_3_0A3B:
        db  068h, 06Ch, 07Ch, 000h, 000h
loc_3_0A40:
        db  0E8h, 0FBh, 029h, 0F8h, 0FFh
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0ch], eax
        jne    SHORT loc_3_0A5D
        db  068h, 078h, 07Ch, 000h, 000h
        db  0E8h, 0E6h, 029h, 0F8h, 0FFh
        add    esp, 4
loc_3_0A5D:
        push   ebx
        push   dword ptr [ebp - 4]
        push   esi
        db  068h, 080h, 07Ch, 000h, 000h
        db  0E8h, 0D4h, 029h, 0F8h, 0FFh
        add    esp, 10h
        test   byte ptr [edi], 8
        jne    SHORT loc_3_0AE6
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4ch]
        mov    eax, dword ptr [eax + esi*4]
        mov    dword ptr [ebp - 8], eax
        push   eax
        push   dword ptr [ebp + 10h]
        db  068h, 0B4h, 07Ch, 000h, 000h
        db  0E8h, 0B2h, 029h, 0F8h, 0FFh
        add    esp, 0ch
        pushad
        mov    ax, 79h
        mov    ecx, 3bh
        mov    esi, dword ptr [ebp - 8]
        int    41h
        movzx  eax, ax
        mov    dword ptr [ebp - 0ch], eax
        popad
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_3_0AD9
        db  068h, 0C4h, 07Ch, 000h, 000h
        db  0E8h, 089h, 029h, 0F8h, 0FFh
        add    esp, 4
        push   dword ptr [ebp - 8]
        call   sub_3_0B3E
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        movzx  eax, word ptr [eax]
        push   eax
        db  068h, 0C8h, 07Ch, 000h, 000h
        db  0E8h, 06Ah, 029h, 0F8h, 0FFh
        add    esp, 8
loc_3_0AD9:
        db  068h, 0DCh, 07Ch, 000h, 000h
        db  0E8h, 05Dh, 029h, 0F8h, 0FFh
        add    esp, 4
loc_3_0AE6:
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 14h]
        movzx  eax, byte ptr [eax + 1eh]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        movsx  eax, byte ptr [eax + 58h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 18h]
        push   dword ptr [ebp + 10h]
        db  068h, 0E0h, 07Ch, 000h, 000h
        db  0E8h, 034h, 029h, 0F8h, 0FFh
        add    esp, 18h
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0ch], eax
        jne    SHORT loc_3_0B37
        db  081h, 078h, 004h, 0C0h, 0F6h, 000h, 000h
        push   dword ptr [ebp + 10h]
        jne    SHORT loc_3_0B2A
        db  068h, 014h, 07Dh, 000h, 000h
        jmp    SHORT loc_3_0B2F
loc_3_0B2A:
        db  068h, 030h, 07Dh, 000h, 000h
loc_3_0B2F:
        db  0E8h, 00Ch, 029h, 0F8h, 0FFh
        add    esp, 8
loc_3_0B37:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_0B3E:
        push   ebp
        mov    ebp, esp
        push   esi
        sub    eax, eax
        sub    esi, esi
        mov    edx, dword ptr [ebp + 8]
loc_3_0B49:
        mov    cl, byte ptr [edx + eax + 20h]
        cmp    cl, 20h
        je     SHORT loc_3_0B5F
        db  088h, 08Eh, 0C8h, 00Fh, 000h, 000h
        inc    esi
        inc    eax
        cmp    eax, 8
        jl     SHORT loc_3_0B49
loc_3_0B5F:
        db  0C6h, 086h, 0C8h, 00Fh, 000h, 000h, 02Eh
        inc    esi
        mov    eax, 8
loc_3_0B6C:
        mov    cl, byte ptr [edx + eax + 20h]
        cmp    cl, 20h
        je     SHORT loc_3_0B82
        db  088h, 08Eh, 0C8h, 00Fh, 000h, 000h
        inc    esi
        inc    eax
        cmp    eax, 0bh
        jl     SHORT loc_3_0B6C
loc_3_0B82:
        cmp    eax, 8
        jne    SHORT loc_3_0B8C
        or     esi, esi
        jle    SHORT loc_3_0B8C
        dec    esi
loc_3_0B8C:
        db  0C6h, 086h, 0C8h, 00Fh, 000h, 000h, 000h
        db  068h, 0C8h, 00Fh, 000h, 000h
        db  068h, 04Ch, 07Dh, 000h, 000h
        db  0E8h, 09Eh, 028h, 0F8h, 0FFh
        add    esp, 8
        pop    esi
        pop    ebp
        ret
        db  056h, 0E8h, 03Eh, 0F5h, 0FFh, 0FFh, 08Bh, 0F0h, 00Bh, 0F6h, 0B8h, 000h, 000h, 000h, 000h, 00Fh
        db  084h, 02Dh, 001h, 000h, 000h, 056h, 068h, 050h, 07Dh, 000h, 000h, 0E8h, 078h, 028h, 0F8h, 0FFh
        db  083h, 0C4h, 008h, 00Fh, 0B6h, 046h, 005h, 050h, 00Fh, 0B6h, 046h, 004h, 050h, 0FFh, 036h, 068h
        db  060h, 07Dh, 000h, 000h, 0E8h, 05Fh, 028h, 0F8h, 0FFh, 083h, 0C4h, 010h, 0FFh, 076h, 00Ch, 0FFh
        db  076h, 008h, 00Fh, 0B7h, 046h, 006h, 050h, 068h, 0A0h, 07Dh, 000h, 000h, 0E8h, 047h, 028h, 0F8h
        db  0FFh, 083h, 0C4h, 010h, 00Fh, 0B7h, 046h, 018h, 050h, 0FFh, 076h, 014h, 0FFh, 076h, 010h, 068h
        db  0E0h, 07Dh, 000h, 000h, 0E8h, 02Fh, 028h, 0F8h, 0FFh, 083h, 0C4h, 010h, 0FFh, 076h, 020h, 0FFh
        db  076h, 01Ch, 00Fh, 0BFh, 046h, 01Ah, 050h, 068h, 024h, 07Eh, 000h, 000h, 0E8h, 017h, 028h, 0F8h
        db  0FFh, 083h, 0C4h, 010h, 0FFh, 076h, 02Ch, 0FFh, 076h, 028h, 0FFh, 076h, 024h, 068h, 060h, 07Eh
        db  000h, 000h, 0E8h, 001h, 028h, 0F8h, 0FFh, 083h, 0C4h, 010h, 0FFh, 076h, 030h, 068h, 0A0h, 07Eh
        db  000h, 000h, 0E8h, 0F1h, 027h, 0F8h, 0FFh, 083h, 0C4h, 008h, 056h, 068h, 0B0h, 07Eh, 000h, 000h
        db  0E8h, 0E3h, 027h, 0F8h, 0FFh, 083h, 0C4h, 008h, 0FFh, 076h, 078h, 0FFh, 076h, 074h, 068h, 0C0h
        db  07Eh, 000h, 000h, 0E8h, 0D0h, 027h, 0F8h, 0FFh, 083h, 0C4h, 00Ch, 0FFh, 0B6h, 084h, 000h, 000h
        db  000h, 0FFh, 0B6h, 080h, 000h, 000h, 000h, 0FFh, 076h, 07Ch, 068h, 0E8h, 07Eh, 000h, 000h, 0E8h
        db  0B4h, 027h, 0F8h, 0FFh, 083h, 0C4h, 010h, 00Fh, 0B6h, 086h, 08Ah, 000h, 000h, 000h, 050h, 00Fh
        db  0B6h, 086h, 089h, 000h, 000h, 000h, 050h, 00Fh, 0B6h, 086h, 088h, 000h, 000h, 000h, 050h, 068h
        db  028h, 07Fh, 000h, 000h, 0E8h, 08Fh, 027h, 0F8h, 0FFh, 083h, 0C4h, 010h, 0FFh, 0B6h, 090h, 000h
        db  000h, 000h, 0FFh, 0B6h, 08Ch, 000h, 000h, 000h, 00Fh, 0B6h, 086h, 08Bh, 000h, 000h, 000h, 050h
        db  068h, 06Ch, 07Fh, 000h, 000h, 0E8h, 06Eh, 027h, 0F8h, 0FFh, 083h, 0C4h, 010h, 0FFh, 0B6h, 094h
        db  000h, 000h, 000h, 068h, 0B0h, 07Fh, 000h, 000h, 0E8h, 05Bh, 027h, 0F8h, 0FFh, 083h, 0C4h, 008h
        db  02Bh, 0C0h, 05Eh, 0C3h, 053h, 056h, 057h, 08Bh, 03Dh, 088h, 065h, 000h, 000h, 0E8h, 0F2h, 0F3h
        db  0FFh, 0FFh, 00Bh, 0C0h, 075h, 007h, 02Bh, 0C0h, 0E9h, 0CDh, 000h, 000h, 000h, 08Bh, 070h, 024h
        db  00Bh, 0F6h, 075h, 014h, 068h, 0C0h, 07Fh, 000h, 000h, 0E8h, 02Ah, 027h, 0F8h, 0FFh, 083h, 0C4h
        db  004h, 02Bh, 0C0h, 0E9h, 0B2h, 000h, 000h, 000h, 00Fh, 0B7h, 04Eh, 030h, 081h, 0F9h, 000h, 002h
        db  000h, 000h, 07Ch, 052h, 066h, 08Bh, 046h, 032h, 066h, 00Bh, 0C0h, 074h, 028h, 00Fh, 0B7h, 0D0h
        db  081h, 0EAh, 000h, 002h, 000h, 000h, 08Bh, 0C2h, 025h, 03Fh, 0FFh, 0FFh, 0FFh, 0C1h, 0F8h, 006h
        db  08Bh, 0DAh, 08Bh, 080h, 0E0h, 066h, 000h, 000h, 081h, 0E3h, 0FFh, 000h, 000h, 000h, 083h, 07Ch
        db  0D8h, 004h, 000h, 075h, 006h, 08Dh, 091h, 000h, 0FEh, 0FFh, 0FFh, 068h, 0E8h, 07Fh, 000h, 000h
        db  08Bh, 0C2h, 025h, 0FFh, 000h, 000h, 000h, 050h, 0C1h, 0FAh, 008h, 052h, 0E8h, 0A7h, 0FAh, 0FFh
        db  0FFh, 083h, 0C4h, 00Ch, 0EBh, 049h, 081h, 0FFh, 088h, 065h, 000h, 000h, 074h, 033h, 08Dh, 004h
        db  08Dh, 000h, 000h, 000h, 000h, 039h, 0B4h, 007h, 0D4h, 000h, 000h, 000h, 074h, 00Ch, 08Bh, 03Fh
        db  081h, 0FFh, 088h, 065h, 000h, 000h, 075h, 0EDh, 0EBh, 00Fh, 068h, 0F0h, 07Fh, 000h, 000h, 051h
        db  057h, 0E8h, 058h, 0FCh, 0FFh, 0FFh, 083h, 0C4h, 00Ch, 081h, 0FFh, 088h, 065h, 000h, 000h, 075h
        db  00Eh, 056h, 068h, 0F8h, 07Fh, 000h, 000h, 0E8h, 07Ch, 026h, 0F8h, 0FFh, 083h, 0C4h, 008h, 08Bh
        db  076h, 02Ch, 00Bh, 0F6h, 00Fh, 085h, 04Eh, 0FFh, 0FFh, 0FFh, 05Fh, 05Eh, 05Bh, 0C3h
        push   ebp
        mov    ebp, esp
        sub    esp, 104h
        push   esi
        call   sub_3_00EC
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_3_0DF2
        sub    eax, eax
        jmp    NEAR PTR loc_3_0F2B
loc_3_0DF2:
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   dword ptr [esi]
        db  068h, 028h, 080h, 000h, 000h
        db  0E8h, 03Ch, 026h, 0F8h, 0FFh
        add    esp, 10h
        push   dword ptr [esi + 14h]
        push   dword ptr [esi + 10h]
        push   dword ptr [esi + 0ch]
        db  068h, 064h, 080h, 000h, 000h
        db  0E8h, 026h, 026h, 0F8h, 0FFh
        add    esp, 10h
        movzx  eax, byte ptr [esi + 1eh]
        push   eax
        movzx  eax, word ptr [esi + 1ch]
        push   eax
        push   dword ptr [esi + 18h]
        db  068h, 0A0h, 080h, 000h, 000h
        db  0E8h, 00Ch, 026h, 0F8h, 0FFh
        add    esp, 10h
        movzx  eax, word ptr [esi + 22h]
        push   eax
        movzx  eax, word ptr [esi + 20h]
        push   eax
        movzx  eax, byte ptr [esi + 1fh]
        push   eax
        db  068h, 0E0h, 080h, 000h, 000h
        db  0E8h, 0F0h, 025h, 0F8h, 0FFh
        add    esp, 10h
        push   dword ptr [esi + 2ch]
        push   dword ptr [esi + 28h]
        push   dword ptr [esi + 24h]
        db  068h, 020h, 081h, 000h, 000h
        db  0E8h, 0DAh, 025h, 0F8h, 0FFh
        add    esp, 10h
        push   dword ptr [esi + 34h]
        movzx  eax, word ptr [esi + 32h]
        push   eax
        movzx  eax, word ptr [esi + 30h]
        push   eax
        db  068h, 05Ch, 081h, 000h, 000h
        db  0E8h, 0C0h, 025h, 0F8h, 0FFh
        add    esp, 10h
        movzx  eax, word ptr [esi + 3eh]
        push   eax
        movzx  eax, word ptr [esi + 3ch]
        push   eax
        push   dword ptr [esi + 38h]
        db  068h, 098h, 081h, 000h, 000h
        db  0E8h, 0A6h, 025h, 0F8h, 0FFh
        add    esp, 10h
        mov    esi, dword ptr [esi + 24h]
        or     esi, esi
        je     NEAR PTR loc_3_0F2B
        db  068h, 0DCh, 081h, 000h, 000h
        db  0E8h, 08Eh, 025h, 0F8h, 0FFh
        add    esp, 4
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   dword ptr [esi]
        db  068h, 0F8h, 081h, 000h, 000h
        db  0E8h, 079h, 025h, 0F8h, 0FFh
        add    esp, 10h
        movzx  eax, word ptr [esi + 10h]
        push   eax
        push   dword ptr [esi + 0ch]
        db  068h, 034h, 082h, 000h, 000h
        db  0E8h, 064h, 025h, 0F8h, 0FFh
        add    esp, 0ch
        cmp    word ptr [esi + 14h], 0
        je     SHORT loc_3_0F1E
        push   1
        push   103h
        add    esi, 18h
        push   esi
        lea    eax, [ebp - 104h]
        push   eax
        db  0E8h, 029h, 01Bh, 0F9h, 0FFh
        add    esp, 10h
        mov    byte ptr [ebp + eax - 104h], 0
        lea    eax, [ebp - 104h]
        push   eax
        db  068h, 060h, 082h, 000h, 000h
        db  0E8h, 027h, 025h, 0F8h, 0FFh
        add    esp, 8
        jmp    SHORT loc_3_0F2B
loc_3_0F1E:
        db  068h, 074h, 082h, 000h, 000h
        db  0E8h, 018h, 025h, 0F8h, 0FFh
        add    esp, 4
loc_3_0F2B:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  068h, 088h, 082h, 000h, 000h, 0E8h, 006h, 025h, 0F8h, 0FFh, 083h, 0C4h, 004h, 0C3h, 056h, 057h
        db  0E8h, 036h, 027h, 0F8h, 0FFh, 08Bh, 0F8h, 068h, 0B8h, 082h, 000h, 000h, 0E8h, 0EFh, 024h, 0F8h
        db  0FFh, 083h, 0C4h, 004h, 068h, 0CCh, 082h, 000h, 000h, 0E8h, 0E2h, 024h, 0F8h, 0FFh, 083h, 0C4h
        db  004h, 08Bh, 0F7h, 0A1h, 0BCh, 055h, 000h, 000h, 08Bh, 00Ch, 006h, 00Bh, 0C9h, 074h, 041h, 08Bh
        db  0C1h, 025h, 000h, 000h, 000h, 020h, 083h, 0F8h, 001h, 01Bh, 0C0h, 040h, 050h, 08Bh, 0C1h, 025h
        db  000h, 000h, 000h, 040h, 083h, 0F8h, 001h, 01Bh, 0C0h, 040h, 050h, 08Bh, 0C1h, 025h, 000h, 000h
        db  000h, 080h, 083h, 0F8h, 001h, 01Bh, 0C0h, 040h, 050h, 00Fh, 0B7h, 0C1h, 050h, 00Fh, 0B7h, 046h
        db  018h, 050h, 056h, 068h, 0E0h, 082h, 000h, 000h, 0E8h, 093h, 024h, 0F8h, 0FFh, 083h, 0C4h, 01Ch
        db  056h, 0E8h, 00Dh, 0F2h, 0FFh, 0FFh, 083h, 0C4h, 004h, 08Bh, 0F0h, 03Bh, 0FEh, 075h, 0A4h, 05Fh
        db  05Eh, 0C3h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 016h, 000h, 021h, 000h, 041h, 020h, 020h, 020h, 020h, 020h
        db  020h, 020h, 000h, 000h, 000h, 000h, 02Eh, 089h, 01Eh, 012h, 000h, 02Eh, 08Ch, 006h, 014h, 000h
        db  0CBh, 09Ch, 006h, 053h, 02Eh, 0C4h, 01Eh, 012h, 000h, 0B8h, 001h, 000h, 00Dh, 000h, 081h, 026h
        db  089h, 047h, 003h, 05Bh, 007h, 09Dh, 0CBh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
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
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h
IFSMgr_InitUseAdd:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        push   ebp
        dec    esi
        dec    ecx
        inc    ebx
        dec    edi
        inc    esp
        inc    ebp
        inc    edx
        dec    ecx
        dec    esi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_2090:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_2230:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 79h], dl
        jae    SHORT loc_3_22FD
        insd
        pop    esp
        inc    ebx
        jne    SHORT loc_3_2301
        db  072h, 065h
        outsb
        je     SHORT loc_3_22D7
        outsd
        outsb
        je     SHORT loc_3_230A
        outsd
        insb
        push   ebx
        db  065h, 074h, 05Ch
        inc    ebx
        outsd
        outsb
        je     SHORT loc_3_2315
        outsd
        insb
        pop    esp
        dec    esi
        insb
        db  073h, 05Ch
        inc    ebx
        outsd
        db  064h, 065h, 070h, 061h
        add    byte ptr gs:[bx + 45h], cl
        dec    ebp
        inc    ebx
        push   eax
        add    byte ptr [ecx + 43h], al
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   ebx
        jns    SHORT loc_3_2338
        je     SHORT loc_3_232C
        insd
        pop    esp
        inc    ebx
        jne    SHORT loc_3_233E
        db  072h, 065h
        outsb
        db  074h, 043h
        outsd
        outsb
        db  074h, 072h
        outsd
        insb
loc_3_22D7:
        push   ebx
        je     NEAR PTR loc_3_2337
        inc    ebx
        outsd
        outsb
        je     SHORT loc_3_2352
        outsd
        insb
        pop    esp
        inc    esi
        imul   ebp, dword ptr [ebp + 53h], 65747379h
        insd
        add    byte ptr [eax + 72h], dl
        jae    NEAR PTR loc_3_2358
        db  072h, 076h
        dec    esp
        outsd
        outsb
        dec    esi
        popad
        insd
loc_3_22FD:
        jae    NEAR PTR loc_3_2300
loc_3_2300:
        push   edi
loc_3_2301:
        imul   ebp, dword ptr [esi + 33h], 6c694631h
        push   ebx
loc_3_230A:
        jns    SHORT loc_3_237F
        je     SHORT loc_3_2373
        insd
        add    byte ptr [ebx + 6fh], dl
        je     NEAR PTR loc_3_2358
loc_3_2315:
        outsd
        insd
        db  070h, 061h
        je     SHORT loc_3_2368
        outsd
        add    byte ptr gs:[ecx + 73h], al
        db  079h, 06Eh
        arpl   word ptr [esi + 69h], ax
        insb
        inc    ebx
        outsd
        insd
        insd
loc_3_232C:
        imul   esi, dword ptr [eax + eax + 4eh], 61777465h
        db  072h, 065h
        push   eax
loc_3_2337:
        popad
loc_3_2338:
        db  074h, 068h
        push   ebx
        insd
        popad
loc_3_233E:
        outsb
        db  074h, 069h
        arpl   word ptr [ebx], si
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        out    dx, al
        add    al, byte ptr [eax]
        db  000h, 040h, 000h
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
loc_3_2352:
        add    byte ptr [eax], al
        adc    byte ptr [ebx], al
        add    byte ptr [eax], al
loc_3_2358:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        pop    ds
        add    eax, dword ptr [eax]
        db  000h, 043h, 000h
        add    byte ptr [eax], al
loc_3_2368:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        das
        add    eax, dword ptr [eax]
        db  000h, 044h, 000h, 000h
loc_3_2373:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_237F:
        db  000h, 089h, 01Dh, 0B0h, 055h, 000h
        add    byte ptr [ebp - 39f8862eh], al
        db  005h, 0E1h, 055h, 000h, 000h
        add    byte ptr [edi], cl
        mov    bh, 0d2h
        db  089h, 015h, 0D0h, 066h, 000h, 000h
        db  0C7h, 005h, 0ACh, 083h, 000h, 000h, 080h, 059h, 000h, 000h
        db  0C7h, 005h, 0CCh, 06Ch, 000h, 000h, 084h, 05Ah, 000h, 000h
        db  0CDh, 020h, 0B9h, 000h, 001h, 000h
        shl    eax, 4
        db  0A3h, 0B8h, 055h, 000h, 000h
        db  0CDh, 020h, 0BBh, 000h, 001h, 000h
        cmp    ah, 3
        ja     SHORT loc_3_23E5
        db  0C7h, 005h, 0A0h, 065h, 000h, 000h, 036h, 005h, 000h, 000h
        db  0C7h, 005h, 0A4h, 065h, 000h, 000h, 0E8h, 002h, 000h, 000h
        db  0C7h, 005h, 0A8h, 065h, 000h, 000h, 006h, 005h, 000h, 000h
loc_3_23E5:
        db  0F7h, 005h, 0EBh, 003h, 000h, 000h, 000h, 000h, 040h, 000h
        je     SHORT loc_3_23FF
        db  0C6h, 005h, 0C7h, 00Fh, 000h, 000h, 002h
        db  0C6h, 005h, 0C4h, 00Fh, 000h, 000h, 003h
loc_3_23FF:
        db  0E8h, 0ECh, 014h, 000h, 000h
        test   eax, eax
        jne    SHORT loc_3_2427
        db  0E8h, 05Fh, 012h, 000h, 000h
        test   eax, eax
        jne    SHORT loc_3_242D
        push   eax
        push   eax
        db  0CDh, 020h, 001h, 001h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_3_2427
        db  0A3h, 0F0h, 055h, 000h, 000h
        clc
        ret
loc_3_2427:
        db  0CDh, 020h, 0BFh, 080h, 001h, 000h
loc_3_242D:
        mov    esi, eax
        sub    eax, eax
        db  0CDh, 020h, 0BEh, 080h, 001h, 000h
        push   esi
        push   edx
        inc    ebp
        inc    esp
        dec    ecx
        push   edx
        add    byte ptr [ebx], ch
        rcl    byte ptr [eax + 50h], 0cdh
        and    byte ptr [ecx], al
        add    dword ptr [ecx], eax
        db  000h, 083h, 0C4h, 008h, 0A3h, 090h
        add    dword ptr [eax], 0
        or     eax, eax
        ret
        db  0E8h, 0E6h, 0FFh, 0FFh, 0FFh, 00Fh, 084h, 082h, 004h, 000h, 000h, 0E8h, 07Ch, 00Ah, 000h, 000h
        db  0E8h, 06Ch, 00Fh, 000h, 000h, 00Bh, 0C0h, 074h, 01Ch, 0A1h, 000h, 000h, 000h, 000h, 0BFh, 010h
        db  003h, 000h, 000h, 056h, 0BEh, 0D0h, 055h, 000h, 000h, 0CDh, 020h, 0B1h, 000h, 001h, 000h, 05Eh
        db  0A3h, 000h, 000h, 000h, 000h, 083h, 0ECh, 06Ch, 057h, 08Dh, 07Ch, 024h, 004h, 0CDh, 020h, 08Dh
        db  000h, 001h, 000h, 05Fh, 0CDh, 020h, 082h, 000h, 001h, 000h, 066h, 0C7h, 045h, 01Ch, 000h, 0B8h
        db  0B8h, 02Fh, 000h, 000h, 000h, 0CDh, 020h, 084h, 000h, 001h, 000h, 080h, 07Dh, 01Ch, 000h, 074h
        db  04Fh, 08Ah, 04Dh, 010h, 0F6h, 0C1h, 008h, 074h, 047h, 066h, 0C7h, 045h, 01Ch, 09Ah, 05Fh, 066h
        db  083h, 065h, 02Ch, 0FEh, 0B8h, 021h, 000h, 000h, 000h, 0CDh, 020h, 084h, 000h, 001h, 000h, 066h
        db  0F7h, 045h, 02Ch, 001h, 000h, 075h, 029h, 0C6h, 005h, 030h, 059h, 000h, 000h, 001h, 0BEh, 03Ah
        db  014h, 000h, 000h, 0CDh, 020h, 00Bh, 000h, 001h, 000h, 00Fh, 082h, 0EEh, 003h, 000h, 000h, 0A3h
        db  05Eh, 000h, 000h, 000h, 0F6h, 0C1h, 040h, 074h, 007h, 0C6h, 005h, 031h, 059h, 000h, 000h, 001h
        db  0C6h, 045h, 01Dh, 034h, 0B8h, 021h, 000h, 000h, 000h, 0CDh, 020h, 084h, 000h, 001h, 000h, 00Fh
        db  0B7h, 04Dh, 038h, 00Fh, 0B7h, 045h, 010h, 048h, 0C1h, 0E1h, 004h, 003h, 0C1h, 0A3h, 09Ch, 065h
        db  000h, 000h, 001h, 00Dh, 0A0h, 065h, 000h, 000h, 001h, 00Dh, 0A4h, 065h, 000h, 000h, 001h, 00Dh
        db  0A8h, 065h, 000h, 000h, 0C6h, 045h, 01Dh, 052h, 0B8h, 021h, 000h, 000h, 000h, 0CDh, 020h, 084h
        db  000h, 001h, 000h, 00Fh, 0B7h, 04Dh, 038h, 00Fh, 0B7h, 045h, 010h, 0C1h, 0E1h, 004h, 003h, 0C1h
        db  0A3h, 054h, 059h, 000h, 000h, 00Fh, 0B7h, 048h, 01Ah, 066h, 083h, 0F9h, 0FFh, 074h, 012h, 00Fh
        db  0B7h, 040h, 01Ch, 0C1h, 0E0h, 004h, 003h, 0C1h, 00Fh, 0B7h, 040h, 004h, 0A3h, 0D4h, 066h, 000h
        db  000h, 066h, 0C7h, 045h, 01Ch, 007h, 016h, 066h, 0C7h, 045h, 010h, 040h, 000h, 066h, 0C7h, 045h
        db  018h, 003h, 000h, 066h, 0C7h, 045h, 014h, 001h, 000h, 0B8h, 02Fh, 000h, 000h, 000h, 0CDh, 020h
        db  084h, 000h, 001h, 000h, 066h, 081h, 07Dh, 01Ch, 07Ch, 0B9h, 075h, 00Eh, 066h, 081h, 07Dh, 014h
        db  0ABh, 0A2h, 075h, 006h, 00Fh, 0B7h, 04Dh, 018h, 0EBh, 015h, 053h, 0CDh, 020h, 0BBh, 000h, 001h
        db  000h, 05Bh, 0B9h, 058h, 000h, 000h, 000h, 080h, 0FCh, 003h, 077h, 003h, 083h, 0E9h, 007h, 089h
        db  00Dh, 0C8h, 066h, 000h, 000h, 0C6h, 045h, 01Dh, 019h, 0B8h, 021h, 000h, 000h, 000h, 0CDh, 020h
        db  084h, 000h, 001h, 000h, 08Ah, 045h, 01Ch, 0A2h, 040h, 02Dh, 000h, 000h, 066h, 0C7h, 045h, 01Ch
        db  000h, 0EAh, 066h, 0C7h, 045h, 010h, 000h, 000h, 0B8h, 021h, 000h, 000h, 000h, 0CDh, 020h, 084h
        db  000h, 001h, 000h, 08Ah, 045h, 011h, 00Ah, 045h, 010h, 074h, 028h, 066h, 0C7h, 045h, 01Ch, 020h
        db  07Ah, 066h, 0C7h, 045h, 010h, 000h, 000h, 0B8h, 02Fh, 000h, 000h, 000h, 0CDh, 020h, 084h, 000h
        db  001h, 000h, 0B0h, 001h, 066h, 083h, 07Dh, 01Ch, 000h, 00Fh, 094h, 0C4h, 002h, 0C4h, 0A2h, 0CDh
        db  061h, 000h, 000h, 066h, 0C7h, 045h, 01Ch, 000h, 033h, 0B8h, 021h, 000h, 000h, 000h, 0CDh, 020h
        db  084h, 000h, 001h, 000h, 08Ah, 04Dh, 014h, 088h, 00Dh, 042h, 000h, 000h, 000h, 0CDh, 020h, 086h
        db  000h, 001h, 000h, 056h, 08Dh, 074h, 024h, 004h, 0CDh, 020h, 08Eh, 000h, 001h, 000h, 05Eh, 083h
        db  0C4h, 06Ch, 053h, 0CDh, 020h, 0BBh, 000h, 001h, 000h, 05Bh, 0B9h, 035h, 000h, 000h, 000h, 080h
        db  0FCh, 003h, 076h, 005h, 0B9h, 03Bh, 000h, 000h, 000h, 089h, 00Dh, 0CCh, 066h, 000h, 000h, 0E8h
        db  053h, 007h, 000h, 000h, 053h, 0CDh, 020h, 000h, 000h, 08Bh, 004h, 072h, 005h, 0E8h, 0DCh, 05Eh
        db  0FEh, 0FFh, 05Bh, 08Bh, 00Dh, 054h, 059h, 000h, 000h, 08Dh, 049h, 022h, 089h, 00Dh, 038h, 059h
        db  000h, 000h, 08Bh, 00Dh, 0D4h, 066h, 000h, 000h, 081h, 0C1h, 000h, 001h, 000h, 000h, 089h, 00Dh
        db  0D8h, 066h, 000h, 000h, 0A1h, 0DCh, 06Ch, 000h, 000h, 0BAh, 09Bh, 002h, 000h, 000h, 0F7h, 0E2h
        db  0A3h, 0DCh, 06Ch, 000h, 000h, 06Ah, 000h, 0E8h, 003h, 0FAh, 0FCh, 0FFh, 083h, 0C4h, 004h, 00Fh
        db  0B7h, 0D0h, 0C1h, 0E8h, 010h, 066h, 0A3h, 048h, 059h, 000h, 000h, 0C1h, 0E0h, 004h, 0A3h, 0ACh
        db  065h, 000h, 000h, 003h, 0C2h, 0A3h, 044h, 059h, 000h, 000h, 081h, 0C2h, 0B2h, 001h, 000h, 000h
        db  066h, 089h, 015h, 04Ah, 059h, 000h, 000h, 00Fh, 0B7h, 048h, 002h, 0C1h, 0E1h, 004h, 089h, 00Dh
        db  0B0h, 065h, 000h, 000h, 080h, 078h, 012h, 008h, 00Fh, 085h, 0CAh, 001h, 000h, 000h, 06Ah, 000h
        db  0A1h, 0D8h, 066h, 000h, 000h, 08Dh, 004h, 0C5h, 0D4h, 000h, 000h, 000h, 050h, 0CDh, 020h, 0A7h
        db  000h, 001h, 000h, 083h, 0C4h, 008h, 085h, 0C0h, 00Fh, 084h, 0BFh, 001h, 000h, 000h, 0A3h, 0B4h
        db  055h, 000h, 000h, 080h, 064h, 003h, 008h, 0DFh, 08Ah, 00Dh, 042h, 000h, 000h, 000h, 00Ah, 0C9h
        db  074h, 005h, 080h, 04Ch, 003h, 008h, 020h, 0B8h, 033h, 001h, 000h, 000h, 068h, 004h, 001h, 000h
        db  000h, 050h, 0CDh, 020h, 0A8h, 000h, 001h, 000h, 083h, 0C4h, 008h, 00Bh, 0C0h, 00Fh, 084h, 08Ah
        db  001h, 000h, 000h, 0A3h, 0C4h, 061h, 000h, 000h, 08Dh, 035h, 000h, 000h, 000h, 000h, 0C6h, 046h
        db  00Ah, 001h, 08Dh, 005h, 037h, 000h, 000h, 000h, 02Bh, 0C6h, 050h, 06Ah, 004h, 050h, 0CDh, 020h
        db  0A8h, 000h, 001h, 000h, 083h, 0C4h, 008h, 059h, 00Bh, 0C0h, 00Fh, 084h, 05Dh, 001h, 000h, 000h
        db  08Bh, 0F8h, 0C1h, 0E8h, 004h, 066h, 0C7h, 005h, 03Eh, 000h, 000h, 000h, 000h, 000h, 066h, 0A3h
        db  03Ch, 000h, 000h, 000h, 0FCh, 0F3h, 0A4h, 06Ah, 000h, 06Ah, 000h, 068h, 092h, 000h, 000h, 000h
        db  068h, 033h, 001h, 000h, 000h, 050h, 0CDh, 020h, 07Ah, 000h, 001h, 000h, 083h, 0C4h, 014h, 06Ah
        db  000h, 050h, 052h, 0CDh, 020h
loc_3_27B8:
        jbe    SHORT loc_3_27BA
loc_3_27BA:
        add    dword ptr [eax], eax
        add    esp, 0ch
        mov    ecx, eax
        or     ecx, edx
        je     NEAR PTR loc_3_28E0
        db  0A3h, 0C8h, 061h, 000h, 000h
        db  0CDh, 020h, 0FFh, 000h, 001h, 000h
        or     eax, eax
        je     NEAR PTR loc_3_28E0
        db  0A3h, 0BCh, 055h, 000h, 000h
        mov    esi, eax
        db  0CDh, 020h, 00Dh, 001h, 001h, 000h
        mov    edx, edi
loc_3_27EB:
        mov    dword ptr [esi + edi], 0
        db  0CDh, 020h, 013h, 001h, 001h, 000h
        cmp    edi, edx
        jne    SHORT loc_3_27EB
        push   ebx
        db  0E8h, 075h, 033h, 0FDh, 0FFh
        pop    ecx
        test   eax, eax
        jne    NEAR PTR loc_3_28E0
        db  003h, 00Dh, 0B4h, 055h, 000h, 000h
        db  0A0h, 040h, 02Dh, 000h, 000h
        mov    byte ptr [ecx + 0ah], al
        call   sub_3_2B3E
        db  0E8h, 051h, 00Bh, 000h, 000h
        mov    eax, 204e5250h
        call   sub_3_2B0F
        db  0A3h, 008h, 070h, 000h, 000h
        mov    eax, 3154504ch
        call   sub_3_2B0F
        db  0A3h, 0E4h, 06Fh, 000h, 000h
        mov    eax, 3254504ch
        call   sub_3_2B0F
        db  0A3h, 0E8h, 06Fh, 000h, 000h
        mov    eax, 3354504ch
        call   sub_3_2B0F
        db  0A3h, 0ECh, 06Fh, 000h, 000h
        push   ebx
        db  0E8h, 06Bh, 00Eh, 000h, 000h
        pop    ecx
        db  0BEh, 045h, 008h, 000h, 000h
        db  0CDh, 020h, 00Bh, 000h, 001h, 000h
        db  0A3h, 040h, 059h, 000h, 000h
        push   ebx
        sub    esp, 8
        mov    eax, esp
        mov    byte ptr [eax], 5
        push   eax
        db  0CDh, 020h, 008h, 000h, 010h, 000h
        add    esp, 4
        add    esp, 8
        db  0E8h, 061h, 007h, 0FDh, 0FFh
        db  0E8h, 0CEh, 00Ch, 000h, 000h
        db  0E8h, 0B5h, 00Ch, 000h, 000h
        db  0E8h, 019h, 00Ch, 000h, 000h
        db  0E8h, 003h, 00Ch, 000h, 000h
        db  0E8h, 056h, 00Dh, 000h, 000h
        pop    ebx
        test   eax, eax
        jne    SHORT loc_3_28B6
        call   sub_3_2A54
        clc
        ret
loc_3_28B6:
        db  00Fh, 0B7h, 035h, 016h, 000h, 000h, 000h
        db  003h, 035h, 004h, 000h, 000h, 000h
        sub    eax, eax
        db  0CDh, 020h, 0BEh, 080h, 001h, 000h
        db  00Fh, 0B7h, 035h, 014h, 000h, 000h, 000h
        db  003h, 035h, 004h, 000h, 000h, 000h
        sub    eax, eax
        db  0CDh, 020h, 0BEh, 080h, 001h, 000h
loc_3_28E0:
        db  0CDh, 020h, 0BFh, 080h, 001h, 000h
        push   ebx
        db  0CDh, 020h, 000h, 000h, 021h, 000h
        jb     SHORT loc_3_28FB
        cmp    bl, 2
        jne    SHORT loc_3_28FB
        db  0C6h, 005h, 098h, 065h, 000h, 000h, 001h
loc_3_28FB:
        pop    ebx
        db  0E8h, 0EFh, 05Ah, 0FEh, 0FFh
        db  080h, 03Dh, 0E1h, 055h, 000h, 000h, 000h
        je     SHORT loc_3_2921
        db  080h, 03Dh, 029h, 059h, 000h, 000h, 000h
        je     SHORT loc_3_2921
        db  0A1h, 0B0h, 065h, 000h, 000h
        or     word ptr [eax + 611h], 800h
loc_3_2921:
        mov    eax, 2a0015h
        db  0BEh, 0C5h, 007h, 000h, 000h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        db  089h, 035h, 0E4h, 055h, 000h, 000h
        pushfd
        cli
        mov    eax, 1000eh
        db  0BEh, 0D8h, 007h, 000h, 000h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        db  089h, 035h, 0E8h, 055h, 000h, 000h
        popfd
        mov    eax, 10085h
        db  0BEh, 0E5h, 007h, 000h, 000h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        db  089h, 035h, 0ECh, 055h, 000h, 000h
        db  0C6h, 005h, 041h, 000h, 000h, 000h, 001h
        mov    eax, 13h
        db  0CDh, 020h, 041h, 001h, 001h, 000h
        db  0A3h, 0DDh, 055h, 000h, 000h
        db  0BEh, 09Dh, 002h, 000h, 000h
        mov    eax, 1002bh
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        db  089h, 035h, 0A0h, 061h, 000h, 000h
        db  0BEh, 037h, 003h, 000h, 000h
        mov    eax, 1002ch
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        db  089h, 035h, 0A4h, 061h, 000h, 000h
        db  0BEh, 02Fh, 003h, 000h, 000h
        mov    eax, 1002dh
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        db  089h, 035h, 0A8h, 061h, 000h, 000h
        db  0BEh, 01Ah, 002h, 000h, 000h
        mov    eax, 1002eh
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        db  089h, 035h, 0ACh, 061h, 000h, 000h
        db  0BEh, 076h, 002h, 000h, 000h
        mov    eax, 100ech
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        db  089h, 035h, 0B0h, 061h, 000h, 000h
        db  0BEh, 053h, 002h, 000h, 000h
        mov    eax, 1002fh
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        db  089h, 035h, 0B4h, 061h, 000h, 000h
        db  00Fh, 0B7h, 005h, 01Ah, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   0
        push   28h
        push   eax
        db  0CDh, 020h, 026h, 000h, 040h, 000h
        add    esp, 0ch
        db  00Fh, 0B7h, 005h, 01Ch, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   0
        push   29h
        push   eax
        db  0CDh, 020h, 026h, 000h, 040h, 000h
        add    esp, 0ch
        db  00Fh, 0B7h, 005h, 01Eh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   0
        push   2ah
        push   eax
        db  0CDh, 020h, 026h, 000h, 040h, 000h
        add    esp, 0ch
        clc
        ret
sub_3_2A54:
        db  0A1h, 0B4h, 055h, 000h, 000h
        db  0C7h, 044h, 003h, 00Ch, 0E1h, 008h, 000h, 000h
        db  0C7h, 005h, 094h, 065h, 000h, 000h, 0E1h, 008h, 000h, 000h
        push   ebx
        sub    esp, 104h
        mov    edi, esp
        push   edi
        db  0E8h, 0D7h, 0FCh, 0FCh, 0FFh
        add    esp, 4
        push   edi
        db  0E8h, 00Dh, 00Dh, 000h, 000h
        pop    ecx
        add    esp, 104h
        pop    ebx
        db  0A1h, 044h, 059h, 000h, 000h
        movzx  ecx, word ptr [eax + 0ch]
        movzx  eax, word ptr [eax + 0eh]
        shl    eax, 4
        add    eax, ecx
        lea    eax, [eax + 4]
loc_3_2AA0:
        movzx  ecx, word ptr [eax]
        movzx  eax, word ptr [eax + 2]
        shl    eax, 4
        add    eax, ecx
        cmp    word ptr [eax], -1
        jne    SHORT loc_3_2AA0
        db  0A3h, 090h, 065h, 000h, 000h
        db  0A1h, 0B4h, 055h, 000h, 000h
        db  0C7h, 044h, 003h, 00Ch, 010h, 008h, 000h, 000h
        db  08Bh, 015h, 0D0h, 066h, 000h, 000h
        db  0A1h, 044h, 059h, 000h, 000h
        lea    eax, [edx + eax + 2eh]
        db  0A3h, 060h, 059h, 000h, 000h
        mov    eax, 19ah
        sub    eax, edx
        db  0A3h, 064h, 059h, 000h, 000h
        push   0
        db  068h, 058h, 059h, 000h, 000h
        db  0CDh, 020h, 0A6h, 000h, 001h, 000h
        add    esp, 8
        test   eax, eax
        je     SHORT loc_3_2B09
        db  0E8h, 0A2h, 00Dh, 000h, 000h
        db  0BEh, 034h, 012h, 000h, 000h
        db  0CDh, 020h, 03Ah, 000h, 001h, 000h
        ret
loc_3_2B09:
        db  0CDh, 020h, 0BFh, 080h, 001h, 000h
sub_3_2B0F:
        db  08Bh, 00Dh, 038h, 059h, 000h, 000h
loc_3_2B15:
        mov    edx, dword ptr [ecx]
        cmp    dx, -1
        je     SHORT loc_3_2B3B
        mov    esi, edx
        movzx  ecx, dx
        shr    edx, 0ch
        and    dl, 0f0h
        add    ecx, edx
        cmp    dword ptr [ecx + 0ah], eax
        jne    SHORT loc_3_2B15
        cmp    dword ptr [ecx + 0eh], 20202020h
        jne    SHORT loc_3_2B15
        mov    eax, esi
        ret
loc_3_2B3B:
        sub    eax, eax
        ret
sub_3_2B3E:
        push   ebx
        db  08Bh, 01Dh, 044h, 059h, 000h, 000h
        movzx  eax, word ptr [ebx + 0ch]
        movzx  ebx, word ptr [ebx + 0eh]
        shl    ebx, 4
        add    ebx, eax
        movzx  ecx, byte ptr [ebx + 21h]
        db  089h, 00Dh, 044h, 066h, 000h, 000h
        db  089h, 00Dh, 040h, 066h, 000h, 000h
        cmp    ecx, 1ah
        jbe    SHORT loc_3_2B71
        db  0C7h, 005h, 040h, 066h, 000h, 000h, 01Ah, 000h, 000h, 000h
loc_3_2B71:
        movzx  eax, word ptr [ebx]
        movzx  edx, word ptr [ebx + 2]
        shl    edx, 4
        add    eax, edx
        db  0A3h, 0B8h, 061h, 000h, 000h
        movzx  eax, word ptr [ebx + 16h]
        movzx  edx, word ptr [ebx + 18h]
        shl    edx, 4
        add    eax, edx
        db  0BFh, 048h, 066h, 000h, 000h
        db  08Bh, 015h, 0C8h, 066h, 000h, 000h
loc_3_2B9A:
        stosd
        add    eax, edx
        loop   loc_3_2B9A
        xor    edx, edx
        db  08Dh, 01Dh, 0BCh, 061h, 000h, 000h
        mov    byte ptr [ebx], 2
loc_3_2BAA:
        mov    byte ptr [ebx + 2], dl
        add    byte ptr [ebx + 2], 41h
        push   ebx
        db  0CDh, 020h, 008h, 000h, 010h, 000h
        add    esp, 4
        sub    byte ptr [ebx + 2], 41h
        dec    byte ptr [ebx + 1]
        jns    NEAR PTR loc_3_2C6C
        db  080h, 08Ah, 08Ch, 000h, 000h, 000h, 001h
        movzx  edx, byte ptr [ebx + 2]
        db  0A1h, 0B0h, 065h, 000h, 000h
        test   byte ptr [edx + eax + 1308h], 10h
        je     SHORT loc_3_2BE9
        db  080h, 08Ah, 08Ch, 000h, 000h, 000h, 020h
loc_3_2BE9:
        test   dword ptr [ebx + 3], 2000h
        je     SHORT loc_3_2BF9
        db  080h, 08Ah, 08Ch, 000h, 000h, 000h, 004h
loc_3_2BF9:
        test   dword ptr [ebx + 3], 1
        je     SHORT loc_3_2C09
        db  080h, 08Ah, 08Ch, 000h, 000h, 000h, 040h
loc_3_2C09:
        cmp    edx, 1
        ja     SHORT loc_3_2C1E
        test   dword ptr [ebx + 3], 800000h
        je     SHORT loc_3_2C1E
        db  080h, 08Ah, 08Ch, 000h, 000h, 000h, 008h
loc_3_2C1E:
        push   1
        push   18h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        movzx  edx, byte ptr [ebx + 2]
        mov    dword ptr [eax], 0
        db  08Dh, 00Ch, 095h, 0C0h, 065h, 000h, 000h
        mov    dword ptr [ecx], eax
        db  08Bh, 004h, 095h, 048h, 066h, 000h, 000h
        test   byte ptr [eax + 44h], 40h
        jne    SHORT loc_3_2C6C
        or     byte ptr [eax + 44h], 40h
        mov    word ptr [eax + 4fh], 2
        xor    ecx, ecx
        mov    cl, 5ch
        shl    ecx, 10h
        mov    ch, 3ah
        mov    cl, dl
        add    cl, 41h
        mov    dword ptr [eax], ecx
        db  080h, 08Ah, 08Ch, 000h, 000h, 000h, 010h
loc_3_2C6C:
        movzx  edx, byte ptr [ebx + 2]
        inc    edx
        db  03Bh, 015h, 044h, 066h, 000h, 000h
        jb     NEAR PTR loc_3_2BAA
        db  08Bh, 00Dh, 044h, 066h, 000h, 000h
        db  08Dh, 035h, 048h, 066h, 000h, 000h
loc_3_2C89:
        or     ecx, ecx
        db  00Fh, 084h, 02Dh, 001h, 000h, 000h
        lodsd
        test   word ptr [eax + 43h], 1000h
        loope  loc_3_2C89
        je     SHORT loc_3_2C89
        movzx  edx, byte ptr [eax]
        sub    dl, 41h
        db  0F6h, 082h, 08Ch, 000h, 000h, 000h, 001h
        je     SHORT loc_3_2C89
        push   ecx
        push   esi
        db  08Bh, 035h, 044h, 066h, 000h, 000h
        sub    esi, ecx
        dec    esi
        push   edx
        xchg   ebx, eax
        push   1
        push   18h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        xchg   ebx, eax
        pop    edx
        or     ebx, ebx
        db  00Fh, 084h, 0F2h, 000h, 000h, 000h
        mov    dword ptr [ebx], 0
        lea    ecx, [eax + 3]
        mov    dword ptr [ebx + 4], ecx
        mov    dword ptr [ebx + 14h], 0
        mov    byte ptr [ebx + 0ah], 0
        mov    byte ptr [ebx + 0bh], 10h
        db  089h, 01Ch, 0B5h, 0C0h, 065h, 000h, 000h
        inc    edx
        mov    byte ptr [ebx + 0fh], dl
        dec    edx
        mov    dword ptr [eax + 49h], 0ffffffffh
        mov    word ptr [eax + 4dh], 6eb2h
        db  08Bh, 00Dh, 044h, 059h, 000h, 000h
        or     byte ptr [esi + ecx + 2fh], 1
        db  08Bh, 00Dh, 0B0h, 065h, 000h, 000h
        or     byte ptr [esi + ecx + 1308h], 1
        db  080h, 08Eh, 08Ch, 000h, 000h, 000h, 080h
        db  0F6h, 086h, 08Ch, 000h, 000h, 000h, 001h
        jne    SHORT loc_3_2D3D
        db  080h, 08Eh, 08Ch, 000h, 000h, 000h, 001h
        or     byte ptr [ebx + 0bh], 8
        sub    word ptr [eax + 4dh], 4000h
loc_3_2D3D:
        movzx  edx, word ptr [eax + 4fh]
        lea    edx, [edx*2]
        mov    word ptr [ebx + 0ch], dx
loc_3_2D4C:
        push   edx
        add    edx, 2
        push   1
        push   edx
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
loc_3_2D5C:
        pop    edx
        or     eax, eax
        db  074h, 060h
        mov    dword ptr [ebx + 10h], eax
        pop    esi
        push   esi
        push   1
        shr    edx, 1
        push   edx
        push   dword ptr [esi - 4]
        push   eax
        db  0E8h, 0F5h, 01Ch, 0FEh, 0FFh
        add    esp, 10h
        db  003h
_ITEXT ends
        end
