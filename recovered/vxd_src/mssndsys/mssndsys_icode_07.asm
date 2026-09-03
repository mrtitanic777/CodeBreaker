; mssndsys_icode_07.asm -- MSSNDSYS object 7 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_7_0000:
        push   0
        push   4
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_7_0013
loc_7_0011:
        stc
        ret
loc_7_0013:
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, 5
        mov    ecx, 4
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        jb     SHORT loc_7_0011
        db  089h, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        push   ebx
loc_7_003D:
        mov    eax, 5
        mov    ecx, 34h
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        jb     SHORT loc_7_00B2
        mov    dword ptr [edi + ebx], esi
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, dword ptr [esp]
        jne    SHORT loc_7_003D
        pop    ebx
        mov    eax, 15000eh
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        mov    al, 49h
        mov    ah, 1
        mov    ebx, 0f3830180h
        mov    cl, 85h
        db  0BEh, 000h, 000h, 000h, 000h
        xor    edx, edx
        xor    edi, edi
        db  0CDh, 020h, 001h, 000h, 00Dh, 000h
        jb     SHORT loc_7_00B0
        db  0A3h, 000h, 000h, 000h, 000h
        mov    al, 51h
        mov    ah, 1
        mov    ebx, 0f3830180h
        mov    cl, 85h
        db  0BEh, 000h, 000h, 000h, 000h
        xor    edx, edx
        xor    edi, edi
        db  0CDh, 020h, 001h, 000h, 00Dh, 000h
        jb     SHORT loc_7_00B0
        db  0A3h, 000h, 000h, 000h, 000h
loc_7_00B0:
        clc
        ret
loc_7_00B2:
        pop    ebx
        jmp    NEAR PTR loc_7_0011
_ITEXT ends
        end
