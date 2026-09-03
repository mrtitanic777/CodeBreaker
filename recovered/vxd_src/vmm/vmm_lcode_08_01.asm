; vmm_lcode_08_01.asm -- VMM object 8 segment _LTEXT  (part 1/2)
.386p
_LTEXT segment dword public use32 'LCODE'

        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 053h, 079h, 073h, 074h, 065h, 06Dh, 05Ch, 043h
        db  075h, 072h, 072h, 065h, 06Eh, 074h, 043h, 06Fh, 06Eh, 074h, 072h, 06Fh, 06Ch, 053h, 065h, 074h
        db  05Ch, 053h, 065h, 072h, 076h, 069h, 063h, 065h, 073h, 05Ch, 056h, 078h, 044h
loc_8_069D:
        pop    esp
        push   esi
        dec    ebp
        dec    ebp
        add    byte ptr [eax], al
        add    byte ptr [edi + ecx*2 + 53h], al
        push   eax
        popad
        jb     NEAR PTR loc_8_06AD
loc_8_06AD:
        add    byte ptr [eax], al
        add    byte ptr [esi + 58h], dl
        inc    esp
        inc    edi
        jb     SHORT loc_8_0725
        db  075h, 070h
        jae    SHORT loc_8_06BA
loc_8_06BA:
        add    byte ptr [eax], al
        inc    ebp
        outsb
        jne    SHORT loc_8_072D
        pop    esp
        push   edx
        outsd
        outsd
        db  074h, 05Ch
        sub    dl, byte ptr [eax + 4eh]
        push   eax
        xor    byte ptr [ebx + 30h], al
        xor    dword ptr [eax + esi + 30h], ebx
        xor    byte ptr [eax], dh
        add    byte ptr [ebx + 6fh], al
        insd
        db  070h, 075h
        db  074h, 065h
        jb     SHORT loc_8_072B
        popad
        insd
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
        push   ebx
        db  079h, 073h
        db  074h, 065h
        insd
        pop    esp
        inc    ebx
        db  075h, 072h
        jb     SHORT loc_8_0755
        outsb
        db  074h, 043h
        outsd
        outsb
        je     SHORT loc_8_0769
        outsd
        insb
        push   ebx
        je     NEAR PTR loc_8_0759
        push   ebx
        jb     NEAR PTR loc_8_0777
        imul   esp, dword ptr [ebx + 65h], 78565c73h
        inc    esp
        pop    esp
        push   esi
        dec    ebp
        dec    ebp
        add    byte ptr [eax], al
        add    byte ptr [edi + ecx*2 + 53h], al
        push   eax
        popad
        jb     NEAR PTR loc_8_0719
loc_8_0719:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_8_0725:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_8_072B:
        add    byte ptr [eax], al
loc_8_072D:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_8_0755:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_8_0759:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_8_0769:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_8_0777:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        xor    byte ptr [ecx], dh
        xor    dh, byte ptr [ebx]
        xor    al, 35h
        aaa
        cmp    byte ptr [ecx], bh
        inc    ecx
        inc    edx
        inc    ebx
        inc    esp
        inc    ebp
        inc    esi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 6fh], al
        outsb
        jae    SHORT loc_8_0B9E
        db  072h, 076h
        popad
        db  074h, 069h
        db  076h, 065h
        push   ebx
        ja     SHORT loc_8_0BA4
        db  070h, 066h
        imul   ebp, dword ptr [ebp + 55h], 65676173h
        add    byte ptr [ebx + 61h], al
        arpl   word ptr [eax + 65h], bp
        dec    ebp
        imul   esp, dword ptr [eax + edx*2 + 6fh], 746e69h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_8_0B9E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_8_0BA4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_8_0BD8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   ebx
        jns    SHORT loc_8_0D06
        je     SHORT loc_8_0CFA
        insd
        pop    esp
        inc    ebx
        jne    SHORT loc_8_0D0C
        jb     SHORT loc_8_0D01
        outsb
        db  074h, 043h
        outsd
        outsb
        db  074h, 072h
        outsd
        insb
        push   ebx
        db  065h, 074h, 05Ch
        push   ebx
        jb     NEAR PTR loc_8_0D23
        imul   esp, dword ptr [ebx + 65h], 72415c73h
        bound  ebp, qword ptr [ecx + 74h]
        jb     SHORT loc_8_0D1A
        db  074h, 06Fh
        db  072h, 073h
        pop    esp
        inc    ecx
        jb     NEAR PTR loc_8_0D04
        jb     SHORT loc_8_0D27
        add    byte ptr [edi + ebp*2 + 6eh], al
        je     SHORT loc_8_0D0C
        insb
        insb
        outsd
        arpl   word ptr [ecx + 73h], cx
        je     SHORT loc_8_0D21
        insd
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], cl
        add    byte ptr [eax], dl
        or     cl, byte ptr [eax + eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        cwde
        test   byte ptr [eax], 0
        db  0C4h, 0F6h, 000h, 000h, 040h, 0C3h, 000h, 000h, 014h, 0F7h
loc_8_0CFA:
        add    byte ptr [eax], al
        clc
        jecxz  loc_8_0CFF
loc_8_0CFF:
        add    byte ptr [eax], al
loc_8_0D01:
        add    byte ptr [eax], al
        db  000h
loc_8_0D04:
        add    byte ptr [eax], al
loc_8_0D06:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_8_0D0C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_8_0D1A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_8_0D21:
        add    byte ptr [eax], al
loc_8_0D23:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_8_0D27:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 038h
        add    dword ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    ebp
        imul   ebp, dword ptr [esi + 53h], 7350h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esp
        bound  esi, qword ptr gs:[ebp + 67h]
        sub    eax, 63617254h
        add    byte ptr gs:[ebp + 62h], al
        db  075h, 067h
        push   esp
        jb     SHORT loc_8_0E71
        arpl   word ptr [ebp + 42h], sp
        jne    SHORT loc_8_0E7B
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_8_0E71:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_8_0E7B:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx], ch
        ror    byte ptr [ebx], 5
        inc    eax
        add    dword ptr [eax], eax
        add    bl, al
        test   eax, 0fffffff0h
        jne    SHORT loc_8_1015
        db  0A3h, 040h, 001h, 000h, 000h
loc_8_1015:
        ret
        db  000h, 000h, 0C7h, 005h, 0DCh, 0C5h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0F8h, 0C3h, 0C7h, 005h
        db  0DCh, 0C5h, 000h, 000h, 000h, 0F0h, 0FFh, 0FFh, 0F8h, 0C3h, 0C7h, 005h, 0DCh, 0C5h, 000h, 000h
        db  000h, 0FFh, 0FFh, 0FFh, 0F8h, 0C3h, 0FFh, 035h, 0F4h, 0D9h, 000h, 000h, 066h, 0CBh
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        jne    NEAR PTR loc_8_10FC
        test   byte ptr [ebx], 20h
        je     NEAR PTR loc_8_10FC
        mov    ebx, dword ptr [ebx + 8]
        mov    edx, es
        mov    eax, dword ptr [ebx + 34h]
        mov    edi, dword ptr [ebx + 30h]
        mov    es, eax
        lar    eax, eax
        test   eax, 400000h
        jne    SHORT loc_8_1085
        movzx  edi, di
        cmp    dword ptr [ebp + 8], 0
        je     SHORT loc_8_10FA
loc_8_1085:
        push   edi
        push   dword ptr [ebx + 28h]
        push   dword ptr [ebx + 24h]
        mov    ecx, dword ptr [ebp + 10h]
        sub    edi, ecx
        sub    edi, 4
        mov    dword ptr [ebx + 30h], edi
        db  08Dh, 005h, 03Ch, 000h, 000h, 000h
        cmp    dword ptr [ebp + 8], 0
        je     SHORT loc_8_10AF
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [ebx + 28h], ax
        db  0A1h, 0F4h, 0D9h, 000h, 000h
loc_8_10AF:
        cld
        stosd
        mov    esi, dword ptr [ebp + 14h]
        rep movsb
        mov    es, edx
        push   dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebx + 24h], eax
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        push   dword ptr [edi - 2ch]
        lea    ebx, [esp - 4]
        mov    dword ptr [edi - 2ch], ebx
        db  0E8h, 02Ah, 002h, 0FAh, 0FFh
        pop    dword ptr [edi - 2ch]
        pop    ecx
        pop    dword ptr [ebp + 24h]
        pop    dword ptr [ebp + 28h]
        pop    dword ptr [ebp + 30h]
        mov    eax, dword ptr [ebp + 1ch]
        jecxz  loc_8_10F5
        mov    ax, word ptr [ebp + 14h]
        shl    eax, 10h
        mov    ax, word ptr [ebp + 1ch]
        xor    ecx, ecx
loc_8_10F5:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_8_10FA:
        mov    es, edx
loc_8_10FC:
        or     eax, 0ffffffffh
        mov    ecx, eax
        jmp    SHORT loc_8_10F5
        db  090h, 081h, 03Dh, 0D8h, 0E5h, 000h, 000h, 0D1h
loc_8_110B:
        adc    byte ptr [eax], al
        db  000h, 074h
loc_8_110F:
        db  02Bh, 0BEh, 01Ch, 001h, 000h, 000h
        db  0CDh, 020h, 0FBh, 080h, 001h, 000h
        nop
        test   byte ptr [ebx], 20h
        je     SHORT loc_8_1144
        db  08Bh, 015h, 0F4h, 0D9h, 000h, 000h
        cmp    dx, word ptr [ebp + 24h]
        jne    SHORT loc_8_1144
        shr    edx, 10h
        cmp    dx, word ptr [ebp + 28h]
        jne    SHORT loc_8_1144
        db  0E8h, 02Ch, 022h, 0FAh, 0FFh
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        mov    esp, dword ptr [edi - 2ch]
loc_8_1144:
        ret
        db  00Fh, 0A0h, 00Fh, 0A8h, 09Ch, 060h, 08Bh, 0D4h, 083h, 0ECh, 06Ch, 08Bh, 0FCh, 0CDh, 020h, 08Dh
        db  000h, 001h, 000h, 08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h, 08Bh, 06Bh, 008h, 068h, 06Eh, 001h, 000h
        db  000h, 060h, 08Bh, 013h, 0E9h, 03Fh, 023h, 0FAh, 0FFh, 080h, 04Bh, 001h, 001h, 08Bh, 0F2h, 0FCh
        db  08Bh, 0FDh, 0B9h, 008h, 000h, 000h, 000h, 0F3h, 0A5h, 0ADh, 025h, 0FFh, 08Dh, 0FCh, 0FFh, 081h
        db  065h, 02Ch, 000h, 072h, 003h, 000h
loc_8_118B:
        or     dword ptr [ebp + 2ch], eax
        lea    edi, [ebp + 38h]
        mov    ecx, 4
        db  00Fh, 0B7h, 005h, 06Ah, 0F6h, 000h, 000h
        rep stosd
        db  08Bh, 042h
loc_8_11A1:
        xor    ch, cl
        and    byte ptr [eax + eax + 12730001h], al
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        mov    al, byte ptr [edi - 33h]
        cmp    al, byte ptr [edi - 32h]
        jbe    SHORT loc_8_11BC
        or     dword ptr [ebp + 2ch], 1
loc_8_11BC:
        mov    edi, edx
        cld
        mov    esi, ebp
        mov    ecx, 8
        rep movsd
        mov    eax, dword ptr [ebp + 2ch]
        and    eax, 0fffc8fffh
        or     eax, 200h
        stosd
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        mov    esi, esp
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        add    esp, 6ch
        popad
        popfd
        pop    gs
        pop    fs
        ret    4
        db  053h, 051h, 057h, 08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h, 08Bh, 03Dh, 098h, 0F6h, 000h, 000h, 068h
        db  00Ch, 002h, 000h, 000h, 060h, 08Bh, 013h, 0E9h, 0A1h, 022h, 0FAh, 0FFh, 081h, 00Bh, 000h, 001h
        db  000h, 000h, 0CDh, 020h, 084h, 000h, 001h, 000h, 09Ch, 0CDh, 020h, 086h, 000h, 001h, 000h, 09Dh
        db  05Fh, 059h, 05Bh, 0C3h, 083h, 045h, 024h, 002h, 00Fh, 0B6h, 0D5h, 068h, 02Fh, 02Eh, 000h, 000h
        db  0E9h, 0C4h, 02Ah, 0FAh, 0FFh, 02Eh, 08Bh, 0C0h, 0FFh, 045h, 024h, 00Fh, 0B7h, 0D2h, 0E8h, 0B6h
        db  02Ah, 0FAh, 0FFh, 08Bh, 0FAh, 0D0h, 0C2h, 032h, 0D6h, 00Fh, 0B6h, 0D2h, 08Dh, 014h, 052h, 0D1h
        db  0EAh, 087h, 0D7h, 066h, 039h, 014h, 07Dh, 030h, 0DAh, 000h, 000h, 075h, 017h, 051h, 0FFh, 014h
        db  0BDh, 060h, 0DDh, 000h, 000h, 059h, 083h, 0F9h, 008h, 075h, 005h, 066h, 089h, 045h, 01Ch, 0C3h
        db  089h, 045h, 01Ch, 0C3h, 068h, 05Dh, 002h, 000h, 000h, 0E9h, 0EEh, 02Ah, 0FAh, 0FFh, 08Bh, 0C0h
        db  083h, 045h, 024h, 002h, 00Fh, 0B6h, 0D5h, 068h, 043h, 002h, 000h, 000h, 0E9h, 068h, 02Ah, 0FAh
        db  0FFh, 000h, 000h, 000h, 0FCh, 066h, 08Ch, 0D7h, 066h, 083h, 0FFh, 030h, 00Fh, 084h, 023h, 000h
        db  0FAh, 0FFh, 066h, 0BFh, 030h, 000h, 01Eh, 006h, 00Fh, 0A0h, 00Fh, 0A8h, 08Eh, 0DFh, 066h, 08Ch
        db  015h, 0B2h, 002h, 000h, 000h, 089h, 025h, 0AEh, 002h, 000h, 000h, 02Eh, 00Fh, 0B2h, 025h, 0A8h
        db  002h, 000h, 000h, 083h, 0ECh, 06Ch, 08Bh, 0ECh, 056h, 08Eh, 0C7h, 02Eh, 0C5h, 035h, 0AEh, 002h
        db  000h, 000h, 083h, 0C6h, 010h, 0B9h, 01Bh, 000h, 000h, 000h, 08Bh, 0FDh, 0F3h, 0A5h, 066h, 0BFh
        db  030h, 000h, 08Eh, 0DFh, 033h, 0C9h, 08Eh, 0E1h, 08Eh, 0E9h, 05Eh, 09Ch, 00Eh, 068h, 0FBh, 002h
        db  000h, 000h, 083h, 0ECh, 004h, 060h, 0E9h, 0CCh, 0FFh, 0F9h, 0FFh, 02Eh, 00Fh, 0B2h, 025h, 0AEh
        db  002h, 000h, 000h, 00Fh, 0A9h, 00Fh, 0A1h, 007h, 01Fh, 061h, 083h, 0C4h, 004h, 0CFh
        push   ebp
        mov    ebp, esp
        and    esp, 0fffffffch
        pushad
        mov    ecx, 8
loc_8_131A:
        push   0
        loop   loc_8_131A
        push   esp
loc_8_131F:
        db  0CDh, 020h, 00Ah, 000h, 011h, 000h
        pop    ecx
        push   es
        push   fs
        push   gs
        push   ds
        push   0
        db  0C7h, 005h, 0B4h, 002h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    edi, dword ptr [ebp + 28h]
        test   dword ptr [ebp + 2ch], 0ffffffffh
        jne    NEAR PTR loc_8_14D6
        mov    ax, ss
        cmp    ax, 30h
        jne    NEAR PTR loc_8_14D6
        mov    ax, ds
        cmp    ax, 30h
        jne    NEAR PTR loc_8_14D6
        mov    ax, es
        cmp    ax, 30h
        jne    NEAR PTR loc_8_14D6
        mov    ebx, dword ptr [ebp + 0ch]
        cmp    ebx, -1
        je     SHORT loc_8_13C5
        lar    eax, ebx
        jne    NEAR PTR loc_8_14D6
        bt     eax, 16h
        jb     SHORT loc_8_13C5
        mov    word ptr [ebp + 0ah], bx
        push   1
        push   0
        push   9bh
        push   0ah
        db  068h, 0A0h, 004h, 000h, 000h
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        push   0
        push   eax
        push   edx
        db  0CDh, 020h, 076h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     NEAR PTR loc_8_14D6
        shl    eax, 10h
        db  0A3h, 0B4h, 002h, 000h, 000h
        mov    ebx, eax
loc_8_13C5:
        push   0
        db  068h, 0B4h, 002h, 000h, 000h
        db  068h, 0A8h, 002h, 000h, 000h
        db  0CDh, 020h, 094h, 001h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     NEAR PTR loc_8_14D6
        or     dword ptr [esp], 1
        push   0
        db  068h, 0D6h, 004h, 000h, 000h
        db  068h, 094h, 002h, 000h, 000h
        db  0CDh, 020h, 094h, 001h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     NEAR PTR loc_8_14D6
        or     dword ptr [esp], 2
        db  0C6h, 005h, 027h, 004h, 000h, 000h, 0FFh
        db  0C6h, 005h, 032h, 004h, 000h, 000h, 045h
        db  0B8h, 094h, 002h, 000h, 000h
        db  02Dh, 0C0h, 002h, 000h, 000h
        sub    eax, 5
        cli
        db  0C6h, 005h, 0C0h, 002h, 000h, 000h, 0E9h
        db  0A3h, 0C1h, 002h, 000h, 000h
        sti
        db  089h, 025h, 0A8h, 002h, 000h, 000h
        mov    ecx, dword ptr [ebp + 20h]
        mov    esi, dword ptr [ebp + 24h]
        mov    edx, dword ptr [ebp + 8]
        sub    dword ptr [ebp + 10h], 6ch
        lss    esp, ptr [ebp + 10h]
        sub    esp, ecx
        sub    esp, ecx
        mov    edi, esp
        mov    ax, 30h
        mov    ds, ax
        mov    ax, ss
        mov    es, ax
        cld
        rep movsw
        mov    eax, dword ptr cs:[ebp + 18h]
        cmp    eax, -1
        jne    SHORT loc_8_146C
        mov    eax, 30h
loc_8_146C:
        mov    ds, eax
        mov    eax, dword ptr cs:[ebp + 1ch]
        cmp    eax, -1
        jne    SHORT loc_8_147C
        mov    eax, 30h
loc_8_147C:
        mov    es, eax
        xor    ax, ax
        mov    fs, ax
        mov    gs, ax
        cmp    ebx, -1
        je     SHORT loc_8_14B5
        db  02Eh, 083h, 03Dh, 0B4h, 002h, 000h, 000h, 000h
        je     SHORT loc_8_14AB
        db  02Eh, 0FFh, 035h, 0B4h, 002h, 000h, 000h
        push   edx
        retf
        db  066h, 06Ah, 028h, 066h, 068h, 0B7h, 004h, 000h, 000h, 066h, 0CBh
loc_8_14AB:
        push   28h
        db  068h, 0B7h, 004h, 000h, 000h
        push   ebx
        push   edx
        retf
loc_8_14B5:
        call   edx
        db  02Eh, 00Fh, 0B2h, 025h, 0A8h, 002h, 000h, 000h
        mov    edi, eax
        cli
        db  036h, 0C6h, 005h, 0C0h, 002h, 000h, 000h, 0FCh
        db  036h, 0C7h, 005h, 0C1h, 002h, 000h, 000h, 066h, 0BFh, 030h, 000h
        sti
loc_8_14D6:
        pop    esi
        pop    ds
        pop    gs
        pop    fs
        pop    es
        push   esp
        db  0CDh, 020h, 00Bh, 000h, 011h, 000h
        pop    ecx
        add    esp, 20h
        test   esi, 2
        je     SHORT loc_8_1505
        push   0
        db  068h, 0D6h, 004h, 000h, 000h
        db  068h, 094h, 002h, 000h, 000h
        db  0CDh, 020h, 095h, 001h, 001h, 000h
        add    esp, 0ch
loc_8_1505:
        test   esi, 1
        je     SHORT loc_8_1522
        push   0
        db  068h, 0B4h, 002h, 000h, 000h
        db  068h, 0A8h, 002h, 000h, 000h
        db  0CDh, 020h, 095h, 001h, 001h, 000h
        add    esp, 0ch
loc_8_1522:
        mov    dword ptr [esp + 1ch], edi
        db  0C6h, 005h, 032h, 004h, 000h, 000h, 065h
        db  0C6h, 005h, 027h, 004h, 000h, 000h, 0FCh
        db  08Bh, 00Dh, 0B4h, 002h, 000h, 000h
        jecxz  loc_8_154A
        shr    ecx, 10h
        push   0
        push   ecx
        db  0CDh, 020h, 077h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_8_154A:
        popad
        leave
        ret
        db  000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        cld
        mov    edi, dword ptr [ebp + 8]
        add    edi, 0ch
        mov    edx, edi
        mov    al, 20h
loc_8_1560:
        push   8
        pop    ecx
        repne scasb
        neg    cl
        add    cl, 8
        mov    eax, ecx
        jecxz  loc_8_1594
        add    eax, 27h
        cmp    eax, dword ptr [ebp + 10h]
        ja     SHORT loc_8_1594
        push   ecx
        mov    edi, dword ptr [ebp + 0ch]
        db  0BEh, 020h, 003h, 000h, 000h
        mov    ecx, 25h
        rep movsb
        pop    ecx
        mov    byte ptr [edi], 5ch
        inc    edi
        mov    esi, edx
        rep movsb
        mov    byte ptr [edi], 0
loc_8_1594:
        pop    edi
        pop    esi
        leave
        ret
sub_8_1598:
        mov    ecx, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 4]
        jecxz  loc_8_15AC
        mov    eax, dword ptr [esp + 10h]
        add    eax, edx
        cmp    ecx, dword ptr [eax]
        jne    SHORT loc_8_15DD
loc_8_15AC:
        mov    ecx, dword ptr [esp + 8]
        jecxz  loc_8_15CF
        or     edx, edx
        je     SHORT loc_8_15D3
        mov    eax, edx
        add    eax, ecx
        jb     SHORT loc_8_15DD
        and    edx, 0fffff000h
loc_8_15C2:
        test   byte ptr [edx], 55h
        add    edx, 1000h
        cmp    edx, eax
        jb     SHORT loc_8_15C2
loc_8_15CF:
        xor    eax, eax
        inc    eax
        ret
loc_8_15D3:
        test   dword ptr [esp + 14h], 1
        jne    SHORT loc_8_15CF
