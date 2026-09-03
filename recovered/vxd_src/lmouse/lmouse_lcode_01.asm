; lmouse_lcode_01.asm -- LMOUSE object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

LMOUSE_Control:
        cmp    eax, 1
        db  00Fh, 084h, 01Ah, 004h, 0FFh, 0FFh
        cmp    eax, 1bh
        db  00Fh, 084h, 0FDh, 003h, 0FFh, 0FFh
        cmp    eax, 1ch
        db  00Fh, 084h, 0D1h, 003h, 0FFh, 0FFh
        cmp    eax, 0fh
        jne    SHORT loc_1_002E
        push   edx
        push   ebx
        call   sub_1_1254
        add    esp, 8
        cmp    eax, 1
        ret
loc_1_002E:
        cmp    eax, 0
        jne    SHORT loc_1_0041
        push   edx
        push   ebx
        db  0E8h, 048h, 003h, 0FFh, 0FFh
        add    esp, 8
        cmp    eax, 1
        ret
loc_1_0041:
        cmp    eax, 4
        jne    SHORT loc_1_0053
        push   ebx
        db  0E8h, 046h, 003h, 0FFh, 0FFh
        add    esp, 4
        cmp    eax, 1
        ret
loc_1_0053:
        cmp    eax, 2
        jne    SHORT loc_1_0065
        push   ebx
        db  0E8h, 044h, 003h, 0FFh, 0FFh
        add    esp, 4
        cmp    eax, 1
        ret
loc_1_0065:
        cmp    eax, 23h
        jne    SHORT loc_1_007A
        push   esi
        push   edx
        push   ebx
        push   ecx
        db  0E8h, 0C1h, 0AAh, 0FFh, 0FFh
        add    esp, 10h
        cmp    eax, 1
        ret
loc_1_007A:
        clc
        ret
sub_1_007C:
        xor    ecx, ecx
        test   eax, 8
        je     SHORT loc_1_0087
        not    ecx
loc_1_0087:
        db  089h, 00Dh, 090h, 013h, 000h, 000h
        and    eax, 0fffffff7h
        db  0CDh, 020h, 003h, 000h, 00Ch, 000h
        ret
        db  090h, 0EBh, 006h, 0FFh, 025h, 094h, 013h, 000h, 000h
loc_1_00A0:
        db  0F7h, 005h, 090h, 013h, 000h, 000h, 001h, 000h, 000h, 000h
        je     SHORT loc_1_00AF
        or     bh, 4
loc_1_00AF:
        db  0FFh, 025h, 094h, 013h, 000h, 000h
        db  000h, 000h, 000h
loc_1_00B8:
        db  0BFh, 098h, 013h, 000h, 000h
        jmp    SHORT loc_1_00E8
        db  090h
loc_1_00C0:
        db  0BFh, 0A4h, 013h, 000h, 000h
        jmp    SHORT loc_1_00E8
        db  090h
loc_1_00C8:
        db  0BFh, 0B0h, 013h, 000h, 000h
        jmp    SHORT loc_1_00E8
        db  090h
loc_1_00D0:
        db  0BFh, 0BCh, 013h, 000h, 000h
        jmp    SHORT loc_1_00E8
        db  090h
loc_1_00D8:
        db  0BFh, 0C8h, 013h, 000h, 000h
        jmp    SHORT loc_1_00E8
        db  090h
loc_1_00E0:
        db  0BFh, 0D4h, 013h, 000h, 000h
        jmp    SHORT loc_1_00E8
        db  090h
loc_1_00E8:
        push   ebp
        mov    ebp, esp
        push   ecx
        mov    ecx, edi
        mov    edi, dword ptr [edi + 8]
        push   edi
        call   dword ptr [ecx + 4]
        pop    edi
        cmp    ecx, 1
        je     SHORT loc_1_0110
        cmp    ecx, 2
        je     SHORT loc_1_010C
        mov    edi, dword ptr [edi]
        push   esi
        push   edi
        call   sub_1_0113
        add    esp, 8
loc_1_010C:
        clc
loc_1_010D:
        pop    ecx
        pop    ebp
        ret
loc_1_0110:
        stc
        jmp    SHORT loc_1_010D
sub_1_0113:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        cmp    dword ptr [ebp + 0ch], 0
        je     SHORT loc_1_0133
        push   dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_0715
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_015C
loc_1_0133:
        mov    eax, dword ptr [ebp + 8]
        lea    eax, [eax + 0afh]
        cmp    dword ptr [eax + 8], 0
        jne    SHORT loc_1_015C
        mov    dword ptr [eax + 8], 0ffffffffh
        mov    edx, dword ptr [ebp + 8]
        db  0BEh, 05Fh, 001h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        mov    dword ptr [eax + 0ch], esi
        mov    eax, esi
loc_1_015C:
        pop    esi
        leave
        ret
loc_1_015F:
        lea    edi, [edx + 0afh]
        mov    dword ptr [edi + 8], 0
        mov    eax, 0ffffffffh
        xchg   dword ptr [edi + 4], eax
        or     eax, eax
        jne    SHORT loc_1_018E
        mov    esi, edx
loc_1_017A:
        xor    eax, eax
        push   eax
        push   eax
        push   esi
        call   sub_1_08A0
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_017A
        mov    dword ptr [edi + 4], eax
loc_1_018E:
        ret
        db  000h
loc_1_0190:
        lea    esi, [edi + 10h]
        mov    edi, dword ptr [edi + 28h]
        lea    edi, [edi]
        push   esi
        call   dword ptr [edi]
        pop    esi
        cmp    cl, 10h
        je     NEAR PTR loc_1_0252
        test   ecx, 1
        je     SHORT loc_1_01B2
        call   sub_1_0280
loc_1_01B2:
        test   cl, 20h
        je     SHORT loc_1_01C7
        and    byte ptr [edi + 12h], 0fbh
        mov    dword ptr [edi + 4], 0ffffff9ch
        jmp    NEAR PTR loc_1_024C
loc_1_01C7:
        test   ecx, 2
        je     SHORT loc_1_024C
        mov    ecx, 2
        shl    al, cl
        jae    SHORT loc_1_01F6
        test   byte ptr [edi + 12h], 4
        je     SHORT loc_1_01E8
        cmp    dword ptr [edi + 4], 4
        je     SHORT loc_1_01E8
        and    byte ptr [edi + 12h], 0fbh
loc_1_01E8:
        mov    dword ptr [edi + 4], 1
        shr    al, cl
        mov    byte ptr [edi + 0fh], al
        jmp    SHORT loc_1_024C
loc_1_01F6:
        inc    dword ptr [edi + 4]
        js     SHORT loc_1_024C
        cmp    dword ptr [edi + 4], 3
        je     SHORT loc_1_0211
        jg     SHORT loc_1_0258
        mov    ah, byte ptr [edi + 0fh]
        shr    ax, cl
        xchg   ah, al
        mov    word ptr [edi + 0fh], ax
        jmp    SHORT loc_1_024C
