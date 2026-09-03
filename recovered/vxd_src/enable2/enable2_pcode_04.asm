; enable2_pcode_04.asm -- ENABLE2 object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_4_0000:
        mov    edx, dword ptr [esp + 4]
        cmp    edx, 4
        jae    SHORT loc_4_005D
        pushad
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  00Fh, 0B7h, 004h, 055h, 00Eh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 01Dh, 00Ch, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  0FFh, 034h, 095h, 034h, 018h, 000h, 000h
        db  0FFh, 034h, 095h, 030h, 018h, 000h, 000h
        push   eax
        push   ebx
        db  0CDh, 020h, 018h, 000h, 017h, 000h
        add    esp, 10h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        popad
loc_4_005D:
        ret
loc_4_005E:
        mov    edx, dword ptr [esp + 4]
        db  089h, 015h, 054h, 018h, 000h, 000h
        pushad
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0A1h, 050h, 018h, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_009C
        cmp    byte ptr [eax], 0
        je     SHORT loc_4_009C
        db  068h, 054h, 018h, 000h, 000h
        push   4
        db  068h, 058h, 018h, 000h, 000h
        push   eax
        db  0CDh, 020h, 018h, 000h, 017h, 000h
        add    esp, 10h
loc_4_009C:
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        popad
        ret
        db  000h, 000h, 000h
sub_4_00B0:
        db  0F7h, 005h, 076h, 017h, 000h, 000h, 001h, 000h, 000h, 000h
        jne    SHORT loc_4_00C1
        jmp    NEAR PTR loc_4_0184
loc_4_00C1:
        db  066h, 0A1h, 026h, 017h, 000h, 000h
        sub    ax, 37h
        je     SHORT loc_4_00D4
        jb     SHORT loc_4_00D4
        jmp    NEAR PTR loc_4_017E
loc_4_00D4:
        db  066h, 0C7h, 005h, 026h, 017h, 000h, 000h, 000h, 000h
        db  081h, 03Dh, 086h, 017h, 000h, 000h, 0F8h, 07Fh, 000h, 000h
        je     SHORT loc_4_00EE
        jmp    NEAR PTR loc_4_0184
loc_4_00EE:
        db  066h, 0A1h, 024h, 017h, 000h, 000h
        or     ax, ax
        jne    SHORT loc_4_00FE
        jmp    NEAR PTR loc_4_0184
loc_4_00FE:
        db  0F7h, 005h, 076h, 017h, 000h, 000h, 040h, 000h, 000h, 000h
        je     SHORT loc_4_0121
        and    al, 7fh
        cmp    al, 36h
        je     SHORT loc_4_0121
        cmp    al, 2ah
        je     SHORT loc_4_0121
        cmp    al, 1dh
        je     SHORT loc_4_0121
        cmp    al, 38h
        je     SHORT loc_4_0121
        db  0E8h, 057h, 013h, 0FCh, 0FFh
loc_4_0121:
        push   eax
        db  080h, 03Dh, 025h, 017h, 000h, 000h, 0E0h
        jne    SHORT loc_4_0134
        push   eax
        mov    al, 0e0h
        db  0E8h, 001h, 00Bh, 0FCh, 0FFh
        pop    eax
loc_4_0134:
        db  0E8h, 0FBh, 00Ah, 0FCh, 0FFh
        pop    eax
        jb     SHORT loc_4_0151
        db  0A2h, 068h, 018h, 000h, 000h
        mov    ecx, 1
        db  0BEh, 068h, 018h, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 00Dh, 000h
loc_4_0151:
        db  0A1h, 082h, 017h, 000h, 000h
        db  080h, 03Dh, 028h, 017h, 000h, 000h, 000h
        jne    SHORT loc_4_017E
        db  0C6h, 005h, 028h, 017h, 000h, 000h, 0FFh
        db  081h, 03Dh, 086h, 017h, 000h, 000h, 0F8h, 07Fh, 000h, 000h
        je     SHORT loc_4_0179
        db  0A1h, 07Eh, 017h, 000h, 000h
        jmp    SHORT loc_4_017E
loc_4_0179:
        db  0A1h, 07Eh, 017h, 000h, 000h
loc_4_017E:
        db  066h, 0A3h, 026h, 017h, 000h, 000h
loc_4_0184:
        ret
        db  000h, 000h, 000h
