; combuff_pcode_03.asm -- COMBUFF object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        test   eax, eax
        jne    NEAR PTR loc_3_008E
        push   edx
        mov    edx, dword ptr [esi + 18h]
        test   edx, edx
        je     SHORT loc_3_003A
        cmp    byte ptr [esi + 7eh], al
        jne    SHORT loc_3_003A
        mov    byte ptr [esi + 7eh], 0ffh
        add    dl, 2
        in     al, dx
        mov    ah, al
        mov    al, 1
        out    dx, al
        jmp    SHORT loc_3_0024
loc_3_0024:
        in     al, dx
        and    al, 0c0h
        cmp    al, 0c0h
        jne    SHORT loc_3_0036
        and    ah, al
        cmp    ah, al
        je     SHORT loc_3_003A
        xor    al, al
        out    dx, al
        jmp    SHORT loc_3_003A
loc_3_0036:
        mov    byte ptr [esi + 7eh], 1
loc_3_003A:
        pop    edx
        xchg   edx, ebx
        or     ebx, ebx
        je     SHORT loc_3_005C
        mov    eax, dword ptr [esi + 20h]
        or     eax, eax
        je     SHORT loc_3_005C
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        test   ecx, 80h
        je     SHORT loc_3_005C
        db  0CDh, 020h, 003h, 000h, 003h, 000h
loc_3_005C:
        mov    ebx, edx
        test   ebx, ebx
        je     SHORT loc_3_008E
        call   sub_3_0256
        mov    eax, dword ptr [esi + 20h]
        or     eax, eax
        je     SHORT loc_3_007C
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        test   ecx, 10h
        je     SHORT loc_3_0081
loc_3_007C:
        call   sub_3_0216
loc_3_0081:
        lea    edi, [esi + 74h]
        mov    esi, dword ptr [esi]
        lea    esi, [ebx + esi + 2]
        cld
        movsw
        movsb
loc_3_008E:
        clc
        ret
        db  0EBh, 006h, 0FFh, 025h, 000h, 000h, 000h, 000h
loc_3_0098:
        push   eax
        or     eax, ecx
        pop    eax
        jne    SHORT loc_3_00C3
        mov    ebx, 1
        db  0BFh, 0F4h, 001h, 000h, 000h
        add    eax, 413h
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        jb     SHORT loc_3_00C2
        mov    word ptr [esi + 85h], 200h
        mov    byte ptr [esi + 7dh], 0
loc_3_00C2:
        ret
loc_3_00C3:
        db  0FFh, 025h, 000h, 000h, 000h, 000h
        db  02Eh, 08Bh, 0C0h
loc_3_00CC:
        cli
        test   byte ptr [esi + 75h], 80h
        jne    SHORT loc_3_00E5
        mov    al, byte ptr [esi + 79h]
        and    byte ptr [esi + 78h], 0feh
        and    byte ptr [esi + 7bh], 0feh
        db  0E8h, 051h, 000h, 0FDh, 0FFh
        sti
        ret
loc_3_00E5:
        in     al, dx
        sti
        ret
loc_3_00E8:
        cli
        test   byte ptr [esi + 75h], 80h
        jne    SHORT loc_3_00FD
        and    byte ptr [esi + 7bh], 0fdh
        cmp    al, 13h
        je     SHORT loc_3_0115
        cmp    byte ptr [esi + 7ch], 0ffh
        je     SHORT loc_3_0100
loc_3_00FD:
        out    dx, al
        sti
        ret
loc_3_0100:
        mov    byte ptr [esi + 7ch], 0
        push   eax
        push   edx
        db  0E8h, 029h, 000h, 0FDh, 0FFh
        db  0E8h, 06Eh, 0FFh, 0FCh, 0FFh
        pop    edx
        pop    eax
        out    dx, al
        sti
        ret
loc_3_0115:
        cmp    byte ptr [esi + 7dh], 1
        jne    SHORT loc_3_011F
        mov    byte ptr [esi + 7ch], 0ffh
loc_3_011F:
        out    dx, al
        sti
        ret
        db  08Bh, 0C0h
loc_3_0124:
        cli
        test   byte ptr [esi + 75h], 80h
        jne    SHORT loc_3_00E5
        mov    al, byte ptr [esi + 74h]
        sti
        ret
loc_3_0130:
        cli
        test   byte ptr [esi + 75h], 80h
        jne    SHORT loc_3_00FD
        xchg   byte ptr [esi + 74h], al
        out    dx, al
        jmp    SHORT loc_3_013D
