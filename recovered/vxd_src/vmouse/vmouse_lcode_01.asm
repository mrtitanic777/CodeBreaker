; vmouse_lcode_01.asm -- VMOUSE object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

VMOUSE_Control:
        cmp    eax, 0fh
        db  00Fh, 084h, 0F7h, 0FFh, 0FEh, 0FFh
        cmp    eax, 8
        db  00Fh, 084h, 0CEh, 00Dh, 0FFh, 0FFh
        cmp    eax, 9
        db  00Fh, 084h, 0D8h, 006h, 0FFh, 0FFh
        cmp    eax, 0
        db  00Fh, 084h, 0DCh, 0FFh, 0FEh, 0FFh
        cmp    eax, 2
        db  00Fh, 084h, 040h, 001h, 0FFh, 0FFh
        cmp    eax, 1
        db  00Fh, 084h, 0A6h, 000h, 0FFh, 0FFh
        cmp    eax, 22h
        db  00Fh, 084h, 005h, 034h, 0FFh, 0FFh
        cmp    eax, 0ch
        db  00Fh, 084h, 065h, 000h, 0FFh, 0FFh
        cmp    eax, 6
        db  00Fh, 084h, 08Fh, 000h, 0FFh, 0FFh
        cmp    eax, 5
        db  00Fh, 084h, 0F2h, 00Fh, 0FFh, 0FFh
        cmp    eax, 5
        db  00Fh, 084h, 09Ah, 00Dh, 0FFh, 0FFh
        clc
        ret
        db  000h, 000h, 000h
VMD_Post_Absolute_Pointer_Message:
        pushad
        db  08Bh, 01Dh, 090h, 006h, 000h, 000h
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_1_009D
        db  08Bh, 015h, 08Ch, 006h, 000h, 000h
        add    edx, ebx
        bts    dword ptr [edx], 0
        jb     SHORT loc_1_009D
        db  089h, 035h, 068h, 007h, 000h, 000h
        db  089h, 03Dh, 06Ch, 007h, 000h, 000h
        db  0A2h, 070h, 007h, 000h, 000h
        db  0BEh, 04Eh, 00Bh, 000h, 000h
        jmp    SHORT loc_1_00D7
loc_1_009D:
        popad
        clc
        ret
VMD_Post_Pointer_Message:
        pushad
        db  08Bh, 01Dh, 090h, 006h, 000h, 000h
        mov    edx, ebx
        db  003h, 015h, 08Ch, 006h, 000h, 000h
        test   byte ptr [edx], 0c1h
        jne    SHORT loc_1_00FF
        or     byte ptr [edx], 1
        db  089h, 035h, 058h, 007h, 000h, 000h
        db  089h, 03Dh, 05Ch, 007h, 000h, 000h
        db  0A2h, 067h, 007h, 000h, 000h
        mov    esi, dword ptr [esp + 14h]
        db  089h, 035h, 060h, 007h, 000h, 000h
        db  0BEh, 0ECh, 009h, 000h, 000h
loc_1_00D7:
        db  080h, 03Dh, 071h, 007h, 000h, 000h, 000h
        je     SHORT loc_1_00EC
        db  0CDh, 020h, 011h, 000h, 00Dh, 000h
        db  066h, 0A3h, 064h, 007h, 000h, 000h
loc_1_00EC:
        mov    eax, 400000h
        mov    ecx, 9
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        mov    dword ptr [edx + 4], esi
loc_1_00FF:
        db  080h, 03Dh, 098h, 006h, 000h, 000h, 001h
        jne    SHORT loc_1_010E
        db  0CDh, 020h, 00Ch, 000h, 017h, 000h
loc_1_010E:
        popad
        clc
        ret
        db  000h, 000h, 000h
loc_1_0114:
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        push   edx
        mov    esi, 23ch
        mov    edi, 23eh
        mov    cl, 4
        mov    al, 90h
        mov    edx, edi
        out    dx, al
        jmp    SHORT loc_1_012E
