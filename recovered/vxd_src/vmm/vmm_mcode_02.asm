; vmm_mcode_02.asm -- VMM object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

loc_2_0000:
        or     al, 0
        add    byte ptr [eax], al
loc_2_0004:
        sub    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_000E:
        ret    200h
        db  001h
loc_2_0012:
        insb
        add    dword ptr [ebx + 1], esi
        int    2
        db  073h, 003h
        daa
        add    al, 23h
        db  005h
loc_2_001E:
        popfd
        push   es
        cmp    eax, dword ptr [edi]
        int3
        pop    es
        outsd
        or     byte ptr [eax], al
        add    byte ptr [ecx + 6eh], cl
        db  073h, 075h
        imul   sp, word ptr [ebx + 69h], 6e65h
        db  074h, 020h
        insd
        insd
        outsd
        jb     SHORT loc_2_00B4
        and    byte ptr [edi + ebp*2 + 20h], dh
        imul   ebp, dword ptr [esi + 69h], 6c616974h
        imul   edi, dword ptr [edx + 65h], 6e695720h
        outsd
        ja     SHORT loc_2_00C4
        or     eax, 510a0d0ah
        jne    SHORT loc_2_00C2
        db  074h, 020h
        outsd
        outsb
        and    byte ptr gs:[edi + 72h], ch
        and    byte ptr [ebp + 6fh], ch
        db  072h, 065h
        and    byte ptr [ebp + 65h], ch
        insd
        outsd
        db  072h, 079h
        sub    eax, 69736572h
        outsb
        db  074h, 020h
        db  070h, 072h
        outsd
        jb     NEAR PTR loc_2_00DE
        insd
        db  073h, 020h
        outsd
        jb     SHORT loc_2_00A3
        db  072h, 065h
        insd
        outsd
        db  076h, 065h
        and    byte ptr [ebp + 6eh], dh
        outsb
        arpl   word ptr gs:[ebp + 73h], sp
        jae    SHORT loc_2_00F4
        db  072h, 079h
        or     eax, 6974750ah
        insb
        imul   esi, dword ptr [ecx + ebp*2 + 65h], 72662073h
loc_2_00A3:
        outsd
        insd
        and    byte ptr [ecx + 6fh], bh
        db  075h, 072h
        and    byte ptr [ebx + 4fh], al
        dec    esi
        inc    esi
        dec    ecx
        inc    edi
        push   ebx
        pop    ecx
loc_2_00B4:
        push   ebx
        and    byte ptr [ecx + 6eh], ah
        and    byte ptr fs:[ecx + 55h], al
        push   esp
        dec    edi
        inc    ebp
        pop    eax
        inc    ebp
        inc    ebx
loc_2_00C2:
        inc    edx
loc_2_00C4:
        inc    ecx
        push   esp
        and    byte ptr [esi + 69h], ah
        insb
        db  065h, 073h, 02Ch
        and    byte ptr [ecx + 6eh], ah
        and    byte ptr fs:[edx + 65h], dh
        db  073h, 074h
        popad
        db  072h, 074h
        or     eax, 756f790ah
loc_2_00DE:
        db  072h, 020h
        arpl   word ptr [edi + 6dh], bp
        db  070h, 075h
        je     SHORT loc_2_014C
        db  072h, 02Eh
        add    byte ptr [edi + 69h], dl
        outsb
        outsd
        db  077h, 073h
        and    byte ptr [eax + 72h], dh
loc_2_00F4:
        outsd
        db  074h, 065h
        arpl   word ptr [ecx + ebp*2 + 6fh], si
        outsb
        and    byte ptr [ebp + 72h], ah
        db  072h, 06Fh
        db  072h, 02Eh
        and    byte ptr [eax], ah
        pop    ecx
        outsd
        db  075h, 020h
        outsb
        and    byte ptr fs:[edi + ebp*2 + 20h], dh
        db  072h, 065h
        db  073h, 074h
        popad
        jb     SHORT loc_2_018C
        and    byte ptr [ecx + 6fh], bh
        jne    SHORT loc_2_018F
        and    byte ptr [ebx + 6fh], ah
        insd
        jo     SHORT loc_2_0198
        db  074h, 065h
        db  072h, 02Eh
        or     eax, 6854000ah
        imul   esi, dword ptr [ebx + 20h], 6c707061h
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        and    byte ptr [ebx + 61h], ah
        outsb
        and    byte ptr [esi + 6fh], ch
        je     SHORT loc_2_0163
        db  072h, 075h
        outsb
        and    byte ptr [edx + 65h], ah
        arpl   word ptr [ecx + 75h], sp
