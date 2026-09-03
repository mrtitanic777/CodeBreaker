; socket_pcode_02.asm -- SOCKET object 2 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        sub    al, al
        mov    edx, dword ptr [esi + 103h]
        add    dx, 0fh
        out    dx, al
        push   esi
        db  0E8h, 043h, 007h, 0FEh, 0FFh
        push   7d0h
        db  0E8h, 0F9h, 024h, 0FEh, 0FFh
        mov    dx, word ptr [esi + 103h]
        in     al, dx
        cmp    al, 21h
        je     SHORT loc_2_0040
        cmp    al, 23h
        je     SHORT loc_2_0040
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0040:
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0050:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 103h]
        add    dx, 1fh
        mov    cx, word ptr [esi + 103h]
        in     al, dx
        lea    edx, [ecx + 1fh]
        mov    al, 0ffh
        out    dx, al
        mov    dx, cx
        mov    al, 21h
        out    dx, al
        push   7d0h
        db  0E8h, 09Ch, 024h, 0FEh, 0FFh
        mov    dx, word ptr [esi + 103h]
        in     al, dx
        cmp    al, 21h
        je     SHORT loc_2_00A0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_2_00A0:
        mov    di, word ptr [esi + 103h]
        mov    al, 48h
        lea    edx, [edi + 0eh]
        out    dx, al
        lea    edx, [edi + 0fh]
        sub    al, al
        out    dx, al
        lea    edx, [edi + 7]
        mov    al, 0ffh
        out    dx, al
        lea    edx, [edi + 8]
        sub    al, al
        out    dx, al
        lea    edx, [edi + 9]
        out    dx, al
        lea    edx, [edi + 0ah]
        mov    al, 20h
        out    dx, al
        lea    edx, [edi + 0bh]
        sub    al, al
        out    dx, al
        mov    dx, di
        mov    al, 0ah
        out    dx, al
        sub    cl, cl
        lea    edx, [edi + 10h]
loc_2_00DA:
        in     al, dx
        xor    ebx, ebx
        mov    bl, cl
        mov    byte ptr [ebp + ebx - 24h], al
        inc    cl
        cmp    cl, 20h
        jb     SHORT loc_2_00DA
        lea    edx, [edi + 1fh]
        in     al, dx
        mov    edx, dword ptr [esi + 103h]
        add    dx, 1fh
        mov    al, 0ffh
        out    dx, al
        mov    cl, 6
        mov    edx, 42h
        mov    ebx, 57h
loc_2_0107:
        mov    edi, ecx
        and    edi, 0ffh
        mov    al, byte ptr [ebp + edi - 24h]
        cmp    al, dl
        jne    SHORT loc_2_011D
        cmp    byte ptr [ebp + edi - 23h], dl
        je     SHORT loc_2_0139
loc_2_011D:
        cmp    al, bl
        jne    SHORT loc_2_0127
        cmp    byte ptr [ebp + edi - 23h], bl
        je     SHORT loc_2_0139
loc_2_0127:
        inc    cl
        cmp    cl, 1fh
        jb     SHORT loc_2_0107
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0139:
        xor    eax, eax
        mov    al, cl
        cmp    byte ptr [ebp + eax - 24h], 42h
        jne    SHORT loc_2_0156
        mov    byte ptr [esi + 120h], 1
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0156:
        mov    byte ptr [esi + 120h], 0
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_0170:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 4], 0aah
        mov    edi, 400h
        mov    byte ptr [ebp - 3], 55h
        mov    byte ptr [ebp - 2], 0ffh
        mov    byte ptr [ebp - 1], 0
        mov    esi, dword ptr [ebp + 8]
loc_2_0191:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        push   esi
        db  0E8h, 0F2h, 001h, 0FEh, 0FFh
        or     al, al
        je     SHORT loc_2_01D3
        push   4
        push   edi
        lea    eax, [ebp - 8]
        push   eax
        push   esi
        db  0E8h, 051h, 004h, 0FEh, 0FFh
        or     al, al
        je     SHORT loc_2_01D3
        mov    al, byte ptr [ebp - 4]
        cmp    byte ptr [ebp - 8], al
        jne    SHORT loc_2_01D3
        mov    al, byte ptr [ebp - 3]
        cmp    byte ptr [ebp - 7], al
        jne    SHORT loc_2_01D3
        mov    al, byte ptr [ebp - 2]
        cmp    byte ptr [ebp - 6], al
        jne    SHORT loc_2_01D3
        mov    al, byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 5], al
        je     SHORT loc_2_01E6
loc_2_01D3:
        add    edi, 400h
        cmp    edi, 10000h
        jb     SHORT loc_2_0191
        mov    ebx, dword ptr [ebp - 10h]
        jmp    SHORT loc_2_0240
loc_2_01E6:
        mov    ebx, edi
        test   edi, 0ffff0000h
        jne    SHORT loc_2_0240
loc_2_01F0:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        push   ebx
        push   esi
        db  0E8h, 093h, 001h, 0FEh, 0FFh
        or     al, al
        je     SHORT loc_2_0240
        push   4
        push   ebx
        lea    eax, [ebp - 8]
        push   eax
        push   esi
        db  0E8h, 0F2h, 003h, 0FEh, 0FFh
        or     al, al
        je     SHORT loc_2_0240
        mov    al, byte ptr [ebp - 4]
        cmp    byte ptr [ebp - 8], al
        jne    SHORT loc_2_0240
        mov    al, byte ptr [ebp - 3]
        cmp    byte ptr [ebp - 7], al
        jne    SHORT loc_2_0240
        mov    al, byte ptr [ebp - 2]
        cmp    byte ptr [ebp - 6], al
        jne    SHORT loc_2_0240
        mov    al, byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 5], al
        jne    SHORT loc_2_0240
        add    ebx, 400h
        test   ebx, 0ffff0000h
        je     SHORT loc_2_01F0
loc_2_0240:
        cmp    edi, 10000h
        jae    NEAR PTR loc_2_02F1
        cmp    ebx, edi
        je     NEAR PTR loc_2_02F1
        test   ebx, 0ffff0000h
        je     SHORT loc_2_0262
        sub    ebx, 100h
loc_2_0262:
        mov    dword ptr [ebp - 0ch], edi
        cmp    ebx, edi
        jbe    SHORT loc_2_02B7
loc_2_0269:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        push   esi
        db  0E8h, 017h, 001h, 0FEh, 0FFh
        or     al, al
        je     SHORT loc_2_02D0
        push   4
        push   edi
        lea    eax, [ebp - 8]
        push   eax
        push   esi
        db  0E8h, 076h, 003h, 0FEh, 0FFh
        or     al, al
        je     SHORT loc_2_02DB
        mov    al, byte ptr [ebp - 4]
        cmp    byte ptr [ebp - 8], al
        jne    SHORT loc_2_02E6
        mov    al, byte ptr [ebp - 3]
        cmp    byte ptr [ebp - 7], al
        jne    SHORT loc_2_02E6
        mov    al, byte ptr [ebp - 2]
        cmp    byte ptr [ebp - 6], al
        jne    SHORT loc_2_02E6
        mov    al, byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 5], al
        jne    SHORT loc_2_02E6
        add    dword ptr [ebp - 0ch], 4
        cmp    ebx, dword ptr [ebp - 0ch]
        ja     SHORT loc_2_0269
loc_2_02B7:
        mov    dword ptr [esi + 114h], edi
        sub    ebx, edi
        mov    dword ptr [esi + 118h], ebx
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_02D0:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_02DB:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_02E6:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_02F1:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_2_0300:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        db  0E8h, 04Eh, 004h, 0FEh, 0FFh
        mov    cx, word ptr [esi + 103h]
        mov    al, 50h
        lea    edx, [ecx + 0eh]
        out    dx, al
        lea    edx, [ecx + 4]
        mov    al, 0a0h
        out    dx, al
        lea    edx, [ecx + 0dh]
        sub    al, al
        out    dx, al
        lea    edx, [ecx + 0ch]
        mov    al, 20h
        out    dx, al
        lea    edx, [ecx + 1]
        mov    al, 4
        out    dx, al
        lea    edx, [ecx + 2]
        mov    al, 0ffh
        out    dx, al
        lea    edx, [ecx + 3]
        mov    al, 4
        out    dx, al
        lea    edx, [ecx + 5]
        mov    al, 3ch
        out    dx, al
        lea    edx, [ecx + 6]
        sub    al, al
        out    dx, al
        push   7d0h
        db  0E8h, 0C7h, 021h, 0FEh, 0FFh
        mov    di, word ptr [esi + 103h]
        mov    al, 0ffh
        lea    ecx, [edi + 7]
        mov    dx, cx
        out    dx, al
        mov    dx, di
        mov    al, 41h
        out    dx, al
        mov    dx, cx
        mov    al, 4
        out    dx, al
        mov    dx, di
        mov    al, 1
        out    dx, al
        push   7d0h
        db  0E8h, 09Bh, 021h, 0FEh, 0FFh
        mov    dx, word ptr [esi + 103h]
        in     al, dx
        test   al, 1
        jne    SHORT loc_2_03A0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_2_03A0:
        mov    cx, word ptr [esi + 103h]
        mov    al, 37h
        lea    edx, [ecx + 0ah]
        out    dx, al
        cmp    byte ptr [esi + 45h], 3
        mov    dx, cx
        mov    al, 0ah
        out    dx, al
        jne    SHORT loc_2_03E0
        mov    byte ptr [esi + 120h], 0
loc_2_03C0:
        mov    cx, word ptr [esi + 103h]
        sub    al, al
        lea    edx, [ecx + 0fh]
        out    dx, al
        cmp    byte ptr [esi + 120h], al
        mov    dx, cx
        out    dx, al
        lea    edx, [ecx + 0eh]
        je     SHORT loc_2_03FB
        mov    al, 48h
        jmp    SHORT loc_2_03FD
loc_2_03E0:
        push   esi
        call   sub_2_0050
        or     al, al
        jne    SHORT loc_2_03C0
        push   esi
        db  0E8h, 070h, 003h, 0FEh, 0FFh
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_03FB:
        mov    al, 49h
loc_2_03FD:
        out    dx, al
        lea    edx, [ecx + 0dh]
        sub    al, al
        out    dx, al
        lea    edx, [ecx + 0ch]
        out    dx, al
        lea    edx, [ecx + 7]
        mov    al, 0ffh
        out    dx, al
        mov    dx, cx
        mov    al, 21h
        out    dx, al
        lea    edx, [ecx + 0ah]
        sub    al, al
        out    dx, al
        lea    edx, [ecx + 0bh]
        out    dx, al
        mov    cl, 0ffh
        mov    ebx, 80h
loc_2_0425:
        dec    cl
        je     SHORT loc_2_0438
        mov    edx, dword ptr [esi + 103h]
        add    dx, 7
        in     al, dx
        test   bl, al
        je     SHORT loc_2_0425
loc_2_0438:
        mov    cx, word ptr [esi + 103h]
        mov    al, 2
        lea    edx, [ecx + 0dh]
        out    dx, al
        mov    dx, cx
        mov    al, 22h
        out    dx, al
        push   esi
        call   sub_2_0170
        or     al, al
        push   esi
        jne    SHORT loc_2_0466
        db  0E8h, 005h, 003h, 0FEh, 0FFh
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0466:
        db  0E8h, 0F5h, 002h, 0FEh, 0FFh
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0480:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    al, 0ch
        mov    cx, word ptr [esi + 103h]
        lea    edx, [ecx + 0ah]
        out    dx, al
        lea    edx, [ecx + 0bh]
        sub    al, al
        out    dx, al
        lea    edx, [ecx + 8]
        out    dx, al
        lea    edx, [ecx + 9]
        out    dx, al
        mov    dx, cx
        mov    al, 0ah
        out    dx, al
        sub    ecx, ecx
        lea    ebx, [ebx]
