; setp3_lcode_01_01.asm -- SETP3 object 1 segment _LTEXT  (part 1/2)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   21h
        push   0
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 1]
        push   eax
        push   0
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [ebp - 1]
        and    al, 21h
        cmp    al, 21h
        je     SHORT loc_1_0033
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0033:
        push   48h
        xor    ebx, ebx
        push   0eh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   ebx
        push   0bh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   ebx
        push   0ah
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [esi + 15bh]
        push   eax
        push   0ch
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   2
        push   0dh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [esi + 0fah]
        push   eax
        push   3
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [esi + 0fah]
        push   eax
        push   1
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [esi + 0fbh]
        push   eax
        push   2
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [esi + 0fah]
        push   0ffh
        inc    al
        push   7
        mov    byte ptr [esi + 0fdh], al
        mov    byte ptr [esi + 0fch], al
        mov    byte ptr [esi + 118h], bl
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        mov    cl, byte ptr [esi + 15ch]
        push   ecx
        push   0fh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   61h
        push   ebx
        db  0FFh, 015h, 000h, 000h, 000h, 000h
loc_1_00D9:
        mov    al, byte ptr [esi + ebx + 104h]
        lea    ecx, [ebx + 1]
        push   eax
        inc    ebx
        push   ecx
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        cmp    ebx, 6
        jb     SHORT loc_1_00D9
        mov    eax, dword ptr [esi + 14bh]
        xor    ebx, ebx
        mov    edi, dword ptr [eax + 0ch]
        and    edi, 4
loc_1_00FF:
        mov    eax, 0ffh
        test   edi, edi
        jne    SHORT loc_1_0111
        xor    eax, eax
        mov    al, byte ptr [esi + ebx + 14fh]
loc_1_0111:
        push   eax
        lea    eax, [ebx + 8]
        inc    ebx
        push   eax
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        cmp    ebx, 8
        jb     SHORT loc_1_00FF
        mov    al, byte ptr [esi + 0fdh]
        push   eax
        push   7
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   21h
        push   0
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   22h
        push   0
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_1_0150:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   eax
        call   sub_1_0670
        push   0
        xor    esi, esi
        push   0bh
        mov    ebx, 80h
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0
        push   0ah
        db  0FFh, 015h, 000h, 000h, 000h, 000h
loc_1_017E:
        lea    eax, [ebp - 1]
        push   eax
        push   7
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        test   byte ptr [ebp - 1], bl
        jne    SHORT loc_1_01A4
        mov    edi, 0ah
loc_1_0194:
        push   64h
        call   sub_1_4250
        dec    edi
        jne    SHORT loc_1_0194
        inc    esi
        cmp    esi, 4
        jb     SHORT loc_1_017E
loc_1_01A4:
        push   2
        push   0dh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   22h
        push   0
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_01D0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    edi, 14h
        call   sub_1_0150
loc_1_01E3:
        push   64h
        call   sub_1_4250
        dec    edi
        jne    SHORT loc_1_01E3
        call   sub_1_1540
        call   sub_1_0980
        push   esi
        call   0
        test   al, al
        jne    SHORT loc_1_0220
        push   1
        mov    eax, dword ptr [esi + 4]
        push   1
        push   2
        push   0aa000003h
        push   eax
        call   sub_1_4328
        add    esp, 14h
        xor    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_0220:
        call   sub_1_1520
        push   0
        push   0dh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0240:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        lea    eax, [ebp - 14h]
        push   esi
        lea    ecx, [ebp - 8]
        push   edi
        mov    edx, dword ptr [ebp + 0ch]
        push   eax
        push   ecx
        push   0
        push   0
        push   edx
        call   sub_1_4280
        cmp    dword ptr [ebp - 14h], 0
        jne    SHORT loc_1_026F
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_026F:
        mov    eax, dword ptr [ebp + 8]
        lea    ecx, [ebp - 10h]
        mov    edx, dword ptr [ebp - 8]
        mov    esi, dword ptr [eax + 4fh]
        shl    esi, 8
        add    esi, dword ptr [eax + 0edh]
        lea    eax, [ebp - 0ch]
        push   eax
        push   ecx
        push   edx
        call   sub_1_42A4
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_02BF
        xor    edi, edi
loc_1_0297:
        cmp    dword ptr [ebp - 0ch], edi
        jne    SHORT loc_1_02BF
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_42C8
        lea    ecx, [ebp - 0ch]
        lea    edx, [ebp - 10h]
        push   ecx
        mov    eax, dword ptr [ebp - 8]
        push   edx
        push   eax
        call   sub_1_42A4
        cmp    dword ptr [ebp - 8], edi
        jne    SHORT loc_1_0297
loc_1_02BF:
        pushf
        cli
        push   0
        push   0
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0ffh
        push   0ah
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0
        push   0bh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0ah
        push   0
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        in     al, 61h
        in     al, 61h
        in     al, 61h
        push   40h
        push   7
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   esi
        push   8
        shr    esi, 8
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   esi
        push   9
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        push   0ah
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 14h]
        shr    eax, 8
        xor    esi, esi
        push   eax
        push   0bh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   12h
        push   0
        db  0FFh, 015h, 000h, 000h, 000h, 000h
loc_1_0339:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        push   eax
        push   ecx
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 8]
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_42C8
        cmp    dword ptr [ebp - 8], esi
        je     SHORT loc_1_039C
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 10h]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_42A4
        cmp    dword ptr [ebp - 8], esi
        je     SHORT loc_1_03A1
loc_1_036F:
        cmp    dword ptr [ebp - 0ch], esi
        jne    SHORT loc_1_039C
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_42C8
        cmp    dword ptr [ebp - 8], esi
        je     SHORT loc_1_039C
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 10h]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_42A4
        cmp    dword ptr [ebp - 8], esi
        jne    SHORT loc_1_036F
loc_1_039C:
        cmp    dword ptr [ebp - 8], esi
        jne    SHORT loc_1_0339
loc_1_03A1:
        mov    ecx, dword ptr [ebp - 14h]
        mov    eax, dword ptr [ebp + 10h]
        mov    esi, 0ffffh
        mov    ebx, 40h
        mov    dword ptr [eax], ecx
loc_1_03B3:
        test   esi, esi
        je     SHORT loc_1_03C9
        dec    esi
        lea    eax, [ebp - 1]
        push   eax
        push   7
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        test   byte ptr [ebp - 1], bl
        je     SHORT loc_1_03B3
loc_1_03C9:
        popf
        pop    edi
        cmp    esi, 1
        sbb    al, al
        pop    esi
        inc    al
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h
sub_1_03E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        test   esi, esi
        jne    SHORT loc_1_03FB
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_03FB:
        pushf
        cli
        push   0
        push   0
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0ffh
        push   0ah
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0
        push   0bh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0ah
        push   0
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        in     al, 61h
        in     al, 61h
        in     al, 61h
        push   40h
        push   7
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        mov    edi, dword ptr [ebp + 8]
        push   edi
        push   8
        shr    edi, 8
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   edi
        push   9
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   esi
        push   0ah
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        mov    eax, esi
        shr    eax, 8
        push   eax
        push   0bh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   12h
        push   0
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   esi
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        mov    ebx, 40h
        call   esi
        mov    si, 0ffffh
loc_1_0488:
        test   si, si
        je     SHORT loc_1_04A0
        dec    si
        lea    eax, [ebp - 1]
        push   eax
        push   7
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        test   byte ptr [ebp - 1], bl
        je     SHORT loc_1_0488
loc_1_04A0:
        popf
        cmp    si, 1
        pop    edi
        sbb    al, al
        pop    esi
        inc    al
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_04C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        test   esi, esi
        jne    SHORT loc_1_04DB
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_04DB:
        pushf
        cli
        push   0
        push   0
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        push   8
        shr    edi, 8
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   edi
        push   9
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   esi
        push   0ah
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        mov    eax, esi
        shr    eax, 8
        push   eax
        push   0bh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0ah
        push   0
        mov    ebx, 40h
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   esi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    esi, 0ffffh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
loc_1_0536:
        test   esi, esi
        je     SHORT loc_1_054C
        dec    esi
        lea    eax, [ebp - 1]
        push   eax
        push   7
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        test   byte ptr [ebp - 1], bl
        je     SHORT loc_1_0536
loc_1_054C:
        popf
        pop    edi
        cmp    esi, 1
        sbb    al, al
        pop    esi
        inc    al
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
sub_1_0560:
        push   ebp
        mov    eax, 0ffffffffh
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        cmp    dword ptr [ebp + 0ch], esi
        je     SHORT loc_1_05A8
loc_1_0572:
        mov    ecx, dword ptr [ebp + 8]
        mov    dl, byte ptr [esi + ecx]
        mov    ecx, 8
loc_1_057D:
        mov    edi, eax
        mov    bl, dl
        shr    dl, 1
        and    edi, 80000000h
        shr    edi, 1fh
        and    ebx, 1
        xor    edi, ebx
        add    eax, eax
        test   edi, edi
        je     SHORT loc_1_059F
        xor    eax, 4c11db6h
        or     eax, edi
loc_1_059F:
        dec    ecx
        jne    SHORT loc_1_057D
        inc    esi
        cmp    dword ptr [ebp + 0ch], esi
        ja     SHORT loc_1_0572
loc_1_05A8:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  090h
sub_1_05B0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   6
        push   eax
        call   sub_1_0560
        shr    eax, 1ah
        mov    edx, dword ptr [ebp + 0ch]
        mov    ebx, eax
        and    al, 7
        shr    ebx, 3
        mov    cl, al
        mov    byte ptr [edx], bl
        mov    dl, 1
        shl    dl, cl
        mov    ecx, dword ptr [ebp + 10h]
        pop    ebx
        pop    ebp
        mov    byte ptr [ecx], dl
        ret    0ch
        db  090h
sub_1_05E0:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        mov    eax, 8
        sub    esp, 0ch
        mov    dword ptr [ebp - 8], edx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
loc_1_05F5:
        mov    ecx, dword ptr [ebp - 8]
        inc    dword ptr [ebp - 8]
        mov    byte ptr [ecx + esi + 14fh], dl
        cmp    dword ptr [ebp - 8], eax
        jb     SHORT loc_1_05F5
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 8]
        push   eax
        mov    ecx, dword ptr [esi + 14bh]
        push   60h
        lea    edx, [ebp - 0ch]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0622:
        cmp    dword ptr [ebp - 8], 0
        jbe    SHORT loc_1_065C
        xor    edi, edi
loc_1_062A:
        lea    eax, [ebp - 1]
        lea    ecx, [ebp - 2]
        push   eax
        dec    dword ptr [ebp - 8]
        push   ecx
        mov    edx, dword ptr [ebp - 8]
        lea    eax, [edx + edx*2]
        db  08Dh, 00Ch, 045h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_1_05B0
        xor    edx, edx
        mov    cl, byte ptr [ebp - 1]
        mov    dl, byte ptr [ebp - 2]
        or     byte ptr [edx + esi + 14fh], cl
        cmp    dword ptr [ebp - 8], edi
        ja     SHORT loc_1_062A
loc_1_065C:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0670:
        push   21h
        push   0
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        xor    al, al
        ret    4
        db  090h
sub_1_0680:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        mov    ecx, dword ptr [eax + 57h]
        push   esi
        mov    esi, dword ptr [eax + ecx*4 + 0bdh]
        mov    al, byte ptr [eax + 0f9h]
        add    al, cl
        push   eax
        push   4
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        cmp    esi, 3ch
        jae    SHORT loc_1_06B0
        mov    esi, 3ch
loc_1_06B0:
        mov    eax, esi
        shr    eax, 8
        push   eax
        push   6
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   esi
        push   5
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 1]
        push   ecx
        push   0
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   26h
        push   0
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_06F0:
        push   ebp
        mov    ebp, esp
        push   62h
        push   0
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0fdh
        push   eax
        push   7
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   22h
        push   0
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        xor    al, al
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_0720:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        add    eax, 0feh
        push   eax
        push   4
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        xor    al, al
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_0740:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    cl, byte ptr [eax + 0fch]
        cmp    byte ptr [eax + 0fah], cl
        jne    SHORT loc_1_076A
        mov    al, byte ptr [eax + 0fbh]
        dec    al
        push   eax
        push   3
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        pop    ebp
        ret    4
loc_1_076A:
        dec    cl
        push   ecx
        push   3
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0780:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    al, byte ptr [eax + 15bh]
        push   eax
        push   0ch
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        xor    al, al
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_07A0:
        push   esi
        push   62h
        push   0
        xor    esi, esi
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
loc_1_07B2:
        lea    ecx, [esi + 8]
        push   0ffh
        push   ecx
        inc    esi
        call   eax
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    esi, 8
        jb     SHORT loc_1_07B2
        push   22h
        push   0
        call   eax
        xor    al, al
        pop    esi
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_07E0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   62h
        push   0
        xor    edi, edi
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [ebp + 8]
loc_1_07F4:
        mov    al, byte ptr [esi + edi + 14fh]
        lea    ecx, [edi + 8]
        push   eax
        inc    edi
        push   ecx
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        cmp    edi, 8
        jb     SHORT loc_1_07F4
        push   22h
        push   0
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        xor    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_0820:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        test   byte ptr [eax + 103h], 4
        je     SHORT loc_1_0836
        push   eax
        call   sub_1_0840
loc_1_0836:
        xor    al, al
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_0840:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 1]
        push   eax
        push   0dh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    al, byte ptr [ebp - 1]
        lea    ecx, [ebp - 1]
        push   ecx
        add    dword ptr [esi + 132h], eax
        push   0eh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        xor    ecx, ecx
        lea    edx, [ebp - 1]
        mov    cl, byte ptr [ebp - 1]
        push   edx
        push   0fh
        add    dword ptr [esi + 136h], ecx
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        xor    ecx, ecx
        xor    al, al
        mov    cl, byte ptr [ebp - 1]
        add    dword ptr [esi + 13ah], ecx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_08A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, 14h
        push   esi
        call   sub_1_0670
loc_1_08B6:
        push   64h
        call   sub_1_4250
        dec    edi
        jne    SHORT loc_1_08B6
        test   byte ptr [esi + 103h], 0ah
        jne    SHORT loc_1_08E8
        lea    eax, [ebp - 1]
        push   eax
        push   7
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        test   byte ptr [ebp - 1], 0ah
        jne    SHORT loc_1_08E8
        mov    al, byte ptr [esi + 5bh]
        mov    byte ptr [esi + 5ch], al
        mov    byte ptr [esi + 1b1h], 0
loc_1_08E8:
        mov    byte ptr [esi + 5bh], 0
        mov    dword ptr [esi + 147h], 0
        push   0
        push   0bh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0
        push   0ah
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   2
        push   0dh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   22h
        push   0
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        xor    al, al
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0930:
        db  066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        db  0A0h, 000h, 000h, 000h, 000h
        sub    esp, 0ch
        mov    byte ptr [ebp - 3], al
        mov    word ptr [ebp - 0ah], cx
        mov    dx, word ptr [ebp - 0ah]
        mov    al, byte ptr [ebp - 3]
        out    dx, al
        mov    byte ptr [ebp - 2], 1dh
        add    cx, 2
        mov    word ptr [ebp - 8], cx
        mov    dx, word ptr [ebp - 8]
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        mov    byte ptr [ebp - 1], 15h
        mov    word ptr [ebp - 6], cx
        mov    dx, word ptr [ebp - 6]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0980:
        db  066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        mov    byte ptr [ebp - 6], 0aah
        mov    word ptr [ebp - 12h], cx
        mov    dx, word ptr [ebp - 12h]
        mov    al, byte ptr [ebp - 6]
        out    dx, al
        mov    byte ptr [ebp - 5], 1dh
        add    cx, 2
        mov    word ptr [ebp - 10h], cx
        mov    dx, word ptr [ebp - 10h]
        mov    al, byte ptr [ebp - 5]
        out    dx, al
        mov    byte ptr [ebp - 4], 15h
        mov    word ptr [ebp - 0eh], cx
        mov    dx, word ptr [ebp - 0eh]
        mov    al, byte ptr [ebp - 4]
        out    dx, al
        push   14h
        call   sub_1_4250
        mov    byte ptr [ebp - 3], 0
        db  066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 0ch], cx
        mov    dx, word ptr [ebp - 0ch]
        mov    al, byte ptr [ebp - 3]
        out    dx, al
        mov    byte ptr [ebp - 2], 1dh
        add    cx, 2
        mov    word ptr [ebp - 0ah], cx
        mov    dx, word ptr [ebp - 0ah]
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        mov    byte ptr [ebp - 1], 15h
        mov    word ptr [ebp - 8], cx
        mov    dx, word ptr [ebp - 8]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        push   14h
        call   sub_1_4250
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_1_0A10:
        db  0A0h, 000h, 000h, 000h, 000h
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        or     al, 8
        and    cl, 0fbh
        or     al, 10h
        db  00Ah, 00Dh, 000h, 000h, 000h, 000h
        db  00Ah, 00Dh, 000h, 000h, 000h, 000h
        db  0A2h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 000h
        db  088h, 00Dh, 000h, 000h, 000h, 000h
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0A50:
        db  0A0h, 000h, 000h, 000h, 000h
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        or     al, 8
        or     cl, 4
        or     al, 10h
        db  00Ah, 00Dh, 000h, 000h, 000h, 000h
        db  00Ah, 00Dh, 000h, 000h, 000h, 000h
        db  0A2h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 000h
        db  088h, 00Dh, 000h, 000h, 000h, 000h
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0A90:
        db  0A0h, 000h, 000h, 000h, 000h
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        and    al, 0f7h
        and    cl, 0fbh
        or     al, 10h
        db  00Ah, 00Dh, 000h, 000h, 000h, 000h
        db  00Ah, 00Dh, 000h, 000h, 000h, 000h
        db  0A2h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 000h
        db  088h, 00Dh, 000h, 000h, 000h, 000h
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0AD0:
        xor    al, al
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0AE0:
        push   ebp
        mov    ebp, esp
        push   esi
        pushf
        cli
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [ebp + 8]
        or     al, 20h
        jmp    NEAR PTR loc_1_0AF8
loc_1_0AF8:
        jmp    NEAR PTR loc_1_0AFD
loc_1_0AFD:
        out    dx, al
        add    dx, 2
        mov    al, 1dh
        jmp    NEAR PTR loc_1_0B09
loc_1_0B09:
        jmp    NEAR PTR loc_1_0B0E
loc_1_0B0E:
        out    dx, al
        mov    ax, 1515h
        jmp    NEAR PTR loc_1_0B18
loc_1_0B18:
        jmp    NEAR PTR loc_1_0B1D
loc_1_0B1D:
        out    dx, al
        mov    al, 17h
        jmp    NEAR PTR loc_1_0B25
loc_1_0B25:
        jmp    NEAR PTR loc_1_0B2A
loc_1_0B2A:
        out    dx, al
        dec    dx
        nop
        nop
        nop
        push   ecx
        mov    ecx, 1000h
        jmp    NEAR PTR loc_1_0B3B
loc_1_0B3B:
        jmp    NEAR PTR loc_1_0B40
loc_1_0B40:
        in     al, dx
        jmp    NEAR PTR loc_1_0B46
loc_1_0B46:
        jmp    NEAR PTR loc_1_0B4B
loc_1_0B4B:
        test   al, 40h
        jne    SHORT loc_1_0B7E
        in     al, dx
        jmp    NEAR PTR loc_1_0B55
loc_1_0B55:
        jmp    NEAR PTR loc_1_0B5A
loc_1_0B5A:
        test   al, 40h
        jne    SHORT loc_1_0B7E
        in     al, dx
        jmp    NEAR PTR loc_1_0B64
loc_1_0B64:
        jmp    NEAR PTR loc_1_0B69
loc_1_0B69:
        test   al, 40h
        jne    SHORT loc_1_0B7E
loc_1_0B6D:
        in     al, dx
        jmp    NEAR PTR loc_1_0B73
loc_1_0B73:
        jmp    NEAR PTR loc_1_0B78