loc_1_0211:
        mov    ah, byte ptr [edi + 0fh]
        shr    ax, cl
        movsx  ebx, al
        mov    dword ptr [esi + 4], ebx
        movsx  ebx, byte ptr [edi + 10h]
        mov    dword ptr [esi], ebx
        mov    al, ah
        and    eax, 3
        db  08Ah, 080h, 0E0h, 013h, 000h, 000h
        mov    ah, byte ptr [edi + 12h]
        and    ah, 0fch
        or     al, ah
        mov    byte ptr [edi + 12h], al
        and    eax, 7
        mov    dword ptr [esi + 8], eax
loc_1_023F:
        mov    dword ptr [esi + 0ch], 2
        mov    ecx, 3
        ret
loc_1_024C:
        mov    ecx, 2
        ret
loc_1_0252:
        mov    ecx, 1
        ret
loc_1_0258:
        cmp    dword ptr [edi + 4], 4
        ja     SHORT loc_1_024C
        shr    al, 5
        and    al, 4
        mov    ah, byte ptr [edi + 12h]
        and    ah, 0fbh
        or     al, ah
        mov    byte ptr [edi + 12h], al
        and    eax, 7
        mov    dword ptr [esi + 8], eax
        xor    eax, eax
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 4], eax
        jmp    SHORT loc_1_023F
        db  02Eh, 08Bh, 0C0h
sub_1_0280:
        push   eax
        push   ecx
        push   esi
        test   dword ptr [edi + 13h], 1
        je     SHORT loc_1_02A3
        mov    cl, ch
        and    ecx, 20h
        xor    ecx, dword ptr [edi + 27h]
        jne    SHORT loc_1_02A3
        mov    edx, edi
        db  0BEh, 0A8h, 002h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
loc_1_02A3:
        pop    esi
        pop    ecx
        pop    eax
        ret
        db  090h
loc_1_02A8:
        mov    edi, edx
        mov    dx, word ptr [edi + 0ch]
        add    dx, 6
        in     al, dx
        and    eax, 20h
        xor    eax, dword ptr [edi + 27h]
        jne    SHORT loc_1_02C7
        mov    edx, dword ptr [edi + 17h]
        push   edx
        db  0E8h, 061h, 026h, 0FFh, 0FFh
        add    esp, 4
loc_1_02C7:
        ret
loc_1_02C8:
        lea    esi, [edi + 10h]
        mov    edi, dword ptr [edi + 28h]
        lea    edi, [edi]
        push   esi
        call   dword ptr [edi]
        pop    esi
        cmp    cl, 10h
        je     NEAR PTR loc_1_0364
        test   ecx, 20h
        je     SHORT loc_1_02EE
        mov    dword ptr [edi + 4], 0ffffff9ch
        jmp    SHORT loc_1_035E
loc_1_02EE:
        test   ecx, 2
        je     SHORT loc_1_035E
        test   al, 80h
        jne    SHORT loc_1_0310
        inc    dword ptr [edi + 4]
        js     SHORT loc_1_035E
        cmp    dword ptr [edi + 4], 3
        je     SHORT loc_1_0326
        jb     SHORT loc_1_0321
        mov    dword ptr [edi + 4], 0ffffff9ch
        jmp    SHORT loc_1_035E
loc_1_0310:
        and    al, 7fh
        shl    al, 3
        mov    dword ptr [edi + 4], 1
        mov    byte ptr [edi + 0fh], al
        jmp    SHORT loc_1_035E
loc_1_0321:
        mov    byte ptr [edi + 10h], al
        jmp    SHORT loc_1_035E
loc_1_0326:
        movsx  ebx, byte ptr [edi + 10h]
        mov    ah, byte ptr [edi + 0fh]
        shl    ah, 1
        jb     SHORT loc_1_0333
        neg    ebx
loc_1_0333:
        mov    dword ptr [esi], ebx
        shl    ah, 1
        jae    SHORT loc_1_033B
        neg    al
loc_1_033B:
        movsx  ebx, al
        mov    dword ptr [esi + 4], ebx
        rol    ax, 3
        and    eax, 7
        db  08Ah, 080h, 0E4h, 013h, 000h, 000h
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [esi + 0ch], 2
        mov    ecx, 3
        ret
loc_1_035E:
        mov    ecx, 2
        ret
loc_1_0364:
        mov    ecx, 1
        ret
        db  000h, 000h
loc_1_036C:
        push   ebx
        push   eax
        mov    dx, word ptr [edi + 0ch]
        add    dx, 2
        in     al, dx
        mov    ebx, eax
        test   al, 1
        pop    eax
        mov    cl, 10h
        jne    NEAR PTR loc_1_0405
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        xor    ecx, ecx
        and    ebx, 6
        jne    SHORT loc_1_03AE
        mov    dx, word ptr [edi + 0ch]
        in     al, dx
        jmp    SHORT loc_1_0398
loc_1_0398:
        jmp    SHORT loc_1_039A
loc_1_039A:
        jmp    SHORT loc_1_039C
loc_1_039C:
        jmp    SHORT loc_1_039E
loc_1_039E:
        mov    dx, word ptr [edi + 0ch]
        add    dx, 6
        in     al, dx
        mov    ch, al
        or     ecx, 1
        jmp    SHORT loc_1_0405
loc_1_03AE:
        mov    dx, word ptr [edi + 0ch]
        add    dx, 5
        in     al, dx
        mov    bh, al
        jmp    SHORT loc_1_03BB
loc_1_03BB:
        jmp    SHORT loc_1_03BD
loc_1_03BD:
        jmp    SHORT loc_1_03BF
loc_1_03BF:
        jmp    SHORT loc_1_03C1
loc_1_03C1:
        mov    dx, word ptr [edi + 0ch]
        add    edx, 6
        in     al, dx
        mov    bl, al
        jmp    SHORT loc_1_03CD
loc_1_03CD:
        jmp    SHORT loc_1_03CF
loc_1_03CF:
        jmp    SHORT loc_1_03D1
loc_1_03D1:
        jmp    SHORT loc_1_03D3
loc_1_03D3:
        mov    dx, word ptr [edi + 0ch]
        in     al, dx
        or     ecx, 2
        test   bh, 1eh
        je     SHORT loc_1_03F0
        jmp    SHORT loc_1_03E2
loc_1_03E2:
        jmp    SHORT loc_1_03E4
loc_1_03E4:
        jmp    SHORT loc_1_03E6
loc_1_03E6:
        jmp    SHORT loc_1_03E8
loc_1_03E8:
        mov    dx, word ptr [edi + 0ch]
        in     al, dx
        or     ecx, 20h
loc_1_03F0:
        test   bl, 8
        je     SHORT loc_1_0405
        jmp    SHORT loc_1_03F7
loc_1_03F7:
        jmp    SHORT loc_1_03F9
loc_1_03F9:
        jmp    SHORT loc_1_03FB
loc_1_03FB:
        jmp    SHORT loc_1_03FD
loc_1_03FD:
        mov    dx, word ptr [edi + 0ch]
        in     al, dx
        or     ecx, 20h
loc_1_0405:
        pop    ebx
        ret
        db  000h
