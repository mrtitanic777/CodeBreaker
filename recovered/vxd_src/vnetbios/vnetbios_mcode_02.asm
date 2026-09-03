; vnetbios_mcode_02.asm -- VNETBIOS object 2 segment _LMSGTABLE
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
        mov    al, byte ptr [22017d00h]
        db  002h, 0BEh, 003h, 005h, 004h
loc_2_0018:
        push   esp
        push   61207369h
        db  070h, 070h
        insb
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        and    byte ptr [ebx + 68h], dh
        outsd
        db  075h, 06Ch
        and    byte ptr fs:[esi + 6fh], ch
        je     SHORT loc_2_0054
        bound  esp, qword ptr [ebp + 20h]
        db  074h, 065h
        db  072h, 06Dh
        imul   ebp, dword ptr [esi + 61h], 20646574h
        bound  esp, qword ptr [ebp + 63h]
        popad
        jne    SHORT loc_2_00BB
        and    byte ptr gs:[ecx + 74h], ch
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [ebx + 75h], ah
        db  072h, 072h
loc_2_0054:
        outsb
        db  074h, 06Ch
        jns    SHORT loc_2_007A
        arpl   word ptr [edi + 6dh], bp
        insd
        db  075h, 06Eh
        imul   esp, dword ptr [ebx + 61h], 676e6974h
        and    byte ptr [edi + 6eh], ch
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [esi + 65h], ch
        db  074h, 077h
        outsd
        jb     SHORT loc_2_00E1
        and    byte ptr cs:[ecx + 6fh], bl
loc_2_007A:
        db  075h, 020h
        jae    SHORT loc_2_00E6
        outsd
        db  075h, 06Ch
        and    byte ptr fs:[edx + esi*2 + 79h], dh
        and    byte ptr [edi + ebp*2 + 20h], dh
        db  071h, 075h
        imul   esi, dword ptr [eax + 74h], 20736968h
        popad
        jo     SHORT loc_2_0107
        insb
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        and    byte ptr [ebp + 73h], dh
        imul   ebp, dword ptr [esi + 67h], 73746920h
        and    byte ptr [ebp + 78h], ah
        imul   esi, dword ptr [eax + 63h], 616d6d6fh
        outsb
        add    byte ptr cs:[eax + ebp*2 + 69h], dl
loc_2_00BB:
        jae    SHORT loc_2_00DD
        popad
        db  070h, 070h
        insb
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        and    byte ptr [edi + 61h], dh
        db  073h, 020h
        arpl   word ptr [edi + 6dh], bp
        insd
        db  075h, 06Eh
        imul   esp, dword ptr [ebx + 61h], 676e6974h
        and    byte ptr [edi + 6eh], ch
loc_2_00DD:
        and    byte ptr [eax + ebp*2 + 65h], dh
loc_2_00E1:
        and    byte ptr [esi + 65h], ch
        db  074h, 077h
loc_2_00E6:
        outsd
        db  072h, 06Bh
        and    byte ptr [edi + 68h], dh
        outsb
        and    byte ptr [ecx + 6fh], bh
        db  075h, 020h
        je     SHORT loc_2_015A
        jb     SHORT loc_2_0164
        imul   ebp, dword ptr [esi + 61h], 20646574h
        imul   esi, dword ptr [esi + ebp + 20h], 68746c41h
        outsd
loc_2_0107:
        db  075h, 067h
        push   6e695720h
        outsd
        ja     SHORT loc_2_0185
        and    byte ptr [ecx + 74h], ah
        je     SHORT loc_2_017C
        insd
        jo     SHORT loc_2_018E
        and    byte ptr fs:[edi + ebp*2 + 20h], dh
        db  072h, 065h
        jae    SHORT loc_2_0198
        outsd
        db  072h, 065h
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebx + 74h], dh
        popad
        db  074h, 065h
        and    byte ptr [edi + 66h], ch
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [esi + 65h], ch
        db  074h, 077h
        outsd
        db  072h, 06Bh
        sub    al, 20h
        db  079h, 06Fh
        db  075h, 020h
        insd
        popad
        db  079h, 020h
        push   20657661h
        jo     SHORT loc_2_01C3
        outsd
        bound  ebp, qword ptr [ebp + 6dh]
        jae    SHORT loc_2_0178
        ja     SHORT loc_2_01C3
loc_2_015A:
        je     SHORT loc_2_01C4
        and    byte ptr [esi + 65h], ch
        db  074h, 077h
        outsd
        db  072h, 06Bh
loc_2_0164:
        and    byte ptr [ebx + 6fh], ah
        insd
        insd
        db  075h, 06Eh
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        jae    SHORT loc_2_0194
        jne    SHORT loc_2_01E4
        db  074h, 069h
