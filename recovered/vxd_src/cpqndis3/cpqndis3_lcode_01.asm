; cpqndis3_lcode_01.asm -- CPQNDIS3 object 1 segment _LTEXT
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
        push   esi
        push   eax
        call   sub_1_0660
        push   0
        push   0bh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0
        push   0ah
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 1]
        push   eax
        push   7
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        test   byte ptr [ebp - 1], 80h
        jne    SHORT loc_1_0196
        mov    esi, 0ah
loc_1_018C:
        push   64h
        call   sub_1_3CC0
        dec    esi
        jne    SHORT loc_1_018C
loc_1_0196:
        push   2
        push   0dh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   22h
        push   0
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_01C0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    edi, 14h
        call   sub_1_0150
loc_1_01D3:
        push   64h
        call   sub_1_3CC0
        dec    edi
        jne    SHORT loc_1_01D3
        call   sub_1_0F30
        call   sub_1_0A70
        push   esi
        call   0
        test   al, al
        jne    SHORT loc_1_0210
        push   1
        mov    eax, dword ptr [esi + 4]
        push   1
        push   2
        push   0aa000003h
        push   eax
        call   sub_1_3D98
        add    esp, 14h
        xor    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_0210:
        call   sub_1_0F10
        push   0
        push   0dh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0230:
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
        call   sub_1_3CF0
        cmp    dword ptr [ebp - 14h], 0
        jne    SHORT loc_1_025F
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_025F:
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
        call   sub_1_3D14
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_02AF
        xor    edi, edi
loc_1_0287:
        cmp    dword ptr [ebp - 0ch], edi
        jne    SHORT loc_1_02AF
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_3D38
        lea    ecx, [ebp - 0ch]
        lea    edx, [ebp - 10h]
        push   ecx
        mov    eax, dword ptr [ebp - 8]
        push   edx
        push   eax
        call   sub_1_3D14
        cmp    dword ptr [ebp - 8], edi
        jne    SHORT loc_1_0287
loc_1_02AF:
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
loc_1_0329:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        push   eax
        push   ecx
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 8]
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_3D38
        cmp    dword ptr [ebp - 8], esi
        je     SHORT loc_1_038C
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 10h]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_3D14
        cmp    dword ptr [ebp - 8], esi
        je     SHORT loc_1_0391
loc_1_035F:
        cmp    dword ptr [ebp - 0ch], esi
        jne    SHORT loc_1_038C
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_3D38
        cmp    dword ptr [ebp - 8], esi
        je     SHORT loc_1_038C
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 10h]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_3D14
        cmp    dword ptr [ebp - 8], esi
        jne    SHORT loc_1_035F
loc_1_038C:
        cmp    dword ptr [ebp - 8], esi
        jne    SHORT loc_1_0329
loc_1_0391:
        mov    ecx, dword ptr [ebp - 14h]
        mov    eax, dword ptr [ebp + 10h]
        mov    esi, 0ffffh
        mov    ebx, 40h
        mov    dword ptr [eax], ecx
loc_1_03A3:
        test   esi, esi
        je     SHORT loc_1_03B9
        dec    esi
        lea    eax, [ebp - 1]
        push   eax
        push   7
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        test   byte ptr [ebp - 1], bl
        je     SHORT loc_1_03A3
loc_1_03B9:
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
sub_1_03D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        test   esi, esi
        jne    SHORT loc_1_03EB
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_03EB:
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
loc_1_0478:
        test   si, si
        je     SHORT loc_1_0490
        dec    si
        lea    eax, [ebp - 1]
        push   eax
        push   7
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        test   byte ptr [ebp - 1], bl
        je     SHORT loc_1_0478
loc_1_0490:
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
sub_1_04B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        test   esi, esi
        jne    SHORT loc_1_04CB
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_04CB:
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
loc_1_0526:
        test   esi, esi
        je     SHORT loc_1_053C
        dec    esi
        lea    eax, [ebp - 1]
        push   eax
        push   7
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        test   byte ptr [ebp - 1], bl
        je     SHORT loc_1_0526
loc_1_053C:
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
sub_1_0550:
        push   ebp
        mov    eax, 0ffffffffh
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        cmp    dword ptr [ebp + 0ch], esi
        je     SHORT loc_1_0598
loc_1_0562:
        mov    ecx, dword ptr [ebp + 8]
        mov    dl, byte ptr [esi + ecx]
        mov    ecx, 8
loc_1_056D:
        mov    edi, eax
        mov    bl, dl
        shr    dl, 1
        and    edi, 80000000h
        shr    edi, 1fh
        and    ebx, 1
        xor    edi, ebx
        add    eax, eax
        test   edi, edi
        je     SHORT loc_1_058F
        xor    eax, 4c11db6h
        or     eax, edi
loc_1_058F:
        dec    ecx
        jne    SHORT loc_1_056D
        inc    esi
        cmp    dword ptr [ebp + 0ch], esi
        ja     SHORT loc_1_0562
loc_1_0598:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  090h
sub_1_05A0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   6
        push   eax
        call   sub_1_0550
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
sub_1_05D0:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        mov    eax, 8
        sub    esp, 0ch
        mov    dword ptr [ebp - 8], edx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
loc_1_05E5:
        mov    ecx, dword ptr [ebp - 8]
        inc    dword ptr [ebp - 8]
        mov    byte ptr [ecx + esi + 14fh], dl
        cmp    dword ptr [ebp - 8], eax
        jb     SHORT loc_1_05E5
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 8]
        push   eax
        mov    ecx, dword ptr [esi + 14bh]
        push   60h
        lea    edx, [ebp - 0ch]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0612:
        cmp    dword ptr [ebp - 8], 0
        jbe    SHORT loc_1_064C
        xor    edi, edi
loc_1_061A:
        lea    eax, [ebp - 1]
        lea    ecx, [ebp - 2]
        push   eax
        dec    dword ptr [ebp - 8]
        push   ecx
        mov    edx, dword ptr [ebp - 8]
        lea    eax, [edx + edx*2]
        db  08Dh, 00Ch, 045h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_1_05A0
        xor    edx, edx
        mov    cl, byte ptr [ebp - 1]
        mov    dl, byte ptr [ebp - 2]
        or     byte ptr [edx + esi + 14fh], cl
        cmp    dword ptr [ebp - 8], edi
        ja     SHORT loc_1_061A
loc_1_064C:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0660:
        push   21h
        push   0
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        xor    al, al
        ret    4
        db  090h
sub_1_0670:
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
        jae    SHORT loc_1_06A0
        mov    esi, 3ch
loc_1_06A0:
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
sub_1_06E0:
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
sub_1_0710:
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
sub_1_0730:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    cl, byte ptr [eax + 0fch]
        cmp    byte ptr [eax + 0fah], cl
        jne    SHORT loc_1_075A
        mov    al, byte ptr [eax + 0fbh]
        dec    al
        push   eax
        push   3
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        pop    ebp
        ret    4
loc_1_075A:
        dec    cl
        push   ecx
        push   3
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0770:
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
sub_1_0790:
        push   esi
        push   62h
        push   0
        xor    esi, esi
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
loc_1_07A2:
        lea    ecx, [esi + 8]
        push   0ffh
        push   ecx
        inc    esi
        call   eax
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    esi, 8
        jb     SHORT loc_1_07A2
        push   22h
        push   0
        call   eax
        xor    al, al
        pop    esi
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_07D0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   62h
        push   0
        xor    edi, edi
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [ebp + 8]
loc_1_07E4:
        mov    al, byte ptr [esi + edi + 14fh]
        lea    ecx, [edi + 8]
        push   eax
        inc    edi
        push   ecx
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        cmp    edi, 8
        jb     SHORT loc_1_07E4
        push   22h
        push   0
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        xor    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_0810:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        test   byte ptr [eax + 103h], 4
        je     SHORT loc_1_0826
        push   eax
        call   sub_1_0830
loc_1_0826:
        xor    al, al
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_0830:
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
sub_1_0890:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, 14h
        push   esi
        call   sub_1_0660
loc_1_08A6:
        push   64h
        call   sub_1_3CC0
        dec    edi
        jne    SHORT loc_1_08A6
        test   byte ptr [esi + 103h], 0ah
        jne    SHORT loc_1_08D8
        lea    eax, [ebp - 1]
        push   eax
        push   7
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        test   byte ptr [ebp - 1], 0ah
        jne    SHORT loc_1_08D8
        mov    al, byte ptr [esi + 5bh]
        mov    byte ptr [esi + 5ch], al
        mov    byte ptr [esi + 1b1h], 0
loc_1_08D8:
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
sub_1_0920:
        push   ebp
        mov    ebp, esp
        pushf
        cli
        cmp    byte ptr [ebp + 8], 1
        je     SHORT loc_1_0938
        cmp    byte ptr [ebp + 8], 2
        je     SHORT loc_1_098D
        cmp    byte ptr [ebp + 8], 3
        je     SHORT loc_1_0963
loc_1_0938:
        mov    dx, 0fbh
        out    dx, al
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        add    dx, 402h
        in     al, dx
        and    al, 1fh
        out    dx, al
        mov    dx, 0ech
        mov    al, 5ch
        out    dx, al
        mov    dx, 0edh
        in     al, dx
        and    al, 0dfh
        out    dx, al
        mov    dx, 0fbh
        out    dx, al
        jmp    SHORT loc_1_09B8
loc_1_0963:
        mov    dx, 0fbh
        out    dx, al
        mov    dx, 0ech
        mov    al, 5bh
        out    dx, al
        mov    dx, 0edh
        in     al, dx
        and    al, 9fh
        out    dx, al
        mov    dx, 0ech
        mov    al, 5ch
        out    dx, al
        mov    dx, 0edh
        in     al, dx
        or     al, 20h
        out    dx, al
        mov    dx, 0fbh
        out    dx, al
        jmp    SHORT loc_1_09B8
loc_1_098D:
        mov    dx, 0fbh
        out    dx, al
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        add    dx, 402h
        in     al, dx
        and    al, 1fh
        or     al, 20h
        out    dx, al
        mov    dx, 0ech
        mov    al, 5ch
        out    dx, al
        mov    dx, 0edh
        in     al, dx
        and    al, 0dfh
        out    dx, al
        mov    dx, 0fbh
        out    dx, al
loc_1_09B8:
        popf
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_09C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushf
        cli
        mov    dx, 0fbh
        out    dx, al
        mov    dx, 0ech
        mov    al, 5ch
        out    dx, al
        mov    dx, 0edh
        in     al, dx
        and    al, 20h
        je     SHORT loc_1_09E2
        mov    ah, 3
        jmp    SHORT loc_1_0A01
loc_1_09E2:
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        add    dx, 402h
        in     al, dx
        test   al, 0e0h
        jne    SHORT loc_1_09F7
        mov    ah, 1
        jmp    SHORT loc_1_0A01
loc_1_09F7:
        test   al, 0c0h
        jne    SHORT loc_1_09FF
        mov    ah, 2
        jmp    SHORT loc_1_0A01
loc_1_09FF:
        mov    ah, 0ffh
loc_1_0A01:
        mov    dx, 0f9h
        out    dx, al
        mov    byte ptr [ebp - 1], ah
        popf
        mov    al, byte ptr [ebp - 1]
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0A20:
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
sub_1_0A70:
        push   0aah
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   14h
        call   sub_1_3CC0
        push   0
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   14h
        call   sub_1_3CC0
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0AA0:
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
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 0A0h
        db  000h, 000h, 000h, 000h, 08Ah, 00Dh, 000h, 000h, 000h, 000h, 00Ch, 008h, 080h, 0C9h, 004h, 00Ch
        db  010h, 00Ah, 00Dh, 000h, 000h, 000h, 000h, 00Ah, 00Dh, 000h, 000h, 000h, 000h, 0A2h, 000h, 000h
        db  000h, 000h, 0C6h, 005h, 000h, 000h, 000h, 000h, 000h, 088h, 00Dh, 000h, 000h, 000h, 000h, 0C3h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0B20:
        push   ebp
        mov    ebp, esp
        push   esi
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
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_0B6A
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_0B6A
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_0B6A
        push   ecx
        mov    ecx, 1000h
loc_1_0B62:
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_0B6A
        loop   loc_1_0B62
        pop    ecx
loc_1_0B6A:
        dec    dx
        in     al, dx
        mov    esi, dword ptr [ebp + 0ch]
        mov    byte ptr [esi], al
        add    dx, 2
        mov    al, ah
        out    dx, al
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_1_0B80:
        push   ebp
        mov    ebp, esp
        push   esi
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
        mov    esi, dword ptr [ebp + 8]
        add    dx, 2
loc_1_0BA9:
        mov    ax, 3537h
        out    dx, al
        dec    dx
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_0BCD
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_0BCD
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_0BCD
        push   ecx
        mov    ecx, 1000h
loc_1_0BC5:
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_0BCD
        loop   loc_1_0BC5
        pop    ecx
loc_1_0BCD:
        dec    dx
        in     al, dx
        mov    byte ptr [esi], al
        inc    esi
        add    dx, 2
        mov    al, ah
        out    dx, al
        loop   loc_1_0BA9
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0BF0:
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0C00:
        db  0A0h, 000h, 000h, 000h, 000h
        or     al, 8
        or     al, 10h
        db  0A2h, 000h, 000h, 000h, 000h
        db  0A0h, 000h, 000h, 000h, 000h
        and    al, 0fbh
        or     al, 8
        db  00Ah, 005h, 000h, 000h, 000h, 000h
        or     al, 2
        db  0A2h, 000h, 000h, 000h, 000h
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0C30:
        db  0A0h, 000h, 000h, 000h, 000h
        and    al, 0f7h
        db  0A2h, 000h, 000h, 000h, 000h
        ret
        db  08Dh, 049h, 000h
loc_1_0C40:
        push   ebp
        mov    ebp, esp
        push   esi
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        add    dx, 2
        in     al, dx
        and    al, 0dfh
        out    dx, al
        inc    dx
        mov    al, byte ptr [ebp + 8]
        or     al, 20h
        out    dx, al
        dec    dx
        jmp    NEAR PTR loc_1_0C62
loc_1_0C62:
        jmp    NEAR PTR loc_1_0C67
loc_1_0C67:
        in     al, dx
        or     al, 20h
        out    dx, al
        add    dx, 2
        jmp    NEAR PTR loc_1_0C74
loc_1_0C74:
        in     al, dx
        mov    esi, dword ptr [ebp + 0ch]
        mov    byte ptr [esi], al
        pop    esi
        pop    ebp
        ret    8
        db  090h
loc_1_0C80:
        push   ebp
        mov    ebp, esp
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        add    dx, 2
        in     al, dx
        and    al, 0dfh
        out    dx, al
        inc    dx
        mov    al, byte ptr [ebp + 8]
        out    dx, al
        jmp    NEAR PTR loc_1_0C9D
loc_1_0C9D:
        jmp    NEAR PTR loc_1_0CA2
loc_1_0CA2:
        jmp    NEAR PTR loc_1_0CA7