loc_8_15DD:
        xor    eax, eax
        stc
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 083h
        db  0ECh, 040h, 053h, 056h, 057h, 055h, 08Bh, 0FAh, 08Bh, 0F1h, 02Bh, 0DBh, 00Fh, 0B7h, 04Ch, 024h
        db  054h, 08Bh, 0C7h, 02Bh, 0D2h, 0F7h, 0F1h, 08Bh, 0EAh, 08Bh, 0C7h, 02Bh, 0D2h, 0F7h, 0F1h, 08Bh
        db  0F8h, 083h, 0FDh, 00Ah, 0B8h, 030h, 000h, 000h, 000h, 072h, 00Dh, 083h, 07Ch, 024h, 060h, 001h
        db  01Bh, 0C0h, 083h, 0E0h, 020h, 083h, 0C0h, 037h, 08Dh, 004h, 028h, 088h, 044h, 01Ch, 010h, 043h
        db  00Bh, 0FFh, 075h, 0CDh, 08Bh, 054h, 024h, 058h, 00Bh, 0D2h, 074h, 035h, 083h, 0FAh, 040h, 072h
        db  005h, 0BAh, 040h, 000h, 000h, 000h, 03Bh, 0DAh, 073h, 027h, 00Fh, 0BEh, 044h, 024h, 05Ch, 08Ah
        db  0E8h, 08Ah, 0C8h, 02Bh, 0D3h, 08Bh, 0C1h, 0C1h, 0E0h, 010h, 066h, 08Bh, 0C1h, 08Dh, 07Ch, 01Ch
        db  010h, 08Bh, 0CAh, 0C1h, 0E9h, 002h, 0F3h, 0ABh, 08Bh, 0CAh, 083h, 0E1h, 003h, 0F3h, 0AAh, 003h
        db  0DAh, 00Bh, 0DBh, 074h, 00Ch, 04Bh, 08Ah, 044h, 01Ch, 010h, 088h, 006h, 046h, 00Bh, 0DBh, 075h
        db  0F4h, 0C6h, 006h, 000h, 05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 040h, 0C2h, 010h, 000h, 08Bh, 001h
        db  08Dh, 050h, 001h, 089h, 011h, 08Ah, 000h, 0C3h, 0F6h, 0C2h, 004h, 08Bh, 001h, 074h, 009h, 08Dh
        db  050h, 004h, 089h, 011h, 08Bh, 000h, 0EBh, 008h, 08Dh, 050h, 002h, 089h, 011h, 00Fh, 0B7h, 000h
        db  0C3h, 056h, 057h, 08Bh, 0F1h, 068h, 000h, 001h, 000h, 000h, 08Bh, 006h, 08Bh, 0FAh, 0FFh, 030h
        db  057h, 0FFh, 015h, 014h, 0D9h, 000h, 000h, 083h, 0C4h, 00Ch, 083h, 006h, 004h, 08Bh, 0C7h, 05Fh
        db  05Eh, 0C3h, 081h, 0ECh, 034h, 001h, 000h, 000h, 053h, 056h, 057h, 055h, 089h, 054h, 024h, 018h
        db  08Bh, 0F1h, 089h, 04Ch, 024h, 040h, 08Bh, 04Ch, 024h, 018h, 0E8h, 09Fh, 0FFh, 0FFh, 0FFh, 00Ah
        db  0C0h, 00Fh, 084h, 0FFh, 003h, 000h, 000h, 03Ch, 025h, 074h, 00Dh, 03Ch, 00Ah, 075h, 004h, 0C6h
        db  006h, 00Dh, 046h, 088h, 006h, 046h, 0EBh, 0DEh, 0C6h, 044h, 024h, 011h, 020h, 0C7h, 044h, 024h
        db  03Ch, 000h, 000h, 000h, 000h, 0C7h, 044h, 024h, 028h, 000h, 000h, 000h, 000h, 0C7h, 044h, 024h
        db  02Ch, 000h, 000h, 000h, 000h, 02Bh, 0EDh, 0C7h, 044h, 024h, 030h, 000h, 000h, 000h, 000h, 0C7h
        db  044h, 024h, 038h, 000h, 000h, 000h, 000h, 0C7h, 044h, 024h, 024h, 000h, 000h, 000h, 000h, 089h
        db  06Ch, 024h, 01Ch, 08Dh, 044h, 024h, 044h, 0C7h, 044h, 024h, 014h, 048h, 003h, 000h, 000h, 089h
        db  044h, 024h, 020h, 066h, 08Bh, 0BCh, 024h, 04Ch, 001h, 000h, 000h, 066h, 089h, 06Ch, 024h, 012h
        db  089h, 06Ch, 024h, 034h, 08Bh, 04Ch, 024h, 018h, 0E8h, 021h, 0FFh, 0FFh, 0FFh, 08Ah, 0D8h, 00Fh
        db  0BEh, 0C3h, 083h, 0F8h, 020h, 074h, 03Bh, 083h, 0F8h, 023h, 074h, 040h, 083h, 0F8h, 02Bh, 074h
        db  045h, 083h, 0F8h, 02Dh, 074h, 04Ah, 083h, 0F8h, 030h, 074h, 04Fh, 080h, 0FBh, 02Ah, 075h, 051h
        db  08Bh, 094h, 024h, 04Ch, 001h, 000h, 000h, 08Bh, 08Ch, 024h, 048h, 001h, 000h, 000h, 0E8h, 0F5h
        db  0FEh, 0FFh, 0FFh, 08Bh, 0E8h, 08Bh, 04Ch, 024h, 018h, 0E8h, 0E0h, 0FEh, 0FFh, 0FFh, 08Ah, 0D8h
        db  0EBh, 053h, 0C7h, 044h, 024h, 02Ch, 001h, 000h, 000h, 000h, 0EBh, 0A8h, 0C7h, 044h, 024h, 024h
        db  001h, 000h, 000h, 000h, 0EBh, 09Eh, 0C7h, 044h, 024h, 030h, 001h, 000h, 000h, 000h, 0EBh, 094h
        db  0C7h, 044h, 024h, 028h, 001h, 000h, 000h, 000h, 0EBh, 08Ah, 0C6h, 044h, 024h, 011h, 030h, 0EBh
        db  083h, 080h, 0FBh, 030h, 07Ch, 01Fh, 080h, 0FBh, 039h, 07Fh, 01Ah, 00Fh, 0BEh, 0C3h, 06Bh, 0EDh
        db  00Ah, 08Dh, 06Ch, 028h, 0D0h, 08Bh, 04Ch, 024h, 018h, 0E8h, 090h, 0FEh, 0FFh, 0FFh, 08Ah, 0D8h
        db  080h, 0FBh, 030h, 07Dh, 0E1h, 080h, 0FBh, 02Eh, 075h, 060h, 08Bh, 04Ch, 024h, 018h, 0E8h, 07Bh
        db  0FEh, 0FFh, 0FFh, 08Ah, 0D8h, 080h, 0FBh, 02Ah, 075h, 024h, 08Bh, 094h, 024h, 04Ch, 001h, 000h
        db  000h, 08Bh, 08Ch, 024h, 048h, 001h, 000h, 000h, 0E8h, 06Bh, 0FEh, 0FFh, 0FFh, 089h, 044h, 024h
        db  01Ch, 08Bh, 04Ch, 024h, 018h, 0E8h, 054h, 0FEh, 0FFh, 0FFh, 08Ah, 0D8h, 0EBh, 02Ch, 080h, 0FBh
        db  030h, 07Ch, 027h, 080h, 0FBh, 039h, 07Fh, 022h, 00Fh, 0BEh, 0C3h, 08Bh, 04Ch, 024h, 01Ch, 06Bh
        db  0C9h, 00Ah, 08Dh, 044h, 008h, 0D0h, 089h, 044h, 024h, 01Ch, 08Bh, 04Ch, 024h, 018h, 0E8h, 02Bh
        db  0FEh, 0FFh, 0FFh, 08Ah, 0D8h, 080h, 0FBh, 030h, 07Dh, 0D9h, 00Fh, 0BEh, 0C3h, 083h, 0F8h, 068h
        db  074h, 02Ch, 083h, 0F8h, 06Ch, 074h, 032h, 00Fh, 0BEh, 0C3h, 083h, 0F8h, 064h, 07Fh, 040h, 00Fh
        db  084h, 09Ch, 000h, 000h, 000h, 00Bh, 0C0h, 00Fh, 084h, 0A3h, 000h, 000h, 000h, 083h, 0F8h, 058h
        db  074h, 04Eh, 083h, 0F8h, 062h, 074h, 06Eh, 083h, 0F8h, 063h, 074h, 072h, 0EBh, 03Dh, 066h, 081h
        db  0E7h, 0F0h, 0FFh, 066h, 083h, 0CFh, 002h, 0EBh, 009h, 066h, 081h, 0E7h, 0F0h, 0FFh, 066h, 083h
        db  0CFh, 004h, 08Bh, 04Ch, 024h, 018h, 0E8h, 0D3h, 0FDh, 0FFh, 0FFh, 08Ah, 0D8h, 0EBh, 0ABh, 083h
        db  0F8h, 06Fh, 00Fh, 084h, 0AFh, 000h, 000h, 000h, 083h, 0F8h, 073h, 00Fh, 084h, 0BEh, 000h, 000h
        db  000h, 083h, 0F8h, 075h, 074h, 053h, 083h, 0F8h, 078h, 074h, 005h, 088h, 01Eh, 046h, 0EBh, 050h
        db  08Dh, 043h, 0A8h, 03Ch, 001h, 01Bh, 0C0h, 0F7h, 0D8h, 089h, 044h, 024h, 038h, 083h, 07Ch, 024h
        db  024h, 000h, 074h, 008h, 0C7h, 044h, 024h, 014h, 050h, 003h, 000h, 000h, 066h, 0C7h, 044h, 024h
        db  012h, 010h, 000h, 0EBh, 02Bh, 066h, 0C7h, 044h, 024h, 012h, 002h, 000h, 0EBh, 022h, 08Bh, 0D7h
        db  08Bh, 08Ch, 024h, 048h, 001h, 000h, 000h, 0E8h, 07Ch, 0FDh, 0FFh, 0FFh, 088h, 006h, 046h, 0EBh
        db  00Fh, 0C7h, 044h, 024h, 03Ch, 001h, 000h, 000h, 000h, 066h, 0C7h, 044h, 024h, 012h, 00Ah, 000h
        db  00Ah, 0DBh, 00Fh, 084h, 0BEh, 001h, 000h, 000h, 066h, 083h, 07Ch, 024h, 012h, 000h, 00Fh, 084h
        db  0E6h, 000h, 000h, 000h, 08Bh, 0D7h, 08Bh, 08Ch, 024h, 048h, 001h, 000h, 000h, 0E8h, 046h, 0FDh
        db  0FFh, 0FFh, 083h, 07Ch, 024h, 03Ch, 000h, 00Fh, 084h, 091h, 000h, 000h, 000h, 066h, 0F7h, 0C7h
        db  002h, 000h, 074h, 003h, 00Fh, 0BFh, 0C0h, 00Bh, 0C0h, 07Dh, 063h, 0C7h, 044h, 024h, 014h, 054h
        db  003h, 000h, 000h, 0F7h, 0D8h, 0EBh, 077h, 083h, 07Ch, 024h, 024h, 000h, 074h, 008h, 0C7h, 044h
        db  024h, 014h, 04Ch, 003h, 000h, 000h, 066h, 0C7h, 044h, 024h, 012h, 008h, 000h, 0EBh, 0A1h, 08Dh
        db  054h, 024h, 044h, 08Bh, 08Ch, 024h, 048h, 001h, 000h, 000h, 0E8h, 012h, 0FDh, 0FFh, 0FFh, 089h
        db  044h, 024h, 020h, 050h, 0FFh, 015h, 018h, 0D9h, 000h, 000h, 089h, 044h, 024h, 038h, 0C6h, 044h
        db  024h, 015h, 020h, 083h, 0C4h, 004h, 083h, 07Ch, 024h, 01Ch, 000h, 00Fh, 085h, 06Fh, 0FFh, 0FFh
        db  0FFh, 0C7h, 044h, 024h, 01Ch, 0FFh, 07Fh, 000h, 000h, 0E9h, 062h, 0FFh, 0FFh, 0FFh, 083h, 07Ch
        db  024h, 030h, 000h, 074h, 00Ah, 0C7h, 044h, 024h, 014h, 058h, 003h, 000h, 000h, 0EBh, 00Fh, 083h
        db  07Ch, 024h, 02Ch, 000h, 074h, 008h, 0C7h, 044h, 024h, 014h, 05Ch, 003h, 000h, 000h, 00Bh, 0C0h
        db  075h, 008h, 0C7h, 044h, 024h, 014h, 060h, 003h, 000h, 000h, 0FFh, 074h, 024h, 038h, 06Ah, 030h
        db  055h, 08Bh, 0D0h, 08Dh, 04Ch, 024h, 050h, 0FFh, 074h, 024h, 01Eh, 0E8h, 0DFh, 0FBh, 0FFh, 0FFh
        db  08Dh, 044h, 024h, 044h, 050h, 0FFh, 015h, 018h, 0D9h, 000h, 000h, 089h, 044h, 024h, 038h, 0C7h
        db  044h, 024h, 020h, 0FFh, 07Fh, 000h, 000h, 083h, 0C4h, 004h, 083h, 07Ch, 024h, 034h, 000h, 00Fh
        db  084h, 0B1h, 0FCh, 0FFh, 0FFh, 0FFh, 074h, 024h, 014h, 0FFh, 015h, 018h, 0D9h, 000h, 000h, 08Bh
        db  04Ch, 024h, 038h, 083h, 0C4h, 004h, 003h, 0C8h, 02Bh, 0E9h, 080h, 07Ch, 024h, 011h, 030h, 075h
        db  014h, 08Bh, 0C6h, 08Bh, 04Ch, 024h, 014h, 046h, 08Ah, 009h, 088h, 008h, 0FFh, 044h, 024h, 014h
        db  00Ah, 0C9h, 075h, 0EDh, 04Eh, 083h, 07Ch, 024h, 028h, 000h, 075h, 02Bh, 00Bh, 0EDh, 07Eh, 027h
        db  00Fh, 0BEh, 044h, 024h, 011h, 08Ah, 0E8h, 08Ah, 0C8h, 08Bh, 0D5h, 08Bh, 0C1h, 0C1h, 0E0h, 010h
        db  066h, 08Bh, 0C1h, 08Bh, 0FEh, 08Bh, 0CAh, 0C1h, 0E9h, 002h, 0F3h, 0ABh, 08Bh, 0CAh, 083h, 0E1h
        db  003h, 0F3h, 0AAh, 003h, 0F2h, 02Bh, 0EAh, 080h, 07Ch, 024h, 011h, 020h, 075h, 014h, 08Bh, 0C6h
        db  08Bh, 04Ch, 024h, 014h, 046h, 08Ah, 009h, 088h, 008h, 0FFh, 044h, 024h, 014h, 00Ah, 0C9h, 075h
        db  0EDh, 04Eh, 08Bh, 04Ch, 024h, 034h, 08Bh, 044h, 024h, 01Ch, 03Bh, 0C8h, 07Ch, 002h, 08Bh, 0C8h
        db  08Bh, 0C1h, 049h, 00Bh, 0C0h, 07Eh, 014h, 08Bh, 044h, 024h, 020h, 0FFh, 044h, 024h, 020h, 08Ah
        db  000h, 088h, 006h, 046h, 08Bh, 0C1h, 049h, 00Bh, 0C0h, 07Fh, 0ECh, 08Bh, 0C5h, 04Dh, 00Bh, 0C0h
        db  00Fh, 08Eh, 000h, 0FCh, 0FFh, 0FFh, 0C6h, 006h, 020h, 046h, 08Bh, 0C5h, 04Dh, 00Bh, 0C0h, 07Fh
        db  0F5h, 0E9h, 0F0h, 0FBh, 0FFh, 0FFh, 0C6h, 006h, 000h, 02Bh, 074h, 024h, 040h, 08Bh, 0C6h, 05Dh
        db  05Fh, 05Eh, 05Bh, 081h, 0C4h, 034h, 001h, 000h, 000h, 0C2h, 008h, 000h, 083h, 0ECh, 004h, 08Dh
        db  044h, 024h, 010h, 089h, 044h, 024h, 000h, 08Dh, 044h, 024h, 000h, 06Ah, 004h, 08Dh, 054h, 024h
        db  010h, 08Bh, 04Ch, 024h, 00Ch, 050h, 0E8h, 0A7h, 0FBh, 0FFh, 0FFh, 083h, 0C4h, 004h, 0C3h, 0FFh
        db  074h, 024h, 004h, 0CDh, 020h, 07Ch, 001h, 001h, 000h, 059h, 040h, 050h, 0FFh, 074h, 024h, 00Ch
        db  0FFh, 074h, 024h, 00Ch, 0E8h, 0ACh, 01Ah, 000h, 000h, 083h, 0C4h, 00Ch, 00Fh, 0BFh, 0C0h, 0F7h
loc_8_1B51:
        fadd   st(3)
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        push   esi
        call   sub_8_36BB
        pop    ecx
        pop    ecx
        xchg   esi, eax
        pop    esi
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 08Bh, 044h, 024h, 004h
        db  0C1h, 0E8h, 00Ch, 08Bh, 04Ch, 024h, 008h, 049h, 0C1h, 0E9h, 00Ch, 02Bh, 0C8h, 0FFh, 074h, 024h
        db  00Ch, 041h, 051h, 050h, 0FFh, 015h, 0B4h, 0D4h, 000h, 000h, 083h, 0C4h, 00Ch, 0C3h, 08Bh, 044h
        db  024h, 004h, 0C1h, 0E8h, 00Ch, 08Bh, 04Ch, 024h, 008h, 049h, 0C1h, 0E9h, 00Ch, 02Bh, 0C8h, 0FFh
        db  074h, 024h, 00Ch, 041h, 051h, 050h, 0FFh, 015h, 0B8h, 0D4h, 000h, 000h, 083h, 0C4h, 00Ch, 0C3h
        db  056h, 057h, 02Bh, 0C0h, 08Bh, 035h, 074h, 003h, 000h, 000h, 085h, 0F6h, 074h, 028h, 08Bh, 044h
        db  024h, 00Ch, 085h, 0C0h, 075h, 00Ah, 0CDh, 020h, 0A3h, 000h, 001h, 000h, 075h, 00Ah, 0EBh, 016h
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h, 074h, 00Eh, 08Bh, 0F0h, 08Bh, 07Ch, 024h, 010h, 0B9h, 006h
        db  000h, 000h, 000h, 0FCh, 0F3h, 0A5h, 05Fh, 05Eh, 0C3h, 000h, 000h, 000h, 053h, 056h, 057h, 08Bh
        db  07Ch, 024h, 010h, 083h, 0FFh, 010h, 07Dh, 004h, 00Bh, 0FFh, 075h, 038h, 0F7h, 0C7h, 00Fh, 000h
        db  000h, 000h, 075h, 030h, 08Bh, 05Ch, 024h, 014h, 0F7h, 0C3h, 0FCh, 0FFh, 0FFh, 0FFh, 074h, 002h
        db  0EBh, 022h, 06Ah, 001h, 06Ah, 020h, 0FFh, 015h, 064h, 0D4h, 000h, 000h, 083h, 0C4h, 008h, 08Bh
        db  0F0h, 00Bh, 0F6h, 074h, 00Bh, 083h, 0CBh, 004h, 053h, 057h, 056h, 0E8h, 00Ah, 000h, 000h, 000h
        db  08Bh, 0C6h, 0EBh, 002h, 02Bh, 0C0h, 05Fh, 05Eh, 05Bh, 0C3h, 08Bh, 044h, 024h, 004h, 08Bh, 04Ch
        db  024h, 008h, 066h, 0C7h, 040h, 002h, 04Dh, 058h, 0C7h, 040h, 00Ch, 000h, 000h, 000h, 000h, 0C7h
        db  040h, 010h, 000h, 000h, 000h, 000h, 089h, 048h, 014h, 0C7h, 040h, 018h, 000h, 000h, 000h, 000h
        db  089h, 040h, 008h, 089h, 040h, 004h, 08Ah, 04Ch, 024h, 00Ch, 088h, 008h, 050h, 0E8h, 08Ah, 06Ah
        db  0FAh, 0FFh, 0C2h, 00Ch, 000h, 000h, 000h, 000h
sub_8_1C7C:
        enter  4, 0
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        test   dword ptr [ebp + 0ch], 0ffffffffh
        jne    SHORT loc_8_1CEE
        test   byte ptr [ebp + 10h], 3
        je     SHORT loc_8_1CCB
        mov    edx, dword ptr [ebp + 8]
        mov    ax, 4300h
        push   ebp
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebp
        jb     SHORT loc_8_1CCB
        test   byte ptr [ebp + 10h], 1
        je     SHORT loc_8_1CB5
        and    cl, 0feh
loc_8_1CB5:
        mov    edx, dword ptr [ebp + 8]
        mov    ax, 4301h
        push   ebp
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebp
        jae    SHORT loc_8_1CCB
        inc    dword ptr [ebp - 4]
loc_8_1CCB:
        mov    ax, 3da0h
        test   byte ptr [ebp + 10h], 1
        je     SHORT loc_8_1CD7
        mov    al, 92h
loc_8_1CD7:
        mov    edx, dword ptr [ebp + 8]
        push   ebp
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebp
        movzx  eax, ax
        jae    SHORT loc_8_1CEE
        mov    eax, 0ffffffffh
loc_8_1CEE:
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [ecx], edx
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_8_1CFB:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        xor    ax, ax
        test   dword ptr [ebp + 10h], 0ffffffffh
        jne    SHORT loc_8_1D6B
        test   dword ptr [ebp + 14h], 0ffffffffh
        je     SHORT loc_8_1D27
        mov    ax, 6800h
        mov    ebx, dword ptr [ebp + 0ch]
        push   ebp
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebp
loc_8_1D27:
        mov    ax, 3e00h
        mov    ebx, dword ptr [ebp + 0ch]
        push   ebp
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebp
        test   dword ptr [ebp + 14h], 0ffffffffh
        je     SHORT loc_8_1D6B
        push   eax
        mov    edx, dword ptr [ebp + 8]
        mov    ax, 4300h
        push   ebp
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebp
        jb     SHORT loc_8_1D6A
        or     cx, 7
        mov    edx, dword ptr [ebp + 8]
        mov    ax, 4301h
        push   ebp
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebp
loc_8_1D6A:
        pop    eax
loc_8_1D6B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_8_1D70:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, 3f00h
        mov    ecx, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 0ch]
        push   ecx
        push   ebp
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebp
        pop    ecx
        pop    edi
        pop    esi
        pop    ebx
        jb     SHORT loc_8_1D97
loc_8_1D95:
        leave
        ret
loc_8_1D97:
        mov    eax, 0ffffffffh
        jmp    SHORT loc_8_1D95
sub_8_1D9E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        test   dword ptr [ebp + 14h], 0ffffffffh
        jne    SHORT loc_8_1DD7
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, 3f00h
        mov    ecx, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 0ch]
        push   ecx
        push   ebp
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebp
        pop    ecx
        jb     SHORT loc_8_1DCD
        sub    eax, ecx
        je     SHORT loc_8_1DD2
loc_8_1DCD:
        mov    eax, 0ffffffffh
loc_8_1DD2:
        pop    edi
        pop    esi
loc_8_1DD4:
        pop    ebx
        leave
        ret
loc_8_1DD7:
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_8_8466
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_8_1DCD
        xor    eax, eax
        jmp    SHORT loc_8_1DD2
sub_8_1DF0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        test   dword ptr [ebp + 14h], 0ffffffffh
        jne    SHORT loc_8_1E1F
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, 4000h
        mov    ecx, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 0ch]
        push   ecx
        push   ebp
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebp
        pop    ecx
        jb     SHORT loc_8_1E1F
        sub    eax, ecx
        je     SHORT loc_8_1E24
loc_8_1E1F:
        mov    eax, 0ffffffffh
loc_8_1E24:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_8_1E29:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 0ch]
        test   dword ptr [ebp + 14h], 0ffffffffh
        jne    SHORT loc_8_1E58
        mov    ah, 42h
        movzx  edx, cx
        shr    ecx, 10h
        push   ebp
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebp
loc_8_1E53:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_8_1E58:
        push   eax
        push   ecx
        push   ebx
        call   sub_8_83FC
        add    esp, 0ch
        clc
        jmp    SHORT loc_8_1E53
sub_8_1E66:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        test   dword ptr [ebp + 0ch], 0ffffffffh
        jne    SHORT loc_8_1E9C
        mov    esi, dword ptr [ebp + 8]
        mov    bx, 92h
        mov    dx, 12h
        mov    ax, 6c00h
        mov    cx, 6
        push   ebp
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebp
        jb     SHORT loc_8_1E9C
        movzx  eax, ax
loc_8_1E97:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_8_1E9C:
        mov    eax, 0ffffffffh
        jmp    SHORT loc_8_1E97