sub_4_0188:
        db  0F7h, 005h, 096h, 017h, 000h, 000h, 001h, 000h, 000h, 000h
        je     NEAR PTR loc_4_02D2
        db  0E8h, 04Eh, 005h, 0FCh, 0FFh
        jae    SHORT loc_4_01A4
        jmp    NEAR PTR loc_4_02D2
loc_4_01A4:
        db  080h, 03Dh, 06Ch, 018h, 000h, 000h, 000h
        je     NEAR PTR loc_4_02D2
        db  0F7h, 005h, 096h, 017h, 000h, 000h, 040h, 000h, 000h, 000h
        jne    SHORT loc_4_01F0
        db  08Ah, 02Dh, 062h, 017h, 000h, 000h
        db  00Ah, 02Dh, 037h, 017h, 000h, 000h
        db  00Ah, 02Dh, 063h, 017h, 000h, 000h
        test   ch, 14h
        je     SHORT loc_4_01F0
        db  0A1h, 03Ch, 019h, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_01E9
        dec    eax
        db  0A3h, 03Ch, 019h, 000h, 000h
        jne    NEAR PTR loc_4_02D2
loc_4_01E9:
        db  0A1h, 0A2h, 017h, 000h, 000h
        jmp    SHORT loc_4_026B
loc_4_01F0:
        test   ch, 3
        jne    SHORT loc_4_01F7
        jmp    SHORT loc_4_0213
loc_4_01F7:
        db  0A1h, 03Ch, 019h, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_020C
        dec    eax
        db  0A3h, 03Ch, 019h, 000h, 000h
        jne    NEAR PTR loc_4_02D2
loc_4_020C:
        mov    eax, 1
        jmp    SHORT loc_4_026B
loc_4_0213:
        xor    eax, eax
        xor    ebx, ebx
        db  080h, 03Dh, 06Dh, 018h, 000h, 000h, 000h
        jne    SHORT loc_4_024B
        db  08Ah, 01Dh, 070h, 018h, 000h, 000h
        db  03Ah, 01Dh, 03Bh, 019h, 000h, 000h
        jb     SHORT loc_4_0239
        db  0C6h, 005h, 06Dh, 018h, 000h, 000h, 0FFh
        xor    ebx, ebx
        jmp    SHORT loc_4_025B
loc_4_0239:
        db  08Ah, 083h, 044h, 019h, 000h, 000h
        cbw
        inc    bx
        db  088h, 01Dh, 070h, 018h, 000h, 000h
        jmp    SHORT loc_4_026B
loc_4_024B:
        db  08Ah, 01Dh, 071h, 018h, 000h, 000h
        db  03Ah, 01Dh, 0C4h, 019h, 000h, 000h
        jb     SHORT loc_4_025B
        xor    ebx, ebx
loc_4_025B:
        db  08Ah, 083h, 0C5h, 019h, 000h, 000h
        cbw
        inc    bx
        db  088h, 01Dh, 071h, 018h, 000h, 000h
loc_4_026B:
        or     ax, ax
        je     SHORT loc_4_02D2
        xor    bx, bx
        db  038h, 01Dh, 073h, 018h, 000h, 000h
        je     SHORT loc_4_0283
        mov    bx, ax
        jg     SHORT loc_4_0283
        neg    bx
loc_4_0283:
        xor    cx, cx
        db  038h, 00Dh, 072h, 018h, 000h, 000h
        je     SHORT loc_4_0296
        mov    cx, ax
        jg     SHORT loc_4_0296
        neg    cx
loc_4_0296:
        db  066h, 089h, 00Dh, 07Ch, 018h, 000h, 000h
        db  066h, 089h, 01Dh, 07Eh, 018h, 000h, 000h
        db  066h, 083h, 00Dh, 080h, 018h, 000h, 000h, 001h
        db  00Fh, 0BFh, 035h, 07Eh, 018h, 000h, 000h
        db  00Fh, 0BFh, 03Dh, 07Ch, 018h, 000h, 000h
        db  066h, 0A1h, 080h, 018h, 000h, 000h
        db  0E8h, 02Bh, 006h, 0FCh, 0FFh
        db  0E8h, 00Bh, 004h, 0FCh, 0FFh
        db  0CDh, 020h, 003h, 000h, 00Ch, 000h
        jmp    SHORT loc_4_02D2
loc_4_02D2:
        ret