loc_2_014C:
        jae    SHORT loc_2_01B3
        and    byte ptr [edi + 66h], ch
        and    byte ptr [ecx + 6eh], ch
        db  073h, 075h
        imul   sp, word ptr [ebx + 69h], 6e65h
        db  074h, 020h
        insd
        insd
        outsd
loc_2_0163:
        db  072h, 079h
        and    byte ptr cs:[ecx + 75h], dl
        imul   esi, dword ptr [eax + 6fh], 6f20656eh
        db  072h, 020h
        insd
        outsd
        db  072h, 065h
        and    byte ptr [ecx + 70h], ah
        db  070h, 06Ch
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        db  073h, 020h
        popad
        outsb
        and    byte ptr fs:[edx + esi*2 + 79h], dh
loc_2_018C:
        and    byte ptr [ecx + 67h], ah
loc_2_018F:
        popad
        imul   ebp, dword ptr [esi + 2eh], 2d534d00h
        inc    esp
loc_2_0198:
        dec    edi
        push   ebx
        add    byte ptr [ecx + 6eh], cl
        jbe    SHORT loc_2_0200
        insb
        imul   esp, dword ptr [eax + 56h], 64204478h
        db  079h, 06Eh
        popad
        insd
        imul   esp, dword ptr [ebx + 20h], 6b6e696ch
loc_2_01B3:
        and    byte ptr [ebx + 61h], ah
        insb
        insb
        and    byte ptr [esi + 72h], ah
        outsd
        insd
        and    byte ptr [6f742073h], ah
        and    byte ptr [ebp + 76h], ah
        imul   esp, dword ptr [ebx + 65h], 32252220h
        db  073h, 022h
        sub    al, 20h
        db  073h, 065h
        jb     SHORT loc_2_024C
        imul   esp, dword ptr [ebx + 65h], 78332520h
        or     eax, 756f590ah
        db  072h, 020h
        push   edi
        imul   ebp, dword ptr [esi + 64h], 2073776fh
        arpl   word ptr [edi + 6eh], bp
        imul   sp, word ptr [edi + 75h], 6172h
        db  074h, 069h
        outsd
        outsb
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [ecx + 6eh], ch
loc_2_0200:
        db  076h, 061h
        insb
        imul   esp, dword ptr [esi + ebp + 20h], 206e7552h
        db  074h, 068h
        and    byte ptr gs:[edi + 69h], dl
        outsb
        outsd
        ja     SHORT loc_2_0289
        and    byte ptr [ebx + 65h], dl
        je     SHORT loc_2_0290
        jo     SHORT loc_2_023D
        db  070h, 072h
        outsd
        jb     NEAR PTR loc_2_0284
        insd
        or     eax, 6167610ah
        imul   ebp, dword ptr [esi + 20h], 63206f74h
        outsd
        db  072h, 072h
        arpl   word ptr gs:[eax + 74h], si
        push   70207369h
loc_2_023D:
        jb     SHORT loc_2_02AE
        bound  ebp, qword ptr [ebp + 6dh]
        or     eax, 6f540a0ah
        and    byte ptr [ebx + 6fh], ah
loc_2_024C:
        outsb
        db  074h, 069h
        outsb
        jne    SHORT loc_2_02B7
        and    byte ptr [edx + 75h], dh
        outsb
        outsb
        imul   ebp, dword ptr [esi + 67h], 6e695720h
        outsd
        db  077h, 073h
        and    byte ptr [eax + 72h], dh
        jae    NEAR PTR loc_2_02DB
        and    byte ptr [ecx + 20h], bl
        outsd
        db  072h, 020h
        inc    ebp
        dec    esi
        push   esp
        inc    ebp
        push   edx
        and    byte ptr cs:[edi + ebp*2 + 20h], dl
        db  071h, 075h
        imul   esi, dword ptr [eax + 74h], 63206568h
        db  075h, 072h
