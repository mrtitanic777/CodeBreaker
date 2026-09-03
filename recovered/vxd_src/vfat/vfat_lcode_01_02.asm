; vfat_lcode_01_02.asm -- VFAT object 1 segment _LTEXT  (part 2/3)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_5574:
        mov    eax, dword ptr [ebp + 1ch]
        call   sub_1_05FA
        mov    edi, dword ptr [ebp + 1ch]
        mov    byte ptr [edi + 0bch], 0
        or     word ptr [ebp + 38h], 4000h
        mov    ebx, 2
loc_1_5591:
        push   ebx
        call   sub_1_1F2C
        mov    edx, ebx
        pop    ebx
        jb     SHORT loc_1_55C9
        push   ebx
        call   sub_1_1D4C
        pop    ebx
        jb     SHORT loc_1_55C9
        movzx  eax, word ptr [edi + 20h]
        shr    eax, 2
        add    ebx, eax
        neg    eax
        and    ebx, eax
        cmp    dword ptr [edi + 2ch], ebx
        jae    SHORT loc_1_5591
        mov    eax, edi
        call   sub_1_05FA
        mov    edi, dword ptr [ebp + 1ch]
        cmp    byte ptr [edi + 0bch], 10h
        cmc
loc_1_55C9:
        pushfd
        and    word ptr [ebp + 38h], 0bfffh
        popfd
        ret
sub_1_55D2:
        mov    edi, dword ptr [esi + 0ch]
        test   byte ptr [edi + 52h], 3
        jne    SHORT loc_1_5606
        cmp    dword ptr [edi + 0a4h], -1
        je     SHORT loc_1_5606
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        push   ebx
        push   esi
        mov    esi, ebx
        mov    ebx, dword ptr [edi + 0a4h]
        mov    ecx, 1
        call   sub_1_1574
        pop    esi
        pop    ebx
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
loc_1_5606:
        mov    dl, 1
        jmp    NEAR PTR sub_1_03F3
sub_1_560D:
        cld
        sub    eax, eax
        mov    byte ptr [ebp + 37h], al
        mov    word ptr [ebp + 38h], 1400h
        mov    byte ptr [ebp + 34h], al
        mov    byte ptr [ebp + 35h], al
        call   sub_1_A318
        mov    edi, dword ptr [ebp + 1ch]
        movzx  ecx, byte ptr [edi + 4ch]
        movzx  edx, word ptr [edi + 22h]
        ror    edx, cl
        mov    eax, dword ptr [ebp]
        test   edx, eax
        jne    SHORT loc_1_5695
        or     eax, eax
        je     SHORT loc_1_5694
        mov    edx, dword ptr [ebp + 24h]
        add    edx, eax
        jb     SHORT loc_1_5695
        cmp    dword ptr [edi + 8], edx
        jb     SHORT loc_1_5695
        shl    eax, cl
        mov    dword ptr [ebp], eax
        clc
        call   sub_1_9DFC
        call   sub_1_5E7C
        movzx  ecx, byte ptr [edi + 4ch]
        mov    eax, dword ptr [ebp]
        shr    eax, cl
        mov    ecx, eax
        xor    eax, eax
        mov    edx, eax
        mov    dword ptr [ebp + 6ch], eax
        mov    ebx, dword ptr [ebp + 24h]
        call   sub_1_70F6
        pushfd
        push   eax
        jb     SHORT loc_1_5689
        mov    eax, dword ptr [ebp]
        pushfd
        db  001h, 005h, 060h, 0B7h, 000h, 000h
        popfd
        pushfd
        db  0FFh, 005h, 068h, 0B7h, 000h, 000h
        popfd
loc_1_5689:
        call   sub_1_9EB2
        pop    eax
        popfd
        sbb    edx, edx
        and    eax, edx
loc_1_5694:
        ret
loc_1_5695:
        mov    ax, 1bh
        ret
sub_1_569A:
        cld
        sub    eax, eax
        mov    byte ptr [ebp + 37h], al
        mov    word ptr [ebp + 38h], 1400h
        mov    byte ptr [ebp + 34h], al
        mov    byte ptr [ebp + 35h], al
        call   sub_1_A26B
        push   0
        push   0
        call   sub_1_5CFF
        mov    byte ptr [ebp + 3ah], 8
        movzx  eax, word ptr [ebp + 18h]
        test   al, 1
        je     SHORT loc_1_56D4
        and    ah, 0e0h
        cmp    ah, 40h
        jne    SHORT loc_1_56D4
        or     word ptr [ebp + 38h], 800h
loc_1_56D4:
        mov    edi, dword ptr [ebp + 1ch]
        mov    ecx, dword ptr [edi + 14h]
        mov    eax, 1dh
        test   dword ptr [ecx + 4], 40h
        jne    NEAR PTR loc_1_57AF
        movzx  ecx, byte ptr [edi + 4ch]
        movzx  edx, word ptr [edi + 22h]
        ror    edx, cl
        mov    eax, dword ptr [ebp]
        or     eax, eax
        je     NEAR PTR loc_1_57A4
        test   edx, eax
        jne    NEAR PTR loc_1_57A9
        mov    edx, dword ptr [ebp + 24h]
        add    edx, eax
        jb     NEAR PTR loc_1_57A9
        cmp    dword ptr [edi + 8], edx
        jb     NEAR PTR loc_1_57A9
        shl    eax, cl
        mov    dword ptr [ebp], eax
        stc
        call   sub_1_9DFC
        movzx  ecx, byte ptr [edi + 4ch]
        mov    eax, dword ptr [ebp]
        shr    eax, cl
        mov    ecx, eax
        mov    ebx, dword ptr [ebp + 24h]
        mov    edx, dword ptr [edi + 30h]
        movzx  eax, byte ptr [edi + 46h]
        imul   edx, eax
        movzx  eax, word ptr [edi + 26h]
        add    edx, eax
        cmp    ebx, edx
        jae    SHORT loc_1_5764
        inc    dword ptr [esp]
        or     dword ptr [edi + 40h], 0ffffffffh
        push   ebx
        push   ecx
        push   edi
        call   sub_1_5BCE
        mov    eax, dword ptr [ebp + 1ch]
        call   sub_1_05FA
        pop    edi
        pop    ecx
        pop    ebx
loc_1_5764:
        call   sub_1_57B5
        mov    eax, 1dh
        sbb    edx, edx
        and    eax, edx
        mov    dword ptr [esp + 4], eax
        or     eax, eax
        jne    SHORT loc_1_578D
        mov    eax, dword ptr [ebp]
        pushfd
        db  001h, 005h, 064h, 0B7h, 000h, 000h
        popfd
        pushfd
        db  0FFh, 005h, 06Ch, 0B7h, 000h, 000h
        popfd
loc_1_578D:
        call   sub_1_9EB2
        cmp    dword ptr [esp], 0
        je     SHORT loc_1_57A0
        mov    eax, dword ptr [ebp + 1ch]
        call   sub_1_05FA
loc_1_57A0:
        mov    eax, dword ptr [esp + 4]
loc_1_57A4:
        lea    esp, [esp + 8]
        ret
loc_1_57A9:
        mov    ax, 1bh
        jmp    SHORT loc_1_57A4
loc_1_57AF:
        mov    byte ptr [ebp + 34h], 1fh
        jmp    SHORT loc_1_57A4
sub_1_57B5:
        test   byte ptr [ebp + 3ah], 2
        jne    SHORT loc_1_5833
        push   ecx
        push   ebx
        sub    ebx, dword ptr [edi + 0ch]
        mov    al, byte ptr [edi + 4eh]
        not    al
        and    bl, al
        add    ebx, dword ptr [edi + 0ch]
        call   sub_1_0050
        mov    eax, ebx
        pop    ebx
        jae    SHORT loc_1_5801
        sub    eax, ebx
        add    al, byte ptr [edi + 4dh]
        movzx  eax, al
loc_1_57DC:
        cmp    eax, ecx
        jbe    SHORT loc_1_57EE
        test   byte ptr [ebp + 37h], 1
        jne    SHORT loc_1_57EC
        cmp    dword ptr [ebp + 48h], 0
        je     SHORT loc_1_5801
loc_1_57EC:
        mov    eax, ecx
loc_1_57EE:
        add    ebx, eax
        sub    ecx, eax
        je     SHORT loc_1_5801
        call   sub_1_0050
        jae    SHORT loc_1_5801
        movzx  eax, byte ptr [edi + 4dh]
        jmp    SHORT loc_1_57DC
loc_1_5801:
        pop    esi
        sub    esi, ecx
        je     SHORT loc_1_5833
        push   esi
        push   ebx
        push   ecx
        sub    ebx, esi
        mov    ecx, esi
        mov    esi, dword ptr [ebp + 14h]
        call   sub_1_1574
        pop    ecx
        pop    ebx
        pop    esi
        jb     NEAR PTR loc_1_58B4
        mov    edi, dword ptr [ebp + 1ch]
        xchg   ecx, eax
        mov    cl, byte ptr [edi + 4ch]
        shl    esi, cl
        add    dword ptr [ebp + 14h], esi
        xchg   ecx, eax
        or     ecx, ecx
        je     NEAR PTR loc_1_58B4
loc_1_5833:
        push   ebx
        push   ecx
        call   sub_1_02F8
        mov    edx, ebx
        pop    ecx
        pop    ebx
        jb     SHORT loc_1_58B4
        mov    ax, word ptr [edi + 4dh]
        sub    ebx, dword ptr [edi + 0ch]
        and    ah, bl
        add    ebx, dword ptr [edi + 0ch]
        sub    al, ah
        movzx  eax, al
        cmp    eax, ecx
        jbe    SHORT loc_1_5857
        mov    eax, ecx
loc_1_5857:
        add    ebx, eax
        sub    ecx, eax
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        push   esi
        push   eax
        push   edx
        push   ecx
        mov    cl, byte ptr [edi + 4ch]
        shl    eax, cl
        mov    ecx, eax
        mov    edi, edx
        mov    esi, dword ptr [ebp + 14h]
        call   sub_1_823A
        mov    dword ptr [ebp + 14h], esi
        pop    eax
        pop    edx
        pop    ecx
        pop    esi
        mov    edi, dword ptr [ebp + 1ch]
        jb     SHORT loc_1_58A4
        push   eax
        push   ebx
        sub    ebx, ecx
        sub    ebx, dword ptr [esi + 8]
        mov    al, 1
        shl    al, cl
        dec    al
        xchg   ecx, ebx
        shl    al, cl
        xchg   ecx, ebx
        not    al
        and    byte ptr [esi + 24h], al
        and    byte ptr [esi + 25h], al
        call   sub_1_5C8F
        pop    ebx
        pop    ecx
loc_1_58A4:
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        jb     SHORT loc_1_58B4
        or     ecx, ecx
        jne    NEAR PTR sub_1_57B5
loc_1_58B4:
        ret
        db  08Dh, 064h, 024h, 008h, 0C3h
sub_1_58BA:
        mov    edi, dword ptr [edx + 60h]
        or     edi, edi
        jne    SHORT loc_1_58ED
        mov    al, byte ptr [edx + 0ach]
        db  08Bh, 03Dh, 054h, 0B7h, 000h, 000h
        mov    ecx, edi
loc_1_58CF:
        cmp    byte ptr [edi], al
        je     SHORT loc_1_58EA
        mov    edi, dword ptr [edi + 19h]
        cmp    di, -1
        stc
        je     SHORT loc_1_58ED
        movzx  ecx, di
        shr    edi, 10h
        shl    edi, 4
        add    edi, ecx
        jmp    SHORT loc_1_58CF
loc_1_58EA:
        mov    dword ptr [edx + 60h], edi
loc_1_58ED:
        ret
sub_1_58EE:
        mov    edi, edx
        mov    edx, dword ptr [edi + 60h]
        mov    al, byte ptr [edi + 0ach]
        mov    byte ptr [esi], al
        mov    byte ptr [esi + 1], 0ffh
        mov    byte ptr [esi + 18h], 0
        or     edx, edx
        je     SHORT loc_1_5915
        mov    al, byte ptr [edx + 1]
        mov    byte ptr [esi + 1], al
        mov    al, byte ptr [edx + 18h]
        and    al, 80h
        mov    byte ptr [esi + 18h], al
loc_1_5915:
        movzx  eax, word ptr [edi + 20h]
        mov    word ptr [esi + 2], ax
        mov    al, byte ptr [edi + 24h]
        mov    byte ptr [esi + 4], al
        mov    al, byte ptr [edi + 25h]
        mov    byte ptr [esi + 5], al
        movzx  eax, word ptr [edi + 26h]
        mov    word ptr [esi + 6], ax
        mov    al, byte ptr [edi + 46h]
        mov    byte ptr [esi + 8], al
        movzx  eax, word ptr [edi + 44h]
        mov    word ptr [esi + 9], ax
        mov    al, byte ptr [edi + 0bah]
        mov    byte ptr [esi + 17h], al
        mov    eax, dword ptr [edi + 28h]
        mov    word ptr [esi + 0bh], ax
        mov    dword ptr [esi + 29h], eax
        mov    eax, dword ptr [edi + 30h]
        mov    word ptr [esi + 0fh], ax
        mov    dword ptr [esi + 31h], eax
        mov    eax, dword ptr [edi + 34h]
        mov    word ptr [esi + 11h], ax
        mov    eax, dword ptr [edi + 38h]
        mov    word ptr [esi + 1dh], ax
        mov    dword ptr [esi + 39h], eax
        push   esi
        call   sub_1_5BEF
        pop    esi
        jb     SHORT loc_1_59F1
        mov    dword ptr [esi + 1fh], edx
        inc    ebx
        mov    word ptr [esi + 0dh], bx
        mov    dword ptr [esi + 2dh], ebx
        xor    eax, eax
        mov    word ptr [esi + 23h], ax
        mov    word ptr [esi + 25h], ax
        mov    word ptr [esi + 27h], ax
        mov    dword ptr [esi + 35h], eax
        test   byte ptr [edi + 52h], 3
        jne    SHORT loc_1_59EA
        mov    word ptr [esi + 0bh], ax
        mov    word ptr [esi + 0dh], ax
        mov    word ptr [esi + 0fh], ax
        mov    word ptr [esi + 11h], ax
        mov    word ptr [esi + 1dh], ax
        mov    word ptr [esi + 1fh], ax
        movzx  edx, byte ptr [edi + 48h]
        mov    word ptr [esi + 23h], dx
        test   byte ptr [edi + 52h], 10h
        je     SHORT loc_1_59C4
        or     word ptr [esi + 23h], 100h
loc_1_59C4:
        test   byte ptr [edi + 52h], 4
        je     SHORT loc_1_59D0
        or     word ptr [esi + 23h], 80h
loc_1_59D0:
        mov    edx, dword ptr [edi + 0a0h]
        mov    word ptr [esi + 25h], dx
        mov    edx, dword ptr [edi + 0a4h]
        mov    word ptr [esi + 27h], dx
        mov    edx, dword ptr [edi + 3ch]
        mov    dword ptr [esi + 35h], edx
loc_1_59EA:
        dec    eax
        mov    dword ptr [esi + 13h], eax
        mov    dword ptr [esi + 19h], eax
loc_1_59F1:
        ret
loc_1_59F2:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        cld
        sub    eax, eax
        mov    byte ptr [ebp + 37h], al
        mov    word ptr [ebp + 38h], ax
        mov    byte ptr [ebp + 34h], al
        mov    byte ptr [ebp + 35h], al
        call   sub_1_A318
        mov    edx, dword ptr [ebp + 1ch]
        test   byte ptr [ebp + 18h], 1
        jne    NEAR PTR loc_1_5AA1
        test   byte ptr [edx + 52h], 3
        je     SHORT loc_1_5A98
        call   sub_1_58BA
        jb     SHORT loc_1_5A98
        mov    ebx, edi
        movzx  eax, word ptr [edx + 20h]
        mov    word ptr [edi + 2], ax
        mov    al, byte ptr [edx + 24h]
        mov    byte ptr [edi + 4], al
        mov    al, byte ptr [edx + 25h]
        mov    byte ptr [edi + 5], al
        movzx  eax, word ptr [edx + 26h]
        mov    word ptr [edi + 6], ax
        mov    al, byte ptr [edx + 46h]
        mov    byte ptr [edi + 8], al
        movzx  eax, word ptr [edx + 44h]
        mov    word ptr [edi + 9], ax
        mov    eax, dword ptr [edx + 28h]
        mov    word ptr [edi + 0bh], ax
        mov    eax, dword ptr [edx + 2ch]
        mov    word ptr [edi + 0dh], ax
        mov    eax, dword ptr [edx + 30h]
        mov    word ptr [edi + 0fh], ax
        mov    eax, dword ptr [edx + 34h]
        mov    word ptr [edi + 11h], ax
        mov    al, byte ptr [edx + 0bah]
        mov    byte ptr [edi + 17h], al
        mov    edi, edx
        mov    eax, ebx
        call   sub_1_5BEF
        jb     SHORT loc_1_5A98
        mov    word ptr [eax + 1fh], dx
        shl    eax, 0ch
        shr    ax, 0ch
        mov    dword ptr [ebp + 10h], eax
        jmp    NEAR PTR loc_1_4574
loc_1_5A98:
        mov    ax, 0ffffh
        jmp    NEAR PTR loc_1_4576
loc_1_5AA1:
        call   sub_1_58BA
        mov    eax, dword ptr [ebp]
        cmp    eax, 3fh
        jb     SHORT loc_1_5ACB
        mov    esi, dword ptr [ebp + 14h]
        mov    word ptr [esi], 3dh
        add    esi, 2
        call   sub_1_58EE
        jae    NEAR PTR loc_1_4574
        mov    al, 5
        jmp    NEAR PTR loc_1_4576
loc_1_5ACB:
        mov    al, 18h
        jmp    NEAR PTR loc_1_4576
loc_1_5AD2:
        pop    ds
loc_1_5AD3:
        or     al, 1
        sbb    ecx, dword ptr [eax]
        add    al, 1eh
        or     edx, dword ptr [eax]
        push   ss
        or     al, 1
        adc    al, 1
        add    dl, byte ptr [0fa30202h]
        add    dl, byte ptr [ebx]
        add    byte ptr [ebx], al
sub_1_5AEA:
        db  0BFh, 0D2h, 05Ah, 000h, 000h
        mov    ecx, 8
loc_1_5AF4:
        scasb
        je     SHORT loc_1_5B01
        add    edi, 2
        loop   loc_1_5AF4
        db  0BFh, 0D3h, 05Ah, 000h, 000h
loc_1_5B01:
        mov    ax, word ptr [edi]
        ret
sub_1_5B05:
        push   ebx
        push   ecx
        push   edx
        push   esi
        mov    ecx, dword ptr [edi + 14h]
        movzx  ebx, word ptr [ecx + 0ah]
        mov    esi, dword ptr [ecx + 0ch]
        mov    ah, 2
        call   sub_1_A4E1
        mov    esi, ebx
        mov    dword ptr [esi + 44h], ecx
        mov    al, byte ptr [edi + 0ach]
        mov    byte ptr [esi + 4dh], al
        mov    dword ptr [esi + 8], 500h
        mov    word ptr [esi + 4], 29h
        mov    word ptr [esi + 2eh], 0db02h
        movzx  eax, byte ptr [edi + 24h]
        inc    eax
        mov    dword ptr [esi + 34h], eax
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [esi + 38h], eax
        db  0CDh, 020h, 004h, 000h, 010h, 000h
        cmp    word ptr [esi + 6], 10h
        cmc
        pushfd
        sbb    ecx, ecx
        mov    eax, dword ptr [esi + 3ch]
        or     eax, ecx
        push   eax
        call   sub_1_A4AE
        pop    eax
        mov    edx, dword ptr [edi + 6ch]
        movzx  ecx, byte ptr [edi + 4dh]
        bsf    ecx, ecx
        add    cl, byte ptr [edi + 4ch]
        shl    edx, cl
        mov    cl, byte ptr [edi + 47h]
        shr    edx, cl
        sub    eax, edx
        cmc
        sbb    edx, edx
        and    eax, edx
        popfd
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        ret
sub_1_5B85:
        mov    eax, 1
        mov    edi, dword ptr [ebp + 1ch]
sub_1_5B8D:
        test   word ptr [edi + 52h], 10h
        je     SHORT loc_1_5BCD
        push   ebx
        push   edx
        push   esi
        push   eax
        mov    ecx, 0db03h
        mov    ebx, edi
        call   sub_1_449E
        cmp    eax, 10h
        sbb    eax, eax
        and    eax, edx
        pop    ecx
        sub    eax, dword ptr [edi + 6ch]
        jb     SHORT loc_1_5BBB
        sub    eax, dword ptr [edi + 68h]
        jb     SHORT loc_1_5BBB
        sub    eax, ecx
        jae    SHORT loc_1_5BCA
loc_1_5BBB:
        push   edi
        mov    eax, edi
        call   sub_1_05FA
        pop    edi
        call   sub_1_5CB0
        stc
loc_1_5BCA:
        pop    esi
        pop    edx
        pop    ebx
loc_1_5BCD:
        ret
sub_1_5BCE:
        or     eax, 0ffffffffh
        jmp    SHORT loc_1_5BD5
sub_1_5BD3:
        xor    eax, eax
loc_1_5BD5:
        mov    esi, dword ptr [edi + 9ch]
        or     esi, esi
        je     SHORT loc_1_5BEE
        mov    ecx, dword ptr [edi + 2ch]
        shr    ecx, 0ah
        inc    ecx
        push   edi
        cld
        mov    edi, esi
        rep stosw
        pop    edi
loc_1_5BEE:
        ret
sub_1_5BEF:
        push   eax
loc_1_5BF0:
        mov    ebx, dword ptr [edi + 2ch]
        dec    ebx
        mov    edx, dword ptr [edi + 40h]
        cmp    edx, -1
        je     SHORT loc_1_5C04
        cmp    ebx, edx
        jae    SHORT loc_1_5C3E
        or     dword ptr [edi + 40h], 0ffffffffh
loc_1_5C04:
        cmp    dword ptr [edi + 9ch], 0
        jne    SHORT loc_1_5C40
        mov    esi, dword ptr [edi + 0a8h]
        xor    edx, edx
        mov    ecx, ebx
        jecxz  loc_1_5C39
        lea    ebx, [edx + 1]
loc_1_5C1C:
        inc    ebx
        push   ebx
        push   ecx
        push   edx
        push   esi
        call   sub_1_1F2C
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        jb     SHORT loc_1_5C3E
        jne    SHORT loc_1_5C2F
        inc    edx
loc_1_5C2F:
        loop   loc_1_5C1C
        cmp    dword ptr [edi + 0a8h], esi
        jne    SHORT loc_1_5BF0
loc_1_5C39:
        mov    dword ptr [edi + 40h], edx
        dec    ebx
        clc
loc_1_5C3E:
        pop    eax
        ret
loc_1_5C40:
        xor    ebx, ebx
loc_1_5C42:
        call   sub_1_7788
        jb     SHORT loc_1_5C4F
        add    ebx, 400h
loc_1_5C4F:
        cmp    dword ptr [edi + 2ch], ebx
        jae    SHORT loc_1_5C42
        mov    esi, dword ptr [edi + 9ch]
        xor    edx, edx
loc_1_5C5C:
        movsx  eax, word ptr [esi]
        add    esi, 2
        cmp    eax, -2
        je     SHORT loc_1_5C73
        cmp    eax, 401h
        cmc
        jb     SHORT loc_1_5C3E
        add    edx, eax
        jmp    SHORT loc_1_5C5C
loc_1_5C73:
        mov    ebx, dword ptr [edi + 2ch]
        jmp    SHORT loc_1_5C39
        db  033h, 0D2h, 066h, 0F7h, 045h, 038h, 004h, 000h, 074h, 008h, 042h, 0F6h, 045h, 03Ah, 008h, 074h
        db  001h, 042h, 0E9h, 064h, 0A7h, 0FFh, 0FFh
sub_1_5C8F:
        push   edx
        xor    edx, edx
        test   word ptr [ebp + 38h], 4
        je     SHORT loc_1_5CA9
        mov    dl, 2
        test   byte ptr [ebp + 3ah], 8
        jne    SHORT loc_1_5CA9
        dec    edx
        cmp    byte ptr [edi + 4dh], cl
        ja     SHORT loc_1_5CA9
        dec    edx
loc_1_5CA9:
        call   sub_1_043A
        pop    edx
        ret
sub_1_5CB0:
        cmp    byte ptr [ebp + 35h], 0
        jne    SHORT loc_1_5CDF
        or     word ptr [ebp + 38h], 4
loc_1_5CBB:
        mov    eax, dword ptr [ebp + 1ch]
        movzx  edx, byte ptr [eax + 1eh]
loc_1_5CC2:
        bts    dword ptr [eax + 1ch], edx
        jb     SHORT loc_1_5CE0
        mov    byte ptr [edx + eax + 0bch], dh
        mov    byte ptr [ebp + 35h], dl
        inc    edx
        and    dl, 0fh
        cmp    dl, 1
        adc    dl, 0
        mov    byte ptr [eax + 1eh], dl
loc_1_5CDF:
        ret
loc_1_5CE0:
        cmp    word ptr [eax + 1ch], -2
        jae    SHORT loc_1_5CF3
        inc    edx
        and    dl, 0fh
        cmp    dl, 1
        adc    dl, 0
        jmp    SHORT loc_1_5CC2
loc_1_5CF3:
        db  0B8h, 0F4h, 0B6h, 000h, 000h
        call   sub_1_A220
        jmp    SHORT loc_1_5CBB
sub_1_5CFF:
        mov    eax, dword ptr [ebp + 1ch]
        test   word ptr [eax + 52h], 0a400h
        jne    SHORT loc_1_5D16
        mov    eax, dword ptr [eax + 14h]
        test   dword ptr [eax + 4], 8000h
        je     SHORT loc_1_5D1B
loc_1_5D16:
        call   sub_1_5CB0
loc_1_5D1B:
        ret
loc_1_5D1C:
        pushad
        mov    ebx, dword ptr [esp + 24h]
        mov    ecx, dword ptr [esp + 28h]
        push   ebp
        sub    esp, 74h
        mov    ebp, esp
        cld
        sub    eax, eax
        mov    byte ptr [ebp + 37h], al
        mov    word ptr [ebp + 38h], ax
        mov    byte ptr [ebp + 34h], al
        mov    byte ptr [ebp + 35h], al
        mov    eax, ebx
        mov    edx, ecx
        db  083h, 03Dh, 0B8h, 0B6h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_5E67
        db  0C7h, 005h, 0B8h, 0B6h, 000h, 000h, 001h, 000h, 000h, 000h
        db  08Bh, 035h, 058h, 0B7h, 000h, 000h
        cmp    esi, 0
        je     NEAR PTR loc_1_5E51
        mov    dword ptr [ebp + 20h], esi
        mov    dword ptr [ebp + 14h], eax
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ebp + 24h], edx
        mov    edi, dword ptr [eax + 28h]
        cmp    dword ptr [edi + 68h], 8
        jae    NEAR PTR loc_1_5E51
        mov    dword ptr [ebp], 1000h
        mov    dword ptr [ebp + 1ch], edi
        mov    eax, dword ptr [edi + 14h]
        test   dword ptr [eax + 4], 40h
        jne    NEAR PTR loc_1_5E51
        mov    dword ptr [ebp + 3ch], edx
        mov    ebx, dword ptr [esi + 8]
        add    edx, 1000h
        cmp    edx, dword ptr [ebx + 30h]
        ja     NEAR PTR loc_1_5E51
        push   esi
        call   sub_1_774E
        pop    esi
        jb     NEAR PTR loc_1_5E51
        push   esi
        call   sub_1_7704
        pop    esi
        db  0A1h, 0F0h, 0B6h, 000h, 000h
        lea    edx, [eax + 10h]
        mov    dword ptr [eax + 0ch], edx
loc_1_5DCC:
        push   edx
        mov    ecx, dword ptr [ebp + 48h]
        call   sub_1_76B7
        pop    edx
        jb     SHORT loc_1_5E51
        mov    dword ptr [edx], ebx
        mov    dword ptr [edx + 4], ecx
        add    edx, 8
        sub    dword ptr [ebp + 48h], ecx
        jne    SHORT loc_1_5DCC
        db  0A1h, 0F0h, 0B6h, 000h, 000h
        mov    dword ptr [edx], 0
        mov    ebx, dword ptr [eax + 10h]
        mov    edx, eax
        mov    eax, dword ptr [ebp + 3ch]
        mov    dword ptr [edx], eax
        push   edi
        push   esi
        push   edx
        db  08Ah, 025h, 0F8h, 0B6h, 000h, 000h
        mov    al, 13h
        db  0CDh, 020h, 004h, 000h, 08Bh, 004h
        pop    edx
        cmp    esi, 0
        je     SHORT loc_1_5E4F
        mov    ebx, esi
        or     byte ptr [esi + 2bh], 2
        mov    dword ptr [edx + 4], esi
        mov    edi, dword ptr [esi + 10h]
        mov    dword ptr [edx + 8], edi
        mov    esi, dword ptr [ebp + 14h]
        mov    ecx, 1000h
        call   sub_1_823A
        pop    esi
        pop    edi
        pushad
        db  08Bh, 035h, 0F0h, 0B6h, 000h, 000h
        call   sub_1_8C70
        popad
        jb     SHORT loc_1_5E5B
        add    dword ptr [ebp + 3ch], 1000h
        clc
        add    esp, 74h
        pop    ebp
        popad
        xor    eax, eax
        clc
        ret
