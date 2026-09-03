; nwlink_mcode_02.asm -- NWLINK object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  01Dh, 000h, 000h, 000h
loc_2_0004:
        dec    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        db  000h
loc_2_000E:
        sbb    al, 0
        and    eax, dword ptr [eax]
        and    eax, 32003000h
        add    byte ptr [eax + eax], bh
        push   esp
        add    byte ptr [edi], bh
        lea    eax, [eax]
        test   eax, 0ea00da00h
        add    byte ptr [edx], al
        add    dword ptr [eax + 1], ebp
        db  078h, 001h
        xchg   byte ptr [ecx], al
        popfd
        add    dword ptr [ebp - 22fe3affh], ebp
        add    ebx, ebp
        add    dword ptr [edi], eax
        add    ah, byte ptr [ebx]
        add    ch, byte ptr [edi]
        add    al, byte ptr [ebp + 2]
        jo     SHORT loc_2_0044
        jnp    SHORT loc_2_0046
loc_2_0044:
        lea    eax, [edx]
loc_2_0046:
        mov    ah, 2
loc_2_0048:
        dec    ecx
        push   eax
        pop    eax
        das
        push   ebx
        push   eax
        pop    eax
        and    byte ptr [ebx + 6fh], ah
        insd
        jo     SHORT loc_2_00B6
        je     SHORT loc_2_00C0
        bound  ebp, qword ptr [ebp + 20h]
        db  070h, 072h
        outsd
        db  074h, 06Fh
        arpl   word ptr [edi + 6ch], bp
        add    byte ptr [esi + 77h], cl
        insb
        imul   ebp, dword ptr [esi + 6bh], 70002000h
        db  065h, 072h, 020h
        jae    SHORT loc_2_00D8
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr fs:[eax], ah
        add    byte ptr [ecx + 50h], cl
        pop    eax
        pop    edi
        push   ebx
        outsb
        jae    NEAR PTR loc_2_0084
loc_2_0084:
        dec    ecx
        push   eax
        pop    eax
        and    byte ptr [eax + 61h], dh
        arpl   word ptr [ebx + 65h], bp
        je     SHORT loc_2_0102
        and    byte ptr [ebx + 65h], dh
        outsb
        db  074h, 02Fh
        jae    SHORT loc_2_00FC
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr fs:[ecx + 50h], cl
        pop    eax
        and    byte ptr [eax + 61h], dh
        arpl   word ptr [ebx + 65h], bp
        db  074h, 073h
        and    byte ptr [ebx + 65h], dh
        outsb
        je     SHORT loc_2_00CD
        outsd
        outsb
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [esi + 65h], ch
loc_2_00B6:
        je     SHORT loc_2_012F
        outsd
        jb     SHORT loc_2_0126
        and    byte ptr [eax + 65h], dh
        jb     SHORT loc_2_00E0
loc_2_00C0:
        jae    SHORT loc_2_0127
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr fs:[ecx + 50h], cl
        pop    eax
        pop    edi
        push   eax
        popad
loc_2_00CD:
        arpl   word ptr [ebx + 65h], bp
        je     SHORT loc_2_0145
        dec    ecx
        outsb
        add    byte ptr [ecx + 50h], cl
        pop    eax
loc_2_00D8:
        and    byte ptr [eax + 61h], dh
        arpl   word ptr [ebx + 65h], bp
        je     SHORT loc_2_0153
loc_2_00E0:
        and    byte ptr [edx + 65h], dh
        arpl   word ptr [ebp + 69h], sp
        jbe    SHORT loc_2_014D
        das
        db  073h, 065h
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr fs:[ecx + 50h], cl
        pop    eax
        and    byte ptr [eax + 61h], dh
        arpl   word ptr [ebx + 65h], bp
        je     SHORT loc_2_016F
loc_2_00FC:
        and    byte ptr [edx + 65h], dh
        arpl   word ptr [ebp + 69h], sp
loc_2_0102:
        db  076h, 065h
        and    byte ptr fs:[esi + 72h], ah
        outsd
        insd
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [esi + 65h], ch
        db  074h, 077h
        outsd
        db  072h, 06Bh
        and    byte ptr [eax + 65h], dh
        jb     SHORT loc_2_013B
        db  073h, 065h
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr fs:[ecx + 50h], cl
        pop    eax
        pop    edi
loc_2_0126:
        push   eax
loc_2_0127:
        popad
        arpl   word ptr [ebx + 65h], bp
        je     SHORT loc_2_01A0
        dec    esp
        outsd
loc_2_012F:
        db  073h, 074h
        add    byte ptr [ecx + 50h], cl
        pop    eax
        and    byte ptr [eax + 61h], dh
        arpl   word ptr [ebx + 65h], bp
loc_2_013B:
        db  074h, 073h
        and    byte ptr [edi + ebp*2 + 73h], ch
        db  074h, 02Fh
        jae    SHORT loc_2_01AA
loc_2_0145:
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr fs:[ecx + 50h], cl
        pop    eax
loc_2_014D:
        and    byte ptr [eax + 61h], dh
        arpl   word ptr [ebx + 65h], bp
loc_2_0153:
        db  074h, 073h
        and    byte ptr [edx + 65h], dh
        arpl   word ptr [ebp + 69h], sp
        jbe    SHORT loc_2_01C2
        and    byte ptr fs:[esi + 72h], ah
        outsd
        insd
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [esi + 65h], ch
        db  074h, 077h
        outsd
        jb     SHORT loc_2_01DA
