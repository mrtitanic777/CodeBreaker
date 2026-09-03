; vmd_lcode_01.asm -- VMD object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

loc_1_0000:
        db  080h, 03Dh, 098h, 00Ah, 000h, 000h, 0FFh
        jne    SHORT loc_1_004D
        db  08Bh, 015h, 094h, 00Ah, 000h, 000h
        add    edx, ebx
        or     byte ptr [edx], 40h
        and    byte ptr [edx], 0eeh
        mov    word ptr [edx + 2], 0ffffh
        mov    word ptr [edx + 4], 0ffffh
        mov    word ptr [edx + 6], 0
        mov    word ptr [edx + 8], 77ffh
        mov    word ptr [edx + 0ah], 7700h
        lea    edi, [edx + 1ch]
        db  0BEh, 050h, 00Ah, 000h, 000h
        mov    ecx, 10h
        cld
        rep movsd
        xor    eax, eax
        db  0CDh, 020h, 003h, 000h, 00Ah, 000h
loc_1_004D:
        clc
        ret
loc_1_004F:
        db  083h, 03Dh, 090h, 00Ah, 000h, 000h, 000h
        je     SHORT loc_1_00BC
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_1_00BC
        cmp    eax, 2
        ja     SHORT loc_1_00BC
        je     SHORT loc_1_00B7
        mov    ebx, dword ptr [ebp + 10h]
        db  08Bh, 015h, 094h, 00Ah, 000h, 000h
        add    edx, ebx
        mov    ecx, dword ptr [ebp + 18h]
        mov    edi, dword ptr [ebp]
        cmp    word ptr [edx + 10h], cx
        je     SHORT loc_1_0085
        db  0C6h, 005h, 0A0h, 00Ah, 000h, 000h, 003h
loc_1_0085:
        mov    word ptr [edx + 10h], cx
        rol    ecx, 10h
        mov    word ptr [edx + 12h], cx
        mov    ecx, dword ptr [edx + 14h]
        mov    dword ptr [edx + 18h], ecx
        mov    dword ptr [edx + 14h], edi
        cmp    dword ptr [edx + 0ch], 0
        jne    SHORT loc_1_00B7
        db  0BEh, 0C1h, 000h, 000h, 000h
        mov    eax, 400000h
        mov    ecx, 9
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        mov    dword ptr [edx + 0ch], esi
loc_1_00B7:
        and    byte ptr [ebp + 2ch], 0feh
        ret
loc_1_00BC:
        or     byte ptr [ebp + 2ch], 1
        ret
loc_1_00C1:
        mov    dword ptr [edx + 0ch], 0
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    edi, dword ptr [edx + 14h]
        xor    eax, eax
        btr    word ptr [edx], 0ch
        jb     SHORT loc_1_010D
        btr    word ptr [edx], 0bh
        jae    SHORT loc_1_0110
        mov    dword ptr [ebp + 1ch], 3
        mov    eax, 33h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    ax, word ptr [ebp + 18h]
        shl    eax, 10h
        mov    ax, word ptr [ebp + 14h]
loc_1_010D:
        mov    dword ptr [edx + 18h], eax
loc_1_0110:
        test   byte ptr [edx + 1], 2
        jne    SHORT loc_1_011B
loc_1_0116:
        mov    dword ptr [edx + 18h], edi
        jmp    SHORT loc_1_0184
loc_1_011B:
        test   byte ptr [edx + 1], 4
        jne    SHORT loc_1_0116
        test   byte ptr [edx + 1], 1
        je     SHORT loc_1_0116
        mov    ecx, dword ptr [edx + 18h]
loc_1_012A:
        mov    ax, cx
        add    cx, 8
        sub    ax, 8
        jae    SHORT loc_1_013A
        xor    ax, ax
loc_1_013A:
        cmp    cx, di
        ja     SHORT loc_1_0145
        mov    word ptr [edx + 18h], cx
        jmp    SHORT loc_1_0154
