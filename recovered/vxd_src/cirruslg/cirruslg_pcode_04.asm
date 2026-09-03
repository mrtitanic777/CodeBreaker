; cirruslg_pcode_04.asm -- CIRRUSLG object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_4_0000:
        db  0C6h, 005h, 0E0h, 001h, 000h, 000h, 0FFh
        db  0C6h, 005h, 0EDh, 001h, 000h, 000h, 0FFh
        db  083h, 03Dh, 0E9h, 001h, 000h, 000h, 000h
        je     SHORT loc_4_002B
        mov    ecx, 4
        db  0C6h, 005h, 0E8h, 001h, 000h, 000h, 000h
        db  08Bh, 015h, 0E9h, 001h, 000h, 000h
        jmp    SHORT loc_4_002D
loc_4_002B:
        mov    edx, ecx
loc_4_002D:
        xor    ah, ah
        ret
loc_4_0030:
        db  080h, 03Dh, 0EDh, 001h, 000h, 000h, 0FFh
        jne    SHORT loc_4_005B
        push   eax
        push   edx
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    al, 9
        out    dx, al
        inc    edx
        in     al, dx
        db  0A2h, 0EDh, 001h, 000h, 000h
        db  0A0h, 0E8h, 001h, 000h, 000h
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_4_005B:
        ret
loc_4_005C:
        db  080h, 03Dh, 0EDh, 001h, 000h, 000h, 0FFh
        je     SHORT loc_4_0095
        push   eax
        push   edx
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    al, 9
        out    dx, al
        inc    edx
        in     al, dx
        db  03Ah, 005h, 0E8h, 001h, 000h, 000h
        jne    SHORT loc_4_0083
        db  0A0h, 0EDh, 001h, 000h, 000h
        out    dx, al
loc_4_0083:
        db  0C6h, 005h, 0EDh, 001h, 000h, 000h, 0FFh
        mov    edx, 3ceh
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_4_0095:
        ret
loc_4_0096:
        push   eax
        push   edx
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    al, 9
        out    dx, al
        inc    edx
        in     al, dx
        db  0A2h, 0EEh, 001h, 000h, 000h
        db  0A0h, 0E8h, 001h, 000h, 000h
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
        ret
loc_4_00B9:
        push   eax
        push   edx
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    al, 9
        db  08Ah, 025h, 0EEh, 001h, 000h, 000h
        out    dx, ax
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
        ret
loc_4_00D5:
        db  080h, 00Dh, 0DCh, 001h, 000h, 000h, 002h
        db  0C6h, 005h, 0E8h, 001h, 000h, 000h, 040h
        push   esi
        push   edi
        push   ecx
        db  08Bh, 035h, 0D0h, 001h, 000h, 000h
        and    word ptr [esi + 0e6h], 0fffeh
        mov    word ptr [esi + 0c0h], 0
        and    word ptr [esi + 0f0h], 0efffh
        mov    eax, dword ptr [esi + 3fch]
        and    eax, 0efffffffh
        mov    dword ptr [esi + 3f4h], eax
        mov    dword ptr [esi + 3fch], eax
        pop    ecx
        pop    edi
        pop    esi
        ret
loc_4_0121:
        push   edx
        push   eax
        push   edi
        db  0C6h, 005h, 0DCh, 001h, 000h, 000h, 000h
        mov    edi, ebx
        db  003h, 03Dh, 0C4h, 001h, 000h, 000h
        mov    al, byte ptr [edi]
        or     al, al
        je     SHORT loc_4_014E
        mov    edx, 3c4h
        in     al, dx
        ror    eax, 8
        mov    al, 15h
        out    dx, al
        inc    edx
        mov    al, byte ptr [edi]
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
loc_4_014E:
        db  08Bh, 015h, 0D0h, 001h, 000h, 000h
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebx, dword ptr [ebx + 8]
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    byte ptr [ebx + 1dh], 12h
        mov    byte ptr [ebx + 10h], 0b5h
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    ax, word ptr [ebx + 1ch]
        and    ah, 3fh
        mov    al, byte ptr [edx + 0eah]
        and    al, 0c0h
        and    al, 7fh
        or     ah, al
        call   sub_4_04D9
        mov    byte ptr [edx + 0eah], ah
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    ebx
        pop    edi
        pop    eax
        pop    edx
        ret
