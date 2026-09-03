; paralink_lcode_01.asm -- PARALINK object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

PARALINK_Control:
        cmp    eax, 1bh
        db  00Fh, 084h, 0F7h, 0FFh, 0FEh, 0FFh
        clc
        ret
sub_1_000B:
        cld
        push   esi
        push   edi
        lea    edi, [esi + 94h]
        mov    esi, ebx
        mov    ecx, 30h
        rep movsb
        xor    eax, eax
        pop    edi
        pop    esi
        ret
sub_1_0022:
        xor    eax, eax
        or     ebx, ebx
        je     SHORT loc_1_0033
        mov    dword ptr [ebx], eax
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [esi + 58h]
        mov    dword ptr [ebx + 8], eax
loc_1_0033:
        ret
loc_1_0034:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cld
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        call   sub_1_000B
        mov    dword ptr [esi + 0ch], eax
        or     al, 1
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_0050:
        push   ebp
        mov    ebp, esp
        push   edi
        or     al, 1
        pop    edi
        leave
        ret
loc_1_0059:
        push   ebp
        mov    ebp, esp
        push   esi
        pop    esi
        leave
        ret
loc_1_0060:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edx
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        call   sub_1_0022
        xor    eax, eax
        mov    dword ptr [esi + 0ch], eax
        xchg   dword ptr [esi + 34h], eax
        mov    edx, dword ptr [ebp + 10h]
        mov    dword ptr [edx], eax
        or     al, 1
        pop    esi
        pop    edx
        pop    ebx
        leave
        ret
loc_1_0085:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    dword ptr [esi + 0ch], 0ffffffech
        cmp    bl, 5
        jne    SHORT loc_1_00BA
        test   byte ptr [esi + 12ah], 1
        jne    SHORT loc_1_011D
        or     byte ptr [esi + 12ah], 1
        call   sub_1_1B87
        xor    eax, eax
        jmp    SHORT loc_1_011D
loc_1_00BA:
        cmp    bl, 6
        jne    SHORT loc_1_00D8
        test   byte ptr [esi + 12ah], 1
        je     SHORT loc_1_011D
        and    byte ptr [esi + 12ah], 0feh
        call   sub_1_1BB4
        xor    eax, eax
        jmp    SHORT loc_1_011D
loc_1_00D8:
        cmp    bl, 13h
        jne    SHORT loc_1_00F3
        mov    edi, dword ptr [ebp + 10h]
        or     edi, edi
        je     SHORT loc_1_011E
        cmp    edi, -1
        je     SHORT loc_1_011E
        mov    dword ptr [esi + 130h], edi
        xor    eax, eax
        jmp    SHORT loc_1_011D
loc_1_00F3:
        cmp    bl, 10h
        jne    SHORT loc_1_0101
        or     byte ptr [esi + 129h], 20h
        jmp    SHORT loc_1_011D
loc_1_0101:
        cmp    bl, 15h
        jne    SHORT loc_1_010F
        and    byte ptr [esi + 129h], 0dfh
        jmp    SHORT loc_1_011D
loc_1_010F:
        cmp    bl, 14h
        jne    SHORT loc_1_011E
        and    byte ptr [esi + 129h], 0bfh
        jmp    SHORT loc_1_011D
loc_1_011D:
        inc    eax
loc_1_011E:
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        leave
        ret
loc_1_0125:
        push   ebp
        mov    ebp, esp
        push   ecx
        push   edx
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + 0ch], 0
        mov    edx, dword ptr [edx + 0e4h]
        mov    eax, dword ptr [ebp + 0ch]
        not    eax
        mov    ecx, dword ptr [edx]
        and    eax, ecx
        mov    dword ptr [edx], eax
        mov    edx, dword ptr [ebp + 10h]
        mov    dword ptr [edx], ecx
        or     al, 1
        pop    edx
        pop    ecx
        leave
        ret
loc_1_0150:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 10h]
        mov    edi, dword ptr [ebp + 0ch]
        push   esi
        mov    eax, 13h
        call   sub_1_03A8
        pop    esi
        mov    ecx, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 14h]
        mov    dword ptr [edx], ecx
        cmp    dword ptr [esi + 104h], 0
        je     SHORT loc_1_0191
        cmp    dword ptr [esi + 120h], 0
        je     SHORT loc_1_0191
        mov    eax, 2
        call   sub_1_022B
loc_1_0191:
        xor    eax, eax
        mov    dword ptr [esi + 0ch], eax
        mov    dword ptr [esi + 34h], eax
        inc    eax
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        leave
        ret
loc_1_01A1:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 14h]
        mov    dword ptr [edx], 0
        mov    edx, dword ptr [ebp + 8]
        mov    ebx, dword ptr [edx + 4ch]
        or     ebx, ebx
        je     SHORT loc_1_0207
        mov    ecx, dword ptr [ebp + 10h]
        mov    edi, dword ptr [ebp + 0ch]
        cmp    ecx, ebx
        jle    SHORT loc_1_01C8
        xchg   ebx, ecx
loc_1_01C8:
        push   ecx
        mov    ebx, dword ptr [ebp + 14h]
        mov    dword ptr [ebx], ecx
        mov    esi, dword ptr [edx + 50h]
        mov    eax, dword ptr [edx + 3ch]
        add    eax, dword ptr [edx + 40h]
        sub    eax, esi
        sub    eax, ecx
        mov    ebx, eax
        jge    SHORT loc_1_01E1
        add    ecx, eax
loc_1_01E1:
        cld
        shr    cx, 1
        rep movsw
        jae    SHORT loc_1_01EB
        movsb
loc_1_01EB:
        cmp    ebx, 0
        jg     SHORT loc_1_0200
        mov    ecx, ebx
        neg    ecx
        mov    esi, dword ptr [edx + 3ch]
        shr    cx, 1
        rep movsw
        jae    SHORT loc_1_0200
        movsb
loc_1_0200:
        pop    ecx
        sub    dword ptr [edx + 4ch], ecx
        mov    dword ptr [edx + 50h], esi
loc_1_0207:
        mov    eax, dword ptr [edx + 4ch]
        cmp    eax, dword ptr [edx + 11ch]
        jae    SHORT loc_1_0219
        and    byte ptr [edx + 0e9h], 0feh
loc_1_0219:
        xor    eax, eax
        mov    dword ptr [edx + 0ch], eax
        inc    eax
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        leave
        ret
        db  000h, 000h
loc_1_0228:
        xor    eax, eax
        ret
sub_1_022B:
        pushad
        or     byte ptr [esi + 0e9h], al
        cmp    eax, 2
        jne    SHORT loc_1_0259
        mov    ebx, dword ptr [esi + 104h]
        or     ebx, ebx
        je     SHORT loc_1_02AC
        inc    ebx
        je     SHORT loc_1_02AC
        push   0
        push   eax
        push   dword ptr [esi + 108h]
        push   esi
        call   dword ptr [esi + 104h]
        add    esp, 10h
        jmp    SHORT loc_1_02AC
loc_1_0259:
        cmp    eax, 1
        jne    SHORT loc_1_0280
        mov    ebx, dword ptr [esi + 0fch]
        or     ebx, ebx
        je     SHORT loc_1_02AC
        inc    ebx
        je     SHORT loc_1_02AC
        push   0
        push   eax
        push   dword ptr [esi + 100h]
        push   esi
        call   dword ptr [esi + 0fch]
        add    esp, 10h
        jmp    SHORT loc_1_02AC
loc_1_0280:
        cmp    eax, 4
        jne    SHORT loc_1_02AC
        mov    ebx, dword ptr [esi + 0f4h]
        or     ebx, ebx
        je     SHORT loc_1_02AC
        inc    ebx
        je     SHORT loc_1_02AC
        mov    ebx, dword ptr [esi + 0e4h]
        mov    ebx, dword ptr [ebx]
        push   ebx
        push   eax
        push   dword ptr [esi + 0f8h]
        push   esi
        call   dword ptr [esi + 0f4h]
        add    esp, 10h
loc_1_02AC:
        popad
        ret
sub_1_02AE:
        push   edi
        mov    edx, dword ptr [eax + 144h]
        xor    ecx, ecx
loc_1_02B7:
        mov    cl, byte ptr [edx]
        mov    ch, byte ptr [edi]
        or     ecx, ecx
        je     SHORT loc_1_02D3
        call   sub_1_02D5
        xchg   ch, cl
        call   sub_1_02D5
        inc    edx
        inc    edi
        cmp    cl, ch
        je     SHORT loc_1_02B7
        or     esi, esi
loc_1_02D3:
        pop    edi
        ret
sub_1_02D5:
        cmp    cl, 41h
        jb     SHORT loc_1_02E2
        cmp    cl, 5ah
        ja     SHORT loc_1_02E2
        or     cl, 20h
loc_1_02E2:
        ret
        db  000h
sub_1_02E4:
        push   eax
        push   ebx
        push   edx
        movzx  eax, byte ptr [esi + 151h]
        db  08Bh, 004h, 085h, 050h, 025h, 000h, 000h
        movzx  ebx, byte ptr [esi + 152h]
        mov    edx, dword ptr [esi + 0c8h]
        call   dword ptr [eax + ebx*4]
        pop    edx
        pop    ebx
        pop    eax
        ret
loc_1_0309:
        stc
        ret
loc_1_030B:
        mov    al, 0e5h
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 10h
        jb     SHORT loc_1_0319
        in     al, dx
        and    al, 10h
loc_1_0319:
        or     al, 0ch
        out    dx, al
        dec    dx
        dec    dx
        clc
        ret
loc_1_0322:
        push   edx
        inc    dx
        inc    dx
        add    dx, 400h
        mov    al, 35h
        out    dx, al
        pop    edx
        mov    al, 0e5h
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 10h
        jb     SHORT loc_1_033E
        in     al, dx
        and    al, 10h
loc_1_033E:
        or     al, 0fh
        out    dx, al
        dec    dx
        dec    dx
        clc
        ret
loc_1_0347:
        mov    al, 0e5h
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 10h
        jb     SHORT loc_1_0355
        in     al, dx
        and    al, 10h
loc_1_0355:
        or     al, 0fh
        out    dx, al
        dec    dx
        dec    dx
        clc
        ret
sub_1_035E:
        mov    al, 0e5h
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 10h
        jb     SHORT loc_1_036C
        in     al, dx
        and    al, 10h
loc_1_036C:
        mov    ah, al
        or     al, 0ch
        out    dx, al
        push   ax
        in     al, 61h
        pop    ax
        mov    al, 0eh
        or     al, ah
        out    dx, al
        dec    dx
        dec    dx
        mov    al, 0
        out    dx, al
        mov    al, 0e5h
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0ch
        or     al, ah
        out    dx, al
        dec    dx
        dec    dx
        clc
        ret
loc_1_0395:
        add    dx, 402h
        mov    al, 35h
        out    dx, al
        sub    dx, 402h
        call   sub_1_035E
        ret
sub_1_03A8:
        db  0A3h, 074h, 027h, 000h, 000h
        db  089h, 00Dh, 06Ch, 027h, 000h, 000h
        db  089h, 03Dh, 070h, 027h, 000h, 000h
        db  089h, 035h, 068h, 027h, 000h, 000h
        db  0C7h, 005h, 088h, 027h, 000h, 000h, 003h, 000h, 000h, 000h
        inc    dword ptr [esi + 188h]
loc_1_03CF:
        cmp    byte ptr [esi + 128h], 0
        je     NEAR PTR loc_1_0498
        mov    edx, dword ptr [esi + 0c8h]
        db  0C7h, 005h, 099h, 027h, 000h, 000h, 074h, 06Dh, 078h, 054h
        cld
        cmp    byte ptr [esi + 151h], 0
        jne    SHORT loc_1_03FB
        call   sub_1_22CA
loc_1_03FB:
        inc    edx
        call   sub_1_1046
        and    al, 0f0h
        cmp    al, 0e0h
        je     NEAR PTR loc_1_04EF
        dec    edx
        movzx  eax, byte ptr [esi + 151h]
        mov    edx, dword ptr [esi + 0c8h]
        db  0FFh, 014h, 085h, 060h, 025h, 000h, 000h
        jb     SHORT loc_1_0498
        movzx  eax, byte ptr [esi + 150h]
        shl    al, 2
        or     al, byte ptr [esi + 1fch]
        cmp    ax, 0fh
        jg     SHORT loc_1_0492
        movzx  ebx, byte ptr [esi + 151h]
        db  08Bh, 01Ch, 09Dh, 080h, 025h, 000h, 000h
        mov    edx, dword ptr [esi + 0c8h]
        call   dword ptr [ebx + eax*4]
        jb     SHORT loc_1_049F
        inc    dword ptr [esi + 18ch]
        inc    dword ptr [esi + 190h]
        cmp    dword ptr [esi + 190h], 2
        jl     SHORT loc_1_0476
        cmp    byte ptr [esi + 151h], 0
        jne    SHORT loc_1_0476
        mov    byte ptr [esi + 151h], 1
loc_1_0476:
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        mov    dword ptr [esi + 194h], eax
        db  0A3h, 06Ch, 027h, 000h, 000h
        push   ecx
        call   sub_1_1DC8
        pop    ecx
        xor    eax, eax
        clc
        ret
loc_1_0492:
        inc    dword ptr [esi + 1ech]
loc_1_0498:
        mov    byte ptr [esi + 151h], 0
loc_1_049F:
        db  0FFh, 00Dh, 088h, 027h, 000h, 000h
        jge    SHORT loc_1_04C2
        xor    eax, eax
        mov    dword ptr [esi + 190h], eax
        db  0A3h, 06Ch, 027h, 000h, 000h
        push   ecx
        call   sub_1_1DC8
        pop    ecx
        mov    eax, 2
        stc
        ret
loc_1_04C2:
        mov    edx, dword ptr [esi + 0c8h]
        inc    edx
        mov    ecx, dword ptr [esi + 170h]
        shl    ecx, 1
