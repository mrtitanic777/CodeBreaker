; snip_pcode_02.asm -- SNIP object 2 segment _PTEXT
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
        sub    esp, 4
        push   esi
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
        push   4e20h
        db  0E8h, 09Eh, 024h, 0FEh, 0FFh
        mov    dx, word ptr [esi + 103h]
        in     al, dx
        cmp    al, 21h
        je     SHORT loc_2_0097
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0097:
        mov    cx, word ptr [esi + 103h]
        mov    al, 48h
        lea    edx, [ecx + 0eh]
        out    dx, al
        lea    edx, [ecx + 0fh]
        sub    al, al
        out    dx, al
        lea    edx, [ecx + 7]
        mov    al, 0ffh
        out    dx, al
        lea    edx, [ecx + 8]
        sub    al, al
        out    dx, al
        lea    edx, [ecx + 9]
        out    dx, al
        lea    edx, [ecx + 0ah]
        mov    al, 20h
        out    dx, al
        lea    edx, [ecx + 0bh]
        sub    al, al
        out    dx, al
        mov    dx, cx
        mov    al, 0ah
        out    dx, al
        lea    edx, [ecx + 1fh]
        in     al, dx
        mov    edx, dword ptr [esi + 103h]
        add    dx, 1fh
        mov    al, 0ffh
        out    dx, al
        mov    byte ptr [esi + 120h], 0
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_2_00F0:
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
loc_2_0111:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        push   esi
        db  0E8h, 072h, 002h, 0FEh, 0FFh
        or     al, al
        je     SHORT loc_2_0153
        push   4
        push   edi
        lea    eax, [ebp - 8]
        push   eax
        push   esi
        db  0E8h, 0C1h, 004h, 0FEh, 0FFh
        or     al, al
        je     SHORT loc_2_0153
        mov    al, byte ptr [ebp - 4]
        cmp    byte ptr [ebp - 8], al
        jne    SHORT loc_2_0153
        mov    al, byte ptr [ebp - 3]
        cmp    byte ptr [ebp - 7], al
        jne    SHORT loc_2_0153
        mov    al, byte ptr [ebp - 2]
        cmp    byte ptr [ebp - 6], al
        jne    SHORT loc_2_0153
        mov    al, byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 5], al
        je     SHORT loc_2_0166
loc_2_0153:
        add    edi, 400h
        cmp    edi, 10000h
        jb     SHORT loc_2_0111
        mov    ebx, dword ptr [ebp - 10h]
        jmp    SHORT loc_2_01C0
loc_2_0166:
        mov    ebx, edi
        test   edi, 0ffff0000h
        jne    SHORT loc_2_01C0
loc_2_0170:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        push   ebx
        push   esi
        db  0E8h, 013h, 002h, 0FEh, 0FFh
        or     al, al
        je     SHORT loc_2_01C0
        push   4
        push   ebx
        lea    eax, [ebp - 8]
        push   eax
        push   esi
        db  0E8h, 062h, 004h, 0FEh, 0FFh
        or     al, al
        je     SHORT loc_2_01C0
        mov    al, byte ptr [ebp - 4]
        cmp    byte ptr [ebp - 8], al
        jne    SHORT loc_2_01C0
        mov    al, byte ptr [ebp - 3]
        cmp    byte ptr [ebp - 7], al
        jne    SHORT loc_2_01C0
        mov    al, byte ptr [ebp - 2]
        cmp    byte ptr [ebp - 6], al
        jne    SHORT loc_2_01C0
        mov    al, byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 5], al
        jne    SHORT loc_2_01C0
        add    ebx, 400h
        test   ebx, 0ffff0000h
        je     SHORT loc_2_0170
loc_2_01C0:
        cmp    edi, 10000h
        jae    NEAR PTR loc_2_0271
        cmp    ebx, edi
        je     NEAR PTR loc_2_0271
        test   ebx, 0ffff0000h
        je     SHORT loc_2_01E2
        sub    ebx, 100h
loc_2_01E2:
        mov    dword ptr [ebp - 0ch], edi
        cmp    ebx, edi
        jbe    SHORT loc_2_0237
loc_2_01E9:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        push   esi
        db  0E8h, 097h, 001h, 0FEh, 0FFh
        or     al, al
        je     SHORT loc_2_0250
        push   4
        push   edi
        lea    eax, [ebp - 8]
        push   eax
        push   esi
        db  0E8h, 0E6h, 003h, 0FEh, 0FFh
        or     al, al
        je     SHORT loc_2_025B
        mov    al, byte ptr [ebp - 4]
        cmp    byte ptr [ebp - 8], al
        jne    SHORT loc_2_0266
        mov    al, byte ptr [ebp - 3]
        cmp    byte ptr [ebp - 7], al
        jne    SHORT loc_2_0266
        mov    al, byte ptr [ebp - 2]
        cmp    byte ptr [ebp - 6], al
        jne    SHORT loc_2_0266
        mov    al, byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 5], al
        jne    SHORT loc_2_0266
        add    dword ptr [ebp - 0ch], 4
        cmp    ebx, dword ptr [ebp - 0ch]
        ja     SHORT loc_2_01E9
loc_2_0237:
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
loc_2_0250:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_025B:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0266:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0271:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_2_0280:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        db  0E8h, 0CEh, 004h, 0FEh, 0FFh
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
        db  0E8h, 047h, 022h, 0FEh, 0FFh
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
        db  0E8h, 01Bh, 022h, 0FEh, 0FFh
        mov    dx, word ptr [esi + 103h]
        in     al, dx
        test   al, 1
        jne    SHORT loc_2_0320
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_2_0320:
        mov    cx, word ptr [esi + 103h]
        mov    al, 37h
        lea    edx, [ecx + 0ah]
        out    dx, al
        cmp    byte ptr [esi + 45h], 3
        mov    dx, cx
        mov    al, 0ah
        out    dx, al
        jne    SHORT loc_2_0360
        mov    byte ptr [esi + 120h], 0
loc_2_0340:
        mov    cx, word ptr [esi + 103h]
        sub    al, al
        lea    edx, [ecx + 0fh]
        out    dx, al
        cmp    byte ptr [esi + 120h], al
        mov    dx, cx
        out    dx, al
        lea    edx, [ecx + 0eh]
        je     SHORT loc_2_037B
        mov    al, 48h
        jmp    SHORT loc_2_037D
loc_2_0360:
        push   esi
        call   sub_2_0050
        or     al, al
        jne    SHORT loc_2_0340
        push   esi
        db  0E8h, 0F0h, 003h, 0FEh, 0FFh
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_037B:
        mov    al, 49h
loc_2_037D:
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
loc_2_03A5:
        dec    cl
        je     SHORT loc_2_03B8
        mov    edx, dword ptr [esi + 103h]
        add    dx, 7
        in     al, dx
        test   bl, al
        je     SHORT loc_2_03A5
loc_2_03B8:
        mov    cx, word ptr [esi + 103h]
        mov    al, 2
        lea    edx, [ecx + 0dh]
        out    dx, al
        mov    dx, cx
        mov    al, 22h
        out    dx, al
        push   esi
        call   sub_2_00F0
        or     al, al
        push   esi
        jne    SHORT loc_2_03E6
        db  0E8h, 085h, 003h, 0FEh, 0FFh
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_03E6:
        db  0E8h, 075h, 003h, 0FEh, 0FFh
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0400:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 108h], 0
        jne    NEAR PTR loc_2_04CF
        cmp    byte ptr [esi + 109h], 0
        jne    NEAR PTR loc_2_04CF
        cmp    byte ptr [esi + 10ah], 0
        jne    NEAR PTR loc_2_04CF
        cmp    byte ptr [esi + 10bh], 0
        jne    NEAR PTR loc_2_04CF
        cmp    byte ptr [esi + 10ch], 0
        jne    NEAR PTR loc_2_04CF
        cmp    byte ptr [esi + 10dh], 0
        jne    SHORT loc_2_04CF
        push   1000h
        mov    eax, dword ptr [esi + 46h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        lea    edi, [esi + 46h]
        push   eax
        push   edi
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 083h, 021h, 0FEh, 0FFh
        sub    eax, eax
loc_2_0471:
        mov    ecx, dword ptr [edi]
        inc    eax
        mov    cl, byte ptr [ecx + eax*2 + 0feeh]
        mov    byte ptr [esi + eax + 10dh], cl
        cmp    eax, 6
        jb     SHORT loc_2_0471
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
loc_2_04CF:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_04E0:
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
        je     SHORT loc_2_0508
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0508:
        cmp    byte ptr [ecx + 120h], 0
        mov    edx, dword ptr [ecx + 103h]
        je     SHORT loc_2_0520
        add    dx, 0eh
        mov    al, 48h
        jmp    SHORT loc_2_0526
        db  090h
loc_2_0520:
        add    dx, 0eh
        mov    al, 49h
loc_2_0526:
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
loc_2_059A:
        lea    edx, [edi + esi]
        inc    dx
        mov    al, byte ptr [ecx + edi + 108h]
        out    dx, al
        inc    edi
        cmp    edi, 6
        jb     SHORT loc_2_059A
        mov    eax, dword ptr [ecx + 14fh]
        sub    ebx, ebx
        mov    edi, dword ptr [eax + 0ch]
        and    edi, 4
        add    eax, 0
loc_2_05C0:
        or     edi, edi
        mov    eax, 0ffh
        jne    SHORT loc_2_05D2
        xor    eax, eax
        mov    al, byte ptr [ecx + ebx + 153h]
loc_2_05D2:
        lea    edx, [ebx + esi]
        add    dx, 8
        out    dx, al
        inc    ebx
        cmp    ebx, 8
        jb     SHORT loc_2_05C0
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
sub_2_0610:
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
        db  0E8h, 0EFh, 01Eh, 0FEh, 0FFh
        mov    cx, word ptr [esi + 103h]
        sub    al, al
        lea    edx, [ecx + 0bh]
        out    dx, al
        lea    edx, [ecx + 0ah]
        out    dx, al
        sub    edi, edi
        mov    ebx, 80h
loc_2_0643:
        mov    edx, dword ptr [esi + 103h]
        add    dx, 7
        in     al, dx
        test   bl, al
        jne    SHORT loc_2_0662
        push   3e8h
        db  0E8h, 0C4h, 01Eh, 0FEh, 0FFh
        inc    edi
        cmp    edi, 4
        jb     SHORT loc_2_0643
loc_2_0662:
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
sub_2_0680:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_0610
        push   7d0h
        db  0E8h, 089h, 01Eh, 0FEh, 0FFh
        push   esi
        call   sub_2_04E0
        or     al, al
        jne    SHORT loc_2_06C0
        push   1
        push   1
        push   2
        push   0aa000003h
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 043h, 01Fh, 0FEh, 0FFh
        add    esp, 14h
        sub    al, al
        pop    esi
        pop    ebp
        ret    4
        db  090h
loc_2_06C0:
        mov    edx, dword ptr [esi + 103h]
        add    dx, 0dh
        sub    al, al
        out    dx, al
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_06E0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, 0ffffffffh
        sub    edx, edx
        cmp    dword ptr [ebp + 0ch], edx
        je     SHORT loc_2_072D
loc_2_06F2:
        mov    ecx, dword ptr [ebp + 8]
        mov    bl, byte ptr [edx + ecx]
        mov    esi, 8
        lea    ecx, [ecx]
loc_2_0700:
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
        je     SHORT loc_2_0724
        xor    eax, 4c11db6h
        or     eax, edi
loc_2_0724:
        dec    esi
        jne    SHORT loc_2_0700
        inc    edx
        cmp    edx, dword ptr [ebp + 0ch]
        jb     SHORT loc_2_06F2
loc_2_072D:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0740:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_06E0
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
sub_2_0770:
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
loc_2_0790:
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [esi + ecx + 153h], dl
        inc    dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 8], eax
        jb     SHORT loc_2_0790
        db  068h, 0C0h, 026h, 000h, 000h
        lea    eax, [ebp - 8]
        push   eax
        push   60h
        mov    eax, dword ptr [esi + 14fh]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_2_2B20
        cmp    dword ptr [ebp - 8], 0
        jbe    SHORT loc_2_07F7
        sub    edi, edi