loc_1_0408:
        lea    esi, [edi + 10h]
        mov    edi, dword ptr [edi + 28h]
        lea    edi, [edi]
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        mov    bx, word ptr [edi + 8]
        mov    dx, bx
        add    dx, 2
        mov    ecx, 4
        mov    al, 80h
        out    dx, al
        jmp    SHORT loc_1_042B
loc_1_042B:
        jmp    SHORT loc_1_042D
loc_1_042D:
        jmp    SHORT loc_1_042F
loc_1_042F:
        jmp    SHORT loc_1_0431
loc_1_0431:
        jmp    SHORT loc_1_0433
loc_1_0433:
        xchg   bx, dx
        in     al, dx
        jmp    SHORT loc_1_0439
loc_1_0439:
        jmp    SHORT loc_1_043B
loc_1_043B:
        jmp    SHORT loc_1_043D
loc_1_043D:
        jmp    SHORT loc_1_043F
loc_1_043F:
        jmp    SHORT loc_1_0441
loc_1_0441:
        ror    ax, cl
        xchg   bx, dx
        mov    al, 0a0h
        out    dx, al
        jmp    SHORT loc_1_044C
loc_1_044C:
        jmp    SHORT loc_1_044E
loc_1_044E:
        jmp    SHORT loc_1_0450
loc_1_0450:
        jmp    SHORT loc_1_0452
loc_1_0452:
        jmp    SHORT loc_1_0454
loc_1_0454:
        xchg   bx, dx
        in     al, dx
        jmp    SHORT loc_1_045A
loc_1_045A:
        jmp    SHORT loc_1_045C
loc_1_045C:
        jmp    SHORT loc_1_045E
loc_1_045E:
        jmp    SHORT loc_1_0460
loc_1_0460:
        jmp    SHORT loc_1_0462
loc_1_0462:
        rol    ax, cl
        cbw
        cwde
        mov    dword ptr [esi], eax
        xchg   bx, dx
        mov    al, 0c0h
        out    dx, al
        jmp    SHORT loc_1_0472
loc_1_0472:
        jmp    SHORT loc_1_0474
loc_1_0474:
        jmp    SHORT loc_1_0476
loc_1_0476:
        jmp    SHORT loc_1_0478
loc_1_0478:
        jmp    SHORT loc_1_047A
loc_1_047A:
        xchg   bx, dx
        in     al, dx
        jmp    SHORT loc_1_0480
loc_1_0480:
        jmp    SHORT loc_1_0482
loc_1_0482:
        jmp    SHORT loc_1_0484
loc_1_0484:
        jmp    SHORT loc_1_0486
loc_1_0486:
        jmp    SHORT loc_1_0488
loc_1_0488:
        ror    ax, cl
        xchg   bx, dx
        mov    al, 0e0h
        out    dx, al
        jmp    SHORT loc_1_0493
loc_1_0493:
        jmp    SHORT loc_1_0495
loc_1_0495:
        jmp    SHORT loc_1_0497
loc_1_0497:
        jmp    SHORT loc_1_0499
loc_1_0499:
        jmp    SHORT loc_1_049B
loc_1_049B:
        xchg   bx, dx
        in     al, dx
        jmp    SHORT loc_1_04A1
loc_1_04A1:
        jmp    SHORT loc_1_04A3
loc_1_04A3:
        jmp    SHORT loc_1_04A5
loc_1_04A5:
        jmp    SHORT loc_1_04A7
loc_1_04A7:
        jmp    SHORT loc_1_04A9
loc_1_04A9:
        xchg   bx, dx
        rol    ax, cl
        push   eax
        cbw
        cwde
        mov    dword ptr [esi + 4], eax
        pop    eax
        ror    ax, 9
        not    eax
        and    eax, 7
        db  08Ah, 080h, 0ECh, 013h, 000h, 000h
        mov    dword ptr [esi + 8], eax
        push   eax
        xor    al, al
        out    dx, al
        pop    eax
        mov    ecx, dword ptr [edi]
        mov    dword ptr [edi], eax
        xor    ecx, eax
        or     ecx, dword ptr [esi]
        or     ecx, dword ptr [esi + 4]
        jecxz  loc_1_04E8
        mov    dword ptr [esi + 0ch], 2
        mov    ecx, 3
        ret
loc_1_04E8:
        mov    ecx, 2
        ret
        db  08Bh, 0C0h
loc_1_04F0:
        mov    eax, 0ffffffffh
        ret
        db  000h, 000h
loc_1_04F8:
        lea    esi, [edi + 10h]
        mov    edi, dword ptr [edi + 28h]
        lea    edi, [edi]
        push   eax
        db  083h, 03Dh, 0F8h, 013h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_05C5
        db  083h, 03Dh, 0F4h, 013h, 000h, 000h, 000h
        je     SHORT loc_1_0539
        mov    dx, 311h
        in     al, dx
        test   al, 2
        je     SHORT loc_1_0539
        mov    ah, al
        mov    dx, 310h
        in     al, dx
        test   ah, 20h
        je     SHORT loc_1_0549
        cmp    al, 0aah
        je     SHORT loc_1_0549
        pop    eax
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        jmp    SHORT loc_1_0582
loc_1_0539:
        in     al, 64h
        test   al, 1
        je     NEAR PTR loc_1_05C5
        test   al, 20h
        je     SHORT loc_1_05C5
        in     al, 60h
loc_1_0549:
        mov    edx, dword ptr [edi + 4]
        shl    edx, 8
        mov    dl, al
        mov    dword ptr [edi + 4], edx
        inc    dword ptr [edi]
        pop    eax
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        mov    ebx, eax
        xchg   dword ptr [edi + 8], eax
        sub    ebx, eax
        cmp    ebx, 5ah
        jb     SHORT loc_1_0575
        mov    dword ptr [edi], 1
loc_1_0575:
        cmp    dword ptr [edi], 3
        je     SHORT loc_1_0584
        jb     SHORT loc_1_0582
        mov    dword ptr [edi], 0
loc_1_0582:
        jmp    SHORT loc_1_05BF
loc_1_0584:
        mov    eax, edx
        shr    eax, 10h
        mov    dword ptr [edi], 0
        mov    cl, dh
        bt     ax, 4
        sbb    ch, ch
        movsx  ecx, cx
        mov    dword ptr [esi], ecx
        bt     ax, 5
        sbb    dh, dh
        movsx  edx, dx
        neg    edx
        mov    dword ptr [esi + 4], edx
        and    eax, 7
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [esi + 0ch], 2
        mov    ecx, 3
        ret
loc_1_05BF:
        mov    ecx, 2
        ret
loc_1_05C5:
        pop    eax
        mov    ecx, 1
        ret
loc_1_05CC:
        mov    ax, word ptr [ebp + 1ch]
        cmp    ah, 0c2h
        jne    SHORT loc_1_05DD
        cmp    al, 7
        db  00Fh, 086h, 0FFh, 033h, 0FFh, 0FFh
loc_1_05DD:
        stc
        ret
        db  0F8h, 0C3h, 000h, 000h, 000h
