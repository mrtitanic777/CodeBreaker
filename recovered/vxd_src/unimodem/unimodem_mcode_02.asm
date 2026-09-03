; unimodem_mcode_02.asm -- UNIMODEM object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  065h, 000h, 000h, 000h
loc_2_0004:
        fadd   dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_2_000C:
        add    byte ptr [eax], al
loc_2_000E:
        or     al, byte ptr [eax]
loc_2_0010:
        pop    ds
        add    byte ptr [eax + eax], dh
loc_2_0014:
        dec    ecx
        db  000h
loc_2_0016:
        pushad
        db  000h
loc_2_0018:
        jnp    SHORT loc_2_001A
loc_2_001A:
        xchg   esi, eax
        db  000h
loc_2_001C:
        scasd
        db  000h
loc_2_001E:
        retf
        db  000h
loc_2_0020:
        in     eax, 0
loc_2_0022:
        sti
        add    byte ptr [ebx], dl
        db  001h
loc_2_0026:
        sub    al, 1
loc_2_0028:
        dec    esp
        db  001h
loc_2_002A:
        pop    esi
        add    dword ptr [ebx - 43fe5affh], eax
        add    esi, eax
        add    eax, edx
        add    esi, ebx
        add    eax, edi
        add    dword ptr [eax], eax
        add    al, byte ptr [ebx]
        add    cl, byte ptr [edi]
        add    dl, byte ptr [edi]
        add    bl, byte ptr [34022802h]
        add    bh, byte ptr [ecx]
        add    al, byte ptr [ebx + 2]
        dec    eax
        add    cl, byte ptr [edx + eax + 50h]
        add    dl, byte ptr [edx + eax + 58h]
        add    bl, byte ptr [edx + eax + 60h]
        add    ah, byte ptr [edx + eax + 68h]
        add    ch, byte ptr [ebp + 2]
        jb     SHORT loc_2_0064
        ja     SHORT loc_2_0066
loc_2_0064:
        db  07Ch, 002h
loc_2_0066:
        add    dword ptr [edx], 28b0286h
        sahf
        add    dh, byte ptr [ecx - 22fd3bfeh]
        add    bh, bh
        add    bl, byte ptr [esi]
        add    edi, dword ptr [edx]
        add    ecx, dword ptr [ebp + 3]
loc_2_007C:
        arpl   word ptr [ebx], ax
loc_2_007E:
        test   dword ptr [ebx], eax
loc_2_0080:
        movsd
        add    esi, dword ptr [ebx + eax + 3e303bdh]
loc_2_0088:
        test   byte ptr [ebx], 2
        add    al, 8
        add    al, 1dh
        add    al, 3eh
        add    al, 5fh
        add    al, 7ah
        add    al, 95h
        add    al, 0b0h
        add    al, 0cbh
        add    al, 0e6h
        add    al, 1
        add    eax, 53f052ah
loc_2_00A4:
        db  071h, 005h
loc_2_00A6:
        xchg   byte ptr [5b5059eh], al
loc_2_00AC:
        int3
        db  005h
loc_2_00AE:
        cli
        db  005h
loc_2_00B0:
        xor    byte ptr [esi], al
loc_2_00B2:
        dec    ebp
        push   es
loc_2_00B4:
        outsb
        push   es
loc_2_00B6:
        pop    dword ptr [esi]
loc_2_00B8:
        mov    dh, 6
loc_2_00BA:
        fadd   qword ptr [esi]
loc_2_00BC:
        inc    dword ptr [esi]
loc_2_00BE:
        sbb    dword ptr [edi], eax
loc_2_00C0:
        xor    al, 7
        pop    ebx
        pop    es
loc_2_00C4:
        db  079h, 007h
loc_2_00C6:
        cmpsd
        pop    es
loc_2_00C8:
        rol    dword ptr [edi], 1
loc_2_00CA:
        and    dword ptr [eax], ecx
loc_2_00CC:
        or     byte ptr [eax], 8ch
        or     byte ptr [eax + ecx + 8c708b0h], bl
        ror    dword ptr [eax], cl