loc_1_0CA7:
        mov    al, byte ptr [ebp + 0ch]
        inc    dx
        out    dx, al
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0CC0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        add    dx, 2
        in     al, dx
        and    al, 0dfh
        out    dx, al
        inc    dx
        mov    al, 30h
        out    dx, al
        cld
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        dec    dx
        in     al, dx
        or     al, 20h
        out    dx, al
        add    dx, 2
        rep insb
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0D00:
        push   ebp
        mov    ebp, esp
        push   esi
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        add    dx, 2
        in     al, dx
        and    al, 0dfh
        out    dx, al
        inc    dx
        mov    al, 10h
        out    dx, al
        cld
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        inc    dx
        rep outsb
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_0D30:
        push   ebp
        mov    ebp, esp
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        add    dx, 2
        in     al, dx
        and    al, 0dfh
        out    dx, al
        inc    dx
        mov    al, byte ptr [ebp + 8]
        out    dx, al
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_0D50:
        push   ebp
        mov    ebp, esp
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        add    dx, 2
        in     al, dx
        and    al, 0dfh
        out    dx, al
        inc    dx
        mov    al, byte ptr [ebp + 8]
        out    dx, al
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_0D70:
        push   ebp
        mov    ebp, esp
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
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_0DB8
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_0DB8
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_0DB8
        push   ecx
        mov    ecx, 1000h
loc_1_0DB0:
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_0DB8
        loop   loc_1_0DB0
        pop    ecx
loc_1_0DB8:
        inc    dx
        mov    al, ah
        out    dx, al
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0DD0:
        push   ebp
        mov    ebp, esp
        push   esi
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
loc_1_0DEE:
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [esi]
        inc    esi
        out    dx, al
        add    dx, 2
        mov    ax, 1517h
        out    dx, al
        dec    dx
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_0E21
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_0E21
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_0E21
        push   ecx
        mov    ecx, 1000h
loc_1_0E19:
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_0E21
        loop   loc_1_0E19
        pop    ecx
loc_1_0E21:
        inc    dx
        mov    al, ah
        out    dx, al
        loop   loc_1_0DEE
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
loc_1_0E30:
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0E40:
        push   ebp
        mov    ebp, esp
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
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_0E70:
        push   ebp
        mov    ebp, esp
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
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0EA0:
        pushf
        cli
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_0EBF
        call   sub_1_0F50
        db  0A0h, 000h, 000h, 000h, 000h
        and    al, 0feh
        push   eax
        db  0FFh, 015h, 000h, 000h, 000h, 000h
loc_1_0EBF:
        db  0FEh, 00Dh, 000h, 000h, 000h, 000h
        popf
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0ED0:
        pushf
        cli
        db  0A0h, 000h, 000h, 000h, 000h
        push   eax
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        db  0FEh, 005h, 000h, 000h, 000h, 000h
        js     SHORT loc_1_0F01
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 000h
        db  0A0h, 000h, 000h, 000h, 000h
        or     al, 1
        push   eax
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        call   sub_1_0FF0
loc_1_0F01:
        popf
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0F10:
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
sub_1_0F30:
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
sub_1_0F50:
        call   sub_1_09C0
        db  0A2h, 000h, 000h, 000h, 000h
        cmp    al, 2
        je     SHORT loc_1_0F65
        push   2
        call   sub_1_0920
loc_1_0F65:
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        in     al, dx
        db  0A2h, 000h, 000h, 000h, 000h
        add    dx, 2
        mov    al, 14h
        out    dx, al
        sub    dx, 2
        mov    al, 0ffh
        out    dx, al
        in     al, 61h
        in     al, 61h
        in     al, 61h
        mov    al, 0dfh
        out    dx, al
        in     al, 61h
        in     al, 61h
        in     al, 61h
        mov    al, 0ffh
        out    dx, al
        in     al, 61h
        in     al, 61h
        in     al, 61h
        mov    al, 0fbh
        out    dx, al
        in     al, 61h
        in     al, 61h
        in     al, 61h
        mov    al, 0dbh
        out    dx, al
        in     al, 61h
        in     al, 61h
        in     al, 61h
        mov    al, 0fbh
        out    dx, al
        in     al, 61h
        in     al, 61h
        in     al, 61h
        mov    al, 0dbh
        out    dx, al
        in     al, 61h
        in     al, 61h
        in     al, 61h
        mov    al, 0fbh
        out    dx, al
        in     al, 61h
        in     al, 61h
        in     al, 61h
        mov    al, 0ffh
        out    dx, al
        in     al, 61h
        in     al, 61h
        in     al, 61h
        mov    al, 0dfh
        out    dx, al
        in     al, 61h
        in     al, 61h
        in     al, 61h
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_0FE8
        push   3
        call   sub_1_0920
        ret
loc_1_0FE8:
        push   2
        call   sub_1_0920
        ret
sub_1_0FF0:
        push   2
        call   sub_1_0920
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        mov    al, 0
        out    dx, al
        add    dx, 2
        mov    al, 1ch
        out    dx, al
        mov    al, 14h
        out    dx, al
        sub    dx, 2
        mov    al, 0ffh
        out    dx, al
        in     al, 61h
        in     al, 61h
        in     al, 61h
        mov    al, 0dfh
        out    dx, al
        in     al, 61h
        in     al, 61h
        in     al, 61h
        mov    al, 0ffh
        out    dx, al
        in     al, 61h
        in     al, 61h
        in     al, 61h
        mov    al, 0fbh
        out    dx, al
        in     al, 61h
        in     al, 61h
        in     al, 61h
        mov    al, 0dbh
        out    dx, al
        in     al, 61h
        in     al, 61h
        in     al, 61h
        mov    al, 0fbh
        out    dx, al
        in     al, 61h
        in     al, 61h
        in     al, 61h
        mov    al, 0dbh
        out    dx, al
        in     al, 61h
        in     al, 61h
        in     al, 61h
        mov    al, 0fbh
        out    dx, al
        in     al, 61h
        in     al, 61h
        in     al, 61h
        mov    al, 0dbh
        out    dx, al
        in     al, 61h
        in     al, 61h
        in     al, 61h
        db  0A0h, 000h, 000h, 000h, 000h
        out    dx, al
        in     al, 61h
        in     al, 61h
        in     al, 61h
        db  0A0h, 000h, 000h, 000h, 000h
        cmp    al, 2
        je     SHORT loc_1_107B
        push   eax
        call   sub_1_0920
loc_1_107B:
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_1080:
        call   sub_1_0EA0
        push   0
        push   0fh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        call   sub_1_0ED0
        mov    al, 1
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_10A0:
        xor    al, al
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_10B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 11dh], 0
        jne    NEAR PTR loc_1_1304
        lea    edi, [esi + 103h]
        call   sub_1_0EA0
        push   edi
        inc    dword ptr [esi + 30h]
        push   7
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [edi]
        test   al, al
        je     SHORT loc_1_10EC
        push   eax
        push   7
        db  0FFh, 015h, 000h, 000h, 000h, 000h
loc_1_10EC:
        mov    al, byte ptr [edi]
        test   al, 20h
        je     SHORT loc_1_10F9
        mov    eax, 8
        jmp    SHORT loc_1_112D
loc_1_10F9:
        test   al, 10h
        je     SHORT loc_1_110A
        push   esi
        call   sub_1_0830
        mov    eax, 4
        jmp    SHORT loc_1_112D
loc_1_110A:
        test   al, 0ah
        je     SHORT loc_1_1115
        mov    eax, 2
        jmp    SHORT loc_1_112D
loc_1_1115:
        test   al, 1
        jne    SHORT loc_1_1128
        test   al, 4
        mov    eax, 10h
        je     SHORT loc_1_112D
        push   esi
        call   sub_1_0830
loc_1_1128:
        mov    eax, 1
loc_1_112D:
        xor    ebx, ebx
loc_1_112F:
        cmp    eax, 10h
        jne    SHORT loc_1_114C
        cmp    dword ptr [esi + 161h], ebx
        je     NEAR PTR loc_1_1235
        cmp    byte ptr [esi + 13eh], bl
        jne    NEAR PTR loc_1_1235
loc_1_114C:
        dec    eax
        cmp    eax, 7
        ja     SHORT loc_1_11B6
        xor    ecx, ecx
        db  08Ah, 088h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 08Dh, 000h, 000h, 000h, 000h
loc_1_1161:
        push   esi
        call   sub_1_0710
        mov    byte ptr [esi + 1b1h], bl
        cmp    byte ptr [esi + 11bh], bl
        jne    SHORT loc_1_1194
        mov    byte ptr [esi + 5bh], bl
        mov    dword ptr [esi + 147h], ebx
        test   byte ptr [edi], 8
        je     SHORT loc_1_1189
        push   esi
        call   sub_1_24A0
loc_1_1189:
        test   byte ptr [edi], 2
        je     SHORT loc_1_1194
        push   esi
        call   sub_1_1560
loc_1_1194:
        and    byte ptr [edi], 0f5h
        jmp    SHORT loc_1_11B6
loc_1_1199:
        mov    byte ptr [esi + 118h], 1
        and    byte ptr [edi], 0efh
loc_1_11A3:
        push   esi
        call   sub_1_1330
        test   al, al
        je     SHORT loc_1_11B0
        and    byte ptr [edi], 0fah
loc_1_11B0:
        push   esi
        call   sub_1_2140
loc_1_11B6:
        cmp    dword ptr [esi + 161h], ebx
        je     SHORT loc_1_11CC
        cmp    byte ptr [esi + 13eh], bl
        jne    SHORT loc_1_11CC
        push   esi
        call   sub_1_1330
loc_1_11CC:
        mov    al, byte ptr [edi]
        test   al, 20h
        je     SHORT loc_1_11E7
        mov    eax, 8
        jmp    NEAR PTR loc_1_112F
loc_1_11DC:
        push   esi
        call   sub_1_0830
        and    byte ptr [edi], 0dfh
        jmp    SHORT loc_1_11B6
loc_1_11E7:
        test   al, 10h
        je     SHORT loc_1_11FB
        push   esi
        call   sub_1_0830
        mov    eax, 4
        jmp    NEAR PTR loc_1_112F
loc_1_11FB:
        test   al, 0ah
        je     SHORT loc_1_1209
        mov    eax, 2
        jmp    NEAR PTR loc_1_112F
loc_1_1209:
        test   al, 1
        je     SHORT loc_1_1217
        mov    eax, 1
        jmp    NEAR PTR loc_1_112F
loc_1_1217:
        test   al, 4
        je     SHORT loc_1_122B
        push   esi
        call   sub_1_0830
        mov    eax, 1
        jmp    NEAR PTR loc_1_112F
loc_1_122B:
        mov    eax, 10h
        jmp    NEAR PTR loc_1_112F
loc_1_1235:
        push   edi
        push   7
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [edi]
        test   al, al
        je     SHORT loc_1_124D
        push   eax
        push   7
        db  0FFh, 015h, 000h, 000h, 000h, 000h
loc_1_124D:
        mov    al, byte ptr [edi]
        test   al, 20h
        je     SHORT loc_1_125A
        mov    eax, 8
        jmp    SHORT loc_1_128E
loc_1_125A:
        test   al, 10h
        je     SHORT loc_1_126B
        push   esi
        call   sub_1_0830
        mov    eax, 4
        jmp    SHORT loc_1_128E
loc_1_126B:
        test   al, 0ah
        je     SHORT loc_1_1276
        mov    eax, 2
        jmp    SHORT loc_1_128E
loc_1_1276:
        test   al, 1
        jne    SHORT loc_1_1289
        test   al, 4
        mov    eax, 10h
        je     SHORT loc_1_128E
        push   esi
        call   sub_1_0830
loc_1_1289:
        mov    eax, 1
loc_1_128E:
        cmp    eax, 10h
        jne    NEAR PTR loc_1_114C
        mov    byte ptr [esi + 11dh], 0
        mov    byte ptr [esi + 15ch], 1bh
        push   1bh
        push   0fh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esi + 30h]
        dec    eax
        mov    dword ptr [esi + 30h], eax
        jne    SHORT loc_1_12E0
        cmp    byte ptr [esi + 13eh], 0
        jne    SHORT loc_1_12D0
        cmp    dword ptr [esi + 16dh], 0
        jne    SHORT loc_1_12D0
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_1_12E0
loc_1_12D0:
        push   1
        add    esi, 179h
        push   esi
        call   sub_1_3CA2
        jmp    SHORT loc_1_12FF
loc_1_12E0:
        cmp    dword ptr [esi + 47h], 0
        je     SHORT loc_1_12FF
        mov    eax, dword ptr [esi + 4fh]
        cmp    dword ptr [esi + eax*4 + 5dh], 0
        jne    SHORT loc_1_12FF
        cmp    byte ptr [esi + 11dh], 0
        jne    SHORT loc_1_12FF
        push   esi
        call   sub_1_2140
loc_1_12FF:
        call   sub_1_0ED0
loc_1_1304:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  090h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 001h, 004h, 002h, 004h, 004h, 004h, 003h, 08Dh, 064h, 024h
        db  000h, 08Dh, 064h, 024h, 000h
sub_1_1330:
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
        call   sub_1_06E0
        cmp    byte ptr [esi + 13eh], 0
        jne    SHORT loc_1_1365
        cmp    byte ptr [esi + 118h], 0
        je     SHORT loc_1_1365
        push   esi
        call   sub_1_0890
loc_1_1365:
        xor    ebx, ebx
loc_1_1367:
        cmp    byte ptr [esi + 13eh], bl
        jne    NEAR PTR loc_1_1454
        test   byte ptr [esi + 103h], 4
        jne    NEAR PTR loc_1_14FB
        mov    al, byte ptr [esi + 0fdh]
        cmp    byte ptr [esi + 0fch], al
        jne    SHORT loc_1_13A6
        push   esi
        call   sub_1_06E0
        mov    al, byte ptr [esi + 0fdh]
        cmp    byte ptr [esi + 0fch], al
        je     NEAR PTR loc_1_1454
loc_1_13A6:
        push   esi
        inc    dword ptr [esi + 34h]
        mov    dword ptr [esi + 169h], ebx
        call   sub_1_1A50
        test   al, al
        je     SHORT loc_1_140F
        xor    ecx, ecx
        xor    eax, eax
        mov    cl, byte ptr [esi + 1b6h]
        mov    al, byte ptr [esi + 1b5h]
        shl    ecx, 8
        add    ecx, eax
        mov    eax, dword ptr [esi + 15dh]
        sub    ecx, 4
        cmp    ecx, eax
        jb     SHORT loc_1_13DD
        mov    ecx, eax
loc_1_13DD:
        add    ecx, 0eh
        mov    eax, dword ptr [esi + 1b7h]
        push   ecx
        lea    edx, [esi + 1bbh]
        push   eax
        push   edx
        call   sub_1_04B0
        test   al, al
        je     SHORT loc_1_140F
        push   esi
        call   sub_1_1C00
        cmp    eax, 3
        je     NEAR PTR loc_1_1518
        inc    dword ptr [esi + 122h]
        jmp    SHORT loc_1_1414
loc_1_140F:
        mov    eax, 1
loc_1_1414:
        cmp    eax, 3
        je     NEAR PTR loc_1_1518
        cmp    eax, 1
        jne    SHORT loc_1_1430
        push   esi
        call   sub_1_06E0
        mov    al, byte ptr [esi + 0fdh]
        jmp    SHORT loc_1_1436
loc_1_1430:
        mov    al, byte ptr [esi + 1b4h]
loc_1_1436:
        push   esi
        mov    byte ptr [esi + 0fch], al
        call   sub_1_0730
        cmp    dword ptr [esi + 34h], 0ah
        jbe    NEAR PTR loc_1_1367
        xor    eax, eax
        mov    byte ptr [ebp - 1], al
        mov    dword ptr [esi + 34h], eax