loc_1_5E4F:
        pop    esi
        pop    edi
loc_1_5E51:
        db  0C7h, 005h, 0B8h, 0B6h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_5E5B:
        add    esp, 74h
        pop    ebp
        popad
        mov    eax, 1
        stc
        ret
loc_1_5E67:
        db  083h, 03Dh, 0B8h, 0B6h, 000h, 000h, 002h
        jb     SHORT loc_1_5E5B
        add    esp, 74h
        pop    ebp
        popad
        mov    eax, 2
        stc
        ret
sub_1_5E7C:
        cmp    byte ptr [edi + 46h], 1
        jbe    SHORT loc_1_5EF2
        mov    eax, dword ptr [ebp + 24h]
        movzx  ecx, byte ptr [edi + 4ch]
        mov    edx, dword ptr [ebp]
        shr    edx, cl
        lea    ecx, [eax + edx]
        mov    ebx, dword ptr [edi + 18h]
        mov    edx, dword ptr [edi + 30h]
        add    edx, ebx
        cmp    eax, edx
        jae    SHORT loc_1_5EF2
        cmp    ecx, ebx
        jbe    SHORT loc_1_5EF2
        cmp    ebx, eax
        jae    SHORT loc_1_5EA7
        mov    ebx, eax
loc_1_5EA7:
        cmp    edx, ecx
        jbe    SHORT loc_1_5EAD
        mov    edx, ecx
loc_1_5EAD:
        push   ebx
        movzx  eax, byte ptr [edi + 4eh]
        not    eax
        sub    ebx, dword ptr [edi + 0ch]
        and    ebx, eax
        add    ebx, dword ptr [edi + 0ch]
        call   sub_1_0050
        jb     SHORT loc_1_5EF3
        pop    ebx
loc_1_5EC4:
        mov    eax, ebx
        sub    eax, dword ptr [esi + 8]
        sub    ecx, ecx
        bts    ecx, eax
        or     byte ptr [esi + 24h], cl
        inc    ebx
        cmp    ebx, edx
        jae    SHORT loc_1_5EF2
        movzx  eax, byte ptr [edi + 4dh]
        add    eax, dword ptr [esi + 8]
        cmp    ebx, eax
        jb     SHORT loc_1_5EC4
loc_1_5EE1:
        call   sub_1_0050
        jae    SHORT loc_1_5EC4
loc_1_5EE8:
        movzx  eax, byte ptr [edi + 4dh]
        add    ebx, eax
        cmp    ebx, edx
        jb     SHORT loc_1_5EE1
loc_1_5EF2:
        ret
loc_1_5EF3:
        pop    eax
        jmp    SHORT loc_1_5EE8
        db  000h, 000h, 040h, 000h, 08Ah, 05Fh, 000h, 000h, 041h, 000h, 07Bh, 061h, 000h, 000h, 042h, 000h
        db  08Ah, 05Fh, 000h, 000h, 046h, 000h, 059h, 060h, 000h, 000h, 047h, 000h, 08Ah, 05Fh, 000h, 000h
        db  048h, 001h, 08Ah, 05Fh, 000h, 000h, 049h, 001h, 08Ah, 05Fh, 000h, 000h, 060h, 001h, 08Ah, 05Fh
        db  000h, 000h, 061h, 000h, 07Bh, 061h, 000h, 000h, 062h, 000h, 08Ah, 05Fh, 000h, 000h, 066h, 001h
        db  009h, 061h, 000h, 000h, 067h, 001h, 08Ah, 05Fh, 000h, 000h, 068h, 001h, 08Ah, 05Fh, 000h, 000h
        db  0FFh, 001h, 08Ah, 05Fh, 000h, 000h
loc_1_5F4C:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        cld
        sub    eax, eax
        mov    byte ptr [ebp + 37h], al
        mov    word ptr [ebp + 38h], ax
        mov    byte ptr [ebp + 34h], al
        mov    byte ptr [ebp + 35h], al
        mov    edi, dword ptr [ebp + 1ch]
        mov    ecx, dword ptr [ebp + 28h]
        movzx  eax, byte ptr [ecx + 1ch]
        movzx  edx, word ptr [ecx + 18h]
        cmp    al, 12h
        je     NEAR PTR loc_1_619C
        cmp    al, 0dh
        je     NEAR PTR loc_1_6016
        cmp    al, 11h
        je     NEAR PTR loc_1_6043
loc_1_5F8A:
        call   sub_1_A3B9
        mov    word ptr [ebx + 4], 0dh
        mov    dword ptr [ebx + 8], 510h
        mov    esi, ebx
        mov    dword ptr [esi + 38h], ecx
        mov    ax, word ptr [ecx + 10h]
        mov    word ptr [esi + 2ch], ax
        dec    al
        mov    byte ptr [esi + 4dh], al
        movzx  eax, word ptr [ecx + 18h]
        mov    dword ptr [esi + 30h], eax
        test   byte ptr [ebp + 18h], 1
        je     SHORT loc_1_5FC7
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [esi + 34h], eax
        and    byte ptr [esi + 8], 0efh
        jmp    SHORT loc_1_5FD4
loc_1_5FC7:
        mov    ax, 3c14h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        mov    dword ptr [esi + 34h], eax
loc_1_5FD4:
        mov    ax, word ptr [ecx + 1ch]
        mov    word ptr [esi + 2eh], ax
        xor    eax, eax
        mov    dword ptr [esi + 44h], eax
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        cld
        mov    dword ptr [edi + 78h], eax
        mov    eax, esi
        push   ecx
        push   eax
        call   sub_1_199E
        pop    edi
        pop    ebx
        mov    eax, dword ptr [edi + 3ch]
        mov    word ptr [ebx + 1ch], ax
        cmp    word ptr [edi + 6], 10h
        jae    SHORT loc_1_6006
        xor    eax, eax
loc_1_6006:
        mov    word ptr [ebp + 1ah], ax
        mov    ebx, edi
        call   sub_1_A4AE
loc_1_6011:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_6016:
        db  0BBh, 0F2h, 05Eh, 000h, 000h
loc_1_601B:
        add    ebx, 6
        cmp    byte ptr [ebx], dl
        jb     SHORT loc_1_601B
        ja     NEAR PTR loc_1_5F8A
        cmp    byte ptr [ebx + 1], 0
        jne    SHORT loc_1_6040
        mov    eax, dword ptr [ebp + 1ch]
        test   byte ptr [eax + 52h], 3
        jne    SHORT loc_1_6040
        mov    ax, 5
        cmp    dh, 48h
        jne    SHORT loc_1_6053
loc_1_6040:
        jmp    dword ptr [ebx + 2]
loc_1_6043:
        cmp    dl, 66h
        je     SHORT loc_1_6051
        cmp    dl, 46h
        jne    NEAR PTR loc_1_5F8A
loc_1_6051:
        xor    eax, eax
loc_1_6053:
        mov    word ptr [ebp + 1ah], ax
        jmp    SHORT loc_1_6011
loc_1_6059:
        call   sub_1_A318
        xor    ebx, ebx
        call   sub_1_00D0
        mov    al, 1eh
        mov    byte ptr [ebp + 34h], 0
        jb     NEAR PTR loc_1_4576
        mov    al, 1ah
        cmp    byte ptr [ebx + 15h], 0f0h
        jb     NEAR PTR loc_1_4576
        xor    edx, edx
        cmp    word ptr [ebx + 16h], dx
        jne    SHORT loc_1_6096
        cmp    byte ptr [ebx + 42h], 29h
        jne    NEAR PTR loc_1_4576
        mov    edx, 1ch
        jmp    SHORT loc_1_60A0
loc_1_6096:
        cmp    byte ptr [ebx + 26h], 29h
        jne    NEAR PTR loc_1_4576
loc_1_60A0:
        mov    ax, 3c14h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        push   esi
        lea    edi, [edx + ebx + 27h]
        lea    esi, [eax + 2]
        mov    ecx, 17h
        rep movsb
        pop    esi
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        push   eax
        call   sub_1_55D2
        pop    ebx
        mov    al, 1dh
        mov    byte ptr [ebp + 34h], 0
        jb     NEAR PTR loc_1_4576
        lea    esi, [ebx + 2]
        mov    eax, dword ptr [ebp + 1ch]
        lea    edi, [eax + 54h]
        movsd
        lea    edi, [eax + 0aeh]
        mov    ecx, 0bh
        rep movsb
        lea    edi, [eax + 0aeh]
        call   sub_1_6EC3
        mov    edi, dword ptr [ebp + 1ch]
        mov    byte ptr [edi + 0b9h], al
        jmp    NEAR PTR loc_1_4574
loc_1_6109:
        call   sub_1_A318
        xor    ebx, ebx
        call   sub_1_00D0
        mov    al, 1eh
        mov    byte ptr [ebp + 34h], 0
        jb     NEAR PTR loc_1_4576
        mov    al, 1ah
        cmp    byte ptr [ebx + 15h], 0f0h
        jb     NEAR PTR loc_1_4576
        xor    edx, edx
        cmp    word ptr [ebx + 16h], dx
        jne    SHORT loc_1_6146
        cmp    byte ptr [ebx + 42h], 29h
        jne    NEAR PTR loc_1_4576
        mov    edx, 1ch
        jmp    SHORT loc_1_6150
loc_1_6146:
        cmp    byte ptr [ebx + 26h], 29h
        jne    NEAR PTR loc_1_4576
loc_1_6150:
        mov    ax, 3c14h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        push   esi
        lea    edi, [eax + 2]
        lea    esi, [edx + ebx + 27h]
        mov    ecx, 17h
        rep movsb
        pop    esi
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        jmp    NEAR PTR loc_1_4574
loc_1_617B:
        push   ecx
        push   edi
        push   eax
        mov    eax, edi
        call   sub_1_05FA
        pop    eax
        pop    edi
        pop    ecx
        or     eax, eax
        je     NEAR PTR loc_1_5F8A
        mov    ebx, edi
        call   sub_1_A72F
        jmp    NEAR PTR loc_1_5F8A
loc_1_619C:
        mov    esi, dword ptr [ebp + 14h]
        cmp    dword ptr [esi], 1
        ja     NEAR PTR loc_1_622F
        call   sub_1_A318
        cmp    dword ptr [esi], 1
        jne    SHORT loc_1_61B7
        mov    esi, dword ptr [esi + 8]
        jmp    SHORT loc_1_61B9
loc_1_61B7:
        xor    esi, esi
loc_1_61B9:
        mov    ebx, dword ptr [ebp + 20h]
        mov    ebx, dword ptr [ebx + 8]
        mov    ebx, dword ptr [ebx + 10h]
        mov    edi, dword ptr [ebp + 1ch]
        xor    ecx, ecx
        or     ebx, ebx
        je     SHORT loc_1_6224
        cmp    dword ptr [edi + 2ch], ebx
        jb     SHORT loc_1_622F
        or     esi, esi
        je     SHORT loc_1_61FB
loc_1_61D4:
        mov    eax, ecx
        mov    edx, ebx
        mov    cl, byte ptr [edi + 25h]
        sub    edx, 2
        jb     SHORT loc_1_622F
        shl    edx, cl
        add    edx, dword ptr [edi + 28h]
        mov    ecx, eax
        mov    dword ptr [esi + 4], edx
        mov    dword ptr [esi], 0
loc_1_61F0:
        or     esi, esi
        je     SHORT loc_1_61FB
        movzx  eax, byte ptr [edi + 24h]
        inc    eax
        add    dword ptr [esi], eax
loc_1_61FB:
        push   ebx
        push   ecx
        push   esi
        call   sub_1_1F2C
        pop    esi
        pop    ecx
        pop    eax
        jb     SHORT loc_1_622F
        call   sub_1_2102
        jae    SHORT loc_1_6223
        cmp    dword ptr [edi + 2ch], ebx
        jb     SHORT loc_1_622F
        inc    eax
        cmp    eax, ebx
        je     SHORT loc_1_61F0
        inc    ecx
        or     esi, esi
        je     SHORT loc_1_61FB
        add    esi, 8
        jmp    SHORT loc_1_61D4
loc_1_6223:
        inc    ecx
loc_1_6224:
        mov    esi, dword ptr [ebp + 14h]
        mov    dword ptr [esi + 4], ecx
        jmp    NEAR PTR loc_1_4574
loc_1_622F:
        mov    eax, 5
        jmp    NEAR PTR loc_1_4576
        db  000h, 000h, 000h, 08Ah, 062h, 000h, 000h, 083h, 062h, 000h, 000h, 0D0h, 062h, 000h, 000h, 0D7h
        db  062h, 000h, 000h, 0EFh, 062h, 000h, 000h, 00Bh, 063h, 000h, 000h, 025h, 063h, 000h, 000h
loc_1_6258:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        cld
        sub    eax, eax
        mov    byte ptr [ebp + 37h], al
        mov    word ptr [ebp + 38h], ax
        mov    byte ptr [ebp + 34h], al
        mov    byte ptr [ebp + 35h], al
        movzx  esi, byte ptr [ebp + 4]
        cmp    esi, 7
        jae    SHORT loc_1_6283
        db  0FFh, 024h, 0B5h, 03Ch, 062h, 000h, 000h
loc_1_6283:
        mov    eax, 1
        jmp    SHORT loc_1_62C7
loc_1_628A:
        call   sub_1_A26B
        db  00Fh, 0BAh, 02Dh, 086h, 0B7h, 000h, 000h, 000h
        call   sub_1_6338
        db  0C7h, 045h, 010h, 08Ch, 022h, 000h, 000h
        push   eax
        call   sub_1_A2CE
        db  00Fh, 0BAh, 035h, 086h, 0B7h, 000h, 000h, 000h
        pop    eax
        or     eax, eax
        jne    SHORT loc_1_62C7
        mov    eax, 1
        db  023h, 005h, 070h, 0B8h, 000h, 000h
        mov    word ptr [ebp + 6], ax
        sub    eax, eax
loc_1_62C7:
        mov    word ptr [ebp + 1ah], ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_62D0:
        call   sub_1_6593
        jmp    SHORT loc_1_62C7
loc_1_62D7:
        mov    edi, dword ptr [ebp + 28h]
        mov    eax, edi
        call   sub_1_05FA
        mov    ebx, dword ptr [ebp + 28h]
        call   sub_1_A72F
        mov    edi, ebx
        sub    eax, eax
        jmp    SHORT loc_1_62C7
loc_1_62EF:
        call   sub_1_66AD
        or     eax, eax
        jne    SHORT loc_1_62C7
        mov    eax, 1
        db  023h, 005h, 070h, 0B8h, 000h, 000h
        mov    word ptr [ebp + 6], ax
        sub    eax, eax
        jmp    SHORT loc_1_62C7
loc_1_630B:
        mov    edx, dword ptr [ebp + 1ch]
        movzx  eax, word ptr [ebp + 18h]
        mov    byte ptr [edx + 0ach], al
        mov    eax, dword ptr [ebp + 24h]
        mov    byte ptr [edx + 0adh], al
        xor    eax, eax
        jmp    SHORT loc_1_62C7
loc_1_6325:
        mov    edx, dword ptr [ebp + 1ch]
        mov    al, byte ptr [edx + 0adh]
        mov    byte ptr [edx + 0ach], al
        xor    eax, eax
        jmp    SHORT loc_1_62C7
sub_1_6338:
        mov    ebx, dword ptr [ebp + 10h]
        cmp    dword ptr [ebx + 10h], 800h
        ja     NEAR PTR loc_1_6523
        mov    edx, dword ptr [ebp + 28h]
        mov    ebx, dword ptr [ebp + 24h]
        mov    eax, dword ptr [ebp + 1ch]
        db  0A3h, 054h, 0B7h, 000h, 000h
        xor    eax, eax
        mov    byte ptr [ebp + 34h], al
        mov    eax, 0cch
        call   sub_1_A1CE
        jb     NEAR PTR loc_1_6523
        mov    edi, eax
        sub    eax, eax
        mov    ecx, 33h
        rep stosd
        lea    eax, [edi - 0cch]
        mov    dword ptr [ebp + 1ch], eax
        mov    dword ptr [eax + 10h], ebx
        mov    byte ptr [eax + 0ach], dl
        mov    byte ptr [eax + 0adh], dl
        mov    ebx, dword ptr [ebp + 10h]
        db  0C7h, 043h, 018h, 058h, 062h, 000h, 000h
        mov    dword ptr [eax + 14h], ebx
        or     word ptr [ebp + 38h], 400h
        db  08Bh, 01Dh, 07Ch, 0B8h, 000h, 000h
        mov    dword ptr [eax + 74h], ebx
        db  08Bh, 01Dh, 074h, 0B8h, 000h, 000h
        mov    dword ptr [eax + 7ch], ebx
        db  08Bh, 01Dh, 078h, 0B8h, 000h, 000h
        mov    dword ptr [eax + 70h], ebx
        mov    byte ptr [eax + 1fh], 30h
        xchg   esi, eax
        push   0
        push   800h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_1_650F
        xchg   esi, eax
        push   esi
        mov    ebx, dword ptr [ebp + 1ch]
        mov    ecx, 1
        sub    edi, edi
        push   esi
        call   sub_1_1458
        pop    esi
        jb     NEAR PTR loc_1_6502
        push   esi
        call   sub_1_68DF
        pop    esi
        jb     NEAR PTR loc_1_6502
        mov    ebx, dword ptr [ebp + 1ch]
        mov    eax, dword ptr [ebx + 14h]
        test   byte ptr [eax + 1], 40h
        je     SHORT loc_1_6410
        call   sub_1_A5B7
loc_1_6410:
        pop    esi
        mov    ebx, dword ptr [ebp + 1ch]
        test   byte ptr [ebx + 52h], 3
        jne    SHORT loc_1_6463
        mov    ecx, 1
        mov    edi, dword ptr [ebx + 0a0h]
        cmp    edi, -1
        je     SHORT loc_1_6463
        push   ebx
        push   esi
        call   sub_1_1458
        pop    esi
        pop    ebx
        jb     NEAR PTR loc_1_6503
        add    esi, 1e4h
        cmp    dword ptr [esi], 61417272h
        jne    SHORT loc_1_645D
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ebx + 40h], eax
        mov    eax, dword ptr [esi + 8]
        cmp    eax, 2
        jb     SHORT loc_1_645D
        cmp    dword ptr [ebx + 2ch], eax
        jb     SHORT loc_1_645D
        mov    dword ptr [ebx + 38h], eax
loc_1_645D:
        sub    esi, 1e4h
loc_1_6463:
        push   0
        push   esi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        db  0B8h, 048h, 0B7h, 000h, 000h
loc_1_6474:
        mov    eax, dword ptr [eax]
        db  03Dh, 048h, 0B7h, 000h, 000h
        je     SHORT loc_1_64CE
        call   sub_1_684B
        jne    SHORT loc_1_6474
        mov    edx, dword ptr [eax + 14h]
        test   byte ptr [edx + 1], 40h
        jne    SHORT loc_1_64BB
loc_1_648D:
        mov    dword ptr [ebp + 1ch], edx
        test   byte ptr [ebx + 52h], 80h
        je     SHORT loc_1_649B
        call   sub_1_A72F
loc_1_649B:
        and    word ptr [eax + 52h], 0feffh
        mov    dx, word ptr [ebx + 52h]
        and    dx, 100h
        or     word ptr [eax + 52h], dx
        xchg   ebx, eax
        call   sub_1_A1F8
        xchg   ebx, eax
        mov    eax, 11ch
        ret
loc_1_64BB:
        or     dword ptr [eax + 40h], 0ffffffffh
        cmp    dword ptr [eax + 68h], 0
        jne    SHORT loc_1_648D
        xchg   ebx, eax
        call   sub_1_A72F
        xchg   ebx, eax
        jmp    SHORT loc_1_648D
loc_1_64CE:
        mov    eax, dword ptr [ebx + 14h]
        mov    dword ptr [eax + 14h], ebx
        pushfd
        cli
        db  0B8h, 048h, 0B7h, 000h, 000h
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebx], ecx
        mov    dword ptr [ebx + 4], eax
        mov    dword ptr [ecx + 4], ebx
        mov    dword ptr [eax], ebx
        popfd
        mov    edx, dword ptr [ebp + 1ch]
        call   sub_1_6DDA
        call   sub_1_6529
        call   sub_1_6556
        call   sub_1_6EDA
        sub    eax, eax
        ret
loc_1_6502:
        pop    esi
loc_1_6503:
        push   0
        push   esi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_1_650F:
        mov    eax, dword ptr [ebp + 1ch]
        call   sub_1_A1F8
        movzx  ecx, byte ptr [ebp + 34h]
        jecxz  loc_1_6523
        db  0CDh, 020h, 00Fh, 080h, 010h, 000h
loc_1_6523:
        mov    eax, 5
        ret
sub_1_6529:
        mov    edx, dword ptr [ebp + 1ch]
        mov    dword ptr [edx + 90h], 0
        mov    eax, dword ptr [edx + 14h]
        test   byte ptr [eax + 1], 40h
        jne    SHORT loc_1_6555
        db  0A1h, 07Ch, 0B2h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_6555
        call   sub_1_A1E0
        jb     SHORT loc_1_6555
        mov    dword ptr [edx + 90h], eax
loc_1_6555:
        ret
sub_1_6556:
        mov    edx, dword ptr [ebp + 1ch]
        mov    eax, dword ptr [edx + 2ch]
        shr    eax, 0ah
        cmp    eax, 1eh
        jb     SHORT loc_1_6592
        lea    eax, [eax + eax + 4]
        push   eax
        call   sub_1_A1CE
        pop    ecx
        jb     SHORT loc_1_6592
        mov    dword ptr [edx + 9ch], eax
        push   edi
        mov    edi, eax
        or     eax, 0ffffffffh
        shrd   edx, ecx, 2
        cld
        shr    ecx, 2
        rep stosd
        shld   ecx, edx, 2
        rep stosb
        dec    word ptr [edi - 2]
        pop    edi
loc_1_6592:
        ret
sub_1_6593:
        mov    ebx, dword ptr [ebp + 28h]
        mov    eax, 0cch
        call   sub_1_A516
        jb     NEAR PTR loc_1_66A7
        mov    edi, eax
        sub    eax, eax
        mov    ecx, 33h
        rep stosd
        lea    eax, [edi - 0cch]
        mov    dword ptr [eax + 10h], 0
        mov    dword ptr [ebp + 1ch], eax
        mov    dl, byte ptr [ebx + 0ach]
        mov    byte ptr [eax + 0ach], dl
        mov    dl, byte ptr [ebx + 0adh]
        mov    byte ptr [eax + 0adh], dl
        or     word ptr [ebp + 38h], 500h
        db  08Bh, 015h, 07Ch, 0B8h, 000h, 000h
        mov    dword ptr [eax + 74h], edx
        db  08Bh, 015h, 074h, 0B8h, 000h, 000h
        mov    dword ptr [eax + 7ch], edx
        db  08Bh, 015h, 078h, 0B8h, 000h, 000h
        mov    dword ptr [eax + 70h], edx
        mov    byte ptr [eax + 1fh], 30h
        mov    edx, dword ptr [ebx + 14h]
        mov    dword ptr [eax + 14h], edx
        mov    eax, 800h
        call   sub_1_A516
        or     eax, eax
        je     NEAR PTR loc_1_669F
        mov    esi, eax
        mov    ebx, dword ptr [ebp + 1ch]
        mov    ecx, 1
        sub    edi, edi
        push   esi
        call   sub_1_1458
        pop    esi
        jb     SHORT loc_1_668B
        push   esi
        call   sub_1_68DF
        pop    esi
        jb     SHORT loc_1_668B
        mov    ebx, dword ptr [ebp + 1ch]
        mov    eax, dword ptr [ebx + 14h]
        test   byte ptr [eax + 1], 40h
        je     SHORT loc_1_6645
        call   sub_1_A5B7
loc_1_6645:
        mov    eax, esi
        call   sub_1_A53E
        mov    ebx, dword ptr [ebp + 1ch]
        mov    eax, dword ptr [ebp + 28h]
        call   sub_1_684B
        pushf
        mov    eax, ebx
        call   sub_1_A53E
        popf
        jne    SHORT loc_1_6667
        xor    eax, eax
        ret
loc_1_6667:
        mov    eax, dword ptr [ebp + 28h]
        mov    ebx, dword ptr [eax + 54h]
        mov    dword ptr [ebp + 28h], ebx
        movzx  ecx, byte ptr [eax + 0b9h]
        lea    esi, [eax + 0aeh]
        mov    edi, dword ptr [ebp + 14h]
        rep movsb
        xor    al, al
        stosb
        mov    eax, 0ffffffffh
        ret
loc_1_668B:
        mov    eax, esi
        call   sub_1_A53E
        mov    eax, dword ptr [ebp + 1ch]
        call   sub_1_A53E
        mov    eax, dword ptr [ebp + 28h]
        jmp    SHORT loc_1_6667
loc_1_669F:
        mov    eax, dword ptr [ebp + 1ch]
        call   sub_1_A53E
loc_1_66A7:
        mov    eax, 5
        ret
sub_1_66AD:
        db  0C7h, 045h, 010h, 08Ch, 022h, 000h, 000h
        mov    ebx, dword ptr [ebp + 1ch]
        mov    eax, 0cch
        call   sub_1_A1CE
        jb     NEAR PTR loc_1_681E
        mov    edi, eax
        mov    ecx, 33h
        mov    esi, ebx
        rep movsd
        mov    edi, eax
        mov    dword ptr [ebp + 1ch], edi
        mov    edx, dword ptr [ebp + 28h]
        mov    byte ptr [edi + 0ach], dl
        mov    byte ptr [edi + 0adh], dl
        xor    eax, eax
        mov    dword ptr [edi + 9ch], eax
        mov    dword ptr [edi], eax
        mov    dword ptr [edi + 4], eax
        mov    byte ptr [edi + 0bbh], al
        mov    dword ptr [edi + 64h], eax
        mov    dword ptr [edi + 68h], eax
        mov    dword ptr [edi + 6ch], eax
        mov    dword ptr [edi + 78h], eax
        mov    dword ptr [edi + 80h], eax
        mov    dword ptr [edi + 84h], eax
        mov    dword ptr [edi + 88h], eax
        mov    dword ptr [edi + 8ch], eax
        mov    dword ptr [edi + 0bch], eax
        mov    dword ptr [edi + 0c0h], eax
        mov    dword ptr [edi + 0c4h], eax
        mov    dword ptr [edi + 0c8h], eax
        mov    word ptr [edi + 52h], ax
        mov    dword ptr [edi + 90h], eax
        mov    byte ptr [edi + 1fh], 30h
        db  0A1h, 07Ch, 0B8h, 000h, 000h
        mov    dword ptr [edi + 74h], eax
        db  0A1h, 074h, 0B8h, 000h, 000h
        mov    dword ptr [edi + 7ch], eax
        db  0A1h, 078h, 0B8h, 000h, 000h
        mov    dword ptr [edi + 70h], eax
        push   800h
        db  0CDh, 020h, 00Dh, 000h, 040h, 000h
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_1_67FD
        mov    esi, eax
        push   esi
        mov    ebx, edi
        mov    ecx, 1
        sub    edi, edi
        call   sub_1_1458
        pop    esi
        jb     SHORT loc_1_67F3
        push   esi
        call   sub_1_68DF
        pop    esi
        jb     SHORT loc_1_67F3
        push   esi
        db  0CDh, 020h, 00Eh, 000h, 040h, 000h
        add    esp, 4
        mov    ebx, dword ptr [ebp + 1ch]
        db  0B8h, 048h, 0B7h, 000h, 000h
loc_1_67A0:
        mov    eax, dword ptr [eax]
        db  03Dh, 048h, 0B7h, 000h, 000h
        je     SHORT loc_1_67C5
        call   sub_1_684B
        jne    SHORT loc_1_67A0
        mov    ecx, dword ptr [eax + 14h]
        cmp    dword ptr [ebx + 14h], ecx
        jne    SHORT loc_1_67A0
        xchg   ebx, eax
        call   sub_1_A1F8
        xchg   ebx, eax
        mov    eax, 55h
        ret
loc_1_67C5:
        pushfd
        cli
        db  0B8h, 048h, 0B7h, 000h, 000h
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebx], ecx
        mov    dword ptr [ebx + 4], eax
        mov    dword ptr [ecx + 4], ebx
        mov    dword ptr [eax], ebx
        popfd
        or     word ptr [ebx + 52h], 800h
        mov    edx, ebx
        call   sub_1_6DDA
        call   sub_1_6529
        call   sub_1_6556
        xor    eax, eax
        ret
loc_1_67F3:
        push   esi
        db  0CDh, 020h, 00Eh, 000h, 040h, 000h
        add    esp, 4
loc_1_67FD:
        mov    eax, dword ptr [ebp + 1ch]
        mov    esi, dword ptr [eax + 14h]
        call   sub_1_A1F8
        db  0B8h, 048h, 0B7h, 000h, 000h
loc_1_680D:
        mov    eax, dword ptr [eax]
        db  03Dh, 048h, 0B7h, 000h, 000h
        je     SHORT loc_1_681E
        cmp    dword ptr [eax + 14h], esi
        jne    SHORT loc_1_680D
        mov    dword ptr [ebp + 1ch], eax
