; sbawe_lcode_01_02.asm -- SBAWE object 1 segment _LTEXT  (part 2/2)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_49F5:
        push   ebp
        mov    ecx, dword ptr [esp + 0ch]
        shl    ecx, 5
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   ebx
        lea    edx, [ecx + eax + 442h]
        push   esi
        xor    ecx, ecx
        push   edi
        movzx  esi, byte ptr [edx + 16h]
        test   esi, esi
        jne    SHORT loc_1_4A20
loc_1_4A19:
        mov    ecx, 80h
        mov    esi, ecx
loc_1_4A20:
        cmp    ecx, 120h
        jae    SHORT loc_1_4A66
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + ecx*4 + 662h]
        test   eax, eax
        je     SHORT loc_1_4A63
        mov    edi, dword ptr [eax]
        xor    eax, eax
        cmp    word ptr [edi], 0ffffh
        je     SHORT loc_1_4A63
loc_1_4A41:
        cmp    word ptr [edi], 0
        je     SHORT loc_1_4A58
        movzx  ebx, byte ptr [edi + 3]
        cmp    ebx, esi
        jne    SHORT loc_1_4A58
        movzx  ebx, byte ptr [edi + 2]
        cmp    ebx, dword ptr [ebp + 10h]
        je     SHORT loc_1_4A78
loc_1_4A58:
        add    edi, 1ah
        inc    eax
        cmp    word ptr [edi], 0ffffh
        jne    SHORT loc_1_4A41
loc_1_4A63:
        inc    ecx
        jmp    SHORT loc_1_4A20
loc_1_4A66:
        test   esi, esi
        je     SHORT loc_1_4A8B
        cmp    esi, 80h
        jne    SHORT loc_1_4A19
        xor    ecx, ecx
        xor    esi, esi
        jmp    SHORT loc_1_4A20
loc_1_4A78:
        mov    word ptr [edx + 14h], cx
        mov    word ptr [edx], ax
        mov    ax, word ptr [ebp + 10h]
        mov    word ptr [edx + 2], ax
        xor    eax, eax
        jmp    SHORT loc_1_4A90
loc_1_4A8B:
        mov    eax, 1
loc_1_4A90:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
sub_1_4A99:
        xor    eax, eax
        ret    10h
sub_1_4A9E:
        push   ebp
        mov    ecx, 1eh
        mov    ebp, esp
        sub    edx, edx
        mov    eax, dword ptr [ebp + 10h]
        div    ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 8]
        shl    ecx, 5
        add    ecx, edx
        mov    byte ptr [ecx + 44dh], al
        movzx  eax, byte ptr [ecx + 44ch]
        imul   eax, eax, 1eh
        push   eax
        push   dword ptr [ebp + 0ch]
        push   edx
        call   sub_1_41FA
        pop    ebp
        ret    0ch
sub_1_4AD6:
        mov    ecx, dword ptr [esp + 10h]
        sub    esp, 4
        sub    ecx, 40h
        push   ebx
        shl    ecx, 7
        push   esi
        add    ecx, dword ptr [esp + 18h]
        push   edi
        mov    eax, dword ptr [esp + 18h]
        push   ebp
        shl    eax, 5
        mov    edi, dword ptr [esp + 18h]
        lea    ebx, [eax + edi]
        movzx  eax, byte ptr [ebx + 44fh]
        test   eax, eax
        jg     SHORT loc_1_4B09
        mov    eax, 2
loc_1_4B09:
        movsx  eax, ax
        imul   eax, ecx
        mov    ecx, 18h
        xor    ebp, ebp
        cdq
        idiv   ecx
        mov    dword ptr [esp + 10h], eax
        mov    dword ptr [ebx + 452h], eax
        cmp    word ptr [edi + 6], bp
        jbe    SHORT loc_1_4B7F
        lea    esi, [edi + 42h]
