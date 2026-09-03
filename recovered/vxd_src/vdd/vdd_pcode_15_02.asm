; vdd_pcode_15_02.asm -- VDD object 15 segment _PTEXT  (part 2/2)
.386p
_PTEXT segment dword public use32 'PCODE'

sub_15_47E8:
        bts    dword ptr [edi], 0ah
        jb     SHORT loc_15_480D
        push   ebx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    eax, 10h
        mov    ecx, 0bh
        mov    edx, edi
        db  0BEh, 00Eh, 048h, 000h, 000h
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        pop    ebx
loc_15_480D:
        ret
loc_15_480E:
        mov    ebx, edx
        db  02Bh, 01Dh, 0CCh, 006h, 000h, 000h
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     SHORT loc_15_4835
        db  0BEh, 036h, 048h, 000h, 000h
        mov    ecx, 41ah
        mov    eax, 1000a0h
        db  0CDh, 020h, 002h, 000h, 017h, 000h
        jb     SHORT loc_15_4836
loc_15_4835:
        ret
loc_15_4836:
        and    byte ptr [edx + 1], 0fbh
        mov    edi, edx
        mov    ebx, edx
        db  02Bh, 01Dh, 0CCh, 006h, 000h, 000h
        jmp    NEAR PTR sub_15_016E
sub_15_4849:
        test   byte ptr [edi + 2], 2
        je     SHORT loc_15_4876
        btr    dword ptr [edi], 7
        jae    SHORT loc_15_4876
        cmp    dword ptr [edi + 5d4h], 0
        jne    SHORT loc_15_4876
        xor    eax, eax
        mov    ecx, 3
        db  0BEh, 005h, 049h, 000h, 000h
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        mov    dword ptr [edi + 5d4h], esi
loc_15_4876:
        ret
sub_15_4877:
        test   byte ptr [edi + 2], 40h
        je     SHORT loc_15_4882
        call   sub_15_498E
loc_15_4882:
        ret
sub_15_4883:
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        mov    dword ptr [edi + 5d4h], 0
        mov    esi, dword ptr [edi + 5dch]
        or     esi, esi
        je     SHORT loc_15_48AC
        push   edi
        lea    edi, [edi + 0ch]
        mov    ecx, 25h
        cld
        rep movsd
        pop    edi
loc_15_48AC:
        db  0A1h, 0FCh, 00Eh, 000h, 000h
        or     eax, eax
        je     SHORT loc_15_48BF
        db  066h, 08Bh, 00Dh, 0B2h, 00Fh, 000h, 000h
        or     word ptr [eax], cx
loc_15_48BF:
        push   ebp
        mov    ebp, dword ptr [ebx + 8]
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 4001h
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        test   byte ptr [edi], 80h
        je     SHORT loc_15_495E
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    ebp
        ret
sub_15_4905:
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        db  03Bh, 01Dh, 0F0h, 00Eh, 000h, 000h
        jne    SHORT loc_15_4938
        db  0F6h, 005h, 0A2h, 007h, 000h, 000h, 005h
        je     SHORT loc_15_4938
        xor    eax, eax
        mov    ecx, 0bh
        db  0BEh, 005h, 049h, 000h, 000h
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        mov    dword ptr [edi + 5d4h], esi
        jmp    SHORT loc_15_498D
loc_15_4938:
        mov    dword ptr [edi + 5d4h], 0
        test   byte ptr [edi], 80h
        push   ebp
        mov    ebp, dword ptr [ebx + 8]
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
loc_15_495E:
        mov    word ptr [ebp + 1ch], 4002h
        mov    eax, 2fh
        or     byte ptr [edi + 3], 1
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        and    byte ptr [edi + 3], 0feh
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    ebp
loc_15_498D:
        ret
sub_15_498E:
        cmp    dword ptr [edi + 5d4h], 0
        je     SHORT loc_15_49A8
        test   byte ptr [edi], 80h
        jne    SHORT loc_15_49C4
        db  080h, 00Dh, 0C4h, 006h, 000h, 000h, 010h
        or     byte ptr [edi], 80h
        jmp    SHORT loc_15_49C4
loc_15_49A8:
        db  080h, 00Dh, 0C4h, 006h, 000h, 000h, 010h
        or     byte ptr [edi], 80h
        db  0CDh, 020h, 002h, 000h, 001h, 000h
        jne    SHORT loc_15_49C5
        test   byte ptr [ebx], 8
        jne    SHORT loc_15_49C5
        call   sub_15_4883
loc_15_49C4:
        ret
loc_15_49C5:
        xor    eax, eax
        mov    ecx, 3
        db  0BEh, 083h, 048h, 000h, 000h
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        mov    dword ptr [edi + 5d4h], esi
        ret
sub_15_49DE:
        test   byte ptr [edi + 4], 3
        je     SHORT loc_15_4A17
        push   edi
        push   ebx
loc_15_49E6:
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, dword ptr [esp]
        je     SHORT loc_15_4A15
        test   byte ptr [ebx], 2
        je     SHORT loc_15_49E6
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        test   byte ptr [edi], 80h
        jne    SHORT loc_15_49E6
        test   byte ptr [edi + 2], 40h
        je     SHORT loc_15_49E6
        call   sub_15_4538
        call   sub_15_498E
        jmp    SHORT loc_15_49E6