loc_1_681E:
        db  0C7h, 045h, 010h, 090h, 0B7h, 000h, 000h
        xor    eax, eax
        ret
loc_1_6828:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, 5
        mov    word ptr [ecx + 1ah], ax
        ret
loc_1_6836:
        mov    ecx, dword ptr [esp + 4]
        cmp    byte ptr [ecx + 4], 2
        jne    NEAR PTR loc_1_3F50
        xor    eax, eax
        mov    word ptr [ecx + 1ah], ax
        ret
sub_1_684B:
        mov    edx, dword ptr [ebx + 54h]
        cmp    dword ptr [eax + 54h], edx
        jne    NEAR PTR loc_1_68DE
        lea    edi, [eax + 20h]
        lea    esi, [ebx + 20h]
        mov    ecx, 6
        repe cmpsd
        jne    SHORT loc_1_68DE
        movzx  edx, word ptr [eax + 44h]
        cmp    word ptr [ebx + 44h], dx
        jne    SHORT loc_1_68DE
        lea    edi, [eax + 0aeh]
        lea    esi, [ebx + 0aeh]
        mov    ecx, 0dh
        repe cmpsb
        jne    SHORT loc_1_68DE
        mov    dl, byte ptr [ebx + 0ach]
        mov    esi, dword ptr [eax + 14h]
        db  0B9h, 048h, 0B7h, 000h, 000h
loc_1_6893:
        mov    ecx, dword ptr [ecx]
        db  081h, 0F9h, 048h, 0B7h, 000h, 000h
        je     SHORT loc_1_68B6
        cmp    dword ptr [ecx + 14h], esi
        jne    SHORT loc_1_6893
        cmp    byte ptr [ecx + 0ach], dl
        jne    SHORT loc_1_6893
        test   byte ptr [eax + 53h], 2
        je     SHORT loc_1_68C2
        test   byte ptr [ebx + 53h], 2
        jne    SHORT loc_1_68BA
loc_1_68B6:
        or     ebx, ebx
        jmp    SHORT loc_1_68DE
loc_1_68BA:
        mov    edx, dword ptr [eax + 5ch]
        cmp    dword ptr [ebx + 5ch], edx
        jmp    SHORT loc_1_68DE
loc_1_68C2:
        mov    edx, dword ptr [eax + 58h]
        cmp    dword ptr [ebx + 58h], edx
        jne    SHORT loc_1_68DE
        test   byte ptr [ebx + 53h], 2
        je     SHORT loc_1_68DE
        mov    edx, dword ptr [ebx + 5ch]
        mov    dword ptr [eax + 5ch], edx
        or     word ptr [eax + 52h], 200h
        cmp    eax, eax
loc_1_68DE:
        ret
sub_1_68DF:
        mov    edi, dword ptr [ebp + 1ch]
        and    word ptr [edi + 52h], 0feffh
        mov    eax, dword ptr [edi + 14h]
        test   byte ptr [eax + 4], 40h
        jne    SHORT loc_1_6901
        movzx  eax, byte ptr [edi + 0ach]
        db  00Fh, 0A3h, 005h, 068h, 0B8h, 000h, 000h
        jae    SHORT loc_1_6907
loc_1_6901:
        or     word ptr [edi + 52h], 100h
loc_1_6907:
        mov    byte ptr [edi + 1eh], 1
        or     dword ptr [edi + 0a0h], 0ffffffffh
        or     dword ptr [edi + 0a4h], 0ffffffffh
        cmp    word ptr [esi + 16h], 0
        je     NEAR PTR loc_1_6A7F
        mov    byte ptr [edi + 48h], 0
        cmp    byte ptr [esi + 26h], 29h
        je     SHORT loc_1_6936
        or     word ptr [edi + 52h], 80h
        jmp    SHORT loc_1_695E
loc_1_6936:
        mov    eax, dword ptr [esi + 27h]
        mov    dword ptr [edi + 54h], eax
        push   edi
        push   esi
        lea    edi, [edi + 0aeh]
        lea    esi, [esi + 2bh]
        mov    ecx, 0bh
        rep movsb
        lea    edi, [edi - 0bh]
        call   sub_1_6EC3
        pop    esi
        pop    edi
        mov    byte ptr [edi + 0b9h], al
loc_1_695E:
        lea    esi, [esi + 0bh]
        lodsw
        mov    word ptr [edi + 20h], ax
        and    al, al
        jne    NEAR PTR loc_1_6CA6
        cmp    ah, 2
        je     SHORT loc_1_6987
        cmp    ah, 4
        je     SHORT loc_1_6987
        cmp    ah, 8
        je     SHORT loc_1_6987
        cmp    ah, 10h
        jne    NEAR PTR loc_1_6CA6
loc_1_6987:
        mov    edx, eax
        dec    eax
        mov    word ptr [edi + 22h], ax
        lodsb
        sub    al, 1
        jb     NEAR PTR loc_1_6CA6
        mov    byte ptr [edi + 24h], al
        inc    al
        movzx  eax, al
        bsf    ebx, eax
        mov    byte ptr [edi + 25h], bl
        btr    eax, ebx
        or     eax, eax
        jne    NEAR PTR loc_1_6CA6
        lodsw
        mov    word ptr [edi + 26h], ax
        lodsb
        mov    byte ptr [edi + 46h], al
        and    al, al
        je     NEAR PTR loc_1_6CA6
        cmp    al, 3
        jae    NEAR PTR loc_1_6CA6
        mov    bh, al
        lodsw
        mov    word ptr [edi + 44h], ax
        movzx  edx, dx
        shr    edx, 5
        lea    eax, [edx + eax - 1]
        mov    ecx, edx
        xor    edx, edx
        div    ecx
        mov    ecx, eax
        add    esi, 2
        lodsb
        mov    byte ptr [edi + 0bah], al
        lodsw
        movzx  eax, ax
        mov    dword ptr [edi + 30h], eax
        movzx  edx, bh
        mul    edx
        movzx  edx, word ptr [edi + 26h]
        add    eax, edx
        mov    dword ptr [edi + 34h], eax
        add    eax, ecx
        mov    dword ptr [edi + 28h], eax
        mov    cl, bl
        cmp    word ptr [esi - 5], 0
        jne    SHORT loc_1_6A17
        mov    ebx, dword ptr [esi + 8]
        jmp    SHORT loc_1_6A1B
loc_1_6A17:
        movzx  ebx, word ptr [esi - 5]
loc_1_6A1B:
        mov    dword ptr [edi + 8], ebx
        sub    ebx, eax
        jb     NEAR PTR loc_1_6CA6
        shr    ebx, cl
        inc    ebx
        mov    dword ptr [edi + 2ch], ebx
        mov    ebx, dword ptr [edi + 30h]
        movzx  eax, word ptr [edi + 20h]
        imul   eax, ebx
        cmp    dword ptr [edi + 2ch], 0ff6h
        jae    SHORT loc_1_6A5E
        add    eax, eax
        sub    edx, edx
        mov    ebx, 3
        div    ebx
        dec    eax
        mov    cl, 1
        cmp    dword ptr [edi + 2ch], eax
        jbe    NEAR PTR loc_1_6BF8
        mov    dword ptr [edi + 2ch], eax
        jmp    NEAR PTR loc_1_6BF8
loc_1_6A5E:
        shr    eax, 1
        mov    cl, 2
        cmp    eax, 0ff7h
        jb     NEAR PTR loc_1_6BF8
        dec    eax
        cmp    dword ptr [edi + 2ch], eax
        jbe    NEAR PTR loc_1_6BF8
        mov    dword ptr [edi + 2ch], eax
        jmp    NEAR PTR loc_1_6BF8
loc_1_6A7F:
        cmp    byte ptr [esi + 42h], 29h
        jne    NEAR PTR loc_1_6CA6
        cmp    dword ptr [esi + 52h], 33544146h
        jne    NEAR PTR loc_1_6CA6
        cmp    dword ptr [esi + 56h], 20202032h
        jne    NEAR PTR loc_1_6CA6
        cmp    word ptr [esi + 2ah], 0
        ja     NEAR PTR loc_1_6CA6
        mov    eax, dword ptr [esi + 43h]
        mov    dword ptr [edi + 54h], eax
        push   edi
        push   esi
        lea    edi, [edi + 0aeh]
        lea    esi, [esi + 47h]
        mov    ecx, 0bh
        rep movsb
        sub    edi, 0bh
        call   sub_1_6EC3
        pop    esi
        pop    edi
        mov    byte ptr [edi + 0b9h], al
        movzx  eax, word ptr [esi + 0bh]
        mov    word ptr [edi + 20h], ax
        and    al, al
        jne    NEAR PTR loc_1_6CA6
        cmp    ah, 2
        je     SHORT loc_1_6AFE
        cmp    ah, 4
        je     SHORT loc_1_6AFE
        cmp    ah, 8
        je     SHORT loc_1_6AFE
        cmp    ah, 10h
        jne    NEAR PTR loc_1_6CA6
loc_1_6AFE:
        mov    edx, eax
        dec    eax
        mov    word ptr [edi + 22h], ax
        movzx  eax, byte ptr [esi + 0dh]
        sub    eax, 1
        jb     NEAR PTR loc_1_6CA6
        mov    byte ptr [edi + 24h], al
        inc    eax
        bsf    ebx, eax
        mov    byte ptr [edi + 25h], bl
        btr    eax, ebx
        or     eax, eax
        jne    NEAR PTR loc_1_6CA6
        movzx  ecx, word ptr [esi + 0eh]
        mov    word ptr [edi + 26h], cx
        or     ecx, ecx
        je     NEAR PTR loc_1_6CA6
        movzx  eax, word ptr [esi + 30h]
        or     eax, eax
        je     SHORT loc_1_6B49
        cmp    eax, ecx
        jae    SHORT loc_1_6B49
        mov    dword ptr [edi + 0a0h], eax
loc_1_6B49:
        movzx  eax, word ptr [esi + 32h]
        or     eax, eax
        je     SHORT loc_1_6B5B
        cmp    eax, ecx
        jae    SHORT loc_1_6B5B
        mov    dword ptr [edi + 0a4h], eax
loc_1_6B5B:
        mov    al, byte ptr [esi + 10h]
        mov    byte ptr [edi + 46h], al
        or     al, al
        je     NEAR PTR loc_1_6CA6
        cmp    al, 2
        ja     NEAR PTR loc_1_6CA6
        mov    al, byte ptr [esi + 15h]
        mov    byte ptr [edi + 0bah], al
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [edi + 30h], eax
        cmp    byte ptr [edi + 46h], 2
        jb     SHORT loc_1_6B88
        add    eax, eax
loc_1_6B88:
        add    eax, ecx
        mov    dword ptr [edi + 28h], eax
        mov    cl, bl
        mov    ebx, dword ptr [esi + 20h]
        mov    dword ptr [edi + 8], ebx
        sub    ebx, eax
        jb     NEAR PTR loc_1_6CA6
        shr    ebx, cl
        inc    ebx
        mov    dword ptr [edi + 2ch], ebx
        mov    eax, dword ptr [edi + 30h]
        shr    edx, 2
        mul    edx
        or     edx, edx
        jne    SHORT loc_1_6BB8
        dec    eax
        cmp    dword ptr [edi + 2ch], eax
        jbe    SHORT loc_1_6BB8
        mov    dword ptr [edi + 2ch], eax
loc_1_6BB8:
        cmp    dword ptr [edi + 2ch], 0fff6h
        jb     NEAR PTR loc_1_6CA6
        mov    eax, dword ptr [esi + 2ch]
        mov    dword ptr [edi + 3ch], eax
        cmp    eax, 2
        jb     NEAR PTR loc_1_6CA6
        cmp    dword ptr [edi + 2ch], eax
        jb     NEAR PTR loc_1_6CA6
        movzx  eax, word ptr [esi + 28h]
        xor    ecx, ecx
        test   al, 80h
        je     SHORT loc_1_6BF8
        or     ecx, 4
        and    al, 0fh
        mov    byte ptr [edi + 48h], al
        cmp    byte ptr [edi + 46h], al
        jbe    NEAR PTR loc_1_6CA6
loc_1_6BF8:
        or     cl, 20h
        mov    eax, dword ptr [edi + 14h]
        cmp    word ptr [eax + 8], 10h
        jb     SHORT loc_1_6C08
        and    cl, 0dfh
loc_1_6C08:
        or     byte ptr [edi + 52h], cl
        and    word ptr [edi + 52h], 0fbffh
        db  0F6h, 005h, 06Fh, 0B8h, 000h, 000h, 080h
        jne    SHORT loc_1_6C2C
        movzx  ecx, byte ptr [edi + 0ach]
        db  00Fh, 0A3h, 00Dh, 06Ch, 0B8h, 000h, 000h
        jae    SHORT loc_1_6C38
        jmp    SHORT loc_1_6C3E
loc_1_6C2C:
        test   byte ptr [eax + 1], 40h
        je     SHORT loc_1_6C3E
        test   byte ptr [eax + 2], 1
        jne    SHORT loc_1_6C3E
loc_1_6C38:
        or     word ptr [edi + 52h], 400h
loc_1_6C3E:
        movzx  eax, word ptr [edi + 20h]
        bsf    ecx, eax
        mov    byte ptr [edi + 4ch], cl
        add    cl, byte ptr [edi + 25h]
        mov    byte ptr [edi + 47h], cl
        or     eax, 0ffffffffh
        mov    dword ptr [edi + 40h], eax
        shl    eax, cl
        not    eax
        mov    word ptr [edi + 4ah], ax
        mov    dword ptr [edi + 38h], 2
        mov    eax, 1000h
        mov    cl, byte ptr [edi + 4ch]
        shr    eax, cl
        mov    byte ptr [edi + 4dh], al
        dec    eax
        mov    byte ptr [edi + 4eh], al
loc_1_6C74:
        and    eax, dword ptr [edi + 28h]
        mov    dword ptr [edi + 0ch], eax
        movzx  ebx, word ptr [edi + 4ah]
        inc    ebx
        cmp    ebx, 1000h
        jb     SHORT loc_1_6C8C
        mov    ebx, 1000h
loc_1_6C8C:
        setb   al
        inc    eax
        mov    byte ptr [edi + 4fh], al
        neg    ebx
        mov    word ptr [edi + 50h], bx
        movzx  eax, word ptr [edi + 26h]
        add    eax, dword ptr [edi + 30h]
        mov    dword ptr [edi + 18h], eax
        mov    edx, edi
        ret
loc_1_6CA6:
        mov    edi, dword ptr [ebp + 1ch]
        mov    ebx, dword ptr [edi + 14h]
        test   byte ptr [ebx + 1], 40h
        je     NEAR PTR loc_1_6DD8
        mov    ecx, dword ptr [ebx + 10h]
        cmp    ecx, 200h
        jne    NEAR PTR loc_1_6DD8
        push   edi
        push   esi
        mov    ebx, edi
        mov    ecx, 1
        mov    edi, ecx
        call   sub_1_1458
        pop    esi
        pop    edi
        jb     NEAR PTR loc_1_6DD8
        mov    ah, byte ptr [esi]
        cmp    ah, 0f8h
        jb     NEAR PTR loc_1_6DD8
        mov    al, 1
        mov    ebx, 4008h
        mov    ecx, 140h
        mov    edx, 101h
        test   ah, 2
        jne    SHORT loc_1_6D05
        inc    al
        inc    bl
        add    ecx, 28h
loc_1_6D05:
        test   ah, 1
        je     SHORT loc_1_6D12
        add    ecx, ecx
        mov    bh, 70h
        inc    dh
        inc    dl
loc_1_6D12:
        mov    byte ptr [edi + 0bah], ah
        mov    dword ptr [edi + 8], ecx
        movzx  ecx, al
        mov    dword ptr [edi + 30h], ecx
        inc    ecx
        mov    dword ptr [edi + 18h], ecx
        add    ecx, ecx
        dec    ecx
        mov    dword ptr [edi + 34h], ecx
        movzx  ecx, dh
        bsf    ecx, ecx
        mov    byte ptr [edi + 25h], cl
        dec    dh
        mov    byte ptr [edi + 24h], dh
        mov    eax, 200h
        mov    word ptr [edi + 20h], ax
        shl    eax, cl
        bsf    ecx, eax
        mov    byte ptr [edi + 47h], cl
        dec    eax
        mov    word ptr [edi + 4ah], ax
        movzx  ecx, bh
        mov    word ptr [edi + 44h], cx
        shr    ecx, 4
        add    ecx, dword ptr [edi + 18h]
        add    ecx, dword ptr [edi + 30h]
        mov    dword ptr [edi + 28h], ecx
        mov    eax, dword ptr [edi + 8]
        sub    eax, ecx
        mov    cl, byte ptr [edi + 25h]
        shr    eax, cl
        inc    eax
        mov    dword ptr [edi + 2ch], eax
        mov    eax, dword ptr [edi + 8]
        mov    word ptr [edi + 22h], 1ffh
        mov    byte ptr [edi + 4ch], 9
        mov    byte ptr [edi + 46h], 2
        mov    word ptr [edi + 26h], 1
        mov    byte ptr [edi + 4dh], 8
        mov    byte ptr [edi + 4eh], 7
        mov    word ptr [edi + 52h], 1181h
        mov    dword ptr [edi + 0aeh], 0
        mov    dword ptr [edi + 0b2h], 0
        mov    word ptr [edi + 0b6h], 0
        mov    byte ptr [edi + 0b8h], 0
        mov    byte ptr [edi + 0b9h], 0
        mov    byte ptr [edi + 1eh], 1
        or     dword ptr [edi + 40h], 0ffffffffh
        mov    dword ptr [edi + 38h], 2
        movzx  eax, byte ptr [edi + 4dh]
        jmp    NEAR PTR loc_1_6C74
loc_1_6DD8:
        stc
        ret
sub_1_6DDA:
        mov    ecx, dword ptr [edx + 14h]
        movzx  ebx, word ptr [ecx + 0ah]
        mov    esi, dword ptr [ecx + 0ch]
        mov    ah, 2
        call   sub_1_A4E1
        mov    esi, ebx
        mov    dword ptr [esi + 44h], ecx
        mov    al, byte ptr [edx + 0ach]
        mov    byte ptr [esi + 4dh], al
        mov    dword ptr [esi + 8], 500h
        bt     dword ptr [ebp + 38h], 0ah
        sbb    eax, eax
        and    eax, 808h
        or     dword ptr [esi + 8], eax
        mov    word ptr [esi + 4], 29h
        mov    word ptr [esi + 2eh], 0db00h
        push   edx
        db  0CDh, 020h, 004h, 000h, 010h, 000h
        pop    edx
        and    word ptr [edx + 52h], 0ffefh
        cmp    word ptr [esi + 6], 10h
        jae    SHORT loc_1_6E34
        or     word ptr [edx + 52h], 10h
loc_1_6E34:
        mov    ebx, esi
        call   sub_1_A4AE
        ret
sub_1_6E3C:
        sub    esp, 4
        mov    dword ptr [esp], eax
        sub    ebx, ebx
        call   sub_1_00D0
        jb     SHORT loc_1_6EBE
        lea    edi, [ebx + 47h]
        cmp    word ptr [ebx + 16h], 0
        je     SHORT loc_1_6E5E
        lea    edi, [ebx + 2bh]
        cmp    byte ptr [ebx + 26h], 29h
        jne    SHORT loc_1_6E7E
loc_1_6E5E:
        mov    eax, esi
        mov    esi, dword ptr [esp]
        test   esi, esi
        jne    SHORT loc_1_6E6C
        db  0BEh, 070h, 0B7h, 000h, 000h
loc_1_6E6C:
        mov    ecx, 0bh
        rep movsb
        mov    esi, eax
        push   ebx
        call   sub_1_55D2
        pop    ebx
        jb     SHORT loc_1_6EBE
loc_1_6E7E:
        mov    edi, dword ptr [ebp + 1ch]
        mov    esi, dword ptr [esp]
        test   esi, esi
        jne    SHORT loc_1_6E9F
        db  0BEh, 070h, 0B7h, 000h, 000h
        cmp    word ptr [ebx + 16h], 0
        je     SHORT loc_1_6E9F
        cmp    byte ptr [ebx + 26h], 29h
        je     SHORT loc_1_6E9F
        db  0BEh, 07Bh, 0B7h, 000h, 000h
loc_1_6E9F:
        lea    edi, [edi + 0aeh]
        mov    ecx, 0bh
        rep movsb
        lea    edi, [edi - 0bh]
        call   sub_1_6EC3
        mov    edi, dword ptr [ebp + 1ch]
        mov    byte ptr [edi + 0b9h], al
        clc
loc_1_6EBE:
        lea    esp, [esp + 4]
        ret
sub_1_6EC3:
        mov    eax, 0bh
loc_1_6EC8:
        cmp    byte ptr [eax + edi - 1], 20h
        je     SHORT loc_1_6ED6
        cmp    byte ptr [eax + edi - 1], 0
        jne    SHORT loc_1_6ED9
loc_1_6ED6:
        dec    eax
        jne    SHORT loc_1_6EC8
loc_1_6ED9:
        ret
sub_1_6EDA:
        mov    edx, dword ptr [ebp + 1ch]
        mov    eax, dword ptr [edx + 14h]
        test   byte ptr [eax + 1], 40h
        jne    NEAR PTR loc_1_6FE3
        test   byte ptr [edx + 52h], 10h
        jne    NEAR PTR loc_1_6FE3
        test   byte ptr [edx + 53h], 8
        jne    NEAR PTR loc_1_6FE3
        test   byte ptr [edx + 52h], 3
        je     NEAR PTR loc_1_6FE3
        cmp    byte ptr [edx + 0ach], 2
        ja     NEAR PTR loc_1_6FE3
        db  08Bh, 03Dh, 054h, 0B7h, 000h, 000h
loc_1_6F1B:
        movzx  eax, byte ptr [edx + 0ach]
        cmp    byte ptr [edi], al
        je     SHORT loc_1_6F40
        mov    edi, dword ptr [edi + 19h]
        cmp    di, -1
        je     NEAR PTR loc_1_6FE3
        movzx  ecx, di
        shr    edi, 10h
        shl    edi, 4
        add    edi, ecx
        jmp    SHORT loc_1_6F1B
loc_1_6F40:
        cmp    byte ptr [edi + 17h], 0ffh
        je     NEAR PTR loc_1_6FE3
        mov    al, byte ptr [edx + 0bah]
        cmp    byte ptr [edi + 17h], al
        jne    SHORT loc_1_6FB1
        mov    ax, word ptr [edx + 20h]
        cmp    word ptr [edi + 2], ax
        jne    SHORT loc_1_6FB1
        mov    al, byte ptr [edx + 24h]
        cmp    byte ptr [edi + 4], al
        jne    SHORT loc_1_6FB1
        mov    al, byte ptr [edx + 25h]
        cmp    byte ptr [edi + 5], al
        jne    SHORT loc_1_6FB1
        mov    eax, dword ptr [edx + 2ch]
        cmp    word ptr [edi + 0dh], ax
        jne    SHORT loc_1_6FB1
        mov    ax, word ptr [edx + 44h]
        cmp    word ptr [edi + 9], ax
        jne    SHORT loc_1_6FB1
        mov    ax, word ptr [edx + 26h]
        cmp    word ptr [edi + 6], ax
        jne    SHORT loc_1_6FB1
        mov    al, byte ptr [edx + 46h]
        cmp    byte ptr [edi + 8], al
        jne    SHORT loc_1_6FB1
        mov    eax, dword ptr [edx + 30h]
        cmp    word ptr [edi + 0fh], ax
        jne    SHORT loc_1_6FB1
        mov    eax, dword ptr [edx + 28h]
        cmp    word ptr [edi + 0bh], ax
        jne    SHORT loc_1_6FB1
        mov    eax, dword ptr [edx + 34h]
        cmp    word ptr [edi + 11h], ax
        jne    SHORT loc_1_6FB1
        jmp    SHORT loc_1_6FE3
loc_1_6FB1:
        mov    al, byte ptr [edx + 0ach]
        or     al, 41h
        db  0A2h, 014h, 0B9h, 000h, 000h
        db  08Dh, 00Dh, 014h, 0B9h, 000h, 000h
        push   ecx
        push   0c0003h
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 059h, 001h, 001h, 000h
        pushad
        mov    esi, eax
        mov    eax, 1
        db  0CDh, 020h, 0BEh, 000h, 001h, 000h
loc_1_6FE3:
        ret
sub_1_6FE4:
        mov    edi, dword ptr [ebp + 1ch]
        mov    ecx, dword ptr [ebp]
        shr    ecx, 0ch
        je     SHORT loc_1_6FFE
        inc    ecx
        db  08Ah, 025h, 0F8h, 0B6h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 08Bh, 004h
        jb     SHORT loc_1_7008
loc_1_6FFE:
        test   byte ptr [ebp + 37h], 1
        jne    SHORT loc_1_7008
        or     byte ptr [ebp + 3ah], 4
loc_1_7008:
        mov    esi, dword ptr [ebp + 20h]
        call   sub_1_774E
        jb     SHORT loc_1_707E
        mov    esi, dword ptr [ebp + 20h]
        call   sub_1_7704
        mov    esi, dword ptr [ebp + 20h]
        movzx  eax, word ptr [ebp + 3ch]
        and    ax, word ptr [edi + 22h]
        mov    dword ptr [ebp + 6ch], eax
        cmp    dword ptr [ebp + 4ch], 0
        je     SHORT loc_1_7031
        inc    dword ptr [ebp + 48h]
loc_1_7031:
        cmp    dword ptr [ebp + 44h], 0
        je     SHORT loc_1_703A
        inc    dword ptr [ebp + 48h]
loc_1_703A:
        mov    esi, dword ptr [ebp + 20h]
        mov    ecx, dword ptr [ebp + 48h]
        call   sub_1_76B7
        jb     SHORT loc_1_707E
        mov    eax, dword ptr [ebp + 44h]
        mov    edx, dword ptr [ebp + 4ch]
        cmp    ecx, dword ptr [ebp + 48h]
        je     SHORT loc_1_7054
        sub    edx, edx
loc_1_7054:
        push   ecx
        test   byte ptr [ebp + 37h], 10h
        jne    SHORT loc_1_7071
        call   sub_1_70F6
        jb     SHORT loc_1_707D
loc_1_7062:
        sub    ecx, ecx
        mov    dword ptr [ebp + 44h], ecx
        mov    dword ptr [ebp + 6ch], ecx
        pop    ecx
        sub    dword ptr [ebp + 48h], ecx
        jne    SHORT loc_1_703A
        ret
loc_1_7071:
        mov    esi, dword ptr [ebp + 20h]
        call   sub_1_7080
        jb     SHORT loc_1_707D
        jmp    SHORT loc_1_7062
loc_1_707D:
        pop    eax
loc_1_707E:
        stc
        ret
sub_1_7080:
        push   edi
        xchg   edi, ebx
        or     eax, edx
        jne    SHORT loc_1_70A3
        add    edi, dword ptr [ebx + 10h]
        mov    esi, dword ptr [ebp + 14h]
        push   ebx
        push   ecx
        call   sub_1_1458
        pop    eax
        pop    ebx
        jb     SHORT loc_1_70A1
        mov    cl, byte ptr [ebx + 4ch]
        shl    eax, cl
        add    dword ptr [ebp + 14h], eax
        clc
loc_1_70A1:
        pop    edi
        ret
loc_1_70A3:
        stc
        jmp    SHORT loc_1_70A1
sub_1_70A6:
        push   esi
        call   sub_1_774E
        pop    esi
        jb     SHORT loc_1_70D8
        push   esi
        call   sub_1_7704
        pop    esi
        cmp    dword ptr [ebp + 48h], 0
        je     SHORT loc_1_70D8
loc_1_70BC:
        mov    ecx, dword ptr [ebp + 48h]
        push   esi
        call   sub_1_76B7
        pop    esi
        jb     SHORT loc_1_70D8
        sub    dword ptr [ebp + 48h], ecx
        call   sub_1_70D9
        jb     SHORT loc_1_70D8
        cmp    dword ptr [ebp + 48h], 0
        jne    SHORT loc_1_70BC
loc_1_70D8:
        ret
sub_1_70D9:
        push   esi
        add    ebx, dword ptr [edi + 10h]
        mov    esi, dword ptr [ebp + 14h]
        push   edi
        push   ecx
        call   sub_1_1574
        pop    eax
        pop    edi
        jb     SHORT loc_1_70F4
        mov    cl, byte ptr [edi + 4ch]
        shl    eax, cl
        add    dword ptr [ebp + 14h], eax
        clc
loc_1_70F4:
        pop    esi
        ret
sub_1_70F6:
        mov    dword ptr [ebp + 58h], eax
        mov    dword ptr [ebp + 5ch], edx
        mov    dword ptr [ebp + 60h], ebx
        mov    dword ptr [ebp + 68h], ecx
        mov    dword ptr [ebp + 70h], 0
        test   byte ptr [ebp + 37h], 1
        jne    NEAR PTR loc_1_748A
loc_1_7113:
        and    eax, eax
        je     SHORT loc_1_7118
        dec    ecx
loc_1_7118:
        and    edx, edx
        je     SHORT loc_1_711D
        dec    ecx