loc_1_0145:
        cmp    ax, di
        jbe    SHORT loc_1_0150
        mov    word ptr [edx + 18h], ax
        jmp    SHORT loc_1_0154
loc_1_0150:
        mov    word ptr [edx + 18h], di
loc_1_0154:
        shr    edi, 10h
        shr    ecx, 10h
        mov    ax, cx
        add    cx, 8
        sub    ax, 8
        jae    SHORT loc_1_016A
        xor    ax, ax
loc_1_016A:
        cmp    cx, di
        ja     SHORT loc_1_0175
        mov    word ptr [edx + 1ah], cx
        jmp    SHORT loc_1_0184
loc_1_0175:
        cmp    ax, di
        jbe    SHORT loc_1_0180
        mov    word ptr [edx + 1ah], ax
        jmp    SHORT loc_1_0184
loc_1_0180:
        mov    word ptr [edx + 1ah], di
loc_1_0184:
        mov    edi, dword ptr [edx + 18h]
        mov    cx, word ptr [edx + 10h]
        xor    esi, esi
        cmp    edi, dword ptr [edx + 14h]
        jne    SHORT loc_1_0196
        xchg   word ptr [edx + 12h], si
loc_1_0196:
        mov    word ptr [ebp + 14h], cx
        mov    word ptr [ebp + 4], si
        mov    word ptr [ebp + 1ch], 1
        mov    word ptr [ebp + 18h], di
        rol    edi, 10h
        mov    word ptr [ebp + 10h], di
        push   edx
        db  08Bh, 00Dh, 090h, 00Ah, 000h, 000h
        movzx  edx, cx
        shr    ecx, 10h
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        pop    edx
        mov    edi, dword ptr [edx + 14h]
        mov    ecx, dword ptr [edx + 18h]
        cmp    ecx, edi
        jne    NEAR PTR loc_1_012A
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        ret
        db  02Eh, 08Bh, 0C0h
loc_1_01F0:
        db  08Bh, 015h, 094h, 00Ah, 000h, 000h
        add    edx, ebx
        mov    eax, dword ptr [ebp + 1ch]
        cmp    ax, 0bh
        je     SHORT loc_1_0242
        cmp    ax, 3
        je     SHORT loc_1_0246
        or     ax, ax
        jne    SHORT loc_1_0214
        and    byte ptr [edx + 1], 0f8h
        or     byte ptr [edx + 1], 10h
loc_1_0214:
        cmp    ax, 21h
        jne    SHORT loc_1_0222
        and    byte ptr [edx + 1], 0f8h
        or     byte ptr [edx + 1], 10h
loc_1_0222:
        cmp    ax, 0ch
        jne    SHORT loc_1_022C
        or     byte ptr [edx + 1], 2
loc_1_022C:
        cmp    ax, 0ah
        jne    SHORT loc_1_0236
        or     byte ptr [edx + 1], 4
loc_1_0236:
        cmp    ax, 9
        jne    SHORT loc_1_0272
        or     byte ptr [edx + 1], 4
        jmp    SHORT loc_1_0272
loc_1_0242:
        or     byte ptr [edx + 1], 1
loc_1_0246:
        db  03Bh, 01Dh, 004h, 00Bh, 000h, 000h
        je     SHORT loc_1_0272
        db  080h, 03Dh, 099h, 00Ah, 000h, 000h, 0FFh
        jne    SHORT loc_1_0272
        db  0FEh, 005h, 0A0h, 00Ah, 000h, 000h
        db  080h, 03Dh, 0A0h, 00Ah, 000h, 000h, 003h
        jbe    SHORT loc_1_0272
        db  088h, 025h, 0A0h, 00Ah, 000h, 000h
        db  0CDh, 020h, 00Dh, 000h, 00Ah, 000h
loc_1_0272:
        stc
        ret