loc_2_07C5:
        dec    dword ptr [ebp - 8]
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [ebp - 2]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        lea    eax, [eax + eax*2]
        db  08Dh, 004h, 045h, 0C0h, 026h, 000h, 000h
        push   eax
        call   sub_2_0740
        xor    eax, eax
        mov    al, byte ptr [ebp - 2]
        mov    cl, byte ptr [ebp - 1]
        or     byte ptr [eax + esi + 153h], cl
        cmp    dword ptr [ebp - 8], edi
        ja     SHORT loc_2_07C5
loc_2_07F7:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_2_0800:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        sub    ecx, ecx
        mov    si, word ptr [eax + 103h]
        mov    dx, si
        mov    al, 62h
        out    dx, al
loc_2_0816:
        lea    edx, [esi + ecx]
        add    dx, 8
        mov    al, 0ffh
        out    dx, al
        inc    ecx
        cmp    ecx, 8
        jb     SHORT loc_2_0816
        mov    dx, si
        mov    al, 22h
        out    dx, al
        sub    al, al
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0840:
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
loc_2_0857:
        lea    edx, [edi + esi]
        add    dx, 8
        mov    al, byte ptr [ecx + esi + 153h]
        out    dx, al
        inc    esi
        cmp    esi, 8
        jb     SHORT loc_2_0857
        mov    dx, di
        mov    al, 22h
        out    dx, al
        sub    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_0880:
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
        db  0E8h, 0C5h, 0FEh, 0FDh, 0FFh
        push   7d0h
        db  0E8h, 07Bh, 01Ch, 0FEh, 0FFh
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
        je     SHORT loc_2_08D1
        mov    byte ptr [edi + 5fh], 1
        push   edi
        db  0E8h, 0AFh, 0FEh, 0FDh, 0FFh
loc_2_08D1:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_08E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        mov    word ptr [ebp - 10h], 5
        mov    word ptr [ebp - 0eh], 5
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
        db  0E8h, 010h, 01Bh, 0FEh, 0FFh
        lea    eax, [ebp - 1ch]
        db  0A3h, 0AEh, 027h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        db  0A3h, 06Ch, 027h, 000h, 000h
        db  0C6h, 005h, 0B2h, 027h, 000h, 000h, 000h
        db  0C7h, 005h, 0A6h, 027h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 070h, 027h, 000h, 000h, 003h
        db  0C6h, 005h, 071h, 027h, 000h, 000h, 00Ah
        db  0C7h, 005h, 072h, 027h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 076h, 027h, 000h, 000h, 0F0h, 00Fh, 000h, 000h
        db  0C7h, 005h, 07Ah, 027h, 000h, 000h, 000h, 012h, 000h, 000h
        db  0C7h, 005h, 07Eh, 027h, 000h, 000h, 020h, 01Ah, 000h, 000h
        db  0C7h, 005h, 082h, 027h, 000h, 000h, 000h, 017h, 000h, 000h
        db  0C7h, 005h, 086h, 027h, 000h, 000h, 0C0h, 01Dh, 000h, 000h
        db  0C7h, 005h, 08Ah, 027h, 000h, 000h, 0D0h, 013h, 000h, 000h
        db  0C7h, 005h, 08Eh, 027h, 000h, 000h, 020h, 01Ch, 000h, 000h
        db  0C7h, 005h, 092h, 027h, 000h, 000h, 000h, 01Dh, 000h, 000h
        db  0C7h, 005h, 096h, 027h, 000h, 000h, 020h, 00Ah, 000h, 000h
        db  0C7h, 005h, 09Ah, 027h, 000h, 000h, 030h, 01Dh, 000h, 000h
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
        db  0E8h, 050h, 01Ah, 0FEh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_2_0A10
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 039h, 01Ah, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 8]
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_0A10:
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_2_0A20:
        push   ebp
        mov    ebp, esp
        sub    esp, 5ch
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 3ch], 7
        mov    word ptr [ebp - 3ah], 7
        db  0C7h, 045h, 0C8h, 0A0h, 028h, 000h, 000h
        mov    word ptr [ebp - 34h], 0ah
        mov    word ptr [ebp - 32h], 0ah
        db  0C7h, 045h, 0D0h, 0B4h, 028h, 000h, 000h
        mov    esi, 300h
        mov    word ptr [ebp - 2ch], 0dh
        mov    word ptr [ebp - 2ah], 0dh
        db  0C7h, 045h, 0D8h, 0D0h, 028h, 000h, 000h
        mov    word ptr [ebp - 44h], 8
        mov    bl, 3
        mov    word ptr [ebp - 42h], 8
        db  0C7h, 045h, 0C0h, 000h, 029h, 000h, 000h
        mov    dword ptr [ebp - 14h], 10h
        db  0A1h, 064h, 027h, 000h, 000h
        push   eax
        lea    eax, [ebp - 8]
        push   0
        push   2c3h
        push   eax
        db  0E8h, 08Dh, 01Ah, 0FEh, 0FFh
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     SHORT loc_2_0AA9
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_0AA9:
        mov    edi, dword ptr [ebp - 8]
        sub    eax, eax
        mov    ecx, 0b0h
        rep stosd
        stosw
        stosb
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 0DBh, 01Ah, 0FEh, 0FFh
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_0AF0
        push   0
        push   2c3h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 04Dh, 01Ah, 0FEh, 0FFh
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h
loc_2_0AF0:
        push   1
        lea    eax, [ebp - 44h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 0A3h, 01Ah, 0FEh, 0FFh
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_2_0B19
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 4]
        mov    byte ptr [ecx + 45h], al
loc_2_0B19:
        push   1
        lea    eax, [ebp - 3ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 07Ah, 01Ah, 0FEh, 0FFh
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_2_0B3C
        mov    eax, dword ptr [ebp - 0ch]
        mov    esi, dword ptr [eax + 4]
loc_2_0B3C:
        push   0
        lea    eax, [ebp - 34h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 057h, 01Ah, 0FEh, 0FFh
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_2_0B5F
        mov    eax, dword ptr [ebp - 0ch]
        mov    bl, byte ptr [eax + 4]
loc_2_0B5F:
        lea    eax, [ebp - 5ah]
        mov    word ptr [ebp - 5ch], 0
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        mov    word ptr [eax + 0ch], 0
        lea    eax, [ebp - 20h]
        push   eax
        lea    eax, [ebp - 24h]
        push   eax
        push   10h
        lea    eax, [ebp - 5ch]
        push   eax
        push   1
        push   edi
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 058h, 019h, 0FEh, 0FFh
        add    esp, 1ch
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_0BB1
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_0BB1:
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 58h]
        mov    dword ptr [ecx + 46h], eax
        lea    eax, [ebp - 2ch]
        push   0
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 0D9h, 019h, 0FEh, 0FFh
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_2_0BE0
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebp - 14h], ecx
loc_2_0BE0:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 0B7h, 018h, 0FEh, 0FFh
        cmp    dword ptr [ebp - 1ch], 6
        jne    SHORT loc_2_0C1F
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_2_0C1F
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 18h]
        mov    eax, edx
        add    ecx, 108h
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    al, byte ptr [edx + 4]
        mov    byte ptr [ecx + 4], al
        mov    al, byte ptr [edx + 5]
        mov    byte ptr [ecx + 5], al
loc_2_0C1F:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 088h, 019h, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 38h], 0ch
        mov    eax, dword ptr [ebp - 8]
        push   0
        mov    dword ptr [eax + 3ch], esi
        mov    eax, dword ptr [ebp - 8]
        push   0
        mov    byte ptr [eax + 40h], bl
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [eax + 41h], ecx
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_0C90
        or     eax, eax
        je     SHORT loc_2_0C7B
        push   0
        push   2c3h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 0BFh, 018h, 0FEh, 0FFh
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_0C7B:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0C90:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 38h], 0ch
        jbe    SHORT loc_2_0CB5
        mov    dword ptr [ebp - 4], 0c000009ah