sub_8_1EA3:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        test   dword ptr [ebp + 0ch], 0ffffffffh
        jne    SHORT loc_8_1EEB
        mov    edx, dword ptr [ebp + 8]
        mov    ax, 4300h
        push   ebp
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebp
        jb     SHORT loc_8_1EEB
        and    cx, 0fff8h
        mov    edx, dword ptr [ebp + 8]
        mov    ax, 4301h
        push   ebp
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebp
        mov    edx, dword ptr [ebp + 8]
        mov    ax, 4100h
        push   ebp
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebp
loc_8_1EEB:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_8_1EF0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        test   dword ptr [ebp + 10h], 0ffffffffh
        jne    SHORT loc_8_1F71
        mov    edx, dword ptr [ebp + 8]
        mov    ax, 4300h
        push   ebp
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebp
        jb     SHORT loc_8_1F29
        and    cx, 0fff8h
        mov    edx, dword ptr [ebp + 8]
        mov    ax, 4301h
        push   ebp
        push   21h
loc_8_1F22:
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebp
loc_8_1F29:
        mov    edx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ax, 5600h
        push   ebp
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebp
        mov    eax, 0
        jb     SHORT loc_8_1F71
        mov    edx, dword ptr [ebp + 0ch]
        mov    ax, 4300h
        push   ebp
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebp
        jb     SHORT loc_8_1F6C
        or     cx, 7
        mov    edx, dword ptr [ebp + 0ch]
        mov    ax, 4301h
        push   ebp
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebp
loc_8_1F6C:
        mov    eax, 0ffffffffh
loc_8_1F71:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_8_1F76:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        test   dword ptr [ebp + 0ch], 0ffffffffh
        jne    SHORT loc_8_1FA1
        mov    edx, dword ptr [ebp + 8]
        mov    ax, 4300h
        push   ebp
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebp
        jb     SHORT loc_8_1FA1
        mov    ax, cx
        or     ax, 8000h
        jmp    SHORT loc_8_1FA3
loc_8_1FA1:
        xor    eax, eax
loc_8_1FA3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_8_1FA8:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 14h]
        mov    esi, dword ptr [ebp + 8]
        add    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 10h]
        push   ecx
        shr    ecx, 2
        cld
        rep movsd
        pop    ecx
        and    ecx, 3
        rep movsb
        pop    edi
        pop    esi
        leave
        ret
sub_8_1FCA:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        shr    ecx, 2
        xor    eax, eax
loc_8_1FD9:
        add    eax, dword ptr [esi]
        add    esi, 4
        dec    ecx
        jne    SHORT loc_8_1FD9
        mov    edx, eax
        shr    edx, 10h
        pop    esi
        leave
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_8_1FF0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 1ch]
        mov    edi, dword ptr [ebx + 10h]
        test   byte ptr [ebx + 19h], 20h
        je     NEAR PTR loc_8_2115
        cmp    dword ptr [ebx + 24h], -1
        je     NEAR PTR loc_8_2115
        cmp    dword ptr [esp + 20h], 0
        je     NEAR PTR loc_8_2115
        mov    eax, dword ptr [esp + 20h]
        cmp    byte ptr [eax], 0
        je     NEAR PTR loc_8_2115
        db  068h, 05Ch, 0F9h, 000h, 000h
        push   dword ptr [esp + 24h]
        call   sub_8_3580
        add    esp, 8
        mov    ebp, eax
        cmp    ebp, -1
        jne    SHORT loc_8_204C
        mov    eax, 3f2h
        jmp    NEAR PTR loc_8_214A
loc_8_204C:
        cmp    edi, -1
        je     NEAR PTR loc_8_20DA
        mov    esi, dword ptr [esp + 18h]
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [eax + 24h]
        cmp    dword ptr [ecx], 4e4b4752h
        jne    SHORT loc_8_209A
        push   0
        push   dword ptr [esp + 28h]
        db  068h, 05Ch, 0F9h, 000h, 000h
        push   edi
        push   eax
        call   sub_8_2152
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_8_209A
        mov    eax, dword ptr [esp + 24h]
        mov    edi, dword ptr [eax]
        mov    eax, dword ptr [esi]
        sub    edi, dword ptr [eax + 24h]
        cmp    byte ptr [ebp], 0
        je     SHORT loc_8_2091
        inc    ebp
loc_8_2091:
        mov    dword ptr [esp + 20h], ebp
        jmp    NEAR PTR loc_8_2125
loc_8_209A:
        lea    eax, [esp + 10h]
        push   eax
        db  068h, 05Ch, 0F9h, 000h, 000h
        push   ebx
        call   sub_8_8618
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_8_20CA
        cmp    byte ptr [ebp], 0
        je     SHORT loc_8_20B8
        inc    ebp
loc_8_20B8:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esp + 20h], ebp
        mov    edi, dword ptr [eax + 10h]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [esi], eax
        jmp    SHORT loc_8_2119
loc_8_20CA:
        mov    eax, dword ptr [esp + 28h]
        cmp    eax, 2
        je     SHORT loc_8_2129
        mov    eax, 0ffff0000h
        jmp    SHORT loc_8_214A
loc_8_20DA:
        lea    eax, [esp + 10h]
        push   eax
        db  068h, 05Ch, 0F9h, 000h, 000h
        push   ebx
        call   sub_8_8618
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_8_20F8
        mov    eax, 0ffff0000h
        jmp    SHORT loc_8_214A
loc_8_20F8:
        cmp    byte ptr [ebp], 0
        je     SHORT loc_8_20FF
        inc    ebp
loc_8_20FF:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esp + 20h], ebp
        mov    edi, dword ptr [eax + 10h]
        mov    esi, dword ptr [esp + 18h]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [esi], eax
        jmp    SHORT loc_8_2119
loc_8_2115:
        mov    esi, dword ptr [esp + 18h]
loc_8_2119:
        cmp    edi, -1
        jne    SHORT loc_8_2125
        mov    eax, 0ffff0000h
        jmp    SHORT loc_8_214A
loc_8_2125:
        mov    eax, dword ptr [esp + 28h]
loc_8_2129:
        push   0
        cmp    eax, 1
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 28h]
        push   edi
        push   dword ptr [esi]
        jne    SHORT loc_8_2142
        call   sub_8_2152
        jmp    SHORT loc_8_2147
loc_8_2142:
        call   sub_8_2531
loc_8_2147:
        add    esp, 14h
loc_8_214A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_8_2152:
        sub    esp, 0ch
        push   ebx
        push   esi
loc_8_2157:
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 24h]
loc_8_215D:
        mov    eax, dword ptr [esp + 20h]
        mov    ebx, dword ptr [eax + 24h]
        mov    edi, ebx
        mov    eax, dword ptr [esp + 30h]
        inc    dword ptr [esp + 30h]
        cmp    eax, 20h
        ja     NEAR PTR loc_8_226D
        cmp    esi, -1
        lea    ecx, [esi + edi]
        jne    SHORT loc_8_2184
        mov    ecx, dword ptr [edi + 8]
        add    ecx, edi
loc_8_2184:
        cmp    dword ptr [esp + 28h], 0
        je     NEAR PTR loc_8_2298
        mov    eax, dword ptr [esp + 28h]
        cmp    byte ptr [eax], 0
        je     NEAR PTR loc_8_2298
        mov    esi, dword ptr [ecx + 10h]
        cmp    esi, -1
        je     NEAR PTR loc_8_2274
        add    ebx, 4
        cmp    dword ptr [ebx], esi
        jbe    NEAR PTR loc_8_227B
        db  068h, 05Ch, 0F9h, 000h, 000h
        push   dword ptr [esp + 2ch]
        call   sub_8_3580
        mov    dword ptr [esp + 1ch], eax
        add    esp, 8
        cmp    eax, -1
        je     NEAR PTR loc_8_2282
        mov    ebp, eax
        sub    ebp, dword ptr [esp + 28h]
        db  068h, 05Ch, 0F9h, 000h, 000h
        call   sub_8_36EB
        mov    dword ptr [esp + 1ch], eax
        add    esp, 4
        cmp    esi, -1
        je     SHORT loc_8_2244
loc_8_21ED:
        cmp    dword ptr [ebx], esi
        jbe    SHORT loc_8_2244
        add    esi, edi
        mov    eax, dword ptr [esp + 18h]
        cmp    dword ptr [esi + 4], eax
        jne    SHORT loc_8_223C
        lea    eax, [esp + 10h]
        push   eax
        push   esi
        push   dword ptr [esp + 28h]
        call   sub_8_22A8
        add    esp, 0ch
        cmp    eax, 0eh
        je     NEAR PTR loc_8_22A0
        or     eax, eax
        jne    SHORT loc_8_223C
        mov    eax, dword ptr [esp + 10h]
        cmp    word ptr [eax + 0ch], bp
        jne    SHORT loc_8_223C
        push   ebp
        add    eax, 14h
        push   eax
        db  068h, 05Ch, 0F9h, 000h, 000h
        call   sub_8_35F6
        add    esp, 0ch
        or     ax, ax
        je     SHORT loc_8_224B
loc_8_223C:
        mov    esi, dword ptr [esi + 14h]
        cmp    esi, -1
        jne    SHORT loc_8_21ED
loc_8_2244:
        mov    eax, 0ffff0000h
        jmp    SHORT loc_8_22A0
loc_8_224B:
        mov    eax, dword ptr [esp + 14h]
        inc    dword ptr [esp + 14h]
        cmp    byte ptr [eax], 0
        je     SHORT loc_8_2289
        cmp    dword ptr [esi + 10h], -1
        je     SHORT loc_8_2291
        sub    esi, edi
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [esp + 28h], eax
        jmp    NEAR PTR loc_8_215D
loc_8_226D:
        mov    eax, 3f3h
        jmp    SHORT loc_8_22A0
loc_8_2274:
        mov    eax, 0ffff0000h
        jmp    SHORT loc_8_22A0
loc_8_227B:
        mov    eax, 0ffff0000h
        jmp    SHORT loc_8_22A0
loc_8_2282:
        mov    eax, 3f2h
        jmp    SHORT loc_8_22A0
loc_8_2289:
        mov    eax, dword ptr [esp + 2ch]
        mov    dword ptr [eax], esi
        jmp    SHORT loc_8_229E
loc_8_2291:
        mov    eax, 0ffff0000h
        jmp    SHORT loc_8_22A0
loc_8_2298:
        mov    eax, dword ptr [esp + 2ch]
        mov    dword ptr [eax], ecx
loc_8_229E:
        sub    eax, eax
loc_8_22A0:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
sub_8_22A8:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 20h]
        mov    byte ptr [esp + 0fh], 0
        cmp    dword ptr [edi + 18h], -1
        mov    esi, dword ptr [esp + 1ch]
        jne    SHORT loc_8_22FC
        mov    eax, dword ptr [esi + 24h]
        mov    ecx, dword ptr [eax + 8]
        add    ecx, eax
        cmp    ecx, edi
        jne    NEAR PTR loc_8_235A
        lea    eax, [esp + 14h]
        push   eax
        push   21h
        lea    eax, [esp + 17h]
        push   1
        push   eax
        push   esi
        call   sub_8_2EBF
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_8_235A
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [edi + 18h], eax
        push   esi
        call   sub_8_52BC
        add    esp, 4
loc_8_22FC:
        mov    ebx, dword ptr [edi + 18h]
        lea    eax, [esp + 10h]
        push   eax
        push   ebx
        push   esi
loc_8_2306:
        call   sub_8_2E75
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_8_2337
        cmp    eax, 0eh
        jne    SHORT loc_8_2333
        push   -1
        push   esi
        call   sub_8_4589
        lea    eax, [esp + 18h]
        add    esp, 8
        push   eax
        push   dword ptr [edi + 18h]
        push   esi
        call   sub_8_2E75
        add    esp, 0ch
loc_8_2333:
        or     eax, eax
        jne    SHORT loc_8_235F
loc_8_2337:
        mov    ecx, dword ptr [esp + 10h]
        cmp    word ptr [ecx + 14h], bx
        jae    SHORT loc_8_2343
        jmp    SHORT loc_8_235A
loc_8_2343:
        mov    ecx, dword ptr [esp + 10h]
        movzx  edx, bx
        mov    ecx, dword ptr [ecx + 18h]
        mov    edx, dword ptr [ecx + edx*4]
        mov    ecx, dword ptr [esp + 24h]
        or     edx, edx
        mov    dword ptr [ecx], edx
        jne    SHORT loc_8_235F
loc_8_235A:
        mov    eax, 3f3h
loc_8_235F:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
sub_8_2366:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        cmp    word ptr [edi + 0eh], 0
        jne    SHORT loc_8_2379
        jmp    NEAR PTR loc_8_23FD
loc_8_2379:
        mov    ebx, dword ptr [esp + 14h]
        or     ebx, ebx
        jne    SHORT loc_8_2386
        sub    si, si
        jmp    SHORT loc_8_23A9
loc_8_2386:
        push   ebx
        call   sub_8_3651
        add    esp, 4
        mov    si, ax
        cmp    si, 100h
        jb     SHORT loc_8_239B
        jmp    SHORT loc_8_23FD
loc_8_239B:
        db  068h, 05Ch, 0F9h, 000h, 000h
        push   ebx
        call   sub_8_36BB
        add    esp, 8
loc_8_23A9:
        movzx  eax, word ptr [edi + 0ch]
        movzx  ecx, word ptr [edi + 10h]
        add    eax, ecx
        lea    ebx, [edi + eax + 14h]
        mov    di, word ptr [edi + 0eh]
        or     di, di
        je     SHORT loc_8_23FD
loc_8_23C0:
        cmp    word ptr [ebx + 8], si
        jne    SHORT loc_8_23EB
        or     si, si
        je     SHORT loc_8_23E6
        push   esi
        lea    eax, [ebx + 0ch]
        push   eax
        db  068h, 05Ch, 0F9h, 000h, 000h
        call   sub_8_35F6
        add    esp, 0ch
        or     ax, ax
        jne    SHORT loc_8_23EB
        mov    eax, ebx
        jmp    SHORT loc_8_2402
loc_8_23E6:
        cmp    dword ptr [ebx], 1
        je     SHORT loc_8_2406
loc_8_23EB:
        movzx  eax, word ptr [ebx + 0ah]
        movzx  ecx, word ptr [ebx + 8]
        add    eax, ecx
        lea    ebx, [ebx + eax + 0ch]
        dec    di
        jne    SHORT loc_8_23C0
loc_8_23FD:
        mov    eax, 0ffffffffh
loc_8_2402:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_8_2406:
        mov    eax, ebx
        jmp    SHORT loc_8_2402
sub_8_240A:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 1ch]
        push   dword ptr [esp + 20h]
        push   ebx
        call   sub_8_2366
        add    esp, 8
        cmp    eax, -1
        jne    SHORT loc_8_242E
        mov    eax, 0ffffffffh
        jmp    SHORT loc_8_2494
loc_8_242E:
        movzx  ecx, word ptr [eax + 8]
        movzx  edx, word ptr [eax + 0ah]
        lea    ebp, [ecx + edx + 0ch]
        lea    esi, [eax + ebp]
        mov    edi, dword ptr [ebx + 8]
        mov    ecx, edi
        sub    ecx, esi
        lea    edx, [ebx + ecx]
        sub    edi, ebp
        mov    dword ptr [ebx + 8], edi
        mov    cx, word ptr [ebx + 0eh]
        dec    cx
        mov    word ptr [ebx + 0eh], cx
        je     SHORT loc_8_246C
        or     edx, edx
        je     SHORT loc_8_246C
        mov    edi, eax
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
loc_8_246C:
        lea    eax, [esp + 10h]
        push   eax
        mov    esi, dword ptr [esp + 1ch]
        push   dword ptr [ebx + 4]
        push   esi
        call   sub_8_2E75
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_8_2492
        push   dword ptr [esp + 10h]
        push   esi
        call   sub_8_52F0
        add    esp, 8
loc_8_2492:
        sub    eax, eax
loc_8_2494:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_8_249C:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 18h]
        mov    edi, dword ptr [esp + 1ch]
        mov    ebx, dword ptr [esi + 24h]
loc_8_24AE:
        mov    ebp, dword ptr [edi + 10h]
        cmp    ebp, -1
        je     SHORT loc_8_24DD
        mov    eax, dword ptr [esi + 24h]
        cmp    dword ptr [eax + 4], ebp
        jbe    SHORT loc_8_24D6
        call   sub_8_7B50
        lea    eax, [ebx + ebp]
        push   eax
        push   esi
        call   sub_8_249C
        add    esp, 8
        or     eax, eax
        je     SHORT loc_8_24AE
        jmp    SHORT loc_8_2529
loc_8_24D6:
        mov    dword ptr [edi + 10h], 0ffffffffh
loc_8_24DD:
        lea    eax, [esp + 10h]
        push   eax
        push   edi
        push   esi
        call   sub_8_22A8
        add    esp, 0ch
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        jne    SHORT loc_8_2529
        push   dword ptr [esp + 10h]
        push   esi
        call   sub_8_2D72
        add    esp, 8
        mov    dword ptr [edi + 18h], 0ffffffffh
        push   ebx
        push   edi
        call   sub_8_2E0B
        add    esp, 8
        push   esi
        sub    edi, ebx
        push   edi
        call   sub_8_73C3
        add    esp, 8
        push   esi
        call   sub_8_52BC
        add    esp, 4
        sub    eax, eax
loc_8_2529:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_8_2531:
        sub    esp, 2ch
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 44h]
        or     edi, edi
        je     SHORT loc_8_25B2
        mov    esi, 0ffffffffh
        sub    ebx, ebx
loc_8_2546:
        cmp    byte ptr [edi], bl
        je     SHORT loc_8_25B2
        mov    eax, dword ptr [esp + 4ch]
        inc    dword ptr [esp + 4ch]
        cmp    eax, 20h
        ja     SHORT loc_8_25CC
        db  068h, 05Ch, 0F9h, 000h, 000h
        push   edi
        call   sub_8_3580
        mov    dword ptr [esp + 1ch], eax
        add    esp, 8
        cmp    eax, esi
        je     SHORT loc_8_25D3
        lea    eax, [esp + 0ch]
        push   dword ptr [esp + 4ch]
        push   eax
        db  068h, 05Ch, 0F9h, 000h, 000h
        push   dword ptr [esp + 4ch]
        push   dword ptr [esp + 4ch]
        call   sub_8_2152
        add    esp, 14h
        cmp    eax, ebx
        jne    SHORT loc_8_25E0
        mov    eax, dword ptr [esp + 14h]
        inc    dword ptr [esp + 14h]
        cmp    byte ptr [eax], bl
        je     SHORT loc_8_25DA
        mov    eax, dword ptr [esp + 3ch]
        mov    ecx, dword ptr [esp + 0ch]
        sub    ecx, dword ptr [eax + 24h]
        mov    edi, dword ptr [esp + 14h]
        cmp    edi, ebx
        mov    dword ptr [esp + 40h], ecx
        jne    SHORT loc_8_2546
loc_8_25B2:
        mov    eax, dword ptr [esp + 3ch]
        mov    eax, dword ptr [eax + 24h]
        add    eax, dword ptr [esp + 40h]
loc_8_25BD:
        mov    ecx, dword ptr [esp + 48h]
        mov    dword ptr [ecx], eax
loc_8_25C3:
        sub    eax, eax
loc_8_25C5:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 2ch
        ret
loc_8_25CC:
        mov    eax, 3f3h
        jmp    SHORT loc_8_25C5
loc_8_25D3:
        mov    eax, 3f2h
        jmp    SHORT loc_8_25C5
loc_8_25DA:
        mov    eax, dword ptr [esp + 0ch]
        jmp    SHORT loc_8_25BD
loc_8_25E0:
        mov    eax, dword ptr [esp + 3ch]
        test   byte ptr [eax + 11h], 80h
        je     SHORT loc_8_25F1
        mov    eax, 5
        jmp    SHORT loc_8_25C5
loc_8_25F1:
        db  068h, 05Ch, 0F9h, 000h, 000h
        push   edi
        call   sub_8_35D4
        add    esp, 8
        db  068h, 05Ch, 0F9h, 000h, 000h
        call   sub_8_3651
        lea    ecx, [esp + 34h]
        add    esp, 4
        push   ecx
        push   21h
        push   eax
        db  068h, 05Ch, 0F9h, 000h, 000h
        push   dword ptr [esp + 4ch]
        call   sub_8_2EBF
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_8_2630
        mov    eax, 3f3h
        jmp    SHORT loc_8_25C5
loc_8_2630:
        mov    eax, dword ptr [esp + 3ch]
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [esp + 10h], eax
        mov    eax, dword ptr [eax + 0ch]
        cmp    eax, -1
        mov    dword ptr [esp + 18h], eax
        je     SHORT loc_8_265D
        add    eax, dword ptr [esp + 10h]
        mov    dword ptr [esp + 0ch], eax
        cmp    dword ptr [eax + 4], 1ch
        jae    SHORT loc_8_265D
        mov    dword ptr [esp + 18h], 0ffffffffh
loc_8_265D:
        cmp    dword ptr [esp + 18h], -1
        jne    NEAR PTR loc_8_26F6
loc_8_2668:
        push   dword ptr [esp + 3ch]
        call   sub_8_4AD4
        mov    dword ptr [esp + 20h], eax
        add    esp, 4
        or     eax, eax
        je     SHORT loc_8_26DC
        cmp    eax, 0eh
        jne    SHORT loc_8_26D3
        lea    eax, [esp + 34h]
        push   eax
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 44h]
        call   sub_8_2E75
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_8_26D3
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 40h]
        call   sub_8_4589
        add    esp, 8
        push   dword ptr [esp + 3ch]
        call   sub_8_4AD4
        add    esp, 4
        or     eax, eax
        je     SHORT loc_8_26DC
        push   dword ptr [esp + 30h]
        push   dword ptr [esp + 40h]
        call   sub_8_2D26
        mov    eax, dword ptr [esp + 24h]
        add    esp, 8
        jmp    NEAR PTR loc_8_25C5
loc_8_26D3:
        mov    eax, dword ptr [esp + 1ch]
        jmp    NEAR PTR loc_8_25C5
loc_8_26DC:
        mov    eax, dword ptr [esp + 3ch]
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [esp + 10h], eax
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [esp + 18h], eax
        add    eax, dword ptr [esp + 10h]
        mov    dword ptr [esp + 0ch], eax
loc_8_26F6:
        mov    eax, dword ptr [esp + 0ch]
        mov    eax, dword ptr [eax + 8]
        cmp    eax, -1
        je     SHORT loc_8_2707
        jmp    NEAR PTR loc_8_27AA
loc_8_2707:
        mov    eax, dword ptr [esp + 0ch]
        add    eax, 4
        mov    dword ptr [esp + 20h], eax
        cmp    dword ptr [eax], 38h
        jae    SHORT loc_8_273D
        mov    eax, dword ptr [esp + 18h]
        add    ax, 1ch
        movzx  eax, ax
        add    eax, 1ch
        cmp    eax, 10000h
        jbe    NEAR PTR loc_8_2668
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 0ch], 0ffffffffh
        jmp    SHORT loc_8_27B1
loc_8_273D:
        mov    eax, dword ptr [esp + 0ch]
        add    eax, 1ch
        mov    dword ptr [esp + 2ch], eax
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esp + 24h], eax
        mov    eax, dword ptr [esp + 10h]
        add    eax, dword ptr [esp + 24h]
        cmp    eax, dword ptr [esp + 2ch]
        ja     SHORT loc_8_2782
        mov    eax, dword ptr [esp + 24h]
        mov    ecx, dword ptr [esp + 20h]
        sub    eax, dword ptr [esp + 18h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ecx + 0ch], eax
        jmp    NEAR PTR loc_8_2668
loc_8_2782:
        mov    eax, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 2ch]
        mov    eax, dword ptr [eax + 4]
        sub    eax, 1ch
        mov    dword ptr [ecx + 4], eax
        mov    eax, ecx
        mov    dword ptr [eax], 80000000h
        mov    eax, ecx
        mov    dword ptr [eax + 8], 0ffffffffh
        mov    eax, ecx
        sub    eax, dword ptr [esp + 10h]
