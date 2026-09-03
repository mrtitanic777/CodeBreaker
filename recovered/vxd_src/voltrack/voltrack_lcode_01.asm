; voltrack_lcode_01.asm -- VOLTRACK object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

voltrack_Control:
        cmp    eax, 1bh
        db  00Fh, 084h, 0F7h, 0FFh, 0FEh, 0FFh
        clc
        ret
        db  000h
sub_1_000C:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    ebx, dword ptr [esi + 4]
        mov    edi, dword ptr [esi + 0a8h]
        cmp    word ptr [esi + 68h], 4
        ja     NEAR PTR loc_1_0118
loc_1_0027:
        call   sub_1_018A
        jne    SHORT loc_1_004B
        test   byte ptr [esi + 6ch], 20h
        jne    SHORT loc_1_004B
        mov    eax, dword ptr [esi + 8]
        test   byte ptr [eax + 1dh], 20h
        jne    NEAR PTR loc_1_02EE
loc_1_0041:
        mov    ebx, dword ptr [esi + 4]
        cli
        test   byte ptr [ebx + 22h], 8
        jne    SHORT loc_1_0092
loc_1_004B:
        sti
        test   byte ptr [ebx + 21h], 80h
        inc    byte ptr [ebx + 78h]
        call   sub_1_018A
        jne    SHORT loc_1_0082
        test   dword ptr [esi + 6ch], 28h
        jne    SHORT loc_1_0082
        or     edi, edi
        je     SHORT loc_1_0082
        test   byte ptr [edi + 4], 20h
        jne    NEAR PTR loc_1_013E
        mov    ebx, dword ptr [esi + 8]
        test   dword ptr [ebx + 20h], 101000h
        jne    SHORT loc_1_008B
        cmp    dword ptr [ebx + 18h], edi
        jne    SHORT loc_1_008B
loc_1_0082:
        call   sub_1_02B0
loc_1_0087:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_008B:
        call   sub_1_033E
        jmp    SHORT loc_1_0087
loc_1_0092:
        test   byte ptr [ebx + 0b8h], 2
        jne    SHORT loc_1_00EF
        test   byte ptr [esi + 6dh], 40h
        jne    SHORT loc_1_004B
        test   byte ptr [ebx + 0b8h], 4
        jne    SHORT loc_1_004B
        test   byte ptr [ebx + 22h], 4
        je     SHORT loc_1_00BD
        cmp    word ptr [esi + 68h], 0
        jne    SHORT loc_1_00BD
        test   byte ptr [esi + 6dh], 8
        jne    SHORT loc_1_00C3
loc_1_00BD:
        cmp    byte ptr [ebx + 78h], 1
        ja     SHORT loc_1_00EF
loc_1_00C3:
        test   byte ptr [esi + 6ch], 8
        jne    SHORT loc_1_00D8
        cmp    dword ptr [esi + 0a8h], 0
        je     SHORT loc_1_00D8
        cmp    byte ptr [ebx + 78h], 0
        jne    SHORT loc_1_00EF
loc_1_00D8:
        or     byte ptr [ebx + 0b8h], 2
        sti
        mov    edx, esi
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        jmp    SHORT loc_1_0087
loc_1_00EF:
        sti
        test   byte ptr [esi + 6ch], 8
        jne    NEAR PTR loc_1_004B
        or     edi, edi
        je     NEAR PTR loc_1_004B
        cli
        mov    ecx, dword ptr [ebx + 0afh]
        mov    dword ptr [esi + 64h], ecx
        mov    dword ptr [ebx + 0afh], esi
        sti
        jmp    NEAR PTR loc_1_0087
loc_1_0118:
        movzx  eax, word ptr [esi + 68h]
        cld
        db  08Dh, 03Dh, 066h, 001h, 000h, 000h
        mov    ecx, 6
        repne scasw
        mov    edi, dword ptr [esi + 0a8h]
        jne    NEAR PTR loc_1_0027
        db  0FFh, 024h, 08Dh, 072h, 001h, 000h, 000h
loc_1_013E:
        mov    word ptr [esi + 6ah], 11h
        push   esi
        call   sub_1_0484
        add    esp, 4
        jmp    NEAR PTR loc_1_0087
loc_1_0152:
        call   sub_1_050C
        jmp    NEAR PTR loc_1_0087
loc_1_015C:
        call   sub_1_02CD
        jmp    NEAR PTR loc_1_0087
