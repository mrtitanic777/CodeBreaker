; reboot_mcode_02.asm -- REBOOT object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  005h, 000h, 000h, 000h
loc_2_0004:
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_000E:
        xor    al, 0
        db  03Eh, 000h
loc_2_0012:
        db  079h, 001h
        mov    al, byte ptr [ecx]
        rol    dword ptr [edx], 1
loc_2_0018:
        push   eax
        jb     SHORT loc_2_0080
        db  073h, 073h
        and    byte ptr [ebx + 54h], al
        push   edx
        dec    esp
        sub    eax, dword ptr [ecx + 4ch]
        push   esp
        sub    eax, dword ptr [ebp + eax*2 + 4ch]
        and    byte ptr [ecx + 67h], ah
        popad
        imul   ebp, dword ptr [esi + 20h], 72206f74h
        jae    NEAR PTR loc_2_00AC
        popad
        db  072h, 074h
        and    byte ptr [ecx + 6fh], bh
        db  075h, 072h
        and    byte ptr [ebx + 6fh], ah
        insd
        db  070h, 075h
        db  074h, 065h
        db  072h, 020h
        add    byte ptr [eax], al
        push   edi
        inc    ecx
        push   edx
        dec    esi
        dec    ecx
        dec    esi
        inc    edi
        and    dword ptr [eax], eax
        add    byte ptr [eax + ebp*2 + 65h], dl
        and    byte ptr [ebx + 79h], dh
        db  073h, 074h
        insd
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [ebp + 69h], ah
        db  074h, 068h
        db  065h, 072h, 020h
        bound  esi, qword ptr [ebp + 73h]
        db  079h, 020h
        outsd
        db  072h, 020h
        push   62207361h
        arpl   word ptr gs:[edi + 6dh], bp
        and    byte ptr gs:[ebp + 6eh], dh
loc_2_0080:
        jae    SHORT loc_2_00F6
        popad
        bound  ebp, qword ptr [ebp + 2eh]
        and    byte ptr [ecx + 6fh], bl
        jne    SHORT loc_2_00AC
        arpl   word ptr [ecx + 6eh], sp
        and    byte ptr [edi + 61h], dh
        imul   esi, dword ptr [eax + 61h], 7320646eh
        and    byte ptr gs:[ecx + 66h], ch
        and    byte ptr [ecx + 74h], ch
        and    byte ptr [edx + 65h], ah
        arpl   word ptr [edi + 6dh], bp
        jae    NEAR PTR loc_2_00CB
        popad
loc_2_00AC:
        db  076h, 061h
        imul   ebp, dword ptr [ecx + 62h], 6120656ch
        popad
        imul   ebp, dword ptr [esi + 2ch], 20726f20h
        db  079h, 06Fh
        db  075h, 020h
        arpl   word ptr [ecx + 6eh], sp
        and    byte ptr [edx + 65h], dh
        jae    SHORT loc_2_013F
loc_2_00CB:
        popad
        jb     SHORT loc_2_0142
        and    byte ptr [ecx + 6fh], bh
        db  075h, 072h
        and    byte ptr [ebx + 6fh], ah
        insd
        jo     SHORT loc_2_014E
        je     SHORT loc_2_0140
        db  072h, 02Eh
        or     eax, 202a0a0ah
        and    byte ptr [eax + 72h], dl
        db  065h, 073h, 073h
        and    byte ptr [ecx + 6eh], ah
        db  079h, 020h
        imul   esp, dword ptr [ebp + 79h], 20h
        db  074h, 06Fh
        and    byte ptr [edx + 65h], dh
loc_2_00F6:
        je     SHORT loc_2_016D
        jb     SHORT loc_2_0168
        and    byte ptr [edi + ebp*2 + 20h], dh
        push   edi
        imul   ebp, dword ptr [esi + 64h], 2073776fh
        popad
        outsb
        and    byte ptr fs:[edi + 61h], dh
        imul   esi, dword ptr [esi + ebp + 0dh], 20202a0ah
        push   eax
        jb     SHORT loc_2_017C
        db  073h, 073h
        and    byte ptr [ebx + 54h], al
        push   edx
        dec    esp
        sub    eax, dword ptr [ecx + 4ch]
        push   esp
        sub    eax, dword ptr [ebp + eax*2 + 4ch]
        and    byte ptr [ecx + 67h], ah
        popad
        imul   ebp, dword ptr [esi + 20h], 72206f74h
        jae    NEAR PTR loc_2_01A8
        popad
        db  072h, 074h
        and    byte ptr [ecx + 6fh], bh
        db  075h, 072h
        and    byte ptr [ebx + 6fh], ah
loc_2_013F:
        insd
loc_2_0140:
        jo     SHORT loc_2_01B7
loc_2_0142:
        db  074h, 065h
        db  072h, 02Eh
        and    byte ptr [ecx + 6fh], bl
        jne    SHORT loc_2_016B
        db  077h, 069h
        insb
loc_2_014E:
        insb
        or     eax, 2020080ah
        and    byte ptr [edi + ebp*2 + 73h], ch
        and    byte ptr gs:[ebp + 6eh], dh
        jae    SHORT loc_2_01BF
        db  076h, 065h
        and    byte ptr fs:[ecx + 6eh], ch
        outsw
        db  072h, 06Dh
loc_2_0168:
        popad
        db  074h, 069h
loc_2_016B:
        outsd
        outsb
loc_2_016D:
        and    byte ptr [ecx + 6eh], ch
        and    byte ptr [ecx + 6eh], ah
        db  079h, 020h
        jo     SHORT loc_2_01E9
        outsd
        jb     NEAR PTR loc_2_01DC
        insd