loc_1_1454:
        cmp    byte ptr [esi + 118h], 0
        je     SHORT loc_1_1495
        mov    byte ptr [esi + 118h], 0
        push   esi
        call   sub_1_0810
        push   0
        push   0dh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        cmp    byte ptr [esi + 5ch], 0
        je     SHORT loc_1_1495
        cmp    dword ptr [esi + 57h], -1
        je     SHORT loc_1_1495
        mov    byte ptr [esi + 5ch], 0
        mov    byte ptr [esi + 1b1h], 0
        mov    byte ptr [esi + 5bh], 1
        push   esi
        call   sub_1_0670
loc_1_1495:
        cmp    dword ptr [esi + 161h], 0
        je     SHORT loc_1_14E6
loc_1_149E:
        cmp    byte ptr [esi + 13eh], 0
        jne    SHORT loc_1_14EF
        mov    edi, dword ptr [esi + 161h]
        push   edi
        push   esi
        mov    eax, dword ptr [edi + 24h]
        mov    dword ptr [esi + 161h], eax
        mov    dword ptr [esi + 169h], edi
        inc    dword ptr [esi + 11eh]
        mov    ebx, dword ptr [edi + 28h]
        call   sub_1_18B0
        push   0
        mov    ecx, dword ptr [edi + 28h]
        push   edi
        mov    eax, dword ptr [ecx]
        push   eax
        call   sub_1_3C0A
        dec    dword ptr [ebx + 1ch]
        cmp    dword ptr [esi + 161h], 0
        jne    SHORT loc_1_149E
loc_1_14E6:
        cmp    byte ptr [esi + 13eh], 0
        je     SHORT loc_1_152A
loc_1_14EF:
        mov    al, byte ptr [ebp - 1]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_14FB:
        push   esi
        call   sub_1_06E0
        mov    al, byte ptr [esi + 0fdh]
        push   esi
        mov    byte ptr [esi + 0fch], al
        call   sub_1_0730
        jmp    NEAR PTR loc_1_1454
loc_1_1518:
        mov    byte ptr [esi + 15ch], 1ah
        push   esi
        call   sub_1_01C0
        jmp    NEAR PTR loc_1_1454
loc_1_152A:
        cmp    byte ptr [esi + 11ah], 0
        je     SHORT loc_1_1546
        mov    eax, dword ptr [esi + 14bh]
        push   eax
        call   sub_1_3A50
        mov    byte ptr [esi + 11ah], 0
loc_1_1546:
        mov    al, byte ptr [ebp - 1]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1560:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [esi + 11bh], 1
        cmp    dword ptr [esi + 57h], -1
        jne    SHORT loc_1_159E
        push   4
        mov    eax, dword ptr [esi + 4]
        mov    byte ptr [esi + 11bh], 0
        push   1
        push   0aa000006h
        push   eax
        call   sub_1_3D98
        add    esp, 10h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_159E:
        push   esi
        call   sub_1_0710
        test   byte ptr [esi + 0feh], 1
        je     SHORT loc_1_15B5
        inc    dword ptr [esi + 11eh]
        jmp    SHORT loc_1_15BB
loc_1_15B5:
        inc    dword ptr [esi + 126h]
loc_1_15BB:
        mov    ecx, dword ptr [esi + 57h]
        mov    eax, dword ptr [esi + ecx*4 + 0bdh]
        lea    edx, [esi + ecx*4]
        add    eax, 0ffh
        shr    eax, 8
        mov    dword ptr [ebp - 4], eax
        add    eax, ecx
        cmp    eax, ecx
        jbe    SHORT loc_1_15F0
        add    edx, 5dh
loc_1_15DC:
        mov    dword ptr [edx], 0
        add    edx, 4
        inc    ecx
        mov    eax, dword ptr [esi + 57h]
        add    eax, dword ptr [ebp - 4]
        cmp    eax, ecx
        ja     SHORT loc_1_15DC
loc_1_15F0:
        mov    eax, dword ptr [esi + 53h]
        add    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 53h], eax
        cmp    eax, 0ch
        jne    SHORT loc_1_1605
        mov    dword ptr [esi + 53h], 0
loc_1_1605:
        mov    eax, dword ptr [esi + 53h]
        cmp    dword ptr [esi + eax*4 + 5dh], 0
        jne    SHORT loc_1_161B
        cmp    dword ptr [esi + 4fh], eax
        je     SHORT loc_1_161B
        mov    dword ptr [esi + 53h], 0
loc_1_161B:
        mov    eax, dword ptr [esi + 57h]
        mov    dword ptr [esi + eax*4 + 8dh], 0
        mov    ecx, dword ptr [esi + 53h]
        lea    eax, [esi + ecx*4 + 5dh]
        mov    edx, dword ptr [eax]
        test   edx, edx
        je     SHORT loc_1_1658
        cmp    edx, 1
        je     NEAR PTR loc_1_1839
        cmp    edx, 2
        je     NEAR PTR loc_1_1856
        mov    byte ptr [esi + 11bh], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1658:
        cmp    byte ptr [esi + 13eh], 0
        je     SHORT loc_1_1680
        mov    dword ptr [esi + 57h], 0ffffffffh
        mov    byte ptr [esi + 11bh], 0
        push   2
        push   esi
        call   sub_1_33A0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1680:
        mov    ebx, dword ptr [esi + 47h]
        test   ebx, ebx
        je     NEAR PTR loc_1_1822
        lea    edi, [ebx + 24h]
        lea    ecx, [ebp - 4]
        push   ecx
        push   0
        mov    eax, dword ptr [edi]
        push   0
        push   0
        mov    dword ptr [esi + 47h], eax
        push   ebx
        call   sub_1_3CF0
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0ffh
        shr    eax, 8
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [esi + 4fh]
        add    eax, dword ptr [ebp - 4]
        cmp    eax, 0ch
        jbe    SHORT loc_1_16C3
        mov    dword ptr [esi + 4fh], 0
loc_1_16C3:
        mov    ecx, dword ptr [esi + 4fh]
        mov    eax, dword ptr [ebp - 4]
        add    eax, ecx
        mov    dword ptr [esi + 53h], ecx
        cmp    eax, ecx
        jbe    SHORT loc_1_16EA
        lea    eax, [esi + ecx*4 + 5dh]
loc_1_16D6:
        mov    dword ptr [eax], 2
        add    eax, 4
        inc    ecx
        mov    edx, dword ptr [esi + 4fh]
        add    edx, dword ptr [ebp - 4]
        cmp    edx, ecx
        ja     SHORT loc_1_16D6
loc_1_16EA:
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
        call   sub_1_0230
        test   al, al
        jne    SHORT loc_1_1763
        push   0c0000001h
        mov    eax, dword ptr [ebx + 28h]
        push   ebx
        mov    ecx, dword ptr [eax]
        push   ecx
        call   sub_1_3C0A
        mov    edx, dword ptr [esi + 4fh]
        mov    eax, dword ptr [ebp - 4]
        add    eax, edx
        cmp    eax, edx
        jbe    SHORT loc_1_1746
        lea    eax, [esi + edx*4 + 5dh]
loc_1_1732:
        mov    dword ptr [eax], 0
        add    eax, 4
        inc    edx
        mov    ecx, dword ptr [esi + 4fh]
        add    ecx, dword ptr [ebp - 4]
        cmp    ecx, edx
        ja     SHORT loc_1_1732
loc_1_1746:
        mov    eax, dword ptr [esi + 4fh]
        push   esi
        mov    dword ptr [esi + eax*4 + 8dh], 0
        call   sub_1_2140
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1763:
        mov    ecx, dword ptr [esi + 4fh]
        mov    eax, dword ptr [esi + ecx*4 + 0bdh]
        cmp    eax, 3ch
        jae    SHORT loc_1_1792
        shl    ecx, 8
        mov    edx, 3ch
        sub    edx, eax
        push   edx
        db  068h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [esi + 0edh]
        add    edx, ecx
        add    edx, eax
        push   edx
        call   sub_1_03D0
loc_1_1792:
        mov    eax, dword ptr [esi + 4fh]
        add    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 4fh], eax
        cmp    eax, 0ch
        jne    SHORT loc_1_17A7
        mov    dword ptr [esi + 4fh], 0
loc_1_17A7:
        mov    byte ptr [esi + 11bh], 0
        cmp    byte ptr [esi + 118h], 0
        je     SHORT loc_1_17BD
        mov    byte ptr [esi + 5ch], 1
        jmp    SHORT loc_1_17C7
loc_1_17BD:
        mov    byte ptr [esi + 5bh], 1
        push   esi
        call   sub_1_0670
loc_1_17C7:
        cmp    byte ptr [edi + 8], 0
        jne    SHORT loc_1_17ED
        mov    edi, dword ptr [edi + 4]
        push   0
        push   ebx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_3C0A
        push   esi
        dec    dword ptr [edi + 1ch]
        call   sub_1_2140
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_17ED:
        cmp    dword ptr [esi + 161h], 0
        jne    SHORT loc_1_17FE
        mov    dword ptr [esi + 161h], ebx
        jmp    SHORT loc_1_1807
loc_1_17FE:
        mov    eax, dword ptr [esi + 165h]
        mov    dword ptr [eax + 24h], ebx
loc_1_1807:
        push   esi
        mov    dword ptr [esi + 165h], ebx
        mov    dword ptr [edi], 0
        call   sub_1_2140
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1822:
        mov    dword ptr [esi + 57h], 0ffffffffh
        mov    byte ptr [esi + 11bh], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1839:
        mov    dword ptr [esi + 57h], 0ffffffffh
        mov    byte ptr [esi + 11bh], 0
        push   esi
        call   sub_1_2140
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1856:
        cmp    byte ptr [esi + 13eh], 0
        je     SHORT loc_1_1884
        mov    dword ptr [esi + 57h], 0ffffffffh
        push   2
        mov    dword ptr [eax], 0
        mov    byte ptr [esi + 11bh], 0
        push   esi
        call   sub_1_33A0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1884:
        push   esi
        mov    dword ptr [esi + 57h], ecx
        mov    byte ptr [esi + 11bh], 0
        mov    byte ptr [esi + 5bh], 1
        call   sub_1_0670
        push   esi
        call   sub_1_2140
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_18B0:
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
        call   sub_1_3CF0
        cmp    dword ptr [ebp - 4], 6
        jae    SHORT loc_1_18E0
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_18E0:
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [esi + 15dh]
        add    edi, 0eh
        cmp    edi, eax
        jb     SHORT loc_1_18F5
        mov    edi, eax
loc_1_18F5:
        push   edi
        lea    eax, [esi + 1bbh]
        push   0
        push   ebx
        push   eax
        call   sub_1_1970
        cmp    edi, 0eh
        jae    SHORT loc_1_1928
        cmp    edi, 6
        jb     SHORT loc_1_1951
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
        jmp    SHORT loc_1_194C
loc_1_1928:
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
loc_1_194C:
        call   sub_1_3870
loc_1_1951:
        xor    eax, eax
        pop    edi
        mov    byte ptr [esi + 11ah], 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1970:
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
        call   sub_1_3CF0
        mov    ebx, dword ptr [ebp + 10h]
        cmp    dword ptr [ebp - 4], esi
        je     SHORT loc_1_19C4
loc_1_1995:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_3D14
        mov    eax, dword ptr [ebp - 8]
        add    eax, edi
        cmp    ebx, eax
        jb     SHORT loc_1_19C4
        mov    edi, eax
        mov    ecx, dword ptr [ebp - 4]
        lea    eax, [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_3D38
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_1995
loc_1_19C4:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_19D5
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_19D5:
        mov    eax, ebx
        sub    eax, edi
        sub    edi, ebx
        add    dword ptr [ebp - 0ch], eax
        add    dword ptr [ebp - 8], edi
        mov    edi, dword ptr [ebp + 14h]
loc_1_19E4:
        mov    eax, dword ptr [ebp - 8]
        add    eax, esi
        cmp    eax, edi
        jbe    SHORT loc_1_19F3
        mov    ebx, edi
        sub    ebx, esi
        jmp    SHORT loc_1_19F6
loc_1_19F3:
        mov    ebx, dword ptr [ebp - 8]
loc_1_19F6:
        push   ebx
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, esi
        add    esi, ebx
        push   eax
        call   sub_1_3DAC
        cmp    esi, edi
        je     SHORT loc_1_1A32
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_3D38
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_1A3D
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_3D14
        jmp    SHORT loc_1_19E4
loc_1_1A32:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_1A3D:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_1A50:
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
        call   sub_1_04B0
        test   al, al
        jne    SHORT loc_1_1A95
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1A95:
        xor    ecx, ecx
        lea    edx, [ebp - 18h]
        add    dword ptr [ebp - 0ch], 4
loc_1_1A9E:
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
        jne    SHORT loc_1_1B30
        test   byte ptr [edx], 20h
        je     SHORT loc_1_1ACC
        test   ecx, ecx
        jne    SHORT loc_1_1ACC
        test   byte ptr [edi], 1
        je     SHORT loc_1_1B30
loc_1_1ACC:
        mov    edi, dword ptr [ebp - 10h]
        cmp    byte ptr [edi], bl
        jne    SHORT loc_1_1AEE
        mov    edi, dword ptr [ebp - 8]
        mov    byte ptr [ebp - 2], bl
        mov    bl, byte ptr [edi]
        mov    edi, dword ptr [ebp - 10h]
        sub    bl, byte ptr [esi + 0fch]
        dec    bl
        mov    byte ptr [eax], bl
        cmp    byte ptr [edi], 0fch
        jbe    SHORT loc_1_1AEE
        inc    eax
loc_1_1AEE:
        xor    ebx, ebx
        mov    edi, dword ptr [ebp - 10h]
        mov    bl, byte ptr [eax]
        shl    ebx, 8
        movzx  edi, byte ptr [edi]
        lea    edi, [ebx + edi - 4]
        cmp    edi, 5eah
        ja     SHORT loc_1_1B0C
        cmp    edi, 3ch
        jae    SHORT loc_1_1B15
loc_1_1B0C:
        mov    byte ptr [ebp - 1], 1
        mov    bl, byte ptr [ebp - 2]
        mov    byte ptr [eax], bl
loc_1_1B15:
        cmp    byte ptr [ebp - 1], 0
        jne    SHORT loc_1_1B3A
        mov    edi, dword ptr [ebp - 8]
        mov    al, byte ptr [edi]
        cmp    byte ptr [esi + 0fah], al
        ja     SHORT loc_1_1B30
        cmp    byte ptr [esi + 0fbh], al
        jae    SHORT loc_1_1B34
loc_1_1B30:
        mov    byte ptr [ebp - 1], 1
loc_1_1B34:
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_1B6C
loc_1_1B3A:
        test   ecx, ecx
        je     SHORT loc_1_1B45
        cmp    ecx, 1
        je     SHORT loc_1_1B5A
        jmp    SHORT loc_1_1B6B
loc_1_1B45:
        mov    edx, dword ptr [ebp - 8]
        inc    dword ptr [ebp - 0ch]
        cmp    byte ptr [esi + 11ch], 0
        jne    SHORT loc_1_1B6B
        inc    edx
        inc    dword ptr [ebp - 0ch]
        jmp    SHORT loc_1_1B6B
loc_1_1B5A:
        dec    edx
        dec    dword ptr [ebp - 0ch]
        cmp    byte ptr [esi + 11ch], 0
        jne    SHORT loc_1_1B6B
        dec    edx
        dec    dword ptr [ebp - 0ch]
loc_1_1B6B:
        inc    ecx
loc_1_1B6C:
        cmp    ecx, 2
        jae    SHORT loc_1_1B7B
        cmp    byte ptr [ebp - 1], 0
        jne    NEAR PTR loc_1_1A9E
loc_1_1B7B:
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
        ja     SHORT loc_1_1BD2
        cmp    eax, 3ch
        jb     SHORT loc_1_1BD2
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1BD2:
        cmp    byte ptr [esi + 0fah], cl
        ja     SHORT loc_1_1BED
        cmp    byte ptr [esi + 0fbh], cl
        jb     SHORT loc_1_1BED
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1BED:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_1C00:
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
        jb     SHORT loc_1_1C2A
        sub    dl, cl
        add    dl, byte ptr [esi + 0fah]
loc_1_1C2A:
        mov    al, byte ptr [esi + 1b4h]
        cmp    dl, al
        je     SHORT loc_1_1C4F
        inc    dl
        cmp    dl, cl
        jne    SHORT loc_1_1C40
        mov    dl, byte ptr [esi + 0fah]
loc_1_1C40:
        cmp    dl, al
        je     SHORT loc_1_1C4F
        mov    eax, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_1C4F:
        cmp    byte ptr [esi + 0fah], al
        ja     NEAR PTR loc_1_1CDC
        cmp    al, cl
        ja     SHORT loc_1_1CDC
        xor    ecx, ecx
        xor    eax, eax
        mov    cl, byte ptr [esi + 1b5h]
        mov    al, bl
        shl    eax, 8
        lea    eax, [ecx + eax - 4]
        cmp    eax, 5eah
        jbe    SHORT loc_1_1C84
        mov    eax, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_1C84:
        mov    ecx, dword ptr [esi + 15dh]
        add    ecx, 0eh
        cmp    ecx, eax
        jb     SHORT loc_1_1C93
        mov    ecx, eax
loc_1_1C93:
        mov    dword ptr [esi + 2b7h], eax
        cmp    ecx, 0eh
        jae    SHORT loc_1_1CA7
        push   0
        push   0
        push   0
        push   ecx
        jmp    SHORT loc_1_1CB8
loc_1_1CA7:
        sub    eax, 0eh
        sub    ecx, 0eh
        push   eax
        lea    edx, [esi + 1c9h]
        push   ecx
        push   edx
        push   0eh
loc_1_1CB8:
        lea    eax, [esi + 1bbh]
        mov    edx, dword ptr [esi + 14bh]
        push   eax
        push   eax
        push   esi
        push   edx
        call   sub_1_3870
        mov    byte ptr [esi + 11ah], 1
        xor    eax, eax
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_1CDC:
        mov    eax, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1CF0:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        call   sub_1_0EA0
        mov    edi, dword ptr [ebp + 10h]
        mov    esi, dword ptr [ebp + 0ch]
        add    edi, 0eh
        cmp    dword ptr [esi + 169h], 0
        je     NEAR PTR loc_1_1DA1
        push   0
        lea    eax, [ebp - 8]
        push   eax
        mov    ecx, dword ptr [ebp + 18h]
        push   0
        mov    ebx, edi
        push   0
        push   ecx
        call   sub_1_3CF0
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_1D6F
loc_1_1D2F:
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 18h]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_3D14
        mov    ecx, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [esi + 169h]
        push   ecx
        mov    eax, dword ptr [ebp - 18h]
        push   ebx
        push   edx
        push   eax
        call   sub_1_1970
        add    ebx, eax
        cmp    eax, dword ptr [ebp - 0ch]
        jb     SHORT loc_1_1D6F
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_3D38
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_1D2F
loc_1_1D6F:
        call   sub_1_0ED0
        cmp    edi, ebx
        jbe    SHORT loc_1_1D8F
        mov    ecx, dword ptr [ebp + 1ch]
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [ecx], 0
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_1D8F:
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
loc_1_1DA1:
        mov    ecx, dword ptr [esi + 2b7h]
        mov    eax, dword ptr [ebp + 14h]
        lea    edx, [edi + eax]
        cmp    edx, ecx
        jbe    SHORT loc_1_1DD8
        cmp    edi, ecx
        jbe    SHORT loc_1_1DD1
        mov    ecx, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx], 0
        call   sub_1_0ED0
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_1DD1:
        sub    ecx, edi
        mov    dword ptr [ebp - 1ch], ecx
        jmp    SHORT loc_1_1DDB
