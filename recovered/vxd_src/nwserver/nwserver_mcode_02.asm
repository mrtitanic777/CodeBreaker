; nwserver_mcode_02.asm -- NWSERVER object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  005h, 000h, 000h, 000h
loc_2_0004:
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_2_000C:
        add    byte ptr [eax], al
loc_2_000E:
        push   ss
        db  000h
loc_2_0010:
        jecxz  loc_2_0012
loc_2_0012:
        add    dword ptr [ecx], eax
loc_2_0014:
        cmc
        add    dword ptr [ebx + 3], edx
loc_2_0018:
        dec    esi
        je     NEAR PTR loc_2_0093
        outsd
        jb     SHORT loc_2_008A
        and    byte ptr [eax + 65h], dl
        db  065h, 072h, 020h
        push   ebx
        db  065h, 072h, 076h
        imul   esp, dword ptr [ebx + 65h], 6f590073h
        db  075h, 020h
        push   20657661h
        arpl   word ptr [eax + 6fh], bp
        jae    SHORT loc_2_00A1
        outsb
        and    byte ptr [ebp + 73h], dh
        db  065h, 072h, 02Dh
        insb
        jbe    NEAR PTR loc_2_00AC
        insb
        and    byte ptr [ebx + 65h], dh
        arpl   word ptr [ebp + 72h], si
        imul   esi, dword ptr [ecx + edi*2 + 2ch], 74756220h
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebx + 65h], dh
        db  072h, 076h
        db  065h, 072h, 020h
        jns    SHORT loc_2_00D3
        jne    SHORT loc_2_0086
        push   20657661h
        jae    SHORT loc_2_00D2
        insb
        arpl   word ptr gs:[ebp + 64h], si
        and    byte ptr [edi + ebp*2 + 20h], dh
        db  070h, 072h
        outsd
        db  076h, 069h
        and    byte ptr gs:[ebp + 73h], dh
        db  065h, 072h, 020h
        popad
        outsb
loc_2_0086:
        and    byte ptr fs:[edi + 72h], ah
loc_2_008A:
        outsd
        jne    SHORT loc_2_00FD
        and    byte ptr [ecx + ebp*2 + 73h], ch
        db  074h, 073h
loc_2_0093:
        sub    al, 20h
        and    eax, 69202c73h
        db  073h, 020h
        outsb
        outsd
        je     SHORT loc_2_00C0
        popad
loc_2_00A1:
        jbe    SHORT loc_2_0104
        imul   ebp, dword ptr [ecx + 62h], 202e656ch
        pop    ecx
loc_2_00AC:
        outsd
        jne    SHORT loc_2_00CF
        db  077h, 069h
        insb
        insb
        and    byte ptr [edx + 65h], ah
        and    byte ptr [ebp + 6eh], dh
        popad
        bound  ebp, qword ptr [ebp + 20h]
        je     SHORT loc_2_012F
loc_2_00C0:
        and    byte ptr [ebx + 68h], dh
        popad
        jb     SHORT loc_2_012B
        and    byte ptr [ecx + 6fh], bh
        db  075h, 072h
        and    byte ptr [esi + 69h], ah
        insb
loc_2_00CF:
        db  065h, 073h, 020h
loc_2_00D2:
        popad
loc_2_00D3:
        outsb
        and    byte ptr fs:[eax + 72h], dh
        imul   ebp, dword ptr [esi + 74h], 20737265h
        db  075h, 06Eh
        je     SHORT loc_2_014C
        insb
        and    byte ptr [ecx + 74h], ch
        and    byte ptr [edx + 65h], ah
        arpl   word ptr [edi + 6dh], bp
        db  065h, 073h, 020h
        popad
        jbe    SHORT loc_2_0154
        imul   ebp, dword ptr [ecx + 62h], 2e656ch
        dec    esi
        outsd
loc_2_00FD:
        db  074h, 020h
        outsb
        outsd
        db  075h, 067h
loc_2_0104:
        push   73657220h
        outsd
        jne    SHORT loc_2_017E
        arpl   word ptr [ebp + 73h], sp
        and    byte ptr [edi + ebp*2 + 20h], dh
        insb
        outsd
        popad
        add    byte ptr cs:[ecx + 6fh], bl
        db  075h, 020h
        push   20657661h
        arpl   word ptr [eax + 6fh], bp
        jae    SHORT loc_2_018C
        outsb
        and    byte ptr [ebp + 73h], dh
