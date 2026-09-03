; vmm_mcode_03.asm -- VMM object 3 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

loc_3_0000:
        aaa
        add    byte ptr [eax], al
        db  000h
loc_3_0004:
        jl     6
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_3_000C:
        add    byte ptr [eax], al
        sub    dword ptr [eax], eax
loc_3_0010:
        dec    ecx
        add    byte ptr [eax], bl
loc_3_0014:
        pop    esp
        add    byte ptr [edx], ah
loc_3_0018:
        imul   eax, dword ptr [eax], 7b006fh
loc_3_001E:
        add    dword ptr [eax], -4dh
        add    cl, al
        add    bl, cl
        add    dl, dh
        add    byte ptr [ebx], al
        add    dword ptr [ebx], ecx
        add    dword ptr [eax + 6a01ad01h], esp
        db  002h
loc_3_0032:
        jae    SHORT loc_3_0036
loc_3_0034:
        test   byte ptr [edx], al
loc_3_0036:
        sub    al, byte ptr [ebx]
loc_3_0038:
        xor    al, 3
loc_3_003A:
        inc    eax
        add    ecx, dword ptr [ebx + eax + 74h]
        add    eax, dword ptr [ebx + eax + 3c00394h]
loc_3_0046:
        leave
        add    edx, edx
        add    edi, ebx
        add    eax, dword ptr [39040f04h]
        add    al, 42h
        add    al, 4ch
        add    al, 70h
        add    al, 7eh
        add    al, 8ah
        add    al, 0a6h
        add    al, 0b6h
        add    al, 0c7h
        add    al, 0fah
        add    al, 0dh
        add    eax, 5ec0522h
loc_3_006A:
        cld
        add    eax, 6250609h
loc_3_0070:
        cmp    eax, 66064d06h
        push   es
loc_3_0076:
        jle    SHORT loc_3_007E
loc_3_0078:
        mov    es, word ptr [esi]
        cmpsd
        push   es
loc_3_007C:
        inc    ebp
        push   edx
loc_3_007E:
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        push   ebp
        outsb
        db  073h, 075h
        jo     SHORT loc_3_00F9
        outsd
        db  072h, 074h
        and    byte ptr fs:[ecx + 74h], ah
        popad
        and    byte ptr [ebx + 6fh], ah
        outsb
        imul   sp, word ptr [edi + 75h], 6172h
        db  074h, 069h
        outsd
        outsb
        or     eax, 6857000ah
        imul   ebp, dword ptr [ebp + 20h], 74696e69h
        imul   esp, dword ptr [ecx + 6ch], 6e697a69h
        and    byte ptr [si + 65h], ah
        jbe    SHORT loc_3_0125
        arpl   word ptr [ebp + 20h], sp
        and    eax, 0a0d3a73h
        add    byte ptr [ebp + 65h], cl
        insd
        outsd
        db  072h, 079h
        and    byte ptr [ebp + 61h], cl
        outsb
        popad
        jb     NEAR PTR loc_3_00D4
loc_3_00D4:
        push   esi
        dec    ebp
        dec    ebp
        add    byte ptr [edx + 79h], ah
        je     SHORT loc_3_0141
        jae    SHORT loc_3_00DE
loc_3_00DE:
        popaw
        jne    SHORT loc_3_014E
        je     SHORT loc_3_0157
        add    byte ptr [eax + 61h], dh
        jae    NEAR PTR loc_3_00EB
loc_3_00EB:
        inc    esi
        jb     SHORT loc_3_0153
        and    byte ptr gs:[ebp + 65h], ch
        insd
        outsd
        db  072h, 079h
        add    byte ptr [ebx + 70h], ah
loc_3_00F9:
        inc    esi
        db  072h, 065h
        add    byte ptr gs:[ecx + 6dh], al
        outsd
        db  075h, 06Eh
        db  074h, 020h
        outsd
        and    byte ptr [eax + 68h], dh
        db  079h, 073h
        imul   esp, dword ptr [ebx + 61h], 656d206ch
        insd
        outsd
        db  072h, 079h
        and    byte ptr [esi + 6fh], ch
        db  074h, 020h
        arpl   word ptr [ebp + 72h], si
        jb     SHORT loc_3_0187
        outsb
        db  074h, 06Ch
