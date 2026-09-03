; netbeui_lcode_01_02.asm -- NETBEUI object 1 segment _LTEXT  (part 2/2)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_470E:
        test   word ptr [ebx + 40h], 40h
        je     SHORT loc_1_4718
        stc
        ret
loc_1_4718:
        push   eax
        push   esi
        push   edi
        push   ebx
        test   word ptr [ebx + 42h], 20h
        je     SHORT loc_1_472C
        cmp    byte ptr [edi + 7], 1
        je     SHORT loc_1_4759
        jmp    SHORT loc_1_475E
loc_1_472C:
        mov    esi, eax
        mov    edi, edx
        cmp    dword ptr [esi + 28h], 0
        je     SHORT loc_1_4743
        mov    eax, dword ptr [edi + 8]
        and    al, 7fh
        add    edi, 2
        add    ebx, 2
        jmp    SHORT loc_1_4746
loc_1_4743:
        mov    eax, dword ptr [edi + 6]
loc_1_4746:
        cmp    eax, dword ptr [ebx + 98h]
        jne    SHORT loc_1_475E
        mov    eax, dword ptr [edi + 8]
        cmp    eax, dword ptr [ebx + 9ah]
        jne    SHORT loc_1_475E
loc_1_4759:
        pop    ebx
        pop    edi
        pop    esi
        pop    eax
        ret
loc_1_475E:
        pop    ebx
        pop    edi
        pop    esi
        pop    eax
        or     word ptr [ebx + 42h], 40h
        call   sub_1_38B0
        mov    ecx, eax
        test   dword ptr [eax + 4], 1
        jne    SHORT loc_1_477B
        mov    al, 1
        jmp    SHORT loc_1_4790
loc_1_477B:
        test   dword ptr [eax + 4], 2
        je     SHORT loc_1_478B
        db  0A0h, 021h, 06Fh, 000h, 000h
        jmp    SHORT loc_1_4790
loc_1_478B:
        db  0A0h, 01Fh, 06Fh, 000h, 000h
loc_1_4790:
        mov    byte ptr [ebx + 90h], al
        db  0B8h, 0CCh, 046h, 000h, 000h
        call   sub_1_34D0
        cmp    byte ptr [ebx + 25h], 5
        jne    SHORT loc_1_47A8
        stc
        ret
loc_1_47A8:
        test   word ptr [ebx + 40h], 80h
        je     SHORT loc_1_47C5
        mov    esi, dword ptr [ebx + 88h]
        and    word ptr [ebx + 40h], 0ff7fh
        mov    byte ptr [esi + 1], 19h
        call   sub_1_6BE9
loc_1_47C5:
        stc
        ret
loc_1_47C7:
        movzx  ebx, byte ptr [edi + 8]
        dec    ebx
        cmp    ebx, 0fdh
        ja     SHORT loc_1_4827
        db  08Bh, 01Ch, 09Dh, 0C0h, 077h, 000h, 000h
        or     ebx, ebx
        je     SHORT loc_1_4827
        cmp    byte ptr [ebx + 25h], 0
        je     SHORT loc_1_4827
        test   word ptr [ebx + 40h], 8
        je     SHORT loc_1_47F5
        call   sub_1_470E
        jae    SHORT loc_1_47F5
        ret
loc_1_47F5:
        cmp    byte ptr [edi + 7], 0
        je     SHORT loc_1_4800
        or     word ptr [ebx + 42h], 20h
loc_1_4800:
        push   eax
        movzx  eax, byte ptr [edi + 5]
        mov    word ptr [ebx + 0cah], ax
        pop    eax
        or     word ptr [ebx + 40h], 8
        cli
        test   word ptr [ebx + 40h], 80h
        je     SHORT loc_1_4827
        cmp    byte ptr [ebx + 25h], 1
        je     SHORT loc_1_4857
        cmp    byte ptr [ebx + 25h], 7
        je     SHORT loc_1_4828
loc_1_4827:
        ret
loc_1_4828:
        ret
loc_1_4829:
        sti
        pop    eax
        mov    esi, eax
        mov    edi, edx
        cmp    dword ptr [esi + 28h], 0
        je     SHORT loc_1_484C
        mov    eax, dword ptr [edi + 8]
        and    al, 7fh
loc_1_483A:
        mov    dword ptr [ebx + 9ah], eax
        mov    ax, word ptr [edi + 0ch]
        mov    word ptr [ebx + 9eh], ax
        ret
loc_1_484C:
        mov    eax, dword ptr [edi + 6]
        sub    edi, 2
        sub    ebx, 2
        jmp    SHORT loc_1_483A
loc_1_4857:
        push   eax
        mov    al, byte ptr [edi + 6]
        or     al, al
        je     SHORT loc_1_4829
        mov    ecx, dword ptr [ebx + 88h]
        db  08Bh, 035h, 017h, 06Fh, 000h, 000h
        mov    dword ptr [ecx + 32h], esi
        db  0C7h, 041h, 03Ah, 06Ah, 00Bh, 000h, 000h
        sti
        mov    byte ptr [ebx + 69h], al
        mov    ax, word ptr [edi + 0ah]
        mov    word ptr [ebx + 64h], ax
        pop    eax
        push   ebx
        push   edx
        push   edi
        mov    edi, edx
        cli
        call   sub_1_236B
        jae    SHORT loc_1_4901
        cli
        push   eax
        mov    eax, 0c8h
        call   sub_1_2233
        mov    esi, eax
        pop    eax
        test   esi, esi
        je     SHORT loc_1_48F6
        mov    dword ptr [esi + 40h], eax
        call   sub_1_27B0
        sti
        jb     SHORT loc_1_48F6
        mov    esi, ebx
        pop    edi
        pop    edx
        pop    ebx
        mov    eax, esi
        call   sub_1_6298
        mov    dword ptr [ebx + 38h], esi
        mov    byte ptr [ebx + 25h], 3
        mov    edi, dword ptr [ebx + 94h]
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [esi + 40h], eax
        cli
        or     byte ptr [ebx + 24h], 2
        push   esi
        lea    esi, [esi]
        call   sub_1_2892
        pop    esi
        sti
        mov    byte ptr [esi + 38h], 1
        mov    dword ptr [esi + 3ch], 8
        xchg   ebx, esi
        call   sub_1_2959
        xchg   ebx, esi
        push   esi
        call   sub_1_4E6C
        ret
loc_1_48F6:
        pop    edi
        pop    edx
        pop    ebx
        mov    al, 9
        call   sub_1_0BAA
        ret
loc_1_4901:
        mov    esi, ebx
        pop    edi
        pop    edx
        pop    ebx
        mov    eax, esi
        call   sub_1_6298
        or     byte ptr [ebx + 24h], 2
        push   esi
        lea    esi, [esi]
        call   sub_1_2892
        pop    esi
        mov    dword ptr [ebx + 38h], esi
        cmp    byte ptr [esi + 38h], 4
        jne    SHORT loc_1_493A
        sti
        mov    byte ptr [ebx + 25h], 4
        mov    eax, dword ptr [esi + 40h]
        xchg   ebx, esi
        call   sub_1_2959
        xchg   ebx, esi
        call   sub_1_3BB8
        ret
loc_1_493A:
        mov    byte ptr [ebx + 25h], 3
        cmp    byte ptr [esi + 38h], 1
        je     SHORT loc_1_494B
        or     dword ptr [esi + 34h], 800h
loc_1_494B:
        sti
        ret
loc_1_494D:
        mov    ebx, eax
        cmp    byte ptr [ebx + 25h], 5
        jne    SHORT loc_1_495A
        call   sub_1_569A
loc_1_495A:
        ret
loc_1_495B:
        mov    ebx, eax
        cmp    byte ptr [ebx + 25h], 5
        jne    SHORT loc_1_49A2
        test   word ptr [ebx + 40h], 20h
        jne    SHORT loc_1_49A2
        test   byte ptr [edi + 5], 8
        je     SHORT loc_1_4976
        call   sub_1_569A
loc_1_4976:
        test   word ptr [ebx + 40h], 4000h
        jne    SHORT loc_1_49AD
loc_1_497E:
        and    word ptr [ebx + 40h], 0bfffh
        call   sub_1_5CEC
        jb     SHORT loc_1_49B5
        call   sub_1_608F
        jb     SHORT loc_1_49DC
        cmp    word ptr [ebx + 86h], 0
        je     SHORT loc_1_49DC
        test   byte ptr [edi + 5], 1
        jne    SHORT loc_1_49A3
loc_1_49A2:
        ret
loc_1_49A3:
        mov    ax, word ptr [edi + 0ah]
        call   sub_1_3E03
        ret
loc_1_49AD:
        cmp    word ptr [edi + 6], 1
        je     SHORT loc_1_497E
        ret
loc_1_49B5:
        or     word ptr [ebx + 40h], 0c000h
        mov    eax, dword ptr [ebx + 38h]
        mov    eax, dword ptr [eax + 40h]
        inc    dword ptr [eax + 100h]
        call   sub_1_5C40
        test   byte ptr [edi + 5], 2
        jne    SHORT loc_1_49D3
        ret
loc_1_49D3:
        xor    eax, eax
        stc
        call   sub_1_3DA6
        ret
loc_1_49DC:
        or     word ptr [ebx + 40h], 4000h
        mov    eax, dword ptr [ebx + 38h]
        mov    eax, dword ptr [eax + 40h]
        inc    dword ptr [eax + 100h]
        movzx  eax, word ptr [ebx + 84h]
        push   eax
        test   byte ptr [edi + 5], 2
        jne    SHORT loc_1_4A23
        mov    al, 6
        call   sub_1_5744
        call   sub_1_5CEC
        pop    eax
        jb     SHORT loc_1_4A11
        call   sub_1_3D51
        ret
loc_1_4A11:
        clc
        call   sub_1_3DA6
loc_1_4A17:
        or     word ptr [ebx + 40h], 8000h
        call   sub_1_5C40
        ret
loc_1_4A23:
        pop    eax
        stc
        call   sub_1_3DA6
        mov    al, 6
        call   sub_1_5744
        call   sub_1_5CEC
        jb     SHORT loc_1_4A17
        xor    eax, eax
        call   sub_1_3D51
        ret
loc_1_4A40:
        cmp    word ptr [edi + 6], 1
        je     SHORT loc_1_4A6F
        test   byte ptr [edi + 5], 8
        je     SHORT loc_1_4A52
        call   sub_1_569A
loc_1_4A52:
        ret
loc_1_4A53:
        mov    ebx, eax
        cmp    byte ptr [ebx + 25h], 5
        jne    NEAR PTR loc_1_4ADF
        test   word ptr [ebx + 40h], 20h
        jne    SHORT loc_1_4AE0
        test   word ptr [ebx + 40h], 4000h
        jne    SHORT loc_1_4A40
loc_1_4A6F:
        and    word ptr [ebx + 40h], 0bfffh
        call   sub_1_5CEC
        jb     SHORT loc_1_4AEE
        call   sub_1_608F
        jb     NEAR PTR loc_1_4B1F
        mov    cl, byte ptr [edi + 5]
        mov    ch, 0ffh
        test   cl, 2
        jne    SHORT loc_1_4ABA
        mov    ax, word ptr [edi + 0ah]
        mov    word ptr [ebx + 64h], ax
        test   cl, 4
        je     SHORT loc_1_4ABC
        test   word ptr [ebx + 6ah], 20h
        jne    SHORT loc_1_4ABC
        db  080h, 03Dh, 032h, 06Fh, 000h, 000h, 000h
        je     SHORT loc_1_4ABC
        or     word ptr [ebx + 6ah], 4
        db  0FFh, 005h, 0FFh, 06Eh, 000h, 000h
loc_1_4ABA:
        xor    ch, ch
loc_1_4ABC:
        sub    al, al
        call   sub_1_5744
        test   cl, 8
        je     SHORT loc_1_4ACD
        call   sub_1_569A
loc_1_4ACD:
        or     ch, ch
        jne    SHORT loc_1_4AD2
        ret
loc_1_4AD2:
        mov    ax, word ptr [ebx + 64h]
        call   sub_1_3E4E
        jb     SHORT loc_1_4AE1
        sti
        ret
loc_1_4ADF:
        ret
loc_1_4AE0:
        ret
loc_1_4AE1:
        or     word ptr [ebx + 6ah], 4
        db  0FFh, 005h, 0FFh, 06Eh, 000h, 000h
        sti
        ret
loc_1_4AEE:
        or     word ptr [ebx + 40h], 0c000h
        mov    eax, dword ptr [ebx + 38h]
        mov    eax, dword ptr [eax + 40h]
        inc    dword ptr [eax + 100h]
        call   sub_1_5C40
        test   byte ptr [edi + 5], 8
        je     SHORT loc_1_4B10
        call   sub_1_569A
loc_1_4B10:
        test   byte ptr [edi + 5], 2
        jne    SHORT loc_1_4B17
        ret
loc_1_4B17:
        xor    eax, eax
        stc
        jmp    NEAR PTR sub_1_3DA6
loc_1_4B1F:
        or     word ptr [ebx + 40h], 4000h
        mov    eax, dword ptr [ebx + 38h]
        mov    eax, dword ptr [eax + 40h]
        inc    dword ptr [eax + 100h]
        movzx  eax, word ptr [ebx + 84h]
        push   eax
        mov    al, 6
        call   sub_1_5744
        test   byte ptr [edi + 5], 8
        je     SHORT loc_1_4B4B
        call   sub_1_569A
loc_1_4B4B:
        test   byte ptr [edi + 5], 2
        jne    SHORT loc_1_4B5F
        call   sub_1_5CEC
        pop    eax
        jb     SHORT loc_1_4B74
loc_1_4B59:
        call   sub_1_3D51
        ret
loc_1_4B5F:
        pop    eax
        stc
        call   sub_1_3DA6
        call   sub_1_5CEC
        mov    eax, 0
        jae    SHORT loc_1_4B59
        jmp    SHORT loc_1_4B7A
loc_1_4B74:
        clc
        call   sub_1_3DA6
loc_1_4B7A:
        or     word ptr [ebx + 40h], 8000h
        call   sub_1_5C40
        ret
sub_1_4B86:
        mov    edi, dword ptr [ebx + 94h]
        lea    esi, [ebx + 28h]
        cmp    esi, dword ptr [esi]
        je     SHORT loc_1_4BF3
        cmp    word ptr [ebx + 84h], 0
        jne    SHORT loc_1_4BB1
        mov    al, 7
        call   sub_1_5744
        cmp    esi, dword ptr [esi]
        je     SHORT loc_1_4BAF
        push   ecx
        call   sub_1_5CEC
        pop    ecx
loc_1_4BAF:
        clc
        ret
loc_1_4BB1:
        mov    eax, esi
        mov    esi, dword ptr [esi]
        mov    esi, dword ptr [esi]
        cmp    eax, esi
        je     SHORT loc_1_4BF3
        push   esi
        push   edi
        lea    esi, [esi]
        cli
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        sti
        pop    edi
        pop    esi
        dec    byte ptr [ebx + 8ch]
        mov    al, byte ptr [ebx + 68h]
        mov    ah, byte ptr [edi + 2]
        lea    edi, [esi - 32h]
        mov    word ptr [edi + 2], ax
loc_1_4BE0:
        db  0FEh, 005h, 005h, 06Fh, 000h, 000h
        mov    esi, edi
        mov    byte ptr [esi + 1], 7
        call   sub_1_6BE9
        clc
        ret
loc_1_4BF3:
        cli
        lea    esi, [edi + 34h]
        call   sub_1_28E3
        lea    esi, [esi - 32h]
        sti
        je     SHORT loc_1_4C0F
        dec    byte ptr [edi + 19h]
        mov    edi, esi
loc_1_4C07:
        mov    al, byte ptr [ebx + 68h]
        mov    byte ptr [edi + 2], al
        jmp    SHORT loc_1_4BE0
loc_1_4C0F:
        mov    al, byte ptr [edi + 2]
        mov    edi, dword ptr [edi + 4]
        cli
        lea    esi, [edi + 9ch]
        call   sub_1_28E3
        lea    esi, [esi - 32h]
        sti
        je     SHORT loc_1_4C2E
        mov    edi, esi
        mov    byte ptr [edi + 3], al
        jmp    SHORT loc_1_4C07
loc_1_4C2E:
        stc
        ret
loc_1_4C30:
        mov    ebx, eax
        cmp    byte ptr [ebx + 25h], 5
        jne    SHORT loc_1_4C4E
        or     word ptr [ebx + 40h], 2
        test   byte ptr [edi + 5], 2
        jne    SHORT loc_1_4C4F
        mov    cx, word ptr [edi + 6]
loc_1_4C47:
        mov    word ptr [ebx + 0cch], cx
loc_1_4C4E:
        ret
loc_1_4C4F:
        xor    ecx, ecx
        call   sub_1_4B86
        jae    SHORT loc_1_4C47
        or     word ptr [ebx + 42h], 4
        call   sub_1_5C40
        jmp    SHORT loc_1_4C47
loc_1_4C64:
        mov    ebx, eax
        cmp    byte ptr [ebx + 25h], 5
        jne    SHORT loc_1_4C7F
        and    word ptr [ebx + 40h], 0fffdh
        push   ebx
        call   sub_1_57BB
        push   dword ptr [ebx + 38h]
        call   sub_1_4E6C