sub_1_05E4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    esi, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    edi, dword ptr [ebx + 8]
        mov    ecx, 8
        push   ecx
        push   edi
        cld
        rep movsd
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        pop    esi
        mov    eax, esi
        mov    edi, dword ptr [ebp + 10h]
        pop    ecx
        cld
        rep movsd
        mov    eax, dword ptr [eax + 2ch]
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0645:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    esi, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    edi, dword ptr [ebx + 8]
        mov    ebx, edi
        mov    ecx, 8
        push   ecx
        push   edi
        cld
        rep movsd
        mov    esi, dword ptr [ebp + 14h]
        mov    ax, word ptr [esi]
        mov    word ptr [ebx + 3ch], ax
        mov    ax, word ptr [esi + 2]
        mov    word ptr [ebx + 38h], ax
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    ax, word ptr [ebx + 3ch]
        mov    word ptr [esi], ax
        mov    ax, word ptr [ebx + 38h]
        mov    word ptr [esi + 2], ax
        pop    esi
        mov    eax, esi
        mov    edi, dword ptr [ebp + 10h]
        pop    ecx
        cld
        rep movsd
        mov    eax, dword ptr [eax + 2ch]
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h
sub_1_06CC:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        mov    edi, esi
        imul   edi, dword ptr [ebp + 0ch]
        push   1
        push   edi
        call   sub_1_12F4
        mov    ecx, dword ptr [ebp + 8]
        add    esp, 8
        mov    dword ptr [ecx], eax
        mov    dword ptr [ecx + 0ch], eax
        mov    dword ptr [ecx + 8], eax
        add    eax, edi
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ecx + 10h], esi
        cmp    dword ptr [ecx], 1
        sbb    eax, eax
        inc    eax
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_1_0702:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   dword ptr [eax]
        call   sub_1_12FC
        mov    esp, ebp
        pop    ebp
        ret
sub_1_0715:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        sub    eax, eax
        mov    edi, dword ptr [edx + 8]
        mov    ebx, dword ptr [edx + 10h]
        add    ebx, edi
        cmp    dword ptr [edx + 4], ebx
        jne    SHORT loc_1_0732
        mov    ebx, dword ptr [edx]
loc_1_0732:
        cmp    dword ptr [edx + 0ch], ebx
        je     SHORT loc_1_0755
        mov    eax, 1
        mov    ecx, dword ptr [edx + 10h]
        mov    dword ptr [ebp - 4], ecx
        mov    esi, dword ptr [ebp + 0ch]
        shr    ecx, 2
        rep movsd
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 3
        rep movsb
        mov    dword ptr [edx + 8], ebx
loc_1_0755:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_075C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        sub    eax, eax
        mov    esi, dword ptr [edx + 0ch]
        cmp    dword ptr [edx + 8], esi
        je     SHORT loc_1_079B
        mov    eax, 1
        mov    ebx, dword ptr [edx + 10h]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [edx + 0ch]
        add    ecx, dword ptr [edx + 10h]
        mov    dword ptr [edx + 0ch], ecx
        cmp    dword ptr [edx + 4], ecx
        jne    SHORT loc_1_079B
        mov    ecx, dword ptr [edx]
        mov    dword ptr [edx + 0ch], ecx
loc_1_079B:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_07A0:
        push   ebp
        mov    ebp, esp
        push   esi
        sub    esi, esi
        push   1
        push   14h
        call   sub_1_12F4
        mov    ecx, dword ptr [ebp + 8]
        add    esp, 8
        mov    dword ptr [ecx], eax
        mov    ecx, eax
        cmp    ecx, esi
        je     SHORT loc_1_07CD
        push   10h
        push   dword ptr [ebp + 0ch]
        push   ecx
        call   sub_1_06CC
        add    esp, 0ch
        mov    esi, eax
loc_1_07CD:
        mov    eax, esi
        pop    esi
        pop    ebp
        ret
sub_1_07D2:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0702
        add    esp, 4
        push   0
        push   esi
        call   sub_1_12FC
        add    esp, 8
        pop    esi
        pop    ebp
        ret
sub_1_07F0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 24h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ecx + eax*4]
        pop    ebp
        ret
sub_1_0801:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0ch]
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_0830
        db  0BFh, 000h, 000h, 000h, 000h
loc_1_0817:
        mov    eax, dword ptr [esi + 0ch]
        mov    ecx, dword ptr [edi]
        cmp    dword ptr [eax], ecx
        je     SHORT loc_1_0825
        add    edi, 20h
        jmp    SHORT loc_1_0817
loc_1_0825:
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [eax], 0
        jmp    SHORT loc_1_085B
loc_1_0830:
        mov    eax, dword ptr [ebp + 10h]
        or     eax, eax
        db  0BFh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_0852
        push   dword ptr [ebp + 0ch]
        push   dword ptr [esi + 10h]
        call   sub_1_075C
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_085B
        sub    eax, eax
        jmp    SHORT loc_1_089B
loc_1_0852:
        mov    ecx, edi
        add    edi, 20h
        cmp    dword ptr [ecx], eax
        jne    SHORT loc_1_0852
loc_1_085B:
        mov    ebx, dword ptr [ebp + 14h]
        cmp    dword ptr [edi], ebx
        je     SHORT loc_1_0896
loc_1_0862:
        db  081h, 0FFh, 080h, 001h, 000h, 000h
        jae    SHORT loc_1_0896
        test   byte ptr [edi + 4], 4
        jne    SHORT loc_1_088F
        push   dword ptr [ebp + 0ch]
        mov    eax, edi
        db  02Dh, 000h, 000h, 000h, 000h
        and    eax, 0ffffffe7h
        sar    eax, 3
        mov    ecx, dword ptr [esi + 24h]
        push   dword ptr [eax + ecx]
        call   dword ptr [edi]
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_0896
loc_1_088F:
        add    edi, 20h
        cmp    dword ptr [edi], ebx
        jne    SHORT loc_1_0862
loc_1_0896:
        mov    eax, 1
loc_1_089B:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_08A0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0ch]
        cmp    dword ptr [eax + 4], 0
        mov    eax, 0
        jne    SHORT loc_1_08C6
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ecx + 8]
        push   ecx
        call   sub_1_0801
        add    esp, 10h
loc_1_08C6:
        pop    ebp
        ret
sub_1_08C8:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 0ch]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        pop    ebp
        ret
sub_1_08D8:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        or     edi, edi
        jne    SHORT loc_1_08F4
        push   0
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0113
        add    esp, 8
        jmp    SHORT loc_1_08F7
loc_1_08F4:
        mov    esi, dword ptr [ebp + 8]
loc_1_08F7:
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [eax + 4], edi
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  000h, 000h, 000h
loc_1_0904:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        sub    edi, edi
        sub    esi, esi
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
loc_1_0913:
        mov    eax, dword ptr [ecx + 8]
        or     eax, eax
        je     SHORT loc_1_092E
        test   al, 1
        je     SHORT loc_1_0920
        or     edi, dword ptr [edx]
loc_1_0920:
        shr    eax, 1
        mov    dword ptr [ecx + 8], eax
        add    edx, 4
        inc    esi
        cmp    esi, 20h
        jb     SHORT loc_1_0913