loc_15_4A15:
        pop    ebx
        pop    edi
loc_15_4A17:
        ret
sub_15_4A18:
        call   sub_15_2962
        jae    SHORT loc_15_4A7E
        push   edi
        push   ebx
        test   byte ptr [edi + 4], 80h
        jne    SHORT loc_15_4A7B
loc_15_4A27:
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, dword ptr [esp]
        je     SHORT loc_15_4A7B
        test   byte ptr [ebx], 2
        je     SHORT loc_15_4A27
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        test   byte ptr [edi], 80h
        jne    SHORT loc_15_4A27
        test   byte ptr [edi + 2], 2
        je     SHORT loc_15_4A57
        test   byte ptr [edi + 4], 10h
        jne    SHORT loc_15_4A27
        call   sub_15_498E
        jmp    SHORT loc_15_4A27
loc_15_4A57:
        call   sub_15_3A9D
        jae    SHORT loc_15_4A27
        db  0CDh, 020h, 02Bh, 000h, 001h, 000h
        jb     SHORT loc_15_4A27
        or     byte ptr [edi], 80h
        or     byte ptr [edi + 9], 4
        db  080h, 00Dh, 0C4h, 006h, 000h, 000h, 010h
        db  0E8h, 0A2h, 0BAh, 0F0h, 0FFh
        jmp    SHORT loc_15_4A27
loc_15_4A7B:
        pop    ebx
        pop    edi
        ret
loc_15_4A7E:
        db  00Fh, 0BAh, 035h, 0C4h, 006h, 000h, 000h, 004h
        jae    NEAR PTR loc_15_4B2B
        push   edi
        push   ebx
        jmp    SHORT loc_15_4A9F
loc_15_4A90:
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, dword ptr [esp]
        je     NEAR PTR loc_15_4B29
loc_15_4A9F:
        db  03Bh, 01Dh, 0A4h, 007h, 000h, 000h
        je     SHORT loc_15_4AAC
        test   byte ptr [ebx], 2
        je     SHORT loc_15_4A90
loc_15_4AAC:
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        test   byte ptr [edi], 80h
        je     SHORT loc_15_4A90
        test   byte ptr [edi + 2], 2
        je     SHORT loc_15_4B03
        db  03Bh, 01Dh, 0A4h, 007h, 000h, 000h
        je     SHORT loc_15_4AD6
        test   byte ptr [edi + 2], 40h
        je     SHORT loc_15_4AD6
loc_15_4ACD:
        db  080h, 00Dh, 0C4h, 006h, 000h, 000h, 010h
        jmp    SHORT loc_15_4A90
loc_15_4AD6:
        push   eax
        call   sub_15_3E44
        pop    eax
        and    byte ptr [edi], 7fh
        cmp    dword ptr [edi + 5d4h], 0
        jne    SHORT loc_15_4A90
        xor    eax, eax
        mov    ecx, 3
        db  0BEh, 005h, 049h, 000h, 000h
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        mov    dword ptr [edi + 5d4h], esi
        jmp    SHORT loc_15_4A90
loc_15_4B03:
        db  03Bh, 01Dh, 0A8h, 007h, 000h, 000h
        je     SHORT loc_15_4B1B
        push   edi
        db  08Bh, 03Dh, 0D2h, 00Eh, 000h, 000h
        db  03Bh, 03Dh, 0CCh, 00Eh, 000h, 000h
        pop    edi
        jbe    SHORT loc_15_4ACD
loc_15_4B1B:
        and    byte ptr [edi], 7fh
        db  0CDh, 020h, 02Dh, 000h, 001h, 000h
        jmp    NEAR PTR loc_15_4A90
loc_15_4B29:
        pop    ebx
        pop    edi
loc_15_4B2B:
        ret
sub_15_4B2C:
        test   byte ptr [edi], 80h
        je     SHORT loc_15_4B44
        test   byte ptr [edi + 2], 2
        je     SHORT loc_15_4B44
        call   sub_15_3E44
        and    byte ptr [edi], 7fh
        call   sub_15_4905
loc_15_4B44:
        ret
sub_15_4B45:
        push   edi
        push   ebx
loc_15_4B47:
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, dword ptr [esp]
        je     SHORT loc_15_4BB6
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        test   byte ptr [edi], 80h
        jne    SHORT loc_15_4B47
        test   byte ptr [edi + 2], 2
        je     SHORT loc_15_4B72
        test   byte ptr [edi + 4], 10h
        jne    SHORT loc_15_4B47
        call   sub_15_498E
        jmp    SHORT loc_15_4B47
loc_15_4B72:
        test   byte ptr [edi + 1], 20h
        jne    SHORT loc_15_4B8A
        cmp    byte ptr [edi + 5b3h], 56h
        je     SHORT loc_15_4B8A
        cmp    byte ptr [edi + 5b3h], 58h
        jne    SHORT loc_15_4B47
loc_15_4B8A:
        db  0CDh, 020h, 02Bh, 000h, 001h, 000h
        jb     SHORT loc_15_4B47
        or     byte ptr [edi], 80h
        or     byte ptr [edi + 9], 4
        db  080h, 00Dh, 0C4h, 006h, 000h, 000h, 010h
        test   byte ptr [edi + 1], 20h
        jne    SHORT loc_15_4B47
        cmp    byte ptr [edi + 5b3h], 56h
        je     SHORT loc_15_4B47
        db  0E8h, 067h, 0B9h, 0F0h, 0FFh
        jmp    SHORT loc_15_4B47
