; octk32_lcode_01.asm -- OCTK32 object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

loc_1_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 4ffh]
        mov    eax, edi
        or     dword ptr [esi + 0b7h], 8000h
        push   eax
        call   sub_1_0984
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [esi + 4ceh]
        push   eax
        call   sub_1_0888
        test   byte ptr [esi + 0b7h], 10h
        je     SHORT loc_1_0059
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [esi + 515h]
        push   eax
        call   sub_1_089A
        push   9c4h
        call   sub_1_08A0
loc_1_0059:
        lea    eax, [esi + 515h]
        push   eax
        call   sub_1_0894
        mov    eax, dword ptr [esi + 545h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0071:
        push   1
        push   1
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_007B:
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        cmp    dword ptr [esi + 3eh], 7
        jne    SHORT loc_1_00A4
        mov    eax, 1
        mov    cl, byte ptr [esi + 2fh]
        shl    eax, cl
        not    eax
        db  021h, 005h, 01Dh, 000h, 000h, 000h
        jmp    SHORT loc_1_00CD
loc_1_00A4:
        mov    ecx, dword ptr [esi + 4f7h]
        cmp    ecx, 1fh
        mov    eax, 1
        ja     SHORT loc_1_00C0
        shl    eax, cl
        not    eax
        db  021h, 005h, 010h, 000h, 000h, 000h
        jmp    SHORT loc_1_00CD
loc_1_00C0:
        sub    cl, 20h
        shl    eax, cl
        not    eax
        db  021h, 005h, 014h, 000h, 000h, 000h
loc_1_00CD:
        push   edi
        call   sub_1_083E
        push   edi
        call   sub_1_07D8
        mov    dx, 21h
        mov    ecx, dword ptr [esi + 20h]
        cmp    ecx, 7
        jbe    SHORT loc_1_00ED
        mov    dx, 0a1h
        sub    cx, 8
loc_1_00ED:
        in     al, dx
        mov    bx, 1
        shl    bx, cl
        and    ax, 0ffh
        or     al, bl
        out    dx, al
        push   0
        push   683h
        push   esi
        call   sub_1_08AC
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0120:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 1]
        mov    esi, dword ptr [ebp + 8]
        push   eax
        lea    eax, [esi + 4ceh]
        push   eax
        call   sub_1_0888
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        add    esi, 515h
        push   esi
        call   sub_1_089A
        push   9c4h
        call   sub_1_08A0
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_0160:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_07CC
        push   0
        mov    eax, dword ptr [esi + 0ch]
        push   eax
        call   sub_1_07C0
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0190:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    cx, word ptr [eax + 10h]
        lea    eax, [ecx + 0ch]
        mov    dx, ax
        sub    al, al
        out    dx, al
        lea    eax, [ecx + 0dh]
        mov    dx, ax
        sub    al, al
        out    dx, al
        lea    edx, [ecx + 4]
        mov    al, 20h
        out    dx, al
        lea    edx, [ecx + 5]
        sub    al, al
        out    dx, al
        mov    esi, dword ptr [ebp + 8]
        add    esi, 4ah
        mov    edi, esi
        mov    dword ptr [ebp - 0ch], 3
loc_1_01D0:
        mov    ax, word ptr [edi]
        mov    word ptr [ebp - 2], ax
        add    edi, 2
        shl    al, 8
        or     al, byte ptr [ebp - 1]
        lea    edx, [ecx + 2]
        out    dx, al
        lea    edx, [ecx + 3]
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        dec    dword ptr [ebp - 0ch]
        jne    SHORT loc_1_01D0
        mov    eax, dword ptr [ebp + 8]
        lea    edx, [ecx + 2]
        mov    ax, word ptr [eax + 57h]
        mov    word ptr [ebp - 10h], ax
        shl    al, 8
        or     al, byte ptr [ebp - 0fh]
        mov    byte ptr [ebp - 14h], al
        out    dx, al
        lea    edx, [ecx + 3]
        mov    al, byte ptr [ebp - 10h]
        out    dx, al
        mov    dword ptr [ebp - 8], 3
loc_1_0216:
        mov    bx, word ptr [esi]
        not    bx
        add    esi, 2
        mov    ax, bx
        shl    al, 8
        or     al, bh
        lea    edx, [ecx + 2]
        out    dx, al
        lea    edx, [ecx + 3]
        mov    al, bl
        out    dx, al
        dec    dword ptr [ebp - 8]
        jne    SHORT loc_1_0216
        mov    al, byte ptr [ebp - 14h]
        not    al
        lea    edx, [ecx + 2]
        out    dx, al
        mov    al, byte ptr [ebp - 10h]
        not    al
        lea    edx, [ecx + 3]
        out    dx, al
        lea    eax, [ecx + 0ch]
        mov    dx, ax
        mov    al, 1
        out    dx, al
        lea    eax, [ecx + 0dh]
        mov    dx, ax
        sub    al, al
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0270:
        db  0A0h, 038h, 00Ah, 000h, 000h
        cmp    al, 0
        jne    SHORT loc_1_0295
        db  0FEh, 005h, 038h, 00Ah, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 028h, 000h
        jb     SHORT loc_1_0297
        push   0
        push   0
        db  0E8h, 050h, 008h, 0FFh, 0FFh
        cmp    eax, 0
        jne    SHORT loc_1_0297
loc_1_0295:
        clc
        ret
loc_1_0297:
        stc
        ret
OCTK32_Control:
        sub    eax, 1
        cmp    eax, 1bh
        jae    SHORT loc_1_02A8
        db  0FFh, 024h, 085h, 0AAh, 002h, 000h, 000h
loc_1_02A8:
        clc
        ret
        db  070h, 002h, 000h, 000h, 0A8h, 002h, 000h, 000h, 0A8h, 002h, 000h, 000h, 0A8h, 002h, 000h, 000h
        db  0A8h, 002h, 000h, 000h, 0A8h, 002h, 000h, 000h, 0A8h, 002h, 000h, 000h, 0A8h, 002h, 000h, 000h
        db  0A8h, 002h, 000h, 000h, 0A8h, 002h, 000h, 000h, 0A8h, 002h, 000h, 000h, 0A8h, 002h, 000h, 000h
        db  0A8h, 002h, 000h, 000h, 0A8h, 002h, 000h, 000h, 0A8h, 002h, 000h, 000h, 0A8h, 002h, 000h, 000h
        db  0A8h, 002h, 000h, 000h, 0A8h, 002h, 000h, 000h, 0A8h, 002h, 000h, 000h, 0A8h, 002h, 000h, 000h
        db  0A8h, 002h, 000h, 000h, 0A8h, 002h, 000h, 000h, 0A8h, 002h, 000h, 000h, 0A8h, 002h, 000h, 000h
        db  0A8h, 002h, 000h, 000h, 0A8h, 002h, 000h, 000h, 070h, 002h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h
sub_1_0320:
        push   ebp
        mov    ebp, esp
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
        call   sub_1_0350
        pop    ebp
        ret    1ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0350:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 10h]
        add    ecx, 2
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 8
        cmp    dword ptr [ebp + 14h], 0eh
        jb     NEAR PTR loc_1_0470
        mov    al, byte ptr [eax]
        shr    al, 7
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ecx]
        and    al, 80h
        mov    byte ptr [ebp - 2], al
        or     al, al
        je     NEAR PTR loc_1_0414
        mov    al, byte ptr [ecx]
        cmp    al, 0ffh
        jne    SHORT loc_1_0392
        cmp    byte ptr [ecx + 1], 0ffh
        je     SHORT loc_1_039C
loc_1_0392:
        cmp    al, 0c0h
        jne    SHORT loc_1_03B8
        cmp    byte ptr [ecx + 1], 0
        jne    SHORT loc_1_03B8
loc_1_039C:
        cmp    byte ptr [ecx + 2], 0ffh
        jne    SHORT loc_1_03B8
        cmp    byte ptr [ecx + 3], 0ffh
        jne    SHORT loc_1_03B8
        cmp    byte ptr [ecx + 4], 0ffh
        jne    SHORT loc_1_03B8
        cmp    byte ptr [ecx + 5], 0ffh
        mov    byte ptr [ebp - 2], 1
        je     SHORT loc_1_03BC
loc_1_03B8:
        mov    byte ptr [ebp - 2], 0
loc_1_03BC:
        cmp    byte ptr [ebp - 2], 0
        je     SHORT loc_1_03D0
        mov    dword ptr [ebp - 8], 8
        jmp    NEAR PTR loc_1_047B
        db  08Bh, 0FFh
loc_1_03D0:
        mov    al, byte ptr [ecx]
        and    al, byte ptr [ecx + 2]
        and    al, 80h
        mov    byte ptr [ebp - 2], al
        or     al, al
        mov    dword ptr [ebp - 8], 1000h
        jne    SHORT loc_1_03EC
        mov    dword ptr [ebp - 8], 4000h
loc_1_03EC:
        xor    edx, edx
        mov    dl, byte ptr [ecx + 4]
        xor    eax, eax
        mov    al, byte ptr [ecx + 2]
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        xor    eax, eax
        mov    al, byte ptr [ecx + 3]
        shl    eax, 10h
        or     edx, eax
        xor    eax, eax
        mov    al, byte ptr [ecx + 5]
        or     edx, eax
        mov    dword ptr [ebp - 0ch], edx
        jmp    SHORT loc_1_047B
loc_1_0414:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 10h], 30h
        je     SHORT loc_1_0460
        mov    edx, dword ptr [esi + 1ah]
        lea    eax, [esi + 18h]
        cmp    dword ptr [ecx + 2], edx
        jne    SHORT loc_1_0437
        mov    ax, word ptr [eax]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 10h], 0
        je     SHORT loc_1_043E
loc_1_0437:
        mov    dword ptr [ebp - 10h], 1
loc_1_043E:
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_0450
        mov    dword ptr [ebp - 8], 1
        jmp    SHORT loc_1_047E
        db  08Dh, 049h, 000h
loc_1_0450:
        mov    dword ptr [ebp - 8], 0
        jmp    SHORT loc_1_047E
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0460:
        mov    dword ptr [ebp - 8], 1
        jmp    SHORT loc_1_047E
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0470:
        mov    dword ptr [ebp - 8], 20h
        mov    byte ptr [ebp - 1], 0
loc_1_047B:
        mov    esi, dword ptr [ebp + 8]
loc_1_047E:
        mov    edi, dword ptr [esi + 14h]
        or     edi, edi
        je     NEAR PTR loc_1_0561
loc_1_0489:
        mov    ecx, dword ptr [edi + 8]
        mov    eax, ecx
        and    eax, dword ptr [ebp - 8]
        test   al, 9
        jne    SHORT loc_1_04CD
        test   ah, 40h
        je     SHORT loc_1_04A2
        mov    eax, dword ptr [edi + 0ch]
        test   dword ptr [ebp - 0ch], eax
        jne    SHORT loc_1_04CD
loc_1_04A2:
        test   byte ptr [ebp - 7], 40h
        je     SHORT loc_1_04AD
        test   ch, 20h
        jne    SHORT loc_1_04CD
loc_1_04AD:
        test   byte ptr [ebp - 7], 10h
        je     SHORT loc_1_04B9
        cmp    byte ptr [edi + 1dh], 0
        jne    SHORT loc_1_04CD
loc_1_04B9:
        test   cl, 10h
        je     SHORT loc_1_04C4
        cmp    byte ptr [ebp - 1], 0
        jne    SHORT loc_1_04CD
loc_1_04C4:
        test   cl, 20h
        je     NEAR PTR loc_1_0556
loc_1_04CD:
        inc    dword ptr [edi + 18h]
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
        mov    eax, dword ptr [edi + 4]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        call   sub_1_0802
        mov    byte ptr [edi + 1eh], 1
        mov    eax, dword ptr [edi + 18h]
        dec    eax
        mov    dword ptr [edi + 18h], eax
        jne    SHORT loc_1_0556
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_050F
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_050F:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_0520
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_0526
        db  08Bh, 0FFh
loc_1_0520:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 14h], eax
loc_1_0526:
        cmp    byte ptr [edi + 1eh], 0
        je     SHORT loc_1_0535
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_0808
loc_1_0535:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 2ah]
        mov    eax, 1
        mov    cl, byte ptr [edi + 1ch]
        shl    eax, cl
        or     dword ptr [esi + 2eh], eax
        push   0
        push   1fh
        push   edi
        call   sub_1_08AC
        mov    edi, ebx
        jmp    SHORT loc_1_0559
loc_1_0556:
        mov    edi, dword ptr [edi + 10h]
loc_1_0559:
        or     edi, edi
        jne    NEAR PTR loc_1_0489
loc_1_0561:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_0570:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0580
        pop    ebp
        ret    4
sub_1_0580:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 14h]
        or     edi, edi
        je     SHORT loc_1_05FE
loc_1_0590:
        inc    dword ptr [edi + 18h]
        cmp    byte ptr [edi + 1eh], 0
        je     SHORT loc_1_05A6
        mov    byte ptr [edi + 1eh], 0
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_0808
loc_1_05A6:
        mov    eax, dword ptr [edi + 18h]
        dec    eax
        mov    dword ptr [edi + 18h], eax
        jne    SHORT loc_1_05F7
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_05BC
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_05BC:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_05D0
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_05D6
        db  005h, 000h, 000h, 000h, 000h
loc_1_05D0:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 14h], eax
loc_1_05D6:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 2ah]
        mov    eax, 1
        mov    cl, byte ptr [edi + 1ch]
        shl    eax, cl
        or     dword ptr [esi + 2eh], eax
        push   0
        push   1fh
        push   edi
        call   sub_1_08AC
        mov    edi, ebx
        jmp    SHORT loc_1_05FA
loc_1_05F7:
        mov    edi, dword ptr [edi + 10h]
loc_1_05FA:
        or     edi, edi
        jne    SHORT loc_1_0590
loc_1_05FE:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    edx, dword ptr [ebp + 10h]
        xor    ecx, ecx
        mov    cl, byte ptr [edx + 4]
        xor    eax, eax
        mov    al, byte ptr [edx + 2]
        shl    eax, 10h
        or     ecx, eax
        shl    ecx, 8
        xor    eax, eax
        mov    al, byte ptr [edx + 3]
        shl    eax, 10h
        or     ecx, eax
        xor    eax, eax
        mov    al, byte ptr [edx + 5]
        or     ecx, eax
        mov    al, byte ptr [edx]
        and    al, byte ptr [edx + 2]
        and    al, 80h
        mov    byte ptr [ebp - 1], al
        or     al, al
        mov    eax, dword ptr [ebp + 8]
        je     SHORT loc_1_0662
        cmp    dword ptr [eax + 8], ecx
        jne    SHORT loc_1_0662
        cmp    dword ptr [eax + 0ch], 0
        je     SHORT loc_1_0662
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_0662:
        mov    esi, dword ptr [eax + 10h]
        or     esi, esi
        jne    SHORT loc_1_0672
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_0672:
        test   esi, 20h
        je     SHORT loc_1_0683
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_0683:
        mov    al, byte ptr [edx]
        shr    al, 7
        or     al, al
        je     SHORT loc_1_06A0
        test   esi, 10h
        je     SHORT loc_1_06A0
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h
loc_1_06A0:
        mov    eax, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [eax]
        and    cl, 80h
        mov    byte ptr [ebp - 1], cl
        or     cl, cl
        je     SHORT loc_1_072A
        mov    cl, byte ptr [eax]
        cmp    cl, 0ffh
        jne    SHORT loc_1_06BC
        cmp    byte ptr [eax + 1], 0ffh
        je     SHORT loc_1_06C7
loc_1_06BC:
        cmp    cl, 0c0h
        jne    SHORT loc_1_06E3
        cmp    byte ptr [eax + 1], 0
        jne    SHORT loc_1_06E3
loc_1_06C7:
        cmp    byte ptr [eax + 2], 0ffh
        jne    SHORT loc_1_06E3
        cmp    byte ptr [eax + 3], 0ffh
        jne    SHORT loc_1_06E3
        cmp    byte ptr [eax + 4], 0ffh
        jne    SHORT loc_1_06E3
        cmp    byte ptr [eax + 5], 0ffh
        mov    byte ptr [ebp - 1], 1
        je     SHORT loc_1_06E7
loc_1_06E3:
        mov    byte ptr [ebp - 1], 0
loc_1_06E7:
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_0710
        test   esi, 8
        je     SHORT loc_1_0700
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
loc_1_0700:
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0710:
        test   esi, 6000h
        je     SHORT loc_1_0721
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_0721:
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_072A:
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0740:
        push   esi
        mov    ecx, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 8]
        cld
        rep outsb
        pop    esi
        ret    0ch
        db  056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 066h
        db  06Fh, 05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh, 054h, 024h, 008h, 08Bh, 074h, 024h, 00Ch, 08Bh, 04Ch
        db  024h, 010h, 0FCh, 0F3h, 06Fh, 05Eh, 0C2h, 00Ch, 000h
sub_1_077D:
        push   edi
        mov    ecx, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 8]
        cld
        rep insb
        pop    edi
        ret    0ch
        db  057h, 08Bh, 04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 066h
        db  06Dh, 05Fh, 0C2h, 00Ch, 000h, 057h, 08Bh, 054h, 024h, 008h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 04Ch
        db  024h, 010h, 0FCh, 0F3h, 06Dh, 05Fh, 0C2h, 00Ch, 000h
sub_1_07BA:
        db  0CDh, 020h, 031h, 080h, 028h, 000h
sub_1_07C0:
        db  0CDh, 020h, 032h, 080h, 028h, 000h
sub_1_07C6:
        db  0CDh, 020h, 033h, 080h, 028h, 000h
sub_1_07CC:
        db  0CDh, 020h, 034h, 080h, 028h, 000h
sub_1_07D2:
        db  0CDh, 020h, 035h, 080h, 028h, 000h
sub_1_07D8:
        db  0CDh, 020h, 036h, 080h, 028h, 000h
sub_1_07DE:
        db  0CDh, 020h, 037h, 080h, 028h, 000h
sub_1_07E4:
        db  0CDh, 020h, 038h, 080h, 028h, 000h
sub_1_07EA:
        db  0CDh, 020h, 039h, 080h, 028h, 000h
        db  0CDh, 020h, 03Ah, 080h, 028h, 000h
sub_1_07F6:
        db  0CDh, 020h, 03Bh, 080h, 028h, 000h
sub_1_07FC:
        db  0CDh, 020h, 03Ch, 080h, 028h, 000h
sub_1_0802:
        db  0CDh, 020h, 03Dh, 080h, 028h, 000h
sub_1_0808:
        db  0CDh, 020h, 03Eh, 080h, 028h, 000h
sub_1_080E:
        db  0CDh, 020h, 03Fh, 080h, 028h, 000h
sub_1_0814:
        db  0CDh, 020h, 040h, 080h, 028h, 000h
sub_1_081A:
        db  0CDh, 020h, 041h, 080h, 028h, 000h
        db  0CDh, 020h, 009h, 080h, 028h, 000h
        db  0CDh, 020h, 008h, 080h, 028h, 000h
sub_1_082C:
        db  0CDh, 020h, 044h, 080h, 028h, 000h
        db  0CDh, 020h, 048h, 080h, 028h, 000h
        db  0CDh, 020h, 049h, 080h, 028h, 000h
sub_1_083E:
        db  0CDh, 020h, 04Fh, 080h, 028h, 000h
        db  0CDh, 020h, 057h, 080h, 028h, 000h
        db  0CDh, 020h, 058h, 080h, 028h, 000h
        db  0CDh, 020h, 059h, 080h, 028h, 000h
        db  0CDh, 020h, 05Ah, 080h, 028h, 000h
        db  0CDh, 020h, 05Bh, 080h, 028h, 000h
        db  0CDh, 020h, 05Ch, 080h, 028h, 000h
        db  0CDh, 020h, 05Dh, 080h, 028h, 000h
        db  0CDh, 020h, 05Eh, 080h, 028h, 000h
        db  0CDh, 020h, 056h, 080h, 028h, 000h
        add    byte ptr [eax], al
        db  0CDh, 020h, 000h, 080h, 028h, 000h
sub_1_0882:
        db  0CDh, 020h, 00Ch, 080h, 028h, 000h
sub_1_0888:
        db  0CDh, 020h, 00Dh, 080h, 028h, 000h
sub_1_088E:
        db  0CDh, 020h, 00Fh, 080h, 028h, 000h
sub_1_0894:
        db  0CDh, 020h, 010h, 080h, 028h, 000h
sub_1_089A:
        db  0CDh, 020h, 011h, 080h, 028h, 000h
sub_1_08A0:
        db  0CDh, 020h, 00Eh, 080h, 028h, 000h
sub_1_08A6:
        db  0CDh, 020h, 00Ah, 080h, 028h, 000h
sub_1_08AC:
        db  0CDh, 020h, 00Bh, 080h, 028h, 000h
sub_1_08B2:
        db  0CDh, 020h, 016h, 080h, 028h, 000h
sub_1_08B8:
        db  0CDh, 020h, 017h, 080h, 028h, 000h
sub_1_08BE:
        db  0CDh, 020h, 018h, 080h, 028h, 000h
        db  0CDh, 020h, 019h, 080h, 028h, 000h
sub_1_08CA:
        db  0CDh, 020h, 01Ah, 080h, 028h, 000h
sub_1_08D0:
        db  0CDh, 020h, 01Bh, 080h, 028h, 000h
sub_1_08D6:
        db  0CDh, 020h, 01Ch, 080h, 028h, 000h
sub_1_08DC:
        db  0CDh, 020h, 01Dh, 080h, 028h, 000h
sub_1_08E2:
        db  0CDh, 020h, 01Eh, 080h, 028h, 000h
        db  0CDh, 020h, 01Fh, 080h, 028h, 000h
sub_1_08EE:
        db  0CDh, 020h, 020h, 080h, 028h, 000h
sub_1_08F4:
        db  0CDh, 020h, 021h, 080h, 028h, 000h
        db  0CDh, 020h, 022h, 080h, 028h, 000h
sub_1_0900:
        db  0CDh, 020h, 023h, 080h, 028h, 000h
        db  0CDh, 020h, 024h, 080h, 028h, 000h
sub_1_090C:
        db  0CDh, 020h, 025h, 080h, 028h, 000h
        db  0CDh, 020h, 026h, 080h, 028h, 000h
sub_1_0918:
        db  0CDh, 020h, 027h, 080h, 028h, 000h
        db  0CDh, 020h, 028h, 080h, 028h, 000h
sub_1_0924:
        db  0CDh, 020h, 005h, 080h, 028h, 000h
sub_1_092A:
        db  0CDh, 020h, 006h, 080h, 028h, 000h
sub_1_0930:
        db  0CDh, 020h, 007h, 080h, 028h, 000h
        db  0CDh, 020h, 042h, 080h, 028h, 000h
        db  0CDh, 020h, 012h, 080h, 028h, 000h
        db  0CDh, 020h, 013h, 080h, 028h, 000h
        db  0CDh, 020h, 014h, 080h, 028h, 000h
        db  0CDh, 020h, 015h, 080h, 028h, 000h
        db  0CDh, 020h, 04Ah, 080h, 028h, 000h
        db  0CDh, 020h, 04Bh, 080h, 028h, 000h
        db  0CDh, 020h, 04Ch, 080h, 028h, 000h
        db  0CDh, 020h, 04Dh, 080h, 028h, 000h
        db  0CDh, 020h, 04Eh, 080h, 028h, 000h
sub_1_0972:
        db  0CDh, 020h, 046h, 080h, 028h, 000h
sub_1_0978:
        db  0CDh, 020h, 045h, 080h, 028h, 000h
sub_1_097E:
        db  0CDh, 020h, 043h, 080h, 028h, 000h
sub_1_0984:
        db  0CDh, 020h, 047h, 080h, 028h, 000h
        add    byte ptr [eax], al
sub_1_098C:
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
        db  056h, 057h, 09Ch, 08Bh, 04Ch, 024h, 018h, 08Bh, 074h, 024h, 014h, 08Bh, 07Ch, 024h, 010h, 03Bh
        db  0F7h, 073h, 00Dh, 00Bh, 0C9h, 074h, 01Fh, 0FDh, 049h, 003h, 0F1h, 003h, 0F9h, 041h, 0EBh, 014h
        db  08Bh, 0C6h, 02Bh, 0C7h, 083h, 0F8h, 004h, 072h, 00Bh, 08Ah, 0C1h, 024h, 003h, 0C1h, 0E9h, 002h
        db  0F3h, 0A5h, 08Ah, 0C8h, 0F3h, 0A4h, 09Dh, 05Fh, 05Eh, 0C2h, 00Ch, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 00Ah, 003h, 000h, 000h, 003h, 00Ah, 000h, 000h, 04Fh, 043h, 054h, 04Bh
        db  033h, 032h, 020h, 020h, 000h, 040h, 000h, 054h, 099h, 002h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 028h, 023h, 029h, 06Fh, 06Ch, 069h, 074h
        db  06Fh, 06Bh, 02Eh, 063h, 020h, 024h, 052h, 065h, 076h, 069h, 073h, 069h, 06Fh, 06Eh, 03Ah, 020h
        db  020h, 020h, 031h, 02Eh, 031h, 038h, 020h, 020h, 024h, 000h, 000h, 000h, 040h, 028h, 023h, 029h
        db  040h, 021h, 023h, 021h, 020h, 04Fh, 06Ch, 069h, 063h, 06Fh, 06Dh, 020h, 054h, 06Fh, 06Bh, 065h
        db  06Eh, 02Dh, 052h, 069h, 06Eh, 067h, 020h, 044h, 072h, 069h, 076h, 065h, 072h, 020h, 066h, 06Fh
        db  072h, 020h, 043h, 068h, 069h, 063h, 061h, 067h, 06Fh, 02Ch, 020h, 056h, 065h, 072h, 073h, 069h
        db  06Fh, 06Eh, 020h, 032h, 02Eh, 030h, 033h, 000h, 028h, 043h, 029h, 020h, 043h, 06Fh, 070h, 079h
        db  072h, 069h, 067h, 068h, 074h, 020h, 04Fh, 06Ch, 069h, 063h, 06Fh, 06Dh, 020h, 041h, 02Fh, 053h
        db  020h, 031h, 039h, 039h, 033h, 02Dh, 031h, 039h, 039h, 035h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        db  041h, 064h, 061h, 070h, 074h, 065h, 072h, 000h
loc_1_0AD0:
        inc    ecx
        popad
        db  070h, 074h
        jb     NEAR PTR loc_1_0AD8
loc_1_0AD8:
        dec    ebp
        popad
        db  078h, 046h
        db  072h, 061h
        insd
        push   ebx
        imul   edi, dword ptr [edx + 65h], 0
loc_1_0AE8:
        dec    ebp
        popad
        db  078h, 046h
        db  072h, 061h
        insd
        push   ebx
        imul   edi, dword ptr [edx + 65h], 0
        inc    edx
        jne    SHORT loc_1_0B6E
        dec    esi
        jne    SHORT loc_1_0B6B
        bound  esp, qword ptr [ebp + 72h]
        add    byte ptr [eax], al
        add    byte ptr [edx + 75h], al
        jae    SHORT loc_1_0B56
        jne    SHORT loc_1_0B77
        bound  esp, qword ptr [ebp + 72h]
        add    byte ptr [eax], al
        add    byte ptr [edx + 75h], al
        db  073h, 054h
        jns    SHORT loc_1_0B86
        add    byte ptr gs:[edx + 75h], al
        jae    SHORT loc_1_0B70
        jns    SHORT loc_1_0B8E
        add    byte ptr gs:[edx + esi*2 + 61h], dl
        outsb
        db  073h, 06Dh
        imul   esi, dword ptr [edx + eax*2 + 75h], 72656666h
        push   ebx
        imul   edi, dword ptr [edx + 65h], 72540000h
        popad
        outsb
        db  073h, 06Dh
        imul   esi, dword ptr [edx + eax*2 + 75h], 72656666h
        push   ebx
        imul   edi, dword ptr [edx + 65h], 72540000h
        popad
        outsb
        db  073h, 06Dh
        imul   esi, dword ptr [edx + eax*2 + 75h], 72656666h
loc_1_0B56:
        inc    ebx
        outsd
        db  075h, 06Eh
        je     SHORT loc_1_0B5C
loc_1_0B5C:
        push   esp
        jb     SHORT loc_1_0BC0
        outsb
        db  073h, 06Dh
        imul   esi, dword ptr [edx + eax*2 + 75h], 72656666h
        inc    ebx
loc_1_0B6B:
        outsd
        jne    SHORT loc_1_0BDC
loc_1_0B6E:
        je     SHORT loc_1_0B70
loc_1_0B70:
        push   edx
        arpl   word ptr gs:[edx + 75h], ax
        inc    bx
loc_1_0B77:
        outsd
        db  075h, 06Eh
        je     SHORT loc_1_0B7C
loc_1_0B7C:
        push   edx
        arpl   word ptr gs:[edx + 75h], ax
        inc    bx
        outsd
        jne    SHORT loc_1_0BF4
loc_1_0B86:
        je     SHORT loc_1_0B88
loc_1_0B88:
        inc    esi
        db  072h, 061h
        insd
        push   ebx
loc_1_0B8E:
        je     SHORT loc_1_0BF1
        je     SHORT loc_1_0C07
        jae    SHORT loc_1_0B94
loc_1_0B94:
        inc    esi
        jb     SHORT loc_1_0BF8
        insd
        push   ebx
        db  074h, 061h
        db  074h, 075h
        jae    SHORT loc_1_0BA0
loc_1_0BA0:
        inc    ebp
        popad
        jb     SHORT loc_1_0C10
        jns    SHORT loc_1_0BF8
        insb
        popad
        db  073h, 065h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BB0:
        inc    ebp
        popad
        db  072h, 06Ch
        db  079h, 052h
        insb
        popad
        jae    SHORT loc_1_0C21
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BC0:
        dec    edi
        arpl   word ptr [ebx + ebp*2 + 33h], si
        xor    al, byte ptr [eax]
        add    byte ptr [edi], al
        add    byte ptr [edi], al
        db  000h, 080h, 001h, 000h, 000h, 040h
        sub    byte ptr [ebx], ah
        sub    dword ptr [ecx + 64h], esp
        popad
        jo     SHORT loc_1_0C4D
        jb     NEAR PTR loc_1_0C0A
loc_1_0BDC:
        arpl   word ptr [eax], sp
        and    al, 52h
        jbe    NEAR PTR loc_1_0C4C
        db  073h, 069h
        outsd
        outsb
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        xor    dword ptr [esi], ebp
        xor    dword ptr [edi], esi
        and    byte ptr [eax], ah
loc_1_0BF1:
        and    al, 0
        db  000h
loc_1_0BF4:
        or     al, 0
        or     al, 0
loc_1_0BF8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BFC:
        or     al, 0
        or     al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    eax
        sub    byte ptr [ebx], ah
loc_1_0C07:
        sub    dword ptr [ecx + 64h], esp
loc_1_0C0A:
        jo     SHORT loc_1_0C3F
        xor    ch, byte ptr [esi]
        arpl   word ptr [eax], sp
loc_1_0C10:
        and    al, 52h
        db  065h, 076h, 069h
        db  073h, 069h
        outsd
        outsb
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        xor    dword ptr [esi], ebp
        cmp    byte ptr [eax], ah
loc_1_0C21:
        and    byte ptr [eax + eax], ah
        inc    eax
        sub    byte ptr [ebx], ah
        sub    dword ptr [ecx + 64h], esp
        jo     SHORT loc_1_0C95
        outsb
        imul   esi, dword ptr [esi + ebp + 63h], 65522420h
        db  076h, 069h
        db  073h, 069h
        outsd
        outsb
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
loc_1_0C3F:
        xor    dword ptr [esi], ebp
        xor    dword ptr [eax], edi
        and    byte ptr [eax], ah
        and    al, 0
        add    byte ptr [eax + 28h], al
        and    ebp, dword ptr [ecx]
loc_1_0C4C:
        popad
loc_1_0C4D:
        db  064h, 070h, 069h
        outsb
        imul   esi, dword ptr [ebx], 20632e32h
        and    al, 52h
        jbe    NEAR PTR loc_1_0CC5
        jae    SHORT loc_1_0CC7
        outsd
        outsb
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        xor    dword ptr [esi], ebp
        cmp    dword ptr [eax], esp
        and    byte ptr [eax + eax], ah
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [7000000h], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], cl
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
        add    byte ptr [ebx], cl
        add    byte ptr [eax], al
        add    byte ptr [esi], cl
        add    byte ptr [eax], al
        add    byte ptr [edi], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [ebx], al
loc_1_0C95:
        add    byte ptr [eax], al
        add    byte ptr [6000000h], al
        add    byte ptr [eax], al
        add    byte ptr [edi], al
        add    byte ptr [eax], al
        add    bh, bh
        add    byte ptr [eax], al
        add    bh, bh
        add    byte ptr [eax], al
        add    byte ptr [eax + 28h], al
        and    ebp, dword ptr [ecx]
        jb     SHORT loc_1_0D17
        db  071h, 075h
        db  065h, 073h, 074h
        arpl   word ptr cs:[eax], sp
        and    al, 52h
        db  065h, 076h, 069h
        jae    SHORT loc_1_0D2A
        outsd
        outsb
        cmp    ah, byte ptr [eax]
loc_1_0CC5:
        and    byte ptr [eax], ah
loc_1_0CC7:
        xor    dword ptr [esi], ebp
        xor    dword ptr [eax], esi
        and    byte ptr [eax + eax], ah
        db  000h
loc_1_0CD0:
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
loc_1_0D17:
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [edx], eax
        add    byte ptr [edx], al
        add    dword ptr [edx], eax
        add    byte ptr [ebx], al
        add    dword ptr [edx], eax
        add    byte ptr [ecx + eax], al
loc_1_0D2A:
        add    al, byte ptr [eax]
        add    eax, 1000201h
        add    al, byte ptr [edx]
        add    byte ptr [edx], al
        add    al, byte ptr [edx]
        add    byte ptr [ebx], al
        add    al, byte ptr [edx]
        add    byte ptr [edx + eax], al
        add    al, byte ptr [eax]
        add    eax, 6000202h
        add    al, byte ptr [edx]
        add    byte ptr [edi], al
        add    al, byte ptr [edx]
        add    byte ptr [eax], cl
        add    al, byte ptr [edx]
        add    byte ptr [ecx], cl
        add    al, byte ptr [edx]
        add    byte ptr [edx], cl
        add    al, byte ptr [edx]
        add    byte ptr [ebx], cl
        add    al, byte ptr [edx]
        add    byte ptr [edx + eax], cl
        add    al, byte ptr [eax]
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    al, byte ptr [ecx]
        add    dword ptr [edx], eax
        add    eax, dword ptr [ecx]
        add    dword ptr [edx], eax
        add    al, 1
        add    dword ptr [edx], eax
        add    eax, 6020101h
        add    dword ptr [ecx], eax
        add    al, byte ptr [edi]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx]
        add    dword ptr [edx], eax
        add    al, byte ptr [edx]
        add    dword ptr [edx], eax
        add    al, byte ptr [ecx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [ebx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx + eax]
        add    al, byte ptr [edx]
        add    eax, 6020202h
        add    al, byte ptr [edx]
        add    al, byte ptr [edi]
        add    al, byte ptr [edx]
        add    al, byte ptr [ecx]
        add    dword ptr [ecx], eax
        inc    dword ptr [edx]
        add    dword ptr [ecx], eax
        inc    dword ptr [ebx]
        add    dword ptr [ecx], eax
        inc    dword ptr [ecx + eax]
        add    edi, edi
        add    eax, 1ff0101h
        add    al, byte ptr [edx]
        inc    dword ptr [edx]
        add    al, byte ptr [edx]
        db  0FFh
loc_1_0DBC:
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
        add    al, byte ptr [edx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ebx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx + eax]
        add    dword ptr [edx], eax
loc_1_0E18:
        xor    dword ptr [esi], esi
        das
        xor    al, 20h
        push   esp
        outsd
        imul   esp, dword ptr [ebp + 6eh], 20h
        push   edx
        imul   ebp, dword ptr [esi + 67h], 72655320h
        jbe    SHORT loc_1_0E92
        jb     SHORT loc_1_0E4F
        inc    ecx
        popad
        db  070h, 074h
        jb     NEAR PTR loc_1_0E37
loc_1_0E37:
        add    byte ptr [eax + 28h], al
        and    ebp, dword ptr [ecx]
        db  078h, 06Dh
        imul   esi, dword ptr [edx + esi*2 + 63h], 20632e76h
        and    al, 52h
        db  065h, 076h, 069h
        jae    SHORT loc_1_0EB6
        outsd
        outsb
loc_1_0E4F:
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        xor    dword ptr [esi], ebp
        xor    dword ptr [ecx], esi
        and    byte ptr [eax], ah
        and    al, 0
        add    byte ptr [eax + 28h], al
        and    ebp, dword ptr [ecx]
        js     SHORT loc_1_0ED4
        xor    esi, dword ptr [edx]
        arpl   word ptr cs:[eax], sp
        and    al, 52h
        db  065h, 076h, 069h
        db  073h, 069h
        outsd
        outsb
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        xor    dword ptr [esi], ebp
        xor    dword ptr [eax], esi
        and    byte ptr [eax], ah
        and    al, 0
        inc    eax
        sub    byte ptr [ebx], ah
        sub    dword ptr [edi + ebp*2 + 77h], esp
        outsb
        arpl   word ptr [ebx + esi + 32h], sp
        arpl   word ptr cs:[eax], sp
        and    al, 52h
        db  065h, 076h, 069h
        db  073h, 069h
loc_1_0E92:
        outsd
        outsb
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        xor    dword ptr [esi], ebp
        xor    byte ptr [eax], ah
        and    byte ptr [eax + eax], ah
        add    byte ptr [eax], ah
        push   esp
        dec    ebp
        push   ebx
        and    byte ptr [ebp + 61h], al
        insb
        and    byte ptr gs:[ebp + 41h], cl
        inc    ebx
        and    byte ptr [ebx + 6fh], ah
        and    byte ptr gs:[esi + 65h], dh
loc_1_0EB6:
        jb     SHORT loc_1_0F2B
        imul   ebp, dword ptr [edi + 6eh], 322e3220h
        xor    esp, dword ptr [eax]
        db  077h, 069h
        je     SHORT loc_1_0F2D
        and    byte ptr [edi + 70h], ch
        je     SHORT loc_1_0F33
        insd
        imul   edi, dword ptr [edx + 61h], 6e6f6974h
        jae    SHORT loc_1_0EF4
loc_1_0ED4:
        db  076h, 031h
        xor    dword ptr [esi], ebp
        xor    byte ptr [eax], dh
        add    byte ptr [eax], al
        in     al, 0eah
loc_1_0EDE:
        add    byte ptr [eax], al
        mov    al, 0
        inc    eax
        add    byte ptr [eax + ebp*2 + 69h], dl
        jae    SHORT loc_1_0F09
        arpl   word ptr [edi + 64h], bp
        and    byte ptr gs:[ecx + 73h], ch
        and    byte ptr [edx + 65h], dh
        insb
loc_1_0EF4:
        popad
        db  073h, 065h
        and    byte ptr fs:[ebp + 6eh], dh
        db  064h, 065h, 072h, 020h
        insb
        imul   esp, dword ptr [ebx + 65h], 2065636eh
        dec    edi
loc_1_0F09:
        outsb
        insb
        db  079h, 02Ch
        and    byte ptr [ecx + 6ch], al
        insb
        and    byte ptr [edx + 69h], dh
        push   72207374h
        jae    NEAR PTR loc_1_0F82
        db  072h, 076h
        add    dword ptr cs:[eax + 1], esp
        inc    edx
        or     byte ptr [eax], al
        push   ss
        or     byte ptr [ebx], al
loc_1_0F2B:
        mov    al, 4
loc_1_0F2D:
        loopne loc_1_0F2F
loc_1_0F2F:
        add    byte ptr [ecx], al
        and    byte ptr [eax], al
loc_1_0F33:
        add    bh, bh
        call   dword ptr [ebx]
        cld
        add    eax, dword ptr [eax + 4201a007h]
        or     byte ptr [eax], al
        pop    es
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        pop    es
        mov    edi, dword ptr [1600100h]
        inc    edx
        add    byte ptr [eax - 74f8fdedh], al
        sub    eax, 10bc800h
        dec    edx
        add    al, byte ptr [eax]
        mov    al, byte ptr [0c8a00900h]
        add    byte ptr [ecx], al
        add    ah, al
        add    byte ptr [eax + 401a007h], ah
        add    byte ptr [eax], ah
        add    dword ptr [eax + 0fe4001h], esp
        dec    eax
        and    al, ah
        sub    al, byte ptr [ecx]
        inc    edx
        add    al, 0e0h
        add    dword ptr [edx], eax
        add    al, 0e0h
loc_1_0F82:
        or     dword ptr [edx], edx
        add    al, 0e0h
        add    dword ptr [edx + 1c016001h], eax
        add    byte ptr [esi + edx], al
        add    eax, dword ptr [ecx]
        loopne loc_1_0F94
        inc    eax
loc_1_0F94:
        or     al, 0
        push   es
        mov    al, byte ptr [0a007aeb1h]
        add    dword ptr [eax + eax], eax
        sub    eax, 0e020c2h
        add    cl, byte ptr [eax + 141623e2h]
        pop    es
        mov    al, byte ptr [2e000401h]
        add    dword ptr [eax + 1], esp
        inc    edx
        add    eax, dword ptr [eax]
        push   ss
        or     eax, 401a007h
        add    byte ptr [ecx], ah
        pop    es
        mov    byte ptr [eax - 76f900h], dh
        dec    byte ptr [ecx]
        test   al, 9
        test   eax, 2e08a07h
        push   es
        mov    al, byte ptr [561072f5h]
        pop    es
        mov    byte ptr [eax - 5076f900h], ah
        dec    byte ptr [ecx]
        test   al, 9
        test   eax, 7ae08a07h
        push   es
        mov    al, byte ptr [4b1072f5h]
        add    eax, 70401a0h
        mov    byte ptr [eax], al
        or     byte ptr [edi], al
        mov    dword ptr [eax], eax
        db  07Ah, 003h
        add    byte ptr [eax], al
        add    dword ptr [esi], eax
        mov    al, byte ptr [4010aaf0h]
        push   es
        mov    al, byte ptr [0e002aeb1h]
        add    esi, esp
        pop    es
        mov    byte ptr [eax], al
        mov    al, byte ptr [0fe008907h]
        push   es
        mov    al, byte ptr [3510aaf0h]
        add    ah, al
        add    byte ptr [eax + 7e01e004h], ah
        add    al, 0c8h
        add    cl, byte ptr [ecx]
        add    esi, edx
        bound  ecx, qword ptr [eax - 3fh]
        loopne loc_1_102A
        inc    eax
loc_1_102A:
        add    dword ptr [eax + 44001h], esp
        into
        sbb    byte ptr [esi], al
        or     dword ptr [esi], edx
        std
        add    dword ptr [eax + 404001h], esp
        enter  107h, 40h
        pop    es
        mov    byte ptr [ebx], al
        hlt
        pop    es
        mov    dword ptr [edi], edi
        inc    byte ptr [esi]
        mov    al, byte ptr [1910aaf0h]
        add    ah, al
        add    byte ptr [eax - 2d0e5ffah], ah
        adc    byte ptr [eax + ecx*8], dl
        or     byte ptr [esp + eax*2], al
        enter  409h, 46h
        push   es
        mov    al, byte ptr [0d1000f1h]
        pop    es
        and    eax, 13b1c07eh
        enter  1, -20h
        add    eax, 60401a0h
        xchg   edx, eax
        adc    byte ptr [ebx], al
        shl    byte ptr [eax], 0e0h
        adc    ch, dh
        add    eax, esp
        add    dword ptr [eax + eax], eax
        adc    byte ptr [ebx], al
        mov    al, 10h
        inc    dword ptr [ecx]
        mov    al, byte ptr [800401h]
        add    eax, dword ptr [eax - 5fff7ff9h]
        add    al, 0c2h
        int3
        add    byte ptr [edi], 81h
        mov    cl, 9ch
        add    al, byte ptr [edx + 2161e00h]
        pop    es
        xor    dword ptr [ecx - 737e3b5ah], 2e91681h
        add    byte ptr [eax], 7ch
        push   ss
        add    eax, dword ptr [eax]
        add    byte ptr [edi], cl
        add    al, 5bh
        pop    es
        and    ecx, 82070ah
        add    al, 4
        db  0E0h, 001h
        add    byte ptr [eax], 1
        add    ah, 91h
        db  08Ch, 0B1h, 016h, 0D8h, 002h, 082h, 000h, 07Ch, 016h, 0FAh, 0C8h, 020h, 0E0h, 004h, 001h, 082h
        db  0E8h, 020h, 0E0h, 00Ch, 001h, 082h, 0C8h, 020h, 0E0h, 010h, 001h, 080h, 007h, 081h, 0E0h, 088h
        db  0C0h, 0B1h, 013h, 007h, 0C4h, 0B1h, 010h, 0FCh, 007h, 0A0h, 001h, 004h, 000h, 02Eh, 004h, 060h
        db  0B1h, 09Ch, 007h, 081h, 0E0h, 034h, 007h, 082h, 005h, 0FCh, 007h, 083h, 000h, 00Ah, 0CCh, 0B1h
        db  006h, 043h, 016h, 0FDh, 002h, 002h, 006h, 000h, 0D0h, 060h, 001h, 04Eh, 013h, 0EDh, 002h, 021h
        db  0F7h, 000h, 002h, 021h, 0C0h, 000h, 0DCh, 081h, 0D0h, 060h, 001h, 04Fh, 0C0h, 0C1h, 009h, 041h
        db  002h, 021h, 0F0h, 000h, 0DCh, 081h, 002h, 043h, 00Fh, 000h, 002h, 023h, 0F0h, 000h, 0DCh, 083h
        db  002h, 001h, 00Ch, 032h, 0C0h, 0A0h, 004h, 044h, 0C0h, 0E0h, 004h, 046h, 0C1h, 003h, 061h, 002h
        db  005h, 084h, 0C8h, 004h, 004h, 048h, 0C1h, 003h, 005h, 084h, 0A1h, 004h, 0A1h, 001h, 0C8h, 004h
        db  00Ch, 030h, 0C1h, 003h, 005h, 084h, 004h, 0F1h, 006h, 004h, 016h, 0FDh, 002h, 008h, 0B0h, 000h
        db  009h, 0A8h, 0C2h, 060h, 00Ch, 030h, 002h, 029h, 003h, 0FFh, 009h, 0A9h, 002h, 029h, 000h, 040h
        db  007h, 080h, 0A0h, 000h, 009h, 0A0h, 007h, 08Ah, 0AFh, 0FEh, 002h, 02Ah, 003h, 0FFh, 009h, 0AAh
        db  002h, 001h, 00Ch, 032h, 002h, 005h, 000h, 000h, 0C1h, 003h, 005h, 084h, 007h, 011h, 005h, 0C1h
        db  005h, 085h, 006h, 004h, 013h, 00Bh, 080h, 085h, 01Ah, 0F9h, 080h, 005h, 01Ah, 0F8h, 082h, 085h
        db  01Ah, 0F5h, 082h, 005h, 01Ah, 0F4h, 082h, 045h, 01Ah, 0F1h, 010h, 0F1h, 02Dh, 020h, 000h, 002h
        db  001h, 060h, 001h, 040h, 040h, 000h, 016h, 006h, 007h, 08Ah, 008h, 000h, 001h, 0A0h, 001h, 040h
        db  040h, 000h, 010h, 048h, 001h, 060h, 001h, 042h, 080h, 000h, 016h, 006h, 007h, 08Ah, 010h, 000h
        db  001h, 0A0h, 001h, 042h, 080h, 000h, 010h, 03Eh, 001h, 060h, 001h, 002h, 010h, 000h, 016h, 00Ah
        db  001h, 060h, 001h, 000h, 004h, 000h, 016h, 006h, 007h, 08Ah, 080h, 000h, 001h, 0A0h, 001h, 002h
        db  010h, 000h, 010h, 030h, 001h, 060h, 001h, 002h, 008h, 000h, 016h, 00Ah, 001h, 060h, 001h, 000h
        db  004h, 000h, 016h, 006h, 001h, 0A0h, 001h, 002h, 008h, 000h, 002h, 00Dh, 000h, 001h, 010h, 00Dh
        db  001h, 060h, 001h, 002h, 004h, 000h, 016h, 016h, 001h, 060h, 001h, 000h, 008h, 000h, 016h, 012h
        db  001h, 0A0h, 001h, 002h, 004h, 000h, 002h, 00Dh, 000h, 002h, 0C3h, 0A0h, 001h, 00Eh, 0C3h, 0E0h
        db  001h, 010h, 007h, 08Ah, 020h, 000h, 001h, 060h, 001h, 000h, 080h, 000h, 013h, 00Bh, 007h, 08Ah
        db  040h, 000h, 010h, 008h, 007h, 08Ah, 000h, 004h, 010h, 005h, 007h, 08Ah, 000h, 002h, 010h, 002h
        db  007h, 08Ah, 000h, 008h, 003h, 000h, 000h, 000h, 004h, 0E0h, 001h, 082h, 007h, 08Bh, 005h, 0E0h
        db  0CEh, 0CAh, 0CEh, 0CDh, 0CEh, 0CEh, 0C6h, 0CFh, 0C3h, 020h, 007h, 058h, 023h, 020h, 0E0h, 004h
        db  013h, 012h, 007h, 08Bh, 0FFh, 018h, 002h, 08Ah, 080h, 000h, 013h, 00Ah, 005h, 08Bh, 0A2h, 0CDh
        db  002h, 08Ah, 040h, 000h, 013h, 005h, 002h, 08Ah, 020h, 000h, 013h, 002h, 007h, 08Bh, 0FFh, 01Dh
        db  0C8h, 00Bh, 001h, 004h, 010h, 00Dh, 0D3h, 020h, 001h, 005h, 011h, 0FDh, 0D8h, 020h, 007h, 0DFh
        db  001h, 017h, 007h, 08Bh, 0FFh, 080h, 0C8h, 00Bh, 001h, 004h, 0E8h, 020h, 0E0h, 00Ah, 001h, 000h
        db  0C2h, 0E0h, 001h, 004h, 022h, 0E0h, 0E1h, 088h, 016h, 0FBh, 002h, 0E0h, 000h, 0A0h, 004h, 0E0h
        db  001h, 082h, 0E8h, 020h, 0E0h, 00Ah, 001h, 000h, 0C2h, 0E0h, 001h, 000h, 022h, 0E0h, 0E0h, 006h
        db  013h, 0F8h, 001h, 0A0h, 001h, 040h, 0F6h, 000h, 004h, 060h, 0B0h, 082h, 003h, 000h, 000h, 002h
        db  007h, 0A0h, 001h, 002h, 0DFh, 0FFh, 003h, 090h, 0FFh, 0FFh, 003h, 080h, 003h, 000h, 000h, 002h
        db  007h, 009h, 0C2h, 0A0h, 001h, 004h, 001h, 08Ah, 000h, 080h, 052h, 04Ah, 0D2h, 089h, 0C8h, 00Ah
        db  001h, 004h, 0D2h, 0A0h, 001h, 004h, 016h, 0F9h, 005h, 049h, 001h, 089h, 080h, 000h, 001h, 049h
        db  040h, 000h, 013h, 00Eh, 0F8h, 009h, 007h, 03Ah, 0C2h, 060h, 007h, 036h, 016h, 003h, 003h, 090h
        db  0FFh, 0FFh, 003h, 080h, 004h, 0E0h, 007h, 036h, 004h, 054h, 003h, 090h, 0FFh, 0FFh, 003h, 080h
        db  022h, 060h, 0E1h, 088h, 013h, 0C2h, 004h, 0E0h, 001h, 082h, 004h, 060h, 0B3h, 0D2h, 003h, 000h
        db  000h, 002h, 004h, 0C9h, 0C1h, 0A0h, 006h, 034h, 016h, 004h, 006h, 0A0h, 0C1h, 0EAh, 004h, 0E0h
        db  008h, 0D2h, 007h, 086h, 005h, 0E8h, 001h, 0A0h, 001h, 040h, 080h, 000h, 0C2h, 020h, 001h, 084h
        db  048h, 020h, 0E0h, 008h, 001h, 084h, 022h, 020h, 0E0h, 008h, 013h, 008h, 001h, 060h, 001h, 0AEh
        db  000h, 001h, 016h, 004h, 001h, 0E0h, 006h, 034h, 080h, 000h, 010h, 006h, 0C2h, 020h, 008h, 0E4h
        db  013h, 006h, 001h, 0E0h, 006h, 034h, 008h, 000h, 004h, 0E0h, 006h, 030h, 007h, 009h, 005h, 0A0h
        db  005h, 0EEh, 006h, 020h, 005h, 0ECh, 016h, 002h, 0C2h, 016h, 016h, 003h, 0C2h, 049h, 016h, 012h
        db  003h, 080h, 0C5h, 098h, 0C1h, 0E8h, 000h, 002h, 0E9h, 0E0h, 0E0h, 014h, 000h, 004h, 004h, 0D7h
        db  002h, 027h, 000h, 008h, 006h, 0A0h, 0C1h, 080h, 0C2h, 016h, 013h, 004h, 0C8h, 028h, 000h, 008h
        db  005h, 0ECh, 013h, 0EFh, 004h, 054h, 003h, 000h, 000h, 002h, 0C1h, 0E0h, 001h, 086h, 002h, 047h
        db  000h, 00Eh, 0C2h, 0A7h, 0E1h, 092h, 004h, 05Ah, 007h, 08Ah, 0A0h, 000h, 0C8h, 00Ah, 001h, 086h
        db  0A1h, 0C7h, 002h, 027h, 0E1h, 09Ah, 0E8h, 037h, 006h, 034h, 0E8h, 017h, 006h, 0D2h, 004h, 0E0h
        db  006h, 030h, 004h, 060h, 0B9h, 038h, 0E8h, 00Ah, 006h, 0D2h, 001h, 0E0h, 006h, 034h, 008h, 000h
        db  004h, 0E0h, 006h, 030h, 0E0h, 020h, 0E0h, 018h, 004h, 05Bh, 005h, 0A0h, 008h, 0D2h, 088h, 020h
        db  008h, 0D2h, 0E0h, 016h, 01Ah, 0E5h, 004h, 0E0h, 008h, 0D2h, 006h, 0A0h, 0DFh, 0ECh, 003h, 080h
        db  005h, 0A0h, 008h, 0E4h, 003h, 080h, 0C3h, 001h, 013h, 0FBh, 001h, 060h, 009h, 018h, 000h, 001h
        db  013h, 002h, 005h, 0A0h, 008h, 0E4h, 003h, 000h, 000h, 002h, 0C0h, 0E0h, 001h, 06Ch, 0C3h, 020h
        db  001h, 08Ah, 098h, 020h, 001h, 083h, 0E2h, 002h, 016h, 03Ch, 0C0h, 041h, 013h, 006h, 0C8h, 001h
        db  001h, 06Ch, 0C2h, 0E0h, 0FCh, 002h, 011h, 001h, 010h, 070h, 001h, 060h, 001h, 09Ch, 000h, 040h
        db  016h, 06Eh, 0D8h, 020h, 008h, 0E1h, 001h, 083h, 0C0h, 041h, 013h, 004h, 0C8h, 001h, 001h, 08Ah
        db  0C8h, 001h, 008h, 0CAh, 007h, 086h, 000h, 043h, 0C8h, 006h, 001h, 06Ch, 002h, 007h, 0FCh, 002h
        db  0C2h, 017h, 004h, 060h, 0B6h, 0DEh, 004h, 0E0h, 008h, 0CAh, 061h, 0C7h, 007h, 008h, 001h, 060h
        db  0FCh, 006h, 000h, 040h, 013h, 002h, 002h, 008h, 000h, 001h, 010h, 009h, 0C2h, 04Ch, 0C3h, 020h
        db  0FCh, 000h, 013h, 02Eh, 0C8h, 00Ch, 001h, 06Ch, 0C2h, 0E0h, 0FCh, 002h, 011h, 01Fh, 001h, 04Bh
        db  001h, 000h, 016h, 0F4h, 022h, 0C8h, 013h, 016h, 001h, 0CBh, 040h, 000h, 0C8h, 00Bh, 0FCh, 002h
        db  010h, 011h, 001h, 060h, 001h, 09Ch, 000h, 040h, 016h, 038h, 0C1h, 0E0h, 008h, 0CAh, 0C3h, 001h
        db  013h, 021h, 0C2h, 04Ch, 013h, 015h, 0C8h, 00Ch, 001h, 06Ch, 0C2h, 0E0h, 0FCh, 002h, 011h, 006h
        db  081h, 0CCh, 013h, 0D1h, 0C2h, 04Ch, 0C3h, 020h, 0FCh, 000h, 010h, 0F4h, 0C8h, 009h, 001h, 06Ch
        db  0C2h, 0E0h, 0FCh, 002h, 016h, 01Eh, 007h, 0A0h, 0FCh, 002h, 080h, 000h, 0C3h, 009h, 010h, 019h
        db  0C8h, 009h, 001h, 06Ch, 0C2h, 0E0h, 0FCh, 002h, 016h, 005h, 007h, 0A0h, 0FCh, 002h, 080h, 000h
        db  0C3h, 009h, 010h, 00Fh, 0C2h, 0E0h, 00Ch, 002h, 011h, 001h, 010h, 00Fh, 0D8h, 020h, 0E2h, 002h
        db  001h, 083h, 009h, 08Bh, 009h, 08Bh, 009h, 08Bh, 009h, 08Bh, 007h, 0A0h, 001h, 08Ah, 000h, 043h
        db  010h, 004h, 0C8h, 00Ch, 001h, 08Ah, 0C8h, 00Ch, 008h, 0CAh, 083h, 001h, 016h, 02Fh, 0C8h, 003h
        db  001h, 06Ch, 001h, 040h, 000h, 010h, 016h, 017h, 0C2h, 0E0h, 006h, 02Eh, 013h, 003h, 0C2h, 0E0h
        db  005h, 0F8h, 016h, 003h, 0C2h, 0E0h, 009h, 014h, 013h, 00Eh, 001h, 0E0h, 007h, 03Ah, 080h, 000h
        db  001h, 080h, 000h, 010h, 0C2h, 0E0h, 007h, 036h, 013h, 006h, 004h, 0E0h, 007h, 036h, 001h, 080h
        db  000h, 020h, 004h, 060h, 0B9h, 038h, 001h, 040h, 000h, 020h, 013h, 0F9h, 003h, 090h, 011h, 0FFh
        db  003h, 080h, 001h, 008h, 004h, 000h, 016h, 014h, 001h, 060h, 009h, 018h, 000h, 001h, 016h, 010h
        db  001h, 088h, 01Ah, 000h, 001h, 0C8h, 001h, 000h, 0C5h, 0C8h, 010h, 00Ah, 0C1h, 081h, 0C8h, 001h
        db  001h, 06Ch, 002h, 007h, 0FCh, 000h, 0C0h, 077h, 0C2h, 017h, 001h, 048h, 018h, 000h, 013h, 0E9h
        db  001h, 040h, 000h, 040h, 016h, 01Bh, 001h, 080h, 000h, 045h, 0C1h, 046h, 0D0h, 020h, 0FCh, 007h
        db  028h, 020h, 0E0h, 006h, 081h, 060h, 008h, 0CAh, 016h, 004h, 004h, 0E0h, 008h, 0CAh, 001h, 0C0h
        db  000h, 004h, 0C2h, 0E0h, 0FCh, 008h, 011h, 008h, 0C2h, 0E0h, 0FCh, 00Eh, 015h, 007h, 013h, 006h
        db  0C2h, 0E0h, 0FCh, 014h, 015h, 003h, 013h, 002h, 001h, 0C0h, 000h, 001h, 001h, 048h, 001h, 000h
        db  013h, 005h, 001h, 000h, 040h, 080h, 013h, 06Ch, 004h, 060h, 0B5h, 066h, 001h, 0C0h, 000h, 040h
        db  001h, 040h, 000h, 004h, 016h, 00Bh, 001h, 048h, 000h, 001h, 016h, 003h, 001h, 040h, 040h, 000h
        db  013h, 005h, 001h, 0C8h, 040h, 000h, 005h, 0A0h, 008h, 0E4h, 0C5h, 0C8h, 005h, 0C7h, 0C2h, 0D7h
        db  062h, 0C4h, 005h, 00Bh, 002h, 02Bh, 0FFh, 0FCh, 0C5h, 0CBh, 015h, 002h, 081h, 046h, 013h, 063h
        db  001h, 008h, 05Eh, 000h, 016h, 060h, 001h, 008h, 000h, 088h, 013h, 00Dh, 001h, 040h, 000h, 001h
        db  013h, 00Ah, 001h, 008h, 000h, 002h, 016h, 007h, 001h, 048h, 000h, 001h, 016h, 004h, 007h, 08Ah
        db  000h, 080h, 006h, 0A0h, 0B5h, 01Eh, 002h, 086h, 000h, 043h, 016h, 010h, 001h, 040h, 040h, 000h
        db  016h, 00Bh, 001h, 008h, 000h, 003h, 013h, 008h, 0C2h, 084h, 002h, 02Ah, 0FFh, 0D8h, 0C8h, 006h
        db  001h, 06Ch, 068h, 00Ah, 0FCh, 004h, 010h, 05Fh, 004h, 060h, 0B8h, 0A2h, 0C0h, 000h, 011h, 0FCh
        db  0C2h, 060h, 009h, 018h, 001h, 040h, 040h, 000h, 016h, 00Ah, 001h, 048h, 000h, 001h, 013h, 035h
        db  001h, 048h, 000h, 002h, 013h, 00Ah, 001h, 049h, 000h, 004h, 016h, 0EEh, 010h, 006h, 001h, 049h
        db  000h, 002h, 013h, 003h, 001h, 008h, 000h, 003h, 013h, 068h, 001h, 049h, 000h, 001h, 016h, 00Dh
        db  0C8h, 006h, 001h, 06Ch, 002h, 007h, 0FCh, 002h, 0A1h, 0E0h, 008h, 0DEh, 0CDh, 0E0h, 005h, 0EEh
        db  0C5h, 0E0h, 0FCh, 004h, 0C8h, 020h, 008h, 0DEh, 0FCh, 004h, 001h, 000h, 040h, 080h, 016h, 001h
        db  0C1h, 046h, 004h, 0E0h, 0FCh, 000h, 007h, 087h, 005h, 0F8h, 0C2h, 017h, 013h, 008h, 005h, 0C7h
        db  0C8h, 017h, 001h, 06Ch, 0C8h, 005h, 0FCh, 000h, 0C5h, 0C6h, 004h, 060h, 0B5h, 066h, 0CDh, 0C5h
        db  0C5h, 0C6h, 004h, 060h, 0B5h, 066h, 004h, 060h, 0B8h, 086h, 0C8h, 006h, 001h, 06Ch, 081h, 085h
        db  013h, 01Ah, 0C2h, 0E0h, 0FCh, 004h, 015h, 017h, 0C2h, 086h, 0A2h, 08Ah, 0C1h, 0AAh, 00Ch, 032h
        db  0C8h, 006h, 001h, 06Ch, 0A8h, 00Bh, 0FCh, 004h, 009h, 01Ah, 0C8h, 00Ah, 001h, 06Ch, 0C2h, 0E0h
        db  0FCh, 002h, 004h, 0E0h, 0FCh, 000h, 0C8h, 006h, 001h, 06Ch, 0C8h, 00Bh, 0FCh, 002h, 006h, 0A0h
        db  0C0h, 0ECh, 0C8h, 006h, 001h, 06Ch, 004h, 0E0h, 0FCh, 000h, 001h, 0A0h, 0FCh, 002h, 000h, 002h
        db  007h, 087h, 006h, 030h, 001h, 0E7h, 000h, 004h, 000h, 040h, 004h, 0D7h, 002h, 027h, 000h, 00Ch
        db  0C2h, 005h, 006h, 0A0h, 0C1h, 096h, 001h, 0C0h, 000h, 020h, 004h, 060h, 0B5h, 066h, 001h, 048h
        db  018h, 000h, 013h, 003h, 001h, 048h, 010h, 000h, 016h, 002h, 005h, 0A0h, 008h, 0E4h, 002h, 086h
        db  000h, 043h, 013h, 003h, 001h, 000h, 040h, 080h, 013h, 0ABh, 0C8h, 006h, 001h, 06Ch, 004h, 0E0h
        db  0FCh, 000h, 0C2h, 085h, 006h, 0A0h, 0C0h, 0ECh, 010h, 0E8h, 003h, 000h, 000h, 002h, 0C2h, 020h
        db  001h, 08Ch, 0C0h, 0E0h, 001h, 06Ch, 0C2h, 020h, 005h, 0F4h, 0C8h, 028h, 000h, 008h, 001h, 06Ch
        db  0C1h, 0E8h, 000h, 00Ah, 0C3h, 020h, 0FCh, 002h, 001h, 00Ch, 0FEh, 000h, 016h, 03Fh, 001h, 047h
        db  000h, 040h, 013h, 054h, 004h, 0E0h, 005h, 0F4h, 0CAh, 00Ch, 000h, 008h, 001h, 047h, 000h, 008h
        db  016h, 006h, 0C2h, 0A8h, 000h, 006h, 006h, 0A0h, 0C0h, 0ECh, 004h, 0E8h, 000h, 006h, 001h, 007h
        db  000h, 020h, 016h, 029h, 0C1h, 0E8h, 000h, 002h, 001h, 0E7h, 000h, 004h, 000h, 020h, 0C2h, 0D7h
        db  013h, 003h, 004h, 0D7h, 001h, 0C3h, 080h, 000h, 002h, 027h, 000h, 010h, 006h, 0A0h, 0C1h, 080h
        db  0C2h, 020h, 005h, 0F0h, 013h, 007h, 0C2h, 0E0h, 005h, 0F6h, 016h, 004h, 0C8h, 018h, 005h, 0F0h
        db  006h, 0A0h, 0BBh, 0A4h, 0C8h, 003h, 001h, 06Ch, 011h, 003h, 003h, 090h, 011h, 0FFh, 003h, 080h
        db  003h, 090h, 011h, 0F8h, 002h, 0E0h, 000h, 0C0h, 0C3h, 060h, 000h, 0FAh, 0C3h, 0A0h, 000h, 0FCh
        db  0C3h, 0E0h, 000h, 0FEh, 004h, 054h, 004h, 0E8h, 000h, 002h, 010h, 0E2h, 0C2h, 0E8h, 000h, 008h
        db  0C2h, 0A8h, 000h, 006h, 0C3h, 00Ch, 011h, 033h, 023h, 020h, 0E0h, 00Ah, 013h, 045h, 023h, 020h
        db  0E0h, 010h, 013h, 046h, 023h, 020h, 0E0h, 00Eh, 013h, 013h, 021h, 0E0h, 0E0h, 016h, 016h, 0B2h
        db  023h, 020h, 0E0h, 006h, 016h, 003h, 027h, 020h, 0E4h, 06Ah, 013h, 00Ah, 0C2h, 0E8h, 000h, 008h
        db  0C2h, 0A8h, 000h, 006h, 001h, 04Ch, 000h, 088h, 016h, 0A5h, 001h, 00Ch, 000h, 044h, 016h, 0A2h
        db  006h, 020h, 008h, 0C8h, 013h, 09Fh, 0C8h, 00Ah, 001h, 06Ch, 0C8h, 020h, 0E0h, 004h, 0FCh, 002h
        db  0C8h, 00Bh, 001h, 06Ch, 007h, 0A0h, 0FCh, 002h, 081h, 000h, 0C3h, 020h, 001h, 080h, 001h, 0A0h
        db  001h, 080h, 0C4h, 000h, 0C8h, 00Ch, 001h, 080h, 0C8h, 00Ah, 001h, 08Ch, 010h, 0B3h, 0C2h, 00Ah
        db  013h, 00Fh, 0C8h, 008h, 001h, 06Ch, 0C2h, 0A0h, 0FCh, 000h, 0C3h, 020h, 0FCh, 002h, 023h, 020h
        db  0E0h, 012h, 016h, 0F5h, 0C8h, 00Bh, 001h, 06Ch, 0C8h, 00Ch, 0FCh, 002h, 004h, 060h, 0B8h, 0BEh
        db  007h, 08Ah, 004h, 000h, 004h, 060h, 0B3h, 07Ch, 007h, 08Ah, 000h, 020h, 004h, 060h, 0B3h, 07Ch
        db  007h, 08Ah, 002h, 000h, 027h, 020h, 0E0h, 00Eh, 016h, 002h, 006h, 0A0h, 0B5h, 01Eh, 0C2h, 0A8h
        db  000h, 006h, 004h, 060h, 0B8h, 0DCh, 003h, 000h, 000h, 002h, 001h, 0C0h, 000h, 010h, 0C2h, 0E0h
        db  006h, 02Eh, 013h, 003h, 0C2h, 0E0h, 005h, 0F8h, 016h, 003h, 0C2h, 0E0h, 009h, 014h, 013h, 005h
        db  001h, 0E0h, 007h, 03Ah, 080h, 000h, 001h, 080h, 000h, 010h, 003h, 090h, 07Fh, 0FFh, 003h, 080h
        db  003h, 000h, 000h, 002h, 0C2h, 020h, 005h, 0F6h, 0E2h, 020h, 005h, 0F4h, 016h, 02Fh, 0D8h, 020h
        db  008h, 0E0h, 001h, 080h, 0C8h, 02Bh, 000h, 006h, 001h, 08Ch, 007h, 0A0h, 008h, 0C8h, 000h, 004h
        db  0C8h, 00Bh, 005h, 0F4h, 003h, 090h, 0FFh, 0FFh, 003h, 080h, 003h, 000h, 000h, 002h, 0C2h, 00Bh
        db  0C3h, 020h, 005h, 0F4h, 013h, 00Fh, 0C2h, 0A8h, 000h, 00Ah, 001h, 04Ah, 000h, 010h, 016h, 017h
        db  022h, 0A0h, 0E0h, 004h, 016h, 01Bh, 0C3h, 008h, 006h, 0A0h, 0BBh, 062h, 0C2h, 00Ch, 0C3h, 020h
        db  005h, 0F4h, 016h, 014h, 001h, 068h, 000h, 00Ah, 000h, 010h, 013h, 003h, 0C2h, 0E0h, 005h, 0F6h
        db  016h, 006h, 006h, 0A0h, 0BBh, 0A4h, 003h, 090h, 0FFh, 0FFh, 003h, 080h, 0C2h, 00Bh, 007h, 087h
        db  005h, 0F0h, 006h, 0A0h, 0C1h, 080h, 003h, 090h, 0FFh, 0FFh, 003h, 080h, 007h, 087h, 005h, 0F0h
        db  006h, 0A0h, 0C1h, 0C6h, 003h, 080h, 003h, 000h, 000h, 002h, 007h, 087h, 005h, 0F0h, 0C2h, 0CBh
        db  016h, 008h, 006h, 0A0h, 0BBh, 062h, 007h, 020h, 005h, 0F6h, 0CBh, 060h, 005h, 0F4h, 000h, 002h
        db  003h, 080h, 004h, 0E0h, 005h, 0F6h, 0C2h, 020h, 005h, 0F4h, 016h, 005h, 0C2h, 017h, 013h, 003h
        db  0C5h, 0D8h, 006h, 0A0h, 0BBh, 0A4h, 003h, 080h, 003h, 000h, 000h, 002h, 0C2h, 08Bh, 006h, 0A0h
        db  0BBh, 062h, 0C1h, 0CAh, 002h, 027h, 000h, 010h, 007h, 088h, 005h, 0F0h, 0C1h, 088h, 0C2h, 018h
        db  013h, 026h, 082h, 0A8h, 000h, 002h, 016h, 0FAh, 0C2h, 0E8h, 000h, 00Ah, 022h, 0E0h, 0E0h, 01Eh
        db  016h, 0F5h, 0C5h, 098h, 022h, 0E0h, 0E0h, 01Ch, 016h, 00Bh, 0C8h, 028h, 000h, 006h, 000h, 0F4h
        db  002h, 0E0h, 000h, 0E0h, 006h, 0A0h, 0C0h, 0ECh, 002h, 0E0h, 000h, 0C0h, 004h, 0E8h, 000h, 006h
        db  022h, 0E0h, 0E0h, 018h, 013h, 0E4h, 0EAh, 020h, 0E0h, 022h, 000h, 00Ah, 0EAh, 0A0h, 0E0h, 018h
        db  000h, 004h, 004h, 0DAh, 006h, 0A0h, 0C1h, 080h, 006h, 047h, 0C2h, 006h, 010h, 0D8h, 0C8h, 006h
        db  005h, 0F2h, 0CBh, 060h, 005h, 0F4h, 000h, 002h, 004h, 054h, 0C2h, 020h, 005h, 0F4h, 013h, 013h
        db  001h, 0E0h, 001h, 09Ch, 040h, 000h, 00Bh, 08Bh, 00Bh, 08Bh, 001h, 060h, 001h, 09Ch, 040h, 000h
        db  016h, 00Ah, 0C2h, 060h, 001h, 06Ch, 0C8h, 028h, 000h, 006h, 001h, 06Ch, 0C2h, 0A0h, 0FCh, 002h
        db  011h, 003h, 0C8h, 009h, 001h, 06Ch, 004h, 05Bh, 0C8h, 009h, 001h, 06Ch, 0C2h, 04Bh, 007h, 087h
        db  005h, 0F0h, 006h, 0A0h, 0C1h, 0C6h, 004h, 0E0h, 005h, 0F4h, 004h, 059h, 0C2h, 0A8h, 000h, 00Ah
        db  011h, 00Dh, 0F2h, 0A0h, 008h, 0E0h, 0D8h, 00Ah, 001h, 080h, 0C8h, 028h, 000h, 006h, 001h, 08Ch
        db  007h, 0A0h, 008h, 0C8h, 000h, 004h, 0C8h, 008h, 005h, 0F4h, 004h, 05Bh, 0C3h, 020h, 001h, 06Ch
        db  0C8h, 028h, 000h, 006h, 001h, 06Ch, 0C8h, 020h, 0FCh, 00Eh, 001h, 0BCh, 0C8h, 020h, 0FCh, 010h
        db  001h, 0B0h, 0C8h, 020h, 0FCh, 012h, 001h, 0B2h, 0C8h, 00Ch, 001h, 06Ch, 0F2h, 0A0h, 008h, 0E0h
        db  001h, 08Ah, 010h, 000h, 001h, 0A0h, 001h, 080h, 0C4h, 000h, 010h, 0DDh, 007h, 033h, 007h, 033h
        db  010h, 00Ch, 007h, 013h, 007h, 023h, 000h, 002h, 0C8h, 0CBh, 000h, 006h, 002h, 023h, 000h, 018h
        db  0CCh, 0E0h, 001h, 06Ch, 0CCh, 0CDh, 0CCh, 0CEh, 0CCh, 0CFh, 007h, 083h, 006h, 030h, 0C1h, 0D3h
        db  013h, 00Ah, 007h, 083h, 007h, 036h, 0C1h, 0D3h, 013h, 006h, 007h, 083h, 000h, 0A0h, 000h, 093h
        db  0C8h, 00Ch, 001h, 06Ch, 003h, 080h, 007h, 063h, 000h, 002h, 015h, 039h, 0C2h, 063h, 000h, 004h
        db  042h, 063h, 000h, 006h, 013h, 0DBh, 0C3h, 063h, 000h, 01Ah, 0D2h, 049h, 013h, 00Ch, 006h, 0C9h
        db  072h, 049h, 0D2h, 069h, 0E1h, 0C2h, 006h, 0C9h, 072h, 049h, 048h, 0E9h, 0E0h, 004h, 000h, 004h
        db  0CBh, 049h, 000h, 002h, 004h, 052h, 0D2h, 069h, 0E1h, 0C2h, 006h, 0C9h, 072h, 049h, 002h, 029h
        db  000h, 010h, 0C2h, 0C3h, 0A2h, 0E9h, 0E1h, 0AAh, 0C3h, 01Bh, 002h, 089h, 000h, 012h, 016h, 00Dh
        db  0C8h, 00Ch, 001h, 06Ch, 0C6h, 0E0h, 0FCh, 000h, 016h, 00Dh, 048h, 0E9h, 0E0h, 004h, 000h, 004h
        db  0CBh, 049h, 000h, 002h, 0CBh, 04Ch, 000h, 004h, 004h, 052h, 0C6h, 0DCh, 016h, 003h, 048h, 0E9h
        db  0E0h, 004h, 000h, 004h, 0CBh, 049h, 000h, 002h, 0CBh, 04Ch, 000h, 004h, 004h, 052h, 003h, 090h
        db  001h, 0FFh, 000h, 093h, 0C8h, 00Ch, 001h, 06Ch, 003h, 080h, 003h, 000h, 000h, 002h, 0C2h, 0DBh
        db  0C2h, 063h, 000h, 004h, 042h, 04Bh, 013h, 09Dh, 0D2h, 049h, 013h, 00Eh, 006h, 0C9h, 072h, 049h
        db  0D2h, 069h, 0E1h, 0C2h, 006h, 0C9h, 072h, 049h, 048h, 0E9h, 0E0h, 004h, 000h, 004h, 0CBh, 049h
        db  000h, 002h, 003h, 090h, 0FFh, 0FFh, 003h, 080h, 0D2h, 069h, 0E1h, 0C2h, 006h, 0C9h, 072h, 049h
        db  002h, 029h, 000h, 010h, 0C2h, 0C3h, 0A2h, 0E9h, 0E1h, 0AAh, 0C3h, 01Bh, 002h, 089h, 000h, 012h
        db  016h, 00Fh, 0C8h, 00Ch, 001h, 06Ch, 0C6h, 0E0h, 0FCh, 000h, 016h, 003h, 048h, 0E9h, 0E0h, 004h
        db  000h, 004h, 0CBh, 049h, 000h, 002h, 0CBh, 04Ch, 000h, 004h, 003h, 090h, 0FFh, 0FFh, 003h, 080h
        db  0C6h, 0DCh, 013h, 0F4h, 010h, 0F6h, 003h, 000h, 000h, 002h, 0C2h, 0BBh, 0C1h, 0BBh, 0D1h, 086h
        db  013h, 003h, 0EAh, 086h, 000h, 004h, 010h, 013h, 0D1h, 0A6h, 0E1h, 0C2h, 006h, 0C6h, 071h, 086h
        db  0C1h, 0CAh, 0A1h, 0E6h, 0E1h, 0BAh, 0EAh, 0A6h, 0E0h, 014h, 000h, 004h, 0C2h, 01Bh, 002h, 086h
        db  000h, 002h, 016h, 003h, 006h, 0A0h, 0C1h, 0A6h, 010h, 002h, 006h, 0A0h, 0C1h, 080h, 004h, 0DAh
        db  003h, 080h, 003h, 000h, 000h, 002h, 0C2h, 0ABh, 000h, 006h, 007h, 08Ch, 005h, 0E8h, 0C2h, 05Ch
        db  013h, 016h, 0C1h, 0A0h, 005h, 0ECh, 081h, 08Ah, 01Ah, 01Ah, 0C1h, 0C6h, 0C2h, 009h, 0C2h, 059h
        db  013h, 020h, 0A1h, 0E9h, 000h, 008h, 082h, 087h, 012h, 0F9h, 0A2h, 0A9h, 000h, 008h, 062h, 087h
        db  0CAh, 0CAh, 000h, 008h, 06Ah, 04Ah, 000h, 008h, 0C6h, 0C9h, 0C6h, 00Bh, 003h, 080h, 0CAh, 0CAh
        db  000h, 008h, 0C8h, 00Ah, 005h, 0ECh, 004h, 0DBh, 0CFh, 00Bh, 0C7h, 00Bh, 003h, 080h, 061h, 08Ah
        db  0CAh, 046h, 000h, 008h, 0CAh, 0CAh, 000h, 008h, 0C8h, 00Ah, 005h, 0ECh, 0C6h, 0C9h, 0C7h, 00Bh
        db  003h, 080h, 062h, 087h, 0CAh, 0CAh, 000h, 008h, 004h, 0DBh, 0C6h, 00Bh, 0CBh, 00Bh, 000h, 002h
        db  003h, 080h, 003h, 000h, 000h, 002h, 0C1h, 0BBh, 0C2h, 0DBh, 007h, 08Ch, 005h, 0E8h, 0C2h, 04Ch
        db  004h, 0EDh, 000h, 002h, 0C2h, 009h, 0C2h, 059h, 013h, 018h, 081h, 0A9h, 000h, 002h, 016h, 0FAh
        db  082h, 0E9h, 000h, 004h, 016h, 0F7h, 0CBh, 049h, 000h, 004h, 0C2h, 099h, 0C6h, 00Ah, 013h, 00Ah
        db  083h, 008h, 013h, 004h, 0AAh, 0A9h, 000h, 008h, 000h, 008h, 003h, 080h, 0A8h, 02Ah, 000h, 008h
        db  005h, 0ECh, 003h, 080h, 0CBh, 008h, 000h, 002h, 003h, 080h, 007h, 02Dh, 000h, 002h, 007h, 08Ch
        db  000h, 008h, 0A3h, 006h, 0C2h, 04Ch, 0C2h, 009h, 0C2h, 059h, 013h, 013h, 082h, 0E9h, 000h, 004h
        db  016h, 0FAh, 007h, 0ADh, 000h, 002h, 000h, 001h, 0CBh, 049h, 000h, 004h, 0C6h, 019h, 013h, 001h
        db  003h, 080h, 083h, 008h, 016h, 004h, 049h, 0A0h, 0E0h, 014h, 000h, 004h, 003h, 080h, 0CBh, 008h
        db  000h, 002h, 003h, 080h, 003h, 000h, 000h, 002h, 006h, 00Bh, 011h, 015h, 013h, 043h, 007h, 08Bh
        db  04Eh, 000h, 001h, 060h, 001h, 042h, 000h, 080h, 013h, 002h, 007h, 08Bh, 03Ah, 000h, 0C8h, 00Bh
        db  001h, 044h, 001h, 0E0h, 001h, 040h, 002h, 000h, 001h, 0E0h, 001h, 040h, 020h, 000h, 007h, 084h
        db  0BCh, 004h, 004h, 060h, 0BCh, 012h, 0C8h, 020h, 0E0h, 016h, 000h, 0E0h, 0C2h, 0E0h, 009h, 018h
        db  022h, 0E0h, 0E0h, 010h, 016h, 003h, 0E8h, 020h, 0E0h, 014h, 000h, 0E0h, 0C8h, 020h, 0E0h, 004h
        db  001h, 082h, 0C8h, 020h, 000h, 0E2h, 001h, 08Ah, 004h, 0E0h, 008h, 0CAh, 004h, 0E0h, 005h, 0F4h
        db  004h, 0E0h, 005h, 0F8h, 004h, 0E0h, 005h, 0F0h, 004h, 0E0h, 007h, 042h, 007h, 0A0h, 001h, 088h
        db  000h, 020h, 0C2h, 0E0h, 008h, 0E2h, 013h, 009h, 007h, 0A0h, 001h, 088h, 000h, 080h, 0E8h, 020h
        db  0E0h, 016h, 001h, 080h, 001h, 0E0h, 001h, 082h, 003h, 000h, 007h, 08Bh, 0A0h, 000h, 0E8h, 00Bh
        db  001h, 086h, 003h, 080h, 004h, 0E0h, 001h, 086h, 001h, 0E0h, 001h, 09Ch, 000h, 040h, 001h, 0E0h
        db  001h, 09Ch, 040h, 000h, 004h, 0CBh, 003h, 0B0h, 006h, 00Bh, 013h, 004h, 001h, 060h, 001h, 09Ch
        db  040h, 000h, 016h, 0F9h, 004h, 0E0h, 001h, 080h, 0E8h, 020h, 0E0h, 008h, 009h, 018h, 007h, 08Bh
        db  080h, 000h, 0C8h, 00Bh, 007h, 098h, 0C8h, 00Bh, 007h, 078h, 007h, 08Bh, 087h, 06Fh, 048h, 00Bh
        db  007h, 03Ah, 0C2h, 0E0h, 007h, 050h, 002h, 08Bh, 007h, 058h, 013h, 010h, 0E8h, 020h, 0E0h, 00Ah
        db  001h, 000h, 0C2h, 0E0h, 001h, 000h, 022h, 0E0h, 0E0h, 006h, 013h, 0F8h, 007h, 08Bh, 007h, 058h
        db  0C8h, 00Bh, 007h, 050h, 007h, 08Bh, 0C4h, 036h, 0C8h, 00Bh, 007h, 052h, 003h, 080h, 003h, 000h
        db  000h, 002h, 0C2h, 0E0h, 008h, 0CCh, 013h, 00Ch, 006h, 020h, 008h, 0CEh, 0C8h, 00Bh, 001h, 06Ch
        db  0C8h, 020h, 0FCh, 000h, 008h, 0CCh, 0CBh, 04Bh, 000h, 002h, 003h, 090h, 0FFh, 0FFh, 003h, 080h
        db  0C0h, 041h, 013h, 00Fh, 080h, 081h, 013h, 00Dh, 0A0h, 082h, 0C2h, 0E2h, 00Ch, 032h, 009h, 012h
        db  0C8h, 00Bh, 001h, 06Ch, 004h, 0E0h, 0FCh, 000h, 0C3h, 020h, 0FCh, 002h, 011h, 007h, 0C8h, 002h
        db  0FCh, 000h, 004h, 0EDh, 000h, 002h, 004h, 0E0h, 001h, 06Ch, 003h, 080h, 0CBh, 042h, 000h, 002h
        db  0C8h, 002h, 001h, 06Ch, 0C0h, 08Bh, 003h, 080h, 003h, 000h, 000h, 002h, 007h, 083h, 080h, 000h
        db  0C2h, 060h, 009h, 02Ch, 0C1h, 009h, 002h, 024h, 0FFh, 0F8h, 008h, 0A9h, 002h, 001h, 000h, 000h
        db  002h, 002h, 000h, 000h, 002h, 00Bh, 000h, 000h, 002h, 00Ch, 000h, 000h, 002h, 007h, 000h, 000h
        db  0CBh, 02Ch, 00Ch, 032h, 00Ch, 032h, 013h, 006h, 005h, 08Bh, 005h, 0CCh, 088h, 00Bh, 004h, 046h
        db  01Bh, 027h, 010h, 0F6h, 0C2h, 009h, 0C2h, 08Bh, 006h, 008h, 013h, 00Ah, 005h, 08Bh, 005h, 0CCh
        db  088h, 00Bh, 004h, 046h, 01Bh, 01Dh, 0CBh, 02Ch, 00Ch, 032h, 00Ch, 032h, 016h, 0EDh, 010h, 0F4h
        db  0C0h, 082h, 013h, 014h, 0C8h, 002h, 001h, 06Ch, 0C8h, 00Ah, 0FCh, 000h, 0C8h, 00Ah, 001h, 06Ch
        db  004h, 0E0h, 0FCh, 000h, 007h, 0A0h, 0FCh, 002h, 080h, 000h, 0C8h, 004h, 0FCh, 004h, 0C2h, 00Ah
        db  0A2h, 008h, 0CAh, 002h, 00Ch, 032h, 0C0h, 08Ah, 005h, 087h, 010h, 0D6h, 0C0h, 04Ah, 010h, 0EEh
        db  0CBh, 047h, 000h, 002h, 004h, 0E0h, 001h, 06Ch, 007h, 08Bh, 000h, 043h, 004h, 0E0h, 00Ch, 000h
        db  003h, 000h, 000h, 002h, 0C8h, 00Bh, 001h, 06Ch, 0C2h, 08Bh, 006h, 0A0h, 0C0h, 0F0h, 003h, 090h
        db  000h, 07Fh, 003h, 080h, 001h, 0A0h, 0FCh, 002h, 000h, 020h, 001h, 060h, 009h, 018h, 000h, 001h
        db  016h, 00Bh, 002h, 00Ah, 0FCh, 002h, 0A2h, 0A0h, 008h, 0DEh, 0CEh, 0A0h, 005h, 0EEh, 0C6h, 0A0h
        db  0FCh, 004h, 0C8h, 020h, 008h, 0DEh, 0FCh, 004h, 007h, 08Ah, 005h, 0F8h, 0C2h, 05Ah, 013h, 008h
        db  005h, 0CAh, 0C2h, 05Ah, 0C8h, 009h, 001h, 06Ch, 0C8h, 00Bh, 0FCh, 000h, 0C6h, 08Bh, 010h, 002h
        db  0CEh, 08Bh, 0C6h, 08Bh, 020h, 020h, 0E0h, 01Ah, 016h, 005h, 0E8h, 020h, 0E0h, 004h, 007h, 03Ah
        db  004h, 0E0h, 007h, 036h, 003h, 090h, 000h, 07Fh, 003h, 080h, 003h, 000h, 000h, 002h, 0C8h, 00Bh
        db  001h, 06Ch, 004h, 0CCh, 004h, 0E0h, 0FCh, 000h, 0C2h, 08Bh, 006h, 0A0h, 0C0h, 0FEh, 003h, 090h
        db  000h, 07Fh, 003h, 080h, 007h, 0A0h, 0FCh, 002h, 080h, 000h, 0C8h, 020h, 0E1h, 08Eh, 0FCh, 004h
        db  0C0h, 041h, 016h, 00Fh, 0D8h, 020h, 0E2h, 002h, 001h, 083h, 00Bh, 08Bh, 00Bh, 08Bh, 00Bh, 08Bh
        db  00Bh, 08Bh, 00Bh, 08Bh, 00Bh, 08Bh, 00Bh, 08Bh, 00Bh, 08Bh, 00Bh, 08Bh, 00Bh, 08Bh, 0C8h, 00Ah
        db  001h, 08Ah, 004h, 05Bh, 0C8h, 00Ah, 001h, 06Ch, 0C3h, 020h, 0FCh, 000h, 004h, 0E0h, 0FCh, 000h
        db  002h, 08Ah, 000h, 043h, 013h, 0DFh, 007h, 0A0h, 0FCh, 002h, 080h, 000h, 0C8h, 004h, 0FCh, 004h
        db  098h, 020h, 009h, 032h, 008h, 0CFh, 013h, 00Ah, 0C8h, 020h, 008h, 0CCh, 0FCh, 000h, 0C8h, 00Ah
        db  008h, 0CCh, 005h, 0A0h, 008h, 0CEh, 0C2h, 08Ch, 016h, 0E5h, 004h, 05Bh, 0C0h, 041h, 013h, 010h
        db  0A2h, 08Ah, 0CAh, 082h, 00Ch, 032h, 009h, 01Ah, 0C8h, 002h, 001h, 06Ch, 0C8h, 00Ah, 0FCh, 000h
        db  0C0h, 08Ah, 098h, 020h, 001h, 083h, 0E2h, 002h, 013h, 009h, 0C2h, 08Ch, 016h, 0D3h, 004h, 05Bh
        db  0C0h, 04Ah, 0C0h, 08Ah, 098h, 020h, 001h, 083h, 0E2h, 002h, 016h, 011h, 001h, 0E0h, 001h, 09Ch
        db  000h, 040h, 001h, 060h, 001h, 09Ch, 000h, 040h, 016h, 0FCh, 001h, 060h, 00Ch, 002h, 001h, 000h
        db  013h, 0ECh, 0D8h, 020h, 008h, 0E1h, 001h, 083h, 007h, 0A0h, 00Ch, 002h, 080h, 000h, 0C8h, 00Ah
        db  001h, 08Ah, 0C8h, 00Ah, 008h, 0CAh, 010h, 0E1h, 004h, 0D8h, 0C2h, 057h, 016h, 003h, 0CDh, 0C8h
        db  0C5h, 0C8h, 004h, 05Bh, 005h, 0C7h, 0C2h, 057h, 0C6h, 048h, 0C5h, 0C8h, 004h, 05Bh, 0C8h, 008h
        db  001h, 06Ch, 0A2h, 008h, 0CAh, 020h, 0FCh, 000h, 00Ch, 032h, 009h, 018h, 010h, 002h, 0C8h, 008h
        db  001h, 06Ch, 004h, 0E0h, 0FCh, 000h, 0C2h, 057h, 016h, 003h, 0CDh, 0C8h, 0C5h, 0C8h, 004h, 05Bh
        db  005h, 0C7h, 0C8h, 017h, 001h, 06Ch, 0C8h, 008h, 0FCh, 000h, 0C5h, 0C8h, 004h, 05Bh, 0C6h, 017h
        db  016h, 002h, 0C9h, 0C8h, 000h, 002h, 0C5h, 0C8h, 004h, 05Bh, 0C2h, 017h, 0C8h, 008h, 001h, 06Ch
        db  013h, 007h, 0C5h, 0E0h, 0FCh, 000h, 0A2h, 008h, 0C8h, 028h, 00Ch, 032h, 0FCh, 000h, 009h, 018h
        db  004h, 05Bh, 001h, 060h, 001h, 082h, 020h, 000h, 016h, 00Ah, 0C2h, 060h, 001h, 084h, 001h, 0A0h
        db  001h, 082h, 020h, 000h, 001h, 0E0h, 001h, 082h, 020h, 000h, 0C8h, 009h, 001h, 084h, 004h, 0C9h
        db  004h, 05Bh, 006h, 0A0h, 0C4h, 022h, 004h, 0D3h, 004h, 0E0h, 001h, 002h, 0E8h, 020h, 0E0h, 014h
        db  001h, 000h, 0C8h, 020h, 0E0h, 016h, 001h, 004h, 02Ch, 005h, 048h, 020h, 0E0h, 014h, 001h, 000h
        db  007h, 08Ch, 00Ah, 000h, 007h, 08Dh, 007h, 0D8h, 007h, 08Eh, 000h, 018h, 0CFh, 07Ch, 006h, 04Eh
        db  016h, 0FDh, 002h, 0E0h, 007h, 0D8h, 007h, 08Fh, 0FFh, 011h, 002h, 08Bh, 059h, 03Bh, 016h, 021h
        db  002h, 08Ah, 059h, 03Bh, 013h, 01Eh, 005h, 08Fh, 020h, 020h, 0E0h, 016h, 016h, 001h, 010h, 019h
        db  020h, 020h, 0E0h, 004h, 016h, 016h, 001h, 000h, 000h, 0BFh, 016h, 013h, 007h, 08Bh, 040h, 0C0h
        db  001h, 000h, 060h, 000h, 013h, 010h, 001h, 040h, 060h, 000h, 016h, 00Bh, 007h, 08Bh, 044h, 0C4h
        db  0C3h, 0A0h, 001h, 002h, 048h, 00Eh, 001h, 002h, 001h, 04Eh, 010h, 000h, 016h, 004h, 007h, 08Fh
        db  0FFh, 018h, 004h, 060h, 0C3h, 0F8h, 0C3h, 00Bh, 0C3h, 04Bh, 020h, 020h, 0E0h, 00Ah, 016h, 002h
        db  002h, 06Bh, 020h, 020h, 020h, 020h, 0E0h, 00Ch, 016h, 002h, 002h, 06Ch, 020h, 000h, 020h, 020h
        db  0E0h, 00Eh, 016h, 002h, 002h, 06Ch, 000h, 020h, 005h, 08Fh, 020h, 020h, 0E0h, 010h, 016h, 007h
        db  002h, 06Dh, 000h, 020h, 021h, 020h, 0E0h, 022h, 013h, 0E4h, 0C1h, 004h, 016h, 002h, 007h, 084h
        db  07Fh, 0FEh, 005h, 08Fh, 020h, 020h, 0E0h, 012h, 016h, 002h, 002h, 06Dh, 020h, 000h, 021h, 060h
        db  0E0h, 022h, 013h, 0D7h, 0C1h, 045h, 016h, 002h, 007h, 085h, 07Fh, 0FEh, 005h, 08Fh, 0D1h, 086h
        db  013h, 00Bh, 025h, 0A0h, 0E0h, 026h, 013h, 008h, 005h, 08Fh, 026h, 020h, 0E0h, 022h, 016h, 004h
        db  005h, 08Fh, 026h, 0A0h, 0E0h, 022h, 013h, 002h, 004h, 060h, 0C3h, 0F8h, 0D8h, 001h, 008h, 09Eh
        db  0D8h, 020h, 007h, 0DBh, 008h, 0B2h, 0D8h, 002h, 008h, 0A8h, 0D8h, 020h, 007h, 0DDh, 008h, 094h
        db  002h, 0E0h, 007h, 058h, 006h, 0A0h, 0C4h, 022h, 007h, 089h, 0E3h, 05Eh, 004h, 0E0h, 001h, 01Ah
        db  0C8h, 020h, 007h, 0E4h, 001h, 018h, 0C8h, 019h, 001h, 00Ch, 0C8h, 039h, 001h, 00Ah, 0C8h, 039h
        db  001h, 012h, 016h, 009h, 0C3h, 079h, 002h, 00Fh, 0E0h, 000h, 063h, 04Fh, 002h, 02Dh, 0A0h, 000h
        db  0C8h, 00Dh, 001h, 014h, 010h, 002h, 0C8h, 039h, 001h, 014h, 0C3h, 0F9h, 0C8h, 03Fh, 001h, 00Eh
        db  0C8h, 01Fh, 001h, 010h, 004h, 0E0h, 008h, 0C6h, 0C2h, 0B9h, 0D8h, 01Ah, 001h, 000h, 006h, 096h
        db  002h, 089h, 0E3h, 086h, 016h, 0E0h, 007h, 08Fh, 0FFh, 01Ch, 007h, 08Ch, 00Ah, 000h, 007h, 08Dh
        db  0E3h, 086h, 007h, 08Eh, 000h, 010h, 08Fh, 07Ch, 016h, 03Bh, 006h, 04Eh, 016h, 0FCh, 007h, 08Ch
        db  0E3h, 096h, 007h, 08Dh, 00Ah, 000h, 007h, 08Eh, 0E3h, 0A6h, 063h, 08Ch, 0CFh, 07Ch, 006h, 04Eh
        db  016h, 0FDh, 004h, 0E0h, 008h, 0E2h, 001h, 020h, 001h, 042h, 004h, 000h, 016h, 002h, 007h, 020h
        db  008h, 0E2h, 0C2h, 060h, 001h, 062h, 004h, 0E0h, 001h, 062h, 007h, 08Eh, 080h, 000h, 007h, 08Ch
        db  008h, 0E6h, 007h, 08Dh, 000h, 006h, 0DFh, 03Eh, 005h, 08Eh, 006h, 00Dh, 016h, 0FCh, 0D3h, 0FEh
        db  006h, 0CFh, 005h, 08Eh, 0D3h, 0FEh, 006h, 0CFh, 007h, 08Ch, 008h, 0E6h, 0C8h, 009h, 001h, 062h
        db  004h, 0C9h, 0A3h, 05Ch, 0E2h, 07Ch, 0A3h, 05Ch, 0E2h, 07Ch, 0A3h, 05Ch, 0E2h, 07Ch, 013h, 002h
        db  083h, 0CDh, 013h, 009h, 007h, 020h, 008h, 0E6h, 010h, 006h, 007h, 08Fh, 0FFh, 019h, 0A3h, 0CDh
        db  0C8h, 00Fh, 001h, 004h, 010h, 0FFh, 001h, 0A0h, 001h, 002h, 010h, 000h, 0C3h, 0E0h, 007h, 0EEh
        db  043h, 0E0h, 0E0h, 006h, 0D8h, 00Fh, 001h, 000h, 0C0h, 020h, 0E0h, 004h, 004h, 0E0h, 006h, 0FEh
        db  004h, 0D3h, 004h, 0E0h, 001h, 004h, 004h, 060h, 0C4h, 036h, 007h, 08Ch, 00Ah, 000h, 007h, 08Dh
        db  000h, 018h, 007h, 08Eh, 059h, 03Bh, 0CFh, 00Eh, 006h, 04Dh, 016h, 0FDh, 004h, 05Bh, 0C3h, 0D3h
        db  043h, 0C5h, 016h, 00Ch, 0C3h, 0E0h, 007h, 098h, 011h, 003h, 002h, 0E0h, 007h, 098h, 004h, 051h
        db  0C3h, 0E0h, 007h, 078h, 011h, 003h, 002h, 0E0h, 007h, 078h, 004h, 051h, 02Ch, 005h, 0A0h, 041h
        db  004h, 021h, 0E2h, 0C2h, 007h, 08Bh, 0C4h, 036h, 001h, 000h, 040h, 000h, 013h, 005h, 001h, 0C0h
        db  020h, 000h, 001h, 0DDh, 000h, 080h, 004h, 05Bh, 0C2h, 04Bh, 0D2h, 0A0h, 007h, 0EFh, 007h, 08Ch
        db  000h, 008h, 001h, 09Dh, 000h, 080h, 0C0h, 0BDh, 0C3h, 0A0h, 007h, 0EAh, 0C3h, 0E0h, 007h, 0ECh
        db  006h, 0A0h, 0C6h, 02Eh, 001h, 0C0h, 040h, 000h, 0D8h, 002h, 001h, 017h, 002h, 062h, 0FFh, 080h
        db  006h, 0A0h, 0C6h, 08Ah, 0C8h, 002h, 001h, 004h, 004h, 059h, 001h, 080h, 040h, 000h, 001h, 000h
        db  000h, 0FEh, 013h, 014h, 0C3h, 0C0h, 073h, 0CFh, 0D3h, 0EFh, 0E1h, 0C2h, 006h, 0CFh, 073h, 0CFh
        db  0C3h, 0AFh, 0EAh, 080h, 0C3h, 09Eh, 001h, 04Eh, 000h, 080h, 013h, 008h, 040h, 02Fh, 0E0h, 014h
        db  0A3h, 0CFh, 004h, 02Fh, 0E2h, 0F2h, 001h, 040h, 040h, 000h, 013h, 0B1h, 001h, 040h, 020h, 000h
        db  013h, 004h, 001h, 000h, 000h, 0FEh, 016h, 0E6h, 010h, 0AAh, 007h, 08Dh, 008h, 094h, 001h, 05Dh
        db  000h, 080h, 013h, 008h, 002h, 02Dh, 000h, 00Ah, 002h, 08Dh, 008h, 0C6h, 016h, 0F8h, 001h, 080h
        db  020h, 000h, 010h, 0EFh, 004h, 058h, 001h, 040h, 040h, 000h, 016h, 005h, 0E0h, 020h, 0E0h, 014h
        db  002h, 065h, 058h, 000h, 010h, 094h, 0D8h, 020h, 007h, 0DEh, 001h, 017h, 007h, 08Fh, 0FFh, 086h
        db  0C8h, 00Fh, 001h, 004h, 001h, 0C0h, 040h, 000h, 002h, 045h, 0A7h, 0FFh, 010h, 088h, 0C3h, 020h
        db  006h, 0FEh, 027h, 020h, 0E3h, 03Ah, 013h, 007h, 023h, 020h, 0E0h, 022h, 013h, 01Ah, 002h, 065h
        db  0DFh, 0FFh, 040h, 020h, 0E0h, 014h, 0E0h, 020h, 0E0h, 016h, 0C8h, 00Ch, 008h, 098h, 007h, 08Dh
        db  008h, 094h, 004h, 058h, 048h, 020h, 0E0h, 008h, 006h, 0FEh, 0C3h, 020h, 008h, 098h, 027h, 020h
        db  0E3h, 03Ah, 016h, 019h, 003h, 080h, 0C3h, 002h, 0C2h, 06Ch, 000h, 00Ah, 006h, 099h, 004h, 060h
        db  0C4h, 036h, 0D2h, 0A0h, 007h, 0EFh, 007h, 08Ch, 000h, 001h, 007h, 08Dh, 006h, 006h, 004h, 0CEh
        db  0C3h, 0E0h, 006h, 008h, 013h, 001h, 006h, 097h, 0D8h, 020h, 006h, 007h, 001h, 017h, 007h, 08Bh
        db  0FFh, 082h, 0C8h, 00Bh, 001h, 004h, 006h, 0A0h, 0CAh, 072h, 004h, 060h, 0C4h, 036h, 0C2h, 0A0h
        db  007h, 0EEh, 007h, 08Ch, 000h, 006h, 007h, 08Dh, 008h, 0A0h, 0C3h, 0A0h, 007h, 0E6h, 0C3h, 0E0h
        db  007h, 0E8h, 006h, 097h, 0D2h, 0A0h, 007h, 0EFh, 007h, 08Dh, 008h, 0A6h, 004h, 0DDh, 007h, 08Ch
        db  000h, 002h, 006h, 097h, 007h, 08Dh, 080h, 000h, 0C2h, 0A0h, 008h, 0A0h, 088h, 00Ah, 006h, 00Ch
        db  01Bh, 014h, 007h, 082h, 0C5h, 0FEh, 0C3h, 0A0h, 008h, 0A2h, 0C3h, 0E0h, 008h, 0A4h, 007h, 08Bh
        db  0E3h, 00Eh, 002h, 08Ah, 000h, 014h, 01Ah, 004h, 007h, 08Bh, 0EAh, 07Ch, 002h, 02Ah, 0FFh, 0ECh
        db  0A2h, 08Ah, 0A2h, 0CAh, 0C2h, 0DBh, 013h, 001h, 006h, 09Bh, 0C8h, 020h, 008h, 0A0h, 008h, 0A4h
        db  0C8h, 020h, 0E0h, 020h, 008h, 0A0h, 0C8h, 00Dh, 008h, 0A2h, 007h, 08Dh, 008h, 09Eh, 0E0h, 020h
        db  0E0h, 018h, 002h, 065h, 058h, 000h, 004h, 058h, 002h, 045h, 0A7h, 0FFh, 003h, 080h, 0C0h, 060h
        db  005h, 0EEh, 002h, 021h, 003h, 0E8h, 001h, 020h, 001h, 002h, 000h, 006h, 016h, 007h, 088h, 001h
        db  005h, 0EEh, 016h, 0F9h, 010h, 03Dh, 0D0h, 060h, 001h, 003h, 013h, 0F1h, 004h, 0E0h, 001h, 01Ah
        db  002h, 001h, 001h, 00Ah, 0CCh, 04Ch, 0CCh, 04Ch, 0CCh, 04Eh, 0CCh, 04Fh, 007h, 0B1h, 000h, 040h
        db  0CCh, 04Dh, 0D8h, 00Ah, 001h, 000h, 004h, 05Bh, 0C0h, 060h, 005h, 0EEh, 002h, 021h, 003h, 0E8h
        db  001h, 020h, 001h, 002h, 000h, 006h, 016h, 007h, 088h, 001h, 005h, 0EEh, 016h, 0F9h, 010h, 020h
        db  0D0h, 060h, 001h, 003h, 013h, 0F1h, 004h, 0E0h, 001h, 01Ah, 002h, 001h, 001h, 00Ah, 0CCh, 04Ch
        db  0CCh, 04Ch, 0CCh, 04Eh, 0CCh, 04Fh, 007h, 0B1h, 000h, 040h, 0CCh, 04Dh, 0D8h, 00Ah, 001h, 000h
        db  003h, 0B0h, 0D0h, 060h, 001h, 003h, 013h, 001h, 004h, 05Bh, 0C0h, 060h, 005h, 0EEh, 002h, 021h
        db  003h, 0E8h, 001h, 020h, 001h, 002h, 000h, 006h, 016h, 0F7h, 088h, 001h, 005h, 0EEh, 016h, 0F9h
        db  004h, 0CDh, 007h, 08Ah, 040h, 000h, 0C3h, 020h, 001h, 000h, 001h, 00Ch, 080h, 000h, 013h, 002h
        db  007h, 08Ah, 020h, 000h, 0C3h, 0A0h, 001h, 00Eh, 0C3h, 0E0h, 001h, 010h, 003h, 0B0h, 0C3h, 020h
        db  007h, 058h, 023h, 020h, 0E0h, 004h, 013h, 002h, 004h, 060h, 0C3h, 0F2h, 004h, 060h, 0B3h, 07Ch
        db  007h, 08Dh, 020h, 000h, 020h, 020h, 0E0h, 00Ah, 016h, 001h, 004h, 05Bh, 002h, 00Dh, 00Ch, 032h
        db  0C2h, 05Dh, 011h, 001h, 004h, 0DDh, 005h, 0CDh, 088h, 00Dh, 00Ch, 030h, 016h, 0F9h, 0C2h, 060h
        db  006h, 00Ah, 007h, 08Dh, 009h, 018h, 006h, 0A0h, 0CAh, 0BAh, 002h, 009h, 000h, 06Eh, 0C3h, 0E0h
        db  008h, 0E2h, 016h, 003h, 001h, 0E0h, 009h, 018h, 000h, 010h, 0C2h, 0E0h, 009h, 018h, 002h, 00Fh
        db  001h, 000h, 026h, 0C9h, 013h, 002h, 004h, 060h, 0C9h, 044h, 002h, 009h, 012h, 000h, 001h, 04Bh
        db  000h, 010h, 013h, 002h, 002h, 009h, 013h, 000h, 0D8h, 009h, 008h, 0E0h, 007h, 08Fh, 040h, 000h
        db  007h, 089h, 009h, 01Ah, 004h, 0CBh, 0E2h, 0F9h, 0E2h, 0F9h, 0E2h, 0F9h, 016h, 007h, 007h, 08Bh
        db  008h, 0E6h, 007h, 08Ch, 009h, 01Ah, 0CFh, 03Bh, 0CFh, 03Bh, 0C7h, 01Bh, 0C3h, 020h, 009h, 01Ah
        db  011h, 018h, 007h, 08Fh, 020h, 000h, 007h, 089h, 009h, 028h, 006h, 0A0h, 0C7h, 06Ch, 006h, 0A0h
        db  0C7h, 06Ch, 010h, 011h, 0C3h, 019h, 016h, 002h, 007h, 08Ch, 000h, 01Ah, 002h, 08Ch, 000h, 00Eh
        db  013h, 006h, 002h, 08Ch, 000h, 014h, 013h, 003h, 002h, 08Ch, 000h, 01Ah, 016h, 002h, 0CEh, 04Ch
        db  004h, 05Bh, 004h, 060h, 0C9h, 044h, 007h, 08Fh, 010h, 000h, 0C2h, 0D9h, 011h, 0FAh, 016h, 002h
        db  007h, 08Bh, 004h, 000h, 0C3h, 04Bh, 002h, 08Dh, 000h, 020h, 014h, 002h, 002h, 00Dh, 000h, 020h
        db  002h, 08Dh, 004h, 000h, 012h, 002h, 002h, 00Dh, 004h, 000h, 002h, 02Dh, 0FFh, 0F8h, 0C8h, 00Dh
        db  008h, 0DEh, 002h, 02Bh, 003h, 0FFh, 001h, 08Bh, 003h, 0FFh, 0CEh, 04Bh, 004h, 0CBh, 004h, 0E0h
        db  005h, 0F8h, 004h, 0E0h, 009h, 014h, 0C8h, 020h, 00Ch, 030h, 009h, 02Eh, 007h, 0A0h, 009h, 030h
        db  0DFh, 0FEh, 007h, 08Dh, 0DFh, 0FEh, 0C3h, 0E0h, 007h, 0D8h, 023h, 0E0h, 0E0h, 016h, 016h, 024h
        db  0C3h, 0E0h, 00Ch, 030h, 063h, 04Fh, 004h, 0FFh, 004h, 0FFh, 006h, 04Dh, 016h, 0FDh, 007h, 08Dh
        db  0DFh, 0FEh, 004h, 020h, 0EAh, 064h, 0C3h, 0A0h, 0EAh, 064h, 0C3h, 0EEh, 000h, 012h, 016h, 0C1h
        db  0C3h, 06Eh, 000h, 018h, 009h, 0ADh, 007h, 08Ch, 0E0h, 000h, 009h, 0ACh, 063h, 00Dh, 013h, 00Ch
        db  0C3h, 06Eh, 000h, 018h, 009h, 0ADh, 002h, 02Dh, 000h, 040h, 00Ah, 01Dh, 002h, 02Dh, 00Ch, 032h
        db  007h, 0BDh, 07Fh, 0FFh, 006h, 00Ch, 016h, 0FCh, 0C3h, 020h, 004h, 046h, 002h, 08Ch, 000h, 080h
        db  01Ah, 013h, 002h, 0ACh, 0C8h, 00Ch, 000h, 09Ah, 002h, 0E0h, 000h, 080h, 007h, 088h, 000h, 080h
        db  0C2h, 060h, 004h, 046h, 006h, 0A0h, 0F1h, 000h, 010h, 002h, 000h, 09Dh, 010h, 005h, 000h, 09Dh
        db  007h, 08Fh, 008h, 000h, 004h, 060h, 0C9h, 044h, 02Dh, 04Bh, 0C3h, 081h, 005h, 0C9h, 007h, 08Fh
        db  010h, 000h, 002h, 08Eh, 000h, 002h, 011h, 06Ah, 007h, 08Fh, 004h, 000h, 005h, 0C9h, 0C2h, 0D9h
        db  026h, 0E0h, 0E0h, 026h, 016h, 002h, 002h, 02Bh, 000h, 006h, 0CEh, 04Bh, 0C3h, 04Bh, 072h, 0CBh
        db  002h, 02Eh, 0FFh, 0FEh, 083h, 08Bh, 01Bh, 05Ah, 006h, 0CDh, 073h, 04Dh, 082h, 0CDh, 01Bh, 056h
        db  004h, 0E0h, 008h, 0CCh, 004h, 0E0h, 008h, 0CEh, 0C3h, 0B9h, 0C3h, 0D9h, 007h, 089h, 000h, 012h
        db  007h, 08Dh, 008h, 0ECh, 006h, 0A0h, 0CAh, 0BAh, 0C3h, 060h, 007h, 0D8h, 023h, 060h, 0E0h, 016h
        db  016h, 009h, 0E8h, 020h, 0E0h, 010h, 009h, 018h, 0E8h, 020h, 0E0h, 018h, 007h, 098h, 0E8h, 020h
        db  0E0h, 012h, 007h, 078h, 0C3h, 060h, 009h, 018h, 023h, 060h, 0E0h, 01Eh, 016h, 003h, 048h, 020h
        db  0E3h, 0A6h, 009h, 018h, 023h, 060h, 0E0h, 022h, 013h, 006h, 027h, 060h, 0E3h, 0A8h, 013h, 003h
        db  0E8h, 020h, 0E0h, 010h, 009h, 018h, 02Dh, 020h, 000h, 000h, 007h, 08Eh, 000h, 000h, 006h, 0A0h
        db  0CAh, 09Ah, 005h, 04Eh, 02Ch, 00Eh, 0C0h, 0A0h, 007h, 0FEh, 0C3h, 0EFh, 000h, 006h, 016h, 01Bh
        db  0C3h, 0A0h, 009h, 020h, 0C3h, 0E0h, 009h, 022h, 006h, 0A0h, 0C9h, 080h, 0C3h, 0A0h, 009h, 024h
        db  0C3h, 0E0h, 009h, 026h, 006h, 0A0h, 0C9h, 09Eh, 0E0h, 020h, 0E0h, 00Ah, 0C3h, 060h, 007h, 0D8h
        db  023h, 060h, 0E0h, 016h, 016h, 005h, 004h, 0E0h, 006h, 02Eh, 041h, 060h, 0E0h, 004h, 02Eh, 04Dh
        db  007h, 08Dh, 080h, 000h, 004h, 052h, 073h, 0CFh, 002h, 02Fh, 002h, 000h, 0C3h, 04Fh, 004h, 052h
        db  020h, 020h, 0E0h, 00Ah, 013h, 003h, 007h, 08Dh, 010h, 000h, 004h, 05Bh, 040h, 020h, 0E0h, 00Ah
        db  002h, 040h, 0F0h, 0FFh, 007h, 08Eh, 000h, 002h, 006h, 0A0h, 0CAh, 09Ah, 005h, 04Eh, 02Ch, 00Eh
        db  0C0h, 0A0h, 007h, 0FEh, 006h, 0A0h, 0CAh, 072h, 0C3h, 060h, 007h, 0D8h, 023h, 060h, 0E0h, 016h
        db  016h, 066h, 004h, 020h, 0EAh, 078h, 010h, 063h, 002h, 06Eh, 080h, 000h, 007h, 08Dh, 0C0h, 000h
        db  0C8h, 00Dh, 001h, 0A6h, 0C8h, 00Eh, 009h, 020h, 0C8h, 00Fh, 009h, 022h, 0C8h, 00Eh, 001h, 0A8h
        db  0C8h, 00Fh, 001h, 0AAh, 010h, 012h, 001h, 08Fh, 000h, 001h, 007h, 08Ah, 009h, 024h, 0E3h, 0A0h
        db  009h, 000h, 0CEh, 08Eh, 001h, 09Ah, 0FFh, 0FEh, 0E3h, 0E0h, 009h, 002h, 0E6h, 08Fh, 007h, 08Ah
        db  001h, 0ACh, 0CEh, 08Eh, 001h, 09Ah, 0FFh, 0FEh, 0E6h, 08Fh, 020h, 020h, 0E0h, 00Ah, 013h, 03Fh
        db  007h, 08Dh, 010h, 000h, 004h, 05Bh, 020h, 020h, 0E0h, 00Ah, 013h, 003h, 002h, 00Dh, 010h, 000h
        db  004h, 05Bh, 0C3h, 08Eh, 013h, 004h, 001h, 0E0h, 009h, 002h, 001h, 000h, 010h, 006h, 001h, 0A0h
        db  009h, 002h, 001h, 000h, 001h, 0A0h, 009h, 026h, 001h, 000h, 0C3h, 0A0h, 009h, 024h, 0C3h, 0E0h
        db  009h, 026h, 0E3h, 0A0h, 009h, 000h, 0E3h, 0E0h, 009h, 002h, 0C8h, 00Eh, 001h, 0ACh, 0C8h, 00Fh
        db  001h, 0AEh, 0C8h, 00Eh, 009h, 024h, 0C8h, 00Fh, 009h, 026h, 010h, 019h, 002h, 06Eh, 080h, 000h
        db  0C8h, 00Eh, 001h, 0A6h, 020h, 020h, 0E0h, 00Ah, 013h, 012h, 002h, 00Dh, 010h, 000h, 004h, 05Bh
        db  007h, 08Dh, 007h, 028h, 007h, 089h, 000h, 00Eh, 006h, 0A0h, 0CAh, 0C6h, 007h, 08Dh, 007h, 028h
        db  004h, 0FDh, 002h, 08Dh, 007h, 036h, 016h, 0FCh, 048h, 020h, 0E0h, 014h, 006h, 0FEh, 007h, 08Dh
        db  080h, 000h, 004h, 052h, 0C2h, 0A0h, 007h, 0EEh, 007h, 08Ch, 000h, 004h, 007h, 08Dh, 008h, 0A2h
        db  006h, 097h, 0C2h, 07Dh, 0C3h, 05Dh, 043h, 060h, 0E0h, 022h, 006h, 0A0h, 0CAh, 0C6h, 010h, 0EFh
        db  0C8h, 00Eh, 006h, 006h, 0C8h, 00Fh, 006h, 008h, 010h, 0EAh, 007h, 08Ch, 0E0h, 014h, 048h, 01Ch
        db  008h, 094h, 048h, 01Ch, 008h, 0A8h, 048h, 01Ch, 008h, 0B2h, 002h, 040h, 0C0h, 000h, 004h, 0E0h
        db  006h, 006h, 007h, 08Ch, 040h, 010h, 044h, 0CCh, 004h, 0E0h, 006h, 0FEh, 007h, 085h, 080h, 040h
        db  004h, 05Bh, 0C8h, 002h, 007h, 0FEh, 007h, 08Fh, 007h, 0F4h, 0CBh, 0CEh, 000h, 002h, 007h, 08Eh
        db  007h, 036h, 0CBh, 0CEh, 000h, 004h, 007h, 08Dh, 006h, 030h, 007h, 08Eh, 000h, 010h, 02Ch, 04Dh
        db  004h, 05Bh, 0C8h, 020h, 007h, 0E2h, 001h, 01Ah, 0C2h, 0A0h, 007h, 0F2h, 010h, 005h, 0C8h, 020h
        db  007h, 0E0h, 001h, 01Ah, 0D2h, 0A0h, 007h, 0F3h, 0C8h, 00Bh, 008h, 09Ch, 0C3h, 009h, 013h, 00Ah
        db  006h, 0A0h, 0C6h, 068h, 0C2h, 0A0h, 001h, 000h, 0E2h, 0A0h, 0E0h, 006h, 0A3h, 04Ch, 0A3h, 0CCh
        db  017h, 001h, 005h, 08Eh, 062h, 04Ch, 0C2h, 0E0h, 008h, 09Ch, 004h, 05Bh, 007h, 08Dh, 010h, 000h
        db  020h, 020h, 0E0h, 00Ah, 013h, 001h, 004h, 05Bh, 002h, 00Dh, 000h, 06Eh, 0C3h, 0E0h, 008h, 0E2h
        db  016h, 002h, 001h, 0CEh, 000h, 010h, 027h, 08Dh, 013h, 003h, 002h, 00Dh, 001h, 000h, 004h, 052h
        db  003h, 000h, 000h, 002h, 0C3h, 060h, 009h, 018h, 002h, 04Dh, 080h, 008h, 002h, 04Eh, 07Fh, 0F7h
        db  0E3h, 08Dh, 0C3h, 0E0h, 007h, 0D8h, 023h, 0E0h, 0E0h, 016h, 013h, 004h, 007h, 08Dh, 000h, 006h
        db  027h, 08Dh, 013h, 002h, 0E3h, 0A0h, 0E0h, 010h, 0C8h, 00Eh, 009h, 018h, 002h, 00Dh, 012h, 000h
        db  001h, 04Eh, 000h, 010h, 013h, 002h, 002h, 00Dh, 013h, 000h, 0D8h, 00Dh, 008h, 0E0h, 0C3h, 060h
        db  001h, 080h, 002h, 04Eh, 000h, 001h, 002h, 04Dh, 0FFh, 0FEh, 0E3h, 04Eh, 0C8h, 00Dh, 001h, 080h
        db  0D8h, 020h, 0E2h, 042h, 008h, 0E1h, 001h, 020h, 009h, 018h, 000h, 006h, 013h, 003h, 0D8h, 020h
        db  0E1h, 0D2h, 008h, 0E1h, 098h, 020h, 001h, 083h, 0E2h, 002h, 013h, 003h, 0D8h, 020h, 008h, 0E1h
        db  001h, 083h, 003h, 000h, 000h, 00Fh, 004h, 060h, 0CAh, 046h, 020h, 020h, 0E0h, 00Ah, 013h, 003h
        db  002h, 00Dh, 010h, 000h, 004h, 05Bh, 002h, 009h, 000h, 008h, 002h, 00Dh, 009h, 00Ah, 006h, 0A0h
        db  0CAh, 0BAh, 007h, 0A0h, 002h, 002h, 000h, 000h, 002h, 00Dh, 004h, 000h, 0C3h, 0E0h, 009h, 00Ah
        db  001h, 00Fh, 07Ch, 000h, 013h, 001h, 004h, 052h, 0C3h, 08Fh, 002h, 04Eh, 000h, 00Fh, 013h, 0FBh
        db  002h, 08Eh, 000h, 00Fh, 013h, 0F8h, 002h, 00Dh, 040h, 000h, 0C2h, 04Fh, 009h, 049h, 002h, 049h
        db  000h, 03Fh, 001h, 009h, 000h, 001h, 016h, 0EFh, 002h, 089h, 000h, 006h, 01Ah, 0ECh, 002h, 089h
        db  000h, 020h, 014h, 0E9h, 006h, 0C9h, 009h, 01Fh, 002h, 04Fh, 040h, 000h, 0E2h, 04Fh, 002h, 069h
        db  080h, 000h, 0C8h, 009h, 009h, 00Ah, 002h, 00Fh, 0FFh, 0FFh, 0C2h, 04Eh, 009h, 01Fh, 006h, 009h
        db  016h, 0FDh, 005h, 04Fh, 002h, 00Dh, 020h, 000h, 0C2h, 060h, 009h, 00Ch, 013h, 0D4h, 026h, 04Fh
        db  016h, 0D2h, 002h, 00Dh, 010h, 000h, 0C2h, 060h, 009h, 00Eh, 013h, 0CDh, 026h, 04Fh, 016h, 0CBh
        db  002h, 00Dh, 030h, 000h, 088h, 020h, 009h, 00Ch, 009h, 00Eh, 013h, 0C5h, 0C3h, 0E0h, 009h, 00Ch
        db  0C2h, 04Eh, 00Ah, 01Fh, 006h, 009h, 016h, 0FDh, 0E3h, 0E0h, 009h, 010h, 0C8h, 00Fh, 009h, 00Ch
        db  0C3h, 0E0h, 009h, 00Eh, 0C2h, 04Eh, 00Ah, 01Fh, 006h, 009h, 016h, 0FDh, 0E3h, 0E0h, 009h, 010h
        db  0C8h, 00Fh, 009h, 00Eh, 002h, 00Fh, 0FFh, 0FFh, 0C2h, 04Eh, 00Ah, 01Fh, 006h, 009h, 016h, 0FDh
        db  002h, 00Dh, 008h, 000h, 0C2h, 060h, 009h, 010h, 026h, 04Fh, 016h, 0A5h, 005h, 04Fh, 0C8h, 00Fh
        db  009h, 010h, 002h, 00Fh, 002h, 002h, 002h, 00Eh, 000h, 003h, 0C3h, 060h, 001h, 040h, 002h, 00Ch
        db  0CCh, 0C8h, 001h, 0A0h, 001h, 040h, 004h, 000h, 005h, 0CFh, 002h, 009h, 055h, 055h, 006h, 09Ch
        db  005h, 049h, 006h, 09Ch, 007h, 009h, 006h, 09Ch, 005h, 049h, 006h, 09Ch, 006h, 00Eh, 016h, 0F4h
        db  001h, 0A0h, 001h, 040h, 040h, 000h, 0C8h, 00Dh, 001h, 040h, 002h, 009h, 000h, 008h, 002h, 00Eh
        db  009h, 00Ah, 002h, 00Fh, 002h, 002h, 0CFh, 0FEh, 006h, 049h, 016h, 0FDh, 004h, 060h, 0CAh, 046h
        db  0C7h, 0C9h, 082h, 05Fh, 016h, 001h, 004h, 05Bh, 001h, 0A0h, 001h, 040h, 040h, 000h, 0C8h, 00Dh
        db  001h, 040h, 002h, 00Dh, 001h, 000h, 004h, 052h, 007h, 08Dh, 010h, 000h, 020h, 020h, 0E0h, 00Ah
        db  016h, 00Ah, 007h, 08Dh, 008h, 000h, 020h, 020h, 0E0h, 010h, 013h, 005h, 007h, 08Dh, 040h, 000h
        db  001h, 04Fh, 000h, 001h, 016h, 001h, 004h, 05Bh, 0E0h, 020h, 0E0h, 010h, 007h, 08Dh, 007h, 036h
        db  007h, 089h, 008h, 04Ah, 0CAh, 04Dh, 000h, 002h, 006h, 0A0h, 0CEh, 0E0h, 007h, 089h, 008h, 062h
        db  0CAh, 04Dh, 000h, 002h, 006h, 0A0h, 0CEh, 0E0h, 007h, 089h, 008h, 07Ah, 0CAh, 04Dh, 000h, 002h
        db  006h, 0A0h, 0CEh, 0E0h, 004h, 0E0h, 008h, 0B8h, 004h, 0E0h, 008h, 0BAh, 0C8h, 00Eh, 008h, 022h
        db  0C8h, 00Fh, 008h, 024h, 0C8h, 00Eh, 008h, 040h, 0C8h, 00Fh, 008h, 042h, 004h, 0E0h, 008h, 030h
        db  004h, 0E0h, 008h, 036h, 002h, 0E0h, 007h, 078h, 004h, 0E0h, 008h, 046h, 004h, 0E0h, 008h, 048h
        db  040h, 020h, 0E3h, 042h, 0E0h, 020h, 0E0h, 00Ch, 004h, 060h, 0D2h, 032h, 001h, 080h, 0F0h, 000h
        db  010h, 00Dh, 001h, 0C0h, 0F0h, 000h, 040h, 020h, 0E0h, 006h, 010h, 008h, 001h, 0C0h, 0F0h, 000h
        db  001h, 080h, 020h, 000h, 0C3h, 0E0h, 008h, 046h, 016h, 001h, 004h, 05Bh, 0C0h, 04Bh, 004h, 020h
        db  0EAh, 07Ch, 001h, 040h, 020h, 000h, 016h, 0FBh, 004h, 051h, 0C2h, 0A0h, 007h, 0D8h, 001h, 04Ah
        db  000h, 040h, 016h, 001h, 004h, 05Bh, 002h, 0E0h, 007h, 078h, 020h, 020h, 0E0h, 00Ch, 016h, 0EFh
        db  0C2h, 043h, 013h, 002h, 006h, 0A0h, 0CEh, 0CEh, 02Fh, 020h, 007h, 036h, 040h, 020h, 0E0h, 00Ch
        db  006h, 0A0h, 0CDh, 074h, 0C3h, 0A0h, 008h, 046h, 016h, 0FBh, 006h, 0A0h, 0CEh, 008h, 007h, 08Eh
        db  008h, 0B6h, 007h, 09Eh, 080h, 000h, 020h, 020h, 0E0h, 010h, 016h, 005h, 007h, 08Fh, 008h, 022h
        db  0CFh, 0BFh, 0CFh, 0BFh, 0C7h, 09Fh, 006h, 0A0h, 0CEh, 02Ch, 003h, 0B0h, 0E8h, 020h, 0E3h, 03Eh
        db  007h, 062h, 006h, 0A0h, 0CEh, 008h, 003h, 0B0h, 048h, 020h, 0E3h, 03Eh, 007h, 062h, 040h, 020h
        db  0E3h, 042h, 0E0h, 020h, 0E0h, 004h, 003h, 0B0h, 048h, 020h, 0E0h, 010h, 007h, 058h, 004h, 05Bh
        db  001h, 080h, 0F0h, 000h, 0E0h, 020h, 0E0h, 004h, 0C3h, 0E0h, 008h, 0B2h, 023h, 0E0h, 0E0h, 014h
        db  013h, 002h, 006h, 09Bh, 010h, 0B4h, 003h, 0B0h, 0E8h, 020h, 0E0h, 01Eh, 007h, 058h, 010h, 0AFh
        db  006h, 09Bh, 003h, 080h, 002h, 0E0h, 007h, 058h, 007h, 08Dh, 008h, 0B2h, 006h, 0A0h, 0C4h, 060h
        db  002h, 0E0h, 007h, 078h, 004h, 05Bh, 0C2h, 060h, 007h, 05Ch, 006h, 020h, 008h, 046h, 0C2h, 0A9h
        db  000h, 008h, 006h, 0A0h, 0CEh, 0CEh, 001h, 069h, 000h, 00Ah, 000h, 001h, 013h, 036h, 0C3h, 049h
        db  002h, 02Dh, 000h, 00Eh, 0C3h, 00Ah, 0D3h, 01Dh, 001h, 08Ch, 084h, 000h, 001h, 0CCh, 040h, 000h
        db  001h, 00Ah, 05Eh, 000h, 013h, 008h, 002h, 00Eh, 023h, 000h, 0DBh, 04Eh, 000h, 001h, 001h, 0CCh
        db  004h, 000h, 0D7h, 04Ch, 010h, 001h, 0C7h, 04Ch, 0C3h, 0A9h, 000h, 010h, 0C3h, 0E9h, 000h, 012h
        db  0CAh, 041h, 000h, 010h, 002h, 02Fh, 000h, 004h, 017h, 001h, 005h, 08Eh, 007h, 08Ch, 000h, 002h
        db  0D2h, 0A0h, 007h, 0F1h, 006h, 0A0h, 0C6h, 02Eh, 0C0h, 069h, 000h, 010h, 0C8h, 029h, 000h, 014h
        db  008h, 0B8h, 0C8h, 029h, 000h, 016h, 008h, 0BAh, 007h, 0A0h, 008h, 0B6h, 040h, 000h, 001h, 069h
        db  000h, 00Eh, 008h, 000h, 016h, 002h, 006h, 0A0h, 0CEh, 02Ch, 001h, 040h, 040h, 000h, 016h, 001h
        db  004h, 051h, 004h, 060h, 0CDh, 086h, 0C3h, 0A9h, 000h, 00Ch, 013h, 006h, 068h, 00Eh, 008h, 048h
        db  0C3h, 0A9h, 000h, 006h, 013h, 001h, 02Eh, 01Eh, 004h, 0E9h, 000h, 006h, 004h, 0E9h, 000h, 00Ch
        db  007h, 029h, 000h, 004h, 004h, 05Bh, 007h, 081h, 020h, 020h, 007h, 089h, 008h, 022h, 0CEh, 041h
        db  0CEh, 063h, 000h, 010h, 0C6h, 063h, 000h, 012h, 006h, 0A0h, 0C6h, 08Ah, 0C2h, 043h, 013h, 002h
        db  006h, 0A0h, 0CEh, 0CEh, 0E0h, 020h, 0E0h, 010h, 004h, 060h, 0CDh, 0B4h, 006h, 0A0h, 0C6h, 08Ah
        db  0C3h, 0A0h, 008h, 022h, 0C3h, 0E0h, 008h, 024h, 001h, 04Fh, 000h, 001h, 016h, 01Dh, 001h, 040h
        db  004h, 000h, 016h, 0E1h, 006h, 0A0h, 0CDh, 074h, 0C3h, 0A0h, 008h, 046h, 016h, 0FBh, 0C2h, 0A0h
        db  007h, 0F0h, 007h, 08Ch, 000h, 004h, 007h, 08Dh, 008h, 022h, 0C3h, 0A0h, 008h, 040h, 0C3h, 0E0h
        db  008h, 042h, 006h, 0A0h, 0C6h, 068h, 0C3h, 0A0h, 008h, 022h, 0C3h, 0E0h, 008h, 024h, 001h, 04Fh
        db  000h, 001h, 016h, 002h, 004h, 060h, 0CDh, 0B4h, 0C2h, 0A0h, 007h, 0F0h, 0C3h, 020h, 009h, 02Ah
        db  007h, 08Dh, 008h, 022h, 006h, 0A0h, 0C6h, 068h, 006h, 0C0h, 0D0h, 020h, 008h, 026h, 006h, 0C0h
        db  001h, 040h, 004h, 000h, 016h, 00Ah, 001h, 040h, 000h, 080h, 013h, 007h, 0C8h, 00Eh, 008h, 022h
        db  0C8h, 00Fh, 008h, 024h, 006h, 0A0h, 0CDh, 06Ah, 010h, 0DEh, 0C8h, 00Eh, 008h, 040h, 0C8h, 00Fh
        db  008h, 042h, 001h, 040h, 004h, 000h, 013h, 006h, 001h, 040h, 000h, 020h, 016h, 030h, 007h, 081h
        db  020h, 010h, 010h, 0A3h, 007h, 083h, 008h, 04Ah, 007h, 063h, 000h, 004h, 011h, 00Eh, 007h, 083h
        db  008h, 062h, 007h, 063h, 000h, 004h, 011h, 009h, 007h, 083h, 008h, 07Ah, 007h, 063h, 000h, 004h
        db  011h, 004h, 060h, 0C3h, 006h, 0A0h, 0CDh, 074h, 010h, 0EDh, 0C8h, 0E0h, 008h, 026h, 000h, 00Eh
        db  0C8h, 0E0h, 008h, 040h, 000h, 010h, 0C8h, 0E0h, 008h, 042h, 000h, 012h, 004h, 0C7h, 001h, 040h
        db  000h, 020h, 016h, 0DDh, 0C8h, 020h, 008h, 028h, 008h, 044h, 016h, 009h, 007h, 081h, 020h, 080h
        db  0C8h, 0E0h, 008h, 040h, 000h, 014h, 0C8h, 0E0h, 008h, 042h, 000h, 016h, 010h, 0D2h, 0C8h, 0E0h
        db  008h, 040h, 000h, 014h, 0C8h, 0E0h, 008h, 042h, 000h, 016h, 001h, 080h, 004h, 000h, 007h, 082h
        db  008h, 02Ah, 0C1h, 032h, 011h, 008h, 0C1h, 072h, 0C1h, 092h, 007h, 082h, 008h, 03Ch, 0C1h, 004h
        db  016h, 007h, 004h, 060h, 0D0h, 0E0h, 0C1h, 072h, 0C1h, 0B2h, 001h, 084h, 080h, 000h, 013h, 0F9h
        db  068h, 004h, 008h, 044h, 0C1h, 0C7h, 016h, 023h, 098h, 020h, 008h, 049h, 009h, 033h, 016h, 00Ah
        db  007h, 081h, 020h, 040h, 0C1h, 0E0h, 008h, 046h, 013h, 065h, 006h, 0A0h, 0CDh, 074h, 010h, 0F4h
        db  006h, 0A0h, 0CDh, 064h, 02Dh, 081h, 0C2h, 001h, 013h, 0FBh, 005h, 0A0h, 008h, 048h, 0C1h, 0C3h
        db  002h, 027h, 000h, 006h, 006h, 0A0h, 0C1h, 0A6h, 005h, 0A3h, 000h, 00Ch, 0C8h, 008h, 001h, 06Ch
        db  0C8h, 008h, 008h, 092h, 002h, 008h, 0FCh, 004h, 0C1h, 0F8h, 0C1h, 004h, 013h, 059h, 0D2h, 0E0h
        db  001h, 003h, 016h, 00Bh, 0C2h, 0E0h, 007h, 03Ah, 042h, 0E0h, 007h, 062h, 026h, 0E0h, 0E3h, 03Ch
        db  013h, 002h, 006h, 0A0h, 0CDh, 064h, 006h, 0A0h, 0C6h, 08Ah, 0C8h, 020h, 007h, 0E2h, 001h, 01Ah
        db  002h, 00Bh, 001h, 00Ah, 0CEh, 0C4h, 0CEh, 0C7h, 0CEh, 0C5h, 0CEh, 0C6h, 004h, 0FBh, 002h, 009h
        db  004h, 000h, 0A2h, 048h, 0C6h, 0C9h, 0A8h, 020h, 008h, 092h, 001h, 012h, 0D8h, 020h, 007h, 0F2h
        db  001h, 000h, 0C2h, 047h, 081h, 0C4h, 014h, 001h, 0C2h, 044h, 061h, 0C9h, 0A2h, 009h, 0A1h, 089h
        db  017h, 001h, 005h, 085h, 061h, 009h, 016h, 0AEh, 002h, 082h, 008h, 03Ch, 016h, 005h, 001h, 040h
        db  000h, 010h, 013h, 026h, 004h, 060h, 0CFh, 014h, 004h, 060h, 0D0h, 01Ah, 006h, 0C1h, 0D8h, 0C1h
        db  000h, 00Fh, 006h, 0A0h, 0CDh, 074h, 0E0h, 020h, 0E0h, 006h, 0C2h, 043h, 007h, 081h, 0D2h, 032h
        db  004h, 060h, 0CEh, 046h, 020h, 020h, 0E0h, 012h, 016h, 005h, 007h, 081h, 000h, 044h, 020h, 020h
        db  0E0h, 012h, 013h, 0ECh, 004h, 060h, 0CEh, 0F2h, 007h, 081h, 020h, 080h, 010h, 0F8h, 007h, 081h
        db  020h, 080h, 010h, 0F0h, 007h, 081h, 020h, 002h, 010h, 0F2h, 007h, 081h, 020h, 004h, 010h, 0EFh
        db  0C8h, 023h, 000h, 008h, 001h, 06Ch, 005h, 007h, 0A1h, 0E0h, 000h, 0E8h, 002h, 00Ch, 0FCh, 002h
        db  007h, 0BCh, 081h, 000h, 0CFh, 007h, 0D2h, 0E0h, 001h, 003h, 016h, 00Bh, 0C2h, 0E0h, 007h, 03Ah
        db  042h, 0E0h, 007h, 062h, 026h, 0E0h, 0E3h, 03Ch, 013h, 002h, 006h, 0A0h, 0CDh, 064h, 006h, 0A0h
        db  0C6h, 08Ah, 0C8h, 023h, 000h, 006h, 001h, 06Ch, 0C2h, 0A3h, 000h, 00Eh, 001h, 04Ah, 001h, 000h
        db  013h, 00Bh, 002h, 00Ch, 0FCh, 00Eh, 0C2h, 05Ch, 002h, 049h, 080h, 000h, 002h, 00Dh, 009h, 01Ah
        db  0E2h, 07Dh, 0CFh, 009h, 0CFh, 03Dh, 0CFh, 03Dh, 002h, 00Ch, 0FCh, 000h, 0C3h, 06Ch, 000h, 006h
        db  002h, 04Dh, 0E0h, 0FFh, 002h, 04Ah, 002h, 000h, 0A2h, 08Ah, 0A2h, 08Ah, 0E3h, 04Ah, 0CBh, 00Dh
        db  000h, 006h, 006h, 0CDh, 0C2h, 0E0h, 008h, 044h, 016h, 0B7h, 002h, 00Bh, 000h, 00Fh, 082h, 0ECh
        db  000h, 004h, 011h, 0B2h, 0C3h, 0ECh, 000h, 00Eh, 015h, 011h, 013h, 010h, 0C2h, 06Ch, 000h, 014h
        db  002h, 049h, 01Fh, 000h, 013h, 0ACh, 006h, 0C9h, 002h, 089h, 000h, 012h, 01Bh, 0A8h, 001h, 049h
        db  000h, 001h, 013h, 0A5h, 0A2h, 0C9h, 082h, 0ECh, 000h, 004h, 011h, 0A1h, 0A3h, 04Dh, 018h, 0A2h
        db  011h, 014h, 001h, 060h, 009h, 018h, 080h, 000h, 013h, 018h, 009h, 01Dh, 0A2h, 0CCh, 0C2h, 0EBh
        db  000h, 008h, 009h, 07Bh, 002h, 04Bh, 000h, 01Eh, 0C3h, 0A0h, 006h, 0F0h, 023h, 0ABh, 0E0h, 004h
        db  016h, 094h, 027h, 060h, 0E3h, 040h, 016h, 091h, 0A3h, 04Dh, 0A3h, 04Dh, 0A3h, 04Dh, 006h, 0CDh
        db  002h, 04Dh, 000h, 007h, 088h, 00Dh, 006h, 0EEh, 015h, 00Dh, 007h, 0A3h, 000h, 00Ah, 002h, 004h
        db  003h, 090h, 040h, 07Fh, 02Fh, 053h, 005h, 0A0h, 008h, 046h, 004h, 0C3h, 001h, 0C0h, 004h, 000h
        db  004h, 060h, 0CFh, 018h, 001h, 060h, 009h, 018h, 080h, 000h, 013h, 0EFh, 002h, 001h, 020h, 008h
        db  004h, 060h, 0D1h, 016h, 007h, 08Dh, 010h, 000h, 020h, 020h, 0E0h, 00Ah, 016h, 00Ah, 007h, 08Dh
        db  008h, 000h, 020h, 020h, 0E0h, 00Eh, 013h, 005h, 007h, 08Dh, 040h, 000h, 001h, 04Fh, 000h, 001h
        db  016h, 001h, 004h, 05Bh, 0E0h, 020h, 0E0h, 00Eh, 007h, 0A0h, 008h, 0ACh, 080h, 000h, 0C8h, 00Eh
        db  007h, 0F4h, 0C8h, 00Fh, 007h, 0F6h, 0C8h, 00Eh, 008h, 014h, 0C8h, 00Fh, 008h, 016h, 004h, 0E0h
        db  008h, 002h, 004h, 0E0h, 008h, 008h, 002h, 0E0h, 007h, 098h, 040h, 020h, 0E3h, 04Eh, 007h, 020h
        db  006h, 02Eh, 004h, 060h, 0D6h, 008h, 070h, 000h, 0C0h, 04Bh, 004h, 0E0h, 006h, 02Eh, 010h, 00Bh
        db  0F0h, 020h, 0E3h, 04Dh, 010h, 002h, 0F0h, 020h, 0E3h, 04Ch, 0C0h, 04Bh, 004h, 0E0h, 006h, 02Eh
        db  001h, 0E0h, 007h, 062h, 000h, 040h, 0E8h, 020h, 0E3h, 048h, 007h, 062h, 004h, 020h, 0EAh, 07Ch
        db  001h, 040h, 020h, 000h, 013h, 004h, 010h, 0FAh, 001h, 040h, 040h, 000h, 016h, 0F7h, 007h, 020h
        db  006h, 02Eh, 050h, 020h, 0E3h, 052h, 004h, 051h, 024h, 020h, 0E3h, 054h, 013h, 0EFh, 080h, 0E0h
        db  008h, 020h, 013h, 003h, 0C2h, 0E0h, 008h, 020h, 02Eh, 01Bh, 0C3h, 003h, 0C8h, 00Ch, 001h, 06Ch
        db  0C2h, 0E0h, 0FCh, 002h, 001h, 04Bh, 001h, 000h, 013h, 003h, 0C3h, 020h, 0FCh, 000h, 010h, 0F6h
        db  080h, 0CCh, 013h, 003h, 0C8h, 020h, 0FCh, 000h, 005h, 0F8h, 004h, 0E0h, 0FCh, 000h, 024h, 020h
        db  0E3h, 050h, 013h, 00Bh, 007h, 08Bh, 000h, 001h, 0C3h, 020h, 0EAh, 060h, 0CBh, 00Bh, 000h, 018h
        db  0CBh, 003h, 000h, 014h, 004h, 020h, 0EAh, 060h, 010h, 001h, 02Eh, 013h, 040h, 020h, 0E3h, 054h
        db  007h, 020h, 006h, 02Eh, 004h, 060h, 0D6h, 008h, 010h, 0C1h, 002h, 0E0h, 007h, 058h, 007h, 08Dh
        db  008h, 0A8h, 006h, 0A0h, 0C4h, 060h, 002h, 0E0h, 007h, 098h, 004h, 05Bh, 002h, 0E0h, 007h, 058h
        db  020h, 020h, 0E0h, 006h, 016h, 010h, 0E0h, 020h, 0E0h, 01Ch, 002h, 0E0h, 007h, 098h, 001h, 0C0h
        db  080h, 000h, 010h, 0A4h, 0C1h, 020h, 008h, 0A8h, 021h, 020h, 0E0h, 014h, 016h, 006h, 0E8h, 020h
        db  0E0h, 01Ch, 007h, 058h, 003h, 080h, 002h, 0E0h, 007h, 098h, 0D8h, 020h, 007h, 0DCh, 001h, 017h
        db  007h, 08Fh, 0FFh, 08Eh, 0C8h, 00Fh, 001h, 004h, 0E8h, 020h, 0E0h, 006h, 007h, 058h, 001h, 080h
        db  080h, 000h, 004h, 05Bh, 0C2h, 0E0h, 008h, 020h, 082h, 0C3h, 013h, 003h, 02Dh, 0DBh, 0C8h, 003h
        db  008h, 020h, 0C8h, 003h, 001h, 06Ch, 0C2h, 060h, 0FCh, 002h, 001h, 049h, 001h, 000h, 016h, 002h
        db  004h, 060h, 0D5h, 04Ch, 0C0h, 0E0h, 005h, 0F8h, 013h, 0FDh, 003h, 000h, 000h, 002h, 0C0h, 0E0h
        db  005h, 0F8h, 0C8h, 003h, 001h, 06Ch, 0C8h, 020h, 0FCh, 000h, 005h, 0F8h, 003h, 000h, 000h, 00Fh
        db  0C8h, 003h, 001h, 06Ch, 007h, 088h, 0FCh, 002h, 0C2h, 078h, 0C1h, 0F8h, 001h, 049h, 001h, 000h
        db  016h, 04Dh, 001h, 009h, 05Eh, 000h, 016h, 029h, 001h, 049h, 000h, 002h, 016h, 00Bh, 001h, 060h
        db  008h, 012h, 002h, 000h, 016h, 00Ah, 002h, 027h, 000h, 004h, 088h, 007h, 009h, 02Ch, 012h, 005h
        db  002h, 027h, 0FFh, 0FCh, 001h, 0A0h, 008h, 012h, 002h, 000h, 0C1h, 0C7h, 015h, 037h, 02Dh, 0D3h
        db  0C0h, 0E0h, 008h, 020h, 0A8h, 007h, 008h, 01Eh, 0A8h, 007h, 008h, 01Ch, 015h, 00Ch, 0C8h, 020h
        db  008h, 014h, 007h, 0F4h, 0C8h, 020h, 008h, 016h, 007h, 0F6h, 0C8h, 020h, 008h, 018h, 008h, 014h
        db  0C8h, 020h, 008h, 01Ah, 008h, 016h, 004h, 060h, 0D5h, 04Ch, 006h, 0A0h, 0C6h, 08Ah, 02Dh, 0D3h
        db  0C2h, 0E0h, 008h, 020h, 082h, 0C3h, 013h, 001h, 02Dh, 0DBh, 088h, 020h, 008h, 016h, 008h, 010h
        db  016h, 00Dh, 088h, 020h, 008h, 014h, 008h, 00Eh, 016h, 009h, 004h, 0E0h, 008h, 01Ch, 007h, 082h
        db  007h, 0FCh, 061h, 004h, 004h, 0E0h, 008h, 01Eh, 004h, 060h, 0D6h, 010h, 0C8h, 020h, 008h, 00Eh
        db  007h, 0F4h, 0C8h, 020h, 008h, 010h, 007h, 0F6h, 004h, 060h, 0D6h, 008h, 0A8h, 007h, 008h, 01Eh
        db  0C1h, 004h, 016h, 019h, 002h, 082h, 008h, 00Eh, 016h, 008h, 001h, 060h, 007h, 0F6h, 000h, 001h
        db  016h, 002h, 006h, 0A0h, 0D7h, 07Ah, 006h, 0A0h, 0D6h, 0FAh, 0C1h, 032h, 011h, 007h, 0C1h, 072h
        db  0C1h, 092h, 007h, 082h, 008h, 00Eh, 0C1h, 004h, 016h, 006h, 010h, 0ECh, 0C1h, 072h, 0C1h, 0B2h
        db  001h, 084h, 080h, 000h, 013h, 0E7h, 0D2h, 0E0h, 001h, 003h, 016h, 00Ah, 001h, 020h, 007h, 03Ah
        db  070h, 000h, 013h, 004h, 006h, 0A0h, 0D2h, 09Eh, 007h, 020h, 006h, 02Eh, 006h, 0A0h, 0C6h, 08Ah
        db  0C8h, 020h, 007h, 0E0h, 001h, 01Ah, 002h, 00Bh, 001h, 00Ah, 0CEh, 0C4h, 0CEh, 0C7h, 0CEh, 0C5h
        db  0CEh, 0C6h, 004h, 0FBh, 002h, 009h, 004h, 000h, 0A2h, 048h, 0C6h, 0C9h, 0A8h, 003h, 001h, 012h
        db  0D8h, 020h, 007h, 0F3h, 001h, 000h, 0C2h, 047h, 081h, 0C4h, 014h, 001h, 0C2h, 044h, 0A8h, 009h
        db  008h, 01Ch, 061h, 009h, 0A2h, 009h, 0A1h, 089h, 017h, 001h, 005h, 085h, 061h, 0C9h, 016h, 0B8h
        db  001h, 040h, 000h, 040h, 016h, 015h, 007h, 087h, 000h, 020h, 061h, 0E0h, 008h, 01Ch, 081h, 0C4h
        db  01Ah, 008h, 0A8h, 007h, 008h, 01Eh, 0A8h, 007h, 008h, 01Ch, 061h, 007h, 0A1h, 087h, 017h, 001h
        db  005h, 085h, 001h, 080h, 000h, 040h, 0C8h, 003h, 001h, 06Ch, 0C1h, 0E0h, 0FCh, 004h, 010h, 09Eh
        db  004h, 060h, 0D3h, 09Ch, 0D2h, 0E0h, 001h, 003h, 016h, 00Ah, 001h, 020h, 007h, 03Ah, 070h, 000h
        db  013h, 004h, 006h, 0A0h, 0D2h, 09Eh, 007h, 020h, 006h, 02Eh, 006h, 0A0h, 0C6h, 08Ah, 003h, 090h
        db  040h, 03Fh, 02Dh, 0D3h, 001h, 060h, 007h, 0F6h, 000h, 001h, 016h, 002h, 006h, 0A0h, 0D7h, 07Ah
        db  0D2h, 060h, 008h, 012h, 001h, 089h, 0F1h, 000h, 001h, 0C9h, 070h, 000h, 088h, 020h, 008h, 016h
        db  008h, 010h, 016h, 004h, 088h, 020h, 008h, 014h, 008h, 00Eh, 013h, 014h, 001h, 089h, 010h, 000h
        db  007h, 08Dh, 008h, 01Ch, 007h, 09Dh, 050h, 000h, 0D2h, 0A0h, 007h, 0F1h, 007h, 08Ch, 000h, 002h
        db  0C3h, 0A0h, 008h, 014h, 0C3h, 0E0h, 008h, 016h, 002h, 02Fh, 000h, 004h, 017h, 001h, 005h, 08Eh
        db  006h, 0A0h, 0C6h, 02Eh, 007h, 08Dh, 007h, 0F8h, 0CFh, 049h, 0C7h, 060h, 008h, 01Eh, 006h, 04Dh
        db  0D2h, 0A0h, 007h, 0F1h, 007h, 08Ch, 000h, 004h, 0C3h, 0A0h, 008h, 00Eh, 0C3h, 0E0h, 008h, 010h
        db  0A3h, 0CCh, 017h, 001h, 005h, 08Eh, 006h, 0A0h, 0C6h, 02Eh, 0C8h, 020h, 008h, 014h, 008h, 0AEh
        db  0C8h, 020h, 008h, 016h, 008h, 0B0h, 001h, 009h, 00Ch, 000h, 013h, 00Ah, 001h, 049h, 004h, 000h
        db  016h, 005h, 006h, 0A0h, 0D3h, 042h, 006h, 0A0h, 0D2h, 0AEh, 010h, 002h, 006h, 0A0h, 0D3h, 042h
        db  001h, 0C0h, 000h, 080h, 006h, 0A0h, 0D6h, 0FAh, 0C2h, 0E0h, 007h, 042h, 013h, 02Eh, 0C2h, 0E0h
        db  006h, 02Eh, 013h, 02Bh, 002h, 0E0h, 007h, 058h, 007h, 08Fh, 0FFh, 0BFh, 02Ch, 00Fh, 002h, 0E0h
        db  007h, 098h, 0C0h, 0E0h, 007h, 05Ch, 0C8h, 003h, 008h, 020h, 0C8h, 003h, 001h, 06Ch, 0C2h, 0C3h
        db  0A2h, 0CBh, 0C2h, 0EBh, 00Ch, 032h, 013h, 015h, 0C8h, 00Bh, 0FCh, 000h, 0C3h, 00Bh, 0C3h, 04Bh
        db  0C8h, 00Bh, 001h, 06Ch, 0C2h, 0E0h, 0FCh, 000h, 016h, 0FAh, 003h, 000h, 000h, 002h, 0C8h, 020h
        db  005h, 0F8h, 0FCh, 000h, 016h, 002h, 0C8h, 00Dh, 005h, 0FAh, 0C8h, 00Ch, 005h, 0F8h, 003h, 000h
        db  000h, 00Fh, 001h, 040h, 000h, 020h, 016h, 020h, 010h, 01Fh, 0C3h, 0A0h, 006h, 02Eh, 013h, 003h
        db  0C0h, 0E0h, 005h, 0F8h, 016h, 012h, 0C3h, 020h, 009h, 014h, 013h, 002h, 004h, 020h, 0EAh, 058h
        db  02Eh, 04Fh, 001h, 0C0h, 080h, 000h, 001h, 0A0h, 007h, 062h, 080h, 000h, 0C3h, 08Eh, 013h, 003h
        db  001h, 0A0h, 007h, 062h, 000h, 040h, 004h, 060h, 0D2h, 0C4h, 0C8h, 003h, 001h, 06Ch, 0C8h, 020h
        db  0FCh, 000h, 005h, 0F8h, 0C8h, 003h, 008h, 020h, 001h, 060h, 009h, 018h, 004h, 000h, 013h, 002h
        db  004h, 060h, 0D3h, 0D8h, 007h, 08Ch, 000h, 00Eh, 0C8h, 003h, 001h, 06Ch, 0C2h, 020h, 0FCh, 00Eh
        db  015h, 00Ah, 013h, 009h, 0C2h, 020h, 0FCh, 014h, 002h, 048h, 01Fh, 000h, 006h, 0C8h, 002h, 088h
        db  000h, 012h, 01Bh, 0EEh, 0A3h, 008h, 007h, 088h, 0FCh, 002h, 0C2h, 078h, 0C1h, 0F8h, 083h, 007h
        db  01Ah, 0E7h, 061h, 0CCh, 0C8h, 007h, 0FCh, 004h, 0C1h, 0CCh, 001h, 0C0h, 000h, 040h, 004h, 060h
        db  0D3h, 0E4h, 0C1h, 00Bh, 0C2h, 0A0h, 007h, 0F0h, 0C3h, 020h, 009h, 028h, 007h, 08Dh, 007h, 0F4h
        db  0C3h, 0A0h, 007h, 0F4h, 0C3h, 0E0h, 007h, 0F6h, 006h, 0A0h, 0C6h, 02Eh, 0C8h, 020h, 008h, 014h
        db  008h, 018h, 0C8h, 020h, 008h, 016h, 008h, 01Ah, 0C8h, 00Eh, 008h, 014h, 0C8h, 00Fh, 008h, 016h
        db  007h, 082h, 007h, 0FCh, 004h, 0E0h, 008h, 01Ch, 001h, 040h, 000h, 080h, 016h, 006h, 0C8h, 00Eh
        db  008h, 00Eh, 0C8h, 00Fh, 008h, 010h, 004h, 0E0h, 008h, 01Eh, 006h, 0A0h, 0C6h, 08Ah, 0C2h, 0E0h
        db  007h, 0F8h, 011h, 00Dh, 0C8h, 00Eh, 007h, 0F4h, 0C8h, 00Fh, 007h, 0F6h, 0C8h, 020h, 008h, 018h
        db  008h, 014h, 0C8h, 020h, 008h, 01Ah, 008h, 016h, 006h, 0A0h, 0D2h, 0A8h, 010h, 0CBh, 001h, 040h
        db  000h, 080h, 016h, 002h, 0C8h, 00Bh, 008h, 012h, 0C2h, 0C4h, 061h, 004h, 001h, 080h, 000h, 080h
        db  004h, 05Bh, 0C0h, 08Bh, 0C2h, 0A0h, 007h, 0F0h, 007h, 08Ch, 000h, 004h, 007h, 08Dh, 007h, 0F4h
        db  0C3h, 0A0h, 008h, 014h, 0C3h, 0E0h, 008h, 016h, 006h, 0A0h, 0C6h, 068h, 001h, 060h, 007h, 0F6h
        db  000h, 001h, 013h, 004h, 007h, 0A0h, 008h, 0ACh, 080h, 000h, 004h, 052h, 0C3h, 0E0h, 008h, 0A8h
        db  023h, 0E0h, 0E0h, 014h, 016h, 007h, 0D0h, 020h, 0E0h, 004h, 0E8h, 020h, 0E0h, 01Ah, 007h, 058h
        db  004h, 060h, 0D2h, 0B4h, 007h, 0A0h, 008h, 0ACh, 040h, 000h, 0C8h, 020h, 008h, 014h, 008h, 0AEh
        db  0C8h, 020h, 008h, 016h, 008h, 0B0h, 006h, 0A0h, 0D3h, 042h, 006h, 0A0h, 0D2h, 0AEh, 010h, 0D2h
        db  0C2h, 0ADh, 000h, 002h, 0C2h, 06Dh, 000h, 000h, 016h, 005h, 007h, 0AAh, 000h, 002h, 007h, 036h
        db  02Ch, 09Ah, 003h, 080h, 02Ch, 0EAh, 000h, 002h, 0CBh, 041h, 000h, 000h, 003h, 080h, 0C3h, 02Dh
        db  000h, 018h, 007h, 0ACh, 000h, 002h, 007h, 036h, 04Bh, 020h, 0EAh, 098h, 000h, 00Ah, 0EBh, 020h
        db  0EAh, 092h, 000h, 00Ah, 02Eh, 09Ch, 003h, 080h, 0C2h, 0A0h, 0E0h, 022h, 004h, 060h, 0B3h, 07Ch
        db  0C0h, 0EDh, 000h, 018h, 006h, 0A0h, 0D8h, 048h, 003h, 080h, 0C2h, 044h, 0C0h, 0C3h, 013h, 002h
        db  006h, 0A0h, 0D8h, 048h, 0C3h, 019h, 0CBh, 009h, 000h, 018h, 005h, 0C9h, 0CBh, 019h, 000h, 016h
        db  0C2h, 04Ch, 002h, 02Ch, 000h, 01Ah, 0CFh, 00Dh, 0CFh, 00Eh, 0C7h, 00Fh, 000h, 099h, 004h, 05Bh
        db  007h, 08Ch, 008h, 0BCh, 0C2h, 09Ch, 022h, 0A0h, 0E0h, 014h, 013h, 006h, 0C2h, 0A0h, 007h, 058h
        db  022h, 0A0h, 0E0h, 020h, 016h, 001h, 003h, 080h, 0C1h, 003h, 004h, 0C3h, 007h, 08Ah, 000h, 004h
        db  0A2h, 084h, 0CFh, 03Ah, 0CFh, 03Ah, 0CFh, 03Ah, 0CFh, 03Ah, 0CFh, 03Ah, 002h, 0E0h, 007h, 058h
        db  007h, 08Dh, 008h, 0BCh, 0C8h, 00Bh, 007h, 0C2h, 006h, 0A0h, 0C4h, 060h, 0C2h, 0E0h, 007h, 0C2h
        db  0E0h, 020h, 0E0h, 020h, 002h, 0E0h, 007h, 0B8h, 004h, 05Bh, 0C3h, 02Dh, 000h, 018h, 0C2h, 08Ch
        db  0C2h, 060h, 001h, 06Ch, 0C8h, 00Ah, 001h, 06Ch, 0C2h, 0E0h, 0FCh, 000h, 013h, 002h, 0C2h, 08Bh
        db  010h, 0F9h, 0C8h, 009h, 001h, 06Ch, 007h, 08Bh, 005h, 0F8h, 0C2h, 05Bh, 013h, 00Ch, 005h, 0CBh
        db  0C2h, 05Bh, 0C6h, 0CAh, 0C2h, 0E0h, 001h, 06Ch, 0C8h, 009h, 001h, 06Ch, 0C8h, 00Ch, 0FCh, 000h
        db  0C8h, 00Bh, 001h, 06Ch, 010h, 002h, 0CEh, 0CCh, 0C6h, 0CAh, 0C2h, 0A0h, 000h, 0E0h, 022h, 0A0h
        db  0E0h, 01Ah, 016h, 006h, 0E8h, 020h, 0E0h, 004h, 007h, 03Ah, 048h, 020h, 0E0h, 01Ah, 000h, 0E0h
        db  003h, 080h, 0D3h, 0E0h, 0E3h, 0ADh, 0C1h, 0E0h, 006h, 0A8h, 016h, 005h, 002h, 007h, 006h, 0A2h
        db  006h, 0A0h, 0C1h, 0D2h, 016h, 00Bh, 0C1h, 0E0h, 006h, 0BAh, 016h, 020h, 002h, 007h, 006h, 0B4h
        db  006h, 0A0h, 0C1h, 0D2h, 013h, 01Bh, 002h, 007h, 006h, 0B8h, 010h, 002h, 002h, 007h, 006h, 0A6h
        db  0C1h, 060h, 0FCh, 002h, 0C8h, 025h, 000h, 00Ch, 0FCh, 002h, 0C9h, 0C5h, 000h, 00Ch, 0CDh, 0F5h
        db  0CDh, 0F5h, 0CDh, 0F5h, 0CDh, 0F5h, 0CDh, 0F5h, 0C5h, 0F5h, 001h, 0B7h, 000h, 028h, 002h, 027h
        db  0FFh, 0F4h, 007h, 0A7h, 000h, 004h, 0DAh, 032h, 02Eh, 097h, 010h, 0D5h, 003h, 000h, 000h, 002h
        db  006h, 0A0h, 0C1h, 0EAh, 003h, 000h, 000h, 00Fh, 02Ch, 020h, 0EDh, 03Ah, 093h, 0E0h, 0E3h, 0ADh
        db  016h, 003h, 002h, 081h, 000h, 016h, 016h, 0C7h, 0C1h, 021h, 0EAh, 0A2h, 004h, 054h, 093h, 0E0h
        db  0E0h, 010h, 016h, 003h, 0D2h, 0A0h, 0E3h, 0AAh, 010h, 00Bh, 0D3h, 0CFh, 016h, 009h, 023h, 0A0h
        db  0E0h, 008h, 016h, 006h, 007h, 084h, 000h, 020h, 0E8h, 004h, 006h, 0D2h, 0D2h, 0A0h, 0E0h, 00Ch
        db  004h, 060h, 0DAh, 0AAh, 004h, 060h, 0DDh, 01Ah, 010h, 07Bh, 0C1h, 022h, 000h, 004h, 004h, 0E2h
        db  000h, 002h, 004h, 054h, 0C8h, 002h, 001h, 06Ch, 0A0h, 082h, 0C8h, 022h, 00Ch, 032h, 0FCh, 000h
        db  002h, 002h, 0FCh, 000h, 093h, 0E0h, 0E3h, 0ACh, 013h, 070h, 006h, 0A0h, 0DBh, 0ACh, 048h, 0A0h
        db  0E0h, 004h, 000h, 00Eh, 002h, 085h, 000h, 007h, 013h, 00Eh, 001h, 00Eh, 000h, 003h, 013h, 00Bh
        db  023h, 0A0h, 0E0h, 022h, 016h, 003h, 0D2h, 0A0h, 0E0h, 00Eh, 010h, 002h, 0D2h, 0A0h, 0E3h, 0AAh
        db  001h, 08Eh, 000h, 003h, 010h, 05Ah, 0C8h, 005h, 006h, 0FCh, 0C0h, 0C3h, 016h, 053h, 043h, 0A0h
        db  0E0h, 010h, 088h, 022h, 000h, 00Eh, 009h, 01Ah, 016h, 00Ah, 088h, 022h, 000h, 010h, 009h, 01Ch
        db  016h, 006h, 088h, 022h, 000h, 012h, 009h, 01Eh, 016h, 002h, 0E3h, 0A0h, 0E0h, 010h, 002h, 085h
        db  000h, 009h, 013h, 002h, 006h, 0A0h, 0E1h, 0B8h, 0A1h, 045h, 0C1h, 065h, 0E3h, 0AEh, 004h, 055h
        db  0C0h, 062h, 000h, 004h, 0C8h, 022h, 000h, 006h, 001h, 06Ch, 002h, 082h, 004h, 048h, 01Bh, 002h
        db  043h, 0A0h, 0E0h, 00Ch, 0C1h, 022h, 000h, 00Eh, 004h, 051h, 0C0h, 042h, 004h, 0E1h, 000h, 002h
        db  0C0h, 0A2h, 000h, 00Ch, 0C1h, 022h, 000h, 00Ah, 021h, 020h, 0E0h, 018h, 013h, 007h, 0C8h, 0A1h
        db  000h, 00Ah, 000h, 00Ah, 0C8h, 0A1h, 000h, 008h, 000h, 008h, 010h, 0E2h, 0C8h, 022h, 000h, 006h
        db  001h, 06Ch, 006h, 0A0h, 0E0h, 076h, 004h, 060h, 0DAh, 088h, 0C8h, 002h, 006h, 0D4h, 0C1h, 062h
        db  000h, 002h, 0C1h, 065h, 0E3h, 0D0h, 004h, 055h, 010h, 00Bh, 007h, 085h, 003h, 0F4h, 004h, 0F5h
        db  0CDh, 060h, 0EDh, 018h, 006h, 0A0h, 0E2h, 0A2h, 0E3h, 0A0h, 0E0h, 00Ch, 02Eh, 0A0h, 003h, 0F4h
        db  004h, 060h, 0D8h, 0EEh, 006h, 0A0h, 0DFh, 042h, 010h, 00Ch, 006h, 0A0h, 0E0h, 076h, 010h, 009h
        db  006h, 0A0h, 0E2h, 02Ah, 010h, 006h, 006h, 0A0h, 0E0h, 076h, 0C8h, 003h, 008h, 0DCh, 0D2h, 0A0h
        db  0E3h, 0ACh, 006h, 0A0h, 0DBh, 01Ch, 092h, 0A0h, 0E0h, 026h, 016h, 00Ch, 0D3h, 0E0h, 0E0h, 026h
        db  023h, 0E0h, 0E0h, 014h, 013h, 00Ah, 0C1h, 00Ah, 083h, 0C4h, 013h, 007h, 0C3h, 0C4h, 0C1h, 024h
        db  0E3h, 0D4h, 004h, 054h, 093h, 0CAh, 013h, 0DCh, 0D3h, 0CAh, 003h, 0B0h, 0D8h, 00Fh, 006h, 059h
        db  071h, 004h, 0C1h, 024h, 0E3h, 0E0h, 004h, 054h, 023h, 0A0h, 0E0h, 00Ch, 013h, 0D1h, 0C3h, 04Dh
        db  013h, 0CFh, 007h, 086h, 000h, 002h, 007h, 084h, 000h, 026h, 023h, 046h, 013h, 003h, 006h, 044h
        db  0A1h, 086h, 010h, 0FBh, 043h, 046h, 010h, 0B9h, 007h, 085h, 007h, 01Ch, 007h, 086h, 004h, 01Ah
        db  06Dh, 076h, 06Dh, 076h, 06Dh, 076h, 005h, 0C6h, 06Dh, 076h, 06Dh, 076h, 06Dh, 076h, 007h, 083h
        db  090h, 000h, 010h, 0BEh, 0C3h, 00Bh, 007h, 086h, 001h, 000h, 007h, 085h, 080h, 000h, 0C1h, 020h
        db  006h, 0D2h, 013h, 037h, 004h, 0C4h, 0C0h, 060h, 006h, 0D2h, 020h, 045h, 013h, 004h, 005h, 084h
        db  009h, 015h, 016h, 0F9h, 010h, 02Eh, 0D3h, 0CFh, 016h, 006h, 023h, 0E0h, 0E0h, 014h, 016h, 003h
        db  001h, 00Eh, 000h, 003h, 013h, 003h, 004h, 0E0h, 006h, 0D2h, 010h, 023h, 0D0h, 064h, 007h, 01Ch
        db  0B0h, 046h, 018h, 010h, 0D9h, 001h, 007h, 01Ch, 023h, 060h, 0E0h, 020h, 013h, 00Bh, 007h, 081h
        db  000h, 018h, 0C0h, 061h, 0E3h, 0F0h, 088h, 011h, 0EDh, 018h, 013h, 004h, 002h, 008h, 080h, 018h
        db  006h, 0A0h, 0DEh, 0F6h, 0D0h, 064h, 007h, 028h, 0B0h, 046h, 018h, 008h, 0D9h, 001h, 007h, 028h
        db  0B0h, 046h, 017h, 004h, 007h, 083h, 080h, 040h, 006h, 0A0h, 0E2h, 02Ah, 048h, 005h, 006h, 0D2h
        db  016h, 0CAh, 0C1h, 020h, 008h, 0E4h, 016h, 001h, 004h, 05Ch, 002h, 004h, 000h, 007h, 006h, 020h
        db  008h, 0E4h, 010h, 0C9h, 0C3h, 00Bh, 004h, 0C5h, 0C0h, 042h, 004h, 0C7h, 0C2h, 020h, 001h, 06Ch
        db  0A1h, 0E1h, 000h, 004h, 0C8h, 011h, 001h, 06Ch, 016h, 0FBh, 0C8h, 008h, 001h, 06Ch, 004h, 0C8h
        db  043h, 0A0h, 0E0h, 01Ah, 0C1h, 022h, 000h, 00Eh, 015h, 00Dh, 013h, 00Ch, 0E3h, 0A0h, 0E0h, 01Ah
        db  006h, 0A0h, 0E2h, 014h, 0C2h, 008h, 013h, 048h, 002h, 088h, 000h, 012h, 01Bh, 045h, 022h, 020h
        db  0E0h, 022h, 013h, 042h, 0C1h, 002h, 0A1h, 008h, 005h, 008h, 002h, 028h, 0FFh, 0F2h, 0A2h, 007h
        db  007h, 083h, 080h, 001h, 002h, 088h, 000h, 004h, 011h, 06Eh, 0C2h, 064h, 000h, 016h, 0D2h, 049h
        db  016h, 002h, 081h, 002h, 016h, 031h, 001h, 009h, 0F0h, 000h, 016h, 028h, 0C1h, 049h, 071h, 045h
        db  004h, 0C3h, 002h, 085h, 000h, 009h, 013h, 07Ch, 007h, 083h, 080h, 002h, 0C1h, 0A4h, 000h, 014h
        db  081h, 088h, 016h, 076h, 005h, 083h, 002h, 085h, 000h, 010h, 01Bh, 013h, 005h, 083h, 099h, 049h
        db  0EAh, 0C2h, 013h, 00Ah, 098h, 009h, 0E0h, 00Eh, 016h, 06Bh, 098h, 025h, 0EAh, 0C2h, 0E0h, 00Ch
        db  016h, 067h, 0C1h, 0E0h, 006h, 0ECh, 016h, 064h, 004h, 0C3h, 0C2h, 052h, 013h, 00Fh, 007h, 083h
        db  080h, 009h, 0C1h, 0E0h, 009h, 018h, 001h, 047h, 010h, 000h, 016h, 05Ah, 0C0h, 0A0h, 001h, 06Ch
        db  006h, 0A0h, 0E0h, 0CEh, 004h, 060h, 0DAh, 088h, 004h, 060h, 0DAh, 092h, 007h, 089h, 007h, 00Eh
        db  004h, 0C7h, 0D1h, 0E5h, 0EAh, 0D3h, 013h, 005h, 006h, 0C7h, 002h, 027h, 0EAh, 0E4h, 0CEh, 077h
        db  015h, 0FEh, 0C0h, 044h, 002h, 021h, 000h, 018h, 002h, 028h, 0FFh, 0FCh, 013h, 036h, 0C1h, 091h
        db  0D1h, 086h, 013h, 01Fh, 006h, 0C6h, 007h, 087h, 007h, 00Eh, 0C0h, 0F7h, 002h, 046h, 0BFh, 0FFh
        db  002h, 043h, 03Fh, 0FFh, 091h, 0A0h, 0EDh, 03Fh, 016h, 009h, 003h, 0B0h, 098h, 020h, 0E0h, 00Eh
        db  006h, 05Dh, 016h, 00Fh, 0C8h, 021h, 000h, 002h, 007h, 00Ch, 010h, 017h, 082h, 047h, 01Bh, 00Ch
        db  090h, 0C6h, 016h, 0EBh, 006h, 047h, 004h, 0F7h, 003h, 0B0h, 098h, 020h, 006h, 05Dh, 006h, 057h
        db  013h, 00Ch, 007h, 083h, 080h, 005h, 010h, 01Ch, 0C0h, 0D1h, 020h, 0E0h, 0E0h, 016h, 016h, 003h
        db  007h, 083h, 080h, 008h, 010h, 015h, 044h, 060h, 0E0h, 026h, 071h, 086h, 0A0h, 046h, 062h, 006h
        db  007h, 083h, 080h, 005h, 0C2h, 008h, 015h, 0CBh, 016h, 00Bh, 004h, 0C3h, 007h, 087h, 007h, 00Eh
        db  0C0h, 077h, 082h, 047h, 01Bh, 005h, 020h, 060h, 0E0h, 006h, 016h, 0FAh, 007h, 083h, 080h, 007h
        db  004h, 05Ch, 092h, 0A0h, 0E0h, 00Eh, 016h, 00Fh, 0C8h, 020h, 0E0h, 020h, 007h, 008h, 004h, 0E0h
        db  001h, 084h, 005h, 060h, 007h, 002h, 013h, 03Bh, 048h, 020h, 0E0h, 006h, 001h, 082h, 007h, 083h
        db  0C0h, 000h, 006h, 0A0h, 0E2h, 02Ah, 0C8h, 020h, 0E0h, 01Eh, 007h, 002h, 0E3h, 0A0h, 0E0h, 004h
        db  002h, 008h, 080h, 024h, 006h, 0A0h, 0DEh, 0F6h, 010h, 034h, 0C1h, 020h, 001h, 084h, 002h, 044h
        db  088h, 000h, 013h, 01Eh, 048h, 004h, 001h, 084h, 006h, 020h, 007h, 002h, 016h, 0F1h, 023h, 0A0h
        db  0E0h, 022h, 016h, 005h, 043h, 0A0h, 0E0h, 022h, 0D2h, 0A0h, 0E0h, 00Eh, 010h, 0D5h, 023h, 0E0h
        db  0E0h, 014h, 013h, 004h, 098h, 020h, 0E3h, 0ABh, 006h, 065h, 016h, 006h, 092h, 0A0h, 0E0h, 00Eh
        db  013h, 0CBh, 0D2h, 0A0h, 0E3h, 0AAh, 010h, 0D7h, 007h, 083h, 000h, 002h, 004h, 060h, 0DAh, 0A2h
        db  0E8h, 020h, 0E0h, 006h, 001h, 082h, 007h, 020h, 007h, 002h, 043h, 0A0h, 0E0h, 004h, 0C8h, 020h
        db  0E4h, 070h, 001h, 086h, 088h, 020h, 0E0h, 020h, 007h, 008h, 016h, 003h, 0C8h, 020h, 0EAh, 0FCh
        db  007h, 008h, 004h, 060h, 0DAh, 0AAh, 001h, 00Eh, 000h, 003h, 013h, 016h, 0D3h, 0CFh, 016h, 008h
        db  023h, 0A0h, 0E0h, 020h, 016h, 003h, 0D2h, 0A0h, 0E3h, 0AAh, 010h, 002h, 0D2h, 0A0h, 0E0h, 00Eh
        db  001h, 08Eh, 000h, 003h, 010h, 009h, 0C1h, 060h, 001h, 084h, 021h, 060h, 0E0h, 00Ah, 016h, 004h
        db  007h, 083h, 084h, 000h, 004h, 060h, 0DAh, 0A2h, 0C8h, 020h, 0E0h, 02Eh, 001h, 084h, 002h, 008h
        db  080h, 006h, 006h, 0A0h, 0DEh, 0F6h, 004h, 060h, 0DAh, 0AAh, 0E3h, 060h, 0E0h, 020h, 004h, 060h
        db  0DAh, 0AAh, 093h, 0E0h, 0E0h, 026h, 016h, 010h, 023h, 0A0h, 0E0h, 008h, 016h, 00Dh, 023h, 0A0h
        db  0E0h, 006h, 013h, 002h, 0E3h, 060h, 0E0h, 01Ch, 0E3h, 060h, 0E0h, 018h, 043h, 0A0h, 0E0h, 006h
        db  002h, 008h, 080h, 03Ch, 006h, 0A0h, 0DEh, 0F6h, 004h, 060h, 0DAh, 0AAh, 092h, 0A0h, 0E3h, 0AAh
        db  013h, 003h, 092h, 0A0h, 0E3h, 0ABh, 016h, 01Eh, 023h, 0E0h, 0E0h, 014h, 013h, 008h, 098h, 020h
        db  0E3h, 0ABh, 006h, 065h, 013h, 004h, 007h, 083h, 000h, 007h, 004h, 060h, 0DAh, 0A2h, 0D2h, 0A0h
        db  0E0h, 00Eh, 0C8h, 020h, 0E0h, 020h, 007h, 008h, 027h, 0A0h, 0E0h, 004h, 016h, 00Bh, 0C8h, 020h
        db  0E0h, 01Eh, 007h, 008h, 093h, 0E0h, 0E3h, 0AAh, 016h, 005h, 023h, 0A0h, 0E0h, 012h, 013h, 002h
        db  006h, 020h, 007h, 008h, 004h, 060h, 0DAh, 0AAh, 023h, 0E0h, 0E0h, 014h, 013h, 036h, 003h, 0B0h
        db  098h, 020h, 0E0h, 00Eh, 006h, 06Fh, 016h, 00Fh, 0D3h, 0CFh, 016h, 02Fh, 0D2h, 0A0h, 0E3h, 0AAh
        db  004h, 060h, 0DAh, 0AAh, 092h, 0A0h, 0E0h, 00Ch, 016h, 028h, 023h, 0E0h, 0E0h, 014h, 013h, 0F6h
        db  007h, 083h, 000h, 006h, 010h, 007h, 007h, 083h, 000h, 005h, 093h, 0E0h, 0E0h, 00Eh, 016h, 002h
        db  007h, 083h, 000h, 007h, 004h, 060h, 0DAh, 0A2h, 0E3h, 060h, 0E0h, 012h, 023h, 0E0h, 0E0h, 014h
        db  013h, 00Dh, 098h, 020h, 0E0h, 00Ch, 006h, 065h, 016h, 003h, 0D8h, 020h, 0E3h, 0ABh, 006h, 065h
        db  010h, 00Ch, 0C1h, 020h, 001h, 084h, 021h, 020h, 0E0h, 006h, 016h, 0D6h, 048h, 020h, 0E0h, 006h
        db  001h, 084h, 002h, 008h, 080h, 030h, 006h, 0A0h, 0DEh, 0F6h, 004h, 060h, 0DAh, 0AAh, 0E3h, 0A0h
        db  0E0h, 00Ah, 0C2h, 008h, 011h, 002h, 043h, 0A0h, 0E0h, 00Ah, 042h, 020h, 0E0h, 004h, 002h, 028h
        db  0E3h, 0F0h, 0C0h, 058h, 0C0h, 002h, 088h, 011h, 0EDh, 018h, 016h, 003h, 02Ch, 0D1h, 0C0h, 058h
        db  004h, 0D1h, 0C0h, 080h, 001h, 00Eh, 010h, 000h, 013h, 00Fh, 0CCh, 060h, 0EDh, 018h, 005h, 0C8h
        db  0CCh, 078h, 016h, 003h, 006h, 041h, 0CCh, 060h, 006h, 0D6h, 0C4h, 058h, 016h, 002h, 0C4h, 060h
        db  007h, 000h, 002h, 021h, 0FFh, 0FAh, 02Ch, 091h, 004h, 05Bh, 0C3h, 00Bh, 006h, 0A0h, 0DFh, 0DEh
        db  006h, 0A0h, 0DFh, 0B8h, 0C2h, 008h, 016h, 005h, 0C2h, 062h, 000h, 002h, 026h, 060h, 0E4h, 06Ah
        db  016h, 00Dh, 0C2h, 042h, 005h, 0C9h, 0CEh, 060h, 0EDh, 03Ch, 0C6h, 060h, 0EAh, 0FEh, 006h, 0A0h
        db  0E0h, 020h, 0CAh, 018h, 000h, 00Ah, 046h, 020h, 0E0h, 026h, 016h, 004h, 0C0h, 0A0h, 001h, 06Ch
        db  02Eh, 012h, 010h, 01Dh, 0C1h, 012h, 013h, 005h, 0C1h, 060h, 001h, 06Ch, 02Eh, 014h, 0C8h, 005h
        db  001h, 06Ch, 004h, 0D2h, 006h, 048h, 007h, 084h, 000h, 002h, 0C1h, 048h, 0C0h, 0A0h, 001h, 06Ch
        db  0C0h, 002h, 006h, 0A0h, 0E2h, 0A2h, 0C5h, 060h, 0FCh, 002h, 002h, 007h, 006h, 0A2h, 002h, 025h
        db  0FFh, 0F4h, 0C8h, 005h, 0FCh, 002h, 0C2h, 020h, 001h, 06Ch, 006h, 0A0h, 0C1h, 096h, 004h, 05Ch
        db  0C2h, 042h, 002h, 029h, 000h, 008h, 0C2h, 039h, 002h, 048h, 0C0h, 000h, 002h, 088h, 0C0h, 000h
        db  016h, 008h, 08Eh, 060h, 0E0h, 02Eh, 016h, 005h, 086h, 060h, 0E0h, 02Eh, 016h, 002h, 004h, 0C8h
        db  004h, 05Bh, 007h, 008h, 004h, 05Bh, 088h, 020h, 0E1h, 090h, 001h, 06Ch, 016h, 002h, 004h, 060h
        db  0DAh, 092h, 004h, 05Bh, 007h, 088h, 001h, 0AEh, 0E8h, 020h, 0E0h, 00Eh, 001h, 080h, 006h, 008h
        db  016h, 0FEh, 048h, 020h, 0E0h, 00Eh, 001h, 080h, 004h, 05Bh, 004h, 0C2h, 023h, 0A0h, 0E0h, 00Ch
        db  016h, 00Ah, 02Fh, 020h, 006h, 030h, 007h, 082h, 0FFh, 0DFh, 02Ch, 002h, 002h, 082h, 003h, 0F4h
        db  016h, 0FAh, 043h, 0A0h, 0E0h, 00Ch, 004h, 05Bh, 0C2h, 042h, 007h, 088h, 000h, 00Eh, 0A2h, 009h
        db  002h, 029h, 000h, 008h, 0CEh, 078h, 0CEh, 078h, 0CEh, 078h, 0CEh, 060h, 009h, 01Ah, 0CEh, 060h
        db  009h, 01Ch, 0CEh, 060h, 009h, 01Eh, 023h, 0A0h, 0E0h, 01Ah, 016h, 00Fh, 0C2h, 058h, 002h, 049h
        db  01Fh, 080h, 02Ah, 060h, 0E0h, 014h, 0E8h, 0A0h, 0E0h, 004h, 000h, 00Eh, 0C6h, 009h, 002h, 049h
        db  01Fh, 000h, 006h, 0C9h, 0A2h, 009h, 0A8h, 089h, 000h, 004h, 002h, 028h, 000h, 002h, 0C2h, 058h
        db  00Ah, 049h, 002h, 049h, 0F0h, 000h, 0D6h, 009h, 004h, 0E2h, 000h, 006h, 004h, 05Bh, 007h, 000h
        db  0C0h, 082h, 013h, 053h, 0C0h, 0A0h, 001h, 06Ch, 0C1h, 0A0h, 0FCh, 006h, 002h, 046h, 000h, 00Fh
        db  002h, 086h, 000h, 001h, 012h, 03Dh, 088h, 006h, 006h, 0F2h, 016h, 012h, 002h, 001h, 0FCh, 00Eh
        db  088h, 031h, 006h, 0F4h, 016h, 00Dh, 088h, 031h, 006h, 0F6h, 016h, 00Ah, 088h, 031h, 006h, 0F8h
        db  016h, 007h, 002h, 086h, 000h, 002h, 016h, 02Ch, 088h, 020h, 007h, 00Ah, 006h, 0FAh, 013h, 028h
        db  0C1h, 020h, 009h, 018h, 001h, 044h, 008h, 000h, 013h, 006h, 002h, 086h, 000h, 002h, 016h, 020h
        db  001h, 044h, 000h, 080h, 016h, 01Dh, 007h, 000h, 023h, 0E0h, 0E0h, 014h, 016h, 019h, 002h, 082h
        db  000h, 043h, 013h, 016h, 002h, 000h, 0FCh, 002h, 0C0h, 040h, 001h, 0B0h, 000h, 020h, 001h, 060h
        db  009h, 018h, 000h, 001h, 016h, 007h, 0A0h, 060h, 008h, 0DEh, 0CCh, 060h, 005h, 0EEh, 0C4h, 050h
        db  0C4h, 020h, 008h, 0DEh, 007h, 080h, 007h, 036h, 007h, 081h, 000h, 040h, 02Ch, 040h, 004h, 0C0h
        db  007h, 084h, 006h, 0F2h, 0CDh, 006h, 002h, 001h, 0FCh, 00Eh, 0CDh, 031h, 0CDh, 031h, 0CDh, 031h
        db  0C5h, 020h, 007h, 00Ah, 0C0h, 000h, 013h, 001h, 02Eh, 012h, 004h, 0E0h, 001h, 06Ch, 004h, 05Bh
        db  0C8h, 00Bh, 008h, 0D4h, 006h, 0A0h, 0E1h, 046h, 002h, 008h, 080h, 042h, 006h, 0A0h, 0DEh, 0F6h
        db  002h, 008h, 080h, 030h, 006h, 0A0h, 0DEh, 0F6h, 0C2h, 0E0h, 008h, 0D4h, 004h, 05Bh, 0C3h, 00Bh
        db  048h, 020h, 0EBh, 000h, 001h, 082h, 048h, 020h, 0E4h, 06Eh, 001h, 080h, 048h, 020h, 0E0h, 022h
        db  001h, 0AEh, 048h, 020h, 0E0h, 022h, 009h, 026h, 043h, 060h, 0E0h, 018h, 043h, 0A0h, 0E0h, 008h
        db  002h, 008h, 000h, 042h, 006h, 0A0h, 0DEh, 0F6h, 004h, 05Ch, 0C3h, 00Bh, 0E8h, 020h, 0E0h, 00Eh
        db  001h, 082h, 0E8h, 020h, 0E0h, 022h, 001h, 0AEh, 0E8h, 020h, 0E0h, 022h, 009h, 026h, 0E3h, 0A0h
        db  0E0h, 008h, 0E3h, 060h, 0E0h, 018h, 043h, 0A0h, 0E0h, 006h, 002h, 008h, 080h, 03Ch, 006h, 0A0h
        db  0DEh, 0F6h, 002h, 008h, 080h, 042h, 006h, 0A0h, 0DEh, 0F6h, 004h, 05Ch, 0C3h, 00Bh, 007h, 083h
        db  068h, 000h, 006h, 0A0h, 0E2h, 02Ah, 007h, 083h, 080h, 010h, 006h, 0A0h, 0E2h, 02Ah, 004h, 05Ch
        db  0C3h, 00Bh, 006h, 0A0h, 0E2h, 014h, 0A2h, 002h, 0C2h, 068h, 000h, 014h, 002h, 029h, 0FFh, 0FCh
        db  013h, 024h, 002h, 028h, 000h, 018h, 007h, 087h, 000h, 00Eh, 007h, 081h, 007h, 00Eh, 004h, 0F1h
        db  006h, 047h, 015h, 0FDh, 0C0h, 058h, 003h, 0B0h, 078h, 001h, 006h, 063h, 002h, 041h, 000h, 03Fh
        db  013h, 00Eh, 002h, 081h, 000h, 01Fh, 01Bh, 00Bh, 0A0h, 041h, 0C0h, 061h, 0E4h, 050h, 0C1h, 0F8h
        db  006h, 0C7h, 071h, 0C7h, 006h, 047h, 0CCh, 078h, 006h, 047h, 015h, 0FDh, 010h, 004h, 0C0h, 058h
        db  006h, 0C1h, 070h, 041h, 0A2h, 001h, 0C2h, 049h, 015h, 0E5h, 004h, 05Ch, 023h, 0A0h, 0E0h, 01Ah
        db  013h, 002h, 004h, 0C8h, 004h, 05Bh, 0C2h, 022h, 000h, 014h, 002h, 048h, 01Fh, 000h, 006h, 0C8h
        db  004h, 05Bh, 002h, 083h, 000h, 00Fh, 01Bh, 017h, 0C1h, 0A0h, 006h, 0D4h, 013h, 035h, 002h, 026h
        db  000h, 004h, 0CDh, 0A0h, 0EDh, 018h, 0C5h, 083h, 013h, 004h, 0C2h, 04Ah, 00Ah, 039h, 0E0h, 0C9h
        db  0C5h, 083h, 007h, 086h, 007h, 036h, 007h, 087h, 000h, 010h, 0C2h, 020h, 006h, 0D4h, 004h, 0E0h
        db  006h, 0D4h, 02Ch, 046h, 004h, 05Bh, 0C0h, 060h, 006h, 0FEh, 0C2h, 020h, 009h, 018h, 002h, 048h
        db  060h, 000h, 022h, 020h, 0E0h, 006h, 016h, 004h, 0E2h, 020h, 0E0h, 00Ah, 0E2h, 020h, 0E0h, 018h
        db  00Ah, 013h, 018h, 004h, 005h, 041h, 048h, 003h, 006h, 0FEh, 010h, 006h, 002h, 083h, 000h, 002h
        db  016h, 001h, 009h, 013h, 0E8h, 003h, 006h, 0FEh, 040h, 0C8h, 040h, 0C1h, 013h, 005h, 007h, 088h
        db  007h, 036h, 007h, 089h, 040h, 000h, 02Ch, 048h, 004h, 05Bh, 004h, 0C9h, 0C1h, 024h, 0EBh, 016h
        db  0C1h, 084h, 071h, 086h, 002h, 026h, 0EBh, 0C8h, 006h, 0C4h, 071h, 004h, 002h, 024h, 0EBh, 03Eh
        db  0D2h, 014h, 009h, 0C8h, 0A2h, 008h, 003h, 0B0h, 0D8h, 034h, 006h, 05Fh, 002h, 047h, 000h, 00Fh
        db  0A1h, 0C7h, 0C2h, 028h, 0EBh, 004h, 004h, 058h, 0CDh, 076h, 006h, 047h, 016h, 0FDh, 010h, 032h
        db  0C2h, 036h, 010h, 026h, 009h, 017h, 0A1h, 047h, 010h, 02Dh, 009h, 017h, 061h, 047h, 010h, 02Ah
        db  043h, 0A0h, 0E0h, 016h, 004h, 05Bh, 043h, 0A0h, 0E0h, 016h, 0C2h, 049h, 016h, 003h, 0C2h, 044h
        db  0C8h, 006h, 008h, 0D4h, 0C1h, 027h, 0ECh, 0D8h, 0C1h, 084h, 071h, 086h, 002h, 026h, 0EDh, 00Eh
        db  006h, 0C4h, 071h, 004h, 002h, 024h, 0ECh, 0F4h, 010h, 0D3h, 0C1h, 009h, 0C1h, 0A0h, 008h, 0D4h
        db  004h, 0C9h, 010h, 010h, 0C2h, 036h, 0D5h, 078h, 041h, 060h, 0E0h, 022h, 005h, 0C5h, 010h, 00Ah
        db  0CDh, 078h, 006h, 047h, 015h, 0FDh, 010h, 006h, 023h, 0A0h, 0E0h, 016h, 016h, 0CDh, 0C2h, 049h
        db  016h, 0ECh, 010h, 0D6h, 0E3h, 0A0h, 0E0h, 016h, 010h, 0BBh, 002h, 008h, 080h, 05Ah, 006h, 0A0h
        db  0DEh, 0F6h, 010h, 03Eh, 092h, 0A0h, 0E0h, 00Ch, 016h, 015h, 002h, 044h, 05Eh, 000h, 016h, 014h
        db  048h, 020h, 0E4h, 06Eh, 001h, 080h, 006h, 0A0h, 0E1h, 072h, 0C8h, 020h, 001h, 09Eh, 001h, 09Eh
        db  02Eh, 0E0h, 000h, 001h, 043h, 0A0h, 0E0h, 018h, 0D2h, 0A0h, 0E0h, 026h, 007h, 083h, 000h, 010h
        db  006h, 0A0h, 0E2h, 02Ah, 004h, 060h, 0DAh, 0AAh, 007h, 084h, 000h, 008h, 004h, 060h, 0DAh, 072h
        db  007h, 085h, 002h, 003h, 0C8h, 005h, 006h, 0CEh, 043h, 0A0h, 0E0h, 012h, 004h, 0E0h, 006h, 0FAh
        db  006h, 0A0h, 0E1h, 0A4h, 002h, 008h, 080h, 048h, 006h, 0A0h, 0DEh, 0F6h, 010h, 011h, 027h, 0A0h
        db  0E0h, 02Ch, 016h, 004h, 002h, 008h, 080h, 054h, 006h, 0A0h, 0DEh, 0F6h, 007h, 083h, 0A8h, 000h
        db  088h, 020h, 007h, 008h, 0E0h, 020h, 016h, 002h, 007h, 083h, 0E8h, 000h, 006h, 0A0h, 0E2h, 02Ah
        db  002h, 008h, 000h, 036h, 006h, 0A0h, 0DEh, 0F6h, 0E8h, 020h, 0E0h, 00Ch, 001h, 082h, 023h, 0A0h
        db  0E0h, 018h, 013h, 006h, 0E3h, 0A0h, 0E0h, 018h, 02Eh, 0E0h, 000h, 000h, 0C0h, 041h, 016h, 0FAh
        db  006h, 0A0h, 0E0h, 002h, 0C0h, 082h, 013h, 002h, 002h, 04Fh, 0FFh, 080h, 004h, 0C4h, 0D1h, 00Fh
        db  006h, 0C4h, 004h, 060h, 0DAh, 072h, 006h, 0A0h, 0E4h, 024h, 002h, 008h, 080h, 036h, 007h, 0A0h
        db  006h, 0D6h, 0E4h, 012h, 006h, 0A0h, 0DEh, 0F6h, 010h, 010h, 006h, 0A0h, 0E4h, 024h, 0D1h, 020h
        db  006h, 0CEh, 013h, 0E6h, 078h, 020h, 0E0h, 012h, 006h, 0CEh, 010h, 0E2h, 0C1h, 020h, 004h, 016h
        db  00Ah, 014h, 006h, 0C4h, 091h, 00Ah, 016h, 001h, 004h, 05Bh, 004h, 060h, 0DAh, 0AAh, 004h, 0CAh
        db  004h, 0CDh, 004h, 0CEh, 004h, 0CFh, 004h, 0E0h, 006h, 0A8h, 004h, 0E0h, 006h, 0BAh, 007h, 084h
        db  001h, 0A0h, 007h, 085h, 000h, 010h, 004h, 0F4h, 006h, 045h, 015h, 0FDh, 007h, 084h, 006h, 0D8h
        db  007h, 085h, 007h, 034h, 061h, 044h, 004h, 0F4h, 006h, 045h, 015h, 0FDh, 007h, 084h, 000h, 0C8h
        db  0C8h, 004h, 007h, 000h, 007h, 084h, 07Fh, 0FFh, 0C8h, 004h, 006h, 0F0h, 007h, 084h, 000h, 006h
        db  0C8h, 004h, 006h, 0EEh, 003h, 0B0h, 0D8h, 020h, 0E0h, 010h, 006h, 065h, 004h, 060h, 0DAh, 0AAh
        db  0F8h, 020h, 0EDh, 063h, 001h, 082h, 0C8h, 020h, 0E0h, 010h, 006h, 0C6h, 0C8h, 020h, 0E0h, 020h
        db  006h, 0C8h, 02Eh, 0E0h, 000h, 000h, 006h, 0A0h, 0E0h, 002h, 0C8h, 020h, 009h, 01Ah, 001h, 0A0h
        db  0C8h, 020h, 009h, 01Ch, 001h, 0A2h, 0C8h, 020h, 009h, 01Eh, 001h, 0A4h, 0C8h, 020h, 009h, 01Ch
        db  001h, 0B0h, 0C8h, 020h, 009h, 01Eh, 001h, 0B2h, 0C8h, 020h, 009h, 01Eh, 006h, 0CCh, 0F8h, 020h
        db  0EDh, 062h, 001h, 080h, 006h, 0A0h, 0DFh, 0ECh, 048h, 020h, 0E0h, 004h, 001h, 080h, 004h, 0E0h
        db  001h, 09Eh, 002h, 008h, 080h, 012h, 006h, 0A0h, 0DEh, 0F6h, 007h, 084h, 000h, 00Ah, 007h, 085h
        db  003h, 0F4h, 088h, 020h, 006h, 0C6h, 0E0h, 020h, 01Bh, 004h, 007h, 084h, 000h, 01Ch, 007h, 085h
        db  003h, 0F8h, 006h, 0A0h, 0E2h, 0A2h, 007h, 085h, 0E5h, 016h, 0C8h, 005h, 003h, 0F8h, 023h, 0A0h
        db  0E0h, 00Ch, 013h, 023h, 0E3h, 0A0h, 0E0h, 00Ch, 02Eh, 0A0h, 003h, 0F4h, 010h, 01Eh, 003h, 0B0h
        db  098h, 020h, 0E0h, 010h, 006h, 065h, 016h, 019h, 002h, 044h, 05Eh, 000h, 016h, 004h, 006h, 020h
        db  006h, 0C6h, 016h, 0D7h, 010h, 008h, 003h, 0B0h, 098h, 020h, 0E0h, 010h, 006h, 065h, 016h, 00Dh
        db  006h, 020h, 006h, 0C8h, 016h, 0C4h, 001h, 00Eh, 000h, 003h, 013h, 003h, 007h, 083h, 082h, 000h
        db  010h, 002h, 007h, 083h, 000h, 001h, 004h, 060h, 0DAh, 0A2h, 004h, 060h, 0DAh, 088h, 001h, 00Eh
        db  000h, 003h, 013h, 00Ah, 002h, 008h, 080h, 00Ch, 006h, 0A0h, 0DEh, 0F6h, 0E3h, 0E0h, 0E0h, 014h
        db  0C8h, 020h, 0E4h, 070h, 001h, 086h, 010h, 026h, 048h, 020h, 0E0h, 00Ch, 001h, 082h, 02Eh, 0E0h
        db  000h, 001h, 0C1h, 060h, 008h, 0D0h, 00Ah, 035h, 0E8h, 005h, 001h, 082h, 0C1h, 020h, 009h, 018h
        db  001h, 004h, 000h, 006h, 013h, 006h, 0D8h, 020h, 0E1h, 0D2h, 008h, 0E1h, 0D8h, 020h, 0E1h, 0D2h
        db  001h, 083h, 021h, 020h, 0E0h, 022h, 016h, 003h, 0E8h, 020h, 0E0h, 022h, 001h, 080h, 021h, 020h
        db  0E0h, 004h, 016h, 004h, 0E3h, 0A0h, 0E0h, 014h, 004h, 060h, 0DEh, 094h, 002h, 008h, 080h, 000h
        db  006h, 0A0h, 0DEh, 0F6h, 0E8h, 020h, 0E0h, 008h, 001h, 082h, 004h, 060h, 0DAh, 088h, 0C8h, 020h
        db  0E4h, 070h, 001h, 086h, 002h, 008h, 080h, 000h, 006h, 0A0h, 0DEh, 0F6h, 0C2h, 020h, 008h, 0D0h
        db  0A2h, 008h, 005h, 008h, 0C8h, 028h, 0E0h, 022h, 006h, 0CAh, 0C8h, 020h, 0E0h, 020h, 006h, 0C6h
        db  0C8h, 020h, 0E0h, 020h, 006h, 0C8h, 0E3h, 060h, 0E0h, 016h, 004h, 060h, 0DAh, 0AAh, 0C1h, 044h
        db  002h, 044h, 05Eh, 000h, 016h, 0F8h, 025h, 060h, 0E4h, 06Ah, 016h, 00Fh, 006h, 020h, 006h, 0C6h
        db  016h, 0F2h, 006h, 020h, 006h, 0CAh, 013h, 003h, 005h, 0A0h, 006h, 0CCh, 010h, 0E6h, 003h, 0B0h
        db  0D8h, 020h, 0E0h, 00Ch, 006h, 065h, 004h, 060h, 0DAh, 0AAh, 006h, 020h, 006h, 0C8h, 016h, 0E3h
        db  088h, 020h, 009h, 01Eh, 006h, 0CCh, 016h, 003h, 007h, 083h, 000h, 008h, 010h, 002h, 007h, 083h
        db  000h, 00Ch, 004h, 060h, 0DAh, 0A2h, 004h, 060h, 0DAh, 0AAh, 023h, 0A0h, 0E0h, 008h, 013h, 003h
        db  023h, 060h, 0E0h, 012h, 016h, 006h, 003h, 0B0h, 0D8h, 020h, 0E3h, 0ABh, 006h, 065h, 004h, 060h
        db  0DAh, 0AAh, 002h, 008h, 080h, 000h, 006h, 0A0h, 0DEh, 0F6h, 004h, 060h, 0DAh, 088h, 002h, 008h
        db  080h, 000h, 006h, 0A0h, 0DEh, 0F6h, 0C8h, 020h, 0E0h, 01Eh, 006h, 0C6h, 0C8h, 020h, 0E0h, 01Eh
        db  006h, 0C8h, 0E3h, 060h, 0E0h, 010h, 004h, 060h, 0DAh, 0AAh, 023h, 0E0h, 0E0h, 014h, 013h, 019h
        db  0C1h, 044h, 002h, 044h, 01Eh, 000h, 016h, 0F5h, 025h, 060h, 0E4h, 06Ah, 016h, 006h, 006h, 020h
        db  006h, 0C8h, 016h, 0EFh, 004h, 0C2h, 004h, 060h, 0EAh, 0F4h, 002h, 008h, 080h, 01Eh, 006h, 0A0h
        db  0DEh, 0F6h, 010h, 007h, 006h, 020h, 006h, 0C6h, 016h, 0E4h, 007h, 083h, 000h, 009h, 006h, 0A0h
        db  0DAh, 0A2h, 004h, 060h, 0DAh, 088h, 004h, 0CEh, 004h, 0E0h, 008h, 0DCh, 0D3h, 0E0h, 0E3h, 0ACh
        db  0C2h, 08Fh, 0C8h, 020h, 0E4h, 072h, 001h, 086h, 048h, 020h, 0E0h, 008h, 001h, 082h, 007h, 086h
        db  000h, 005h, 007h, 084h, 006h, 072h, 0C1h, 054h, 013h, 001h, 02Ch, 0D4h, 002h, 024h, 000h, 00Ah
        db  006h, 006h, 016h, 0F9h, 002h, 008h, 080h, 02Ah, 006h, 0A0h, 0DEh, 0F6h, 02Ch, 020h, 0E0h, 01Ah
        db  004h, 060h, 0D9h, 054h, 006h, 0A0h, 0E1h, 046h, 004h, 0CDh, 023h, 0A0h, 0E0h, 01Ch, 013h, 00Dh
        db  001h, 00Eh, 000h, 003h, 013h, 00Ah, 0E3h, 0A0h, 0E0h, 01Ch, 003h, 0B0h, 0D8h, 020h, 0E0h, 010h
        db  006h, 065h, 0D2h, 0A0h, 0E0h, 026h, 004h, 0CFh, 010h, 008h, 02Dh, 020h, 000h, 001h, 0C0h, 0E0h
        db  008h, 0DCh, 006h, 0A0h, 0E2h, 02Ah, 0D2h, 0A0h, 0E3h, 0ADh, 004h, 060h, 0DAh, 0AAh, 006h, 0A0h
        db  0DFh, 0B8h, 0C2h, 008h, 013h, 019h, 007h, 083h, 080h, 080h, 023h, 0E0h, 0E0h, 014h, 013h, 002h
        db  007h, 083h, 000h, 00Ah, 004h, 060h, 0DAh, 09Eh, 0C1h, 020h, 006h, 006h, 013h, 00Dh, 006h, 0A0h
        db  0DFh, 0B8h, 0C2h, 008h, 013h, 009h, 007h, 083h, 000h, 00Bh, 023h, 0E0h, 0E0h, 014h, 016h, 002h
        db  007h, 083h, 080h, 001h, 004h, 060h, 0DAh, 09Eh, 007h, 083h, 080h, 00Ah, 004h, 060h, 0DAh, 08Ch
        db  093h, 0E0h, 0E0h, 00Eh, 013h, 05Eh, 093h, 0E0h, 0E0h, 010h, 013h, 017h, 023h, 0E0h, 0E0h, 014h
        db  013h, 004h, 007h, 083h, 000h, 007h, 004h, 060h, 0DAh, 09Eh, 007h, 083h, 0A0h, 000h, 006h, 0A0h
        db  0E2h, 02Ah, 007h, 083h, 048h, 000h, 006h, 0A0h, 0E2h, 02Ah, 0D2h, 0A0h, 0E0h, 010h, 0C8h, 020h
        db  0E0h, 01Ch, 006h, 0CAh, 0C8h, 020h, 0E0h, 020h, 006h, 0CCh, 006h, 0A0h, 0E1h, 046h, 002h, 008h
        db  080h, 04Eh, 006h, 0A0h, 0DEh, 0F6h, 023h, 0A0h, 0E0h, 01Ch, 013h, 020h, 088h, 020h, 009h, 01Ah
        db  007h, 00Eh, 016h, 01Ch, 088h, 020h, 009h, 01Ch, 007h, 010h, 016h, 018h, 088h, 020h, 009h, 01Eh
        db  007h, 012h, 016h, 014h, 088h, 020h, 007h, 00Ah, 0E0h, 022h, 013h, 010h, 006h, 020h, 006h, 0CAh
        db  016h, 038h, 0E3h, 0A0h, 0E0h, 020h, 010h, 006h, 023h, 0E0h, 0E0h, 014h, 016h, 0CAh, 0E3h, 0A0h
        db  0E0h, 022h, 004h, 0C2h, 0D2h, 0A0h, 0E3h, 0ACh, 004h, 060h, 0DAh, 092h, 0C8h, 020h, 0E0h, 01Ch
        db  006h, 0CAh, 088h, 0A0h, 006h, 0DCh, 000h, 00Eh, 016h, 010h, 088h, 0A0h, 006h, 0DEh, 000h, 010h
        db  016h, 00Ch, 088h, 0A0h, 006h, 0E0h, 000h, 012h, 016h, 008h, 006h, 020h, 006h, 0CCh, 016h, 019h
        db  0E8h, 020h, 0E0h, 00Eh, 001h, 082h, 0E3h, 0A0h, 0E0h, 01Eh, 0C8h, 020h, 0E0h, 020h, 006h, 0CCh
        db  010h, 010h, 023h, 0A0h, 0E0h, 010h, 016h, 008h, 0C1h, 064h, 000h, 006h, 021h, 060h, 0E0h, 00Ch
        db  013h, 008h, 0D2h, 0A0h, 0E3h, 0AAh, 010h, 005h, 088h, 020h, 007h, 00Ah, 007h, 008h, 012h, 096h
        db  010h, 000h, 004h, 060h, 0DAh, 092h, 093h, 0E0h, 0E0h, 00Eh, 013h, 050h, 093h, 0E0h, 0E3h, 0ABh
        db  013h, 04Dh, 093h, 0E0h, 0E3h, 0AAh, 013h, 01Ch, 006h, 0A0h, 0E1h, 0A4h, 023h, 0A0h, 0E0h, 010h
        db  013h, 045h, 023h, 0A0h, 0E0h, 008h, 016h, 006h, 0E3h, 060h, 0E0h, 01Eh, 0C8h, 020h, 0E0h, 022h
        db  007h, 006h, 010h, 034h, 023h, 0E0h, 0E0h, 014h, 016h, 031h, 0C1h, 060h, 009h, 018h, 021h, 060h
        db  0E0h, 012h, 016h, 02Ch, 006h, 0A0h, 0E9h, 008h, 010h, 031h, 0D2h, 0A0h, 0E3h, 0AAh, 010h, 02Eh
        db  0E3h, 0A0h, 0E0h, 012h, 006h, 0A0h, 0E9h, 008h, 0C1h, 064h, 000h, 006h, 021h, 060h, 0E0h, 00Ch
        db  013h, 025h, 088h, 020h, 007h, 00Eh, 006h, 0DCh, 016h, 014h, 088h, 020h, 007h, 010h, 006h, 0DEh
        db  016h, 010h, 088h, 020h, 007h, 012h, 006h, 0E0h, 016h, 00Ch, 098h, 020h, 006h, 0CEh, 006h, 0CFh
        db  013h, 015h, 006h, 020h, 006h, 0CEh, 016h, 012h, 0E3h, 060h, 0E0h, 01Ah, 0D2h, 0A0h, 0E0h, 00Ch
        db  010h, 00Dh, 0E3h, 060h, 0E0h, 01Eh, 0C8h, 020h, 0E0h, 032h, 007h, 006h, 006h, 0A0h, 0E1h, 046h
        db  002h, 008h, 080h, 048h, 006h, 0A0h, 0DEh, 0F6h, 0D2h, 0A0h, 0E3h, 0ABh, 004h, 060h, 0DAh, 092h
        db  088h, 022h, 000h, 00Eh, 009h, 01Ah, 01Ah, 0C9h, 01Bh, 00Bh, 088h, 022h, 000h, 010h, 009h, 01Ch
        db  01Ah, 0C4h, 01Bh, 006h, 088h, 022h, 000h, 012h, 009h, 01Eh, 01Ah, 0BFh, 01Bh, 001h, 004h, 05Bh
        db  0C1h, 060h, 001h, 06Ch, 002h, 085h, 000h, 043h, 013h, 0E1h, 093h, 0E0h, 0E3h, 0AAh, 016h, 0DEh
        db  0C8h, 0A0h, 0EDh, 04Ah, 000h, 002h, 007h, 084h, 000h, 00Eh, 0C1h, 042h, 0C0h, 0A0h, 001h, 06Ch
        db  0C0h, 002h, 002h, 025h, 000h, 048h, 007h, 081h, 0E9h, 05Ah, 007h, 083h, 0AEh, 014h, 004h, 060h
        db  0ECh, 066h, 002h, 002h, 0FCh, 000h, 010h, 0CAh, 098h, 020h, 0E0h, 00Eh, 006h, 065h, 016h, 003h
        db  0D8h, 020h, 0E3h, 0AAh, 006h, 065h, 093h, 0E0h, 0E3h, 0ABh, 013h, 00Dh, 023h, 0A0h, 0E0h, 008h
        db  016h, 019h, 023h, 0A0h, 0E0h, 010h, 013h, 016h, 0E3h, 060h, 0E0h, 01Eh, 0C8h, 020h, 0E0h, 020h
        db  007h, 006h, 006h, 0A0h, 0E1h, 046h, 043h, 0A0h, 0E0h, 018h, 02Eh, 0E0h, 000h, 001h, 0D2h, 0A0h
        db  0E0h, 026h, 007h, 083h, 000h, 010h, 006h, 0A0h, 0E2h, 02Ah, 023h, 0E0h, 0E0h, 014h, 016h, 002h
        db  006h, 0A0h, 0E1h, 028h, 043h, 0A0h, 0E0h, 02Ch, 0C8h, 020h, 0E0h, 020h, 008h, 0D6h, 004h, 060h
        db  0DAh, 092h, 043h, 0A0h, 0E0h, 00Eh, 0C1h, 0A0h, 008h, 0D6h, 013h, 002h, 006h, 020h, 008h, 0D6h
        db  023h, 0E0h, 0E0h, 014h, 013h, 003h, 023h, 0A0h, 0E0h, 008h, 016h, 026h, 002h, 008h, 080h, 042h
        db  006h, 0A0h, 0DEh, 0F6h, 023h, 0A0h, 0E0h, 008h, 016h, 01Fh, 023h, 0A0h, 0E0h, 010h, 016h, 00Ah
        db  0C1h, 022h, 000h, 002h, 025h, 020h, 0E4h, 06Ah, 016h, 023h, 007h, 083h, 080h, 020h, 006h, 0A0h
        db  0E2h, 02Ah, 010h, 012h, 006h, 0A0h, 0E1h, 046h, 023h, 0E0h, 0E0h, 014h, 016h, 002h, 006h, 0A0h
        db  0E1h, 028h, 0E3h, 060h, 0E0h, 01Eh, 0C8h, 020h, 0E0h, 020h, 007h, 006h, 023h, 0A0h, 0E0h, 008h
        db  016h, 003h, 023h, 0A0h, 0E0h, 006h, 013h, 051h, 098h, 020h, 0E0h, 00Eh, 006h, 065h, 016h, 003h
        db  0D8h, 020h, 0E3h, 0AAh, 006h, 065h, 0C1h, 022h, 000h, 002h, 025h, 020h, 0E4h, 06Ah, 013h, 00Eh
        db  007h, 083h, 000h, 020h, 006h, 0A0h, 0E2h, 02Ah, 0C8h, 022h, 000h, 00Eh, 006h, 0E6h, 0C8h, 022h
        db  000h, 010h, 006h, 0E8h, 0C8h, 022h, 000h, 012h, 006h, 0EAh, 010h, 037h, 088h, 022h, 000h, 00Eh
        db  006h, 0DCh, 016h, 008h, 088h, 022h, 000h, 010h, 006h, 0DEh, 016h, 004h, 088h, 022h, 000h, 012h
        db  006h, 0E0h, 013h, 00Bh, 0C8h, 022h, 000h, 00Eh, 006h, 0DCh, 0C8h, 022h, 000h, 010h, 006h, 0DEh
        db  0C8h, 022h, 000h, 012h, 006h, 0E0h, 0E3h, 060h, 0E0h, 014h, 023h, 0A0h, 0E0h, 00Eh, 016h, 008h
        db  0C1h, 0A0h, 008h, 0D6h, 016h, 01Ah, 007h, 086h, 010h, 000h, 0E8h, 006h, 006h, 0D2h, 010h, 015h
        db  0E3h, 0A0h, 0E0h, 00Eh, 023h, 0A0h, 0E0h, 008h, 016h, 009h, 0E3h, 0A0h, 0E0h, 006h, 004h, 0E0h
        db  006h, 0E6h, 004h, 0E0h, 006h, 0E8h, 004h, 0E0h, 006h, 0EAh, 010h, 007h, 002h, 008h, 080h, 036h
        db  007h, 0A0h, 006h, 0D6h, 0DEh, 0C0h, 006h, 0A0h, 0DEh, 0F6h, 004h, 060h, 0DAh, 092h, 098h, 020h
        db  006h, 065h, 0E0h, 010h, 016h, 003h, 0D8h, 020h, 0E0h, 00Eh, 006h, 065h, 004h, 060h, 0DAh, 092h
        db  023h, 0E0h, 0E0h, 014h, 013h, 002h, 004h, 060h, 0DAh, 092h, 010h, 02Eh, 003h, 0B0h, 098h, 020h
        db  0E3h, 0ABh, 006h, 06Fh, 016h, 019h, 0C2h, 024h, 000h, 008h, 011h, 016h, 0E3h, 0E0h, 0E0h, 014h
        db  007h, 083h, 000h, 000h, 006h, 0A0h, 0E2h, 02Ah, 023h, 0A0h, 0E0h, 014h, 013h, 004h, 002h, 008h
        db  080h, 006h, 006h, 0A0h, 0DEh, 0F6h, 002h, 008h, 000h, 01Eh, 006h, 0A0h, 0DEh, 0F6h, 023h, 0A0h
        db  0E0h, 008h, 013h, 002h, 006h, 0A0h, 0E1h, 028h, 0C0h, 082h, 016h, 002h, 004h, 060h, 0DAh, 0AAh
        db  0E8h, 020h, 0EDh, 066h, 006h, 0F0h, 099h, 020h, 0E0h, 00Eh, 000h, 016h, 016h, 005h, 004h, 0E0h
        db  006h, 0ECh, 048h, 020h, 0E0h, 014h, 006h, 0F0h, 007h, 083h, 000h, 001h, 004h, 060h, 0DAh, 08Ch
        db  0C2h, 064h, 000h, 014h, 002h, 024h, 000h, 018h, 0C1h, 0C4h, 061h, 0C2h, 002h, 027h, 0FFh, 0FCh
        db  0C1h, 074h, 0C1h, 085h, 071h, 045h, 002h, 085h, 000h, 027h, 016h, 046h, 0C1h, 054h, 002h, 045h
        db  0FFh, 0CFh, 016h, 042h, 004h, 0C8h, 0C1h, 064h, 000h, 008h, 015h, 006h, 013h, 005h, 0C2h, 024h
        db  000h, 00Eh, 002h, 048h, 01Fh, 000h, 006h, 0C8h, 002h, 028h, 000h, 011h, 0A2h, 004h, 098h, 018h
        db  0EDh, 06Bh, 016h, 032h, 0C1h, 042h, 002h, 025h, 000h, 004h, 065h, 047h, 0C2h, 035h, 0CDh, 074h
        db  006h, 048h, 015h, 0FDh, 0C0h, 0A0h, 001h, 06Ch, 0C1h, 002h, 0C8h, 004h, 001h, 06Ch, 0C1h, 0A0h
        db  0FCh, 000h, 013h, 005h, 0C8h, 020h, 0EBh, 002h, 0FCh, 002h, 0C1h, 006h, 010h, 0F6h, 0C8h, 002h
        db  001h, 06Ch, 0C8h, 020h, 0EDh, 04Ah, 0FCh, 002h, 007h, 081h, 0EBh, 0D0h, 0C0h, 004h, 007h, 083h
        db  002h, 010h, 007h, 084h, 000h, 00Eh, 010h, 03Bh, 007h, 084h, 000h, 00Ch, 0C0h, 0E2h, 000h, 008h
        db  002h, 005h, 0FCh, 000h, 0C0h, 0A0h, 001h, 06Ch, 0C0h, 002h, 0C1h, 0A0h, 0FCh, 000h, 013h, 02Fh
        db  004h, 0E0h, 0FCh, 000h, 02Eh, 016h, 010h, 02Bh, 0C8h, 0A0h, 0EDh, 06Ch, 000h, 00Eh, 0C8h, 0A0h
        db  0EDh, 06Eh, 000h, 010h, 0C8h, 0A0h, 0EDh, 070h, 000h, 012h, 007h, 083h, 080h, 006h, 004h, 060h
        db  0DAh, 08Ch, 004h, 060h, 0DAh, 0AAh, 007h, 084h, 000h, 010h, 007h, 085h, 000h, 034h, 010h, 009h
        db  007h, 084h, 000h, 012h, 007h, 085h, 000h, 032h, 010h, 004h, 007h, 084h, 000h, 014h, 007h, 085h
        db  000h, 038h, 006h, 0A0h, 0DFh, 0DEh, 0C8h, 085h, 000h, 004h, 006h, 0A0h, 0E0h, 020h, 0C8h, 0A0h
        db  0EDh, 04Ah, 000h, 002h, 0C0h, 0A0h, 001h, 06Ch, 0C0h, 002h, 006h, 048h, 0C1h, 048h, 006h, 0A0h
        db  0E2h, 0A2h, 002h, 007h, 006h, 0A2h, 0C5h, 060h, 0FCh, 002h, 002h, 025h, 0FFh, 0F4h, 0C8h, 005h
        db  0FCh, 002h, 0C2h, 020h, 001h, 06Ch, 006h, 0A0h, 0C1h, 096h, 004h, 060h, 0DAh, 088h, 006h, 0A0h
        db  0E2h, 0A2h, 002h, 007h, 006h, 0B4h, 010h, 0EFh, 0C3h, 00Bh, 003h, 000h, 000h, 002h, 007h, 082h
        db  000h, 0C0h, 0C8h, 020h, 000h, 00Ch, 000h, 0C0h, 0C8h, 020h, 000h, 00Eh, 000h, 0C2h, 0C8h, 020h
        db  000h, 010h, 000h, 0C4h, 0C8h, 020h, 000h, 012h, 000h, 0C6h, 0C8h, 020h, 000h, 014h, 000h, 0C8h
        db  0C8h, 020h, 000h, 016h, 000h, 0CAh, 0C8h, 020h, 000h, 004h, 000h, 0CCh, 0C8h, 020h, 000h, 006h
        db  000h, 0CEh, 0C8h, 002h, 000h, 00Ch, 007h, 0A0h, 000h, 00Eh, 0EDh, 010h, 0C8h, 002h, 000h, 010h
        db  007h, 0A0h, 000h, 012h, 0EDh, 01Ah, 0C8h, 002h, 000h, 014h, 007h, 0A0h, 000h, 016h, 0EDh, 04Ah
        db  0C8h, 002h, 000h, 004h, 007h, 0A0h, 000h, 006h, 0EDh, 060h, 001h, 060h, 001h, 01Ch, 000h, 004h
        db  016h, 009h, 001h, 0E0h, 001h, 040h, 008h, 000h, 001h, 0A0h, 001h, 040h, 040h, 000h, 001h, 0E0h
        db  001h, 040h, 004h, 000h, 006h, 0A0h, 0F0h, 020h, 002h, 005h, 080h, 000h, 0D8h, 005h, 004h, 080h
        db  004h, 0C7h, 003h, 000h, 000h, 00Fh, 007h, 088h, 010h, 000h, 002h, 009h, 020h, 000h, 007h, 08Ah
        db  0EDh, 078h, 002h, 003h, 0E6h, 000h, 004h, 05Ah, 003h, 000h, 000h, 000h, 0D2h, 020h, 001h, 087h
        db  010h, 006h, 003h, 000h, 000h, 000h, 0C2h, 020h, 001h, 08Ah, 002h, 008h, 01Ah, 000h, 0C2h, 060h
        db  000h, 0AEh, 0DAh, 048h, 004h, 080h, 005h, 089h, 002h, 089h, 000h, 006h, 015h, 007h, 007h, 088h
        db  080h, 000h, 0DAh, 048h, 004h, 080h, 0C8h, 009h, 000h, 0AEh, 003h, 080h, 002h, 0E0h, 000h, 0A0h
        db  004h, 05Ch, 003h, 000h, 000h, 000h, 001h, 060h, 001h, 09Ch, 000h, 020h, 013h, 0E2h, 0C2h, 020h
        db  001h, 08Ch, 002h, 008h, 01Ch, 000h, 010h, 0E3h, 003h, 000h, 000h, 000h, 001h, 060h, 001h, 040h
        db  040h, 000h, 016h, 0ECh, 001h, 0A0h, 001h, 040h, 040h, 000h, 002h, 008h, 002h, 000h, 010h, 0D7h
        db  0C0h, 0B3h, 006h, 092h, 010h, 0FDh, 0C0h, 0B3h, 0C0h, 048h, 0CCh, 072h, 0CCh, 072h, 0C1h, 032h
        db  0CCh, 044h, 0DCh, 072h, 006h, 004h, 016h, 0FDh, 004h, 05Bh, 0C0h, 048h, 002h, 002h, 0E9h, 092h
        db  007h, 084h, 000h, 006h, 010h, 0F6h, 002h, 002h, 0E5h, 0E0h, 0C0h, 049h, 007h, 084h, 000h, 006h
        db  010h, 0F0h, 0C0h, 0B3h, 0C1h, 032h, 002h, 001h, 000h, 001h, 0D0h, 044h, 006h, 0C1h, 002h, 044h
        db  000h, 0FFh, 010h, 0E7h, 0C1h, 033h, 0C0h, 073h, 0D1h, 044h, 002h, 044h, 000h, 0FFh, 0DCh, 045h
        db  006h, 004h, 016h, 0FDh, 004h, 05Ah, 006h, 0A0h, 0EFh, 0A0h, 0C8h, 033h, 001h, 09Eh, 004h, 05Ah
        db  006h, 0A0h, 0EDh, 09Eh, 0C1h, 089h, 009h, 0A6h, 002h, 066h, 000h, 040h, 0C8h, 006h, 001h, 08Ah
        db  0C1h, 088h, 009h, 0A6h, 002h, 066h, 000h, 040h, 0C8h, 006h, 001h, 08Ch, 004h, 0C2h, 0C1h, 0C7h
        db  016h, 003h, 006h, 002h, 016h, 0FCh, 010h, 04Dh, 004h, 05Ah, 006h, 0A0h, 0EEh, 0EAh, 0C1h, 089h
        db  009h, 0A6h, 002h, 066h, 000h, 040h, 0C8h, 006h, 001h, 08Ah, 0C1h, 088h, 009h, 0A6h, 002h, 066h
        db  000h, 040h, 0C8h, 006h, 001h, 08Ch, 0C8h, 033h, 001h, 09Eh, 010h, 0E8h, 08Ah, 033h, 000h, 002h
        db  016h, 038h, 08Ah, 073h, 000h, 002h, 016h, 035h, 004h, 05Ah, 08Ah, 020h, 0E9h, 08Ch, 000h, 002h
        db  016h, 030h, 08Ah, 060h, 0E9h, 090h, 000h, 002h, 016h, 02Ch, 007h, 082h, 0EAh, 036h, 010h, 001h
        db  0C0h, 0B3h, 002h, 004h, 004h, 080h, 0D1h, 052h, 013h, 003h, 09Dh, 032h, 016h, 022h, 010h, 0FBh
        db  007h, 085h, 080h, 000h, 0D8h, 005h, 004h, 080h, 004h, 0C7h, 004h, 05Ah, 0C8h, 020h, 000h, 0C0h
        db  000h, 00Ch, 0C8h, 020h, 000h, 0C2h, 000h, 00Eh, 0C8h, 020h, 000h, 0C4h, 000h, 010h, 0C8h, 020h
        db  000h, 0C6h, 000h, 012h, 0C8h, 020h, 000h, 0C8h, 000h, 014h, 0C8h, 020h, 000h, 0CAh, 000h, 016h
        db  0C8h, 020h, 000h, 0CCh, 000h, 004h, 0C8h, 020h, 000h, 0CEh, 000h, 006h, 003h, 000h, 000h, 00Fh
        db  005h, 0CCh, 004h, 05Ch, 004h, 0E0h, 001h, 082h, 002h, 002h, 0E5h, 0DAh, 0C8h, 032h, 001h, 082h
        db  0C8h, 032h, 001h, 080h, 006h, 0A0h, 0EEh, 0B6h, 0C8h, 012h, 001h, 082h, 0C2h, 0CAh, 007h, 084h
        db  007h, 0D0h, 004h, 0E0h, 001h, 084h, 006h, 004h, 016h, 0FCh, 0C1h, 020h, 001h, 084h, 016h, 0E9h
        db  002h, 004h, 000h, 032h, 007h, 085h, 080h, 000h, 0D8h, 005h, 004h, 080h, 004h, 0C7h, 0C1h, 060h
        db  001h, 086h, 006h, 004h, 016h, 0FCh, 0C1h, 020h, 001h, 084h, 004h, 05Bh, 0C0h, 0B3h, 0C4h, 0B3h
        db  004h, 05Bh, 0C0h, 048h, 0C0h, 0B3h, 0A0h, 073h, 0C4h, 042h, 004h, 05Bh, 088h, 033h, 001h, 084h
        db  016h, 0E6h, 004h, 05Ah, 0C1h, 089h, 009h, 0A6h, 002h, 066h, 000h, 040h, 0C8h, 006h, 001h, 08Ah
        db  004h, 05Bh, 004h, 0C5h, 007h, 0A0h, 001h, 09Ch, 000h, 040h, 001h, 060h, 001h, 09Ch, 000h, 040h
        db  013h, 003h, 006h, 005h, 016h, 0F7h, 004h, 05Ch, 004h, 05Bh, 006h, 0A0h, 0EFh, 03Eh, 0C0h, 060h
        db  001h, 040h, 0C8h, 005h, 001h, 040h, 0C5h, 002h, 0C8h, 001h, 001h, 040h, 004h, 05Ah, 006h, 0A0h
        db  0EFh, 03Eh, 0A1h, 008h, 010h, 0F4h, 0C0h, 0B3h, 0C1h, 033h, 0C1h, 060h, 001h, 040h, 001h, 085h
        db  004h, 000h, 001h, 0C5h, 010h, 000h, 004h, 05Bh, 0C1h, 008h, 0C2h, 009h, 0C2h, 044h, 004h, 05Bh
        db  002h, 005h, 000h, 0C8h, 006h, 005h, 016h, 0FEh, 004h, 05Bh, 0C1h, 033h, 0C0h, 003h, 0C0h, 0C4h
        db  004h, 05Bh, 0C0h, 0C0h, 004h, 05Bh, 094h, 0E0h, 001h, 09Eh, 016h, 0C2h, 005h, 0C3h, 004h, 05Bh
        db  0C0h, 073h, 006h, 0A0h, 0EFh, 0B8h, 002h, 02Dh, 000h, 008h, 007h, 085h, 000h, 008h, 09Fh, 071h
        db  016h, 0B7h, 006h, 005h, 016h, 0FCh, 004h, 05Ah, 002h, 002h, 0E5h, 0E6h, 004h, 060h, 0EDh, 0A2h
        db  08Ch, 0E9h, 000h, 004h, 016h, 0ADh, 004h, 05Bh, 0C1h, 020h, 001h, 080h, 007h, 085h, 007h, 0D0h
        db  001h, 0E0h, 001h, 080h, 004h, 000h, 006h, 045h, 016h, 0FEh, 0C8h, 004h, 001h, 080h, 004h, 05Bh
        db  0C1h, 033h, 0C3h, 048h, 0C1h, 004h, 013h, 004h, 002h, 02Dh, 004h, 000h, 006h, 004h, 016h, 0FCh
        db  004h, 05Bh, 0C3h, 08Dh, 006h, 0A0h, 0EFh, 0B8h, 0C1h, 08Dh, 009h, 0A6h, 002h, 066h, 000h, 040h
        db  0C7h, 086h, 004h, 05Ah, 0C1h, 08Dh, 009h, 0A6h, 002h, 066h, 000h, 040h, 0C8h, 006h, 001h, 08Ah
        db  004h, 05Bh, 0C1h, 08Dh, 009h, 0A6h, 002h, 066h, 000h, 040h, 0C8h, 006h, 001h, 08Ch, 004h, 05Bh
        db  0C0h, 04Dh, 002h, 004h, 000h, 028h, 007h, 085h, 000h, 000h, 0F0h, 000h, 006h, 0F8h, 055h, 000h
        db  004h, 060h, 0EDh, 0C6h, 0C0h, 04Dh, 0C0h, 0B3h, 0C1h, 032h, 004h, 060h, 0EDh, 08Ah, 0C1h, 033h
        db  001h, 060h, 001h, 01Ch, 000h, 004h, 016h, 001h, 004h, 05Bh, 0C0h, 0C4h, 004h, 05Bh, 007h, 089h
        db  0E5h, 028h, 0C2h, 039h, 013h, 007h, 0C6h, 039h, 086h, 039h, 016h, 025h, 0C6h, 039h, 086h, 039h
        db  016h, 022h, 010h, 0F7h, 002h, 002h, 0F0h, 03Eh, 004h, 0C4h, 004h, 0C5h, 0C2h, 039h, 013h, 002h
        db  004h, 060h, 0F0h, 07Ah, 002h, 002h, 0F0h, 04Ch, 004h, 0C4h, 0C2h, 039h, 013h, 003h, 0C1h, 079h
        db  004h, 060h, 0F0h, 07Ah, 002h, 002h, 0F0h, 05Ch, 004h, 0C5h, 0C2h, 039h, 013h, 003h, 0C1h, 039h
        db  004h, 060h, 0F0h, 07Ah, 0C0h, 079h, 0C0h, 0B9h, 060h, 081h, 005h, 0C2h, 009h, 012h, 004h, 0F1h
        db  006h, 002h, 016h, 0FDh, 004h, 05Bh, 004h, 05Ch, 002h, 001h, 0AAh, 0AAh, 0C6h, 001h, 0E0h, 044h
        db  040h, 045h, 080h, 058h, 016h, 0F8h, 002h, 001h, 000h, 014h, 006h, 001h, 016h, 0FEh, 002h, 001h
        db  055h, 055h, 0C6h, 001h, 0E0h, 044h, 040h, 045h, 080h, 058h, 016h, 0EDh, 004h, 052h, 0C0h, 048h
        db  0C0h, 089h, 060h, 081h, 005h, 0C2h, 004h, 05Bh, 0C3h, 00Bh, 006h, 0A0h, 0F0h, 0A0h, 0CCh, 041h
        db  006h, 042h, 016h, 0FDh, 006h, 0A0h, 0F0h, 0A0h, 0C1h, 001h, 08Ch, 044h, 016h, 012h, 005h, 0C4h
        db  006h, 042h, 016h, 0FBh, 002h, 004h, 0F0h, 0E6h, 002h, 003h, 001h, 001h, 006h, 094h, 002h, 003h
        db  05Ah, 05Ah, 006h, 094h, 005h, 043h, 006h, 094h, 007h, 003h, 006h, 094h, 004h, 0C3h, 006h, 094h
        db  005h, 0CCh, 004h, 05Ch, 0C1h, 0CBh, 006h, 0A0h, 0F0h, 0A0h, 0CCh, 043h, 006h, 042h, 016h, 0FDh
        db  006h, 0A0h, 0F0h, 0A0h, 08Ch, 043h, 016h, 0F5h, 006h, 042h, 016h, 0FCh, 004h, 057h, 0C2h, 08Bh
        db  0C0h, 008h, 0C1h, 049h, 005h, 085h, 002h, 080h, 000h, 040h, 011h, 003h, 002h, 080h, 000h, 04Fh
        db  012h, 045h, 002h, 001h, 0F0h, 0A0h, 009h, 0A1h, 080h, 001h, 013h, 040h, 002h, 001h, 0F1h, 0D0h
        db  009h, 0A1h, 080h, 001h, 013h, 03Bh, 0C0h, 060h, 000h, 006h, 009h, 0A1h, 080h, 001h, 013h, 036h
        db  005h, 081h, 080h, 001h, 013h, 033h, 0C0h, 04Ah, 009h, 0A1h, 080h, 001h, 013h, 02Fh, 0C8h, 000h
        db  001h, 06Ah, 002h, 080h, 000h, 080h, 014h, 017h, 002h, 001h, 0F8h, 000h, 0C1h, 0A0h, 001h, 040h
        db  001h, 0A0h, 001h, 040h, 004h, 000h, 002h, 002h, 010h, 000h, 002h, 003h, 004h, 000h, 0CCh, 0B1h
        db  006h, 043h, 016h, 0FDh, 001h, 0A0h, 001h, 040h, 040h, 000h, 002h, 008h, 0F8h, 010h, 0C8h, 006h
        db  001h, 040h, 0C0h, 000h, 013h, 002h, 002h, 008h, 0F8h, 000h, 002h, 009h, 0FBh, 0FEh, 006h, 0A0h
        db  0F0h, 0AAh, 010h, 025h, 002h, 080h, 000h, 080h, 014h, 009h, 002h, 001h, 0F8h, 000h, 002h, 002h
        db  010h, 000h, 002h, 003h, 004h, 000h, 0CCh, 072h, 006h, 043h, 016h, 0FDh, 005h, 080h, 002h, 080h
        db  000h, 080h, 012h, 004h, 001h, 060h, 001h, 004h, 000h, 020h, 013h, 005h, 081h, 040h, 016h, 0ABh
        db  002h, 080h, 000h, 080h, 014h, 00Bh, 007h, 0A0h, 001h, 06Ah, 000h, 07Eh, 002h, 002h, 010h, 000h
        db  002h, 003h, 004h, 000h, 004h, 0C1h, 0CCh, 081h, 006h, 043h, 016h, 0FDh, 005h, 0CAh, 004h, 05Ah
        db  002h, 000h, 0F1h, 0C2h, 002h, 001h, 0F2h, 0F2h, 002h, 040h, 0FCh, 000h, 002h, 041h, 0FCh, 000h
        db  080h, 040h, 013h, 004h, 007h, 0A0h, 001h, 004h, 000h, 03Ch, 004h, 05Bh, 004h, 0C0h, 002h, 001h
        db  000h, 008h, 002h, 002h, 012h, 000h, 0C1h, 0E0h, 001h, 040h, 001h, 0A0h, 001h, 040h, 004h, 000h
        db  002h, 003h, 001h, 000h, 0C8h, 000h, 001h, 06Ah, 0CCh, 0A0h, 0F8h, 010h, 005h, 080h, 006h, 003h
        db  016h, 0F9h, 002h, 022h, 002h, 000h, 006h, 001h, 016h, 0F3h, 001h, 0A0h, 001h, 040h, 040h, 000h
        db  0C8h, 007h, 001h, 040h, 002h, 000h, 008h, 000h, 0C0h, 040h, 006h, 001h, 0C8h, 001h, 001h, 06Ah
        db  002h, 061h, 080h, 000h, 0C8h, 001h, 0F8h, 010h, 006h, 000h, 016h, 0F6h, 004h, 0C0h, 004h, 0C8h
        db  004h, 0C9h, 002h, 003h, 008h, 000h, 0C8h, 000h, 001h, 06Ah, 0C1h, 080h, 002h, 066h, 080h, 000h
        db  0C1h, 020h, 0F8h, 010h, 081h, 006h, 016h, 015h, 0C2h, 008h, 013h, 006h, 005h, 080h, 006h, 003h
        db  016h, 0F2h, 0C2h, 008h, 013h, 00Dh, 010h, 019h, 007h, 0A0h, 0F8h, 010h, 055h, 055h, 0C1h, 020h
        db  0F8h, 010h, 002h, 084h, 055h, 055h, 016h, 002h, 0C2h, 006h, 010h, 0F0h, 081h, 006h, 013h, 0EEh
        db  004h, 05Bh, 007h, 0A0h, 0F8h, 010h, 055h, 055h, 0C1h, 060h, 0F8h, 010h, 081h, 005h, 013h, 003h
        db  002h, 085h, 055h, 055h, 016h, 0F5h, 0C2h, 008h, 013h, 0E1h, 0C2h, 040h, 006h, 009h, 002h, 048h
        db  007h, 0FFh, 004h, 0C0h, 002h, 001h, 000h, 008h, 002h, 002h, 012h, 000h, 002h, 003h, 001h, 000h
        db  0C8h, 000h, 001h, 06Ah, 0C8h, 032h, 0F8h, 010h, 005h, 080h, 006h, 003h, 016h, 0F9h, 002h, 022h
        db  002h, 000h, 006h, 001h, 016h, 0F3h, 002h, 088h, 000h, 040h, 015h, 013h, 002h, 089h, 000h, 04Fh
        db  011h, 010h, 004h, 0C0h, 002h, 002h, 012h, 000h, 002h, 001h, 000h, 008h, 002h, 003h, 001h, 000h
        db  0CCh, 080h, 006h, 003h, 016h, 0FDh, 002h, 022h, 002h, 000h, 006h, 001h, 016h, 0F7h, 005h, 0CBh
        db  004h, 05Bh, 007h, 0A0h, 001h, 004h, 000h, 037h, 004h, 05Bh, 002h, 0E0h, 000h, 0A0h, 007h, 088h
        db  000h, 0C0h, 002h, 009h, 0F3h, 050h, 007h, 084h, 0E5h, 0ECh, 002h, 005h, 000h, 001h, 0C2h, 08Bh
        db  004h, 0CCh, 006h, 0A0h, 0F3h, 05Ah, 02Ch, 060h, 000h, 001h, 006h, 099h, 02Ch, 0A0h, 000h, 002h
        db  006h, 099h, 02Dh, 020h, 000h, 004h, 006h, 099h, 02Eh, 020h, 000h, 008h, 006h, 099h, 02Fh, 0A0h
        db  000h, 010h, 005h, 08Ch, 016h, 009h, 0CCh, 080h, 0C4h, 081h, 007h, 083h, 0F3h, 09Eh, 0C0h, 088h
        db  004h, 002h, 005h, 08Ch, 016h, 001h, 010h, 033h, 002h, 0E0h, 000h, 0A0h, 004h, 05Ah, 005h, 08Ch
        db  016h, 0FBh, 0CCh, 080h, 0C4h, 081h, 00Ah, 015h, 0C0h, 0B4h, 0C0h, 012h, 0CCh, 088h, 0C0h, 052h
        db  0C4h, 0B4h, 006h, 042h, 004h, 05Bh, 007h, 02Dh, 000h, 018h, 08Bh, 041h, 000h, 00Ah, 016h, 0ECh
        db  082h, 0C1h, 016h, 0EAh, 002h, 0C2h, 002h, 042h, 002h, 000h, 016h, 0E6h, 003h, 080h, 007h, 081h
        db  000h, 001h, 010h, 0F1h, 002h, 001h, 000h, 002h, 010h, 0EEh, 002h, 001h, 000h, 004h, 010h, 0EBh
        db  002h, 001h, 000h, 008h, 010h, 0E8h, 002h, 001h, 000h, 010h, 010h, 0E5h, 002h, 0A1h, 08Bh, 041h
        db  000h, 010h, 013h, 002h, 004h, 060h, 0F3h, 04Ah, 007h, 02Dh, 000h, 018h, 003h, 080h, 002h, 009h
        db  008h, 000h, 002h, 003h, 000h, 004h, 004h, 0C7h, 006h, 0A0h, 0F4h, 0CAh, 001h, 060h, 001h, 01Ch
        db  000h, 004h, 016h, 01Ch, 001h, 0A0h, 001h, 040h, 008h, 000h, 001h, 0E0h, 001h, 040h, 010h, 000h
        db  002h, 004h, 000h, 001h, 0CEh, 044h, 006h, 0C4h, 0C6h, 044h, 001h, 0A0h, 001h, 040h, 010h, 000h
        db  006h, 049h, 001h, 0A0h, 001h, 040h, 040h, 000h, 001h, 0E0h, 001h, 040h, 004h, 000h, 001h, 0E0h
        db  001h, 040h, 008h, 000h, 006h, 0A0h, 0F5h, 068h, 006h, 0A0h, 0F5h, 068h, 005h, 0C7h, 002h, 004h
        db  0E4h, 0A6h, 004h, 0E0h, 003h, 0D0h, 0C1h, 074h, 0C1h, 0B4h, 005h, 086h, 013h, 01Ch, 002h, 0E0h
        db  000h, 0C0h, 002h, 000h, 000h, 000h, 0C0h, 040h, 0C0h, 080h, 0C0h, 0C0h, 0C1h, 000h, 0C1h, 040h
        db  0C1h, 080h, 0C1h, 0C0h, 0C2h, 000h, 0C2h, 040h, 0C2h, 080h, 0C2h, 0C0h, 0C3h, 000h, 0C3h, 040h
        db  0C3h, 080h, 0C3h, 0C0h, 004h, 0A0h, 000h, 0AAh, 003h, 0D0h, 003h, 0D0h, 010h, 03Fh, 005h, 085h
        db  081h, 085h, 013h, 0E1h, 010h, 0E4h, 005h, 0C7h, 002h, 005h, 07Fh, 0FFh, 0A1h, 045h, 003h, 0D0h
        db  003h, 0D0h, 010h, 034h, 0CCh, 0C0h, 0C4h, 0C1h, 002h, 003h, 000h, 028h, 006h, 0A0h, 0F4h, 0CAh
        db  001h, 0E0h, 001h, 042h, 010h, 000h, 005h, 0C7h, 003h, 0D0h, 003h, 0D0h, 010h, 027h, 005h, 0C7h
        db  0C1h, 0A0h, 001h, 04Ah, 007h, 0A0h, 001h, 04Ah, 00Eh, 000h, 001h, 0A0h, 001h, 040h, 002h, 000h
        db  007h, 020h, 001h, 044h, 0C1h, 060h, 001h, 044h, 002h, 085h, 0FFh, 000h, 016h, 017h, 001h, 0E0h
        db  001h, 040h, 022h, 000h, 002h, 005h, 000h, 080h, 006h, 005h, 003h, 0D0h, 016h, 0FDh, 0C1h, 060h
        db  001h, 046h, 002h, 085h, 0FFh, 000h, 013h, 00Ah, 002h, 005h, 021h, 0DDh, 006h, 005h, 010h, 000h
        db  010h, 000h, 010h, 000h, 010h, 000h, 010h, 000h, 003h, 0D0h, 016h, 0F8h, 010h, 051h, 0C8h, 006h
        db  001h, 04Ah, 0CCh, 0C0h, 0C4h, 0C1h, 010h, 04Bh, 0C0h, 013h, 0CCh, 0C8h, 0C0h, 053h, 002h, 002h
        db  0F4h, 0DAh, 0C4h, 0C2h, 006h, 043h, 004h, 05Bh, 0C0h, 060h, 000h, 0AEh, 002h, 0C4h, 002h, 044h
        db  000h, 00Fh, 088h, 044h, 0E4h, 08Ch, 016h, 03Ch, 002h, 081h, 000h, 008h, 013h, 027h, 0C1h, 021h
        db  0E4h, 09Eh, 0C1h, 014h, 021h, 021h, 0E4h, 07Ch, 016h, 033h, 0C1h, 021h, 0E4h, 084h, 002h, 081h
        db  000h, 000h, 013h, 00Bh, 002h, 00Dh, 000h, 0A0h, 083h, 084h, 013h, 009h, 005h, 0C4h, 083h, 084h
        db  013h, 006h, 005h, 0C4h, 083h, 084h, 013h, 003h, 010h, 023h, 081h, 00Eh, 016h, 021h, 0C1h, 021h
        db  0E4h, 09Eh, 045h, 021h, 0E4h, 07Ch, 001h, 0E0h, 001h, 042h, 010h, 000h, 001h, 0A0h, 001h, 042h
        db  010h, 000h, 0C3h, 0A1h, 0E4h, 096h, 002h, 00Fh, 000h, 02Fh, 003h, 080h, 001h, 0A0h, 001h, 040h
        db  002h, 000h, 001h, 0A0h, 001h, 040h, 080h, 000h, 0C0h, 06Dh, 000h, 00Ah, 013h, 009h, 002h, 081h
        db  000h, 00Ah, 015h, 006h, 002h, 00Eh, 0F4h, 0C0h, 002h, 00Fh, 000h, 00Fh, 003h, 080h, 005h, 0CAh
        db  002h, 0E0h, 000h, 0A0h, 004h, 05Ah, 0CEh, 059h, 088h, 020h, 0E4h, 0A6h, 0E4h, 0A6h, 010h, 0F8h
        db  004h, 0C1h, 062h, 048h, 005h, 089h, 0C0h, 0A0h, 001h, 06Ch, 0C8h, 008h, 001h, 06Ch, 002h, 003h
        db  0FCh, 000h, 002h, 004h, 002h, 000h, 0A0h, 073h, 006h, 004h, 016h, 0FDh, 005h, 088h, 006h, 009h
        db  016h, 0F4h, 0C8h, 002h, 001h, 06Ch, 086h, 081h, 016h, 001h, 005h, 0CBh, 004h, 05Bh, 028h, 043h
        db  029h, 031h, 039h, 038h, 033h, 02Dh, 038h, 038h, 020h, 042h, 059h, 020h, 054h, 049h, 0C2h, 04Bh
        db  002h, 0A8h, 000h, 098h, 007h, 083h, 000h, 002h, 002h, 028h, 000h, 008h, 0C6h, 023h, 0E4h, 0F8h
        db  006h, 048h, 0C0h, 0C4h, 00Ah, 073h, 017h, 065h, 006h, 0A0h, 0F6h, 096h, 0C1h, 0C8h, 005h, 0C7h
        db  002h, 003h, 000h, 0A5h, 003h, 0B0h, 0CDh, 0F8h, 0CDh, 0F8h, 002h, 0A6h, 062h, 006h, 002h, 088h
        db  000h, 00Ah, 016h, 057h, 029h, 003h, 016h, 055h, 029h, 005h, 080h, 0C4h, 016h, 052h, 009h, 015h
        db  017h, 050h, 009h, 015h, 018h, 04Eh, 002h, 085h, 000h, 029h, 016h, 04Bh, 005h, 0C6h, 000h, 096h
        db  007h, 003h, 004h, 0C4h, 006h, 045h, 000h, 095h, 005h, 044h, 016h, 043h, 081h, 044h, 016h, 041h
        db  003h, 000h, 000h, 005h, 002h, 0C4h, 003h, 000h, 000h, 00Ah, 002h, 044h, 000h, 00Fh, 002h, 084h
        db  000h, 005h, 016h, 037h, 002h, 0C4h, 003h, 000h, 000h, 00Fh, 002h, 044h, 000h, 00Fh, 002h, 084h
        db  000h, 00Ah, 016h, 02Fh, 002h, 004h, 0FFh, 0FEh, 013h, 02Ch, 015h, 02Bh, 01Ah, 02Ah, 005h, 084h
        db  012h, 028h, 015h, 027h, 01Ah, 026h, 018h, 025h, 005h, 084h, 016h, 023h, 01Bh, 022h, 017h, 021h
        db  005h, 084h, 013h, 01Fh, 01Ah, 01Eh, 011h, 01Dh, 006h, 004h, 016h, 01Bh, 002h, 0A5h, 0C1h, 0C5h
        db  002h, 025h, 000h, 006h, 002h, 003h, 0A5h, 0A5h, 0C1h, 083h, 000h, 095h, 038h, 003h, 000h, 094h
        db  002h, 083h, 06Bh, 02Eh, 016h, 00Eh, 002h, 084h, 01Ch, 059h, 016h, 00Bh, 002h, 024h, 000h, 069h
        db  000h, 095h, 03Ch, 003h, 000h, 094h, 081h, 083h, 016h, 004h, 002h, 084h, 000h, 069h, 016h, 001h
        db  005h, 0C9h, 004h, 059h, 0D0h, 0C3h, 013h, 0FDh, 01Ch, 001h, 010h, 0FBh, 090h, 0E0h, 0E4h, 0FFh
        db  016h, 0F8h, 006h, 0C3h, 0D0h, 0C3h, 01Ch, 0F5h, 016h, 0F4h, 090h, 0E0h, 0E4h, 0FCh, 016h, 0F1h
        db  004h, 05Bh, 0C3h, 00Bh, 002h, 009h, 0E5h, 000h, 006h, 0A0h, 0F0h, 024h, 005h, 0CCh, 004h, 05Ch
        db  007h, 088h, 0B0h, 000h, 007h, 089h, 0FFh, 0FEh, 009h, 0A8h, 009h, 0A9h, 007h, 08Ah, 0E0h, 002h
        db  006h, 0A0h, 0F5h, 072h, 000h, 000h, 007h, 088h, 0A0h, 000h, 007h, 089h, 0AFh, 0FEh, 009h, 0A8h
        db  009h, 0A9h, 007h, 08Ah, 0E0h, 07Ah, 006h, 0A0h, 0F5h, 072h, 000h, 000h, 006h, 0A0h, 0F5h, 0B0h
        db  000h, 000h, 010h, 0E6h, 010h, 0E5h, 000h, 000h, 0F6h, 0F8h, 008h, 0C1h, 000h, 000h, 000h, 000h
        db  0A0h, 000h, 001h, 0D2h, 0E2h, 023h, 061h, 0E5h, 080h, 000h, 040h, 000h, 020h, 000h, 010h, 000h
        db  008h, 000h, 004h, 000h, 002h, 000h, 001h, 000h, 000h, 080h, 000h, 040h, 000h, 020h, 000h, 010h
        db  000h, 008h, 000h, 004h, 000h, 002h, 000h, 001h, 0FFh, 000h, 000h, 0FFh, 0FFh, 0F0h, 0F0h, 000h
        db  000h, 00Fh, 0FFh, 0FFh, 07Fh, 0FFh, 000h, 003h, 000h, 000h, 000h, 0D4h, 0E7h, 0E7h, 0E7h, 0F2h
        db  0F2h, 0F3h, 028h, 043h, 029h, 020h, 043h, 04Fh, 050h, 059h, 052h, 049h, 047h, 048h, 054h, 020h
        db  049h, 042h, 04Dh, 020h, 031h, 039h, 038h, 033h, 02Ch, 034h, 02Ch, 035h, 02Ch, 036h, 028h, 043h
        db  029h, 020h, 043h, 04Fh, 050h, 059h, 052h, 049h, 047h, 048h, 054h, 020h, 054h, 049h, 020h, 031h
        db  039h, 038h, 033h, 02Dh, 038h, 039h, 02Ch, 039h, 030h, 02Ch, 031h, 02Ch, 032h, 000h, 000h, 000h
        db  0A6h, 00Eh, 0F5h, 0B0h, 0F2h, 0FCh, 0ECh, 070h, 0F6h, 0B4h, 000h, 000h, 000h, 0C4h, 0BCh, 0A6h
        db  006h, 04Ah, 006h, 050h, 006h, 04Ch, 0D8h, 0EAh, 006h, 04Eh, 000h, 00Fh, 006h, 032h, 000h, 001h
        db  007h, 050h, 007h, 058h, 007h, 052h, 0C2h, 00Ah, 007h, 054h, 000h, 00Fh, 007h, 038h, 000h, 001h
        db  000h, 0BAh, 000h, 0A0h, 000h, 0BCh, 0F6h, 0C2h, 000h, 0BEh, 000h, 00Fh, 007h, 05Eh, 007h, 03Ah
        db  007h, 062h, 080h, 040h, 007h, 064h, 0C6h, 08Ah, 007h, 066h, 0C6h, 068h, 007h, 068h, 0C4h, 05Ch
        db  007h, 098h, 080h, 000h, 007h, 078h, 080h, 000h, 008h, 094h, 000h, 004h, 008h, 096h, 000h, 001h
        db  008h, 09Eh, 000h, 008h, 008h, 0A8h, 000h, 00Ah, 008h, 0AAh, 000h, 006h, 008h, 0B2h, 000h, 00Ch
        db  008h, 0B4h, 000h, 004h, 001h, 0AEh, 000h, 000h, 008h, 0D0h, 000h, 000h, 009h, 014h, 000h, 000h
        db  006h, 00Ch, 000h, 013h, 006h, 00Ah, 000h, 020h, 000h, 000h, 000h, 0E0h, 0B3h, 078h, 000h, 0E0h
        db  0B2h, 0D8h, 000h, 0E0h, 0B3h, 078h, 000h, 0E0h, 0B4h, 0EEh, 000h, 0E0h, 0B5h, 05Eh, 000h, 0E0h
        db  0B8h, 0B2h, 000h, 0E0h, 0B4h, 004h, 000h, 0C0h, 0B4h, 014h, 000h, 0E0h, 0B3h, 078h, 000h, 0E0h
        db  0B3h, 078h, 000h, 0C0h, 0B4h, 066h, 000h, 0E0h, 0B3h, 078h, 000h, 0E0h, 0B3h, 078h, 000h, 0E0h
        db  0B3h, 078h, 000h, 0E0h, 0B3h, 078h, 000h, 0E0h, 0B3h, 078h, 000h, 0C0h, 0BCh, 0B2h, 000h, 0C0h
        db  0BDh, 01Eh, 000h, 0C0h, 0BDh, 05Ah, 000h, 0C0h, 0BDh, 0CAh, 000h, 0C0h, 0BEh, 04Ch, 000h, 0E0h
        db  0BFh, 0A0h, 000h, 0E0h, 0BFh, 046h, 000h, 0E0h, 0C0h, 0A2h, 000h, 0E0h, 0C0h, 038h, 000h, 0E0h
        db  0BAh, 00Eh, 000h, 0C0h, 0BAh, 062h, 000h, 0C0h, 0BAh, 0BEh, 000h, 0C0h, 0BAh, 0F0h, 000h, 0C0h
        db  0BAh, 038h, 000h, 0C0h, 0B3h, 072h, 000h, 0C0h, 0B3h, 072h, 03Fh, 000h, 07Fh, 000h, 05Eh, 000h
        db  000h, 030h, 000h, 028h, 000h, 043h, 0B6h, 0A4h, 0B6h, 0A4h, 0B5h, 008h, 0B5h, 000h, 0B5h, 032h
        db  0B5h, 032h, 0B5h, 04Eh, 0B6h, 0A4h, 040h, 000h, 000h, 000h, 020h, 000h, 000h, 000h, 008h, 000h
        db  020h, 000h, 008h, 000h, 080h, 000h, 008h, 000h, 001h, 000h, 010h, 000h, 000h, 000h, 000h, 008h
        db  000h, 00Ch, 000h, 010h, 000h, 014h, 010h, 00Eh, 00Ch, 00Ch, 00Ah, 00Ah, 00Ah, 00Ah, 008h, 008h
        db  008h, 008h, 008h, 008h, 008h, 008h, 000h, 000h, 0A1h, 0D2h, 000h, 011h, 006h, 006h, 000h, 000h
        db  0A1h, 0E2h, 000h, 021h, 004h, 004h, 000h, 000h, 0A2h, 002h, 000h, 041h, 002h, 002h, 000h, 000h
        db  0A2h, 042h, 000h, 081h, 000h, 000h, 000h, 000h, 0A2h, 0C2h, 007h, 08Eh, 007h, 098h, 0D6h, 072h
        db  007h, 058h, 0C5h, 026h, 007h, 058h, 0C4h, 0A2h, 007h, 058h, 0C5h, 096h, 007h, 058h, 0C4h, 0FEh
        db  007h, 098h, 0D2h, 0D0h, 007h, 098h, 0D2h, 0C8h, 007h, 078h, 0CDh, 08Ah, 007h, 058h, 0C5h, 05Eh
        db  007h, 098h, 0D6h, 02Ah, 007h, 078h, 0CEh, 03Eh, 007h, 058h, 0D3h, 040h, 007h, 058h, 0C5h, 00Eh
        db  007h, 058h, 0C5h, 04Ch, 007h, 058h, 0C6h, 010h, 007h, 098h, 0D7h, 0BCh, 007h, 098h, 0D3h, 06Ch
        db  007h, 078h, 0CEh, 028h, 007h, 0B8h, 0D8h, 022h, 000h, 000h, 000h, 000h, 000h, 000h, 0C6h, 0D8h
        db  0CCh, 0E0h, 0CDh, 092h, 0D2h, 04Ch, 0C9h, 048h, 0C9h, 080h, 0C9h, 09Eh, 0CAh, 028h, 0CAh, 04Ch
        db  0CAh, 068h, 0CAh, 0F4h, 0CAh, 0F4h, 0CAh, 014h, 0CBh, 092h, 0C9h, 0CEh, 000h, 000h, 000h, 000h
        db  02Ch, 000h, 00Ch, 000h, 00Fh, 001h, 0FEh, 0FFh, 058h, 000h, 00Eh, 000h, 0FEh, 0FFh, 000h, 00Eh
        db  070h, 000h, 080h, 040h, 05Eh, 000h, 0C0h, 0A0h, 0FFh, 0DFh, 018h, 000h, 0E0h, 000h, 078h, 000h
        db  050h, 000h, 060h, 000h, 070h, 000h, 00Ch, 000h, 000h, 006h, 000h, 000h, 0E3h, 086h, 007h, 0E6h
        db  007h, 0EFh, 000h, 008h, 000h, 000h, 0E3h, 08Ch, 007h, 0EAh, 007h, 0EFh, 000h, 006h, 000h, 040h
        db  00Ah, 000h, 007h, 0E6h, 007h, 0EEh, 000h, 008h, 000h, 040h, 00Ah, 006h, 007h, 0EAh, 007h, 0EEh
        db  000h, 000h, 0C1h, 0E2h, 0D4h, 08Bh, 0FFh, 0FFh, 0D1h, 0D7h, 0C5h, 0D9h, 0C3h, 0D4h, 059h, 03Bh
        db  008h, 0E6h, 005h, 0FCh, 009h, 01Ah, 006h, 0D8h, 004h, 006h, 0EAh, 07Ch, 008h, 0E2h, 004h, 048h
        db  008h, 080h, 000h, 006h, 006h, 00Ah, 00Ch, 00Eh, 0DAh, 092h, 0E7h, 050h, 0E7h, 078h, 0E8h, 05Eh
        db  0E9h, 060h, 0E9h, 0BAh, 0EAh, 014h, 0EAh, 0C6h, 0DAh, 092h, 0EBh, 048h, 0EAh, 0D8h, 0E7h, 036h
        db  0EAh, 0E4h, 0EAh, 0E4h, 0ECh, 00Eh, 0ECh, 018h, 0ECh, 022h, 0E4h, 036h, 0E6h, 0BEh, 0DAh, 088h
        db  0E4h, 088h, 0E5h, 056h, 0E5h, 0C6h, 0E6h, 042h, 0E6h, 066h, 0DAh, 0E0h, 0DAh, 088h, 0E3h, 0A6h
        db  0E3h, 088h, 0E3h, 042h, 0DAh, 088h, 0E6h, 0CAh, 0DAh, 088h, 006h, 072h, 0DEh, 080h, 007h, 008h
        db  006h, 072h, 0DDh, 0DEh, 001h, 0F4h, 006h, 072h, 0DDh, 0BEh, 007h, 008h, 006h, 07Ch, 0E5h, 02Eh
        db  000h, 004h, 006h, 07Ch, 0DEh, 002h, 000h, 000h, 006h, 07Ch, 0E6h, 0ACh, 000h, 0FAh, 006h, 086h
        db  0DDh, 052h, 000h, 005h, 006h, 090h, 0E6h, 0FCh, 000h, 028h, 006h, 090h, 0DEh, 0DAh, 001h, 004h
        db  006h, 090h, 000h, 000h, 000h, 002h, 006h, 090h, 0DEh, 00Ah, 002h, 0BCh, 006h, 09Ah, 0DEh, 090h
        db  005h, 0DCh, 006h, 09Ah, 0DEh, 034h, 000h, 064h, 006h, 09Ah, 0DEh, 094h, 000h, 014h, 006h, 09Ah
        db  0E7h, 0F0h, 006h, 040h, 006h, 09Ah, 0DEh, 09Ch, 000h, 064h, 000h, 000h, 007h, 00Ah, 007h, 00Eh
        db  007h, 004h, 006h, 0D8h, 007h, 000h, 006h, 0F0h, 006h, 0EEh, 006h, 0ECh, 007h, 00Ch, 006h, 0E6h
        db  007h, 018h, 050h, 000h, 000h, 0CCh, 000h, 003h, 084h, 000h, 0A8h, 000h, 0A0h, 000h, 020h, 000h
        db  080h, 000h, 040h, 000h, 008h, 000h, 040h, 000h, 080h, 000h, 040h, 000h, 010h, 000h, 0F5h, 070h
        db  0F4h, 036h, 0F4h, 050h, 0F4h, 06Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 009h, 000h, 009h
        db  0F4h, 0D8h, 0F4h, 040h, 0F4h, 056h, 0F4h, 070h, 001h, 040h, 001h, 042h, 001h, 042h, 001h, 042h
        db  000h, 000h, 000h, 07Fh, 000h, 0A0h, 000h, 0FFh, 002h, 010h, 002h, 01Fh, 002h, 030h, 002h, 03Fh
        db  002h, 050h, 002h, 05Fh, 002h, 070h, 002h, 07Fh, 002h, 090h, 002h, 09Fh, 002h, 0B0h, 002h, 0BFh
        db  002h, 0D0h, 002h, 0DFh, 002h, 0E1h, 002h, 0FFh, 003h, 001h, 003h, 07Fh, 003h, 081h, 003h, 08Fh
        db  003h, 091h, 003h, 09Fh, 003h, 0A1h, 003h, 0AFh, 003h, 0B1h, 003h, 0BFh, 003h, 0C1h, 003h, 0CFh
        db  003h, 0E1h, 003h, 0FFh, 007h, 0C0h, 007h, 0FFh, 00Ch, 000h, 00Fh, 0FFh, 030h, 000h, 037h, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FEh, 0BCh, 000h, 007h, 002h, 05Eh, 001h, 000h, 0BAh, 0FFh, 0BAh, 080h
        db  000h, 000h, 000h, 000h, 000h, 000h, 001h, 00Ch, 001h, 00Ah, 001h, 00Eh, 001h, 010h, 001h, 014h
        db  000h, 000h, 001h, 012h, 0F8h, 000h, 001h, 016h, 0FFh, 000h, 000h, 000h, 000h, 000h, 001h, 00Ah
        db  001h, 01Ch, 001h, 082h, 096h, 066h, 096h, 066h, 055h, 055h, 000h, 000h, 001h, 082h, 08Ah, 02Ah
        db  08Ah, 02Ah, 0C9h, 018h, 0C9h, 018h, 001h, 086h, 0A2h, 0AAh, 0A0h, 01Eh, 055h, 055h, 054h, 01Eh
        db  001h, 08Ah, 050h, 000h, 000h, 000h, 0A8h, 000h, 000h, 000h, 001h, 08Ch, 050h, 000h, 000h, 000h
        db  0A8h, 000h, 000h, 000h, 001h, 08Eh, 050h, 000h, 000h, 000h, 0A8h, 000h, 000h, 000h, 001h, 090h
        db  050h, 000h, 000h, 000h, 0A8h, 000h, 000h, 000h, 001h, 092h, 050h, 000h, 000h, 000h, 0A8h, 000h
        db  000h, 000h, 001h, 094h, 050h, 000h, 000h, 000h, 0A8h, 000h, 000h, 000h, 001h, 096h, 050h, 000h
        db  000h, 000h, 0A8h, 000h, 000h, 000h, 001h, 098h, 050h, 000h, 000h, 000h, 0A8h, 000h, 000h, 000h
        db  001h, 09Ah, 050h, 000h, 000h, 000h, 0A8h, 000h, 000h, 000h, 001h, 09Ch, 055h, 055h, 07Fh, 0C0h
        db  0AAh, 0AAh, 07Fh, 0C0h, 000h, 000h, 001h, 0A2h, 001h, 0A4h, 001h, 0A8h, 001h, 0AAh, 001h, 0AEh
        db  001h, 0B0h, 001h, 0B2h, 001h, 080h, 000h, 000h, 001h, 088h, 0FFh, 000h, 001h, 09Eh, 000h, 0FFh
        db  001h, 0A0h, 080h, 000h, 001h, 0ACh, 080h, 000h, 000h, 000h, 001h, 0A6h, 080h, 000h, 000h, 000h
        db  001h, 080h, 001h, 0B2h, 088h, 000h, 006h, 000h, 0C8h, 000h, 000h, 000h, 080h, 000h, 000h, 030h
        db  000h, 000h, 080h, 000h, 000h, 002h, 000h, 044h, 0F3h, 080h, 000h, 048h, 0F3h, 086h, 000h, 050h
        db  0F3h, 08Ch, 000h, 060h, 0F3h, 092h, 000h, 078h, 0F3h, 098h, 0EEh, 09Ch, 0EDh, 0AAh, 0E9h, 0FEh
        db  0EDh, 0BCh, 055h, 014h, 001h, 0A0h, 0EDh, 07Eh, 0E9h, 092h, 0EDh, 0D8h, 0EEh, 032h, 0EDh, 092h
        db  0EEh, 0EAh, 000h, 000h, 000h, 01Eh, 0EDh, 0D8h, 0EEh, 024h, 041h, 000h, 041h, 001h, 0EEh, 048h
        db  0EAh, 035h, 0EDh, 0AAh, 0EAh, 00Ah, 0EDh, 07Eh, 0E9h, 0C6h, 0EDh, 0E8h, 0EDh, 0F4h, 0EEh, 048h
        db  0EAh, 030h, 0EEh, 0F4h, 000h, 000h, 0EEh, 0C8h, 0EDh, 07Eh, 0E9h, 0BCh, 0EDh, 0E8h, 0EDh, 0F4h
        db  0EEh, 0C8h, 0EEh, 0F4h, 000h, 000h, 0EDh, 07Eh, 0E9h, 0B2h, 0EDh, 09Eh, 0EDh, 0DCh, 0EDh, 0F4h
        db  0EEh, 0C8h, 0EDh, 07Eh, 0E9h, 0BCh, 0EDh, 0E8h, 0EDh, 0F4h, 0EEh, 0C8h, 0EEh, 0F4h, 020h, 000h
        db  0EDh, 0BCh, 055h, 014h, 001h, 0A0h, 0EDh, 0AAh, 0EAh, 012h, 0EDh, 07Eh, 0E9h, 092h, 0EEh, 0EAh
        db  055h, 050h, 000h, 00Ch, 0EDh, 0D8h, 0EEh, 024h, 001h, 000h, 000h, 000h, 0EEh, 048h, 0EAh, 037h
        db  0EDh, 092h, 0EEh, 0EAh, 055h, 055h, 000h, 00Ch, 0EDh, 0E8h, 0EEh, 032h, 0EDh, 092h, 0EEh, 0EAh
        db  0FFh, 0FFh, 000h, 008h, 0EEh, 0EAh, 010h, 002h, 000h, 006h, 0EDh, 0D8h, 0EEh, 024h, 001h, 000h
        db  001h, 001h, 0EEh, 048h, 0EAh, 042h, 0EDh, 092h, 0EEh, 0EAh, 0C0h, 000h, 000h, 008h, 0EEh, 0EAh
        db  0FFh, 0FFh, 000h, 00Ah, 0EEh, 0EAh, 0FFh, 0FFh, 000h, 00Ch, 0EEh, 0EAh, 010h, 00Dh, 000h, 006h
        db  0EDh, 0D8h, 0EEh, 024h, 001h, 000h, 001h, 001h, 0EEh, 048h, 0EAh, 036h, 0EEh, 0F4h, 020h, 008h
        db  0EDh, 092h, 0EEh, 0E4h, 001h, 082h, 0C9h, 002h, 0EDh, 0D8h, 0EEh, 048h, 0EAh, 042h, 0EEh, 0F4h
        db  020h, 034h, 0EDh, 092h, 0EEh, 0EAh, 010h, 000h, 000h, 006h, 0EDh, 0D8h, 0EFh, 058h, 0EEh, 048h
        db  0EAh, 03Ah, 0EEh, 0E4h, 001h, 09Ch, 000h, 040h, 0EDh, 0AAh, 0EAh, 012h, 0EDh, 0BCh, 000h, 0FFh
        db  007h, 080h, 0EFh, 0B8h, 000h, 003h, 0EFh, 0F8h, 0F0h, 006h, 0E9h, 0D4h, 0EFh, 0CAh, 000h, 000h
        db  0F0h, 006h, 0E9h, 0DEh, 0EFh, 0CAh, 000h, 004h, 0F0h, 006h, 0E9h, 0E6h, 0EFh, 0CAh, 000h, 007h
        db  0F0h, 006h, 0E9h, 0EEh, 0EFh, 0B8h, 000h, 001h, 0F0h, 006h, 0E9h, 0F6h, 0EFh, 0CAh, 000h, 002h
        db  0F0h, 006h, 0E9h, 0F6h, 0EFh, 0CAh, 000h, 006h, 0F0h, 006h, 0E9h, 0F6h, 0EFh, 0CAh, 000h, 005h
        db  0F0h, 006h, 0E9h, 0F6h, 0EFh, 0B8h, 000h, 001h, 0EFh, 0DCh, 0EFh, 0B8h, 000h, 003h, 0EFh, 0EAh
        db  0EDh, 0F4h, 0EFh, 078h, 0E9h, 09Ah, 000h, 001h, 0EFh, 078h, 0E9h, 0E7h, 000h, 002h, 0EFh, 078h
        db  0E9h, 0F1h, 000h, 006h, 0EFh, 078h, 0E9h, 0FCh, 000h, 005h, 0EEh, 048h, 0EAh, 036h, 0EEh, 0C8h
        db  0EDh, 07Eh, 0E9h, 092h, 0EDh, 0E8h, 0EFh, 058h, 0EDh, 09Eh, 0EEh, 024h, 001h, 000h, 080h, 000h
        db  0EEh, 048h, 0EAh, 03Ah, 0EDh, 092h, 0EFh, 090h, 0EEh, 0E4h, 001h, 080h, 08Eh, 041h, 0EDh, 0DCh
        db  0EEh, 024h, 001h, 000h, 01Bh, 001h, 0EFh, 098h, 0FFh, 0E4h, 0EEh, 048h, 0EAh, 03Eh, 0EFh, 050h
        db  0EDh, 0AAh, 0EAh, 018h, 0EDh, 09Eh, 0EEh, 0FCh, 0EDh, 0CEh, 0E0h, 000h, 0EFh, 058h, 0EEh, 048h
        db  0EAh, 048h, 0EDh, 0CEh, 0E8h, 000h, 0EDh, 0F4h, 0EEh, 048h, 0EAh, 047h, 0EDh, 0CEh, 008h, 000h
        db  0EFh, 058h, 0EEh, 048h, 0EAh, 048h, 0EDh, 0CEh, 0F8h, 000h, 0EDh, 0F4h, 0EEh, 048h, 0EAh, 047h
        db  0EEh, 0E4h, 001h, 080h, 002h, 000h, 0EDh, 0CEh, 0E0h, 000h, 0EDh, 0F4h, 0EEh, 048h, 0EAh, 047h
        db  0EEh, 0E4h, 001h, 084h, 000h, 000h, 0EEh, 0F4h, 000h, 034h, 0EDh, 0CEh, 000h, 000h, 0EFh, 058h
        db  0EEh, 0F4h, 060h, 034h, 0EFh, 0A0h, 0EEh, 0E4h, 001h, 084h, 000h, 000h, 0EEh, 048h, 0EAh, 048h
        db  0EEh, 0E4h, 001h, 082h, 0C8h, 000h, 0EDh, 0CEh, 0E0h, 000h, 0EFh, 058h, 0EDh, 0CEh, 010h, 000h
        db  0EFh, 058h, 0EEh, 0F4h, 060h, 034h, 0EEh, 0E4h, 001h, 080h, 006h, 000h, 0EDh, 0CEh, 000h, 010h
        db  0EFh, 00Ah, 0EEh, 0C8h, 0EEh, 0E4h, 001h, 084h, 000h, 000h, 0EEh, 0F4h, 000h, 034h, 0EDh, 07Eh
        db  0E9h, 092h, 0EDh, 0AAh, 0EAh, 01Eh, 0EFh, 062h, 0E9h, 054h, 0EDh, 092h, 0EDh, 09Eh, 0EEh, 002h
        db  0F0h, 040h, 000h, 006h, 0C7h, 000h, 0EEh, 032h, 0EFh, 06Eh, 0E0h, 000h, 0EDh, 092h, 0EDh, 09Eh
        db  0EEh, 002h, 0D0h, 040h, 000h, 006h, 0E0h, 000h, 0EEh, 032h, 0EFh, 06Eh, 0C0h, 000h, 0EDh, 092h
        db  0EDh, 09Eh, 0EEh, 002h, 090h, 040h, 000h, 006h, 0A0h, 000h, 0EEh, 032h, 0EFh, 06Eh, 080h, 000h
        db  0EDh, 092h, 0EDh, 09Eh, 0EEh, 002h, 050h, 040h, 000h, 006h, 060h, 000h, 0EEh, 032h, 0EFh, 06Eh
        db  040h, 000h, 0EDh, 092h, 0EDh, 09Eh, 0EEh, 002h, 010h, 040h, 000h, 006h, 020h, 000h, 0EEh, 032h
        db  0EFh, 06Eh, 000h, 000h, 0EFh, 062h, 0E9h, 054h, 0EDh, 092h, 0EDh, 09Eh, 0EEh, 002h, 0D0h, 040h
        db  000h, 006h, 0A6h, 000h, 0EEh, 032h, 0EFh, 06Eh, 0C0h, 000h, 0EDh, 092h, 0EDh, 09Eh, 0EEh, 002h
        db  090h, 040h, 000h, 006h, 0C0h, 000h, 0EEh, 032h, 0EFh, 06Eh, 080h, 000h, 0EDh, 092h, 0EDh, 09Eh
        db  0EEh, 002h, 050h, 040h, 000h, 006h, 040h, 000h, 0EEh, 032h, 0EFh, 06Eh, 040h, 000h, 0EDh, 092h
        db  0EDh, 09Eh, 0EEh, 002h, 070h, 040h, 000h, 006h, 060h, 000h, 0EEh, 032h, 0EFh, 06Eh, 060h, 000h
        db  0F0h, 010h, 0E9h, 052h, 0EDh, 0AAh, 0EAh, 024h, 0EDh, 07Eh, 0E9h, 092h, 0EFh, 036h, 055h, 055h
        db  000h, 016h, 0EDh, 0D8h, 0EEh, 024h, 000h, 000h, 000h, 000h, 0EEh, 048h, 0EAh, 04Dh, 0EEh, 09Ch
        db  0EDh, 0AAh, 0EAh, 024h, 0EEh, 0EAh, 055h, 055h, 000h, 016h, 0EDh, 092h, 0EDh, 0D8h, 0EEh, 032h
        db  0EDh, 0BCh, 000h, 0FFh, 008h, 000h, 0EDh, 0BCh, 000h, 0FFh, 00Ch, 000h, 0EDh, 0BCh, 000h, 0FFh
        db  010h, 000h, 0EDh, 0BCh, 000h, 0FFh, 014h, 000h, 0EDh, 0BCh, 000h, 0FFh, 018h, 000h, 0EDh, 0BCh
        db  000h, 0FFh, 01Ch, 000h, 0EDh, 0BCh, 000h, 0FFh, 020h, 000h, 0EDh, 0BCh, 000h, 0FFh, 024h, 000h
        db  0EDh, 0BCh, 000h, 0FFh, 028h, 000h, 0EDh, 0BCh, 000h, 0FFh, 02Ch, 000h, 0EEh, 064h, 0EDh, 092h
        db  0EDh, 09Eh, 0EEh, 002h, 030h, 040h, 000h, 006h, 001h, 000h, 0EEh, 032h, 0EFh, 06Eh, 020h, 000h
        db  0EDh, 092h, 0EDh, 09Eh, 0EEh, 002h, 070h, 040h, 000h, 006h, 043h, 000h, 0EEh, 032h, 0EFh, 06Eh
        db  060h, 000h, 0EDh, 092h, 0EDh, 09Eh, 0EEh, 002h, 0B0h, 040h, 000h, 006h, 085h, 000h, 0EEh, 032h
        db  0EFh, 06Eh, 0A0h, 000h, 0EFh, 06Ah, 001h, 000h, 001h, 003h, 001h, 001h, 000h, 000h, 081h, 000h
        db  000h, 01Ah, 010h, 040h, 055h, 055h, 055h, 055h, 055h, 055h, 055h, 055h, 055h, 055h, 055h, 055h
        db  082h, 072h, 0A9h, 04Ah, 05Ah, 0A5h, 0E7h, 0DAh, 009h, 003h, 09Dh, 011h, 000h, 000h, 081h, 000h
        db  000h, 004h, 090h, 0D8h, 010h, 000h, 000h, 000h, 081h, 000h, 000h, 004h, 090h, 0D8h, 0B4h, 0D8h
        db  000h, 000h, 081h, 000h, 000h, 008h, 090h, 0D8h, 016h, 046h, 040h, 000h, 0B4h, 0D8h, 000h, 008h
        db  000h, 000h, 080h, 000h, 000h, 013h, 010h, 040h, 000h, 016h, 000h, 000h, 080h, 000h, 000h, 010h
        db  000h, 015h, 000h, 000h, 080h, 000h, 000h, 00Fh, 000h, 015h, 000h, 000h, 081h, 000h, 000h, 00Fh
        db  000h, 006h, 000h, 000h, 080h, 000h, 000h, 012h, 080h, 00Ah, 09Eh, 040h, 0C8h, 000h, 000h, 000h
        db  080h, 05Eh, 000h, 00Fh, 080h, 006h, 0FEh, 040h, 0CCh, 000h, 000h, 000h, 080h, 004h, 08Eh, 040h
        db  0C9h, 000h, 080h, 004h, 006h, 000h, 0CCh, 000h, 080h, 004h, 00Ah, 040h, 0C8h, 000h, 080h, 00Ah
        db  08Ah, 040h, 0C8h, 000h, 000h, 000h, 080h, 05Eh, 000h, 00Fh, 008h, 00Ah, 01Ch, 080h, 000h, 00Ah
        db  01Ah, 01Ch, 080h, 000h, 00Ch, 01Ch, 080h, 000h, 01Ah, 01Ch, 080h, 000h, 00Eh, 01Ah, 01Ch, 080h
        db  000h, 004h, 080h, 000h, 002h, 080h, 000h, 002h, 080h, 000h, 000h, 000h, 0AAh, 050h, 000h, 02Dh
        db  000h, 000h, 000h, 000h, 0AAh, 07Ch, 002h, 0F6h, 007h, 058h, 0C4h, 036h, 0E0h, 016h, 008h, 094h
        db  008h, 09Eh, 008h, 0A8h, 0E0h, 016h, 008h, 0B2h, 008h, 0BCh, 007h, 0B8h, 0D7h, 0D8h, 002h, 080h
        db  007h, 0B8h, 0D7h, 0F6h, 0FFh, 084h, 007h, 0B8h, 0D8h, 018h, 007h, 0B8h, 0D8h, 092h, 0D9h, 072h
        db  0D9h, 066h, 0D9h, 08Ch, 0D9h, 090h, 0D9h, 088h, 0DAh, 070h, 0DAh, 070h, 0DAh, 070h, 0D9h, 092h
        db  0D9h, 09Ch, 0DAh, 018h, 0DAh, 062h, 0DAh, 070h, 0DAh, 070h, 0DAh, 070h, 0DAh, 070h, 000h, 007h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 008h, 001h, 008h, 008h, 005h, 008h, 008h, 008h, 000h
        db  000h, 002h, 004h, 004h, 004h, 004h, 000h, 008h, 00Ah, 000h, 000h, 00Ch, 010h, 000h, 000h, 000h
        db  000h, 000h, 041h, 004h, 00Bh, 006h, 0C2h, 008h, 0E6h, 000h, 0E7h, 000h, 006h, 004h, 007h, 004h
        db  003h, 004h, 004h, 006h, 005h, 004h, 088h, 004h, 000h, 003h, 000h, 01Ch, 00Ch, 000h, 080h, 000h
        db  0E2h, 0D0h, 0E2h, 0D8h, 0E3h, 01Ch, 0E2h, 0DCh, 0E2h, 0E8h, 0E2h, 0EEh, 0E3h, 012h, 0E2h, 0E2h
        db  0E3h, 030h, 006h, 000h, 000h, 000h, 007h, 006h, 00Eh, 014h, 014h, 01Eh, 02Ah, 04Ch, 038h, 07Ch
        db  03Eh, 078h, 043h, 092h, 058h, 09Ch, 04Dh, 0B2h, 02Fh, 064h, 01Dh, 034h, 06Ch, 0D0h, 023h, 03Eh
        db  01Dh, 02Ah, 065h, 0C0h, 072h, 0E0h, 078h, 0EEh, 07Fh, 0FCh, 001h, 032h, 057h, 001h, 011h, 032h
        db  051h, 081h, 056h, 002h, 055h, 003h, 011h, 054h, 081h, 056h, 002h, 055h, 002h, 054h, 081h, 056h
        db  076h, 001h, 034h, 002h, 055h, 002h, 054h, 081h, 058h, 002h, 055h, 002h, 054h, 081h, 058h, 002h
        db  012h, 011h, 052h, 002h, 083h, 058h, 005h, 052h, 004h, 081h, 058h, 005h, 052h, 003h, 015h, 05Ch
        db  05Bh, 053h, 087h, 052h, 003h, 011h, 051h, 041h, 051h, 078h, 011h, 034h, 011h, 081h, 031h, 020h
        db  057h, 054h, 053h, 001h, 012h, 05Ah, 051h, 081h, 031h, 020h, 057h, 05Bh, 05Ah, 001h, 011h, 001h
        db  011h, 051h, 081h, 031h, 020h, 057h, 001h, 015h, 001h, 013h, 001h, 011h, 011h, 011h, 051h, 081h
        db  058h, 005h, 052h, 002h, 054h, 05Bh, 081h, 05Dh, 005h, 052h, 002h, 054h, 081h, 058h, 002h, 050h
        db  003h, 013h, 081h, 058h, 002h, 050h, 003h, 011h, 054h, 081h, 05Dh, 072h, 003h, 050h, 003h, 013h
        db  001h, 013h, 054h, 040h, 000h, 00Eh, 006h, 020h, 006h, 056h, 0E3h, 0FEh, 000h, 024h, 010h, 002h
        db  000h, 016h, 000h, 002h, 004h, 001h, 007h, 008h, 0E3h, 0FEh, 000h, 020h, 010h, 003h, 000h, 012h
        db  000h, 003h, 0E3h, 04Ch, 08Eh, 014h, 000h, 020h, 010h, 004h, 000h, 012h, 000h, 004h, 0DAh, 0AAh
        db  000h, 020h, 0E0h, 005h, 000h, 012h, 000h, 005h, 0DAh, 0AAh, 000h, 020h, 060h, 006h, 000h, 012h
        db  000h, 006h, 0E5h, 0F6h, 000h, 012h, 060h, 001h, 009h, 01Ah, 006h, 0CCh, 000h, 004h, 000h, 007h
        db  000h, 000h, 006h, 030h, 0E5h, 016h, 005h, 0F0h, 060h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  005h, 0E2h, 000h, 008h, 0FFh, 026h, 005h, 0DCh, 0E6h, 082h, 000h, 03Eh, 060h, 000h, 0C0h, 000h
        db  000h, 000h, 000h, 002h, 000h, 030h, 050h, 020h, 00Ch, 023h, 005h, 0FCh, 006h, 052h, 006h, 056h
        db  000h, 000h, 081h, 000h, 000h, 016h, 060h, 000h, 0C0h, 000h, 000h, 000h, 000h, 010h, 000h, 008h
        db  040h, 02Ah, 004h, 02Ah, 006h, 056h, 000h, 026h, 0ECh, 063h, 006h, 02Bh, 009h, 020h, 000h, 022h
        db  000h, 024h, 0ECh, 079h, 00Ch, 023h, 005h, 0FCh, 008h, 028h, 008h, 0E6h, 008h, 029h, 007h, 058h
        db  007h, 078h, 007h, 098h, 000h, 023h, 000h, 02Ah, 0ECh, 087h, 004h, 006h, 006h, 0F0h, 004h, 007h
        db  006h, 0EEh, 000h, 024h, 0DAh, 0AAh, 000h, 034h, 060h, 000h, 0C0h, 000h, 000h, 000h, 000h, 010h
        db  000h, 026h, 040h, 025h, 0DAh, 0AAh, 000h, 020h, 060h, 000h, 0C0h, 000h, 000h, 000h, 000h, 010h
        db  000h, 012h, 040h, 026h, 0DAh, 0AAh, 000h, 01Ah, 060h, 000h, 000h, 00Ch, 060h, 027h, 008h, 00Ah
        db  006h, 0E6h, 0DAh, 0AAh, 000h, 024h, 060h, 000h, 000h, 016h, 060h, 028h, 004h, 030h, 007h, 006h
        db  0DBh, 000h, 081h, 000h, 000h, 030h, 060h, 000h, 000h, 022h, 060h, 029h, 008h, 02Dh, 007h, 01Ch
        db  008h, 02Eh, 007h, 022h, 000h, 000h, 002h, 008h, 001h, 006h, 006h, 014h, 008h, 018h, 00Ch, 020h
        db  00Eh, 026h, 00Fh, 030h, 011h, 034h, 012h, 03Eh, 014h, 042h, 016h, 046h, 00Ah, 01Ch, 018h, 04Ah
        db  003h, 013h, 083h, 011h, 011h, 001h, 081h, 011h, 081h, 012h, 001h, 013h, 083h, 052h, 085h, 081h
        db  011h, 085h, 081h, 012h, 081h, 012h, 081h, 019h, 085h, 060h, 0C0h, 000h, 000h, 000h, 000h, 008h
        db  009h, 01Ah, 000h, 000h, 006h, 030h, 0EBh, 0D0h, 006h, 054h, 006h, 050h, 002h, 038h, 004h, 021h
        db  008h, 0D0h, 006h, 00Bh, 006h, 0D8h, 008h, 002h, 006h, 0DCh, 0C0h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        db  000h, 041h, 000h, 041h, 0AEh, 014h, 000h, 000h, 081h, 000h, 004h, 009h, 007h, 00Ch, 000h, 041h
        db  000h, 041h, 002h, 014h, 000h, 000h, 081h, 000h, 006h, 00Bh, 006h, 0D8h, 006h, 02Ch, 009h, 024h
        db  014h, 022h, 008h, 0ECh, 000h, 041h, 000h, 041h, 002h, 054h, 000h, 000h, 081h, 000h, 006h, 0D8h
        db  084h, 000h, 048h, 000h, 0FFh, 0FCh, 000h, 009h, 0C0h, 000h, 000h, 000h, 000h, 010h, 000h, 000h
        db  0ADh, 072h, 002h, 08Fh, 000h, 000h, 000h, 000h, 0FFh, 0B8h, 000h, 048h, 028h, 043h, 029h, 031h
        db  039h, 038h, 033h, 02Dh, 038h, 039h, 02Ch, 039h, 030h, 02Ch, 031h, 02Ch, 032h, 020h, 054h, 065h
        db  078h, 061h, 073h, 020h, 049h, 06Eh, 073h, 074h, 072h, 075h, 06Dh, 065h, 06Eh, 074h, 073h, 00Ah
        db  028h, 043h, 029h, 031h, 039h, 038h, 033h, 02Ch, 034h, 02Ch, 035h, 02Ch, 036h, 020h, 049h, 042h
        db  04Dh, 020h, 043h, 06Fh, 072h, 070h, 000h, 000h, 000h, 000h, 000h, 000h, 0A0h, 034h, 0FFh, 0B8h
        db  000h, 080h, 0B0h, 03Eh, 000h, 000h, 0C0h, 038h, 000h, 036h, 003h, 000h, 000h, 002h, 0C8h, 00Bh
        db  001h, 06Ch, 0C2h, 08Bh, 001h, 060h, 009h, 018h, 000h, 004h, 016h, 00Dh, 001h, 060h, 0FCh, 002h
        db  000h, 020h, 016h, 009h, 002h, 08Bh, 000h, 043h, 013h, 006h, 001h, 060h, 007h, 058h, 002h, 000h
        db  016h, 002h, 004h, 060h, 0FEh, 072h, 006h, 0A0h, 0C0h, 0F0h, 003h, 090h, 000h, 07Fh, 003h, 080h
        db  000h, 000h, 0CDh, 004h, 00Ah, 088h, 010h, 01Bh, 045h, 04Fh, 020h, 056h, 032h, 032h, 033h, 02Eh
        db  031h, 031h, 02Eh, 030h, 030h, 020h, 020h, 028h, 043h, 029h, 020h, 043h, 06Fh, 070h, 079h, 072h
        db  069h, 067h, 068h, 074h, 020h, 04Fh, 06Ch, 069h, 063h, 06Fh, 06Dh, 020h, 031h, 039h, 039h, 034h
        db  02Eh, 000h, 001h, 060h, 007h, 058h, 014h, 000h, 016h, 0FCh, 004h, 060h, 0CEh, 00Ah, 007h, 089h
        db  014h, 08Eh, 0D2h, 0E0h, 009h, 032h, 009h, 08Bh, 003h, 000h, 000h, 002h, 02Dh, 081h, 0C0h, 041h
        db  013h, 019h, 010h, 007h, 02Dh, 081h, 0C0h, 041h, 013h, 00Fh, 0C8h, 00Ah, 001h, 06Ch, 0C8h, 001h
        db  0FCh, 000h, 0CEh, 041h, 0C8h, 001h, 001h, 06Ch, 0C2h, 081h, 004h, 0E0h, 0FCh, 004h, 007h, 0A0h
        db  0FCh, 002h, 080h, 000h, 006h, 00Bh, 016h, 0EEh, 0C8h, 020h, 001h, 06Ch, 008h, 02Ch, 0C8h, 020h
        db  001h, 06Ch, 008h, 042h, 0D8h, 020h, 009h, 032h, 008h, 0CFh, 007h, 08Fh, 0D2h, 004h, 0C3h, 08Eh
        db  011h, 002h, 007h, 08Fh, 0D1h, 034h, 0C8h, 00Fh, 008h, 050h, 007h, 08Fh, 0D2h, 080h, 088h, 00Fh
        db  000h, 016h, 013h, 003h, 0C8h, 020h, 008h, 050h, 000h, 016h, 003h, 000h, 000h, 00Fh, 004h, 020h
        db  0EAh, 07Ch, 001h, 020h, 0FCh, 000h, 00Fh, 0FFh, 013h, 010h, 001h, 0A0h, 0FCh, 000h, 0C0h, 000h
        db  0C0h, 060h, 0FCh, 000h, 0C8h, 001h, 001h, 06Ch, 0C1h, 041h, 004h, 0E0h, 0FCh, 004h, 007h, 0A0h
        db  0FCh, 002h, 080h, 000h, 005h, 0A0h, 010h, 082h, 010h, 0ECh, 001h, 043h, 080h, 000h, 016h, 022h
        db  004h, 0C3h, 003h, 000h, 000h, 002h, 001h, 060h, 007h, 058h, 040h, 000h, 013h, 009h, 007h, 0A0h
        db  001h, 004h, 0FFh, 08Ch, 001h, 0E0h, 007h, 058h, 040h, 000h, 003h, 000h, 000h, 00Fh, 010h, 012h
        db  001h, 0E0h, 000h, 0E0h, 000h, 002h, 003h, 000h, 000h, 00Fh, 010h, 00Ch, 003h, 000h, 000h, 00Fh
        db  0C0h, 086h, 002h, 042h, 000h, 07Fh, 0A0h, 082h, 0C8h, 005h, 001h, 06Ch, 001h, 020h, 0FCh, 000h
        db  00Fh, 0FFh, 016h, 0CBh, 088h, 006h, 010h, 086h, 013h, 0F9h, 0C1h, 022h, 014h, 08Eh, 0C8h, 004h
        db  001h, 06Ch, 0C0h, 060h, 0FCh, 000h, 011h, 041h, 003h, 000h, 000h, 002h, 0C2h, 08Ah, 016h, 014h
        db  0C8h, 004h, 001h, 08Ch, 0C2h, 0C4h, 0C3h, 004h, 0C8h, 00Ch, 001h, 06Ch, 001h, 020h, 0FCh, 000h
        db  00Fh, 0FFh, 013h, 003h, 0C3h, 020h, 0FCh, 000h, 010h, 0F7h, 005h, 086h, 007h, 08Ah, 000h, 001h
        db  001h, 084h, 07Fh, 0FFh, 0E2h, 084h, 010h, 0D2h, 005h, 086h, 0C8h, 009h, 001h, 06Ch, 013h, 014h
        db  0C8h, 004h, 0FCh, 000h, 001h, 0E0h, 0FCh, 000h, 040h, 000h, 0C2h, 044h, 001h, 084h, 07Fh, 0FFh
        db  0E1h, 0C4h, 0C8h, 009h, 001h, 06Ch, 001h, 020h, 0FCh, 000h, 00Fh, 0FFh, 013h, 003h, 0C2h, 060h
        db  0FCh, 000h, 010h, 0F7h, 005h, 087h, 010h, 0BAh, 0C2h, 004h, 0C2h, 044h, 001h, 084h, 07Fh, 0FFh
        db  0E1h, 0C4h, 0C8h, 009h, 001h, 06Ch, 001h, 020h, 0FCh, 000h, 00Fh, 0FFh, 013h, 003h, 0C2h, 060h
        db  0FCh, 000h, 010h, 0F7h, 007h, 087h, 000h, 001h, 010h, 0A9h, 0C0h, 0A0h, 0FCh, 004h, 002h, 022h
        db  0FCh, 006h, 0C0h, 012h, 0C8h, 001h, 001h, 06Ch, 0C8h, 000h, 0FCh, 004h, 003h, 000h, 000h, 002h
        db  0C2h, 08Ah, 016h, 032h, 0C8h, 004h, 001h, 08Ch, 0C2h, 0C4h, 0C3h, 020h, 0FCh, 000h, 005h, 086h
        db  007h, 08Ah, 000h, 001h, 001h, 084h, 07Fh, 0FFh, 0E2h, 084h, 003h, 000h, 000h, 00Fh, 0C3h, 020h
        db  0FCh, 000h, 011h, 005h, 001h, 00Ch, 00Fh, 0FFh, 016h, 0FAh, 004h, 060h, 0CEh, 00Ah, 0C0h, 0A0h
        db  0FCh, 004h, 002h, 022h, 0FCh, 006h, 0C0h, 032h, 013h, 0F8h, 001h, 000h, 0FFh, 0FCh, 013h, 005h
        db  0C8h, 00Ch, 001h, 06Ch, 0C8h, 000h, 0FCh, 004h, 010h, 0EAh, 0C0h, 092h, 0C8h, 00Ch, 001h, 06Ch
        db  0C8h, 002h, 0FCh, 006h, 0C8h, 000h, 0FCh, 004h, 007h, 0A0h, 0FCh, 000h, 040h, 000h, 007h, 0A0h
        db  0FCh, 002h, 081h, 000h, 004h, 060h, 0CEh, 00Ah, 005h, 086h, 0C8h, 009h, 001h, 06Ch, 013h, 00Ch
        db  0C8h, 004h, 0FCh, 000h, 001h, 0E0h, 0FCh, 000h, 040h, 000h, 0C8h, 001h, 001h, 06Ch, 005h, 087h
        db  001h, 084h, 07Fh, 0FFh, 0E1h, 0C4h, 010h, 008h, 0C2h, 004h, 0C8h, 001h, 001h, 06Ch, 007h, 087h
        db  000h, 001h, 001h, 084h, 07Fh, 0FFh, 0E1h, 0C4h, 0C2h, 060h, 0FCh, 000h, 003h, 000h, 000h, 00Fh
        db  0C0h, 020h, 0FCh, 000h, 011h, 001h, 010h, 0FCh, 003h, 000h, 000h, 002h, 0C2h, 049h, 013h, 0B5h
        db  0C2h, 040h, 0C0h, 0A0h, 0FCh, 004h, 002h, 022h, 0FCh, 006h, 0C0h, 032h, 013h, 017h, 001h, 000h
        db  0FFh, 0FCh, 013h, 007h, 0C8h, 009h, 001h, 06Ch, 0C8h, 000h, 0FCh, 004h, 003h, 000h, 000h, 00Fh
        db  010h, 0E7h, 0C0h, 092h, 0C8h, 009h, 001h, 06Ch, 0C8h, 002h, 0FCh, 006h, 0C8h, 000h, 0FCh, 004h
        db  007h, 0A0h, 0FCh, 000h, 040h, 000h, 007h, 0A0h, 0FCh, 002h, 081h, 000h, 004h, 060h, 0CEh, 00Ah
        db  0C2h, 084h, 0C3h, 00Ah, 0C8h, 00Ah, 001h, 06Ah, 001h, 020h, 0F8h, 002h, 081h, 000h, 013h, 00Fh
        db  001h, 020h, 0F8h, 002h, 0FEh, 000h, 016h, 012h, 0C2h, 0A0h, 0F8h, 000h, 001h, 00Ah, 00Fh, 0FFh
        db  016h, 0F0h, 0C1h, 060h, 001h, 06Ah, 004h, 0CAh, 001h, 04Bh, 000h, 001h, 004h, 05Bh, 0C2h, 0A0h
        db  0F8h, 000h, 001h, 00Ah, 00Fh, 0FFh, 016h, 0E6h, 006h, 0A0h, 0D1h, 0A2h, 0C2h, 0A0h, 0F8h, 002h
        db  011h, 009h, 001h, 00Ah, 002h, 000h, 016h, 053h, 001h, 00Ah, 014h, 000h, 013h, 05Dh, 001h, 04Ah
        db  010h, 000h, 016h, 027h, 004h, 0CAh, 0D2h, 0A0h, 007h, 02Ch, 002h, 08Ah, 0FEh, 000h, 013h, 008h
        db  002h, 08Ah, 07Fh, 000h, 013h, 005h, 0D2h, 0A0h, 007h, 020h, 002h, 08Ah, 07Fh, 000h, 016h, 00Bh
        db  001h, 0E0h, 006h, 0D2h, 008h, 000h, 001h, 0E0h, 006h, 034h, 008h, 000h, 004h, 0E0h, 006h, 030h
        db  001h, 0CBh, 000h, 001h, 010h, 00Eh, 0D2h, 0A0h, 007h, 02Ch, 002h, 02Ah, 001h, 000h, 018h, 002h
        db  0D8h, 00Ah, 007h, 02Ch, 0D2h, 0A0h, 007h, 020h, 002h, 02Ah, 001h, 000h, 018h, 002h, 0D8h, 00Ah
        db  007h, 020h, 0C8h, 00Ch, 001h, 06Ah, 007h, 0A0h, 0F8h, 002h, 080h, 000h, 001h, 060h, 0F8h, 000h
        db  040h, 000h, 016h, 004h, 007h, 0A0h, 0F8h, 002h, 081h, 000h, 010h, 013h, 001h, 020h, 0F8h, 000h
        db  00Fh, 0FFh, 016h, 006h, 001h, 060h, 0F8h, 000h, 040h, 000h, 013h, 00Bh, 006h, 0A0h, 0D1h, 0A2h
        db  0C2h, 0A0h, 0F8h, 004h, 013h, 006h, 0C8h, 020h, 0F8h, 000h, 001h, 06Ah, 010h, 0E4h, 006h, 0A0h
        db  0D1h, 0A2h, 007h, 08Ah, 001h, 08Ch, 0C6h, 08Ch, 001h, 04Bh, 000h, 001h, 004h, 05Bh, 001h, 04Ah
        db  006h, 000h, 013h, 00Ah, 001h, 0E0h, 006h, 0D2h, 002h, 000h, 001h, 0E0h, 006h, 034h, 008h, 000h
        db  004h, 0E0h, 006h, 030h, 001h, 0CBh, 000h, 001h, 001h, 020h, 0F8h, 000h, 040h, 000h, 013h, 0C9h
        db  0D2h, 0A0h, 0E0h, 012h, 0C8h, 00Ah, 0F8h, 002h, 0C3h, 020h, 0F8h, 000h, 001h, 00Ch, 00Fh, 0FFh
        db  013h, 008h, 0C8h, 00Ch, 001h, 06Ah, 0C2h, 0A0h, 0F8h, 002h, 011h, 0DBh, 0C2h, 08Ch, 004h, 060h
        db  0CFh, 0C2h, 0C1h, 060h, 001h, 06Ah, 004h, 0CAh, 001h, 04Bh, 000h, 001h, 004h, 05Bh, 0C2h, 08Ah
        db  016h, 057h, 007h, 08Ah, 001h, 08Ch, 0C0h, 082h, 013h, 00Dh, 0C6h, 081h, 0C8h, 009h, 001h, 06Ah
        db  0C8h, 004h, 0F8h, 000h, 0C2h, 045h, 0E8h, 003h, 008h, 028h, 0C0h, 0C0h, 0C1h, 001h, 0C1h, 042h
        db  004h, 0C2h, 010h, 046h, 0C8h, 009h, 001h, 06Ah, 0C8h, 004h, 0F8h, 000h, 0C2h, 045h, 0E8h, 003h
        db  008h, 028h, 004h, 0C3h, 010h, 03Dh, 003h, 000h, 000h, 002h, 086h, 08Ah, 001h, 060h, 001h, 09Ch
        db  040h, 000h, 016h, 03Eh, 001h, 003h, 000h, 0FFh, 013h, 03Bh, 0C8h, 005h, 001h, 06Ah, 001h, 020h
        db  0F8h, 002h, 0FFh, 0FFh, 013h, 004h, 001h, 020h, 0F8h, 002h, 0FEh, 000h, 013h, 007h, 006h, 0A0h
        db  0CFh, 0BEh, 013h, 0CDh, 0C2h, 08Ah, 016h, 010h, 007h, 08Ah, 001h, 08Ch, 0C0h, 082h, 013h, 00Fh
        db  0C6h, 081h, 0C8h, 009h, 001h, 06Ah, 0C8h, 004h, 0F8h, 000h, 0C2h, 045h, 0E8h, 003h, 008h, 028h
        db  0C0h, 0C0h, 0C1h, 001h, 0C1h, 042h, 004h, 0C2h, 003h, 090h, 000h, 01Fh, 003h, 080h, 0C8h, 009h
        db  001h, 06Ah, 0C8h, 004h, 0F8h, 000h, 0C2h, 045h, 0E8h, 003h, 008h, 028h, 004h, 0C3h, 003h, 090h
        db  000h, 01Fh, 003h, 080h, 0C8h, 00Bh, 008h, 044h, 002h, 0E0h, 008h, 030h, 004h, 060h, 0B3h, 07Ch
        db  007h, 08Bh, 000h, 0DAh, 0CEh, 0CDh, 0CEh, 0CEh, 0C6h, 0CFh, 002h, 0E0h, 000h, 0C0h, 004h, 054h
        db  003h, 090h, 0FFh, 0FFh, 003h, 080h, 0C2h, 08Ah, 016h, 0F3h, 007h, 08Ah, 001h, 08Ch, 0C0h, 082h
        db  013h, 00Fh, 0C6h, 081h, 0C8h, 009h, 001h, 06Ah, 0C8h, 004h, 0F8h, 000h, 0C2h, 045h, 0E8h, 003h
        db  008h, 028h, 006h, 0A0h, 0F7h, 0FEh, 0C0h, 0C0h, 0C1h, 001h, 0C1h, 042h, 004h, 0C2h, 010h, 0E0h
        db  0C8h, 009h, 001h, 06Ah, 0C8h, 004h, 0F8h, 000h, 0C2h, 045h, 0E8h, 003h, 008h, 028h, 006h, 0A0h
        db  0F7h, 0FEh, 004h, 0C3h, 010h, 0D5h, 003h, 000h, 000h, 002h, 086h, 08Ah, 001h, 060h, 001h, 09Ch
        db  040h, 000h, 016h, 0D6h, 001h, 003h, 000h, 0FFh, 013h, 0D3h, 0C8h, 005h, 001h, 06Ah, 001h, 020h
        db  0F8h, 002h, 0FFh, 0FFh, 013h, 004h, 001h, 020h, 0F8h, 002h, 0FEh, 000h, 013h, 007h, 006h, 0A0h
        db  0CFh, 0BEh, 013h, 0C9h, 0C2h, 08Ah, 016h, 0C4h, 007h, 08Ah, 001h, 08Ch, 0C0h, 082h, 013h, 011h
        db  0C6h, 081h, 0C8h, 009h, 001h, 06Ah, 0C8h, 004h, 0F8h, 000h, 0C2h, 045h, 0E8h, 003h, 008h, 028h
        db  006h, 0A0h, 0F7h, 0FEh, 0C0h, 0C0h, 0C1h, 001h, 0C1h, 042h, 004h, 0C2h, 003h, 090h, 000h, 01Fh
        db  003h, 080h, 0C8h, 009h, 001h, 06Ah, 0C8h, 004h, 0F8h, 000h, 0C2h, 045h, 0E8h, 003h, 008h, 028h
        db  006h, 0A0h, 0F7h, 0FEh, 004h, 0C3h, 003h, 090h, 000h, 01Fh, 003h, 080h, 003h, 090h, 0FFh, 0FFh
        db  003h, 080h, 003h, 000h, 000h, 002h, 0C2h, 020h, 001h, 08Ch, 001h, 060h, 001h, 09Ch, 040h, 000h
        db  016h, 0F5h, 0C0h, 0E0h, 001h, 06Ch, 0C2h, 020h, 005h, 0F4h, 013h, 002h, 004h, 060h, 0B8h, 0C2h
        db  002h, 0E0h, 008h, 030h, 001h, 003h, 000h, 0FFh, 013h, 029h, 001h, 043h, 004h, 000h, 013h, 026h
        db  0C8h, 005h, 001h, 06Ah, 001h, 020h, 0F8h, 002h, 0FFh, 0FFh, 013h, 004h, 001h, 020h, 0F8h, 002h
        db  0FEh, 000h, 013h, 00Ch, 006h, 0A0h, 0CFh, 0BEh, 016h, 005h, 001h, 0E0h, 000h, 0E6h, 080h, 000h
        db  005h, 0A0h, 016h, 038h, 0C2h, 08Ah, 016h, 036h, 007h, 08Ah, 001h, 08Ch, 0C8h, 009h, 001h, 06Ah
        db  0C8h, 004h, 0F8h, 000h, 0C2h, 045h, 0E8h, 003h, 008h, 028h, 0C1h, 060h, 008h, 050h, 002h, 085h
        db  0D2h, 004h, 016h, 002h, 006h, 0A0h, 0F7h, 0FEh, 002h, 043h, 003h, 000h, 002h, 0E0h, 000h, 0E0h
        db  001h, 020h, 008h, 036h, 001h, 000h, 016h, 01Ch, 0C2h, 020h, 005h, 0F0h, 013h, 002h, 004h, 060h
        db  0B9h, 024h, 002h, 0E0h, 008h, 030h, 001h, 083h, 006h, 000h, 016h, 005h, 0C1h, 042h, 013h, 008h
        db  0C0h, 0C0h, 0C1h, 001h, 004h, 0C2h, 0D8h, 020h, 008h, 0E0h, 001h, 080h, 0C8h, 004h, 001h, 08Ch
        db  002h, 0E0h, 000h, 0E0h, 007h, 0A0h, 000h, 014h, 008h, 030h, 0C8h, 020h, 008h, 050h, 000h, 016h
        db  004h, 060h, 0B9h, 02Ch, 002h, 0E0h, 000h, 0E0h, 004h, 060h, 0B9h, 02Ch, 003h, 000h, 000h, 002h
        db  0C0h, 0E0h, 001h, 06Ch, 0C3h, 020h, 001h, 08Ah, 001h, 060h, 001h, 09Ch, 000h, 040h, 016h, 058h
        db  001h, 020h, 009h, 018h, 000h, 006h, 016h, 004h, 001h, 0A0h, 001h, 082h, 000h, 006h, 010h, 003h
        db  001h, 0E0h, 001h, 082h, 000h, 006h, 0C8h, 001h, 001h, 08Ah, 0C8h, 001h, 008h, 0CAh, 007h, 0A0h
        db  0E1h, 09Eh, 0B5h, 04Eh, 007h, 0A0h, 000h, 012h, 0D4h, 02Ch, 0C2h, 0E0h, 00Ch, 002h, 013h, 002h
        db  004h, 060h, 0B5h, 09Ch, 007h, 0A0h, 00Ch, 002h, 080h, 000h, 010h, 03Ah, 0C2h, 0E0h, 000h, 012h
        db  002h, 08Bh, 0D4h, 02Ch, 016h, 035h, 004h, 060h, 0D4h, 0A2h, 0C2h, 0E0h, 000h, 012h, 002h, 08Bh
        db  0D4h, 02Ch, 016h, 02Eh, 004h, 060h, 0D4h, 0CAh, 0C0h, 041h, 013h, 002h, 006h, 0A0h, 0FCh, 0BAh
        db  004h, 060h, 0D5h, 076h, 0C0h, 041h, 013h, 008h, 0C8h, 001h, 001h, 06Ch, 0C2h, 0E0h, 0FCh, 002h
        db  011h, 003h, 004h, 0CCh, 004h, 060h, 0D4h, 0C2h, 001h, 060h, 001h, 09Ch, 000h, 040h, 016h, 0ECh
        db  0C3h, 001h, 013h, 014h, 001h, 020h, 009h, 018h, 000h, 006h, 016h, 004h, 001h, 0A0h, 001h, 082h
        db  000h, 006h, 010h, 003h, 001h, 0E0h, 001h, 082h, 000h, 006h, 0C8h, 001h, 001h, 08Ah, 0C8h, 001h
        db  008h, 0CAh, 007h, 0A0h, 000h, 02Ah, 0B4h, 066h, 004h, 060h, 0B5h, 09Ch, 004h, 060h, 0B5h, 082h
        db  004h, 060h, 0D5h, 076h, 004h, 060h, 0B5h, 0F2h, 001h, 000h, 0C0h, 000h, 016h, 002h, 004h, 060h
        db  0D5h, 0FAh, 004h, 060h, 0B6h, 0C4h, 004h, 060h, 0D6h, 07Eh, 004h, 060h, 0D6h, 0CEh, 003h, 000h
        db  000h, 002h, 0C0h, 0E0h, 001h, 06Ch, 0C1h, 0E0h, 010h, 08Ch, 088h, 007h, 010h, 088h, 013h, 032h
        db  0C2h, 007h, 005h, 087h, 002h, 048h, 001h, 0FFh, 0A2h, 008h, 0C2h, 028h, 010h, 08Eh, 0C2h, 0C8h
        db  0C8h, 008h, 001h, 06Ch, 0C8h, 004h, 0FCh, 004h, 007h, 0A0h, 0FCh, 002h, 080h, 000h, 0A2h, 008h
        db  0CAh, 00Ah, 00Ch, 032h, 0C2h, 0A0h, 001h, 06Ch, 0C2h, 020h, 0FCh, 000h, 016h, 0F1h, 088h, 007h
        db  010h, 088h, 013h, 00Ch, 0C2h, 007h, 005h, 087h, 002h, 048h, 001h, 0FFh, 0A2h, 008h, 0C2h, 028h
        db  010h, 08Eh, 0C8h, 008h, 0FCh, 000h, 010h, 0E4h, 0C0h, 04Bh, 010h, 009h, 0C0h, 041h, 013h, 0FCh
        db  0C8h, 002h, 001h, 06Ch, 0C8h, 00Bh, 0FCh, 000h, 0A2h, 0CBh, 0CAh, 0C2h, 00Ch, 032h, 0C0h, 08Ah
        db  0C8h, 007h, 010h, 08Ch, 0C3h, 020h, 001h, 08Ah, 001h, 020h, 001h, 082h, 000h, 006h, 013h, 00Ah
        db  001h, 060h, 001h, 082h, 000h, 004h, 013h, 006h, 004h, 060h, 0D3h, 0C2h, 001h, 080h, 000h, 020h
        db  004h, 060h, 0B9h, 038h, 001h, 060h, 001h, 09Ch, 000h, 040h, 013h, 0A4h, 083h, 001h, 013h, 054h
        db  001h, 040h, 000h, 040h, 016h, 0A1h, 0C1h, 041h, 004h, 0C6h, 0C8h, 001h, 001h, 06Ch, 0C2h, 020h
        db  0FCh, 002h, 0D0h, 020h, 0FCh, 007h, 028h, 020h, 0E0h, 006h, 001h, 000h, 0C0h, 000h, 016h, 09Bh
        db  088h, 001h, 008h, 0CAh, 013h, 09Ah, 001h, 048h, 001h, 000h, 016h, 05Dh, 026h, 020h, 016h, 034h
        db  013h, 043h, 001h, 008h, 05Eh, 088h, 016h, 042h, 068h, 004h, 0FCh, 004h, 005h, 020h, 0FCh, 004h
        db  0C1h, 081h, 0C0h, 060h, 0FCh, 000h, 083h, 001h, 016h, 0E0h, 006h, 020h, 016h, 032h, 016h, 00Dh
        db  0C3h, 020h, 001h, 084h, 001h, 0A0h, 001h, 082h, 020h, 000h, 001h, 0E0h, 001h, 082h, 020h, 000h
        db  0C8h, 00Ch, 001h, 084h, 007h, 0A0h, 016h, 032h, 000h, 032h, 0C3h, 020h, 001h, 08Ah, 013h, 005h
        db  002h, 08Ch, 000h, 043h, 013h, 002h, 083h, 001h, 016h, 0C8h, 0C1h, 086h, 013h, 015h, 0C2h, 020h
        db  010h, 080h, 002h, 048h, 001h, 0FFh, 0A2h, 008h, 0CAh, 005h, 010h, 08Eh, 004h, 0E0h, 0FCh, 000h
        db  005h, 0A0h, 010h, 080h, 001h, 060h, 007h, 058h, 040h, 000h, 013h, 006h, 007h, 0A0h, 001h, 004h
        db  0FFh, 08Ch, 001h, 0E0h, 007h, 058h, 040h, 000h, 0C8h, 003h, 001h, 06Ch, 001h, 040h, 000h, 020h
        db  013h, 09Dh, 003h, 090h, 011h, 0FFh, 003h, 080h, 004h, 060h, 0D6h, 0D8h, 001h, 008h, 05Eh, 000h
        db  016h, 077h, 001h, 048h, 000h, 001h, 016h, 0B8h, 0C2h, 060h, 0FCh, 008h, 011h, 0B5h, 0C2h, 060h
        db  0FCh, 00Eh, 013h, 004h, 015h, 003h, 0C2h, 060h, 0FCh, 014h, 011h, 0AEh, 007h, 08Ah, 000h, 080h
        db  006h, 0A0h, 0B5h, 01Eh, 010h, 0A9h, 0C2h, 041h, 0C0h, 060h, 0FCh, 000h, 0C1h, 086h, 013h, 017h
        db  0C2h, 020h, 010h, 080h, 002h, 048h, 001h, 0FFh, 0A2h, 008h, 0CAh, 005h, 010h, 08Eh, 0C8h, 006h
        db  001h, 06Ch, 004h, 0E0h, 0FCh, 000h, 005h, 0A0h, 010h, 080h, 001h, 060h, 007h, 058h, 040h, 000h
        db  013h, 006h, 007h, 0A0h, 001h, 004h, 0FFh, 08Ch, 001h, 0E0h, 007h, 058h, 040h, 000h, 0C8h, 004h
        db  008h, 01Eh, 0C1h, 049h, 001h, 080h, 000h, 040h, 083h, 001h, 013h, 0BEh, 0C8h, 001h, 001h, 06Ch
        db  0C2h, 020h, 0FCh, 002h, 001h, 048h, 001h, 000h, 016h, 05Bh, 001h, 0C0h, 000h, 040h, 026h, 020h
        db  016h, 034h, 013h, 003h, 001h, 000h, 000h, 004h, 013h, 004h, 001h, 0C8h, 040h, 000h, 001h, 080h
        db  000h, 005h, 001h, 008h, 05Eh, 088h, 013h, 019h, 001h, 008h, 05Eh, 000h, 016h, 044h, 001h, 048h
        db  000h, 001h, 016h, 013h, 0C8h, 005h, 001h, 06Ch, 0C2h, 060h, 0FCh, 008h, 011h, 006h, 0C2h, 060h
        db  0FCh, 00Eh, 013h, 003h, 015h, 002h, 0C2h, 060h, 0FCh, 014h, 0C8h, 001h, 001h, 06Ch, 0C2h, 049h
        db  011h, 004h, 007h, 08Ah, 000h, 080h, 006h, 0A0h, 0B5h, 01Eh, 0C1h, 081h, 0C0h, 060h, 0FCh, 000h
        db  0C2h, 020h, 0FCh, 004h, 062h, 004h, 005h, 008h, 0C8h, 008h, 0FCh, 004h, 0A2h, 020h, 008h, 01Eh
        db  0C8h, 005h, 001h, 06Ch, 0C8h, 008h, 0FCh, 004h, 0C8h, 006h, 001h, 06Ch, 004h, 060h, 0D5h, 014h
        db  0C1h, 086h, 013h, 017h, 0C2h, 020h, 010h, 080h, 002h, 048h, 001h, 0FFh, 0A2h, 008h, 0CAh, 005h
        db  010h, 08Eh, 0C8h, 006h, 001h, 06Ch, 004h, 0E0h, 0FCh, 000h, 005h, 0A0h, 010h, 080h, 001h, 060h
        db  007h, 058h, 040h, 000h, 013h, 006h, 007h, 0A0h, 001h, 004h, 0FFh, 08Ch, 001h, 0E0h, 007h, 058h
        db  040h, 000h, 004h, 060h, 0B6h, 0C4h, 0C1h, 081h, 0C0h, 060h, 0FCh, 000h, 004h, 060h, 0B8h, 086h
        db  0A8h, 004h, 008h, 01Eh, 0C0h, 060h, 0FCh, 000h, 083h, 001h, 016h, 098h, 004h, 060h, 0D5h, 076h
        db  004h, 0E0h, 008h, 0CAh, 001h, 048h, 001h, 000h, 016h, 022h, 0C2h, 001h, 0C0h, 060h, 0FCh, 000h
        db  004h, 0E0h, 0FCh, 000h, 007h, 0A0h, 0FCh, 002h, 080h, 000h, 0C8h, 004h, 0FCh, 004h, 0C0h, 041h
        db  013h, 01Ah, 0C3h, 00Ch, 016h, 001h, 0C3h, 008h, 0C8h, 002h, 001h, 06Ch, 0C8h, 008h, 0FCh, 000h
        db  0A2h, 008h, 0CAh, 002h, 00Ch, 032h, 0C0h, 0A0h, 0FCh, 000h, 0C8h, 006h, 001h, 06Ch, 016h, 003h
        db  0C1h, 041h, 004h, 060h, 0D5h, 014h, 0C8h, 001h, 0FCh, 000h, 004h, 060h, 0D5h, 014h, 001h, 0C0h
        db  000h, 004h, 004h, 060h, 0D5h, 0B4h, 0C0h, 048h, 007h, 08Bh, 0D5h, 048h, 004h, 060h, 0FCh, 0B8h
        db  001h, 060h, 007h, 058h, 002h, 000h, 013h, 002h, 004h, 060h, 0B8h, 0A2h, 0A8h, 020h, 0E0h, 01Eh
        db  0FCh, 004h, 0C2h, 020h, 0FCh, 004h, 0C8h, 005h, 001h, 06Ch, 010h, 004h, 0A2h, 004h, 0C8h, 020h
        db  0FCh, 000h, 001h, 06Ch, 001h, 060h, 0FCh, 002h, 001h, 000h, 016h, 0F8h, 0C8h, 005h, 001h, 06Ch
        db  0C8h, 008h, 0FCh, 004h, 0C2h, 020h, 010h, 080h, 002h, 048h, 001h, 0FFh, 0A2h, 008h, 0CAh, 005h
        db  010h, 08Eh, 005h, 0A0h, 010h, 080h, 001h, 060h, 007h, 058h, 040h, 000h, 013h, 006h, 007h, 0A0h
        db  001h, 004h, 0FFh, 08Ch, 001h, 0E0h, 007h, 058h, 040h, 000h, 004h, 060h, 0B5h, 066h, 000h, 000h
        db  0F6h, 0F8h, 007h, 0FAh, 0A8h, 0C3h, 0A9h, 0A0h, 0CFh, 0ECh, 0E9h, 0E3h, 0EFh, 0EDh, 0A0h, 0B1h
        db  0B9h, 0B9h, 0B4h, 000h, 000h, 000h, 002h, 00Dh, 010h, 000h, 020h, 00Dh, 016h, 00Fh, 002h, 00Dh
        db  008h, 000h, 001h, 040h, 004h, 000h, 013h, 00Ah, 002h, 00Dh, 004h, 000h, 001h, 040h, 002h, 000h
        db  016h, 005h, 002h, 00Dh, 040h, 000h, 001h, 04Fh, 000h, 001h, 016h, 001h, 004h, 05Bh, 001h, 0C0h
        db  004h, 000h, 001h, 085h, 000h, 040h, 004h, 020h, 0EAh, 07Ch, 003h, 000h, 000h, 002h, 0C2h, 060h
        db  010h, 08Ch, 088h, 009h, 010h, 088h, 013h, 041h, 002h, 0E0h, 000h, 0E0h, 0C0h, 0E0h, 001h, 06Ch
        db  0C1h, 0E0h, 010h, 08Ch, 0C2h, 007h, 005h, 087h, 002h, 048h, 001h, 0FFh, 0A2h, 008h, 0C2h, 028h
        db  010h, 08Eh, 0C2h, 0C8h, 0C8h, 008h, 001h, 06Ch, 0C8h, 004h, 0FCh, 004h, 007h, 0A0h, 0FCh, 002h
        db  080h, 000h, 0A2h, 008h, 0CAh, 00Ah, 00Ch, 032h, 0C2h, 0A0h, 001h, 06Ch, 0C2h, 020h, 0FCh, 000h
        db  016h, 0F1h, 088h, 007h, 010h, 088h, 013h, 00Ah, 0C2h, 007h, 005h, 087h, 002h, 048h, 001h, 0FFh
        db  0A2h, 008h, 0C2h, 028h, 010h, 08Eh, 0C8h, 008h, 0FCh, 000h, 010h, 0E4h, 0C0h, 041h, 013h, 009h
        db  0C8h, 002h, 001h, 06Ch, 0C8h, 00Bh, 0FCh, 000h, 0A2h, 0CBh, 0CAh, 0C2h, 00Ch, 032h, 0C0h, 08Ah
        db  010h, 002h, 0C0h, 08Ah, 0C0h, 04Bh, 006h, 0A0h, 0FCh, 0BAh, 0C8h, 007h, 010h, 08Ch, 0C8h, 003h
        db  001h, 06Ch, 002h, 0E0h, 000h, 0C0h, 004h, 060h, 0B4h, 06Ah, 004h, 060h, 0B4h, 06Ah, 007h, 0A0h
        db  000h, 02Ah, 0F7h, 03Eh, 007h, 0A0h, 001h, 08Ah, 000h, 043h, 004h, 060h, 0B6h, 062h, 007h, 0A0h
        db  000h, 02Ah, 0F7h, 03Eh, 0C8h, 00Ah, 001h, 08Ah, 004h, 05Bh, 007h, 0A0h, 000h, 02Ah, 0B4h, 066h
        db  007h, 0A0h, 00Ch, 002h, 080h, 000h, 004h, 060h, 0C1h, 076h, 0C8h, 004h, 001h, 06Ah, 010h, 003h
        db  0C8h, 020h, 0F8h, 000h, 001h, 06Ah, 0C3h, 020h, 0F8h, 002h, 001h, 04Ch, 001h, 000h, 016h, 0F8h
        db  0C1h, 060h, 010h, 084h, 002h, 045h, 000h, 07Fh, 006h, 0CCh, 0D9h, 04Ch, 015h, 08Eh, 013h, 00Dh
        db  005h, 0A0h, 010h, 084h, 006h, 003h, 001h, 003h, 000h, 0FFh, 013h, 052h, 0C1h, 020h, 0F8h, 000h
        db  0C8h, 004h, 001h, 06Ah, 016h, 0E8h, 006h, 0A0h, 0D1h, 0A2h, 001h, 00Ch, 000h, 0FEh, 016h, 0F0h
        db  0C3h, 020h, 001h, 06Ah, 0C8h, 004h, 001h, 06Ah, 001h, 020h, 0F8h, 00Eh, 080h, 000h, 013h, 007h
        db  0C1h, 020h, 0F8h, 014h, 001h, 084h, 060h, 0FFh, 002h, 084h, 002h, 000h, 01Bh, 033h, 0C1h, 020h
        db  0F8h, 008h, 011h, 00Ch, 088h, 020h, 0F8h, 00Ch, 001h, 0A4h, 016h, 02Fh, 088h, 020h, 0F8h, 00Ah
        db  001h, 0A2h, 016h, 02Bh, 088h, 004h, 001h, 0A0h, 013h, 025h, 010h, 027h, 0C1h, 020h, 0F8h, 00Ah
        db  011h, 009h, 025h, 020h, 001h, 0ACh, 016h, 01Eh, 0C1h, 020h, 0F8h, 00Ch, 025h, 020h, 001h, 0AEh
        db  016h, 019h, 010h, 01Bh, 002h, 084h, 0FFh, 0FFh, 016h, 00Ah, 088h, 004h, 0F8h, 00Ch, 016h, 007h
        db  041h, 020h, 0F8h, 008h, 013h, 00Fh, 002h, 084h, 03Fh, 0FFh, 013h, 00Ch, 010h, 00Eh, 088h, 004h
        db  001h, 0A8h, 016h, 00Bh, 088h, 020h, 0F8h, 00Ch, 001h, 0AAh, 016h, 007h, 088h, 020h, 0F8h, 008h
        db  001h, 0A6h, 016h, 003h, 0D9h, 060h, 0EEh, 000h, 015h, 08Eh, 0C8h, 00Ch, 001h, 06Ah, 010h, 0A8h
        db  004h, 05Bh, 003h, 000h, 000h, 002h, 0C2h, 00Bh, 0C3h, 020h, 005h, 0F4h, 016h, 008h, 0C3h, 020h
        db  008h, 036h, 001h, 00Ch, 000h, 0FFh, 013h, 01Ah, 001h, 04Ch, 004h, 000h, 013h, 017h, 0C2h, 0A8h
        db  000h, 00Ah, 001h, 04Ah, 000h, 010h, 016h, 03Eh, 001h, 04Ah, 080h, 000h, 016h, 019h, 0C3h, 008h
        db  006h, 0A0h, 0F9h, 098h, 0C2h, 00Ch, 0C3h, 020h, 005h, 0F4h, 016h, 012h, 0C3h, 020h, 008h, 036h
        db  001h, 00Ch, 000h, 0FFh, 013h, 003h, 001h, 04Ch, 004h, 000h, 016h, 00Ah, 001h, 068h, 000h, 00Ah
        db  000h, 010h, 013h, 003h, 001h, 00Ch, 001h, 000h, 016h, 025h, 006h, 0A0h, 0BBh, 0A4h, 010h, 004h
        db  007h, 087h, 005h, 0F0h, 006h, 0A0h, 0C1h, 0C6h, 001h, 0E0h, 008h, 036h, 002h, 000h, 007h, 0A0h
        db  000h, 014h, 000h, 0E0h, 007h, 0A0h, 000h, 016h, 0D2h, 080h, 003h, 090h, 0FFh, 0FFh, 003h, 080h
        db  003h, 000h, 000h, 002h, 0C2h, 020h, 008h, 036h, 016h, 00Ch, 0D8h, 020h, 008h, 0E0h, 001h, 080h
        db  0C8h, 02Bh, 000h, 006h, 001h, 08Ch, 007h, 0A0h, 008h, 0C8h, 000h, 004h, 0C8h, 00Bh, 005h, 0F4h
        db  010h, 005h, 0C2h, 00Bh, 007h, 087h, 005h, 0F0h, 006h, 0A0h, 0C1h, 080h, 001h, 0E0h, 008h, 036h
        db  002h, 000h, 007h, 0A0h, 000h, 014h, 000h, 0E0h, 007h, 0A0h, 000h, 016h, 0D2h, 080h, 003h, 090h
        db  0FFh, 0FFh, 003h, 080h, 0C2h, 020h, 005h, 0F4h, 013h, 002h, 004h, 060h, 0BBh, 068h, 002h, 0A8h
        db  0C8h, 008h, 008h, 04Ah, 002h, 0E0h, 008h, 030h, 001h, 003h, 000h, 0FFh, 013h, 060h, 001h, 043h
        db  004h, 000h, 013h, 05Dh, 001h, 0E0h, 001h, 09Ch, 040h, 000h, 00Bh, 0ABh, 00Bh, 0ABh, 001h, 060h
        db  001h, 09Ch, 040h, 000h, 016h, 054h, 0C8h, 004h, 001h, 06Ah, 0C2h, 0E0h, 0F8h, 002h, 011h, 04Ah
        db  0C3h, 084h, 0C2h, 083h, 004h, 0C3h, 004h, 0CFh, 0C3h, 00Eh, 001h, 060h, 0F8h, 000h, 040h, 000h
        db  013h, 009h, 001h, 060h, 0F8h, 002h, 080h, 000h, 013h, 013h, 0C3h, 020h, 0F8h, 000h, 0C8h, 00Ch
        db  001h, 06Ah, 010h, 0F3h, 005h, 083h, 006h, 00Ah, 0C3h, 0CCh, 0C3h, 0A0h, 0F8h, 000h, 001h, 00Eh
        db  00Fh, 0FFh, 013h, 006h, 0C8h, 00Eh, 001h, 06Ah, 001h, 060h, 0F8h, 002h, 080h, 000h, 016h, 0E3h
        db  0C8h, 00Fh, 001h, 06Ah, 013h, 02Eh, 0C8h, 020h, 0F8h, 000h, 001h, 06Ah, 001h, 060h, 0F8h, 002h
        db  080h, 000h, 016h, 004h, 007h, 0A0h, 0F8h, 002h, 080h, 000h, 010h, 0F5h, 0C8h, 00Fh, 001h, 06Ah
        db  007h, 0A0h, 0F8h, 000h, 040h, 000h, 0C8h, 009h, 001h, 06Ah, 0C8h, 004h, 0F8h, 000h, 0C2h, 04Ch
        db  0C3h, 0C5h, 0E8h, 003h, 008h, 028h, 0C1h, 060h, 008h, 050h, 002h, 085h, 0D2h, 004h, 016h, 002h
        db  006h, 0A0h, 0F7h, 0FEh, 0C0h, 0CAh, 0C1h, 00Eh, 0C1h, 04Fh, 007h, 08Ah, 001h, 08Ch, 001h, 003h
        db  000h, 0FFh, 013h, 003h, 001h, 0C3h, 004h, 000h, 010h, 002h, 002h, 043h, 003h, 000h, 000h, 09Dh
        db  004h, 05Bh, 0C8h, 004h, 001h, 06Ah, 0C8h, 020h, 0F8h, 000h, 001h, 06Ah, 001h, 060h, 0F8h, 002h
        db  080h, 000h, 016h, 0F0h, 007h, 0A0h, 0F8h, 002h, 080h, 000h, 010h, 0F5h, 003h, 000h, 000h, 002h
        db  007h, 087h, 005h, 0F0h, 0C2h, 0CBh, 016h, 00Fh, 006h, 0A0h, 0F9h, 098h, 001h, 0E0h, 008h, 036h
        db  001h, 000h, 007h, 0A0h, 000h, 014h, 000h, 0E0h, 007h, 0A0h, 000h, 016h, 0D2h, 080h, 0CBh, 060h
        db  005h, 0F4h, 000h, 002h, 003h, 080h, 0C2h, 020h, 008h, 036h, 001h, 088h, 001h, 000h, 0C8h, 008h
        db  008h, 036h, 001h, 008h, 004h, 000h, 016h, 003h, 001h, 008h, 000h, 0FFh, 016h, 008h, 0C2h, 020h
        db  005h, 0F4h, 016h, 005h, 0C2h, 017h, 013h, 004h, 0C5h, 0D8h, 006h, 0A0h, 0BBh, 0A4h, 003h, 080h
        db  002h, 0E0h, 008h, 030h, 001h, 083h, 006h, 000h, 016h, 005h, 0C1h, 042h, 013h, 008h, 0C0h, 0C0h
        db  0C1h, 001h, 004h, 0C2h, 0D8h, 020h, 008h, 0E0h, 001h, 080h, 0C8h, 004h, 001h, 08Ch, 002h, 0E0h
        db  000h, 0C0h, 007h, 0A0h, 000h, 014h, 008h, 030h, 0C8h, 020h, 008h, 050h, 000h, 016h, 003h, 080h
        db  003h, 000h, 000h, 002h, 007h, 009h, 0C2h, 0A0h, 001h, 004h, 001h, 08Ah, 000h, 080h, 052h, 04Ah
        db  0D2h, 089h, 0C8h, 00Ah, 001h, 004h, 0D2h, 0A0h, 001h, 004h, 016h, 0F9h, 002h, 089h, 05Fh, 0FFh
        db  016h, 01Bh, 001h, 060h, 000h, 0E0h, 000h, 002h, 013h, 004h, 088h, 020h, 010h, 080h, 010h, 088h
        db  013h, 009h, 007h, 0A0h, 001h, 004h, 0FFh, 08Ch, 001h, 0A0h, 000h, 0E0h, 000h, 002h, 003h, 090h
        db  0FFh, 0FFh, 003h, 080h, 001h, 0A0h, 007h, 058h, 040h, 000h, 001h, 020h, 007h, 058h, 020h, 0FEh
        db  016h, 02Eh, 003h, 090h, 0FFh, 0FFh, 003h, 080h, 005h, 049h, 001h, 089h, 080h, 000h, 001h, 049h
        db  040h, 000h, 013h, 02Fh, 001h, 049h, 020h, 000h, 016h, 01Bh, 001h, 060h, 000h, 0E0h, 000h, 002h
        db  013h, 007h, 001h, 049h, 001h, 000h, 013h, 00Bh, 088h, 020h, 010h, 080h, 010h, 088h, 013h, 007h
        db  007h, 0A0h, 001h, 004h, 0FFh, 08Ch, 001h, 0A0h, 000h, 0E0h, 000h, 002h, 010h, 007h, 001h, 0A0h
        db  007h, 058h, 040h, 000h, 001h, 020h, 007h, 058h, 020h, 0FEh, 016h, 002h, 001h, 089h, 020h, 000h
        db  001h, 089h, 001h, 000h, 0D2h, 049h, 016h, 005h, 003h, 090h, 0FFh, 0FFh, 003h, 080h, 007h, 089h
        db  020h, 000h, 0F8h, 009h, 007h, 03Ah, 0C2h, 060h, 007h, 036h, 013h, 0F6h, 004h, 0E0h, 007h, 036h
        db  004h, 054h, 004h, 060h, 0B4h, 058h, 003h, 000h, 000h, 002h, 001h, 040h, 040h, 000h, 016h, 007h
        db  001h, 0DDh, 000h, 080h, 001h, 0C0h, 020h, 000h, 003h, 000h, 000h, 00Fh, 004h, 05Bh, 003h, 000h
        db  000h, 00Fh, 004h, 060h, 0C4h, 070h, 003h, 000h, 000h, 00Fh, 0C0h, 082h, 010h, 004h, 006h, 0A0h
        db  0C6h, 068h, 001h, 0C2h, 0FFh, 080h, 003h, 000h, 000h, 002h, 001h, 040h, 040h, 000h, 013h, 0F3h
        db  0C8h, 002h, 001h, 004h, 001h, 0C0h, 040h, 000h, 003h, 000h, 000h, 00Fh, 004h, 059h, 003h, 000h
        db  000h, 002h, 001h, 040h, 040h, 000h, 013h, 00Bh, 007h, 0A0h, 001h, 004h, 0FFh, 086h, 002h, 060h
        db  040h, 000h, 001h, 085h, 058h, 000h, 003h, 000h, 000h, 00Fh, 004h, 060h, 0C4h, 036h, 002h, 060h
        db  000h, 080h, 002h, 065h, 058h, 000h, 010h, 0F7h, 0C0h, 0E0h, 007h, 05Ch, 0C8h, 003h, 001h, 06Ch
        db  0C3h, 060h, 0FCh, 004h, 0C2h, 0C3h, 0A2h, 0CBh, 0C2h, 0EBh, 00Ch, 032h, 0C8h, 00Bh, 0FCh, 000h
        db  013h, 00Ah, 0C8h, 00Bh, 001h, 06Ch, 0A3h, 060h, 0FCh, 004h, 0C2h, 0E0h, 0FCh, 000h, 016h, 0F9h
        db  0A8h, 020h, 0E0h, 01Eh, 0FCh, 004h, 002h, 02Dh, 000h, 004h, 0C8h, 003h, 001h, 06Ch, 0C8h, 00Dh
        db  0FCh, 004h, 003h, 000h, 000h, 002h, 0C2h, 020h, 010h, 080h, 002h, 048h, 001h, 0FFh, 0A2h, 008h
        db  0CAh, 003h, 010h, 08Eh, 005h, 0A0h, 010h, 080h, 001h, 060h, 007h, 058h, 040h, 000h, 013h, 006h
        db  007h, 0A0h, 001h, 004h, 0FFh, 08Ch, 001h, 0E0h, 007h, 058h, 040h, 000h, 003h, 000h, 000h, 00Fh
        db  004h, 020h, 0EAh, 07Ch, 0C0h, 081h, 001h, 020h, 001h, 082h, 000h, 006h, 013h, 026h, 001h, 020h
        db  001h, 082h, 000h, 004h, 016h, 022h, 007h, 0A0h, 016h, 02Eh, 003h, 0E8h, 001h, 0E0h, 001h, 09Ch
        db  000h, 040h, 006h, 020h, 016h, 02Eh, 013h, 021h, 001h, 060h, 001h, 09Ch, 000h, 040h, 016h, 0F9h
        db  001h, 060h, 00Ch, 002h, 001h, 000h, 013h, 015h, 001h, 020h, 009h, 018h, 000h, 006h, 016h, 004h
        db  001h, 0A0h, 001h, 082h, 000h, 002h, 010h, 003h, 001h, 0E0h, 001h, 082h, 000h, 006h, 00Bh, 0BBh
        db  00Bh, 0BBh, 00Bh, 0ABh, 007h, 0A0h, 00Ch, 002h, 080h, 000h, 0C8h, 001h, 001h, 08Ah, 0C8h, 001h
        db  008h, 0CAh, 007h, 0A0h, 000h, 02Ah, 0B4h, 066h, 004h, 05Bh, 007h, 0A0h, 0E1h, 09Eh, 0D3h, 04Ah
        db  007h, 0A0h, 000h, 012h, 0D3h, 04Ah, 007h, 020h, 016h, 030h, 007h, 0A0h, 000h, 02Ah, 0B4h, 066h
        db  004h, 05Bh, 001h, 060h, 016h, 030h, 0FFh, 0FFh, 013h, 01Ah, 007h, 0A0h, 016h, 02Eh, 003h, 0E8h
        db  001h, 0E0h, 001h, 09Ch, 000h, 040h, 006h, 020h, 016h, 02Eh, 013h, 006h, 001h, 060h, 001h, 09Ch
        db  000h, 040h, 016h, 0F9h, 004h, 060h, 0C1h, 062h, 007h, 0A0h, 0E1h, 09Eh, 0D3h, 04Ah, 007h, 0A0h
        db  000h, 012h, 0D3h, 04Ah, 007h, 0A0h, 000h, 02Ah, 0B4h, 066h, 007h, 020h, 016h, 030h, 004h, 060h
        db  0C1h, 042h, 001h, 0E0h, 0FCh, 002h, 000h, 020h, 007h, 08Bh, 0B8h, 07Eh, 004h, 060h, 0C1h, 096h
        db  007h, 08Fh, 010h, 080h, 004h, 0FFh, 002h, 08Fh, 018h, 000h, 016h, 0FCh, 007h, 08Fh, 008h, 022h
        db  004h, 0FFh, 002h, 08Fh, 008h, 050h, 016h, 0FCh, 007h, 0A0h, 008h, 044h, 001h, 08Ch, 007h, 08Fh
        db  0D1h, 034h, 0C8h, 00Fh, 008h, 050h, 007h, 0A0h, 017h, 0A0h, 017h, 0A0h, 007h, 0A0h, 017h, 0A2h
        db  0FEh, 03Eh, 004h, 0E0h, 017h, 0A6h, 004h, 0E0h, 016h, 030h, 004h, 060h, 0C4h, 036h, 007h, 0A0h
        db  016h, 032h, 000h, 032h, 004h, 060h, 0C6h, 0D8h, 007h, 020h, 016h, 032h, 004h, 060h, 0C9h, 048h
        db  001h, 060h, 007h, 058h, 040h, 000h, 016h, 004h, 001h, 060h, 001h, 004h, 020h, 000h, 016h, 0FCh
        db  007h, 0A0h, 001h, 004h, 0FFh, 080h, 004h, 060h, 0B3h, 0D2h, 002h, 02Eh, 000h, 011h, 0C2h, 060h
        db  001h, 062h, 004h, 0E0h, 001h, 062h, 007h, 08Ch, 008h, 0E6h, 0CFh, 01Eh, 011h, 018h, 0C3h, 05Eh
        db  0C3h, 0FEh, 0CFh, 01Eh, 0E3h, 05Eh, 0A3h, 0FEh, 0CFh, 01Eh, 0E3h, 05Eh, 013h, 010h, 0A3h, 0FEh
        db  083h, 0FEh, 016h, 00Dh, 007h, 08Fh, 000h, 004h, 0C3h, 07Eh, 005h, 04Dh, 083h, 06Eh, 0FFh, 0F6h
        db  016h, 006h, 006h, 00Fh, 016h, 0F9h, 0C8h, 009h, 001h, 062h, 004h, 060h, 0C3h, 0FEh, 0C8h, 009h
        db  001h, 062h, 007h, 020h, 008h, 0E6h, 004h, 060h, 0C3h, 0FEh, 003h, 000h, 000h, 002h, 0C0h, 0A0h
        db  001h, 06Ah, 0C3h, 00Eh, 009h, 0ACh, 0C8h, 00Ch, 001h, 06Ah, 0C3h, 00Eh, 002h, 04Ch, 003h, 0FFh
        db  0D8h, 0ECh, 0F8h, 000h, 017h, 0C0h, 005h, 083h, 002h, 043h, 000h, 03Fh, 0D8h, 0ECh, 0F8h, 001h
        db  017h, 0C0h, 0C8h, 002h, 001h, 06Ah, 005h, 0CEh, 003h, 090h, 0EFh, 0FFh, 003h, 080h, 004h, 0CBh
        db  010h, 004h, 0A2h, 0C4h, 0C8h, 020h, 0FCh, 000h, 001h, 06Ch, 001h, 060h, 0FCh, 002h, 001h, 000h
        db  016h, 0F8h, 0A8h, 020h, 0E0h, 01Eh, 0FCh, 004h, 0A2h, 0E0h, 0FCh, 004h, 0C8h, 00Ah, 001h, 06Ch
        db  0C8h, 00Bh, 0FCh, 004h, 0C2h, 0E0h, 010h, 080h, 002h, 04Bh, 001h, 0FFh, 0A2h, 0CBh, 0CAh, 0CAh
        db  010h, 08Eh, 005h, 0A0h, 010h, 080h, 001h, 060h, 007h, 058h, 040h, 000h, 013h, 006h, 007h, 0A0h
        db  001h, 004h, 0FFh, 08Ch, 001h, 0E0h, 007h, 058h, 040h, 000h, 003h, 090h, 000h, 07Fh, 003h, 080h
        db  001h, 060h, 009h, 018h, 000h, 004h, 013h, 005h, 007h, 020h, 016h, 034h, 007h, 08Dh, 080h, 000h
        db  004h, 052h, 001h, 060h, 009h, 018h, 000h, 002h, 013h, 0F7h, 007h, 0A0h, 016h, 034h, 000h, 003h
        db  010h, 0F5h, 0BAh, 0BEh, 0FAh, 0CEh, 042h, 041h, 042h, 045h, 046h, 041h, 043h, 045h, 000h, 000h
        db  0B2h, 02Ch, 000h, 004h, 002h, 002h, 006h, 003h, 000h, 000h, 0B2h, 090h, 000h, 002h, 00Bh, 0FFh
        db  000h, 000h, 0B3h, 0BEh, 000h, 004h, 004h, 060h, 0FDh, 0D4h, 000h, 000h, 0B5h, 05Eh, 000h, 004h
        db  004h, 060h, 0D4h, 02Ch, 000h, 000h, 0B5h, 066h, 000h, 004h, 004h, 060h, 0D3h, 09Ah, 000h, 000h
        db  0B6h, 052h, 000h, 004h, 004h, 060h, 0F7h, 0D2h, 000h, 000h, 0B6h, 062h, 000h, 004h, 004h, 060h
        db  0D3h, 0A8h, 000h, 000h, 0B7h, 0FEh, 000h, 004h, 004h, 060h, 0D7h, 02Eh, 000h, 000h, 0B8h, 07Ah
        db  000h, 004h, 004h, 060h, 0FDh, 076h, 000h, 000h, 0B9h, 018h, 000h, 004h, 004h, 060h, 0D2h, 0FEh
        db  000h, 000h, 0BAh, 0F6h, 000h, 004h, 006h, 0A0h, 0F9h, 098h, 000h, 000h, 0BBh, 084h, 000h, 004h
        db  0C1h, 0E0h, 0FCh, 002h, 000h, 000h, 0BEh, 062h, 000h, 004h, 007h, 08Bh, 04Eh, 000h, 000h, 000h
        db  0C0h, 0E6h, 000h, 004h, 004h, 060h, 0F7h, 0E2h, 000h, 000h, 0C1h, 054h, 000h, 004h, 004h, 060h
        db  0FDh, 036h, 000h, 000h, 0C1h, 070h, 000h, 004h, 004h, 060h, 0F7h, 0EEh, 000h, 000h, 0C2h, 022h
        db  000h, 006h, 001h, 020h, 001h, 000h, 000h, 080h, 000h, 000h, 0C3h, 0ECh, 000h, 004h, 004h, 060h
        db  0FDh, 0EEh, 000h, 000h, 0C4h, 01Eh, 000h, 004h, 004h, 060h, 0FDh, 084h, 000h, 000h, 0C4h, 03Ah
        db  000h, 004h, 004h, 060h, 0C4h, 054h, 000h, 000h, 0C4h, 060h, 000h, 004h, 004h, 060h, 0FBh, 0DAh
        db  000h, 000h, 0C4h, 088h, 000h, 004h, 004h, 060h, 0FCh, 002h, 000h, 000h, 0C7h, 002h, 000h, 004h
        db  007h, 089h, 000h, 060h, 000h, 000h, 0C9h, 038h, 000h, 004h, 004h, 060h, 0FEh, 0C4h, 000h, 000h
        db  0CBh, 000h, 000h, 004h, 007h, 08Dh, 000h, 060h, 000h, 000h, 0CBh, 08Eh, 000h, 004h, 004h, 060h
        db  0FEh, 0C4h, 000h, 000h, 0A0h, 002h, 000h, 002h, 07Ch, 097h, 000h, 000h, 0A0h, 07Ch, 000h, 002h
        db  046h, 05Dh, 000h, 000h, 0A0h, 034h, 000h, 00Ah, 000h, 002h, 023h, 0F1h, 0F1h, 0F0h, 0F0h, 0E7h
        db  0E7h, 0E7h, 000h, 000h, 0A0h, 0AEh, 000h, 002h, 008h, 022h, 000h, 000h, 0A0h, 0B2h, 000h, 002h
        db  0CDh, 030h, 000h, 000h, 0A1h, 018h, 000h, 002h, 0D4h, 02Ch, 000h, 000h, 0A1h, 01Ch, 000h, 002h
        db  0D2h, 080h, 000h, 000h, 0A1h, 024h, 000h, 002h, 0FBh, 014h, 000h, 000h, 0A1h, 070h, 000h, 002h
        db  0F8h, 0D6h, 000h, 000h, 0A1h, 074h, 000h, 002h, 0FAh, 090h, 000h, 000h, 0A1h, 07Ch, 000h, 002h
        db  0F9h, 054h, 000h, 000h, 0A2h, 0E6h, 000h, 004h, 016h, 00Eh, 0FCh, 04Ch, 000h, 000h, 0A2h, 0CCh
        db  000h, 002h, 0C4h, 0A6h, 000h, 000h, 0A2h, 0D4h, 000h, 002h, 0FCh, 022h, 000h, 000h, 0A2h, 0F4h
        db  000h, 002h, 0FCh, 022h, 000h, 000h, 0A3h, 014h, 000h, 002h, 0FDh, 0C2h, 000h, 000h, 0A3h, 01Ah
        db  000h, 002h, 0F7h, 00Ah, 000h, 000h, 0A3h, 01Ch, 000h, 002h, 0FDh, 0CCh, 000h, 000h, 0AEh, 000h
        db  000h, 002h, 0CCh, 000h, 000h, 000h, 0F1h, 0F2h, 000h, 002h, 000h, 004h, 000h, 000h, 0F2h, 0A8h
        db  000h, 002h, 000h, 004h, 000h, 000h, 0F2h, 0DCh, 000h, 002h, 000h, 004h, 000h, 000h, 0F2h, 028h
        db  000h, 002h, 004h, 000h, 000h, 000h, 0F2h, 046h, 000h, 002h, 004h, 000h, 0DEh, 0ADh, 0BEh, 0EFh
        db  000h, 000h, 044h, 045h, 041h, 044h, 042h, 045h, 045h, 046h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
_LTEXT ends
        end