loc_1_0166:
        or     dword ptr [eax], eax
        or     al, byte ptr [eax]
        or     byte ptr [eax], al
        or     eax, 28002700h
        db  000h, 05Ch, 001h, 000h
        db  000h, 05Ch, 001h, 000h, 000h, 073h, 003h, 000h, 000h, 082h, 002h, 000h, 000h, 009h, 002h, 000h
        db  000h, 09Ah, 001h, 000h, 000h
sub_1_018A:
        mov    eax, dword ptr [esi + 8]
        test   byte ptr [ebx + 21h], 20h
        jne    SHORT loc_1_0195
        mov    eax, ebx
loc_1_0195:
        test   byte ptr [eax + 21h], 1
        ret
loc_1_019A:
        mov    word ptr [esi + 6ah], 16h
        test   byte ptr [ebx + 1eh], 1
        je     SHORT loc_1_0152
        mov    word ptr [esi + 6ah], 18h
        or     edi, edi
        je     SHORT loc_1_01B6
        test   byte ptr [edi + 4], 20h
        jne    SHORT loc_1_0152
loc_1_01B6:
        mov    ebx, dword ptr [esi + 4]
        test   byte ptr [ebx + 0b8h], 1
        jne    SHORT loc_1_0152
        mov    word ptr [esi + 6ah], 26h
        cmp    byte ptr [ebx + 79h], 0ffh
        je     SHORT loc_1_0152
        mov    word ptr [esi + 6ah], 0
        inc    byte ptr [ebx + 79h]
        cmp    byte ptr [ebx + 79h], 1
        je     SHORT loc_1_01F4
        inc    byte ptr [ebx + 78h]
        mov    word ptr [esi + 6ah], 23h
        push   esi
        call   sub_1_0484
        add    esp, 4
        jmp    NEAR PTR loc_1_0087
loc_1_01F4:
        push   esi
        xor    esi, esi
        xchg   dword ptr [ebx + 0b3h], esi
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        pop    esi
        jmp    NEAR PTR loc_1_0027
loc_1_0209:
        mov    word ptr [esi + 6ah], 16h
        mov    ebx, dword ptr [esi + 4]
        test   byte ptr [ebx + 1eh], 1
        je     NEAR PTR loc_1_0152
        mov    word ptr [esi + 6ah], 22h
        cmp    byte ptr [ebx + 79h], 0
        je     NEAR PTR loc_1_0152
        mov    word ptr [esi + 6ah], 0
        dec    byte ptr [ebx + 79h]
        jne    NEAR PTR loc_1_0152
        test   byte ptr [ebx + 0b8h], 1
        jne    NEAR PTR loc_1_0027
        push   esi
        mov    eax, 1388h
        mov    edx, ebx
        db  0BEh, 067h, 002h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    dword ptr [ebx + 0b3h], esi
        pop    esi
        jmp    NEAR PTR loc_1_0152
loc_1_0267:
        mov    ebx, edx
        xor    eax, eax
        mov    dword ptr [ebx + 0b3h], eax
        cmp    byte ptr [ebx + 79h], al
        jne    SHORT loc_1_0281
        mov    ax, 0ah
        xor    ecx, ecx
        db  0E8h, 0A5h, 004h, 0FFh, 0FFh
loc_1_0281:
        ret
loc_1_0282:
        test   byte ptr [esi + 6fh], 10h
        je     SHORT loc_1_029E
        push   0
        push   esi
        db  068h, 02Fh, 002h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 017h, 000h
        add    esp, 0ch
        jmp    NEAR PTR loc_1_0087
loc_1_029E:
        mov    edx, esi
        db  0BEh, 03Fh, 002h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        jmp    NEAR PTR loc_1_0087
sub_1_02B0:
        mov    eax, dword ptr [esi + 8]
        test   byte ptr [eax + 1dh], 20h
        jne    SHORT loc_1_02DD
loc_1_02B9:
        cmp    word ptr [esi + 68h], 0fh
        je     SHORT loc_1_02E6
        mov    eax, dword ptr [esi + 14h]
        db  0C7h, 000h, 084h, 004h, 000h, 000h
        add    dword ptr [esi + 14h], 8
sub_1_02CD:
        push   esi
        mov    eax, dword ptr [esi + 10h]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [esi + 10h], eax
        call   dword ptr [eax]
        add    esp, 4
        ret
loc_1_02DD:
        mov    byte ptr [esi + 0b1h], 0
        jmp    SHORT loc_1_02B9
