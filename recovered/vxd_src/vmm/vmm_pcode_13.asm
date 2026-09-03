; vmm_pcode_13.asm -- VMM object 13 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_13_0000:
        db  0CDh, 020h, 047h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 1ch]
        movzx  ecx, ah
        cmp    cl, 0eh
        ja     SHORT loc_13_0018
        db  0FFh, 024h, 08Dh, 0B0h, 000h, 000h, 000h
loc_13_0018:
        or     byte ptr [ebp + 2ch], 1
        ret
loc_13_001D:
        and    byte ptr [ebp + 2ch], 0feh
        ret
loc_13_0022:
        cmp    al, 0dh
        ja     SHORT loc_13_0018
        movzx  eax, al
        db  0FFh, 024h, 085h, 0ECh, 000h, 000h, 000h
loc_13_0030:
        push   0
        push   0
        push   0f2h
        push   0
        push   0
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        movzx  ecx, word ptr [ebp + 18h]
        push   0
        push   ecx
        push   eax
        push   edx
        push   ebx
        db  0CDh, 020h, 078h, 000h, 001h, 000h
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_13_0018
        mov    word ptr [ebp + 1ch], ax
        jmp    SHORT loc_13_001D
loc_13_0063:
        movzx  ecx, word ptr [ebp + 10h]
        and    ecx, 0fffffffch
        db  0E8h, 060h, 06Dh, 0F3h, 0FFh
        and    eax, 0fffffffch
        cmp    ax, cx
        jne    SHORT loc_13_0079
        jmp    SHORT loc_13_0018
loc_13_0079:
        push   0
        push   ecx
        push   ebx
        db  0CDh, 020h, 079h, 000h, 001h, 000h
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_13_0018
        mov    ax, word ptr [ebp + 10h]
        cmp    ax, word ptr [ebp + 3ch]
        jne    SHORT loc_13_009A
        mov    word ptr [ebp + 3ch], 0
loc_13_009A:
        cmp    ax, word ptr [ebp + 38h]
        jne    NEAR PTR loc_13_001D
        mov    word ptr [ebp + 38h], 0
        jmp    NEAR PTR loc_13_001D
loc_13_00AF:
        movzx  eax, word ptr [ebp + 10h]
        shl    eax, 4
        mov    ecx, 0ffffh
        db  0CDh, 020h, 01Dh, 000h, 001h, 000h
        jb     NEAR PTR loc_13_0018
        mov    word ptr [ebp + 1ch], cx
        jmp    NEAR PTR loc_13_001D
loc_13_00D0:
        mov    word ptr [ebp + 1ch], 8
        jmp    NEAR PTR loc_13_001D
loc_13_00DB:
        call   sub_13_0125
        jb     NEAR PTR loc_13_0018
        push   0
        push   esi
        push   eax
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     NEAR PTR loc_13_0018
        jmp    NEAR PTR loc_13_001D
loc_13_0100:
        call   sub_13_0125
        jb     NEAR PTR loc_13_0018
        push   0
        push   esi
        push   eax
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     NEAR PTR loc_13_0018
        jmp    NEAR PTR loc_13_001D
sub_13_0125:
        movzx  esi, word ptr [ebp + 10h]
        test   esi, 4
        je     SHORT loc_13_0150
        push   0
        push   esi
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        lsl    esi, esi
        jne    SHORT loc_13_0150
        add    esi, eax
        shr    eax, 0ch
        shr    esi, 0ch
        sub    esi, eax
        inc    esi
        clc
        ret
loc_13_0150:
        stc
        ret
loc_13_0152:
        movzx  eax, word ptr [ebp + 10h]
        test   eax, 4
        je     NEAR PTR loc_13_0018
        push   0
        push   eax
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, -1
        je     NEAR PTR loc_13_0018
        mov    esi, dword ptr [ebx + 4]
        cmp    eax, esi
        jb     SHORT loc_13_018A
        lea    ecx, [esi + 3fffffh]
        cmp    eax, ecx
        ja     SHORT loc_13_018A
        sub    eax, esi
loc_13_018A:
        mov    word ptr [ebp + 14h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 18h], ax
        jmp    NEAR PTR loc_13_001D
loc_13_019A:
        movzx  edi, word ptr [ebp + 10h]
        test   edi, 4
        je     NEAR PTR loc_13_0018
        push   0
        push   ebx
        push   edi
        db  0CDh, 020h, 07Bh, 000h, 001h, 000h
        add    esp, 0ch
        mov    ecx, eax
        or     ecx, edx
        je     NEAR PTR loc_13_0018
        ror    eax, 10h
        mov    ax, word ptr [ebp + 14h]
        ror    eax, 10h
        mov    dl, byte ptr [ebp + 18h]
        rol    edx, 8
        mov    dl, byte ptr [ebp + 19h]
        ror    edx, 8
        push   0
        push   eax
        push   edx
        push   ebx
        push   edi
        db  0CDh, 020h, 07Ch, 000h, 001h, 000h
        add    esp, 14h
        test   eax, eax
        je     NEAR PTR loc_13_0018
        jmp    NEAR PTR loc_13_001D
loc_13_01F3:
        mov    si, word ptr [ebp + 18h]
        shl    esi, 10h
        mov    si, word ptr [ebp + 14h]
        movzx  edi, word ptr [ebp + 10h]
        test   edi, 4
        je     NEAR PTR loc_13_0018
        push   0
        push   ebx
        push   edi
        db  0CDh, 020h, 07Bh, 000h, 001h, 000h
        add    esp, 0ch
        mov    ecx, eax
        or     ecx, edx
        je     NEAR PTR loc_13_0018
        cmp    esi, 100000h
        jb     SHORT loc_13_024A
        push   esi
        and    esi, 0fffh
        cmp    esi, 0fffh
        pop    esi
        jne    NEAR PTR loc_13_0018
        shr    esi, 0ch
        bts    edx, 17h
        jmp    SHORT loc_13_024E
loc_13_024A:
        btr    edx, 17h
loc_13_024E:
        mov    ax, si
        and    esi, 0f0000h
        and    edx, 0fff0ffffh
        or     edx, esi
        push   0
        push   eax
        push   edx
        push   ebx
        push   edi
        db  0CDh, 020h, 07Ch, 000h, 001h, 000h
        add    esp, 14h
        test   eax, eax
        je     NEAR PTR loc_13_0018
        jmp    NEAR PTR loc_13_001D
loc_13_027B:
        movzx  edi, word ptr [ebp + 10h]
        test   edi, 4
        je     NEAR PTR loc_13_0018
        push   0
        push   ebx
        push   edi
        db  0CDh, 020h, 07Bh, 000h, 001h, 000h
        add    esp, 0ch
        mov    ecx, eax
        or     ecx, edx
        je     NEAR PTR loc_13_0018
        mov    ecx, dword ptr [ebp + 18h]
        test   ecx, 10h
        je     NEAR PTR loc_13_0018
        or     cl, 60h
        mov    dh, cl
        and    ecx, 0f000h
        shl    ecx, 8
        and    edx, 0ff0fffffh
        or     edx, ecx
        push   0
        push   eax
        push   edx
        push   ebx
        push   edi
        db  0CDh, 020h, 07Ch, 000h, 001h, 000h
        add    esp, 14h
        test   eax, eax
        je     NEAR PTR loc_13_0018
        jmp    NEAR PTR loc_13_001D
loc_13_02E3:
        movzx  edi, word ptr [ebp + 10h]
        push   0
        push   ebx
        push   edi
        db  0CDh, 020h, 07Bh, 000h, 001h, 000h
        add    esp, 0ch
        mov    ecx, eax
        or     ecx, edx
        je     NEAR PTR loc_13_0018
        and    dh, 0f0h
        or     dh, 2
        push   0
        push   1
        push   eax
        push   edx
        push   ebx
        db  0CDh, 020h, 078h, 000h, 001h, 000h
        add    esp, 14h
        test   eax, eax
        je     NEAR PTR loc_13_0018
        mov    word ptr [ebp + 1ch], ax
        jmp    NEAR PTR loc_13_001D
loc_13_0325:
        movzx  eax, word ptr [ebp + 10h]
        test   eax, 4
        je     NEAR PTR loc_13_0018
        push   0
        push   ebx
        push   eax
        db  0CDh, 020h, 07Bh, 000h, 001h, 000h
        add    esp, 0ch
        mov    ecx, eax
        or     ecx, edx
        je     NEAR PTR loc_13_0018
        xchg   edi, eax
        mov    ax, 3800h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        cld
        stosd
        mov    dword ptr [edi], edx
        jmp    NEAR PTR loc_13_001D
loc_13_0360:
        mov    ax, 3800h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
        cld
        lodsd
        mov    edx, dword ptr [esi]
        test   dh, 10h
        je     NEAR PTR loc_13_0018
        mov    esi, edx
        shr    esi, 0dh
        and    esi, 3
        cmp    esi, 3
        jne    NEAR PTR loc_13_0018
        movzx  ecx, word ptr [ebp + 10h]
        test   ecx, 4
        je     NEAR PTR loc_13_0018
        push   0
        push   eax
        push   edx
        push   ebx
        push   ecx
        db  0CDh, 020h, 07Ch, 000h, 001h, 000h
        add    esp, 14h
        test   eax, eax
        je     NEAR PTR loc_13_0018
        jmp    NEAR PTR loc_13_001D
loc_13_03B5:
        push   0
        push   0
        push   0f2h
        push   0
        push   0
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        movzx  ecx, word ptr [ebp + 10h]
        push   1
        push   ecx
        push   eax
        push   edx
        push   ebx
        db  0CDh, 020h, 078h, 000h, 001h, 000h
        add    esp, 14h
        test   eax, eax
        je     NEAR PTR loc_13_0018
        jmp    NEAR PTR loc_13_001D
sub_13_03EB:
        xor    edi, edi
        inc    edi
        test   word ptr [ebx], 180h
        jne    SHORT loc_13_03FC
        lsl    edi, esi
        shr    edi, 10h
        inc    edi
loc_13_03FC:
        push   0
        push   esi
        push   ebx
        db  0CDh, 020h, 079h, 000h, 001h, 000h
        add    esp, 0ch
        add    esi, 8
        dec    edi
        jne    SHORT loc_13_03FC
        ret
loc_13_0410:
        cmp    al, 83h
        je     NEAR PTR loc_13_04F9
        cmp    al, 5
        ja     NEAR PTR loc_13_0018
        je     NEAR PTR loc_13_0555
        cmp    al, 3
        ja     NEAR PTR loc_13_0530
        je     NEAR PTR loc_13_04ED
        cmp    al, 1
        ja     SHORT loc_13_0470
        je     SHORT loc_13_0455
        movzx  eax, byte ptr [ebp + 10h]
        mov    ecx, dword ptr [eax*4]
        mov    word ptr [ebp + 14h], cx
        shr    ecx, 10h
        mov    word ptr [ebp + 18h], cx
        jmp    NEAR PTR loc_13_001D
loc_13_0455:
        mov    cx, word ptr [ebp + 18h]
        shl    ecx, 10h
        mov    cx, word ptr [ebp + 14h]
        movzx  eax, byte ptr [ebp + 10h]
        mov    dword ptr [eax*4], ecx
        jmp    NEAR PTR loc_13_001D
loc_13_0470:
        movzx  edx, byte ptr [ebp + 10h]
        cmp    edx, 20h
        jae    NEAR PTR loc_13_0018
        lea    edi, [edx + edx*2]
        shl    edi, 1
        cmp    word ptr [edi + ebx + 0bch], 0
        jne    SHORT loc_13_04C5
        db  08Bh, 004h, 095h, 000h, 000h, 000h, 000h
        test   eax, eax
        jne    SHORT loc_13_04B0
        db  0BEh, 005h, 007h, 000h, 000h
        db  0CDh, 020h, 00Ch, 000h, 001h, 000h
        jb     NEAR PTR loc_13_0018
        db  089h, 004h, 095h, 000h, 000h, 000h, 000h
loc_13_04B0:
        movzx  esi, ax
        shr    eax, 10h
        mov    word ptr [edi + ebx + 0bch], ax
        mov    dword ptr [edi + ebx + 0b8h], esi
loc_13_04C5:
        mov    eax, dword ptr [edi + ebx + 0b8h]
        test   word ptr [ebx], 180h
        je     SHORT loc_13_04D8
        mov    dword ptr [ebp + 14h], eax
        jmp    SHORT loc_13_04DC
loc_13_04D8:
        mov    word ptr [ebp + 14h], ax
loc_13_04DC:
        mov    ax, word ptr [edi + ebx + 0bch]
        mov    word ptr [ebp + 18h], ax
        jmp    NEAR PTR loc_13_001D
loc_13_04ED:
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     NEAR PTR loc_13_0018
loc_13_04F9:
        movzx  edx, byte ptr [ebp + 10h]
        cmp    edx, 20h
        jae    NEAR PTR loc_13_0018
        lea    edi, [edx + edx*2]
        shl    edi, 1
        mov    eax, dword ptr [ebp + 14h]
        test   word ptr [ebx], 180h
        jne    SHORT loc_13_0518
        movzx  eax, ax
loc_13_0518:
        mov    dword ptr [edi + ebx + 0b8h], eax
        mov    ax, word ptr [ebp + 18h]
        mov    word ptr [edi + ebx + 0bch], ax
        jmp    NEAR PTR loc_13_001D
loc_13_0530:
        movzx  eax, byte ptr [ebp + 10h]
        db  0CDh, 020h, 044h, 000h, 001h, 000h
        mov    word ptr [ebp + 18h], cx
        mov    word ptr [ebp + 14h], dx
        test   word ptr [ebx], 180h
        je     NEAR PTR loc_13_001D
        mov    dword ptr [ebp + 14h], edx
        jmp    NEAR PTR loc_13_001D
loc_13_0555:
        movzx  eax, byte ptr [ebp + 10h]
        movzx  ecx, word ptr [ebp + 18h]
        mov    edx, dword ptr [ebp + 14h]
        test   word ptr [ebx], 180h
        jne    SHORT loc_13_0575
loc_13_0567:
        movzx  edx, dx
loc_13_056A:
        db  0CDh, 020h, 045h, 000h, 001h, 000h
        jmp    NEAR PTR loc_13_001D