loc_8_27AA:
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [ecx + 0ch], eax
loc_8_27B1:
        mov    eax, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 40h]
        mov    dword ptr [eax + 0ch], esi
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax + 10h], 0ffffffffh
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax + 14h], 0ffffffffh
        mov    ecx, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esp + 30h]
        mov    dword ptr [ecx + 18h], eax
        mov    eax, dword ptr [esp + 0ch]
        db  068h, 05Ch, 0F9h, 000h, 000h
        mov    dword ptr [eax + 8], 0ffffffffh
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax], 0
        call   sub_8_36EB
        mov    ecx, dword ptr [esp + 10h]
        add    esp, 4
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esp + 3ch]
        mov    ecx, dword ptr [eax + 24h]
        lea    edx, [esi + ecx + 10h]
        mov    eax, dword ptr [edx]
        cmp    eax, -1
        je     SHORT loc_8_2849
        mov    edi, dword ptr [ecx + 4]
        cmp    edi, eax
        jbe    SHORT loc_8_2849
        mov    esi, dword ptr [esp + 10h]
        lea    edx, [eax + esi]
        cmp    dword ptr [edx + 14h], -1
        je     SHORT loc_8_2840
        mov    eax, 0ffffffffh
loc_8_2831:
        mov    ecx, dword ptr [edx + 14h]
        cmp    edi, ecx
        jbe    SHORT loc_8_2840
        lea    edx, [esi + ecx]
        cmp    dword ptr [edx + 14h], eax
        jne    SHORT loc_8_2831
loc_8_2840:
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [edx + 14h], eax
        jmp    SHORT loc_8_284F
loc_8_2849:
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [edx], eax
loc_8_284F:
        push   dword ptr [esp + 3ch]
        call   sub_8_52BC
        mov    eax, dword ptr [esp + 18h]
        inc    dword ptr [esp + 18h]
        add    esp, 4
        cmp    byte ptr [eax], 0
        je     SHORT loc_8_28A5
        push   dword ptr [esp + 4ch]
        push   dword ptr [esp + 4ch]
        push   dword ptr [esp + 1ch]
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 4ch]
        call   sub_8_2531
        add    esp, 14h
        mov    esi, eax
        or     esi, esi
        je     NEAR PTR loc_8_25C3
        push   dword ptr [esp + 0ch]
        push   dword ptr [esp + 40h]
        call   sub_8_249C
        add    esp, 8
        mov    eax, esi
        jmp    NEAR PTR loc_8_25C5
loc_8_28A5:
        mov    eax, dword ptr [esp + 0ch]
        jmp    NEAR PTR loc_8_25BD
sub_8_28AE:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    word ptr [esp + 12h], 0
        mov    esi, dword ptr [esp + 30h]
        or     esi, esi
        je     SHORT loc_8_28D2
        push   esi
        call   sub_8_3651
        mov    word ptr [esp + 16h], ax
        add    esp, 4
loc_8_28D2:
        cmp    word ptr [esp + 12h], 0
        jne    SHORT loc_8_28EB
        cmp    dword ptr [esp + 38h], 1
        je     SHORT loc_8_28EB
        mov    eax, 57h
        jmp    NEAR PTR loc_8_2B6A
loc_8_28EB:
        cmp    word ptr [esp + 12h], 100h
        jb     SHORT loc_8_28FE
        mov    eax, 57h
        jmp    NEAR PTR loc_8_2B6A
loc_8_28FE:
        movzx  eax, word ptr [esp + 12h]
        mov    ecx, dword ptr [esp + 3ch]
        lea    eax, [eax + ecx + 0ch]
        mov    dword ptr [esp + 18h], eax
        cmp    eax, 4000h
        jbe    SHORT loc_8_2920
        mov    eax, 57h
        jmp    NEAR PTR loc_8_2B6A
loc_8_2920:
        lea    eax, [esp + 14h]
        push   eax
        push   dword ptr [esp + 30h]
        push   dword ptr [esp + 30h]
        call   sub_8_22A8
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_8_2B6A
        push   esi
        push   dword ptr [esp + 18h]
        call   sub_8_2366
        add    esp, 8
        mov    edi, eax
        cmp    edi, -1
        je     NEAR PTR loc_8_2A8E
        mov    eax, dword ptr [esp + 38h]
        cmp    dword ptr [edi], eax
        jne    SHORT loc_8_2996
        movzx  eax, word ptr [edi + 0ah]
        cmp    eax, dword ptr [esp + 3ch]
        jne    SHORT loc_8_2996
        mov    ebx, dword ptr [esp + 34h]
        movzx  eax, word ptr [edi + 8]
        lea    eax, [edi + eax + 0ch]
        mov    edx, dword ptr [esp + 3ch]
        or     edx, edx
        jle    SHORT loc_8_298F
loc_8_297B:
        mov    ebp, ebx
        inc    ebx
        mov    ecx, eax
        inc    eax
        mov    cl, byte ptr [ecx]
        cmp    byte ptr [ebp], cl
        jne    SHORT loc_8_298D
        dec    edx
        or     edx, edx
        jg     SHORT loc_8_297B
loc_8_298D:
        or     edx, edx
loc_8_298F:
        jne    SHORT loc_8_2996
        jmp    NEAR PTR loc_8_2B68
loc_8_2996:
        movzx  eax, word ptr [edi + 8]
        movzx  ecx, word ptr [edi + 0ah]
        lea    eax, [eax + ecx + 0ch]
        mov    dword ptr [esp + 1ch], eax
        cmp    eax, dword ptr [esp + 18h]
        je     NEAR PTR loc_8_2AEA
        jae    NEAR PTR loc_8_2A59
        mov    edx, dword ptr [esp + 18h]
        sub    edx, eax
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 14h]
        mov    eax, dword ptr [eax]
        sub    eax, dword ptr [ecx + 8]
        cmp    eax, edx
        jae    SHORT loc_8_2A13
        push   edx
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 34h]
        call   sub_8_3231
        mov    dword ptr [esp + 24h], eax
        add    esp, 10h
        cmp    eax, -1
        jne    SHORT loc_8_29F5
        mov    eax, 0eh
        jmp    NEAR PTR loc_8_2B6A
loc_8_29F5:
        push   esi
        push   dword ptr [esp + 18h]
        call   sub_8_2366
        add    esp, 8
        mov    edi, eax
        cmp    edi, -1
        jne    SHORT loc_8_2A13
        mov    eax, 3f1h
        jmp    NEAR PTR loc_8_2B6A
loc_8_2A13:
        mov    edx, dword ptr [esp + 1ch]
        add    edx, edi
        mov    ecx, dword ptr [esp + 18h]
        add    ecx, edi
        mov    ebp, dword ptr [esp + 14h]
        mov    eax, dword ptr [ebp + 8]
        add    ebp, 8
        mov    ebx, eax
        sub    ebx, edx
        add    ebx, dword ptr [esp + 14h]
        sub    eax, dword ptr [esp + 1ch]
        add    eax, dword ptr [esp + 18h]
        mov    dword ptr [ebp], eax
        mov    eax, ebx
        dec    ebx
        or     eax, eax
        je     NEAR PTR loc_8_2AEA
loc_8_2A47:
        mov    al, byte ptr [edx + ebx]
        mov    byte ptr [ecx + ebx], al
        mov    eax, ebx
        dec    ebx
        or     eax, eax
        jne    SHORT loc_8_2A47
        jmp    NEAR PTR loc_8_2AEA
loc_8_2A59:
        mov    ebx, dword ptr [esp + 1ch]
        add    ebx, edi
        mov    edx, dword ptr [esp + 18h]
        add    edx, edi
        mov    ecx, dword ptr [esp + 14h]
        mov    eax, dword ptr [ecx + 8]
        add    ecx, 8
        mov    ebp, eax
        sub    ebp, ebx
        add    ebp, dword ptr [esp + 14h]
        sub    eax, dword ptr [esp + 1ch]
        add    eax, dword ptr [esp + 18h]
        mov    dword ptr [ecx], eax
        push   ebp
        push   ebx
        push   edx
        call   sub_8_366A
        add    esp, 0ch
        jmp    SHORT loc_8_2AEA
loc_8_2A8E:
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 14h]
        mov    eax, dword ptr [eax]
        sub    eax, dword ptr [ecx + 8]
        cmp    eax, dword ptr [esp + 18h]
        jae    SHORT loc_8_2ACC
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 34h]
        call   sub_8_3231
        mov    dword ptr [esp + 24h], eax
        add    esp, 10h
        cmp    eax, -1
        jne    SHORT loc_8_2ACC
        mov    eax, 0eh
        jmp    NEAR PTR loc_8_2B6A
loc_8_2ACC:
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [eax + 8]
        add    eax, 8
        mov    edi, dword ptr [esp + 14h]
        add    edi, ecx
        add    ecx, dword ptr [esp + 18h]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esp + 14h]
        inc    word ptr [eax + 0eh]
loc_8_2AEA:
        mov    eax, dword ptr [esp + 38h]
        mov    dword ptr [edi], eax
        mov    dword ptr [edi + 4], 0
        mov    ax, word ptr [esp + 12h]
        dec    word ptr [esp + 12h]
        mov    word ptr [edi + 8], ax
        add    edi, 0ch
        mov    cx, word ptr [esp + 3ch]
        mov    word ptr [edi - 2], cx
        or     ax, ax
        je     SHORT loc_8_2B2B
loc_8_2B16:
        mov    al, byte ptr [esi]
        mov    byte ptr [edi], al
        inc    esi
        inc    edi
        mov    ax, word ptr [esp + 12h]
        dec    word ptr [esp + 12h]
        or     ax, ax
        jne    SHORT loc_8_2B16
loc_8_2B2B:
        push   dword ptr [esp + 3ch]
        push   dword ptr [esp + 38h]
        push   edi
        call   sub_8_366A
        lea    eax, [esp + 2ch]
        add    esp, 0ch
        push   eax
        mov    eax, dword ptr [esp + 18h]
        push   dword ptr [eax + 4]
        push   dword ptr [esp + 30h]
        call   sub_8_2E75
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_8_2B68
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 2ch]
        call   sub_8_52F0
        add    esp, 8
loc_8_2B68:
        sub    eax, eax
loc_8_2B6A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
sub_8_2B72:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 1ch]
        mov    edx, 0ffffffffh
        mov    esi, dword ptr [ecx + 10h]
        cmp    esi, edx
        je     SHORT loc_8_2BFC
        mov    edi, dword ptr [ecx + 0ch]
        mov    ebx, dword ptr [edi + 24h]
        mov    eax, dword ptr [ebx + 4]
        cmp    eax, esi
        jbe    SHORT loc_8_2BEF
        mov    edx, dword ptr [esi + ebx + 10h]
        cmp    edx, -1
        je     NEAR PTR loc_8_2C72
        cmp    eax, edx
        ja     SHORT loc_8_2BB4
        mov    dword ptr [esi + ebx + 10h], 0ffffffffh
        jmp    NEAR PTR loc_8_2C72
loc_8_2BB4:
        add    edx, ebx
        mov    esi, dword ptr [esp + 20h]
        mov    ebp, esi
        dec    esi
        or     ebp, ebp
        je     SHORT loc_8_2C04
loc_8_2BC1:
        mov    ebp, dword ptr [edx + 14h]
        cmp    ebp, -1
        je     SHORT loc_8_2BDA
        cmp    eax, ebp
        jbe    SHORT loc_8_2BE1
        lea    edx, [ebp + ebx]
        mov    ebp, esi
        dec    esi
        or     ebp, ebp
        jne    SHORT loc_8_2BC1
        jmp    SHORT loc_8_2C04
loc_8_2BDA:
        mov    edx, 0ffffffffh
        jmp    SHORT loc_8_2C04
loc_8_2BE1:
        mov    dword ptr [edx + 14h], 0ffffffffh
        mov    edx, 0ffffffffh
        jmp    SHORT loc_8_2C04
loc_8_2BEF:
        mov    dword ptr [ecx + 10h], 0ffffffffh
        mov    esi, dword ptr [esp + 20h]
        jmp    SHORT loc_8_2C04
loc_8_2BFC:
        mov    esi, dword ptr [esp + 20h]
        mov    edi, dword ptr [esp + 14h]
loc_8_2C04:
        cmp    edx, -1
        je     SHORT loc_8_2C76
        lea    eax, [esp + 10h]
        push   eax
        push   edx
        push   edi
        call   sub_8_22A8
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_8_2CE1
        mov    ecx, dword ptr [esp + 10h]
        mov    ebx, dword ptr [esp + 28h]
        mov    cx, word ptr [ecx + 0ch]
        cmp    word ptr [ebx], cx
        ja     SHORT loc_8_2C41
        movzx  eax, cx
        inc    eax
        mov    dword ptr [ebx], eax
        mov    eax, 0eah
        jmp    NEAR PTR loc_8_2CE1
loc_8_2C41:
        mov    esi, dword ptr [esp + 24h]
        or     esi, esi
        je     NEAR PTR loc_8_2CE1
        movzx  eax, cx
        push   eax
        mov    eax, dword ptr [esp + 14h]
        add    eax, 14h
        push   eax
        push   esi
        call   sub_8_366A
        mov    eax, dword ptr [esp + 1ch]
        add    esp, 0ch
        movzx  eax, word ptr [eax + 0ch]
        mov    byte ptr [esi + eax], 0
        sub    eax, eax
        jmp    SHORT loc_8_2CE1
loc_8_2C72:
        mov    esi, dword ptr [esp + 20h]
loc_8_2C76:
        test   byte ptr [ecx + 19h], 20h
        je     SHORT loc_8_2CDC
        mov    edi, dword ptr [ecx + 24h]
        cmp    edi, -1
        je     SHORT loc_8_2CDC
        mov    eax, esi
        dec    esi
        or     eax, eax
        je     SHORT loc_8_2C9A
loc_8_2C8B:
        cmp    edi, -1
        je     SHORT loc_8_2CDC
        mov    edi, dword ptr [edi + 8]
        mov    eax, esi
        dec    esi
        or     eax, eax
        jne    SHORT loc_8_2C8B
loc_8_2C9A:
        cmp    edi, -1
        jne    SHORT loc_8_2CA1
        jmp    SHORT loc_8_2CDC
loc_8_2CA1:
        push   dword ptr [edi + 14h]
        call   sub_8_3651
        add    esp, 4
        movzx  ebp, ax
        mov    ebx, dword ptr [esp + 28h]
        inc    ebp
        cmp    dword ptr [ebx], ebp
        jae    SHORT loc_8_2CC1
        mov    dword ptr [ebx], ebp
        mov    eax, 0eah
        jmp    SHORT loc_8_2CE1
loc_8_2CC1:
        mov    esi, dword ptr [esp + 24h]
        or     esi, esi
        je     SHORT loc_8_2CD6
        push   ebp
        push   dword ptr [edi + 14h]
        push   esi
        call   sub_8_366A
        add    esp, 0ch
loc_8_2CD6:
        mov    dword ptr [ebx], ebp
        sub    eax, eax
        jmp    SHORT loc_8_2CE1
loc_8_2CDC:
        mov    eax, 103h
loc_8_2CE1:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_8_2CE9:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 0ch]
        movzx  eax, word ptr [esi + 0eh]
        cmp    eax, edx
        ja     SHORT loc_8_2D01
        mov    eax, 0ffffffffh
        jmp    SHORT loc_8_2D24
loc_8_2D01:
        movzx  eax, word ptr [esi + 0ch]
        movzx  ecx, word ptr [esi + 10h]
        add    eax, ecx
        lea    eax, [esi + eax + 14h]
        or     edx, edx
        je     SHORT loc_8_2D24
loc_8_2D13:
        movzx  ecx, word ptr [eax + 8]
        movzx  esi, word ptr [eax + 0ah]
        add    ecx, esi
        lea    eax, [eax + ecx + 0ch]
        dec    edx
        jne    SHORT loc_8_2D13
loc_8_2D24:
        pop    esi
        ret
sub_8_2D26:
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 14h]
        lea    eax, [esp + 8]
        push   eax
        mov    edi, dword ptr [esp + 14h]
        push   esi
        push   edi
        call   sub_8_2E75
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_8_2D6C
        mov    eax, dword ptr [esp + 8]
        cmp    word ptr [eax + 14h], si
        jb     SHORT loc_8_2D6C
        mov    eax, dword ptr [eax + 18h]
        movzx  ecx, si
        mov    eax, dword ptr [eax + ecx*4]
        or     eax, eax
        je     SHORT loc_8_2D6C
        cmp    dword ptr [eax + 4], esi
        jne    SHORT loc_8_2D6C
        push   eax
        push   edi
        call   sub_8_2D72
        add    esp, 8
loc_8_2D6C:
        pop    edi
        pop    esi
        add    esp, 4
        ret
sub_8_2D72:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 1ch]
        lea    eax, [esp + 10h]
        mov    edi, dword ptr [ebx + 4]
        mov    ebp, dword ptr [ebx]
        push   eax
        mov    esi, dword ptr [esp + 1ch]
        push   edi
        push   esi
        call   sub_8_2E75
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_8_2E03
        mov    dword ptr [ebx + 4], 0ffffffffh
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebx + 8], eax
        mov    eax, dword ptr [esp + 10h]
        sub    ebx, dword ptr [esp + 10h]
        mov    dword ptr [eax + 10h], ebx
        mov    eax, dword ptr [esp + 10h]
        add    dword ptr [eax + 8], ebp
        mov    ecx, dword ptr [esp + 10h]
        mov    ax, word ptr [ecx + 14h]
        add    ecx, 14h
        cmp    di, ax
        jne    SHORT loc_8_2DD6
        or     ax, ax
        je     SHORT loc_8_2DD6
        dec    ax
        mov    word ptr [ecx], ax
loc_8_2DD6:
        mov    eax, dword ptr [esp + 10h]
        add    eax, 16h
        cmp    word ptr [eax], di
        jbe    SHORT loc_8_2DE5
        mov    word ptr [eax], di
loc_8_2DE5:
        mov    eax, dword ptr [esp + 10h]
        movzx  ecx, di
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [eax + ecx*4], 0
        push   dword ptr [esp + 10h]
        push   esi
        call   sub_8_52F0
        add    esp, 8
loc_8_2E03:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_8_2E0B:
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, dword ptr [ecx + 0ch]
        cmp    eax, -1
        je     SHORT loc_8_2E71
        mov    edx, dword ptr [esp + 14h]
        mov    edi, dword ptr [edx + 4]
        cmp    eax, edi
        jae    SHORT loc_8_2E71
        lea    ebx, [edx + eax]
        mov    esi, ecx
        sub    esi, edx
        mov    eax, dword ptr [ebx + 10h]
        cmp    eax, esi
        jne    SHORT loc_8_2E3B
        mov    eax, dword ptr [ecx + 14h]
        mov    dword ptr [ebx + 10h], eax
        jmp    SHORT loc_8_2E5B
loc_8_2E3B:
        cmp    eax, -1
        je     SHORT loc_8_2E5B
loc_8_2E40:
        cmp    eax, edi
        jae    SHORT loc_8_2E5B
        lea    ebx, [edx + eax]
        mov    eax, dword ptr [ebx + 14h]
        cmp    eax, esi
        je     SHORT loc_8_2E55
        cmp    eax, -1
        jne    SHORT loc_8_2E40
        jmp    SHORT loc_8_2E5B
loc_8_2E55:
        mov    eax, dword ptr [ecx + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_8_2E5B:
        mov    eax, dword ptr [edx + 0ch]
        mov    dword ptr [ecx + 8], eax
        mov    dword ptr [ecx + 4], 1ch
        mov    dword ptr [ecx], 80000000h
        mov    dword ptr [edx + 0ch], esi
loc_8_2E71:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_8_2E75:
        push   esi
        push   edi
        sub    eax, eax
        mov    ecx, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 0ch]
        shr    ecx, 10h
        cmp    word ptr [edi + 0eh], cx
        ja     SHORT loc_8_2E91
        mov    eax, 3f7h
        jmp    SHORT loc_8_2EBC
loc_8_2E91:
        movzx  esi, cx
        imul   esi, esi, 0ch
        mov    edx, dword ptr [edi + 28h]
        cmp    dword ptr [esi + edx], -1
        jne    SHORT loc_8_2EAC
        push   -1
        push   ecx
        push   edi
        call   sub_8_4350
        add    esp, 0ch
loc_8_2EAC:
        or     eax, eax
        jne    SHORT loc_8_2EBC
        mov    ecx, dword ptr [edi + 28h]
        mov    edx, dword ptr [esi + ecx]
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [ecx], edx
loc_8_2EBC:
        pop    edi
        pop    esi
        ret
sub_8_2EBF:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    bx, 0ffffh
        movzx  eax, word ptr [esp + 30h]
        mov    word ptr [esp + 10h], bx
        movzx  ecx, word ptr [esp + 34h]
        mov    word ptr [esp + 12h], 0
        mov    edi, eax
        mov    dword ptr [esp + 18h], 0ffffffffh
        mov    dword ptr [esp + 20h], eax
        add    edi, ecx
        cmp    edi, 0efe0h
        jbe    SHORT loc_8_2F02
        mov    eax, 57h
        jmp    NEAR PTR loc_8_3109
loc_8_2F02:
        mov    esi, dword ptr [esp + 28h]
loc_8_2F06:
        push   dword ptr [esp + 12h]
        push   ebx
        push   esi
        call   sub_8_3156
        add    esp, 0ch
        mov    bx, ax
        cmp    bx, 0ffffh
        je     NEAR PTR loc_8_2FFA
        movzx  eax, bx
        shl    eax, 10h
        mov    dword ptr [esp + 1ch], eax
        lea    eax, [esp + 14h]
        push   eax
        push   dword ptr [esp + 20h]
        push   esi
        call   sub_8_2E75
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_8_2F06
loc_8_2F42:
        mov    eax, dword ptr [esp + 14h]
        cmp    word ptr [eax + 16h], 0ffh
        jae    SHORT loc_8_2F06
        cmp    dword ptr [eax + 8], edi
        jb     SHORT loc_8_2F6D
        push   edi
        push   dword ptr [esp + 18h]
        call   sub_8_31BF
        mov    dword ptr [esp + 20h], eax
        add    esp, 8
        cmp    eax, -1
        jne    NEAR PTR loc_8_2FF3
loc_8_2F6D:
        push   edi
        push   ebx
        push   esi
        call   sub_8_49E8
        add    esp, 0ch
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_8_2F94
        lea    eax, [esp + 14h]
        push   eax
        push   dword ptr [esp + 20h]
        push   esi
        call   sub_8_2E75
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_8_2FD9
loc_8_2F94:
        cmp    ebp, 0eh
        jne    NEAR PTR loc_8_2F06
        cmp    word ptr [esp + 10h], bx
        je     NEAR PTR loc_8_2F06
        mov    word ptr [esp + 10h], bx
        push   dword ptr [esp + 14h]
        push   esi
        call   sub_8_4589
        lea    eax, [esp + 1ch]
        add    esp, 8
        push   eax
        push   dword ptr [esp + 20h]
        push   esi
        call   sub_8_2E75
        add    esp, 0ch
        or     eax, eax
        je     NEAR PTR loc_8_2F42
        jmp    NEAR PTR loc_8_2F06
loc_8_2FD9:
        push   edi
        push   dword ptr [esp + 18h]
        call   sub_8_31BF
        mov    dword ptr [esp + 20h], eax
        add    esp, 8
        cmp    eax, -1
        je     NEAR PTR loc_8_2F06
loc_8_2FF3:
        cmp    bx, 0ffffh
        jne    SHORT loc_8_300B
loc_8_2FFA:
        inc    word ptr [esp + 12h]
        cmp    word ptr [esp + 12h], 2
        jb     NEAR PTR loc_8_2F06