loc_1_711D:
        mov    eax, ecx
        mov    cl, byte ptr [edi + 4ch]
        shl    eax, cl
        add    eax, dword ptr [ebp + 58h]
        add    eax, edx
        mov    edx, ebx
        sub    edx, dword ptr [edi + 0ch]
        movzx  esi, byte ptr [edi + 4eh]
        not    esi
        and    edx, esi
        add    edx, dword ptr [edi + 0ch]
        mov    dword ptr [ebp + 64h], edx
        sub    ebx, edx
        add    dword ptr [ebp + 68h], ebx
        shl    ebx, cl
        mov    esi, dword ptr [ebp + 6ch]
        add    esi, ebx
        lea    edx, [eax + esi]
        mov    ebx, 1000h
        cmp    edx, ebx
        ja     SHORT loc_1_7164
        mov    dword ptr [ebp + 58h], eax
        sub    ebx, ebx
        mov    dword ptr [ebp + 5ch], ebx
        mov    bl, byte ptr [edi + 4dh]
        mov    dword ptr [ebp + 68h], ebx
        jmp    SHORT sub_1_7184
loc_1_7164:
        sub    ebx, esi
        and    esi, esi
        jne    SHORT loc_1_716C
        mov    ebx, esi
loc_1_716C:
        mov    dword ptr [ebp + 58h], ebx
        and    edx, 0fffh
        mov    dword ptr [ebp + 5ch], edx
        movzx  eax, byte ptr [edi + 4eh]
        add    dword ptr [ebp + 68h], eax
        not    eax
        and    dword ptr [ebp + 68h], eax
sub_1_7184:
        mov    ebx, dword ptr [ebp + 64h]
        call   sub_1_00B0
        jb     SHORT loc_1_71A3
        clc
        call   sub_1_7502
        jb     SHORT loc_1_71A2
        cmp    dword ptr [ebp + 68h], 0
        jne    SHORT sub_1_7184
        mov    ebx, dword ptr [ebp + 70h]
        add    dword ptr [ebp + 14h], ebx
loc_1_71A2:
        ret
loc_1_71A3:
        movzx  ecx, byte ptr [edi + 4dh]
        mov    eax, dword ptr [ebp + 68h]
        cmp    ecx, eax
        je     SHORT loc_1_71F6
        sub    eax, ecx
        mov    ebx, dword ptr [ebp + 64h]
        add    ebx, eax
        mov    edx, eax
        call   sub_1_00B0
        jb     SHORT loc_1_71F6
        push   dword ptr [ebp + 64h]
        push   dword ptr [ebp + 14h]
        add    dword ptr [ebp + 64h], edx
        mov    cl, byte ptr [edi + 4ch]
        shl    edx, cl
        mov    ecx, dword ptr [ebp + 58h]
        jecxz  loc_1_71D8
        lea    edx, [ecx + edx - 1000h]
loc_1_71D8:
        add    dword ptr [ebp + 14h], edx
        stc
        push   dword ptr [ebp + 14h]
        call   sub_1_7502
        lahf
        pop    ebx
        sub    ebx, dword ptr [ebp + 14h]
        sub    dword ptr [ebp + 70h], ebx
        sahf
        pop    dword ptr [ebp + 14h]
        pop    dword ptr [ebp + 64h]
        jae    SHORT loc_1_71A3
        ret
loc_1_71F6:
        test   byte ptr [ebp + 3ah], 2
        jne    NEAR PTR loc_1_746E
        mov    ecx, dword ptr [ebp + 68h]
        mov    ebx, dword ptr [ebp + 64h]
        cmp    dword ptr [edi + 8], ebx
        jbe    NEAR PTR loc_1_746E
        movzx  edx, byte ptr [edi + 4dh]
loc_1_7213:
        call   sub_1_00B0
        jae    SHORT loc_1_7222
        add    ebx, edx
        sub    ecx, edx
        jne    SHORT loc_1_7213
        jmp    SHORT loc_1_725B
loc_1_7222:
        sub    ecx, edx
        je     NEAR PTR sub_1_7184
        push   dword ptr [ebp + 5ch]
        push   dword ptr [ebp + 70h]
        push   ecx
        sub    dword ptr [ebp + 68h], ecx
        sub    eax, eax
        mov    dword ptr [ebp + 5ch], eax
        mov    dword ptr [ebp + 70h], eax
        mov    eax, dword ptr [ebp + 64h]
        add    eax, dword ptr [ebp + 68h]
        push   eax
        call   sub_1_7184
        pop    dword ptr [ebp + 64h]
        pop    dword ptr [ebp + 68h]
        pop    dword ptr [ebp + 70h]
        pop    dword ptr [ebp + 5ch]
        jae    NEAR PTR sub_1_7184
        ret
loc_1_725B:
        sub    esi, esi
        cmp    dword ptr [ebp + 5ch], esi
        je     SHORT loc_1_7273
        movzx  eax, byte ptr [edi + 4dh]
        mov    ebx, dword ptr [ebp + 68h]
        sub    ebx, eax
        add    ebx, dword ptr [ebp + 64h]
        call   sub_1_7608
loc_1_7273:
        push   esi
        sub    esi, esi
        cmp    dword ptr [ebp + 58h], esi
        je     SHORT loc_1_7283
        mov    ebx, dword ptr [ebp + 64h]
        call   sub_1_7608
loc_1_7283:
        push   esi
        call   sub_1_A3B9
        mov    edx, dword ptr [ebp + 64h]
        add    edx, dword ptr [edi + 10h]
        mov    dword ptr [ebx + 10h], edx
        mov    ecx, dword ptr [ebp + 68h]
        mov    dword ptr [ebx + 2ch], 0
        mov    dword ptr [ebx + 30h], 0
        db  0C7h, 043h, 00Ch, 04Fh, 015h, 000h, 000h
        or     dword ptr [ebx + 8], 2
        lea    esi, [ebx + 50h]
        mov    dword ptr [ebx + 1ch], esi
        mov    edx, dword ptr [ebp + 14h]
        mov    eax, dword ptr [ebp + 58h]
        test   eax, eax
        je     SHORT loc_1_72DF
        add    edx, eax
        movzx  eax, byte ptr [edi + 4dh]
        sub    ecx, eax
        cmp    dword ptr [esp], 0
        je     SHORT loc_1_72DF
        add    dword ptr [ebx + 18h], eax
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [esi + 4], eax
        add    esi, 8
        xor    eax, eax
loc_1_72DF:
        add    dword ptr [ebx + 10h], eax
        jecxz  loc_1_7311
        mov    dword ptr [esi], ecx
        mov    dword ptr [esi + 4], edx
        cmp    dword ptr [ebp + 5ch], 0
        je     SHORT loc_1_730E
        movzx  eax, byte ptr [edi + 4dh]
        sub    ecx, eax
        je     SHORT loc_1_72FC
        mov    dword ptr [esi], ecx
        add    esi, 8
loc_1_72FC:
        mov    edx, dword ptr [esp + 4]
        test   edx, edx
        je     SHORT loc_1_7311
        add    ecx, eax
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edx + 10h]
        mov    dword ptr [esi + 4], eax
loc_1_730E:
        add    esi, 8
loc_1_7311:
        mov    dword ptr [esi], 0
        add    dword ptr [ebx + 18h], ecx
        je     SHORT loc_1_736E
        mov    eax, dword ptr [ebx + 18h]
        add    eax, dword ptr [ebx + 10h]
        sub    eax, dword ptr [edi + 8]
        cmc
        sbb    edx, edx
        and    eax, edx
        sub    dword ptr [ebx + 18h], eax
        sub    dword ptr [esi - 8], eax
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        cld
        mov    dword ptr [edi + 78h], eax
        mov    eax, ebx
        push   ebx
        push   edi
        call   sub_1_A558
        db  0FFh, 005h, 08Ch, 0B6h, 000h, 000h
        call   sub_1_199E
        pop    edi
        pop    ebx
loc_1_7350:
        cli
        bt     dword ptr [ebx + 2ch], 0
        jb     SHORT loc_1_7362
        lea    eax, [ebx + 30h]
        call   sub_1_A220
        jmp    SHORT loc_1_7350
loc_1_7362:
        sti
        cmp    word ptr [ebx + 6], 10h
        jae    NEAR PTR loc_1_743D
loc_1_736E:
        call   sub_1_A4AE
        pop    esi
        test   esi, esi
        je     SHORT loc_1_7385
        mov    al, 0
        call   sub_1_0036
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
loc_1_7385:
        pop    esi
        test   esi, esi
        je     SHORT loc_1_7397
        mov    al, 0
        call   sub_1_0036
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
loc_1_7397:
        cmp    dword ptr [ebp + 58h], 0
        je     SHORT loc_1_73A9
        clc
        call   sub_1_7502
        jb     NEAR PTR loc_1_7434
loc_1_73A9:
        test   byte ptr [ebp + 3ah], 4
        je     SHORT loc_1_740C
        mov    ecx, dword ptr [ebp + 68h]
        jecxz  loc_1_740C
        cmp    dword ptr [ebp + 5ch], 0
        je     SHORT loc_1_73C2
        movzx  eax, byte ptr [edi + 4dh]
        sub    ecx, eax
        je     SHORT loc_1_740C
loc_1_73C2:
        push   ecx
        mov    ebx, dword ptr [ebp + 64h]
        call   sub_1_7608
        je     SHORT loc_1_73F6
        mov    edi, dword ptr [esi + 10h]
        mov    edx, esi
        mov    esi, dword ptr [ebp + 14h]
        mov    ecx, 400h
        rep movsd
        mov    byte ptr [edx + 25h], 0
        mov    byte ptr [edx + 24h], 0
        mov    esi, edx
        mov    edi, dword ptr [ebp + 1ch]
        mov    al, 0
        call   sub_1_0036
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
loc_1_73F6:
        movzx  eax, byte ptr [edi + 4dh]
        add    dword ptr [ebp + 64h], eax
        add    dword ptr [ebp + 14h], 1000h
        sub    dword ptr [ebp + 68h], eax
        pop    ecx
        sub    ecx, eax
        jne    SHORT loc_1_73C2
loc_1_740C:
        mov    ebx, dword ptr [ebp + 68h]
        add    dword ptr [ebp + 64h], ebx
        mov    cl, byte ptr [edi + 4ch]
        shl    ebx, cl
        add    dword ptr [ebp + 14h], ebx
        cmp    dword ptr [ebp + 5ch], 0
        je     SHORT loc_1_7434
        sub    dword ptr [ebp + 14h], 1000h
        movzx  ecx, byte ptr [edi + 4dh]
        sub    dword ptr [ebp + 64h], ecx
        stc
        call   sub_1_7502
loc_1_7434:
        lahf
        mov    ebx, dword ptr [ebp + 70h]
        add    dword ptr [ebp + 14h], ebx
        sahf
        ret
loc_1_743D:
        call   sub_1_A4AE
        pop    esi
        test   esi, esi
        je     SHORT loc_1_7458
        mov    byte ptr [esi + 24h], 0ffh
        mov    al, 0
        call   sub_1_0036
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
loc_1_7458:
        pop    esi
        test   esi, esi
        je     SHORT loc_1_746E
        mov    byte ptr [esi + 24h], 0ffh
        mov    al, 0
        call   sub_1_0036
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
loc_1_746E:
        cmp    dword ptr [ebp + 58h], 0
        je     SHORT loc_1_747B
        call   sub_1_7502
        jb     SHORT loc_1_7434
loc_1_747B:
        cmp    dword ptr [ebp + 68h], 0
        je     SHORT loc_1_7434
        call   sub_1_7502
        jb     SHORT loc_1_7434
        jmp    SHORT loc_1_747B
loc_1_748A:
        test   byte ptr [ebp + 3ah], 2
        jne    NEAR PTR loc_1_7113
        mov    esi, eax
        or     esi, edx
        jne    NEAR PTR loc_1_7113
        call   sub_1_74DA
        jb     SHORT loc_1_74BF
        mov    ecx, dword ptr [ebp + 68h]
        mov    ebx, edi
        mov    edi, dword ptr [ebp + 60h]
        mov    esi, dword ptr [ebp + 14h]
        call   sub_1_1458
        mov    byte ptr [ebp + 34h], 0
        mov    edi, dword ptr [ebp + 1ch]
        mov    edx, dword ptr [ebp + 5ch]
loc_1_74BF:
        mov    ecx, dword ptr [ebp + 68h]
        mov    ebx, dword ptr [ebp + 60h]
        mov    eax, dword ptr [ebp + 58h]
        jb     NEAR PTR loc_1_7113
        mov    eax, ecx
        mov    cl, byte ptr [edi + 4ch]
        shl    eax, cl
        add    dword ptr [ebp + 14h], eax
        clc
        ret
sub_1_74DA:
        mov    esi, ebx
        sub    ebx, dword ptr [edi + 0ch]
        movzx  eax, byte ptr [edi + 4eh]
        not    eax
        and    ebx, eax
        add    ebx, dword ptr [edi + 0ch]
        sub    esi, ebx
        add    ecx, esi
loc_1_74EE:
        call   sub_1_00B0
        jae    SHORT loc_1_7500
        movzx  eax, byte ptr [edi + 4dh]
        add    ebx, eax
        sub    ecx, eax
        ja     SHORT loc_1_74EE
        stc
loc_1_7500:
        cmc
        ret
sub_1_7502:
        movzx  ecx, byte ptr [edi + 4dh]
        jb     NEAR PTR loc_1_75B0
        cmp    dword ptr [ebp + 58h], 0
        jne    SHORT loc_1_7559
        cmp    ecx, dword ptr [ebp + 68h]
        jae    NEAR PTR loc_1_75B0
loc_1_751B:
        mov    ebx, dword ptr [ebp + 64h]
        push   ecx
        call   sub_1_00D0
        pop    edx
        jb     SHORT loc_1_7558
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        push   edi
        xchg   ebx, esi
        movzx  ecx, word ptr [edi + 20h]
        mov    edi, dword ptr [ebp + 14h]
        call   sub_1_823A
        mov    dword ptr [ebp + 14h], edi
        pop    edi
        mov    esi, ebx
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        jb     SHORT loc_1_7558
        mov    ecx, edx
        inc    dword ptr [ebp + 64h]
        loop   loc_1_751B
loc_1_7551:
        movzx  eax, byte ptr [edi + 4dh]
        sub    dword ptr [ebp + 68h], eax
loc_1_7558:
        ret
loc_1_7559:
        mov    edx, dword ptr [ebp + 6ch]
loc_1_755C:
        push   edx
        mov    ebx, dword ptr [ebp + 60h]
        inc    dword ptr [ebp + 60h]
        call   sub_1_00D0
        pop    edx
        jb     SHORT loc_1_7558
        movzx  ecx, word ptr [edi + 20h]
        sub    ecx, edx
        sub    dword ptr [ebp + 58h], ecx
        jae    SHORT loc_1_7580
        add    ecx, dword ptr [ebp + 58h]
        mov    dword ptr [ebp + 58h], 0
loc_1_7580:
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        push   esi
        push   edi
        lea    esi, [edx + ebx]
        mov    edi, dword ptr [ebp + 14h]
        call   sub_1_823A
        mov    dword ptr [ebp + 14h], edi
        pop    edi
        pop    esi
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        jb     SHORT loc_1_7558
        sub    edx, edx
        cmp    dword ptr [ebp + 58h], edx
        jne    SHORT loc_1_755C
        movzx  eax, byte ptr [edi + 4dh]
        add    dword ptr [ebp + 64h], eax
        jmp    SHORT loc_1_7551
loc_1_75B0:
        cmp    dword ptr [ebp + 5ch], 0
        je     NEAR PTR loc_1_751B
        mov    ebx, dword ptr [ebp + 64h]
        call   sub_1_00D0
        jb     SHORT loc_1_7558
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        xchg   ebx, esi
        mov    ecx, dword ptr [ebp + 5ch]
        cmp    cx, word ptr [edi + 20h]
        jb     SHORT loc_1_75D9
        mov    cx, word ptr [edi + 20h]
loc_1_75D9:
        sub    dword ptr [ebp + 5ch], ecx
        push   edi
        mov    edi, dword ptr [ebp + 14h]
        call   sub_1_823A
        mov    esi, ebx
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        jb     NEAR PTR loc_1_7558
        mov    dword ptr [ebp + 14h], edi
        pop    edi
        inc    dword ptr [ebp + 64h]
        cmp    dword ptr [ebp + 5ch], 0
        jne    SHORT loc_1_75B0
        movzx  eax, byte ptr [edi + 4dh]
        sub    dword ptr [ebp + 68h], eax
        ret
sub_1_7608:
        db  08Ah, 025h, 0F8h, 0B6h, 000h, 000h
        mov    al, 7
        call   sub_1_0064
        jae    SHORT loc_1_7620
        test   esi, esi
        je     SHORT loc_1_7626
        mov    byte ptr [esi + 28h], 1
        ret
loc_1_7620:
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
loc_1_7626:
        xor    esi, esi
        ret
loc_1_7629:
        ret
sub_1_762A:
        push   ebx
        call   sub_1_7F50
        jb     SHORT loc_1_7659
        mov    dl, byte ptr [ebp + 3bh]
        call   sub_1_7E5E
        jb     SHORT loc_1_7659
        mov    eax, dword ptr [esp]
        and    ax, word ptr [edi + 22h]
        jne    SHORT loc_1_7654
        test   byte ptr [ebp + 3ah], 1
        je     SHORT loc_1_7654
        call   sub_1_02F8
        jmp    SHORT loc_1_7659
        db  08Bh, 0C0h
loc_1_7654:
        call   sub_1_00D0
loc_1_7659:
        pop    edx
        jb     SHORT loc_1_7629
        mov    ecx, dword ptr [ebp + 44h]
        push   ebx
        xchg   esi, ebx
        movzx  eax, word ptr [edi + 22h]
        and    edx, eax
        add    esi, edx
        add    edx, ecx
        and    edx, eax
        mov    edi, esi
        mov    esi, ebx
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        mov    esi, dword ptr [ebp + 14h]
        call   sub_1_823A
        mov    dword ptr [ebp + 14h], esi
        mov    esi, ebx
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        pop    ebx
        mov    edi, dword ptr [ebp + 1ch]
        jb     SHORT loc_1_76B6
        push   edx
        call   sub_1_03EC
        pop    edx
        mov    edi, dword ptr [ebp + 1ch]
        jb     SHORT loc_1_76B6
        test   edx, edx
        jne    SHORT loc_1_76B5
        mov    ecx, dword ptr [ebp + 20h]
        mov    al, byte ptr [ebp + 3bh]
        inc    eax
        cmp    al, byte ptr [edi + 24h]
        jbe    SHORT loc_1_76B2
        inc    dword ptr [ebp + 40h]
        mov    al, 0
loc_1_76B2:
        mov    byte ptr [ebp + 3bh], al
loc_1_76B5:
        clc
loc_1_76B6:
        ret
sub_1_76B7:
        mov    eax, dword ptr [esi + 8]
        mov    edx, dword ptr [eax + 54h]
        sub    edx, 8
        mov    ebx, dword ptr [ebp + 40h]
loc_1_76C3:
        add    edx, 8
        sub    ebx, dword ptr [edx]
        jae    SHORT loc_1_76C3
        mov    eax, ebx
        add    ebx, dword ptr [edx]
        neg    eax
        add    ebx, dword ptr [edx + 4]
        mov    edx, ecx
        mov    cl, byte ptr [edi + 25h]
        shl    eax, cl
        mov    ecx, edx
        movzx  edx, byte ptr [ebp + 3bh]
        sub    eax, edx
        cmp    eax, ecx
        jae    SHORT loc_1_76E8
        mov    ecx, eax
loc_1_76E8:
        push   ecx
        push   edx
        add    edx, ecx
        mov    eax, edx
        and    dl, byte ptr [edi + 24h]
        mov    cl, byte ptr [edi + 25h]
        shr    eax, cl
        mov    byte ptr [ebp + 3bh], dl
        add    dword ptr [ebp + 40h], eax
        pop    edx
        call   sub_1_7E5E
        pop    ecx
        ret
sub_1_7704:
        movzx  eax, word ptr [ebp + 3ch]
        movzx  edx, word ptr [edi + 20h]
        and    ax, word ptr [edi + 22h]
        mov    ebx, dword ptr [ebp]
        je     SHORT loc_1_7732
        sub    eax, edx
        neg    eax
        sub    ebx, eax
        jae    SHORT loc_1_7721
        add    eax, ebx
        xor    ebx, ebx
loc_1_7721:
        mov    dword ptr [ebp + 44h], eax
        xchg   ebx, eax
        mov    ebx, edx
        xor    edx, edx
        div    ebx
        mov    dword ptr [ebp + 48h], eax
        mov    dword ptr [ebp + 4ch], edx
        ret
loc_1_7732:
        test   byte ptr [ebp + 37h], 1
        je     SHORT loc_1_7721
        cmp    dword ptr [ebp], 1000h
        jne    SHORT loc_1_7721
        test   dword ptr [ebp + 3ch], 0fffh
        jne    SHORT loc_1_7721
        sub    eax, eax
        jmp    SHORT loc_1_7721
sub_1_774E:
        mov    eax, dword ptr [ebp + 3ch]
        mov    cl, byte ptr [edi + 4ch]
        shr    eax, cl
        mov    ebx, eax
        and    bl, byte ptr [edi + 24h]
        mov    byte ptr [ebp + 3bh], bl
        mov    cl, byte ptr [edi + 25h]
        shr    eax, cl
        mov    dword ptr [ebp + 40h], eax
        mov    eax, dword ptr [ebp + 3ch]
        add    eax, dword ptr [ebp]
        movzx  ecx, word ptr [edi + 4ah]
        add    eax, ecx
        mov    cl, byte ptr [edi + 47h]
        shr    eax, cl
        mov    edx, dword ptr [esi + 8]
        je     SHORT loc_1_777D
        dec    eax
loc_1_777D:
        cmp    dword ptr [edx + 50h], eax
        ja     SHORT loc_1_7787
        call   sub_1_2165
loc_1_7787:
        ret
sub_1_7788:
        or     eax, 0ffffffffh
        cmp    dword ptr [edi + 9ch], 0
        je     SHORT loc_1_77BD
loc_1_7794:
        mov    ecx, ebx
        mov    eax, dword ptr [edi + 9ch]
        shr    ecx, 0ah
        movsx  eax, word ptr [eax + ecx*2]
        or     eax, eax
        jg     SHORT loc_1_77BD
        mov    eax, dword ptr [edi + 9ch]
        jl     SHORT loc_1_77BE
loc_1_77AF:
        inc    ecx
        cmp    word ptr [eax + ecx*2], 0
        je     SHORT loc_1_77AF
        mov    ebx, ecx
        shl    ebx, 0ah
        stc
loc_1_77BD:
        ret
loc_1_77BE:
        push   edx
        push   ebx
        push   dword ptr [edi + 0a8h]
        xor    edx, edx
        lea    eax, [eax + ecx*2]
        mov    ebx, ecx
        shl    ebx, 0ah
        mov    ecx, dword ptr [edi + 2ch]
        dec    ebx
        sub    ecx, ebx
        cmp    ecx, 400h
        jb     SHORT loc_1_77E3
        mov    ecx, 400h
loc_1_77E3:
        inc    ebx
        push   eax
        push   ebx
        push   ecx
        push   edx
        push   esi
        call   sub_1_1F2C
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        jb     SHORT loc_1_780A
        jne    SHORT loc_1_77F8
        inc    edx
loc_1_77F8:
        loop   loc_1_77E3
loc_1_77FA:
        pop    ecx
        pop    ebx
        cmp    dword ptr [edi + 0a8h], ecx
        jne    SHORT loc_1_7807
        mov    word ptr [eax], dx
loc_1_7807:
        pop    edx
        jmp    SHORT loc_1_7794
loc_1_780A:
        cmp    ebx, 2
        jb     SHORT loc_1_77F8
        mov    edx, 401h
        jmp    SHORT loc_1_77FA
sub_1_7816:
        or     esi, esi
        je     SHORT loc_1_783C
        and    byte ptr [esi + 24h], 0bfh
        movzx  eax, word ptr [edi + 4ah]
        add    eax, dword ptr [esi + 30h]
        rcr    eax, 1
        push   ecx
        mov    cl, byte ptr [edi + 47h]
        dec    cl
        shr    eax, cl
        pop    ecx
        cmp    dword ptr [esi + 50h], eax
        je     SHORT loc_1_783C
        or     byte ptr [esi + 24h], 40h
        mov    eax, dword ptr [esi + 50h]
loc_1_783C:
        push   eax
        push   ebx
        push   ecx
        push   edi
        test   byte ptr [edi + 53h], 40h
        je     SHORT loc_1_7860
        db  083h, 03Dh, 08Ah, 0B7h, 000h, 000h, 000h
        je     SHORT loc_1_7860
        push   5
        db  0FFh, 035h, 08Ah, 0B7h, 000h, 000h
        db  0CDh, 020h, 02Eh, 001h, 001h, 000h
        add    esp, 8
loc_1_7860:
        pop    edi
        pop    ecx
        pop    ebx
        pop    edx
loc_1_7864:
        bts    dword ptr [edi + 94h], 0
        jae    SHORT loc_1_787B
        lea    eax, [edi + 98h]
        call   sub_1_A220
        jmp    SHORT loc_1_7864
loc_1_787B:
        sub    esp, 28h
        mov    eax, 27h
        cmp    dword ptr [edi + 2ch], ecx
        jbe    NEAR PTR loc_1_7A4E
        cmp    dword ptr [edi + 40h], ecx
        jb     NEAR PTR loc_1_79CB
        mov    dword ptr [esp + 4], esi
        mov    dword ptr [esp], edx
        test   byte ptr [edi + 52h], 10h
        je     SHORT loc_1_78C5
        test   byte ptr [ebp + 37h], 10h
        jne    SHORT loc_1_78BC
        call   sub_1_5B05
        cmp    eax, 1
        jae    SHORT loc_1_78C5
        mov    eax, 27h
        jmp    NEAR PTR loc_1_79CB
loc_1_78BC:
        push   ecx
        mov    eax, ecx
        call   sub_1_5B8D
        pop    ecx
loc_1_78C5:
        mov    dword ptr [esp + 20h], ebx
        mov    dword ptr [esp + 18h], ecx
        mov    dword ptr [esp + 10h], ecx
        mov    eax, dword ptr [edi + 0a8h]
        mov    dword ptr [esp + 8], eax
        mov    dword ptr [esp + 14h], ebx
        and    dword ptr [esp + 24h], 0
        test   byte ptr [ebp + 37h], 10h
        jne    NEAR PTR loc_1_7BBD
        mov    eax, ecx
        mov    edx, ecx
        mov    cl, byte ptr [edi + 47h]
        shl    eax, cl
        mov    ecx, edx
        db  039h, 005h, 088h, 0B8h, 000h, 000h
        jbe    NEAR PTR loc_1_7BBD
loc_1_7905:
        and    ebx, ebx
        je     NEAR PTR loc_1_7A54
        call   sub_1_2102
        jae    NEAR PTR loc_1_7A49
loc_1_7918:
        inc    ebx
        cmp    dword ptr [edi + 2ch], ebx
        jb     NEAR PTR loc_1_7A54
        push   ebx
        call   sub_1_1F2C
        pop    edx
        jb     NEAR PTR loc_1_7A49
        jne    NEAR PTR loc_1_7A54
loc_1_7935:
        cmp    dword ptr [esp + 24h], 0
        jne    SHORT loc_1_7940
        mov    dword ptr [esp + 24h], edx
loc_1_7940:
        mov    ebx, dword ptr [esp + 14h]
        and    ebx, ebx
        je     SHORT loc_1_7955
        push   edx
        call   sub_1_1D4C
        pop    edx
        jb     NEAR PTR loc_1_7A49
loc_1_7955:
        mov    ebx, edx
        dec    edx
        cmp    dword ptr [edi + 38h], edx
        jne    SHORT loc_1_7960
        mov    dword ptr [edi + 38h], ebx
loc_1_7960:
        mov    eax, dword ptr [edi + 38h]
        cmp    dword ptr [edi + 2ch], eax
        ja     SHORT loc_1_796F
        mov    dword ptr [edi + 38h], 2
loc_1_796F:
        or     edx, 0ffffffffh
        push   ebx
        call   sub_1_1D4C
        pop    ebx
        jb     NEAR PTR loc_1_7A49
        inc    dword ptr [edi + 0a8h]
        inc    dword ptr [esp + 8]
        mov    esi, dword ptr [esp + 4]
        call   sub_1_7C8D
        mov    ecx, dword ptr [edi + 9ch]
        jecxz  loc_1_79A9
        mov    eax, ebx
        shr    eax, 0ah
        cmp    word ptr [ecx + eax*2], -1
        sbb    word ptr [ecx + eax*2], 0
loc_1_79A9:
        test   byte ptr [ebp + 37h], 10h
        jne    SHORT loc_1_7A25
loc_1_79AF:
        mov    dword ptr [esp + 14h], ebx
        dec    dword ptr [esp + 10h]
        jne    NEAR PTR loc_1_7918
        cmp    dword ptr [edi + 40h], -1
        je     SHORT loc_1_79CB
        mov    eax, dword ptr [esp + 18h]
        sub    dword ptr [edi + 40h], eax
        clc