loc_13_0575:
        lsl    esi, ecx
        cmp    edx, esi
        jb     SHORT loc_13_056A
        cmp    esi, 0ffffh
        jbe    SHORT loc_13_0567
        jmp    SHORT loc_13_056A
loc_13_0586:
        dec    dword ptr [ebx + 1bch]
        js     SHORT loc_13_05B1
        mov    eax, dword ptr [ebp + 2ch]
        or     ah, 2
        and    ah, 0feh
        mov    dword ptr [ebp + 2ch], eax
        ret
loc_13_059B:
        test   byte ptr [ebx + 1dh], 2
        je     SHORT loc_13_05C3
        test   byte ptr [ebp + 2dh], 2
        je     SHORT loc_13_05C3
        or     byte ptr [ebp + 2dh], 1
        inc    dword ptr [ebx + 1bch]
loc_13_05B1:
        inc    dword ptr [ebx + 1bch]
        jmp    SHORT loc_13_05C3
loc_13_05B9:
        cmp    esi, 4
        je     SHORT loc_13_0586
        cmp    esi, 2ch
        je     SHORT loc_13_059B
loc_13_05C3:
        mov    edx, esi
        shr    edx, 2
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        and    byte ptr [edi + 2], 7fh
        lea    edi, [edx + edx*2]
        shl    edi, 1
        cmp    word ptr [edi + ebx + 0bch], 0
        je     NEAR PTR loc_13_06E1
        push   edx
        mov    eax, dword ptr [ebp + 34h]
        mov    ecx, dword ptr [ebp + 30h]
        mov    edx, dword ptr [ebp + 28h]
        mov    esi, dword ptr [ebp + 24h]
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        sub    dword ptr [ebp + 30h], 20h
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    eax, ecx
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 2ch]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        db  0CDh, 020h, 01Bh, 000h, 001h, 000h
        and    byte ptr [ebp + 2dh], 0feh
        mov    eax, edx
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    eax, esi
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 20h]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    edx, dword ptr [edi + ebx + 0b8h]
        movzx  ecx, word ptr [edi + ebx + 0bch]
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        push   eax
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        push   eax
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        mov    ecx, dword ptr [ebp + 2ch]
        and    ecx, 17200h
        mov    edx, eax
        and    edx, 0fffe8dffh
        or     ecx, edx
        mov    dword ptr [ebp + 2ch], ecx
        test   eax, 200h
        je     SHORT loc_13_0691
        db  0CDh, 020h, 01Ah, 000h, 001h, 000h
        jmp    SHORT loc_13_0697
loc_13_0691:
        db  0CDh, 020h, 01Bh, 000h, 001h, 000h
loc_13_0697:
        pop    edx
        pop    ecx
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        mov    esi, eax
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        add    dword ptr [ebp + 30h], 20h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        test   byte ptr [ebx], 80h
        jne    SHORT loc_13_06C3
        lar    edi, edx
        test   edi, 400000h
        je     SHORT loc_13_06C3
        jmp    SHORT loc_13_06D1
loc_13_06C3:
        mov    dword ptr [ebp + 24h], ecx
        mov    word ptr [ebp + 28h], dx
        mov    dword ptr [ebp + 30h], esi
        mov    word ptr [ebp + 34h], ax
loc_13_06D1:
        pop    edx
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        test   dword ptr [edi], 800000h
        jne    SHORT loc_13_06EB
loc_13_06E0:
        ret
loc_13_06E1:
        cmp    edx, 6
        jb     SHORT loc_13_06FD
        cmp    edx, 7
        je     SHORT loc_13_06FD
loc_13_06EB:
        mov    esi, edx
        mov    cx, 12h
        db  0E8h, 01Ah, 02Dh, 0F3h, 0FFh
        jb     SHORT loc_13_06E0
        db  0E9h, 041h, 02Ch, 0F3h, 0FFh
loc_13_06FD:
        mov    eax, edx
        db  0CDh, 020h, 046h, 080h, 001h, 000h
loc_13_0705:
        push   fs
        push   gs
        cmp    edx, 7
        ja     NEAR PTR loc_13_07DE
        cmp    edx, 6
        je     NEAR PTR loc_13_07DE
        mov    ax, word ptr [ebp + 34h]
        mov    esi, dword ptr [ebp + 30h]
        mov    fs, ax
        lsl    eax, eax
        cmp    eax, 10000h
        jae    SHORT loc_13_0732
        movzx  esi, si
loc_13_0732:
        mov    eax, edx
        db  0CDh, 020h, 044h, 000h, 001h, 000h
        test   word ptr [ebx], 180h
        jne    SHORT loc_13_078F
        push   eax
        lar    eax, word ptr fs:[esi + 0eh]
        lar    edi, word ptr fs:[esi + 8]
        or     edi, eax
        test   edi, 400000h
        pop    eax
        jne    SHORT loc_13_07CA
        movzx  edi, word ptr fs:[esi + 0ch]
        mov    gs, word ptr fs:[esi + 0eh]
        sub    edi, 6
        mov    word ptr fs:[esi + 0ch], di
        xchg   word ptr fs:[esi + 6], dx
        mov    word ptr gs:[edi], dx
        xchg   word ptr fs:[esi + 8], cx
        mov    word ptr gs:[edi + 2], cx
        mov    ax, word ptr fs:[esi + 0ah]
        mov    word ptr gs:[edi + 4], ax
        and    word ptr fs:[esi + 0ah], 0fdffh
        jmp    SHORT loc_13_07C0
loc_13_078F:
        mov    gs, word ptr fs:[esi + 1ch]
        mov    edi, dword ptr fs:[esi + 18h]
        sub    edi, 0ch
        mov    dword ptr fs:[esi + 18h], edi
        xchg   dword ptr fs:[esi + 0ch], edx
        mov    dword ptr gs:[edi], edx
        xchg   word ptr fs:[esi + 10h], cx
        mov    word ptr gs:[edi + 4], cx
        mov    eax, dword ptr fs:[esi + 14h]
        mov    dword ptr gs:[edi + 8], eax
        and    dword ptr fs:[esi + 14h], 0fffffdffh
loc_13_07C0:
        pop    gs
        pop    fs
        db  0CDh, 020h, 04Ah, 080h, 001h, 000h
loc_13_07CA:
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        jmp    SHORT loc_13_07C0
loc_13_07DE:
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        or     dword ptr [edi], 800000h
        jmp    SHORT loc_13_07C0
loc_13_07EC:
        cmp    al, 6
        ja     NEAR PTR loc_13_0018
        je     NEAR PTR loc_13_0A06
        cmp    al, 4
        ja     NEAR PTR loc_13_09CA
        je     NEAR PTR loc_13_0987
        cmp    al, 3
        je     NEAR PTR loc_13_08B8
        mov    bl, al
        mov    ax, 3800h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edx, eax
        mov    bh, byte ptr [ebp + 10h]
        shl    ebx, 10h
        mov    bx, word ptr [ebp + 18h]
        sub    esp, 6ch
        mov    edi, esp
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        push   esi
        call   sub_13_0A47
        movzx  ecx, bx
        jecxz  loc_13_0866
        add    bx, bx
        sub    word ptr [ebp + 30h], bx
        mov    esi, edx
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        cld
        rep movsw
loc_13_0866:
        pop    esi
        shr    ebx, 10h
        cmp    bl, 0
        je     SHORT loc_13_088C
        movzx  ecx, word ptr [esi + 2ch]
        movzx  edx, word ptr [esi + 2ah]
        cmp    bl, 1
        je     SHORT loc_13_0884
        db  0CDh, 020h, 04Ch, 000h, 001h, 000h
        jmp    SHORT loc_13_0895
loc_13_0884:
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        jmp    SHORT loc_13_0895
loc_13_088C:
        movzx  eax, bh
        db  0CDh, 020h, 046h, 000h, 001h, 000h
loc_13_0895:
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        mov    edi, esi
        call   sub_13_0A28
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        mov    esi, esp
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        add    esp, 6ch
        jmp    NEAR PTR loc_13_001D
loc_13_08B8:
        db  08Bh, 035h, 080h, 000h, 000h, 000h
        db  0CDh, 020h, 0A5h, 000h, 001h, 000h
        jne    SHORT loc_13_08E3
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     NEAR PTR loc_13_0018
        push   eax
        mov    edx, eax
        db  0BEh, 0FCh, 00Ah, 000h, 000h
        db  0CDh, 020h, 00Bh, 000h, 001h, 000h
        mov    dword ptr [edx], eax
        pop    eax
loc_13_08E3:
        mov    esi, dword ptr [ebx - 2ch]
        test   esi, esi
        jne    SHORT loc_13_0901
        push   eax
        mov    eax, 4
        mov    ecx, 12h
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        pop    eax
        jb     SHORT loc_13_0976
        mov    dword ptr [ebx - 2ch], esi
loc_13_0901:
        mov    edi, eax
        push   0
        push   0
        push   0f2h
        push   0ffffh
        push   0
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        push   0
        push   1
        push   eax
        push   edx
        push   ebx
        db  0CDh, 020h, 078h, 000h, 001h, 000h
        add    esp, 14h
        xchg   edi, eax
        test   edi, edi
        je     SHORT loc_13_0976
        db  0CDh, 020h, 09Eh, 000h, 001h, 000h
        mov    word ptr [eax + 10h], di
        mov    esi, dword ptr [ebp + 4]
        mov    edi, dword ptr [ebp]
        test   word ptr [ebx], 180h
        jne    SHORT loc_13_094E
        movzx  esi, si
        movzx  edi, di
loc_13_094E:
        mov    dword ptr [eax + 4], esi
        mov    dword ptr [eax + 8], edi
        mov    di, word ptr [ebp + 3ch]
        mov    word ptr [eax + 0ch], di
        mov    di, word ptr [ebp + 38h]
        mov    word ptr [eax + 0eh], di
        mov    edi, dword ptr [eax]
        mov    word ptr [ebp + 14h], di
        shr    edi, 10h
        mov    word ptr [ebp + 18h], di
        jmp    NEAR PTR loc_13_001D
loc_13_0976:
        db  08Bh, 035h, 080h, 000h, 000h, 000h
        db  0CDh, 020h, 09Eh, 000h, 001h, 000h
        jmp    NEAR PTR loc_13_0018
loc_13_0987:
        mov    esi, dword ptr [ebx - 2ch]
        test   esi, esi
        je     NEAR PTR loc_13_0018
        mov    cx, word ptr [ebp + 18h]
        shl    ecx, 10h
        mov    cx, word ptr [ebp + 14h]
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     NEAR PTR loc_13_0018
loc_13_09A9:
        cmp    dword ptr [eax], ecx
        je     SHORT loc_13_09BA
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jne    SHORT loc_13_09A9
        jmp    NEAR PTR loc_13_0018
loc_13_09BA:
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        call   sub_13_0C2B
        jmp    NEAR PTR loc_13_001D
loc_13_09CA:
        mov    word ptr [ebp + 1ch], 24h
        db  0A1h, 090h, 000h, 000h, 000h
        mov    word ptr [ebp + 18h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 10h], ax
        db  0A1h, 094h, 000h, 000h, 000h
        mov    word ptr [ebp], ax
        shr    eax, 10h
        mov    word ptr [ebp + 4], ax
loc_13_09F0:
        test   word ptr [ebx], 180h
        je     NEAR PTR loc_13_001D
        mov    word ptr [ebp + 2], 0
        jmp    NEAR PTR loc_13_001D
loc_13_0A06:
        db  0A1h, 088h, 000h, 000h, 000h
        mov    word ptr [ebp + 18h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 10h], ax
        db  0A1h, 08Ch, 000h, 000h, 000h
        mov    word ptr [ebp], ax
        shr    eax, 10h
        mov    word ptr [ebp + 4], ax
        jmp    SHORT loc_13_09F0
sub_13_0A28:
        lea    esi, [ebp]
        mov    ecx, 8
        cld
        rep movsd
        mov    ax, word ptr [ebp + 2ch]
        stosw
        lea    esi, [ebp + 38h]
        mov    ecx, 4
loc_13_0A41:
        lodsd
        stosw
        loop   loc_13_0A41
        ret
sub_13_0A47:
        mov    edi, ebp
        mov    ecx, 8
        cld
        rep movsd
        xor    eax, eax
        lodsw
        mov    ecx, eax
        and    word ptr [ebp + 2ch], 7200h
        and    ax, 8dffh
        or     word ptr [ebp + 2ch], ax
        test   cx, 200h
        je     SHORT loc_13_0A74
        db  0CDh, 020h, 01Ah, 000h, 001h, 000h
        jmp    SHORT loc_13_0A7A
loc_13_0A74:
        db  0CDh, 020h, 01Bh, 000h, 001h, 000h
loc_13_0A7A:
        lea    edi, [ebp + 38h]
        mov    ecx, 4
loc_13_0A82:
        lodsw
        stosd
        loop   loc_13_0A82
        add    esi, 4
        mov    ecx, dword ptr [esi]
        jecxz  loc_13_0A99
        mov    word ptr [ebp + 30h], cx
        shr    ecx, 10h
        mov    word ptr [ebp + 34h], cx
loc_13_0A99:
        ret
        db  08Bh, 0C0h
loc_13_0A9C:
        mov    ax, 3800h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        lea    esi, [ebp + 48h]
        cmp    byte ptr [ebp + 1ch], 0
        je     SHORT loc_13_0AB2
        xchg   edi, esi
loc_13_0AB2:
        mov    ecx, 9
        cld
        rep movsd
        db  0CDh, 020h, 04Ah, 080h, 001h, 000h
loc_13_0AC0:
        db  0CDh, 020h, 08Ah, 000h, 001h, 000h
        test   word ptr [ebx], 180h
        jne    SHORT loc_13_0AD5
loc_13_0ACD:
        mov    word ptr [ebp + 2], dx
        mov    word ptr [ebp + 12h], dx
loc_13_0AD5:
        mov    word ptr [ebp + 40h], dx
        mov    word ptr [ebp + 44h], dx
        lea    esi, [ebp]
        lea    edi, [ebp + 24h]
        cld
        movsd
        movsd
        add    esi, 8
        add    edi, 4
        mov    ecx, 4
        rep movsd
        ret
loc_13_0AF4:
        db  0CDh, 020h, 089h, 000h, 001h, 000h
        jmp    SHORT loc_13_0ACD