loc_1_0274:
        db  083h, 03Dh, 090h, 00Ah, 000h, 000h, 000h
        je     SHORT loc_1_02D9
        db  08Bh, 015h, 094h, 00Ah, 000h, 000h
        add    edx, ebx
        or     byte ptr [edx + 1], 10h
        db  03Bh, 01Dh, 004h, 00Bh, 000h, 000h
        je     SHORT loc_1_02D9
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 2
        db  08Bh, 00Dh, 090h, 00Ah, 000h, 000h
        movzx  edx, cx
        shr    ecx, 10h
        db  0CDh, 020h, 04Ch, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
loc_1_02D9:
        db  080h, 03Dh, 09Fh, 00Ah, 000h, 000h, 000h
        je     SHORT loc_1_031E
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    dword ptr [ebp + 1ch], 0
        mov    eax, 33h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
loc_1_031E:
        ret
        db  090h
loc_1_0320:
        movzx  eax, word ptr [ebp + 1ch]
        cmp    eax, 1
        jb     SHORT loc_1_0337
        je     SHORT loc_1_0356
        cmp    eax, 9
        je     SHORT loc_1_0362
        cmp    eax, 0ah
        je     SHORT loc_1_03AA
loc_1_0335:
        stc
        ret
loc_1_0337:
        call   0
        db  00Fh, 0B6h, 005h, 09Eh, 00Ah, 000h, 000h
        mov    word ptr [ebp + 1ch], ax
        db  0BEh, 07Ah, 004h, 000h, 000h
        xor    eax, eax
        db  0CDh, 020h, 00Dh, 000h, 001h, 000h
        jmp    SHORT loc_1_0335
loc_1_0356:
        db  08Bh, 015h, 094h, 00Ah, 000h, 000h
        or     byte ptr [edx + ebx], 10h
        jmp    SHORT loc_1_0335
loc_1_0362:
        db  08Bh, 015h, 094h, 00Ah, 000h, 000h
        add    edx, ebx
        test   word ptr [edx], 4
        jne    SHORT loc_1_0335
        mov    ax, word ptr [ebp + 10h]
        mov    word ptr [edx + 2], ax
        mov    ax, word ptr [ebp + 18h]
        mov    word ptr [edx + 2], ax
        xor    eax, eax
        db  0CDh, 020h, 003h, 000h, 00Ah, 000h
        xchg   esi, eax
        mov    ax, 3814h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
        lea    edi, [edx + 1ch]
        mov    ecx, 10h
        cld
        rep movsd
        db  039h, 01Dh, 004h, 00Bh, 000h, 000h
        je     SHORT loc_1_0335
        clc
        ret
loc_1_03AA:
        db  08Bh, 015h, 094h, 00Ah, 000h, 000h
        add    edx, ebx
        test   word ptr [edx], 4
        jne    NEAR PTR loc_1_0335
        mov    ax, word ptr [ebp + 10h]
        mov    word ptr [edx + 6], ax
        mov    ax, word ptr [ebp + 18h]
        mov    word ptr [edx + 8], ax
        mov    ax, word ptr [ebp + 14h]
        mov    word ptr [edx + 0ah], ax
        db  039h, 01Dh, 004h, 00Bh, 000h, 000h
        je     NEAR PTR loc_1_0335
        cmp    word ptr [edx + 6], 0
        je     SHORT loc_1_03F1
        or     eax, 0ffffffffh
        db  0CDh, 020h, 003h, 000h, 00Ah, 000h
loc_1_03F1:
        clc
        ret
sub_1_03F3:
        db  080h, 03Dh, 098h, 00Ah, 000h, 000h, 0FFh
        jne    SHORT loc_1_0440
        test   ebx, ebx
        je     SHORT loc_1_0440
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_1_0440
        db  0A1h, 094h, 00Ah, 000h, 000h
        db  08Bh, 00Dh, 090h, 00Ah, 000h, 000h
        or     ecx, ecx
        jne    SHORT loc_1_041F
        test   word ptr [eax + ebx], 10h
        je     SHORT loc_1_0440