loc_1_4B2C:
        mov    ax, word ptr [esi]
        shr    ax, 8
        movzx  ecx, ax
        cmp    ecx, dword ptr [esp + 1ch]
        jne    SHORT loc_1_4B73
        movsx  ecx, word ptr [ebx + 450h]
        movzx  eax, word ptr [esi + 0eh]
        add    ecx, eax
        mov    eax, dword ptr [esp + 10h]
        add    eax, ecx
        cmp    eax, 0ffffh
        jbe    SHORT loc_1_4B5D
        mov    eax, 0ffffh
        jmp    SHORT loc_1_4B63
loc_1_4B5D:
        test   eax, eax
        jge    SHORT loc_1_4B63
        xor    eax, eax
loc_1_4B63:
        movzx  eax, ax
        push   eax
        mov    eax, ebp
        or     ah, 4
        push   eax
        push   edi
        call   sub_1_19AA
loc_1_4B73:
        add    esi, 20h
        inc    ebp
        movzx  eax, word ptr [edi + 6]
        cmp    eax, ebp
        ja     SHORT loc_1_4B2C
loc_1_4B7F:
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    10h
sub_1_4B8B:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0BFh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [ebp + 0ch]
loc_1_4B9B:
        mov    eax, edi
        xor    ecx, ecx
        cmp    dword ptr [ebp + 10h], ecx
        je     SHORT loc_1_4BBC
loc_1_4BA4:
        mov    bl, byte ptr [eax]
        cmp    bl, 0feh
        je     SHORT loc_1_4BB5
        cmp    bl, 0ffh
        je     SHORT loc_1_4BBC
        cmp    byte ptr [esi + ecx], bl
        jne    SHORT loc_1_4BBC
loc_1_4BB5:
        inc    ecx
        inc    eax
        cmp    ecx, dword ptr [ebp + 10h]
        jb     SHORT loc_1_4BA4
loc_1_4BBC:
        cmp    byte ptr [eax], 0ffh
        je     SHORT loc_1_4BD9
        add    edi, 16h
        inc    edx
        db  081h, 0FFh, 02Ch, 000h, 000h, 000h
        jb     SHORT loc_1_4B9B
        mov    eax, 1
loc_1_4BD2:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_1_4BD9:
        imul   edx, edx, 16h
        db  00Fh, 0B7h, 08Ah, 010h, 000h, 000h, 000h
        movzx  eax, byte ptr [ecx + esi]
        push   eax
        push   dword ptr [ebp + 8]
        db  0FFh, 092h, 012h, 000h, 000h, 000h
        add    esp, 8
        jmp    SHORT loc_1_4BD2
sub_1_4BF6:
        push   ebx
        xor    edx, edx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        db  0C7h, 086h, 0E2h, 00Ah, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 086h, 0E6h, 00Ah, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 086h, 0EAh, 00Ah, 000h, 000h, 000h, 000h, 000h, 000h
        lea    ecx, [esi + 0eeeh]
loc_1_4C22:
        mov    word ptr [ecx], 0
        xor    eax, eax
loc_1_4C29:
        mov    byte ptr [eax + ecx + 3eh], 0
        lea    ebx, [eax + edx]
        inc    eax
        mov    word ptr [esi + ebx*2 + 0ef0h], 2000h
        cmp    eax, 1eh
        jl     SHORT loc_1_4C29
        add    edx, 2eh
        add    ecx, 5ch
        cmp    edx, 2e0h
        jl     SHORT loc_1_4C22
        xor    eax, eax
        pop    esi
        pop    ebx
        ret    4
loc_1_4C56:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        test   byte ptr [ebp + 0dh], 80h
        push   ebx
        push   esi
        push   edi
        je     SHORT loc_1_4CD2
        and    dword ptr [ebp + 0ch], 1fh
        mov    eax, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        imul   eax, eax, 5ch
        db  0C7h, 045h, 0FCh, 000h, 000h, 000h, 000h
        lea    edi, [eax + esi + 0ef0h]
        lea    ebx, [eax + esi + 0eeeh]
        xor    esi, esi