loc_13_0AFC:
        mov    edi, edx
        movzx  eax, word ptr [edi + 0eh]
        push   0
        push   eax
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        mov    esi, eax
        add    esi, dword ptr [edi + 8]
        pushad
        mov    edi, esi
        call   sub_13_0A28
        popad
        mov    ax, word ptr [ebp + 28h]
        mov    word ptr [esi + 2ch], ax
        mov    ax, word ptr [ebp + 24h]
        mov    word ptr [esi + 2ah], ax
        mov    ax, word ptr [ebp + 34h]
        mov    word ptr [esi + 30h], ax
        mov    ax, word ptr [ebp + 30h]
        mov    word ptr [esi + 2eh], ax
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        push   dword ptr [ebp + 3ch]
        push   dword ptr [ebp + 38h]
        push   dword ptr [ebp + 40h]
        push   dword ptr [ebp + 44h]
        movzx  ecx, word ptr [esi + 30h]
        shl    ecx, 4
        push   ecx
        movzx  ecx, word ptr [edi + 10h]
        push   0
        push   ebx
        push   ecx
        db  0CDh, 020h, 07Bh, 000h, 001h, 000h
        add    esp, 0ch
        pop    ecx
        push   eax
        push   edx
        ror    eax, 10h
        mov    ax, cx
        ror    eax, 10h
        shr    ecx, 10h
        mov    dl, cl
        rol    edx, 8
        mov    dl, ch
        ror    edx, 8
        movzx  ecx, word ptr [edi + 10h]
        mov    word ptr [ebp + 3ch], cx
        push   0
        push   eax
        push   edx
        push   ebx
        push   ecx
        db  0CDh, 020h, 07Ch, 000h, 001h, 000h
        add    esp, 14h
        movzx  ecx, word ptr [esi + 2eh]
        mov    dword ptr [ebp + 4], ecx
        mov    cx, word ptr [edi + 0eh]
        mov    word ptr [ebp + 38h], cx
        mov    ecx, dword ptr [edi + 8]
        mov    dword ptr [ebp], ecx
        mov    cx, word ptr [edi + 0ch]
        mov    edx, dword ptr [edi + 4]
        db  0CDh, 020h, 04Ch, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        pop    edx
        pop    eax
        movzx  ecx, word ptr [edi + 10h]
        push   0
        push   eax
        push   edx
        push   ebx
        push   ecx
        db  0CDh, 020h, 07Ch, 000h, 001h, 000h
        add    esp, 14h
        mov    esi, dword ptr [ebp]
        test   word ptr [ebx], 180h
        jne    SHORT loc_13_0BE2
        movzx  esi, si
loc_13_0BE2:
        push   0
        push   dword ptr [ebp + 38h]
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        add    esi, eax
        pop    dword ptr [ebp + 44h]
        pop    dword ptr [ebp + 40h]
        pop    dword ptr [ebp + 38h]
        pop    dword ptr [ebp + 3ch]
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        pushad
        call   sub_13_0A47
        popad
        mov    ax, word ptr [esi + 2ch]
        mov    word ptr [ebp + 28h], ax
        movzx  eax, word ptr [esi + 2ah]
        mov    dword ptr [ebp + 24h], eax
        mov    ax, word ptr [esi + 30h]
        mov    word ptr [ebp + 34h], ax
        movzx  eax, word ptr [esi + 2eh]
        mov    dword ptr [ebp + 30h], eax
        ret
sub_13_0C2B:
        pushad
        db  08Bh, 035h, 080h, 000h, 000h, 000h
        db  0CDh, 020h, 09Eh, 000h, 001h, 000h
        movzx  eax, word ptr [eax + 10h]
        push   0
        push   eax
        push   ebx
        db  0CDh, 020h, 079h, 000h, 001h, 000h
        add    esp, 0ch
        popad
        ret
loc_13_0C4B:
        test   al, al
        jne    NEAR PTR loc_13_0018
        mov    word ptr [ebp + 1ch], 5ah
        mov    word ptr [ebp + 10h], 5
        mov    byte ptr [ebp + 19h], 0
        xor    eax, eax
        db  0CDh, 020h, 00Dh, 000h, 003h, 000h
        mov    byte ptr [ebp + 15h], al
        mov    byte ptr [ebp + 14h], 0
        mov    eax, 8
        db  0CDh, 020h, 00Dh, 000h, 003h, 000h
        jb     SHORT loc_13_0C82
        mov    byte ptr [ebp + 14h], al
loc_13_0C82:
        mov    byte ptr [ebp + 18h], 3
        db  0CDh, 020h, 0BBh, 000h, 001h, 000h
        test   ebx, 100000h
        je     SHORT loc_13_0C98
        or     byte ptr [ebp + 19h], 1
loc_13_0C98:
        test   ebx, 10000h
        je     SHORT loc_13_0CA3
        inc    byte ptr [ebp + 18h]
loc_13_0CA3:
        db  0CDh, 020h, 000h, 000h, 014h, 000h
        test   eax, eax
        je     NEAR PTR loc_13_001D
        test   ebx, 1
        je     NEAR PTR loc_13_001D
        or     word ptr [ebp + 10h], 8
        jmp    NEAR PTR loc_13_001D
loc_13_0CC7:
        cmp    al, 3
        ja     NEAR PTR loc_13_0018
        je     NEAR PTR loc_13_0EBF
        cmp    al, 1
        je     NEAR PTR loc_13_0D95
        ja     NEAR PTR loc_13_0E3B
        mov    ax, 3800h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        push   0
        db  0CDh, 020h, 059h, 000h, 001h, 000h
        pop    ecx
        mov    esi, eax
        shr    esi, 3
        sub    eax, esi
        db  08Bh, 035h, 018h, 0EFh, 000h, 000h
        shl    esi, 1
        cmp    eax, esi
        jb     SHORT loc_13_0D0C
        mov    eax, esi
loc_13_0D0C:
        mov    ecx, dword ptr [ebx + 34h]
        cmp    eax, ecx
        jb     SHORT loc_13_0D15
        mov    eax, ecx
loc_13_0D15:
        cmp    edx, ecx
        jb     SHORT loc_13_0D1B
        mov    edx, ecx
loc_13_0D1B:
        push   eax
        cld
        shl    eax, 0ch
        stosd
        shr    eax, 0ch
        stosd
        mov    eax, edx
        stosd
        sub    esp, 40h
        mov    esi, esp
        push   0
        push   esi
        db  0CDh, 020h, 06Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        cld
        mov    eax, dword ptr [esi + 2ch]
        add    eax, dword ptr [esi + 4]
        stosd
        mov    eax, dword ptr [esi + 0ch]
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_13_0D51
        stosd
        mov    eax, dword ptr [esi + 8]
        jmp    SHORT loc_13_0D66
loc_13_0D51:
        cmp    eax, 200h
        jae    SHORT loc_13_0D5D
        mov    eax, 200h
loc_13_0D5D:
        cmp    eax, dword ptr [ebx + 34h]
        jbe    SHORT loc_13_0D65
        mov    eax, dword ptr [ebx + 34h]
loc_13_0D65:
        stosd
loc_13_0D66:
        stosd
        mov    eax, dword ptr [esi + 4]
        stosd
        mov    esi, dword ptr [esi + 2ch]
        add    esp, 40h
        push   80000h
        call   sub_13_25E0
        pop    ecx
        cmp    eax, ecx
        jbe    SHORT loc_13_0D82
        mov    eax, ecx
loc_13_0D82:
        stosd
        mov    eax, esi
        stosd
        mov    ecx, 3
        or     eax, 0ffffffffh
        rep stosd
        jmp    NEAR PTR loc_13_001D
loc_13_0D95:
        mov    edi, dword ptr [ebp + 0eh]
        mov    di, word ptr [ebp + 18h]
        add    edi, 0fffh
        shr    edi, 0ch
        xor    eax, eax
        mov    esi, dword ptr [ebx - 3ch]
        db  003h, 035h, 084h, 000h, 000h, 000h
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        je     SHORT loc_13_0DC7
        cmp    dword ptr [ebx + 34h], edi
        jb     SHORT loc_13_0DD9
        mov    edx, edi
        dec    edx
        je     SHORT loc_13_0E14
loc_13_0DC2:
        call   sub_13_0F49
loc_13_0DC7:
        push   eax
        push   eax
        push   eax
        push   eax
        push   eax
        push   ebx
        push   0
        push   edi
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
loc_13_0DD9:
        test   eax, eax
        je     SHORT loc_13_0E36
loc_13_0DDD:
        sub    dword ptr [ebx + 34h], edi
        mov    esi, dword ptr [esi]
        mov    ecx, eax
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     SHORT loc_13_0E2A
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 4], edi
        db  0CDh, 020h, 09Eh, 000h, 001h, 000h
        mov    eax, ecx
        mov    word ptr [ebp], ax
        shr    eax, 10h
        mov    word ptr [ebp + 4], ax
        mov    word ptr [ebp + 18h], dx
        shr    edx, 10h
        mov    word ptr [ebp + 10h], dx
        jmp    NEAR PTR loc_13_001D
loc_13_0E14:
        cmp    dword ptr [esi + edx*4 + 14h], eax
        jne    SHORT loc_13_0E22
        inc    edx
        cmp    dl, 4
        jb     SHORT loc_13_0E14
        jmp    SHORT loc_13_0DC2
loc_13_0E22:
        xchg   dword ptr [esi + edx*4 + 14h], eax
        mov    edx, eax
        jmp    SHORT loc_13_0DDD
loc_13_0E2A:
        pop    eax
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_13_0E36:
        jmp    NEAR PTR loc_13_0018
loc_13_0E3B:
        mov    cx, word ptr [ebp + 4]
        shl    ecx, 10h
        mov    cx, word ptr [ebp]
        db  0CDh, 020h, 088h, 000h, 001h, 000h
        db  003h, 03Dh, 084h, 000h, 000h, 000h
        mov    esi, dword ptr [edi]
        call   sub_13_0F7D
        jb     SHORT loc_13_0E36
        mov    edx, dword ptr [eax + 4]
        add    dword ptr [ebx + 34h], edx
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        jne    SHORT loc_13_0E89
        push   0
        push   ecx
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        test   eax, eax
        je     SHORT loc_13_0E36
loc_13_0E84:
        jmp    NEAR PTR loc_13_001D
loc_13_0E89:
        dec    edx
        jne    SHORT loc_13_0EA2
loc_13_0E8C:
        xor    eax, eax
        cmp    dword ptr [edi + edx*4 + 14h], eax
        je     SHORT loc_13_0E9C
        inc    edx
        cmp    dl, 4
        jb     SHORT loc_13_0E8C
        jmp    SHORT loc_13_0EA2
loc_13_0E9C:
        mov    dword ptr [edi + edx*4 + 14h], ecx
        jmp    SHORT loc_13_0E84
loc_13_0EA2:
        call   sub_13_0F49
        mov    dword ptr [edi + 0ch], ecx
        mov    eax, 3e8h
        db  0BEh, 036h, 00Fh, 000h, 000h
        db  0CDh, 020h, 03Dh, 000h, 001h, 000h
        mov    dword ptr [edi + 10h], esi
        jmp    SHORT loc_13_0E84
loc_13_0EBF:
        mov    ecx, dword ptr [ebp + 2]
        mov    cx, word ptr [ebp]
        db  0CDh, 020h, 088h, 000h, 001h, 000h
        db  003h, 03Dh, 084h, 000h, 000h, 000h
        mov    esi, dword ptr [edi]
        call   sub_13_0F7D
        jb     SHORT loc_13_0F31
        mov    edi, eax
        mov    esi, dword ptr [ebp + 0eh]
        mov    si, word ptr [ebp + 18h]
        add    esi, 0fffh
        shr    esi, 0ch
        je     SHORT loc_13_0F31
        xor    eax, eax
        mov    edx, esi
        sub    edx, dword ptr [edi + 4]
        cmp    dword ptr [ebx + 34h], edx
        jl     SHORT loc_13_0F07
        push   eax
        push   esi
        push   ecx
        db  0CDh, 020h, 054h, 000h, 001h, 000h
        add    esp, 0ch
loc_13_0F07:
        test   eax, eax
        je     SHORT loc_13_0F31
        mov    dword ptr [edi], eax
        xchg   dword ptr [edi + 4], esi
        sub    esi, dword ptr [edi + 4]
        add    dword ptr [ebx + 34h], esi
        mov    word ptr [ebp], ax
        shr    eax, 10h
        mov    word ptr [ebp + 4], ax
        mov    word ptr [ebp + 18h], dx
        shr    edx, 10h
        mov    word ptr [ebp + 10h], dx
        jmp    NEAR PTR loc_13_0E84
loc_13_0F31:
        jmp    NEAR PTR loc_13_0E36
loc_13_0F36:
        push   edi
        db  0CDh, 020h, 088h, 000h, 001h, 000h
        db  003h, 03Dh, 084h, 000h, 000h, 000h
        and    dword ptr [edi + 10h], 0
        jmp    SHORT loc_13_0F56
sub_13_0F49:
        push   edi
        db  0CDh, 020h, 088h, 000h, 001h, 000h
        db  003h, 03Dh, 084h, 000h, 000h, 000h
loc_13_0F56:
        cmp    dword ptr [edi + 0ch], 0
        jne    SHORT loc_13_0F5E
        pop    edi
        ret
loc_13_0F5E:
        pushad
        xor    esi, esi
        xchg   dword ptr [edi + 10h], esi
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        xor    esi, esi
        xchg   dword ptr [edi + 0ch], esi
        push   0
        push   esi
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        popad
        pop    edi
        ret
sub_13_0F7D:
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_13_0F91
loc_13_0F85:
        cmp    dword ptr [eax], ecx
        je     SHORT loc_13_0F92
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jne    SHORT loc_13_0F85
loc_13_0F91:
        stc
loc_13_0F92:
        ret
loc_13_0F93:
        cmp    al, 4
        ja     NEAR PTR loc_13_0018
        je     NEAR PTR loc_13_105F
        mov    ecx, dword ptr [ebp + 2]
        mov    cx, word ptr [ebp]
        mov    edx, dword ptr [ebp + 0eh]
        mov    dx, word ptr [ebp + 18h]
        cmp    al, 2
        jae    SHORT loc_13_1022
        test   byte ptr [ebx], 80h
        je     SHORT loc_13_0FCE
        cmp    al, 0
        jne    SHORT loc_13_0FCE
        cmp    ecx, 0a28h
        ja     SHORT loc_13_0FCE
        cmp    ch, 7
        jb     SHORT loc_13_0FCE
        call   sub_13_1070