loc_2_0284:
        db  072h, 065h
        outsb
        je     SHORT loc_2_02A9
loc_2_0289:
        db  070h, 072h
        outsd
        jb     NEAR PTR loc_2_02F0
        insd
loc_2_0290:
        and    byte ptr [eax + 72h], dh
        db  065h, 073h, 073h
        and    byte ptr [esi + 2eh], cl
        and    byte ptr [ecx + 66h], cl
        and    byte ptr [ecx + 6fh], bh
        db  075h, 020h
        arpl   word ptr [edi + 6eh], bp
        db  074h, 069h
        outsb
        db  075h, 065h
loc_2_02A9:
        and    byte ptr [edx + 75h], dh
        outsb
        outsb
loc_2_02AE:
        imul   ebp, dword ptr [esi + 67h], 6e695720h
        outsd
loc_2_02B7:
        ja     SHORT loc_2_032C
        sub    al, 20h
        jns    SHORT loc_2_032C
        db  075h, 072h
        and    byte ptr [ebx + 79h], dh
        db  073h, 074h
        insd
        and    byte ptr [ebp + 61h], ch
        db  079h, 020h
        bound  esp, qword ptr [ebp + 63h]
        outsd
        insd
        and    byte ptr gs:[ebp + 6eh], dh
        db  073h, 074h
        popad
        bound  ebp, qword ptr [ebp + 2eh]
loc_2_02DB:
        and    byte ptr [edi + ebp*2 + 20h], al
        jns    SHORT loc_2_0350
        db  075h, 020h
        ja     SHORT loc_2_0346
        outsb
        db  074h, 020h
        je     SHORT loc_2_0359
        and    byte ptr [ebx + 6fh], ah
        outsb
        je     SHORT loc_2_0359
loc_2_02F0:
        outsb
        jne    SHORT loc_2_0358
        aas
        add    byte ptr [ecx + 6eh], cl
        db  076h, 061h
        insb
        imul   esp, dword ptr [eax + 56h], 64204478h
        db  079h, 06Eh
        popad
        insd
        imul   esp, dword ptr [ebx + 20h], 6b6e696ch
        and    byte ptr [ebx + 61h], ah
        insb
        insb
        and    byte ptr [edi + ebp*2 + 20h], dh
        db  064h, 065h, 076h, 069h
        arpl   word ptr [ebp + 20h], sp
        outsb
        db  075h, 06Dh
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [73202c78h], ah
        db  065h, 072h, 076h
loc_2_032C:
        imul   esp, dword ptr [ebx + 65h], 78322520h
        or     eax, 756f590ah
        db  072h, 020h
        push   edi
        imul   ebp, dword ptr [esi + 64h], 2073776fh
        arpl   word ptr [edi + 6eh], bp
loc_2_0346:
        imul   sp, word ptr [edi + 75h], 6172h
        db  074h, 069h
        outsd
        outsb
loc_2_0350:
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [ecx + 6eh], ch
        db  076h, 061h
loc_2_0358:
        insb
loc_2_0359:
        imul   esp, dword ptr [esi + ebp + 20h], 206e7552h
        je     SHORT loc_2_03CB
        and    byte ptr gs:[edi + 69h], dl
        outsb
        outsd
        ja     SHORT loc_2_03DF
        and    byte ptr [ebx + 65h], dl
        je     SHORT loc_2_03E6
        jo     SHORT loc_2_0393
        jo     SHORT loc_2_03E7
        outsd
        jb     NEAR PTR loc_2_03DA
        insd
        or     eax, 6167610ah
        imul   ebp, dword ptr [esi + 20h], 63206f74h
        outsd
        db  072h, 072h
        arpl   word ptr gs:[eax + 74h], si
        push   70207369h
loc_2_0393:
        db  072h, 06Fh
        bound  ebp, qword ptr [ebp + 6dh]
        add    byte ptr cs:[eax + ebp*2 + 65h], dl
        db  072h, 065h
        and    byte ptr [ecx + 72h], ah
        and    byte ptr gs:[esi + 6fh], ch
        and    byte ptr [ebx + 70h], dh
        popad
        jb     SHORT loc_2_0412
        and    byte ptr [ebx + 74h], dh
        popad
        arpl   word ptr [ebx + 20h], bp
        db  070h, 061h
        db  067h, 065h, 073h, 02Eh
        and    byte ptr [eax], ah
        dec    ecx
        je     SHORT loc_2_03DF
        insd
        popad
        db  079h, 020h
        bound  esp, qword ptr [ebp + 20h]
        outsb
        arpl   word ptr gs:[ebp + 73h], sp