loc_1_012E:
        mov    edx, esi
        in     al, dx
        jmp    SHORT loc_1_0133
loc_1_0133:
        ror    ax, cl
        mov    al, 0b0h
        mov    edx, edi
        out    dx, al
        jmp    SHORT loc_1_013D
loc_1_013D:
        mov    edx, esi
        in     al, dx
        jmp    SHORT loc_1_0142
loc_1_0142:
        rol    ax, cl
        movsx  eax, al
        push   eax
        mov    al, 0d0h
        mov    edx, edi
        out    dx, al
        jmp    SHORT loc_1_0150
loc_1_0150:
        mov    edx, esi
        in     al, dx
        jmp    SHORT loc_1_0155
loc_1_0155:
        ror    ax, cl
        mov    al, 0f0h
        mov    edx, edi
        out    dx, al
        jmp    SHORT loc_1_015F
loc_1_015F:
        mov    edx, esi
        in     al, dx
        jmp    SHORT loc_1_0164
loc_1_0164:
        mov    ch, al
        rol    ax, cl
        movsx  eax, al
        push   eax
        xor    al, al
        mov    edx, edi
        out    dx, al
        jmp    SHORT loc_1_0174
loc_1_0174:
        pop    edi
        pop    esi
        not    ch
        xor    al, al
        test   ch, 80h
        je     SHORT loc_1_0181
        or     al, 20h
loc_1_0181:
        test   ch, 20h
        je     SHORT loc_1_0188
        or     al, 10h
loc_1_0188:
        pop    edx
        movzx  ecx, al
        db  032h, 00Dh, 074h, 007h, 000h, 000h
        or     ecx, esi
        or     ecx, edi
        je     SHORT loc_1_01A3
        db  0A2h, 074h, 007h, 000h, 000h
        db  0CDh, 020h, 003h, 080h, 00Ch, 000h
loc_1_01A3:
        clc
        ret
        db  000h, 000h, 000h
loc_1_01A8:
        mov    eax, dword ptr [ebp + 1ch]
        cmp    ah, 0c2h
        je     SHORT loc_1_01C2
        db  08Bh, 00Dh, 078h, 007h, 000h, 000h
        db  08Bh, 015h, 07Ch, 007h, 000h, 000h
        db  0CDh, 020h, 049h, 080h, 001h, 000h
loc_1_01C2:
        db  0CDh, 020h, 047h, 000h, 001h, 000h
        jmp    SHORT loc_1_01D7
        db  08Bh, 0C0h
loc_1_01CC:
        mov    ax, word ptr [ebp + 1ch]
        cmp    ah, 0c2h
        je     SHORT loc_1_01D7
loc_1_01D5:
        stc
        ret
loc_1_01D7:
        db  080h, 03Dh, 096h, 007h, 000h, 000h, 0FFh
        je     SHORT loc_1_01D5
        db  08Bh, 035h, 090h, 007h, 000h, 000h
        add    esi, ebx
        movzx  eax, al
        cmp    eax, 7
        ja     SHORT loc_1_0200
        db  080h, 03Dh, 08Bh, 007h, 000h, 000h, 000h
        je     SHORT loc_1_022C
        db  0FFh, 024h, 085h, 0A0h, 007h, 000h, 000h
loc_1_0200:
        or     word ptr [ebp + 2ch], 1
        mov    byte ptr [ebp + 1dh], 1
        clc
        ret
loc_1_020B:
        or     word ptr [ebp + 2ch], 1
        mov    byte ptr [ebp + 1dh], 2
        clc
        ret
loc_1_0216:
        or     word ptr [ebp + 2ch], 1
        mov    byte ptr [ebp + 1dh], 5
        clc
        ret
loc_1_0221:
        and    word ptr [ebp + 2ch], 0fffeh
        mov    byte ptr [ebp + 1dh], 0
        clc
        ret