loc_1_041F:
        or     byte ptr [eax + ebx], 80h
        bts    word ptr [eax + ebx], 1
        jb     SHORT loc_1_0440
        mov    eax, 10h
        mov    ecx, 3
        db  0BEh, 07Ah, 004h, 000h, 000h
        db  0CDh, 020h, 014h, 080h, 001h, 000h
loc_1_0440:
        ret
loc_1_0441:
        db  080h, 03Dh, 098h, 00Ah, 000h, 000h, 0FFh
        jne    SHORT loc_1_0453
        db  003h, 01Dh, 094h, 00Ah, 000h, 000h
        or     byte ptr [ebx], 20h
loc_1_0453:
        clc
        ret
loc_1_0455:
        db  080h, 03Dh, 098h, 00Ah, 000h, 000h, 0FFh
        jne    SHORT loc_1_0478
        db  0A1h, 094h, 00Ah, 000h, 000h
        and    byte ptr [eax + ebx], 0dfh
        btr    word ptr [eax + ebx], 3
        jae    SHORT loc_1_0478
        and    byte ptr [eax + ebx], 0fdh
        call   sub_1_03F3
loc_1_0478:
        clc
        ret
loc_1_047A:
        db  08Bh, 015h, 094h, 00Ah, 000h, 000h
        add    edx, ebx
loc_1_0482:
        test   byte ptr [edx], 20h
        jne    NEAR PTR loc_1_0603
        or     byte ptr [edx], 4
        and    byte ptr [edx], 7fh
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        db  083h, 03Dh, 090h, 00Ah, 000h, 000h, 000h
        je     SHORT loc_1_04EB
        push   edx
        mov    ax, 2
        or     byte ptr [edx + 1], 8
        db  03Bh, 01Dh, 004h, 00Bh, 000h, 000h
        jne    SHORT loc_1_04C6
        and    byte ptr [edx + 1], 0f7h
        inc    ax
loc_1_04C6:
        mov    word ptr [ebp + 1ch], ax
        db  08Bh, 00Dh, 090h, 00Ah, 000h, 000h
        movzx  edx, cx
        shr    ecx, 10h
        db  0CDh, 020h, 04Ch, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        pop    edx
        and    byte ptr [edx], 0f9h
        jmp    NEAR PTR loc_1_05E2
loc_1_04EB:
        mov    word ptr [ebp + 1ch], 9
        mov    ax, word ptr [edx + 2]
        mov    word ptr [ebp + 10h], ax
        mov    ax, word ptr [edx + 4]
        mov    word ptr [ebp + 18h], ax
        push   fs
        mov    ax, ds
        mov    fs, ax
        lea    esi, [edx + 1ch]
        mov    ecx, 40h
        stc
        db  0CDh, 020h, 008h, 000h, 006h, 000h
        pop    fs
        jb     NEAR PTR loc_1_05E2
        mov    eax, edi
        mov    word ptr [ebp + 14h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 38h], ax
        db  03Bh, 01Dh, 004h, 00Bh, 000h, 000h
        je     SHORT loc_1_0594
        or     byte ptr [edx], 1
        mov    ecx, 8
        cld
        movzx  eax, di
        shr    edi, 0ch
        and    edi, 0fffffff0h
        add    edi, eax
        push   ecx
        or     eax, 0ffffffffh
        rep stosd
        pop    ecx
        xor    eax, eax
        rep stosd
        mov    eax, 33h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        cmp    word ptr [edx + 6], 0
        je     SHORT loc_1_0571
        or     eax, 0ffffffffh
        db  0CDh, 020h, 003h, 000h, 00Ah, 000h
        jmp    SHORT loc_1_05D3
loc_1_0571:
        mov    word ptr [ebp + 1ch], 0ah
        mov    word ptr [ebp + 18h], 0ffffh
        xor    eax, eax
        mov    word ptr [ebp + 10h], ax
        mov    word ptr [ebp + 14h], ax
        mov    eax, 33h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        jmp    SHORT loc_1_05D3