loc_2_012B:
        jb     NEAR PTR loc_2_015B
        insb
loc_2_012F:
        jbe    NEAR PTR loc_2_0197
        insb
        and    byte ptr [ebx + 65h], dh
        arpl   word ptr [ebp + 72h], si
        imul   esi, dword ptr [ecx + edi*2 + 20h], 20747562h
        jns    SHORT loc_2_01B2
        db  075h, 020h
        push   20657661h
        outsb
        outsd
loc_2_014C:
        je     SHORT loc_2_016E
        db  073h, 070h
        arpl   word ptr gs:[ecx + 66h], bp
loc_2_0154:
        imul   esp, dword ptr [ebp + 64h], 73206120h
loc_2_015B:
        jb     NEAR PTR loc_2_01D4
        db  065h, 072h, 020h
        db  077h, 068h
        db  065h, 072h, 065h
        and    byte ptr [ebp + 73h], dh
        jb     NEAR PTR loc_2_018C
        popad
        outsb
loc_2_016E:
        and    byte ptr fs:[edi + 72h], ah
        outsd
        db  075h, 070h
        and    byte ptr [ecx + ebp*2 + 73h], ch
        je     SHORT loc_2_01EE
        and    byte ptr [ecx + 72h], ah
loc_2_017E:
        and    byte ptr gs:[ebx + 74h], dh
        outsd
        jb     SHORT loc_2_01EA
        and    byte ptr cs:[edi + 70h], cl
        outsb
loc_2_018C:
        and    byte ptr [ebx + 6fh], al
        outsb
        db  074h, 072h
        outsd
        insb
        and    byte ptr [eax + 61h], dl
loc_2_0197:
        outsb
        insb
        sub    al, 20h
        outsd
        db  075h, 062h
        insb
        sub    eax, 63696c63h
        imul   esp, dword ptr [eax], 74h
        push   654e2065h
        je     SHORT loc_2_0228
        outsd
loc_2_01B2:
        db  072h, 06Bh
        and    byte ptr [ecx + 63h], ch
        outsd
        outsb
        sub    al, 20h
        popad
        outsb
        and    byte ptr fs:[eax + ebp*2 + 65h], dh
        outsb
        and    byte ptr [ebx + 6ch], ah
        imul   esp, dword ptr [ebx + 6bh], 63634120h
        db  065h, 073h, 073h
        and    byte ptr [ebx + 6fh], al
        outsb
loc_2_01D4:
        je     SHORT loc_2_0248
        outsd
        insb
        and    byte ptr cs:[esi + 65h], cl
        js     SHORT loc_2_0252
        sub    al, 20h
        jae    SHORT loc_2_0252
        arpl   word ptr gs:[ecx + 66h], bp
        db  079h, 020h
        je     SHORT loc_2_0252
loc_2_01EA:
        and    byte ptr gs:[esi + 61h], ch
loc_2_01EE:
        insd
        and    byte ptr gs:[edi + 66h], ch
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebx + 65h], dh
        db  072h, 076h
        db  065h, 072h, 020h
        popad
        outsb
        and    byte ptr fs:[ebx + 6ch], ah
        imul   esp, dword ptr [ebx + 6bh], 2e4b4f20h
        add    byte ptr [ecx + 6fh], bl
        db  075h, 020h
        push   20657661h
        imul   esi, dword ptr fs:[ebx + 61h], 64656c62h
        and    byte ptr [edx + 6fh], ah
        je     SHORT loc_2_028B
        and    byte ptr [edi + 6fh], dl
        db  072h, 06Bh
loc_2_0228:
        jb     NEAR PTR loc_2_029A
        jne    SHORT loc_2_029D
        and    byte ptr [ecx + 64h], al
        db  076h, 065h
        jb     SHORT loc_2_02A8
        imul   esi, dword ptr [ebx + 69h], 6120676eh
        outsb
        and    byte ptr fs:[ebx + 41h], dl
        push   eax
        and    byte ptr [ecx + 64h], al
        db  076h, 065h
        db  072h, 074h