loc_2_04B0:
        mov    edx, dword ptr [esi + 103h]
        add    dx, 10h
        in     al, dx
        mov    byte ptr [esi + ecx + 10eh], al
        inc    ecx
        cmp    ecx, 6
        jb     SHORT loc_2_04B0
        cmp    byte ptr [esi + 108h], 0
        jne    SHORT loc_2_0546
        cmp    byte ptr [esi + 109h], 0
        jne    SHORT loc_2_0546
        cmp    byte ptr [esi + 10ah], 0
        jne    SHORT loc_2_0546
        cmp    byte ptr [esi + 10bh], 0
        jne    SHORT loc_2_0546
        cmp    byte ptr [esi + 10ch], 0
        jne    SHORT loc_2_0546
        cmp    byte ptr [esi + 10dh], 0
        jne    SHORT loc_2_0546
        mov    al, byte ptr [esi + 10eh]
        mov    byte ptr [esi + 108h], al
        mov    al, byte ptr [esi + 10fh]
        mov    byte ptr [esi + 109h], al
        mov    al, byte ptr [esi + 110h]
        mov    byte ptr [esi + 10ah], al
        mov    al, byte ptr [esi + 111h]
        mov    byte ptr [esi + 10bh], al
        mov    al, byte ptr [esi + 112h]
        mov    byte ptr [esi + 10ch], al
        mov    al, byte ptr [esi + 113h]
        mov    byte ptr [esi + 10dh], al
loc_2_0546:
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_2_0550:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    al, 21h
        mov    dx, word ptr [ecx + 103h]
        out    dx, al
        in     al, dx
        and    al, 21h
        cmp    al, 21h
        je     SHORT loc_2_0578
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0578:
        cmp    byte ptr [ecx + 120h], 0
        mov    edx, dword ptr [ecx + 103h]
        je     SHORT loc_2_0590
        add    dx, 0eh
        mov    al, 48h
        jmp    SHORT loc_2_0596
        db  090h
loc_2_0590:
        add    dx, 0eh
        mov    al, 49h
loc_2_0596:
        mov    si, word ptr [ecx + 103h]
        out    dx, al
        lea    edx, [esi + 0bh]
        sub    al, al
        out    dx, al
        lea    edx, [esi + 0ah]
        out    dx, al
        lea    edx, [esi + 0ch]
        mov    al, byte ptr [ecx + 15fh]
        out    dx, al
        lea    edx, [esi + 0dh]
        mov    al, 2
        out    dx, al
        mov    bl, byte ptr [ecx + 0feh]
        lea    edx, [esi + 3]
        mov    al, bl
        out    dx, al
        lea    edx, [esi + 1]
        out    dx, al
        lea    edx, [esi + 2]
        mov    al, byte ptr [ecx + 0ffh]
        out    dx, al
        inc    bl
        mov    byte ptr [ecx + 101h], bl
        mov    al, byte ptr [ecx + 0feh]
        inc    al
        mov    byte ptr [ecx + 100h], al
        mov    byte ptr [ecx + 11ch], 0
        lea    eax, [esi + 7]
        mov    dx, ax
        mov    al, 0ffh
        out    dx, al
        lea    edx, [esi + 0fh]
        mov    al, byte ptr [ecx + 160h]
        out    dx, al
        mov    dx, si
        mov    al, 61h
        out    dx, al
        sub    edi, edi
loc_2_060A:
        lea    edx, [esi + edi]
        inc    dx
        mov    al, byte ptr [ecx + edi + 108h]
        out    dx, al
        inc    edi
        cmp    edi, 6
        jb     SHORT loc_2_060A
        mov    eax, dword ptr [ecx + 14fh]
        sub    edi, edi
        mov    ebx, dword ptr [eax + 0ch]
        and    ebx, 4
        add    eax, 0
loc_2_0630:
        or     ebx, ebx
        mov    eax, 0ffh
        jne    SHORT loc_2_0642
        xor    eax, eax
        mov    al, byte ptr [ecx + edi + 153h]
loc_2_0642:
        lea    edx, [esi + edi]
        add    dx, 8
        out    dx, al
        inc    edi
        cmp    edi, 8
        jb     SHORT loc_2_0630
        lea    eax, [esi + 7]
        mov    dx, ax
        mov    al, byte ptr [ecx + 101h]
        out    dx, al
        mov    dx, si
        mov    al, 21h
        out    dx, al
        mov    al, 22h
        out    dx, al
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0680:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        lea    eax, [esi + 8]
        db  068h, 060h, 007h, 000h, 000h
        push   eax
        db  0E8h, 07Fh, 01Eh, 0FEh, 0FFh
        mov    cx, word ptr [esi + 103h]
        sub    al, al
        lea    edx, [ecx + 0bh]
        out    dx, al
        lea    edx, [ecx + 0ah]
        out    dx, al
        sub    edi, edi
        mov    ebx, 80h
loc_2_06B3:
        mov    edx, dword ptr [esi + 103h]
        add    dx, 7
        in     al, dx
        test   bl, al
        jne    SHORT loc_2_06D2
        push   3e8h
        db  0E8h, 054h, 01Eh, 0FEh, 0FFh
        inc    edi
        cmp    edi, 4
        jb     SHORT loc_2_06B3
loc_2_06D2:
        mov    cx, word ptr [esi + 103h]
        mov    al, 2
        lea    edx, [ecx + 0dh]
        out    dx, al
        mov    dx, cx
        mov    al, 22h
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_2_06F0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_0680
        push   7d0h
        db  0E8h, 019h, 01Eh, 0FEh, 0FFh
        push   esi
        call   sub_2_0550
        or     al, al
        jne    SHORT loc_2_0730
        push   1
        push   1
        push   2
        push   0aa000003h
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 0D3h, 01Eh, 0FEh, 0FFh
        add    esp, 14h
        sub    al, al
        pop    esi
        pop    ebp
        ret    4
        db  090h
loc_2_0730:
        mov    edx, dword ptr [esi + 103h]
        add    dx, 0dh
        sub    al, al
        out    dx, al
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0750:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, 0ffffffffh
        sub    edx, edx
        cmp    dword ptr [ebp + 0ch], edx
        je     SHORT loc_2_079D
loc_2_0762:
        mov    ecx, dword ptr [ebp + 8]
        mov    bl, byte ptr [edx + ecx]
        mov    esi, 8
        lea    ecx, [ecx]
loc_2_0770:
        mov    ecx, eax
        and    ecx, 80000000h
        cmp    ecx, 1
        sbb    edi, edi
        inc    edi
        mov    cl, bl
        and    ecx, 1
        xor    edi, ecx
        add    eax, eax
        shr    bl, 1
        or     edi, edi
        je     SHORT loc_2_0794
        xor    eax, 4c11db6h
        or     eax, edi
loc_2_0794:
        dec    esi
        jne    SHORT loc_2_0770
        inc    edx
        cmp    edx, dword ptr [ebp + 0ch]
        jb     SHORT loc_2_0762
loc_2_079D:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_07B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0750
        shr    eax, 1ah
        mov    ebx, eax
        mov    edx, dword ptr [ebp + 0ch]
        shr    ebx, 3
        mov    byte ptr [edx], bl
        mov    dl, 1
        and    al, 7
        mov    cl, al
        mov    eax, dword ptr [ebp + 10h]
        shl    dl, cl
        mov    byte ptr [eax], dl
        pop    ebx
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
sub_2_07E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    esi, dword ptr [ebp + 8]
        sub    edx, edx
        mov    eax, 8
        lea    esp, [esp]
loc_2_0800:
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [esi + ecx + 153h], dl
        inc    dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 8], eax
        jb     SHORT loc_2_0800
        db  068h, 0C0h, 026h, 000h, 000h
        lea    eax, [ebp - 8]
        push   eax
        push   60h
        mov    eax, dword ptr [esi + 14fh]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_2_2B90
        cmp    dword ptr [ebp - 8], 0
        jbe    SHORT loc_2_0867
        sub    edi, edi
loc_2_0835:
        dec    dword ptr [ebp - 8]
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [ebp - 2]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        lea    eax, [eax + eax*2]
        db  08Dh, 004h, 045h, 0C0h, 026h, 000h, 000h
        push   eax
        call   sub_2_07B0
        xor    eax, eax
        mov    al, byte ptr [ebp - 2]
        mov    cl, byte ptr [ebp - 1]
        or     byte ptr [eax + esi + 153h], cl
        cmp    dword ptr [ebp - 8], edi
        ja     SHORT loc_2_0835
loc_2_0867:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_2_0870:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        sub    ecx, ecx
        mov    si, word ptr [eax + 103h]
        mov    dx, si
        mov    al, 62h
        out    dx, al
loc_2_0886:
        lea    edx, [esi + ecx]
        add    dx, 8
        mov    al, 0ffh
        out    dx, al
        inc    ecx
        cmp    ecx, 8
        jb     SHORT loc_2_0886
        mov    dx, si
        mov    al, 22h
        out    dx, al
        sub    al, al
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_08B0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    al, 62h
        mov    di, word ptr [ecx + 103h]
        mov    dx, di
        out    dx, al
        sub    esi, esi
loc_2_08C7:
        lea    edx, [edi + esi]
        add    dx, 8
        mov    al, byte ptr [ecx + esi + 153h]
        out    dx, al
        inc    esi
        cmp    esi, 8
        jb     SHORT loc_2_08C7
        mov    dx, di
        mov    al, 22h
        out    dx, al
        sub    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_08F0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    al, 8
        mov    edx, dword ptr [edi + 103h]
        add    dx, 7
        out    dx, al
        push   edi
        db  0E8h, 055h, 0FEh, 0FDh, 0FFh
        push   7d0h
        db  0E8h, 00Bh, 01Ch, 0FEh, 0FFh
        mov    si, word ptr [edi + 103h]
        mov    al, 2
        lea    ecx, [esi + 0dh]
        mov    dx, cx
        out    dx, al
        mov    dx, si
        mov    al, 22h
        out    dx, al
        mov    dx, cx
        sub    al, al
        out    dx, al
        cmp    dword ptr [edi + 5bh], -1
        je     SHORT loc_2_0941
        mov    byte ptr [edi + 5fh], 1
        push   edi
        db  0E8h, 03Fh, 0FEh, 0FDh, 0FFh