loc_3_0125:
        db  079h, 020h
        imul   ebp, dword ptr [esi + 20h], 2e657375h
        add    byte ptr [edi + ebp*2 + 63h], cl
        imul   esp, dword ptr [ebp + 64h], 20h
        insd
        insd
        outsd
        db  072h, 079h
        add    byte ptr [ebx + 70h], ah
        dec    esp
loc_3_0141:
        outsd
        arpl   word ptr [ebx + 65h], bp
        add    byte ptr fs:[ecx + 6dh], al
        outsd
        jne    SHORT loc_3_01BA
        db  074h, 020h
loc_3_014E:
        outsd
        and    byte ptr [ebp + 65h], ch
loc_3_0153:
        insd
        outsd
        jb     SHORT loc_3_01D0
loc_3_0157:
        and    byte ptr [ecx + 6ch], ah
        insb
        outsd
        arpl   word ptr [ecx + 74h], sp
        and    byte ptr fs:[ecx + 6eh], ah
        and    byte ptr fs:[edi + ebp*2 + 63h], ch
        imul   esp, dword ptr [ebp + 64h], 2eh
        add    byte ptr [ebx + 77h], dl
        popad
        jo     SHORT loc_3_01E3
        popad
        bound  ebp, qword ptr [ebp + 20h]
        insd
        insd
        outsd
        jb     SHORT loc_3_01F7
        add    byte ptr [ebx + 70h], ah
        push   ebx
        db  077h, 061h
        jo     SHORT loc_3_0187
loc_3_0187:
        dec    esi
        jne    SHORT loc_3_01F7
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [edi + 66h], ch
        and    byte ptr [edx + 79h], ah
        db  074h, 065h
        jae    SHORT loc_3_01B7
        popad
        insb
        insb
        outsd
        arpl   word ptr [ecx + 74h], sp
        and    byte ptr fs:[esi + 72h], ah
        outsd
        insd
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebx + 77h], dh
        popad
        db  070h, 020h
        imul   bp, word ptr [ebp + 2eh], 2020h
        dec    esi
loc_3_01B7:
        outsd
        je     SHORT loc_3_021F
loc_3_01BA:
        and    byte ptr [eax + ebp*2 + 61h], dh
        db  074h, 020h
        jae    SHORT loc_3_0239
        popad
        db  070h, 020h
        imul   bp, word ptr [ebp + 20h], 6170h
        jae    NEAR PTR loc_3_01F0
loc_3_01D0:
        ja     SHORT loc_3_023A
        imul   esp, dword ptr [ebx + 68h], 65726120h
        and    byte ptr [edi + ebp*2 + 63h], ch
        imul   esp, dword ptr [ebp + 64h], 20h
        db  073h, 074h
loc_3_01E3:
        imul   ebp, dword ptr [esp + ebp*2 + 20h], 6e756f63h
        db  074h, 020h
        popad
        db  073h, 020h
loc_3_01F0:
        pushad
        db  073h, 077h
        popad
        db  070h, 070h
        popad
loc_3_01F7:
        bound  ebp, qword ptr [ebp + 27h]
        and    byte ptr [esi + 6fh], ah
        db  072h, 020h
        db  074h, 068h
        and    byte ptr gs:[eax + 75h], dh
        jb     SHORT loc_3_0278
        outsd
        db  073h, 065h
        and    byte ptr [edi + 66h], ch
        and    byte ptr [eax + ebp*2 + 69h], dh
        db  073h, 020h
        insd
        db  065h, 074h, 072h
        imul   esp, dword ptr [ebx + 2eh], 68744f00h
loc_3_021F:
        jb     NEAR PTR loc_3_0242
        insd
        insd
        outsd
        jb     SHORT loc_3_02A1
        add    byte ptr [esi + 75h], cl
        insd
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [edi + 66h], ch
        and    byte ptr [edx + 79h], ah
        db  074h, 065h
        jae    SHORT loc_3_0259
loc_3_0239:
        outsd
loc_3_023A:
        and    byte ptr [ebp + 65h], ch
        insd
        outsd
        jb     SHORT loc_3_02BB