loc_1_79CB:
        pushfd
        push   eax
        mov    eax, dword ptr [edi + 60h]
        or     eax, eax
        je     SHORT loc_1_79E1
        mov    edx, dword ptr [edi + 38h]
        test   byte ptr [edi + 52h], 3
        je     SHORT loc_1_79E1
        mov    word ptr [eax + 1dh], dx
loc_1_79E1:
        and    dword ptr [edi + 94h], 0
        cmp    dword ptr [edi + 98h], 0
        je     SHORT loc_1_79FC
        lea    eax, [edi + 98h]
        call   sub_1_A258
loc_1_79FC:
        test   byte ptr [edi + 53h], 40h
        je     SHORT loc_1_7A1A
        db  083h, 03Dh, 08Ah, 0B7h, 000h, 000h, 000h
        je     SHORT loc_1_7A1A
        db  0FFh, 035h, 08Ah, 0B7h, 000h, 000h
        db  0CDh, 020h, 02Fh, 001h, 001h, 000h
        add    esp, 4
loc_1_7A1A:
        pop    eax
        popfd
        mov    ebx, dword ptr [esp + 24h]
        lea    esp, [esp + 28h]
        ret
loc_1_7A25:
        test   byte ptr [edi + 52h], 10h
        je     SHORT loc_1_79AF
        pushad
        mov    edx, ebx
        mov    ebx, edi
        mov    cx, 0db0ah
        call   sub_1_449E
        popad
        jae    NEAR PTR loc_1_79AF
        dec    dword ptr [esp + 10h]
        jmp    NEAR PTR loc_1_7B55
loc_1_7A49:
        mov    eax, 5
loc_1_7A4E:
        stc
        jmp    NEAR PTR loc_1_79CB
loc_1_7A54:
        mov    ebx, dword ptr [edi + 38h]
        mov    esi, dword ptr [esp + 4]
        call   sub_1_7CFF
        jae    SHORT loc_1_7A7A
        mov    eax, 1
        mov    cl, 11h
        sub    cl, byte ptr [edi + 47h]
        shl    eax, cl
        add    ebx, eax
        cmp    dword ptr [edi + 2ch], ebx
        jae    SHORT loc_1_7A7A
        mov    ebx, 2
loc_1_7A7A:
        test   byte ptr [edi + 52h], 2
        jne    SHORT loc_1_7AB4
loc_1_7A80:
        call   sub_1_7788
        jb     SHORT loc_1_7A95
        push   ebx
        call   sub_1_1F2C
        pop    edx
        jb     SHORT loc_1_7A49
        je     SHORT loc_1_7AAC
        lea    ebx, [edx + 1]
loc_1_7A95:
        cmp    dword ptr [edi + 2ch], ebx
        jae    SHORT loc_1_7A80
        mov    ebx, 2
        cmp    dword ptr [edi + 38h], ebx
        mov    dword ptr [edi + 38h], ebx
        jne    SHORT loc_1_7A80
        jmp    NEAR PTR loc_1_7B33
loc_1_7AAC:
        mov    dword ptr [edi + 38h], edx
        jmp    NEAR PTR loc_1_7935
loc_1_7AB4:
        mov    eax, dword ptr [edi + 2ch]
        movzx  ecx, word ptr [edi + 22h]
        lea    eax, [ecx + eax*2 + 2]
        mov    cl, byte ptr [edi + 4ch]
        shr    eax, cl
        inc    eax
        mov    dword ptr [esp + 0ch], eax
loc_1_7AC9:
        dec    dword ptr [esp + 0ch]
        js     SHORT loc_1_7B33
        mov    dword ptr [esp + 1ch], ebx
        add    ebx, ebx
        mov    eax, ebx
        mov    cl, byte ptr [edi + 4ch]
        shr    ebx, cl
        movzx  ecx, word ptr [edi + 26h]
        cmp    byte ptr [edi + 48h], 0
        je     SHORT loc_1_7AE9
        add    ecx, dword ptr [edi + 30h]
loc_1_7AE9:
        add    ebx, ecx
        push   eax
        call   sub_1_01D0
        pop    eax
        jb     NEAR PTR loc_1_7A49
        movzx  ecx, word ptr [edi + 22h]
        and    eax, ecx
        lea    esi, [eax + ebx]
        inc    ecx
        sub    ecx, eax
        shr    ecx, 1
        push   edi
        mov    edi, esi
        sub    eax, eax
        mov    edx, ecx
        repne scasw
        pop    edi
        jne    SHORT loc_1_7B26
        sub    edx, ecx
        dec    edx
        add    edx, dword ptr [esp + 1ch]
        cmp    dword ptr [edi + 2ch], edx
        jae    SHORT loc_1_7AAC
loc_1_7B1F:
        mov    ebx, 2
        jmp    SHORT loc_1_7AC9
loc_1_7B26:
        mov    ebx, dword ptr [esp + 1ch]
        add    ebx, edx
        cmp    dword ptr [edi + 2ch], ebx
        jae    SHORT loc_1_7AC9
        jmp    SHORT loc_1_7B1F
loc_1_7B33:
        mov    eax, dword ptr [esp + 8]
        cmp    dword ptr [edi + 0a8h], eax
        jne    SHORT loc_1_7B55
        cmp    dword ptr [edi + 40h], -1
        jne    SHORT loc_1_7B55
        call   sub_1_5BD3
        mov    ebx, dword ptr [esp + 18h]
        sub    ebx, dword ptr [esp + 10h]
        mov    dword ptr [edi + 40h], ebx
loc_1_7B55:
        mov    ebx, dword ptr [esp + 18h]
        mov    ecx, dword ptr [esp + 20h]
        mov    edx, dword ptr [esp + 24h]
        sub    ebx, dword ptr [esp + 10h]
        je     SHORT loc_1_7B9A
        mov    esi, dword ptr [esp + 4]
        mov    eax, dword ptr [esp]
        call   sub_1_7CC8
        add    dword ptr [esp + 8], ebx
        push   dword ptr [edi + 40h]
        or     dword ptr [edi + 40h], 0ffffffffh
        jecxz  loc_1_7B8E
        mov    ebx, ecx
        mov    edx, 0ffffffffh
        call   sub_1_7D52
        jmp    SHORT loc_1_7B95
loc_1_7B8E:
        mov    ebx, edx
        call   sub_1_7D50
loc_1_7B95:
        pop    dword ptr [edi + 40h]
        jb     SHORT loc_1_7BB3
loc_1_7B9A:
        mov    eax, dword ptr [edi + 0a8h]
        mov    ebx, dword ptr [esp + 20h]
        mov    ecx, dword ptr [esp + 18h]
        cmp    dword ptr [esp + 8], eax
        jne    NEAR PTR loc_1_78C5
        stc
loc_1_7BB3:
        mov    eax, 27h
        jmp    NEAR PTR loc_1_79CB
loc_1_7BBD:
        or     ebx, ebx
        jne    SHORT loc_1_7BC4
        mov    ebx, dword ptr [edi + 38h]
loc_1_7BC4:
        inc    ebx
        call   sub_1_7BE5
        mov    edx, ebx
        jae    NEAR PTR loc_1_7AAC
        cmp    byte ptr [ebp + 34h], 10h
        jae    NEAR PTR loc_1_7A49
        mov    ebx, dword ptr [esp + 20h]
        jmp    NEAR PTR loc_1_7905
sub_1_7BE5:
        sub    esp, 14h
        mov    dword ptr [esp + 10h], ecx
        mov    eax, dword ptr [edi + 2ch]
        mov    dword ptr [esp + 4], eax
loc_1_7BF3:
        lea    esi, [ebx - 1]
        lea    ebx, [ecx + esi]
        cmp    eax, ebx
        jae    SHORT loc_1_7C04
        mov    ebx, 2
        jmp    SHORT loc_1_7BF3
loc_1_7C04:
        lea    eax, [ebx - 1]
        mov    dword ptr [esp], eax
        mov    dword ptr [esp + 0ch], ebx
        and    dword ptr [esp + 8], 0
        jmp    SHORT loc_1_7C38
loc_1_7C15:
        cmp    eax, 400h
        je     SHORT loc_1_7C79
loc_1_7C1C:
        push   ebx
        push   esi
        call   sub_1_1F2C
        pop    esi
        pop    ebx
        jb     SHORT loc_1_7C88
        jne    SHORT loc_1_7C40
loc_1_7C29:
        mov    ecx, ebx
        dec    ebx
        cmp    ebx, esi
        jbe    SHORT loc_1_7C81
        test   ecx, 3ffh
        jne    SHORT loc_1_7C1C
loc_1_7C38:
        call   sub_1_7788
        jae    SHORT loc_1_7C15
        dec    ebx
loc_1_7C40:
        mov    eax, dword ptr [esp + 0ch]
        sub    eax, ebx
        cmc
        sbb    edx, edx
        and    eax, edx
        mov    dword ptr [esp + 8], eax
        lea    esi, [ebx + eax]
        add    ebx, dword ptr [esp + 10h]
        mov    dword ptr [esp + 0ch], ebx
        cmp    dword ptr [esp + 4], ebx
        jae    SHORT loc_1_7C38
        xor    eax, eax
        xchg   dword ptr [esp], eax
        mov    dword ptr [esp + 4], eax
        mov    ebx, 1
        mov    dword ptr [esp + 0ch], ebx
        or     eax, eax
        jne    SHORT loc_1_7C40
        stc
        jmp    SHORT loc_1_7C88
loc_1_7C79:
        and    ebx, 0fffffc00h
        jmp    SHORT loc_1_7C29
loc_1_7C81:
        lea    ebx, [esi + 1]
        sub    ebx, dword ptr [esp + 8]
loc_1_7C88:
        lea    esp, [esp + 14h]
        ret
sub_1_7C8D:
        or     esi, esi
        je     SHORT loc_1_7CAE
        test   byte ptr [esi + 24h], 40h
        jne    SHORT loc_1_7CAE
        mov    edx, dword ptr [esi + 5ch]
        cmp    dword ptr [esi + 50h], 0
        je     SHORT loc_1_7CAF
        mov    ecx, dword ptr [edx]
        add    ecx, dword ptr [edx + 4]
        cmp    ecx, ebx
        jne    SHORT loc_1_7CAF
loc_1_7CA9:
        inc    dword ptr [edx]
        inc    dword ptr [esi + 50h]
loc_1_7CAE:
        ret
loc_1_7CAF:
        add    edx, 8
        cmp    dword ptr [esi + 58h], edx
        jbe    SHORT loc_1_7CC2
        mov    dword ptr [esi + 5ch], edx
        mov    dword ptr [edx + 4], ebx
        and    dword ptr [edx], 0
        jmp    SHORT loc_1_7CA9
loc_1_7CC2:
        or     byte ptr [esi + 24h], 40h
        jmp    SHORT loc_1_7CAE
sub_1_7CC8:
        or     esi, esi
        je     SHORT loc_1_7CF0
        cmp    dword ptr [esi + 50h], eax
        je     SHORT loc_1_7CF0
        or     eax, eax
        je     SHORT loc_1_7CF1
        push   ebx
        push   ecx
        push   eax
        mov    ecx, esi
        lea    ebx, [eax - 1]
        call   sub_1_2276
        mov    dword ptr [esi + 5ch], eax
        sub    ebx, dword ptr [eax + 4]
        inc    ebx
        mov    dword ptr [eax], ebx
        pop    dword ptr [esi + 50h]
        pop    ecx
        pop    ebx
loc_1_7CF0:
        ret
loc_1_7CF1:
        mov    dword ptr [esi + 50h], eax
        mov    eax, dword ptr [esi + 54h]
        sub    eax, 8
        mov    dword ptr [esi + 5ch], eax
        jmp    SHORT loc_1_7CF0
sub_1_7CFF:
        db  0BAh, 020h, 0B7h, 000h, 000h
loc_1_7D04:
        mov    edx, dword ptr [edx + 8]
        db  081h, 0FAh, 020h, 0B7h, 000h, 000h
        je     SHORT loc_1_7D4F
        cmp    word ptr [edx + 2eh], 0
        je     SHORT loc_1_7D04
        cmp    edx, esi
        je     SHORT loc_1_7D04
        cmp    dword ptr [edx + 28h], edi
        jne    SHORT loc_1_7D04
        test   byte ptr [edx + 23h], 81h
        jne    SHORT loc_1_7D04
        cmp    dword ptr [edx + 50h], 0
        je     SHORT loc_1_7D04
        bt     dword ptr [edx + 24h], 4
        jb     SHORT loc_1_7D04
        inc    word ptr [edx + 20h]
        mov    ecx, dword ptr [edx + 5ch]
        mov    eax, dword ptr [ecx + 4]
        add    eax, dword ptr [ecx]
        push   eax
        push   edi
        mov    edi, edx
        call   sub_1_A040
        pop    edi
        pop    eax
        dec    eax
        cmp    eax, ebx
        jne    SHORT loc_1_7D04
        stc
loc_1_7D4F:
        ret
sub_1_7D50:
        xor    edx, edx
sub_1_7D52:
        cmp    ebx, 2
        jb     NEAR PTR loc_1_7E5A
        cmp    dword ptr [edi + 2ch], ebx
        jb     NEAR PTR loc_1_7E5A
        or     edx, edx
        jne    NEAR PTR loc_1_7E03
        push   ebx
        call   sub_1_7E5E
        jb     NEAR PTR loc_1_7E00
        mov    esi, ebx
        sub    ebx, dword ptr [edi + 0ch]
        mov    al, byte ptr [edi + 4eh]
        not    al
        and    bl, al
        add    ebx, dword ptr [edi + 0ch]
        mov    cl, byte ptr [edi + 24h]
        mov    dl, 2
        shl    dl, cl
        dec    dl
        mov    ecx, esi
        sub    ecx, ebx
        shl    dl, cl
        movzx  ecx, word ptr [edi + 4ah]
        add    ecx, 1000h
        shr    ecx, 0ch
loc_1_7DA3:
        call   sub_1_00B0
        jb     SHORT loc_1_7DF8
        cli
        db  0CDh, 020h, 00Dh, 000h, 08Bh, 004h
        or     eax, eax
        jne    SHORT loc_1_7DF7
        mov    al, byte ptr [esi + 2bh]
        test   al, 10h
        jne    SHORT loc_1_7DF7
        push   ecx
        push   edx
        not    dl
        and    byte ptr [esi + 2ah], dl
        and    byte ptr [esi + 32h], dl
        jne    SHORT loc_1_7DDD
        test   al, 8
        je     SHORT loc_1_7DDD
        push   ebx
        mov    ebx, esi
        cli
        call   sub_1_893E
        mov    esi, ebx
        pop    ebx
        call   sub_1_899F
loc_1_7DDD:
        mov    edx, dword ptr [esp]
        or     dl, byte ptr [esi + 24h]
        mov    byte ptr [esi + 24h], dl
        inc    dl
        jne    SHORT loc_1_7DF5
        call   0
        db  0CDh, 020h, 005h, 000h, 08Bh, 004h
loc_1_7DF5:
        pop    edx
        pop    ecx
loc_1_7DF7:
        sti
loc_1_7DF8:
        movzx  eax, byte ptr [edi + 4dh]
        add    ebx, eax
        loop   loc_1_7DA3
loc_1_7E00:
        pop    ebx
        sub    edx, edx
loc_1_7E03:
        push   ebx
        push   edx
        call   sub_1_1F2C
        mov    eax, ebx
        pop    edx
        pop    ebx
        jb     SHORT loc_1_7E5C
        push   eax
        push   edx
        push   ebx
        call   sub_1_1D4C
        pop    eax
        pop    edx
        pop    ebx
        jb     SHORT loc_1_7E5C
        and    ebx, ebx
        je     SHORT loc_1_7E5A
        or     edx, edx
        jne    SHORT loc_1_7E4F
        inc    dword ptr [edi + 0a8h]
        cmp    dword ptr [edi + 9ch], 0
        je     SHORT loc_1_7E47
        shr    eax, 0ah
        add    eax, eax
        add    eax, dword ptr [edi + 9ch]
        cmp    word ptr [eax], -1
        adc    word ptr [eax], 0
loc_1_7E47:
        cmp    dword ptr [edi + 40h], -1
        adc    dword ptr [edi + 40h], 0
loc_1_7E4F:
        call   sub_1_2102
        jb     NEAR PTR sub_1_7D50
loc_1_7E5A:
        clc
        ret
loc_1_7E5C:
        stc
        ret
sub_1_7E5E:
        cmp    dword ptr [edi + 2ch], ebx
        jb     SHORT loc_1_7E77
        sub    ebx, 2
        jb     SHORT loc_1_7E77
        mov    cl, byte ptr [edi + 25h]
        mov    al, dl
        shl    ebx, cl
        or     bl, al
        mov    ecx, dword ptr [edi + 28h]
        add    ebx, ecx
        ret
loc_1_7E77:
        stc
        ret
sub_1_7E79:
        mov    edi, dword ptr [ebp + 20h]
        cmp    dword ptr [edi + 8], 0
        jne    SHORT loc_1_7E9F
        push   ebx
        sub    eax, eax
        call   sub_1_7FED
        pop    ebx
        jb     SHORT loc_1_7EE9
        mov    edi, dword ptr [ebp + 20h]
        mov    dword ptr [edi + 8], eax
        mov    esi, dword ptr [eax]
        mov    dword ptr [edi], esi
        mov    dword ptr [edi + 4], eax
        mov    dword ptr [esi + 4], edi
        mov    dword ptr [eax], edi
loc_1_7E9F:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [edi + 14h], eax
        movzx  eax, byte ptr [ebp + 5]
        mov    dword ptr [edi + 10h], eax
        mov    word ptr [edi + 0eh], 24ch
        mov    dword ptr [edi + 20h], 1
        sub    eax, eax
        mov    dword ptr [edi + 18h], eax
        mov    dword ptr [edi + 24h], eax
        mov    dword ptr [edi + 28h], eax
        mov    eax, dword ptr [ebp + 1ch]
        test   byte ptr [eax + 53h], 1
        je     SHORT loc_1_7ED2
        or     word ptr [edi + 0eh], 20h
loc_1_7ED2:
        test   byte ptr [eax + 53h], 4
        je     SHORT loc_1_7EDD
        or     word ptr [edi + 0eh], 2
loc_1_7EDD:
        xor    eax, eax
        mov    dword ptr [ebp + 3ch], eax
        mov    dword ptr [ebp + 40h], eax
        mov    byte ptr [ebp + 3bh], al
        clc
loc_1_7EE9:
        ret
sub_1_7EEA:
        push   ecx
        push   esi
        push   edi
        mov    edi, edx
        call   sub_1_9FEC
        or     byte ptr [edi + 23h], 1
        mov    ecx, dword ptr [edi + 58h]
        sub    ecx, dword ptr [edi + 54h]
        cmp    ecx, 800h
        jb     SHORT loc_1_7F34
        mov    ecx, dword ptr [edi + 5ch]
        add    ecx, 8
        sub    ecx, dword ptr [edi + 54h]
        push   ecx
        mov    eax, ecx
        call   sub_1_A1CE
        pop    ecx
        jb     SHORT loc_1_7F38
        shr    ecx, 2
        mov    edi, eax
        xchg   dword ptr [edx + 54h], eax
        mov    esi, eax
        rep movsd
        call   sub_1_A1F8
        mov    dword ptr [edx + 58h], edi
        sub    edi, 8
        mov    dword ptr [edx + 5ch], edi
loc_1_7F34:
        or     byte ptr [edx + 24h], 2
loc_1_7F38:
        pushfd
        mov    edi, edx
        call   sub_1_A01A
        popfd
        pop    edi
        pop    esi
        pop    ecx
        ret
loc_1_7F45:
        push   eax
        mov    eax, edx
        call   sub_1_2165
        pop    eax
        jb     SHORT loc_1_7F64
sub_1_7F50:
        mov    edx, dword ptr [ebp + 40h]
        mov    ecx, dword ptr [esi + 8]
        cmp    edx, dword ptr [ecx + 50h]
        jae    SHORT loc_1_7F45
        mov    ebx, edx
        push   eax
        call   sub_1_2276
        pop    eax
loc_1_7F64:
        ret
loc_1_7F65:
        mov    ecx, dword ptr [esp + 8]
        mov    edx, dword ptr [ecx + 8]
        cmp    edx, dword ptr [ecx + 4]
        jne    SHORT loc_1_7F79
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx + 8], eax
        xor    eax, eax
        ret
loc_1_7F79:
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx + 8], eax
        mov    ecx, dword ptr [esp + 4]
        mov    al, byte ptr [edx + 0ch]
        mov    ah, al
        and    ax, 807fh
        mov    word ptr [ecx + 0ch], ax
        movzx  eax, word ptr [edx + 16h]
        mov    dword ptr [ecx], eax
        mov    eax, 1
        ret
sub_1_7F9B:
        mov    ecx, ebx
        sub    ecx, dword ptr [esi + 10h]
        shr    ecx, 5
        mov    dl, cl
        mov    edi, dword ptr [esi + 0ch]
        mov    eax, ecx
        mov    cl, byte ptr [edi + 4ch]
        sub    cl, 5
        shr    eax, cl
        mov    ecx, eax
        add    ecx, dword ptr [esi + 8]
        db  0B8h, 020h, 0B7h, 000h, 000h
loc_1_7FBC:
        mov    eax, dword ptr [eax + 8]
        db  03Dh, 020h, 0B7h, 000h, 000h
        je     SHORT loc_1_7FEB
        cmp    dl, byte ptr [eax + 25h]
        jne    SHORT loc_1_7FBC
        cmp    ecx, dword ptr [eax + 1ch]
        jne    SHORT loc_1_7FBC
        cmp    edi, dword ptr [eax + 28h]
        jne    SHORT loc_1_7FBC
        push   ecx
        push   edi
        push   esi
        lea    esi, [ebx]
        lea    edi, [eax + 45h]
        mov    ecx, 0bh
        repe cmpsb
        pop    esi
        pop    edi
        pop    ecx
        jne    SHORT loc_1_7FBC
        clc
        ret
loc_1_7FEB:
        stc
        ret
sub_1_7FED:
        push   eax
loc_1_7FEE:
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        call   sub_1_7F9B
        jb     NEAR PTR loc_1_80C9
        xchg   edi, eax
        mov    ecx, dword ptr [esp]
        test   ecx, 1
        jne    SHORT loc_1_8052
        and    ecx, 2
        sub    esp, 10h
        mov    ebx, esp
        mov    al, byte ptr [ebp + 62h]
        mov    ah, al
        and    ax, 807fh
        mov    word ptr [ebx + 0ch], ax
        movzx  ax, byte ptr [edi + 43h]
        mov    word ptr [ebx + 0eh], ax
        movzx  eax, word ptr [ebp + 0ah]
        mov    dword ptr [ebx], eax
        mov    dword ptr [ebx + 4], edi
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebx + 8], eax
        push   ebx
        db  068h, 065h, 07Fh, 000h, 000h
        push   ebx
        push   ecx
        db  0CDh, 020h, 052h, 000h, 040h, 000h
        add    esp, 10h
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_8082
loc_1_8052:
        mov    bl, byte ptr [ebp + 62h]
        and    ebx, 7
        db  08Bh, 00Ch, 09Dh, 0D0h, 0B7h, 000h, 000h
        add    dword ptr [edi + 2ch], ecx
loc_1_8062:
        test   byte ptr [edi + 23h], 81h
        jne    SHORT loc_1_807B
        inc    dword ptr [edi + 20h]
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        mov    eax, edi
        add    esp, 4
        clc
        ret
loc_1_807B:
        call   sub_1_A02F
        jmp    SHORT loc_1_8062
loc_1_8082:
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        add    esp, 4
        stc
        ret
loc_1_8090:
        db  0B8h, 0F4h, 0B7h, 000h, 000h
        call   sub_1_A220
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        jmp    NEAR PTR loc_1_7FEE
loc_1_80A8:
        db  0C6h, 005h, 0F0h, 0B7h, 000h, 000h, 000h
        db  0B8h, 0F4h, 0B7h, 000h, 000h
        call   sub_1_A258
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        mov    al, 8
        add    esp, 4
        stc
        ret
loc_1_80C9:
        db  00Fh, 0BAh, 02Dh, 0F0h, 0B7h, 000h, 000h, 000h
        jb     SHORT loc_1_8090
        mov    eax, 68h
        push   ecx
        call   sub_1_A1CE
        pop    ecx
        jb     SHORT loc_1_80A8
        push   edx
        mov    edx, dword ptr [ebp + 50h]
        mov    dword ptr [eax + 18h], edx
        mov    edx, dword ptr [ebp + 4ch]
        mov    dword ptr [eax + 14h], edx
        pop    edx
        mov    byte ptr [eax + 25h], dl
        mov    dword ptr [eax + 1ch], ecx
        db  0B9h, 020h, 0B7h, 000h, 000h
        mov    edi, dword ptr [ecx + 8]
        mov    dword ptr [eax + 8], edi
        mov    dword ptr [eax + 0ch], ecx
        mov    dword ptr [edi + 0ch], eax
        mov    dword ptr [ecx + 8], eax
        xchg   edi, eax
        sub    eax, eax
        mov    dword ptr [edi + 20h], eax
        mov    dword ptr [edi + 60h], eax
        mov    byte ptr [edi + 23h], 1
        db  0A2h, 0F0h, 0B7h, 000h, 000h
        db  0B8h, 0F4h, 0B7h, 000h, 000h
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_812A
        call   sub_1_A258
loc_1_812A:
        xor    ecx, ecx
        mov    dword ptr [edi + 64h], ecx
        mov    dword ptr [edi], edi
        mov    dword ptr [edi + 4], edi
        mov    al, byte ptr [ebx + 0bh]
        mov    byte ptr [edi + 43h], al
        mov    byte ptr [edi + 24h], cl
        mov    ecx, dword ptr [ebp + 1ch]
        mov    dword ptr [edi + 28h], ecx
        xor    eax, eax
        test   byte ptr [ecx + 52h], 3
        jne    SHORT loc_1_8152
        mov    eax, dword ptr [ebx + 12h]
        or     byte ptr [edi + 24h], 8
loc_1_8152:
        mov    ax, word ptr [ebx + 1ah]
        mov    dword ptr [edi + 10h], eax
        mov    al, byte ptr [ebx + 0dh]
        mov    byte ptr [edi + 42h], al
        mov    eax, dword ptr [ebx + 0eh]
        mov    dword ptr [edi + 3eh], eax
        mov    ax, word ptr [ebx + 12h]
        mov    word ptr [edi + 3ch], ax
        mov    eax, dword ptr [ebx + 16h]
        mov    dword ptr [edi + 38h], eax
        mov    eax, dword ptr [ebx + 1ch]
        mov    dword ptr [edi + 30h], eax
        mov    esi, ebx
        add    edi, 45h
        mov    ecx, 0bh
        rep movsb
        sub    edi, 50h
        mov    esi, dword ptr [edi + 28h]
        test   byte ptr [ebp + 37h], 10h
        je     SHORT loc_1_81D6
        mov    ecx, dword ptr [esi + 2ch]
        lea    ecx, [ecx*8 + 0ff7h]
        shr    ecx, 0ch
        push   8
        push   ecx
        push   80080000h
        db  0CDh, 020h, 01Dh, 001h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_821F
        push   8
        push   1
        push   eax
        db  0CDh, 020h, 054h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_821F
        or     byte ptr [edi + 24h], 1
        mov    dword ptr [edi + 54h], eax
        add    eax, 1000h
        mov    dword ptr [edi + 58h], eax
        jmp    SHORT loc_1_81EB
loc_1_81D6:
        mov    eax, 40h
        call   sub_1_A1CE
        jb     SHORT loc_1_821F
        mov    dword ptr [edi + 54h], eax
        add    eax, 40h
        mov    dword ptr [edi + 58h], eax
loc_1_81EB:
        and    dword ptr [edi + 50h], 0
        mov    eax, dword ptr [edi + 54h]
        sub    eax, 8
        mov    dword ptr [edi + 5ch], eax
        mov    bl, byte ptr [ebp + 62h]
        and    ebx, 7
        db  08Bh, 00Ch, 09Dh, 0D0h, 0B7h, 000h, 000h
        mov    dword ptr [edi + 2ch], ecx
        call   sub_1_A01A
        xchg   edi, eax
        inc    dword ptr [eax + 20h]
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        add    esp, 4
        clc
        ret
loc_1_821F:
        mov    eax, dword ptr [edi + 8]
        mov    ebx, dword ptr [edi + 0ch]
        mov    dword ptr [eax + 0ch], ebx
        mov    dword ptr [ebx + 8], eax
        xchg   edi, eax
        call   sub_1_A1F8
        xchg   edi, eax
        jmp    NEAR PTR loc_1_80A8
        db  090h
loc_1_8238:
        movsb
        dec    ecx
sub_1_823A:
        jecxz  loc_1_8253
        test   esi, 3
        jne    SHORT loc_1_8238
        shrd   eax, ecx, 2
        shr    ecx, 2
        rep movsd
        shld   ecx, eax, 2
        rep movsb
loc_1_8253:
        clc
        ret
loc_1_8255:
        mov    byte ptr [ebp + 34h], 0fh
        stc
        ret
sub_1_825B:
        db  0B8h, 020h, 0B7h, 000h, 000h