loc_2_00D8:
        inc    esp
        imul   esp, dword ptr [ecx + 6ch], 2e676e69h
        or     al, byte ptr [eax]
        push   edi
        popad
        imul   esi, dword ptr [ecx + ebp*2 + 6eh], 6f662067h
        jb     SHORT loc_2_010E
        popad
        and    byte ptr [ebx + 61h], ah
        insb
        insb
        or     al, byte ptr cs:[eax]
        inc    ecx
        outsb
        db  073h, 077h
        db  065h, 072h, 069h
        outsb
        and    byte ptr [si + 68h], dh
        and    byte ptr gs:[ebx + 61h], ah
        insb
        insb
        or     al, byte ptr cs:[eax]
        dec    ecx
        outsb
loc_2_010E:
        imul   esi, dword ptr [ecx + ebp*2 + 61h], 697a696ch
        outsb
        and    byte ptr [di + 6fh], ch
        insd
        or     al, byte ptr cs:[eax]
        dec    eax
        popad
        outsb
        imul   ebp, dword ptr [bp + 67h], 20707520h
        db  074h, 068h
        and    byte ptr gs:[ebp + 6fh], ch
        insd
        or     al, byte ptr cs:[eax]
        inc    ecx
        outsb
        db  073h, 077h
        db  065h, 072h, 069h
        outsb
        and    byte ptr [si + 68h], dh
        and    byte ptr gs:[esi + 6fh], dh
        imul   esp, dword ptr [ebx + 65h], 6c616320h
        insb
        or     al, byte ptr cs:[eax]
        push   ebx
        je     NEAR PTR loc_2_01CC
        db  070h, 020h
        db  074h, 06Fh
        and    byte ptr [ecx + ebp*2 + 61h], ah
        insb
        and    byte ptr [esi + 6fh], dh
        imul   esp, dword ptr [ebx + 65h], 6c616320h
        insb
        or     al, byte ptr cs:[eax]
        dec    ecx
        db  073h, 073h
        db  075h, 065h
        imul   ebp, dword ptr [esi + 67h], 696f7620h
        arpl   word ptr [ebp + 20h], sp
        arpl   word ptr [edi + 6dh], bp
        insd
        popad
        outsb
        or     al, byte ptr cs:[eax]
        push   ebx
        je     SHORT loc_2_01EB
        db  072h, 074h
        imul   ebp, dword ptr [esi + 67h], 76617720h
        and    byte ptr gs:[eax + 6ch], dh
        popad
        db  079h, 02Fh
        jb     SHORT loc_2_0201
        arpl   word ptr [edi + 72h], bp
        or     al, byte ptr cs:[eax]
        inc    ebp
        outsb
        imul   ebp, dword ptr fs:[esi + 67h], 76617720h
        and    byte ptr gs:[eax + 6ch], dh
        popad
        jns    SHORT loc_2_01E3
        db  072h, 065h
        arpl   word ptr [edi + 72h], bp
        or     al, byte ptr cs:[eax]
        push   ebx
        db  065h, 074h, 074h
        imul   ebp, dword ptr [esi + 67h], 76617720h
        and    byte ptr gs:[esi + 6fh], ah
loc_2_01CC:
        jb     SHORT loc_2_023B
        popad
        je     SHORT loc_2_01FF
        or     al, byte ptr [eax]
        push   esi
        outsd
        imul   esp, dword ptr [ebx + 65h], 646f4d20h
        insd
        and    byte ptr [edi + 61h], dl
        db  076h, 065h
loc_2_01E3:
        and    byte ptr [edi + 70h], cl
        outsb
        or     al, byte ptr cs:[eax]
loc_2_01EB:
        push   esi
        outsd
        imul   esp, dword ptr [ebx + 65h], 646f4d20h
        insd
        and    byte ptr [edi + 61h], dl
        db  076h, 065h
        and    byte ptr [ebx + 6ch], al
        outsd
loc_2_01FF:
        jae    SHORT loc_2_0266
loc_2_0201:
        or     al, byte ptr cs:[eax]
        dec    ecx
        jae    SHORT loc_2_027A
        db  075h, 065h
        imul   ebp, dword ptr [esi + 67h], 65705320h
        popad
        imul   esp, dword ptr [ebp + 72h], 70h
        push   20656e6fh
        arpl   word ptr [edi + 6dh], bp
        insd
        popad
        outsb
        or     al, byte ptr cs:[eax]
        inc    edi
        outsb
        db  065h, 072h, 061h
        je     SHORT loc_2_0295
        outsb
        and    byte ptr [si + 54h], al
        dec    ebp
        inc    esi
        or     al, byte ptr cs:[eax]
        push   edx
        arpl   word ptr gs:[ebp + 69h], sp