loc_1_4C7F:
        ret
loc_1_4C80:
        mov    ebx, eax
        cmp    byte ptr [ebx + 25h], 5
        jne    SHORT loc_1_4CCA
        movzx  ecx, word ptr [edi + 6]
        test   word ptr [ebx + 40h], 2
        je     SHORT loc_1_4C9B
        movzx  ecx, word ptr [ebx + 0cch]
loc_1_4C9B:
        test   word ptr [ebx + 40h], 400h
        je     SHORT loc_1_4CB2
        test   word ptr [ebx + 42h], 200h
        jne    SHORT loc_1_4CB2
        call   sub_1_5E8F
        jb     SHORT loc_1_4CB7
loc_1_4CB2:
        or     word ptr [ebx + 42h], 1
loc_1_4CB7:
        and    word ptr [ebx + 40h], 0fffdh
        push   ebx
        call   sub_1_57BB
        push   dword ptr [ebx + 38h]
        call   sub_1_4E6C
loc_1_4CCA:
        ret
loc_1_4CCB:
        lea    esi, [edi + 0ch]
        push   eax
        call   sub_1_2480
        jae    SHORT loc_1_4D06
        pop    eax
        cli
        lea    esi, [eax + 0ach]
        call   sub_1_28E3
        lea    esi, [esi - 32h]
        sti
        je     SHORT loc_1_4D05
        push   esi
        push   edi
        mov    edi, esi
        add    edi, 1ah
        pop    esi
        add    esi, 0ch
        cld
        movsd
        movsd
        movsd
        movsd
        mov    edi, esi
        sub    edi, 1ch
        pop    esi
        sub    cl, cl
        mov    ebx, eax
        jmp    SHORT loc_1_4D3C
loc_1_4D05:
        ret
loc_1_4D06:
        add    esp, 4
        mov    cl, byte ptr [ebx + 2]
        cli
        lea    esi, [ebx + 2ch]
        call   sub_1_28E3
        lea    esi, [esi - 32h]
        sti
        jne    SHORT loc_1_4D31
        mov    ebx, dword ptr [ebx + 4]
        cli
        lea    esi, [ebx + 94h]
        call   sub_1_28E3
        lea    esi, [esi - 32h]
        sti
        jne    SHORT loc_1_4D3C
        ret
loc_1_4D31:
        dec    byte ptr [ebx + 18h]
        test   byte ptr [ebx], 2
        jne    SHORT loc_1_4D70
        mov    ebx, dword ptr [ebx + 4]
loc_1_4D3C:
        push   edi
        mov    eax, edi
        sub    eax, edx
        add    eax, 2ch
        mov    edi, ebx
        mov    ebx, eax
        db  0FEh, 005h, 005h, 06Fh, 000h, 000h
        mov    byte ptr [esi + 3], cl
        call   sub_1_60E7
        mov    edx, dword ptr [esi + 4]
        movzx  ecx, word ptr [esi + 8]
        push   esi
        call   sub_1_60D4
        pop    esi
        mov    byte ptr [esi + 1], al
        mov    word ptr [esi + 8], cx
        jmp    NEAR PTR sub_1_6BE9
loc_1_4D70:
        mov    edi, esi
        mov    al, 19h
        cli
        call   sub_1_2913
        sti
        ret
loc_1_4D7C:
        add    esp, 14h
        pop    ebp
        ret
loc_1_4D81:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        mov    ebx, eax
        mov    dword ptr [ebp - 12h], edi
        cli
        lea    esi, [ebx + 0a4h]
        call   sub_1_28E3
        lea    esi, [esi - 32h]
        sti
        je     SHORT loc_1_4D7C
        mov    ecx, edi
        sub    ecx, edx
        add    ecx, 2ch
        push   ecx
        mov    dword ptr [ebp - 4], esi
        mov    eax, esi
        mov    esi, ebx
        mov    bl, byte ptr [eax + 3]
        call   sub_1_24CC
        dec    byte ptr [ebx + 18h]
        mov    ebx, esi
        mov    esi, eax
        db  0FEh, 005h, 005h, 06Fh, 000h, 000h
        push   dword ptr [ebp - 12h]
        call   sub_1_60E7
        mov    edi, ebx
        pop    ebx
        mov    edx, dword ptr [esi + 4]
        mov    dword ptr [ebp - 8], edx
        movzx  ecx, word ptr [esi + 8]
        push   ebp
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebp, dword ptr [ebx + 8]
        pop    ebx
        call   sub_1_60D4
        pop    ebp
        mov    byte ptr [esi + 1], al
        mov    word ptr [esi + 8], cx
        mov    byte ptr [ebp - 0eh], al
        mov    dword ptr [ebp - 0ch], ecx
loc_1_4DF6:
        cli
        lea    esi, [edi + 0a4h]
        call   sub_1_28E3
        lea    esi, [esi - 32h]
        sti
        je     SHORT loc_1_4E5C
        mov    eax, esi
        mov    esi, edi
        mov    edi, eax
        push   ebx
        mov    bl, byte ptr [edi + 3]
        call   sub_1_24CC
        dec    byte ptr [ebx + 18h]
        pop    ebx
        xchg   esi, edi
        db  0FEh, 005h, 005h, 06Fh, 000h, 000h
        push   dword ptr [ebp - 12h]
        call   sub_1_60E7
        movzx  ecx, word ptr [esi + 8]
        mov    eax, dword ptr [ebp - 0ch]
        sub    ecx, eax
        jae    SHORT loc_1_4E3A
        mov    byte ptr [ebp - 0eh], 6
loc_1_4E3A:
        sbb    edx, edx
        and    ecx, edx
        add    ecx, eax
        mov    word ptr [esi + 8], cx
        push   dword ptr [esi + 4]
        push   dword ptr [ebp - 8]
        call   sub_1_402C
        mov    al, byte ptr [ebp - 0eh]
        mov    byte ptr [esi + 1], al
        call   sub_1_6BE9
        jmp    SHORT loc_1_4DF6
loc_1_4E5C:
        mov    esi, dword ptr [ebp - 4]
        call   sub_1_6BE9
        add    esp, 14h
        pop    ebp
        ret
        db  000h, 000h, 000h
sub_1_4E6C:
        push   edi
        mov    edi, dword ptr [esp + 8]
        cli
        test   byte ptr [edi + 0b8h], 1
        jne    SHORT loc_1_4EA8
        or     byte ptr [edi + 0b8h], 1
        push   ebx
        mov    ebx, dword ptr [edi + 40h]
        push   edi
        push   eax
        lea    eax, [ebx + 0c4h]
        lea    edi, [edi + 24h]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [ebx + 0c8h]
        mov    dword ptr [edi + 4], eax
        xchg   edi, eax
        mov    dword ptr [edi], eax
        mov    dword ptr [ebx + 0c8h], eax
        pop    eax
        pop    edi
        pop    ebx
loc_1_4EA8:
        db  083h, 00Dh, 0ECh, 06Eh, 000h, 000h, 008h
        sti
        pop    edi
        ret    4
sub_1_4EB4:
        push   esi
        mov    esi, dword ptr [edi + 40h]
        cli
        dec    byte ptr [esi + 30h]
        js     SHORT loc_1_4F1F
        sti
        movzx  ebx, byte ptr [edi + 0b6h]
        mov    word ptr [ebx + edi + 8ch], dx
        mov    word ptr [ebx + edi + 8eh], ax
        lea    eax, [edi + 8ch]
        mov    dword ptr [esi + 60h], eax
        mov    word ptr [esi + 64h], bx
        add    word ptr [esi + 64h], cx
        movzx  ebx, byte ptr [edi + 0b7h]
        xchg   cl, ch
        mov    word ptr [ebx + edi + 8ch], cx
        mov    eax, dword ptr [edi + 34h]
        and    eax, 400h
        or     word ptr [esi + 58h], ax
        push   esi
        lea    esi, [esi + 48h]
        mov    dword ptr [esi + 0ch], edi
        call   sub_1_0517
        pop    esi
        and    word ptr [esi + 58h], 0fbffh
        cmp    eax, 2
        ja     SHORT loc_1_4F25
        pop    esi
        clc
        ret
loc_1_4F1F:
        inc    byte ptr [esi + 30h]
        or     dword ptr [esi], 10h
loc_1_4F25:
        cli
        test   byte ptr [edi + 0b8h], 1
        jne    SHORT loc_1_4F48
        or     byte ptr [edi + 0b8h], 1
        push   esi
        lea    esi, [esi + 0c4h]
        push   edi
        lea    edi, [edi + 24h]
        call   sub_1_28BB
        pop    edi
        pop    esi
loc_1_4F48:
        sti
        pop    esi
        stc
        ret
loc_1_4F4C:
        or     dword ptr [ecx], 10h
        test   byte ptr [edi + 0b8h], 1
        jne    SHORT loc_1_4F71
        or     byte ptr [edi + 0b8h], 1
        push   esi
        lea    esi, [ecx + 0c4h]
        push   edi
        lea    edi, [edi + 24h]
        call   sub_1_28BB
        pop    edi
        pop    esi
loc_1_4F71:
        inc    byte ptr [ecx + 30h]
        push   edi
        lea    edi, [edi + 8]
        call   sub_1_28A7
        pop    edi
loc_1_4F7E:
        sti
        stc
        ret
sub_1_4F81:
        cli
        push   eax
        mov    esi, dword ptr [edi + 8]
        mov    esi, dword ptr [esi]
        lea    eax, [edi + 8]
        mov    dword ptr [esi + 4], eax
        xchg   dword ptr [edi + 8], esi
        cmp    esi, eax
        pop    eax
        je     SHORT loc_1_4F7E
sub_1_4F96:
        mov    ecx, dword ptr [edi + 40h]
        dec    byte ptr [ecx + 30h]
        js     SHORT loc_1_4F4C
        test   dword ptr [edi + 34h], 0ah
        jne    SHORT loc_1_4F71
        mov    ecx, dword ptr [esi + 18h]
        movzx  edx, byte ptr [edi + 0b6h]
        add    ecx, edx
        mov    dx, word ptr [edi + 44h]
        add    dx, dx
        mov    word ptr [ecx + 2], dx
        and    byte ptr [ecx + 1], 0feh
        mov    al, 81h
        add    al, byte ptr [edi + 44h]
        sub    al, byte ptr [edi + 46h]
        and    al, 7fh
        cmp    byte ptr [edi + 48h], al
        jbe    NEAR PTR loc_1_5082
loc_1_4FD5:
        push   esi
        lea    esi, [esi + 34h]
        cld
        lodsd
        mov    dword ptr [ecx + 8], eax
        lodsd
        mov    dword ptr [ecx + 0ch], eax
        pop    esi
        mov    ecx, dword ptr [esi + 18h]
        movzx  eax, byte ptr [edi + 0b7h]
        add    ecx, eax
        mov    ax, word ptr [esi + 3ch]
        xchg   al, ah
        mov    word ptr [ecx], ax
        mov    al, byte ptr [edi + 44h]
        inc    al
        and    al, 7fh
        mov    byte ptr [edi + 44h], al
        mov    ah, byte ptr [edi + 46h]
        mov    cl, al
        mov    ch, byte ptr [edi + 81h]
        sub    cl, ah
        sub    ch, ah
        and    cx, 7f7fh
        cmp    ch, cl
        ja     SHORT loc_1_5020
        mov    byte ptr [edi + 81h], al
loc_1_5020:
        inc    byte ptr [edi + 0bah]
        test   dword ptr [edi + 34h], 4000h
        jne    SHORT loc_1_506D
loc_1_502F:
        mov    al, byte ptr [edi + 45h]
        mov    byte ptr [edi + 47h], al
        lea    eax, [edi + 10h]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [eax], esi
        mov    dword ptr [edi + 14h], esi
        and    dword ptr [edi + 3ch], 0ffffff7fh
        mov    dword ptr [edi + 60h], 0ffffffffh
        btr    word ptr [esi + 10h], 2
        jae    SHORT loc_1_506B
        mov    ecx, dword ptr [esi + 30h]
        inc    byte ptr [ecx + 6ch]
        db  0A0h, 02Fh, 06Fh, 000h, 000h
        cmp    byte ptr [ecx + 6ch], al
        je     SHORT loc_1_50B7
loc_1_506B:
        clc
        ret
loc_1_506D:
        cmp    byte ptr [edi + 0bah], 64h
        jb     SHORT loc_1_502F
        push   ebx
        mov    ebx, edi
        cli
        call   sub_1_1EE8
        pop    ebx
        jmp    SHORT loc_1_502F
loc_1_5082:
        cmp    byte ptr [esi + 34h], 16h
        je     NEAR PTR loc_1_4FD5
        push   edi
        mov    edi, dword ptr [edi + 40h]
        lea    eax, [edi + 0c4h]
        cmp    eax, dword ptr [edi + 0c4h]
        pop    edi
        jne    NEAR PTR loc_1_4FD5
        or     byte ptr [ecx + 3], 1
        or     dword ptr [edi + 34h], 2008h
        and    dword ptr [edi + 3ch], 0fffffffeh
        jmp    NEAR PTR loc_1_4FD5
loc_1_50B7:
        db  083h, 00Dh, 0ECh, 06Eh, 000h, 000h, 020h
        test   byte ptr [ecx + 24h], 8
        jne    SHORT loc_1_506B
        push   ecx
        push   eax
        db  08Dh, 005h, 0A0h, 06Fh, 000h, 000h
        lea    ecx, [ecx + 10h]
        mov    dword ptr [ecx], eax
        db  0A1h, 0A4h, 06Fh, 000h, 000h
        mov    dword ptr [ecx + 4], eax
        xchg   ecx, eax
        mov    dword ptr [ecx], eax
        db  0A3h, 0A4h, 06Fh, 000h, 000h
        pop    eax
        pop    ecx
        or     byte ptr [ecx + 24h], 8
        jmp    SHORT loc_1_506B
sub_1_50E9:
        mov    dword ptr [esi + 0ch], edi
        sti
        call   sub_1_0517
        db  08Ah, 00Dh, 020h, 06Fh, 000h, 000h
        mov    byte ptr [edi + 0b9h], cl
        push   edi
        mov    edi, dword ptr [edi + 40h]
        test   dword ptr [edi + 4], 2
        pop    edi
        je     SHORT loc_1_510C
loc_1_510C:
        push   eax
        mov    eax, dword ptr [edi + 70h]
        mov    dword ptr [edi + 5ch], eax
        pop    eax
        cmp    eax, 2
        ja     SHORT loc_1_511B
        clc
        ret
loc_1_511B:
        and    dword ptr [edi + 34h], 0fffffff7h
        stc
        ret
sub_1_5121:
        test   dword ptr [edi + 3ch], 10h
        je     SHORT loc_1_51A2
        mov    al, 5
        mov    ah, byte ptr [edi + 45h]
        add    ah, ah
        or     ah, 1
        test   dword ptr [edi + 34h], 1
        jne    SHORT loc_1_516E
        mov    al, 1
        mov    ebx, eax
        call   sub_1_4F81
        mov    eax, ebx
        jb     SHORT loc_1_516E
        test   dword ptr [edi + 34h], 8
        jne    SHORT loc_1_5199
        mov    ebx, dword ptr [esi + 18h]
        movzx  ecx, byte ptr [edi + 0b6h]
        add    ebx, ecx
        or     byte ptr [ebx + 3], 1
        or     byte ptr [ebx + 1], 1
        call   sub_1_50E9
        jae    SHORT loc_1_5181
loc_1_516E:
        mov    ecx, 4
        mov    dx, word ptr [edi + 78h]
        or     dh, 1
        call   sub_1_4EB4
        jb     SHORT loc_1_5198
loc_1_5181:
        cli
        and    dword ptr [edi + 3ch], 0ffffff6fh
loc_1_5189:
        mov    al, byte ptr [edi + 45h]
        mov    byte ptr [edi + 47h], al
        sti
        mov    dword ptr [edi + 60h], 0ffffffffh
        clc
loc_1_5198:
        ret
loc_1_5199:
        call   sub_1_50E9
        cli
        jae    SHORT loc_1_5189
        sti
loc_1_51A2:
        stc
        ret
sub_1_51A4:
        test   dword ptr [edi + 3ch], 80h
        je     SHORT loc_1_5208
        mov    al, 5
        mov    ah, byte ptr [edi + 45h]
        add    ah, ah
        test   dword ptr [edi + 34h], 1
        jne    SHORT loc_1_51DD
        mov    al, 1
        mov    ebx, eax
        call   sub_1_4F81
        mov    eax, ebx
        jb     SHORT loc_1_51DD
        mov    ebx, dword ptr [esi + 18h]
        movzx  ecx, byte ptr [edi + 0b6h]
        add    ebx, ecx
        call   sub_1_50E9
        jae    SHORT loc_1_51F0
loc_1_51DD:
        mov    ecx, 4
        mov    dx, word ptr [edi + 78h]
        or     dh, 1
        call   sub_1_4EB4
        jb     SHORT loc_1_5207
loc_1_51F0:
        cli
        and    dword ptr [edi + 3ch], 0ffffff7fh
        mov    al, byte ptr [edi + 45h]
        mov    byte ptr [edi + 47h], al
        sti
        mov    dword ptr [edi + 60h], 0ffffffffh
        clc
loc_1_5207:
        ret
