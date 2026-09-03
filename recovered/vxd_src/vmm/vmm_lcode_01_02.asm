; vmm_lcode_01_02.asm -- VMM object 1 segment _LTEXT  (part 2/4)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_4A74:
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebx, ebx
        db  08Bh, 035h, 0B0h, 0F8h, 000h, 000h
        cmp    esi, ebx
        je     NEAR PTR loc_1_4B81
        db  0A1h, 0D8h, 0FCh, 000h, 000h
        cmp    word ptr [eax], 20h
        jl     NEAR PTR loc_1_4B81
        db  0A1h, 0D4h, 0FCh, 000h, 000h
        test   byte ptr [eax + 1], 2
        jne    NEAR PTR loc_1_4B81
        db  08Bh, 00Dh, 018h, 0EFh, 000h, 000h
        mov    eax, ecx
        db  02Bh, 005h, 088h, 0EEh, 000h, 000h
        jns    SHORT loc_1_4ABA
        sub    ecx, ecx
        jmp    SHORT loc_1_4AC0
loc_1_4ABA:
        db  02Bh, 00Dh, 088h, 0EEh, 000h, 000h
loc_1_4AC0:
        db  08Bh, 015h, 084h, 0EEh, 000h, 000h
        db  0A1h, 064h, 0EFh, 000h, 000h
        and    eax, ecx
        sub    edx, eax
        db  03Bh, 015h, 060h, 0EFh, 000h, 000h
        jle    NEAR PTR loc_1_4B81
        db  08Bh, 00Dh, 0E4h, 0F7h, 000h, 000h
        db  08Bh, 015h, 068h, 0FAh, 000h, 000h
        db  0BDh, 070h, 056h, 000h, 000h
loc_1_4AEC:
        mov    eax, dword ptr [esi + 8]
        and    eax, 0fffffh
        imul   eax, eax, 0dh
        lea    esi, [ecx + eax]
        cmp    edx, esi
        je     SHORT loc_1_4B77
        mov    edi, dword ptr [esi]
        test   byte ptr [edi + 3], 2
        jne    SHORT loc_1_4AEC
        test   byte ptr [edi + 3], 40h
        je     SHORT loc_1_4AEC
        movzx  eax, byte ptr [edi + 2]
        db  08Bh, 004h, 085h, 008h, 0FCh, 000h, 000h
        cmp    dword ptr [eax + 0ch], ebp
        jne    SHORT loc_1_4AEC
        db  089h, 035h, 0B0h, 0F8h, 000h, 000h
        call   sub_1_590D
        mov    ebx, eax
        mov    dword ptr [esi + 4], ebx
        lea    ebp, [esi + 4]
        or     byte ptr [edi + 3], 20h
        push   esi
        call   sub_1_4E68
        mov    esi, eax
        dec    ebx
        shl    ebx, 0ch
        push   ebx
        push   esi
        db  0FFh, 015h, 094h, 0F6h, 000h, 000h
        add    esp, 8
        mov    ebx, eax
        push   esi
        call   sub_1_4EB1
        or     ebx, ebx
        jne    SHORT loc_1_4B5C
        and    byte ptr [edi + 3], 0bfh
        jmp    SHORT loc_1_4B81
loc_1_4B5C:
        push   0
        push   0
        push   ebp
        call   sub_1_58BE
        add    esp, 0ch
        cmp    ebx, 1
        je     SHORT loc_1_4B81
        db  0C6h, 005h, 044h, 0F9h, 000h, 000h, 000h
        jmp    SHORT loc_1_4B81
loc_1_4B77:
        db  0C7h, 005h, 0B0h, 0F8h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_4B81:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_4B88:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    byte ptr [esp + 13h], 0
        db  083h, 03Dh, 050h, 0F9h, 000h, 000h, 040h
        jge    SHORT loc_1_4BA4
        push   40h
        call   sub_1_4ED2
loc_1_4BA4:
        db  08Bh, 035h, 0E4h, 0F7h, 000h, 000h
loc_1_4BAA:
        db  0F6h, 005h, 030h, 0F9h, 000h, 000h, 008h
        jne    SHORT loc_1_4BCE
        db  0A1h, 098h, 0F6h, 000h, 000h
        cmp    dword ptr [eax - 28h], 0
        jne    SHORT loc_1_4BC7
        db  083h, 03Dh, 0D0h, 0F6h, 000h, 000h, 000h
        je     SHORT loc_1_4BCE
loc_1_4BC7:
        mov    byte ptr [esp + 12h], 1
        jmp    SHORT loc_1_4BD3
loc_1_4BCE:
        mov    byte ptr [esp + 12h], 0
loc_1_4BD3:
        cmp    byte ptr [esp + 12h], 0
        jne    SHORT loc_1_4BFE
        cmp    byte ptr [esp + 13h], 0
        jne    SHORT loc_1_4BFE
        db  0A1h, 018h, 0EFh, 000h, 000h
        db  02Bh, 005h, 01Ch, 0EFh, 000h, 000h
        cdq
        sub    eax, edx
        sar    eax, 1
        db  03Bh, 005h, 0F4h, 0F8h, 000h, 000h
        mov    byte ptr [esp + 11h], 1
        jl     SHORT loc_1_4C03
loc_1_4BFE:
        mov    byte ptr [esp + 11h], 0
loc_1_4C03:
        db  0A1h, 060h, 0FAh, 000h, 000h
        mov    eax, dword ptr [eax + 9]
        shr    eax, 0ch
        db  03Bh, 005h, 0A0h, 0FDh, 000h, 000h
        je     SHORT loc_1_4C21
        cmp    byte ptr [esp + 11h], 0
        je     NEAR PTR loc_1_4CE2
loc_1_4C21:
        cmp    byte ptr [esp + 11h], 0
        jne    SHORT loc_1_4C50
        call   sub_1_7382
        or     eax, eax
        db  08Bh, 00Dh, 0C0h, 0F8h, 000h, 000h
        je     SHORT loc_1_4C50
        push   ecx
        and    eax, 0fffff3ffh
        shr    eax, 0ah
        sub    eax, 800000h
        push   eax
        call   sub_1_3B30
        jmp    NEAR PTR loc_1_4BAA
loc_1_4C50:
        db  08Bh, 01Dh, 068h, 0FAh, 000h, 000h
loc_1_4C56:
        mov    eax, dword ptr [ebx + 8]
        and    eax, 0fffffh
        imul   eax, eax, 0dh
        lea    ebx, [esi + eax]
        db  039h, 01Dh, 068h, 0FAh, 000h, 000h
        je     SHORT loc_1_4CB5
        mov    edi, dword ptr [ebx]
        movzx  eax, byte ptr [edi + 2]
        db  08Bh, 02Ch, 085h, 008h, 0FCh, 000h, 000h
        test   byte ptr [edi + 3], 2
        jne    SHORT loc_1_4C56
        cmp    byte ptr [esp + 12h], 0
        je     SHORT loc_1_4C8C
        test   byte ptr [ebp + 1ch], 2
        jne    SHORT loc_1_4C56
loc_1_4C8C:
        cmp    byte ptr [esp + 11h], 0
        je     SHORT loc_1_4C99
        test   byte ptr [ebp + 1ch], 2
        je     SHORT loc_1_4C56
loc_1_4C99:
        or     byte ptr [edi + 3], 2
        push   ebp
        push   edi
        call   sub_1_5AB7
        or     eax, eax
        jne    SHORT loc_1_4D01
        mov    byte ptr [esp + 13h], 1
        push   edi
        call   sub_1_36BD
        jmp    SHORT loc_1_4C56
loc_1_4CB5:
        db  08Bh, 03Dh, 050h, 0F9h, 000h, 000h
        db  0A1h, 050h, 0F9h, 000h, 000h
        inc    eax
        push   eax
        call   sub_1_4ED2
        db  03Bh, 03Dh, 050h, 0F9h, 000h, 000h
        jne    NEAR PTR loc_1_4BAA
        push   1f4h
        call   sub_1_3717
        jmp    NEAR PTR loc_1_4BAA
loc_1_4CE2:
        db  0A1h, 060h, 0FAh, 000h, 000h
        mov    eax, dword ptr [eax + 9]
        shr    eax, 0ch
        imul   eax, eax, 0dh
        lea    ebx, [esi + eax]
        db  0FFh, 00Dh, 040h, 0F9h, 000h, 000h
        push   ebx
        call   sub_1_4E1F
        jmp    SHORT loc_1_4D74
loc_1_4D01:
        db  0A1h, 0D4h, 0F7h, 000h, 000h
        inc    eax
        db  0A3h, 0D4h, 0F7h, 000h, 000h
        db  0A1h, 098h, 0FDh, 000h, 000h
        db  089h, 03Ch, 085h, 030h, 0FDh, 000h, 000h
        inc    eax
        db  0A3h, 098h, 0FDh, 000h, 000h
        cmp    eax, 1ah
        jl     SHORT loc_1_4D2D
        db  0C7h, 005h, 098h, 0FDh, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_4D2D:
        and    byte ptr [edi + 3], 0e7h
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [edi + 6], eax
        db  039h, 01Dh, 0B0h, 0F8h, 000h, 000h
        jne    SHORT loc_1_4D49
        db  0A1h, 068h, 0FAh, 000h, 000h
        db  0A3h, 0B0h, 0F8h, 000h, 000h
loc_1_4D49:
        push   ebx
        call   sub_1_4E1F
        push   edi
        call   sub_1_36BD
        test   byte ptr [ebp + 1ch], 2
        je     SHORT loc_1_4D66
        db  0A1h, 0F4h, 0F8h, 000h, 000h
        dec    eax
        db  0A3h, 0F4h, 0F8h, 000h, 000h
loc_1_4D66:
        db  080h, 03Dh, 044h, 0F9h, 000h, 000h, 000h
        je     SHORT loc_1_4D74
        call   sub_1_4A74
loc_1_4D74:
        mov    dword ptr [ebx], 0fffff155h
        mov    word ptr [ebx + 0ah], 0
        mov    word ptr [ebx + 8], 0
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_4D90:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    eax, edi
        db  08Bh, 035h, 0E4h, 0F7h, 000h, 000h
        sub    eax, esi
        mov    ecx, 0dh
        sub    edx, edx
        div    ecx
        mov    ecx, dword ptr [edi + 8]
        mov    edx, dword ptr [esp + 18h]
        xor    edx, ecx
        and    edx, 0fffffh
        xor    edx, ecx
        mov    dword ptr [edi + 8], edx
        mov    edx, dword ptr [esp + 14h]
        mov    ebx, dword ptr [edi + 9]
        mov    ecx, dword ptr [edx + 9]
        xor    ebx, ecx
        and    ebx, 0fffh
        xor    ebx, ecx
        shr    ecx, 0ch
        imul   ecx, ecx, 0dh
        add    esi, ecx
        mov    dword ptr [edi + 9], ebx
        and    dword ptr [esi + 8], 0fff00000h
        mov    ecx, eax
        and    ecx, 0fffffh
        or     dword ptr [esi + 8], ecx
        and    dword ptr [edx + 9], 0fffh
        shl    eax, 0ch
        or     dword ptr [edx + 9], eax
        db  083h, 03Dh, 0B0h, 0F8h, 000h, 000h, 000h
        jne    SHORT loc_1_4E13
        db  039h, 015h, 068h, 0FAh, 000h, 000h
        jne    SHORT loc_1_4E13
        db  089h, 035h, 0B0h, 0F8h, 000h, 000h
loc_1_4E13:
        db  0FFh, 005h, 050h, 0F9h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        ret    0ch
sub_1_4E1F:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [eax + 9]
        mov    ecx, esi
        shr    ecx, 0ch
        imul   ecx, ecx, 0dh
        db  08Bh, 015h, 0E4h, 0F7h, 000h, 000h
        lea    ecx, [ecx + edx + 8]
        and    dword ptr [ecx], 0fff00000h
        mov    eax, dword ptr [eax + 8]
        and    eax, 0fffffh
        or     dword ptr [ecx], eax
        imul   eax, eax, 0dh
        lea    eax, [edx + eax + 9]
        and    esi, 0fffff000h
        and    dword ptr [eax], 0fffh
        or     dword ptr [eax], esi
        db  0FFh, 00Dh, 050h, 0F9h, 000h, 000h
        pop    esi
        ret    4
sub_1_4E68:
        push   esi
        push   edi
        db  08Bh, 03Dh, 0F0h, 0FCh, 000h, 000h
        sub    esi, esi
        test   byte ptr [edi + 1], 2
        je     SHORT loc_1_4E84
        mov    eax, edi
loc_1_4E7A:
        add    eax, 4
        inc    esi
        test   byte ptr [eax + 1], 2
        jne    SHORT loc_1_4E7A
loc_1_4E84:
        mov    eax, dword ptr [esp + 0ch]
        db  02Bh, 005h, 0E4h, 0F7h, 000h, 000h
        mov    ecx, 0dh
        sub    edx, edx
        div    ecx
        shl    eax, 0ch
        or     eax, 0a03h
        mov    dword ptr [edi + esi*4], eax
        shl    esi, 0ch
        db  0A1h, 0F0h, 0F7h, 000h, 000h
        add    eax, esi
        pop    edi
        pop    esi
        ret    4
sub_1_4EB1:
        mov    ecx, dword ptr [esp + 4]
        db  02Bh, 00Dh, 0F0h, 0F7h, 000h, 000h
        db  0A1h, 0F0h, 0FCh, 000h, 000h
        shr    ecx, 0ch
        mov    dword ptr [eax + ecx*4], 0fffff000h
        call   sub_1_370E
        ret    4
sub_1_4ED2:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  083h, 03Dh, 00Ch, 0E4h, 000h, 000h, 000h
        je     NEAR PTR loc_1_4F8B
        mov    dword ptr [esp + 14h], 0
loc_1_4EEE:
        push   5
        db  0FFh, 035h, 03Ch, 0F9h, 000h, 000h
        db  0FFh, 015h, 0F0h, 0F8h, 000h, 000h
        mov    dword ptr [esp + 18h], 0
        add    esp, 8
loc_1_4F07:
        db  081h, 03Dh, 08Ch, 0EEh, 000h, 000h, 000h, 002h, 000h, 000h
        ja     SHORT loc_1_4F4B
loc_1_4F13:
        db  0A1h, 060h, 0F8h, 000h, 000h
        db  08Bh, 00Dh, 08Ch, 0EEh, 000h, 000h
        cmp    dword ptr [eax + 4], ecx
        ja     NEAR PTR loc_1_4FAB
        mov    eax, dword ptr [eax + 0ch]
        db  0A3h, 060h, 0F8h, 000h, 000h
        db  0C7h, 005h, 08Ch, 0EEh, 000h, 000h, 001h, 000h, 000h, 000h
        db  039h, 005h, 054h, 0F9h, 000h, 000h
        jne    SHORT loc_1_4F13
        db  0C7h, 005h, 08Ch, 0EEh, 000h, 000h, 000h, 002h, 000h, 000h
loc_1_4F4B:
        db  081h, 03Dh, 08Ch, 0EEh, 000h, 000h, 0FDh, 003h, 000h, 000h
        jb     SHORT loc_1_4F95
        db  0C7h, 005h, 08Ch, 0EEh, 000h, 000h, 001h, 000h, 000h, 000h
        inc    dword ptr [esp + 14h]
        cmp    dword ptr [esp + 14h], 4
        jb     SHORT loc_1_4F75
        db  083h, 03Dh, 050h, 0F9h, 000h, 000h, 000h
        jg     SHORT loc_1_4F7C
loc_1_4F75:
        cmp    dword ptr [esp + 20h], 0
        jne    SHORT loc_1_4F13
loc_1_4F7C:
        db  0FFh, 035h, 03Ch, 0F9h, 000h, 000h
        db  0FFh, 015h, 0ECh, 0F7h, 000h, 000h
        add    esp, 4
loc_1_4F8B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret    4
loc_1_4F95:
        db  0A1h, 08Ch, 0EEh, 000h, 000h
        lea    esi, [eax*4 - 402000h]
        mov    dword ptr [esp + 18h], 0ffbfeff8h
        jmp    SHORT loc_1_4FC9
loc_1_4FAB:
        db  0A1h, 060h, 0F8h, 000h, 000h
        mov    ecx, dword ptr [eax]
        db  0A1h, 08Ch, 0EEh, 000h, 000h
        lea    esi, [ecx + eax*4]
        db  0A1h, 060h, 0F8h, 000h, 000h
        mov    eax, dword ptr [eax + 4]
        lea    eax, [ecx + eax*4]
        mov    dword ptr [esp + 18h], eax
loc_1_4FC9:
        cmp    dword ptr [esp + 18h], esi
        jbe    NEAR PTR loc_1_4F07
loc_1_4FD3:
        db  0FFh, 005h, 08Ch, 0EEh, 000h, 000h
        test   byte ptr [esi], 1
        je     NEAR PTR loc_1_5094
        mov    eax, dword ptr [esi]
        db  08Bh, 00Dh, 02Ch, 0F9h, 000h, 000h
        or     eax, 800h
        mov    dword ptr [ecx], eax
        db  08Bh, 03Dh, 034h, 0F9h, 000h, 000h
        mov    ebx, edi
        add    ebx, 1000h
        sub    ebp, ebp
        cmp    ebx, edi
        jbe    SHORT loc_1_503B
loc_1_5005:
        mov    ecx, dword ptr [edi]
        test   cl, 1
        je     SHORT loc_1_5034
        test   ecx, 420h
        jne    SHORT loc_1_501E
        push   edi
        call   sub_1_50C1
        add    ebp, eax
        jmp    SHORT loc_1_5034
loc_1_501E:
        mov    eax, ecx
        shl    eax, 5
        xor    eax, ecx
        and    eax, 400h
        xor    eax, ecx
        mov    dword ptr [edi], eax
        and    eax, 0ffffffdfh
        mov    dword ptr [edi], eax
        inc    ebp
loc_1_5034:
        add    edi, 4
        cmp    ebx, edi
        ja     SHORT loc_1_5005
loc_1_503B:
        call   sub_1_370E
        or     ebp, ebp
        jne    SHORT loc_1_5079
        mov    eax, dword ptr [esi]
        shr    eax, 4
        xor    eax, dword ptr [esi]
        and    eax, 40h
        xor    eax, dword ptr [esi]
        mov    dword ptr [esi], eax
        push   esi
        call   sub_1_50C1
        db  0A1h, 03Ch, 0F8h, 000h, 000h
        db  039h, 005h, 060h, 0F8h, 000h, 000h
        jne    SHORT loc_1_5079
        mov    eax, dword ptr [esi]
        db  08Bh, 00Dh, 08Ch, 0EEh, 000h, 000h
        mov    dword ptr [ecx*4 - 402004h], eax
        call   sub_1_370E
loc_1_5079:
        mov    eax, dword ptr [esp + 20h]
        db  039h, 005h, 050h, 0F9h, 000h, 000h
        jge    NEAR PTR loc_1_4F7C
        inc    dword ptr [esp + 10h]
        cmp    dword ptr [esp + 10h], 14h
        jae    SHORT loc_1_50A6
loc_1_5094:
        add    esi, 4
        cmp    dword ptr [esp + 18h], esi
        ja     NEAR PTR loc_1_4FD3
        jmp    NEAR PTR loc_1_4F07
loc_1_50A6:
        db  0FFh, 035h, 03Ch, 0F9h, 000h, 000h
        db  0FFh, 015h, 0ECh, 0F7h, 000h, 000h
        add    esp, 4
        push   0
        call   sub_1_3717
        jmp    NEAR PTR loc_1_4EEE
sub_1_50C1:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        mov    edi, dword ptr [ebp]
        test   edi, 800h
        jne    NEAR PTR loc_1_516D
        mov    eax, edi
        shr    eax, 0ch
        imul   eax, eax, 0dh
        db  08Bh, 00Dh, 0E4h, 0F7h, 000h, 000h
        lea    esi, [eax + ecx]
        mov    ebx, dword ptr [esi]
        or     ebx, ebx
        je     NEAR PTR loc_1_5176
        test   byte ptr [ebx + 3], 2
        jne    SHORT loc_1_5176
        cmp    word ptr [esi + 8], 0
        je     SHORT loc_1_511E
        mov    eax, ebp
        and    eax, 0fffff3ffh
        shr    eax, 0ah
        mov    eax, dword ptr [eax - 800000h]
        shr    eax, 0ch
        imul   eax, eax, 0dh
        cmp    word ptr [ecx + eax + 8], 0
        jne    SHORT loc_1_5176
loc_1_511E:
        test   edi, 40h
        je     SHORT loc_1_512C
        push   ebx
        call   sub_1_5B69
loc_1_512C:
        mov    eax, ebx
        db  02Bh, 005h, 0ACh, 0F8h, 000h, 000h
        mov    ecx, 0ah
        sub    edx, edx
        div    ecx
        shl    eax, 0ch
        and    edi, 0f9eh
        or     eax, edi
        mov    dword ptr [ebp], eax
        mov    ax, word ptr [esi + 0ah]
        dec    ax
        mov    word ptr [esi + 0ah], ax
        jne    SHORT loc_1_516D
        or     byte ptr [ebx + 3], 8
        db  0FFh, 035h, 0DCh, 0F7h, 000h, 000h
        db  0FFh, 035h, 068h, 0FAh, 000h, 000h
        push   esi
        call   sub_1_4D90
loc_1_516D:
        sub    eax, eax
loc_1_516F:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    4
loc_1_5176:
        mov    eax, 1
        jmp    SHORT loc_1_516F
sub_1_517D:
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 44h]
        mov    eax, ecx
        not    eax
        mov    dword ptr [esp + 14h], eax
        mov    eax, dword ptr [esp + 40h]
        cmp    eax, dword ptr [esp + 3ch]
        jbe    NEAR PTR loc_1_52BB
        cmp    ecx, 1fh
        ja     NEAR PTR loc_1_52BB
        lea    eax, [ecx + 1]
        test   ecx, eax
        jne    NEAR PTR loc_1_52BB
        mov    eax, dword ptr [esp + 3ch]
        add    eax, dword ptr [esp + 38h]
        cmp    eax, dword ptr [esp + 40h]
        ja     NEAR PTR loc_1_52BB
        dec    dword ptr [esp + 40h]
loc_1_51C8:
        mov    dword ptr [esp + 10h], 0ffffffffh
        db  0A1h, 06Ch, 0F8h, 000h, 000h
        db  08Dh, 004h, 0C5h, 064h, 0FAh, 000h, 000h
        mov    dword ptr [esp + 1ch], eax
        db  03Dh, 06Ch, 0FAh, 000h, 000h
        jb     NEAR PTR loc_1_5299
loc_1_51EB:
        mov    eax, dword ptr [esp + 1ch]
        mov    edx, dword ptr [eax + 4]
        mov    eax, dword ptr [esp + 40h]
        cmp    edx, eax
        jb     SHORT loc_1_51FC
        mov    edx, eax
loc_1_51FC:
        sub    edx, dword ptr [esp + 38h]
        inc    edx
        mov    eax, dword ptr [esp + 1ch]
        and    edx, dword ptr [esp + 14h]
        mov    edi, dword ptr [eax]
        mov    eax, dword ptr [esp + 3ch]
        cmp    edi, eax
        ja     SHORT loc_1_5215
        mov    edi, eax
loc_1_5215:
        cmp    edx, edi
        jl     SHORT loc_1_5286
        sub    ebx, ebx
loc_1_521B:
        mov    eax, edx
        imul   eax, eax, 0dh
        db  08Bh, 00Dh, 0E4h, 0F7h, 000h, 000h
        add    ecx, eax
        mov    eax, dword ptr [esp + 38h]
        mov    ebp, ebx
        cmp    eax, ebx
        je     SHORT loc_1_5267
loc_1_5232:
        mov    esi, dword ptr [ecx]
        cmp    esi, ebx
        je     SHORT loc_1_525B
        cmp    esi, 0fffff155h
        jae    SHORT loc_1_5263
        test   byte ptr [esi + 3], 2
        jne    SHORT loc_1_5263
        test   byte ptr [esi + 3], 8
        je     SHORT loc_1_524F
        inc    ebp
        jmp    SHORT loc_1_525B
loc_1_524F:
        cmp    word ptr [ecx + 8], bx
        jne    SHORT loc_1_5263
        or     ebp, 80000000h
loc_1_525B:
        dec    eax
        add    ecx, 0dh
        cmp    eax, ebx
        jne    SHORT loc_1_5232
loc_1_5263:
        cmp    eax, ebx
        jne    SHORT loc_1_527C
loc_1_5267:
        cmp    dword ptr [esp + 10h], ebp
        jbe    SHORT loc_1_5279
        mov    dword ptr [esp + 18h], edx
        mov    dword ptr [esp + 10h], ebp
        cmp    ebp, ebx
        je     SHORT loc_1_52ED
loc_1_5279:
        dec    edx
        jmp    SHORT loc_1_527E
loc_1_527C:
        sub    edx, eax
loc_1_527E:
        and    edx, dword ptr [esp + 14h]
        cmp    edx, edi
        jge    SHORT loc_1_521B
loc_1_5286:
        sub    dword ptr [esp + 1ch], 8
        db  081h, 07Ch, 024h, 01Ch, 06Ch, 0FAh, 000h, 000h
        jae    NEAR PTR loc_1_51EB
loc_1_5299:
        cmp    dword ptr [esp + 10h], 80000000h
        jb     SHORT loc_1_52ED
        cmp    dword ptr [esp + 10h], -1
        jne    SHORT loc_1_52D1
        push   dword ptr [esp + 38h]
        call   sub_1_54C6
        or     eax, eax
loc_1_52B5:
        jne    NEAR PTR loc_1_51C8
loc_1_52BB:
        mov    dword ptr [esp + 30h], 0ffffffffh