loc_1_4C89:
        mov    al, byte ptr [ebx + esi + 3eh]
        test   al, al
        je     SHORT loc_1_4CBC
        cmp    al, 1
        jne    SHORT loc_1_4CBC
        push   4
        mov    edx, dword ptr [ebp - 4]
        movsx  eax, word ptr [edi]
        movzx  ecx, byte ptr [edx]
        sub    eax, 2000h
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4CAC:
        mov    ecx, dword ptr [ebp - 4]
        movzx  ecx, byte ptr [ecx]
        add    esp, 0ch
        mov    edx, dword ptr [ebp + 10h]
        mov    word ptr [edx + ecx*2], ax
loc_1_4CBC:
        add    edi, 2
        inc    esi
        add    dword ptr [ebp - 4], 6
        db  081h, 07Dh, 0FCh, 0B4h, 000h, 000h, 000h
        jb     SHORT loc_1_4C89
        jmp    NEAR PTR loc_1_4E73
loc_1_4CD2:
        mov    eax, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        shl    eax, 5
        xor    ebx, ebx
        lea    edi, [eax + esi + 442h]
        mov    eax, dword ptr [ebp + 0ch]
        imul   eax, eax, 5ch
        lea    edx, [eax + esi + 0eeeh]
        mov    ax, word ptr [edi + 1ch]
        mov    bx, ax
        and    ebx, 7fh
        cmp    ebx, 1eh
        jb     SHORT loc_1_4D0A
        mov    eax, 1
        jmp    NEAR PTR loc_1_4E75
loc_1_4D0A:
        mov    cx, word ptr [edi + 1eh]
        shr    ax, 8
        movzx  eax, ax
        cmp    eax, 7fh
        je     SHORT loc_1_4D24
        mov    eax, 1
        jmp    NEAR PTR loc_1_4E75
loc_1_4D24:
        mov    word ptr [edx], 1
        mov    byte ptr [ebx + edx + 3eh], 1
        mov    word ptr [edx + ebx*2 + 2], cx
        imul   ebx, ebx, 6
        db  080h, 0BBh, 001h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_4E75
        db  00Fh, 0B6h, 083h, 000h, 000h, 000h, 000h
        sub    cx, 2000h
        mov    dword ptr [ebp - 0ch], eax
        cmp    eax, 4
        jne    SHORT loc_1_4D6F
        movsx  eax, cx
        shl    eax, 0ch
        mov    ecx, 4b0h
        cdq
        idiv   ecx
        mov    word ptr [ebp - 2], ax
        mov    word ptr [edi + 0eh], ax
        jmp    SHORT loc_1_4DC4
loc_1_4D6F:
        cmp    dword ptr [ebp - 0ch], 5
        movsx  eax, cx
        push   4
        push   eax
        jne    SHORT loc_1_4DA7
        push   5
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4D82:
        movzx  cx, byte ptr [edi + 0ah]
        movzx  dx, byte ptr [edi + 0bh]
        add    cx, dx
        add    esp, 0ch
        add    ax, cx
        mov    word ptr [ebp - 2], ax
        cmp    ax, 7fh
        jle    SHORT loc_1_4DBF
        mov    word ptr [ebp - 2], 7fh
        jmp    SHORT loc_1_4DBF
loc_1_4DA7:
        push   dword ptr [ebp - 0ch]
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4DAF:
        mov    word ptr [ebp - 2], ax
        add    esp, 0ch
        db  080h, 0BBh, 004h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_4DC4
loc_1_4DBF:
        shl    word ptr [ebp - 2], 8
loc_1_4DC4:
        lea    eax, [esi + 42h]
        mov    dword ptr [ebp - 10h], eax
        xor    eax, eax
        cmp    word ptr [esi + 6], ax
        mov    dword ptr [ebp - 8], eax
        jbe    NEAR PTR loc_1_4E73
