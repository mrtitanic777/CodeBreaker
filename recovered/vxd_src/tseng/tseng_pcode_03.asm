; tseng_pcode_03.asm -- TSENG object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        mov    eax, dword ptr [ebp + 18h]
        db  0A3h, 054h, 001h, 000h, 000h
        mov    edx, 3cch
        in     al, dx
        db  0A2h, 062h, 001h, 000h, 000h
        db  080h, 03Dh, 060h, 001h, 000h, 000h, 010h
        jne    SHORT loc_3_002E
        movzx  eax, word ptr [ebp + 14h]
        db  0CDh, 020h, 012h, 000h, 00Ah, 000h
        mov    word ptr [ebp + 14h], 0
        jmp    SHORT loc_3_0084
loc_3_002E:
        db  0A1h, 05Ch, 001h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_3_0080
        mov    edx, dword ptr [ebp + 14h]
        push   0
        push   8000h
        push   edx
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        push   0
        push   80h
        push   92h
        push   8
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
        db  0A3h, 05Ch, 001h, 000h, 000h
        push   eax
        db  0CDh, 020h, 012h, 000h, 00Ah, 000h
        pop    eax
loc_3_0080:
        mov    word ptr [ebp + 14h], ax
loc_3_0084:
        ret
loc_3_0085:
        mov    eax, ecx
        shr    eax, 10h
        or     cx, cx
        je     SHORT loc_3_00A9
        db  08Bh, 015h, 054h, 001h, 000h, 000h
        shr    edx, 10h
        dec    dl
        cmp    al, dl
        jae    SHORT loc_3_010D
        inc    al
        xor    cx, cx
        add    ecx, 10000h
loc_3_00A9:
        db  08Bh, 015h, 054h, 001h, 000h, 000h
        sub    edx, ecx
        cmp    edx, 10000h
        mov    edx, 10000h
        ja     SHORT loc_3_00C3
        mov    edx, 8000h
loc_3_00C3:
        push   edx
        push   ebx
        mov    ah, al
        shr    al, 2
        and    ah, 3
        shl    ah, 2
        db  088h, 025h, 069h, 001h, 000h, 000h
        mov    bl, al
        mov    bh, al
        and    bl, 0fh
        mov    al, bl
        shl    bl, 4
        or     al, bl
        db  0A2h, 064h, 001h, 000h, 000h
        db  0C6h, 005h, 063h, 001h, 000h, 000h, 000h
        db  080h, 03Dh, 060h, 001h, 000h, 000h, 010h
        je     SHORT loc_3_0109
        and    bh, 30h
        mov    bl, bh
        shr    bl, 4
        or     bl, bh
        db  088h, 01Dh, 063h, 001h, 000h, 000h
loc_3_0109:
        pop    ebx
        pop    edx
        jmp    SHORT loc_3_010F
loc_3_010D:
        mov    edx, ecx
loc_3_010F:
        ret
_PTEXT ends
        end