loc_1_52C3:
        mov    eax, dword ptr [esp + 30h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret    10h
loc_1_52D1:
        db  08Bh, 035h, 050h, 0F9h, 000h, 000h
        db  0A1h, 050h, 0F9h, 000h, 000h
        add    eax, 32h
        push   eax
        call   sub_1_4ED2
        db  039h, 035h, 050h, 0F9h, 000h, 000h
        jmp    SHORT loc_1_52B5
loc_1_52ED:
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [esp + 30h], eax
        imul   eax, eax, 0dh
        db  08Bh, 01Dh, 0E4h, 0F7h, 000h, 000h
        add    ebx, eax
        mov    edi, ebx
        mov    esi, dword ptr [esp + 38h]
        or     esi, esi
        je     SHORT loc_1_5342
loc_1_530A:
        mov    eax, dword ptr [edi]
        or     eax, eax
        jne    SHORT loc_1_531E
        mov    dword ptr [edi], 0fffff155h
        db  0FFh, 00Dh, 040h, 0F9h, 000h, 000h
        jmp    SHORT loc_1_5334
loc_1_531E:
        or     byte ptr [eax + 3], 2
        db  03Bh, 03Dh, 0B0h, 0F8h, 000h, 000h
        jne    SHORT loc_1_5334
        db  0A1h, 068h, 0FAh, 000h, 000h
        db  0A3h, 0B0h, 0F8h, 000h, 000h
loc_1_5334:
        push   edi
        call   sub_1_4E1F
        dec    esi
        add    edi, 0dh
        or     esi, esi
        jne    SHORT loc_1_530A
loc_1_5342:
        db  0FFh, 035h, 0D8h, 0FCh, 000h, 000h
        call   sub_1_36D9
        db  0FFh, 035h, 0D8h, 0FCh, 000h, 000h
        call   sub_1_36D9
        mov    eax, dword ptr [esp + 38h]
        mov    dword ptr [esp + 2ch], eax
        or     eax, eax
        je     NEAR PTR loc_1_5406
loc_1_5368:
        mov    ebp, dword ptr [ebx]
        cmp    ebp, 0fffff155h
        jae    SHORT loc_1_53E1
        call   sub_1_4B88
        mov    dword ptr [esp + 20h], eax
        mov    edi, eax
        mov    esi, ebx
        movsd
        movsd
        movsd
        movsb
        push   ebx
        call   sub_1_4E68
        mov    dword ptr [esp + 28h], eax
        push   dword ptr [esp + 20h]
        call   sub_1_4E68
        mov    dword ptr [esp + 24h], eax
        mov    edi, eax
        mov    esi, dword ptr [esp + 28h]
        mov    ecx, 400h
        rep movsd
        push   dword ptr [esp + 28h]
        call   sub_1_4EB1
        push   dword ptr [esp + 24h]
        call   sub_1_4EB1
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [ebp + 6], eax
        db  0FFh, 035h, 0DCh, 0F7h, 000h, 000h
        db  0FFh, 035h, 068h, 0FAh, 000h, 000h
        push   dword ptr [esp + 28h]
        call   sub_1_4D90
        push   ebp
        call   sub_1_36BD
        mov    dword ptr [ebx], 0fffff155h
loc_1_53E1:
        mov    word ptr [ebx + 0ah], 1
        dec    dword ptr [esp + 2ch]
        mov    word ptr [ebx + 8], 0f188h
        mov    dword ptr [ebx + 4], 0
        add    ebx, 0dh
        cmp    dword ptr [esp + 2ch], 0
        jne    NEAR PTR loc_1_5368
loc_1_5406:
        db  0FFh, 035h, 0D8h, 0FCh, 000h, 000h
        call   sub_1_36EC
        db  0FFh, 035h, 0D8h, 0FCh, 000h, 000h
        call   sub_1_36EC
        jmp    NEAR PTR loc_1_52C3
sub_1_5421:
        push   esi
        db  083h, 03Dh, 0F4h, 0F8h, 000h, 000h, 000h
        je     NEAR PTR loc_1_54C4
        db  083h, 03Dh, 040h, 0F9h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_54C4
        db  0A1h, 068h, 0FAh, 000h, 000h
        mov    ecx, dword ptr [eax + 8]
        and    ecx, 0fffffh
        imul   ecx, ecx, 0dh
        db  0A1h, 0E4h, 0F7h, 000h, 000h
        mov    esi, dword ptr [ecx + eax]
        test   byte ptr [esi + 3], 40h
        je     SHORT loc_1_54C4
        movzx  eax, byte ptr [esi + 2]
        db  08Bh, 004h, 085h, 008h, 0FCh, 000h, 000h
        test   byte ptr [eax + 1ch], 2
        je     SHORT loc_1_54C4
        test   byte ptr [esi + 3], 2
        jne    SHORT loc_1_54C4
        db  08Bh, 00Dh, 098h, 0F6h, 000h, 000h
        cmp    dword ptr [ecx - 28h], 0
        jne    SHORT loc_1_54C4
        db  083h, 03Dh, 0D0h, 0F6h, 000h, 000h, 000h
        jne    SHORT loc_1_54C4
        or     byte ptr [esi + 3], 2
        push   eax
        push   esi
        call   sub_1_5AB7
        push   esi
        call   sub_1_36BD
        db  0A1h, 0D4h, 0F7h, 000h, 000h
        inc    eax
        db  0A3h, 0D4h, 0F7h, 000h, 000h
        db  0A1h, 098h, 0FDh, 000h, 000h
        db  089h, 034h, 085h, 030h, 0FDh, 000h, 000h
        inc    eax
        db  0A3h, 098h, 0FDh, 000h, 000h
        cmp    eax, 1ah
        jl     SHORT loc_1_54C4
        db  0C7h, 005h, 098h, 0FDh, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_54C4:
        pop    esi
        ret
sub_1_54C6:
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    eax, esi
        dec    esi
        or     eax, eax
        je     SHORT loc_1_5518
loc_1_54D6:
        db  0FFh, 005h, 0B4h, 0F8h, 000h, 000h
        call   sub_1_7382
        mov    edi, eax
        or     edi, edi
        db  0A1h, 0C0h, 0F8h, 000h, 000h
        je     SHORT loc_1_550B
        push   eax
        mov    eax, edi
        and    eax, 0fffff3ffh
        shr    eax, 0ah
        sub    eax, 800000h
        push   eax
        call   sub_1_3B30
        mov    eax, esi
        dec    esi
        or     eax, eax
        jne    SHORT loc_1_54D6
        jmp    SHORT loc_1_551C
loc_1_550B:
        db  0A1h, 0B4h, 0F8h, 000h, 000h
        dec    eax
        db  0A3h, 0B4h, 0F8h, 000h, 000h
        jmp    SHORT loc_1_551C
loc_1_5518:
        mov    edi, dword ptr [esp + 8]
loc_1_551C:
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 4
        ret    4
loc_1_5526:
        db  0A1h, 0D8h, 0FCh, 000h, 000h
        db  0C6h, 005h, 044h, 0F9h, 000h, 000h, 001h
        cmp    word ptr [eax], 20h
        mov    eax, 0
        jl     SHORT loc_1_5542
        call   sub_1_4A74
loc_1_5542:
        ret
loc_1_5543:
        db  083h, 03Dh, 0B0h, 0F8h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_5629
        db  083h, 03Dh, 064h, 0EFh, 000h, 000h, 000h
        jne    NEAR PTR loc_1_5629
        db  08Bh, 00Dh, 020h, 0DAh, 000h, 000h
        db  0A1h, 0C8h, 0F8h, 000h, 000h
        cmp    ecx, eax
        jb     NEAR PTR loc_1_5629
        or     eax, eax
        je     SHORT loc_1_55D0
        db  0C7h, 005h, 0C8h, 0F8h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 00Eh, 0FDh, 000h, 000h, 000h
loc_1_5585:
        push   0
        call   sub_1_4ED2
        db  0A1h, 060h, 0F8h, 000h, 000h
        db  039h, 005h, 054h, 0F9h, 000h, 000h
        jne    SHORT loc_1_55B5
        db  0A1h, 0B8h, 0F8h, 000h, 000h
        db  033h, 005h, 08Ch, 0EEh, 000h, 000h
        test   ah, 2
        je     SHORT loc_1_55B5
        db  0A0h, 00Eh, 0FDh, 000h, 000h
        inc    al
        db  0A2h, 00Eh, 0FDh, 000h, 000h
loc_1_55B5:
        db  0A1h, 08Ch, 0EEh, 000h, 000h
        db  0A3h, 0B8h, 0F8h, 000h, 000h
        db  083h, 03Dh, 0B0h, 0F8h, 000h, 000h, 000h
        je     SHORT loc_1_55CD
        call   sub_1_4A74
loc_1_55CD:
        clc
        jmp    SHORT loc_1_562A
loc_1_55D0:
        db  0A0h, 00Eh, 0FDh, 000h, 000h
        cmp    al, 8
        jb     SHORT loc_1_5585
        jne    SHORT loc_1_55F3
        inc    al
        db  0A2h, 00Eh, 0FDh, 000h, 000h
        db  0A1h, 050h, 0F9h, 000h, 000h
        sub    eax, 100h
        db  0A3h, 038h, 0F9h, 000h, 000h
        jmp    SHORT loc_1_5623
loc_1_55F3:
        db  0A1h, 038h, 0F9h, 000h, 000h
        db  03Bh, 005h, 050h, 0F9h, 000h, 000h
        jle    SHORT loc_1_5608
        lea    eax, [ecx + 3e8h]
        jmp    SHORT loc_1_561E
loc_1_5608:
        mov    edx, ecx
        db  02Bh, 015h, 020h, 0FDh, 000h, 000h
        cmp    edx, 3e8h
        jbe    SHORT loc_1_5623
        lea    eax, [ecx + 1388h]
loc_1_561E:
        db  0A3h, 0C8h, 0F8h, 000h, 000h
loc_1_5623:
        db  089h, 00Dh, 020h, 0FDh, 000h, 000h
loc_1_5629:
        stc
loc_1_562A:
        ret
        db  000h, 000h, 000h, 000h, 000h
sub_1_5630:
        mov    edx, dword ptr [esp + 4]
        mov    eax, edx
        shr    eax, 0ch
        shl    eax, 2
        lea    ecx, [eax - 800000h]
        and    ecx, 0fffff3ffh
        shr    ecx, 0ah
        test   byte ptr [ecx - 800000h], 1
        je     SHORT loc_1_566B
        test   byte ptr [eax - 800000h], 1
        je     SHORT loc_1_566B
        cmp    dword ptr [edx + 10h], 62634d56h
        mov    eax, 1
        je     SHORT loc_1_566D
loc_1_566B:
        sub    eax, eax
loc_1_566D:
        ret    4
loc_1_5670:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0A1h, 0D4h, 0FCh, 000h, 000h
        test   byte ptr [eax + 1], 2
        jne    NEAR PTR loc_1_580A
        call   sub_1_375B
        mov    dword ptr [esp + 10h], eax
        or     eax, eax
        je     NEAR PTR loc_1_580A
        db  08Bh, 00Dh, 0D4h, 0FCh, 000h, 000h
        db  08Bh, 035h, 068h, 0FAh, 000h, 000h
        lea    ebp, [ecx + 4]
        lea    ebx, [ecx + 20h]
        cmp    ebx, ebp
        jbe    SHORT loc_1_5709
loc_1_56AD:
        mov    edi, dword ptr [esi + 8]
        and    edi, 0fffffh
        mov    edx, edi
        imul   edx, edx, 0dh
        db  08Bh, 035h, 0E4h, 0F7h, 000h, 000h
        add    esi, edx
        db  03Bh, 035h, 068h, 0FAh, 000h, 000h
        je     SHORT loc_1_5709
        mov    edx, dword ptr [esi]
        mov    al, byte ptr [edx + 3]
        and    al, 42h
        cmp    al, 40h
        jne    SHORT loc_1_5705
        movzx  eax, byte ptr [edx + 2]
        db  08Bh, 004h, 085h, 008h, 0FCh, 000h, 000h
        db  081h, 078h, 00Ch, 070h, 056h, 000h, 000h
        jne    SHORT loc_1_5705
        push   edx
        call   sub_1_369C
        shl    edi, 0ch
        or     edi, 0a03h
        mov    dword ptr [ebp], edi
        add    ebp, 4
        db  08Bh, 00Dh, 0D4h, 0FCh, 000h, 000h
loc_1_5705:
        cmp    ebx, ebp
        ja     SHORT loc_1_56AD
loc_1_5709:
        mov    ebx, ebp
        sub    ebx, ecx
        sar    ebx, 2
        cmp    ebx, 1
        je     NEAR PTR loc_1_580A
        mov    eax, dword ptr [esp + 20h]
        and    eax, 0fffff3ffh
        shr    eax, 0ah
        mov    eax, dword ptr [eax - 800000h]
        mov    dword ptr [ecx], eax
        db  0A1h, 03Ch, 0EFh, 000h, 000h
        lea    eax, [ebx + eax - 1]
        db  0A3h, 03Ch, 0EFh, 000h, 000h
        mov    eax, 0ffh
        mov    cl, 8
        sub    cl, bl
        sar    eax, cl
        mov    byte ptr [esp + 14h], al
        mov    eax, dword ptr [esp + 10h]
        mov    cl, byte ptr [esp + 14h]
        mov    byte ptr [eax], cl
        db  001h, 01Dh, 060h, 0EFh, 000h, 000h
        mov    ecx, dword ptr [esp + 10h]
        db  02Bh, 00Dh, 004h, 0FCh, 000h, 000h
        lea    edx, [ecx*8]
        lea    edi, [ebx + edx]
        mov    eax, edi
        db  08Bh, 00Dh, 030h, 0EFh, 000h, 000h
        cmp    eax, ecx
        ja     SHORT loc_1_577C
        mov    eax, ecx
loc_1_577C:
        db  0A3h, 030h, 0EFh, 000h, 000h
        push   ebx
        db  0FFh, 035h, 068h, 0F8h, 000h, 000h
        inc    edx
        push   edx
        push   1
        call   sub_1_3776
        mov    esi, eax
loc_1_5793:
        mov    ecx, dword ptr [ebp - 4]
        sub    ebp, 4
        shr    ecx, 0ch
        imul   ecx, ecx, 0dh
        db  0A1h, 0E4h, 0F7h, 000h, 000h
        add    eax, ecx
        mov    dword ptr [ebp], 0fffff000h
        mov    ecx, dword ptr [eax]
        mov    dword ptr [eax + 4], edi
        or     byte ptr [ecx + 3], 20h
        db  039h, 02Dh, 0D4h, 0FCh, 000h, 000h
        je     SHORT loc_1_57D5
        or     esi, esi
        je     SHORT loc_1_57C9
        and    byte ptr [ecx + 3], 0bfh
        dec    edi
        jmp    SHORT loc_1_57CD
loc_1_57C9:
        and    byte ptr [ecx + 3], 0dfh
loc_1_57CD:
        push   ecx
        call   sub_1_36BD
        jmp    SHORT loc_1_5793
loc_1_57D5:
        or     esi, esi
        jne    SHORT loc_1_5841
        and    byte ptr [ecx + 3], 0dfh
        mov    dl, 0ffh
        mov    cl, bl
        shl    dl, cl
        mov    eax, dword ptr [esp + 10h]
        and    dl, byte ptr [esp + 14h]
        mov    byte ptr [eax], dl
        db  029h, 01Dh, 060h, 0EFh, 000h, 000h
        db  0FFh, 035h, 028h, 0EFh, 000h, 000h
        db  0FFh, 035h, 004h, 0FCh, 000h, 000h
        call   sub_1_37FA
        inc    eax
        db  0A3h, 030h, 0EFh, 000h, 000h
loc_1_580A:
        mov    esi, dword ptr [esp + 1ch]
        mov    ebx, dword ptr [esi - 4]
loc_1_5811:
        call   sub_1_590D
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_1_583B
        push   1
        push   dword ptr [esp + 24h]
        push   edi
        push   1
        call   sub_1_3776
        or     eax, eax
        db  0A1h, 02Ch, 0EFh, 000h, 000h
        jne    SHORT loc_1_583B
        inc    eax
        db  0A3h, 02Ch, 0EFh, 000h, 000h
        jmp    SHORT loc_1_5811
loc_1_583B:
        or     byte ptr [ebx + 3], 20h
        mov    dword ptr [esi], edi
loc_1_5841:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_584B:
        push   1
        mov    eax, dword ptr [esp + 8]
        push   dword ptr [esp + 0ch]
        push   dword ptr [eax]
        push   0
        call   sub_1_3776
        or     eax, eax
        jne    SHORT loc_1_586F
        db  08Bh, 00Dh, 02Ch, 0EFh, 000h, 000h
        inc    ecx
        db  089h, 00Dh, 02Ch, 0EFh, 000h, 000h
loc_1_586F:
        ret
loc_1_5870:
        push   esi
        mov    eax, dword ptr [esp + 8]
        test   byte ptr [eax - 3], 20h
        lea    ecx, [eax - 6]
        je     SHORT loc_1_58BA
        and    byte ptr [ecx + 3], 0dfh
        mov    esi, dword ptr [eax]
        push   esi
        db  0FFh, 035h, 004h, 0FCh, 000h, 000h
        call   sub_1_37EB
        db  03Bh, 035h, 030h, 0EFh, 000h, 000h
        jne    SHORT loc_1_58AF
        db  0FFh, 035h, 028h, 0EFh, 000h, 000h
        db  0FFh, 035h, 004h, 0FCh, 000h, 000h
        call   sub_1_37FA
        inc    eax
        db  0A3h, 030h, 0EFh, 000h, 000h
loc_1_58AF:
        db  0A1h, 060h, 0EFh, 000h, 000h
        dec    eax
        db  0A3h, 060h, 0EFh, 000h, 000h
loc_1_58BA:
        sub    eax, eax
        pop    esi
        ret
sub_1_58BE:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [eax - 4]
        add    ecx, 3
        test   byte ptr [ecx], 20h
        je     SHORT loc_1_5909
        and    byte ptr [ecx], 0dfh
        mov    esi, dword ptr [eax]
        push   esi
        db  0FFh, 035h, 004h, 0FCh, 000h, 000h
        call   sub_1_37EB
        db  03Bh, 035h, 030h, 0EFh, 000h, 000h
        jne    SHORT loc_1_58FE
        db  0FFh, 035h, 028h, 0EFh, 000h, 000h
        db  0FFh, 035h, 004h, 0FCh, 000h, 000h
        call   sub_1_37FA
        inc    eax
        db  0A3h, 030h, 0EFh, 000h, 000h
loc_1_58FE:
        db  0A1h, 060h, 0EFh, 000h, 000h
        dec    eax
        db  0A3h, 060h, 0EFh, 000h, 000h
loc_1_5909:
        sub    eax, eax
        pop    esi
        ret
sub_1_590D:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  08Bh, 035h, 0E4h, 0F7h, 000h, 000h
loc_1_5917:
        db  0FFh, 035h, 028h, 0EFh, 000h, 000h
        db  0FFh, 035h, 004h, 0FCh, 000h, 000h
        call   sub_1_379F
        or     eax, eax
        jne    SHORT loc_1_59A9
        db  0BFh, 06Ch, 0FAh, 000h, 000h
        sub    ecx, ecx
loc_1_5933:
        mov    eax, dword ptr [edi]
        imul   eax, eax, 0dh
        lea    edx, [esi + eax]
        mov    ebx, dword ptr [edi + 4]
        mov    eax, ebx
        imul   eax, eax, 0dh
        add    eax, esi
        cmp    eax, edx
        jb     SHORT loc_1_596C
loc_1_5949:
        cmp    dword ptr [edx], 0
        je     SHORT loc_1_5965
        mov    ebp, dword ptr [edx]
        cmp    ebp, 0fffff155h
        jae    SHORT loc_1_5965
        test   byte ptr [ebp + 3], 20h
        je     SHORT loc_1_5965
        test   byte ptr [ebp + 3], 2
        je     SHORT loc_1_5987
        inc    ecx
loc_1_5965:
        add    edx, 0dh
        cmp    eax, edx
        jae    SHORT loc_1_5949
loc_1_596C:
        add    edi, 8
        db  039h, 01Dh, 020h, 0EFh, 000h, 000h
        jne    SHORT loc_1_5933
        or     ecx, ecx
        je     SHORT loc_1_59A5
        push   1f4h
        call   sub_1_3717
        jmp    SHORT loc_1_5917
loc_1_5987:
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        and    byte ptr [ecx + 3], 0dfh
        mov    ecx, dword ptr [edx]
        or     byte ptr [ecx + 3], 40h
        db  08Bh, 00Dh, 058h, 0EFh, 000h, 000h
        inc    ecx
        db  089h, 00Dh, 058h, 0EFh, 000h, 000h
        jmp    SHORT loc_1_59C3
loc_1_59A5:
        sub    eax, eax
        jmp    SHORT loc_1_59C3
loc_1_59A9:
        db  08Bh, 00Dh, 060h, 0EFh, 000h, 000h
        inc    ecx
        db  089h, 00Dh, 060h, 0EFh, 000h, 000h
        db  08Bh, 00Dh, 05Ch, 0EFh, 000h, 000h
        inc    ecx
        db  089h, 00Dh, 05Ch, 0EFh, 000h, 000h
loc_1_59C3:
        or     eax, eax
        je     SHORT loc_1_59DB
        mov    ecx, eax
        db  08Bh, 015h, 030h, 0EFh, 000h, 000h
        cmp    ecx, edx
        ja     SHORT loc_1_59D5
        mov    ecx, edx
loc_1_59D5:
        db  089h, 00Dh, 030h, 0EFh, 000h, 000h
loc_1_59DB:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_59E0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        lea    eax, [esp + 10h]
        mov    edi, dword ptr [esp + 1ch]
        push   eax
        push   edi
        push   dword ptr [esp + 20h]
        call   sub_1_38F0
        or     eax, eax
        jne    SHORT loc_1_5A05
        sub    esi, esi
        jmp    NEAR PTR loc_1_5AAD
loc_1_5A05:
        mov    esi, 1
        db  0FFh, 035h, 0D8h, 0FCh, 000h, 000h
        call   sub_1_36D9
        shl    edi, 2
        add    edi, dword ptr [esp + 10h]
        cmp    edi, dword ptr [esp + 10h]
        jbe    SHORT loc_1_5A9D
loc_1_5A22:
        mov    eax, dword ptr [esp + 10h]
        and    eax, 0fffff3ffh
        shr    eax, 0ah
        test   byte ptr [eax - 7fffffh], 2
        je     SHORT loc_1_5A92
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax]
        test   ah, 2
        je     SHORT loc_1_5A92
        test   ah, 8
        jne    SHORT loc_1_5A92
        push   dword ptr [esp + 10h]
        call   sub_1_3CC3
        mov    ebx, eax
        movzx  eax, byte ptr [ebx + 2]
        db  08Bh, 02Ch, 085h, 008h, 0FCh, 000h, 000h
        test   byte ptr [ebp + 1ch], 1
        je     SHORT loc_1_5A8C
        test   byte ptr [ebx + 3], 10h
        je     SHORT loc_1_5A8C
        mov    eax, dword ptr [esp + 10h]
        test   byte ptr [eax], 40h
        je     SHORT loc_1_5A7F
        push   ebx
        call   sub_1_5B69
        mov    eax, dword ptr [esp + 10h]
        and    dword ptr [eax], 0ffffffbfh
loc_1_5A7F:
        push   ebp
        push   ebx
        call   sub_1_5AB7
        or     eax, eax
        jne    SHORT loc_1_5A8C
        sub    esi, esi
loc_1_5A8C:
        push   ebx
        call   sub_1_36BD
loc_1_5A92:
        add    dword ptr [esp + 10h], 4
        cmp    edi, dword ptr [esp + 10h]
        ja     SHORT loc_1_5A22
loc_1_5A9D:
        db  0FFh, 035h, 0D8h, 0FCh, 000h, 000h
        call   sub_1_36EC
        call   sub_1_370E
loc_1_5AAD:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_5AB7:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 1ch]
        mov    ecx, dword ptr [esp + 20h]
        test   byte ptr [esi + 3], 40h
        je     SHORT loc_1_5AD1
        mov    edi, dword ptr [ecx + 0ch]
        jmp    SHORT loc_1_5AD4
loc_1_5AD1:
        mov    edi, dword ptr [ecx + 8]
loc_1_5AD4:
        or     edi, edi
        je     SHORT loc_1_5B48
        db  0A1h, 03Ch, 0EFh, 000h, 000h
        inc    eax
        db  0A3h, 03Ch, 0EFh, 000h, 000h
        mov    ebx, dword ptr [ecx + 1ch]
        and    ebx, 2
        je     SHORT loc_1_5AF0
        call   sub_1_36FD
loc_1_5AF0:
        db  0A1h, 098h, 0F6h, 000h, 000h
        mov    ebp, dword ptr [esi + 6]
        inc    dword ptr [eax - 28h]
        push   ebp
        call   sub_1_4E68
        mov    dword ptr [esp + 10h], eax
        call   sub_1_389A
        mov    dword ptr [esp + 14h], eax
        push   -1
        add    ebp, 4
        push   dword ptr [esp + 14h]
        push   ebp
        call   edi
        add    esp, 0ch
        mov    edi, eax
        push   dword ptr [esp + 14h]
        call   sub_1_38A3
        push   dword ptr [esp + 10h]
        call   sub_1_4EB1
        db  0A1h, 098h, 0F6h, 000h, 000h
        dec    dword ptr [eax - 28h]
        or     ebx, ebx
        je     SHORT loc_1_5B42
        call   sub_1_3708
loc_1_5B42:
        or     edi, edi
        jne    SHORT loc_1_5B59
        jmp    SHORT loc_1_5B5D
loc_1_5B48:
        mov    edi, 1
        db  0A1h, 040h, 0EFh, 000h, 000h
        add    eax, edi
        db  0A3h, 040h, 0EFh, 000h, 000h
loc_1_5B59:
        and    byte ptr [esi + 3], 0bfh
loc_1_5B5D:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    8
sub_1_5B69:
        mov    ecx, dword ptr [esp + 4]
        test   byte ptr [ecx + 3], 40h
        jne    SHORT loc_1_5B99
        or     byte ptr [ecx + 3], 0c0h
        movzx  eax, byte ptr [ecx + 2]
        db  08Bh, 004h, 085h, 008h, 0FCh, 000h, 000h
        mov    eax, dword ptr [eax + 18h]
        or     eax, eax
        je     SHORT loc_1_5B99
        push   0
        push   0
        mov    ecx, dword ptr [ecx + 6]
        add    ecx, 4
        push   ecx
        call   eax
        add    esp, 0ch
loc_1_5B99:
        ret    4
sub_1_5B9C:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        movzx  eax, byte ptr [ecx + 2]
        db  08Bh, 034h, 085h, 008h, 0FCh, 000h, 000h
        test   byte ptr [ecx + 3], 80h
        je     SHORT loc_1_5BB7
        mov    eax, dword ptr [esi + 14h]
        jmp    SHORT loc_1_5BBA
loc_1_5BB7:
        mov    eax, dword ptr [esi + 10h]
loc_1_5BBA:
        or     eax, eax
        je     SHORT loc_1_5BCB
        push   0
        push   0
        add    ecx, 6
        push   ecx
        call   eax
        add    esp, 0ch
loc_1_5BCB:
        cmp    dword ptr [esp + 0ch], 0
        je     SHORT loc_1_5BE3
        test   byte ptr [esi + 1ch], 2
        je     SHORT loc_1_5BE3
        db  0A1h, 0F4h, 0F8h, 000h, 000h
        dec    eax
        db  0A3h, 0F4h, 0F8h, 000h, 000h
loc_1_5BE3:
        mov    eax, esi
        pop    esi
        ret    8
sub_1_5BE9:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  08Bh, 035h, 030h, 0EFh, 000h, 000h
        mov    ebx, 0ah
        db  0FFh, 015h, 028h, 0FDh, 000h, 000h
        db  08Bh, 00Dh, 0ACh, 0F8h, 000h, 000h
        mov    edi, ecx
        mov    eax, dword ptr [ecx - 18h]
        sub    eax, 1ch
        sub    edx, edx
        div    ebx
        imul   eax, ebx
        lea    edx, [ecx + eax]
        cmp    edx, ecx
        jbe    NEAR PTR loc_1_5D5B
loc_1_5C21:
        mov    eax, dword ptr [edi]
        or     eax, eax
        je     NEAR PTR loc_1_5CDB
        cmp    eax, ecx
        jb     SHORT loc_1_5C37
        cmp    eax, edx
        jbe    NEAR PTR loc_1_5CDB
loc_1_5C37:
        test   byte ptr [edi + 3], 20h
        je     NEAR PTR loc_1_5CDB
        test   byte ptr [edi + 3], 10h
        je     NEAR PTR loc_1_5CD2
        mov    eax, dword ptr [edi + 6]
        cmp    dword ptr [eax + 4], esi
        jne    NEAR PTR loc_1_5CDB
loc_1_5C57:
        test   byte ptr [edi + 3], 2
        jne    NEAR PTR loc_1_5D5B
        or     byte ptr [edi + 3], 2
        db  03Bh, 035h, 060h, 0EFh, 000h, 000h
        jbe    NEAR PTR loc_1_5D55
        db  0FFh, 035h, 028h, 0EFh, 000h, 000h
        db  0FFh, 035h, 004h, 0FCh, 000h, 000h
        call   sub_1_379F
        mov    ebp, eax
        db  0FFh, 035h, 0D8h, 0FCh, 000h, 000h
        call   sub_1_36D9
        call   sub_1_4B88
        mov    ebx, eax
        push   ebx
        call   sub_1_4E68
        mov    dword ptr [esp + 10h], eax
        push   1
        push   dword ptr [esp + 14h]
        push   esi
        push   0
        call   sub_1_3776
        or     eax, eax
        je     SHORT loc_1_5D12
        push   1
        push   dword ptr [esp + 14h]
        push   ebp
        push   1
        call   sub_1_3776
        or     eax, eax
        je     SHORT loc_1_5D12
        test   byte ptr [edi + 3], 10h
        je     SHORT loc_1_5CE8
        mov    eax, dword ptr [edi + 6]
        mov    dword ptr [eax + 4], ebp
        jmp    SHORT loc_1_5CEB
loc_1_5CD2:
        cmp    dword ptr [edi + 6], esi
        je     NEAR PTR loc_1_5C57
loc_1_5CDB:
        add    edi, 0ah
        cmp    edx, edi
        ja     NEAR PTR loc_1_5C21
        jmp    SHORT loc_1_5D5B
loc_1_5CE8:
        mov    dword ptr [edi + 6], ebp
loc_1_5CEB:
        push   esi
        db  0FFh, 035h, 004h, 0FCh, 000h, 000h
        call   sub_1_37EB
        db  0FFh, 035h, 028h, 0EFh, 000h, 000h
        db  0FFh, 035h, 004h, 0FCh, 000h, 000h
        call   sub_1_37FA
        inc    eax
        db  0A3h, 030h, 0EFh, 000h, 000h
        sub    esi, esi
        jmp    SHORT loc_1_5D1E
loc_1_5D12:
        push   ebp
        db  0FFh, 035h, 004h, 0FCh, 000h, 000h
        call   sub_1_37EB
loc_1_5D1E:
        push   dword ptr [esp + 10h]
        call   sub_1_4EB1
        mov    dword ptr [ebx], 0
        db  0A1h, 040h, 0F9h, 000h, 000h
        inc    eax
        db  0A3h, 040h, 0F9h, 000h, 000h
        db  0FFh, 035h, 0A0h, 0FDh, 000h, 000h
        db  0FFh, 035h, 060h, 0FAh, 000h, 000h
        push   ebx
        call   sub_1_4D90
        db  0FFh, 035h, 0D8h, 0FCh, 000h, 000h
        call   sub_1_36EC
loc_1_5D55:
        push   edi
        call   sub_1_36BD
loc_1_5D5B:
        db  0FFh, 015h, 070h, 0F8h, 000h, 000h
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  000h, 000h, 000h, 000h, 000h
_PageCheckLinRange:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        push   esi
        call   sub_1_38B0
        lea    edx, [esi*4 - 800000h]
        sub    esi, esi
        mov    ecx, dword ptr [esp + 14h]
        cmp    ecx, esi
        je     SHORT loc_1_5DB2
        mov    edi, 200h
loc_1_5D93:
        mov    ebx, edx
        and    ebx, 0fffff3ffh
        shr    ebx, 0ah
        test   dword ptr [ebx - 800000h], edi
        je     SHORT loc_1_5DB2
        test   dword ptr [edx], edi
        je     SHORT loc_1_5DB2
        add    edx, 4
        inc    esi
        cmp    ecx, esi
        ja     SHORT loc_1_5D93
loc_1_5DB2:
        push   dword ptr [eax + 10h]
        db  0FFh, 015h, 0ECh, 0F7h, 000h, 000h
        add    esp, 4
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        ret
_CopyPageTable:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edx, dword ptr [esp + 18h]
        cmp    edx, 0ffbffh
        ja     NEAR PTR loc_1_5E82
        db  039h, 015h, 014h, 0FDh, 000h, 000h
        ja     SHORT loc_1_5DF1
        db  039h, 015h, 004h, 0F9h, 000h, 000h
        jbe    SHORT loc_1_5DF1
        db  089h, 015h, 004h, 0F9h, 000h, 000h