loc_1_4DD9:
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax]
        shr    ax, 8
        movzx  eax, ax
        cmp    eax, 0ffh
        je     SHORT loc_1_4E5F
        cmp    eax, dword ptr [ebp + 0ch]
        jne    SHORT loc_1_4E5F
        cmp    dword ptr [ebp - 0ch], 4
        jne    SHORT loc_1_4E1D
        mov    eax, dword ptr [ebp - 10h]
        movsx  ecx, word ptr [ebp - 2]
        movzx  eax, word ptr [eax + 0eh]
        add    eax, dword ptr [edi + 10h]
        add    eax, ecx
        cmp    eax, 0ffffh
        jle    SHORT loc_1_4E14
        mov    eax, 0ffffh
loc_1_4E14:
        push   eax
        mov    eax, dword ptr [ebp - 8]
        or     ah, 4
        jmp    SHORT loc_1_4E58
loc_1_4E1D:
        db  080h, 0BBh, 004h, 000h, 000h, 000h, 000h
        db  00Fh, 0B7h, 083h, 002h, 000h, 000h, 000h
        je     SHORT loc_1_4E3C
        or     eax, dword ptr [ebp - 8]
        push   eax
        push   esi
        call   sub_1_19DF
        mov    byte ptr [ebp - 2], al
        jmp    SHORT loc_1_4E49
loc_1_4E3C:
        or     eax, dword ptr [ebp - 8]
        push   eax
        push   esi
        call   sub_1_19DF
        mov    byte ptr [ebp - 1], ah
loc_1_4E49:
        movsx  eax, word ptr [ebp - 2]
        push   eax
        db  00Fh, 0B7h, 083h, 002h, 000h, 000h, 000h
        or     eax, dword ptr [ebp - 8]
loc_1_4E58:
        push   eax
        push   esi
        call   sub_1_19AA
loc_1_4E5F:
        add    dword ptr [ebp - 10h], 20h
        movzx  eax, word ptr [esi + 6]
        inc    dword ptr [ebp - 8]
        cmp    eax, dword ptr [ebp - 8]
        ja     NEAR PTR loc_1_4DD9
loc_1_4E73:
        xor    eax, eax
loc_1_4E75:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_4E7C:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        shl    eax, 5
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 8]
        lea    esi, [eax + edx]
        mov    word ptr [esi + 45ah], 200h
        add    esi, 45eh
        movzx  ax, byte ptr [ebp + 10h]
        mov    cx, word ptr [esi]
        and    cx, 7f00h
        or     ax, cx
        mov    word ptr [esi], cx
        movzx  ecx, ax
        mov    word ptr [esi], ax
        push   ecx
        push   dword ptr [ebp + 0ch]
        push   edx
        call   sub_1_4EC2
        pop    esi
        pop    ebp
        ret
sub_1_4EC2:
        push   ebp
        mov    ecx, dword ptr [esp + 10h]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    bl, ch
        test   bl, bl
        jne    SHORT loc_1_4F25
        mov    esi, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 0ch]
        imul   esi, esi, 5ch
        imul   edi, edi, 2eh
        mov    word ptr [edx + esi + 0eeeh], 0
        shl    eax, 5
        mov    dword ptr [ebp - 4], edi
        mov    word ptr [eax + edx + 450h], 0
        xor    eax, eax
loc_1_4F03:
        lea    edi, [esi + eax]
        mov    byte ptr [edi + edx + 0f2ch], 0
        mov    edi, dword ptr [ebp - 4]
        add    edi, eax
        inc    eax
        mov    word ptr [edx + edi*2 + 0ef0h], 2000h
        cmp    eax, 1eh
        jb     SHORT loc_1_4F03
        jmp    SHORT loc_1_4F28
loc_1_4F25:
        mov    edx, dword ptr [ebp + 8]
