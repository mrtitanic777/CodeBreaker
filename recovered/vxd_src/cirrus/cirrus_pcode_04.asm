; cirrus_pcode_04.asm -- CIRRUS object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_4_0000:
        db  080h, 03Dh, 0FFh, 000h, 000h, 000h, 000h
        je     SHORT loc_4_001F
        mov    edx, 3c4h
        in     al, dx
        ror    eax, 8
        mov    al, 7
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0fh
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
loc_4_001F:
        ret
loc_4_0020:
        db  080h, 03Dh, 0FFh, 000h, 000h, 000h, 000h
        je     SHORT loc_4_0047
        mov    edx, 3c4h
        in     al, dx
        ror    eax, 8
        mov    al, 7
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0fh
        db  08Ah, 025h, 0FFh, 000h, 000h, 000h
        or     al, ah
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
loc_4_0047:
        ret
loc_4_0048:
        db  0F7h, 005h, 0ECh, 000h, 000h, 000h, 000h, 000h, 002h, 000h
        je     SHORT loc_4_0061
        db  080h, 03Dh, 0F0h, 000h, 000h, 000h, 06Ah
        jl     NEAR PTR loc_4_00FC
loc_4_0061:
        db  0C6h, 005h, 0FEh, 000h, 000h, 000h, 0FFh
        db  0C6h, 005h, 003h, 001h, 000h, 000h, 0FFh
        mov    eax, ecx
        shr    eax, 10h
        or     cx, cx
        je     SHORT loc_4_0093
        db  08Bh, 015h, 0DCh, 000h, 000h, 000h
        shr    edx, 10h
        dec    dl
        cmp    al, dl
        jae    SHORT loc_4_00FC
        inc    al
        xor    cx, cx
        add    ecx, 10000h
loc_4_0093:
        db  0F7h, 005h, 0ECh, 000h, 000h, 000h, 000h, 000h, 002h, 000h
        jne    SHORT loc_4_00FC
        db  08Bh, 015h, 0DCh, 000h, 000h, 000h
        sub    edx, ecx
        cmp    edx, 10000h
        mov    edx, 10000h
        ja     SHORT loc_4_00B9
        mov    edx, 8000h
loc_4_00B9:
        db  0F7h, 005h, 0ECh, 000h, 000h, 000h, 003h, 000h, 000h, 000h
        jne    SHORT loc_4_00EF
        push   ecx
        mov    ecx, 4
        db  081h, 03Dh, 0DCh, 000h, 000h, 000h, 000h, 000h, 010h, 000h
        jle    SHORT loc_4_00DC
        mov    ecx, 2002h
loc_4_00DC:
        shl    al, cl
        db  088h, 02Dh, 002h, 001h, 000h, 000h
        shr    al, 2
        db  0A2h, 001h, 001h, 000h, 000h
        pop    ecx
        jmp    SHORT loc_4_00FE
loc_4_00EF:
        shl    al, 4
        shr    al, 2
        db  0A2h, 001h, 001h, 000h, 000h
        jmp    SHORT loc_4_00FE
loc_4_00FC:
        mov    edx, ecx
loc_4_00FE:
        xor    ah, ah
        ret
loc_4_0101:
        db  080h, 03Dh, 003h, 001h, 000h, 000h, 0FFh
        jne    SHORT loc_4_013F
        push   eax
        push   edx
        mov    edx, 3c4h
        in     al, dx
        ror    eax, 8
        mov    ax, 1206h
        out    dx, ax
        rol    eax, 8
        out    dx, al
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    al, 9
        out    dx, al
        inc    edx
        in     al, dx
        db  0A2h, 003h, 001h, 000h, 000h
        db  0A0h, 001h, 001h, 000h, 000h
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_4_013F:
        ret
loc_4_0140:
        db  080h, 03Dh, 003h, 001h, 000h, 000h, 0FFh
        jne    SHORT loc_4_0184
        push   eax
        push   edx
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    ax, 0ec0ah
        out    dx, ax
        rol    eax, 8
        out    dx, al
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    ax, 30dh
        out    dx, ax
        mov    al, 0eh
        out    dx, al
        inc    edx
        in     al, dx
        db  0A2h, 003h, 001h, 000h, 000h
        db  0A0h, 001h, 001h, 000h, 000h
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_4_0184:
        ret
loc_4_0185:
        db  080h, 03Dh, 003h, 001h, 000h, 000h, 0FFh
        je     SHORT loc_4_01D1
        push   eax
        push   edx
        mov    edx, 3c4h
        in     al, dx
        ror    eax, 8
        mov    ax, 1206h
        out    dx, ax
        rol    eax, 8
        out    dx, al
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    al, 9
        out    dx, al
        inc    edx
        in     al, dx
        db  03Ah, 005h, 001h, 001h, 000h, 000h
        jne    SHORT loc_4_01BF
        db  0A0h, 003h, 001h, 000h, 000h
        out    dx, al
