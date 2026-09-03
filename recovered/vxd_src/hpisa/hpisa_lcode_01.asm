; hpisa_lcode_01.asm -- HPISA object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        db  052h, 00Fh, 0B7h, 054h, 024h, 008h, 033h, 0C0h, 0ECh, 05Ah, 0C3h, 090h, 052h, 00Fh, 0B7h, 054h
        db  024h, 008h, 08Ah, 044h, 024h, 00Ch, 0EEh, 0E4h, 061h, 05Ah, 0C3h, 090h
sub_1_001C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edx
        push   ecx
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        movzx  edx, word ptr [edi + 2]
        mov    word ptr [edi + 34h], 0
        mov    ax, cx
        and    al, 0feh
        mov    word ptr [edi + 36h], ax
        test   cx, 1
        je     SHORT loc_1_0053
        add    esi, ecx
        mov    ah, 0ffh
        mov    al, byte ptr [esi - 1]
        sub    esi, ecx
        mov    word ptr [edi + 34h], ax
loc_1_0053:
        test   byte ptr [edi + 8], 2
        je     SHORT loc_1_0071
        xor    dl, 8
        movzx  eax, byte ptr [edi + 3bh]
        xchg   al, ah
        out    dx, ax
        xor    dl, 4
        shr    cx, 1
        rep outsw
        xor    eax, eax
        jmp    SHORT loc_1_00C4
loc_1_0071:
        xor    dl, 1ah
        mov    al, 4
        out    dx, al
        xor    dl, 2
        out    dx, al
        xor    dl, 8
        mov    al, 0ah
        out    dx, al
        push   ecx
        mov    ecx, 40h
        xor    dl, 8
loc_1_008A:
        in     al, dx
        cmp    al, 4
        loope  loc_1_008A
        pop    ecx
        xor    eax, eax
        xor    dl, 0
        out    dx, al
        movzx  eax, byte ptr [edi + 3bh]
        xor    dl, 1
        out    dx, al
        mov    ax, cx
        and    al, 0feh
        xor    dl, 3
        out    dx, al
        xchg   al, ah
        xor    dl, 1
        out    dx, al
        xor    dl, 0bh
        mov    al, 12h
        out    dx, al
        xor    dl, 1ch
        cld
        shr    cx, 1
        rep outsw
        call   sub_1_068B
        jne    SHORT loc_1_00CA
loc_1_00C4:
        pop    edi
        pop    ecx
        pop    edx
        pop    esi
        leave
        ret
loc_1_00CA:
        mov    word ptr [edi + 34h], 0
        mov    word ptr [edi + 36h], 0
        jmp    SHORT loc_1_00C4
sub_1_00D8:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        push   edx
        push   ecx
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        movzx  edx, word ptr [edi + 2]
        xor    ecx, ecx
        mov    eax, dword ptr [ebp + 10h]
        or     ecx, eax
        je     NEAR PTR loc_1_01D7
        movzx  ebx, word ptr [edi + 34h]
        or     ebx, ebx
        je     SHORT loc_1_0101
        dec    ecx
        inc    eax
loc_1_0101:
        and    ax, 0fffeh
        je     NEAR PTR loc_1_01E0
        add    word ptr [edi + 36h], ax
        test   byte ptr [edi + 8], 2
        je     NEAR PTR loc_1_0194
        test   byte ptr [edi + 8], 4
        je     SHORT loc_1_0164
        push   edi
        mov    edi, dword ptr [edi + 20h]
        xor    dl, 4
        in     ax, dx
        and    ax, 0efffh
        out    dx, ax
        push   eax
        or     ebx, ebx
        je     SHORT loc_1_013B
        mov    bh, byte ptr [esi]
        inc    esi
        mov    word ptr [edi], bx
        xor    ebx, ebx
loc_1_013B:
        mov    eax, ecx
        shr    ecx, 2
        jecxz  loc_1_0146
        cld
        nop
        rep movsd
loc_1_0146:
        test   al, 2
        je     SHORT loc_1_014C
        movsw
loc_1_014C:
        test   al, 1
        je     SHORT loc_1_0154
        mov    bh, 0ffh
        mov    bl, byte ptr [esi]
loc_1_0154:
        pop    eax
        or     ax, 1000h
        out    dx, ax
        pop    edi
        mov    word ptr [edi + 34h], bx
        xor    eax, eax
        jmp    SHORT loc_1_01D7
loc_1_0164:
        xor    dl, 0ch
        or     ebx, ebx
        je     SHORT loc_1_0174
        mov    al, bl
        mov    ah, byte ptr [esi]
        inc    esi
        out    dx, ax
        xor    ebx, ebx
loc_1_0174:
        mov    eax, ecx
        shr    ecx, 2
        jecxz  loc_1_017E
        cld
        rep outsd
loc_1_017E:
        test   al, 2
        je     SHORT loc_1_0184
        outsw
loc_1_0184:
        test   al, 1
        je     SHORT loc_1_018C
        mov    bh, 0ffh
        mov    bl, byte ptr [esi]
loc_1_018C:
        mov    word ptr [edi + 34h], bx
        xor    eax, eax
        jmp    SHORT loc_1_01D7
loc_1_0194:
        xor    dl, 1ah
        out    dx, al
        xchg   al, ah
        xor    dl, 1
        out    dx, al
        xor    dl, 0bh
        mov    al, 12h
        out    dx, al
        xor    dl, 1ch
        or     ebx, ebx
        je     SHORT loc_1_01B4
        mov    al, bl
        mov    ah, byte ptr [esi]
        inc    esi
        out    dx, ax
        xor    ebx, ebx
loc_1_01B4:
        mov    eax, ecx
        shr    ecx, 2
        jecxz  loc_1_01BE
        cld
        rep outsd
loc_1_01BE:
        test   al, 2
        je     SHORT loc_1_01C4
        outsw
loc_1_01C4:
        test   al, 1
        je     SHORT loc_1_01CC
        mov    bh, 0ffh
        mov    bl, byte ptr [esi]
loc_1_01CC:
        mov    word ptr [edi + 34h], bx
        call   sub_1_068B
        jne    SHORT loc_1_01EC
loc_1_01D7:
        pop    ebx
        pop    ecx
        pop    edx
        pop    esi
        pop    edi
        leave
        ret
        db  08Bh, 0C0h
loc_1_01E0:
        mov    ah, 0ffh
        mov    al, byte ptr [esi]
        mov    word ptr [edi + 34h], ax
        xor    eax, eax
        jmp    SHORT loc_1_01D7