loc_2_0CA9:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_0CB5:
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
        db  0E8h, 058h, 017h, 0FEh, 0FFh
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        jne    SHORT loc_2_0CA9
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_2_0D22
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        push   1
        push   0aa00000ah
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 0DEh, 018h, 0FEh, 0FFh
        add    esp, 10h
        jmp    NEAR PTR loc_2_0F53
loc_2_0D22:
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
        jne    SHORT loc_2_0D79
        push   0
        push   0aa000004h
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 08Eh, 018h, 0FEh, 0FFh
        add    esp, 0ch
        mov    dword ptr [ebp - 4], 0c0010006h
        jmp    NEAR PTR loc_2_0F53
loc_2_0D79:
        push   esi
        call   sub_2_0280
        or     al, al
        jne    SHORT loc_2_0DA1
        push   0
        push   0aa000004h
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 066h, 018h, 0FEh, 0FFh
        add    esp, 0ch
        mov    dword ptr [ebp - 4], 0c0010006h
        jmp    NEAR PTR loc_2_0F53
loc_2_0DA1:
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
        jbe    SHORT loc_2_0E2F
        lea    eax, [esi + 61h]
        lea    ebx, [ebx]
loc_2_0E20:
        mov    dword ptr [eax], 0
        add    eax, 4
        inc    ecx
        cmp    dword ptr [esi + 38h], ecx
        ja     SHORT loc_2_0E20
loc_2_0E2F:
        mov    byte ptr [esi + 1b5h], 0
        mov    byte ptr [esi + 11fh], 0
        push   esi
        call   sub_2_0400
        lea    ebx, [esi + 14fh]
        push   ebx
        push   esi
        lea    eax, [esi + 108h]
        push   eax
        db  068h, 0F0h, 022h, 000h, 000h
        db  068h, 070h, 022h, 000h, 000h
        db  068h, 0F0h, 021h, 000h, 000h
        mov    eax, dword ptr [esi + 41h]
        push   eax
        call   sub_2_23C0
        or     al, al
        jne    SHORT loc_2_0E90
        push   0
        push   0aa000002h
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 07Bh, 017h, 0FEh, 0FFh
        add    esp, 0ch
        mov    dword ptr [ebp - 4], 0c0000001h
        jmp    NEAR PTR loc_2_0F53
        db  08Dh, 064h, 024h, 000h
loc_2_0E90:
        mov    byte ptr [esi + 160h], 1bh
        db  0C7h, 046h, 020h, 068h, 027h, 000h, 000h
        db  0A1h, 0A6h, 027h, 000h, 000h
        mov    dword ptr [esi + 24h], eax
        db  089h, 035h, 0A6h, 027h, 000h, 000h
        push   esi
        call   sub_2_04E0
        or     al, al
        jne    SHORT loc_2_0ED1
        push   0
        push   0aa000004h
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 033h, 017h, 0FEh, 0FFh
        add    esp, 0ch
        mov    dword ptr [ebp - 4], 0c0010006h
        jmp    SHORT loc_2_0F12
loc_2_0ED1:
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
        db  0E8h, 018h, 016h, 0FEh, 0FFh
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_2_0F70
        push   0
        push   0aa000005h
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 0E9h, 016h, 0FEh, 0FFh
        add    esp, 0ch
loc_2_0F12:
        db  039h, 035h, 0A6h, 027h, 000h, 000h
        jne    SHORT loc_2_0F24
        mov    eax, dword ptr [esi + 24h]
        db  0A3h, 0A6h, 027h, 000h, 000h
        jmp    SHORT loc_2_0F41
loc_2_0F24:
        db  08Bh, 00Dh, 0A6h, 027h, 000h, 000h
        cmp    dword ptr [ecx + 24h], esi
        je     SHORT loc_2_0F38
        nop
loc_2_0F30:
        mov    ecx, dword ptr [ecx + 24h]
        cmp    dword ptr [ecx + 24h], esi
        jne    SHORT loc_2_0F30
loc_2_0F38:
        mov    eax, dword ptr [ecx + 24h]
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [ecx + 24h], eax
loc_2_0F41:
        mov    dx, word ptr [esi + 103h]
        mov    al, 1
        out    dx, al
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_2_24E0
loc_2_0F53:
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 0FDh, 014h, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0F70:
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
        db  0E8h, 02Bh, 015h, 0FEh, 0FFh
        mov    byte ptr [esi + 1b6h], 0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0FF0:
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
loc_2_1013:
        or     edx, edx
        je     SHORT loc_2_1022
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 0
        jne    SHORT loc_2_1013
        mov    dword ptr [ecx], edx
loc_2_1022:
        cmp    dword ptr [ecx], -1
        jne    SHORT loc_2_1034
        mov    eax, 0c0010019h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_2_1034:
        db  0A1h, 064h, 027h, 000h, 000h
        push   eax
        push   0
        push   75h
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 0DFh, 014h, 0FEh, 0FFh
        or     eax, eax
        je     SHORT loc_2_1070
        push   0
        mov    esi, dword ptr [ebp + 20h]
        push   0aa000002h
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 09Ah, 015h, 0FEh, 0FFh
        add    esp, 0ch
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  08Bh, 0FFh
loc_2_1070:
        mov    esi, dword ptr [ebp + 20h]
        inc    dword ptr [esi + 30h]
        mov    ecx, dword ptr [esi + 28h]
        or     ecx, ecx
        jne    SHORT loc_2_1090
        cmp    dword ptr [esi + 2ch], 0
        jne    SHORT loc_2_1090
        mov    edx, dword ptr [esi + 103h]
        add    dx, 0dh
        sub    al, al
        out    dx, al
loc_2_1090:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0ch], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 28h], eax
        cmp    byte ptr [esi + 142h], 0
        jne    NEAR PTR loc_2_1182
        mov    eax, dword ptr [ebp - 4]
        add    eax, 14h
        push   eax
        mov    edi, dword ptr [ebp + 1ch]
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [esi + 14fh]
        push   eax
        call   sub_2_2530
        or     al, al
        je     NEAR PTR loc_2_1182
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
        jne    SHORT loc_2_1159
        cmp    byte ptr [esi + 142h], 0
        jne    SHORT loc_2_1141
        cmp    dword ptr [esi + 171h], 0
        jne    SHORT loc_2_1141
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_2_1159
loc_2_1141:
        push   1
        add    esi, 17dh
        push   esi
        db  0E8h, 0B3h, 013h, 0FEh, 0FFh
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_2_1159:
        cmp    dword ptr [esi + 4bh], 0
        je     SHORT loc_2_1178
        mov    eax, dword ptr [esi + 53h]
        cmp    dword ptr [esi + eax*4 + 61h], 0
        jne    SHORT loc_2_1178
        cmp    byte ptr [esi + 121h], 0
        jne    SHORT loc_2_1178
        push   esi
        db  0E8h, 0C8h, 009h, 0FEh, 0FFh
loc_2_1178:
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_2_1182:
        dec    dword ptr [esi + 30h]
        mov    eax, dword ptr [ebp - 4]
        push   0
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [esi + 28h], eax
        push   75h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 091h, 013h, 0FEh, 0FFh
        push   0
        push   0aa000002h
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 04Dh, 014h, 0FEh, 0FFh
        add    esp, 0ch
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  005h, 000h, 000h, 000h, 000h
sub_2_11C0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        sub    edx, edx
        mov    esi, dword ptr [eax + 28h]
        cmp    esi, edx
        je     SHORT loc_2_11E0
loc_2_11D0:
        mov    ecx, dword ptr [esi + 18h]
        cmp    edx, ecx
        jae    SHORT loc_2_11D9
        mov    edx, ecx
loc_2_11D9:
        mov    esi, dword ptr [esi + 0ch]
        or     esi, esi
        jne    SHORT loc_2_11D0
loc_2_11E0:
        or     edx, edx
        jne    SHORT loc_2_11E9
        mov    edx, 0eeh
loc_2_11E9:
        mov    dword ptr [eax + 161h], edx
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_1200:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        cmp    byte ptr [esi + 20h], 0
        je     SHORT loc_2_1223
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1223:
        inc    dword ptr [edi + 30h]
        inc    dword ptr [esi + 1ch]
        mov    ecx, dword ptr [edi + 28h]
        cmp    ecx, esi
        jne    SHORT loc_2_1238
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [edi + 28h], eax
        jmp    SHORT loc_2_1251
loc_2_1238:
        cmp    dword ptr [ecx + 0ch], esi
        je     SHORT loc_2_1248
loc_2_1240:
        mov    ecx, dword ptr [ecx + 0ch]
        cmp    dword ptr [ecx + 0ch], esi
        jne    SHORT loc_2_1240
loc_2_1248:
        lea    eax, [esi + 0ch]
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [ecx + 0ch], eax
loc_2_1251:
        mov    edx, dword ptr [edi + 103h]
        add    dx, 7
        in     al, dx
        cmp    al, 0ffh
        jne    SHORT loc_2_12BD
        mov    edx, dword ptr [edi + 103h]
        add    dx, 7
        sub    al, al
        out    dx, al
        in     al, dx
        cmp    al, 0ffh
        jne    SHORT loc_2_12B0
        mov    eax, dword ptr [edi + 14fh]
        mov    dword ptr [edi + 2bfh], 1
        mov    dword ptr [eax + 28h], 0ffffffffh
        mov    eax, dword ptr [edi + 14fh]
        push   0
        mov    dword ptr [eax + 10h], 0
        push   75h
        push   esi
        db  0E8h, 08Ch, 012h, 0FEh, 0FFh
        dec    dword ptr [edi + 30h]
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_2_12B0:
        mov    edx, dword ptr [edi + 103h]
        add    dx, 7
        mov    al, 0ffh
        out    dx, al
loc_2_12BD:
        push   0
        mov    eax, dword ptr [esi + 14h]
        push   eax
        mov    eax, dword ptr [edi + 14fh]
        push   eax
        call   sub_2_2600
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_2_12F8
        mov    eax, dword ptr [esi + 1ch]
        cmp    eax, 2
        je     SHORT loc_2_12F0
        mov    byte ptr [esi + 20h], 1
        sub    dword ptr [esi + 1ch], 2
        mov    ebx, 103h
        jmp    SHORT loc_2_1324
        db  08Dh, 064h, 024h, 000h