loc_1_5208:
        stc
        ret
sub_1_520A:
        test   dword ptr [edi + 3ch], 1
        je     SHORT loc_1_5220
        test   dword ptr [edi + 34h], 8
        je     SHORT loc_1_5222
        and    dword ptr [edi + 3ch], 0fffffffeh
loc_1_5220:
        stc
loc_1_5221:
        ret
loc_1_5222:
        mov    al, 5
        mov    ah, byte ptr [edi + 45h]
        add    ah, ah
        or     ah, 1
        test   dword ptr [edi + 34h], 1
        jne    SHORT loc_1_5237
        mov    al, 1
loc_1_5237:
        mov    ecx, 4
        mov    dx, word ptr [edi + 78h]
        call   sub_1_4EB4
        jb     SHORT loc_1_5221
        cli
        and    dword ptr [edi + 3ch], 0ffffff7eh
        or     dword ptr [edi + 34h], 8
        mov    al, byte ptr [edi + 45h]
        mov    byte ptr [edi + 47h], al
        mov    dword ptr [edi + 60h], 0ffffffffh
        push   edi
        mov    edi, dword ptr [edi + 40h]
        test   dword ptr [edi + 4], 2
        pop    edi
        je     SHORT loc_1_526E
loc_1_526E:
        push   eax
        mov    eax, dword ptr [edi + 70h]
        mov    dword ptr [edi + 5ch], eax
        pop    eax
        test   dword ptr [edi + 34h], 20h
        sti
        jne    SHORT loc_1_5281
        ret
loc_1_5281:
        mov    ebx, edi
        and    dword ptr [edi + 34h], 0ffffffdfh
        mov    byte ptr [edi + 38h], 4
        call   sub_1_6104
        clc
        ret
sub_1_5292:
        test   dword ptr [edi + 3ch], 2
        je     SHORT loc_1_52D8
        mov    byte ptr [edi + 38h], 2
        or     dword ptr [edi + 34h], 10h
        mov    ebx, edi
        call   sub_1_6255
        mov    al, 53h
        mov    ecx, 3
        mov    dx, word ptr [edi + 78h]
        call   sub_1_4EB4
        jb     SHORT loc_1_52D7
        and    dword ptr [edi + 3ch], 0fffffffdh
        push   edi
        mov    edi, dword ptr [edi + 40h]
        test   dword ptr [edi + 4], 2
        pop    edi
        je     SHORT loc_1_52CE
loc_1_52CE:
        push   eax
        mov    eax, dword ptr [edi + 70h]
        mov    dword ptr [edi + 5ch], eax
        pop    eax
        clc
loc_1_52D7:
        ret
loc_1_52D8:
        stc
        ret
sub_1_52DA:
        test   dword ptr [edi + 3ch], 4
        je     SHORT loc_1_5319
        mov    al, 73h
        mov    ecx, 3
        mov    dx, word ptr [edi + 78h]
        or     dh, 1
        call   sub_1_4EB4
        jb     SHORT loc_1_5303
        and    dword ptr [edi + 3ch], 0fffffffbh
        cmp    byte ptr [edi + 38h], 2
        je     SHORT loc_1_5304
        clc
loc_1_5303:
        ret
loc_1_5304:
        test   dword ptr [edi + 34h], 10h
        jne    SHORT loc_1_5303
        mov    byte ptr [edi + 38h], 0
        push   edi
        call   sub_1_6435
        clc
        ret
loc_1_5319:
        stc
        ret
sub_1_531B:
        test   dword ptr [edi + 3ch], 8
        je     SHORT loc_1_535C
        cmp    byte ptr [edi + 38h], 4
        je     SHORT loc_1_5356
        mov    al, 7fh
        mov    ecx, 3
        mov    dx, word ptr [edi + 78h]
        call   sub_1_4EB4
        jb     SHORT loc_1_535B
        mov    byte ptr [edi + 38h], 1
        push   edi
        mov    edi, dword ptr [edi + 40h]
        test   dword ptr [edi + 4], 2
        pop    edi
        je     SHORT loc_1_534E
loc_1_534E:
        push   eax
        mov    eax, dword ptr [edi + 70h]
        mov    dword ptr [edi + 5ch], eax
        pop    eax
loc_1_5356:
        and    dword ptr [edi + 3ch], 0fffffff7h
        clc
loc_1_535B:
        ret
loc_1_535C:
        stc
        ret
sub_1_535E:
        test   dword ptr [edi + 3ch], 20h
        je     SHORT loc_1_53AC
        test   dword ptr [edi + 34h], 4
        je     SHORT loc_1_53A8
        mov    al, 9
        mov    ah, byte ptr [edi + 45h]
        add    ah, ah
        test   dword ptr [edi + 3ch], 10h
        je     SHORT loc_1_5383
        or     ah, 1
loc_1_5383:
        mov    ecx, 4
        mov    dx, word ptr [edi + 78h]
        or     dh, 1
        call   sub_1_4EB4
        jb     SHORT loc_1_53A7
        and    dword ptr [edi + 3ch], 0ffffff4fh
        mov    ebx, dword ptr [edi + 40h]
        inc    dword ptr [ebx + 0e0h]
        clc
loc_1_53A7:
        ret
loc_1_53A8:
        and    dword ptr [edi + 3ch], 0ffffffdfh
loc_1_53AC:
        stc
        ret
sub_1_53AE:
        test   dword ptr [edi + 3ch], 40h
        jne    SHORT loc_1_53B9
        stc
        ret
loc_1_53B9:
        push   esi
        lea    esi, [edi + 8ch]
        movzx  eax, byte ptr [edi + 0b6h]
        add    esi, eax
        mov    al, 87h
        test   byte ptr [edi + 80h], 80h
        je     SHORT loc_1_53D6
        or     al, 10h
loc_1_53D6:
        mov    bx, word ptr [edi + 7ch]
        test   byte ptr [edi + 80h], 40h
        je     SHORT loc_1_53E5
        xor    bh, bh
loc_1_53E5:
        mov    word ptr [esi + 3], bx
        mov    ah, bl
        mov    dx, word ptr [edi + 78h]
        or     dh, 1
        mov    bx, word ptr [edi + 7eh]
        mov    word ptr [esi + 5], bx
        mov    bl, byte ptr [edi + 80h]
        and    bl, 1fh
        mov    byte ptr [esi + 7], bl
        mov    ecx, 8
        call   sub_1_4EB4
        jb     SHORT loc_1_5426
        mov    byte ptr [edi + 38h], 3
        and    dword ptr [edi + 3ch], 0ffffffbfh
        mov    ebx, dword ptr [edi + 40h]
        inc    dword ptr [ebx + 0d8h]
        pop    esi
        clc
        ret
loc_1_5426:
        pop    esi
        stc
        ret
sub_1_5429:
        push   esi
        push   edi
        mov    esi, dword ptr [edi + 40h]
loc_1_542E:
        test   dword ptr [esi], 90h
        jne    SHORT loc_1_5478
        call   sub_1_535E
        jae    SHORT loc_1_546C
        call   sub_1_5121
        jae    SHORT loc_1_546C
        call   sub_1_520A
        jae    SHORT loc_1_546C
        call   sub_1_51A4
        jae    SHORT loc_1_546C
        call   sub_1_5292
        jae    SHORT loc_1_546C
        call   sub_1_52DA
        jae    SHORT loc_1_546C
        call   sub_1_531B
        jae    SHORT loc_1_546C
        call   sub_1_53AE
loc_1_546C:
        mov    esi, dword ptr [edi + 40h]
        test   dword ptr [edi + 3ch], 7fffh
        jne    SHORT loc_1_542E
loc_1_5478:
        pop    edi
        pop    esi
        ret
loc_1_547B:
        and    dword ptr [edi], 0fffffeffh
        pop    esi
        clc
        ret
sub_1_5484:
        mov    edi, esi
        push   esi
loc_1_5487:
        cli
        lea    esi, [edi + 0bch]
        call   sub_1_28E3
        sti
        je     SHORT loc_1_547B
        cli
        dec    byte ptr [edi + 30h]
        js     SHORT loc_1_54F6
        sti
        call   sub_1_0517
        cmp    eax, 2
        ja     SHORT loc_1_54FD
        je     SHORT loc_1_5487
        cli
        or     word ptr [esi + 10h], 40h
        test   word ptr [esi + 10h], 400h
        sti
        jne    SHORT loc_1_5487
        test   word ptr [esi + 10h], 1000h
        je     SHORT loc_1_54CA
        db  081h, 025h, 0ECh, 06Eh, 000h, 000h, 0FFh, 0EFh, 0FFh, 0FFh
loc_1_54CA:
        test   word ptr [esi + 10h], 8
        je     SHORT loc_1_54E5
        test   eax, eax
        je     SHORT loc_1_54D8
        mov    al, 21h
loc_1_54D8:
        push   esi
        mov    esi, dword ptr [esi + 30h]
        mov    byte ptr [esi + 1], al
        call   sub_1_6BE9
        pop    esi
loc_1_54E5:
        cli
        push   edi
        db  08Dh, 03Dh, 070h, 06Fh, 000h, 000h
        call   sub_1_2868
        pop    edi
        sti
        jmp    SHORT loc_1_5487
loc_1_54F6:
        inc    byte ptr [edi + 30h]
        or     dword ptr [edi], 10h
        sti
loc_1_54FD:
        cli
        push   edi
        lea    edi, [edi + 0bch]
        call   sub_1_28A7
        pop    edi
        sti
        pop    esi
        stc
        ret
sub_1_550F:
        test   dword ptr [esi], 100h
        je     SHORT loc_1_551C
        call   sub_1_5484
loc_1_551C:
        push   esi
loc_1_551D:
        pop    esi
        test   dword ptr [esi], 90h
        jne    NEAR PTR loc_1_5600
loc_1_552A:
        cli
        push   eax
        mov    edi, dword ptr [esi + 0c4h]
        mov    edi, dword ptr [edi]
        lea    eax, [esi + 0c4h]
        mov    dword ptr [edi + 4], eax
        xchg   dword ptr [esi + 0c4h], edi
        cmp    edi, eax
        lea    edi, [edi - 24h]
        pop    eax
        je     NEAR PTR loc_1_5600
        and    byte ptr [edi + 0b8h], 0feh
        push   esi
        sti
        test   dword ptr [edi + 3ch], 7fffh
        je     SHORT loc_1_5566
        call   sub_1_5429
loc_1_5566:
        call   sub_1_4F81
        jb     SHORT loc_1_551D
        mov    dword ptr [esi + 0ch], edi
        sti
        call   sub_1_0517
        pop    esi
        cli
        test   byte ptr [edi + 0b8h], 1
        jne    SHORT loc_1_55A9
        or     byte ptr [edi + 0b8h], 1
        push   edi
        push   eax
        lea    eax, [esi + 0c4h]
        lea    edi, [edi + 24h]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [esi + 0c8h]
        mov    dword ptr [edi + 4], eax
        xchg   edi, eax
        mov    dword ptr [edi], eax
        mov    dword ptr [esi + 0c8h], eax
        pop    eax
        pop    edi
loc_1_55A9:
        sti
        db  08Ah, 00Dh, 020h, 06Fh, 000h, 000h
        mov    byte ptr [edi + 0b9h], cl
        push   edi
        mov    edi, dword ptr [edi + 40h]
        test   dword ptr [edi + 4], 2
        pop    edi
        je     SHORT loc_1_55C4
loc_1_55C4:
        push   eax
        mov    eax, dword ptr [edi + 70h]
        mov    dword ptr [edi + 5ch], eax
        pop    eax
        cmp    eax, 2
        jbe    NEAR PTR loc_1_552A
        cli
        cmp    dword ptr [edi + 14h], edx
        jne    SHORT loc_1_55FC
        mov    eax, dword ptr [edx + 4]
        lea    ecx, [edi + 10h]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        lea    ecx, [edi + 8]
        mov    dword ptr [edx + 4], ecx
        mov    eax, dword ptr [ecx]
        mov    dword ptr [ecx], edx
        mov    dword ptr [edx], eax
        mov    dword ptr [eax + 4], edx
        dec    byte ptr [edi + 44h]
        and    byte ptr [edi + 44h], 7fh
loc_1_55FC:
        and    dword ptr [edi + 34h], 0fffffff7h
loc_1_5600:
        sti
        ret
sub_1_5602:
        push   0
loc_1_5604:
        mov    eax, dword ptr [esp]
        cmp    al, 8
        jae    SHORT loc_1_5620
        db  08Bh, 034h, 085h, 040h, 06Fh, 000h, 000h
        test   esi, esi
        je     SHORT loc_1_561B
        call   sub_1_550F
loc_1_561B:
        inc    byte ptr [esp]
        jmp    SHORT loc_1_5604
loc_1_5620:
        pop    eax
        ret
        db  000h, 000h
sub_1_5624:
        push   ebx
        mov    ebx, edi
        cmp    byte ptr [ebx + 25h], 0
        je     SHORT loc_1_5636
        mov    byte ptr [ebx + 25h], 4
        call   sub_1_3BB8
loc_1_5636:
        pop    ebx
        ret
sub_1_5638:
        db  083h, 03Dh, 0FFh, 06Eh, 000h, 000h, 000h
        jne    SHORT loc_1_5642
        ret
loc_1_5642:
        mov    ecx, 0ffffffffh
        cli
loc_1_5648:
        inc    ecx
        cmp    ecx, 0fdh
        ja     SHORT loc_1_568B
        db  08Bh, 01Ch, 08Dh, 0C0h, 077h, 000h, 000h
        test   ebx, ebx
        je     SHORT loc_1_5648
        cmp    byte ptr [ebx + 25h], 5
        jne    SHORT loc_1_5648
        test   word ptr [ebx + 6ah], 4
        je     SHORT loc_1_5648
        bts    word ptr [ebx + 6ah], 3
        jae    SHORT loc_1_5648
        and    word ptr [ebx + 6ah], 0fff3h
        mov    ax, word ptr [ebx + 64h]
        call   sub_1_3E4E
        cli
        jb     SHORT loc_1_568D
        db  0FFh, 00Dh, 0FFh, 06Eh, 000h, 000h
        jne    SHORT loc_1_5648
loc_1_568B:
        sti
        ret
loc_1_568D:
        or     word ptr [ebx + 6ah], 0ch
        jmp    SHORT loc_1_568B
loc_1_5694:
        sti
        jmp    NEAR PTR loc_1_572F
sub_1_569A:
        push   esi
        push   edi
        cli
        push   eax
        mov    edi, dword ptr [ebx + 30h]
        mov    edi, dword ptr [edi]
        lea    eax, [ebx + 30h]
        mov    dword ptr [edi + 4], eax
        xchg   dword ptr [ebx + 30h], edi
        cmp    edi, eax
        lea    edi, [edi - 32h]
        pop    eax
        je     SHORT loc_1_5694
        dec    byte ptr [ebx + 8dh]
        btr    word ptr [edi + 3fh], 3
        jae    SHORT loc_1_5704
        db  0C7h, 047h, 03Ah, 078h, 01Ch, 000h, 000h
        push   edi
        push   eax
        db  08Dh, 005h, 080h, 06Fh, 000h, 000h
        lea    edi, [edi + 32h]
        mov    dword ptr [edi], eax
        db  0A1h, 084h, 06Fh, 000h, 000h
        mov    dword ptr [edi + 4], eax
        xchg   edi, eax
        mov    dword ptr [edi], eax
        db  0A3h, 084h, 06Fh, 000h, 000h
        pop    eax
        pop    edi
        db  081h, 00Dh, 0ECh, 06Eh, 000h, 000h, 000h, 004h, 000h, 000h
        mov    ax, word ptr [edi + 0ah]
        mov    word ptr [edi + 8], ax
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [edi + 4], eax
        jmp    SHORT loc_1_570C
        db  08Bh, 0C0h
loc_1_5704:
        xor    eax, eax
        call   sub_1_2913
        nop
loc_1_570C:
        and    word ptr [ebx + 40h], 0f1ffh
        lea    esi, [ebx + 30h]
        cmp    esi, dword ptr [esi]
        jne    SHORT loc_1_5732
        test   word ptr [ebx + 42h], 4000h
        sti
        je     SHORT loc_1_572F
        test   word ptr [ebx + 40h], 40h
        jne    SHORT loc_1_572F
        call   sub_1_3CD2
loc_1_572F:
        pop    edi
        pop    esi
        ret
loc_1_5732:
        mov    edi, dword ptr [esi]
        sub    edi, 32h
        or     word ptr [ebx + 40h], 100h
        call   sub_1_5E2C
        jmp    SHORT loc_1_572F