loc_1_8260:
        mov    eax, dword ptr [eax + 8]
        db  03Dh, 020h, 0B7h, 000h, 000h
        je     SHORT loc_1_8275
        cmp    ebx, dword ptr [eax + 28h]
        jne    SHORT loc_1_8260
        or     byte ptr [eax + 44h], 1
        jmp    SHORT loc_1_8260
loc_1_8275:
        ret
        db  000h, 000h
loc_1_8278:
        ret
sub_1_8279:
        db  0B8h, 0FCh, 0B6h, 000h, 000h
        cmp    dword ptr [eax + 1ch], eax
        je     SHORT loc_1_8278
        db  0BFh, 048h, 0B7h, 000h, 000h
loc_1_8288:
        mov    edi, dword ptr [edi]
        db  081h, 0FFh, 048h, 0B7h, 000h, 000h
        je     SHORT loc_1_829E
        mov    dword ptr [edi + 84h], 0
        jmp    SHORT loc_1_8288
loc_1_829E:
        mov    bh, 1
        db  066h, 089h, 01Dh, 034h, 0B8h, 000h, 000h
        call   sub_1_89CF
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        cld
        mov    edx, eax
        db  08Ah, 00Dh, 01Dh, 0B8h, 000h, 000h
        db  0BEh, 0FCh, 0B6h, 000h, 000h
        mov    ebx, 1000000h
        mov    eax, eax
loc_1_82C8:
        mov    esi, dword ptr [esi + 20h]
loc_1_82CB:
        db  081h, 0FEh, 0FCh, 0B6h, 000h, 000h
        je     NEAR PTR loc_1_838E
        mov    edi, dword ptr [esi + 0ch]
        btr    dword ptr [esi + 2bh], 2
        jb     SHORT loc_1_82F4
        mov    eax, dword ptr [edi + 74h]
        shr    eax, cl
        add    eax, dword ptr [esi + 18h]
        sub    eax, edx
        jbe    SHORT loc_1_82F4
        cmp    eax, ebx
        jae    SHORT loc_1_82C8
        xchg   ebx, eax
        jmp    SHORT loc_1_82C8
loc_1_82F4:
        push   ebx
        push   ecx
        push   edx
        push   dword ptr [esi + 20h]
        mov    eax, dword ptr [esi + 1ch]
        mov    ebx, dword ptr [esi + 20h]
        mov    dword ptr [eax + 20h], ebx
        mov    dword ptr [ebx + 1ch], eax
        db  0FFh, 00Dh, 050h, 0B7h, 000h, 000h
        add    byte ptr [esi + 2bh], 8
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [edi + 84h]
        jecxz  loc_1_8330
        cmp    eax, dword ptr [ecx + 8]
        jbe    SHORT loc_1_8330
        mov    ebx, dword ptr [ecx + 1ch]
        cmp    ebx, dword ptr [edi + 80h]
        je     SHORT loc_1_8370
        cmp    eax, dword ptr [ebx + 8]
        jb     SHORT loc_1_8370
loc_1_8330:
        mov    ecx, dword ptr [edi + 80h]
        jecxz  loc_1_835F
loc_1_8338:
        mov    ecx, dword ptr [ecx + 20h]
        cmp    eax, dword ptr [ecx + 8]
        ja     SHORT loc_1_8370
        cmp    ecx, dword ptr [edi + 80h]
        jne    SHORT loc_1_8338
        mov    eax, dword ptr [ecx + 20h]
        mov    dword ptr [eax + 1ch], esi
        mov    dword ptr [esi + 1ch], ecx
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [ecx + 20h], esi
        mov    dword ptr [edi + 80h], esi
        jmp    SHORT loc_1_837F
loc_1_835F:
        mov    dword ptr [edi + 80h], esi
        mov    dword ptr [esi + 1ch], esi
        mov    dword ptr [esi + 20h], esi
        jmp    SHORT loc_1_837F
        db  02Eh, 08Bh, 0C0h
loc_1_8370:
        mov    eax, dword ptr [ecx + 1ch]
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 20h], ecx
        mov    dword ptr [eax + 20h], esi
        mov    dword ptr [ecx + 1ch], esi
loc_1_837F:
        mov    dword ptr [edi + 84h], esi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        jmp    NEAR PTR loc_1_82CB
loc_1_838E:
        db  0C6h, 005h, 035h, 0B8h, 000h, 000h, 000h
        push   ebx
        db  0BFh, 048h, 0B7h, 000h, 000h
loc_1_839B:
        mov    edi, dword ptr [edi]
        db  081h, 0FFh, 048h, 0B7h, 000h, 000h
        je     SHORT loc_1_83C6
        sub    eax, eax
        cmp    dword ptr [edi + 80h], eax
        je     SHORT loc_1_839B
        push   dword ptr [edi + 78h]
        call   sub_1_83D0
        pop    eax
        db  080h, 03Dh, 034h, 0B8h, 000h, 000h, 000h
        je     SHORT loc_1_839B
        mov    dword ptr [edi + 78h], eax
        jmp    SHORT loc_1_839B
loc_1_83C6:
        pop    ebx
loc_1_83C7:
        ret
        db  001h, 002h, 004h, 008h, 010h, 020h, 040h, 080h
sub_1_83D0:
        call   sub_1_83F7
        jb     SHORT loc_1_83D8
        ret
loc_1_83D8:
        db  0BEh, 02Bh, 089h, 000h, 000h
        mov    eax, 64h
        mov    edx, edi
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        cld
        ret
loc_1_83EC:
        stc
        ret
loc_1_83EE:
        db  00Fh, 0BAh, 035h, 03Ch, 0B8h, 000h, 000h, 000h
        ret
sub_1_83F7:
        db  00Fh, 0BAh, 02Dh, 03Ch, 0B8h, 000h, 000h, 000h
        jb     SHORT loc_1_83EC
        push   eax
        mov    ah, 1
        call   sub_1_A3AF
        pop    eax
        jb     SHORT loc_1_83EE
        db  089h, 01Dh, 040h, 0B8h, 000h, 000h
        mov    byte ptr [edi + 0bbh], al
        cmp    dword ptr [edi + 68h], 8
        jae    NEAR PTR loc_1_8624
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        cld
        db  0A3h, 038h, 0B8h, 000h, 000h
loc_1_842E:
        mov    esi, dword ptr [edi + 80h]
        and    esi, esi
        je     NEAR PTR loc_1_8624
        cmp    byte ptr [esi + 32h], 0
        jne    SHORT loc_1_844E
        mov    ebx, esi
        pushfd
        cli
        call   sub_1_893E
        popfd
        jmp    SHORT loc_1_842E
loc_1_844E:
        test   byte ptr [esi + 32h], 1
        je     SHORT loc_1_8487
loc_1_8454:
        mov    ecx, esi
        mov    ebx, dword ptr [esi + 8]
        movzx  eax, byte ptr [edi + 4dh]
        sub    ebx, eax
        call   sub_1_00B0
        xchg   ecx, esi
        jb     SHORT loc_1_8487
        jne    SHORT loc_1_8487
        test   byte ptr [ecx + 32h], 80h
        je     SHORT loc_1_8487
        cmp    dword ptr [ecx + 1ch], 0
        je     SHORT loc_1_8487
        mov    esi, ecx
        movzx  eax, byte ptr [edi + 4dh]
        db  08Ah, 080h, 0E1h, 002h, 000h, 000h
        cmp    byte ptr [esi + 32h], al
        je     SHORT loc_1_8454
loc_1_8487:
        bsf    ecx, dword ptr [esi + 32h]
        mov    ax, word ptr [edi + 20h]
        mul    cx
        movzx  ebx, ax
        mov    edx, dword ptr [esi + 8]
        add    edx, ecx
        add    ebx, dword ptr [esi + 10h]
        sub    eax, eax
        inc    eax
        shl    eax, cl
        sub    ecx, ecx
        mov    ah, al
        mov    eax, eax
loc_1_84A8:
        inc    ecx
        or     ah, al
        shl    al, 1
        jb     SHORT loc_1_84B4
        test   byte ptr [esi + 32h], al
        jne    SHORT loc_1_84A8
loc_1_84B4:
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        push   eax
        push   esi
        push   ebx
        mov    ah, 1
        call   sub_1_A3AF
        jb     NEAR PTR loc_1_864D
        mov    word ptr [ebx + 4], 1
        add    edx, dword ptr [edi + 10h]
        mov    dword ptr [ebx + 10h], edx
        mov    edx, ebx
        mov    esi, dword ptr [esp + 4]
        mov    dword ptr [edx + 2ch], esi
        db  0C7h, 042h, 00Ch, 069h, 087h, 000h, 000h
        or     dword ptr [edx + 8], 2
        mov    al, byte ptr [edi + 0bbh]
        xor    byte ptr [edx + 8], al
        bt     dword ptr [esi + 2bh], 0
        sbb    eax, eax
        and    eax, 1000000h
        or     dword ptr [edx + 8], eax
        lea    esi, [edx + 50h]
        mov    dword ptr [edx + 1ch], esi
loc_1_8508:
        mov    dword ptr [esi], ecx
        add    dword ptr [edx + 18h], ecx
        pop    dword ptr [esi + 4]
        add    esi, 8
        pop    ebx
        pop    eax
        inc    byte ptr [edx + 4ch]
        test   byte ptr [edx + 8], 1
        je     SHORT loc_1_8527
        db  08Bh, 00Dh, 038h, 0B8h, 000h, 000h
        mov    dword ptr [edi + 78h], ecx
loc_1_8527:
        not    ah
        and    byte ptr [ebx + 2ah], ah
        and    byte ptr [ebx + 32h], ah
        je     NEAR PTR loc_1_8658
loc_1_8535:
        mov    dword ptr [esi], 0
        test   byte ptr [edi + 52h], 4
        jne    NEAR PTR loc_1_85E7
        cmp    byte ptr [edi + 46h], 1
        je     NEAR PTR loc_1_85E7
        mov    eax, dword ptr [edi + 18h]
        cmp    byte ptr [edi + 48h], 0
        je     SHORT loc_1_855B
        add    eax, dword ptr [edi + 30h]
loc_1_855B:
        mov    ebx, dword ptr [edx + 10h]
        cmp    ebx, eax
        jae    NEAR PTR loc_1_85E7
        mov    ecx, dword ptr [edx + 18h]
        add    ecx, ebx
        movzx  eax, word ptr [edi + 26h]
        cmp    ecx, eax
        jbe    SHORT loc_1_85E7
        push   edx
        push   edi
        mov    esi, edx
        sub    edi, edi
        db  087h, 03Dh, 040h, 0B8h, 000h, 000h
        mov    eax, dword ptr [edi + 48h]
        push   dword ptr [edi + 24h]
        mov    ecx, 26h
        mov    edx, edi
        rep movsd
        pop    dword ptr [edx + 24h]
        pop    edi
        mov    dword ptr [edx + 48h], eax
        lea    eax, [edx + 50h]
        mov    dword ptr [edx + 1ch], eax
        mov    eax, dword ptr [edi + 30h]
        cmp    byte ptr [edi + 48h], 0
        je     SHORT loc_1_85A6
        neg    eax
loc_1_85A6:
        add    dword ptr [edx + 10h], eax
        mov    esi, dword ptr [edx + 2ch]
        lea    eax, [edx + 50h]
loc_1_85AF:
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        add    eax, 8
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_85CD
        movzx  ebx, byte ptr [edi + 4dh]
        add    ebx, dword ptr [esi + 8]
        push   eax
        call   sub_1_00B0
        pop    eax
        jmp    SHORT loc_1_85AF
loc_1_85CD:
        xchg   dword ptr [esp], edx
        inc    dword ptr [edi + 68h]
        push   edi
        xchg   edx, eax
        call   sub_1_A558
        db  0FFh, 005h, 08Ch, 0B6h, 000h, 000h
        call   sub_1_199E
        pop    edi
        pop    edx
loc_1_85E7:
        xchg   edx, eax
        call   sub_1_A558
        db  0FFh, 005h, 08Ch, 0B6h, 000h, 000h
        inc    dword ptr [edi + 68h]
        push   edi
        call   sub_1_199E
        pop    edi
        cmp    dword ptr [edi + 68h], 8
        jae    SHORT loc_1_8624
        db  083h, 03Dh, 040h, 0B8h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_842E
        mov    ah, 1
        call   sub_1_A3AF
        jb     SHORT loc_1_8635
        db  089h, 01Dh, 040h, 0B8h, 000h, 000h
        jmp    NEAR PTR loc_1_842E
loc_1_8624:
        xor    ebx, ebx
        db  087h, 01Dh, 040h, 0B8h, 000h, 000h
        or     ebx, ebx
        je     SHORT loc_1_8635
        call   sub_1_A4AE
loc_1_8635:
        db  0C6h, 005h, 03Ch, 0B8h, 000h, 000h, 000h
        cmp    dword ptr [edi + 68h], 0
        jne    SHORT loc_1_864C
        cmp    dword ptr [edi + 80h], 0
        je     SHORT loc_1_864C
        stc
loc_1_864C:
        ret
loc_1_864D:
        pop    eax
        pop    esi
        pop    eax
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        jmp    SHORT loc_1_8624
loc_1_8658:
        mov    al, byte ptr [ebx + 2bh]
        push   eax
        push   edx
        push   esi
        push   dword ptr [ebx + 8]
        pushfd
        cli
        call   sub_1_893E
        popfd
        call   sub_1_899F
        pop    ebx
        pop    esi
        pop    edx
        pop    eax
        cmp    byte ptr [edx + 4ch], 8
        je     NEAR PTR loc_1_8535
        movzx  ecx, byte ptr [edi + 4dh]
        db  084h, 0A1h, 0C7h, 083h, 000h, 000h
        jne    NEAR PTR loc_1_8535
        mov    ecx, dword ptr [edi + 14h]
        mov    cx, word ptr [ecx + 8]
        cmp    cl, byte ptr [edx + 4ch]
        je     NEAR PTR loc_1_8535
        movzx  ecx, byte ptr [edi + 4dh]
        add    ebx, ecx
        push   eax
        push   esi
        call   sub_1_00B0
        mov    ebx, esi
        pop    esi
        pop    eax
        jb     NEAR PTR loc_1_8535
        jne    NEAR PTR loc_1_8535
        test   byte ptr [ebx + 32h], 1
        je     NEAR PTR loc_1_8535
        xor    al, byte ptr [ebx + 2bh]
        test   al, 1
        jne    NEAR PTR loc_1_8535
        cmp    dword ptr [ebx + 1ch], 0
        je     NEAR PTR loc_1_8535
        xchg   ebx, esi
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        xchg   ebx, esi
        sub    eax, eax
        sub    ecx, ecx
        inc    eax
loc_1_86E7:
        inc    ecx
        or     ah, al
        shl    al, 1
        jb     SHORT loc_1_86F3
        test   byte ptr [ebx + 32h], al
        jne    SHORT loc_1_86E7
loc_1_86F3:
        push   eax
        push   ebx
        push   dword ptr [ebx + 10h]
        jmp    NEAR PTR loc_1_8508
sub_1_86FD:
        mov    esi, dword ptr [edx + 2ch]
        mov    edi, dword ptr [esi + 0ch]
        mov    ebx, dword ptr [edx + 1ch]
loc_1_8706:
        cmp    word ptr [esi + 26h], 0
        je     SHORT loc_1_8714
        cmp    word ptr [esi + 26h], 2
        jb     SHORT loc_1_872C
loc_1_8714:
        add    ebx, 8
        cmp    dword ptr [ebx], 0
        je     SHORT loc_1_872C
        push   ebx
        movzx  ebx, byte ptr [edi + 4dh]
        add    ebx, dword ptr [esi + 8]
        call   sub_1_00B0
        pop    ebx
        jmp    SHORT loc_1_8706
loc_1_872C:
        ret
loc_1_872D:
        call   sub_1_86FD
        jae    SHORT loc_1_878F
        mov    esi, dword ptr [edx + 2ch]
        mov    ebx, dword ptr [esi + 0ch]
        test   byte ptr [ebx + 1fh], 20h
        je     SHORT loc_1_8757
        db  0BFh, 0F0h, 022h, 000h, 000h
        mov    ecx, 4
        movzx  eax, word ptr [edx + 6]
        cld
        repne scasb
        jne    SHORT loc_1_8757
        or     byte ptr [ebx + 1fh], 40h
loc_1_8757:
        push   edx
        mov    edx, ebx
        db  0BEh, 074h, 088h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        cld
        pop    edx
        jmp    SHORT loc_1_878F
loc_1_8769:
        cld
        db  0FFh, 00Dh, 08Ch, 0B6h, 000h, 000h
        mov    edx, esi
        sub    eax, eax
        mov    ebx, eax
        mov    ecx, 408h
        db  0BEh, 087h, 087h, 000h, 000h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        ret
loc_1_8787:
        cld
        cmp    word ptr [edx + 6], 10h
        jae    SHORT loc_1_872D
loc_1_878F:
        mov    esi, dword ptr [edx + 2ch]
        mov    ebx, dword ptr [edx + 1ch]
        mov    edi, dword ptr [esi + 0ch]
loc_1_8798:
        cmp    word ptr [edx + 6], 10h
        jae    SHORT loc_1_87CF
loc_1_879F:
        cmp    byte ptr [esi + 32h], 1
        sbb    eax, eax
        db  029h, 005h, 0D8h, 0B1h, 000h, 000h
        not    eax
        and    word ptr [esi + 26h], ax
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        add    ebx, 8
        cmp    dword ptr [ebx], 0
        je     SHORT loc_1_87EA
        push   ebx
        movzx  ebx, byte ptr [edi + 4dh]
        add    ebx, dword ptr [esi + 8]
        call   sub_1_00B0
        pop    ebx
        jmp    SHORT loc_1_8798
loc_1_87CF:
        movzx  eax, word ptr [edx + 6]
        mov    ecx, 10h
loc_1_87D8:
        dec    ecx
        je     SHORT loc_1_879F
        bt     dword ptr [esi + 26h], ecx
        jae    SHORT loc_1_87D8
        mov    byte ptr [ecx + edi + 0bch], al
        jmp    SHORT loc_1_87D8
loc_1_87EA:
        db  0B8h, 0D4h, 0B1h, 000h, 000h
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_87F9
        call   sub_1_A258
loc_1_87F9:
        lea    ebx, [edx]
        mov    edx, dword ptr [edx + 8]
        call   sub_1_A4AE
        dec    dword ptr [edi + 68h]
        test   dl, 1
        je     SHORT loc_1_8831
        call   sub_1_88F1
loc_1_8810:
        mov    eax, dword ptr [edi + 68h]
        or     eax, dword ptr [edi + 80h]
        jne    SHORT loc_1_8830
        call   sub_1_89A5
        lea    eax, [edi + 88h]
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_8830
        call   sub_1_A258
loc_1_8830:
        ret
loc_1_8831:
        cmp    dword ptr [edi + 80h], 0
        je     SHORT loc_1_8841
        call   sub_1_88F1
        jmp    SHORT loc_1_885C
loc_1_8841:
        push   edi
        call   sub_1_8AF8
        pop    edi
        jb     SHORT loc_1_8810
        db  083h, 03Dh, 02Ah, 0B8h, 000h, 000h, 001h
        jne    SHORT loc_1_885C
        db  083h, 03Dh, 05Ch, 0B7h, 000h, 000h, 000h
        jne    SHORT loc_1_88CE
loc_1_885C:
        db  00Fh, 0BAh, 02Dh, 008h, 0B8h, 000h, 000h, 000h
        jb     SHORT loc_1_8810
        db  0BEh, 027h, 08Ch, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        cld
        jmp    SHORT loc_1_8810
loc_1_8874:
        db  00Fh, 0BAh, 02Dh, 008h, 0B8h, 000h, 000h, 001h
        jb     SHORT loc_1_88CD
        mov    al, byte ptr [edx + 0ach]
        add    al, 41h
        db  0A2h, 014h, 0B9h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  08Dh, 00Dh, 014h, 0B9h, 000h, 000h
        push   ecx
        push   0c0002h
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 059h, 001h, 001h, 000h
        mov    ecx, eax
        db  00Fh, 0B7h, 03Dh, 00Ch, 000h, 000h, 000h
        db  003h, 03Dh, 004h, 000h, 000h, 000h
        mov    eax, 0c0001000h
        db  0CDh, 020h, 003h, 000h, 017h, 000h
        add    esp, dword ptr [esp]
        db  00Fh, 0BAh, 035h, 008h, 0B8h, 000h, 000h, 001h
loc_1_88CD:
        ret
loc_1_88CE:
        db  083h, 03Dh, 0B8h, 0B6h, 000h, 000h, 000h
        jne    SHORT loc_1_885C
        db  083h, 03Dh, 08Ch, 0B6h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_885C
        pushad
        db  0FFh, 015h, 05Ch, 0B7h, 000h, 000h
        popad
        jmp    NEAR PTR loc_1_885C
sub_1_88F1:
        cmp    dword ptr [edi + 68h], 6
        jae    SHORT loc_1_8925
        sub    eax, eax
        cmp    dword ptr [edi + 80h], eax
        je     SHORT loc_1_8925
        db  038h, 005h, 035h, 0B8h, 000h, 000h
        jne    SHORT loc_1_8925
        call   sub_1_83F7
        jae    SHORT loc_1_8925
        bts    dword ptr [edi + 1fh], 0
        jb     SHORT loc_1_8925
        db  0BEh, 026h, 089h, 000h, 000h
        mov    edx, edi
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        cld
loc_1_8925:
        ret
loc_1_8926:
        btr    dword ptr [edx + 1fh], 0
loc_1_892B:
        cld
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, edx
        sub    eax, eax
        call   sub_1_83D0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_893E:
        mov    esi, dword ptr [ebx + 1ch]
        mov    eax, dword ptr [ebx + 1ch]
        mov    edx, dword ptr [ebx + 20h]
        mov    dword ptr [eax + 20h], edx
        mov    dword ptr [edx + 1ch], eax
        test   byte ptr [ebx + 2bh], 8
        jne    SHORT loc_1_8977
        cmp    dword ptr [edi + 80h], ebx
        jne    SHORT loc_1_8961
        mov    dword ptr [edi + 80h], esi
loc_1_8961:
        cmp    ebx, esi
        jne    SHORT loc_1_896D
        sub    esi, esi
        mov    dword ptr [edi + 80h], esi
loc_1_896D:
        sub    eax, eax
        mov    dword ptr [ebx + 1ch], eax
        and    byte ptr [ebx + 2bh], 0e6h
        ret
loc_1_8977:
        db  0FFh, 00Dh, 050h, 0B7h, 000h, 000h
        jmp    SHORT loc_1_896D
sub_1_897F:
        inc    dword ptr [edi + 6ch]
        cmp    dword ptr [edi + 6ch], 1
        je     SHORT loc_1_8989
loc_1_8988:
        ret
loc_1_8989:
        mov    eax, dword ptr [edi + 14h]
        test   dword ptr [eax], 10000h
        je     SHORT loc_1_8988
        push   ebx
        mov    ebx, edi
        call   sub_1_14EC
        pop    ebx
        jmp    SHORT loc_1_8988
sub_1_899F:
        dec    dword ptr [edi + 6ch]
        je     SHORT loc_1_89AB
loc_1_89A4:
        ret
sub_1_89A5:
        cmp    dword ptr [edi + 6ch], 0
        jne    SHORT loc_1_89A4
loc_1_89AB:
        cmp    dword ptr [edi + 68h], 0
        jne    SHORT loc_1_89A4
        mov    eax, dword ptr [edi + 14h]
        test   dword ptr [eax], 10000h
        je     SHORT loc_1_89A4
        test   word ptr [edi + 52h], 4000h
        jne    SHORT loc_1_89A4
        push   ebx
        mov    ebx, edi
        call   sub_1_14FB
        pop    ebx
        jmp    SHORT loc_1_89A4
sub_1_89CF:
        push   ebx
        push   ecx
        push   edx
        db  0A1h, 050h, 0B7h, 000h, 000h
        db  08Bh, 01Dh, 004h, 0B8h, 000h, 000h
        db  0A3h, 004h, 0B8h, 000h, 000h
        sub    ebx, eax
        sbb    edx, edx
        and    eax, edx
        db  003h, 005h, 050h, 0B7h, 000h, 000h
        shl    eax, 3
        push   eax
        db  08Ah, 025h, 0F8h, 0B6h, 000h, 000h
        db  0CDh, 020h, 002h, 000h, 08Bh, 004h
        mov    ebx, eax
        pop    eax
        sub    edx, edx
        div    ebx
        db  08Ah, 080h, 00Ch, 0B8h, 000h, 000h
        db  0A2h, 01Dh, 0B8h, 000h, 000h
        pop    edx
        pop    ecx
        pop    ebx
        ret
loc_1_8A14:
        cld
        db  0C7h, 005h, 01Eh, 0B8h, 000h, 000h, 000h, 000h, 000h, 000h
        db  03Bh, 015h, 022h, 0B8h, 000h, 000h
        jne    SHORT loc_1_8A93
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0FFh, 005h, 0F8h, 0B7h, 000h, 000h
        db  081h, 03Dh, 018h, 0B7h, 000h, 000h, 0FCh, 0B6h, 000h, 000h
        je     SHORT loc_1_8A94
        push   edx
        mov    bl, 1
        call   sub_1_8279
        push   ebx
        call   sub_1_8AF8
        pop    eax
        pop    edx
        jb     SHORT loc_1_8A80
        cmp    eax, ebx
        jb     SHORT loc_1_8A54
        xchg   ebx, eax
loc_1_8A54:
        cmp    eax, 7d0h
        jb     SHORT loc_1_8A60
loc_1_8A5B:
        mov    eax, 7d0h
loc_1_8A60:
        db  039h, 015h, 022h, 0B8h, 000h, 000h
        jne    SHORT loc_1_8A80
        cmp    eax, 1
        adc    eax, 0
        db  0BEh, 014h, 08Ah, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        cld
        db  089h, 035h, 01Eh, 0B8h, 000h, 000h
loc_1_8A80:
        db  0B8h, 0D4h, 0B1h, 000h, 000h
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_8A8F
        call   sub_1_A258
loc_1_8A8F:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
loc_1_8A93:
        ret
loc_1_8A94:
        db  083h, 03Dh, 05Ch, 0B7h, 000h, 000h, 000h
        je     SHORT loc_1_8A5B
        db  083h, 03Dh, 0B8h, 0B6h, 000h, 000h, 000h
        jne    SHORT loc_1_8A5B
        db  083h, 03Dh, 08Ch, 0B6h, 000h, 000h, 000h
        jne    SHORT loc_1_8A5B
        pushad
        db  0FFh, 015h, 05Ch, 0B7h, 000h, 000h
        cmp    eax, 0
        popad
        je     SHORT loc_1_8A5B
        jmp    SHORT loc_1_8A80
loc_1_8ABE:
        db  03Bh, 015h, 0F8h, 0B7h, 000h, 000h
        je     SHORT loc_1_8AE3
loc_1_8AC6:
        db  08Bh, 015h, 0F8h, 0B7h, 000h, 000h
        db  089h, 015h, 0FCh, 0B7h, 000h, 000h
        mov    eax, 1388h
        db  0BEh, 0BEh, 08Ah, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        ret
loc_1_8AE3:
        db  0FFh, 005h, 000h, 0B8h, 000h, 000h
        pushad
        call   sub_1_8C27
        popad
        jmp    SHORT loc_1_8AC6
loc_1_8AF2:
        mov    ebx, 1f4h
        ret
sub_1_8AF8:
        db  080h, 03Dh, 035h, 0B8h, 000h, 000h, 000h
        jne    SHORT loc_1_8AF2
        db  0C7h, 005h, 02Ah, 0B8h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 02Eh, 0B8h, 000h, 000h, 001h, 000h, 000h, 000h
        call   sub_1_89CF
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        cld
        xchg   edx, eax
        db  08Ah, 00Dh, 01Dh, 0B8h, 000h, 000h
        mov    ebx, 0f4240h
        sub    esi, esi
        db  0BFh, 048h, 0B7h, 000h, 000h
loc_1_8B34:
        mov    edi, dword ptr [edi]
        db  081h, 0FFh, 048h, 0B7h, 000h, 000h
        je     SHORT loc_1_8B7C
        cmp    dword ptr [edi + 68h], 8
        jae    SHORT loc_1_8B6C
        or     byte ptr [edi + 1fh], 2
        inc    esi
        mov    eax, dword ptr [edi + 7ch]
        shr    eax, cl
        add    eax, dword ptr [edi + 78h]
        sub    eax, edx
        jbe    SHORT loc_1_8B34
        dec    esi
        db  0C7h, 005h, 02Eh, 0B8h, 000h, 000h, 000h, 000h, 000h, 000h
        btr    dword ptr [edi + 1fh], 1
        cmp    eax, ebx
        jae    SHORT loc_1_8B34
        xchg   ebx, eax
        jmp    SHORT loc_1_8B34
loc_1_8B6C:
        and    byte ptr [edi + 1fh], 0fdh
        db  0C7h, 005h, 02Eh, 0B8h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_8B34
loc_1_8B7C:
        and    esi, esi
        je     NEAR PTR loc_1_8C1B
        push   ebx
        mov    ecx, esi
        pushfd
        cli
        db  0BEh, 0FCh, 0B6h, 000h, 000h
        mov    eax, eax