loc_2_12F0:
        sub    eax, 2
        mov    dword ptr [esi + 1ch], eax
        jmp    SHORT loc_2_1324
loc_2_12F8:
        cmp    ebx, 103h
        jne    SHORT loc_2_1310
        mov    byte ptr [esi + 20h], 1
        sub    dword ptr [esi + 1ch], 2
        jmp    SHORT loc_2_1324
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_1310:
        cmp    ebx, 0c001000eh
        jne    SHORT loc_2_1321
        mov    byte ptr [esi + 20h], 1
        mov    ebx, 103h
loc_2_1321:
        dec    dword ptr [esi + 1ch]
loc_2_1324:
        cmp    dword ptr [esi + 1ch], 0
        jne    SHORT loc_2_1359
        mov    eax, dword ptr [edi + 161h]
        cmp    dword ptr [esi + 18h], eax
        jne    SHORT loc_2_133B
        push   edi
        call   sub_2_11C0
loc_2_133B:
        push   0
        push   75h
        push   esi
        db  0E8h, 0E7h, 011h, 0FEh, 0FFh
        cmp    dword ptr [edi + 28h], 0
        jne    SHORT loc_2_1367
        cmp    dword ptr [edi + 2ch], 0
        jne    SHORT loc_2_1367
        push   edi
        call   sub_2_0610
        jmp    SHORT loc_2_1367
loc_2_1359:
        mov    eax, dword ptr [edi + 2ch]
        mov    dword ptr [esi + 0ch], eax
        mov    dword ptr [edi + 2ch], esi
        mov    ebx, 103h
loc_2_1367:
        mov    eax, dword ptr [edi + 30h]
        dec    eax
        mov    dword ptr [edi + 30h], eax
        jne    SHORT loc_2_13A1
        cmp    byte ptr [edi + 142h], 0
        jne    SHORT loc_2_1388
        cmp    dword ptr [edi + 171h], 0
        jne    SHORT loc_2_1388
        cmp    dword ptr [edi + 2ch], 0
        je     SHORT loc_2_13A1
loc_2_1388:
        push   1
        add    edi, 17dh
        push   edi
        db  0E8h, 06Ch, 011h, 0FEh, 0FFh
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_13A1:
        cmp    dword ptr [edi + 4bh], 0
        je     SHORT loc_2_13C0
        mov    eax, dword ptr [edi + 53h]
        cmp    dword ptr [edi + eax*4 + 61h], 0
        jne    SHORT loc_2_13C0
        cmp    byte ptr [edi + 121h], 0
        jne    SHORT loc_2_13C0
        push   edi
        db  0E8h, 080h, 007h, 0FEh, 0FFh
loc_2_13C0:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_2_13D0:
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
        je     SHORT loc_2_13F0
        mov    esi, 0c0010002h
        jmp    SHORT loc_2_142C
        db  090h
loc_2_13F0:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ecx + 10h]
        or     eax, eax
        jne    SHORT loc_2_1406
        push   ecx
        push   ebx
        push   edi
        call   sub_2_1820
        mov    esi, eax
        jmp    SHORT loc_2_142C
loc_2_1406:
        cmp    eax, 1
        jne    SHORT loc_2_1427
        cmp    byte ptr [edi + 142h], 0
        je     SHORT loc_2_141B
        mov    esi, 0c001000dh
        jmp    SHORT loc_2_142C
loc_2_141B:
        push   ecx
        push   ebx
        push   edi
        call   sub_2_1870
        mov    esi, eax
        jmp    SHORT loc_2_142C
loc_2_1427:
        mov    esi, 10001h
loc_2_142C:
        dec    dword ptr [ebx + 1ch]
        mov    eax, dword ptr [edi + 30h]
        dec    eax
        mov    dword ptr [edi + 30h], eax
        jne    SHORT loc_2_1467
        cmp    byte ptr [edi + 142h], 0
        jne    SHORT loc_2_1450
        cmp    dword ptr [edi + 171h], 0
        jne    SHORT loc_2_1450
        cmp    dword ptr [edi + 2ch], 0
        je     SHORT loc_2_1467
loc_2_1450:
        push   1
        add    edi, 17dh
        push   edi
        db  0E8h, 0A4h, 010h, 0FEh, 0FFh
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_2_1467:
        cmp    dword ptr [edi + 4bh], 0
        je     SHORT loc_2_1486
        mov    eax, dword ptr [edi + 53h]
        cmp    dword ptr [edi + eax*4 + 61h], 0
        jne    SHORT loc_2_1486
        cmp    byte ptr [edi + 121h], 0
        jne    SHORT loc_2_1486
        push   edi
        db  0E8h, 0BAh, 006h, 0FEh, 0FFh
loc_2_1486:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  090h
sub_2_1490:
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
        ja     SHORT loc_2_14E0
        je     NEAR PTR loc_2_1660
        sub    eax, 10101h
        cmp    eax, 12h
        ja     SHORT loc_2_1501
        shl    eax, 2
        db  0FFh, 0A0h, 0C8h, 017h, 000h, 000h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_14E0:
        cmp    eax, 1010102h
        je     NEAR PTR loc_2_1682
        cmp    eax, 1010103h
        je     NEAR PTR loc_2_16A4
        cmp    eax, 1010104h
        je     NEAR PTR loc_2_1776
loc_2_1501:
        mov    dword ptr [ebp - 0ch], 0c00000bbh
        jmp    NEAR PTR loc_2_177F
        db  08Dh, 049h, 000h
loc_2_1510:
        cmp    byte ptr [ebp + 14h], 0
        jne    SHORT loc_2_1525
        db  0BEh, 02Ch, 028h, 000h, 000h
        mov    edi, 58h
        jmp    NEAR PTR loc_2_177F
loc_2_1525:
        db  0BEh, 0B4h, 027h, 000h, 000h
        mov    edi, 78h
        jmp    NEAR PTR loc_2_177F
loc_2_1534:
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 142h], 0
        mov    dword ptr [ebp - 14h], 2
        jne    SHORT loc_2_154E
        mov    dword ptr [ebp - 14h], 0
loc_2_154E:
        lea    esi, [ebp - 14h]
        mov    edi, 4
        jmp    NEAR PTR loc_2_177F
        db  005h, 000h, 000h, 000h, 000h
loc_2_1560:
        lea    esi, [ebp - 18h]
        mov    edi, 4
        jmp    NEAR PTR loc_2_177F
        db  08Dh, 049h, 000h
loc_2_1570:
        mov    dword ptr [ebp - 8], 0eeh
        jmp    NEAR PTR loc_2_177F
        db  08Dh, 064h, 024h, 000h
loc_2_1580:
        mov    dword ptr [ebp - 8], 5dch
        jmp    NEAR PTR loc_2_177F
        db  08Dh, 064h, 024h, 000h
loc_2_1590:
        mov    dword ptr [ebp - 8], 186a0h
        jmp    NEAR PTR loc_2_177F
        db  08Dh, 064h, 024h, 000h
loc_2_15A0:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 38h]
        shl    eax, 8
        jmp    NEAR PTR loc_2_177C
        db  08Bh, 0FFh
loc_2_15B0:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, 20h
        sub    eax, dword ptr [ebx + 38h]
        shl    eax, 8
        jmp    NEAR PTR loc_2_177C
loc_2_15C3:
        mov    dword ptr [ebp - 8], 100h
        jmp    NEAR PTR loc_2_177F
        db  090h
loc_2_15D0:
        mov    dword ptr [ebp - 8], 100h
        jmp    NEAR PTR loc_2_177F
        db  08Dh, 064h, 024h, 000h
loc_2_15E0:
        db  0BEh, 008h, 029h, 000h, 000h
        mov    edi, 24h
        jmp    NEAR PTR loc_2_177F
        db  090h
loc_2_15F0:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_2_1607
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 14fh]
        mov    eax, dword ptr [eax + 0ch]
        jmp    NEAR PTR loc_2_177C
loc_2_1607:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        mov    eax, dword ptr [eax + 8]
        jmp    NEAR PTR loc_2_177C
loc_2_1615:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_2_1629
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 161h]
        jmp    NEAR PTR loc_2_177C
loc_2_1629:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 18h]
        jmp    NEAR PTR loc_2_177C
loc_2_1634:
        mov    word ptr [ebp - 2], 30ah
        lea    esi, [ebp - 2]
        mov    edi, 2
        jmp    NEAR PTR loc_2_177F
loc_2_1647:
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_2_177F
loc_2_1653:
        mov    dword ptr [ebp - 8], 0fh
        jmp    NEAR PTR loc_2_177F
        db  090h
loc_2_1660:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 10eh
        push   ebx
        lea    eax, [ebp - 20h]
        push   eax
        db  0E8h, 097h, 00Fh, 0FEh, 0FFh
        lea    esi, [ebp - 20h]
        mov    edi, 6
        jmp    NEAR PTR loc_2_177F
loc_2_1682:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 108h
        push   ebx
        lea    eax, [ebp - 20h]
        push   eax
        db  0E8h, 075h, 00Fh, 0FEh, 0FFh
        lea    esi, [ebp - 20h]
        mov    edi, 6
        jmp    NEAR PTR loc_2_177F
loc_2_16A4:
        cmp    byte ptr [ebp + 14h], 0
        mov    ebx, dword ptr [ebp + 8]
        je     SHORT loc_2_1711
        mov    eax, dword ptr [ebx + 14fh]
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        cmp    dword ptr [ebp + 1ch], ecx
        jae    SHORT loc_2_16D9
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebp - 0ch], 0c0010014h
        jmp    NEAR PTR loc_2_177F
loc_2_16D9:
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
        call   sub_2_2B20
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_2_177F
        jmp    NEAR PTR loc_2_177F
loc_2_1711:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        mov    eax, dword ptr [ebx + 14fh]
        push   eax
        call   sub_2_2A20
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        cmp    dword ptr [ebp + 1ch], ecx
        jae    SHORT loc_2_1744
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebp - 0ch], 0c0010014h
        jmp    SHORT loc_2_177F