loc_8_300B:
        cmp    dword ptr [esp + 18h], -1
        jne    SHORT loc_8_3088
        push   edi
        push   esi
        call   sub_8_471D
        add    esp, 8
        or     eax, eax
        je     SHORT loc_8_3046
        cmp    eax, 0eh
        jne    NEAR PTR loc_8_3109
        push   -1
        push   esi
        call   sub_8_4589
        add    esp, 8
        push   edi
        push   esi
        call   sub_8_471D
        add    esp, 8
        or     eax, eax
        jne    NEAR PTR loc_8_3109
loc_8_3046:
        mov    bx, word ptr [esi + 0eh]
        dec    bx
        lea    eax, [esp + 14h]
        push   eax
        movzx  eax, bx
        shl    eax, 10h
        push   eax
        push   esi
        call   sub_8_2E75
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_8_3109
        push   edi
        push   dword ptr [esp + 18h]
        call   sub_8_31BF
        mov    dword ptr [esp + 20h], eax
        add    esp, 8
        cmp    eax, -1
        mov    eax, 0eh
        je     NEAR PTR loc_8_3109
loc_8_3088:
        push   dword ptr [esp + 14h]
        call   sub_8_3111
        add    esp, 4
        movzx  edi, ax
        movzx  eax, bx
        shl    eax, 10h
        mov    ecx, dword ptr [esp + 18h]
        add    eax, edi
        mov    dword ptr [ecx + 4], eax
        mov    ax, word ptr [esp + 30h]
        push   dword ptr [esp + 20h]
        mov    word ptr [ecx + 0ch], ax
        push   dword ptr [esp + 30h]
        mov    word ptr [ecx + 0eh], 0
        mov    word ptr [ecx + 10h], 0
        mov    word ptr [ecx + 12h], 0
        mov    eax, dword ptr [esp + 20h]
        add    eax, 14h
        push   eax
        call   sub_8_366A
        mov    eax, dword ptr [esp + 2ch]
        add    esp, 0ch
        add    eax, 14h
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [esp + 14h]
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [eax + edi*4], ecx
        mov    eax, dword ptr [esp + 38h]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [eax], ecx
        push   dword ptr [esp + 14h]
        push   esi
        call   sub_8_52F0
        add    esp, 8
        sub    eax, eax
loc_8_3109:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
sub_8_3111:
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 10h]
        mov    ax, word ptr [edx + 16h]
        mov    cx, word ptr [edx + 14h]
        cmp    ax, cx
        jbe    SHORT loc_8_312E
        mov    word ptr [edx + 14h], ax
        lea    esi, [eax + 1]
        jmp    SHORT loc_8_314E
loc_8_312E:
        lea    esi, [eax + 1]
        mov    edi, dword ptr [edx + 18h]
        movzx  ebx, si
        lea    edi, [edi + ebx*4]
        cmp    cx, si
        jb     SHORT loc_8_314E
loc_8_313F:
        cmp    dword ptr [edi], 0
        je     SHORT loc_8_314E
        add    edi, 4
        inc    si
        cmp    cx, si
        jae    SHORT loc_8_313F
loc_8_314E:
        mov    word ptr [edx + 16h], si
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_8_3156:
        push   ebx
        push   esi
        push   edi
        mov    di, 0ffffh
        mov    si, word ptr [esp + 14h]
        mov    eax, dword ptr [esp + 10h]
        inc    si
        mov    bx, word ptr [eax + 0eh]
        cmp    bx, si
        ja     SHORT loc_8_3173
        jmp    SHORT loc_8_31B8
loc_8_3173:
        movzx  edx, si
        imul   edx, edx, 0ch
        dec    bx
        add    edx, dword ptr [eax + 28h]
        cmp    bx, si
        jb     SHORT loc_8_31B8
        mov    cx, word ptr [esp + 18h]
loc_8_3188:
        cmp    dword ptr [edx], -1
        jne    SHORT loc_8_31A4
        or     cx, cx
        je     SHORT loc_8_31A9
        push   -1
        push   esi
        push   eax
        call   sub_8_4350
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_8_31B8
        jmp    SHORT loc_8_31B5
loc_8_31A4:
        or     cx, cx
        je     SHORT loc_8_31B5
loc_8_31A9:
        inc    si
        add    edx, 0ch
        cmp    bx, si
        jae    SHORT loc_8_3188
        jmp    SHORT loc_8_31B8
loc_8_31B5:
        mov    di, si
loc_8_31B8:
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_8_31BF:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edx, dword ptr [esp + 14h]
        mov    esi, 0ffffffffh
        mov    eax, dword ptr [edx + 10h]
        cmp    eax, esi
        je     SHORT loc_8_31EA
        movzx  edi, word ptr [esp + 18h]
loc_8_31D8:
        add    eax, edx
        mov    ecx, dword ptr [eax]
        cmp    ecx, edi
        jae    SHORT loc_8_31F4
        mov    esi, eax
        mov    eax, dword ptr [eax + 8]
        cmp    eax, -1
        jne    SHORT loc_8_31D8
loc_8_31EA:
        mov    eax, 0ffffffffh
loc_8_31EF:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_8_31F4:
        mov    ebx, dword ptr [eax + 8]
        mov    ebp, ecx
        sub    ebp, edi
        cmp    ebp, 2ah
        jbe    SHORT loc_8_321D
        lea    ecx, [eax + edi]
        mov    ebp, dword ptr [eax]
        sub    ebp, edi
        mov    dword ptr [ecx], ebp
        mov    dword ptr [ecx + 4], 0ffffffffh
        mov    dword ptr [ecx + 8], ebx
        mov    dword ptr [eax], edi
        sub    ecx, edx
        mov    ebx, ecx
        mov    ecx, edi
        jmp    SHORT loc_8_321F
loc_8_321D:
        mov    dword ptr [eax], ecx
loc_8_321F:
        cmp    esi, -1
        jne    SHORT loc_8_3229
        mov    dword ptr [edx + 10h], ebx
        jmp    SHORT loc_8_322C
loc_8_3229:
        mov    dword ptr [esi + 8], ebx
loc_8_322C:
        sub    dword ptr [edx + 8], ecx
        jmp    SHORT loc_8_31EF
sub_8_3231:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 28h]
        mov    ebx, dword ptr [esp + 2ch]
        add    ebx, dword ptr [eax + 8]
        cmp    ebx, 0efe0h
        jbe    SHORT loc_8_3254
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_8_3501
loc_8_3254:
        mov    ecx, dword ptr [esp + 28h]
        mov    eax, dword ptr [ecx + 4]
        add    ecx, 4
        mov    dword ptr [esp + 14h], eax
        mov    edi, eax
        shr    edi, 10h
        lea    eax, [esp + 10h]
        push   eax
        mov    esi, dword ptr [esp + 24h]
        push   dword ptr [ecx]
        push   esi
        call   sub_8_2E75
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_8_3289
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_8_3501
loc_8_3289:
        mov    eax, dword ptr [esp + 28h]
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax]
        add    eax, dword ptr [esp + 28h]
        mov    dword ptr [esp + 0ch], eax
        sub    eax, dword ptr [esp + 10h]
        cmp    eax, dword ptr [ecx + 4]
        jae    SHORT loc_8_32E5
        mov    eax, dword ptr [esp + 0ch]
        cmp    dword ptr [eax + 4], -1
        jne    SHORT loc_8_32E5
        mov    ecx, dword ptr [esp + 28h]
        mov    eax, dword ptr [eax]
        add    eax, dword ptr [ecx]
        cmp    eax, ebx
        jb     SHORT loc_8_32E5
        push   ebx
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 30h]
        push   dword ptr [esp + 1ch]
        call   sub_8_3508
        add    esp, 10h
        push   dword ptr [esp + 10h]
        push   esi
        call   sub_8_52F0
        mov    eax, dword ptr [esp + 30h]
        add    esp, 8
        jmp    NEAR PTR loc_8_3501
loc_8_32E5:
        push   ebx
        push   dword ptr [esp + 14h]
        call   sub_8_31BF
        mov    dword ptr [esp + 14h], eax
        add    esp, 8
        cmp    eax, -1
        jne    NEAR PTR loc_8_3482
        mov    eax, dword ptr [esp + 10h]
        push   dword ptr [eax + 4]
        push   dword ptr [esp + 14h]
        call   sub_8_4B7D
        add    esp, 8
        or     eax, eax
        je     SHORT loc_8_3330
        push   ebx
        push   dword ptr [esp + 14h]
        call   sub_8_31BF
        mov    dword ptr [esp + 14h], eax
        add    esp, 8
        cmp    eax, -1
        jne    NEAR PTR loc_8_3467
loc_8_3330:
        push   ebx
        push   edi
        push   esi
        call   sub_8_49E8
        add    esp, 0ch
        or     eax, eax
        je     NEAR PTR loc_8_3427
        lea    eax, [esp + 28h]
        mov    edi, dword ptr [esp + 24h]
        push   eax
        push   edi
        push   esi
        call   sub_8_22A8
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_8_3364
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_8_3501
loc_8_3364:
        mov    eax, dword ptr [esp + 28h]
        lea    ecx, [esp + 14h]
        mov    ax, word ptr [eax + 0ch]
        push   ecx
        sub    bx, ax
        push   ebx
        push   eax
        mov    eax, dword ptr [esp + 34h]
        add    eax, 14h
        push   eax
        push   esi
        call   sub_8_2EBF
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_8_3395
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_8_3501
loc_8_3395:
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [edi + 18h], eax
        push   esi
        call   sub_8_52BC
        lea    eax, [esp + 10h]
        add    esp, 4
        push   eax
        push   edi
        push   esi
        call   sub_8_22A8
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_8_33CC
        mov    eax, dword ptr [esp + 28h]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [edi + 18h], eax
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_8_3501
loc_8_33CC:
        lea    eax, [esp + 18h]
        push   eax
        push   dword ptr [esp + 18h]
        push   esi
        call   sub_8_2E75
        add    esp, 0ch
        or     eax, eax
        mov    eax, dword ptr [esp + 28h]
        je     SHORT loc_8_33F6
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [edi + 18h], eax
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_8_3501
loc_8_33F6:
        mov    ecx, dword ptr [eax + 8]
        add    eax, 8
        sub    ecx, 8
        push   ecx
        push   eax
        mov    eax, dword ptr [esp + 14h]
        add    eax, 8
        push   eax
        call   sub_8_366A
        add    esp, 0ch
        push   dword ptr [esp + 28h]
        push   esi
        call   sub_8_2D72
        add    esp, 8
        push   dword ptr [esp + 18h]
        jmp    NEAR PTR loc_8_34F4
loc_8_3427:
        lea    eax, [esp + 10h]
        push   eax
        push   dword ptr [esp + 18h]
        push   esi
        call   sub_8_2E75
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_8_3447
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_8_3501
loc_8_3447:
        push   ebx
        push   dword ptr [esp + 14h]
        call   sub_8_31BF
        mov    dword ptr [esp + 14h], eax
        add    esp, 8
        cmp    eax, -1
        jne    SHORT loc_8_3467
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_8_3501
loc_8_3467:
        lea    eax, [esp + 28h]
        push   eax
        push   dword ptr [esp + 28h]
        push   esi
        call   sub_8_22A8
        add    esp, 0ch
        or     eax, eax
        mov    eax, 0ffffffffh
        jne    SHORT loc_8_3501
loc_8_3482:
        mov    eax, dword ptr [esp + 28h]
        mov    eax, dword ptr [eax + 8]
        sub    eax, 4
        push   eax
        mov    eax, dword ptr [esp + 2ch]
        add    eax, 4
        push   eax
        mov    eax, dword ptr [esp + 14h]
        add    eax, 4
        push   eax
        call   sub_8_366A
        mov    eax, dword ptr [esp + 34h]
        add    esp, 0ch
        mov    dword ptr [eax + 4], 0ffffffffh
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 28h]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [esp + 28h]
        mov    ecx, dword ptr [esp + 10h]
        sub    eax, dword ptr [esp + 10h]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [esp + 28h]
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax]
        add    dword ptr [ecx + 8], eax
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 0ch]
        mov    edx, dword ptr [ecx + 18h]
        movzx  eax, word ptr [eax + 4]
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [edx + eax*4], ecx
        push   dword ptr [esp + 10h]
loc_8_34F4:
        push   esi
        call   sub_8_52F0
        mov    eax, dword ptr [esp + 14h]
        add    esp, 8
loc_8_3501:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
sub_8_3508:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 18h]
        mov    edi, dword ptr [esp + 20h]
        mov    ecx, dword ptr [esi]
        mov    ebx, dword ptr [esp + 1ch]
        sub    edi, ecx
        mov    edx, dword ptr [ebx]
        lea    eax, [edi + 2ah]
        cmp    eax, edx
        jae    SHORT loc_8_3542
        lea    ebp, [ebx + edi]
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [ebp + 8], eax
        mov    dword ptr [ebp + 4], 0ffffffffh
        sub    edx, edi
        mov    dword ptr [ebp], edx
        mov    edx, dword ptr [esp + 14h]
        sub    ebp, edx
        jmp    SHORT loc_8_354B
loc_8_3542:
        mov    ebp, dword ptr [ebx + 8]
        mov    edi, edx
        mov    edx, dword ptr [esp + 14h]
loc_8_354B:
        mov    eax, dword ptr [edx + 10h]
        add    eax, edx
        cmp    ebx, eax
        jne    SHORT loc_8_3559
        mov    dword ptr [edx + 10h], ebp
        jmp    SHORT loc_8_3572
loc_8_3559:
        sub    ebx, edx
        cmp    dword ptr [eax + 8], ebx
        je     SHORT loc_8_356F
loc_8_3560:
        mov    eax, dword ptr [eax + 8]
        add    eax, edx
        cmp    dword ptr [eax + 8], ebx
        jne    SHORT loc_8_3560
        cmp    dword ptr [eax + 8], ebx
        jne    SHORT loc_8_3572
loc_8_356F:
        mov    dword ptr [eax + 8], ebp
loc_8_3572:
        lea    eax, [edi + ecx]
        mov    dword ptr [esi], eax
        sub    dword ptr [edx + 8], edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h
sub_8_3580:
        push   ebx
        push   esi
        sub    edx, edx
        mov    eax, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 10h]
loc_8_358C:
        mov    bl, byte ptr [eax]
        or     bl, bl
        je     SHORT loc_8_35CE
        cmp    bl, 5ch
        je     SHORT loc_8_35BA
        cmp    bl, 20h
        jb     SHORT loc_8_35C7
        movzx  esi, bl
        inc    ecx
        db  08Ah, 09Eh, 030h, 007h, 000h, 000h
        mov    byte ptr [ecx - 1], bl
        inc    eax
        inc    edx
        cmp    edx, 100h
        jl     SHORT loc_8_358C
        mov    eax, 0ffffffffh
        jmp    SHORT loc_8_35D1
loc_8_35BA:
        cmp    byte ptr [eax + 1], 5ch
        jne    SHORT loc_8_35CE
        mov    eax, 0ffffffffh
        jmp    SHORT loc_8_35D1
loc_8_35C7:
        mov    eax, 0ffffffffh
        jmp    SHORT loc_8_35D1
loc_8_35CE:
        mov    byte ptr [ecx], 0
loc_8_35D1:
        pop    esi
        pop    ebx
        ret
sub_8_35D4:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        mov    dl, byte ptr [eax]
        inc    eax
        or     dl, dl
        je     SHORT loc_8_35F2
loc_8_35E3:
        cmp    dl, 5ch
        je     SHORT loc_8_35F2
        mov    byte ptr [ecx], dl
        inc    ecx
        mov    dl, byte ptr [eax]
        inc    eax
        or     dl, dl
        jne    SHORT loc_8_35E3
loc_8_35F2:
        mov    byte ptr [ecx], 0
        ret
sub_8_35F6:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 0ch]
        movzx  eax, byte ptr [esi]
        movzx  ecx, byte ptr [edx]
        db  08Ah, 089h, 030h, 007h, 000h, 000h
        db  038h, 088h, 030h, 007h, 000h, 000h
        jne    SHORT loc_8_3635
        mov    ax, word ptr [esp + 14h]
loc_8_3619:
        dec    ax
        je     SHORT loc_8_3635
        movzx  edi, byte ptr [esi + 1]
        inc    esi
        movzx  ecx, byte ptr [edx + 1]
        inc    edx
        db  08Ah, 089h, 030h, 007h, 000h, 000h
        db  038h, 08Fh, 030h, 007h, 000h, 000h
        je     SHORT loc_8_3619
loc_8_3635:
        movzx  eax, byte ptr [esi]
        movzx  ecx, byte ptr [edx]
        db  066h, 00Fh, 0B6h, 080h, 030h, 007h, 000h, 000h
        db  066h, 00Fh, 0B6h, 089h, 030h, 007h, 000h, 000h
        sub    ax, cx
        pop    edi
        pop    esi
        ret
sub_8_3651:
        sub    ax, ax
        mov    edx, dword ptr [esp + 4]
        mov    ecx, edx
        inc    edx
        cmp    byte ptr [ecx], al
        je     SHORT loc_8_3669
loc_8_365F:
        inc    ax
        mov    ecx, edx
        inc    edx
        cmp    byte ptr [ecx], 0
        jne    SHORT loc_8_365F
loc_8_3669:
        ret
sub_8_366A:
        push   esi
        mov    esi, dword ptr [esp + 10h]
        mov    eax, esi
        dec    esi
        or     eax, eax
loc_8_3674:
        je     SHORT loc_8_368B
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [esp + 0ch]
loc_8_367E:
        mov    dl, byte ptr [ecx]
        mov    byte ptr [eax], dl
loc_8_3682:
        inc    ecx
        inc    eax
        mov    edx, esi
        dec    esi
        or     edx, edx
        jne    SHORT loc_8_367E
loc_8_368B:
        pop    esi
        ret
        db  056h, 08Bh, 054h, 024h, 008h
loc_8_3692:
        cmp    byte ptr [edx], 0
        db  074h, 006h
loc_8_3697:
        inc    edx
        db  080h
loc_8_3699:
        cmp    al, byte ptr [eax]
        jne    SHORT loc_8_3697
        db  08Bh, 074h, 024h
loc_8_36A0:
        adc    byte ptr [ebx - 3ff4b13ah], cl
        je     SHORT loc_8_36B9
        mov    eax, dword ptr [esp + 0ch]
loc_8_36AC:
        mov    cl, byte ptr [eax]
loc_8_36AE:
        mov    byte ptr [edx], cl
        inc    eax
        inc    edx
        mov    ecx, esi
        dec    esi
loc_8_36B5:
        or     ecx, ecx
        jne    SHORT loc_8_36AC
loc_8_36B9:
        pop    esi
        ret
sub_8_36BB:
        push   esi
loc_8_36BC:
        mov    esi, dword ptr [esp + 0ch]
        mov    eax, esi
        mov    edx, dword ptr [esp + 8]
        inc    esi
        movzx  ecx, byte ptr [edx]
loc_8_36CA:
        db  08Ah, 089h, 030h, 007h, 000h, 000h
        mov    byte ptr [eax], cl
        or     cl, cl
        je     SHORT loc_8_36E9
loc_8_36D6:
        inc    edx
        mov    eax, esi
        movzx  ecx, byte ptr [edx]
        inc    esi
        db  08Ah, 089h, 030h, 007h, 000h, 000h
        mov    byte ptr [eax], cl
        or     cl, cl
        jne    SHORT loc_8_36D6
loc_8_36E9:
        pop    esi
        ret
sub_8_36EB:
        mov    edx, dword ptr [esp + 4]
        sub    eax, eax
        mov    cl, byte ptr [edx]
        inc    edx
loc_8_36F4:
        cmp    cl, al
        je     SHORT loc_8_371B
loc_8_36F8:
        cmp    cl, 80h
        jae    SHORT loc_8_3714
        cmp    cl, 61h
        jb     SHORT loc_8_370F
        cmp    cl, 7ah
        ja     SHORT loc_8_370F
        movzx  ecx, cl
        sub    ecx, 20h
        jmp    SHORT loc_8_3712
loc_8_370F:
        movzx  ecx, cl
loc_8_3712:
        add    eax, ecx
loc_8_3714:
        mov    cl, byte ptr [edx]
loc_8_3716:
        inc    edx
        or     cl, cl
        jne    SHORT loc_8_36F8
loc_8_371B:
        ret
        db  000h, 000h, 000h, 000h
sub_8_3720:
        sub    esp, 54h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 1ch], 0
        mov    esi, dword ptr [esp + 6ch]
        mov    dword ptr [esp + 14h], 0ffffffffh
        mov    dword ptr [esi + 20h], 0ffffffffh
        mov    dword ptr [esi + 24h], 0ffffffffh
        mov    dword ptr [esi + 1ch], 0
        mov    dword ptr [esi + 28h], 0ffffffffh
        mov    ebx, dword ptr [esi + 10h]
        and    ebx, 100h
        jne    SHORT loc_8_37A3
        push   dword ptr [esp + 68h]
        call   sub_8_3651
        inc    ax
        add    esp, 4
        movzx  edi, ax
        push   201h
        push   edi
        db  0FFh, 015h, 064h, 0D4h, 000h, 000h
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_8_3792
        mov    eax, 0eh
        jmp    NEAR PTR loc_8_3B23
loc_8_3792:
        push   edi
        push   dword ptr [esp + 6ch]
        push   ebp
        call   sub_8_366A
        add    esp, 0ch
        mov    dword ptr [esi + 14h], ebp
loc_8_37A3:
        push   201h
        push   20h
        db  0FFh, 015h, 064h, 0D4h, 000h, 000h
        add    esp, 8
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_8_37C3
        mov    eax, 0eh
        jmp    NEAR PTR loc_8_3B23
loc_8_37C3:
        mov    dword ptr [esi + 20h], edi
        or     ebx, ebx
        je     SHORT loc_8_3812
        mov    dword ptr [esi + 1ch], 1
        push   1
        push   edi
        call   sub_8_8510
        add    esp, 8
loc_8_37DC:
        db  066h, 0A1h, 0D0h, 003h, 000h, 000h
        mov    word ptr [esp + 12h], ax
        db  066h, 0C7h, 005h, 0D0h, 003h, 000h, 000h, 0FFh, 000h
        push   edi
        call   sub_8_4DB7
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_8_3892
        or     dword ptr [esi + 10h], 8
        mov    dword ptr [esp + 1ch], 3f1h
        jmp    NEAR PTR loc_8_3AE8
loc_8_3812:
        lea    eax, [esp + 20h]
        push   eax
        push   2
        push   dword ptr [esi + 2ch]
        push   dword ptr [esp + 74h]
        call   sub_8_1C7C
        mov    dword ptr [esp + 24h], eax
        add    esp, 10h
        cmp    eax, -1
        jne    SHORT loc_8_383E
        mov    dword ptr [esp + 1ch], 3f8h
        jmp    NEAR PTR loc_8_3AE8
loc_8_383E:
        cmp    dword ptr [esp + 20h], 0
        je     SHORT loc_8_384C
        or     dword ptr [esi + 10h], 8000h
loc_8_384C:
        mov    ebx, dword ptr [esi + 2ch]
        or     ebx, ebx
        je     SHORT loc_8_3868
        mov    eax, dword ptr [esi + 30h]
        mov    dword ptr [esp + 14h], eax
        push   0
        push   dword ptr [esp + 18h]
        call   sub_8_83CE
        add    esp, 8
loc_8_3868:
        push   ebx
        push   20h
        push   edi
        push   dword ptr [esp + 20h]
        call   sub_8_1D9E
        add    esp, 10h
        cmp    eax, -1
        jne    NEAR PTR loc_8_37DC
        or     dword ptr [esi + 10h], 20h
        mov    dword ptr [esp + 1ch], 3f8h
        jmp    NEAR PTR loc_8_3AE8