loc_1_04D1:
        call   sub_1_1046
        and    al, 0f0h
        cmp    al, 0e0h
        je     SHORT loc_1_04E4
        cmp    al, 0a0h
        je     SHORT loc_1_04E4
        loop   loc_1_04D1
        jmp    SHORT loc_1_049F
loc_1_04E4:
        inc    dword ptr [esi + 1e4h]
        jmp    NEAR PTR loc_1_03CF
loc_1_04EF:
        inc    dword ptr [esi + 1e8h]
        pushad
        call   sub_1_1E2C
        popad
        jmp    NEAR PTR loc_1_03CF
loc_1_0501:
        stc
        ret
sub_1_0503:
        db  089h, 035h, 068h, 027h, 000h, 000h
        db  089h, 03Dh, 078h, 027h, 000h, 000h
        mov    edx, dword ptr [esi + 0c8h]
        db  0C7h, 005h, 099h, 027h, 000h, 000h, 076h, 063h, 065h, 052h
        cld
        inc    dword ptr [esi + 180h]
        cmp    byte ptr [esi + 128h], 0
        je     NEAR PTR loc_1_05C8
        cmp    byte ptr [esi + 151h], 0
        jne    SHORT loc_1_0541
        call   sub_1_22CA
loc_1_0541:
        movzx  eax, byte ptr [esi + 151h]
        mov    edx, dword ptr [esi + 0c8h]
        db  0FFh, 014h, 085h, 070h, 025h, 000h, 000h
        jb     SHORT loc_1_05C8
        movzx  eax, byte ptr [esi + 150h]
        shl    al, 2
        or     al, byte ptr [esi + 1fch]
        cmp    ax, 0fh
        jg     SHORT loc_1_05C2
        movzx  ebx, byte ptr [esi + 151h]
        db  08Bh, 01Ch, 09Dh, 090h, 025h, 000h, 000h
        mov    edx, dword ptr [esi + 0c8h]
        call   dword ptr [ebx + eax*4]
        jb     SHORT loc_1_05CF
        push   eax
        push   ecx
        inc    dword ptr [esi + 184h]
        inc    dword ptr [esi + 190h]
        cmp    dword ptr [esi + 190h], 2
        jl     SHORT loc_1_05AD
        cmp    byte ptr [esi + 151h], 0
        jne    SHORT loc_1_05AD
        mov    byte ptr [esi + 151h], 1
loc_1_05AD:
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        mov    dword ptr [esi + 194h], eax
        call   sub_1_1DC8
        pop    ecx
        pop    eax
        clc
        ret
loc_1_05C2:
        inc    dword ptr [esi + 1ech]
loc_1_05C8:
        mov    byte ptr [esi + 151h], 0
loc_1_05CF:
        xor    eax, eax
        mov    dword ptr [esi + 190h], eax
        push   ecx
        call   sub_1_1DC8
        pop    ecx
        mov    eax, 2
        stc
        ret
loc_1_05E5:
        stc
        ret
loc_1_05E7:
        db  0C7h, 005h, 099h, 027h, 000h, 000h, 034h, 064h, 048h, 054h
        mov    edx, dword ptr [esi + 0c8h]
        mov    al, 0ch
        inc    dx
        inc    dx
        out    dx, al
        sti
        dec    dx
        dec    dx
        mov    al, 0e5h
        out    dx, al
        mov    al, 0edh
        out    dx, al
        mov    ecx, dword ptr [esi + 170h]
        inc    dx
loc_1_0611:
        call   sub_1_1046
        cmp    al, 0e8h
        je     NEAR PTR loc_1_0745
        cmp    al, 0d8h
        je     SHORT loc_1_0629
        loop   loc_1_0611
        jmp    NEAR PTR loc_1_06C5
loc_1_0629:
        dec    dx
        db  0C6h, 005h, 096h, 027h, 000h, 000h, 000h
        mov    al, byte ptr [esi + 1ffh]
        mov    ah, byte ptr [esi + 153h]
        call   sub_1_076F
        jb     NEAR PTR loc_1_06D1
        xor    ah, 0ffh
        cmp    al, ah
        jne    NEAR PTR loc_1_0701
        cmp    al, 3
        jg     NEAR PTR loc_1_070D
        mov    byte ptr [esi + 1fch], al
        db  0A1h, 074h, 027h, 000h, 000h
        mov    ah, byte ptr [esi + 150h]
        cmp    ah, 3
        jg     NEAR PTR loc_1_0719
        call   sub_1_076F
        jb     SHORT loc_1_06DD
        db  066h, 03Bh, 005h, 08Ch, 027h, 000h, 000h
        jne    NEAR PTR loc_1_0725
        db  0A1h, 06Ch, 027h, 000h, 000h
        call   sub_1_076F
        jb     SHORT loc_1_06E9
        db  066h, 03Bh, 005h, 08Ch, 027h, 000h, 000h
        jne    NEAR PTR loc_1_0731
        mov    al, 0eah
        out    dx, al
        mov    al, 0ch
        inc    dx
        inc    dx
        out    dx, al
        dec    dx
        mov    ecx, dword ptr [esi + 170h]
loc_1_06B5:
        call   sub_1_1046
        cmp    al, 0d0h
        loopne loc_1_06B5
        jcxz   loc_1_06F5
        dec    dx
        clc
        ret
loc_1_06C5:
        add    dword ptr [esi + 1b0h], 1000h
        jmp    SHORT loc_1_073D
loc_1_06D1:
        add    dword ptr [esi + 1b4h], 1000h
        jmp    SHORT loc_1_073D
loc_1_06DD:
        add    dword ptr [esi + 1b8h], 1000h
        jmp    SHORT loc_1_073D
loc_1_06E9:
        add    dword ptr [esi + 1bch], 1000h
        jmp    SHORT loc_1_073D
loc_1_06F5:
        add    dword ptr [esi + 1c0h], 1000h
        jmp    SHORT loc_1_073D
loc_1_0701:
        add    dword ptr [esi + 1b0h], 100000h
        jmp    SHORT loc_1_073D
loc_1_070D:
        add    dword ptr [esi + 1b4h], 100000h
        jmp    SHORT loc_1_073D
loc_1_0719:
        add    dword ptr [esi + 1b8h], 100000h
        jmp    SHORT loc_1_073D
loc_1_0725:
        add    dword ptr [esi + 1bch], 100000h
        jmp    SHORT loc_1_073D
loc_1_0731:
        add    dword ptr [esi + 1c0h], 100000h
        jmp    SHORT loc_1_073D
loc_1_073D:
        inc    dword ptr [esi + 1a0h]
        jmp    SHORT loc_1_0753
loc_1_0745:
        inc    dword ptr [esi + 1c4h]
        jmp    SHORT loc_1_0753
        db  0FFh, 086h, 098h, 001h, 000h, 000h
loc_1_0753:
        mov    edx, dword ptr [esi + 0c8h]
        mov    al, 0e5h
        out    dx, al
        mov    al, 0ch
        or     al, byte ptr [esi + 136h]
        inc    dx
        inc    dx
        out    dx, al
        dec    dx
        dec    dx
        stc
        ret
sub_1_076F:
        push   edx
        mov    bx, ax
        and    al, 0fh
        db  0A2h, 08Eh, 027h, 000h, 000h
        db  002h, 005h, 096h, 027h, 000h, 000h
        and    al, 0fh
        db  0A2h, 096h, 027h, 000h, 000h
        xchg   al, bl
        shr    al, 4
        db  0A2h, 08Fh, 027h, 000h, 000h
        db  002h, 005h, 096h, 027h, 000h, 000h
        and    al, 0fh
        db  0A2h, 096h, 027h, 000h, 000h
        xchg   al, bh
        and    al, 0fh
        db  0A2h, 090h, 027h, 000h, 000h
        db  002h, 005h, 096h, 027h, 000h, 000h
        and    al, 0fh
        db  0A2h, 096h, 027h, 000h, 000h
        xchg   al, ah
        shr    al, 4
        db  0A2h, 091h, 027h, 000h, 000h
        db  002h, 005h, 096h, 027h, 000h, 000h
        and    al, 0fh
        db  0A2h, 096h, 027h, 000h, 000h
        shl    al, 4
        or     al, ah
        shl    bh, 4
        or     bl, bh
        mov    ah, bl
        xchg   ah, al
        db  066h, 0A3h, 08Ch, 027h, 000h, 000h
        db  0A0h, 08Eh, 027h, 000h, 000h
        shl    al, 1
        xor    al, 0f0h
        out    dx, al
        mov    ecx, dword ptr [esi + 16ch]
        inc    dx
loc_1_07F1:
        in     al, dx
        test   al, 8
        loopne loc_1_07F1
        jcxz   loc_1_0818
        in     al, dx
        db  0A2h, 08Eh, 027h, 000h, 000h
        dec    dx
        db  0A0h, 08Fh, 027h, 000h, 000h
        shl    al, 1
        xor    al, 0f1h
        out    dx, al
        mov    ecx, dword ptr [esi + 16ch]
        inc    dx
loc_1_0813:
        in     al, dx
        test   al, 8
        loope  loc_1_0813
loc_1_0818:
        jcxz   loc_1_0889
        in     al, dx
        db  0A2h, 08Fh, 027h, 000h, 000h
        dec    dx
        db  0A0h, 090h, 027h, 000h, 000h
        shl    al, 1
        xor    al, 0f0h
        out    dx, al
        mov    ecx, dword ptr [esi + 16ch]
        inc    dx
loc_1_0835:
        in     al, dx
        test   al, 8
        loopne loc_1_0835
        jcxz   loc_1_0889
        in     al, dx
        db  0A2h, 090h, 027h, 000h, 000h
        dec    dx
        db  0A0h, 091h, 027h, 000h, 000h
        shl    al, 1
        xor    al, 0f1h
        out    dx, al
        mov    ecx, dword ptr [esi + 16ch]
        inc    dx
loc_1_0857:
        in     al, dx
        test   al, 8
        loope  loc_1_0857
        jcxz   loc_1_0889
        in     al, dx
        db  0A2h, 091h, 027h, 000h, 000h
        dec    dx
        db  066h, 0A1h, 08Eh, 027h, 000h, 000h
        db  066h, 08Bh, 01Dh, 090h, 027h, 000h, 000h
        xchg   bl, ah
        and    ax, 0f0f0h
        and    bx, 0f0f0h
        shr    ax, 4
        or     ax, bx
        pop    edx
        clc
        ret
loc_1_0889:
        pop    edx
        stc
        ret
loc_1_088C:
        db  0C7h, 005h, 099h, 027h, 000h, 000h, 034h, 064h, 048h, 052h
        mov    al, 0ch
        inc    dx
        inc    dx
        out    dx, al
        sti
        dec    dx
        mov    ecx, dword ptr [esi + 174h]
loc_1_08A6:
        call   sub_1_1046
        cmp    al, 0e8h
        je     SHORT loc_1_08B6
        loop   loc_1_08A6
        jmp    NEAR PTR loc_1_0985
loc_1_08B6:
        db  0C6h, 005h, 096h, 027h, 000h, 000h, 000h
        dec    dx
        mov    al, 0ebh
        out    dx, al
        mov    al, byte ptr [esi + 150h]
        cmp    al, 3
        jg     NEAR PTR loc_1_09C1
        mov    ah, al
        and    al, 0fh
        db  0A2h, 08Eh, 027h, 000h, 000h
        mov    al, ah
        shr    al, 4
        db  0A2h, 08Fh, 027h, 000h, 000h
        xor    ah, 0ffh
        mov    al, ah
        and    al, 0fh
        db  0A2h, 090h, 027h, 000h, 000h
        shr    ah, 4
        db  088h, 025h, 091h, 027h, 000h, 000h
        call   sub_1_0A2F
        jb     NEAR PTR loc_1_0991
        mov    byte ptr [esi + 1ffh], al
        call   sub_1_0A36
        jb     NEAR PTR loc_1_099D
        cmp    ah, 3
        jg     NEAR PTR loc_1_09CD
        mov    byte ptr [esi + 1fch], ah
        movzx  eax, al
        db  0A3h, 080h, 027h, 000h, 000h
        call   sub_1_0A36
        jb     SHORT loc_1_09A9
        movzx  ebx, ax
        db  089h, 01Dh, 07Ch, 027h, 000h, 000h
        cmp    bx, 1000h
        ja     NEAR PTR loc_1_0A0D
        cmp    bx, 1
        jl     NEAR PTR loc_1_0A0D
        mov    eax, dword ptr [esi + 40h]
        sub    eax, dword ptr [esi + 4ch]
        cmp    bx, ax
        jg     NEAR PTR loc_1_0A0D
        inc    dx
        mov    ecx, dword ptr [esi + 174h]
loc_1_0967:
        call   sub_1_1046
        cmp    al, 0d0h
        loopne loc_1_0967
        jcxz   loc_1_09B5
        dec    dx
        mov    al, 0eah
        out    dx, al
        mov    al, 0ch
        inc    dx
        inc    dx
        out    dx, al
        dec    dx
        dec    dx
        clc
        ret
loc_1_0985:
        add    dword ptr [esi + 1c8h], 1000h
        jmp    SHORT loc_1_09FD
loc_1_0991:
        add    dword ptr [esi + 1cch], 1000h
        jmp    SHORT loc_1_09FD
loc_1_099D:
        add    dword ptr [esi + 1d0h], 1000h
        jmp    SHORT loc_1_09FD
loc_1_09A9:
        add    dword ptr [esi + 1d4h], 1000h
        jmp    SHORT loc_1_09FD
loc_1_09B5:
        add    dword ptr [esi + 1d8h], 1000h
        jmp    SHORT loc_1_09FD
loc_1_09C1:
        add    dword ptr [esi + 1c8h], 100000h
        jmp    SHORT loc_1_09FD