loc_4_01BF:
        db  0C6h, 005h, 003h, 001h, 000h, 000h, 0FFh
        mov    edx, 3ceh
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_4_01D1:
        ret
loc_4_01D2:
        db  080h, 03Dh, 003h, 001h, 000h, 000h, 0FFh
        je     SHORT loc_4_021E
        push   eax
        push   edx
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    ax, 0ec0ah
        out    dx, ax
        rol    eax, 8
        out    dx, al
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    al, 0eh
        out    dx, al
        inc    edx
        in     al, dx
        db  03Ah, 005h, 001h, 001h, 000h, 000h
        jne    SHORT loc_4_020C
        db  0A0h, 003h, 001h, 000h, 000h
        out    dx, al
loc_4_020C:
        db  0C6h, 005h, 003h, 001h, 000h, 000h, 0FFh
        mov    edx, 3ceh
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_4_021E:
        ret
loc_4_021F:
        push   eax
        push   edx
        mov    edx, 3c4h
        in     al, dx
        ror    eax, 8
        mov    ax, 1206h
        out    dx, ax
        rol    eax, 8
        out    dx, al
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    al, 9
        out    dx, al
        inc    edx
        in     al, dx
        db  0A2h, 004h, 001h, 000h, 000h
        db  0A0h, 001h, 001h, 000h, 000h
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
        ret
loc_4_0255:
        push   eax
        push   edx
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    ax, 0ec0ah
        out    dx, ax
        rol    eax, 8
        out    dx, al
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    ax, 30dh
        out    dx, ax
        mov    al, 0eh
        out    dx, al
        inc    edx
        in     al, dx
        db  0A2h, 004h, 001h, 000h, 000h
        db  0A0h, 001h, 001h, 000h, 000h
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
        ret
loc_4_0291:
        push   eax
        push   edx
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    al, 9
        db  08Ah, 025h, 004h, 001h, 000h, 000h
        out    dx, ax
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
        ret
loc_4_02AD:
        push   eax
        push   edx
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    al, 0eh
        db  08Ah, 025h, 004h, 001h, 000h, 000h
        out    dx, ax
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
        ret
loc_4_02C9:
        db  080h, 00Dh, 0FDh, 000h, 000h, 000h, 002h
        db  0F7h, 005h, 0ECh, 000h, 000h, 000h, 0F0h, 0E3h, 01Fh, 000h
        je     SHORT loc_4_0323
        mov    edx, 3ceh
        mov    ax, 431h
        out    dx, ax
        mov    ah, 0
        out    dx, ax
        db  0F7h, 005h, 0ECh, 000h, 000h, 000h, 080h, 000h, 010h, 000h
        je     SHORT loc_4_0323
        mov    edx, 3d4h
        mov    eax, 3eh
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0feh
        out    dx, al
        db  0F7h, 005h, 0ECh, 000h, 000h, 000h, 000h, 080h, 001h, 000h
        je     SHORT loc_4_0323
        mov    edx, 3d4h
        mov    eax, 3ch
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0efh
        out    dx, al
loc_4_0323:
        ret
loc_4_0324:
        db  0C6h, 005h, 0FDh, 000h, 000h, 000h, 000h
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    ax, 9
        out    dx, ax
        mov    ax, 0ah
        out    dx, ax
        rol    eax, 8
        out    dx, al
        ret
loc_4_0345:
        db  080h, 00Dh, 0FDh, 000h, 000h, 000h, 001h
        ret
loc_4_034D:
        db  0C6h, 005h, 0FDh, 000h, 000h, 000h, 000h
        ret
loc_4_0355:
        push   edx
        push   edi
        mov    edx, 3c4h
        in     al, dx
        ror    eax, 8
        mov    ax, 1206h
        out    dx, ax
        rol    eax, 8
        out    dx, al
        mov    edi, ebx
        db  003h, 03Dh, 0E4h, 000h, 000h, 000h
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    al, 0bh
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 5], al
        xor    al, al
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi], al
        mov    al, 1
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 1], al
        mov    al, 5
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 2], al
        rol    eax, 8
        out    dx, al
        mov    edx, 3c4h
        in     al, dx
        ror    eax, 8
        mov    al, 2
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 6], al
        mov    al, 12h
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 0ah], al
        mov    byte ptr [edi + 10h], 0
        rol    eax, 8
        out    dx, al
        db  0F7h, 005h, 0ECh, 000h, 000h, 000h, 000h, 0E0h, 001h, 000h
        je     SHORT loc_4_044C
        mov    byte ptr [edi + 0fh], 0
        in     al, dx
        ror    eax, 8
        mov    al, 9
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 8], al
        mov    al, 0ah
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 9], al
        mov    al, 14h
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 0bh], al
        mov    al, 15h
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 0ch], al
        mov    al, 28h
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 0dh], al
        mov    al, 29h
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 0eh], al
        mov    al, 12h
        out    dx, al
        inc    edx
        in     al, dx
        ror    eax, 8
        or     al, 2
        out    dx, al
        mov    edx, 3c7h
        add    edi, 17h
        xor    ah, ah
        mov    ecx, 10h
        cld