loc_13_0FCE:
        jecxz  loc_13_101D
        lea    ecx, [edx + ecx - 1]
        shr    ecx, 0ch
        shr    edx, 0ch
        sub    ecx, edx
        inc    ecx
        db  0CDh, 020h, 088h, 000h, 001h, 000h
        db  003h, 03Dh, 084h, 000h, 000h, 000h
        test   byte ptr [edi + 8], 2
        jne    SHORT loc_13_101D
        test   byte ptr [edi + 8], 8
        je     SHORT loc_13_0FFC
        call   sub_13_10F8
        jb     SHORT loc_13_101D
loc_13_0FFC:
        push   0
        push   ecx
        push   edx
        cmp    al, 1
        je     SHORT loc_13_100C
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        jmp    SHORT loc_13_1012
loc_13_100C:
        db  0CDh, 020h, 064h, 000h, 001h, 000h
loc_13_1012:
        add    esp, 0ch
loc_13_1015:
        test   eax, eax
        je     NEAR PTR loc_13_0018
loc_13_101D:
        jmp    NEAR PTR loc_13_001D
loc_13_1022:
        lea    ecx, [edx + ecx]
        shr    ecx, 0ch
        add    edx, 0fffh
        shr    edx, 0ch
        sub    ecx, edx
        jle    SHORT loc_13_1015
        cmp    al, 3
        je     SHORT loc_13_104C
        push   200h
        push   ecx
        push   edx
        push   ebx
        db  0CDh, 020h, 065h, 000h, 001h, 000h
        add    esp, 10h
        jmp    SHORT loc_13_1015
loc_13_104C:
        push   400h
        push   ecx
        push   edx
        push   ebx
        db  0CDh, 020h, 065h, 000h, 001h, 000h
        add    esp, 10h
        jmp    SHORT loc_13_1015
loc_13_105F:
        mov    word ptr [ebp + 10h], 0
        mov    word ptr [ebp + 18h], 1000h
        jmp    NEAR PTR loc_13_001D
sub_13_1070:
        pushad
        push   0
        push   0
        push   0
        push   ecx
        push   edx
        db  0CDh, 020h, 073h, 001h, 001h, 000h
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_13_10A1
        mov    edx, dword ptr [esp + 14h]
        cmp    byte ptr [edx], 53h
        jne    SHORT loc_13_1092
        add    edx, 1ah
loc_13_1092:
        mov    eax, dword ptr [edx]
        shl    eax, 8
        mov    al, byte ptr [edx + 7]
        cmp    eax, 3d832e00h
        je     SHORT loc_13_10A3
loc_13_10A1:
        popad
        ret
loc_13_10A3:
        mov    ebp, dword ptr [edx + 3]
        add    edx, 8
        mov    ecx, 12ch
        xor    ebx, ebx
loc_13_10B0:
        jecxz  loc_13_10A1
loc_13_10B2:
        inc    edx
        cmp    dword ptr [edx], ebp
        loopne loc_13_10B2
        jne    SHORT loc_13_10A1
        mov    ax, 0dffh
        cmp    word ptr [edx - 2], ax
        jne    SHORT loc_13_10B0
        cmp    word ptr [edx + 4], 158eh
        jne    SHORT loc_13_10B0
        cmp    word ptr [edx + 0ah], 258bh
        jne    SHORT loc_13_10B0
        test   ebx, ebx
        jne    SHORT loc_13_10DB
        mov    ebx, edx
        jmp    SHORT loc_13_10B0
loc_13_10DB:
        call   sub_13_10E9
        mov    edx, ebx
        call   sub_13_10E9
        jmp    SHORT loc_13_10A1
sub_13_10E9:
        lea    edi, [edx - 2]
        lea    esi, [edi + 6]
        cld
        movsd
        movsd
        movsd
        stosw
        mov    dword ptr [edi], ebp
        ret
sub_13_10F8:
        pushad
        lea    eax, [ecx + edx]
        cmp    eax, 110h
        jbe    SHORT loc_13_1149
loc_13_1103:
        db  0CDh, 020h, 088h, 000h, 001h, 000h
        db  003h, 03Dh, 084h, 000h, 000h, 000h
        mov    esi, dword ptr [edi]
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_13_1131
loc_13_1119:
        mov    edi, dword ptr [eax]
        shr    edi, 0ch
        sub    edi, edx
        ja     SHORT loc_13_1129
        neg    edi
        sub    edi, dword ptr [eax + 4]
        jb     SHORT loc_13_1143
loc_13_1129:
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jne    SHORT loc_13_1119
loc_13_1131:
        shl    edx, 0ch
        shl    ecx, 0ch
        push   ecx
        push   edx
        db  0E8h, 02Ch, 016h, 0F3h, 0FFh
        cmp    eax, 1
        jmp    SHORT loc_13_114B
loc_13_1143:
        sub    edx, edi
        add    ecx, edi
        jg     SHORT loc_13_1103
loc_13_1149:
        or     al, 0f9h
loc_13_114B:
        popad
        ret
loc_13_114D:
        xor    edx, edx
        cmp    al, 1
        ja     SHORT loc_13_1190
        jb     SHORT loc_13_115B
        or     edx, 10000h
loc_13_115B:
        mov    ax, word ptr [ebp + 10h]
        shl    eax, 10h
        mov    ax, word ptr [ebp + 18h]
        mov    cx, word ptr [ebp + 4]
        shl    ecx, 10h
        mov    cx, word ptr [ebp]
        push   edx
        push   ecx
        db  0FFh, 035h, 00Ch, 0F7h, 000h, 000h
        push   eax
        db  0CDh, 020h, 069h, 000h, 001h, 000h
        add    esp, 10h
        test   eax, eax
        je     NEAR PTR loc_13_0018
        jmp    NEAR PTR loc_13_001D
loc_13_1190:
        cmp    al, 3
        ja     NEAR PTR loc_13_0018
        jb     SHORT loc_13_11A0
        or     edx, 10000h
loc_13_11A0:
        mov    ax, word ptr [ebp + 10h]
        shl    eax, 10h
        mov    ax, word ptr [ebp + 18h]
        mov    cx, word ptr [ebp + 4]
        shl    ecx, 10h
        mov    cx, word ptr [ebp]
        test   ecx, ecx
        je     NEAR PTR loc_13_001D
        lea    esi, [ecx + eax - 1000h]
        add    eax, 0fffh
        shr    eax, 0ch
        shr    esi, 0ch
        sub    esi, eax
        jb     NEAR PTR loc_13_001D
        inc    esi
        push   edx
        push   esi
        db  0FFh, 035h, 00Ch, 0F7h, 000h, 000h
        push   eax
        db  0CDh, 020h, 069h, 000h, 001h, 000h
        add    esp, 10h
        test   eax, eax
        je     NEAR PTR loc_13_0018
        jmp    NEAR PTR loc_13_001D
loc_13_11F8:
        cmp    al, 2
        ja     NEAR PTR loc_13_0018
        mov    ecx, dword ptr [ebp + 2ch]
        shr    ecx, 9
        and    cl, 1
        mov    byte ptr [ebp + 1ch], cl
        cmp    al, 1
        ja     NEAR PTR loc_13_001D
        jb     SHORT loc_13_121F
        or     byte ptr [ebp + 2dh], 2
        jmp    NEAR PTR loc_13_001D
loc_13_121F:
        and    byte ptr [ebp + 2dh], 0fdh
        jmp    NEAR PTR loc_13_001D
sub_13_1228:
        push   eax
        db  0CDh, 020h, 000h, 000h, 011h, 000h
        pop    eax
        db  072h, 027h
        cmp    dl, 1
        db  077h, 022h
        jb     SHORT loc_13_1243
        and    al, 3
        db  0CDh, 020h, 002h, 000h, 011h, 000h
        jmp    SHORT loc_13_1258
loc_13_1243:
        shl    cl, 4
        or     cl, cl
        je     SHORT loc_13_124D
        or     cl, 4
loc_13_124D:
        db  0CDh, 020h, 001h, 000h, 011h, 000h
        and    eax, 3
        or     eax, ecx
loc_13_1258:
        test   al, 0f9h
        ret
loc_13_125B:
        mov    dl, al
        movzx  eax, byte ptr [ebp + 10h]
        call   sub_13_1228
        jb     NEAR PTR loc_13_0018
        cmp    dl, 0
        jne    SHORT loc_13_1275
        mov    word ptr [ebp + 1ch], ax
loc_13_1275:
        jmp    NEAR PTR loc_13_001D
        db  000h, 000h
loc_13_127C:
        mov    ds, word ptr [ebp + 34h]
        mov    esi, dword ptr [ebp + 30h]
        lar    eax, word ptr [ebp + 34h]
        test   eax, 400000h
        jne    SHORT loc_13_1291
        movzx  esi, si
loc_13_1291:
        mov    eax, dword ptr [esi + 8]
        mov    edx, dword ptr [esi + 4]
loc_13_1297:
        mov    dword ptr [ebp + 24h], eax
        mov    word ptr [ebp + 28h], dx
        add    esi, 0ch
        mov    eax, dword ptr [ebp + 1ch]
        mov    edx, eax
        shr    edx, 10h
        cmp    edx, 40h
        jae    SHORT loc_13_12E8
        db  026h, 08Bh, 014h, 095h, 0E0h, 0C5h, 000h, 000h
        or     edx, edx
        je     SHORT loc_13_1308
loc_13_12BA:
        movzx  eax, ax
        cmp    dword ptr es:[edx], eax
        jbe    SHORT loc_13_1308
        inc    eax
        mov    ecx, dword ptr es:[edx + eax*8 + 4]
        mov    edx, dword ptr es:[edx + eax*8]
        pop    eax
        shl    ecx, 2
        sub    esp, ecx
        mov    edi, esp
        push   ebx
        push   ebp
        push   eax
        shr    ecx, 2
        je     SHORT loc_13_12DF
        cld
loc_13_12DD:
        rep movsd
loc_13_12DF:
        mov    eax, ss
        mov    ds, eax
        mov    dword ptr [ebp + 30h], esi
        jmp    edx
loc_13_12E8:
        db  0B9h, 040h, 0C3h, 000h, 000h
loc_13_12ED:
        mov    ecx, dword ptr es:[ecx]
        jecxz  loc_13_1308
        cmp    word ptr es:[ecx + 6], dx
        jne    SHORT loc_13_12ED
        test   word ptr es:[ecx + 0ah], 4000h
        je     SHORT loc_13_1308
        mov    edx, dword ptr es:[ecx + 38h]
        jmp    SHORT loc_13_12BA
loc_13_1308:
        mov    dword ptr [ebp + 1ch], 1
        mov    eax, ss
        mov    ds, eax
        mov    dword ptr [ebp + 30h], esi
        ret
        db  000h
sub_13_1318:
        cmp    eax, 10h
        jb     SHORT loc_13_138E
        cmp    eax, 3fh
        ja     SHORT loc_13_138E
        cmp    eax, 20h
        je     SHORT loc_13_138E
        cmp    eax, 22h
        je     SHORT loc_13_138E
        cmp    eax, 30h
        je     SHORT loc_13_138E
        pushad
        lea    esi, [esi + eax*8]
        lea    edi, [edi + eax*8]
        mov    cx, word ptr [esi + 4]
        mov    edx, dword ptr [esi]
        cmp    cx, 3bh
        jne    SHORT loc_13_1365
        push   eax
        add    eax, eax
        cmp    edx, eax
        pop    eax
        jne    SHORT loc_13_1365
        db  08Dh, 034h, 0C5h, 09Ch, 09Bh, 000h, 000h
        mov    cx, word ptr [esi + 2]
        mov    dx, word ptr [esi + 6]
        shl    edx, 10h
        mov    dx, word ptr [esi]
        mov    al, 8eh
        jmp    SHORT loc_13_1378
loc_13_1365:
        mov    al, 0e7h
        cmp    word ptr [esi + 6], 0fcffh
        jne    SHORT loc_13_1371
        mov    al, 0e6h
loc_13_1371:
        test   byte ptr [ebx], 80h
        je     SHORT loc_13_1378
        or     al, 8
loc_13_1378:
        mov    byte ptr [edi + 5], al
        mov    word ptr [edi + 2], cx
        mov    word ptr [edi], dx
        shr    edx, 10h
        mov    word ptr [edi + 6], dx
        mov    byte ptr [edi + 4], 0
        popad
loc_13_138E:
        ret
        db  000h
loc_13_1390:
        pop    ebx
        pop    esi
        pop    edi
        db  0CDh, 020h, 01Eh, 001h, 001h, 000h
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    1ch
loc_13_13A5:
        pop    ebx
        pop    esi
        pop    edi
        db  0CDh, 020h, 01Fh, 001h, 001h, 000h
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    14h
sub_13_13BA:
        mov    eax, 2
        mov    edx, dword ptr [esp + 4]
        and    edx, 20h
        shr    edx, 5
        or     edx, dword ptr [esp + 4]
        and    edx, 1
        sub    eax, edx
        mov    edx, dword ptr [esp + 4]
        and    edx, 8
        shr    edx, 2
        add    eax, edx
        ret    4
sub_13_13E1:
        xor    eax, eax
        db  0F6h, 005h, 030h, 0F9h, 000h, 000h, 002h
        je     SHORT loc_13_13F8
        push   ebx
        mov    ecx, dword ptr [esp + 8]
        db  0CDh, 020h, 008h, 000h, 007h, 000h
        pop    ebx
loc_13_13F8:
        ret    4
        db  000h, 000h, 000h, 000h, 000h
sub_13_1400:
        push   esi
        db  0A1h, 0E8h, 0F7h, 000h, 000h
        mov    ecx, eax
        mov    esi, dword ptr [esp + 8]
loc_13_140C:
        cmp    esi, ecx
        jbe    SHORT loc_13_1418
        mov    edx, dword ptr [ecx]
        add    edx, ecx
        cmp    edx, esi
        ja     SHORT loc_13_144C
loc_13_1418:
        mov    ecx, dword ptr [ecx + 4]
        or     ecx, ecx
        jne    SHORT loc_13_140C
        db  0A1h, 0C4h, 0F8h, 000h, 000h
        mov    ecx, eax