loc_1_09CD:
        add    dword ptr [esi + 1cch], 100000h
        jmp    SHORT loc_1_09FD
        db  081h, 086h, 0D0h, 001h, 000h, 000h, 000h, 000h, 010h, 000h, 0EBh, 018h, 081h, 086h, 0D4h, 001h
        db  000h, 000h, 000h, 000h, 010h, 000h, 0EBh, 00Ch, 081h, 086h, 0D8h, 001h, 000h, 000h, 000h, 000h
        db  010h, 000h, 0EBh, 000h
loc_1_09FD:
        inc    dword ptr [esi + 1a4h]
        jmp    SHORT loc_1_0A13
        db  0FFh, 086h, 09Ch, 001h, 000h, 000h, 0EBh, 006h
loc_1_0A0D:
        inc    dword ptr [esi + 1dch]
loc_1_0A13:
        mov    edx, dword ptr [esi + 0c8h]
        mov    al, 0e5h
        out    dx, al
        mov    al, 0ch
        or     al, byte ptr [esi + 136h]
        inc    dx
        inc    dx
        out    dx, al
        dec    dx
        dec    dx
        stc
        ret
sub_1_0A2F:
        push   edi
        mov    di, 0ffffh
        jmp    SHORT loc_1_0A3A
sub_1_0A36:
        push   edi
        xor    di, di
loc_1_0A3A:
        push   edx
        mov    ecx, dword ptr [esi + 16ch]
        inc    dx
loc_1_0A43:
        in     al, dx
        test   al, 8
        loopne loc_1_0A43
        jcxz   loc_1_0A83
        in     al, dx
        shr    al, 4
        mov    bl, al
        db  002h, 005h, 096h, 027h, 000h, 000h
        and    al, 0fh
        db  0A2h, 096h, 027h, 000h, 000h
        db  0A2h, 092h, 027h, 000h, 000h
        dec    dx
        test   di, 0ffffh
        je     SHORT loc_1_0A71
        db  0A0h, 08Eh, 027h, 000h, 000h
loc_1_0A71:
        shl    al, 1
        xor    al, 0f0h
        out    dx, al
        mov    ecx, dword ptr [esi + 16ch]
        inc    dx
loc_1_0A7E:
        in     al, dx
        test   al, 8
        loope  loc_1_0A7E
loc_1_0A83:
        jcxz   loc_1_0AC2
        in     al, dx
        and    al, 0f0h
        mov    bh, al
        shr    al, 4
        db  002h, 005h, 096h, 027h, 000h, 000h
        and    al, 0fh
        db  0A2h, 096h, 027h, 000h, 000h
        db  0A2h, 093h, 027h, 000h, 000h
        dec    dx
        test   di, 0ffffh
        je     SHORT loc_1_0AAE
        db  0A0h, 08Fh, 027h, 000h, 000h
loc_1_0AAE:
        shl    al, 1
        xor    al, 0f1h
        out    dx, al
        or     bl, bh
        mov    ecx, dword ptr [esi + 16ch]
        inc    dx
loc_1_0ABD:
        in     al, dx
        test   al, 8
        loopne loc_1_0ABD
loc_1_0AC2:
        jcxz   loc_1_0B35
        in     al, dx
        shr    al, 4
        mov    ah, al
        db  002h, 005h, 096h, 027h, 000h, 000h
        and    al, 0fh
        db  0A2h, 096h, 027h, 000h, 000h
        db  0A2h, 094h, 027h, 000h, 000h
        dec    dx
        test   di, 0ffffh
        je     SHORT loc_1_0AEB
        db  0A0h, 090h, 027h, 000h, 000h
loc_1_0AEB:
        shl    al, 1
        xor    al, 0f0h
        out    dx, al
        mov    ecx, dword ptr [esi + 16ch]
        inc    dx
loc_1_0AF8:
        in     al, dx
        test   al, 8
        loope  loc_1_0AF8
        jcxz   loc_1_0B35
        in     al, dx
        and    al, 0f0h
        mov    bh, al
        shr    al, 4
        db  002h, 005h, 096h, 027h, 000h, 000h
        and    al, 0fh
        db  0A2h, 096h, 027h, 000h, 000h
        db  0A2h, 095h, 027h, 000h, 000h
        dec    dx
        test   di, 0ffffh
        je     SHORT loc_1_0B28
        db  0A0h, 091h, 027h, 000h, 000h
loc_1_0B28:
        shl    al, 1
        xor    al, 0f1h
        out    dx, al
        or     ah, bh
        mov    al, bl
        pop    edx
        pop    edi
        clc
        ret
loc_1_0B35:
        pop    edx
        pop    edi
        stc
        ret
loc_1_0B39:
        db  0C7h, 005h, 099h, 027h, 000h, 000h, 038h, 064h, 048h, 054h
        mov    al, 0edh
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0fh
        out    dx, al
        mov    al, 0eh
        out    dx, al
        dec    dx
        mov    ecx, dword ptr [esi + 170h]
loc_1_0B58:
        call   sub_1_1046
        and    al, 0f0h
        cmp    al, 0e0h
        jne    SHORT loc_1_0B7A
        dec    dx
        mov    al, 0e5h
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0fh
        or     al, byte ptr [esi + 136h]
        out    dx, al
        jmp    NEAR PTR loc_1_0CF6
loc_1_0B7A:
        cmp    al, 80h
        je     SHORT loc_1_0B85
        loop   loc_1_0B58
        jmp    NEAR PTR loc_1_0CFE
loc_1_0B85:
        mov    edx, dword ptr [esi + 0c8h]
        db  0C6h, 005h, 096h, 027h, 000h, 000h, 000h
        mov    al, byte ptr [esi + 1ffh]
        db  000h, 005h, 096h, 027h, 000h, 000h
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0fh
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 0a0h
        call   sub_1_1676
        jae    SHORT loc_1_0BB8
        jmp    NEAR PTR loc_1_0CFE
loc_1_0BB8:
        mov    al, byte ptr [esi + 153h]
        db  000h, 005h, 096h, 027h, 000h, 000h
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0eh
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 0e0h
        call   sub_1_1676
        jae    SHORT loc_1_0BDE
        jmp    NEAR PTR loc_1_0CFE
loc_1_0BDE:
        inc    dx
        inc    dx
        mov    al, 0eeh
        out    dx, al
        dec    dx
        dec    dx
        in     al, dx
        db  000h, 005h, 096h, 027h, 000h, 000h
        cmp    al, 3
        jg     NEAR PTR loc_1_0D06
        mov    byte ptr [esi + 1fch], al
        inc    dx
        inc    dx
        mov    al, 0edh
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 20h
        call   sub_1_1676
        jae    SHORT loc_1_0C17
        jmp    NEAR PTR loc_1_0CFE
loc_1_0C17:
        in     al, dx
        db  03Ah, 005h, 096h, 027h, 000h, 000h
        jne    NEAR PTR loc_1_0D06
        db  000h, 005h, 096h, 027h, 000h, 000h
        mov    al, 0a5h
        db  000h, 005h, 096h, 027h, 000h, 000h
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0ch
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 60h
        call   sub_1_1676
        jae    SHORT loc_1_0C4C
        jmp    NEAR PTR loc_1_0CFE
loc_1_0C4C:
        db  0A1h, 074h, 027h, 000h, 000h
        db  000h, 005h, 096h, 027h, 000h, 000h
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0fh
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 0a0h
        call   sub_1_1676
        jae    SHORT loc_1_0C71
        jmp    NEAR PTR loc_1_0CFE
loc_1_0C71:
        mov    al, byte ptr [esi + 150h]
        db  000h, 005h, 096h, 027h, 000h, 000h
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0eh
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 0e0h
        call   sub_1_1676
        jae    SHORT loc_1_0C94
        jmp    SHORT loc_1_0CFE
loc_1_0C94:
        db  0A1h, 06Ch, 027h, 000h, 000h
        mov    bx, ax
        db  000h, 005h, 096h, 027h, 000h, 000h
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0dh
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 20h
        call   sub_1_1676
        jae    SHORT loc_1_0CB9
        jmp    SHORT loc_1_0CFE
loc_1_0CB9:
        mov    al, bh
        db  000h, 005h, 096h, 027h, 000h, 000h
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0ch
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 60h
        call   sub_1_1676
        jae    SHORT loc_1_0CD8
        jmp    SHORT loc_1_0CFE
loc_1_0CD8:
        db  0A0h, 096h, 027h, 000h, 000h
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0fh
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 0a0h
        call   sub_1_1676
        jae    SHORT loc_1_0CF4
        jmp    SHORT loc_1_0CFE
loc_1_0CF4:
        clc
        ret
loc_1_0CF6:
        inc    dword ptr [esi + 1c4h]
        jmp    SHORT loc_1_0D0C
loc_1_0CFE:
        inc    dword ptr [esi + 1a0h]
        jmp    SHORT loc_1_0D0C
loc_1_0D06:
        inc    dword ptr [esi + 198h]
loc_1_0D0C:
        mov    edx, dword ptr [esi + 0c8h]
        mov    al, 0e5h
        out    dx, al
        mov    al, 0fh
        or     al, byte ptr [esi + 136h]
        inc    dx
        inc    dx
        out    dx, al
        dec    dx
        dec    dx
        stc
        ret
loc_1_0D28:
        db  0C7h, 005h, 099h, 027h, 000h, 000h, 038h, 064h, 048h, 052h
        inc    dx
        inc    dx
        mov    al, 0efh
        out    dx, al
        dec    dx
        dec    dx
        mov    ecx, dword ptr [esi + 174h]
loc_1_0D43:
        inc    dx
        call   sub_1_1046
        and    al, 0f0h
        mov    ah, al
        dec    dx
        in     al, dx
        xor    ax, 0e0edh
        loopne loc_1_0D43
        jcxz   loc_1_0D5C
        jmp    SHORT loc_1_0D61
loc_1_0D5C:
        jmp    NEAR PTR loc_1_0EFB
loc_1_0D61:
        mov    edx, dword ptr [esi + 0c8h]
        inc    dx
        inc    dx
        mov    al, 0ebh
        out    dx, al
        dec    dx
        dec    dx
        db  0C6h, 005h, 096h, 027h, 000h, 000h, 000h
        mov    ah, 0a0h
        call   sub_1_1676
        jae    SHORT loc_1_0D87
        jmp    NEAR PTR loc_1_0EFB
loc_1_0D87:
        in     al, dx
        db  000h, 005h, 096h, 027h, 000h, 000h
        inc    dx
        inc    dx
        mov    al, 0efh
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 0e0h
        call   sub_1_1676
        jae    SHORT loc_1_0DA7
        jmp    NEAR PTR loc_1_0EFB
loc_1_0DA7:
        in     al, dx
        db  000h, 005h, 096h, 027h, 000h, 000h
        mov    byte ptr [esi + 1ffh], al
        mov    al, byte ptr [esi + 150h]
        db  000h, 005h, 096h, 027h, 000h, 000h
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0eh
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 20h
        call   sub_1_1676
        jae    SHORT loc_1_0DDA
        jmp    NEAR PTR loc_1_0EFB
loc_1_0DDA:
        db  0A0h, 096h, 027h, 000h, 000h
        db  000h, 005h, 096h, 027h, 000h, 000h
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0dh
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 60h
        call   sub_1_1676
        jae    SHORT loc_1_0DFF
        jmp    NEAR PTR loc_1_0EFB
loc_1_0DFF:
        inc    dx
        inc    dx
        mov    al, 0edh
        out    dx, al
        dec    dx
        dec    dx
        in     al, dx
        db  000h, 005h, 096h, 027h, 000h, 000h
        cmp    al, 0a5h
        je     SHORT loc_1_0E1A
        jmp    NEAR PTR loc_1_0F0B
loc_1_0E1A:
        inc    dx
        inc    dx
        mov    al, 0ech
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 0a0h
        call   sub_1_1676
        jae    SHORT loc_1_0E33
        jmp    NEAR PTR loc_1_0EFB
loc_1_0E33:
        in     al, dx
        db  000h, 005h, 096h, 027h, 000h, 000h
        movzx  eax, al
        db  0A3h, 080h, 027h, 000h, 000h
        inc    dx
        inc    dx
        mov    al, 0efh
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 0e0h
        call   sub_1_1676
        jae    SHORT loc_1_0E5B
        jmp    NEAR PTR loc_1_0EFB
loc_1_0E5B:
        in     al, dx
        db  000h, 005h, 096h, 027h, 000h, 000h
        cmp    al, 3
        jg     NEAR PTR loc_1_0F0B
        mov    byte ptr [esi + 1fch], al
        inc    dx
        inc    dx
        mov    al, 0eeh
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 20h
        call   sub_1_1676
        jae    SHORT loc_1_0E86
        jmp    SHORT loc_1_0EFB
loc_1_0E86:
        in     al, dx
        db  000h, 005h, 096h, 027h, 000h, 000h
        mov    bl, al
        inc    dx
        inc    dx
        mov    al, 0edh
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 60h
        call   sub_1_1676
        jae    SHORT loc_1_0EA5
        jmp    SHORT loc_1_0EFB
loc_1_0EA5:
        in     al, dx
        db  000h, 005h, 096h, 027h, 000h, 000h
        mov    bh, al
        cmp    bx, 1000h
        ja     SHORT loc_1_0F03
        cmp    bx, 1
        jl     SHORT loc_1_0F03
        mov    eax, dword ptr [esi + 40h]
        sub    eax, dword ptr [esi + 4ch]
        cmp    bx, ax
        jg     SHORT loc_1_0F03
        movzx  ebx, bx
        db  089h, 01Dh, 07Ch, 027h, 000h, 000h
        inc    dx
        inc    dx
        mov    al, 0ech
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 0a0h
        call   sub_1_1676
        jae    SHORT loc_1_0EE5
        jmp    SHORT loc_1_0EFB