loc_1_1DD8:
        mov    dword ptr [ebp - 1ch], eax
loc_1_1DDB:
        mov    eax, dword ptr [ebp - 1ch]
        add    edi, dword ptr [esi + 1b7h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [esi + 0f5h]
        cmp    edi, eax
        jbe    SHORT loc_1_1DF9
        sub    edi, eax
        add    edi, dword ptr [esi + 0f1h]
loc_1_1DF9:
        push   0
        lea    eax, [ebp - 8]
        push   eax
        mov    ecx, dword ptr [ebp + 18h]
        push   0
        push   0
        push   ecx
        call   sub_1_3CF0
        lea    ecx, [ebp - 0ch]
        lea    edx, [ebp - 14h]
        push   ecx
        mov    eax, dword ptr [ebp - 8]
        push   edx
        push   eax
        call   sub_1_3D14
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [ebp - 1ch], 0
        je     NEAR PTR loc_1_1EBA
loc_1_1E2E:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ebx, dword ptr [ebp - 10h]
        sub    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 10h], eax
        jb     SHORT loc_1_1E3E
        mov    ebx, eax
loc_1_1E3E:
        mov    eax, dword ptr [esi + 0f5h]
        lea    ecx, [ebx + edi]
        cmp    ecx, eax
        jbe    SHORT loc_1_1E4F
        mov    ebx, eax
        sub    ebx, edi
loc_1_1E4F:
        push   ebx
        mov    eax, dword ptr [ebp - 14h]
        push   edi
        add    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_04B0
        test   al, al
        je     NEAR PTR loc_1_1F2D
        add    edi, ebx
        sub    dword ptr [ebp - 10h], ebx
        je     SHORT loc_1_1EBA
        cmp    dword ptr [esi + 0f5h], edi
        jne    SHORT loc_1_1E7A
        mov    edi, dword ptr [esi + 0f1h]
loc_1_1E7A:
        add    dword ptr [ebp - 4], ebx
        mov    eax, dword ptr [ebp - 4]
        cmp    eax, dword ptr [ebp - 0ch]
        jne    SHORT loc_1_1EB0
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_3D38
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_1EBA
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 14h]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_3D14
        mov    dword ptr [ebp - 4], 0
loc_1_1EB0:
        cmp    dword ptr [ebp - 10h], 0
        jne    NEAR PTR loc_1_1E2E
loc_1_1EBA:
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp + 1ch]
        sub    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ecx], eax
        cmp    byte ptr [esi + 118h], 0
        jne    NEAR PTR loc_1_1FF5
        cmp    dword ptr [esi + 57h], -1
        je     NEAR PTR loc_1_1FF5
        lea    eax, [ebp - 4]
        push   eax
        push   7
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        test   byte ptr [ebp - 4], 8
        je     SHORT loc_1_1EF4
        push   esi
        call   sub_1_24A0
loc_1_1EF4:
        test   byte ptr [ebp - 4], 2
        je     NEAR PTR loc_1_1FF5
        push   2
        push   7
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   esi
        call   sub_1_0710
        mov    byte ptr [esi + 5bh], 0
        mov    dword ptr [esi + 147h], 0
        test   byte ptr [esi + 0feh], 1
        je     SHORT loc_1_1F60
        inc    dword ptr [esi + 11eh]
        jmp    SHORT loc_1_1F66
loc_1_1F2D:
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp + 1ch]
        sub    eax, dword ptr [ebp - 10h]
        push   2
        push   1
        mov    dword ptr [ecx], eax
        push   0aa000003h
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_1_3D98
        add    esp, 10h
        call   sub_1_0ED0
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_1F60:
        inc    dword ptr [esi + 126h]
loc_1_1F66:
        mov    edi, dword ptr [esi + 57h]
        mov    edx, dword ptr [esi + edi*4 + 0bdh]
        lea    ecx, [esi + edi*4]
        add    edx, 0ffh
        shr    edx, 8
        lea    eax, [edi + edx]
        cmp    eax, edi
        jbe    SHORT loc_1_1F99
        add    ecx, 5dh
loc_1_1F86:
        mov    dword ptr [ecx], 0
        add    ecx, 4
        inc    edi
        mov    eax, dword ptr [esi + 57h]
        add    eax, edx
        cmp    eax, edi
        ja     SHORT loc_1_1F86
loc_1_1F99:
        mov    eax, dword ptr [esi + 53h]
        add    eax, edx
        mov    dword ptr [esi + 53h], eax
        cmp    eax, 0ch
        jne    SHORT loc_1_1FAD
        mov    dword ptr [esi + 53h], 0
loc_1_1FAD:
        mov    eax, dword ptr [esi + 53h]
        cmp    dword ptr [esi + eax*4 + 5dh], 0
        jne    SHORT loc_1_1FC3
        cmp    dword ptr [esi + 4fh], eax
        je     SHORT loc_1_1FC3
        mov    dword ptr [esi + 53h], 0
loc_1_1FC3:
        mov    eax, dword ptr [esi + 57h]
        xor    ecx, ecx
        mov    dword ptr [esi + eax*4 + 8dh], ecx
        mov    byte ptr [esi + 1b1h], cl
        mov    eax, dword ptr [esi + 53h]
        cmp    dword ptr [esi + eax*4 + 5dh], 2
        jne    SHORT loc_1_1FEE
        push   esi
        mov    dword ptr [esi + 57h], eax
        mov    byte ptr [esi + 5bh], 1
        call   sub_1_0670
        jmp    SHORT loc_1_1FF5
loc_1_1FEE:
        mov    dword ptr [esi + 57h], 0ffffffffh
loc_1_1FF5:
        call   sub_1_0ED0
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2010:
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
        call   sub_1_0EA0
        cmp    byte ptr [ebx + 20h], 0
        je     SHORT loc_1_2043
        call   sub_1_0ED0
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_2043:
        cmp    byte ptr [esi + 13eh], 0
        je     SHORT loc_1_205F
        call   sub_1_0ED0
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_205F:
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
        call   sub_1_3CF0
        lea    ecx, [ebp - 0ch]
        lea    edx, [ebp - 4]
        push   ecx
        mov    eax, dword ptr [ebp - 8]
        push   edx
        push   eax
        call   sub_1_3D14
        test   byte ptr [ebx + 71h], 2
        je     SHORT loc_1_2098
        mov    byte ptr [edi + 8], 0
        jmp    SHORT loc_1_20AB
loc_1_2098:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 14bh]
        push   eax
        push   ecx
        call   sub_1_3AF0
        mov    byte ptr [edi + 8], al
loc_1_20AB:
        cmp    dword ptr [esi + 47h], 0
        je     SHORT loc_1_20BC
        mov    eax, dword ptr [esi + 4bh]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 24h], ecx
        jmp    SHORT loc_1_20C2
loc_1_20BC:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 47h], ecx
loc_1_20C2:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 4bh], ecx
        mov    dword ptr [edi], 0
        mov    eax, dword ptr [esi + 30h]
        dec    eax
        mov    dword ptr [esi + 30h], eax
        jne    SHORT loc_1_20FF
        cmp    byte ptr [esi + 13eh], 0
        jne    SHORT loc_1_20EF
        cmp    dword ptr [esi + 16dh], 0
        jne    SHORT loc_1_20EF
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_1_20FF
loc_1_20EF:
        push   1
        add    esi, 179h
        push   esi
        call   sub_1_3CA2
        jmp    SHORT loc_1_211E
loc_1_20FF:
        cmp    dword ptr [esi + 47h], 0
        je     SHORT loc_1_211E
        mov    eax, dword ptr [esi + 4fh]
        cmp    dword ptr [esi + eax*4 + 5dh], 0
        jne    SHORT loc_1_211E
        cmp    byte ptr [esi + 11dh], 0
        jne    SHORT loc_1_211E
        push   esi
        call   sub_1_2140
loc_1_211E:
        call   sub_1_0ED0
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2140:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        call   sub_1_0EA0
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 47h], 0
        je     NEAR PTR loc_1_239A
loc_1_215B:
        mov    eax, dword ptr [esi + 4fh]
        cmp    dword ptr [esi + eax*4 + 5dh], 0
        jne    NEAR PTR loc_1_239A
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [esi + 47h]
        push   eax
        push   0
        push   0
        push   0
        push   ecx
        call   sub_1_3CF0
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 57h]
        add    eax, 0ffh
        shr    eax, 8
        cmp    ecx, -1
        mov    dword ptr [ebp - 4], eax
        jne    SHORT loc_1_21EF
        mov    ecx, dword ptr [esi + 53h]
        cmp    dword ptr [esi + ecx*4 + 5dh], 0
        jne    SHORT loc_1_21B0
        mov    eax, dword ptr [esi + 4fh]
        add    eax, dword ptr [ebp - 4]
        cmp    eax, 0ch
        jbe    SHORT loc_1_2224
        mov    dword ptr [esi + 4fh], 0
        jmp    SHORT loc_1_2224
loc_1_21B0:
        mov    edx, dword ptr [esi + 4fh]
        mov    eax, dword ptr [ebp - 4]
        cmp    edx, ecx
        jae    SHORT loc_1_21CE
        add    eax, edx
        cmp    eax, ecx
        jbe    SHORT loc_1_2224
        call   sub_1_0ED0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_21CE:
        add    eax, edx
        cmp    eax, 0ch
        jbe    SHORT loc_1_2224
        mov    dword ptr [esi + 4fh], 0
        cmp    dword ptr [ebp - 4], ecx
        jbe    SHORT loc_1_2224
        call   sub_1_0ED0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_21EF:
        mov    edx, dword ptr [esi + 4fh]
        mov    eax, dword ptr [ebp - 4]
        cmp    ecx, edx
        jbe    SHORT loc_1_220D
        add    eax, edx
        cmp    eax, ecx
        jbe    SHORT loc_1_2224
        call   sub_1_0ED0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_220D:
        add    eax, edx
        cmp    eax, 0ch
        jbe    SHORT loc_1_2224
        mov    dword ptr [esi + 4fh], 0
        cmp    dword ptr [ebp - 4], ecx
        ja     NEAR PTR loc_1_23A8
loc_1_2224:
        mov    ecx, dword ptr [esi + 4fh]
        mov    ebx, dword ptr [esi + 47h]
        mov    dword ptr [ebp - 8], ecx
        mov    eax, dword ptr [ebx + 24h]
        mov    dword ptr [esi + 47h], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, ecx
        cmp    ecx, eax
        jae    SHORT loc_1_224A
        lea    edi, [esi + ecx*4 + 5dh]
        mov    eax, 1
        mov    ecx, dword ptr [ebp - 4]
        rep stosd
loc_1_224A:
        mov    ecx, dword ptr [ebp - 8]
        lea    edi, [esi + ecx*4 + 0bdh]
        mov    dword ptr [esi + ecx*4 + 8dh], ebx
        lea    eax, [esi + ecx*4]
        push   edi
        push   ebx
        mov    dword ptr [ebp - 0ch], eax
        push   esi
        call   sub_1_0230
        test   al, al
        jne    SHORT loc_1_22AE
        push   0c0000001h
        mov    eax, dword ptr [ebx + 28h]
        push   ebx
        mov    ecx, dword ptr [eax]
        push   ecx
        call   sub_1_3C0A
        mov    eax, dword ptr [ebp - 4]
        add    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 8], eax
        jae    SHORT loc_1_2296
        mov    edi, dword ptr [ebp - 0ch]
        xor    eax, eax
        add    edi, 5dh
        mov    ecx, dword ptr [ebp - 4]
        rep stosd