loc_3_0242:
        and    byte ptr [ecx + 6ch], ah
        insb
        outsd
        arpl   word ptr [ecx + 74h], sp
        and    byte ptr fs:[edi + 68h], dh
        imul   esp, dword ptr [ebx + 68h], 65726120h
        and    byte ptr [esi + 6fh], ch
loc_3_0259:
        je     SHORT loc_3_027B
        db  073h, 074h
        outsd
        db  072h, 065h
        and    byte ptr fs:[ecx + 6eh], ch
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebx + 77h], dh
        popad
        db  070h, 020h
        imul   bp, word ptr [ebp + 2eh], 2020h
        inc    ebp
        db  078h, 061h
loc_3_0278:
        insd
        db  070h, 06Ch
loc_3_027B:
        jae    NEAR PTR loc_3_029E
        outsd
        and    byte ptr [eax + 6fh], ah
        db  074h, 068h
        jb     NEAR PTR loc_3_02AF
        and    byte ptr [ebp + 65h], ch
        insd
        outsd
        db  072h, 079h
        and    byte ptr [ecx + 72h], ah
        and    byte ptr gs:[ecx + ebp*2 + 73h], ah
        imul   esp, dword ptr [eax], 63h
        popad
        arpl   word ptr [eax + 65h], bp
loc_3_029E:
        and    byte ptr [eax + 61h], dh
loc_3_02A1:
        db  067h, 065h, 073h, 02Ch
        and    byte ptr [ebp + 65h], ch
        insd
        outsd
        db  072h, 079h
        and    byte ptr [ecx + 6ch], ah
loc_3_02AF:
        insb
        outsd
        arpl   word ptr [ecx + 74h], sp
        and    byte ptr fs:[esi + 69h], ah
        db  078h, 065h
loc_3_02BB:
        and    byte ptr fs:[eax], ch
        outsb
        outsd
        outsb
        sub    eax, 65676170h
        popad
        bound  ebp, qword ptr [ebp + 29h]
        sub    al, 20h
        popad
        outsb
        and    byte ptr fs:[ebp + 65h], ch
        insd
        outsd
        db  072h, 079h
        sub    eax, 7070616dh
        and    byte ptr fs:[esi + 69h], ah
        insb
        db  065h, 073h, 02Eh
        add    byte ptr [ebx + 70h], ah
        dec    edi
        je     SHORT loc_3_0354
        jb     NEAR PTR loc_3_02EF
loc_3_02EF:
        inc    ecx
        insb
        insb
        outsd
        arpl   word ptr [ecx + 74h], sp
        and    byte ptr fs:[ebp + 65h], ch
        insd
        outsd
        jb     SHORT loc_3_0378
        add    byte ptr [esi + 75h], cl
        insd
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [edi + 66h], ch
        and    byte ptr [edx + 79h], ah
        db  074h, 065h
        db  073h, 020h
        outsd
        and    byte ptr [ebp + 65h], ch
        insd
        outsd
        db  072h, 079h
        and    byte ptr [ebx + 6fh], ah
        insd
        insd
        imul   esi, dword ptr [esp + esi*2 + 65h], 6e692064h
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebx + 79h], dh
        jae    SHORT loc_3_03A3
        insd
        and    byte ptr cs:[eax], ah
        push   esp
        push   69207369h
        jae    SHORT loc_3_035C
        db  074h, 068h
        and    byte ptr gs:[edi + ebp*2 + 74h], dh
        popad
        insb
        and    byte ptr [ecx + 6dh], ah
        outsd
        jne    SHORT loc_3_03B9
        db  074h, 020h
        outsd
        and    byte ptr [ebp + 65h], ch
        insd
        outsd
loc_3_0354:
        db  072h, 079h
        and    byte ptr [eax + ebp*2 + 61h], dh
        je     SHORT loc_3_037C
loc_3_035C:
        push   62207361h
        outsb
        and    byte ptr [ecx + 6ch], ah
        insb
        outsd
        arpl   word ptr [ecx + 74h], sp
        and    byte ptr fs:[ecx + 6eh], ch
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebx + 79h], dh
loc_3_0378:
        db  073h, 074h
        insd