loc_1_0EE5:
        in     al, dx
        db  03Ah, 005h, 096h, 027h, 000h, 000h
        jne    SHORT loc_1_0F0B
        inc    dx
        inc    dx
        mov    al, 0efh
        out    dx, al
        dec    dx
        dec    dx
        clc
        ret
loc_1_0EFB:
        inc    dword ptr [esi + 1a4h]
        jmp    SHORT loc_1_0F11
loc_1_0F03:
        inc    dword ptr [esi + 1dch]
        jmp    SHORT loc_1_0F11
loc_1_0F0B:
        inc    dword ptr [esi + 19ch]
loc_1_0F11:
        push   eax
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        mov    edx, dword ptr [esi + 0c8h]
        mov    al, 0e5h
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0fh
        or     al, byte ptr [esi + 136h]
        out    dx, al
        pop    eax
        clc
        ret
loc_1_0F31:
        db  0C7h, 005h, 099h, 027h, 000h, 000h, 034h, 054h, 000h, 000h
        mov    eax, 0ch
        inc    dx
        inc    dx
        out    dx, al
        sti
        dec    dx
        dec    dx
        mov    al, 0e3h
        out    dx, al
        inc    dx
        mov    ecx, dword ptr [esi + 170h]
loc_1_0F55:
        call   sub_1_1046
        cmp    al, 98h
        je     SHORT loc_1_0F65
        loop   loc_1_0F55
        jmp    NEAR PTR loc_1_1037
loc_1_0F65:
        dec    dx
        mov    ecx, dword ptr [esi + 16ch]
        shl    ecx, 2
        db  089h, 00Dh, 084h, 027h, 000h, 000h
        db  08Bh, 035h, 070h, 027h, 000h, 000h
        db  08Bh, 03Dh, 06Ch, 027h, 000h, 000h
        shr    edi, 1
        jae    SHORT loc_1_0F87
        inc    edi
loc_1_0F87:
        lodsw
        xor    ax, 8888h
        mov    bx, ax
        shl    al, 1
        out    dx, al
        mov    ah, bl
        shr    ah, 3
        or     ah, 1
        inc    dx
loc_1_0F9D:
        in     al, dx
        test   al, 8
        loopne loc_1_0F9D
        jcxz   loc_1_0FCC
        mov    al, ah
        dec    dx
        out    dx, al
        mov    ah, bh
        shl    ah, 1
        inc    dx
loc_1_0FB0:
        in     al, dx
        test   al, 8
        loope  loc_1_0FB0
        jcxz   loc_1_0FE1
        mov    al, ah
        dec    dx
        out    dx, al
        mov    ah, bh
        shr    ah, 3
        or     ah, 1
        inc    dx
loc_1_0FC7:
        in     al, dx
        test   al, 8
        loopne loc_1_0FC7
loc_1_0FCC:
        jcxz   loc_1_1019
        mov    al, ah
        dec    dx
        out    dx, al
        db  08Bh, 00Dh, 084h, 027h, 000h, 000h
        inc    dx
loc_1_0FDC:
        in     al, dx
        test   al, 8
        loope  loc_1_0FDC
loc_1_0FE1:
        jcxz   loc_1_1028
        dec    dx
        dec    di
        jne    SHORT loc_1_0F87
        mov    al, 0e5h
        out    dx, al
        xor    eax, eax
        db  087h, 00Dh, 06Ch, 027h, 000h, 000h
        clc
loc_1_0FF6:
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        mov    edx, dword ptr [esi + 0c8h]
        push   eax
        mov    al, 0e5h
        out    dx, al
        mov    al, 0ch
        or     al, byte ptr [esi + 136h]
        inc    dx
        inc    dx
        out    dx, al
        dec    dx
        dec    dx
        pop    eax
        ret
loc_1_1019:
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        inc    dword ptr [esi + 1b0h]
        stc
        jmp    SHORT loc_1_0FF6
loc_1_1028:
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        inc    dword ptr [esi + 1b4h]
        stc
        jmp    SHORT loc_1_0FF6
loc_1_1037:
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        inc    dword ptr [esi + 1a8h]
        stc
        jmp    SHORT loc_1_0FF6
sub_1_1046:
        push   ecx
        mov    ecx, 5
loc_1_104C:
        in     al, dx
        mov    ah, al
        in     al, dx
        and    ax, 0f8f8h
        cmp    ah, al
        loopne loc_1_104C
        pop    ecx
        ret
loc_1_105A:
        db  0C7h, 005h, 099h, 027h, 000h, 000h, 034h, 052h, 000h, 000h
        mov    al, 0ch
        inc    dx
        inc    dx
        out    dx, al
        sti
        mov    ecx, dword ptr [esi + 174h]
        dec    dx
loc_1_1074:
        call   sub_1_1046
        cmp    al, 98h
        loopne loc_1_1074
        jcxz   loc_1_10A3
        dec    dx
        mov    al, 0e3h
        out    dx, al
        mov    ecx, dword ptr [esi + 16ch]
        shl    ecx, 2
        db  089h, 00Dh, 084h, 027h, 000h, 000h
        db  08Bh, 035h, 07Ch, 027h, 000h, 000h
        shr    si, 1
        jae    SHORT loc_1_10A1
        inc    si
loc_1_10A1:
        jmp    SHORT loc_1_10A8
loc_1_10A3:
        jmp    NEAR PTR loc_1_114C
loc_1_10A8:
        db  08Bh, 03Dh, 078h, 027h, 000h, 000h
loc_1_10AE:
        db  08Bh, 00Dh, 084h, 027h, 000h, 000h
        inc    dx
loc_1_10B6:
        in     al, dx
        test   al, 8
        loopne loc_1_10B6
        jcxz   loc_1_10E8
        in     al, dx
        shl    ax, 4
        dec    dx
        out    dx, al
        mov    bl, ah
        inc    dx
loc_1_10CA:
        in     al, dx
        test   al, 8
        loope  loc_1_10CA
        jcxz   loc_1_10FA
        in     al, dx
        mov    bh, al
        or     al, 1
        dec    dx
        out    dx, al
        and    bx, 0f00fh
        or     bl, bh
        inc    dx
loc_1_10E3:
        in     al, dx
        test   al, 8
        loopne loc_1_10E3
loc_1_10E8:
        jcxz   loc_1_113D
        in     al, dx
        shl    ax, 4
        dec    dx
        out    dx, al
        inc    dx
loc_1_10F5:
        in     al, dx
        test   al, 8
        loope  loc_1_10F5
loc_1_10FA:
        jcxz   loc_1_112E
        in     al, dx
        and    ax, 0ff0h
        or     ah, al
        or     al, 1
        dec    dx
        out    dx, al
        mov    al, bl
        stosw
        dec    si
        jne    SHORT loc_1_10AE
        inc    dx
loc_1_1113:
        call   sub_1_1046
        cmp    al, 0a8h
        je     SHORT loc_1_1120
        loop   loc_1_1113
        jmp    SHORT loc_1_114C
loc_1_1120:
        db  08Bh, 00Dh, 07Ch, 027h, 000h, 000h
        db  0A1h, 080h, 027h, 000h, 000h
        clc
        jmp    SHORT loc_1_1159
loc_1_112E:
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        inc    dword ptr [esi + 1cch]
        stc
        jmp    SHORT loc_1_1159
loc_1_113D:
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        inc    dword ptr [esi + 1c8h]
        stc
        jmp    SHORT loc_1_1159
loc_1_114C:
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        inc    dword ptr [esi + 1ach]
        stc
loc_1_1159:
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        mov    edx, dword ptr [esi + 0c8h]
        push   eax
        mov    al, 0e5h
        out    dx, al
        mov    al, 0ch
        or     al, byte ptr [esi + 136h]
        inc    dx
        inc    dx
        out    dx, al
        dec    dx
        dec    dx
        pop    eax
        ret
loc_1_117C:
        db  0C7h, 005h, 099h, 027h, 000h, 000h, 038h, 078h, 054h, 055h
        mov    al, 0e3h
        out    dx, al
        inc    dx
        mov    ecx, dword ptr [esi + 170h]
loc_1_1191:
        call   sub_1_1046
        cmp    al, 98h
        je     SHORT loc_1_11C0
        loop   loc_1_1191
        jmp    SHORT loc_1_11AF
loc_1_119E:
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        inc    dword ptr [esi + 1b0h]
        jmp    NEAR PTR loc_1_127F
loc_1_11AF:
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        inc    dword ptr [esi + 1a8h]
        jmp    NEAR PTR loc_1_127F
loc_1_11C0:
        dec    dx
        mov    al, 0e8h
        mov    ah, 0eah
        mov    bl, 80h
        mov    bh, 90h
        stc
        call   sub_1_1AC6
        jb     SHORT loc_1_119E
        mov    al, 0f1h
        call   sub_1_1B44
        inc    dx
        mov    ecx, 4000h
loc_1_11E0:
        in     al, dx
        test   al, 8
        loopne loc_1_11E0
        jcxz   loc_1_11F9
        inc    dx
        mov    al, 4
        out    dx, al
        dec    dx
        mov    ecx, 4000h
loc_1_11F4:
        in     al, dx
        test   al, 10h
        loope  loc_1_11F4
loc_1_11F9:
        jcxz   loc_1_1263
        mov    ecx, dword ptr [esi + 16ch]
        mov    edi, ecx
        db  08Bh, 035h, 070h, 027h, 000h, 000h
        db  08Bh, 01Dh, 06Ch, 027h, 000h, 000h
        shr    bx, 1
        jae    SHORT loc_1_1217
        inc    bx
loc_1_1217:
        dec    dx
        outsb
        inc    dx
        inc    dx
        mov    al, 6
        out    dx, al
        dec    dx
        mov    ecx, edi
loc_1_1225:
        in     al, dx
        test   al, 80h
        loope  loc_1_1225
        jcxz   loc_1_1271
        dec    dx
        outsb
        inc    dx
        inc    dx
        mov    al, 4
        out    dx, al
        dec    dx
        mov    ecx, edi
loc_1_123B:
        in     al, dx
        test   al, 80h
        loopne loc_1_123B
        jcxz   loc_1_1271
        dec    bx
        jne    SHORT loc_1_1217
        db  087h, 00Dh, 06Ch, 027h, 000h, 000h
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        mov    edx, dword ptr [esi + 0c8h]
        stc
        call   sub_1_035E
        xor    eax, eax
        clc
        ret
loc_1_1263:
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        inc    dword ptr [esi + 1b4h]
        jmp    SHORT loc_1_127F
loc_1_1271:
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        inc    dword ptr [esi + 1b8h]
        jmp    SHORT loc_1_127F
loc_1_127F:
        push   eax
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        mov    edx, dword ptr [esi + 0c8h]
        stc
        call   sub_1_035E
        pop    eax
        stc
        ret
loc_1_1295:
        db  0C7h, 005h, 099h, 027h, 000h, 000h, 045h, 078h, 052h, 055h
        mov    ecx, dword ptr [esi + 174h]
        inc    dx
loc_1_12A7:
        call   sub_1_1046
        cmp    al, 98h
        loopne loc_1_12A7
        jcxz   loc_1_12F1
        dec    dx
        mov    al, 0e3h
        out    dx, al
        mov    al, 0e0h
        mov    ah, 0e2h
        mov    bl, 0c0h
        mov    bh, 0d0h
        clc
        call   sub_1_1AC6
        jb     SHORT loc_1_12FF
        inc    dx
        mov    ecx, 4000h
loc_1_12CF:
        in     al, dx
        test   al, 8
        loope  loc_1_12CF
        jcxz   loc_1_130D
        dec    dx
        mov    al, 0e1h
        call   sub_1_1B44
        inc    dx
        mov    ecx, 4000h
loc_1_12E7:
        in     al, dx
        test   al, 8
        loopne loc_1_12E7
        jcxz   loc_1_130D
        jmp    SHORT loc_1_133F
loc_1_12F1:
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        inc    dword ptr [esi + 1ach]
        jmp    SHORT loc_1_1329
loc_1_12FF:
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        inc    dword ptr [esi + 1c8h]
        jmp    SHORT loc_1_1329
loc_1_130D:
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        inc    dword ptr [esi + 1cch]
        jmp    SHORT loc_1_1329
loc_1_131B:
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        inc    dword ptr [esi + 1d0h]
        jmp    SHORT loc_1_1329
loc_1_1329:
        push   eax
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        mov    edx, dword ptr [esi + 0c8h]
        stc
        call   sub_1_035E
        pop    eax
        stc
        ret
loc_1_133F:
        mov    ecx, 4000h
loc_1_1344:
        in     al, dx
        test   al, 10h
        loope  loc_1_1344
        jcxz   loc_1_130D
        inc    dx
        mov    al, 0e4h
        out    dx, al
        mov    ecx, dword ptr [esi + 16ch]
        mov    esi, ecx
        db  08Bh, 03Dh, 078h, 027h, 000h, 000h
        db  08Bh, 01Dh, 07Ch, 027h, 000h, 000h
        shr    bx, 1
        jae    SHORT loc_1_136C
        inc    bx
loc_1_136C:
        dec    dx
loc_1_136E:
        in     al, dx
        test   al, 80h
        loope  loc_1_136E
        jcxz   loc_1_131B
        dec    dx
        insb
        inc    dx
        inc    dx
        mov    al, 0e6h
        out    dx, al
        dec    dx
        mov    ecx, esi
loc_1_1384:
        in     al, dx
        test   al, 80h
        loopne loc_1_1384
        jcxz   loc_1_131B
        dec    dx
        insb
        inc    dx
        inc    dx
        mov    al, 0e4h
        out    dx, al
        dec    dx
        mov    ecx, esi
        dec    bx
        jne    SHORT loc_1_136E
        db  08Bh, 00Dh, 07Ch, 027h, 000h, 000h
        db  0A1h, 080h, 027h, 000h, 000h
        push   eax
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        mov    edx, dword ptr [esi + 0c8h]
        stc
        call   sub_1_035E
        pop    eax
        clc
        ret
