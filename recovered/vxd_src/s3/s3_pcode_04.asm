; s3_pcode_04.asm -- S3 object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_4_0000:
        db  0C6h, 005h, 0DAh, 013h, 000h, 000h, 0FFh
        db  0C6h, 005h, 0DBh, 013h, 000h, 000h, 0FFh
        mov    eax, ecx
        shr    eax, 10h
        or     cx, cx
        je     SHORT loc_4_0036
        db  08Bh, 015h, 04Ch, 013h, 000h, 000h
        shr    edx, 10h
        dec    dl
        cmp    al, dl
        jae    NEAR PTR loc_4_00C6
        inc    al
        xor    cx, cx
        add    ecx, 10000h
loc_4_0036:
        db  08Bh, 015h, 04Ch, 013h, 000h, 000h
        sub    edx, ecx
        db  03Bh, 015h, 03Ch, 013h, 000h, 000h
        db  08Bh, 015h, 03Ch, 013h, 000h, 000h
        ja     SHORT loc_4_0051
        mov    edx, 8000h
loc_4_0051:
        db  080h, 03Dh, 0D8h, 013h, 000h, 000h, 000h
        je     SHORT loc_4_0081
        db  0A2h, 0DEh, 013h, 000h, 000h
        db  0C6h, 005h, 0DDh, 013h, 000h, 000h, 03Fh
        db  080h, 03Dh, 0D7h, 013h, 000h, 000h, 0E0h
        je     SHORT loc_4_00C8
        db  080h, 03Dh, 0D7h, 013h, 000h, 000h, 011h
        je     SHORT loc_4_00C8
        db  0C6h, 005h, 0DDh, 013h, 000h, 000h, 0FFh
        jmp    SHORT loc_4_00C8
loc_4_0081:
        push   edx
        push   ebx
        mov    bl, al
        mov    bh, al
        and    bl, 0fh
        shr    bh, 4
        mov    edx, 3d4h
        mov    al, 35h
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0f0h
        or     al, bl
        db  0A2h, 0DEh, 013h, 000h, 000h
        db  0C6h, 005h, 0DFh, 013h, 000h, 000h, 000h
        db  080h, 03Dh, 0D7h, 013h, 000h, 000h, 080h
        je     SHORT loc_4_00C2
        dec    edx
        mov    al, 51h
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0f3h
        shl    bh, 2
        or     al, bh
        db  0A2h, 0DFh, 013h, 000h, 000h
loc_4_00C2:
        pop    ebx
        pop    edx
        jmp    SHORT loc_4_00C8
loc_4_00C6:
        mov    edx, ecx
loc_4_00C8:
        xor    ah, ah
        ret
loc_4_00CB:
        db  080h, 03Dh, 0E1h, 013h, 000h, 000h, 0FFh
        jne    SHORT loc_4_0151
        db  080h, 03Dh, 0DBh, 013h, 000h, 000h, 000h
        je     SHORT loc_4_0151
        push   eax
        push   edx
        mov    edx, 3d4h
        in     al, dx
        ror    eax, 8
        mov    ax, 4838h
        out    dx, ax
        mov    ax, 0a539h
        out    dx, ax
        mov    al, 31h
        out    dx, al
        inc    edx
        in     al, dx
        db  0A2h, 0E0h, 013h, 000h, 000h
        or     al, 1
        out    dx, al
        dec    edx
        db  080h, 03Dh, 0D8h, 013h, 000h, 000h, 000h
        je     SHORT loc_4_0120
        mov    al, 6ah
        out    dx, al
        inc    dl
        in     al, dx
        db  0A2h, 0E1h, 013h, 000h, 000h
        db  0A0h, 0DEh, 013h, 000h, 000h
        out    dx, al
        dec    dl
        jmp    SHORT loc_4_014B
loc_4_0120:
        mov    al, 35h
        out    dx, al
        inc    edx
        in     al, dx
        db  0A2h, 0E1h, 013h, 000h, 000h
        db  0A0h, 0DEh, 013h, 000h, 000h
        out    dx, al
        dec    edx
        db  080h, 03Dh, 0D7h, 013h, 000h, 000h, 080h
        je     SHORT loc_4_014B
        mov    al, 51h
        out    dx, al
        inc    edx
        in     al, dx
        db  0A2h, 0E2h, 013h, 000h, 000h
        db  0A0h, 0DFh, 013h, 000h, 000h
        out    dx, al
        dec    edx