loc_13_1426:
        cmp    esi, ecx
        jbe    SHORT loc_13_1432
        mov    edx, dword ptr [ecx]
        add    edx, ecx
        cmp    edx, esi
        ja     SHORT loc_13_144C
loc_13_1432:
        mov    ecx, dword ptr [ecx + 4]
        or     ecx, ecx
        jne    SHORT loc_13_1426
        db  0A1h, 0F8h, 0F7h, 000h, 000h
        cmp    esi, eax
        jbe    SHORT loc_13_144A
        db  03Bh, 035h, 018h, 0FDh, 000h, 000h
        jb     SHORT loc_13_144C
loc_13_144A:
        sub    eax, eax
loc_13_144C:
        pop    esi
        ret    4
sub_13_1450:
        sub    esp, 4
        push   esi
        mov    eax, dword ptr [esp + 10h]
        sub    eax, 4
        mov    dword ptr [esp + 4], eax
        mov    esi, dword ptr [esp + 0ch]
        push   eax
        push   esi
        call   sub_13_19C9
        or     eax, eax
        jne    SHORT loc_13_1472
        sub    eax, eax
        jmp    SHORT loc_13_14D9
loc_13_1472:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [esp + 4]
        mov    eax, dword ptr [eax]
        mov    ecx, eax
        or     eax, 0f0000000h
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [esp + 4]
        and    ecx, 0ffffffch
        test   byte ptr [eax], 2
        je     SHORT loc_13_14BE
        mov    eax, dword ptr [eax - 4]
        mov    dword ptr [esp + 4], eax
        mov    eax, dword ptr [eax]
        and    eax, 0ffffffch
        add    ecx, eax
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [eax + 8], edx
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [eax + 4], edx
loc_13_14BE:
        push   200h
        push   ecx
        push   dword ptr [esp + 0ch]
        push   esi
        call   sub_13_190F
        push   esi
        call   sub_13_1A32
        mov    eax, 1
loc_13_14D9:
        pop    esi
        add    esp, 4
        ret    8
sub_13_14E0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 1ch]
        cmp    edi, 0fffff98h
        ja     NEAR PTR loc_13_15FD
        mov    esi, dword ptr [esp + 18h]
        push   0
        push   esi
        call   sub_13_19C9
        or     eax, eax
        je     NEAR PTR loc_13_15FD
        add    edi, 7
        and    edi, 0fffffffch
        cmp    edi, 10h
        ja     SHORT loc_13_151B
        mov    edi, 10h
loc_13_151B:
        lea    ebp, [esi + 0ch]
loc_13_151E:
        lea    eax, [esi + 8]
        cmp    dword ptr [esi + 8], edi
        jae    SHORT loc_13_152D
loc_13_1526:
        add    eax, 10h
        cmp    dword ptr [eax], edi
        jb     SHORT loc_13_1526
loc_13_152D:
        mov    ebx, dword ptr [eax + 8]
        cmp    ebp, ebx
        je     SHORT loc_13_154A
loc_13_1534:
        mov    eax, dword ptr [ebx]
        and    eax, 0ffffffch
        cmp    eax, edi
        jae    NEAR PTR loc_13_15C7
        mov    ebx, dword ptr [ebx + 4]
        cmp    ebp, ebx
        jne    SHORT loc_13_1534
loc_13_154A:
        mov    al, byte ptr [esi + 70h]
        test   al, 40h
        je     NEAR PTR loc_13_15F7
        lea    ebx, [edi + 2000h]
        and    ebx, 0fffff000h
        cmp    ebx, 400000h
        ja     SHORT loc_13_156E
        mov    ebx, 400000h
loc_13_156E:
        and    al, 80h
        cmp    al, 1
        cmc
        sbb    eax, eax
        and    eax, 8
        or     eax, 10h
        push   eax
        mov    eax, ebx
        shr    eax, 0ch
        push   eax
        push   80080000h
        db  0E8h, 054h, 0ECh, 0F2h, 0FFh
        mov    dword ptr [esp + 1ch], eax
        add    esp, 0ch
        cmp    eax, -1
        je     SHORT loc_13_15F7
        movzx  eax, byte ptr [esi + 70h]
        or     eax, 100h
        push   eax
        push   ebx
        push   dword ptr [esp + 18h]
        push   esi
        db  0E8h, 083h, 0ECh, 0F2h, 0FFh
        or     eax, eax
        je     SHORT loc_13_15E9
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esi + 4], eax
        jmp    NEAR PTR loc_13_151E
loc_13_15C7:
        push   dword ptr [esp + 20h]
        push   edi
        push   ebx
        push   esi
        call   sub_13_176E
        or     eax, eax
        je     SHORT loc_13_15F7
        or     eax, 0a0000000h
        mov    dword ptr [ebx], eax
        push   esi
        call   sub_13_1A32
        lea    eax, [ebx + 4]
        jmp    SHORT loc_13_15FF
loc_13_15E9:
        push   10h
        push   dword ptr [esp + 14h]
        db  0E8h, 03Ch, 0EBh, 0F2h, 0FFh
        add    esp, 8
loc_13_15F7:
        push   esi
        call   sub_13_1A32
loc_13_15FD:
        sub    eax, eax
loc_13_15FF:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    0ch
sub_13_1609:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 20h]
        cmp    esi, 0fffff98h
        ja     NEAR PTR loc_13_1758
        mov    edi, dword ptr [esp + 1ch]
        sub    edi, 4
        push   edi
        push   dword ptr [esp + 1ch]
        call   sub_13_19C9
        or     eax, eax
        je     NEAR PTR loc_13_1758
        add    esi, 7
        and    esi, 0fffffffch
        cmp    esi, 10h
        ja     SHORT loc_13_1649
        mov    esi, 10h
loc_13_1649:
        mov    eax, dword ptr [edi]
        mov    dword ptr [esp + 10h], eax
        mov    ebx, eax
        and    ebx, 0ffffffch
        lea    eax, [esi + 10h]
        cmp    eax, ebx
        ja     SHORT loc_13_168C
        push   200h
        sub    ebx, esi
        push   ebx
        lea    eax, [edi + esi]
        push   eax
        push   dword ptr [esp + 24h]
        call   sub_13_190F
        mov    eax, dword ptr [edi]
        and    eax, 0f0000003h
        or     eax, esi
loc_13_167C:
        mov    dword ptr [edi], eax
loc_13_167E:
        push   dword ptr [esp + 18h]
        call   sub_13_1A32
        jmp    NEAR PTR loc_13_1760
loc_13_168C:
        cmp    esi, ebx
        jbe    SHORT loc_13_167E
        lea    edx, [ebx + edi]
        mov    eax, dword ptr [edx]
        test   al, 1
        je     SHORT loc_13_16CE
        mov    ecx, esi
        sub    ecx, ebx
        and    eax, 0ffffffch
        cmp    eax, ecx
        jb     SHORT loc_13_16CE
        mov    ebp, dword ptr [esp + 24h]
        push   ebp
        push   ecx
        push   edx
        push   dword ptr [esp + 24h]
        call   sub_13_176E
        or     eax, eax
        je     NEAR PTR loc_13_174F
        add    eax, ebx
        mov    ecx, dword ptr [esp + 10h]
        and    ecx, 0f0000003h
        or     eax, ecx
        jmp    SHORT loc_13_167C
loc_13_16CE:
        mov    ebp, dword ptr [esp + 24h]
        test   ebp, 10h
        je     SHORT loc_13_174F
        sub    esi, 4
        test   ebp, 4
        je     SHORT loc_13_170E
        push   dword ptr [esp + 1ch]
        or     ebp, 8
        push   dword ptr [esp + 1ch]
        call   sub_13_1450
        push   ebp
        push   esi
        push   dword ptr [esp + 20h]
        call   sub_13_14E0
        mov    dword ptr [esp + 1ch], eax
        or     eax, eax
        jne    NEAR PTR loc_13_167E
        jmp    SHORT loc_13_174F
loc_13_170E:
        or     ebp, 8
        push   ebp
        push   esi
        push   dword ptr [esp + 20h]
        call   sub_13_14E0
        mov    ebp, eax
        or     ebp, ebp
        je     SHORT loc_13_174F
        sub    ebx, 4
        mov    edi, ebp
        mov    esi, dword ptr [esp + 1ch]
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        push   dword ptr [esp + 1ch]
        push   dword ptr [esp + 1ch]
        call   sub_13_1450
        mov    dword ptr [esp + 1ch], ebp
        jmp    NEAR PTR loc_13_167E
loc_13_174F:
        push   dword ptr [esp + 18h]
        call   sub_13_1A32
loc_13_1758:
        mov    dword ptr [esp + 1ch], 0
loc_13_1760:
        mov    eax, dword ptr [esp + 1ch]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    10h
sub_13_176E:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 24h]
        mov    eax, dword ptr [esp + 2ch]
        and    eax, 1
        mov    edi, dword ptr [esi]
        and    edi, 0ffffffch
        mov    dword ptr [esp + 18h], eax
        or     eax, eax
        mov    ebx, dword ptr [esp + 28h]
        je     SHORT loc_13_17A6
        cmp    ebx, 4000h
        jbe    SHORT loc_13_17A6
        mov    dword ptr [esp + 14h], 1
        jmp    SHORT loc_13_17AE
loc_13_17A6:
        mov    dword ptr [esp + 14h], 0
loc_13_17AE:
        lea    ebp, [esi + 100bh]
        shr    ebp, 0ch
        lea    eax, [esi + edi]
        sub    eax, 4
        shr    eax, 0ch
        mov    dword ptr [esp + 10h], eax
        lea    eax, [ebx + 10h]
        cmp    eax, edi
        ja     SHORT loc_13_181A
        lea    ecx, [ebx + esi]
        add    ecx, 0bh
        shr    ecx, 0ch
        cmp    ecx, dword ptr [esp + 10h]
        jne    SHORT loc_13_17DB
        dec    ecx
loc_13_17DB:
        mov    eax, dword ptr [esp + 20h]
        sub    ecx, ebp
        movzx  eax, byte ptr [eax + 70h]
        or     eax, dword ptr [esp + 14h]
        push   eax
        inc    ecx
        push   ecx
        push   ebp
        call   sub_13_18BC
        or     eax, eax
        je     NEAR PTR loc_13_18AE
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [eax + 8], ecx
        mov    dword ptr [ecx + 4], eax
        sub    edi, ebx
        push   0
        push   edi
        lea    eax, [esi + ebx]
        push   eax
        push   dword ptr [esp + 2ch]
        call   sub_13_190F
        jmp    SHORT loc_13_184A
loc_13_181A:
        mov    eax, dword ptr [esp + 20h]
        movzx  eax, byte ptr [eax + 70h]
        or     eax, dword ptr [esp + 14h]
        push   eax
        mov    eax, dword ptr [esp + 14h]
        sub    eax, ebp
        push   eax
        push   ebp
        call   sub_13_18BC
        or     eax, eax
        je     SHORT loc_13_18AE
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ecx + 8], eax
        mov    dword ptr [eax + 4], ecx
        mov    ebx, edi
        and    dword ptr [esi + edi], 0fffffffdh
loc_13_184A:
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_13_18B0
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_13_189A
        shl    ebp, 0ch
        sub    ebp, esi
        mov    edi, esi
        sub    eax, eax
        mov    ecx, ebp
        shr    ecx, 2
        rep stosd
        mov    ecx, ebp
        and    ecx, 3
        rep stosb
        mov    edi, dword ptr [esp + 10h]
        shl    edi, 0ch
        lea    eax, [ebx + esi]
        cmp    eax, edi
        jbe    SHORT loc_13_18B0
        mov    eax, dword ptr [esp + 10h]
        shl    eax, 0ch
        sub    esi, eax
        add    esi, ebx
        sub    eax, eax
        mov    ecx, esi
        shr    ecx, 2
        rep stosd
        mov    ecx, esi
        and    ecx, 3
        rep stosb
        jmp    SHORT loc_13_18B0
loc_13_189A:
        mov    edi, esi
        sub    eax, eax
        mov    ecx, ebx
        shr    ecx, 2
        rep stosd
        mov    ecx, ebx
        and    ecx, 3
        rep stosb
        jmp    SHORT loc_13_18B0
loc_13_18AE:
        sub    ebx, ebx
loc_13_18B0:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret    10h
sub_13_18BC:
        push   esi
        mov    eax, 1
        mov    esi, dword ptr [esp + 0ch]
        or     esi, esi
        jle    SHORT loc_13_190B
        mov    edx, dword ptr [esp + 10h]
        mov    eax, edx
        and    eax, 80h
        cmp    eax, 1
        cmc
        sbb    ecx, ecx
        and    ecx, 8
        or     ecx, 0a0060000h
        push   ecx
        push   0
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0fffffffeh
        add    eax, 4
        and    edx, 1
        cmp    edx, 1
        sbb    ecx, ecx
        inc    ecx
        sub    eax, ecx
        push   eax
        push   esi
        push   dword ptr [esp + 18h]
        call   sub_13_1A60
        add    esp, 14h
loc_13_190B:
        pop    esi
        ret    0ch
sub_13_190F:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 1ch]
        mov    ecx, 100000h
        mov    esi, dword ptr [esp + 18h]
        lea    ebx, [edi + esi]
        mov    eax, dword ptr [ebx]
        test   al, 1
        je     SHORT loc_13_1948
        and    eax, 0ffffffch
        add    edi, eax
        mov    ecx, dword ptr [ebx + 4]
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [ecx + 8], eax
        mov    dword ptr [eax + 4], ecx
        lea    ecx, [ebx + 100bh]
        shr    ecx, 0ch
        lea    ebx, [esi + edi]
loc_13_1948:
        lea    ebp, [ebx - 4]
        test   byte ptr [esp + 21h], 2
        je     SHORT loc_13_197B
        lea    eax, [esi + 100bh]
        shr    eax, 0ch
        mov    edx, ebp
        shr    edx, 0ch
        cmp    edx, ecx
        jb     SHORT loc_13_1966
        mov    edx, ecx
loc_13_1966:
        sub    edx, eax
        or     edx, edx
        jle    SHORT loc_13_197B
        push   20000000h
        push   edx
        push   eax
        call   sub_13_1DAE
        add    esp, 0ch