loc_1_13BF:
        db  0C7h, 005h, 099h, 027h, 000h, 000h, 045h, 078h, 038h, 054h
        pushfd
        cli
        mov    al, 5ah
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0dh
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 20h
        call   sub_1_1676
        jae    SHORT loc_1_13E7
        jmp    NEAR PTR loc_1_14A4
loc_1_13E7:
        inc    dx
        inc    dx
        add    dx, 400h
        mov    al, 75h
        out    dx, al
        sub    dx, 400h
        mov    al, 4
        out    dx, al
        dec    dx
        mov    ecx, 4000h
loc_1_1402:
        in     al, dx
        and    al, 0f0h
        cmp    al, 0d0h
        loopne loc_1_1402
        jcxz   loc_1_147A
        add    dx, 3ffh
        db  08Bh, 035h, 070h, 027h, 000h, 000h
        db  08Bh, 03Dh, 06Ch, 027h, 000h, 000h
        mov    bx, di
        and    bx, 0fh
        shr    di, 4
        inc    di
loc_1_142A:
        dec    di
        je     SHORT loc_1_144C
        mov    ecx, 10h
        rep outsb
        inc    dx
        inc    dx
        mov    ecx, 4000h
loc_1_143E:
        in     al, dx
        test   al, 1
        loope  loc_1_143E
        jcxz   loc_1_1488
        dec    dx
        dec    dx
        jmp    SHORT loc_1_142A
loc_1_144C:
        inc    dx
        inc    dx
        or     bx, bx
        je     SHORT loc_1_1462
        movzx  ecx, bx
        dec    dx
        dec    dx
        rep outsb
        inc    dx
        inc    dx
loc_1_1462:
        mov    ecx, 4000h
loc_1_1467:
        in     al, dx
        test   al, 1
        loope  loc_1_1467
        mov    al, 35h
        out    dx, al
        jcxz   loc_1_14B2
        db  087h, 00Dh, 06Ch, 027h, 000h, 000h
        jmp    SHORT loc_1_14F2
loc_1_147A:
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        inc    dword ptr [esi + 1b0h]
        jmp    SHORT loc_1_14C0
loc_1_1488:
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        inc    dword ptr [esi + 1b4h]
        jmp    SHORT loc_1_14C0
        db  08Bh, 035h, 068h, 027h, 000h, 000h, 0FFh, 086h, 0B8h, 001h, 000h, 000h, 0EBh, 01Ch
loc_1_14A4:
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        inc    dword ptr [esi + 1a8h]
        jmp    SHORT loc_1_14C0
loc_1_14B2:
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        inc    dword ptr [esi + 1dch]
        jmp    SHORT loc_1_14C0
loc_1_14C0:
        push   eax
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        mov    edx, dword ptr [esi + 0c8h]
        add    dx, 402h
        mov    al, 35h
        out    dx, al
        sub    dx, 402h
        mov    al, 0e5h
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0fh
        or     al, byte ptr [esi + 136h]
        out    dx, al
        dec    dx
        dec    dx
        pop    eax
        popfd
        stc
        ret
loc_1_14F2:
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        mov    edx, dword ptr [esi + 0c8h]
        mov    al, 0e5h
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0fh
        or     al, byte ptr [esi + 136h]
        out    dx, al
        dec    dx
        dec    dx
        popfd
        xor    eax, eax
        clc
        ret
loc_1_1517:
        db  0C7h, 005h, 099h, 027h, 000h, 000h, 045h, 078h, 038h, 054h
        pushfd
        cli
        mov    ah, 20h
        call   sub_1_1676
        jae    SHORT loc_1_1531
        jmp    NEAR PTR loc_1_1607
loc_1_1531:
        in     al, dx
        cmp    al, 5ah
        jne    NEAR PTR loc_1_1607
        inc    dx
        inc    dx
        mov    al, 0edh
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 70h
        call   sub_1_1676
        jae    SHORT loc_1_1553
        jmp    NEAR PTR loc_1_15F9
loc_1_1553:
        add    dx, 402h
        mov    al, 75h
        out    dx, al
        sub    dx, 400h
        mov    al, 0e0h
        out    dx, al
        mov    ecx, 4000h
        add    dx, 400h
        db  08Bh, 01Dh, 07Ch, 027h, 000h, 000h
        db  08Bh, 03Dh, 078h, 027h, 000h, 000h
loc_1_1579:
        in     al, dx
        test   al, 2
        jne    SHORT loc_1_1594
        test   al, 1
        loopne loc_1_1579
        jcxz   loc_1_15EB
        dec    dx
        dec    dx
        insb
        inc    dx
        inc    dx
        dec    bx
        je     SHORT loc_1_15C6
        jmp    SHORT loc_1_1579
loc_1_1594:
        mov    ecx, 10h
        dec    dx
        dec    dx
        rep insb
        inc    dx
        inc    dx
        sub    bx, 10h
        js     SHORT loc_1_15AD
        je     SHORT loc_1_15C6
        jmp    SHORT loc_1_1579
loc_1_15AD:
        jmp    SHORT loc_1_15C6
        db  0B9h, 000h, 010h, 000h, 000h, 0ECh, 0A8h, 001h, 075h, 05Ch, 066h, 04Ah, 066h, 04Ah, 0ECh, 066h
        db  042h, 066h, 042h, 0E2h, 0F0h, 0EBh, 04Fh
loc_1_15C6:
        mov    al, 35h
        out    dx, al
        sub    dx, 402h
        mov    al, 0e5h
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0fh
        or     al, byte ptr [esi + 136h]
        out    dx, al
        db  08Bh, 00Dh, 07Ch, 027h, 000h, 000h
        db  0A1h, 080h, 027h, 000h, 000h
        jmp    SHORT loc_1_1623
loc_1_15EB:
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        inc    dword ptr [esi + 1cch]
        jmp    SHORT loc_1_1648
loc_1_15F9:
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        inc    dword ptr [esi + 1c8h]
        jmp    SHORT loc_1_1648
loc_1_1607:
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        inc    dword ptr [esi + 1ach]
        jmp    SHORT loc_1_1648
        db  08Bh, 035h, 068h, 027h, 000h, 000h, 0FFh, 086h, 0DCh, 001h, 000h, 000h, 0EBh, 025h
loc_1_1623:
        push   eax
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        mov    edx, dword ptr [esi + 0c8h]
        mov    al, 0e5h
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0fh
        or     al, byte ptr [esi + 136h]
        out    dx, al
        dec    dx
        dec    dx
        pop    eax
        popfd
        clc
        ret
loc_1_1648:
        push   eax
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        mov    edx, dword ptr [esi + 0c8h]
        mov    al, 0e5h
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0fh
        or     al, byte ptr [esi + 136h]
        out    dx, al
        add    dx, 400h
        mov    al, 35h
        out    dx, al
        sub    dx, 402h
        pop    eax
        popfd
        stc
        ret
sub_1_1676:
        inc    dx
        mov    ecx, 4000h
loc_1_167D:
        in     al, dx
        and    al, 0f0h
        cmp    al, ah
        loopne loc_1_167D
        jcxz   loc_1_1693
        in     al, dx
        cmp    al, ah
        loopne loc_1_167D
        jcxz   loc_1_1693
        dec    dx
        clc
        ret
loc_1_1693:
        dec    dx
        stc
        ret
loc_1_1697:
        pushfd
        db  0C7h, 005h, 099h, 027h, 000h, 000h, 045h, 078h, 054h, 055h
        mov    al, 0e3h
        out    dx, al
        inc    dx
        mov    ecx, dword ptr [esi + 170h]
loc_1_16AD:
        call   sub_1_1046
        cmp    al, 98h
        je     SHORT loc_1_16F4
        loop   loc_1_16AD
        jmp    SHORT loc_1_16D1
loc_1_16BA:
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        mov    edx, dword ptr [esi + 0c8h]
        stc
        call   sub_1_035E
        xor    eax, eax
        popfd
        clc
        ret
loc_1_16D1:
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        inc    dword ptr [esi + 1a8h]
loc_1_16DD:
        push   eax
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        mov    edx, dword ptr [esi + 0c8h]
        stc
        call   sub_1_035E
        pop    eax
        popfd
        stc
        ret
loc_1_16F4:
        dec    dx
        mov    al, 0e8h
        mov    ah, 0eah
        mov    bl, 80h
        mov    bh, 90h
        cli
        stc
        call   sub_1_1AC6
        jb     NEAR PTR loc_1_17B8
        mov    al, 0f3h
        call   sub_1_1B44
        inc    dx
        mov    ecx, 4000h
loc_1_1719:
        in     al, dx
        test   al, 8
        loopne loc_1_1719
        jcxz   loc_1_1758
        inc    dx
        add    dx, 400h
        mov    al, 75h
        out    dx, al
        db  08Bh, 035h, 070h, 027h, 000h, 000h
        db  08Bh, 03Dh, 06Ch, 027h, 000h, 000h
        mov    bx, di
        and    bx, 0fh
        shr    di, 4
        inc    di
        sub    dx, 400h
        mov    al, 14h
        out    dx, al
        dec    dx
        mov    ecx, 4000h
loc_1_1753:
        in     al, dx
        test   al, 10h
        loope  loc_1_1753
loc_1_1758:
        jcxz   loc_1_17BA
        add    dx, 3ffh
loc_1_1760:
        dec    di
        je     SHORT loc_1_1782
        mov    ecx, 10h
        rep outsb
        inc    dx
        inc    dx
        mov    ecx, 4000h
loc_1_1774:
        in     al, dx
        test   al, 1
        loope  loc_1_1774
        jcxz   loc_1_17CC
        dec    dx
        dec    dx
        jmp    SHORT loc_1_1760
loc_1_1782:
        inc    dx
        inc    dx
        or     bx, bx
        je     SHORT loc_1_1798
        movzx  ecx, bx
        dec    dx
        dec    dx
        rep outsb
        inc    dx
        inc    dx
loc_1_1798:
        mov    ecx, 4000h
loc_1_179D:
        in     al, dx
        test   al, 1
        loope  loc_1_179D
        in     al, dx
        and    al, 1fh
        or     al, 35h
        out    dx, al
        jcxz   loc_1_17CC
        xor    ecx, ecx
        db  087h, 00Dh, 06Ch, 027h, 000h, 000h
        jmp    NEAR PTR loc_1_16BA
loc_1_17B8:
        nop
        nop
loc_1_17BA:
        nop
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        inc    dword ptr [esi + 1b8h]
        jmp    NEAR PTR loc_1_16DD
loc_1_17CC:
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        inc    dword ptr [esi + 1bch]
        jmp    NEAR PTR loc_1_16DD
loc_1_17DD:
        pushfd
        db  0C7h, 005h, 099h, 027h, 000h, 000h, 045h, 078h, 052h, 055h
        mov    ecx, dword ptr [esi + 174h]
        inc    dx
loc_1_17F0:
        call   sub_1_1046
        cmp    al, 98h
        loopne loc_1_17F0
        jcxz   loc_1_182C
        dec    dx
        mov    al, 0e3h
        out    dx, al
        mov    al, 0e0h
        mov    ah, 0e2h
        mov    bl, 0c0h
        mov    bh, 0d0h
        cli
        clc
        call   sub_1_1AC6
        jb     SHORT loc_1_185F
        inc    dx
        mov    ecx, 4000h
loc_1_1819:
        in     al, dx
        test   al, 8
        loope  loc_1_1819
        jcxz   loc_1_185F
        dec    dx
        mov    al, 0e3h
        call   sub_1_1B44
        jmp    SHORT loc_1_1870
loc_1_182C:
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        inc    dword ptr [esi + 1ach]
        jmp    NEAR PTR loc_1_1925
        db  08Bh, 035h, 068h, 027h, 000h, 000h, 0FFh, 086h, 0C8h, 001h, 000h, 000h, 0E9h, 0D7h, 000h, 000h
        db  000h, 08Bh, 035h, 068h, 027h, 000h, 000h, 0FFh, 086h, 0CCh, 001h, 000h, 000h, 0E9h, 0C6h, 000h
        db  000h, 000h
loc_1_185F:
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        inc    dword ptr [esi + 1d0h]
        jmp    NEAR PTR loc_1_1925
loc_1_1870:
        db  08Bh, 01Dh, 07Ch, 027h, 000h, 000h
        db  08Bh, 03Dh, 078h, 027h, 000h, 000h
        inc    dx
        mov    ecx, 4000h
loc_1_1883:
        in     al, dx
        and    al, 18h
        xor    al, 8
        cmp    al, 18h
        loopne loc_1_1883
        jcxz   loc_1_185F
        inc    dx
        mov    al, 14h
        out    dx, al
        add    dx, 400h
        in     al, dx
        and    al, 1fh
        or     al, 75h
        out    dx, al
        sub    dx, 400h
        mov    al, 20h
        out    dx, al
        mov    ecx, 4000h
        add    dx, 400h
loc_1_18B1:
        in     al, dx
        test   al, 2
        jne    SHORT loc_1_18CC
        test   al, 1
        loopne loc_1_18B1
        jcxz   loc_1_1900
        dec    dx
        dec    dx
        insb
        inc    dx
        inc    dx
        dec    bx
        je     SHORT loc_1_18E5
        jmp    SHORT loc_1_18B1
loc_1_18CC:
        mov    ecx, 10h
        dec    dx
        dec    dx
        rep insb
        inc    dx
        inc    dx
        sub    bx, 10h
        jns    SHORT loc_1_18E3
        jmp    SHORT loc_1_18E5
loc_1_18E3:
        jne    SHORT loc_1_18B1
loc_1_18E5:
        in     al, dx
        and    al, 1fh
        or     al, 35h
        out    dx, al
        sub    dx, 400h
        mov    al, 14h
        out    dx, al
        db  08Bh, 00Dh, 07Ch, 027h, 000h, 000h
        db  0A1h, 080h, 027h, 000h, 000h
        jmp    SHORT loc_1_190E