loc_2_023B:
        db  076h, 065h
        and    byte ptr fs:[esp + ecx*2 + 45h], al
        and    byte ptr [edx + 61h], dl
        db  077h, 03Dh
        and    eax, 54202c64h
        db  072h, 061h
        outsb
        db  073h, 06Ch
        popad
        db  074h, 065h
        cmp    eax, 0a2e6425h
        add    byte ptr [eax + 61h], cl
        db  072h, 064h
        db  077h, 061h
        jb     SHORT loc_2_02C8
        and    byte ptr [eax + 61h], ch
loc_2_0266:
        outsb
        jne    NEAR PTR loc_2_02DA
        and    byte ptr [edx + 79h], ah
        and    byte ptr [edi + ebp*2 + 77h], ch
        jb     NEAR PTR loc_2_02DD
        outsb
        and    byte ptr [si + 54h], al
        push   edx
loc_2_027A:
        or     al, byte ptr cs:[eax]
        push   edx
        insd
        outsd
        db  074h, 065h
        and    byte ptr [ebp + 6fh], ch
        insd
        and    byte ptr [eax + 75h], ch
        outsb
        and    byte ptr [di + 70h], dh
        or     al, byte ptr cs:[eax]
        push   ebx
loc_2_0295:
        outsb
        cmp    ah, byte ptr fs:[eax]
        and    eax, 52000a73h
        arpl   word ptr gs:[esi + 3ah], si
        and    byte ptr [3c000a73h], ah
        outsb
        outsd
        and    byte ptr [edx + 65h], dh
        db  073h, 070h
        outsd
        outsb
        db  073h, 065h
        add    byte ptr ds:[ecx + 6eh], cl
        db  074h, 065h
        db  072h, 070h
        jb     SHORT loc_2_0323
        db  074h, 065h
        and    byte ptr fs:[edx + 65h], dh
        jae    SHORT loc_2_0336
        outsd
        outsb
loc_2_02C8:
        jae    SHORT loc_2_032F
        cmp    ah, byte ptr [eax]
        and    eax, 55000a73h
        outsb
        imul   ebp, dword ptr [esi + 6fh], 77h
        outsb
        add    byte ptr [edi + 6bh], cl
loc_2_02DA:
        add    byte ptr [ecx + 6eh], cl
loc_2_02DD:
        outsw
        jb     SHORT loc_2_034E
        popad
        je     SHORT loc_2_034D
        db  076h, 065h
        add    byte ptr [ebx + 6fh], al
        outsb
        outsb
        arpl   word ptr gs:[eax + eax + 45h], si
        db  072h, 072h
        outsd
        jb     SHORT loc_2_02F5
loc_2_02F5:
        dec    esi
        outsd
        and    byte ptr [ebx + 61h], al
        db  072h, 072h
        imul   esp, dword ptr [ebp + 72h], 206f4e00h
        inc    esp
        imul   esp, dword ptr [ecx + 6ch], 656e6f74h
        add    byte ptr [edx + 75h], al
        jae    SHORT loc_2_0389
        add    byte ptr [esi + 6fh], cl
        and    byte ptr [ecx + 6eh], al
        db  073h, 077h
        jb     NEAR PTR loc_2_031B
loc_2_031B:
        push   edx
        imul   ebp, dword ptr [esi + 67h], 56535300h
loc_2_0323:
        add    byte ptr [ebx + 4dh], dl
        inc    esp
        add    byte ptr [ebx + 46h], dl
        inc    ecx
        add    byte ptr [ebx + 52h], dl
        inc    ecx
loc_2_032F:
        add    byte ptr [ebx + 52h], dl
        push   ecx
        add    byte ptr [ebx + 52h], dl
loc_2_0336:
        inc    ebx
        add    byte ptr [ebx + 54h], dl
        dec    edi
        add    byte ptr [ebx + 56h], dl
        dec    ebp
        add    byte ptr [edx + edx*2 + 4fh], al
        dec    esi
        add    byte ptr [edx + edx*2 + 4fh], al
        inc    esi
        add    byte ptr [ecx + eax*2 + 54h], al