loc_1_01EC:
        mov    word ptr [edi + 34h], 0
        mov    word ptr [edi + 36h], 0
        jmp    SHORT loc_1_01D7
        db  08Bh, 0C0h
sub_1_01FC:
        push   ebp
        mov    ebp, esp
        push   edi
        push   edx
        push   ecx
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        xor    eax, eax
        xor    ebx, ebx
        movzx  ecx, word ptr [edi + 36h]
        or     ecx, ecx
        je     SHORT loc_1_028F
        xchg   word ptr [edi + 34h], bx
        or     ebx, ebx
        je     SHORT loc_1_021C
        inc    ecx
        inc    eax
loc_1_021C:
        cmp    ecx, 3ch
        jge    SHORT loc_1_022B
        sub    ecx, 3ch
        sub    eax, ecx
        mov    ecx, 3ch
loc_1_022B:
        or     eax, eax
        je     SHORT loc_1_028F
        inc    eax
        and    al, 0feh
        mov    word ptr [edi + 36h], cx
        movzx  ecx, ax
        movzx  edx, word ptr [edi + 2]
        test   byte ptr [edi + 8], 2
        je     SHORT loc_1_025D
        xor    dl, 0ch
        shr    ecx, 1
        or     ebx, ebx
        je     SHORT loc_1_0255
        movzx  eax, bl
        out    dx, ax
        loop   loc_1_0255
        jmp    SHORT loc_1_028F
loc_1_0255:
        xor    eax, eax
loc_1_0257:
        out    dx, ax
        loop   loc_1_0257
        jmp    SHORT loc_1_028F
loc_1_025D:
        xor    dl, 1ah
        movzx  eax, cx
        out    dx, al
        xchg   al, ah
        xor    dl, 1
        out    dx, al
        xor    dl, 0bh
        mov    al, 12h
        out    dx, al
        xor    dl, 1ch
        shr    ecx, 1
        or     ebx, ebx
        je     SHORT loc_1_0282
        movzx  eax, bl
        out    dx, ax
        loop   loc_1_0282
        jmp    SHORT loc_1_0288
loc_1_0282:
        xor    eax, eax
loc_1_0284:
        out    dx, ax
        loop   loc_1_0284
loc_1_0288:
        call   sub_1_068B
        jne    SHORT loc_1_0291
loc_1_028F:
        xor    eax, eax
loc_1_0291:
        pop    ebx
        pop    ecx
        pop    edx
        pop    edi
        leave
        ret
        db  090h
sub_1_0298:
        push   edx
        movzx  edx, word ptr [esp + 8]
        xor    dl, 10h
        mov    al, 62h
        out    dx, al
        xor    dl, 7
        in     al, dx
        xchg   ah, al
        xor    dl, 7
        mov    al, 22h
        out    dx, al
        movzx  eax, ah
        pop    edx
        ret
        db  02Eh, 08Bh, 0C0h
sub_1_02B8:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        push   edx
        push   ecx
        push   ebx
        mov    ecx, dword ptr [ebp + 10h]
        mov    edi, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        movzx  edx, word ptr [esi + 2]
        mov    eax, dword ptr [esi + 24h]
        xchg   al, ah
        mov    ebx, 0
        test   byte ptr [esi + 8], 2
        je     SHORT loc_1_02F7
        xor    dl, 0ah
        out    dx, ax
        xor    dl, 6
        in     eax, dx
        mov    dword ptr [esi + 2ch], eax
        shr    cx, 1
        rep insw
        jae    SHORT loc_1_033B
        in     ax, dx
        mov    byte ptr [edi], al
        jmp    SHORT loc_1_033B
loc_1_02F7:
        xor    dl, 18h
        out    dx, al
        xchg   ah, al
        xor    dl, 1
        out    dx, al
        xor    dl, 3
        mov    ax, cx
        add    ax, 4
        inc    ax
        and    al, 0feh
        out    dx, al
        xchg   al, ah
        xor    dl, 1
        out    dx, al
        xor    dl, 0bh
        mov    al, 0ah
        out    dx, al
        xor    dl, 1ch
        in     eax, dx
        mov    dword ptr [esi + 2ch], eax
        cld
        shr    cx, 1
        rep insw
        jae    SHORT loc_1_0330
        in     ax, dx
        mov    byte ptr [edi], al
loc_1_0330:
        call   sub_1_068B
        jne    NEAR PTR loc_1_04F2
loc_1_033B:
        movzx  eax, word ptr [esi + 2ch]
        test   al, 48h
        jne    NEAR PTR loc_1_0523
        test   al, 1
        je     NEAR PTR loc_1_0406
loc_1_034F:
        shr    ax, 8
        mov    ecx, dword ptr [esi + 24h]
        sub    ax, cx
        dec    ax
        jns    SHORT loc_1_036A
        mov    al, byte ptr [esi + 0fh]
        sub    al, cl
        add    al, byte ptr [esi + 2dh]
        sub    al, byte ptr [esi + 0eh]
        dec    al
loc_1_036A:
        shl    ax, 8
        mov    al, byte ptr [esi + 2eh]
        cmp    al, 0fch
        jbe    SHORT loc_1_0377
        dec    ah
loc_1_0377:
        sub    ax, 4
        mov    word ptr [esi + 2eh], ax
        or     ebx, ebx
        jne    SHORT loc_1_03B0
        cmp    ax, 5eah
        ja     NEAR PTR loc_1_044A
        cmp    ax, 3ch
        jb     NEAR PTR loc_1_044A
        mov    edi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [edi]
        movzx  edx, word ptr [edi + 4]
        mov    edi, dword ptr [esi + 10h]
        test   eax, 1
        jne    NEAR PTR loc_1_0469
        jmp    SHORT loc_1_03C0
loc_1_03B0:
        movzx  eax, word ptr [esi + 2eh]
loc_1_03B4:
        mov    edi, dword ptr [ebp + 14h]
        mov    dword ptr [edi], ebx
        pop    ebx
        pop    ecx
        pop    edx
        pop    esi
        pop    edi
        leave
        ret
loc_1_03C0:
        cmp    eax, dword ptr [edi]
        jne    SHORT loc_1_03EE
        cmp    dx, word ptr [edi + 4]
        jne    SHORT loc_1_03EE
        test   dword ptr [esi + 0c2h], 1
        jne    SHORT loc_1_03B0
        mov    ebx, 100h
        test   dword ptr [esi + 0c2h], 10h
        jne    SHORT loc_1_03B0
        mov    eax, 0
        jmp    SHORT loc_1_03B4