loc_15_4BB6:
        pop    ebx
        pop    edi
        ret
        db  000h, 000h, 000h
sub_15_4BBC:
        db  0F6h, 005h, 0C4h, 006h, 000h, 000h, 001h
        je     SHORT loc_15_4C23
        test   byte ptr [edi], 12h
        jne    SHORT loc_15_4C23
        db  03Bh, 01Dh, 0A8h, 007h, 000h, 000h
        jne    SHORT loc_15_4C23
        db  0F6h, 005h, 0C4h, 006h, 000h, 000h, 002h
        jne    SHORT loc_15_4C23
        db  03Bh, 01Dh, 0ACh, 007h, 000h, 000h
        jne    SHORT loc_15_4C23
        test   byte ptr [edi + 2], 2
        jne    SHORT loc_15_4C3D
        test   byte ptr [edi + 5e2h], 0eh
        je     SHORT loc_15_4C23
        test   byte ptr [edi + 7ch], 1
        jne    SHORT loc_15_4C03
        test   byte ptr [edi + 5e2h], 2
        jne    SHORT loc_15_4C3D
        jmp    SHORT loc_15_4C23
loc_15_4C03:
        test   byte ptr [edi + 6ah], 4
        je     SHORT loc_15_4C0F
        test   byte ptr [edi + 23h], 1
        jne    SHORT loc_15_4C1A
loc_15_4C0F:
        test   byte ptr [edi + 5e2h], 4
        jne    SHORT loc_15_4C3D
        jmp    SHORT loc_15_4C23
loc_15_4C1A:
        test   byte ptr [edi + 5e2h], 8
        jne    SHORT loc_15_4C3D
loc_15_4C23:
        or     byte ptr [edi + 2], 1
        db  0E8h, 0CCh, 0B9h, 0F0h, 0FFh
        mov    edx, 3c0h
loc_15_4C31:
        call   sub_15_4C7C
        inc    edx
        cmp    dl, 0e0h
        jb     SHORT loc_15_4C31
        ret
loc_15_4C3D:
        btr    dword ptr [edi], 10h
        jae    SHORT loc_15_4C7B
        mov    edx, 3c0h
        mov    dword ptr [edi + 4a7h], 0
loc_15_4C52:
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        inc    edx
        cmp    dl, 0e0h
        jb     SHORT loc_15_4C52
        btr    dword ptr [edi], 17h
        jae    SHORT loc_15_4C7B
        mov    dl, 0b4h
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        inc    edx
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        mov    dl, 0bah
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
loc_15_4C7B:
        ret
sub_15_4C7C:
        push   edx
        and    edx, 1fh
        db  00Fh, 0A3h, 015h, 014h, 010h, 000h, 000h
        jb     SHORT loc_15_4C99
loc_15_4C89:
        bts    dword ptr [edi + 4a7h], edx
        jb     SHORT loc_15_4CAF
        pop    edx
        db  0CDh, 020h, 098h, 080h, 001h, 000h
loc_15_4C99:
        db  03Bh, 01Dh, 0A8h, 007h, 000h, 000h
        jne    SHORT loc_15_4C89
        test   byte ptr [edi], 12h
        jne    SHORT loc_15_4C89
        db  0F6h, 005h, 0C4h, 006h, 000h, 000h, 002h
        jne    SHORT loc_15_4C89
loc_15_4CAF:
        pop    edx
        ret
        db  02Eh, 08Bh, 0C0h
loc_15_4CB4:
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        cmp    ecx, 4
        jbe    SHORT loc_15_4CC7
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_15_4CC7:
        clc
        pushfd
        db  03Bh, 01Dh, 0A8h, 007h, 000h, 000h
        jne    SHORT loc_15_4CDE
        db  0F6h, 005h, 0C4h, 006h, 000h, 000h, 001h
        je     SHORT loc_15_4CDE
        or     byte ptr [esp], 1
loc_15_4CDE:
        popfd
        pushfd
        jne    SHORT loc_15_4CF4
        test   byte ptr [edi], 80h
        jne    SHORT loc_15_4D0B
        db  03Bh, 01Dh, 0A8h, 007h, 000h, 000h
        jne    SHORT loc_15_4CF4
        call   sub_15_119E
loc_15_4CF4:
        mov    esi, edx
        sub    esi, 3c0h
        jns    SHORT loc_15_4D03
        mov    esi, 20h
loc_15_4D03:
        popfd
        db  0FFh, 024h, 0B5h, 018h, 010h, 000h, 000h
loc_15_4D0B:
        cmp    edx, 3d4h
        je     SHORT loc_15_4CF4
        cmp    edx, 3b4h
        je     SHORT loc_15_4CF4
        popfd
        ret
loc_15_4D1D:
        pushfd
        mov    esi, 14h
        cmp    dl, 0b4h
        je     SHORT loc_15_4D3C
        mov    esi, 15h
        cmp    dl, 0b5h
        je     SHORT loc_15_4D3C
        mov    esi, 1ah
        cmp    dl, 0bah
        jne    SHORT loc_15_4D44