loc_2_017C:
        db  073h, 020h
        db  074h, 068h
        popad
        je     SHORT loc_2_01A3
        popad
        db  072h, 065h
        and    byte ptr [edx + 75h], dh
        outsb
        outsb
        imul   ebp, dword ptr [esi + 67h], 5300002eh
        jns    SHORT loc_2_0207
        db  074h, 065h
        insd
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [edx + 75h], ah
        db  073h, 079h
        add    byte ptr cs:[eax], al
        push   esp
loc_2_01A3:
        push   79732065h
loc_2_01A8:
        jae    SHORT loc_2_021E
        insd
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [edx + 75h], ah
        db  073h, 079h
        and    byte ptr [edi + 61h], dh
loc_2_01B7:
        imul   esi, dword ptr [ecx + ebp*2 + 6eh], 6f662067h
loc_2_01BF:
        db  072h, 020h
        je     SHORT loc_2_022B
        and    byte ptr gs:[ebx + 6ch], al
        outsd
        jae    SHORT loc_2_022F
        and    byte ptr [eax + 72h], dl
        outsd
        db  067h, 072h, 061h
        insd
        and    byte ptr [ecx + ebp*2 + 61h], ah
        insb
        outsd
        and    byte ptr [bp + si + 6fh], ah
loc_2_01DC:
        db  078h, 020h
        db  074h, 06Fh
        and    byte ptr [edx + 65h], ah
        and    byte ptr [ecx + ebp*2 + 73h], ah
        db  070h, 06Ch
loc_2_01E9:
        popad
        db  079h, 065h
        and    byte ptr cs:[eax], ah
        pop    ecx
        outsd
        jne    SHORT loc_2_0214
        arpl   word ptr [ecx + 6eh], sp
        and    byte ptr [edi + 61h], dh
        imul   esi, dword ptr [eax + 61h], 7320646eh
        and    byte ptr gs:[ecx + 66h], ch
loc_2_0207:
        and    byte ptr [ecx + 74h], ch
        and    byte ptr [ecx + 70h], ah
        jo     SHORT loc_2_0274
        popad
        db  072h, 073h
        sub    al, 20h
loc_2_0214:
        outsd
        db  072h, 020h
        db  079h, 06Fh
        db  075h, 020h
        arpl   word ptr [ecx + 6eh], sp
loc_2_021E:
        and    byte ptr [edx + 65h], dh
        jae    SHORT loc_2_0297
        popad
        jb     SHORT loc_2_029A
        and    byte ptr [ecx + 6fh], bh
        db  075h, 072h
loc_2_022B:
        and    byte ptr [ebx + 6fh], ah
        insd
loc_2_022F:
        jo     SHORT loc_2_02A6
        je     SHORT loc_2_0298
        db  072h, 02Eh
        or     eax, 202a0a0ah
        and    byte ptr [eax + 72h], dl
        db  065h, 073h, 073h
        and    byte ptr [ecx + 6eh], ah
        db  079h, 020h
        imul   esp, dword ptr [ebp + 79h], 20h
        db  074h, 06Fh
        and    byte ptr [edx + 65h], dh
        je     SHORT loc_2_02C5
        jb     SHORT loc_2_02C0
        and    byte ptr [edi + ebp*2 + 20h], dh
        push   edi
        imul   ebp, dword ptr [esi + 64h], 2073776fh
        popad
        outsb
        and    byte ptr fs:[edi + 61h], dh
        imul   esi, dword ptr [esi + ebp + 0dh], 20202a0ah
        push   eax
        jb     SHORT loc_2_02D4
        jae    SHORT loc_2_02E4
        and    byte ptr [ebx + 54h], al
loc_2_0274:
        push   edx
        dec    esp
        sub    eax, dword ptr [ecx + 4ch]
        push   esp
        sub    eax, dword ptr [ebp + eax*2 + 4ch]
        and    byte ptr [ecx + 67h], ah
        popad
        imul   ebp, dword ptr [esi + 20h], 72206f74h
        db  065h, 073h, 074h
        popad
        db  072h, 074h
        and    byte ptr [ecx + 6fh], bh
        db  075h, 072h
        and    byte ptr [ebx + 6fh], ah
loc_2_0297:
        insd
loc_2_0298:
        db  070h, 075h
loc_2_029A:
        db  074h, 065h
        jb     SHORT loc_2_02CC
        and    byte ptr [ecx + 6fh], bl
        jne    SHORT loc_2_02C3
        db  077h, 069h
        insb
loc_2_02A6:
        insb
        or     eax, 2020080ah
        and    byte ptr [edi + ebp*2 + 73h], ch
        and    byte ptr gs:[ebp + 6eh], dh
        db  073h, 061h
        db  076h, 065h
        and    byte ptr fs:[ecx + 6eh], ch
        outsw
        db  072h, 06Dh
loc_2_02C0:
        popad
        db  074h, 069h
loc_2_02C3:
        outsd
        outsb
loc_2_02C5:
        and    byte ptr [ecx + 6eh], ch
        and    byte ptr [ecx + 6ch], ah
        insb
loc_2_02CC:
        and    byte ptr [eax + 72h], dh
        outsd
        db  067h, 072h, 061h
        insd
loc_2_02D4:
        db  073h, 020h
        db  074h, 068h
        popad
        db  074h, 020h
        popad
        db  072h, 065h
        and    byte ptr [edx + 75h], dh
        outsb
        outsb
        db  069h
loc_2_02E4:
        outsb
        add    byte ptr cs:[bx + si], al
_LMSGTABLE ends
        end