loc_1_03EE:
        mov    ebx, 104h
        test   dword ptr [esi + 0c2h], 10h
        jne    SHORT loc_1_03B0
        mov    eax, 0
        jmp    SHORT loc_1_03B4
loc_1_0406:
        mov    ebx, 206h
        and    eax, 6
        cmp    eax, 6
        je     SHORT loc_1_0430
        mov    ebx, 202h
        test   eax, 2
        jne    SHORT loc_1_0430
        mov    ebx, 204h
        test   eax, 4
        jne    SHORT loc_1_0430
        mov    ebx, 201h
loc_1_0430:
        test   dword ptr [esi + 0c2h], 20h
        jne    NEAR PTR loc_1_034F
        mov    eax, 0
        jmp    NEAR PTR loc_1_03B4
loc_1_044A:
        mov    ebx, 208h
        test   dword ptr [esi + 0c2h], 20h
        jne    NEAR PTR loc_1_03B0
        mov    eax, 0
        jmp    NEAR PTR loc_1_03B4
loc_1_0469:
        cmp    eax, -1
        jne    SHORT loc_1_04A8
        cmp    dx, -1
        jne    SHORT loc_1_04A8
        mov    ebx, 1
        test   dword ptr [esi + 0c2h], 2
        jne    NEAR PTR loc_1_03B0
        mov    ebx, 101h
        test   dword ptr [esi + 0c2h], 10h
        jne    NEAR PTR loc_1_03B0
        mov    eax, 0
        jmp    NEAR PTR loc_1_03B4
loc_1_04A8:
        mov    ecx, dword ptr [esi + 14h]
        jecxz  loc_1_04CF
loc_1_04AD:
        add    edi, 6
        cmp    eax, dword ptr [edi]
        jne    SHORT loc_1_04EE
        cmp    dx, word ptr [edi + 4]
        jne    SHORT loc_1_04EE
        mov    ebx, 2
        test   dword ptr [esi + 0c2h], 4
        jne    NEAR PTR loc_1_03B0
loc_1_04CF:
        mov    ebx, 102h
        test   dword ptr [esi + 0c2h], 18h
        jne    NEAR PTR loc_1_03B0
        mov    eax, 0
        jmp    NEAR PTR loc_1_03B4
loc_1_04EE:
        loop   loc_1_04AD
        jmp    SHORT loc_1_04CF
loc_1_04F2:
        inc    dword ptr [esi + 1e2h]
        and    dl, 0e0h
        push   edx
        call   sub_1_0298
        add    esp, 4
        movzx  eax, al
        mov    dword ptr [esi + 24h], eax
        dec    eax
        cmp    byte ptr [esi + 0eh], al
        jbe    SHORT loc_1_0515
        movzx  eax, byte ptr [esi + 0fh]
        dec    eax
loc_1_0515:
        xor    dl, 13h
        out    dx, al
        mov    eax, 0fffffffeh
        jmp    NEAR PTR loc_1_03B4
loc_1_0523:
        inc    dword ptr [esi + 1deh]
        and    dl, 0e0h
        xor    dl, 1ah
        mov    al, 0ffh
        out    dx, al
        xor    dl, 1
        mov    al, 0ffh
        out    dx, al
        mov    al, byte ptr [esi + 0eh]
        xor    dl, 8
        out    dx, al
        xor    dl, 3
        mov    al, 61h
        out    dx, al
        movzx  eax, byte ptr [esi + 0eh]
        inc    al
        mov    dword ptr [esi + 24h], eax
        xor    dl, 7
        out    dx, al
        xor    dl, 7
        mov    al, 22h
        out    dx, al
        xor    dl, 7
        mov    al, 5
        out    dx, al
        xor    dl, 7
        in     al, dx
        and    al, 0c2h
        mov    eax, 0fffffffdh
        cmp    al, 2
        je     NEAR PTR loc_1_03B4
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_03B4
        db  090h
sub_1_057C:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        push   edx
        push   ecx
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [esi + 24h]
        xchg   al, ah
        add    eax, ebx
        add    eax, 4
        and    al, 0feh
        mov    ecx, eax
        shr    eax, 8
        movzx  edx, word ptr [esi + 0c0h]
        cmp    eax, edx
        jl     SHORT loc_1_05B4
        shl    edx, 8
        sub    ecx, edx
        movzx  eax, byte ptr [esi + 0eh]
        shl    eax, 8
        add    ecx, eax
loc_1_05B4:
        mov    eax, ecx
        movzx  edx, word ptr [esi + 2]
        mov    edi, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 14h]
        test   byte ptr [esi + 8], 2
        je     SHORT loc_1_05CD
        xor    dl, 0ah
        out    dx, ax
        jmp    SHORT loc_1_0600
loc_1_05CD:
        xor    dl, 18h
        out    dx, al
        xchg   ah, al
        xor    dl, 1
        out    dx, al
        mov    eax, ecx
        test   bl, 1
        je     SHORT loc_1_05E9
        inc    ax
        test   cl, 1
        jne    SHORT loc_1_05F0
        inc    ax
        jmp    SHORT loc_1_05F0
loc_1_05E9:
        test   cl, 1
        je     SHORT loc_1_05F0
        inc    ax
loc_1_05F0:
        xor    dl, 3
        out    dx, al
        xchg   al, ah
        xor    dl, 1
        out    dx, al
        xor    dl, 0bh
        mov    al, 0ah
        out    dx, al
loc_1_0600:
        and    dl, 0e0h
        test   ebx, 1
        je     SHORT loc_1_0618
        dec    cx
        xor    dl, 0ch
        in     ax, dx
        mov    byte ptr [edi], ah
        inc    edi
        and    dl, 0e0h
loc_1_0618:
        push   ecx
        test   byte ptr [esi + 8], 2
        je     SHORT loc_1_0654
        test   byte ptr [esi + 8], 4
        je     SHORT loc_1_0646
        push   esi
        mov    esi, dword ptr [esi + 20h]
        xor    dl, 4
        in     ax, dx
        and    ax, 0efffh
        out    dx, ax
        cld
        shr    ecx, 2
        rep movsd
        or     ax, 1000h
        out    dx, ax
        pop    esi
        xor    dl, 8
        jmp    SHORT loc_1_065E
loc_1_0646:
        xor    dl, 0ch
        cld
        shr    ecx, 2
        mov    eax, eax
        rep insd
        jmp    SHORT loc_1_065E
loc_1_0654:
        xor    dl, 0ch
        cld
        shr    ecx, 2
        nop
        rep insd