loc_1_0594:
        and    byte ptr [edx], 0feh
        mov    eax, 33h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 0ah
        mov    ax, word ptr [edx + 6]
        mov    word ptr [ebp + 10h], ax
        mov    ax, word ptr [edx + 8]
        mov    word ptr [ebp + 18h], ax
        mov    ax, word ptr [edx + 0ah]
        mov    word ptr [ebp + 14h], ax
        mov    eax, 33h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        xor    eax, eax
        db  0CDh, 020h, 003h, 000h, 00Ah, 000h
loc_1_05D3:
        and    byte ptr [edx], 0f9h
        clc
        mov    ecx, 40h
        db  0CDh, 020h, 009h, 000h, 006h, 000h
loc_1_05E2:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        btr    word ptr [edx], 7
        jb     NEAR PTR loc_1_0482
        ret
loc_1_0603:
        or     byte ptr [edx], 8
        ret
loc_1_0607:
        db  0CDh, 020h, 047h, 000h, 001h, 000h
        mov    esi, ebx
        db  003h, 035h, 094h, 00Ah, 000h, 000h
        test   word ptr [esi], 40h
        jne    SHORT loc_1_062C
        mov    eax, 33h
        db  0CDh, 020h, 042h, 000h, 001h, 000h
        je     SHORT loc_1_066F
        or     byte ptr [esi], 40h
loc_1_062C:
        movzx  eax, word ptr [ebp + 1ch]
        cmp    eax, 14h
        je     SHORT loc_1_066B
        cmp    eax, 18h
        je     SHORT loc_1_066B
        cmp    eax, 19h
        je     SHORT loc_1_066B
        cmp    eax, 1fh
        je     SHORT loc_1_066B
        cmp    eax, 9
        je     SHORT loc_1_068A
        cmp    eax, 0ch
        je     SHORT loc_1_06CD
        cmp    eax, 15h
        je     NEAR PTR sub_1_075B
        cmp    eax, 16h
        je     NEAR PTR loc_1_0784
        cmp    eax, 17h
        je     NEAR PTR loc_1_07FE
        jmp    SHORT sub_1_0670
loc_1_066B:
        or     byte ptr [ebp + 2ch], 1
loc_1_066F:
        ret
sub_1_0670:
        push   eax
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    eax, 33h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        pop    eax
        ret
loc_1_068A:
        push   fs
        mov    ax, 3814h
        db  0CDh, 020h, 007h, 000h, 006h, 000h
        mov    ecx, 40h
        stc
        db  0CDh, 020h, 008h, 000h, 006h, 000h
        jb     SHORT loc_1_06CA
        push   word ptr [ebp + 5ch]
        push   dword ptr [ebp + 14h]
        mov    word ptr [ebp + 14h], di
        shr    edi, 10h
        mov    word ptr [ebp + 5ch], di
        call   sub_1_0670
        pop    word ptr [ebp + 14h]
        pop    word ptr [ebp + 5ch]
        clc
        db  0CDh, 020h, 009h, 000h, 006h, 000h
loc_1_06CA:
        pop    fs
        ret
loc_1_06CD:
        push   fs
        mov    ax, 3814h
        db  0CDh, 020h, 007h, 000h, 006h, 000h
        db  003h, 01Dh, 094h, 00Ah, 000h, 000h
        mov    word ptr [ebx + 5ch], fs
        mov    dword ptr [ebx + 5eh], esi
        db  066h, 0A1h, 09Ah, 00Ah, 000h, 000h
        db  066h, 08Bh, 00Dh, 09Ch, 00Ah, 000h, 000h
        xchg   word ptr [ebp + 5ch], ax
        xchg   word ptr [ebp + 14h], cx
        call   sub_1_0670
        mov    word ptr [ebp + 5ch], ax
        mov    word ptr [ebp + 14h], cx
        pop    fs
        ret