loc_2_1744:
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
        call   sub_2_2A60
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_2_177F
loc_2_1776:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 41h]
loc_2_177C:
        mov    dword ptr [ebp - 8], eax
loc_2_177F:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_2_17BB
        cmp    dword ptr [ebp + 10h], 1010103h
        je     SHORT loc_2_17BB
        cmp    dword ptr [ebp + 1ch], edi
        jae    SHORT loc_2_17AB
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
loc_2_17AB:
        push   edi
        push   esi
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        db  0E8h, 056h, 00Eh, 0FEh, 0FFh
        mov    ecx, dword ptr [ebp + 24h]
        add    dword ptr [ecx], edi
loc_2_17BB:
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  090h
loc_2_17C8:
        db  010h, 015h, 000h, 000h, 034h, 015h
        add    byte ptr [eax], al
        pushad
        db  015h, 000h, 000h, 060h, 015h
        add    byte ptr [eax], al
        db  070h, 015h
        add    byte ptr [eax], al
        db  080h, 015h, 000h, 000h, 090h, 015h, 000h
        db  000h, 0A0h, 015h, 000h, 000h, 0B0h
        db  015h, 000h, 000h, 0C3h, 015h
        add    byte ptr [eax], al
        db  0D0h, 015h, 000h, 000h, 001h, 015h
        add    byte ptr [eax], al
        loopne loc_2_180F
        add    byte ptr [eax], al
        db  0F0h, 015h, 000h, 000h, 015h, 016h, 000h, 000h, 034h, 016h, 000h, 000h, 047h, 016h, 000h, 000h
        db  001h, 015h, 000h
loc_2_180F:
        db  000h, 053h, 016h
        add    byte ptr [eax], al
        add    eax, 0
        lea    esp, [esp]
sub_2_1820:
        push   ebp
        mov    ebp, esp
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
        call   sub_2_1490
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 20h], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 24h], ecx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
sub_2_1870:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 10h]
        sub    ebx, ebx
        mov    esi, dword ptr [edi + 1ch]
        mov    ecx, dword ptr [edi + 18h]
        mov    eax, dword ptr [edi + 14h]
        cmp    eax, 1010eh
        je     SHORT loc_2_18C7
        cmp    eax, 1010fh
        je     NEAR PTR loc_2_1928
        cmp    eax, 10112h
        je     NEAR PTR loc_2_19B0
        cmp    eax, 1010103h
        je     NEAR PTR loc_2_19E0
        mov    ebx, 0c0010017h
        mov    dword ptr [edi + 20h], 0
        mov    dword ptr [edi + 24h], 0
        jmp    NEAR PTR loc_2_1A21
loc_2_18C7:
        cmp    esi, 4
        je     SHORT loc_2_18E4
        mov    ebx, 0c0010014h
        mov    dword ptr [edi + 20h], 0
        mov    dword ptr [edi + 24h], 0
        jmp    NEAR PTR loc_2_1A21
loc_2_18E4:
        push   4
        push   ecx
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 01Ch, 00Dh, 0FEh, 0FFh
        test   dword ptr [ebp - 8], 0f050h
        je     SHORT loc_2_1911
        mov    ebx, 0c00000bbh
        mov    dword ptr [edi + 20h], 4
        mov    dword ptr [edi + 24h], 0
        jmp    NEAR PTR loc_2_1A21
loc_2_1911:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        mov    edx, dword ptr [ebp + 8]
        push   edx
        call   sub_2_1A40
        jmp    NEAR PTR loc_2_1A1F
loc_2_1928:
        cmp    esi, 4
        je     SHORT loc_2_1945
        mov    ebx, 0c0010014h
        mov    dword ptr [edi + 20h], 0
        mov    dword ptr [edi + 24h], 0
        jmp    NEAR PTR loc_2_1A21
loc_2_1945:
        push   4
        push   ecx
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 0BBh, 00Ch, 0FEh, 0FFh
        cmp    dword ptr [ebp - 4], 0eeh
        ja     SHORT loc_2_19A9
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 161h]
        cmp    dword ptr [ebp - 4], eax
        jbe    SHORT loc_2_1980
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edx + 161h], eax
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 18h], eax
        jmp    NEAR PTR loc_2_1A21
        db  090h
loc_2_1980:
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [ecx + 18h], eax
        jne    SHORT loc_2_19A1
        mov    eax, dword ptr [ecx + 18h]
        cmp    dword ptr [ebp - 4], eax
        jae    SHORT loc_2_19A1
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 18h], eax
        push   edx
        call   sub_2_11C0
        jmp    NEAR PTR loc_2_1A21
loc_2_19A1:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 18h], eax
        jmp    SHORT loc_2_1A21
loc_2_19A9:
        mov    ebx, 0c0010014h
        jmp    SHORT loc_2_1A21
loc_2_19B0:
        cmp    esi, 4
        je     SHORT loc_2_19CA
        mov    ebx, 0c0010014h
        mov    dword ptr [edi + 20h], 0
        mov    dword ptr [edi + 24h], 0
        jmp    SHORT loc_2_1A21
loc_2_19CA:
        push   4
        push   ecx
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 71h
        push   ecx
        db  0E8h, 033h, 00Ch, 0FEh, 0FFh
        sub    ebx, ebx
        jmp    SHORT loc_2_1A21
        db  08Dh, 049h, 000h
loc_2_19E0:
        mov    ebx, 6
        mov    eax, esi
        sub    edx, edx
        div    ebx
        or     edx, edx
        je     SHORT loc_2_1A04
        mov    ebx, 0c0010014h
        mov    dword ptr [edi + 20h], 0
        mov    dword ptr [edi + 24h], 0
        jmp    SHORT loc_2_1A21
loc_2_1A04:
        push   ecx
        mov    ecx, 6
        mov    eax, esi
        sub    edx, edx
        div    ecx
        push   eax
        push   edi
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        mov    edx, dword ptr [ebp + 8]
        push   edx
        call   sub_2_1AA0
loc_2_1A1F:
        mov    ebx, eax
loc_2_1A21:
        or     ebx, ebx
        jne    SHORT loc_2_1A2F
        mov    dword ptr [edi + 20h], esi
        mov    dword ptr [edi + 24h], 0
loc_2_1A2F:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_1A40:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 142h], 0
        jne    SHORT loc_2_1A8A
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 20h], 0
        jne    SHORT loc_2_1A80
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
        call   sub_2_29A0
        dec    dword ptr [esi + 1ch]
        pop    esi
        pop    ebp
        ret    10h
        db  08Bh, 0FFh
loc_2_1A80:
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    10h
loc_2_1A8A:
        mov    eax, 0c001000dh
        pop    esi
        pop    ebp
        ret    10h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1AA0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 142h], 0
        jne    SHORT loc_2_1AF0
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 20h], 0
        jne    SHORT loc_2_1AE2
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
        call   sub_2_26C0
        dec    dword ptr [esi + 1ch]
        pop    esi
        pop    ebp
        ret    14h
loc_2_1AE2:
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h
loc_2_1AF0:
        mov    eax, 0c001000dh
        pop    esi
        pop    ebp
        ret    14h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_1B00:
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
        call   sub_2_1490
        mov    edi, eax
        cmp    edi, 0c00000bbh
        jne    NEAR PTR loc_2_1C06
        sub    edi, edi
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 20105h
        ja     SHORT loc_2_1B86
        je     SHORT loc_2_1BC8
        cmp    eax, 20101h
        je     SHORT loc_2_1BA2
        cmp    eax, 20102h
        je     SHORT loc_2_1BB0
        cmp    eax, 20103h
        je     SHORT loc_2_1BB8
        cmp    eax, 20104h
        je     SHORT loc_2_1BC0
        mov    edi, 0c0010017h
        jmp    SHORT loc_2_1BE9
loc_2_1B86:
        cmp    eax, 1020101h
        je     SHORT loc_2_1BD0
        cmp    eax, 1020102h
        je     SHORT loc_2_1BD8
        cmp    eax, 1020103h
        je     SHORT loc_2_1BE0
        mov    edi, 0c0010017h
        jmp    SHORT loc_2_1BE9
loc_2_1BA2:
        mov    eax, dword ptr [ebx + 122h]
        jmp    SHORT loc_2_1BE6
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_1BB0:
        mov    eax, dword ptr [ebx + 126h]
        jmp    SHORT loc_2_1BE6
loc_2_1BB8:
        mov    eax, dword ptr [ebx + 12ah]
        jmp    SHORT loc_2_1BE6
loc_2_1BC0:
        mov    eax, dword ptr [ebx + 13ah]
        jmp    SHORT loc_2_1BE6
loc_2_1BC8:
        mov    eax, dword ptr [ebx + 13eh]
        jmp    SHORT loc_2_1BE6
loc_2_1BD0:
        mov    eax, dword ptr [ebx + 136h]
        jmp    SHORT loc_2_1BE6
loc_2_1BD8:
        mov    eax, dword ptr [ebx + 12eh]
        jmp    SHORT loc_2_1BE6
loc_2_1BE0:
        mov    eax, dword ptr [ebx + 132h]
loc_2_1BE6:
        mov    dword ptr [ebp - 4], eax
loc_2_1BE9:
        cmp    dword ptr [ebp - 14h], 4
        jb     SHORT loc_2_1C06
        or     edi, edi
        jne    SHORT loc_2_1C06
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 00Ah, 00Ah, 0FEh, 0FFh
        add    dword ptr [ebp - 8], 4
loc_2_1C06:
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
loc_2_1C20:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0ch]
        sub    esi, esi
        mov    eax, dword ptr [ecx + 14h]
        cmp    eax, 20105h
        ja     SHORT loc_2_1C60
        cmp    eax, 20101h
        jae    SHORT loc_2_1C81
        cmp    eax, 10101h
        jb     SHORT loc_2_1C7C
        cmp    eax, 1010bh
        jbe    SHORT loc_2_1C81
        cmp    eax, 1010dh
        jb     SHORT loc_2_1C7C
        cmp    eax, 10111h
        jbe    SHORT loc_2_1C81
        cmp    eax, 10113h
        je     SHORT loc_2_1C81
        jmp    SHORT loc_2_1C7C