loc_1_065E:
        pop    ecx
        test   cx, 2
        je     SHORT loc_1_0668
        insw
loc_1_0668:
        test   cx, 1
        je     SHORT loc_1_0673
        in     ax, dx
        mov    byte ptr [edi], al
loc_1_0673:
        test   byte ptr [esi + 8], 2
        jne    SHORT loc_1_0682
        push   edx
        call   sub_1_068B
        pop    edx
        jne    SHORT loc_1_0684
loc_1_0682:
        xor    eax, eax
loc_1_0684:
        pop    ebx
        pop    ecx
        pop    edx
        pop    esi
        pop    edi
        leave
        ret
sub_1_068B:
        push   ecx
        xor    dl, 1bh
        mov    ecx, 1ah
loc_1_0694:
        in     al, dx
        and    al, 40h
        loope  loc_1_0694
        je     SHORT loc_1_06A2
        mov    al, 40h
        out    dx, al
        pop    ecx
        xor    eax, eax
        ret
loc_1_06A2:
        push   edx
        xor    dl, 7
        mov    al, 22h
        out    dx, al
        pop    edx
        mov    al, 40h
        out    dx, al
        pop    ecx
        mov    eax, 0fffffffeh
        or     eax, eax
        ret
        db  000h, 000h
loc_1_06B8:
        db  0A0h, 018h, 00Dh, 000h, 000h
        cmp    al, 0
        jne    SHORT loc_1_06DD
        db  0FEh, 005h, 018h, 00Dh, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 028h, 000h
        jb     SHORT loc_1_06DF
        push   0
        push   0
        db  0E8h, 0D8h, 014h, 0FFh, 0FFh
        cmp    eax, 0
        jne    SHORT loc_1_06DF
loc_1_06DD:
        clc
        ret
loc_1_06DF:
        stc
        ret
HPISA_Control:
        sub    eax, 1
        cmp    eax, 1bh
        jae    SHORT loc_1_06F0
        db  0FFh, 024h, 085h, 0F2h, 006h, 000h, 000h
loc_1_06F0:
        clc
        ret
        db  0B8h, 006h, 000h, 000h, 0F0h, 006h, 000h, 000h, 0F0h, 006h, 000h, 000h, 0F0h, 006h, 000h, 000h
        db  0F0h, 006h, 000h, 000h, 0F0h, 006h, 000h, 000h, 0F0h, 006h, 000h, 000h, 0F0h, 006h, 000h, 000h
        db  0F0h, 006h, 000h, 000h, 0F0h, 006h, 000h, 000h, 0F0h, 006h, 000h, 000h, 0F0h, 006h, 000h, 000h
        db  0F0h, 006h, 000h, 000h, 0F0h, 006h, 000h, 000h, 0F0h, 006h, 000h, 000h, 0F0h, 006h, 000h, 000h
        db  0F0h, 006h, 000h, 000h, 0F0h, 006h, 000h, 000h, 0F0h, 006h, 000h, 000h, 0F0h, 006h, 000h, 000h
        db  0F0h, 006h, 000h, 000h, 0F0h, 006h, 000h, 000h, 0F0h, 006h, 000h, 000h, 0F0h, 006h, 000h, 000h
        db  0F0h, 006h, 000h, 000h, 0F0h, 006h, 000h, 000h, 0B8h, 006h, 000h, 000h, 000h, 000h
sub_1_0760:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 24h]
        push   eax
        mov    eax, dword ptr [ebp + 20h]
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0790
        pop    ebp
        ret    20h
        db  08Dh, 064h, 024h, 000h
sub_1_0790:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 18h], 0eh
        jb     NEAR PTR loc_1_0830
        mov    edx, dword ptr [ebp + 10h]
        mov    al, byte ptr [edx]
        test   al, 1
        je     SHORT loc_1_07D0
        cmp    dword ptr [edx + 2], -1
        jne    SHORT loc_1_07C5
        cmp    al, 0ffh
        jne    SHORT loc_1_07C5
        cmp    byte ptr [edx + 1], 0ffh
        jne    SHORT loc_1_07C5
        mov    dword ptr [ebp - 4], 8
        jmp    SHORT loc_1_0837
loc_1_07C5:
        mov    dword ptr [ebp - 4], 2
        jmp    SHORT loc_1_0837
        db  08Bh, 0FFh
loc_1_07D0:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0ch], 20h
        je     SHORT loc_1_0820
        mov    edx, dword ptr [ebp + 10h]
        lea    ecx, [esi + 2ch]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_07F6
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 10h], 0
        je     SHORT loc_1_07FD
loc_1_07F6:
        mov    dword ptr [ebp - 10h], 1
loc_1_07FD:
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_0810
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_083A
        db  08Dh, 064h, 024h, 000h
loc_1_0810:
        mov    dword ptr [ebp - 4], 20h
        jmp    SHORT loc_1_083A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0820:
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_083A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0830:
        mov    dword ptr [ebp - 4], 20h
loc_1_0837:
        mov    esi, dword ptr [ebp + 8]
loc_1_083A:
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     NEAR PTR loc_1_0931
loc_1_0845:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, ebx
        and    eax, dword ptr [ebp - 4]
        test   al, 9
        jne    SHORT loc_1_089D
        test   al, 2
        je     SHORT loc_1_0889
        lea    eax, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        db  0E8h, 056h, 0FFh, 0FEh, 0FFh
        or     al, al
        je     SHORT loc_1_0889
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 19h]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        test   dword ptr [eax + ecx*4], edx
        jne    SHORT loc_1_089D
loc_1_0889:
        test   byte ptr [ebp - 4], 2
        je     SHORT loc_1_0894
        test   bl, 4
        jne    SHORT loc_1_089D
loc_1_0894:
        test   bl, 20h
        je     NEAR PTR loc_1_0926
loc_1_089D:
        inc    dword ptr [edi + 0ch]
        mov    eax, dword ptr [ebp + 24h]
        push   eax
        mov    eax, dword ptr [ebp + 20h]
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [edi + 4]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        call   sub_1_0B12
        mov    byte ptr [edi + 18h], 1
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_0926
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_08DF
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_08DF:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_08F0
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_08F6
        db  08Bh, 0FFh
loc_1_08F0:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_08F6:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_0905
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_0B18
loc_1_0905:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 1ch]
        mov    eax, 1
        mov    cl, byte ptr [edi + 19h]
        shl    eax, cl
        or     dword ptr [esi + 28h], eax
        push   0
        push   1ah
        push   edi
        call   sub_1_0B8C
        mov    edi, ebx
        jmp    SHORT loc_1_0929