loc_4_014B:
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_4_0151:
        ret
loc_4_0152:
        db  080h, 03Dh, 0E1h, 013h, 000h, 000h, 0FFh
        je     NEAR PTR loc_4_0200
        push   eax
        push   edx
        mov    edx, 3d4h
        in     al, dx
        ror    eax, 8
        mov    ax, 4838h
        out    dx, ax
        mov    ax, 0a539h
        out    dx, ax
        db  080h, 03Dh, 0D8h, 013h, 000h, 000h, 000h
        je     SHORT loc_4_019B
        mov    al, 6ah
        out    dx, al
        inc    dl
        in     al, dx
        db  022h, 005h, 0DDh, 013h, 000h, 000h
        db  03Ah, 005h, 0DEh, 013h, 000h, 000h
        jne    SHORT loc_4_01EE
        db  0A0h, 0E1h, 013h, 000h, 000h
        out    dx, al
        jmp    SHORT loc_4_01EE
loc_4_019B:
        xor    ah, ah
        db  080h, 03Dh, 0D7h, 013h, 000h, 000h, 080h
        je     SHORT loc_4_01AE
        mov    al, 51h
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, al
        dec    edx
loc_4_01AE:
        mov    al, 35h
        out    dx, al
        inc    edx
        in     al, dx
        db  08Ah, 015h, 0DEh, 013h, 000h, 000h
        db  08Ah, 035h, 0DFh, 013h, 000h, 000h
        cmp    ax, dx
        jne    SHORT loc_4_01EE
        mov    edx, 3d5h
        db  0A0h, 0E1h, 013h, 000h, 000h
        out    dx, al
        dec    edx
        db  080h, 03Dh, 0D7h, 013h, 000h, 000h, 080h
        je     SHORT loc_4_01E4
        mov    al, 51h
        out    dx, al
        inc    edx
        db  0A0h, 0E2h, 013h, 000h, 000h
        out    dx, al
        dec    edx
loc_4_01E4:
        mov    al, 31h
        db  08Ah, 025h, 0E0h, 013h, 000h, 000h
        out    dx, ax
loc_4_01EE:
        db  0C6h, 005h, 0E1h, 013h, 000h, 000h, 0FFh
        mov    edx, 3d4h
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_4_0200:
        ret
loc_4_0201:
        push   eax
        push   edx
        db  080h, 03Dh, 0DBh, 013h, 000h, 000h, 000h
        je     SHORT loc_4_027C
        mov    edx, 3d4h
        in     al, dx
        ror    eax, 8
        mov    ax, 4838h
        out    dx, ax
        mov    ax, 0a539h
        out    dx, ax
        mov    al, 31h
        out    dx, al
        inc    edx
        in     al, dx
        db  0A2h, 0E3h, 013h, 000h, 000h
        or     al, 1
        out    dx, al
        dec    edx
        db  080h, 03Dh, 0D8h, 013h, 000h, 000h, 000h
        je     SHORT loc_4_024D
        mov    al, 6ah
        out    dx, al
        inc    dl
        in     al, dx
        db  0A2h, 0E4h, 013h, 000h, 000h
        db  0A0h, 0DEh, 013h, 000h, 000h
        out    dx, al
        dec    dl
        jmp    SHORT loc_4_0278
loc_4_024D:
        mov    al, 35h
        out    dx, al
        inc    edx
        in     al, dx
        db  0A2h, 0E4h, 013h, 000h, 000h
        db  0A0h, 0DEh, 013h, 000h, 000h
        out    dx, al
        dec    edx
        db  080h, 03Dh, 0D7h, 013h, 000h, 000h, 080h
        je     SHORT loc_4_0278
        mov    al, 51h
        out    dx, al
        inc    edx
        in     al, dx
        db  0A2h, 0E5h, 013h, 000h, 000h
        db  0A0h, 0DFh, 013h, 000h, 000h
        out    dx, al
        dec    edx
loc_4_0278:
        rol    eax, 8
        out    dx, al
loc_4_027C:
        pop    edx
        pop    eax
        ret
