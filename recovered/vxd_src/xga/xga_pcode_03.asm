; xga_pcode_03.asm -- XGA object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        cmp    byte ptr [ebp + 18h], 0
        jne    SHORT loc_3_0024
        db  0A1h, 00Ch, 001h, 000h, 000h
        mov    dword ptr [ebp + 1ch], eax
        db  08Bh, 015h, 0FCh, 000h, 000h, 000h
        mov    word ptr [ebp + 14h], dx
        db  08Bh, 03Dh, 010h, 001h, 000h, 000h
        mov    word ptr [ebp], di
        jmp    SHORT loc_3_0094
loc_3_0024:
        call   sub_3_0095
        mov    dword ptr [ebp + 1ch], esi
        db  00Fh, 0B7h, 005h, 062h, 001h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_3_0070
        db  0A1h, 008h, 001h, 000h, 000h
        mov    ecx, 3ffh
        mov    edx, 93h
        mov    edi, 10h
        push   0
        push   edi
        push   edx
        push   ecx
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
        db  066h, 0A3h, 062h, 001h, 000h, 000h
loc_3_0070:
        mov    word ptr [ebp + 14h], ax
        db  0B8h, 018h, 001h, 000h, 000h
        mov    ecx, 4ah
        mov    word ptr [eax], cx
        db  0CDh, 020h, 014h, 000h, 00Ah, 000h
        db  00Fh, 0B7h, 03Dh, 038h, 001h, 000h, 000h
        db  089h, 03Dh, 014h, 001h, 000h, 000h
loc_3_0094:
        ret
sub_3_0095:
        db  08Bh, 015h, 0FCh, 000h, 000h, 000h
        add    dl, 4
        xor    al, al
        out    dx, al
        inc    dl
        mov    al, 0ffh
        out    dx, al
        sub    dl, 5
        mov    al, 4
        out    dx, al
        inc    dl
        mov    al, 1
        out    dx, al
        add    dl, 7
        mov    al, 0ch
        out    dx, al
        mov    esi, 80000h
        db  08Bh, 03Dh, 0ECh, 000h, 000h, 000h
        mov    byte ptr [edi], 5ah
        cmp    byte ptr [edi], 5ah
        jne    SHORT loc_3_00FE
        mov    byte ptr [edi], 0
        cmp    byte ptr [edi], 0
        jne    SHORT loc_3_00FE
        mov    esi, 100000h
        db  081h, 03Dh, 010h, 001h, 000h, 000h, 0DBh, 08Fh, 000h, 000h
        je     SHORT loc_3_00FE
        db  08Bh, 015h, 000h, 001h, 000h, 000h
        mov    al, 11h
        out    dx, al
        mov    byte ptr [edi], 5ah
        cmp    byte ptr [edi], 5ah
        jne    SHORT loc_3_00FE
        mov    byte ptr [edi], 0
        cmp    byte ptr [edi], 0
        jne    SHORT loc_3_00FE
        add    esi, esi
loc_3_00FE:
        db  089h, 035h, 0F4h, 000h, 000h, 000h
        db  08Bh, 015h, 000h, 001h, 000h, 000h
        xor    al, al
        out    dx, al
        add    dl, 2
        db  089h, 015h, 004h, 001h, 000h, 000h
        ret
loc_3_0117:
        db  0E8h, 0EBh, 0FEh, 0FCh, 0FFh
        ret
_PTEXT ends
        end
