; chips_pcode_05.asm -- CHIPS object 5 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_5_0000:
        db  080h, 03Dh, 0EDh, 000h, 000h, 000h, 0FFh
        jne    SHORT loc_5_0044
        push   eax
        push   edx
        mov    edx, 3d6h
        in     al, dx
        ror    eax, 8
        mov    al, 10h
        out    dx, al
        inc    edx
        in     al, dx
        db  0A2h, 0EDh, 000h, 000h, 000h
        db  0A0h, 0EBh, 000h, 000h, 000h
        out    dx, al
        dec    edx
        mov    al, 0ch
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, al
        and    ax, 0cf30h
        db  0A2h, 0EEh, 000h, 000h, 000h
        db  0A0h, 0ECh, 000h, 000h, 000h
        or     al, ah
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_5_0044:
        and    al, 0fh
        db  002h, 005h, 0F1h, 000h, 000h, 000h
        ret
loc_5_004D:
        db  080h, 03Dh, 0EDh, 000h, 000h, 000h, 0FFh
        je     SHORT loc_5_00B1
        push   eax
        push   edx
        mov    edx, 3d6h
        in     al, dx
        ror    eax, 8
        mov    al, 0ch
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, al
        and    ah, 30h
        dec    edx
        mov    al, 10h
        out    dx, al
        inc    edx
        in     al, dx
        db  08Ah, 015h, 0EBh, 000h, 000h, 000h
        db  08Ah, 035h, 0ECh, 000h, 000h, 000h
        cmp    ax, dx
        jne    SHORT loc_5_009F
        mov    edx, 3d7h
        db  0A0h, 0EDh, 000h, 000h, 000h
        out    dx, al
        dec    edx
        mov    al, 0ch
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0cfh
        db  08Ah, 025h, 0EEh, 000h, 000h, 000h
        or     al, ah
        out    dx, al
        dec    edx
loc_5_009F:
        db  0C6h, 005h, 0EDh, 000h, 000h, 000h, 0FFh
        mov    edx, 3d6h
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_5_00B1:
        ret
loc_5_00B2:
        push   eax
        push   edx
        mov    edx, 3d6h
        in     al, dx
        ror    eax, 8
        mov    al, 10h
        out    dx, al
        inc    edx
        in     al, dx
        db  0A2h, 0EFh, 000h, 000h, 000h
        db  0A0h, 0EBh, 000h, 000h, 000h
        out    dx, al
        dec    edx
        mov    al, 0ch
        out    dx, al
        inc    edx
        in     al, dx
        db  0A2h, 0F0h, 000h, 000h, 000h
        db  08Ah, 025h, 0ECh, 000h, 000h, 000h
        and    al, 0cfh
        or     al, ah
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
        ret
loc_5_00EB:
        push   eax
        push   edx
        mov    edx, 3d6h
        in     al, dx
        ror    eax, 8
        mov    al, 10h
        db  08Ah, 025h, 0EFh, 000h, 000h, 000h
        out    dx, ax
        mov    al, 0ch
        db  08Ah, 025h, 0F0h, 000h, 000h, 000h
        out    dx, ax
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
        ret
loc_5_0111:
        mov    edx, 3d6h
        in     al, dx
        ror    eax, 8
        mov    ax, 10bh
        out    dx, ax
        mov    ax, 10h
        out    dx, ax
        mov    ax, 11h
        out    dx, ax
        rol    eax, 8
        out    dx, al
        ret
loc_5_0131:
        push   edx
        push   edi
        mov    edx, 3d6h
        in     al, dx
        ror    eax, 8
        mov    edi, ebx
        db  003h, 03Dh, 0E4h, 000h, 000h, 000h
        mov    ecx, 0ffh
loc_5_0149:
        mov    al, cl
        out    dx, al
        in     ax, dx
        mov    byte ptr [ecx + edi], ah
        dec    ecx
        jns    SHORT loc_5_0149
        rol    eax, 8
        out    dx, al
        pop    edi
        pop    edx
        ret