loc_3_037C:
        sub    al, 20h
        popad
        arpl   word ptr [edx + 6fh], si
        db  073h, 073h
        and    byte ptr [ecx + 6ch], ah
        insb
        and    byte ptr [ebx + 6fh], ah
        insd
        jo     SHORT loc_3_03FD
        outsb
        outsb
        je     SHORT loc_3_0406
        and    byte ptr [ecx + 6eh], ah
        and    byte ptr fs:[ecx + 70h], ah
        db  070h, 06Ch
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
loc_3_03A3:
        db  073h, 02Eh
        add    byte ptr [ebx + 70h], ah
        inc    ebx
        outsd
        insd
        insd
        imul   esi, dword ptr [eax + eax + 50h], 20656761h
        popaw
        jne    SHORT loc_3_0425
loc_3_03B9:
        db  074h, 073h
        add    byte ptr [ebx + 50h], ah
        popad
        inc    esi
        popad
        db  075h, 06Ch
        db  074h, 073h
        add    byte ptr [esi + 75h], cl
        insd
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [edi + 66h], ch
        and    byte ptr [eax + 61h], dh
        and    byte ptr gs:[bp + 61h], ah
        db  075h, 06Ch
        db  074h, 073h
        and    byte ptr [ecx + 6bh], dh
        outsb
        and    byte ptr [eax + 65h], dh
        db  072h, 020h
        jae    SHORT loc_3_044F
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr cs:[ecx + 6eh], cl
        jae    SHORT loc_3_0468
        popad
        outsb
        arpl   word ptr [ebp + 20h], sp
        popaw
        jne    SHORT loc_3_0469
loc_3_03FD:
        db  074h, 073h
        add    byte ptr [ebx + 49h], ah
        outsb
        jae    SHORT loc_3_0479
        popad
loc_3_0406:
        outsb
        arpl   word ptr [ebp + 46h], sp
        popad
        jne    SHORT loc_3_0479
        db  074h, 073h
        add    byte ptr [esi + 75h], cl
        insd
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [edi + 66h], ch
        and    byte ptr [ecx + 6eh], ch
        db  073h, 074h
        popad
        outsb
        arpl   word ptr [ebp + 20h], sp
        popaw
loc_3_0425:
        db  075h, 06Ch
        db  074h, 073h
        and    byte ptr [ecx + 6bh], dh
        outsb
        and    byte ptr [eax + 65h], dh
        jb     SHORT loc_3_0454
        db  073h, 065h
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr cs:[eax + 61h], dl
        sub    eax, 736e69h
        arpl   word ptr [eax + 61h], dx
        dec    ecx
        outsb
        jae    SHORT loc_3_044E
loc_3_044E:
        dec    esi
loc_3_044F:
        jne    SHORT loc_3_04BE
        bound  esp, qword ptr [ebp + 72h]
loc_3_0454:
        and    byte ptr [edi + 66h], ch
        and    byte ptr [eax + 61h], dh
        sub    eax, 6f206e69h
        jo     SHORT loc_3_04C8
        db  072h, 061h
        db  074h, 069h
        outsd
loc_3_0468:
        outsb
loc_3_0469:
        jae    SHORT loc_3_048B
        db  070h, 065h
        db  072h, 020h
        db  073h, 065h
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr cs:[eax + 61h], dl
loc_3_0479:
        sub    eax, 7374756fh
        add    byte ptr [ebx + 50h], ah
        popad
        dec    edi
        db  075h, 074h
        jae    SHORT loc_3_048B
loc_3_048B:
        dec    esi
        db  075h, 06Dh
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [edi + 66h], ch
        and    byte ptr [eax + 61h], dh
        sub    eax, 2074756fh
        outsd
        db  070h, 065h
        db  072h, 061h
        db  074h, 069h
        outsd
        outsb
        jae    SHORT loc_3_04C9
        jo     SHORT loc_3_0510
        db  072h, 020h
        db  073h, 065h
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr cs:[ecx + ebp*2 + 73h], al
        arpl   word ptr [ecx + 72h], sp
        jae    NEAR PTR loc_3_04BE