loc_1_070B:
        db  0CDh, 020h, 04Ah, 000h, 001h, 000h
        db  003h, 01Dh, 094h, 00Ah, 000h, 000h
        mov    cx, word ptr [ebx + 5ch]
        test   cx, cx
        je     SHORT loc_1_075A
        mov    edx, dword ptr [ebx + 5eh]
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        ret
loc_1_075A:
        ret
sub_1_075B:
        push   eax
        push   ebx
        db  003h, 01Dh, 094h, 00Ah, 000h, 000h
        mov    ax, word ptr [ebx + 62h]
        test   ax, ax
        jne    SHORT loc_1_0779
        call   sub_1_0670
        mov    ax, word ptr [ebp + 10h]
        mov    word ptr [ebx + 62h], ax
loc_1_0779:
        add    ax, 6
        mov    word ptr [ebp + 10h], ax
        pop    ebx
        pop    eax
        ret
loc_1_0784:
        push   fs
        push   dword ptr [ebp + 14h]
        push   word ptr [ebp + 5ch]
        mov    ax, 3814h
        db  0CDh, 020h, 007h, 000h, 006h, 000h
        mov    edi, ebx
        db  003h, 03Dh, 094h, 00Ah, 000h, 000h
        movzx  ecx, word ptr [edi + 62h]
        test   ecx, ecx
        jne    SHORT loc_1_07C1
        mov    ecx, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp + 1ch]
        mov    word ptr [ebp + 1ch], 15h
        call   sub_1_075B
        mov    dword ptr [ebp + 1ch], eax
        xchg   dword ptr [ebp + 10h], ecx
        movzx  ecx, cx
loc_1_07C1:
        push   edi
        clc
        db  0CDh, 020h, 008h, 000h, 006h, 000h
        jb     SHORT loc_1_07FB
        mov    dword ptr [ebp + 14h], edi
        shr    edi, 10h
        mov    word ptr [ebp + 5ch], di
        pop    edi
        call   sub_1_0670
        stc
        db  0CDh, 020h, 009h, 000h, 006h, 000h
        add    esi, ecx
        mov    ax, word ptr [edi + 5ch]
        mov    word ptr [esi], ax
        mov    eax, dword ptr [edi + 5eh]
        mov    dword ptr [esi + 2], eax
loc_1_07F1:
        pop    word ptr [ebp + 5ch]
        pop    dword ptr [ebp + 14h]
        pop    fs
        ret
loc_1_07FB:
        pop    edi
        jmp    SHORT loc_1_07F1
loc_1_07FE:
        push   fs
        push   word ptr [ebp + 5ch]
        push   dword ptr [ebp + 14h]
        mov    ax, 3814h
        db  0CDh, 020h, 007h, 000h, 006h, 000h
        mov    edi, ebx
        db  003h, 03Dh, 094h, 00Ah, 000h, 000h
        movzx  ecx, word ptr [edi + 62h]
        test   ecx, ecx
        jne    SHORT loc_1_083B
        mov    ecx, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp + 1ch]
        mov    word ptr [ebp + 1ch], 15h
        call   sub_1_075B
        mov    dword ptr [ebp + 1ch], eax
        xchg   dword ptr [ebp + 10h], ecx
        movzx  ecx, cx
loc_1_083B:
        add    esi, ecx
        mov    ax, word ptr [esi]
        mov    word ptr [edi + 5ch], ax
        mov    eax, dword ptr [esi + 2]
        mov    dword ptr [edi + 5eh], eax
        sub    esi, ecx
        stc
        db  0CDh, 020h, 008h, 000h, 006h, 000h
        jb     SHORT loc_1_086C
        mov    dword ptr [ebp + 14h], edi
        shr    edi, 10h
        mov    word ptr [ebp + 5ch], di
        pop    edi
        call   sub_1_0670
        clc
        db  0CDh, 020h, 009h, 000h, 006h, 000h