loc_1_0B78:
        test   al, 40h
        jne    SHORT loc_1_0B7E
        loop   loc_1_0B6D
loc_1_0B7E:
        pop    ecx
        inc    dx
        xchg   ah, al
        jmp    NEAR PTR loc_1_0B88
loc_1_0B88:
        jmp    NEAR PTR loc_1_0B8D
loc_1_0B8D:
        out    dx, al
        dec    dx
        jmp    NEAR PTR loc_1_0B95
loc_1_0B95:
        jmp    NEAR PTR loc_1_0B9A
loc_1_0B9A:
        in     al, dx
        sar    al, 3
        and    ax, 0b847h
        or     al, ah
        mov    esi, dword ptr [ebp + 0ch]
        mov    byte ptr [esi], al
        popf
        pop    esi
        pop    ebp
        ret    8
loc_1_0BB0:
        push   ebp
        mov    ebp, esp
        push   edi
        pushf
        cli
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        mov    al, 30h
        out    dx, al
        add    dx, 2
        mov    al, 1dh
        jmp    NEAR PTR loc_1_0BCC
loc_1_0BCC:
        jmp    NEAR PTR loc_1_0BD1
loc_1_0BD1:
        out    dx, al
        mov    al, 15h
        jmp    NEAR PTR loc_1_0BD9
loc_1_0BD9:
        jmp    NEAR PTR loc_1_0BDE
loc_1_0BDE:
        out    dx, al
        nop
        nop
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
loc_1_0BE7:
        push   ecx
        mov    ecx, 10h
loc_1_0BED:
        in     al, 61h
        nop
        loop   loc_1_0BED
        pop    ecx
        mov    al, 16h
        out    dx, al
        dec    dx
        push   ecx
        mov    ecx, 1000h
        jmp    NEAR PTR loc_1_0C03
loc_1_0C03:
        jmp    NEAR PTR loc_1_0C08
loc_1_0C08:
        in     al, dx
        jmp    NEAR PTR loc_1_0C0E
loc_1_0C0E:
        jmp    NEAR PTR loc_1_0C13
loc_1_0C13:
        test   al, 40h
        jne    SHORT loc_1_0C46
        in     al, dx
        jmp    NEAR PTR loc_1_0C1D
loc_1_0C1D:
        jmp    NEAR PTR loc_1_0C22
loc_1_0C22:
        test   al, 40h
        jne    SHORT loc_1_0C46
        in     al, dx
        jmp    NEAR PTR loc_1_0C2C
loc_1_0C2C:
        jmp    NEAR PTR loc_1_0C31
loc_1_0C31:
        test   al, 40h
        jne    SHORT loc_1_0C46
loc_1_0C35:
        in     al, dx
        jmp    NEAR PTR loc_1_0C3B
loc_1_0C3B:
        jmp    NEAR PTR loc_1_0C40
loc_1_0C40:
        test   al, 40h
        jne    SHORT loc_1_0C46
        loop   loc_1_0C35
loc_1_0C46:
        pop    ecx
        mov    ah, al
        inc    dx
        mov    al, 14h
        jmp    NEAR PTR loc_1_0C52
loc_1_0C52:
        jmp    NEAR PTR loc_1_0C57
loc_1_0C57:
        out    dx, al
        jmp    NEAR PTR loc_1_0C5D
loc_1_0C5D:
        jmp    NEAR PTR loc_1_0C62
loc_1_0C62:
        nop
        dec    dx
        in     al, dx
        inc    dx
        sar    al, 3
        and    ax, 0b847h
        or     al, ah
        mov    byte ptr [edi], al
        inc    edi
        dec    ecx
        jne    NEAR PTR loc_1_0BE7
        popf
        pop    edi
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0C90:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        call   sub_1_0A90
        call   sub_1_0930
        db  0A0h, 000h, 000h, 000h, 000h
        xor    esi, esi
        push   eax
        mov    ebx, 0aah
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0aah
        push   8
        call   sub_1_13C0
loc_1_0CC1:
        lea    eax, [ebp - 1]
        push   eax
        push   8
        call   sub_1_0CF0
        cmp    byte ptr [ebp - 1], bl
        jne    SHORT loc_1_0CE2
        inc    esi
        cmp    esi, 3e8h
        jb     SHORT loc_1_0CC1
        xor    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_0CE2:
        mov    al, 0ffh
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_0CF0:
        push   ebp
        mov    ebp, esp
        push   esi
        pushf
        cli
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [ebp + 8]
        or     al, 20h
        out    dx, al
        add    dx, 2
        mov    al, 1dh
        out    dx, al
        mov    ax, 1515h
        out    dx, al
        mov    al, 17h
        out    dx, al
        dec    dx
        nop
        nop
        nop
        push   ecx
        mov    ecx, 1000h
        in     al, dx
        test   al, 40h
        jne    SHORT loc_1_0D34
        in     al, dx
        test   al, 40h
        jne    SHORT loc_1_0D34
        in     al, dx
        test   al, 40h
        jne    SHORT loc_1_0D34
loc_1_0D2D:
        in     al, dx
        test   al, 40h
        jne    SHORT loc_1_0D34
        loop   loc_1_0D2D
loc_1_0D34:
        pop    ecx
        inc    dx
        xchg   ah, al
        out    dx, al
        dec    dx
        in     al, dx
        sar    al, 3
        and    ax, 0b847h
        or     al, ah
        mov    esi, dword ptr [ebp + 0ch]
        mov    byte ptr [esi], al
        popf
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0D60:
        push   ebp
        mov    ebp, esp
        push   edi
        pushf
        cli
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        mov    al, 30h
        out    dx, al
        add    dx, 2
        mov    al, 1dh
        out    dx, al
        mov    al, 15h
        out    dx, al
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
loc_1_0D81:
        jmp    NEAR PTR loc_1_0D86
loc_1_0D86:
        jmp    NEAR PTR loc_1_0D8B
loc_1_0D8B:
        jmp    NEAR PTR loc_1_0D90
loc_1_0D90:
        mov    al, 16h
        out    dx, al
        dec    dx
        push   ecx
        mov    ecx, 1000h
        in     al, dx
        test   al, 40h
        jne    SHORT loc_1_0DB1
        in     al, dx
        test   al, 40h
        jne    SHORT loc_1_0DB1
        in     al, dx
        test   al, 40h
        jne    SHORT loc_1_0DB1
loc_1_0DAA:
        in     al, dx
        test   al, 40h
        jne    SHORT loc_1_0DB1
        loop   loc_1_0DAA
loc_1_0DB1:
        pop    ecx
        mov    ah, al
        inc    dx
        mov    al, 14h
        out    dx, al
        nop
        dec    dx
        in     al, dx
        inc    dx
        sar    al, 3
        and    ax, 0b847h
        or     al, ah
        mov    byte ptr [edi], al
        inc    edi
        loop   loc_1_0D81
        popf
        pop    edi
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0DE0:
        push   ebp
        mov    ebp, esp
        push   esi
        pushf
        cli
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [ebp + 8]
        or     al, 20h
        out    dx, al
        add    dx, 2
        mov    al, 1dh
        out    dx, al
        mov    al, 15h
        out    dx, al
        sub    dx, 2
        mov    al, 0ffh
        out    dx, al
        add    dx, 2
        mov    ax, 3537h
        out    dx, al
        dec    dx
        push   ecx
        mov    ecx, 1000h
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_0E2C
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_0E2C
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_0E2C
loc_1_0E25:
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_0E2C
        loop   loc_1_0E25
loc_1_0E2C:
        pop    ecx
        dec    dx
        in     al, dx
        mov    esi, dword ptr [ebp + 0ch]
        mov    byte ptr [esi], al
        add    dx, 2
        mov    al, ah
        out    dx, al
        popf
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0E50:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        pushf
        cli
        cld
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        mov    al, 30h
        out    dx, al
        add    dx, 2
        mov    al, 1dh
        out    dx, al
        mov    al, 15h
        out    dx, al
        sub    dx, 2
        mov    al, 0ffh
        out    dx, al
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        add    dx, 2
loc_1_0E7E:
        jmp    NEAR PTR loc_1_0E83
loc_1_0E83:
        jmp    NEAR PTR loc_1_0E88
loc_1_0E88:
        jmp    NEAR PTR loc_1_0E8D
loc_1_0E8D:
        mov    ax, 3537h
        out    dx, al
        dec    dx
        push   ecx
        mov    ecx, 1000h
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_0EB0
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_0EB0
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_0EB0
loc_1_0EA9:
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_0EB0
        loop   loc_1_0EA9
loc_1_0EB0:
        pop    ecx
        dec    dx
        insb
        add    dx, 2
        mov    al, ah
        out    dx, al
        nop
        nop
        loop   loc_1_0E7E
        popf
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0ED0:
        push   ebp
        mov    ebp, esp
        push   esi
        pushf
        cli
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [ebp + 8]
        or     al, 20h
        out    dx, al
        add    dx, 2
        mov    al, 1dh
        out    dx, al
        mov    al, 15h
        out    dx, al
        sub    dx, 2
        mov    al, 0ffh
        out    dx, al
        add    dx, 2
        mov    ax, 9597h
        out    dx, al
        dec    dx
        push   ecx
        mov    ecx, 1000h
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_0F1C
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_0F1C
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_0F1C
loc_1_0F15:
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_0F1C
        loop   loc_1_0F15
loc_1_0F1C:
        pop    ecx
        dec    dx
        in     al, dx
        mov    esi, dword ptr [ebp + 0ch]
        mov    byte ptr [esi], al
        add    dx, 2
        mov    al, ah
        out    dx, al
        popf
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0F40:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        pushf
        cli
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        mov    al, 30h
        out    dx, al
        add    dx, 2
        mov    al, 1dh
        out    dx, al
        mov    al, 15h
        out    dx, al
        sub    dx, 2
        mov    al, 0ffh
        out    dx, al
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        add    dx, 2
loc_1_0F6D:
        jmp    NEAR PTR loc_1_0F72
loc_1_0F72:
        jmp    NEAR PTR loc_1_0F77
loc_1_0F77:
        jmp    NEAR PTR loc_1_0F7C
loc_1_0F7C:
        mov    ax, 9597h
        out    dx, al
        dec    dx
        push   ecx
        mov    ecx, 1000h
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_0F9F
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_0F9F
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_0F9F
loc_1_0F98:
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_0F9F
        loop   loc_1_0F98
loc_1_0F9F:
        pop    ecx
        dec    dx
        insb
        add    dx, 2
        mov    al, ah
        out    dx, al
        nop
        nop
        loop   loc_1_0F6D
        popf
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0FC0:
        push   ebp
        mov    ebp, esp
        push   esi
        pushf
        cli
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [ebp + 8]
        or     al, 20h
        out    dx, al
        add    dx, 2
        mov    al, 1dh
        out    dx, al
        mov    al, 15h
        out    dx, al
        sub    dx, 2
        mov    al, 0ffh
        out    dx, al
        add    dx, 2
        mov    ax, 1517h
        out    dx, al
        dec    dx
        push   ecx
        mov    ecx, 1000h
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_100C
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_100C
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_100C
loc_1_1005:
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_100C
        loop   loc_1_1005
loc_1_100C:
        pop    ecx
        dec    dx
        in     al, dx
        mov    esi, dword ptr [ebp + 0ch]
        mov    byte ptr [esi], al
        add    dx, 2
        mov    al, ah
        out    dx, al
        popf
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1030:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        pushf
        cli
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        mov    al, 30h
        out    dx, al
        add    dx, 2
        mov    al, 1dh
        out    dx, al
        mov    al, 15h
        out    dx, al
        sub    dx, 2
        mov    al, 0ffh
        out    dx, al
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        add    dx, 2
loc_1_105D:
        jmp    NEAR PTR loc_1_1062
loc_1_1062:
        jmp    NEAR PTR loc_1_1067
loc_1_1067:
        jmp    NEAR PTR loc_1_106C
loc_1_106C:
        mov    ax, 1517h
        out    dx, al
        dec    dx
        push   ecx
        mov    ecx, 1000h
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_108F
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_108F
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_108F
loc_1_1088:
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_108F
        loop   loc_1_1088
loc_1_108F:
        pop    ecx
        dec    dx
        insb
        add    dx, 2
        mov    al, ah
        out    dx, al
        nop
        nop
        loop   loc_1_105D
        popf
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_10B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        db  066h, 081h, 03Dh, 000h, 000h, 000h, 000h, 078h, 003h
        push   ebx
        je     SHORT loc_1_10D4
        db  066h, 081h, 03Dh, 000h, 000h, 000h, 000h, 078h, 002h
        je     SHORT loc_1_10D4
        mov    al, 0ffh
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_10D4:
        db  0A0h, 000h, 000h, 000h, 000h
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        or     al, 8
        and    cl, 0fbh
        or     al, 10h
        db  00Ah, 00Dh, 000h, 000h, 000h, 000h
        db  00Ah, 00Dh, 000h, 000h, 000h, 000h
        db  0A2h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 000h
        db  088h, 00Dh, 000h, 000h, 000h, 000h
        call   sub_1_0930
        push   bx
        push   dx
        pushf
        cli
        in     ax, 22h
        mov    bx, ax
        test   ax, 1
        jne    SHORT loc_1_1129
        mov    ax, 8000h
        out    23h, al
        xchg   ah, al
        out    22h, al
        out    22h, ax
loc_1_1129:
        in     al, 22h
        and    al, 0f0h
        or     al, 3
        out    22h, al
        mov    dx, 30eh
        in     ax, dx
        mov    word ptr [ebp - 2], ax
        mov    ax, 8000h
        out    23h, al
        xchg   ah, al
        out    22h, al
        mov    ax, bx
        out    22h, ax
        test   ax, 1
        jne    SHORT loc_1_1158
        or     ax, 100h
        out    22h, ax
loc_1_1158:
        popf
        pop    dx
        pop    bx
        cmp    word ptr [ebp - 2], 43h
        je     SHORT loc_1_1181
        cmp    word ptr [ebp - 2], 73h
        je     SHORT loc_1_1181
        cmp    word ptr [ebp - 2], 70h
        je     SHORT loc_1_1181
        cmp    word ptr [ebp - 2], 40h
        je     SHORT loc_1_1181
        mov    al, 0ffh
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_1181:
        xor    al, al
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_1190:
        push   ebx
        pushf
        cli
        in     ax, 22h
        mov    cx, ax
        test   ax, 1
        jne    SHORT loc_1_11AD
        mov    ax, 8000h
        out    23h, al
        xchg   ah, al
        out    22h, al
        out    22h, ax
loc_1_11AD:
        mov    ax, 100h
        out    22h, ax
        in     al, 23h
        test   al, 80h
        je     SHORT loc_1_11CB
        mov    dx, 0fc23h
        in     al, dx
        mov    dx, 0f023h
        in     al, dx
        mov    dx, 0c023h
        in     al, dx
        in     al, 23h
loc_1_11CB:
        mov    al, 61h
        out    24h, al
        in     al, 25h
        mov    bh, al
        or     al, 8
        out    25h, al
        xor    al, al
        out    0fbh, al
        mov    al, 2
        out    0aeh, al
        in     al, 0afh
        db  0A2h, 000h, 000h, 000h, 000h
        and    al, 8fh
        db  066h, 081h, 03Dh, 000h, 000h, 000h, 000h, 078h, 003h
        jne    SHORT loc_1_11F7
        mov    ah, 90h
        jmp    SHORT loc_1_11F9
loc_1_11F7:
        mov    ah, 0a0h
loc_1_11F9:
        or     al, ah
        out    0afh, al
        xor    al, al
        out    0f9h, al
        mov    al, 61h
        out    24h, al
        mov    al, bh
        out    25h, al
        test   bl, 80h
        je     SHORT loc_1_1216
        mov    al, 0fah
        out    24h, al
        mov    al, 1
        out    25h, al
loc_1_1216:
        mov    ax, 8000h
        out    23h, al
        xchg   ah, al
        out    22h, al
        mov    ax, cx
        out    22h, ax
        test   ax, 1
        jne    SHORT loc_1_1233
        or     ax, 100h
        out    22h, ax
loc_1_1233:
        popf
        pop    ebx
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1240:
        push   ebx
        pushf
        cli
        in     ax, 22h
        mov    cx, ax
        test   ax, 1
        jne    SHORT loc_1_125D
        mov    ax, 8000h
        out    23h, al
        xchg   ah, al
        out    22h, al
        out    22h, ax
loc_1_125D:
        mov    ax, 100h
        out    22h, ax
        in     al, 23h
        test   al, 80h
        je     SHORT loc_1_127B
        mov    dx, 0fc23h
        in     al, dx
        mov    dx, 0f023h
        in     al, dx
        mov    dx, 0c023h
        in     al, dx
        in     al, 23h
loc_1_127B:
        mov    al, 61h
        out    24h, al
        in     al, 25h
        mov    bh, al
        or     al, 8
        out    25h, al
        xor    al, al
        out    0fbh, al
        mov    al, 2
        out    0aeh, al
        db  0A0h, 000h, 000h, 000h, 000h
        out    0afh, al
        xor    al, al
        out    0f9h, al
        mov    al, 61h
        out    24h, al
        mov    al, bh
        out    25h, al
        test   bl, 80h
        je     SHORT loc_1_12AF
        mov    al, 0fah
        out    24h, al
        mov    al, 1
        out    25h, al
loc_1_12AF:
        mov    ax, 8000h
        out    23h, al
        xchg   ah, al
        out    22h, al
        mov    ax, cx
        out    22h, ax
        test   ax, 1
        jne    SHORT loc_1_12CC
        or     ax, 100h
        out    22h, ax
loc_1_12CC:
        popf
        pop    ebx
        ret
loc_1_12D0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        pushf
        cli
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [ebp + 8]
        or     al, 20h
        add    dx, 3
        out    dx, al
        inc    dx
        mov    edi, dword ptr [ebp + 0ch]
        insb
        popf
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1300:
        push   ebp
        mov    ebp, esp
        pushf
        cli
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [ebp + 8]
        add    dx, 3
        out    dx, al
        mov    al, byte ptr [ebp + 0ch]
        inc    dx
        out    dx, al
        popf
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1330:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        pushf
        cli
        cld
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        mov    al, 30h
        out    dx, al
        add    dx, 2
        mov    al, 1dh
        out    dx, al
        mov    al, 15h
        out    dx, al
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        add    dx, 4
        rep insb
        popf
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1370:
        push   ebp
        mov    ebp, esp
        push   esi
        pushf
        cli
        cld
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        mov    al, 10h
        out    dx, al
        add    dx, 2
        mov    al, 1dh
        out    dx, al
        mov    al, 15h
        out    dx, al
        mov    ecx, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        add    dx, 4
        rep outsb
        popf
        pop    esi
        pop    ebp
        ret    8
        db  090h
loc_1_13A0:
        push   ebp
        mov    ebp, esp
        pushf
        cli
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [ebp + 8]
        add    dx, 3
        out    dx, al
        popf
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_13C0:
        push   ebp
        mov    ebp, esp
        pushf
        cli
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [ebp + 8]
        out    dx, al
        add    dx, 2
        mov    al, 1dh
        out    dx, al
        mov    al, 15h
        out    dx, al
        sub    dx, 2
        mov    al, byte ptr [ebp + 0ch]
        out    dx, al
        add    dx, 2
        mov    ax, 1517h
        out    dx, al
        dec    dx
        push   ecx
        mov    ecx, 1000h
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_140A
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_140A
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_140A
loc_1_1403:
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_140A
        loop   loc_1_1403
loc_1_140A:
        pop    ecx
        inc    dx
        mov    al, ah
        out    dx, al
        popf
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1420:
        push   ebp
        mov    ebp, esp
        push   esi
        pushf
        cli
        cld
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        mov    al, 10h
        out    dx, al
        add    dx, 2
        mov    al, 1dh
        out    dx, al
        mov    al, 15h
        out    dx, al
        mov    ecx, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
loc_1_1442:
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        outsb
        add    dx, 2
        mov    ax, 1517h
        out    dx, al
        dec    dx
        push   ecx
        mov    ecx, 1000h
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_1471
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_1471
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_1471
loc_1_146A:
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_1471
        loop   loc_1_146A
