; vdd_pcode_16.asm -- VDD object 16 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  081h, 0ECh, 0CCh, 000h, 000h, 000h, 056h, 057h, 08Dh, 044h, 024h, 008h, 068h, 0CCh, 000h, 000h
        db  000h, 050h, 0FFh, 0B4h, 024h, 0E0h, 000h, 000h, 000h, 0E8h, 07Fh, 003h, 0F0h, 0FFh, 083h, 0C4h
        db  00Ch, 00Bh, 0C0h, 074h, 012h, 08Dh, 074h, 024h, 008h, 0BFh, 0D4h, 010h, 000h, 000h, 0B9h, 033h
        db  000h, 000h, 000h, 0F3h, 0A5h, 0EBh, 00Ah, 0C7h, 005h, 0D4h, 010h, 000h, 000h, 0FFh, 0FFh, 0FFh
        db  0FFh, 0C7h, 005h, 0B0h, 010h, 000h, 000h, 001h, 000h, 000h, 000h, 0B8h, 001h, 000h, 000h, 000h
        db  05Fh, 05Eh, 081h, 0C4h, 0CCh, 000h, 000h, 000h, 0C3h
sub_16_0059:
        sub    esp, 4
        mov    al, byte ptr [esp + 10h]
        mov    byte ptr [esp + 2], al
        mov    byte ptr [esp + 3], 0
        push   1
        lea    eax, [esp + 6]
        push   2
        push   eax
        mov    eax, dword ptr [esp + 18h]
        push   1
        db  0FFh, 034h, 085h, 038h, 000h, 000h, 000h
        db  0FFh, 035h, 028h, 000h, 000h, 000h
        push   dword ptr [esp + 20h]
        db  0E8h, 00Ah, 019h, 0F0h, 0FFh
        add    esp, 20h
        ret
sub_16_0092:
        sub    esp, 18h
        mov    ecx, dword ptr [esp + 20h]
        mov    dword ptr [esp], 14h
        db  08Ah, 081h, 040h, 000h, 000h, 000h
        mov    byte ptr [esp + 4], al
        lea    eax, [esp]
        push   1
        push   eax
        lea    eax, [esp + 0ch]
        push   eax
        push   1
        db  0FFh, 034h, 08Dh, 038h, 000h, 000h, 000h
        db  0FFh, 035h, 028h, 000h, 000h, 000h
        push   dword ptr [esp + 34h]
        db  0E8h, 0C1h, 018h, 0F0h, 0FFh
        movsx  eax, byte ptr [esp + 20h]
        add    esp, 1ch
        add    esp, 18h
        ret
sub_16_00DB:
        sub    esp, 14ch
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 160h]
        cmp    ebx, 2
        jb     SHORT loc_16_00F7
        sub    eax, eax
        jmp    NEAR PTR loc_16_01F4
loc_16_00F7:
        mov    esi, dword ptr [esp + 15ch]
        push   ebx
        push   esi
        call   sub_16_0092
        add    esp, 8
        cmp    eax, 30h
        jne    SHORT loc_16_0114
        sub    eax, eax
        jmp    NEAR PTR loc_16_01F4
loc_16_0114:
        lea    edi, [esp + 8ch]
        sub    eax, eax
        mov    ecx, 33h
        rep stosd
        mov    dword ptr [esp + 8ch], ebx
        push   eax
        lea    eax, [esp + 94h]
        push   0c8h
        push   eax
        push   esi
        db  0E8h, 009h, 018h, 0F0h, 0FFh
        add    esp, 10h
        db  068h, 044h, 000h, 000h, 000h
        db  0E8h, 09Ch, 017h, 0F0h, 0FFh
        push   eax
        lea    eax, [esp + 14h]
        db  068h, 050h, 000h, 000h, 000h
        push   eax
        db  0E8h, 058h, 017h, 0F0h, 0FFh
        add    esp, 10h
        db  083h, 03Dh, 0B0h, 010h, 000h, 000h, 000h
        jne    SHORT loc_16_0175
        lea    eax, [esp + 0ch]
        push   eax
        call   0
        add    esp, 4
loc_16_0175:
        db  0A1h, 0D4h, 010h, 000h, 000h
        cmp    dword ptr [esp + 8ch], eax
        jne    SHORT loc_16_01D5
        db  0B8h, 0D8h, 010h, 000h, 000h
        lea    edx, [esp + 90h]
loc_16_018F:
        mov    cl, byte ptr [eax]
        cmp    cl, byte ptr [edx]
        jne    SHORT loc_16_01AF
        or     cl, cl
        je     SHORT loc_16_01AB
        mov    cl, byte ptr [eax + 1]
        cmp    cl, byte ptr [edx + 1]
        jne    SHORT loc_16_01AF
        add    eax, 2
        add    edx, 2
        or     cl, cl
        jne    SHORT loc_16_018F
loc_16_01AB:
        sub    eax, eax
        jmp    SHORT loc_16_01B4
loc_16_01AF:
        sbb    eax, eax
        sbb    eax, -1
loc_16_01B4:
        or     eax, eax
        jne    SHORT loc_16_01D5
        lea    eax, [esp + 0ch]
        push   eax
        db  0E8h, 0A0h, 001h, 0F0h, 0FFh
        add    esp, 4
        push   30h
        push   ebx
        push   esi
        call   sub_16_0059
        add    esp, 0ch
        sub    eax, eax
        jmp    SHORT loc_16_01F4
loc_16_01D5:
        lea    eax, [esp + 8ch]
        push   0cch
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        db  0E8h, 008h, 001h, 0F0h, 0FFh
        add    esp, 0ch
        mov    eax, 1
loc_16_01F4:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14ch
        ret
sub_16_01FE:
        sub    esp, 80h
        cmp    dword ptr [esp + 88h], 2
        mov    eax, 0
        jae    SHORT loc_16_0242
        db  068h, 058h, 000h, 000h, 000h
        db  0E8h, 0CBh, 016h, 0F0h, 0FFh
        push   eax
        lea    eax, [esp + 8]
        db  068h, 064h, 000h, 000h, 000h
        push   eax
        db  0E8h, 087h, 016h, 0F0h, 0FFh
        lea    eax, [esp + 10h]
        add    esp, 10h
        push   eax
        db  0E8h, 028h, 001h, 0F0h, 0FFh
        add    esp, 4
        mov    eax, 1
loc_16_0242:
        add    esp, 80h
        ret
_PTEXT ends
        end
