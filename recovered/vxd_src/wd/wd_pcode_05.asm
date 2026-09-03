; wd_pcode_05.asm -- WD object 5 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_5_0000:
        mov    edx, 3c4h
        in     al, dx
        mov    ah, al
        mov    al, 1
        out    dx, al
        inc    dl
        in     al, dx
        and    al, 0dfh
        out    dx, al
        dec    dl
        mov    al, ah
        out    dx, al
        ret
loc_5_0017:
        db  080h, 03Dh, 058h, 001h, 000h, 000h, 000h
        je     SHORT loc_5_0020
loc_5_0020:
        ret
loc_5_0021:
        push   ebx
        db  080h, 03Dh, 059h, 001h, 000h, 000h, 000h
        jne    SHORT loc_5_003D
        mov    eax, 7fh
        mov    ebx, 2301h
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
loc_5_003D:
        pop    ebx
        ret
sub_5_003F:
        push   ebx
        db  080h, 03Dh, 059h, 001h, 000h, 000h, 001h
        ja     SHORT loc_5_005E
        mov    eax, 7fh
        mov    bh, 23h
        db  08Ah, 01Dh, 059h, 001h, 000h, 000h
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
loc_5_005E:
        pop    ebx
        ret
loc_5_0060:
        pushfd
        cmp    cl, 2ah
        jne    SHORT loc_5_007B
        popfd
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
        mov    al, ah
        stc
        jmp    SHORT loc_5_007C
loc_5_007B:
        popfd
loc_5_007C:
        ret
loc_5_007D:
        mov    edi, ebx
        db  003h, 03Dh, 050h, 001h, 000h, 000h
        cmp    cl, 32h
        jne    SHORT loc_5_009B
        mov    ah, byte ptr [edi + 2]
        and    ah, 0a0h
        cmp    ah, 0a0h
        jne    SHORT loc_5_00A4
        or     byte ptr [edi + 1], 1
        jmp    SHORT loc_5_00A0
loc_5_009B:
        cmp    cl, 34h
        jne    SHORT loc_5_00A4
loc_5_00A0:
        mov    byte ptr [ecx + edi - 32h], al
loc_5_00A4:
        clc
        ret
loc_5_00A6:
        mov    edi, ebx
        db  003h, 03Dh, 050h, 001h, 000h, 000h
        cmp    cl, 27h
        jne    SHORT loc_5_00BA
        or     byte ptr [edi + 1], 2
        mov    byte ptr [edi + 3], al
loc_5_00BA:
        clc
        ret
loc_5_00BC:
        pushfd
        cmp    cl, 7
        je     SHORT loc_5_00CC
        cmp    cl, 8
        je     SHORT loc_5_00CC
        cmp    cl, 9
        jne    SHORT loc_5_00E7
loc_5_00CC:
        popfd
        dec    edx
        in     al, dx
        ror    eax, 8
        mov    ax, 4806h
        out    dx, ax
        mov    al, cl
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        rol    eax, 8
        out    dx, al
        mov    al, ah
        stc
        jmp    SHORT loc_5_00E8
loc_5_00E7:
        popfd
loc_5_00E8:
        ret
loc_5_00E9:
        mov    edx, 23c0h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 23c1h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 23c2h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 23c3h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 23c4h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 23c5h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        ret
loc_5_012C:
        mov    edx, 23c0h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 23c1h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 23c2h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 23c3h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 23c4h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 23c5h
        ret
loc_5_0169:
        push   esi
        push   edi
        db  03Bh, 01Dh, 04Ch, 001h, 000h, 000h
        je     SHORT loc_5_0180
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        cmp    edi, ebx
        je     SHORT loc_5_0180
        stc
        jmp    SHORT loc_5_0181
loc_5_0180:
        clc
loc_5_0181:
        pop    edi
        pop    esi
        ret
_PTEXT ends
        end