loc_1_1471:
        pop    ecx
        inc    dx
        mov    al, ah
        out    dx, al
        loop   loc_1_1442
        popf
        pop    esi
        pop    ebp
        ret    8
loc_1_1480:
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1490:
        push   ebp
        mov    ebp, esp
        pushf
        cli
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [ebp + 8]
        out    dx, al
        add    dx, 2
        mov    al, 1dh
        db  00Ah, 005h, 000h, 000h, 000h, 000h
        out    dx, al
        mov    al, 15h
        db  00Ah, 005h, 000h, 000h, 000h, 000h
        out    dx, al
        popf
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_14C0:
        pushf
        cli
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_14DA
        db  0A0h, 000h, 000h, 000h, 000h
        and    al, 0feh
        push   eax
        db  0FFh, 015h, 000h, 000h, 000h, 000h
loc_1_14DA:
        db  0FEh, 00Dh, 000h, 000h, 000h, 000h
        popf
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_14F0:
        pushf
        cli
        db  0A0h, 000h, 000h, 000h, 000h
        push   eax
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        db  0FEh, 005h, 000h, 000h, 000h, 000h
        js     SHORT loc_1_151C
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 000h
        db  0A0h, 000h, 000h, 000h, 000h
        or     al, 1
        push   eax
        db  0FFh, 015h, 000h, 000h, 000h, 000h
loc_1_151C:
        popf
        ret
        db  090h
sub_1_1520:
        pushf
        cli
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 000h
        db  0A0h, 000h, 000h, 000h, 000h
        or     al, 1
        push   eax
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        popf
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_1_1540:
        pushf
        cli
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 000h
        db  0A0h, 000h, 000h, 000h, 000h
        and    al, 0feh
        push   eax
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        popf
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_1560:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   eax
        push   edx
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        inc    dx
        in     al, dx
        pop    edx
        pop    eax
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax + 1b2h], 1
        jne    SHORT loc_1_1588
        xor    al, al
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1588:
        lea    eax, [ebp - 1]
        push   eax
        push   7
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        cmp    byte ptr [ebp - 1], 0ffh
        jne    SHORT loc_1_15A2
        xor    al, al
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_15A2:
        call   sub_1_14C0
        push   0
        push   0fh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        mov    al, 1
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_15C0:
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 005h
        push   eax
        push   edx
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        inc    dx
        in     al, dx
        pop    edx
        pop    eax
        db  0A0h, 000h, 000h, 000h, 000h
        xor    al, 2
        push   eax
        db  0A2h, 000h, 000h, 000h, 000h
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        xor    al, al
        ret    4
        db  08Dh, 049h, 000h
loc_1_15F0:
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 007h
        push   eax
        push   edx
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        inc    dx
        in     al, dx
        pop    edx
        pop    eax
        db  0A0h, 000h, 000h, 000h, 000h
        xor    al, 2
        push   eax
        db  0A2h, 000h, 000h, 000h, 000h
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        xor    al, al
        ret    4
        db  08Dh, 049h, 000h
loc_1_1620:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 11dh], 0
        jne    NEAR PTR loc_1_187E
        lea    edi, [esi + 103h]
        call   sub_1_14C0
        push   edi
        inc    dword ptr [esi + 30h]
        push   7
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [edi]
        test   al, al
        je     SHORT loc_1_165C
        push   eax
        push   7
        db  0FFh, 015h, 000h, 000h, 000h, 000h
loc_1_165C:
        mov    al, byte ptr [edi]
        test   al, 20h
        je     SHORT loc_1_1669
        mov    eax, 8
        jmp    SHORT loc_1_169D
loc_1_1669:
        test   al, 10h
        je     SHORT loc_1_167A
        push   esi
        call   sub_1_0840
        mov    eax, 4
        jmp    SHORT loc_1_169D
loc_1_167A:
        test   al, 0ah
        je     SHORT loc_1_1685
        mov    eax, 2
        jmp    SHORT loc_1_169D
loc_1_1685:
        test   al, 1
        jne    SHORT loc_1_1698
        test   al, 4
        mov    eax, 10h
        je     SHORT loc_1_169D
        push   esi
        call   sub_1_0840
loc_1_1698:
        mov    eax, 1
loc_1_169D:
        xor    ebx, ebx
loc_1_169F:
        cmp    eax, 10h
        jne    SHORT loc_1_16BC
        cmp    dword ptr [esi + 161h], ebx
        je     NEAR PTR loc_1_17A5
        cmp    byte ptr [esi + 13eh], bl
        jne    NEAR PTR loc_1_17A5
loc_1_16BC:
        dec    eax
        cmp    eax, 7
        ja     SHORT loc_1_1726
        xor    ecx, ecx
        db  08Ah, 088h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 08Dh, 000h, 000h, 000h, 000h
loc_1_16D1:
        push   esi
        call   sub_1_0720
        mov    byte ptr [esi + 1b1h], bl
        cmp    byte ptr [esi + 11bh], bl
        jne    SHORT loc_1_1704
        mov    byte ptr [esi + 5bh], bl
        mov    dword ptr [esi + 147h], ebx
        test   byte ptr [edi], 8
        je     SHORT loc_1_16F9
        push   esi
        call   sub_1_2A20
loc_1_16F9:
        test   byte ptr [edi], 2
        je     SHORT loc_1_1704
        push   esi
        call   sub_1_1AE0
loc_1_1704:
        and    byte ptr [edi], 0f5h
        jmp    SHORT loc_1_1726
loc_1_1709:
        mov    byte ptr [esi + 118h], 1
        and    byte ptr [edi], 0efh
loc_1_1713:
        push   esi
        call   sub_1_18B0
        test   al, al
        je     SHORT loc_1_1720
        and    byte ptr [edi], 0fah
loc_1_1720:
        push   esi
        call   sub_1_26C0
loc_1_1726:
        cmp    dword ptr [esi + 161h], ebx
        je     SHORT loc_1_173C
        cmp    byte ptr [esi + 13eh], bl
        jne    SHORT loc_1_173C
        push   esi
        call   sub_1_18B0
loc_1_173C:
        mov    al, byte ptr [edi]
        test   al, 20h
        je     SHORT loc_1_1757
        mov    eax, 8
        jmp    NEAR PTR loc_1_169F
loc_1_174C:
        push   esi
        call   sub_1_0840
        and    byte ptr [edi], 0dfh
        jmp    SHORT loc_1_1726
loc_1_1757:
        test   al, 10h
        je     SHORT loc_1_176B
        push   esi
        call   sub_1_0840
        mov    eax, 4
        jmp    NEAR PTR loc_1_169F
loc_1_176B:
        test   al, 0ah
        je     SHORT loc_1_1779
        mov    eax, 2
        jmp    NEAR PTR loc_1_169F
loc_1_1779:
        test   al, 1
        je     SHORT loc_1_1787
        mov    eax, 1
        jmp    NEAR PTR loc_1_169F
loc_1_1787:
        test   al, 4
        je     SHORT loc_1_179B
        push   esi
        call   sub_1_0840
        mov    eax, 1
        jmp    NEAR PTR loc_1_169F
loc_1_179B:
        mov    eax, 10h
        jmp    NEAR PTR loc_1_169F
loc_1_17A5:
        push   edi
        push   7
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [edi]
        test   al, al
        je     SHORT loc_1_17BD
        push   eax
        push   7
        db  0FFh, 015h, 000h, 000h, 000h, 000h
loc_1_17BD:
        mov    al, byte ptr [edi]
        test   al, 20h
        je     SHORT loc_1_17CA
        mov    eax, 8
        jmp    SHORT loc_1_17FE
loc_1_17CA:
        test   al, 10h
        je     SHORT loc_1_17DB
        push   esi
        call   sub_1_0840
        mov    eax, 4
        jmp    SHORT loc_1_17FE
loc_1_17DB:
        test   al, 0ah
        je     SHORT loc_1_17E6
        mov    eax, 2
        jmp    SHORT loc_1_17FE
loc_1_17E6:
        test   al, 1
        jne    SHORT loc_1_17F9
        test   al, 4
        mov    eax, 10h
        je     SHORT loc_1_17FE
        push   esi
        call   sub_1_0840
loc_1_17F9:
        mov    eax, 1
loc_1_17FE:
        cmp    eax, 10h
        jne    NEAR PTR loc_1_16BC
        mov    byte ptr [esi + 11dh], 0
        call   sub_1_14F0
        mov    byte ptr [esi + 15ch], 1bh
        push   1bh
        push   0fh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        call   sub_1_14F0
        mov    eax, dword ptr [esi + 30h]
        dec    eax
        mov    dword ptr [esi + 30h], eax
        jne    SHORT loc_1_185F
        cmp    byte ptr [esi + 13eh], 0
        jne    SHORT loc_1_184A
        cmp    dword ptr [esi + 16dh], 0
        jne    SHORT loc_1_184A
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_1_185F
loc_1_184A:
        push   1
        add    esi, 179h
        push   esi
        call   sub_1_4232
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
loc_1_185F:
        cmp    dword ptr [esi + 47h], 0
        je     SHORT loc_1_187E
        mov    eax, dword ptr [esi + 4fh]
        cmp    dword ptr [esi + eax*4 + 5dh], 0
        jne    SHORT loc_1_187E
        cmp    byte ptr [esi + 11dh], 0
        jne    SHORT loc_1_187E
        push   esi
        call   sub_1_26C0
loc_1_187E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  08Dh, 049h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 004h, 002h, 004h, 004h, 004h, 003h, 005h
        db  000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_18B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    byte ptr [ebp - 1], 1
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    byte ptr [esi + 11ah], 0
        call   sub_1_06F0
        cmp    byte ptr [esi + 13eh], 0
        jne    SHORT loc_1_18E5
        cmp    byte ptr [esi + 118h], 0
        je     SHORT loc_1_18E5
        push   esi
        call   sub_1_08A0
loc_1_18E5:
        xor    ebx, ebx
loc_1_18E7:
        cmp    byte ptr [esi + 13eh], bl
        jne    NEAR PTR loc_1_19D4
        test   byte ptr [esi + 103h], 4
        jne    NEAR PTR loc_1_1A7B
        mov    al, byte ptr [esi + 0fdh]
        cmp    byte ptr [esi + 0fch], al
        jne    SHORT loc_1_1926
        push   esi
        call   sub_1_06F0
        mov    al, byte ptr [esi + 0fdh]
        cmp    byte ptr [esi + 0fch], al
        je     NEAR PTR loc_1_19D4
loc_1_1926:
        push   esi
        inc    dword ptr [esi + 34h]
        mov    dword ptr [esi + 169h], ebx
        call   sub_1_1FD0
        test   al, al
        je     SHORT loc_1_198F
        xor    ecx, ecx
        xor    eax, eax
        mov    cl, byte ptr [esi + 1b6h]
        mov    al, byte ptr [esi + 1b5h]
        shl    ecx, 8
        add    ecx, eax
        mov    eax, dword ptr [esi + 15dh]
        sub    ecx, 4
        cmp    ecx, eax
        jb     SHORT loc_1_195D
        mov    ecx, eax
loc_1_195D:
        add    ecx, 0eh
        mov    eax, dword ptr [esi + 1b7h]
        push   ecx
        lea    edx, [esi + 1bbh]
        push   eax
        push   edx
        call   sub_1_04C0
        test   al, al
        je     SHORT loc_1_198F
        push   esi
        call   sub_1_2180
        cmp    eax, 3
        je     NEAR PTR loc_1_1A98
        inc    dword ptr [esi + 122h]
        jmp    SHORT loc_1_1994
loc_1_198F:
        mov    eax, 1
loc_1_1994:
        cmp    eax, 3
        je     NEAR PTR loc_1_1A98
        cmp    eax, 1
        jne    SHORT loc_1_19B0
        push   esi
        call   sub_1_06F0
        mov    al, byte ptr [esi + 0fdh]
        jmp    SHORT loc_1_19B6
loc_1_19B0:
        mov    al, byte ptr [esi + 1b4h]
loc_1_19B6:
        push   esi
        mov    byte ptr [esi + 0fch], al
        call   sub_1_0740
        cmp    dword ptr [esi + 34h], 0ah
        jbe    NEAR PTR loc_1_18E7
        xor    eax, eax
        mov    byte ptr [ebp - 1], al
        mov    dword ptr [esi + 34h], eax
loc_1_19D4:
        cmp    byte ptr [esi + 118h], 0
        je     SHORT loc_1_1A15
        mov    byte ptr [esi + 118h], 0
        push   esi
        call   sub_1_0820
        push   0
        push   0dh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        cmp    byte ptr [esi + 5ch], 0
        je     SHORT loc_1_1A15
        cmp    dword ptr [esi + 57h], -1
        je     SHORT loc_1_1A15
        mov    byte ptr [esi + 5ch], 0
        mov    byte ptr [esi + 1b1h], 0
        mov    byte ptr [esi + 5bh], 1
        push   esi
        call   sub_1_0680
loc_1_1A15:
        cmp    dword ptr [esi + 161h], 0
        je     SHORT loc_1_1A66
loc_1_1A1E:
        cmp    byte ptr [esi + 13eh], 0
        jne    SHORT loc_1_1A6F
        mov    edi, dword ptr [esi + 161h]
        push   edi
        push   esi
        mov    eax, dword ptr [edi + 24h]
        mov    dword ptr [esi + 161h], eax
        mov    dword ptr [esi + 169h], edi
        inc    dword ptr [esi + 11eh]
        mov    ebx, dword ptr [edi + 28h]
        call   sub_1_1E30
        push   0
        mov    ecx, dword ptr [edi + 28h]
        push   edi
        mov    eax, dword ptr [ecx]
        push   eax
        call   sub_1_419A
        dec    dword ptr [ebx + 1ch]
        cmp    dword ptr [esi + 161h], 0
        jne    SHORT loc_1_1A1E
loc_1_1A66:
        cmp    byte ptr [esi + 13eh], 0
        je     SHORT loc_1_1AAA
loc_1_1A6F:
        mov    al, byte ptr [ebp - 1]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1A7B:
        push   esi
        call   sub_1_06F0
        mov    al, byte ptr [esi + 0fdh]
        push   esi
        mov    byte ptr [esi + 0fch], al
        call   sub_1_0740
        jmp    NEAR PTR loc_1_19D4
loc_1_1A98:
        mov    byte ptr [esi + 15ch], 1ah
        push   esi
        call   sub_1_01D0
        jmp    NEAR PTR loc_1_19D4
loc_1_1AAA:
        cmp    byte ptr [esi + 11ah], 0
        je     SHORT loc_1_1AC6
        mov    eax, dword ptr [esi + 14bh]
        push   eax
        call   sub_1_3FE0
        mov    byte ptr [esi + 11ah], 0
loc_1_1AC6:
        mov    al, byte ptr [ebp - 1]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1AE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [esi + 11bh], 1
        cmp    dword ptr [esi + 57h], -1
        jne    SHORT loc_1_1B1E
        push   4
        mov    eax, dword ptr [esi + 4]
        mov    byte ptr [esi + 11bh], 0
        push   1
        push   0aa000006h
        push   eax
        call   sub_1_4328
        add    esp, 10h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1B1E:
        push   esi
        call   sub_1_0720
        test   byte ptr [esi + 0feh], 1
        je     SHORT loc_1_1B35
        inc    dword ptr [esi + 11eh]
        jmp    SHORT loc_1_1B3B
loc_1_1B35:
        inc    dword ptr [esi + 126h]
loc_1_1B3B:
        mov    ecx, dword ptr [esi + 57h]
        mov    eax, dword ptr [esi + ecx*4 + 0bdh]
        lea    edx, [esi + ecx*4]
        add    eax, 0ffh
        shr    eax, 8
        mov    dword ptr [ebp - 4], eax
        add    eax, ecx
        cmp    eax, ecx
        jbe    SHORT loc_1_1B70
        add    edx, 5dh
loc_1_1B5C:
        mov    dword ptr [edx], 0
        add    edx, 4
        inc    ecx
        mov    eax, dword ptr [esi + 57h]
        add    eax, dword ptr [ebp - 4]
        cmp    eax, ecx
        ja     SHORT loc_1_1B5C
loc_1_1B70:
        mov    eax, dword ptr [esi + 53h]
        add    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 53h], eax
        cmp    eax, 0ch
        jne    SHORT loc_1_1B85
        mov    dword ptr [esi + 53h], 0
loc_1_1B85:
        mov    eax, dword ptr [esi + 53h]
        cmp    dword ptr [esi + eax*4 + 5dh], 0
        jne    SHORT loc_1_1B9B
        cmp    dword ptr [esi + 4fh], eax
        je     SHORT loc_1_1B9B
        mov    dword ptr [esi + 53h], 0
loc_1_1B9B:
        mov    eax, dword ptr [esi + 57h]
        mov    dword ptr [esi + eax*4 + 8dh], 0
        mov    ecx, dword ptr [esi + 53h]
        lea    eax, [esi + ecx*4 + 5dh]
        mov    edx, dword ptr [eax]
        test   edx, edx
        je     SHORT loc_1_1BD8
        cmp    edx, 1
        je     NEAR PTR loc_1_1DB9
        cmp    edx, 2
        je     NEAR PTR loc_1_1DD6
        mov    byte ptr [esi + 11bh], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1BD8:
        cmp    byte ptr [esi + 13eh], 0
        je     SHORT loc_1_1C00
        mov    dword ptr [esi + 57h], 0ffffffffh
        mov    byte ptr [esi + 11bh], 0
        push   2
        push   esi
        call   sub_1_3930
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1C00:
        mov    ebx, dword ptr [esi + 47h]
        test   ebx, ebx
        je     NEAR PTR loc_1_1DA2
        lea    edi, [ebx + 24h]
        lea    ecx, [ebp - 4]
        push   ecx
        push   0
        mov    eax, dword ptr [edi]
        push   0
        push   0
        mov    dword ptr [esi + 47h], eax
        push   ebx
        call   sub_1_4280
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0ffh
        shr    eax, 8
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [esi + 4fh]
        add    eax, dword ptr [ebp - 4]
        cmp    eax, 0ch
        jbe    SHORT loc_1_1C43
        mov    dword ptr [esi + 4fh], 0
loc_1_1C43:
        mov    ecx, dword ptr [esi + 4fh]
        mov    eax, dword ptr [ebp - 4]
        add    eax, ecx
        mov    dword ptr [esi + 53h], ecx
        cmp    eax, ecx
        jbe    SHORT loc_1_1C6A
        lea    eax, [esi + ecx*4 + 5dh]
loc_1_1C56:
        mov    dword ptr [eax], 2
        add    eax, 4
        inc    ecx
        mov    edx, dword ptr [esi + 4fh]
        add    edx, dword ptr [ebp - 4]
        cmp    edx, ecx
        ja     SHORT loc_1_1C56
loc_1_1C6A:
        mov    eax, dword ptr [esi + 4fh]
        mov    dword ptr [esi + eax*4 + 8dh], ebx
        mov    eax, dword ptr [esi + 4fh]
        mov    ecx, dword ptr [esi + 53h]
        lea    eax, [esi + eax*4]
        add    eax, 0bdh
        mov    dword ptr [esi + 57h], ecx
        push   eax
        push   ebx
        push   esi
        call   sub_1_0240
        test   al, al
        jne    SHORT loc_1_1CE3
        push   0c0000001h
        mov    eax, dword ptr [ebx + 28h]
        push   ebx
        mov    ecx, dword ptr [eax]
        push   ecx
        call   sub_1_419A
        mov    edx, dword ptr [esi + 4fh]
        mov    eax, dword ptr [ebp - 4]
        add    eax, edx
        cmp    eax, edx
        jbe    SHORT loc_1_1CC6
        lea    eax, [esi + edx*4 + 5dh]
loc_1_1CB2:
        mov    dword ptr [eax], 0
        add    eax, 4
        inc    edx
        mov    ecx, dword ptr [esi + 4fh]
        add    ecx, dword ptr [ebp - 4]
        cmp    ecx, edx
        ja     SHORT loc_1_1CB2