loc_1_086C:
        pop    dword ptr [ebp + 14h]
        pop    word ptr [ebp + 38h]
        pop    fs
        ret
        db  000h, 000h
VMD_Control:
        cmp    eax, 0
        db  00Fh, 084h, 07Fh, 0F7h, 0FEh, 0FFh
        cmp    eax, 1
        db  00Fh, 084h, 08Ah, 0F9h, 0FEh, 0FFh
        cmp    eax, 2
        db  00Fh, 084h, 0CDh, 0F8h, 0FEh, 0FFh
        cmp    eax, 0fh
        je     SHORT loc_1_08B5
        cmp    eax, 10h
        je     NEAR PTR loc_1_0441
        cmp    eax, 11h
        je     NEAR PTR loc_1_0455
        cmp    eax, 9
        je     NEAR PTR loc_1_0274
        clc
        ret
loc_1_08B5:
        test   edx, edx
        je     SHORT loc_1_08BE
        cmp    edx, 0ch
        jne    SHORT loc_1_0939
loc_1_08BE:
        db  087h, 01Dh, 004h, 00Bh, 000h, 000h
        db  080h, 03Dh, 00Ah, 00Bh, 000h, 000h, 0FFh
        je     SHORT loc_1_0939
        call   sub_1_03F3
        push   ebx
        db  08Bh, 01Dh, 004h, 00Bh, 000h, 000h
        call   sub_1_03F3
        pop    ebx
        db  00Fh, 0B6h, 035h, 00Bh, 00Bh, 000h, 000h
        test   esi, esi
        jne    SHORT loc_1_0923
        db  080h, 03Dh, 008h, 00Bh, 000h, 000h, 0FFh
        jne    SHORT loc_1_0939
        db  0A1h, 000h, 00Bh, 000h, 000h
        db  080h, 03Dh, 009h, 00Bh, 000h, 000h, 003h
        je     SHORT loc_1_090F
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        test   ecx, 20h
        je     SHORT loc_1_0939
loc_1_090F:
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        db  08Bh, 01Dh, 004h, 00Bh, 000h, 000h
        db  0CDh, 020h, 002h, 000h, 003h, 000h
        jmp    SHORT loc_1_0939
loc_1_0923:
        mov    eax, 0fh
        mov    edx, 0eh
        db  08Bh, 01Dh, 004h, 00Bh, 000h, 000h
        db  0CDh, 020h, 093h, 000h, 001h, 000h
loc_1_0939:
        clc
        ret
VMD_Get_Version:
        mov    eax, 300h
        clc
        ret
VMD_Set_Mouse_Type:
        db  080h, 03Dh, 00Ah, 00Bh, 000h, 000h, 0FFh
        jne    NEAR PTR loc_1_09EE
        pushad
        db  088h, 00Dh, 009h, 00Bh, 000h, 000h
        test   eax, eax
        jns    SHORT loc_1_0964
        neg    eax
        db  0CDh, 020h, 00Eh, 000h, 003h, 000h
        jb     SHORT loc_1_09E1
loc_1_0964:
        cmp    al, 2
        jne    SHORT loc_1_096A
        mov    al, 9
loc_1_096A:
        db  0A2h, 00Ah, 00Bh, 000h, 000h
        db  080h, 03Dh, 009h, 00Bh, 000h, 000h, 004h
        je     SHORT loc_1_09DE
        db  080h, 03Dh, 009h, 00Bh, 000h, 000h, 082h
        jne    SHORT loc_1_098C
        db  0C6h, 005h, 009h, 00Bh, 000h, 000h, 002h
        mov    al, dl
        jmp    SHORT loc_1_0999
loc_1_098C:
        db  080h, 03Dh, 009h, 00Bh, 000h, 000h, 002h
        jne    SHORT loc_1_09B7
        sub    al, 5
        neg    al