loc_1_5DF1:
        lea    eax, [edx*4 - 800000h]
        mov    ebx, eax
        and    ebx, 0fffff3ffh
        shr    ebx, 0ah
        sub    ebx, 800000h
        mov    dword ptr [esp + 10h], eax
        mov    ebp, 400h
        mov    eax, edx
        and    eax, 3ffh
        sub    ebp, eax
        mov    eax, dword ptr [esp + 1ch]
        cmp    ebp, eax
        jb     SHORT loc_1_5E25
        mov    ebp, eax
loc_1_5E25:
        test   byte ptr [ebx], 1
        je     SHORT loc_1_5E70
loc_1_5E2A:
        lea    ecx, [ebp*4]
        mov    edi, dword ptr [esp + 20h]
        mov    esi, dword ptr [esp + 10h]
        shr    ecx, 2
        rep movsd
        sub    eax, ebp
        je     SHORT loc_1_5E8C
        lea    ecx, [ebp*4]
        add    dword ptr [esp + 10h], ecx
        add    dword ptr [esp + 20h], ecx
        add    edx, ebp
        add    ebx, 4
        mov    ebp, 400h
        mov    ecx, edx
        and    ecx, 3ffh
        sub    ebp, ecx
        cmp    ebp, eax
        jb     SHORT loc_1_5E6B
        mov    ebp, eax
loc_1_5E6B:
        test   byte ptr [ebx], 1
        jne    SHORT loc_1_5E2A
loc_1_5E70:
        lea    ecx, [ebp*4]
        mov    edi, dword ptr [esp + 20h]
        sub    eax, eax
        shr    ecx, 2
        rep stosd
loc_1_5E82:
        sub    eax, eax
loc_1_5E84:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_5E8C:
        mov    eax, 0ffffffffh
        jmp    SHORT loc_1_5E84
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_5EA0:
        push   esi
        db  0FFh, 035h, 0C8h, 0FCh, 000h, 000h
        call   sub_1_36D9
        sub    esi, esi
        db  0A1h, 0FCh, 0FAh, 000h, 000h
        mov    ecx, eax
loc_1_5EB5:
        test   byte ptr [ecx + 1], 2
        je     SHORT loc_1_5EC6
        add    ecx, 4
        inc    esi
        cmp    esi, 1
        jb     SHORT loc_1_5EB5
        jmp    SHORT loc_1_5EDD
loc_1_5EC6:
        lea    eax, [eax + esi*4]
        push   eax
        shl    esi, 0ch
        push   dword ptr [esp + 0ch]
        db  0E8h, 035h, 0A8h, 0FEh, 0FFh
        db  0A1h, 0F4h, 0FCh, 000h, 000h
        add    eax, esi
loc_1_5EDD:
        pop    esi
        ret    4
sub_1_5EE1:
        mov    ecx, dword ptr [esp + 4]
        db  02Bh, 00Dh, 0F4h, 0FCh, 000h, 000h
        shr    ecx, 0ch
        db  0A1h, 0FCh, 0FAh, 000h, 000h
        push   0fffff000h
        lea    eax, [eax + ecx*4]
        push   eax
        call   sub_1_3B30
        call   sub_1_370E
        db  0FFh, 035h, 0C8h, 0FCh, 000h, 000h
        call   sub_1_36EC
        ret    4
sub_1_5F14:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    esi, esi
        db  0FFh, 015h, 0FCh, 0D7h, 000h, 000h
        mov    ebx, dword ptr [esp + 1ch]
        mov    eax, ebx
        and    eax, 0fffffc00h
        shl    eax, 0ch
        lea    edi, [ebx*4 - 800000h]
        lea    ebp, [eax + 1200e8h]
        push   ebp
        call   sub_1_5630
        cmp    eax, esi
        je     SHORT loc_1_5F6F
        test   byte ptr [ebp + 24h], 4
        je     SHORT loc_1_5F6F
        db  0F6h, 005h, 0F9h, 0F8h, 000h, 000h, 001h
        jne    SHORT loc_1_5F5D
        test   byte ptr [ebp + 1], 20h
        je     SHORT loc_1_5F6F
loc_1_5F5D:
        test   byte ptr [ebp + 1], 10h
        je     SHORT loc_1_5F6F
        mov    eax, 1
        and    bl, 7
        mov    cl, bl
        shl    eax, cl
loc_1_5F6F:
        mov    ebp, dword ptr [esp + 28h]
        mov    ebx, ebp
        and    ebx, 60000h
        or     ebx, 0a000000h
        shr    ebx, 10h
        mov    eax, dword ptr [esp + 24h]
        shl    eax, 0ch
        or     ebx, eax
        mov    eax, ebp
        and    eax, 1
        or     ebx, eax
        and    ebp, 1000h
        mov    eax, dword ptr [esp + 20h]
        lea    eax, [edi + eax*4]
        mov    dword ptr [esp + 10h], eax
        cmp    eax, edi
        jbe    SHORT loc_1_5FBD
loc_1_5FA9:
        push   ebx
        push   edi
        call   sub_1_3B30
        add    esi, eax
        add    edi, 4
        add    ebx, ebp
        cmp    dword ptr [esp + 10h], edi
        ja     SHORT loc_1_5FA9
loc_1_5FBD:
        or     esi, esi
        je     SHORT loc_1_5FC6
        db  0E8h, 00Ch, 0C0h, 0FEh, 0FFh
loc_1_5FC6:
        call   sub_1_370E
        db  0FFh, 015h, 000h, 0D8h, 000h, 000h
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_1_5FDE:
        db  0F6h, 005h, 030h, 0F9h, 000h, 000h, 010h
        mov    eax, 1
        je     SHORT loc_1_5FF0
        mov    eax, dword ptr [esp + 0ch]
loc_1_5FF0:
        or     eax, 60000h
        push   eax
        db  0FFh, 035h, 0D8h, 0F7h, 000h, 000h
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        call   sub_1_5F14
        add    esp, 10h
        ret
sub_1_600D:
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0FFh, 015h, 0FCh, 0D7h, 000h, 000h
        mov    eax, dword ptr [esp + 30h]
        cmp    eax, 0ff7ffh
        jae    NEAR PTR loc_1_61C4
        mov    ecx, dword ptr [esp + 34h]
        lea    edi, [ecx*4 - 800000h]
        lea    esi, [eax*4 - 800000h]
        mov    edx, dword ptr [esp + 38h]
        lea    eax, [esi + edx*4]
        mov    dword ptr [esp + 18h], eax
        mov    eax, ecx
        and    eax, 0fffffc00h
        shl    eax, 0ch
        lea    ebx, [eax + 1200e8h]
        mov    byte ptr [esp + 13h], 0
        test   byte ptr [ebx + 24h], 4
        je     SHORT loc_1_60AB
        db  0F6h, 005h, 0F9h, 0F8h, 000h, 000h, 001h
        jne    SHORT loc_1_6070
        test   byte ptr [ebx + 1], 20h
        je     SHORT loc_1_60AB
loc_1_6070:
        mov    byte ptr [esp + 13h], 1
        lea    eax, [esp + 14h]
        mov    dword ptr [esp + 20h], eax
        mov    dword ptr [esp + 14h], 0
        test   byte ptr [ebx + 1], 10h
        je     SHORT loc_1_60AB
        mov    eax, ecx
        and    eax, 3f8h
        shr    eax, 3
        lea    eax, [ebx + eax - 60h]
        mov    dword ptr [esp + 20h], eax
        mov    eax, 1
        and    cl, 7
        shl    eax, cl
        mov    dword ptr [esp + 14h], eax
loc_1_60AB:
        mov    dword ptr [esp + 1ch], 0
        cmp    dword ptr [esp + 18h], esi
        jbe    NEAR PTR loc_1_6184
        db  08Bh, 00Dh, 0ACh, 0F8h, 000h, 000h
loc_1_60C3:
        mov    eax, esi
        and    eax, 0fffff3ffh
        shr    eax, 0ah
        test   byte ptr [eax - 7fffffh], 2
        je     NEAR PTR loc_1_61C4
        mov    ebx, dword ptr [esi]
        test   bh, 2
        je     NEAR PTR loc_1_61C4
        cmp    byte ptr [esp + 13h], 0
        je     NEAR PTR loc_1_6174
        test   bh, 8
        jne    SHORT loc_1_6174
        sub    ebp, ebp
        test   bl, 1
        je     SHORT loc_1_610C
        shr    ebx, 0ch
        imul   ebx, ebx, 0dh
        db  08Bh, 02Dh, 0E4h, 0F7h, 000h, 000h
        add    ebp, ebx
        jmp    SHORT loc_1_6120
loc_1_610C:
        shr    ebx, 0ch
        imul   ebx, ebx, 0ah
        add    ebx, ecx
        mov    al, byte ptr [ebx + 3]
        and    al, 18h
        cmp    al, 10h
        jne    SHORT loc_1_6120
        mov    ebp, dword ptr [ebx + 6]
loc_1_6120:
        or     ebp, ebp
        je     SHORT loc_1_612B
        cmp    word ptr [ebp + 8], 0
        jne    SHORT loc_1_6174
loc_1_612B:
        sub    ebx, ebx
        mov    ebp, dword ptr [edi]
        mov    eax, ebp
        and    ah, 0ah
        cmp    ah, 2
        jne    SHORT loc_1_6165
        test   ebp, 1
        je     SHORT loc_1_6151
        shr    ebp, 0ch
        imul   ebp, ebp, 0dh
        db  08Bh, 01Dh, 0E4h, 0F7h, 000h, 000h
        add    ebx, ebp
        jmp    SHORT loc_1_6165
loc_1_6151:
        shr    ebp, 0ch
        imul   ebp, ebp, 0ah
        add    ebp, ecx
        mov    al, byte ptr [ebp + 3]
        and    al, 18h
        cmp    al, 10h
        jne    SHORT loc_1_6165
        mov    ebx, dword ptr [ebp + 6]
loc_1_6165:
        or     ebx, ebx
        je     SHORT loc_1_6170
        cmp    word ptr [ebx + 8], 1
        je     SHORT loc_1_6174
loc_1_6170:
        inc    dword ptr [esp + 1ch]
loc_1_6174:
        add    esi, 4
        add    edi, 4
        cmp    dword ptr [esp + 18h], esi
        ja     NEAR PTR loc_1_60C3
loc_1_6184:
        lea    eax, [edx*4]
        sub    esi, eax
        sub    edi, eax
        cmp    dword ptr [esp + 1ch], 0
        je     SHORT loc_1_61DB
        mov    eax, dword ptr [esp + 1ch]
        db  003h, 005h, 01Ch, 0EFh, 000h, 000h
        db  0A3h, 01Ch, 0EFh, 000h, 000h
        db  039h, 005h, 024h, 0EFh, 000h, 000h
        jge    SHORT loc_1_61DB
        push   dword ptr [esp + 1ch]
        call   sub_1_54C6
        or     eax, eax
        jne    SHORT loc_1_61DB
        mov    eax, dword ptr [esp + 1ch]
        db  029h, 005h, 01Ch, 0EFh, 000h, 000h
loc_1_61C4:
        sub    esi, esi
loc_1_61C6:
        db  0FFh, 015h, 000h, 0D8h, 000h, 000h
        call   sub_1_370E
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_1_61DB:
        mov    dword ptr [esp + 28h], 0
        sub    ebx, ebx
        cmp    dword ptr [esp + 18h], esi
        jbe    NEAR PTR loc_1_62F1
loc_1_61EF:
        cmp    byte ptr [esp + 13h], 0
        je     NEAR PTR loc_1_6293
        mov    eax, dword ptr [esp + 20h]
        mov    eax, dword ptr [eax]
        test   dword ptr [esp + 14h], eax
        jne    NEAR PTR loc_1_6293
        mov    ecx, dword ptr [edi]
        mov    eax, ecx
        and    ah, 0ah
        cmp    ah, 2
        jne    SHORT loc_1_6237
        shr    ecx, 0ch
        imul   ecx, ecx, 0dh
        db  0A1h, 0E4h, 0F7h, 000h, 000h
        add    eax, ecx
        mov    cx, word ptr [eax + 8]
        cmp    cx, 0f000h
        jae    SHORT loc_1_6237
        dec    cx
        mov    word ptr [eax + 8], cx
        jne    SHORT loc_1_6237
        dec    ebx
loc_1_6237:
        test   byte ptr [esi + 1], 8
        jne    SHORT loc_1_62B4
        push   esi
        call   sub_1_3CC3
        mov    ebp, eax
        test   byte ptr [esi], 1
        db  0A1h, 028h, 0FDh, 000h, 000h
        jne    SHORT loc_1_626F
        call   eax
        push   ebp
        push   esi
        push   0
        call   sub_1_4802
        mov    dword ptr [esp + 24h], eax
        db  0FFh, 015h, 070h, 0F8h, 000h, 000h
        cmp    dword ptr [esp + 24h], 0
        je     NEAR PTR loc_1_61C4
loc_1_626F:
        mov    ecx, dword ptr [ebp + 6]
        mov    ax, word ptr [ecx + 8]
        cmp    ax, 0f000h
        jae    SHORT loc_1_6288
        or     ax, ax
        jne    SHORT loc_1_6282
        inc    ebx
loc_1_6282:
        inc    ax
        mov    word ptr [ecx + 8], ax
loc_1_6288:
        inc    word ptr [ebp]
        inc    word ptr [ecx + 0ah]
        push   ebp
        jmp    SHORT loc_1_62AF
loc_1_6293:
        test   byte ptr [esi + 1], 8
        jne    SHORT loc_1_62B4
        push   esi
        call   sub_1_3CC3
        inc    word ptr [eax]
        test   byte ptr [esi], 1
        je     SHORT loc_1_62AE
        mov    ecx, dword ptr [eax + 6]
        inc    word ptr [ecx + 0ah]
loc_1_62AE:
        push   eax
loc_1_62AF:
        call   sub_1_36BD
loc_1_62B4:
        mov    eax, dword ptr [esi]
        and    eax, 0ffffff9fh
        or     eax, 6
        push   eax
        push   edi
        call   sub_1_3B30
        add    dword ptr [esp + 28h], eax
        test   byte ptr [esp + 17h], 80h
        je     SHORT loc_1_62DD
        add    dword ptr [esp + 20h], 4
        mov    dword ptr [esp + 14h], 1
        jmp    SHORT loc_1_62E1
loc_1_62DD:
        shl    dword ptr [esp + 14h], 1
loc_1_62E1:
        add    esi, 4
        add    edi, 4
        cmp    dword ptr [esp + 18h], esi
        ja     NEAR PTR loc_1_61EF
loc_1_62F1:
        cmp    ebx, dword ptr [esp + 1ch]
        je     SHORT loc_1_6301
        sub    ebx, dword ptr [esp + 1ch]
        db  001h, 01Dh, 01Ch, 0EFh, 000h, 000h
loc_1_6301:
        mov    esi, 1
        cmp    dword ptr [esp + 28h], 0
        je     NEAR PTR loc_1_61C6
        db  0E8h, 0BCh, 0BCh, 0FEh, 0FFh
        jmp    NEAR PTR loc_1_61C6
_LinMapIntoV86:
        sub    esp, 10h
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 24h]
        mov    dword ptr [esp + 8], ecx
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 10h]
        push   eax
        push   dword ptr [esp + 34h]
        mov    edi, dword ptr [esp + 34h]
        push   0
        push   edi
        push   dword ptr [esp + 34h]
        push   ecx
        call   sub_1_64E6
        or     eax, eax
        jne    SHORT loc_1_634F
        sub    edi, edi
        jmp    SHORT loc_1_639E
loc_1_634F:
        cmp    eax, -1
        mov    esi, dword ptr [esp + 1ch]
        je     SHORT loc_1_6385
        cmp    esi, 110h
        jae    SHORT loc_1_636B
        mov    dword ptr [esp + 8], esi
        mov    edi, 1
        jmp    SHORT loc_1_639A
loc_1_636B:
        push   edi
        push   eax
        push   esi
        call   sub_1_600D
        add    esp, 0ch
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_1_639A
        cmp    dword ptr [esp + 0ch], 0
        je     SHORT loc_1_639A
        jmp    SHORT loc_1_6389
loc_1_6385:
        mov    edi, dword ptr [esp + 14h]
loc_1_6389:
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        push   esi
        call   sub_1_600D
        add    esp, 0ch
loc_1_639A:
        mov    edx, dword ptr [esp + 8]
loc_1_639E:
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 10h
        ret
_MapIntoV86:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 14h]
        mov    esi, dword ptr [esp + 40h]
        push   eax
        mov    eax, esi
        and    eax, 0ffffffefh
        push   eax
        mov    edi, dword ptr [esp + 40h]
        push   0
        push   edi
        push   dword ptr [esp + 40h]
        push   dword ptr [esp + 48h]
        call   sub_1_64E6
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_1_646E
        cmp    ebx, -1
        je     SHORT loc_1_644C
        mov    eax, dword ptr [esp + 28h]
        cmp    eax, -1
        jne    SHORT loc_1_6402
        xor    esi, 10h
        shr    esi, 4
        push   esi
        push   edi
        push   ebx
        call   sub_1_5FDE
        add    esp, 0ch
        mov    edi, eax
        jmp    SHORT loc_1_6470
loc_1_6402:
        mov    ebp, eax
        shr    ebp, 0ch
        test   eax, 0fffh
        jne    SHORT loc_1_646E
        mov    esi, dword ptr [esp + 38h]
        lea    eax, [esp + 18h]
        push   eax
        push   edi
        lea    eax, [esi + ebp]
        push   eax
        call   sub_1_38F0
        or     eax, eax
        je     SHORT loc_1_646E
        mov    eax, dword ptr [eax + 4]
        shr    eax, 0ch
        cmp    eax, ebp
        jne    SHORT loc_1_646E
        push   edi
        push   ebx
        lea    eax, [esi + ebp]
        push   eax
        call   sub_1_600D
        add    esp, 0ch
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_1_6470
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_6470
        jmp    SHORT loc_1_6458
loc_1_644C:
        mov    esi, dword ptr [esp + 38h]
        mov    edi, dword ptr [esp + 1ch]
        mov    ebp, dword ptr [esp + 20h]
loc_1_6458:
        push   dword ptr [esp + 14h]
        lea    eax, [esi + ebp]
        push   dword ptr [esp + 14h]
        push   eax
        call   sub_1_600D
        add    esp, 0ch
        jmp    SHORT loc_1_6470
loc_1_646E:
        sub    edi, edi
loc_1_6470:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
_PhysIntoV86:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 10h]
        push   eax
        mov    esi, dword ptr [esp + 20h]
        push   dword ptr [esp + 30h]
        mov    edi, dword ptr [esp + 30h]
        push   esi
        push   edi
        push   dword ptr [esp + 30h]
        push   dword ptr [esp + 38h]
        call   sub_1_64E6
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_1_64DD
        cmp    ebx, -1
        je     SHORT loc_1_64C0
        push   61001h
        push   esi
        push   edi
        push   ebx
        call   sub_1_5F14
        add    esp, 10h
loc_1_64C0:
        cmp    dword ptr [esp + 0ch], 0
        je     SHORT loc_1_64DD
        push   61001h
        push   esi
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        call   sub_1_5F14
        add    esp, 10h
loc_1_64DD:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_1_64E6:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebx, ebx
        sub    ebp, ebp
        mov    edi, dword ptr [esp + 24h]
        mov    dword ptr [esp + 10h], edi
        mov    esi, dword ptr [esp + 1ch]
        cmp    esi, 110h
        ja     NEAR PTR loc_1_6620
        lea    eax, [esi + edi]
        cmp    eax, 110h
        ja     NEAR PTR loc_1_6620
        push   dword ptr [esp + 20h]
        call   sub_1_5630
        cmp    eax, ebx
        je     NEAR PTR loc_1_6620
        mov    eax, dword ptr [esp + 20h]
        mov    eax, dword ptr [eax + 4]
        shr    eax, 0ch
        mov    dword ptr [esp + 14h], eax
        mov    ebx, eax
        add    ebx, esi
        db  080h, 03Dh, 068h, 0F6h, 000h, 000h, 000h
        je     SHORT loc_1_6576
        dec    edi
        lea    ecx, [esi + edi]
        lea    eax, [edi + ebx]
        lea    edx, [eax*4 - 800000h]
        mov    eax, dword ptr [esp + 28h]
        add    eax, edi
        cmp    ecx, esi
        jb     SHORT loc_1_6576
loc_1_655B:
        db  080h, 0B9h, 0E8h, 0F0h, 000h, 000h, 000h
        je     SHORT loc_1_656D
        mov    edi, dword ptr [edx]
        shr    edi, 0ch
        cmp    edi, eax
        jne    SHORT loc_1_658D
loc_1_656D:
        dec    ecx
        sub    edx, 4
        dec    eax
        cmp    ecx, esi
        jae    SHORT loc_1_655B
loc_1_6576:
        mov    eax, dword ptr [esp + 20h]
        test   byte ptr [eax + 24h], 1
        jne    SHORT loc_1_65CC
        cmp    esi, 10h
        jae    SHORT loc_1_6594
        lea    ebp, [ebx + 100h]
        jmp    SHORT loc_1_6600
loc_1_658D:
        sub    ebx, ebx
        jmp    NEAR PTR loc_1_6620
loc_1_6594:
        cmp    esi, 100h
        jb     SHORT loc_1_65B0
        lea    ebp, [ebx - 100h]
        test   byte ptr [esp + 2ch], 1
        je     SHORT loc_1_6600
        mov    ebx, 0ffffffffh
        jmp    SHORT loc_1_6600
loc_1_65B0:
        mov    eax, dword ptr [esp + 10h]
        add    eax, esi
        cmp    eax, 100h
        jbe    SHORT loc_1_6600
        sub    eax, 100h
        mov    dword ptr [esp + 10h], eax
        mov    ebp, dword ptr [esp + 14h]
        jmp    SHORT loc_1_6600
loc_1_65CC:
        test   byte ptr [esp + 2ch], 1
        je     SHORT loc_1_6600
        cmp    esi, 100h
        jb     SHORT loc_1_65E0
        lea    ebp, [ebx + 10h]
        jmp    SHORT loc_1_6600
loc_1_65E0:
        mov    eax, dword ptr [esp + 10h]
        add    eax, esi
        cmp    eax, 100h
        jbe    SHORT loc_1_6600
        sub    eax, 100h
        mov    dword ptr [esp + 10h], eax
        mov    ebp, dword ptr [esp + 14h]
        add    ebp, 110h
loc_1_6600:
        mov    eax, dword ptr [esp + 30h]
        mov    edx, 10h
        mov    dword ptr [eax], ebp
        and    ebp, 0fh
        sub    edx, ebp
        mov    ecx, dword ptr [esp + 10h]
        cmp    edx, ecx
        mov    eax, dword ptr [esp + 34h]
        jb     SHORT loc_1_661E
        mov    edx, ecx
loc_1_661E:
        mov    dword ptr [eax], edx
loc_1_6620:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    1ch
sub_1_662C:
        enter  0, 0
        push   esi
        push   edi
        push   ebx
        pushfd
        cli
        db  080h, 03Dh, 0A0h, 0EFh, 000h, 000h, 000h
        je     NEAR PTR loc_1_66C6
        mov    esi, dword ptr [ebp + 8]
        add    esi, 3
        and    esi, 0fffffffch
        or     esi, esi
        je     SHORT loc_1_66C6
        mov    ecx, 6
        db  0BAh, 070h, 0EFh, 000h, 000h
loc_1_6659:
        cmp    esi, dword ptr [edx + 4]
        jbe    SHORT loc_1_6665
        add    edx, 8
        loop   loc_1_6659
        jmp    SHORT loc_1_66C6
loc_1_6665:
        mov    edi, dword ptr [edx]
        add    dword ptr [edx], esi
        sub    dword ptr [edx + 4], esi
        mov    ecx, 6
        db  0BAh, 070h, 0EFh, 000h, 000h
loc_1_6676:
        cmp    dword ptr [edx + 4], 1000h
        jae    SHORT loc_1_66AC
        add    edx, 8
        loop   loc_1_6676
        db  066h, 00Fh, 0BAh, 02Dh, 030h, 0F9h, 000h, 000h, 000h
        jb     SHORT loc_1_66AC
        push   esi
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    eax, 10h
        mov    ecx, 0bh
        db  0BEh, 0CBh, 066h, 000h, 000h
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        pop    esi
loc_1_66AC:
        popfd
        mov    eax, edi
        test   byte ptr [ebp + 0ch], 1
        je     SHORT loc_1_66C1
        push   edi
        xor    eax, eax
        mov    ecx, esi
        shr    ecx, 2
        cld
        rep stosd
        pop    eax
loc_1_66C1:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
loc_1_66C6:
        popfd
        xor    eax, eax
        jmp    SHORT loc_1_66C1
sub_1_66CB:
        pushfd
        xor    esi, esi
        db  087h, 035h, 06Ch, 0EFh, 000h, 000h
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        db  0CDh, 020h, 000h, 000h, 021h, 000h
        or     eax, eax
        je     SHORT loc_1_66EC
        db  0CDh, 020h, 006h, 000h, 021h, 000h
        jb     SHORT loc_1_6737
loc_1_66EC:
        push   0
        push   2000h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        je     SHORT loc_1_672C
        cli
        mov    ecx, 6
        db  0BAh, 070h, 0EFh, 000h, 000h
        mov    ebx, edx
loc_1_670C:
        cmp    dword ptr [edx + 4], 0
        je     SHORT loc_1_6723
        mov    esi, dword ptr [ebx + 4]
        cmp    esi, dword ptr [edx + 4]
        jbe    SHORT loc_1_671C
        mov    ebx, edx
loc_1_671C:
        add    edx, 8
        loop   loc_1_670C
        mov    edx, ebx
loc_1_6723:
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], 2000h
loc_1_672C:
        db  066h, 00Fh, 0BAh, 035h, 030h, 0F9h, 000h, 000h, 000h
loc_1_6735:
        popfd
        ret
loc_1_6737:
        mov    eax, 3e8h
        db  0BEh, 04Fh, 067h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 06Ch, 0EFh, 000h, 000h
        jmp    SHORT loc_1_6735
loc_1_674F:
        db  0C7h, 005h, 06Ch, 0EFh, 000h, 000h, 000h, 000h, 000h, 000h
        db  0F6h, 005h, 030h, 0F9h, 000h, 000h, 001h
        je     SHORT loc_1_6767
        call   sub_1_66CB
loc_1_6767:
        ret
_SelectorMapFlat:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        movzx  esi, word ptr [ebp + 0ch]
        mov    ebx, dword ptr [ebp + 8]
        call   sub_1_67E2
        jb     SHORT loc_1_67DD
        mov    eax, dword ptr [esi]
        or     eax, dword ptr [esi + 4]
        je     SHORT loc_1_67DD
        mov    al, byte ptr [esi + 4]
        mov    ah, byte ptr [esi + 7]
        shl    eax, 10h
        mov    ax, word ptr [esi + 2]
        test   byte ptr [ebp + 0ch], 4
        je     SHORT loc_1_67D8
        cmp    eax, 400000h
        jae    SHORT loc_1_67D8
        test   byte ptr [esi + 5], 4
        jne    SHORT loc_1_67D8
        movzx  dx, byte ptr [esi + 6]
        and    dl, 0fh
        shl    edx, 10h
        mov    dx, word ptr [esi]
        test   byte ptr [esi + 6], 80h
        je     SHORT loc_1_67C0
        shl    edx, 0ch
        or     edx, 0fffh
loc_1_67C0:
        cmp    edx, 400000h
        jae    SHORT loc_1_67D8
        add    edx, eax
        cmp    edx, 400000h
        jae    SHORT loc_1_67D8
        mov    ebx, dword ptr [ebp + 8]
        add    eax, dword ptr [ebx + 4]
loc_1_67D8:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
loc_1_67DD:
        xor    eax, eax
        dec    eax
        jmp    SHORT loc_1_67D8
sub_1_67E2:
        enter  8, 0
        test   si, 4
        jne    SHORT loc_1_6806
        and    esi, 0fff8h
        or     esi, esi
        je     SHORT loc_1_6846
        sgdt   [ebp - 8]
        cmp    si, word ptr [ebp - 8]
        ja     SHORT loc_1_6846
        add    esi, dword ptr [ebp - 6]
        jmp    SHORT loc_1_6843
loc_1_6806:
        and    esi, 0fff8h
        cmp    word ptr [ebx + 1ah], 0
        je     SHORT loc_1_6846
        movzx  ebx, word ptr [ebx + 1ah]
        and    ebx, 0fff8h
        sgdt   [ebp - 8]
        add    ebx, dword ptr [ebp - 6]
        mov    cl, byte ptr [ebx + 6]
        and    ecx, 0fh
        shl    ecx, 10h
        mov    cx, word ptr [ebx]
        cmp    esi, ecx
        ja     SHORT loc_1_6846
        mov    al, byte ptr [ebx + 4]
        mov    ah, byte ptr [ebx + 7]
        shl    eax, 10h
        mov    ax, word ptr [ebx + 2]
        add    esi, eax
loc_1_6843:
        clc
loc_1_6844:
        leave
        ret