loc_4_02D3:
        db  0C7h, 005h, 0F1h, 019h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    al, dl
        cmp    al, 0feh
        jne    SHORT loc_4_02EE
        db  080h, 025h, 0E9h, 019h, 000h, 000h, 0EFh
        xor    ebx, ebx
        jmp    SHORT loc_4_032C
loc_4_02EE:
        cmp    al, 0ffh
        jne    SHORT loc_4_02FD
        db  080h, 00Dh, 0E9h, 019h, 000h, 000h, 010h
        xor    ebx, ebx
        jmp    SHORT loc_4_032C
loc_4_02FD:
        shr    al, 2
        and    al, 0ch
        db  08Ah, 025h, 074h, 018h, 000h, 000h
        and    ah, 3
        or     al, ah
        db  080h, 03Dh, 06Fh, 018h, 000h, 000h, 000h
        jne    SHORT loc_4_0318
        or     al, 10h
loc_4_0318:
        db  0A2h, 0E9h, 019h, 000h, 000h
        db  03Ah, 005h, 0EDh, 019h, 000h, 000h
        je     SHORT loc_4_0366
        db  0A2h, 0EDh, 019h, 000h, 000h
        xor    ebx, ebx
loc_4_032C:
        db  08Bh, 02Dh, 010h, 017h, 000h, 000h
        cmp    dword ptr [ebp + ebx*8], 0ffffh
        je     SHORT loc_4_0360
        mov    eax, dword ptr [ebp + ebx*8]
        mov    ecx, dword ptr [ebp + ebx*8 + 4]
        push   ebx
        push   edx
        db  08Bh, 035h, 0E9h, 019h, 000h, 000h
        push   0
        push   0
        push   esi
        push   2
        push   ecx
        push   eax
        db  0CDh, 020h, 006h, 000h, 017h, 000h
        add    esp, 18h
        pop    edx
        pop    ebx
loc_4_0360:
        inc    ebx
        cmp    ebx, 8
        jl     SHORT loc_4_032C
loc_4_0366:
        ret
loc_4_0367:
        xor    edx, edx
        db  0A1h, 09Ah, 017h, 000h, 000h
        mov    bx, 12h
        cmp    ax, bx
        jae    SHORT loc_4_037A
        mov    ax, bx
loc_4_037A:
        div    bx
        mov    ecx, 12h
        db  0BFh, 0C5h, 019h, 000h, 000h
        rep stosb
        mov    eax, 5
        db  081h, 03Dh, 09Eh, 017h, 000h, 000h, 088h, 013h, 000h, 000h
        jae    SHORT loc_4_03B4
        db  0A1h, 09Eh, 017h, 000h, 000h
        shl    eax, 1
        or     eax, eax
        je     SHORT loc_4_03B4
        mov    ebx, 3e8h
        cdq
        div    ebx
        sub    eax, 2
        jae    SHORT loc_4_03B4
        xor    eax, eax
loc_4_03B4:
        cld
        xor    ecx, ecx
        db  0BFh, 044h, 019h, 000h, 000h
        db  08Bh, 034h, 085h, 000h, 000h, 000h, 000h
        mov    cl, byte ptr [esi]
        db  088h, 00Dh, 03Bh, 019h, 000h, 000h
        inc    esi
        rep movsb
        xor    eax, eax
        mov    al, 9
        db  0F7h, 005h, 076h, 017h, 000h, 000h, 001h, 000h, 000h, 000h
        je     SHORT loc_4_0409
        db  081h, 03Dh, 086h, 017h, 000h, 000h, 0F8h, 07Fh, 000h, 000h
        je     SHORT loc_4_0409
        db  0A1h, 086h, 017h, 000h, 000h
        je     SHORT loc_4_0409
        push   edx
        and    edx, 0
        db  066h, 08Bh, 015h, 088h, 017h, 000h, 000h
        mov    bx, 37h
        div    bx
        pop    edx
        and    eax, 0ffffh
loc_4_0409:
        db  0A3h, 040h, 019h, 000h, 000h
        xor    ax, ax
        db  08Ah, 01Dh, 0C5h, 019h, 000h, 000h
        db  0A0h, 03Bh, 019h, 000h, 000h
        div    bl
        cmp    al, 0
        jne    SHORT loc_4_0424
        mov    al, 1