loc_15_4D3C:
        popfd
        db  0FFh, 024h, 0B5h, 018h, 010h, 000h, 000h
loc_15_4D44:
        popfd
        mov    al, 0ffh
        ret
loc_15_4D48:
        in     al, dx
        ret
        db  08Bh, 0C0h
loc_15_4D4C:
        jne    SHORT loc_15_4D48
        out    dx, al
        ret
loc_15_4D50:
        jne    SHORT loc_15_4D6B
        jae    SHORT loc_15_4D79
        test   byte ptr [edi + 4ch], 80h
        je     SHORT loc_15_4D78
        db  0F6h, 005h, 0C4h, 006h, 000h, 000h, 002h
        je     SHORT loc_15_4D78
        mov    byte ptr [edi + 4ch], al
        and    al, 0dfh
        out    dx, al
        jmp    SHORT loc_15_4DAC
loc_15_4D6B:
        mov    al, byte ptr [edi + 4ch]
        and    al, 7fh
        ret
        db  02Eh, 08Bh, 0C0h
loc_15_4D74:
        jne    SHORT loc_15_4DAD
        jae    SHORT loc_15_4D79
loc_15_4D78:
        out    dx, al
loc_15_4D79:
        test   byte ptr [edi + 4ch], 80h
        jne    SHORT loc_15_4DA7
        movzx  ecx, byte ptr [edi + 4ch]
        test   cl, 10h
        jne    SHORT loc_15_4D8D
        test   cl, 20h
        jne    SHORT loc_15_4DA1
loc_15_4D8D:
        and    cl, 0dfh
        cmp    cl, 15h
        ja     SHORT loc_15_4DA1
        cmp    byte ptr [edi + 4ch], 0fh
        ja     SHORT loc_15_4D9D
        and    al, 3fh
loc_15_4D9D:
        mov    byte ptr [ecx + edi + 50h], al
loc_15_4DA1:
        or     byte ptr [edi + 4ch], 80h
        jmp    SHORT loc_15_4DAC
loc_15_4DA7:
        and    al, 7fh
        mov    byte ptr [edi + 4ch], al
loc_15_4DAC:
        ret
loc_15_4DAD:
        movzx  eax, byte ptr [edi + 4ch]
        and    al, 1fh
        cmp    al, 14h
        ja     SHORT loc_15_4DBC
        mov    al, byte ptr [eax + edi + 50h]
        ret
loc_15_4DBC:
        mov    al, 0ffh
        ret
        db  090h
loc_15_4DC0:
        jne    SHORT loc_15_4E09
        push   eax
        jb     SHORT loc_15_4DF1
        db  03Bh, 01Dh, 0ACh, 007h, 000h, 000h
        jne    SHORT loc_15_4DF2
        db  0F6h, 005h, 0C4h, 006h, 000h, 000h, 001h
        je     SHORT loc_15_4DF2
        db  08Bh, 035h, 0A8h, 007h, 000h, 000h
        db  003h, 035h, 0CCh, 006h, 000h, 000h
        mov    ah, byte ptr [esi + 96h]
        mov    ecx, 0dd22h
        and    eax, ecx
        or     al, ah
loc_15_4DF1:
        out    dx, al
loc_15_4DF2:
        pop    eax
        mov    byte ptr [edi + 96h], al
        test   byte ptr [edi + 4], 4
        jne    SHORT loc_15_4E08
        test   al, 1
        jne    SHORT loc_15_4E08
        db  0E8h, 0F0h, 0B7h, 0F0h, 0FFh
loc_15_4E08:
        ret
loc_15_4E09:
        jae    SHORT loc_15_4E0D
        in     al, dx
        ret
loc_15_4E0D:
        mov    al, byte ptr [edi + 9bh]
        or     al, al
        jne    SHORT loc_15_4E18
        in     al, dx
loc_15_4E18:
        xor    al, 9
        mov    byte ptr [edi + 9bh], al
        ret
        db  02Eh, 08Bh, 0C0h
loc_15_4E24:
        jne    SHORT loc_15_4E3C
        db  03Bh, 01Dh, 0ACh, 007h, 000h, 000h
        jne    SHORT loc_15_4E38
        db  0F6h, 005h, 0C4h, 006h, 000h, 000h, 001h
        je     SHORT loc_15_4E38
        out    dx, al
loc_15_4E38:
        mov    byte ptr [edi + 4dh], al
        ret
loc_15_4E3C:
        db  03Bh, 01Dh, 0ACh, 007h, 000h, 000h
        jne    SHORT loc_15_4E4F
        db  0F6h, 005h, 0C4h, 006h, 000h, 000h, 001h
        je     SHORT loc_15_4E4F
        in     al, dx
        ret
loc_15_4E4F:
        mov    al, byte ptr [edi + 4dh]
        ret
        db  090h
loc_15_4E54:
        movzx  ecx, byte ptr [edi + 4dh]
        jne    NEAR PTR loc_15_4EFC
        jae    SHORT loc_15_4E8D
        cmp    eax, eax
        push   edi
        pushfd
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 4ch], 0
        je     SHORT loc_15_4E75
        popfd
        call   dword ptr [edi + 4ch]
        jmp    SHORT loc_15_4E76
loc_15_4E75:
        popfd
loc_15_4E76:
        pop    edi
        jae    SHORT loc_15_4E7A
        ret