loc_1_022C:
        cmp    eax, 7
        je     SHORT loc_1_0221
        db  08Ah, 080h, 0C0h, 007h, 000h, 000h
        mov    byte ptr [ebp + 1dh], al
        cmp    eax, 4
        jne    SHORT loc_1_0243
        mov    byte ptr [ebp + 1ch], 0feh
loc_1_0243:
        or     word ptr [ebp + 2ch], 1
        clc
        ret
loc_1_024A:
        cmp    byte ptr [ebp + 11h], 1
        ja     SHORT loc_1_020B
        jb     SHORT loc_1_025D
        cmp    dword ptr [esi], 0
        je     SHORT loc_1_0216
        mov    byte ptr [esi + 5], 0ffh
        jmp    SHORT loc_1_0221
loc_1_025D:
        mov    byte ptr [esi + 5], 0
        jmp    SHORT loc_1_0221
loc_1_0263:
        mov    byte ptr [ebp + 11h], 0
        mov    byte ptr [esi + 6], 0
loc_1_026B:
        mov    dword ptr [esi], 0
        mov    byte ptr [esi + 5], 0
        jmp    SHORT loc_1_0221
loc_1_0277:
        cmp    byte ptr [ebp + 11h], 0
        jne    SHORT loc_1_0221
        mov    byte ptr [esi + 6], 0ffh
        jmp    SHORT loc_1_0221
loc_1_0283:
        mov    byte ptr [ebp + 11h], 0
        cmp    byte ptr [esi + 6], 0ffh
        jne    SHORT loc_1_0221
        db  066h, 0A1h, 084h, 007h, 000h, 000h
        mov    word ptr [ebp + 10h], ax
        jmp    SHORT loc_1_0221
loc_1_0299:
        mov    cl, byte ptr [ebp + 11h]
        cmp    cl, 2
        ja     NEAR PTR loc_1_020B
        je     NEAR PTR loc_1_0221
        test   cl, cl
        jne    NEAR PTR loc_1_0221
        cmp    byte ptr [esi + 5], 0ffh
        jne    SHORT loc_1_02BC
        or     cl, 20h
loc_1_02BC:
        db  08Ah, 015h, 094h, 007h, 000h, 000h
        add    dl, 2
        and    dl, 5
        or     cl, dl
        mov    byte ptr [ebp + 10h], cl
        mov    byte ptr [ebp + 18h], 3
        mov    byte ptr [ebp + 14h], 28h
        jmp    NEAR PTR loc_1_0221
loc_1_02DA:
        mov    cx, word ptr [ebp + 38h]
        shl    ecx, 10h
        mov    cx, word ptr [ebp + 10h]
        mov    dword ptr [esi], ecx
        mov    byte ptr [esi + 4], 0
        test   dword ptr [ebx], 20h
        je     NEAR PTR loc_1_0221
        mov    byte ptr [esi + 4], 0ffh
        jmp    NEAR PTR loc_1_0221
loc_1_0300:
        mov    edi, eax
        mov    esi, edx
        db  0F6h, 005h, 08Ah, 007h, 000h, 000h, 001h
        je     SHORT loc_1_032C
        mov    dx, 311h
        in     al, dx
        test   al, 2
        je     SHORT loc_1_032C
        mov    ah, al
        mov    dx, 310h
        in     al, dx
        test   ah, 20h
        je     SHORT loc_1_0342
        cmp    al, 0aah
        jne    NEAR PTR loc_1_048A
        je     SHORT loc_1_0342
loc_1_032C:
        in     al, 64h
        test   al, 1
        je     NEAR PTR loc_1_0488
        test   al, 20h
        je     NEAR PTR loc_1_0488
        mov    ah, al
        in     al, 60h