loc_2_0941:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0950:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        mov    word ptr [ebp - 10h], 7
        mov    word ptr [ebp - 0eh], 7
        db  0C7h, 045h, 0F4h, 090h, 028h, 000h, 000h
        mov    word ptr [ebp - 1ch], 0
        mov    word ptr [ebp - 1ah], 3
        mov    word ptr [ebp - 18h], 67b0h
        mov    word ptr [ebp - 16h], 611fh
        mov    word ptr [ebp - 14h], 7154h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 0A0h, 01Ah, 0FEh, 0FFh
        lea    eax, [ebp - 1ch]
        db  0A3h, 0AEh, 027h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        db  0A3h, 06Ch, 027h, 000h, 000h
        db  0C6h, 005h, 0B2h, 027h, 000h, 000h, 000h
        db  0C7h, 005h, 0A6h, 027h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 070h, 027h, 000h, 000h, 003h
        db  0C6h, 005h, 071h, 027h, 000h, 000h, 000h
        db  0C7h, 005h, 072h, 027h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 076h, 027h, 000h, 000h, 060h, 010h, 000h, 000h
        db  0C7h, 005h, 07Ah, 027h, 000h, 000h, 070h, 012h, 000h, 000h
        db  0C7h, 005h, 07Eh, 027h, 000h, 000h, 020h, 01Ah, 000h, 000h
        db  0C7h, 005h, 082h, 027h, 000h, 000h, 000h, 017h, 000h, 000h
        db  0C7h, 005h, 086h, 027h, 000h, 000h, 030h, 01Eh, 000h, 000h
        db  0C7h, 005h, 08Ah, 027h, 000h, 000h, 040h, 014h, 000h, 000h
        db  0C7h, 005h, 08Eh, 027h, 000h, 000h, 090h, 01Ch, 000h, 000h
        db  0C7h, 005h, 092h, 027h, 000h, 000h, 070h, 01Dh, 000h, 000h
        db  0C7h, 005h, 096h, 027h, 000h, 000h, 090h, 00Ah, 000h, 000h
        db  0C7h, 005h, 09Ah, 027h, 000h, 000h, 0A0h, 01Dh, 000h, 000h
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 0ch]
        db  0A3h, 09Eh, 027h, 000h, 000h
        db  089h, 00Dh, 0A2h, 027h, 000h, 000h
        push   36h
        db  068h, 070h, 027h, 000h, 000h
        db  068h, 068h, 027h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        db  068h, 068h, 027h, 000h, 000h
        push   eax
        db  0E8h, 0E0h, 019h, 0FEh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_2_0A80
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 0C9h, 019h, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 8]
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_0A80:
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_2_0A90:
        push   ebp
        mov    ebp, esp
        sub    esp, 58h
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 48h], 7
        mov    word ptr [ebp - 46h], 7
        db  0C7h, 045h, 0BCh, 0A0h, 028h, 000h, 000h
        mov    word ptr [ebp - 40h], 0ah
        mov    word ptr [ebp - 3eh], 0ah
        db  0C7h, 045h, 0C4h, 0B4h, 028h, 000h, 000h
        mov    word ptr [ebp - 30h], 0dh
        mov    word ptr [ebp - 2eh], 0dh
        db  0C7h, 045h, 0D4h, 0D0h, 028h, 000h, 000h
        mov    esi, 300h
        mov    word ptr [ebp - 38h], 0eh
        mov    word ptr [ebp - 36h], 0eh
        db  0C7h, 045h, 0CCh, 018h, 029h, 000h, 000h
        mov    byte ptr [ebp - 1], 0
        mov    bl, 3
        mov    dword ptr [ebp - 18h], 0
        mov    dword ptr [ebp - 20h], 0
        mov    dword ptr [ebp - 1ch], 10h
        db  0A1h, 064h, 027h, 000h, 000h
        push   eax
        lea    eax, [ebp - 0ch]
        push   0
        push   2c3h
        push   eax
        db  0E8h, 00Bh, 01Ah, 0FEh, 0FFh
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        je     SHORT loc_2_0B2B
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_0B2B:
        mov    edi, dword ptr [ebp - 0ch]
        sub    eax, eax
        mov    ecx, 0b0h
        rep stosd
        stosw
        stosb
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 059h, 01Ah, 0FEh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_2_0B70
        push   0
        push   2c3h
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  0E8h, 0CBh, 019h, 0FEh, 0FFh
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  090h
loc_2_0B70:
        push   1
        lea    eax, [ebp - 48h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 023h, 01Ah, 0FEh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_0B93
        mov    eax, dword ptr [ebp - 10h]
        mov    esi, dword ptr [eax + 4]
loc_2_0B93:
        cmp    esi, 120h
        jb     NEAR PTR loc_2_0CC4
        cmp    esi, 0c3d0h
        ja     NEAR PTR loc_2_0CC4
        push   0
        lea    eax, [ebp - 40h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 0E8h, 019h, 0FEh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_0BCE
        mov    eax, dword ptr [ebp - 10h]
        mov    bl, byte ptr [eax + 4]
loc_2_0BCE:
        push   0
        lea    eax, [ebp - 38h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 0C5h, 019h, 0FEh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_0BF6
        mov    eax, dword ptr [ebp - 10h]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 4]
        mov    dword ptr [ebp - 20h], ecx
loc_2_0BF6:
        push   0
        lea    eax, [ebp - 30h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 09Dh, 019h, 0FEh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_0C1C
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebp - 1ch], ecx
loc_2_0C1C:
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        lea    eax, [ebp - 28h]
        push   eax
        lea    eax, [ebp - 24h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 07Bh, 018h, 0FEh, 0FFh
        cmp    dword ptr [ebp - 28h], 6
        jne    SHORT loc_2_0C5B
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_0C5B
        mov    ecx, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp - 24h]
        mov    eax, edx
        add    ecx, 108h
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    al, byte ptr [edx + 4]
        mov    byte ptr [ecx + 4], al
        mov    al, byte ptr [edx + 5]
        mov    byte ptr [ecx + 5], al
loc_2_0C5B:
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        db  0E8h, 04Ch, 019h, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 38h], 0ch
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 3ch], esi
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 40h], bl
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 20h]
        mov    dword ptr [eax + 47h], ecx
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    dword ptr [eax + 41h], ecx
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    al, byte ptr [ebp - 1]
        push   eax
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_2_0CE0
        or     eax, eax
        je     SHORT loc_2_0CCD
        push   0
        push   2c3h
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  0E8h, 076h, 018h, 0FEh, 0FFh
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_0CC4:
        mov    byte ptr [ebp - 1], 1
        mov    dword ptr [ebp - 18h], esi
        jmp    SHORT loc_2_0C5B
loc_2_0CCD:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_0CE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 38h], 0ch
        jbe    SHORT loc_2_0D05
        mov    dword ptr [ebp - 4], 0c000009ah
loc_2_0CF9:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_0D05:
        lea    edi, [ebp - 2ch]
        sub    eax, eax
        mov    ecx, 9
        rep stosd
        stosw
        mov    dword ptr [ebp - 22h], 1
        mov    dword ptr [ebp - 16h], 1
        mov    eax, dword ptr [esi + 3ch]
        lea    edi, [esi + 4]
        mov    dword ptr [ebp - 12h], eax
        mov    dword ptr [ebp - 0eh], 20h
        lea    eax, [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   esi
        db  0A1h, 068h, 027h, 000h, 000h
        push   eax
        push   edi
        db  0E8h, 008h, 017h, 0FEh, 0FFh
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        jne    SHORT loc_2_0CF9
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_2_0D72
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        push   1
        push   0aa00000ah
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 08Eh, 018h, 0FEh, 0FFh
        add    esp, 10h
        jmp    NEAR PTR loc_2_0FC3
loc_2_0D72:
        db  0C7h, 086h, 089h, 001h, 000h, 000h, 0A0h, 01Eh, 000h, 000h
        mov    dword ptr [esi + 17dh], 0
        mov    eax, dword ptr [esi + 189h]
        mov    dword ptr [esi + 181h], eax
        mov    dword ptr [esi + 185h], esi
        mov    eax, dword ptr [esi + 3ch]
        mov    dword ptr [esi + 103h], eax
        push   esi
        call   0
        or     al, al
        jne    SHORT loc_2_0DC9
        push   0
        push   0aa000004h
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 03Eh, 018h, 0FEh, 0FFh
        add    esp, 0ch
        mov    dword ptr [ebp - 4], 0c0010006h
        jmp    NEAR PTR loc_2_0FC3
loc_2_0DC9:
        push   esi
        call   sub_2_0300
        or     al, al
        jne    SHORT loc_2_0DF1
        push   0
        push   0aa000004h
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 016h, 018h, 0FEh, 0FFh
        add    esp, 0ch
        mov    dword ptr [ebp - 4], 0c0010006h
        jmp    NEAR PTR loc_2_0FC3
loc_2_0DF1:
        mov    eax, dword ptr [esi + 114h]
        mov    dword ptr [esi + 0f1h], eax
        mov    byte ptr [esi + 0fdh], ah
        mov    eax, dword ptr [esi + 38h]
        shl    eax, 8
        add    eax, dword ptr [esi + 0f1h]
        mov    dword ptr [esi + 0f5h], eax
        mov    eax, dword ptr [esi + 38h]
        add    al, byte ptr [esi + 0fdh]
        mov    byte ptr [esi + 0feh], al
        mov    eax, dword ptr [esi + 0f1h]
        add    eax, dword ptr [esi + 118h]
        mov    dword ptr [esi + 0f9h], eax
        mov    eax, dword ptr [esi + 118h]
        shr    eax, 8
        add    al, byte ptr [esi + 0fdh]
        mov    byte ptr [esi + 0ffh], al
        mov    byte ptr [esi + 15fh], 0
        mov    byte ptr [esi + 121h], 0
        sub    ecx, ecx
        mov    dword ptr [esi + 5bh], 0ffffffffh
        cmp    dword ptr [esi + 38h], ecx
        jbe    SHORT loc_2_0E7F
        lea    eax, [esi + 61h]
        lea    ebx, [ebx]
loc_2_0E70:
        mov    dword ptr [eax], 0
        add    eax, 4
        inc    ecx
        cmp    dword ptr [esi + 38h], ecx
        ja     SHORT loc_2_0E70
loc_2_0E7F:
        mov    byte ptr [esi + 1b5h], 0
        mov    byte ptr [esi + 11fh], 0
        push   esi
        call   sub_2_0480
        lea    ebx, [esi + 14fh]
        push   ebx
        push   esi
        lea    eax, [esi + 108h]
        push   eax
        db  068h, 060h, 023h, 000h, 000h
        db  068h, 0E0h, 022h, 000h, 000h
        db  068h, 060h, 022h, 000h, 000h
        mov    eax, dword ptr [esi + 41h]
        push   eax
        call   sub_2_2430
        or     al, al
        jne    SHORT loc_2_0EE0
        push   0
        push   0aa000002h
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 02Bh, 017h, 0FEh, 0FFh
        add    esp, 0ch
        mov    dword ptr [ebp - 4], 0c0000001h
        jmp    NEAR PTR loc_2_0FC3
        db  08Dh, 064h, 024h, 000h
loc_2_0EE0:
        mov    byte ptr [esi + 160h], 1bh
        db  0C7h, 046h, 020h, 068h, 027h, 000h, 000h
        db  0A1h, 0A6h, 027h, 000h, 000h
        mov    dword ptr [esi + 24h], eax
        db  089h, 035h, 0A6h, 027h, 000h, 000h
        push   esi
        call   sub_2_0550
        or     al, al
        jne    SHORT loc_2_0F21
        push   0
        push   0aa000004h
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 0E3h, 016h, 0FEh, 0FFh
        add    esp, 0ch
        mov    dword ptr [ebp - 4], 0c0010006h
        jmp    SHORT loc_2_0F7D
loc_2_0F21:
        cmp    dword ptr [esi + 47h], 0
        mov    edx, dword ptr [esi + 103h]
        je     SHORT loc_2_0F35
        add    dx, 18h
        mov    al, 1
        jmp    SHORT loc_2_0F3B
loc_2_0F35:
        add    dx, 18h
        sub    al, al
loc_2_0F3B:
        out    dx, al
        movsx  eax, byte ptr [esi + 40h]
        push   1
        push   0
        push   eax
        push   eax
        db  068h, 0B0h, 009h, 000h, 000h
        push   esi
        db  068h, 090h, 009h, 000h, 000h
        mov    eax, dword ptr [edi]
        push   eax
        lea    eax, [esi + 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 0ADh, 015h, 0FEh, 0FFh
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_2_0FE0
        push   0
        push   0aa000005h
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 07Eh, 016h, 0FEh, 0FFh
        add    esp, 0ch
loc_2_0F7D:
        db  039h, 035h, 0A6h, 027h, 000h, 000h
        jne    SHORT loc_2_0F90
        mov    eax, dword ptr [esi + 24h]
        db  0A3h, 0A6h, 027h, 000h, 000h
        jmp    SHORT loc_2_0FB1
        db  090h
loc_2_0F90:
        db  08Bh, 00Dh, 0A6h, 027h, 000h, 000h
        cmp    dword ptr [ecx + 24h], esi
        je     SHORT loc_2_0FA8
        add    eax, 0
loc_2_0FA0:
        mov    ecx, dword ptr [ecx + 24h]
        cmp    dword ptr [ecx + 24h], esi
        jne    SHORT loc_2_0FA0
loc_2_0FA8:
        mov    eax, dword ptr [ecx + 24h]
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [ecx + 24h], eax
loc_2_0FB1:
        mov    dx, word ptr [esi + 103h]
        mov    al, 1
        out    dx, al
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_2_2550
loc_2_0FC3:
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 08Dh, 014h, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0FE0:
        mov    dword ptr [esi + 18dh], 0
        db  0C7h, 086h, 091h, 001h, 000h, 000h, 0B0h, 009h, 000h, 000h
        mov    dword ptr [esi + 195h], esi
        mov    dword ptr [esi + 199h], 0
        lea    eax, [esi + 1a9h]
        db  0C7h, 086h, 09Dh, 001h, 000h, 000h, 0B0h, 009h, 000h, 000h
        mov    dword ptr [esi + 1a1h], esi
        db  0C7h, 086h, 0A5h, 001h, 000h, 000h, 010h, 01Eh, 000h, 000h
        mov    dword ptr [eax], 0
        mov    ecx, dword ptr [esi + 1a5h]
        mov    dword ptr [esi + 1adh], ecx
        mov    dword ptr [esi + 1b1h], esi
        push   7d0h
        push   eax
        db  0E8h, 0BBh, 014h, 0FEh, 0FFh
        mov    byte ptr [esi + 1b6h], 0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_1060:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [eax], 0
        mov    dword ptr [ecx], 0ffffffffh
        mov    eax, dword ptr [ebp + 14h]
        mov    edx, dword ptr [ebp + 18h]
        lea    eax, [eax + edx*4]
loc_2_1083:
        or     edx, edx
        je     SHORT loc_2_1092
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 0
        jne    SHORT loc_2_1083
        mov    dword ptr [ecx], edx
loc_2_1092:
        cmp    dword ptr [ecx], -1
        jne    SHORT loc_2_10A4
        mov    eax, 0c0010019h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_2_10A4:
        db  0A1h, 064h, 027h, 000h, 000h
        push   eax
        push   0
        push   75h
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 06Fh, 014h, 0FEh, 0FFh
        or     eax, eax
        je     SHORT loc_2_10E0
        push   0
        mov    esi, dword ptr [ebp + 20h]
        push   0aa000002h
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 02Ah, 015h, 0FEh, 0FFh
        add    esp, 0ch
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  08Bh, 0FFh
loc_2_10E0:
        mov    esi, dword ptr [ebp + 20h]
        inc    dword ptr [esi + 30h]
        mov    ecx, dword ptr [esi + 28h]
        or     ecx, ecx
        jne    SHORT loc_2_1100
        cmp    dword ptr [esi + 2ch], 0
        jne    SHORT loc_2_1100
        mov    edx, dword ptr [esi + 103h]
        add    dx, 0dh
        sub    al, al
        out    dx, al
loc_2_1100:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0ch], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 28h], eax
        cmp    byte ptr [esi + 142h], 0
        jne    NEAR PTR loc_2_11F2
        mov    eax, dword ptr [ebp - 4]
        add    eax, 14h
        push   eax
        mov    edi, dword ptr [ebp + 1ch]
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [esi + 14fh]
        push   eax
        call   sub_2_25A0
        or     al, al
        je     NEAR PTR loc_2_11F2
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], esi
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 20h]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], edi
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 28h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 71h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 20h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 18h], 0eeh
        mov    dword ptr [esi + 161h], 0eeh
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 1ch], 1
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esi + 30h]
        dec    eax
        mov    dword ptr [esi + 30h], eax
        jne    SHORT loc_2_11C9
        cmp    byte ptr [esi + 142h], 0
        jne    SHORT loc_2_11B1
        cmp    dword ptr [esi + 171h], 0
        jne    SHORT loc_2_11B1
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_2_11C9
loc_2_11B1:
        push   1
        add    esi, 17dh
        push   esi
        db  0E8h, 043h, 013h, 0FEh, 0FFh
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_2_11C9:
        cmp    dword ptr [esi + 4bh], 0
        je     SHORT loc_2_11E8
        mov    eax, dword ptr [esi + 53h]
        cmp    dword ptr [esi + eax*4 + 61h], 0
        jne    SHORT loc_2_11E8
        cmp    byte ptr [esi + 121h], 0
        jne    SHORT loc_2_11E8
        push   esi
        db  0E8h, 058h, 009h, 0FEh, 0FFh