loc_1_8B90:
        mov    esi, dword ptr [esi + 20h]
        db  081h, 0FEh, 0FCh, 0B6h, 000h, 000h
        je     SHORT loc_1_8C19
        db  0C7h, 005h, 02Eh, 0B8h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    edi, dword ptr [esi + 0ch]
        test   byte ptr [edi + 1fh], 2
        je     SHORT loc_1_8B90
        cmp    dword ptr [edi + 80h], 0
        jne    SHORT loc_1_8B90
        mov    eax, dword ptr [esi + 14h]
        add    eax, dword ptr [edi + 70h]
        cmp    eax, edx
        ja     SHORT loc_1_8B90
        mov    eax, dword ptr [esi + 1ch]
        mov    ebx, dword ptr [esi + 20h]
        mov    dword ptr [eax + 20h], ebx
        mov    dword ptr [ebx + 1ch], eax
        db  0FFh, 00Dh, 050h, 0B7h, 000h, 000h
        add    byte ptr [esi + 2bh], 8
        mov    dword ptr [edi + 80h], esi
        mov    dword ptr [esi + 1ch], esi
        mov    dword ptr [esi + 20h], esi
        popfd
        mov    al, 1
        call   sub_1_83F7
        jae    SHORT loc_1_8C16
        cmp    dword ptr [edi + 80h], 1
        jb     SHORT loc_1_8C16
        cmp    dword ptr [edi + 68h], 0
        jne    SHORT loc_1_8C15
        bts    dword ptr [edi + 1fh], 0
        jb     SHORT loc_1_8C16
        db  0BEh, 026h, 089h, 000h, 000h
        mov    eax, 64h
        mov    edx, edi
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
loc_1_8C15:
        stc
loc_1_8C16:
        pop    eax
        cmc
        ret
loc_1_8C19:
        popfd
        pop    ebx
loc_1_8C1B:
        db  0A1h, 02Eh, 0B8h, 000h, 000h
        db  0A3h, 02Ah, 0B8h, 000h, 000h
        clc
        ret
sub_1_8C27:
        cld
        db  00Fh, 0BAh, 035h, 008h, 0B8h, 000h, 000h, 000h
        db  0FFh, 005h, 026h, 0B8h, 000h, 000h
        db  08Bh, 015h, 026h, 0B8h, 000h, 000h
        db  089h, 015h, 022h, 0B8h, 000h, 000h
        jmp    NEAR PTR loc_1_8A14
sub_1_8C47:
        db  00Fh, 0BAh, 02Dh, 008h, 0B8h, 000h, 000h, 000h
        jb     SHORT loc_1_8C6D
        push   esi
        sub    esi, esi
        db  087h, 035h, 01Eh, 0B8h, 000h, 000h
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        db  0BEh, 027h, 08Ch, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        cld
        pop    esi
loc_1_8C6D:
        ret
        db  08Bh, 0C0h
sub_1_8C70:
        pushad
        mov    ah, 1
        push   esi
        call   sub_1_A3AF
        pop    esi
        jb     SHORT loc_1_8CF0
        call   sub_1_897F
        lea    edx, [esi + 10h]
loc_1_8C84:
        mov    word ptr [ebx + 4], 1
        mov    dword ptr [ebx + 2ch], esi
        db  0C7h, 043h, 00Ch, 0F2h, 08Ch, 000h, 000h
        or     dword ptr [ebx + 8], 1000000h
        or     dword ptr [ebx + 8], 2
        mov    al, byte ptr [edi + 0bbh]
        xor    byte ptr [ebx + 8], al
        mov    eax, dword ptr [edx]
        add    eax, dword ptr [edi + 10h]
        mov    dword ptr [ebx + 10h], eax
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ebx + 18h], eax
        add    edx, 8
        mov    dword ptr [esi + 0ch], edx
        lea    ecx, [ebx + 50h]
        mov    byte ptr [ebx + 4ch], 1
        mov    dword ptr [ebx + 1ch], ecx
        mov    dword ptr [ecx], eax
        mov    edx, dword ptr [esi + 8]
        mov    dword ptr [ecx + 4], edx
        mov    edx, ecx
        mov    cl, byte ptr [edi + 4ch]
        shl    eax, cl
        add    dword ptr [esi + 8], eax
        add    edx, 8
        mov    dword ptr [edx], 0
        mov    eax, ebx
        call   sub_1_A558
        push   edi
        call   sub_1_199E
        pop    edi
        clc
loc_1_8CF0:
        popad
        ret
loc_1_8CF2:
        pushad
        mov    edx, dword ptr [esi + 2ch]
        cmp    word ptr [esi + 6], 10h
        jae    SHORT loc_1_8D0A
        mov    edi, dword ptr [edx + 0ch]
        cmp    dword ptr [esi], -1
        je     SHORT loc_1_8D0A
        cmp    dword ptr [edi], 0
        jne    SHORT loc_1_8D19
loc_1_8D0A:
        mov    edx, esi
        db  0BEh, 02Bh, 08Dh, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        popad
        ret
loc_1_8D19:
        mov    ebx, esi
        mov    edi, dword ptr [edx + 4]
        mov    esi, edx
        mov    edi, dword ptr [edi + 0ch]
        mov    edx, dword ptr [esi + 0ch]
        jmp    NEAR PTR loc_1_8C84
loc_1_8D2B:
        pushad
        mov    eax, dword ptr [edx + 2ch]
        mov    ebx, edx
        mov    esi, dword ptr [eax + 4]
        cmp    word ptr [ebx + 6], 10h
        mov    edi, dword ptr [esi + 0ch]
        jb     SHORT loc_1_8D4F
        cmp    dword ptr [eax], -1
        je     SHORT loc_1_8D4F
        db  0C7h, 005h, 0B8h, 0B6h, 000h, 000h, 002h, 000h, 000h, 000h
        jmp    SHORT loc_1_8D6B
loc_1_8D4F:
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        db  0CDh, 020h, 005h, 000h, 08Bh, 004h
        mov    dword ptr [eax], 0ffffffffh
        db  0C7h, 005h, 0B8h, 0B6h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_8D6B:
        call   sub_1_899F
        call   sub_1_A4AE
        db  00Fh, 0BAh, 02Dh, 008h, 0B8h, 000h, 000h, 000h
        jb     SHORT loc_1_8D85
        popad
        jmp    NEAR PTR sub_1_8C27
loc_1_8D85:
        popad
        clc
        ret
sub_1_8D88:
        mov    ah, 0bh
loc_1_8D8A:
        ror    al, 1
        add    al, byte ptr [esi]
        inc    esi
        dec    ah
        jne    SHORT loc_1_8D8A
        ret
sub_1_8D94:
        mov    esi, dword ptr [ebp + 6ch]
        test   esi, esi
        je     SHORT loc_1_8DA5
        cmp    cl, 0
        je     SHORT loc_1_8DD9
        jmp    NEAR PTR loc_1_8E30
loc_1_8DA5:
        push   ecx
        sub    eax, eax
        mov    esi, eax
        db  087h, 035h, 044h, 0B8h, 000h, 000h
        cmp    esi, 0
        je     SHORT loc_1_8DC3
        push   edi
        mov    edi, esi
        mov    ecx, 0f5h
        rep stosd
        pop    edi
        pop    ecx
        jmp    SHORT loc_1_8DD6
loc_1_8DC3:
        mov    eax, 3d4h
        call   sub_1_A1CE
        mov    esi, eax
        pop    ecx
        jb     NEAR PTR loc_1_8EA0
loc_1_8DD6:
        mov    dword ptr [ebp + 6ch], esi
loc_1_8DD9:
        mov    byte ptr [esi + 9], 1
        mov    byte ptr [esi + 0ah], 0
        mov    word ptr [esi + 0b0h], 0
        cmp    cl, 0
        jne    SHORT loc_1_8E30
        test   word ptr [ebp + 42h], 24h
        je     SHORT loc_1_8E30
        mov    eax, dword ptr [ebp + 3ah]
        movzx  ecx, word ptr [eax]
        sub    cx, word ptr [eax + 2]
        sub    ecx, 2
        mov    eax, ecx
        mov    dx, 1ah
        div    dl
        cmp    dh, ah
        adc    al, dh
        inc    al
        mov    byte ptr [esi + 9], al
        mov    byte ptr [esi + 0ah], ah
        mov    word ptr [esi + 0b0h], cx
        bt     word ptr [ebp + 42h], 9
        sbb    eax, eax
        and    eax, dword ptr [ebp + 48h]
        mov    dword ptr [esi + 338h], eax
loc_1_8E30:
        mov    byte ptr [esi + 4], 0
        mov    byte ptr [esi + 5], 0
        mov    byte ptr [esi + 6], 0
        mov    byte ptr [esi + 7], 0
        mov    byte ptr [esi + 8], 0
        mov    dword ptr [esi + 0ch], 0ffffffffh
        mov    dword ptr [esi + 10h], 0ffffffffh
        mov    dword ptr [esi + 20h], 0ffffffffh
        mov    dword ptr [esi + 18h], 0ffffffffh
        mov    dword ptr [esi + 1ch], 0ffffffffh
        mov    dword ptr [esi + 14h], 0ffffffffh
        mov    byte ptr [esi + 0bh], 0
        mov    ax, word ptr [ebp + 42h]
        and    ax, 410h
        cmp    ax, 410h
        jne    SHORT loc_1_8E9E
        sub    eax, eax
        lea    edi, [esi + 33ch]
        mov    ecx, 26h
        rep stosd
        bts    dword ptr [esi + 354h], 0
        inc    word ptr [esi + 340h]
loc_1_8E9E:
        clc
        ret
loc_1_8EA0:
        stc
        mov    eax, 8
        ret
sub_1_8EA7:
        cmp    eax, dword ptr [esi + 338h]
        jne    SHORT loc_1_8EB9
        mov    dword ptr [esi + 338h], 0
loc_1_8EB9:
        cmp    eax, 98967fh
        ja     SHORT loc_1_8EEC
        sub    eax, dword ptr [esi + 33ch]
        jb     SHORT loc_1_8EEC
        cmp    eax, 2800h
        jae    SHORT loc_1_8EEC
        mov    edx, eax
        and    eax, 3ffh
        shr    edx, 0ah
        jne    SHORT loc_1_8EE4
        bts    dword ptr [esi + 354h], eax
        jb     SHORT loc_1_8EEC
loc_1_8EE4:
        inc    word ptr [esi + edx*2 + 340h]
loc_1_8EEC:
        ret
sub_1_8EED:
        mov    eax, dword ptr [esi + 338h]
        or     eax, eax
        jne    SHORT loc_1_8F2E
        cmp    word ptr [esi + 340h], 400h
        jae    SHORT loc_1_8F31
        lea    edi, [esi + 354h]
        mov    eax, 0ffffffffh
        mov    ecx, 20h
        repe scasd
        je     SHORT loc_1_8F31
        mov    eax, dword ptr [edi - 4]
        not    eax
        bsf    eax, eax
        sub    ecx, 1fh
        neg    ecx
        shl    ecx, 5
        add    eax, ecx
        add    eax, dword ptr [esi + 33ch]
loc_1_8F2E:
        cmp    ecx, ecx
        ret
loc_1_8F31:
        mov    eax, 400h
        lea    edi, [esi + 342h]
        mov    ecx, 9
        repe scasw
        jbe    SHORT loc_1_8F55
        lea    eax, [esi + 342h]
        sub    eax, edi
        neg    eax
        shl    eax, 9
        jmp    SHORT loc_1_8F5A
loc_1_8F55:
        mov    eax, 2800h
loc_1_8F5A:
        add    dword ptr [esi + 33ch], eax
        cmp    dword ptr [esi + 33ch], 989400h
        jae    SHORT loc_1_8F7D
        lea    edi, [esi + 340h]
        sub    eax, eax
        mov    ecx, 25h
        rep stosd
        inc    ecx
        ret
loc_1_8F7D:
        stc
        ret
sub_1_8F7F:
        sub    esp, 14h
        mov    dword ptr [esp + 4], ebx
        mov    dword ptr [esp], ecx
        call   sub_1_910A
        lea    edi, [esp + 8]
        push   1
        push   8
        push   10h
        push   esi
        push   edi
        db  0CDh, 020h, 040h, 000h, 040h, 000h
        add    esp, 14h
        lea    esi, [esi + 10h]
        lea    edi, [edi + 8]
        push   1
        push   3
        push   6
        push   esi
        push   edi
        db  0CDh, 020h, 040h, 000h, 040h, 000h
        add    esp, 14h
        cmp    byte ptr [esp + 8], 0e5h
        jne    SHORT loc_1_8FC6
        mov    byte ptr [esp + 8], 5
loc_1_8FC6:
        mov    esi, dword ptr [ebp + 6ch]
        mov    ah, byte ptr [esi + 0ah]
        mov    al, byte ptr [esi + 9]
        cmp    al, 1
        jbe    SHORT loc_1_9051
        movzx  ecx, word ptr [esi + 0b0h]
        test   ecx, ecx
        je     SHORT loc_1_9051
        dec    al
        test   ah, ah
        je     SHORT loc_1_8FE7
        add    ecx, 2
loc_1_8FE7:
        movzx  eax, al
        shl    eax, 5
        add    dword ptr [esp + 4], eax
        lea    esi, [esp + 8]
        sub    al, al
        call   sub_1_8D88
        mov    edx, ecx
        mov    esi, dword ptr [ebp + 28h]
        mov    edi, dword ptr [esp + 4]
        mov    ah, 1
        sub    edi, 20h
loc_1_900A:
        db  0BBh, 048h, 0B8h, 000h, 000h
        mov    byte ptr [edi], ah
        inc    ah
        mov    byte ptr [edi + 0bh], 0fh
        mov    byte ptr [edi + 0ch], 0
        mov    byte ptr [edi + 0dh], al
        mov    word ptr [edi + 1ah], 0
loc_1_9024:
        mov    ecx, dword ptr [ebx]
        jecxz  loc_1_9046
        add    edi, dword ptr [ebx + 4]
        sub    edx, ecx
        jb     SHORT loc_1_9036
        rep movsb
loc_1_9031:
        add    ebx, 8
        jmp    SHORT loc_1_9024
loc_1_9036:
        add    ecx, edx
        rep movsb
        mov    ecx, edx
        neg    ecx
        mov    al, 0ffh
        rep stosb
        sub    edx, edx
        jmp    SHORT loc_1_9031
loc_1_9046:
        sub    edi, 40h
        test   edx, edx
        jne    SHORT loc_1_900A
        or     byte ptr [edi + 20h], 40h
loc_1_9051:
        lea    esi, [esp + 8]
        mov    edi, dword ptr [esp + 4]
        movsd
        movsd
        movsw
        movsb
        mov    ecx, dword ptr [esp]
        jecxz  loc_1_90BC
        lea    esi, [ecx + 0bh]
        movsd
        movsd
        movsd
        movsd
        movsd
        movsb
        mov    edi, dword ptr [esp + 4]
        mov    eax, dword ptr [ebp + 1ch]
        test   byte ptr [edi + 0bh], 10h
        je     SHORT loc_1_9099
        cmp    word ptr [edi + 1ah], 0
        jne    SHORT loc_1_9099
        test   byte ptr [eax + 52h], 3
        jne    SHORT loc_1_9093
        cmp    word ptr [edi + 14h], 0
        jne    SHORT loc_1_9099
        mov    word ptr [edi + 14h], 0ffffh
loc_1_9093:
        mov    word ptr [edi + 1ah], 0ffffh
loc_1_9099:
        test   byte ptr [eax + 53h], 1
        je     SHORT loc_1_90B5
        mov    word ptr [edi + 12h], 0
        mov    word ptr [edi + 10h], 0
        mov    word ptr [edi + 0eh], 0
        mov    byte ptr [edi + 0dh], 0
loc_1_90B5:
        call   sub_1_90E6
        jmp    SHORT loc_1_90E2
loc_1_90BC:
        mov    al, byte ptr [ebp + 44h]
        stosb
        sub    eax, eax
        stosb
        clc
        call   sub_1_12EA
        bt     dword ptr [ebp + 44h], 4
        sbb    eax, eax
        mov    word ptr [edi - 6], ax
        stosw
        sub    eax, eax
        stosd
        mov    edi, dword ptr [esp + 4]
        call   sub_1_90E6
loc_1_90E2:
        add    esp, 14h
        ret
sub_1_90E6:
        test   byte ptr [ebp + 43h], 10h
        je     SHORT loc_1_9109
        mov    eax, dword ptr [ebp + 1ch]
        test   byte ptr [eax + 53h], 1
        jne    SHORT loc_1_9109
        mov    eax, dword ptr [ebp + 14h]
        mov    edx, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 8]
        call   sub_1_141D
        mov    dword ptr [edi + 0eh], edx
        mov    byte ptr [edi + 0dh], al
loc_1_9109:
        ret
sub_1_910A:
        sub    esp, 18h
        mov    ebx, dword ptr [ebp + 6ch]
        test   byte ptr [ebp + 43h], 10h
        je     SHORT loc_1_914F
        mov    eax, dword ptr [ebp + 14h]
        mov    eax, dword ptr [eax + 4]
        movzx  ecx, word ptr [eax]
        sub    ecx, 2
        lea    eax, [eax + 2]
        lea    edx, [esp]
        push   ecx
        push   eax
        push   edx
        db  0CDh, 020h, 06Eh, 000h, 040h, 000h
        add    esp, 0ch
        mov    ecx, 0bh
        lea    edi, [esp]
        lea    esi, [ebx + 50h]
        test   byte ptr [ebx + 8], 8
        je     SHORT loc_1_9148
        lea    esi, [ebx + 3ah]
loc_1_9148:
        push   esi
        repe cmpsw
        pop    esi
        je     SHORT loc_1_916D
loc_1_914F:
        lea    esi, [ebx + 24h]
        test   byte ptr [ebp + 42h], 20h
        je     SHORT loc_1_916D
        lea    esi, [ebx + 3ah]
        test   byte ptr [ebx + 8], 8
        jne    SHORT loc_1_916D
        cmp    dword ptr [ebx + 338h], 0
        jne    SHORT loc_1_916D
        lea    esi, [ebx + 50h]
loc_1_916D:
        add    esp, 18h
        ret
sub_1_9171:
        movzx  ecx, word ptr [edi]
        shr    ecx, 1
        sub    ecx, 1
        jbe    SHORT loc_1_91B4
        lea    edi, [edi + 2]
loc_1_917E:
        sub    edx, edx
        mov    eax, 7eh
        repne scasw
        jne    SHORT loc_1_91B6
        jecxz  loc_1_91B6
loc_1_918C:
        movzx  eax, word ptr [edi]
        cmp    eax, 2eh
        je     SHORT loc_1_91B6
        or     eax, eax
        je     SHORT loc_1_91B6
        sub    eax, 30h
        cmp    eax, 9
        ja     SHORT loc_1_917E
        lea    edx, [edx + edx*4]
        lea    edx, [eax + edx*2]
        cmp    edx, 0f423fh
        ja     SHORT loc_1_91B4
        lea    edi, [edi + 2]
        loop   loc_1_918C
        ret
loc_1_91B4:
        sub    edx, edx
loc_1_91B6:
        ret
        db  000h
sub_1_91B8:
        sub    esp, 10h
        mov    dword ptr [ebp + 3ah], eax
        movzx  ecx, word ptr [eax]
        sub    cx, word ptr [eax + 2]
        je     NEAR PTR loc_1_9346
        jb     NEAR PTR loc_1_9358
        sub    ecx, 2
        jbe    NEAR PTR loc_1_9358
        and    dword ptr [ebp + 70h], 0
        and    word ptr [ebp + 42h], 0ffefh
        and    dword ptr [esp + 4], 0
        test   byte ptr [ebp + 43h], 8
        je     SHORT loc_1_9202
        mov    eax, dword ptr [ebp + 10h]
        mov    word ptr [eax], 4
        mov    word ptr [eax + 2], 4
        mov    word ptr [eax + 4], 0
loc_1_9202:
        test   byte ptr [ebp + 43h], 28h
        jne    SHORT loc_1_924F
        mov    esi, dword ptr [ebp + 3ah]
        call   sub_1_07D4
        jb     NEAR PTR loc_1_9346
        jne    SHORT loc_1_924F
        mov    dword ptr [ebp + 70h], eax
        bt     word ptr [ebp + 42h], 0fh
        jae    SHORT loc_1_923F
        mov    ecx, eax
        mov    esi, dword ptr [ebp + 3ah]
        movzx  eax, word ptr [esi + 2]
        lea    esi, [eax + esi + 2]
        call   sub_1_09AF
        jne    SHORT loc_1_923F
        mov    dword ptr [esp + 8], eax
        mov    dword ptr [esp + 4], ebx
loc_1_923F:
        call   sub_1_1201
        jae    NEAR PTR loc_1_92E9
        jmp    NEAR PTR loc_1_9358
loc_1_924F:
        call   sub_1_11BD
        mov    esi, dword ptr [ebp + 3ah]
        lea    esi, [esi + 4]
        movzx  eax, word ptr [esi]
        cmp    word ptr [eax + esi], 0
        je     NEAR PTR loc_1_92E9
loc_1_9268:
        mov    dword ptr [esp], esi
        mov    ax, 16h
        xchg   word ptr [ebp + 45h], ax
        mov    word ptr [esp + 0ch], ax
        mov    eax, dword ptr [esp]
        mov    ecx, 0
        call   sub_1_9366
        jae    SHORT loc_1_9294
        or     eax, eax
        jne    NEAR PTR loc_1_9361
        jmp    NEAR PTR loc_1_9358
loc_1_9294:
        test   byte ptr [ebx + 0bh], 10h
        je     NEAR PTR loc_1_9358
        call   sub_1_99B8
        jb     NEAR PTR loc_1_935F
        mov    ax, word ptr [esp + 0ch]
        mov    word ptr [ebp + 45h], ax
        mov    esi, dword ptr [esp]
        movzx  eax, word ptr [esi]
        lea    esi, [eax + esi]
        movzx  eax, word ptr [esi]
        cmp    word ptr [eax + esi], 0
        je     SHORT loc_1_92D1
        call   sub_1_11ED
        jae    SHORT loc_1_9268
        jmp    NEAR PTR loc_1_9358
loc_1_92D1:
        test   byte ptr [ebp + 43h], 28h
        jne    SHORT loc_1_92E2
        mov    esi, dword ptr [ebp + 3ah]
        call   sub_1_0891
        mov    dword ptr [ebp + 70h], eax
loc_1_92E2:
        call   sub_1_11ED
        jb     SHORT loc_1_9358
loc_1_92E9:
        or     word ptr [ebp + 42h], 10h
        mov    esi, dword ptr [ebp + 3ah]
        movzx  eax, word ptr [esi + 2]
        lea    esi, [eax + esi]
        mov    eax, esi
        cmp    dword ptr [esp + 4], 0
        je     SHORT loc_1_9320
        mov    ebx, dword ptr [esp + 4]
        mov    edx, dword ptr [esp + 8]
        mov    ecx, 3
        call   sub_1_9366
        jae    SHORT loc_1_9332
        or     eax, eax
        jne    SHORT loc_1_9361
        and    dword ptr [esp + 4], 0
        jmp    SHORT loc_1_92E9
loc_1_9320:
        mov    ecx, 0
        call   sub_1_9366
        jae    SHORT loc_1_9332
        or     eax, eax
        jne    SHORT loc_1_9361
        jmp    SHORT loc_1_934F
loc_1_9332:
        call   sub_1_99B8
        jb     SHORT loc_1_935F
        add    esp, 10h
        test   byte ptr [ebx + 0bh], 10h
        sete   al
        and    al, al
        ret
loc_1_9346:
        add    esp, 10h
        or     ebx, 0ffffffffh
        xor    ah, ah
        ret
loc_1_934F:
        mov    al, 2
        add    esp, 10h
        cmp    eax, eax
        stc
        ret
loc_1_9358:
        mov    al, 3
        add    esp, 10h
        stc
        ret
loc_1_935F:
        mov    al, 5
loc_1_9361:
        add    esp, 10h
        stc
        ret
sub_1_9366:
        mov    dword ptr [ebp + 3eh], eax
        sub    esp, 10h
        mov    byte ptr [esp + 6], cl
        mov    byte ptr [esp + 7], 0
        mov    dword ptr [esp + 8], ebx
        mov    dword ptr [esp + 0ch], edx
        sub    eax, eax
        bt     word ptr [ebp + 42h], 0dh
        adc    eax, eax
        mov    dword ptr [esp], eax
        pushfd
        cmp    dword ptr [ebp + 64h], 0
        je     SHORT loc_1_93A0
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        pop    esi
        inc    byte ptr [esp + 0bh]
loc_1_93A0:
        popfd
        call   sub_1_8D94
        jb     NEAR PTR loc_1_9944
        mov    edi, dword ptr [ebp + 3eh]
        movzx  ecx, word ptr [edi]
        sub    eax, eax
        xchg   word ptr [ecx + edi], ax
        mov    word ptr [esp + 4], ax
        mov    al, byte ptr [ebp + 42h]
        and    al, 30h
        cmp    al, 10h
        jne    SHORT loc_1_93E1
        mov    eax, dword ptr [ebp + 3eh]
        movzx  ecx, word ptr [eax]
        sub    ecx, 2
        lea    eax, [eax + 2]
        lea    edx, [esi + 24h]
        push   ecx
        push   eax
        push   edx
        db  0CDh, 020h, 06Eh, 000h, 040h, 000h
        add    esp, 0ch
loc_1_93E1:
        mov    ax, word ptr [ebp + 42h]
        and    ax, 410h
        cmp    ax, 410h
        jne    SHORT loc_1_941C
        mov    eax, dword ptr [ebp + 3eh]
        movzx  ecx, word ptr [eax]
        sub    ecx, 2
        mov    eax, dword ptr [ebp + 28h]
        lea    edx, [esi + 3ah]
        push   ecx
        push   eax
        push   edx
        db  0CDh, 020h, 045h, 000h, 040h, 000h
        add    esp, 0ch
        or     byte ptr [esi + 8], al
        push   esi
        lea    edi, [esi + 50h]
        lea    esi, [esi + 3ah]
        movsd
        movsd
        movsd
        movsd
        movsd
        movsw
        pop    esi
loc_1_941C:
        mov    byte ptr [esi + 7ch], 0
        test   word ptr [ebp + 42h], 810h
        jne    SHORT loc_1_9445
        mov    eax, dword ptr [ebp + 3eh]
        movzx  ecx, word ptr [eax]
        sub    ecx, 2
        jbe    SHORT loc_1_9445
        lea    ebx, [eax + 2]
        lea    eax, [esi + 7ch]
        push   ecx
        push   ebx
        push   eax
        db  0CDh, 020h, 078h, 000h, 040h, 000h
        add    esp, 0ch
loc_1_9445:
        mov    ebx, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 0ch]
        cmp    byte ptr [esp + 6], 0
        je     SHORT loc_1_94BD
        cmp    byte ptr [esp + 6], 1
        je     NEAR PTR loc_1_9842
        or     ebx, ebx
        jne    SHORT loc_1_9472
        mov    eax, dword ptr [ebp + 1ch]
        movzx  eax, word ptr [eax + 44h]
        cmp    edx, eax
        jae    NEAR PTR loc_1_991B
loc_1_9472:
        call   sub_1_1201
        jb     NEAR PTR loc_1_991B
        pushfd
        cmp    byte ptr [esp + 0bh], 0
        je     SHORT loc_1_9494
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        pop    esi
        dec    byte ptr [esp + 0bh]
loc_1_9494:
        popfd
        mov    eax, dword ptr [esp + 0ch]
        call   sub_1_0BD7
        jb     NEAR PTR loc_1_991B
        pushfd
        cmp    dword ptr [ebp + 64h], 0
        je     SHORT loc_1_94BA
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        pop    esi
        inc    byte ptr [esp + 0bh]
loc_1_94BA:
        popfd
        jmp    SHORT loc_1_950C
loc_1_94BD:
        mov    dword ptr [ebp + 4ch], 0
        mov    al, byte ptr [ebp + 45h]
        and    al, 9eh
        cmp    al, 8
        jne    SHORT loc_1_94D2
        call   sub_1_11BD
loc_1_94D2:
        pushfd
        cmp    byte ptr [esp + 0bh], 0
        je     SHORT loc_1_94E9
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        pop    esi
        dec    byte ptr [esp + 0bh]
loc_1_94E9:
        popfd
        call   sub_1_0BD4
        jb     NEAR PTR loc_1_991B
        pushfd
        cmp    dword ptr [ebp + 64h], 0
        je     SHORT loc_1_950B
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        pop    esi
        inc    byte ptr [esp + 0bh]
loc_1_950B:
        popfd
loc_1_950C:
        mov    esi, dword ptr [ebp + 6ch]
        cmp    dword ptr [esp], 1
        jne    SHORT loc_1_9536
        movzx  eax, word ptr [esi + 2b2h]
        cmp    eax, 10h
        jae    SHORT loc_1_9536
        mov    ecx, dword ptr [ebp + 50h]
        mov    dword ptr [esi + eax*8 + 2b6h], ecx
        and    dword ptr [esi + eax*8 + 2bah], 0
        inc    dword ptr [esp]