loc_1_0342:
        db  080h, 03Dh, 097h, 007h, 000h, 000h, 0FFh
        je     NEAR PTR loc_1_048A
        db  08Bh, 00Dh, 09Ah, 007h, 000h, 000h
        shl    ecx, 10h
        shr    ecx, 8
        mov    cl, al
        db  089h, 00Dh, 09Ah, 007h, 000h, 000h
        db  080h, 03Dh, 099h, 007h, 000h, 000h, 001h
        jb     SHORT loc_1_039A
        cmp    cx, 0aa00h
        jne    SHORT loc_1_039A
        db  080h, 03Dh, 08Ch, 007h, 000h, 000h, 001h
        jne    SHORT loc_1_039A
loc_1_037C:
        xor    eax, eax
        db  0A2h, 099h, 007h, 000h, 000h
        db  0A3h, 09Ah, 007h, 000h, 000h
        mov    edx, esi
        db  0BEh, 0C6h, 00Eh, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        jmp    NEAR PTR loc_1_048A
loc_1_039A:
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        db  087h, 005h, 086h, 007h, 000h, 000h
        db  00Fh, 0B6h, 01Dh, 099h, 007h, 000h, 000h
        or     ebx, ebx
        je     SHORT loc_1_03C2
        db  02Bh, 005h, 086h, 007h, 000h, 000h
        neg    eax
        cmp    eax, 0c8h
        jb     SHORT loc_1_03C2
        xor    ebx, ebx
loc_1_03C2:
        inc    ebx
        db  088h, 01Dh, 099h, 007h, 000h, 000h
        cmp    bl, 3
        jb     NEAR PTR loc_1_048A
        db  0C6h, 005h, 099h, 007h, 000h, 000h, 000h
        mov    eax, edi
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        mov    eax, ecx
        shr    ecx, 10h
        db  088h, 00Dh, 094h, 007h, 000h, 000h
sub_1_03EC:
        db  066h, 083h, 03Dh, 084h, 007h, 000h, 000h, 000h
        je     SHORT loc_1_0413
        mov    ecx, eax
        shr    ecx, 10h
        or     cl, cl
        jns    SHORT loc_1_0403
        or     ah, ah
        jns    SHORT loc_1_0408
loc_1_0403:
        jmp    NEAR PTR loc_1_037C
loc_1_0408:
        and    ah, 7fh
        test   cl, 40h
        je     SHORT loc_1_0413
        or     ah, 80h
loc_1_0413:
        movzx  ecx, al
        jecxz  loc_1_0428
        neg    cl
        xor    eax, 200000h
        test   eax, 200000h
        je     SHORT loc_1_0428
        dec    ch
loc_1_0428:
        movsx  edi, cx
        movzx  ecx, ah
        test   eax, 100000h
        je     SHORT loc_1_0437
        dec    ch
loc_1_0437:
        mov    ebx, esi
        movsx  esi, cx
        shr    eax, 10h
        db  066h, 083h, 03Dh, 084h, 007h, 000h, 000h, 000h
        je     SHORT loc_1_046B
        mov    dl, al
        xor    al, al
        test   dl, 1
        je     SHORT loc_1_0454
        or     al, 20h
loc_1_0454:
        test   dl, 2
        je     SHORT loc_1_045B
        or     al, 10h
loc_1_045B:
        test   dl, 4
        je     SHORT loc_1_0462
        or     al, 8
loc_1_0462:
        test   dl, 8
        je     SHORT loc_1_0480
        or     al, 4
        jmp    SHORT loc_1_0480
loc_1_046B:
        and    eax, 3
        je     SHORT loc_1_0480
        btr    eax, 0
        jae    SHORT loc_1_0478
        or     al, 20h
loc_1_0478:
        btr    eax, 1
        jae    SHORT loc_1_0480
        or     al, 10h
loc_1_0480:
        mov    edx, ebx
        db  0CDh, 020h, 003h, 080h, 00Ch, 000h
loc_1_0488:
        stc
        ret
loc_1_048A:
        mov    eax, edi
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        clc
        ret