loc_1_1CC6:
        mov    eax, dword ptr [esi + 4fh]
        push   esi
        mov    dword ptr [esi + eax*4 + 8dh], 0
        call   sub_1_26C0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1CE3:
        mov    ecx, dword ptr [esi + 4fh]
        mov    eax, dword ptr [esi + ecx*4 + 0bdh]
        cmp    eax, 3ch
        jae    SHORT loc_1_1D12
        shl    ecx, 8
        mov    edx, 3ch
        sub    edx, eax
        push   edx
        db  068h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [esi + 0edh]
        add    edx, ecx
        add    edx, eax
        push   edx
        call   sub_1_03E0
loc_1_1D12:
        mov    eax, dword ptr [esi + 4fh]
        add    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 4fh], eax
        cmp    eax, 0ch
        jne    SHORT loc_1_1D27
        mov    dword ptr [esi + 4fh], 0
loc_1_1D27:
        mov    byte ptr [esi + 11bh], 0
        cmp    byte ptr [esi + 118h], 0
        je     SHORT loc_1_1D3D
        mov    byte ptr [esi + 5ch], 1
        jmp    SHORT loc_1_1D47
loc_1_1D3D:
        mov    byte ptr [esi + 5bh], 1
        push   esi
        call   sub_1_0680
loc_1_1D47:
        cmp    byte ptr [edi + 8], 0
        jne    SHORT loc_1_1D6D
        mov    edi, dword ptr [edi + 4]
        push   0
        push   ebx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_419A
        push   esi
        dec    dword ptr [edi + 1ch]
        call   sub_1_26C0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1D6D:
        cmp    dword ptr [esi + 161h], 0
        jne    SHORT loc_1_1D7E
        mov    dword ptr [esi + 161h], ebx
        jmp    SHORT loc_1_1D87
loc_1_1D7E:
        mov    eax, dword ptr [esi + 165h]
        mov    dword ptr [eax + 24h], ebx
loc_1_1D87:
        push   esi
        mov    dword ptr [esi + 165h], ebx
        mov    dword ptr [edi], 0
        call   sub_1_26C0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1DA2:
        mov    dword ptr [esi + 57h], 0ffffffffh
        mov    byte ptr [esi + 11bh], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1DB9:
        mov    dword ptr [esi + 57h], 0ffffffffh
        mov    byte ptr [esi + 11bh], 0
        push   esi
        call   sub_1_26C0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1DD6:
        cmp    byte ptr [esi + 13eh], 0
        je     SHORT loc_1_1E04
        mov    dword ptr [esi + 57h], 0ffffffffh
        push   2
        mov    dword ptr [eax], 0
        mov    byte ptr [esi + 11bh], 0
        push   esi
        call   sub_1_3930
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1E04:
        push   esi
        mov    dword ptr [esi + 57h], ecx
        mov    byte ptr [esi + 11bh], 0
        mov    byte ptr [esi + 5bh], 1
        call   sub_1_0680
        push   esi
        call   sub_1_26C0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1E30:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        lea    eax, [ebp - 4]
        push   esi
        mov    ebx, dword ptr [ebp + 0ch]
        push   edi
        push   eax
        push   0
        push   0
        push   0
        push   ebx
        call   sub_1_4280
        cmp    dword ptr [ebp - 4], 6
        jae    SHORT loc_1_1E60
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1E60:
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [esi + 15dh]
        add    edi, 0eh
        cmp    edi, eax
        jb     SHORT loc_1_1E75
        mov    edi, eax
loc_1_1E75:
        push   edi
        lea    eax, [esi + 1bbh]
        push   0
        push   ebx
        push   eax
        call   sub_1_1EF0
        cmp    edi, 0eh
        jae    SHORT loc_1_1EA8
        cmp    edi, 6
        jb     SHORT loc_1_1ED1
        push   0
        lea    eax, [esi + 1bbh]
        push   0
        mov    edx, dword ptr [esi + 14bh]
        push   0
        push   edi
        push   eax
        push   eax
        push   esi
        push   edx
        jmp    SHORT loc_1_1ECC
loc_1_1EA8:
        mov    eax, dword ptr [ebp - 4]
        sub    edi, 0eh
        sub    eax, 0eh
        lea    ecx, [esi + 1c9h]
        push   eax
        push   edi
        push   ecx
        lea    eax, [esi + 1bbh]
        push   0eh
        mov    ecx, dword ptr [esi + 14bh]
        push   eax
        push   eax
        push   esi
        push   ecx
loc_1_1ECC:
        call   sub_1_3E00
loc_1_1ED1:
        xor    eax, eax
        pop    edi
        mov    byte ptr [esi + 11ah], 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1EF0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        lea    eax, [ebp - 4]
        push   esi
        push   edi
        xor    esi, esi
        xor    edi, edi
        push   esi
        mov    ecx, dword ptr [ebp + 0ch]
        push   eax
        push   esi
        push   esi
        push   ecx
        call   sub_1_4280
        mov    ebx, dword ptr [ebp + 10h]
        cmp    dword ptr [ebp - 4], esi
        je     SHORT loc_1_1F44
loc_1_1F15:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_42A4
        mov    eax, dword ptr [ebp - 8]
        add    eax, edi
        cmp    ebx, eax
        jb     SHORT loc_1_1F44
        mov    edi, eax
        mov    ecx, dword ptr [ebp - 4]
        lea    eax, [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_42C8
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_1F15
loc_1_1F44:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_1F55
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_1F55:
        mov    eax, ebx
        sub    eax, edi
        sub    edi, ebx
        add    dword ptr [ebp - 0ch], eax
        add    dword ptr [ebp - 8], edi
        mov    edi, dword ptr [ebp + 14h]
loc_1_1F64:
        mov    eax, dword ptr [ebp - 8]
        add    eax, esi
        cmp    eax, edi
        jbe    SHORT loc_1_1F73
        mov    ebx, edi
        sub    ebx, esi
        jmp    SHORT loc_1_1F76
loc_1_1F73:
        mov    ebx, dword ptr [ebp - 8]
loc_1_1F76:
        push   ebx
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, esi
        add    esi, ebx
        push   eax
        call   sub_1_433C
        cmp    edi, esi
        je     SHORT loc_1_1FB2
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_42C8
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_1FBD
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_42A4
        jmp    SHORT loc_1_1F64
loc_1_1FB2:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_1FBD:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_1FD0:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 18h
        xor    ecx, ecx
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   6
        mov    al, byte ptr [esi + 0fch]
        mov    cl, byte ptr [esi + 0fah]
        sub    eax, ecx
        lea    ecx, [ebp - 18h]
        shl    eax, 8
        add    eax, dword ptr [esi + 0f1h]
        push   eax
        mov    dword ptr [ebp - 0ch], eax
        push   ecx
        call   sub_1_04C0
        test   al, al
        jne    SHORT loc_1_2015
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2015:
        xor    ecx, ecx
        lea    edx, [ebp - 18h]
        add    dword ptr [ebp - 0ch], 4
loc_1_201E:
        lea    edi, [edx + 1]
        lea    eax, [edx + 2]
        mov    dword ptr [ebp - 8], edi
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [edx + 3]
        lea    edi, [edx + 4]
        mov    byte ptr [ebp - 1], 0
        test   byte ptr [edx], 5eh
        mov    bl, byte ptr [eax]
        mov    byte ptr [ebp - 2], bl
        jne    SHORT loc_1_20B0
        test   byte ptr [edx], 20h
        je     SHORT loc_1_204C
        test   ecx, ecx
        jne    SHORT loc_1_204C
        test   byte ptr [edi], 1
        je     SHORT loc_1_20B0
loc_1_204C:
        mov    edi, dword ptr [ebp - 10h]
        cmp    byte ptr [edi], bl
        jne    SHORT loc_1_206E
        mov    edi, dword ptr [ebp - 8]
        mov    byte ptr [ebp - 2], bl
        mov    bl, byte ptr [edi]
        mov    edi, dword ptr [ebp - 10h]
        sub    bl, byte ptr [esi + 0fch]
        dec    bl
        mov    byte ptr [eax], bl
        cmp    byte ptr [edi], 0fch
        jbe    SHORT loc_1_206E
        inc    eax
loc_1_206E:
        xor    ebx, ebx
        mov    edi, dword ptr [ebp - 10h]
        mov    bl, byte ptr [eax]
        shl    ebx, 8
        movzx  edi, byte ptr [edi]
        lea    edi, [ebx + edi - 4]
        cmp    edi, 5eah
        ja     SHORT loc_1_208C
        cmp    edi, 3ch
        jae    SHORT loc_1_2095
loc_1_208C:
        mov    byte ptr [ebp - 1], 1
        mov    bl, byte ptr [ebp - 2]
        mov    byte ptr [eax], bl
loc_1_2095:
        cmp    byte ptr [ebp - 1], 0
        jne    SHORT loc_1_20BA
        mov    edi, dword ptr [ebp - 8]
        mov    al, byte ptr [edi]
        cmp    byte ptr [esi + 0fah], al
        ja     SHORT loc_1_20B0
        cmp    byte ptr [esi + 0fbh], al
        jae    SHORT loc_1_20B4
loc_1_20B0:
        mov    byte ptr [ebp - 1], 1
loc_1_20B4:
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_20EC
loc_1_20BA:
        test   ecx, ecx
        je     SHORT loc_1_20C5
        cmp    ecx, 1
        je     SHORT loc_1_20DA
        jmp    SHORT loc_1_20EB
loc_1_20C5:
        mov    edx, dword ptr [ebp - 8]
        inc    dword ptr [ebp - 0ch]
        cmp    byte ptr [esi + 11ch], 0
        jne    SHORT loc_1_20EB
        inc    edx
        inc    dword ptr [ebp - 0ch]
        jmp    SHORT loc_1_20EB
loc_1_20DA:
        dec    edx
        dec    dword ptr [ebp - 0ch]
        cmp    byte ptr [esi + 11ch], 0
        jne    SHORT loc_1_20EB
        dec    edx
        dec    dword ptr [ebp - 0ch]
loc_1_20EB:
        inc    ecx
loc_1_20EC:
        cmp    ecx, 2
        jae    SHORT loc_1_20FB
        cmp    byte ptr [ebp - 1], 0
        jne    NEAR PTR loc_1_201E
loc_1_20FB:
        mov    al, byte ptr [edx]
        inc    edx
        inc    edx
        mov    byte ptr [esi + 1b3h], al
        mov    cl, byte ptr [edx - 1]
        inc    edx
        mov    edi, dword ptr [ebp - 0ch]
        mov    byte ptr [esi + 1b4h], cl
        mov    al, byte ptr [edx - 1]
        mov    byte ptr [esi + 1b5h], al
        and    eax, 0ffh
        mov    dl, byte ptr [edx]
        mov    byte ptr [esi + 1b6h], dl
        mov    dword ptr [esi + 1b7h], edi
        and    edx, 0ffh
        shl    edx, 8
        lea    eax, [edx + eax - 4]
        cmp    eax, 5eah
        ja     SHORT loc_1_2152
        cmp    eax, 3ch
        jb     SHORT loc_1_2152
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2152:
        cmp    byte ptr [esi + 0fah], cl
        ja     SHORT loc_1_216D
        cmp    byte ptr [esi + 0fbh], cl
        jb     SHORT loc_1_216D
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_216D:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_2180:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    bl, byte ptr [esi + 1b6h]
        mov    al, byte ptr [esi + 0fch]
        mov    cl, byte ptr [esi + 0fbh]
        lea    edx, [eax + ebx + 1]
        cmp    dl, cl
        jb     SHORT loc_1_21AA
        sub    dl, cl
        add    dl, byte ptr [esi + 0fah]
loc_1_21AA:
        mov    al, byte ptr [esi + 1b4h]
        cmp    dl, al
        je     SHORT loc_1_21CF
        inc    dl
        cmp    cl, dl
        jne    SHORT loc_1_21C0
        mov    dl, byte ptr [esi + 0fah]
loc_1_21C0:
        cmp    al, dl
        je     SHORT loc_1_21CF
        mov    eax, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_21CF:
        cmp    byte ptr [esi + 0fah], al
        ja     NEAR PTR loc_1_225C
        cmp    al, cl
        ja     SHORT loc_1_225C
        xor    ecx, ecx
        xor    eax, eax
        mov    cl, byte ptr [esi + 1b5h]
        mov    al, bl
        shl    eax, 8
        lea    eax, [ecx + eax - 4]
        cmp    eax, 5eah
        jbe    SHORT loc_1_2204
        mov    eax, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_2204:
        mov    ecx, dword ptr [esi + 15dh]
        add    ecx, 0eh
        cmp    ecx, eax
        jb     SHORT loc_1_2213
        mov    ecx, eax
loc_1_2213:
        mov    dword ptr [esi + 2b7h], eax
        cmp    ecx, 0eh
        jae    SHORT loc_1_2227
        push   0
        push   0
        push   0
        push   ecx
        jmp    SHORT loc_1_2238
loc_1_2227:
        sub    eax, 0eh
        sub    ecx, 0eh
        push   eax
        lea    edx, [esi + 1c9h]
        push   ecx
        push   edx
        push   0eh
loc_1_2238:
        lea    eax, [esi + 1bbh]
        mov    edx, dword ptr [esi + 14bh]
        push   eax
        push   eax
        push   esi
        push   edx
        call   sub_1_3E00
        mov    byte ptr [esi + 11ah], 1
        xor    eax, eax
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_225C:
        mov    eax, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2270:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        call   sub_1_14C0
        mov    edi, dword ptr [ebp + 10h]
        mov    esi, dword ptr [ebp + 0ch]
        add    edi, 0eh
        cmp    dword ptr [esi + 169h], 0
        je     NEAR PTR loc_1_2321
        push   0
        lea    eax, [ebp - 8]
        push   eax
        mov    ecx, dword ptr [ebp + 18h]
        push   0
        mov    ebx, edi
        push   0
        push   ecx
        call   sub_1_4280
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_22EF
loc_1_22AF:
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 18h]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_42A4
        mov    ecx, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [esi + 169h]
        push   ecx
        mov    eax, dword ptr [ebp - 18h]
        push   ebx
        push   edx
        push   eax
        call   sub_1_1EF0
        add    ebx, eax
        cmp    eax, dword ptr [ebp - 0ch]
        jb     SHORT loc_1_22EF
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_42C8
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_22AF
loc_1_22EF:
        call   sub_1_14F0
        cmp    edi, ebx
        jbe    SHORT loc_1_230F
        mov    ecx, dword ptr [ebp + 1ch]
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [ecx], 0
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_230F:
        sub    ebx, edi
        mov    ecx, dword ptr [ebp + 1ch]
        xor    eax, eax
        pop    edi
        pop    esi
        mov    dword ptr [ecx], ebx
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_2321:
        mov    eax, dword ptr [esi + 2b7h]
        mov    ecx, dword ptr [ebp + 14h]
        lea    edx, [edi + ecx]
        cmp    edx, eax
        jbe    SHORT loc_1_2358
        cmp    edi, eax
        jbe    SHORT loc_1_2351
        mov    ecx, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx], 0
        call   sub_1_14F0
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_2351:
        sub    eax, edi
        mov    dword ptr [ebp - 1ch], eax
        jmp    SHORT loc_1_235B
loc_1_2358:
        mov    dword ptr [ebp - 1ch], ecx
loc_1_235B:
        mov    eax, dword ptr [ebp - 1ch]
        add    edi, dword ptr [esi + 1b7h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [esi + 0f5h]
        cmp    edi, eax
        jbe    SHORT loc_1_2379
        sub    edi, eax
        add    edi, dword ptr [esi + 0f1h]
loc_1_2379:
        push   0
        lea    eax, [ebp - 8]
        push   eax
        mov    ecx, dword ptr [ebp + 18h]
        push   0
        push   0
        push   ecx
        call   sub_1_4280
        lea    ecx, [ebp - 0ch]
        lea    edx, [ebp - 14h]
        push   ecx
        mov    eax, dword ptr [ebp - 8]
        push   edx
        push   eax
        call   sub_1_42A4
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [ebp - 1ch], 0
        je     NEAR PTR loc_1_243A
loc_1_23AE:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ebx, dword ptr [ebp - 10h]
        sub    eax, dword ptr [ebp - 4]
        cmp    eax, dword ptr [ebp - 10h]
        ja     SHORT loc_1_23BE
        mov    ebx, eax
loc_1_23BE:
        mov    eax, dword ptr [esi + 0f5h]
        lea    ecx, [ebx + edi]
        cmp    ecx, eax
        jbe    SHORT loc_1_23CF
        mov    ebx, eax
        sub    ebx, edi
loc_1_23CF:
        push   ebx
        mov    eax, dword ptr [ebp - 14h]
        push   edi
        add    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_04C0
        test   al, al
        je     NEAR PTR loc_1_24AD
        add    edi, ebx
        sub    dword ptr [ebp - 10h], ebx
        je     SHORT loc_1_243A
        cmp    dword ptr [esi + 0f5h], edi
        jne    SHORT loc_1_23FA
        mov    edi, dword ptr [esi + 0f1h]
loc_1_23FA:
        add    dword ptr [ebp - 4], ebx
        mov    eax, dword ptr [ebp - 4]
        cmp    eax, dword ptr [ebp - 0ch]
        jne    SHORT loc_1_2430
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_42C8
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_243A
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 14h]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_42A4
        mov    dword ptr [ebp - 4], 0
loc_1_2430:
        cmp    dword ptr [ebp - 10h], 0
        jne    NEAR PTR loc_1_23AE
loc_1_243A:
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp + 1ch]
        sub    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ecx], eax
        cmp    byte ptr [esi + 118h], 0
        jne    NEAR PTR loc_1_2575
        cmp    dword ptr [esi + 57h], -1
        je     NEAR PTR loc_1_2575
        lea    eax, [ebp - 4]
        push   eax
        push   7
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        test   byte ptr [ebp - 4], 8
        je     SHORT loc_1_2474
        push   esi
        call   sub_1_2A20
loc_1_2474:
        test   byte ptr [ebp - 4], 2
        je     NEAR PTR loc_1_2575
        push   2
        push   7
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   esi
        call   sub_1_0720
        mov    byte ptr [esi + 5bh], 0
        mov    dword ptr [esi + 147h], 0
        test   byte ptr [esi + 0feh], 1
        je     SHORT loc_1_24E0
        inc    dword ptr [esi + 11eh]
        jmp    SHORT loc_1_24E6
loc_1_24AD:
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp + 1ch]
        sub    eax, dword ptr [ebp - 10h]
        push   2
        push   1
        mov    dword ptr [ecx], eax
        push   0aa000003h
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_1_4328
        add    esp, 10h
        call   sub_1_14F0
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_24E0:
        inc    dword ptr [esi + 126h]
loc_1_24E6:
        mov    edi, dword ptr [esi + 57h]
        mov    edx, dword ptr [esi + edi*4 + 0bdh]
        lea    ecx, [esi + edi*4]
        add    edx, 0ffh
        shr    edx, 8
        lea    eax, [edx + edi]
        cmp    eax, edi
        jbe    SHORT loc_1_2519
        add    ecx, 5dh
loc_1_2506:
        mov    dword ptr [ecx], 0
        add    ecx, 4
        inc    edi
        mov    eax, dword ptr [esi + 57h]
        add    eax, edx
        cmp    eax, edi
        ja     SHORT loc_1_2506
loc_1_2519:
        mov    eax, dword ptr [esi + 53h]
        add    eax, edx
        mov    dword ptr [esi + 53h], eax
        cmp    eax, 0ch
        jne    SHORT loc_1_252D
        mov    dword ptr [esi + 53h], 0
loc_1_252D:
        mov    eax, dword ptr [esi + 53h]
        cmp    dword ptr [esi + eax*4 + 5dh], 0
        jne    SHORT loc_1_2543
        cmp    dword ptr [esi + 4fh], eax
        je     SHORT loc_1_2543
        mov    dword ptr [esi + 53h], 0