loc_13_197B:
        mov    dword ptr [ebp], esi
        mov    eax, dword ptr [esp + 14h]
        add    eax, 8
        or     dword ptr [ebx], 2
        cmp    dword ptr [eax], edi
        jae    SHORT loc_13_1993
loc_13_198C:
        add    eax, 10h
        cmp    dword ptr [eax], edi
        jb     SHORT loc_13_198C
loc_13_1993:
        add    eax, 4
loc_13_1996:
        mov    edx, eax
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [eax]
        and    ecx, 0ffffffch
        cmp    edi, ecx
        jbe    SHORT loc_13_19AB
        or     ecx, ecx
        jne    SHORT loc_13_1996
loc_13_19AB:
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [eax + 8], esi
        mov    dword ptr [esi + 8], edx
        or     edi, 0a0000001h
        mov    dword ptr [edx + 4], esi
        mov    dword ptr [esi], edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    10h
sub_13_19C9:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    word ptr [esi + 72h], 4948h
        jne    SHORT loc_13_1A25
        test   byte ptr [esi + 70h], 8
        jne    SHORT loc_13_19EA
        push   5
        push   dword ptr [esi + 4ch]
        db  0FFh, 015h, 0F0h, 0F8h, 000h, 000h
        add    esp, 8
loc_13_19EA:
        db  0A1h, 098h, 0F6h, 000h, 000h
        inc    dword ptr [eax - 28h]
        mov    eax, dword ptr [esp + 0ch]
        or     eax, eax
        je     SHORT loc_13_1A2B
        mov    ecx, esi
loc_13_19FC:
        cmp    ecx, eax
        jae    SHORT loc_13_1A08
        mov    edx, dword ptr [ecx]
        add    edx, ecx
        cmp    edx, eax
        ja     SHORT loc_13_1A11
loc_13_1A08:
        mov    ecx, dword ptr [ecx + 4]
        or     ecx, ecx
        jne    SHORT loc_13_19FC
        jmp    SHORT loc_13_1A1F
loc_13_1A11:
        mov    eax, dword ptr [eax]
        and    eax, 0f0000001h
        cmp    eax, 0a0000000h
        je     SHORT loc_13_1A2B
loc_13_1A1F:
        push   esi
        call   sub_13_1A32
loc_13_1A25:
        sub    eax, eax
loc_13_1A27:
        pop    esi
        ret    8
loc_13_1A2B:
        mov    eax, 1
        jmp    SHORT loc_13_1A27
sub_13_1A32:
        db  0A1h, 098h, 0F6h, 000h, 000h
        dec    dword ptr [eax - 28h]
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax + 70h], 8
        jne    SHORT loc_13_1A50
        push   dword ptr [eax + 4ch]
        db  0FFh, 015h, 0ECh, 0F7h, 000h, 000h
        add    esp, 4
loc_13_1A50:
        ret    4
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_13_1A60:
        sub    esp, 38h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 24h], 0
        mov    dword ptr [esp + 1ch], 0
        mov    byte ptr [esp + 13h], 0
        cmp    dword ptr [esp + 54h], 0
        je     NEAR PTR loc_13_1D75
        cmp    dword ptr [esp + 54h], 20h
        jae    NEAR PTR loc_13_1D75
        mov    eax, dword ptr [esp + 54h]
        db  08Bh, 014h, 085h, 008h, 0FCh, 000h, 000h
        or     edx, edx
        je     NEAR PTR loc_13_1D75
        db  066h, 08Bh, 00Dh, 030h, 0F9h, 000h, 000h
        test   cl, 6
        mov    ebx, dword ptr [esp + 5ch]
        jne    SHORT loc_13_1ABB
        or     ebx, 80000000h
loc_13_1ABB:
        db  085h, 01Dh, 0F8h, 0F8h, 000h, 000h
        jne    SHORT loc_13_1ACE
        test   cl, 4
        je     SHORT loc_13_1AFB
        test   byte ptr [edx + 1ch], 1
        jne    SHORT loc_13_1AFB
loc_13_1ACE:
        mov    eax, dword ptr [esp + 50h]
        db  003h, 005h, 01Ch, 0EFh, 000h, 000h
        db  0A3h, 01Ch, 0EFh, 000h, 000h
        db  03Bh, 005h, 024h, 0EFh, 000h, 000h
        jle    SHORT loc_13_1AF6
        push   dword ptr [esp + 50h]
        db  0E8h, 0D8h, 039h, 0F3h, 0FFh
        or     eax, eax
        je     NEAR PTR loc_13_1D6B
loc_13_1AF6:
        mov    byte ptr [esp + 13h], 1
loc_13_1AFB:
        push   dword ptr [esp + 54h]
        push   dword ptr [esp + 54h]
        call   sub_13_1E64
        or     eax, eax
        je     NEAR PTR loc_13_1D64
        push   dword ptr [esp + 4ch]
        db  0E8h, 097h, 01Dh, 0F3h, 0FFh
        mov    dword ptr [esp + 28h], eax
        lea    eax, [esp + 14h]
        push   eax
        push   dword ptr [esp + 54h]
        push   dword ptr [esp + 54h]
        db  0E8h, 0C1h, 01Dh, 0F3h, 0FFh
        or     eax, eax
        je     NEAR PTR loc_13_1D4A
        test   byte ptr [eax + 4], 10h
        je     SHORT loc_13_1B49
        test   ebx, 20000000h
        je     NEAR PTR loc_13_1D4A
loc_13_1B49:
        mov    ecx, dword ptr [esp + 54h]
        shl    ecx, 10h
        mov    cl, 1
        db  02Bh, 005h, 0ECh, 0FAh, 000h, 000h
        mov    dword ptr [esp + 3ch], ecx
        mov    ecx, 0ch
        sub    edx, edx
        div    ecx
        mov    dword ptr [esp + 38h], eax
        mov    word ptr [esp + 40h], ax
        mov    eax, dword ptr [esp + 58h]
        mov    dword ptr [esp + 42h], eax
        mov    eax, ebx
        and    eax, 40000000h
        shr    eax, 1eh
        mov    dword ptr [esp + 20h], eax
        lea    eax, [esp + 30h]
        push   eax
        push   dword ptr [esp + 54h]
        push   dword ptr [esp + 54h]
        call   sub_13_21EE
        mov    dword ptr [esp + 2ch], eax
        mov    eax, dword ptr [esp + 50h]
        shl    eax, 2
        mov    dword ptr [esp + 34h], eax
        add    eax, dword ptr [esp + 14h]
        mov    dword ptr [esp + 18h], eax
        cmp    eax, dword ptr [esp + 14h]
        jbe    NEAR PTR loc_13_1CBA
loc_13_1BB7:
        mov    eax, dword ptr [esp + 14h]
        mov    eax, dword ptr [eax]
        test   ah, 2
        je     SHORT loc_13_1BE8
        db  085h, 01Dh, 088h, 0F8h, 000h, 000h
        jne    NEAR PTR loc_13_1CE4
        test   ebx, 20000h
        je     SHORT loc_13_1BDF
        mov    ecx, dword ptr [esp + 14h]
        or     eax, 2
        mov    dword ptr [ecx], eax
loc_13_1BDF:
        dec    dword ptr [esp + 1ch]
        jmp    NEAR PTR loc_13_1C9F
loc_13_1BE8:
        db  0FFh, 035h, 0ACh, 0F8h, 000h, 000h
        db  0E8h, 02Dh, 01Eh, 0F3h, 0FFh
        mov    ebp, eax
        lea    esi, [esp + 3ch]
        mov    edi, ebp
        movsd
        movsd
        movsw
        mov    eax, ebp
        db  02Bh, 005h, 0ACh, 0F8h, 000h, 000h
        mov    ecx, 0ah
        sub    edx, edx
        div    ecx
        shl    eax, 0ch
        mov    ecx, ebx
        and    ecx, 60000h
        or     ecx, 2000000h
        shr    ecx, 10h
        or     eax, ecx
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [ecx], eax
        db  085h, 01Dh, 088h, 0F8h, 000h, 000h
        je     SHORT loc_13_1C9F
loc_13_1C34:
        push   ebp
        db  0E8h, 062h, 01Ah, 0F3h, 0FFh
        or     eax, eax
        je     SHORT loc_13_1C34
        push   ebp
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 54h]
        db  0E8h, 0B6h, 02Bh, 0F3h, 0FFh
        mov    esi, eax
        or     esi, esi
        db  0A1h, 0F8h, 0F8h, 000h, 000h
        je     NEAR PTR loc_13_1CEB
        inc    dword ptr [esp + 4ch]
        test   ebx, 8000000h
        je     SHORT loc_13_1C6E
        mov    ecx, dword ptr [esp + 14h]
        or     dword ptr [ecx], 40h
loc_13_1C6E:
        test   ebx, eax
        je     SHORT loc_13_1C99
        mov    di, word ptr [esi + 8]
        or     di, di
        jne    SHORT loc_13_1C99
        push   dword ptr [esp + 14h]
        db  0E8h, 043h, 029h, 0F3h, 0FFh
        or     eax, eax
        je     SHORT loc_13_1CF8
        inc    di
        mov    word ptr [esi + 8], di
        test   bl, 8
        je     SHORT loc_13_1C99
        mov    word ptr [esi + 8], 0f188h
loc_13_1C99:
        push   ebp
        db  0E8h, 01Eh, 01Ah, 0F3h, 0FFh
loc_13_1C9F:
        add    dword ptr [esp + 14h], 4
        mov    eax, dword ptr [esp + 20h]
        add    dword ptr [esp + 42h], eax
        mov    eax, dword ptr [esp + 18h]
        cmp    eax, dword ptr [esp + 14h]
        ja     NEAR PTR loc_13_1BB7
loc_13_1CBA:
        push   dword ptr [esp + 30h]
        push   dword ptr [esp + 30h]
        call   sub_13_225C
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [esp + 24h], 1
        push   dword ptr [eax + 10h]
        db  0FFh, 015h, 0ECh, 0F7h, 000h, 000h
        add    esp, 4
        jmp    NEAR PTR loc_13_1D75
loc_13_1CE4:
        sub    dword ptr [esp + 14h], 4
        jmp    SHORT loc_13_1CFE
loc_13_1CEB:
        cmp    byte ptr [esp + 13h], 0
        je     SHORT loc_13_1CF8
        db  0FFh, 00Dh, 01Ch, 0EFh, 000h, 000h
loc_13_1CF8:
        push   ebp
        db  0E8h, 0BFh, 019h, 0F3h, 0FFh
loc_13_1CFE:
        mov    edi, dword ptr [esp + 38h]
        shl    edi, 0ch
        mov    eax, dword ptr [esp + 18h]
        sub    eax, dword ptr [esp + 34h]
        mov    dword ptr [esp + 18h], eax
        cmp    eax, dword ptr [esp + 14h]
        ja     SHORT loc_13_1D38
        mov    esi, 0fffffffch
loc_13_1D1C:
        push   edi
        push   dword ptr [esp + 18h]
        db  0E8h, 00Ah, 01Eh, 0F3h, 0FFh
        dec    dword ptr [esp + 50h]
        add    dword ptr [esp + 14h], esi
        mov    eax, dword ptr [esp + 18h]
        cmp    eax, dword ptr [esp + 14h]
        jbe    SHORT loc_13_1D1C
loc_13_1D38:
        db  0E8h, 0D1h, 019h, 0F3h, 0FFh
        push   dword ptr [esp + 30h]
        push   dword ptr [esp + 30h]
        call   sub_13_225C
loc_13_1D4A:
        mov    eax, dword ptr [esp + 28h]
        push   dword ptr [eax + 10h]
        db  0FFh, 015h, 0ECh, 0F7h, 000h, 000h
        mov    eax, dword ptr [esp + 54h]
        add    esp, 4
        neg    eax
        mov    dword ptr [esp + 1ch], eax
loc_13_1D64:
        cmp    byte ptr [esp + 13h], 0
        je     SHORT loc_13_1D75
loc_13_1D6B:
        mov    eax, dword ptr [esp + 50h]
        db  029h, 005h, 01Ch, 0EFh, 000h, 000h
loc_13_1D75:
        cmp    dword ptr [esp + 1ch], 0
        je     SHORT loc_13_1DA2
        mov    eax, dword ptr [esp + 54h]
        db  08Bh, 004h, 085h, 008h, 0FCh, 000h, 000h
        mov    ecx, dword ptr [eax + 1ch]
        and    ecx, 1
        mov    eax, dword ptr [esp + 1ch]
        db  001h, 004h, 08Dh, 084h, 0EEh, 000h, 000h
        call   sub_13_1FD2
        db  0E8h, 06Ch, 019h, 0F3h, 0FFh