sub_1_5744:
        push   esi
        push   edi
        cli
        and    word ptr [ebx + 40h], 0efffh
        push   eax
        mov    esi, dword ptr [ebx + 28h]
        mov    esi, dword ptr [esi]
        lea    eax, [ebx + 28h]
        mov    dword ptr [esi + 4], eax
        xchg   dword ptr [ebx + 28h], esi
        cmp    esi, eax
        lea    esi, [esi - 32h]
        pop    eax
        je     SHORT loc_1_57B7
        dec    byte ptr [ebx + 8ch]
        and    word ptr [ebx + 6ah], 0ff9fh
        mov    byte ptr [esi + 1], al
        mov    ax, word ptr [ebx + 84h]
        mov    word ptr [esi + 8], ax
        and    byte ptr [esi + 3fh], 0fbh
        push   esi
        push   eax
        db  08Dh, 005h, 0A8h, 06Fh, 000h, 000h
        lea    esi, [esi + 32h]
        mov    dword ptr [esi], eax
        db  0A1h, 0ACh, 06Fh, 000h, 000h
        mov    dword ptr [esi + 4], eax
        xchg   esi, eax
        mov    dword ptr [esi], eax
        db  0A3h, 0ACh, 06Fh, 000h, 000h
        pop    eax
        pop    esi
        mov    edi, dword ptr [ebx + 94h]
        mov    al, byte ptr [ebx + 68h]
        mov    ah, byte ptr [edi + 2]
        mov    word ptr [esi + 2], ax
        db  083h, 00Dh, 0ECh, 06Eh, 000h, 000h, 040h
loc_1_57B7:
        sti
        pop    edi
        pop    esi
        ret
sub_1_57BB:
        push   ebx
        mov    ebx, dword ptr [esp + 8]
        cli
        test   byte ptr [ebx + 24h], 8
        jne    SHORT loc_1_57DD
        push   esi
        db  08Dh, 035h, 0A0h, 06Fh, 000h, 000h
        push   ebx
        lea    ebx, [ebx + 10h]
        call   sub_1_2892
        pop    ebx
        pop    esi
        or     byte ptr [ebx + 24h], 8
loc_1_57DD:
        db  083h, 00Dh, 0ECh, 06Eh, 000h, 000h, 020h
        sti
        pop    ebx
        ret    4
sub_1_57E9:
        sub    ecx, ecx
        mov    dword ptr [esi + 30h], ebx
        or     word ptr [esi + 10h], 4
        mov    byte ptr [esi + 34h], 15h
        mov    word ptr [esi + 1eh], 1
        mov    ax, word ptr [ebx + 70h]
        cmp    ax, word ptr [ebx + 6eh]
        jb     SHORT loc_1_580B
        mov    ax, word ptr [ebx + 6eh]
loc_1_580B:
        sub    word ptr [ebx + 70h], ax
        mov    edx, dword ptr [ebx + 74h]
        mov    dword ptr [esi + 24h], edx
        mov    byte ptr [esi + 20h], 0
        movzx  eax, ax
        add    word ptr [esi + 3ch], ax
        mov    word ptr [esi + 22h], ax
        add    dword ptr [ebx + 74h], eax
        cmp    word ptr [ebx + 70h], 0
        je     SHORT loc_1_584B
loc_1_582E:
        cmp    word ptr [ebx + 70h], 0
        je     SHORT loc_1_5836
        ret
loc_1_5836:
        mov    byte ptr [esi + 34h], 16h
        inc    ecx
        test   word ptr [ebx + 42h], 200h
        jne    SHORT loc_1_5844
        ret
loc_1_5844:
        or     word ptr [esi + 10h], 200h
        ret
loc_1_584B:
        test   word ptr [ebx + 40h], 800h
        je     SHORT loc_1_582E
        push   esi
        mov    esi, dword ptr [ebx + 30h]
        sub    esi, 32h
        mov    dx, word ptr [esi + 0ah]
        mov    word ptr [ebx + 70h], dx
        mov    edx, dword ptr [esi + 0ch]
        mov    dword ptr [ebx + 74h], edx
        pop    esi
        and    word ptr [ebx + 40h], 0f7ffh
        mov    dx, word ptr [ebx + 6eh]
        cmp    word ptr [esi + 22h], dx
        je     SHORT loc_1_582E
        sub    dx, word ptr [esi + 22h]
        movzx  edx, dx
        movzx  eax, word ptr [ebx + 70h]
        cmp    eax, edx
        jb     SHORT loc_1_588A
        mov    eax, edx
loc_1_588A:
        sub    word ptr [ebx + 70h], ax
        mov    word ptr [esi + 1eh], 2
        mov    edx, dword ptr [ebx + 74h]
        mov    dword ptr [esi + 2ch], edx
        mov    byte ptr [esi + 28h], 0
        add    word ptr [esi + 3ch], ax
        mov    word ptr [esi + 2ah], ax
        add    dword ptr [ebx + 74h], eax
        jmp    SHORT loc_1_582E
loc_1_58AB:
        cli
        test   byte ptr [ebx + 24h], 8
        jne    SHORT loc_1_58C8
        push   esi
        db  08Dh, 035h, 0A0h, 06Fh, 000h, 000h
        push   ebx
        lea    ebx, [ebx + 10h]
        call   sub_1_28CF
        pop    ebx
        pop    esi
        or     byte ptr [ebx + 24h], 8
loc_1_58C8:
        sti
loc_1_58C9:
        inc    byte ptr [ebx + 6ch]
        push   dword ptr [ebx + 38h]
        call   sub_1_4E6C
loc_1_58D4:
        ret
sub_1_58D5:
        test   word ptr [ebx + 40h], 100h
        je     SHORT loc_1_58D4
        test   word ptr [ebx + 40h], 200h
        jne    SHORT loc_1_58D4
        test   word ptr [ebx + 40h], 2
        jne    SHORT loc_1_58D4
        mov    edi, dword ptr [ebx + 38h]
loc_1_58F0:
        dec    byte ptr [ebx + 6ch]
        js     SHORT loc_1_58C9
        call   sub_1_3821
        jae    SHORT loc_1_58AB
        sub    eax, eax
        mov    word ptr [esi + 36h], ax
        mov    word ptr [esi + 38h], ax
        mov    word ptr [esi + 3ah], cs
        db  080h, 03Dh, 032h, 06Fh, 000h, 000h, 000h
        je     SHORT loc_1_591D
        test   word ptr [ebx + 6ah], 40h
        jne    SHORT loc_1_591D
        or     al, 4
loc_1_591D:
        test   word ptr [ebx + 42h], 200h
        je     SHORT loc_1_5929
        or     al, 2
        and    al, 0fbh
loc_1_5929:
        cli
        test   word ptr [ebx + 6ah], 4
        je     SHORT loc_1_594D
        or     al, 8
        db  0FFh, 00Dh, 0FFh, 06Eh, 000h, 000h
        and    word ptr [ebx + 6ah], 0fff3h
        shl    eax, 10h
        mov    ax, word ptr [ebx + 64h]
        mov    word ptr [esi + 38h], ax
        shr    eax, 10h
loc_1_594D:
        sti
        mov    byte ptr [esi + 35h], al
        test   word ptr [ebx + 42h], 1
        je     SHORT loc_1_5964
        and    word ptr [ebx + 42h], 0fffeh
        mov    word ptr [esi + 36h], 1
loc_1_5964:
        call   sub_1_57E9
        cli
        push   edi
        lea    edi, [edi + 8]
        call   sub_1_2868
        pop    edi
        sti
        or     ecx, ecx
        je     NEAR PTR loc_1_58F0
        cli
        or     word ptr [ebx + 40h], 200h
        and    word ptr [ebx + 40h], 0feffh
        or     word ptr [ebx + 6ah], 40h
        sti
        push   dword ptr [ebx + 38h]
        call   sub_1_4E6C
        ret
sub_1_5999:
        push   ecx
        push   eax
        push   ebx
        push   esi
        cli
        mov    byte ptr [edi + 25h], 0
        test   byte ptr [edi + 24h], 84h
        je     SHORT loc_1_59BD
        push   esi
        push   edi
        lea    esi, [edi + 8]
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
        and    byte ptr [edi + 24h], 7bh
loc_1_59BD:
        test   word ptr [edi + 40h], 2000h
        jne    NEAR PTR loc_1_5B81
        xor    ecx, ecx
        test   word ptr [edi + 6ah], 4
        je     SHORT loc_1_59DE
        and    word ptr [edi + 6ah], 0fff3h
        db  0FFh, 00Dh, 0FFh, 06Eh, 000h, 000h
loc_1_59DE:
        test   byte ptr [edi + 24h], 2
        je     SHORT loc_1_5A1F
        and    byte ptr [edi + 24h], 0fdh
        push   esi
        push   edi
        lea    esi, [edi]
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
        test   word ptr [edi + 42h], 4002h
        je     SHORT loc_1_5A1F
        test   word ptr [edi + 42h], 4000h
        je     SHORT loc_1_5A09
        inc    ecx
loc_1_5A09:
        mov    ebx, dword ptr [edi + 38h]
        lea    edx, [ebx]
        cmp    edx, dword ptr [ebx]
        jne    SHORT loc_1_5A1F
        mov    dword ptr [ebx + 3ch], 2
        push   ebx
        call   sub_1_4E6C
loc_1_5A1F:
        and    word ptr [edi + 42h], 0fffdh
        sti
        mov    dl, 34h
        db  0F7h, 005h, 0ECh, 06Eh, 000h, 000h, 001h, 000h, 000h, 000h
        je     SHORT loc_1_5A3F
        mov    dl, 0ah
        test   word ptr [edi + 40h], 40h
        je     SHORT loc_1_5A3F
        mov    dl, 18h
loc_1_5A3F:
        cli
        test   byte ptr [edi + 24h], 8
        je     SHORT loc_1_5A5B
        and    byte ptr [edi + 24h], 0f7h
        push   esi
        push   edi
        lea    esi, [edi + 10h]
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
loc_1_5A5B:
        test   byte ptr [edi + 24h], 20h
        je     SHORT loc_1_5A77
        and    byte ptr [edi + 24h], 0dfh
        push   esi
        push   edi
        lea    esi, [edi + 18h]
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
        sti
loc_1_5A77:
        mov    esi, dword ptr [edi + 94h]
        mov    ah, byte ptr [esi + 2]
        mov    al, byte ptr [edi + 68h]
loc_1_5A83:
        cli
        lea    esi, [edi + 28h]
        call   sub_1_28E3
        lea    esi, [esi - 32h]
        sti
        je     SHORT loc_1_5AAF
        inc    ecx
        db  0FEh, 005h, 005h, 06Fh, 000h, 000h
        mov    word ptr [esi + 2], ax
        test   word ptr [edi + 6ah], 2
        jne    SHORT loc_1_5A83
        mov    byte ptr [esi + 1], dl
        call   sub_1_6BE9
        jmp    SHORT loc_1_5A83
loc_1_5AAF:
        test   word ptr [edi + 6ah], 2
        jne    SHORT loc_1_5B02
        mov    esi, dword ptr [edi + 94h]
        cli
        lea    ebx, [esi + 34h]
        call   sub_1_28F3
        lea    ebx, [ebx - 32h]
        je     SHORT loc_1_5AE8
        dec    byte ptr [esi + 19h]
loc_1_5ACE:
        sti
        mov    al, byte ptr [edi + 68h]
        mov    esi, ebx
        mov    byte ptr [esi + 2], al
        mov    byte ptr [esi + 1], dl
        inc    ecx
        db  0FEh, 005h, 005h, 06Fh, 000h, 000h
        call   sub_1_6BE9
        jmp    SHORT loc_1_5B02
loc_1_5AE8:
        mov    esi, dword ptr [edi + 94h]
        mov    esi, dword ptr [esi + 4]
        lea    ebx, [esi + 9ch]
        call   sub_1_28F3
        lea    ebx, [ebx - 32h]
        jne    SHORT loc_1_5ACE
        sti
loc_1_5B02:
        cli
        lea    esi, [edi + 30h]
        call   sub_1_28E3
        lea    esi, [esi - 32h]
        sti
        je     SHORT loc_1_5B2A
        inc    ecx
        db  0FEh, 005h, 005h, 06Fh, 000h, 000h
        test   word ptr [edi + 6ah], 2
        jne    SHORT loc_1_5B02
        mov    byte ptr [esi + 1], dl
        call   sub_1_6BE9
        jmp    SHORT loc_1_5B02
loc_1_5B2A:
        cli
        test   word ptr [edi + 40h], 80h
        je     SHORT loc_1_5B5F
        and    word ptr [edi + 40h], 0ff7fh
        test   word ptr [edi + 6ah], 2
        jne    SHORT loc_1_5B5F
        mov    esi, dword ptr [edi + 88h]
        cmp    dl, 0ah
        jne    SHORT loc_1_5B56
        mov    al, byte ptr [esi]
        and    al, 7fh
        cmp    al, 12h
        jne    SHORT loc_1_5B56
        sub    dl, dl
loc_1_5B56:
        mov    byte ptr [esi + 1], dl
        inc    ecx
        call   sub_1_6BE9
loc_1_5B5F:
        mov    byte ptr [edi + 24h], 0
        test   word ptr [edi + 6ah], 2
        jne    SHORT loc_1_5B81
        mov    esi, dword ptr [edi + 94h]
        dec    byte ptr [esi + 3]
        cmp    byte ptr [esi + 1], 3
        jne    SHORT loc_1_5B7F
        call   sub_1_250A
loc_1_5B7F:
        jecxz  loc_1_5BD0
loc_1_5B81:
        cli
        xor    eax, eax
        mov    word ptr [edi + 40h], ax
        mov    word ptr [edi + 42h], ax
        mov    word ptr [edi + 6ah], ax
        movzx  ebx, byte ptr [edi + 68h]
        dec    ebx
        db  0C7h, 004h, 09Dh, 0C0h, 077h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0B8h, 0BCh, 06Fh, 000h, 000h
        sub    eax, 20h
loc_1_5BA8:
        cmp    dword ptr [eax + 20h], 0
        je     SHORT loc_1_5BC0
        cmp    dword ptr [eax + 20h], edi
        jne    SHORT loc_1_5BBB
        mov    ecx, dword ptr [edi + 20h]
        mov    dword ptr [eax + 20h], ecx
        jmp    SHORT loc_1_5BC0
loc_1_5BBB:
        mov    eax, dword ptr [eax + 20h]
        jmp    SHORT loc_1_5BA8
loc_1_5BC0:
        push   eax
        mov    eax, edi
        call   sub_1_227A
        pop    eax
        sti
        clc
loc_1_5BCB:
        pop    esi
        pop    ebx
        pop    eax
        pop    ecx
        ret
loc_1_5BD0:
        test   word ptr [edi + 42h], 4000h
        jne    SHORT loc_1_5B81
        mov    byte ptr [edi + 25h], 0ah
        mov    ebx, dword ptr [esi + 4]
        mov    dword ptr [edi + 3ch], ebx
        cmp    byte ptr [esi + 1], 3
        stc
        je     SHORT loc_1_5BCB
        mov    ebx, edi
        call   sub_1_5C40
        stc
        jmp    SHORT loc_1_5BCB
sub_1_5BF3:
        push   edi
        mov    edi, dword ptr [esp + 8]
        cli
        test   byte ptr [edi + 24h], 84h
        jne    SHORT loc_1_5C1C
        call   sub_1_220C
        jb     SHORT loc_1_5C28
        or     byte ptr [edi + 24h], 4
        push   esi
        db  08Dh, 035h, 090h, 06Fh, 000h, 000h
        push   edi
        lea    edi, [edi + 8]
        call   sub_1_287D
        pop    edi
        pop    esi
loc_1_5C1C:
        db  083h, 00Dh, 0ECh, 06Eh, 000h, 000h, 010h
        sti
        pop    edi
        ret    4
loc_1_5C28:
        or     byte ptr [edi + 24h], 80h
        push   esi
        db  08Dh, 035h, 098h, 06Fh, 000h, 000h
        push   edi
        lea    edi, [edi + 8]
        call   sub_1_287D
        pop    edi
        pop    esi
        jmp    SHORT loc_1_5C1C
sub_1_5C40:
        cli
        test   byte ptr [ebx + 24h], 20h
        jne    SHORT loc_1_5C6B
        push   edi
        mov    edi, dword ptr [ebx + 94h]
        or     byte ptr [ebx + 24h], 20h
        push   esi
        lea    esi, [edi + 3ch]
        push   ebx
        lea    ebx, [ebx + 18h]
        call   sub_1_2892
        pop    ebx
        pop    esi
        mov    edi, dword ptr [edi + 4]
        or     dword ptr [edi], 400h
        pop    edi
loc_1_5C6B:
        sti
        ret
loc_1_5C6D:
        mov    esi, dword ptr [ebx + 94h]
        mov    al, byte ptr [ebx + 68h]
        mov    ah, byte ptr [esi + 2]
        mov    esi, edi
        mov    word ptr [esi + 2], ax
        and    word ptr [ebx + 42h], 0fffbh
        db  0FEh, 005h, 005h, 06Fh, 000h, 000h
        mov    byte ptr [esi + 1], 7
        call   sub_1_6BE9
        test   word ptr [ebx + 40h], 8000h
        je     SHORT loc_1_5CDB
        jmp    SHORT loc_1_5CF6
loc_1_5C9D:
        clc
        ret
loc_1_5C9F:
        cli
        lea    edi, [ebx + 28h]
        call   sub_1_2903
        lea    edi, [edi - 32h]
        jne    SHORT loc_1_5CB6
        and    word ptr [ebx + 42h], 7fffh
        sti
        jmp    SHORT loc_1_5CFE
loc_1_5CB6:
        or     word ptr [ebx + 6ah], 20h
        sti
        dec    byte ptr [ebx + 8ch]
        jmp    SHORT loc_1_5D1C