loc_2_11E8:
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_2_11F2:
        dec    dword ptr [esi + 30h]
        mov    eax, dword ptr [ebp - 4]
        push   0
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [esi + 28h], eax
        push   75h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 021h, 013h, 0FEh, 0FFh
        push   0
        push   0aa000002h
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 0DDh, 013h, 0FEh, 0FFh
        add    esp, 0ch
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  005h, 000h, 000h, 000h, 000h
sub_2_1230:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        sub    edx, edx
        mov    esi, dword ptr [ecx + 28h]
        cmp    esi, edx
        je     SHORT loc_2_1250
loc_2_1240:
        mov    eax, dword ptr [esi + 18h]
        cmp    edx, eax
        jae    SHORT loc_2_1249
        mov    edx, eax
loc_2_1249:
        mov    esi, dword ptr [esi + 0ch]
        or     esi, esi
        jne    SHORT loc_2_1240
loc_2_1250:
        or     edx, edx
        jne    SHORT loc_2_1259
        mov    edx, 0eeh
loc_2_1259:
        mov    dword ptr [ecx + 161h], edx
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_1270:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        cmp    byte ptr [edi + 20h], 0
        je     SHORT loc_2_1293
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1293:
        inc    dword ptr [esi + 30h]
        inc    dword ptr [edi + 1ch]
        mov    ecx, dword ptr [esi + 28h]
        cmp    edi, ecx
        jne    SHORT loc_2_12A8
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [esi + 28h], eax
        jmp    SHORT loc_2_12C1
loc_2_12A8:
        cmp    dword ptr [ecx + 0ch], edi
        je     SHORT loc_2_12B8
loc_2_12B0:
        mov    ecx, dword ptr [ecx + 0ch]
        cmp    dword ptr [ecx + 0ch], edi
        jne    SHORT loc_2_12B0
loc_2_12B8:
        lea    eax, [edi + 0ch]
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [ecx + 0ch], eax
loc_2_12C1:
        mov    edx, dword ptr [esi + 103h]
        add    dx, 7
        in     al, dx
        cmp    al, 0ffh
        jne    SHORT loc_2_132D
        mov    edx, dword ptr [esi + 103h]
        add    dx, 7
        sub    al, al
        out    dx, al
        in     al, dx
        cmp    al, 0ffh
        jne    SHORT loc_2_1320
        mov    eax, dword ptr [esi + 14fh]
        mov    dword ptr [esi + 2bfh], 1
        mov    dword ptr [eax + 28h], 0ffffffffh
        mov    eax, dword ptr [esi + 14fh]
        push   0
        mov    dword ptr [eax + 10h], 0
        push   75h
        push   edi
        db  0E8h, 01Ch, 012h, 0FEh, 0FFh
        dec    dword ptr [esi + 30h]
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_2_1320:
        mov    edx, dword ptr [esi + 103h]
        add    dx, 7
        mov    al, 0ffh
        out    dx, al
loc_2_132D:
        push   0
        mov    eax, dword ptr [edi + 14h]
        push   eax
        mov    eax, dword ptr [esi + 14fh]
        push   eax
        call   sub_2_2670
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_2_1368
        mov    eax, dword ptr [edi + 1ch]
        cmp    eax, 2
        je     SHORT loc_2_1360
        mov    byte ptr [edi + 20h], 1
        sub    dword ptr [edi + 1ch], 2
        mov    ebx, 103h
        jmp    SHORT loc_2_1394
        db  08Dh, 064h, 024h, 000h
loc_2_1360:
        sub    eax, 2
        mov    dword ptr [edi + 1ch], eax
        jmp    SHORT loc_2_1394
loc_2_1368:
        cmp    ebx, 103h
        jne    SHORT loc_2_1380
        mov    byte ptr [edi + 20h], 1
        sub    dword ptr [edi + 1ch], 2
        jmp    SHORT loc_2_1394
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_1380:
        cmp    ebx, 0c001000eh
        jne    SHORT loc_2_1391
        mov    byte ptr [edi + 20h], 1
        mov    ebx, 103h
loc_2_1391:
        dec    dword ptr [edi + 1ch]
loc_2_1394:
        cmp    dword ptr [edi + 1ch], 0
        jne    SHORT loc_2_13C9
        mov    eax, dword ptr [edi + 18h]
        cmp    dword ptr [esi + 161h], eax
        jne    SHORT loc_2_13AB
        push   esi
        call   sub_2_1230
loc_2_13AB:
        push   0
        push   75h
        push   edi
        db  0E8h, 077h, 011h, 0FEh, 0FFh
        cmp    dword ptr [esi + 28h], 0
        jne    SHORT loc_2_13D7
        cmp    dword ptr [esi + 2ch], 0
        jne    SHORT loc_2_13D7
        push   esi
        call   sub_2_0680
        jmp    SHORT loc_2_13D7
loc_2_13C9:
        mov    eax, dword ptr [esi + 2ch]
        mov    dword ptr [edi + 0ch], eax
        mov    dword ptr [esi + 2ch], edi
        mov    ebx, 103h
loc_2_13D7:
        mov    eax, dword ptr [esi + 30h]
        dec    eax
        mov    dword ptr [esi + 30h], eax
        jne    SHORT loc_2_1411
        cmp    byte ptr [esi + 142h], 0
        jne    SHORT loc_2_13F8
        cmp    dword ptr [esi + 171h], 0
        jne    SHORT loc_2_13F8
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_2_1411
loc_2_13F8:
        push   1
        add    esi, 17dh
        push   esi
        db  0E8h, 0FCh, 010h, 0FEh, 0FFh
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1411:
        cmp    dword ptr [esi + 4bh], 0
        je     SHORT loc_2_1430
        mov    eax, dword ptr [esi + 53h]
        cmp    dword ptr [esi + eax*4 + 61h], 0
        jne    SHORT loc_2_1430
        cmp    byte ptr [esi + 121h], 0
        jne    SHORT loc_2_1430
        push   esi
        db  0E8h, 010h, 007h, 0FEh, 0FFh
loc_2_1430:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_2_1440:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebx + 8]
        inc    dword ptr [ebx + 1ch]
        inc    dword ptr [edi + 30h]
        cmp    byte ptr [ebx + 20h], 0
        je     SHORT loc_2_1460
        mov    esi, 0c0010002h
        jmp    SHORT loc_2_149C
        db  090h
loc_2_1460:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 10h]
        or     ecx, ecx
        jne    SHORT loc_2_1476
        push   eax
        push   ebx
        push   edi
        call   sub_2_1890
        mov    esi, eax
        jmp    SHORT loc_2_149C
loc_2_1476:
        cmp    ecx, 1
        jne    SHORT loc_2_1497
        cmp    byte ptr [edi + 142h], 0
        je     SHORT loc_2_148B
        mov    esi, 0c001000dh
        jmp    SHORT loc_2_149C
loc_2_148B:
        push   eax
        push   ebx
        push   edi
        call   sub_2_18E0
        mov    esi, eax
        jmp    SHORT loc_2_149C
loc_2_1497:
        mov    esi, 10001h
loc_2_149C:
        dec    dword ptr [ebx + 1ch]
        mov    eax, dword ptr [edi + 30h]
        dec    eax
        mov    dword ptr [edi + 30h], eax
        jne    SHORT loc_2_14D7
        cmp    byte ptr [edi + 142h], 0
        jne    SHORT loc_2_14C0
        cmp    dword ptr [edi + 171h], 0
        jne    SHORT loc_2_14C0
        cmp    dword ptr [edi + 2ch], 0
        je     SHORT loc_2_14D7
loc_2_14C0:
        push   1
        add    edi, 17dh
        push   edi
        db  0E8h, 034h, 010h, 0FEh, 0FFh
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_2_14D7:
        cmp    dword ptr [edi + 4bh], 0
        je     SHORT loc_2_14F6
        mov    eax, dword ptr [edi + 53h]
        cmp    dword ptr [edi + eax*4 + 61h], 0
        jne    SHORT loc_2_14F6
        cmp    byte ptr [edi + 121h], 0
        jne    SHORT loc_2_14F6
        push   edi
        db  0E8h, 04Ah, 006h, 0FEh, 0FFh
loc_2_14F6:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  090h
sub_2_1500:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 18h], 0
        lea    esi, [ebp - 8]
        mov    dword ptr [ebp - 0ch], 0
        mov    edi, 4
        mov    dword ptr [ebp - 14h], 0
        mov    eax, dword ptr [ebp + 10h]
        cmp    eax, 1010101h
        ja     SHORT loc_2_1550
        je     NEAR PTR loc_2_16D0
        sub    eax, 10101h
        cmp    eax, 12h
        ja     SHORT loc_2_1571
        shl    eax, 2
        db  0FFh, 0A0h, 038h, 018h, 000h, 000h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_1550:
        cmp    eax, 1010102h
        je     NEAR PTR loc_2_16F2
        cmp    eax, 1010103h
        je     NEAR PTR loc_2_1714
        cmp    eax, 1010104h
        je     NEAR PTR loc_2_17E6
loc_2_1571:
        mov    dword ptr [ebp - 0ch], 0c00000bbh
        jmp    NEAR PTR loc_2_17EF
        db  08Dh, 049h, 000h
loc_2_1580:
        cmp    byte ptr [ebp + 14h], 0
        jne    SHORT loc_2_1595
        db  0BEh, 02Ch, 028h, 000h, 000h
        mov    edi, 58h
        jmp    NEAR PTR loc_2_17EF
loc_2_1595:
        db  0BEh, 0B4h, 027h, 000h, 000h
        mov    edi, 78h
        jmp    NEAR PTR loc_2_17EF