loc_1_2296:
        mov    eax, dword ptr [ebx + 28h]
        mov    ecx, dword ptr [ebp - 0ch]
        dec    dword ptr [eax + 1ch]
        mov    dword ptr [ecx + 8dh], 0
        jmp    NEAR PTR loc_1_2390
loc_1_22AE:
        mov    edx, dword ptr [edi]
        cmp    edx, 3ch
        jae    SHORT loc_1_22D8
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
        call   sub_1_03D0
loc_1_22D8:
        cmp    byte ptr [esi + 13eh], 0
        jne    NEAR PTR loc_1_23B6
        mov    eax, dword ptr [ebp - 4]
        add    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 8], eax
        jae    SHORT loc_1_2300
        mov    edi, dword ptr [ebp - 0ch]
        mov    eax, 2
        add    edi, 5dh
        mov    ecx, dword ptr [ebp - 4]
        rep stosd
loc_1_2300:
        mov    eax, dword ptr [esi + 4fh]
        add    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 4fh], eax
        cmp    eax, 0ch
        jne    SHORT loc_1_2315
        mov    dword ptr [esi + 4fh], 0
loc_1_2315:
        cmp    dword ptr [esi + 57h], -1
        jne    SHORT loc_1_2350
        mov    eax, dword ptr [esi + 53h]
        cmp    dword ptr [esi + eax*4 + 5dh], 0
        jne    SHORT loc_1_2331
        cmp    dword ptr [esi + 4fh], eax
        je     SHORT loc_1_2331
        mov    dword ptr [esi + 53h], 0
loc_1_2331:
        cmp    byte ptr [esi + 118h], 0
        mov    eax, dword ptr [esi + 53h]
        mov    dword ptr [esi + 57h], eax
        je     SHORT loc_1_2346
        mov    byte ptr [esi + 5ch], 1
        jmp    SHORT loc_1_2350
loc_1_2346:
        mov    byte ptr [esi + 5bh], 1
        push   esi
        call   sub_1_0670
loc_1_2350:
        cmp    byte ptr [ebx + 2ch], 0
        jne    SHORT loc_1_2369
        mov    edi, dword ptr [ebx + 28h]
        push   0
        push   ebx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_3C0A
        dec    dword ptr [edi + 1ch]
        jmp    SHORT loc_1_2390
loc_1_2369:
        cmp    dword ptr [esi + 161h], 0
        jne    SHORT loc_1_237A
        mov    dword ptr [esi + 161h], ebx
        jmp    SHORT loc_1_2383
loc_1_237A:
        mov    eax, dword ptr [esi + 165h]
        mov    dword ptr [eax + 24h], ebx
loc_1_2383:
        mov    dword ptr [esi + 165h], ebx
        mov    dword ptr [ebx + 24h], 0
loc_1_2390:
        cmp    dword ptr [esi + 47h], 0
        jne    NEAR PTR loc_1_215B
loc_1_239A:
        call   sub_1_0ED0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_23A8:
        call   sub_1_0ED0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_23B6:
        mov    eax, dword ptr [ebx + 28h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 8], eax
        jae    SHORT loc_1_23D5
        mov    ecx, dword ptr [ebp - 8]
        xor    eax, eax
        lea    edi, [esi + ecx*4 + 5dh]
        mov    ecx, dword ptr [ebp - 4]
        rep stosd
loc_1_23D5:
        push   0
        mov    eax, dword ptr [ebx + 28h]
        push   ebx
        mov    ecx, dword ptr [eax]
        push   ecx
        call   sub_1_3C0A
        push   3
        push   esi
        call   sub_1_33A0
        mov    ecx, dword ptr [ebp - 0ch]
        dec    dword ptr [ecx + 1ch]
        call   sub_1_0ED0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_2400:
        push   ebp
        mov    ebp, esp
        push   esi
        pushf
        cli
        call   sub_1_0EA0
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 1b1h], 0
        je     SHORT loc_1_2467
        cmp    dword ptr [esi + 57h], -1
        je     SHORT loc_1_2474
        mov    byte ptr [esi + 1b1h], 0
        push   3
        mov    eax, dword ptr [esi + 4]
        push   1
        push   0aa000003h
        push   eax
        call   sub_1_3D98
        add    esp, 10h
        push   esi
        call   sub_1_0150
        cmp    dword ptr [esi + 147h], 4
        jbe    SHORT loc_1_2459
        push   esi
        call   sub_1_01C0
        mov    dword ptr [esi + 147h], 0
loc_1_2459:
        push   esi
        call   sub_1_24A0
        inc    dword ptr [esi + 147h]
        jmp    SHORT loc_1_2474
loc_1_2467:
        cmp    dword ptr [esi + 57h], -1
        je     SHORT loc_1_2474
        mov    byte ptr [esi + 1b1h], 1
loc_1_2474:
        push   7d0h
        add    esi, 1a5h
        push   esi
        call   sub_1_3CA2
        call   sub_1_0ED0
        popf
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_24A0:
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
        call   sub_1_0660
loc_1_24BD:
        push   64h
        call   sub_1_3CC0
        dec    edi
        jne    SHORT loc_1_24BD
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
        je     SHORT loc_1_24F5
        mov    byte ptr [esi + 5bh], 1
        push   esi
        call   sub_1_0670
loc_1_24F5:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_2500:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 28h]
        test   edx, edx
        je     SHORT loc_1_2520
loc_1_2510:
        mov    esi, dword ptr [edx + 18h]
        cmp    ecx, esi
        jae    SHORT loc_1_2519
        mov    ecx, esi
loc_1_2519:
        mov    edx, dword ptr [edx + 0ch]
        test   edx, edx
        jne    SHORT loc_1_2510
loc_1_2520:
        test   ecx, ecx
        jne    SHORT loc_1_2529
        mov    ecx, 0eeh
loc_1_2529:
        pop    esi
        mov    dword ptr [eax + 15dh], ecx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2540:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 20h], 0
        mov    esi, dword ptr [ebx + 8]
        je     SHORT loc_1_2563
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2563:
        call   sub_1_0EA0
        inc    dword ptr [esi + 30h]
        inc    dword ptr [ebx + 1ch]
        mov    ecx, dword ptr [esi + 28h]
        cmp    ecx, ebx
        jne    SHORT loc_1_257D
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [esi + 28h], eax
        jmp    SHORT loc_1_2590
loc_1_257D:
        cmp    dword ptr [ecx + 0ch], ebx
        je     SHORT loc_1_258A
loc_1_2582:
        mov    ecx, dword ptr [ecx + 0ch]
        cmp    dword ptr [ecx + 0ch], ebx
        jne    SHORT loc_1_2582
loc_1_258A:
        mov    edx, dword ptr [ebx + 0ch]
        mov    dword ptr [ecx + 0ch], edx
loc_1_2590:
        push   0
        mov    eax, dword ptr [ebx + 14h]
        push   eax
        mov    ecx, dword ptr [esi + 14bh]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_25A2:
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_25C7
        mov    eax, dword ptr [ebx + 1ch]
        cmp    eax, 2
        je     SHORT loc_1_25BF
        mov    byte ptr [ebx + 20h], 1
        sub    dword ptr [ebx + 1ch], 2
        mov    edi, 103h
        jmp    SHORT loc_1_25ED
loc_1_25BF:
        sub    eax, 2
        mov    dword ptr [ebx + 1ch], eax
        jmp    SHORT loc_1_25ED
loc_1_25C7:
        cmp    edi, 103h
        jne    SHORT loc_1_25D9
        mov    byte ptr [ebx + 20h], 1
        sub    dword ptr [ebx + 1ch], 2
        jmp    SHORT loc_1_25ED
loc_1_25D9:
        cmp    edi, 0c001000eh
        jne    SHORT loc_1_25EA
        mov    byte ptr [ebx + 20h], 1
        mov    edi, 103h
loc_1_25EA:
        dec    dword ptr [ebx + 1ch]
loc_1_25ED:
        cmp    dword ptr [ebx + 1ch], 0
        jne    SHORT loc_1_2622
        mov    eax, dword ptr [ebx + 18h]
        cmp    dword ptr [esi + 15dh], eax
        jne    SHORT loc_1_2604
        push   esi
        call   sub_1_2500
loc_1_2604:
        push   0
        push   75h
        push   ebx
        call   sub_1_3CCC
        cmp    dword ptr [esi + 28h], 0
        jne    SHORT loc_1_2630
        cmp    dword ptr [esi + 2ch], 0
        jne    SHORT loc_1_2630
        push   esi
        call   sub_1_0150
        jmp    SHORT loc_1_2630
loc_1_2622:
        mov    eax, dword ptr [esi + 2ch]
        mov    edi, 103h
        mov    dword ptr [ebx + 0ch], eax
        mov    dword ptr [esi + 2ch], ebx
loc_1_2630:
        mov    eax, dword ptr [esi + 30h]
        dec    eax
        mov    dword ptr [esi + 30h], eax
        jne    SHORT loc_1_2661
        cmp    byte ptr [esi + 13eh], 0
        jne    SHORT loc_1_2651
        cmp    dword ptr [esi + 16dh], 0
        jne    SHORT loc_1_2651
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_1_2661
loc_1_2651:
        push   1
        add    esi, 179h
        push   esi
        call   sub_1_3CA2
        jmp    SHORT loc_1_2680
loc_1_2661:
        cmp    dword ptr [esi + 47h], 0
        je     SHORT loc_1_2680
        mov    eax, dword ptr [esi + 4fh]
        cmp    dword ptr [esi + eax*4 + 5dh], 0
        jne    SHORT loc_1_2680
        cmp    byte ptr [esi + 11dh], 0
        jne    SHORT loc_1_2680
        push   esi
        call   sub_1_2140
loc_1_2680:
        call   sub_1_0ED0
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2690:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebx + 8]
        call   sub_1_0EA0
        inc    dword ptr [ebx + 1ch]
        inc    dword ptr [edi + 30h]
        cmp    byte ptr [ebx + 20h], 0
        je     SHORT loc_1_26B4
        mov    esi, 0c0010002h
        jmp    SHORT loc_1_26F0
loc_1_26B4:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ecx + 10h]
        test   eax, eax
        jne    SHORT loc_1_26CA
        push   ecx
        push   ebx
        push   edi
        call   sub_1_2AE0
        mov    esi, eax
        jmp    SHORT loc_1_26F0
loc_1_26CA:
        cmp    eax, 1
        jne    SHORT loc_1_26EB
        cmp    byte ptr [edi + 13eh], 0
        je     SHORT loc_1_26DF
        mov    esi, 0c001000dh
        jmp    SHORT loc_1_26F0
loc_1_26DF:
        push   ecx
        push   ebx
        push   edi
        call   sub_1_2B30
        mov    esi, eax
        jmp    SHORT loc_1_26F0
loc_1_26EB:
        mov    esi, 10001h
loc_1_26F0:
        dec    dword ptr [ebx + 1ch]
        mov    eax, dword ptr [edi + 30h]
        dec    eax
        mov    dword ptr [edi + 30h], eax
        jne    SHORT loc_1_2724
        cmp    byte ptr [edi + 13eh], 0
        jne    SHORT loc_1_2714
        cmp    dword ptr [edi + 16dh], 0
        jne    SHORT loc_1_2714
        cmp    dword ptr [edi + 2ch], 0
        je     SHORT loc_1_2724
loc_1_2714:
        push   1
        add    edi, 179h
        push   edi
        call   sub_1_3CA2
        jmp    SHORT loc_1_2743
loc_1_2724:
        cmp    dword ptr [edi + 47h], 0
        je     SHORT loc_1_2743
        mov    eax, dword ptr [edi + 4fh]
        cmp    dword ptr [edi + eax*4 + 5dh], 0
        jne    SHORT loc_1_2743
        cmp    byte ptr [edi + 11dh], 0
        jne    SHORT loc_1_2743
        push   edi
        call   sub_1_2140
loc_1_2743:
        call   sub_1_0ED0
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2760:
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
        ja     SHORT loc_1_279D
        je     NEAR PTR loc_1_292F
        sub    eax, 10101h
        cmp    eax, 12h
        ja     SHORT loc_1_27BE
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_279D:
        cmp    eax, 1010102h
        je     NEAR PTR loc_1_2951
        cmp    eax, 1010103h
        je     NEAR PTR loc_1_2973
        cmp    eax, 1010104h
        je     NEAR PTR loc_1_2A38
loc_1_27BE:
        mov    dword ptr [ebp - 0ch], 0c00000bbh
        jmp    NEAR PTR loc_1_2A41
loc_1_27CA:
        cmp    byte ptr [ebp + 14h], 0
        jne    SHORT loc_1_27DF
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edi, 5ch
        jmp    NEAR PTR loc_1_2A41
loc_1_27DF:
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edi, 7ch
        jmp    NEAR PTR loc_1_2A41
loc_1_27EE:
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 14h], 2
        cmp    byte ptr [ebx + 13eh], 0
        jne    SHORT loc_1_2808
        mov    dword ptr [ebp - 14h], 0
loc_1_2808:
        lea    esi, [ebp - 14h]
        mov    edi, 4
        jmp    NEAR PTR loc_1_2A41
loc_1_2815:
        lea    esi, [ebp - 18h]
        mov    edi, 4
        jmp    NEAR PTR loc_1_2A41
loc_1_2822:
        mov    dword ptr [ebp - 8], 0eeh
        jmp    NEAR PTR loc_1_2A41
loc_1_282E:
        mov    dword ptr [ebp - 8], 5dch
        jmp    NEAR PTR loc_1_2A41
loc_1_283A:
        mov    dword ptr [ebp - 8], 186a0h
        jmp    NEAR PTR loc_1_2A41
loc_1_2846:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 38h]
        shl    eax, 8
        jmp    NEAR PTR loc_1_2A3E
loc_1_2854:
        mov    eax, 20h
        mov    ebx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ebx + 38h]
        shl    eax, 8
        jmp    NEAR PTR loc_1_2A3E
loc_1_2867:
        mov    dword ptr [ebp - 8], 100h
        jmp    NEAR PTR loc_1_2A41
loc_1_2873:
        mov    dword ptr [ebp - 8], 100h
        jmp    NEAR PTR loc_1_2A41
loc_1_287F:
        push   3
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 10ah
        lea    eax, [ebp - 8]
        push   ebx
        lea    esi, [ebp - 8]
        push   eax
        mov    edi, 4
        call   sub_1_3DAC
        and    dword ptr [ebp - 8], 0ffffff00h
        jmp    NEAR PTR loc_1_2A41
loc_1_28A8:
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edi, 15h
        jmp    NEAR PTR loc_1_2A41
loc_1_28B7:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_1_28D1
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 14bh]
        mov    ecx, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 8], ecx
        jmp    NEAR PTR loc_1_2A41
loc_1_28D1:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 14h]
        mov    edx, dword ptr [ecx + 8]
        mov    dword ptr [ebp - 8], edx
        jmp    NEAR PTR loc_1_2A41
loc_1_28E2:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_1_28F6
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 15dh]
        jmp    NEAR PTR loc_1_2A3E
loc_1_28F6:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 8], ecx
        jmp    NEAR PTR loc_1_2A41