loc_4_042A:
        mov    al, ah
        out    dx, al
        stosb
        add    edx, 2
        in     al, dx
        stosb
        in     al, dx
        stosb
        in     al, dx
        stosb
        sub    edx, 2
        inc    ah
        loop   loc_4_042A
        mov    edx, 3c5h
        rol    eax, 8
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
loc_4_044C:
        db  0C7h, 005h, 04Fh, 001h, 000h, 000h, 001h, 000h, 000h, 000h
        pop    edi
        pop    edx
        ret
loc_4_0459:
        push   edx
        push   edi
        mov    edi, ebx
        db  003h, 03Dh, 0E4h, 000h, 000h, 000h
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    ax, 0ec0ah
        out    dx, ax
        mov    al, 0a1h
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 2], al
        mov    al, 0dh
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 1], al
        mov    al, 0ah
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi], al
        db  0F7h, 005h, 0ECh, 000h, 000h, 000h, 002h, 000h, 000h, 000h
        je     SHORT loc_4_04AA
        mov    al, 0aeh
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 3], al
        mov    al, 0b0h
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 4], al
loc_4_04AA:
        rol    eax, 8
        out    dx, al
        pop    edi
        pop    edx
        ret
loc_4_04B1:
        db  0F7h, 005h, 0F1h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        je     SHORT loc_4_04D1
        db  083h, 03Dh, 0F9h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_04D1
        db  0C7h, 005h, 0F9h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        sti
loc_4_04D1:
        push   edx
        db  080h, 03Dh, 0FEh, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_4_0609
        mov    edx, 3c4h
        in     al, dx
        ror    eax, 8
        mov    ax, 1206h
        out    dx, ax
        rol    eax, 8
        out    dx, al
        mov    edx, 3d4h
        in     al, dx
        ror    eax, 8
        mov    al, 1bh
        out    dx, al
        inc    edx
        in     al, dx
        or     al, 2
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
        db  03Bh, 00Dh, 0D0h, 000h, 000h, 000h
        jne    SHORT loc_4_053F
        db  0A0h, 000h, 001h, 000h, 000h
        or     al, al
        je     SHORT loc_4_053F
        db  0F7h, 005h, 0F1h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        je     SHORT loc_4_052C
        pushfd
        cli
        call   sub_4_08EF
loc_4_052C:
        mov    edx, 3c2h
        out    dx, al
        db  0F7h, 005h, 0F1h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        je     SHORT loc_4_053F
        popfd
loc_4_053F:
        db  03Bh, 035h, 0D8h, 000h, 000h, 000h
        je     NEAR PTR loc_4_0609
        db  03Bh, 035h, 0D4h, 000h, 000h, 000h
        je     SHORT loc_4_05BB
        db  0F6h, 005h, 0FDh, 000h, 000h, 000h, 003h
        jne    NEAR PTR loc_4_0609
        db  03Bh, 035h, 0D0h, 000h, 000h, 000h
        jne    SHORT loc_4_05D4
        db  003h, 035h, 0E4h, 000h, 000h, 000h
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    al, 0bh
        mov    ah, byte ptr [esi + 5]
        out    dx, ax
        xor    al, al
        mov    ah, byte ptr [esi]
        out    dx, ax
        mov    al, 1
        mov    ah, byte ptr [esi + 1]
        out    dx, ax
        db  03Bh, 00Dh, 0D0h, 000h, 000h, 000h
        jne    SHORT loc_4_059A
        mov    al, 5
        mov    ah, byte ptr [esi + 2]
        out    dx, ax
loc_4_059A:
        rol    eax, 8
        out    dx, al
        mov    edx, 3c4h
        in     al, dx
        ror    eax, 8
        mov    al, 2
        mov    ah, byte ptr [esi + 6]
        out    dx, ax
        mov    al, 12h
        mov    ah, byte ptr [esi + 0ah]
        out    dx, ax
        rol    eax, 8
        out    dx, al
        jmp    SHORT loc_4_0609
loc_4_05BB:
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    al, 0bh
        db  08Ah, 025h, 002h, 001h, 000h, 000h
        out    dx, ax
        rol    eax, 8
        out    dx, al
        jmp    SHORT loc_4_0609
loc_4_05D4:
        db  003h, 035h, 0E4h, 000h, 000h, 000h
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    al, 0bh
        mov    ah, byte ptr [esi + 5]
        db  03Bh, 00Dh, 0D0h, 000h, 000h, 000h
        jne    SHORT loc_4_05F6
        db  08Ah, 025h, 002h, 001h, 000h, 000h
loc_4_05F6:
        out    dx, ax
        rol    eax, 8
        out    dx, al
        mov    edx, 3c4h
        in     al, dx
        ror    eax, 8
        rol    eax, 8
        out    dx, al
loc_4_0609:
        pop    edx
        ret
