; drvspacx_mcode_02.asm -- DRVSPACX object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

loc_2_0000:
        or     eax, dword ptr [eax]
        add    byte ptr [eax], al
loc_2_0004:
        and    al, 0
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        xor    al, byte ptr [eax]
        fld    qword ptr [eax]
        xchg   dword ptr [ecx], eax
        and    al, byte ptr [edx]
loc_2_0016:
        lds    eax, ptr [edx]
loc_2_0018:
        salc
        add    bh, byte ptr [ebx]
        add    ecx, dword ptr [esi + 3]
loc_2_001E:
        mov    ecx, 0d503c903h
        add    eax, dword ptr [edx + esi*2 + 69h]
        jbe    SHORT loc_2_008E
        push   ebx
        db  070h, 061h
        arpl   word ptr [ebp + 20h], sp
        inc    ebp
        db  072h, 072h
        outsd
        db  072h, 03Ah
        and    byte ptr [edx + esi*2 + 69h], al
        db  076h, 065h
        and    byte ptr [203a7331h], ah
        sub    byte ptr [5c3a7332h], ah
        inc    esp
        and    eax, 50537333h
        inc    ecx
        inc    ebx
        inc    ebp
        and    eax, 297334h
        inc    ecx
        outsb
        and    byte ptr [ebp + 72h], ah
        db  072h, 06Fh
        db  072h, 020h
        outsd
        arpl   word ptr [ebx + 75h], sp
        jb     SHORT loc_2_00D7
        and    byte ptr fs:[edi + 68h], dh
        imul   ebp, dword ptr [ebp + 20h], 64616572h
        imul   ebp, dword ptr [esi + 67h], 20726f20h
        ja     SHORT loc_2_00ED
        imul   esi, dword ptr [ecx + ebp*2 + 6eh], 68742067h
        and    byte ptr gs:[edx + 69h], al
        db  074h, 046h
        inc    ecx
        push   esp
        and    byte ptr [edi + 6eh], ch
loc_2_008E:
        and    byte ptr [eax + ebp*2 + 69h], dh
        db  073h, 020h
        db  064h, 072h, 069h
        db  076h, 065h
        or     eax, 206f540ah
        imul   di, word ptr [eax + 20h], 6874h
        and    byte ptr gs:[eax + 72h], dh
        outsd
        bound  ebp, qword ptr [ebp + 6dh]
        sub    al, 20h
        db  072h, 075h
        outsb
        and    byte ptr [ebx + 63h], dl
        popad
        outsb
        inc    esp
        imul   esi, dword ptr [ebx + 6bh], 646e6120h
        and    byte ptr [eax + 65h], dh
        db  072h, 066h
        outsd
        jb     SHORT loc_2_0135
        and    byte ptr [ecx + 20h], ah
        je     SHORT loc_2_0135
        outsd
        db  072h, 06Fh
        db  075h, 067h
        push   73657420h
loc_2_00D7:
        je     SHORT loc_2_00F9
        outsd
        or     ax, 640ah
        jb     SHORT loc_2_0149
        db  076h, 065h
        and    byte ptr [61207331h], ah
        outsb
        and    byte ptr fs:[ecx + 74h], ch
loc_2_00ED:
        db  073h, 020h
        push   2074736fh
        jb     NEAR PTR loc_2_0160
        db  076h, 065h
loc_2_00F9:
        and    byte ptr [0d2e7332h], ah
        or     al, byte ptr [eax]
        inc    ecx
        outsb
        and    byte ptr [ebp + 72h], ah
        jb     SHORT loc_2_0177
        db  072h, 020h
        outsd
        arpl   word ptr [ebx + 75h], sp
        db  072h, 072h
        and    byte ptr fs:[edi + 68h], dh
        imul   ebp, dword ptr [ebp + 20h], 64616572h
        imul   ebp, dword ptr [esi + 67h], 20726f20h
        db  077h, 072h
        imul   esi, dword ptr [ecx + ebp*2 + 6eh], 68742067h
        and    byte ptr gs:[ebp + 44h], cl
        inc    esi
        inc    ecx
        push   esp
loc_2_0135:
        and    byte ptr [edi + 6eh], ch
        and    byte ptr [eax + ebp*2 + 69h], dh
        db  073h, 020h
        db  064h, 072h, 069h
        db  076h, 065h
        or     eax, 206f540ah
loc_2_0149:
        imul   di, word ptr [eax + 20h], 6874h
        and    byte ptr gs:[eax + 72h], dh
        outsd
        bound  ebp, qword ptr [ebp + 6dh]
        sub    al, 20h
        jb     SHORT loc_2_01D1
        outsb
        and    byte ptr [ebx + 63h], dl