loc_2_0248:
        imul   esi, dword ptr [ebx + 69h], 202e676eh
        and    byte ptr [ecx + 74h], al
loc_2_0252:
        and    byte ptr [ebp + 61h], ch
        jae    SHORT loc_2_02CC
        and    byte ptr [edi + 6eh], ch
        and    byte ptr gs:[edi + 66h], ch
        and    byte ptr [eax + ebp*2 + 65h], dh
        db  073h, 065h
        and    byte ptr [ebp + 75h], ch
        db  073h, 074h
        and    byte ptr [edx + 65h], ah
        and    byte ptr [ebp + 6eh], ah
        popad
        bound  ebp, qword ptr [ebp + 64h]
        and    byte ptr [esi + 6fh], ah
        jb     SHORT loc_2_029A
        inc    esi
        imul   ebp, dword ptr [ebp + 20h], 20646e61h
        push   eax
        db  072h, 069h
        outsb
        db  074h, 065h
        db  072h, 020h
loc_2_028B:
        push   ebx
        push   6e697261h
        and    byte ptr [si + 6fh], dh
        and    byte ptr [edi + 6fh], dh
        jb     SHORT loc_2_0305
loc_2_029A:
        and    byte ptr [eax + 72h], dh
loc_2_029D:
        outsd
        jo     SHORT loc_2_0305
        db  072h, 06Ch
        jns    SHORT loc_2_02D2
        and    byte ptr [eax], ah
        push   edi
        outsd
loc_2_02A8:
        jb     SHORT loc_2_0315
        jb     NEAR PTR loc_2_031C
        jne    SHORT loc_2_031F
        and    byte ptr [ecx + 64h], al
        db  076h, 065h
        db  072h, 074h
        imul   esi, dword ptr [ebx + 69h], 6820676eh
        popad
        jae    SHORT loc_2_02E0
        bound  esp, qword ptr [ebp + 65h]
        outsb
        and    byte ptr [edx + 65h], dh
        sub    eax, 62616e65h
loc_2_02CC:
        insb
        and    byte ptr fs:[esi + 6fh], ah
loc_2_02D2:
        jb     SHORT loc_2_02F4
        jns    SHORT loc_2_0345
        db  075h, 02Eh
        and    byte ptr [edi + ebp*2 + 20h], dl
        popad
        push   75h
loc_2_02E0:
        db  073h, 074h
        and    byte ptr [eax + ebp*2 + 65h], dh
        jae    SHORT loc_2_034D
        and    byte ptr [ebx + 65h], dh
        db  074h, 074h
        imul   ebp, dword ptr [esi + 67h], 6f202c73h
loc_2_02F4:
        db  070h, 065h
        outsb
        and    byte ptr [ebx + 6fh], al
        outsb
        db  074h, 072h
        outsd
        insb
        and    byte ptr [eax + 61h], dl
        outsb
        insb
loc_2_0305:
        sub    al, 20h
        outsd
        db  075h, 062h
        insb
        sub    eax, 63696c63h
        imul   esp, dword ptr [eax], 74h
loc_2_0315:
        push   654e2065h
        db  074h, 077h
loc_2_031C:
        outsd
        db  072h, 06Bh
loc_2_031F:
        and    byte ptr [ecx + 63h], ch
        outsd
        outsb
        sub    al, 20h
        popad
        outsb
        and    byte ptr fs:[eax + ebp*2 + 65h], dh
        outsb
        and    byte ptr [edi + ebp*2 + 75h], ah
        bound  ebp, qword ptr [ebp + 2dh]
        arpl   word ptr [ecx + ebp*2 + 63h], bp
        imul   esp, dword ptr [eax], 46h
        imul   ebp, dword ptr [ebp + 20h], 20646e61h
loc_2_0345:
        push   eax
        db  072h, 069h
        outsb
        db  074h, 065h
        db  072h, 020h
loc_2_034D:
        push   ebx
        push   6e697261h
        and    byte ptr [bp + 6fh], ah
        db  072h, 020h
        dec    esi
        db  065h, 074h, 077h
        popad
        db  072h, 065h
        and    byte ptr [esi + 65h], cl
        db  074h, 077h
        outsd
        db  072h, 06Bh
        db  073h, 02Eh
        db  000h
_LMSGTABLE ends
        end