loc_1_092E:
        mov    dword ptr [ecx + 8], edi
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  000h, 000h
loc_1_093C:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        cmp    dword ptr [edi], 0
        je     NEAR PTR loc_1_09EB
        mov    esi, dword ptr [ebp + 0ch]
        test   byte ptr [esi + 0ch], 1
        jne    NEAR PTR loc_1_09EB
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp - 28h], eax
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ebp - 2ch], eax
        mov    eax, dword ptr [ebp - 2ch]
        mov    ecx, dword ptr [ebp - 28h]
        imul   ecx
        mov    ebx, eax
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ebp - 20h], eax
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [ebp - 24h], eax
        mov    eax, dword ptr [ebp - 24h]
        mov    ecx, dword ptr [ebp - 20h]
        imul   ecx
        sub    ebx, eax
        add    ebx, dword ptr [edi + 0ch]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp - 18h]
        imul   ecx
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 10h]
        imul   ecx
        mov    ecx, dword ptr [edi + 10h]
        add    ecx, eax
        add    ecx, dword ptr [ebp - 0ch]
        mov    eax, ebx
        and    eax, 0ffffh
        mov    dword ptr [edi + 0ch], eax
        mov    eax, ecx
        and    eax, 0ffffh
        mov    dword ptr [edi + 10h], eax
        mov    dword ptr [ebp - 8], ebx
        movzx  eax, word ptr [ebp - 6]
        movsx  eax, ax
        mov    dword ptr [esi], eax
        mov    dword ptr [ebp - 4], ecx
        movzx  eax, word ptr [ebp - 2]
        movsx  eax, ax
        mov    dword ptr [esi + 4], eax
loc_1_09EB:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  000h
loc_1_09F8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 8], 0
        je     SHORT loc_1_0A71
        push   1
        push   dword ptr [esi]
        call   sub_1_07F0
        add    esp, 8
        cmp    dword ptr [eax + 4], 0
        jne    SHORT loc_1_0A71
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [esi + 4]
        mov    edx, dword ptr [ecx + 8]
        xor    edi, edx
        mov    dword ptr [esi + 4], edx
        mov    eax, dword ptr [esi + 0ch]
        or     eax, dword ptr [esi + 8]
        mov    ebx, edx
        not    ebx
        and    eax, ebx
        test   edi, eax
        je     SHORT loc_1_0A4F
        mov    eax, dword ptr [esi + 14h]
        and    eax, edi
        test   dword ptr [esi + 8], eax
        jne    SHORT loc_1_0A4F
        mov    dword ptr [esi + 10h], 0
        mov    dword ptr [esi + 14h], 0
loc_1_0A4F:
        mov    eax, dword ptr [esi + 8]
        and    eax, edx
        and    eax, edi
        je     SHORT loc_1_0A61
        xor    dword ptr [esi + 14h], eax
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [esi + 10h], eax
loc_1_0A61:
        mov    eax, dword ptr [esi + 8]
        not    eax
        or     eax, dword ptr [esi + 0ch]
        and    eax, edx
        or     eax, dword ptr [esi + 10h]
        mov    dword ptr [ecx + 8], eax
loc_1_0A71:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  000h
loc_1_0A7C:
        push   ebp
        mov    ebp, esp
        sub    esp, 30h
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        test   byte ptr [edi + 0ch], 1
        jne    SHORT loc_1_0B0A
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ebp - 2ch], eax
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebp - 30h], eax
        mov    eax, dword ptr [ebp - 30h]
        mov    ecx, dword ptr [ebp - 2ch]
        imul   ecx
        lea    ecx, [esi + 8]
        mov    dword ptr [ebp - 1ch], ecx
        mov    dword ptr [ebp - 20h], edi
        mov    dword ptr [ebp - 24h], 320h
        mov    ecx, dword ptr [ecx]
        add    ecx, eax
        mov    dword ptr [ebp - 28h], ecx
        mov    eax, dword ptr [ebp - 28h]
        cdq
        idiv   dword ptr [ebp - 24h]
        mov    ebx, dword ptr [ebp - 20h]
        mov    dword ptr [ebx], eax
        mov    ebx, dword ptr [ebp - 1ch]
        mov    dword ptr [ebx], edx
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [ebp - 14h], eax
        add    edi, 4
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 14h]
        imul   ecx
        add    esi, 10h
        mov    dword ptr [ebp - 4], esi
        mov    dword ptr [ebp - 8], edi
        mov    dword ptr [ebp - 0ch], 320h
        mov    ecx, dword ptr [esi]
        add    ecx, eax
        mov    dword ptr [ebp - 10h], ecx
        mov    eax, dword ptr [ebp - 10h]
        cdq
        idiv   dword ptr [ebp - 0ch]
        mov    ebx, dword ptr [ebp - 8]
        mov    dword ptr [ebx], eax
        mov    ebx, dword ptr [ebp - 4]
        mov    dword ptr [ebx], edx
loc_1_0B0A:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  000h, 000h
loc_1_0B18:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], 1
        mov    ebx, dword ptr [esi]
        cmp    dword ptr [esi + 8], 0
        je     SHORT loc_1_0B89
        cmp    dword ptr [esi + 18h], 0
        je     SHORT loc_1_0B89
        push   1
        push   ebx
        call   sub_1_07F0
        add    esp, 8
        cmp    dword ptr [eax + 4], 0
        jne    SHORT loc_1_0B89
        mov    eax, dword ptr [esi + 14h]
        or     eax, eax
        je     SHORT loc_1_0B93
        cmp    eax, 1
        je     NEAR PTR loc_1_0C00
        cmp    eax, 2
        je     NEAR PTR loc_1_0D11
        cmp    eax, 3
        je     NEAR PTR loc_1_0D3E
        cmp    eax, 4
        je     NEAR PTR loc_1_0D59
        mov    edi, dword ptr [ebp + 0ch]
loc_1_0B78:
        test   byte ptr [edi + 0ch], 1
        je     SHORT loc_1_0B89
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 1ch], eax
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [esi + 20h], eax
loc_1_0B89:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_0B93:
        mov    dword ptr [esi + 10h], 0
        mov    edi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [esi + 8]
        mov    eax, ecx
        mov    edx, dword ptr [edi + 8]
        and    eax, edx
        cmp    eax, ecx
        jne    SHORT loc_1_0BC3
        not    ecx
        and    ecx, edx
        mov    dword ptr [edi + 8], ecx
        mov    eax, dword ptr [esi + 0ch]
        or     eax, ecx
        mov    dword ptr [edi + 8], eax
        mov    dword ptr [esi + 14h], 2
        jmp    SHORT loc_1_0B78