loc_1_0999:
        push   eax
        db  0CDh, 020h, 000h, 000h, 00Eh, 000h
        test   eax, eax
        pop    eax
        je     SHORT loc_1_09B7
        db  0A2h, 00Bh, 00Bh, 000h, 000h
        movzx  eax, al
        xor    edx, edx
        db  0CDh, 020h, 001h, 000h, 00Eh, 000h
        jmp    SHORT loc_1_09DE
loc_1_09B7:
        db  00Fh, 0B6h, 03Dh, 00Ah, 00Bh, 000h, 000h
        db  066h, 089h, 03Dh, 00Ch, 00Bh, 000h, 000h
        db  0BFh, 00Ch, 00Bh, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 003h, 000h
        jb     SHORT loc_1_09E1
        db  0A3h, 000h, 00Bh, 000h, 000h
        db  0C6h, 005h, 008h, 00Bh, 000h, 000h, 0FFh
loc_1_09DE:
        popad
        clc
        ret
loc_1_09E1:
        db  0C6h, 005h, 00Ah, 00Bh, 000h, 000h, 0FFh
        popad
        xor    eax, eax
        inc    eax
        stc
        ret
loc_1_09EE:
        xor    eax, eax
        stc
        ret
VMD_Get_Mouse_Owner:
        db  08Bh, 01Dh, 004h, 00Bh, 000h, 000h
        ret
VMD_PM_API:
        movzx  eax, word ptr [ebp + 1ch]
        test   eax, eax
        jne    SHORT loc_1_0A0D
        mov    word ptr [ebp + 1ch], 300h
loc_1_0A07:
        and    word ptr [ebp + 2ch], 0fffeh
        ret
loc_1_0A0D:
        test   ah, ah
        jne    SHORT loc_1_0A16
        jmp    NEAR PTR loc_1_004F
loc_1_0A16:
        cmp    ax, 100h
        jne    SHORT loc_1_0A31
        movzx  eax, byte ptr [ebp + 11h]
        neg    eax
        movzx  ecx, byte ptr [ebp + 10h]
        mov    dl, byte ptr [ebp + 18h]
        db  0CDh, 020h, 001h, 000h, 00Ch, 000h
        jae    SHORT loc_1_0A07
loc_1_0A31:
        or     word ptr [ebp + 2ch], 1
        ret
        db  090h
loc_1_0A38:
        db  08Bh, 01Dh, 004h, 00Bh, 000h, 000h
        db  0CDh, 020h, 002h, 080h, 003h, 000h
loc_1_0A44:
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        db  0CDh, 020h, 003h, 080h, 003h, 000h
        db  0FFh, 03Fh, 0FFh, 01Fh, 0FFh, 00Fh, 0FFh, 007h, 0FFh, 003h, 0FFh, 001h, 0FFh, 000h, 07Fh, 000h
        db  03Fh, 000h, 01Fh, 000h, 0FFh, 001h, 0FFh, 010h, 0FFh, 030h, 07Fh, 0F8h, 07Fh, 0F8h, 07Fh, 0FCh
        db  000h, 000h, 000h, 040h, 000h, 060h, 000h, 070h, 000h, 078h, 000h, 07Ch, 000h, 07Eh, 000h, 07Fh
        db  080h, 07Fh, 000h, 07Ch, 000h, 06Ch, 000h, 046h, 000h, 006h, 000h, 003h, 000h, 003h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 03Bh, 009h, 000h, 000h, 042h, 009h, 000h, 000h, 0F2h, 009h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 00Ch, 000h, 003h, 000h, 000h, 000h, 056h, 04Dh, 044h, 020h
        db  020h, 020h, 020h, 020h, 000h, 000h, 000h, 034h, 078h, 008h, 000h, 000h, 0F9h, 009h, 000h, 000h
        db  0F9h, 009h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A4h, 00Ah, 000h, 000h, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0B0A:
        inc    dword ptr [eax]
loc_1_0B0C:
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        cmp    byte ptr [edx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esp
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    ah, dh
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
_LTEXT ends
        end