loc_1_4F28:
        cmp    bl, 7fh
        jne    SHORT loc_1_4F78
        and    ecx, 7fh
        cmp    cl, 1eh
        jae    SHORT loc_1_4F78
        mov    si, 2000h
        mov    eax, dword ptr [ebp + 0ch]
        imul   eax, eax, 5ch
        cmp    word ptr [edx + eax + 0eeeh], 0
        je     SHORT loc_1_4F66
        add    eax, ecx
        cmp    byte ptr [eax + edx + 0f2ch], 1
        jne    SHORT loc_1_4F66
        mov    eax, dword ptr [ebp + 0ch]
        imul   eax, eax, 2eh
        add    eax, ecx
        mov    si, word ptr [edx + eax*2 + 0ef0h]
loc_1_4F66:
        mov    eax, dword ptr [ebp + 0ch]
        shl    eax, 5
        mov    word ptr [eax + edx + 460h], si
        xor    eax, eax
        jmp    SHORT loc_1_4F7D
loc_1_4F78:
        mov    eax, 1
loc_1_4F7D:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_4F86:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        shl    eax, 5
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 8]
        lea    esi, [eax + edx]
        mov    eax, dword ptr [ebp + 10h]
        shl    ax, 8
        add    esi, 45eh
        mov    word ptr [esi - 4], 200h
        mov    cx, word ptr [esi]
        and    cx, 0ffh
        or     ax, cx
        mov    word ptr [esi], cx
        movzx  ecx, ax
        mov    word ptr [esi], ax
        push   ecx
        push   dword ptr [ebp + 0ch]
        push   edx
        call   sub_1_4EC2
        pop    esi
        pop    ebp
        ret
sub_1_4FCB:
        mov    ecx, dword ptr [esp + 8]
        push   esi
        push   edi
        mov    edx, 17fh
        mov    esi, dword ptr [esp + 14h]
        movsx  edi, word ptr [ecx + 22h]
        movzx  eax, byte ptr [esi + 7]
        add    eax, edi
        add    eax, eax
        sub    edx, eax
        cmp    edx, 0feh
        jl     SHORT loc_1_4FF7
        mov    edx, 0ffh
        jmp    SHORT loc_1_4FFE
loc_1_4FF7:
        cmp    edx, 1
        jg     SHORT loc_1_4FFE
        xor    edx, edx
loc_1_4FFE:
        movzx  ax, dl
        mov    word ptr [ecx + 22h], ax
        test   edx, edx
        je     SHORT loc_1_5016
        neg    dl
        movzx  ax, dl
        mov    word ptr [ecx + 24h], ax
        jmp    SHORT loc_1_501C
loc_1_5016:
        mov    word ptr [ecx + 24h], 0ffh
loc_1_501C:
        lea    edi, [ecx + 1eh]
        movzx  eax, byte ptr [esi + 6]
        movsx  edx, word ptr [edi]
        add    eax, edx
        mov    edx, 0ffh
        mov    word ptr [edi], dx
        cmp    eax, edx
        jg     SHORT loc_1_5037
        mov    word ptr [edi], ax
loc_1_5037:
        add    ecx, 20h
        movzx  eax, byte ptr [esi + 5]
        movsx  edx, word ptr [ecx]
        add    eax, edx
        mov    edx, 0ffh
        mov    word ptr [ecx], dx
        cmp    eax, edx
        jg     SHORT loc_1_5052
        mov    word ptr [ecx], ax
loc_1_5052:
        pop    edi
        pop    esi
        ret
sub_1_5055:
        mov    edx, dword ptr [esp + 4]
        push   esi
        test   edx, edx
        jne    SHORT loc_1_5064
        mov    ax, 7fh
        jmp    SHORT loc_1_5085
loc_1_5064:
        mov    eax, 1
        db  0B9h, 002h, 000h, 000h, 000h
