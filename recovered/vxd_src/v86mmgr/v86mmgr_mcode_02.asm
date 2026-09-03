; v86mmgr_mcode_02.asm -- V86MMGR object 2 segment _LMSGTABLE
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
        pushad
        add    byte ptr [eax - 6bfe6a00h], bl
        add    bl, al
        add    al, byte ptr [ebp + 52h]
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        push   ebp
        outsb
        jae    SHORT loc_2_0098
        jo     SHORT loc_2_0095
        outsd
        db  072h, 074h
        and    byte ptr fs:[ebp + 78h], ah
        db  070h, 061h
        outsb
        sub    eax, 6f6d656dh
        jb     SHORT loc_2_00B3
        and    byte ptr [edx + esi*2 + 69h], ah
        db  076h, 065h
        db  072h, 020h
        popad
        insb
        jb     SHORT loc_2_00AB
        popad
        jns    NEAR PTR loc_2_006A
        imul   ebp, dword ptr [esi + 73h], 6c6c6174h
        and    byte ptr cs:[edx + 65h], dl
        insd
        outsd
        db  076h, 065h
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [edx + esi*2 + 69h], ah
        db  076h, 065h
        db  072h, 020h
        jb     NEAR PTR loc_2_00D9
loc_2_006A:
        insd
        and    byte ptr [ebx + 4fh], al
        dec    esi
        inc    esi
        dec    ecx
        inc    edi
        push   ebx
        pop    ecx
        push   ebx
        add    byte ptr cs:[ebp + 52h], al
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        inc    ebx
        popad
        outsb
        outsb
        outsd
        je     SHORT loc_2_00A6
        imul   ebp, dword ptr [esi + 73h], 6c6c6174h
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebx], dh
        cmp    byte ptr [esi], dh
loc_2_0095:
        and    byte ptr [ebp + 6eh], ah
loc_2_0098:
        push   65636e61h
        and    byte ptr fs:[ebp + 6fh], ch
        and    byte ptr gs:[eax + 4dh], bl
loc_2_00A6:
        push   ebx
        and    byte ptr [edx + esi*2 + 69h], ah
loc_2_00AB:
        db  076h, 065h
        jb     SHORT loc_2_00DD
        add    byte ptr [edi + 69h], dl
        outsb
loc_2_00B3:
        outsd
        db  077h, 073h
        and    byte ptr [ebx + 61h], ah
        outsb
        outsb
        outsd
        je     SHORT loc_2_00DF
        jae    SHORT loc_2_0126
        je     SHORT loc_2_00E3
        db  075h, 070h
        and    byte ptr [ecx + 6eh], ah
        and    byte ptr [ebp + 70h], dh
        jo     SHORT loc_2_0132
        jb     SHORT loc_2_00EF
        insd
        insd
        outsd
        db  072h, 079h
        and    byte ptr [edx + 6ch], ah
        outsd
loc_2_00D9:
        arpl   word ptr [ebx + 20h], bp
        popad
loc_2_00DD:
        db  074h, 020h
loc_2_00DF:
        jae    SHORT loc_2_0146
        insd
loc_2_00E3:
        outsb
        je     SHORT loc_2_0107
        and    eax, 0a0d2e78h
        inc    ebp
        db  078h, 063h
loc_2_00EF:
        insb
        jne    SHORT loc_2_0156
        and    byte ptr gs:[eax + ebp*2 + 69h], dh
        jae    SHORT loc_2_0119
        popad
        db  064h, 064h, 072h, 065h
        jae    SHORT loc_2_0173
        and    byte ptr [ebx + 70h], dh
        popad
        arpl   word ptr [ebp + 20h], sp
loc_2_0107:
        bound  edi, qword ptr [ecx + 20h]
        jne    SHORT loc_2_017F
        imul   ebp, dword ptr [esi + 67h], 65687420h
        and    byte ptr [ebx + 79h], dh
        outsb
        db  074h, 061h
loc_2_0119:
        db  078h, 020h
        outsd
        and    byte ptr [ecx + 6fh], bh
        db  075h, 072h
        and    byte ptr [ebp + 65h], ch
        insd
loc_2_0126:
        outsd
        jb     SHORT loc_2_01A2
        and    byte ptr [ebp + 61h], ch
        outsb
        popad
        db  067h, 065h, 072h, 02Eh
loc_2_0132:
        or     eax, 726f460ah
        and    byte ptr [ebp + 6fh], ch
        db  072h, 065h
        and    byte ptr [ecx + 6eh], ch
        outsw
        jb     SHORT loc_2_01B0
        popad
        db  074h, 069h
loc_2_0146:
        outsd
        outsb
        and    byte ptr [ebx + 65h], dh
        and    byte ptr gs:[eax + ebp*2 + 65h], dh
        and    byte ptr [edx + 45h], dl
        inc    ecx
        inc    esp
        dec    ebp
loc_2_0156:
        inc    ebp
        push   edi
        push   edx
        dec    ecx
        and    byte ptr [esi + 69h], ah
        insb
        and    byte ptr cs:[ecx + edi*2 + 70h], dl
        and    byte ptr gs:[edi + 49h], dl
        dec    esi
        and    byte ptr [edi], ch
        push   ebx
        and    byte ptr [edi + ebp*2 + 20h], dh
        db  073h, 074h
loc_2_0173:
        popad
        jb     SHORT loc_2_01EA
        or     eax, 6e69570ah
        outsd
        db  077h, 073h