loc_1_1900:
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        inc    dword ptr [esi + 1d4h]
        jmp    SHORT loc_1_1925
loc_1_190E:
        push   eax
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        mov    edx, dword ptr [esi + 0c8h]
        stc
        call   sub_1_035E
        pop    eax
        popfd
        clc
        ret
loc_1_1925:
        push   eax
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        mov    edx, dword ptr [esi + 0c8h]
        stc
        call   sub_1_035E
        pop    eax
        popfd
        stc
        ret
loc_1_193C:
        pushfd
        db  0C7h, 005h, 099h, 027h, 000h, 000h, 050h, 078h, 054h, 055h
        mov    al, 0e3h
        out    dx, al
        inc    dx
        mov    ecx, dword ptr [esi + 170h]
loc_1_1952:
        call   sub_1_1046
        cmp    al, 98h
        loopne loc_1_1952
        jcxz   loc_1_1960
        jmp    SHORT loc_1_1962
loc_1_1960:
        jmp    SHORT loc_1_19CE
loc_1_1962:
        dec    dx
        mov    al, 0e8h
        mov    ah, 0eah
        mov    bl, 80h
        mov    bh, 90h
        cli
        stc
        call   sub_1_1AC6
        jb     SHORT loc_1_19CF
        mov    al, 0f2h
        call   sub_1_1B44
        inc    dx
        mov    ecx, 4000h
loc_1_1983:
        in     al, dx
        test   al, 8
        loopne loc_1_1983
        jcxz   loc_1_19D0
        inc    dx
        mov    al, 14h
        out    dx, al
        dec    dx
        mov    ecx, 4000h
loc_1_1997:
        in     al, dx
        test   al, 10h
        loope  loc_1_1997
        jcxz   loc_1_19D1
        db  08Bh, 035h, 070h, 027h, 000h, 000h
        db  08Bh, 00Dh, 06Ch, 027h, 000h, 000h
        add    dx, 3
        rep outsb
        db  08Bh, 00Dh, 06Ch, 027h, 000h, 000h
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        mov    edx, dword ptr [esi + 0c8h]
        stc
        call   sub_1_035E
        xor    eax, eax
        popfd
        clc
        ret
loc_1_19CE:
        nop
loc_1_19CF:
        nop
loc_1_19D0:
        nop
loc_1_19D1:
        nop
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        inc    dword ptr [esi + 1a8h]
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        mov    edx, dword ptr [esi + 0c8h]
        stc
        call   sub_1_035E
        popfd
        stc
        ret
loc_1_19F3:
        pushfd
        db  0C7h, 005h, 099h, 027h, 000h, 000h, 050h, 078h, 052h, 055h
        mov    ecx, dword ptr [esi + 174h]
        inc    dx
loc_1_1A06:
        call   sub_1_1046
        cmp    al, 98h
        loopne loc_1_1A06
        jcxz   loc_1_1A14
        jmp    SHORT loc_1_1A19
loc_1_1A14:
        jmp    NEAR PTR loc_1_1AA1
loc_1_1A19:
        dec    dx
        mov    al, 0e3h
        out    dx, al
        mov    al, 0e0h
        mov    ah, 0e2h
        mov    bl, 0c0h
        mov    bh, 0d0h
        cli
        clc
        call   sub_1_1AC6
        jb     SHORT loc_1_1AA2
        inc    dx
        mov    ecx, 4000h
loc_1_1A36:
        in     al, dx
        test   al, 8
        loope  loc_1_1A36
        jcxz   loc_1_1AA3
        mov    al, 0e2h
        dec    dx
        call   sub_1_1B44
        inc    dx
        mov    ecx, 4000h
loc_1_1A4E:
        in     al, dx
        and    al, 18h
        xor    al, 8
        cmp    al, 18h
        loopne loc_1_1A4E
        jcxz   loc_1_1AA4
        inc    dx
        mov    al, 0f4h
        out    dx, al
        inc    dx
        inc    dx
        db  08Bh, 03Dh, 078h, 027h, 000h, 000h
        db  08Bh, 00Dh, 07Ch, 027h, 000h, 000h
        rep insb
        dec    dx
        dec    dx
        mov    al, 0e4h
        out    dx, al
        mov    al, 0e6h
        out    dx, al
        mov    al, 0e4h
        out    dx, al
        mov    al, 0ech
        out    dx, al
        db  08Bh, 00Dh, 07Ch, 027h, 000h, 000h
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        mov    edx, dword ptr [esi + 0c8h]
        stc
        call   sub_1_035E
        db  0A1h, 080h, 027h, 000h, 000h
        popfd
        clc
        ret
loc_1_1AA1:
        nop
loc_1_1AA2:
        nop
loc_1_1AA3:
        nop
loc_1_1AA4:
        nop
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        inc    dword ptr [esi + 1d4h]
        db  08Bh, 035h, 068h, 027h, 000h, 000h
        mov    edx, dword ptr [esi + 0c8h]
        stc
        call   sub_1_035E
        popfd
        stc
        ret
sub_1_1AC6:
        push   edx
        pushfd
        jb     SHORT loc_1_1AE4
        push   eax
        inc    dx
        mov    ecx, 4000h
loc_1_1AD2:
        in     al, dx
        and    al, 0f8h
        cmp    al, bl
        loopne loc_1_1AD2
        pop    eax
        jcxz   loc_1_1B30
        dec    dx
        out    dx, al
        inc    dx
        jmp    SHORT loc_1_1AF6
loc_1_1AE4:
        out    dx, al
        mov    ecx, 4000h
        inc    dx
loc_1_1AEC:
        in     al, dx
        and    al, 0f8h
        cmp    al, bl
        loopne loc_1_1AEC
        jcxz   loc_1_1B34
loc_1_1AF6:
        popfd
        pushfd
        jb     SHORT loc_1_1B14
        mov    ecx, 4000h
loc_1_1AFF:
        in     al, dx
        and    al, 0f8h
        cmp    al, bh
        loopne loc_1_1AFF
        jcxz   loc_1_1B38
        dec    dx
        mov    al, ah
        out    dx, al
        xor    al, al
        popfd
        pop    edx
        clc
        ret
loc_1_1B14:
        dec    dx
        mov    al, ah
        out    dx, al
        inc    dx
        mov    ecx, 4000h
loc_1_1B20:
        in     al, dx
        and    al, 0f8h
        cmp    al, bh
        loopne loc_1_1B20
        jcxz   loc_1_1B38
        xor    al, al
        popfd
        clc
        pop    edx
        ret
loc_1_1B30:
        mov    al, 1
        jmp    SHORT loc_1_1B40
loc_1_1B34:
        mov    al, 2
        jmp    SHORT loc_1_1B40
loc_1_1B38:
        mov    al, 3
        jmp    SHORT loc_1_1B40
        db  0B0h, 004h, 0EBh, 000h
loc_1_1B40:
        popfd
        stc
        pop    edx
        ret
sub_1_1B44:
        clc
        mov    ah, al
        and    ah, 0fh
        je     SHORT loc_1_1B51
        test   al, 10h
        jne    SHORT loc_1_1B51
        stc
loc_1_1B51:
        out    dx, al
        push   ax
        in     al, 61h
        pop    ax
        inc    dx
        inc    dx
        mov    al, 0eh
        out    dx, al
        push   ax
        in     al, 61h
        pop    ax
        mov    al, 0ah
        out    dx, al
        mov    al, 0eh
        out    dx, al
        push   ax
        in     al, 61h
        pop    ax
        mov    al, 0ch
        jae    SHORT loc_1_1B77
        or     al, 0e0h
loc_1_1B77:
        out    dx, al
        push   ax
        in     al, 61h
        pop    ax
        dec    dx
        dec    dx
        mov    al, 0e5h
        out    dx, al
        ret
        db  0C3h
sub_1_1B87:
        pushad
        mov    byte ptr [esi + 128h], 1
        mov    byte ptr [esi + 148h], 1
        mov    eax, 11h
        mov    ecx, 0ach
        lea    edi, [esi + 148h]
        call   sub_1_03A8
        popad
        mov    byte ptr [esi + 128h], 1
        ret
sub_1_1BB4:
        pushad
        mov    byte ptr [esi + 148h], 4
        mov    eax, 11h
        mov    ecx, 0ach
        lea    edi, [esi + 148h]
        call   sub_1_03A8
        popad
        call   sub_1_1E13
        ret
sub_1_1BD8:
        pushad
        db  066h, 083h, 03Dh, 097h, 027h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_1CBB
        db  083h, 03Dh, 099h, 027h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_1CBB
        db  08Bh, 035h, 008h, 025h, 000h, 000h
        or     esi, esi
        je     NEAR PTR loc_1_1CBB
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     NEAR PTR loc_1_1CBB
loc_1_1C0E:
        mov    esi, eax
        mov    edx, dword ptr [esi + 0c8h]
        mov    al, byte ptr [esi + 128h]
        cmp    al, 0
        je     NEAR PTR loc_1_1CA6
        mov    ebx, dword ptr [esi + 178h]
        or     ebx, ebx
        je     SHORT loc_1_1CA6
        db  0A1h, 0A5h, 027h, 000h, 000h
        sub    eax, ebx
        cmp    eax, 7530h
        jle    SHORT loc_1_1C8A
        pushad
        mov    ebx, dword ptr [esi + 12ch]
        mov    edx, dword ptr [esi + 0e4h]
        mov    ecx, dword ptr [edx]
        push   ecx
        or     ecx, 30h
        mov    eax, dword ptr [esi + 0e0h]
        and    eax, ecx
        mov    dword ptr [edx], eax
        mov    ecx, 0ffffff5fh
        and    byte ptr [ebx], cl
        call   sub_1_1E13
        xor    ebx, ebx
        mov    dword ptr [esi + 178h], ebx
        pop    ebx
        cmp    dword ptr [esi + 0f4h], -1
        je     SHORT loc_1_1C87
        not    ebx
        and    eax, ebx
        je     SHORT loc_1_1C87
        mov    eax, 4
        call   sub_1_022B
loc_1_1C87:
        popad
        jmp    SHORT loc_1_1CA6
loc_1_1C8A:
        mov    byte ptr [esi + 148h], 3
        mov    eax, 11h
        mov    ecx, 0ach
        lea    edi, [esi + 148h]
        call   sub_1_03A8
loc_1_1CA6:
        mov    eax, esi
        db  08Bh, 035h, 008h, 025h, 000h, 000h
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_1_1CBB
        jmp    NEAR PTR loc_1_1C0E
loc_1_1CBB:
        popad
        ret
        db  02Eh, 08Bh, 0C0h
loc_1_1CC0:
        pushad
        pushfd
        db  0FFh, 005h, 09Dh, 027h, 000h, 000h
        call   sub_1_1D18
        popfd
        popad
        ret
loc_1_1CD0:
        xor    eax, eax
        call   sub_1_1D18
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        db  0A3h, 0A5h, 027h, 000h, 000h
        db  02Bh, 005h, 0A9h, 027h, 000h, 000h
        cmp    eax, 0bb8h
        jl     SHORT loc_1_1CFE
        call   sub_1_1BD8
        db  0A1h, 0A5h, 027h, 000h, 000h
        db  0A3h, 0A9h, 027h, 000h, 000h
loc_1_1CFE:
        mov    eax, 14h
        mov    edx, esi
        db  0BEh, 0D0h, 01Ch, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 010h, 025h, 000h, 000h
        ret
        db  090h
sub_1_1D18:
        cli
        or     eax, eax
        je     SHORT loc_1_1D49
        mov    edi, eax
        db  08Bh, 035h, 008h, 025h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_1D49
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_1_1D49
loc_1_1D31:
        cmp    edi, dword ptr [eax + 13ch]
        je     SHORT loc_1_1D43
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_1_1D49
        jmp    SHORT loc_1_1D31
loc_1_1D43:
        inc    byte ptr [eax + 135h]
loc_1_1D49:
        db  083h, 03Dh, 099h, 027h, 000h, 000h, 000h
        jne    SHORT loc_1_1DC4
        db  066h, 083h, 03Dh, 097h, 027h, 000h, 000h, 000h
        jne    SHORT loc_1_1DC4
        db  066h, 0FFh, 005h, 097h, 027h, 000h, 000h
        db  08Bh, 035h, 008h, 025h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_1DBD
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_1_1DBD
        sti
loc_1_1D76:
        mov    esi, eax
        mov    edx, dword ptr [esi + 0c8h]
        mov    al, byte ptr [esi + 128h]
        cmp    al, 0
        je     SHORT loc_1_1D9B
        inc    edx
        call   sub_1_1046
        and    al, 0f0h
        cmp    al, 0e0h
        jne    SHORT loc_1_1D9B
        call   sub_1_1E2C
        jmp    SHORT loc_1_1DAD
loc_1_1D9B:
        mov    eax, esi
        db  08Bh, 035h, 008h, 025h, 000h, 000h
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_1_1DAD
        jmp    SHORT loc_1_1D76
loc_1_1DAD:
        cli
        db  066h, 0FFh, 00Dh, 097h, 027h, 000h, 000h
        call   sub_1_1DC8
        sti
        clc
        ret
loc_1_1DBD:
        db  066h, 0FFh, 00Dh, 097h, 027h, 000h, 000h
loc_1_1DC4:
        sti
        stc
        ret
        db  090h
sub_1_1DC8:
        cli
        push   esi
        push   edi
        db  0C7h, 005h, 099h, 027h, 000h, 000h, 000h, 000h, 000h, 000h
        db  08Bh, 035h, 008h, 025h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_1E0F
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_1_1E0F
loc_1_1DE7:
        cmp    byte ptr [eax + 135h], 0
        je     SHORT loc_1_1E07
        mov    edi, eax
        mov    eax, dword ptr [edi + 13ch]
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        mov    byte ptr [edi + 135h], 0
        mov    eax, edi
