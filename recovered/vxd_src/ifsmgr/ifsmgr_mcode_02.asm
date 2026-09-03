; ifsmgr_mcode_02.asm -- IFSMGR object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  00Ch, 000h, 000h, 000h
loc_2_0004:
        sub    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_2_000C:
        add    byte ptr [eax], al
loc_2_000E:
        jg     SHORT loc_2_0010
loc_2_0010:
        cmp    al, byte ptr [ecx]
loc_2_0012:
        mov    esp, 0f4024401h
        add    ch, byte ptr [ebp + 47040003h]
        add    al, 99h
        add    al, 0feh
        add    al, 1bh
        add    eax, 608h
loc_2_0028:
        push   esp
        push   6f762065h
        insb
        jne    SHORT loc_2_009E
        and    byte ptr gs:[eax + ebp*2 + 61h], dh
        db  074h, 020h
        ja     SHORT loc_2_009B
        db  073h, 020h
        db  072h, 065h
        insd
        outsd
        db  076h, 065h
        and    byte ptr fs:[eax + 61h], ch
        and    byte ptr fs:[edi + 70h], ch
        outsb
        and    byte ptr [6c69660ah], cl
        db  065h, 073h, 020h
        outsd
        outsb
        and    byte ptr [ecx + 74h], ch
        and    byte ptr cs:[esi + 65h], cl
        js     SHORT loc_2_00D4
        and    byte ptr [ecx + ebp*2 + 6dh], dh
        and    byte ptr gs:[eax + 6ch], dh
        popad
        db  073h, 065h
        and    byte ptr [ebx + 68h], ah
        arpl   word ptr gs:[ebx + 20h], bp
        imul   si, word ptr [edx + 73h], 2074h
        or     eax, 206f740ah
        db  073h, 065h
        and    byte ptr gs:[ecx + 66h], ch
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [esi + 6fh], dh
        insb
        jne    SHORT loc_2_00FB
        and    byte ptr gs:[ebx + 61h], ah
        outsb
        and    byte ptr [edx + 65h], dh
        popad
        insb
        insb
        jns    SHORT loc_2_00BB
loc_2_009B:
        bound  esp, qword ptr [ebp + 20h]
loc_2_009E:
        db  072h, 065h
        insd
        outsd
        jbe    SHORT loc_2_0109
        add    byte ptr cs:[eax + ebp*2 + 65h], dl
        and    byte ptr [esi + 69h], ah
        insb
        and    byte ptr gs:[ebx + 79h], dh
        db  073h, 074h
        insd
        and    byte ptr [ebp + 6fh], ch
        db  075h, 06Eh
loc_2_00BB:
        db  074h, 065h
        and    byte ptr fs:[edi + 6eh], ch
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [esi + 6fh], dh
        insb
        db  075h, 06Dh
        and    byte ptr gs:[eax + 61h], ch
        and    byte ptr fs:[ecx + 74h], ah
loc_2_00D4:
        popad
        and    byte ptr [6168740ah], cl
        db  074h, 020h
        outsb
        and    byte ptr fs:[edi + ebp*2 + 20h], dh
        bound  esp, qword ptr [ebp + 20h]
        insb
        db  075h, 073h
        push   202e6465h
        push   ebx
        outsd
        insd
        and    byte ptr gs:[ecx + 74h], ah
loc_2_00FB:
        popad
        and    byte ptr [edi + 6eh], ch
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [6c6f760ah], cl
loc_2_0109:
        jne    SHORT loc_2_0178
        and    byte ptr gs:[edi + 61h], dh
        db  073h, 020h
        insb
        outsd
        db  073h, 074h
        and    byte ptr cs:[esi + 65h], cl
        db  078h, 074h
        and    byte ptr [ecx + ebp*2 + 6dh], dh
        and    byte ptr gs:[eax + 6ch], dh
        popad
        jae    SHORT loc_2_018C
        and    byte ptr [ebx + 68h], ah
        arpl   word ptr gs:[ebx + 20h], bp
        imul   si, word ptr [edx + 73h], 2074h
        or     eax, 206f740ah
        db  073h, 065h
        and    byte ptr gs:[ecx + 66h], ch
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [esi + 6fh], dh
        insb
        jne    SHORT loc_2_01B6
        and    byte ptr gs:[ebx + 61h], ah
        outsb
        and    byte ptr [edx + 65h], dh
        popad
        insb
        insb
        jns    SHORT loc_2_0176
        bound  esp, qword ptr [ebp + 20h]
        db  072h, 065h
        insd
        outsd
        db  076h, 065h
        add    byte ptr cs:[eax + ebp*2 + 65h], dl
        and    byte ptr [ebp + 69h], cl
        arpl   word ptr [edx + 6fh], si
        db  073h, 06Fh
        je     NEAR PTR loc_2_0190
        dec    ecx
        outsb
        db  073h, 074h
        popad
        insb