loc_15_4E7A:
        push   eax
        cmp    cl, 0fh
        ja     SHORT loc_15_4E8A
        xchg   dword ptr [esp], eax
        mov    byte ptr [ecx + edi + 66h], al
        xchg   dword ptr [esp], eax
loc_15_4E8A:
        out    dx, al
loc_15_4E8B:
        pop    eax
        ret
loc_15_4E8D:
        cmp    eax, eax
        push   edi
        pushfd
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 4ch], 0
        je     SHORT loc_15_4EA2
        popfd
        call   dword ptr [edi + 4ch]
        jmp    SHORT loc_15_4EA3
loc_15_4EA2:
        popfd
loc_15_4EA3:
        pop    edi
        jb     SHORT loc_15_4EFB
        push   eax
        cmp    cl, 0fh
        ja     SHORT loc_15_4E8B
        cmp    byte ptr [ecx + edi + 66h], al
        je     SHORT loc_15_4E8B
        db  08Bh, 035h, 0A8h, 007h, 000h, 000h
        db  003h, 035h, 0CCh, 006h, 000h, 000h
        mov    ah, byte ptr [ecx + esi + 66h]
        mov    esi, ebx
        db  08Ah, 099h, 0E4h, 00Fh, 000h, 000h
        or     bl, bl
        je     SHORT loc_15_4EF4
        and    al, bl
        not    bl
        and    ah, bl
        or     al, ah
        mov    ah, al
        xor    ah, byte ptr [ecx + edi + 66h]
        not    bl
        test   bl, ah
        je     SHORT loc_15_4EF4
        db  03Bh, 035h, 0ACh, 007h, 000h, 000h
        jne    SHORT loc_15_4EF4
        db  0F6h, 005h, 0C4h, 006h, 000h, 000h, 001h
        je     SHORT loc_15_4EF4
        out    dx, al
loc_15_4EF4:
        mov    ebx, esi
        pop    eax
        mov    byte ptr [ecx + edi + 66h], al
loc_15_4EFB:
        ret
loc_15_4EFC:
        jae    SHORT loc_15_4F08
        db  03Bh, 01Dh, 0ACh, 007h, 000h, 000h
        jne    SHORT loc_15_4F08
        in     al, dx
loc_15_4F07:
        ret
loc_15_4F08:
        cmp    eax, eax
        push   edi
        pushfd
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 48h], 0
        je     SHORT loc_15_4F1D
        popfd
        call   dword ptr [edi + 48h]
        jmp    SHORT loc_15_4F1E
loc_15_4F1D:
        popfd
loc_15_4F1E:
        pop    edi
        jb     SHORT loc_15_4F07
        mov    al, byte ptr [ecx + edi + 66h]
        jmp    SHORT loc_15_4F07
        db  090h
loc_15_4F28:
        jne    SHORT loc_15_4F57
        cmp    eax, eax
        push   edi
        pushfd
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 78h], 0
        je     SHORT loc_15_4F3F
        popfd
        call   dword ptr [edi + 78h]
        jmp    SHORT loc_15_4F40
loc_15_4F3F:
        popfd
loc_15_4F40:
        pop    edi
        jb     SHORT loc_15_4F71
        db  03Bh, 01Dh, 0A8h, 007h, 000h, 000h
        jne    SHORT loc_15_4F71
        db  0F6h, 005h, 0C4h, 006h, 000h, 000h, 001h
        je     SHORT loc_15_4F71
        out    dx, al
        jmp    SHORT loc_15_4F71
loc_15_4F57:
        cmp    eax, eax
        push   edi
        pushfd
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 7ch], 0
        je     SHORT loc_15_4F6C
        popfd
        call   dword ptr [edi + 7ch]
        jmp    SHORT loc_15_4F6D
loc_15_4F6C:
        popfd
loc_15_4F6D:
        pop    edi
        jb     SHORT loc_15_4F71
        in     al, dx
loc_15_4F71:
        ret
        db  08Bh, 0C0h
loc_15_4F74:
        jne    SHORT loc_15_4FC3
        cmp    eax, eax
        push   edi
        pushfd
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 78h], 0
        je     SHORT loc_15_4F8B
        popfd
        call   dword ptr [edi + 78h]
        jmp    SHORT loc_15_4F8C
loc_15_4F8B:
        popfd
loc_15_4F8C:
        pop    edi
        jb     SHORT loc_15_4FF5
        db  03Bh, 01Dh, 0A8h, 007h, 000h, 000h
        jne    SHORT loc_15_4FA1
        db  0F6h, 005h, 0C4h, 006h, 000h, 000h, 001h
        je     SHORT loc_15_4FA1
        out    dx, al
loc_15_4FA1:
        movzx  edx, al
        shl    edx, 2
        lea    edx, [edx + edi + 0a0h]
        mov    dword ptr [edi + 4a0h], edx
        mov    byte ptr [edi + 4a4h], 1
        mov    byte ptr [edi + 4a5h], 0
        ret
loc_15_4FC3:
        cmp    eax, eax
        push   edi
        pushfd
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 7ch], 0
        je     SHORT loc_15_4FD8
        popfd
        call   dword ptr [edi + 7ch]
        jmp    SHORT loc_15_4FD9
loc_15_4FD8:
        popfd