loc_1_6846:
        stc
        jmp    SHORT loc_1_6844
        db  000h, 000h, 000h, 0FFh, 0FFh, 001h, 0FFh, 0FFh, 001h, 0FFh, 0FFh, 0FFh, 001h, 0FFh, 001h, 001h
        db  0FFh, 001h, 001h
_Allocate_GDT_Selector:
        enter  8, 0
        push   edi
        push   esi
        push   ebx
        db  080h, 03Dh, 068h, 0F6h, 000h, 000h, 000h
        je     SHORT loc_1_6877
        mov    ecx, 6
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
loc_1_6877:
        mov    eax, dword ptr [ebp + 8]
        shr    eax, 8
        and    eax, 1fh
        btr    eax, 4
        jb     SHORT loc_1_689E
        db  080h, 0B8h, 04Ch, 068h, 000h, 000h, 001h
        jne    SHORT loc_1_6895
        test   byte ptr [ebp + 13h], 20h
        jne    SHORT loc_1_689E
loc_1_6895:
        db  080h, 0B8h, 04Ch, 068h, 000h, 000h, 000h
        jne    SHORT loc_1_68FB
loc_1_689E:
        sgdt   [ebp - 8]
loc_1_68A2:
        movzx  esi, word ptr [ebp - 8]
        sub    esi, 10h
        mov    edx, dword ptr [ebp - 6]
        mov    eax, 78h
loc_1_68B1:
        cmp    eax, esi
        ja     SHORT loc_1_6900
        mov    ecx, dword ptr [edx + eax]
        or     ecx, dword ptr [edx + eax + 4]
        je     SHORT loc_1_68C3
        add    eax, 8
        jmp    SHORT loc_1_68B1
loc_1_68C3:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + eax], ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [edx + eax + 4], ecx
        shr    ch, 5
        and    ch, 3
        or     al, ch
        movzx  edx, word ptr [ebp - 8]
        inc    edx
        shr    edx, 3
        shl    edx, 10h
        mov    dx, 20h
loc_1_68E7:
        db  080h, 03Dh, 068h, 0F6h, 000h, 000h, 000h
        je     SHORT loc_1_68F6
        db  0CDh, 020h, 020h, 000h, 001h, 000h
loc_1_68F6:
        pop    ebx
        pop    esi
        pop    edi
        leave
        ret
loc_1_68FB:
        xor    eax, eax
        cdq
        jmp    SHORT loc_1_68E7
loc_1_6900:
        cmp    word ptr [ebp - 8], -1
        jae    SHORT loc_1_68FB
        pushfd
        pop    ecx
        and    ch, 2
        jne    SHORT loc_1_6918
        cmp    word ptr [ebp - 8], si
        mov    si, word ptr [ebp - 8]
        jne    SHORT loc_1_68B1
loc_1_6918:
        add    esi, 1011h
        shr    esi, 0ch
        push   9
        push   esi
        db  0FFh, 035h, 0A8h, 0EFh, 000h, 000h
        db  0CDh, 020h, 054h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_68FB
        push   fs
        push   gs
        pushfd
        xor    ebx, ebx
        mov    fs, bx
        mov    gs, bx
        mov    ebx, esi
        shl    ebx, 0ch
        dec    ebx
        mov    edi, edx
        mov    esi, dword ptr [ebp - 6]
        movzx  ecx, word ptr [ebp - 8]
        inc    ecx
        shr    ecx, 2
        mov    dword ptr [ebp - 6], edx
        db  089h, 015h, 0ACh, 0EFh, 000h, 000h
        mov    word ptr [ebp - 8], bx
        cld
        cli
        rep movsd
        mov    ecx, edx
        add    edx, 20h
        mov    word ptr [edx], bx
        mov    word ptr [edx + 2], cx
        shr    ecx, 10h
        mov    byte ptr [edx + 4], cl
        mov    byte ptr [edx + 7], ch
        lgdt   [ebp - 8]
        popfd
        pop    gs
        pop    fs
        jmp    NEAR PTR loc_1_68A2
_Free_GDT_Selector:
        enter  8, 0
        push   edi
        push   esi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0fffffffch
        test   eax, 0ffff0007h
        jne    SHORT loc_1_69DB
        cmp    eax, 30h
        jbe    SHORT loc_1_69DB
        sgdt   [ebp - 8]
        mov    edx, dword ptr [ebp - 6]
        cmp    ax, word ptr [ebp - 8]
        ja     SHORT loc_1_69DB
        mov    ebx, dword ptr [edx + eax + 4]
        shr    ebx, 8
        and    ebx, 1fh
        btr    ebx, 4
        jb     SHORT loc_1_69CC
        cmp    ebx, 2
        jne    SHORT loc_1_69CC
        test   byte ptr [ebp + 0fh], 20h
        je     SHORT loc_1_69DB
loc_1_69CC:
        xor    ebx, ebx
        mov    dword ptr [edx + eax + 4], ebx
        mov    dword ptr [edx + eax], ebx
        dec    eax
loc_1_69D6:
        pop    ebx
        pop    esi
        pop    edi
        leave
        ret
loc_1_69DB:
        xor    eax, eax
        jmp    SHORT loc_1_69D6
_Allocate_LDT_Selector:
        enter  1ch, 0
        push   edi
        push   esi
        push   ebx
        db  080h, 03Dh, 068h, 0F6h, 000h, 000h, 000h
        je     SHORT loc_1_69FA
        mov    ecx, 6
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
loc_1_69FA:
        mov    dword ptr [ebp - 0ch], 0
        mov    ecx, dword ptr [ebp + 14h]
        test   byte ptr [ebp + 18h], 1
        jne    NEAR PTR loc_1_6A8E
        or     ecx, ecx
        je     NEAR PTR loc_1_6BF1
        cmp    ecx, 2000h
        jae    NEAR PTR loc_1_6BF1
loc_1_6A22:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        shr    eax, 8
        and    eax, 1fh
        btr    eax, 4
        jae    NEAR PTR loc_1_6BF1
        cmp    word ptr [ebx + 1ah], 0
        je     NEAR PTR loc_1_6BF6
loc_1_6A43:
        mov    edx, dword ptr [ebx + 28h]
        movzx  eax, word ptr [ebx - 34h]
        mov    dword ptr [ebp - 14h], edx
        mov    dword ptr [ebp - 18h], eax
        shl    eax, 9
        mov    dword ptr [ebp - 0ch], eax
        shl    eax, 3
        mov    edi, edx
        mov    edx, eax
        test   byte ptr [ebp + 18h], 1
        jne    SHORT loc_1_6AB6
        db  0A1h, 0A4h, 0EFh, 000h, 000h
        xor    esi, esi
        mov    ecx, dword ptr [ebp + 14h]
loc_1_6A6D:
        cmp    eax, edx
        jae    SHORT loc_1_6ADF
        push   ecx
        mov    ecx, dword ptr [edi + eax]
        or     ecx, dword ptr [edi + eax + 4]
        pop    ecx
        jne    SHORT loc_1_6AD5
        or     esi, esi
        jne    SHORT loc_1_6A82
        mov    esi, eax
loc_1_6A82:
        add    eax, 8
        loop   loc_1_6A6D
        mov    eax, esi
        jmp    NEAR PTR loc_1_6BAA
loc_1_6A8E:
        test   cl, 4
        je     NEAR PTR loc_1_6BF1
        and    ecx, 0fffffff8h
        db  03Bh, 00Dh, 0A4h, 0EFh, 000h, 000h
        jae    NEAR PTR loc_1_6BF1
        mov    dword ptr [ebp - 1ch], ecx
        mov    ecx, 1
        mov    dword ptr [ebp + 14h], ecx
        jmp    NEAR PTR loc_1_6A22
loc_1_6AB6:
        mov    eax, dword ptr [ebp - 1ch]
        cmp    eax, edx
        jae    NEAR PTR loc_1_6BF1
        mov    ecx, dword ptr [edi + eax]
        or     ecx, dword ptr [edi + eax + 4]
        jne    NEAR PTR loc_1_6BF1
        mov    esi, eax
        jmp    NEAR PTR loc_1_6BAA
loc_1_6AD5:
        mov    ecx, dword ptr [ebp + 14h]
        xor    esi, esi
        add    eax, 8
        jmp    SHORT loc_1_6A6D
loc_1_6ADF:
        shr    edx, 0ch
        cmp    edx, 10h
        jae    NEAR PTR loc_1_6BF1
        shl    ecx, 3
        add    ecx, 0fffh
        shr    ecx, 0ch
        add    edx, ecx
        cmp    edx, 10h
        ja     NEAR PTR loc_1_6BF1
        mov    esi, dword ptr [ebx + 28h]
        mov    dword ptr [ebp - 10h], esi
        mov    edi, edx
        mov    eax, 81h
        test   byte ptr [ebx], 10h
        jne    SHORT loc_1_6B1A
        test   byte ptr [ebx + 24h], 4
        jne    SHORT loc_1_6B1F
loc_1_6B1A:
        mov    eax, 1
loc_1_6B1F:
        push   eax
        push   edi
        push   esi
        db  0CDh, 020h, 054h, 000h, 001h, 000h
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_1_6BF1
        mov    word ptr [ebx - 34h], di
        mov    ecx, edi
        mov    eax, edx
        db  003h, 015h, 074h, 0F6h, 000h, 000h
        shl    ecx, 0ch
        dec    ecx
        mov    word ptr [edx], cx
        mov    word ptr [edx + 2], ax
        shr    ecx, 10h
        shr    eax, 10h
        mov    byte ptr [edx + 4], al
        mov    byte ptr [edx + 7], ah
        mov    byte ptr [edx + 5], 0f2h
        and    cl, 0fh
        mov    byte ptr [edx + 6], cl
        sgdt   [ebp - 8]
        mov    edx, dword ptr [ebp - 6]
        movzx  eax, word ptr [ebx + 1ah]
        mov    ecx, esi
        shl    edi, 0ch
        dec    edi
        add    edx, eax
        mov    eax, edi
        pushfd
        cli
        mov    word ptr [edx], ax
        mov    word ptr [edx + 2], cx
        shr    eax, 10h
        shr    ecx, 10h
        mov    byte ptr [edx + 4], cl
        mov    byte ptr [edx + 7], ch
        mov    byte ptr [edx + 5], 82h
        and    al, 0fh
        mov    byte ptr [edx + 6], al
        db  03Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        jne    SHORT loc_1_6BA4
        mov    ax, word ptr [ebx + 1ah]
        lldt   ax
loc_1_6BA4:
        popfd
        jmp    NEAR PTR loc_1_6A43
loc_1_6BAA:
        mov    ecx, dword ptr [ebp + 14h]
        push   eax
        mov    esi, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 0ch]
loc_1_6BB4:
        mov    dword ptr [edi + eax], esi
        mov    dword ptr [edi + eax + 4], edx
        add    eax, 8
        loop   loc_1_6BB4
        pop    eax
        or     eax, 4
        shr    dh, 5
        and    dh, 3
        or     al, dh
        mov    edx, dword ptr [ebp - 0ch]
        shl    edx, 10h
        db  066h, 08Bh, 015h, 074h, 0F6h, 000h, 000h
        or     dx, 7
loc_1_6BDD:
        db  080h, 03Dh, 068h, 0F6h, 000h, 000h, 000h
        je     SHORT loc_1_6BEC
        db  0CDh, 020h, 020h, 000h, 001h, 000h
loc_1_6BEC:
        pop    ebx
        pop    esi
        pop    edi
        leave
        ret
loc_1_6BF1:
        xor    eax, eax
        cdq
        jmp    SHORT loc_1_6BDD
loc_1_6BF6:
        mov    edi, dword ptr [ebp + 14h]
        inc    edi
        shl    edi, 3
        add    edi, 0fffh
        shr    edi, 0ch
        push   0
        push   10h
        push   80060000h
        db  0CDh, 020h, 01Dh, 001h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_6BF1
        mov    ecx, 60080h
        test   byte ptr [ebx], 10h
        jne    SHORT loc_1_6C2C
        test   byte ptr [ebx + 24h], 4
        jne    SHORT loc_1_6C31
loc_1_6C2C:
        mov    ecx, 60000h
loc_1_6C31:
        mov    dword ptr [ebx + 28h], eax
        mov    esi, eax
        shr    eax, 0ch
        push   ecx
        push   0
        push   1
        push   edi
        push   eax
        db  0CDh, 020h, 01Eh, 001h, 001h, 000h
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_1_6CA4
        mov    word ptr [ebx - 34h], di
        shl    edi, 0ch
        dec    edi
        push   1
        push   0
        push   82h
        push   edi
        push   esi
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        push   edx
        push   eax
        push   20000000h
        push   eax
        push   edx
        db  0CDh, 020h, 076h, 000h, 001h, 000h
        add    esp, 0ch
        pop    edx
        pop    ecx
        or     eax, eax
        je     SHORT loc_1_6CA4
        mov    word ptr [ebx + 1ah], ax
        db  03Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        jne    SHORT loc_1_6C90
        lldt   ax
loc_1_6C90:
        mov    ch, 0f2h
        mov    eax, esi
        db  003h, 005h, 074h, 0F6h, 000h, 000h
        mov    dword ptr [eax], edx
        mov    dword ptr [eax + 4], ecx
        jmp    NEAR PTR loc_1_6A43
loc_1_6CA4:
        xor    eax, eax
        mov    word ptr [ebx - 34h], ax
        xchg   dword ptr [ebx + 28h], eax
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        jmp    NEAR PTR loc_1_6BF1
_Free_LDT_Selector:
        push   edi
        push   esi
        push   ebx
        mov    ebx, dword ptr [esp + 10h]
        cmp    word ptr [ebx + 1ah], 0
        je     SHORT loc_1_6D00
        mov    edi, dword ptr [esp + 14h]
        test   di, 4
        je     SHORT loc_1_6D00
        test   edi, 0ffff0000h
        jne    SHORT loc_1_6D00
        and    edi, 0fff8h
        mov    esi, dword ptr [ebx + 28h]
        movzx  eax, word ptr [ebx - 34h]
        shl    eax, 0ch
        cmp    edi, eax
        jae    SHORT loc_1_6D00
        xor    eax, eax
        mov    dword ptr [edi + esi + 4], eax
        mov    dword ptr [edi + esi], eax
        dec    eax
loc_1_6CFC:
        pop    ebx
        pop    esi
        pop    edi
        ret
loc_1_6D00:
        xor    eax, eax
        jmp    SHORT loc_1_6CFC
_BuildDescriptorDWORDs:
        test   byte ptr [esp + 14h], 1
        jne    SHORT loc_1_6D15
        and    byte ptr [esp + 0ch], 9fh
        or     byte ptr [esp + 0ch], 60h
loc_1_6D15:
        mov    edx, dword ptr [esp + 8]
        shr    edx, 10h
        or     edx, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 4]
        shr    ecx, 10h
        mov    dh, ch
        shl    edx, 10h
        mov    eax, dword ptr [esp + 0ch]
        mov    dh, al
        mov    dl, cl
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        shl    eax, 10h
        mov    ax, cx
        ret
_GetDescriptor:
        push   esi
        push   edi
        push   ebx
        movzx  esi, word ptr [esp + 10h]
        mov    ebx, dword ptr [esp + 14h]
        call   sub_1_67E2
        jb     SHORT loc_1_6D5F
        mov    eax, dword ptr [esi]
        mov    edx, dword ptr [esi + 4]
loc_1_6D5B:
        pop    ebx
        pop    edi
        pop    esi
        ret
loc_1_6D5F:
        xor    eax, eax
        cdq
        jmp    SHORT loc_1_6D5B
_SetDescriptor:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        movzx  esi, word ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        call   sub_1_67E2
        jb     SHORT loc_1_6DCB
        test   byte ptr [ebp + 8], 4
        jne    SHORT loc_1_6DA7
        mov    eax, dword ptr [ebp + 10h]
        shr    eax, 8
        and    eax, 1fh
        btr    eax, 4
        jb     SHORT loc_1_6DB6
        db  080h, 0B8h, 04Ch, 068h, 000h, 000h, 001h
        jne    SHORT loc_1_6D9C
        test   byte ptr [ebp + 1bh], 20h
        jne    SHORT loc_1_6DB6
loc_1_6D9C:
        db  080h, 0B8h, 04Ch, 068h, 000h, 000h, 000h
        jne    SHORT loc_1_6DCB
        jmp    SHORT loc_1_6DB6
loc_1_6DA7:
        mov    eax, dword ptr [ebp + 10h]
        shr    eax, 8
        and    eax, 1fh
        btr    eax, 4
        jae    SHORT loc_1_6DCB
loc_1_6DB6:
        mov    edx, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp + 14h]
        pushfd
        cli
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 4], edx
        popfd
        mov    al, 1
loc_1_6DC6:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
loc_1_6DCB:
        xor    eax, eax
        jmp    SHORT loc_1_6DC6
sub_1_6DCF:
        db  066h, 0A1h, 074h, 0F6h, 000h, 000h
        or     ax, 7
        ret
        db  000h, 000h
loc_1_6DDC:
        push   ecx
        call   sub_1_6DE8
        mov    eax, dword ptr [ecx + eax*4]
        pop    ecx
        ret
        db  090h
sub_1_6DE8:
        xor    ecx, ecx
        db  00Ah, 00Dh, 0E8h, 0F0h, 000h, 000h
        je     SHORT loc_1_6E04
        lea    ecx, [ecx + ecx*2 - 3]
        shl    ecx, 3
        db  003h, 00Dh, 0E4h, 0F0h, 000h, 000h
        cmp    dword ptr [ecx + 10h], ebx
        jne    SHORT loc_1_6E08
loc_1_6E04:
        mov    ecx, dword ptr [ebx + 4]
        ret
loc_1_6E08:
        mov    ecx, dword ptr [ebx + 30h]
        ret
loc_1_6E0C:
        db  083h, 03Dh, 004h, 0F2h, 000h, 000h, 000h
        je     SHORT loc_1_6E65
        db  00Fh, 0B6h, 015h, 0E8h, 0F0h, 000h, 000h
        lea    edx, [edx + edx*2 - 3]
        shl    edx, 3
        db  003h, 015h, 0E4h, 0F0h, 000h, 000h
        cmp    dword ptr [edx + 10h], ebx
        jne    SHORT loc_1_6E4E
        db  08Bh, 015h, 008h, 0F2h, 000h, 000h
        test   cl, 1
        mov    cx, word ptr [edx]
        je     SHORT loc_1_6E62
        mov    word ptr [edx], ax
        mov    edx, dword ptr [ebx + 30h]
        test   edx, edx
        je     SHORT loc_1_6E62
        db  003h, 015h, 004h, 0F2h, 000h, 000h
        jmp    SHORT loc_1_6E5F
loc_1_6E4E:
        mov    edx, dword ptr [ebx + 30h]
        db  003h, 015h, 004h, 0F2h, 000h, 000h
loc_1_6E57:
        test   cl, 1
        mov    cx, word ptr [edx]
        je     SHORT loc_1_6E62
loc_1_6E5F:
        mov    word ptr [edx], ax
loc_1_6E62:
        mov    eax, ecx
        ret
loc_1_6E65:
        db  08Bh, 015h, 000h, 0F2h, 000h, 000h
        jmp    SHORT loc_1_6E57
_MMGR_Toggle_HMA:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 10h]
        test   byte ptr [esp + 14h], 8
        je     SHORT loc_1_6E86
        mov    eax, dword ptr [ebx + 24h]
        and    eax, 1
        jmp    NEAR PTR loc_1_6F30
loc_1_6E86:
        test   byte ptr [esp + 14h], 4
        je     SHORT loc_1_6EB3
        and    byte ptr [ebx + 24h], 0feh
        mov    esi, dword ptr [ebx + 4]
        shr    esi, 0ch
        lea    edi, [esi + 100h]
        push   10h
        push   edi
        push   esi
        call   sub_1_600D
        add    esp, 0ch
        or     eax, eax
        je     NEAR PTR loc_1_6F34
        jmp    SHORT loc_1_6F2E
loc_1_6EB3:
        or     byte ptr [ebx + 24h], 1
        mov    edi, dword ptr [ebx + 4]
        shr    edi, 0ch
        add    edi, 100h
        test   byte ptr [esp + 14h], 1
        jne    SHORT loc_1_6EDD
        push   0
        push   10h
        push   edi
        call   sub_1_5FDE
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_6F34
        jmp    SHORT loc_1_6F2E
loc_1_6EDD:
        lea    esi, [edi + 10h]
        push   10h
        push   edi
        push   esi
        call   sub_1_600D
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_6F34
        mov    ecx, 10h
        mov    edi, 100h
        mov    esi, dword ptr [ebx - 10h]
        lea    esi, [esi + edi*4]
loc_1_6F00:
        db  00Fh, 0B6h, 087h, 0E8h, 0F0h, 000h, 000h
        test   eax, eax
        je     SHORT loc_1_6F20
        lea    eax, [eax + eax*2 - 3]
        shl    eax, 3
        db  003h, 005h, 0E4h, 0F0h, 000h, 000h
        cmp    ebx, dword ptr [eax + 10h]
        je     SHORT loc_1_6F20
        and    byte ptr [esi], 0feh
loc_1_6F20:
        add    esi, 4
        inc    edi
        loop   loc_1_6F00
        db  0A1h, 0CCh, 0FCh, 000h, 000h
        mov    cr3, eax
loc_1_6F2E:
        mov    al, 1
loc_1_6F30:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_6F34:
        xor    eax, eax
        jmp    SHORT loc_1_6F30
sub_1_6F38:
        mov    ecx, dword ptr [edi + 4]
        cmp    ecx, 400h
        jae    SHORT loc_1_6F71
        or     byte ptr [edi + 10h], 2
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
loc_1_6F4D:
        cmp    ecx, 100h
        jb     SHORT loc_1_6F69
        cmp    ecx, 10fh
        ja     SHORT loc_1_6F6E
        test   byte ptr [ebx + 24h], 1
        jne    SHORT loc_1_6F69
        and    ecx, 0ffh
loc_1_6F69:
        clc
loc_1_6F6A:
        mov    dword ptr [edi + 4], ecx
        ret
loc_1_6F6E:
        stc
        jmp    SHORT loc_1_6F6A
loc_1_6F71:
        mov    ebx, ecx
        shl    ebx, 0ch
        and    ebx, 0ffc00000h
        add    ebx, 1200e8h
        mov    edx, dword ptr [ebx + 4]
        shr    edx, 0ch
        sub    ecx, edx
        or     byte ptr [edi + 10h], 4
        mov    dword ptr [edi + 0ch], ebx
        jmp    SHORT loc_1_6F4D
loc_1_6F93:
        call   sub_1_6F38
        jae    SHORT sub_1_6FAB
        or     byte ptr [edi + 10h], 8
        stc
        ret
sub_1_6FA0:
        mov    ecx, dword ptr [edi + 4]
        cmp    ecx, 100h
        jae    SHORT loc_1_6F93
sub_1_6FAB:
        db  0CDh, 020h, 013h, 000h, 003h, 000h
        db  00Fh, 0B6h, 091h, 0E8h, 0F0h, 000h, 000h
        test   edx, edx
        je     NEAR PTR loc_1_7053
        lea    edx, [edx + edx*2 - 3]
        shl    edx, 3
        db  003h, 015h, 0E4h, 0F0h, 000h, 000h
        mov    eax, ebx
        xchg   dword ptr [edx + 10h], eax
        cmp    eax, ebx
        je     SHORT loc_1_7053
        lea    edi, [ecx*4]
        mov    esi, edi
        add    esi, dword ptr [eax - 10h]
        add    edi, dword ptr [ebx - 10h]
        cmp    ecx, 100h
        jae    SHORT loc_1_705B
        mov    ch, byte ptr [esi]
        and    byte ptr [esi], 0feh
        or     byte ptr [edi], 1
        cmp    cl, 10h
        jae    SHORT loc_1_701D
        test   byte ptr [eax + 24h], 1
        jne    SHORT loc_1_700B
        add    esi, 400h
        or     ch, byte ptr [esi]
        and    byte ptr [esi], 0feh
loc_1_700B:
        test   byte ptr [ebx + 24h], 1
        jne    SHORT loc_1_701D
        lea    esi, [edi + 400h]
        and    byte ptr [esi], 0bfh
        or     byte ptr [esi], 1
loc_1_701D:
        push   edi
        mov    edi, dword ptr [edx + 14h]
        shl    edi, 0ch
        add    edi, dword ptr [ebx + 4]
        test   ch, 40h
        je     SHORT loc_1_703C
        mov    esi, edi
        xchg   edi, eax
        mov    edi, dword ptr [edi + 30h]
        add    edi, dword ptr [edx + 4]
        xor    ecx, ecx
        call   dword ptr [edx + 8]
        mov    edi, eax
loc_1_703C:
        mov    esi, dword ptr [ebx + 30h]
        add    esi, dword ptr [edx + 4]
        xor    ecx, ecx
        call   dword ptr [edx + 0ch]
        pop    eax
        and    byte ptr [eax], 0bfh
loc_1_704B:
        db  0A1h, 0CCh, 0FCh, 000h, 000h
        mov    cr3, eax
loc_1_7053:
        db  0CDh, 020h, 014h, 000h, 003h, 000h
        clc
        ret
loc_1_705B:
        test   byte ptr [eax + 24h], 1
        je     SHORT loc_1_7064
        and    byte ptr [esi], 0feh
loc_1_7064:
        test   byte ptr [ebx + 24h], 1
        je     SHORT loc_1_706D
        or     byte ptr [edi], 1
loc_1_706D:
        mov    esi, dword ptr [edx]
        mov    edi, dword ptr [eax + 30h]
        add    edi, dword ptr [edx + 4]
        mov    eax, esi
        xor    ecx, ecx
        call   dword ptr [edx + 8]
        mov    edi, eax
        mov    esi, dword ptr [ebx + 30h]
        add    esi, dword ptr [edx + 4]
        call   dword ptr [edx + 0ch]
        jmp    SHORT loc_1_704B
        db  000h, 000h, 000h
loc_1_708C:
        mov    edx, dword ptr [ebx + 28h]
        lsl    eax, word ptr [ebx + 1ah]
        ret
Hook_Invalid_Page_Fault:
        push   eax
        push   esi
        db  08Bh, 035h, 00Ch, 0F2h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_70B4
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     SHORT loc_1_70B4
        db  0CDh, 020h, 09Eh, 000h, 001h, 000h
        pop    esi
        mov    dword ptr [eax], esi
        clc
loc_1_70B2:
        pop    eax
        ret
loc_1_70B4:
        stc
        pop    esi
        jmp    SHORT loc_1_70B2
Unhook_Invalid_Page_Fault:
        pushad
        db  08Bh, 00Dh, 00Ch, 0F2h, 000h, 000h
        jecxz  loc_1_70D7
        xchg   esi, ecx
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_1_70D7
loc_1_70CB:
        cmp    dword ptr [eax], ecx
        je     SHORT loc_1_70DA
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jne    SHORT loc_1_70CB
loc_1_70D7:
        stc
        jmp    SHORT loc_1_70E7
loc_1_70DA:
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        clc
loc_1_70E7:
        popad
        ret
        db  02Eh, 08Bh, 0C0h
loc_1_70EC:
        mov    esi, cr2
        push   esi
        call   sub_1_4665
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_70FD
        ret
loc_1_70FD:
        sub    esp, 14h
        mov    edi, esp
        mov    dword ptr [edi + 10h], 0
        mov    dword ptr [edi + 0ch], ebx
        mov    dword ptr [edi], esi
        mov    edx, eax
        mov    eax, esi
        mov    ecx, esi
        shr    ecx, 0ch
        mov    dword ptr [edi + 4], ecx
        mov    esi, dword ptr [edx]
        mov    dword ptr [edi + 8], esi
        test   si, 800h
        je     SHORT loc_1_716B
        test   si, 1
        jne    SHORT loc_1_716B
        push   edx
        db  0CDh, 020h, 007h, 000h, 001h, 000h
        push   ecx
        shr    esi, 0ch
        db  03Bh, 035h, 0D8h, 0F7h, 000h, 000h
        je     SHORT loc_1_7156
        test   dword ptr [edi + 8], 1
        jne    SHORT loc_1_7156
        db  0FFh, 005h, 048h, 0EFh, 000h, 000h
        call   sub_1_6FA0
        jmp    SHORT loc_1_715B
loc_1_7156:
        call   sub_1_7214
loc_1_715B:
        lahf
        pop    ecx
        db  0CDh, 020h, 008h, 000h, 001h, 000h
        pop    edx
        sahf
        jb     SHORT loc_1_716B
loc_1_7167:
        add    esp, 14h
        ret
loc_1_716B:
        xor    ecx, ecx
        sti
        mov    edi, esp
        mov    esi, dword ptr [edi + 8]
        and    esi, 0a01h
        cmp    esi, 200h
        jne    SHORT loc_1_7184
        or     cl, 10h
loc_1_7184:
        cmp    edx, 0ffbfe000h
        jb     SHORT loc_1_7197
        cmp    edx, 0ffbff000h
        jae    SHORT loc_1_7197
        or     cl, 1
loc_1_7197:
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        db  083h, 03Dh, 0F8h, 0E3h, 000h, 000h, 000h
        je     SHORT loc_1_71AB
        or     cl, 40h
        jmp    SHORT loc_1_71BA
loc_1_71AB:
        test   byte ptr [ebx], 20h
        je     SHORT loc_1_71B5
        or     cl, 80h
        jmp    SHORT loc_1_71BA