loc_4_0424:
        xor    edx, edx
        mov    dl, al
        mov    cl, dl
        xor    ebx, ebx
        db  08Ah, 01Dh, 03Bh, 019h, 000h, 000h
        db  0BFh, 044h, 019h, 000h, 000h
        add    edi, edx
        sub    bl, cl
        mov    al, 1
loc_4_043D:
        cmp    bl, 0
        je     SHORT loc_4_045C
        add    byte ptr [edi], al
        inc    edi
        dec    cl
        dec    bl
        cmp    cl, 0
        jne    SHORT loc_4_043D
        mov    cl, dl
        db  03Ah, 005h, 0C5h, 019h, 000h, 000h
        je     SHORT loc_4_043D
        inc    al
        jmp    SHORT loc_4_043D
loc_4_045C:
        ret
        db  000h, 000h, 000h
sub_4_0460:
        db  066h, 083h, 03Dh, 0FCh, 019h, 000h, 000h, 000h
        je     SHORT loc_4_049F
        db  066h, 083h, 03Dh, 0FEh, 019h, 000h, 000h, 000h
        jne    SHORT loc_4_049F
        xor    ebx, ebx
        db  08Bh, 01Dh, 0F4h, 017h, 000h, 000h
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        db  02Bh, 005h, 008h, 01Ah, 000h, 000h
        cmp    eax, ebx
        jb     SHORT loc_4_049F
        db  08Bh, 01Dh, 010h, 01Ah, 000h, 000h
        db  0BEh, 032h, 008h, 000h, 000h
        xor    edx, edx
        db  0CDh, 020h, 00Fh, 000h, 001h, 000h
loc_4_049F:
        db  080h, 03Dh, 023h, 01Ah, 000h, 000h, 000h
        je     SHORT loc_4_04D3
        xor    ebx, ebx
        db  08Bh, 01Dh, 0F4h, 017h, 000h, 000h
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        db  02Bh, 005h, 00Ch, 01Ah, 000h, 000h
        cmp    eax, ebx
        jb     SHORT loc_4_04D3
        db  08Bh, 01Dh, 010h, 01Ah, 000h, 000h
        db  0BEh, 01Eh, 00Ah, 000h, 000h
        xor    edx, edx
        db  0CDh, 020h, 00Fh, 000h, 001h, 000h
loc_4_04D3:
        ret
loc_4_04D4:
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    dword ptr [ebp + 1ch], 0f00h
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 1ch]
        db  066h, 0A3h, 020h, 01Ah, 000h, 000h
        mov    ebx, dword ptr [ebp + 10h]
        db  088h, 03Dh, 022h, 01Ah, 000h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        db  08Bh, 01Dh, 0F8h, 019h, 000h, 000h
        db  066h, 0A1h, 020h, 01Ah, 000h, 000h
        cmp    al, 7
        je     SHORT loc_4_054B
        cmp    al, 4
        jb     SHORT loc_4_054B
        db  083h, 03Dh, 0FCh, 017h, 000h, 000h, 003h
        jne    SHORT loc_4_058B
        db  0A1h, 000h, 018h, 000h, 000h
        call   sub_4_0AD7
        jmp    SHORT loc_4_058B
loc_4_054B:
        db  083h, 03Dh, 0F0h, 017h, 000h, 000h, 003h
        jne    SHORT loc_4_0560
        db  0A1h, 0F4h, 017h, 000h, 000h
        call   sub_4_0AD7
        jmp    SHORT loc_4_058B
loc_4_0560:
        db  083h, 03Dh, 0F0h, 017h, 000h, 000h, 001h
        jne    SHORT loc_4_0575
        mov    edx, 42656570h
        call   sub_4_058C
        jmp    SHORT loc_4_058B
loc_4_0575:
        db  083h, 03Dh, 0F0h, 017h, 000h, 000h, 002h
        jne    SHORT loc_4_058B
        db  08Bh, 015h, 0F4h, 017h, 000h, 000h
        call   sub_4_0A69
        jmp    SHORT loc_4_058B
loc_4_058B:
        ret