loc_1_1E07:
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jne    SHORT loc_1_1DE7
loc_1_1E0F:
        pop    edi
        pop    esi
        sti
        ret
sub_1_1E13:
        push   eax
        push   edx
        mov    edx, dword ptr [esi + 0c8h]
        inc    dx
        inc    dx
        mov    al, 0ch
        out    dx, al
        mov    byte ptr [esi + 270h], 0
        pop    edx
        pop    eax
        ret
sub_1_1E2C:
        db  0FFh, 005h, 0A1h, 027h, 000h, 000h
        mov    edx, dword ptr [esi + 0c8h]
        mov    edi, dword ptr [esi + 114h]
        call   sub_1_0503
        jb     SHORT loc_1_1E72
        db  0C7h, 005h, 099h, 027h, 000h, 000h, 06Bh, 050h, 06Fh, 044h
        sti
        cmp    al, 13h
        jne    SHORT loc_1_1E61
        mov    edi, dword ptr [esi + 114h]
        call   sub_1_1E73
        jmp    SHORT loc_1_1E72
loc_1_1E61:
        cmp    al, 11h
        jne    SHORT loc_1_1E72
        mov    edi, dword ptr [esi + 114h]
        call   sub_1_1F31
        jmp    SHORT loc_1_1E72
loc_1_1E72:
        ret
sub_1_1E73:
        push   eax
        push   ebx
        push   edx
        push   esi
        push   ecx
        mov    edx, esi
        mov    eax, dword ptr [edx + 40h]
        sub    eax, dword ptr [edx + 4ch]
        cmp    ecx, eax
        jg     NEAR PTR loc_1_1F28
        mov    esi, edi
        mov    edi, dword ptr [edx + 54h]
        mov    eax, dword ptr [edx + 3ch]
        add    eax, dword ptr [edx + 40h]
        sub    eax, edi
        sub    eax, ecx
        mov    ebx, eax
        jns    SHORT loc_1_1E9D
        add    ecx, ebx
loc_1_1E9D:
        mov    edi, dword ptr [edx + 54h]
        cld
        shr    ecx, 1
        rep movsw
        jae    SHORT loc_1_1EA9
        movsb
loc_1_1EA9:
        cmp    ebx, 0
        jg     SHORT loc_1_1EBD
        mov    ecx, ebx
        neg    ecx
        mov    edi, dword ptr [edx + 3ch]
        shr    ecx, 1
        rep movsw
        jae    SHORT loc_1_1EBD
        movsb
loc_1_1EBD:
        mov    dword ptr [edx + 54h], edi
        pop    ecx
        add    dword ptr [edx + 4ch], ecx
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        mov    dword ptr [edx + 178h], eax
        mov    edi, dword ptr [edx + 130h]
        mov    dword ptr [edi], eax
        and    byte ptr [edx + 0e9h], 7fh
        cmp    dword ptr [edx + 0fch], 0
        je     SHORT loc_1_1F23
        cmp    dword ptr [edx + 11ch], -1
        je     SHORT loc_1_1F23
        test   byte ptr [edx + 129h], 20h
        je     SHORT loc_1_1F10
        test   byte ptr [edx + 0e9h], 1
        je     SHORT loc_1_1F05
        jmp    SHORT loc_1_1F23
loc_1_1F05:
        mov    ecx, dword ptr [edx + 4ch]
        cmp    ecx, dword ptr [edx + 11ch]
        jb     SHORT loc_1_1F23
loc_1_1F10:
        and    byte ptr [edx + 0e9h], 1
        mov    eax, 1
        mov    esi, edx
        call   sub_1_022B
loc_1_1F23:
        pop    esi
        pop    edx
        pop    ebx
        pop    eax
        ret
loc_1_1F28:
        inc    dword ptr [esi + 1dch]
        pop    ecx
        jmp    SHORT loc_1_1F23
sub_1_1F31:
        push   ebx
        push   edx
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        mov    dword ptr [esi + 178h], eax
        mov    ebx, dword ptr [esi + 12ch]
        mov    edx, dword ptr [esi + 0e4h]
        mov    al, byte ptr [edi]
        cmp    al, 4
        jne    SHORT loc_1_1F91
        mov    ecx, dword ptr [edx]
        push   ecx
        or     ecx, 30h
        mov    eax, dword ptr [esi + 0e0h]
        and    eax, ecx
        mov    dword ptr [edx], eax
        mov    ecx, 0ffffff5fh
        and    byte ptr [ebx], cl
        call   sub_1_1E13
        pop    ebx
        cmp    dword ptr [esi + 0f4h], -1
        je     NEAR PTR loc_1_2005
        not    ebx
        and    eax, ebx
        je     NEAR PTR loc_1_2005
        mov    eax, 4
        call   sub_1_022B
        jmp    SHORT loc_1_2005
loc_1_1F91:
        cmp    al, 2
        jne    SHORT loc_1_1FCF
        mov    ecx, dword ptr [edx]
        push   ecx
        or     ecx, 30h
        mov    ecx, 0a0h
        and    byte ptr [ebx], cl
        mov    dword ptr [edx], ecx
        mov    byte ptr [esi + 270h], 2
        mov    eax, dword ptr [esi + 0e0h]
        and    eax, ecx
        pop    ebx
        cmp    dword ptr [esi + 0f4h], -1
        je     SHORT loc_1_2005
        not    ebx
        and    eax, ebx
        je     SHORT loc_1_2005
        mov    eax, 4
        call   sub_1_022B
        jmp    SHORT loc_1_2005
loc_1_1FCF:
        cmp    al, 1
        jne    SHORT loc_1_2001
        pushad
        mov    byte ptr [esi + 128h], 1
        mov    byte ptr [esi + 148h], 2
        mov    eax, 11h
        mov    ecx, 0ach
        lea    edi, [esi + 148h]
        call   sub_1_03A8
        popad
        mov    byte ptr [esi + 128h], 2
        jmp    SHORT loc_1_2005
loc_1_2001:
        cmp    al, 3
        jne    SHORT loc_1_2005
loc_1_2005:
        pop    edx
        pop    ebx
        ret
sub_1_2008:
        pushad
        mov    edx, dword ptr [esi + 0c8h]
        call   sub_1_2037
        or     ax, ax
        je     SHORT loc_1_2035
        mov    al, 5
        mov    byte ptr [esi + 152h], al
        mov    edx, dword ptr [esi + 0c8h]
        inc    dx
        inc    dx
        add    dx, 400h
        mov    al, 35h
        out    dx, al
        jmp    SHORT loc_1_2035
loc_1_2035:
        popad
        ret
sub_1_2037:
        push   ecx
        push   edx
        add    dx, 402h
        in     al, dx
        push   eax
        mov    al, 0c0h
        mov    ah, al
        out    dx, al
        push   ax
        in     al, 61h
        pop    ax
        in     al, dx
        and    al, 0e0h
        cmp    al, ah
        jne    SHORT loc_1_209F
        sub    cx, cx
loc_1_2055:
        in     al, dx
        test   al, 2
        jne    SHORT loc_1_206E
        dec    dx
        dec    dx
        out    dx, al
        inc    dx
        inc    dx
        inc    cx
        cmp    cx, 800h
        jae    SHORT loc_1_209F
        jmp    SHORT loc_1_2055
loc_1_206E:
        mov    word ptr [esi + 154h], cx
        sub    cx, cx
loc_1_2078:
        in     al, dx
        test   al, 1
        jne    SHORT loc_1_2091
        dec    dx
        dec    dx
        in     al, dx
        inc    dx
        inc    dx
        inc    cx
        cmp    cx, 800h
        jae    SHORT loc_1_209F
        jmp    SHORT loc_1_2078
loc_1_2091:
        mov    word ptr [esi + 156h], cx
        pop    eax
        out    dx, al
        mov    ax, cx
        jmp    SHORT loc_1_20B2
loc_1_209F:
        pop    eax
        out    dx, al
        sub    ax, ax
        mov    word ptr [esi + 154h], ax
        mov    word ptr [esi + 156h], ax
loc_1_20B2:
        pop    edx
        pop    ecx
        ret
        db  0FAh, 053h, 051h, 052h, 066h, 033h, 0D2h, 066h, 0C7h, 005h, 0B0h, 027h, 000h, 000h, 00Eh, 003h
        db  066h, 0C7h, 005h, 0B2h, 027h, 000h, 000h, 000h, 043h, 0E8h, 0F7h, 000h, 000h, 000h, 066h, 03Dh
        db  086h, 004h, 075h, 012h, 066h, 0C7h, 005h, 0B0h, 027h, 000h, 000h, 00Ah, 007h, 066h, 0C7h, 005h
        db  0B2h, 027h, 000h, 000h, 000h, 044h, 066h, 03Dh, 086h, 003h, 074h, 00Bh, 066h, 03Dh, 086h, 004h
        db  074h, 005h, 0E9h, 0C6h, 000h, 000h, 000h, 0E4h, 070h, 050h, 0E4h, 074h, 050h, 0E4h, 076h, 050h
        db  0B0h, 010h, 0E6h, 070h, 0E4h, 074h, 03Ch, 010h, 075h, 005h, 0E9h, 0A5h, 000h, 000h, 000h, 0B0h
        db  05Ah, 0E6h, 074h, 0E4h, 074h, 024h, 07Fh, 03Ch, 05Ah, 074h, 005h, 0E9h, 094h, 000h, 000h, 000h
        db  0B0h, 025h, 0E6h, 074h, 0E4h, 074h, 024h, 07Fh, 03Ch, 025h, 074h, 005h, 0E9h, 083h, 000h, 000h
        db  000h, 0B0h, 000h, 0E6h, 074h, 0E4h, 076h, 050h, 0F6h, 0D0h, 08Ah, 0E0h, 0E6h, 076h, 0E4h, 076h
        db  038h, 0C4h, 075h, 070h, 058h, 0E6h, 076h, 0B0h, 001h, 0E6h, 074h, 0E4h, 076h, 050h, 0F6h, 0D0h
        db  08Ah, 0E0h, 0E6h, 076h, 0E4h, 076h, 038h, 0C4h, 075h, 05Ah, 058h, 0E6h, 076h, 066h, 0E5h, 022h
        db  066h, 08Bh, 0C8h, 066h, 0A9h, 001h, 000h, 075h, 017h, 066h, 0B8h, 000h, 080h, 0E6h, 023h, 066h
        db  050h, 0E4h, 061h, 0E4h, 061h, 0E4h, 061h, 066h, 058h, 086h, 0C4h, 0E6h, 022h, 066h, 0E7h, 022h
        db  0E4h, 022h, 024h, 0F0h, 00Ch, 003h, 0E6h, 022h, 066h, 08Bh, 015h, 0B0h, 027h, 000h, 000h, 066h
        db  0EDh, 066h, 08Bh, 0D0h, 066h, 0B8h, 000h, 080h, 0E6h, 023h, 086h, 0C4h, 0E6h, 022h, 066h, 0E7h
        db  022h, 066h, 08Bh, 0C1h, 066h, 0E7h, 022h, 066h, 0A9h, 001h, 000h, 075h, 007h, 066h, 00Dh, 000h
        db  001h, 066h, 0E7h, 022h, 058h, 0E6h, 074h, 058h, 0E6h, 076h, 058h, 0E6h, 070h, 066h, 08Bh, 0C2h
        db  0FBh, 05Ah, 059h, 05Bh, 0C3h, 09Ch, 051h, 052h, 08Bh, 0D4h, 066h, 083h, 0E4h, 0FCh, 09Ch, 058h
        db  08Bh, 0C8h, 035h, 000h, 000h, 004h, 000h, 050h, 09Dh, 09Ch, 058h, 08Bh, 0E2h, 066h, 0BAh, 086h
        db  003h, 033h, 0C1h, 074h, 017h, 066h, 0BAh, 086h, 004h, 08Bh, 0C1h, 035h, 000h, 000h, 020h, 000h
        db  050h, 09Dh, 09Ch, 058h, 033h, 0C1h, 074h, 004h, 066h, 0BAh, 086h, 005h, 066h, 08Bh, 0C2h, 05Ah
        db  059h, 09Dh, 0C3h, 0B4h, 0E2h, 0CDh, 017h, 080h, 0FCh, 0E2h, 075h, 003h, 066h, 033h, 0C0h, 0C3h
        db  053h, 051h, 052h, 066h, 0E5h, 022h, 066h, 08Bh, 0C8h, 066h, 0A9h, 001h, 000h, 075h, 00Dh, 066h
        db  0B8h, 000h, 080h, 0E6h, 023h, 086h, 0C4h, 0E6h, 022h, 066h, 0E7h, 022h, 0E4h, 022h, 024h, 0F0h
        db  00Ch, 003h, 0E6h, 022h, 066h, 08Bh, 015h, 0B0h, 027h, 000h, 000h, 066h, 0EDh, 066h, 08Bh, 01Dh
        db  0B2h, 027h, 000h, 000h, 066h, 033h, 0D2h, 038h, 0FCh, 075h, 053h, 066h, 0B8h, 000h, 001h, 066h
        db  0E7h, 022h, 0E4h, 023h, 08Ah, 0D8h, 0A8h, 080h, 074h, 014h, 066h, 0BAh, 023h, 0FCh, 0ECh, 066h
        db  0BAh, 023h, 0F0h, 0ECh, 066h, 0BAh, 023h, 0C0h, 0ECh, 066h, 0BAh, 023h, 000h, 0ECh, 0B0h, 061h
        db  0E6h, 024h, 0E4h, 025h, 08Ah, 0F8h, 00Ch, 008h, 0E6h, 025h, 0E6h, 0FBh, 0B0h, 002h, 0E6h, 0AEh
        db  0E4h, 0AFh, 066h, 08Bh, 0D0h, 0E6h, 0AFh, 0E6h, 0F9h, 0B0h, 061h, 0E6h, 024h, 08Ah, 0C7h, 0E6h
        db  025h, 0F6h, 0C3h, 080h, 074h, 008h, 0B0h, 0FAh, 0E6h, 024h, 0B0h, 001h, 0E6h, 025h, 066h, 0B8h
        db  000h, 080h, 0E6h, 023h, 086h, 0C4h, 0E6h, 022h, 066h, 0E7h, 022h, 066h, 08Bh, 0C1h, 066h, 0E7h
        db  022h, 066h, 0A9h, 001h, 000h, 075h, 007h, 066h, 00Dh, 000h, 001h, 066h, 0E7h, 022h, 066h, 08Bh
        db  0C2h, 05Ah, 059h, 05Bh, 0C3h