loc_2_034D:
        inc    ebp
loc_2_034E:
        add    byte ptr [ecx + ecx*2 + 4dh], dl
        inc    ebp
        add    byte ptr [esi + 4dh], cl
        inc    edx
        push   edx
        add    byte ptr [esi + 41h], cl
        dec    ebp
        inc    ebp
        add    byte ptr [ebp + 45h], cl
        push   ebx
        inc    edi
        add    byte ptr [ecx + ebp*2 + 73h], al
        db  074h, 069h
        outsb
        arpl   word ptr [ecx + ebp*2 + 76h], si
        and    byte ptr gs:[edx + 69h], dl
        outsb
        and    byte ptr [bx + di], al
        inc    esp
        imul   esi, dword ptr [ebx + 74h], 74636e69h
        imul   esi, dword ptr [esi + 65h], 6e695220h
        and    byte ptr [bp + si], al
loc_2_0389:
        inc    esp
        imul   esi, dword ptr [ebx + 74h], 74636e69h
        imul   esi, dword ptr [esi + 65h], 6e695220h
        and    byte ptr [bp + di], al
        inc    ebx
        outsd
        outsb
        outsb
        arpl   word ptr gs:[ecx + ebp*2 + 6fh], si
        outsb
        and    byte ptr [ebp + 73h], ah
        je     SHORT loc_2_040C
        bound  ebp, qword ptr [ecx + ebp*2 + 73h]
        push   0a2e6465h
        add    byte ptr [ebx + 6fh], al
        outsb
        outsb
        arpl   word ptr gs:[ecx + ebp*2 + 6fh], si
        outsb
        and    byte ptr [ebp + 73h], ah
        je     SHORT loc_2_0425
        bound  ebp, qword ptr [ecx + ebp*2 + 73h]
        push   61206465h
        db  074h, 020h
        and    eax, 73706264h
        or     al, byte ptr cs:[eax]
        inc    ebp
        db  072h, 072h
        outsd
        jb     SHORT loc_2_040A
        arpl   word ptr [edi + 6eh], bp
        db  074h, 072h
        outsd
        insb
        and    byte ptr [edi + 66h], ch
        and    byte ptr [edi + 72h], ch
        and    byte ptr [ebp + 6eh], dh
        imul   ebp, dword ptr [esi + 6fh], 77h
        outsb
        or     al, byte ptr cs:[eax]
        inc    ebx
        insb
        insb
        jne    SHORT loc_2_0468
        popad
        jb     SHORT loc_2_041F
        db  065h, 072h, 072h
        outsd
        jb     SHORT loc_2_0432
        arpl   word ptr [edi + 6eh], bp
        db  074h, 072h
loc_2_040A:
        outsd
        insb
loc_2_040C:
        and    byte ptr [edi + 6eh], ch
        or     al, byte ptr cs:[eax]
        inc    ebp
        db  072h, 072h
        outsd
        jb     SHORT loc_2_0445
        arpl   word ptr [edi + 6eh], bp
        je     SHORT loc_2_048F
        outsd
        insb
loc_2_041F:
        and    byte ptr [edi + 6eh], ch
        or     al, byte ptr cs:[eax]
loc_2_0425:
        inc    esp
        popad
        je     SHORT loc_2_048A
        and    byte ptr [ebx + 6fh], ah
        insd
        db  070h, 072h
        db  065h, 073h, 073h
loc_2_0432:
        imul   ebp, dword ptr [edi + 6eh], 2e6e6f20h
        or     al, byte ptr [eax]
        inc    esp
        popad
        je     SHORT loc_2_04A0
        and    byte ptr [ebx + 6fh], ah
        insd
        db  070h, 072h
loc_2_0445:
        db  065h, 073h, 073h
        imul   ebp, dword ptr [edi + 6eh], 66666f20h
        and    byte ptr [edi + 72h], ch
        and    byte ptr [ebp + 6eh], dh
        imul   ebp, dword ptr [esi + 6fh], 77h
        outsb
        or     al, byte ptr cs:[eax]
        inc    edx
        db  079h, 074h
        jae    NEAR PTR loc_2_0483
        jae    SHORT loc_2_04CA
        outsb
        je     SHORT loc_2_0488