loc_1_0BC3:
        or     eax, eax
        je     SHORT loc_1_0B78
        and    edx, ecx
        mov    dword ptr [esi + 10h], edx
        mov    dword ptr [esi + 14h], 1
        push   1
        push   ebx
        call   sub_1_08D8
        add    esp, 8
        db  068h, 000h, 000h, 000h, 000h
        push   5
        push   ebx
        call   sub_1_1229
        add    esp, 0ch
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [esi + 28h], eax
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_0B78
loc_1_0C00:
        db  068h, 000h, 000h, 000h, 000h
        push   0
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        push   ebx
        call   sub_1_0801
        add    esp, 10h
        mov    dword ptr [esi + 24h], eax
        or     eax, eax
        je     NEAR PTR loc_1_0CA5
        mov    eax, dword ptr [esi + 10h]
        cmp    eax, dword ptr [edi + 8]
        jne    SHORT loc_1_0C72
        test   byte ptr [edi + 0ch], 1
        jne    SHORT loc_1_0C4C
        mov    eax, dword ptr [edi]
        cdq
        xor    eax, edx
        sub    eax, edx
        cmp    eax, 2
        jge    SHORT loc_1_0C46
        mov    eax, dword ptr [edi + 4]
        cdq
        xor    eax, edx
        sub    eax, edx
        cmp    eax, 2
        jl     SHORT loc_1_0C6B
loc_1_0C46:
        test   byte ptr [edi + 0ch], 1
        je     SHORT loc_1_0C72
loc_1_0C4C:
        mov    eax, dword ptr [esi + 1ch]
        sub    eax, dword ptr [edi]
        cdq
        xor    eax, edx
        sub    eax, edx
        cmp    eax, 2
        jge    SHORT loc_1_0C72
        mov    eax, dword ptr [esi + 20h]
        sub    eax, dword ptr [edi + 4]
        cdq
        xor    eax, edx
        sub    eax, edx
        cmp    eax, 2
        jge    SHORT loc_1_0C72
loc_1_0C6B:
        mov    dword ptr [esi + 24h], 0
loc_1_0C72:
        mov    dword ptr [ebp - 4], 1
        cmp    dword ptr [esi + 24h], 0
        je     SHORT loc_1_0CFA
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [edi + 8]
        mov    edx, eax
        and    edx, ecx
        cmp    edx, eax
        jne    SHORT loc_1_0CCE
        not    eax
        and    eax, ecx
        mov    dword ptr [edi + 8], eax
        mov    ecx, dword ptr [esi + 0ch]
        or     ecx, eax
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [esi + 14h], 2
        jmp    SHORT loc_1_0D01
loc_1_0CA5:
        mov    eax, dword ptr [esi + 28h]
        or     eax, eax
        lea    ecx, [eax - 1]
        mov    dword ptr [esi + 28h], ecx
        je     SHORT loc_1_0C72
        db  068h, 000h, 000h, 000h, 000h
        push   5
        push   ebx
        call   sub_1_1229
        add    esp, 0ch
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_0B78
loc_1_0CCE:
        mov    eax, dword ptr [esi + 10h]
        mov    edx, ecx
        not    edx
        test   eax, edx
        je     SHORT loc_1_0CFA
        xor    ecx, eax
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [esi + 14h], 4
        db  068h, 000h, 000h, 000h, 000h
        push   5
        push   ebx
        call   sub_1_1229
        add    esp, 0ch
        jmp    NEAR PTR loc_1_0B78
loc_1_0CFA:
        mov    dword ptr [esi + 14h], 3
loc_1_0D01:
        push   0
        push   ebx
        call   sub_1_08D8
        add    esp, 8
        jmp    NEAR PTR loc_1_0B78
loc_1_0D11:
        mov    edi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [esi + 8]
        mov    eax, dword ptr [edi + 8]
        test   eax, ecx
        jne    SHORT loc_1_0D2A
        mov    dword ptr [esi + 14h], 0
        jmp    NEAR PTR loc_1_0B78
loc_1_0D2A:
        not    ecx
        and    ecx, eax
        mov    dword ptr [edi + 8], ecx
        mov    eax, dword ptr [esi + 0ch]
        or     eax, ecx
        mov    dword ptr [edi + 8], eax
        jmp    NEAR PTR loc_1_0B78
loc_1_0D3E:
        mov    edi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [edi + 8]
        test   dword ptr [esi + 10h], eax
        jne    NEAR PTR loc_1_0B78
        mov    dword ptr [esi + 14h], 0
        jmp    NEAR PTR loc_1_0B78
loc_1_0D59:
        mov    edi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [esi + 10h]
        xor    dword ptr [edi + 8], eax
        mov    dword ptr [esi + 14h], 0
        push   0
        push   ebx
        call   sub_1_08D8
        add    esp, 8
        jmp    NEAR PTR loc_1_0B78
        db  000h, 000h, 000h
loc_1_0D7C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [esi]
        cmp    dword ptr [esi + 8], 0
        je     NEAR PTR loc_1_0EAE
        push   1
        push   ebx
        call   sub_1_07F0
        add    esp, 8
        cmp    dword ptr [eax + 4], 0
        jne    NEAR PTR loc_1_0EAE
        mov    eax, dword ptr [esi + 10h]
        or     eax, eax
        je     SHORT loc_1_0DD2
        cmp    eax, 1
        je     SHORT loc_1_0E2B
        cmp    eax, 2
        je     NEAR PTR loc_1_0E4D
        cmp    eax, 3
        je     NEAR PTR loc_1_0E6C
        cmp    eax, 4
        je     NEAR PTR loc_1_0E89
        jmp    NEAR PTR loc_1_0EAE
loc_1_0DD2:
        mov    edi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [esi + 8]
        test   dword ptr [edi + 8], eax
        je     NEAR PTR loc_1_0EAE
        push   1
        push   ebx
        call   sub_1_08D8
        add    esp, 8
        inc    dword ptr [esi + 10h]
        mov    eax, dword ptr [esi + 0ch]
        or     eax, dword ptr [esi + 8]
        not    eax
        and    eax, dword ptr [edi + 8]
        mov    dword ptr [esi + 4], eax
        test   byte ptr [edi + 0ch], 1
        jne    SHORT loc_1_0E10
        mov    dword ptr [edi], 0
        mov    dword ptr [edi + 4], 0
loc_1_0E10:
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [edi + 8], eax
        db  068h, 000h, 000h, 000h, 000h
        push   5
        push   ebx
        call   sub_1_1229
        add    esp, 0ch
        jmp    NEAR PTR loc_1_0EAE
loc_1_0E2B:
        inc    eax
        mov    dword ptr [esi + 10h], eax
        mov    eax, dword ptr [esi + 4]
        mov    edi, dword ptr [ebp + 0ch]
        or     eax, dword ptr [esi + 0ch]
        mov    dword ptr [edi + 8], eax
        db  068h, 000h, 000h, 000h, 000h
        push   5
        push   ebx
        call   sub_1_1229
        add    esp, 0ch
        jmp    SHORT loc_1_0EAE
loc_1_0E4D:
        inc    eax
        mov    dword ptr [esi + 10h], eax
        mov    edi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [edi + 8], eax
        db  068h, 000h, 000h, 000h, 000h
        push   5
        push   ebx
        call   sub_1_1229
        add    esp, 0ch
        jmp    SHORT loc_1_0EAE
loc_1_0E6C:
        inc    eax
        mov    dword ptr [esi + 10h], eax
        push   0
        push   ebx
        call   sub_1_08D8
        add    esp, 8
        mov    eax, dword ptr [esi + 4]
        mov    edi, dword ptr [ebp + 0ch]
        or     eax, dword ptr [esi + 0ch]
        mov    dword ptr [edi + 8], eax
        jmp    SHORT loc_1_0EAE