loc_15_4FD9:
        pop    edi
        jb     SHORT loc_15_4FF5
        db  03Bh, 01Dh, 0A8h, 007h, 000h, 000h
        jne    SHORT loc_15_4FEF
        db  0F6h, 005h, 0C4h, 006h, 000h, 000h, 001h
        je     SHORT loc_15_4FEF
        in     al, dx
        ret
loc_15_4FEF:
        mov    al, byte ptr [edi + 4a5h]
loc_15_4FF5:
        ret
        db  08Bh, 0C0h
loc_15_4FF8:
        jne    SHORT loc_15_5047
        cmp    eax, eax
        push   edi
        pushfd
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 78h], 0
        je     SHORT loc_15_500F
        popfd
        call   dword ptr [edi + 78h]
        jmp    SHORT loc_15_5010
loc_15_500F:
        popfd
loc_15_5010:
        pop    edi
        jb     SHORT loc_15_5087
        db  03Bh, 01Dh, 0A8h, 007h, 000h, 000h
        jne    SHORT loc_15_5025
        db  0F6h, 005h, 0C4h, 006h, 000h, 000h, 001h
        je     SHORT loc_15_5025
        out    dx, al
loc_15_5025:
        movzx  edx, al
        shl    edx, 2
        lea    edx, [edx + edi + 0a0h]
        mov    dword ptr [edi + 4a0h], edx
        mov    byte ptr [edi + 4a4h], 1
        mov    byte ptr [edi + 4a5h], 3
        ret
loc_15_5047:
        cmp    eax, eax
        push   edi
        pushfd
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 7ch], 0
        je     SHORT loc_15_505C
        popfd
        call   dword ptr [edi + 7ch]
        jmp    SHORT loc_15_505D
loc_15_505C:
        popfd
loc_15_505D:
        pop    edi
        jb     SHORT loc_15_5087
        db  03Bh, 01Dh, 0A8h, 007h, 000h, 000h
        jne    SHORT loc_15_5073
        db  0F6h, 005h, 0C4h, 006h, 000h, 000h, 001h
        je     SHORT loc_15_5073
        in     al, dx
        ret
loc_15_5073:
        mov    edx, dword ptr [edi + 4a0h]
        lea    eax, [edi + 0a0h]
        cmp    edx, eax
        mov    al, 0
        jbe    SHORT loc_15_5087
        mov    al, byte ptr [edx]
loc_15_5087:
        ret
loc_15_5088:
        jne    SHORT loc_15_50D8
        cmp    eax, eax
        push   edi
        pushfd
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 78h], 0
        je     SHORT loc_15_509F
        popfd
        call   dword ptr [edi + 78h]
        jmp    SHORT loc_15_50A0
loc_15_509F:
        popfd
loc_15_50A0:
        pop    edi
        jb     NEAR PTR loc_15_513D
        db  03Bh, 01Dh, 0A8h, 007h, 000h, 000h
        jne    SHORT loc_15_50B9
        db  0F6h, 005h, 0C4h, 006h, 000h, 000h, 001h
        je     SHORT loc_15_50B9
        out    dx, al
loc_15_50B9:
        mov    esi, dword ptr [edi + 4a0h]
        movzx  edx, byte ptr [edi + 4a4h]
        cmp    byte ptr [edi + 4a5h], 0
        je     SHORT loc_15_5114
        or     byte ptr [edi + 3], 10h
        mov    byte ptr [edx + esi], al
        jmp    SHORT loc_15_5114
loc_15_50D8:
        cmp    eax, eax
        push   edi
        pushfd
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 7ch], 0
        je     SHORT loc_15_50ED
        popfd
        call   dword ptr [edi + 7ch]
        jmp    SHORT loc_15_50EE
loc_15_50ED:
        popfd
loc_15_50EE:
        pop    edi
        jb     SHORT loc_15_513D
        db  03Bh, 01Dh, 0A8h, 007h, 000h, 000h
        jne    SHORT loc_15_5104
        db  0F6h, 005h, 0C4h, 006h, 000h, 000h, 001h
        je     SHORT loc_15_5104
        in     al, dx
        ret
loc_15_5104:
        mov    esi, dword ptr [edi + 4a0h]
        movzx  edx, byte ptr [edi + 4a4h]
        mov    al, byte ptr [edx + esi]
loc_15_5114:
        cmp    edx, 3
        jb     SHORT loc_15_5136
        xor    edx, edx
        add    esi, 4
        push   eax
        lea    eax, [edi + 4a0h]
        cmp    esi, eax
        pop    eax
        jb     SHORT loc_15_5130
        lea    esi, [edi + 0a0h]
loc_15_5130:
        mov    dword ptr [edi + 4a0h], esi
loc_15_5136:
        inc    edx
        mov    byte ptr [edi + 4a4h], dl
loc_15_513D:
        ret
        db  08Bh, 0C0h
loc_15_5140:
        jne    NEAR PTR loc_15_4D48
        jae    SHORT loc_15_5149
        out    dx, al
loc_15_5149:
        ret
        db  08Bh, 0C0h
loc_15_514C:
        jne    SHORT loc_15_5152
        jae    SHORT loc_15_5151
        out    dx, al
loc_15_5151:
        ret
loc_15_5152:
        jae    SHORT loc_15_5156
        in     al, dx
        ret
