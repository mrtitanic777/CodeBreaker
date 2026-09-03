; tseng_pcode_04.asm -- TSENG object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_4_0000:
        db  083h, 03Dh, 054h, 001h, 000h, 000h, 000h
        je     SHORT loc_4_0053
        push   eax
        push   edx
        mov    edx, 3cdh
        in     al, dx
        mov    ah, al
        xor    al, al
        db  080h, 03Dh, 060h, 001h, 000h, 000h, 010h
        je     SHORT loc_4_0021
        mov    dl, 0cbh
        in     al, dx
loc_4_0021:
        db  066h, 03Bh, 005h, 063h, 001h, 000h, 000h
        je     SHORT loc_4_0049
        db  066h, 0A3h, 065h, 001h, 000h, 000h
        mov    dl, 0cdh
        db  0A0h, 064h, 001h, 000h, 000h
        out    dx, al
        db  080h, 03Dh, 060h, 001h, 000h, 000h, 010h
        je     SHORT loc_4_0049
        mov    dl, 0cbh
        db  0A0h, 063h, 001h, 000h, 000h
        out    dx, al
loc_4_0049:
        pop    edx
        pop    eax
        and    al, 0fh
        db  002h, 005h, 069h, 001h, 000h, 000h
loc_4_0053:
        ret
loc_4_0054:
        push   eax
        push   edx
        db  083h, 03Dh, 054h, 001h, 000h, 000h, 000h
        je     SHORT loc_4_0097
        mov    edx, 3cdh
        in     al, dx
        mov    ah, al
        xor    al, al
        db  080h, 03Dh, 060h, 001h, 000h, 000h, 010h
        je     SHORT loc_4_0075
        mov    dl, 0cbh
        in     al, dx
loc_4_0075:
        db  066h, 03Bh, 005h, 063h, 001h, 000h, 000h
        jne    SHORT loc_4_0097
        mov    dl, 0cdh
        db  0A0h, 066h, 001h, 000h, 000h
        out    dx, al
        db  080h, 03Dh, 060h, 001h, 000h, 000h, 010h
        jbe    SHORT loc_4_0097
        mov    dl, 0cbh
        db  0A0h, 065h, 001h, 000h, 000h
        out    dx, al
loc_4_0097:
        pop    edx
        pop    eax
        ret
loc_4_009A:
        push   eax
        push   edx
        db  083h, 03Dh, 054h, 001h, 000h, 000h, 000h
        je     SHORT loc_4_00CD
        mov    edx, 3cdh
        in     al, dx
        db  0A2h, 067h, 001h, 000h, 000h
        db  0A0h, 064h, 001h, 000h, 000h
        out    dx, al
        db  080h, 03Dh, 060h, 001h, 000h, 000h, 010h
        jbe    SHORT loc_4_00CD
        mov    dl, 0cbh
        in     al, dx
        db  0A2h, 068h, 001h, 000h, 000h
        db  0A0h, 063h, 001h, 000h, 000h
        out    dx, al
loc_4_00CD:
        pop    edx
        pop    eax
        ret
loc_4_00D0:
        push   eax
        push   edx
        db  083h, 03Dh, 054h, 001h, 000h, 000h, 000h
        je     SHORT loc_4_00F7
        mov    edx, 3cdh
        db  0A0h, 067h, 001h, 000h, 000h
        out    dx, al
        db  080h, 03Dh, 060h, 001h, 000h, 000h, 010h
        jbe    SHORT loc_4_00F7
        mov    dl, 0cbh
        db  0A0h, 068h, 001h, 000h, 000h
        out    dx, al
loc_4_00F7:
        pop    edx
        pop    eax
        ret
loc_4_00FA:
        db  080h, 00Dh, 061h, 001h, 000h, 000h, 002h
        db  080h, 03Dh, 060h, 001h, 000h, 000h, 010h
        jne    SHORT loc_4_0115
        mov    edx, 3ddh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
loc_4_0115:
        ret
loc_4_0116:
        db  0C6h, 005h, 061h, 001h, 000h, 000h, 000h
        ret
loc_4_011E:
        db  080h, 00Dh, 061h, 001h, 000h, 000h, 001h
        ret
loc_4_0126:
        db  0C6h, 005h, 061h, 001h, 000h, 000h, 000h
        db  080h, 03Dh, 060h, 001h, 000h, 000h, 010h
        jne    SHORT loc_4_0141
        mov    edx, 3ddh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
loc_4_0141:
        ret