loc_2_016F:
        and    byte ptr [eax + ebp*2 + 61h], dh
        db  074h, 020h
        ja     SHORT loc_2_01DC
        db  072h, 065h
        and    byte ptr [edx + esi*2 + 6fh], ah
        jo     SHORT loc_2_01EF
        and    byte ptr fs:[edx + 65h], ah
        arpl   word ptr [ecx + 75h], sp
        jae    SHORT loc_2_01EE
        and    byte ptr [esi + 6fh], ch
        bound  ebp, qword ptr [edi + 64h]
        db  079h, 020h
        outsd
        outsb
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [edi + ebp*2 + 63h], ch
        popad
        insb
        and    byte ptr [ebx + 79h], dh
loc_2_01A0:
        jae    SHORT loc_2_0216
        insd
        and    byte ptr [edi + 61h], dh
        outsb
        db  074h, 073h
loc_2_01AA:
        and    byte ptr [eax + ebp*2 + 65h], dh
        insd
        add    byte ptr [ecx + 50h], cl
        pop    eax
        pop    edi
        push   ebx
        outsd
        arpl   word ptr [ebx + 65h], bp
        je     SHORT loc_2_022E
        dec    edi
        jo     SHORT loc_2_0223
        outsb
        add    byte ptr [edi + 70h], cl
loc_2_01C2:
        outsb
        and    byte ptr [ebx + 6fh], dh
        arpl   word ptr [ebx + 65h], bp
        db  074h, 073h
        and    byte ptr [eax], al
        dec    esi
        jne    SHORT loc_2_023E
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [edi + 66h], ch
        and    byte ptr [edi + 70h], ch
loc_2_01DA:
        outsb
loc_2_01DC:
        and    byte ptr [ebx + 6fh], dh
        arpl   word ptr [ebx + 65h], bp
        db  074h, 073h
        add    byte ptr [ebx + 50h], dl
        pop    eax
        pop    edi
        push   ebx
        outsb
        push   eax
loc_2_01EE:
        popad
loc_2_01EF:
        arpl   word ptr [ebx + 65h], bp
        db  074h, 073h
        add    byte ptr [ebx + 50h], dl
        pop    eax
        and    byte ptr [eax + 61h], dh
        arpl   word ptr [ebx + 65h], bp
        je     SHORT loc_2_0273
        and    byte ptr [ebx + 65h], dh
        outsb
        je     SHORT loc_2_0235
        jae    SHORT loc_2_026D
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr fs:[ebx + 50h], dl
        pop    eax
        and    byte ptr [eax + 61h], dh
        arpl   word ptr [ebx + 65h], bp
loc_2_0216:
        db  074h, 073h
        and    byte ptr [ebx + 65h], dh
        outsb
        je     SHORT loc_2_024D
        jae    SHORT loc_2_0285
        arpl   word ptr [edi + 6eh], bp
loc_2_0223:
        add    byte ptr fs:[ebx + 50h], dl
        pop    eax
        pop    edi
        push   eax
        popad
        arpl   word ptr [ebx + 65h], bp
loc_2_022E:
        db  074h, 073h
        dec    ecx
        outsb
        add    byte ptr [ebx + 50h], dl
loc_2_0235:
        pop    eax
        and    byte ptr [eax + 61h], dh
        arpl   word ptr [ebx + 65h], bp
        db  074h, 073h
loc_2_023E:
        and    byte ptr [edx + 65h], dh
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        das
        db  073h, 065h
        arpl   word ptr [edi + 6eh], bp
loc_2_024D:
        add    byte ptr fs:[ebx + 50h], dl
        pop    eax
        and    byte ptr [eax + 61h], dh
        arpl   word ptr [ebx + 65h], bp
        db  074h, 073h
        and    byte ptr [edx + 65h], dh
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        das
        jae    SHORT loc_2_02CB
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr fs:[edx + 49h], dl
loc_2_026D:
        push   esp
        pop    edi
        inc    ebp
        outsb
        db  074h, 072h
loc_2_0273:
        imul   esp, dword ptr [ebp + 73h], 756f5200h
        db  074h, 069h
        outsb
        and    byte ptr [si + 61h], dl
        bound  ebp, qword ptr [ebp + 20h]
loc_2_0285:
        outsb
        db  074h, 072h
        imul   esp, dword ptr [ebp + 73h], 6d754e00h
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [edi + 66h], ch
        and    byte ptr [ecx + 50h], cl
        pop    eax
        and    byte ptr [ecx + 6eh], cl
        db  074h, 065h
        db  072h, 06Eh
        db  065h, 074h, 077h
        outsd
        db  072h, 06Bh
        imul   ebp, dword ptr [esi + 67h], 756f7220h
        db  074h, 065h
        db  073h, 020h
        imul   ebp, dword ptr [esi + 6fh], 77h
        outsb
        add    byte ptr [ebx + 49h], dl
        push   esp
        inc    ebp
        outsb
        db  074h, 072h
        imul   esp, dword ptr [ebp + 73h], 50415300h
        and    byte ptr [ecx + 62h], dl
        insb
loc_2_02CB:
        and    byte ptr gs:[ebp + 6eh], ah
        db  074h, 072h
        imul   esp, dword ptr [ebp + 73h], 6d754e00h
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [edi + 66h], ch
        and    byte ptr [ebx + 65h], dh
        db  072h, 076h
        imul   esp, dword ptr [ebx + 65h], 76646120h
        db  065h, 072h, 074h
        imul   esi, dword ptr [ebx + 65h], 746e656dh
        db  073h, 020h
        imul   ebp, dword ptr [esi + 6fh], 77h
        outsb
        db  000h
_LMSGTABLE ends
        end