loc_1_0494:
        push   edx
        mov    edx, 238h
        jmp    SHORT loc_1_04A2
loc_1_049C:
        push   edx
        mov    edx, 23ch
loc_1_04A2:
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        mov    al, 7
        out    dx, al
        inc    edx
        in     al, dx
        or     al, 20h
        out    dx, al
        db  080h, 03Dh, 0D4h, 007h, 000h, 000h, 011h
        ja     SHORT loc_1_04BF
        push   eax
        in     al, dx
        in     al, dx
        in     al, dx
        pop    eax
        out    dx, al
loc_1_04BF:
        dec    edx
        mov    al, 1
        out    dx, al
        inc    edx
        jmp    SHORT loc_1_04C6
loc_1_04C6:
        in     al, dx
        movsx  esi, al
        dec    edx
        mov    al, 2
        out    dx, al
        inc    edx
        jmp    SHORT loc_1_04D1
loc_1_04D1:
        in     al, dx
        movsx  edi, al
        dec    edx
        mov    al, 0
        out    dx, al
        inc    edx
        jmp    SHORT loc_1_04DC
loc_1_04DC:
        in     al, dx
        mov    bl, al
        dec    edx
        mov    al, 7
        out    dx, al
        inc    edx
        jmp    SHORT loc_1_04E6
loc_1_04E6:
        in     al, dx
        and    al, 0dfh
        jmp    SHORT loc_1_04EB
loc_1_04EB:
        out    dx, al
        xor    eax, eax
        test   bl, 4
        je     SHORT loc_1_04F5
        or     al, 20h
loc_1_04F5:
        test   bl, 1
        je     SHORT loc_1_04FC
        or     al, 10h
loc_1_04FC:
        pop    edx
        sti
        db  0CDh, 020h, 003h, 080h, 00Ch, 000h
loc_1_0504:
        mov    edi, edx
        movzx  edx, word ptr [edi]
        add    dl, 2
        push   eax
        in     al, dx
        test   al, 1
        pop    eax
        jne    SHORT loc_1_0546
        sub    dl, 2
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        in     al, dx
        mov    ah, al
        add    dl, 5
        in     al, dx
        sub    dl, 5
        xchg   al, ah
        test   ah, 2
        jne    SHORT loc_1_0553
        movzx  ecx, byte ptr [edi + 7]
        jecxz  loc_1_0548
        test   al, 40h
        jne    SHORT loc_1_0548
        mov    byte ptr [edi + ecx + 3], al
        inc    ecx
        cmp    byte ptr [edi + 2], cl
        je     SHORT loc_1_0559
        mov    byte ptr [edi + 7], cl
loc_1_0544:
        clc
        ret
loc_1_0546:
        stc
        ret
loc_1_0548:
        mov    byte ptr [edi + 3], al
        mov    byte ptr [edi + 7], 1
        test   al, 40h
        jne    SHORT loc_1_0544
loc_1_0553:
        mov    byte ptr [edi + 7], 0
        jmp    SHORT loc_1_0544
loc_1_0559:
        mov    ah, byte ptr [edi + 2]
        mov    byte ptr [edi + 7], 0
        mov    edx, edi
        mov    bx, word ptr [edx + 3]
        mov    al, bl
        shl    al, 6
        or     al, bh
        movsx  esi, al
        mov    al, bl
        shr    al, 2
        shl    al, 6
        or     al, byte ptr [edx + 5]
        movsx  edi, al
        mov    al, bl
        and    al, 30h
        cmp    ah, 4
        jne    SHORT loc_1_058F
        mov    bl, byte ptr [edx + 6]
        and    bl, 0ch
        or     al, bl
loc_1_058F:
        mov    edx, dword ptr [edx + 8]
        db  0CDh, 020h, 003h, 080h, 00Ch, 000h
        jmp    SHORT loc_1_05A0
        db  0FFh, 025h, 0E0h, 007h, 000h, 000h