loc_1_0E89:
        mov    edi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [esi + 8]
        mov    eax, dword ptr [edi + 8]
        test   eax, ecx
        je     SHORT loc_1_0EA7
        not    ecx
        and    ecx, eax
        mov    dword ptr [edi + 8], ecx
        mov    eax, dword ptr [esi + 0ch]
        or     eax, ecx
        mov    dword ptr [edi + 8], eax
        jmp    SHORT loc_1_0EAE
loc_1_0EA7:
        mov    dword ptr [esi + 10h], 0
loc_1_0EAE:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_0EB8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 4], 0
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_0F32
        call   sub_1_1297
        mov    eax, dword ptr [eax + 4]
        movzx  esi, word ptr [eax + 417h]
        test   esi, 30fh
        je     SHORT loc_1_0F32
        sub    edi, edi
        sub    ecx, ecx
        mov    eax, dword ptr [ebp + 8]
        db  0BAh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
loc_1_0EF8:
        mov    eax, 1
        shl    eax, cl
        test   dword ptr [ebp - 4], eax
        je     SHORT loc_1_0F18
        movzx  ebx, word ptr [edx]
        and    ebx, esi
        movzx  eax, word ptr [edx + 2]
        xor    ebx, eax
        cmp    ebx, 1
        sbb    eax, eax
        neg    eax
        or     edi, eax
loc_1_0F18:
        add    edx, 4
        inc    ecx
        db  081h, 0FAh, 018h, 000h, 000h, 000h
        jb     SHORT loc_1_0EF8
        or     edi, edi
        je     SHORT loc_1_0F32
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 4], 1
loc_1_0F32:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  000h, 000h
sub_1_0F40:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx]
        cdq
        xor    eax, edx
        sub    eax, edx
        mov    ecx, dword ptr [esi]
        imul   ecx, ecx, 41h
        movzx  edx, byte ptr [esi + ecx + 4]
        dec    edx
        add    ecx, esi
        lea    ecx, [edx + ecx + 5]
        mov    edi, edx
        movzx  ebx, byte ptr [ecx]
        cmp    ebx, eax
        jbe    SHORT loc_1_0F7E
loc_1_0F70:
        or     edi, edi
        je     SHORT loc_1_0F7E
        dec    edi
        movzx  ebx, byte ptr [ecx - 1]
        dec    ecx
        cmp    ebx, eax
        ja     SHORT loc_1_0F70
loc_1_0F7E:
        cmp    edi, edx
        jb     SHORT loc_1_0F87
        mov    dword ptr [ebp - 4], edi
        jmp    SHORT loc_1_0F8D
loc_1_0F87:
        lea    ecx, [edi + 1]
        mov    dword ptr [ebp - 4], ecx
loc_1_0F8D:
        mov    dword ptr [esi + 108h], 0
        mov    ebx, dword ptr [esi]
        imul   ebx, ebx, 41h
        lea    ecx, [esi + edi]
        lea    edx, [ebx + ecx]
        add    ebx, esi
        mov    ecx, dword ptr [ebp - 4]
        add    ebx, ecx
        mov    cl, byte ptr [edx + 5]
        cmp    byte ptr [ebx + 5], cl
        je     SHORT loc_1_0FF2
        movzx  ebx, byte ptr [ebx + 25h]
        movzx  ecx, byte ptr [edx + 25h]
        sub    ebx, ecx
        movzx  ecx, byte ptr [edx + 5]
        sub    eax, ecx
        imul   ebx, eax
        shl    ebx, 4
        mov    eax, ebx
        mov    dword ptr [esi + 108h], eax
        mov    edx, dword ptr [esi]
        imul   edx, edx, 41h
        lea    ebx, [esi + edx]
        mov    ecx, dword ptr [ebp - 4]
        movzx  ebx, byte ptr [ebx + ecx + 5]
        lea    ecx, [esi + edi]
        movzx  ecx, byte ptr [edx + ecx + 5]
        sub    ebx, ecx
        cdq
        idiv   ebx
        mov    dword ptr [esi + 108h], eax
loc_1_0FF2:
        mov    dword ptr [ebp - 20h], 10h
        mov    eax, dword ptr [esi]
        imul   eax, eax, 41h
        add    eax, edi
        movzx  eax, byte ptr [eax + esi + 25h]
        mov    dword ptr [ebp - 24h], eax
        mov    eax, dword ptr [ebp - 24h]
        mov    ecx, dword ptr [ebp - 20h]
        imul   ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ebp - 18h], ecx
        mov    ecx, dword ptr [esi + 108h]
        add    ecx, eax
        mov    dword ptr [ebp - 1ch], ecx
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp - 18h]
        imul   ecx
        add    esi, 10ch
        mov    dword ptr [ebp - 8], esi
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 0ch], ecx
        mov    dword ptr [ebp - 10h], 10h
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        cdq
        idiv   dword ptr [ebp - 10h]
        mov    ebx, dword ptr [ebp - 0ch]
        mov    dword ptr [ebx], eax
        mov    ebx, dword ptr [ebp - 8]
        mov    dword ptr [ebx], edx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_105C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        test   byte ptr [edi + 0ch], 1
        jne    SHORT loc_1_1084
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0F40
        add    esp, 8
        add    edi, 4
        push   edi
        push   esi
        call   sub_1_0F40
        add    esp, 8
loc_1_1084:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  000h, 000h, 000h
loc_1_1090:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        mov    ecx, dword ptr [ebp + 0ch]
        test   byte ptr [ecx + 0ch], 1
        jne    SHORT loc_1_1119
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi], 0
        jne    SHORT loc_1_1119
        add    esi, 4
        mov    dword ptr [ebp - 14h], esi
        mov    dword ptr [ebp - 18h], ecx
        mov    dword ptr [ebp - 1ch], 2
        mov    edx, dword ptr [ecx]
        mov    eax, edx
        and    eax, 8000h
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 2
        dec    eax
        add    eax, edx
        mov    dword ptr [ebp - 20h], eax
        mov    eax, dword ptr [ebp - 20h]
        cdq
        idiv   dword ptr [ebp - 1ch]
        mov    ebx, dword ptr [ebp - 18h]
        mov    dword ptr [ebx], eax
        mov    ebx, dword ptr [ebp - 14h]
        mov    dword ptr [ebx], edx
        mov    dword ptr [ebp - 4], esi
        add    ecx, 4
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebp - 0ch], 2
        mov    ecx, dword ptr [ecx]
        mov    eax, ecx
        and    eax, 8000h
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 2
        dec    eax
        add    eax, ecx
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        cdq
        idiv   dword ptr [ebp - 0ch]
        mov    ebx, dword ptr [ebp - 8]
        mov    dword ptr [ebx], eax
        mov    ebx, dword ptr [ebp - 4]
        mov    dword ptr [ebx], edx
loc_1_1119:
        mov    eax, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_1124:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 8]
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [eax], ecx
        mov    eax, 1
        ret
        db  000h, 000h, 000h