loc_1_02E6:
        mov    eax, dword ptr [esi + 4]
        dec    byte ptr [eax + 78h]
        jmp    SHORT sub_1_02CD
loc_1_02EE:
        mov    al, byte ptr [esi + 0b1h]
        test   byte ptr [esi + 6eh], 1
        jne    SHORT loc_1_0307
        mov    byte ptr [esi + 19h], al
        mov    ecx, dword ptr [ebx]
        cmp    byte ptr [ecx + 0aeh], al
        jne    SHORT loc_1_030C
loc_1_0307:
        jmp    NEAR PTR loc_1_0041
loc_1_030C:
        db  0E8h, 0C8h, 0FEh, 0FEh, 0FFh
        test   byte ptr [esi + 6ch], 8
        je     NEAR PTR loc_1_0041
        test   byte ptr [ebx + 21h], 20h
        je     SHORT loc_1_0324
        mov    ebx, dword ptr [esi + 8]
loc_1_0324:
        test   byte ptr [ebx + 1eh], 4
        je     NEAR PTR loc_1_0041
        mov    ax, 6
        xor    ecx, ecx
        db  0E8h, 0F8h, 003h, 0FFh, 0FFh
        jmp    NEAR PTR loc_1_0041
sub_1_033E:
        cli
        mov    edx, dword ptr [ebx]
        inc    byte ptr [edx + 76h]
        test   byte ptr [edi + 4], 4
        jne    SHORT loc_1_0355
        cmp    dword ptr [ebx + 18h], edi
        je     SHORT sub_1_036C
        cmp    byte ptr [edx + 78h], 1
        je     SHORT sub_1_036C
loc_1_0355:
        dec    byte ptr [edx + 78h]
        dec    byte ptr [edx + 76h]
        mov    ecx, dword ptr [edx + 0afh]
        mov    dword ptr [esi + 64h], ecx
        mov    dword ptr [edx + 0afh], esi
        sti
        ret
sub_1_036C:
        or     byte ptr [edi + 4], 4
        sti
        mov    edx, esi
        db  0BEh, 030h, 004h, 000h, 000h
        db  0CDh, 020h, 00Eh, 080h, 001h, 000h
        add    byte ptr [eax], al
loc_1_0380:
        mov    ebx, dword ptr [esp + 4]
        mov    word ptr [ebx + 2], 0
        movzx  esi, word ptr [ebx]
        cmp    si, 7
        je     SHORT loc_1_03B8
        cmp    si, 3
        db  00Fh, 084h, 0A3h, 005h, 0FFh, 0FFh
        cmp    si, 2
        db  00Fh, 084h, 015h, 006h, 0FFh, 0FFh
        cmp    si, 0eh
        db  00Fh, 084h, 01Bh, 006h, 0FFh, 0FFh
        mov    word ptr [ebx + 2], 0ffffh
        ret
loc_1_03B8:
        db  08Bh, 015h, 0D8h, 005h, 000h, 000h
        mov    eax, dword ptr [edx + 30h]
loc_1_03C1:
        or     eax, eax
        je     SHORT loc_1_041B
        cmp    byte ptr [eax + 40h], 0
        je     SHORT loc_1_03DC
        cmp    byte ptr [eax + 40h], 0ah
        je     SHORT loc_1_03DC
        cmp    byte ptr [eax + 40h], 5
        je     SHORT loc_1_03DC
loc_1_03D7:
        mov    eax, dword ptr [eax + 0ch]
        jmp    SHORT loc_1_03C1
loc_1_03DC:
        mov    ecx, dword ptr [eax + 20h]
        and    ecx, 48004h
        cmp    ecx, 8004h
        jne    SHORT loc_1_03D7
        cmp    byte ptr [eax + 78h], 0
        jne    SHORT loc_1_03D7
        mov    cl, byte ptr [eax + 0b7h]
        inc    byte ptr [eax + 0b7h]
        and    cl, 3
        cmp    cl, 3
        jne    SHORT loc_1_03D7
        mov    cl, 1
        test   dword ptr [eax + 20h], 30000h
        je     SHORT loc_1_0414
        xor    cl, cl
loc_1_0414:
        call   sub_1_041C
        jmp    SHORT loc_1_03D7
loc_1_041B:
        ret
sub_1_041C:
        pushfd
        or     cl, cl
        mov    ecx, 1000h
        je     SHORT loc_1_0433
        or     ecx, 100000h
        or     dword ptr [eax + 20h], 80000h