loc_4_060B:
        push   edx
        db  080h, 03Dh, 0FEh, 000h, 000h, 000h, 000h
        je     SHORT loc_4_0609
        db  083h, 03Dh, 04Fh, 001h, 000h, 000h, 001h
        jne    SHORT loc_4_0609
        mov    edx, 3c4h
        in     al, dx
        ror    eax, 8
        mov    ax, 1206h
        out    dx, ax
        rol    eax, 8
        out    dx, al
        db  03Bh, 00Dh, 0D0h, 000h, 000h, 000h
        jne    SHORT loc_4_064F
        mov    edx, 3d4h
        in     al, dx
        ror    eax, 8
        mov    al, 1bh
        out    dx, al
        inc    edx
        in     al, dx
        or     al, 2
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
loc_4_064F:
        db  03Bh, 00Dh, 0D0h, 000h, 000h, 000h
        jne    SHORT loc_4_0666
        db  0A0h, 000h, 001h, 000h, 000h
        or     al, al
        je     SHORT loc_4_0666
        mov    edx, 3c2h
        out    dx, al
loc_4_0666:
        db  03Bh, 035h, 0D8h, 000h, 000h, 000h
        je     NEAR PTR loc_4_07B7
        db  03Bh, 035h, 0D4h, 000h, 000h, 000h
        je     NEAR PTR loc_4_070F
        db  0F6h, 005h, 0FDh, 000h, 000h, 000h, 003h
        jne    NEAR PTR loc_4_07B7
        db  03Bh, 035h, 0D0h, 000h, 000h, 000h
        jne    NEAR PTR loc_4_072B
        db  003h, 035h, 0E4h, 000h, 000h, 000h
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    al, 0bh
        mov    ah, byte ptr [esi + 5]
        out    dx, ax
        xor    al, al
        mov    ah, byte ptr [esi]
        out    dx, ax
        mov    al, 1
        mov    ah, byte ptr [esi + 1]
        out    dx, ax
        mov    al, 5
        mov    ah, byte ptr [esi + 2]
        out    dx, ax
        rol    eax, 8
        out    dx, al
        mov    edx, 3c4h
        in     al, dx
        ror    eax, 8
        mov    al, 2
        mov    ah, byte ptr [esi + 6]
        out    dx, ax
        mov    al, 12h
        mov    ah, byte ptr [esi + 0ah]
        out    dx, ax
        mov    al, 9
        mov    ah, byte ptr [esi + 8]
        out    dx, ax
        mov    al, 0ah
        out    dx, al
        inc    edx
        in     al, dx
        or     al, 20h
        out    dx, al
        dec    edx
        mov    al, 14h
        mov    ah, byte ptr [esi + 0bh]
        mov    al, 15h
        mov    ah, byte ptr [esi + 0ch]
        out    dx, ax
        mov    al, 28h
        mov    ah, byte ptr [esi + 0dh]
        out    dx, ax
        mov    al, 29h
        mov    ah, byte ptr [esi + 0eh]
        out    dx, ax
        rol    eax, 8
        out    dx, al
        jmp    NEAR PTR loc_4_07B7
loc_4_070F:
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    al, 0bh
        db  08Ah, 025h, 002h, 001h, 000h, 000h
        out    dx, ax
        rol    eax, 8
        out    dx, al
        jmp    NEAR PTR loc_4_07B7
loc_4_072B:
        db  003h, 035h, 0E4h, 000h, 000h, 000h
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    al, 0bh
        mov    ah, byte ptr [esi + 5]
        db  03Bh, 00Dh, 0D0h, 000h, 000h, 000h
        jne    SHORT loc_4_074D
        db  08Ah, 025h, 002h, 001h, 000h, 000h
loc_4_074D:
        out    dx, ax
        rol    eax, 8
        out    dx, al
        mov    edx, 3c4h
        in     al, dx
        ror    eax, 8
        mov    al, 0ah
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0dfh
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
        mov    edx, 3c4h
        in     al, dx
        ror    eax, 8
        mov    al, 12h
        out    dx, al
        inc    edx
        in     al, dx
        ror    eax, 8
        or     al, 2
        out    dx, al
        mov    edx, 3c8h
        add    esi, 17h
        mov    ecx, 10h
loc_4_078A:
        mov    al, byte ptr [esi]
        and    al, 0fh
        cmp    al, 0bh
        jb     SHORT loc_4_07A4
        cmp    al, 0fh
        je     SHORT loc_4_07A4
        out    dx, al
        inc    edx
        mov    al, byte ptr [esi + 1]
        out    dx, al
        mov    al, byte ptr [esi + 2]
        out    dx, al
        mov    al, byte ptr [esi + 3]
        dec    edx
loc_4_07A4:
        add    esi, 4
        loop   loc_4_078A
        mov    edx, 3c5h
        rol    eax, 8
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
loc_4_07B7:
        pop    edx
        ret