loc_1_5CC4:
        push   esi
        mov    esi, dword ptr [esi + 4]
        cli
        lea    edi, [esi + 9ch]
        call   sub_1_2903
        lea    edi, [edi - 32h]
        sti
        pop    esi
        jne    SHORT loc_1_5D1C
loc_1_5CDB:
        test   word ptr [ebx + 40h], 8000h
        jne    SHORT loc_1_5CE8
        call   sub_1_5C40
loc_1_5CE8:
        pop    edi
        pop    esi
        stc
        ret
sub_1_5CEC:
        test   word ptr [ebx + 40h], 1000h
        jne    SHORT loc_1_5C9D
        push   esi
        push   edi
loc_1_5CF6:
        test   word ptr [ebx + 42h], 8000h
        jne    SHORT loc_1_5C9F
loc_1_5CFE:
        mov    esi, dword ptr [ebx + 94h]
        cli
        lea    eax, [esi + 34h]
        mov    edi, dword ptr [eax]
        cmp    edi, eax
        je     SHORT loc_1_5CC4
        mov    ecx, dword ptr [edi]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        sub    edi, 32h
        sti
        dec    byte ptr [esi + 19h]
loc_1_5D1C:
        test   word ptr [ebx + 42h], 4
        jne    NEAR PTR loc_1_5C6D
        cli
        lea    eax, [ebx + 28h]
        lea    ecx, [edi + 32h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    dword ptr [ebx + 28h], ecx
        mov    dword ptr [eax + 4], ecx
        sti
        inc    byte ptr [ebx + 8ch]
        mov    edx, dword ptr [edi + 4]
        mov    cx, word ptr [edi + 8]
        mov    word ptr [ebx + 86h], cx
        mov    word ptr [ebx + 84h], 0
        mov    dword ptr [ebx + 80h], edx
        or     byte ptr [edi + 3fh], 4
        or     word ptr [ebx + 40h], 1000h
        cli
        test   word ptr [ebx + 40h], 8000h
        jne    SHORT loc_1_5D78
loc_1_5D73:
        sti
        pop    edi
        pop    esi
        clc
        ret
loc_1_5D78:
        and    word ptr [ebx + 40h], 7fffh
        sti
        mov    ax, word ptr [ebx + 84h]
        call   sub_1_3D51
        jmp    SHORT loc_1_5D73
sub_1_5D8D:
        cli
        mov    edi, ebx
        movzx  ebx, byte ptr [edi + 2]
        dec    ebx
        cmp    ebx, 0fdh
        ja     SHORT loc_1_5DB8
        db  08Bh, 01Ch, 09Dh, 0C0h, 077h, 000h, 000h
        or     ebx, ebx
        je     SHORT loc_1_5DB8
        test   word ptr [ebx + 40h], 60h
        jne    SHORT loc_1_5DC0
        cmp    byte ptr [ebx + 25h], 5
        jne    SHORT loc_1_5DB8
        clc
        ret
loc_1_5DB8:
        mov    al, 8
loc_1_5DBA:
        sti
        mov    byte ptr [edi + 1], al
        stc
        ret
loc_1_5DC0:
        mov    al, 18h
        test   word ptr [ebx + 40h], 40h
        jne    SHORT loc_1_5DCC
        mov    al, 0ah
loc_1_5DCC:
        test   byte ptr [ebx + 24h], 80h
        jne    SHORT loc_1_5DBA
        push   eax
        test   byte ptr [ebx + 24h], 4
        je     SHORT loc_1_5DFB
        and    byte ptr [ebx + 24h], 0fbh
        push   esi
        push   edi
        lea    esi, [ebx + 8]
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
        push   edi
        mov    edi, ebx
        sti
        call   sub_1_5999
        cli
        pop    edi
        jae    SHORT loc_1_5E29
loc_1_5DFB:
        test   byte ptr [ebx + 24h], 20h
        je     SHORT loc_1_5E16
        push   esi
        push   edi
        lea    esi, [ebx + 18h]
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
        and    byte ptr [ebx + 24h], 0dfh
loc_1_5E16:
        or     word ptr [ebx + 42h], 4000h
        test   word ptr [ebx + 42h], 2
        jne    SHORT loc_1_5E29
        call   sub_1_235E
loc_1_5E29:
        pop    eax
        jmp    SHORT loc_1_5DBA
sub_1_5E2C:
        cli
        or     word ptr [ebx + 40h], 100h
        movzx  eax, word ptr [ebx + 40h]
        test   eax, 400h
        je     SHORT loc_1_5E40
        jmp    SHORT loc_1_5E80
loc_1_5E40:
        and    eax, 0fffff5ffh
        or     eax, 400h
        test   byte ptr [edi + 3fh], 2
        je     SHORT loc_1_5E55
        or     eax, 800h
loc_1_5E55:
        mov    word ptr [ebx + 40h], ax
        and    word ptr [ebx + 42h], 0fdffh
        test   byte ptr [edi + 3fh], 1
        je     SHORT loc_1_5E6B
        or     word ptr [ebx + 42h], 200h
loc_1_5E6B:
        mov    ax, word ptr [edi + 8]
        mov    word ptr [ebx + 70h], ax
        mov    word ptr [ebx + 72h], 0
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ebx + 74h], eax
        sti
loc_1_5E80:
        push   ebx
        call   sub_1_57BB
        push   dword ptr [ebx + 38h]
        call   sub_1_4E6C
        ret
sub_1_5E8F:
        push   esi
        push   edi
        push   ecx
        cli
        cli
        lea    edi, [ebx + 30h]
        cmp    edi, dword ptr [edi]
        sti
        je     SHORT loc_1_5EAD
        mov    edi, dword ptr [edi]
        sub    edi, 32h
        or     word ptr [ebx + 40h], 100h
        test   word ptr [ebx + 40h], 400h
loc_1_5EAD:
        je     SHORT loc_1_5EED
        and    word ptr [ebx + 40h], 0f5ffh
        or     word ptr [ebx + 40h], 400h
        add    cx, word ptr [ebx + 72h]
        mov    word ptr [ebx + 72h], cx
        cmp    cx, word ptr [edi + 8]
        jae    SHORT loc_1_5EF2
        mov    edx, dword ptr [edi + 4]
        add    edx, ecx
        mov    dword ptr [ebx + 74h], edx
        mov    ax, word ptr [edi + 8]
        sub    ax, cx
        mov    word ptr [ebx + 70h], ax
        test   byte ptr [edi + 3fh], 2
        je     SHORT loc_1_5EE8
        or     word ptr [ebx + 40h], 800h
loc_1_5EE8:
        or     word ptr [ebx + 42h], 1
loc_1_5EED:
        sti
        pop    ecx
        pop    edi
        pop    esi
        ret
loc_1_5EF2:
        test   byte ptr [edi + 3fh], 2
        jne    SHORT loc_1_5F08
        sti
        jecxz  loc_1_5EE8
        or     word ptr [ebx + 40h], 60h
        call   sub_1_3CD2
        stc
        jmp    SHORT loc_1_5EED
loc_1_5F08:
        sub    cx, word ptr [edi + 8]
        mov    edx, dword ptr [edi + 0ch]
        add    edx, ecx
        mov    dword ptr [ebx + 74h], edx
        mov    ax, word ptr [edi + 0ah]
        sub    ax, cx
        mov    word ptr [ebx + 70h], ax
        and    word ptr [ebx + 40h], 0f7ffh
        or     word ptr [ebx + 42h], 1
        sti
        pop    ecx
        pop    edi
        pop    esi
        ret
loc_1_5F2F:
        movzx  eax, word ptr [edi + 76h]
        sub    eax, ebx
        cmp    ecx, eax
        jae    SHORT loc_1_5F3B
        mov    eax, ecx
loc_1_5F3B:
        mov    ecx, eax
        pushfd
        push   ecx
        push   edi
        push   ecx
        mov    edi, dword ptr [edi + 68h]
        lea    edi, [edi + 2]
        push   edi
        mov    edi, edx
        cld
loc_1_5F4B:
        pop    esi
        pop    eax
        or     eax, eax
        je     SHORT loc_1_5F98
        movzx  ecx, word ptr [esi + 4]
        sub    ecx, ebx
        cmp    ecx, eax
        jb     SHORT loc_1_5F5D
        mov    ecx, eax
loc_1_5F5D:
        sub    eax, ecx
        push   eax
        mov    eax, 6
        add    eax, esi
        push   eax
        mov    eax, dword ptr [esp + 8]
        test   dword ptr [eax], 1
        jne    SHORT loc_1_5F82
        movzx  eax, word ptr [esi]
        movzx  esi, word ptr [esi + 2]
        shl    esi, 4
        add    esi, eax
        jmp    SHORT loc_1_5F84
loc_1_5F82:
        mov    esi, dword ptr [esi]
loc_1_5F84:
        add    esi, ebx
        sub    ebx, ebx
        mov    eax, ecx
        shr    ecx, 2
        rep movsd
        and    eax, 3
        mov    ecx, eax
        rep movsb
        jmp    SHORT loc_1_5F4B
loc_1_5F98:
        pop    eax
        pop    ecx
        popfd
        jmp    SHORT loc_1_5FD9
sub_1_5F9D:
        enter  8, 0
        push   edi
        test   dword ptr [edi], 4
        je     SHORT loc_1_5F2F
        movzx  eax, word ptr [edi + 76h]
        or     eax, eax
        je     SHORT loc_1_5FDC
        cmp    ax, word ptr [edi + 74h]
        ja     SHORT loc_1_5FE2
loc_1_5FB8:
        sub    eax, ebx
        cmp    ecx, eax
        jb     SHORT loc_1_5FC0
        mov    ecx, eax
loc_1_5FC0:
        push   ecx
        mov    esi, dword ptr [edi + 70h]
        mov    edi, edx
        add    esi, ebx
        cld
        shr    ecx, 2
        rep movsd
        mov    ecx, dword ptr [esp]
        and    ecx, 3
        rep movsb
        pop    ecx
        cmp    ecx, eax
loc_1_5FD9:
        pop    edi
        leave
        ret
loc_1_5FDC:
        movzx  eax, word ptr [edi + 74h]
        jmp    SHORT loc_1_5FB8
loc_1_5FE2:
        sub    eax, ebx
        cmp    ecx, eax
        jb     SHORT loc_1_5FEA
        mov    ecx, eax
loc_1_5FEA:
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [edi + 78h]
        push   ecx
        db  02Bh, 01Dh, 080h, 06Eh, 000h, 000h
        push   ebx
        push   dword ptr [edi + 80h]
        push   dword ptr [edi + 10h]
        lea    eax, [ebp - 4]
        push   eax
        sub    esp, 8
        push   ecx
        push   edx
        push   dword ptr [edi + 12ch]
        mov    eax, esp
        add    eax, 0ch
        push   eax
        add    eax, 4
        push   eax
        call   sub_1_6CB2
        pop    eax
        pop    edx
        cmp    edx, 0
        jne    SHORT loc_1_6067
        push   eax
        mov    dword ptr [edi + 7ch], eax
        mov    eax, dword ptr [edi + 78h]
        mov    dword ptr [eax + 3ch], 0
        push   eax
        push   eax
        call   sub_1_6C94
        call   sub_1_6CD6
        call   sub_1_6C22
        cmp    dword ptr [ebp - 4], 103h
        je     SHORT loc_1_606D
loc_1_6050:
        push   dword ptr [edi + 7ch]
        call   sub_1_6CBE
        pop    eax
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_6067
        mov    ecx, dword ptr [ebp - 8]
        cmp    ecx, eax
        pop    edi
        leave
        ret
loc_1_6067:
        sub    ecx, ecx
        stc
        pop    edi
        leave
        ret