loc_1_0433:
        push   edx
        push   eax
        test   dword ptr [eax + 20h], 4000h
        jne    SHORT loc_1_0441
        mov    eax, dword ptr [eax + 10h]
loc_1_0441:
        or     eax, eax
        je     SHORT loc_1_047F
        test   dword ptr [eax + 20h], 4
        je     SHORT loc_1_0479
        or     dword ptr [eax + 20h], ecx
        db  08Bh, 015h, 0D8h, 005h, 000h, 000h
        mov    edx, dword ptr [edx + 40h]
        cli
loc_1_045B:
        or     edx, edx
        je     SHORT loc_1_0479
        cmp    dword ptr [edx + 28h], eax
        jne    SHORT loc_1_0474
        or     dword ptr [edx + 4], 2
        test   ecx, 100000h
        je     SHORT loc_1_0474
        or     dword ptr [edx + 4], 1
loc_1_0474:
        mov    edx, dword ptr [edx + 24h]
        jmp    SHORT loc_1_045B
loc_1_0479:
        sti
        mov    eax, dword ptr [eax + 10h]
        jmp    SHORT loc_1_0441
loc_1_047F:
        pop    eax
        pop    edx
        popfd
        ret
        db  000h
sub_1_0484:
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esi + 0a8h]
        mov    ebx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi + 8]
        test   byte ptr [eax + 1dh], 20h
        jne    SHORT loc_1_04F4
loc_1_049D:
        cmp    word ptr [esi + 6ah], 0
        jne    SHORT loc_1_051C
        mov    byte ptr [ebx + 0b7h], 0
loc_1_04AB:
        dec    byte ptr [ebx + 78h]
        test   byte ptr [ebx + 0b8h], 1
        jne    SHORT loc_1_04CC
loc_1_04B7:
        push   ebx
        call   sub_1_050C
        pop    ebx
        cmp    dword ptr [ebx + 0afh], 0
        jne    SHORT loc_1_04FF
loc_1_04C7:
        sti
        pop    ebx
        pop    edi
        pop    esi
        ret
loc_1_04CC:
        cmp    word ptr [esi + 68h], 8
        jne    SHORT loc_1_04B7
        and    byte ptr [ebx + 0b8h], 0feh
        cmp    word ptr [esi + 6ah], 10h
        jae    SHORT loc_1_04B7
        push   0
        push   ebx
        db  068h, 064h, 00Bh, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 017h, 000h
        add    esp, 0ch
        jmp    SHORT loc_1_04B7
loc_1_04F4:
        mov    al, byte ptr [esi + 19h]
        mov    byte ptr [esi + 0b1h], al
        jmp    SHORT loc_1_049D
loc_1_04FF:
        cmp    byte ptr [ebx + 78h], 0
        jne    SHORT loc_1_04C7
        db  0E8h, 028h, 006h, 0FFh, 0FFh
        jmp    SHORT loc_1_04C7
sub_1_050C:
        mov    eax, dword ptr [esi + 14h]
        sub    eax, 8
        mov    dword ptr [esi + 14h], eax
        push   esi
        call   dword ptr [eax]
        add    esp, 4
        ret
loc_1_051C:
        cmp    word ptr [esi + 68h], 9
        jne    SHORT loc_1_0526
        dec    byte ptr [ebx + 79h]
loc_1_0526:
        call   sub_1_018A
        jne    NEAR PTR loc_1_04AB
        test   byte ptr [esi + 6ch], 20h
        jne    NEAR PTR loc_1_04AB
        inc    byte ptr [ebx + 76h]
        mov    edx, esi
        db  0BEh, 018h, 00Ah, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        jmp    NEAR PTR loc_1_04C7
sub_1_0550:
        push   ebx
        mov    ebx, dword ptr [esi + 8]
        mov    eax, dword ptr [ebx + 8]
loc_1_0557:
        db  081h, 038h, 00Ch, 000h, 000h, 000h
        je     SHORT loc_1_0566
        mov    eax, dword ptr [eax + 0ch]
        or     eax, eax
        jne    SHORT loc_1_0557
loc_1_0566:
        mov    dword ptr [esi + 10h], eax
        pop    ebx
        ret
        db  000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 090h, 000h, 003h, 00Ah, 000h, 000h, 076h, 06Fh, 06Ch, 074h
        db  072h, 061h, 063h, 06Bh, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 084h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_05C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_05D0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_05D8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_05F0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_05F4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end
