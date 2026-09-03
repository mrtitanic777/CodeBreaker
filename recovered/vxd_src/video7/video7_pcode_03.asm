; video7_pcode_03.asm -- VIDEO7 object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        mov    eax, dword ptr [ebp + 18h]
        db  0A3h, 06Ah, 001h, 000h, 000h
        movzx  eax, word ptr [ebp + 14h]
        db  0CDh, 020h, 012h, 000h, 00Ah, 000h
        ret
loc_3_0013:
        db  0C6h, 005h, 076h, 001h, 000h, 000h, 0FFh
        mov    ecx, 0c0000h
        db  03Bh, 00Dh, 06Ah, 001h, 000h, 000h
        jae    SHORT loc_3_009F
        mov    eax, ecx
        shr    eax, 10h
        or     cx, cx
        je     SHORT loc_3_004B
        db  08Bh, 015h, 06Ah, 001h, 000h, 000h
        shr    edx, 10h
        dec    dl
        cmp    al, dl
        jae    SHORT loc_3_009F
        inc    al
        xor    cx, cx
        add    ecx, 10000h
loc_3_004B:
        db  08Bh, 015h, 06Ah, 001h, 000h, 000h
        sub    edx, ecx
        cmp    edx, 10000h
        mov    edx, 10000h
        ja     SHORT loc_3_0065
        mov    edx, 8000h
loc_3_0065:
        push   edx
        push   ebx
        db  0C6h, 005h, 072h, 001h, 000h, 000h, 0FFh
        db  0A2h, 06Eh, 001h, 000h, 000h
        mov    bl, al
        and    bl, 1
        db  088h, 01Dh, 06Fh, 001h, 000h, 000h
        mov    ah, al
        and    ah, 2
        shl    ah, 4
        db  088h, 025h, 071h, 001h, 000h, 000h
        and    al, 0ch
        mov    bh, al
        shr    al, 2
        or     bh, al
        db  088h, 03Dh, 070h, 001h, 000h, 000h
        pop    ebx
        pop    edx
        jmp    SHORT loc_3_00A1
loc_3_009F:
        mov    edx, ecx
loc_3_00A1:
        ret
loc_3_00A2:
        db  0C6h, 005h, 076h, 001h, 000h, 000h, 000h
        ret
loc_3_00AA:
        cld
        push   esi
        push   edi
        push   ecx
        db  08Bh, 035h, 0D8h, 000h, 000h, 000h
        db  003h, 035h, 0DCh, 000h, 000h, 000h
        db  0BFh, 0E8h, 000h, 000h, 000h
        mov    ecx, 82h
        rep movsb
        db  0C6h, 005h, 079h, 001h, 000h, 000h, 0FFh
        pop    ecx
        pop    edi
        pop    esi
        ret
_PTEXT ends
        end