loc_1_506E:
        movzx  esi, word ptr [ecx]
        cmp    esi, edx
        jb     SHORT loc_1_5085
        add    ecx, 2
        inc    eax
        db  081h, 0F9h, 0FEh, 000h, 000h, 000h
        jb     SHORT loc_1_506E
        mov    ax, 7eh
loc_1_5085:
        pop    esi
        ret
loc_1_5087:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    word ptr [esi + 2ah], 1eh
        cmp    dword ptr [esi + 154eh], 0
        jne    SHORT loc_1_50A2
        mov    word ptr [esi + 2ah], 20h
loc_1_50A2:
        lea    edi, [esi + 24h]
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_50AB:
        push   11h
        push   edi
        call   sub_1_18E9
        mov    al, 0a0h
        pop    edi
        movzx  edx, word ptr [esi + 28h]
        add    edx, 2
        pop    esi
        out    dx, al
        xor    eax, eax
        ret
sub_1_50C2:
        mov    eax, dword ptr [esp + 4]
        push   0
        add    eax, 24h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_50D1:
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        ret
sub_1_50D8:
        push   ebx
        push   esi
        push   edi
        mov    esi, 120h
        mov    edi, dword ptr [esp + 10h]
        push   ebp
        xor    ebp, ebp
        lea    ebx, [edi + 686h]
loc_1_50ED:
        mov    eax, dword ptr [ebx]
        test   eax, eax
        je     SHORT loc_1_50FE
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_50F9:
        add    esp, 4
        mov    dword ptr [ebx], ebp
loc_1_50FE:
        add    ebx, 4
        dec    esi
        jne    SHORT loc_1_50ED
        mov    al, 0a0h
        pop    ebp
        movzx  edx, word ptr [edi + 28h]
        add    edx, 2
        pop    edi
        out    dx, al
        xor    eax, eax
        pop    esi
        pop    ebx
        ret
sub_1_5115:
        xor    eax, eax
        ret
sub_1_5118:
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    ebx, dword ptr [esp + 10h]
        cmp    word ptr [ebx + 2ah], si
        jbe    SHORT loc_1_515E
        lea    edi, [ebx + 66h]
loc_1_512A:
        cmp    word ptr [edi], 2000h
        jae    SHORT loc_1_5152
        push   807fh
        mov    eax, esi
        or     eax, 2a0h
        lea    ecx, [ebx + 24h]
        push   eax
        push   ecx
        call   sub_1_19AA
        mov    word ptr [edi], 0ffeh
        mov    word ptr [edi + 1ah], 0
loc_1_5152:
        add    edi, 20h
        inc    esi
        movzx  eax, word ptr [ebx + 2ah]
        cmp    eax, esi
        jg     SHORT loc_1_512A
loc_1_515E:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_5164:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        lea    edi, [esi + 24h]
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5173:
        add    esp, 4
        push   9
        push   5
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5180:
        add    esp, 0ch
        push   327fh
        push   373h
        push   edi
        call   sub_1_19AA
        push   337fh
        push   37bh
        push   edi
        call   sub_1_19AA
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_51A9:
        push   -1
        push   7eh
        push   edi
        call   sub_1_1A14
        push   -1
        push   7fh
        push   edi
        call   sub_1_1A14
        push   4
        push   23fh
        push   edi
        call   sub_1_1942
        mov    al, 0a0h
        pop    edi
        movzx  edx, word ptr [esi + 28h]
        add    edx, 2
        pop    esi
        out    dx, al
        xor    eax, eax
        ret
sub_1_51D9:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        push   ebp
        xor    edi, edi
        lea    ebx, [esi + 7dh]
        mov    ebp, 1d8h
        mov    dword ptr [esi + 3ch], 20h
        add    esi, 24h