loc_1_05A0:
        pushad
        movzx  ecx, word ptr [edi]
        jecxz  loc_1_0602
        add    edi, 2
loc_1_05A9:
        movzx  edx, word ptr [edi]
        mov    esi, dword ptr [edi + 2]
        cmp    edx, 3ceh
        je     SHORT loc_1_05E7
        cmp    edx, 3c4h
        je     SHORT loc_1_05EF
        cmp    edx, 3cfh
        je     SHORT loc_1_05F7
        db  03Bh, 015h, 0F8h, 007h, 000h, 000h
        je     SHORT loc_1_05DF
        cmp    edx, 3c5h
        jne    SHORT loc_1_05FD
        db  089h, 035h, 0E8h, 007h, 000h, 000h
        jmp    SHORT loc_1_05FD
loc_1_05DF:
        db  089h, 035h, 0F4h, 007h, 000h, 000h
        jmp    SHORT loc_1_05FD
loc_1_05E7:
        db  089h, 035h, 0ECh, 007h, 000h, 000h
        jmp    SHORT loc_1_05FD
loc_1_05EF:
        db  089h, 035h, 0E4h, 007h, 000h, 000h
        jmp    SHORT loc_1_05FD
loc_1_05F7:
        db  089h, 035h, 0F0h, 007h, 000h, 000h
loc_1_05FD:
        add    edi, 6
        loop   loc_1_05A9
loc_1_0602:
        popad
        db  0FFh, 025h, 0E0h, 007h, 000h, 000h
        db  000h, 000h, 000h, 0CBh, 000h, 000h, 000h, 0D2h, 000h, 000h, 000h, 0D6h, 000h, 000h, 000h, 0A0h
        db  000h, 000h, 000h, 029h, 011h, 000h, 000h, 085h, 011h, 000h, 000h, 004h, 008h, 000h, 000h, 0C0h
        db  00Dh, 000h, 000h, 0EBh, 009h, 000h, 000h, 06Ch, 00Bh, 000h, 000h, 06Dh, 00Bh, 000h, 000h, 068h
        db  000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 00Ch, 000h, 004h, 000h, 000h, 000h, 056h, 04Dh, 04Fh, 055h
        db  053h, 045h, 020h, 020h, 000h, 000h, 000h, 034h, 000h, 000h, 000h, 000h, 034h, 008h, 000h, 000h
        db  034h, 008h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  00Ch, 006h, 000h, 000h, 00Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0696:
        add    al, byte ptr [eax]
loc_1_0698:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_069C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  046h, 001h, 000h, 000h, 068h, 001h, 000h, 000h, 094h, 001h, 000h, 000h, 0B1h, 001h, 000h, 000h
        db  0C8h, 001h, 000h, 000h, 0FAh, 001h, 000h, 000h, 038h, 002h, 000h, 000h, 077h, 002h, 000h, 000h
        db  096h, 002h, 000h, 000h, 0BFh, 010h, 000h, 000h, 069h, 010h, 000h, 000h, 0B5h, 002h, 000h, 000h
        db  0C6h, 002h, 000h, 000h, 0F6h, 002h, 000h, 000h, 0FAh, 002h, 000h, 000h, 0FEh, 002h, 000h, 000h
        db  018h, 003h, 000h, 000h, 012h, 001h, 000h, 000h, 012h, 001h, 000h, 000h, 046h, 003h, 000h, 000h
        db  047h, 003h, 000h, 000h, 06Ch, 003h, 000h, 000h, 080h, 003h, 000h, 000h, 09Eh, 003h, 000h, 000h
        db  0BEh, 003h, 000h, 000h, 0ECh, 003h, 000h, 000h, 021h, 004h, 000h, 000h, 063h, 004h, 000h, 000h
        db  07Ah, 004h, 000h, 000h, 07Bh, 004h, 000h, 000h, 08Bh, 004h, 000h, 000h, 095h, 004h, 000h, 000h
        db  0ACh, 004h, 000h, 000h, 0BEh, 004h, 000h, 000h, 0D6h, 004h, 000h, 000h, 0E3h, 004h, 000h, 000h
        db  0ECh, 004h, 000h, 000h, 00Ah, 005h, 000h, 000h, 035h, 005h, 000h, 000h, 04Eh, 005h, 000h, 000h
        db  070h, 005h, 000h, 000h, 0C3h, 005h, 000h, 000h, 0F6h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h