loc_4_027F:
        push   eax
        push   edx
        db  080h, 03Dh, 0DBh, 013h, 000h, 000h, 000h
        je     SHORT loc_4_02D4
        mov    edx, 3d4h
        in     al, dx
        ror    eax, 8
        mov    al, 31h
        db  08Ah, 025h, 0E3h, 013h, 000h, 000h
        out    dx, ax
        db  080h, 03Dh, 0D8h, 013h, 000h, 000h, 000h
        je     SHORT loc_4_02B2
        mov    al, 6ah
        db  08Ah, 025h, 0E4h, 013h, 000h, 000h
        out    dx, ax
        jmp    SHORT loc_4_02D0
loc_4_02B2:
        mov    al, 35h
        db  08Ah, 025h, 0E4h, 013h, 000h, 000h
        out    dx, ax
        db  080h, 03Dh, 0D7h, 013h, 000h, 000h, 080h
        je     SHORT loc_4_02D0
        mov    al, 51h
        out    dx, al
        inc    edx
        db  0A0h, 0E5h, 013h, 000h, 000h
        out    dx, al
        dec    edx
loc_4_02D0:
        rol    eax, 8
        out    dx, al
loc_4_02D4:
        pop    edx
        pop    eax
        ret
loc_4_02D7:
        db  080h, 00Dh, 0D9h, 013h, 000h, 000h, 002h
        db  0A1h, 05Ch, 013h, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_0324
        mov    word ptr [eax], 0
        mov    edx, 3d4h
        in     al, dx
        ror    eax, 8
        mov    ax, 0a539h
        out    dx, ax
        mov    al, 40h
        out    dx, al
        inc    dl
        in     al, dx
        or     al, 1
        and    al, 0f7h
        out    dx, al
        dec    dl
        rol    eax, 8
        out    dx, al
loc_4_030C:
        mov    edx, 9ae9h
        in     al, dx
        test   al, 2
        je     SHORT loc_4_0324
        cmp    al, 0ffh
        je     SHORT loc_4_0323
        mov    edx, 0e2e8h
        out    dx, ax
        jmp    SHORT loc_4_030C
loc_4_0323:
        int3
loc_4_0324:
        mov    edx, 4ae8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 4ae9h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        ret
loc_4_033B:
        mov    edx, 4ae8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 4ae9h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        db  0C6h, 005h, 0D9h, 013h, 000h, 000h, 000h
        db  03Bh, 01Dh, 048h, 013h, 000h, 000h
        je     SHORT loc_4_0365
        call   sub_4_0376
loc_4_0365:
        ret
loc_4_0366:
        db  080h, 00Dh, 0D9h, 013h, 000h, 000h, 001h
        ret
loc_4_036E:
        db  0C6h, 005h, 0D9h, 013h, 000h, 000h, 000h
        ret
sub_4_0376:
        db  080h, 03Dh, 0DBh, 013h, 000h, 000h, 000h
        je     SHORT loc_4_03D2
        push   edx
        push   edi
        mov    edi, ebx
        db  003h, 03Dh, 054h, 013h, 000h, 000h
        mov    edx, 3d4h
        in     al, dx
        ror    eax, 8
        mov    ax, 0a539h
        out    dx, ax
        mov    al, 40h
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 2], al
        mov    al, 42h
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 3], al
        db  080h, 03Dh, 0D7h, 013h, 000h, 000h, 080h
        je     SHORT loc_4_03C4
        mov    al, 53h
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 5], al
        mov    al, 58h
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [edi + 6], al
loc_4_03C4:
        mov    dl, 0d4h
        rol    eax, 8
        out    dx, al
        mov    dl, 0cch
        in     al, dx
        mov    byte ptr [edi + 0ch], al
        pop    edi
        pop    edx
loc_4_03D2:
        ret
loc_4_03D3:
        push   edx
        db  080h, 03Dh, 0DAh, 013h, 000h, 000h, 000h
        je     NEAR PTR loc_4_051D
        db  080h, 03Dh, 0DBh, 013h, 000h, 000h, 000h
        je     NEAR PTR loc_4_051D
        db  03Bh, 00Dh, 048h, 013h, 000h, 000h
        jne    SHORT loc_4_0405
        db  0B9h, 068h, 013h, 000h, 000h
        db  02Bh, 00Dh, 054h, 013h, 000h, 000h
        mov    esi, ecx
        jmp    SHORT loc_4_0418