loc_1_71B5:
        or     cx, 100h
loc_1_71BA:
        or     dword ptr [edi + 10h], ecx
        mov    eax, dword ptr [edi]
        mov    ecx, eax
        shr    ecx, 0ch
        db  0FFh, 005h, 054h, 0EFh, 000h, 000h
        db  08Bh, 035h, 00Ch, 0F2h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_71EA
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_1_71EA
loc_1_71DC:
        pushad
        call   dword ptr [eax]
        popad
        jae    SHORT loc_1_7167
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jne    SHORT loc_1_71DC
loc_1_71EA:
        add    esp, 14h
        mov    esi, 38h
        db  083h, 03Dh, 0F8h, 0E3h, 000h, 000h, 000h
        je     SHORT loc_1_7201
        db  0FFh, 025h, 010h, 0E4h, 000h, 000h
loc_1_7201:
        test   byte ptr [ebx], 20h
        jne    SHORT loc_1_720C
        db  0FFh, 025h, 004h, 0E4h, 000h, 000h
loc_1_720C:
        db  0FFh, 025h, 00Ch, 0E4h, 000h, 000h
        db  08Bh, 0C0h
sub_1_7214:
        sti
        mov    esi, eax
        push   edx
        call   sub_1_6F38
        pop    edx
        jb     SHORT loc_1_7240
        mov    eax, ecx
        db  0FFh, 005h, 04Ch, 0EFh, 000h, 000h
        cmp    ecx, 110h
        jae    SHORT loc_1_7239
        db  0FFh, 014h, 08Dh, 010h, 0F2h, 000h, 000h
        clc
loc_1_7238:
        ret
loc_1_7239:
        call   sub_1_7248
        jmp    SHORT loc_1_7238
loc_1_7240:
        or     byte ptr [edi + 10h], 8
        stc
        jmp    SHORT loc_1_7238
        db  090h
sub_1_7248:
        db  0FFh, 005h, 050h, 0EFh, 000h, 000h
        db  0FFh, 00Dh, 04Ch, 0EFh, 000h, 000h
        or     byte ptr [edx], 1
        ret
Hook_V86_Page:
        push   eax
        push   ecx
        mov    ecx, eax
        btr    eax, 1fh
        jb     SHORT loc_1_726A
        db  03Bh, 005h, 070h, 0F6h, 000h, 000h
        jbe    SHORT loc_1_729A
loc_1_726A:
        cmp    eax, 110h
        jae    SHORT loc_1_729A
        or     ecx, ecx
        jns    SHORT loc_1_7283
        db  087h, 034h, 085h, 010h, 0F2h, 000h, 000h
        db  0BFh, 048h, 072h, 000h, 000h
        jmp    SHORT loc_1_7297
loc_1_7283:
        db  081h, 03Ch, 085h, 010h, 0F2h, 000h, 000h, 048h, 072h, 000h, 000h
        jne    SHORT loc_1_729A
        db  089h, 034h, 085h, 010h, 0F2h, 000h, 000h
loc_1_7297:
        pop    ecx
        pop    eax
        ret
loc_1_729A:
        stc
        jmp    SHORT loc_1_7297
loc_1_729D:
        pushad
        mov    ecx, eax
        btr    eax, 1fh
        cmp    eax, 110h
        jae    SHORT loc_1_72C6
        db  039h, 034h, 085h, 010h, 0F2h, 000h, 000h
        jne    SHORT loc_1_72C6
        test   ecx, ecx
        js     SHORT loc_1_72BD
        db  0BFh, 048h, 072h, 000h, 000h
loc_1_72BD:
        db  089h, 03Ch, 085h, 010h, 0F2h, 000h, 000h
loc_1_72C4:
        popad
        ret
loc_1_72C6:
        stc
        jmp    SHORT loc_1_72C4
        db  000h, 000h, 000h
_XchgFreePhysReg:
        xor    eax, eax
        ret
        db  000h
loc_1_72D0:
        xor    eax, eax
        mov    al, 2
        ret
loc_1_72D5:
        db  083h, 03Dh, 0A8h, 0FDh, 000h, 000h, 0FFh
        je     SHORT loc_1_7358
        db  0CDh, 020h, 00Dh, 000h, 010h, 000h
        db  0C7h, 005h, 02Ch, 0FDh, 000h, 000h, 001h, 000h, 000h, 000h
        test   eax, eax
        jne    SHORT loc_1_7358
        db  08Bh, 03Dh, 0A8h, 0FDh, 000h, 000h
        or     edi, edi
        js     SHORT loc_1_7358
        db  0CDh, 020h, 0F1h, 000h, 001h, 000h
        jb     SHORT loc_1_7358
        db  08Bh, 035h, 0C8h, 0F7h, 000h, 000h
        push   esi
        db  0CDh, 020h, 003h, 001h, 001h, 000h
        pop    ecx
        test   eax, eax
        jne    SHORT loc_1_7358
        db  039h, 03Dh, 030h, 0EFh, 000h, 000h
        ja     SHORT loc_1_7365
loc_1_731E:
        xor    edx, edx
        dec    edx
        db  089h, 015h, 0A8h, 0FDh, 000h, 000h
        db  089h, 015h, 02Ch, 0FDh, 000h, 000h
        push   5
        push   esi
        db  0CDh, 020h, 02Eh, 001h, 001h, 000h
        pop    ecx
        pop    ecx
        db  0A1h, 028h, 0EFh, 000h, 000h
        db  089h, 03Dh, 028h, 0EFh, 000h, 000h
        sub    edi, eax
        push   edi
        db  0E8h, 096h, 0A0h, 0FEh, 0FFh
        db  0A3h, 028h, 0EFh, 000h, 000h
        push   esi
        db  0CDh, 020h, 02Fh, 001h, 001h, 000h
        pop    ecx
loc_1_7358:
        db  0BEh, 0D5h, 072h, 000h, 000h
        db  0CDh, 020h, 02Bh, 001h, 001h, 000h
loc_1_7363:
        clc
        ret
loc_1_7365:
        call   sub_1_5BE9
        db  08Bh, 00Dh, 0A8h, 0FDh, 000h, 000h
        or     ecx, ecx
        js     SHORT loc_1_7358
        or     eax, eax
        je     SHORT loc_1_7363
        mov    edi, eax
        cmp    edi, ecx
        jae    SHORT loc_1_731E
        mov    edi, ecx
        jmp    SHORT loc_1_731E
sub_1_7382:
        db  0A1h, 0B4h, 0F8h, 000h, 000h
        test   eax, eax
        je     SHORT loc_1_73E2
        db  0A1h, 0F4h, 0FAh, 000h, 000h
        db  02Bh, 005h, 0FCh, 0F8h, 000h, 000h
        jbe    SHORT loc_1_73E2
        db  0C7h, 005h, 078h, 0F8h, 000h, 000h, 000h, 000h, 000h, 000h
        push   esi
        db  0CDh, 020h, 012h, 000h, 08Bh, 004h
        test   eax, eax
        je     SHORT loc_1_73E3
        mov    esi, eax
        shr    eax, 0ch
        db  02Bh, 005h, 090h, 0F6h, 000h, 000h
        jb     SHORT loc_1_73C2
        db  03Bh, 005h, 07Ch, 0F8h, 000h, 000h
        jb     SHORT loc_1_73C6
loc_1_73C2:
        xor    eax, eax
        jmp    SHORT loc_1_73E3
loc_1_73C6:
        inc    eax
        push   eax
        db  0FFh, 035h, 08Ch, 0F6h, 000h, 000h
        call   sub_1_37EB
        db  0FFh, 00Dh, 0B4h, 0F8h, 000h, 000h
        db  0FFh, 00Dh, 0F4h, 0FAh, 000h, 000h
        mov    eax, esi
loc_1_73E1:
        pop    esi
loc_1_73E2:
        ret
loc_1_73E3:
        xor    eax, eax
        db  0A3h, 0B4h, 0F8h, 000h, 000h
        jmp    SHORT loc_1_73E1
sub_1_73EC:
        db  083h, 03Dh, 0C0h, 0F8h, 000h, 000h, 000h
        je     NEAR PTR loc_1_7544
        db  0FFh, 00Dh, 02Ch, 0FDh, 000h, 000h
        je     NEAR PTR loc_1_7545
loc_1_7405:
        db  0CDh, 020h, 00Eh, 000h, 08Bh, 004h
        xor    eax, eax
        xor    edx, edx
        xor    esi, esi
        db  087h, 005h, 064h, 0F8h, 000h, 000h
        db  087h, 015h, 0A4h, 0FDh, 000h, 000h
        db  087h, 035h, 0D4h, 0F7h, 000h, 000h
        add    esi, eax
        je     SHORT loc_1_7439
        db  0C7h, 005h, 084h, 0F6h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0FFh, 005h, 088h, 0F6h, 000h, 000h
        jmp    SHORT loc_1_743B
loc_1_7439:
        xor    edx, edx
loc_1_743B:
        add    edi, ecx
        jne    SHORT loc_1_7441
        xor    ebx, ebx
loc_1_7441:
        cmp    edi, esi
        jbe    SHORT loc_1_7455
        db  0C7h, 005h, 088h, 0F6h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0FFh, 005h, 084h, 0F6h, 000h, 000h
loc_1_7455:
        cmp    edx, ebx
        je     NEAR PTR loc_1_74FB
        ja     SHORT loc_1_74C1
        sub    ebx, eax
        cmp    ebx, 1
        jle    NEAR PTR loc_1_7534
loc_1_746A:
        db  0A1h, 07Ch, 0F8h, 000h, 000h
        db  02Bh, 005h, 0F4h, 0FAh, 000h, 000h
        jbe    NEAR PTR loc_1_7534
        db  083h, 03Dh, 078h, 0F8h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_7534
        db  08Bh, 00Dh, 040h, 0F9h, 000h, 000h
        shr    ecx, 3
        cmp    ecx, 1ah
        jae    SHORT loc_1_749B
        mov    ecx, 1ah
loc_1_749B:
        db  089h, 00Dh, 078h, 0F8h, 000h, 000h
        xor    eax, eax
        xor    ebx, ebx
        db  0A3h, 088h, 0F6h, 000h, 000h
        db  0A3h, 0B4h, 0F8h, 000h, 000h
        mov    ecx, 4ah
        db  0BEh, 0A4h, 075h, 000h, 000h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        jmp    SHORT loc_1_7534
loc_1_74C1:
        sub    edx, ecx
        cmp    edx, 1
        jle    SHORT loc_1_7534
loc_1_74C8:
        db  0A1h, 0F4h, 0FAh, 000h, 000h
        db  02Bh, 005h, 0FCh, 0F8h, 000h, 000h
        jbe    SHORT loc_1_7534
        db  0C7h, 005h, 084h, 0F6h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A1h, 0F4h, 0FAh, 000h, 000h
        db  02Bh, 005h, 0ECh, 0F8h, 000h, 000h
        cmp    eax, 1ah
        jg     SHORT loc_1_74F4
        mov    eax, 1ah
loc_1_74F4:
        db  0A3h, 0B4h, 0F8h, 000h, 000h
        jmp    SHORT loc_1_7534
loc_1_74FB:
        or     edx, ebx
        jne    SHORT loc_1_7534
        test   esi, esi
        je     SHORT loc_1_751F
        db  0A1h, 0F4h, 0FAh, 000h, 000h
        db  02Bh, 005h, 0ECh, 0F8h, 000h, 000h
        jns    SHORT loc_1_74F4
        test   edi, edi
        jne    SHORT loc_1_7534
        db  083h, 03Dh, 088h, 0F6h, 000h, 000h, 004h
        jb     SHORT loc_1_7534
        jmp    SHORT loc_1_74C8
loc_1_751F:
        test   edi, edi
        je     SHORT loc_1_7534
        cmp    edi, ecx
        je     SHORT loc_1_7534
        db  083h, 03Dh, 084h, 0F6h, 000h, 000h, 00Fh
        jae    NEAR PTR loc_1_746A
loc_1_7534:
        mov    eax, 3e8h
        db  0BEh, 0ECh, 073h, 000h, 000h
        db  0CDh, 020h, 0FEh, 000h, 001h, 000h
loc_1_7544:
        ret
loc_1_7545:
        db  0BEh, 0D5h, 072h, 000h, 000h
        db  0CDh, 020h, 03Ah, 000h, 001h, 000h
        jmp    NEAR PTR loc_1_7405
sub_1_7555:
        xor    eax, eax
        db  039h, 005h, 078h, 0F8h, 000h, 000h
        jne    SHORT loc_1_75A3
        mov    al, 80h
        db  039h, 005h, 0F4h, 0FAh, 000h, 000h
        jae    SHORT loc_1_75A3
        db  03Bh, 005h, 07Ch, 0F8h, 000h, 000h
        jbe    SHORT loc_1_7576
        db  0A1h, 07Ch, 0F8h, 000h, 000h
loc_1_7576:
        db  02Bh, 005h, 0F4h, 0FAh, 000h, 000h
        db  0A3h, 078h, 0F8h, 000h, 000h
        sub    eax, eax
        db  0A3h, 088h, 0F6h, 000h, 000h
        db  0A3h, 0B4h, 0F8h, 000h, 000h
        push   ebx
        push   esi
        mov    ebx, eax
        mov    ecx, 4ah
        db  0BEh, 0A4h, 075h, 000h, 000h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        pop    esi
        pop    ebx
loc_1_75A3:
        ret
loc_1_75A4:
        xor    edi, edi
        db  039h, 03Dh, 078h, 0F8h, 000h, 000h
        je     SHORT loc_1_761F
loc_1_75AE:
        db  0FFh, 035h, 07Ch, 0F8h, 000h, 000h
        db  0FFh, 035h, 08Ch, 0F6h, 000h, 000h
        call   sub_1_379F
        test   eax, eax
        je     SHORT loc_1_761F
        dec    eax
        db  003h, 005h, 090h, 0F6h, 000h, 000h
        mov    esi, eax
        push   20008h
        push   0
        push   4
        push   1
        push   eax
        db  0CDh, 020h, 01Eh, 001h, 001h, 000h
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_1_7620
        db  0A1h, 0CCh, 0FCh, 000h, 000h
        mov    cr3, eax
        cli
        db  083h, 03Dh, 0D4h, 0F7h, 000h, 000h, 000h
        je     SHORT loc_1_75FD
        db  0FFh, 00Dh, 0D4h, 0F7h, 000h, 000h
loc_1_75FD:
        sti
        mov    eax, esi
        shl    eax, 0ch
        db  0CDh, 020h, 013h, 000h, 08Bh, 004h
        db  0FFh, 005h, 0F4h, 0FAh, 000h, 000h
        db  039h, 03Dh, 078h, 0F8h, 000h, 000h
        je     SHORT loc_1_761F
        db  0FFh, 00Dh, 078h, 0F8h, 000h, 000h
        jne    SHORT loc_1_75AE
loc_1_761F:
        ret
loc_1_7620:
        db  02Bh, 035h, 090h, 0F6h, 000h, 000h
        inc    esi
        push   esi
        db  0FFh, 035h, 08Ch, 0F6h, 000h, 000h
        call   sub_1_37EB
        jmp    SHORT loc_1_761F
_GetV86PageableArray:
        push   edi
        push   esi
        mov    eax, dword ptr [esp + 0ch]
        lea    esi, [eax - 60h]
        mov    edi, dword ptr [esp + 10h]
        push   8
        pop    ecx
        cld
        rep movsd
        pop    esi
        pop    edi
        ret
Enable_Touch_1st_Meg:
        ret
Disable_Touch_1st_Meg:
        ret
        db  000h, 000h, 000h
sub_1_7650:
        db  08Bh, 00Dh, 098h, 0F6h, 000h, 000h
        test   byte ptr [ecx], 40h
        je     SHORT loc_1_7678
        mov    al, byte ptr [ecx - 33h]
        dec    al
        mov    byte ptr [ecx - 33h], al
        jne    SHORT loc_1_766C
        call   sub_1_8BC6
        jmp    SHORT loc_1_76A5
loc_1_766C:
        cmp    byte ptr [ecx - 32h], al
        jne    SHORT loc_1_76A5
        call   sub_1_25DB
        jmp    SHORT loc_1_76A5
loc_1_7678:
        mov    dl, byte ptr [ecx - 33h]
        lea    eax, [ecx - 33h]
        dec    dl
        mov    byte ptr [eax], dl
        jne    SHORT loc_1_76A5
        mov    eax, dword ptr [ecx]
        test   ah, 20h
        je     SHORT loc_1_7699
        and    eax, 0ffffdfffh
        mov    dword ptr [ecx], eax
        call   sub_1_8BC6
        jmp    SHORT loc_1_76A5
loc_1_7699:
        and    eax, 0ffffff7fh
        mov    dword ptr [ecx], eax
        call   sub_1_76A6
loc_1_76A5:
        ret
sub_1_76A6:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        db  0A1h, 00Ch, 0F7h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        db  08Bh, 00Dh, 098h, 0F6h, 000h, 000h
        cli
        or     dword ptr [eax], 8
        or     dword ptr [ecx], 8
        sti
        db  0BEh, 082h, 084h, 000h, 000h
        mov    edx, dword ptr [ebp - 4]
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        db  0C6h, 005h, 0BCh, 0F6h, 000h, 000h, 001h
        push   3
        push   ecx
        call   sub_1_8F05
        db  0C6h, 005h, 0BCh, 0F6h, 000h, 000h, 000h
        call   sub_1_8A2D
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_7700:
        mov    eax, dword ptr [esp + 4]
        db  08Bh, 00Dh, 0A0h, 0F6h, 000h, 000h
        mov    dword ptr [eax + 1ch], ecx
        db  0A3h, 0A0h, 0F6h, 000h, 000h
        ret    4
loc_1_7715:
        sub    ecx, ecx
        db  0A1h, 0A0h, 0F6h, 000h, 000h
        cmp    eax, ecx
        je     SHORT loc_1_7731
        mov    edx, dword ptr [esp + 4]
loc_1_7724:
        cmp    eax, edx
        je     SHORT loc_1_7734
        mov    ecx, eax
        mov    eax, dword ptr [eax + 1ch]
        or     eax, eax
        jne    SHORT loc_1_7724
loc_1_7731:
        sub    eax, eax
loc_1_7733:
        ret
loc_1_7734:
        or     ecx, ecx
        mov    eax, dword ptr [edx + 1ch]
        jne    SHORT loc_1_7742
        db  0A3h, 0A0h, 0F6h, 000h, 000h
        jmp    SHORT loc_1_7745
loc_1_7742:
        mov    dword ptr [ecx + 1ch], eax
loc_1_7745:
        xor    word ptr [edx + 2], 20h
        test   byte ptr [edx], 4
        je     SHORT loc_1_775B
        push   0
        push   edx
        db  0FFh, 015h, 06Ch, 0D4h, 000h, 000h
        add    esp, 8
loc_1_775B:
        mov    eax, 0ffffffffh
        jmp    SHORT loc_1_7733
        db  000h, 000h
Begin_Critical_Section:
        push   eax
        push   edx
        push   ecx
loc_1_7767:
        db  068h, 0C4h, 0F6h, 000h, 000h
        call   sub_1_79F4
        add    esp, 4
        db  0A1h, 00Ch, 0F7h, 000h, 000h
        db  03Bh, 005h, 010h, 0F7h, 000h, 000h
        je     SHORT loc_1_7785
loc_1_7781:
        pop    ecx
        pop    edx
        pop    eax
        ret
loc_1_7785:
        db  08Bh, 015h, 098h, 0F6h, 000h, 000h
        db  0B9h, 0E4h, 0F6h, 000h, 000h
        mov    eax, dword ptr [ecx + 10h]
        test   eax, eax
        jne    SHORT loc_1_77A7
loc_1_7797:
        push   ecx
        call   sub_1_79F4
        add    esp, 4
        jmp    SHORT loc_1_7781
loc_1_77A2:
        inc    dword ptr [ecx + 0ch]
        jmp    SHORT loc_1_7781
loc_1_77A7:
        cmp    eax, edx
        je     SHORT loc_1_77A2
        db  068h, 0C4h, 0F6h, 000h, 000h
        call   sub_1_7AB4
        add    esp, 4
        db  068h, 0E4h, 0F6h, 000h, 000h
        call   sub_1_79F4
        add    esp, 4
        db  0B9h, 0C4h, 0F6h, 000h, 000h
        mov    eax, dword ptr [ecx + 10h]
        test   eax, eax
        je     SHORT loc_1_7797
        db  03Bh, 005h, 098h, 0F6h, 000h, 000h
        je     SHORT loc_1_77A2
        db  068h, 0E4h, 0F6h, 000h, 000h
        call   sub_1_7AB4
        add    esp, 4
        jmp    NEAR PTR loc_1_7767
        db  0B8h, 0E4h, 0F6h, 000h, 000h, 083h, 078h, 00Ch, 000h, 074h, 005h, 039h, 050h, 010h, 075h, 00Eh
        db  0B8h, 0C4h, 0F6h, 000h, 000h, 083h, 078h, 00Ch, 000h, 074h, 003h, 039h, 050h, 010h, 0C3h, 08Bh
        db  0C0h
End_Critical_Section:
        push   eax
        push   ecx
        push   edx
        db  083h, 03Dh, 0D0h, 0F6h, 000h, 000h, 000h
        je     SHORT loc_1_7842
        db  0A1h, 0D4h, 0F6h, 000h, 000h
        mov    eax, dword ptr [eax + 14h]
        db  03Bh, 005h, 010h, 0F7h, 000h, 000h
        jne    SHORT loc_1_7835
        db  068h, 0E4h, 0F6h, 000h, 000h
        call   sub_1_7AB4
        add    esp, 4
loc_1_7835:
        db  068h, 0C4h, 0F6h, 000h, 000h
        call   sub_1_7AB4
        add    esp, 4
loc_1_7842:
        pop    edx
        pop    ecx
        pop    eax
        ret
        db  08Bh, 0C0h
loc_1_7848:
        push   eax
        push   edx
        db  0A1h, 00Ch, 0F7h, 000h, 000h
        db  03Bh, 005h, 010h, 0F7h, 000h, 000h
        jne    SHORT loc_1_7866
        push   ecx
        db  068h, 0E4h, 0F6h, 000h, 000h
        call   sub_1_79F4
        add    esp, 4
        pop    ecx
loc_1_7866:
        pop    edx
        pop    eax
        ret
        db  02Eh, 08Bh, 0C0h
loc_1_786C:
        push   eax
        push   ecx
        push   edx
        db  0A1h, 00Ch, 0F7h, 000h, 000h
        db  03Bh, 005h, 010h, 0F7h, 000h, 000h
        jne    SHORT loc_1_7889
        db  068h, 0E4h, 0F6h, 000h, 000h
        call   sub_1_7AB4
        add    esp, 4
loc_1_7889:
        pop    edx
        pop    ecx
        pop    eax
        ret
loc_1_788D:
        push   ebx
        db  0CDh, 020h, 029h, 000h, 001h, 000h
        pop    ebx
        push   esi
        push   edi
        mov    dword ptr [ebx + 2ch], eax
        db  0C7h, 043h, 018h, 0E4h, 0F6h, 000h, 000h
        db  0C7h, 043h, 004h, 0C4h, 0F6h, 000h, 000h
        lea    eax, [ebx + 14h]
        mov    dword ptr [ebx], eax
        lea    eax, [ebx + 28h]
        mov    dword ptr [ebx + 14h], eax
        lea    edi, [ebx]
        call   sub_1_78CD
        lea    edi, [ebx + 14h]
        call   sub_1_78CD
        lea    edi, [ebx + 28h]
        call   sub_1_78CD
        pop    edi
        pop    esi
        ret
sub_1_78CD:
        cmp    dword ptr [edi + 8], 0
        jne    SHORT loc_1_7908
        mov    eax, 1388h
        db  0BEh, 009h, 079h, 000h, 000h
        mov    edx, edi
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    dword ptr [edi + 10h], esi
        push   35h
        push   dword ptr [edi + 4]
        call   sub_1_79F4
        pop    ecx
        pop    ecx
        xor    esi, esi
        xchg   dword ptr [edi + 10h], esi
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        push   dword ptr [edi + 4]
        call   sub_1_7AB4
        pop    ecx
loc_1_7908:
        ret
loc_1_7909:
        pushfd
        cli
        mov    ebx, edx
        xor    eax, eax
        xchg   dword ptr [ebx + 10h], eax
loc_1_7912:
        mov    esi, dword ptr [ebx + 4]
        xor    edi, edi
        xchg   dword ptr [esi + 10h], edi
        or     edi, edi
        je     SHORT loc_1_7958
        mov    dword ptr [ebx + 0ch], edi
        call   sub_1_8452
        xor    ecx, ecx
        xchg   dword ptr [esi + 0ch], ecx
        mov    dword ptr [ebx + 8], ecx
        xor    eax, eax
        xchg   dword ptr [esi + 18h], eax
        add    eax, dword ptr [esi + 14h]
        sub    dword ptr [edi + 3ch], eax
        cmp    esi, dword ptr [esi + 4]
        je     SHORT loc_1_794A
        mov    eax, 2
        call   sub_1_821F
        jmp    SHORT loc_1_7952
loc_1_794A:
        mov    eax, dword ptr [ebx + 4]
        cmp    edi, dword ptr [eax + 10h]
        je     SHORT loc_1_7912
loc_1_7952:
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    SHORT loc_1_794A
loc_1_7958:
        popfd
        ret
loc_1_795A:
        push   esi
        push   edi
        lea    edi, [ebx]
        call   sub_1_7976
        lea    edi, [ebx + 14h]
        call   sub_1_7976
        lea    edi, [ebx + 28h]
        call   sub_1_7976
        pop    edi
        pop    esi
        ret
sub_1_7976:
        mov    esi, dword ptr [edi + 8]
        or     esi, esi
        je     SHORT loc_1_79AC
loc_1_797D:
        push   35h
        push   dword ptr [edi + 4]
        call   sub_1_79F4
        pop    ecx
        pop    ecx
        dec    esi
        jne    SHORT loc_1_797D
        mov    esi, dword ptr [edi + 4]
        mov    edi, dword ptr [edi + 0ch]
        mov    dword ptr [esi + 10h], edi
        db  08Bh, 015h, 098h, 0F6h, 000h, 000h
        mov    ecx, dword ptr [esi + 18h]
        add    ecx, dword ptr [esi + 14h]
        sub    dword ptr [edx + 3ch], ecx
        add    dword ptr [edi + 3ch], ecx
        call   sub_1_84FC
loc_1_79AC:
        ret
sub_1_79AD:
        xor    eax, eax
sub_1_79AF:
        push   ebx
        push   esi
        db  08Bh, 035h, 0A0h, 0F6h, 000h, 000h
loc_1_79B7:
        test   esi, esi
        je     SHORT loc_1_79EE
        cmp    dword ptr [esi + 10h], edi
        jne    SHORT loc_1_79E9
        test   byte ptr [esi], al
        jne    SHORT loc_1_79E9
        xor    ebx, ebx
        mov    dword ptr [esi + 0ch], ebx
        xchg   dword ptr [esi + 10h], ebx
        push   eax
        xor    eax, eax
        xchg   dword ptr [esi + 18h], eax
        add    eax, dword ptr [esi + 14h]
        sub    dword ptr [ebx + 3ch], eax
        cmp    dword ptr [esi + 4], esi
        je     SHORT loc_1_79E8
        pushfd
        cli
        add    eax, 2
        call   sub_1_821F
        popfd
loc_1_79E8:
        pop    eax
loc_1_79E9:
        mov    esi, dword ptr [esi + 1ch]
        jmp    SHORT loc_1_79B7
loc_1_79EE:
        pop    esi
        pop    ebx
        ret
        db  02Eh, 08Bh, 0C0h
sub_1_79F4:
        mov    ecx, dword ptr [esp + 4]
        cmp    word ptr [ecx + 2], 584dh
        jne    SHORT loc_1_7A27
        db  08Bh, 015h, 098h, 0F6h, 000h, 000h
        cmp    dword ptr [ecx + 0ch], 0
        jne    SHORT loc_1_7A28
        mov    dword ptr [ecx + 10h], edx
        inc    dword ptr [ecx + 0ch]
        cmp    dword ptr [ecx + 0ch], 100h
        test   byte ptr [ecx], 1
        je     SHORT loc_1_7A21
        inc    byte ptr [edx - 33h]
loc_1_7A21:
        mov    eax, dword ptr [ecx + 14h]
        add    dword ptr [edx + 3ch], eax
loc_1_7A27:
        ret
loc_1_7A28:
        cmp    edx, dword ptr [ecx + 10h]
        jne    SHORT loc_1_7A31
        inc    dword ptr [ecx + 0ch]
        ret
loc_1_7A31:
        cli
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ecx + 14h]
        shr    eax, 1
        add    dword ptr [edx + 3ch], eax
        mov    edi, dword ptr [ecx + 10h]
        mov    esi, dword ptr [edx + 3ch]
        sub    esi, dword ptr [edi + 3ch]
        mov    ebx, dword ptr [edi + 14h]
        sub    esi, dword ptr [ebx - 64h]
        jle    SHORT loc_1_7A54
        add    dword ptr [ecx + 18h], esi
        add    dword ptr [edi + 3ch], esi
loc_1_7A54:
        mov    esi, ecx
        test   byte ptr [edi + 1], 4
        je     SHORT loc_1_7A61
        call   sub_1_7CA7
loc_1_7A61:
        sti