loc_1_2904:
        lea    esi, [ebp - 2]
        mov    edi, 2
        mov    word ptr [ebp - 2], 300h
        jmp    NEAR PTR loc_1_2A41
loc_1_2917:
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_1_2A41
loc_1_2923:
        mov    dword ptr [ebp - 8], 0fh
        jmp    NEAR PTR loc_1_2A41
loc_1_292F:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 10ah
        lea    eax, [ebp - 20h]
        push   ebx
        lea    esi, [ebp - 20h]
        push   eax
        mov    edi, 6
        call   sub_1_3DAC
        jmp    NEAR PTR loc_1_2A41
loc_1_2951:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 104h
        lea    eax, [ebp - 20h]
        push   ebx
        lea    esi, [ebp - 20h]
        push   eax
        mov    edi, 6
        call   sub_1_3DAC
        jmp    NEAR PTR loc_1_2A41
loc_1_2973:
        cmp    byte ptr [ebp + 14h], 0
        mov    ebx, dword ptr [ebp + 8]
        je     SHORT loc_1_29D3
        mov    ecx, dword ptr [ebx + 14bh]
        mov    eax, dword ptr [ecx + 24h]
        mov    dword ptr [ebp - 10h], eax
        lea    ebx, [eax + eax*2]
        lea    edx, [ebx*2]
        cmp    dword ptr [ebp + 1ch], edx
        jae    SHORT loc_1_29A8
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 0ch], 0c0010014h
        mov    dword ptr [eax], edx
        jmp    NEAR PTR loc_1_2A41
loc_1_29A8:
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
loc_1_29BE:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ecx], eax
        je     SHORT loc_1_2A47
        jmp    SHORT loc_1_2A41
loc_1_29D3:
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebx + 14bh]
        mov    ecx, dword ptr [eax + 14h]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_29E6:
        lea    edx, [eax + eax*2]
        mov    dword ptr [ebp - 10h], eax
        lea    ecx, [edx*2]
        cmp    dword ptr [ebp + 1ch], ecx
        jae    SHORT loc_1_2A06
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 0ch], 0c0010014h
        mov    dword ptr [eax], ecx
        jmp    SHORT loc_1_2A41
loc_1_2A06:
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
loc_1_2A29:
        mov    ecx, dword ptr [ebp - 10h]
        lea    eax, [ecx + ecx*2]
        mov    ecx, dword ptr [ebp + 24h]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_1_2A41
loc_1_2A38:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 41h]
loc_1_2A3E:
        mov    dword ptr [ebp - 8], eax
loc_1_2A41:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_2A7D
loc_1_2A47:
        cmp    dword ptr [ebp + 10h], 1010103h
        je     SHORT loc_1_2A7D
        cmp    edi, dword ptr [ebp + 1ch]
        jbe    SHORT loc_1_2A6D
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
loc_1_2A6D:
        push   edi
        mov    eax, dword ptr [ebp + 18h]
        push   esi
        push   eax
        call   sub_1_3DAC
        mov    ecx, dword ptr [ebp + 24h]
        add    dword ptr [ecx], edi
loc_1_2A7D:
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
sub_1_2AE0:
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
        call   sub_1_2760
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 20h], ecx
        mov    dword ptr [esi + 24h], edx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2B30:
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
        ja     SHORT loc_1_2B6D
        je     NEAR PTR loc_1_2C7C
        cmp    eax, 1010eh
        je     SHORT loc_1_2B7F
        cmp    eax, 1010fh
        je     SHORT loc_1_2BDA
        cmp    eax, 10112h
        je     NEAR PTR loc_1_2C55
loc_1_2B6D:
        mov    ebx, 0c0010017h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_1_2CB7
loc_1_2B7F:
        cmp    edi, 4
        je     SHORT loc_1_2B96
        mov    ebx, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_1_2CB7
loc_1_2B96:
        push   4
        lea    eax, [ebp - 8]
        push   ecx
        push   eax
        call   sub_1_3DAC
        test   dword ptr [ebp - 8], 0f050h
        je     SHORT loc_1_2BC3
        mov    dword ptr [esi + 20h], 4
        mov    dword ptr [esi + 24h], 0
        mov    ebx, 0c00000bbh
        jmp    NEAR PTR loc_1_2CB7
loc_1_2BC3:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0ch]
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   esi
        push   ecx
        push   edx
        call   sub_1_2CD0
        jmp    NEAR PTR loc_1_2CB5
loc_1_2BDA:
        cmp    edi, 4
        je     SHORT loc_1_2BF1
        mov    ebx, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_1_2CB7
loc_1_2BF1:
        push   4
        lea    eax, [ebp - 4]
        push   ecx
        push   eax
        call   sub_1_3DAC
        cmp    dword ptr [ebp - 4], 0eeh
        ja     SHORT loc_1_2C4E
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 15dh]
        cmp    eax, dword ptr [ebp - 4]
        jae    SHORT loc_1_2C28
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 15dh], eax
        mov    dword ptr [ecx + 18h], eax
        jmp    NEAR PTR loc_1_2CB7
loc_1_2C28:
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [ecx + 18h], eax
        jne    SHORT loc_1_2C46
        mov    eax, dword ptr [ecx + 18h]
        cmp    eax, dword ptr [ebp - 4]
        jbe    SHORT loc_1_2C46
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    dword ptr [ecx + 18h], eax
        call   sub_1_2500
        jmp    SHORT loc_1_2CB7
loc_1_2C46:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 18h], eax
        jmp    SHORT loc_1_2CB7
loc_1_2C4E:
        mov    ebx, 0c0010014h
        jmp    SHORT loc_1_2CB7
loc_1_2C55:
        cmp    edi, 4
        je     SHORT loc_1_2C69
        mov    ebx, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    SHORT loc_1_2CB7
loc_1_2C69:
        push   4
        xor    ebx, ebx
        push   ecx
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 71h
        push   ecx
        call   sub_1_3DAC
        jmp    SHORT loc_1_2CB7
loc_1_2C7C:
        mov    ebx, 6
        mov    eax, edi
        sub    edx, edx
        div    ebx
        test   edx, edx
        je     SHORT loc_1_2C9A
        mov    ebx, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    SHORT loc_1_2CB7
loc_1_2C9A:
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
        call   sub_1_2D30
loc_1_2CB5:
        mov    ebx, eax
loc_1_2CB7:
        test   ebx, ebx
        jne    SHORT loc_1_2CC5
        mov    dword ptr [esi + 24h], 0
        mov    dword ptr [esi + 20h], edi
loc_1_2CC5:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
sub_1_2CD0:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        cmp    byte ptr [ecx + 13eh], 0
        jne    SHORT loc_1_2D19
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 20h], 0
        jne    SHORT loc_1_2D0F
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
loc_1_2D07:
        dec    dword ptr [esi + 1ch]
        pop    esi
        pop    ebp
        ret    10h
loc_1_2D0F:
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    10h
loc_1_2D19:
        mov    eax, 0c001000dh
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2D30:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        cmp    byte ptr [ecx + 13eh], 0
        jne    SHORT loc_1_2D7D
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 20h], 0
        jne    SHORT loc_1_2D73
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
loc_1_2D6B:
        dec    dword ptr [esi + 1ch]
        pop    esi
        pop    ebp
        ret    14h
loc_1_2D73:
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    14h
loc_1_2D7D:
        mov    eax, 0c001000dh
        pop    esi
        pop    ebp
        ret    14h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2D90:
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
        call   sub_1_2760
        cmp    eax, 0c00000bbh
        mov    edi, eax
        jne    NEAR PTR loc_1_2E76
        xor    edi, edi
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 1020101h
        ja     SHORT loc_1_2E03
        je     SHORT loc_1_2E40
        sub    eax, 20101h
        cmp    eax, 4
        ja     SHORT loc_1_2E11
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_2E03:
        cmp    eax, 1020102h
        je     SHORT loc_1_2E48
        cmp    eax, 1020103h
        je     SHORT loc_1_2E50
loc_1_2E11:
        mov    edi, 0c0010017h
        jmp    SHORT loc_1_2E59
loc_1_2E18:
        mov    eax, dword ptr [ebx + 11eh]
        jmp    SHORT loc_1_2E56
loc_1_2E20:
        mov    eax, dword ptr [ebx + 122h]
        jmp    SHORT loc_1_2E56
loc_1_2E28:
        mov    eax, dword ptr [ebx + 126h]
        jmp    SHORT loc_1_2E56
loc_1_2E30:
        mov    eax, dword ptr [ebx + 136h]
        jmp    SHORT loc_1_2E56
loc_1_2E38:
        mov    eax, dword ptr [ebx + 13ah]
        jmp    SHORT loc_1_2E56
loc_1_2E40:
        mov    eax, dword ptr [ebx + 132h]
        jmp    SHORT loc_1_2E56
loc_1_2E48:
        mov    eax, dword ptr [ebx + 12ah]
        jmp    SHORT loc_1_2E56
loc_1_2E50:
        mov    eax, dword ptr [ebx + 12eh]
loc_1_2E56:
        mov    dword ptr [ebp - 4], eax
loc_1_2E59:
        cmp    dword ptr [ebp - 14h], 4
        jb     SHORT loc_1_2E76
        test   edi, edi
        jne    SHORT loc_1_2E76
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        call   sub_1_3DAC
        add    dword ptr [ebp - 8], 4
loc_1_2E76:
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
loc_1_2EB0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        xor    esi, esi
        call   sub_1_0EA0
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ecx + 14h]
        cmp    eax, 10113h
        ja     SHORT loc_1_2EDB
        je     SHORT loc_1_2F0C
        cmp    eax, 10101h
        jb     SHORT loc_1_2F07
        cmp    eax, 10111h
        jbe    SHORT loc_1_2F0C
        jmp    SHORT loc_1_2F07
loc_1_2EDB:
        cmp    eax, 1010104h
        ja     SHORT loc_1_2EF9
        cmp    eax, 1010101h
        jae    SHORT loc_1_2F0C
        cmp    eax, 20101h
        jb     SHORT loc_1_2F07
        cmp    eax, 20105h
        jbe    SHORT loc_1_2F0C
        jmp    SHORT loc_1_2F07
loc_1_2EF9:
        cmp    eax, 1020101h
        jb     SHORT loc_1_2F07
        cmp    eax, 1020103h
        jbe    SHORT loc_1_2F0C
loc_1_2F07:
        mov    esi, 0c0010017h
loc_1_2F0C:
        mov    edi, dword ptr [ebp + 8]
        inc    dword ptr [edi + 30h]
        test   esi, esi
        jne    SHORT loc_1_2F1F
        push   ecx
        push   edi
        call   sub_1_2D90
        mov    esi, eax
loc_1_2F1F:
        mov    eax, dword ptr [edi + 30h]
        dec    eax
        mov    dword ptr [edi + 30h], eax
        jne    SHORT loc_1_2F50
        cmp    byte ptr [edi + 13eh], 0
        jne    SHORT loc_1_2F40
        cmp    dword ptr [edi + 16dh], 0
        jne    SHORT loc_1_2F40
        cmp    dword ptr [edi + 2ch], 0
        je     SHORT loc_1_2F50
loc_1_2F40:
        push   1
        add    edi, 179h
        push   edi
        call   sub_1_3CA2
        jmp    SHORT loc_1_2F6F
loc_1_2F50:
        cmp    dword ptr [edi + 47h], 0
        je     SHORT loc_1_2F6F
        mov    eax, dword ptr [edi + 4fh]
        cmp    dword ptr [edi + eax*4 + 5dh], 0
        jne    SHORT loc_1_2F6F
        cmp    byte ptr [edi + 11dh], 0
        jne    SHORT loc_1_2F6F
        push   edi
        call   sub_1_2140
loc_1_2F6F:
        call   sub_1_0ED0
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
loc_1_2F80:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 4
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_3BEC
        push   0
        db  08Bh, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        call   sub_1_3BE0
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2FB0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [esi + 1b2h], 1
        call   sub_1_0EA0
        db  03Bh, 035h, 03Eh, 000h, 000h, 000h
        jne    SHORT loc_1_2FD9
        mov    eax, dword ptr [esi + 24h]
        db  0A3h, 03Eh, 000h, 000h, 000h
        jmp    SHORT loc_1_2FF5
loc_1_2FD9:
        db  08Bh, 00Dh, 03Eh, 000h, 000h, 000h
        cmp    dword ptr [ecx + 24h], esi
        je     SHORT loc_1_2FEC
loc_1_2FE4:
        mov    ecx, dword ptr [ecx + 24h]
        cmp    dword ptr [ecx + 24h], esi
        jne    SHORT loc_1_2FE4
loc_1_2FEC:
        mov    eax, dword ptr [ecx + 24h]
        mov    edx, dword ptr [eax + 24h]
        mov    dword ptr [ecx + 24h], edx
loc_1_2FF5:
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 1a5h]
        push   eax
        mov    edi, 61a8h
        push   ecx
        call   sub_1_3CA8
loc_1_300A:
        push   64h
        call   sub_1_3CC0
        dec    edi
        jne    SHORT loc_1_300A
        mov    eax, dword ptr [esi + 14bh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_3020:
        lea    eax, [esi + 8]
        push   eax
        call   sub_1_3CB4
        lea    eax, [esi + 2bbh]
        push   eax
        call   sub_1_3CB4
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_3BF8
        push   0
        push   2d3h
        push   esi
        call   sub_1_3CCC
        call   sub_1_0ED0
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_3060:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 20h], 0
        mov    esi, dword ptr [ebx + 8]
        je     SHORT loc_1_3083
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3083:
        call   sub_1_0EA0
        inc    dword ptr [ebx + 1ch]
        inc    dword ptr [esi + 30h]
        cmp    byte ptr [esi + 13eh], 0
        je     SHORT loc_1_30B0
        dec    dword ptr [ebx + 1ch]
        dec    dword ptr [esi + 30h]
        call   sub_1_0ED0
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_30B0:
        mov    edi, dword ptr [esi + 28h]
        mov    dword ptr [ebp - 4], 0
        test   edi, edi
        je     SHORT loc_1_30E0
loc_1_30BE:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        inc    dword ptr [edi + 1ch]
        push   40010004h
        mov    ecx, dword ptr [edi]
        push   ecx
        call   sub_1_3C2E
        mov    eax, dword ptr [edi + 0ch]
        dec    dword ptr [edi + 1ch]
        mov    edi, eax
        test   eax, eax
        jne    SHORT loc_1_30BE
loc_1_30E0:
        mov    byte ptr [esi + 13eh], 1
        mov    dword ptr [esi + 13fh], 0
        push   esi
        mov    dword ptr [esi + 143h], ebx
        call   sub_1_3170
        mov    dword ptr [ebp - 4], eax
        dec    dword ptr [ebx + 1ch]
        mov    eax, dword ptr [esi + 30h]
        dec    eax
        mov    dword ptr [esi + 30h], eax
        jne    SHORT loc_1_3134
        cmp    byte ptr [esi + 13eh], 0
        jne    SHORT loc_1_3124
        cmp    dword ptr [esi + 16dh], 0
        jne    SHORT loc_1_3124
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_1_3134
loc_1_3124:
        push   1
        add    esi, 179h
        push   esi
        call   sub_1_3CA2
        jmp    SHORT loc_1_3153
loc_1_3134:
        cmp    dword ptr [esi + 47h], 0
        je     SHORT loc_1_3153
        mov    eax, dword ptr [esi + 4fh]
        cmp    dword ptr [esi + eax*4 + 5dh], 0
        jne    SHORT loc_1_3153
        cmp    byte ptr [esi + 11dh], 0
        jne    SHORT loc_1_3153
        push   esi
        call   sub_1_2140