loc_1_51F5:
        push   80h
        mov    eax, edi
        or     eax, 2a0h
        push   eax
        push   esi
        call   sub_1_19AA
        push   ebp
        mov    eax, edi
        or     al, 0c0h
        push   eax
        push   esi
        call   sub_1_1A14
        push   1e0h
        mov    eax, edi
        or     al, 0e0h
        push   eax
        push   esi
        call   sub_1_1A14
        push   ebp
        mov    eax, edi
        or     ah, 2
        push   eax
        push   esi
        call   sub_1_1A14
        push   0ffffh
        mov    eax, edi
        or     al, 60h
        push   eax
        push   esi
        call   sub_1_1A14
        push   0ffffh
        mov    eax, edi
        or     al, 40h
        push   eax
        push   esi
        call   sub_1_1A14
        push   40000000h
        mov    eax, edi
        or     al, 20h
        inc    edi
        push   eax
        push   esi
        call   sub_1_1A14
        mov    byte ptr [ebx], 1
        add    ebx, 20h
        cmp    edi, 20h
        jb     SHORT loc_1_51F5
        push   0
        push   23fh
        push   esi
        call   sub_1_1942
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5280:
        movzx  eax, word ptr [esi + 12h]
        test   eax, eax
        jne    SHORT loc_1_52C0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_528E:
        add    esp, 4
        movsx  eax, byte ptr [esi + 14h]
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_529C:
        add    esp, 8
        movsx  ecx, byte ptr [esi + 15h]
        push   ecx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_52AA:
        movsx  ecx, byte ptr [esi + 17h]
        add    esp, 8
        movsx  eax, byte ptr [esi + 16h]
        push   ecx
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_52BD:
        add    esp, 0ch
loc_1_52C0:
        push   59h
        push   23dh
        push   esi
        call   sub_1_1942
        push   20h
        push   23eh
        push   esi
        call   sub_1_1942
        push   6
        push   23fh
        push   esi
        call   sub_1_1942
        cmp    word ptr [esi + 6], 1eh
        jne    SHORT loc_1_52F4
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_52F4:
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_52FB:
        push   ebp
        movzx  eax, word ptr [esp + 0ch]
        mov    ebp, esp
        sub    esp, 4
        cmp    eax, 0ch
        push   ebx
        push   esi
        push   edi
        ja     SHORT loc_1_5315
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_5315:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5326:
        add    esp, 10h
        jmp    NEAR PTR loc_1_5501
loc_1_532E:
        mov    esi, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [esi]
        mov    eax, ecx
        shr    eax, 10h
        movzx  ebx, ch
        movzx  edx, al
        movzx  eax, cl
        push   edx
        push   ebx
        push   eax
        push   dword ptr [ebp + 10h]
        call   sub_1_3C92
        jmp    NEAR PTR loc_1_54FF
loc_1_5351:
        mov    esi, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [esi]
        mov    eax, ecx
        shr    eax, 10h
        movzx  ebx, ch
        movzx  edx, al
        movzx  eax, cl
        push   edx
        push   ebx
        push   eax
        push   dword ptr [ebp + 10h]
        call   sub_1_3DB3
        jmp    NEAR PTR loc_1_54FF
loc_1_5374:
        mov    esi, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [esi]
        mov    eax, ecx
        shr    eax, 10h
        movzx  ebx, ch
        movzx  edx, al
        movzx  eax, cl
        push   edx
        push   ebx
        push   eax
        push   dword ptr [ebp + 10h]
        call   sub_1_4A99
        jmp    NEAR PTR loc_1_54FF
loc_1_5397:
        mov    esi, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [esi]
        mov    eax, ecx
        shr    eax, 10h
        movzx  ebx, ch
        movzx  edx, al
        movzx  eax, cl
        push   edx
        push   ebx
        push   eax
        push   dword ptr [ebp + 10h]
        call   sub_1_42A5
        jmp    NEAR PTR loc_1_54FF
loc_1_53BA:
        mov    esi, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [esi]
        movzx  eax, ch
        movzx  edx, cl
        push   eax
        push   edx
        push   dword ptr [ebp + 10h]
        call   sub_1_488D
        jmp    NEAR PTR loc_1_54FF
