; ati_pcode_03.asm -- ATI object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        mov    eax, dword ptr [ebp + 18h]
        db  0A3h, 084h, 001h, 000h, 000h
        mov    edx, dword ptr [ebp + 14h]
        or     edx, edx
        je     SHORT loc_3_0064
        db  083h, 03Dh, 090h, 001h, 000h, 000h, 000h
        jne    SHORT loc_3_0061
        push   0
        push   800000h
        push   edx
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        push   0
        push   80h
        push   92h
        push   800h
        push   eax
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        push   0
        push   1
        push   eax
        push   edx
        push   ebx
        db  0CDh, 020h, 078h, 000h, 001h, 000h
        add    esp, 14h
        db  0A3h, 090h, 001h, 000h, 000h
        push   eax
        db  0CDh, 020h, 012h, 000h, 00Ah, 000h
        pop    eax
loc_3_0061:
        mov    dword ptr [ebp + 14h], eax
loc_3_0064:
        mov    edx, dword ptr [ebp + 4]
        or     edx, edx
        je     SHORT loc_3_00BD
        db  083h, 03Dh, 094h, 001h, 000h, 000h, 000h
        jne    SHORT loc_3_00BA
        push   0
        push   2000h
        push   edx
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        push   0
        push   80h
        push   92h
        push   2
        push   eax
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        push   0
        push   1
        push   eax
        push   edx
        push   ebx
        db  0CDh, 020h, 078h, 000h, 001h, 000h
        add    esp, 14h
        db  0A3h, 094h, 001h, 000h, 000h
        push   eax
        db  0CDh, 020h, 012h, 000h, 00Ah, 000h
        pop    eax
loc_3_00BA:
        mov    dword ptr [ebp + 4], eax
loc_3_00BD:
        ret
loc_3_00BE:
        db  0C6h, 005h, 0A0h, 001h, 000h, 000h, 0FFh
        db  080h, 03Dh, 0A2h, 001h, 000h, 000h, 032h
        je     SHORT loc_3_00E9
        db  080h, 03Dh, 0A2h, 001h, 000h, 000h, 064h
        je     NEAR PTR loc_3_016A
        xor    ecx, ecx
        mov    edx, 10000h
        xor    ah, ah
        jmp    NEAR PTR loc_3_016D
loc_3_00E9:
        mov    eax, ecx
        shr    eax, 10h
        or     cx, cx
        je     SHORT loc_3_010D
        db  08Bh, 015h, 084h, 001h, 000h, 000h
        shr    edx, 10h
        dec    dl
        cmp    al, dl
        jae    SHORT loc_3_016A
        inc    al
        xor    cx, cx
        add    ecx, 10000h
loc_3_010D:
        db  08Bh, 015h, 084h, 001h, 000h, 000h
        sub    edx, ecx
        cmp    edx, 10000h
        mov    edx, 10000h
        ja     SHORT loc_3_0127
        mov    edx, 8000h
loc_3_0127:
        push   edx
        push   ebx
        db  0C6h, 005h, 09Ch, 001h, 000h, 000h, 0FFh
        mov    ah, al
        shr    al, 2
        and    ah, 3
        shl    ah, 2
        db  088h, 025h, 0A1h, 001h, 000h, 000h
        mov    bl, al
        mov    bh, al
        and    bl, 0fh
        shl    bl, 1
        db  088h, 01Dh, 09Ah, 001h, 000h, 000h
        shr    bh, 4
        mov    edx, 1ceh
        mov    al, 0aeh
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0f0h
        or     al, bh
        db  0A2h, 09Bh, 001h, 000h, 000h
        pop    ebx
        pop    edx
        jmp    SHORT loc_3_016D
loc_3_016A:
        mov    edx, dword ptr [ebp + 18h]
loc_3_016D:
        ret
_PTEXT ends
        end