loc_1_0926:
        mov    edi, dword ptr [edi + 10h]
loc_1_0929:
        or     edi, edi
        jne    NEAR PTR loc_1_0845
loc_1_0931:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_0940:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0950
        pop    ebp
        ret    4
sub_1_0950:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     SHORT loc_1_09CE
loc_1_0960:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_09C7
        mov    byte ptr [edi + 18h], 0
        inc    dword ptr [edi + 0ch]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_0B18
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_09C7
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_098C
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_098C:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_09A0
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_09A6
        db  005h, 000h, 000h, 000h, 000h
loc_1_09A0:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_09A6:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 1ch]
        mov    eax, 1
        mov    cl, byte ptr [edi + 19h]
        shl    eax, cl
        or     dword ptr [esi + 28h], eax
        push   0
        push   1ah
        push   edi
        call   sub_1_0B8C
        mov    edi, ebx
        jmp    SHORT loc_1_09CA
loc_1_09C7:
        mov    edi, dword ptr [edi + 10h]
loc_1_09CA:
        or     edi, edi
        jne    SHORT loc_1_0960
loc_1_09CE:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_09E0:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0ch]
        test   al, 20h
        je     SHORT loc_1_09F5
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_09F5:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    bl, byte ptr [ecx]
        test   bl, 1
        je     SHORT loc_1_0A31
        cmp    dword ptr [ecx + 2], -1
        jne    SHORT loc_1_0A22
        cmp    bl, 0ffh
        jne    SHORT loc_1_0A22
        cmp    byte ptr [ecx + 1], 0ffh
        jne    SHORT loc_1_0A22
        test   al, 8
        je     SHORT loc_1_0A1B
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_0A1B:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
loc_1_0A22:
        test   al, 4
        jne    SHORT loc_1_0A2A
        test   al, 2
        je     SHORT loc_1_0A49
loc_1_0A2A:
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_0A31:
        mov    eax, dword ptr [edx + 2eh]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_0A49
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 2ch], ax
        jne    SHORT loc_1_0A49
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_0A49:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
        db  056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Eh
        db  05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h
        db  008h, 0FCh, 0F3h, 066h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh, 054h, 024h, 008h, 08Bh, 074h
        db  024h, 00Ch, 08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 057h, 08Bh, 04Ch
        db  024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Ch, 05Fh, 0C2h, 00Ch
        db  000h, 057h, 08Bh, 04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h
        db  066h, 06Dh, 05Fh, 0C2h, 00Ch, 000h, 057h, 08Bh, 054h, 024h, 008h, 08Bh, 07Ch, 024h, 00Ch, 08Bh
        db  04Ch, 024h, 010h, 0FCh, 0F3h, 06Dh, 05Fh, 0C2h, 00Ch, 000h
sub_1_0ACA:
        db  0CDh, 020h, 031h, 080h, 028h, 000h
sub_1_0AD0:
        db  0CDh, 020h, 032h, 080h, 028h, 000h
sub_1_0AD6:
        db  0CDh, 020h, 033h, 080h, 028h, 000h
sub_1_0ADC:
        db  0CDh, 020h, 034h, 080h, 028h, 000h
sub_1_0AE2:
        db  0CDh, 020h, 035h, 080h, 028h, 000h
sub_1_0AE8:
        db  0CDh, 020h, 036h, 080h, 028h, 000h
sub_1_0AEE:
        db  0CDh, 020h, 037h, 080h, 028h, 000h
sub_1_0AF4:
        db  0CDh, 020h, 038h, 080h, 028h, 000h
sub_1_0AFA:
        db  0CDh, 020h, 039h, 080h, 028h, 000h
        db  0CDh, 020h, 03Ah, 080h, 028h, 000h
sub_1_0B06:
        db  0CDh, 020h, 03Bh, 080h, 028h, 000h
        db  0CDh, 020h, 03Ch, 080h, 028h, 000h
sub_1_0B12:
        db  0CDh, 020h, 03Dh, 080h, 028h, 000h
sub_1_0B18:
        db  0CDh, 020h, 03Eh, 080h, 028h, 000h
sub_1_0B1E:
        db  0CDh, 020h, 03Fh, 080h, 028h, 000h
sub_1_0B24:
        db  0CDh, 020h, 040h, 080h, 028h, 000h
        db  0CDh, 020h, 041h, 080h, 028h, 000h
        db  0CDh, 020h, 009h, 080h, 028h, 000h
        db  0CDh, 020h, 008h, 080h, 028h, 000h
        db  0CDh, 020h, 044h, 080h, 028h, 000h
        db  0CDh, 020h, 048h, 080h, 028h, 000h
        db  0CDh, 020h, 049h, 080h, 028h, 000h
        db  0CDh, 020h, 04Fh, 080h, 028h, 000h
        db  0CDh, 020h, 056h, 080h, 028h, 000h
        add    byte ptr [eax], al
        db  0CDh, 020h, 000h, 080h, 028h, 000h
sub_1_0B62:
        db  0CDh, 020h, 00Ch, 080h, 028h, 000h
        db  0CDh, 020h, 00Dh, 080h, 028h, 000h
sub_1_0B6E:
        db  0CDh, 020h, 00Fh, 080h, 028h, 000h
sub_1_0B74:
        db  0CDh, 020h, 010h, 080h, 028h, 000h
sub_1_0B7A:
        db  0CDh, 020h, 011h, 080h, 028h, 000h
sub_1_0B80:
        db  0CDh, 020h, 00Eh, 080h, 028h, 000h
sub_1_0B86:
        db  0CDh, 020h, 00Ah, 080h, 028h, 000h
sub_1_0B8C:
        db  0CDh, 020h, 00Bh, 080h, 028h, 000h
        db  0CDh, 020h, 016h, 080h, 028h, 000h
        db  0CDh, 020h, 017h, 080h, 028h, 000h
        db  0CDh, 020h, 018h, 080h, 028h, 000h
        db  0CDh, 020h, 019h, 080h, 028h, 000h
        db  0CDh, 020h, 01Ah, 080h, 028h, 000h
sub_1_0BB0:
        db  0CDh, 020h, 01Bh, 080h, 028h, 000h
sub_1_0BB6:
        db  0CDh, 020h, 01Ch, 080h, 028h, 000h
sub_1_0BBC:
        db  0CDh, 020h, 01Dh, 080h, 028h, 000h
