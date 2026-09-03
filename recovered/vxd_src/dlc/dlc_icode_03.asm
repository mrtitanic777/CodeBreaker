; dlc_icode_03.asm -- DLC object 3 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

sub_3_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_000E:
        mov    dword ptr [ebp - 4], eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0016:
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 1
        jne    NEAR PTR loc_3_00E9
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 4
        je     NEAR PTR loc_3_004F
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_004C:
        add    esp, 10h
loc_3_004F:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 4
        je     NEAR PTR loc_3_0090
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_008D:
        add    esp, 14h
loc_3_0090:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 4
        je     NEAR PTR loc_3_00C8
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_00C5:
        add    esp, 10h
loc_3_00C8:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 4
        je     NEAR PTR loc_3_00E9
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_00E6:
        add    esp, 8
loc_3_00E9:
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_3_00F1
loc_3_00F1:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h
loc_3_00F8:
        dec    ecx
        dec    esi
        dec    ecx
        cmp    ah, byte ptr [eax]
        jbe    SHORT loc_3_0164
        jb     SHORT loc_3_0174
        imul   ebp, dword ptr [edi + 6eh], 20732520h
        bound  esi, qword ptr [ebp + 69h]
        insb
        db  074h, 020h
        outsd
        outsb
        and    byte ptr [74612073h], ah
        and    byte ptr [0a73h], ah
loc_3_011C:
        dec    ecx
        dec    esi
        dec    ecx
        cmp    ah, byte ptr [eax]
        jae    SHORT loc_3_018C
        jp     SHORT loc_3_018A
        jae    SHORT loc_3_0147
        sub    eax, 42434120h
        cmp    eax, 41206425h
        dec    edi
        inc    ebx
        inc    edx
        cmp    eax, 53206425h
        inc    ecx
        push   eax
        cmp    eax, 4c206425h
        imul   ebp, dword ptr [esi + 6bh], 0a64253dh
loc_3_0147:
        add    byte ptr [ecx + 4eh], cl
        dec    ecx
        cmp    ah, byte ptr [eax]
        db  073h, 069h
        db  07Ah, 065h
        jae    SHORT loc_3_0173
        sub    eax, 58554120h
        pop    edi
        inc    ebx
        inc    ebx
        inc    edx
        cmp    eax, 53206425h
        dec    ebp
        inc    ecx
        dec    esp
loc_3_0164:
        dec    esp
        pop    edi
        inc    esi
        push   edx
        inc    ecx
        dec    ebp
        inc    ebp
        cmp    eax, 43206425h
        inc    ebx
        inc    edx
        pop    edi
loc_3_0173:
        push   eax
loc_3_0174:
        db  072h, 06Fh
        db  078h, 079h
        cmp    eax, 0a6425h
        add    byte ptr [eax], al
        add    byte ptr [4e490a21h], ah
        dec    ecx
        cmp    ah, byte ptr [eax]
        inc    esp
        dec    esp
loc_3_018A:
        inc    ebx
        pop    edi
loc_3_018C:
        inc    esp
        db  079h, 06Eh
        popad
        insd
        imul   esp, dword ptr [ebx + 5fh], 74696e49h
        sub    byte ptr [ecx], ch
        and    byte ptr [ebp + 6ch], ah
        popad
        db  070h, 073h
        and    byte ptr fs:[ecx + ebp*2 + 6dh], dh
        and    byte ptr gs:[ecx + 73h], ch
        and    byte ptr [0a6ch], ah
_ITEXT ends
        end