loc_8_3892:
        mov    ebx, dword ptr [esi + 10h]
        and    ebx, 0ffffff86h
        mov    dword ptr [esi + 10h], ebx
        and    dword ptr [edi + 12h], 0fffffffah
        push   edi
        call   sub_8_4DB7
        add    esp, 4
        mov    ax, word ptr [edi + 10h]
        mov    word ptr [esi + 0eh], ax
        test   bh, 1
        je     SHORT loc_8_38EB
        lea    eax, [esp + 24h]
        push   eax
        call   sub_8_855B
        add    esp, 4
loc_8_38C2:
        mov    edi, dword ptr [esp + 28h]
        push   201h
        push   dword ptr [esp + 2ch]
        db  0FFh, 015h, 064h, 0D4h, 000h, 000h
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_8_3917
        mov    dword ptr [esp + 1ch], 0eh
        jmp    NEAR PTR loc_8_3AE8
loc_8_38EB:
        push   dword ptr [esi + 2ch]
        lea    eax, [esp + 28h]
        push   20h
        push   eax
        push   dword ptr [esp + 20h]
        call   sub_8_1D9E
        add    esp, 10h
        cmp    eax, -1
        jne    SHORT loc_8_38C2
        mov    dword ptr [esp + 1ch], 3f1h
        or     dword ptr [esi + 10h], 20h
        jmp    NEAR PTR loc_8_3AE8
loc_8_3917:
        lea    eax, [esp + 24h]
        push   20h
        push   eax
        push   ebx
        call   sub_8_366A
        add    esp, 0ch
        mov    dword ptr [esi + 24h], ebx
        lea    eax, [ebx + 20h]
        mov    ebp, dword ptr [esi + 10h]
        test   ebp, 100h
        je     SHORT loc_8_395F
        push   eax
        call   sub_8_8582
        add    esp, 4
loc_8_3941:
        push   ebx
        call   sub_8_4E23
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_8_398B
        mov    dword ptr [esp + 1ch], 3f1h
        or     dword ptr [esi + 10h], 8
        jmp    NEAR PTR loc_8_3AE8
loc_8_395F:
        push   dword ptr [esi + 2ch]
        db  083h, 0EFh
loc_8_3964:
        and    byte ptr [edi + 50h], dl
        push   dword ptr [esp + 20h]
        call   sub_8_1D9E
        add    esp, 10h
        cmp    eax, -1
        jne    SHORT loc_8_3941
        mov    dword ptr [esp + 1ch], 3f1h
        or     ebp, 20h
        mov    dword ptr [esi + 10h], ebp
        jmp    NEAR PTR loc_8_3AE8
loc_8_398B:
        mov    eax, dword ptr [esi + 24h]
        push   eax
        and    dword ptr [eax + 10h], 0fffffff8h
        call   sub_8_4E23
        add    esp, 4
        mov    di, word ptr [esi + 0eh]
        or     di, di
        je     SHORT loc_8_39E8
        push   201h
        movzx  eax, di
        imul   eax, eax, 18h
        push   eax
        db  0FFh, 015h, 064h, 0D4h, 000h, 000h
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_8_39CC
        mov    dword ptr [esp + 1ch], 0eh
        jmp    NEAR PTR loc_8_3AE8
loc_8_39CC:
        mov    ecx, ebx
        mov    ax, di
        or     ax, ax
        je     SHORT loc_8_39ED
loc_8_39D6:
        mov    dword ptr [ecx], 0ffffffffh
        dec    ax
        add    ecx, 0ch
        or     ax, ax
        jne    SHORT loc_8_39D6
        jmp    SHORT loc_8_39ED
loc_8_39E8:
        mov    ebx, 0ffffffffh
loc_8_39ED:
        mov    eax, dword ptr [esi + 20h]
        mov    dword ptr [esi + 28h], ebx
        mov    ebp, dword ptr [eax + 8]
        or     di, di
        mov    word ptr [esp + 10h], di
        je     NEAR PTR loc_8_3AD6
loc_8_3A04:
        mov    edi, dword ptr [esi + 10h]
        and    edi, 100h
        jne    SHORT loc_8_3A21
        push   dword ptr [esi + 2ch]
        push   0
        push   ebp
        push   dword ptr [esp + 20h]
        call   sub_8_1E29
        add    esp, 10h
loc_8_3A21:
        mov    dword ptr [ebx + 4], ebp
        or     edi, edi
        je     SHORT loc_8_3A8A
        push   201h
        push   1000h
        db  0FFh, 015h, 064h, 0D4h, 000h, 000h
        add    esp, 8
        mov    edi, eax
        or     edi, edi
loc_8_3A3F:
        je     NEAR PTR loc_8_3B2B
        push   edi
        call   sub_8_85CE
        add    esp, 4
        mov    dword ptr [ebx], edi
        push   201h
        push   3fch
        db  0FFh, 015h, 064h, 0D4h, 000h, 000h
        mov    dword ptr [esp + 20h], eax
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_8_3B35
        push   eax
        push   edi
        call   sub_8_48C7
        add    esp, 8
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [ecx + 18h], eax
        lea    edx, [ebx + 8]
        mov    eax, dword ptr [ecx + 4]
        jmp    SHORT loc_8_3ABE
loc_8_3A8A:
        push   dword ptr [esi + 2ch]
        lea    eax, [esp + 28h]
        push   20h
        push   eax
        push   dword ptr [esp + 20h]
        call   sub_8_1D9E
        add    esp, 10h
        cmp    eax, -1
        je     NEAR PTR loc_8_3B3F
        cmp    dword ptr [esp + 24h], 42444752h
        jne    NEAR PTR loc_8_3B4D
        lea    edx, [ebx + 8]
        mov    eax, dword ptr [esp + 28h]
loc_8_3ABE:
        mov    dword ptr [edx], eax
        add    ebp, dword ptr [edx]
        add    ebx, 0ch
        dec    word ptr [esp + 10h]
        cmp    word ptr [esp + 10h], 0
        jne    NEAR PTR loc_8_3A04
loc_8_3AD6:
        mov    eax, dword ptr [esi + 24h]
        test   byte ptr [eax + 10h], 8
        jne    SHORT loc_8_3AE8
        push   esi
        call   sub_8_3B5B
        add    esp, 4
loc_8_3AE8:
        mov    ax, word ptr [esp + 12h]
        cmp    dword ptr [esp + 1ch], 0
        db  066h, 0A3h, 0D0h, 003h, 000h, 000h
        je     SHORT loc_8_3B03
        push   esi
        call   sub_8_80C4
        add    esp, 4
loc_8_3B03:
        cmp    dword ptr [esp + 14h], -1
        je     SHORT loc_8_3B1F
        push   0
        push   dword ptr [esi + 2ch]
        push   dword ptr [esp + 1ch]
        push   dword ptr [esp + 74h]
        call   sub_8_1CFB
        add    esp, 10h
loc_8_3B1F:
        mov    eax, dword ptr [esp + 1ch]
loc_8_3B23:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 54h
        ret
loc_8_3B2B:
        mov    dword ptr [esp + 1ch], 0eh
        jmp    SHORT loc_8_3AE8
loc_8_3B35:
        mov    dword ptr [esp + 1ch], 0eh
        jmp    SHORT loc_8_3AE8
loc_8_3B3F:
        mov    dword ptr [esp + 1ch], 3f7h
        or     dword ptr [esi + 10h], 20h
        jmp    SHORT loc_8_3AE8
loc_8_3B4D:
        mov    dword ptr [esp + 1ch], 3f1h
        or     dword ptr [esi + 10h], 8
        jmp    SHORT loc_8_3AE8
sub_8_3B5B:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 18h]
        mov    ebx, 20h
        mov    edi, dword ptr [esi + 24h]
        mov    eax, dword ptr [edi + 10h]
        or     eax, 8
        mov    dword ptr [edi + 10h], eax
        and    eax, 0fffffffah
        mov    dword ptr [edi + 10h], eax
        cmp    dword ptr [edi + 4], ebx
        jbe    NEAR PTR loc_8_3C1B
loc_8_3B86:
        lea    ebp, [edi + ebx]
        mov    eax, dword ptr [ebp]
        or     eax, eax
        jne    SHORT loc_8_3BEF
        cmp    dword ptr [ebp + 18h], -1
        je     SHORT loc_8_3BEA
        lea    eax, [esp + 10h]
        push   eax
        push   ebp
        push   esi
        call   sub_8_22A8
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_8_3BEA
        mov    eax, dword ptr [esp + 10h]
        movzx  eax, word ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [esp + 14h]
        add    eax, 14h
        push   eax
        db  068h, 05Ch, 0F9h, 000h, 000h
        call   sub_8_366A
        mov    eax, dword ptr [esp + 1ch]
        add    esp, 0ch
        movzx  eax, word ptr [eax + 0ch]
        db  068h, 05Ch, 0F9h, 000h, 000h
        db  0C6h, 080h, 05Ch, 0F9h, 000h, 000h, 000h
        call   sub_8_36EB
        add    esp, 4
        mov    dword ptr [ebp + 4], eax
        and    dword ptr [edi + 10h], 0fffffffah
loc_8_3BEA:
        add    ebx, 1ch
        jmp    SHORT loc_8_3BF9
loc_8_3BEF:
        cmp    eax, 80000000h
        jne    SHORT loc_8_3C1B
        add    ebx, dword ptr [ebp + 4]
loc_8_3BF9:
        mov    eax, ebx
        shr    eax, 0ch
        or     eax, eax
        je     SHORT loc_8_3C12
        lea    ecx, [ebx + 1ch]
        shr    ecx, 0ch
        cmp    ecx, eax
        jbe    SHORT loc_8_3C12
        lea    ebx, [eax + 1]
        shl    ebx, 0ch
loc_8_3C12:
        cmp    dword ptr [edi + 4], ebx
        ja     NEAR PTR loc_8_3B86
loc_8_3C1B:
        push   esi
        call   sub_8_52BC
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_8_3C2C:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 1ch]
        test   byte ptr [edi + 11h], 1
        jne    NEAR PTR loc_8_3D20
        lea    eax, [esp + 14h]
        push   eax
        push   0
        push   0
        push   dword ptr [edi + 14h]
        call   sub_8_1C7C
        add    esp, 10h
        mov    ebx, eax
        cmp    ebx, -1
        jne    SHORT loc_8_3C65
        or     dword ptr [edi + 10h], 20h
        jmp    NEAR PTR loc_8_3CF5
loc_8_3C65:
        push   201h
        push   10000h
        db  0FFh, 015h, 064h, 0D4h, 000h, 000h
        mov    dword ptr [esp + 18h], eax
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_8_3C89
        or     dword ptr [edi + 10h], 400h
        jmp    SHORT loc_8_3CF5
loc_8_3C89:
        mov    esi, dword ptr [edi + 28h]
        cmp    esi, -1
        jne    SHORT loc_8_3C9A
        or     dword ptr [edi + 10h], 400h
        jmp    SHORT loc_8_3CF5
loc_8_3C9A:
        mov    bp, word ptr [edi + 0eh]
        or     bp, bp
        je     SHORT loc_8_3CF5
loc_8_3CA3:
        cmp    dword ptr [esi], -1
        jne    SHORT loc_8_3CDF
        push   0
        push   0
        push   dword ptr [esi + 4]
        push   ebx
        call   sub_8_1E29
        add    esp, 10h
        push   0
        push   dword ptr [esi + 8]
        push   dword ptr [esp + 18h]
        push   ebx
        call   sub_8_1D9E
        add    esp, 10h
        cmp    eax, -1
        je     SHORT loc_8_3CEB
        push   dword ptr [esp + 10h]
        call   sub_8_4E84
        add    esp, 4
        or     eax, eax
        je     SHORT loc_8_3CF1
loc_8_3CDF:
        dec    bp
        add    esi, 0ch
        or     bp, bp
        jne    SHORT loc_8_3CA3
        jmp    SHORT loc_8_3CF5
loc_8_3CEB:
        or     dword ptr [edi + 10h], 20h
        jmp    SHORT loc_8_3CF5
loc_8_3CF1:
        or     dword ptr [edi + 10h], 8
loc_8_3CF5:
        cmp    ebx, -1
        je     SHORT loc_8_3D0A
        push   0
        push   0
        push   ebx
        push   dword ptr [edi + 14h]
        call   sub_8_1CFB
        add    esp, 10h
loc_8_3D0A:
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_8_3D20
        push   0
        push   dword ptr [esp + 14h]
        db  0FFh, 015h, 06Ch, 0D4h, 000h, 000h
        add    esp, 8
loc_8_3D20:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_8_3D28:
        sub    esp, 220h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 20h], 0ffffffffh
        mov    dword ptr [esp + 14h], 0
        db  066h, 083h, 03Dh, 0C8h, 003h, 000h, 000h, 000h
        je     SHORT loc_8_3D51
        jmp    NEAR PTR loc_8_429D
loc_8_3D51:
        mov    esi, dword ptr [esp + 234h]
        mov    eax, dword ptr [esi + 10h]
        test   al, 1
        je     NEAR PTR loc_8_429D
        cmp    dword ptr [esi + 2ch], 0
        jne    NEAR PTR loc_8_429D
        test   ah, 1
        je     SHORT loc_8_3D77
        jmp    NEAR PTR loc_8_429D
loc_8_3D77:
        test   al, 38h
        je     SHORT loc_8_3D85
        mov    eax, 3f7h
loc_8_3D80:
        jmp    NEAR PTR loc_8_429F
loc_8_3D85:
        push   dword ptr [esi + 20h]
        call   sub_8_4DB7
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_8_4292
        push   dword ptr [esi + 24h]
        call   sub_8_4E23
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_8_4292
        mov    eax, dword ptr [esi + 10h]
        test   ah, 40h
        je     SHORT loc_8_3DB8
        jmp    NEAR PTR loc_8_429D
loc_8_3DB8:
        or     eax, 4000h
        mov    dword ptr [esi + 10h], eax
        and    eax, 0fffff79fh
        mov    dword ptr [esi + 10h], eax
        push   esi
        call   sub_8_4C81
        mov    word ptr [esp + 16h], ax
        lea    eax, [esp + 2ch]
        add    esp, 4
        cmp    word ptr [esp + 12h], 1
        push   eax
        sbb    eax, eax
        neg    eax
        push   eax
        push   0
        push   dword ptr [esi + 14h]
        call   sub_8_1C7C
        mov    dword ptr [esp + 34h], eax
        add    esp, 10h
        cmp    eax, -1
        jne    SHORT loc_8_3E05
        or     dword ptr [esi + 10h], 20h
        jmp    NEAR PTR loc_8_4117
loc_8_3E05:
        cmp    word ptr [esp + 12h], 0
        je     SHORT loc_8_3E5E
        lea    eax, [esp + 30h]
        push   7eh
        push   eax
        push   dword ptr [esi + 14h]
        call   sub_8_4CC9
        add    esp, 0ch
        lea    eax, [esp + 30h]
        push   0
        push   eax
        call   sub_8_1F76
        add    esp, 8
        or     eax, eax
        je     SHORT loc_8_3E41
        lea    eax, [esp + 30h]
        push   0
        push   eax
        call   sub_8_1EA3
        add    esp, 8
loc_8_3E41:
        lea    eax, [esp + 30h]
        push   0
        push   eax
        call   sub_8_1E66
        mov    dword ptr [esp + 28h], eax
        add    esp, 8
        cmp    eax, -1
        jne    SHORT loc_8_3E66
        jmp    NEAR PTR loc_8_4113
loc_8_3E5E:
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [esp + 20h], eax
loc_8_3E66:
        mov    eax, dword ptr [esi + 20h]
        test   byte ptr [eax + 12h], 1
        jne    SHORT loc_8_3E77
        cmp    word ptr [esp + 12h], 0
        je     SHORT loc_8_3E92
loc_8_3E77:
        push   0
        push   20h
        push   eax
        push   dword ptr [esp + 2ch]
        call   sub_8_1DF0
        add    esp, 10h
        cmp    eax, -1
        jne    SHORT loc_8_3E92
        jmp    NEAR PTR loc_8_4113
loc_8_3E92:
        mov    eax, dword ptr [esi + 24h]
        test   byte ptr [eax + 10h], 1
        jne    SHORT loc_8_3EA3
        cmp    word ptr [esp + 12h], 0
        je     SHORT loc_8_3ED4
loc_8_3EA3:
        push   0
        push   0
        push   20h
        push   dword ptr [esp + 2ch]
        call   sub_8_1E29
        add    esp, 10h
        mov    eax, dword ptr [esi + 24h]
        push   0
        push   dword ptr [eax + 4]
        push   eax
        push   dword ptr [esp + 2ch]
        call   sub_8_1DF0
        add    esp, 10h
        cmp    eax, -1
        jne    SHORT loc_8_3ED4
        jmp    NEAR PTR loc_8_4113
loc_8_3ED4:
        mov    bx, word ptr [esi + 0eh]
        mov    edx, dword ptr [esi + 28h]
        movzx  eax, bx
        imul   eax, eax, 0ch
        lea    ecx, [edx + eax]
        sub    di, di
        cmp    bx, di
        je     SHORT loc_8_3F20
        mov    ebx, dword ptr [esp + 2ch]
loc_8_3EF0:
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        or     di, di
        jne    SHORT loc_8_3F07
        mov    eax, dword ptr [esi + 20h]
        mov    eax, dword ptr [eax + 8]
        jmp    SHORT loc_8_3F0D
loc_8_3F07:
        mov    eax, dword ptr [ebx + 4]
        add    eax, dword ptr [ebx + 8]
loc_8_3F0D:
        mov    dword ptr [ecx + 4], eax
        mov    ebx, ecx
        inc    di
        add    edx, 0ch
        add    ecx, 0ch
        cmp    word ptr [esi + 0eh], di
        ja     SHORT loc_8_3EF0
loc_8_3F20:
        mov    edx, dword ptr [esi + 28h]
        mov    cx, word ptr [esi + 0eh]
        mov    dword ptr [esp + 18h], edx
        movzx  eax, cx
        imul   eax, eax, 0ch
        mov    edi, edx
        add    edi, eax
        sub    bp, bp
        cmp    cx, bp
        je     NEAR PTR loc_8_3FF1
loc_8_3F41:
        mov    ebx, dword ptr [edi]
        cmp    ebx, -1
        je     NEAR PTR loc_8_3FDD
        push   ebx
        call   sub_8_4E84
        add    esp, 4
loc_8_3F55:
        or     eax, eax
        je     NEAR PTR loc_8_4105
        mov    eax, dword ptr [ebx + 0ch]
        test   al, 8
        je     SHORT loc_8_3F7D
        shr    eax, 10h
        cmp    ax, bp
        je     SHORT loc_8_3F93
        or     dword ptr [esi + 10h], 10h
        mov    dword ptr [esp + 14h], 3f7h
        jmp    NEAR PTR loc_8_411F
loc_8_3F7D:
        and    eax, 0fffbh
        mov    dword ptr [ebx + 0ch], eax
        movzx  ecx, bp
        shl    ecx, 10h
        add    ecx, 8
        or     ecx, eax
        mov    dword ptr [ebx + 0ch], ecx
loc_8_3F93:
        test   byte ptr [ebx + 0ch], 1
        je     SHORT loc_8_3FBD
        mov    edx, dword ptr [esp + 18h]
        push   dword ptr [edx + 8]
        push   ebx
        call   sub_8_4B7D
        add    esp, 8
        or     eax, eax
        je     SHORT loc_8_3FB7
        push   ebx
        push   esi
        call   sub_8_52F0
        add    esp, 8
loc_8_3FB7:
        test   byte ptr [ebx + 0ch], 1
        jne    SHORT loc_8_3FC5
loc_8_3FBD:
        cmp    word ptr [esp + 12h], 0
        je     SHORT loc_8_3FDD
loc_8_3FC5:
        push   esi
        push   1
        push   edi
        push   dword ptr [esp + 2ch]
        call   sub_8_42AA
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_8_4113
loc_8_3FDD:
        inc    bp
        add    edi, 0ch
        add    dword ptr [esp + 18h], 0ch
        cmp    word ptr [esi + 0eh], bp
        ja     NEAR PTR loc_8_3F41
loc_8_3FF1:
        cmp    word ptr [esp + 12h], 0
        je     NEAR PTR loc_8_411F
        mov    ecx, dword ptr [esi + 28h]
        mov    dx, word ptr [esi + 0eh]
        mov    dword ptr [esp + 18h], ecx
        movzx  eax, dx
        imul   eax, eax, 0ch
        add    eax, ecx
        mov    dword ptr [esp + 1ch], eax
        mov    word ptr [esp + 10h], 0
        or     dx, dx
        je     NEAR PTR loc_8_411F
        sub    edi, edi
loc_8_4026:
        mov    ecx, dword ptr [esp + 18h]
        cmp    dword ptr [ecx], -1
        jne    NEAR PTR loc_8_40E5
        mov    eax, dword ptr [esp + 1ch]
        cmp    dword ptr [eax + 8], edi
        je     NEAR PTR loc_8_40E5
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 14h]
        push   esi
        call   sub_8_4350
        mov    dword ptr [esp + 20h], eax
        add    esp, 0ch
        cmp    eax, edi
        je     SHORT loc_8_40B5
        cmp    eax, 0eh
        jne    NEAR PTR loc_8_411F
        mov    ebp, dword ptr [esi + 28h]
        cmp    word ptr [esi + 0eh], di
        mov    bx, di
        jbe    SHORT loc_8_409C
loc_8_406E:
        mov    eax, dword ptr [ebp]
        cmp    eax, -1
        je     SHORT loc_8_4091
        test   byte ptr [eax + 0ch], 1
        jne    SHORT loc_8_4091
        push   ebp
        push   esi
        call   sub_8_468A
        add    esp, 8
        movzx  eax, word ptr [esi + 0eh]
        imul   eax, eax, 0ch
        mov    dword ptr [ebp + eax + 8], edi
loc_8_4091:
        inc    bx
        add    ebp, 0ch
        cmp    word ptr [esi + 0eh], bx
        ja     SHORT loc_8_406E
loc_8_409C:
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 14h]
        push   esi
        call   sub_8_4350
        mov    dword ptr [esp + 20h], eax
        add    esp, 0ch
        cmp    eax, edi
        jne    SHORT loc_8_411F
loc_8_40B5:
        mov    ecx, dword ptr [esp + 18h]
        mov    eax, dword ptr [esp + 1ch]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ecx + 0ch]
        shr    eax, 10h
        cmp    ax, word ptr [esp + 10h]
        jne    SHORT loc_8_4105
        push   esi
        push   1
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 2ch]
        call   sub_8_42AA
        add    esp, 10h
        cmp    eax, edi
        jne    SHORT loc_8_4113
loc_8_40E5:
        inc    word ptr [esp + 10h]
        add    dword ptr [esp + 18h], 0ch
        mov    ax, word ptr [esp + 10h]
        add    dword ptr [esp + 1ch], 0ch
        cmp    word ptr [esi + 0eh], ax
        ja     NEAR PTR loc_8_4026
        jmp    SHORT loc_8_411F
loc_8_4105:
        or     dword ptr [esi + 10h], 10h
        mov    dword ptr [esp + 14h], 3f7h
        jmp    SHORT loc_8_411F
loc_8_4113:
        or     dword ptr [esi + 10h], 40h
loc_8_4117:
        mov    dword ptr [esp + 14h], 3f8h
loc_8_411F:
        cmp    dword ptr [esp + 24h], -1
        je     SHORT loc_8_4142
        cmp    word ptr [esp + 12h], 1
        sbb    eax, eax
        neg    eax
        push   eax
        push   0
        push   dword ptr [esp + 2ch]
        push   dword ptr [esi + 14h]
        call   sub_8_1CFB
        add    esp, 10h
loc_8_4142:
        mov    eax, dword ptr [esp + 20h]
        cmp    eax, dword ptr [esp + 24h]
        je     SHORT loc_8_4166
        cmp    eax, -1
        je     SHORT loc_8_4166
        push   1
        push   0
        lea    eax, [esp + 38h]
        push   dword ptr [esp + 28h]
        push   eax
        call   sub_8_1CFB
        add    esp, 10h