loc_1_2543:
        mov    eax, dword ptr [esi + 57h]
        xor    ecx, ecx
        mov    dword ptr [esi + eax*4 + 8dh], ecx
        mov    byte ptr [esi + 1b1h], cl
        mov    eax, dword ptr [esi + 53h]
        cmp    dword ptr [esi + eax*4 + 5dh], 2
        jne    SHORT loc_1_256E
        push   esi
        mov    dword ptr [esi + 57h], eax
        mov    byte ptr [esi + 5bh], 1
        call   sub_1_0680
        jmp    SHORT loc_1_2575
loc_1_256E:
        mov    dword ptr [esi + 57h], 0ffffffffh
loc_1_2575:
        call   sub_1_14F0
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2590:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebx + 8]
        add    edi, 24h
        call   sub_1_14C0
        cmp    byte ptr [ebx + 20h], 0
        je     SHORT loc_1_25C3
        call   sub_1_14F0
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_25C3:
        cmp    byte ptr [esi + 13eh], 0
        je     SHORT loc_1_25DF
        call   sub_1_14F0
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_25DF:
        push   0
        lea    eax, [ebp - 8]
        push   eax
        mov    ecx, dword ptr [ebp + 0ch]
        push   0
        inc    dword ptr [ebx + 1ch]
        push   0
        inc    dword ptr [esi + 30h]
        push   ecx
        mov    dword ptr [edi + 4], ebx
        call   sub_1_4280
        lea    ecx, [ebp - 0ch]
        lea    edx, [ebp - 4]
        push   ecx
        mov    eax, dword ptr [ebp - 8]
        push   edx
        push   eax
        call   sub_1_42A4
        test   byte ptr [ebx + 71h], 2
        je     SHORT loc_1_2618
        mov    byte ptr [edi + 8], 0
        jmp    SHORT loc_1_262B
loc_1_2618:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 14bh]
        push   eax
        push   ecx
        call   sub_1_4080
        mov    byte ptr [edi + 8], al
loc_1_262B:
        cmp    dword ptr [esi + 47h], 0
        je     SHORT loc_1_263C
        mov    eax, dword ptr [esi + 4bh]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 24h], ecx
        jmp    SHORT loc_1_2642
loc_1_263C:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 47h], ecx
loc_1_2642:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 4bh], ecx
        mov    dword ptr [edi], 0
        mov    eax, dword ptr [esi + 30h]
        dec    eax
        mov    dword ptr [esi + 30h], eax
        jne    SHORT loc_1_267F
        cmp    byte ptr [esi + 13eh], 0
        jne    SHORT loc_1_266F
        cmp    dword ptr [esi + 16dh], 0
        jne    SHORT loc_1_266F
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_1_267F
loc_1_266F:
        push   1
        add    esi, 179h
        push   esi
        call   sub_1_4232
        jmp    SHORT loc_1_269E
loc_1_267F:
        cmp    dword ptr [esi + 47h], 0
        je     SHORT loc_1_269E
        mov    eax, dword ptr [esi + 4fh]
        cmp    dword ptr [esi + eax*4 + 5dh], 0
        jne    SHORT loc_1_269E
        cmp    byte ptr [esi + 11dh], 0
        jne    SHORT loc_1_269E
        push   esi
        call   sub_1_26C0
loc_1_269E:
        call   sub_1_14F0
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_26C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        call   sub_1_14C0
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 47h], 0
        je     NEAR PTR loc_1_291A
loc_1_26DB:
        mov    eax, dword ptr [esi + 4fh]
        cmp    dword ptr [esi + eax*4 + 5dh], 0
        jne    NEAR PTR loc_1_291A
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [esi + 47h]
        push   eax
        push   0
        push   0
        push   0
        push   ecx
        call   sub_1_4280
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 57h]
        add    eax, 0ffh
        shr    eax, 8
        cmp    ecx, -1
        mov    dword ptr [ebp - 4], eax
        jne    SHORT loc_1_276F
        mov    edx, dword ptr [esi + 53h]
        cmp    dword ptr [esi + edx*4 + 5dh], 0
        jne    SHORT loc_1_2730
        mov    eax, dword ptr [esi + 4fh]
        add    eax, dword ptr [ebp - 4]
        cmp    eax, 0ch
        jbe    SHORT loc_1_27A4
        mov    dword ptr [esi + 4fh], 0
        jmp    SHORT loc_1_27A4
loc_1_2730:
        mov    ecx, dword ptr [esi + 4fh]
        mov    eax, dword ptr [ebp - 4]
        cmp    ecx, edx
        jae    SHORT loc_1_274E
        add    eax, ecx
        cmp    eax, edx
        jbe    SHORT loc_1_27A4
        call   sub_1_14F0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_274E:
        add    eax, ecx
        cmp    eax, 0ch
        jbe    SHORT loc_1_27A4
        mov    dword ptr [esi + 4fh], 0
        cmp    dword ptr [ebp - 4], edx
        jbe    SHORT loc_1_27A4
        call   sub_1_14F0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_276F:
        mov    edx, dword ptr [esi + 4fh]
        mov    eax, dword ptr [ebp - 4]
        cmp    ecx, edx
        jbe    SHORT loc_1_278D
        add    eax, edx
        cmp    eax, ecx
        jbe    SHORT loc_1_27A4
        call   sub_1_14F0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_278D:
        add    eax, edx
        cmp    eax, 0ch
        jbe    SHORT loc_1_27A4
        mov    dword ptr [esi + 4fh], 0
        cmp    dword ptr [ebp - 4], ecx
        ja     NEAR PTR loc_1_2928
loc_1_27A4:
        mov    ecx, dword ptr [esi + 4fh]
        mov    ebx, dword ptr [esi + 47h]
        mov    dword ptr [ebp - 8], ecx
        mov    eax, dword ptr [ebx + 24h]
        mov    dword ptr [esi + 47h], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, ecx
        cmp    eax, ecx
        jbe    SHORT loc_1_27CA
        lea    edi, [esi + ecx*4 + 5dh]
        mov    eax, 1
        mov    ecx, dword ptr [ebp - 4]
        rep stosd
loc_1_27CA:
        mov    ecx, dword ptr [ebp - 8]
        lea    edi, [esi + ecx*4 + 0bdh]
        mov    dword ptr [esi + ecx*4 + 8dh], ebx
        lea    eax, [esi + ecx*4]
        push   edi
        push   ebx
        mov    dword ptr [ebp - 0ch], eax
        push   esi
        call   sub_1_0240
        test   al, al
        jne    SHORT loc_1_282E
        push   0c0000001h
        mov    eax, dword ptr [ebx + 28h]
        push   ebx
        mov    ecx, dword ptr [eax]
        push   ecx
        call   sub_1_419A
        mov    eax, dword ptr [ebp - 4]
        add    eax, dword ptr [ebp - 8]
        cmp    eax, dword ptr [ebp - 8]
        jbe    SHORT loc_1_2816
        mov    edi, dword ptr [ebp - 0ch]
        xor    eax, eax
        add    edi, 5dh
        mov    ecx, dword ptr [ebp - 4]
        rep stosd
loc_1_2816:
        mov    eax, dword ptr [ebx + 28h]
        mov    ecx, dword ptr [ebp - 0ch]
        dec    dword ptr [eax + 1ch]
        mov    dword ptr [ecx + 8dh], 0
        jmp    NEAR PTR loc_1_2910
loc_1_282E:
        mov    edx, dword ptr [edi]
        cmp    edx, 3ch
        jae    SHORT loc_1_2858
        mov    eax, 3ch
        mov    ecx, dword ptr [ebp - 8]
        shl    ecx, 8
        sub    eax, edx
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esi + 0edh]
        add    eax, ecx
        add    eax, edx
        push   eax
        call   sub_1_03E0
loc_1_2858:
        cmp    byte ptr [esi + 13eh], 0
        jne    NEAR PTR loc_1_2936
        mov    eax, dword ptr [ebp - 4]
        add    eax, dword ptr [ebp - 8]
        cmp    eax, dword ptr [ebp - 8]
        jbe    SHORT loc_1_2880
        mov    edi, dword ptr [ebp - 0ch]
        mov    eax, 2
        add    edi, 5dh
        mov    ecx, dword ptr [ebp - 4]
        rep stosd
loc_1_2880:
        mov    eax, dword ptr [esi + 4fh]
        add    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 4fh], eax
        cmp    eax, 0ch
        jne    SHORT loc_1_2895
        mov    dword ptr [esi + 4fh], 0
loc_1_2895:
        cmp    dword ptr [esi + 57h], -1
        jne    SHORT loc_1_28D0
        mov    eax, dword ptr [esi + 53h]
        cmp    dword ptr [esi + eax*4 + 5dh], 0
        jne    SHORT loc_1_28B1
        cmp    dword ptr [esi + 4fh], eax
        je     SHORT loc_1_28B1
        mov    dword ptr [esi + 53h], 0
loc_1_28B1:
        cmp    byte ptr [esi + 118h], 0
        mov    eax, dword ptr [esi + 53h]
        mov    dword ptr [esi + 57h], eax
        je     SHORT loc_1_28C6
        mov    byte ptr [esi + 5ch], 1
        jmp    SHORT loc_1_28D0
loc_1_28C6:
        mov    byte ptr [esi + 5bh], 1
        push   esi
        call   sub_1_0680
loc_1_28D0:
        cmp    byte ptr [ebx + 2ch], 0
        jne    SHORT loc_1_28E9
        mov    edi, dword ptr [ebx + 28h]
        push   0
        push   ebx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_419A
        dec    dword ptr [edi + 1ch]
        jmp    SHORT loc_1_2910
loc_1_28E9:
        cmp    dword ptr [esi + 161h], 0
        jne    SHORT loc_1_28FA
        mov    dword ptr [esi + 161h], ebx
        jmp    SHORT loc_1_2903
loc_1_28FA:
        mov    eax, dword ptr [esi + 165h]
        mov    dword ptr [eax + 24h], ebx
loc_1_2903:
        mov    dword ptr [esi + 165h], ebx
        mov    dword ptr [ebx + 24h], 0
loc_1_2910:
        cmp    dword ptr [esi + 47h], 0
        jne    NEAR PTR loc_1_26DB
loc_1_291A:
        call   sub_1_14F0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2928:
        call   sub_1_14F0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2936:
        mov    eax, dword ptr [ebx + 28h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, dword ptr [ebp - 8]
        cmp    eax, dword ptr [ebp - 8]
        jbe    SHORT loc_1_2955
        mov    ecx, dword ptr [ebp - 8]
        xor    eax, eax
        lea    edi, [esi + ecx*4 + 5dh]
        mov    ecx, dword ptr [ebp - 4]
        rep stosd
loc_1_2955:
        push   0
        mov    eax, dword ptr [ebx + 28h]
        push   ebx
        mov    ecx, dword ptr [eax]
        push   ecx
        call   sub_1_419A
        push   3
        push   esi
        call   sub_1_3930
        mov    ecx, dword ptr [ebp - 0ch]
        dec    dword ptr [ecx + 1ch]
        call   sub_1_14F0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_2980:
        push   ebp
        mov    ebp, esp
        push   esi
        pushf
        cli
        call   sub_1_14C0
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 1b1h], 0
        je     SHORT loc_1_29E7
        cmp    dword ptr [esi + 57h], -1
        je     SHORT loc_1_29F4
        mov    byte ptr [esi + 1b1h], 0
        push   3
        mov    eax, dword ptr [esi + 4]
        push   1
        push   0aa000003h
        push   eax
        call   sub_1_4328
        add    esp, 10h
        push   esi
        call   sub_1_0150
        cmp    dword ptr [esi + 147h], 4
        jbe    SHORT loc_1_29D9
        push   esi
        call   sub_1_01D0
        mov    dword ptr [esi + 147h], 0
loc_1_29D9:
        push   esi
        call   sub_1_2A20
        inc    dword ptr [esi + 147h]
        jmp    SHORT loc_1_29F4
loc_1_29E7:
        cmp    dword ptr [esi + 57h], -1
        je     SHORT loc_1_29F4
        mov    byte ptr [esi + 1b1h], 1
loc_1_29F4:
        push   7d0h
        add    esi, 1a5h
        push   esi
        call   sub_1_4232
        call   sub_1_14F0
        popf
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2A20:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   8
        push   7
        mov    edi, 14h
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0670
loc_1_2A3D:
        push   64h
        call   sub_1_4250
        dec    edi
        jne    SHORT loc_1_2A3D
        push   2
        push   0dh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   22h
        push   0
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0
        push   0dh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        cmp    dword ptr [esi + 57h], -1
        je     SHORT loc_1_2A75
        mov    byte ptr [esi + 5bh], 1
        push   esi
        call   sub_1_0680
loc_1_2A75:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_2A80:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 28h]
        test   edx, edx
        je     SHORT loc_1_2AA0
loc_1_2A90:
        mov    esi, dword ptr [edx + 18h]
        cmp    ecx, esi
        jae    SHORT loc_1_2A99
        mov    ecx, esi
loc_1_2A99:
        mov    edx, dword ptr [edx + 0ch]
        test   edx, edx
        jne    SHORT loc_1_2A90
loc_1_2AA0:
        test   ecx, ecx
        jne    SHORT loc_1_2AA9
        mov    ecx, 0eeh
loc_1_2AA9:
        pop    esi
        mov    dword ptr [eax + 15dh], ecx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2AC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 20h], 0
        mov    esi, dword ptr [ebx + 8]
        je     SHORT loc_1_2AE3
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2AE3:
        call   sub_1_14C0
        inc    dword ptr [esi + 30h]
        inc    dword ptr [ebx + 1ch]
        mov    ecx, dword ptr [esi + 28h]
        cmp    ebx, ecx
        jne    SHORT loc_1_2AFD
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [esi + 28h], eax
        jmp    SHORT loc_1_2B10
loc_1_2AFD:
        cmp    dword ptr [ecx + 0ch], ebx
        je     SHORT loc_1_2B0A
loc_1_2B02:
        mov    ecx, dword ptr [ecx + 0ch]
        cmp    dword ptr [ecx + 0ch], ebx
        jne    SHORT loc_1_2B02
loc_1_2B0A:
        mov    edx, dword ptr [ebx + 0ch]
        mov    dword ptr [ecx + 0ch], edx
loc_1_2B10:
        push   0
        mov    eax, dword ptr [ebx + 14h]
        push   eax
        mov    ecx, dword ptr [esi + 14bh]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_2B22:
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_2B47
        mov    eax, dword ptr [ebx + 1ch]
        cmp    eax, 2
        je     SHORT loc_1_2B3F
        mov    byte ptr [ebx + 20h], 1
        sub    dword ptr [ebx + 1ch], 2
        mov    edi, 103h
        jmp    SHORT loc_1_2B6D
loc_1_2B3F:
        sub    eax, 2
        mov    dword ptr [ebx + 1ch], eax
        jmp    SHORT loc_1_2B6D
loc_1_2B47:
        cmp    edi, 103h
        jne    SHORT loc_1_2B59
        mov    byte ptr [ebx + 20h], 1
        sub    dword ptr [ebx + 1ch], 2
        jmp    SHORT loc_1_2B6D
loc_1_2B59:
        cmp    edi, 0c001000eh
        jne    SHORT loc_1_2B6A
        mov    byte ptr [ebx + 20h], 1
        mov    edi, 103h
loc_1_2B6A:
        dec    dword ptr [ebx + 1ch]
loc_1_2B6D:
        cmp    dword ptr [ebx + 1ch], 0
        jne    SHORT loc_1_2BA2
        mov    eax, dword ptr [ebx + 18h]
        cmp    dword ptr [esi + 15dh], eax
        jne    SHORT loc_1_2B84
        push   esi
        call   sub_1_2A80
loc_1_2B84:
        push   0
        push   75h
        push   ebx
        call   sub_1_425C
        cmp    dword ptr [esi + 28h], 0
        jne    SHORT loc_1_2BB0
        cmp    dword ptr [esi + 2ch], 0
        jne    SHORT loc_1_2BB0
        push   esi
        call   sub_1_0150
        jmp    SHORT loc_1_2BB0
loc_1_2BA2:
        mov    eax, dword ptr [esi + 2ch]
        mov    edi, 103h
        mov    dword ptr [ebx + 0ch], eax
        mov    dword ptr [esi + 2ch], ebx
loc_1_2BB0:
        mov    eax, dword ptr [esi + 30h]
        dec    eax
        mov    dword ptr [esi + 30h], eax
        jne    SHORT loc_1_2BE1
        cmp    byte ptr [esi + 13eh], 0
        jne    SHORT loc_1_2BD1
        cmp    dword ptr [esi + 16dh], 0
        jne    SHORT loc_1_2BD1
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_1_2BE1
loc_1_2BD1:
        push   1
        add    esi, 179h
        push   esi
        call   sub_1_4232
        jmp    SHORT loc_1_2C00
loc_1_2BE1:
        cmp    dword ptr [esi + 47h], 0
        je     SHORT loc_1_2C00
        mov    eax, dword ptr [esi + 4fh]
        cmp    dword ptr [esi + eax*4 + 5dh], 0
        jne    SHORT loc_1_2C00
        cmp    byte ptr [esi + 11dh], 0
        jne    SHORT loc_1_2C00
        push   esi
        call   sub_1_26C0
loc_1_2C00:
        call   sub_1_14F0
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2C10:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebx + 8]
        call   sub_1_14C0
        inc    dword ptr [ebx + 1ch]
        inc    dword ptr [edi + 30h]
        cmp    byte ptr [ebx + 20h], 0
        je     SHORT loc_1_2C34
        mov    esi, 0c0010002h
        jmp    SHORT loc_1_2C70
loc_1_2C34:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ecx + 10h]
        test   eax, eax
        jne    SHORT loc_1_2C4A
        push   ecx
        push   ebx
        push   edi
        call   sub_1_3060
        mov    esi, eax
        jmp    SHORT loc_1_2C70
loc_1_2C4A:
        cmp    eax, 1
        jne    SHORT loc_1_2C6B
        cmp    byte ptr [edi + 13eh], 0
        je     SHORT loc_1_2C5F
        mov    esi, 0c001000dh
        jmp    SHORT loc_1_2C70
loc_1_2C5F:
        push   ecx
        push   ebx
        push   edi
        call   sub_1_30B0
        mov    esi, eax
        jmp    SHORT loc_1_2C70
loc_1_2C6B:
        mov    esi, 10001h
loc_1_2C70:
        dec    dword ptr [ebx + 1ch]
        mov    eax, dword ptr [edi + 30h]
        dec    eax
        mov    dword ptr [edi + 30h], eax
        jne    SHORT loc_1_2CA4
        cmp    byte ptr [edi + 13eh], 0
        jne    SHORT loc_1_2C94
        cmp    dword ptr [edi + 16dh], 0
        jne    SHORT loc_1_2C94
        cmp    dword ptr [edi + 2ch], 0
        je     SHORT loc_1_2CA4
loc_1_2C94:
        push   1
        add    edi, 179h
        push   edi
        call   sub_1_4232
        jmp    SHORT loc_1_2CC3
loc_1_2CA4:
        cmp    dword ptr [edi + 47h], 0
        je     SHORT loc_1_2CC3
        mov    eax, dword ptr [edi + 4fh]
        cmp    dword ptr [edi + eax*4 + 5dh], 0
        jne    SHORT loc_1_2CC3
        cmp    byte ptr [edi + 11dh], 0
        jne    SHORT loc_1_2CC3
        push   edi
        call   sub_1_26C0
loc_1_2CC3:
        call   sub_1_14F0
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2CE0:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        mov    dword ptr [ebp - 18h], eax
        push   esi
        mov    dword ptr [ebp - 0ch], eax
        push   edi
        lea    esi, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp + 10h]
        cmp    eax, 1010101h
        ja     SHORT loc_1_2D1D
        je     NEAR PTR loc_1_2EAF
        sub    eax, 10101h
        cmp    eax, 12h
        ja     SHORT loc_1_2D3E
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_2D1D:
        cmp    eax, 1010102h
        je     NEAR PTR loc_1_2ED1
        cmp    eax, 1010103h
        je     NEAR PTR loc_1_2EF3
        cmp    eax, 1010104h
        je     NEAR PTR loc_1_2FB8