loc_2_03CB:
        db  073h, 061h
        db  072h, 079h
        and    byte ptr [edi + ebp*2 + 20h], dh
        imul   ebp, dword ptr [esi + 63h], 73616572h
loc_2_03DA:
        and    byte ptr gs:[eax + ebp*2 + 65h], dh
loc_2_03DF:
        or     eax, 7465730ah
        db  074h, 069h
loc_2_03E6:
        outsb
loc_2_03E7:
        and    byte ptr [bx + 66h], ch
        and    byte ptr [edi], ah
        dec    ebp
        imul   ebp, dword ptr [esi + 53h], 20277350h
        imul   ebp, dword ptr [esi + 20h], 54535953h
        inc    ebp
        dec    ebp
        dec    ecx
        dec    esi
        dec    ecx
        and    byte ptr [edi + ebp*2 + 20h], dh
        jo     SHORT loc_2_047A
        db  065h, 076h, 065h
        outsb
        db  074h, 020h
        db  070h, 06Fh
        jae    SHORT loc_2_0485
loc_2_0412:
        imul   esp, dword ptr [edx + 6ch], 74732065h
        popad
        arpl   word ptr [ebx + 73h], bp
        and    byte ptr [esi + 61h], ah
        db  075h, 06Ch
        db  074h, 073h
        or     eax, 6568540ah
        db  072h, 065h
        and    byte ptr [ecx + 72h], ah
        and    byte ptr gs:[ebx + 75h], ah
        db  072h, 072h
        outsb
        je     SHORT loc_2_04A5
        db  079h, 020h
        and    eax, 50532075h
        db  073h, 020h
        popad
        insb
        insb
        outsd
        arpl   word ptr [ecx + 74h], sp
        or     eax, 6554000ah
        db  072h, 06Dh
        imul   ebp, dword ptr [esi + 61h], 676e6974h
        and    byte ptr [eax + ebp*2 + 72h], dh
        popad
        and    byte ptr fs:[ebp + esi*2 + 65h], ah
        and    byte ptr [edi + ebp*2 + 20h], dh
        popad
        and    byte ptr [ebx + 74h], dh
        popad
        arpl   word ptr [ebx + 20h], bp
        outsd
        db  076h, 065h
        db  072h, 066h
        insb
        outsd
        ja     SHORT loc_2_049A
loc_2_047A:
        db  070h, 072h
        outsd
        bound  ebp, qword ptr [ebp + 6dh]
        and    byte ptr cs:[eax], ah
        inc    ecx
loc_2_0485:
        and    byte ptr [esi + 78h], dl
        inc    esp
        sub    al, 20h
        jo     SHORT loc_2_04FC
        jae    SHORT loc_2_0502
        imul   esp, dword ptr [edx + 6ch], 720a0d79h
        arpl   word ptr gs:[ebp + 6eh], sp
loc_2_049A:
        db  074h, 06Ch
        db  079h, 020h
        imul   ebp, dword ptr [esi + 73h], 6c6c6174h
loc_2_04A5:
        sub    al, 20h
        push   63207361h
        outsd
        outsb
        db  073h, 075h
        insd
        and    byte ptr fs:[edi + ebp*2 + 6fh], dh
        and    byte ptr [ebp + 75h], ch
        arpl   word ptr [eax + 20h], bp
        jae    SHORT loc_2_0535
        popad
        arpl   word ptr [ebx + 20h], bp
        jae    SHORT loc_2_0537
        popad
        arpl   word ptr [ebp + 2eh], sp
        and    byte ptr [eax], ah
        dec    ecx
        outsb
        arpl   word ptr [edx + 65h], si
        popad
        db  073h, 065h
        or     eax, 6568740ah
        and    byte ptr [ebx + 65h], dh
        je     SHORT loc_2_0553
        imul   ebp, dword ptr [esi + 67h], 20666f20h
        daa
        dec    ebp
        imul   ebp, dword ptr [esi + 53h], 20277350h
        imul   ebp, dword ptr [esi + 20h], 54535953h
        inc    ebp
        dec    ebp
        dec    ecx
        dec    esi
        dec    ecx