sub_4_058C:
        db  066h, 083h, 03Dh, 0FEh, 019h, 000h, 000h, 000h
        jne    NEAR PTR loc_4_0830
        db  066h, 0C7h, 005h, 0FCh, 019h, 000h, 000h, 000h, 000h
        cmp    edx, 0
        je     NEAR PTR loc_4_0830
        push   eax
        push   ecx
        db  089h, 015h, 004h, 01Ah, 000h, 000h
        mov    ecx, edx
        db  089h, 01Dh, 010h, 01Ah, 000h, 000h
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    dword ptr [ebp + 1ch], 300h
        xor    ebx, ebx
        db  08Ah, 03Dh, 022h, 01Ah, 000h, 000h
        mov    dword ptr [ebp + 10h], ebx
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        push   dword ptr [ebp + 14h]
        cmp    cl, 0
        je     SHORT loc_4_066E
        mov    dword ptr [ebp + 1ch], 200h
        xor    ebx, ebx
        db  08Ah, 03Dh, 022h, 01Ah, 000h, 000h
        mov    dword ptr [ebp + 10h], ebx
        mov    dword ptr [ebp + 14h], 4fh
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    dword ptr [ebp + 1ch], 800h
        xor    ebx, ebx
        db  08Ah, 03Dh, 022h, 01Ah, 000h, 000h
        mov    dword ptr [ebp + 10h], ebx
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 1ch]
        db  0A2h, 003h, 01Ah, 000h, 000h
        mov    eax, 0a00h
        mov    al, cl
        mov    dword ptr [ebp + 1ch], eax
        xor    ebx, ebx
        db  08Ah, 03Dh, 022h, 01Ah, 000h, 000h
        mov    dword ptr [ebp + 10h], ebx
        mov    dword ptr [ebp + 18h], 1
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  066h, 083h, 00Dh, 0FCh, 019h, 000h, 000h, 001h
loc_4_066E:
        shr    ecx, 8
        cmp    cl, 0
        je     SHORT loc_4_06EE
        mov    dword ptr [ebp + 1ch], 200h
        xor    ebx, ebx
        db  08Ah, 03Dh, 022h, 01Ah, 000h, 000h
        mov    dword ptr [ebp + 10h], ebx
        mov    dword ptr [ebp + 14h], 4eh
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    dword ptr [ebp + 1ch], 800h
        xor    ebx, ebx
        db  08Ah, 03Dh, 022h, 01Ah, 000h, 000h
        mov    dword ptr [ebp + 10h], ebx
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 1ch]
        db  0A2h, 002h, 01Ah, 000h, 000h
        mov    eax, 0a00h
        mov    al, cl
        mov    dword ptr [ebp + 1ch], eax
        xor    ebx, ebx
        db  08Ah, 03Dh, 022h, 01Ah, 000h, 000h
        mov    dword ptr [ebp + 10h], ebx
        mov    dword ptr [ebp + 18h], 1
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  066h, 083h, 00Dh, 0FCh, 019h, 000h, 000h, 002h
loc_4_06EE:
        shr    ecx, 8
        cmp    cl, 0
        je     SHORT loc_4_076E
        mov    dword ptr [ebp + 1ch], 200h
        xor    ebx, ebx
        db  08Ah, 03Dh, 022h, 01Ah, 000h, 000h
        mov    dword ptr [ebp + 10h], ebx
        mov    dword ptr [ebp + 14h], 4dh
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    dword ptr [ebp + 1ch], 800h
        xor    ebx, ebx
        db  08Ah, 03Dh, 022h, 01Ah, 000h, 000h
        mov    dword ptr [ebp + 10h], ebx
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 1ch]
        db  0A2h, 001h, 01Ah, 000h, 000h
        mov    eax, 0a00h
        mov    al, cl
        mov    dword ptr [ebp + 1ch], eax
        xor    ebx, ebx
        db  08Ah, 03Dh, 022h, 01Ah, 000h, 000h
        mov    dword ptr [ebp + 10h], ebx
        mov    dword ptr [ebp + 18h], 1
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  066h, 083h, 00Dh, 0FCh, 019h, 000h, 000h, 004h
loc_4_076E:
        shr    ecx, 8
        cmp    cl, 0
        je     SHORT loc_4_07EE
        mov    dword ptr [ebp + 1ch], 200h
        xor    ebx, ebx
        db  08Ah, 03Dh, 022h, 01Ah, 000h, 000h
        mov    dword ptr [ebp + 10h], ebx
        mov    dword ptr [ebp + 14h], 4ch
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    dword ptr [ebp + 1ch], 800h
        xor    ebx, ebx
        db  08Ah, 03Dh, 022h, 01Ah, 000h, 000h
        mov    dword ptr [ebp + 10h], ebx
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 1ch]
        db  0A2h, 000h, 01Ah, 000h, 000h
        mov    eax, 0a00h
        mov    al, cl
        mov    dword ptr [ebp + 1ch], eax
        xor    ebx, ebx
        db  08Ah, 03Dh, 022h, 01Ah, 000h, 000h
        mov    dword ptr [ebp + 10h], ebx
        mov    dword ptr [ebp + 18h], 1
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  066h, 083h, 00Dh, 0FCh, 019h, 000h, 000h, 008h
loc_4_07EE:
        mov    dword ptr [ebp + 1ch], 200h
        xor    ebx, ebx
        db  08Ah, 03Dh, 022h, 01Ah, 000h, 000h
        mov    dword ptr [ebp + 10h], ebx
        pop    dword ptr [ebp + 14h]
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        db  0A3h, 008h, 01Ah, 000h, 000h
        pop    ecx
        pop    eax