loc_4_07B9:
        push   edx
        db  080h, 03Dh, 0FEh, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_4_0886
        db  03Bh, 00Dh, 0D0h, 000h, 000h, 000h
        jne    SHORT loc_4_07DE
        db  0A0h, 000h, 001h, 000h, 000h
        or     al, al
        je     SHORT loc_4_07DE
        mov    edx, 3c2h
        out    dx, al
loc_4_07DE:
        db  03Bh, 035h, 0D8h, 000h, 000h, 000h
        je     NEAR PTR loc_4_0886
        db  03Bh, 035h, 0D4h, 000h, 000h, 000h
        je     SHORT loc_4_0800
        db  0F6h, 005h, 0FDh, 000h, 000h, 000h, 003h
        je     SHORT loc_4_0830
        jmp    NEAR PTR loc_4_0886
loc_4_0800:
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    ax, 0ec0ah
        out    dx, ax
        mov    al, 0a1h
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0cfh
        out    dx, al
        dec    edx
        db  0F7h, 005h, 0ECh, 000h, 000h, 000h, 002h, 000h, 000h, 000h
        je     SHORT loc_4_082A
        mov    ax, 0b0h
        out    dx, ax
loc_4_082A:
        rol    eax, 8
        out    dx, al
        jmp    SHORT loc_4_0886
loc_4_0830:
        mov    ecx, esi
        db  003h, 035h, 0E4h, 000h, 000h, 000h
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    ax, 0ec0ah
        out    dx, ax
        mov    al, 0dh
        mov    ah, byte ptr [esi + 1]
        db  03Bh, 00Dh, 0D0h, 000h, 000h, 000h
        je     SHORT loc_4_0857
        or     ah, 3
loc_4_0857:
        out    dx, ax
        mov    al, 0a1h
        mov    ah, byte ptr [esi + 2]
        out    dx, ax
        db  0F7h, 005h, 0ECh, 000h, 000h, 000h, 002h, 000h, 000h, 000h
        je     SHORT loc_4_0882
        mov    al, 0b0h
        mov    ah, byte ptr [esi + 4]
        out    dx, ax
        db  03Bh, 00Dh, 0D0h, 000h, 000h, 000h
        jne    SHORT loc_4_0882
        mov    al, 0aeh
        mov    ah, byte ptr [esi + 3]
        out    dx, ax
loc_4_0882:
        rol    eax, 8
        out    dx, al
loc_4_0886:
        pop    edx
        ret
loc_4_0888:
        push   edx
        db  03Bh, 005h, 0D0h, 000h, 000h, 000h
        jne    SHORT loc_4_08C5
        mov    edx, 3c4h
        in     al, dx
        ror    eax, 8
        mov    ax, 1206h
        out    dx, ax
        rol    eax, 8
        out    dx, al
        mov    edx, 3ceh
        mov    ecx, 1000h
        mov    al, 31h
        out    dx, al
loc_4_08B1:
        in     ax, dx
        test   ah, 1
        je     SHORT loc_4_08C5
        call   sub_4_08E3
        loop   loc_4_08B1
        mov    al, 4
        out    dx, al
        mov    al, 0
        out    dx, al
loc_4_08C5:
        pop    edx
        db  0F7h, 005h, 0F1h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        je     SHORT loc_4_08E2
        cli
        db  0C7h, 005h, 0F9h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        call   sub_4_08EF
loc_4_08E2:
        ret
sub_4_08E3:
        push   ecx
        mov    ecx, 3e8h
loc_4_08E9:
        push   ecx
        pop    ecx
        loop   loc_4_08E9
        pop    ecx
        ret
sub_4_08EF:
        push   edx
        push   ecx
        push   eax
        mov    edx, 3cch
        in     al, dx
        mov    edx, 3dah
        test   al, 1
        jne    SHORT loc_4_0906
        mov    edx, 3bah
loc_4_0906:
        mov    ecx, 0ffffffffh
loc_4_090B:
        in     al, dx
        test   al, 1
        jne    SHORT loc_4_0912
        loop   loc_4_090B
loc_4_0912:
        mov    ecx, 0ffffffffh
loc_4_0917:
        in     al, dx
        test   al, 1
        je     SHORT loc_4_091E
        loop   loc_4_0917
loc_4_091E:
        pop    eax
        pop    ecx
        pop    edx
        ret
loc_4_0922:
        pushfd
        test   cl, 4
        je     SHORT loc_4_093A
        cli
        call   sub_4_08EF
        and    byte ptr [esp + 1], 0fdh
        popfd
        db  0FFh, 025h, 0F5h, 000h, 000h, 000h
loc_4_093A:
        popfd
        db  0FFh, 025h, 0F5h, 000h, 000h, 000h
loc_4_0941:
        cmp    cl, 12h
        jne    SHORT loc_4_095A
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        db  003h, 035h, 0E4h, 000h, 000h, 000h
        mov    byte ptr [esi + 0ah], al
        jmp    NEAR PTR loc_4_09FA