sub_1_113C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        sub    esi, esi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
loc_1_1149:
        mov    edx, eax
        and    dl, 1
        db  002h, 096h, 000h, 000h, 000h, 000h
        db  088h, 096h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_1169
        mov    ebx, ecx
        and    bl, 1
        sub    dl, bl
        db  088h, 096h, 000h, 000h, 000h, 000h
loc_1_1169:
        shr    eax, 1
        shr    ecx, 1
        inc    esi
        cmp    esi, 3
        jb     SHORT loc_1_1149
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_1177:
        sub    eax, eax
        mov    ecx, 2
        sub    edx, edx
loc_1_1180:
        add    eax, eax
        db  038h, 091h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_118D
        or     eax, 1
loc_1_118D:
        dec    ecx
        jns    SHORT loc_1_1180
        ret
loc_1_1191:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [edi + 8]
        and    edx, 7
        mov    dword ptr [edi + 8], edx
        mov    ecx, dword ptr [esi]
        mov    eax, edx
        xor    eax, ecx
        je     SHORT loc_1_11C0
        and    ecx, eax
        push   ecx
        and    edx, eax
        push   edx
        call   sub_1_113C
        add    esp, 8
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [esi], eax
loc_1_11C0:
        call   sub_1_1177
        mov    dword ptr [edi + 8], eax
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  000h, 000h, 000h
sub_1_11D4:
        db  0A1h, 0F8h, 015h, 000h, 000h
        ret
loc_1_11DA:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 4]
        db  089h, 015h, 0F8h, 015h, 000h, 000h
        mov    edx, dword ptr [edx]
        mov    eax, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [eax]
        mov    edi, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 8]
        and    eax, 7
        db  08Ah, 080h, 0F0h, 015h, 000h, 000h
        call   sub_1_007C
        pop    edi
        pop    esi
        leave
        ret
        db  000h, 000h
loc_1_120C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], edx
        mov    eax, dword ptr [ebp - 4]
        push   0
        push   0
        push   dword ptr [eax + 8]
        push   eax
        call   sub_1_0801
        mov    esp, ebp
        pop    ebp
        ret
sub_1_1229:
        push   ebp
        mov    ebp, esp
        push   esi
        push   dword ptr [ebp + 10h]
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_08C8
        add    esp, 8
        push   esi
        push   dword ptr [ebp + 0ch]
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1310
        mov    eax, 1
        pop    esi
        pop    ebp
        ret
        db  000h, 000h
sub_1_1254:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    ecx, 0ch
        je     SHORT loc_1_1263
        or     ecx, ecx
        jne    SHORT loc_1_126B
loc_1_1263:
        mov    eax, dword ptr [ebp + 8]
        db  0A3h, 000h, 000h, 000h, 000h
loc_1_126B:
        cmp    ecx, 0dh
        je     SHORT loc_1_1274
        or     ecx, ecx
        jne    SHORT loc_1_127C
loc_1_1274:
        mov    eax, dword ptr [ebp + 8]
        db  0A3h, 004h, 000h, 000h, 000h
loc_1_127C:
        mov    eax, 1
        pop    ebp
        ret
        db  0A1h, 000h, 000h, 000h, 000h, 00Bh, 0C0h, 075h, 00Ah, 0E8h, 073h, 000h, 000h, 000h, 0A3h, 000h
        db  000h, 000h, 000h, 0C3h
sub_1_1297:
        db  0A1h, 004h, 000h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_1_12AA
        call   sub_1_1304
        db  0A3h, 004h, 000h, 000h, 000h
loc_1_12AA:
        ret
        db  000h
sub_1_12AC:
        db  0CDh, 020h, 00Eh, 080h, 017h, 000h
        add    byte ptr [eax], al
sub_1_12B4:
        db  0CDh, 020h, 018h, 080h, 017h, 000h
        add    byte ptr [eax], al
sub_1_12BC:
        db  0CDh, 020h, 011h, 080h, 017h, 000h
        add    byte ptr [eax], al
loc_1_12C4:
        db  0CDh, 020h, 012h, 080h, 017h, 000h
        add    byte ptr [eax], al
sub_1_12CC:
        db  0CDh, 020h, 048h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_1_12D4:
        db  0CDh, 020h, 049h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_1_12DC:
        db  0CDh, 020h, 04Ah, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_1_12E4:
        db  0CDh, 020h, 051h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_1_12EC:
        db  0CDh, 020h, 052h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_1_12F4:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_1_12FC:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_1_1304:
        push   ebx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    eax, ebx
        pop    ebx
        ret
        db  000h
sub_1_1310:
        push   esi
        mov    edx, dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        ret    0ch
        db  000h, 000h, 000h
sub_1_132C:
        db  0CDh, 020h, 0CFh, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_1_1334:
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, ebx
        pop    ebx
        ret
        db  000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 08Ah, 031h, 001h, 000h, 000h, 000h, 04Ch, 04Dh, 04Fh, 055h
        db  053h, 045h, 020h, 020h, 000h, 000h, 000h, 03Ah, 000h, 000h, 000h, 000h, 006h, 004h, 000h, 000h
        db  006h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h
loc_1_1394:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1398:
        mov    eax, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 0C0h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 0C8h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 0D0h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 0D8h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 0E0h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [ecx]
        add    eax, dword ptr [eax]
        add    al, byte ptr [esi + eax]
        add    dword ptr [ebx], eax
        add    eax, 4020007h
        push   es
        add    dword ptr [ebx], eax
        add    eax, 7
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx], al
        add    al, 3
        add    eax, dword ptr [esp + eax]
        add    al, 0
loc_1_1404:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_140C:
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1414:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_1418:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_141C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_1420:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1448:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], 0
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
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        adc    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        adc    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1508:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        adc    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1528:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        adc    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1584:
        add    dword ptr [eax], eax
        add    dword ptr [eax], eax
        add    al, byte ptr [eax]
        add    al, byte ptr [eax]
        or     byte ptr [edx], al
        or     byte ptr [eax], al
        or     byte ptr [edx], al
        or     byte ptr [edx], al
        add    al, 1
        add    al, 0
        add    al, 1
        add    al, 1
loc_1_159C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    edx
        db  075h, 074h
        db  074h, 06Fh
        outsb
        db  073h, 020h
        outsd
        db  077h, 06Eh
        das
        db  075h, 070h
        and    byte ptr [ebp + 6ch], ah
        db  074h, 061h
        cmp    ah, byte ptr [eax]
        and    eax, 25202c78h
        db  078h, 00Ah
        add    byte ptr [eax], al
        db  072h, 067h
        bound  eax, qword ptr [edx + 6ch]
        outsb
        inc    edx
        db  075h, 074h
        db  074h, 06Fh
        outsb
        pop    ebx
        and    eax, 203a5d64h
        and    eax, 52000a64h
        db  065h, 073h, 075h
        insb
        db  074h, 069h
        outsb
        and    byte ptr [bp + si + 75h], ah
        db  074h, 074h
        outsd
        outsb
        db  073h, 03Ah
        and    byte ptr [0a78h], ah
loc_1_15F0:
        add    byte ptr [eax], ah
        adc    byte ptr [eax], dh
        or     byte ptr [eax], ch
        sbb    byte ptr [eax], bh
loc_1_15F8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_15FC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1600:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end