sub_1_0BC2:
        db  0CDh, 020h, 01Eh, 080h, 028h, 000h
        db  0CDh, 020h, 01Fh, 080h, 028h, 000h
sub_1_0BCE:
        db  0CDh, 020h, 020h, 080h, 028h, 000h
sub_1_0BD4:
        db  0CDh, 020h, 021h, 080h, 028h, 000h
        db  0CDh, 020h, 022h, 080h, 028h, 000h
        db  0CDh, 020h, 023h, 080h, 028h, 000h
        db  0CDh, 020h, 024h, 080h, 028h, 000h
        db  0CDh, 020h, 025h, 080h, 028h, 000h
        db  0CDh, 020h, 026h, 080h, 028h, 000h
sub_1_0BF8:
        db  0CDh, 020h, 027h, 080h, 028h, 000h
sub_1_0BFE:
        db  0CDh, 020h, 028h, 080h, 028h, 000h
sub_1_0C04:
        db  0CDh, 020h, 005h, 080h, 028h, 000h
sub_1_0C0A:
        db  0CDh, 020h, 006h, 080h, 028h, 000h
sub_1_0C10:
        db  0CDh, 020h, 007h, 080h, 028h, 000h
        db  0CDh, 020h, 042h, 080h, 028h, 000h
        db  0CDh, 020h, 012h, 080h, 028h, 000h
        db  0CDh, 020h, 013h, 080h, 028h, 000h
        db  0CDh, 020h, 014h, 080h, 028h, 000h
        db  0CDh, 020h, 015h, 080h, 028h, 000h
sub_1_0C34:
        db  0CDh, 020h, 04Ah, 080h, 028h, 000h
sub_1_0C3A:
        db  0CDh, 020h, 04Bh, 080h, 028h, 000h
sub_1_0C40:
        db  0CDh, 020h, 04Ch, 080h, 028h, 000h
loc_1_0C46:
        db  0CDh, 020h, 04Dh, 080h, 028h, 000h
sub_1_0C4C:
        db  0CDh, 020h, 04Eh, 080h, 028h, 000h
sub_1_0C52:
        db  0CDh, 020h, 046h, 080h, 028h, 000h
sub_1_0C58:
        db  0CDh, 020h, 045h, 080h, 028h, 000h
        db  0CDh, 020h, 043h, 080h, 028h, 000h
        db  0CDh, 020h, 047h, 080h, 028h, 000h
        add    byte ptr [eax], al
sub_1_0C6C:
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 14h]
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 0ch]
        rep movsd
        mov    cl, al
        rep movsb
        pop    edi
        pop    esi
        ret    0ch
sub_1_0C8C:
        push   esi
        push   edi
        pushfd
        mov    ecx, dword ptr [esp + 18h]
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 10h]
        cmp    esi, edi
        jae    SHORT loc_1_0CAC
        or     ecx, ecx
        je     SHORT loc_1_0CC2
        std
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    SHORT loc_1_0CC0
loc_1_0CAC:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     SHORT loc_1_0CC0
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd
        mov    cl, al
loc_1_0CC0:
        rep movsb
loc_1_0CC2:
        popfd
        pop    edi
        pop    esi
        ret    0ch
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 00Ah, 003h, 000h, 000h, 003h, 00Ah, 000h, 000h, 048h, 050h, 049h, 053h
        db  041h, 020h, 020h, 020h, 000h, 040h, 000h, 054h, 0E1h, 006h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 04Dh, 061h, 078h, 04Dh, 075h, 06Ch, 074h, 069h
        db  063h, 061h, 073h, 074h, 000h, 000h, 000h, 000h
loc_1_0D30:
        dec    ebp
        popad
        db  078h, 04Dh
        jne    SHORT loc_1_0DA2
        db  074h, 069h
        arpl   word ptr [ecx + 73h], sp
        je     SHORT loc_1_0D3D
loc_1_0D3D:
        add    byte ptr [eax], al
        add    byte ptr [ecx + 6fh], ch
        popad
        db  064h, 064h, 072h, 065h
        db  073h, 073h
        add    byte ptr [eax], al
        add    byte ptr [ecx + 6fh], ch
        popad
        db  064h, 064h, 072h, 065h
        jae    SHORT loc_1_0DC8
        add    byte ptr [eax], al
        add    byte ptr [ecx + 6eh], ch
        je     SHORT loc_1_0DC1
        db  072h, 072h
        db  075h, 070h
        je     SHORT loc_1_0D62
loc_1_0D62:
        add    byte ptr [eax], al
loc_1_0D64:
        imul   ebp, dword ptr [esi + 74h], 75727265h
        jo     SHORT loc_1_0DE1
        add    byte ptr [eax], al
        add    byte ptr [edx + 75h], al
        db  073h, 04Eh
        jne    SHORT loc_1_0DE3
        bound  esp, qword ptr [ebp + 72h]
        add    byte ptr [eax], al
        db  000h
loc_1_0D7C:
        inc    edx
        jne    SHORT loc_1_0DF2
        dec    esi
        jne    SHORT loc_1_0DEF
        bound  esp, qword ptr [ebp + 72h]
        add    byte ptr [eax], al
        add    byte ptr [ebp + 65h], cl
        insd
        outsd
        jb     SHORT loc_1_0E07
        dec    ebp
        popad
        db  070h, 070h
        inc    edx
        popad
        jae    SHORT loc_1_0DFD
        inc    ecx
        db  064h, 064h, 072h, 065h
        jae    SHORT loc_1_0E12
        add    byte ptr [ebp + 65h], cl
loc_1_0DA2:
        insd
        outsd
        jb     SHORT loc_1_0E1F
        dec    ebp
        popad
        db  070h, 070h
        inc    edx
        popad
        db  073h, 065h
        inc    ecx
        db  064h, 064h, 072h, 065h
        jae    SHORT loc_1_0E2A
        add    byte ptr [ecx], al
        add    dword ptr [ecx], eax
        add    byte ptr [edx], al
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], al
loc_1_0DC1:
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], al
        add    dword ptr [eax], eax
loc_1_0DC8:
        add    eax, 6000101h
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], al
        add    dword ptr [ecx], eax
        add    byte ptr [eax], cl
loc_1_0DE1:
        add    dword ptr [ecx], eax
loc_1_0DE3:
        add    byte ptr [ecx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], cl
        add    dword ptr [ecx], eax
loc_1_0DEF:
        add    byte ptr [ecx + eax], cl