loc_4_095A:
        db  0F7h, 005h, 0ECh, 000h, 000h, 000h, 000h, 0E0h, 001h, 000h
        je     NEAR PTR loc_4_09FA
        cmp    cl, 9
        jne    SHORT loc_4_0984
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        db  003h, 035h, 0E4h, 000h, 000h, 000h
        mov    byte ptr [esi + 8], al
        stc
        jmp    NEAR PTR loc_4_0A26
loc_4_0984:
        cmp    cl, 0ah
        jne    SHORT loc_4_099E
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        db  003h, 035h, 0E4h, 000h, 000h, 000h
        mov    byte ptr [esi + 9], al
        stc
        jmp    NEAR PTR loc_4_0A26
loc_4_099E:
        cmp    cl, 14h
        jne    SHORT loc_4_09B5
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        db  003h, 035h, 0E4h, 000h, 000h, 000h
        mov    byte ptr [esi + 0bh], al
        stc
        jmp    SHORT loc_4_0A26
loc_4_09B5:
        cmp    cl, 15h
        jne    SHORT loc_4_09CC
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        db  003h, 035h, 0E4h, 000h, 000h, 000h
        mov    byte ptr [esi + 0ch], al
        stc
        jmp    SHORT loc_4_0A26
loc_4_09CC:
        cmp    cl, 28h
        jne    SHORT loc_4_09E3
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        db  003h, 035h, 0E4h, 000h, 000h, 000h
        mov    byte ptr [esi + 0dh], al
        stc
        jmp    SHORT loc_4_0A26
loc_4_09E3:
        cmp    cl, 29h
        jne    SHORT loc_4_09FA
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        db  003h, 035h, 0E4h, 000h, 000h, 000h
        mov    byte ptr [esi + 0eh], al
        stc
        jmp    SHORT loc_4_0A26
loc_4_09FA:
        db  0F7h, 005h, 0F1h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        je     SHORT loc_4_0A25
        cmp    cl, 1
        je     SHORT loc_4_0A1F
        cmp    cl, 0bh
        jb     SHORT loc_4_0A25
        cmp    cl, 0eh
        jbe    SHORT loc_4_0A1F
        cmp    cl, 1bh
        jb     SHORT loc_4_0A25
        cmp    cl, 1fh
        ja     SHORT loc_4_0A25
loc_4_0A1F:
        cli
        call   sub_4_08EF
loc_4_0A25:
        clc
loc_4_0A26:
        ret
loc_4_0A27:
        cmp    cl, 12h
        jne    SHORT loc_4_0A41
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        db  003h, 035h, 0E4h, 000h, 000h, 000h
        mov    al, byte ptr [esi + 0ah]
        stc
        jmp    NEAR PTR loc_4_0AE9
loc_4_0A41:
        db  0F7h, 005h, 0ECh, 000h, 000h, 000h, 000h, 0E0h, 001h, 000h
        je     NEAR PTR loc_4_0AE8
        cmp    cl, 6
        jne    SHORT loc_4_0A5E
        mov    al, 0fh
        stc
        jmp    NEAR PTR loc_4_0AE9
loc_4_0A5E:
        cmp    cl, 9
        jne    SHORT loc_4_0A75
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        db  003h, 035h, 0E4h, 000h, 000h, 000h
        mov    al, byte ptr [esi + 8]
        stc
        jmp    SHORT loc_4_0AE9
loc_4_0A75:
        cmp    cl, 0ah
        jne    SHORT loc_4_0A8C
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        db  003h, 035h, 0E4h, 000h, 000h, 000h
        mov    al, byte ptr [esi + 9]
        stc
        jmp    SHORT loc_4_0AE9
loc_4_0A8C:
        cmp    cl, 14h
        jne    SHORT loc_4_0AA3
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        db  003h, 035h, 0E4h, 000h, 000h, 000h
        mov    al, byte ptr [esi + 0bh]
        stc
        jmp    SHORT loc_4_0AE9
loc_4_0AA3:
        cmp    cl, 15h
        jne    SHORT loc_4_0ABA
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        db  003h, 035h, 0E4h, 000h, 000h, 000h
        mov    al, byte ptr [esi + 0ch]
        stc
        jmp    SHORT loc_4_0AE9
loc_4_0ABA:
        cmp    cl, 28h
        jne    SHORT loc_4_0AD1
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        db  003h, 035h, 0E4h, 000h, 000h, 000h
        mov    al, byte ptr [esi + 0dh]
        stc
        jmp    SHORT loc_4_0AE9
loc_4_0AD1:
        cmp    cl, 29h
        jne    SHORT loc_4_0AE8
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        db  003h, 035h, 0E4h, 000h, 000h, 000h
        mov    al, byte ptr [esi + 0eh]
        stc
        jmp    SHORT loc_4_0AE9
loc_4_0AE8:
        clc
loc_4_0AE9:
        ret
loc_4_0AEA:
        cmp    cl, 19h
        jb     SHORT loc_4_0B17
        cmp    cl, 1dh
        jne    SHORT loc_4_0B01
        push   ebx
        db  003h, 01Dh, 0E4h, 000h, 000h, 000h
        mov    byte ptr [ebx + 10h], al
        pop    ebx
        jmp    SHORT loc_4_0B15