loc_15_5156:
        mov    al, byte ptr [edi + 96h]
        ret
        db  02Eh, 08Bh, 0C0h
loc_15_5160:
        pushfd
        db  03Bh, 01Dh, 0F0h, 00Eh, 000h, 000h
        jne    SHORT loc_15_5186
        db  0F7h, 005h, 09Ch, 010h, 000h, 000h, 000h, 000h, 000h, 080h
        je     SHORT loc_15_5186
        db  0F7h, 005h, 09Ch, 010h, 000h, 000h, 001h, 000h, 000h, 000h
        je     SHORT loc_15_5186
        call   sub_15_079E
loc_15_5186:
        popfd
        jne    SHORT loc_15_519F
        db  03Bh, 01Dh, 0ACh, 007h, 000h, 000h
        jne    SHORT loc_15_519B
        db  0F6h, 005h, 0C4h, 006h, 000h, 000h, 001h
        je     SHORT loc_15_519B
        out    dx, al
loc_15_519B:
        mov    byte ptr [edi + 4fh], al
        ret
loc_15_519F:
        db  03Bh, 01Dh, 0ACh, 007h, 000h, 000h
        jne    SHORT loc_15_51B2
        db  0F6h, 005h, 0C4h, 006h, 000h, 000h, 001h
        je     SHORT loc_15_51B2
        in     al, dx
        ret
loc_15_51B2:
        mov    al, byte ptr [edi + 4fh]
        ret
        db  08Bh, 0C0h
loc_15_51B8:
        pushfd
        db  03Bh, 01Dh, 0F0h, 00Eh, 000h, 000h
        jne    SHORT loc_15_51DE
        db  0F7h, 005h, 09Ch, 010h, 000h, 000h, 000h, 000h, 000h, 080h
        je     SHORT loc_15_51DE
        db  0F7h, 005h, 09Ch, 010h, 000h, 000h, 001h, 000h, 000h, 000h
        je     SHORT loc_15_51DE
        call   sub_15_079E
loc_15_51DE:
        popfd
        movzx  ecx, byte ptr [edi + 4fh]
        jne    NEAR PTR loc_15_52E1
        push   eax
        jae    SHORT loc_15_5245
        cmp    ecx, 6
        jne    SHORT loc_15_51FD
        test   dword ptr [edi], 4000h
        je     SHORT loc_15_51FD
        and    al, 0f3h
        or     al, 4
loc_15_51FD:
        pop    eax
        cmp    eax, eax
        push   edi
        pushfd
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 54h], 0
        je     SHORT loc_15_5213
        popfd
        call   dword ptr [edi + 54h]
        jmp    SHORT loc_15_5214
loc_15_5213:
        popfd
loc_15_5214:
        pop    edi
        jb     SHORT loc_15_5244
        push   eax
        cmp    cl, 1fh
        ja     SHORT loc_15_5242
        xchg   dword ptr [esp], eax
        cmp    cl, 6
        jne    SHORT loc_15_523B
        mov    esi, eax
        xor    al, byte ptr [edi + 7ch]
        test   al, 0ch
        mov    eax, esi
        je     SHORT loc_15_523B
        push   ecx
        push   edx
        push   eax
        call   sub_15_3A50
        pop    eax
        pop    edx
        pop    ecx
loc_15_523B:
        mov    byte ptr [ecx + edi + 76h], al
        xchg   dword ptr [esp], eax
loc_15_5242:
        out    dx, al
loc_15_5243:
        pop    eax
loc_15_5244:
        ret
loc_15_5245:
        pop    eax
        cmp    eax, eax
        push   edi
        pushfd
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 54h], 0
        je     SHORT loc_15_525B
        popfd
        call   dword ptr [edi + 54h]
        jmp    SHORT loc_15_525C
loc_15_525B:
        popfd
loc_15_525C:
        pop    edi
        jb     SHORT loc_15_5244
        push   eax
        cmp    cl, 1fh
        ja     SHORT loc_15_5243
        cmp    byte ptr [ecx + edi + 76h], al
        je     SHORT loc_15_5243
        db  08Bh, 035h, 0A8h, 007h, 000h, 000h
        db  003h, 035h, 0CCh, 006h, 000h, 000h
        mov    ah, byte ptr [ecx + esi + 76h]
        mov    esi, ebx
        db  08Ah, 099h, 0F4h, 00Fh, 000h, 000h
        or     bl, bl
        je     SHORT loc_15_52DA
        and    al, bl
        not    bl
        and    ah, bl
        or     al, ah
        mov    ah, al
        xor    ah, byte ptr [ecx + edi + 76h]
        not    bl
        test   bl, ah
        mov    ebx, esi
        je     SHORT loc_15_52DA
        db  03Bh, 01Dh, 0ACh, 007h, 000h, 000h
        jne    SHORT loc_15_52C0
        db  0F6h, 005h, 0C4h, 006h, 000h, 000h, 001h
        je     SHORT loc_15_52C0
        cmp    ecx, 6
        jne    SHORT loc_15_52BF
        test   dword ptr [edi], 4000h
        je     SHORT loc_15_52BF
        and    al, 0f3h
        or     al, 4
loc_15_52BF:
        out    dx, al
loc_15_52C0:
        mov    al, byte ptr [esp]
        cmp    cl, 6
        jne    SHORT loc_15_52D4
        test   ah, 0ch
        je     SHORT loc_15_52D4
        push   ecx
        call   sub_15_3A50
        pop    ecx
