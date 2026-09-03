; disktsd_icode_02.asm -- DISKTSD object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        mov    ah, 30h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        xchg   ah, al
        cmp    ax, 70ah
        jb     SHORT loc_2_003B
        call   sub_2_003C
        db  0C7h, 005h, 045h, 001h, 000h, 000h, 00Ch, 000h, 000h, 000h
        db  0C7h, 005h, 049h, 001h, 000h, 000h, 050h, 000h, 000h, 000h
        db  068h, 039h, 001h, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 010h, 000h
        add    esp, 4
        xor    eax, eax
loc_2_003B:
        ret
sub_2_003C:
        sub    esp, 10h
        db  08Dh, 035h, 0DCh, 000h, 000h, 000h
        lea    edi, [esp]
        push   edi
        push   esi
        push   80000002h
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_2_00D7
        db  08Dh, 01Dh, 019h, 001h, 000h, 000h
loc_2_0062:
        mov    ecx, dword ptr [ebx + 4]
        or     ecx, ecx
        je     SHORT loc_2_00C5
        mov    eax, dword ptr [ebx + 8]
        lea    edx, [esp + 4]
        lea    esi, [esp + 0ch]
        lea    edi, [esp + 8]
        mov    dword ptr [edi], ecx
        mov    ecx, dword ptr [esp]
        push   edi
        push   esi
        push   edx
        push   0
        push   eax
        push   ecx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_2_00C0
        mov    eax, dword ptr [esp + 4]
        cmp    eax, dword ptr [ebx]
        je     SHORT loc_2_00A9
        cmp    eax, 4
        jne    SHORT loc_2_00A7
        cmp    dword ptr [ebx], 3
        jne    SHORT loc_2_00A7
        cmp    dword ptr [ebx + 4], 4
loc_2_00A7:
        jne    SHORT loc_2_00C0
loc_2_00A9:
        mov    edi, dword ptr [ebx + 0ch]
        mov    ecx, dword ptr [ebx + 4]
        sub    al, al
        rep stosb
        lea    esi, [esp + 0ch]
        mov    edi, dword ptr [ebx + 0ch]
        mov    ecx, dword ptr [esp + 8]
        rep movsb
loc_2_00C0:
        add    ebx, 10h
        jmp    SHORT loc_2_0062
loc_2_00C5:
        mov    eax, dword ptr [esp]
        push   eax
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
        add    esp, 10h
        clc
        ret
loc_2_00D7:
        add    esp, 10h
        stc
        ret
loc_2_00DC:
        push   ebx
        db  079h, 073h
        je     SHORT loc_2_0146
        insd
        pop    esp
        inc    ebx
        db  075h, 072h
        db  072h, 065h
        outsb
        je     SHORT loc_2_012E
        outsd
        outsb
        je     SHORT loc_2_0161
        outsd
        insb
        push   ebx
        je     NEAR PTR loc_2_0151
        inc    ebx
        outsd
        outsb
        db  074h, 072h
        outsd
        insb
        pop    esp
        inc    esi
        imul   ebp, dword ptr [ebp + 53h], 65747379h
        insd
        add    byte ptr [edi + 65h], al
        db  074h, 044h
        jbe    NEAR PTR loc_2_015F
        popad
        db  072h, 06Dh
        jae    SHORT loc_2_015D
        outsd
        arpl   word ptr [esp + ebp*2], si
loc_2_0119:
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        or     byte ptr [ecx], al
        add    byte ptr [eax], al
        mov    esp, 2
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_012E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_2_0139:
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        add    byte ptr [eax], 0
        add    byte ptr [eax], al
loc_2_0146:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi + 69h], al
        db  078h, 065h
loc_2_0151:
        and    byte ptr fs:[ecx + ebp*2 + 73h], al
        imul   esp, dword ptr [eax], 54h
        push   ebx
        inc    esp
        and    byte ptr [eax], ah
loc_2_015D:
        add    byte ptr [eax], al
loc_2_015F:
        add    byte ptr [eax], al
loc_2_0161:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
_ITEXT ends
        end