loc_4_01BE:
        db  080h, 00Dh, 0DCh, 001h, 000h, 000h, 001h
        db  0C6h, 005h, 0E8h, 001h, 000h, 000h, 000h
        ret
loc_4_01CD:
        db  0C6h, 005h, 0DCh, 001h, 000h, 000h, 000h
        ret
loc_4_01D5:
        push   edx
        push   edi
        mov    edi, ebx
        db  003h, 03Dh, 0C4h, 001h, 000h, 000h
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    al, 0bh
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [edi + 3], al
        dec    edx
        mov    al, 9
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [edi + 1], al
        dec    edx
        mov    al, 0ah
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [edi + 2], al
        dec    edx
        rol    eax, 8
        out    dx, al
        db  0C7h, 005h, 0D8h, 001h, 000h, 000h, 001h, 000h, 000h, 000h
        pop    edi
        pop    edx
        ret
loc_4_0214:
        push   edx
        db  080h, 03Dh, 0E0h, 001h, 000h, 000h, 000h
        je     SHORT loc_4_0285
        db  083h, 03Dh, 0D8h, 001h, 000h, 000h, 001h
        jne    SHORT loc_4_0285
        db  03Bh, 00Dh, 0ACh, 001h, 000h, 000h
        jne    SHORT loc_4_023E
        db  0A0h, 0E4h, 001h, 000h, 000h
        or     al, al
        je     SHORT loc_4_023E
        mov    edx, 3c2h
        out    dx, al
loc_4_023E:
        db  03Bh, 035h, 0B8h, 001h, 000h, 000h
        je     SHORT loc_4_0285
        db  03Bh, 035h, 0B4h, 001h, 000h, 000h
        je     SHORT loc_4_0285
        db  0F6h, 005h, 0DCh, 001h, 000h, 000h, 003h
        jne    SHORT loc_4_0285
        db  003h, 035h, 0C4h, 001h, 000h, 000h
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    al, 0bh
        out    dx, al
        inc    edx
        mov    al, byte ptr [esi + 3]
        out    dx, al
        dec    edx
        mov    al, 9
        out    dx, al
        inc    edx
        mov    al, byte ptr [esi + 1]
        out    dx, al
        dec    edx
        mov    al, 0ah
        out    dx, al
        inc    edx
        mov    al, byte ptr [esi + 2]
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
loc_4_0285:
        pop    edx
        ret
loc_4_0287:
        mov    ecx, 100000h
        stc
        ret
loc_4_028E:
        push   ebx
        db  003h, 01Dh, 0C4h, 001h, 000h, 000h
        mov    edx, 3c4h
        in     al, dx
        ror    eax, 8
        mov    al, 15h
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [ebx], al
        dec    edx
        rol    eax, 8
        out    dx, al
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    al, 0ah
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [ebx + 2], al
        dec    dl
        mov    al, 9
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [ebx + 1], al
        pop    ebx
        dec    dl
        rol    eax, 8
        out    dx, al
        movzx  edx, ah
        stc
        ret
loc_4_02D3:
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    al, 0bh
        out    dx, al
        inc    dl
        in     al, dx
        mov    ah, al
        dec    dl
        mov    al, 0ah
        test   ah, 1
        jne    SHORT loc_4_02EF
        mov    al, 9
loc_4_02EF:
        out    dx, al
        inc    dl
        in     al, dx
        dec    dl
        rol    eax, 8
        out    dx, al
        movzx  edx, ah
        stc
        ret
loc_4_02FE:
        push   ecx
        push   ebx
        xor    eax, eax
        mov    edx, 1000h
        pop    ebx
        pop    ecx
        stc
        ret
loc_4_030B:
        push   edx
        push   eax
        push   ebx
        db  003h, 01Dh, 0C4h, 001h, 000h, 000h
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    al, 0bh
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [ebx + 3], al
        xor    al, al
        out    dx, al
        rol    eax, 8
        dec    dl
        out    dx, al
        pop    ebx
        pop    eax
        pop    edx
        stc
        ret
loc_4_0334:
        push   edx
        push   eax
        push   ebx
        db  003h, 01Dh, 0C4h, 001h, 000h, 000h
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    al, 0bh
        mov    ah, byte ptr [ebx + 3]
        out    dx, ax
        rol    eax, 8
        out    dx, al
        pop    ebx
        pop    eax
        pop    edx
        stc
        ret