loc_15_52D4:
        pop    eax
        mov    byte ptr [ecx + edi + 76h], al
loc_15_52D9:
        ret
loc_15_52DA:
        pop    eax
        mov    byte ptr [ecx + edi + 76h], al
        jmp    SHORT loc_15_52D9
loc_15_52E1:
        jae    SHORT loc_15_52ED
        db  03Bh, 01Dh, 0ACh, 007h, 000h, 000h
        jne    SHORT loc_15_52ED
        in     al, dx
loc_15_52EC:
        ret
loc_15_52ED:
        cmp    eax, eax
        push   edi
        pushfd
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 50h], 0
        je     SHORT loc_15_5302
        popfd
        call   dword ptr [edi + 50h]
        jmp    SHORT loc_15_5303
loc_15_5302:
        popfd
loc_15_5303:
        pop    edi
        jb     SHORT loc_15_52EC
        mov    al, byte ptr [ecx + edi + 76h]
        jmp    SHORT loc_15_52EC
loc_15_530C:
        jne    SHORT loc_15_5315
        jae    SHORT loc_15_5311
        out    dx, al
loc_15_5311:
        mov    byte ptr [edi + 4eh], al
        ret
loc_15_5315:
        jae    SHORT loc_15_5319
        in     al, dx
        ret
loc_15_5319:
        mov    al, byte ptr [edi + 4eh]
        ret
        db  02Eh, 08Bh, 0C0h
loc_15_5320:
        jne    SHORT loc_15_5383
        movzx  ecx, byte ptr [edi + 4eh]
        cmp    eax, eax
        push   edi
        pushfd
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 44h], 0
        je     SHORT loc_15_533B
        popfd
        call   dword ptr [edi + 44h]
        jmp    SHORT loc_15_533C
loc_15_533B:
        popfd
loc_15_533C:
        pop    edi
        jb     SHORT loc_15_5382
        db  03Bh, 01Dh, 0A8h, 007h, 000h, 000h
        jne    SHORT loc_15_5379
        db  0F6h, 005h, 0C4h, 006h, 000h, 000h, 001h
        jne    SHORT loc_15_535F
        cmp    cl, 18h
        jbe    SHORT loc_15_5379
        dec    dl
        mov    ah, al
        mov    al, cl
        out    dx, ax
        jmp    SHORT loc_15_5382
loc_15_535F:
        cmp    cl, 0ch
        jb     SHORT loc_15_5378
        cmp    cl, 0dh
        ja     SHORT loc_15_5378
        cmp    byte ptr [ecx + edi + 0ch], al
        je     SHORT loc_15_5378
        mov    byte ptr [ecx + edi + 0ch], al
        call   sub_15_396E
loc_15_5378:
        out    dx, al
loc_15_5379:
        cmp    cl, 18h
        ja     SHORT loc_15_5382
        mov    byte ptr [ecx + edi + 0ch], al
loc_15_5382:
        ret
loc_15_5383:
        jae    SHORT loc_15_5387
        in     al, dx
        ret
loc_15_5387:
        movzx  ecx, byte ptr [edi + 4eh]
        cmp    eax, eax
        push   edi
        pushfd
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 40h], 0
        je     SHORT loc_15_53A0
        popfd
        call   dword ptr [edi + 40h]
        jmp    SHORT loc_15_53A1
loc_15_53A0:
        popfd
loc_15_53A1:
        pop    edi
        jb     SHORT loc_15_53B7
        cmp    cl, 18h
        jbe    SHORT loc_15_53B3
        dec    dl
        mov    al, cl
        out    dx, al
        inc    dl
        in     al, dx
        jmp    SHORT loc_15_53B7
loc_15_53B3:
        mov    al, byte ptr [ecx + edi + 0ch]
loc_15_53B7:
        ret
loc_15_53B8:
        jne    SHORT loc_15_53C4
        jae    SHORT loc_15_53BD
        out    dx, al
loc_15_53BD:
        mov    byte ptr [edi + 99h], al
        ret
loc_15_53C4:
        jae    SHORT loc_15_53CA
        or     byte ptr [edi + 4ch], 80h
loc_15_53CA:
        mov    al, byte ptr [edi + 9ch]
        cmp    byte ptr [edi + 5e0h], 0
        je     SHORT loc_15_53E1
        dec    byte ptr [edi + 5e0h]
        jmp    SHORT loc_15_5403
loc_15_53E1:
        dec    byte ptr [edi + 5e1h]
        je     SHORT loc_15_53ED
        xor    al, 9
        jmp    SHORT loc_15_53FD
loc_15_53ED:
        mov    byte ptr [edi + 5e0h], 0ch
        mov    byte ptr [edi + 5e1h], 0ffh
        or     al, 9
loc_15_53FD:
        mov    byte ptr [edi + 9ch], al
loc_15_5403:
        mov    cl, al
        in     al, dx
        and    al, 0f6h
        or     al, cl
        or     byte ptr [edi + 4ch], 80h
        ret
        db  090h
loc_15_5410:
        jne    NEAR PTR loc_15_4D48
        jae    SHORT loc_15_5419
        out    dx, al
loc_15_5419:
        ret
        db  000h, 000h
_PTEXT ends
        end
