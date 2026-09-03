; video7_pcode_04.asm -- VIDEO7 object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_4_0000:
        db  080h, 03Dh, 072h, 001h, 000h, 000h, 0FFh
        jne    SHORT loc_4_0074
        db  080h, 03Dh, 076h, 001h, 000h, 000h, 000h
        je     SHORT loc_4_0074
        push   eax
        push   ecx
        push   edx
        mov    edx, 3cch
        in     al, dx
        mov    cl, al
        shr    cl, 4
        and    cl, 2
        and    al, 0dfh
        db  00Ah, 005h, 071h, 001h, 000h, 000h
        mov    dl, 0c2h
        out    dx, al
        mov    dl, 0c4h
        in     al, dx
        ror    eax, 8
        mov    ax, 0ea06h
        out    dx, ax
        mov    al, 0f9h
        out    dx, al
        inc    dl
        in     al, dx
        and    al, 1
        or     cl, al
        db  0A0h, 06Fh, 001h, 000h, 000h
        out    dx, al
        dec    dl
        mov    al, 0f6h
        out    dx, al
        inc    dl
        in     al, dx
        mov    ch, al
        and    ch, 3
        shl    ch, 2
        or     cl, ch
        db  088h, 00Dh, 072h, 001h, 000h, 000h
        and    al, 0f0h
        db  00Ah, 005h, 070h, 001h, 000h, 000h
        out    dx, al
        dec    dl
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    ecx
        pop    eax
loc_4_0074:
        ret
loc_4_0075:
        db  080h, 03Dh, 072h, 001h, 000h, 000h, 0FFh
        je     NEAR PTR loc_4_011A
        push   eax
        push   ecx
        push   edx
        mov    edx, 3cch
        in     al, dx
        mov    cl, al
        shr    cl, 4
        and    cl, 2
        mov    dl, 0c4h
        in     al, dx
        ror    eax, 8
        mov    ax, 0ea06h
        out    dx, ax
        mov    al, 0f9h
        out    dx, al
        inc    dl
        in     al, dx
        and    al, 1
        or     cl, al
        dec    dl
        mov    al, 0f6h
        out    dx, al
        inc    dl
        in     al, dx
        and    al, 3
        shl    al, 2
        or     cl, al
        dec    dl
        rol    eax, 8
        out    dx, al
        db  03Ah, 00Dh, 06Eh, 001h, 000h, 000h
        jne    SHORT loc_4_0110
        mov    dl, 0cch
        in     al, dx
        db  08Ah, 025h, 072h, 001h, 000h, 000h
        and    ah, 2
        shl    ah, 4
        and    ax, 20dfh
        or     al, ah
        out    dx, al
        mov    dl, 0c4h
        in     al, dx
        ror    eax, 8
        mov    al, 0f9h
        db  08Ah, 025h, 072h, 001h, 000h, 000h
        and    ah, 1
        out    dx, ax
        db  0A0h, 072h, 001h, 000h, 000h
        and    al, 0ch
        mov    ah, al
        shr    al, 2
        or     ah, al
        mov    al, 0f6h
        out    dx, al
        inc    dl
        in     al, dx
        and    ax, 0ff0h
        or     al, ah
        out    dx, al
        dec    dl
        rol    eax, 8
        out    dx, al
loc_4_0110:
        db  0C6h, 005h, 072h, 001h, 000h, 000h, 0FFh
        pop    edx
        pop    ecx
        pop    eax
loc_4_011A:
        ret
loc_4_011B:
        db  080h, 03Dh, 076h, 001h, 000h, 000h, 000h
        je     SHORT loc_4_0177
        push   eax
        push   edx
        mov    edx, 3cch
        in     al, dx
        db  0A2h, 075h, 001h, 000h, 000h
        and    al, 0dfh
        db  00Ah, 005h, 071h, 001h, 000h, 000h
        mov    dl, 0c2h
        out    dx, al
        mov    dl, 0c4h
        in     al, dx
        ror    eax, 8
        mov    ax, 0ea06h
        out    dx, ax
        mov    al, 0f9h
        out    dx, al
        inc    dl
        in     al, dx
        db  0A2h, 073h, 001h, 000h, 000h
        db  0A0h, 06Fh, 001h, 000h, 000h
        out    dx, al
        dec    dl
        mov    al, 0f6h
        out    dx, al
        inc    dl
        in     al, dx
        db  0A2h, 074h, 001h, 000h, 000h
        and    al, 0f0h
        db  00Ah, 005h, 070h, 001h, 000h, 000h
        out    dx, al
        dec    dl
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_4_0177:
        ret