loc_4_0405:
        db  03Bh, 035h, 044h, 013h, 000h, 000h
        jne    SHORT loc_4_0418
        db  0BEh, 078h, 013h, 000h, 000h
        db  02Bh, 035h, 054h, 013h, 000h, 000h
loc_4_0418:
        db  03Bh, 00Dh, 040h, 013h, 000h, 000h
        jne    SHORT loc_4_0431
        db  0A0h, 0E7h, 013h, 000h, 000h
        or     al, al
        je     SHORT loc_4_045D
        mov    edx, 3c2h
        out    dx, al
        jmp    SHORT loc_4_045D
loc_4_0431:
        db  003h, 00Dh, 054h, 013h, 000h, 000h
        mov    edx, 3c2h
        mov    al, byte ptr [ecx + 0ch]
        out    dx, al
        and    al, 0ch
        cmp    al, 0ch
        jne    SHORT loc_4_0457
        mov    dl, 0d4h
        in     al, dx
        ror    eax, 8
        mov    al, 42h
        mov    ah, byte ptr [ecx + 3]
        out    dx, ax
        rol    eax, 8
        out    dx, al
loc_4_0457:
        db  02Bh, 00Dh, 054h, 013h, 000h, 000h
loc_4_045D:
        db  080h, 03Dh, 0D7h, 013h, 000h, 000h, 080h
        je     NEAR PTR loc_4_051D
        db  03Bh, 035h, 040h, 013h, 000h, 000h
        jne    SHORT loc_4_04D4
        db  0F6h, 005h, 0D9h, 013h, 000h, 000h, 003h
        jne    SHORT loc_4_04D4
        mov    edx, 3d4h
        in     al, dx
        ror    eax, 8
        mov    ax, 0a539h
        out    dx, ax
        mov    al, 40h
        out    dx, al
        inc    edx
        in     al, dx
        db  08Bh, 03Dh, 05Ch, 013h, 000h, 000h
        mov    ah, byte ptr [edi]
        and    ax, 1feh
        or     al, ah
        out    dx, al
        dec    edx
        mov    al, 53h
        out    dx, al
        inc    edx
        in     al, dx
        db  08Bh, 03Dh, 060h, 013h, 000h, 000h
        mov    ah, byte ptr [edi]
        and    ax, 10efh
        or     al, ah
        out    dx, al
        dec    edx
        mov    al, 58h
        out    dx, al
        inc    edx
        in     al, dx
        db  08Bh, 03Dh, 05Ch, 013h, 000h, 000h
        mov    ah, byte ptr [edi]
        not    ah
        and    ax, 1efh
        shl    ah, 4
        or     al, ah
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
        jmp    SHORT loc_4_051D
loc_4_04D4:
        db  003h, 035h, 054h, 013h, 000h, 000h
        mov    edx, 3d4h
        in     al, dx
        ror    eax, 8
        mov    ax, 0a539h
        out    dx, ax
        mov    al, 40h
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, byte ptr [esi + 2]
        and    ax, 9f6h
        or     al, ah
        out    dx, al
        dec    edx
        mov    al, 53h
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, byte ptr [esi + 5]
        and    ax, 10efh
        or     al, ah
        out    dx, al
        dec    edx
        mov    al, 58h
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, byte ptr [esi + 6]
        and    ax, 10efh
        or     al, ah
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
loc_4_051D:
        pop    edx
        ret
loc_4_051F:
        db  080h, 03Dh, 0D7h, 013h, 000h, 000h, 080h
        je     SHORT loc_4_055F
        db  080h, 03Dh, 0DBh, 013h, 000h, 000h, 000h
        je     SHORT loc_4_055F
        mov    edx, 3d4h
        in     al, dx
        ror    eax, 8
        mov    ax, 0a539h
        out    dx, ax
        mov    ax, 0a5ah
        out    dx, ax
        mov    ax, 59h
        out    dx, ax
        mov    al, 58h
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0fch
        out    dx, al
        dec    edx
        mov    ax, 39h
        out    dx, ax
        rol    eax, 8
        out    dx, al
loc_4_055F:
        ret