loc_4_0830:
        clc
        ret
loc_4_0832:
        db  066h, 0C7h, 005h, 0FEh, 019h, 000h, 000h, 001h, 000h
        pushfd
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    dword ptr [ebp + 1ch], 300h
        xor    ebx, ebx
        db  08Ah, 03Dh, 022h, 01Ah, 000h, 000h
        mov    dword ptr [ebp + 10h], ebx
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        push   dword ptr [ebp + 14h]
        db  066h, 0F7h, 005h, 0FCh, 019h, 000h, 000h, 001h, 000h
        je     SHORT loc_4_08CA
        mov    dword ptr [ebp + 1ch], 200h
        xor    ebx, ebx
        db  08Ah, 03Dh, 022h, 01Ah, 000h, 000h
        mov    dword ptr [ebp + 10h], ebx
        mov    dword ptr [ebp + 14h], 4fh
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    eax, 0a00h
        db  0A0h, 003h, 01Ah, 000h, 000h
        mov    dword ptr [ebp + 1ch], eax
        xor    ebx, ebx
        db  08Ah, 03Dh, 022h, 01Ah, 000h, 000h
        mov    dword ptr [ebp + 10h], ebx
        mov    dword ptr [ebp + 18h], 1
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
loc_4_08CA:
        db  066h, 0F7h, 005h, 0FCh, 019h, 000h, 000h, 002h, 000h
        je     SHORT loc_4_0923
        mov    dword ptr [ebp + 1ch], 200h
        xor    ebx, ebx
        db  08Ah, 03Dh, 022h, 01Ah, 000h, 000h
        mov    dword ptr [ebp + 10h], ebx
        mov    dword ptr [ebp + 14h], 4eh
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    eax, 0a00h
        db  0A0h, 002h, 01Ah, 000h, 000h
        mov    dword ptr [ebp + 1ch], eax
        xor    ebx, ebx
        db  08Ah, 03Dh, 022h, 01Ah, 000h, 000h
        mov    dword ptr [ebp + 10h], ebx
        mov    dword ptr [ebp + 18h], 1
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
loc_4_0923:
        db  066h, 0F7h, 005h, 0FCh, 019h, 000h, 000h, 004h, 000h
        je     SHORT loc_4_097C
        mov    dword ptr [ebp + 1ch], 200h
        xor    ebx, ebx
        db  08Ah, 03Dh, 022h, 01Ah, 000h, 000h
        mov    dword ptr [ebp + 10h], ebx
        mov    dword ptr [ebp + 14h], 4dh
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    eax, 0a00h
        db  0A0h, 001h, 01Ah, 000h, 000h
        mov    dword ptr [ebp + 1ch], eax
        xor    ebx, ebx
        db  08Ah, 03Dh, 022h, 01Ah, 000h, 000h
        mov    dword ptr [ebp + 10h], ebx
        mov    dword ptr [ebp + 18h], 1
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
loc_4_097C:
        db  066h, 0F7h, 005h, 0FCh, 019h, 000h, 000h, 008h, 000h
        je     SHORT loc_4_09D5
        mov    dword ptr [ebp + 1ch], 200h
        xor    ebx, ebx
        db  08Ah, 03Dh, 022h, 01Ah, 000h, 000h
        mov    dword ptr [ebp + 10h], ebx
        mov    dword ptr [ebp + 14h], 4ch
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    eax, 0a00h
        db  0A0h, 000h, 01Ah, 000h, 000h
        mov    dword ptr [ebp + 1ch], eax
        xor    ebx, ebx
        db  08Ah, 03Dh, 022h, 01Ah, 000h, 000h
        mov    dword ptr [ebp + 10h], ebx
        mov    dword ptr [ebp + 18h], 1
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
loc_4_09D5:
        mov    dword ptr [ebp + 1ch], 200h
        xor    ebx, ebx
        db  08Ah, 03Dh, 022h, 01Ah, 000h, 000h
        mov    dword ptr [ebp + 10h], ebx
        pop    dword ptr [ebp + 14h]
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        popfd
        db  066h, 0C7h, 005h, 0FCh, 019h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 0FEh, 019h, 000h, 000h, 000h, 000h
        ret