loc_4_0178:
        db  080h, 03Dh, 076h, 001h, 000h, 000h, 000h
        je     SHORT loc_4_01B4
        push   eax
        push   edx
        mov    edx, 3c2h
        db  0A0h, 075h, 001h, 000h, 000h
        out    dx, al
        mov    dl, 0c4h
        in     al, dx
        ror    eax, 8
        mov    ax, 0ea06h
        out    dx, ax
        mov    al, 0f9h
        db  08Ah, 025h, 073h, 001h, 000h, 000h
        out    dx, ax
        mov    al, 0f6h
        db  08Ah, 025h, 074h, 001h, 000h, 000h
        out    dx, ax
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_4_01B4:
        ret
loc_4_01B5:
        push   edx
        push   edi
        db  03Bh, 01Dh, 0E4h, 000h, 000h, 000h
        je     SHORT loc_4_0234
        db  03Bh, 01Dh, 0E0h, 000h, 000h, 000h
        je     SHORT loc_4_0234
        mov    edx, 3c4h
        in     al, dx
        ror    eax, 8
        db  03Bh, 01Dh, 0D8h, 000h, 000h, 000h
        je     SHORT loc_4_020D
        push   ebx
        db  003h, 01Dh, 0DCh, 000h, 000h, 000h
        test   byte ptr [ebx + 80h], 1
        pop    ebx
        jne    SHORT loc_4_020D
        mov    al, 4
        out    dx, al
        inc    dl
        in     al, dx
        cmp    al, 6
        jne    SHORT loc_4_01F6
        mov    al, 2
        out    dx, al
loc_4_01F6:
        mov    dl, 0ceh
        in     al, dx
        mov    ah, al
        mov    al, 8
        out    dx, al
        inc    dl
        in     al, dx
        or     al, al
        jne    SHORT loc_4_0208
        dec    al
        out    dx, al
loc_4_0208:
        dec    dl
        mov    al, ah
        out    dx, al
loc_4_020D:
        mov    dl, 0c4h
        mov    ax, 0ea06h
        out    dx, ax
        mov    edi, ebx
        db  003h, 03Dh, 0DCh, 000h, 000h, 000h
        mov    ecx, 80h
loc_4_0222:
        mov    al, cl
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [ecx + edi - 80h], al
        dec    edx
        inc    cl
        jne    SHORT loc_4_0222
        rol    eax, 8
        out    dx, al
loc_4_0234:
        pop    edi
        pop    edx
        ret
loc_4_0237:
        push   edx
        db  03Bh, 01Dh, 0D8h, 000h, 000h, 000h
        je     SHORT loc_4_027F
        db  03Bh, 01Dh, 0E4h, 000h, 000h, 000h
        je     SHORT loc_4_027F
        db  03Bh, 01Dh, 0E0h, 000h, 000h, 000h
        je     NEAR PTR loc_4_02FB
        push   ebx
        db  003h, 01Dh, 0DCh, 000h, 000h, 000h
        test   byte ptr [ebx + 80h], 1
        pop    ebx
        jne    SHORT loc_4_027F
        mov    edx, 3ceh
        in     al, dx
        mov    ah, al
        mov    al, 8
        out    dx, al
        inc    dl
        in     al, dx
        or     al, al
        jne    SHORT loc_4_027A
        dec    al
        out    dx, al
loc_4_027A:
        dec    dl
        mov    al, ah
        out    dx, al
loc_4_027F:
        mov    edx, 3c4h
        in     al, dx
        ror    eax, 8
        mov    ax, 0ea06h
        out    dx, ax
        db  03Bh, 00Dh, 0E4h, 000h, 000h, 000h
        jne    SHORT loc_4_02A1
        db  0B9h, 0E8h, 000h, 000h, 000h
        db  02Bh, 00Dh, 0DCh, 000h, 000h, 000h
loc_4_02A1:
        db  003h, 00Dh, 0DCh, 000h, 000h, 000h
        xor    al, al
        or     al, byte ptr [ecx + 24h]
        or     al, byte ptr [ecx + 78h]
        or     al, byte ptr [ecx + 7dh]
        je     SHORT loc_4_02F7
        mov    al, 0a4h
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        cmp    al, byte ptr [ecx + 24h]
        jne    SHORT loc_4_02D5
        mov    al, 0f8h
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        cmp    al, byte ptr [ecx + 78h]
        jne    SHORT loc_4_02D5
        mov    al, 0fdh
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        cmp    al, byte ptr [ecx + 7dh]
        je     SHORT loc_4_02F7