loc_2_0176:
        insb
        popad
loc_2_0178:
        bound  ebp, qword ptr [ebp + 20h]
        inc    esi
        imul   ebp, dword ptr [ebp + 20h], 74737953h
        insd
        and    byte ptr [ebp + 61h], cl
        outsb
        popad
loc_2_018C:
        jb     NEAR PTR loc_2_01B0
loc_2_0190:
        arpl   word ptr [ecx + 6eh], sp
        outsb
        outsd
        db  074h, 020h
        imul   bp, word ptr [esi + 64h], 7420h
        push   65682065h
        insb
        db  070h, 065h
        jb     SHORT loc_2_01C7
        or     eax, 6972640ah
        db  076h, 065h
        jb     SHORT loc_2_01DE
loc_2_01B0:
        and    byte ptr [eax], ah
        push   eax
        insb
        popad
loc_2_01B6:
        jae    SHORT loc_2_021D
        and    byte ptr [ebp + 6eh], ah
        db  073h, 075h
        db  072h, 065h
        and    byte ptr [eax + ebp*2 + 61h], dh
        db  074h, 020h
        dec    ecx
        inc    esi
loc_2_01C7:
        push   ebx
        dec    eax
        dec    esp
        push   eax
        push   ebx
        pop    ecx
        push   ebx
        and    byte ptr [eax + 61h], ch
        jae    SHORT loc_2_01F4
        bound  esp, qword ptr [ebp + 65h]
        outsb
        and    byte ptr [ecx + 6eh], ch
        jae    SHORT loc_2_0251
        popad
loc_2_01DE:
        insb
        insb
        add    byte ptr cs:[eax + ebp*2 + 65h], dl
        and    byte ptr [ebp + 69h], cl
        arpl   word ptr [edx + 6fh], si
        db  073h, 06Fh
        je     NEAR PTR loc_2_0212
        dec    ecx
        outsb
loc_2_01F4:
        db  073h, 074h
        popad
        insb
        insb
        popad
        bound  ebp, qword ptr [ebp + 20h]
        inc    esi
        imul   ebp, dword ptr [ebp + 20h], 74737953h
        insd
        and    byte ptr [ebp + 61h], cl
        outsb
        popad
        db  067h, 065h, 072h, 020h
loc_2_0212:
        arpl   word ptr [ecx + 6eh], sp
        outsb
        outsd
        db  074h, 020h
        arpl   word ptr [edi + 6eh], bp
        outsb
loc_2_021D:
        arpl   word ptr gs:[eax + 74h], si
        outsd
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [eax + 65h], ch
        insb
        db  070h, 065h
        jb     SHORT loc_2_024F
        or     eax, 6972640ah
        db  076h, 065h
        jb     SHORT loc_2_0266
        and    byte ptr [eax], ah
        push   eax
        insb
        popad
        jae    SHORT loc_2_02A5
        and    byte ptr [ebp + 6eh], ah
        db  073h, 075h
        jb     SHORT loc_2_02AC
        and    byte ptr [eax + ebp*2 + 61h], dh
        db  074h, 020h
        dec    ecx
        inc    esi
loc_2_024F:
        push   ebx
        dec    eax
loc_2_0251:
        dec    esp
        push   eax
        push   ebx
        pop    ecx
        push   ebx
        and    byte ptr [eax + 61h], ch
        jae    SHORT loc_2_027C
        bound  esp, qword ptr [ebp + 65h]
        outsb
        and    byte ptr [ecx + 6eh], ch
        db  073h, 074h
        popad
loc_2_0266:
        insb
        insb
        add    byte ptr cs:[eax + ebp*2 + 65h], dl
        and    byte ptr [ebp + 69h], cl
        arpl   word ptr [edx + 6fh], si
        jae    SHORT loc_2_02E6
        je     NEAR PTR loc_2_029A
        dec    ecx
        outsb