loc_2_0468:
        je     SHORT loc_2_04D9
        and    byte ptr [ebp + 6fh], ch
        insd
        and    byte ptr [eax + 65h], dh
        db  072h, 020h
        jae    SHORT loc_2_04DC
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr cs:[edx + 79h], al
        db  074h, 065h
        db  073h, 020h
loc_2_0483:
        db  073h, 065h
        outsb
        db  074h, 02Fh
loc_2_0488:
        jae    SHORT loc_2_04EF
loc_2_048A:
        arpl   word ptr [eax], ax
        push   ebx
        outsb
loc_2_048F:
        push   edx
        popad
        db  074h, 065h
        add    byte ptr [edx + 79h], al
        je     SHORT loc_2_04FE
        db  073h, 020h
        jb     SHORT loc_2_0502
        arpl   word ptr [ebp + 69h], sp
loc_2_04A0:
        jbe    SHORT loc_2_0507
        and    byte ptr fs:[esi + 72h], ah
        outsd
        insd
        and    byte ptr [ebp + 6fh], ch
        insd
        and    byte ptr [eax + 65h], dh
        jb     SHORT loc_2_04D3
        db  073h, 065h
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr cs:[edx + 79h], al
        db  074h, 065h
        jae    SHORT loc_2_04E1
        jb     SHORT loc_2_0528
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        das
loc_2_04CA:
        db  073h, 065h
        arpl   word ptr [eax], ax
        push   edx
        arpl   word ptr gs:[ebp + 69h], sp
loc_2_04D3:
        db  076h, 065h
        push   edx
        popad
        db  074h, 065h
loc_2_04D9:
        add    byte ptr [edx + 79h], al
loc_2_04DC:
        je     SHORT loc_2_0543
        jae    SHORT loc_2_04E0
loc_2_04E0:
        push   ebx
loc_2_04E1:
        jae    NEAR PTR loc_2_0557
        imul   ebp, dword ptr [edi + 6eh], 61745320h
        db  074h, 069h
        jae    SHORT loc_2_0563
loc_2_04EF:
        imul   esp, dword ptr [ebx + 73h], 20000a3ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_2_04FE:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_2_0502:
        and    byte ptr [eax], ah
        push   edx
        popad
loc_2_0507:
        jae    NEAR PTR loc_2_052A
        cmp    ah, byte ptr [eax]
        and    eax, 79622064h
        je     SHORT loc_2_0578
        jae    SHORT loc_2_051F
        add    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_2_051F:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        push   edi
        jb     SHORT loc_2_0591
loc_2_0528:
        db  074h, 065h
loc_2_052A:
        db  073h, 03Ah
        and    byte ptr [79622064h], ah
        je     SHORT loc_2_0599
        jae    SHORT loc_2_0540
        add    byte ptr [eax], ah
        and    byte ptr [edi + 65h], al
        outsb
        db  065h, 072h, 061h
        insb
loc_2_0540:
        and    byte ptr [ebx + 6fh], al
loc_2_0543:
        insd
        insd
        and    byte ptr [ebp + 72h], al
        db  072h, 06Fh
        jb     SHORT loc_2_05BF
        cmp    ah, byte ptr [eax]
        and    eax, 20000a64h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_2_0557:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        inc    esi
        jb     SHORT loc_2_05BF
        insd
        and    byte ptr gs:[ebp + 72h], al
loc_2_0563:
        db  072h, 06Fh
        jb     SHORT loc_2_05DA
        cmp    ah, byte ptr [eax]
        and    eax, 20000a64h
        and    byte ptr [ecx + 6eh], cl
        db  070h, 075h
        je     SHORT loc_2_05A4
        dec    edi
        db  075h, 074h
loc_2_0578:
        jo     SHORT loc_2_05EF
        db  074h, 020h
        inc    ebp
        jb     SHORT loc_2_05F1
        outsd
        db  072h, 073h
        cmp    ah, byte ptr [eax]
        and    eax, 53000a64h
        db  065h, 072h, 069h
        popad
        insb
        and    byte ptr [edi + 76h], cl
loc_2_0591:
        jb     NEAR PTR loc_2_0606
        jne    SHORT loc_2_0604
        and    byte ptr [ebp + 72h], al
loc_2_0599:
        jb     SHORT loc_2_060A
        db  072h, 073h
        cmp    ah, byte ptr [eax]
        and    eax, 42000a64h