loc_1_3153:
        call   sub_1_0ED0
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3170:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 30h]
        cmp    dword ptr [esi + 16dh], 0
        je     SHORT loc_1_31BC
loc_1_3184:
        mov    ecx, dword ptr [esi + 16dh]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 16dh], eax
        mov    eax, dword ptr [ecx + 8]
        mov    edi, dword ptr [ecx + 4]
        cmp    eax, 4
        je     SHORT loc_1_31B0
        cmp    eax, 8
        je     SHORT loc_1_31B0
        push   0c001000ch
        mov    eax, dword ptr [edi]
        push   ecx
        push   eax
        call   sub_1_3C1C
loc_1_31B0:
        dec    dword ptr [edi + 1ch]
        cmp    dword ptr [esi + 16dh], 0
        jne    SHORT loc_1_3184
loc_1_31BC:
        cmp    dword ptr [esi + 57h], -1
        je     SHORT loc_1_31DA
        mov    eax, 103h
        pop    edi
        mov    dword ptr [esi + 13fh], 2
        dec    dword ptr [esi + 30h]
        pop    esi
        pop    ebp
        ret    4
loc_1_31DA:
        push   esi
        dec    dword ptr [esi + 30h]
        call   sub_1_31F0
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_31F0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        mov    ecx, dword ptr [esi + 30h]
        inc    ecx
        mov    eax, dword ptr [esi + 38h]
        mov    dword ptr [esi + 30h], ecx
        mov    dword ptr [esi + 4fh], edi
        mov    dword ptr [esi + 53h], edi
        cmp    eax, edi
        je     SHORT loc_1_321E
        lea    edx, [esi + 5dh]
loc_1_3211:
        cmp    dword ptr [edx], 0
        jne    SHORT loc_1_322E
        add    edx, 4
        inc    edi
        cmp    eax, edi
        ja     SHORT loc_1_3211
loc_1_321E:
        dec    ecx
        push   esi
        mov    dword ptr [esi + 30h], ecx
        call   sub_1_3250
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_322E:
        dec    ecx
        mov    eax, 103h
        pop    edi
        mov    dword ptr [esi + 30h], ecx
        mov    dword ptr [esi + 13fh], 3
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3250:
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
        jbe    SHORT loc_1_32A0
        lea    edi, [esi + 8dh]
loc_1_3272:
        mov    ecx, dword ptr [edi]
        test   ecx, ecx
        je     SHORT loc_1_3292
        mov    ebx, dword ptr [ecx + 28h]
        push   0c001000ch
        push   ecx
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_3C0A
        dec    dword ptr [ebx + 1ch]
        mov    dword ptr [edi], 0
loc_1_3292:
        add    edi, 4
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [esi + 38h], eax
        ja     SHORT loc_1_3272
loc_1_32A0:
        mov    edx, dword ptr [esi + 47h]
        test   edx, edx
        je     SHORT loc_1_32CA
loc_1_32A7:
        lea    eax, [edx + 24h]
        push   0c001000ch
        push   edx
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi + 47h], ecx
        mov    edi, dword ptr [eax + 4]
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_3C0A
        dec    dword ptr [edi + 1ch]
        mov    edx, dword ptr [esi + 47h]
        test   edx, edx
        jne    SHORT loc_1_32A7
loc_1_32CA:
        mov    edx, dword ptr [esi + 161h]
        test   edx, edx
        je     SHORT loc_1_32FD
loc_1_32D4:
        lea    eax, [edx + 24h]
        push   0c001000ch
        push   edx
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi + 161h], ecx
        mov    edi, dword ptr [eax + 4]
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_3C0A
        dec    dword ptr [edi + 1ch]
        mov    edx, dword ptr [esi + 161h]
        test   edx, edx
        jne    SHORT loc_1_32D4
loc_1_32FD:
        cmp    byte ptr [esi + 11dh], 0
        je     SHORT loc_1_331E
loc_1_3306:
        mov    edi, 64h
loc_1_330B:
        push   64h
        call   sub_1_3CC0
        dec    edi
        jne    SHORT loc_1_330B
        cmp    byte ptr [esi + 11dh], 0
        jne    SHORT loc_1_3306
loc_1_331E:
        mov    byte ptr [esi + 15ch], 1bh
        push   esi
        call   sub_1_01C0
        mov    edi, dword ptr [esi + 28h]
        cmp    al, 1
        sbb    eax, eax
        xor    ebx, ebx
        and    eax, 0c0000001h
        mov    byte ptr [esi + 13eh], bl
        mov    dword ptr [ebp - 4], eax
        test   edi, edi
        je     SHORT loc_1_3384
loc_1_3346:
        inc    dword ptr [edi + 1ch]
        cmp    dword ptr [ebp - 4], ebx
        je     SHORT loc_1_335D
        push   ebx
        mov    eax, dword ptr [edi]
        push   ebx
        push   40010007h
        push   eax
        call   sub_1_3C2E
loc_1_335D:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [edi]
        push   40010005h
        push   ecx
        call   sub_1_3C2E
        mov    ecx, dword ptr [edi]
        push   ecx
        call   sub_1_3C34
        mov    eax, dword ptr [edi + 0ch]
        dec    dword ptr [edi + 1ch]
        mov    edi, eax
        test   eax, eax
        jne    SHORT loc_1_3346
loc_1_3384:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        dec    dword ptr [esi + 30h]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_33A0:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 13fh], eax
        jne    NEAR PTR loc_1_348D
        cmp    eax, 1
        je     SHORT loc_1_3423
        cmp    eax, 2
        je     SHORT loc_1_342B
        cmp    eax, 3
        je     SHORT loc_1_3433
        mov    eax, dword ptr [ebp - 4]
loc_1_33CD:
        cmp    eax, 103h
        je     SHORT loc_1_33EC
        push   eax
        mov    eax, dword ptr [esi + 143h]
        mov    ecx, dword ptr [eax]
        push   ecx
        call   sub_1_3C16
        mov    ecx, dword ptr [esi + 143h]
        dec    dword ptr [ecx + 1ch]
loc_1_33EC:
        mov    eax, dword ptr [esi + 30h]
        dec    eax
        mov    dword ptr [esi + 30h], eax
        jne    SHORT loc_1_346E
        cmp    byte ptr [esi + 13eh], 0
        jne    SHORT loc_1_340D
        cmp    dword ptr [esi + 16dh], 0
        jne    SHORT loc_1_340D
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_1_346E
loc_1_340D:
        push   1
        add    esi, 179h
        push   esi
        call   sub_1_3CA2
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_3423:
        push   esi
        call   sub_1_3170
        jmp    SHORT loc_1_33CD
loc_1_342B:
        push   esi
        call   sub_1_31F0
        jmp    SHORT loc_1_33CD
loc_1_3433:
        mov    edx, dword ptr [esi + 30h]
        xor    eax, eax
        inc    edx
        mov    ecx, dword ptr [esi + 38h]
        mov    dword ptr [esi + 30h], edx
        cmp    ecx, eax
        je     SHORT loc_1_3453
        lea    edi, [esi + 5dh]
loc_1_3446:
        cmp    dword ptr [edi], 0
        jne    SHORT loc_1_3462
        add    edi, 4
        inc    eax
        cmp    eax, ecx
        jb     SHORT loc_1_3446
loc_1_3453:
        dec    edx
        push   esi
        mov    dword ptr [esi + 30h], edx
        call   sub_1_3250
        jmp    NEAR PTR loc_1_33CD
loc_1_3462:
        dec    edx
        pop    edi
        mov    dword ptr [esi + 30h], edx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_346E:
        cmp    dword ptr [esi + 47h], 0
        je     SHORT loc_1_348D
        mov    eax, dword ptr [esi + 4fh]
        cmp    dword ptr [esi + eax*4 + 5dh], 0
        jne    SHORT loc_1_348D
        cmp    byte ptr [esi + 11dh], 0
        jne    SHORT loc_1_348D
        push   esi
        call   sub_1_2140
loc_1_348D:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_34A0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 18h]
        mov    esi, dword ptr [ebp + 1ch]
        mov    edi, dword ptr [ebx + 8]
        call   sub_1_0EA0
        test   esi, esi
        jne    SHORT loc_1_34BB
        lea    esi, [ebx + 49h]
loc_1_34BB:
        cmp    byte ptr [edi + 13eh], 0
        je     SHORT loc_1_34CB
        mov    esi, 0c001000dh
        jmp    SHORT loc_1_3510
loc_1_34CB:
        mov    dword ptr [esi + 4], ebx
        inc    dword ptr [ebx + 1ch]
        cmp    byte ptr [ebp + 20h], 1
        sbb    eax, eax
        and    eax, 0fffffffah
        add    eax, 0ah
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [esi], 0
        cmp    dword ptr [edi + 16dh], 0
        jne    SHORT loc_1_34FD
        mov    dword ptr [edi + 171h], esi
        mov    dword ptr [edi + 16dh], esi
        jmp    SHORT loc_1_350B
loc_1_34FD:
        mov    eax, dword ptr [edi + 171h]
        mov    dword ptr [eax], esi
        mov    dword ptr [edi + 171h], esi
loc_1_350B:
        mov    esi, 103h
loc_1_3510:
        call   sub_1_0ED0
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    1ch
        db  08Bh, 0FFh
loc_1_3520:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 10h]
        mov    edi, dword ptr [ebp + 14h]
        mov    esi, dword ptr [ebx + 8]
        call   sub_1_0EA0
        test   edi, edi
        jne    SHORT loc_1_353B
        lea    edi, [ebx + 21h]
loc_1_353B:
        cmp    byte ptr [esi + 13eh], 0
        je     SHORT loc_1_354B
        mov    esi, 0c001000dh
        jmp    SHORT loc_1_358D
loc_1_354B:
        mov    dword ptr [edi + 4], ebx
        inc    dword ptr [ebx + 1ch]
        cmp    byte ptr [ebp + 18h], 1
        sbb    eax, eax
        add    eax, 9
        mov    dword ptr [edi + 8], eax
        mov    dword ptr [edi], 0
        cmp    dword ptr [esi + 16dh], 0
        jne    SHORT loc_1_357A
        mov    dword ptr [esi + 171h], edi
        mov    dword ptr [esi + 16dh], edi
        jmp    SHORT loc_1_3588
loc_1_357A:
        mov    eax, dword ptr [esi + 171h]
        mov    dword ptr [eax], edi
        mov    dword ptr [esi + 171h], edi
loc_1_3588:
        mov    esi, 103h
loc_1_358D:
        call   sub_1_0ED0
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    14h
        db  005h, 000h, 000h, 000h, 000h
loc_1_35A0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        dec    dword ptr [eax + 1ch]
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_35B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        call   sub_1_0EA0
        mov    esi, dword ptr [ebp + 0ch]
        inc    dword ptr [esi + 30h]
        cmp    byte ptr [esi + 13eh], 0
        jne    NEAR PTR loc_1_3670
        cmp    dword ptr [esi + 175h], 0
        jne    NEAR PTR loc_1_3670
loc_1_35DB:
        mov    edi, dword ptr [esi + 16dh]
        test   edi, edi
        je     SHORT loc_1_3646
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 16dh], eax
        cmp    dword ptr [esi + 171h], edi
        jne    SHORT loc_1_35FF
        mov    dword ptr [esi + 171h], 0
loc_1_35FF:
        mov    dword ptr [esi + 175h], edi
        mov    eax, dword ptr [edi + 8]
        cmp    eax, 4
        je     SHORT loc_1_361A
        cmp    eax, 0ah
        je     SHORT loc_1_361A
        push   esi
        call   sub_1_3730
        jmp    SHORT loc_1_3620
loc_1_361A:
        push   esi
        call   sub_1_37A0
loc_1_3620:
        mov    ebx, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi + 8]
        cmp    ecx, 8
        je     SHORT loc_1_363A
        cmp    ecx, 4
        je     SHORT loc_1_363A
        push   eax
        push   edi
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_3C1C
loc_1_363A:
        dec    dword ptr [ebx + 1ch]
        cmp    byte ptr [esi + 13eh], 0
        je     SHORT loc_1_35DB
loc_1_3646:
        mov    dword ptr [esi + 175h], 0
        cmp    byte ptr [esi + 13eh], 0
        je     SHORT loc_1_3670
        push   1
        dec    dword ptr [esi + 30h]
        push   esi
        call   sub_1_33A0
        call   sub_1_0ED0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
loc_1_3670:
        mov    edi, dword ptr [esi + 2ch]
        test   edi, edi
        je     SHORT loc_1_36D6
        xor    ebx, ebx
loc_1_3679:
        cmp    dword ptr [edi + 1ch], 0
        jbe    SHORT loc_1_3686
        mov    edi, dword ptr [edi + 0ch]
        mov    ebx, edi
        jmp    SHORT loc_1_36C0
loc_1_3686:
        mov    eax, dword ptr [esi + 15dh]
        cmp    dword ptr [edi + 18h], eax
        jne    SHORT loc_1_3697
        push   esi
        call   sub_1_2500
loc_1_3697:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_3C04
        lea    ecx, [edi + 0ch]
        test   ebx, ebx
        mov    eax, dword ptr [ecx]
        je     SHORT loc_1_36AF
        mov    dword ptr [ebx + 0ch], eax
        jmp    SHORT loc_1_36B2
loc_1_36AF:
        mov    dword ptr [esi + 2ch], eax
loc_1_36B2:
        mov    eax, edi
        push   0
        mov    edi, dword ptr [ecx]
        push   75h
        push   eax
        call   sub_1_3CCC
loc_1_36C0:
        test   edi, edi
        jne    SHORT loc_1_3679
        cmp    dword ptr [esi + 2ch], 0
        jne    SHORT loc_1_36D6
        cmp    dword ptr [esi + 28h], 0
        jne    SHORT loc_1_36D6
        push   esi
        call   sub_1_0150
loc_1_36D6:
        dec    dword ptr [esi + 30h]
        jne    SHORT loc_1_3703
        cmp    byte ptr [esi + 13eh], 0
        jne    SHORT loc_1_36F3
        cmp    dword ptr [esi + 16dh], 0
        jne    SHORT loc_1_36F3
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_1_3703
loc_1_36F3:
        push   1
        add    esi, 179h
        push   esi
        call   sub_1_3CA2
        jmp    SHORT loc_1_3722
loc_1_3703:
        cmp    dword ptr [esi + 47h], 0
        je     SHORT loc_1_3722
        mov    eax, dword ptr [esi + 4fh]
        cmp    dword ptr [esi + eax*4 + 5dh], 0
        jne    SHORT loc_1_3722
        cmp    byte ptr [esi + 11dh], 0
        jne    SHORT loc_1_3722
        push   esi
        call   sub_1_2140
loc_1_3722:
        call   sub_1_0ED0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  08Bh, 0FFh
sub_1_3730:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    eax, dword ptr [esi + 14bh]
        mov    edi, dword ptr [eax + 0ch]
        test   edi, 24h
        je     SHORT loc_1_3751
        call   sub_1_0790
        jmp    SHORT loc_1_3756
loc_1_3751:
        call   sub_1_37A0
loc_1_3756:
        test   edi, 26h
        lea    eax, [esi + 15bh]
        je     SHORT loc_1_3769
        or     byte ptr [eax], 8
        jmp    SHORT loc_1_376C