loc_2_1C60:
        cmp    eax, 1010101h
        jb     SHORT loc_2_1C7C
        cmp    eax, 1010104h
        jbe    SHORT loc_2_1C81
        cmp    eax, 1020101h
        jb     SHORT loc_2_1C7C
        cmp    eax, 1020103h
        jbe    SHORT loc_2_1C81
loc_2_1C7C:
        mov    esi, 0c0010017h
loc_2_1C81:
        mov    edi, dword ptr [ebp + 8]
        inc    dword ptr [edi + 30h]
        or     esi, esi
        jne    SHORT loc_2_1C94
        push   ecx
        push   edi
        call   sub_2_1B00
        mov    esi, eax
loc_2_1C94:
        mov    eax, dword ptr [edi + 30h]
        dec    eax
        mov    dword ptr [edi + 30h], eax
        jne    SHORT loc_2_1CD0
        cmp    byte ptr [edi + 142h], 0
        jne    SHORT loc_2_1CB5
        cmp    dword ptr [edi + 171h], 0
        jne    SHORT loc_2_1CB5
        cmp    dword ptr [edi + 2ch], 0
        je     SHORT loc_2_1CD0
loc_2_1CB5:
        push   1
        add    edi, 17dh
        push   edi
        db  0E8h, 03Fh, 008h, 0FEh, 0FFh
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_2_1CD0:
        cmp    dword ptr [edi + 4bh], 0
        je     SHORT loc_2_1CEF
        mov    eax, dword ptr [edi + 53h]
        cmp    dword ptr [edi + eax*4 + 61h], 0
        jne    SHORT loc_2_1CEF
        cmp    byte ptr [edi + 121h], 0
        jne    SHORT loc_2_1CEF
        push   edi
        db  0E8h, 051h, 0FEh, 0FDh, 0FFh
loc_2_1CEF:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_1D00:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        db  0A1h, 068h, 027h, 000h, 000h
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 037h, 007h, 0FEh, 0FFh
        push   0
        db  0A1h, 06Ch, 027h, 000h, 000h
        push   eax
        db  0E8h, 01Eh, 007h, 0FEh, 0FFh
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_2_1D30:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [esi + 1b6h], 1
        db  03Bh, 035h, 0A6h, 027h, 000h, 000h
        jne    SHORT loc_2_1D53
        mov    eax, dword ptr [esi + 24h]
        db  0A3h, 0A6h, 027h, 000h, 000h
        jmp    SHORT loc_2_1D71
loc_2_1D53:
        db  08Bh, 00Dh, 0A6h, 027h, 000h, 000h
        cmp    dword ptr [ecx + 24h], esi
        je     SHORT loc_2_1D68
        mov    edi, edi
loc_2_1D60:
        mov    ecx, dword ptr [ecx + 24h]
        cmp    dword ptr [ecx + 24h], esi
        jne    SHORT loc_2_1D60
loc_2_1D68:
        mov    eax, dword ptr [ecx + 24h]
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [ecx + 24h], eax
loc_2_1D71:
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [esi + 1a9h]
        push   eax
        db  0E8h, 087h, 007h, 0FEh, 0FFh
        push   2625a0h
        db  0E8h, 095h, 007h, 0FEh, 0FFh
        mov    eax, dword ptr [esi + 14fh]
        push   eax
        call   sub_2_24E0
        lea    eax, [esi + 8]
        push   eax
        db  0E8h, 074h, 007h, 0FEh, 0FFh
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 0AFh, 006h, 0FEh, 0FFh
        push   0
        push   2c3h
        push   esi
        db  0E8h, 076h, 007h, 0FEh, 0FFh
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_2_1DC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        cmp    byte ptr [edi + 20h], 0
        je     SHORT loc_2_1DE3
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1DE3:
        inc    dword ptr [edi + 1ch]
        inc    dword ptr [esi + 30h]
        cmp    byte ptr [esi + 142h], 0
        je     SHORT loc_2_1E06
        dec    dword ptr [edi + 1ch]
        dec    dword ptr [esi + 30h]
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1E06:
        mov    ebx, dword ptr [esi + 28h]
        mov    dword ptr [ebp - 4], 0
        or     ebx, ebx
        je     SHORT loc_2_1E36
loc_2_1E14:
        inc    dword ptr [ebx + 1ch]
        push   4
        lea    eax, [ebp - 4]
        push   eax
        push   40010004h
        mov    eax, dword ptr [ebx]
        push   eax
        db  0E8h, 064h, 006h, 0FEh, 0FFh
        mov    eax, dword ptr [ebx + 0ch]
        dec    dword ptr [ebx + 1ch]
        mov    ebx, eax
        or     eax, eax
        jne    SHORT loc_2_1E14
loc_2_1E36:
        mov    byte ptr [esi + 142h], 1
        mov    dword ptr [esi + 143h], 0
        mov    dword ptr [esi + 147h], edi
        push   esi
        call   sub_2_1EC0
        mov    dword ptr [ebp - 4], eax
        dec    dword ptr [edi + 1ch]
        mov    eax, dword ptr [esi + 30h]
        dec    eax
        mov    dword ptr [esi + 30h], eax
        jne    SHORT loc_2_1E94
        cmp    byte ptr [esi + 142h], 0
        jne    SHORT loc_2_1E7A
        cmp    dword ptr [esi + 171h], 0
        jne    SHORT loc_2_1E7A
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_2_1E94
loc_2_1E7A:
        push   1
        add    esi, 17dh
        push   esi
        db  0E8h, 07Ah, 006h, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1E94:
        cmp    dword ptr [esi + 4bh], 0
        je     SHORT loc_2_1EB3
        mov    eax, dword ptr [esi + 53h]
        cmp    dword ptr [esi + eax*4 + 61h], 0
        jne    SHORT loc_2_1EB3
        cmp    byte ptr [esi + 121h], 0
        jne    SHORT loc_2_1EB3
        push   esi
        db  0E8h, 08Dh, 0FCh, 0FDh, 0FFh
loc_2_1EB3:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_2_1EC0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 30h]
        cmp    dword ptr [esi + 171h], 0
        je     SHORT loc_2_1F0C
loc_2_1ED4:
        mov    ecx, dword ptr [esi + 171h]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 171h], eax
        mov    edi, dword ptr [ecx + 4]
        mov    eax, dword ptr [ecx + 8]
        cmp    eax, 4
        je     SHORT loc_2_1F00
        cmp    eax, 8
        je     SHORT loc_2_1F00
        push   0c001000ch
        push   ecx
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 07Ch, 005h, 0FEh, 0FFh
loc_2_1F00:
        dec    dword ptr [edi + 1ch]
        cmp    dword ptr [esi + 171h], 0
        jne    SHORT loc_2_1ED4
loc_2_1F0C:
        cmp    dword ptr [esi + 5bh], -1
        je     SHORT loc_2_1F2A
        mov    dword ptr [esi + 143h], 2
        dec    dword ptr [esi + 30h]
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_2_1F2A:
        dec    dword ptr [esi + 30h]
        push   esi
        call   sub_2_1F40
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1F40:
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
        je     SHORT loc_2_1F71
        lea    esi, [eax + 61h]
loc_2_1F64:
        cmp    dword ptr [esi], 0
        jne    SHORT loc_2_1F80
        add    esi, 4
        inc    ecx
        cmp    edx, ecx
        ja     SHORT loc_2_1F64
loc_2_1F71:
        dec    dword ptr [eax + 30h]
        push   eax
        call   sub_2_1FA0
        pop    esi
        pop    ebp
        ret    4
        db  090h
loc_2_1F80:
        mov    dword ptr [eax + 143h], 3
        dec    dword ptr [eax + 30h]
        mov    eax, 103h
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1FA0:
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
        jbe    SHORT loc_2_1FF0
        lea    edi, [esi + 91h]
loc_2_1FC2:
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     SHORT loc_2_1FE2
        mov    ebx, dword ptr [ecx + 28h]
        push   0c001000ch
        push   ecx
        mov    eax, dword ptr [ebx]
        push   eax
        db  0E8h, 091h, 004h, 0FEh, 0FFh
        dec    dword ptr [ebx + 1ch]
        mov    dword ptr [edi], 0
loc_2_1FE2:
        add    edi, 4
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [esi + 38h], eax
        ja     SHORT loc_2_1FC2
loc_2_1FF0:
        mov    edx, dword ptr [esi + 4bh]
        or     edx, edx
        je     SHORT loc_2_201B
loc_2_1FF7:
        mov    ecx, dword ptr [edx + 24h]
        lea    eax, [edx + 24h]
        mov    dword ptr [esi + 4bh], ecx
        mov    edi, dword ptr [eax + 4]
        push   0c001000ch
        push   edx
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 059h, 004h, 0FEh, 0FFh
        dec    dword ptr [edi + 1ch]
        mov    edx, dword ptr [esi + 4bh]
        or     edx, edx
        jne    SHORT loc_2_1FF7
loc_2_201B:
        mov    edx, dword ptr [esi + 165h]
        or     edx, edx
        je     SHORT loc_2_204F
loc_2_2025:
        mov    ecx, dword ptr [edx + 24h]
        lea    eax, [edx + 24h]
        mov    dword ptr [esi + 165h], ecx
        mov    edi, dword ptr [eax + 4]
        push   0c001000ch
        push   edx
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 028h, 004h, 0FEh, 0FFh
        dec    dword ptr [edi + 1ch]
        mov    edx, dword ptr [esi + 165h]
        or     edx, edx
        jne    SHORT loc_2_2025
loc_2_204F:
        cmp    byte ptr [esi + 121h], 0
        je     SHORT loc_2_206B
loc_2_2058:
        push   2710h
        db  0E8h, 0BEh, 004h, 0FEh, 0FFh
        cmp    byte ptr [esi + 121h], 0
        jne    SHORT loc_2_2058
loc_2_206B:
        mov    byte ptr [esi + 160h], 1bh
        push   esi
        call   sub_2_0680
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0c0000001h
        mov    dword ptr [ebp - 4], eax
        mov    byte ptr [esi + 142h], 0
        mov    edi, dword ptr [esi + 28h]
        or     edi, edi
        je     SHORT loc_2_20D2
        sub    ebx, ebx