loc_2_05A4:
        db  075h, 066h
        jb     NEAR PTR loc_2_05CA
        dec    edi
        db  076h, 065h
        db  072h, 072h
        jne    SHORT loc_2_061F
        and    byte ptr [ebp + 72h], al
        db  072h, 06Fh
        db  072h, 073h
        cmp    ah, byte ptr [eax]
        and    eax, 20000a64h
loc_2_05BF:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax + 61h], dl
        db  072h, 069h
loc_2_05CA:
        db  074h, 079h
        and    byte ptr [ebp + 72h], al
        db  072h, 06Fh
        db  072h, 073h
        cmp    ah, byte ptr [eax]
        and    eax, 45000a64h
loc_2_05DA:
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        dec    ecx
        outsb
        imul   esi, dword ptr [eax + 73h], 6e697274h
        and    byte ptr [bp + di + 6fh], ah
        outsb
loc_2_05EF:
        jae    SHORT loc_2_0665
loc_2_05F1:
        jb     SHORT loc_2_0668
        arpl   word ptr [ecx + ebp*2 + 6fh], si
        outsb
        and    byte ptr [esi + 61h], ah
        imul   ebp, dword ptr [ebp + 64h], 45000a2eh
        push   edx
loc_2_0604:
        push   edx
        dec    edi
loc_2_0606:
        push   edx
        cmp    ah, byte ptr [eax]
        dec    ecx
loc_2_060A:
        outsb
        imul   esi, dword ptr [eax + 66h], 656c6961h
        or     al, byte ptr cs:[eax]
        inc    ebp
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
loc_2_061F:
        imul   esp, dword ptr [ecx + 6ch], 72747320h
        imul   ebp, dword ptr [esi + 67h], 6e6f6320h
        db  073h, 074h
        jb     SHORT loc_2_06A6
        arpl   word ptr [ecx + ebp*2 + 6fh], si
        outsb
        and    byte ptr [esi + 61h], ah
        imul   ebp, dword ptr [ebp + 64h], 726f6620h
        and    byte ptr [edi], ah
        and    eax, 0a2e2773h
        add    byte ptr [ebp + 52h], al
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        imul   esp, dword ptr [ecx + 6ch], 69616620h
        insb
        or     al, byte ptr cs:[eax]
        inc    ebp
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
loc_2_0665:
        dec    ebp
        outsd
        outsb
loc_2_0668:
        imul   esi, dword ptr [edi + ebp*2 + 72h], 69616620h
        insb
        or     al, byte ptr cs:[eax]
        inc    ebp
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        inc    ecx
        outsb
        jae    SHORT loc_2_06F8
        db  065h, 072h, 020h
        popaw
        imul   ebp, dword ptr [ebp + 64h], 45000a2eh
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        dec    eax
        popad
        outsb
        jne    NEAR PTR loc_2_070A
        and    byte ptr [esi + 61h], ah
        imul   ebp, dword ptr [ebp + 64h], 45000a2eh
        push   edx
loc_2_06A6:
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        push   ebp
        outsb
        popad
        bound  ebp, qword ptr [ebp + 20h]
        db  074h, 06Fh
        and    byte ptr [ebx + 65h], dh
        outsb
        and    byte ptr fs:[ebx + 6fh], ah
        insd
        insd
        popad
        outsb
        and    byte ptr fs:[edi + ebp*2 + 20h], dh
        db  074h, 068h
        and    byte ptr gs:[ebp + 76h], ah
        imul   esp, dword ptr [ebx + 65h], 45000a2eh
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        push   ebp
        outsb
        popad
        bound  ebp, qword ptr [ebp + 20h]
        db  074h, 06Fh
        and    byte ptr [edx + 65h], dh
        arpl   word ptr [edi + 76h], bp
        jb     NEAR PTR loc_2_070B
        jb     NEAR PTR loc_2_075D
        insd
        and    byte ptr [ebp + 6eh], dh
        jb     SHORT loc_2_0759
        arpl   word ptr [edi + 67h], bp
        outsb
loc_2_06F8:
        imul   edi, dword ptr [edx + 65h], 65722064h
        jae    SHORT loc_2_0771
        outsd
        outsb
        jae    SHORT loc_2_076A
        or     al, byte ptr cs:[eax]
        inc    ebp
        push   edx