loc_4_02D5:
        mov    eax, 100h
        out    dx, ax
        mov    al, 0a4h
        mov    ah, byte ptr [ecx + 24h]
        out    dx, ax
        mov    al, 0f8h
        mov    ah, byte ptr [ecx + 78h]
        out    dx, ax
        mov    al, 0fdh
        mov    ah, byte ptr [ecx + 7dh]
        out    dx, ax
        mov    ax, 300h
        out    dx, ax
loc_4_02F7:
        rol    eax, 8
        out    dx, al
loc_4_02FB:
        pop    edx
        ret
loc_4_02FD:
        db  0C6h, 005h, 077h, 001h, 000h, 000h, 0FFh
        ret
loc_4_0305:
        db  0C6h, 005h, 077h, 001h, 000h, 000h, 000h
        ret
loc_4_030D:
        db  080h, 03Dh, 078h, 001h, 000h, 000h, 000h
        jne    NEAR PTR loc_4_03D1
        db  0C6h, 005h, 078h, 001h, 000h, 000h, 0FFh
        cmp    cl, 4
        jne    SHORT loc_4_0378
        db  03Bh, 01Dh, 0D8h, 000h, 000h, 000h
        je     SHORT loc_4_0378
        cmp    al, 6
        jne    SHORT loc_4_0378
        push   ebx
        db  003h, 01Dh, 0DCh, 000h, 000h, 000h
        mov    ah, byte ptr [ebx + 80h]
        mov    ch, byte ptr [ebx + 81h]
        pop    ebx
        cmp    ch, 3
        mov    ch, 0
        jne    SHORT loc_4_0378
        test   ah, 1
        jne    SHORT loc_4_0378
        push   eax
        push   ebx
        push   ecx
        push   edx
        mov    eax, 204h
        mov    ecx, 0ch
        dec    dl
        db  0CDh, 020h, 030h, 001h, 001h, 000h
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        cmp    ebx, edi
        je     SHORT loc_4_03BD
        jmp    SHORT loc_4_03CE
loc_4_0378:
        db  080h, 03Dh, 077h, 001h, 000h, 000h, 000h
        jne    SHORT loc_4_03BD
        mov    edi, ebx
        db  003h, 03Dh, 0DCh, 000h, 000h, 000h
        cmp    cl, 2
        jne    SHORT loc_4_0396
        mov    byte ptr [edi + 81h], al
        jmp    SHORT loc_4_03D1
loc_4_0396:
        cmp    cl, 80h
        jb     SHORT loc_4_03D1
        and    ecx, 0ffh
        sub    cl, 80h
        mov    byte ptr [ecx + edi], al
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        cmp    ebx, edi
        jne    SHORT loc_4_03CE
        db  080h, 03Dh, 079h, 001h, 000h, 000h, 000h
        je     SHORT loc_4_03CE
        add    cl, 80h
loc_4_03BD:
        dec    dl
        mov    ch, al
        in     al, dx
        ror    eax, 8
        mov    ax, cx
        out    dx, ax
        rol    eax, 8
        out    dx, al
loc_4_03CE:
        stc
        jmp    SHORT loc_4_03D2
loc_4_03D1:
        clc
loc_4_03D2:
        db  0C6h, 005h, 078h, 001h, 000h, 000h, 000h
        ret
loc_4_03DA:
        pushf
        cmp    cl, 80h
        jb     SHORT loc_4_041D
        popf
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        cmp    ebx, edi
        je     SHORT loc_4_0408
        db  080h, 03Dh, 077h, 001h, 000h, 000h, 000h
        jne    SHORT loc_4_0408
        sub    ecx, 80h
        db  003h, 00Dh, 0DCh, 000h, 000h, 000h
        mov    al, byte ptr [ebx + ecx]
        stc
        jmp    SHORT loc_4_041F
loc_4_0408:
        dec    dl
        in     al, dx
        mov    ah, al
        mov    al, cl
        out    dx, al
        inc    dl
        in     al, dx
        dec    dl
        xchg   ah, al
        out    dx, al
        mov    al, ah
        stc
        jmp    SHORT loc_4_041F
loc_4_041D:
        popf
loc_4_041F:
        ret
loc_4_0420:
        pushfd
        cmp    cl, 6
        jne    SHORT loc_4_0434
        push   ebx
        db  003h, 01Dh, 0DCh, 000h, 000h, 000h
        mov    byte ptr [ebx + 80h], al
        pop    ebx
loc_4_0434:
        popfd
        ret
_PTEXT ends
        end