loc_4_0560:
        db  080h, 03Dh, 0D7h, 013h, 000h, 000h, 080h
        je     SHORT loc_4_05C0
        db  080h, 03Dh, 0DBh, 013h, 000h, 000h, 000h
        je     SHORT loc_4_05C0
        db  080h, 03Dh, 0D6h, 013h, 000h, 000h, 000h
        jne    SHORT loc_4_05C0
        mov    edx, 3d4h
        in     al, dx
        ror    eax, 8
        mov    ax, 4838h
        out    dx, ax
        mov    ax, 0a539h
        out    dx, ax
        mov    al, 31h
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0feh
        out    dx, al
        dec    edx
        db  066h, 0A1h, 0D2h, 013h, 000h, 000h
        out    dx, ax
        db  066h, 0A1h, 0D4h, 013h, 000h, 000h
        out    dx, ax
        mov    al, 58h
        out    dx, al
        inc    edx
        in     al, dx
        db  08Ah, 025h, 0E6h, 013h, 000h, 000h
        and    ax, 3fch
        or     al, ah
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
loc_4_05C0:
        ret
loc_4_05C1:
        db  080h, 03Dh, 0DBh, 013h, 000h, 000h, 000h
        je     NEAR PTR loc_4_0689
        mov    edx, 9ae8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 9ae9h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0e2e8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0bee8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 96e8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 86e8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 82e8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 8ee8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 8ae8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0b6e8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0bae8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0aae8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0aee8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0a2e8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0a6e8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 92e8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0b2e8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
loc_4_0689:
        mov    edx, 4ae8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 4ae9h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        ret
sub_4_06A0:
        db  080h, 03Dh, 0DBh, 013h, 000h, 000h, 000h
        je     NEAR PTR loc_4_0768
        mov    edx, 9ae8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 9ae9h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0e2e8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0bee8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 96e8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 86e8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 82e8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 8ee8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 8ae8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0b6e8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0bae8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0aae8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0aee8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0a2e8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0a6e8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 92e8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0b2e8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
loc_4_0768:
        ret
loc_4_0769:
        push   edi
        push   esi
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        db  03Bh, 03Dh, 040h, 013h, 000h, 000h
        jne    SHORT loc_4_077D
        cmp    ebx, edi
        jne    SHORT loc_4_0787
loc_4_077D:
        cmp    ecx, 4
        je     SHORT loc_4_0786
        out    dx, ax
        jmp    SHORT loc_4_0787
loc_4_0786:
        out    dx, al
loc_4_0787:
        pop    esi
        pop    edi
        ret
loc_4_078A:
        push   edx
        db  03Bh, 005h, 040h, 013h, 000h, 000h
        jne    SHORT loc_4_07FE
        db  080h, 03Dh, 0DBh, 013h, 000h, 000h, 000h
        je     SHORT loc_4_07FE
        in     al, dx
        ror    eax, 8
        mov    ax, 4838h
        out    dx, ax
        mov    ax, 0a539h
        out    dx, ax
        mov    al, 40h
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        rol    eax, 8
        out    dx, al
        test   ah, 1
        je     SHORT loc_4_07FE
        db  080h, 03Dh, 0D7h, 013h, 000h, 000h, 080h
        je     SHORT loc_4_07DB
        test   ah, 8
        jne    SHORT loc_4_07FE
        ror    eax, 8
        mov    al, 58h
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        rol    eax, 8
        out    dx, al
        test   ah, 10h
        jne    SHORT loc_4_07FE
loc_4_07DB:
        db  0A1h, 058h, 013h, 000h, 000h
        cmp    byte ptr [eax], 0
        jne    SHORT loc_4_07FE
        mov    ecx, 1000h
loc_4_07EA:
        mov    edx, 9ae8h
        in     ax, dx
        test   ah, 2
        je     SHORT loc_4_07FE
        call   sub_4_0800
        dec    ecx
        jne    SHORT loc_4_07EA
loc_4_07FE:
        pop    edx
        ret
sub_4_0800:
        push   ecx
        mov    ecx, 3e8h
loc_4_0806:
        push   ecx
        pop    ecx
        loop   loc_4_0806
        pop    ecx
        ret
loc_4_080C:
        cmp    cl, 30h
        jae    SHORT loc_4_0814
        clc
        jmp    SHORT loc_4_084C