loc_4_0356:
        push   ecx
        push   edx
        push   eax
        mov    ecx, eax
        mov    ah, dl
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    ah, al
        mov    al, 9
        out    dx, ax
        mov    al, 0bh
        out    dx, al
        inc    dl
        in     al, dx
        dec    dl
        test   al, 1
        je     SHORT loc_4_037E
        mov    al, 0ah
        mov    ah, cl
        out    dx, ax
loc_4_037E:
        rol    eax, 8
        out    dx, al
        pop    eax
        pop    edx
        pop    ecx
        stc
        ret
loc_4_0387:
        push   edx
        push   eax
        mov    al, byte ptr [ebp + 1ch]
        cmp    al, 8
        jne    SHORT loc_4_039E
        mov    byte ptr [ebp + 11h], 6
        mov    word ptr [ebp + 1ch], 4fh
        pop    eax
        pop    edx
        stc
        ret
loc_4_039E:
        cmp    al, 2
        jbe    SHORT loc_4_03A6
        cmp    al, 6
        jne    SHORT loc_4_03C8
loc_4_03A6:
        mov    edx, 3c4h
        in     al, dx
        ror    eax, 8
        mov    al, 14h
        out    dx, al
        inc    dl
        in     al, dx
        mov    ah, al
        and    ax, 7f8h
        out    dx, al
        db  088h, 025h, 0DDh, 001h, 000h, 000h
        rol    eax, 8
        dec    dl
        out    dx, al
loc_4_03C8:
        pop    eax
        pop    edx
        clc
        ret
loc_4_03CC:
        push   eax
        push   edx
        cmp    dl, 2
        jbe    SHORT loc_4_03DC
        cmp    dl, 6
        jne    NEAR PTR loc_4_04D6
loc_4_03DC:
        mov    dx, 3c4h
        in     al, dx
        ror    eax, 8
        mov    al, 14h
        out    dx, al
        inc    dl
        in     al, dx
        db  00Ah, 005h, 0DDh, 001h, 000h, 000h
        out    dx, al
        rol    eax, 8
        dec    dl
        out    dx, al
        mov    dl, byte ptr [esp]
        cmp    dl, 0
        jne    SHORT loc_4_0415
        movzx  eax, word ptr [ebp + 38h]
        movzx  edx, word ptr [ebp]
        shl    eax, 4
        add    eax, edx
        and    byte ptr [eax + 0ah], 0feh
        jmp    NEAR PTR loc_4_04D6
loc_4_0415:
        cmp    dl, 2
        jne    NEAR PTR loc_4_04D6
        cmp    word ptr [ebp + 1ch], 4fh
        jne    NEAR PTR loc_4_04D6
        push   edi
        push   esi
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        pop    esi
        cmp    ebx, edi
        pop    edi
        jne    NEAR PTR loc_4_04D6
        db  08Bh, 015h, 0D0h, 001h, 000h, 000h
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    byte ptr [ebp + 1dh], 12h
        mov    byte ptr [ebp + 10h], 0b5h
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    ax, word ptr [ebp + 1ch]
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        or     ah, 80h
        cmp    word ptr [esp + 2], 13h
        jbe    SHORT loc_4_0490
        and    ah, 7fh
loc_4_0490:
        call   sub_4_04D9
        mov    byte ptr [edx + 0eah], ah
        mov    byte ptr [edx + 0c1h], al
        shr    al, 3
        and    al, 10h
        and    byte ptr [edx + 0f1h], 10h
        or     byte ptr [edx + 0f1h], al
        mov    eax, dword ptr [edx + 3fch]
        and    eax, 0efffffffh
        test   byte ptr [esp + 3], 40h
        je     SHORT loc_4_04CA
        or     eax, 10000000h
loc_4_04CA:
        mov    dword ptr [edx + 3f4h], eax
        mov    dword ptr [edx + 3fch], eax
loc_4_04D6:
        pop    edx
        pop    eax
        ret
sub_4_04D9:
        push   ax
        push   cx
        push   dx
        xor    cx, cx
        mov    dx, 3cch
        in     al, dx
        mov    dx, 3dah
        test   al, 1
        jne    SHORT loc_4_04F3
        mov    dx, 3bah
loc_4_04F3:
        in     al, dx
        test   al, 8
        loopne loc_4_04F3
        xor    cx, cx
loc_4_04FB:
        in     al, dx
        test   al, 8
        loope  loc_4_04FB
        pop    dx
        pop    cx
        pop    ax
        ret
_PTEXT ends
        end