loc_4_0A1E:
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    dword ptr [ebp + 1ch], 0b00h
        mov    dword ptr [ebp + 10h], 0
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        db  0C6h, 005h, 023h, 01Ah, 000h, 000h, 000h
        ret
sub_4_0A69:
        db  089h, 01Dh, 010h, 01Ah, 000h, 000h
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    dword ptr [ebp + 1ch], 0b00h
        mov    dword ptr [ebp + 10h], 1fh
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        db  0A3h, 00Ch, 01Ah, 000h, 000h
        db  0C6h, 005h, 023h, 01Ah, 000h, 000h, 001h
        ret
loc_4_0AC5:
        mov    eax, 11h
        db  08Bh, 01Dh, 0F8h, 019h, 000h, 000h
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        ret
sub_4_0AD7:
        push   eax
        mov    eax, 10h
        db  08Bh, 01Dh, 0F8h, 019h, 000h, 000h
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        mov    eax, 0
        db  08Bh, 01Dh, 0F8h, 019h, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 00Ah, 000h
        pop    eax
        xor    edx, edx
        db  0BEh, 0C5h, 00Ah, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        ret
        db  000h, 000h, 000h
loc_4_0B0C:
        push   eax
        db  0F7h, 005h, 0C0h, 017h, 000h, 000h, 010h, 000h, 000h, 000h
        je     SHORT loc_4_0B29
        mov    eax, 2
        mov    ecx, 1
        db  0CDh, 020h, 005h, 000h, 037h, 000h
loc_4_0B29:
        mov    ebx, 8001h
        db  0BFh, 0BCh, 017h, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 037h, 000h
        pop    eax
        ret
loc_4_0B3B:
        pushad
        mov    esi, edx
        xor    ebx, ebx
        db  08Bh, 02Dh, 00Ch, 017h, 000h, 000h
        cmp    byte ptr [ebp], bl
        je     SHORT loc_4_0B95
        db  08Bh, 02Dh, 014h, 017h, 000h, 000h
        cmp    esi, dword ptr [ebp]
        je     SHORT loc_4_0B95
        db  08Bh, 02Dh, 014h, 017h, 000h, 000h
        mov    dword ptr [ebp], esi
loc_4_0B5F:
        db  08Bh, 02Dh, 010h, 017h, 000h, 000h
        cmp    dword ptr [ebp + ebx*8], 0ffffh
        je     SHORT loc_4_0B8F
        mov    eax, dword ptr [ebp + ebx*8]
        mov    ecx, dword ptr [ebp + ebx*8 + 4]
        push   ebx
        push   edx
        push   esi
        push   0
        push   0
        push   esi
        push   1
        push   ecx
        push   eax
        db  0CDh, 020h, 006h, 000h, 017h, 000h
        add    esp, 18h
        pop    esi
        pop    edx
        pop    ebx
loc_4_0B8F:
        inc    ebx
        cmp    ebx, 8
        jl     SHORT loc_4_0B5F
loc_4_0B95:
        popad
        ret
        db  000h
loc_4_0B98:
        pushad
        db  0C7h, 005h, 040h, 01Ah, 000h, 000h, 000h, 000h, 000h, 000h
        call   sub_4_00B0
        call   sub_4_0188
        call   sub_4_0460
        db  083h, 03Dh, 040h, 01Ah, 000h, 000h, 0FFh
        je     SHORT loc_4_0BD3
        mov    eax, 37h
        xor    edx, edx
        db  0BEh, 098h, 00Bh, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 040h, 01Ah, 000h, 000h
loc_4_0BD3:
        popad
        ret
        db  000h, 000h, 000h
_PTEXT ends
        end