loc_4_0814:
        dec    dl
        in     al, dx
        mov    ch, al
        mov    ax, 4838h
        out    dx, ax
        mov    ax, 0a539h
        out    dx, ax
        cmp    cl, 40h
        jne    SHORT loc_4_0832
        mov    al, 40h
        out    dx, al
        inc    dl
        in     al, dx
        jmp    SHORT loc_4_083D
loc_4_0832:
        cmp    cl, 30h
        jne    SHORT loc_4_0842
        mov    al, 30h
        out    dx, al
        inc    dl
        in     al, dx
loc_4_083D:
        dec    dl
        stc
        jmp    SHORT loc_4_0843
loc_4_0842:
        clc
loc_4_0843:
        xchg   ch, al
        out    dx, al
        mov    dl, 0d5h
        mov    al, ch
        mov    ch, 0
loc_4_084C:
        ret
loc_4_084D:
        cmp    cl, 30h
        cmc
        jae    SHORT loc_4_08BD
        db  080h, 03Dh, 0DCh, 013h, 000h, 000h, 0FFh
        jb     SHORT loc_4_08BD
        dec    dl
        push   eax
        in     al, dx
        ror    eax, 8
        mov    ax, 4838h
        out    dx, ax
        mov    ax, 0a539h
        out    dx, ax
        rol    eax, 8
        out    dx, al
        pop    eax
        inc    dl
        cmp    cl, 40h
        jne    SHORT loc_4_0888
        mov    edi, ebx
        db  003h, 03Dh, 054h, 013h, 000h, 000h
        mov    byte ptr [edi + 2], al
        jmp    SHORT loc_4_08BC
loc_4_0888:
        cmp    cl, 42h
        jne    SHORT loc_4_089A
        mov    edi, ebx
        db  003h, 03Dh, 054h, 013h, 000h, 000h
        mov    byte ptr [edi + 3], al
        jmp    SHORT loc_4_08BC
loc_4_089A:
        cmp    cl, 53h
        jne    SHORT loc_4_08AC
        mov    edi, ebx
        db  003h, 03Dh, 054h, 013h, 000h, 000h
        mov    byte ptr [edi + 5], al
        jmp    SHORT loc_4_08BC
loc_4_08AC:
        cmp    cl, 58h
        jne    SHORT loc_4_08BC
        mov    edi, ebx
        db  003h, 03Dh, 054h, 013h, 000h, 000h
        mov    byte ptr [edi + 6], al
loc_4_08BC:
        clc
loc_4_08BD:
        ret
loc_4_08BE:
        pushfd
        test   cl, 4
        je     SHORT loc_4_08CF
        push   ebx
        db  003h, 01Dh, 054h, 013h, 000h, 000h
        mov    byte ptr [ebx + 0ch], al
        pop    ebx
loc_4_08CF:
        popfd
        db  0FFh, 025h, 064h, 013h, 000h, 000h
loc_4_08D6:
        db  08Bh, 00Dh, 04Ch, 013h, 000h, 000h
        stc
        ret
loc_4_08DE:
        mov    edx, 3d4h
        in     al, dx
        ror    eax, 8
        db  080h, 03Dh, 0D8h, 013h, 000h, 000h, 000h
        je     SHORT loc_4_08FC
        mov    al, 6ah
        out    dx, al
        inc    dl
        in     al, dx
        dec    dl
        or     al, al
        jne    SHORT loc_4_0920
loc_4_08FC:
        xor    ah, ah
        db  080h, 03Dh, 0D7h, 013h, 000h, 000h, 080h
        je     SHORT loc_4_0911
        mov    al, 51h
        out    dx, al
        inc    dl
        in     al, dx
        mov    ah, al
        dec    dl
loc_4_0911:
        mov    al, 35h
        out    dx, al
        inc    dl
        in     al, dx
        and    ax, 0c0fh
        shl    ah, 2
        or     al, ah
loc_4_0920:
        mov    dl, 0d4h
        rol    eax, 8
        out    dx, al
        movzx  edx, ah
        stc
        ret
loc_4_092B:
        mov    edx, 10000h
        xor    eax, eax
        stc
        ret