loc_13_1DA2:
        mov    eax, dword ptr [esp + 24h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 38h
        ret
sub_13_1DAE:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    edi, edi
        mov    ebp, dword ptr [esp + 20h]
        push   ebp
        db  0E8h, 0EFh, 01Ah, 0F3h, 0FFh
        mov    dword ptr [esp + 14h], eax
        lea    eax, [esp + 10h]
        mov    esi, dword ptr [esp + 24h]
        push   eax
        push   esi
        push   ebp
        db  0E8h, 01Bh, 01Bh, 0F3h, 0FFh
        mov    ebx, eax
        cmp    ebx, edi
        je     SHORT loc_13_1E45
        test   byte ptr [ebx + 4], 10h
        je     SHORT loc_13_1DE8
        test   byte ptr [esp + 2bh], 20h
        je     SHORT loc_13_1E45
loc_13_1DE8:
        lea    eax, [esp + 18h]
        push   eax
        push   esi
        push   ebp
        call   sub_13_21EE
        mov    edi, eax
        db  02Bh, 01Dh, 0ECh, 0FAh, 000h, 000h
        mov    ecx, 0ch
        mov    eax, ebx
        sub    edx, edx
        div    ecx
        shl    eax, cl
        mov    ebx, eax
        shl    esi, 2
        add    esi, dword ptr [esp + 10h]
        cmp    esi, dword ptr [esp + 10h]
        jbe    SHORT loc_13_1E31
        mov    ebp, 4
loc_13_1E1D:
        push   ebx
        push   dword ptr [esp + 14h]
        db  0E8h, 009h, 01Dh, 0F3h, 0FFh
        add    dword ptr [esp + 10h], ebp
        cmp    esi, dword ptr [esp + 10h]
        ja     SHORT loc_13_1E1D
loc_13_1E31:
        push   dword ptr [esp + 18h]
        push   edi
        call   sub_13_225C
        mov    edi, 1
        db  0E8h, 0C9h, 018h, 0F3h, 0FFh
loc_13_1E45:
        mov    eax, dword ptr [esp + 14h]
        push   dword ptr [eax + 10h]
        db  0FFh, 015h, 0ECh, 0F7h, 000h, 000h
        add    esp, 4
        call   sub_13_1FD2
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
sub_13_1E64:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    byte ptr [esp + 13h], 0
        push   5
        db  0FFh, 035h, 0C8h, 0F7h, 000h, 000h
        db  0FFh, 015h, 0F0h, 0F8h, 000h, 000h
        add    esp, 8
        db  0FEh, 005h, 0F8h, 0FAh, 000h, 000h
        mov    esi, dword ptr [esp + 20h]
loc_13_1E8B:
        db  0A1h, 088h, 0EEh, 000h, 000h
        db  08Bh, 00Dh, 084h, 0EEh, 000h, 000h
        lea    eax, [eax + ecx + 28h]
        mov    dword ptr [esp + 14h], eax
        mov    eax, dword ptr [esp + 24h]
        db  08Bh, 03Ch, 085h, 008h, 0FCh, 000h, 000h
        mov    eax, dword ptr [edi + 1ch]
        and    eax, 1
        db  001h, 034h, 085h, 084h, 0EEh, 000h, 000h
        db  0A1h, 018h, 0EFh, 000h, 000h
        db  02Bh, 005h, 088h, 0EEh, 000h, 000h
        mov    eax, 0
        js     SHORT loc_13_1ED3
        db  0A1h, 018h, 0EFh, 000h, 000h
        db  02Bh, 005h, 088h, 0EEh, 000h, 000h
loc_13_1ED3:
        db  08Bh, 00Dh, 084h, 0EEh, 000h, 000h
        db  08Bh, 015h, 064h, 0EFh, 000h, 000h
        and    edx, eax
        sub    ecx, edx
        push   ecx
        call   sub_13_22B0
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_13_1F79
        db  080h, 03Dh, 0F8h, 0FAh, 000h, 000h, 001h
        ja     NEAR PTR loc_13_1FB1
        mov    ecx, esi
        imul   ecx, ecx, 0ah
        mov    ebp, ecx
        shr    ebp, 0ch
        db  0A1h, 088h, 0EEh, 000h, 000h
        db  003h, 005h, 084h, 0EEh, 000h, 000h
        lea    ebx, [ebp + eax + 28h]
        db  0A1h, 0ACh, 0F8h, 000h, 000h
        mov    dword ptr [esp + 18h], 0ah
        mov    eax, dword ptr [eax - 18h]
        sub    eax, 1ch
        sub    edx, edx
        div    dword ptr [esp + 18h]
        cmp    ebx, eax
        jbe    SHORT loc_13_1F67
        add    ecx, 0fffh
        shr    ecx, 0ch
        add    ecx, dword ptr [esp + 14h]
        add    ecx, 28h
        push   ecx
        db  0FFh, 035h, 0ACh, 0F8h, 000h, 000h
        db  0E8h, 01Fh, 01Bh, 0F3h, 0FFh
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_13_1F79
        db  0A1h, 088h, 0EEh, 000h, 000h
        db  003h, 005h, 084h, 0EEh, 000h, 000h
        lea    ebx, [ebp + eax + 28h]
loc_13_1F67:
        push   ebx
        db  0FFh, 035h, 0ACh, 0F8h, 000h, 000h
        db  0E8h, 0FEh, 01Ah, 0F3h, 0FFh
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_13_1FB1
loc_13_1F79:
        mov    eax, dword ptr [edi + 1ch]
        and    eax, 1
        db  029h, 034h, 085h, 084h, 0EEh, 000h, 000h
        db  083h, 03Dh, 064h, 0EFh, 000h, 000h, 000h
        jne    SHORT loc_13_1F9A
        db  0FFh, 00Dh, 064h, 0EFh, 000h, 000h
        jmp    NEAR PTR loc_13_1E8B
loc_13_1F9A:
        cmp    byte ptr [esp + 13h], 0
        jne    SHORT loc_13_1FB1
        mov    byte ptr [esp + 13h], 1
        push   esi
        db  0E8h, 01Ah, 035h, 0F3h, 0FFh
        jmp    NEAR PTR loc_13_1E8B
loc_13_1FB1:
        db  0FEh, 00Dh, 0F8h, 0FAh, 000h, 000h
        db  0FFh, 035h, 0C8h, 0F7h, 000h, 000h
        db  0FFh, 015h, 0ECh, 0F7h, 000h, 000h
        add    esp, 4
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret    8
sub_13_1FD2:
        push   5
        db  0FFh, 035h, 0C8h, 0F7h, 000h, 000h
        db  0FFh, 015h, 0F0h, 0F8h, 000h, 000h
        add    esp, 8
        db  08Bh, 015h, 018h, 0EFh, 000h, 000h
        mov    eax, edx
        db  02Bh, 005h, 088h, 0EEh, 000h, 000h
        jns    SHORT loc_13_1FF7
        sub    edx, edx
        jmp    SHORT loc_13_1FFD
loc_13_1FF7:
        db  02Bh, 015h, 088h, 0EEh, 000h, 000h
loc_13_1FFD:
        db  0A1h, 084h, 0EEh, 000h, 000h
        db  08Bh, 00Dh, 064h, 0EFh, 000h, 000h
        and    ecx, edx
        sub    eax, ecx
        push   eax
        call   sub_13_22B0
        db  0A1h, 088h, 0EEh, 000h, 000h
        db  003h, 005h, 084h, 0EEh, 000h, 000h
        add    eax, 28h
        push   eax
        db  0FFh, 035h, 0ACh, 0F8h, 000h, 000h
        db  0E8h, 045h, 01Ah, 0F3h, 0FFh
        db  0FFh, 035h, 0C8h, 0F7h, 000h, 000h
        db  0FFh, 015h, 0ECh, 0F7h, 000h, 000h
        add    esp, 4
        ret
_PageReAllocate:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 28h]
        and    edi, 188h
        lea    eax, [edi - 1]
        test   edi, eax
        jne    NEAR PTR loc_13_21D6
        test   dword ptr [esp + 20h], 0fffh
        jne    NEAR PTR loc_13_21D6
        mov    ebx, dword ptr [esp + 20h]
        shr    ebx, 0ch
        push   ebx
        db  0E8h, 03Dh, 018h, 0F3h, 0FFh
        mov    dword ptr [esp + 18h], eax
        lea    eax, [esp + 10h]
        push   eax
        push   1
        push   ebx
        db  0E8h, 06Ch, 018h, 0F3h, 0FFh
        or     eax, eax
        je     NEAR PTR loc_13_21C6
        mov    ecx, dword ptr [eax + 4]
        mov    edx, ecx
        shr    edx, 0ch
        cmp    edx, ebx
        jne    NEAR PTR loc_13_21C6
        test   cl, 10h
        jne    NEAR PTR loc_13_21C6
        sub    esi, esi
        cmp    dword ptr [eax], esi
        jbe    SHORT loc_13_20C3
        mov    ecx, 4
loc_13_20B0:
        mov    edx, dword ptr [esp + 10h]
        test   byte ptr [edx + 1], 2
        je     SHORT loc_13_20C3
        add    dword ptr [esp + 10h], ecx
        inc    esi
        cmp    dword ptr [eax], esi
        ja     SHORT loc_13_20B0
loc_13_20C3:
        mov    ebp, dword ptr [esp + 24h]
        cmp    esi, ebp
        jbe    SHORT loc_13_210C
        push   0
        mov    eax, esi
        sub    eax, ebp
        push   eax
        lea    eax, [ebp + ebx]
        push   eax
        db  0FFh, 015h, 0A4h, 0D7h, 000h, 000h
        add    esp, 0ch
loc_13_20E0:
        test   byte ptr [esp + 28h], 20h
        je     SHORT loc_13_20F7
        mov    ecx, esi
        shl    ecx, 0ch
        mov    edi, dword ptr [esp + 20h]
        sub    eax, eax
        shr    ecx, 2
        rep stosd
loc_13_20F7:
        mov    eax, dword ptr [esp + 18h]
        push   dword ptr [eax + 10h]
        db  0FFh, 015h, 0ECh, 0F7h, 000h, 000h
        add    esp, 4
        jmp    NEAR PTR loc_13_21DE
loc_13_210C:
        jae    SHORT loc_13_20E0
        mov    ecx, dword ptr [eax + 4]
        and    ecx, 8
        or     dword ptr [esp + 28h], ecx
        mov    dword ptr [esp + 14h], ebx
        cmp    dword ptr [eax], ebp
        jae    SHORT loc_13_214F
        test   dword ptr [esp + 28h], 10000008h
        jne    NEAR PTR loc_13_21C6
        push   0
        push   ebp
        or     ebx, 80000000h
        push   ebx
        db  0FFh, 015h, 09Ch, 0D7h, 000h, 000h
        mov    dword ptr [esp + 2ch], eax
        add    esp, 0ch
        cmp    eax, -1
        je     SHORT loc_13_21C6
        mov    ebx, eax
        shr    ebx, 0ch
loc_13_214F:
        or     edi, 60000h
        push   edi
        push   0
        push   dword ptr [esp + 30h]
        call   sub_13_13BA
        push   eax
        mov    eax, ebp
        sub    eax, esi
        push   eax
        lea    eax, [ebx + esi]
        push   eax
        db  0FFh, 015h, 0A0h, 0D7h, 000h, 000h
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_13_21B1
        cmp    dword ptr [esp + 14h], ebx
        je     NEAR PTR loc_13_20E0
        or     ebp, ebp
        je     NEAR PTR loc_13_20E0
        push   esi
        push   ebx
        push   dword ptr [esp + 1ch]
        db  0E8h, 0FBh, 001h, 0F3h, 0FFh
        or     eax, eax
        je     SHORT loc_13_21B1
        mov    eax, dword ptr [esp + 14h]
        push   0
        shl    eax, 0ch
        push   eax
        db  0FFh, 015h, 07Ch, 0D4h, 000h, 000h
        add    esp, 8
        jmp    NEAR PTR loc_13_20E0
loc_13_21B1:
        cmp    dword ptr [esp + 14h], ebx
        je     SHORT loc_13_21C6
        push   0
        push   dword ptr [esp + 24h]
        db  0FFh, 015h, 07Ch, 0D4h, 000h, 000h
        add    esp, 8
loc_13_21C6:
        mov    eax, dword ptr [esp + 18h]
        push   dword ptr [eax + 10h]
        db  0FFh, 015h, 0ECh, 0F7h, 000h, 000h
        add    esp, 4
loc_13_21D6:
        mov    dword ptr [esp + 20h], 0
loc_13_21DE:
        mov    edx, dword ptr [esp + 20h]
        mov    eax, dword ptr [esp + 20h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
sub_13_21EE:
        push   esi
        mov    edx, dword ptr [esp + 8]
        mov    eax, edx
        shr    eax, 0ah
        cmp    edx, 7ffffh
        ja     SHORT loc_13_2212
        lea    ecx, [eax*4]
        db  0A1h, 03Ch, 0F8h, 000h, 000h
        mov    esi, dword ptr [eax]
        add    esi, ecx
        jmp    SHORT loc_13_2227
loc_13_2212:
        cmp    edx, 0ff7ffh
        ja     SHORT loc_13_2256
        lea    ecx, [eax*4]
        lea    esi, [ecx - 402000h]
loc_13_2227:
        mov    eax, dword ptr [esp + 0ch]
        add    eax, edx
        dec    eax
        and    eax, 0fffffcffh
        shr    eax, 8
        sub    eax, ecx
        lea    edx, [esi + eax]
        mov    eax, esi
        cmp    edx, esi
        jb     SHORT loc_13_224E
loc_13_2241:
        or     dword ptr [esi], 400h
        add    esi, 4
        cmp    edx, esi
        jae    SHORT loc_13_2241
loc_13_224E:
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [ecx], edx
        jmp    SHORT loc_13_2258
loc_13_2256:
        sub    eax, eax
loc_13_2258:
        pop    esi
        ret    0ch
sub_13_225C:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        or     esi, esi
        je     SHORT loc_13_229F
        mov    edi, dword ptr [esp + 18h]
        cmp    edi, esi
        jb     SHORT loc_13_229F
        db  08Bh, 01Dh, 0E4h, 0F7h, 000h, 000h
loc_13_2276:
        mov    ebp, dword ptr [esi]
        test   ebp, 1
        je     SHORT loc_13_2290
        mov    eax, ebp
        shr    eax, 0ch
        imul   eax, eax, 0dh
        push   dword ptr [ebx + eax]
        db  0E8h, 0D9h, 038h, 0F3h, 0FFh
loc_13_2290:
        and    ebp, 0fffffbffh
        mov    dword ptr [esi], ebp
        add    esi, 4
        cmp    edi, esi
        jae    SHORT loc_13_2276
loc_13_229F:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_13_22B0:
        push   ebx
        push   esi
        push   edi
        mov    esi, 1
        db  08Ah, 00Dh, 028h, 0F9h, 000h, 000h
        or     cl, cl
        jne    NEAR PTR loc_13_23B2
        db  0F6h, 005h, 030h, 0F9h, 000h, 000h, 004h
        mov    edi, dword ptr [esp + 10h]
        je     SHORT loc_13_22DE
        or     edi, edi
        jle    SHORT loc_13_22DE
        sub    esi, esi
        jmp    NEAR PTR loc_13_23B2
loc_13_22DE:
        db  0A1h, 02Ch, 0EFh, 000h, 000h
        lea    edi, [edi + eax + 20h]
        mov    eax, edi
        cdq
        and    edx, 7
        add    eax, edx
        sar    eax, 3
        cdq
        and    edx, 0fffh
        add    eax, edx
        sar    eax, 0ch
        lea    ebx, [edi + eax]
        or     ebx, ebx
        jg     SHORT loc_13_2307
        sub    ebx, ebx
loc_13_2307:
        db  08Bh, 015h, 028h, 0EFh, 000h, 000h
        lea    eax, [edx - 80h]
        cmp    eax, ebx
        jl     SHORT loc_13_2336
        add    ebx, 7fh
        and    ebx, 0ffffff80h
        db  089h, 01Dh, 0A8h, 0FDh, 000h, 000h
        db  0A1h, 02Ch, 0FDh, 000h, 000h
        cmp    eax, 2dh
        jb     SHORT loc_13_232F
        mov    eax, 2dh
loc_13_232F:
        db  0A3h, 02Ch, 0FDh, 000h, 000h
        jmp    SHORT loc_13_23B2
loc_13_2336:
        db  0C7h, 005h, 0A8h, 0FDh, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0C7h, 005h, 02Ch, 0FDh, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        cmp    ebx, edx
        jle    SHORT loc_13_23B2
        inc    cl
        db  088h, 00Dh, 028h, 0F9h, 000h, 000h
        lea    eax, [ebx + 7fh]
        and    eax, 0ffffff80h
        sub    eax, edx
        push   eax
        call   sub_13_13E1
        mov    edi, eax
        cmp    ebx, edi
        jbe    SHORT loc_13_236E
        sub    esi, esi
        jmp    SHORT loc_13_23A6
loc_13_236E:
        db  039h, 03Dh, 000h, 0FBh, 000h, 000h
        jae    SHORT loc_13_23A0
        mov    ebx, edi
        shr    ebx, 0fh
        inc    ebx
        push   10000009h
        push   ebx
        db  0FFh, 035h, 004h, 0FCh, 000h, 000h
        db  0FFh, 015h, 078h, 0D4h, 000h, 000h
        add    esp, 0ch
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_13_23A6
        shl    ebx, 0fh
        db  089h, 01Dh, 000h, 0FBh, 000h, 000h
loc_13_23A0:
        db  089h, 03Dh, 028h, 0EFh, 000h, 000h
loc_13_23A6:
        db  0A0h, 028h, 0F9h, 000h, 000h
        dec    al
        db  0A2h, 028h, 0F9h, 000h, 000h
loc_13_23B2:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        ret    4
_PageDiscardPages:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 1ch], 0
        mov    edi, dword ptr [esp + 28h]
        cmp    edi, 110h
        mov    ebx, dword ptr [esp + 30h]
        lea    eax, [edi + ebx]
        jae    SHORT loc_13_2448
        cmp    eax, 110h
        ja     NEAR PTR loc_13_25BA
        mov    esi, dword ptr [esp + 2ch]
        push   esi
        db  0E8h, 03Fh, 032h, 0F3h, 0FFh
        or     eax, eax
        je     NEAR PTR loc_13_25BA
        test   byte ptr [esi + 1], 10h
        je     NEAR PTR loc_13_25BA
        mov    eax, edi
        shr    eax, 3
        lea    ebp, [esi + eax - 60h]
        mov    edx, 1
        mov    ecx, edi
        and    cl, 7
        shl    edx, cl
        mov    eax, ebx
        or     ebx, ebx
        je     SHORT loc_13_243E