loc_2_15A4:
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 142h], 0
        mov    dword ptr [ebp - 14h], 2
        jne    SHORT loc_2_15BE
        mov    dword ptr [ebp - 14h], 0
loc_2_15BE:
        lea    esi, [ebp - 14h]
        mov    edi, 4
        jmp    NEAR PTR loc_2_17EF
        db  005h, 000h, 000h, 000h, 000h
loc_2_15D0:
        lea    esi, [ebp - 18h]
        mov    edi, 4
        jmp    NEAR PTR loc_2_17EF
        db  08Dh, 049h, 000h
loc_2_15E0:
        mov    dword ptr [ebp - 8], 0eeh
        jmp    NEAR PTR loc_2_17EF
        db  08Dh, 064h, 024h, 000h
loc_2_15F0:
        mov    dword ptr [ebp - 8], 5dch
        jmp    NEAR PTR loc_2_17EF
        db  08Dh, 064h, 024h, 000h
loc_2_1600:
        mov    dword ptr [ebp - 8], 186a0h
        jmp    NEAR PTR loc_2_17EF
        db  08Dh, 064h, 024h, 000h
loc_2_1610:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 38h]
        shl    eax, 8
        jmp    NEAR PTR loc_2_17EC
        db  08Bh, 0FFh
loc_2_1620:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, 20h
        sub    eax, dword ptr [ebx + 38h]
        shl    eax, 8
        jmp    NEAR PTR loc_2_17EC
loc_2_1633:
        mov    dword ptr [ebp - 8], 100h
        jmp    NEAR PTR loc_2_17EF
        db  090h
loc_2_1640:
        mov    dword ptr [ebp - 8], 100h
        jmp    NEAR PTR loc_2_17EF
        db  08Dh, 064h, 024h, 000h
loc_2_1650:
        db  0BEh, 028h, 029h, 000h, 000h
        mov    edi, 15h
        jmp    NEAR PTR loc_2_17EF
        db  090h
loc_2_1660:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_2_1677
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 14fh]
        mov    eax, dword ptr [eax + 0ch]
        jmp    NEAR PTR loc_2_17EC
loc_2_1677:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        mov    eax, dword ptr [eax + 8]
        jmp    NEAR PTR loc_2_17EC
loc_2_1685:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_2_1699
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 161h]
        jmp    NEAR PTR loc_2_17EC
loc_2_1699:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 18h]
        jmp    NEAR PTR loc_2_17EC
loc_2_16A4:
        mov    word ptr [ebp - 2], 300h
        lea    esi, [ebp - 2]
        mov    edi, 2
        jmp    NEAR PTR loc_2_17EF
loc_2_16B7:
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_2_17EF
loc_2_16C3:
        mov    dword ptr [ebp - 8], 0fh
        jmp    NEAR PTR loc_2_17EF
        db  090h
loc_2_16D0:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 10eh
        push   ebx
        lea    eax, [ebp - 20h]
        push   eax
        db  0E8h, 027h, 00Fh, 0FEh, 0FFh
        lea    esi, [ebp - 20h]
        mov    edi, 6
        jmp    NEAR PTR loc_2_17EF
loc_2_16F2:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 108h
        push   ebx
        lea    eax, [ebp - 20h]
        push   eax
        db  0E8h, 005h, 00Fh, 0FEh, 0FFh
        lea    esi, [ebp - 20h]
        mov    edi, 6
        jmp    NEAR PTR loc_2_17EF
loc_2_1714:
        cmp    byte ptr [ebp + 14h], 0
        mov    ebx, dword ptr [ebp + 8]
        je     SHORT loc_2_1781
        mov    eax, dword ptr [ebx + 14fh]
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        cmp    dword ptr [ebp + 1ch], ecx
        jae    SHORT loc_2_1749
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebp - 0ch], 0c0010014h
        jmp    NEAR PTR loc_2_17EF
loc_2_1749:
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebx + 14fh]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_2_2B90
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_2_17EF
        jmp    NEAR PTR loc_2_17EF
loc_2_1781:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        mov    eax, dword ptr [ebx + 14fh]
        push   eax
        call   sub_2_2A90
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        cmp    dword ptr [ebp + 1ch], ecx
        jae    SHORT loc_2_17B4
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebp - 0ch], 0c0010014h
        jmp    SHORT loc_2_17EF
loc_2_17B4:
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        mov    eax, dword ptr [ebx + 14fh]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_2_2AD0
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_2_17EF
loc_2_17E6:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 41h]
loc_2_17EC:
        mov    dword ptr [ebp - 8], eax
loc_2_17EF:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_2_182B
        cmp    dword ptr [ebp + 10h], 1010103h
        je     SHORT loc_2_182B
        cmp    dword ptr [ebp + 1ch], edi
        jae    SHORT loc_2_181B
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [eax], edi
        mov    dword ptr [ebp - 0ch], 0c0010014h
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
loc_2_181B:
        push   edi
        push   esi
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        db  0E8h, 0E6h, 00Dh, 0FEh, 0FFh
        mov    ecx, dword ptr [ebp + 24h]
        add    dword ptr [ecx], edi
loc_2_182B:
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  090h
loc_2_1838:
        db  080h, 015h, 000h, 000h, 0A4h, 015h, 000h
        db  000h, 0D0h
        db  015h, 000h, 000h, 0D0h, 015h
        add    byte ptr [eax], al
        loopne loc_2_185F
        add    byte ptr [eax], al
        db  0F0h, 015h, 000h, 000h, 000h, 016h, 000h, 000h, 010h, 016h, 000h, 000h, 020h, 016h, 000h, 000h
        db  033h, 016h, 000h
loc_2_185F:
        db  000h, 040h, 016h
        add    byte ptr [eax], al
        db  071h, 015h
        add    byte ptr [eax], al
        push   eax
        push   ss
        add    byte ptr [eax], al
        pushad
        push   ss
        add    byte ptr [eax], al
        test   dword ptr [esi], edx
        add    byte ptr [eax], al
        movsb
        push   ss
        add    byte ptr [eax], al
        mov    bh, 16h
        add    byte ptr [eax], al
        jno    SHORT loc_2_1893
        add    byte ptr [eax], al
        ret
        db  016h, 000h, 000h, 005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1890:
        push   ebp
        mov    ebp, esp
loc_2_1893:
        sub    esp, 8
        push   esi
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 4], 0
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        mov    esi, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [esi + 1ch]
        push   eax
        mov    eax, dword ptr [esi + 18h]
        push   eax
        push   0
        mov    eax, dword ptr [esi + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1500
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 20h], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 24h], ecx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
sub_2_18E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        sub    ebx, ebx
        mov    edi, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [esi + 18h]
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 1010eh
        je     SHORT loc_2_1937
        cmp    eax, 1010fh
        je     NEAR PTR loc_2_1998
        cmp    eax, 10112h
        je     NEAR PTR loc_2_1A20
        cmp    eax, 1010103h
        je     NEAR PTR loc_2_1A50
        mov    ebx, 0c0010017h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    NEAR PTR loc_2_1A91
loc_2_1937:
        cmp    edi, 4
        je     SHORT loc_2_1954
        mov    ebx, 0c0010014h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    NEAR PTR loc_2_1A91
loc_2_1954:
        push   4
        push   ecx
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 0ACh, 00Ch, 0FEh, 0FFh
        test   dword ptr [ebp - 8], 0f050h
        je     SHORT loc_2_1981
        mov    ebx, 0c00000bbh
        mov    dword ptr [esi + 20h], 4
        mov    dword ptr [esi + 24h], 0
        jmp    NEAR PTR loc_2_1A91
loc_2_1981:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   esi
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        mov    edx, dword ptr [ebp + 8]
        push   edx
        call   sub_2_1AB0
        jmp    NEAR PTR loc_2_1A8F
loc_2_1998:
        cmp    edi, 4
        je     SHORT loc_2_19B5
        mov    ebx, 0c0010014h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    NEAR PTR loc_2_1A91
loc_2_19B5:
        push   4
        push   ecx
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 04Bh, 00Ch, 0FEh, 0FFh
        cmp    dword ptr [ebp - 4], 0eeh
        ja     SHORT loc_2_1A19
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 161h]
        cmp    dword ptr [ebp - 4], eax
        jbe    SHORT loc_2_19F0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edx + 161h], eax
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 18h], eax
        jmp    NEAR PTR loc_2_1A91
        db  090h
loc_2_19F0:
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [ecx + 18h], eax
        jne    SHORT loc_2_1A11
        mov    eax, dword ptr [ecx + 18h]
        cmp    dword ptr [ebp - 4], eax
        jae    SHORT loc_2_1A11
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 18h], eax
        push   edx
        call   sub_2_1230
        jmp    NEAR PTR loc_2_1A91
loc_2_1A11:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 18h], eax
        jmp    SHORT loc_2_1A91
loc_2_1A19:
        mov    ebx, 0c0010014h
        jmp    SHORT loc_2_1A91
loc_2_1A20:
        cmp    edi, 4
        je     SHORT loc_2_1A3A
        mov    ebx, 0c0010014h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    SHORT loc_2_1A91
loc_2_1A3A:
        push   4
        push   ecx
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 71h
        push   ecx
        db  0E8h, 0C3h, 00Bh, 0FEh, 0FFh
        sub    ebx, ebx
        jmp    SHORT loc_2_1A91
        db  08Dh, 049h, 000h
loc_2_1A50:
        mov    ebx, 6
        mov    eax, edi
        sub    edx, edx
        div    ebx
        or     edx, edx
        je     SHORT loc_2_1A74
        mov    ebx, 0c0010014h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    SHORT loc_2_1A91
loc_2_1A74:
        push   ecx
        mov    ecx, 6
        mov    eax, edi
        sub    edx, edx
        div    ecx
        push   eax
        push   esi
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        mov    edx, dword ptr [ebp + 8]
        push   edx
        call   sub_2_1B10
loc_2_1A8F:
        mov    ebx, eax
loc_2_1A91:
        or     ebx, ebx
        jne    SHORT loc_2_1A9F
        mov    dword ptr [esi + 20h], edi
        mov    dword ptr [esi + 24h], 0
loc_2_1A9F:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_1AB0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 142h], 0
        jne    SHORT loc_2_1AFA
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 20h], 0
        jne    SHORT loc_2_1AF0
        inc    dword ptr [esi + 1ch]
        push   1
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [esi + 14h]
        push   eax
        mov    eax, dword ptr [ecx + 14fh]
        push   eax
        call   sub_2_2A10
        dec    dword ptr [esi + 1ch]
        pop    esi
        pop    ebp
        ret    10h
        db  08Bh, 0FFh
loc_2_1AF0:
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    10h
loc_2_1AFA:
        mov    eax, 0c001000dh
        pop    esi
        pop    ebp
        ret    10h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1B10:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 142h], 0
        jne    SHORT loc_2_1B60
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 20h], 0
        jne    SHORT loc_2_1B52
        inc    dword ptr [esi + 1ch]
        push   1
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [esi + 14h]
        push   eax
        mov    eax, dword ptr [ecx + 14fh]
        push   eax
        call   sub_2_2730
        dec    dword ptr [esi + 1ch]
        pop    esi
        pop    ebp
        ret    14h
loc_2_1B52:
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h
loc_2_1B60:
        mov    eax, 0c001000dh
        pop    esi
        pop    ebp
        ret    14h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_1B70:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    esi, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        push   1
        mov    eax, dword ptr [esi + 14h]
        push   eax
        push   0
        mov    ebx, dword ptr [ebp + 8]
        push   ebx
        call   sub_2_1500
        mov    edi, eax
        cmp    edi, 0c00000bbh
        jne    NEAR PTR loc_2_1C76
        sub    edi, edi
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 20105h
        ja     SHORT loc_2_1BF6
        je     SHORT loc_2_1C38
        cmp    eax, 20101h
        je     SHORT loc_2_1C12
        cmp    eax, 20102h
        je     SHORT loc_2_1C20
        cmp    eax, 20103h
        je     SHORT loc_2_1C28
        cmp    eax, 20104h
        je     SHORT loc_2_1C30
        mov    edi, 0c0010017h
        jmp    SHORT loc_2_1C59
loc_2_1BF6:
        cmp    eax, 1020101h
        je     SHORT loc_2_1C40
        cmp    eax, 1020102h
        je     SHORT loc_2_1C48
        cmp    eax, 1020103h
        je     SHORT loc_2_1C50
        mov    edi, 0c0010017h
        jmp    SHORT loc_2_1C59