loc_8_4166:
        cmp    dword ptr [esp + 14h], 0
        jne    NEAR PTR loc_8_4202
        cmp    word ptr [esp + 12h], 0
        je     NEAR PTR loc_8_4215
        lea    eax, [esp + 130h]
        push   73h
        push   eax
        push   dword ptr [esi + 14h]
        call   sub_8_4CC9
        add    esp, 0ch
        lea    eax, [esp + 130h]
        push   0
        push   eax
        call   sub_8_1EA3
        add    esp, 8
        lea    eax, [esp + 130h]
        push   0
        push   eax
        push   dword ptr [esi + 14h]
        call   sub_8_1EF0
        add    esp, 0ch
        push   0
        lea    eax, [esp + 34h]
        push   dword ptr [esi + 14h]
        push   eax
        call   sub_8_1EF0
        add    esp, 0ch
        or     eax, eax
        push   0
        jne    SHORT loc_8_41F9
        lea    eax, [esp + 34h]
        push   eax
        call   sub_8_1EA3
        add    esp, 8
        push   0
        lea    eax, [esp + 134h]
        push   dword ptr [esi + 14h]
        push   eax
        call   sub_8_1EF0
        add    esp, 0ch
        or     dword ptr [esi + 10h], 40h
        jmp    SHORT loc_8_4215
loc_8_41F9:
        lea    eax, [esp + 134h]
        jmp    SHORT loc_8_420C
loc_8_4202:
        or     dword ptr [esi + 10h], 40h
        lea    eax, [esp + 30h]
        push   0
loc_8_420C:
        push   eax
        call   sub_8_1EA3
        add    esp, 8
loc_8_4215:
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_8_4285
        and    dword ptr [esi + 10h], 0fffffffeh
        mov    cx, word ptr [esi + 0eh]
        mov    ebx, dword ptr [esi + 28h]
        movzx  eax, cx
        imul   eax, eax, 0ch
        lea    edi, [ebx + eax]
        sub    bp, bp
        cmp    cx, bp
        je     SHORT loc_8_4261
loc_8_4238:
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [ebx]
        cmp    eax, -1
        je     SHORT loc_8_4253
        and    dword ptr [eax + 0ch], 0fffffff8h
        push   dword ptr [ebx]
        call   sub_8_4E84
        add    esp, 4
loc_8_4253:
        inc    bp
        add    ebx, 0ch
        add    edi, 0ch
        cmp    word ptr [esi + 0eh], bp
        ja     SHORT loc_8_4238
loc_8_4261:
        mov    eax, dword ptr [esi + 24h]
        and    dword ptr [eax + 10h], 0fffffff8h
        push   dword ptr [esi + 24h]
        call   sub_8_4E23
        add    esp, 4
        mov    eax, dword ptr [esi + 20h]
        and    dword ptr [eax + 12h], 0fffffffah
        push   dword ptr [esi + 20h]
        call   sub_8_4DB7
        add    esp, 4
loc_8_4285:
        and    dword ptr [esi + 10h], 0ffffbfffh
        mov    eax, dword ptr [esp + 14h]
        jmp    SHORT loc_8_429F
loc_8_4292:
        or     dword ptr [esi + 10h], 10h
        mov    eax, 3f7h
        jmp    SHORT loc_8_429F
loc_8_429D:
        sub    eax, eax
loc_8_429F:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 220h
        ret
sub_8_42AA:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  066h, 083h, 03Dh, 0C8h, 003h, 000h, 000h, 000h
        je     SHORT loc_8_42C2
        mov    eax, 0a7h
        jmp    NEAR PTR loc_8_434B
loc_8_42C2:
        mov    edi, dword ptr [esp + 20h]
        mov    esi, dword ptr [esp + 18h]
        mov    ebp, dword ptr [edi + 10h]
        mov    ebx, ebp
        and    ebx, 4000h
        jne    SHORT loc_8_42EC
        push   dword ptr [esi]
        call   sub_8_4E84
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_8_42EC
        mov    eax, 3f1h
        jmp    SHORT loc_8_434B
loc_8_42EC:
        test   ebp, 100h
        je     SHORT loc_8_42F6
        jmp    SHORT loc_8_4349
loc_8_42F6:
        cmp    word ptr [esp + 1ch], 0
        je     SHORT loc_8_4315
        push   dword ptr [edi + 2ch]
        push   0
        mov    ebp, dword ptr [esp + 1ch]
        push   dword ptr [esi + 4]
        push   ebp
        call   sub_8_1E29
        add    esp, 10h
        jmp    SHORT loc_8_4319
loc_8_4315:
        mov    ebp, dword ptr [esp + 14h]
loc_8_4319:
        push   dword ptr [edi + 2ch]
        push   dword ptr [esi + 8]
        push   dword ptr [esi]
        push   ebp
        call   sub_8_1DF0
        add    esp, 10h
        cmp    eax, -1
        jne    SHORT loc_8_4336
        mov    eax, 3f8h
        jmp    SHORT loc_8_434B
loc_8_4336:
        or     ebx, ebx
        jne    SHORT loc_8_4349
        mov    eax, dword ptr [esi]
        push   eax
        and    dword ptr [eax + 0ch], 0fffffff8h
        call   sub_8_4E84
        add    esp, 4
loc_8_4349:
        sub    eax, eax
loc_8_434B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_8_4350:
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 14h], 0
        db  066h, 083h, 03Dh, 0C8h, 003h, 000h, 000h, 000h
        je     SHORT loc_8_4373
        mov    eax, 0a7h
        jmp    NEAR PTR loc_8_4581
loc_8_4373:
        mov    esi, dword ptr [esp + 2ch]
        mov    ax, word ptr [esp + 30h]
        cmp    word ptr [esi + 0eh], ax
        ja     SHORT loc_8_4390
        or     dword ptr [esi + 10h], 8
        mov    eax, 3f1h
        jmp    NEAR PTR loc_8_4581
loc_8_4390:
        movzx  eax, word ptr [esp + 30h]
        mov    dword ptr [esp + 20h], eax
        imul   eax, eax, 0ch
        mov    dword ptr [esp + 1ch], eax
        mov    eax, dword ptr [esi + 28h]
        mov    ecx, dword ptr [esp + 1ch]
        mov    eax, dword ptr [eax + ecx + 8]
        mov    ebx, 1
        mov    dword ptr [esp + 18h], eax
loc_8_43B4:
        mov    eax, dword ptr [esi + 28h]
        mov    ecx, dword ptr [esp + 1ch]
        cmp    dword ptr [eax + ecx], -1
        jne    SHORT loc_8_441F
        push   201h
        push   dword ptr [esp + 1ch]
        db  0FFh, 015h, 064h, 0D4h, 000h, 000h
        add    esp, 8
        mov    edi, eax
        or     edi, edi
        db  0A1h, 064h, 0D4h, 000h, 000h
        jne    SHORT loc_8_43E0
        jmp    SHORT loc_8_4405
loc_8_43E0:
        push   201h
        push   3fch
        call   eax
        mov    dword ptr [esp + 1ch], eax
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_8_4426
        push   0
        push   edi
        db  0FFh, 015h, 06Ch, 0D4h, 000h, 000h
        add    esp, 8
        sub    edi, edi
loc_8_4405:
        push   -1
        push   esi
        call   sub_8_4589
        add    esp, 8
        mov    ebp, 0eh
        mov    eax, ebx
        inc    ebx
        cmp    eax, 1
        jle    SHORT loc_8_43B4
        jmp    SHORT loc_8_4428
loc_8_441F:
        sub    eax, eax
        jmp    NEAR PTR loc_8_4581
loc_8_4426:
        sub    ebp, ebp
loc_8_4428:
        mov    ebx, dword ptr [esp + 34h]
        cmp    ebx, -1
        jne    SHORT loc_8_4456
        lea    eax, [esp + 24h]
        push   eax
        push   0
        push   dword ptr [esi + 2ch]
        push   dword ptr [esi + 14h]
        call   sub_8_1C7C
        add    esp, 10h
        mov    ebx, eax
        cmp    ebx, -1
        jne    SHORT loc_8_4456
        or     dword ptr [esi + 10h], 20h
        mov    ebp, 3f8h
loc_8_4456:
        db  066h, 0A1h, 0D0h, 003h, 000h, 000h
        mov    word ptr [esp + 12h], ax
        db  066h, 0C7h, 005h, 0D0h, 003h, 000h, 000h, 0FFh, 000h
        or     ebp, ebp
        jne    NEAR PTR loc_8_4532
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_8_4486
        mov    ebx, dword ptr [esi + 30h]
        push   0
        push   ebx
        call   sub_8_83CE
        add    esp, 8
loc_8_4486:
        mov    ebp, dword ptr [esi + 28h]
        add    ebp, dword ptr [esp + 1ch]
        push   dword ptr [esi + 2ch]
        push   0
        push   dword ptr [ebp + 4]
        push   ebx
        call   sub_8_1E29
        add    esp, 10h
        push   dword ptr [esi + 2ch]
        push   dword ptr [esp + 1ch]
        push   edi
        push   ebx
        call   sub_8_1D9E
        add    esp, 10h
        cmp    eax, -1
        jne    SHORT loc_8_44BF
        or     dword ptr [esi + 10h], 20h
        mov    ebp, 3f8h
        jmp    SHORT loc_8_4532
loc_8_44BF:
        mov    eax, dword ptr [esp + 18h]
        cmp    dword ptr [edi + 4], eax
        jne    SHORT loc_8_4529
        push   edi
        call   sub_8_4E84
        add    esp, 4
        or     eax, eax
        je     SHORT loc_8_4529
        mov    eax, dword ptr [edi + 0ch]
        test   al, 8
        je     SHORT loc_8_44E6
        shr    eax, 10h
        cmp    ax, word ptr [esp + 30h]
        jne    SHORT loc_8_4529
loc_8_44E6:
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [edi + 18h], eax
        push   eax
        push   edi
        call   sub_8_48C7
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_8_44FF
        or     dword ptr [esi + 10h], 8
loc_8_44FF:
        mov    dword ptr [ebp], edi
        mov    eax, dword ptr [edi + 0ch]
        and    eax, 0fff8h
        mov    dword ptr [edi + 0ch], eax
        mov    ecx, dword ptr [esp + 20h]
        shl    ecx, 10h
        add    ecx, 8
        or     ecx, eax
        mov    dword ptr [edi + 0ch], ecx
        push   edi
        call   sub_8_4E84
        add    esp, 4
        sub    ebp, ebp
        jmp    SHORT loc_8_4532
loc_8_4529:
        or     dword ptr [esi + 10h], 8
        mov    ebp, 3f7h
loc_8_4532:
        mov    ax, word ptr [esp + 12h]
        cmp    dword ptr [esp + 34h], -1
        db  066h, 0A3h, 0D0h, 003h, 000h, 000h
        jne    SHORT loc_8_4555
        push   0
        push   dword ptr [esi + 2ch]
        push   ebx
        push   dword ptr [esi + 14h]
        call   sub_8_1CFB
        add    esp, 10h
loc_8_4555:
        or     ebp, ebp
        je     SHORT loc_8_457F
        or     edi, edi
        je     SHORT loc_8_4569
        push   0
        push   edi
        db  0FFh, 015h, 06Ch, 0D4h, 000h, 000h
        add    esp, 8
loc_8_4569:
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_8_457F
        push   0
        push   dword ptr [esp + 18h]
        db  0FFh, 015h, 06Ch, 0D4h, 000h, 000h
        add    esp, 8
loc_8_457F:
        mov    eax, ebp
loc_8_4581:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
sub_8_4589:
        push   ebx
        push   esi
        push   edi
        db  066h, 083h, 03Dh, 0C8h, 003h, 000h, 000h, 000h
        jne    NEAR PTR loc_8_4625
        mov    esi, dword ptr [esp + 10h]
        push   esi
        call   sub_8_4C81
        add    esp, 4
        or     ax, ax
        je     SHORT loc_8_45B5
        push   esi
        call   sub_8_3D28
        add    esp, 4
loc_8_45B5:
        mov    ebx, dword ptr [esp + 14h]
        push   ebx
        push   esi
        call   sub_8_4629
        add    esp, 8
        mov    edi, dword ptr [esi + 4]
        cmp    edi, -1
        je     SHORT loc_8_45F4
loc_8_45CB:
        push   edi
        call   sub_8_4C81
        add    esp, 4
        or     ax, ax
        je     SHORT loc_8_45E2
        push   edi
        call   sub_8_3D28
        add    esp, 4
loc_8_45E2:
        push   ebx
        push   edi
        call   sub_8_4629
        add    esp, 8
        mov    edi, dword ptr [edi + 4]
        cmp    edi, -1
        jne    SHORT loc_8_45CB
loc_8_45F4:
        mov    esi, dword ptr [esi + 8]
        cmp    esi, -1
        je     SHORT loc_8_4625
loc_8_45FC:
        push   esi
        call   sub_8_4C81
        add    esp, 4
        or     ax, ax
        je     SHORT loc_8_4613
        push   esi
        call   sub_8_3D28
        add    esp, 4
loc_8_4613:
        push   ebx
        push   esi
        call   sub_8_4629
        add    esp, 8
        mov    esi, dword ptr [esi + 8]
        cmp    esi, -1
        jne    SHORT loc_8_45FC
loc_8_4625:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_8_4629:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        test   byte ptr [esi + 11h], 1
        jne    SHORT loc_8_4685
        mov    ebx, dword ptr [esi + 28h]
        cmp    ebx, -1
        je     SHORT loc_8_4685
        push   esi
        call   sub_8_4C81
        add    esp, 4
        or     ax, ax
        jne    SHORT loc_8_4685
        sub    bp, bp
        cmp    word ptr [esi + 0eh], bp
        jbe    SHORT loc_8_4685
        mov    edi, dword ptr [esp + 18h]
loc_8_465A:
        mov    eax, dword ptr [ebx]
        cmp    eax, -1
        je     SHORT loc_8_467A
        cmp    edi, eax
        je     SHORT loc_8_467A
        mov    eax, dword ptr [eax + 0ch]
        shr    eax, 10h
        cmp    ax, bp
        jne    SHORT loc_8_467A
        push   ebx
        push   esi
        call   sub_8_468A
        add    esp, 8
loc_8_467A:
        inc    bp
        add    ebx, 0ch
        cmp    word ptr [esi + 0eh], bp
        ja     SHORT loc_8_465A
loc_8_4685:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_8_468A:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 20h]
        mov    edi, dword ptr [esi]
        cmp    edi, -1
        je     SHORT loc_8_4715
        test   byte ptr [edi + 0ch], 1
        je     SHORT loc_8_46EF
        mov    ebx, dword ptr [esp + 1ch]
        cmp    dword ptr [ebx + 2ch], 0
        jne    SHORT loc_8_46EF
        lea    eax, [esp + 14h]
        push   eax
        push   1
        push   0
        push   dword ptr [ebx + 14h]
        call   sub_8_1C7C
        add    esp, 10h
        mov    ebp, eax
        cmp    ebp, -1
        je     SHORT loc_8_4715
        push   ebx
        push   1
        push   esi
        push   ebp
        call   sub_8_42AA
        mov    dword ptr [esp + 20h], eax
        add    esp, 10h
        push   1
        push   0
        push   ebp
        push   dword ptr [ebx + 14h]
        call   sub_8_1CFB
        add    esp, 10h
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_8_4715
loc_8_46EF:
        mov    eax, dword ptr [edi + 18h]
        cmp    eax, -1
        je     SHORT loc_8_4703
        push   0
        push   eax
        db  0FFh, 015h, 06Ch, 0D4h, 000h, 000h
        add    esp, 8
loc_8_4703:
        push   0
        push   edi
        db  0FFh, 015h, 06Ch, 0D4h, 000h, 000h
        add    esp, 8
        mov    dword ptr [esi], 0ffffffffh
loc_8_4715:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_8_471D:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 20h]
        mov    bp, word ptr [esp + 24h]
        add    bp, 4dh
        mov    bx, word ptr [esi + 0eh]
        and    bp, 0fffch
        movzx  edi, bx
        push   201h
        lea    eax, [edi + 1]
        imul   eax, eax, 18h
        push   eax
        push   dword ptr [esi + 28h]
loc_8_474C:
        db  0FFh, 015h, 068h, 0D4h, 000h, 000h
        mov    dword ptr [esp + 20h], eax
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_8_4764
        mov    eax, 0eh
        jmp    SHORT loc_8_47C4
loc_8_4764:
        push   ebx
        lea    eax, [esp + 1ch]
        push   ebp
        push   eax
        push   esi
        call   sub_8_47CC
        mov    dword ptr [esp + 20h], eax
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_8_477E
        jmp    SHORT loc_8_47C4
loc_8_477E:
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [esi + 28h], eax
        imul   edi, edi, 0ch
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [edi + eax], ecx
        dec    bx
        push   ebx
        push   esi
        call   sub_8_489C
        add    esp, 8
        mov    ecx, dword ptr [esi + 28h]
        mov    dword ptr [edi + ecx + 4], eax
        movzx  eax, bp
        mov    ecx, dword ptr [esi + 28h]
        mov    dword ptr [edi + ecx + 8], eax
        mov    eax, dword ptr [esi + 20h]
        inc    word ptr [esi + 0eh]
        inc    word ptr [eax + 10h]
        push   esi
        call   sub_8_5288
        mov    eax, dword ptr [esp + 14h]
        add    esp, 4
loc_8_47C4:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
sub_8_47CC:
        push   esi
        push   edi
        movzx  edi, word ptr [esp + 14h]
        push   201h
        push   edi
        db  0FFh, 015h, 064h, 0D4h, 000h, 000h
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_8_47F2
        mov    eax, 0eh
        jmp    NEAR PTR loc_8_4873
loc_8_47F2:
        push   201h
        push   3fch
        db  0FFh, 015h, 064h, 0D4h, 000h, 000h
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_8_481C
        push   0
        push   esi
        db  0FFh, 015h, 06Ch, 0D4h, 000h, 000h
        add    esp, 8
        mov    eax, 0eh
        jmp    SHORT loc_8_4873
loc_8_481C:
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [ecx], esi
        mov    dword ptr [esi], 42444752h
        lea    ecx, [edi - 20h]
        mov    dword ptr [esi + 8], ecx
        mov    dword ptr [esi + 4], edi
        mov    word ptr [esi + 16h], 0
        mov    word ptr [esi + 14h], 0
        mov    dword ptr [esi + 18h], eax
        lea    eax, [esi + 20h]
        mov    dword ptr [esi + 10h], 20h
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 4], 0ffffffffh
        mov    dword ptr [eax + 8], 0ffffffffh
        movzx  eax, word ptr [esp + 18h]
        shl    eax, 10h
        add    eax, 9
        mov    dword ptr [esi + 0ch], eax
        push   esi
        call   sub_8_4E84
        add    esp, 4
        sub    eax, eax
loc_8_4873:
        pop    edi
        pop    esi
        ret
        db  056h, 08Bh, 074h, 024h, 008h, 083h, 0FEh, 0FFh, 074h, 01Ah, 06Ah, 000h, 0FFh, 076h, 018h, 0FFh
        db  015h, 06Ch, 0D4h, 000h, 000h, 083h, 0C4h, 008h, 06Ah, 000h, 056h, 0FFh, 015h, 06Ch, 0D4h, 000h
        db  000h, 083h, 0C4h, 008h, 05Eh, 0C3h
sub_8_489C:
        mov    ax, word ptr [esp + 8]
        cmp    ax, 0ffffh
        jne    SHORT loc_8_48B3
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 20h]
        mov    eax, dword ptr [eax + 8]
        jmp    SHORT loc_8_48C6
loc_8_48B3:
        movzx  ecx, ax
        mov    eax, dword ptr [esp + 4]
        imul   ecx, ecx, 0ch
        add    ecx, dword ptr [eax + 28h]
        mov    eax, dword ptr [ecx + 4]
        add    eax, dword ptr [ecx + 8]
loc_8_48C6:
        ret
sub_8_48C7:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 1ch]
        db  0C7h, 044h
loc_8_48D4:
        and    al, 14h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        lea    esi, [ecx + 20h]
        mov    edi, dword ptr [ecx + 4]
        lea    eax, [ecx + edi]
        mov    dword ptr [esp + 10h], eax
        mov    ebx, esi
        sub    edi, 20h
        cmp    esi, eax
        jae    SHORT loc_8_4927
        mov    edx, dword ptr [esp + 20h]
loc_8_48F4:
        mov    ebp, dword ptr [ebx]
        or     ebp, ebp
        je     SHORT loc_8_4934
        cmp    ebp, edi
        ja     SHORT loc_8_4934
        mov    eax, dword ptr [ebx + 4]
        cmp    eax, -1
        je     SHORT loc_8_4919
        cmp    ax, 0ffh
        jae    SHORT loc_8_4934
        cmp    eax, -1
        je     SHORT loc_8_4919
        movzx  eax, ax
        mov    dword ptr [edx + eax*4], ebx
        jmp    SHORT loc_8_491D
loc_8_4919:
        add    dword ptr [esp + 14h], ebp
loc_8_491D:
        sub    edi, ebp
        add    ebx, ebp
        cmp    dword ptr [esp + 10h], ebx
        ja     SHORT loc_8_48F4
loc_8_4927:
        mov    eax, 1
loc_8_492C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_8_4934:
        mov    eax, ebx
        lea    ebp, [eax + 0ch]
        cmp    ebp, dword ptr [esp + 10h]
        jbe    SHORT loc_8_494F
        sub    eax, ecx
        mov    dword ptr [ecx + 4], eax
        lea    eax, [ebx - 15h]
        mov    dword ptr [esp + 10h], eax
        sub    edi, edi
        jmp    SHORT loc_8_495F
loc_8_494F:
        mov    dword ptr [ebx], edi
        mov    dword ptr [ebx + 4], 0ffffffffh
        mov    dword ptr [ebx + 8], 0ffffffffh
loc_8_495F:
        mov    eax, dword ptr [esp + 14h]
loc_8_4963:
        add    eax, edi
        mov    dword ptr [ecx + 8], eax
        mov    eax, 0ffffffffh
        cmp    esi, dword ptr [esp + 10h]
        jae    SHORT loc_8_4998
        mov    ebx, eax
loc_8_4975:
        cmp    dword ptr [esi + 4], ebx
        jne    SHORT loc_8_4990
        cmp    eax, ebx
        jne    SHORT loc_8_4987
        mov    eax, esi
        sub    eax, ecx
        mov    dword ptr [ecx + 10h], eax
        jmp    SHORT loc_8_498E
loc_8_4987:
        mov    edi, esi
        sub    edi, ecx
        mov    dword ptr [eax + 8], edi
loc_8_498E:
        mov    eax, esi
loc_8_4990:
        add    esi, dword ptr [esi]
        cmp    dword ptr [esp + 10h], esi
        ja     SHORT loc_8_4975
loc_8_4998:
        cmp    eax, -1
        je     SHORT loc_8_49A4
        mov    dword ptr [eax + 8], 0ffffffffh
loc_8_49A4:
        mov    word ptr [ecx + 16h], 0
        sub    ax, ax
loc_8_49AD:
        movzx  esi, ax
        cmp    dword ptr [edx + esi*4], 0
        je     SHORT loc_8_49C0
        inc    ax
loc_8_49B8:
        cmp    ax, 0ffh
        jb     SHORT loc_8_49AD
        jmp    SHORT loc_8_49C4
loc_8_49C0:
        mov    word ptr [ecx + 16h], ax
loc_8_49C4:
        mov    word ptr [ecx + 14h], 0
        mov    ax, 0feh
loc_8_49CE:
        movzx  esi, ax
        cmp    dword ptr [edx + esi*4], 0
        jne    SHORT loc_8_49DD
        dec    ax
        jne    SHORT loc_8_49CE
        jmp    SHORT loc_8_49E1
loc_8_49DD:
        mov    word ptr [ecx + 14h], ax