loc_2_04FC:
        and    byte ptr [edi + 72h], ch
        and    byte ptr [edx + 65h], dh
loc_2_0502:
        insd
        outsd
        db  076h, 065h
        and    byte ptr [edx + 65h], dh
        arpl   word ptr [ebp + 6eh], sp
        je     SHORT loc_2_057A
        db  079h, 020h
        imul   ebp, dword ptr [esi + 73h], 6c6c6174h
        or     eax, 4478560ah
        daa
        jae    SHORT loc_2_054F
        and    byte ptr [eax], ah
        push   esp
        push   20657265h
        popad
        db  072h, 065h
        and    byte ptr [ebx + 75h], ah
        db  072h, 072h
        outsb
        db  074h, 06Ch
loc_2_0535:
        db  079h, 020h
loc_2_0537:
        and    eax, 50532075h
        jae    SHORT loc_2_055E
        popad
        insb
        insb
        outsd
        arpl   word ptr [ecx + 74h], sp
        or     eax, 2041000ah
        db  073h, 074h
loc_2_054F:
        popad
        arpl   word ptr [ebx + 20h], bp
loc_2_0553:
        outsd
        db  076h, 065h
        jb     SHORT loc_2_05BE
        insb
        outsd
        db  077h, 020h
        jo     SHORT loc_2_05D0
loc_2_055E:
        outsd
        bound  ebp, qword ptr [ebp + 6dh]
        and    byte ptr [eax + 61h], ch
        jae    SHORT loc_2_0588
        outsd
        arpl   word ptr [ebx + 75h], sp
        db  072h, 072h
        and    byte ptr fs:[edi + 68h], dh
        imul   esp, dword ptr [ebx + 68h], 6e616320h
loc_2_057A:
        and    byte ptr [esi + 6fh], ch
        db  074h, 020h
        bound  esp, qword ptr [ebp + 20h]
        jne    NEAR PTR loc_2_05F1
        insb
        jns    SHORT loc_2_05A8
loc_2_0588:
        db  072h, 065h
        arpl   word ptr [edi + 76h], bp
        db  065h, 072h, 064h
        or     eax, 5620410ah
        db  078h, 044h
        sub    al, 20h
        jo     SHORT loc_2_060B
        db  073h, 073h
        imul   esp, dword ptr [edx + 6ch], 65722079h
        arpl   word ptr [ebp + 6eh], sp
loc_2_05A8:
        db  074h, 06Ch
        db  079h, 020h
        imul   ebp, dword ptr [esi + 73h], 6c6c6174h
        sub    al, 20h
        push   63207361h
        outsd
        outsb
loc_2_05BE:
        db  073h, 075h
        insd
        and    byte ptr fs:[edi + ebp*2 + 6fh], dh
        and    byte ptr [ebp + 75h], ch
        arpl   word ptr [eax + 20h], bp
        db  073h, 074h
        popad
loc_2_05D0:
        arpl   word ptr [ebx + 20h], bp
        jae    SHORT loc_2_0645
        popad
        arpl   word ptr [ebp + 2eh], sp
        or     eax, 756f590ah
        and    byte ptr [ebx + 68h], dh
        outsd
        db  075h, 06Ch
        and    byte ptr fs:[ecx + 74h], ah
        je     SHORT loc_2_064F
        insd
        db  070h, 074h
        and    byte ptr [edi + ebp*2 + 20h], dh
loc_2_05F1:
        db  073h, 061h
        db  076h, 065h
        and    byte ptr [ecx + 6ch], ah
        insb
        and    byte ptr [edi + 6fh], dh
        db  072h, 06Bh
        and    byte ptr [ecx + 6eh], ah
        and    byte ptr fs:[ebx + 68h], dh
        db  075h, 074h
        outsd
        db  077h, 06Eh