loc_2_2094:
        inc    dword ptr [edi + 1ch]
        cmp    dword ptr [ebp - 4], ebx
        je     SHORT loc_2_20AB
        push   ebx
        push   ebx
        push   40010007h
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 0E3h, 003h, 0FEh, 0FFh
loc_2_20AB:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        push   40010005h
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 0D0h, 003h, 0FEh, 0FFh
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 0CEh, 003h, 0FEh, 0FFh
        mov    eax, dword ptr [edi + 0ch]
        dec    dword ptr [edi + 1ch]
        mov    edi, eax
        cmp    eax, ebx
        jne    SHORT loc_2_2094
loc_2_20D2:
        dec    dword ptr [esi + 30h]
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_20F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [esi + 143h], eax
        jne    NEAR PTR loc_2_21DF
        cmp    eax, 1
        je     SHORT loc_2_2172
        cmp    eax, 2
        je     SHORT loc_2_217A
        cmp    eax, 3
        je     SHORT loc_2_2182
        mov    eax, dword ptr [ebp - 4]
loc_2_211C:
        cmp    eax, 103h
        je     SHORT loc_2_213B
        push   eax
        mov    eax, dword ptr [esi + 147h]
        mov    eax, dword ptr [eax]
        push   eax
        db  0E8h, 044h, 003h, 0FEh, 0FFh
        mov    eax, dword ptr [esi + 147h]
        dec    dword ptr [eax + 1ch]
loc_2_213B:
        mov    eax, dword ptr [esi + 30h]
        dec    eax
        mov    dword ptr [esi + 30h], eax
        jne    SHORT loc_2_21C0
        cmp    byte ptr [esi + 142h], 0
        jne    SHORT loc_2_215C
        cmp    dword ptr [esi + 171h], 0
        jne    SHORT loc_2_215C
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_2_21C0
loc_2_215C:
        push   1
        add    esi, 17dh
        push   esi
        db  0E8h, 098h, 003h, 0FEh, 0FFh
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_2172:
        push   esi
        call   sub_2_1EC0
        jmp    SHORT loc_2_211C
loc_2_217A:
        push   esi
        call   sub_2_1F40
        jmp    SHORT loc_2_211C
loc_2_2182:
        mov    ecx, dword ptr [esi + 30h]
        inc    ecx
        mov    dword ptr [esi + 30h], ecx
        sub    edx, edx
        mov    eax, dword ptr [esi + 38h]
        cmp    eax, edx
        je     SHORT loc_2_21A2
        lea    edi, [esi + 61h]
loc_2_2195:
        cmp    dword ptr [edi], 0
        jne    SHORT loc_2_21B1
        add    edi, 4
        inc    edx
        cmp    eax, edx
        ja     SHORT loc_2_2195
loc_2_21A2:
        dec    ecx
        mov    dword ptr [esi + 30h], ecx
        push   esi
        call   sub_2_1FA0
        jmp    NEAR PTR loc_2_211C
loc_2_21B1:
        dec    ecx
        mov    dword ptr [esi + 30h], ecx
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
loc_2_21C0:
        cmp    dword ptr [esi + 4bh], 0
        je     SHORT loc_2_21DF
        mov    eax, dword ptr [esi + 53h]
        cmp    dword ptr [esi + eax*4 + 61h], 0
        jne    SHORT loc_2_21DF
        cmp    byte ptr [esi + 121h], 0
        jne    SHORT loc_2_21DF
        push   esi
        db  0E8h, 061h, 0F9h, 0FDh, 0FFh
loc_2_21DF:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_21F0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 18h]
        mov    edx, dword ptr [ebp + 1ch]
        mov    ecx, dword ptr [eax + 8]
        or     edx, edx
        jne    SHORT loc_2_2203
        lea    edx, [eax + 49h]
loc_2_2203:
        cmp    byte ptr [ecx + 142h], 0
        je     SHORT loc_2_2215
        mov    eax, 0c001000dh
        pop    ebp
        ret    1ch
loc_2_2215:
        mov    dword ptr [edx + 4], eax
        inc    dword ptr [eax + 1ch]
        cmp    byte ptr [ebp + 20h], 1
        sbb    eax, eax
        and    eax, 0fffffffah
        add    eax, 0ah
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx], 0
        cmp    dword ptr [ecx + 171h], 0
        jne    SHORT loc_2_2250
        mov    dword ptr [ecx + 175h], edx
        mov    dword ptr [ecx + 171h], edx
        mov    eax, 103h
        pop    ebp
        ret    1ch
        db  08Bh, 0FFh
loc_2_2250:
        mov    eax, dword ptr [ecx + 175h]
        pop    ebp
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 175h], edx
        mov    eax, 103h
        ret    1ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_2270:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [eax + 8]
        or     edx, edx
        jne    SHORT loc_2_2283
        lea    edx, [eax + 21h]
loc_2_2283:
        cmp    byte ptr [ecx + 142h], 0
        je     SHORT loc_2_2295
        mov    eax, 0c001000dh
        pop    ebp
        ret    14h
loc_2_2295:
        mov    dword ptr [edx + 4], eax
        inc    dword ptr [eax + 1ch]
        cmp    byte ptr [ebp + 18h], 1
        sbb    eax, eax
        add    eax, 9
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx], 0
        cmp    dword ptr [ecx + 171h], 0
        jne    SHORT loc_2_22D0
        mov    dword ptr [ecx + 175h], edx
        mov    dword ptr [ecx + 171h], edx
        mov    eax, 103h
        pop    ebp
        ret    14h
        db  005h, 000h, 000h, 000h, 000h
loc_2_22D0:
        mov    eax, dword ptr [ecx + 175h]
        pop    ebp
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 175h], edx
        mov    eax, 103h
        ret    14h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_22F0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        dec    dword ptr [eax + 1ch]
        ret    4
        db  08Dh, 049h, 000h
sub_2_2300:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    eax, dword ptr [esi + 14fh]
        mov    edi, dword ptr [eax + 0ch]
        test   edi, 24h
        je     SHORT loc_2_2330
        db  068h, 000h, 008h, 000h, 000h
        lea    eax, [esi + 8]
        push   eax
        db  0E8h, 0F2h, 001h, 0FEh, 0FFh
        jmp    SHORT loc_2_2335
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_2330:
        call   sub_2_2390
loc_2_2335:
        test   edi, 26h
        lea    eax, [esi + 15fh]
        je     SHORT loc_2_2348
        or     byte ptr [eax], 8
        jmp    SHORT loc_2_234B
loc_2_2348:
        and    byte ptr [eax], 0f7h
loc_2_234B:
        test   edi, 20h
        je     SHORT loc_2_2358
        or     byte ptr [eax], 10h
        jmp    SHORT loc_2_235B
loc_2_2358:
        and    byte ptr [eax], 0efh
loc_2_235B:
        test   edi, 28h
        je     SHORT loc_2_2368
        or     byte ptr [eax], 4
        jmp    SHORT loc_2_236B
loc_2_2368:
        and    byte ptr [eax], 0fbh
loc_2_236B:
        push   esi
        db  068h, 070h, 008h, 000h, 000h
        add    esi, 8
        push   esi
        db  0E8h, 0A0h, 001h, 0FEh, 0FFh
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2390:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_0770
        push   esi
        db  068h, 040h, 008h, 000h, 000h
        add    esi, 8
        push   esi
        db  0E8h, 06Eh, 001h, 0FEh, 0FFh
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_23C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        db  08Bh, 01Dh, 030h, 029h, 000h, 000h
        lea    eax, [ebp - 4]
        push   ebx
        push   0
        push   32h
        push   eax
        db  0E8h, 049h, 001h, 0FEh, 0FFh
        mov    ecx, dword ptr [ebp + 20h]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     SHORT loc_2_23F4
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_2_23F4:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0ch
        rep stosd
        stosw
        mov    esi, dword ptr [ebp + 8]
        cmp    esi, eax
        jne    SHORT loc_2_240E
        mov    esi, 2
loc_2_240E:
        push   ebx
        push   0
        lea    eax, [esi + esi*2]
        shl    eax, 2
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 005h, 001h, 0FEh, 0FFh
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [edx + 4], ecx
        or     eax, eax
        je     SHORT loc_2_2442
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_24E0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_2_2442:
        push   ebx
        push   0
        lea    eax, [esi*8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        push   eax
        db  0E8h, 0CDh, 000h, 0FEh, 0FFh
        or     eax, eax
        mov    eax, dword ptr [ebp - 4]
        je     SHORT loc_2_2471
        push   eax
        call   sub_2_24E0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_2_2471:
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
sub_2_24E0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     SHORT loc_2_2500
        mov    ecx, dword ptr [esi + 20h]
        push   0
        lea    ecx, [ecx + ecx*2]
        shl    ecx, 2
        push   ecx
        push   eax
        db  0E8h, 02Ch, 000h, 0FEh, 0FFh
loc_2_2500:
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     SHORT loc_2_2516
        push   0
        mov    ecx, dword ptr [esi + 20h]
        shl    ecx, 3
        push   ecx
        push   eax
        db  0E8h, 016h, 000h, 0FEh, 0FFh
loc_2_2516:
        push   0
        push   32h
        push   esi
        db  0E8h, 00Ch, 000h, 0FEh, 0FFh
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2530:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 28h], 0
        jne    SHORT loc_2_2549
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_2549:
        db  0A1h, 030h, 029h, 000h, 000h
        push   eax
        push   0
        push   1ah
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 0CAh, 0FFh, 0FDh, 0FFh
        or     eax, eax
        je     SHORT loc_2_2569
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_2569:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 28h]
        mov    edi, edi
loc_2_2570:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    SHORT loc_2_2583
        inc    ecx
        cmp    ecx, 20h
        jb     SHORT loc_2_2570
        jmp    SHORT loc_2_2592
loc_2_2583:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 28h], eax
loc_2_2592:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 10h]
        mov    dword ptr [eax + 10h], ecx
        mov    eax, dword ptr [esi + 10h]
        or     eax, eax
        je     SHORT loc_2_25A8
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], ecx
loc_2_25A8:
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
sub_2_2600:
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
        call   sub_2_29A0
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_2_2629
        cmp    ebx, 103h
        jne    SHORT loc_2_2640