loc_8_49E1:
        sub    eax, eax
        jmp    NEAR PTR loc_8_492C
sub_8_49E8:
        push   ebx
        push   esi
        push   edi
        push   ebp
        movzx  esi, word ptr [esp + 18h]
        mov    eax, dword ptr [esp + 14h]
        imul   esi, esi, 0ch
        mov    ebp, dword ptr [eax + 28h]
        add    ebp, esi
        mov    ebx, dword ptr [esp + 1ch]
        mov    ecx, dword ptr [ebp]
        mov    eax, dword ptr [ecx + 8]
        cmp    ebx, eax
        jbe    SHORT loc_8_4A0D
        sub    ebx, eax
loc_8_4A0D:
        add    ebx, 0fffh
        and    ebx, 0fffff000h
        mov    edx, dword ptr [ebp + 8]
        add    edx, ebx
        mov    dword ptr [esp + 1ch], edx
        mov    eax, edx
        shr    eax, 10h
        or     ax, ax
        je     SHORT loc_8_4A36
        mov    eax, 0eah
        jmp    NEAR PTR loc_8_4ACF
loc_8_4A36:
        push   201h
        push   dword ptr [esp + 20h]
        push   ecx
        db  0FFh, 015h, 068h, 0D4h, 000h, 000h
        add    esp, 0ch
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_8_4A56
        mov    eax, 0eh
        jmp    SHORT loc_8_4ACF
loc_8_4A56:
        push   dword ptr [ebp + 8]
        push   edi
        call   sub_8_4B7D
        add    esp, 8
        add    dword ptr [edi + 8], ebx
        mov    eax, dword ptr [edi + 10h]
        cmp    eax, -1
        lea    ecx, [eax + edi]
        jne    SHORT loc_8_4A8D
        mov    eax, dword ptr [edi + 4]
        lea    ecx, [edi + eax]
        mov    dword ptr [ecx], 0
        mov    dword ptr [ecx + 4], 0ffffffffh
        mov    dword ptr [ecx + 8], 0ffffffffh
        mov    dword ptr [edi + 10h], eax
loc_8_4A8D:
        mov    edx, dword ptr [esp + 1ch]
        mov    dword ptr [edi + 4], edx
        add    dword ptr [ecx], ebx
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [ebp], edi
        mov    eax, dword ptr [eax + 28h]
        mov    dword ptr [esi + eax + 8], edx
        push   dword ptr [edi + 18h]
        push   edi
        call   sub_8_48C7
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_8_4ABC
        mov    eax, dword ptr [esp + 14h]
        or     dword ptr [eax + 10h], 8
loc_8_4ABC:
        push   edi
        or     dword ptr [edi + 0ch], 2
        push   dword ptr [esp + 18h]
        call   sub_8_52F0
        add    esp, 8
        sub    eax, eax
loc_8_4ACF:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_8_4AD4:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        push   201h
        mov    eax, dword ptr [esi + 24h]
        mov    ebp, dword ptr [eax + 4]
        lea    ebx, [ebp + 1800h]
        and    ebx, 0fffff000h
        push   ebx
        push   eax
        db  0FFh, 015h, 068h, 0D4h, 000h, 000h
        add    esp, 0ch
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_8_4B0B
        mov    eax, 0eh
        jmp    SHORT loc_8_4B78
loc_8_4B0B:
        mov    dword ptr [esi + 24h], edi
        mov    dword ptr [edi + 4], ebx
        mov    ecx, dword ptr [esi + 20h]
        lea    eax, [ebx + 20h]
        mov    dword ptr [ecx + 8], eax
        push   esi
        call   sub_8_5288
        add    esp, 4
        lea    ecx, [edi + ebp]
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [ecx], 0ffffffffh
        sub    ebx, ebp
        mov    dword ptr [ecx + 4], ebx
        mov    dword ptr [ecx + 8], 0ffffffffh
        cmp    eax, -1
        jne    SHORT loc_8_4B47
        sub    ecx, edi
        mov    dword ptr [edi + 0ch], ecx
        jmp    SHORT loc_8_4B69
loc_8_4B47:
        add    eax, edi
        cmp    dword ptr [eax + 8], -1
        je     SHORT loc_8_4B5E
        mov    edx, 0ffffffffh
loc_8_4B54:
        mov    eax, dword ptr [eax + 8]
        add    eax, edi
        cmp    dword ptr [eax + 8], edx
        jne    SHORT loc_8_4B54
loc_8_4B5E:
        sub    ecx, edi
        mov    dword ptr [eax + 8], ecx
        mov    dword ptr [eax], 80000000h
loc_8_4B69:
        or     dword ptr [edi + 10h], 2
        push   esi
        call   sub_8_52BC
        add    esp, 4
        sub    eax, eax
loc_8_4B78:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_8_4B7D:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, 0ffffffffh
        mov    edi, dword ptr [esp + 1ch]
        add    edi, dword ptr [esp + 18h]
        mov    ebx, dword ptr [esp + 18h]
        add    ebx, 20h
        cmp    edi, ebx
        jbe    SHORT loc_8_4C18
loc_8_4B9C:
        sub    ebp, ebp
        mov    eax, 0ffffffffh
loc_8_4BA3:
        cmp    dword ptr [ebx + 4], eax
        je     SHORT loc_8_4BB5
        add    ebx, dword ptr [ebx]
        cmp    edi, ebx
        ja     SHORT loc_8_4BA3
        sub    eax, eax
        jmp    NEAR PTR loc_8_4C41
loc_8_4BB5:
        cmp    esi, -1
        jne    SHORT loc_8_4BBC
        mov    esi, ebx
loc_8_4BBC:
        mov    eax, 0ffffffffh
loc_8_4BC1:
        cmp    dword ptr [ebx + 4], eax
        jne    SHORT loc_8_4BCC
        add    ebx, dword ptr [ebx]
        cmp    edi, ebx
        ja     SHORT loc_8_4BC1
loc_8_4BCC:
        mov    dword ptr [esp + 10h], ebx
        cmp    edi, ebx
        jbe    SHORT loc_8_4C18
        mov    ecx, 0ffffffffh
loc_8_4BD9:
        cmp    dword ptr [ebx + 4], ecx
        je     SHORT loc_8_4BE8
        mov    eax, dword ptr [ebx]
        add    ebp, eax
        add    ebx, eax
        cmp    edi, ebx
        ja     SHORT loc_8_4BD9
loc_8_4BE8:
        push   ebp
        push   dword ptr [esp + 14h]
        push   esi
        call   sub_8_366A
        mov    eax, dword ptr [esp + 1ch]
        add    esp, 0ch
        mov    ecx, eax
        sub    ecx, esi
        push   ecx
        add    eax, ebp
        push   eax
        add    esi, ebp
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 24h]
        call   sub_8_4C49
        add    esp, 10h
        cmp    edi, ebx
        ja     SHORT loc_8_4B9C
loc_8_4C18:
        mov    ecx, esi
        sub    edi, ecx
        mov    dword ptr [esi], edi
        mov    dword ptr [esi + 4], 0ffffffffh
        mov    dword ptr [esi + 8], 0ffffffffh
        mov    eax, dword ptr [esp + 18h]
        sub    ecx, dword ptr [esp + 18h]
        mov    dword ptr [eax + 10h], ecx
        mov    ecx, dword ptr [esi]
        mov    dword ptr [eax + 8], ecx
        mov    eax, 1
loc_8_4C41:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_8_4C49:
        push   ebx
        push   esi
        push   edi
        db  08Bh, 044h, 024h
sub_8_4C4F:
        adc    byte ptr [ebx - 40f0e7a8h], cl
        jo     SHORT loc_8_4C6B
        or     esi, esi
        jl     SHORT loc_8_4C7D
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 18h]
        mov    edx, dword ptr [esp + 1ch]
loc_8_4C67:
        mov    edi, dword ptr [ebx]
        cmp    eax, edi
loc_8_4C6B:
        ja     SHORT loc_8_4C75
        cmp    ecx, edi
        db  076h
sub_8_4C70:
        add    al, 2bh
        cli
        mov    dword ptr [ebx], edi
loc_8_4C75:
        dec    esi
        add    ebx, 4
        or     esi, esi
        jge    SHORT loc_8_4C67
loc_8_4C7D:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_8_4C81:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [eax + 24h]
        test   byte ptr [ecx + 10h], 2
        je     SHORT loc_8_4C96
        mov    ax, 1
        jmp    SHORT loc_8_4CC0
loc_8_4C96:
        mov    edi, dword ptr [eax + 28h]
        sub    esi, esi
        movzx  ecx, word ptr [eax + 0eh]
        dec    ecx
        cmp    ecx, esi
        jle    SHORT loc_8_4CBD
        mov    edx, 2
loc_8_4CA9:
        mov    eax, dword ptr [edi]
        cmp    eax, -1
        je     SHORT loc_8_4CB5
        test   dword ptr [eax + 0ch], edx
        jne    SHORT loc_8_4CC3
loc_8_4CB5:
        inc    esi
        add    edi, 0ch
        cmp    esi, ecx
        jl     SHORT loc_8_4CA9
loc_8_4CBD:
        sub    ax, ax
loc_8_4CC0:
        pop    edi
        pop    esi
        ret
loc_8_4CC3:
        mov    ax, 1
        jmp    SHORT loc_8_4CC0
sub_8_4CC9:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        push   esi
        call   sub_8_3651
        movzx  edi, ax
        add    esp, 4
        lea    eax, [edi + 1]
        push   eax
        push   esi
        mov    esi, dword ptr [esp + 18h]
        push   esi
        call   sub_8_366A
        add    esp, 0ch
        lea    eax, [edi + esi - 3]
        mov    cl, byte ptr [esp + 14h]
        mov    byte ptr [eax], 7eh
        inc    eax
        pop    edi
        mov    byte ptr [eax], 7eh
        mov    byte ptr [eax + 1], cl
        mov    eax, esi
        pop    esi
        ret
sub_8_4D04:
        sub    esp, 0c8h
        push   esi
        push   edi
        lea    eax, [esp + 6ch]
        push   72h
        mov    esi, dword ptr [esp + 0d8h]
        push   eax
        push   esi
        call   sub_8_4CC9
        add    esp, 0ch
        lea    eax, [esp + 8]
        push   73h
        push   eax
        push   esi
        call   sub_8_4CC9
        add    esp, 0ch
        lea    eax, [esp + 6ch]
        push   0
        push   eax
        call   sub_8_1F76
        add    esp, 8
        mov    edi, eax
        and    al, 7
        cmp    al, 7
        jne    SHORT loc_8_4DAC
        lea    eax, [esp + 8]
        push   0
        push   eax
        call   sub_8_1EA3
        add    esp, 8
        lea    eax, [esp + 8]
        push   0
        push   eax
        push   esi
        call   sub_8_1EF0
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_8_4D72
        mov    edi, 3f8h
loc_8_4D72:
        push   0
        lea    eax, [esp + 70h]
        push   esi
        push   eax
        call   sub_8_1EF0
        add    esp, 0ch
        or     eax, eax
loc_8_4D84:
        jne    SHORT loc_8_4D9D
        mov    edi, 3f8h
        push   0
        lea    eax, [esp + 0ch]
        push   esi
        push   eax
        call   sub_8_1EF0
        add    esp, 0ch
        jmp    SHORT loc_8_4DAC
loc_8_4D9D:
        lea    eax, [esp + 8]
        push   0
        push   eax
        call   sub_8_1EA3
        add    esp, 8
loc_8_4DAC:
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 0c8h
        ret
sub_8_4DB7:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi], 47455243h
        je     SHORT loc_8_4DC8
        sub    eax, eax
        jmp    SHORT loc_8_4E21
loc_8_4DC8:
        cmp    dword ptr [esi + 4], 10000h
        je     SHORT loc_8_4DD5
        sub    eax, eax
        jmp    SHORT loc_8_4E21
loc_8_4DD5:
        db  066h, 083h, 03Dh, 0D0h, 003h, 000h, 000h, 002h
        jbe    SHORT loc_8_4E18
        mov    eax, dword ptr [esi + 12h]
        test   al, 4
        je     SHORT loc_8_4DF9
        push   20h
        push   esi
        call   sub_8_1FCA
        add    esp, 8
        or     eax, eax
        je     SHORT loc_8_4E1C
        sub    eax, eax
        jmp    SHORT loc_8_4E21
loc_8_4DF9:
        or     eax, 4
        mov    dword ptr [esi + 12h], eax
        mov    dword ptr [esi + 0ch], 0
        push   20h
        push   esi
        call   sub_8_1FCA
        add    esp, 8
        neg    eax
        mov    dword ptr [esi + 0ch], eax
        jmp    SHORT loc_8_4E1C
loc_8_4E18:
        and    dword ptr [esi + 12h], 0fffffffbh
loc_8_4E1C:
        mov    eax, 1
loc_8_4E21:
        pop    esi
        ret
sub_8_4E23:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi], 4e4b4752h
        je     SHORT loc_8_4E34
        sub    eax, eax
        jmp    SHORT loc_8_4E82
loc_8_4E34:
        db  066h, 083h, 03Dh, 0D0h, 003h, 000h, 000h, 002h
        jbe    SHORT loc_8_4E79
        mov    eax, dword ptr [esi + 10h]
        test   al, 4
        je     SHORT loc_8_4E59
        push   dword ptr [esi + 4]
        push   esi
        call   sub_8_1FCA
        add    esp, 8
        or     eax, eax
loc_8_4E53:
        je     SHORT loc_8_4E7D
        sub    eax, eax
loc_8_4E57:
        jmp    SHORT loc_8_4E82
loc_8_4E59:
        or     eax, 4
        mov    dword ptr [esi + 10h], eax
        mov    dword ptr [esi + 14h], 0
        push   dword ptr [esi + 4]
        push   esi
        call   sub_8_1FCA
        add    esp, 8
        neg    eax
        mov    dword ptr [esi + 14h], eax
        jmp    SHORT loc_8_4E7D
loc_8_4E79:
        and    dword ptr [esi + 10h], 0fffffffbh
loc_8_4E7D:
        mov    eax, 1
loc_8_4E82:
        pop    esi
        ret
sub_8_4E84:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi], 42444752h
        je     SHORT loc_8_4E95
        sub    eax, eax
        jmp    SHORT loc_8_4EE3
loc_8_4E95:
        db  066h, 083h, 03Dh, 0D0h, 003h, 000h, 000h, 002h
        jbe    SHORT loc_8_4EDA
        mov    eax, dword ptr [esi + 0ch]
        test   al, 4
loc_8_4EA4:
        je     SHORT loc_8_4EBA
        push   dword ptr [esi + 4]
        push   esi
        call   sub_8_1FCA
        add    esp, 8
        or     eax, eax
        je     SHORT loc_8_4EDE
        sub    eax, eax
        jmp    SHORT loc_8_4EE3
loc_8_4EBA:
        or     eax, 4
        mov    dword ptr [esi + 0ch], eax
        mov    dword ptr [esi + 1ch], 0
        push   dword ptr [esi + 4]
        push   esi
        call   sub_8_1FCA
        add    esp, 8
loc_8_4ED3:
        neg    eax
        mov    dword ptr [esi + 1ch], eax
        jmp    SHORT loc_8_4EDE
loc_8_4EDA:
        and    dword ptr [esi + 0ch], 0fffffffbh
loc_8_4EDE:
        mov    eax, 1
loc_8_4EE3:
        pop    esi
        ret
sub_8_4EE5:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, 0ffffffffh
loc_8_4EF3:
        mov    edi, esi
        mov    dword ptr [ebp - 4], 0
        xor    edx, edx
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   eax
        push   eax
        push   eax
        mov    eax, 1
        push   eax
        mov    eax, 20h
        push   eax
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        mov    dword ptr [ebp - 4], eax
        add    esp, 20h
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_8_4FCF
        lea    eax, [ebp - 8]
        push   eax
        push   0
        push   0
        push   dword ptr [ebp + 8]
        call   sub_8_1C7C
        add    esp, 10h
        mov    esi, eax
        cmp    esi, -1
        je     NEAR PTR loc_8_4FCF
        push   0
loc_8_4F48:
        push   dword ptr [ebp + 0ch]
        call   sub_8_1E66
        add    esp, 8
        mov    edi, eax
        cmp    edi, -1
        je     SHORT loc_8_4FCF
loc_8_4F5A:
        push   20000h
        push   dword ptr [ebp - 4]
        push   esi
        call   sub_8_1D70
        add    esp, 0ch
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_8_4F96
        cmp    ebx, -1
        je     SHORT loc_8_4FCF
        call   sub_8_7B50
        push   0
        push   ebx
        push   dword ptr [ebp - 4]
        push   edi
        call   sub_8_1DF0
        add    esp, 10h
        cmp    eax, -1
        je     SHORT loc_8_4FCF
        call   sub_8_7B50
        jmp    SHORT loc_8_4F5A
loc_8_4F96:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_8_4FAB
        xor    eax, eax
        push   eax
        push   dword ptr [ebp - 4]
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
loc_8_4FAB:
        push   0
        push   0
        push   esi
        push   dword ptr [ebp + 8]
        call   sub_8_1CFB
        add    esp, 10h
        push   1
        push   0
        push   edi
        push   dword ptr [ebp + 0ch]
        call   sub_8_1CFB
        add    esp, 10h
        sub    eax, eax
        jmp    SHORT loc_8_500B
loc_8_4FCF:
        cmp    esi, -1
        je     SHORT loc_8_4FE4
        push   0
        push   0
        push   esi
        push   dword ptr [ebp + 8]
        call   sub_8_1CFB
        add    esp, 10h
loc_8_4FE4:
        cmp    edi, -1
        je     SHORT loc_8_5006
        push   1
        push   0
        push   edi
        push   dword ptr [ebp + 0ch]
        call   sub_8_1CFB
        add    esp, 10h
        push   0
        push   dword ptr [ebp + 0ch]
        call   sub_8_1EA3
        add    esp, 8
loc_8_5006:
        mov    eax, 3f8h
loc_8_500B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_8_5012:
        sub    esp, 208h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    bx, 30h
        db  066h, 083h, 03Dh, 0C8h, 003h, 000h, 000h, 000h
        je     SHORT loc_8_5034
        mov    eax, 0a7h
        jmp    NEAR PTR loc_8_5171
loc_8_5034:
        mov    edi, dword ptr [esp + 21ch]
        mov    eax, dword ptr [edi + 14h]
        cmp    eax, -1
        jne    SHORT loc_8_504D
        mov    eax, 3f8h
        jmp    NEAR PTR loc_8_5171
loc_8_504D:
        push   eax
        call   sub_8_3651
        inc    ax
        add    esp, 4
        movzx  esi, ax
        push   esi
        lea    eax, [esp + 118h]
        push   dword ptr [edi + 14h]
        push   eax
        call   sub_8_366A
        add    esp, 0ch
        lea    ebp, [esp + esi + 112h]
        cmp    byte ptr [ebp], 40h
        jg     SHORT loc_8_5080
loc_8_507C:
        mov    bx, 41h
loc_8_5080:
        lea    eax, [esp + 114h]
        push   esi
        push   eax
        mov    byte ptr [ebp], bl
        lea    eax, [esp + 18h]
        push   eax
        call   sub_8_366A
        add    esp, 0ch
        lea    esi, [esp + esi + 0eh]
        lea    eax, [ebx + 1]
        mov    byte ptr [esi], al
        lea    eax, [esp + 10h]
        push   0
        push   eax
        call   sub_8_1F76
        add    esp, 8
        or     eax, eax
        je     SHORT loc_8_50C4
        lea    eax, [esp + 10h]
        push   0
        push   eax
        call   sub_8_1EA3
        add    esp, 8
loc_8_50C4:
        lea    eax, [esp + 114h]
        push   0
        push   eax
        call   sub_8_1F76
        add    esp, 8
        or     eax, eax
        je     SHORT loc_8_50F1
        lea    eax, [esp + 10h]
loc_8_50DE:
        push   0
loc_8_50E0:
        push   eax
        lea    eax, [esp + 11ch]
        push   eax
        call   sub_8_1EF0
        add    esp, 0ch
loc_8_50F1:
        lea    eax, [esp + 114h]
        push   eax
        push   dword ptr [edi + 14h]
        call   sub_8_4EE5
        add    esp, 8
        or     eax, eax
        je     SHORT loc_8_5148
        or     dword ptr [edi + 10h], 80h
        lea    eax, [ebx + 1]
        mov    byte ptr [esi], al
        mov    byte ptr [ebp], bl
        lea    eax, [esp + 10h]
        push   0
        push   eax
        call   sub_8_1F76
        add    esp, 8
        or     eax, eax
        je     SHORT loc_8_5141
        lea    eax, [esp + 114h]
        push   0
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        call   sub_8_1EF0
        add    esp, 0ch
loc_8_5141:
        mov    eax, 3f8h
        jmp    SHORT loc_8_5171
loc_8_5148:
        lea    eax, [ebx + 1]
        mov    byte ptr [esi], al
        lea    eax, [esp + 10h]
        push   0
        push   eax
        call   sub_8_1F76
        add    esp, 8
        or     eax, eax
loc_8_515E:
        je     SHORT loc_8_516F
        lea    eax, [esp + 10h]
        push   0
        push   eax
loc_8_5167:
        call   sub_8_1EA3
        add    esp, 8
loc_8_516F:
        sub    eax, eax
loc_8_5171:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 208h
        ret
        db  081h, 0ECh, 008h, 002h, 000h, 000h, 053h, 056h, 057h, 066h, 0BBh, 030h, 000h, 066h, 083h, 03Dh
        db  0C8h, 003h, 000h, 000h, 000h, 074h, 00Ah, 0B8h, 0A7h, 000h, 000h, 000h, 0E9h, 0E1h, 000h, 000h
        db  000h, 08Bh, 0B4h
loc_8_519F:
        and    al, 18h
        add    al, byte ptr [eax]
        add    bh, bh
        db  076h, 014h
        call   sub_8_3651
        inc    ax
        add    esp, 4
        movzx  edi, ax
        push   edi
        lea    eax, [esp + 114h]
        push   dword ptr [esi + 14h]
        push   eax
        call   sub_8_366A
        add    esp, 0ch
        lea    edi, [esp + edi + 10eh]
        cmp    byte ptr [edi], 40h
        jg     SHORT loc_8_51D8
loc_8_51D4:
        mov    bx, 41h
loc_8_51D8:
        lea    eax, [esp + 0ch]
        push   4dh
        push   eax
        push   dword ptr [esi + 14h]
        call   sub_8_4CC9
        add    esp, 0ch
        lea    eax, [esp + 0ch]
        push   0
        push   eax
        call   sub_8_1F76
        add    esp, 8
        or     eax, eax
        je     SHORT loc_8_520C
        lea    eax, [esp + 0ch]
        push   0
        push   eax
        call   sub_8_1EA3
        add    esp, 8
loc_8_520C:
        lea    eax, [esp + 0ch]
        push   0
        push   eax
        push   dword ptr [esi + 14h]
        call   sub_8_1EF0
        add    esp, 0ch
        mov    byte ptr [edi], bl
        lea    eax, [esp + 110h]
        push   dword ptr [esi + 14h]
        push   eax
        call   sub_8_4EE5
        add    esp, 8
        or     eax, eax
        je     SHORT loc_8_5264
        or     dword ptr [esi + 10h], 80h
        push   0
        push   dword ptr [esi + 14h]
        call   sub_8_1EA3
        add    esp, 8
        push   0
        lea    eax, [esp + 10h]
        push   dword ptr [esi + 14h]
        push   eax
        call   sub_8_1EF0
        add    esp, 0ch
        mov    eax, 3f8h
        jmp    SHORT loc_8_527E
loc_8_5264:
        lea    eax, [esp + 0ch]
        push   0
        push   eax
        call   sub_8_1EA3
        add    esp, 8
        push   esi
        call   sub_8_7570
        add    esp, 4
        sub    eax, eax
loc_8_527E:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 208h
        ret
_LTEXT ends
        end