loc_2_060B:
        and    byte ptr [ecx + 6dh], ch
        insd
        imul   esp, dword ptr fs:[ecx + 74h], 2e796c65h
        or     eax, 756f590ah
        and    byte ptr [ebx + 61h], ah
        outsb
        and    byte ptr [ecx + 76h], ah
        outsd
        imul   esp, dword ptr [eax + 74h], 20736968h
        db  070h, 072h
        outsd
        bound  ebp, qword ptr [ebp + 6dh]
        and    byte ptr [ecx + 6eh], ch
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [esi + 75h], ah
        db  074h, 075h
        db  072h, 065h
        and    byte ptr [edx + 79h], ah
loc_2_0645:
        and    byte ptr [ecx + 6eh], ch
        arpl   word ptr [edx + 65h], si
        popad
        db  073h, 069h
        outsb
loc_2_064F:
        and    byte ptr [si + 68h], dh
        and    byte ptr gs:[ebx + 65h], dh
        db  074h, 074h
        imul   ebp, dword ptr [esi + 67h], 666f0a0dh
        and    byte ptr [edi], ah
        dec    ebp
        imul   ebp, dword ptr [esi + 53h], 20277350h
        imul   ebp, dword ptr [esi + 20h], 54535953h
        inc    ebp
        dec    ebp
        dec    ecx
        dec    esi
        dec    ecx
        and    byte ptr [edi + 72h], ch
        and    byte ptr [edx + 65h], dh
        insd
        outsd
        db  076h, 065h
        and    byte ptr [edx + 65h], dh
        arpl   word ptr [ebp + 6eh], sp
        je     SHORT loc_2_06F5
        jns    SHORT loc_2_06AB
        imul   ebp, dword ptr [esi + 73h], 6c6c6174h
        and    byte ptr fs:[esi + 78h], dl
        inc    esp
        daa
        jae    SHORT loc_2_06C9
        or     eax, 6568540ah
        jb     SHORT loc_2_0707
        and    byte ptr [ecx + 72h], ah
        and    byte ptr gs:[ebx + 75h], ah
        jb     SHORT loc_2_071D
loc_2_06AB:
        outsb
        je     SHORT loc_2_071B
        db  079h, 020h
        and    eax, 50532075h
        db  073h, 020h
        popad
        insb
        insb
        outsd
        arpl   word ptr [ecx + 74h], sp
        or     eax, 0a0d000ah
        or     ch, byte ptr [edx]
loc_2_06C9:
        and    byte ptr [eax], ah
        push   eax
        db  072h, 065h
        db  073h, 073h
        and    byte ptr [ecx + 6eh], ah
        jns    SHORT loc_2_06F5
        imul   esp, dword ptr [ebp + 79h], 20h
        db  074h, 06Fh
        and    byte ptr [ecx + 74h], ah
        db  074h, 065h
        insd
        jo     SHORT loc_2_0757
        and    byte ptr [edi + ebp*2 + 20h], dh
        arpl   word ptr [edi + 6eh], bp
        db  074h, 069h
        outsb
        jne    SHORT loc_2_0754
        or     eax, 20202a0ah
loc_2_06F5:
        push   eax
        db  072h, 065h
        jae    SHORT loc_2_076D
        and    byte ptr [ebx + 54h], al
        push   edx
        dec    esp
        sub    eax, dword ptr [ecx + 4ch]
        push   esp
        sub    eax, dword ptr [ebp + eax*2 + 4ch]
loc_2_0707:
        and    byte ptr [edi + ebp*2 + 20h], dh
        db  072h, 065h
        db  073h, 074h
        popad
        db  072h, 074h
        and    byte ptr [ecx + 6fh], bh
        db  075h, 072h
        and    byte ptr [ebx + 6fh], ah
        insd
loc_2_071B:
        db  070h, 075h
loc_2_071D:
        je     SHORT loc_2_0784
        db  072h, 02Eh
        and    byte ptr [eax], ah
        pop    ecx
        outsd
        db  075h, 020h
        db  077h, 069h
        insb
        insb
        or     eax, 2020080ah
        and    byte ptr [edi + ebp*2 + 73h], ch
        and    byte ptr gs:[ecx + 6eh], ah
        db  079h, 020h
        db  075h, 06Eh
        jae    SHORT loc_2_079F
        jbe    SHORT loc_2_07A5
        and    byte ptr fs:[ecx + 6eh], ch
        outsw
        db  072h, 06Dh
        popad
        je     SHORT loc_2_07B4
        outsd
        outsb
        and    byte ptr [ecx + 6eh], ch
        and    byte ptr [ecx + 6ch], ah
        insb