loc_1_606D:
        mov    eax, dword ptr [edi + 130h]
        mov    ecx, 1
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        mov    eax, dword ptr [edi + 2ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [edi + 78h]
        mov    eax, dword ptr [eax + 3ch]
        mov    dword ptr [ebp - 8], eax
        jmp    SHORT loc_1_6050
sub_1_608F:
        push   ecx
        push   esi
        push   edi
        mov    esi, dword ptr [ebx + 38h]
        movzx  edx, byte ptr [esi + 0b6h]
        add    edx, 12h
        mov    edi, dword ptr [esi + 40h]
        mov    eax, dword ptr [ebx + 80h]
        movzx  ecx, word ptr [ebx + 86h]
        push   ebx
        mov    ebx, edx
        mov    edx, eax
        call   sub_1_5F9D
        pop    ebx
        pushfd
        sub    word ptr [ebx + 86h], cx
        add    word ptr [ebx + 84h], cx
        add    dword ptr [ebx + 80h], ecx
        popfd
        pop    edi
        pop    esi
        pop    ecx
        ret
sub_1_60D4:
        push   esi
        push   edx
        push   eax
        call   sub_1_5F9D
        pop    eax
        pop    edx
        pop    esi
        jb     SHORT loc_1_60E4
        sub    al, al
        ret
loc_1_60E4:
        mov    al, 6
        ret
sub_1_60E7:
        push   esi
        push   edi
        push   ecx
        mov    ecx, 4
        lea    edi, [esi + 0ah]
        mov    esi, dword ptr [esp + 10h]
        lea    esi, [esi + 1ch]
        cld
        rep movsd
        pop    ecx
        pop    edi
        pop    esi
        ret    4
        db  000h, 000h
sub_1_6104:
        push   ecx
        push   edi
        push   esi
        push   eax
        cli
        lea    edi, [ebx]
        call   sub_1_2903
        je     SHORT loc_1_613D
        mov    ecx, edi
loc_1_6114:
        cli
        push   esi
        lea    esi, [ebx]
        call   sub_1_287D
        pop    esi
        sti
        push   ecx
        call   sub_1_5624
        pop    ecx
        cli
        lea    edi, [ebx]
        call   sub_1_2903
        je     SHORT loc_1_613D
        cmp    ecx, edi
        jne    SHORT loc_1_6114
        push   esi
        lea    esi, [ebx]
        call   sub_1_28BB
        pop    esi
loc_1_613D:
        sti
        pop    eax
        pop    esi
        pop    edi
        pop    ecx
        ret
sub_1_6143:
        push   esi
        movzx  ecx, al
        shr    cl, 1
loc_1_6149:
        lea    esi, [ebx + 8]
        call   sub_1_28E3
        inc    byte ptr [ebx + 44h]
        and    byte ptr [ebx + 44h], 7fh
        or     word ptr [esi + 10h], 8000h
        push   edi
        db  08Dh, 03Dh, 068h, 06Fh, 000h, 000h
        call   sub_1_2868
        pop    edi
        loop   loc_1_6149
        pop    esi
        ret
loc_1_616F:
        sub    al, ah
        je     SHORT loc_1_61E3
        call   sub_1_6143
        jmp    SHORT loc_1_61E3
sub_1_617A:
        push   eax
        push   ecx
        push   edx
        push   edi
        push   esi
        cli
        mov    ah, byte ptr [ebx + 46h]
        mov    byte ptr [ebx + 46h], al
        shl    eax, 1
        sub    edx, edx
        lea    ecx, [ebx + 10h]
        mov    esi, dword ptr [ebx + 10h]
        cmp    esi, ecx
        je     SHORT loc_1_616F
loc_1_6194:
        cmp    ah, al
        je     SHORT loc_1_61E3
        add    ah, 2
        push   eax
        mov    esi, dword ptr [ebx + 10h]
        mov    esi, dword ptr [esi]
        lea    eax, [ebx + 10h]
        mov    dword ptr [esi + 4], eax
        xchg   dword ptr [ebx + 10h], esi
        cmp    esi, eax
        pop    eax
        je     SHORT loc_1_616F
        test   word ptr [esi + 10h], 200h
        jne    SHORT loc_1_620D
loc_1_61B7:
        or     word ptr [esi + 10h], 8000h
        test   word ptr [esi + 10h], 100h
        jne    SHORT loc_1_6224
loc_1_61C5:
        push   esi
        push   eax
        db  08Dh, 005h, 068h, 06Fh, 000h, 000h
        mov    dword ptr [esi], eax
        db  0A1h, 06Ch, 06Fh, 000h, 000h
        mov    dword ptr [esi + 4], eax
        xchg   esi, eax
        mov    dword ptr [esi], eax
        db  0A3h, 06Ch, 06Fh, 000h, 000h
        pop    eax
        pop    esi
        jmp    SHORT loc_1_6194
loc_1_61E3:
        sti
        or     edx, edx
        jne    SHORT loc_1_622F
loc_1_61E8:
        db  0F7h, 005h, 0ECh, 06Eh, 000h, 000h, 000h, 008h, 000h, 000h
        jne    SHORT loc_1_61FA
loc_1_61F4:
        pop    esi
        pop    edi
        pop    edx
        pop    ecx
        pop    eax
        ret
loc_1_61FA:
        db  083h, 00Dh, 0ECh, 06Eh, 000h, 000h, 020h
        db  081h, 025h, 0ECh, 06Eh, 000h, 000h, 0FFh, 0F7h, 0FFh, 0FFh
        jmp    SHORT loc_1_61F4
loc_1_620D:
        mov    edi, dword ptr [esi + 30h]
        test   byte ptr [edi + 24h], 40h
        jne    SHORT loc_1_621F
        mov    dword ptr [edi + 78h], edx
        mov    edx, edi
        or     byte ptr [edi + 24h], 40h
loc_1_621F:
        inc    byte ptr [edi + 6dh]
        jmp    SHORT loc_1_61B7
loc_1_6224:
        mov    edi, dword ptr [esi + 30h]
        push   edi
        call   sub_1_5BF3
        jmp    SHORT loc_1_61C5
loc_1_622F:
        or     edx, edx
        je     SHORT loc_1_61E8
        push   ebx
        mov    ebx, edx
        and    word ptr [ebx + 42h], 0fdffh
        movzx  ecx, byte ptr [ebx + 6dh]
loc_1_6240:
        call   sub_1_569A
        loop   loc_1_6240
        mov    byte ptr [ebx + 6dh], 0
        and    byte ptr [ebx + 24h], 0bfh
        mov    edx, dword ptr [ebx + 78h]
        pop    ebx
        jmp    SHORT loc_1_622F
sub_1_6255:
        push   esi
        cli
loc_1_6257:
        lea    esi, [ebx + 10h]
        call   sub_1_28E3
        je     SHORT loc_1_6276
        or     word ptr [esi + 10h], 8000h
        push   edi
        db  08Dh, 03Dh, 068h, 06Fh, 000h, 000h
        call   sub_1_2868
        pop    edi
        jmp    SHORT loc_1_6257
loc_1_6276:
        lea    esi, [ebx + 8]
        call   sub_1_28E3
        je     SHORT loc_1_6295
        or     word ptr [esi + 10h], 8000h
        push   edi
        db  08Dh, 03Dh, 068h, 06Fh, 000h, 000h
        call   sub_1_2868
        pop    edi
        jmp    SHORT loc_1_6276
loc_1_6295:
        sti
        pop    esi
        ret
sub_1_6298:
        push   esi
        push   edi
        push   eax
        push   ecx
        mov    esi, eax
        movzx  ecx, byte ptr [esi + 0b6h]
        mov    byte ptr [ebx + 91h], cl
        push   esi
        lea    esi, [esi + 8ch]
        lea    edi, [ebx + 98h]
        cld
        rep movsb
        pop    esi
        mov    ax, 0f0f0h
        stosw
        stosw
        mov    word ptr [edi], 0eh
        mov    word ptr [edi + 2], 0efffh
        sub    eax, eax
        mov    byte ptr [edi + 5], al
        mov    word ptr [edi + 6], ax
        mov    word ptr [edi + 8], ax
        mov    word ptr [edi + 0ah], ax
        mov    ax, word ptr [ebx + 68h]
        xchg   al, ah
        mov    word ptr [edi + 0ch], ax
        add    byte ptr [ebx + 91h], 12h
        pop    ecx
        pop    eax
        pop    edi
        pop    esi
        clc
        ret
loc_1_62F7:
        pop    eax
        stc
        ret
sub_1_62FA:
        db  0A3h, 0D8h, 06Eh, 000h, 000h
        push   eax
        push   ebx
        push   edi
        mov    edi, edx
        call   sub_1_236B
        pop    edi
        pop    eax
        jb     SHORT loc_1_62F7
        add    esp, 4
        xchg   esi, eax
        cli
        or     byte ptr [esi + 24h], 2
        push   edi
        lea    edi, [ebx]
        call   sub_1_2868
        pop    edi
        sti
        push   eax
        db  0A1h, 0D8h, 06Eh, 000h, 000h
        call   sub_1_2959
        pop    eax
        mov    dword ptr [esi + 38h], ebx
        xchg   esi, eax
        xchg   ebx, eax
        call   sub_1_6298
        clc
        ret
sub_1_6338:
        cli
        mov    dword ptr [edi + 3ch], 0
        test   byte ptr [edi + 0b8h], 4
        je     SHORT loc_1_6361
        push   esi
        push   edi
        lea    esi, [edi + 2ch]
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
        and    byte ptr [edi + 0b8h], 0fbh
loc_1_6361:
        db  0B8h, 0B8h, 06Fh, 000h, 000h
        sub    eax, 18h
loc_1_6369:
        cmp    dword ptr [eax + 18h], 0
        je     SHORT loc_1_6381
        cmp    dword ptr [eax + 18h], edi
        jne    SHORT loc_1_637C
        mov    ecx, dword ptr [edi + 18h]
        mov    dword ptr [eax + 18h], ecx
        jmp    SHORT loc_1_6381
loc_1_637C:
        mov    eax, dword ptr [eax + 18h]
        jmp    SHORT loc_1_6369
loc_1_6381:
        mov    dword ptr [edi + 34h], 0
        mov    dword ptr [edi + 40h], 0
        test   byte ptr [edi + 0b8h], 1
        je     SHORT loc_1_63A9
        push   esi
        push   edi
        lea    esi, [edi + 24h]
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
loc_1_63A9:
        test   byte ptr [edi + 0b8h], 2
        je     SHORT loc_1_63C3
        push   esi
        push   edi
        lea    esi, [edi + 1ch]
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
loc_1_63C3:
        sti
        cli
        lea    esi, [edi + 8]
        call   sub_1_28E3
        sti
        je     SHORT loc_1_63E7
        or     word ptr [esi + 10h], 8000h
        cli
        push   edi
        db  08Dh, 03Dh, 068h, 06Fh, 000h, 000h
        call   sub_1_2868
        pop    edi
        sti
        jmp    SHORT loc_1_63C3
loc_1_63E7:
        cli
        lea    esi, [edi + 10h]
        call   sub_1_28E3
        sti
        je     SHORT loc_1_640A
        or     word ptr [esi + 10h], 8000h
        cli
        push   edi
        db  08Dh, 03Dh, 068h, 06Fh, 000h, 000h
        call   sub_1_2868
        pop    edi
        sti
        jmp    SHORT loc_1_63E7
loc_1_640A:
        lea    ebx, [edi]
        call   sub_1_28F3
        je     SHORT loc_1_6424
        and    byte ptr [ebx + 24h], 0fdh
        or     word ptr [ebx + 40h], 40h
        push   ebx
        call   sub_1_5BF3
        jmp    SHORT loc_1_640A
loc_1_6424:
        mov    byte ptr [edi + 0b8h], 0
        push   eax
        mov    eax, edi
        call   sub_1_227A
        pop    eax
        ret
sub_1_6435:
        push   edi
        mov    edi, dword ptr [esp + 8]
        cli
        test   byte ptr [edi + 0b8h], 4
        jne    SHORT loc_1_645D
        or     byte ptr [edi + 0b8h], 4
        push   esi
        db  08Dh, 035h, 088h, 06Fh, 000h, 000h
        push   edi
        lea    edi, [edi + 2ch]
        call   sub_1_287D
        pop    edi
        pop    esi
loc_1_645D:
        db  083h, 00Dh, 0ECh, 06Eh, 000h, 000h, 010h
        sti
        pop    edi
        ret    4
        db  000h, 000h, 000h
loc_1_646C:
        ret
sub_1_646D:
        mov    al, 1
        db  086h, 005h, 006h, 06Fh, 000h, 000h
        or     al, al
        jne    SHORT loc_1_646C
        cli
        db  0BEh, 0A8h, 06Fh, 000h, 000h
        mov    ebx, dword ptr [esi]
        cmp    esi, ebx
        je     SHORT loc_1_64B1
loc_1_6485:
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esi], eax
        mov    dword ptr [eax + 4], esi
        db  0FEh, 005h, 005h, 06Fh, 000h, 000h
        sti
        sub    ebx, 32h
        mov    al, byte ptr [ebx + 1]
        mov    byte ptr [ebx + 31h], al
        test   byte ptr [ebx], 80h
        je     SHORT loc_1_64AA
        mov    ecx, dword ptr [ebx + 2ch]
        jecxz  loc_1_64AA
        pushad
        call   ecx
        popad
loc_1_64AA:
        cli
        mov    ebx, dword ptr [esi]
        cmp    esi, ebx
        jne    SHORT loc_1_6485
loc_1_64B1:
        db  083h, 025h, 0ECh, 06Eh, 000h, 000h, 0BFh
        sti
        db  0C6h, 005h, 006h, 06Fh, 000h, 000h, 000h
        ret
sub_1_64C1:
        cli
        db  08Dh, 03Dh, 088h, 06Fh, 000h, 000h
        call   sub_1_2903
        lea    edi, [edi - 2ch]
        je     SHORT loc_1_6513
        and    byte ptr [edi + 0b8h], 0fbh
        test   dword ptr [edi + 34h], 800h
        jne    NEAR PTR loc_1_6578
        test   byte ptr [edi + 0b8h], 2
        je     SHORT loc_1_6507
        push   esi
        push   edi
        lea    esi, [edi + 1ch]
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
        and    byte ptr [edi + 0b8h], 0fdh
loc_1_6507:
        mov    byte ptr [edi + 38h], 0
        sti
        call   sub_1_6338
loc_1_6511:
        jmp    SHORT sub_1_64C1
loc_1_6513:
        cli
        db  08Dh, 03Dh, 090h, 06Fh, 000h, 000h
        call   sub_1_2903
        lea    edi, [edi - 8]
        je     SHORT loc_1_6530
        and    byte ptr [edi + 24h], 0fbh
        sti
        call   sub_1_5999
        jmp    SHORT loc_1_6513
loc_1_6530:
        sub    ebx, ebx
loc_1_6532:
        db  08Dh, 03Dh, 098h, 06Fh, 000h, 000h
        call   sub_1_2903
        lea    edi, [edi - 8]
        je     SHORT loc_1_655B
        call   sub_1_220C
        jb     SHORT loc_1_6554
        and    byte ptr [edi + 24h], 7fh
        call   sub_1_5999
        jmp    SHORT loc_1_6532
loc_1_6554:
        mov    dword ptr [edi + 8], ebx
        mov    ebx, edi
        jmp    SHORT loc_1_6532
loc_1_655B:
        or     ebx, ebx
        je     SHORT loc_1_659C
        mov    edi, ebx
        mov    ebx, dword ptr [edi + 8]
        push   esi
        db  08Dh, 035h, 098h, 06Fh, 000h, 000h
        push   edi
        lea    edi, [edi + 8]
        call   sub_1_287D
        pop    edi
        pop    esi
        jmp    SHORT loc_1_655B
loc_1_6578:
        mov    byte ptr [edi + 38h], 1
        sti
        push   esi
        mov    esi, edi
        call   sub_1_26A5
        pop    esi
        or     dword ptr [edi + 3ch], 8
        and    dword ptr [edi + 34h], 0fffff7ffh
        push   edi
        call   sub_1_4E6C
        jmp    NEAR PTR loc_1_6511
loc_1_659C:
        sti
        ret
sub_1_659E:
        db  0F7h, 005h, 0ECh, 06Eh, 000h, 000h, 000h, 008h, 000h, 000h
        jne    SHORT loc_1_65D0
        cli
        db  08Dh, 01Dh, 0A0h, 06Fh, 000h, 000h
        call   sub_1_28F3
        lea    ebx, [ebx - 10h]
        je     SHORT loc_1_65CF
        and    byte ptr [ebx + 24h], 0f7h
        sti
        test   word ptr [ebx + 6ah], 1f80h
        jne    SHORT loc_1_65D1
loc_1_65C8:
        call   sub_1_58D5
        jmp    SHORT sub_1_659E
loc_1_65CF:
        sti
loc_1_65D0:
        ret
loc_1_65D1:
        test   word ptr [ebx + 6ah], 400h
        je     SHORT loc_1_65E6
        call   sub_1_3BB8
        jb     SHORT loc_1_65D0
        and    word ptr [ebx + 6ah], 0fbffh
loc_1_65E6:
        test   word ptr [ebx + 6ah], 800h
        je     SHORT loc_1_65FB
        call   sub_1_3C42
        jb     SHORT loc_1_65D0
        and    word ptr [ebx + 6ah], 0f7ffh
loc_1_65FB:
        test   word ptr [ebx + 6ah], 80h
        je     SHORT loc_1_6614
        mov    ax, word ptr [ebx + 7ch]
        call   sub_1_3DA6
        jb     SHORT loc_1_65D0
        and    word ptr [ebx + 6ah], 0ff7fh
loc_1_6614:
        test   word ptr [ebx + 6ah], 1000h
        je     SHORT loc_1_662E
        stc
        mov    ax, word ptr [ebx + 7ch]
        call   sub_1_3DA6
loc_1_6626:
        jb     SHORT loc_1_65D0
        and    word ptr [ebx + 6ah], 0efffh
loc_1_662E:
        test   word ptr [ebx + 6ah], 100h
        je     SHORT loc_1_6647
        mov    ax, word ptr [ebx + 7eh]
        call   sub_1_3D51
        jb     SHORT loc_1_6626
        and    word ptr [ebx + 6ah], 0feffh
loc_1_6647:
        test   word ptr [ebx + 6ah], 200h
        je     SHORT loc_1_6660
        mov    ax, word ptr [ebx + 66h]
        call   sub_1_3E03
        jb     SHORT loc_1_6626
        and    word ptr [ebx + 6ah], 0fdffh
loc_1_6660:
        jmp    NEAR PTR loc_1_65C8
sub_1_6665:
        cli
        test   word ptr [edi + 10h], 2
        jne    SHORT loc_1_66BC
        push   esi
        db  08Dh, 035h, 078h, 06Fh, 000h, 000h
        call   sub_1_287D
        pop    esi
        push   esi
        push   edi
        db  0BFh, 040h, 06Fh, 000h, 000h
        mov    ecx, 8
loc_1_6687:
        mov    esi, dword ptr [edi]
        add    edi, 4
        test   esi, esi
        je     SHORT loc_1_66A5
        test   dword ptr [esi], 80h
        je     SHORT loc_1_66A5
        and    dword ptr [esi], 0ffffff7fh
        db  083h, 00Dh, 0ECh, 06Eh, 000h, 000h, 008h
loc_1_66A5:
        loop   loc_1_6687
        pop    edi
        pop    esi
loc_1_66A9:
        sti
        db  0F7h, 005h, 0ECh, 06Eh, 000h, 000h, 040h, 000h, 000h, 000h
        je     SHORT loc_1_66BB
        call   sub_1_646D
loc_1_66BB:
        ret
loc_1_66BC:
        cli
        and    word ptr [edi + 10h], 0fbffh
        test   word ptr [edi + 10h], 40h
        sti
        je     SHORT loc_1_66A9
        test   word ptr [edi + 10h], 1000h
        je     SHORT loc_1_66DE
        db  081h, 025h, 0ECh, 06Eh, 000h, 000h, 0FFh, 0EFh, 0FFh, 0FFh
loc_1_66DE:
        test   word ptr [edi + 10h], 8
        je     SHORT loc_1_66F3
        sub    eax, eax
        mov    esi, dword ptr [edi + 30h]
        mov    byte ptr [esi + 1], al
        call   sub_1_6BE9
loc_1_66F3:
        push   esi
        db  08Dh, 035h, 070h, 06Fh, 000h, 000h
        call   sub_1_287D
        pop    esi
        jmp    SHORT loc_1_66A9
loc_1_6702:
        ret
sub_1_6703:
        cli
        db  08Dh, 03Dh, 0B0h, 06Fh, 000h, 000h
        call   sub_1_2903
        sti
        je     SHORT loc_1_6702
        mov    esi, dword ptr [edi + 14h]
        movzx  ecx, word ptr [edi + 1eh]
        db  066h, 089h, 00Dh, 0C0h, 07Bh, 000h, 000h
        mov    dx, word ptr [edi + 1ch]
        db  0BBh, 0C2h, 07Bh, 000h, 000h
        or     dx, dx
        je     SHORT loc_1_6741
        db  066h, 0FFh, 005h, 0C0h, 07Bh, 000h, 000h
        mov    eax, dword ptr [edi + 18h]
        mov    dword ptr [ebx], eax
        mov    word ptr [ebx + 4], dx
        add    ebx, 6
loc_1_6741:
        jecxz  loc_1_6767
        push   edi
        lea    esi, [edi + 20h]
        mov    edi, ebx
loc_1_6749:
        push   ecx
        mov    eax, dword ptr [esi + 4]
        mov    cx, word ptr [esi + 2]
        add    dx, cx
        mov    word ptr [edi + 4], cx
        mov    dword ptr [edi], eax
        add    edi, 6
        add    esi, 8
        pop    ecx
        loop   loc_1_6749
        pop    edi
        mov    esi, dword ptr [edi + 14h]
loc_1_6767:
        db  08Bh, 01Dh, 0C2h, 07Bh, 000h, 000h
        mov    word ptr [esi + 76h], dx
        db  0B8h, 0C0h, 07Bh, 000h, 000h
        mov    dword ptr [esi + 68h], eax
        push   esi
        push   edi
        and    dword ptr [esi], 0fffffffbh
        or     dword ptr [esi], 1
        call   dword ptr [esi + 44h]
        pop    edi
        pop    esi
        call   sub_1_6665
        jmp    NEAR PTR sub_1_6703
sub_1_6790:
        cli
        db  08Dh, 01Dh, 080h, 06Fh, 000h, 000h
        call   sub_1_28F3
        lea    ebx, [ebx - 32h]
        sti
        je     SHORT loc_1_67DC
        mov    edi, dword ptr [ebx + 3ah]
        db  0FEh, 005h, 005h, 06Fh, 000h, 000h
        mov    al, byte ptr [ebx + 30h]
        call   sub_1_2550
        jb     SHORT loc_1_67C5
        test   dword ptr [esi], 2
        je     SHORT loc_1_67D3