loc_2_017F:
        and    byte ptr [ecx + 6eh], ch
        and    byte ptr [ebx + 74h], dh
        popad
        outsb
        popad
        db  072h, 064h
        and    byte ptr [ebp + 6fh], ch
        and    byte ptr gs:[ecx + 6eh], ah
        and    byte ptr fs:[ebx + 68h], ah
        outsd
        outsd
        db  073h, 065h
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [edx + 65h], dl
loc_2_01A2:
        popad
        and    byte ptr fs:[ebp + 65h], cl
        and    byte ptr [ecx + 63h], ch
        outsd
        outsb
        add    byte ptr cs:[edi + 69h], dl
loc_2_01B0:
        outsb
        outsd
        db  077h, 073h
        and    byte ptr [ebx + 61h], ah
        outsb
        outsb
        outsd
        je     SHORT loc_2_01DD
        jae    SHORT loc_2_0224
        je     SHORT loc_2_01E1
        db  075h, 070h
        and    byte ptr [ecx + 6eh], ah
        and    byte ptr [ebp + 70h], dh
        jo     SHORT loc_2_0230
        jb     SHORT loc_2_01ED
        insd
        insd
        outsd
        db  072h, 079h
        and    byte ptr [edx + 6ch], ah
        outsd
        arpl   word ptr [ebx + 20h], bp
        popad
        db  074h, 020h
loc_2_01DD:
        jae    SHORT loc_2_0244
        insd
loc_2_01E1:
        outsb
        je     SHORT loc_2_0205
        and    eax, 0a0d2e78h
loc_2_01EA:
        inc    ebp
        db  078h, 063h
loc_2_01ED:
        insb
        jne    SHORT loc_2_0254
        and    byte ptr gs:[eax + ebp*2 + 69h], dh
        jae    SHORT loc_2_0217
        popad
        db  064h, 064h, 072h, 065h
        jae    SHORT loc_2_0271
        and    byte ptr [ebx + 70h], dh
        popad
        arpl   word ptr [ebp + 20h], sp
loc_2_0205:
        bound  edi, qword ptr [ecx + 20h]
        jne    SHORT loc_2_027D
        imul   ebp, dword ptr [esi + 67h], 65687420h
        and    byte ptr [ebx + 79h], dh
        outsb
        db  074h, 061h
loc_2_0217:
        db  078h, 020h
        outsd
        and    byte ptr [ecx + 6fh], bh
        db  075h, 072h
        and    byte ptr [ebp + 65h], ch
        insd
loc_2_0224:
        outsd
        jb     SHORT loc_2_02A0
        and    byte ptr [ebp + 61h], ch
        outsb
        popad
        db  067h, 065h, 072h, 02Eh
loc_2_0230:
        or     eax, 726f460ah
        and    byte ptr [ebp + 6fh], ch
        db  072h, 065h
        and    byte ptr [ecx + 6eh], ch
        outsw
        jb     SHORT loc_2_02AE
        popad
        db  074h, 069h
loc_2_0244:
        outsd
        outsb
        and    byte ptr [ebx + 65h], dh
        and    byte ptr gs:[eax + ebp*2 + 65h], dh
        and    byte ptr [edx + 45h], dl
        inc    ecx
        inc    esp
        dec    ebp
loc_2_0254:
        inc    ebp
        push   edi
        push   edx
        dec    ecx
        and    byte ptr [esi + 69h], ah
        insb
        and    byte ptr cs:[ecx + edi*2 + 70h], dl
        and    byte ptr gs:[edi + 49h], dl
        dec    esi
        and    byte ptr [edi], ch
        push   ebx
        and    byte ptr [edi + ebp*2 + 20h], dh
        jae    SHORT loc_2_02E5
loc_2_0271:
        popad
        db  072h, 074h
        or     eax, 6e69570ah
        outsd
        db  077h, 073h
loc_2_027D:
        and    byte ptr [ecx + 6eh], ch
        and    byte ptr [ebx + 74h], dh
        popad
        outsb
        popad
        jb     SHORT loc_2_02ED
        and    byte ptr [ebp + 6fh], ch
        and    byte ptr gs:[ecx + 6eh], ah
        and    byte ptr fs:[ebx + 68h], ah
        outsd
        outsd
        db  073h, 065h
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [edx + 65h], dl
loc_2_02A0:
        popad
        and    byte ptr fs:[ebp + 65h], cl
        and    byte ptr [ecx + 63h], ch
        outsd
        outsb
        add    byte ptr cs:[ebp + 52h], al
loc_2_02AE:
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        inc    ebx
        popad
        outsb
        outsb
        outsd
        je     SHORT loc_2_02DA
        imul   ebp, dword ptr [esi + 73h], 6c6c6174h
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebx], dh
        cmp    byte ptr [esi], dh
        and    byte ptr [ebp + 6eh], ah
        push   65636e61h
        and    byte ptr fs:[ebp + 6fh], ch
        and    byte ptr gs:[ebp + 78h], ah
loc_2_02DA:
        db  070h, 061h
        outsb
        sub    eax, 6f6d656dh
loc_2_02E5:
        db  072h, 079h
        and    byte ptr [edx + esi*2 + 69h], ah
        db  076h, 065h
loc_2_02ED:
        db  072h, 02Eh
        db  000h
_LMSGTABLE ends
        end