loc_3_04BE:
        arpl   word ptr [ecx + ebp*2 + 73h], ax
        arpl   word ptr [ecx + 72h], sp
        jae    NEAR PTR loc_3_04C8
loc_3_04C8:
        dec    esi
loc_3_04C9:
        jne    SHORT loc_3_0538
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [edi + 66h], ch
        and    byte ptr [eax + 61h], dh
        and    byte ptr gs:[si + 69h], ah
        db  073h, 063h
        popad
        jb     SHORT loc_3_0542
        jae    SHORT loc_3_0500
        db  070h, 065h
        db  072h, 020h
        db  073h, 065h
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr cs:[ebx + 77h], dl
        popad
        db  070h, 066h
        imul   ebp, dword ptr [ebp + 20h], 657a6973h
        add    byte ptr [ebx + 70h], ah
        push   ebx
        db  077h, 061h
loc_3_0500:
        jo     SHORT loc_3_0568
        imul   ebp, dword ptr [ebp], 657a6953h
        and    byte ptr [edi + 66h], ch
        and    byte ptr [ebx + 77h], dh
loc_3_0510:
        popad
        db  070h, 020h
        imul   bp, word ptr [ebp + 20h], 6e69h
        and    byte ptr [edx + 79h], ah
        db  074h, 065h
        jae    SHORT loc_3_054F
        add    byte ptr [ebx + 77h], dl
        popad
        jo     SHORT loc_3_058D
        imul   ebp, dword ptr [ebp + 20h], 75206e69h
        db  073h, 065h
        add    byte ptr [ebx + 70h], ah
        push   ebx
        ja     SHORT loc_3_0599
loc_3_0538:
        jo     SHORT loc_3_05A0
        imul   ebp, dword ptr [ebp + 49h], 6573556eh
loc_3_0542:
        add    byte ptr [esi + 75h], cl
        insd
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [edi + 66h], ch
        and    byte ptr [edx + 79h], ah
loc_3_054F:
        db  074h, 065h
        db  073h, 020h
        imul   ebp, dword ptr [esi + 20h], 20656874h
        db  073h, 077h
        popad
        db  070h, 020h
        imul   bp, word ptr [ebp + 20h], 7563h
        db  072h, 072h
loc_3_0568:
        outsb
        db  074h, 06Ch
        db  079h, 020h
        imul   ebp, dword ptr [esi + 20h], 2e657375h
        add    byte ptr [ebx + 77h], dl
        popad
        db  070h, 066h
        imul   ebp, dword ptr [ebp + 20h], 65666564h
        arpl   word ptr [ecx + ebp*2 + 76h], si
        add    byte ptr gs:[ebx + 70h], ah
        push   ebx
loc_3_058D:
        db  077h, 061h
        db  070h, 066h
        imul   ebp, dword ptr [ebp + 44h], 63656665h
loc_3_0599:
        db  074h, 069h
        jbe    SHORT loc_3_0602
        add    byte ptr [esi + 75h], cl
loc_3_05A0:
        insd
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [edi + 66h], ch
        and    byte ptr [edx + 79h], ah
        db  074h, 065h
        db  073h, 020h
        imul   ebp, dword ptr [esi + 20h], 20656874h
        db  073h, 077h
        popad
        db  070h, 020h
        imul   bp, word ptr [ebp + 20h], 6f66h
        db  075h, 06Eh
        and    byte ptr fs:[edi + ebp*2 + 20h], dh
        bound  esp, qword ptr [ebp + 20h]
        db  070h, 068h
        db  079h, 073h
        imul   esp, dword ptr [ebx + 61h], 20796c6ch
        arpl   word ptr gs:[ecx + ebp*2 + 76h], si
        and    byte ptr gs:[edi + 6eh], ch
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebx + 77h], dh
        popad
        jo     SHORT loc_3_060C
        insd
        imul   esi, dword ptr fs:[ebp + 6dh], 5320202eh
        db  077h, 061h
        jo     SHORT loc_3_061A
        imul   bp, word ptr [ebp + 20h], 7073h
        popad
loc_3_0602:
        arpl   word ptr [ebp + 20h], sp
        imul   esi, dword ptr [ebx + 20h], 6f6c6c61h