loc_2_027C:
        db  073h, 074h
        popad
        insb
        insb
        popad
        bound  ebp, qword ptr [ebp + 20h]
        inc    esi
        imul   ebp, dword ptr [ebp + 20h], 74737953h
        insd
        and    byte ptr [ebp + 61h], cl
        outsb
        popad
        db  067h, 065h, 072h, 020h
loc_2_029A:
        outsw
        jne    SHORT loc_2_030C
        and    byte ptr fs:[ecx + 6eh], ah
        and    byte ptr [ecx + 6eh], ch
loc_2_02A5:
        arpl   word ptr [edi + 6dh], bp
        db  070h, 061h
        je     SHORT loc_2_0315
loc_2_02AC:
        bound  ebp, qword ptr [ebp + 20h]
        jbe    SHORT loc_2_0317
        jb     SHORT loc_2_0327
        imul   ebp, dword ptr [edi + 6eh], 6f0a0d20h
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [eax + 65h], ch
        insb
        jo     SHORT loc_2_032B
        db  072h, 020h
        db  064h, 072h, 069h
        jbe    SHORT loc_2_0332
        db  072h, 02Eh
        and    byte ptr [eax], ah
        push   eax
        insb
        popad
        db  073h, 065h
        and    byte ptr [ebp + 6eh], ah
        db  073h, 075h
        db  072h, 065h
        and    byte ptr [eax + ebp*2 + 61h], dh
        db  074h, 020h
        db  074h, 068h
loc_2_02E6:
        and    byte ptr gs:[ebx + 6fh], ah
        db  072h, 072h
        arpl   word ptr gs:[eax + 76h], si
        jb     NEAR PTR loc_2_0367
        imul   ebp, dword ptr [edi + 6eh], 20666f20h
        or     eax, 5346490ah
        dec    eax
        dec    esp
        push   eax
        push   ebx
        pop    ecx
        push   ebx
        and    byte ptr [eax + 61h], ch
        jae    SHORT loc_2_032C
loc_2_030C:
        bound  esp, qword ptr [ebp + 65h]
        outsb
        and    byte ptr [ecx + 6eh], ch
        db  073h, 074h
loc_2_0315:
        popad
        insb
loc_2_0317:
        insb
        add    byte ptr cs:[eax + ebp*2 + 65h], dl
        and    byte ptr [ebp + 69h], cl
        arpl   word ptr [edx + 6fh], si
        jae    SHORT loc_2_0396
loc_2_0327:
        je     NEAR PTR loc_2_034A
        dec    ecx
loc_2_032B:
        outsb
loc_2_032C:
        jae    SHORT loc_2_03A2
        popad
        insb
        insb
        popad
loc_2_0332:
        bound  ebp, qword ptr [ebp + 20h]
        inc    esi
        imul   ebp, dword ptr [ebp + 20h], 74737953h
        insd
        and    byte ptr [ebp + 61h], cl
        outsb
        popad
        db  067h, 065h, 072h, 020h
loc_2_034A:
        or     eax, 756f630ah
        insb
        and    byte ptr fs:[esi + 6fh], ch
        db  074h, 020h
        imul   ebp, dword ptr [esi + 73h], 6c6c6174h
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [eax + 65h], ch
        insb
        db  070h, 065h
loc_2_0367:
        db  072h, 020h
        db  064h, 072h, 069h
        db  076h, 065h
        jb     SHORT loc_2_0390
        push   736b6f6fh
        and    byte ptr cs:[656c500ah], cl
        popad
        jae    SHORT loc_2_03E4
        and    byte ptr [ebp + 6eh], ah
        db  073h, 075h
        jb     SHORT loc_2_03EB
        and    byte ptr [eax + ebp*2 + 61h], dh
        je     SHORT loc_2_03AC
        dec    ecx
        inc    esi
        push   ebx
        dec    eax
loc_2_0390:
        dec    esp
        push   eax
        push   ebx
        pop    ecx
        push   ebx
loc_2_0396:
        and    byte ptr [eax + 61h], ch
        db  073h, 020h
        bound  esp, qword ptr [ebp + 65h]
        outsb
        and    byte ptr [ecx + 6eh], ch
loc_2_03A2:
        db  073h, 074h
        popad
        insb
        insb
        and    byte ptr fs:[ecx + 6eh], ah