loc_2_0160:
        popad
        outsb
        inc    esp
        imul   esi, dword ptr [ebx + 6bh], 646e6120h
        and    byte ptr [eax + 65h], dh
        db  072h, 066h
        outsd
        jb     SHORT loc_2_01DF
        and    byte ptr [ecx + 20h], ah
        je     SHORT loc_2_01DF
loc_2_0177:
        outsd
        jb     SHORT loc_2_01E9
        jne    SHORT loc_2_01E3
        push   73657420h
        je     SHORT loc_2_01A3
        outsd
        or     ax, 640ah
        db  072h, 069h
        jbe    SHORT loc_2_01F1
        and    byte ptr [61207331h], ah
        outsb
        and    byte ptr fs:[ecx + 74h], ch
        db  073h, 020h
        push   2074736fh
        jb     NEAR PTR loc_2_020A
        db  076h, 065h
loc_2_01A3:
        and    byte ptr [0d2e7332h], ah
        or     al, byte ptr [eax]
        push   esp
        push   20657265h
        popad
        jb     SHORT loc_2_0219
        and    byte ptr [ecx + 6eh], ch
        arpl   word ptr [edi + 6eh], bp
        db  073h, 069h
        db  073h, 074h
        outsb
        arpl   word ptr [ecx + 65h], bp
        db  073h, 020h
        imul   ebp, dword ptr [esi + 20h], 20656874h
        dec    ebp
        inc    esp
        inc    esi
        inc    ecx
        push   esp
loc_2_01D1:
        and    byte ptr [edi + 6eh], ch
        and    byte ptr [eax + ebp*2 + 69h], dh
        jae    SHORT loc_2_01FA
        jb     NEAR PTR loc_2_0246
        db  076h, 065h
loc_2_01DF:
        and    byte ptr cs:[eax], ah
        push   esp
loc_2_01E3:
        outsd
        and    byte ptr [esi + 69h], ah
        db  078h, 020h
loc_2_01E9:
        je     SHORT loc_2_0253
        or     eax, 6f72700ah
loc_2_01F1:
        bound  ebp, qword ptr [ebp + 6dh]
        sub    al, 20h
        db  072h, 075h
        outsb
loc_2_01FA:
        and    byte ptr [ebx + 63h], dl
        popad
        outsb
        inc    esp
        imul   esi, dword ptr [ebx + 6bh], 646e6120h
        and    byte ptr [eax + 65h], dh
loc_2_020A:
        jb     SHORT loc_2_0272
        outsd
        db  072h, 06Dh
        and    byte ptr [ecx + 20h], ah
        db  074h, 068h
        outsd
        db  072h, 06Fh
        db  075h, 067h
loc_2_0219:
        push   73657420h
        je     SHORT loc_2_0240
        outsd
        and    byte ptr [edx + esi*2 + 69h], ah
        db  076h, 065h
        and    byte ptr [61207331h], ah
        outsb
        or     eax, 7374690ah
        and    byte ptr [eax + 6fh], ch
        jae    SHORT loc_2_02AE
        and    byte ptr [edx + esi*2 + 69h], ah
        db  076h, 065h
loc_2_0240:
        and    byte ptr [2e7332h], ah
loc_2_0246:
        inc    ecx
        outsb
        and    byte ptr [ebp + 72h], ah
        db  072h, 06Fh
        db  072h, 020h
        outsd
        arpl   word ptr [ebx + 75h], sp
loc_2_0253:
        db  072h, 072h
        and    byte ptr fs:[edi + 68h], dh
        imul   ebp, dword ptr [ebp + 20h], 74697277h
        imul   ebp, dword ptr [esi + 67h], 6e6f6320h
        db  074h, 072h
        outsd
        insb
        and    byte ptr [ecx + 74h], ah
        popad
loc_2_0272:
        and    byte ptr [edi + ebp*2 + 20h], dh
        db  074h, 068h
        imul   esi, dword ptr [ebx + 20h], 76697264h
        and    byte ptr cs:[eax], ah
        push   esp
        outsd
        or     eax, 7869660ah
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [eax + 72h], dh
        outsd
        bound  ebp, qword ptr [ebp + 6dh]
        sub    al, 20h
        jb     SHORT loc_2_030F
        outsb
        and    byte ptr [ebx + 63h], dl
        popad
        outsb
        inc    esp
        imul   esi, dword ptr [ebx + 6bh], 646e6120h
        and    byte ptr [eax + 65h], dh
        db  072h, 066h
        outsd