loc_3_060C:
        arpl   word ptr [ecx + 74h], sp
        and    byte ptr fs:[ecx + 6eh], ch
        and    byte ptr [ebx + ecx*2], dh
        and    byte ptr [esi + 72h], ah
loc_3_061A:
        popad
        insd
        jae    NEAR PTR loc_3_065A
        and    byte ptr [ecx + 20h], ah
        jae    SHORT loc_3_068D
        outsb
        insb
        and    byte ptr gs:[ecx + 6dh], ah
        popad
        and    byte ptr fs:[bp + di + 65h], dh
        arpl   word ptr [edi + ebp*2 + 72h], si
        and    byte ptr [ebx + 61h], ah
        db  075h, 073h
        db  065h, 073h, 020h
        db  074h, 068h
        and    byte ptr gs:[ebp + 6eh], ah
        je     SHORT loc_3_06B0
        db  072h, 065h
        and    byte ptr [esi + 72h], ah
        popad
        insd
        and    byte ptr gs:[edi + ebp*2 + 20h], dh
        bound  esp, qword ptr [ebp + 20h]
        insd
        popad
        db  072h, 06Bh
loc_3_065A:
        and    byte ptr fs:[ebp + 66h], ah
        arpl   word ptr gs:[ecx + ebp*2 + 76h], si
        add    byte ptr cs:[ecx + ebp*2 + 73h], al
        imul   esp, dword ptr [eax], 63h
        popad
        arpl   word ptr [eax + 65h], bp
        and    byte ptr [ebx + 69h], dh
        db  07Ah, 065h
        add    byte ptr [ebx + 70h], ah
        inc    esp
        imul   esi, dword ptr [ebx + 6bh], 68636163h
        add    byte ptr gs:[ebx + 75h], al
        db  072h, 072h
        outsb
        je     SHORT loc_3_06AD
loc_3_068D:
        db  073h, 069h
        db  07Ah, 065h
        and    byte ptr [edi + 66h], ch
        and    byte ptr [ecx + ebp*2 + 73h], ah
        imul   esp, dword ptr [eax], 63h
        popad
        arpl   word ptr [eax + 65h], bp
        add    byte ptr cs:[ebp + 69h], cl
        outsb
        imul   ebp, dword ptr [ebp + 75h], 6964206dh
        jae    SHORT loc_3_0718
loc_3_06AD:
        and    byte ptr [ebx + 61h], ah
loc_3_06B0:
        arpl   word ptr [eax + 65h], bp
        and    byte ptr [ebx + 69h], dh
        db  07Ah, 065h
        add    byte ptr [ebx + 70h], ah
        inc    esp
        imul   esi, dword ptr [ebx + 6bh], 68636163h
        dec    ebp
        imul   ebp, dword ptr [esi], 696e694dh
        insd
        db  075h, 06Dh
        and    byte ptr [ecx + ebp*2 + 73h], ah
        imul   esp, dword ptr [eax], 63h
        popad
        arpl   word ptr [eax + 65h], bp
        and    byte ptr [ebx + 69h], dh
        db  07Ah, 065h
        add    byte ptr cs:[ebp + 61h], cl
        db  078h, 069h
        insd
        db  075h, 06Dh
        and    byte ptr [ecx + ebp*2 + 73h], ah
        imul   esp, dword ptr [eax], 63h
        popad
        arpl   word ptr [eax + 65h], bp
        and    byte ptr [ebx + 69h], dh
        db  07Ah, 065h
        add    byte ptr [ebx + 70h], ah
        inc    esp
        imul   esi, dword ptr [ebx + 6bh], 68636163h
        dec    ebp
        popad
        arpl   word ptr [eax], ax
        dec    ebp
        popad
        db  078h, 069h
        insd
        db  075h, 06Dh
        and    byte ptr [ecx + ebp*2 + 73h], ah
        imul   esp, dword ptr [eax], 63h
loc_3_0718:
        popad
        arpl   word ptr [eax + 65h], bp
        and    byte ptr [ebx + 69h], dh
        db  07Ah, 065h
        db  02Eh, 000h
_LMSGTABLE ends
        end