loc_4_0934:
        push   esi
        push   eax
        push   edx
        mov    esi, ebx
        db  003h, 035h, 054h, 013h, 000h, 000h
        mov    edx, 3d4h
        in     al, dx
        ror    eax, 8
        mov    ax, 4838h
        out    dx, ax
        mov    ax, 0a539h
        out    dx, ax
        mov    al, 31h
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi], al
        or     al, 1
        out    dx, al
        dec    dl
        mov    al, 35h
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi + 1], al
        dec    dl
        db  080h, 03Dh, 0D7h, 013h, 000h, 000h, 080h
        je     SHORT loc_4_09DF
        mov    al, 51h
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi + 4], al
        out    dx, al
        dec    dl
        db  080h, 03Dh, 0D8h, 013h, 000h, 000h, 000h
        je     SHORT loc_4_0995
        mov    al, 6ah
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi + 9], al
        dec    dl
loc_4_0995:
        mov    al, 40h
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi + 2], al
        and    al, 0feh
        or     al, 8
        out    dx, al
        dec    dl
        mov    al, 53h
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi + 5], al
        and    al, 0efh
        out    dx, al
        dec    dl
        mov    al, 59h
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi + 7], al
        xor    al, al
        out    dx, al
        dec    dl
        mov    al, 5ah
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi + 8], al
        mov    al, 0ah
        out    dx, al
        dec    dl
        mov    al, 58h
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi + 6], al
        and    al, 0fch
        or     al, 10h
        out    dx, al
        dec    dl
loc_4_09DF:
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
        pop    esi
        stc
        ret
loc_4_09E8:
        push   esi
        push   eax
        push   edx
        mov    esi, ebx
        db  003h, 035h, 054h, 013h, 000h, 000h
        mov    edx, 3d4h
        in     al, dx
        ror    eax, 8
        mov    ax, 4838h
        out    dx, ax
        mov    ax, 0a539h
        out    dx, ax
        mov    al, 31h
        mov    ah, byte ptr [esi]
        out    dx, ax
        mov    al, 35h
        mov    ah, byte ptr [esi + 1]
        out    dx, ax
        db  080h, 03Dh, 0D7h, 013h, 000h, 000h, 080h
        je     SHORT loc_4_0A58
        mov    al, 51h
        mov    ah, byte ptr [esi + 4]
        out    dx, ax
        db  080h, 03Dh, 0D8h, 013h, 000h, 000h, 000h
        je     SHORT loc_4_0A35
        mov    al, 6ah
        mov    ah, byte ptr [esi + 9]
        out    dx, ax
loc_4_0A35:
        mov    al, 40h
        mov    ah, byte ptr [esi + 2]
        out    dx, ax
        mov    al, 53h
        mov    ah, byte ptr [esi + 5]
        out    dx, ax
        mov    al, 59h
        mov    ah, byte ptr [esi + 7]
        out    dx, ax
        mov    al, 5ah
        mov    ah, byte ptr [esi + 8]
        out    dx, ax
        mov    al, 58h
        mov    ah, byte ptr [esi + 6]
        out    dx, ax
loc_4_0A58:
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
        pop    esi
        stc
        ret
loc_4_0A61:
        push   edx
        push   eax
        push   ecx
        mov    cl, dl
        mov    edx, 3d4h
        in     al, dx
        ror    eax, 8
        mov    ax, 4838h
        out    dx, ax
        mov    ax, 0a539h
        out    dx, ax
        mov    al, 35h
        mov    ah, cl
        and    ah, 0fh
        out    dx, ax
        db  080h, 03Dh, 0D7h, 013h, 000h, 000h, 080h
        je     SHORT loc_4_0AB8
        mov    al, 51h
        out    dx, al
        inc    dl
        in     al, dx
        and    al, 0f3h
        mov    ah, cl
        and    ah, 30h
        shr    ah, 2
        or     al, ah
        out    dx, al
        dec    dl
        db  080h, 03Dh, 0D8h, 013h, 000h, 000h, 000h
        je     SHORT loc_4_0AB8
        mov    al, 6ah
        mov    ah, cl
        test   cl, 0c0h
        jne    SHORT loc_4_0AB6
        xor    ah, ah
loc_4_0AB6:
        out    dx, ax
loc_4_0AB8:
        rol    eax, 8
        out    dx, al
        pop    ecx
        pop    eax
        pop    edx
        stc
        ret
loc_4_0AC1:
        push   eax
        mov    eax, 86h
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    eax
        clc
        ret
        db  020h, 053h
_PTEXT ends
        end