loc_2_070A:
        push   edx
loc_2_070B:
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        push   esi
        outsd
        imul   esp, dword ptr [ebx + 65h], 736e4120h
        db  077h, 065h
        db  072h, 020h
        popaw
        imul   ebp, dword ptr [ebp + 64h], 45000a2eh
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        push   esi
        outsd
        imul   esp, dword ptr [ebx + 65h], 61696420h
        insb
        and    byte ptr [ebx + 65h], dh
        db  074h, 075h
        jo     SHORT loc_2_075D
        popaw
        imul   ebp, dword ptr [ebp + 64h], 45000a2eh
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        push   esi
        outsd
        imul   esp, dword ptr [ebx + 65h], 6d6f4320h
        insd
        popad
        outsb
loc_2_0759:
        and    byte ptr fs:[esi + 61h], ah
loc_2_075D:
        imul   ebp, dword ptr [ebp + 64h], 7325202eh
        or     al, byte ptr [eax]
        inc    ebp
        push   edx
        push   edx
loc_2_076A:
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        push   ebx
        je     SHORT loc_2_07D2
loc_2_0771:
        db  072h, 074h
        and    byte ptr [edi + 61h], dh
        db  076h, 065h
        and    byte ptr [eax + 6ch], dh
        popad
        jns    SHORT loc_2_07AD
        db  072h, 065h
        arpl   word ptr [edi + 72h], bp
        and    byte ptr fs:[esi + 61h], ah
        imul   ebp, dword ptr [ebp + 64h], 45000a2eh
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        push   ebx
        je     SHORT loc_2_0807
        db  070h, 020h
        db  077h, 061h
        db  076h, 065h
        and    byte ptr [eax + 6ch], dh
        popad
        jns    SHORT loc_2_07D3
        db  072h, 065h
        arpl   word ptr [edi + 72h], bp
        and    byte ptr fs:[esi + 61h], ah
loc_2_07AD:
        imul   ebp, dword ptr [ebp + 64h], 45000a2eh
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esi
        popad
        imul   ebp, dword ptr [ebp + 64h], 206f7420h
        jae    SHORT loc_2_082C
        db  074h, 020h
        ja     SHORT loc_2_082C
        db  076h, 065h
        and    byte ptr [esi + 6fh], ah
        db  072h, 061h
loc_2_07D2:
        insd
loc_2_07D3:
        db  074h, 02Eh
        or     al, byte ptr [eax]
        inc    ebp
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esi
        popad
        imul   ebp, dword ptr [ebp + 64h], 76615720h
        and    byte ptr gs:[edi + 70h], cl
        outsb
        or     al, byte ptr cs:[eax]
        inc    ebp
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esi
        popad
        imul   ebp, dword ptr [ebp + 64h], 76615720h
        and    byte ptr gs:[ebx + 6ch], al
        outsd
loc_2_0807:
        db  073h, 065h
        or     al, byte ptr cs:[eax]
        inc    ebp
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        push   ebx
        db  070h, 065h
        popad
        imul   esp, dword ptr [ebp + 72h], 20h
        push   eax
        push   656e6f61h
        and    byte ptr [ebx + 6fh], al
        insd
        insd
        popad
        outsb
        and    byte ptr fs:[esi + 61h], al
loc_2_082C:
        imul   ebp, dword ptr [ebp + 64h], 45000a2eh
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        inc    edi
        outsb
        jb     NEAR PTR loc_2_08A1
        db  074h, 065h
        and    byte ptr [esp + edx*2 + 4dh], al
        inc    esi
        and    byte ptr [esi + 61h], ah
        imul   ebp, dword ptr [ebp + 64h], 57000a2eh
        inc    ecx
        push   edx
        dec    esi
        dec    ecx
        dec    esi
        inc    edi
        cmp    ah, byte ptr [eax]
        push   ebp
        outsb
        db  072h, 065h
        arpl   word ptr [edi + 67h], bp
        outsb
        imul   edi, dword ptr [edx + 65h], 65722064h
        jae    SHORT loc_2_08DB
        outsd
        outsb
        db  073h, 065h
        and    byte ptr cs:[eax], ah
        push   edx
        je     NEAR PTR loc_2_08E8
        jns    SHORT loc_2_08E1
        outsb
        or     al, byte ptr cs:[bx + si]
        push   edi
        inc    ecx
        push   edx
        dec    esi
        dec    ecx
        dec    esi
        inc    edi
        cmp    ah, byte ptr [eax]
        push   ebp
        outsb
        popad
        bound  ebp, qword ptr [ebp + 20h]
        je     SHORT loc_2_0900
        and    byte ptr [edi + ebp*2 + 61h], ch
        and    byte ptr fs:[eax + ebp*2 + 65h], dh
        and    byte ptr [edi], ah
        and    eax, 73202773h