loc_1_67BD:
        push   ebx
        call   edi
        pop    ebx
        cmp    al, 0ffh
        je     SHORT sub_1_6790
loc_1_67C5:
        mov    byte ptr [ebx + 1], al
        xor    ah, ah
        mov    esi, ebx
        call   sub_1_6BE9
        jmp    SHORT sub_1_6790
loc_1_67D3:
        cmp    byte ptr [ebx], 32h
        je     SHORT loc_1_67BD
        mov    al, 34h
        jmp    SHORT loc_1_67C5
loc_1_67DC:
        ret
loc_1_67DD:
        db  0FEh, 00Dh, 0F6h, 06Eh, 000h, 000h
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, 0fff00000h
        db  0CDh, 020h, 01Eh, 000h, 001h, 000h
        pop    ebx
        ret
sub_1_67F7:
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, 100000h
        db  0CDh, 020h, 01Eh, 000h, 001h, 000h
        db  0FEh, 005h, 0F6h, 06Eh, 000h, 000h
        pop    ebx
        jne    SHORT loc_1_67DD
sub_1_6812:
        cli
        db  0F7h, 005h, 0ECh, 06Eh, 000h, 000h, 000h, 004h, 000h, 000h
        je     SHORT loc_1_6830
        db  081h, 025h, 0ECh, 06Eh, 000h, 000h, 0FFh, 0FBh, 0FFh, 0FFh
        sti
        call   sub_1_6790
        cli
loc_1_6830:
        db  0F7h, 005h, 0ECh, 06Eh, 000h, 000h, 020h, 000h, 000h, 000h
        je     SHORT loc_1_684A
        db  083h, 025h, 0ECh, 06Eh, 000h, 000h, 0DFh
        sti
        call   sub_1_659E
        cli
loc_1_684A:
        db  0F7h, 005h, 0ECh, 06Eh, 000h, 000h, 008h, 000h, 000h, 000h
        je     SHORT loc_1_6864
        db  083h, 025h, 0ECh, 06Eh, 000h, 000h, 0F7h
        sti
        call   sub_1_5602
        cli
loc_1_6864:
        db  0F7h, 005h, 0ECh, 06Eh, 000h, 000h, 000h, 002h, 000h, 000h
        je     SHORT loc_1_6881
        db  081h, 025h, 0ECh, 06Eh, 000h, 000h, 0FFh, 0FDh, 0FFh, 0FFh
        sti
        call   sub_1_6703
        cli
loc_1_6881:
        db  0F7h, 005h, 0ECh, 06Eh, 000h, 000h, 010h, 000h, 000h, 000h
        je     SHORT loc_1_689B
        db  083h, 025h, 0ECh, 06Eh, 000h, 000h, 0EFh
        sti
        call   sub_1_64C1
        cli
loc_1_689B:
        db  0F7h, 005h, 0ECh, 06Eh, 000h, 000h, 0B8h, 006h, 000h, 000h
        jne    NEAR PTR sub_1_6812
        db  0FEh, 00Dh, 0F6h, 06Eh, 000h, 000h
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, 0fff00000h
        db  0CDh, 020h, 01Eh, 000h, 001h, 000h
        pop    ebx
        sti
        ret
        db  000h, 000h
sub_1_68C8:
        aam    0ch
        add    byte ptr [eax], al
        db  0A3h, 00Dh, 000h, 000h, 053h
        push   cs
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], ch
        mov    dword ptr [edx], ebx
        add    byte ptr [eax], al
        add    bl, byte ptr [eax + eax]
        db  000h, 078h, 01Ch
        add    byte ptr [eax], al
        sbb    al, byte ptr gs:[eax]
        db  000h, 000h
        insb
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        and    byte ptr fs:[eax], al
        db  000h, 042h, 021h
        add    byte ptr [eax], al
        pushad
        and    byte ptr [eax], al
        db  000h, 052h, 021h
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        push   10h
        add    byte ptr [eax], al
        test   dword ptr [ecx], edx
        add    byte ptr [eax], al
        sahf
        sbb    byte ptr [eax], al
        db  000h, 066h, 019h
        add    byte ptr [eax], al
        wait
        push   cs
        add    byte ptr [eax], al
        lodsb
        sbb    byte ptr [eax], al
        db  000h, 06Ah, 010h
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        popad
        sbb    al, byte ptr [eax]
        db  000h, 000h
        insb
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax + eax], ch
        cmp    bl, byte ptr [edx]
        add    byte ptr [eax], al
        inc    edi
        sbb    al, byte ptr [eax]
        db  000h, 04Bh, 01Ah
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], ch
        dec    ebx
        push   ss
        add    byte ptr [eax], al
        db  02Dh, 010h, 000h, 000h, 02Eh
        push   ss
        add    byte ptr [eax], al
loc_1_6A64:
        mov    al, 3
        jmp    NEAR PTR loc_1_6BE1
loc_1_6A6B:
        movzx  ecx, byte ptr [ebx]
        push   ecx
        mov    al, byte ptr [ebx + 30h]
        call   sub_1_2550
        and    cl, 7fh
        sub    cl, 10h
        jb     SHORT loc_1_6A64
        cmp    cl, 66h
        ja     SHORT loc_1_6A64
        mov    byte ptr [ebx + 1], 0ffh
        mov    byte ptr [ebx + 31h], 0ffh
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, 100000h
        db  0CDh, 020h, 01Eh, 000h, 001h, 000h
        db  0FEh, 005h, 0F6h, 06Eh, 000h, 000h
        pop    ebx
        jne    SHORT loc_1_6B13
        test   dword ptr [esi], 2
        je     NEAR PTR loc_1_6BB7
loc_1_6AB3:
        push   ebx
        db  0FFh, 014h, 08Dh, 0C8h, 068h, 000h, 000h
        pop    ebx
        sub    ah, ah
        cmp    al, 0ffh
        jne    SHORT loc_1_6B00
        xor    al, al
loc_1_6AC4:
        db  0F7h, 005h, 0ECh, 06Eh, 000h, 000h, 0B8h, 006h, 000h, 000h
        jne    SHORT loc_1_6AF5
        db  0FEh, 00Dh, 0F6h, 06Eh, 000h, 000h
        push   eax
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, 0fff00000h
        db  0CDh, 020h, 01Eh, 000h, 001h, 000h
        pop    ebx
        pop    eax
loc_1_6AEB:
        test   byte ptr [esp], 80h
        je     SHORT loc_1_6B5E
loc_1_6AF1:
        add    esp, 4
        ret
loc_1_6AF5:
        push   eax
        push   ebx
        call   sub_1_6812
        pop    ebx
        pop    eax
        jmp    SHORT loc_1_6AEB
loc_1_6B00:
        mov    byte ptr [ebx + 1], al
        mov    byte ptr [ebx + 31h], al
        test   al, al
        jne    SHORT loc_1_6AC4
        mov    esi, ebx
        call   sub_1_6BE9
        jmp    SHORT loc_1_6AC4
loc_1_6B13:
        db  0FEh, 00Dh, 0F6h, 06Eh, 000h, 000h
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, 0fff00000h
        db  0CDh, 020h, 01Eh, 000h, 001h, 000h
        pop    ebx
        db  0FEh, 00Dh, 005h, 06Fh, 000h, 000h
        db  08Bh, 004h, 08Dh, 0C8h, 068h, 000h, 000h
        mov    dword ptr [ebx + 3ah], eax
        cli
        push   esi
        db  08Dh, 035h, 080h, 06Fh, 000h, 000h
        push   ebx
        lea    ebx, [ebx + 32h]
        call   sub_1_2892
        pop    ebx
        pop    esi
        sti
        db  081h, 00Dh, 0ECh, 06Eh, 000h, 000h, 000h, 004h, 000h, 000h
        sub    eax, eax
        jmp    SHORT loc_1_6AEB
loc_1_6B5E:
        sub    edx, edx
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        db  0CDh, 020h, 01Ah, 000h, 001h, 000h
loc_1_6B7B:
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        cmp    byte ptr [ebx + 31h], 0ffh
        jne    SHORT loc_1_6B9A
        push   eax
        sub    eax, eax
        cmp    edx, 40h
        jbe    SHORT loc_1_6B90
        inc    eax
loc_1_6B90:
        db  0CDh, 020h, 061h, 001h, 001h, 000h
        pop    eax
        inc    edx
        jmp    SHORT loc_1_6B7B
loc_1_6B9A:
        mov    al, byte ptr [ebx + 31h]
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
loc_1_6BB2:
        jmp    NEAR PTR loc_1_6AF1
loc_1_6BB7:
        cmp    byte ptr [ebx], 32h
        je     NEAR PTR loc_1_6AB3
        mov    al, 34h
        db  0FEh, 00Dh, 0F6h, 06Eh, 000h, 000h
        push   eax
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, 0fff00000h
        db  0CDh, 020h, 01Eh, 000h, 001h, 000h
        pop    ebx
        pop    eax
        jmp    SHORT loc_1_6BE1
        db  0B0h, 022h
loc_1_6BE1:
        mov    byte ptr [ebx + 1], al
        mov    byte ptr [ebx + 31h], al
        jmp    SHORT loc_1_6BB2
sub_1_6BE9:
        pushad
        mov    ebx, esi
        mov    al, byte ptr [ebx + 1]
        mov    byte ptr [ebx + 31h], al
        test   byte ptr [ebx], 80h
        je     SHORT loc_1_6BFE
        mov    ecx, dword ptr [ebx + 2ch]
        jecxz  loc_1_6BFE
        call   ecx
loc_1_6BFE:
        popad
        ret
loc_1_6C00:
        mov    al, 3
        ret
        db  000h
sub_1_6C04:
        db  0CDh, 020h, 029h, 080h, 028h, 000h
sub_1_6C0A:
        db  0CDh, 020h, 02Ah, 080h, 028h, 000h
sub_1_6C10:
        db  0CDh, 020h, 02Bh, 080h, 028h, 000h
sub_1_6C16:
        db  0CDh, 020h, 02Ch, 080h, 028h, 000h
sub_1_6C1C:
        db  0CDh, 020h, 02Dh, 080h, 028h, 000h
sub_1_6C22:
        db  0CDh, 020h, 02Eh, 080h, 028h, 000h
        db  0CDh, 020h, 02Fh, 080h, 028h, 000h
sub_1_6C2E:
        db  0CDh, 020h, 030h, 080h, 028h, 000h
        db  0CDh, 020h, 050h, 080h, 028h, 000h
sub_1_6C3A:
        db  0CDh, 020h, 051h, 080h, 028h, 000h
        db  0CDh, 020h, 052h, 080h, 028h, 000h
        db  0CDh, 020h, 053h, 080h, 028h, 000h
        db  0CDh, 020h, 000h, 080h, 028h, 000h
        db  0CDh, 020h, 00Ch, 080h, 028h, 000h
        db  0CDh, 020h, 00Dh, 080h, 028h, 000h
        db  0CDh, 020h, 00Fh, 080h, 028h, 000h
        db  0CDh, 020h, 010h, 080h, 028h, 000h
        db  0CDh, 020h, 011h, 080h, 028h, 000h
        db  0CDh, 020h, 00Eh, 080h, 028h, 000h
sub_1_6C76:
        db  0CDh, 020h, 00Ah, 080h, 028h, 000h
sub_1_6C7C:
        db  0CDh, 020h, 00Bh, 080h, 028h, 000h
sub_1_6C82:
        db  0CDh, 020h, 016h, 080h, 028h, 000h
sub_1_6C88:
        db  0CDh, 020h, 017h, 080h, 028h, 000h
sub_1_6C8E:
        db  0CDh, 020h, 018h, 080h, 028h, 000h
sub_1_6C94:
        db  0CDh, 020h, 019h, 080h, 028h, 000h
sub_1_6C9A:
        db  0CDh, 020h, 01Ah, 080h, 028h, 000h
        db  0CDh, 020h, 01Bh, 080h, 028h, 000h
sub_1_6CA6:
        db  0CDh, 020h, 01Ch, 080h, 028h, 000h
sub_1_6CAC:
        db  0CDh, 020h, 01Dh, 080h, 028h, 000h
sub_1_6CB2:
        db  0CDh, 020h, 01Eh, 080h, 028h, 000h
        db  0CDh, 020h, 01Fh, 080h, 028h, 000h
sub_1_6CBE:
        db  0CDh, 020h, 020h, 080h, 028h, 000h
sub_1_6CC4:
        db  0CDh, 020h, 021h, 080h, 028h, 000h
        db  0CDh, 020h, 022h, 080h, 028h, 000h
        db  0CDh, 020h, 023h, 080h, 028h, 000h
sub_1_6CD6:
        db  0CDh, 020h, 024h, 080h, 028h, 000h
sub_1_6CDC:
        db  0CDh, 020h, 025h, 080h, 028h, 000h
        db  0CDh, 020h, 026h, 080h, 028h, 000h
        db  0CDh, 020h, 027h, 080h, 028h, 000h
        db  0CDh, 020h, 028h, 080h, 028h, 000h
        db  0CDh, 020h, 005h, 080h, 028h, 000h
sub_1_6CFA:
        db  0CDh, 020h, 006h, 080h, 028h, 000h
sub_1_6D00:
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
        db  0CDh, 020h, 045h, 080h, 028h, 000h
        db  0CDh, 020h, 043h, 080h, 028h, 000h
        db  0CDh, 020h, 047h, 080h, 028h, 000h
        add    byte ptr [eax], al
        adc    al, 0
        add    byte ptr [eax], al
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 031h, 000h, 003h, 000h, 000h, 000h, 04Eh, 045h, 054h, 042h
        db  045h, 055h, 049h, 020h, 000h, 060h, 000h, 054h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 04Eh, 065h, 074h, 077h, 06Fh, 072h, 06Bh, 000h
loc_1_6DE0:
        dec    esi
        inc    ebp
        push   esp
        inc    edx
        inc    ebp
        push   ebp
        dec    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_6DEC:
        add    ecx, dword ptr [edx]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        mov    bh, 7
        add    byte ptr [eax], al
        fld    dword ptr [edi]
        add    byte ptr [eax], al
        out    dx, al
        pop    es
        add    byte ptr [eax], al
        cmpsb
        or     byte ptr [eax], al
        db  000h, 0EBh
        pop    es
        add    byte ptr [eax], al
        mov    cl, byte ptr [ecx]
        add    byte ptr [eax], al
        add    dword ptr [ecx], ecx
        add    byte ptr [eax], al
        aam    8
        add    byte ptr [eax], al
        sahf
        or     dword ptr [eax], eax
        db  000h, 0C2h
        or     al, byte ptr [eax]
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 01Bh
        add    dword ptr [eax], eax
        db  000h, 0C4h
        add    dword ptr [eax], eax
        add    byte ptr [eax], cl
        add    byte ptr [eax], cl
        db  000h, 02Eh
        outsb
        add    byte ptr [eax], al
loc_1_6E2E:
        dec    esi
        inc    ebp
        push   esp
        inc    edx
        inc    ebp
        push   ebp
        dec    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi], ch
        xor    ebp, dword ptr [esi]
        xor    eax, 4944h
        add    byte ptr [eax], al
loc_1_6E48:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_6E4C:
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
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax - 80000000h], 0d5h
loc_1_6E79:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        scasd
        adc    dword ptr [eax], eax
        add    byte ptr [ecx], dl
        rcl    dword ptr [ecx]
        add    byte ptr [eax], al
        db  015h, 0EDh, 012h, 000h, 000h
        push   ss
        add    dword ptr [ebx], edx
        add    byte ptr [eax], al
        db  023h, 003h
        adc    eax, dword ptr [eax]
        add    byte ptr [ecx], ah
        add    eax, 33000013h
        pop    es
        adc    eax, dword ptr [eax]
        db  000h, 014h, 072h
        adc    eax, dword ptr [eax]
        add    byte ptr [edi], dl
        db  072h, 013h
        add    byte ptr [eax], al
        dec    eax
        mov    al, byte ptr [76000011h]
        add    edx, dword ptr [edx]
        add    byte ptr [eax], al
        db  074h, 003h
        adc    al, byte ptr [eax]
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
loc_1_6EF1:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    bh, bh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_6EFF:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_6F03:
        add    byte ptr [eax], al
loc_1_6F05:
        inc    dword ptr [eax]
loc_1_6F07:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_6F0B:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_6F0F:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_6F13:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_6F17:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_6F1B:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_6F1F:
        add    byte ptr [eax], al
loc_1_6F21:
        add    byte ptr [eax], al
loc_1_6F23:
        add    byte ptr [eax], al
loc_1_6F25:
        add    byte ptr [eax], al
loc_1_6F27:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_6F2B:
        add    byte ptr [eax], al
loc_1_6F2D:
        add    byte ptr [eax], al
loc_1_6F2F:
        add    byte ptr [eax], al
loc_1_6F31:
        add    byte ptr [eax], al
loc_1_6F33:
        add    byte ptr [eax], al
loc_1_6F35:
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 6db0h], ah
        test   al, al
        je     SHORT loc_1_700B
        clc
        ret