loc_2_02AE:
        db  072h, 06Dh
        and    byte ptr [ecx + 20h], ah
        db  074h, 068h
        outsd
        jb     SHORT loc_2_0327
        db  075h, 067h
        push   73657420h
        je     SHORT loc_2_02E1
        outsd
        or     ax, 640ah
        db  072h, 069h
        jbe    SHORT loc_2_032F
        and    byte ptr [61207331h], ah
        outsb
        and    byte ptr fs:[ecx + 74h], ch
        jae    SHORT loc_2_02F7
        push   2074736fh
        db  064h, 072h, 069h
        db  076h, 065h
loc_2_02E1:
        and    byte ptr [0d2e7332h], ah
        or     al, byte ptr [eax]
        inc    esp
        db  072h, 069h
        jbe    SHORT loc_2_0353
        push   ebx
        jo     SHORT loc_2_0352
        arpl   word ptr [ebp + 20h], sp
        inc    ebp
        db  072h, 072h
loc_2_02F7:
        outsd
        jb     SHORT loc_2_02FA
loc_2_02FA:
        push   esp
        push   72442065h
        imul   esi, dword ptr [esi + 65h], 63617053h
        and    byte ptr gs:[esi + 78h], dl
        inc    esp
        and    byte ptr [eax + 61h], ch
loc_2_030F:
        db  073h, 020h
        outsd
        jbe    SHORT loc_2_0379
        jb     SHORT loc_2_037C
        insb
        outsd
        db  077h, 065h
        and    byte ptr fs:[ecx + 74h], ch
        jae    SHORT loc_2_0340
        imul   ebp, dword ptr [esi + 74h], 616e7265h
loc_2_0327:
        insb
        and    byte ptr [ebx + 74h], dh
        popad
        arpl   word ptr [ebx + 2eh], bp
loc_2_032F:
        or     eax, 6265520ah
        outsd
        outsd
        db  074h, 02Ch
        and    byte ptr [edx + 75h], dh
        outsb
        and    byte ptr [ebx + 63h], dl
        popad
loc_2_0340:
        outsb
        inc    esp
        imul   esi, dword ptr [ebx + 6bh], 6e61202ch
        and    byte ptr fs:[ebx + 68h], ah
        arpl   word ptr gs:[ebx + 20h], bp
        popad
loc_2_0352:
        insb
loc_2_0353:
        insb
        and    byte ptr [edx + esi*2 + 69h], ah
        jbe    SHORT loc_2_03BF
        db  073h, 02Eh
        or     eax, 7244000ah
        imul   esi, dword ptr [esi + 65h], 63617053h
        and    byte ptr gs:[edi + 61h], dl
        db  072h, 06Eh
        imul   ebp, dword ptr [esi + 67h], 65685400h
        and    byte ptr [edx + esi*2 + 69h], al
loc_2_0379:
        jbe    SHORT loc_2_03E0
        push   ebx
loc_2_037C:
        jo     SHORT loc_2_03DF
        arpl   word ptr [ebp + 20h], sp
        push   esi
        db  078h, 044h
        and    byte ptr [ecx + 6eh], ah
        and    byte ptr fs:[eax + ebp*2 + 65h], dh
        and    byte ptr [edx + esi*2 + 69h], al
        jbe    SHORT loc_2_03F7
        push   ebx
        jo     SHORT loc_2_03F6
        arpl   word ptr [ebp + 20h], sp
        db  072h, 065h
        popad
        insb
        and    byte ptr [ebp + 6fh], ch
        and    byte ptr gs:[edx + esi*2 + 69h], ah
        db  076h, 065h
        db  072h, 00Dh
        or     ah, byte ptr [ecx + 72h]
        and    byte ptr gs:[ebp + 69h], ch
        db  073h, 06Dh
        popad
        db  074h, 063h
        push   202e6465h
        and    byte ptr [ecx + 6fh], bl
        jne    SHORT loc_2_03DF
loc_2_03BF:
        insd
        popad
        jns    SHORT loc_2_03E3
        outsb
        and    byte ptr fs:[edi + ebp*2 + 20h], dh
        db  072h, 065h
        imul   ebp, dword ptr [esi + 73h], 6c6c6174h
        and    byte ptr [eax + ebp*2 + 65h], dh
        insd
        or     eax, 3a41000ah
loc_2_03DF:
        pop    esp
loc_2_03E0:
        push   edx
        inc    ebp
        inc    ecx
loc_2_03E3:
        inc    esp
        push   esp
        dec    eax
        dec    ecx
        push   ebx
        push   esp
        pop    eax
        push   esp
        add    byte ptr [edx + 45h], dl
        inc    ecx
        inc    esp
        push   esp
        dec    eax
        dec    ecx
        push   ebx
        push   esp
loc_2_03F6:
        pop    eax
loc_2_03F7:
        push   esp
        db  000h
_LMSGTABLE ends
        end