loc_4_0142:
        push   edx
        db  03Bh, 00Dh, 044h, 001h, 000h, 000h
        jne    SHORT loc_4_015A
        db  0A0h, 062h, 001h, 000h, 000h
        or     al, al
        je     SHORT loc_4_015A
        mov    edx, 3c2h
        out    dx, al
loc_4_015A:
        db  03Bh, 035h, 050h, 001h, 000h, 000h
        je     SHORT loc_4_0173
        db  03Bh, 035h, 04Ch, 001h, 000h, 000h
        je     SHORT loc_4_0173
        db  0F6h, 005h, 061h, 001h, 000h, 000h, 003h
        jne    SHORT loc_4_0173
loc_4_0173:
        pop    edx
        ret
loc_4_0175:
        mov    edx, 217ah
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 217bh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        ret
loc_4_018C:
        mov    edx, 217ah
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 217bh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        ret
loc_4_01A3:
        push   eax
        mov    edx, 3cdh
        in     al, dx
        mov    ah, al
        xor    al, al
        db  080h, 03Dh, 060h, 001h, 000h, 000h, 010h
        je     SHORT loc_4_01BA
        mov    dl, 0cbh
        in     al, dx
loc_4_01BA:
        and    ax, 0f030h
        shr    ah, 4
        or     al, ah
        movzx  edx, al
        pop    eax
        stc
        ret
loc_4_01C9:
        push   eax
        mov    edx, 3cdh
        in     al, dx
        mov    ah, al
        xor    al, al
        db  080h, 03Dh, 060h, 001h, 000h, 000h, 010h
        je     SHORT loc_4_01E0
        mov    dl, 0cbh
        in     al, dx
loc_4_01E0:
        and    ax, 0f03h
        shl    al, 4
        or     al, ah
        movzx  edx, al
        pop    eax
        stc
        ret
loc_4_01EF:
        push   eax
        push   edx
        mov    dh, al
        mov    eax, edx
        and    ax, 0f0fh
        shl    ah, 4
        or     al, ah
        and    dx, 0f0f0h
        shr    dl, 4
        or     dl, dh
        mov    ah, dl
        mov    edx, 3cdh
        out    dx, al
        db  080h, 03Dh, 060h, 001h, 000h, 000h, 010h
        je     SHORT loc_4_021E
        mov    dl, 0cbh
        mov    al, ah
        out    dx, al
loc_4_021E:
        pop    edx
        pop    eax
        stc
        ret
loc_4_0222:
        push   edi
        push   esi
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        db  03Bh, 03Dh, 044h, 001h, 000h, 000h
        jne    SHORT loc_4_0236
        cmp    ebx, edi
        jne    SHORT loc_4_0241
loc_4_0236:
        cmp    ecx, 4
        je     SHORT loc_4_023E
        in     al, dx
        jmp    SHORT loc_4_0241
loc_4_023E:
        out    dx, al
        jmp    SHORT loc_4_0241
loc_4_0241:
        pop    esi
        pop    edi
        ret
loc_4_0244:
        cmp    ecx, 31h
        jb     SHORT loc_4_025F
        db  0F6h, 005h, 061h, 001h, 000h, 000h, 003h
        jne    SHORT loc_4_025F
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        cmp    ebx, edi
        je     SHORT loc_4_025F
        stc
        jmp    SHORT loc_4_0260
loc_4_025F:
        clc
loc_4_0260:
        ret
loc_4_0261:
        push   edi
        push   esi
        db  0F6h, 005h, 061h, 001h, 000h, 000h, 003h
        jne    SHORT loc_4_027E
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        db  03Bh, 03Dh, 044h, 001h, 000h, 000h
        jne    SHORT loc_4_027E
        cmp    ebx, edi
        jne    SHORT loc_4_0292
loc_4_027E:
        cmp    ecx, 4
        je     SHORT loc_4_028F
        cmp    ecx, 0ch
        je     SHORT loc_4_028B
loc_4_0288:
        in     al, dx
        jmp    SHORT loc_4_029E
loc_4_028B:
        out    dx, ax
        jmp    SHORT loc_4_029E
loc_4_028F:
        out    dx, al
        jmp    SHORT loc_4_029E
loc_4_0292:
        cmp    ecx, 4
        je     SHORT loc_4_029E
        cmp    ecx, 0ch
        je     SHORT loc_4_029E
        jmp    SHORT loc_4_0288
loc_4_029E:
        pop    esi
        pop    edi
        ret
_PTEXT ends
        end