loc_2_03AC:
        and    byte ptr fs:[eax + ebp*2 + 65h], dh
        and    byte ptr [edx + 65h], dh
        popad
        insb
        and    byte ptr [ebp + 6fh], ch
        and    byte ptr gs:[74656e0ah], cl
        db  077h, 06Fh
        db  072h, 06Bh
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [esi + 6fh], ch
        db  074h, 020h
        insb
        outsd
        popad
        add    byte ptr cs:[eax + ebp*2 + 65h], dl
        and    byte ptr [ebp + 69h], cl
        arpl   word ptr [edx + 6fh], si
        db  073h, 06Fh
        je     NEAR PTR loc_2_0403
        dec    ecx
loc_2_03E4:
        outsb
        db  073h, 074h
        popad
        insb
        insb
        popad
loc_2_03EB:
        bound  ebp, qword ptr [ebp + 20h]
        inc    esi
        imul   ebp, dword ptr [ebp + 20h], 74737953h
        insd
        and    byte ptr [ebp + 61h], cl
        outsb
        popad
        db  067h, 065h, 072h, 020h
loc_2_0403:
        or     eax, 6e61630ah
        outsb
        outsd
        db  074h, 020h
        jo     SHORT loc_2_0480
        outsd
        arpl   word ptr [ebp + 73h], sp
        db  073h, 020h
        db  074h, 068h
        and    byte ptr gs:[ebx + 6fh], ah
        and    byte ptr gs:[eax + 61h], dh
        and    byte ptr gs:[bp + 69h], ah
        insb
        add    byte ptr cs:[eax + ebp*2 + 65h], dl
        and    byte ptr [esi + 6fh], dh
        insb
        db  075h, 06Dh
        and    byte ptr gs:[eax + 61h], ch
        jae    SHORT loc_2_0457
        bound  esp, qword ptr [ebp + 65h]
        outsb
        and    byte ptr [edi + ebp*2 + 63h], ch
        imul   esp, dword ptr [ebp + 64h], 20h
        bound  edi, qword ptr [ecx + 20h]
        popad
        outsb
        outsd
        je     SHORT loc_2_04B3
        db  065h, 072h, 020h
        db  070h, 072h
        outsd
        arpl   word ptr [ebp + 73h], sp
        jae    SHORT loc_2_0476
        popad
loc_2_0457:
        outsb
        and    byte ptr fs:[ebx + 61h], ah
        outsb
        outsb
        outsd
        db  074h, 020h
        bound  esp, qword ptr [ebp + 20h]
        popad
        arpl   word ptr [ebx + 65h], sp
        db  073h, 073h
        or     eax, 6854000ah
        and    byte ptr gs:[esi + 6fh], dh
        insb
loc_2_0476:
        jne    SHORT loc_2_04E5
        and    byte ptr gs:[edi + ebp*2 + 63h], ch
        imul   esp, dword ptr [eax], 6fh
loc_2_0480:
        db  072h, 020h
        db  075h, 06Eh
        insb
        outsd
        arpl   word ptr [ebx + 20h], bp
        outsd
        db  070h, 065h
        jb     SHORT loc_2_04EF
        db  074h, 069h
        outsd
        outsb
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [eax + 72h], dh
        outsd
        arpl   word ptr [ebp + 73h], sp
        db  073h, 020h
        imul   esi, dword ptr [ebx + 20h], 69797274h
        outsb
        and    byte ptr [si + 6fh], dh
        and    byte ptr [eax + 65h], dh
        jb     SHORT loc_2_0516
        outsd
        jb     SHORT loc_2_0520
loc_2_04B3:
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [ecx + 6eh], ch
        jbe    SHORT loc_2_051C
        insb
        imul   esp, dword ptr [ebp + ecx + 0ah], 65685400h
        and    byte ptr [esi + 6fh], dh
        insb
        db  075h, 06Dh
        and    byte ptr gs:[edi + ebp*2 + 63h], ch
        imul   esp, dword ptr [eax], 68h
        popad
        db  073h, 020h
        bound  esp, qword ptr [ebp + 65h]
        outsb
        and    byte ptr [esi + 61h], ah
        imul   ebp, dword ptr [ebp + 64h], 20796220h
        popad
loc_2_04E5:
        outsb
        outsd
        db  074h, 068h
        db  065h, 072h, 020h
        popad
        db  070h, 070h