loc_4_0B01:
        cmp    cl, 1bh
        jne    SHORT loc_4_0B08
        or     al, 2
loc_4_0B08:
        cmp    cl, 1eh
        je     SHORT loc_4_0B15
        dec    edx
        mov    ah, al
        mov    al, cl
        out    dx, ax
        inc    edx
loc_4_0B15:
        stc
        ret
loc_4_0B17:
        push   ebx
        db  003h, 01Dh, 0E4h, 000h, 000h, 000h
        test   byte ptr [ebx + 10h], 80h
        pop    ebx
        jne    SHORT loc_4_0B15
        clc
        ret
loc_4_0B27:
        cmp    cl, 19h
        jb     SHORT loc_4_0B52
        cmp    cl, 1dh
        jne    SHORT loc_4_0B3E
        push   ebx
        db  003h, 01Dh, 0E4h, 000h, 000h, 000h
        mov    al, byte ptr [ebx + 10h]
        pop    ebx
        jmp    SHORT loc_4_0B50
loc_4_0B3E:
        dec    edx
        in     al, dx
        ror    eax, 8
        mov    al, cl
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        rol    eax, 8
        out    dx, al
        inc    edx
        mov    al, ah
loc_4_0B50:
        stc
        ret
loc_4_0B52:
        clc
        ret
loc_4_0B54:
        cmp    cl, 7
        ja     SHORT loc_4_0B69
        push   ebx
        db  003h, 01Dh, 0E4h, 000h, 000h, 000h
        test   byte ptr [ebx + 0fh], 80h
        pop    ebx
        je     SHORT loc_4_0B79
        jmp    SHORT loc_4_0B7B
loc_4_0B69:
        cmp    cl, 11h
        jne    SHORT loc_4_0B79
        push   ebx
        db  003h, 01Dh, 0E4h, 000h, 000h, 000h
        mov    byte ptr [ebx + 0fh], al
        pop    ebx
loc_4_0B79:
        clc
        ret
loc_4_0B7B:
        stc
        ret
loc_4_0B7D:
        db  03Bh, 01Dh, 0D0h, 000h, 000h, 000h
        clc
        je     NEAR PTR loc_4_0C23
        cmp    edx, 3c6h
        jne    SHORT loc_4_0B98
        clc
        jmp    NEAR PTR loc_4_0C23
loc_4_0B98:
        cmp    edx, 3c7h
        jne    SHORT loc_4_0BBF
        push   esi
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        db  003h, 035h, 0E4h, 000h, 000h, 000h
        mov    byte ptr [esi + 11h], al
        mov    dword ptr [esi + 12h], 1
        mov    byte ptr [esi + 16h], 3
        pop    esi
        clc
        jmp    SHORT loc_4_0C23
loc_4_0BBF:
        cmp    edx, 3c8h
        jne    SHORT loc_4_0BE6
        push   esi
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        db  003h, 035h, 0E4h, 000h, 000h, 000h
        mov    byte ptr [esi + 11h], al
        mov    dword ptr [esi + 12h], 1
        mov    byte ptr [esi + 16h], 0
        pop    esi
        clc
        jmp    SHORT loc_4_0C23
loc_4_0BE6:
        push   esi
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        db  003h, 035h, 0E4h, 000h, 000h, 000h
        test   byte ptr [esi + 0ah], 2
        jne    SHORT loc_4_0BFD
        pop    esi
        clc
        jmp    SHORT loc_4_0C23
loc_4_0BFD:
        push   ebx
        movzx  ebx, byte ptr [esi + 11h]
        and    ebx, 0fh
        shl    ebx, 2
        add    ebx, dword ptr [esi + 12h]
        mov    byte ptr [ebx + esi + 17h], al
        mov    ebx, dword ptr [esi + 12h]
        cmp    ebx, 3
        jb     SHORT loc_4_0C1C
        xor    ebx, ebx
        inc    byte ptr [esi + 11h]
loc_4_0C1C:
        inc    ebx
        mov    dword ptr [esi + 12h], ebx
        pop    ebx
        pop    esi
        stc
loc_4_0C23:
        ret
loc_4_0C24:
        db  03Bh, 01Dh, 0D0h, 000h, 000h, 000h
        clc
        je     NEAR PTR loc_4_0CBB
        cmp    edx, 3c6h
        jne    SHORT loc_4_0C3C
        clc
        jmp    SHORT loc_4_0CBB
loc_4_0C3C:
        cmp    edx, 3c7h
        jne    SHORT loc_4_0C58
        push   esi
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        db  003h, 035h, 0E4h, 000h, 000h, 000h
        mov    al, byte ptr [esi + 16h]
        pop    esi
        stc
        jmp    SHORT loc_4_0CBB
