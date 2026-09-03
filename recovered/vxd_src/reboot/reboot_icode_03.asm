; reboot_icode_03.asm -- REBOOT object 3 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_3_0000:
        mov    eax, 19h
        db  0BEh, 0DFh, 000h, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        mov    ax, 0ff53h
        mov    ebx, 1800180h
        mov    cl, 1
        db  0BEh, 03Bh, 000h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 00Dh, 000h
        xor    esi, esi
        db  0BFh, 0EEh, 000h, 000h, 000h
        mov    eax, 0ffffffffh
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        cmp    eax, -1
        je     SHORT loc_3_0044
        db  080h, 00Dh, 0E4h, 002h, 000h, 000h, 001h
loc_3_0044:
        xor    esi, esi
        db  0BFh, 0FAh, 000h, 000h, 000h
        xor    eax, eax
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        test   eax, eax
        je     SHORT loc_3_005E
        db  080h, 00Dh, 0E4h, 002h, 000h, 000h, 040h
loc_3_005E:
        xor    esi, esi
        db  0BFh, 0E4h, 000h, 000h, 000h
        xor    eax, eax
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        test   eax, eax
        je     SHORT loc_3_007C
        db  0BEh, 06Fh, 001h, 000h, 000h
        db  0CDh, 020h, 018h, 000h, 001h, 000h
loc_3_007C:
        db  068h, 0B8h, 002h, 000h, 000h
        db  068h, 068h, 002h, 000h, 000h
        db  0CDh, 020h, 029h, 001h, 001h, 000h
        pop    ecx
        pop    ecx
        db  0CDh, 020h, 0B7h, 000h, 001h, 000h
        mov    al, byte ptr [edx]
        and    al, 0dfh
        sub    al, 41h
        db  0A2h, 0E7h, 002h, 000h, 000h
        clc
        ret
        db  000h, 000h, 000h
loc_3_00A4:
        push   eax
        mov    eax, esp
        push   0
        push   eax
        push   1
        push   0ffh
        db  0CDh, 020h, 061h, 000h, 001h, 000h
        add    esp, 10h
        pop    eax
        shr    eax, 0ch
        cmp    eax, 0ffh
        jne    SHORT loc_3_00E0
        db  0BEh, 020h, 002h, 000h, 000h
        db  0CDh, 020h, 0E9h, 000h, 001h, 000h
        jb     SHORT loc_3_00E0
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 050h
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 053h
loc_3_00E0:
        clc
        ret
        db  000h, 000h
loc_3_00E4:
        dec    esi
        dec    ebp
        dec    ecx
        push   edx
        inc    ebp
        inc    edx
        dec    edi
        dec    edi
        push   esp
        db  000h
loc_3_00EE:
        dec    esp
        dec    edi
        inc    ebx
        inc    ecx
        dec    esp
        push   edx
        inc    ebp
        inc    edx
        dec    edi
        dec    edi
        push   esp
        db  000h
loc_3_00FA:
        inc    esp
        bound  esi, qword ptr gs:[ebp + 67h]
        dec    esp
        outsd
        arpl   word ptr [ecx + 6ch], sp
        push   edx
        bound  ebp, qword ptr gs:[edi + 6fh]
        db  074h, 000h
        db  000h
_ITEXT ends
        end