sub_1_22CA:
        push   ebx
        push   ecx
        push   edx
        pushfd
        cli
        mov    edx, dword ptr [esi + 0c8h]
        inc    dword ptr [esi + 1e0h]
        mov    al, 0e5h
        out    dx, al
        inc    dx
        inc    dx
        in     al, dx
        mov    ch, al
        and    ch, 10h
        mov    al, 0ch
        or     al, ch
        out    dx, al
        push   ax
        in     al, 61h
        pop    ax
        mov    al, 0eh
        or     al, ch
        out    dx, al
        dec    dx
        dec    dx
        mov    al, 0
        out    dx, al
        mov    al, 0e0h
        out    dx, al
        inc    dx
        in     al, dx
        mov    bh, al
        dec    dx
        mov    al, 0e1h
        out    dx, al
        inc    dx
        in     al, dx
        mov    bl, al
        dec    dx
        mov    al, 0e2h
        out    dx, al
        inc    dx
        in     al, dx
        mov    ah, al
        dec    dx
        mov    al, 0e3h
        out    dx, al
        inc    dx
        in     al, dx
        push   eax
        inc    dx
        mov    al, 0ch
        or     al, ch
        out    dx, al
        dec    dx
        dec    dx
        mov    al, 0e5h
        out    dx, al
        pop    eax
        and    bx, 7878h
        and    ax, 7878h
        shl    bh, 1
        shr    bl, 3
        or     bl, bh
        shl    ah, 1
        shr    al, 3
        or     al, ah
        cmp    bl, 0cah
        jne    SHORT loc_1_23BA
        cmp    al, 2
        je     SHORT loc_1_2359
        cmp    al, 1
        je     SHORT loc_1_2359
        jmp    SHORT loc_1_23BA
loc_1_2359:
        cmp    byte ptr [esi + 152h], 0
        jne    SHORT loc_1_23A4
        mov    byte ptr [esi + 152h], 2
        inc    dx
        inc    dx
        in     al, dx
        mov    ch, al
        and    ch, 10h
        mov    al, 0ech
        or     al, ch
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 0a5h
        mov    al, ah
        out    dx, al
        inc    dx
        in     al, dx
        dec    dx
        in     al, dx
        cmp    al, ah
        jne    SHORT loc_1_23A4
        xor    ah, 0ffh
        mov    al, ah
        out    dx, al
        inc    dx
        in     al, dx
        dec    dx
        in     al, dx
        cmp    al, ah
        jne    SHORT loc_1_23A4
        mov    byte ptr [esi + 152h], 1
        jne    SHORT loc_1_23A4
loc_1_23A4:
        mov    byte ptr [esi + 151h], 3
        inc    dx
        inc    dx
        mov    al, 0ch
        or     al, ch
        out    dx, al
        dec    dx
        dec    dx
        jmp    SHORT loc_1_2432
loc_1_23BA:
        inc    dx
        inc    dx
        mov    ebx, 3
loc_1_23C3:
        in     al, dx
        mov    ah, al
        and    ah, 10h
        mov    al, 4
        or     al, ah
        out    dx, al
        push   ax
        in     al, 61h
        in     al, 61h
        in     al, 61h
        in     al, 61h
        in     al, 61h
        pop    ax
        dec    dx
        mov    ecx, 5
loc_1_23E3:
        in     al, dx
        test   al, 8
        je     SHORT loc_1_2420
        loop   loc_1_23E3
        inc    dx
        mov    al, 0ch
        or     al, ah
        out    dx, al
        push   ax
        in     al, 61h
        in     al, 61h
        in     al, 61h
        in     al, 61h
        in     al, 61h
        pop    ax
        dec    dx
        mov    ecx, 5
loc_1_2406:
        in     al, dx
        test   al, 8
        jne    SHORT loc_1_2420
        loop   loc_1_2406
        inc    dx
        dec    bx
        jg     SHORT loc_1_23C3
        dec    dx
        dec    dx
        mov    byte ptr [esi + 151h], 2
        jmp    SHORT loc_1_2432
loc_1_2420:
        inc    dx
        mov    al, 0ch
        or     al, ah
        out    dx, al
        dec    dx
        dec    dx
        mov    byte ptr [esi + 151h], 0
loc_1_2432:
        movzx  eax, byte ptr [esi + 152h]
        movzx  ebx, byte ptr [esi + 151h]
        imul   eax, eax, 4
        db  08Ah, 084h, 003h, 038h, 025h, 000h, 000h
        mov    byte ptr [esi + 150h], al
        clc
        call   sub_1_02E4
        popfd
        pop    edx
        pop    ecx
        pop    ebx
        ret
        db  000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 001h, 000h, 000h, 000h, 050h, 041h, 052h, 041h
        db  04Ch, 049h, 04Eh, 04Bh, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  034h, 000h, 000h, 000h, 004h, 003h, 000h, 000h, 050h, 000h, 000h, 000h, 059h, 000h, 000h, 000h
        db  02Ch, 003h, 000h, 000h, 0F9h, 003h, 000h, 000h, 060h, 000h, 000h, 000h, 022h, 004h, 000h, 000h
        db  042h, 004h, 000h, 000h, 085h, 000h, 000h, 000h, 07Dh, 004h, 000h, 000h, 0A3h, 004h, 000h, 000h
        db  025h, 001h, 000h, 000h, 050h, 001h, 000h, 000h, 0A1h, 001h, 000h, 000h, 078h, 005h, 000h, 000h
        db  021h, 005h, 000h, 000h, 0C9h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 0D0h, 005h, 000h, 000h
        db  0F3h, 005h, 000h, 000h, 01Dh, 006h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 0F4h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h
        db  000h, 000h, 001h, 000h, 000h, 000h, 001h, 001h, 000h, 000h, 001h, 002h, 000h, 000h, 001h, 002h
        db  000h, 000h, 003h, 003h
loc_1_2550:
        db  0A0h, 025h, 000h, 000h, 0A0h
        db  025h, 000h, 000h, 0B8h, 025h
        add    byte ptr [eax], al
        db  0D0h, 025h, 000h, 000h
sub_1_2560:
        out    5, eax
        add    byte ptr [eax], al
        out    5, eax
        add    byte ptr [eax], al
        cmp    dword ptr [ebx], ecx
        add    byte ptr [eax], al
        out    5, eax
        add    byte ptr [eax], al
sub_1_2570:
        mov    word ptr [eax], cs
        add    byte ptr [eax], al
        mov    word ptr [eax], cs
        add    byte ptr [eax], al
        db  028h, 00Dh, 000h, 000h, 08Ch, 008h
        add    byte ptr [eax], al
loc_1_2580:
        db  0E8h, 025h, 000h, 000h, 0E8h
        db  025h, 000h, 000h, 028h, 026h
        add    byte ptr [eax], al
        db  068h, 026h, 000h, 000h, 0A8h
        add    byte ptr es:[eax], al
        test   al, 26h
        add    byte ptr [eax], al
        db  0E8h, 026h, 000h, 000h, 028h
        daa
        add    byte ptr [eax], al
loc_1_25A0:
        or     eax, dword ptr [ebx]
        add    byte ptr [eax], al
        or     eax, dword ptr [ebx]
        add    byte ptr [eax], al
        or     eax, dword ptr [ebx]
        add    byte ptr [eax], al
        or     eax, dword ptr [ebx]
        add    byte ptr [eax], al
        or     eax, dword ptr [ebx]
        add    byte ptr [eax], al
        or     eax, dword ptr [ebx]
        add    byte ptr [eax], al
loc_1_25B8:
        inc    edi
        add    eax, dword ptr [eax]
        db  000h, 009h
        add    eax, dword ptr [eax]
        db  000h, 047h, 003h
        add    byte ptr [eax], al
        inc    edi
        add    eax, dword ptr [eax]
        db  000h, 047h, 003h
        add    byte ptr [eax], al
        and    al, byte ptr [ebx]
        add    byte ptr [eax], al
loc_1_25D0:
        pop    esi
        add    eax, dword ptr [eax]
        db  000h, 05Eh, 003h
        add    byte ptr [eax], al
        pop    esi
        add    eax, dword ptr [eax]
        db  000h, 05Eh, 003h
        add    byte ptr [eax], al
        pop    esi
        add    eax, dword ptr [eax]
        db  000h, 095h, 003h, 000h, 000h, 031h
        sldt   word ptr [eax]
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
loc_1_2628:
        db  001h, 005h, 000h, 000h, 001h, 005h
        add    byte ptr [eax], al
        db  001h, 005h, 000h, 000h, 001h, 005h
        add    byte ptr [eax], al
        db  001h, 005h, 000h, 000h, 001h, 005h
        add    byte ptr [eax], al
        db  001h, 005h, 000h, 000h, 001h, 005h
        add    byte ptr [eax], al
        db  001h, 005h, 000h, 000h, 001h, 005h
        add    byte ptr [eax], al
        db  001h, 005h, 000h, 000h, 001h, 005h
        add    byte ptr [eax], al
        db  001h, 005h, 000h, 000h, 001h, 005h
        add    byte ptr [eax], al
        db  001h, 005h, 000h, 000h, 0BFh, 013h
        add    byte ptr [eax], al
loc_1_2668:
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        db  07Ch, 011h
        add    byte ptr [eax], al
        db  07Ch, 011h
        add    byte ptr [eax], al
        db  07Ch, 011h
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        db  07Ch, 011h
        add    byte ptr [eax], al
        cmp    al, 19h
        add    byte ptr [eax], al
        cmp    al, 19h
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        db  07Ch, 011h
        add    byte ptr [eax], al
        xchg   edi, eax
        push   ss
        add    byte ptr [eax], al
        xchg   edi, eax
        push   ss
        add    byte ptr [eax], al
loc_1_26A8:
        pop    edx
        adc    byte ptr [eax], al
        db  000h, 05Ah, 010h
        add    byte ptr [eax], al
        pop    edx
        adc    byte ptr [eax], al
        db  000h, 05Ah, 010h
        add    byte ptr [eax], al
        pop    edx
        adc    byte ptr [eax], al
        db  000h, 05Ah, 010h
        add    byte ptr [eax], al
        pop    edx
        adc    byte ptr [eax], al
        db  000h, 05Ah, 010h
        add    byte ptr [eax], al
        pop    edx
        adc    byte ptr [eax], al
        db  000h, 05Ah, 010h
        add    byte ptr [eax], al
        pop    edx
        adc    byte ptr [eax], al
        db  000h, 05Ah, 010h
        add    byte ptr [eax], al
        pop    edx
        adc    byte ptr [eax], al
        db  000h, 05Ah, 010h
        add    byte ptr [eax], al
        pop    edx
        adc    byte ptr [eax], al
        db  000h, 05Ah, 010h
        add    byte ptr [eax], al
loc_1_26E8:
        in     eax, 5
        add    byte ptr [eax], al
        in     eax, 5
        add    byte ptr [eax], al
        in     eax, 5
        add    byte ptr [eax], al
        in     eax, 5
        add    byte ptr [eax], al
        in     eax, 5
        add    byte ptr [eax], al
        in     eax, 5
        add    byte ptr [eax], al
        in     eax, 5
        add    byte ptr [eax], al
        in     eax, 5
        add    byte ptr [eax], al
        in     eax, 5
        add    byte ptr [eax], al
        in     eax, 5
        add    byte ptr [eax], al
        in     eax, 5
        add    byte ptr [eax], al
        in     eax, 5
        add    byte ptr [eax], al
        in     eax, 5
        add    byte ptr [eax], al
        in     eax, 5
        add    byte ptr [eax], al
        in     eax, 5
        add    byte ptr [eax], al
        pop    ss
        db  015h, 000h, 000h, 05Ah, 010h
        add    byte ptr [eax], al
        pop    edx
        adc    byte ptr [eax], al
        db  000h, 05Ah, 010h
        add    byte ptr [eax], al
        pop    edx
        adc    byte ptr [eax], al
        db  000h, 05Ah, 010h
        add    byte ptr [eax], al
        xchg   ebp, eax
        adc    al, byte ptr [eax]
        db  000h, 095h, 012h, 000h, 000h, 095h
        adc    al, byte ptr [eax]
        db  000h, 05Ah, 010h
        add    byte ptr [eax], al
        xchg   ebp, eax
        adc    al, byte ptr [eax]
        db  000h, 0F3h
        sbb    dword ptr [eax], eax
        db  000h, 0F3h
        sbb    dword ptr [eax], eax
        db  000h, 05Ah, 010h
        add    byte ptr [eax], al
        xchg   ebp, eax
        adc    al, byte ptr [eax]
        db  000h, 0DDh
        pop    ss
        add    byte ptr [eax], al
        fst    qword ptr [edi]
        add    byte ptr [eax], al
loc_1_2768:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_276C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2770:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2774:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2778:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_277C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2780:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2784:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2788:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_278C:
        add    byte ptr [eax], al
loc_1_278E:
        add    byte ptr [eax], al
loc_1_2790:
        add    byte ptr [eax], al
loc_1_2792:
        add    byte ptr [eax], al
loc_1_2794:
        add    byte ptr [eax], al
loc_1_2796:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_27B0:
        add    byte ptr [eax], al
loc_1_27B2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end