loc_2_08A1:
        db  074h, 072h
        imul   ebp, dword ptr [esi + 67h], 57000a2eh
        inc    ecx
        push   edx
        dec    esi
        dec    ecx
        dec    esi
        inc    edi
        cmp    ah, byte ptr [eax]
        push   esp
        push   6f6d2065h
        insd
        and    byte ptr [ecx + ebp*2 + 64h], ah
        and    byte ptr [esi + 6fh], ch
        db  074h, 020h
        jb     SHORT loc_2_092B
        jae    SHORT loc_2_0938
        outsd
        outsb
        and    byte ptr fs:[edi + ebp*2 + 20h], dh
        insb
        outsd
        ja     SHORT loc_2_0938
        db  072h, 069h
        outsb
        and    byte ptr [si + 54h], al
        push   edx
loc_2_08DB:
        and    byte ptr cs:[eax], ah
        push   esp
        db  072h, 079h
loc_2_08E1:
        imul   ebp, dword ptr [esi + 67h], 666f7320h
loc_2_08E8:
        db  074h, 077h
        popad
        jb     SHORT loc_2_0952
        and    byte ptr [eax + 61h], ch
        outsb
        jne    NEAR PTR loc_2_0964
        or     al, byte ptr cs:[eax]
        push   edi
        inc    ecx
        push   edx
        dec    esi
        dec    ecx
        dec    esi
        inc    edi
loc_2_0900:
        cmp    ah, byte ptr [eax]
        inc    ecx
        and    byte ptr [eax + 72h], dh
        db  065h, 076h, 069h
        outsd
        db  075h, 073h
        and    byte ptr [eax + 72h], dh
        outsd
        db  067h, 072h, 061h
        insd
        and    byte ptr [ebp + 66h], ch
        db  074h, 020h
        popad
        and    byte ptr [ebx + 6fh], ah
        outsb
        outsb
        arpl   word ptr gs:[ecx + ebp*2 + 6fh], si
        outsb
        and    byte ptr [ebp + 73h], ah
        db  074h, 061h
loc_2_092B:
        bound  ebp, qword ptr [ecx + ebp*2 + 73h]
        push   202e6465h
        and    byte ptr [eax + 61h], cl
        outsb
loc_2_0938:
        imul   ebp, dword ptr [bp + 67h], 20707520h
        db  070h, 072h
        db  065h, 076h, 069h
        outsd
        db  075h, 073h
        and    byte ptr [ebx + 6fh], ah
        outsb
        outsb
        arpl   word ptr gs:[ecx + ebp*2 + 6fh], si
loc_2_0952:
        outsb
        or     al, byte ptr cs:[eax]
        and    eax, 6e692073h
        and    byte ptr [ebp + 73h], dh
        or     al, byte ptr cs:[eax]
loc_2_0964:
        dec    ebp
        outsd
        insd
        and    byte ptr [ecx + edi*2 + 70h], dh
        cmp    ah, byte ptr gs:[eax]
        and    eax, 4d000a73h
        outsd
        insd
        and    byte ptr [ecx + 6eh], ch
        and    byte ptr [eax + 61h], dh
        db  074h, 068h
        cmp    ah, byte ptr [eax]
        and    eax, 4d000a73h
        outsd
        insd
        and    byte ptr [ecx + 6eh], ch
        and    byte ptr [ebx + 65h], dh
        arpl   word ptr [ecx + ebp*2 + 6fh], si
        outsb
        cmp    ah, byte ptr [eax]
        and    eax, 25000a73h
        db  073h, 020h
        arpl   word ptr [edi + ebp*2 + 73h], bp
        or     al, byte ptr cs:[eax]
_LMSGTABLE ends
        end