loc_1_2D3E:
        mov    dword ptr [ebp - 0ch], 0c00000bbh
        jmp    NEAR PTR loc_1_2FC1
loc_1_2D4A:
        cmp    byte ptr [ebp + 14h], 0
        jne    SHORT loc_1_2D5F
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edi, 5ch
        jmp    NEAR PTR loc_1_2FC1
loc_1_2D5F:
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edi, 7ch
        jmp    NEAR PTR loc_1_2FC1
loc_1_2D6E:
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 14h], 2
        cmp    byte ptr [ebx + 13eh], 0
        jne    SHORT loc_1_2D88
        mov    dword ptr [ebp - 14h], 0
loc_1_2D88:
        lea    esi, [ebp - 14h]
        mov    edi, 4
        jmp    NEAR PTR loc_1_2FC1
loc_1_2D95:
        lea    esi, [ebp - 18h]
        mov    edi, 4
        jmp    NEAR PTR loc_1_2FC1
loc_1_2DA2:
        mov    dword ptr [ebp - 8], 0eeh
        jmp    NEAR PTR loc_1_2FC1
loc_1_2DAE:
        mov    dword ptr [ebp - 8], 5dch
        jmp    NEAR PTR loc_1_2FC1
loc_1_2DBA:
        mov    dword ptr [ebp - 8], 186a0h
        jmp    NEAR PTR loc_1_2FC1
loc_1_2DC6:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 38h]
        shl    eax, 8
        jmp    NEAR PTR loc_1_2FBE
loc_1_2DD4:
        mov    eax, 20h
        mov    ebx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ebx + 38h]
        shl    eax, 8
        jmp    NEAR PTR loc_1_2FBE
loc_1_2DE7:
        mov    dword ptr [ebp - 8], 100h
        jmp    NEAR PTR loc_1_2FC1
loc_1_2DF3:
        mov    dword ptr [ebp - 8], 100h
        jmp    NEAR PTR loc_1_2FC1
loc_1_2DFF:
        push   3
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 10ah
        lea    eax, [ebp - 8]
        push   ebx
        lea    esi, [ebp - 8]
        push   eax
        mov    edi, 4
        call   sub_1_433C
        and    dword ptr [ebp - 8], 0ffffff00h
        jmp    NEAR PTR loc_1_2FC1
loc_1_2E28:
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edi, 21h
        jmp    NEAR PTR loc_1_2FC1
loc_1_2E37:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_1_2E51
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 14bh]
        mov    ecx, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 8], ecx
        jmp    NEAR PTR loc_1_2FC1
loc_1_2E51:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 14h]
        mov    edx, dword ptr [ecx + 8]
        mov    dword ptr [ebp - 8], edx
        jmp    NEAR PTR loc_1_2FC1
loc_1_2E62:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_1_2E76
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 15dh]
        jmp    NEAR PTR loc_1_2FBE
loc_1_2E76:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 8], ecx
        jmp    NEAR PTR loc_1_2FC1
loc_1_2E84:
        lea    esi, [ebp - 2]
        mov    edi, 2
        mov    word ptr [ebp - 2], 300h
        jmp    NEAR PTR loc_1_2FC1
loc_1_2E97:
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_1_2FC1
loc_1_2EA3:
        mov    dword ptr [ebp - 8], 0fh
        jmp    NEAR PTR loc_1_2FC1
loc_1_2EAF:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 10ah
        lea    eax, [ebp - 20h]
        push   ebx
        lea    esi, [ebp - 20h]
        push   eax
        mov    edi, 6
        call   sub_1_433C
        jmp    NEAR PTR loc_1_2FC1
loc_1_2ED1:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 104h
        lea    eax, [ebp - 20h]
        push   ebx
        lea    esi, [ebp - 20h]
        push   eax
        mov    edi, 6
        call   sub_1_433C
        jmp    NEAR PTR loc_1_2FC1
loc_1_2EF3:
        cmp    byte ptr [ebp + 14h], 0
        mov    ebx, dword ptr [ebp + 8]
        je     SHORT loc_1_2F53
        mov    ecx, dword ptr [ebx + 14bh]
        mov    eax, dword ptr [ecx + 24h]
        mov    dword ptr [ebp - 10h], eax
        lea    ebx, [eax + eax*2]
        lea    edx, [ebx*2]
        cmp    edx, dword ptr [ebp + 1ch]
        jbe    SHORT loc_1_2F28
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 0ch], 0c0010014h
        mov    dword ptr [eax], edx
        jmp    NEAR PTR loc_1_2FC1
loc_1_2F28:
        mov    eax, dword ptr [ebp + 18h]
        lea    edx, [ebp - 10h]
        push   eax
        mov    ebx, dword ptr [ebp + 1ch]
        push   edx
        lea    eax, [ebp - 0ch]
        push   ebx
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_2F3E:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ecx], eax
        je     SHORT loc_1_2FC7
        jmp    SHORT loc_1_2FC1
loc_1_2F53:
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebx + 14bh]
        mov    ecx, dword ptr [eax + 14h]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_2F66:
        lea    edx, [eax + eax*2]
        mov    dword ptr [ebp - 10h], eax
        lea    ecx, [edx*2]
        cmp    ecx, dword ptr [ebp + 1ch]
        jbe    SHORT loc_1_2F86
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 0ch], 0c0010014h
        mov    dword ptr [eax], ecx
        jmp    SHORT loc_1_2FC1
loc_1_2F86:
        mov    eax, dword ptr [ebp + 18h]
        lea    ecx, [ebp - 10h]
        push   eax
        mov    edx, dword ptr [ebp + 1ch]
        push   ecx
        mov    eax, dword ptr [ebp + 0ch]
        push   edx
        mov    ecx, dword ptr [eax + 14h]
        push   ecx
        mov    edx, dword ptr [ebx + 14bh]
        push   edx
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_2FA9:
        mov    ecx, dword ptr [ebp - 10h]
        lea    eax, [ecx + ecx*2]
        mov    ecx, dword ptr [ebp + 24h]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_1_2FC1
loc_1_2FB8:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 41h]
loc_1_2FBE:
        mov    dword ptr [ebp - 8], eax
loc_1_2FC1:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_2FFD
loc_1_2FC7:
        cmp    dword ptr [ebp + 10h], 1010103h
        je     SHORT loc_1_2FFD
        cmp    edi, dword ptr [ebp + 1ch]
        jbe    SHORT loc_1_2FED
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 0ch], 0c0010014h
        mov    dword ptr [eax], edi
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
loc_1_2FED:
        push   edi
        mov    eax, dword ptr [ebp + 18h]
        push   esi
        push   eax
        call   sub_1_433C
        mov    ecx, dword ptr [ebp + 24h]
        add    dword ptr [ecx], edi
loc_1_2FFD:
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  08Dh, 049h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 08Dh
        db  064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_3060:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 8
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    edx, [ebp - 4]
        push   edx
        mov    esi, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [esi + 1ch]
        mov    edx, dword ptr [esi + 18h]
        push   eax
        push   edx
        push   ecx
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [esi + 14h]
        mov    edx, dword ptr [ebp + 8]
        push   ecx
        push   eax
        push   edx
        call   sub_1_2CE0
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 20h], ecx
        mov    dword ptr [esi + 24h], edx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_30B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        xor    ebx, ebx
        mov    edi, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [esi + 18h]
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 1010103h
        ja     SHORT loc_1_30ED
        je     NEAR PTR loc_1_31FC
        cmp    eax, 1010eh
        je     SHORT loc_1_30FF
        cmp    eax, 1010fh
        je     SHORT loc_1_315A
        cmp    eax, 10112h
        je     NEAR PTR loc_1_31D5
loc_1_30ED:
        mov    ebx, 0c0010017h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_1_3237
loc_1_30FF:
        cmp    edi, 4
        je     SHORT loc_1_3116
        mov    ebx, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_1_3237
loc_1_3116:
        push   4
        lea    eax, [ebp - 8]
        push   ecx
        push   eax
        call   sub_1_433C
        test   dword ptr [ebp - 8], 0f050h
        je     SHORT loc_1_3143
        mov    dword ptr [esi + 20h], 4
        mov    dword ptr [esi + 24h], 0
        mov    ebx, 0c00000bbh
        jmp    NEAR PTR loc_1_3237
loc_1_3143:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0ch]
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   esi
        push   ecx
        push   edx
        call   sub_1_3250
        jmp    NEAR PTR loc_1_3235
loc_1_315A:
        cmp    edi, 4
        je     SHORT loc_1_3171
        mov    ebx, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_1_3237
loc_1_3171:
        push   4
        lea    eax, [ebp - 4]
        push   ecx
        push   eax
        call   sub_1_433C
        cmp    dword ptr [ebp - 4], 0eeh
        ja     SHORT loc_1_31CE
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 15dh]
        cmp    dword ptr [ebp - 4], eax
        jbe    SHORT loc_1_31A8
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 15dh], eax
        mov    dword ptr [ecx + 18h], eax
        jmp    NEAR PTR loc_1_3237
loc_1_31A8:
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [ecx + 18h], eax
        jne    SHORT loc_1_31C6
        mov    eax, dword ptr [ecx + 18h]
        cmp    dword ptr [ebp - 4], eax
        jae    SHORT loc_1_31C6
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    dword ptr [ecx + 18h], eax
        call   sub_1_2A80
        jmp    SHORT loc_1_3237
loc_1_31C6:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 18h], eax
        jmp    SHORT loc_1_3237
loc_1_31CE:
        mov    ebx, 0c0010014h
        jmp    SHORT loc_1_3237
loc_1_31D5:
        cmp    edi, 4
        je     SHORT loc_1_31E9
        mov    ebx, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    SHORT loc_1_3237
loc_1_31E9:
        push   4
        xor    ebx, ebx
        push   ecx
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 71h
        push   ecx
        call   sub_1_433C
        jmp    SHORT loc_1_3237
loc_1_31FC:
        mov    ebx, 6
        mov    eax, edi
        sub    edx, edx
        div    ebx
        test   edx, edx
        je     SHORT loc_1_321A
        mov    ebx, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    SHORT loc_1_3237
loc_1_321A:
        push   ecx
        mov    eax, edi
        mov    ecx, 6
        sub    edx, edx
        div    ecx
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   esi
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   edx
        call   sub_1_32B0
loc_1_3235:
        mov    ebx, eax
loc_1_3237:
        test   ebx, ebx
        jne    SHORT loc_1_3245
        mov    dword ptr [esi + 24h], 0
        mov    dword ptr [esi + 20h], edi
loc_1_3245:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
sub_1_3250:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        cmp    byte ptr [ecx + 13eh], 0
        jne    SHORT loc_1_3299
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 20h], 0
        jne    SHORT loc_1_328F
        push   1
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        mov    eax, dword ptr [esi + 14h]
        push   eax
        inc    dword ptr [esi + 1ch]
        mov    ecx, dword ptr [ecx + 14bh]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_3287:
        dec    dword ptr [esi + 1ch]
        pop    esi
        pop    ebp
        ret    10h
loc_1_328F:
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    10h
loc_1_3299:
        mov    eax, 0c001000dh
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_32B0:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        cmp    byte ptr [ecx + 13eh], 0
        jne    SHORT loc_1_32FD
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 20h], 0
        jne    SHORT loc_1_32F3
        push   1
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    edx, dword ptr [ebp + 14h]
        push   edx
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    edx, dword ptr [esi + 14h]
        push   edx
        inc    dword ptr [esi + 1ch]
        mov    ecx, dword ptr [ecx + 14bh]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_32EB:
        dec    dword ptr [esi + 1ch]
        pop    esi
        pop    ebp
        ret    14h
loc_1_32F3:
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    14h
loc_1_32FD:
        mov    eax, 0c001000dh
        pop    esi
        pop    ebp
        ret    14h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3310:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 0ch], 0
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        lea    edx, [ebp - 8]
        mov    eax, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [esi + 18h]
        push   edx
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 10h], ecx
        lea    eax, [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 14h]
        push   eax
        mov    edx, dword ptr [ebp - 10h]
        push   ecx
        mov    eax, dword ptr [esi + 14h]
        push   edx
        mov    ebx, dword ptr [ebp + 8]
        push   1
        push   eax
        push   0
        push   ebx
        call   sub_1_2CE0
        cmp    eax, 0c00000bbh
        mov    edi, eax
        jne    NEAR PTR loc_1_33F6
        xor    edi, edi
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 1020101h
        ja     SHORT loc_1_3383
        je     SHORT loc_1_33C0
        sub    eax, 20101h
        cmp    eax, 4
        ja     SHORT loc_1_3391
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_3383:
        cmp    eax, 1020102h
        je     SHORT loc_1_33C8
        cmp    eax, 1020103h
        je     SHORT loc_1_33D0
loc_1_3391:
        mov    edi, 0c0010017h
        jmp    SHORT loc_1_33D9
loc_1_3398:
        mov    eax, dword ptr [ebx + 11eh]
        jmp    SHORT loc_1_33D6
loc_1_33A0:
        mov    eax, dword ptr [ebx + 122h]
        jmp    SHORT loc_1_33D6
loc_1_33A8:
        mov    eax, dword ptr [ebx + 126h]
        jmp    SHORT loc_1_33D6
loc_1_33B0:
        mov    eax, dword ptr [ebx + 136h]
        jmp    SHORT loc_1_33D6
loc_1_33B8:
        mov    eax, dword ptr [ebx + 13ah]
        jmp    SHORT loc_1_33D6
loc_1_33C0:
        mov    eax, dword ptr [ebx + 132h]
        jmp    SHORT loc_1_33D6
loc_1_33C8:
        mov    eax, dword ptr [ebx + 12ah]
        jmp    SHORT loc_1_33D6
loc_1_33D0:
        mov    eax, dword ptr [ebx + 12eh]
loc_1_33D6:
        mov    dword ptr [ebp - 4], eax
loc_1_33D9:
        cmp    dword ptr [ebp - 14h], 4
        jb     SHORT loc_1_33F6
        test   edi, edi
        jne    SHORT loc_1_33F6
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        call   sub_1_433C
        add    dword ptr [ebp - 8], 4
loc_1_33F6:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], ecx
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h
        db  000h, 000h, 000h
loc_1_3430:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        xor    esi, esi
        call   sub_1_14C0
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ecx + 14h]
        cmp    eax, 10113h
        ja     SHORT loc_1_345B
        je     SHORT loc_1_348C
        cmp    eax, 10101h
        jb     SHORT loc_1_3487
        cmp    eax, 10111h
        jbe    SHORT loc_1_348C
        jmp    SHORT loc_1_3487
loc_1_345B:
        cmp    eax, 1010104h
        ja     SHORT loc_1_3479
        cmp    eax, 1010101h
        jae    SHORT loc_1_348C
        cmp    eax, 20101h
        jb     SHORT loc_1_3487
        cmp    eax, 20105h
        jbe    SHORT loc_1_348C
        jmp    SHORT loc_1_3487
loc_1_3479:
        cmp    eax, 1020101h
        jb     SHORT loc_1_3487
        cmp    eax, 1020103h
        jbe    SHORT loc_1_348C
loc_1_3487:
        mov    esi, 0c0010017h
loc_1_348C:
        mov    edi, dword ptr [ebp + 8]
        inc    dword ptr [edi + 30h]
        test   esi, esi
        jne    SHORT loc_1_349F
        push   ecx
        push   edi
        call   sub_1_3310
        mov    esi, eax
loc_1_349F:
        mov    eax, dword ptr [edi + 30h]
        dec    eax
        mov    dword ptr [edi + 30h], eax
        jne    SHORT loc_1_34D0
        cmp    byte ptr [edi + 13eh], 0
        jne    SHORT loc_1_34C0
        cmp    dword ptr [edi + 16dh], 0
        jne    SHORT loc_1_34C0
        cmp    dword ptr [edi + 2ch], 0
        je     SHORT loc_1_34D0
loc_1_34C0:
        push   1
        add    edi, 179h
        push   edi
        call   sub_1_4232
        jmp    SHORT loc_1_34EF
loc_1_34D0:
        cmp    dword ptr [edi + 47h], 0
        je     SHORT loc_1_34EF
        mov    eax, dword ptr [edi + 4fh]
        cmp    dword ptr [edi + eax*4 + 5dh], 0
        jne    SHORT loc_1_34EF
        cmp    byte ptr [edi + 11dh], 0
        jne    SHORT loc_1_34EF
        push   edi
        call   sub_1_26C0
loc_1_34EF:
        call   sub_1_14F0
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
loc_1_3500:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 4
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_417C
        push   0
        db  08Bh, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        call   sub_1_4170
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3530:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [esi + 1b2h], 1
        call   sub_1_14C0
        db  039h, 035h, 03Eh, 000h, 000h, 000h
        jne    SHORT loc_1_3559
        mov    eax, dword ptr [esi + 24h]
        db  0A3h, 03Eh, 000h, 000h, 000h
        jmp    SHORT loc_1_3575
loc_1_3559:
        db  08Bh, 00Dh, 03Eh, 000h, 000h, 000h
        cmp    dword ptr [ecx + 24h], esi
        je     SHORT loc_1_356C
loc_1_3564:
        mov    ecx, dword ptr [ecx + 24h]
        cmp    dword ptr [ecx + 24h], esi
        jne    SHORT loc_1_3564
loc_1_356C:
        mov    eax, dword ptr [ecx + 24h]
        mov    edx, dword ptr [eax + 24h]
        mov    dword ptr [ecx + 24h], edx
loc_1_3575:
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 1a5h]
        push   eax
        mov    edi, 61a8h
        push   ecx
        call   sub_1_4238
loc_1_358A:
        push   64h
        call   sub_1_4250
        dec    edi
        jne    SHORT loc_1_358A
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 000h
        push   21h
        push   0
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_35B0:
        mov    eax, dword ptr [esi + 14bh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_35BC:
        lea    eax, [esi + 8]
        push   eax
        call   sub_1_4244
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_4188
        push   0
        push   2ebh
        push   esi
        call   sub_1_425C
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_35F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        cmp    byte ptr [edi + 20h], 0
        mov    esi, dword ptr [edi + 8]
        je     SHORT loc_1_3613
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3613:
        call   sub_1_14C0
        inc    dword ptr [edi + 1ch]
        inc    dword ptr [esi + 30h]
        cmp    byte ptr [esi + 13eh], 0
        je     SHORT loc_1_3640
        dec    dword ptr [edi + 1ch]
        dec    dword ptr [esi + 30h]
        call   sub_1_14F0
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3640:
        mov    ebx, dword ptr [esi + 28h]
        mov    dword ptr [ebp - 4], 0
        test   ebx, ebx
        je     SHORT loc_1_3670
loc_1_364E:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        inc    dword ptr [ebx + 1ch]
        push   40010004h
        mov    ecx, dword ptr [ebx]
        push   ecx
        call   sub_1_41BE
        mov    eax, dword ptr [ebx + 0ch]
        dec    dword ptr [ebx + 1ch]
        mov    ebx, eax
        test   eax, eax
        jne    SHORT loc_1_364E
loc_1_3670:
        mov    byte ptr [esi + 13eh], 1
        mov    dword ptr [esi + 13fh], 0
        push   esi
        mov    dword ptr [esi + 143h], edi
        call   sub_1_3700
        mov    dword ptr [ebp - 4], eax
        dec    dword ptr [edi + 1ch]
        mov    eax, dword ptr [esi + 30h]
        dec    eax
        mov    dword ptr [esi + 30h], eax
        jne    SHORT loc_1_36C4
        cmp    byte ptr [esi + 13eh], 0
        jne    SHORT loc_1_36B4
        cmp    dword ptr [esi + 16dh], 0
        jne    SHORT loc_1_36B4
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_1_36C4
loc_1_36B4:
        push   1
        add    esi, 179h
        push   esi
        call   sub_1_4232
        jmp    SHORT loc_1_36E3
loc_1_36C4:
        cmp    dword ptr [esi + 47h], 0
        je     SHORT loc_1_36E3
        mov    eax, dword ptr [esi + 4fh]
        cmp    dword ptr [esi + eax*4 + 5dh], 0
        jne    SHORT loc_1_36E3
        cmp    byte ptr [esi + 11dh], 0
        jne    SHORT loc_1_36E3
        push   esi
        call   sub_1_26C0
loc_1_36E3:
        call   sub_1_14F0
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3700:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 30h]
        cmp    dword ptr [esi + 16dh], 0
        je     SHORT loc_1_374C