loc_1_53D4:
        mov    esi, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [esi]
        movzx  eax, ch
        movzx  edx, cl
        push   eax
        push   edx
        push   dword ptr [ebp + 10h]
        call   sub_1_4A9E
        jmp    NEAR PTR loc_1_54FF
loc_1_53EE:
        mov    esi, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [esi]
        mov    eax, ecx
        shr    eax, 10h
        movzx  ebx, ch
        movzx  edx, al
        movzx  eax, cl
        push   edx
        push   ebx
        push   eax
        push   dword ptr [ebp + 10h]
        call   sub_1_4AD6
        jmp    NEAR PTR loc_1_54FF
loc_1_5411:
        mov    esi, dword ptr [ebp + 14h]
        push   dword ptr [esi]
        add    esi, 8
        push   esi
        push   dword ptr [ebp + 10h]
        call   sub_1_4B8B
        jmp    NEAR PTR loc_1_54FF
loc_1_5427:
        push   10h
        push   dword ptr [ebp + 10h]
        call   sub_1_18E9
        push   dword ptr [ebp + 10h]
        call   sub_1_4BF6
        jmp    NEAR PTR loc_1_54FF
loc_1_543E:
        xor    edi, edi
        mov    ecx, dword ptr [ebp + 10h]
        add    ecx, 6
        cmp    word ptr [ecx], di
        mov    dword ptr [ebp - 4], ecx
        jbe    NEAR PTR loc_1_54FF
        mov    esi, dword ptr [ebp + 10h]
        add    esi, 42h
loc_1_5458:
        push   807fh
        mov    eax, edi
        or     eax, 2a0h
        inc    edi
        push   eax
        push   dword ptr [ebp + 10h]
        call   sub_1_19AA
        mov    word ptr [esi], 0ffeh
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [esi + 1ah], 0
        movzx  ecx, word ptr [eax]
        add    esi, 20h
        cmp    ecx, edi
        jg     SHORT loc_1_5458
        jmp    SHORT loc_1_54FF
loc_1_5488:
        mov    esi, dword ptr [ebp + 14h]
        movzx  ecx, word ptr [esi]
        lea    eax, [esi + 2]
        push   eax
        push   ecx
        push   dword ptr [ebp + 10h]
        call   sub_1_3F5F
        jmp    SHORT loc_1_54FF
loc_1_549D:
        mov    esi, dword ptr [ebp + 14h]
        mov    eax, dword ptr [esi]
        sub    eax, 2
        cdq
        test   al, 1
        je     SHORT loc_1_54BF
        sub    eax, edx
        mov    ecx, dword ptr [ebp + 10h]
        sar    eax, 1
        movzx  edx, word ptr [ecx + eax*2]
        add    edx, 2
        in     ax, dx
        movzx  eax, ax
        jmp    SHORT loc_1_5501
loc_1_54BF:
        sub    eax, edx
        mov    ecx, dword ptr [ebp + 10h]
        sar    eax, 1
        mov    dx, word ptr [ecx + eax*2]
        in     ax, dx
        movzx  eax, ax
        jmp    SHORT loc_1_5501
loc_1_54D1:
        mov    esi, dword ptr [ebp + 14h]
        mov    eax, dword ptr [esi]
        sub    eax, 2
        cdq
        test   al, 1
        je     SHORT loc_1_54EE
        sub    eax, edx
        mov    ecx, dword ptr [ebp + 10h]
        sar    eax, 1
        movzx  edx, word ptr [ecx + eax*2]
        add    edx, 2
        jmp    SHORT loc_1_54F9
loc_1_54EE:
        sub    eax, edx
        mov    ecx, dword ptr [ebp + 10h]
        sar    eax, 1
        mov    dx, word ptr [ecx + eax*2]
loc_1_54F9:
        mov    ax, word ptr [esi + 4]
        out    dx, ax
loc_1_54FF:
        xor    eax, eax
loc_1_5501:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
_LTEXT ends
        end