loc_2_0754:
        and    byte ptr [ecx + 70h], ah
loc_2_0757:
        db  070h, 06Ch
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        db  073h, 02Eh
        add    byte ptr [ecx + 6eh], al
        and    byte ptr [ebp + 78h], ah
        arpl   word ptr [ebp + 70h], sp
        je     SHORT loc_2_07D6
loc_2_076D:
        outsd
        outsb
        and    byte ptr [20783231h], ah
        push   6f207361h
        arpl   word ptr [ebx + 75h], sp
        db  072h, 072h
        and    byte ptr fs:[ecx + 74h], ah
loc_2_0784:
        and    byte ptr [3a783432h], ah
        and    eax, 20783833h
        imul   ebp, dword ptr [esi + 20h], 20447856h
        and    eax, 202e7334h
        and    byte ptr [eax + ebp*2 + 69h], dl
loc_2_079F:
        db  073h, 020h
        ja     SHORT loc_2_0804
        db  073h, 020h
loc_2_07A5:
        arpl   word ptr [ecx + 6ch], sp
        insb
        and    byte ptr fs:[esi + 72h], ah
        outsd
        insd
        and    byte ptr [eax], dh
        xor    byte ptr [edx], dh
loc_2_07B4:
        cmp    byte ptr [edx], bh
        and    eax, 20783835h
        imul   ebp, dword ptr [esi + 20h], 20447856h
        and    eax, 202e7336h
        and    byte ptr [ecx + 74h], cl
        and    byte ptr [ebp + 61h], ch
        db  079h, 020h
        bound  esp, qword ptr [ebp + 20h]
        jo     SHORT loc_2_0843
        jae    SHORT loc_2_0849
loc_2_07D6:
        imul   esp, dword ptr [edx + 6ch], 6f742065h
        and    byte ptr [ebx + 6fh], ah
        outsb
        db  074h, 069h
        outsb
        db  075h, 065h
        and    byte ptr [esi + 6fh], ch
        jb     SHORT loc_2_0858
        popad
        insb
        insb
        db  079h, 02Eh
        and    eax, 41007337h
        and    byte ptr [ebx], dh
        xor    ch, byte ptr [20746962h]
        db  064h, 065h, 076h, 069h
        arpl   word ptr [ebp + 20h], sp
loc_2_0804:
        db  064h, 072h, 069h
        db  076h, 065h
        jb     SHORT loc_2_082B
        push   63207361h
        outsd
        db  072h, 072h
        db  075h, 070h
        je     SHORT loc_2_087C
        and    byte ptr fs:[ebx + 72h], ah
        imul   esi, dword ptr [ecx + ebp*2 + 63h], 73206c61h
        db  079h, 073h
        db  074h, 065h
        insd
        and    byte ptr [ebp + 65h], ch
loc_2_082B:
        insd
        outsd
        db  072h, 079h
        sub    al, 20h
        db  072h, 065h
        db  073h, 075h
        insb
        db  074h, 069h
        outsb
        and    byte ptr [bx + di + 6eh], ch
        and    byte ptr [ecx + 6eh], ah
        and    byte ptr [ebp + 78h], ah
loc_2_0843:
        arpl   word ptr [ebp + 70h], sp
        db  074h, 069h
        outsd
loc_2_0849:
        outsb
        and    byte ptr [20783231h], ah
        popad
        je     SHORT loc_2_0873
        and    eax, 3a783432h
loc_2_0858:
        and    eax, 20783833h
        imul   ebp, dword ptr [esi + 20h], 20447856h
        and    eax, 202e7334h
        and    byte ptr [eax + ebp*2 + 69h], dl
        db  073h, 020h
        db  077h, 061h
        db  073h, 020h
loc_2_0873:
        arpl   word ptr [ecx + 6ch], sp
        insb
        and    byte ptr fs:[esi + 72h], ah
loc_2_087C:
        outsd
        insd
        and    byte ptr [eax], dh
        xor    byte ptr [edx], dh
        cmp    byte ptr [edx], bh
        and    eax, 20783835h
        imul   ebp, dword ptr [esi + 20h], 20447856h
        and    eax, 252e7336h
        aaa
        db  000h
_LMSGTABLE ends
        end