loc_4_0C58:
        cmp    edx, 3c8h
        jne    SHORT loc_4_0C7E
        push   esi
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        db  003h, 035h, 0E4h, 000h, 000h, 000h
        test   byte ptr [esi + 0ah], 2
        jne    SHORT loc_4_0C77
        pop    esi
        clc
        jmp    SHORT loc_4_0CBB
loc_4_0C77:
        mov    al, byte ptr [esi + 11h]
        pop    esi
        stc
        jmp    SHORT loc_4_0CBB
loc_4_0C7E:
        push   esi
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        db  003h, 035h, 0E4h, 000h, 000h, 000h
        test   byte ptr [esi + 0ah], 2
        jne    SHORT loc_4_0C95
        pop    esi
        clc
        jmp    SHORT loc_4_0CBB
loc_4_0C95:
        push   ebx
        movzx  ebx, byte ptr [esi + 11h]
        and    ebx, 0fh
        shl    ebx, 2
        add    ebx, dword ptr [esi + 12h]
        mov    al, byte ptr [ebx + esi + 17h]
        mov    ebx, dword ptr [esi + 12h]
        cmp    ebx, 3
        jb     SHORT loc_4_0CB4
        xor    ebx, ebx
        inc    byte ptr [esi + 11h]
loc_4_0CB4:
        inc    ebx
        mov    dword ptr [esi + 12h], ebx
        pop    ebx
        pop    esi
        stc
loc_4_0CBB:
        ret
loc_4_0CBC:
        cmp    cl, 0ah
        je     SHORT loc_4_0CDE
        cmp    cl, 30h
        ja     SHORT loc_4_0CC9
        clc
        jmp    SHORT loc_4_0CF1
loc_4_0CC9:
        dec    edx
        in     al, dx
        ror    eax, 8
        mov    al, cl
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        rol    eax, 8
        out    dx, al
        inc    edx
        mov    al, ah
        stc
        jmp    SHORT loc_4_0CF1
loc_4_0CDE:
        push   esi
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        db  003h, 035h, 0E4h, 000h, 000h, 000h
        mov    al, byte ptr [esi]
        and    al, 1
        pop    esi
        stc
loc_4_0CF1:
        ret
loc_4_0CF2:
        cmp    cl, 0b0h
        je     SHORT loc_4_0D15
        cmp    cl, 0ah
        jne    SHORT loc_4_0D20
        mov    edi, ebx
        db  003h, 03Dh, 0E4h, 000h, 000h, 000h
        cmp    al, 0ech
        jne    SHORT loc_4_0D0B
        mov    byte ptr [edi], 1
loc_4_0D0B:
        cmp    al, 0ceh
        jne    SHORT loc_4_0D12
        mov    byte ptr [edi], 0
loc_4_0D12:
        stc
        jmp    SHORT loc_4_0D21
loc_4_0D15:
        mov    edi, ebx
        db  003h, 03Dh, 0E4h, 000h, 000h, 000h
        mov    byte ptr [edi + 4], al
loc_4_0D20:
        clc
loc_4_0D21:
        ret
loc_4_0D22:
        db  08Bh, 00Dh, 0DCh, 000h, 000h, 000h
        stc
        ret
loc_4_0D2A:
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    al, 9
        out    dx, al
        inc    dl
        in     al, dx
        dec    dl
        rol    eax, 8
        out    dx, al
        movzx  edx, ah
        stc
        ret
loc_4_0D44:
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
        jne    SHORT loc_4_0D60
        mov    al, 9
loc_4_0D60:
        out    dx, al
        inc    dl
        in     al, dx
        dec    dl
        rol    eax, 8
        out    dx, al
        movzx  edx, ah
        stc
        ret
loc_4_0D6F:
        mov    edx, 1000h
        db  081h, 03Dh, 0DCh, 000h, 000h, 000h, 000h, 000h, 010h, 000h
        jbe    SHORT loc_4_0D83
        shl    edx, 2
loc_4_0D83:
        xor    eax, eax
        stc
        ret
loc_4_0D87:
        push   edx
        push   eax
        push   ebx
        db  003h, 01Dh, 0E4h, 000h, 000h, 000h
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    al, 0bh
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [ebx + 5], al
        xor    al, al
        db  081h, 03Dh, 0DCh, 000h, 000h, 000h, 000h, 000h, 010h, 000h
        jbe    SHORT loc_4_0DB2
        mov    al, 20h
loc_4_0DB2:
        out    dx, al
        rol    eax, 8
        dec    dl
        out    dx, al
        pop    ebx
        pop    eax
        pop    edx
        stc
        ret
loc_4_0DBE:
        push   edx
        push   eax
        push   ebx
        db  003h, 01Dh, 0E4h, 000h, 000h, 000h
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    al, 0bh
        mov    ah, byte ptr [ebx + 5]
        out    dx, ax
        rol    eax, 8
        out    dx, al
        pop    ebx
        pop    eax
        pop    edx
        stc
        ret
loc_4_0DE0:
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
        rol    eax, 8
        out    dx, al
        pop    eax
        pop    edx
        pop    ecx
        stc
        ret
_PTEXT ends
        end