loc_2_0178:
        insb
        and    byte ptr [ecx + 6fh], bh
loc_2_017C:
        jne    SHORT loc_2_019E
        db  072h, 065h
        db  073h, 074h
        popad
        jb     SHORT loc_2_01F9
loc_2_0185:
        and    byte ptr [ecx + 6fh], bh
        jne    SHORT loc_2_01FC
        and    byte ptr [ebx + 6fh], ah
        insd
loc_2_018E:
        db  070h, 075h
        je     SHORT loc_2_01F7
        db  072h, 02Eh
loc_2_0194:
        add    byte ptr [eax + ebp*2 + 69h], dl
loc_2_0198:
        jae    SHORT loc_2_01BA
        popad
        jo     SHORT loc_2_020D
        insb
loc_2_019E:
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        and    byte ptr [edi + 61h], dh
        db  073h, 020h
        arpl   word ptr [edi + 6dh], bp
        insd
        db  075h, 06Eh
        imul   esp, dword ptr [ebx + 61h], 676e6974h
        and    byte ptr [edi + 6eh], ch
loc_2_01BA:
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [esi + 65h], ch
        db  074h, 077h
loc_2_01C3:
        outsd
loc_2_01C4:
        db  072h, 06Bh
        and    byte ptr [edi + 68h], dh
        outsb
        and    byte ptr [ecx + 6fh], bh
        db  075h, 020h
        je     SHORT loc_2_0237
        db  072h, 06Dh
        imul   ebp, dword ptr [esi + 61h], 20646574h
        imul   esi, dword ptr [esi + ebp + 20h], 646e6957h
        outsd
loc_2_01E4:
        db  077h, 073h
        and    byte ptr [edi + 61h], dh
        db  073h, 020h
        db  075h, 06Eh
        popad
        bound  ebp, qword ptr [ebp + 20h]
        db  074h, 06Fh
        and    byte ptr [edx + 65h], dh
loc_2_01F7:
        db  073h, 074h
loc_2_01F9:
        outsd
        db  072h, 065h
loc_2_01FC:
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebx + 74h], dh
        popad
        je     SHORT loc_2_026B
        and    byte ptr [edi + 66h], ch
        and    byte ptr [eax + ebp*2 + 65h], dh
loc_2_020D:
        and    byte ptr [esi + 65h], ch
        db  074h, 077h
        outsd
        jb     SHORT loc_2_0280
        and    byte ptr cs:[ecx + 6fh], bl
        db  075h, 020h
        outsb
        and    byte ptr fs:[edi + ebp*2 + 20h], dh
        db  072h, 065h
        db  073h, 074h
        popad
        db  072h, 074h
        and    byte ptr [ecx + 6fh], bh
        jne    SHORT loc_2_02A1
        and    byte ptr [ebx + 6fh], ah
        insd
        jo     SHORT loc_2_02AA
        db  074h, 065h
loc_2_0237:
        db  072h, 02Eh
        add    byte ptr [eax + ebp*2 + 69h], dl
        db  073h, 020h
        popad
        db  070h, 070h
        insb
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        and    byte ptr [edx + 65h], dh
        db  071h, 075h
        imul   esi, dword ptr [edx + 65h], 20612073h
        insb
        popad
        jb     SHORT loc_2_02C1
        db  065h, 072h, 020h
        bound  esi, qword ptr [ebp + 66h]
        db  066h, 065h, 072h, 020h
        outsw
        jb     SHORT loc_2_0288
        db  074h, 072h
        popad
loc_2_026B:
        outsb
        jae    SHORT loc_2_02D4
        jb     NEAR PTR loc_2_02E3
        imul   ebp, dword ptr [esi + 67h], 666e6920h
        outsd
        db  072h, 06Dh
        popad
        je     SHORT loc_2_02E7
        outsd
        outsb
loc_2_0280:
        and    byte ptr [edi + 76h], ch
        db  065h, 072h, 020h
        db  074h, 068h
loc_2_0288:
        and    byte ptr gs:[esi + 65h], ch
        db  074h, 077h
        outsd
        db  072h, 06Bh
        and    byte ptr cs:[ecx + 6fh], bl
        db  075h, 020h
        arpl   word ptr [ecx + 6eh], sp
        and    byte ptr [ecx + 6eh], ch
        arpl   word ptr [edx + 65h], si
        popad
loc_2_02A1:
        db  073h, 065h
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [edx + 75h], ah
loc_2_02AA:
        db  066h, 066h, 065h, 072h, 020h
        db  073h, 069h
        jp     SHORT loc_2_0318
        and    byte ptr [edx + 79h], ah
        and    byte ptr [ebp + 6fh], ch
        imul   esp, dword ptr fs:[esi + 79h], 20676e69h