loc_1_7A62:
        mov    ecx, dword ptr [esp + 14h]
        db  0BFh, 0A7h, 07Ah, 000h, 000h
        call   sub_1_7DA6
loc_1_7A70:
        sti
        mov    eax, dword ptr [esi + 14h]
        shr    eax, 1
        cmp    dword ptr [esi + 0ch], 0
        jne    SHORT loc_1_7A87
        mov    dword ptr [esi + 10h], edx
        inc    dword ptr [esi + 0ch]
        add    dword ptr [edx + 3ch], eax
        jmp    SHORT loc_1_7A99
loc_1_7A87:
        cmp    dword ptr [esi + 10h], edx
        jne    SHORT loc_1_7A62
        neg    eax
        inc    dword ptr [esi + 0ch]
        mov    edi, edx
        db  0CDh, 020h, 014h, 001h, 001h, 000h
loc_1_7A99:
        mov    ecx, esi
        pop    edi
        pop    esi
        pop    ebx
        test   byte ptr [ecx], 1
        je     SHORT loc_1_7AA6
        inc    byte ptr [edx - 33h]
loc_1_7AA6:
        ret
loc_1_7AA7:
        cmp    dword ptr [esi + 0ch], 0
        je     SHORT loc_1_7AB0
        cmp    dword ptr [esi + 10h], edx
loc_1_7AB0:
        ret
        db  02Eh, 08Bh, 0C0h
sub_1_7AB4:
        mov    ecx, dword ptr [esp + 4]
        cmp    word ptr [ecx + 2], 584dh
        jne    SHORT loc_1_7AFD
        cmp    dword ptr [ecx + 0ch], 0
        je     SHORT loc_1_7AFD
        dec    dword ptr [ecx + 0ch]
        jne    SHORT loc_1_7AFD
        xor    edx, edx
        xor    eax, eax
        xchg   dword ptr [ecx + 10h], edx
        xchg   dword ptr [ecx + 18h], eax
        add    eax, dword ptr [ecx + 14h]
        cmp    dword ptr [ecx + 4], ecx
        jne    SHORT loc_1_7B24
        db  083h, 03Dh, 0A4h, 0F6h, 000h, 000h, 000h
        jne    SHORT loc_1_7B09
loc_1_7AE6:
        or     eax, eax
        je     SHORT loc_1_7AFE
        sub    dword ptr [edx + 3ch], eax
        cli
loc_1_7AEE:
        test   byte ptr [ecx], 1
        je     SHORT loc_1_7AF8
        call   sub_1_7B58
loc_1_7AF8:
        call   sub_1_8ACC
loc_1_7AFD:
        ret
loc_1_7AFE:
        test   byte ptr [ecx], 1
        je     SHORT loc_1_7AFD
        call   sub_1_7B58
        ret
loc_1_7B09:
        db  081h, 0F9h, 0C4h, 0F6h, 000h, 000h
        je     SHORT loc_1_7B19
        db  081h, 0F9h, 0E4h, 0F6h, 000h, 000h
        jne    SHORT loc_1_7AE6
loc_1_7B19:
        sub    dword ptr [edx + 3ch], eax
        cli
        call   sub_1_7C5D
        jmp    SHORT loc_1_7AEE
loc_1_7B24:
        cli
        sub    dword ptr [edx + 3ch], eax
        movzx  eax, byte ptr [ecx]
        and    al, 1
        push   esi
        mov    esi, ecx
        call   sub_1_821F
        pop    esi
        sti
        ret
loc_1_7B38:
        mov    ecx, dword ptr [esp + 4]
        cmp    word ptr [ecx + 2], 584dh
        jne    SHORT loc_1_7B48
        mov    eax, dword ptr [ecx + 10h]
        ret
loc_1_7B48:
        xor    eax, eax
        ret
        db  090h
loc_1_7B4C:
        db  0A1h, 098h, 0F6h, 000h, 000h
        inc    byte ptr [eax - 33h]
        ret
        db  02Eh, 08Bh, 0C0h
sub_1_7B58:
        db  0A1h, 098h, 0F6h, 000h, 000h
        test   word ptr [eax], 20c0h
        jne    SHORT loc_1_7B68
        dec    byte ptr [eax - 33h]
        ret
loc_1_7B68:
        push   ecx
        push   edx
        pushfd
        call   sub_1_7650
        popfd
        pop    edx
        pop    ecx
        ret
loc_1_7B74:
        mov    ecx, dword ptr [esp + 4]
        xor    eax, eax
        mov    ecx, dword ptr [ecx]
        test   ecx, 20c0h
        jne    SHORT loc_1_7B85
        ret
loc_1_7B85:
        test   ecx, 40h
        je     SHORT loc_1_7B90
        or     eax, 1
loc_1_7B90:
        test   ecx, 2000h
        je     SHORT loc_1_7B9B
        or     eax, 2
loc_1_7B9B:
        test   ecx, 80h
        je     SHORT loc_1_7BA6
        or     eax, 4
loc_1_7BA6:
        ret
        db  090h
Signal_Semaphore:
        cmp    word ptr [eax + 2], 4553h
        jne    SHORT loc_1_7BBB
        pushfd
        cli
        inc    word ptr [eax]
        cmp    dword ptr [eax + 4], eax
        jne    SHORT loc_1_7BBC
        popfd
loc_1_7BBB:
        ret
loc_1_7BBC:
        push   eax
        push   esi
        mov    esi, eax
        mov    eax, 2
        call   sub_1_821F
        pop    esi
        pop    eax
        popfd
        ret
        db  08Bh, 0C0h
Wait_Semaphore:
        cmp    word ptr [eax + 2], 4553h
        jne    SHORT loc_1_7BDF
        cli
        dec    word ptr [eax]
        js     SHORT loc_1_7BE0
        sti
loc_1_7BDF:
        ret
loc_1_7BE0:
        inc    word ptr [eax]
        sti
        pushad
        mov    esi, eax
        db  0BFh, 0F8h, 07Bh, 000h, 000h
        call   sub_1_7DA6
loc_1_7BF1:
        dec    word ptr [esi]
        sti
        popad
        clc
        ret
loc_1_7BF8:
        mov    ax, word ptr [esi]
        dec    eax
        shr    ah, 7
        ret
sub_1_7C00:
        push   ebx
        test   byte ptr [edi], 4
        jne    SHORT loc_1_7C3A
        cmp    dword ptr [edi + 3ch], 100000h
        jae    SHORT loc_1_7C2C
        mov    ebx, dword ptr [edi + 14h]
        db  083h, 03Dh, 0D4h, 0F6h, 000h, 000h, 000h
        jne    SHORT loc_1_7C2C
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        jne    SHORT loc_1_7C3A
        db  083h, 03Dh, 0F4h, 0F6h, 000h, 000h, 000h
        je     SHORT loc_1_7C3A
loc_1_7C2C:
        or     byte ptr [edi], 4
        mov    eax, edi
        db  087h, 005h, 0A4h, 0F6h, 000h, 000h
        mov    dword ptr [edi - 38h], eax
loc_1_7C3A:
        pop    ebx
        ret
sub_1_7C3C:
        btr    dword ptr [edi], 2
        jae    SHORT loc_1_7C5A
        db  0B8h, 0DCh, 0F6h, 000h, 000h
loc_1_7C47:
        mov    ecx, dword ptr [eax - 38h]
        cmp    ecx, edi
        je     SHORT loc_1_7C54
        jecxz  loc_1_7C5B
        mov    eax, ecx
        jmp    SHORT loc_1_7C47
loc_1_7C54:
        mov    ecx, dword ptr [edi - 38h]
        mov    dword ptr [eax - 38h], ecx
loc_1_7C5A:
        ret
loc_1_7C5B:
        jmp    SHORT loc_1_7C5A
sub_1_7C5D:
        db  08Bh, 015h, 0A4h, 0F6h, 000h, 000h
        test   edx, edx
        je     SHORT loc_1_7CA6
        push   ecx
loc_1_7C68:
        and    word ptr [edx], 0fbfbh
        mov    eax, dword ptr [edx + 14h]
        or     byte ptr [edx + 1], 8
        inc    dword ptr [eax - 6ch]
        db  03Bh, 005h, 010h, 0F7h, 000h, 000h
        jne    SHORT loc_1_7C87
        db  03Bh, 015h, 09Ch, 0F6h, 000h, 000h
        jne    SHORT loc_1_7C8F
loc_1_7C87:
        and    byte ptr [eax + 1], 0fbh
        or     byte ptr [eax + 1], 8
loc_1_7C8F:
        xchg   edi, edx
        call   sub_1_819E
        xchg   edi, edx
        mov    edx, dword ptr [edx - 38h]
        db  089h, 015h, 0A4h, 0F6h, 000h, 000h
        test   edx, edx
        jne    SHORT loc_1_7C68
        pop    ecx
loc_1_7CA6:
        ret
sub_1_7CA7:
        and    byte ptr [edi + 1], 0fbh
        mov    eax, dword ptr [edi + 14h]
        or     byte ptr [edi + 1], 8
        inc    dword ptr [eax - 6ch]
        db  03Bh, 005h, 010h, 0F7h, 000h, 000h
        jne    SHORT loc_1_7CC5
        db  03Bh, 03Dh, 09Ch, 0F6h, 000h, 000h
        jne    SHORT loc_1_7CCD
loc_1_7CC5:
        and    byte ptr [eax + 1], 0fbh
        or     byte ptr [eax + 1], 8
loc_1_7CCD:
        call   sub_1_7C3C
        call   sub_1_81D3
        ret
sub_1_7CD8:
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        test   ecx, 10h
        jne    SHORT loc_1_7CEC
        db  0FFh, 005h, 008h, 0F7h, 000h, 000h
loc_1_7CEC:
        or     byte ptr [edi + 1], 4
        dec    dword ptr [ebx - 6ch]
        test   esi, esi
        je     SHORT loc_1_7D09
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [edi + 8], eax
        mov    dword ptr [esi + 8], edi
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [edi + 4], esi
        mov    dword ptr [edi + 64h], esi
loc_1_7D09:
        test   byte ptr [edi], 1
        je     SHORT loc_1_7D1D
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        jne    SHORT loc_1_7D1D
        cmp    word ptr [edi + 68h], 0
        jne    SHORT loc_1_7D53
loc_1_7D1D:
        mov    eax, ecx
        shl    eax, 11h
        xor    eax, dword ptr [edi]
        and    eax, 400000h
        xor    dword ptr [edi], eax
        push   ecx
        push   edx
        sti
        push   ecx
        push   edi
        call   sub_1_9198
        cli
        pop    edx
        pop    ecx
        call   sub_1_8A51
loc_1_7D3D:
        cli
        test   ecx, 10h
        jne    SHORT loc_1_7D4C
        db  0FFh, 00Dh, 008h, 0F7h, 000h, 000h
loc_1_7D4C:
        and    word ptr [edi + 1], 0bff7h
        ret
loc_1_7D53:
        test   byte ptr [edi], 4
        jne    SHORT loc_1_7D1D
        cmp    byte ptr [ebx - 68h], 0
        je     SHORT loc_1_7D1D
        bts    dword ptr [edi], 1ah
        jb     SHORT loc_1_7D1D
        pushad
        xor    eax, eax
        mov    ecx, 1
        mov    edx, edi
        db  0BEh, 07Ch, 07Dh, 000h, 000h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        popad
        jmp    SHORT loc_1_7D1D
loc_1_7D7C:
        mov    edi, edx
        push   80000000h
        push   0ch
        push   42434854h
        push   3ch
        push   edx
        db  0CDh, 020h, 073h, 001h, 001h, 000h
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_1_7DA5
        cli
        and    byte ptr [edi + 3], 0fbh
        call   sub_1_8168
        sti
loc_1_7DA5:
        ret
sub_1_7DA6:
        push   ebp
        push   edx
        push   edi
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        mov    ebp, dword ptr [edi + 10h]
        test   cl, 8
        jne    SHORT loc_1_7DBD
        cmp    byte ptr [edi - 34h], 0
        je     SHORT loc_1_7DC0
loc_1_7DBD:
        inc    byte ptr [edi - 34h]
loc_1_7DC0:
        cli
        mov    eax, dword ptr [edi]
        push   eax
        push   dword ptr [ebp + 2ch]
        and    ax, 0effeh
        mov    dword ptr [edi], eax
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        test   cl, 3
        je     NEAR PTR loc_1_7E9C
        test   byte ptr [ebp + 2dh], 2
        jne    SHORT loc_1_7DF5
        test   cl, 4
        je     NEAR PTR loc_1_7E9C
        call   sub_1_8041
        call   Enable_VM_Ints
loc_1_7DF5:
        test   cl, 2
        je     SHORT loc_1_7E04
        test   byte ptr [ebx + 1], 20h
        je     NEAR PTR loc_1_7E9C
loc_1_7E04:
        or     byte ptr [edi], 1
loc_1_7E07:
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        and    cl, 7fh
        test   byte ptr [edi], 1
        je     NEAR PTR loc_1_7E9C
        xor    eax, eax
        call   sub_1_216D
        je     NEAR PTR loc_1_7EB7
        test   cl, 20h
        jne    SHORT loc_1_7E73
        test   byte ptr [eax + 8], 2
        je     SHORT loc_1_7E49
        pushad
        test   byte ptr [edi + 1], 10h
        jne    SHORT loc_1_7E43
        call   sub_1_8041
        test   byte ptr [eax + 8], 40h
        jne    SHORT loc_1_7E48
loc_1_7E43:
        call   sub_1_229A
loc_1_7E48:
        popad
loc_1_7E49:
        db  08Bh, 015h, 0D4h, 0F6h, 000h, 000h
        test   edx, edx
        jne    SHORT loc_1_7E65
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        jne    SHORT loc_1_7E69
        db  08Bh, 015h, 0F4h, 0F6h, 000h, 000h
        test   edx, edx
        je     SHORT loc_1_7E69
loc_1_7E65:
        cmp    edx, edi
        jne    SHORT loc_1_7E99
loc_1_7E69:
        sti
        push   ecx
        xor    ecx, ecx
        call   Begin_Critical_Section
        pop    ecx
loc_1_7E73:
        test   byte ptr [edi + 1], 10h
        jne    SHORT loc_1_7E7E
        call   sub_1_8041
loc_1_7E7E:
        sti
        call   Enable_VM_Ints
        mov    al, byte ptr [edi - 33h]
        db  068h, 0CCh, 07Eh, 000h, 000h
        test   al, al
        je     NEAR PTR Resume_Exec
        jmp    NEAR PTR loc_1_2530
loc_1_7E99:
        or     cl, 80h
loc_1_7E9C:
        mov    eax, 400h
        call   sub_1_216D
        je     SHORT loc_1_7EB7
        pushad
        or     byte ptr [edi], 2
        call   sub_1_229A
        and    byte ptr [edi], 0fdh
        popad
        jmp    SHORT loc_1_7E9C
loc_1_7EB7:
        cmp    byte ptr [edi - 34h], 0
        je     SHORT loc_1_7F20
loc_1_7EBD:
        push   ecx
loc_1_7EBE:
        call   sub_1_819E
        call   sub_1_8A51
        pop    ecx
        sti
        jmp    SHORT loc_1_7ED6
loc_1_7ECC:
        test   cl, 20h
        jne    SHORT loc_1_7ED6
        call   End_Critical_Section
loc_1_7ED6:
        cli
        mov    edx, dword ptr [esp + 0ch]
        call   dword ptr [esp + 8]
        jne    NEAR PTR loc_1_7E07
loc_1_7EE5:
        test   byte ptr [edi + 1], 10h
        je     SHORT loc_1_7F07
        call   End_Nest_Exec
        mov    eax, dword ptr [esp]
        xor    ah, byte ptr [ebp + 2dh]
        and    ah, 2
        xor    byte ptr [ebp + 2dh], ah
        cmp    byte ptr [edi - 33h], 0
        je     SHORT loc_1_7F07
        call   sub_1_7B58
loc_1_7F07:
        add    esp, 4
        pop    eax
        xor    eax, dword ptr [edi]
        and    eax, 1001h
        xor    dword ptr [edi], eax
        cmp    byte ptr [edi - 34h], 1
        adc    byte ptr [edi - 34h], 0ffh
        pop    edi
        pop    edx
        pop    ebp
        ret
loc_1_7F20:
        db  081h, 03Dh, 08Ch, 0FCh, 000h, 000h, 088h, 0FCh, 000h, 000h
        je     SHORT loc_1_7F6A
        db  0FFh, 035h, 0E0h, 0D9h, 000h, 000h
        sti
        nop
        mov    edx, dword ptr [esp + 10h]
        cli
        call   dword ptr [esp + 0ch]
        pop    edx
        je     SHORT loc_1_7EE5
        test   byte ptr [edi], 1
        je     SHORT loc_1_7F60
        xor    eax, eax
        db  066h, 00Bh, 005h, 0E2h, 0D9h, 000h, 000h
        je     SHORT loc_1_7F60
        shr    edx, 10h
        cmp    eax, edx
        jne    NEAR PTR loc_1_7E07
        call   sub_1_7C00
loc_1_7F60:
        call   sub_1_7CD8
        jmp    NEAR PTR loc_1_7ED6
loc_1_7F6A:
        db  081h, 03Dh, 000h, 0F8h, 000h, 000h, 0FCh, 0F7h, 000h, 000h
        jne    NEAR PTR loc_1_7EBD
        test   byte ptr [edi], 1
        je     SHORT loc_1_7F84
        test   cl, 80h
        je     SHORT loc_1_7FA0
loc_1_7F84:
        db  0A1h, 0D4h, 0F6h, 000h, 000h
        db  066h, 083h, 03Dh, 0E0h, 0D9h, 000h, 000h, 000h
        jne    SHORT loc_1_7FA9
        db  08Bh, 015h, 010h, 0F7h, 000h, 000h
        cmp    word ptr [edx + 18h], 0
        jne    SHORT loc_1_7FDE
loc_1_7FA0:
        sti
        nop
        nop
        cli
        jmp    NEAR PTR loc_1_7ED6
loc_1_7FA9:
        push   ecx
        xor    ecx, ecx
        inc    ecx
        test   eax, eax
        je     SHORT loc_1_7FBE
loc_1_7FB1:
        cmp    eax, edi
        je     SHORT loc_1_7FB9
        test   dword ptr [eax], ecx
        jne    SHORT loc_1_8017
loc_1_7FB9:
        xor    ecx, ecx
        inc    ecx
        jmp    SHORT loc_1_7FC7
loc_1_7FBE:
        db  0A1h, 0F4h, 0F6h, 000h, 000h
        test   eax, eax
        jne    SHORT loc_1_7FB1
loc_1_7FC7:
        db  08Bh, 01Dh, 014h, 0F7h, 000h, 000h
loc_1_7FCD:
        call   sub_1_8025
        jne    SHORT loc_1_8017
        mov    ebx, dword ptr [ebx + 2ch]
        test   ebx, ebx
        jne    SHORT loc_1_7FCD
loc_1_7FDB:
        pop    ecx
        jmp    SHORT loc_1_7FA0
loc_1_7FDE:
        push   ecx
        db  08Bh, 01Dh, 010h, 0F7h, 000h, 000h
        mov    ecx, 1
        test   eax, eax
        je     SHORT loc_1_8007
loc_1_7FEE:
        cmp    eax, edi
        je     SHORT loc_1_7FFB
        cmp    ebx, dword ptr [eax + 14h]
        jne    SHORT loc_1_8007
        test   dword ptr [eax], ecx
        jne    SHORT loc_1_8017
loc_1_7FFB:
        mov    ecx, 400000h
        jmp    SHORT loc_1_8010
        db  0B9h, 000h, 000h, 040h, 000h
loc_1_8007:
        db  0A1h, 0F4h, 0F6h, 000h, 000h
        test   eax, eax
        jne    SHORT loc_1_7FEE
loc_1_8010:
        call   sub_1_8025
        je     SHORT loc_1_7FDB
loc_1_8017:
        push   edi
        mov    edi, eax
        call   sub_1_7CA7
        pop    edi
        jmp    NEAR PTR loc_1_7EBE
sub_1_8025:
        mov    eax, dword ptr [ebx - 1ch]
loc_1_8028:
        test   eax, eax
        je     SHORT loc_1_8040
        cmp    eax, edi
        je     SHORT loc_1_8034
        test   dword ptr [eax], ecx
        jne    SHORT loc_1_8039
loc_1_8034:
        mov    eax, dword ptr [eax + 40h]
        jmp    SHORT loc_1_8028
loc_1_8039:
        test   byte ptr [eax], 18h
        jne    SHORT loc_1_8034
        test   eax, eax
loc_1_8040:
        ret
sub_1_8041:
        or     byte ptr [edi + 1], 10h
        cmp    byte ptr [edi - 33h], 1
        sbb    byte ptr [edi - 33h], 0ffh
        call   Begin_Nest_Exec
        ret
loc_1_8053:
        cmp    byte ptr [edx + 0ch], 0
        ret
_BlockOnID:
        sub    esp, 10h
        pushad
        mov    eax, dword ptr [esp + 34h]
        db  08Bh, 00Dh, 098h, 0F6h, 000h, 000h
        mov    byte ptr [esp + 2ch], 0ffh
        mov    dword ptr [esp + 24h], eax
        mov    dword ptr [esp + 28h], ecx
        inc    byte ptr [ecx + 72h]
        lea    eax, [esp + 20h]
        mov    edx, eax
        cli
        db  087h, 005h, 004h, 0F7h, 000h, 000h
        mov    dword ptr [esp + 20h], eax
        db  0BFh, 053h, 080h, 000h, 000h
        xor    esi, esi
        mov    ecx, dword ptr [esp + 38h]
        sti
        call   sub_1_7DA6
loc_1_8098:
        mov    eax, edx
        db  0BAh, 004h, 0F7h, 000h, 000h
loc_1_809F:
        mov    ecx, dword ptr [edx]
        cmp    ecx, eax
        je     SHORT loc_1_80AB
        test   ecx, ecx
        mov    edx, ecx
        jmp    SHORT loc_1_809F
loc_1_80AB:
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        sti
        db  08Bh, 015h, 098h, 0F6h, 000h, 000h
        dec    byte ptr [edx + 72h]
        popad
        add    esp, 10h
        ret
        db  08Bh, 0C0h
loc_1_80C0:
        push   edi
        db  0BAh, 004h, 0F7h, 000h, 000h
loc_1_80C6:
        mov    eax, dword ptr [esp + 8]
loc_1_80CA:
        mov    edx, dword ptr [edx]
        test   edx, edx
        je     SHORT loc_1_80F3
        cmp    dword ptr [edx + 4], eax
        jne    SHORT loc_1_80CA
        pushfd
        cli
        mov    byte ptr [edx + 0ch], 0
        mov    eax, dword ptr [edx + 8]
        test   byte ptr [eax + 1], 4
        je     SHORT loc_1_80F0
        test   byte ptr [eax], 10h
        jne    SHORT loc_1_80F0
        mov    edi, eax
        call   sub_1_7CA7
loc_1_80F0:
        popfd
        jmp    SHORT loc_1_80C6
loc_1_80F3:
        pop    edi
        ret
sub_1_80F5:
        pushad
        mov    edi, dword ptr [esp + 24h]
        pushfd
        cli
        cmp    byte ptr [edi + 72h], 0
        je     SHORT loc_1_811D
        db  0BAh, 004h, 0F7h, 000h, 000h
loc_1_8107:
        mov    ecx, dword ptr [edx]
        jecxz  loc_1_811D
        cmp    dword ptr [ecx + 8], edi
        je     SHORT loc_1_8114
        mov    edx, ecx
        jmp    SHORT loc_1_8107
loc_1_8114:
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [edx], ecx
        dec    byte ptr [edi + 72h]
        jmp    SHORT loc_1_8107
loc_1_811D:
        popfd
        popad
        ret    4
        db  000h, 000h
sub_1_8124:
        xchg   dword ptr [ebx - 1ch], ecx
        mov    dword ptr [edi + 40h], ecx
        ret
sub_1_812B:
        mov    esi, ebx
        db  087h, 035h, 014h, 0F7h, 000h, 000h
        mov    dword ptr [ebx + 2ch], esi
        or     dword ptr [ebx], 0ch
        mov    dword ptr [ebx - 14h], 1
        ret
sub_1_8141:
        pushad
        push   edi
        call   sub_1_80F5
        mov    ecx, dword ptr [edi + 54h]
        db  0E8h, 064h, 083h, 0FEh, 0FFh
        pushfd
        cli
        lea    eax, [ebx - 1ch]
loc_1_8155:
        mov    esi, dword ptr [eax]
        cmp    edi, esi
        je     SHORT loc_1_8160
        lea    eax, [esi + 40h]
        jmp    SHORT loc_1_8155
loc_1_8160:
        mov    ecx, dword ptr [edi + 40h]
        mov    dword ptr [eax], ecx
        popfd
        popad
        ret
sub_1_8168:
        test   byte ptr [edi + 1], 4
        je     SHORT loc_1_8196
        test   byte ptr [edi], 1
        je     SHORT loc_1_8196
        test   byte ptr [edi], 8
        jne    SHORT loc_1_8196
        push   eax
        push   ecx
        push   edx
        test   byte ptr [edi + 2], 40h
        jne    SHORT loc_1_818E
        db  0A1h, 0D4h, 0F6h, 000h, 000h
        test   eax, eax
        je     SHORT loc_1_818E
        cmp    eax, edi
        jne    SHORT loc_1_8197
loc_1_818E:
        call   sub_1_7CA7
loc_1_8193:
        pop    edx
        pop    ecx
        pop    eax
loc_1_8196:
        ret
loc_1_8197:
        call   sub_1_7C00
        jmp    SHORT loc_1_8193
sub_1_819E:
        mov    eax, dword ptr [edi + 64h]
        test   eax, eax
        je     SHORT loc_1_81B6
        mov    eax, dword ptr [edi + 8]
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [ecx + 8], eax
        xor    eax, eax
        mov    dword ptr [edi + 64h], eax
loc_1_81B6:
        test   byte ptr [edi], 8
        jne    SHORT loc_1_81D2
        db  0B8h, 088h, 0FCh, 000h, 000h
        mov    ecx, dword ptr [eax + 8]
        mov    dword ptr [ecx + 4], edi
        mov    dword ptr [edi + 4], eax
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [eax + 8], edi
        mov    dword ptr [edi + 64h], eax
loc_1_81D2:
        ret
sub_1_81D3:
        push   esi
        mov    eax, dword ptr [edi + 64h]
        test   eax, eax
        je     SHORT loc_1_81EC
        mov    eax, dword ptr [edi + 8]
        mov    esi, dword ptr [edi + 4]
        mov    dword ptr [eax + 4], esi
        mov    dword ptr [esi + 8], eax
        xor    eax, eax
        mov    dword ptr [edi + 64h], eax
loc_1_81EC:
        test   byte ptr [edi], 8
        jne    SHORT loc_1_821D
        db  0BEh, 0FCh, 0F7h, 000h, 000h
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [edi + 4], eax
        mov    dword ptr [esi + 4], edi
        mov    dword ptr [eax + 8], edi
        mov    dword ptr [edi + 8], esi
        mov    dword ptr [edi + 64h], esi
        db  00Fh, 0BAh, 02Dh, 0B4h, 0F6h, 000h, 000h, 000h
        jb     SHORT loc_1_821D
        db  0BEh, 0B7h, 082h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
loc_1_821D:
        pop    esi
        ret
sub_1_821F:
        pushad
        mov    edi, dword ptr [esi + 4]
        cmp    edi, esi
        je     NEAR PTR loc_1_82B5
        cmp    dword ptr [edi + 4], esi
        je     SHORT loc_1_8249
        mov    edx, edi
        mov    ecx, dword ptr [edi + 3ch]
loc_1_8235:
        mov    edx, dword ptr [edx + 4]
        cmp    edx, esi
        je     SHORT loc_1_8249
        mov    eax, dword ptr [edx + 3ch]
        cmp    ecx, eax
        jae    SHORT loc_1_8235
        mov    ecx, eax
        mov    edi, edx
        jmp    SHORT loc_1_8235
loc_1_8249:
        call   sub_1_7C3C
        and    byte ptr [edi + 1], 0fbh
        mov    eax, dword ptr [edi + 14h]
        inc    dword ptr [eax - 6ch]
        or     byte ptr [edi + 1], 8
        db  03Bh, 005h, 010h, 0F7h, 000h, 000h
        jne    SHORT loc_1_826C
        db  03Bh, 03Dh, 09Ch, 0F6h, 000h, 000h
        jne    SHORT loc_1_8274
loc_1_826C:
        and    byte ptr [eax + 1], 0fbh
        or     byte ptr [eax + 1], 8
loc_1_8274:
        test   byte ptr [esp + 1ch], 2
        jne    SHORT loc_1_829C
        db  083h, 03Dh, 0F8h, 0E3h, 000h, 000h, 000h
        jne    SHORT loc_1_82B0
        call   sub_1_819E
        test   byte ptr [esp + 1ch], 1
        je     SHORT loc_1_8296
        db  0CDh, 020h, 036h, 001h, 001h, 000h
loc_1_8296:
        popad
        jmp    NEAR PTR sub_1_8ACC
loc_1_829C:
        db  0A1h, 098h, 0F6h, 000h, 000h
        mov    eax, dword ptr [eax + 3ch]
        cmp    eax, dword ptr [edi + 3ch]
        jb     SHORT loc_1_82B0
        call   sub_1_819E
        popad
        ret