loc_1_0771:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_078B:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0795:
        add    byte ptr [eax], al
        db  000h
loc_1_0798:
        inc    dword ptr [eax]
loc_1_079A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  04Ah, 002h, 000h, 000h, 063h, 002h, 000h, 000h, 077h, 002h, 000h, 000h, 021h, 002h, 000h, 000h
        db  083h, 002h, 000h, 000h, 06Bh, 002h, 000h, 000h, 099h, 002h, 000h, 000h, 0DAh, 002h, 000h, 000h
loc_1_07C0:
        add    eax, dword ptr [ebx + eax]
        add    eax, dword ptr [esp + eax]
        add    al, 0
loc_1_07C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_07CC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_07D0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_07D4:
        adc    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_07D8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_07DC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_07E0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_07E4:
        mov    ch, 26h
        add    byte ptr [eax], al
sub_1_07E8:
        mov    ch, 26h
        add    byte ptr [eax], al
sub_1_07EC:
        mov    ch, 26h
        add    byte ptr [eax], al
sub_1_07F0:
        mov    ch, 26h
        add    byte ptr [eax], al
sub_1_07F4:
        mov    ch, 26h
        add    byte ptr [eax], al
loc_1_07F8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_07FC:
        cld
        adc    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [edi], al
        add    bh, bh
        add    byte ptr [eax], al
        db  000h, 03Dh, 019h, 000h, 000h, 08Ah
        sbb    dword ptr [eax], eax
        db  000h, 027h
        and    eax, dword ptr [eax]
        db  000h, 03Ah
        sbb    al, byte ptr [eax]
        db  000h, 052h, 015h
        add    byte ptr [eax], al
        loope  loc_1_0848
        add    byte ptr [eax], al
        leave
        add    byte ptr es:[eax], al
        add    ebx, dword ptr [ecx]
        add    byte ptr [eax], al
        db  022h, 015h, 000h, 000h, 023h, 015h
        add    byte ptr [eax], al
        or     ah, byte ptr [edi]
        add    byte ptr [eax], al
        or     esp, dword ptr [edi]
        add    byte ptr [eax], al
        and    eax, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [edx], eax
loc_1_0848:
        add    eax, dword ptr [edi]
        add    al, 5
        push   es
        or     eax, 11100f0eh
        adc    dl, byte ptr [ebx]
loc_1_0854:
        add    al, byte ptr [edx]
        add    dword ptr [esp + eax], eax
        add    dword ptr [ecx + eax], eax
        add    al, 8
loc_1_085E:
        add    al, byte ptr [edx]
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
loc_1_0868:
        inc    eax
        add    eax, ecx
        add    byte ptr [eax + 1], al
        enter  -8000h, 2
        enter  4000h, 1
        enter  -8000h, 2
        enter  -8000h, 2
        pop    esi
        add    dword ptr [eax - 7ffea1feh], eax
        add    ah, al
        add    dword ptr [eax + 4001e002h], eax
        add    eax, ecx
        add    byte ptr [eax + 40h], al
        inc    eax
        and    byte ptr [eax - 80h], al
        add    byte ptr [eax], 0
        add    byte ptr [ebp + 1fh], al
        test   dword ptr [esi], edi
        insd
        insd
        add    eax, 5960596h
        cli
        push   ebx
        db  065h, 072h, 069h
        popad
        insb
        dec    ebp
        outsd
        db  075h, 073h
        db  065h, 000h
_LTEXT ends
        end