loc_2_1C12:
        mov    eax, dword ptr [ebx + 122h]
        jmp    SHORT loc_2_1C56
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_1C20:
        mov    eax, dword ptr [ebx + 126h]
        jmp    SHORT loc_2_1C56
loc_2_1C28:
        mov    eax, dword ptr [ebx + 12ah]
        jmp    SHORT loc_2_1C56
loc_2_1C30:
        mov    eax, dword ptr [ebx + 13ah]
        jmp    SHORT loc_2_1C56
loc_2_1C38:
        mov    eax, dword ptr [ebx + 13eh]
        jmp    SHORT loc_2_1C56
loc_2_1C40:
        mov    eax, dword ptr [ebx + 136h]
        jmp    SHORT loc_2_1C56
loc_2_1C48:
        mov    eax, dword ptr [ebx + 12eh]
        jmp    SHORT loc_2_1C56
loc_2_1C50:
        mov    eax, dword ptr [ebx + 132h]
loc_2_1C56:
        mov    dword ptr [ebp - 4], eax
loc_2_1C59:
        cmp    dword ptr [ebp - 14h], 4
        jb     SHORT loc_2_1C76
        or     edi, edi
        jne    SHORT loc_2_1C76
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 09Ah, 009h, 0FEh, 0FFh
        add    dword ptr [ebp - 8], 4
loc_2_1C76:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 20h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 24h], eax
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
loc_2_1C90:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0ch]
        sub    esi, esi
        mov    eax, dword ptr [ecx + 14h]
        cmp    eax, 20105h
        ja     SHORT loc_2_1CD0
        cmp    eax, 20101h
        jae    SHORT loc_2_1CF1
        cmp    eax, 10101h
        jb     SHORT loc_2_1CEC
        cmp    eax, 1010bh
        jbe    SHORT loc_2_1CF1
        cmp    eax, 1010dh
        jb     SHORT loc_2_1CEC
        cmp    eax, 10111h
        jbe    SHORT loc_2_1CF1
        cmp    eax, 10113h
        je     SHORT loc_2_1CF1
        jmp    SHORT loc_2_1CEC
loc_2_1CD0:
        cmp    eax, 1010101h
        jb     SHORT loc_2_1CEC
        cmp    eax, 1010104h
        jbe    SHORT loc_2_1CF1
        cmp    eax, 1020101h
        jb     SHORT loc_2_1CEC
        cmp    eax, 1020103h
        jbe    SHORT loc_2_1CF1
loc_2_1CEC:
        mov    esi, 0c0010017h
loc_2_1CF1:
        mov    edi, dword ptr [ebp + 8]
        inc    dword ptr [edi + 30h]
        or     esi, esi
        jne    SHORT loc_2_1D04
        push   ecx
        push   edi
        call   sub_2_1B70
        mov    esi, eax
loc_2_1D04:
        mov    eax, dword ptr [edi + 30h]
        dec    eax
        mov    dword ptr [edi + 30h], eax
        jne    SHORT loc_2_1D40
        cmp    byte ptr [edi + 142h], 0
        jne    SHORT loc_2_1D25
        cmp    dword ptr [edi + 171h], 0
        jne    SHORT loc_2_1D25
        cmp    dword ptr [edi + 2ch], 0
        je     SHORT loc_2_1D40
loc_2_1D25:
        push   1
        add    edi, 17dh
        push   edi
        db  0E8h, 0CFh, 007h, 0FEh, 0FFh
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_2_1D40:
        cmp    dword ptr [edi + 4bh], 0
        je     SHORT loc_2_1D5F
        mov    eax, dword ptr [edi + 53h]
        cmp    dword ptr [edi + eax*4 + 61h], 0
        jne    SHORT loc_2_1D5F
        cmp    byte ptr [edi + 121h], 0
        jne    SHORT loc_2_1D5F
        push   edi
        db  0E8h, 0E1h, 0FDh, 0FDh, 0FFh
loc_2_1D5F:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_1D70:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        db  0A1h, 068h, 027h, 000h, 000h
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 0C7h, 006h, 0FEh, 0FFh
        push   0
        db  0A1h, 06Ch, 027h, 000h, 000h
        push   eax
        db  0E8h, 0AEh, 006h, 0FEh, 0FFh
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_2_1DA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [esi + 1b6h], 1
        db  03Bh, 035h, 0A6h, 027h, 000h, 000h
        jne    SHORT loc_2_1DC3
        mov    eax, dword ptr [esi + 24h]
        db  0A3h, 0A6h, 027h, 000h, 000h
        jmp    SHORT loc_2_1DE1
loc_2_1DC3:
        db  08Bh, 00Dh, 0A6h, 027h, 000h, 000h
        cmp    dword ptr [ecx + 24h], esi
        je     SHORT loc_2_1DD8
        mov    edi, edi
loc_2_1DD0:
        mov    ecx, dword ptr [ecx + 24h]
        cmp    dword ptr [ecx + 24h], esi
        jne    SHORT loc_2_1DD0
loc_2_1DD8:
        mov    eax, dword ptr [ecx + 24h]
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [ecx + 24h], eax
loc_2_1DE1:
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [esi + 1a9h]
        push   eax
        db  0E8h, 017h, 007h, 0FEh, 0FFh
        push   2625a0h
        db  0E8h, 025h, 007h, 0FEh, 0FFh
        mov    eax, dword ptr [esi + 14fh]
        push   eax
        call   sub_2_2550
        lea    eax, [esi + 8]
        push   eax
        db  0E8h, 004h, 007h, 0FEh, 0FFh
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 03Fh, 006h, 0FEh, 0FFh
        push   0
        push   2c3h
        push   esi
        db  0E8h, 006h, 007h, 0FEh, 0FFh
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_2_1E30:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        cmp    byte ptr [edi + 20h], 0
        je     SHORT loc_2_1E53
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1E53:
        inc    dword ptr [edi + 1ch]
        inc    dword ptr [esi + 30h]
        cmp    byte ptr [esi + 142h], 0
        je     SHORT loc_2_1E76
        dec    dword ptr [edi + 1ch]
        dec    dword ptr [esi + 30h]
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1E76:
        mov    ebx, dword ptr [esi + 28h]
        mov    dword ptr [ebp - 4], 0
        or     ebx, ebx
        je     SHORT loc_2_1EA6
loc_2_1E84:
        inc    dword ptr [ebx + 1ch]
        push   4
        lea    eax, [ebp - 4]
        push   eax
        push   40010004h
        mov    eax, dword ptr [ebx]
        push   eax
        db  0E8h, 0F4h, 005h, 0FEh, 0FFh
        mov    eax, dword ptr [ebx + 0ch]
        dec    dword ptr [ebx + 1ch]
        mov    ebx, eax
        or     eax, eax
        jne    SHORT loc_2_1E84
loc_2_1EA6:
        mov    byte ptr [esi + 142h], 1
        mov    dword ptr [esi + 143h], 0
        mov    dword ptr [esi + 147h], edi
        push   esi
        call   sub_2_1F30
        mov    dword ptr [ebp - 4], eax
        dec    dword ptr [edi + 1ch]
        mov    eax, dword ptr [esi + 30h]
        dec    eax
        mov    dword ptr [esi + 30h], eax
        jne    SHORT loc_2_1F04
        cmp    byte ptr [esi + 142h], 0
        jne    SHORT loc_2_1EEA
        cmp    dword ptr [esi + 171h], 0
        jne    SHORT loc_2_1EEA
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_2_1F04
loc_2_1EEA:
        push   1
        add    esi, 17dh
        push   esi
        db  0E8h, 00Ah, 006h, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1F04:
        cmp    dword ptr [esi + 4bh], 0
        je     SHORT loc_2_1F23
        mov    eax, dword ptr [esi + 53h]
        cmp    dword ptr [esi + eax*4 + 61h], 0
        jne    SHORT loc_2_1F23
        cmp    byte ptr [esi + 121h], 0
        jne    SHORT loc_2_1F23
        push   esi
        db  0E8h, 01Dh, 0FCh, 0FDh, 0FFh
loc_2_1F23:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_2_1F30:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 30h]
        cmp    dword ptr [esi + 171h], 0
        je     SHORT loc_2_1F7C
loc_2_1F44:
        mov    ecx, dword ptr [esi + 171h]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 171h], eax
        mov    edi, dword ptr [ecx + 4]
        mov    eax, dword ptr [ecx + 8]
        cmp    eax, 4
        je     SHORT loc_2_1F70
        cmp    eax, 8
        je     SHORT loc_2_1F70
        push   0c001000ch
        push   ecx
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 00Ch, 005h, 0FEh, 0FFh
loc_2_1F70:
        dec    dword ptr [edi + 1ch]
        cmp    dword ptr [esi + 171h], 0
        jne    SHORT loc_2_1F44
loc_2_1F7C:
        cmp    dword ptr [esi + 5bh], -1
        je     SHORT loc_2_1F9A
        mov    dword ptr [esi + 143h], 2
        dec    dword ptr [esi + 30h]
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_2_1F9A:
        dec    dword ptr [esi + 30h]
        push   esi
        call   sub_2_1FB0
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1FB0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        sub    ecx, ecx
        inc    dword ptr [eax + 30h]
        mov    dword ptr [eax + 53h], 0
        mov    dword ptr [eax + 57h], 0
        mov    edx, dword ptr [eax + 38h]
        cmp    edx, ecx
        je     SHORT loc_2_1FE1
        lea    esi, [eax + 61h]
loc_2_1FD4:
        cmp    dword ptr [esi], 0
        jne    SHORT loc_2_1FF0
        add    esi, 4
        inc    ecx
        cmp    ecx, edx
        jb     SHORT loc_2_1FD4
loc_2_1FE1:
        dec    dword ptr [eax + 30h]
        push   eax
        call   sub_2_2010
        pop    esi
        pop    ebp
        ret    4
        db  090h
loc_2_1FF0:
        mov    dword ptr [eax + 143h], 3
        dec    dword ptr [eax + 30h]
        mov    eax, 103h
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2010:
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
        jbe    SHORT loc_2_2060
        lea    edi, [esi + 91h]
loc_2_2032:
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     SHORT loc_2_2052
        mov    ebx, dword ptr [ecx + 28h]
        push   0c001000ch
        push   ecx
        mov    eax, dword ptr [ebx]
        push   eax
        db  0E8h, 021h, 004h, 0FEh, 0FFh
        dec    dword ptr [ebx + 1ch]
        mov    dword ptr [edi], 0
loc_2_2052:
        add    edi, 4
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [esi + 38h], eax
        ja     SHORT loc_2_2032
loc_2_2060:
        mov    edx, dword ptr [esi + 4bh]
        or     edx, edx
        je     SHORT loc_2_208B
loc_2_2067:
        mov    ecx, dword ptr [edx + 24h]
        lea    eax, [edx + 24h]
        mov    dword ptr [esi + 4bh], ecx
        mov    edi, dword ptr [eax + 4]
        push   0c001000ch
        push   edx
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 0E9h, 003h, 0FEh, 0FFh
        dec    dword ptr [edi + 1ch]
        mov    edx, dword ptr [esi + 4bh]
        or     edx, edx
        jne    SHORT loc_2_2067
loc_2_208B:
        mov    edx, dword ptr [esi + 165h]
        or     edx, edx
        je     SHORT loc_2_20BF
loc_2_2095:
        mov    ecx, dword ptr [edx + 24h]
        lea    eax, [edx + 24h]
        mov    dword ptr [esi + 165h], ecx
        mov    edi, dword ptr [eax + 4]
        push   0c001000ch
        push   edx
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 0B8h, 003h, 0FEh, 0FFh
        dec    dword ptr [edi + 1ch]
        mov    edx, dword ptr [esi + 165h]
        or     edx, edx
        jne    SHORT loc_2_2095
loc_2_20BF:
        cmp    byte ptr [esi + 121h], 0
        je     SHORT loc_2_20DB
loc_2_20C8:
        push   2710h
        db  0E8h, 04Eh, 004h, 0FEh, 0FFh
        cmp    byte ptr [esi + 121h], 0
        jne    SHORT loc_2_20C8
loc_2_20DB:
        mov    byte ptr [esi + 160h], 1bh
        push   esi
        call   sub_2_06F0
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0c0000001h
        mov    dword ptr [ebp - 4], eax
        mov    byte ptr [esi + 142h], 0
        mov    edi, dword ptr [esi + 28h]
        or     edi, edi
        je     SHORT loc_2_2142
        sub    ebx, ebx