loc_3_013D:
        mov    al, byte ptr [esi + 74h]
        or     al, 1
        out    dx, al
        sti
        ret
loc_3_0145:
        cli
        movzx  eax, byte ptr [esi + 7bh]
        and    al, byte ptr [esi + 74h]
        and    al, 0fh
        db  08Ah, 080h, 044h, 002h, 000h, 000h
        test   byte ptr [esi + 77h], 1
        je     SHORT loc_3_015D
        mov    ah, 0c0h
loc_3_015D:
        cmp    al, 2
        je     SHORT loc_3_0165
        or     al, ah
        sti
        ret
loc_3_0165:
        or     al, ah
        and    byte ptr [esi + 7bh], 0fdh
        sti
        ret
loc_3_016D:
        cli
        cmp    byte ptr [esi + 7eh], 0ffh
        jne    SHORT loc_3_0184
        out    dx, al
        test   al, 1
        je     SHORT loc_3_0180
        and    al, 0c1h
        mov    byte ptr [esi + 77h], al
        sti
        ret
loc_3_0180:
        mov    byte ptr [esi + 77h], 0
loc_3_0184:
        sti
        ret
loc_3_0186:
        cli
        mov    al, byte ptr [esi + 75h]
        sti
        ret
loc_3_018C:
        cli
        mov    byte ptr [esi + 75h], al
        sti
        out    dx, al
        ret
loc_3_0193:
        cli
        mov    al, byte ptr [esi + 76h]
        sti
        ret
loc_3_0199:
        cli
        xchg   byte ptr [esi + 76h], al
        out    dx, al
        jmp    SHORT loc_3_01A0
loc_3_01A0:
        mov    al, byte ptr [esi + 76h]
        or     al, 8
        out    dx, al
        test   al, 1
        je     NEAR PTR sub_3_0256
        sti
        ret
loc_3_01B0:
        cli
        in     al, dx
        test   al, 1fh
        je     SHORT loc_3_01C1
        push   eax
        and    al, 1fh
        db  0E8h, 0FDh, 0FEh, 0FCh, 0FFh
        pop    eax
        and    al, 60h
loc_3_01C1:
        mov    bl, byte ptr [esi + 78h]
        or     al, bl
        and    bl, 1
        mov    byte ptr [esi + 78h], bl
        and    byte ptr [esi + 7bh], 0fbh
        sti
        ret
loc_3_01D2:
        cli
        and    al, 1fh
        mov    byte ptr [esi + 78h], al
        and    byte ptr [esi + 7bh], 0fah
        test   al, 1
        je     SHORT loc_3_01E4
        or     byte ptr [esi + 7bh], 1
loc_3_01E4:
        test   al, 1eh
        je     SHORT loc_3_01EC
        or     byte ptr [esi + 7bh], 4
loc_3_01EC:
        sti
        ret
loc_3_01EE:
        cli
        in     al, dx
        or     al, byte ptr [esi + 7ah]
        mov    byte ptr [esi + 7ah], 0
        and    byte ptr [esi + 7bh], 0f7h
        sti
        ret
loc_3_01FD:
        cli
        out    dx, al
        mov    byte ptr [esi + 7ah], 0
        sti
        ret
loc_3_0205:
        jecxz  loc_3_022A
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        test   ecx, 2
        je     SHORT sub_3_0216
        ret
sub_3_0216:
        pushad
        mov    edx, dword ptr [esi + 18h]
        mov    ecx, 7
loc_3_021F:
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        inc    edx
        loop   loc_3_021F
        popad
        ret
loc_3_022A:
        pushad
        mov    edx, dword ptr [esi + 18h]
        inc    edx
        in     al, dx
        mov    byte ptr [esi + 74h], al
        add    dl, 2
        jmp    SHORT loc_3_0238
loc_3_0238:
        in     al, dx
        mov    byte ptr [esi + 75h], al
        inc    edx
        jmp    SHORT loc_3_023F
loc_3_023F:
        in     al, dx
        mov    byte ptr [esi + 76h], al
        sub    dl, 4
        mov    ecx, 7
loc_3_024B:
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        inc    edx
        loop   loc_3_024B
        popad
        ret
sub_3_0256:
        mov    word ptr [esi + 83h], 0
        mov    word ptr [esi + 7fh], 0
        mov    word ptr [esi + 81h], 0
        and    byte ptr [esi + 78h], 0feh
        and    byte ptr [esi + 7bh], 0feh
        mov    byte ptr [esi + 7ch], 0
        ret
_PTEXT ends
        end