loc_1_82B0:
        call   sub_1_81D3
loc_1_82B5:
        popad
        ret
loc_1_82B7:
        cli
        db  0C6h, 005h, 0B4h, 0F6h, 000h, 000h, 000h
        db  08Bh, 03Dh, 000h, 0F8h, 000h, 000h
        db  081h, 0FFh, 0FCh, 0F7h, 000h, 000h
        je     SHORT loc_1_82E5
loc_1_82CD:
        call   sub_1_819E
        db  08Bh, 03Dh, 000h, 0F8h, 000h, 000h
        db  081h, 0FFh, 0FCh, 0F7h, 000h, 000h
        jne    SHORT loc_1_82CD
        jmp    NEAR PTR sub_1_8ACC
loc_1_82E5:
        sti
        ret
        db  090h
Adjust_Exec_Priority:
        cli
        push   edi
        mov    edi, dword ptr [ebx - 1ch]
loc_1_82ED:
        add    dword ptr [edi + 3ch], eax
        mov    edi, dword ptr [edi + 40h]
        test   edi, edi
        jne    SHORT loc_1_82ED
        mov    edi, dword ptr [ebx - 64h]
        add    edi, eax
        pushfd
        jae    SHORT loc_1_8319
        cmp    edi, 100000h
        jae    SHORT loc_1_8319
        cmp    dword ptr [ebx - 64h], 100000h
        jb     SHORT loc_1_8319
        push   eax
        push   edx
        call   sub_1_7C5D
        pop    edx
        pop    eax
loc_1_8319:
        mov    dword ptr [ebx - 64h], edi
        db  03Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        je     SHORT loc_1_8328
        xor    byte ptr [esp], 1
loc_1_8328:
        popfd
        pop    edi
        jb     NEAR PTR sub_1_8ACC
        sti
        ret
        db  08Bh, 0C0h
loc_1_8334:
        cli
        add    dword ptr [edi + 3ch], eax
        db  03Bh, 03Dh, 098h, 0F6h, 000h, 000h
        jne    SHORT loc_1_834A
        test   eax, eax
        js     NEAR PTR sub_1_8ACC
loc_1_8348:
        sti
        ret
loc_1_834A:
        test   eax, eax
        js     SHORT loc_1_8348
        db  081h, 07Fh, 064h, 088h, 0FCh, 000h, 000h
        jne    SHORT loc_1_8348
        push   eax
        push   ecx
        mov    ecx, dword ptr [edi + 4]
        db  081h, 0F9h, 088h, 0FCh, 000h, 000h
        je     SHORT loc_1_8381
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [ecx + 8], eax
        db  0B8h, 088h, 0FCh, 000h, 000h
        mov    ecx, dword ptr [eax + 8]
        mov    dword ptr [ecx + 4], edi
        mov    dword ptr [edi + 4], eax
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [eax + 8], edi
loc_1_8381:
        pop    ecx
        pop    eax
        jmp    NEAR PTR sub_1_8ACC
sub_1_8388:
        cli
        add    dword ptr [edi + 3ch], eax
        db  03Bh, 03Dh, 098h, 0F6h, 000h, 000h
        jne    SHORT loc_1_8396
loc_1_8394:
        sti
        ret
loc_1_8396:
        test   eax, eax
        js     SHORT loc_1_8394
        db  081h, 07Fh, 064h, 088h, 0FCh, 000h, 000h
        jne    SHORT loc_1_8348
        push   eax
        push   ecx
        mov    ecx, dword ptr [edi + 4]
        db  081h, 0F9h, 088h, 0FCh, 000h, 000h
        je     SHORT loc_1_83CD
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [ecx + 8], eax
        db  0B8h, 088h, 0FCh, 000h, 000h
        mov    ecx, dword ptr [eax + 8]
        mov    dword ptr [ecx + 4], edi
        mov    dword ptr [edi + 4], eax
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [eax + 8], edi
loc_1_83CD:
        pop    ecx
        pop    eax
        sti
        ret
Suspend_VM:
        pushad
        cli
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        stc
        je     SHORT loc_1_8440
        mov    edi, dword ptr [ebx - 20h]
        db  03Bh, 03Dh, 0D4h, 0F6h, 000h, 000h
        stc
        je     SHORT loc_1_8440
        cmp    byte ptr [edi - 33h], 0
        jne    SHORT loc_1_8443
        mov    eax, dword ptr [ebx - 14h]
        inc    dword ptr [ebx - 14h]
        test   eax, eax
        jne    SHORT loc_1_843F
        db  03Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        je     SHORT loc_1_844B
        or     byte ptr [ebx], 8
        or     byte ptr [edi], 8
        xor    eax, eax
        xchg   dword ptr [edi + 64h], eax
        test   eax, eax
        je     SHORT loc_1_841B
        mov    eax, dword ptr [edi + 8]
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [ecx + 8], eax
loc_1_841B:
        call   sub_1_7C3C
        test   byte ptr [edi + 1], 4
        je     SHORT loc_1_8429
        inc    dword ptr [ebx - 6ch]
loc_1_8429:
        and    byte ptr [edi + 1], 0f3h
        and    byte ptr [ebx + 1], 0f3h
        sti
        push   3
        push   edi
        call   sub_1_8F05
        call   sub_1_8484
loc_1_843F:
        clc
loc_1_8440:
        sti
        popad
        ret
loc_1_8443:
        inc    dword ptr [ebx - 14h]
        or     byte ptr [edi], 80h
        jmp    SHORT loc_1_843F
loc_1_844B:
        call   sub_1_76A6
        jmp    SHORT loc_1_843F
sub_1_8452:
        pushfd
        cli
        bts    dword ptr [edi], 3
        jb     SHORT loc_1_8480
        mov    eax, dword ptr [edi + 64h]
        db  03Dh, 088h, 0FCh, 000h, 000h
        je     SHORT loc_1_846B
        db  03Dh, 0FCh, 0F7h, 000h, 000h
        jne    SHORT loc_1_847B
loc_1_846B:
        mov    eax, dword ptr [edi + 8]
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [ecx + 8], eax
        and    dword ptr [edi + 64h], 0
loc_1_847B:
        call   sub_1_7C3C
loc_1_8480:
        popfd
        ret
loc_1_8482:
        mov    ebx, edx
sub_1_8484:
        mov    ecx, 1
        call   Begin_Critical_Section
        mov    eax, 0dh
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        db  0E8h, 062h, 07Bh, 0FEh, 0FFh
        db  0E8h, 075h, 07Bh, 0FEh, 0FFh
        mov    eax, dword ptr [ebx - 20h]
        cmp    byte ptr [eax + 72h], 0
        jne    SHORT loc_1_84C1
        mov    eax, dword ptr [eax + 54h]
        shr    eax, 0ch
        inc    eax
        push   0
        push   1
        push   eax
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
loc_1_84C1:
        push   ebx
        db  0E8h, 079h, 07Bh, 0FEh, 0FFh
        jmp    NEAR PTR End_Critical_Section
Resume_VM:
        pushad
        dec    dword ptr [ebx - 14h]
        js     SHORT loc_1_84F7
        jne    SHORT loc_1_84ED
        mov    edi, dword ptr [ebx - 20h]
        btr    dword ptr [edi], 7
        jb     SHORT loc_1_84ED
        call   sub_1_852B
        jb     SHORT loc_1_84F1
        cli
        and    byte ptr [ebx], 0f7h
        call   sub_1_84FC
loc_1_84ED:
        clc
loc_1_84EE:
        sti
        popad
        ret
loc_1_84F1:
        inc    dword ptr [ebx - 14h]
        stc
        jmp    SHORT loc_1_84EE
loc_1_84F7:
        inc    dword ptr [ebx - 14h]
        jmp    SHORT loc_1_84ED
sub_1_84FC:
        pushfd
        cli
        btr    dword ptr [edi], 3
        jae    SHORT loc_1_8529
        push   2
        push   edi
        call   sub_1_8F05
        test   byte ptr [edi + 1], 4
        jne    SHORT loc_1_8529
        test   byte ptr [edi + 1], 8
        je     SHORT loc_1_851F
        call   sub_1_81D3
        jmp    SHORT loc_1_8529
loc_1_851F:
        call   sub_1_819E
        call   sub_1_8ACC
loc_1_8529:
        popfd
        ret
sub_1_852B:
        mov    ecx, 1
        call   Begin_Critical_Section
        mov    eax, dword ptr [ebx - 20h]
        cmp    byte ptr [eax + 72h], 0
        jne    SHORT loc_1_8557
        mov    eax, dword ptr [eax + 54h]
        shr    eax, 0ch
        inc    eax
        push   0
        push   1
        push   eax
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_858F
loc_1_8557:
        push   ebx
        db  0E8h, 0E3h, 07Ah, 0FEh, 0FFh
        or     eax, eax
        je     SHORT loc_1_858F
        db  0E8h, 0B6h, 07Ah, 0FEh, 0FFh
        jb     SHORT loc_1_8589
        db  0E8h, 093h, 07Ah, 0FEh, 0FFh
        jb     SHORT loc_1_8589
        mov    eax, 0eh
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        jb     SHORT loc_1_8584
loc_1_857C:
        pushfd
        call   End_Critical_Section
        popfd
        ret
loc_1_8584:
        db  0E8h, 08Fh, 07Ah, 0FEh, 0FFh
loc_1_8589:
        push   ebx
        db  0E8h, 0B1h, 07Ah, 0FEh, 0FFh
loc_1_858F:
        stc
        jmp    SHORT loc_1_857C
End_Crit_And_Suspend:
        pushad
        xor    ecx, ecx
        db  0BEh, 0C4h, 0F6h, 000h, 000h
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        cmp    dword ptr [esi + 0ch], 1
        jne    SHORT loc_1_85E5
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        je     SHORT loc_1_85E5
        cmp    edi, dword ptr [esi + 10h]
        jne    SHORT loc_1_85E5
        dec    dword ptr [esi + 0ch]
        sub    dword ptr [edi + 3ch], 100000h
        mov    dword ptr [esi + 10h], ecx
        cmp    esi, dword ptr [esi + 4]
        je     SHORT loc_1_85D7
        cli
        mov    eax, 2
        call   sub_1_821F
        sti
loc_1_85D7:
        db  0CDh, 020h, 02Bh, 000h, 001h, 000h
        jae    SHORT loc_1_85E6
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
loc_1_85E5:
        stc
loc_1_85E6:
        popad
        ret
Claim_Critical_Section:
        test   ax, ax
        je     SHORT loc_1_8613
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        dec    eax
        db  066h, 001h, 005h, 0D0h, 0F6h, 000h, 000h
        push   ebx
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        pop    ebx
        jne    SHORT loc_1_8612
        db  066h, 001h, 005h, 0F0h, 0F6h, 000h, 000h
loc_1_8612:
        inc    eax
loc_1_8613:
        ret
Release_Critical_Section:
        or     cx, cx
        je     SHORT loc_1_8642
        push   ebx
        dec    ecx
        db  066h, 029h, 00Dh, 0D0h, 0F6h, 000h, 000h
        db  08Bh, 01Dh, 0D4h, 0F6h, 000h, 000h
        mov    ebx, dword ptr [ebx + 14h]
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        jne    SHORT loc_1_863A
        db  066h, 029h, 00Dh, 0F0h, 0F6h, 000h, 000h
loc_1_863A:
        db  0CDh, 020h, 020h, 000h, 001h, 000h
        inc    ecx
        pop    ebx
loc_1_8642:
        ret
        db  090h
Get_Crit_Section_Status:
        db  03Dh, 0D1h, 079h, 0FEh, 0FFh
        mov    eax, eax
Get_Crit_Status_No_Block:
        push   esi
        db  0BEh, 0C4h, 0F6h, 000h, 000h
        mov    ecx, dword ptr [esi + 0ch]
        jecxz  loc_1_8660
loc_1_8657:
        mov    ebx, dword ptr [esi + 10h]
        mov    ebx, dword ptr [ebx + 14h]
        stc
        pop    esi
        ret
loc_1_8660:
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        jne    SHORT loc_1_8681
        db  0BEh, 0E4h, 0F6h, 000h, 000h
        mov    ecx, dword ptr [esi + 0ch]
        shl    ecx, 10h
        jne    SHORT loc_1_8657
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
loc_1_8681:
        db  08Bh, 035h, 098h, 0F6h, 000h, 000h
        cmp    dword ptr [esi + 3ch], 100000h
        cmc
        pop    esi
        ret
        db  02Eh, 08Bh, 0C0h
loc_1_8694:
        db  0BFh, 0C4h, 0F6h, 000h, 000h
        mov    ecx, dword ptr [edi + 0ch]
        test   ecx, ecx
        je     SHORT loc_1_86A5
loc_1_86A0:
        mov    edi, dword ptr [edi + 10h]
        stc
        ret
loc_1_86A5:
        db  08Bh, 03Dh, 00Ch, 0F7h, 000h, 000h
        db  03Bh, 03Dh, 010h, 0F7h, 000h, 000h
        jne    SHORT loc_1_86C0
        db  0BFh, 0E4h, 0F6h, 000h, 000h
        mov    ecx, dword ptr [edi + 0ch]
        shl    ecx, 10h
        jne    SHORT loc_1_86A0
loc_1_86C0:
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        cmp    dword ptr [edi + 3ch], 100000h
        cmc
        ret
        db  090h
Call_When_Not_Critical:
        pushad
        xor    ecx, ecx
        db  039h, 00Dh, 0D4h, 0F6h, 000h, 000h
        jne    SHORT loc_1_8708
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        jne    SHORT loc_1_86F1
        db  039h, 00Dh, 0F4h, 0F6h, 000h, 000h
        jne    SHORT loc_1_8708
loc_1_86F1:
        db  0A1h, 098h, 0F6h, 000h, 000h
        cmp    dword ptr [eax + 3ch], 100000h
        jae    SHORT loc_1_8708
        mov    edi, eax
        mov    ebp, dword ptr [eax + 10h]
        call   esi
        popad
        ret
loc_1_8708:
        xor    eax, eax
        xor    ebx, ebx
        mov    ecx, 2
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        popad
        ret
loc_1_8719:
        ret
sub_1_871A:
        xor    ecx, ecx
        xchg   esi, edi
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_1_8749
loc_1_8726:
        mov    ebx, eax
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        xchg   ebx, eax
        cmp    dword ptr [eax], edi
        jne    SHORT loc_1_8743
        pushfd
        cli
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        popfd
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        inc    ecx
loc_1_8743:
        mov    eax, ebx
        or     eax, eax
        jne    SHORT loc_1_8726
loc_1_8749:
        cmp    ecx, 1
        ret
Create_Semaphore:
        cmp    ecx, 7fffh
        ja     SHORT loc_1_877A
        push   ecx
        push   edx
        push   1
        push   0ch
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    edx
        pop    ecx
        test   eax, eax
        je     SHORT loc_1_877A
        mov    word ptr [eax + 2], 4553h
        mov    dword ptr [eax + 4], eax
        mov    dword ptr [eax + 8], eax
        mov    word ptr [eax], cx
        clc
        ret
loc_1_877A:
        stc
        ret
Destroy_Semaphore:
        cmp    word ptr [eax + 2], 4553h
        jne    SHORT loc_1_879A
        push   eax
        push   ecx
        push   edx
        xor    word ptr [eax + 2], 20h
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    edx
        pop    ecx
        pop    eax
loc_1_879A:
        ret
loc_1_879B:
        push   ebp
        mov    ebp, esp
        db  0A1h, 098h, 0F6h, 000h, 000h
        mov    edx, esp
        sub    edx, dword ptr [eax + 54h]
        shr    edx, 0ch
        dec    edx
        jne    SHORT loc_1_87B2
        or     byte ptr [eax + 3], 40h
loc_1_87B2:
        push   eax
        push   edx
        mov    eax, esp
        mov    esp, dword ptr [ebp + 10h]
        push   eax
        push   dword ptr [ebp + 0ch]
        call   dword ptr [ebp + 8]
        pop    eax
        pop    esp
        pop    edx
        pop    eax
        test   edx, edx
        jne    SHORT loc_1_87CC
        and    byte ptr [eax + 3], 0bfh
loc_1_87CC:
        leave
        ret
loc_1_87CE:
        inc    edx
        push   ebp
        inc    edi
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax + ebp*2 + 72h], dl
        popad
        and    byte ptr fs:[ebp + 73h], dh
        imul   ebp, dword ptr [esi + 67h], 726f6d20h
        and    byte ptr gs:[eax + ebp*2 + 61h], dh
        outsb
        and    byte ptr [ebx + ecx*2], dh
        bound  esp, qword ptr [eax]
        outsd
        and    byte ptr [ebx + 74h], dh
        popad
        arpl   word ptr [ebx + 20h], bp
        db  073h, 070h
        popad
        arpl   word ptr [ebp + 0dh], sp
        or     al, byte ptr [eax]
loc_1_8800:
        push   esi
        mov    eax, dword ptr [esi - 5ch]
        test   eax, eax
        je     SHORT loc_1_8859
        bts    dword ptr [esi], 1dh
        jb     SHORT loc_1_8853
        cmp    byte ptr [eax + 3], 0
        jne    SHORT loc_1_8853
        inc    edx
        jne    SHORT loc_1_8822
        db  068h, 0CEh, 087h, 000h, 000h
        db  0CDh, 020h, 0F4h, 000h, 001h, 000h
loc_1_8822:
        inc    byte ptr [eax + 3]
        db  08Bh, 035h, 038h, 0F7h, 000h, 000h
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        db  0FFh, 00Dh, 034h, 0F7h, 000h, 000h
        jg     SHORT loc_1_8853
        db  083h, 03Dh, 040h, 0F7h, 000h, 000h, 000h
        jne    SHORT loc_1_8853
        db  0BEh, 036h, 02Eh, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        db  089h, 035h, 040h, 0F7h, 000h, 000h
loc_1_8853:
        pop    esi
        jmp    NEAR PTR loc_1_891D
loc_1_8859:
        mov    edx, esi
        db  0BEh, 0B8h, 02Dh, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        jmp    SHORT loc_1_8853
loc_1_8868:
        mov    eax, dword ptr [esi - 5ch]
        mov    byte ptr [eax + 3], 0
        push   esi
        db  08Bh, 035h, 038h, 0F7h, 000h, 000h
        db  0CDh, 020h, 09Eh, 000h, 001h, 000h
        pop    esi
        db  0FFh, 005h, 034h, 0F7h, 000h, 000h
        and    byte ptr [esi + 3], 0dfh
        jmp    NEAR PTR loc_1_891D
loc_1_888C:
        push   esi
        call   sub_1_89F1
        pop    esi
        jmp    NEAR PTR loc_1_8938
sub_1_8898:
        push   fs
        push   gs
        xor    eax, eax
        mov    fs, ax
        mov    gs, ax
        mov    ebx, dword ptr [edi + 14h]
        call   sub_1_90A9
        db  0CDh, 020h, 001h, 000h, 005h, 000h
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        call   sub_1_90F4
        db  03Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        je     SHORT loc_1_88DB
        call   sub_1_2C8C
        cli
        mov    eax, dword ptr [ebx - 0ch]
        mov    dword ptr [0ffbfe000h], eax
        db  0A1h, 0CCh, 0FCh, 000h, 000h
        mov    cr3, eax
loc_1_88DB:
        cli
        mov    ax, word ptr [edi + 68h]
        add    ax, word ptr [ebx + 18h]
        db  066h, 0A3h, 0E2h, 0D9h, 000h, 000h
        mov    esi, edi
        db  087h, 035h, 098h, 0F6h, 000h, 000h
        mov    eax, dword ptr [edi + 58h]
        db  087h, 005h, 0A0h, 09Eh, 000h, 000h
        mov    dword ptr [esi + 58h], eax
loc_1_88FE:
        test   byte ptr [esi + 3], 40h
        jne    SHORT loc_1_891D
        mov    edx, esp
        sub    edx, dword ptr [esi + 54h]
        shr    edx, 0ch
        dec    edx
        jne    NEAR PTR loc_1_8800
        test   byte ptr [esi + 3], 20h
        jne    NEAR PTR loc_1_8868
loc_1_891D:
        test   byte ptr [edi + 3], 20h
        jne    SHORT loc_1_8938
        mov    edx, dword ptr [edi - 5ch]
        test   edx, edx
        je     NEAR PTR loc_1_888C
        cmp    byte ptr [edx + 3], 0
        jne    NEAR PTR loc_1_888C
loc_1_8938:
        mov    dword ptr [esi + 5ch], esp
        mov    esp, dword ptr [edi + 5ch]
        mov    ebp, ebx
        db  087h, 02Dh, 00Ch, 0F7h, 000h, 000h
        and    byte ptr [esi + 3], 0f7h
        mov    eax, dword ptr [ebp]
        and    eax, 120h
        mov    dword ptr [esi + 60h], eax
        mov    eax, dword ptr [ebx]
        and    eax, 0fffffedfh
        or     eax, dword ptr [edi + 60h]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [ebx + 8], eax
        mov    dword ptr [ebx - 20h], edi
        test   byte ptr [eax + 2dh], 2
        jne    SHORT loc_1_8973
        dec    byte ptr [ebx - 68h]
loc_1_8973:
        mov    eax, dword ptr [esi + 10h]
        test   byte ptr [eax + 2dh], 2
        jne    SHORT loc_1_8986
        inc    byte ptr [ebp - 68h]
        and    dword ptr [eax + 2ch], 0ffffcfffh
loc_1_8986:
        db  08Dh, 005h, 090h, 09Bh, 000h, 000h
        test   byte ptr [ebx], 20h
        je     SHORT loc_1_8994
        lea    eax, [ebx - 32h]
loc_1_8994:
        lidt   [eax]
        sti
        push   esi
        cmp    ebx, ebp
        jne    SHORT loc_1_89C2
        db  08Bh, 00Dh, 020h, 0F7h, 000h, 000h
        mov    ecx, dword ptr [ecx]
        jmp    SHORT loc_1_89B6
loc_1_89A7:
        mov    eax, dword ptr [esp]
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        push   dword ptr [ecx - 4]
        call   dword ptr [ecx]
        pop    ecx
loc_1_89B6:
        test   ecx, ecx
        jne    SHORT loc_1_89A7
        add    esp, 4
        pop    gs
        pop    fs
        ret
loc_1_89C2:
        lldt   word ptr [ebx + 1ah]
        push   ebp
        db  08Bh, 00Dh, 01Ch, 0F7h, 000h, 000h
        mov    ecx, dword ptr [ecx]
        jmp    SHORT loc_1_89E0
loc_1_89D1:
        mov    eax, dword ptr [esp]
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        push   dword ptr [ecx - 4]
        call   dword ptr [ecx]
        pop    ecx
loc_1_89E0:
        test   ecx, ecx
        jne    SHORT loc_1_89D1
        add    esp, 4
        db  08Bh, 00Dh, 020h, 0F7h, 000h, 000h
        mov    ecx, dword ptr [ecx]
        jmp    SHORT loc_1_89B6
sub_1_89F1:
        db  08Bh, 035h, 038h, 0F7h, 000h, 000h
        test   esi, esi
        je     SHORT loc_1_8A24
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        mov    dword ptr [edi - 5ch], eax
        push   eax
        je     SHORT loc_1_8A25
        mov    ecx, dword ptr [eax]
loc_1_8A09:
        mov    eax, dword ptr [edi + 54h]
        shr    eax, 0ch
        push   60080h
        push   ecx
        push   1
        push   eax
        db  0CDh, 020h, 028h, 001h, 001h, 000h
        add    esp, 10h
        pop    eax
        test   eax, eax
loc_1_8A24:
        ret
loc_1_8A25:
        db  08Bh, 00Dh, 0D8h, 0F7h, 000h, 000h
        jmp    SHORT loc_1_8A09
sub_1_8A2D:
        cli
        db  081h, 03Dh, 08Ch, 0FCh, 000h, 000h, 088h, 0FCh, 000h, 000h
        jne    SHORT sub_1_8A51
        db  081h, 03Dh, 000h, 0F8h, 000h, 000h, 0FCh, 0F7h, 000h, 000h
        jne    SHORT sub_1_8A51
        db  08Bh, 03Dh, 09Ch, 0F6h, 000h, 000h
        call   sub_1_7CA7
sub_1_8A51:
        pushad
        cli
        db  0BAh, 0FCh, 0F7h, 000h, 000h
        db  0BEh, 088h, 0FCh, 000h, 000h
        mov    eax, dword ptr [edx + 4]
        cmp    eax, edx
        je     SHORT loc_1_8A88
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [eax + 8], ecx
        mov    dword ptr [edx + 4], edx
        mov    ecx, dword ptr [edx + 8]
        mov    dword ptr [edx + 8], edx
        mov    dword ptr [ecx + 4], esi
        mov    dword ptr [esi + 8], ecx
loc_1_8A7C:
        mov    dword ptr [eax + 64h], esi
        cmp    eax, ecx
        je     SHORT loc_1_8A88
        mov    eax, dword ptr [eax + 4]
        jmp    SHORT loc_1_8A7C
loc_1_8A88:
        mov    edi, dword ptr [esi + 4]
        cmp    edi, esi
        je     SHORT loc_1_8AC7
        mov    edx, edi
        mov    ecx, dword ptr [edi + 3ch]
loc_1_8A94:
        mov    edx, dword ptr [edx + 4]
        cmp    edx, esi
        je     SHORT loc_1_8AA8
        mov    eax, dword ptr [edx + 3ch]
        cmp    ecx, eax
        jae    SHORT loc_1_8A94
        mov    ecx, eax
        mov    edi, edx
        jmp    SHORT loc_1_8A94
loc_1_8AA8:
        mov    eax, dword ptr [edi + 8]
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [ecx + 8], eax
        xor    eax, eax
        mov    dword ptr [edi + 64h], eax
        sti
        db  03Bh, 03Dh, 098h, 0F6h, 000h, 000h
        je     SHORT loc_1_8AC7
        call   sub_1_8898
loc_1_8AC7:
        popad
        ret
        db  02Eh, 08Bh, 0C0h
sub_1_8ACC:
        db  081h, 03Dh, 08Ch, 0FCh, 000h, 000h, 088h, 0FCh, 000h, 000h
        je     SHORT loc_1_8B0F
        pushad
loc_1_8AD9:
        db  0BEh, 088h, 0FCh, 000h, 000h
        mov    edi, dword ptr [esi + 4]
        mov    edx, edi
        mov    ecx, dword ptr [edi + 3ch]
loc_1_8AE6:
        mov    edx, dword ptr [edx + 4]
        cmp    edx, esi
        je     SHORT loc_1_8AFA
        mov    eax, dword ptr [edx + 3ch]
        cmp    ecx, eax
        jae    SHORT loc_1_8AE6
        mov    ecx, eax
        mov    edi, edx
        jmp    SHORT loc_1_8AE6
loc_1_8AFA:
        mov    eax, edi
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        test   byte ptr [edi + 3], 1
        je     SHORT loc_1_8B09
        inc    ecx
loc_1_8B09:
        cmp    dword ptr [edi + 3ch], ecx
        jb     SHORT loc_1_8B31
        popad
loc_1_8B0F:
        sti
        ret
loc_1_8B11:
        cmp    byte ptr [edi + 3fh], 40h
        jae    SHORT loc_1_8B3A
        add    byte ptr [edi + 3fh], 40h
        xor    eax, eax
        mov    ecx, 308h
        mov    ebx, edi
        db  0BEh, 054h, 08Bh, 000h, 000h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        jmp    SHORT loc_1_8AD9
loc_1_8B31:
        mov    ebp, dword ptr [edi + 10h]
        test   byte ptr [ebp + 2dh], 2
        je     SHORT loc_1_8B11
loc_1_8B3A:
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ecx + 4], edi
        mov    dword ptr [edi + 4], esi
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [esi + 8], edi
        mov    dword ptr [edi + 64h], esi
        mov    edi, eax
        jmp    NEAR PTR loc_1_8AA8
        db  090h
loc_1_8B54:
        cli
        sub    byte ptr [edi + 3fh], 40h
        jmp    NEAR PTR sub_1_8ACC
Nuke_VM:
        pushad
        db  03Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        je     SHORT loc_1_8B95
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        je     NEAR PTR loc_1_8C14
        mov    edi, dword ptr [ebx - 20h]
        cmp    byte ptr [edi - 33h], 0
        je     SHORT loc_1_8B82
        or     byte ptr [edi + 1], 20h
        jmp    SHORT loc_1_8B93
loc_1_8B82:
        call   sub_1_79AD
        mov    ecx, 2
        mov    edx, ebx
        db  0E8h, 033h, 075h, 0FEh, 0FFh
loc_1_8B93:
        popad
        ret
loc_1_8B95:
        db  0CDh, 020h, 02Fh, 080h, 001h, 000h
Crash_Cur_VM:
        db  08Bh, 015h, 00Ch, 0F7h, 000h, 000h
        and    byte ptr [edx + 2], 0feh
loc_1_8BA5:
        db  08Bh, 01Dh, 010h, 0F7h, 000h, 000h
        cmp    edx, ebx
        je     SHORT loc_1_8C14
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        mov    al, byte ptr [edi - 33h]
        test   al, al
        je     SHORT loc_1_8BD8
        cmp    al, byte ptr [edi - 32h]
        ja     SHORT loc_1_8BD8
        jmp    NEAR PTR sub_1_25DB