loc_2_2104:
        inc    dword ptr [edi + 1ch]
        cmp    dword ptr [ebp - 4], ebx
        je     SHORT loc_2_211B
        push   ebx
        push   ebx
        push   40010007h
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 073h, 003h, 0FEh, 0FFh
loc_2_211B:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        push   40010005h
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 060h, 003h, 0FEh, 0FFh
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 05Eh, 003h, 0FEh, 0FFh
        mov    eax, dword ptr [edi + 0ch]
        dec    dword ptr [edi + 1ch]
        mov    edi, eax
        cmp    eax, ebx
        jne    SHORT loc_2_2104
loc_2_2142:
        dec    dword ptr [esi + 30h]
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2160:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [esi + 143h], eax
        jne    NEAR PTR loc_2_224F
        cmp    eax, 1
        je     SHORT loc_2_21E2
        cmp    eax, 2
        je     SHORT loc_2_21EA
        cmp    eax, 3
        je     SHORT loc_2_21F2
        mov    eax, dword ptr [ebp - 4]
loc_2_218C:
        cmp    eax, 103h
        je     SHORT loc_2_21AB
        push   eax
        mov    eax, dword ptr [esi + 147h]
        mov    eax, dword ptr [eax]
        push   eax
        db  0E8h, 0D4h, 002h, 0FEh, 0FFh
        mov    eax, dword ptr [esi + 147h]
        dec    dword ptr [eax + 1ch]
loc_2_21AB:
        mov    eax, dword ptr [esi + 30h]
        dec    eax
        mov    dword ptr [esi + 30h], eax
        jne    SHORT loc_2_2230
        cmp    byte ptr [esi + 142h], 0
        jne    SHORT loc_2_21CC
        cmp    dword ptr [esi + 171h], 0
        jne    SHORT loc_2_21CC
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_2_2230
loc_2_21CC:
        push   1
        add    esi, 17dh
        push   esi
        db  0E8h, 028h, 003h, 0FEh, 0FFh
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_21E2:
        push   esi
        call   sub_2_1F30
        jmp    SHORT loc_2_218C
loc_2_21EA:
        push   esi
        call   sub_2_1FB0
        jmp    SHORT loc_2_218C
loc_2_21F2:
        mov    ecx, dword ptr [esi + 30h]
        inc    ecx
        mov    dword ptr [esi + 30h], ecx
        sub    edx, edx
        mov    eax, dword ptr [esi + 38h]
        cmp    eax, edx
        je     SHORT loc_2_2212
        lea    edi, [esi + 61h]
loc_2_2205:
        cmp    dword ptr [edi], 0
        jne    SHORT loc_2_2221
        add    edi, 4
        inc    edx
        cmp    edx, eax
        jb     SHORT loc_2_2205
loc_2_2212:
        dec    ecx
        mov    dword ptr [esi + 30h], ecx
        push   esi
        call   sub_2_2010
        jmp    NEAR PTR loc_2_218C
loc_2_2221:
        dec    ecx
        mov    dword ptr [esi + 30h], ecx
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
loc_2_2230:
        cmp    dword ptr [esi + 4bh], 0
        je     SHORT loc_2_224F
        mov    eax, dword ptr [esi + 53h]
        cmp    dword ptr [esi + eax*4 + 61h], 0
        jne    SHORT loc_2_224F
        cmp    byte ptr [esi + 121h], 0
        jne    SHORT loc_2_224F
        push   esi
        db  0E8h, 0F1h, 0F8h, 0FDh, 0FFh
loc_2_224F:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_2260:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 18h]
        mov    edx, dword ptr [ebp + 1ch]
        mov    ecx, dword ptr [eax + 8]
        or     edx, edx
        jne    SHORT loc_2_2273
        lea    edx, [eax + 49h]
loc_2_2273:
        cmp    byte ptr [ecx + 142h], 0
        je     SHORT loc_2_2285
        mov    eax, 0c001000dh
        pop    ebp
        ret    1ch
loc_2_2285:
        mov    dword ptr [edx + 4], eax
        inc    dword ptr [eax + 1ch]
        cmp    byte ptr [ebp + 20h], 1
        sbb    eax, eax
        and    eax, 0fffffffah
        add    eax, 0ah
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx], 0
        cmp    dword ptr [ecx + 171h], 0
        jne    SHORT loc_2_22C0
        mov    dword ptr [ecx + 175h], edx
        mov    dword ptr [ecx + 171h], edx
        mov    eax, 103h
        pop    ebp
        ret    1ch
        db  08Bh, 0FFh
loc_2_22C0:
        mov    eax, dword ptr [ecx + 175h]
        pop    ebp
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 175h], edx
        mov    eax, 103h
        ret    1ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_22E0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [eax + 8]
        or     edx, edx
        jne    SHORT loc_2_22F3
        lea    edx, [eax + 21h]
loc_2_22F3:
        cmp    byte ptr [ecx + 142h], 0
        je     SHORT loc_2_2305
        mov    eax, 0c001000dh
        pop    ebp
        ret    14h
loc_2_2305:
        mov    dword ptr [edx + 4], eax
        inc    dword ptr [eax + 1ch]
        cmp    byte ptr [ebp + 18h], 1
        sbb    eax, eax
        add    eax, 9
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx], 0
        cmp    dword ptr [ecx + 171h], 0
        jne    SHORT loc_2_2340
        mov    dword ptr [ecx + 175h], edx
        mov    dword ptr [ecx + 171h], edx
        mov    eax, 103h
        pop    ebp
        ret    14h
        db  005h, 000h, 000h, 000h, 000h
loc_2_2340:
        mov    eax, dword ptr [ecx + 175h]
        pop    ebp
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 175h], edx
        mov    eax, 103h
        ret    14h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_2360:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        dec    dword ptr [eax + 1ch]
        ret    4
        db  08Dh, 049h, 000h
sub_2_2370:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    eax, dword ptr [esi + 14fh]
        mov    edi, dword ptr [eax + 0ch]
        test   edi, 24h
        je     SHORT loc_2_23A0
        db  068h, 070h, 008h, 000h, 000h
        lea    eax, [esi + 8]
        push   eax
        db  0E8h, 082h, 001h, 0FEh, 0FFh
        jmp    SHORT loc_2_23A5
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_23A0:
        call   sub_2_2400
loc_2_23A5:
        test   edi, 26h
        lea    eax, [esi + 15fh]
        je     SHORT loc_2_23B8
        or     byte ptr [eax], 8
        jmp    SHORT loc_2_23BB
loc_2_23B8:
        and    byte ptr [eax], 0f7h
loc_2_23BB:
        test   edi, 20h
        je     SHORT loc_2_23C8
        or     byte ptr [eax], 10h
        jmp    SHORT loc_2_23CB
loc_2_23C8:
        and    byte ptr [eax], 0efh
loc_2_23CB:
        test   edi, 28h
        je     SHORT loc_2_23D8
        or     byte ptr [eax], 4
        jmp    SHORT loc_2_23DB
loc_2_23D8:
        and    byte ptr [eax], 0fbh
loc_2_23DB:
        push   esi
        db  068h, 070h, 008h, 000h, 000h
        add    esi, 8
        push   esi
        db  0E8h, 030h, 001h, 0FEh, 0FFh
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2400:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_07E0
        push   esi
        db  068h, 0B0h, 008h, 000h, 000h
        add    esi, 8
        push   esi
        db  0E8h, 0FEh, 000h, 0FEh, 0FFh
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2430:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        db  08Bh, 01Dh, 040h, 029h, 000h, 000h
        lea    eax, [ebp - 4]
        push   ebx
        push   0
        push   32h
        push   eax
        db  0E8h, 0D9h, 000h, 0FEh, 0FFh
        mov    ecx, dword ptr [ebp + 20h]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     SHORT loc_2_2464
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_2_2464:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0ch
        rep stosd
        stosw
        mov    esi, dword ptr [ebp + 8]
        cmp    esi, eax
        jne    SHORT loc_2_247E
        mov    esi, 2
loc_2_247E:
        push   ebx
        push   0
        lea    eax, [esi + esi*2]
        shl    eax, 2
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 095h, 000h, 0FEh, 0FFh
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [edx + 4], ecx
        or     eax, eax
        je     SHORT loc_2_24B2
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_2550
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_2_24B2:
        push   ebx
        push   0
        lea    eax, [esi*8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        push   eax
        db  0E8h, 05Dh, 000h, 0FEh, 0FFh
        or     eax, eax
        mov    eax, dword ptr [ebp - 4]
        je     SHORT loc_2_24E1
        push   eax
        call   sub_2_2550
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_2_24E1:
        mov    dword ptr [eax + 28h], 0ffffffffh
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx]
        mov    dword ptr [eax + 2ch], edx
        mov    dl, byte ptr [ecx + 4]
        mov    byte ptr [eax + 30h], dl
        add    eax, 2ch
        mov    cl, byte ptr [ecx + 5]
        mov    byte ptr [eax + 5], cl
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 14h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 18h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        mov    dword ptr [eax + 24h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 20h], esi
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_2550:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     SHORT loc_2_2570
        mov    ecx, dword ptr [esi + 20h]
        push   0
        lea    ecx, [ecx + ecx*2]
        shl    ecx, 2
        push   ecx
        push   eax
        db  0E8h, 0BCh, 0FFh, 0FDh, 0FFh
loc_2_2570:
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     SHORT loc_2_2586
        push   0
        mov    ecx, dword ptr [esi + 20h]
        shl    ecx, 3
        push   ecx
        push   eax
        db  0E8h, 0A6h, 0FFh, 0FDh, 0FFh
loc_2_2586:
        push   0
        push   32h
        push   esi
        db  0E8h, 09Ch, 0FFh, 0FDh, 0FFh
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_25A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 28h], 0
        jne    SHORT loc_2_25B9
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_25B9:
        db  0A1h, 040h, 029h, 000h, 000h
        push   eax
        push   0
        push   1ah
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 05Ah, 0FFh, 0FDh, 0FFh
        or     eax, eax
        je     SHORT loc_2_25D9
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_25D9:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 28h]
        mov    edi, edi
loc_2_25E0:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    SHORT loc_2_25F3
        inc    ecx
        cmp    ecx, 20h
        jb     SHORT loc_2_25E0
        jmp    SHORT loc_2_2602
loc_2_25F3:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 28h], eax
loc_2_2602:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 10h]
        mov    dword ptr [eax + 10h], ecx
        mov    eax, dword ptr [esi + 10h]
        or     eax, eax
        je     SHORT loc_2_2618
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], ecx
loc_2_2618:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 10h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [ebp - 8]
        mov    byte ptr [eax + 19h], cl
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0ch], 1
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 4]
        pop    esi
        mov    byte ptr [eax + 18h], 0
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    al, 1
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Bh, 0FFh
sub_2_2670:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_2A10
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_2_2699
        cmp    ebx, 103h
        jne    SHORT loc_2_26B0
loc_2_2699:
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   edi
        push   esi
        call   sub_2_2730
        or     eax, eax
        je     SHORT loc_2_26B0
        mov    ebx, eax
loc_2_26B0:
        or     ebx, ebx
        je     SHORT loc_2_26BC
        cmp    ebx, 103h
        jne    SHORT loc_2_271B
loc_2_26BC:
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_2_2716
        mov    eax, dword ptr [edi + 10h]
        or     eax, eax
        je     SHORT loc_2_26D2
        mov    ecx, dword ptr [edi + 14h]
        mov    dword ptr [eax + 14h], ecx
loc_2_26D2:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_2_26E1
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_2_26E7
loc_2_26E1:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_2_26E7:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_2_26F6
        mov    eax, dword ptr [edi + 4]
        push   eax
        db  0E8h, 092h, 0FDh, 0FDh, 0FFh
loc_2_26F6:
        mov    eax, 1
        mov    cl, byte ptr [edi + 19h]
        shl    eax, cl
        or     dword ptr [esi + 28h], eax
        push   0
        push   1ah
        push   edi
        db  0E8h, 01Fh, 0FEh, 0FDh, 0FFh
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_2_2716:
        mov    ebx, 0c001000eh
