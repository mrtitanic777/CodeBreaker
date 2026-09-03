; combuff_icode_02.asm -- COMBUFF object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        mov    ecx, eax
        db  0CDh, 020h, 000h, 000h, 00Eh, 000h
        cmp    eax, 300h
        jb     NEAR PTR loc_2_0100
        cmp    eax, 400h
        jae    SHORT loc_2_0024
        cmp    ecx, 0
        je     SHORT loc_2_002D
        jmp    NEAR PTR loc_2_00FF
loc_2_0024:
        cmp    ecx, 1
        jne    NEAR PTR loc_2_00FF
loc_2_002D:
        xor    ecx, ecx
loc_2_002F:
        inc    ecx
        push   ecx
        lea    eax, [ecx + 30h]
        db  0A2h, 037h, 001h, 000h, 000h
        db  0A2h, 042h, 001h, 000h, 000h
        db  0A2h, 054h, 001h, 000h, 000h
        mov    eax, 100h
        xor    esi, esi
        db  0BFh, 034h, 001h, 000h, 000h
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        test   eax, eax
        je     SHORT loc_2_00D8
        cmp    eax, 2710h
        jb     SHORT loc_2_0065
        mov    eax, 2710h
loc_2_0065:
        push   eax
        push   ecx
        xchg   ecx, eax
        mov    ebx, 1
        add    ecx, ecx
        add    ecx, 13h
        xor    edx, edx
        db  0BEh, 0F4h, 001h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 00Eh, 000h
        mov    ebx, eax
        pop    ecx
        pop    eax
        jb     SHORT loc_2_00D8
        mov    word ptr [ebx + 85h], ax
        xor    edx, edx
        xor    esi, esi
        db  0BFh, 03Fh, 001h, 000h, 000h
        db  0CDh, 020h, 0B3h, 000h, 001h, 000h
        mov    byte ptr [ebx + 7dh], 0
        test   edx, edx
        je     SHORT loc_2_00B5
        db  0BEh, 04Ch, 001h, 000h, 000h
        mov    edi, edx
        call   sub_2_0101
        jb     SHORT loc_2_00B5
        mov    byte ptr [ebx + 7dh], 1
loc_2_00B5:
        mov    eax, 2
        xor    esi, esi
        db  0BFh, 051h, 001h, 000h, 000h
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        cmp    eax, 1
        jb     SHORT loc_2_00D4
        ja     SHORT loc_2_00D8
        mov    byte ptr [ebx + 7eh], 0ffh
        jmp    SHORT loc_2_00D8
loc_2_00D4:
        mov    byte ptr [ebx + 7eh], 1
loc_2_00D8:
        pop    ecx
        cmp    ecx, 4
        jb     NEAR PTR loc_2_002F
        db  0CDh, 020h, 000h, 000h, 00Eh, 000h
        cmp    eax, 190h
        jb     SHORT loc_2_00FF
        mov    eax, 0e000bh
        db  0BEh, 098h, 000h, 000h, 000h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
loc_2_00FF:
        clc
loc_2_0100:
        ret
sub_2_0101:
        pushad
        xor    ecx, ecx
        cld
loc_2_0105:
        lodsb
        test   al, al
        je     SHORT loc_2_0123
        call   sub_2_0126
        mov    bl, al
        mov    al, byte ptr [edi]
        inc    edi
        call   sub_2_0126
        cmp    al, bl
        jne    SHORT loc_2_0120
        inc    ecx
        jmp    SHORT loc_2_0105
loc_2_0120:
        stc
        jmp    SHORT loc_2_0124
loc_2_0123:
        clc
loc_2_0124:
        popad
        ret
sub_2_0126:
        cmp    al, 61h
        jb     SHORT loc_2_0130
        cmp    al, 7ah
        ja     SHORT loc_2_0130
        sub    al, 20h
loc_2_0130:
        ret
        db  000h, 000h, 000h
loc_2_0134:
        inc    ebx
        dec    edi
        dec    ebp
loc_2_0137:
        add    byte ptr [edx + 55h], al
        inc    esi
        inc    esi
        inc    ebp
        push   edx
        db  000h
loc_2_013F:
        inc    ebx
        dec    edi
        dec    ebp
loc_2_0142:
        add    byte ptr [eax + 52h], dl
        dec    edi
        push   esp
        dec    edi
        inc    ebx
        dec    edi
        dec    esp
        add    byte ptr [eax + 4fh], bl
        inc    esi
        inc    esi
        add    byte ptr [ebx + 4fh], al
        dec    ebp
loc_2_0154:
        add    byte ptr [esi + 49h], al
        inc    esi
        dec    edi
        add    byte ptr [eax], al
        db  000h
_ITEXT ends
        end