sub_1_8BC6:
        db  08Bh, 01Dh, 010h, 0F7h, 000h, 000h
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        db  08Bh, 015h, 00Ch, 0F7h, 000h, 000h
loc_1_8BD8:
        db  0C7h, 005h, 0F8h, 0E3h, 000h, 000h, 000h, 000h, 000h, 000h
        call   sub_1_79AD
        mov    byte ptr [edi - 33h], 0
        or     byte ptr [edi], 10h
        or     byte ptr [edx], 10h
        mov    eax, 400000h
        xor    ecx, ecx
        db  0BEh, 01Dh, 08Ch, 000h, 000h
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        mov    ebx, edx
        db  0CDh, 020h, 02Bh, 000h, 001h, 000h
        pushad
        xor    esi, esi
        db  0CDh, 020h, 0BEh, 000h, 001h, 000h
loc_1_8C14:
        pushad
        xor    esi, esi
        db  0CDh, 020h, 0BEh, 000h, 001h, 000h
loc_1_8C1D:
        mov    ecx, 1
        test   byte ptr [edx + 2], 1
        je     SHORT loc_1_8C2D
        mov    ecx, 20h
loc_1_8C2D:
        db  0E9h, 094h, 074h, 0FEh, 0FFh
Close_VM:
        bts    dword ptr [ebx], 10h
        jb     SHORT loc_1_8C75
        pushad
        mov    edi, eax
        mov    eax, 1000h
        mov    ecx, 13h
        db  0BEh, 07Bh, 08Ch, 000h, 000h
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        test   byte ptr [esp + 18h], 1
        jne    SHORT loc_1_8C73
        db  03Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        jne    SHORT loc_1_8C73
        db  0CDh, 020h, 007h, 000h, 001h, 000h
        db  0CDh, 020h, 083h, 000h, 001h, 000h
loc_1_8C6B:
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        jmp    SHORT loc_1_8C6B
loc_1_8C73:
        popad
        ret
loc_1_8C75:
        db  0CDh, 020h, 02Eh, 000h, 001h, 000h
loc_1_8C7B:
        xor    edx, edx
        mov    fs, edx
        mov    gs, edx
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        je     SHORT loc_1_8CBA
        db  0CDh, 020h, 002h, 000h, 001h, 000h
        jne    SHORT loc_1_8CCB
        test   byte ptr [ebp + 2dh], 2
        je     SHORT loc_1_8CA0
        db  083h, 03Dh, 0D4h, 0F6h, 000h, 000h, 000h
        je     SHORT loc_1_8CA5
loc_1_8CA0:
        mov    edx, 1
loc_1_8CA5:
        push   edx
        mov    eax, 19h
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        pop    ecx
        jecxz  loc_1_8CC0
        db  0CDh, 020h, 02Fh, 080h, 001h, 000h
loc_1_8CBA:
        db  0CDh, 020h, 003h, 080h, 009h, 000h
loc_1_8CC0:
        db  08Bh, 015h, 00Ch, 0F7h, 000h, 000h
        jmp    NEAR PTR loc_1_8BA5
loc_1_8CCB:
        db  0CDh, 020h, 02Eh, 080h, 001h, 000h
loc_1_8CD1:
        push   ebx
        push   esi
        push   edi
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        db  0C7h, 005h, 0F8h, 0E3h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 2
        call   sub_1_79AF
        mov    byte ptr [edi - 33h], 0
        mov    byte ptr [edi - 32h], 0
        mov    ebx, dword ptr [edi + 10h]
        or     byte ptr [ebx + 2dh], 2
        db  0CDh, 020h, 08Ah, 000h, 001h, 000h
        cli
loc_1_8D04:
        mov    esi, dword ptr [edi + 48h]
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     SHORT loc_1_8D1D
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [esi + 4]
        mov    edx, eax
        xchg   dword ptr [ecx], edx
        xchg   dword ptr [eax - 4], edx
        mov    dword ptr [esi], edx
        jmp    SHORT loc_1_8D04
loc_1_8D1D:
        sti
        mov    dword ptr [edi + 24h], 0
        mov    byte ptr [edi - 34h], 0
        and    dword ptr [edi], 0ffdfefffh
        push   edi
        call   sub_1_80F5
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [edi + 4ch], eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ecx
        mov    esp, eax
        db  068h, 000h, 003h, 000h, 000h
        push   ecx
        ret
Get_Next_VM_Handle:
        mov    ebx, dword ptr [ebx + 2ch]
        test   ebx, ebx
        je     SHORT loc_1_8D50
        ret
loc_1_8D50:
        db  08Bh, 01Dh, 014h, 0F7h, 000h, 000h
        ret
        db  090h
loc_1_8D58:
        cmp    dword ptr [edi + 40h], 0
        je     SHORT loc_1_8D62
        mov    edi, dword ptr [edi + 40h]
        ret
loc_1_8D62:
        mov    edi, dword ptr [edi + 14h]
        mov    edi, dword ptr [edi + 2ch]
        test   edi, edi
        je     SHORT loc_1_8D70
        mov    edi, dword ptr [edi - 1ch]
        ret
loc_1_8D70:
        db  08Bh, 03Dh, 014h, 0F7h, 000h, 000h
        mov    edi, dword ptr [edi - 1ch]
        ret
Call_When_Task_Switched:
        pushad
        push   esi
        db  08Bh, 035h, 01Ch, 0F7h, 000h, 000h
loc_1_8D82:
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        pop    dword ptr [eax]
        db  0CDh, 020h, 09Eh, 000h, 001h, 000h
        popad
        ret
loc_1_8D92:
        pushad
        push   esi
        db  08Bh, 035h, 020h, 0F7h, 000h, 000h
        jmp    SHORT loc_1_8D82
loc_1_8D9C:
        pushad
        db  08Bh, 03Dh, 01Ch, 0F7h, 000h, 000h
        call   sub_1_871A
        popad
        ret
loc_1_8DAA:
        pushad
        db  08Bh, 03Dh, 020h, 0F7h, 000h, 000h
        call   sub_1_871A
        popad
        ret
sub_1_8DB8:
        cli
        db  0C6h, 005h, 0BCh, 0F6h, 000h, 000h, 001h
        push   5
        push   edx
        call   sub_1_8F05
        db  0C6h, 005h, 0BCh, 0F6h, 000h, 000h, 000h
        or     dword ptr [edx], 10h
        sti
        ret
sub_1_8DD4:
        xor    eax, eax
        db  039h, 01Dh, 044h, 0F7h, 000h, 000h
        je     SHORT loc_1_8E07
        db  039h, 01Dh, 010h, 0F7h, 000h, 000h
        je     SHORT loc_1_8E07
        mov    ecx, dword ptr [ebx]
        test   ecx, 4
        jne    SHORT loc_1_8E06
        test   ecx, 200h
        jne    SHORT loc_1_8E07
        db  08Bh, 015h, 044h, 0F7h, 000h, 000h
        test   ecx, 2
        jne    SHORT loc_1_8E07
loc_1_8E06:
        inc    eax
loc_1_8E07:
        or     eax, eax
        ret
sub_1_8E0A:
        push   edi
        bts    dword ptr [ebx], 11h
        jb     SHORT loc_1_8E2D
        mov    edi, dword ptr [ebx - 20h]
        cmp    byte ptr [edi - 33h], 0
        je     SHORT loc_1_8E27
        or     byte ptr [edi], 20h
        mov    eax, 10h
        call   sub_1_8388
loc_1_8E27:
        db  0CDh, 020h, 02Bh, 000h, 001h, 000h
loc_1_8E2D:
        pop    edi
        ret
sub_1_8E2F:
        push   edi
        btr    dword ptr [ebx], 11h
        jae    SHORT loc_1_8E56
        mov    edi, dword ptr [ebx - 20h]
        btr    dword ptr [edi], 5
        jae    SHORT loc_1_8E49
        mov    eax, 0fffffff0h
        call   sub_1_8388
loc_1_8E49:
        db  0CDh, 020h, 02Ch, 000h, 001h, 000h
        jae    SHORT loc_1_8E56
        or     byte ptr [ebx + 2], 2
        stc
loc_1_8E56:
        pop    edi
        ret
sub_1_8E58:
        push   ebp
        mov    ebp, esp
        add    esp, -4
        push   edi
        push   ebx
        push   esi
        db  08Bh, 01Dh, 010h, 0F7h, 000h, 000h
        db  08Bh, 03Dh, 044h, 0F7h, 000h, 000h
        mov    dword ptr [ebp - 4], edi
        cmp    ebx, edi
        je     SHORT loc_1_8E9C
        mov    esi, 1
        xor    edx, edx
        mov    eax, 0fh
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        db  00Fh, 0B7h, 00Dh, 010h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
loc_1_8E9C:
        db  08Bh, 01Dh, 048h, 0F7h, 000h, 000h
        mov    eax, 80001030h
        xor    edi, edi
        xor    esi, esi
        db  0CDh, 020h, 004h, 000h, 017h, 000h
        pop    esi
        pop    ebx
        pop    edi
        leave
        ret
sub_1_8EB6:
        push   edi
        push   ebx
loc_1_8EB8:
        db  08Bh, 01Dh, 014h, 0F7h, 000h, 000h
        mov    edi, ebx
loc_1_8EC0:
        call   sub_1_8DD4
        je     SHORT loc_1_8ECE
        call   sub_1_8E0A
        jmp    SHORT loc_1_8EDC
loc_1_8ECE:
        call   sub_1_8E2F
        jae    SHORT loc_1_8EDC
        call   sub_1_8E58
        jmp    SHORT loc_1_8EB8
loc_1_8EDC:
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, edi
        jne    SHORT loc_1_8EC0
        pop    ebx
        pop    edi
        ret
        db  018h, 08Fh, 000h, 000h, 04Eh, 08Fh, 000h, 000h, 0A8h, 08Fh, 000h, 000h, 068h, 08Fh, 000h, 000h
        db  076h, 08Fh, 000h, 000h, 076h, 08Fh, 000h, 000h, 06Fh, 08Fh, 000h, 000h
sub_1_8F05:
        push   ebp
        mov    ebp, esp
        push   edi
        push   ecx
        push   edx
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        db  0FFh, 024h, 085h, 0E9h, 08Eh, 000h, 000h
loc_1_8F18:
        mov    dword ptr [edi - 4], edi
        mov    eax, 8
        db  0CDh, 020h, 067h, 001h, 001h, 000h
        db  0A1h, 050h, 0F7h, 000h, 000h
        mov    dword ptr [edi + 78h], eax
        push   ebx
        mov    ebx, dword ptr [edi + 14h]
        db  0CDh, 020h, 02Ch, 000h, 001h, 000h
        call   sub_1_8DD4
        je     SHORT loc_1_8F46
        or     byte ptr [ebx + 2], 2
        pop    ebx
        jmp    SHORT loc_1_8FA8
loc_1_8F46:
        call   sub_1_8E2F
        pop    ebx
        jmp    SHORT loc_1_8FA8
loc_1_8F4E:
        mov    eax, 8
        db  0CDh, 020h, 067h, 001h, 001h, 000h
        db  0A1h, 050h, 0F7h, 000h, 000h
        mov    dword ptr [edi + 78h], eax
        call   sub_1_84FC
        jmp    SHORT loc_1_8FA8
loc_1_8F68:
        call   sub_1_905F
        jmp    SHORT loc_1_8FA8
loc_1_8F6F:
        call   sub_1_8EB6
        jmp    SHORT loc_1_8FA8
loc_1_8F76:
        db  0CDh, 020h, 06Ch, 001h, 001h, 000h
        db  0FFh, 005h, 078h, 0F7h, 000h, 000h
loc_1_8F82:
        mov    ecx, dword ptr [edi - 14h]
        jecxz  loc_1_8F91
        mov    eax, ecx
        db  0CDh, 020h, 065h, 001h, 001h, 000h
        jmp    SHORT loc_1_8F82
loc_1_8F91:
        mov    ecx, dword ptr [edi - 18h]
        jecxz  loc_1_8FA0
        mov    eax, ecx
        db  0CDh, 020h, 065h, 001h, 001h, 000h
        jmp    SHORT loc_1_8F91
loc_1_8FA0:
        db  0FFh, 00Dh, 078h, 0F7h, 000h, 000h
        jmp    SHORT loc_1_8FA8
loc_1_8FA8:
        or     eax, 0ffffffffh
        pop    edx
        pop    ecx
        pop    edi
        leave
        ret    8
sub_1_8FB2:
        push   ebx
        xor    eax, eax
        mov    ecx, dword ptr [edi - 18h]
        jecxz  loc_1_8FCC
loc_1_8FBA:
        mov    ebx, dword ptr [ecx]
        sub    eax, ebx
        cdq
        not    edx
        and    eax, edx
        add    eax, ebx
        mov    ecx, dword ptr [ecx - 4]
        or     ecx, ecx
        jne    SHORT loc_1_8FBA
loc_1_8FCC:
        mov    dword ptr [edi - 1ch], eax
        pop    ebx
        ret
sub_1_8FD1:
        mov    eax, dword ptr [edi - 10h]
        mov    ecx, dword ptr [edi - 8]
        or     eax, eax
        jns    SHORT loc_1_8FE4
        sub    eax, ecx
        cdq
        and    eax, edx
        add    eax, ecx
        jmp    SHORT loc_1_8FED
loc_1_8FE4:
        sub    eax, ecx
        cdq
        not    edx
        and    eax, edx
        add    eax, ecx
loc_1_8FED:
        add    eax, dword ptr [edi - 24h]
        jns    SHORT loc_1_8FF4
        xor    eax, eax
loc_1_8FF4:
        sub    eax, 1fh
        cdq
        and    eax, edx
        add    eax, 1fh
        mov    ecx, dword ptr [edi - 1ch]
        sub    eax, ecx
        cdq
        not    edx
        and    eax, edx
        add    eax, ecx
        mov    ecx, eax
        sub    eax, dword ptr [edi - 20h]
        je     SHORT loc_1_904A
        add    dword ptr [edi - 20h], eax
        call   sub_1_8388
        mov    eax, dword ptr [edi - 20h]
        mov    edx, dword ptr [edi - 14h]
loc_1_901E:
        or     edx, edx
        je     SHORT loc_1_9039
        db  0FFh, 005h, 078h, 0F7h, 000h, 000h
        db  0CDh, 020h, 064h, 001h, 001h, 000h
        db  0FFh, 00Dh, 078h, 0F7h, 000h, 000h
        mov    edx, dword ptr [edx + 0ch]
        jmp    SHORT loc_1_901E
loc_1_9039:
        db  083h, 03Dh, 078h, 0F7h, 000h, 000h, 000h
        jne    SHORT loc_1_904A
        pushfd
        cli
        call   sub_1_8ACC
        popfd
loc_1_904A:
        mov    eax, ecx
        ret
sub_1_904D:
        db  0FFh, 005h, 078h, 0F7h, 000h, 000h
        call   sub_1_8FD1
        db  0FFh, 00Dh, 078h, 0F7h, 000h, 000h
        ret
sub_1_905F:
        mov    eax, dword ptr [edi - 10h]
        mov    dword ptr [edi - 0ch], eax
sub_1_9065:
        push   esi
        mov    ecx, dword ptr [edi - 10h]
        jecxz  loc_1_9098
        mov    eax, dword ptr [edi - 0ch]
        or     ecx, ecx
        js     SHORT loc_1_909A
        sub    eax, ecx
        cdq
        and    eax, edx
        add    eax, ecx
loc_1_9079:
        neg    eax
        add    ecx, eax
        mov    dword ptr [edi - 10h], ecx
        or     ecx, ecx
        jne    SHORT loc_1_9087
        mov    dword ptr [edi - 0ch], ecx
loc_1_9087:
        db  0FFh, 005h, 078h, 0F7h, 000h, 000h
        call   sub_1_8FD1
        db  0FFh, 00Dh, 078h, 0F7h, 000h, 000h
loc_1_9098:
        pop    esi
        ret
loc_1_909A:
        sub    eax, ecx
        cdq
        not    edx
        and    eax, edx
        add    eax, ecx
        jmp    SHORT loc_1_9079
        db  08Bh, 0C1h, 0EBh, 0D0h
sub_1_90A9:
        push   esi
        xor    esi, esi
        db  087h, 035h, 070h, 0F7h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_90BC
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
loc_1_90BC:
        pop    esi
        ret
sub_1_90BE:
        push   esi
        xor    esi, esi
        db  087h, 035h, 070h, 0F7h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_90D1
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
loc_1_90D1:
        db  0BEh, 0A1h, 091h, 000h, 000h
        mov    eax, dword ptr [edi + 78h]
        sub    eax, dword ptr [edi + 74h]
        jbe    SHORT loc_1_90EC
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 070h, 0F7h, 000h, 000h
loc_1_90EA:
        pop    esi
        ret
loc_1_90EC:
        or     dword ptr [edi], 1000000h
        jmp    SHORT loc_1_90EA
sub_1_90F4:
        push   esi
        mov    dword ptr [edi - 50h], eax
        mov    esi, eax
        cmp    dword ptr [edi - 10h], 0
        jge    SHORT loc_1_9105
        call   sub_1_9065
loc_1_9105:
        mov    eax, esi
        mov    edx, esi
        mov    esi, edi
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        btr    dword ptr [edi], 18h
        jb     SHORT loc_1_9156
        sub    eax, dword ptr [edi - 50h]
        mov    dword ptr [edi - 50h], edx
        test   byte ptr [edi + 3], 18h
        je     SHORT loc_1_9179
        mov    dword ptr [edi - 54h], edx
        add    eax, dword ptr [edi + 74h]
        cmp    eax, dword ptr [edi + 78h]
        jae    SHORT loc_1_9156
        mov    dword ptr [edi + 74h], eax
loc_1_9131:
        cmp    dword ptr [edi - 8], 0
        jne    SHORT loc_1_9170
        db  0A1h, 074h, 0F7h, 000h, 000h
        db  08Bh, 00Dh, 074h, 0F7h, 000h, 000h
        db  0FFh, 005h, 078h, 0F7h, 000h, 000h
        db  0CDh, 020h, 062h, 001h, 001h, 000h
        db  0FFh, 00Dh, 078h, 0F7h, 000h, 000h
        jmp    SHORT loc_1_9170
loc_1_9156:
        mov    dword ptr [edi + 74h], 0
        db  0A1h, 050h, 0F7h, 000h, 000h
        mov    dword ptr [edi + 78h], eax
        cmp    dword ptr [edi - 10h], 0
        jl     SHORT loc_1_9170
        call   sub_1_9065
loc_1_9170:
        mov    edi, esi
        call   sub_1_90BE
        pop    esi
        ret
loc_1_9179:
        mov    eax, dword ptr [edi + 14h]
        db  03Bh, 005h, 010h, 0F7h, 000h, 000h
        je     SHORT loc_1_9170
        mov    al, byte ptr [edi - 31h]
        inc    al
        cmp    al, 40h
        jb     SHORT loc_1_918F
        xor    al, al
loc_1_918F:
        mov    byte ptr [edi - 31h], al
        je     SHORT loc_1_9131
        jmp    SHORT loc_1_9170
        db  08Bh, 0C0h
sub_1_9198:
        push   edi
        mov    edi, dword ptr [esp + 8]
        pop    edi
        ret    8
loc_1_91A1:
        push   edi
        db  0C7h, 005h, 070h, 0F7h, 000h, 000h, 000h, 000h, 000h, 000h
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        or     byte ptr [edi + 3], 1
        pushfd
        cli
        call   sub_1_8ACC
        popfd
        db  083h, 03Dh, 070h, 0F7h, 000h, 000h, 000h
        jne    SHORT loc_1_91E2
        call   sub_1_9065
        and    byte ptr [edi + 3], 0feh
        db  0A1h, 050h, 0F7h, 000h, 000h
        mov    dword ptr [edi + 78h], eax
        xor    eax, eax
        mov    dword ptr [edi + 74h], eax
        call   sub_1_90BE
loc_1_91E2:
        pop    edi
        ret
loc_1_91E4:
        db  0FFh, 005h, 078h, 0F7h, 000h, 000h
        db  0C7h, 005h, 07Ch, 0F7h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        mov    ecx, eax
        db  08Bh, 015h, 080h, 0F7h, 000h, 000h
        mov    ebx, edx
        shr    ebx, 1
        add    edx, ebx
        xor    ebp, ebp
        db  0BEh, 088h, 0FCh, 000h, 000h
        mov    edi, dword ptr [esi + 4]
loc_1_9212:
        cmp    edi, esi
        jne    SHORT loc_1_921C
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
loc_1_921C:
        inc    ebp
        sub    eax, dword ptr [edi - 54h]
        cmp    eax, edx
        jb     SHORT loc_1_9269
        cmp    dword ptr [edi - 20h], 0
        je     SHORT loc_1_9269
        db  0A1h, 084h, 0F7h, 000h, 000h
        mov    ebx, dword ptr [edi - 20h]
        push   ecx
        db  08Bh, 00Dh, 010h, 0F7h, 000h, 000h
        cmp    dword ptr [ecx - 64h], 10h
        jne    SHORT loc_1_9249
        cmp    dword ptr [edi + 14h], ecx
        mov    ecx, 1fh
        jne    SHORT loc_1_924E
loc_1_9249:
        mov    ecx, 0fh
loc_1_924E:
        add    eax, ebx
        sub    eax, ecx
        cdq
        and    eax, edx
        add    eax, ecx
        pop    ecx
        sub    eax, ebx
        jbe    SHORT loc_1_9269
        push   ecx
        add    eax, dword ptr [edi - 10h]
        mov    ecx, eax
        db  0CDh, 020h, 062h, 001h, 001h, 000h
        pop    ecx
loc_1_9269:
        mov    eax, ecx
        db  03Bh, 03Dh, 098h, 0F6h, 000h, 000h
        je     SHORT loc_1_9278
        mov    edi, dword ptr [edi + 4]
        jmp    SHORT loc_1_9212
loc_1_9278:
        db  0A1h, 050h, 0F7h, 000h, 000h
        mul    ebp
        sub    eax, 30h
        cdq
        not    edx
        and    eax, edx
        add    eax, 30h
        db  0A3h, 080h, 0F7h, 000h, 000h
        db  0A1h, 080h, 0F7h, 000h, 000h
        db  0BEh, 0E4h, 091h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 07Ch, 0F7h, 000h, 000h
        db  0FFh, 00Dh, 078h, 0F7h, 000h, 000h
        ret
loc_1_92AC:
        xor    eax, eax
        db  0A3h, 068h, 0F7h, 000h, 000h
        db  08Bh, 03Dh, 09Ch, 0F6h, 000h, 000h
        call   sub_1_905F
        ret
loc_1_92BF:
        mov    eax, dword ptr [edi - 24h]
        mov    ecx, dword ptr [edi - 20h]
        mov    edx, dword ptr [edi - 10h]
        mov    esi, dword ptr [edi - 1ch]
        ret
loc_1_92CC:
        push   ecx
        push   edx
        cmp    eax, 1fh
        ja     SHORT loc_1_92EC
        mov    dword ptr [edi - 24h], eax
        xor    ecx, ecx
        mov    dword ptr [edi - 10h], ecx
        mov    dword ptr [edi - 0ch], ecx
        call   sub_1_8FD1
        mov    eax, 0ffffffe0h
        clc
loc_1_92E9:
        pop    edx
        pop    ecx
        ret
loc_1_92EC:
        stc
        jmp    SHORT loc_1_92E9
loc_1_92EF:
        push   esi
        push   edx
        push   ecx
        mov    ecx, eax
        or     ebx, ebx
        je     SHORT loc_1_92F8
loc_1_92F8:
        db  08Bh, 035h, 058h, 0F7h, 000h, 000h
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     SHORT loc_1_933A
        mov    esi, eax
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [eax + 8], ebx
        mov    dword ptr [eax + 10h], edx
        mov    edx, dword ptr [edi - 18h]
        mov    dword ptr [eax - 4], edx
        mov    dword ptr [edi - 18h], eax
        or     ebx, ebx
        je     SHORT loc_1_9329
        mov    edx, dword ptr [ebx - 14h]
        mov    dword ptr [eax + 0ch], edx
        mov    dword ptr [ebx - 14h], eax
loc_1_9329:
        mov    eax, ecx
        sub    ecx, dword ptr [edi - 1ch]
        jbe    SHORT loc_1_9338
        mov    dword ptr [edi - 1ch], eax
        call   sub_1_904D
loc_1_9338:
        mov    eax, esi
loc_1_933A:
        pop    ecx
        pop    edx
        pop    esi
        ret
loc_1_933E:
        push   edi
        push   edx
        push   ecx
        push   eax
        mov    edi, dword ptr [edx + 4]
        mov    ecx, eax
        xchg   dword ptr [edx], ecx
        cmp    eax, ecx
        je     SHORT loc_1_935C
        jb     SHORT loc_1_9361
        cmp    eax, dword ptr [edi - 1ch]
        jbe    SHORT loc_1_935C
        mov    dword ptr [edi - 1ch], eax
        call   sub_1_904D
loc_1_935C:
        pop    eax
        pop    ecx
        pop    edx
        pop    edi
        ret
loc_1_9361:
        cmp    ecx, dword ptr [edi - 1ch]
        jb     SHORT loc_1_935C
        call   sub_1_8FB2
        call   sub_1_904D
loc_1_9370:
        jmp    SHORT loc_1_935C
loc_1_9372:
        push   edi
        push   esi
        push   edx
        push   ecx
        push   eax
        mov    edi, dword ptr [eax + 8]
        or     edi, edi
        je     SHORT loc_1_9393
        lea    edx, [edi - 14h]
loc_1_9381:
        mov    ecx, dword ptr [edx]
        jecxz  loc_1_9393
        cmp    ecx, eax
        je     SHORT loc_1_938E
        lea    edx, [ecx + 0ch]
        jmp    SHORT loc_1_9381
loc_1_938E:
        mov    ecx, dword ptr [eax + 0ch]
        mov    dword ptr [edx], ecx
loc_1_9393:
        mov    edi, dword ptr [eax + 4]
        lea    edx, [edi - 18h]
loc_1_9399:
        mov    ecx, dword ptr [edx]
        jecxz  loc_1_93A6
        cmp    ecx, eax
        je     SHORT loc_1_93A9
        lea    edx, [ecx - 4]
        jmp    SHORT loc_1_9399
loc_1_93A6:
        stc
        jmp    SHORT loc_1_93D1
loc_1_93A9:
        mov    ecx, dword ptr [eax - 4]
        mov    dword ptr [edx], ecx
        mov    ecx, dword ptr [eax]
        mov    edx, dword ptr [eax + 4]
        db  08Bh, 035h, 058h, 0F7h, 000h, 000h
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        cmp    dword ptr [edx - 1ch], ecx
        jne    SHORT loc_1_93D0
        push   eax
        call   sub_1_8FB2
        call   sub_1_904D
        pop    eax
loc_1_93D0:
        clc
loc_1_93D1:
        pop    eax
        pop    ecx
        pop    edx
        pop    esi
        pop    edi
        ret
loc_1_93D7:
        push   edx
        push   ecx
        push   ebx
        or     eax, eax
        je     SHORT loc_1_93FE
        mov    ebx, eax
        xor    eax, eax
        mov    ecx, dword ptr [edi - 14h]
loc_1_93E5:
        jecxz  loc_1_93FE
        cmp    dword ptr [ecx + 10h], ebx
        mov    edx, dword ptr [ecx + 0ch]
        jne    SHORT loc_1_93FA
        push   eax
        mov    eax, ecx
        db  0CDh, 020h, 065h, 001h, 001h, 000h
        pop    eax
        inc    eax
loc_1_93FA:
        mov    ecx, edx
        jmp    SHORT loc_1_93E5
loc_1_93FE:
        pop    ebx
        pop    ecx
        pop    edx
        ret
loc_1_9402:
        push   eax
        push   ebx
        push   ecx
        push   edx
        mov    ebx, dword ptr [edi - 10h]
        or     eax, eax
        js     SHORT loc_1_945A
        cmp    eax, ebx
        jle    SHORT loc_1_9455
        mov    ebx, dword ptr [edi - 24h]
        add    eax, ebx
        push   ecx
        db  08Bh, 00Dh, 010h, 0F7h, 000h, 000h
        cmp    dword ptr [ecx - 64h], 10h
        jne    SHORT loc_1_942D
        cmp    dword ptr [edi + 14h], ecx
        mov    ecx, 1fh
        jne    SHORT loc_1_9432
loc_1_942D:
        mov    ecx, 0fh
loc_1_9432:
        sub    eax, ecx
        cdq
        and    eax, edx
        add    eax, ecx
        pop    ecx
        sub    eax, ebx
        jbe    SHORT loc_1_9455
        mov    dword ptr [edi - 10h], eax
        mov    eax, dword ptr [edi - 0ch]
        sub    eax, ecx
        cdq
        not    edx
        and    eax, edx
        add    eax, ecx
loc_1_944D:
        mov    dword ptr [edi - 0ch], eax
        call   sub_1_8FD1
loc_1_9455:
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        ret
loc_1_945A:
        cmp    eax, ebx
        jge    SHORT loc_1_9455
        mov    ebx, dword ptr [edi - 24h]
        neg    eax
        sub    eax, ebx
        cdq
        and    eax, edx
        add    eax, ebx
        neg    eax
        mov    dword ptr [edi - 10h], eax
        mov    eax, dword ptr [edi - 0ch]
        sub    eax, ecx
        cdq
        and    eax, edx
        add    eax, ecx
        jmp    SHORT loc_1_944D
_LTEXT ends
        end