loc_1_3714:
        mov    ecx, dword ptr [esi + 16dh]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 16dh], eax
        mov    eax, dword ptr [ecx + 8]
        mov    edi, dword ptr [ecx + 4]
        cmp    eax, 4
        je     SHORT loc_1_3740
        cmp    eax, 8
        je     SHORT loc_1_3740
        push   0c001000ch
        mov    eax, dword ptr [edi]
        push   ecx
        push   eax
        call   sub_1_41AC
loc_1_3740:
        dec    dword ptr [edi + 1ch]
        cmp    dword ptr [esi + 16dh], 0
        jne    SHORT loc_1_3714
loc_1_374C:
        cmp    dword ptr [esi + 57h], -1
        je     SHORT loc_1_376A
        mov    eax, 103h
        pop    edi
        mov    dword ptr [esi + 13fh], 2
        dec    dword ptr [esi + 30h]
        pop    esi
        pop    ebp
        ret    4
loc_1_376A:
        push   esi
        dec    dword ptr [esi + 30h]
        call   sub_1_3780
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3780:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        xor    ecx, ecx
        push   esi
        mov    dword ptr [eax + 4fh], ecx
        push   edi
        mov    esi, dword ptr [eax + 30h]
        inc    esi
        mov    edx, dword ptr [eax + 38h]
        mov    dword ptr [eax + 30h], esi
        mov    dword ptr [eax + 53h], ecx
        cmp    edx, ecx
        je     SHORT loc_1_37AF
        lea    edi, [eax + 5dh]
loc_1_37A2:
        cmp    dword ptr [edi], 0
        jne    SHORT loc_1_37BF
        add    edi, 4
        inc    ecx
        cmp    ecx, edx
        jb     SHORT loc_1_37A2
loc_1_37AF:
        dec    esi
        push   eax
        mov    dword ptr [eax + 30h], esi
        call   sub_1_37E0
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_37BF:
        dec    esi
        pop    edi
        mov    dword ptr [eax + 13fh], 3
        mov    dword ptr [eax + 30h], esi
        mov    eax, 103h
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_37E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        inc    dword ptr [esi + 30h]
        cmp    dword ptr [esi + 38h], 0
        jbe    SHORT loc_1_3830
        lea    edi, [esi + 8dh]
loc_1_3802:
        mov    ecx, dword ptr [edi]
        test   ecx, ecx
        je     SHORT loc_1_3822
        mov    ebx, dword ptr [ecx + 28h]
        push   0c001000ch
        push   ecx
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_419A
        dec    dword ptr [ebx + 1ch]
        mov    dword ptr [edi], 0
loc_1_3822:
        add    edi, 4
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [esi + 38h], eax
        ja     SHORT loc_1_3802
loc_1_3830:
        mov    edx, dword ptr [esi + 47h]
        test   edx, edx
        je     SHORT loc_1_385A
loc_1_3837:
        lea    eax, [edx + 24h]
        push   0c001000ch
        push   edx
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi + 47h], ecx
        mov    edi, dword ptr [eax + 4]
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_419A
        dec    dword ptr [edi + 1ch]
        mov    edx, dword ptr [esi + 47h]
        test   edx, edx
        jne    SHORT loc_1_3837
loc_1_385A:
        mov    edx, dword ptr [esi + 161h]
        test   edx, edx
        je     SHORT loc_1_388D
loc_1_3864:
        lea    eax, [edx + 24h]
        push   0c001000ch
        push   edx
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi + 161h], ecx
        mov    edi, dword ptr [eax + 4]
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_419A
        dec    dword ptr [edi + 1ch]
        mov    edx, dword ptr [esi + 161h]
        test   edx, edx
        jne    SHORT loc_1_3864
loc_1_388D:
        cmp    byte ptr [esi + 11dh], 0
        je     SHORT loc_1_38AE
loc_1_3896:
        mov    edi, 64h
loc_1_389B:
        push   64h
        call   sub_1_4250
        dec    edi
        jne    SHORT loc_1_389B
        cmp    byte ptr [esi + 11dh], 0
        jne    SHORT loc_1_3896
loc_1_38AE:
        mov    byte ptr [esi + 15ch], 1bh
        push   esi
        call   sub_1_01D0
        mov    edi, dword ptr [esi + 28h]
        cmp    al, 1
        sbb    eax, eax
        xor    ebx, ebx
        and    eax, 0c0000001h
        mov    byte ptr [esi + 13eh], bl
        mov    dword ptr [ebp - 4], eax
        test   edi, edi
        je     SHORT loc_1_3914
loc_1_38D6:
        inc    dword ptr [edi + 1ch]
        cmp    dword ptr [ebp - 4], ebx
        je     SHORT loc_1_38ED
        push   ebx
        mov    eax, dword ptr [edi]
        push   ebx
        push   40010007h
        push   eax
        call   sub_1_41BE
loc_1_38ED:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [edi]
        push   40010005h
        push   ecx
        call   sub_1_41BE
        mov    ecx, dword ptr [edi]
        push   ecx
        call   sub_1_41C4
        mov    eax, dword ptr [edi + 0ch]
        dec    dword ptr [edi + 1ch]
        mov    edi, eax
        test   eax, eax
        jne    SHORT loc_1_38D6
loc_1_3914:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        dec    dword ptr [esi + 30h]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3930:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 13fh], eax
        jne    NEAR PTR loc_1_3A1D
        cmp    eax, 1
        je     SHORT loc_1_39B3
        cmp    eax, 2
        je     SHORT loc_1_39BB
        cmp    eax, 3
        je     SHORT loc_1_39C3
        mov    eax, dword ptr [ebp - 4]
loc_1_395D:
        cmp    eax, 103h
        je     SHORT loc_1_397C
        push   eax
        mov    eax, dword ptr [esi + 143h]
        mov    ecx, dword ptr [eax]
        push   ecx
        call   sub_1_41A6
        mov    ecx, dword ptr [esi + 143h]
        dec    dword ptr [ecx + 1ch]
loc_1_397C:
        mov    eax, dword ptr [esi + 30h]
        dec    eax
        mov    dword ptr [esi + 30h], eax
        jne    SHORT loc_1_39FE
        cmp    byte ptr [esi + 13eh], 0
        jne    SHORT loc_1_399D
        cmp    dword ptr [esi + 16dh], 0
        jne    SHORT loc_1_399D
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_1_39FE
loc_1_399D:
        push   1
        add    esi, 179h
        push   esi
        call   sub_1_4232
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_39B3:
        push   esi
        call   sub_1_3700
        jmp    SHORT loc_1_395D
loc_1_39BB:
        push   esi
        call   sub_1_3780
        jmp    SHORT loc_1_395D
loc_1_39C3:
        mov    ecx, dword ptr [esi + 30h]
        xor    edx, edx
        inc    ecx
        mov    eax, dword ptr [esi + 38h]
        mov    dword ptr [esi + 30h], ecx
        cmp    eax, edx
        je     SHORT loc_1_39E3
        lea    edi, [esi + 5dh]
loc_1_39D6:
        cmp    dword ptr [edi], 0
        jne    SHORT loc_1_39F2
        add    edi, 4
        inc    edx
        cmp    edx, eax
        jb     SHORT loc_1_39D6
loc_1_39E3:
        dec    ecx
        push   esi
        mov    dword ptr [esi + 30h], ecx
        call   sub_1_37E0
        jmp    NEAR PTR loc_1_395D
loc_1_39F2:
        dec    ecx
        pop    edi
        mov    dword ptr [esi + 30h], ecx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_39FE:
        cmp    dword ptr [esi + 47h], 0
        je     SHORT loc_1_3A1D
        mov    eax, dword ptr [esi + 4fh]
        cmp    dword ptr [esi + eax*4 + 5dh], 0
        jne    SHORT loc_1_3A1D
        cmp    byte ptr [esi + 11dh], 0
        jne    SHORT loc_1_3A1D
        push   esi
        call   sub_1_26C0
loc_1_3A1D:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3A30:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 18h]
        mov    edi, dword ptr [ebp + 1ch]
        mov    esi, dword ptr [ebx + 8]
        call   sub_1_14C0
        test   edi, edi
        jne    SHORT loc_1_3A4B
        lea    edi, [ebx + 49h]
loc_1_3A4B:
        cmp    byte ptr [esi + 13eh], 0
        je     SHORT loc_1_3A5B
        mov    esi, 0c001000dh
        jmp    SHORT loc_1_3AA0
loc_1_3A5B:
        mov    dword ptr [edi + 4], ebx
        inc    dword ptr [ebx + 1ch]
        cmp    byte ptr [ebp + 20h], 1
        sbb    eax, eax
        and    eax, 0fffffffah
        add    eax, 0ah
        mov    dword ptr [edi + 8], eax
        mov    dword ptr [edi], 0
        cmp    dword ptr [esi + 16dh], 0
        jne    SHORT loc_1_3A8D
        mov    dword ptr [esi + 171h], edi
        mov    dword ptr [esi + 16dh], edi
        jmp    SHORT loc_1_3A9B
loc_1_3A8D:
        mov    eax, dword ptr [esi + 171h]
        mov    dword ptr [eax], edi
        mov    dword ptr [esi + 171h], edi
loc_1_3A9B:
        mov    esi, 103h
loc_1_3AA0:
        call   sub_1_14F0
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    1ch
        db  08Bh, 0FFh
loc_1_3AB0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 10h]
        mov    edi, dword ptr [ebp + 14h]
        mov    esi, dword ptr [ebx + 8]
        call   sub_1_14C0
        test   edi, edi
        jne    SHORT loc_1_3ACB
        lea    edi, [ebx + 21h]
loc_1_3ACB:
        cmp    byte ptr [esi + 13eh], 0
        je     SHORT loc_1_3ADB
        mov    esi, 0c001000dh
        jmp    SHORT loc_1_3B1D
loc_1_3ADB:
        mov    dword ptr [edi + 4], ebx
        inc    dword ptr [ebx + 1ch]
        cmp    byte ptr [ebp + 18h], 1
        sbb    eax, eax
        add    eax, 9
        mov    dword ptr [edi + 8], eax
        mov    dword ptr [edi], 0
        cmp    dword ptr [esi + 16dh], 0
        jne    SHORT loc_1_3B0A
        mov    dword ptr [esi + 171h], edi
        mov    dword ptr [esi + 16dh], edi
        jmp    SHORT loc_1_3B18
loc_1_3B0A:
        mov    eax, dword ptr [esi + 171h]
        mov    dword ptr [eax], edi
        mov    dword ptr [esi + 171h], edi
loc_1_3B18:
        mov    esi, 103h
loc_1_3B1D:
        call   sub_1_14F0
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    14h
        db  005h, 000h, 000h, 000h, 000h
loc_1_3B30:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        dec    dword ptr [eax + 1ch]
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_3B40:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        call   sub_1_14C0
        mov    esi, dword ptr [ebp + 0ch]
        inc    dword ptr [esi + 30h]
        cmp    byte ptr [esi + 13eh], 0
        jne    NEAR PTR loc_1_3C00
        cmp    dword ptr [esi + 175h], 0
        jne    NEAR PTR loc_1_3C00
loc_1_3B6B:
        mov    edi, dword ptr [esi + 16dh]
        test   edi, edi
        je     SHORT loc_1_3BD6
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 16dh], eax
        cmp    dword ptr [esi + 171h], edi
        jne    SHORT loc_1_3B8F
        mov    dword ptr [esi + 171h], 0
loc_1_3B8F:
        mov    dword ptr [esi + 175h], edi
        mov    eax, dword ptr [edi + 8]
        cmp    eax, 4
        je     SHORT loc_1_3BAA
        cmp    eax, 0ah
        je     SHORT loc_1_3BAA
        push   esi
        call   sub_1_3CC0
        jmp    SHORT loc_1_3BB0
loc_1_3BAA:
        push   esi
        call   sub_1_3D30
loc_1_3BB0:
        mov    ebx, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi + 8]
        cmp    ecx, 8
        je     SHORT loc_1_3BCA
        cmp    ecx, 4
        je     SHORT loc_1_3BCA
        push   eax
        push   edi
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_41AC
loc_1_3BCA:
        dec    dword ptr [ebx + 1ch]
        cmp    byte ptr [esi + 13eh], 0
        je     SHORT loc_1_3B6B
loc_1_3BD6:
        mov    dword ptr [esi + 175h], 0
        cmp    byte ptr [esi + 13eh], 0
        je     SHORT loc_1_3C00
        push   1
        dec    dword ptr [esi + 30h]
        push   esi
        call   sub_1_3930
        call   sub_1_14F0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
loc_1_3C00:
        mov    edi, dword ptr [esi + 2ch]
        test   edi, edi
        je     SHORT loc_1_3C66
        xor    ebx, ebx
loc_1_3C09:
        cmp    dword ptr [edi + 1ch], 0
        jbe    SHORT loc_1_3C16
        mov    edi, dword ptr [edi + 0ch]
        mov    ebx, edi
        jmp    SHORT loc_1_3C50
loc_1_3C16:
        mov    eax, dword ptr [esi + 15dh]
        cmp    dword ptr [edi + 18h], eax
        jne    SHORT loc_1_3C27
        push   esi
        call   sub_1_2A80
loc_1_3C27:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_4194
        lea    ecx, [edi + 0ch]
        test   ebx, ebx
        mov    eax, dword ptr [ecx]
        je     SHORT loc_1_3C3F
        mov    dword ptr [ebx + 0ch], eax
        jmp    SHORT loc_1_3C42
loc_1_3C3F:
        mov    dword ptr [esi + 2ch], eax
loc_1_3C42:
        mov    eax, edi
        push   0
        mov    edi, dword ptr [ecx]
        push   75h
        push   eax
        call   sub_1_425C
loc_1_3C50:
        test   edi, edi
        jne    SHORT loc_1_3C09
        cmp    dword ptr [esi + 2ch], 0
        jne    SHORT loc_1_3C66
        cmp    dword ptr [esi + 28h], 0
        jne    SHORT loc_1_3C66
        push   esi
        call   sub_1_0150
loc_1_3C66:
        dec    dword ptr [esi + 30h]
        jne    SHORT loc_1_3C93
        cmp    byte ptr [esi + 13eh], 0
        jne    SHORT loc_1_3C83
        cmp    dword ptr [esi + 16dh], 0
        jne    SHORT loc_1_3C83
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_1_3C93
loc_1_3C83:
        push   1
        add    esi, 179h
        push   esi
        call   sub_1_4232
        jmp    SHORT loc_1_3CB2
loc_1_3C93:
        cmp    dword ptr [esi + 47h], 0
        je     SHORT loc_1_3CB2
        mov    eax, dword ptr [esi + 4fh]
        cmp    dword ptr [esi + eax*4 + 5dh], 0
        jne    SHORT loc_1_3CB2
        cmp    byte ptr [esi + 11dh], 0
        jne    SHORT loc_1_3CB2
        push   esi
        call   sub_1_26C0
loc_1_3CB2:
        call   sub_1_14F0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  08Bh, 0FFh
sub_1_3CC0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    eax, dword ptr [esi + 14bh]
        mov    edi, dword ptr [eax + 0ch]
        test   edi, 24h
        je     SHORT loc_1_3CE1
        call   sub_1_07A0
        jmp    SHORT loc_1_3CE6
loc_1_3CE1:
        call   sub_1_3D30
loc_1_3CE6:
        test   edi, 26h
        lea    eax, [esi + 15bh]
        je     SHORT loc_1_3CF9
        or     byte ptr [eax], 8
        jmp    SHORT loc_1_3CFC
loc_1_3CF9:
        and    byte ptr [eax], 0f7h
loc_1_3CFC:
        test   edi, 20h
        je     SHORT loc_1_3D09
        or     byte ptr [eax], 10h
        jmp    SHORT loc_1_3D0C
loc_1_3D09:
        and    byte ptr [eax], 0efh
loc_1_3D0C:
        test   edi, 28h
        je     SHORT loc_1_3D19
        or     byte ptr [eax], 4
        jmp    SHORT loc_1_3D1C
loc_1_3D19:
        and    byte ptr [eax], 0fbh
loc_1_3D1C:
        push   esi
        call   sub_1_0780
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_3D30:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_05E0
        push   esi
        call   sub_1_07E0
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_3D50:
        db  0A0h, 0E8h, 043h, 000h, 000h
        cmp    al, 0
        jne    SHORT loc_1_3D75
        db  0FEh, 005h, 0E8h, 043h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 028h, 000h
        jb     SHORT loc_1_3D77
        push   0
        push   0
        db  0E8h, 000h, 0D0h, 0FEh, 0FFh
        cmp    eax, 0
        jne    SHORT loc_1_3D77
loc_1_3D75:
        clc
        ret
loc_1_3D77:
        stc
        ret
SETP3_Control:
        sub    eax, 1
        cmp    eax, 1bh
        jae    SHORT loc_1_3D88
        db  0FFh, 024h, 085h, 08Ah, 03Dh, 000h, 000h
loc_1_3D88:
        clc
        ret
        db  050h, 03Dh, 000h, 000h, 088h, 03Dh, 000h, 000h, 088h, 03Dh, 000h, 000h, 088h, 03Dh, 000h, 000h
        db  088h, 03Dh, 000h, 000h, 088h, 03Dh, 000h, 000h, 088h, 03Dh, 000h, 000h, 088h, 03Dh, 000h, 000h
        db  088h, 03Dh, 000h, 000h, 088h, 03Dh, 000h, 000h, 088h, 03Dh, 000h, 000h, 088h, 03Dh, 000h, 000h
        db  088h, 03Dh, 000h, 000h, 088h, 03Dh, 000h, 000h, 088h, 03Dh, 000h, 000h, 088h, 03Dh, 000h, 000h
        db  088h, 03Dh, 000h, 000h, 088h, 03Dh, 000h, 000h, 088h, 03Dh, 000h, 000h, 088h, 03Dh, 000h, 000h
        db  088h, 03Dh, 000h, 000h, 088h, 03Dh, 000h, 000h, 088h, 03Dh, 000h, 000h, 088h, 03Dh, 000h, 000h
        db  088h, 03Dh, 000h, 000h, 088h, 03Dh, 000h, 000h, 050h, 03Dh, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h
sub_1_3E00:
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
        call   sub_1_3E30
        pop    ebp
        ret    20h
        db  08Dh, 064h, 024h, 000h
sub_1_3E30:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 18h], 0eh
        jb     NEAR PTR loc_1_3ED0
        mov    edx, dword ptr [ebp + 10h]
        mov    al, byte ptr [edx]
        test   al, 1
        je     SHORT loc_1_3E70
        cmp    dword ptr [edx + 2], -1
        jne    SHORT loc_1_3E65
        cmp    al, 0ffh
        jne    SHORT loc_1_3E65
        cmp    byte ptr [edx + 1], 0ffh
        jne    SHORT loc_1_3E65
        mov    dword ptr [ebp - 4], 8
        jmp    SHORT loc_1_3ED7
loc_1_3E65:
        mov    dword ptr [ebp - 4], 2
        jmp    SHORT loc_1_3ED7
        db  08Bh, 0FFh
loc_1_3E70:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0ch], 20h
        je     SHORT loc_1_3EC0
        mov    edx, dword ptr [ebp + 10h]
        lea    ecx, [esi + 2ch]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_3E96
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 10h], 0
        je     SHORT loc_1_3E9D
loc_1_3E96:
        mov    dword ptr [ebp - 10h], 1
loc_1_3E9D:
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_3EB0
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_3EDA
        db  08Dh, 064h, 024h, 000h