loc_5_015B:
        mov    edx, 3d6h
        in     al, dx
        ror    eax, 8
        mov    ax, 10bh
        out    dx, ax
        rol    eax, 8
        out    dx, al
        ret
loc_5_016F:
        mov    edx, 3d6h
        in     al, dx
        ror    eax, 8
        db  066h, 0A1h, 0E8h, 000h, 000h, 000h
        out    dx, ax
        mov    ax, 10h
        out    dx, ax
        mov    al, 0ch
        out    dx, al
        in     ax, dx
        and    ah, 0cfh
        out    dx, ax
        rol    eax, 8
        out    dx, al
        ret
loc_5_0195:
        push   edi
        push   esi
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        db  03Bh, 03Dh, 0D0h, 000h, 000h, 000h
        jne    SHORT loc_5_01A9
        cmp    ebx, edi
        jne    SHORT loc_5_01C6
loc_5_01A9:
        cmp    ecx, 4
        je     SHORT loc_5_01C3
        cmp    ecx, 0ch
        je     SHORT loc_5_01BF
        cmp    ecx, 8
        je     SHORT loc_5_01BB
        in     al, dx
        jmp    SHORT loc_5_022E
loc_5_01BB:
        in     ax, dx
        jmp    SHORT loc_5_022E
loc_5_01BF:
        out    dx, ax
        jmp    SHORT loc_5_022E
loc_5_01C3:
        out    dx, al
        jmp    SHORT loc_5_022E
loc_5_01C6:
        mov    edi, ebx
        db  003h, 03Dh, 0E4h, 000h, 000h, 000h
        cmp    ecx, 4
        je     SHORT loc_5_0214
        cmp    ecx, 0ch
        je     SHORT loc_5_0204
        cmp    ecx, 8
        je     SHORT loc_5_01F8
        cmp    dx, 3d6h
        jne    SHORT loc_5_01EC
        mov    al, byte ptr [edi + 100h]
        jmp    SHORT loc_5_022E
loc_5_01EC:
        movzx  eax, byte ptr [edi + 100h]
        mov    al, byte ptr [eax + edi]
        jmp    SHORT loc_5_022E
loc_5_01F8:
        movzx  eax, byte ptr [edi + 100h]
        mov    ah, byte ptr [eax + edi]
        jmp    SHORT loc_5_022E
loc_5_0204:
        mov    byte ptr [edi + 100h], al
        mov    cl, ah
        movzx  eax, al
        mov    byte ptr [eax + edi], cl
        jmp    SHORT loc_5_022E
loc_5_0214:
        cmp    edx, 3d6h
        jne    SHORT loc_5_0224
        mov    byte ptr [edi + 100h], al
        jmp    SHORT loc_5_022E
loc_5_0224:
        movzx  ecx, byte ptr [edi + 100h]
        mov    byte ptr [ecx + edi], al
loc_5_022E:
        pop    esi
        pop    edi
        ret
loc_5_0231:
        db  0F6h, 005h, 0F4h, 000h, 000h, 000h, 001h
        je     SHORT loc_5_026F
        push   edx
loc_5_023B:
        db  08Bh, 015h, 0DCh, 000h, 000h, 000h
        or     edx, edx
        je     SHORT loc_5_024D
        mov    eax, dword ptr [edx + 93d0h]
        jmp    SHORT loc_5_0253
loc_5_024D:
        mov    edx, 93d0h
        in     eax, dx
loc_5_0253:
        cmp    eax, -1
        je     SHORT loc_5_026E
        test   ax, 4000h
        je     SHORT loc_5_0267
        test   eax, 100000h
        je     SHORT loc_5_0267
        jmp    SHORT loc_5_026E
loc_5_0267:
        test   eax, 100000h
        jne    SHORT loc_5_023B
loc_5_026E:
        pop    edx
loc_5_026F:
        ret
loc_5_0270:
        db  08Bh, 00Dh, 0E0h, 000h, 000h, 000h
        stc
        ret
_PTEXT ends
        end