loc_13_241E:
        test   dword ptr [ebp], edx
        je     NEAR PTR loc_13_25BA
        test   edx, 80000000h
        je     SHORT loc_13_2439
        add    ebp, 4
        mov    edx, 1
        jmp    SHORT loc_13_243B
loc_13_2439:
        add    edx, edx
loc_13_243B:
        dec    eax
        jne    SHORT loc_13_241E
loc_13_243E:
        mov    eax, dword ptr [esi + 4]
        shr    eax, 0ch
        add    edi, eax
        jmp    SHORT loc_13_2453
loc_13_2448:
        cmp    eax, 0ffbffh
        ja     NEAR PTR loc_13_25BA
loc_13_2453:
        push   edi
        db  0E8h, 057h, 014h, 0F3h, 0FFh
        mov    dword ptr [esp + 20h], eax
        mov    esi, 0ffffffdfh
        mov    eax, dword ptr [esp + 34h]
        and    eax, 10000h
        mov    dword ptr [esp + 18h], eax
        or     eax, eax
        je     SHORT loc_13_2478
        mov    esi, 0ffffff9fh
loc_13_2478:
        lea    edi, [edi*4 - 800000h]
        lea    eax, [edi + ebx*4]
        mov    dword ptr [esp + 14h], eax
        cmp    eax, edi
        jbe    NEAR PTR loc_13_25A2
loc_13_248E:
        mov    eax, edi
        and    eax, 0fffff3ffh
        shr    eax, 0ah
        test   byte ptr [eax - 7fffffh], 2
        je     NEAR PTR loc_13_25AA
        mov    eax, dword ptr [edi]
        test   ah, 2
        je     NEAR PTR loc_13_25AA
        cmp    dword ptr [esp + 18h], 0
        je     NEAR PTR loc_13_2593
        test   ah, 8
        jne    NEAR PTR loc_13_2593
        push   edi
        db  0E8h, 0F9h, 017h, 0F3h, 0FFh
        mov    ebx, eax
        sub    ebp, ebp
        test   byte ptr [ebx + 3], 10h
        db  0A1h, 0B0h, 0F8h, 000h, 000h
        je     SHORT loc_13_2548
        mov    ebp, dword ptr [ebx + 6]
        test   byte ptr [ebx + 3], 8
        jne    SHORT loc_13_24F3
        cmp    word ptr [ebp + 8], 0
        jne    NEAR PTR loc_13_258D
        test   byte ptr [ebx + 3], 8
        je     SHORT loc_13_2509
loc_13_24F3:
        cmp    ebp, eax
        jne    SHORT loc_13_2501
        db  0A1h, 068h, 0FAh, 000h, 000h
        db  0A3h, 0B0h, 0F8h, 000h, 000h
loc_13_2501:
        push   ebp
        db  0E8h, 018h, 029h, 0F3h, 0FFh
        jmp    SHORT loc_13_2542
loc_13_2509:
        mov    ecx, dword ptr [edi]
        test   cl, 1
        je     NEAR PTR loc_13_25CB
        cmp    word ptr [ebp + 0ah], 1
        jne    NEAR PTR loc_13_25CB
        mov    eax, ebx
        db  02Bh, 005h, 0ACh, 0F8h, 000h, 000h
        mov    dword ptr [esp + 10h], 0ah
        sub    edx, edx
        div    dword ptr [esp + 10h]
        shl    eax, 0ch
        and    ecx, 0ffeh
        or     eax, ecx
        mov    dword ptr [edi], eax
loc_13_2542:
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [ebx + 6], eax
loc_13_2548:
        push   ebp
        push   ebx
        db  0E8h, 04Dh, 036h, 0F3h, 0FFh
        and    byte ptr [ebx + 3], 27h
        or     ebp, ebp
        db  0A1h, 060h, 0FAh, 000h, 000h
        db  08Bh, 00Dh, 0A0h, 0FDh, 000h, 000h
        je     SHORT loc_13_2577
        mov    dword ptr [ebp], 0
        db  0FFh, 005h, 040h, 0F9h, 000h, 000h
        push   ecx
        push   eax
        push   ebp
        db  0E8h, 019h, 028h, 0F3h, 0FFh
loc_13_2577:
        test   byte ptr [esp + 34h], 1
        je     SHORT loc_13_258D
        mov    al, byte ptr [ebx + 2]
        cmp    al, 1
        je     SHORT loc_13_2589
        cmp    al, 2
        jne    SHORT loc_13_258D
loc_13_2589:
        mov    byte ptr [ebx + 2], 1
loc_13_258D:
        push   ebx
        db  0E8h, 02Ah, 011h, 0F3h, 0FFh
loc_13_2593:
        and    dword ptr [edi], esi
        add    edi, 4
        cmp    dword ptr [esp + 14h], edi
        ja     NEAR PTR loc_13_248E
loc_13_25A2:
        mov    dword ptr [esp + 1ch], 1
loc_13_25AA:
        mov    eax, dword ptr [esp + 20h]
        push   dword ptr [eax + 10h]
        db  0FFh, 015h, 0ECh, 0F7h, 000h, 000h
        add    esp, 4
loc_13_25BA:
        db  0E8h, 04Fh, 011h, 0F3h, 0FFh
        mov    eax, dword ptr [esp + 1ch]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
loc_13_25CB:
        and    byte ptr [ebx + 3], 0bfh
        jmp    SHORT loc_13_258D
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_13_25E0:
        push   esi
        push   dword ptr [esp + 8]
        db  0E8h, 0C6h, 012h, 0F3h, 0FFh
        mov    ecx, dword ptr [eax + 8]
        sub    esi, esi
loc_13_25EF:
        mov    ecx, dword ptr [ecx + 8]
        mov    edx, dword ptr [ecx]
        add    esi, edx
        or     edx, edx
        jne    SHORT loc_13_25EF
        push   dword ptr [eax + 10h]
        db  0FFh, 015h, 0ECh, 0F7h, 000h, 000h
        add    esp, 4
        mov    eax, esi
        pop    esi
        ret    4
_HeapAllocate:
        push   ds
        pop    es
        db  0A1h, 0E8h, 0F7h, 000h, 000h
        test   byte ptr [esp + 9], 7
        je     SHORT loc_13_2626
        db  0A1h, 0C4h, 0F8h, 000h, 000h
        test   byte ptr [esp + 9], 2
        je     SHORT loc_13_2635
loc_13_2626:
        push   dword ptr [esp + 8]
        push   dword ptr [esp + 8]
        push   eax
        call   sub_13_14E0
loc_13_2634:
        ret
loc_13_2635:
        db  0A1h, 0F8h, 0F7h, 000h, 000h
        test   byte ptr [esp + 9], 4
        je     SHORT loc_13_264E
        db  080h, 03Dh, 068h, 0F6h, 000h, 000h, 000h
        je     SHORT loc_13_2626
        xor    eax, eax
        jmp    SHORT loc_13_2634
loc_13_264E:
        db  0A1h, 0E0h, 0F7h, 000h, 000h
        jmp    SHORT loc_13_2626
_HeapReAllocate:
        or     byte ptr [esp + 0ch], 10h
        push   dword ptr [esp + 4]
        call   sub_13_1400
        or     eax, eax
        je     SHORT loc_13_2680
        push   dword ptr [esp + 0ch]
        push   dword ptr [esp + 0ch]
        push   dword ptr [esp + 0ch]
        push   eax
        call   sub_13_1609
        test   byte ptr [esp + 0ch], 2
        jne    SHORT loc_13_2681
loc_13_2680:
        ret
loc_13_2681:
        or     eax, eax
        je     SHORT loc_13_2680
        push   eax
        push   edi
        mov    edi, eax
        mov    ecx, dword ptr [esp + 10h]
        add    ecx, 3
        shr    ecx, 2
        xor    eax, eax
        rep stosd
        pop    edi
        pop    eax
        jmp    SHORT loc_13_2680
_HeapFree:
        push   dword ptr [esp + 4]
        call   sub_13_1400
        or     eax, eax
        je     SHORT loc_13_26B2
        push   dword ptr [esp + 4]
        push   eax
        call   sub_13_1450
loc_13_26B2:
        ret
        db  000h
_GetDemandPageInfo:
        push   edi
        mov    edi, dword ptr [esp + 8]
        mov    dword ptr [edi], 100000h
        db  08Bh, 015h, 018h, 0EFh, 000h, 000h
        mov    dword ptr [edi + 4], edx
        db  08Bh, 00Dh, 040h, 0F9h, 000h, 000h
        mov    dword ptr [edi + 8], ecx
        db  02Bh, 015h, 01Ch, 0EFh, 000h, 000h
        mov    dword ptr [edi + 0ch], edx
        xor    ecx, ecx
        mov    dword ptr [edi + 10h], ecx
        push   400h
        call   sub_13_25E0
        mov    dword ptr [edi + 14h], eax
        db  08Bh, 015h, 034h, 0EFh, 000h, 000h
        mov    dword ptr [edi + 18h], edx
        db  08Bh, 015h, 038h, 0EFh, 000h, 000h
        mov    dword ptr [edi + 1ch], edx
        db  08Bh, 015h, 03Ch, 0EFh, 000h, 000h
        mov    dword ptr [edi + 20h], edx
        db  08Bh, 015h, 040h, 0EFh, 000h, 000h
        mov    dword ptr [edi + 24h], edx
        db  08Bh, 015h, 048h, 0EFh, 000h, 000h
        mov    dword ptr [edi + 28h], edx
        xor    ecx, ecx
        mov    dword ptr [edi + 2ch], ecx
        mov    dword ptr [edi + 30h], ecx
        db  0F6h, 005h, 030h, 0F9h, 000h, 000h, 002h
        je     SHORT loc_13_275B
        push   ebx
        push   esi
        db  0CDh, 020h, 007h, 000h, 021h, 000h
        pop    esi
        pop    ebx
        mov    dword ptr [edi + 2ch], ecx
        db  08Bh, 00Dh, 084h, 0EEh, 000h, 000h
        db  0A1h, 018h, 0EFh, 000h, 000h
        db  02Bh, 005h, 088h, 0EEh, 000h, 000h
        jae    SHORT loc_13_274C
        xor    eax, eax
loc_13_274C:
        db  023h, 005h, 064h, 0EFh, 000h, 000h
        sub    ecx, eax
        jae    SHORT loc_13_2758
        xor    ecx, ecx
loc_13_2758:
        mov    dword ptr [edi + 30h], ecx
loc_13_275B:
        db  08Bh, 015h, 084h, 0EEh, 000h, 000h
        db  003h, 015h, 088h, 0EEh, 000h, 000h
        mov    dword ptr [edi + 34h], edx
        pop    edi
        ret
_PageGetAllocInfo:
        xor    ecx, ecx
        db  0F6h, 005h, 030h, 0F9h, 000h, 000h, 002h
        je     SHORT loc_13_2781
        push   ebx
        push   esi
        db  0CDh, 020h, 007h, 000h, 021h, 000h
        pop    esi
        pop    ebx
loc_13_2781:
        db  0A1h, 018h, 0EFh, 000h, 000h
        db  02Bh, 005h, 088h, 0EEh, 000h, 000h
        jae    SHORT loc_13_2790
        xor    eax, eax
loc_13_2790:
        add    eax, ecx
        db  02Bh, 005h, 084h, 0EEh, 000h, 000h
        db  08Bh, 015h, 024h, 0EFh, 000h, 000h
        db  02Bh, 015h, 01Ch, 0EFh, 000h, 000h
        db  08Bh, 00Dh, 018h, 0EFh, 000h, 000h
        ret
        db  000h
_PTEXT ends
        end