loc_1_3EB0:
        mov    dword ptr [ebp - 4], 20h
        jmp    SHORT loc_1_3EDA
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3EC0:
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_3EDA
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3ED0:
        mov    dword ptr [ebp - 4], 20h
loc_1_3ED7:
        mov    esi, dword ptr [ebp + 8]
loc_1_3EDA:
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     NEAR PTR loc_1_3FD1
loc_1_3EE5:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, dword ptr [ebp - 4]
        and    eax, ebx
        test   al, 9
        jne    SHORT loc_1_3F3D
        test   al, 2
        je     SHORT loc_1_3F29
        lea    eax, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        db  0E8h, 066h, 0DFh, 0FEh, 0FFh
        or     al, al
        je     SHORT loc_1_3F29
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 19h]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0ch]
        test   dword ptr [ecx + eax*4], edx
        jne    SHORT loc_1_3F3D
loc_1_3F29:
        test   byte ptr [ebp - 4], 2
        je     SHORT loc_1_3F34
        test   bl, 4
        jne    SHORT loc_1_3F3D
loc_1_3F34:
        test   bl, 20h
        je     NEAR PTR loc_1_3FC6
loc_1_3F3D:
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
        call   sub_1_41B2
        mov    byte ptr [edi + 18h], 1
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_3FC6
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_3F7F
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_3F7F:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_3F90
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_3F96
        db  08Bh, 0FFh
loc_1_3F90:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_3F96:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_3FA5
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_41B8
loc_1_3FA5:
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
        call   sub_1_425C
        mov    edi, ebx
        jmp    SHORT loc_1_3FC9
loc_1_3FC6:
        mov    edi, dword ptr [edi + 10h]
loc_1_3FC9:
        or     edi, edi
        jne    NEAR PTR loc_1_3EE5
loc_1_3FD1:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_3FE0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3FF0
        pop    ebp
        ret    4
sub_1_3FF0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     SHORT loc_1_406E
loc_1_4000:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_4067
        mov    byte ptr [edi + 18h], 0
        inc    dword ptr [edi + 0ch]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_41B8
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_4067
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_402C
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_402C:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_4040
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_4046
        db  005h, 000h, 000h, 000h, 000h
loc_1_4040:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_4046:
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
        call   sub_1_425C
        mov    edi, ebx
        jmp    SHORT loc_1_406A
loc_1_4067:
        mov    edi, dword ptr [edi + 10h]
loc_1_406A:
        or     edi, edi
        jne    SHORT loc_1_4000
loc_1_406E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4080:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0ch]
        test   al, 20h
        je     SHORT loc_1_4095
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_4095:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    bl, byte ptr [ecx]
        test   bl, 1
        je     SHORT loc_1_40D1
        cmp    dword ptr [ecx + 2], -1
        jne    SHORT loc_1_40C2
        cmp    bl, 0ffh
        jne    SHORT loc_1_40C2
        cmp    byte ptr [ecx + 1], 0ffh
        jne    SHORT loc_1_40C2
        test   al, 8
        je     SHORT loc_1_40BB
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_40BB:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
loc_1_40C2:
        test   al, 4
        jne    SHORT loc_1_40CA
        test   al, 2
        je     SHORT loc_1_40E9
loc_1_40CA:
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_40D1:
        mov    eax, dword ptr [edx + 2eh]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_40E9
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 2ch], ax
        jne    SHORT loc_1_40E9
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_40E9:
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
sub_1_416A:
        db  0CDh, 020h, 031h, 080h, 028h, 000h
sub_1_4170:
        db  0CDh, 020h, 032h, 080h, 028h, 000h
sub_1_4176:
        db  0CDh, 020h, 033h, 080h, 028h, 000h
sub_1_417C:
        db  0CDh, 020h, 034h, 080h, 028h, 000h
sub_1_4182:
        db  0CDh, 020h, 035h, 080h, 028h, 000h
sub_1_4188:
        db  0CDh, 020h, 036h, 080h, 028h, 000h
        db  0CDh, 020h, 037h, 080h, 028h, 000h
sub_1_4194:
        db  0CDh, 020h, 038h, 080h, 028h, 000h
sub_1_419A:
        db  0CDh, 020h, 039h, 080h, 028h, 000h
        db  0CDh, 020h, 03Ah, 080h, 028h, 000h
sub_1_41A6:
        db  0CDh, 020h, 03Bh, 080h, 028h, 000h
sub_1_41AC:
        db  0CDh, 020h, 03Ch, 080h, 028h, 000h
sub_1_41B2:
        db  0CDh, 020h, 03Dh, 080h, 028h, 000h
sub_1_41B8:
        db  0CDh, 020h, 03Eh, 080h, 028h, 000h
sub_1_41BE:
        db  0CDh, 020h, 03Fh, 080h, 028h, 000h
sub_1_41C4:
        db  0CDh, 020h, 040h, 080h, 028h, 000h
        db  0CDh, 020h, 041h, 080h, 028h, 000h
        db  0CDh, 020h, 009h, 080h, 028h, 000h
        db  0CDh, 020h, 008h, 080h, 028h, 000h
sub_1_41DC:
        db  0CDh, 020h, 044h, 080h, 028h, 000h
        db  0CDh, 020h, 048h, 080h, 028h, 000h
        db  0CDh, 020h, 049h, 080h, 028h, 000h
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
sub_1_4232:
        db  0CDh, 020h, 00Ch, 080h, 028h, 000h
sub_1_4238:
        db  0CDh, 020h, 00Dh, 080h, 028h, 000h
sub_1_423E:
        db  0CDh, 020h, 00Fh, 080h, 028h, 000h
sub_1_4244:
        db  0CDh, 020h, 010h, 080h, 028h, 000h
        db  0CDh, 020h, 011h, 080h, 028h, 000h
sub_1_4250:
        db  0CDh, 020h, 00Eh, 080h, 028h, 000h
sub_1_4256:
        db  0CDh, 020h, 00Ah, 080h, 028h, 000h
sub_1_425C:
        db  0CDh, 020h, 00Bh, 080h, 028h, 000h
        db  0CDh, 020h, 016h, 080h, 028h, 000h
        db  0CDh, 020h, 017h, 080h, 028h, 000h
        db  0CDh, 020h, 018h, 080h, 028h, 000h
        db  0CDh, 020h, 019h, 080h, 028h, 000h
        db  0CDh, 020h, 01Ah, 080h, 028h, 000h
sub_1_4280:
        db  0CDh, 020h, 01Bh, 080h, 028h, 000h
        db  0CDh, 020h, 01Ch, 080h, 028h, 000h
        db  0CDh, 020h, 01Dh, 080h, 028h, 000h
        db  0CDh, 020h, 01Eh, 080h, 028h, 000h
        db  0CDh, 020h, 01Fh, 080h, 028h, 000h
        db  0CDh, 020h, 020h, 080h, 028h, 000h
sub_1_42A4:
        db  0CDh, 020h, 021h, 080h, 028h, 000h
        db  0CDh, 020h, 022h, 080h, 028h, 000h
        db  0CDh, 020h, 023h, 080h, 028h, 000h
        db  0CDh, 020h, 024h, 080h, 028h, 000h
        db  0CDh, 020h, 025h, 080h, 028h, 000h
        db  0CDh, 020h, 026h, 080h, 028h, 000h
sub_1_42C8:
        db  0CDh, 020h, 027h, 080h, 028h, 000h
        db  0CDh, 020h, 028h, 080h, 028h, 000h
sub_1_42D4:
        db  0CDh, 020h, 005h, 080h, 028h, 000h
sub_1_42DA:
        db  0CDh, 020h, 006h, 080h, 028h, 000h
sub_1_42E0:
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
        db  0CDh, 020h, 046h, 080h, 028h, 000h
sub_1_4328:
        db  0CDh, 020h, 045h, 080h, 028h, 000h
        db  0CDh, 020h, 043h, 080h, 028h, 000h
        db  0CDh, 020h, 047h, 080h, 028h, 000h
        add    byte ptr [eax], al
sub_1_433C:
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
sub_1_435C:
        push   esi
        push   edi
        pushfd
        mov    ecx, dword ptr [esp + 18h]
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 10h]
        cmp    esi, edi
        jae    SHORT loc_1_437C
        or     ecx, ecx
        je     SHORT loc_1_4392
        std
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    SHORT loc_1_4390
loc_1_437C:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     SHORT loc_1_4390
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd
        mov    cl, al
loc_1_4390:
        rep movsb
loc_1_4392:
        popfd
        pop    edi
        pop    esi
        ret    0ch
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 00Ah, 003h, 000h, 000h, 003h, 00Ah, 000h, 000h, 053h, 045h, 054h, 050h
        db  033h, 020h, 020h, 020h, 000h, 040h, 000h, 054h, 079h, 03Dh, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_4450:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_4454:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4458:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_445C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4460:
        add    byte ptr [eax], al
        rol    eax, 81h
        rol    dword ptr [eax + 1], 1
        ret
        db  0C0h, 003h, 080h, 002h, 041h, 0C2h, 001h, 0C6h, 0C0h, 006h, 080h, 007h, 041h, 0C7h, 000h, 005h
        db  0C1h, 0C5h, 081h, 0C4h, 040h, 004h, 001h, 0CCh, 0C0h, 00Ch, 080h, 00Dh, 041h, 0CDh, 000h, 00Fh
        db  0C1h, 0CFh, 081h, 0CEh, 040h, 00Eh, 000h, 00Ah, 0C1h, 0CAh, 081h, 0CBh, 040h, 00Bh, 001h, 0C9h
        db  0C0h, 009h, 080h, 008h, 041h, 0C8h, 001h, 0D8h, 0C0h, 018h, 080h, 019h, 041h, 0D9h, 000h, 01Bh
        db  0C1h, 0DBh, 081h, 0DAh, 040h, 01Ah, 000h, 01Eh, 0C1h, 0DEh, 081h, 0DFh, 040h, 01Fh, 001h, 0DDh
        db  0C0h, 01Dh, 080h, 01Ch, 041h, 0DCh, 000h, 014h, 0C1h, 0D4h, 081h, 0D5h, 040h, 015h, 001h, 0D7h
        db  0C0h, 017h, 080h, 016h, 041h, 0D6h, 001h, 0D2h, 0C0h, 012h, 080h, 013h, 041h, 0D3h, 000h, 011h
        db  0C1h, 0D1h, 081h, 0D0h, 040h, 010h, 001h, 0F0h, 0C0h, 030h, 080h, 031h, 041h, 0F1h, 000h, 033h
        db  0C1h, 0F3h, 081h, 0F2h, 040h, 032h, 000h, 036h, 0C1h, 0F6h, 081h, 0F7h, 040h, 037h, 001h, 0F5h
        db  0C0h, 035h, 080h, 034h, 041h, 0F4h, 000h, 03Ch, 0C1h, 0FCh, 081h, 0FDh, 040h, 03Dh, 001h, 0FFh
        db  0C0h, 03Fh, 080h, 03Eh, 041h, 0FEh, 001h, 0FAh, 0C0h, 03Ah, 080h, 03Bh, 041h, 0FBh, 000h, 039h
        db  0C1h, 0F9h, 081h, 0F8h, 040h, 038h, 000h, 028h, 0C1h, 0E8h, 081h, 0E9h, 040h, 029h, 001h, 0EBh
        db  0C0h, 02Bh, 080h, 02Ah, 041h, 0EAh, 001h, 0EEh, 0C0h, 02Eh, 080h, 02Fh, 041h, 0EFh, 000h, 02Dh
        db  0C1h, 0EDh, 081h, 0ECh, 040h, 02Ch, 001h, 0E4h, 0C0h, 024h, 080h, 025h, 041h, 0E5h, 000h, 027h
        db  0C1h, 0E7h, 081h, 0E6h, 040h, 026h, 000h, 022h, 0C1h, 0E2h, 081h, 0E3h, 040h, 023h, 001h, 0E1h
        db  0C0h, 021h, 080h, 020h, 041h, 0E0h, 001h, 0A0h, 0C0h, 060h, 080h, 061h, 041h, 0A1h, 000h, 063h
        db  0C1h, 0A3h, 081h, 0A2h, 040h, 062h, 000h, 066h, 0C1h, 0A6h, 081h, 0A7h, 040h, 067h, 001h, 0A5h
        db  0C0h, 065h, 080h, 064h, 041h, 0A4h, 000h, 06Ch, 0C1h, 0ACh, 081h, 0ADh, 040h, 06Dh, 001h, 0AFh
        db  0C0h, 06Fh, 080h, 06Eh, 041h, 0AEh, 001h, 0AAh, 0C0h, 06Ah, 080h, 06Bh, 041h, 0ABh, 000h, 069h
        db  0C1h, 0A9h, 081h, 0A8h, 040h, 068h, 000h, 078h, 0C1h, 0B8h, 081h, 0B9h, 040h, 079h, 001h, 0BBh
        db  0C0h, 07Bh, 080h, 07Ah, 041h, 0BAh, 001h, 0BEh, 0C0h, 07Eh, 080h, 07Fh, 041h, 0BFh, 000h, 07Dh
        db  0C1h, 0BDh, 081h, 0BCh, 040h, 07Ch, 001h, 0B4h, 0C0h, 074h, 080h, 075h, 041h, 0B5h, 000h, 077h
        db  0C1h, 0B7h, 081h, 0B6h, 040h, 076h, 000h, 072h, 0C1h, 0B2h, 081h, 0B3h, 040h, 073h, 001h, 0B1h
        db  0C0h, 071h, 080h, 070h, 041h, 0B0h, 000h, 050h, 0C1h, 090h, 081h, 091h, 040h, 051h, 001h, 093h
        db  0C0h, 053h, 080h, 052h, 041h, 092h, 001h, 096h, 0C0h, 056h, 080h, 057h, 041h, 097h, 000h, 055h
        db  0C1h, 095h, 081h, 094h, 040h, 054h, 001h, 09Ch, 0C0h, 05Ch, 080h, 05Dh, 041h, 09Dh, 000h, 05Fh
        db  0C1h, 09Fh, 081h, 09Eh, 040h, 05Eh, 000h, 05Ah, 0C1h, 09Ah, 081h, 09Bh, 040h, 05Bh, 001h, 099h
        db  0C0h, 059h, 080h, 058h, 041h, 098h, 001h, 088h, 0C0h, 048h, 080h, 049h, 041h, 089h, 000h, 04Bh
        db  0C1h, 08Bh, 081h, 08Ah, 040h, 04Ah, 000h, 04Eh, 0C1h, 08Eh, 081h, 08Fh, 040h, 04Fh, 001h, 08Dh
        db  0C0h, 04Dh, 080h, 04Ch, 041h, 08Ch, 000h, 044h, 0C1h, 084h, 081h, 085h, 040h, 045h, 001h, 087h
        db  0C0h, 047h, 080h, 046h, 041h, 086h, 001h, 082h, 0C0h, 042h, 080h, 043h, 041h, 083h, 000h, 041h
        db  0C1h, 081h, 081h, 080h, 040h, 040h
loc_1_4660:
        inc    eax
        add    byte ptr [eax], al
        add    al, al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_466C:
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4670:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4674:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4678:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_467C:
        inc    ebp
        outsb
        push   65636e61h
        and    byte ptr fs:[eax + 61h], dl
        jb     SHORT loc_1_46EA
        insb
        insb
        insb
        and    byte ptr [eax + 6fh], dl
        jb     SHORT loc_1_4706
        add    byte ptr [eax], al
loc_1_4694:
        push   eax
        push   ebx
        das
        xor    ah, byte ptr [eax]
        inc    edx
        imul   ebp, dword ptr [65726964h], 6f697463h
        outsb
        popad
        insb
        and    byte ptr [ebx + 52h], bl
        pop    ebp
        add    byte ptr [edi + ecx*2 + 53h], dl
        dec    eax
        dec    ecx
        inc    edx
        inc    ecx
        and    byte ptr [eax], ah
        inc    edx
        imul   ebp, dword ptr [65726964h], 6f697463h
        outsb
        popad
        insb
        and    byte ptr [ebx + 52h], bl
        pop    ebp
        add    byte ptr [edx + 69h], al
        sub    eax, 65726964h
        arpl   word ptr [ecx + ebp*2 + 6fh], si
        outsb
        popad
        insb
        and    byte ptr [ebx + 52h], bl
        pop    ebp
        add    byte ptr [eax], al
loc_1_46DC:
        push   ebp
        outsb
        imul   esp, dword ptr [ecx + ebp*2 + 72h], 69746365h
        outsd
        outsb
        popad
        insb
loc_1_46EA:
        and    byte ptr [ebx + 52h], bl
        pop    ebp
        add    byte ptr [eax], al
loc_1_46F0:
        push   ebp
        outsb
        imul   esp, dword ptr [ecx + ebp*2 + 72h], 69746365h
        outsd
        outsb
        popad
        insb
        and    byte ptr [ebx + 46h], bl
        pop    ebp
        add    byte ptr [eax], al
loc_1_4704:
        add    byte ptr [eax], al
loc_1_4706:
        add    byte ptr [eax], al
loc_1_4708:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_470C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4710:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4714:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4718:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_471C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4720:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4724:
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_47E4:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_1_4820:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_4874:
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
        add    dword ptr [edx], eax
        add    byte ptr [edx], al
        add    dword ptr [edx], eax
        add    byte ptr [ebx], al
        add    dword ptr [edx], eax
        add    byte ptr [ecx + eax], al
        add    al, byte ptr [eax]
        add    eax, 1000201h
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ebx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx + eax], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx]
        add    al, byte ptr [ecx]
        add    al, byte ptr [ecx]
        add    eax, dword ptr [ecx]
        add    al, byte ptr [ecx]
loc_1_48F0:
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   ebx
        inc    ebp
        push   esp
        push   eax
        xor    eax, dword ptr [eax]
        add    byte ptr [eax], al
loc_1_4958:
        push   ebx
        inc    ebp
        push   esp
        push   eax
        xor    eax, dword ptr [eax]
        add    byte ptr [eax], al
        push   eax
        push   eax
        pop    edi
        dec    ebp
        outsd
        db  064h, 065h, 000h
loc_1_4968:
        push   eax
        push   eax
        pop    edi
        dec    ebp
        outsd
        add    byte ptr gs:[ecx + ebp*2 + 73h], al
        popad
        bound  ebp, qword ptr [ebp + 5fh]
        dec    esp
        imul   ebp, dword ptr [esi + 6bh], 0
loc_1_4980:
        inc    esp
        imul   esi, dword ptr [ebx + 61h], 5f656c62h
        dec    esp
        imul   ebp, dword ptr [esi + 6bh], 0
        dec    ebp
        imul   ebp, dword ptr [esi + 69h], 6f6c425fh
        arpl   word ptr [ebx], bp
        add    byte ptr [ebp + 69h], cl
        outsb
        imul   ebx, dword ptr [edi + 42h], 6b636f6ch
        add    byte ptr [eax], al
        dec    esp
        push   eax
        push   esp
        db  000h
loc_1_49AC:
        dec    esp
        push   eax
        push   esp
        add    byte ptr [ebp + 41h], cl
        pop    eax
        dec    ebp
        push   ebp
        dec    esp
        push   esp
        dec    ecx
        inc    ebx
        inc    ecx
        push   ebx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_49C0:
        dec    ebp
        inc    ecx
        pop    eax
        dec    ebp
        push   ebp
        dec    esp
        push   esp
        dec    ecx
        inc    ebx
        inc    ecx
        push   ebx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    esi
        inc    ebp
        push   esp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        dec    esi
        inc    ebp
        push   esp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
loc_1_49E8:
        push   ebx
        imul   ebp, dword ptr [ecx + ebp*2 + 63h], 45206d6fh
        db  074h, 068h
        jb     NEAR PTR loc_1_4A64
        db  065h, 074h, 020h
        push   eax
        outsd
        arpl   word ptr [ebx + 65h], bp
        je     SHORT sub_1_4A20
        inc    ecx
        popad
        db  070h, 074h
        jb     NEAR PTR loc_1_4A36
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
_LTEXT ends
        end