loc_1_700B:
        inc    byte ptr [6db0h]
        db  0CDh, 020h, 000h, 000h, 028h, 000h
        jb     SHORT loc_1_7056
        db  0CDh, 020h, 000h, 000h, 014h, 000h
        jb     SHORT loc_1_7056
        cmp    eax, 30ah
        jb     SHORT loc_1_7056
        db  0CDh, 020h, 000h, 000h, 040h, 000h
        jb     SHORT loc_1_7037
        mov    byte ptr [6ec0h], 1
loc_1_7037:
        clc
        pushf
        cld
        push   ecx
        push   edi
        push   eax
        xor    eax, eax
        mov    edi, 77c0h
        mov    ecx, 100h
        rep stosd
        pop    eax
        pop    edi
        pop    ecx
        popf
        call   sub_1_743B
loc_1_7056:
        ret
        db  000h
sub_1_7058:
        xor    ecx, ecx
        movzx  eax, byte ptr [6f22h]
        imul   eax, eax, 84h
        add    ecx, eax
        movzx  eax, byte ptr [6f23h]
        imul   eax, eax, 3eh
        add    ecx, eax
        movzx  eax, byte ptr [6f24h]
        imul   eax, eax, 70h
        add    ecx, eax
        add    ecx, 3
        and    cl, 0fch
        push   1
        push   ecx
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_709D
        mov    dword ptr [6e3ah], eax
        ret
loc_1_709D:
        stc
        ret
sub_1_709F:
        mov    dword ptr [6eech], 1
        lea    eax, [6f68h]
        mov    dword ptr [6f68h], eax
        mov    dword ptr [6f6ch], eax
        lea    eax, [6f70h]
        mov    dword ptr [6f70h], eax
        mov    dword ptr [6f74h], eax
        lea    eax, [6f78h]
        mov    dword ptr [6f78h], eax
        mov    dword ptr [6f7ch], eax
        lea    eax, [6f88h]
        mov    dword ptr [6f88h], eax
        mov    dword ptr [6f8ch], eax
        lea    eax, [6f90h]
        mov    dword ptr [6f90h], eax
        mov    dword ptr [6f94h], eax
        lea    eax, [6f98h]
        mov    dword ptr [6f98h], eax
        mov    dword ptr [6f9ch], eax
        lea    eax, [6fa0h]
        mov    dword ptr [6fa0h], eax
        mov    dword ptr [6fa4h], eax
        lea    eax, [6fa8h]
        mov    dword ptr [6fa8h], eax
        mov    dword ptr [6fach], eax
        lea    eax, [6fb0h]
        mov    dword ptr [6fb0h], eax
        mov    dword ptr [6fb4h], eax
        lea    eax, [6f80h]
        mov    dword ptr [6f80h], eax
        mov    dword ptr [6f84h], eax
        mov    edi, 6fc0h
        mov    ecx, 100h
loc_1_7153:
        lea    eax, [edi]
        mov    dword ptr [edi], eax
        mov    dword ptr [edi + 4], eax
        add    edi, 8
        loop   loc_1_7153
        mov    edi, dword ptr [6e3ah]
        mov    dword ptr [6f64h], edi
        movzx  ecx, byte ptr [6f22h]
loc_1_7172:
        lea    eax, [edi + 34h]
        mov    dword ptr [edi + 24h], eax
        mov    word ptr [edi + 1eh], 1
        mov    word ptr [edi + 10h], 2
        cli
        push   esi
        lea    esi, [6f70h]
        db  0E8h, 0ECh, 026h, 0FEh, 0FFh
        pop    esi
        sti
        add    edi, 84h
        loop   loc_1_7172
        mov    dword ptr [6f60h], edi
        movzx  ecx, byte ptr [6f23h]
loc_1_71A8:
        mov    word ptr [edi + 10h], 8001h
        cli
        push   esi
        lea    esi, [6f68h]
        db  0E8h, 0C2h, 026h, 0FEh, 0FFh
        pop    esi
        sti
        add    edi, 3eh
        loop   loc_1_71A8
        movzx  ecx, byte ptr [6f24h]
        jecxz  loc_1_71DF
loc_1_71CB:
        cli
        push   esi
        lea    esi, [6f78h]
        db  0E8h, 0A5h, 026h, 0FEh, 0FFh
        pop    esi
        sti
        add    edi, 70h
        loop   loc_1_71CB
loc_1_71DF:
        db  0CDh, 020h, 002h, 000h, 005h, 000h
        mov    dword ptr [6de8h], eax
        mov    ecx, eax
        mov    eax, dword ptr [6f07h]
        sub    edx, edx
        idiv   ecx
        cmp    edx, 1
        cmc
        adc    eax, 0
        mov    dword ptr [6f07h], eax
        mov    eax, dword ptr [6f0bh]
        sub    edx, edx
        idiv   ecx
        mov    dword ptr [6f0bh], eax
        cmp    edx, 1
        cmc
        adc    eax, 0
        mov    eax, dword ptr [6f0fh]
        sub    edx, edx
        idiv   ecx
        cmp    edx, 1
        cmc
        adc    eax, 0
        mov    dword ptr [6f0fh], eax
        mov    eax, dword ptr [6f0bh]
        movzx  edx, byte ptr [6f26h]
        mov    byte ptr [6f2dh], dl
        test   edx, edx
        je     SHORT loc_1_7243
        mul    edx
loc_1_7243:
        mov    edx, eax
        shr    eax, 1
        or     al, 1
        add    eax, edx
        mov    edx, dword ptr [6f27h]
        test   edx, edx
        je     SHORT loc_1_7259
        cmp    eax, edx
        ja     SHORT loc_1_725B
loc_1_7259:
        mov    eax, edx
loc_1_725B:
        sub    edx, edx
        idiv   ecx
        cmp    edx, 1
        cmc
        adc    eax, 0
        mov    dword ptr [6f13h], eax
        mov    al, byte ptr [6f2eh]
        shr    al, 1
        or     al, 1
        mov    byte ptr [6f2fh], al
        mov    eax, dword ptr [6f1bh]
        sub    edx, edx
        idiv   ecx
        cmp    edx, 1
        cmc
        adc    eax, 0
        mov    dword ptr [6f1bh], eax
        mov    eax, 7530h
        sub    edx, edx
        idiv   ecx
        cmp    edx, 1
        cmc
        adc    eax, 0
        mov    dword ptr [6f17h], eax
        mov    eax, 1f4h
        sub    edx, edx
        idiv   ecx
        cmp    edx, 1
        cmc
        adc    eax, 0
        mov    dword ptr [6ec8h], eax
        mov    dword ptr [6ec4h], eax
        mov    eax, 3e8h
        sub    edx, edx
        idiv   ecx
        cmp    edx, 1
        cmc
        adc    eax, 0
        mov    dword ptr [6ed0h], eax
        mov    dword ptr [6ecch], eax
        mov    al, byte ptr [6f31h]
        cmp    al, byte ptr [6f30h]
        jbe    SHORT loc_1_72F0
        mov    al, byte ptr [6f30h]
        shr    al, 1
        mov    byte ptr [6f31h], al
loc_1_72F0:
        clc
        ret
sub_1_72F2:
        mov    eax, dword ptr [6de8h]
        mov    esi, 3654h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    dword ptr [6f38h], esi
        cmp    esi, 1
        ret
sub_1_730C:
        enter  18h, 0
        push   edi
        push   esi
        lea    edi, [6e48h]
        lea    esi, [6de0h]
        mov    word ptr [ebp - 8], 8
        mov    word ptr [ebp - 6], 8
        mov    dword ptr [ebp - 4], esi
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        push   edi
        db  0E8h, 0FFh, 068h, 0FEh, 0FFh
        cmp    dword ptr [edi], 0
        je     SHORT loc_1_7346
loc_1_7340:
        stc
        jmp    NEAR PTR loc_1_7413
loc_1_7346:
        cld
        lea    ebx, [ebp - 0ch]
        mov    edx, 43eh
loc_1_734F:
        add    edx, 3eh
        cmp    edx, 0a8ah
        jae    NEAR PTR loc_1_740A
        mov    ax, word ptr [edx + 20h]
        mov    word ptr [ebp - 8], ax
        mov    word ptr [ebp - 6], ax
        lea    eax, [edx]
        mov    dword ptr [ebp - 4], eax
        push   edx
        push   dword ptr [edx + 22h]
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp - 10h]
        push   ebx
        push   6e48h
        db  0E8h, 075h, 069h, 0FEh, 0FFh
        pop    edx
        cmp    dword ptr [6e48h], 0
        jne    SHORT loc_1_73B9
        cmp    dword ptr [edx + 22h], 3
        je     SHORT loc_1_73BE
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 4]
        lea    esi, [edx + 2eh]
        cmp    eax, dword ptr [esi]
        jae    SHORT loc_1_73AF
        lea    esi, [edx + 2ah]
        cmp    eax, dword ptr [esi]
        jb     SHORT loc_1_73AF
        mov    esi, dword ptr [ebp - 0ch]
        lea    esi, [esi + 4]
loc_1_73AF:
        mov    edi, dword ptr [edx + 3ah]
        mov    ecx, dword ptr [edx + 36h]
        rep movsb
        jmp    SHORT loc_1_734F
loc_1_73B9:
        lea    esi, [edx + 32h]
        jmp    SHORT loc_1_73AF
loc_1_73BE:
        mov    esi, dword ptr [ebp - 0ch]
        movzx  ecx, word ptr [esi + 4]
        push   edx
        mov    eax, ecx
        db  0E8h, 066h, 01Eh, 0FEh, 0FFh
        pop    edx
        movzx  ecx, word ptr [esi + 4]
        mov    esi, dword ptr [esi + 8]
        mov    edi, eax
        test   eax, eax
        je     NEAR PTR loc_1_7340
        push   ecx
        rep movsb
        pop    ecx
        mov    edi, eax
        mov    esi, dword ptr [edx + 3ah]
        mov    eax, dword ptr [edx + 36h]
loc_1_73EB:
        cmp    byte ptr [edi], 0
        je     NEAR PTR loc_1_734F
        mov    dword ptr [esi], edi
        add    esi, 4
        sub    eax, 4
        je     NEAR PTR loc_1_734F
        push   eax
        xor    al, al
        repne scasb
        pop    eax
        jmp    SHORT loc_1_73EB
loc_1_740A:
        push   dword ptr [ebp - 10h]
        db  0E8h, 0EEh, 068h, 0FEh, 0FFh
        clc
loc_1_7413:
        pop    esi
        pop    edi
        leave
        ret
sub_1_7417:
        enter  4, 0
        push   42h
        push   6dech
        push   6db4h
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 0D4h, 067h, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 4]
        cmp    eax, 0
        je     SHORT loc_1_7439
        stc
loc_1_7439:
        leave
        ret
sub_1_743B:
        call   sub_1_7417
        jb     SHORT loc_1_7471
        call   sub_1_730C
        jb     SHORT loc_1_7473
        call   sub_1_7058
        jb     SHORT loc_1_7475
        call   sub_1_709F
        jb     SHORT loc_1_7477
        call   sub_1_72F2
        jb     SHORT loc_1_7479
        ret
loc_1_745F:
        push   dword ptr [6db4h]
        push   6e48h
        db  0E8h, 09Bh, 067h, 0FEh, 0FFh
        stc
        ret
loc_1_7471:
        stc
        ret
loc_1_7473:
        jmp    SHORT loc_1_745F
loc_1_7475:
        jmp    SHORT loc_1_745F
loc_1_7477:
        jmp    SHORT loc_1_745F
loc_1_7479:
        jmp    SHORT loc_1_745F
        db  000h, 050h, 049h, 047h, 047h, 059h, 042h, 041h, 043h, 04Bh, 041h, 043h, 04Bh, 053h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 00Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h
        db  000h, 000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 032h, 06Fh, 000h, 000h, 042h
        db  049h, 04Eh, 044h, 049h, 04Eh, 047h, 053h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 009h
        db  000h, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 020h, 000h, 000h, 000h, 0B8h, 06Dh, 000h, 000h, 053h, 045h, 053h
        db  053h, 049h, 04Fh, 04Eh, 053h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 009h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 0FEh, 000h, 000h, 000h, 006h
        db  000h, 000h, 000h, 001h, 000h, 000h, 000h, 0F0h, 06Eh, 000h, 000h, 04Eh, 041h, 04Dh, 045h, 043h
        db  041h, 043h, 048h, 045h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 001h, 000h, 000h, 000h, 025h, 06Fh, 000h, 000h, 04Eh, 041h, 04Dh, 045h, 053h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 006h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 001h, 000h, 000h, 000h, 0FEh, 000h, 000h, 000h, 011h, 000h, 000h, 000h, 001h
        db  000h, 000h, 000h, 0F1h, 06Eh, 000h, 000h, 050h, 041h, 043h, 04Bh, 045h, 054h, 053h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 008h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 001h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 032h, 000h, 000h, 000h, 001h, 000h, 000h
        db  000h, 023h, 06Fh, 000h, 000h, 04Ch, 04Fh, 04Fh, 050h, 050h, 041h, 043h, 04Bh, 045h, 054h, 053h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 003h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 024h
        db  06Fh, 000h, 000h, 044h, 041h, 054h, 041h, 047h, 052h, 041h, 04Dh, 050h, 041h, 043h, 04Bh, 045h
        db  054h, 053h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 010h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 000h
        db  000h, 0FFh, 0FFh, 0FFh, 0FFh, 014h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 022h, 06Fh, 000h
        db  000h, 054h, 049h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh
        db  0FFh, 0FFh, 0FFh, 030h, 075h, 000h, 000h, 004h, 000h, 000h, 000h, 007h, 06Fh, 000h, 000h, 054h
        db  031h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 003h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh
        db  0FFh, 0F4h, 001h, 000h, 000h, 004h, 000h, 000h, 000h, 00Bh, 06Fh, 000h, 000h, 054h, 032h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 003h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 0C8h
        db  000h, 000h, 000h, 004h, 000h, 000h, 000h, 00Fh, 06Fh, 000h, 000h, 041h, 044h, 041h, 050h, 054h
        db  052h, 041h, 054h, 045h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 0E8h, 003h, 000h
        db  000h, 004h, 000h, 000h, 000h, 027h, 06Fh, 000h, 000h, 057h, 049h, 04Eh, 044h, 04Fh, 057h, 045h
        db  052h, 052h, 04Fh, 052h, 053h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Dh, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 001h, 000h, 000h, 000h, 001h
        db  000h, 000h, 000h, 026h, 06Fh, 000h, 000h, 04Eh, 045h, 054h, 042h, 049h, 04Fh, 053h, 054h, 049h
        db  04Dh, 045h, 04Fh, 055h, 054h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 0F4h, 001h, 000h, 000h, 004h, 000h, 000h
        db  000h, 01Bh, 06Fh, 000h, 000h, 04Eh, 045h, 054h, 042h, 049h, 04Fh, 053h, 052h, 045h, 054h, 052h
        db  049h, 045h, 053h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h
        db  000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 002h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 01Fh
        db  06Fh, 000h, 000h, 057h, 041h, 04Eh, 052h, 045h, 054h, 052h, 049h, 045h, 053h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 00Bh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h
        db  000h, 0FFh, 0FFh, 0FFh, 0FFh, 002h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 021h, 06Fh, 000h
        db  000h, 044h, 04Ch, 043h, 052h, 045h, 054h, 052h, 049h, 045h, 053h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 00Bh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 0FFh
        db  0FFh, 0FFh, 0FFh, 005h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 020h, 06Fh, 000h, 000h, 04Dh
        db  041h, 058h, 049h, 04Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 006h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 07Fh, 000h, 000h
        db  000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 02Bh, 06Fh, 000h, 000h, 04Dh, 041h, 058h
        db  04Fh, 055h, 054h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 007h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 07Fh, 000h, 000h, 000h, 001h
        db  000h, 000h, 000h, 001h, 000h, 000h, 000h, 02Ch, 06Fh, 000h, 000h, 050h, 049h, 050h, 045h, 04Ch
        db  049h, 04Eh, 045h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 009h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 014h, 000h, 000h
        db  000h, 001h, 000h, 000h, 000h, 02Eh, 06Fh, 000h, 000h, 04Dh, 041h, 058h, 054h, 052h, 041h, 04Eh
        db  053h, 04Dh, 049h, 054h, 053h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Dh, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 001h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 005h, 000h, 000h, 000h, 001h
        db  000h, 000h, 000h, 030h, 06Fh, 000h, 000h, 04Dh, 049h, 04Eh, 054h, 052h, 041h, 04Eh, 053h, 04Dh
        db  049h, 054h, 053h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Dh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 002h, 000h, 000h, 000h, 001h, 000h, 000h
        db  000h, 031h, 06Fh, 000h, 000h, 04Ch, 041h, 04Eh, 041h, 042h, 041h, 053h, 045h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 009h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 01Fh, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 033h
        db  06Fh, 000h, 000h, 055h, 053h, 045h, 043h, 053h, 041h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 007h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 035h, 06Fh, 000h
        db  000h, 051h, 075h, 065h, 072h, 079h, 041h, 06Ch, 077h, 061h, 079h, 073h, 055h, 073h, 065h, 073h
        db  053h, 06Fh, 075h, 072h, 063h, 065h, 052h, 06Fh, 075h, 074h, 069h, 06Eh, 067h, 000h, 000h, 000h
        db  000h, 01Dh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 036h, 06Fh, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h
_LTEXT ends
        end