loc_1_0DF2:
        add    dword ptr [eax], eax
        or     eax, 10000101h
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
loc_1_0DFD:
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [edx], eax
loc_1_0E07:
        add    byte ptr [edx], al
        add    dword ptr [edx], eax
        add    byte ptr [ebx], al
        add    dword ptr [edx], eax
        add    byte ptr [ecx + eax], al
loc_1_0E12:
        add    al, byte ptr [eax]
        add    eax, 1000201h
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
loc_1_0E1F:
        add    dword ptr [ebx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx + eax], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
loc_1_0E2A:
        add    al, byte ptr [ecx]
        add    al, byte ptr [ecx]
        add    al, byte ptr [ecx]
        add    eax, dword ptr [ecx]
        add    al, byte ptr [ecx]
loc_1_0E34:
        add    dword ptr [ecx], eax
        add    dword ptr [eax], eax
        add    al, byte ptr [ecx]
        add    dword ptr [eax], eax
        add    eax, dword ptr [ecx]
        add    dword ptr [eax], eax
        add    al, 1
        add    dword ptr [eax], eax
        add    eax, 6000101h
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], al
        add    dword ptr [ecx], eax
        add    byte ptr [eax], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], cl
        add    dword ptr [eax], eax
        or     eax, 10000101h
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ebx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx + eax], eax
        add    dword ptr [ecx], eax
loc_1_0E90:
        dec    eax
        push   eax
        and    byte ptr [eax + 43h], dl
        and    byte ptr [ecx + eax*2 + 4eh], cl
        and    byte ptr [ecx + 64h], al
        popad
        jo     SHORT loc_1_0F13
        db  065h, 072h, 02Eh
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 048h, 050h, 049h, 053h, 041h, 000h, 000h, 000h
loc_1_0EB0:
        dec    eax
        push   eax
        dec    ecx
        push   ebx
        inc    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax + 50h], cl
        das
        cmp    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 50h], cl
        das
        xor    dword ptr [esi], esi
        and    byte ptr [eax + edx*2], dl
        add    byte ptr [eax], al
        add    byte ptr [eax + 50h], cl
        das
        xor    dword ptr [esi], esi
        and    byte ptr [eax + 6ch], dl
        db  075h, 073h
        add    byte ptr [eax], al
loc_1_0ED8:
        dec    eax
        push   eax
        das
        xor    dword ptr [esi], esi
        and    byte ptr [eax + 6ch], dl
        jne    SHORT loc_1_0F55
        and    byte ptr [ebp + 65h], ch
        insd
        outsd
        db  072h, 079h
        cmp    eax, 3f3f7830h
        aas
        xor    byte ptr [eax], dh
        xor    byte ptr [eax], al
        add    byte ptr [esi + 4fh], cl
        push   esp
        dec    ecx
        inc    ebx
        inc    ebp
        cmp    ah, byte ptr [eax]
        inc    ecx
        popad
        jo     SHORT loc_1_0F75
        jb     NEAR PTR loc_1_0F24
        outsb
        outsd
        je     SHORT loc_1_0F28
        jo     SHORT loc_1_0F7C
        db  065h, 073h, 065h
        outsb
        je     SHORT loc_1_0F10
loc_1_0F10:
        inc    ebp
        push   edx
        push   edx
loc_1_0F13:
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        dec    ecx
        outsb
        jbe    SHORT loc_1_0F7C
        insb
        imul   esp, dword ptr [eax + 49h], 61204f2fh
loc_1_0F24:
        db  064h, 064h, 072h, 065h
loc_1_0F28:
        db  073h, 073h
        sub    byte ptr [ebp + 73h], ah
        sub    dword ptr [eax], eax
        add    byte ptr [ebp + 52h], al
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        dec    ecx
        outsb
        jbe    SHORT loc_1_0F9C
        insb
        imul   esp, dword ptr [eax + 69h], 7265746eh
        jb     SHORT loc_1_0FBB
        db  070h, 074h
        and    byte ptr [esi + 65h], dh
        arpl   word ptr [edi + ebp*2 + 72h], si
        add    byte ptr [ebp + 52h], al
        push   edx
        dec    edi
        push   edx
loc_1_0F55:
        cmp    ah, byte ptr [eax]
        dec    ecx
        outsb
        db  074h, 065h
        jb     SHORT loc_1_0FCF
        jne    SHORT loc_1_0FCF
        db  074h, 020h
        db  076h, 065h
        arpl   word ptr [edi + ebp*2 + 72h], si
        and    byte ptr [ebp + 69h], ch
        jae    SHORT loc_1_0FD9
        popad
        db  074h, 063h
        push   0
loc_1_0F74:
        inc    ebp
loc_1_0F75:
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        inc    ecx
loc_1_0F7C:
        popad
        db  070h, 074h
        db  065h, 072h, 020h
        outsb
        outsd
        db  074h, 020h
        db  073h, 075h
        jo     SHORT loc_1_0FFB
        outsd
        jb     SHORT loc_1_1002
        add    byte ptr fs:[eax], al
        add    byte ptr [eax], al
loc_1_0F94:
        inc    ebp
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        push   ebx
loc_1_0F9C:
        je     SHORT loc_1_100D
        jb     SHORT loc_1_1005
        and    byte ptr fs:[ebx + 6fh], ah
        outsb
        imul   sp, word ptr [edi + 75h], 6172h
        db  074h, 069h
        outsd
        outsb
        and    byte ptr [ebx + 6fh], ah
        jb     SHORT loc_1_1026
        jne    SHORT loc_1_1026
        db  074h, 065h
        add    byte ptr fs:[eax], al
loc_1_0FBB:
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
loc_1_0FCF:
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
loc_1_0FD9:
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h
loc_1_0FE4:
        xor    byte ptr [ecx], dh
        xor    dh, byte ptr [ebx]
        xor    al, 35h
        aaa
        cmp    byte ptr [ecx], bh
        popad
        bound  esp, qword ptr [ebx + 64h]
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
        push   ebp
        db  073h, 069h
loc_1_0FFB:
        outsb
        and    byte ptr [di + 78h], ah
        db  074h, 065h
loc_1_1002:
        db  072h, 06Eh
        popad
loc_1_1005:
        insb
        and    byte ptr [edx + esi*2 + 61h], dh
        outsb
        db  073h, 063h
loc_1_100D:
        imul   esi, dword ptr gs:[esi + 65h], 55000072h
        db  073h, 069h
        outsb
        and    byte ptr [bx + di + 6eh], ch
        db  074h, 065h
        db  072h, 06Eh
        popad
        insb
        and    byte ptr [edx + esi*2 + 61h], dh