loc_2_04EF:
        insb
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        and    byte ptr [edi + 72h], ch
        and    byte ptr [edx + esi*2 + 69h], ah
        db  076h, 065h
        db  072h, 00Dh
        or     ch, byte ptr [edi + 72h]
        and    byte ptr [edx + 65h], ah
        arpl   word ptr [ecx + 75h], sp
        jae    SHORT loc_2_0572
        and    byte ptr [eax + ebp*2 + 65h], dh
        jb     SHORT loc_2_0578
        and    byte ptr [ecx + 72h], ah
loc_2_0516:
        and    byte ptr gs:[edi + 70h], ch
        outsb
loc_2_051C:
        and    byte ptr [esi + 69h], ah
        insb
loc_2_0520:
        db  065h, 073h, 02Eh
        or     eax, 6c4f000ah
        and    byte ptr fs:[ecx + ebp*2 + 73h], al
        imul   esp, dword ptr [eax], 55h
        je     SHORT loc_2_059B
        insb
        imul   esi, dword ptr [ecx + edi*2 + 20h], 74697257h
        and    byte ptr gs:[ebp + 72h], al
        jb     SHORT loc_2_05B0
        jb     SHORT loc_2_0543
loc_2_0543:
        pop    ecx
        outsd
        db  075h, 020h
        popad
        db  072h, 065h
        and    byte ptr [ebp + 73h], dh
        imul   ebp, dword ptr [esi + 67h], 64206120h
        imul   esi, dword ptr [ebx + 6bh], 69747520h
        insb
        imul   esi, dword ptr [ecx + edi*2 + 20h], 69736564h
        outsb
        and    byte ptr fs:[esi + 6fh], ah
        db  072h, 020h
        popad
        outsb
        and    byte ptr [edi + 6ch], ch
loc_2_0572:
        jb     NEAR PTR loc_2_0596
        db  076h, 065h
loc_2_0578:
        db  072h, 073h
        imul   ebp, dword ptr [edi + 6eh], 20666f20h
        dec    ebp
        push   ebx
        sub    eax, 20534f44h
        outsd
        db  072h, 020h
        push   edi
        imul   ebp, dword ptr [esi + 64h], 2e73776fh
        or     bl, byte ptr [ecx + 6fh]
loc_2_0596:
        db  075h, 072h
        and    byte ptr [esi + 69h], ah
loc_2_059B:
        insb
        jae    NEAR PTR loc_2_05BF
        insd
        popad
        db  079h, 020h
        bound  esp, qword ptr [ebp + 20h]
        popad
        insd
        popad
        and    byte ptr fs:[bx + di + 66h], ch
loc_2_05B0:
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebp + 74h], dh
        imul   ebp, dword ptr [ecx + ebp*2 + 74h], 72772079h
loc_2_05BF:
        imul   esi, dword ptr [ebp + 73h], 206f7420h
        db  074h, 068h
        and    byte ptr gs:[ecx + ebp*2 + 73h], ah
        imul   ebp, dword ptr [esi], 20h
        push   esp
        outsd
        and    byte ptr [edx + 75h], dh
        outsb
        and    byte ptr [ecx + 74h], ch
        sub    al, 20h
        bound  esi, qword ptr [ebp + 74h]
        or     dh, byte ptr [eax + 72h]
        db  065h, 076h, 065h
        outsb
        je     SHORT loc_2_0608
        imul   esi, dword ptr [eax + 66h], 206d6f72h
        db  077h, 072h
        imul   esi, dword ptr [ecx + ebp*2 + 6eh], 6f742067h
        and    byte ptr [ecx + 6fh], bh
        db  075h, 072h
        and    byte ptr [ecx + ebp*2 + 73h], ah
        imul   ebp, dword ptr [eax], 63h
        insb
loc_2_0608:
        imul   esp, dword ptr [ebx + 6bh], 73655920h
        and    byte ptr cs:[edi + ebp*2 + 20h], dl
        db  073h, 074h
        outsd
        db  070h, 020h
        db  074h, 068h
        and    byte ptr gs:[ebp + 74h], dh
        imul   ebp, dword ptr [ecx + ebp*2 + 74h], 63202c79h
        insb
        imul   esp, dword ptr [ebx + 6bh], 2e6f4e20h
        db  000h
_LMSGTABLE ends
        end