loc_2_02C1:
        db  074h, 068h
        and    byte ptr gs:[esi + 65h], cl
        je     SHORT loc_2_0311
        popad
        db  070h, 053h
        imul   edi, dword ptr [edx + 65h], 74657320h
loc_2_02D4:
        je     SHORT loc_2_033F
        outsb
        and    byte ptr [bx + di + 6eh], ch
        and    byte ptr [ecx + 6fh], bh
        db  075h, 072h
        and    byte ptr [ebx + 59h], dl
loc_2_02E3:
        push   ebx
        push   esp
        inc    ebp
        dec    ebp
loc_2_02E7:
        dec    ecx
        dec    esi
        dec    ecx
        and    byte ptr [esi + 69h], ah
        insb
        or     eax, 6e490a0ah
        arpl   word ptr [ebp + esi*2 + 64h], bp
        and    byte ptr gs:[edi + 72h], ch
        and    byte ptr [ebp + 6fh], ch
        imul   esp, dword ptr fs:[esi + 79h], 65687420h
        and    byte ptr [esi + 6fh], ah
        insb
        insb
        outsd
        ja     SHORT loc_2_037A
loc_2_0311:
        outsb
        and    byte ptr [bp + di + 65h], dh
        je     SHORT loc_2_038C
loc_2_0318:
        imul   ebp, dword ptr [esi + 67h], 206e6920h
        db  074h, 068h
        and    byte ptr gs:[ebx + 33h], bl
        cmp    byte ptr [esi], dh
        inc    ebp
        outsb
        push   6573205dh
        arpl   word ptr [ecx + ebp*2 + 6fh], si
        outsb
        and    byte ptr [edi + 66h], ch
        and    byte ptr [ecx + 6fh], bh
        db  075h, 072h
        and    byte ptr [ebx + 59h], dl
        push   ebx
loc_2_033F:
        push   esp
        inc    ebp
        dec    ebp
        dec    ecx
        dec    esi
        dec    ecx
        and    byte ptr [esi + 69h], ah
        insb
        cmp    cl, byte ptr gs:[20200a0ah]
        and    byte ptr [eax], ah
        dec    esi
        je     NEAR PTR loc_2_039F
        popad
        db  070h, 053h
        imul   edi, dword ptr [edx + 65h], 0d73253dh
        or     cl, byte ptr [edx]
        dec    ecx
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [esi + 65h], cl
        db  074h, 048h
        popad
        db  070h, 053h
        imul   edi, dword ptr [edx + 65h], 6c617620h
loc_2_037A:
        jne    SHORT loc_2_03E1
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [ebx + 74h], dh
        imul   ebp, dword ptr [esp + ebp*2 + 20h], 206f6f74h
        insb
        outsd
loc_2_038C:
        db  077h, 02Ch
        and    byte ptr [eax + ebp*2 + 69h], dh
        db  073h, 020h
        insd
        jae    NEAR PTR loc_2_040B
        popad
        and    byte ptr gs:[bx + 69h], dh
        insb
loc_2_039F:
        insb
        and    byte ptr [ecx + 70h], ah
        db  070h, 065h
        popad
        jb     SHORT loc_2_03C8
        popad
        popad
        imul   ebp, dword ptr [esi + 2ch], 67757320h
        db  067h, 065h, 073h, 074h
        imul   ebp, dword ptr [esi + 67h], 6e206120h
        db  065h, 077h, 020h
        db  076h, 061h
        insb
        db  075h, 065h
        and    byte ptr [esi + 6fh], ah
loc_2_03C8:
        jb     SHORT loc_2_03EA
        db  079h, 06Fh
        db  075h, 020h
        db  074h, 06Fh
        and    byte ptr [edx + esi*2 + 79h], dh
        add    byte ptr cs:[eax + ebp*2 + 69h], dl
        db  073h, 020h
        db  070h, 072h
        outsd
        db  067h, 072h, 061h
loc_2_03E1:
        insd
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [ebx + 61h], ah
        db  075h, 073h
loc_2_03EA:
        imul   ebp, dword ptr [esi + 67h], 756f7920h
        db  072h, 020h
        db  073h, 079h
        db  073h, 074h
        insd
        and    byte ptr [edi + ebp*2 + 20h], dh
        insb
        outsd
        arpl   word ptr [ebx + 20h], bp
        db  075h, 070h
        and    byte ptr cs:[edi + ebp*2 + 20h], al
        db  079h, 06Fh
loc_2_040B:
        db  075h, 020h
        db  077h, 061h
        outsb
        db  074h, 020h
        db  074h, 06Fh
        and    byte ptr [ebp + 6eh], ah
        and    byte ptr fs:[ecx + 74h], ch
        aas
        db  000h
_LMSGTABLE ends
        end