loc_1_3769:
        and    byte ptr [eax], 0f7h
loc_1_376C:
        test   edi, 20h
        je     SHORT loc_1_3779
        or     byte ptr [eax], 10h
        jmp    SHORT loc_1_377C
loc_1_3779:
        and    byte ptr [eax], 0efh
loc_1_377C:
        test   edi, 28h
        je     SHORT loc_1_3789
        or     byte ptr [eax], 4
        jmp    SHORT loc_1_378C
loc_1_3789:
        and    byte ptr [eax], 0fbh
loc_1_378C:
        push   esi
        call   sub_1_0770
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_37A0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_05D0
        push   esi
        call   sub_1_07D0
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_37C0:
        db  0A0h, 058h, 03Eh, 000h, 000h
        cmp    al, 0
        jne    SHORT loc_1_37E5
        db  0FEh, 005h, 058h, 03Eh, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 028h, 000h
        jb     SHORT loc_1_37E7
        push   0
        push   0
        db  0E8h, 0F0h, 0D4h, 0FEh, 0FFh
        cmp    eax, 0
        jne    SHORT loc_1_37E7
loc_1_37E5:
        clc
        ret
loc_1_37E7:
        stc
        ret
CPQNDIS3_Control:
        sub    eax, 1
        cmp    eax, 1bh
        jae    SHORT loc_1_37F8
        db  0FFh, 024h, 085h, 0FAh, 037h, 000h, 000h
loc_1_37F8:
        clc
        ret
        db  0C0h, 037h, 000h, 000h, 0F8h, 037h, 000h, 000h, 0F8h, 037h, 000h, 000h, 0F8h, 037h, 000h, 000h
        db  0F8h, 037h, 000h, 000h, 0F8h, 037h, 000h, 000h, 0F8h, 037h, 000h, 000h, 0F8h, 037h, 000h, 000h
        db  0F8h, 037h, 000h, 000h, 0F8h, 037h, 000h, 000h, 0F8h, 037h, 000h, 000h, 0F8h, 037h, 000h, 000h
        db  0F8h, 037h, 000h, 000h, 0F8h, 037h, 000h, 000h, 0F8h, 037h, 000h, 000h, 0F8h, 037h, 000h, 000h
        db  0F8h, 037h, 000h, 000h, 0F8h, 037h, 000h, 000h, 0F8h, 037h, 000h, 000h, 0F8h, 037h, 000h, 000h
        db  0F8h, 037h, 000h, 000h, 0F8h, 037h, 000h, 000h, 0F8h, 037h, 000h, 000h, 0F8h, 037h, 000h, 000h
        db  0F8h, 037h, 000h, 000h, 0F8h, 037h, 000h, 000h, 0C0h, 037h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h
sub_1_3870:
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
        call   sub_1_38A0
        pop    ebp
        ret    20h
        db  08Dh, 064h, 024h, 000h
sub_1_38A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 18h], 0eh
        jb     NEAR PTR loc_1_3940
        mov    edx, dword ptr [ebp + 10h]
        mov    al, byte ptr [edx]
        test   al, 1
        je     SHORT loc_1_38E0
        cmp    dword ptr [edx + 2], -1
        jne    SHORT loc_1_38D5
        cmp    al, 0ffh
        jne    SHORT loc_1_38D5
        cmp    byte ptr [edx + 1], 0ffh
        jne    SHORT loc_1_38D5
        mov    dword ptr [ebp - 4], 8
        jmp    SHORT loc_1_3947
loc_1_38D5:
        mov    dword ptr [ebp - 4], 2
        jmp    SHORT loc_1_3947
        db  08Bh, 0FFh
loc_1_38E0:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0ch], 20h
        je     SHORT loc_1_3930
        mov    edx, dword ptr [ebp + 10h]
        lea    ecx, [esi + 2ch]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_3906
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 10h], 0
        je     SHORT loc_1_390D
loc_1_3906:
        mov    dword ptr [ebp - 10h], 1
loc_1_390D:
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_3920
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_394A
        db  08Dh, 064h, 024h, 000h
loc_1_3920:
        mov    dword ptr [ebp - 4], 20h
        jmp    SHORT loc_1_394A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3930:
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_394A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3940:
        mov    dword ptr [ebp - 4], 20h
loc_1_3947:
        mov    esi, dword ptr [ebp + 8]
loc_1_394A:
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     NEAR PTR loc_1_3A41
loc_1_3955:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, dword ptr [ebp - 4]
        and    eax, ebx
        test   al, 9
        jne    SHORT loc_1_39AD
        test   al, 2
        je     SHORT loc_1_3999
        lea    eax, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        db  0E8h, 086h, 0E3h, 0FEh, 0FFh
        or     al, al
        je     SHORT loc_1_3999
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 19h]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0ch]
        test   dword ptr [ecx + eax*4], edx
        jne    SHORT loc_1_39AD
loc_1_3999:
        test   byte ptr [ebp - 4], 2
        je     SHORT loc_1_39A4
        test   bl, 4
        jne    SHORT loc_1_39AD
loc_1_39A4:
        test   bl, 20h
        je     NEAR PTR loc_1_3A36
loc_1_39AD:
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
        call   sub_1_3C22
        mov    byte ptr [edi + 18h], 1
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_3A36
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_39EF
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_39EF:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_3A00
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_3A06
        db  08Bh, 0FFh
loc_1_3A00:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_3A06:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_3A15
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_3C28
loc_1_3A15:
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
        call   sub_1_3CCC
        mov    edi, ebx
        jmp    SHORT loc_1_3A39
loc_1_3A36:
        mov    edi, dword ptr [edi + 10h]
loc_1_3A39:
        or     edi, edi
        jne    NEAR PTR loc_1_3955
loc_1_3A41:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_3A50:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3A60
        pop    ebp
        ret    4
sub_1_3A60:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     SHORT loc_1_3ADE
loc_1_3A70:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_3AD7
        mov    byte ptr [edi + 18h], 0
        inc    dword ptr [edi + 0ch]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_3C28
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_3AD7
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_3A9C
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_3A9C:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_3AB0
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_3AB6
        db  005h, 000h, 000h, 000h, 000h
loc_1_3AB0:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_3AB6:
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
        call   sub_1_3CCC
        mov    edi, ebx
        jmp    SHORT loc_1_3ADA
loc_1_3AD7:
        mov    edi, dword ptr [edi + 10h]
loc_1_3ADA:
        or     edi, edi
        jne    SHORT loc_1_3A70
loc_1_3ADE:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3AF0:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0ch]
        test   al, 20h
        je     SHORT loc_1_3B05
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_3B05:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    bl, byte ptr [ecx]
        test   bl, 1
        je     SHORT loc_1_3B41
        cmp    dword ptr [ecx + 2], -1
        jne    SHORT loc_1_3B32
        cmp    bl, 0ffh
        jne    SHORT loc_1_3B32
        cmp    byte ptr [ecx + 1], 0ffh
        jne    SHORT loc_1_3B32
        test   al, 8
        je     SHORT loc_1_3B2B
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_3B2B:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
loc_1_3B32:
        test   al, 4
        jne    SHORT loc_1_3B3A
        test   al, 2
        je     SHORT loc_1_3B59
loc_1_3B3A:
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_3B41:
        mov    eax, dword ptr [edx + 2eh]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_3B59
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 2ch], ax
        jne    SHORT loc_1_3B59
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_3B59:
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
sub_1_3BDA:
        db  0CDh, 020h, 031h, 080h, 028h, 000h
sub_1_3BE0:
        db  0CDh, 020h, 032h, 080h, 028h, 000h
sub_1_3BE6:
        db  0CDh, 020h, 033h, 080h, 028h, 000h
sub_1_3BEC:
        db  0CDh, 020h, 034h, 080h, 028h, 000h
sub_1_3BF2:
        db  0CDh, 020h, 035h, 080h, 028h, 000h
sub_1_3BF8:
        db  0CDh, 020h, 036h, 080h, 028h, 000h
        db  0CDh, 020h, 037h, 080h, 028h, 000h
sub_1_3C04:
        db  0CDh, 020h, 038h, 080h, 028h, 000h
sub_1_3C0A:
        db  0CDh, 020h, 039h, 080h, 028h, 000h
        db  0CDh, 020h, 03Ah, 080h, 028h, 000h
sub_1_3C16:
        db  0CDh, 020h, 03Bh, 080h, 028h, 000h
sub_1_3C1C:
        db  0CDh, 020h, 03Ch, 080h, 028h, 000h
sub_1_3C22:
        db  0CDh, 020h, 03Dh, 080h, 028h, 000h
sub_1_3C28:
        db  0CDh, 020h, 03Eh, 080h, 028h, 000h
sub_1_3C2E:
        db  0CDh, 020h, 03Fh, 080h, 028h, 000h
sub_1_3C34:
        db  0CDh, 020h, 040h, 080h, 028h, 000h
        db  0CDh, 020h, 041h, 080h, 028h, 000h
        db  0CDh, 020h, 009h, 080h, 028h, 000h
        db  0CDh, 020h, 008h, 080h, 028h, 000h
sub_1_3C4C:
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
sub_1_3CA2:
        db  0CDh, 020h, 00Ch, 080h, 028h, 000h
sub_1_3CA8:
        db  0CDh, 020h, 00Dh, 080h, 028h, 000h
sub_1_3CAE:
        db  0CDh, 020h, 00Fh, 080h, 028h, 000h
sub_1_3CB4:
        db  0CDh, 020h, 010h, 080h, 028h, 000h
        db  0CDh, 020h, 011h, 080h, 028h, 000h
sub_1_3CC0:
        db  0CDh, 020h, 00Eh, 080h, 028h, 000h
sub_1_3CC6:
        db  0CDh, 020h, 00Ah, 080h, 028h, 000h
sub_1_3CCC:
        db  0CDh, 020h, 00Bh, 080h, 028h, 000h
        db  0CDh, 020h, 016h, 080h, 028h, 000h
        db  0CDh, 020h, 017h, 080h, 028h, 000h
        db  0CDh, 020h, 018h, 080h, 028h, 000h
        db  0CDh, 020h, 019h, 080h, 028h, 000h
        db  0CDh, 020h, 01Ah, 080h, 028h, 000h
sub_1_3CF0:
        db  0CDh, 020h, 01Bh, 080h, 028h, 000h
        db  0CDh, 020h, 01Ch, 080h, 028h, 000h
        db  0CDh, 020h, 01Dh, 080h, 028h, 000h
        db  0CDh, 020h, 01Eh, 080h, 028h, 000h
        db  0CDh, 020h, 01Fh, 080h, 028h, 000h
        db  0CDh, 020h, 020h, 080h, 028h, 000h
sub_1_3D14:
        db  0CDh, 020h, 021h, 080h, 028h, 000h
        db  0CDh, 020h, 022h, 080h, 028h, 000h
        db  0CDh, 020h, 023h, 080h, 028h, 000h
        db  0CDh, 020h, 024h, 080h, 028h, 000h
        db  0CDh, 020h, 025h, 080h, 028h, 000h
        db  0CDh, 020h, 026h, 080h, 028h, 000h
sub_1_3D38:
        db  0CDh, 020h, 027h, 080h, 028h, 000h
        db  0CDh, 020h, 028h, 080h, 028h, 000h
sub_1_3D44:
        db  0CDh, 020h, 005h, 080h, 028h, 000h
sub_1_3D4A:
        db  0CDh, 020h, 006h, 080h, 028h, 000h
sub_1_3D50:
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
sub_1_3D98:
        db  0CDh, 020h, 045h, 080h, 028h, 000h
        db  0CDh, 020h, 043h, 080h, 028h, 000h
        db  0CDh, 020h, 047h, 080h, 028h, 000h
        add    byte ptr [eax], al
sub_1_3DAC:
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
sub_1_3DCC:
        push   esi
        push   edi
        pushfd
        mov    ecx, dword ptr [esp + 18h]
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 10h]
        cmp    esi, edi
        jae    SHORT loc_1_3DEC
        or     ecx, ecx
        je     SHORT loc_1_3E02
        std
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    SHORT loc_1_3E00
loc_1_3DEC:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     SHORT loc_1_3E00
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd
        mov    cl, al
loc_1_3E00:
        rep movsb
loc_1_3E02:
        popfd
        pop    edi
        pop    esi
        ret    0ch
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 00Ah, 003h, 000h, 000h, 003h, 00Ah, 000h, 000h, 043h, 050h, 051h, 04Eh
        db  044h, 049h, 053h, 033h, 000h, 040h, 000h, 054h, 0E9h, 037h, 000h, 000h, 000h, 000h, 000h, 000h
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
loc_1_3EC0:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_3EC4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3EC8:
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
loc_1_40C8:
        inc    esp
        add    byte ptr [eax], al
        add    al, al
        add    byte ptr [eax], al
        db  000h
loc_1_40D0:
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_40D4:
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_40D8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_40DC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_40E0:
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
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
        add    byte ptr [ebp + 50h], al
        push   eax
        inc    edx
        dec    ecx
        dec    edi
        push   ebx
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
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
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_4174:
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
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_4200:
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
loc_1_427C:
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
        inc    ebx
        push   eax
        push   ecx
        dec    esi
        inc    esp
        dec    ecx
        push   ebx
        xor    eax, dword ptr [eax]
        add    byte ptr [eax], al
        db  000h
loc_1_42E8:
        inc    ebx
        push   eax
        push   ecx
        dec    esi
        inc    esp
        dec    ecx
        push   ebx
        xor    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax + 6fh], dl
        jb     SHORT loc_1_436C
        pop    edi
        push   esp
        jns    SHORT loc_1_436C
        add    byte ptr gs:[eax], al
        add    byte ptr [eax + 6fh], dl
        jb     SHORT loc_1_4378
        pop    edi
        push   esp
        jns    SHORT loc_1_4378
        add    byte ptr gs:[eax], al
        add    byte ptr [ebx + 61h], al
        bound  ebp, qword ptr [ebp + 5fh]
        push   esp
        jns    SHORT loc_1_4385
        add    byte ptr gs:[eax], al
loc_1_4318:
        inc    ebx
        popad
        bound  ebp, qword ptr [ebp + 5fh]
        push   esp
        db  079h, 070h
        add    byte ptr gs:[eax], al
        inc    esp
        imul   esi, dword ptr [ebx + 61h], 5f656c62h
        dec    esp
        imul   ebp, dword ptr [esi + 6bh], 0
loc_1_4334:
        inc    esp
        imul   esi, dword ptr [ebx + 61h], 5f656c62h
        dec    esp
        imul   ebp, dword ptr [esi + 6bh], 0
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
loc_1_4354:
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
loc_1_436C:
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
loc_1_4378:
        push   ebx
        push   ebx
        add    byte ptr [eax], al
loc_1_437C:
        inc    ebx
        push   eax
        push   ecx
        dec    esi
        inc    esp
        dec    ecx
        push   ebx
        xor    esp, dword ptr [eax]
loc_1_4385:
        and    byte ptr [eax], ah
        and    byte ptr [ecx + 64h], al
        popad
        db  070h, 074h
        jb     NEAR PTR loc_1_43BE
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_43A0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_43A4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_43A8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_43AC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_43BE:
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
loc_1_442C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4430:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_4434:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_4438:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_443C:
        add    byte ptr [eax], al
loc_1_443E:
        add    byte ptr [eax], al
sub_1_4440:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_4444:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_4448:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_444C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_4450:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
_LTEXT ends
        end