loc_1_9536:
        mov    ah, byte ptr [ebx]
        call   sub_1_9A38
        jb     NEAR PTR loc_1_98D4
        jne    SHORT loc_1_9564
        mov    byte ptr [esi + 4], 0
        mov    byte ptr [esi + 7], 0
        mov    byte ptr [esi + 5], 0
        mov    dword ptr [esi + 0ch], 0ffffffffh
        mov    dword ptr [esi + 10h], 0ffffffffh
        jmp    NEAR PTR loc_1_9842
loc_1_9564:
        mov    al, byte ptr [ebx + 0bh]
        and    al, 1fh
        cmp    al, 0fh
        jne    NEAR PTR loc_1_9600
        mov    ah, byte ptr [ebx]
        mov    al, byte ptr [ebx + 0dh]
        and    ah, 0bfh
        cmp    ah, 14h
        ja     NEAR PTR loc_1_9825
        cmp    ah, 1
        jb     NEAR PTR loc_1_9825
        test   byte ptr [ebx], 40h
        je     SHORT loc_1_95B7
        mov    byte ptr [esi + 4], 0
        mov    byte ptr [esi + 5], ah
        mov    byte ptr [esi + 6], al
        mov    byte ptr [esi + 7], 0
        mov    al, 1ah
        mul    ah
        movzx  eax, ax
        mov    word ptr [eax + esi + 0b2h], 0
        mov    eax, dword ptr [ebp + 4ch]
        mov    dword ptr [esi + 0ch], eax
        jmp    SHORT loc_1_95D3
loc_1_95B7:
        cmp    byte ptr [esi + 4], 0
        je     NEAR PTR loc_1_9825
        cmp    byte ptr [esi + 5], ah
        jne    NEAR PTR loc_1_9825
        cmp    byte ptr [esi + 6], al
        jne    NEAR PTR loc_1_9825
loc_1_95D3:
        dec    byte ptr [esi + 5]
        inc    byte ptr [esi + 4]
        movzx  eax, byte ptr [esi + 5]
        mov    ah, 1ah
        mul    ah
        lea    edi, [esi + eax + 0b2h]
        push   esi
        lea    esi, [ebx + 1]
        movsd
        movsd
        movsw
        add    esi, 3
        movsd
        movsd
        movsd
        add    esi, 2
        movsd
        pop    esi
        jmp    NEAR PTR loc_1_9842
loc_1_9600:
        bt     dword ptr [ebx + 0bh], 3
        sbb    eax, eax
        and    eax, 40h
        or     word ptr [ebp + 42h], ax
        or     byte ptr [esi + 7], 1
        mov    dword ptr [esi + 94h], 0
        cmp    byte ptr [esi + 7ch], 0
        jne    SHORT loc_1_9627
        call   sub_1_9CEF
loc_1_9627:
        mov    eax, dword ptr [ebp + 50h]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [ebp + 4ch]
        mov    dword ptr [esi + 10h], eax
        cmp    byte ptr [esi + 4], 0
        je     SHORT loc_1_9652
        cmp    byte ptr [esi + 5], 0
        jne    SHORT loc_1_9652
        push   esi
        lea    esi, [ebx]
        sub    al, al
        call   sub_1_8D88
        pop    esi
        cmp    byte ptr [esi + 6], al
        jne    SHORT loc_1_9652
        or     byte ptr [esi + 7], 2
loc_1_9652:
        test   byte ptr [ebx + 0bh], 18h
        jne    SHORT loc_1_9670
        mov    ax, word ptr [ebp + 42h]
        and    ax, 4010h
        cmp    ax, 4010h
        jne    SHORT loc_1_9670
        mov    ecx, dword ptr [ebp + 70h]
        jecxz  loc_1_9670
        call   sub_1_0A19
loc_1_9670:
        mov    ax, word ptr [ebp + 42h]
        and    ax, 410h
        cmp    ax, 410h
        jne    SHORT loc_1_96C5
        test   byte ptr [ebx + 0bh], 8
        jne    SHORT loc_1_96C5
        lea    edi, [esi + 3ah]
        lea    eax, [esi + 66h]
        push   edx
        push   eax
        push   edi
        db  0CDh, 020h, 046h, 000h, 040h, 000h
        add    esp, 8
        pop    edx
        test   eax, eax
        je     SHORT loc_1_96C5
        js     SHORT loc_1_96BB
        or     byte ptr [esi + 8], 10h
        push   edx
        call   sub_1_8EA7
        pop    edx
        lea    edi, [esi + 3ah]
        lea    esi, [esi + 66h]
        mov    ecx, 0bh
        repe cmpsw
        mov    esi, dword ptr [ebp + 6ch]
        jne    SHORT loc_1_96C5
loc_1_96BB:
        or     byte ptr [esi + 8], 8
        mov    eax, dword ptr [ebp + 4ch]
        mov    dword ptr [esi + 20h], eax
loc_1_96C5:
        mov    al, byte ptr [ebp + 42h]
        and    al, 30h
        cmp    al, 10h
        je     NEAR PTR loc_1_976B
        movzx  eax, byte ptr [ebp + 42h]
        and    al, 11h
        cmp    al, 11h
        setne  al
        dec    eax
        and    eax, 1
        mov    edi, dword ptr [ebp + 3eh]
        lea    edi, [edi + 2]
        test   byte ptr [esi + 7], 2
        je     SHORT loc_1_9728
        lea    ecx, [esi + 0b2h]
        push   edx
        push   eax
        or     eax, 4
        push   eax
        push   ecx
        push   edi
        db  0CDh, 020h, 03Dh, 000h, 040h, 000h
        add    esp, 0ch
        test   eax, eax
        pop    eax
        pop    edx
        je     SHORT loc_1_9728
        cmp    word ptr [esi + 94h], 0
        jne    SHORT loc_1_9719
        call   sub_1_9CEF
loc_1_9719:
        call   sub_1_9C63
        je     NEAR PTR loc_1_9884
        and    byte ptr [esi + 7], 0fdh
loc_1_9728:
        cmp    byte ptr [esi + 7ch], 0
        je     SHORT loc_1_974A
        mov    ecx, 0bh
        push   esi
        push   edi
        lea    esi, [esi + 7ch]
        lea    edi, [ebx]
        cld
        repe cmpsb
        pop    edi
        pop    esi
        jne    NEAR PTR loc_1_97FB
        jmp    NEAR PTR loc_1_97CA
loc_1_974A:
        lea    ecx, [esi + 96h]
        push   edx
        or     eax, 4
        push   eax
        push   ecx
        push   edi
        db  0CDh, 020h, 03Dh, 000h, 040h, 000h
        add    esp, 0ch
        pop    edx
        test   eax, eax
        je     NEAR PTR loc_1_97FB
        jmp    SHORT loc_1_97CA
loc_1_976B:
        lea    eax, [esi + 24h]
        lea    ecx, [esi + 66h]
        push   edx
        push   1
        push   ecx
        push   eax
        db  0CDh, 020h, 03Dh, 000h, 040h, 000h
        add    esp, 0ch
        pop    edx
        test   eax, eax
        jne    SHORT loc_1_97CA
        test   byte ptr [ebp + 42h], 1
        jne    SHORT loc_1_97FB
        test   byte ptr [esi + 7], 2
        je     SHORT loc_1_97FB
        mov    eax, dword ptr [ebp + 3eh]
        movzx  ecx, word ptr [eax]
        cmp    word ptr [ecx + esi + 0b0h], 0
        jne    SHORT loc_1_97FB
        lea    eax, [eax + 2]
        lea    ecx, [esi + 0b2h]
        push   edx
        push   4
        push   ecx
        push   eax
        db  0CDh, 020h, 03Dh, 000h, 040h, 000h
        add    esp, 0ch
        pop    edx
        test   eax, eax
        je     SHORT loc_1_97FB
        call   sub_1_9C63
        je     NEAR PTR loc_1_9884
        jmp    SHORT loc_1_97FB
loc_1_97CA:
        or     byte ptr [esi + 7], 4
        cmp    word ptr [esi + 94h], 0
        jne    SHORT loc_1_97DD
        call   sub_1_9CEF
loc_1_97DD:
        test   byte ptr [esi + 7], 2
        je     NEAR PTR loc_1_9884
        call   sub_1_9C63
        je     NEAR PTR loc_1_9884
        and    byte ptr [esi + 7], 0fdh
        jmp    NEAR PTR loc_1_9884
loc_1_97FB:
        mov    esi, dword ptr [ebp + 6ch]
        mov    byte ptr [esi + 4], 0
        mov    byte ptr [esi + 7], 0
        mov    byte ptr [esi + 5], 0
        mov    dword ptr [esi + 0ch], 0ffffffffh
        mov    dword ptr [esi + 10h], 0ffffffffh
        cmp    byte ptr [esp + 6], 3
        je     NEAR PTR loc_1_991B
        jmp    SHORT loc_1_9842
loc_1_9825:
        mov    esi, dword ptr [ebp + 6ch]
        mov    byte ptr [esi + 4], 0
        mov    byte ptr [esi + 7], 0
        mov    byte ptr [esi + 5], 0
        mov    dword ptr [esi + 0ch], 0ffffffffh
        mov    dword ptr [esi + 10h], 0ffffffffh
loc_1_9842:
        pushfd
        cmp    byte ptr [esp + 0bh], 0
        je     SHORT loc_1_9859
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        pop    esi
        dec    byte ptr [esp + 0bh]
loc_1_9859:
        popfd
        call   sub_1_0CDE
        jb     NEAR PTR loc_1_991B
        pushfd
        cmp    dword ptr [ebp + 64h], 0
        je     SHORT loc_1_987B
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        pop    esi
        inc    byte ptr [esp + 0bh]
loc_1_987B:
        popfd
        jne    NEAR PTR loc_1_950C
        jmp    SHORT loc_1_98D4
loc_1_9884:
        mov    al, byte ptr [ebx + 0bh]
        mov    ah, byte ptr [ebp + 46h]
        and    ah, al
        xor    ah, byte ptr [ebp + 46h]
        and    ah, 3fh
        jne    SHORT loc_1_98C2
        mov    ah, byte ptr [ebp + 45h]
        and    ah, 9eh
        test   al, 8
        je     SHORT loc_1_98AC
        test   ah, 8
        jne    NEAR PTR loc_1_9963
        jmp    NEAR PTR loc_1_9825
loc_1_98AC:
        cmp    ah, 8
        je     NEAR PTR loc_1_9825
        not    ah
        and    ah, al
        and    ah, 16h
        je     NEAR PTR loc_1_9963
loc_1_98C2:
        test   byte ptr [ebp + 42h], 1
        je     SHORT loc_1_991B
        cmp    byte ptr [esp + 6], 3
        je     SHORT loc_1_991B
        jmp    NEAR PTR loc_1_9825
loc_1_98D4:
        mov    esi, dword ptr [ebp + 6ch]
        mov    ax, word ptr [ebp + 42h]
        and    ax, 410h
        cmp    ax, 410h
        jne    SHORT loc_1_991B
        cmp    dword ptr [esi + 338h], 0
        jne    SHORT loc_1_9912
        cmp    word ptr [esi + 3ah], 20h
        je     SHORT loc_1_9912
        test   byte ptr [esi + 8], 0bh
        je     SHORT loc_1_991B
        test   byte ptr [ebp + 43h], 10h
        jne    SHORT loc_1_990A
        db  083h, 03Dh, 08Ch, 0B8h, 000h, 000h, 000h
        jne    SHORT loc_1_9912
loc_1_990A:
        test   byte ptr [esi + 8], 18h
        jne    SHORT loc_1_9916
        jmp    SHORT loc_1_991B
loc_1_9912:
        or     byte ptr [esi + 8], 8
loc_1_9916:
        call   sub_1_9B41
loc_1_991B:
        mov    esi, dword ptr [ebp + 6ch]
        mov    eax, dword ptr [ebp + 50h]
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 0ch], 0ffffffffh
        mov    dword ptr [esi + 10h], 0ffffffffh
        mov    eax, dword ptr [ebp + 3eh]
        movzx  ecx, word ptr [eax]
        lea    ecx, [eax + ecx]
        mov    ax, word ptr [esp + 4]
        mov    word ptr [ecx], ax
        sub    eax, eax
loc_1_9944:
        pushfd
        cmp    byte ptr [esp + 0bh], 0
        je     SHORT loc_1_995B
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        pop    esi
        dec    byte ptr [esp + 0bh]
loc_1_995B:
        popfd
        add    esp, 10h
        sub    ah, ah
        stc
        ret
loc_1_9963:
        mov    esi, dword ptr [ebp + 6ch]
        mov    eax, dword ptr [ebp + 3eh]
        movzx  ecx, word ptr [eax]
        lea    ecx, [eax + ecx]
        mov    ax, word ptr [esp + 4]
        mov    word ptr [ecx], ax
        cmp    dword ptr [esp], 2
        jne    SHORT loc_1_999A
        movzx  eax, word ptr [esi + 2b2h]
        cmp    eax, 10h
        jae    SHORT loc_1_999A
        mov    ecx, dword ptr [ebp + 4ch]
        mov    dword ptr [esi + eax*8 + 2bah], ecx
        inc    word ptr [esi + 2b2h]
loc_1_999A:
        pushfd
        cmp    byte ptr [esp + 0bh], 0
        je     SHORT loc_1_99B1
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        pop    esi
        dec    byte ptr [esp + 0bh]
loc_1_99B1:
        popfd
        add    esp, 10h
        sub    eax, eax
        ret
sub_1_99B8:
        test   byte ptr [ebp + 43h], 8
        je     SHORT loc_1_99FA
        mov    esi, dword ptr [ebp + 6ch]
        mov    edi, dword ptr [ebp + 10h]
        cmp    byte ptr [ebp + 4], 4
        je     SHORT loc_1_99FC
loc_1_99CA:
        movzx  ecx, word ptr [esi + 94h]
        movzx  eax, word ptr [edi]
        mov    word ptr [edi + 2], ax
        add    word ptr [edi], cx
        cmp    word ptr [edi], 1feh
        jae    SHORT loc_1_9A36
        mov    word ptr [eax + edi], cx
        lea    edi, [eax + edi + 2]
        lea    esi, [esi + 96h]
        shr    ecx, 2
        rep movsd
        adc    ecx, ecx
        rep movsw
loc_1_99FA:
        clc
        ret
loc_1_99FC:
        test   byte ptr [esi + 7], 2
        je     SHORT loc_1_99CA
        mov    eax, edi
        movzx  ecx, word ptr [edi]
        mov    word ptr [edi + 2], cx
        lea    esi, [esi + 0b2h]
        lea    edi, [ecx + edi + 2]
loc_1_9A15:
        cmp    ecx, 1feh
        jae    SHORT loc_1_9A36
        movsw
        add    ecx, 2
        cmp    word ptr [edi - 2], 0
        jne    SHORT loc_1_9A15
        mov    edi, ecx
        xchg   word ptr [eax], cx
        sub    edi, ecx
        mov    word ptr [ecx + eax], di
        clc
        ret
loc_1_9A36:
        stc
        ret
sub_1_9A38:
        or     ah, ah
        je     SHORT loc_1_9A88
        cmp    ah, 0e5h
        je     SHORT loc_1_9A56
        mov    al, byte ptr [esi + 0bh]
        cmp    al, byte ptr [esi + 9]
        jae    SHORT loc_1_9A85
        mov    dword ptr [esi + 14h], 0ffffffffh
        mov    byte ptr [esi + 0bh], 0
        jmp    SHORT loc_1_9A85
loc_1_9A56:
        movzx  eax, byte ptr [esi + 0bh]
        cmp    al, byte ptr [esi + 9]
        jae    SHORT loc_1_9A76
        add    eax, dword ptr [esi + 14h]
        cmp    eax, dword ptr [ebp + 4ch]
        jne    SHORT loc_1_9A6C
        inc    byte ptr [esi + 0bh]
        jmp    SHORT loc_1_9A76
loc_1_9A6C:
        mov    eax, dword ptr [ebp + 4ch]
        mov    dword ptr [esi + 14h], eax
        mov    byte ptr [esi + 0bh], 1
loc_1_9A76:
        cmp    dword ptr [esi + 18h], -1
        jne    SHORT loc_1_9A82
        mov    eax, dword ptr [ebp + 4ch]
        mov    dword ptr [esi + 18h], eax
loc_1_9A82:
        cmp    eax, eax
        ret
loc_1_9A85:
        or     esp, esp
        ret
loc_1_9A88:
        mov    eax, dword ptr [ebp + 4ch]
        mov    dword ptr [esi + 1ch], eax
        stc
        ret
sub_1_9A90:
        push   ecx
        push   eax
        call   sub_1_0BD7
        jb     NEAR PTR loc_1_9B32
loc_1_9A9D:
        mov    dword ptr [esp], ebx
loc_1_9AA0:
        mov    al, byte ptr [ebx]
        test   al, al
        je     NEAR PTR loc_1_9B3C
        cmp    al, 0e5h
        je     NEAR PTR loc_1_9B3C
        mov    ah, 0fh
        mov    al, byte ptr [ebx + 0bh]
        and    al, ah
        cmp    al, ah
        je     SHORT loc_1_9AC4
        cmp    dword ptr [esp + 4], 1
        jne    SHORT loc_1_9B3C
loc_1_9AC4:
        mov    byte ptr [ebx], 0e5h
        dec    dword ptr [esp + 4]
        je     SHORT loc_1_9AD7
        inc    dword ptr [ebp + 4ch]
        add    ebx, 20h
        cmp    ebx, edx
        jb     SHORT loc_1_9AA0
loc_1_9AD7:
        mov    esi, dword ptr [ebp + 64h]
        mov    ebx, dword ptr [esp]
        call   sub_1_03EC
        jb     SHORT loc_1_9B32
        mov    ecx, dword ptr [esp + 4]
        jecxz  loc_1_9B37
        mov    edi, dword ptr [ebp + 1ch]
        mov    al, byte ptr [ebp + 60h]
        inc    eax
        mov    byte ptr [ebp + 60h], al
        sub    al, byte ptr [ebp + 61h]
        je     SHORT loc_1_9AFF
        mov    ebx, dword ptr [ebp + 5ch]
        inc    ebx
        jmp    SHORT loc_1_9B28
loc_1_9AFF:
        mov    byte ptr [ebp + 60h], al
        mov    ebx, dword ptr [ebp + 54h]
        dec    dword ptr [ebp + 4ch]
        call   sub_1_1F2C
        jb     SHORT loc_1_9B32
        call   sub_1_2102
        jae    SHORT loc_1_9B32
        inc    dword ptr [ebp + 4ch]
        mov    dword ptr [ebp + 54h], ebx
        inc    dword ptr [ebp + 58h]
        sub    edx, edx
        call   sub_1_7E5E
        jb     SHORT loc_1_9B32
loc_1_9B28:
        call   sub_1_0C48
        jmp    NEAR PTR loc_1_9A9D
loc_1_9B32:
        add    esp, 8
        stc
        ret
loc_1_9B37:
        add    esp, 8
        clc
        ret
loc_1_9B3C:
        add    esp, 8
        stc
        ret
sub_1_9B41:
        sub    esp, 2ch
        jmp    NEAR PTR loc_1_9C3B
loc_1_9B49:
        mov    dword ptr [ebp + 4ch], 0
        mov    al, byte ptr [ebp + 45h]
        and    al, 9eh
        cmp    al, 8
        jne    SHORT loc_1_9B5E
        call   sub_1_11BD
loc_1_9B5E:
        call   sub_1_0BD4
        jb     NEAR PTR loc_1_9C5E
loc_1_9B69:
        mov    dword ptr [esp], edx
        cmp    byte ptr [ebx], 0
        je     NEAR PTR loc_1_9C3B
        cmp    byte ptr [ebx], 0e5h
        je     NEAR PTR loc_1_9C2B
        test   byte ptr [ebx + 0bh], 8
        jne    NEAR PTR loc_1_9C2B
        pushfd
        cmp    dword ptr [ebp + 64h], 0
        je     SHORT loc_1_9B9E
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        pop    esi
        inc    byte ptr [esp + 2ch]
loc_1_9B9E:
        popfd
        lea    esi, [ebx]
        lea    edi, [esp + 4]
        movsd
        movsd
        movsw
        movsb
        cmp    byte ptr [esp + 4], 5
        jne    SHORT loc_1_9BB6
        mov    byte ptr [esp + 4], 0e5h
loc_1_9BB6:
        mov    eax, 200020h
        lea    edi, [esp + 10h]
        stosd
        stosd
        stosd
        stosd
        stosd
        stosw
        lea    eax, [esp + 4]
        lea    edi, [esp + 10h]
        push   1
        push   8
        push   eax
        push   edi
        db  0CDh, 020h, 042h, 000h, 040h, 000h
        add    esp, 10h
        lea    eax, [esp + 0ch]
        lea    edi, [esp + 20h]
        push   1
        push   3
        push   eax
        push   edi
        db  0CDh, 020h, 042h, 000h, 040h, 000h
        add    esp, 10h
        lea    eax, [esp + 10h]
        mov    esi, dword ptr [ebp + 6ch]
        lea    edi, [esi + 3ah]
        push   eax
        push   edi
        db  0CDh, 020h, 046h, 000h, 040h, 000h
        add    esp, 8
        cmp    eax, 0
        jle    SHORT loc_1_9C13
        call   sub_1_8EA7
loc_1_9C13:
        pushfd
        cmp    byte ptr [esp + 2ch], 0
        je     SHORT loc_1_9C2A
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        pop    esi
        dec    byte ptr [esp + 2ch]
loc_1_9C2A:
        popfd
loc_1_9C2B:
        mov    edx, dword ptr [esp]
        call   sub_1_0CDE
        ja     NEAR PTR loc_1_9B69
        jb     SHORT loc_1_9C5E
loc_1_9C3B:
        mov    esi, dword ptr [ebp + 6ch]
        call   sub_1_8EED
        jb     SHORT loc_1_9C5E
        jne    NEAR PTR loc_1_9B49
        lea    edi, [esi + 3ah]
        push   eax
        push   edi
        db  0CDh, 020h, 047h, 000h, 040h, 000h
        add    esp, 8
        add    esp, 2ch
        clc
        ret
loc_1_9C5E:
        add    esp, 2ch
        stc
        ret
sub_1_9C63:
        push   eax
        push   ebx
        push   edx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 6ch]
        lea    edi, [esi + 96h]
        lea    esi, [esi + 0b2h]
        db  08Bh, 01Dh, 088h, 0B2h, 000h, 000h
        db  08Bh, 00Dh, 084h, 0B2h, 000h, 000h
        sub    edi, 2
loc_1_9C86:
        add    edi, 2
        movzx  eax, word ptr [edi]
        cmp    eax, 2eh
        je     SHORT loc_1_9CE9
        cmp    eax, 7eh
        je     SHORT loc_1_9CE9
        or     eax, eax
        je     SHORT loc_1_9CE9
        cmp    eax, 20h
        je     SHORT loc_1_9CE6
loc_1_9C9F:
        lodsw
        cmp    eax, 586h
        ja     SHORT loc_1_9CB0
        movzx  edx, byte ptr [eax + ebx]
        sub    ax, word ptr [edx + ecx]
loc_1_9CB0:
        cmp    ax, word ptr [edi]
        je     SHORT loc_1_9C86
        cmp    eax, 20h
        je     SHORT loc_1_9C9F
        cmp    eax, 2eh
        je     SHORT loc_1_9C9F
        or     eax, eax
        je     SHORT loc_1_9CD0
        cmp    word ptr [edi], 5fh
        je     SHORT loc_1_9C86
        cmp    word ptr [edi], 80h
        jae    SHORT loc_1_9C86
loc_1_9CD0:
        movzx  eax, word ptr [edi]
        cmp    eax, 7eh
        je     SHORT loc_1_9CE9
        cmp    eax, 2eh
        je     SHORT loc_1_9CE6
        or     eax, eax
        je     SHORT loc_1_9CE6
        add    edi, 2
        jmp    SHORT loc_1_9CD0
loc_1_9CE6:
        or     eax, 1
loc_1_9CE9:
        pop    edi
        pop    esi
        pop    edx
        pop    ebx
        pop    eax
        ret
sub_1_9CEF:
        push   edx
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esi + 96h], eax
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [esi + 9ah], eax
        mov    eax, dword ptr [ebx + 8]
        mov    byte ptr [esi + 9eh], al
        shr    eax, 8
        mov    word ptr [esi + 9fh], ax
        cmp    byte ptr [esi + 96h], 5
        jne    SHORT loc_1_9D24
        mov    byte ptr [esi + 96h], 0e5h
loc_1_9D24:
        mov    eax, 200020h
        mov    dword ptr [esi + 66h], eax
        mov    dword ptr [esi + 6ah], eax
        mov    dword ptr [esi + 6eh], eax
        mov    dword ptr [esi + 72h], eax
        mov    dword ptr [esi + 76h], eax
        mov    word ptr [esi + 7ah], ax
        lea    eax, [esi + 96h]
        lea    edx, [esi + 66h]
        push   1
        push   8
        push   eax
        push   edx
        db  0CDh, 020h, 042h, 000h, 040h, 000h
        add    esp, 10h
        lea    eax, [esi + 9eh]
        lea    edx, [esi + 76h]
        push   1
        push   3
        push   eax
        push   edx
        db  0CDh, 020h, 042h, 000h, 040h, 000h
        add    esp, 10h
        lea    eax, [esi + 66h]
        lea    edx, [esi + 96h]
        push   0
        push   eax
        push   edx
        db  0CDh, 020h, 048h, 000h, 040h, 000h
        add    esp, 0ch
        mov    word ptr [eax + esi + 96h], 0
        add    eax, 2
        mov    word ptr [esi + 94h], ax
        pop    edx
        ret
sub_1_9D98:
        push   ecx
        push   edx
        mov    edi, dword ptr [ebp + 20h]
        mov    edi, dword ptr [edi + 8]
        sub    ecx, edx
        je     SHORT loc_1_9DC0
loc_1_9DA4:
        push   eax
        push   dword ptr [ebp + 20h]
        push   dword ptr [ebp + 8]
        push   ecx
        push   edx
        push   dword ptr [edi + 64h]
        db  0CDh, 020h, 056h, 000h, 040h, 000h
        add    esp, 18h
loc_1_9DB9:
        cmp    eax, 1
        cmc
        pop    edx
        pop    ecx
        ret
loc_1_9DC0:
        mov    ecx, dword ptr [edi + 30h]
        sub    ecx, edx
        ja     SHORT loc_1_9DA4
        jne    SHORT loc_1_9DCD
        sub    eax, eax
        jmp    SHORT loc_1_9DB9
loc_1_9DCD:
        mov    edx, dword ptr [edi + 30h]
        neg    ecx
        jmp    SHORT loc_1_9DA4
sub_1_9DD4:
        mov    eax, 2ch
        call   sub_1_A1CE
        xchg   edi, eax
        mov    al, 8
        jb     SHORT loc_1_9DEF
        xor    eax, eax
        mov    ecx, 0bh
        rep stosd
        lea    eax, [edi - 2ch]
loc_1_9DEF:
        ret
sub_1_9DF0:
        push   ecx
        push   edx
        db  0CDh, 020h, 007h, 000h, 040h, 000h
        cld
        pop    edx
        pop    ecx
        ret
sub_1_9DFC:
        pushfd
        test   byte ptr [ebp + 37h], 90h
        jne    NEAR PTR loc_1_9E8D
        test   byte ptr [ebp + 37h], 1
        jne    NEAR PTR loc_1_9E9F
        cmp    dword ptr [ebp], 0
        je     SHORT loc_1_9E8D
        popfd
        mov    eax, dword ptr [ebp]
        mov    esi, 200h
        mov    edx, 40000000h
        jb     SHORT loc_1_9E32
        mov    esi, 1000h
        or     edx, 80000000h
loc_1_9E32:
        cmp    eax, esi
        jb     SHORT loc_1_9E73
loc_1_9E36:
        mov    esi, dword ptr [ebp + 14h]
        add    eax, esi
        dec    eax
        shr    esi, 0ch
        shr    eax, 0ch
        sub    eax, esi
        inc    eax
        mov    dword ptr [ebp + 54h], eax
        push   edx
        push   eax
        push   esi
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        cld
        and    eax, eax
        je     SHORT loc_1_9E84
        mov    edx, dword ptr [ebp + 14h]
        and    edx, 0fffh
        or     eax, edx
        mov    dword ptr [ebp + 14h], eax
        shr    eax, 0ch
        mov    dword ptr [ebp + 50h], eax
        or     word ptr [ebp + 38h], 80h
loc_1_9E73:
        mov    eax, dword ptr [ebp + 1ch]
        test   word ptr [eax + 52h], 20h
        jne    SHORT loc_1_9E7F
        ret
loc_1_9E7F:
        or     byte ptr [ebp + 3ah], 2
        ret
loc_1_9E84:
        or     byte ptr [ebp + 3ah], 2
        and    byte ptr [ebp + 37h], 0feh
        ret
loc_1_9E8D:
        popfd
        mov    eax, dword ptr [ebp + 1ch]
        test   word ptr [eax + 52h], 20h
        jne    SHORT loc_1_9E9A
        ret
loc_1_9E9A:
        or     byte ptr [ebp + 3ah], 2
        ret
loc_1_9E9F:
        popfd
        mov    eax, dword ptr [ebp]
        mov    edx, 40000000h
        jb     SHORT loc_1_9E36
        or     edx, 80000000h
        jmp    SHORT loc_1_9E36
_LTEXT ends
        end