loc_1_1026:
        outsb
        db  073h, 063h
        db  065h, 069h, 076h, 065h, 072h, 000h, 000h
loc_1_1030:
        push   ebp
        outsb
        popad
        bound  ebp, qword ptr [ebp + 20h]
        je     SHORT loc_1_10A8
        and    byte ptr [eax + 65h], dh
        db  072h, 066h
        outsd
        db  072h, 06Dh
        and    byte ptr [ecx + eax*2 + 4eh], cl
        and    byte ptr [eax + 6fh], dh
        db  072h, 074h
        and    byte ptr [ecx + 75h], ah
        db  074h, 06Fh
        sub    eax, 736e6573h
        add    byte ptr gs:[eax], al
        add    byte ptr [ecx + 75h], al
        db  074h, 06Fh
        sub    eax, 736e6573h
        and    byte ptr gs:[ecx + eax*2 + 4eh], cl
        and    byte ptr [eax + 6fh], dh
        db  072h, 074h
        and    byte ptr [esi + 61h], ah
        imul   ebp, dword ptr [ebp + 64h], 6556202eh
        jb     SHORT loc_1_10E1
        db  066h, 079h, 020h
        arpl   word ptr [ecx + 62h], sp
        insb
        imul   ebp, dword ptr [esi + 67h], 654d002eh
        insd
        outsd
        db  072h, 079h
        and    byte ptr [ebp + 61h], ch
        jo     SHORT loc_1_10AF
        outsb
        outsd
        db  074h, 020h
        arpl   word ptr [edi + 6eh], bp
        imul   sp, word ptr [edi + 75h], 6572h
        cmp    esp, dword ptr fs:[eax]
        jne    SHORT loc_1_1114
        imul   ebp, dword ptr [esi + 67h], 4f2f4920h
loc_1_10A8:
        and    byte ptr [edx + esi*2 + 61h], dh
        outsb
        db  073h, 066h
loc_1_10AF:
        db  065h, 072h, 073h
        add    byte ptr [eax], al
loc_1_10B4:
        dec    ebp
        insd
        outsd
        jb     SHORT loc_1_1133
        and    byte ptr [ebp + 61h], ch
        db  070h, 020h
        popad
        jb     NEAR PTR loc_1_1129
        jae    SHORT loc_1_1139
        and    byte ptr [edi + ebp*2 + 65h], ah
        jae    SHORT loc_1_10EC
        outsb
        outsd
        db  074h, 020h
        insd
        popad
        je     SHORT loc_1_1137
        push   61646120h
        jo     SHORT loc_1_114F
        db  065h, 072h, 03Bh
        and    byte ptr [ebp + 73h], dh
loc_1_10E1:
        imul   ebp, dword ptr [esi + 67h], 4f2f4920h
        and    byte ptr [edx + esi*2 + 61h], dh
loc_1_10EC:
        outsb
        db  073h, 066h
        jb     NEAR PTR loc_1_1165
        add    byte ptr [eax], al
loc_1_10F4:
        dec    ebp
        insd
        outsd
        db  072h, 079h
        and    byte ptr [ebp + 61h], ch
        db  070h, 020h
        jb     SHORT loc_1_1162
        outsb
        and    byte ptr gs:[di + 75h], ch
        db  073h, 074h
        and    byte ptr [edx + 65h], ah
        and    byte ptr [edx], dh
        xor    byte ptr [eax + edi], dh
        and    byte ptr [edx + 79h], ah
loc_1_1114:
        je     SHORT loc_1_117B
        db  073h, 03Bh
        and    byte ptr [ebp + 73h], dh
        imul   ebp, dword ptr [esi + 67h], 4f2f4920h
        and    byte ptr [edx + esi*2 + 61h], dh
        outsb
        db  073h, 066h
loc_1_1129:
        db  065h, 072h, 073h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1130:
        inc    ebx
        popad
        outsb
loc_1_1133:
        outsb
        outsd
        db  074h, 020h
loc_1_1137:
        insd
        popad
loc_1_1139:
        db  070h, 020h
        insd
        insd
        outsd
        jb     SHORT loc_1_11BA
        and    byte ptr [ecx + 6eh], ch
        db  074h, 06Fh
        and    byte ptr [ebx + 65h], ch
        db  072h, 06Eh
        insb
        cmp    esp, dword ptr [eax]
loc_1_114F:
        jne    SHORT loc_1_11C4
        imul   ebp, dword ptr [esi + 67h], 4f2f4920h
        and    byte ptr [edx + esi*2 + 61h], dh
        outsb
        jae    SHORT loc_1_11C5
        jb     NEAR PTR loc_1_11D5
loc_1_1162:
        add    byte ptr [eax], al
loc_1_1164:
        dec    ebp
loc_1_1165:
        insd
        outsd
        jb     SHORT loc_1_11E3
        and    byte ptr [ebp + 61h], ch
        db  070h, 020h
        arpl   word ptr [edi + 6eh], bp
        insb
        imul   esp, dword ptr [ebx + 74h], 7375203bh
loc_1_117B:
        imul   ebp, dword ptr [esi + 67h], 4f2f4920h
        and    byte ptr [edx + esi*2 + 61h], dh
        outsb
        jae    SHORT loc_1_11EF
        jb     NEAR PTR loc_1_11FF
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1190:
        or     byte ptr [eax], al
        or     dword ptr [eax], eax
loc_1_1194:
        inc    esi
        inc    ecx
        push   esp
        inc    ecx
        dec    esp
        cmp    ah, byte ptr [eax]
        inc    edx
        popad
        and    byte ptr fs:[eax + 2fh], cl
        push   edi
        and    byte ptr [ecx + 6eh], ch
        imul   esi, dword ptr [eax + 63h], 65646fh
        add    byte ptr [eax], al
        db  000h
loc_1_11B0:
        inc    esi
        popad
        imul   ebp, dword ptr [ebp + 64h], 206f7420h
loc_1_11BA:
        jb     SHORT loc_1_1221
        jae    SHORT loc_1_1232
        popad
        db  072h, 074h
        add    byte ptr cs:[eax], al
loc_1_11C4:
        inc    esi
loc_1_11C5:
        popad
        imul   ebp, dword ptr [ebp + 64h], 206f7420h
        db  072h, 065h
        db  073h, 074h
        popad
        db  072h, 074h
loc_1_11D5:
        add    byte ptr cs:[eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_11E3:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_11ED:
        add    byte ptr [eax], al
loc_1_11EF:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_11FF:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1221:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_1232:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end