loc_2_271B:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2730:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [esi + 20h]
        lea    ecx, [eax + eax*2]
        add    ecx, ecx
        add    ecx, dword ptr [esi + 4]
        mov    dword ptr [ebp - 20h], ecx
        mov    ecx, dword ptr [esi + 8]
        lea    eax, [ecx + eax*4]
        mov    dword ptr [ebp - 24h], eax
        mov    eax, dword ptr [esi + 24h]
        shl    eax, 2
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        db  0E8h, 0C1h, 0FEh, 0FDh, 0FFh
        mov    eax, dword ptr [esi + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        sub    edi, edi
        push   eax
        db  0E8h, 0A9h, 0FEh, 0FDh, 0FFh
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [ebp - 0ch], eax
        cmp    eax, edi
        je     SHORT loc_2_27B4
        sub    eax, eax
        nop
loc_2_2790:
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 1ch], ecx
        mov    edx, dword ptr [ebp + 0ch]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 19h]
        shl    ebx, cl
        not    ebx
        mov    ecx, dword ptr [ebp - 1ch]
        and    dword ptr [eax + ecx], ebx
        add    eax, 4
        inc    edi
        cmp    dword ptr [esi + 24h], edi
        ja     SHORT loc_2_2790
loc_2_27B4:
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [esi + 24h], 0
        jbe    SHORT loc_2_281F
loc_2_27C1:
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + ecx*4], 0
        jne    SHORT loc_2_2814
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        mov    eax, dword ptr [esi + 24h]
        sub    eax, dword ptr [ebp - 4]
        add    ecx, dword ptr [esi + 4]
        lea    eax, [eax + eax*2]
        add    eax, eax
        sub    eax, 6
        push   eax
        lea    eax, [ecx + 6]
        push   eax
        push   ecx
        db  0E8h, 03Eh, 0FEh, 0FDh, 0FFh
        mov    ecx, dword ptr [ebp - 4]
        shl    ecx, 2
        add    ecx, dword ptr [esi + 8]
        mov    eax, dword ptr [esi + 24h]
        sub    eax, dword ptr [ebp - 4]
        lea    eax, [eax*4 - 4]
        push   eax
        lea    eax, [ecx + 4]
        push   eax
        push   ecx
        db  0E8h, 01Dh, 0FEh, 0FDh, 0FFh
        dec    dword ptr [esi + 24h]
        jmp    SHORT loc_2_2817
loc_2_2814:
        inc    dword ptr [ebp - 4]
loc_2_2817:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 24h], eax
        ja     SHORT loc_2_27C1
loc_2_281F:
        mov    dword ptr [ebp - 10h], 0
        cmp    dword ptr [ebp + 14h], 0
        je     NEAR PTR loc_2_290A
        mov    edi, dword ptr [ebp + 18h]
loc_2_2833:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        call   sub_2_2BF0
        or     al, al
        je     SHORT loc_2_286A
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 18h], ecx
        mov    edx, dword ptr [ebp + 0ch]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 19h]
        shl    ebx, cl
        mov    ecx, dword ptr [ebp - 18h]
        or     dword ptr [ecx + eax*4], ebx
        jmp    NEAR PTR loc_2_28F8
loc_2_286A:
        mov    ecx, dword ptr [esi + 24h]
        cmp    dword ptr [esi + 20h], ecx
        jbe    NEAR PTR loc_2_2944
        mov    eax, dword ptr [ebp - 4]
        sub    ecx, dword ptr [ebp - 4]
        lea    eax, [eax + eax*2]
        add    eax, eax
        add    eax, dword ptr [esi + 4]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        push   ecx
        push   eax
        add    eax, 6
        push   eax
        db  0E8h, 098h, 0FDh, 0FDh, 0FFh
        mov    eax, dword ptr [ebp - 4]
        lea    eax, [eax + eax*2]
        add    eax, eax
        add    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [edi]
        mov    dword ptr [eax], ecx
        mov    cl, byte ptr [edi + 4]
        mov    byte ptr [eax + 4], cl
        mov    cl, byte ptr [edi + 5]
        mov    byte ptr [eax + 5], cl
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 2
        add    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [esi + 24h]
        sub    ecx, dword ptr [ebp - 4]
        shl    ecx, 2
        push   ecx
        push   eax
        add    eax, 4
        push   eax
        db  0E8h, 060h, 0FDh, 0FDh, 0FFh
        mov    edx, dword ptr [esi + 8]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edx + eax*4], 0
        mov    ebx, 1
        mov    edx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 14h], edx
        mov    edx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [edx + 19h]
        mov    edx, dword ptr [ebp - 14h]
        shl    ebx, cl
        or     dword ptr [edx + eax*4], ebx
        inc    dword ptr [esi + 24h]
loc_2_28F8:
        add    edi, 6
        inc    dword ptr [ebp - 10h]
        mov    eax, dword ptr [ebp - 10h]
        cmp    eax, dword ptr [ebp + 14h]
        jb     NEAR PTR loc_2_2833
loc_2_290A:
        mov    eax, dword ptr [esi + 24h]
        sub    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 8], eax
        sub    edi, edi
        cmp    dword ptr [ebp - 0ch], edi
        je     SHORT loc_2_2995
        mov    edx, dword ptr [ebp - 20h]
        sub    ecx, ecx
        nop
loc_2_2920:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_299B
        mov    eax, dword ptr [esi + 4]
        add    eax, ecx
        mov    ebx, dword ptr [eax + 2]
        cmp    dword ptr [edx + 2], ebx
        jne    SHORT loc_2_2982
        mov    ax, word ptr [eax]
        cmp    word ptr [edx], ax
        jne    SHORT loc_2_2982
        mov    dword ptr [ebp - 8], 0
        jmp    SHORT loc_2_2989
loc_2_2944:
        mov    eax, dword ptr [ebp - 0ch]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 0D2h, 0FCh, 0FDh, 0FFh
        mov    eax, dword ptr [ebp - 0ch]
        shl    eax, 2
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        db  0E8h, 0BEh, 0FCh, 0FDh, 0FFh
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 24h], eax
        mov    eax, 0c0010009h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_2_2982:
        mov    dword ptr [ebp - 8], 1
loc_2_2989:
        add    edx, 6
        add    ecx, 6
        inc    edi
        cmp    edi, dword ptr [ebp - 0ch]
        jb     SHORT loc_2_2920
loc_2_2995:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_2_2A00
loc_2_299B:
        mov    al, byte ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [edx]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   dword ptr [esi + 14h]
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_2_2A02
        cmp    edi, 103h
        je     SHORT loc_2_2A02
        mov    eax, dword ptr [ebp - 0ch]
        lea    eax, [eax + eax*2]
        lea    ebx, [eax*2]
        push   ebx
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 047h, 0FCh, 0FDh, 0FFh
        push   ebx
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 039h, 0FCh, 0FDh, 0FFh
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Bh, 0FFh
loc_2_2A00:
        sub    edi, edi
loc_2_2A02:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 049h, 000h
sub_2_2A10:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [esi + 0ch]
        mov    ecx, dword ptr [edi + 8]
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [esi + 10h]
        or     eax, eax
        mov    dword ptr [esi + 0ch], 0
        je     SHORT loc_2_2A4D
        lea    esp, [esp]
loc_2_2A40:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [esi + 0ch], ecx
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        jne    SHORT loc_2_2A40
loc_2_2A4D:
        mov    ecx, dword ptr [esi + 0ch]
        cmp    ebx, ecx
        je     SHORT loc_2_2A81
        mov    al, byte ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [edi]
        push   eax
        push   ecx
        push   ebx
        call   dword ptr [esi + 18h]
        or     eax, eax
        je     SHORT loc_2_2A83
        cmp    eax, 103h
        je     SHORT loc_2_2A83
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [esi + 0ch], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_2A81:
        sub    eax, eax
loc_2_2A83:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h
sub_2_2A90:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0ch]
        sub    eax, eax
        mov    cl, byte ptr [ecx + 19h]
        mov    edx, dword ptr [ebp + 8]
        and    ecx, 0ffh
        mov    edi, dword ptr [edx + 24h]
        cmp    edi, eax
        je     SHORT loc_2_2AC2
        mov    esi, dword ptr [edx + 8]
        mov    edx, 1
        shl    edx, cl
loc_2_2AB7:
        test   dword ptr [esi], edx
        je     SHORT loc_2_2ABC
        inc    eax
loc_2_2ABC:
        add    esi, 4
        dec    edi
        jne    SHORT loc_2_2AB7
loc_2_2AC2:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_2AD0:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [ebp + 10h]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 19h]
        mov    ebx, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [ebx + 24h], 0
        jbe    SHORT loc_2_2B4C
        mov    edi, dword ptr [ebp + 1ch]
        mov    dword ptr [ebp - 8], 0
        sub    esi, esi
        mov    eax, 1
        shl    eax, cl
        mov    dword ptr [ebp - 10h], eax
        mov    edi, edi
loc_2_2B10:
        mov    ecx, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp - 10h]
        test   dword ptr [ecx + esi], eax
        je     SHORT loc_2_2B3A
        cmp    dword ptr [ebp + 14h], 6
        jb     SHORT loc_2_2B66
        sub    dword ptr [ebp + 14h], 6
        push   6
        mov    eax, dword ptr [ebx + 4]
        add    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        db  0E8h, 0F8h, 0FAh, 0FDh, 0FFh
        add    edi, 6
        inc    dword ptr [ebp - 0ch]
loc_2_2B3A:
        add    dword ptr [ebp - 8], 6
        add    esi, 4
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebx + 24h], eax
        ja     SHORT loc_2_2B10
loc_2_2B4C:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 18h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [eax], ecx
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_2_2B66:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        mov    dword ptr [eax], 0c0000001h
        mov    eax, dword ptr [ebp + 18h]
        pop    esi
        mov    dword ptr [eax], 0
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2B90:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 24h]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        cmp    ecx, dword ptr [ebp + 10h]
        jbe    SHORT loc_2_2BC0
        mov    dword ptr [eax], 0c0000001h
        mov    eax, dword ptr [ebp + 14h]
        pop    ebp
        mov    dword ptr [eax], 0
        ret    14h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_2BC0:
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [edx + 24h]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [edx + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [edx + 4]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        db  0E8h, 048h, 0FAh, 0FDh, 0FFh
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_2BF0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    ecx, ecx
        mov    esi, dword ptr [ebp + 8]
        mov    edx, esi
        shr    edx, 1
        cmp    esi, ecx
        je     NEAR PTR loc_2_2C89
        dec    esi
        cmp    esi, edx
        jb     SHORT loc_2_2C89
        nop
loc_2_2C10:
        cmp    edx, ecx
        jb     SHORT loc_2_2C89
        mov    eax, dword ptr [ebp + 0ch]
        lea    edi, [edx + edx*2]
        lea    edi, [eax + edi*2]
        mov    eax, dword ptr [ebp + 10h]
        mov    ebx, dword ptr [eax + 2]
        mov    eax, dword ptr [edi + 2]
        cmp    ebx, eax
        jae    SHORT loc_2_2C33
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_2_2C67
loc_2_2C33:
        jbe    SHORT loc_2_2C40
        mov    dword ptr [ebp - 4], 0ffffffffh
        jmp    SHORT loc_2_2C67
        db  08Bh, 0FFh
loc_2_2C40:
        mov    eax, dword ptr [ebp + 10h]
        mov    bx, word ptr [eax]
        mov    ax, word ptr [edi]
        cmp    bx, ax
        jae    SHORT loc_2_2C57
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_2_2C67
loc_2_2C57:
        mov    dword ptr [ebp - 4], 0ffffffffh
        ja     SHORT loc_2_2C67
        mov    dword ptr [ebp - 4], 0
loc_2_2C67:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_2C99
        jle    SHORT loc_2_2C78
        or     edx, edx
        je     SHORT loc_2_2C89
        lea    esi, [edx - 1]
        jmp    SHORT loc_2_2C7B
loc_2_2C78:
        lea    ecx, [edx + 1]
loc_2_2C7B:
        mov    eax, esi
        sub    eax, ecx
        inc    eax
        shr    eax, 1
        lea    edx, [ecx + eax]
        cmp    esi, edx
        jae    SHORT loc_2_2C10
loc_2_2C89:
        mov    eax, dword ptr [ebp + 14h]
        pop    edi
        mov    dword ptr [eax], edx
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_2C99:
        mov    eax, dword ptr [ebp + 14h]
        pop    edi
        mov    dword ptr [eax], edx
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
_PTEXT ends
        end