loc_2_2629:
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   edi
        push   esi
        call   sub_2_26C0
        or     eax, eax
        je     SHORT loc_2_2640
        mov    ebx, eax
loc_2_2640:
        or     ebx, ebx
        je     SHORT loc_2_264C
        cmp    ebx, 103h
        jne    SHORT loc_2_26AB
loc_2_264C:
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_2_26A6
        mov    eax, dword ptr [edi + 10h]
        or     eax, eax
        je     SHORT loc_2_2662
        mov    ecx, dword ptr [edi + 14h]
        mov    dword ptr [eax + 14h], ecx
loc_2_2662:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_2_2671
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_2_2677
loc_2_2671:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_2_2677:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_2_2686
        mov    eax, dword ptr [edi + 4]
        push   eax
        db  0E8h, 002h, 0FEh, 0FDh, 0FFh
loc_2_2686:
        mov    eax, 1
        mov    cl, byte ptr [edi + 19h]
        shl    eax, cl
        or     dword ptr [esi + 28h], eax
        push   0
        push   1ah
        push   edi
        db  0E8h, 08Fh, 0FEh, 0FDh, 0FFh
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_2_26A6:
        mov    ebx, 0c001000eh
loc_2_26AB:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_26C0:
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
        db  0E8h, 031h, 0FFh, 0FDh, 0FFh
        mov    eax, dword ptr [esi + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        sub    edi, edi
        push   eax
        db  0E8h, 019h, 0FFh, 0FDh, 0FFh
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [ebp - 0ch], eax
        cmp    eax, edi
        je     SHORT loc_2_2744
        sub    eax, eax
        nop
loc_2_2720:
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
        ja     SHORT loc_2_2720
loc_2_2744:
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [esi + 24h], 0
        jbe    SHORT loc_2_27AF
loc_2_2751:
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + ecx*4], 0
        jne    SHORT loc_2_27A4
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
        db  0E8h, 0AEh, 0FEh, 0FDh, 0FFh
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
        db  0E8h, 08Dh, 0FEh, 0FDh, 0FFh
        dec    dword ptr [esi + 24h]
        jmp    SHORT loc_2_27A7
loc_2_27A4:
        inc    dword ptr [ebp - 4]
loc_2_27A7:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 24h], eax
        ja     SHORT loc_2_2751
loc_2_27AF:
        mov    dword ptr [ebp - 10h], 0
        cmp    dword ptr [ebp + 14h], 0
        je     NEAR PTR loc_2_289A
        mov    edi, dword ptr [ebp + 18h]
loc_2_27C3:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        call   sub_2_2B80
        or     al, al
        je     SHORT loc_2_27FA
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 18h], ecx
        mov    edx, dword ptr [ebp + 0ch]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 19h]
        shl    ebx, cl
        mov    ecx, dword ptr [ebp - 18h]
        or     dword ptr [ecx + eax*4], ebx
        jmp    NEAR PTR loc_2_2888
loc_2_27FA:
        mov    ecx, dword ptr [esi + 24h]
        cmp    dword ptr [esi + 20h], ecx
        jbe    NEAR PTR loc_2_28D4
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
        db  0E8h, 008h, 0FEh, 0FDh, 0FFh
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
        db  0E8h, 0D0h, 0FDh, 0FDh, 0FFh
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
loc_2_2888:
        add    edi, 6
        inc    dword ptr [ebp - 10h]
        mov    eax, dword ptr [ebp - 10h]
        cmp    eax, dword ptr [ebp + 14h]
        jb     NEAR PTR loc_2_27C3
loc_2_289A:
        mov    eax, dword ptr [esi + 24h]
        sub    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 8], eax
        sub    edi, edi
        cmp    dword ptr [ebp - 0ch], edi
        je     SHORT loc_2_2925
        mov    edx, dword ptr [ebp - 20h]
        sub    ecx, ecx
        nop
loc_2_28B0:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_292B
        mov    eax, dword ptr [esi + 4]
        add    eax, ecx
        mov    ebx, dword ptr [eax + 2]
        cmp    dword ptr [edx + 2], ebx
        jne    SHORT loc_2_2912
        mov    ax, word ptr [eax]
        cmp    word ptr [edx], ax
        jne    SHORT loc_2_2912
        mov    dword ptr [ebp - 8], 0
        jmp    SHORT loc_2_2919
loc_2_28D4:
        mov    eax, dword ptr [ebp - 0ch]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 042h, 0FDh, 0FDh, 0FFh
        mov    eax, dword ptr [ebp - 0ch]
        shl    eax, 2
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        db  0E8h, 02Eh, 0FDh, 0FDh, 0FFh
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 24h], eax
        mov    eax, 0c0010009h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_2_2912:
        mov    dword ptr [ebp - 8], 1
loc_2_2919:
        add    edx, 6
        add    ecx, 6
        inc    edi
        cmp    edi, dword ptr [ebp - 0ch]
        jb     SHORT loc_2_28B0
loc_2_2925:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_2_2990
loc_2_292B:
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
        je     SHORT loc_2_2992
        cmp    edi, 103h
        je     SHORT loc_2_2992
        mov    eax, dword ptr [ebp - 0ch]
        lea    eax, [eax + eax*2]
        lea    ebx, [eax*2]
        push   ebx
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 0B7h, 0FCh, 0FDh, 0FFh
        push   ebx
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 0A9h, 0FCh, 0FDh, 0FFh
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Bh, 0FFh
loc_2_2990:
        sub    edi, edi
loc_2_2992:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 049h, 000h
sub_2_29A0:
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
        je     SHORT loc_2_29DD
        lea    esp, [esp]
loc_2_29D0:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [esi + 0ch], ecx
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        jne    SHORT loc_2_29D0
loc_2_29DD:
        mov    ecx, dword ptr [esi + 0ch]
        cmp    ebx, ecx
        je     SHORT loc_2_2A11
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
        je     SHORT loc_2_2A13
        cmp    eax, 103h
        je     SHORT loc_2_2A13
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [esi + 0ch], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_2A11:
        sub    eax, eax
loc_2_2A13:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h
sub_2_2A20:
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
        je     SHORT loc_2_2A52
        mov    esi, dword ptr [edx + 8]
        mov    edx, 1
        shl    edx, cl
loc_2_2A47:
        test   dword ptr [esi], edx
        je     SHORT loc_2_2A4C
        inc    eax
loc_2_2A4C:
        add    esi, 4
        dec    edi
        jne    SHORT loc_2_2A47
loc_2_2A52:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_2A60:
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
        jbe    SHORT loc_2_2ADC
        mov    edi, dword ptr [ebp + 1ch]
        mov    dword ptr [ebp - 8], 0
        sub    esi, esi
        mov    eax, 1
        shl    eax, cl
        mov    dword ptr [ebp - 10h], eax
        mov    edi, edi
loc_2_2AA0:
        mov    ecx, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp - 10h]
        test   dword ptr [ecx + esi], eax
        je     SHORT loc_2_2ACA
        cmp    dword ptr [ebp + 14h], 6
        jb     SHORT loc_2_2AF6
        sub    dword ptr [ebp + 14h], 6
        push   6
        mov    eax, dword ptr [ebx + 4]
        add    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        db  0E8h, 068h, 0FBh, 0FDh, 0FFh
        add    edi, 6
        inc    dword ptr [ebp - 0ch]
loc_2_2ACA:
        add    dword ptr [ebp - 8], 6
        add    esi, 4
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebx + 24h], eax
        ja     SHORT loc_2_2AA0
loc_2_2ADC:
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
loc_2_2AF6:
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
sub_2_2B20:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 24h]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        cmp    ecx, dword ptr [ebp + 10h]
        jbe    SHORT loc_2_2B50
        mov    dword ptr [eax], 0c0000001h
        mov    eax, dword ptr [ebp + 14h]
        pop    ebp
        mov    dword ptr [eax], 0
        ret    14h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_2B50:
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
        db  0E8h, 0B8h, 0FAh, 0FDh, 0FFh
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_2B80:
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
        je     NEAR PTR loc_2_2C19
        dec    esi
        cmp    esi, edx
        jb     SHORT loc_2_2C19
        nop
loc_2_2BA0:
        cmp    edx, ecx
        jb     SHORT loc_2_2C19
        mov    eax, dword ptr [ebp + 0ch]
        lea    edi, [edx + edx*2]
        lea    edi, [eax + edi*2]
        mov    eax, dword ptr [ebp + 10h]
        mov    ebx, dword ptr [eax + 2]
        mov    eax, dword ptr [edi + 2]
        cmp    ebx, eax
        jae    SHORT loc_2_2BC3
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_2_2BF7
loc_2_2BC3:
        jbe    SHORT loc_2_2BD0
        mov    dword ptr [ebp - 4], 0ffffffffh
        jmp    SHORT loc_2_2BF7
        db  08Bh, 0FFh
loc_2_2BD0:
        mov    eax, dword ptr [ebp + 10h]
        mov    bx, word ptr [eax]
        mov    ax, word ptr [edi]
        cmp    bx, ax
        jae    SHORT loc_2_2BE7
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_2_2BF7
loc_2_2BE7:
        mov    dword ptr [ebp - 4], 0ffffffffh
        ja     SHORT loc_2_2BF7
        mov    dword ptr [ebp - 4], 0
loc_2_2BF7:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_2C29
        jle    SHORT loc_2_2C08
        or     edx, edx
        je     SHORT loc_2_2C19
        lea    esi, [edx - 1]
        jmp    SHORT loc_2_2C0B
loc_2_2C08:
        lea    ecx, [edx + 1]
loc_2_2C0B:
        mov    eax, esi
        sub    eax, ecx
        inc    eax
        shr    eax, 1
        lea    edx, [ecx + eax]
        cmp    esi, edx
        jae    SHORT loc_2_2BA0
loc_2_2C19:
        mov    eax, dword ptr [ebp + 14h]
        pop    edi
        mov    dword ptr [eax], edx
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_2C29:
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
